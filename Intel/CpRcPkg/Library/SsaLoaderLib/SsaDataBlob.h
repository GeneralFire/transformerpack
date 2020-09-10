/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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

#ifndef _SSA_DATA_BLOB_H
#define _SSA_DATA_BLOB_H

//
// BSSA EV test result storage type. They can be mixed.
//
#define BSSA_TEST_RESULT_IN_BDAT                0x1
#define BSSA_TEST_RESULT_IN_SHARED_MEMORY       0x2
#define BSSA_TEST_RESULT_IN_SERIAL_LOG          0x4

#pragma pack(push,1)

typedef struct {
  UINT8 Signature[4];
  UINT8 CheckSum;
  UINT8 EntryStructSize;
  UINT8 MajorVersion;
  UINT8 MinorVersion;
  UINT8 ResultStorage;
  UINT8 NumberOfConfigurations;
  UINT16 TotalSize;
} BSSA_CONFIGURE_ENTRY;

typedef struct {
  UINT8 Type;
  UINT16 Size;
  UINT8 Handle;
} BSSA_CONFIGURE_HEADER;

#pragma pack(pop)

/**
  Check if the configuration data blob matches to the new BSSA configure format defined 
  by the BSSA interface spec.

  If a configuration data blob is not in the new format then the BSSA loader will treat the 
  data as the previous version test configure.

  @param[in] Data - Pointer to configuration data structure

  retval TRUE -  Data blob conforms to the new format.
  retval FALSE - Data blob does not conform to the new format.
**/
BOOLEAN
IsSsaDataBlob (
  IN BSSA_CONFIGURE_ENTRY *Data
  );

/**
  Check the integrity of the configure data blob by applying checksum.

  @param[in] Data - Pointer to configuration data structure

  retval TRUE -  Valid.
  retval FALSE - Invalid.
**/
BOOLEAN
IsChecksumValid (
  IN BSSA_CONFIGURE_ENTRY *Data
  );

/**
  Get the number of test configuration structure in the data blob

  @param[in] Data - Pointer to configuration data blob structure

  retval The number of data blocks in the data blob
**/
UINT8
GetNumberOfConfigBlocks (
  IN BSSA_CONFIGURE_ENTRY *Data
  );

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
  );

/**
  Get the start address of a test configuration (without the header) from a test configure data structure

  @param[in] ConfigBlockHandle - Pointer to configure struture
  @param[out] ConfigHandle - Pointer to the test configuration

  retval N/A
**/
VOID
GetTestCfg (
  BSSA_CONFIGURE_HEADER *ConfigBlockHandle,
  OUT VOID **ConfigHanlde
  );

#endif   //_SSA_DATA_BLOB_H
