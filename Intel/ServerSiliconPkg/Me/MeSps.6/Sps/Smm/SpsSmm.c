/** @file
  SPS SMM driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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

//
// Standard header files.
//
#include <Uefi.h>

#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include "Library/MeTypeLib.h"
#include <Library/HobLib.h>
#include <ConfigBlock/SpsGetDxeConfigBlock.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Register/MeRegs.h>
#include <Library/MeUtilsLib.h>
#include <Library/MeSmmLib.h>

//
// Protocols
//
#include <ConfigBlock/Sps/SpsDxeConfig.h>
#include <Protocol/SpsSmmHmrfpoProtocol.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/HeciControlProtocol.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/SmmLegacyBoot.h>
#include <Protocol/SmmExitBootServices.h>
#include <Protocol/SpsSmmMeStorageProtocol.h>
#include <Protocol/SpsSmmBiosUpdateProtocol.h>

#include <Guid/HobList.h>
#include <Guid/SpsInfoHobGuid.h>
#include <MeWatchdogControlSmm.h>
#include <Library/MeWatchdogControlLib.h>

//
// Driver specific headers.
//
#include <MeDefines.h>
#include <Register/HeciRegs.h>
#include <MeAccess.h>
#include <Sps.h>
#include <SpsSmm.h>
#include <SpsRasNotify.h>
#include <SpsSmmHmrfpoExt.h>

#define HECI_TIMEOUT_TICK        100       ///< 0.1 ms tick

// global to store Nonce value read by ME HMRFPO_LOCK request
UINT64 gNonceValue = 0;
// global to store pointer of Smm Heci Protocol
HECI_CONTROL                  *gSmmHeci = NULL;
#if ME_TESTMENU_FLAG
//global to store disable sending HmrfpoLock message to ME
STATIC UINT8  gHmrfpoLockEnabled    = TRUE;
STATIC UINT8  gHmrfpoEnableEnabled  = FALSE;
STATIC UINT32 gHmrfpoEnableRegion   = HMRFPO_EN_REGION_INTEL_ME;
#endif // ME_TESTMENU_FLAG

// var to store DualBios state at ME side
static UINT8  mDualBiosEnabled = FALSE;

// var to disable sending RegionSelect message to ME after bios update
static UINT8  mRegionSelectEnabled = FALSE;

//global used to disable HECI before OS boot
#define HECI_MAX_INTERFACES   4
struct {
  UINT8 HeciId;
  UINT8 HeciState;
#if ME_TESTMENU_FLAG
  ME_UEFI_FW_HEALTH_STATUS_FIELD_ID HeciHealthStatusField;
#endif // ME_TESTMENU_FLAG
} gHeciHideInMe[HECI_MAX_INTERFACES] = {
#if ME_TESTMENU_FLAG
  {4, ME_SPS_HECI_IN_ME_OFF, Heci4HideInMe},
  {3, ME_SPS_HECI_IN_ME_OFF, Heci3HideInMe},
  {2, ME_SPS_HECI_IN_ME_OFF, Heci2HideInMe},
  {1, ME_SPS_HECI_IN_ME_OFF, Heci1HideInMe}
#else // ME_TESTMENU_FLAG
  {4, ME_SPS_HECI_IN_ME_OFF},
  {3, ME_SPS_HECI_IN_ME_OFF},
  {2, ME_SPS_HECI_IN_ME_OFF},
  {1, ME_SPS_HECI_IN_ME_OFF}
#endif // ME_TESTMENU_FLAG
  };

#define HECI1_IX 1
#define HECI2_IX 2
#define HECI3_IX 3
#define HECI4_IX 4

#pragma pack (1)
typedef struct {
  MKHI_MSG_HMRFPO_LOCK_REQ Req;
  MKHI_MSG_HMRFPO_LOCK_RSP Rsp;
} HECI_MSG_HMRFPO_LOCK;
#pragma pack ()

#pragma pack (1)
typedef struct {
  MKHI_MSG_HMRFPO_ENABLE_REQ Req;
  MKHI_MSG_HMRFPO_ENABLE_RSP Rsp;
} HECI_MSG_HMRFPO_ENABLE;
#pragma pack ()

#pragma pack (1)
typedef struct {
  MKHI_MSG_HMRFPO_MERESET_REQ Req;
  MKHI_MSG_HMRFPO_MERESET_RSP Rsp;
} HECI_MSG_HMRFPO_MERESET;
#pragma pack ()

#pragma pack (1)
typedef struct {
    MKHI_MSG_HECI_STATE_CHANGE_REQ HeciStateReq;
    MKHI_MSG_HECI_STATE_CHANGE_RSP HeciStateRsp;
} HECI_MSG_STATE_CHANGE;
#pragma pack ()

/** SPS SMM driver entry point.

  @param[in] ImageHandle    Handle of driver image
  @param[in] pSysTable      Pointer to the system table

  @return EFI_STATUS is returned.
 */
EFI_STATUS
EFIAPI
SpsSmmEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *pSysTable
  )
{

#if !defined (SIMICS_BIOS_FLAG)

  EFI_STATUS                    Status;
  EFI_SMM_BASE2_PROTOCOL        *pSmmBase;
  EFI_HANDLE                    Handle;

  SPS_INFO_HOB                  HobInfo;
  SPS_FEATURE_SET               FeatureSet;

  BOOLEAN                       InSmm;
  VOID                          *Registration;
  HECI_FWS_REGISTER             MeFs1;
  SPS_DXE_CONFIG                *SpsDxeConfig;
  SPS_SMM_HMRFPO_PROTOCOL       *pSpsSmmHmrfpo;
  BOOLEAN                       MeStorageServicesSupported;
  SPS_SMM_ME_STORAGE_PROTOCOL   *pSpsSmmMeStorage;
  BOOLEAN                       MeHmrfpoViaHeci3;

  SPS_SMM_BIOS_UPDATE_PROTOCOL  *pSpsSmmBiosUpdate;

  DEBUG ((DEBUG_INFO, "[SPS_SMM] SpsSmmEntryPoint started.\n"));

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, &pSmmBase);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't locate gEfiSmmBase2Protocol (%r).\n", Status));
    return Status;
  }

  // Test if the entry point is running in SMM mode. If not just return.
  InSmm = FALSE;
  pSmmBase->InSmm (pSmmBase, &InSmm);
  if (!InSmm) {
    return EFI_UNSUPPORTED;
  }

  // Check if ME SPS firmware is running
  if (!MeTypeIsSps ()) {
    MeFs1.ul = HeciPciRead32 (R_ME_HFS);
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Non SPS firmware in ME. Driver will be unloaded. (MEFS1: 0x%08X)\n", MeFs1.ul));

    /*
     * This is workaround for bug in efi core: unload of smm driver cause exception
     */
    //return EFI_UNSUPPORTED;
    return EFI_SUCCESS;
  }

  //save some of SpsPolicy values for further use
  SpsDxeConfig = GetSpsDxeConfigBlock ();
  if (SpsDxeConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Cannot get SpsPolicy configuration\n"
                        "                 Using predefined defaults\n"));
#if ME_TESTMENU_FLAG
    //set default values
    gHmrfpoLockEnabled    = TRUE;
    gHmrfpoEnableEnabled  = FALSE;
#endif // ME_TESTMENU_FLAG
    MeHmrfpoViaHeci3      = FALSE;

    gHeciHideInMe[3].HeciId    = HECI4_IX;
    gHeciHideInMe[3].HeciState = ME_SPS_HECI_IN_ME_OFF;
    gHeciHideInMe[2].HeciId    = HECI3_IX;
    gHeciHideInMe[2].HeciState = ME_SPS_HECI_IN_ME_OFF;
    gHeciHideInMe[1].HeciId    = HECI2_IX;
    gHeciHideInMe[1].HeciState = ME_SPS_HECI_IN_ME_OFF;
    gHeciHideInMe[0].HeciId    = HECI1_IX;
    gHeciHideInMe[0].HeciState = ME_SPS_HECI_IN_ME_OFF;
  } else { //SpsPolicy located
#if ME_TESTMENU_FLAG
    gHmrfpoLockEnabled    = (UINT8)SpsDxeConfig->Common.MeHmrfpoLockEnabled;
    gHmrfpoEnableEnabled  = (UINT8)SpsDxeConfig->Common.MeHmrfpoEnableEnabled;
    gHmrfpoEnableRegion   = (UINT32)SpsDxeConfig->MeHmrfpoEnableRegion;
#endif // ME_TESTMENU_FLAG
    MeHmrfpoViaHeci3      = (UINT8)SpsDxeConfig->MeHmrfpoViaHeci3;

    if (!MeHmrfpoViaHeci3) {
      gHeciHideInMe[0].HeciId    = HECI1_IX;
      gHeciHideInMe[0].HeciState = (UINT8)SpsDxeConfig->Common.Heci1HideInMe;
      gHeciHideInMe[2].HeciId    = HECI3_IX;
      gHeciHideInMe[2].HeciState = (UINT8)SpsDxeConfig->Common.Heci3HideInMe;
    } else {
      gHeciHideInMe[0].HeciId    = HECI3_IX;
      gHeciHideInMe[0].HeciState = (UINT8)SpsDxeConfig->Common.Heci3HideInMe;
      gHeciHideInMe[2].HeciId    = HECI1_IX;
      gHeciHideInMe[2].HeciState = (UINT8)SpsDxeConfig->Common.Heci1HideInMe;
    }
    gHeciHideInMe[1].HeciId    = HECI2_IX;
    gHeciHideInMe[1].HeciState = (UINT8)SpsDxeConfig->Common.Heci2HideInMe;
    gHeciHideInMe[3].HeciId    = HECI4_IX;
    gHeciHideInMe[3].HeciState = (UINT8)SpsDxeConfig->Common.Heci4HideInMe;
    mRegionSelectEnabled = (UINT8)SpsDxeConfig->Common.MeRegionSelectEnabled;
  }

  // Find HECI SMM protocol
  Status = gSmst->SmmLocateProtocol (&gHeciControlSmmProtocolGuid, NULL, &gSmmHeci);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: HECI SMM protocol not found (%r)\n", Status));
    return Status;
  }

  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (SPS_SMM_HMRFPO_PROTOCOL), (VOID**)&pSpsSmmHmrfpo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't allocate memory for SpsSmmHmrfpo protocol!. Status=%r\n", Status));
    return Status;
  }

  //init ME versions
  RetrieveMeVersions (pSpsSmmHmrfpo);

  //install SpsSmmHmrfpo Protocol
  pSpsSmmHmrfpo->Lock      = Hmrfpo_Lock;
  pSpsSmmHmrfpo->Enable    = Hmrfpo_Enable;
  pSpsSmmHmrfpo->MeReset   = Hmrfpo_MeReset;

  Handle = NULL;
  Status = gSmst->SmmInstallProtocolInterface (
                    &Handle,
                    &gSpsSmmHmrfpoProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    pSpsSmmHmrfpo
                    );

  ASSERT_EFI_ERROR (Status);

  Status = InitializeHmrfpoExt (pSpsSmmHmrfpo);
  ASSERT_EFI_ERROR (Status);

  //Decide if callbacks will be needed
  if (gHeciHideInMe[0].HeciState != ME_SPS_HECI_IN_ME_OFF ||
      gHeciHideInMe[1].HeciState != ME_SPS_HECI_IN_ME_OFF ||
      gHeciHideInMe[3].HeciState != ME_SPS_HECI_IN_ME_OFF ||
      gHeciHideInMe[2].HeciState != ME_SPS_HECI_IN_ME_OFF
  ) {
    //HECI needs to be disabled at the end of boot - register callbacks to do it

    //Register callback on ExitBootServices event
    Registration = NULL;
    Status = gSmst->SmmRegisterProtocolNotify (
                      &gEdkiiSmmExitBootServicesProtocolGuid,
                      SpsSmmExitBootServicesCallback,
                      &Registration
                      );

    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "[SPS_SMM] Callback on ExitBootServices event registered .\n"));
    } else {
      DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't register callback on ExitBootServices event.\n"));
      ASSERT_EFI_ERROR (Status);
    }

    //Register callback on LegacyBoot event
    Registration = NULL;
    Status = gSmst->SmmRegisterProtocolNotify (
                      &gEdkiiSmmLegacyBootProtocolGuid,
                      SpsSmmExitBootServicesCallback,
                      &Registration
                      );

    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "[SPS_SMM] Callback on LegacyBoot event registered .\n"));
    } else {
      DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't register callback on ExitBootServices event.\n"));
      ASSERT_EFI_ERROR (Status);
    }
  }

#if ME_TESTMENU_FLAG

  // Send HmrfpoLock to get Nonce value
  if (gHmrfpoLockEnabled) {

#endif // ME_TESTMENU_FLAG
    gNonceValue = 0;
    HeciReq_HmrfpoLock (&gNonceValue, HMRFPO_EN_REGION_INTEL_ME);
    if (gNonceValue != 0) {
      DEBUG ((DEBUG_INFO, "[SPS_SMM] Nonce saved.\n"));
    } else {
      DEBUG ((DEBUG_ERROR, "[SPS SMM] ERROR: Can't get nonce!\n"));
    }

#if ME_TESTMENU_FLAG

    // send HECI enable if needed
    if (gHmrfpoEnableEnabled) {
      DEBUG ((DEBUG_INFO, "[SPS_SMM] HmrfpoEnable needs to be send after HmrfpoLock.\n"));
      HeciReq_HmrfpoEnable (gNonceValue, gHmrfpoEnableRegion);
    }

  } else {
    // if MeHmrfpoLockEnabled is set to FALSE in setup don't send any HMRFPO_LOCK message - ME updated tool needs to get nonce...
    gNonceValue = 0;
    DEBUG ((DEBUG_INFO, "[SPS_SMM] HMRFPO_LOCK disabled in setup.\n"));
  }

#endif // ME_TESTMENU_FLAG

  InstallSpsRasNotifyProtocol ();

  MeStorageServicesSupported = FALSE;
  // Locate SPS HOB
  Status = GetSpsConfiguration (&HobInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't get SPS configuration. (%r)\n", Status));
    mDualBiosEnabled = FALSE;
    DEBUG ((DEBUG_INFO, "[SPS_SMM] ME doesn't support DualBios feature.\n"));
  } else {
    FeatureSet.Data.Set1 = HobInfo.FeatureSet.Data.Set1;
    FeatureSet.Data.Set2 = HobInfo.FeatureSet.Data.Set2;

    //Check if DualBios feature is supported by ME
    if (HobInfo.FeatureSet.Bits.DualBIOSSupport) {
      mDualBiosEnabled = TRUE;
      DEBUG ((DEBUG_INFO, "[SPS_SMM] ME support DualBios feature.\n"));
    } else {
      mDualBiosEnabled = FALSE;
      DEBUG ((DEBUG_INFO, "[SPS_SMM] ME doesn't support DualBios feature.\n"));
    }

    //Check if DualBios Watchdog is enabled
    if (HobInfo.DualBIOSWatchdogEnabled) {
      //Register callback on ReadyToBoot event
      Registration = NULL;
      Status = gSmst->SmmRegisterProtocolNotify (
                        &gEfiEventReadyToBootGuid,
                        SpsSmmReadyToBootCallback,
                        &Registration
                        );
    }

    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "[SPS_SMM] Callback on ReadyToBoot event registered .\n"));
    } else {
      DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't register callback on ReadyToBoot event.\n"));
      ASSERT_EFI_ERROR (Status);
    }
    if (HobInfo.FeatureSet.Bits.MeStorageServices) {
      MeStorageServicesSupported = TRUE;
    }
  }
  DEBUG ((DEBUG_INFO, "[SPS_SMM] ME Storage Service %aavailable\n",
    (MeStorageServicesSupported) ? "" : "NOT "));

  // prepare and install SpsSmmBiosUpdate Protocol
  Status = gSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (*pSpsSmmBiosUpdate), (VOID **) (&pSpsSmmBiosUpdate));
  ASSERT_EFI_ERROR (Status);

  if (!EFI_ERROR (Status)) {

    pSpsSmmBiosUpdate->GetActiveRegionNum =
                            (BIOS_UPDATE_GET_ACTIVE_REGION_NUM)   BiosUpdate_GetActiveRegionNum;
    pSpsSmmBiosUpdate->UpdateAllowed = (BIOS_UPDATE_ALLOWED)      BiosUpdate_UpdateAllowed;
    pSpsSmmBiosUpdate->RegionSelect = (BIOS_UPDATE_REGION_SELECT) BiosUpdate_RegionSelect;

    Handle = NULL;
    Status = gSmst->SmmInstallProtocolInterface (
                      &Handle,
                      &gSpsSmmBiosUpdateProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      pSpsSmmBiosUpdate
                      );
    if (EFI_ERROR (Status)){
      DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't install SpsBiosUpdateProtocol. (%r)\n", Status));
      ASSERT_EFI_ERROR (Status);
    }
  }

  if (MeStorageServicesSupported == TRUE) {
    DEBUG ((DEBUG_INFO, "[SPS_SMM] Install ME Storage protocol\n"));
    Status = gSmst->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      sizeof (*pSpsSmmMeStorage),
                      (VOID **) (&pSpsSmmMeStorage)
                      );
    if (!EFI_ERROR (Status)) {
      pSpsSmmMeStorage->MeStorageSmmEntryRead = MeStorageSmmEntryRead;
      pSpsSmmMeStorage->MeStorageSmmEntryWrite = MeStorageSmmEntryWrite;

      Handle = NULL;
      Status = gSmst->SmmInstallProtocolInterface (
                        &Handle,
                        &gSpsSmmMeStorageProtocolGuid,
                        EFI_NATIVE_INTERFACE,
                        pSpsSmmMeStorage
                        );
      if (EFI_ERROR (Status)){
        DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't install gSpsSmmMeStorageProtocolGuid. (%r)\n", Status));
        ASSERT_EFI_ERROR (Status);
      }
    } else {
      DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't allocate gSpsSmmMeStorageProtocolGuid. (%r)\n", Status));
      ASSERT_EFI_ERROR (Status);
    }
  }

  // DualBios Watchdog initialize
  if (mDualBiosEnabled) {
    MeWatchdogLibInitialize ();
  }

  DEBUG ((DEBUG_INFO, "[SPS_SMM] SpsSmmEntryPoint ends.\n"));

#endif // !SIMICS_BIOS_FLAG

  return EFI_SUCCESS;
} // SpsSmmEntryPoint


/** Procedure locates and return SPS HOB data

  @param[in]  *SystemTable  Pointer of EFI Sysytem Table
  @param[out] *pHob         Buffer for SPS HOB data

  @return EFI_STATUS is returned.

 */
EFI_STATUS
GetSpsConfiguration (
  OUT SPS_INFO_HOB          *pHob
  )
{
  EFI_HOB_GUID_TYPE     *GuidHob;
  SPS_INFO_HOB          *pInfoData;

  if ( !MeTypeIsSps () ) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "[SPS_SMM] Looking for SPS HOB info from PEI\n"));

  GuidHob = GetFirstGuidHob (&gSpsInfoHobGuid);
  if (GuidHob == NULL){
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Cannot locate SPS HOB\n"));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }
  pInfoData = GET_GUID_HOB_DATA (GuidHob);

  if (pInfoData == NULL){
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Cannot locate SPS HOB data\n"));
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  CopyMem (pHob, pInfoData, sizeof (*pHob));

  if (pInfoData->WorkFlow != ME_FUNCTIONAL) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: ME non functional.\n"));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}


/** Procedure to send HMRFPO_LOCK HECI Request

    @param[in,out] HeciMsgLock struct for Heci message
    @return EFI_STATUS is returned.
*/
EFI_STATUS
CallMsg_HmrfpoLock (
  IN OUT HECI_MSG_HMRFPO_LOCK *HeciMsgLock
  )
{
  UINT32     RspLen;
  EFI_STATUS Status;
  UINT32     Timeout = SPS_INITIALIZATION_TIMEOUT/2;

  RspLen = sizeof (HeciMsgLock->Rsp);
  Status = gSmmHeci->HeciSendAndReceive (
                       gSmmHeci,
                       HECI_DEFAULT_PCH_ID,
                       &Timeout,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_MKHI,
                       (UINT32*) &(HeciMsgLock->Req),
                       sizeof (HeciMsgLock->Req),
                       (UINT32*) &(HeciMsgLock->Rsp),
                       &RspLen,
                       NULL,
                       NULL
                       );

  DEBUG ((DEBUG_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", HeciPciRead32 (R_ME_HFS)));

  // Reset HECI to clear NONCE data
  gSmmHeci->HeciReset (
              gSmmHeci,
              HECI_DEFAULT_PCH_ID,
              NULL
              );

  return Status;
}

/** Procedure to send HMRFPO_LOCK HECI Request

    @param[in,out] Nonce           Buffer to store nonce value from HECI message response
    @param[in]     RegionToLock    Region which should be locked
    @return EFI_STATUS is returned.
*/
EFI_STATUS
HeciReq_HmrfpoLock (
  IN OUT UINT64        *Nonce,
  IN     UINT32        RegionToLock
  )
{
  EFI_STATUS Status;
  HECI_MSG_HMRFPO_LOCK HeciMsgLock;

  DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending HMRFPO_LOCK, MEFS1: 0x%08X\n", HeciPciRead32 (R_ME_HFS)));
  DEBUG ((DEBUG_INFO, "  RegionToLock: %d\n", RegionToLock));
  // Construct HECI HMRFPO_LOCK request
  ZeroMem (&HeciMsgLock, sizeof (HeciMsgLock));
  HeciMsgLock.Req.Mkhi.Data = HECI_MSG_LOCK_REQ_MKHI_HDR;
  HeciMsgLock.Req.RegionSelect = RegionToLock;

  // Call HMRFPO message: lock
  Status = CallMsg_HmrfpoLock (&HeciMsgLock);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] HMRFPO_LOCK send failed (%r)\n", Status));
    return EFI_UNSUPPORTED;
  }

  if (HeciMsgLock.Rsp.Mkhi.Data != HECI_MSG_LOCK_RSP_MKHI_HDR) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Invalid HMRFPO_LOCK response (MKHI: 0x%08X)\n",
      HeciMsgLock.Rsp.Mkhi.Data));

    return EFI_UNSUPPORTED;
  }

  if (HeciMsgLock.Rsp.Status != 0) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: HMRFPO_LOCK failed (cause: %d)\n",
      HeciMsgLock.Rsp.Status));

    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "[SPS_SMM] HMRFPO_LOCK reports success\n"));
  if (Nonce != NULL) {
    *Nonce = HeciMsgLock.Rsp.Nonce;
  } //save the nonce value

  return EFI_SUCCESS;
}

/** Procedure to send HMRFPO_ENABLE HECI Request

    @param[in,out] HeciMsgEnable struct for Heci message
    @return EFI_STATUS is returned.
*/
EFI_STATUS
CallMsg_HmrfpoEnable (
  IN OUT HECI_MSG_HMRFPO_ENABLE *HeciMsgEnable
  )
{
  UINT32     RspLen;
  EFI_STATUS Status;
  UINT32     Timeout = SPS_INITIALIZATION_TIMEOUT/2;

  RspLen = sizeof (HeciMsgEnable->Rsp);
  Status = gSmmHeci->HeciSendAndReceive (
                       gSmmHeci,
                       HECI_DEFAULT_PCH_ID,
                       &Timeout,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_MKHI,
                       (UINT32*) &(HeciMsgEnable->Req),
                       sizeof (HeciMsgEnable->Req),
                       (UINT32*) &(HeciMsgEnable->Rsp),
                       &RspLen,
                       NULL,
                       NULL
                       );

  DEBUG ((DEBUG_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", HeciPciRead32 (R_ME_HFS)));

  // Reset HECI to clear NONCE data
  gSmmHeci->HeciReset (
              gSmmHeci,
              HECI_DEFAULT_PCH_ID,
              NULL
              );

  return Status;
}

/** Procedure to send HMRFPO_ENABLE HECI Request

    Please note: After sending HmrfpoEnable message ME needs to reset itself
    - this means that this is highly probable that this function fails at first few tries.
    This is recommended to call this function in a loop a few times until success is returned.
    Please refer to ME-Bios spec. for more details.

    @param[in]  *Nonce          Nonce value for current session
    @param[in]  RegionToEnable  Region, to request access enabling (HMRFPO_EN_REGION_*)

    @return EFI_STATUS is returned.
*/
EFI_STATUS
HeciReq_HmrfpoEnable (
  IN UINT64   Nonce,
  IN UINT32   RegionToEnable
  )
{
  EFI_STATUS Status;
  HECI_MSG_HMRFPO_ENABLE HeciMsgEnable;

  DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending HMRFPO_ENABLE, MEFS1: 0x%08X\n", HeciPciRead32 (R_ME_HFS)));
  DEBUG ((DEBUG_INFO, "  RegionToEnable: %d\n", RegionToEnable));

  // Construct HMRFPO_ENABLE request message
  ZeroMem (&HeciMsgEnable, sizeof (HeciMsgEnable));

  HeciMsgEnable.Req.Mkhi.Data = HECI_MSG_ENABLE_REQ_MKHI_HDR;
  HeciMsgEnable.Req.Nonce = Nonce;
  HeciMsgEnable.Req.RegionSelect = RegionToEnable;

  Status = CallMsg_HmrfpoEnable (&HeciMsgEnable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: HMRFPO_ENABLE sending failed (%r)\n",Status));
    return Status;
  }

  if (HeciMsgEnable.Rsp.Mkhi.Data != HECI_MSG_ENABLE_RSP_MKHI_HDR) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Invalid HMRFPO_ENABLE response (MKHI: 0x%08X)\n",
      HeciMsgEnable.Rsp.Mkhi.Data));
    return EFI_UNSUPPORTED;
  }

  if (HeciMsgEnable.Rsp.Status != 0) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: HMRFPO_ENABLE refused (cause: %d)\n",
      HeciMsgEnable.Rsp.Status));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "[SPS_SMM] HMRFPO_ENABLE reports success.\n"));
  return EFI_SUCCESS;
}

/** Procedure to send HMRFPO_MERESET HECI Request

    @param[in,out] HeciMsgMeReset struct for Heci message
    @return EFI_STATUS is returned.
*/
EFI_STATUS
CallMsg_HmrfpoMeReset (
  IN OUT HECI_MSG_HMRFPO_MERESET *HeciMsgMeReset
  )
{
  UINT32     RspLen;
  EFI_STATUS Status;
  UINT32     Timeout = SPS_INITIALIZATION_TIMEOUT/2;

  RspLen = sizeof (HeciMsgMeReset->Rsp);
  Status = gSmmHeci->HeciSendAndReceive (
                       gSmmHeci,
                       HECI_DEFAULT_PCH_ID,
                       &Timeout,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_MKHI,
                       (UINT32*) &(HeciMsgMeReset->Req),
                       sizeof (HeciMsgMeReset->Req),
                       (UINT32*) &(HeciMsgMeReset->Rsp),
                       &RspLen,
                       NULL,
                       NULL
                       );

  DEBUG ((DEBUG_INFO, "[SPS_SMM] MEFS1: 0x%08X\n", HeciPciRead32 (R_ME_HFS)));

  // Reset HECI to clear NONCE data
  gSmmHeci->HeciReset (
              gSmmHeci,
              HECI_DEFAULT_PCH_ID,
              NULL
              );

  return Status;
}

/** Procedure to send HMRFPO_RESET HECI Request

    Please note: HmrfpoReset message make ME to reset itself
    - this means that this is highly probable that this function return fail (timeout occurs).

    Please refer to ME-Bios spec. for more details.

    @param[in]    *Nonce    Nonce value for current session

    @return EFI_STATUS is returned.
*/
EFI_STATUS
HeciReq_HmrfpoMeReset (
  IN UINT64                Nonce
  )
{
  EFI_STATUS Status;
  HECI_MSG_HMRFPO_MERESET HeciMsgMeReset;

  DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending HMRFPO_MERESET, MEFS1: 0x%08X\n", HeciPciRead32 (R_ME_HFS)));

  // Construct HMRFPO_MERESET request message
  ZeroMem (&HeciMsgMeReset, sizeof (HeciMsgMeReset));

  HeciMsgMeReset.Req.Mkhi.Data = HECI_MSG_MERESET_REQ_MKHI_HDR;
  HeciMsgMeReset.Req.Nonce = Nonce;

  Status = CallMsg_HmrfpoMeReset (&HeciMsgMeReset);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] HMRFPO_MERESET send failed (%r)\n", Status));
    return Status;
  }

  if (HeciMsgMeReset.Rsp.Mkhi.Data != HECI_MSG_MERESET_RSP_MKHI_HDR) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Invalid HMRFPO_MERESET response (MKHI: 0x%08X)\n",
      HeciMsgMeReset.Rsp.Mkhi.Data));
    return EFI_UNSUPPORTED;
  }

  if (HeciMsgMeReset.Rsp.Status != 0) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: HMRFPO_MERESET refused (cause: %d)\n",
      HeciMsgMeReset.Rsp.Status));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "[SPS_SMM] HMRFPO_MERESET reports success.\n"));
  return EFI_SUCCESS;
}

/** Procedure to send BiosUpdateRegionSwap HECI Request

    @param[in]    *pSmmHeci  Pointer to SMM HECI driver
    @param[in]    Nonce     Nonce value for current session

    @return EFI_STATUS is returned.
*/
EFI_STATUS
HeciReq_RegionSelect (
  IN HECI_CONTROL  *pSmmHeci,
  IN UINT64        Nonce
  )
{
  UINT32     BufLen;
  EFI_STATUS Status;
  UINT32     Timeout = SPS_INITIALIZATION_TIMEOUT/2;

  struct {
    HECI_MSG_BIOS_UPDATE_REGION_SELECT_REQ Req;
    HECI_MSG_BIOS_UPDATE_REGION_SELECT_RSP Rsp;
  } HeciMsg;

  if ( !MeTypeIsSps () ) {
    return EFI_UNSUPPORTED;
  }

  // Initializing the HeciMsg
  ZeroMem (&HeciMsg, sizeof (HeciMsg));

  DEBUG ((DEBUG_INFO,
         "[SPS_SMM] Sending SPS_CMD_BIOS_UPDATE_REGION_SELECT_REQ\n"));

  //construct Heci request
  HeciMsg.Req.Command           = SPS_CMD_BIOS_UPDATE_REGION_SELECT_REQ;
  HeciMsg.Req.Nonce             = Nonce;
  HeciMsg.Req.BootTimeout       = 9000; //default: 6min. timeout, temporary increased to 15 min.
  HeciMsg.Req.BootTimeoutAction = BIOS_UPDATE_REGION_SELECT_TIMEOUT_ACTION_RESET;
  HeciMsg.Req.BiosRegionNum    = BIOS_UPDATE_REGION_SELECT_REG_NUM_OTHER;

  BufLen = sizeof (HeciMsg.Rsp);
  Status = gSmmHeci->HeciSendAndReceive (
                       gSmmHeci,
                       HECI_DEFAULT_PCH_ID,
                       &Timeout,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_SPS,
                       (UINT32*) &(HeciMsg.Req),
                       sizeof (HeciMsg.Req),
                       (UINT32*) &(HeciMsg.Rsp),
                       &BufLen,
                       NULL,
                       NULL
                       );

  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR,
           "[SPS_SMM] ERROR: Cannot send SPS_CMD_BIOS_UPDATE_REGION_SELECT_REQ (%r)\n",
           Status));

  } else if (HeciMsg.Rsp.Command != SPS_CMD_BIOS_UPDATE_REGION_SELECT_RSP) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: SPS_CMD_BIOS_UPDATE_REGION_SELECT response failed\n"));

    Status = EFI_UNSUPPORTED;

  } else if (HeciMsg.Rsp.Status != HECI_RSP_STATUS_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: BIOS_UPDATE_REGION_SELECT failed (status: %d)\n", HeciMsg.Rsp.Status));

    Status = EFI_UNSUPPORTED;

  } else {
    //no error, status equals success
    DEBUG ((DEBUG_INFO, "[SPS_SMM] BIOS_UPDATE_REGION_SELECT reports success.\n"));
  }

  // Reset HECI to clear NONCE data
  gSmmHeci->HeciReset (
              gSmmHeci,
              HECI_DEFAULT_PCH_ID,
              NULL
              );

  return Status;
}

/** Procedure to send BiosUpdateWatchdogControl HECI Request

    @param[in]    *pSmmHeci  Pointer to SMM HECI driver
    @param[in]    Action     Action to send in request

    @return EFI_STATUS is returned.
*/
EFI_STATUS
HeciReq_WatchdogControl (
  HECI_CONTROL  *pSmmHeci,
  UINT8         Action
  )
{
  UINT32     BufLen;
  EFI_STATUS Status;
  UINT32     Timeout = SPS_INITIALIZATION_TIMEOUT/2;

  struct {
    HECI_MSG_BIOS_UPDATE_WATCHDOG_CONTROL_REQ Req;
    HECI_MSG_BIOS_UPDATE_WATCHDOG_CONTROL_RSP Rsp;
  } HeciMsg;

  if ( !MeTypeIsSps () ) {
    return EFI_UNSUPPORTED;
  }

  // Initializing the HeciMsg
  ZeroMem (&HeciMsg, sizeof (HeciMsg));

  DEBUG ((DEBUG_INFO,
    "[SPS_SMM] Sending SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ\n"));

  //construct Heci request
  HeciMsg.Req.Command    = SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ;
  HeciMsg.Req.Nonce      = gNonceValue;
  HeciMsg.Req.ActionFlag = Action;
  DEBUG ((DEBUG_INFO, "[SPS_SMM]  Nonce: %s\n", (HeciMsg.Req.Nonce == 0) ? "0" : "VALID"));

  BufLen = sizeof (HeciMsg.Rsp);

  Status = gSmmHeci->HeciSendAndReceive (
                       gSmmHeci,
                       HECI_DEFAULT_PCH_ID,
                       &Timeout,
                       HECI_DEFAULT_RETRY_NUMBER,
                       SPS_CLIENTID_BIOS,
                       SPS_CLIENTID_ME_SPS,
                       (UINT32*) &(HeciMsg.Req),
                       sizeof (HeciMsg.Req),
                       (UINT32*) &(HeciMsg.Rsp),
                       &BufLen,
                       NULL,
                       NULL
                       );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "[SPS_SMM] ERROR: Cannot send SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_REQ (%r)\n",
      Status));

  } else if (HeciMsg.Rsp.Command != SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL_RSP) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: SPS_CMD_BIOS_UPDATE_WATCHDOG_CONTROL response failed\n"));
    Status = EFI_UNSUPPORTED;

  } else if (HeciMsg.Rsp.Status != HECI_RSP_STATUS_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: BIOS_UPDATE_WATCHDOG_CONTROL failed (status: %d)\n", HeciMsg.Rsp.Status));
    Status = EFI_UNSUPPORTED;

  } else {
    //no error, status OK
    DEBUG ((DEBUG_INFO, "[SPS_SMM] BIOS_UPDATE_WATCHDOG_CONTROL reports success\n"));
  }

  return Status;
}

/**
  Function which implements SpsSmmHmrfpo Protocol Lock function

  @return EFI_STATUS is returned.
*/
EFI_STATUS
Hmrfpo_Lock (
  VOID
  )
{
  EFI_STATUS        Status;

#if ME_TESTMENU_FLAG
  if (!gHmrfpoLockEnabled) {
    DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending Hmrfpo_Lock message is disabled in setup\n"));
    return EFI_UNSUPPORTED;
  }
#endif // ME_TESTMENU_FLAG

  Status = HeciReq_HmrfpoLock (NULL, HMRFPO_EN_REGION_INTEL_ME);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS_SMM] ERROR: HeciReq_HmrfpoLock (for region: %d) command returns error (%r)\n",
      HMRFPO_EN_REGION_INTEL_ME,
      Status
      ));
  }

  return Status;
}

/**
  Function which implements SpsSmmHmrfpo Protocol MeReset function

  @return EFI_STATUS is returned.
*/
EFI_STATUS
Hmrfpo_MeReset (
  VOID
  )
{
  EFI_STATUS            Status;

  if (gNonceValue == 0) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Nonce value unknown. Operation not supported\n"));
    return EFI_UNSUPPORTED;
  }

  Status = HeciReq_HmrfpoMeReset (gNonceValue);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: HeciReq_HmrfpoMeReset command returns error (%r)\n", Status));
    return Status;
  }
  return EFI_SUCCESS;
}

/**
  Function which implements SpsSmmHmrfpo Protocol Enable function

  @param[in] RegionToEnable  Region, to request access enabling (HMRFPO_EN_REGION_*)

  @return EFI_STATUS is returned.
*/
EFI_STATUS
Hmrfpo_Enable (
  IN UINT32  RegionToEnable
  )
{
  EFI_STATUS    Status;

  if (RegionToEnable != HMRFPO_EN_REGION_INTEL_ME) {

    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Unknown region. Operation not supported\n"));
    return EFI_UNSUPPORTED;
  }

  if (gNonceValue == 0) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Nonce value unknown. Operation not supported\n"));
    return EFI_UNSUPPORTED;
  }

  Status = HeciReq_HmrfpoEnable (gNonceValue, RegionToEnable);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: HeciReq_HmrfpoEnable command returns error (%r)\n", Status));
    return Status;
  }
  return EFI_SUCCESS;
}

/** Function set the ME versions red by MKHI messages

  @param[in,out]  pSmsSmmHmrfpo -
  @return         EFI_STATUS
*/
VOID RetrieveMeVersions (
  IN OUT SPS_SMM_HMRFPO_PROTOCOL *pSpsSmmHmrfpo
  )
{
  EFI_STATUS          Status;
  GET_FW_VER_ACK_DATA HeciMsgFwVersions;

  //
  // Send MKHI_GET_FW_VERSION request to ME
  //
  DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending MKHI_GET_FW_VERSION, expect success\n"));

  Status = GetMeFwVersionSmm (&HeciMsgFwVersions);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] ERROR: Cannot send MKHI_GET_FW_VERSION (%r)\n",Status));
  } else {
    // no errors - copy data from HECI message

    // Active operational firmware
    pSpsSmmHmrfpo->ActiveFwVersion.Minor  = (UINT16) HeciMsgFwVersions.CodeMinor;
    pSpsSmmHmrfpo->ActiveFwVersion.Major  = (UINT16) HeciMsgFwVersions.CodeMajor;
    pSpsSmmHmrfpo->ActiveFwVersion.Build  = (UINT16) HeciMsgFwVersions.CodeBuildNo;
    pSpsSmmHmrfpo->ActiveFwVersion.Patch  = (UINT16) HeciMsgFwVersions.CodeHotFix;

    // Recovery firmware
    pSpsSmmHmrfpo->RecoveryFwVersion.Minor  = (UINT16) HeciMsgFwVersions.RcvyMinor;
    pSpsSmmHmrfpo->RecoveryFwVersion.Major  = (UINT16) HeciMsgFwVersions.RcvyMajor;
    pSpsSmmHmrfpo->RecoveryFwVersion.Build  = (UINT16) HeciMsgFwVersions.RcvyBuildNo;
    pSpsSmmHmrfpo->RecoveryFwVersion.Patch  = (UINT16) HeciMsgFwVersions.RcvyHotFix;

    //
    // ME in dual-image configuration provides the version of the backup image yet
    // Backup operational firmware (optional): when not provided, zeros are copied
    //
    pSpsSmmHmrfpo->BackupFwVersion.Minor  = (UINT16) HeciMsgFwVersions.FitcMinor;
    pSpsSmmHmrfpo->BackupFwVersion.Major  = (UINT16) HeciMsgFwVersions.FitcMajor;
    pSpsSmmHmrfpo->BackupFwVersion.Build  = (UINT16) HeciMsgFwVersions.FitcBuildNo;
    pSpsSmmHmrfpo->BackupFwVersion.Patch  = (UINT16) HeciMsgFwVersions.FitcHotFix;
  }
}

/**
  This code is called on ReadyToBoot event

  @param[in] Protocol   Points to the protocol's unique identifier
  @param[in] Interface  Points to the interface instance
  @param[in] Handle     The handle on which the interface was installed

  @retval EFI_SUCCESS   always success successfully
*/
EFI_STATUS
EFIAPI
SpsSmmReadyToBootCallback (
  IN CONST EFI_GUID     *Protocol,
  IN VOID               *Interface,
  IN EFI_HANDLE         Handle
  )
{
  EFI_STATUS      Status;
  HECI_CONTROL    *pSmmHeci;

  DEBUG ((DEBUG_INFO, "[SPS_SMM] SpsSmmReadyToBootCallback enter\n"));

  Status = gSmst->SmmLocateProtocol (&gHeciControlSmmProtocolGuid, NULL, &pSmmHeci);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't locate SmmHeciProtocol.\n"));
    ASSERT_EFI_ERROR (Status);
  } else {
    Status = HeciReq_WatchdogControl (pSmmHeci, BIOS_UPDATE_WATCHDOG_CTRL_FLAG_STOP);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: WatchdogControl command returns error=%r.\n", Status));
    } else {
      DEBUG ((DEBUG_INFO, "[SPS_SMM] DualBiosWatchdog: Watchdog stopped. \n"));
    }
  }

  DEBUG ((DEBUG_INFO, "[SPS_SMM] SpsSmmReadyToBootCallback exit\n"));
  return EFI_SUCCESS;
}

/**
  This code is called on ExitBootServices event (or LegacyBoot as well)

  @param[in] Protocol   Points to the protocol's unique identifier
  @param[in] Interface  Points to the interface instance
  @param[in] Handle     The handle on which the interface was installed

  @retval EFI_SUCCESS   always success successfully
*/
EFI_STATUS
EFIAPI
SpsSmmExitBootServicesCallback (
  IN CONST EFI_GUID     *Protocol,
  IN VOID               *Interface,
  IN EFI_HANDLE         Handle
  )
{
  EFI_STATUS      Status;
  UINT8           Index;
  UINT32          RspLen;
  UINT32          Timeout = SPS_INITIALIZATION_TIMEOUT/2;
  HECI_MSG_STATE_CHANGE HeciStateChange;

  DEBUG ((DEBUG_INFO, "[SPS_SMM] SpsSmmExitBootServicesCallback enter\n"));

  //
  // Send HECI_STATE_CHANGE if enabled in ME policy
  //

  for (Index = HECI_MAX_INTERFACES; Index--;) {
    if (gHeciHideInMe[Index].HeciState != ME_SPS_HECI_IN_ME_OFF) {
      HeciStateChange.HeciStateReq.Mkhi.Data = 0x00000DFF;
      HeciStateChange.HeciStateReq.Nonce = gNonceValue;
      HeciStateChange.HeciStateReq.HeciId = gHeciHideInMe[Index].HeciId;
      HeciStateChange.HeciStateReq.State = gHeciHideInMe[Index].HeciState & 1;
      RspLen = sizeof (HeciStateChange.HeciStateRsp);
#if ME_TESTMENU_FLAG
      SetMeUefiFwHealthStatus (gHeciHideInMe[Index].HeciHealthStatusField, (HeciStateChange.HeciStateReq.State) ? StatusOk : StatusOther);
#endif // ME_TESTMENU_FLAG
      DEBUG ((DEBUG_INFO, "[SPS_SMM] Sending HECI_CHANGE_STATE to ME to %a HECI-%d\n",
        (HeciStateChange.HeciStateReq.State) ? "hide" : "disable", HeciStateChange.HeciStateReq.HeciId));

      Status = gSmmHeci->HeciSend (
                           gSmmHeci,
                           HECI_DEFAULT_PCH_ID,
                           &Timeout,
                           SPS_CLIENTID_BIOS,
                           SPS_CLIENTID_ME_MKHI,
                           (UINT32*)&(HeciStateChange.HeciStateReq),
                           sizeof (HeciStateChange.HeciStateReq)
                           );
      //
      // We don't expect response if it is HECI-1 hidden/disabled.
      // ME executes the operation before we can read response, so it fails.
      //
      if (Index == 0) {
        HeciStateChange.HeciStateRsp.Mkhi.Data = 0x00008DFF;
        HeciStateChange.HeciStateRsp.Response = 0;
      } else {
        if (!EFI_ERROR (Status)) {
          Status = gSmmHeci->HeciReceive (
                               gSmmHeci,
                               HECI_DEFAULT_PCH_ID,
                               &Timeout,
                               NULL,
                               NULL,
                               (UINT32*)&(HeciStateChange.HeciStateRsp),
                               &RspLen
                               );
        }
        // Reset HECI to clear NONCE data
        gSmmHeci->HeciReset (
                    gSmmHeci,
                    HECI_DEFAULT_PCH_ID,
                    NULL
                    );
      }
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Cannot send HECI_CHANGE_STATE (%r)\n", Status));
      } else if (HeciStateChange.HeciStateRsp.Mkhi.Data != 0x00008DFF) {
        DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Invalid HECI_CHANGE_STATE response (MKHI: 0x%X)\n",
          HeciStateChange.HeciStateRsp.Mkhi.Data));
      } else if (HeciStateChange.HeciStateRsp.Response != 0) {
        DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: HECI_CHANGE_STATE for HECI-%d failed (%d)\n",
          gHeciHideInMe[Index].HeciId, HeciStateChange.HeciStateRsp.Response));
      }
    }
  } // for (Index...)

  DEBUG ((DEBUG_INFO, "[SPS_SMM] SpsSmmExitBootServicesCallback exit\n"));
  return EFI_SUCCESS;
}


/**
  Function swaps bios region for the next boot.
  This should be called by bios update tool after new image flash to proper region.

  @return EFI_STATUS is returned.
*/
EFI_STATUS
BiosUpdate_RegionSelect (
  VOID
  )
{
  EFI_STATUS       Status;
  HECI_CONTROL     *pSmmHeci;

  if ( !MeTypeIsSps () ) {
    return EFI_UNSUPPORTED;
  }

  // if Me doesn't support DualBios there is nothing more to do here
  if (!mDualBiosEnabled){
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] RegionSelect is not functional if DualBios is not enabled in ME.\n"));
    return EFI_UNSUPPORTED;
  }

  // if RegionSelect message is disabled in setup there is nothing more to do here
  if (!mRegionSelectEnabled){
    DEBUG ((DEBUG_INFO, "[SPS_SMM] RegionSelect message is disabled in setup.\n"));
    return EFI_UNSUPPORTED;
  }

  if ( gNonceValue == 0){
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Nonce value unknown. Operation not supported.\n"));
    return EFI_DEVICE_ERROR;
  }

  // find HECI SMM driver
  pSmmHeci = NULL;

  Status = gSmst->SmmLocateProtocol (&gHeciControlSmmProtocolGuid, NULL, &pSmmHeci);
  if ( EFI_ERROR (Status) ){
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't locate SmmHeciProtocol.\n"));
    ASSERT_EFI_ERROR (Status);
    return EFI_DEVICE_ERROR;
  }

  //send HECI SWAP_REGION message
  Status = HeciReq_RegionSelect (pSmmHeci, gNonceValue);
  if (EFI_ERROR (Status)){
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Can't swap the region!\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "[SPS_SMM] Another region activated.\n"));
  return EFI_SUCCESS;
}


/**
  Function returns number of the current used bios region.

  @param[out]  Region       Buffer for the response - number of the current used region.
                            Proper region number can be equal only to 0 or 1.
  @return EFI_SUCCESS       Returned if success.
  @return EFI_DEVICE_ERROR  Smm Heci Protocol not found or can't recognize which bios region
                            is running and active.
*/
EFI_STATUS
BiosUpdate_GetActiveRegionNum (
  OUT UINT8               *Region
  )
{
  EFI_STATUS        Status;
  HECI_CONTROL      *pSmmHeci;

  // find HECI SMM protocol
  pSmmHeci = NULL;

  Status = gSmst->SmmLocateProtocol (&gHeciControlSmmProtocolGuid, NULL, &pSmmHeci);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] ERROR: Can't locate SmmHeciProtocol.\n"));
    ASSERT_EFI_ERROR (Status);
    return EFI_DEVICE_ERROR;
  }
  return GetActiveRegionNum (pSmmHeci, Region);
}


/**
  Function stops update if DualBios is supported at ME side in generally
  but currently ME is in recovery state.
  This should be called by bios update tool before flash new image.

  @return EFI_STATUS is returned.

 */
EFI_STATUS
BiosUpdate_UpdateAllowed (
  VOID
  )
{
  HECI_FWS_REGISTER MeFs1;

  if ( !MeTypeIsSps () ) {
    return EFI_UNSUPPORTED;
  }

  // if Me doesn't support DualBios - update is allowed
  if (!mDualBiosEnabled)
    return EFI_SUCCESS;

  // read MEFS
  MeFs1.ul = HeciPciRead32 (R_ME_HFS);

  /*
    Me supports DualBios so update is valid only when:
     - SPS firmware is running in ME (checked on function entry)
     - Init Complete is set to 1.
   */
  if (MeFs1.r.FwInitComplete == 1) {
      return EFI_SUCCESS;
  } else {
    // Me is not ready to handle update
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] MEFS1: 0x%08X\n", MeFs1.ul));
    DEBUG ((DEBUG_ERROR, "[SPS_SMM] Me is not in ready to handle bios update. Operation temporary not supported.\n"));

    return EFI_NOT_READY;
  }
}
