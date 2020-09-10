/** @file
  HECI utilities common module for use in PEI, DXE, and SMM.

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

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

**/
#include <PiPei.h>
#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/MeUtilsLib.h>
#include <MeState.h>
#include <Register/HeciRegs.h>
#include <Register/MeRegs.h>
#include <IndustryStandard/Pci30.h>
#include <Library/MeTypeLib.h>
#include <Library/HobLib.h>
#include <MeBiosPayloadHob.h>
#include <Protocol/HeciProtocol.h>
#include <Protocol/HeciControlProtocol.h>
#include <Guid/MeFirmwareVersionInfoHobGuid.h>
#include <MkhiMsgs.h>

/*****************************************************************************
 * Local definitions.
 *****************************************************************************/

/**
  Read HECI 1 16bit register

  @param [IN] Offset         HECI 1 register offset to read

  @retval UINT32             HECI 1 read register value
**/
static
UINT16
MeHeci1PciRead16 (
  IN UINT32 Offset
  )
{
  return PciSegmentRead16 (
    PCI_SEGMENT_LIB_ADDRESS (
      GetHeciPciSegment (),
      GetHeciPciBus (),
      GetHeciPciDevice (),
      HECI1_DEVICE,
      Offset
      ));
}

/**
  Read ME Firmware Status 1 on HECI1

  @retval UINT32             ME Firmware Status
**/
static
UINT32
MeHeciPciReadMeFwsts1 (
  VOID
  )
{
  return PciSegmentRead32 (
    PCI_SEGMENT_LIB_ADDRESS (
      GetHeciPciSegment (),
      GetHeciPciBus (),
      GetHeciPciDevice (),
      HECI1_DEVICE,
      R_ME_HFS
      ));
}

/**
  Read ME Firmware Status 1 on HECI2

  @retval UINT32             ME Firmware Status
**/
static
UINT32
MeHeci2PciReadMeFwsts1 (
  VOID
  )
{
  return PciSegmentRead32 (
    PCI_SEGMENT_LIB_ADDRESS (
      GetHeciPciSegment (),
      GetHeciPciBus (),
      GetHeciPciDevice (),
      HECI2_DEVICE,
      R_ME_HFS
      ));
}

/**
  Return ME Mode

  @param [out] MeMode             Pointer for ME Mode report

  @retval EFI_SUCCESS             MeMode copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
  @retval EFI_DEVICE_ERROR        Read device is not HECI
**/
EFI_STATUS
MeBiosGetMeMode (
  OUT UINT32                       *MeMode
  )
{
  HECI_FWS_REGISTER MeFirmwareStatus;

  if (MeMode == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (MeHeci1PciRead16 (PCI_VENDOR_ID_OFFSET) != V_INTEL_VENDOR_ID) {
    return EFI_DEVICE_ERROR;
  }

  MeFirmwareStatus.ul = MeHeciPciReadMeFwsts1 ();
  if (MeFirmwareStatus.ul == 0xFFFFFFFF) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Check if it's DFX ME FW first
  //
  if (MeFirmwareStatus.r.CurrentState == ME_STATE_DFX_FW) {
    *MeMode = ME_MODE_DFX_FW;
  } else {
    switch (MeFirmwareStatus.r.MeOperationMode) {
      case ME_OPERATION_MODE_SPS:
      case ME_OPERATION_MODE_NORMAL:
        *MeMode = ME_MODE_NORMAL;
        break;

      case ME_OPERATION_MODE_DEBUG:
        *MeMode = ME_MODE_DEBUG;
        break;

      case ME_OPERATION_MODE_SOFT_TEMP_DISABLE:
        *MeMode = ME_MODE_TEMP_DISABLED;
        break;

      case ME_OPERATION_MODE_SECOVR_JMPR:
      case ME_OPERATION_MODE_SECOVR_HECI_MSG:
        *MeMode = ME_MODE_SECOVER;
        break;

      default:
        *MeMode = ME_MODE_FAILED;
    }
  }

  DEBUG ((DEBUG_INFO, "[ME] MeMode %d (HECI-1 HFS: %08X)\n", *MeMode, MeFirmwareStatus.ul));

  return EFI_SUCCESS;
}

/**
  Get an Intel ME Firmware Status from HECI-1 HFS register.

  @param [out] MeStatus           Pointer for status report


  @retval EFI_SUCCESS             MeStatus copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeStatus is invalid
  @retval EFI_DEVICE_ERROR        Read device is not HECI
**/
EFI_STATUS
MeBiosGetMeStatusRaw (
  OUT UINT32                      *MeStatus
  )
 {
  if (MeStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (MeHeci1PciRead16 (PCI_VENDOR_ID_OFFSET) != V_INTEL_VENDOR_ID) {
    return EFI_DEVICE_ERROR;
  }

  *MeStatus = MeHeciPciReadMeFwsts1 ();
  if (*MeStatus == 0xFFFFFFFF) {
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "[ME] %a HECI-1 HFS: %08X\n", __FUNCTION__, *MeStatus));

  return EFI_SUCCESS;
 }

/**
  Get an abstract Intel ME Firmware Status from HECI-1 HFS register.
  This is used to control BIOS flow for various Intel ME functions.

  @param [out] MeStatus           Pointer for status report
                                  see MeState.h - Abstract ME status definitions.

  @retval EFI_SUCCESS             MeStatus copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeStatus is invalid
  @retval EFI_DEVICE_ERROR        Read device is not HECI
**/
EFI_STATUS
MeBiosGetMeStatus (
  OUT UINT32                      *MeStatus
  )
{
  EFI_STATUS                      Status;
  HECI_FWS_REGISTER               MeFirmwareStatus;

  if (MeStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MeBiosGetMeStatusRaw (&MeFirmwareStatus.ul);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (MeFirmwareStatus.r.CurrentState == ME_STATE_NORMAL &&
      MeFirmwareStatus.r.ErrorCode == ME_ERROR_CODE_NO_ERROR) {
    *MeStatus = ME_READY;
  } else if (MeFirmwareStatus.r.CurrentState == ME_STATE_RECOVERY) {
    *MeStatus = ME_IN_RECOVERY_MODE;
  } else if (MeFirmwareStatus.r.CurrentState == ME_STATE_INIT) {
    *MeStatus = ME_INITIALIZING;
  } else if (MeFirmwareStatus.r.CurrentState == ME_STATE_DISABLE_WAIT) {
    *MeStatus = ME_DISABLE_WAIT;
  } else if (MeFirmwareStatus.r.CurrentState == ME_STATE_TRANSITION) {
    *MeStatus = ME_TRANSITION;
  } else {
    *MeStatus = ME_NOT_READY;
  }

  if (MeFirmwareStatus.r.FwUpdateInprogress) {
    *MeStatus |= ME_FW_UPDATES_IN_PROGRESS;
  }

  if (MeFirmwareStatus.r.FwInitComplete == ME_FIRMWARE_COMPLETED) {
    *MeStatus |= ME_FW_INIT_COMPLETE;
  }

  if (MeFirmwareStatus.r.MeBootOptionsPresent == ME_BOOT_OPTIONS_PRESENT) {
    *MeStatus |= ME_FW_BOOT_OPTIONS_PRESENT;
  }

  DEBUG ((DEBUG_INFO, "[ME] MeStatus 0x%X\n", *MeStatus));

  return EFI_SUCCESS;
}

/**
  Checks if Me FW is Client and it is ready

  @retval FALSE           Not ME Client and ready
  @retval TRUE            Ready Client ME detected
**/
BOOLEAN
MeClientIsReady (
  VOID
  )
{
  HECI_FWS_REGISTER MeFirmwareStatus;

  if (MeTypeIsClient ()) {
    MeFirmwareStatus.ul = MeHeciPciReadMeFwsts1 ();
    if (MeFirmwareStatus.ul == 0xFFFFFFFF) {
      return FALSE;
    }
    return  (BOOLEAN) (MeFirmwareStatus.r.FwInitComplete != 0);
  }

  return FALSE;
}

/**
  Checks if Me FW is Client Corporate

  @retval FALSE           Not ME Client corporate
  @retval TRUE            Corporate Client ME detected
**/
BOOLEAN
MeClientIsCorporate (
  VOID
  )
{
  ME_BIOS_PAYLOAD_HOB     *MbpHob;
  if (MeTypeIsClient ()) {
    MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
    if ((MbpHob != NULL) &&
      (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType == IntelMeCorporateFw)) {
      return TRUE;
    }
  }

 return FALSE;
}

/**
  Checks if Me FW is SPS ready

  @retval FALSE           SPS is not ready
  @retval TRUE            Ready SPS detected
**/
BOOLEAN
MeSpsIsReady (
  VOID
  )
{
  HECI_FWS_REGISTER MeFirmwareStatus;

  if (MeTypeIsSps ()) {
    MeFirmwareStatus.ul = MeHeciPciReadMeFwsts1 ();
    if (MeFirmwareStatus.ul == 0xFFFFFFFF) {
      return FALSE;
    }
    return (BOOLEAN) (MeFirmwareStatus.r.FwInitComplete != 0);
  }

  return FALSE;
}

/**
  Checks if Me FW is SPS in recovery

  @retval FALSE           SPS is not in recovery
  @retval TRUE            SPS is in recovery
**/
BOOLEAN
MeSpsInRecovery (
  VOID
  )
{
  HECI_FWS_REGISTER MeFirmwareStatus;

  if (MeTypeIsSps()) {
    MeFirmwareStatus.ul = MeHeciPciReadMeFwsts1 ();
    if (MeFirmwareStatus.ul == 0xFFFFFFFF) {
      return TRUE;
    }
    if (MeFirmwareStatus.r.CurrentState == ME_STATE_RECOVERY) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Checks if Me FW is Client in manufacturing

  @retval FALSE           Client is not in recovery
  @retval TRUE            CLient is in recovery
-**/
BOOLEAN
MeClientInRecovery (
  VOID
  )
{
  HECI_FWS_REGISTER MeFirmwareStatus;

  if (MeTypeIsClient ()) {
    MeFirmwareStatus.ul = MeHeciPciReadMeFwsts1 ();
    if (MeFirmwareStatus.ul == 0xFFFFFFFF) {
      return TRUE;
    }
    if (MeFirmwareStatus.r.MeOperationMode == ME_OPERATION_MODE_SECOVR_JMPR) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Checks if Me is in manufacturing

  @retval FALSE           ME is not in recovery
  @retval TRUE            ME is in recovery
**/
BOOLEAN
MeIsInRecovery (
  VOID
  )
{
  if (MeSpsInRecovery () || MeClientInRecovery ()) {
    return TRUE;
  }

  return FALSE;
}

/**
  Checks if Me FW is SPS NM

  @retval FALSE           SPS NM not detected
  @retval TRUE            SPS NM detected
-**/
BOOLEAN
MeSpsIsNm (
  VOID
  )
{
  UINT32 NmFs;

  if (MeTypeIsSps () && !MeSpsInRecovery ()) {
    NmFs = MeHeci2PciReadMeFwsts1 ();
    if ((NmFs != 0xFFFFFFFF) && (NmFs & BIT31)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Get InitComplete from Firmware Status

  @param[out] ReturnMeFs1 Pointer to return ME Firmware Status 1

  @retval FALSE  InitComplete bit is not set
  @retval TRUE   InitComplete bit is set
**/
BOOLEAN
GetInitComplete (
  OUT UINT32 *ReturnMeFs1
  )
{
  HECI_FWS_REGISTER      MeFs1;

  MeFs1.ul = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_ME_HFS));
  if (ReturnMeFs1 != NULL) {
    *ReturnMeFs1 = MeFs1.ul;
  }
  if ((MeFs1.ul != 0xFFFFFFFF) &&
      (MeFs1.r.FwInitComplete == 1)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Get end of post from SPS Firmware Status

  @param[out] ReturnMeFs2 Pointer to return ME Firmware Status 2

  @retval FALSE  EndOfPost bit is not set
  @retval TRUE   EndOfPost bit is set
**/
BOOLEAN
GetSpsEndOfPostStatus (
  OUT UINT32 *ReturnMeFs2
  )
{
  UINT32       MeFs2;

  MeFs2 = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_ME_HFS_2));
  if (ReturnMeFs2 != NULL) {
    *ReturnMeFs2 = MeFs2;
  }
  // Check SPS bit11 for EopStatus
  if ((MeFs2 != 0xFFFFFFFF) &&
      ((MeFs2 & BIT11) != 0)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Returns the GroupId and Me Client Address for former MCA
  messages

  If no MhkiVersion HOB  exists use HECI_MCHI_MESSAGE_ADDR
  and MKHI_MCA_GROUP_ID as a default.

  @param[in]  Command                Former MCA Command
  @param[out] GroupId                GroupId for the given command
  @param[out] MeClientAddress        Me Client for the given command
**/
VOID
GetMcaMsgParams (
  IN  UINT32 Command,
  OUT UINT8  *GroupId,
  OUT UINT8  *MeClientAddress
  )
{
  EFI_HOB_GUID_TYPE         *GuidHob;
  ME_FIRMWARE_VERSION_HOB   *MeFirmwareHobData;

  //
  // Assume that MCHI is a default ME Client
  // and MCA is a default GroupId
  //
  *MeClientAddress = HECI_MCHI_MESSAGE_ADDR;
  *GroupId         = MKHI_MCA_GROUP_ID;

  if (MeTypeIsSps ()) {
    *MeClientAddress = HECI_MKHI_MESSAGE_ADDR;
  }

  GuidHob = GetFirstGuidHob (&gMeFirmwareVersionHobGuid);
  if (GuidHob != NULL) {
    MeFirmwareHobData = (ME_FIRMWARE_VERSION_HOB *) GET_GUID_HOB_DATA (GuidHob);
    switch (Command) {
      case MCA_CORE_BIOS_DONE_CMD:
        if (MeFirmwareHobData->MkhiVersion.Fields.Minor == 1 && !MeTypeIsClient ()) {
          *MeClientAddress = HECI_MKHI_MESSAGE_ADDR;
        } else if (MeFirmwareHobData->MkhiVersion.Fields.Minor >= 3)  {
          *GroupId         = MKHI_GEN_GROUP_ID;
          *MeClientAddress = HECI_MKHI_MESSAGE_ADDR;
        }
        break;

      default:
        if (MeFirmwareHobData->MkhiVersion.Fields.Minor == 1 && !MeTypeIsClient ()) {
          *MeClientAddress = HECI_MKHI_MESSAGE_ADDR;
        }
        break;
    }
  }

  DEBUG ((DEBUG_INFO, "[ME] %a Selected Client = %d\n", __FUNCTION__, *MeClientAddress));
  DEBUG ((DEBUG_INFO, "[ME] %a Selected Group = %d\n", __FUNCTION__, *GroupId));

}
