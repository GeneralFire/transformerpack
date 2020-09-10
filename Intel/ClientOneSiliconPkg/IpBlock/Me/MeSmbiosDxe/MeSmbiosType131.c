/**@file
  Type 131: ME SMBIOS Structure.

@copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#include "MeSmbiosInternal.h"

//
// Structure for ME SMBIOS type 131 filled with default data.
//
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_TABLE_TYPE131 mSmbiosTableType131Data = {
  {
    MESMBIOS_TYPE_131,            // Smbios type
    sizeof (SMBIOS_TABLE_TYPE131),  // Data Length
    0x00                          // handle value is dummy and is filled by Smbios Core driver
  },
  { 0 },                          // Cpu Capabilities
  {
    0x00,                         // MEBx Version.Major
    0x00,                         // MEBx Version.Minor
    0x00,                         // MEBx Version.Hotfix
    0x00                          // MEBx Version.Build
  },
  { 0 },                          // Pch Capabilities
  { 0 },                          // Me Capabilities
  { 0 },                          // Me Fw Configured
  { 0 },                          // Network Device
  { 0 },                          // Bios Capabilities
  { 'v','P','r','o' },            // Structure Identifier
  0                               // Reserved
};

/**
  This function makes creates the ME SMBIOS table 131 which is filled with default values.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added due to lack of system resources.
**/
EFI_STATUS
CreateSmbiosTableType131 (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = AddSmbiosRecord ((VOID*)&mSmbiosTableType131Data, sizeof (SMBIOS_TABLE_TYPE131));
  DEBUG ((DEBUG_INFO, "%a() exit, status = %r\n", __FUNCTION__, Status));

  return Status;
}