/** @file
  Implementation of poison & viral function for processor RAS lib.

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
  Check if CPU support Software Error Recovery


**/
BOOLEAN
EFIAPI
IsErrorRecoveryCap (
  VOID
  )
{
  MSR_IA32_MCG_CAP_REGISTER McgCapReg;

  McgCapReg.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_CAP);

  return (BOOLEAN) (McgCapReg.Bits.MCG_SER_P);
}

/**
  Set poison and viral function per core.

  @param[in]  Flag        Specify the flag poison and viral feature.

**/
VOID
EFIAPI
ApSetPoisonViral (
  IN POISON_VIRAL_FLAG  *Flag
  )
{
  MSR_MCG_CONTAIN_REGISTER McgContainReg;

  McgContainReg.Uint32 = AsmReadMsr32 (MSR_MCG_CONTAIN);

  McgContainReg.Bits.Pe          = (*Flag & 0x1);
  McgContainReg.Bits.EnableViral = ((*Flag >> 1)  & 0x1);

  AsmWriteMsr32 (MSR_MCG_CONTAIN, McgContainReg.Uint32);

  RAS_DEBUG((MP_PRINT | LEVEL_REG, "[CpuRas]Set Poison/Viral MSR(%x) = %lx\n", MSR_MCG_CONTAIN, McgContainReg.Uint32));
}

/**
  Sets poison and viral feature for the whole system.

  This function supports three option specified by the input flag parameter:
  1. DisablePoisonViral - Disable viral and poison
  2. EnablePoisonOnly   - Enable poison only
  3. EnablePoisonViral  - Enable both poison and viral
  The function will ASSERT() if the flag is not one of the above options.

  @param[in] Flag  Enumerate type to specify poison and viral setting.

**/
VOID
EFIAPI
SetViralPoison (
  IN POISON_VIRAL_FLAG           Flag
  )
{
  ASSERT((Flag == DisablePoisonViral) || (Flag == EnablePoisonOnly) || (Flag == EnablePoisonViral));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "[CpuRas]SetViralPoison: %a\n", (Flag == DisablePoisonViral) ? "Disable" : ((Flag == EnablePoisonOnly) ? "Poison" : "Viral")));
  if (!IsErrorRecoveryCap ()) {
    return;
  }

  MultiProcessorExecute (OneApPerCore, ALL_PACKAGE, ALL_CORE, FALSE, ApSetPoisonViral, &Flag);
}

/**

  Enable 256b granualirty for POISON detection check in the DCU.

  @param None.

  @retval None
**/
VOID
EFIAPI
ApSetDcuMode (
  IN VOID *Buffer
  )
{
  SKX_MSR_DCU_MODE_CONTROL_REGISTER    DcuMode;

  DcuMode.Uint32 = AsmReadMsr32(SKX_MSR_DCU_MODE_CONTROL);
  DcuMode.Bits.Dcu256bReportEn = 1;
  AsmWriteMsr32 (SKX_MSR_DCU_MODE_CONTROL, DcuMode.Uint32);

  RAS_DEBUG((MP_PRINT | LEVEL_REG, "[CpuRas]Set DCU MODE MSR(%x) = %lx\n", SKX_MSR_DCU_MODE_CONTROL, DcuMode.Uint32));
}

/**

  Set the value in SKX_MSR_DCU_MODE_CONTROL MSR per core.

  @param None.

  @retval None
**/
VOID
EFIAPI
SetDCUPoisonDetectionMode (
  IN VOID
  )
{
  //
  //  check whether the Processor capability supports
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "SetDCUPoisonDetectionMode\n"));
  if (!IsErrorRecoveryCap ()) {
    RAS_DEBUG((LEVEL_FUNC_FLOW, "[CpuRas] CPU does NOT support Software Error Recovery\n"));
    return;
  }
  MultiProcessorExecute (OneApPerCore, ALL_PACKAGE, ALL_CORE, FALSE, ApSetDcuMode, NULL);    
}

