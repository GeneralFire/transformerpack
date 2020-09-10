/** @file Smm implementation of the Checkpoint library

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

#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/RcDebugLib.h>
#include <Library/CheckpointLib.h>
#include <Library/BasicIpBlockServicesLib.h>

#include <Guid/MrcPlatformHooksGuid.h>

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
  UINT32 BreakpointLocation = 0;
  UINT8    SbspSktId;
  UINT8    SocketId;
  PSYSHOST Host;
  SYS_SETUP *Setup;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();

  SbspSktId = GetSysSbspSocketIdNv ();

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

    //
    // Call platform function to handle the checkpoint
    //

    Status = LocateInterface (
      &gMrcPlatformHooksInterfaceGuid,
      0,
      (VOID **)&MrcPlatformHooksInterface
      );
    if (!EFI_ERROR (Status)) {
      MrcPlatformHooksInterface->PlatformCheckpoint (MajorCode, MinorCode, Setup->common.debugPort);
    }
    
    //
    // Output the progress code
    //

    RcDebugPrint (SDBG_DEFAULT, "Checkpoint Code: Socket %d, 0x%02X, 0x%02X, 0x%04X\n", SocketId, MajorCode, MinorCode, Data);

  }

  // Compare the breakpoint CSR from socket 0
  BreakpointLocation = GetBreakpointCsr (SbspSktId);
  if (BreakpointLocation == Checkpoint) {
    // Output message indicating that execution is stalled
    RcDebugPrint (SDBG_MINMAX, "Breakpoint match found. S%d waiting...\n", SocketId);
    FlushDebugBuffer ();

    // Stall the boot until breakpoint changes
    while (Checkpoint == GetBreakpointCsr (SbspSktId)) {

      // Do nothing, just wait

    } //while
  }

  return;

} // OutputCheckpoint
