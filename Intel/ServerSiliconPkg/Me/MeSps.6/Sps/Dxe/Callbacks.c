/** @file
  This file contains callbacks of events that SPS is interested in.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
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
**/

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Library/ServerDxeMeLib.h>
#include <Guid/GlobalVariable.h>
#include <Guid/HobList.h>
#include <Guid/EventLegacyBios.h>
#include <Guid/SpsInfoHobGuid.h>
#include <Protocol/MpService.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuLib.h>
#include <Library/DxeMeLib.h>
#include <Protocol/HeciControlProtocol.h>
#include <Protocol/PciIo.h>
#include <Sps.h>
#include <Library/PchInfoLib.h>
#include <Register/HeciRegs.h>
#include <MeAccess.h>
#include <MeDefines.h>

#include "SpsDxe.h"
#include "DualBiosWatchdog.h"

#include <ConfigBlock/SpsGetDxeConfigBlock.h>
#include <PchResetPlatformSpecific.h>
#include <IndustryStandard/Pci22.h>
#include <IpBlock/Pmc/IncludePrivate/Library/PmcPrivateLib.h>

/******************************************************************************
 * Variables
 */

/******************************************************************************
 * Functions
 */

/**
  Update S3 resume script

  @param Void

  @retval Void

**/
VOID
SpsUpdateS3ResumeScript(
  VOID
  )
{
  UINT32               Data32[2];
  UINT8                Data8;
  UINT8                PciSegment;
  UINT8                PciBus;
  UINT8                PciDevice;

  PciSegment = (UINT8)GetHeciPciSegment ();
  PciBus = (UINT8)GetHeciPciBus ();
  PciDevice = (UINT8)GetHeciPciDevice ();

  //
  // If HECI-1 is SMI or SCI mode there is no regular OS driver and
  // we must save it's configuration.
  //
  Data8 = HeciPciRead8 (R_ME_HIDM);
  if ((Data8 != 0xFF) &&
      ((Data8 & ~B_ME_HIDM_L) != V_ME_HIDM_MSI)) {
    DEBUG ((DEBUG_INFO, "[SPS] Saving HECI-1 configuration to S3 boot script\n"));
    S3BootScriptSavePciCfg2Write (
      S3BootScriptWidthUint8,
      PciSegment,
      S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (PciBus, PciDevice, HECI1_DEVICE, R_ME_HIDM),
      1, &Data8
      );

    Data32[0] = HeciPciRead32 (PCI_BASE_ADDRESSREG_OFFSET);
    Data32[1] = HeciPciRead32 (PCI_BASE_ADDRESSREG_OFFSET + 4);
    S3BootScriptSavePciCfg2Write (
      S3BootScriptWidthUint32,
      PciSegment,
      S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (PciBus, PciDevice, HECI1_DEVICE, PCI_BASE_ADDRESSREG_OFFSET),
      2, &Data32[0]
      );

    Data8 = HeciPciRead8 (PCI_INT_LINE_OFFSET);
    S3BootScriptSavePciCfg2Write (
      S3BootScriptWidthUint8,
      PciSegment,
      S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (PciBus, PciDevice, HECI1_DEVICE, PCI_INT_LINE_OFFSET),
      1, &Data8
      );

    Data8 = HeciPciRead8 (PCI_COMMAND_OFFSET);
    S3BootScriptSavePciCfg2Write (
      S3BootScriptWidthUint8,
      PciSegment,
      S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (PciBus, PciDevice, HECI1_DEVICE, PCI_COMMAND_OFFSET),
      1, &Data8);
  }
  //
  // If HECI-2 is SMI or SCI mode there is no regular OS driver and
  // we must save it's configuration.
  //
  Data8 = Heci2PciRead8 (R_ME_HIDM);
  if ((Data8 != 0xFF) &&
      ((Data8 & ~B_ME_HIDM_L) != V_ME_HIDM_MSI)) {
    DEBUG ((DEBUG_INFO, "[SPS] Saving HECI-2 configuration to S3 boot script\n"));
    S3BootScriptSavePciCfg2Write (
      S3BootScriptWidthUint8,
      PciSegment,
      S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (PciBus, PciDevice, HECI2_DEVICE, R_ME_HIDM),
      1, &Data8
      );

    Data32[0] = Heci2PciRead32 (PCI_BASE_ADDRESSREG_OFFSET);
    Data32[1] = Heci2PciRead32 (PCI_BASE_ADDRESSREG_OFFSET + 4);
    S3BootScriptSavePciCfg2Write (
      S3BootScriptWidthUint32,
      PciSegment,
      S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (PciBus, PciDevice, HECI2_DEVICE, PCI_BASE_ADDRESSREG_OFFSET),
      2, &Data32[0]
      );

    Data8 = Heci2PciRead8 (PCI_INT_LINE_OFFSET);
    S3BootScriptSavePciCfg2Write (
      S3BootScriptWidthUint8,
      PciSegment,
      S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (PciBus, PciDevice, HECI2_DEVICE, PCI_INT_LINE_OFFSET),
      1, &Data8
      );

    Data8 = Heci2PciRead8 (PCI_COMMAND_OFFSET);
    if (!(Data8 & EFI_PCI_COMMAND_MEMORY_SPACE) && (Data32[0] != 0 || Data32[1] != 0)) {
      Data8 = EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE;
      Heci2PciWrite8 (PCI_COMMAND_OFFSET, Data8);
    }
    S3BootScriptSavePciCfg2Write (
      S3BootScriptWidthUint8,
      PciSegment,
      S3_BOOT_SCRIPT_LIB_PCI_ADDRESS (PciBus, PciDevice, HECI2_DEVICE, PCI_COMMAND_OFFSET),
      1, &Data8
      );
  }
}

/**
  SPS callback for EndOfDxe event.
  Performs actions required by SPS ME-BIOS Spec before Option ROMs are loaded.

  @param [in] Event    - the event, not used
  @param [in] Context - pointer to SPS firmware description

  @retval Void.
**/
VOID
EFIAPI
SpsEndOfDxeCallback(
  IN EFI_EVENT      Event,
  IN VOID           *Context
  )
{
  EFI_STATUS        Status;
  UINT32            RspLen;
  HECI_CONTROL      *HeciProtocol;
  SPS_DXE_CONTEXT   *SpsContext = (SPS_DXE_CONTEXT*)Context;
  union {
    MCTP_SET_BUS_OWNER_REQ     MctpSetBusOwnerReq;
    MCTP_SET_BUS_OWNER_RSP     MctpSetBusOwnerRsp;
  } HeciMsg;

  gBS->CloseEvent (Event);

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &HeciProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (%r)\n", Status));
  } else {
    //
    // Send MCPT Bus Owner Proxy configuration if configured.
    //
    if (SpsContext->pSpsPolicy->Common.SpsMctpBusOwner != 0 &&
        SpsContext->pSpsHob->FeatureSet.Bits.MctpProxy) {

      DEBUG ((DEBUG_INFO, "[SPS] Sending MCTP_SET_BUS_OWNER with address 0x%X\n",
          SpsContext->pSpsPolicy->Common.SpsMctpBusOwner));

      HeciMsg.MctpSetBusOwnerReq.Command = MCPT_CMD_SET_BUS_OWNER_REQ;
      HeciMsg.MctpSetBusOwnerReq.Reserved0[0] =
      HeciMsg.MctpSetBusOwnerReq.Reserved0[1] =
      HeciMsg.MctpSetBusOwnerReq.Reserved0[2] = 0;
      HeciMsg.MctpSetBusOwnerReq.PCIeAddress = SpsContext->pSpsPolicy->Common.SpsMctpBusOwner;
      HeciMsg.MctpSetBusOwnerReq.Location = 0;
      HeciMsg.MctpSetBusOwnerReq.Reserved1 = 0;
      RspLen = sizeof (HeciMsg.MctpSetBusOwnerRsp);

      Status = HeciProtocol->HeciSendAndReceive (
                               HeciProtocol,
                               HECI_DEFAULT_PCH_ID,
                               NULL,
                               HECI_DEFAULT_RETRY_NUMBER,
                               SPS_CLIENTID_BIOS,
                               SPS_CLIENTID_ME_MCTP,
                               (UINT32*) &HeciMsg.MctpSetBusOwnerReq,
                               sizeof (HeciMsg.MctpSetBusOwnerReq),
                               (UINT32*) &HeciMsg.MctpSetBusOwnerRsp,
                               &RspLen,
                               NULL,
                               NULL
                               );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot send MCTP_SET_BUS_OWNER (%r)\n", Status));
      } else if (HeciMsg.MctpSetBusOwnerRsp.Command != MCPT_CMD_SET_BUS_OWNER_RSP) {
        DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Wrong response received (Command: 0x%x)\n", HeciMsg.MctpSetBusOwnerRsp.Command));
      } else if (HeciMsg.MctpSetBusOwnerRsp.Result != MCPT_CMD_SET_BUS_OWNER_RSP_SUCCESS) {
        DEBUG ((DEBUG_ERROR, "[SPS] ERROR: MCTP_SET_BUS_OWNER failure (cause: %d)\n",
               HeciMsg.MctpSetBusOwnerRsp.Result));
      }
    }

    //
    //Sending notification that BIOS is starting to run 3rd party code.
    //
    if (MeCoreBiosDoneEnabled ()) {
      ServerHeciCoreBiosDoneMsg ();
    }
  } // if (HECI protocol located)

  SpsUpdateS3ResumeScript ();

  return;
} // SpsEndOfDxeCallback()


/**
    SPS callback for ready-to-boot event.
    Performs actions required by SPS ME-BIOS Spec before OS is loaded.

    @param Event    - the event, not used
    @param Context - pointer to SPS firmware description

    @retval Void.
**/
VOID
EFIAPI
SpsReadyToBootCallback(
  IN EFI_EVENT      Event,
  IN VOID           *Context
  )
{
  EFI_STATUS        Status;
  UINT32            RspLen;
  HECI_CONTROL      *HeciProtocol;
  union {
    MKHI_MSG_END_OF_POST_REQ       EopReq;
    MKHI_MSG_END_OF_POST_RSP       EopRsp;
  } HeciMsg;
  PCH_RESET_DATA     ResetData;

  gBS->CloseEvent(Event);

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &HeciProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (%r)\n", Status));
  } else {

#if ME_TESTMENU_FLAG
    //
    // Send END_OF_POST notification if not disabled in ME Test Menu options
    //
    if (MeEndOfPostEnabled ())
#endif
    {
      DEBUG ((DEBUG_INFO, "[SPS] Sending END_OF_POST to ME\n"));

      HeciMsg.EopReq.Mkhi.Data = 0x00000CFF;
      RspLen = sizeof (HeciMsg.EopRsp);

      SetMeUefiFwHealthStatus (EndOfPostSendingStatus, StatusOk);
      Status = HeciProtocol->HeciSendAndReceive (
                               HeciProtocol,
                               HECI_DEFAULT_PCH_ID,
                               NULL,
                               HECI_DEFAULT_RETRY_NUMBER,
                               SPS_CLIENTID_BIOS,
                               SPS_CLIENTID_ME_MKHI,
                               (UINT32*) &HeciMsg.EopReq,
                               sizeof (HeciMsg.EopReq),
                               (UINT32*) &HeciMsg.EopRsp,
                               &RspLen,
                               NULL,
                               NULL
                               );
      if (EFI_ERROR (Status)) {
        SetMeUefiFwHealthStatus (EndOfPostSendingStatus, StatusError);
        DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot send END_OF_POST (%r)\n", Status));
        MeReportError (MSG_EOP_ERROR);
      } else if (HeciMsg.EopRsp.Mkhi.Data != 0x00008CFF) {
        SetMeUefiFwHealthStatus (EndOfPostSendingStatus, StatusOther);
        DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Invalid END_OF_POST response (MKHI: 0x%X)\n",
                            HeciMsg.EopRsp.Mkhi.Data));
      } else if (RspLen == sizeof (HeciMsg.EopRsp) && HeciMsg.EopRsp.Action == 1) {
        // if response contains Action and global reset was requested
        DEBUG ((DEBUG_WARN, "[SPS] WARNING: END_OF_POST response requests Global Reset!!!\n"));
        CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
        StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
        gRT->ResetSystem (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
      }
      // Set EndOfPostDone unconditionally
      MeSetEndOfPostDone ();
    } // if (MeEndOfPostEnabled ())
  } // if (HECI protocol located)

#if ME_TESTMENU_FLAG
  //
  // Upon user menu request set the CF9 reset promotion to global reset promotion
  //
  if (((SPS_DXE_CONTEXT*)Context)->pSpsPolicy->MeGrPromotionEnabled) {
    DEBUG ((DEBUG_INFO, "[SPS] Promoting CF9 reset to global reset\n"));
    PmcEnableCf9GlobalReset ();
  }
#endif //ME_TESTMENU_FLAG

#if ME_TESTMENU_FLAG
  //
  // Lock global reset in ETR3 register if not disabled in ME Test Menu options
  //
  if (((SPS_DXE_CONTEXT*)Context)->pSpsPolicy->MeGrLockEnabled)
#endif //ME_TESTMENU_FLAG
  {
    DEBUG((DEBUG_INFO, "[SPS] Disabling Global Reset capability\n"));
    PmcDisableCf9GlobalResetWithLock ();
  }
  return;
} // SpsReadyToBootCallback()

/**
  DFX ME FW callback for ready-to-boot event.
  Performs actions required by DFX FWbefore OS is loaded.

  @param[in] Event      the event, not used
  @param[in] Context    pointer to SPS firmware description
**/
VOID
EFIAPI
DfxReadyToBootCallback (
  IN EFI_EVENT      Event,
  IN VOID           *Context
  )
{
  SPS_DXE_CONFIG *pSpsPolicy;

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  pSpsPolicy = GetSpsDxeConfigBlock ();

  if (pSpsPolicy != NULL && pSpsPolicy->Common.Heci1HideInMe == ME_SPS_HECI_IN_ME_DISABLE) {
    PmcDisableMeDevices (TRUE);
  }
}