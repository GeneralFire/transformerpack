/** @file
  IA-32/x64 AsmEnablePaging64()

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
  Enables the 64-bit paging mode on the CPU.

  Enables the 64-bit paging mode on the CPU. CR0, CR3, CR4, and the page tables
  must be properly initialized prior to calling this service. This function
  assumes the current execution mode is 32-bit protected mode with flat
  descriptors. This function is only available on IA-32. After the 64-bit
  paging mode is enabled, control is transferred to the function specified by
  EntryPoint using the new stack specified by NewStack and passing in the
  parameters specified by Context1 and Context2. Context1 and Context2 are
  optional and may be 0. The function EntryPoint must never return.

  If the current execution mode is not 32-bit protected mode with flat
  descriptors, then ASSERT().
  If EntryPoint is 0, then ASSERT().
  If NewStack is 0, then ASSERT().

  @param  Cs          The 16-bit selector to load in the CS before EntryPoint
                      is called. The descriptor in the GDT that this selector
                      references must be setup for long mode.
  @param  EntryPoint  The 64-bit virtual address of the function to call with
                      the new stack after paging is enabled.
  @param  Context1    The 64-bit virtual address of the context to pass into
                      the EntryPoint function as the first parameter after
                      paging is enabled.
  @param  Context2    The 64-bit virtual address of the context to pass into
                      the EntryPoint function as the second parameter after
                      paging is enabled.
  @param  NewStack    The 64-bit virtual address of the new stack to use for
                      the EntryPoint function after paging is enabled.

**/
VOID
EFIAPI
AsmEnablePaging64 (
  IN      UINT16                    Cs,
  IN      UINT64                    EntryPoint,
  IN      UINT64                    Context1,  OPTIONAL
  IN      UINT64                    Context2,  OPTIONAL
  IN      UINT64                    NewStack
  )
{
  ASSERT (EntryPoint != 0);
  ASSERT (NewStack != 0);
  InternalX86EnablePaging64 (Cs, EntryPoint, Context1, Context2, NewStack);
}
