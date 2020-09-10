/** @file
  Routes to register includes

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
#ifndef _MEM_MAP_HOST_H_
#define _MEM_MAP_HOST_H_

#include <Include/MemHostChipCommon.h>

#define NGN_DIMM_MAX_NUM_OF_CFG_RECORDS (MAX_SYS_CH * MAX_DCPMM_CH)

#define DCPMM_CFG_CUR_UIDS_VALID    0x1    // TRUE
#define DCPMM_CFG_CUR_UIDS_INVALID  0x0    // FALSE
#define DCPMM_DIMM_PRESENT          0x2
#define DCPMM_DIMM_NOT_PRESENT      0x0

#define NGN_FFW_BANK_NUM            2
#ifdef DDR5_SUPPORT
#define NGN_FFW_CSR_NUM             12
#else
#define NGN_FFW_CSR_NUM             8
#endif

typedef enum {
  NGN_DIMM_CFG_CUR_REC = 0,
  NGN_DIMM_CFG_OUT_REC,
  NGN_DIMM_CFG_ERROR_REC,
  NGN_DIMM_CFG_REC_MAX
} NGN_DIMM_CFG_REC;

typedef enum {
  NGN_DIMM_PCD_BIOS_PARTITION = 0,
  NGN_DIMM_PCD_OS_PARTITION,
  NGN_DIMM_PCD_PARTITION_MAX
} NGN_DIMM_PCD_PARTITION;

//
// NGN DIMM Configuration Current Table
//
typedef struct {
  NGN_DIMM_PLATFORM_CFG_CURRENT_BODY_HOST body;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST interleave[MAX_UNIQUE_NGN_DIMM_INTERLEAVE];
  NGN_DIMM_INTERLEAVE_ID_HOST interleaveId[MAX_UNIQUE_NGN_DIMM_INTERLEAVE][MAX_CH];
} NGN_DIMM_CFG_CUR_HOST;

//
// NGN DIMM Configuration Request Table
//
typedef struct {
  NGN_DIMM_PLATFORM_CFG_INPUT_BODY_HOST body;
  NGN_DIMM_PARTITION_SIZE_CHANGE_PCAT_HOST partitionSize;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST interleave[MAX_UNIQUE_NGN_DIMM_INTERLEAVE];
  NGN_DIMM_INTERLEAVE_ID_HOST interleaveId[MAX_UNIQUE_NGN_DIMM_INTERLEAVE][MAX_CH];
} NGN_DIMM_CFG_REQ_HOST;

//
// NGN DIMM Configuration Output Table
//
typedef struct {
  NGN_DIMM_PLATFORM_CFG_OUTPUT_BODY_HOST body;
  NGN_DIMM_PARTITION_SIZE_CHANGE_PCAT_HOST partitionSize;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST interleave[MAX_UNIQUE_NGN_DIMM_INTERLEAVE];
  NGN_DIMM_INTERLEAVE_ID_HOST interleaveId[MAX_UNIQUE_NGN_DIMM_INTERLEAVE][MAX_CH];
} NGN_DIMM_CFG_OUT_HOST;

//
// NGN DIMM Configuration Error Information
//
typedef struct {
  UINT8 PcdPartitionError[NGN_DIMM_PCD_PARTITION_MAX];
} NGN_DIMM_CFG_ERROR_HOST;

//
//  NGN DIMM Configuration Information
//
typedef struct {
  UINT8                       Socket;       // Socket number for NG NVM DIMM
  UINT8                       Channel;      // Channel number for NG NVM DIMM
  UINT8                       Dimm;         // DIMM slot number for NG NVM DIMM
  NGN_DIMM_SPD_UNIQUE_DIMM_ID DimmUniqueId; // DIMM Unique ID
} NGN_CFG_ENTRY;

//
// DDRT DIMM info
//
typedef struct {
  UINT8                   NgnCfgCurRev;
  BOOLEAN                 NgnCfgCurPresent;
  BOOLEAN                 NgnCfgCurValid;
  UINT8                   NgnCfgCurUidsValid;// BIT-0, 1=valid UID's, 0=Invalid UID's Bit-1: 1=dimm present, 0= dimm not present
  NGN_DIMM_CFG_CUR_HOST   NgnCfgCur;
  UINT8                   NgnCfgReqRev;
  BOOLEAN                 NgnCfgReqPresent;
  BOOLEAN                 NgnCfgReqValid;
  NGN_DIMM_CFG_REQ_HOST   NgnCfgReq;
  UINT8                   NgnCfgOutRev;
  BOOLEAN                 NgnCfgOutPresent;
  NGN_DIMM_CFG_OUT_HOST   NgnCfgOut;
  BOOLEAN                 NgnCfgErrorPresent;
  NGN_DIMM_CFG_ERROR_HOST NgnCfgError;
  BOOLEAN                 NewDimm; ///< Variable to identify if this DIMM is newly populated in this boot.
} DDRT_DIMM_DEVICE_INFO_STRUCT;

typedef struct {
  UINT32 Digest[NGN_FFW_BANK_NUM][NGN_FFW_CSR_NUM];
} FFW_DIGEST_CSR_PER_DIMM_STRUCT;

//
// Memory Channel info.
// Used to define data fields at the "Channel" level in the memory hierarchy
//
typedef struct {
  UINT64                             WpqFlushHintAddress;
  DDRT_DIMM_DEVICE_INFO_STRUCT       DdrtDimmList[MAX_DDRT_DIMM_PER_CH];
  UINT32                             VolIntlvSizePerCh;  // Total interleaved DDR memory size per channel for patrol scrub interval (1LM/2LM/1LM+2LM)
  FFW_DIGEST_CSR_PER_DIMM_STRUCT     FfwDigestPerDimm[MAX_DIMM];
} MEMORY_MAP_HOST_CHANNEL_INFO_STRUCT;

//
// Socket info.
// Used to define data fields at the "Socket" level in the memory hierarchy
//
typedef struct {
  UINT8                               SadIntList[MAX_DRAM_CLUSTERS * MAX_SAD_RULES][MC_MAX_NODE]; // SAD interleave list
  MEMORY_MAP_HOST_CHANNEL_INFO_STRUCT ChannelInfo[MAX_CH];
  UINT8                               XorMode;                   // DDR XOR bitmap for channel XOR, cluster XOR, etc.
} MEMORY_MAP_HOST_SOCKET_INFO_STRUCT;

typedef enum {
  InTile = BIT0,
  XTile = BIT1,
  MaxCacheType
} CACHE_CAPABILITY;

typedef struct {
  UINT8                              MemModeCapabilities;                   // Memory Mode Capabilities: 1LM, 2LM, AD, ADWB
  CACHE_CAPABILITY                   MmCacheCapabilities;                   // 2LM Memory Mode Cache Capabilities
  CACHE_CAPABILITY                   AdWbCacheCapabilities;                 // 2LM AD-WB Mode Cache Capabilities
} MEMORY_MAP_HOST_PCAT_CAPABILITY_STRUCT;

//
// Global Memory Map Host structure
//
typedef struct {
  MEMORY_MAP_HOST_SOCKET_INFO_STRUCT      SocketInfo[MAX_SOCKET];
  NGN_CFG_ENTRY                           CfgRecords[NGN_DIMM_CFG_REC_MAX][NGN_DIMM_MAX_NUM_OF_CFG_RECORDS]; // Configuration records (current configuration/output configuration/configuration errors)
  UINT8                                   NumCfgRecords[NGN_DIMM_CFG_REC_MAX];                               // Number of configuration records (current configuration/output configuration/configuration errors)
  UINT8                                   NumCfgCurRecords;                                                  // Number of current configuration records in CfgCurRecord[]
  UINT8                                   DdrtIntlvGranularity;                                              // DDRT interleave granularity
  MEMORY_MAP_BLOCK_DECODER_DATA           BlockDecoderData;                                                  // Block decoder data for memory map
  UINT8                                   NumberofChaDramClusters;                                           // Number of CHA DRAM decoder clusters
  UINT8                                   DdrClusteringMode;                                                 // DDR memmory clustering mode: All2All, Hemi, Quad, SNC2 or SNC4
  BOOLEAN                                 IsTwoWayNmCacheEn;                                                 // Flag indicates if biased 2-way near memory cache is enabled
  BOOLEAN                                 IsWpqFlushSupported;                                               // Flag indicates if WPQ flush is supported
  BOOLEAN                                 IsFastZeroMemEn;                                                   // Flag indicates if boot-time fast zero memory is enabled
  BOOLEAN                                 IsFadrEnabled;                                                     // Flag indicates if fADR is enabled on system (10nm wave 3 only)
  MEMORY_MAP_HOST_PCAT_CAPABILITY_STRUCT  PcatCapability;                                                    // PCAT platform capability
  UINT8                                   MaxSadRules;                                                       // Maximum SAD entries supported by silicon (24 for 14nm silicon, 16 for 10nm silicon)
  RSAD_TABLE_FPGA                         FpgaRsadRules[MAX_FPGA_REMOTE_SAD_RULES];                          // Store FPGA Remote Dram rules
  BOOLEAN                                 FpgaMemCnt;                                                        // Platform has FPGA sockets/memory
} MEMORY_MAP_HOST;

#endif // _MEM_MAP_HOST_H_

