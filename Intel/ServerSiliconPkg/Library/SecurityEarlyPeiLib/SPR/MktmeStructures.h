/** @file
MKTME Early flow

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

#ifndef _MKTME_STRUCTURES_H_
#define _MKTME_STRUCTURES_H_

#include <ReferenceCodeDataTypes.h>
#include <Cpu/Msr/SprMsr.h>
#include <Cpu/Msr/CommonMsr.h>
#include <Library/MemoryCoreLib.h>

#define MAX_KEYS                   0x80
#define TMECPUIDBIT                BIT13
#define MKTME_INTEGRITY_BIT        BIT49
#define MAX_EXCLUSION_RANGE        100
#define UINT64LOMASK               0x00000000FFFFFFFF
#define UINT64HIMASK               0xFFFFFFFF00000000
#define MKTMECPUID_1B              0x1b

typedef union {
  SPR_MSR_TME_ACTIVATE_REGISTER Reg;
  UINT64_STRUCT Data;
} TME_ACTIVATE_TYPE;

typedef union {
  SPR_MSR_TME_CAPABILITY_REGISTER Reg;
  UINT64_STRUCT Data;
} TME_CAPABILITY_TYPE;

typedef union {
  SPR_MSR_TME_EXCLUDE_MASK_REGISTER Reg;
  UINT64_STRUCT Data;
} TME_EXCLUDE_MASK_TYPE;

typedef union {
  SPR_MSR_TME_EXCLUDE_BASE_REGISTER Reg;
  UINT64_STRUCT Data;
} TME_EXCLUDE_BASE_TYPE;

typedef union {
  MSR_UNCORE_RATIO_LIMIT_REGISTER Reg;
  UINT64_STRUCT Data;
} UNCORE_RATIO_LIMIT_TYPE;

typedef struct {
  struct {
    UINT64 Key0;
    UINT64 Key1;
  }
  Key[MAX_SOCKET];
} TMEKEY;

#endif // _MKTME_STRUCTURES_H_
