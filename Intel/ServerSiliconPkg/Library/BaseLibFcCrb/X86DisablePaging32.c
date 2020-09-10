/** @file
  IA-32/x64 AsmDisablePaging32()

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

#include "BaseLibInternals.h"

/**
  Disables the 32-bit paging mode on the CPU.

  Disables the 32-bit paging mode on the CPU and returns to 32-bit protected
  mode. This function assumes the current execution mode is 32-paged protected
  mode. This function is only available on IA-32. After the 32-bit paging mode
  is disabled, control is transferred to the function specified by EntryPoint
  using the new stack specified by NewStack and passing in the parameters
  specified by Context1 and Context2. Context1 and Context2 are optional and
  may be NULL. The function EntryPoint must never return.

  If the current execution mode is not 32-bit paged mode, then ASSERT().
  If EntryPoint is NULL, then ASSERT().
  If NewStack is NULL, then ASSERT().

  There are a number of constraints that must be followed before calling this
  function:
  1)  Interrupts must be disabled.
  2)  The caller must be in 32-bit paged mode.
  3)  CR0, CR3, and CR4 must be compatible with 32-bit paged mode.
  4)  CR3 must point to valid page tables that guarantee that the pages for
      this function and the stack are identity mapped.

  @param  EntryPoint  A pointer to function to call with the new stack after
                      paging is disabled.
  @param  Context1    A pointer to the context to pass into the EntryPoint
                      function as the first parameter after paging is disabled.
  @param  Context2    A pointer to the context to pass into the EntryPoint
                      function as the second parameter after paging is
                      disabled.
  @param  NewStack    A pointer to the new stack to use for the EntryPoint
                      function after paging is disabled.

**/
VOID
EFIAPI
AsmDisablePaging32 (
  IN      SWITCH_STACK_ENTRY_POINT  EntryPoint,
  IN      VOID                      *Context1,  OPTIONAL
  IN      VOID                      *Context2,  OPTIONAL
  IN      VOID                      *NewStack
  )
{
  ASSERT (EntryPoint != NULL);
  ASSERT (NewStack != NULL);
  InternalX86DisablePaging32 (EntryPoint, Context1, Context2, NewStack);
}
