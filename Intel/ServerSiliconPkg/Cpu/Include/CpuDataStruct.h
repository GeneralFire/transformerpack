/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#ifndef _CPU_DATA_STRUCT_H
#define _CPU_DATA_STRUCT_H

#define inline          __inline

#ifndef MAX_SOCKET
#define MAX_SOCKET (FixedPcdGet32 (PcdMaxCpuSocketCount))
#endif

#ifndef MAX_CORE
#define MAX_CORE (FixedPcdGet32 (PcdMaxCpuCoreCount))
#endif

//
// Total TDP levels for Config TDP + Speed Select (ISS/SST)
//
#define CONFIG_TDP_TOTAL_LEVEL  5

#define CONFIG_TDP_LEVEL_0      0
#define CONFIG_TDP_LEVEL_1      1
#define CONFIG_TDP_LEVEL_2      2

#define MAX_CONFIG_TDP_LEVEL    CONFIG_TDP_LEVEL_2

#define CONFIG_TDP_LEVEL_3      3 // also viewed as SST PP Config 1
#define CONFIG_TDP_LEVEL_4      4 // also viewed as SST PP Config 2

//
// CPU features related definitions
// Make sure the definitions here are not conflicted with the definition
// CPU_FEATURE_XXX in UefiCpuPkg/Include/Library/RegisterCpuFeaturesLib.h.
//
#define CPU_FEATURE_PECI_DOWN_STREAM                25 // CPU_FEATURE_RESERVED_25
#define CPU_FEATURE_L1_NEXT_PAGE_PREFETCHER         26 // CPU_FEATURE_RESERVED_26
#define CPU_FEATURE_DBP_F                           27 // CPU_FEATURE_RESERVED_27
#define CPU_FEATURE_AMP_PREFETCH                    29 // CPU_FEATURE_RESERVED_29
#define CPU_FEATURE_SNC_INIT                        30 // CPU_FEATURE_RESERVED_30
#define CPU_FEATURE_MBM_INIT                        31 // CPU_FEATURE_RESERVED_31
#define CPU_FEATURE_IIO_LLC_WAYS                    (32+0) // Reuse the value of CPU_FEATURE_L2_PREFETCHER which is not used by our code.
#define CPU_FEATURE_MTOI_WA                         (32+13)
#define CPU_FEATURE_AC_SPLIT_LOCK                   (32+14)

typedef struct {
  UINT32  RegEax;
  UINT32  RegEbx;
  UINT32  RegEcx;
  UINT32  RegEdx;
} EFI_CPUID_REGISTER;

#endif
