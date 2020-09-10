/** @file
  Implementation of MP related function for processor RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#include "ProcessorRasLibInternal.h"


/**
  Returns whether machine check is in progress.

  @retval BOOLEAN - True/False

**/
BOOLEAN
EFIAPI
IsMcip (
  VOID
  )
{
  IA32_MCG_STATUS_MSR_STRUCT McgStatus;

  McgStatus.Data = AsmReadMsr32 (MSR_IA32_MCG_STATUS);

  RAS_DEBUG ((MCE_PRINT | LEVEL_REG, "[CpuRas]IsMcip: MSR(%x) = %x\n", MSR_IA32_MCG_STATUS, McgStatus.Data));

  return (McgStatus.Bits.mcip == 1);
}

/**
  Returns whether IP is related to error.

  @retval BOOLEAN - True/False

**/
BOOLEAN
EFIAPI
IsEipv (
  VOID
  )
{
  IA32_MCG_STATUS_MSR_STRUCT McgStatus;

  McgStatus.Data = AsmReadMsr32 (MSR_IA32_MCG_STATUS);

  RAS_DEBUG ((MCE_PRINT | LEVEL_REG, "[CpuRas]IsEipv: MSR(%x) = %x\n", MSR_IA32_MCG_STATUS, McgStatus.Data));

  return (McgStatus.Bits.eipv == 1);
}


/**
  Returns whether restart IP is valid.

  @retval BOOLEAN - True/False

**/
BOOLEAN
EFIAPI
IsRipv (
  VOID
  )
{
  IA32_MCG_STATUS_MSR_STRUCT McgStatus;

  McgStatus.Data = AsmReadMsr32 (MSR_IA32_MCG_STATUS);

  RAS_DEBUG ((MCE_PRINT | LEVEL_REG, "[CpuRas]IsRipv: MSR(%x) = %x\n", MSR_IA32_MCG_STATUS, McgStatus.Data));

  return (McgStatus.Bits.ripv == 1);
}

/**
  Clear Machine Check in progress bit.

**/
VOID
EFIAPI
ClearMcip (
  VOID
  )
{
  IA32_MCG_STATUS_MSR_STRUCT McgStatus;
  McgStatus.Data = AsmReadMsr32 (MSR_IA32_MCG_STATUS);
  McgStatus.Bits.mcip = 0;
  AsmWriteMsr32 (MSR_IA32_MCG_STATUS, McgStatus.Data);
  RAS_DEBUG ((MCE_PRINT | LEVEL_REG, "[CpuRas]ClearMcip: MSR(%x) = %x\n", MSR_IA32_MCG_STATUS, McgStatus.Data));
}
