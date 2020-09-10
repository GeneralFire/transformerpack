/** @file
  Interface of memory topology Data library.

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

#ifndef __PLAT_MEM_TOPOLOGY_LIB_H__
#define __PLAT_MEM_TOPOLOGY_LIB_H__

#include <SiliconSetting.h>
#include <SiliconSettingRAS.h>
#include <Upi/KtiSi.h>

#pragma pack(1)
typedef struct {
  UINT8   enabled;                // 0 = disabled,  1 = enabled
  UINT8   logicalRank;            // Logical Rank number (0 - 7)
  UINT16  rankSize;               // Units of 64 MB
} RANK_DEVICE_STRUCT;

typedef struct {
  UINT8  Present;
  UINT8  AepDimmPresent;
  BOOLEAN Enabled;
  UINT8  X4Present;
  UINT16 nonVolCap;          // Non-volatile capacity (AEP DIMM only)
  UINT16  DimmSize;
  UINT16 SmbiosType17Handle;
  RANK_DEVICE_STRUCT  rankList[MAX_RANK_DIMM * SUB_CH];
} DIMM_DEVICE_STRUCT;

typedef struct  {
  UINT8 Enabled;                            // 0 = channel disabled, 1 = channel enabled
  UINT8 Features;                           // Bit mask of features to enable or disable
  UINT8 MaxDimm;                            // Number of DIMM
  UINT8 NumRanks;                           // Number of ranks on this channel
  UINT8 chFailed;
  UINT8 ngnChFailed;
  UINT8 SpareLogicalRank[MAX_SPARE_RANK];    // Logical rank, selected as Spare
  UINT8 SparePhysicalRank[MAX_SPARE_RANK];   // Physical rank, selected as spare
  UINT16 SpareRankSize[MAX_SPARE_RANK];      // spare rank size
  DIMM_DEVICE_STRUCT  DimmInfo[MAX_DIMM];
} CHANNEL_DEVICE_STRUCT;

typedef struct  {
  UINT8                  Valid;
  UINT8                  imcEnabled[MAX_IMC];
  UINT32                 PatrolScrubInterval[MAX_IMC];
  UINT16                 SmbiosType16Handle[MAX_IMC];
  UINT64                 LLCMap;
  UINT64                 LlcIaCoreEnBitMap;
  CHANNEL_DEVICE_STRUCT  ChannelInfo[IMC_MAX_CH];
  UINT8                  UPIPortValid[MAX_FW_KTI_PORTS];
  UINT8                  TotalCha;
} SOCKET_STRUCT;

typedef struct {
  UINT8         MirrorVersion;
  BOOLEAN       MirrorMemoryBelow4GB;
  UINT16        MirroredAmountAbove4GB;
  UINT8         MirrorStatus;
} ADDR_RANGE_MIRROR_VARIABLE_DATA;

typedef struct {
  BOOLEAN                           MemTopologyInitFlag;  //indicate whether memory topology is initialized.
  UINT8                             SystemRasType;
  UINT8                             DieCount;
  UINT8                             RasModesEnabled;    // RAS modes that are enabled
  UINT16                            ExRasModesEnabled;  // Extended RAS modes that are enabled
  UINT8                             RasModesSupported;  //RAS modes that are supported by current memory population.
  UINT8                             VolMemMode;
  UINT8                             CacheMemType;
  UINT32                            Threshold;
  BOOLEAN                           EkvPresent;       // Set if EKV controller on system
  BOOLEAN                           BwvPresent;       // Set if BWV controller on system
  ADDR_RANGE_MIRROR_VARIABLE_DATA   MirrorCurrentType;
  ADDR_RANGE_MIRROR_VARIABLE_DATA   MirrorRequestType;
  SOCKET_STRUCT                     Socket[MAX_SOCKET];
  UINT32                            DdrCacheSize[MAX_SOCKET][IMC_MAX_CH];
} MEM_TOPOLOGY;
#pragma pack()

/**
  Get memory topology data, in case first one invoke this routine, it will initialize topology data.


  @retval  point of memoty topology info

**/
MEM_TOPOLOGY  *
EFIAPI
GetMemTopology (
  VOID
  );


/**
  Get the number of DIMMs in a channel

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number

  @retval - The number of DIMMs

**/
UINT8
EFIAPI
GetNumOfDimms (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode
  );



/**
  Check whether the socket is present

  @param  Skt             - socket ID

  @retval TRUE          - Socket exist
          FALSE         - Socket not exist

**/
BOOLEAN
EFIAPI
IsSocketPresent (
  IN  UINT8  Skt
  );

/**
  Check whether the node is present

  @param  NodeId          - Memory controller ID

  @retval Status          - TRUE/FALSE

**/
BOOLEAN
EFIAPI
IsNodePresent (
  IN  UINT8         NodeId
  );

/**
  Check whether the channel is present per socket

  @param  Socket          - Socket ID
  @param  ChOnSkt         - Channel number per socket

  @retval Status          - TRUE/FALSE

**/
BOOLEAN
EFIAPI
IsChannelPresentPerSocket (
  IN  UINT8         Socket,
  IN  UINT8         ChOnSkt
  );

/**
  Check whether the channel is present

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number

  @retval Status          - TRUE/FALSE

**/
BOOLEAN
IsChannelPresent (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode
  );





/**
  Get the number of DIMMs in a channel

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  Rank            - Rank Number

  @retval Status          - TRUE/FALSE

**/
BOOLEAN
EFIAPI
IsRankEnabled (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank
  );

/**
  check if current imc is under 1LM mode or not

  @param  none
  @retval TRUE -- 1LM; FALSE -- Not 1LM.
**/
BOOLEAN
EFIAPI
IsImc1LmMode (
  VOID
  );
#endif
