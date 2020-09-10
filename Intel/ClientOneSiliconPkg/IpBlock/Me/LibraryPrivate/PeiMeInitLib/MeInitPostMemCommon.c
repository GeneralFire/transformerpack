/** @file
  The PEI Library Implements ME Init.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include <Ppi/SiPolicy.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiMeLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/HeciInitLib.h>
#include <Library/MeShowBufferLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeFwStsLib.h>
#include <Library/PsfLib.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PchInfoLib.h>
#include <Library/TimerLib.h>
#include <Library/PmcLib.h>
#include <Register/HeciRegs.h>
#include <Register/MeRegs.h>
#include <AmtConfig.h>
#include <MeFwHob.h>
#include <MeBiosPayloadHob.h>
#include "Library/MeTypeLib.h"
#include "MeInitPostMem.h"
#include "MbpData.h"
#include <Guid/MeFirmwareVersionInfoHobGuid.h>

#define ME_FORMAT_FILE_TIMEOUT   180 // 3 minutes for formating file system and restoring data timeout in seconds

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mMeEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) MeOnEndOfPei
};

/**
  Check if Firmware requests the platform to invoke Data Resiliency flow

  @retval TRUE               CSME Data Resiliency is requested
  @retvak FALSE              CSME Data Resiliency is NOT requested
**/
BOOLEAN
IsDataResiliencyRequired (
  VOID
  )
{
  UINT64                   HeciBaseAddress;
  HECI_GS_SHDW_REGISTER    MeFwSts2;

  HeciBaseAddress = PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0);
  if (PciSegmentRead32 (HeciBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
    return FALSE;
  }
  MeFwSts2.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_HFS_2);
  if (MeFwSts2.r.MfsFailure == 0) {
    return FALSE;
  }

  return TRUE;
}

/**
  Invoke data clear for CSME
  If data clear command fails, BIOS should continue to boot;
  else BIOS send CSE reset and wait for CSE to finish data restore and trigger global reset.
**/
VOID
InvokeDataResiliency (
  VOID
  )
{
  EFI_STATUS    Status;
  UINT8         Timeout;

  DEBUG ((DEBUG_INFO, "%a () enter\n", __FUNCTION__));

  Status = PeiHeciDataClear ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Data Clear fail, Status = %r\n", Status));
    return;
  }

  PeiHeciSendCbmResetRequest (CBM_RR_REQ_ORIGIN_BIOS_POST, CBM_HRR_CSME_RESET);
  //
  // CSME will trigger global reset after data restore succeeds.
  //
  Timeout = ME_FORMAT_FILE_TIMEOUT;
  DEBUG ((DEBUG_INFO, "Waiting for formating file system and restoring data to start"));
  do {
    MicroSecondDelay (ME_STATE_STALL_1_SECOND);
    DEBUG ((DEBUG_INFO, "."));
  } while (Timeout--);

  DEBUG ((DEBUG_WARN, "\n Status polling timeout, CSME should trigger global reset."));
  CpuDeadLoop ();
}

/**
  Query if ME operation mode is Temp Disable mode.

  @retval TRUE            ME is in Temp Disable mode.
  @retval FALSE           ME is not in Temp Disable mode.
**/
BOOLEAN
IsMeInTempDisabledMode (
  VOID
  )
{
  HECI_FWS_REGISTER        MeFirmwareStatus;
  UINTN                    Timeout;
  UINT64                   HeciBaseAddress;

  HeciBaseAddress = PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0);
  if (PciSegmentRead16 (HeciBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    return FALSE;
  }

  Timeout = MSG_MAX_WAIT_TIMEOUT;
  do {
    //
    // Read ME status and check for operation mode
    //
    MeFirmwareStatus.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_HFS);
    if (MeFirmwareStatus.r.MeOperationMode == ME_OPERATION_MODE_SOFT_TEMP_DISABLE) {
      return TRUE;
    }

    MicroSecondDelay (ME_STATE_STALL_1_SECOND);
    Timeout--;
  } while (Timeout > 0);

  DEBUG ((DEBUG_INFO, "Timeout: ME not in temp disabled mode after 5s. MeFirmwareStatus: %08x.\n", MeFirmwareStatus.ul));
  return FALSE;
}

/**
  Check if MCTP (Management Component Transport) Protocol is supported by CSME and BIOS.

  @param[in] MbpHob                   Pointer to MbpHob
  @param[in] MePeiConfig              Pointer to ME PEI Config

  @retval    TRUE                     MCTP is supported by CSME or BIOS
  @retval    FALSE                    MCTP is not supported by CSME or BIOS
**/
BOOLEAN
IsMctpConfigurationSupported (
  IN ME_BIOS_PAYLOAD_HOB *MbpHob,
  IN ME_PEI_CONFIG       *MePeiConfig
  )
{
  EFI_STATUS             Status;
  MEFWCAPS_SKU           FwCapsSku;

  if (MePeiConfig->MctpBroadcastCycle == 0) {
    return FALSE;
  }

  if (MeTypeIsSps ()) {
    return TRUE;
  }

  if (MeTypeIsClient ()) {
    if (MbpHob != NULL) {
      if ((MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt == 1) &&
          (MbpHob->MeBiosPayload.FwFeaturesState.FwFeatures.Fields.FullMng == 1))
        return TRUE;
    } else {
      //
      // When MBP is not available (e.g S3 resume), we need
      // to retrieve Feature Capabilities and State from CSME
      //
      Status = PeiHeciGetFwCapsSkuMsg (&FwCapsSku);
      if (!EFI_ERROR (Status) && FwCapsSku.Fields.Amt == 1) {
        Status = PeiHeciGetFwFeatureStateMsg (&FwCapsSku);
        if (!EFI_ERROR (Status) && FwCapsSku.Fields.FullMng == 1) {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

/**
  Wait until CSME updates Operation Mode.

  @retval EFI_SUCCESS             Operation mode is updated.
  @retval EFI_DEVICE_ERROR        CSME device error.
  @retval EFI_TIMEOUT             Timeout exceeded.
**/
EFI_STATUS
WaitForOpModeUpdate (
  VOID
  )
{
  HECI_FWS_REGISTER        MeFirmwareStatus;
  UINTN                    Timeout;
  UINT64                   HeciBaseAddress;

  HeciBaseAddress = PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0);
  if (PciSegmentRead16 (HeciBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    return EFI_DEVICE_ERROR;
  }

  Timeout = MSG_MAX_WAIT_TIMEOUT * 10;
  do {
    ///
    /// Read ME status and check for operation mode
    ///
    MeFirmwareStatus.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_HFS);
    if (MeFirmwareStatus.r.MeOperationMode != ME_OPERATION_MODE_NORMAL) {
      return EFI_SUCCESS;
    }

    MicroSecondDelay (ME_STATE_STALL_1_SECOND / 10);
    Timeout--;
  } while (Timeout > 0);

  DEBUG ((DEBUG_ERROR, "Timeout: CSME is in NORMAL mode after 5s. MeFirmwareStatus: %08x.\n", MeFirmwareStatus.ul));
  return EFI_TIMEOUT;
}

/**
  Save FWSTS to ME FWSTS HOB, if the HOB is not existing, the HOB will be created and publish.
  If the HOB is existing, the data will be overrided.
**/
VOID
SaveFwStsToHob (
  VOID
  )
{
  ME_FW_HOB              *MeFwHob;
  UINT8                  Index;
  UINT8                  Count;
  EFI_STATUS             Status;
  HECI_GS_SHDW_REGISTER  Fwsts2;

  if (PmcIsDwrBootMode () == TRUE) {
    DEBUG ((DEBUG_INFO, "DWR detected : ME FW HOB not installed\n"));
    return;
  }

  MeFwHob = GetFirstGuidHob (&gMeFwHobGuid);
  if (MeFwHob == NULL) {
    //
    // Create HOB for ME Data
    //
    Status = PeiServicesCreateHob (
               EFI_HOB_TYPE_GUID_EXTENSION,
               sizeof (ME_FW_HOB),
               (VOID **) &MeFwHob
               );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return;
    }

    DEBUG ((DEBUG_INFO, "ME FW HOB installed\n"));

    //
    // Initialize default HOB data
    //
    ZeroMem (&(MeFwHob->Revision), (sizeof (ME_FW_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
    MeFwHob->EfiHobGuidType.Name = gMeFwHobGuid;
    MeFwHob->Revision = 1;
    MeFwHob->Count = GetMeFwStsDeviceCount ();
  }

  Fwsts2.ul = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_ME_HFS_2));
  if (MeTypeIsClient () && Fwsts2.r.CseToDisabled) {
    //
    // Waiting for FWSTS to update the operation mode if CSE is on the way to be disabled.
    //
    WaitForOpModeUpdate ();
  }

  DEBUG ((DEBUG_INFO, "ME FW HOB data updated\n"));
  //
  // Save the FWSTS registers set for each MEI device.
  //
  for (Count = 0; Count < GetMeFwStsDeviceCount (); Count++) {
    for (Index = 0; Index < GetMeFwStsOffsetCount (); Index++) {
      MeFwHob->Group[Count].Reg[Index] = PciSegmentRead32 (
                                           PCI_SEGMENT_LIB_ADDRESS (
                                             ME_SEGMENT,
                                             ME_BUS,
                                             ME_DEVICE_NUMBER,
                                             (UINT32)gFwStsDeviceList[Count].HeciDev,
                                             gFwStsOffsetTable[Index]
                                             ));
    }
    MeFwHob->Group[Count].FunNumber = (UINT32)gFwStsDeviceList[Count].HeciDev;
  }

  DEBUG_CODE_BEGIN ();
  DEBUG ((DEBUG_INFO, "Current ME FW HOB data printed - \n"));
  ShowBuffer ((UINT8 *) MeFwHob, sizeof (ME_FW_HOB));
  DEBUG_CODE_END ();
}

/**
  Save all policies in PEI phase to HOB.
**/
VOID
SavePolicyToHob (
  VOID
  )
{
  EFI_STATUS                Status;
  SI_POLICY_PPI             *SiPolicyPpi;
  SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi;
  ME_PEI_PREMEM_CONFIG      *MePeiPreMemConfig;
  ME_PEI_CONFIG             *MePeiConfig;
#if defined (ME_WS_SUPPORT) || (FixedPcdGetBool (PcdAmtEnable) == 1)
  AMT_PEI_CONFIG            *AmtPeiConfig;
#endif
  UINTN                     HobSize;
  VOID                      *HobPtr;

  SiPreMemPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  SiPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);

#if defined (ME_WS_SUPPORT) || (FixedPcdGetBool (PcdAmtEnable) == 1)
  AmtPeiConfig = NULL;
  if (MeTypeIsClient ()) {
    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gAmtPeiConfigGuid, (VOID *) &AmtPeiConfig);
  }
#endif
  ASSERT_EFI_ERROR (Status);

  //
  // Create ME/AMT HOBs.
  //
  HobSize = MePeiPreMemConfig->Header.GuidHob.Header.HobLength;
  DEBUG ((DEBUG_INFO, "HobSize for MePeiPreMemConfig: %x\n", HobSize));
  HobPtr = BuildGuidDataHob (&gMePreMemPolicyHobGuid, MePeiPreMemConfig, HobSize);
  ASSERT (HobPtr != 0);

  HobSize = MePeiConfig->Header.GuidHob.Header.HobLength;
  DEBUG ((DEBUG_INFO, "HobSize for MePeiConfig: %x\n", HobSize));
  HobPtr = BuildGuidDataHob (&gMePolicyHobGuid, MePeiConfig, HobSize);
  ASSERT (HobPtr != 0);

#if defined (ME_WS_SUPPORT) || (FixedPcdGetBool(PcdAmtEnable) == 1)
  if (MeTypeIsClient () && (AmtPeiConfig != NULL)) {
    HobSize = AmtPeiConfig->Header.GuidHob.Header.HobLength;
    DEBUG ((DEBUG_INFO, "HobSize for AmtPeiConfig: %x\n", HobSize));
    HobPtr = BuildGuidDataHob (&gAmtPolicyHobGuid, AmtPeiConfig, HobSize);
    ASSERT (HobPtr != 0);
  }
#endif // ME_WS_SUPPORT

  BuildSpsGuidDataHob (SiPreMemPolicyPpi);
}

/**
  Send Optional Heci Command.
**/
VOID
SendOptionalHeciCommand (
  VOID
  )
{
  EFI_STATUS                Status;
  SI_POLICY_PPI             *SiPolicyPpi;
  ME_PEI_CONFIG             *MePeiConfig;
  ME_BIOS_PAYLOAD_HOB       *MbpHob;
  UINT32                    UnconfigOnRtcClear;

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  SiPolicyPpi = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate gSiPolicyPpiGuid fail. Status = %r\n", Status));
    return;
  }
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get ME PeiConfigBlock fail, Status = %r\n", Status));
    return;
  }

  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob != NULL && MbpHob->MeBiosPayload.UnconfigOnRtcClearState.Available == 1) {
    UnconfigOnRtcClear = MbpHob->MeBiosPayload.UnconfigOnRtcClearState.UnconfigOnRtcClearData.DisUnconfigOnRtcClearState;
    if ((MePeiConfig->MeUnconfigOnRtcClear == 1) && (UnconfigOnRtcClear == DisableState)) {
      UnconfigOnRtcClear = EnableState;
    } else if ((MePeiConfig->MeUnconfigOnRtcClear == 0) && (UnconfigOnRtcClear == EnableState)) {
      UnconfigOnRtcClear = DisableState;
    } else {
      ///
      /// Platform policy same as feature state retrieved from MBP. No update needed.
      ///
      return;
    }
    Status = PeiHeciSetUnconfigOnRtcClearDisableMsg (UnconfigOnRtcClear);
  }
}

/**
 Configure ME device interrupts

 @param[in] SiPolicy      The Silicon Policy PPI instance
 @param[in] MeDev         The CSME device to be configured.
**/
STATIC
VOID
MeDeviceConfigureInterrupts (
  IN CONST SI_POLICY_PPI  *SiPolicy,
  IN ME_DEVICE            MeDev
  )
{
  UINT64      PciBaseAddress;
  UINT8       Response;
  UINT32      Data32;
  EFI_STATUS  Status;
  UINT8       FunctionNum;
  UINT8       InterruptPin;

  DEBUG ((DEBUG_INFO, "%a () for MeDev = %d\n", __FUNCTION__, MeDev));

  FunctionNum = HECI_FUNCTION_NUMBER + (UINT8)MeDev;
  PciBaseAddress = PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, FunctionNum, 0);

  if (PciSegmentRead16 (PciBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "%a () exit (Device disabled)\n", __FUNCTION__));
    return;
  }

  //
  // Configure ME device interrupts
  //
  InterruptPin = ItssGetDevIntPin (SiPolicy, ME_DEVICE_NUMBER, FunctionNum);

  switch (MeDev) {
    case HECI1:
    case HECI2:
    case HECI3:
    case HECI4:
    case IDER:
      PciSegmentWrite8 (PciBaseAddress + PCI_INT_PIN_OFFSET, InterruptPin);
      break;
    case SOL:
      Data32 = InterruptPin;
      Status = PchSbiExecutionEx (
                 PID_CSME12,
                 0,
                 PrivateControlWrite,
                 FALSE,
                 0x000F,
                 0x0000,
                 (ME_DEVICE_NUMBER << 3) + SOL_FUNCTION_NUMBER,
                 &Data32,
                 &Response
                 );
      ASSERT_EFI_ERROR (Status);
      Data32 = 0;
      Status = PchSbiExecutionEx (
                 PID_CSME12,
                 0,
                 PrivateControlRead,
                 FALSE,
                 0x000F,
                 0x0000,
                 (ME_DEVICE_NUMBER << 3) + SOL_FUNCTION_NUMBER,
                 &Data32,
                 &Response
                 );
      ASSERT_EFI_ERROR (Status);
      break;
  }
}

/**
  This function performs basic initialization for ME in PEI phase after memory is initialized.

  @param[in] SiPolicy     The Silicon Policy PPI instance

**/
VOID
MePostMemInit (
  IN  SI_POLICY_PPI  *SiPolicy
  )
{
  EFI_STATUS                Status;
  ME_BIOS_PAYLOAD_HOB       *MbpHob;
  ME_PEI_CONFIG             *MePeiConfig;
  UINT8                     MeDevFunction;

  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));

  if (!MeTypeIsDfx ()) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gMePeiConfigGuid, (VOID *) &MePeiConfig);
    ASSERT_EFI_ERROR (Status);

    MbpHob = NULL;

    //
    // Install Mbp in POST Mem unless it's S3 resume boot.
    //
    if (GetBootModeHob () != BOOT_ON_S3_RESUME) {
      Status = InstallMbpHob ();
      MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
    }

    //
    // Configure CSME devices
    //
    Status = MeDeviceConfigure (IsHeci3Supported (MbpHob));
    ASSERT_EFI_ERROR (Status);

    //
    // Configure MCTP
    //
    MctpConfigure (MbpHob, MePeiConfig);

    //
    // Configure interrupts for CSME devices
    //
    for (MeDevFunction = HECI1; MeDevFunction <= HECI4; MeDevFunction++) {
      MeDeviceConfigureInterrupts (SiPolicy, MeDevFunction);
    }
  }

  ///
  /// Check if CSME data resiliency is required before Core BIOS Done.
  ///
  if (IsDataResiliencyRequired ()) {
    InvokeDataResiliency ();
  }

  PeiServicesNotifyPpi (&mMeEndOfPeiNotifyList);

  DEBUG ((DEBUG_INFO, "%a () - End\n", __FUNCTION__));
}

/**
  Create (if not exists) ME_FIRMWARE_VERSION_HOB

  @retval EFI_SUCCESS        HOB has been created/updated successfully
  @retval EFI_DEVICE_ERROR   Can't create HOB
  @retval EFI_UNSUPPORTED    ME doesn't support MkhiVersion
**/
EFI_STATUS
CreateMkhiVersionHob (
  VOID
  )
{
  EFI_STATUS               Status;
  EFI_HOB_GUID_TYPE        *GuidHob;
  ME_FIRMWARE_VERSION_HOB  MeFirmwareHob;
  VOID                     *MeFirmwareHobPtr;

  DEBUG ((DEBUG_INFO, "[ME] %d ()\n", __FUNCTION__));

  if (!MeTypeIsClient ()) {
    return EFI_UNSUPPORTED;
  }
  Status = EFI_SUCCESS;
  GuidHob = GetFirstGuidHob (&gMeFirmwareVersionHobGuid);
  if (GuidHob == NULL) {
    Status = HeciGetMkhiVersion (&MeFirmwareHob.MkhiVersion);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "ERROR: Can't get FW Version\n", __FUNCTION__));
    } else {
    DEBUG ((
      DEBUG_INFO,
      "[ME] FW Version %d.%d\n",
      MeFirmwareHob.MkhiVersion.Fields.Major,
      MeFirmwareHob.MkhiVersion.Fields.Minor
      ));
      MeFirmwareHobPtr = BuildGuidDataHob (&gMeFirmwareVersionHobGuid, &MeFirmwareHob, sizeof (MeFirmwareHob));
      if (MeFirmwareHobPtr == NULL) {
        Status = EFI_DEVICE_ERROR;
      }
    }
  }

  return Status;
}

/**
  Checks EndOfManufacturing (EOM)

  @retval TRUE   Out of Manufacturing Mode
  @retval FALSE  In Manufacturing Mode
**/
BOOLEAN
IsEom (
  VOID
  )
{
  HECI_FW_STS6_REGISTER          MeFirmwareStatus;

  MeFirmwareStatus.ul = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_ME_HFS_6));
  return ((MeFirmwareStatus.r.FpfSocConfigLock) ? TRUE : FALSE);
}