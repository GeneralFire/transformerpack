/** @file PEI implementation of the Checkpoint library class

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

#include <Library/MemoryCoreLib.h>
#include <EvItpDownloadAgent.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CheckpointLib.h>
#include <Library/MemDdrioIpLib.h>
#include "ScratchpadList.h"
#include <Library/SsaLoaderLib.h>
#include <Library/SsaBuiltInRmtLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/OemPlatformCheckpointLib.h>
#include <Library/IoLib.h>

#include <Guid/MrcPlatformHooksGuid.h>

#define KEEP_BREAKPOINT     0xFFFF

/**

  Low Level Manager
  Tool for making BIOS function calls

  @param[in] Host - Pointer to sysHost

  @retval SUCCESS

**/

UINT32
ValRequestHandlerAtBreakPoint (
  IN PSYSHOST Host
  )
{
  UINT8                   Socket;
  UINT8                   CurrentSocket;
  UINT8                   SbspSktId;
  BIOS_SCRATCHPAD5_STRUCT ValFuncs;
  UINT32                  Checkpoint = 0;
  SYS_SETUP               *Setup;

  Setup = GetSysSetupPointer ();

  SbspSktId = GetSysSbspSocketIdNv ();
  ValFuncs.Data = 0;

  //
  // Get the validation functions from a scratchpad register
  //

  GetSetValFunctions (SbspSktId, GET_REQ, &ValFuncs.Data);

  if (ValFuncs.Bits.JedecInitSeq) {
    RcDebugPrint (SDBG_DEFAULT, "Request to run Jedec Init\n");
    //
    // Disable checkpoint breakpoints
    //
    Checkpoint = GetBreakpointCsr (SbspSktId);
    SetBreakpointCsr (SbspSktId, 0);

    //
    // Call for each socket
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

      if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
        continue;
      }

      JedecInitSequence (Host, Socket, CH_BITMASK);
    }

    //
    // Restore checkpoint breakpoint
    //

    SetBreakpointCsr (SbspSktId, Checkpoint);

    //
    // Clear request for JEDEC init
    //

    ValFuncs.Bits.JedecInitSeq = 0;
    GetSetValFunctions (SbspSktId, RESTORE_REQ, &ValFuncs.Data);

  }

  if ((ValFuncs.Bits.Rmt) && (Setup->mem.enableBiosSsaRMT == 1)) {

    RcDebugPrint (SDBG_DEFAULT, "Request to run Rank Margin tool\n");

    //
    // Disable checkpoint breakpoints
    //

    Checkpoint = GetBreakpointCsr (SbspSktId);
    SetBreakpointCsr (SbspSktId, 0);

    RunBuiltInBssaRmt ();

    //
    // Restore checkpoint breakpoint
    //
    SetBreakpointCsr (SbspSktId, Checkpoint);

    //
    // Clear request for RMT
    //
    ValFuncs.Bits.Rmt = 0;
    GetSetValFunctions (SbspSktId, RESTORE_REQ, &ValFuncs.Data);
  }

  if (ValFuncs.Bits.CacheDdrio) {
    RcDebugPrint (SDBG_DEFAULT, "Request to run Cache DDRIO\n");

    //
    // Disable checkpoint breakpoints
    //
    Checkpoint = GetBreakpointCsr (SbspSktId);
    SetBreakpointCsr (SbspSktId, 0);

    CurrentSocket = Host->var.mem.currentSocket;

    //
    // Call for each socket
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
        continue;
      }
      Host->var.mem.currentSocket = Socket;
      CacheDDRIO (Host, Socket);
    }

    Host->var.mem.currentSocket = CurrentSocket;

    //
    // Restore checkpoint breakpoint
    //
    SetBreakpointCsr (SbspSktId, Checkpoint);

    //
    // Clear request for Cache DDRIO
    //
    ValFuncs.Bits.CacheDdrio = 0;
    GetSetValFunctions (SbspSktId, RESTORE_REQ, &ValFuncs.Data);
  }

  return SUCCESS;
} // ValRequestHandlerAtBreakPoint

/**

  Simulation handler for break points

  @param[in] Host       - Pointer to sysHost
  @param[in] SocketId   - ID of the current socket
  @param[in] Checkpoint - The checkpoint major/minor code

  @retval SUCCESS if checkpoint handled
          FAILURE otherwise

**/
UINT32
BreakPointHandlerSim (
  IN    PSYSHOST   Host,
  IN    UINT8      SocketId,
  IN    UINT16     Checkpoint
  )
{

  return FAILURE;
}

/**

  Handler for break points

  @param[in] Host       - Pointer to sysHost
  @param[in] SocketId   - ID of the current socket
  @param[in] Setup      - Pointer to SysSetup
  @param[in] Checkpoint - The checkpoint major/minor code

  @retval SUCCESS if checkpoint handled
          FAILURE otherwise

**/
UINT32
BreakPointHandler (
  IN    PSYSHOST   Host,
  IN    UINT8      SocketId,
  IN    SYS_SETUP  *Setup,
  IN    UINT16     Checkpoint
  )
{
  UINT8  SbspSktId;
  UINT32 BreakpointLocation;

  SbspSktId = GetSysSbspSocketIdNv ();
  //
  // Compare the breakpoint CSR from socket 0
  //
  BreakpointLocation = GetBreakpointCsr (SbspSktId);
  if (BreakpointLocation == Checkpoint) {
    // Output message indicating that execution is stalled
    RcDebugPrint (SDBG_MINMAX, "Breakpoint match found. S%d waiting...\n", SocketId);
    FlushDebugBuffer ();

    // Stall the boot until breakpoint changes
    while (Checkpoint == GetBreakpointCsr (SbspSktId)) {

      ValRequestHandlerAtBreakPoint (Host);

      EnableSsaBiosLoader ();
    } //while

    if (GetBreakpointCsr (SbspSktId) == KEEP_BREAKPOINT) {
      SetBreakpointCsr (SbspSktId, Checkpoint);
    }

  }
  return SUCCESS;
}

/**

  Socket SBSP writes the checkpoint code to the checkpoint CSR on socket SBSP and calls
  the platform checkpoint routine.

  Socket N reads the breakpoint CSR on socket N. If the breakpoint code matches the
  checkpoint code, then execution will stall in a loop until the breakpoint CSR
  on socket N is changed via ITP or other means.

  @param[in] Socket    - Socket to write Checkpoint
  @param[in] MajorCode - Major Checkpoint code to write
  @param[in] MinorCode - Minor Checkpoint code to write
  @param[in] Data      - Data specific to the minor checkpoint is written to
                         low word of the checkpoint CSR

  @retval N/A

**/

VOID
EFIAPI
OutputCheckpointSocket (
  IN UINT8    Socket,
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT16   Data
  )
{
  UINT8    SocketIdBak;
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  SocketIdBak = GetSysSocketId ();

  SetSysSocketId (Socket);

  OutputCheckpoint (MajorCode,  MinorCode, Data);

  SetSysSocketId (SocketIdBak);

  return;

} // OutputCheckpointSocket

/**

  Socket SBSP writes the checkpoint code to the checkpoint CSR on socket SBSP and calls
  the platform checkpoint routine.

  Socket N reads the breakpoint CSR on socket N. If the breakpoint code matches the
  checkpoint code, then execution will stall in a loop until the breakpoint CSR
  on socket N is changed via ITP or other means.

  @param[in] Host      - Pointer to the system Host (root) structure
  @param[in] MajorCode - Major Checkpoint code to write
  @param[in] MinorCode - Minor Checkpoint code to write
  @param[in] Data      - Data specific to the minor checkpoint is written to
                         low word of the checkpoint CSR

  @retval N/A

**/
VOID
EFIAPI
OutputCheckpoint (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode,
  IN UINT16   Data
  )
{
  EFI_STATUS Status;
  UINT16 Checkpoint;
  UINT32 Postcode;
  UINT8  SocketId;
  PSYSHOST Host;
  SYS_SETUP *Setup;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  MRC_STATUS HookStatus;

  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();

  Checkpoint = (UINT16)((MajorCode << 8) | MinorCode);
  SocketId = GetSysSocketId ();
  Postcode =  SetPostCode (SocketId, Checkpoint, Data);

  //
  // Cache last checkpoint value per socket
  //
  Host->var.mem.lastCheckpoint[SocketId] = Postcode;

  if (SocketId ==  GetSysSbspSocketIdNv ()) {

    //
    // Update checkpoint in the Host structure
    //
    SetCheckPoint (Postcode);

    HookStatus = MRC_FUNCTION_NOT_IMPLEMENTED;

    Status = LocateInterface (
      &gMrcPlatformHooksInterfaceGuid,
      0,
      (VOID **)&MrcPlatformHooksInterface
      );
    if (!EFI_ERROR (Status)) {
      HookStatus = MrcPlatformHooksInterface->PlatformCheckpoint (MajorCode, MinorCode, Setup->common.debugPort);
    }

    //
    // If Hooks PlatformCheckpoint returns not implemented, or
    // if the hooks are not present, perform the generic EWL init.
    //

    if (HookStatus == MRC_FUNCTION_NOT_IMPLEMENTED) {
      //
      // Output major checkpoint
      //

      //
      // TODO - Make a NULL IoLib to avoid #ifdef's
      //

      IoWrite8 (Setup->common.debugPort, MajorCode);
      PlatformDisplayCheckpoint (MajorCode, MinorCode);

    }

    //
    // Output the progress code
    //
    RcDebugPrint (SDBG_MAX, "\nCheckpoint Code: Socket %d, 0x%02X, 0x%02X, 0x%04X\n", SocketId, MajorCode, MinorCode, Data);
  }

  //
  // If simulation break point handler doesn't handle it, call the default handler
  //
  if (BreakPointHandlerSim (Host, SocketId, Checkpoint) != SUCCESS) {
    BreakPointHandler (Host, SocketId, Setup, Checkpoint);
  }

  if (FeaturePcdGet (PcdDebugLevelsOverride)) {
    OverrideDebugLevels (Host, SocketId);
  }

  return;

} // OutputCheckpoint
