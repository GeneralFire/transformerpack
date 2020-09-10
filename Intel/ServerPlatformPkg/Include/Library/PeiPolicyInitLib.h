/** @file
  Header file for the PolicyInitPei Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2017 Intel Corporation. <BR>

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

#ifndef _POLICY_INIT_PEI_LIB_H_
#define _POLICY_INIT_PEI_LIB_H_

#include <Guid/SetupVariable.h>

/**
  Initilize Intel PEI Platform Policy

  @param[in]  PeiServices          General purpose services available to every PEIM.
**/
VOID
EFIAPI
PeiSiPolicyInitPreMem (
  IN CONST EFI_PEI_SERVICES    **PeiServices
  );

/**
  Initilize Intel PEI Platform Policy

  @param[in]  PeiServices          General purpose services available to every PEIM.
**/
VOID
EFIAPI
PeiSiPolicyInit (
  IN CONST EFI_PEI_SERVICES    **PeiServices
  );

/**
  Initilize Intel PEI Platform Policy

  @param[in]  FirmwareConfiguration It uses to skip specific policy init that depends
                                    on the 'FirmwareConfiguration' varaible.

  @retval     EFI_OUT_OF_RESOURCES  Unable to allocate necessary data structures.
**/
VOID
EFIAPI
PeiPolicyInitPreMem (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN UINT8                     FirmwareConfiguration
  );

/**
  Initilize Intel PEI Platform Policy

  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_OUT_OF_RESOURCES Unable to allocate necessary data structures.
**/
VOID
EFIAPI
PeiPolicyInit (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN UINT8                     FirmwareConfiguration
  );
#endif
