/** @file
  AsmMonitor function

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2008 Intel Corporation. <BR>

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
  Sets up a monitor buffer that is used by AsmMwait().

  Executes a MONITOR instruction with the register state specified by Eax, Ecx
  and Edx. Returns Eax. This function is only available on IA-32 and x64.

  @param  RegisterEax The value to load into EAX or RAX before executing the MONITOR
                      instruction.
  @param  RegisterEcx The value to load into ECX or RCX before executing the MONITOR
                      instruction.
  @param  RegisterEdx The value to load into EDX or RDX before executing the MONITOR
                      instruction.

  @return RegisterEax

**/
UINTN
EFIAPI
AsmMonitor (
  IN      UINTN                     RegisterEax,
  IN      UINTN                     RegisterEcx,
  IN      UINTN                     RegisterEdx
  )
{
  _asm {
    mov     eax, RegisterEax
    mov     ecx, RegisterEcx
    mov     edx, RegisterEdx
    _emit   0x0f             // monitor
    _emit   0x01
    _emit   0xc8
  }
}

