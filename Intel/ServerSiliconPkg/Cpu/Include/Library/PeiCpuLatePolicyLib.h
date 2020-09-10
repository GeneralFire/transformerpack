/** @file
  Public API header file for the PeiCpuLatePolicyLib library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef __PEI_CPU_LATE_POLICY_LIB_H__
#define __PEI_CPU_LATE_POLICY_LIB_H__

#include <Ppi/CpuLatePolicyPpi.h>

/**
  CpuCreateLatePolicyDefaults creates default settings of CPU Late Policy.

  @param[in, out] CpuLatePolicyPpi  The pointer to CPU Late Policy PPI instance.

  @retval EFI_SUCCESS               The policy defaults are created.

**/
EFI_STATUS
EFIAPI
CpuCreateLatePolicyDefaults (
  IN OUT CPU_LATE_POLICY_PPI    *CpuLatePolicyPpi
  );

/**
  CpuInstallLatePolicyPpi installs CPU Late Policy PPI.
  While installed, RC assumes the Policy is ready and finalized.
  So please update and override any setting before calling this function.

  @param[in] CpuLatePolicyPpi       The pointer to CPU Late Policy PPI instance.

  @retval EFI_SUCCESS               The policy PPI is installed.
  @retval EFI_OUT_OF_RESOURCES      Insufficient resources to create buffer.

**/
EFI_STATUS
EFIAPI
CpuInstallLatePolicyPpi (
  IN CPU_LATE_POLICY_PPI    *CpuLatePolicyPpi
  );

#endif
