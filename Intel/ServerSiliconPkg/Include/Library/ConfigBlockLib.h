/** @file
  Header file for Config Block Lib implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 Intel Corporation. <BR>

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

#ifndef _CONFIG_BLOCK_LIB_H_
#define _CONFIG_BLOCK_LIB_H_

/**
  Create config block table

  @param[in]     TotalSize                    - Max size to be allocated for the Config Block Table
  @param[out]    ConfigBlockTableAddress      - On return, points to a pointer to the beginning of Config Block Table Address

  @retval EFI_INVALID_PARAMETER - Invalid Parameter
  @retval EFI_OUT_OF_RESOURCES  - Out of resources
  @retval EFI_SUCCESS           - Successfully created Config Block Table at ConfigBlockTableAddress
**/
EFI_STATUS
EFIAPI
CreateConfigBlockTable (
  IN     UINT16    TotalSize,
  OUT    VOID      **ConfigBlockTableAddress
  );

/**
  Add config block into config block table structure

  @param[in]     ConfigBlockTableAddress      - A pointer to the beginning of Config Block Table Address
  @param[out]    ConfigBlockAddress           - On return, points to a pointer to the beginning of Config Block Address

  @retval EFI_OUT_OF_RESOURCES - Config Block Table is full and cannot add new Config Block or
                                 Config Block Offset Table is full and cannot add new Config Block.
  @retval EFI_SUCCESS          - Successfully added Config Block
**/
EFI_STATUS
EFIAPI
AddConfigBlock (
  IN     VOID      *ConfigBlockTableAddress,
  OUT    VOID      **ConfigBlockAddress
  );

/**
  Retrieve a specific Config Block data by GUID

  @param[in]      ConfigBlockTableAddress      - A pointer to the beginning of Config Block Table Address
  @param[in]      ConfigBlockGuid              - A pointer to the GUID uses to search specific Config Block
  @param[out]     ConfigBlockAddress           - On return, points to a pointer to the beginning of Config Block Address

  @retval EFI_NOT_FOUND         - Could not find the Config Block
  @retval EFI_SUCCESS           - Config Block found and return
**/
EFI_STATUS
EFIAPI
GetConfigBlock (
  IN     VOID      *ConfigBlockTableAddress,
  IN     EFI_GUID  *ConfigBlockGuid,
  OUT    VOID      **ConfigBlockAddress
  );

#endif // _CONFIG_BLOCK_LIB_H_
