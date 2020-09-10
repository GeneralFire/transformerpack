/** @file
  RAS Nvram Struct Definition.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _RAS_NVRAM_STRUCT_H_
#define _RAS_NVRAM_STRUCT_H_

#include <SysHost.h>
#include <SiliconSetting.h>

#define  EFI_MEM_RAS_S3_SAVE_DATA_VARIABLE_NAME  L"ImcRasS3SaveData"

//
// DIMM rank info
// List indexed by rank number
//
typedef struct S3ddrRank {
  UINT8   phyRank;                    ///< Physical Rank #(3:0)
  UINT8   logicalRank;                ///< Logical Rank number (0 - 7)
  UINT8   devTagInfo;
} S3DDR_RANK_STRUCT;

//
//DIMM information stored in NVRAM
//
typedef struct S3dimmNvram {
  struct S3ddrRank  rankList[MAX_RANK_DIMM * SUB_CH];
} S3DIMM_NVRAM_STRUCT;
//
// Channel information stored in NVRAM
//
typedef struct S3channelNvram {
  UINT8             chFailed;           ///< ddr4 memory in this channel had failed (MFO)
  UINT8             ngnChFailed;        ///< NGN memory in this channel had failed (MFO)
  S3DIMM_NVRAM_STRUCT dimmList[MAX_DIMM];
  UINT32            rankErrCountStatus;
  UINT32            rankErrCountInfo[(MAX_RANK_CH * SUB_CH)/2];
  UINT32            rankErrThresholdInfo[(MAX_RANK_CH * SUB_CH)/2];
  UINT8             spareInUse;
  UINT8             oldSparePhysicalRank;
  UINT32            SmiSpareCtlMcMainExt;
  UINT8             EccModeMcDecs[(MAX_RANK_CH * SUB_CH)];                                                                    \
  UINT8             EccModeMcMain[(MAX_RANK_CH * SUB_CH)];                                                                    \
  UINT32            Plus1Failover[(MAX_RANK_CH * SUB_CH)];                                                                    \
  UINT32            LinkRetryErrLimits;                                                                            \
  UINT32            LinkLinkFail;
} S3CHANNEL_NVRAM_STRUCT, *S3PCHANNEL_NVRAM_STRUCT;

//
// IMC information stored in NVRAM
//
typedef struct S3imcNvram {
  UINT8               EmcaLtCtlMcMainExt;
  UINT32              ExRasConfigHaCfg;
  UINT32              SmiSpareCtlMcMainExt;
} S3IMC_NVRAM_STRUCT;

//
// Socket information stored in NVRAM
//
typedef struct S3socketNvram {
  S3CHANNEL_NVRAM_STRUCT  channelList[IMC_MAX_CH];
  S3IMC_NVRAM_STRUCT      imc[MAX_IMC];
  UINT32                  smiCtrlUboxMisc;
} S3SOCKET_NVRAM_STRUCT;

typedef struct S3memNvram {
  S3SOCKET_NVRAM_STRUCT socket[MAX_SOCKET];
} S3MEM_NVRAM_STRUCT;

//
//used for Save IMC RAS Runtime setting
//
typedef struct  {
  UINT8 ChFailed[MC_MAX_NODE][MAX_MC_CH];
  UINT8 NgnChFailed[MC_MAX_NODE][MAX_MC_CH];
  UINT8 SpareLogicalRank[MC_MAX_NODE][MAX_MC_CH];
  UINT8 SpareInUse[MC_MAX_NODE][MAX_MC_CH];
  UINT8 OldLogicalRank[MC_MAX_NODE][MAX_MC_CH];
  UINT8 MultiSpareRank[MC_MAX_NODE][MAX_MC_CH];
  UINT8 devTag[MC_MAX_NODE][MAX_MC_CH][MAX_RANK_CH];
  S3MEM_NVRAM_STRUCT    mem;
} IMC_RAS_S3_SAVE;

#endif // #ifndef _RAS_NVRAM_STRUCT_H_
