/** @file
  Header file for the PeiIioSiPolicyUpdateLib.h Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#ifndef _PEI_IIO_SI_POLICY_UPDATE_LIB_H_
#define _PEI_IIO_SI_POLICY_UPDATE_LIB_H_

/**
  Update Iio Silicon init Policy


**/
VOID
UpdateIioSiPolicy (
  VOID
  );

/**
  This function prints the Iio Pei policy.
**/
VOID
PrintIioSiPolicy (
  VOID
  );

/**
  This function allocates Trace Hub Memory, and set allocated memory base into policy

  @param[in, out] TraceHubMemBase       Allocated Trace Hub memory base address

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_OUT_OF_RESOURCES          Allocated memory failure
  @retval EFI_ABORTED                   Failed to allocate memory. gIioSiPolicyHobGuid not found.
**/
EFI_STATUS
TraceHubMemAllocation (
  IN OUT UINT32               *TraceHubMemBase
);

/**
  Updates IIO config block policies from setup data.

  @param[in,out] IioPolicyPpi         The pointer to get IIO Policy PPI instance

  @retval EFI_SUCCESS          The function completed successfully
  @retval EFI_ERROR            Failed to update policies
**/
EFI_STATUS
EFIAPI
IioUpdateConfigBlockPolicy (
  IIO_POLICY_STRUCT *IioPolicyPpi
  );
#endif
