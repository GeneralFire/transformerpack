/** @file
  This file contains sample definitions for platform setup.

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

#ifndef _SETUP_H_
#define _SETUP_H_

#define SETUP_VARIABLE_GUID { \
  0xebd5f304, 0xa278, 0x4ef0, { 0xa2, 0x6, 0x63, 0xd7, 0x20, 0x29, 0x31, 0x60 } \
  }

#define SETUP_VARIABLE_NAME L"Setup"

#pragma pack(1)
typedef struct {
  UINT8  TpmEnable;
  UINT8  VmxInSmxEnable;
  UINT8  VmxOutSmxEnable;
  UINT8  SenterEnable;
  UINT8  VTdEnable;
  UINT8  TxtEnable;
  UINT8  StmEnable;
} SYSTEM_CONFIGURATION;
#pragma pack()

extern EFI_GUID gSetupVariableGuid;

#endif
