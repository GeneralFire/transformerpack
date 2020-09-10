/** @file
  Enable SSA BIOS Loader API implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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
#include <Library/MemDdrioIpLib.h>
#include <ScratchpadList.h>
#include "SsaLoaderLibInternal.h"
#include <EvLoaderPeim.h>
#include <Library/MemFmcIpLib.h>
#include <Library/BasicIpBlockServicesLib.h>

#include <Ppi/SsaBiosServicesPpi.h>

#include <Guid/MrcPlatformHooksGuid.h>

/**
  Initialize the SsaBiosServices load status.

  @param[in] MrcData  - The MRC global data area.

  @retval Nothing
**/
VOID
SsaBiosLoadStatus (
  CONST EFI_PEI_SERVICES  **PeiServices
  );

/**
  Internal function to load SSA Checkpoint for Target Loader.
  Reads Checkpoint written during Target loader, and loads it in Scratchpad register for processing
  during MRC Training execution.

  @param[in]  PeiServices  -  An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
**/
VOID
EFIAPI
SsaLoaderInternal (
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS Status;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  RcDebugPrint (SDBG_DEFAULT, "SsaLoader - Entry \n");

  if (PcdGetBool (PcdMrcSsaOemHook) == TRUE) {
    SYS_SETUP *Setup;

    Setup = GetSysSetupPointer ();

    Status = LocateInterface (
      &gMrcPlatformHooksInterfaceGuid,
      0,
      (VOID **)&MrcPlatformHooksInterface
      );
    if (!EFI_ERROR (Status)) {

      if ((MrcPlatformHooksInterface->PlatformDetectPhysicalPresenceSSA () == TRUE) && (Setup->mem.enableBiosSsaLoader)) {
        SsaBiosLoadStatus (PeiServices);
      }

    }

  }
  RcDebugPrint (SDBG_DEFAULT, "SsaLoader - Exit \n");
}

/**
  Load SSA Checkpoint for Target Loader.
  Reads Checkpoint written during Target loader, and loads it in Scratchpad register for processing
  during MRC Training execution.

  @param[in]  PeiServices  -  An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
**/
VOID
EFIAPI
SsaLoader (
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
  SsaLoaderInternal (PeiServices);
}

/**
  Function setups scratchpad register with EV command for Target loader at given
  checkpoint

  @param[in] MajorCode  -    Checkpoint Major code
  @param[in] MinorCode  -    Checkpoint Minor code

  @retval  None
**/
VOID
SetupSsaTargetLoader (
  IN UINT8    MajorCode,
  IN UINT8    MinorCode
  )
{
  EFI_STATUS                    Status;
  UINT16                        Checkpoint;
  MRC_EV_AGENT_CMD_STRUCT       agentCmd;
  UINT32                        agentReg;
  SYS_SETUP                     *Setup;
  MRC_PLATFORM_HOOKS_INTERFACE        *MrcPlatformHooksInterface = NULL;

  if (PcdGetBool (PcdMrcSsaOemHook) == TRUE) {

    Setup = GetSysSetupPointer ();

    Status = LocateInterface (
      &gMrcPlatformHooksInterfaceGuid,
      0,
      (VOID **)&MrcPlatformHooksInterface
      );
    if (!EFI_ERROR (Status)) {

      if ((MrcPlatformHooksInterface->PlatformDetectPhysicalPresenceSSA () == TRUE) && (Setup->mem.enableBiosSsaLoader == 1)) {

        RcDebugPrint (SDBG_DEFAULT, "SetupSsaTargetLoader() -- Target-Only \n");
        Checkpoint = (UINT16)((MajorCode << 8) | MinorCode);
        RcDebugPrint (SDBG_DEFAULT, "\nSetupSsaTargetLoader() CheckPoint = %x\n", Checkpoint);
        // Setup for target only
        agentCmd.Data = 0;
        agentCmd.Bits.command_pending_execution = MRC_EVAGENT_PENDING_CMD;

        agentCmd.Bits.command_opcode = MRC_EVAGENT_CMD_START_TARGET_ONLY;

        // Setup postcode
        agentCmd.Bits.reserved = 0x0000FFFF & Checkpoint;

        // Setup register
        agentReg = agentCmd.Data;

        SsaBiosLoadStatusChip (agentReg);
      }

    }

  }
}

/**
  Initialize the SsaBiosServices load status.

  @param[in] MrcData  - The MRC global data area.

  @retval Nothing
**/
VOID
SsaBiosLoadStatus (
  CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                             Status;
  UINT8                                  DataBuffer[VAR_BUFFER_SIZE];
  UINTN                                  DataSize = sizeof(DataBuffer);
  EFI_PEI_READ_ONLY_VARIABLE2_PPI        *VarAccessPpiPtr = NULL;
  SSA_BIOS_SYS_CONFIG_HEADER * sysConfigHeaderPtr = NULL;

  RcDebugPrint (SDBG_DEFAULT, "Inside SsaBiosLoadStatus() ... \n");

  // Locate the EFI read only variable PPI
  Status = (*PeiServices)->LocatePpi(PeiServices,
    &gEfiPeiReadOnlyVariable2PpiGuid,
    0,
    NULL,
    (VOID **)&VarAccessPpiPtr);
  if(Status == EFI_NOT_FOUND) {
    RcDebugPrint (SDBG_DEFAULT, "Read Only Variable PPI NOT found. \n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "Read Only Variable PPI found! \n");
    // Check for Target-only
    Status = VarAccessPpiPtr->GetVariable(VarAccessPpiPtr, SYS_CONFIG_NAME, &gBiosSsaVariablesGuid, NULL, &DataSize, (VOID *)DataBuffer);
    RcDebugPrint (SDBG_DEFAULT, "SsaBiosLoadStatus() -- After call VarAccessPpiPtr->PeiGetVariable() \n");

    if(Status == EFI_NOT_FOUND) {  // Host-Target Hybrid
      RcDebugPrint (SDBG_DEFAULT, "SsaBiosLoadStatus() -- Host-Target Hybrid \n");
    // Do nothing
    } else if (Status == EFI_SUCCESS) { // Target-Only
      // Extract postcode
      sysConfigHeaderPtr = (SSA_BIOS_SYS_CONFIG_HEADER *)DataBuffer;
      SetupSsaTargetLoader ((sysConfigHeaderPtr->PostCode >> 8) & 0xFF, sysConfigHeaderPtr->PostCode & 0xFF);
    } else {
      ASSERT_EFI_ERROR(Status);
    }
  }
}

/**
  Internal function to enable SSA Loader functionality (Target\Host mode)

  @retval EFI_SUCCESS           SSA Loader method Supported
  @retVal EFI_UNSUPPORTED       SSA Loader method Un-Supported
**/
EFI_STATUS
EnableSsaBiosLoaderInternal (
  VOID
  )
{
  SYS_SETUP               *Setup;
  PSYSHOST                Host;
  MRC_EV_AGENT_CMD_STRUCT AgentCmd;
  EFI_STATUS              Status;

  Status = EFI_SUCCESS;
  Host = GetSysHostPointer ();
  Setup = GetSysSetupPointer ();

  if (Setup->mem.enableBiosSsaLoader == 1) { //Enter if SSA EV loader is invoked
    //
    // Check for opcode to start the SSA agent.
    //
    AgentCmd.Data = GetSsaAgentData (0);

    switch (AgentCmd.Bits.command_opcode) {

      case MRC_EVAGENT_CMD_START_AGENT:
        RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_START_AGENT received.\n");
        //
        // Disable RID feedback for NVMDIMM
        //
        ConfigNvmdimmtRidFeedback (Host, 1);
        EvItpDownloadCmd (Host);
        ClearSsaAgentData (0);
        break;

      case MRC_EVAGENT_CMD_START_TARGET_ONLY:
        RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_START_TARGET_ONLY received.\n");
        //
        // Disable RID feedback for NVMDIMM
        //
        ConfigNvmdimmtRidFeedback (Host, 1);
        ExecuteTargetOnlyCmd (Host);
        SetStickyScratchpad (0, SR_BIOS_SERIAL_DEBUG, 0);
        break;

      default:
        Status = EFI_UNSUPPORTED;
        break;

    }  //switch
  }  else {
    Status = EFI_UNSUPPORTED;
  }
  return Status;
} // EnableSsaBiosLoaderInternal

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
  EFI_STATUS  Status;
  Status = EnableSsaBiosLoaderInternal ();
  return Status;
} //EnableSsaBiosLoader
