/** @file
  This file contains definitions required for One-Touch function.

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

#ifndef _TXT_ONE_TOUCH_DATA_H_
#define _TXT_ONE_TOUCH_DATA_H_

#define TXT_ONE_TOUCH_FLAGS_GUID \
  { \
    0x6b32632b, 0xeddf, 0x4288, { 0x83, 0x56, 0x1b, 0xb8, 0xbc, 0x83, 0x3b, 0xc3 }\
  }

//
// This variable is used to save TXT Management Flags and corresponding operations.
// It should be protected from malicious software (e.g. Set it as read-only variable). 
//
#define TXT_ONE_TOUCH_FLAGS_VARIABLE  L"TxtOneTouchFlagsVariable"
typedef struct {
  UINT8  Flags;
} TXT_ONE_TOUCH_FLAGS;

extern EFI_GUID  gTxtOneTouchFlagsGuid;



//
// Intel TXT Control Flags
//
#define TXT_ONETOUCH_CONTROL_FLAG_NO_TXT_MAINTENANCE        BIT0
#define TXT_ONETOUCH_CONTROL_FLAG_NO_TXT_REFURBISHING       BIT1

//
// Operation
//
#define SET_NO_TXT_MAINTENANCE_FALSE                        128
#define SET_NO_TXT_MAINTENANCE_TRUE                         129

//
// 0-> Disable
// 1-> Enable
// X-> Don't touch
//

//
// OEN setup Variable operations
//
#define TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_X__TXT_X__STM_X 130
#define TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_X__TXT_0__STM_0 131
#define TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_1__TXT_X__STM_X 132
#define TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_0__TXT_0__STM_0 133
#define TPM_X__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_X__STM_X 134
#define TPM_X__VMXINSMX_0__VMXOUTSMX_0__VTD_0__TXT_0__STM_0 135
#define TPM_1__VMXINSMX_1__VMXOUTSMX_0__VTD_1__TXT_1__STM_0 136
#define TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_0 137
#define TPM_1__VMXINSMX_1__VMXOUTSMX_1__VTD_1__TXT_1__STM_1 138
#define TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_X__STM_0 139
#define TPM_X__VMXINSMX_X__VMXOUTSMX_X__VTD_X__TXT_0__STM_0 140

//
// direct register operations
//
#define SENTER_0__VMXINSMX_0__VMXOUTSMX_0                   150
#define SENTER_0__VMXINSMX_1__VMXOUTSMX_0                   151
#define SENTER_0__VMXINSMX_0__VMXOUTSMX_1                   152
#define SENTER_0__VMXINSMX_1__VMXOUTSMX_1                   153
#define SENTER_1__VMXINSMX_0__VMXOUTSMX_0                   154
#define SENTER_1__VMXINSMX_1__VMXOUTSMX_0                   155
#define SENTER_1__VMXINSMX_0__VMXOUTSMX_1                   156
#define SENTER_1__VMXINSMX_1__VMXOUTSMX_1                   157

//
// Refurbishing commands
//
#define SET_NO_TXT_REFURBISHING_FALSE                       170
#define SET_NO_TXT_REFURBISHING_TRUE                        171

#define EXEC_BOISAC_RESET_ESTABLISHMENT_BIT                 172
#define EXEC_BOISAC_RESET_AUX_INDEX                         173

//
//-------------	Extending PPI-X Spec for One-Touch support -----------------------
// One-Touch Activation/Deactivation operations
#define GET_SYSTEM_FEATURES                                 180
#define   PP_GET_SYSTEM_FEATURES_SIGNATURE	0x5346		// Bit31-16 = 'SF' signature
#define	  PP_SYSTEM_FEATURE_TXT			  0x01		//   Bit-0   = 0/1, TXT not/yes supported
#define	  PP_SYSTEM_FEATURE_TPM			  0x02		//   Bit-1   = 0/1, TPM not/yes supported
								//   Bit15-2...Reserved for future use (set to 0)
#define	  PP_SYSTEM_FEATURES_SUPPORTED	(PP_SYSTEM_FEATURE_TXT | PP_SYSTEM_FEATURE_TPM)	// Bit-mapped information of supported system features
#define ACTIVATE_SYSTEM_FEATURES                            181
#define DEACTIVATE_SYSTEM_FEATURES                          182

//--------------------------------------------------------------------------------
//


#endif
