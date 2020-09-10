/** @file
  BIOS SSA loader ITP implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include "SysHost.h"
#include <EvItpDownloadAgent.h>
#include <Library/MemoryCoreLib.h>
#include <Library/ProcMemInitChipLib.h>
#include "SsaLoaderLibInternal.h"
#include "SsaDataBlob.h"

//
// "EVCP" - EV Check Point
//
#define EV_AGENT_CHECKPOINT  ('E' << 24) | ('V' << 16) | ('C' << 8) | 'P'

/**
  Get the BSSA API revision

  @param[in] Host           - Pointer to the system structure
  @param[in] SsaServicesPpi - Pointer to the SSA_BIOS_SERVICES_PPI

  @retval  Error Code
**/
UINT32
EvAgentGetSsaApiRev (
  IN PSYSHOST Host,
  IN SSA_BIOS_SERVICES_PPI *SsaServicesPpi
  )
{
  RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_GET_CALLBACK_REV  called ... \n");
  writeScratchPad_DATA0 (*(((UINT16*)(&SsaServicesPpi->SsaCommonConfig->Revision)) + 1));
  writeScratchPad_DATA1 (*((UINT16*)(&SsaServicesPpi->SsaCommonConfig->Revision)));
  return MRC_EVAGENT_CMD_NO_ERROR;
}

/**
  Get the EV agent revision

  @param[in] Host - Pointer to the system structure

  @retval  Error Code
**/
UINT32
EvAgentGetEvAgentRev (
  IN PSYSHOST Host
  )
{
  RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_GET_AGENT_VERSION called ... \n");
  writeScratchPad_DATA0 ((TOOL_MAJOR_VERSION << 16) | (TOOL_MINOR_VERSION));
  writeScratchPad_DATA1 (TOOL_REVISION);
  return MRC_EVAGENT_CMD_NO_ERROR;
}

/**
  Get/allocate the buffer where the test configure data blob to be placed

  @param[in] Host               - Pointer to the system structure
  @param[in] SsaServicesPpi     - Pointer to the SSA_BIOS_SERVICES_PPI
  @param[in,out] BufferAddress  - Pointer to a variable that stores the starting address of the allocated buffer 

  @retval  Error Code
**/
UINT32
EvAgentGetBuffer (
  IN PSYSHOST Host,
  IN SSA_BIOS_SERVICES_PPI *SsaServicesPpi,
  IN OUT UINT32 *BufferAddress
  )
{
  UINT32 Size;

  RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_GET_INPUT_BUFFER  called ... \n");

  Size = readScratchPad_DATA0 ();
  RcDebugPrint (SDBG_DEFAULT, "Requestd buffer size:%d \n", Size);

  *BufferAddress = (UINT32)(UINTN)SsaServicesPpi->SsaCommonConfig->Malloc (SsaServicesPpi, Size);

  writeScratchPad_DATA0 (*BufferAddress);
  RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_GET_INPUT_BUFFER  done ... \n");

  if (*BufferAddress) {
    return  MRC_EVAGENT_CMD_NO_ERROR;
  } else {
    return MRC_EVAGENT_CMD_ERROR_MALLOC_FAIL;
  }
}

/**
  Free the buffer where the test configure data blob to be placed

  @param[in] Host           - Pointer to the system structure
  @param[in] SsaServicesPpi - Pointer to the SSA_BIOS_SERVICES_PPI

  @retval  Error Code
**/
UINT32
EvAgentFreeBuffer (
  IN PSYSHOST Host,
  IN SSA_BIOS_SERVICES_PPI *SsaServicesPpi
  )
{
  UINT32 BufferAddress = 0;

  RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_FREE_MEMORY  called ... \n");
  BufferAddress = readScratchPad_DATA0 ();

  if (BufferAddress == 0) {
    return MRC_EVAGENT_CMD_ERROR_INVALID_MEMORY_HANDLE;
  }

  SsaServicesPpi->SsaCommonConfig->Free (SsaServicesPpi, (VOID*)(UINTN)BufferAddress);
  RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_FREE_MEMORY  done ... \n");
  return MRC_EVAGENT_CMD_NO_ERROR;
}

/**
  Free the buffer contains test result.

  @param[in] Host           - Pointer to the system structure
  @param[in] SsaServicesPpi - Pointer to the SSA_BIOS_SERVICES_PPI

  @retval  Error Code
**/
UINT32
EvAgentFreeResult (
  IN PSYSHOST Host,
  IN SSA_BIOS_SERVICES_PPI *SsaServicesPpi
  )
{
  RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_FREE_RESULTS  called ... \n");
  if (SsaServicesPpi->SsaResultsConfig->ResultsData != NULL) {
    RcDebugPrint (SDBG_DEFAULT, "Freeing result buffer ... \n");
    SsaServicesPpi->SsaCommonConfig->Free (SsaServicesPpi, SsaServicesPpi->SsaResultsConfig->ResultsData);
    SsaServicesPpi->SsaResultsConfig->ResultsData = NULL;
  }
  RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_FREE_RESULTS  done ... \n");
  return MRC_EVAGENT_CMD_NO_ERROR;
}

/**
  Run test.

  @param[in] Host               - Pointer to the system structure
  @param[in] SsaServicesPpi     - Pointer to the SSA_BIOS_SERVICES_PPI
  @param[in] ConfigureDataBlob  - Pointer to the configure data blob
  @retval  Error Code
**/
UINT32
EvAgentRunTest (
  IN PSYSHOST Host,
  IN SSA_BIOS_SERVICES_PPI *SsaServicesPpi,
  IN BSSA_CONFIGURE_ENTRY *ConfigureDataBlob
  )
{
  UINT8 NumberOfConfig;
  UINT8 ConfigIndex;
  BSSA_CONFIGURE_HEADER *ConfigBlock;
  VOID *TestCfg;
  RESULTS_DATA_HDR *ResultsDataHdr;
  UINT32 ResultsDataAddress;
  UINT32 ResultsDataSize;

  RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_RUN_EV_TOOL  called ... \n");

  //
  // Check input configure and format
  //
  if (ConfigureDataBlob ==  NULL) {
    RcDebugPrint (SDBG_DEFAULT, "Configure data blob was NULL  ... \n");
    return MRC_EVAGENT_CMD_ERROR_MISSING_CONFIG_DATA;
  }

  if (IsSsaDataBlob (ConfigureDataBlob) == FALSE) {
    RcDebugPrint (SDBG_DEFAULT, "Invalid configure data blob format  ... \n");
    return MRC_EVAGENT_CMD_ERROR_INVALID_CONFIG_DATA;
  }

  if (IsChecksumValid (ConfigureDataBlob) == FALSE) {
    RcDebugPrint (SDBG_DEFAULT, "Configure data blob failed checksum  ... \n");
    return MRC_EVAGENT_CMD_ERROR_CHECKSUM_FAIL;
  }

  NumberOfConfig = GetNumberOfConfigBlocks (ConfigureDataBlob);
  RcDebugPrint (SDBG_DEFAULT, "Configure data blob contain %d configuration blocks \n", NumberOfConfig);

  for (ConfigIndex = 0; ConfigIndex < NumberOfConfig; ConfigIndex++) {

    // Clear the ResultsData
    SsaServicesPpi->SsaResultsConfig->ResultsData = NULL;

    //
    // Find the test config then run the corresponding EV test content
    //
    GetConfigBlockHandle (ConfigureDataBlob, ConfigIndex, &ConfigBlock);

    if (ConfigBlock == NULL) {
      return MRC_EVAGENT_CMD_ERROR_PARSE_CONFIG_DATA;
    }

    RcDebugPrint (SDBG_DEFAULT, "Configuration Block:%d TestType:%d Block Size:%d \n", ConfigIndex, ConfigBlock->Type,
      ConfigBlock->Size);
    GetTestCfg (ConfigBlock, &TestCfg);
    RunEvTest (SsaServicesPpi, ConfigBlock->Type, TestCfg);

    if (SsaServicesPpi->SsaResultsConfig->ResultsData != NULL) {
      ResultsDataHdr = (RESULTS_DATA_HDR *)SsaServicesPpi->SsaResultsConfig->ResultsData;
      ResultsDataAddress = (UINT32)(UINTN)SsaServicesPpi->SsaResultsConfig->ResultsData;
      ResultsDataSize = sizeof (RESULTS_DATA_HDR) + ResultsDataHdr->MdBlock.MetadataSize + (ResultsDataHdr->RsBlock.ResultElementSize * ResultsDataHdr->RsBlock.ResultCapacity);
    } else {
      ResultsDataAddress = 0;
      ResultsDataSize = 0;
    }

    writeScratchPad_DATA0 (ResultsDataAddress);
    writeScratchPad_DATA1 (ResultsDataSize);
  } // ConfigIndex loop

  RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_RUN_EV_TOOL  done ... \n");
  return MRC_EVAGENT_CMD_NO_ERROR;
}

/**

  Do EV commands loop while at a the RUN_EV_TOOL checkpoint

  @param[in] Host - Pointer to the system structure

  @retval  N/A
**/
VOID
EvItpDownloadCmd (
  IN PSYSHOST Host
  )
{
  MRC_EV_AGENT_CMD_STRUCT CmdReg;
  UINT32                  ConfigDataBlobLocation = 0;
  BOOLEAN                 ExitCmdLoop = FALSE;
  EFI_STATUS              Status;
  EFI_PEI_SERVICES        **PeiServices;
  SSA_BIOS_SERVICES_PPI   *SsaServicesPpi;
 
  RcDebugPrint (SDBG_DEFAULT, "Inside EV agent ... \n" );

  PeiServices = (EFI_PEI_SERVICES **) (UINTN) Host->var.common.oemVariable;
  Status = (*PeiServices)->LocatePpi(PeiServices, &gSsaBiosServicesPpiGuid, 0, NULL, &SsaServicesPpi);
  ASSERT_EFI_ERROR (Status);

  writeScratchPad_currentCheckPoint (EV_AGENT_CHECKPOINT);

  do {
    //
    // Read scratchpad register to get the EV command
    //
    CmdReg.Data = readScratchPad_CMD ();

    if (CmdReg.Bits.command_pending_execution == MRC_EVAGENT_NO_PENDING_CMD) {
      continue;
    }

    CmdReg.Bits.command_pending_execution = MRC_EVAGENT_NO_PENDING_CMD;
    CmdReg.Bits.status  = MRC_EVAGENT_STATUS_BUSY;
    writeScratchPad_CMD (CmdReg.Data);

    switch (CmdReg.Bits.command_opcode) {
      case MRC_EVAGENT_CMD_PING:
        CmdReg.Bits.error_code = MRC_EVAGENT_CMD_NO_ERROR;
        break;

      case MRC_EVAGENT_CMD_GET_CALLBACK_REV:
        CmdReg.Bits.error_code = EvAgentGetSsaApiRev (Host, SsaServicesPpi);
        break;

      case MRC_EVAGENT_CMD_GET_AGENT_VERSION:
        CmdReg.Bits.error_code = EvAgentGetEvAgentRev (Host);
        break;

      case MRC_EVAGENT_CMD_GET_INPUT_BUFFER:
        CmdReg.Bits.error_code = EvAgentGetBuffer (Host, SsaServicesPpi, &ConfigDataBlobLocation);
        break;

      case MRC_EVAGENT_CMD_FREE_RESULTS:
        CmdReg.Bits.error_code = EvAgentFreeResult (Host, SsaServicesPpi);
        break;

      case MRC_EVAGENT_CMD_FREE_MEMORY:
        CmdReg.Bits.error_code = EvAgentFreeBuffer (Host, SsaServicesPpi);
        break;

      case MRC_EVAGENT_CMD_RUN_EV_TOOL:
        CmdReg.Bits.error_code = EvAgentRunTest (Host, SsaServicesPpi, (BSSA_CONFIGURE_ENTRY*)(UINTN)ConfigDataBlobLocation);
        break;

      case MRC_EVAGENT_CMD_EXIT:
        RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_EXIT  called ... \n");
        ExitCmdLoop = TRUE;
        CmdReg.Bits.error_code = MRC_EVAGENT_CMD_NO_ERROR;
        RcDebugPrint (SDBG_DEFAULT, "MRC_EVAGENT_CMD_EXIT  done ... \n");
        break;

      default:
        RcDebugPrint (SDBG_DEFAULT, "Unknown command opcode:0x%x\n", CmdReg.Bits.command_opcode);
        CmdReg.Bits.error_code = MRC_EVAGENT_CMD_ERROR_UNKNOWN_CMD;
        break;
    }

    CmdReg.Bits.status = MRC_EVAGENT_STATUS_READY;
    writeScratchPad_CMD (CmdReg.Data);
 
  } while (!ExitCmdLoop); // While still in the cmd loop
}
