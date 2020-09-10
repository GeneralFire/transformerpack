/** @file
  AsmCpuidEx function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2010 Intel Corporation. <BR>

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
  Retrieves CPUID information using an extended leaf identifier.

  Executes the CPUID instruction with EAX set to the value specified by Index
  and ECX set to the value specified by SubIndex. This function always returns
  Index. This function is only available on IA-32 and x64.

  If Eax is not NULL, then the value of EAX after CPUID is returned in Eax.
  If Ebx is not NULL, then the value of EBX after CPUID is returned in Ebx.
  If Ecx is not NULL, then the value of ECX after CPUID is returned in Ecx.
  If Edx is not NULL, then the value of EDX after CPUID is returned in Edx.

  @param  Index         The 32-bit value to load into EAX prior to invoking the
                        CPUID instruction.
  @param  SubIndex      The 32-bit value to load into ECX prior to invoking the
                        CPUID instruction.
  @param  RegisterEax   A pointer to the 32-bit EAX value returned by the CPUID
                        instruction. This is an optional parameter that may be
                        NULL.
  @param  RegisterEbx   A pointer to the 32-bit EBX value returned by the CPUID
                        instruction. This is an optional parameter that may be
                        NULL.
  @param  RegisterEcx   A pointer to the 32-bit ECX value returned by the CPUID
                        instruction. This is an optional parameter that may be
                        NULL.
  @param  RegisterEdx   A pointer to the 32-bit EDX value returned by the CPUID
                        instruction. This is an optional parameter that may be
                        NULL.

  @return Index.

**/
UINT32
EFIAPI
AsmCpuidEx (
  IN      UINT32                    Index,
  IN      UINT32                    SubIndex,
  OUT     UINT32                    *RegisterEax,  OPTIONAL
  OUT     UINT32                    *RegisterEbx,  OPTIONAL
  OUT     UINT32                    *RegisterEcx,  OPTIONAL
  OUT     UINT32                    *RegisterEdx   OPTIONAL
  )
{
  _asm {
    mov     eax, Index
    mov     ecx, SubIndex
    cpuid
    push    ecx
    mov     ecx, RegisterEax
    jecxz   SkipEax
    mov     [ecx], eax
SkipEax:
    mov     ecx, RegisterEbx
    jecxz   SkipEbx
    mov     [ecx], ebx
SkipEbx:
    pop     eax
    mov     ecx, RegisterEcx
    jecxz   SkipEcx
    mov     [ecx], eax
SkipEcx:
    mov     ecx, RegisterEdx
    jecxz   SkipEdx
    mov     [ecx], edx
SkipEdx:
    mov     eax, Index
  }
}
