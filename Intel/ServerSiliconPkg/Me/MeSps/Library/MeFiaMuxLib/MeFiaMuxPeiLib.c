/** @file
  MeFiaMuxPeiLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <Library/PciLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeFiaMuxLib.h>
#include <Library/MeTypeLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchInfoLib.h>

#include <Sps.h>
#include <MeAccess.h>
#include <ConfigBlock/MeGetConfigBlock.h>
#include <Guid/SpsInfoHobGuid.h>
#include <Library/MeUtilsLib.h>
#include <Library/SpsPeiLib.h>
#include <Library/PchFiaLib.h>

/**
  Get the information whether the SPS supports FIA MUX.

  @retval TRUE   FIA MUX is supported by SPS
  @retval FALSE  FIA MUX isn't supported by SPS
**/
BOOLEAN
IsFiaMuxSupportedBySps (
  VOID
  )
{
  BOOLEAN             FiaMuxSupportedBySps = FALSE;
  EFI_HOB_GUID_TYPE   *GuidHob;
  SPS_INFO_HOB        *SpsInfo;

  GuidHob = GetFirstGuidHob (&gSpsInfoHobGuid);
  if (GuidHob == NULL) {
    FiaMuxSupportedBySps = SpsIsFiaMuxSupported ();
  } else {
    SpsInfo = GET_GUID_HOB_DATA (GuidHob);
    if (SpsInfo != NULL) {
      FiaMuxSupportedBySps = (BOOLEAN)SpsInfo->FeatureSet.Bits.FiaMuxConfiguration;
    }
  }

 return FiaMuxSupportedBySps;
}

/**
  Check if FIA MUX messaging is enabled.

  @retval TRUE   FIA MUX messages are enabled
  @retval FALSE  FIA MUX messages are disabled
**/
BOOLEAN
GetFiaMuxMessageEnabled (
  VOID
  )
{
  BOOLEAN          FiaMuxMessageEnabled = FALSE;
#if ME_SPS_SUPPORT
  SPS_PEI_CONFIG  *pSpsPolicy;

  pSpsPolicy = GetSpsPreMemPeiConfigBlock (NULL);
  if (pSpsPolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: FiaMux: Cannot get ME Policy PPI. Use module default.\n"));
  } else {
    FiaMuxMessageEnabled = (BOOLEAN) pSpsPolicy->FiaMuxMessageEnabled;
  }
#endif // ME_SPS_SUPPORT

  return FiaMuxMessageEnabled;
}

/**
  Check if FIA MUX is supported by ME.

  @retval TRUE         FiaMux messages are allowed
  @retval FALSE        FiaMux messages are not allowed
**/
BOOLEAN
MeFiaMuxAllowed (
  VOID
  )
{
  BOOLEAN                         FiaMuxAllowed = FALSE;
#if ME_SPS_SUPPORT
  BOOLEAN                         EndOfPostStatus;
  UINT32                          MeFs2;

  if (!MeTypeIsSps () || MeSpsInRecovery ()) {
    DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a () only supported in SPS Operational mode\n", __FUNCTION__));
    return FALSE;
  }

  if (!IsFiaMuxSupportedBySps ()) {
    DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a () FiaMux isn't supported\n", __FUNCTION__));
    return FALSE;
  }

  if (GetFiaMuxMessageEnabled () == FALSE) {
    DEBUG ((DEBUG_INFO, "[SPS] (ICC) %a () FiaMux messaging is disabled\n", __FUNCTION__));
    return FALSE;
  }

  EndOfPostStatus = GetSpsEndOfPostStatus (&MeFs2);
  if (EndOfPostStatus) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS] ERROR: (ICC) %a: called after END_OF_POST was sent (MEFS1: %08X, MEFS2: %08X).\n",
      __FUNCTION__,
      HeciPciRead32 (R_ME_HFS),
      MeFs2
      ));
    return FALSE;
  }

  FiaMuxAllowed = TRUE;
#endif // ME_SPS_SUPPORT

  return FiaMuxAllowed;
}
