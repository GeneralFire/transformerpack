/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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
#include <Library/RcMemAllocLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/IoAccessLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SemaphoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/HardwareSemaphoreTargetLib.h>
#include <Library/SsaInitializeLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>
#include <Library/PerformanceTrackerLib.h>
#include <Library/PsmiInitLib.h>

#include <Guid/MrcPlatformHooksGuid.h>

#include "Include/BaseMemoryCoreInternal.h"


//
// Internal function declarations
//
STATIC VOID InitializeHeap (PSYSHOST Host, UINT8 *HeapPtr, UINT32 HeapSize);
STATIC VOID InitializeTimers (PSYSHOST Host);
STATIC VOID InitializeDataStructures (PSYSHOST Host);
STATIC VOID ExecuteEarlyUncoreInit (PSYSHOST Host);
STATIC VOID StartPipeInterface (PSYSHOST Host);
STATIC VOID ExecuteEarlyCpuInit (PSYSHOST Host);
STATIC VOID ExecuteMemoryInit (PSYSHOST Host);
STATIC VOID DisplayWarningLog (PSYSHOST Host);
STATIC VOID StopPipeInterface (PSYSHOST Host);
STATIC VOID CoreCheckAndHandleResetRequests (PSYSHOST Host);
STATIC VOID PublishProcMemInitResults (PSYSHOST Host);
STATIC VOID PrintValuesForPlatformConfigXml (PSYSHOST Host);

/**
  Populate and validate the memFunctions pointer table based on the CPUID
  and execute the high level ProcMemInit flow

  @param Host - Pointer to sysHost structure

  @retval N/A
**/
VOID
ProcMemInitMain (
  PSYSHOST    Host
  )
{
  UINT32      Status = SUCCESS;
  INT32       DdrioUltSupport = 0;

  //
  // Save context for Exit() function
  //
  PUSH_EXIT_FRAME (Host);

  //
  // Find the supported processor and call its entry point
  //
  if (ProcessorSupported(Host)) {

      //
      // Initializes debug message infrastructure. transfer the Setup->common.serialDebugMsgLvl data to Debug->MessageLevel.
      //
      InitializeDebugMessages ();

      //
      // Init DdrioUltSupport for Cte builds only
      //
      Host->DdrioUltSupport = 0;
      if (EFI_SUCCESS == CteGetOptions ("ddrio_ult", &DdrioUltSupport)) {
        if (DdrioUltSupport) {
          Host->DdrioUltSupport = 1;
        }
      }

      RcDebugPrint (SDBG_MAX, "Force creation of Debug Data\n...");

      //
      // Initialize Timer Functions
      //
      InitializeTimers (Host);

      //
      // Initialize Host structure defaults
      //

      InitializeDataStructures (Host);

      //
      // Initialize SSA support (SsaBiosInitialize) does following function
      // a. Install SSA PPI
      // b. Check for configuration variable set by EFI SSA Loader tool in NVRAM. if found, update
      //    extracted SSA checkpoint to scratchpad register along with necessary SSA Opcode.
      //    This enables execution of the SSA Loader tool at given checkpoint.
      // c. This function (SsaBiosInitialize) needs to be available as early as possible in MRC
      //    with possible advantage of EV support as soon in MRC as possible.
      // d. But, this function can only be executed after InitializeDataStructures, since only
      //    after this function, we have access to USRA library, which helps writing to Scratchpad register
      //
      SsaBiosInitialize ((EFI_PEI_SERVICES **)Host->var.common.oemVariable);
      //
      // Execute Uncore initialization
      //
      ExecuteEarlyUncoreInit(Host);
      //
      // Update Semaphore sync data based on the present sockets.
      //
      UpdateSemaphoreSyncData (
        (PRINT_SYNC_ACQUIRE)AcquireGenSemaphoreControl,
        (PRINT_SYNC_RELEASE)ReleaseGenSemaphoreControl
        );

      //
      // Start Pipe interface for multi-threaded commands
      //
      StartPipeInterface (Host);

      //
      // Execute Early CPU initialization
      //
      ExecuteEarlyCpuInit (Host);

      //
      // Execute PSMI initialization
      //

      PsmiInit ();

      //
      // Execute Memory initialization
      //
      ExecuteMemoryInit (Host);

      //
      // Stop Pipe interface
      //
      StopPipeInterface (Host);

      //
      // Display Warning Log
      //
      DisplayWarningLog (Host);

      //
      // Reset debug message infrastructure
      //
      ResetDebugMessages ();

      //
      // Check and Handle requested resets
      //
      CoreCheckAndHandleResetRequests(Host);

      //
      // Execute Late PSMI Flow
      //
      ConfigurePsmi ();

      //
      // Publish results
      //
      PublishProcMemInitResults(Host);

  } else {
    RcDebugPrint (SDBG_DEFAULT, "\n!! ProcMemInitMain: Unsupported Processor !!\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_118);
  }

  POP_EXIT_FRAME(Host, Status);
  if (Status) {
    Host->var.common.rcStatus = Status;
  }
} //ProcMemInitMain

STATIC
VOID
InitializeTimers(
  PSYSHOST    Host
  )
{
  //
  // Initialize the power management timer base address
  //
  InitPmTimer ();

  //
  // Initialize Cpu Timer
  //
  InitCpuTimer();
}

STATIC
VOID
InitializeDataStructures(
  PSYSHOST    Host
  )
{
  UINT8 Socket;
  SYS_SETUP *Setup;

  SetMemPhase (PTZ_PRE_MRC);
  //
  // Initialize Host structure defaults
  //
  RcDebugPrint (SDBG_DEFAULT, "InitializeDefaultData() \n");
  InitializeDefaultData(Host);

  //
  // Get the setup pointer after it has been initialized
  // in InitializeDefaultData
  //

  Setup = GetSysSetupPointer ();

  //
  // Initialize Memory Subsystem parameters
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    Host->var.mem.socket[Socket].NumMemSs = MAX_MEM_SS;
    GetMemSubsystemMap (Host, Socket,
                  &Host->var.mem.socket[Socket].NumMemSs,
                  &Host->var.mem.socket[Socket].MemSsList[0]);
  }

  //
  // Initialize platform segment of sysHost
  //
  RcDebugPrint (SDBG_DEFAULT, "InitializePlatformData() \n");
  InitializePlatformData(Host);

  RcDebugPrint (SDBG_DEFAULT, "sizeof sysHost = %d\n", sizeof(SYSHOST));

  if (UbiosGenerationEnabled ()) {
#ifdef DEBUG_CODE_BLOCK
    SetDebugLevel (SDBG_MINMAX | GetDebugLevel ());
#endif // DEBUG_CODE_BLOCK
    SetUbiosOutputMode (ASM_OUTPUT_ENABLE);
    RcDebugPrint (SDBG_DEFAULT, "\n\nuBIOS %a\n ", __FILE__);
  } else if (HybridSystemLevelEmulationEnabled ()) {
    RcDebugPrint (SDBG_DEFAULT, "\n\nSLEBios %a\n ", __FILE__);
    Setup->mem.dfxMemSetup.dfxOptions &= ~CA_PARITY_EN;
  }

}

STATIC
VOID
StartPipeInterface(
  PSYSHOST    Host
  )
{
  //
  // Initialize Pipe Architecture for use by MRC
  //
  RcDebugPrint (SDBG_DEFAULT, "Pipe Init starting...\n");
  InitializePipe (Host);
  RcDebugPrint (SDBG_DEFAULT, "Pipe Init completed! Reset Requested: %x\n", GetSysResetRequired());
}

STATIC
VOID
ExecuteMemoryInit(
  PSYSHOST    Host
  )
{
  UINT32  data32;

  //
  // Invoke memory initialization (MRC)
  //
  {
    RcDebugPrint (SDBG_DEFAULT, "START_MRC_RUN\n");
    data32 = Host->var.common.printfDepth;

    MemoryInit (Host);

    //
    // check if global semaphore ownership has been properly managed
    //
    if (Host->var.common.printfDepth != data32) {
        RcDebugPrint (SDBG_DEFAULT, "MRC internal error: Global Semaphore Ownership Out of Sync\n");
        RcDebugPrint (SDBG_DEFAULT, "printfDepth was %x before MRC, is %x after MRC\n", data32, Host->var.common.printfDepth);
        FatalError (PRINTF_OUTOF_SYNC_ERR_MAJOR, PRINTF_OUTOF_SYNC_ERR_MINOR);
    }
    RcDebugPrint (SDBG_DEFAULT, "Reset Requested: %x\n", GetSysResetRequired());
  }
}

STATIC
VOID
ExecuteEarlyUncoreInit(
  PSYSHOST    Host
  )
{
  EarlyUncoreInit(Host);

}  //ExecuteEarlyUncoreInit

STATIC
VOID
ExecuteEarlyCpuInit(
  PSYSHOST    Host
  )
{
  EarlyCpuInit(Host);

}  //ExecuteEarlyCpuInit

STATIC
VOID
StopPipeInterface(
  PSYSHOST    Host
  )
{
  //
  // Exit Pipe Architecture
  //
  RcDebugPrint (SDBG_DEFAULT, "Pipe Exit starting...\n");
  ExitPipe (Host);
  RcDebugPrint (SDBG_DEFAULT, "Pipe Exit completed! Reset Requested: %x\n", GetSysResetRequired());
}


/**

  Displays any entries found in the warning log
  There are two versions of this function:
  One for the Enhanced Warning Log
  One for the legacy warning log

  @param - Pointer to sysHost structure

  @retval VOID

**/
STATIC
VOID
DisplayWarningLog (
  PSYSHOST Host
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT32 logOffset = 0;
  RcDebugPrint (SDBG_DEFAULT, "Enhanced Warning Log: \n");

  //
  // Return if debug messages are disabled
  //
  if (GetDebugLevel () == 0) {
    return;
  }

  while (logOffset < (Host->var.common.ewlPrivateData.status.Header.FreeOffset)) {
    //
    // For now, just treat as legacy warning and print code/checkpoint/memloc
    //
    EWL_ENTRY_HEADER *warningHeader = (EWL_ENTRY_HEADER*) &(Host->var.common.ewlPrivateData.status.Buffer[logOffset]);
    EwlPrintWarning (warningHeader);
    logOffset += warningHeader->Size;
  }
#endif // DEBUG_CODE_BLOCK
} // DisplayWarningLog

STATIC
VOID
CoreCheckAndHandleResetRequests (
  PSYSHOST Host
  )
{
  if (FeaturePcdGet (PcdCteBuild) == FALSE) {

    //
    // Call into chip layer to handle the reset
    //
    CheckAndHandleResetRequests (Host);
  }

  return;
}

/**

  Check for Board VS CPU conflict.
  Refcode will halt execution. OEMs might prefer to allow system to boot to
  video and display error code on screen.

  @param Host - pointer to sysHost

**/
VOID
CheckForBoardVsCpuConflicts (
  PSYSHOST Host
  )
{
  EFI_STATUS Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  MRC_STATUS HookStatus;
  UINT8      BoardVsCpuConflicts;

  //
  // Check to see if the hook is present
  //

  HookStatus = MRC_FUNCTION_NOT_IMPLEMENTED;
  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    HookStatus = MrcPlatformHooksInterface->OemCheckForBoardVsCpuConflicts (Host);
  }

  //
  // If the hook was present, and it handled checking,
  // skip doing it ourselves.
  //

  if (HookStatus != MRC_FUNCTION_NOT_IMPLEMENTED) {
    return;
  }

  BoardVsCpuConflicts = GetBoardVsCpuConflicts ();

  if (BoardVsCpuConflicts != 0) {
    RcDebugPrint (SDBG_DEFAULT, "\nBoard VS CPU conflict detected: 0x%x\n", BoardVsCpuConflicts);
    RcDeadLoop (1);
  }

} // CheckForBoardVsCpuConflicts

STATIC
VOID
PublishProcMemInitResults (
  PSYSHOST Host
  )
{
  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    return;
  }

  //
  // In Emulation, we print some variables out
  // to create PlatformConfig.xml
  //
  PrintValuesForPlatformConfigXml (Host);

  //
  // If control comes here, warm boot path has completed.
  // Return and continue with the reset of BIOS POST...
  //
  RcDebugPrint (SDBG_DEFAULT, "None \nContinue with system BIOS POST ...\n\n");

  //
  // Check for Board VS CPU conflicts
  //

  CheckForBoardVsCpuConflicts (Host);

  //
  // Publish sysHost information to be used in Post Phase
  //
  PublishDataForPost (Host);

  // TODO: can this be moved to end of MRC?
  FixupPamAttributes(Host);

  return;
}

/**

  Print values required for creation of the PlatformConfig.xml,
  required for CAFE validation.

  @param Host - pointer to sysHost structure on stack

  @retval VOID

**/
STATIC VOID
PrintValuesForPlatformConfigXml (
  PSYSHOST  Host
  )
{
  if (UbiosGenerationOrHsleEnabled ()) {
    RcDebugPrint (SDBG_DEFAULT, "\nPLATFORM CAPABILITIES START\n");
    RcDebugPrint (SDBG_DEFAULT, "  Host->var.common.pmBase   = 0x%04x\n", Host->var.common.pmBase);
    RcDebugPrint (SDBG_DEFAULT, "  TOTAL RAM                 = %d GB\n", (Host->var.mem.memSize >> 4));
    RcDebugPrint (SDBG_DEFAULT, "PLATFORM CAPABILITIES END\n\n");
  }
}
