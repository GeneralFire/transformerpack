/** @file
  This module contains the necessary SPS ME firmware support specified in SPS ME-BIOS
  Interface Specification for the Intel Firmware Support Package (FSP).
  Note it may not include all features and debug capability as provided in
  BIOS reference code build.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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
**/

#ifdef FSP_FLAG

#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

#include <Ppi/HeciControlPpi.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Ppi/SpsHwChangePpi.h>

#include <Guid/SpsInfoHobGuid.h>

#include <Guid/GlobalVariable.h>
#include <ConfigBlock/MeGetConfigBlock.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/MeUefiFwHealthStatusLib.h>

#include <Register/PchRegsPsf.h>
#include <PchReservedResources.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>

#include <Library/MeEwlLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Register/HeciRegs.h>
#include <MeAccess.h>
#include <Sps.h>
#include <Library/MeTypeLib.h>
#include <Library/SpsPeiLib.h>
#include <Ppi/MeUmaPpi.h>
#include <SpsPei.h>
#include <MeDefines.h>
#include <Protocol/SpsSmmHmrfpoProtocol.h>
#include <MeState.h>
#include <Library/MeUtilsLib.h>
#include <Library/PmcPrivateLib.h>

#ifndef CPUID_EXTENDED_TOPOLOGY
#define CPUID_EXTENDED_TOPOLOGY                0xB
#endif

UINT64 mNonce = 0;
extern CONST CHAR8  *SpsPrefix;
//
// FSP Notify routines
//

EFI_STATUS
EFIAPI
SpsFspReadyToBootEvent (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

//
// FSP Notify Descriptor
//
STATIC
EFI_PEI_NOTIFY_DESCRIPTOR  mSpsFspNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiEventReadyToBootGuid,
    SpsFspReadyToBootEvent
  }
};

/**
 @brief
  SpsFspSendMcptBusOwnerProxy: Send HMRFPO_LOCK (and related) messages to ME.

  @param[in] pHeciPPI           - Pointer to HECI interface
  @param[in] SpsMctpBusOwner    - Address of MCTP Bus Owner

  @retval EFI_SUCCESS           - The function completed successfully

**/
EFI_STATUS
SpsFspSendMcptBusOwnerProxy (
  IN HECI_CONTROL        *pHeciPpi,
  IN UINT16              SpsMctpBusOwner
  )
{
  EFI_STATUS        Status;
  UINT32            RspLen;

  union {
    MCTP_SET_BUS_OWNER_REQ     MctpSetBusOwnerReq;
    MCTP_SET_BUS_OWNER_RSP     MctpSetBusOwnerRsp;
  } HeciMsg;

  Status = EFI_SUCCESS;

  if (SpsMctpBusOwner == 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "%a SpsFspSendMcptBusOwnerProxy Start\n", SpsPrefix));
  DEBUG ((DEBUG_INFO, "%a Sending MCTP_SET_BUS_OWNER with address 0x%X\n", SpsPrefix, SpsMctpBusOwner));

  HeciMsg.MctpSetBusOwnerReq.Command = MCPT_CMD_SET_BUS_OWNER_REQ;
  HeciMsg.MctpSetBusOwnerReq.Reserved0[0] =
  HeciMsg.MctpSetBusOwnerReq.Reserved0[1] =
  HeciMsg.MctpSetBusOwnerReq.Reserved0[2] = 0;
  HeciMsg.MctpSetBusOwnerReq.PCIeAddress = SpsMctpBusOwner;
  HeciMsg.MctpSetBusOwnerReq.Location = 0;
  HeciMsg.MctpSetBusOwnerReq.Reserved1 = 0;

  RspLen = sizeof (HeciMsg.MctpSetBusOwnerRsp);

  Status = pHeciPpi->HeciSendAndReceive (
                       pHeciPpi,
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
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot send MCTP_SET_BUS_OWNER (%r)\n", SpsPrefix, Status));
  } else if (HeciMsg.MctpSetBusOwnerRsp.Result != MCPT_CMD_SET_BUS_OWNER_RSP_SUCCESS) {
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR: MCTP_SET_BUS_OWNER failure (cause: %d)\n",
      SpsPrefix,
      HeciMsg.MctpSetBusOwnerRsp.Result
      ));
  }

  DEBUG ((DEBUG_INFO, "%a SpsFspSendMcptBusOwnerProxy End (%r)\n", SpsPrefix, Status));

  return Status;
} // SpsFspSendMcptBusOwnerProxy ()

/**
  @brief
  SpsFspSendHmrfpoLockRequest: Send HMRFPO_LOCK message to ME.

  @param[in] pHeciPPI    - Pointer to HECI interface
  @retval EFI_SUCCESS    - The function completed successfully

**/
EFI_STATUS
SpsFspSendHmrfpoLockRequest (
  IN HECI_CONTROL        *pHeciPpi
  )
{
  EFI_STATUS        Status;
  UINT32            RspLen;
  union {
    MKHI_MSG_HMRFPO_LOCK_REQ   HmrfpoLockReq;
    MKHI_MSG_HMRFPO_LOCK_RSP   HmrfpoLockRsp;
  } HeciMsg;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a SpsFspSendHmrfpoLockRequest Start\n", SpsPrefix));

  HeciMsg.HmrfpoLockReq.Mkhi.Data = 0x00000205;
  RspLen = sizeof (HeciMsg.HmrfpoLockRsp);

  Status = pHeciPpi->HeciSendAndReceive (
                       pHeciPpi,
                       HECI_DEFAULT_PCH_ID,
                       NULL,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_MKHI,
                       (UINT32*) &HeciMsg.HmrfpoLockReq,
                       sizeof (HeciMsg.HmrfpoLockReq),
                       (UINT32*) &HeciMsg.HmrfpoLockRsp,
                       &RspLen,
                       NULL,
                       NULL
                       );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot send HMRFPO_LOCK (%r)\n", SpsPrefix, Status));
  } else if (HeciMsg.HmrfpoLockRsp.Mkhi.Data != 0x00008205) {
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR: Invalid HMRFPO_LOCK response (MKHI: 0x%X)\n",
      SpsPrefix,
      HeciMsg.HmrfpoLockRsp.Mkhi.Data
      ));
  } else if (HeciMsg.HmrfpoLockRsp.Status != 0) {
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR: HMRFPO_LOCK failed (cause: %d)\n",
      SpsPrefix,
      HeciMsg.HmrfpoLockRsp.Status
      ));
  } else {
    mNonce = HeciMsg.HmrfpoLockRsp.Nonce;
  }

  DEBUG ((DEBUG_INFO, "%a SpsFspSendHmrfpoLockRequest End (%r)\n", SpsPrefix, Status));

  return Status;
} // SpsFspSendHmrfpoLockRequest ()

/**
  @brief
  SpsFspSendHmrfpoEnableRequest: Send HMRFPO_ENABLE message to ME.

  @param[in]  pHeciPPI       - Pointer to HECI interface
  @param[in]  RegionToEnable - Region, to request access enabling (HMRFPO_EN_REGION_*)

  @retval EFI_SUCCESS    - The function completed successfully

**/
EFI_STATUS
SpsFspSendHmrfpoEnableRequest (
 IN HECI_CONTROL     *pHeciPpi,
 IN UINT8            RegionToEnable
 )
{
  EFI_STATUS        Status;
  UINT32            RspLen;
  UINT8             Tries;

  union {
    MKHI_MSG_HMRFPO_ENABLE_REQ HmrfpoEnableReq;
    MKHI_MSG_HMRFPO_ENABLE_RSP HmrfpoEnableRsp;
  } HeciMsg;

  DEBUG ((DEBUG_INFO, "%a SpsFspSendHmrfpoEnableRequest Start\n", SpsPrefix));

  //
  // We are required to handle 3 retries for HMRFPO_LOCK.
  //
  for (Tries = 3; Tries--; ) {
    HeciMsg.HmrfpoEnableReq.Mkhi.Data = 0x00000105;
    HeciMsg.HmrfpoEnableReq.Nonce = mNonce;
    HeciMsg.HmrfpoEnableReq.RegionSelect = RegionToEnable;
    RspLen = sizeof(HeciMsg.HmrfpoEnableRsp);

    Status = pHeciPpi->HeciSendAndReceive (
                         pHeciPpi,
                         HECI_DEFAULT_PCH_ID,
                         NULL,
                         HECI_DEFAULT_RETRY_NUMBER,
                         SPS_CLIENTID_BIOS,
                         SPS_CLIENTID_ME_MKHI,
                         (UINT32*) &HeciMsg.HmrfpoEnableReq,
                         sizeof (HeciMsg.HmrfpoEnableReq),
                         (UINT32*) &HeciMsg.HmrfpoEnableRsp,
                         &RspLen,
                         NULL,
                         NULL
                         );
    if (!EFI_ERROR (Status)) {
      if (HeciMsg.HmrfpoEnableRsp.Mkhi.Data != 0x00008105) {
        DEBUG ((
          DEBUG_ERROR,
          "%a ERROR: Invalid HMRFPO_ENABLE response (MKHI: 0x%08X), %d tries left\n",
          SpsPrefix,
          HeciMsg.HmrfpoEnableRsp.Mkhi.Data,
          Tries
          ));
        continue;
      }

      if (HeciMsg.HmrfpoEnableRsp.Status != 0) {
        DEBUG ((
          DEBUG_ERROR,
          "%a ERROR: HMRFPO_ENABLE refused (cause: %d)\n",
          SpsPrefix,
          HeciMsg.HmrfpoEnableRsp.Status
          ));
      }
      break;
    }
    DEBUG ((
      DEBUG_ERROR,
      "%a HMRFPO_ENABLE send failed (%r), %d tries left\n",
      SpsPrefix,
      Status,
      Tries
      ));
  }

  DEBUG ((DEBUG_INFO, "%a SpsFspSendHmrfpoEnableRequest End (%r)\n", SpsPrefix, Status));

  return Status;
} // SpsFspSendHmrfpoEnableRequest ()

/**
 @brief
  SpsFspSendCoreBiosDone: Send CoreBiosDone message to ME.

  @param[in] pHeciPPI    - Pointer to HECI interface
  @retval EFI_SUCCESS    - The function completed successfully

**/
EFI_STATUS
SpsFspSendCoreBiosDone (
  IN HECI_CONTROL        *pHeciPpi
  )
{
  EFI_STATUS                    Status;
  UINT32                        Length;
  UINT32                        RspLen;
  CORE_BIOS_DONE                CoreBiosDone;
  CORE_BIOS_DONE_ACK            CoreBiosDoneAck;
  UINT32                        MeMode;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a SpsFspSendCoreBiosDone Start\n", SpsPrefix));

  if (MeTypeIsDfx ()) {
    DEBUG ((DEBUG_INFO, "%a CoreBiosDone not supported in DFx fw!", SpsPrefix));
    return EFI_UNSUPPORTED;
  }
  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    DEBUG ((DEBUG_ERROR, "%a SpsFspSendCoreBiosDone: unsupported! (status: %r - ME mode: %d)\n", SpsPrefix, Status, MeMode));
    return EFI_UNSUPPORTED;
  }

  Length = sizeof (CORE_BIOS_DONE);
  RspLen = sizeof (CORE_BIOS_DONE_ACK);

  CoreBiosDone.MkhiHeader.Data              = 0;
  CoreBiosDone.MkhiHeader.Fields.GroupId    = MKHI_MCA_GROUP_ID;
  CoreBiosDone.MkhiHeader.Fields.Command    = MCA_CORE_BIOS_DONE_CMD;
  CoreBiosDone.MkhiHeader.Fields.IsResponse = 0;

  Status = pHeciPpi->HeciSendAndReceive (
                       pHeciPpi,
                       HECI_DEFAULT_PCH_ID,
                       NULL,
                       HECI_DEFAULT_RETRY_NUMBER,
                       BIOS_FIXED_HOST_ADDR,
                       HECI_MKHI_MESSAGE_ADDR,
                       (UINT32*) &CoreBiosDone,
                       Length,
                       (UINT32*) &CoreBiosDoneAck,
                       &RspLen,
                       NULL,
                       NULL
                       );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a Core Bios Done Message: message failed! - %r\n", SpsPrefix, Status));
    SetMeUefiFwHealthStatus (CoreBiosDoneSendingStatus, StatusError);
  } else if ((CoreBiosDoneAck.MkhiHeader.Fields.Command != MCA_CORE_BIOS_DONE_CMD) ||
      (CoreBiosDoneAck.MkhiHeader.Fields.GroupId != MKHI_MCA_GROUP_ID) ||
      (CoreBiosDoneAck.MkhiHeader.Fields.IsResponse != 1) ||
      (CoreBiosDoneAck.MkhiHeader.Fields.Result != 0)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR: Invalid response received for CORE_BIOS_DONE (MKHI: %08X)\n",
      SpsPrefix,
      CoreBiosDoneAck.MkhiHeader.Data
      ));
    SetMeUefiFwHealthStatus (CoreBiosDoneSendingStatus, StatusOther);
    return EFI_DEVICE_ERROR;
  } else {
    SetMeUefiFwHealthStatus (CoreBiosDoneSendingStatus, StatusOk);
  }

  DEBUG ((DEBUG_INFO, "%a SpsFspSendCoreBiosDone: exit (%r)\n", SpsPrefix, Status));

  return Status;
} // SpsFspSendCoreBiosDone ()

/**
 @brief
  SpsFspSendEndOfPost: Send END_OF_POST message to ME.

  @param[in] pHeciPPI    - Pointer to HECI interface
  @retval EFI_SUCCESS    - The function completed successfully

**/
EFI_STATUS
SpsFspSendEndOfPost (
  IN HECI_CONTROL        *pHeciPpi
  )
{
  EFI_STATUS                    Status;
  UINT32                        Len;

  union {
    MKHI_MSG_END_OF_POST_REQ EopReq;
    MKHI_MSG_END_OF_POST_RSP EopRsp;
  } HeciMsg;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a SpsFspSendEndOfPost Start\n", SpsPrefix));

  HeciMsg.EopReq.Mkhi.Data = 0x00000CFF;
  Len = sizeof (HeciMsg.EopRsp);

  Status = pHeciPpi->HeciSendAndReceive (
                       pHeciPpi,
                       HECI_DEFAULT_PCH_ID,
                       NULL,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_MKHI,
                       (UINT32*) &HeciMsg.EopReq,
                       sizeof (HeciMsg.EopReq),
                       (UINT32*) &HeciMsg.EopRsp,
                       &Len,
                       NULL,
                       NULL
                       );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot send END_OF_POST (%r)\n", SpsPrefix, Status));
    SetMeUefiFwHealthStatus (EndOfPostSendingStatus, StatusError);
  } else if (HeciMsg.EopRsp.Mkhi.Data != 0x00008CFF) {
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR: Invalid END_OF_POST response (MKHI: 0x%X)\n",
      SpsPrefix,
      HeciMsg.EopRsp.Mkhi.Data
      ));
    SetMeUefiFwHealthStatus (EndOfPostSendingStatus, StatusOther);
  } else if (Len == sizeof (HeciMsg.EopRsp) &&   // if response contains Action and
             HeciMsg.EopRsp.Action == 1) {       // global reset was requested

    DEBUG ((DEBUG_WARN, "%a WARNING: END_OF_POST response requests Global Reset!!!\n", SpsPrefix));
    SetMeUefiFwHealthStatus (EndOfPostSendingStatus, StatusOk);

    (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetCold, EFI_SUCCESS, 0, NULL);

    DEBUG ((DEBUG_INFO, "%a Waiting for system reset ...\n", SpsPrefix));
    //
    // We should never get here
    //
    CpuDeadLoop ();
  } else {
    SetMeUefiFwHealthStatus (EndOfPostSendingStatus, StatusOk);
  }

  DEBUG ((DEBUG_INFO, "%a SpsFspSendEndOfPost End\n", SpsPrefix));

  return Status;
} // SpsFspSendEndOfPost ()

/**
 @brief
  SpsFspSendEndOfPost: Send HECI_CHANGE_STATE message to ME.

  @param[in] pHeciPPI    - Pointer to HECI interface
  @param[in] HeciId      - Heci interface to change (1, 2 or 3)
  @param[in] HeciState   - 0 - disable, 1 - hide
  @retval EFI_SUCCESS    - The function completed successfully
  @retval EFI_INVALID_PARAMETER - HeciId or HeciState is out of range

**/
EFI_STATUS
SpsFspSendHeciStateChange (
  IN HECI_CONTROL        *pHeciPpi,
  IN UINT8               HeciId,
  IN UINT8               HeciState
  )
{
  EFI_STATUS Status;
  UINT32     RspLen;
  UINT32     Timeout;
  union {
    MKHI_MSG_HECI_STATE_CHANGE_REQ HeciStateReq;
    MKHI_MSG_HECI_STATE_CHANGE_RSP HeciStateRsp;
  } HeciMsg;

  DEBUG ((DEBUG_INFO, "%a SpsFspSendHeciStateChange Start\n", SpsPrefix));

  if ((HeciId < 1) || (3 < HeciId)) {
    return EFI_INVALID_PARAMETER;
  }

  if (1 < HeciState) {
    return EFI_INVALID_PARAMETER;
  }

  HeciMsg.HeciStateReq.Mkhi.Data = 0x00000DFF;
  HeciMsg.HeciStateReq.Nonce = mNonce;
  HeciMsg.HeciStateReq.HeciId = HeciId;
  HeciMsg.HeciStateReq.State = HeciState;
  RspLen = sizeof (HeciMsg.HeciStateRsp);

  DEBUG ((
    DEBUG_INFO,
    "%a Sending HECI_CHANGE_STATE to ME to %a HECI-%d\n",
    SpsPrefix
    (HeciMsg.HeciStateReq.State) ? "hide" : "disable", HeciMsg.HeciStateReq.HeciId
    ));

  Timeout = SPS_INITIALIZATION_TIMEOUT / 2;
  Status = pHeciPpi->HeciSend (
                       pHeciPpi,
                       HECI_DEFAULT_PCH_ID,
                       &Timeout,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_MKHI,
                       (UINT32*)&HeciMsg.HeciStateReq,
                       sizeof (HeciMsg.HeciStateReq)
                       );
  if (!EFI_ERROR (Status)) {
    //
    // We don't expect response if it is HECI-1 hidden/disabled.
    // ME executes the operation before we can read response, so it fails.
    //
    if (HeciId == 1) {
      HeciMsg.HeciStateRsp.Mkhi.Data = 0x00008DFF;
      HeciMsg.HeciStateRsp.Response = 0;
    } else {
      Status = pHeciPpi->HeciReceive (
                           pHeciPpi,
                           HECI_DEFAULT_PCH_ID,
                           &Timeout,
                           NULL,
                           NULL,
                           (UINT32*)&HeciMsg.HeciStateRsp,
                           &RspLen
                           );
    }
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot send HECI_CHANGE_STATE (%r)\n", SpsPrefix, Status));
  } else if (HeciMsg.HeciStateRsp.Mkhi.Data != 0x00008DFF) {
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR: Invalid HECI_CHANGE_STATE response (MKHI: 0x%X)\n",
      SpsPrefix,
      HeciMsg.HeciStateRsp.Mkhi.Data
      ));
  } else if (HeciMsg.HeciStateRsp.Response != 0) {
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR: HECI_CHANGE_STATE for HECI-%d failed (%d)\n",
      SpsPrefix,
      HeciId, HeciMsg.HeciStateRsp.Response
      ));
  }

  DEBUG ((DEBUG_INFO, "%a SpsFspSendHeciStateChange End\n", SpsPrefix));

  return Status;
} // SpsFspSendHeciStateChange ()

/**
    TBD.
    @param pHob -
    @retval EFI status is returned.
**/
EFI_STATUS
GetSpsConfiguration (
  SPS_INFO_HOB     *pHob
  )
{
  EFI_HOB_GUID_TYPE     *GuidHob;
  SPS_INFO_HOB          *pInfoData;

  DEBUG ((DEBUG_INFO, "%a Looking for SPS HOB info ..\n", SpsPrefix));

  GuidHob = GetFirstGuidHob (&gSpsInfoHobGuid);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot locate SPS HOB\n", SpsPrefix));
    return EFI_UNSUPPORTED;
  }

  pInfoData = GET_GUID_HOB_DATA (GuidHob);
  if (pInfoData == NULL) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot locate SPS HOB Data\n", SpsPrefix));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((
    DEBUG_INFO,
    "%a HOB: flow %d, feature set 0x%04X, pwr opt boot %d, cores2disable %d\n",
    SpsPrefix,
    pInfoData->WorkFlow, pInfoData->FeatureSet,
    pInfoData->PwrOptBoot, pInfoData->Cores2Disable
    ));

  CopyMem (pHob, pInfoData, sizeof(*pHob));

  return EFI_SUCCESS;
} // GetSpsConfiguration ()

/**
 @brief
  SpsFspReadyToBootEvent: Perform the remaining ME Flow initialization steps within FSP,
     as per ME BIOS Spec, at the Ready To Boot stage.

  @param PeiServices                    - General purpose services available to every PEIM.
  @param NotifyDescriptor               - The notification structure this PEIM registered on install.
  @param Ppi                            - The memory discovered PPI.  Not used.

  @retval EFI_SUCCESS                   - The function completed successfully

**/
EFI_STATUS
EFIAPI
SpsFspReadyToBootEvent (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{
  EFI_STATUS                    Status;
  HECI_CONTROL                  *pHeciPpi;
  SPS_INFO_HOB                  SpsHob;
  SPS_FEATURE_SET               SpsFeatureSet;
  SPS_PEI_CONFIG                *pSpsPolicy;
  ME_PEI_CONFIG                 *MePolicy;
  UINT8                         DisableEop = 0;
  UINT8                         Index;
  UINT8                         HeciHideInMe[4];
  HECI_FWS_REGISTER             MeFs1;
  SPS_MEFS2                     MeFs2;

  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "%a SpsFspReadyToBootEvent Start\n", SpsPrefix));

  MeFs1.ul = HeciPciRead32 (R_ME_HFS);
  DEBUG ((EFI_D_INFO, "[ME] MEFS1: 0x%08X\n", MeFs1.ul));
  MeFs2.UInt32 = HeciPciRead32 (R_ME_HFS_2);
  DEBUG ((EFI_D_INFO, "[ME] MEFS2: 0x%08X\n", MeFs2.UInt32));

  Status = GetSpsConfiguration (&SpsHob);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot locate SPS HOB Data\n", SpsPrefix));
    return Status;
  }
  SpsFeatureSet.Data = SpsHob.FeatureSet.Data;

  pSpsPolicy = GetSpsPreMemPeiConfigBlock (NULL);
  if (pSpsPolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot locate SPS Policy\n", SpsPrefix));
    return Status;
  }

  MePolicy = GetMePeiConfigBlock (NULL);
  if (MePolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot locate ME Policy\n", SpsPrefix));
    return Status;
  }
  Status = PeiServicesLocatePpi (&gHeciControlPpiGuid, 0, NULL, &pHeciPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot locate HECI PPI\n", SpsPrefix));
    return Status;
  } else {
    //
    // ME BIOS Spec: Step A11 in ME BIOS flow (optional)
    //     Send MCPT Bus Owner Proxy configuration if set
    //
    if (SpsFeatureSet.Bits.MctpProxy) {
      Status = SpsFspSendMcptBusOwnerProxy (pHeciPpi, pSpsPolicy->Common.SpsMctpBusOwner);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot Send MCPT Bus Owner Proxy configuration (%r)\n", SpsPrefix, Status));
      }
    }

    //
    // ME BIOS Spec: Step A12 in ME BIOS flow
    // Send HMRFPO_LOCK request if not disabled by ME debug option
    //
    if (pSpsPolicy->Common.MeHmrfpoLockEnabled) {
      Status = SpsFspSendHmrfpoLockRequest (pHeciPpi);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot Send HMRFPO_LOCK request(%r)\n", SpsPrefix, Status));
      }
    }

    if (pSpsPolicy->Common.MeHmrfpoEnableEnabled) {
      Status = SpsFspSendHmrfpoEnableRequest (pHeciPpi, HMRFPO_EN_REGION_INTEL_ME);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot Send HMRFPO_ENABLE request(%r)\n", SpsPrefix, Status));
      }
    }

    //
    // Send BIOS_CORE_DONE message
    //
    if (pSpsPolicy->Common.CoreBiosDoneEnabled) {
      Status = SpsFspSendCoreBiosDone (pHeciPpi);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot Send CoreBiosDone(%r)\n", SpsPrefix, Status));
      }
    }

    //
    // ME BIOS Spec: Step A13 in ME BIOS flow
    //    Send EOP message
    //
    if (MePolicy->EndOfPostMessage && !DisableEop) {
      Status = SpsFspSendEndOfPost (pHeciPpi);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot Send EOP message (%r)\n", SpsPrefix, Status));
      }
    }

    //
    // Send HECI_STATE_CHANGE if enabled in ME policy
    //
    HeciHideInMe[3] = (UINT8)pSpsPolicy->Common.Heci4HideInMe;
    HeciHideInMe[2] = (UINT8)pSpsPolicy->Common.Heci3HideInMe;
    HeciHideInMe[1] = (UINT8)pSpsPolicy->Common.Heci2HideInMe;
    HeciHideInMe[0] = (UINT8)pSpsPolicy->Common.Heci1HideInMe;
    SetMeUefiFwHealthStatus (Heci1HideInMe, HeciHideInMe[0] == 1 ? StatusOk : StatusOther);
    SetMeUefiFwHealthStatus (Heci2HideInMe, HeciHideInMe[1] == 1 ? StatusOk : StatusOther);
    SetMeUefiFwHealthStatus (Heci3HideInMe, HeciHideInMe[2] == 1 ? StatusOk : StatusOther);
    SetMeUefiFwHealthStatus (Heci4HideInMe, HeciHideInMe[3] == 1 ? StatusOk : StatusOther);
    for (Index = ARRAY_SIZE(HeciHideInMe); Index--; ) {
      if (HeciHideInMe[Index] != ME_SPS_HECI_IN_ME_OFF) {
        Status = SpsFspSendHeciStateChange (pHeciPpi, Index + 1,
                                            HeciHideInMe[Index] & 1); // In policy, HideInMe: 0 - nothing, 1 - hide, 2 - disable
                                                                      // Function expects: 0 - disable, 1 - hide
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "%a ERROR: Cannot Send HECI-%d State Change message (%r)\n", SpsPrefix, Index + 1, Status));
        }
      }
    }
  }

  //
  // Lock global reset in PMIR register
  //
  DEBUG ((DEBUG_INFO, "%a Disabling Global Reset capability\n", SpsPrefix));
  PmcDisableCf9GlobalResetWithLock ();

  DEBUG ((DEBUG_INFO, "%a SpsFspReadyToBootEvent End %r\n", SpsPrefix, Status));

  return Status;
} // SpsFspReadyToBootEvent ()

/**
 @brief
  SpsFspInitComplete: Perform the remaining ME Flow initialization steps within FSP, as per ME BIOS Spec.

  @param PpiHeci                    - Heci Protocol PPI
  @param pSpsPolicy                 - SPS Policy
  @param - TODO: add description for all parameters

  @retval EFI_DEVICE_ERROR - ME is not functional
  @retval EFI_INVALID_PARAMETER - PPIs passed to this function are NULL
  @retval EFI_SUCCESS - FSP init complete or ME in DFx mode

**/
EFI_STATUS
SpsFspInitComplete (
  IN HECI_CONTROL                  *PpiHeci,
  IN SPS_PEI_CONFIG                *pSpsPolicy,
  UINT8                            WorkFlow,
  UINT32                           FeatureSet,
  UINT32                           FeatureSet2,
  BOOLEAN                          PwrOptBoot,
  UINT8                            Cores2Disable)
{
  EFI_STATUS                    Status;
  SPS_FEATURE_SET               SpsFeatureSet;
  UINT32                        RspLen;

  union {
    MKHI_MSG_GET_FW_VERSION_REQ      FwVerReq;
    MKHI_MSG_GET_FW_VERSION_RSP      FwVerRsp;
  } HeciMsg;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a SpsFspInitComplete Start\n", SpsPrefix));

  if (NULL == PpiHeci || NULL == pSpsPolicy) {
    DEBUG ((DEBUG_ERROR, "%a SpsFspInitComplete: ERROR - PPIs passed cannot be NULL!\n", SpsPrefix));
    return EFI_INVALID_PARAMETER;
  }

  if (MeTypeIsDfx ()) {
    DEBUG ((DEBUG_WARN, "%a WARNING: ME working in DFx mode\n", SpsPrefix));
    return EFI_SUCCESS;
  }

  if (WorkFlow != ME_FUNCTIONAL) {
    DEBUG ((DEBUG_ERROR, "%a ERROR: ME FW not functional\n", SpsPrefix));
    return EFI_DEVICE_ERROR;
  }

  SpsFeatureSet.Data.Set1 = FeatureSet;
  SpsFeatureSet.Data.Set2 = FeatureSet2;
  DEBUG ((
    DEBUG_INFO,
    "%a SpsFspInitComplete HOB: features 0x%02x%02x, flow %d, boot mode %d, cores to disable %d\n",
    SpsPrefix,
    FeatureSet2,
    FeatureSet,
    WorkFlow,
    PwrOptBoot,
    Cores2Disable
    ));

  if (SpsFeatureSet.Bits.NodeManager) {
    DEBUG ((DEBUG_INFO, "%a NM Mode is not supported\n", SpsPrefix));
  } else {
    DEBUG ((DEBUG_INFO, "%a SiliconEnabling Mode\n", SpsPrefix));
  }

  //
  // Send MKHI_GET_FW_VERSION request to ME
  //
  DEBUG ((DEBUG_INFO, "[ME] Sending MKHI_GET_FW_VERSION request to ME\n"));

  ZeroMem (&HeciMsg, sizeof (HeciMsg));
  HeciMsg.FwVerReq.Mkhi.Fields.GroupId = MKHI_GRP_GEN;
  HeciMsg.FwVerReq.Mkhi.Fields.Command = MKHI_CMD_GET_FW_VERSION;
  RspLen = sizeof (HeciMsg.FwVerRsp);

  Status = PpiHeci->HeciSendAndReceive (
                      PpiHeci,
                      HECI_DEFAULT_PCH_ID,
                      NULL,
                      HECI_DEFAULT_RETRY_NUMBER,
                      SPS_CLIENTID_BIOS,
                      SPS_CLIENTID_ME_MKHI,
                      (UINT32*) &HeciMsg.FwVerReq,
                      sizeof (HeciMsg.FwVerReq),
                      (UINT32*) &HeciMsg.FwVerRsp,
                      &RspLen,
                      NULL,
                      NULL
                      );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] ERROR: Cannot send MKHI_GET_FW_VERSION (status: %r)\n", Status));
  } else if (!((HeciMsg.FwVerRsp.Mkhi.Fields.IsResponse) &&
              (MKHI_GRP_GEN == HeciMsg.FwVerReq.Mkhi.Fields.GroupId) &&
              (MKHI_CMD_GET_FW_VERSION == HeciMsg.FwVerReq.Mkhi.Fields.Command))) {
    DEBUG ((DEBUG_ERROR, "[ME] ERROR: Invalid MKHI_GET_FW_VERSION response (MKHI: 0x%X)\n", HeciMsg.FwVerRsp.Mkhi.Data));
  } else {
    DEBUG ((DEBUG_INFO,
      "[ME] Firmware version is:\n"
      "\tOperational: %02X:%d.%d.%d.%d\n"
      "\tRecovery:    %02X:%d.%d.%d.%d\n",
      (HeciMsg.FwVerRsp.Act.Major >> 8), (HeciMsg.FwVerRsp.Act.Major & 0x00FF), HeciMsg.FwVerRsp.Act.Minor, HeciMsg.FwVerRsp.Act.Patch, HeciMsg.FwVerRsp.Act.Build,
      (HeciMsg.FwVerRsp.Rcv.Major >> 8), (HeciMsg.FwVerRsp.Rcv.Major & 0x00FF), HeciMsg.FwVerRsp.Rcv.Minor, HeciMsg.FwVerRsp.Rcv.Patch, HeciMsg.FwVerRsp.Rcv.Build));
  }

  // Register ready to boot callback event to complete the SPS Init
  Status = PeiServicesNotifyPpi (&mSpsFspNotifyList[0]);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a SpsFspInitComplete End [%x]\n", SpsPrefix, Status));

  return Status;
} // SpsFspInitComplete ()

#endif // FSP_FLAG
