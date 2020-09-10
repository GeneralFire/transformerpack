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
  Copyright 2012 - 2018 Intel Corporation. <BR>

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
#include <Library/SystemInfoLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/SsaLoaderLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/SsaBiosServicesPpi.h>
#include <Library/MemorySsaLib.h>
#include <Library/PrintLib.h>

/**
  Install SSA BIOS Services PPI

  @param[in]  PeiServices  -  An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.

  @retval EFI_SUCCESS  -  PPI Installed successfully
  @retval Other        -..PPI installation failed.
**/
EFI_STATUS
InstallBssaPpi (
  IN CONST EFI_PEI_SERVICES **PeiServices
  );

VOID
EFIAPI
BssaDynamicPrintWrapper (
  SSA_BIOS_SERVICES_PPI *This,
  PRINT_LEVEL           PrintLevel,
  UINT8                 *FormatString,
  ...
  )
{
  VA_LIST Marker;
  CHAR8 Buffer[MAX_EX_DEBUG_STR_LEN];

  if (FormatString != NULL) {
    VA_START (Marker, FormatString);
    AsciiVSPrint (Buffer, MAX_EX_DEBUG_STR_LEN, (CHAR8 *)FormatString, Marker);
    RcDebugPrint (PrintLevel, Buffer);
    VA_END (Marker);
  }
} // External content needs dynamically linked print function with ... arg.

#pragma pack (push, 1)

/*****************************************************************************
SSA Api call table:
*****************************************************************************/

SSA_BIOS_SERVICES_PPI SsaBiosServicesConst = {
    SSA_REVISION_BIOS,
    NULL, //&SsaCommonConfig,
    NULL, //&SsaResultsConfig,
    NULL, //&SsaMemoryConfig,
    NULL, //&SsaMemoryClientConfig,
    NULL, //&SsaMemoryServerConfig,
    NULL  //&SsaMemoryDdrtConfig
  };

SSA_COMMON_CONFIG SsaCommonConfigConst = {
    SSA_REVISION_COMMON,
    NULL,                   // MrcData
    NULL,                   // HookBuffer
    BiosReadMem,
    BiosWriteMem,
    BiosReadIo,
    BiosWriteIo,
    BiosReadPci,
    BiosWritePci,
    BiosGetBaseAddress,
    BiosMalloc,
    BiosFree,
    SSA_D_ERROR,  //PRINT_LEVEL SSA_D_ERROR
    BssaDynamicPrintWrapper,
    BiosWait,
    BiosSaveToBdat,
    BiosRegisterHookHandler,
    BiosRunHook,
    BiosSaveDataPointer,
    BiosRestoreDataPointer,
    BiosDisableWatchdogTimer,
    BiosGetTimestamp
  };

SSA_RESULTS_CONFIG SsaResultsConfigConst = {
    SSA_REVISION_RESULT,
    NULL,                   // Results buffer
    BiosCreateResultBuffer,
    BiosSetRevision,
    BiosGetRevision,
    BiosSetMetadataType,
    BiosGetMetadataType,
    BiosGetMetadataSize,
    BiosGetMetadata,
    BiosSetResultElementType,
    BiosGetResultElementType,
    BiosGetResultElementSize,
    BiosGetResultElementCapacity,
    BiosGetResultElementCount,
    BiosGetNextResultElement,
    BiosSetExplicitTransferMode,
    BiosGetExplicitTransferMode,
    BiosPushResults,
    BiosIncreaseResultBuffer
  };

SSA_MEMORY_CONFIG SsaMemoryConfigConst = {
    SSA_REVISION_MEMORY,
    NULL,                   // MrcData
    BiosGetSystemInfo,
    BiosGetMemVoltage,
    BiosSetMemVoltage,
    BiosGetMemTemp,
    BiosGetControllerBitMask,
    BiosGetChannelBitMask,
    BiosGetDimmBitMask,
    BiosGetRankInDimm,
    BiosGetLogicalRank,
    BiosGetDimmInfo,
    BiosGetDimmUniqueModuleId,
    BiosGetSpdData,
    BiosJedecReset,
    BiosIoReset,
    BiosGetMarginParamSpecificity,
    BiosInitMarginParam,
    BiosGetMarginParamLimits,
    BiosSetMarginParamOffset,
    BiosSetZQCalConfig,
    BiosSetRCompUpdateConfig,
    BiosSetPageOpenConfig,
    BiosScrubMemory,
    BiosInitCpgc
  };

SSA_MEMORY_SERVER_CONFIG SsaMemoryServerConfigConst = {
    SSA_REVISION_MEMORY_SERVER,
    BiosServerGetCpgcInfo,
    BiosServerGetCsEncoding,
    BiosServerGet3dsRankPerCs,
    BiosServerSetCreditWaitConfig,
    BiosServerSetWpqInOrderConfig,
    BiosServerSetMultipleWrCreditConfig,
    BiosServerSetCpgcLockState,
    BiosServerSetDqdbPattern,
    BiosServerGetDqdbPattern,
    BiosServerSetDqdbIndexes,
    BiosServerSetDqdbEccDataSource,
    BiosServerSetDqdbUniseqMode,
    BiosServerSetDqdbUniseqWrSeed,
    BiosServerSetDqdbUniseqRdSeed,
    BiosServerGetDqdbUniseqWrSeed,
    BiosServerGetDqdbUniseqRdSeed,
    BiosServerSetDqdbUniseqLmn,
    BiosServerSetDqdbUniseqLfsrConfig,
    BiosServerSetDqdbInvertDcConfig,
    BiosServerSetCadbConfig,
    BiosServerSetCadbPattern,
    BiosServerSetCadbUniseqMode,
    BiosServerSetCadbUniseqSeed,
    BiosServerSetCadbUniseqLmn,
    BiosServerSetDummyReadLmn,
    BiosServerSetDummyReadBankMask,
    BiosServerSetSubseqConfig,
    BiosServerSetSubseqOffsetConfig,
    BiosServerSetSeqConfig,
    BiosServerSetLoopCount,
    BiosServerSetSeqAddrConfig,
    BiosServerSetSeqStartAddr,
    BiosServerSetSeqWrapAddr,
    BiosServerSetSeqAddrInc,
    BiosServerSetSeqRankMap,
    BiosServerSetSeqRowAddrSwizzle,
    BiosServerGetSeqState,
    BiosServerSetStopMode,
    BiosServerSetErrorCounterMode,
    BiosServerSetLaneValidationMask,
    BiosServerSetChunkValidationMask,
    BiosServerSetCachelineValidationMask,
    BiosServerGetLaneErrorStatus,
    BiosServerGetChunkErrorStatus,
    BiosServerGetRankErrorStatus,
    BiosServerGetErrorAddress,
    BiosServerGetErrorCount,
    BiosServerSetDqdbErrorCaptureConfig,
    BiosServerGetDqdbErrorCaptureStatus,
    BiosServerSetLocalTestControl,
    BiosServerSetGlobalTestControl,
    BiosServerSetAddrConfig,
    BiosServerGetTestDone,
    BiosServerSetScramblerConfig,
    BiosServerSetSelfRefreshConfig,
    BiosServerSetRefreshOverride,
    BiosServerSetZQCalOverride,
    BiosServerSetOdtOverride,
    BiosServerSetCkeOverride
  };

SSA_MEMORY_DDRT_CONFIG SsaMemoryDdrtConfigConst = {
    SSA_REVISION_MEMORY_DDRT,
    BiosSetEridValidationMask,
    BiosGetEridErrorStatus,
    BiosClearEridErrorStatus,
    BiosSetEridPattern,
    BiosSetEridTrainingConfig,
    BiosSetAepDimmTrainingMode,
    BiosAepIoReset,
    BiosAepRpqDrain,
    BiosAepClrPrefetchCache,
    BiosAepFifoTrainReset,
    BiosSetAepTimerConfig,
    BiosGetAepTimerErrorStatus
  };

/*****************************************************************************
SSA Api initialization functions:
*****************************************************************************/


/**
  Install SSA BIOS Services PPI

  @param[in]  PeiServices  -  An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.

  @retval EFI_SUCCESS  -  PPI Installed successfully
  @retval Other        -..PPI installation failed.
**/
EFI_STATUS
InstallBssaPpi (
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS               Status = EFI_SUCCESS;

  SSA_BIOS_SERVICES_PPI    *SsaBiosServicesPpi;
  SSA_COMMON_CONFIG        *SsaCommonConfig;
  SSA_RESULTS_CONFIG       *SsaResultsConfig;
  SSA_MEMORY_CONFIG        *SsaMemoryConfig;
  SSA_MEMORY_SERVER_CONFIG *SsaMemoryServerConfig;
  SSA_MEMORY_DDRT_CONFIG   *SsaMemoryDdrtConfig;
  EFI_PEI_PPI_DESCRIPTOR   *SsaBiosServicesPpiDesc;  //Pointer to store the Address of SSA_BIOS_SERVICES_PPI

  //Allocating PEI memory for SSA_BIOS_SERVICES_PPI and saving the base address of that memory region in *SsaBiosServicesPpi
  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (SSA_BIOS_SERVICES_PPI), &SsaBiosServicesPpi);
  ASSERT_EFI_ERROR (Status);
  //Allocating PEI memory for each of the BSSA Structs
  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (SSA_COMMON_CONFIG), &SsaCommonConfig);//Address of PEI region saved in SsaCommonConfig
  ASSERT_EFI_ERROR (Status);
  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (SSA_RESULTS_CONFIG), &SsaResultsConfig);
  ASSERT_EFI_ERROR (Status);
  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (SSA_MEMORY_CONFIG), &SsaMemoryConfig);
  ASSERT_EFI_ERROR (Status);
  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (SSA_MEMORY_SERVER_CONFIG), &SsaMemoryServerConfig);
  ASSERT_EFI_ERROR (Status);
  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (SSA_MEMORY_DDRT_CONFIG), &SsaMemoryDdrtConfig);
  ASSERT_EFI_ERROR (Status);
  //Allocating PEI memory for the DESCRIPTOR
  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (EFI_PEI_PPI_DESCRIPTOR), &SsaBiosServicesPpiDesc);
  ASSERT_EFI_ERROR (Status);

  //Copying into the PEI Memory Region all the struct SSA_BIOS_SERVICES_PPI
  (*PeiServices)->CopyMem ((VOID*) SsaBiosServicesPpi, (VOID*) &SsaBiosServicesConst, sizeof (SSA_BIOS_SERVICES_PPI));
  //Copying into the PEI Memory Region all the BSSA Structs
  (*PeiServices)->CopyMem ((VOID*) SsaCommonConfig, (VOID*) &SsaCommonConfigConst, sizeof (SSA_COMMON_CONFIG));
  (*PeiServices)->CopyMem ((VOID*) SsaResultsConfig, (VOID*) &SsaResultsConfigConst, sizeof (SSA_RESULTS_CONFIG));
  (*PeiServices)->CopyMem ((VOID*) SsaMemoryConfig, (VOID*) &SsaMemoryConfigConst, sizeof (SSA_MEMORY_CONFIG));
  (*PeiServices)->CopyMem ((VOID*) SsaMemoryServerConfig, (VOID*) &SsaMemoryServerConfigConst, sizeof (SSA_MEMORY_SERVER_CONFIG));
  (*PeiServices)->CopyMem ((VOID*) SsaMemoryDdrtConfig, (VOID*) &SsaMemoryDdrtConfigConst, sizeof (SSA_MEMORY_DDRT_CONFIG));

  SsaBiosServicesPpi->SsaCommonConfig = SsaCommonConfig;
  SsaBiosServicesPpi->SsaResultsConfig = SsaResultsConfig;
  SsaBiosServicesPpi->SsaMemoryConfig = SsaMemoryConfig;
  SsaBiosServicesPpi->SsaMemoryClientConfig = NULL;
  SsaBiosServicesPpi->SsaMemoryServerConfig = SsaMemoryServerConfig;
  SsaBiosServicesPpi->SsaMemoryDdrtConfig = SsaMemoryDdrtConfig;
  SsaBiosServicesPpi->SsaCommonConfig->BiosData = (VOID*)GetSysHostPointer ();
  SsaBiosServicesPpi->SsaMemoryConfig->MrcData  = (VOID*)GetSysHostPointer ();

  //Saving the gSsaBiosServices Flags, Ppi & Guid into the SSA Bios Descriptor
  ZeroMem (SsaBiosServicesPpiDesc, sizeof (EFI_PEI_PPI_DESCRIPTOR));
  SsaBiosServicesPpiDesc->Flags  = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  SsaBiosServicesPpiDesc->Guid   = &gSsaBiosServicesPpiGuid;
  SsaBiosServicesPpiDesc->Ppi    = SsaBiosServicesPpi;
  DEBUG ((EFI_D_INFO, "\nInstalling Ssa Bios Services PPI\n"));
  Status = (*PeiServices)->InstallPpi (PeiServices, SsaBiosServicesPpiDesc);
  ASSERT_EFI_ERROR (Status);
  return Status;
}


/**

  Install SSA PPI,Initialize SSA Loader method.

  @param[in]  PeiServices  -  An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.

  @retval None

**/
VOID
EFIAPI
SsaBiosInitialize (
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
  UINT8     ResultIdx;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  DEBUG ((EFI_D_INFO, "BIOS SSA Initialization Started\n"));

  //Initializing the BSSA BDAT variables - BiosSaveToBdat()
  Host->var.mem.bssaBdatSize      = 0;
  for (ResultIdx = 0; ResultIdx < MAX_NUMBER_SSA_BIOS_RESULTS; ResultIdx++) {
    Host->var.mem.bssaNumberHobs[ResultIdx] = 0;
  }
  Host->var.mem.bssaNumberDistinctGuids = 0;

  InstallBssaPpi (PeiServices);
  SsaLoader (PeiServices);
  DEBUG ((EFI_D_INFO, "BIOS SSA Initialization Completed\n"));
}
#pragma pack (pop)
// Future placeholder: BSSA code intentionally left out for now
