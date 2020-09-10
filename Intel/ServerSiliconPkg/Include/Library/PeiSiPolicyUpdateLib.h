/** @file
  Header file for PEI SiPolicyUpdate Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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

#ifndef _PEI_SI_POLICY_UPDATE_LIB_H_
#define _PEI_SI_POLICY_UPDATE_LIB_H_

#include <Ppi/SiPolicy.h>


/**
  This function performs Silicon PEI Policy initialization.

  @retval EFI_SUCCESS      The function completed successfully
**/
EFI_STATUS
EFIAPI
UpdatePeiSiPolicy (
  IN OUT  SI_POLICY_PPI         *SiPolicy
  );
/**
  This function performs PCH PEI Policy initialization.

  @param[in, out] SiPolicy        The SI Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver

**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicy (
  IN OUT  SI_POLICY_PPI         *SiPolicy
  );

/**
  This function performs PCH PEI Policy initialization.

  @param[in, out] SiPreMemPolicy  The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS             The PPI is installed and initialized.
  @retval EFI ERRORS              The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyPreMem (
  IN OUT  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy
  );

/**
  Update the ME Policy Library

  @param[in, out] SiPolicy       The SI Policy PPI instance

  @retval EFI_SUCCESS            Update complete.
**/
EFI_STATUS
UpdatePeiMePolicy (
  IN OUT  SI_POLICY_PPI         *SiPolicy
  );

/**
  Update the ME Policy Library

  @param[in, out] SiPreMemPolicy The SI PreMem Policy PPI instance

  @retval EFI_SUCCESS            Update complete.
**/
EFI_STATUS
UpdatePeiMePolicyPreMem (
  IN OUT  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy
  );

/**
  Install the Active Management Policy Ppi Library

  @param[in, out] SiPolicy      The SI Policy PPI instance

  @retval EFI_SUCCESS           Initialization complete.
  @retval EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
UpdatePeiAmtPolicy (
  IN OUT  SI_POLICY_PPI         *SiPolicy
  );

/**
  Update the SPS Policy Ppi (pre mem)

  @param[in, out] SiPreMemPolicyPpi   PEI Pre Meme Si Policy

  @retval EFI_SUCCESS           Initialization complete.
  @retval EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
  @retval EFI_INVALID_PARAMETER Wrong pointer passed to the function
**/
EFI_STATUS
UpdatePeiSpsPreMemPolicy (
  IN OUT  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy
  );

/**
  Update the SPS Policy Ppi (post mem)

  @param[in, out] SiPolicyPpi   PEI Si Policy

  @retval EFI_SUCCESS           Initialization complete.
  @retval EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval EFI_DEVICE_ERROR      Device error, driver exits abnormally.
  @retval EFI_INVALID_PARAMETER Wrong pointer passed to the function
**/
 EFI_STATUS
UpdatePeiSpsPolicy (
  IN OUT  SI_POLICY_PPI *SiPolicyPpi
  );
#endif
