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

#include <Library/BaseMemoryLib.h>
#include <Library/CpuAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/KtiApi.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/SimulationThreadLib.h>
#include <Library/SimulationServicesLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/CheckpointLib.h>

#include <Guid/MrcPlatformHooksGuid.h>

#include "Include/Pipe.h"


/**
 * If RC_SIM is defined, sleep this thread for
 * Duration milliseconds. If RC_SIM is not defined,
 * do nothing.
 *
 * @param[in]  Duration    Time to sleep this thread in millisecond.
 *
 * @retval None
 *
 **/
VOID
RcSimSleep (
  IN UINT32 Duration
  )
{

  return;

} // RcSimSleep

/**
 * Get CPU socket id for the current processor
 *
 * @param Host  - Pointer to the system Host (root) structure
 *
 * @retval Current processor's Socket ID
 *
 **/
UINT8
GetLocalSktId (
  PSYSHOST Host
  )
{
  return GetSysSocketId ();
} // GetLocalSktId

/**
 * Dispatcher function for all non-SBSPs
 *
 * @param Host  - Pointer to the system Host (root) structure
 *
 * @retval N/A
 *
 **/
VOID
PipeDispatcher (
               PSYSHOST Host
               )
{
  UINT32 Command = PIPE_DISPATCH_NULL;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  RcDebugPrint (SDBG_MINMAX, "N%d Checked into Pipe\n", GetSysSocketId ());

  while (Command != PIPE_DISPATCH_EXIT) {
    switch (Command) {

    case PIPE_DISPATCH_MEMORY_INIT:
      //
      // Reset the command reg to null
      //
      WriteDispatchPipeCsr (GetLocalSktId(Host), PIPE_DISPATCH_NULL);

      RcDebugPrint (SDBG_MINMAX, "N%d Entering MRC\n", GetSysSocketId ());

      MemStart(Host);
      break;

    case PIPE_RD_MSR:

      //
      // Reset the command reg to null
      //
      WriteDispatchPipeCsr (GetLocalSktId(Host), PIPE_DISPATCH_NULL);

      PipeRdMsr(Host);

      break;

    case PIPE_WR_MSR:

      //
      // Reset the command reg to null
      //
      WriteDispatchPipeCsr (GetLocalSktId(Host), PIPE_DISPATCH_NULL);

      PipeWrMsr(Host);

      break;

    case PIPE_DISPATCH_EXECUTE_CPUID:

      //
      // Reset the command reg to null
      //
      WriteDispatchPipeCsr (GetLocalSktId(Host), PIPE_DISPATCH_NULL);

      CpuidNbspSide (Host);

      break;

    default:
      //
      // Do nothing
      //
      break;
    }

    Command = GetDispatchPipeCsr (GetLocalSktId(Host));
  }

  return;
} // PipeDispatcher

/**
 * Take control of the Command Pipe
 *
 * @param Host  - Pointer to the system Host (root) structure
 * @param Id    - PIPE Slave Id
 *
 * @retval TRUE - Command taken
 * @retval FALSE - Command not given
 *
 **/
BOOLEAN
AcquireCommandPipe (
                   PSYSHOST Host,
                   UINT8    Id
                   )
{
  UINT8 SbspSktId;

  SbspSktId = GetSysSbspSocketIdNv ();

  /*
  if (SbspSktId == SKT_INVALID_ID) {
      //
      //OemDebugPrintM(FBD_D_INFO, "MRC - AcquireCommandPipe: FATAL ERROR - Failure to obtain SBSP ID\n");
      //OemDebugPrintM(FBD_D_INFO, "MRC - Check CSI reference code for SR_CPU_SOCKET_INFO_OFFSET at UBOX_CSR_SR64?\n");
      //
      // Can not run MRC without SBSP. Hang here! Need to check to ensure CSI reference
      // still assigns SR_CPU_SOCKET_INFO_OFFSET at UBOX_CSR_SR64?
      //
      //OemAssertion(PIPE_NO_SBSP_FOUND);
      //
  }
   */

  return(Id == SbspSktId) ? TRUE : FALSE;
} // AcquireCommandPipe

/**
 * Stay in loop listenning to the Command PIPE until a command is received. This
 * function should be called only by a PIPE Slave.
 *
 * @param[in] Host       - Pointer to the system Host (root) structure
 * @param[in] Id         - PIPE Slave Id
 * @param[in] CmdPipe    - Pointer to a command PIPE data structure
 * @param[in] RcSimDelay - Delay between read attempts, only used in RcSim
 *
 * @retval N/A
 *
 **/
VOID
ListenToCommandPipe (
  PSYSHOST Host,
  UINT8    Id,
  UINT32   *CmdPipe,
  UINT32   RcSimDelay
  )
{
  COMMAND_PIPE CommandPipe;

  while (TRUE) {
    CommandPipe.RawPipe = UsraCsrRead (Id, 0, SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_ADDR(Host));
    if (CommandPipe.Pipe.CmdRequest == PIPE_REQUEST &&  CommandPipe.Pipe.TargetId == Id) {
      break;
    }  /* Yes, it's me */

    //
    // RcSimSleep calls do nothing
    // on non-RcSim builds.
    //

    RcSimSleep (RcSimDelay);

  }
  *CmdPipe = CommandPipe.RawPipe;
} // ListenToCommandPipe

/**
 * Unlocks the command pipe (releases control)
 *
 * @param Host  - Pointer to the system Host (root) structure
 * @param Id      - PIPE Slave Id
 *
 * @retval N/A
 *
 **/
VOID
UnlockCommandPipe (
                  PSYSHOST Host,
                  UINT8    Id
                  )
{
  UsraCsrWrite ((UINT8)Id, 0, SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_ADDR(Host), COMMAND_PIPE_UNLOCK(Id));
} // UnlockCommandPipe

/**
 * Lock a specific slave's command pipe
 *
 * @param Host  - Pointer to the system Host (root) structure
 * @param Id      - PIPE Slave Id
 *
 * @retval N/A
 *
 **/
VOID
LockCommandPipe (
                PSYSHOST Host,
                UINT8    Id
                )
{
  UsraCsrWrite (Id, 0, SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_ADDR(Host), COMMAND_PIPE_LOCK(Id));
} // LockCommandPipe

/**
 * Wait for a specific slave's command pipe to be unlocked
 *
 * @param Host    - Pointer to the system Host (root) structure
 * @param Id      - PIPE Slave Id
 *
 * @retval N/A
 *
 **/
VOID
WaitForCommandPipeUnLocked (
                           PSYSHOST Host,
                           UINT8    Id
                           )
{
  UINT32 CmdPipe;
  UINT32 Key = COMMAND_PIPE_UNLOCK(Id);

  //
  // Setup CSR SR Register
  //
  while (TRUE) {
    CmdPipe = UsraCsrRead (Id, 0, SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_ADDR(Host));
    if (CmdPipe == Key) {
      break;
    }
  }
} // WaitForCommandPipeUnLocked

/**
 * Wait for a specific slave processor's pipe's command
 *
 * @param Host    - Pointer to the system Host (root) structure
 * @param Id      - PIPE Slave Id
 * @param CmdPipe - Pointer to a command PIPE data structure
 *
 * @retval Command that arrived in the slave processor's pipe
 *
 **/
UINT32
WaitForPipeCommand (
                   PSYSHOST Host,
                   UINT8    Id,
                   UINT32   CmdType
                   )
{
  COMMAND_PIPE CmdPipe;

  while (TRUE) {
    ListenToCommandPipe (Host, Id, &CmdPipe.RawPipe, RCSIM_SMALL_DELAY);
    if (CmdType & (1 << CmdPipe.Pipe.InternalCmd)) {
      return(UINT32)(1 << CmdPipe.Pipe.InternalCmd);
    }

    CmdPipe.Pipe.Status = PIPE_INVALID;
    CmdPipe.Pipe.NumOfBytes = 0;
    CmdPipe.Pipe.Width_ = 0;
    CmdPipe.Pipe.CmdRequest = PIPE_ACK;
    WriteCommandPipe(Host, Id, &CmdPipe.RawPipe);
  }
} // WaitForPipeCommand

/**
 * Writes a command to a specific slave's pipe
 *
 * @param Host    - Pointer to the system Host (root) structure
 * @param Id      - PIPE Slave Id
 * @param CmdPipe - Pointer to a command PIPE data structure
 *
 * @retval N/A
 *
 **/
VOID
WriteCommandPipe (
                 PSYSHOST Host,
                 UINT8    Id,
                 UINT32   *CmdPipe
                 )
{
  UsraCsrWrite (Id, 0, SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_ADDR(Host), *CmdPipe);
} // WriteCommandPipe

/**
 * Read data pipe of a specific slave processor
 *
 * @param Host  - Pointer to the system Host (root) structure
 * @param Id    - PIPE Slave Id
 * @param Data  - Pointer to a command PIPE data variable
 *
 * @retval N/A
 *
 **/
VOID
ReadDataPipe (
             PSYSHOST Host,
             UINT8    Id,
             UINT32   *Data
             )
{
  *Data = UsraCsrRead (Id, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host));
} // ReadDataPipe

/**
 * Write data pipe of a specific slave processor
 *
 * @param Host  - Pointer to the system Host (root) structure
 * @param Id    - PIPE Slave Id
 * @param Data - Data to write to pipe
 *
 * @retval N/A
 *
 **/
VOID
WriteDataPipe (
              PSYSHOST Host,
              UINT8    Id,
              UINT32   Data
              )
{
  UsraCsrWrite (Id, 0, SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR_ADDR(Host), Data);
} // WriteDataPipe

/**
 * Send a command to the Command PIPE until an ACK is received. This
 * function should be called only by a PIPE Master.
 *
 * @param Host    - Pointer to the system Host (root) structure
 * @param Id      - PIPE Slave Id
 * @param CmdPipe - Pointer to a command PIPE data structure
 *
 * @retval PIPE Status
 *
 **/
UINT32
SendCommandPipe (
                PSYSHOST Host,
                UINT8    Id,
                UINT32   *CmdPipe
                )
{
  PIPE            *Pipe;

  WriteCommandPipe(Host, Id, CmdPipe);

  //
  // Setup CSR SR Register
  //
  while (TRUE) {
    *CmdPipe = UsraCsrRead (Id, 0, SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR_ADDR(Host));
    Pipe = (PIPE *)CmdPipe;
    if (Pipe->CmdRequest == PIPE_ACK) {
      break;
    }
  }

  return Pipe->Status;
} // SendCommandPipe

/**
 * Provide PIPE communication for receiveing package data. Upon this call, this function
 * will enter PIPE package-received mode, as a PIPE master.
 * Calling this function when a PIPE master is ready to receive package from a PIPE slave.
 *
 * Note: Caller should send Query Command to determine the size of HOB before calling this
 * function.
 *
 * @param Id      - PIPE Slave Id
 * @param pBuffer - Pointer to a buffer full with package data being received
 * @param Size    - Size of HOB needed to receive
 *
 * @retval CmdPipe.Pipe.Status of last action
 *
 **/
UINT32
GetPipePackage (
               UINT8    Id,
               UINT8    *pBuffer,
               UINT32   Size
               )
{
  COMMAND_PIPE CmdPipe;
  UINT32 Status = PIPE_SUCCESS;
  UINT32 Data;
  UINT32 NumOfBytes, ByteRemained;
  UINT8 *pBuf = pBuffer;
  UINT8 *pData = (UINT8 *)&Data;
  UINT8 i;
  PSYSHOST Host;

  Host = GetSysHostPointer();
  //
  // Send Read Package Request
  //
  NumOfBytes = Size;
  if (NumOfBytes) {
    CmdPipe.RawPipe = 0;
    CmdPipe.Pipe.Toggle = 0;        // Setup toggle bit
    CmdPipe.Pipe.Begin = 1;         // Signal the begin of data transfer
    CmdPipe.Pipe.TargetId = Id;
    CmdPipe.Pipe.InternalCmd = PIPE_READ_COMMAND;
    ByteRemained = NumOfBytes;
    while (ByteRemained > 0) {
      if (ByteRemained <= sizeof(UINT32)) {
        CmdPipe.Pipe.End_ = 1;
      }   // Signal the end of data transfer

      CmdPipe.Pipe.CmdRequest = PIPE_REQUEST;
      if ((Status = SendCommandPipe(Host, Id, &CmdPipe.RawPipe)) != PIPE_SUCCESS) {
        return Status;
      }

      ByteRemained = (UINT32)CmdPipe.Pipe.NumOfBytes;
      NumOfBytes -= (CmdPipe.Pipe.Width_ + 1);

      ReadDataPipe (Host, Id, &Data);

      for (i = 0; i <= CmdPipe.Pipe.Width_; i++, pBuf++) {
        *pBuf = *(pData + i);
      }

      CmdPipe.Pipe.Toggle = (CmdPipe.Pipe.Toggle == 1) ? 0 : 1;
      CmdPipe.Pipe.Begin = 0;     // Clear begin indicator bit
    }

    if (NumOfBytes) {
      // Double check! Expect this to be zero
      Status = PIPE_INCOMPLETE_ERROR;
    }
  }

  while (UsraCsrRead(Id, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host)) != PIPE_DISPATCH_FREEZE) {

    //
    // RcSimSleep call does nothing on
    // non-RcSim builds
    //

    RcSimSleep (RCSIM_SMALL_DELAY);

  }
  UsraCsrWrite (Id, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), PIPE_DISPATCH_NULL);

  return Status;
} // GetPipePackage

/**
 * Provide PIPE communication for transferring package data. Upon this call, this function
 * will enter PIPE package-transfered mode, as a PIPE slave, until a CmdPipe.Pipe.End
 * is sent by a PIPE master.
 * Calling this function when a PIPE slave is ready to enter PIPE package-transfered
 * mode (PIPE Read Command is received)
 *
 * @param[in] Host       - Pointer to the system Host (root) structure
 * @param[in] Id         - PIPE Slave Id
 * @param[in] pBuffer    - Pointer to a buffer full with package data
 * @param[in] Size       - Size of the package for transfer
 * @param[in] RcSimDelay - Delay for RcSim threads. Does nothing on non-RcSim builds
 *
 * @retval CmdPipe.Pipe.Status of last action.
 *
 **/
UINT32
SendPipePackage (
  IN PSYSHOST Host,
  IN UINT8    Id,
  IN UINT8    *pBuffer,
  IN UINT32   Size,
  IN UINT32   RcSimDelay
  )
{
  COMMAND_PIPE CmdPipe;
  UINT32 Data;
  UINT32 ByteRemained;
  UINT8 *pBuf = pBuffer;
  UINT8 *pData = (UINT8 *)&Data;
  UINT8 Toggle = 0;
  BOOLEAN Done = FALSE;
  BOOLEAN Start = FALSE;

  CmdPipe.RawPipe = 0;
  ByteRemained = Size;
  pBuf = pBuffer;
  while (!Done) {

    ListenToCommandPipe (Host, Id, &CmdPipe.RawPipe, RcSimDelay);

    switch (CmdPipe.Pipe.InternalCmd) {
    case PIPE_READ_COMMAND:
      if (CmdPipe.Pipe.Begin) {
        if (Start || ByteRemained != Size) {
          //
          // Detect invalid command, "Begin" bit must not set here.
          // Don't transfer any thing
          //
          CmdPipe.Pipe.Status = PIPE_INVALID;
          CmdPipe.Pipe.NumOfBytes = ByteRemained;
          CmdPipe.Pipe.Width_ = 0;
          break;
        }
        Toggle = (UINT8) CmdPipe.Pipe.Toggle;       // Save Toggle bit
        Start = TRUE;
      } else if (!Start || (UINT8)CmdPipe.Pipe.Toggle == Toggle) {
        //
        // Detect invalid command, "Toggle" bit is invalid.
        // Don't transfer any thing
        //
        CmdPipe.Pipe.Status = (Start == FALSE) ? PIPE_INVALID : PIPE_TOGGLE_ERROR;
        CmdPipe.Pipe.NumOfBytes = ByteRemained;
        CmdPipe.Pipe.Width_ = 0;
        break;
      }

      if (CmdPipe.Pipe.End_) {
        Done = TRUE;
        if (ByteRemained > sizeof(UINT32)) {
          //
          // Detect invalid command. "End" bit must not set here unless "Abort".
          // Don't transfer any thing
          //
          CmdPipe.Pipe.Status = PIPE_INCOMPLETE_ERROR;
          CmdPipe.Pipe.NumOfBytes = ByteRemained;
          CmdPipe.Pipe.Width_ = 0;
          break;
        }
      }

      //
      // At this point, ready to do DATA Ack
      //
      CmdPipe.Pipe.Status = PIPE_SUCCESS;
      if (ByteRemained > sizeof(UINT32)) {
        CmdPipe.Pipe.Width_ = sizeof(UINT32) - 1;    // 0 = 1 byte, 1 = 2 bytes, ...
        ByteRemained -= sizeof(UINT32);             // Update bytes remained
      } else if (ByteRemained > 0) {
        CmdPipe.Pipe.Width_ = ByteRemained - 1;      // No more bytes remained
        ByteRemained = 0;
      } else {
        CmdPipe.Pipe.Status = PIPE_INVALID;         // Detect invalid command
        CmdPipe.Pipe.Width_ = 0;
      }
      CmdPipe.Pipe.NumOfBytes = ByteRemained;
      Toggle = (UINT8) CmdPipe.Pipe.Toggle;           // Save Toggle bit

      if (CmdPipe.Pipe.Status == PIPE_SUCCESS) {
        //
        // Write to Command/Data Pipe
        //
        Data = 0;
        CopyMem (pData, pBuf, (CmdPipe.Pipe.Width_ + 1));
        pBuf += (CmdPipe.Pipe.Width_ + 1);
        WriteDataPipe(Host, Id, Data);
      }
      break;
    default:
      //
      // Detect invalid command. Don't transfer any thing
      //
      CmdPipe.Pipe.Status = PIPE_INVALID;
      CmdPipe.Pipe.NumOfBytes = ByteRemained;
      CmdPipe.Pipe.Width_ = 0;
      break;
    }
    //
    // Write to Command Pipe with ACK set
    //
    CmdPipe.Pipe.CmdRequest = PIPE_ACK;
    WriteCommandPipe(Host, Id, &CmdPipe.RawPipe);
  }

  UsraCsrWrite (Id, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), PIPE_DISPATCH_FREEZE);

  while (UsraCsrRead(Id, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host)) == PIPE_DISPATCH_FREEZE) {

    //
    // RcSimSleep call does nothing on
    // non-RcSim builds
    //

    RcSimSleep (RCSIM_SMALL_DELAY);

  }

  return CmdPipe.Pipe.Status;
} // SendPipePackage

/**
 * Sync data between SBSP and other sockets
 *
 * @param Host      - Pointer to the system Host (root) structure
 *
 * @retval Fatal Error flag
 *
 **/
UINT32
SyncErrorsTbl (
            PSYSHOST Host
            )
{
  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_NORMAL_MODE, (UINT16)((CHECKPOINT_DATA_SYNC << 8)|Host->var.mem.currentSocket)));
  return SyncErrors();
} // SyncErrorsTbl

/**
 * Sync data between SBSP and other sockets
 *
 * @retval Fatal Error flag
 *
 **/
UINT8
SyncErrors (
         VOID
         )
{
  EFI_STATUS Status;
  UINT8   fatalError  = 0;
  UINT8   SocketFatalError = 0;
  UINT8   socket      = 0;
  UINT8   SocketCtr   = 0;
  UINT8   SckId       = 0;
  UINT8   haltOnError;
  UINT32  data;
  UINT32  code;
  UINT8   SbspSocketIdNv;
  PSYSHOST Host;
  EWL_PRIVATE_DATA      *ewlPrivateData;
  EWL_PUBLIC_DATA       *log;
  UINT32                bspBufferOffset;
  UINT32                apBufferIndex;
  UINT32                apBufferSize;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  Host = GetSysHostPointer();
  ewlPrivateData   = &Host->var.common.ewlPrivateData;
  log              = &Host->var.common.ewlPrivateData.status;
  //
  // BSP Gets Status of APs
  //
  SbspSocketIdNv = GetSysSbspSocketIdNv ();

  if (GetSysSocketId () == SbspSocketIdNv) {
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      if (socket == SbspSocketIdNv) {
        continue;
      }
      //
      // Check if socket is valid
      //
      if (!SocketPresent (socket)) {
        continue;
      }
      if (CheckSktId(Host, socket)) {
        continue;
      }

      //
      // Get number of bytes in EWL buffer
      GetPipePackage(socket, (UINT8*)&apBufferSize, sizeof(UINT32));

      // Get the number of bytes omitted due to space restrictions from AP
      GetPipePackage(socket, (UINT8 *)&data, sizeof(UINT32));
      ewlPrivateData->bufSizeOverflow += data;

      bspBufferOffset = log->Header.FreeOffset;
      //if SBSP buffer does not have room for entire AP buffer
      if (sizeof(log->Buffer) - bspBufferOffset < apBufferSize){
        ewlPrivateData->bufSizeOverflow += apBufferSize - (sizeof(log->Buffer) - bspBufferOffset);
      }

      //iterate through AP's EWL buffer, copy to SBSP if there is room in the buffer
      apBufferIndex = 0;
      while (apBufferIndex < apBufferSize){
        GetPipePackage(socket, (UINT8 *)&code, sizeof(UINT32));
        //copy each dword to sbsp buffer until within 4 bytes of buffer end
        if (bspBufferOffset + apBufferIndex < sizeof(log->Buffer) - 4){
          CopyMem (&log->Buffer[bspBufferOffset + apBufferIndex], (UINT8 *)&code, sizeof (UINT32));
        }
        //If there is a partial dword of space remaining, only copy that amount
        else if (bspBufferOffset + apBufferIndex < sizeof(log->Buffer)){
          CopyMem (&log->Buffer[bspBufferOffset + apBufferIndex], (UINT8 *)&code, (sizeof (log->Buffer) - bspBufferOffset - apBufferIndex));
          RcDebugPrint (SDBG_DEFAULT, "Warning Log ran out of space during sync with other sockets!\n");
        }
        // AP is going to send the rest of the log, must keep in lock step even though we are out of room in BSP buffer
        apBufferIndex += sizeof(UINT32);
      }

      //update bsp Host pointer to the end of the buffer
      if (bspBufferOffset + apBufferSize > sizeof(log->Buffer)){
        log->Header.FreeOffset = sizeof(log->Buffer);
      }
      else{
        log->Header.FreeOffset += (apBufferSize);
      }

      //update current bsp header info about # of logged warnings
      ewlPrivateData->numEntries = CountWarningsInLog ();

      //recalculate crc with copied warnings
      log->Header.Crc = CalculateCrc32((VOID *)log, sizeof(EWL_PUBLIC_DATA));

      //
      // Get fatal error status
      //
      GetPipePackage(socket, (UINT8*)&Host->var.mem.socket[socket].fatalError, sizeof(UINT8));

      //
      // BSP found a fatal error on AP
      //
      if (Host->var.mem.socket[socket].fatalError) {
        GetPipePackage(socket, (UINT8*)&Host->var.mem.socket[socket].majorCode , sizeof(UINT8));
        GetPipePackage(socket, (UINT8*)&Host->var.mem.socket[socket].minorCode , sizeof(UINT8));
        RcDebugPrint (SDBG_DEFAULT, "\n ERROR: BSP Found error on SocketId = %d registered Major Code = 0x%x, Minor Code = 0x%x \n",
                  socket,
                  Host->var.mem.socket[socket].majorCode,
                  Host->var.mem.socket[socket].minorCode);
        fatalError = Host->var.mem.socket[socket].fatalError;
        SocketFatalError = socket;
      }// if Fatal Error
    }//socket loop
  } else {
    //
    // AP Report Status
    //
    SckId   = GetSysSocketId () ;

    //
    // send number of bytes in socket's warning log buffer
    //

    SendPipePackage (Host, SckId, (UINT8*)&log->Header.FreeOffset, sizeof(UINT32), RCSIM_LARGE_DELAY);

    //
    // send the number of bytes omitted from AP's EWL buffer
    //
    SendPipePackage (Host, SckId, (UINT8*)&ewlPrivateData->bufSizeOverflow, sizeof(UINT32), RCSIM_NO_DELAY);

    //
    // for each warning in buffer send type, then send pointer to header (size data determined by header)
    //
    apBufferIndex = 0;
    while (apBufferIndex < log->Header.FreeOffset){
      code = *((UINT32 *)&log->Buffer[apBufferIndex]);
      SendPipePackage (Host, SckId, (UINT8*)&code, sizeof(UINT32), RCSIM_NO_DELAY);
      apBufferIndex += sizeof(UINT32);
    }

    //
    // Reset warning log after transfering data to the master socket
    //
    ewlPrivateData->bufSizeOverflow = 0;
    ewlPrivateData->numEntries = 0;
    log->Header.FreeOffset = 0;
    log->Header.Crc = CalculateCrc32((VOID *)log, sizeof(EWL_PUBLIC_DATA));

    //
    // Get fatal error status
    //
    SendPipePackage (Host, SckId, (UINT8*)&Host->var.mem.socket[SckId].fatalError, sizeof(UINT8), RCSIM_NO_DELAY);

    if (Host->var.mem.socket[SckId].fatalError) {
      SendPipePackage (Host, SckId, (UINT8*)&Host->var.mem.socket[SckId].majorCode, sizeof(UINT8), RCSIM_NO_DELAY);
      SendPipePackage (Host, SckId, (UINT8*)&Host->var.mem.socket[SckId].minorCode, sizeof(UINT8), RCSIM_NO_DELAY);
      //
      // Non-BSP has a fatal error and it must halt
      //
      RcDebugPrint (SDBG_DEFAULT, "\n ERROR: AP has an error on SocketId = %d registered Major Code = 0x%x,Minor Code = 0x%x \n",
                SckId,
                Host->var.mem.socket[SckId].majorCode,
                Host->var.mem.socket[SckId].minorCode);
      fatalError = Host->var.mem.socket[SckId].fatalError;
      SocketFatalError = SckId;
    }// if Fatal Error
  }//If BSP Check STATUS

  //
  // BSP Reports Fatal ERRORS to AP's
  //
  if (CheckMemRas(Host)){

    for (SocketCtr = 0; SocketCtr < MAX_SOCKET; SocketCtr++) {

      if (!SocketPresent (SocketCtr)) {
        continue;
      }

      for (socket = 0; socket < MAX_SOCKET; socket++) {
        if (socket == SbspSocketIdNv) {
          continue;
        }
        //
        // Check if socket is valid, don't send to AP its own data
        //
        if ((!SocketPresent (socket)) ||(socket == SocketCtr)) {
          continue;
        }

        //
        // Send fatal error status
        //
        SendPipePackage (Host, socket, (UINT8*)&Host->var.mem.socket[SocketCtr].fatalError, sizeof(UINT8), RCSIM_NO_DELAY);

        if (Host->var.mem.socket[SocketCtr].fatalError) {
          SendPipePackage (Host, socket, (UINT8*)&Host->var.mem.socket[SocketCtr].majorCode, sizeof(UINT8), RCSIM_NO_DELAY);
          SendPipePackage (Host, socket, (UINT8*)&Host->var.mem.socket[SocketCtr].minorCode, sizeof(UINT8), RCSIM_NO_DELAY);

          //
          // BSP has a fatal error and it must halt
          //
          RcDebugPrint (SDBG_DEFAULT, "\n ERROR: BSP has an error Reported to SocketId = %d registered Major Code = 0x%x, Minor Code = 0x%x \n",
                    socket,
                    Host->var.mem.socket[SocketCtr].majorCode,
                    Host->var.mem.socket[SocketCtr].minorCode);
          fatalError = Host->var.mem.socket[socket].fatalError;
          SocketFatalError = socket;
        }// if Fatal Error
      }//end socket Loop
    }//end SocketCtr Loop
  } else {
    //
    // AP Get status to other AP's including BSP
    //
    SckId   = GetSysSocketId () ;
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      //
      // Check if socket is valid and dont get your own data
      //
      if ((!SocketPresent (socket)) || (socket == SckId)) {
        continue;
      }

      //
      // Get fatal error status
      //
      GetPipePackage(SckId, (UINT8*)&Host->var.mem.socket[socket].fatalError, sizeof(UINT8));

      if (Host->var.mem.socket[socket].fatalError) {
        GetPipePackage(SckId, (UINT8*)&Host->var.mem.socket[socket].majorCode, sizeof(UINT8));
        GetPipePackage(SckId, (UINT8*)&Host->var.mem.socket[socket].minorCode, sizeof(UINT8));
        //
        // AP has detected a fatal error on other CPUs and it must halt
        //
        RcDebugPrint (SDBG_DEFAULT, "\n ERROR: AP detected error on SocketId = %d registered Major Code = 0x%x,Minor Code = 0x%x \n",
                  socket,
                  Host->var.mem.socket[socket].majorCode,
                  Host->var.mem.socket[socket].minorCode);
        fatalError = Host->var.mem.socket[socket].fatalError;
        SocketFatalError = socket;
      }// if fatal error
    }//socket loop
  }//end Send Status

  if (fatalError) {
    if (GetSysSocketId () == SbspSocketIdNv) {
      if (Setup->common.options & HALT_ON_ERROR_EN) {
        haltOnError = 1;
      } else {
        haltOnError = 0;
      }

#ifdef DEBUG_CODE_BLOCK
      //
      // Default halt on error with Auto cold reset and set Maximum seial debug message based on setup option
      //
      if (Setup->common.options & HALT_ON_ERROR_AUTO) {
        haltOnError = 1;
      }
#endif //DEBUG_CODE_BLOCK

      //
      // Call platform hook to handle fatal error
      //

      Status = LocateInterface (
        &gMrcPlatformHooksInterfaceGuid,
        0,
        (VOID **)&MrcPlatformHooksInterface
        );
      if (!EFI_ERROR (Status)) {
        MrcPlatformHooksInterface->PlatformFatalError (
                              Host->var.mem.socket[SocketFatalError].majorCode,
                              Host->var.mem.socket[SocketFatalError].minorCode,
                              &haltOnError
                              );
      }

    }

    //
    // Either halt or exit the MRC
    //
    HaltOnError (Host->var.mem.socket[SocketFatalError].majorCode, Host->var.mem.socket[SocketFatalError].minorCode);
  }

  return fatalError;
} // SyncErrors

/**
 * Issues the command to a slave device to read an MSR
 * @param Host    - Pointer to the system Host (root) structure
 * @param socket  - Socket number
 * @param msr     - Address of MSR to read
 *
 * @retval UINT64_STRUCT - MSR value as read from CPU
 *
 **/
UINT64_STRUCT
ReadMsrRemotePipe (
                  PSYSHOST  Host,
                  UINT8     socket,
                  UINT32    msr
                  )
{
  UINT64_STRUCT msrReg;

  //
  // Dispatch slave to read an MSR
  //
  //RcDebugPrint (SDBG_MAX,
  //              "Dispatch N%d to read MSR\n", socket);
  while (UsraCsrRead (socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host)) != 0x00);
  UsraCsrWrite (socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), PIPE_RD_MSR);

  //
  // Send the MSR to be read to the slave
  //
  SendPipePackage (Host, socket, (UINT8*)&msr, sizeof(UINT32), RCSIM_NO_DELAY);

  //
  // Get MSR data from slave
  //
  GetPipePackage(socket, (UINT8*)&msrReg, sizeof(UINT64_STRUCT));

  return msrReg;
} // ReadMsrRemotePipe

/**
 * Read MSR function for slave device using PIPE
 *
 * @param Host  - Pointer to the system Host (root) structure
 *
 * @retval N/A
 *
 **/
VOID
PipeRdMsr (
          PSYSHOST Host
          )
{
  UINT32        msr;
  UINT64_STRUCT msrReg;

  //
  // Get MSR to read
  //
  GetPipePackage(GetSysSocketId (), (UINT8*)&msr, sizeof(UINT32));

  //
  // Read MSR
  //
  msrReg = ReadMsr (msr);

  //
  // Return data
  //
  SendPipePackage (Host, GetSysSocketId (), (UINT8*)&msrReg.lo, sizeof(UINT64_STRUCT), RCSIM_NO_DELAY);

} // PipeRdMsr

/**
 * Issues the command to a slave device to write an MSR
 *
 * @param Host    - Pointer to the system Host (root) structure
 * @param socket  - Socket number
 * @param msr     - Address of MSR to read
 * @param msrReg  - MSR Data
 *
 * @retval N/A
 *
 **/
VOID
WriteMsrRemotePipe (
                   PSYSHOST      Host,
                   UINT8         socket,
                   UINT32        msr,
                   UINT64_STRUCT msrReg
                   )
{

  //
  // Dispatch slave to read an MSR
  //
  UsraCsrWrite (socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), PIPE_WR_MSR);

  //
  // Send the MSR to be read to the slave
  //
  SendPipePackage (Host, socket, (UINT8*)&msr, sizeof(UINT32), RCSIM_NO_DELAY);

  //
  // Send the MSR to be read to the slave
  //
  SendPipePackage (Host, socket, (UINT8*)&msrReg, sizeof(UINT64_STRUCT), RCSIM_NO_DELAY);

} // WriteMsrRemotePipe


/**
 * Write MSR function for slave device using PIPE
 *
 * @param Host  - Pointer to the system Host (root) structure
 *
 * @retval N/A
 *
 **/
VOID
PipeWrMsr (
          PSYSHOST Host
          )
{
  UINT32        msr;
  UINT64_STRUCT msrReg;

  //
  // Get MSR to write
  //
  GetPipePackage(GetSysSocketId (), (UINT8*)&msr, sizeof(UINT32));

  //
  // Get data to write
  //
  GetPipePackage(GetSysSocketId (), (UINT8*)&msrReg, sizeof(UINT64_STRUCT));

  //
  // Write MSR
  //
  WriteMsr (msr, msrReg);

} // PipeWrMsr

/**
 * Calls the correct read MSR function based on single vs multi-threaded execution
 *
 * @param Host    - Pointer to the system Host (root) structure
 * @param socket  - Socket number
 * @param msr     - Address of MSR to read
 *
 * @retval UINT64_STRUCT - MSR value as read from CPU
 *
 **/
UINT64_STRUCT
EFIAPI
ReadMsrPipe (
            PSYSHOST  Host,
            UINT8     socket,
            UINT32    msr
            )
{
  UINT64_STRUCT msrReg;

  if (socket != GetSysSocketId ()) {
    if (PcdGetBool (PcdDisableSimSlaveThread)) {
      ZeroMem (&msrReg, sizeof (msrReg));
    } else {
      msrReg = ReadMsrRemotePipe (Host, socket, msr);
    }
  } else {
    msrReg = ReadMsr (msr);
  }

  return msrReg;
} // ReadMsrPipe

/**
 * Calls the correct read MSR function based on single vs multi-threaded execution
 *
 * @param Host    - Pointer to the system Host (root) structure
 * @param socket  - Socket number
 * @param msr     - Address of MSR to read
 *
 * @retval UINT64 - MSR value as read from CPU
 *
 **/
UINT64
EFIAPI
ReadMsrPipe64 (
            PSYSHOST  Host,
            UINT8     socket,
            UINT32    msr
            )
{
  UINT64_STRUCT   msrRegStruct;
  UINT64          msrReg64;

  msrRegStruct = ReadMsrPipe (Host, socket, msr);
  msrReg64 = (UINT64)LShiftU64 (msrRegStruct.hi, 32) + msrRegStruct.lo;

  return msrReg64;
} // ReadMsrPipe64



UINT64_STRUCT
ReadMsrPipeHL (
            UINT8     socket,
            UINT32    msr
            )
{
  SYSHOST *Host;
  Host = GetSysHostPointer ();

  return ReadMsrPipe (Host,socket,msr);
} // ReadMsrPipeHL

UINT64
ReadMsrPipeHL64 (
            UINT8     socket,
            UINT32    msr
            )
{
  UINT64_STRUCT   msrRegStruct;
  UINT64          msrReg64;

  msrRegStruct = ReadMsrPipeHL (socket, msr);

  msrReg64 = (UINT64)LShiftU64 (msrRegStruct.hi, 32) + msrRegStruct.lo;

  return msrReg64;
} // ReadMsrPipeHL64


/**
 * Calls the correct write MSR function based on single vs multi-threaded execution
 *
 * @param Host    - Pointer to the system Host (root) structure
 * @param socket  - Socket number
 * @param msr     - Address of MSR to read
 * @param msrReg  - MSR Data
 *
 * @retval N/A
 *
 **/
VOID
EFIAPI
WriteMsrPipe (
             PSYSHOST      Host,
             UINT8         socket,
             UINT32        msr,
             UINT64_STRUCT msrReg
             )
{

  if (socket != GetSysSocketId ()) {
    if (!PcdGetBool (PcdDisableSimSlaveThread)) {
      WriteMsrRemotePipe (Host, socket, msr, msrReg);
    }
  } else {
    WriteMsr (msr, msrReg);
  }

  if (UbiosGenerationEnabled ()) {
    RcDebugPrint (SDBG_DEFAULT, "\n mov  ebx, 0%08xh", socket);
    RcDebugPrint (SDBG_DEFAULT, "\n mov  ecx, 0%08xh", msr);
    RcDebugPrint (SDBG_DEFAULT, "\n mov  eax, 0%08xh", msrReg.lo);
    RcDebugPrint (SDBG_DEFAULT, "\n mov  edx, 0%08xh", msrReg.hi);
    RcDebugPrint (SDBG_DEFAULT, "\n call MsrProgram\n");
  }

} // WriteMsrPipe

/**
 * Calls the correct write MSR function based on single vs multi-threaded execution
 *
 * @param Host    - Pointer to the system Host (root) structure
 * @param socket  - Socket number
 * @param msr     - Address of MSR to read
 * @param msrReg  - MSR Data
 *
 * @retval N/A
 *
 **/
VOID
EFIAPI
WriteMsrPipe64 (
             PSYSHOST      Host,
             UINT8         socket,
             UINT32        msr,
             UINT64        msrReg
             )
{
  UINT64_STRUCT    MsrRegStruct;

  MsrRegStruct.lo = (UINT32)msrReg;
  MsrRegStruct.hi = (UINT32)RShiftU64 (msrReg, 32);

  WriteMsrPipe (Host, socket, msr, MsrRegStruct);

} // WriteMsrPipe64


VOID
WriteMsrPipeHL (
             UINT8         socket,
             UINT32        msr,
             UINT64_STRUCT msrReg
             )
{
  SYSHOST *Host;
  Host = GetSysHostPointer ();

  WriteMsrPipe (Host,socket,msr,msrReg);
} // WriteMsrPipeHL

VOID
WriteMsrPipeHL64 (
             UINT8         socket,
             UINT32        msr,
             UINT64        msrReg
             )
{
  UINT64_STRUCT    MsrRegStruct;

  MsrRegStruct.lo = (UINT32)msrReg;
  MsrRegStruct.hi = (UINT32)RShiftU64 (msrReg, 32);

  WriteMsrPipeHL (socket, msr, MsrRegStruct);
} // WriteMsrPipeHL64


/**
 * Starts communication with NBSP's.
 * Sends parameters to NBSP's, so they can execute cpuid.
 *
 * @param  Host          Pointer to sysHost, the system Host (root) structure
 * @param  Socket        Id of socket on which CPUID have to be executed
 * @param  *CpuidParams  A pointer to the structure that contains cpuid parameters
 * @param  *CpuidInfo    A pointer to the structure that contains values returned by cpuid
 *
 * @retval None
 *
 **/
VOID
ExecuteCpuidRemoteSocket (
          PSYSHOST        Host,
          UINT8           Socket,
          CPUID_PARAMS    *CpuidParams,
          CPUID_REG_INFO  *CpuidInfo
          )
{
  while (UsraCsrRead (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host)) != 0x00);
  UsraCsrWrite (Socket, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR_ADDR(Host), PIPE_DISPATCH_EXECUTE_CPUID);

  //
  // Send the parameters to be read to the slave
  //
  SendPipePackage (Host, Socket, (UINT8*)CpuidParams, sizeof(CPUID_PARAMS), RCSIM_NO_DELAY);

  //
  // Get cpuid data from slave
  //
  GetPipePackage (Socket, (UINT8*)CpuidInfo, sizeof(CPUID_REG_INFO));
} // ExecuteCpuidRemoteSocket

/**
 * Executes CPUID on socket from parameter. Uses pipe to comunicate with non-SBSP sockets.
 *
 * @param  Socket        Number of socket on which CPUID have to be executed
 * @param  Index         The 32-bit value to load into EAX prior to invoking the
 *                       CPUID instruction.
 * @param  SubIndex      The 32-bit value to load into ECX prior to invoking the
 *                       CPUID instruction.
 * @param  RegisterEax   A pointer to the 32-bit EAX value returned by the CPUID
 *                       instruction. This is an optional parameter that may be NULL.
 * @param  RegisterEbx   A pointer to the 32-bit EBX value returned by the CPUID
 *                       instruction. This is an optional parameter that may be NULL.
 * @param  RegisterEcx   A pointer to the 32-bit ECX value returned by the CPUID
 *                       instruction. This is an optional parameter that may be NULL.
 * @param  RegisterEdx   A pointer to the 32-bit EDX value returned by the CPUID
 *                       instruction. This is an optional parameter that may be NULL.
 * @retval None
 *
 **/
VOID
ExecuteCpuidPipe (
           UINT8         Socket,
           UINT32        Index,
           UINT32        SubIndex,
           UINT32        *RegisterEax,
           UINT32        *RegisterEbx,
           UINT32        *RegisterEcx,
           UINT32        *RegisterEdx
           )
{
  CPUID_PARAMS       CpuidParams;
  CPUID_REG_INFO     CpuidInfo;
  SYSHOST            *Host;

  //
  // Save parameters in the structure, so we can pass it to the CSR registers
  //
  CpuidParams.Index = Index;
  CpuidParams.SubIndex = SubIndex;

  Host = GetSysHostPointer ();

  if (Socket != GetSysSocketId ()) {
    if (PcdGetBool (PcdDisableSimSlaveThread)) {
      //
      // Load the value of CPUID7 specified in RcSim ini configuration files so that
      // the correct value of CPUID7 can be returned in the RcSim single-thread mode.
      // This branch is only reachable in the RcSim single-thread mode.
      //
      RcSimSetSetupParamOverride (RCSIMEXTCPUID7EAX, RegisterEax);
      RcSimSetSetupParamOverride (RCSIMEXTCPUID7EBX, RegisterEbx);
      RcSimSetSetupParamOverride (RCSIMEXTCPUID7ECX, RegisterEcx);
      RcSimSetSetupParamOverride (RCSIMEXTCPUID7EDX, RegisterEdx);
    } else {
      ExecuteCpuidRemoteSocket (Host, Socket, &CpuidParams, &CpuidInfo);
      *RegisterEax = CpuidInfo.RegisterEax;
      *RegisterEbx = CpuidInfo.RegisterEbx;
      *RegisterEcx = CpuidInfo.RegisterEcx;
      *RegisterEdx = CpuidInfo.RegisterEdx;
    }
  } else {
    AsmCpuidEx (Index, SubIndex, RegisterEax, RegisterEbx, RegisterEcx, RegisterEdx);
  }
} // ExecuteCpuidPipe

/**
 * Executes cpuid on NBSP and saves returned data to CpuidInfo structure.
 *
 * @param  Host          Pointer to sysHost, the system Host (root) structure
 *
 * @retval None
 *
 **/
VOID
CpuidNbspSide (
          PSYSHOST   Host
          )
{
  CPUID_PARAMS       CpuidParams;
  CPUID_REG_INFO     CpuidInfo;

  //
  // Get params for cpuid
  //
  GetPipePackage (GetSysSocketId (), (UINT8*)&CpuidParams, sizeof(CPUID_PARAMS));

  //
  // Execute cpuid
  //
  AsmCpuidEx (CpuidParams.Index, CpuidParams.SubIndex, &CpuidInfo.RegisterEax, &CpuidInfo.RegisterEbx, &CpuidInfo.RegisterEcx, &CpuidInfo.RegisterEdx);

  //
  // Return data
  //
  SendPipePackage (Host, GetSysSocketId (), (UINT8*)&CpuidInfo, sizeof(CPUID_REG_INFO), RCSIM_NO_DELAY);
} // CpuidNbspSide
