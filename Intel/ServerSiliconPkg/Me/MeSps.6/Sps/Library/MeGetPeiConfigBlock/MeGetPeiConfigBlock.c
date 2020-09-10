/** @file
  This is a library for ME ConfigBlock functionality.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <PchLimits.h>
#include <ConfigBlock/MeGetConfigBlock.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>

/**
  @brief
  Get ME PEI Config Block

  @param[in] SiPolicy      The Silicon Policy PPI instance

  @retval ME_PEI_CONFIG *
**/
ME_PEI_CONFIG *
GetMePeiConfigBlock (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  EFI_STATUS            Status;
  ME_PEI_CONFIG         *MeConfig = NULL;

  if (SiPolicy == NULL) {
    Status = PeiServicesLocatePpi (
               &gSiPolicyPpiGuid,
               0,
               NULL,
               (VOID **)&SiPolicy
               );
    ASSERT_EFI_ERROR (Status);
  }

  if (SiPolicy) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gMePeiConfigGuid, (VOID *) &MeConfig);
    ASSERT_EFI_ERROR (Status);
  }

  return MeConfig;
} // GetMePeiConfigBlock


/**
  @brief
  Get AMT PEI Config Block

  @param[in] SiPolicy      The Silicon Policy PPI instance

  @retval AMT_PEI_CONFIG *
**/
AMT_PEI_CONFIG *
GetAmtPeiConfigBlock (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
#ifdef AMT_ENABLE
  EFI_STATUS            Status;
  AMT_PEI_CONFIG        *AmtConfig = NULL;

  if (SiPolicy == NULL) {
    Status = PeiServicesLocatePpi (
               &gSiPolicyPpiGuid,
               0,
               NULL,
               (VOID **)&SiPolicy
               );
    ASSERT_EFI_ERROR (Status);
  }

  if (SiPolicy) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gAmtPeiConfigGuid, (VOID *) &AmtConfig);
    if (MeClientIsCorporate ()) {
      ASSERT_EFI_ERROR (Status);
    }
  }

  return AmtConfig;
#else // AMT_ENABLE
  return NULL;
#endif // AMT_ENABLE
} // GetAmtPeiConfigBlock

/**
  @brief
  Get SPS PEI Config Block

  @param[in] SiPolicy      The Silicon Policy PPI instance

  @retval SPS_PEI_CONFIG *
**/
SPS_PEI_CONFIG *
GetSpsPreMemPeiConfigBlock (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicy
  )
{
  EFI_STATUS            Status;
  SPS_PEI_CONFIG        *SpsConfig = NULL;

  if (SiPreMemPolicy == NULL) {
    Status = PeiServicesLocatePpi (
               &gSiPreMemPolicyPpiGuid,
               0,
               NULL,
               (VOID **)&SiPreMemPolicy
               );
    ASSERT_EFI_ERROR (Status);
  }

  if (SiPreMemPolicy) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSpsPeiConfigGuid, (VOID *) &SpsConfig);
    if (MeTypeIsSps ()) {
      ASSERT_EFI_ERROR (Status);
    }
  }

  return SpsConfig;
} // GetSpsPreMemPeiConfigBlock

/**
  @brief
  Get ME PreMem Config Block

  @param[in] SiPolicy      The Silicon PreMem Policy PPI instance

  @retval ME_PEI_PREMEM_CONFIG *
**/
ME_PEI_PREMEM_CONFIG *
GetMePreMemConfigBlock (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicy
  )
{
  EFI_STATUS                   Status;
  ME_PEI_PREMEM_CONFIG         *MePreMemConfig = NULL;

  if (SiPreMemPolicy == NULL) {
    Status = PeiServicesLocatePpi (
               &gSiPreMemPolicyPpiGuid,
               0,
               NULL,
               (VOID **)&SiPreMemPolicy
               );
    ASSERT_EFI_ERROR (Status);
  }

  if (SiPreMemPolicy) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gMePeiPreMemConfigGuid, (VOID *) &MePreMemConfig);
    ASSERT_EFI_ERROR (Status);
  }

  return MePreMemConfig;
} // GetMePreMemConfigBlock
