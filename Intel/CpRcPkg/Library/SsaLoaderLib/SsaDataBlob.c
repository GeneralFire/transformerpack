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
#include "SsaDataBlob.h"
#include <Library/MemoryCoreLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>

STATIC
VOID
DisplayEntryPointStruct (
  IN BSSA_CONFIGURE_ENTRY *EntryPointStruct
)
{
  RcDebugPrint (SDBG_DEFAULT, "Display BSSA Configure Entry Point Struct:\n");
  RcDebugPrint (SDBG_DEFAULT, "CheckSum:0x%02x \n", EntryPointStruct->CheckSum);
  RcDebugPrint (SDBG_DEFAULT, "EntryStructSize:%d \n", EntryPointStruct->EntryStructSize);
  RcDebugPrint (SDBG_DEFAULT, "MajorVersion:0x%02x\n", EntryPointStruct->MajorVersion);
  RcDebugPrint (SDBG_DEFAULT, "MinorVersion:0x%02x\n", EntryPointStruct->MinorVersion);
  RcDebugPrint (SDBG_DEFAULT, "ResultStorage:0x%02x\n", EntryPointStruct->ResultStorage);
  RcDebugPrint (SDBG_DEFAULT, "NumberOfConfigure:0x%02x\n", EntryPointStruct->NumberOfConfigurations);
  RcDebugPrint (SDBG_DEFAULT, "TotalSize:0x%02x\n", EntryPointStruct->TotalSize);
}

/**
  Check if the configuration data blob matches to the new BSSA configure format defined
  by the BSSA interface spec.

  If a confiuration data blob is not in the new format then the BSSA loader will treat the
  data as the previous version test configure.

  @param[in] Data - Pointer to configuration data structure

  retval TRUE -  Data blob conforms to the new format.
  retval FALSE - Data blob does not conform to the new format.
**/
BOOLEAN
IsSsaDataBlob (
  IN BSSA_CONFIGURE_ENTRY *Data
  )
{
  //
  // "BSSA" is the "magic" signature
  //
  UINT8 Signature[4] = { 0x42, 0x53, 0x53, 0x41 }; 
  
  if (CompareMem (Signature, (UINT8*)Data, sizeof (Signature)) == 0) {
    DisplayEntryPointStruct (Data);
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Check the integrity of the configure data blob by applying checksum.

  @param[in] Data - Pointer to configuration data structure

  retval TRUE -  Valid.
  retval FALSE - Invalid.
**/
BOOLEAN
IsChecksumValid (
  IN BSSA_CONFIGURE_ENTRY *Data
  )
{
  if (IsSsaDataBlob (Data) == FALSE) {
    return FALSE;
  }

  if (CalculateSum8 ((UINT8*)Data, Data->TotalSize) == 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Get the number of test configuration structure in the data blob

  @param[in] Data - Pointer to configuration data blob structure

  retval The number of data blocks in the data blob
**/
UINT8
GetNumberOfConfigBlocks (
  IN BSSA_CONFIGURE_ENTRY *Data
  )
{
  if (IsSsaDataBlob (Data) == FALSE) {
    return 0;
  }

  return Data->NumberOfConfigurations;
}

/**
  Get the start address of a test configure structure in the data blob

  @param[in]  Data - Pointer to configuration data blob structure
  @param[in]  Index - The index of the configure structure inside the data blob
  @param[out] ConfigBlockHandle - The pointer to the configure structure

  retval N/A
**/
VOID
GetConfigBlockHandle (
  IN BSSA_CONFIGURE_ENTRY *Data,
  IN UINT8 Index,
  OUT BSSA_CONFIGURE_HEADER **ConfigBlockHandle
  )
{
  BSSA_CONFIGURE_HEADER *BlockHeader;
  UINT8 Block;

  if (Index >= GetNumberOfConfigBlocks (Data)) {
    *ConfigBlockHandle = NULL;
  }

  Block = 0;
  BlockHeader = (BSSA_CONFIGURE_HEADER*)((UINTN)Data + sizeof (BSSA_CONFIGURE_ENTRY));

  while (Block < Index) {
    //
    // find the next block 
    //
    BlockHeader = (BSSA_CONFIGURE_HEADER *)((UINTN)BlockHeader + BlockHeader->Size);
  }

  *ConfigBlockHandle = BlockHeader;
}

/**
  Get the start address of a test configuration (without the header) from a test configure data structure

  @param[in] ConfigBlockHandle - Pointer to configure struture
  @param[out] ConfigHandle - Pointer to the test configuration

  retval N/A
**/
VOID
GetTestCfg (
  BSSA_CONFIGURE_HEADER *ConfigBlockHandle,
  OUT VOID **ConfigHandle
)
{
  *ConfigHandle = (VOID*)((UINTN)ConfigBlockHandle + sizeof (BSSA_CONFIGURE_HEADER));
}


