/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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
#include <Library/PipeSyncLib.h>
#include <Library/CteNetLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/KtiApi.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/CheckpointLib.h>
#include <Library/IoAccessLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/TimerLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/CallTablePolicyLib.h>
#include <Library/UsraCsrLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Include/MemMapHost.h>
#include <Library/RcMemAllocLib.h>
#include <Library/PerformanceTrackerLib.h>

#define DATA_TYPE_VAR                 0x01
#define DATA_TYPE_NVRAM_COMMON        0x02
#define DATA_TYPE_NVRAM_DATA          0x03

//
// Local Prototypes
//
UINT32 CheckPF (PSYSHOST Host, UINT8 policyFlag);
UINT32 CheckMPFlag (PSYSHOST Host, UINT8 MPFlag);
UINT32 SendMemoryMode (PSYSHOST Host);
UINT32 GetMemoryMode (PSYSHOST Host);

// -------------------------------------
// Code section
// -------------------------------------

/**
  Main function used to initialize the memory controler on each processor.

  @param Host:              Pointer to sysHost
  @param ctptrOrg:          Pointer to the CallTable to be used
  @param callTableEntries:  Size of the CallTable
  @param TargetSocket:      Socket number to excute the CallTable

  @retval SUCCESS:  Memory initialization completed successfully.
  @retval FAILURE:  A reset is required
**/
UINT32
MemMain (
  PSYSHOST                  Host,
  CallTableEntryType        *ctptrOrg,
  UINT8                     callTableEntries,
  UINT8                     TargetSocket
  )
{
  UINT8                     socket;
  UINT8                     SysSocketId;
  UINT8                     startSocket;
  UINT8                     maxSocket;
  UINT8                     tableIndex;
  UINT64                    startTsc = 0;
  UINT64                    firstStartTsc = 0;
  UINT64                    endTsc = 0;
  UINT32                    status;
  CallTableEntryType        *ctptr;
  PT_ZONE                   CurrentMemPhase;
  SYS_SETUP                 *Setup;

  Setup = GetSysSetupPointer ();
  SysSocketId = GetSysSocketId ();

  status = FAILURE;

  //
  // Loop through the table
  //
  for (tableIndex = 0; tableIndex < callTableEntries; tableIndex++) {
    ctptr = &ctptrOrg[tableIndex];

    //
    // Decide if we need to execute the selected MRC task
    //
    if (ctptr->mrcTask == NULL) {
      //RcDebugPrint (SDBG_MINMAX,
      //              "mrcTask == NULL; tableIndex = %d\n", tableIndex);
      continue;
    }

    GetMemPhase (&CurrentMemPhase);
    SetMemPhase (PTZ_CALL_TABLE);
    //
    // Check policy flag and MP flag before calling this function
    //
    if (CheckPF (Host, ctptr->policyFlag) == SUCCESS &&
        CheckMPFlag (Host, ctptr->mpFlag) == SUCCESS) {

      //
      // Output debug string to serial output and execute the MRC task
      //
#ifdef DEBUG_CODE_BLOCK
      AcquirePrintControl ();
      //RcDebugPrint (SDBG_MINMAX,
      //              ctptr->debug_string_ptr);
      RcDebugPrint (SDBG_DEFAULT, ctptr->debug_string_ptr);
      RcDebugPrint (SDBG_DEFAULT, " -- Started\n");
      ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK

      //
      // Output the POST code
      //

      Host->var.mem.PostCodeMinor = ctptr->postCodeMinor;

      if ((ctptr->postCodeMajor != 0) && (ctptr->postCodeMajor != 0xFF)) {
        OutputCheckpoint (ctptr->postCodeMajor, ctptr->postCodeMinor, 0);
      }

      //
      // Set up socket loop
      //
      if ((Setup->mem.options & MULTI_THREAD_MRC_EN)) {
        startSocket = SysSocketId;
        maxSocket = startSocket + 1;
      } else {
        if (ctptr->mpFlag & MRC_MP_SERIAL_LOOP) {
          // Loop for each socket
          startSocket = 0;
          maxSocket = MAX_SOCKET;
        } else {
          startSocket = TargetSocket;
          maxSocket = startSocket + 1;
        }
      }

      if ((ctptr->postCodeMajor != 0) && (ctptr->postCodeMajor != 0xFF)) {
        if (EFI_SUCCESS != SetMemPhaseCP (ctptr->postCodeMajor, ctptr->postCodeMinor)) {
          //
          // This path is taken if a call table entry has a post code but the post code isn't mapped to
          // a performance tracker zone. In this case, clear the zone (equivalent to SetMemPhase (PTZ_NOZONE)).
          //
          ClearMemPhase (PTZ_CALL_TABLE);
        }
      } else {
        SetMemPhase (CurrentMemPhase);
      }
      if (PcdGetBool (PcdMrcDebugPerformanceStats) == TRUE) {
        //
        // Start time
        //
        startTsc = GetPerformanceCounter ();
        if ((firstStartTsc == 0)) {
          //
          // Save the first instance so we can calculate the total time spent in the MRC
          //
          firstStartTsc = startTsc;
        }
      }

      status = SUCCESS;

      for (socket = startSocket; socket < maxSocket; socket++) {
        if (!SocketPresent (socket)) {
          continue;
        }

        //
        // Save the socket that this function is being called for
        //
        Host->var.mem.currentSocket = socket;

        //
        // Hook to call functions before executing MRC Task
        //
        PreMrcTaskPolicy (ctptr->policyFlag);

        //
        // Call the function
        //
        status |= ctptr->mrcTask (Host);

        //
        // Hook to call functions after executing MRC Task
        //
        PostMrcTaskPolicy (ctptr->policyFlag);
      } // socket loop

      if (PcdGetBool (PcdMrcDebugPerformanceStats) == TRUE) {
        //
        // End time
        //
        endTsc = GetPerformanceCounter ();

        CountTrackingData (PTC_DURATION_TIME, TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        GetMemPhase (&CurrentMemPhase);
        SetMemPhase (PTZ_CALL_TABLE);
      }
#ifdef DEBUG_CODE_BLOCK
      AcquirePrintControl ();
      //RcDebugPrint (SDBG_MINMAX,
      //              ctptr->debug_string_ptr);
      RcDebugPrint (SDBG_DEFAULT, ctptr->debug_string_ptr);
#endif //DEBUG_CODE_BLOCK

      //
      // Only check to see if a reset is required. Do not exit the dispatcher for failures.
      //
      if (HybridSystemLevelEmulationEnabled ()) {
        //
        //FORCE status sucessful for Hybrid model
        //
        status = 0;
      }
      if ((status == POST_RESET_WARM) || (status == POST_RESET_POWERGOOD)) {
#ifdef DEBUG_CODE_BLOCK
        RcDebugPrint (SDBG_DEFAULT, " -- EXIT, status = %Xh\n", status);
        ReleasePrintControl ();
#endif //DEBUG_CODE_BLOCK

        break;
      } else {
#ifdef DEBUG_CODE_BLOCK
        RcDebugPrint (SDBG_DEFAULT, " - %dms\n", TimeDiff (startTsc, endTsc, TDIFF_UNIT_MS));
        ReleasePrintControl ();
#endif //DEBUG_CODE_BLOCK
      }
    }
#ifdef DEBUG_CODE_BLOCK
    else {
      RcDebugPrint (SDBG_MAX,
                    "%a -- Skipped\n", ctptr->debug_string_ptr);
    }
#endif //DEBUG_CODE_BLOCK
    SetMemPhase (CurrentMemPhase);
  } // tableIndex loop

#ifdef DEBUG_CODE_BLOCK
  RcDebugPrint (SDBG_DEFAULT, "Total MRC time = %dms\n", TimeDiff (firstStartTsc, endTsc, TDIFF_UNIT_MS));
#endif //DEBUG_CODE_BLOCK
  return status;
} // MemMain

/**

  This function handles SBSP dispatch and the slave processors acknowledgement

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS - Memory initialization completed successfully.
  @retval FAILURE - A reset is required

**/
UINT32
ParallelModeDispatch (
  PSYSHOST                 Host
  )
{
  if (GetSysSocketId () == GetSysSbspSocketIdNv ()) {
    return DispatchSlaves (Host);
  } else {
    return SlaveAck (Host);
  }
}

/**
  This function dispatches the slave processors to start MemMain

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
  @retval FAILURE:  A reset is required
**/
UINT32
DispatchSlaves (
  PSYSHOST                  Host
  )
{
  UINT8         socket;
  COMMAND_PIPE  cmdPipe;

  cmdPipe.RawPipe = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    Host->var.mem.socket[socket].fatalError = 0;
    Host->var.mem.socket[socket].majorCode  = 0;
    Host->var.mem.socket[socket].minorCode  = 0;

    if (!SocketPresent (socket)) {
      continue;
    }

    //
    // Do not execute this for the SBSP
    //
    if (socket == GetSysSocketId ()) {
      continue;
    }

    //
    // Initialize command pipe of slave before starting the slave
    //
    LockCommandPipe (Host, socket);

    //
    // Can now dispatch the slaves
    // Cause the pipe dispatcher to call the memory init entry point on each slave
    //
    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DIMM_DETECT, CHECKPOINT_MINOR_DISPATCH_SLAVES, socket));
    RcDebugPrint (SDBG_MINMAX,
                  "Dispatch N%d for MemInit\n", socket);

    UsraCsrWrite (socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR (Host), PIPE_DISPATCH_MEMORY_INIT);

    //
    // Parallel DDR Init. Send PIPE_RUN_MEMORY_COMMAND to each slave (package BSPs)
    //
    cmdPipe.Pipe.InternalCmd = PIPE_RUN_MEMORY_COMMAND;

    WaitForCommandPipeUnLocked (Host, socket);
    cmdPipe.Pipe.CmdRequest = PIPE_REQUEST;
    cmdPipe.Pipe.TargetId = socket;
    SendCommandPipe (Host, socket, &cmdPipe.RawPipe);
  } // socket loop

  return SUCCESS;
} // DispatchSlaves


/**
  This function dispatches the slave processors to start MemMain

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
  @retval FAILURE:  A reset is required
**/
UINT32
SlaveAck (
  PSYSHOST                  Host
  )
{
  COMMAND_PIPE  cmdPipe;
  UINT8         SocketId;

  cmdPipe.RawPipe = 0;
  SocketId = GetSysSocketId ();

  UnlockCommandPipe (Host, SocketId);

  //
  // Waits for PIPE Master's PIPE_RUN_MEMORY_COMMAND to run memory init
  //
  WaitForPipeCommand (Host, SocketId, RUN_MEMORY_COMMAND);

  //
  // Respond ACK
  //
  cmdPipe.Pipe.Status     = PIPE_SUCCESS;
  cmdPipe.Pipe.NumOfBytes = 0;
  cmdPipe.Pipe.Width_     = 0;
  cmdPipe.Pipe.CmdRequest = PIPE_ACK;
  WriteCommandPipe (Host, SocketId, &cmdPipe.RawPipe);

  return SUCCESS;
} // SlaveAck

/**
  This function syncs data across sockets

  @param Host:         Pointer to sysHost

  @retval SUCCESS: Sync successfull
**/
UINT32
PipeSync (
  PSYSHOST                  Host
  )
{
  UINT32  status = SUCCESS;

  //
  // Check to make sure none of the threads have encountered an error
  //
  SyncErrors ();

  if (GetSysSocketId () == GetSysSbspSocketIdNv ()) {
    switch (Host->var.mem.PostCodeMinor) {
    case CHECKPOINT_MINOR_PIPE_AP_BOOT_MODE:
      status = GetSocketbootmode (Host);
      break;
    case CHECKPOINT_MINOR_PIPE_AP_DATA:
      status = GetSlaveData ();
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_BOOT_MODE:
      status = Sendbootmode (Host);
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_STATUS:
      status = SendStatus ();
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_DATA:
      status = SendData ();
      break;
    case CHECKPOINT_MINOR_PIPE_AP_SSA_DATA:
      status = GetSlaveSsaData (Host); // Sync3: Get the req. sysHost data from slave processors to BSP pre BDAT/BSSA
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_SSA_DATA:
      status = SendSsaData (Host); // Sync4: Send the req. data from the BSP to slave processors post BDAT/BSSA
      break;
    case CHECKPOINT_MINOR_PIPE_THERMAL_SYNC: // Sync BSP and APs after initThrottlingLate()
      break;
    case CHECKPOINT_MINOR_PIPE_AP_NVRAM_DATA:
      status = GetNvramData (Host); //BSP needs nvram data from APs to create a consolidated block that can be copied over to the nvram region
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_VAR_DATA:
      status = SendVarData (Host);
      break;
    case CHECKPOINT_MINOR_PIPE_CPL2: // Sync before CPL2 bit is set
      break;
    case CHECKPOINT_MINOR_PIPE_AP_FINAL_ERROR_SYNC: // Check errors in SyncErrors()
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_MEM_MODE:
      status = SendMemoryMode (Host);
      break;
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_79)
    }
  } else {
    switch (Host->var.mem.PostCodeMinor) {
    case CHECKPOINT_MINOR_PIPE_AP_BOOT_MODE:
      status = SendSocketbootmode (Host);
      break;
    case CHECKPOINT_MINOR_PIPE_AP_DATA:
      status = SendSlaveData (Host);
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_BOOT_MODE:
      status = Getbootmode (Host);
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_STATUS:
      status = GetStatus (Host);
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_DATA:
      status = GetData (Host);
      break;
    case CHECKPOINT_MINOR_PIPE_AP_SSA_DATA:
      status = SendSlaveSsaData(Host); //  Sync3: Send the req. sysHost data from slave processors to BSP pre BDAT/BSSA
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_SSA_DATA:
      status = GetSsaData(Host); // Sync4: Get data from the BSP to slave processors post BDAT/BSSA
      break;
    case CHECKPOINT_MINOR_PIPE_THERMAL_SYNC: // Sync BSP and APs after initThrottlingLate()
      break;
    case CHECKPOINT_MINOR_PIPE_AP_NVRAM_DATA:
      status = SendNvramData(Host); //Send nvram data from each AP to the BSP
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_VAR_DATA:
      status = GetVarData (Host);
      break;
    case CHECKPOINT_MINOR_PIPE_CPL2: // Sync before CPL2 bit is set
      break;
    case CHECKPOINT_MINOR_PIPE_AP_FINAL_ERROR_SYNC: // Check errors in SyncErrors()
      break;
    case CHECKPOINT_MINOR_PIPE_SBSP_MEM_MODE:
      status = GetMemoryMode(Host); // Get Memory Mode
      break;
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_79)
    }
  }
  return status;
} // PipeSync

/**
  This function gets data from slave processors

  @retval SUCCESS
**/
UINT32
GetSlaveData (
  VOID
  )
{
  UINT8   socket;
  UINT16  sbspSktId;
  PSYSHOST Host;

  Host = GetSysHostPointer ();
  sbspSktId = GetSysSbspSocketIdNv ();

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (socket)) {
      continue;
    }

    if (socket == GetSysSocketId ()) {
      continue;
    }

    RcDebugPrint (SDBG_MAX,
                  "GetSlaveData S%d -> S%d\n", socket, sbspSktId);

    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DIMM_DETECT, CHECKPOINT_MINOR_GET_SLAVE_DATA, (UINT16) ((socket << 8) |DATA_TYPE_VAR)));

    GetReadyToTransferData (Host, socket);

    GetMemVarData (Host, socket);

    GetProcessorCommonVarData (Host, socket);

    GetMemNvramCommonData (Host, socket);
  } // socket loop

  return SUCCESS;
} // GetSlaveData

/**
  This function sends data from slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
SendSlaveData (
  PSYSHOST                  Host
  )
{
  UINT16  sbspSktId;
  UINT8   SocketId;

  sbspSktId = GetSysSbspSocketIdNv ();
  SocketId = GetSysSocketId ();

  RcDebugPrint (SDBG_MAX,
                "SendSlaveData S%d -> S%d\n", SocketId, sbspSktId);

  SendReadyToTransferData (Host, SocketId);

  SendMemVarData (Host, SocketId);

  SendProcessorCommonVarData (Host, SocketId);

  SendMemNvramCommonData (Host, SocketId);

  return SUCCESS;
} // SendSlaveData

/**
  This function sends the DDR frequency to slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
SendFreq (
  PSYSHOST                  Host
  )
{
  UINT8   socket;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (socket)) {
      continue;
    }

    if (socket == GetSysSocketId ()) {
      continue;
    }

    //
    // Send the status variable to each socket
    //
    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CLOCK_INIT, CHECKPOINT_MINOR_SEND_FREQ, socket));
    SendPipePackage (Host, socket, (UINT8*)&Host->nvram.mem.socket[socket].ddrFreq, sizeof (UINT8), RCSIM_NO_DELAY);

  } // socket loop

  // Return requesting a reset
  return SUCCESS;
} // SendFreq

/**
  This function gets the DDR frequency from the master processor

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
GetFreq (
  PSYSHOST                  Host
  )
{

  GetPipePackage (GetSysSocketId (), (UINT8*)&Host->nvram.mem.socket[Host->var.mem.currentSocket].ddrFreq, sizeof (UINT8));
  return SUCCESS;

} // GetFreq

/**
  This function returns the reset status

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
CheckStatus (
  PSYSHOST  Host
  )
{

  // Return requesting a reset
  return GetSysResetRequired ();
} // CheckStatus

/**
  This function sends data to slave processors

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
SendStatus (
  VOID
  )
{
  UINT8   socket;
  UINT32   ResetRequired;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  ResetRequired = GetSysResetRequired ();
  // Let the AP's know if a reset is needed
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (socket)) {
      continue;
    }

    if (socket == GetSysSocketId ()) {
      continue;
    }

    //
    // Send the status variable to each socket
    //
    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CLOCK_INIT, CHECKPOINT_MINOR_SEND_STATUS, socket));
    SendPipePackage (Host, socket, (UINT8*)&ResetRequired, sizeof (UINT32), RCSIM_NO_DELAY);

  } // socket loop

  // Return requesting a reset
  return ResetRequired;
} // SendStatus

/**
  This function gets data from slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
GetStatus (
  PSYSHOST                  Host
  )
{
  UINT32   ResetRequired;

  ResetRequired = GetSysResetRequired ();
  GetPipePackage (GetSysSocketId (), (UINT8*)&ResetRequired, sizeof (UINT32));
  return ResetRequired;

} // GetStatus

/**
  This function gets bootmode to slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
GetSocketbootmode (
  PSYSHOST  Host
  )
{
  UINT8   socket;

  // Let the AP's know if a reset is needed
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (socket)) {
      continue;
    }

    if (socket == GetSysSocketId ()) {
      continue;
    }

    //
    // Get the bootmode of slave processors
    //
    GetPipePackage (socket, (UINT8*)&Host->var.mem.socket[socket].socketSubBootMode, sizeof (UINT8));

  } // socket loop

  return SUCCESS;
} // GetSocketbootmode

/**
  This function sends bootmode from slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
SendSocketbootmode (
  PSYSHOST                  Host
  )
{
  UINT8 SocketId;

  SocketId = GetSysSocketId ();
  SendPipePackage (Host, SocketId, (UINT8*)&Host->var.mem.socket[SocketId].socketSubBootMode, sizeof (UINT8), RCSIM_NO_DELAY);
  return SUCCESS;
} // SendSocketbootmode

/**
This function Gets bootmode to slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
Getbootmode (
  PSYSHOST                  Host
  )
{
  UINT8 SocketId;

  SocketId = GetSysSocketId ();
  GetPipePackage (SocketId, (UINT8*)&Host->var.mem.subBootMode, sizeof (UINT32));
  Host->var.mem.socket[SocketId].socketSubBootMode = Host->var.mem.subBootMode;
  return SUCCESS;
} // Getbootmode

/**
This function Sends bootmode from slave processors

@param host:         Pointer to sysHost

@retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
Sendbootmode (
  PSYSHOST                  Host
  )
{
  UINT8   socket;
  UINT32  socketPresentBitMap;

  // Let the AP's know if a reset is needed
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    socketPresentBitMap = GetSocketPresentBitMap ();
    if ((socketPresentBitMap & (BIT0 << socket)) == 0) {
      continue;
    }

    if (socket == GetSysSocketId ()) {
      continue;
    }

    SendPipePackage (Host, socket, (UINT8*)&Host->var.mem.subBootMode, sizeof (UINT32), RCSIM_NO_DELAY);
    Host->var.mem.socket[socket].socketSubBootMode = Host->var.mem.subBootMode;
  }
  return SUCCESS;
} // Sendbootmode
/**
  This function gets mem.var, mem.nvram common and mem.nvram from slave processors.
  BSP's Host for these structs will be upto date wrt to APs.

  @param Host:         Pointer to sysHost

  @retval SUCCESS
**/
UINT32
GetSlaveSsaData (
  PSYSHOST                  Host
  )
{
  UINT8   socket;
  UINT16  sbspSktId;

  sbspSktId = GetSysSbspSocketIdNv ();

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (socket)) {
      continue;
    }

    if (socket == GetSysSocketId ()) {
      continue;
    }

    RcDebugPrint (SDBG_MAX,
                  "GetSlaveSsaData S%d -> S%d\n", socket, sbspSktId);

    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DIMM_DETECT, CHECKPOINT_MINOR_GET_SLAVE_DATA, (UINT16) ((socket << 8) |DATA_TYPE_VAR)));

    GetReadyToTransferData (Host, socket);

    GetMemVarData (Host, socket);

    GetMemNvramCommonData (Host, socket);

    GetMemNvramData (Host, socket);

  } // socket loop

  return SUCCESS;
} // GetSlaveSsaData

/**
  This function sends mem.var, mem.nvram common and mem.nvram from slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS
**/
UINT32
SendSlaveSsaData (
  PSYSHOST                  Host
  )
{
  UINT16  sbspSktId;
  UINT8   SocketId;

  sbspSktId = GetSysSbspSocketIdNv ();
  SocketId = GetSysSocketId ();

  RcDebugPrint (SDBG_MAX,
                "SendSlaveSsaData S%d -> S%d\n", GetSysSocketId (), sbspSktId);

  SendReadyToTransferData (Host, SocketId);

  SendMemVarData (Host, SocketId);

  SendMemNvramCommonData (Host, SocketId);

  SendMemNvramData (Host, SocketId);

  return SUCCESS;
} // SendSlaveSsaData

/**
  This function sends data to slave processors to sync up SSA. To inform APs that BSP is done with SSA.

  @param Host:         Pointer to sysHost

  @retval SUCCESS
**/
UINT32
SendSsaData (
  PSYSHOST  Host
  )
{
  UINT8   socket;

  //This function sends data to the slave processors to sync up SSA. To inform APs that the BSP is done with SSA.
  //Also to prevent a race condition where the APs have completed through their call table while the BSP is still executing SSA.

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (socket)) {
      continue;
    }

    if (socket == GetSysSocketId ()) {
      continue;
    }

    //
    // Send a variable to each socket
    //
    SendPipePackage (Host, socket, (UINT8 *)&Host->var.common.RcVersion, sizeof (RC_VERSION), RCSIM_NO_DELAY);

  } // socket loop

  return SUCCESS;
} // SendSsaData

/**
  This function gets data from master processor to sync up SSA.
  To halt the APs until BDAT/BSSA are completed on the BSP.

  @param Host:         Pointer to sysHost

  @retval SUCCESS:
**/
UINT32
GetSsaData (
  PSYSHOST                  Host
  )
{
  //In this function; the APs get data from the BSP. To inform APs that the BSP is done with SSA.
  //Also to prevent a race condition where the APs have completed through their call table while the BSP is still executing SSA.

  GetPipePackage (GetSysSocketId (), (UINT8 *)&Host->var.common.RcVersion, sizeof (RC_VERSION));
  return SUCCESS;

} // GetSsaData

/**
  This function sends data to slave processors

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
SendData (
  VOID
  )
{
  UINT8         socket;
  COMMAND_PIPE  cmdPipe;
  UINT8         SocketId;
  PSYSHOST      Host;

  cmdPipe.RawPipe = 0;
  SocketId = GetSysSocketId ();
  Host = GetSysHostPointer ();

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (socket)) {
      continue;
    }

    if (socket == SocketId) {
      continue;
    }

    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_RANK_DETECT, CHECKPOINT_MINOR_SEND_DATA, (UINT16) ((socket << 8) |DATA_TYPE_VAR)));
    RcDebugPrint (SDBG_MAX,
                  "SendData S%d -> S%d\n", SocketId, socket);

    SendReadyToTransferData (Host, socket);

    SendMemVarData (Host, socket);

    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_RANK_DETECT, CHECKPOINT_MINOR_SEND_DATA, (UINT16) ((socket << 8) |DATA_TYPE_NVRAM_COMMON)));
    SendMemNvramCommonData (Host, socket);

    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_RANK_DETECT, CHECKPOINT_MINOR_SEND_DATA, (UINT16) ((socket << 8) |DATA_TYPE_NVRAM_DATA)));
    SendMemNvramData (Host, socket);

    //
    // Continue parallel DDR Init. Send PIPE_RUN_MEMORY_COMMAND to each socket
    //
    cmdPipe.Pipe.InternalCmd = PIPE_RUN_MEMORY_COMMAND;

    cmdPipe.Pipe.CmdRequest = PIPE_REQUEST;
    cmdPipe.Pipe.TargetId = socket;

    SendCommandPipe (Host, socket, &cmdPipe.RawPipe);
  } // socket loop

  return SUCCESS;
} // SendData

/**
  This function gets data from slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
GetData (
  PSYSHOST                  Host
  )
{
  COMMAND_PIPE  cmdPipe;
  UINT8 sbspSktId;
  UINT8 SocketId;

  sbspSktId = GetSysSbspSocketIdNv ();
  SocketId = GetSysSocketId ();

  cmdPipe.RawPipe = 0;

  RcDebugPrint (SDBG_MAX,
                "GetData S%d -> S%d\n", sbspSktId, SocketId);

  GetReadyToTransferData (Host, SocketId);

  GetMemVarData (Host, SocketId);
  GetMemNvramCommonData (Host, SocketId);
  GetMemNvramData (Host, SocketId);

  //
  // Waits for PIPE Master's PIPE_RUN_MEMORY_COMMAND to run memory init
  //
  WaitForPipeCommand (Host, SocketId, RUN_MEMORY_COMMAND);

  //
  // Respond ACK
  //
  cmdPipe.Pipe.Status     = PIPE_SUCCESS;
  cmdPipe.Pipe.NumOfBytes = 0;
  cmdPipe.Pipe.Width_     = 0;
  cmdPipe.Pipe.CmdRequest = PIPE_ACK;
  WriteCommandPipe (Host, SocketId, &cmdPipe.RawPipe);

  return SUCCESS;
} // GetData

/**
  This function gets data from slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS
**/
UINT32
GetNvramData (
  PSYSHOST                  Host
  )
{
  UINT8 socket;
  UINT8 sbspSktId;

  sbspSktId = GetSysSbspSocketIdNv ();

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (socket)) {
      continue;
    }

    if (socket == GetSysSocketId ()) {
      continue;
    }

    RcDebugPrint (SDBG_MAX,
                  "GetNvramData S%d -> S%d\n", socket, sbspSktId);

    GetReadyToTransferData (Host, socket);

    GetMemVarData (Host, socket);
    GetMemNvramCommonData (Host, socket);
  } // socket loop

  return SUCCESS;
} // GetNvramData

/**
  This function sends data from slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS
**/
UINT32
SendNvramData (
  PSYSHOST                  Host
  )
{
  UINT8 sbspSktId;
  UINT8 SocketId;

  sbspSktId = GetSysSbspSocketIdNv ();
  SocketId = GetSysSocketId ();

  RcDebugPrint (SDBG_MAX,
                "SendNvramData S%d -> S%d\n", SocketId, sbspSktId);

  SendReadyToTransferData (Host, SocketId);

  SendMemVarData (Host, SocketId);
  SendMemNvramCommonData (Host, SocketId);

  return SUCCESS;
} // SendNvramData

/**
  This function sends data from Master

  @param Host:         Pointer to sysHost

  @retval SUCCESS
**/
UINT32
SendVarData (
  PSYSHOST                  Host
  )
{
  UINT8 socket;
  UINT8 sbspSktId;

  sbspSktId = GetSysSbspSocketIdNv ();

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (socket)) {
      continue;
    }

    if (socket == GetSysSocketId ()) {
      continue;
    }

    RcDebugPrint (SDBG_MAX,
                  "SendVarData S%d -> S%d\n", socket, sbspSktId);

    SendReadyToTransferData (Host, socket);

    SendMemVarData (Host, socket);
  } // socket loop

  return SUCCESS;
} // SendVarData

/**
This function Gets data from Master

@param Host:         Pointer to sysHost

@retval SUCCESS
**/
UINT32
GetVarData (
  PSYSHOST                  Host
  )
{
  UINT8 sbspSktId;
  UINT8 SocketId;

  sbspSktId = GetSysSbspSocketIdNv ();
  SocketId = GetSysSocketId ();

  RcDebugPrint (SDBG_MAX,
                "SendNvramData S%d -> S%d\n", SocketId, sbspSktId);

  GetReadyToTransferData (Host, SocketId);

  GetMemVarData (Host, SocketId);

  return SUCCESS;
} // GetVarData

/**
  Checks the boot mode flag to determine whether a function should be executed or not

  @param Host:         Pointer to sysHost
  @param policyFlag

  @retval SUCCESS:  It's allowed for a function to be executed under current boot mode.
  @retval FAILURE:  It's not allowed for a function to be executed under current boot mode.
**/
UINT32
CheckPF (
  PSYSHOST  Host,
  UINT8     policyFlag
  )
{
  BootMode  SysBootMode;

  SysBootMode = GetSysBootMode ();

  if  (((SysBootMode == S3Resume) && (policyFlag & MRC_PF_S3))
       || (((Host->var.mem.subBootMode == WarmBootFast) || (Host->var.mem.subBootMode == ColdBootFast)) && (policyFlag & MRC_PF_FAST))
       || (Host->var.mem.subBootMode == NvDimmResume)
       || ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot) && (policyFlag & MRC_PF_COLD))) {
    return SUCCESS;
  } else {
    return FAILURE;
  }
}

/**
  Checks the boot policy to determine whether a function should be executed or not
  In parallel mode, it checks whether a function should be executed on SBSP only or not
  In serial mode, it checks whether a function should be executed in loop or not

  @param Host:         Pointer to sysHost
  @param policyFlag

  @retval SUCCESS:  It's allowed for a function to be executed under current boot policy.
  @retval FAILURE:  It's not allowed for a function to be executed under current boot policy.
**/
UINT32
CheckMPFlag (
  PSYSHOST  Host,
  UINT8     MPFlag
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if ((Setup->mem.options & MULTI_THREAD_MRC_EN)) { // parallel mode
    if (MPFlag & MRC_MP_PARALLEL) {
      return SUCCESS;
    } else if ((MPFlag & MRC_MP_PARALLEL_SBSP) &&
               (GetSysSocketId () == GetSysSbspSocketIdNv ())) {
      return SUCCESS;
    } else {
      return FAILURE;
    }
  } else { // serial mode
    if ((MPFlag & MRC_MP_SERIAL) ||
        (MPFlag & MRC_MP_SERIAL_LOOP)) {
      return SUCCESS;
    } else {
      return FAILURE;
    }
  }
}

/**
  Master Sends Memory Mode to slaves

  @param Host:         Pointer to sysHost

  @retval SUCCESS
**/
UINT32
SendMemoryMode (
  PSYSHOST host
  )
{
  UINT8           socket;
  UINT32          socketPresentBitMap;
  MEMORY_MAP_HOST *MemMapHost;

  MemMapHost = GetMemMapHostPointer ();

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    socketPresentBitMap = GetSocketPresentBitMap ();
    if ((socketPresentBitMap & (BIT0 << socket)) == 0) {
      continue;
    }

    if (socket == GetSysSocketId ()) {
      continue;
    }

    SendPipePackage (host, socket, (UINT8*)&host->var.mem.volMemMode, sizeof (UINT8), RCSIM_NO_DELAY);
    SendPipePackage (host, socket, (UINT8*)&host->var.mem.CacheMemType, sizeof (UINT8), RCSIM_NO_DELAY);
    SendPipePackage (host, socket, (UINT8*)MemMapHost, sizeof(MEMORY_MAP_HOST), RCSIM_NO_DELAY);
  }
  return SUCCESS;
}

/**
  Slaves receive Memory Mode from Master

  @param Host:         Pointer to sysHost

  @retval SUCCESS
**/
UINT32
GetMemoryMode (
  PSYSHOST host
  )
{
  MEMORY_MAP_HOST *MemMapHost;

  //
  // Allocate a buffer in this Socket's memory to store a copy of the
  // MemMapHost data.
  //
  MemMapHost = RcAllocatePool (sizeof (MEMORY_MAP_HOST));
  ASSERT (MemMapHost != NULL);

  if (MemMapHost != NULL) {
    SetMemMapHostPointer (MemMapHost);
    GetPipePackage (GetSysSocketId (), (UINT8*)&host->var.mem.volMemMode, sizeof (UINT8));
    GetPipePackage (GetSysSocketId (), (UINT8*)&host->var.mem.CacheMemType, sizeof (UINT8));
    GetPipePackage (GetSysSocketId (), (UINT8*)MemMapHost, sizeof(MEMORY_MAP_HOST));
  }

  return SUCCESS;
}

/**
  Get PreviousBootError data from  Host->var.mem.previousBootError

  @param PreviousBootErrorValue:         Pointer to data buffer to return previousBootError data

  @retval SUCCESS
**/
UINT32
EFIAPI
GetPreviousBootError (
  OUT UINT8 *PreviousBootErrorValue
  )
{
  PSYSHOST Host;
  Host = GetSysHostPointer ();

  if (PreviousBootErrorValue != NULL) {
    *PreviousBootErrorValue = Host->var.mem.previousBootError;
  }

  return SUCCESS;
}

/**
  Save PreviousBootError data to  Host->var.mem.previousBootError

  @param PreviousBootErrorValue:         value of PreviousBootError would be setting to

  @retval SUCCESS
**/
UINT32
EFIAPI
SetPreviousBootError (
  IN UINT8 PreviousBootErrorValue
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  Host->var.mem.previousBootError = PreviousBootErrorValue;

  return SUCCESS;
}

/**
  Get ImcEnabled data from Host->var.mem.socket[socket].imcEnabled[mcId]

  @param socket:         value of the socket number
  @param mcId:           value of the iMC ID
  @ImcEnabledData:       Pointer to data buffter to return corresponding imcEnabled value with socket and mcId number

  @retval SUCCESS
**/
UINT32
EFIAPI
GetImcEnabledData (
  IN UINT8 socket,
  IN UINT8 mcId,
  OUT UINT8 *ImcEnabledData
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  if (ImcEnabledData != NULL) {
    *ImcEnabledData = Host->var.mem.socket[socket].imcEnabled[mcId];
  }

  return SUCCESS;
}

/**
  Set ImcEnabled data to Host->var.mem.socket[socket].imcEnabled[mcId]

  @param socket:         value of the socket number
  @param mcId:           value of the iMC ID
  @ImcEnabledData:       data to be set to corresponding imcEnabled with socket and mcId number

  @retval SUCCESS
**/
UINT32
EFIAPI
SetImcEnabledData (
  IN UINT8 socket,
  IN UINT8 mcId,
  IN UINT8 ImcEnabledData
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  Host->var.mem.socket[socket].imcEnabled[mcId] = ImcEnabledData;

  return SUCCESS;
}

/**
  Get Memory controller number data from Host->var.mem.socket[socket].channelList[channel].mcId

  @param socket:         value of the socket number
  @param channel:        value of the channel number
  @iMcIdData:            pointer to iMcIdData buffer to get Memory controller number data (mcId) from sysHost with socket and channel number

  @retval SUCCESS
**/
UINT32
EFIAPI
GetMcIdData (
  UINT8 socket,
  UINT8 channel,
  UINT8 *iMcIdData
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  if (iMcIdData != NULL) {
    *iMcIdData = Host->var.mem.socket[socket].channelList[channel].mcId;
  }

  return SUCCESS;
}


/**
  Set Memory controller number data to Host->var.mem.socket[socket].channelList[channel].mcId

  @param socket:         value of the socket number
  @param channel:        value of the channel number
  @iMcIdData:            Memory controller number data to be set to corresponding mcId  with socket and channel number

  @retval SUCCESS
**/
UINT32
EFIAPI
SetMcIdData (
  UINT8 socket,
  UINT8 channel,
  UINT8 iMcIdData
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  Host->var.mem.socket[socket].channelList[channel].mcId = iMcIdData;

  return SUCCESS;
}

/**
  Get DdrFreqCheckFlag data from Host->var.mem.socket[socket].ddrFreqCheckFlag

  @param socket:            value of the socket number
  @ddrFreqCheckFlagData:    pointer to buffer to get ddrFreqCheckFlagData from Host->var.mem.socket[socket].ddrFreqCheckFlag

  @retval SUCCESS
**/
UINT32
EFIAPI
GetDdrFreqCheckFlagData (
  UINT8 socket,
  UINT8 *ddrFreqCheckFlagData
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  if (ddrFreqCheckFlagData != NULL) {
    *ddrFreqCheckFlagData = Host->var.mem.socket[socket].ddrFreqCheckFlag;
  }

  return SUCCESS;
}

/**
  Set DdrFreqCheckFlag data to Host->var.mem.socket[socket].ddrFreqCheckFlag

  @param socket:         value of the socket number
  @ddrFreqCheckFlagData: ddrFreqCheckFlag data to be set to corresponding ddrFreqCheckFlag  with socket  number in sysHost

  @retval SUCCESS
**/
UINT32
EFIAPI
SetDdrFreqCheckFlagData (
  UINT8 socket,
  UINT8 ddrFreqCheckFlagData
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  Host->var.mem.socket[socket].ddrFreqCheckFlag = ddrFreqCheckFlagData;

  return SUCCESS;
}

/**
  Get DDR QCLK ratio from Host->var.mem.socket[socket].ddrClkData

  @param Socket            Value of the socket number
  @param DdrClkData        Pointer to buffer to get DdrClkData from Host->var.mem.socket[socket].ddrClkData,
                           which is QCLK ratio.

  @retval SUCCESS
**/
UINT32
EFIAPI
GetDdrClkData (
  UINT8 socket,
  UINT8 *DdrClkData
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  if (DdrClkData != NULL) {
    *DdrClkData = Host->var.mem.socket[socket].ddrClkData;
  }

  return SUCCESS;
}

/**
  Set DdrClk data to Host->var.mem.socket[socket].ddrClkData, which is QCLK ratio.

  @param socket:         value of the socket number
  @DdrClkData:           DdrClk data to be set to corresponding DdrClkData  with socket  number in sysHost

  @retval SUCCESS
**/
UINT32
EFIAPI
SetDdrClkData (
  UINT8 socket,
  UINT8 DdrClkData
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  Host->var.mem.socket[socket].ddrClkData = DdrClkData;

  return SUCCESS;
}

/**
  Get DdrClkType data from Host->var.mem.socket[socket].ddrClkType

  @param socket:            value of the socket number
  @DdrClkType:              pointer to buffer to get DdrClkType from Host->var.mem.socket[socket].DdrClkType

  @retval SUCCESS
**/
UINT32
EFIAPI
GetDdrClkType (
  UINT8 socket,
  UINT8 *DdrClkType
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  if (DdrClkType != NULL) {
    *DdrClkType = Host->var.mem.socket[socket].ddrClkType;
  }

  return SUCCESS;
}

/**
  Set DdrClkType data to Host->var.mem.socket[socket].ddrClkType

  @param socket:         value of the socket number
  @DdrClkType:           DdrClkType data to be set to corresponding DdrClkType  with socket  number in sysHost

  @retval SUCCESS
**/
UINT32
EFIAPI
SetDdrClkType (
  UINT8 socket,
  UINT8 DdrClkType
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  Host->var.mem.socket[socket].ddrClkType = DdrClkType;

  return SUCCESS;
}

/**
  Get MaxFreq data from Host->var.mem.socket[socket].maxFreq

  @param socket:            value of the socket number
  @MaxFreqData:             pointer to buffer to get MaxFreqData from Host->var.mem.socket[socket].maxFreq

  @retval SUCCESS
**/
UINT32
EFIAPI
GetMaxFreqData (
  UINT8 socket,
  UINT8 *MaxFreqData
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();
  if (MaxFreqData != NULL) {
    *MaxFreqData = Host->var.mem.socket[socket].maxFreq;
  }
  return SUCCESS;
}

/**
  Set MaxFreq data to Host->var.mem.socket[socket].maxFreq

  @param socket:         value of the socket number
  @MaxFreqData:          MaxFreq data to be set to corresponding MaxFreqData  with socket  number in sysHost

  @retval SUCCESS
**/
UINT32
EFIAPI
SetMaxFreqData (
  UINT8 socket,
  UINT8 MaxFreqData
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  Host->var.mem.socket[socket].maxFreq = MaxFreqData;

  return SUCCESS;
}
