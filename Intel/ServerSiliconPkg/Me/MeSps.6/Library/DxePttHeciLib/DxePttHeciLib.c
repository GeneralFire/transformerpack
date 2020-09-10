/** @file
  Implements Platform Trust Technology (PTT) HECI SkuMgr Interface Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2019 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <ConfigBlock.h>
#include <Library/DxeMeLib.h>
#include <Library/PttHeciLib.h>
#include <Library/HobLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>

#if defined(ME_SPS_SUPPORT) && ME_SPS_SUPPORT
#include <Guid/SpsInfoHobGuid.h>
#endif //ME_SPS_SUPPORT

#define CLEAR_FEATURES_BITMASK 0x00000000
#define PTT_BITMASK            BIT29

/**
  Checks whether ME FW has the Platform Trust Technology capability.

  @param[out] PttCapability      TRUE if PTT is supported, FALSE othewrwise.

  @retval EFI_SUCCESS            Command succeeded
**/
EFI_STATUS
EFIAPI
PttHeciGetCapability (
  OUT BOOLEAN   *PttCapability
  )
{
#if defined(ME_SPS_SUPPORT) && ME_SPS_SUPPORT
  EFI_HOB_GUID_TYPE         *SpsHob;
  SPS_INFO_HOB              *SpsHobData;
#endif //ME_SPS_SUPPORT

  if (PttCapability == NULL) {
    ASSERT(PttCapability != NULL);
    return EFI_INVALID_PARAMETER;
  }

  *PttCapability = FALSE;

#if defined(ME_SPS_SUPPORT) && ME_SPS_SUPPORT
  if (MeTypeIsSps()) {
    SpsHob = GetFirstGuidHob(&gSpsInfoHobGuid);
    if (SpsHob != NULL) {
      SpsHobData = GET_GUID_HOB_DATA(SpsHob);
      *PttCapability = (BOOLEAN)SpsHobData->FeatureSet.Bits.PTT;
    }
  }
#endif //ME_SPS_SUPPORT

  DEBUG ((DEBUG_INFO, "[ME] PTT SkuMgr: PttCapability = %d\n", *PttCapability));

  return EFI_SUCCESS;
}


/**
  Checks Platform Trust Technology enablement state.

  @param[out] IsPttEnabledState  TRUE if PTT is enabled, FALSE othewrwise.

  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_UNSUPPORTED        ME doesn't support used message
  @retval EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
**/
EFI_STATUS
EFIAPI
PttHeciGetState (
  OUT BOOLEAN   *IsPttEnabledState
  )
{
#if ME_SUPPORT_FLAG
  EFI_STATUS                 Status;
  MEFWCAPS_SKU               CurrentFeatures;

  *IsPttEnabledState = FALSE;

  if (MeIsInRecovery ()) {
    return EFI_UNSUPPORTED;
  }

  Status = HeciGetFwFeatureStateMsg (&CurrentFeatures);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (CurrentFeatures.Fields.PTT) {
    *IsPttEnabledState = TRUE;
  }

  DEBUG((DEBUG_INFO, "[ME] PTT SkuMgr: PttState = %d\n", *IsPttEnabledState));

  return EFI_SUCCESS;
#else // ME_SUPPORT_FLAG
  return EFI_UNSUPPORTED;
#endif // ME_SUPPORT_FLAG
}

/**
  Changes current Platform Trust Technology state.

  @param[in] PttEnabledState     TRUE to enable, FALSE to disable.

  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
**/
EFI_STATUS
EFIAPI
PttHeciSetState (
  IN  BOOLEAN   PttEnabledState
  )
{
#if ME_SUPPORT_FLAG
  EFI_STATUS      Status;
  UINT32          EnableBitmap;
  UINT32          DisableBitmap;

  if (PttEnabledState) {
    //
    // Enable PTT
    //
    DEBUG ((DEBUG_INFO, "[ME] PTT SkuMgr: Enable PTT\n"));
    EnableBitmap  = PTT_BITMASK;
    DisableBitmap = CLEAR_FEATURES_BITMASK;
  } else {
    //
    // Disable PTT
    //
    DEBUG ((DEBUG_INFO, "[ME] PTT SkuMgr: Disable PTT\n"));
    EnableBitmap  = CLEAR_FEATURES_BITMASK;
    DisableBitmap = PTT_BITMASK;
  }
  Status = HeciFwFeatureStateOverride (EnableBitmap, DisableBitmap);

  return Status;
#else // ME_SUPPORT_FLAG
  return EFI_DEVICE_ERROR;
#endif // ME_SUPPORT_FLAG
}

/**
  Updates Platform Trust Technology state to new state.

  @param[in] NewPttState         TRUE to enable, FALSE to disable.

  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
**/
EFI_STATUS
EFIAPI
PttHeciUpdateState (
  IN  BOOLEAN   NewPttState
  )
{
#if ME_SUPPORT_FLAG
  EFI_STATUS  Status;
  BOOLEAN     PttCurrentState;

  DEBUG((EFI_D_INFO, "[ME] PttHeciUpdateState(%d)\n", NewPttState));
  Status = PttHeciGetState(&PttCurrentState);
  if (!EFI_ERROR(Status)) {
    DEBUG((EFI_D_INFO, "[ME] Current Ptt State: %d\n", PttCurrentState));
    if (PttCurrentState != NewPttState) {
      DEBUG((EFI_D_INFO, "[ME] Run PttHeciSetState(%d)\n", NewPttState));
      Status = PttHeciSetState(NewPttState);
    }
  }

  return Status;
#else // ME_SUPPORT_FLAG
  return EFI_DEVICE_ERROR;
#endif // ME_SUPPORT_FLAG
}
