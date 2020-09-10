/** @file
  BIOS SSA loader Target-Only implementation.

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

#include <Library/MemoryCoreLib.h>
#include <Ppi/SsaBiosServicesPpi.h>
#include <EvLoaderPeim.h>
#include <EvItpDownloadAgent.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/RcMemAllocLib.h>
#include "SsaLoaderLibInternal.h"
#include "SsaDataBlob.h"

/**
  Save BSSA results to BDAT

  @param[in] Host    -   Pointer to the system structure

  @retval  N/A
**/
STATIC
VOID
SaveResultsToBdat (
  IN PSYSHOST Host
  );

/**
  Wrapper for running EV test.

  @param[in] Host               - Pointer to the system structure
  @param[in] ConfigureDataBlob  - Pointer to the configure data blob

**/
STATIC
VOID
RunEvTestWrapper (
  IN PSYSHOST Host,
  IN BSSA_CONFIGURE_ENTRY *ConfigureDataBlob
  );

/**
  Load/retrieve a test configure blob.

  @param[in] Host               - Pointer to the system structure
  @param[in] SysConfigHeader    - The starting address of the BSSA Syscfg variable structure
  @param[in] ConfigCount        - The total number of the configure blobs
  @param[in] ConfigIndex        - The index of the configure blob to be loaded
  @param[in] ConfigHandle       - The pointer to the buffer where the configure blob to be stored

**/
STATIC
VOID
EvLoadConfig (
  IN PSYSHOST                    Host,
  IN SSA_BIOS_SYS_CONFIG_HEADER  *SysConfigHeader,
  IN UINT32                      ConfigCount,
  IN UINT32                      ConfigIndex,
  IN UINT32                      *ConfigHandle
  );

/**
  Copy test configuration from EFI variables to a buffer

  @param[in] Host               - Pointer to the system structure
  @param[in] SourceVarGuid      - The GUID of the BSSA configure data variables
  @param[in] SourceVarName      - The name of the first EFI variable contains the configuration data
  @param[in] DestinationHandle  - The starting address of the buffer where configuration data blobs will be stored
  @param[in] DestinationSize    - The size of the buffer where the configure blob to be stored

**/
STATIC
VOID
GetConfigureFromVariables (
  IN PSYSHOST                  Host,
  IN EFI_GUID                  *SourceVarGuid,
  IN CHAR16                    *SourceVarName,
  IN OUT UINT32                DestinationHandle,
  IN UINT32                    DestinationSize
  );

/**
  Run EV test at a the RUN_EV_TOOL checkpoint in BSSA target mode.

  @param[in] Host    -   Pointer to the system structure

  @retval  N/A
**/
VOID
ExecuteTargetOnlyCmd(
  IN PSYSHOST Host
  )
{
  SSA_STATUS                      Status;
  EFI_PEI_SERVICES                **PeiServices = NULL;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VarAccessPpiPtr = NULL;
  SSA_BIOS_SYS_CONFIG_HEADER *    SysConfigHeader = NULL;
  UINT8                           DataBuffer[VAR_BUFFER_SIZE];
  UINTN                           DataSize = sizeof (DataBuffer);
  UINT32                          ConfigCount = 0;
  UINT32                          ConfigBuffer = 0;
  UINT32                          ConfigVersionHandle = 0;
  UINT32                          ConfigPointerFixupCountHandle = 0;
  UINT32                          ConfigPointerFixupCount = 0;
  UINT32                          ConfigBlockHandle = 0;
  UINT32                          configIndex = 0;


  RcDebugPrint (SDBG_DEFAULT, "Inside ExecuteTargetOnlyCmd() \n");

  //
  // Get the BSSA sys configuration EFI variable
  //
  Status = (*PeiServices)->LocatePpi (PeiServices, &gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, (VOID **)&VarAccessPpiPtr);
  ASSERT_EFI_ERROR (Status);
  ZeroMem (DataBuffer, sizeof(DataBuffer));
  Status = VarAccessPpiPtr->GetVariable(VarAccessPpiPtr, SYS_CONFIG_NAME, &gBiosSsaVariablesGuid, NULL, &DataSize, (VOID *)DataBuffer);
  ASSERT_EFI_ERROR (Status);

  SysConfigHeader = (SSA_BIOS_SYS_CONFIG_HEADER *)AllocatePool (DataSize);
  if (SysConfigHeader == NULL) {
    RcDebugPrint (SDBG_ERROR, "\nEFI_OUT_OF_RESOURCES!!! AllocatePool() returned NULL pointer.\n");
    ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    return;
  }
  CopyMem (SysConfigHeader, DataBuffer, DataSize);

  ConfigCount = SysConfigHeader->ConfigCount;

  //
  // With the new BSSA configure data blob format, multiple test configuration tables can be supported
  // in one data blob. The number of date blob should be 1.
  //
  ASSERT (ConfigCount == 1);
  configIndex = 0;

  // load config
  EvLoadConfig (Host, SysConfigHeader, ConfigCount, configIndex, &ConfigBuffer);
  ConfigVersionHandle = ConfigBuffer;
  ConfigPointerFixupCountHandle = ConfigBuffer + sizeof (UINT32);
  ConfigPointerFixupCount = *(UINT32*)(UINTN)ConfigPointerFixupCountHandle;
  ConfigBlockHandle = ConfigPointerFixupCountHandle + sizeof (UINT32) + (ConfigPointerFixupCount * sizeof (UINT32));

  // run tool
  RcDebugPrint (SDBG_DEFAULT, "About to run EV tool... \n");

  RunEvTestWrapper (Host, (BSSA_CONFIGURE_ENTRY *)(UINTN)ConfigBlockHandle);
}

/**
  Wrapper for running EV test.

  @param[in] Host               - Pointer to the system structure
  @param[in] ConfigureDataBlob  - Pointer to the configure data blob

**/
VOID
RunEvTestWrapper (
  IN PSYSHOST Host,
  IN BSSA_CONFIGURE_ENTRY *ConfigureDataBlob
  )
{
  EFI_PEI_SERVICES **PeiServices = NULL;
  SSA_BIOS_SERVICES_PPI *SsaServicesPpi = NULL;
  SSA_STATUS Status;
  UINT8 NumberOfConfig;
  UINT8 ConfigIndex;
  BSSA_CONFIGURE_HEADER *ConfigBlock;
  VOID *TestCfg;

  PeiServices = (EFI_PEI_SERVICES **)(UINTN)Host->var.common.oemVariable;
  Status = (*PeiServices)->LocatePpi (PeiServices, &gSsaBiosServicesPpiGuid, 0, NULL, &SsaServicesPpi);
  ASSERT_EFI_ERROR (Status);

  //
  // Check input configure and format
  //
  if (IsSsaDataBlob (ConfigureDataBlob) == FALSE) {
    RcDebugPrint (SDBG_DEFAULT, "Invalid configure data blob format  ... \n");
  }

  if (IsChecksumValid (ConfigureDataBlob) == FALSE) {
    RcDebugPrint (SDBG_DEFAULT, "Configure data blob failed checksum  ... \n");
  }

  NumberOfConfig = GetNumberOfConfigBlocks (ConfigureDataBlob);
  RcDebugPrint (SDBG_DEFAULT, "Configure data blob contain %d configuration blocks \n", NumberOfConfig);

  for (ConfigIndex = 0; ConfigIndex < NumberOfConfig; ConfigIndex++) {
    // Clear the ResultsData
    SsaServicesPpi->SsaResultsConfig->ResultsData = NULL;

    //
    // Find the test config then call corresponding EV test content
    //
    GetConfigBlockHandle (ConfigureDataBlob, ConfigIndex, &ConfigBlock);

    ASSERT (ConfigBlock != 0);

    RcDebugPrint (SDBG_DEFAULT, "Configure Block:%d TestType:%d Block Size:%d \n", ConfigIndex, ConfigBlock->Type,
      ConfigBlock->Size);
    GetTestCfg (ConfigBlock, &TestCfg);
    RunEvTest (SsaServicesPpi, ConfigBlock->Type, TestCfg);

    if (SsaServicesPpi->SsaResultsConfig->ResultsData != NULL) {
      SaveResultsToBdat (Host);
    }
  } // ConfigIndex loop
}

/**
  Load/retrieve a test configure blob.

  @param[in] Host               - Pointer to the system structure
  @param[in] SysConfigHeader    - The starting address of the BSSA Syscfg variable structure
  @param[in] ConfigCount        - The total number of the configure blobs
  @param[in] ConfigIndex        - The index of the configure blob to be loaded
  @param[in] ConfigHandle       - The pointer to the buffer where the configure blob to be stored

**/
VOID
EvLoadConfig (
  IN PSYSHOST                    Host,
  IN SSA_BIOS_SYS_CONFIG_HEADER  *SysConfigHeader,
  IN UINT32                      ConfigCount,
  IN UINT32                      ConfigIndex,
  IN UINT32                      *ConfigHandle
  )
{
  UINT32                          ConfigBuffer = 0;
  UINT32                          SysConfigHandle = 0;
  VOID                            *VarNameHeaderPtr = NULL;
  UINT32                          *VarSizePtr = 0;
  CHAR16                          HeadVarName[VAR_NULL_NAME_LENGTH];

  RcDebugPrint (SDBG_DEFAULT, "EvLoadConfig() \n");
  SysConfigHandle = (UINT32) (UINTN) SysConfigHeader;

  //
  // Lookup config variable name and size
  //
  RcDebugPrint (SDBG_DEFAULT, "Lookup config variable name... \n");
  VarNameHeaderPtr = (VOID*)((SysConfigHandle + sizeof(SSA_BIOS_SYS_CONFIG_HEADER)) + (ConfigIndex * VAR_NAME_SIZE));
  ZeroMem(HeadVarName, VAR_NULL_NAME_SIZE);
  CopyMem(HeadVarName, VarNameHeaderPtr, VAR_NAME_SIZE);
  RcDebugPrint (SDBG_DEFAULT, "HeadVarName = %s \n", HeadVarName);

  RcDebugPrint (SDBG_DEFAULT, "Lookup config variable size... \n");
  VarSizePtr = (UINT32 *)((SysConfigHandle + sizeof(SSA_BIOS_SYS_CONFIG_HEADER)) + (ConfigCount * VAR_NAME_SIZE) + (ConfigIndex * sizeof(UINT32)));
  RcDebugPrint (SDBG_DEFAULT, "VarSize = %d \n", *VarSizePtr);

  ConfigBuffer = (UINT32) (UINTN)RcAllocatePool (*VarSizePtr);
  ASSERT (ConfigBuffer);

  GetConfigureFromVariables (Host, &gBiosSsaVariablesGuid, HeadVarName, ConfigBuffer, *VarSizePtr);

  *ConfigHandle = ConfigBuffer;
}

/**
  Copy test configuration from EFI variables to a buffer

  @param[in] Host               - Pointer to the system structure
  @param[in] DestinationHandle  - The starting address of the buffer where configuration data blobs will be stored
  @param[in] SourceGuid         - The GUID of the BSSA configure data variables
  @param[in] SourceName         - The name of the first EFI variable contains the configuration data
  @param[in] DestinationSize    - The size of the buffer where the configure blob to be stored

**/
VOID
GetConfigureFromVariables (
  IN PSYSHOST                  Host,
  IN EFI_GUID                  *SourceVarGuid,
  IN CHAR16                    *SourceVarName,
  IN OUT UINT32                DestinationHandle,
  IN UINT32                    DestinationSize
  )
{
  EFI_PEI_SERVICES                **PeiServices = NULL;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI    *VarAccessPpiPtr = NULL;
  EFI_STATUS  Status;
  UINT8       DataBuffer[VAR_BUFFER_SIZE];
  UINT8 *     DataBufferPtr = DataBuffer;
  UINTN       DataSize = sizeof(DataBuffer);
  UINT32      BytesRead = 0;
  VOID *      DestinationFillPtr = NULL;
  CHAR16      NextVarName[VAR_NULL_NAME_LENGTH];
  UINT32      GetVarIter = 0;
  UINT32      ActualDataSize = 0;

  RcDebugPrint (SDBG_DEFAULT, "Entered CopyVariableListToCache() \n");

  if ((SourceVarGuid == NULL) || (SourceVarName == NULL) || (DestinationSize == 0)) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
  }

  PeiServices = (EFI_PEI_SERVICES **) (UINTN) Host->var.common.oemVariable;
  Status = (*PeiServices)->LocatePpi (PeiServices, &gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, (VOID **)&VarAccessPpiPtr);
  ASSERT_EFI_ERROR (Status);

  Status = VarAccessPpiPtr->GetVariable (VarAccessPpiPtr, SourceVarName, SourceVarGuid, NULL, &DataSize, DataBufferPtr);
  RcDebugPrint (SDBG_DEFAULT, "Just called VarAccessPpiPtr->PeiGetVariable() \n");
  ASSERT_EFI_ERROR(Status);

  RcDebugPrint (SDBG_DEFAULT, "VAR_NAME_SIZE: %d\n", VAR_NAME_SIZE);
  RcDebugPrint (SDBG_DEFAULT, "DataSize: %d\n", DataSize);
  RcDebugPrint (SDBG_DEFAULT, "VAR_BUFFER_SIZE: %d\n", VAR_BUFFER_SIZE);

  //
  // Each variable begins with the name of next variable.
  // 1. Need to extract the next variable name.
  // 2. Copy the variable content to the destination buffer
  //
  ZeroMem (NextVarName, VAR_NULL_NAME_SIZE);
  CopyMem (NextVarName, DataBufferPtr, VAR_NAME_SIZE);
  BytesRead += (UINT32)(DataSize - VAR_NAME_SIZE);

  DestinationFillPtr = (VOID *) (UINTN) DestinationHandle;
  ActualDataSize = (UINT32)(DataSize - VAR_NAME_SIZE);
  ASSERT (((UINT32)(UINTN)DestinationFillPtr + ActualDataSize) <= (DestinationHandle + DestinationSize));
  CopyMem (DestinationFillPtr, (DataBufferPtr + VAR_NAME_SIZE), ActualDataSize);    

  while(StrCmp(NextVarName, NULL_STRING) != 0) {
    DestinationFillPtr = (VOID *)((UINTN)DestinationFillPtr + (DataSize - VAR_NAME_SIZE));  
    GetVarIter++;

    RcDebugPrint (SDBG_DEFAULT, "Iteration: %d\n", GetVarIter);
    RcDebugPrint (SDBG_DEFAULT, "Iteration: %d\n", GetVarIter);
    RcDebugPrint (SDBG_DEFAULT, "DestinationFillPtr = 0x%08X\n", (UINT32) (UINTN) DestinationFillPtr);
    RcDebugPrint (SDBG_DEFAULT, "DestinationHandle = 0x%08X\n", (UINT32) DestinationHandle);
    RcDebugPrint (SDBG_DEFAULT, "DestinationSize = 0x%08X\n", DestinationSize);
    RcDebugPrint (SDBG_DEFAULT, "Is 0x%08X < 0x%08X ?\n", (UINT32) (UINTN) DestinationFillPtr, ((UINT32) DestinationHandle + DestinationSize));

    Status = VarAccessPpiPtr->GetVariable (VarAccessPpiPtr, NextVarName, SourceVarGuid, NULL, &DataSize, DataBufferPtr);
    ASSERT_EFI_ERROR(Status);
    ZeroMem (NextVarName, VAR_NULL_NAME_SIZE);
    CopyMem (NextVarName, DataBufferPtr, VAR_NAME_SIZE);
    RcDebugPrint (SDBG_DEFAULT, "NextVarName = %s \n", NextVarName);
    ActualDataSize = (UINT32)(DataSize - VAR_NAME_SIZE);
    RcDebugPrint (SDBG_DEFAULT, "ActualDataSize = %d \n", ActualDataSize);
    BytesRead += ActualDataSize;

    if(ActualDataSize > 0) {
      ASSERT (((UINT32) (UINTN) DestinationFillPtr + ActualDataSize) <=  (DestinationHandle + DestinationSize));
      CopyMem (DestinationFillPtr, (DataBufferPtr + VAR_NAME_SIZE), ActualDataSize);    
    }
  }

  RcDebugPrint (SDBG_DEFAULT, "ASSERT(BytesRead: %d == DestinationSize: %d) \n", BytesRead, DestinationSize);
  ASSERT(BytesRead == DestinationSize);
}

/**
  Save BSSA results to BDAT

  @param[in] Host    -   Pointer to the system structure

**/
VOID
SaveResultsToBdat (
  IN PSYSHOST  Host
  )
{
  EFI_PEI_SERVICES                **PeiServices = NULL;
  SSA_BIOS_SERVICES_PPI           *SsaServicesPpi  = NULL;
  UINT8 *                         ResultsData = NULL;
  RESULTS_DATA_HDR *              ResultsDataHdr = NULL;
  UINT32                          ResultsDataSize = 0;
  EFI_STATUS                      RetStatus;

  RcDebugPrint (SDBG_DEFAULT, "Sending results via SaveToBdat()... \n");

  //
  // Get PEI and BSSA Services
  //
  PeiServices = (EFI_PEI_SERVICES **) (UINTN) Host->var.common.oemVariable;
  RetStatus = (*PeiServices)->LocatePpi (PeiServices, &gSsaBiosServicesPpiGuid, 0, NULL, &SsaServicesPpi);
  ASSERT_EFI_ERROR (RetStatus);
  ResultsData = (UINT8*)SsaServicesPpi->SsaResultsConfig->ResultsData;
  ResultsDataHdr = (RESULTS_DATA_HDR *) ResultsData;

  //
  //Trim the unused result entries.
  //
  ResultsDataHdr->RsBlock.ResultCapacity = ResultsDataHdr->RsBlock.ResultElementCount;

  ResultsDataSize = sizeof (RESULTS_DATA_HDR) + ResultsDataHdr->MdBlock.MetadataSize + (ResultsDataHdr->RsBlock.ResultElementSize * ResultsDataHdr->RsBlock.ResultCapacity);
  RetStatus = SsaServicesPpi->SsaCommonConfig->SaveToBdat (SsaServicesPpi, &gBiosSsaResultsGuid, ResultsData, ResultsDataSize);

  //
  //Freeing up the temporary memory range in the heap that was used to save test results
  //
  SsaServicesPpi->SsaCommonConfig->Free(SsaServicesPpi, (VOID*)ResultsData);
  SsaServicesPpi->SsaResultsConfig->ResultsData = NULL;
}
