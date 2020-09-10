/** @file
  This file contains the implementation of the BIOS version of the SSA services PPI.
  This version contains code for both client and server implementations.
  For Client:
  Delete include of BiosSsaMemoryServerConfig.h and BiosSsaMemoryDdrtConfig.h.
  Delete definitions of SSA_REVISION_MEMORY_SERVER and SSA_REVISION_MEMORY_DDRT.
  Delete declarations of SsaMemoryServerConfig and SsaMemoryDdrtConfig.
  Replace SsaMemoryServerConfig and SsaMemoryDdrtConfig field values in SsaBiosServicesPpi with NULL.
  For Server:
  Delete include of BiosSsaMemoryClientConfig.h.
  Delete definition of SSA_REVISION_MEMORY_CLIENT.
  Delete declaration of SsaMemoryClientConfig.
  Replace SsaMemoryClientConfig field value in SsaBiosServicesPpi with NULL.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2020 Intel Corporation. <BR>

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
#include <SysHostChip.h>
#include "BiosSsaChipFunc.h"
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/MemoryEvContentLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/SystemInfoLib.h>

//
//This BSSA file contains subsets of all BSSA function calls made from the Core files; all CSR accesses are made here
//

/**
  Initialize the SsaBiosServices data structure.

  @param[in] MrcData  - The MRC global data area.

  @retval Nothing
**/
SSA_STATUS
EvTestEntry (
  SSA_BIOS_SERVICES_PPI *SsaServicesHandle
  )
{
  UINT8 *                         ResultsData = NULL;
  RESULTS_DATA_HDR *              ResultsDataHdr = NULL;
  UINT32                          ResultsDataSize = 0;
  SSA_STATUS                      RetStatus = Success;
  RMT_CONFIG                      TestConfiguration;
  VOID*                           HobAddr = NULL;
  SYS_SETUP                       *Setup;
  UINT8                           Ch;
  UINT8                           NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  Setup = GetSysSetupPointer ();

  ZeroMem (&TestConfiguration, sizeof (TestConfiguration));

  // enable/disable RMT
  // enable/disable per-bit RMT
  // enable/dis table to serail
  // en/dis plot to serial
  // loopcnt
  // step size
  // en/dis backside
  // en/dis turnaround
  // loopcnt_turnaround
  // Erid enable/disable

  if (Setup->mem.biosSsaBacksideMargining) {
    TestConfiguration.IoLevelBitmask = 0x3;
  } else {
    TestConfiguration.IoLevelBitmask = 1;
  }

  if (Setup->mem.biosSsaEarlyReadIdMargining) {
    TestConfiguration.EnableEridMargin = TRUE;
  } else {
    TestConfiguration.EnableEridMargin = FALSE;
  }

  if (Setup->mem.biosSsaPerBitMargining) {
    TestConfiguration.EnableLaneMargin = TRUE;
  } else {
   TestConfiguration.EnableLaneMargin = FALSE;
  }

  if (Setup->mem.biosSsaDisplayTables) {
   TestConfiguration.EnableTestResultLogging = TRUE;
  } else {
   TestConfiguration.EnableTestResultLogging = FALSE;
  }

  if (Setup->mem.biosSsaPerDisplayPlots) {
    TestConfiguration.EnableLaneMarginPlot = TRUE;
  } else {
   TestConfiguration.EnableLaneMarginPlot = FALSE;
  }

  TestConfiguration.SocketBitmask = 0xFFFFFFFF;
  TestConfiguration.ControllerBitmask = 0xFF;
  TestConfiguration.EnableRankMargin = TRUE;
  TestConfiguration.EnableCtlAllMargin = TRUE;

  if ((GetEmulation () & SIMICS_FLAG) != 0) {
     TestConfiguration.RxDqsStepSize = BIOS_SSA_RMT_SIMICS_VALUE;
     TestConfiguration.TxDqStepSize = BIOS_SSA_RMT_SIMICS_VALUE;
     TestConfiguration.RxVrefStepSize = BIOS_SSA_RMT_SIMICS_VALUE;
     TestConfiguration.TxVrefStepSize = BIOS_SSA_RMT_SIMICS_VALUE;
     TestConfiguration.CmdAllStepSize = BIOS_SSA_RMT_SIMICS_VALUE;
     TestConfiguration.CmdVrefStepSize = BIOS_SSA_RMT_SIMICS_VALUE;
     TestConfiguration.CtlAllStepSize = BIOS_SSA_RMT_SIMICS_VALUE;
     TestConfiguration.EridDelayStepSize = BIOS_SSA_RMT_SIMICS_VALUE;
     TestConfiguration.EridVrefStepSize = BIOS_SSA_RMT_SIMICS_VALUE;
  }
  else {
    if (Setup->mem.biosSsaStepSizeOverride) {
     TestConfiguration.RxDqsStepSize = Setup->mem.biosSsaRxDqs;
     TestConfiguration.TxDqStepSize = Setup->mem.biosSsaTxDq;
     TestConfiguration.RxVrefStepSize = Setup->mem.biosSsaRxVref;
     TestConfiguration.TxVrefStepSize = Setup->mem.biosSsaTxVref;
     TestConfiguration.CmdAllStepSize = Setup->mem.biosSsaCmdAll;
     TestConfiguration.CmdVrefStepSize = Setup->mem.biosSsaCmdVref;
     TestConfiguration.CtlAllStepSize = Setup->mem.biosSsaCtlAll;
     TestConfiguration.EridDelayStepSize = Setup->mem.biosSsaEridDelay;
     TestConfiguration.EridVrefStepSize = Setup->mem.biosSsaEridVref;
    } else {
     TestConfiguration.RxDqsStepSize = BIOS_SSA_RMT_DEFAULT_VALUE;
     TestConfiguration.TxDqStepSize = BIOS_SSA_RMT_DEFAULT_VALUE;
     TestConfiguration.RxVrefStepSize = BIOS_SSA_RMT_DEFAULT_VALUE;
     TestConfiguration.TxVrefStepSize = BIOS_SSA_RMT_DEFAULT_VALUE;
     TestConfiguration.CmdAllStepSize = BIOS_SSA_RMT_DEFAULT_VALUE;
     TestConfiguration.CmdVrefStepSize = BIOS_SSA_RMT_DEFAULT_VALUE;
     TestConfiguration.CtlAllStepSize = BIOS_SSA_RMT_DEFAULT_VALUE;
     TestConfiguration.EridDelayStepSize = BIOS_SSA_RMT_DEFAULT_VALUE;
     TestConfiguration.EridVrefStepSize = BIOS_SSA_RMT_DEFAULT_VALUE;
    }
  }


  TestConfiguration.TestStatusLogLevel = Setup->mem.biosSsaDebugMessages;
  TestConfiguration.SinglesLoopCount = Setup->mem.biosSsaLoopCount; //16

  TestConfiguration.SinglesBurstLength = 32;
  TestConfiguration.SinglesBinarySearchLoopCount = 7;
  TestConfiguration.OptimizedShmooBitmask = 0xFF;
  TestConfiguration.ScramblerOverrideMode = DisableScrambler;
  TestConfiguration.EnableExtendedRange = TRUE;

  TestConfiguration.EnableChannelSpecificLoopCountsBurstlengths = FALSE;

  for (Ch = 0; Ch < NumChPerMc; Ch++) {
    TestConfiguration.BurstLengths[Ch] = 32;
    TestConfiguration.LoopCounts[Ch] = 16;
  }

  // Reset the scratchpad registers that will contain the test result address and size.
  writeScratchPad_DATA0 (0x0);
  writeScratchPad_DATA1 (0x0);


  // Call EV RMT test content entry point
  RmtTestEntryPoint (SsaServicesHandle, &TestConfiguration);

  // Use the SSA result service
  ResultsData = (UINT8*)SsaServicesHandle->SsaResultsConfig->ResultsData;
  ResultsDataHdr = (RESULTS_DATA_HDR *) ResultsData;
  ResultsDataSize = sizeof (RESULTS_DATA_HDR) + ResultsDataHdr->MdBlock.MetadataSize + (ResultsDataHdr->RsBlock.ResultElementSize * ResultsDataHdr->RsBlock.ResultCapacity);

  // Results are copied to HOBs in PEI and eventually saved to BDAT in DXE.

  if (PcdGetBool (SaveBuiltInRmtResultToBdat)) {
    RetStatus = SsaServicesHandle->SsaCommonConfig->SaveToBdat (SsaServicesHandle, &gBiosSsaResultsGuid, ResultsData, ResultsDataSize);
    // Save the HOB result data location and size to scratchpad registers, so that the RMT result can
    // be retrieved with ITP even before system boots to EFI.
    HobAddr = GetFirstGuidHob (&gBiosSsaResultsGuid);
    if (HobAddr != NULL) {
      UINT32              PrevDebugLevel;

      //
      // The HobAddr regularly changes between simulation runs and causes unhelpful CSR trace mismatches.
      // Avoid mismatches by skipping this CSR print unless SDBG_FORCE_ALL is enabled.
      //
      PrevDebugLevel = GetDebugLevel ();
      if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
        SetDebugLevel (SDBG_ERROR);
      }
      writeScratchPad_DATA0 ((UINT32)(UINTN)HobAddr);
      SetDebugLevel (PrevDebugLevel);
      writeScratchPad_DATA1 (ResultsDataSize);
    }
  }
  //Freeing up the temporary memory range in the heap that is used to save of results from the 'stitched SSA' method
  SsaServicesHandle->SsaCommonConfig->Free(SsaServicesHandle, (VOID*)ResultsData);
  SsaServicesHandle->SsaResultsConfig->ResultsData = NULL;

  return RetStatus;
}


/**
Set CSR required by SsaBiosLoadStatus
**/
VOID
SsaBiosLoadStatusChip (
  UINT32             agentReg
  )
{
  UsraCsrWrite (0, 0, SR_BIOS_SERIAL_DEBUG_CSR, agentReg);

}

/**
SsaStichedModeRMTChip: Chip layer function call for setting up the SSA Structs needed for StichedModeSsaRMT
**/
VOID
SsaStichedModeRMTChip (
  SSA_BIOS_SERVICES_PPI            *SsaBiosServicesPpi
  )
{
  SSA_STATUS                    RetStatus;

  //Call EV test entry for the 'stitched code'
  RetStatus = EvTestEntry (SsaBiosServicesPpi);

  if (RetStatus != Success) {
    RcDebugPrint (SDBG_MINMAX,
      "Error running EV content.\n");
  }
}
