/** @file
  Implementation of Cloak function for processor RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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
#include <Library/CpuAndRevisionLib.h>

STATIC  BOOLEAN    mIs14nmCpu;

/**
  Enables Cloaking per socket, by disabling the CMCI in MSR 17F and by enabling bits 0&1 in MSR 52
  so that read to MC status registers will return 0 for UCNA & corrected Errors

  @param[in]  Context        Not used

**/
VOID
EFIAPI
ApEnableCloaking (
  IN OUT  VOID *Context
  )
{
  MSR_CLOAKING_REGISTER            McaErrorControlReg;
  ICX_MSR_ERROR_CONTROL_REGISTER   IcxErrorControlReg;
  ICX_MSR_ERROR_CONTROL_REGISTER   IcxErrorControlRegBuffer;

  McaErrorControlReg.Uint64 = AsmReadMsr64 (MSR_CLOAKING);

  McaErrorControlReg.Bits.CerrRdStatusInSmmOnly = 1;
  if (mIs14nmCpu == TRUE) {
    McaErrorControlReg.Bits.UcnaRdStatusInSmmOnly = 1;
  } else {
    McaErrorControlReg.Bits.UcnaRdStatusInSmmOnly = 0;
  }

  AsmWriteMsr64 (MSR_CLOAKING, McaErrorControlReg.Uint64);
  RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Enable Cloaking: MSR(%x) = %lx\n", MSR_CLOAKING, McaErrorControlReg.Uint64));

  IcxErrorControlRegBuffer.Uint64 = AsmReadMsr64 (ICX_MSR_ERROR_CONTROL);
  //
  // Don't write 1 to reserved bits in MSR_ERROR_CONTROL.
  //
  IcxErrorControlReg.Uint64 = 0;
  IcxErrorControlReg.Bits.LlcEwbSraoPromote = IcxErrorControlRegBuffer.Bits.LlcEwbSraoPromote;
  if (mIs14nmCpu == TRUE) {
    IcxErrorControlReg.Bits.CmciDisable = 1;
    //
    // Set Mem_Correrr_logging_Disable
    //
    IcxErrorControlReg.Uint32 |= BIT5;
  } else {
    IcxErrorControlReg.Bits.CmciDisable = 0;
  }

  AsmWriteMsr64 (ICX_MSR_ERROR_CONTROL, IcxErrorControlReg.Uint64);
  RAS_DEBUG ((MP_PRINT | LEVEL_REG, "[CpuRas]Enable Cloaking: MSR(%x) = %lx\n", ICX_MSR_ERROR_CONTROL, IcxErrorControlReg.Uint64));
}

/**
  Enables Cloaking Bits for all the sockets using SmmStartupThisAp()

**/
VOID
EFIAPI
EnableCloaking (
  VOID
  )
{
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "[CpuRas]EnableCloaking\n"));

  mIs14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, FALSE, ApEnableCloaking, NULL);
}

