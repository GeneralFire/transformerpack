/** @file
  Interface header file for the HBM library class.

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

#ifndef _HBM_CORE_HOST_H_
#define _HBM_CORE_HOST_H_

typedef enum {
  HbmRegTxOutStgCompEn = 0,
  HbmRegTxOutStgCompRxOut,
  HbmRegMax
} HBM_DW_REG;

typedef enum {
  HbmCacheRxGroup0 = 0,
  HbmCacheRxVrefCtrl,
  HbmCacheTxDsP,
  HbmCacheTxTco,
  HbmCacheTxWrPi,
  HbmCacheClkTco,
  HbmCacheCmdTco,
  HbmCacheCmdDsP,
  HbmCacheCkCmdAll,
  HbmCacheRndTrip,
  HbmCacheTxCycle,
  HbmCacheRxDqLeft,
  HbmCacheRxDqRight,
  HbmCacheTxDqLeft,
  HbmCacheTxDqRight,
  HbmCacheRxVrefLow,
  HbmCacheRxVrefHigh,
  HbmCacheCmdLeft,
  HbmCacheCmdRight,
  HbmCacheNormalizeFactor,
  HbmMaxCacheDelay
} HBM_CACHE_DELAY;

typedef enum {
  HbmCacheFsmXoverPi = 0,
  HbmCacheFsmAword,
  HbmCacheFsmDwordTx,
  HbmCacheFsmDwordRxP,
  HbmCacheFsmDwordRxN,
  HbmMaxCacheFsm
} HBM_CACHE_FSM;

typedef enum {
  HbmCacheEyeDwordTx = 0,
  HbmCacheEyeDwordRxP,
  HbmCacheEyeDwordRxN,
  HbmMaxCacheEye
} HBM_CACHE_EYE;

typedef struct {
  MRC_GT      Group;
  UINT16      MinVal;
  UINT16      MaxVal;
  UINT16      UsDelay;
  CHAR8       *GroupStr;
} HBM_GSM_LIMIT;

typedef struct {
  HBM_DW_REG  HbmReg;
  INT16       MinVal;
  INT16       MaxVal;
  INT16       UsDelay;
  char        *GroupStr;
} HBM_DW_LIMIT;

typedef struct {
  UINT8   ChParams[2];
  UINT8   DwParams[2];
} HBM_AW_TUNE;

typedef struct _BASE_MARGIN {
  INT16 N;
  INT16 P;
} BASE_MARGIN;
//
// Output structures based on scope
// Mask of bits to ignore
//

typedef struct _HBM_CH_MARGIN {
  BASE_MARGIN Channel[MAX_HBM_CH];
} HBM_CH_MARGIN;

typedef struct _HBM_STROBE_MARGIN {
  BASE_MARGIN Strobe[MAX_HBM_CH][MAX_HBM_STROBE];
} HBM_STROBE_MARGIN;

typedef struct _HBM_BIT_MARGIN {
  BASE_MARGIN Bits[MAX_HBM_CH][MAX_HBM_STROBE][MAX_HBM_BITS_PER_STROBE];
} HBM_BIT_MARGIN;

//
// HBM channel info
//
typedef struct _HBM_CH_LIST {
  UINT16          MemSize;                 // Memory size for this channel (64MB granularity)
  UINT32          PanHandleInfo;
  DEVICE_ID_WDR   DevId;
  HBM_MR0_STRUCT  HbmMr00;
  HBM_MR1_STRUCT  HbmMr01;
  HBM_MR2_STRUCT  HbmMr02;
  HBM_MR3_STRUCT  HbmMr03;
  HBM_MR4_STRUCT  HbmMr04;
  HBM_MR5_STRUCT  HbmMr05;
  HBM_MR6_STRUCT  HbmMr06;
  HBM_MR7_STRUCT  HbmMr07;
  HBM_MR8_STRUCT  HbmMr08;
  HBM_MR9_STRUCT  HbmMr09;
  HBM_MR10_STRUCT HbmMr10;
  HBM_MR11_STRUCT HbmMr11;
  HBM_MR12_STRUCT HbmMr12;
  HBM_MR13_STRUCT HbmMr13;
  HBM_MR14_STRUCT HbmMr14;
  HBM_MR15_STRUCT HbmMr15;
} HBM_CH_LIST;
//
// HBM architecture information
//
typedef struct _HBM_ARCH_INFO {
  UINT32    MaxMemSs;
  UINT32    MaxChannelAll;
  UINT32    MaxChannelPerMemSs[MAX_HBM_IO];
  UINT32    MaxStrobes[MAX_HBM_IO];
  UINT32    MaxStrobesSub[MAX_HBM_IO];
  UINT32    MaxBits[MAX_HBM_IO];
} HBM_ARCH_INFO;
//
// Chip specific section of HBM Cache values
//
typedef struct _HBM_CH_NVRAM {
  UINT8  ChEnable;
  //
  // To add more members for the warm reset and fast cold boot path
  //
} HBM_CH_NVRAM;

//
// Memory map private structure for per-socket HBM data.
//
typedef struct _HBM_MEMORY_MAP_SOCKET_DATA {
  UINT8       StackCount;         ///< Number of enabled stacks. Equal to the number set bits in StackEnableMask.
  UINT8       StackEnableMask;    ///< Mask of enabled stacks. Bit N is set to 1 if stack N is enabled.
  CONST UINT8 *StackRouting;      ///< Pointer to the array of MAX_CLUSTERS for routing the stacks to clusters.
  UINT16      ChannelCapacity;    ///< Total capacity of each HBM channel in units of 64MB.
  UINT8       CacheRatio;         ///< Final chosen cache ratio for the socket.
  UINT8       CacheFmType;        ///< Final chosen far memory type (DDR4 or DDRT).
  UINT16      CacheSizePerStack;  ///< Cache size of each stack expressed in units of 64MB.
  UINT16      FlatSizePerStack;   ///< Flat size of each stack expressed in units of 64MB.
} HBM_MEMORY_MAP_SOCKET_DATA;

//
// Memory map private structure for HBM data.
//
typedef struct _HBM_MEMORY_MAP_DATA {
  UINT8   NumSockets;         ///< Number of entries in Socket array.
  HBM_MEMORY_MAP_SOCKET_DATA Socket[MAX_SOCKET];  ///< Array of sockets.
} HBM_MEMORY_MAP_DATA;

typedef struct _HBM_SOCKET_NVRAM {
  HBM_CH_NVRAM      HbmChData[MAX_HBM_CH];
} HBM_SOCKET_NVRAM;

typedef struct _HBM_NVRAM {
  HBM_SOCKET_NVRAM  Socket[MAX_SOCKET];
} HBM_NVRAM;

typedef struct _HBM_SOCKET {
  UINT8             HbmIoEn[MAX_HBM_IO];
  BOOLEAN           HbmFsmInProgress;
  HBM_ARCH_INFO     HbmArchInfo;
  HBM_CH_LIST       HbmCh[MAX_HBM_IO];
  struct IMC        McHbm[MAX_HBM_IO];
  UINT32            HbmCacheMemSizePerStack;
  UINT32            HbmFlatMemSizePerStack;
  UINT8             HbmInstances;
  HBM_FREQUENCY     HbmFrequency;
} HBM_SOCKET;

typedef struct _HBM_VAR {
  HBM_SOCKET        Socket[MAX_SOCKET];
  UINT32            InitialLfsrSeed;
} HBM_VAR;

typedef struct _HBM_HOST {
  UINT32                HbmFlows;
  HBM_POLICY            Policy;
  HBM_NVRAM             NvRam;
  HBM_VAR               Var;
  HBM_MEMORY_MAP_DATA   HbmData;
} HBM_HOST;

#endif // _HBM_CORE_HOST_H_

