/** @file

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

#ifndef _PLATFORM_VARIABLE_COMMON_H_
#define _PLATFORM_VARIABLE_COMMON_H_


#ifndef PLATFORM_SETUP_VARIABLE_NAME
#define PLATFORM_SETUP_VARIABLE_NAME             L"Setup"
#endif

#define CMOS_CLEAR_EVENT        BIT0
#define CMOS_BAD_CHECKSUM_EVENT BIT1
#define MFG_MODE_EVENT          BIT2
#define RECOVERY_MODE_EVENT     BIT3
#define LOAD_FAILSAFE_EVENT     BIT4
#define LOAD_CUSTOMIZED_EVENT   BIT5
#define NULL_VARIABLE_EVENT     BIT6

#define PLATFORM_VARIABLE_HOB_GUID { 0x71e6d4bc, 0x4837, 0x45f1, { 0xa2, 0xd7, 0x3f, 0x93, 0x8, 0xb1, 0x7e, 0xd7 } }

extern EFI_GUID gPlatformVariableHobGuid;

#define LOAD_FAILSAFE_VARIABLE_NAME        L"LoadEPSDConfigurationDefaults"
#define LOAD_CUSTOMIZED_VARIABLE_NAME   L"LoadSystemConfigurationDefaults"

#endif

