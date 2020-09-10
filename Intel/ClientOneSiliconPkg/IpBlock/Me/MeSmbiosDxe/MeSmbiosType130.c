/**@file
  Type 130: AMT-Specific SMBIOS Structure.

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
// Structure for AMT-specific SMBIOS type 130 filled with default data.
//
GLOBAL_REMOVE_IF_UNREFERENCED SMBIOS_TABLE_TYPE130 mSmbiosTableType130Data = {
  {
    MESMBIOS_TYPE_130,              // Smbios type
    sizeof (SMBIOS_TABLE_TYPE130),  // Data Length
    0x00                            // handle value is dummy and is filled by Smbios Core driver
  },
  { '$','A','M','T' },              // AMT Signature
  0,                                // AMT Supported?
  0,                                // AMT Enabled?
  0,                                // SRoU Enabled?
  0,                                // Serial Over Lan Enabled?
  0,                                // Network interface of the Intel AMT part is enabled?
  0xA5,                             // 0xA5 (for backward compatibility)
  0xE3,                             // 1st Byte of OEM capabilities - bits 0-1 reserved (must be 0x01), 2 BIOS Reflash, 3 BIOS Setup, 4 BIOS Pause, 5-7 reserved (must be 0x01)
  0,                                // 2nd Byte of OEM capabilities (Terminal Emulation), 0x02 = VT100+
  0,                                // 3rd Byte of OEM capabilities - bits 0-5 reserved (must be 0x00), 6 Remote Secure Erase Capability, 7 BIOS Secure Boot Capability,
  0,                                // 4th Byte of OEM capabilities - bits 0 AMT over 2.5GbE, 1-7 reserved (must be 0x00)
  0,                                // Kvm Enabled?
  0                                 // Reserved
};


/**
  This function makes creates the ME SMBIOS table 130 which is filled with default values.

  @retval EFI_SUCCESS           Record was added.
  @retval EFI_OUT_OF_RESOURCES  Record was not added due to lack of system resources.
**/
EFI_STATUS
CreateSmbiosTableType130 (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = AddSmbiosRecord ((VOID*)&mSmbiosTableType130Data, sizeof (SMBIOS_TABLE_TYPE130));
  DEBUG ((DEBUG_INFO, "%a() exit, status = %r\n", __FUNCTION__, Status));

  return Status;
}