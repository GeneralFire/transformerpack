/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#include <Memory/MemoryCheckpointCodes.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/RcDebugLib.h>

#define FIXED_BREAKPOINT   ((CHECKPOINT_MAJOR_NO_CODE << 8) | CHECKPOINT_MAJOR_NO_CODE)

/**
  This function is for debug use only and should not normally be called.

  When this function is called, execution will halt for the socket until the user
  clears BIOS_SCRATCHPAD6_STRUCT.EvToolBreakPoint to 0 (Sticky scratchpad 6 BIT[31:16]).
  The contents of the EvToolBreakPoint field will be restored to its original value
  before the function exits.

  @param[in]  Socket      - Socket number to halt

  @retval   EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
FixedBreakpoint (
  IN  UINT8   Socket
  )
{
#ifdef DEBUG_CODE_BLOCK

  UINT32    PreviousBreakpoint;
  UINT32    Breakpoint;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "FixedBreakpoint\n");

  //
  // Save value already in the EvToolBreakPoint field
  //
  PreviousBreakpoint = GetBreakpointCsr (Socket);
  //
  // Write FIXED_BREAKPOINT to EvToolBreakPoint and wait until user clears it to 0
  //
  Breakpoint = FIXED_BREAKPOINT;
  SetBreakpointCsr (Socket, Breakpoint);

  while (Breakpoint != 0) {
    Breakpoint = GetBreakpointCsr (Socket);
  }

  //
  // Restore original EvToolBreakPoint value
  //
  SetBreakpointCsr (Socket, PreviousBreakpoint);
#endif // DEBUG_CODE_BLOCK

  return EFI_SUCCESS;
} // FixedBreakpoint

