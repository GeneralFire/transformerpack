/** @file
  SsaLoaderNull.c

  Library implementation of API to support SSA Loader function.
  This is a NULL implementation.

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
#include <Uefi.h>
#include <Base.h>
#include <PiPei.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <EvItpDownloadAgent.h>

/**

  Check if its SSA Opcode in Scratchpad 6 register

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

  @retval TRUE  - if scratchpad has SSA opcode
  @retval FALSE - if scratchpad does not have SSA Opcode

**/
BOOLEAN
IsSSAOpCode (
  PSYSHOST                Host,
  UINT8                   Socket
  )
{
  MRC_EV_AGENT_CMD_STRUCT  AgentCmd;

  AgentCmd.Data = GetSsaAgentData (Socket);
  if (AgentCmd.Bits.command_opcode == MRC_EVAGENT_CMD_START_AGENT || AgentCmd.Bits.command_opcode  == MRC_EVAGENT_CMD_START_TARGET_ONLY) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  Enable SSA Loader functionality (Target\Host mode)

  @retval EFI_SUCCESS           SSA Loader method Supported
  @retVal EFI_UNSUPPORTED       SSA Loader method Un-Supported

**/
EFI_STATUS
EFIAPI
EnableSsaBiosLoader (
  VOID
  )
{
  PSYSHOST Host = NULL;
  UINT8 SbspSktId = 0;
  Host = GetSysHostPointer ();
  SbspSktId = GetSysSbspSocketIdNv ();

  if (IsSSAOpCode (Host, SbspSktId)) {
    RcDebugPrint (SDBG_ERROR, "SSA BIOS Loader support is disabled in BIOS Build\n");
    RcDebugPrint (SDBG_ERROR, "Clearing SSA Opcode from Scratchpad register\n");
    ClearSsaAgentData (SbspSktId);
  }
  return EFI_UNSUPPORTED;
}

/**
  Start the SSA loader.

  @param[in]  PeiServices  -  An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.

  @retval N/A
**/
VOID
EFIAPI
SsaLoader (
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
}