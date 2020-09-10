/** @file
  Implementation of Cloak function for processor RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2019 Intel Corporation. <BR>

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
  Is Lmce supported by the firmware

  @return True if Lmce is enabled by firmware

**/
BOOLEAN
EFIAPI
IsLmceFirmwareEnabled (
  VOID
  )
{
  MSR_IA32_FEATURE_CONTROL_REGISTER FeatureControlReg;

  FeatureControlReg.Uint64 = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);

  return (BOOLEAN) (FeatureControlReg.Bits.LmceOn);
}

/**
  Is Lmce supported by the processor

  @return True if Lmce is supported by the processor

**/
BOOLEAN
EFIAPI
IsLmceProcessorSupported (
  VOID
  )
{
  MSR_IA32_MCG_CAP_REGISTER McgCapReg;

  McgCapReg.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_CAP);

  return (BOOLEAN) (McgCapReg.Bits.MCG_LMCE_P);
}

/**
  Has OS enabled Lmce in the mcg_ext_ctl

  @return Whether OS has enabled lmce in the mcg_ext_ctl

**/
BOOLEAN
EFIAPI
IsLmceOsEnabled (
  VOID
  )
{
  MSR_IA32_MCG_EXT_CTL_REGISTER McgExtCtlReg;

  McgExtCtlReg.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_EXT_CTL);

  return (BOOLEAN) (IsLmceProcessorSupported () && IsLmceFirmwareEnabled () && (McgExtCtlReg.Bits.LMCE_EN != 0));
}

VOID
EFIAPI
ApEnableLmce (
  IN OUT  VOID *Context
  )
{
  MSR_IA32_FEATURE_CONTROL_REGISTER       IA32FeatureControl;

  if (IsLmceProcessorSupported ()) {
    IA32FeatureControl.Uint64 = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);
    if ((IA32FeatureControl.Bits.Lock) == 0) {
      IA32FeatureControl.Bits.LmceOn = 1;
      IA32FeatureControl.Uint64 = AsmWriteMsr64 (MSR_IA32_FEATURE_CONTROL, IA32FeatureControl.Uint64);
      RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Enable LMCE MSR(0x%x) = 0x%lx\n", MSR_IA32_FEATURE_CONTROL, IA32FeatureControl.Uint64));
    }
  }
}

VOID
EFIAPI
EnableLmce (
  VOID
  )
{
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]Enable LMCE for all threads.\n"));
  MultiProcessorExecute (AllAps, ALL_PACKAGE, ALL_CORE, FALSE, ApEnableLmce, NULL);
}

BOOLEAN
EFIAPI
IsBmce (
  VOID
  )
{
  UINT8                               Socket;
  MCA_ERR_SRC_LOG_PCU_FUN2_STRUCT     ErrSrcLog;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }
    ErrSrcLog.Data = ReadCpuCsr (Socket, 0, MCA_ERR_SRC_LOG_PCU_FUN2_REG);
    if (ErrSrcLog.Bits.msmi_internal == 1 || ErrSrcLog.Bits.msmi == 1) {
      RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Error source CSR(S 0x%x I0 R 0x%x) = 0x%x\n", Socket, MCA_ERR_SRC_LOG_PCU_FUN2_REG, ErrSrcLog.Data));
      RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Error happens, msmi_internal = 0x%x msmi = 0x%x\n", ErrSrcLog.Bits.msmi_internal, ErrSrcLog.Bits.msmi));
      return TRUE;
    }
  }

  return FALSE;
}

BOOLEAN
IsLmceSignaled (
  VOID
  )
{
  MSR_IA32_MCG_STATUS_REGISTER McgStatusReg;

  McgStatusReg.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_STATUS);

  return (BOOLEAN) (McgStatusReg.Bits.LMCE_S);
}

/**
  Return if Local machine check exception signaled. Indicates (when set) that a local machine check
exception was generated. This indicates that the current machine-check event was delivered to only the
logical processor.

  @param[out]   Lmce count

**/
VOID
EFIAPI
ApIsLmce (
  IN OUT UINT32  *LmceCount
  )
{
  if (IsLmceSignaled ()) {
    InterlockedIncrement (LmceCount);
    RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Lmce is signaled, current Lmce count is 0x%x\n", *LmceCount));
  }
}

BOOLEAN
EFIAPI
IsLmce (
  VOID
  )
{
  UINT32  LmceCount;
  BOOLEAN Lmce;

  Lmce = FALSE;
  if (IsLmceOsEnabled ()) {
    LmceCount = 0;
    MultiProcessorExecute (AllAps, ALL_PACKAGE, ALL_CORE, FALSE, ApIsLmce, &LmceCount);
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]IsLmce: LMCE count is 0x%x\n", LmceCount));
    Lmce = (BOOLEAN) (LmceCount > 0);
  } else {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]IsLmce: OS does not enable LMCE, Processor supported 0x%x Firmware enabled 0x%x\n",
                IsLmceProcessorSupported (), IsLmceFirmwareEnabled ()));
  }
  return Lmce;
}

VOID
EFIAPI
ApClearLmceStatus (
  IN OUT VOID    *Context
  )
{
  MSR_IA32_MCG_STATUS_REGISTER McgStatusReg;

  McgStatusReg.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_STATUS);

  McgStatusReg.Bits.LMCE_S = 0;
 
  AsmWriteMsr64 (MSR_IA32_MCG_STATUS, McgStatusReg.Uint64);
}

/**
  Clear LMCE status for all CPU thread

**/
VOID
EFIAPI
ClearLmceStatus (
  VOID
  )
{
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]ClearLmceStatus\n"));
  MultiProcessorExecute (AllAps, ALL_PACKAGE, ALL_CORE, FALSE, ApClearLmceStatus, NULL);
}

