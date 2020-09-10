/** @file
  Pipe.h

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

#ifndef _PIPE_H_
#define _PIPE_H_

#include <ReferenceCodeDataTypes.h>
#include <Library/PipeSyncLib.h>

//
// PIPE Error Code: Failure to find SBSP
//
#define PIPE_NO_SBSP_FOUND  FBD_INIT_ERR(28)

//
// PIPE Error Codes
//
#define PIPE_INVALID                    1
#define PIPE_TOGGLE_ERROR               2
#define PIPE_INCOMPLETE_ERROR           3

//
// PIPE Commands
//
#define PIPE_READ_COMMAND               1
#define PIPE_SET_PLATFORM_COMMAND       7

#define PIPE_ID_KEY                     0x18
#define COMMAND_PIPE_LOCK(id)           (((PIPE_ID_KEY | id) << 11) | 1)
#define COMMAND_PIPE_UNLOCK(id)         ((PIPE_ID_KEY | id) << 11)

//
// Pipe Dispatcher Commands
//
#define PIPE_RD_MSR                     5
#define PIPE_WR_MSR                     6
#define PIPE_DISPATCH_EXECUTE_CPUID     11
#define PIPE_DISPATCH_FREEZE            0x80

//
// Invalid SBSP Socket ID
//
#define SKT_INVALID_ID     (UINT16)(-1)

#define SEC_IDT_ENTRY_COUNT        34
#define INITIAL_IDT_ENTRY_VALUE    0xffff8e000010ffe4ULL

//
// Definitions for use with SendPipePackage RcSimDelay parameter.
//
#define RCSIM_SMALL_DELAY   20
#define RCSIM_LARGE_DELAY   100

/**
 * In RcSim, we do not send the entire contents
 * of CAR fronm the BSP to the non-BSP processors
 * in InitializePipe (). Instead we send RCSIM_SYNC_DATA
 * over the pipe. Any future structs that are removed from
 * Host will need to be added to this structure.
 **/
typedef struct _RCSIM_SYNC_DATA {
  SYSHOST   SlaveHost;
  SYS_SETUP SlaveMemPolicy;
} RCSIM_SYNC_DATA;

typedef struct _AP_IDT_TABLE {
  UINTN             SysHostPointer;
  UINTN             PeiServices;
  UINT64            IdtTable[SEC_IDT_ENTRY_COUNT];
} AP_IDT_TABLE;

typedef struct {
  UINT32  Index;
  UINT32  SubIndex;
  } CPUID_PARAMS;

  typedef struct {
  UINT32  RegisterEax;
  UINT32  RegisterEbx;
  UINT32  RegisterEcx;
  UINT32  RegisterEdx;
  } CPUID_REG_INFO;

UINT64_STRUCT
ReadMsrRemotePipe (
  PSYSHOST Host,
  UINT8 socket,
  UINT32 msr
  );

VOID
PipeRdMsr (
  PSYSHOST Host
  );

VOID
WriteMsrRemotePipe (PSYSHOST Host,
UINT8 socket,
UINT32 msr,
UINT64_STRUCT msrReg
);

VOID
PipeWrMsr (
  PSYSHOST Host
  );

UINT8
GetLocalSktId (
  PSYSHOST Host
  );

VOID
ReadDataPipe (
  PSYSHOST Host,
  UINT8 Id,
  UINT32 *Data
  );

VOID
WriteDataPipe (
  PSYSHOST Host,
  UINT8 Id,
  UINT32 Data
  );

VOID
ListenToCommandPipe (
  IN PSYSHOST Host, 
  IN UINT8 Id, 
  IN UINT32 *CmdPipe, 
  IN UINT32 RcSimDelay
  );

/**
 * Dispatcher function for all non-SBSPs
 * 
 * @param Host - Pointer to sysHost, the system Host (root) structure
 * 
 * @retval None
 * 
 **/
VOID
PipeDispatcher (
  PSYSHOST Host
  );

/**
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
  );

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
  );

#endif // _PIPE_H_
