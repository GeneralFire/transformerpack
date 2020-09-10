/** @file
  Interface of CPU early data library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef __CPU_EARLY_DATA_LIB_H__
#define __CPU_EARLY_DATA_LIB_H__

#include <CpuDataStruct.h>

typedef struct {
  UINT64  FusedCores[MAX_SOCKET];                         // Fused Core Mask in the package 64bits
  UINT64  ActiveCores[MAX_SOCKET];                        // Active Core Mask in the package 64bits
  UINT8   MaxCoreToBusRatio[MAX_SOCKET];                  // Package Max Non-turbo Ratio (per socket)
  UINT8   MinCoreToBusRatio[MAX_SOCKET];                  // Package Maximum Efficiency Ratio (per socket)
  UINT32  PackageBspApicID[MAX_SOCKET];
  UINT8   IssCapableSystem;                               // 1 = All sockets config TDP / ISS capable
  UINT8   ConfigTdpCapableSystem;                         // 1 = All sockets config TDP capable
  UINT8   IssConfigTdpMaxLevel;                           // B2P CONFIG_TDP_GET_LEVELS_INFO
  UINT8   IssConfigTdpCurrentLevel;                       // B2P CONFIG_TDP_GET_LEVELS_INFO
  UINT8   IssConfigTdpRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_TDP_INFO
  UINT16  IssConfigTdpPower[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_TDP_INFO
  UINT32  IssConfigTdpPowerMinMax[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];  // B2P CONFIG_TDP_GET_POWER_INFO
  UINT8   IssConfigTdpTjmax[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P CONFIG_TDP_GET_ICCP_TJMAX_INFO
  UINT8   IssConfigTdpCoreCount[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];    // B2P CONFIG_TDP_GET_CORE_MASK
  UINT64  IssConfigTdpEnabledCoreMask[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];   // B2P CONFIG_TDP_GET_CORE_MASK
  UINT8   PbfCapableSystem;                                         // 1 = All sockets PBF Capable
  UINT8   PbfCapable[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];           // B2P GET_CONFIG_TDP_CONTROL PBF_SUPPORT Bit[1]
  UINT64  PbfP1HiCoreMap[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];     // B2P PBF_GET_CORE_MASK_INFO Bits
  UINT8   PbfP1HighRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];       // B2P PBF_GET_P1HI_P1LO_INFO P1_HI
  UINT8   PbfP1LowRatio[MAX_SOCKET][CONFIG_TDP_TOTAL_LEVEL];        // B2P PBF_GET_P1HI_P1LO_INFO P1_LO
  UINT32  SstCpSystemStatus;
} CPU_VAR_DATA;


/**
  Get the Cpu var data.
  @return NULL can't find the CpuVarData

 **/
CPU_VAR_DATA *
GetCpuVarData (
  );

#endif
