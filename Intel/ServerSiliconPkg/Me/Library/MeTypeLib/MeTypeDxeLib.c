/** @file
  MeTypeDxeLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <PiDxe.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include "MeAccess.h"
#include "Register/HeciRegs.h"
#include "Library/MeTypeLib.h"
#include "MeTypeHob.h"
#include <Library/HobLib.h>
#include <Library/PmcLib.h>

/**
  Function returns ME Type read from HOB /if it is not present/
  If HOB is not present it tries to create it.

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
#if ME_SUPPORT_FLAG
  ME_FW_TYPE_HOB      *MeTypeHob;
  HECI_FWS_REGISTER   MeHfs;

  MeTypeHob = GetFirstGuidHob (&gMeTypeHobGuid);
  if (MeTypeHob != NULL) {
    MeHfs.ul = HeciPciRead32 (R_ME_HFS);
    if ((MeTypeHob->MeType != ME_TYPE_DISABLED) &&
        (MeTypeHob->MeType != ME_TYPE_CLIENT) &&
        (MeHfs.r.CurrentState == ME_STATE_DISABLED)) {
      DEBUG ((DEBUG_ERROR, "ERROR: HECI: GetMeTypeFromHob() Disabled ME Type detected, in PEI METype was reported as : %d\n",
        MeTypeHob->MeType));
      MeTypeHob->MeType = ME_TYPE_DISABLED;
      ASSERT (FALSE);
    }
    RetVal = MeTypeHob->MeType;
  }
#endif // ME_SUPPORT_FLAG
  DEBUG ((DEBUG_INFO, "HECI: GetMeTypeFromHob() returns %d\n", RetVal));

  return RetVal;
}

