/** @file
  MeTypePeiLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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
#include <PiPei.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/PeiServicesLib.h>
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <BupMsgs.h>
#include <MeTypeHob.h>

#include <Library/HobLib.h>
#include <Register/HeciRegs.h>
#include "MeAccess.h"
#include <Library/PmcLib.h>
#include <MeState.h>
#include <Library/MeTypeGetLib.h>
#include <Library/MeTypeLib.h>


/**
  Function returns ME Type read from HOB /if it is not present/
  If HOB is not present it tries to create it.
  Debugs are restricted to save some space in build targets : RestrictedContent

  @retval ME_TYPE_UNDEF            Can't determine ME Type
  @retval ME_TYPE_SPS              Detected SPS
  @retval ME_TYPE_CLIENT           Detected Client ME
  @retval ME_TYPE_DFX              Detected DFX ME
  @retval ME_TYPE_DISABLED         Disable all ME flows
**/
ON_BOARD_ME_TYPE
GetMeTypeFromHob (
  VOID
  )
{
  ON_BOARD_ME_TYPE    RetVal = ME_TYPE_UNDEF;
#ifdef ME_SUPPORT_FLAG
  ME_FW_TYPE_HOB      *MeTypeHob;
  EFI_STATUS          Status = EFI_NOT_FOUND;
  HECI_FWS_REGISTER   MeHfs;
  UINT8               FwType;
  UINT8               FwSubType;

  FwType    = 0;
  FwSubType = 0;
  MeHfs.ul = HeciPciRead32 (R_ME_HFS);

  MeTypeHob = GetFirstGuidHob (&gMeTypeHobGuid);
  if (MeTypeHob == NULL) {
    if (PmcIsDwrBootMode () == TRUE) {
      RetVal = METYPE_DISABLED;
      FwType = METYPE_DISABLED;
    } else if (MeHfs.r.CurrentState == ME_STATE_DISABLED) {
      RetVal = ME_TYPE_DISABLED;
      FwType = METYPE_DISABLED;
    } else if (MeTypeIsSpsIgnition ()) {
      RetVal = ME_TYPE_DISABLED;
      FwType = METYPE_DISABLED;
    } else {
      RetVal = PeiHeciGetMeType (&FwType, &FwSubType);
    }
    if (RetVal != ME_TYPE_UNDEF) {
      Status = PeiServicesCreateHob (
                 EFI_HOB_TYPE_GUID_EXTENSION,
                 sizeof (ME_FW_TYPE_HOB),
                 &MeTypeHob
                 );
      if (!EFI_ERROR (Status)) {
        MeTypeHob->EfiHobGuidType.Name = gMeTypeHobGuid;
        MeTypeHob->MeType = RetVal;
        MeTypeHob->FwType = FwType;
        MeTypeHob->FwSubType = FwSubType;
        MeTypeShowDebug (L"[ME]");
      }
      RetVal = ME_TYPE_UNDEF;
    }
    MeTypeHob = GetFirstGuidHob (&gMeTypeHobGuid);
  } else {
    if ((MeHfs.r.CurrentState == ME_STATE_DISABLED) &&
        (MeTypeHob->MeType != ME_TYPE_DISABLED) &&
        (MeTypeHob->MeType != ME_TYPE_CLIENT)) {
      MeTypeHob->MeType = ME_TYPE_DISABLED;
    }
  }

  if (MeTypeHob != NULL) {
    RetVal = MeTypeHob->MeType;
  }

  DEBUG ((DEBUG_INFO, "HECI: %a returns %d\n", __FUNCTION__, RetVal));
#endif // ME_SUPPORT_FLAG

  return RetVal;
}
