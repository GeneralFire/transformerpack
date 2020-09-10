/** @file
  Non-existing BaseLib functions on Ia32

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

#include <Library/DebugLib.h>

/**
  Disables the 64-bit paging mode on the CPU.

  Disables the 64-bit paging mode on the CPU and returns to 32-bit protected
  mode. This function assumes the current execution mode is 64-paging mode.
  This function is only available on x64. After the 64-bit paging mode is
  disabled, control is transferred to the function specified by EntryPoint
  using the new stack specified by NewStack and passing in the parameters
  specified by Context1 and Context2. Context1 and Context2 are optional and
  may be 0. The function EntryPoint must never return.

  @param  CodeSelector  The 16-bit selector to load in the CS before EntryPoint
                        is called. The descriptor in the GDT that this selector
                        references must be setup for 32-bit protected mode.
  @param  EntryPoint    The 64-bit virtual address of the function to call with
                        the new stack after paging is disabled.
  @param  Context1      The 64-bit virtual address of the context to pass into
                        the EntryPoint function as the first parameter after
                        paging is disabled.
  @param  Context2      The 64-bit virtual address of the context to pass into
                        the EntryPoint function as the second parameter after
                        paging is disabled.
  @param  NewStack      The 64-bit virtual address of the new stack to use for
                        the EntryPoint function after paging is disabled.

**/
VOID
EFIAPI
InternalX86DisablePaging64 (
  IN      UINT16                    CodeSelector,
  IN      UINT32                    EntryPoint,
  IN      UINT32                    Context1,  OPTIONAL
  IN      UINT32                    Context2,  OPTIONAL
  IN      UINT32                    NewStack
  )
{
  //
  // This function cannot work on IA32 platform
  //
  ASSERT (FALSE);
}
