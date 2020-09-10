/** @file
  PipeSyncLib.h

  Interface header file for the Pipe Sync Library Class. Sync data between Sockets.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef _PIPE_SYNC_LIB_H_
#define _PIPE_SYNC_LIB_H_

#include <ReferenceCodeDataTypes.h>
#include "SysHost.h"

//
// PIPE Commands
//
#define PIPE_ACK                        0
#define PIPE_REQUEST                    1
#define PIPE_RUN_MEMORY_COMMAND         4
#define PIPE_SUCCESS                    0

#define RUN_MEMORY_COMMAND              (1 << PIPE_RUN_MEMORY_COMMAND)

//
// Pipe Dispatcher Commands
//
#define PIPE_DISPATCH_NULL                         0
#define PIPE_DISPATCH_MEMORY_INIT                  1
#define PIPE_DISPATCH_SYNCH_PSYSHOST               4
#define PIPE_DISPATCH_COMMAND_KTI_ROUTINE          5
#define PIPE_DISPATCH_COMMAND_MULTI_SKT_UPD_GO_HLT 6
#define PIPE_DISPATCH_DETECT_DIMM_DONE             7
#define PIPE_DISPATCH_CHECK_STATUS                 8
#define PIPE_DISPATCH_DIMMRANK_DONE                9
#define PIPE_DISPATCH_MEMINIT_DONE                 10
#define PIPE_DISPATCH_COMMAND_KTI_ADAPTATION       11
#define PIPE_DISPATCH_EXIT                         0x99

//
// Magic number "WFTA" for PBSP to indicate it has entered exit code
//
#define PIPE_SYNC_WAIT_FOR_TEARDOWN_ADDRESS        0x41544657

//
// Definitions for use with SendPipePackage
// RcSimDelay parameter. Have no effect on
// non-RcSim builds
//
#define RCSIM_NO_DELAY      0

/**
 * struct PIPE
 *
 * Data Structure for Memory HOB SBSP-NBSP Communication Pipe
 *
 * Members:
 *   UINT32 CmdRequest   Request Indicator (0:Invalid or 1:Valid)
 *   UINT32 InternalCmd  Command Type:
 *                         (Responder must clear this bit on return)
 *                         000b: Write
 *                         001b: Read
 *                         010b: Query
 *                         011b: Semaphore Exit
 *                         100b: Run Memory Init
 *                         101b: Get Memory Init Status. SBSP read FBD Status in data pipe if Status = success
 *                         110b: Set Memory Init Status. NBSP read FBD
 *                         111b: Run Platform Specific Needs (like Mirroring)
 *   UINT32 Width_       Number of valid bytes in data pipe
 *   UINT32 Toggle
 *   UINT32 End_         Indicator of transaction completed
 *   UINT32 Begin        Indicator of begining of transaction
 *   UINT32 Status       Status Type:
 *                         000b:Success
 *                         001b:Invalid Protocol Error
 *                         010b:Toggle bit Error
 *                         011b:Package Error
 *   UINT32 TargetId
 *   UINT32 NumOfBytes   Number of bytes returned for a query or number of bytes remained during data transfer
 *
 **/
typedef struct
{
  UINT32 CmdRequest : 1;
  UINT32 InternalCmd: 3;
  UINT32 Width_     : 2;
  UINT32 Toggle     : 1;
  UINT32 End_       : 1;
  UINT32 Begin      : 1;
  UINT32 Status     : 2;
  UINT32 TargetId   : 4;
  UINT32 NumOfBytes : 17;
} PIPE;

/**
 * struct COMMAND_PIPE
 *
 * Members:
 *   PIPE Pipe
 *   UINT32 RawPipe
 *
 **/
typedef union {
  PIPE   Pipe;
  UINT32 RawPipe;
} COMMAND_PIPE;

/**
 * Slave side of Pipe Initialization
 *
 * @param UINT32    NodeId           NodeId/SocketId of the package BSP as a pipe slave. Only the last 8-bit is meaningful
 * @param UINT32    CfgAddrBDF       Bus:Dev:Func portion of Cofig Address of local SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR
 *
 * @retval This function does not return to caller, but enters a "cli, hlt" loop at the end.
 *
 **/
VOID
PeiPipeSlaveInit (
  UINT32    SocketId,
  UINT32    CfgAddrBDF
  );


/**
 * Master side of pipe initialization
 *
 * @param Host - Pointer to sysHost, the system Host (root) structure
 *
 * @retval None
 *
 **/
VOID
InitializePipe (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**
 * Master side of exiting Pipe on slave processors
 *
 * @param Host - Pointer to sysHost, the system Host (root) structure
 *
 * @retval None
 *
 **/
VOID
ExitPipe (
  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**
 * For RcSim, send a single byte from a slave to master indicating that the
 * slave is ready to transfer data. The slave thread will sleep in between
 * attempts to send the data in order to avoid consuming a large amount of CPU
 * resources.
 *
 * This function does nothing on non-RcSim builds.
 *
 * @param[in]  Host              A pointer to the Host data.
 * @param[in]  Socket            The socket.
 *
 * @retval None
 *
 **/
VOID
SendReadyToTransferData (
  IN PSYSHOST Host,
  IN UINT8    socketId
  );

/**
 * For RcSim, recieve a single byte from a slave to master indicating that the
 * slave is ready to transfer data.
 *
 * This function does nothing on non-RcSim builds.
 *
 * @param[in]  Host              A pointer to the Host data.
 * @param[in]  Socket            The socket.
 *
 * @retval None
 *
 **/
VOID
GetReadyToTransferData (
  IN PSYSHOST Host,
  IN UINT8    socketId
  );

/**
 * Get socket's imc mem varialbe segment of the SYSHOST structure from the specified slave processor
 *
 * @param Host      - Pointer to sysHost, the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
GetMemVarData (
  PSYSHOST Host,
  UINT8 socketId
  );

/**
 * Get socket's processorCommon variable segment of the SYSHOST structure from the specified processor
 *
 * @param[in] Host      - Pointer to sysHost, the system Host (root) structure
 * @param[in] SocketId  - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
GetProcessorCommonVarData (
  IN PSYSHOST Host,
  IN UINT8 SocketId
  );

/**
 * Get socket's imc nvram segment of the SYSHOST structure from the specified slave processor
 *
 * @param Host      - Pointer to sysHost, the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
GetMemNvramCommonData (
  PSYSHOST Host,
  UINT8 socketId
  );

/**
 * Get socket's imc nvram segment of the SYSHOST structure from the specified slave processor
 *
 * @param Host      - Pointer to sysHost, the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
GetMemNvramData (
  PSYSHOST Host,
  UINT8 socketId
  );

/**
 * Send BSP's socket imc mem varialbe segment of the SYSHOST structure to the specified slave processor
 *
 * @param Host      - Pointer to sysHost, the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
SendMemVarData (
  PSYSHOST Host,
  UINT8 socketId
  );

/**
 * Send socket's processorCommon variable segment of the SYSHOST structure from the specified processor
 *
 * @param[in] Host      - Pointer to sysHost, the system Host (root) structure
 * @param[in] SocketId  - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
SendProcessorCommonVarData (
  IN PSYSHOST Host,
  IN UINT8 SocketId
  );

/**
 * Send BSP socket imc nvram segment of the SYSHOST structure to the specified slave processor
 *
 * @param Host      - Pointer to sysHost, the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
SendMemNvramCommonData (
  PSYSHOST Host,
  UINT8 socketId
  );

/**
 * Send BSP socket imc nvram segment of the SYSHOST structure to the specified slave processor
 *
 * @param Host      - Pointer to sysHost, the system Host (root) structure
 * @param socketId  - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
SendMemNvramData (
  PSYSHOST Host,
  UINT8 socketId
  );

/**
 * Take control of the Command Pipe
 *
 * @param Host - Pointer to sysHost, the system Host (root) structure
 * @param Id   - PIPE Slave Id
 *
 * @retval TRUE - Command taken
 * @retval FALSE - Command not given
 *
 **/
BOOLEAN
AcquireCommandPipe (
  PSYSHOST Host,
  UINT8 Id
  );

/**
 * Unlocks the command pipe (releases control)
 *
 * @param Host - Pointer to sysHost, the system Host (root) structure
 * @param Id   - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
UnlockCommandPipe (
  PSYSHOST Host,
  UINT8 Id
  );

/**
 * Lock a specific slave's command pipe
 *
 * @param Host - Pointer to sysHost, the system Host (root) structure
 * @param Id   - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
LockCommandPipe (
  PSYSHOST Host,
  UINT8 Id
  );

/**
 * Wait for a specific slave's command pipe to be unlocked
 *
 * @param Host - Pointer to sysHost, the system Host (root) structure
 * @param Id   - PIPE Slave Id
 *
 * @retval None
 *
 **/
VOID
WaitForCommandPipeUnLocked (
  PSYSHOST Host,
  UINT8 Id
  );

/**
 * Wait for a specific slave processor's pipe's command
 *
 * @param Host    - Pointer to sysHost, the system Host (root) structure
 * @param Id      - PIPE Slave Id
 * @param CmdType - Command Type
 *
 * @retval Command that arrived in the slave processor's pipe
 *
 **/
UINT32
WaitForPipeCommand (
  PSYSHOST Host,
  UINT8 Id,
  UINT32 CmdType
  );

/**
 * Writes a command to a specific slave's pipe
 * @param Host    - Pointer to sysHost, the system Host (root) structure
 * @param Id      - PIPE Slave Id
 * @param CmdPipe - Pointer to a command PIPE data structure
 *
 * @retval None
 *
 **/
VOID
WriteCommandPipe (
  PSYSHOST Host,
  UINT8 Id,
  UINT32 *CmdPipe
  );

/**
 * Send a command to the Command PIPE until an ACK is received. This
 * function should be called only by a PIPE Master.
 *
 * @param Host     - Pointer to sysHost, the system Host (root) structure
 * @param Id       - PIPE Slave Id
 * @param CmdPipe  - Pointer to a command PIPE data structure
 *
 * @retval PIPE Status
 *
 **/
UINT32
SendCommandPipe (
  PSYSHOST Host,
  UINT8 Id,
  UINT32 *CmdPipe
  );

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
  UINT8 Id,
  UINT8 *pBuffer,
  UINT32 Size
  );

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
  );

/**
 * Sync data between SBSP and other sockets
 *
 * @retval Fatal Error flag
 *
 **/
UINT8
SyncErrors (
  VOID
  );

/**
 * Calls the correct read MSR function based on single vs multi-threaded execution
 *
 * @param Host    - Pointer to sysHost, the system Host (root) structure
 * @param socket  - Socket number
 * @param msr     - Address of MSR to read
 *
 * @retval UINT64_STRUCT - MSR value as read from CPU
 *
 **/
UINT64_STRUCT
EFIAPI
ReadMsrPipe (
  PSYSHOST  Host,     // Pointer to the system Host (root) structure
  UINT8     socket,   // Socket number
  UINT32    msr       // Address of MSR to read
  );
/**
 * Calls the correct read MSR function based on single vs multi-threaded execution
 *
 * @param Host    - Pointer to sysHost, the system Host (root) structure
 * @param socket  - Socket number
 * @param msr     - Address of MSR to read
 *
 * @retval UINT64 - MSR value as read from CPU
 *
 **/
UINT64
EFIAPI
ReadMsrPipe64 (
  PSYSHOST  Host,     // Pointer to the system Host (root) structure
  UINT8     socket,   // Socket number
  UINT32    msr       // Address of MSR to read
  );


/**
 * Calls the correct read MSR function based on single vs multi-threaded execution
 *
 * @param socket  - Socket number
 * @param msr     - Address of MSR to read
 *
 * @retval UINT64_STRUCT - MSR value as read from CPU
 *
 **/
UINT64_STRUCT
ReadMsrPipeHL (
  UINT8     socket,
  UINT32    msr
  );
/**
 * Calls the correct read MSR function based on single vs multi-threaded execution
 *
 * @param socket  - Socket number
 * @param msr     - Address of MSR to read
 *
 * @retval UINT64 - MSR value as read from CPU
 *
 **/
UINT64
ReadMsrPipeHL64 (
  UINT8     socket,
  UINT32    msr
  );

/**
 * Calls the correct write MSR function based on single vs multi-threaded execution
 *
 * @param Host   - Pointer to sysHost, the system Host (root) structure
 * @param socket - Socket number
 * @param msr    - Address of MSR to read
 * @param msrReg - MSR Data
 *
 * @retval None
 *
 **/
VOID
EFIAPI
WriteMsrPipe (
  PSYSHOST      Host,      // Pointer to the system Host (root) structure
  UINT8         socket,    // Socket number
  UINT32        msr,       // Address of MSR to read
  UINT64_STRUCT msrReg     // MSR Data
  );
/**
 * Calls the correct write MSR function based on single vs multi-threaded execution
 *
 * @param Host   - Pointer to sysHost, the system Host (root) structure
 * @param socket - Socket number
 * @param msr    - Address of MSR to read
 * @param msrReg - MSR Data
 *
 * @retval None
 *
 **/
VOID
EFIAPI
WriteMsrPipe64 (
  PSYSHOST      Host,      // Pointer to the system Host (root) structure
  UINT8         socket,    // Socket number
  UINT32        msr,       // Address of MSR to read
  UINT64        msrReg     // MSR Data
  );


/**
 * Calls the correct write MSR function based on single vs multi-threaded execution
 *
 * @param socket - Socket number
 * @param msr    - Address of MSR to read
 * @param msrReg - MSR Data
 *
 * @retval None
 *
 **/
VOID
WriteMsrPipeHL (
  UINT8         socket,
  UINT32        msr,
  UINT64_STRUCT msrReg
  );

/**
 * Calls the correct write MSR function based on single vs multi-threaded execution
 *
 * @param socket - Socket number
 * @param msr    - Address of MSR to read
 * @param msrReg - MSR Data
 *
 * @retval None
 *
 **/
VOID
WriteMsrPipeHL64 (
  UINT8         socket,
  UINT32        msr,
  UINT64        msrReg
  );



/**
 * Executes CPUID on socket from parameter. Uses pipe to comunicate with non-SBSP sockets.
 *
 * @param  Socket        Id of socket on which CPUID have to be executed
 * @param  Index         The 32-bit value to load into EAX prior to invoking the
 *                       CPUID instruction.
 * @param  SubIndex      The 32-bit value to load into ECX prior to invoking the
 *                       CPUID instruction.
 * @param  RegisterEax   A pointer to the 32-bit EAX value returned by the CPUID
 *                       instruction. This is an optional parameter that may be
 *                       NULL.
 * @param  RegisterEbx   A pointer to the 32-bit EBX value returned by the CPUID
 *                       instruction. This is an optional parameter that may be
 *                       NULL.
 * @param  RegisterEcx   A pointer to the 32-bit ECX value returned by the CPUID
 *                       instruction. This is an optional parameter that may be
 *                       NULL.
 * @param  RegisterEdx   A pointer to the 32-bit EDX value returned by the CPUID
 *                       instruction. This is an optional parameter that may be
 *                       NULL.
 *
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
  );

#endif // _PIPE_SYNC_LIB_H_
