/** @file
  Interface header file for the CHA IP library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _CHA_REGS_H_
#define _CHA_REGS_H_

#include <RcRegs.h>

typedef enum _TOPOLOGY_UPI_AFFINITY_ENUM_ {
  UPI_AFFINITY_2C2L = 0,
  UPI_AFFINITY_4C2L,
  UPI_AFFINITY_2C4L,
  UPI_AFFINITY_4C4L,
  UPI_AFFINITY_MAX
} TOPOLOGY_UPI_AFFINITY_ENUM;

#ifdef GNRSRF_HOST
//
// CSR name aliasing to workaround register name differences in GNR/SRF
// The registers below are used in ProcMemInit/Chip10nm/Include/PerformanceValueTables.h
//
#define MESEG_BASE_N0_CHABC_SAD_REG                       MESEG_BASE_CFG_N0_CHABC_SAD_REG
typedef MESEG_BASE_CFG_N0_CHABC_SAD_STRUCT                MESEG_BASE_N0_CHABC_SAD_STRUCT;
#define MESEG_BASE_N1_CHABC_SAD_REG                       MESEG_BASE_CFG_N1_CHABC_SAD_REG
typedef MESEG_BASE_CFG_N1_CHABC_SAD_STRUCT                MESEG_BASE_N1_CHABC_SAD_STRUCT;
#define MESEG_LIMIT_N0_CHABC_SAD_REG                      MESEG_LIMIT_CFG_N0_CHABC_SAD_REG
typedef MESEG_LIMIT_CFG_N0_CHABC_SAD_STRUCT               MESEG_LIMIT_N0_CHABC_SAD_STRUCT;
#define MESEG_LIMIT_N1_CHABC_SAD_REG                      MESEG_LIMIT_CFG_N1_CHABC_SAD_REG
typedef MESEG_LIMIT_CFG_N1_CHABC_SAD_STRUCT               MESEG_LIMIT_N1_CHABC_SAD_STRUCT;
#define CBO_COH_CONFIG_CHA_PMA_REG                        CBO_COH_CONFIG_CFG_CHA_PMA_REG
#define CBO_COH_CONFIG1_CHA_PMA_REG                       CBO_COH_CONFIG1_CFG_CHA_PMA_REG
#define CBO_ISOC_CONFIG_CHA_PMA_REG                       CBO_ISOC_CONFIG_CFG_CHA_PMA_REG
#define DBP_CONFIG_CHA_PMA_REG                            DBP_CONFIG_CFG_CHA_PMA_REG
#define HA_COH_CHABC_SAD1_REG                             HA_COH_CFG_CHABC_SAD1_REG
#define HA_OSB_CHABC_SAD1_REG                             HA_OSB_CFG_CHABC_SAD1_REG
#define HA_THRESHOLDS_CHABC_SAD1_REG                      HA_THRESHOLDS_CFG_CHABC_SAD1_REG
#define LOWBWMEM_LOC_TOR_THRESHOLDS_CHABC_SAD1_REG        LOWBWMEM_LOC_TOR_THRESHOLDS_CFG_CHABC_SAD1_REG
#define LOWBWMEM_LOCTOREM_TOR_THRESHOLDS_CHABC_SAD1_REG   LOWBWMEM_LOCTOREM_TOR_THRESHOLDS_CFG_CHABC_SAD1_REG
#define SSLLC_ALLOC_POLICY_CHABC_SAD1_REG                 SSLLC_ALLOC_POLICY_CFG_CHABC_SAD1_REG

//
// The following registers are used in ServerRasPkg/Library10nm/SmmRasSiliconLib10nm/RasCha.c
//
#define SADDBGMM0_CHA_MISC_REG         SADDBGMM0_CFG_CHA_MISC_REG
typedef SADDBGMM0_CFG_CHA_MISC_STRUCT  SADDBGMM0_CHA_MISC_STRUCT;
#define SADDBGMM1_CHA_MISC_REG         SADDBGMM1_CFG_CHA_MISC_REG
typedef SADDBGMM1_CFG_CHA_MISC_STRUCT  SADDBGMM1_CHA_MISC_STRUCT;
#define SADDBGMM2_CHA_MISC_REG         SADDBGMM2_CFG_CHA_MISC_REG
typedef SADDBGMM2_CFG_CHA_MISC_STRUCT  SADDBGMM2_CHA_MISC_STRUCT;

//
// Register used in ServerRasPkg/WheaErrorInj/CoreErrorInj.c:
//
#define LLCERRINJ_CHA_MISC_REG          LLCERRINJ_CFG_CHA_MISC_REG
typedef LLCERRINJ_CFG_CHA_MISC_STRUCT   LLCERRINJ_CHA_MISC_STRUCT;

//
// Registers used in ServerSiliconPkg/Ras/LibrarySpr/AddressDecodeLib/ChaDecodeInit.c
//
#define H0_TGT_ROUTE_TABLE_0_CHA_MISC_REG            H0_TGT_ROUTE_TABLE_0_CFG_CHA_MISC_REG
typedef H0_TGT_ROUTE_TABLE_0_CFG_CHA_MISC_STRUCT     H0_TGT_ROUTE_TABLE_0_CHA_MISC_STRUCT;
#define H0_CH_ROUTE_TABLE_0_CHA_MISC_REG             H0_CH_ROUTE_TABLE_0_CFG_CHA_MISC_REG
typedef H0_CH_ROUTE_TABLE_0_CFG_CHA_MISC_STRUCT      H0_CH_ROUTE_TABLE_0_CHA_MISC_STRUCT;
#define H0_TGT_ROUTE_TABLE_1_CHA_MISC_REG            H0_TGT_ROUTE_TABLE_1_CFG_CHA_MISC_REG
#define H0_CH_ROUTE_TABLE_1_CHA_MISC_REG             H0_CH_ROUTE_TABLE_1_CFG_CHA_MISC_REG
#define H0_TGT_ROUTE_TABLE_2_CHA_MISC_REG            H0_TGT_ROUTE_TABLE_2_CFG_CHA_MISC_REG
#define H0_CH_ROUTE_TABLE_2_CHA_MISC_REG             H0_CH_ROUTE_TABLE_2_CFG_CHA_MISC_REG
#define H0_TGT_ROUTE_TABLE_2LM_CHA_MISC_REG          H0_TGT_ROUTE_TABLE_2LM_CFG_CHA_MISC_REG
#define H1_TGT_ROUTE_TABLE_0_CHA_MISC_REG            H1_TGT_ROUTE_TABLE_0_CFG_CHA_MISC_REG
#define H1_CH_ROUTE_TABLE_0_CHA_MISC_REG             H1_CH_ROUTE_TABLE_0_CFG_CHA_MISC_REG
#define H1_TGT_ROUTE_TABLE_1_CHA_MISC_REG            H1_TGT_ROUTE_TABLE_1_CFG_CHA_MISC_REG
#define H1_CH_ROUTE_TABLE_1_CHA_MISC_REG             H1_CH_ROUTE_TABLE_1_CFG_CHA_MISC_REG
#define H1_TGT_ROUTE_TABLE_2_CHA_MISC_REG            H1_TGT_ROUTE_TABLE_2_CFG_CHA_MISC_REG
#define H1_CH_ROUTE_TABLE_2_CHA_MISC_REG             H1_CH_ROUTE_TABLE_2_CFG_CHA_MISC_REG
#define H1_TGT_ROUTE_TABLE_2LM_CHA_MISC_REG          H1_TGT_ROUTE_TABLE_2LM_CFG_CHA_MISC_REG
#define H1_CH_ROUTE_TABLE_2_CHA_MISC_REG             H1_CH_ROUTE_TABLE_2_CFG_CHA_MISC_REG
#define BLOCK_DECODER_ADDR_N0_CHABC_SAD1_REG         BLOCK_DECODER_ADDR_CFG_N0_CHABC_SAD1_REG
typedef BLOCK_DECODER_ADDR_CFG_N0_CHABC_SAD1_STRUCT  BLOCK_DECODER_ADDR_N0_CHABC_SAD1_STRUCT;
#define BLOCK_DECODER_ADDR_N1_CHABC_SAD1_REG         BLOCK_DECODER_ADDR_CFG_N1_CHABC_SAD1_REG
typedef BLOCK_DECODER_ADDR_CFG_N1_CHABC_SAD1_STRUCT  BLOCK_DECODER_ADDR_N1_CHABC_SAD1_STRUCT;

#endif // GNRSRF_HOST

#endif // _CHA_REGS_H_

