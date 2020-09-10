/** @file
  CpuBreakpoint function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2017 Intel Corporation. <BR>

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

/**
  Microsoft Visual Studio 7.1 Function Prototypes for I/O Intrinsics.
**/
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Cpu/CpuCoreRegs.h> 
#include "BaseLibInternals.h"

void __writemsr (unsigned long Register, unsigned __int64 Value);

#pragma intrinsic(__writemsr)

/**
  Write data to MSR.

  @param  Index                The register index of MSR.
  @param  Value                Data wants to be written.

  @return Value written to MSR.

**/
UINT64
EFIAPI
AsmWriteMsr64Org (
  IN UINT32  Index,
  IN UINT64  Value
  )
{
  __writemsr (Index, Value);
  return Value;
}

/**
  Write data to MSR.

  @param  Index                The register index of MSR.
  @param  Value                Data wants to be written.

  @return Value written to MSR.

**/
UINT64
EFIAPI
AsmWriteMsr64 (
  IN UINT32  Index,
  IN UINT64  Value
  )
{
  UINT64     ApicBaseReg;
  UINT64     Mask = 0;
  UINT32     LabelNumber = 0;
  
  ApicBaseReg = AsmReadMsr64 (EFI_MSR_IA32_APIC_BASE);
  
  if ((ApicBaseReg & B_EFI_MSR_IA32_APIC_BASE_BSP) != 0 && MsrTraceEnabled ()) {
    if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
      DEBUG ((EFI_D_ERROR, "\n;MSR_ACCESS_BEGIN\n"));
      Mask = MsrGuardMask (Index);
      if(Mask) {
          DEBUG ((EFI_D_ERROR, "mov ecx, 0%08xh\n", Index));
          DEBUG ((EFI_D_ERROR, "rdmsr\n"));
          if((UINT32) (Mask >> 32)) {
            DEBUG ((EFI_D_ERROR, "and edx, 0%08xh\n", (UINT32) (Mask >> 32)));
            DEBUG ((EFI_D_ERROR, "jnz LABEL_MSR_SKIP_WRMSR_%d\n", LabelNumber)); 
          }
          DEBUG ((EFI_D_ERROR, "and eax, 0%08xh\n", (UINT32) Mask));
          DEBUG ((EFI_D_ERROR, "jnz LABEL_MSR_SKIP_WRMSR_%d\n", LabelNumber));
          DEBUG ((EFI_D_ERROR, "mov edx, 0%08xh\n", (UINT32) (Value >> 32)));
          DEBUG ((EFI_D_ERROR, "mov eax, 0%08xh\n", (UINT32) Value));
          DEBUG ((EFI_D_ERROR, "wrmsr\n"));
          DEBUG ((EFI_D_ERROR, "LABEL_MSR_SKIP_WRMSR_%d:\n", LabelNumber));
          LabelNumber++;
          SetUbiosLoopbackLabelNumber (LabelNumber);
      }  else {
        DEBUG ((EFI_D_ERROR, "mov ecx, 0%08xh\n", Index));
        DEBUG ((EFI_D_ERROR, "mov edx, 0%08xh\n", (UINT32) (Value >> 32)));
        DEBUG ((EFI_D_ERROR, "mov eax, 0%08xh\n", (UINT32) Value));
        DEBUG ((EFI_D_ERROR, "wrmsr\n"));
      }
      DEBUG ((EFI_D_ERROR, ";MSR_ACCESS_END\n"));
    }
  }
  
  return AsmWriteMsr64Org (Index, Value);
}

