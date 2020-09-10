/**@file
  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef __SECURITY_POLICY_DEFINITIONS_H__
#define __SECURITY_POLICY_DEFINITIONS_H__

//
// Security Policy definitions
// - to be adopted in HII setup menu for consistency
//
// Values for capable/incapable == supported/unsupported
#define SECURITY_POLICY_UNSUPPORTED                0
#define SECURITY_POLICY_SUPPORTED                  1

// Values for enable/disable options
#define SECURITY_POLICY_DISABLE                    0
#define SECURITY_POLICY_ENABLE                     1
#define SECURITY_POLICY_AUTO                       2
#define SECURITY_POLICY_FUSA_SAF_SGX_ENABLED       4

// Values for multichoice options
#define SECURITY_POLICY_PRMRR_SIZE_64MB      0x0004000000
#define SECURITY_POLICY_PRMRR_SIZE_128MB     0x0008000000
#define SECURITY_POLICY_PRMRR_SIZE_256MB     0x0010000000
#define SECURITY_POLICY_PRMRR_SIZE_512MB     0x0020000000
#define SECURITY_POLICY_PRMRR_SIZE_1GB       0x0040000000
#define SECURITY_POLICY_PRMRR_SIZE_2GB       0x0080000000
#define SECURITY_POLICY_PRMRR_SIZE_4GB       0x0100000000
#define SECURITY_POLICY_PRMRR_SIZE_8GB       0x0200000000
#define SECURITY_POLICY_PRMRR_SIZE_16GB      0x0400000000
#define SECURITY_POLICY_PRMRR_SIZE_32GB      0x0800000000
#define SECURITY_POLICY_PRMRR_SIZE_64GB      0x1000000000
#define SECURITY_POLICY_PRMRR_SIZE_128GB     0x2000000000
#define SECURITY_POLICY_PRMRR_SIZE_256GB     0x4000000000
#define SECURITY_POLICY_PRMRR_SIZE_512GB     0x8000000000
#define SECURITY_POLICY_SGX_OWNER_EPOCH_ACTIVATED      0
#define SECURITY_POLICY_SGX_CHANGE_TO_NEW_RANDOM_EPOCH 1
#define SECURITY_POLICY_SGX_USER_MANUAL_EPOCH          2

// DFX - design for X
#define SECURITY_POLICY_DFX_DISABLE                0
#define SECURITY_POLICY_DFX_ENABLE                 1
#define SECURITY_POLICY_DFX_AUTO                   2
#define SECURITY_POLICY_DFX_FUSA_SAF_SGX_ENABLED   4
#define SECURITY_POLICY_DFX_SBX_SERVER             0
#define SECURITY_POLICY_DFX_PRX_SERVER             1

//
// Security setup menu
// - only definitions used in setup menu visual form items
//
#define SECURITY_POLICY_HIDE                    1
#define SECURITY_POLICY_SHOW                    0

#endif
