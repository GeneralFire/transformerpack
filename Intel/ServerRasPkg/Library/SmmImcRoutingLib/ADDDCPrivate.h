/** @file
  Implementation of ADDDC library.

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
#ifndef __ADDDC_H__

//
// ADDDC Bank Mode
//
typedef enum {
  NonBankVlsMode = 0,
  BankVlsPrimaryMode,
  BankVlsBuddyMode,
  BankVlsBusyMode,
  BankInvalidMode,
} ADDDC_BANK_MODE;

//
// ADDDC Rank Mode
//
typedef enum {
  NonRankVlsMode = 0,
  RankVlsPrimaryMode,
  RankVlsBuddyMode,
  RankVlsBusyMode,
  SddcPlusOneMode,
  RankInvalidMode
} ADDDC_RANK_MODE;

//
// ADDDC Event Type
//
typedef enum {
  NoEventInProgress = 0,          // No ADDDC event in progress
  PclsEvent,
  RankVlsEvent,                   // Rank VLS event
  BankVlsEvent,                   // Bank VLS event
  RankVlsReverseEvent,            // Rank VLS reverse event
  BankVlsReverseEvent,            // Bank VLS reverse event
  Bank2RankVlsEvent,              // Promote Bank to Rank VLS
} ADDDC_EVENT_TYPE;

//
// ADDDC Event Structure
//
typedef
EFI_STATUS
(*ADDDC_EVENT_ACTION) (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  );

typedef struct {
  UINT8                 Num;
  ADDDC_EVENT_TYPE      Type;
  SPARING_REGION_STRUCT SparingRegion;
  ADDDC_EVENT_ACTION    ActionFunc;
  BOOLEAN               CombinedSmi;
  BOOLEAN               Valid;
} ADDDC_EVENT_STRUCT;

typedef UINT32          ADDDC_DEVICE_STATUS;

typedef struct {
  ADDDC_BANK_MODE             BankMode;
} ADDDC_BANK_MODE_STRUCT;

typedef struct {
  ADDDC_RANK_MODE             RankMode;
  ADDDC_DEVICE_STATUS         DeviceStatus;
  ADDDC_BANK_MODE_STRUCT      Banks[MAX_BANK_RANK];
} ADDDC_RANK_MODE_STRUCT;

typedef struct{
  ADDDC_RANK_MODE_STRUCT      Ranks[MAX_RANK_CH * SUB_CH];
} ADDDC_CH_MODE_STRUCT;

typedef struct{
  ADDDC_CH_MODE_STRUCT        Chs[MAX_MC_CH];
} ADDDC_NODE_MODE_STRUCT;

typedef struct{
  ADDDC_NODE_MODE_STRUCT      Nodes[MC_MAX_NODE];
} ADDDC_MODE_STRUCT;

#define ADDDC_REGION_SIZE_BANK      0
#define ADDDC_REGION_SIZE_RANK      1

#define MAX_EVT_PENDING            (4)
/**
  Check whether Rank VLS is supported or not
  @retval

**/
BOOLEAN
IsRankVlsSupport(
  VOID
);

/**
  Get the current rank status

  @param NodeId         - Memory controller ID
  @param ChOnNode       - Channel number
  @param Rank           - Rank number

  @retval Mode          - NonRankVlsMode
                        - RankVlsPrimaryMode
                        - RankVlsBuddyMode
                        - SddcPlusOneMode
**/
ADDDC_RANK_MODE
GetRankMode (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank
  );

/**
  Set the current rank status

  @param NodeId         - Memory controller ID
  @param ChOnNode       - Channel number
  @param Rank           - Rank number
  @param Mode           - Rank mode

**/
VOID
SetRankMode (
  IN  UINT8             NodeId,
  IN  UINT8             ChOnNode,
  IN  UINT8             Rank,
  IN  ADDDC_RANK_MODE   RankMode
  );

/**
  Get the current bank status

  @param NodeId         - Memory controller ID
  @param ChOnNode       - Channel number
  @param Rank           - Rank number

  @retval Status        - EFI_SUCCESS if no failures
                        - non-zero error code for failures

**/
ADDDC_BANK_MODE
GetBankMode (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank,
  IN  UINT8         Bank
  );

/**
  Get the buddy rank and bank numbers

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  Rank            - Rank number
  @param  Bank            - Bank number
  @param  BuddyRank       - Pointer to buddy rank nubmer
  @param  BuddyBank       - Pointer to buddy bank number

  @retval Status          - EFI_SUCCESS/EFI_UNSUPPORTED

**/
EFI_STATUS
GetNewNonFailedRankBank (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank,
  IN  UINT8         Bank,
  OUT UINT8         *BuddyRank,
  OUT UINT8         *BuddyBank
  );

/**
  Start ADDDC plus 1

  @param  pAdddcRegion    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/
EFI_STATUS
StartAdddcPlusOneState (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**
  Start SDDC plus 1

  @param  pAdddcRegion    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/
EFI_STATUS
StartSddcPlusOneState (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**
  Do BANK VLS operation

  @param  pAdddcRegion    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/
EFI_STATUS
ActionOnBankVls (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**
  Do BANK VLS reverse operation

  @param  pAdddcRegion    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/
EFI_STATUS
ActionOnBankVlsReverse (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**
  Do RANK VLS operation

  @param  pAdddcRegion    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/
EFI_STATUS
ActionOnRankVls (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**
  Do RANK VLS reverse operation

  @param  pAdddcRegion    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/
EFI_STATUS
ActionOnRankVlsReverse (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**
  Clean up operation for previous VLS event

  @param  pAdddcRegion    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/
EFI_STATUS
CleanPreVlsEvent (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**
  Whether there is previous error in the device

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  Rank            - Rank number
  @param  Bank            - Bank number
  @param  Device          - Device number

  @retval Status          - TRUE - Error existed
                          - FALSE- No error

**/
BOOLEAN
IsErrorExistedInDevice (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank,
  IN  UINT8         Device
  );

/**
  Get the failed rank/bank/device information

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number

  @retval Status

**/
EFI_STATUS
GetFailedDeviceInfo (
  IN   UINT8                 NodeId,
  IN   UINT8                 ChOnNode,
  OUT  SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**
  Find out which region control register contains the rank number

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  pRegionNum      - Pointer to Region number

  @retval Status          - EFI_SUCCESS/EFI_UNSUPPORTED

**/
EFI_STATUS
FindRegionWithRank (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank,
  IN  UINT8         *RegionNum
  );

/**
  Find out which region control register contains the rank&bank number

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  pRegionNum      - Pointer to Region number

  @retval Status          - EFI_SUCCESS/EFI_UNSUPPORTED

**/
EFI_STATUS
FindRegionWithRankBank (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank,
  IN  UINT8         Bank,
  IN  UINT8         *RegionNum
  );

/**
  Try to find available region for VLS

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  Region          - Pointer to available region

  @retval Status          - EFI_SUCCESS if no failures
                          - non-zero error code for failures

**/
EFI_STATUS
GetAvailableRegion (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  OUT UINT8         *RegionNum
  );

/**
  Try to find the count of bank VLS of the rank

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  Rank            - Rank number

  @retval Count           - Bank VLS count

**/
UINT8
GetBankVlsCount (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank
  );

/**
  Clear the 'region_enable' bit of the ADDDC region control register

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  RegionNum       - Region number

  @retval Data            - register data

**/
VOID
FillRegionStruct (
  IN      UINT8                 NodeId,
  IN      UINT8                 ChOnNode,
  IN      UINT8                 RegionNum,
  IN OUT  SPARING_REGION_STRUCT  *SparingRegionPtr
  );

/**
  Set the current rank status

  @param NodeId         - Memory controller ID
  @param ChOnNode       - Channel number
  @param Rank           - Rank number
  @param Bank           - Bank number
  @param Mode           - Bank mode

**/
VOID
SetBankMode (
  IN  UINT8             NodeId,
  IN  UINT8             ChOnNode,
  IN  UINT8             Rank,
  IN  UINT8             Bank,
  IN  ADDDC_BANK_MODE   BankMode
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
  Initialize ADDDC global data structure

  @retval
**/
EFI_STATUS
EFIAPI
InitAdddcData (
  VOID
  );

/**
  Dump the ADDDC structure data

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number

**/
VOID
EFIAPI
DumpAdddcStatus (
  UINT8           NodeId,
  UINT8           ChOnNode
  );

#endif //__ADDDC_H__
