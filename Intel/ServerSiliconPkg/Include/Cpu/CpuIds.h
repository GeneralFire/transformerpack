/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _CPUID_REGS_H_
#define _CPUID_REGS_H_

#define CPU_FAMILY_SKX     0x5065      // Skylake/CascadeLake CPU
#define CPU_FAMILY_ICX     0x606A      // IceLake CPU
#define CPU_FAMILY_ICXD    0x606C      // IceLake-D CPU
#define CPU_FAMILY_SNR     0x8066      // SnowRidge
#define CPU_FAMILY_SPRSP   0x806F      // SPR-SP CPU
#define CPU_FAMILY_GNRSP   0xA06D      // GNR-SP CPU
#define CPU_FAMILY_SRFSP   0xA06F      // SRF-SP CPU

//
// Processor Family and Model information.
//
#define NEHALEM_FAMILY_ID                 0x06

#define SKYLAKE_SERVER_MODEL_ID           0x55
#define ICELAKE_SERVER_MODEL_ID           0x6A
#define ICELAKE_D_SERVER_MODEL_ID         0x6C
#define SAPPHIRERAPIDS_SERVER_MODEL_ID    0x8F
#define SNOWRIDGE_SERVER_MODEL_ID         0x86
#define GRANITERAPIDS_SERVER_MODEL_ID     0xAD
#define SIERRAFOREST_SERVER_MODEL_ID      0xAF

#define IS_SKYLAKE_SERVER_PROC(FamilyId, ModelId) \
  (FamilyId == NEHALEM_FAMILY_ID && \
   ( \
    ModelId == SKYLAKE_SERVER_MODEL_ID \
    ) \
   )

#define IS_ICELAKE_SERVER_PROC(FamilyId, ModelId) \
  (FamilyId == NEHALEM_FAMILY_ID && \
   ( \
    ModelId == ICELAKE_SERVER_MODEL_ID \
    ) \
   )

#define IS_ICELAKE_D_SERVER_PROC(FamilyId, ModelId) \
  (FamilyId == NEHALEM_FAMILY_ID && \
   ( \
    ModelId == ICELAKE_D_SERVER_MODEL_ID \
    ) \
   )

#define IS_SAPPHIRERAPIDS_SERVER_PROC(FamilyId, ModelId) \
  (FamilyId == NEHALEM_FAMILY_ID && \
   ( \
    ModelId == SAPPHIRERAPIDS_SERVER_MODEL_ID \
    ) \
   )

#define IS_SNOWRIDGE_SERVER_PROC(FamilyId, ModelId) \
  (FamilyId == NEHALEM_FAMILY_ID && \
   ( \
    ModelId == SNOWRIDGE_SERVER_MODEL_ID \
    ) \
   )

#define IS_GRANITERAPIDS_SERVER_PROC(FamilyId, ModelId) \
  (FamilyId == NEHALEM_FAMILY_ID && \
   ( \
    ModelId == GRANITERAPIDS_SERVER_MODEL_ID \
    ) \
   )

#define IS_SIERRAFOREST_SERVER_PROC(FamilyId, ModelId) \
  (FamilyId == NEHALEM_FAMILY_ID && \
   ( \
    ModelId == SIERRAFOREST_SERVER_MODEL_ID \
    ) \
   )

//
// Local APIC defines
//
#define B_INTERRUPT_MASK                      (1 << 16)
#define B_DELIVERY_MODE                       (0x07 << 8)
#define V_MODE_SMI                            (0x02 << 8)
#define B_VECTOR                              (0xFF << 0)

#define EFI_CACHE_UNCACHEABLE                 0
#define EFI_CACHE_WRITECOMBINING              1
#define EFI_CACHE_WRITETHROUGH                4
#define EFI_CACHE_WRITEPROTECTED              5
#define EFI_CACHE_WRITEBACK                   6

#define APICID_MASK_BIT14_8 0x7F    //current Si support programmable APICID up to 15bits
#endif  //_CPUID_REGS_H_
