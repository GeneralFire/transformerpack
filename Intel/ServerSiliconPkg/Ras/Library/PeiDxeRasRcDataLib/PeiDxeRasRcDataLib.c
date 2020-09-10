/** @file
  PEI/DXE common RAS RC data functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#include <PiPei.h>
#include <Library/RasRcDataLib.h> // *this* library class
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include "PeiDxeRasRcDataLibInternal.h"

/**
  Get RAS RC policy.

  @return   A pointer to the RAS RC policy data, or NULL if not found.
**/
RAS_RC_POLICY *
EFIAPI
GetRasRcPolicy (
  VOID
  )
{
  return (RAS_RC_POLICY *) GetRasRcHobInternal (&gRasRcPolicyHobGuid);
} // GetRasRcPolicy

/**
  Get RAS RC configuration.

  @return   A pointer to the RAS RC config data, or NULL if not found.
**/
RAS_RC_CONFIG *
EFIAPI
GetRasRcConfig (
  VOID
  )
{
  return (RAS_RC_CONFIG *) GetRasRcHobInternal (&gRasRcConfigHobGuid);
} // GetRasRcConfig

/**
  Get RAS RC HOB.

  Helper function for getting one of our GUIDed HOBs.

  @param[in]  HobGuid         The GUID of the HOB to get.

  @return   A pointer to the GUID HOB's data, or NULL if not found.
**/
VOID *
GetRasRcHobInternal (
  IN CONST EFI_GUID *HobGuid
  )
{
  VOID  *GuidHob;
  VOID  *HobData;

  ASSERT (HobGuid != NULL);
  if (HobGuid == NULL) {
    return NULL;
  }

  GuidHob = GetFirstGuidHob (HobGuid);
  if (GuidHob == NULL) {
    HobData = NULL;
  } else {
    HobData = GET_GUID_HOB_DATA (GuidHob);
  }

  return HobData;
} // GetRasRcHobInternal
