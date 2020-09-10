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
#include <IioUniversalData.h>
#include <Library/ImcRoutingLib.h>
#include <Library/RasDebugLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PlatSSRLib.h>
#include <Library/PclsLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Include/Ras/RasCommon.h>
#include "ImcRoutingPrivate.h"
#include "ADDDCPrivate.h"

#define ADDDC_REGION_CONTROL_MC_MAIN_LIST   {ADDDC_REGION0_CONTROL_MC_MAIN_REG, ADDDC_REGION1_CONTROL_MC_MAIN_REG, \
                                              ADDDC_REGION2_CONTROL_MC_MAIN_REG, ADDDC_REGION3_CONTROL_MC_MAIN_REG}


#define ADDDC_REGION_MCDDC_LIST          {ADDDC_REGION0_MCDDC_DP_REG, ADDDC_REGION1_MCDDC_DP_REG, \
                                              ADDDC_REGION2_MCDDC_DP_REG, ADDDC_REGION3_MCDDC_DP_REG}

UINT32              mAdddcCtrlRegionReg[] = ADDDC_REGION_CONTROL_MC_MAIN_LIST;
UINT32              mAdddcRegionMcddcReg[] = ADDDC_REGION_MCDDC_LIST;

UINT8               mMaxRegion = 2;
UINT8               mMaxDevice = 18;
BOOLEAN             mRankVlsSupport = FALSE;

ADDDC_MODE_STRUCT   *mAdddcStatus = NULL;

/**
  Check whether Rank VLS is supported or not
  @retval

**/
BOOLEAN
IsRankVlsSupport (
  VOID
  )
{
  return mRankVlsSupport;
}
/**
  Get the ADDDC data structure, if first time, allocate a new one

  @retval

**/
ADDDC_MODE_STRUCT *
EFIAPI
GetAdddcData (
  VOID
  )
{
  BOOLEAN             NewAllocation;
  ADDDC_MODE_STRUCT  *AdddcData;

  AdddcData = GetRasGlobalData (L"ADDDCData",
                 sizeof (ADDDC_MODE_STRUCT),
                 &NewAllocation
                 );

  if (NewAllocation) {
    ZeroMem (AdddcData, sizeof (ADDDC_MODE_STRUCT));
  }

  return AdddcData;
}

/**
  Initialize ADDDC global data structure

  @retval

**/
EFI_STATUS
EFIAPI
InitAdddcData (
  VOID
  )
{
  MEM_TOPOLOGY            *MemTopology;

  mAdddcStatus = GetAdddcData ();

  if (mAdddcStatus == NULL) {
    return EFI_UNSUPPORTED;
  }

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    return EFI_UNSUPPORTED;
  }

  switch (MemTopology->SystemRasType) {
    case STANDARD_RAS:
    case CORE_RAS:
      mMaxRegion = 1;     //MAX_REGION_EP;
      mRankVlsSupport = FALSE;
      break;

    case ADVANCED_RAS:
    case FPGA_RAS:
      mMaxRegion = 2;     //MAX_REGION_EX;
      mRankVlsSupport = TRUE; // Only advanced RAS support Rank VLS
      break;

    default:
      mMaxRegion = 1;     //MAX_REGION_EP;
      mRankVlsSupport = FALSE;
      break;
  }

  return EFI_SUCCESS;
}

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
  )
{
  ADDDC_RANK_MODE   RankMode;

  if (mAdddcStatus == NULL) {
    RankMode = RankInvalidMode;
  } else if (!IsRankEnabled (NodeId, ChOnNode, Rank)) {
    RankMode = RankInvalidMode;
  } else {
    RankMode = mAdddcStatus->Nodes[NodeId].Chs[ChOnNode].Ranks[Rank].RankMode;
  }

  return RankMode;
}

/**
  Set the current rank status

  @param NodeId         - Memory controller ID
  @param ChOnNode       - Channel number
  @param Rank           - Rank number
  @param Bank           - Bank number in Rank
  @param Mode           - Rank mode

**/
VOID
SetRankMode (
  IN  UINT8             NodeId,
  IN  UINT8             ChOnNode,
  IN  UINT8             Rank,
  IN  ADDDC_RANK_MODE   RankMode
  )
{
  if (mAdddcStatus == NULL) {
    return;
  }
  if (!IsRankEnabled (NodeId, ChOnNode, Rank)) {
    return;
  }
  mAdddcStatus->Nodes[NodeId].Chs[ChOnNode].Ranks[Rank].RankMode = RankMode;
}

/**
  Get the current bank status

  @param NodeId         - Memory controller ID
  @param ChOnNode       - Channel number
  @param Rank           - Rank number
  @param Bank           - Bank number in Rank

  @retval Status        - EFI_SUCCESS if no failures
                        - non-zero error code for failures

**/
ADDDC_BANK_MODE
GetBankMode (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank,
  IN  UINT8         Bank
  )
{
  if (mAdddcStatus == NULL) {
    return BankInvalidMode;
  }
  return mAdddcStatus->Nodes[NodeId].Chs[ChOnNode].Ranks[Rank].Banks[Bank].BankMode;
}

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
  )
{
  if (mAdddcStatus == NULL) {
    return;
  }
  mAdddcStatus->Nodes[NodeId].Chs[ChOnNode].Ranks[Rank].Banks[Bank].BankMode = BankMode;
}

/**
  Get the size of the rank

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  Rank            - Rank Number

  @retval Status          - TRUE/FALSE

**/
UINT16
EFIAPI
GetRankSize (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank
  )
{
  UINT8             Socket;
  UINT8             ChOnSkt;
  MEM_TOPOLOGY      *MemTopology;
  UINT8             Dimm;
  UINT8             RankOnDimm;
  UINT16            RankSize;

  Socket = NODE_TO_SKT (NodeId);
  ChOnSkt= NODECHA_TO_SKTCHA (NodeId, ChOnNode);
  MemTopology = GetMemTopology ();

  Dimm       = (Rank & BIT2) >> 2;
  RankOnDimm = Rank % 4;

  if (MemTopology == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "\t Incorrect Mem topology value!\n", NodeId));
    RankSize = 0;
  } else if (!IsNodePresent (NodeId)) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "\t Node %d is not present!\n", NodeId));
    RankSize = FALSE;
  } else {
    RankSize = MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].DimmInfo[Dimm].rankList[RankOnDimm].rankSize;
  }
  return RankSize;
}

/**
  Get the device status bit

  @param NodeId         - Memory controller ID
  @param ChOnNode       - Channel number
  @param Rank           - Rank number

**/
UINT32
GetDevStatus (
  IN  UINT8               NodeId,
  IN  UINT8               ChOnNode,
  IN  UINT8               Rank
  )
{
  if (mAdddcStatus == NULL) {
    return (UINT32) - 1;
  }

  if (!IsRankEnabled (NodeId, ChOnNode, Rank)) {
    return (UINT32) - 1;
  }

  return (mAdddcStatus->Nodes[NodeId].Chs[ChOnNode].Ranks[Rank].DeviceStatus);
}

UINT32
GetDevStatusBit (
  IN  UINT8               NodeId,
  IN  UINT8               ChOnNode,
  IN  UINT8               Rank,
  IN  UINT8               DevNum
  )
{
  return GetDevStatus (NodeId, ChOnNode, Rank) & (1 << DevNum);
}


/**
  Set the device status bit

  @param NodeId         - Memory controller ID
  @param ChOnNode       - Channel number
  @param Rank           - Rank number
  @param DevNum         - Device number
  @param Data           - 0/1

**/
VOID
SetDevStatusBit (
  IN  UINT8               NodeId,
  IN  UINT8               ChOnNode,
  IN  UINT8               Rank,
  IN  UINT8               DevNum,
  IN  BOOLEAN             Flag
  )
{
  if (mAdddcStatus == NULL) {
    return;
  }

  if (!IsRankEnabled (NodeId, ChOnNode, Rank)) {
    return;
  }

  if (Flag) {
    mAdddcStatus->Nodes[NodeId].Chs[ChOnNode].Ranks[Rank].DeviceStatus |= (1 << DevNum);
  } else {
    mAdddcStatus->Nodes[NodeId].Chs[ChOnNode].Ranks[Rank].DeviceStatus &= ~(1 << DevNum);
  }
}


/**
  Buddy rank selection function

  @param  FailedRank      - The failed rank number
  @param  Index           - The rank index to check
  @param  DimmPerCh       - The DIMM per channel

  @retval - The buddy rank number

**/
UINT8
EFIAPI
CalcBuddyRank (
        IN  UINT8         FailedRank,
        IN  UINT8         Index,
        IN  UINT8         DimmPerCh
        )
{
  UINT8             BuddyRank;
  UINT8             StartRank;
  switch (DimmPerCh) {
    case 1: // 1 dimm per channel
      /*
           *  1. The farthest rank comes first
           *  2. Make sure the ranks are in the same dimm

               failed rank      buddy rank
               3                  0
               2                  0
               1                  3
               0                  3
         */
      StartRank = (FailedRank & BIT1) ^ BIT1;
      if (StartRank != 0) {
        StartRank = 3;
      }

      if (Index < (MAX_RANK_DIMM * SUB_CH)) {
        BuddyRank = (FailedRank & BIT2) | ((StartRank + Index) % (MAX_RANK_DIMM * SUB_CH));
      } else {
        BuddyRank = 0;
      }
      break;
    case 2: // 2 dimm per channel
      //
      //   Choose the rank in the adjacent DIMM
      //
      BuddyRank = ((FailedRank & BIT2) ^ BIT2) | Index;
      if (FailedRank < (MAX_RANK_DIMM * SUB_CH) && Index >= (MAX_RANK_DIMM * SUB_CH)) {
        BuddyRank = BuddyRank - (MAX_RANK_DIMM * SUB_CH);
      }
      break;
    default:
      BuddyRank = ((FailedRank & BIT2) ^ BIT2) | Index;
      break;
  }

  return BuddyRank;
}

/**
  Buddy bank selection function

  @retval - The buddy bank number

**/
UINT8
EFIAPI
CalcBuddyBank (
        IN  UINT8         FailedBank
        )
{
  UINT8             BuddyBank;

  BuddyBank = FailedBank ^ BIT0;

  return BuddyBank;
}

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
  OUT UINT8         *NonFailedRankReturn,
  OUT UINT8         *NonFailedBankReturn
  )
{
  MEM_TOPOLOGY      *MemTopology;
  EFI_STATUS        Status;
  UINT8             DimmPerCh;
  UINT8             Index;
  UINT8             NonFailedRank = 0;
  UINT8             NonFailedBank;
  UINT8             BankVlsCnt;

  Status = EFI_UNSUPPORTED;

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    return EFI_UNSUPPORTED;
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "\t[GetNewNonFailedRankBank]: NodeId %d, ChOnNode %d, Rank %d, Bank %d.\n",
              NodeId, ChOnNode, Rank, Bank));

  if (!IsThereAnyPotentialPairRank (NodeId, ChOnNode, Rank)) {
    NonFailedRank = Rank;
    RAS_DEBUG((LEVEL_BASIC_FLOW, "\t\tNo available pair rank for the failed rank %d in channel %d, so select itself as buddy rank.\n", Rank, ChOnNode));
    Status = EFI_SUCCESS;
  } else if ((FindBankVlsPairRank (NodeId, ChOnNode, Rank, &NonFailedRank) == EFI_SUCCESS) && \
             (GetRankMode (NodeId, ChOnNode, NonFailedRank) == NonRankVlsMode)) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "\t\tThere is already BankVls between rank %d and rank %d, so select rank %d as buddy rank .\n", Rank, NonFailedRank, NonFailedRank));
      Status = EFI_SUCCESS;
  } else {
    DimmPerCh = GetNumOfDimms (NodeId, ChOnNode);

    for (Index = 0; Index < (MAX_RANK_CH * SUB_CH); Index++) {
      NonFailedRank = CalcBuddyRank (Rank, Index, DimmPerCh);
      //
      // Failed rank and buddy rank must be in the same sub-channel for DDR5.
      //
      if ((NonFailedRank == Rank) || ((NonFailedRank + Rank) % SUB_CH != 0)) {
        continue;
      }
      //
      // Ensure that the non failed rank chosen is of the same size as failed rank.
      //
      if ( (GetRankSize (NodeId, ChOnNode, Rank)) != (GetRankSize (NodeId, ChOnNode, NonFailedRank))) {
        continue;
      }

      BankVlsCnt = GetBankVlsCount (NodeId, ChOnNode, NonFailedRank);
      if (BankVlsCnt > 0) {
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t\tThere is already BankVls in rank%d, skip it as buddy rank.\n", NonFailedRank));
        continue;
      }

      RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t\tCheck buddy rank %d.\n", NonFailedRank));
      if (GetRankMode (NodeId, ChOnNode, NonFailedRank) == NonRankVlsMode) {
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t\tThe buddy rank %d is valid.\n", NonFailedRank));
        Status = EFI_SUCCESS;
        break;
      }
    }

    if (Index == (MAX_RANK_CH * SUB_CH)) {
      if ((GetRankMode (NodeId, ChOnNode, Rank) == NonRankVlsMode)) {
        NonFailedRank = Rank;
        RAS_DEBUG((LEVEL_BASIC_FLOW, "\t\tNo available pair rank for the failed rank %d in channel %d, so select itself as buddy rank.\n", Rank, ChOnNode));
        Status = EFI_SUCCESS;
      }
    }
  }

  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,
                "\t[GetNewNonFailedRankBank]: Can't find available buddy rank/bank, return %r.\n", Status));
    return Status;
  }

  *NonFailedRankReturn = NonFailedRank;
  /* Try to find out the buddy bank */
  if ((Rank != NonFailedRank) &&
    (MemTopology->RasModesEnabled & (FULL_MIRROR_1LM | PARTIAL_MIRROR_1LM))) {
    NonFailedBank = Bank;
  } else {
    NonFailedBank = CalcBuddyBank (Bank);
  }
  if (GetBankMode (NodeId, ChOnNode, NonFailedRank, NonFailedBank) == NonBankVlsMode) {
    *NonFailedBankReturn = NonFailedBank;
  } else {
    for (Index = 0; Index < MAX_BANK_RANK; Index++) {
      if (Index == NonFailedBank) {
        continue;
      }
      if (GetBankMode (NodeId, ChOnNode, NonFailedRank, Index) == NonBankVlsMode) {
        *NonFailedBankReturn = Index;
        break;
      }
    }
  }
  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "\t[GetNewNonFailedRankBank]: Rank %d - Buddy Rank %d, Bank %d - Buddy Bank %d.\n",
              Rank, NonFailedRank, Bank, NonFailedBank));

  return Status;
}

/**
  Get the value of ADDDC region control register

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  RegionNum       - Region number

  @retval Data            - register data

**/
UINT32
GetRegionData (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         RegionNum
  )
{
  UINT8               Socket;
  UINT8               ChOnSkt;

  Socket = NODE_TO_SKT (NodeId);
  ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  if(RegionNum >= (sizeof(mAdddcCtrlRegionReg) / sizeof(mAdddcCtrlRegionReg[0]))) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "\n Unsupported Region number access\n"));
    return 0;
  }

  return  ReadCpuCsr (Socket,
            ChOnSkt,
            mAdddcCtrlRegionReg[RegionNum]);
}

/**
  Set the value of ADDDC region control register

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  RegionNum       - Region number

  @retval Data            - register data

**/

VOID
SetRegionData (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         RegionNum,
  IN  UINT32        Data
  )
{
  UINT8               Socket;
  UINT8               ChOnSkt;

  Socket = NODE_TO_SKT (NodeId);
  ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  if(RegionNum >= (sizeof(mAdddcCtrlRegionReg) / sizeof(mAdddcCtrlRegionReg[0]))) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "\n Unsupported Region number access\n"));
  } else {
     WriteCpuCsr (Socket,
        ChOnSkt,
        mAdddcCtrlRegionReg[RegionNum],
        Data);
  }
}

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
  OUT UINT8         *pRegionNum
  )
{
  UINT8             Region;
  EFI_STATUS        Status;
  UINT8             Skt;
  UINT8             ChOnSkt;
  UINT8             RegionEnable;
  UINT8             FailCs;
  UINT8             NonFailCs;


  Status = EFI_NOT_FOUND;

  Skt = NODE_TO_SKT (NodeId);
  ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  for (Region = 0; Region < mMaxRegion; Region++) {

    GetAdddcRegion (Skt, ChOnSkt, Region, &RegionEnable, &FailCs, NULL, &NonFailCs, NULL, NULL, NULL, NULL);
    if (RegionEnable == 1) {
      if ((FailCs == Rank) ||
          (NonFailCs == Rank)) {
        *pRegionNum = Region;
        Status = EFI_SUCCESS;
        break;
      }
    }
  }

  return Status;
}

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
  OUT UINT8         *pRegionNum
  )
{
  UINT8             Region;
  EFI_STATUS        Status;

  UINT8             Skt;
  UINT8             ChOnSkt;
  UINT8             RegionEnable;
  UINT8             FailCs;
  UINT8             NonFailCs;
  UINT8             FailedBank;
  UINT8             NonFailedBank;

  Skt = NODE_TO_SKT (NodeId);
  ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  Status = EFI_NOT_FOUND;


  for (Region = 0; Region < mMaxRegion; Region++) {
    GetAdddcRegion (Skt, ChOnSkt, Region, &RegionEnable, &FailCs, NULL, &NonFailCs, NULL, NULL, &FailedBank, &NonFailedBank);
    if (RegionEnable == 0) {
      continue;
    }
    if ((FailCs != Rank) && (NonFailCs != Rank)) {
      continue;
    }
    if (FailedBank == Bank || NonFailedBank == Bank) {
      *pRegionNum = Region;
      Status = EFI_SUCCESS;
      break;
    }
  }

  return Status;
}


/**
  Check whether the 'region_enable' bit is set

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  RegionNum       - Region number

  @retval Status          - TRUE/FALSE

**/
BOOLEAN
IsRegionFree (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         RegionNum
  )
{
  BOOLEAN           Status;
  UINT8             Skt;
  UINT8             ChOnSkt;
  UINT8             RegionEnable;

  Skt = NODE_TO_SKT (NodeId);
  ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  Status = FALSE;

  GetAdddcRegion (Skt, ChOnSkt, RegionNum, &RegionEnable, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

  if (RegionEnable == 0) {
    //
    //The region is not in use, it should be free
    //
    Status = TRUE;
  }

  return Status;
}




/**
  Fill the region structure according to ADDDC region register

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  RegionNum       - Region number
  @param  SparingRegionPtr   - Pointer to region structure

**/
VOID
FillRegionStruct (
  IN      UINT8                  NodeId,
  IN      UINT8                  ChOnNode,
  IN      UINT8                  RegionNum,
  IN OUT  SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  ADDDC_REGION0_MCDDC_DP_STRUCT               AdddcRegion;

  UINT8                                       Skt;
  UINT8                                       ChOnSkt;

  UINT8                                       FailCs;
  UINT8                                       NonFailCs;
  UINT8                                       FailedBank;
  UINT8                                       NonFailedBank;
  UINT8                                       FailSubRank;
  UINT8                                       NonFailSubRank;

  Skt = NODE_TO_SKT (NodeId);
  ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  GetAdddcRegion (Skt, ChOnSkt, RegionNum, NULL, &FailCs, &FailSubRank, &NonFailCs, &NonFailSubRank, NULL, &FailedBank, &NonFailedBank);


  SparingRegionPtr->Valid           = 1;
  SparingRegionPtr->RegionNum       = RegionNum;
  SparingRegionPtr->FailedRank      = (UINT8) FailCs;
  SparingRegionPtr->FailedSubRank   = (UINT8) FailSubRank;
  SparingRegionPtr->FailedBank      = (UINT8) FailedBank;
  SparingRegionPtr->NonFailedRank      = (UINT8) NonFailCs;
  SparingRegionPtr->NonFailedSubRank   = (UINT8) NonFailSubRank;
  SparingRegionPtr->NonFailedBank      = (UINT8) NonFailedBank;


  if(RegionNum >= (sizeof(mAdddcRegionMcddcReg) / sizeof(mAdddcRegionMcddcReg[0]))) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "\n Unsupported Region number access\n"));
  } else {
    AdddcRegion.Data  = (UINT8) ReadCpuCsr (NODE_TO_SKT (NodeId),
                                NODECHA_TO_SKTCHA (NodeId, ChOnNode),
                                mAdddcRegionMcddcReg[RegionNum]
                                );
    SparingRegionPtr->FailedDevice = (UINT8) AdddcRegion.Bits.faildevice;

    RAS_DEBUG ((LEVEL_BASIC_FLOW,
      "\t[FillRegionStruct]: Region %d, PRank %d, PBank %d, BRank %d, BBank %d, FailedDevice %d.\n",
      RegionNum,
      SparingRegionPtr->FailedRank,
      SparingRegionPtr->FailedBank,
      SparingRegionPtr->NonFailedRank,
      SparingRegionPtr->NonFailedBank,
      SparingRegionPtr->FailedDevice
      ));
  }
}

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
  OUT UINT8        *RegionNum
  )
{
  EFI_STATUS        Status;
  UINT8             Index;

  Status = EFI_NOT_FOUND;

  for (Index = 0; Index < mMaxRegion; Index++) {
    if (IsRegionFree (NodeId, ChOnNode, Index)) {
      *RegionNum = Index;
      Status = EFI_SUCCESS;
      break;
    }
  }
  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "\t[GetAvailableRegion]: NodeId %d, ChOnNode %d, Region %d, Status %r.\n",
              NodeId, ChOnNode, Index, Status));

  return Status;
}

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
  )
{
  UINT8             Bank;
  UINT8             Cnt;
  ADDDC_BANK_MODE   Mode;
  UINT8             PairRank;

  Cnt = 0;

  for (Bank = 0; Bank < MAX_BANK_RANK; Bank++) {
    Mode = GetBankMode (NodeId, ChOnNode, Rank, Bank);
    if (Mode == BankVlsPrimaryMode || Mode == BankVlsBuddyMode) {
      Cnt++;
    }
  }

  if (FindBankVlsPairRank (NodeId, ChOnNode, Rank, &PairRank) == EFI_SUCCESS) {
    if (Rank == PairRank) {
      Cnt = Cnt / 2;
    }
  }

  return Cnt;
}

/**
  Start ADDDC plus 1

  @param  SparingRegionPtr    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/
EFI_STATUS
StartAdddcPlusOneState (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  RAS_DEBUG ((LEVEL_BASIC_FLOW,
    "\t[StartAdddcPlusOneState]: Node %d, Ch %d, Rank %d, Buddy Rank %d.\n",
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    SparingRegionPtr->NonFailedRank));

  SetRankMode (SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    SddcPlusOneMode);

  if (SparingRegionPtr->Valid != 0) {
    SetRankMode (SparingRegionPtr->NodeId,
      SparingRegionPtr->ChOnNode,
      SparingRegionPtr->NonFailedRank,
      SddcPlusOneMode);
  }
  //
  //Leave it to SDDC handler later
  //
  return EFI_UNSUPPORTED;
}

/**
  Start SDDC plus 1

  @param  SparingRegionPtr    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/
EFI_STATUS
StartSddcPlusOneState (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  RAS_DEBUG ((LEVEL_BASIC_FLOW,
    "\t[StartSddcPlusOneState]: Node %d, Ch %d, Rank %d.\n",
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank));

  SetRankMode (SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    SddcPlusOneMode);

  //
  //Leave it to SDDC handler later
  //
  return EFI_UNSUPPORTED;
}

/**
  Do BANK VLS operation

  @param  SparingRegionPtr    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/
EFI_STATUS
ActionOnBankVls (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
    "\t[ActionOnBankVls] Region %d, NodeId %d, Ch %d, "
    "FailedRank %d - NonFailedRank %d, FailedBank %d - NonFailedBank %d.\n",
    SparingRegionPtr->RegionNum,
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    SparingRegionPtr->NonFailedRank,
    SparingRegionPtr->FailedBank,
    SparingRegionPtr->NonFailedBank));

  SetRankMode (
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    NonRankVlsMode
    );

  SetRankMode (
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->NonFailedRank,
    NonRankVlsMode
    );

  SetBankMode (SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    SparingRegionPtr->FailedBank,
    BankVlsPrimaryMode);

  SetBankMode (SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->NonFailedRank,
    SparingRegionPtr->NonFailedBank,
    BankVlsBuddyMode);

  SetDevStatusBit (SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    SparingRegionPtr->FailedDevice,
    TRUE);

  //
  //Clear previous pending reverse bit if any
  //
  ClearReverseBit (NODE_TO_SKT (SparingRegionPtr->NodeId), NODE_TO_MC (SparingRegionPtr->NodeId),  NODECHA_TO_SKTCHA (SparingRegionPtr->NodeId, SparingRegionPtr->ChOnNode));

  //
  //Clear previous PCLS region on the Failed and Buddy Bank if any
  //
  ClearPclsOnCh (SparingRegionPtr->NodeId, SparingRegionPtr->ChOnNode);

  //
  //Free this region firstly
  //
  FreeAdddcRegion (NODE_TO_SKT (SparingRegionPtr->NodeId),
    NODECHA_TO_SKTCHA (SparingRegionPtr->NodeId, SparingRegionPtr->ChOnNode),
    SparingRegionPtr->RegionNum);

  Status = VlsSparingCopy (SparingRegionPtr, BankAdddcSparingMR);
  SetNodeProgress (SparingRegionPtr->NodeId, TRUE);

  return Status;
}

/**
  Do BANK VLS reverse operation

  @param  SparingRegionPtr    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/

EFI_STATUS
ActionOnBankVlsReverse (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;
  RAS_DEBUG ((LEVEL_BASIC_FLOW,
    "\t[ActionOnBankVlsReverse] Region %d, NodeId %d, Ch %d, PRank %d - BRank %d, PBank %d - BBank %d.\n",
    SparingRegionPtr->RegionNum,
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    SparingRegionPtr->NonFailedRank,
    SparingRegionPtr->FailedBank,
    SparingRegionPtr->NonFailedBank));

 SetBankMode (SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    SparingRegionPtr->FailedBank,
    NonBankVlsMode);

  SetBankMode (SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->NonFailedRank,
    SparingRegionPtr->NonFailedBank,
    NonBankVlsMode);

  Status = VlsSparingCopy (SparingRegionPtr, BankAdddcSparingMR);
  SetNodeProgress (SparingRegionPtr->NodeId, TRUE);

  return Status;
}


/**
  Do RANK VLS operation

  @param  SparingRegionPtr    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/

EFI_STATUS
ActionOnRankVls (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
    "\t[ActionOnRankVls] Region %d, NodeId %d, Ch %d, PRank %d - BRank %d.\n",
    SparingRegionPtr->RegionNum,
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    SparingRegionPtr->NonFailedRank));

  SetRankMode (
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    RankVlsPrimaryMode
    );

  SetRankMode (
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->NonFailedRank,
    RankVlsBuddyMode
    );

  SetDevStatusBit (
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    SparingRegionPtr->FailedDevice,
    TRUE
    );

  //
  //Clear previous pending reverse bit if any
  //
  ClearReverseBit (NODE_TO_SKT (SparingRegionPtr->NodeId), NODE_TO_MC (SparingRegionPtr->NodeId),  NODECHA_TO_SKTCHA (SparingRegionPtr->NodeId, SparingRegionPtr->ChOnNode));

  //
  //Clear previous PCLS region on the Failed and Buddy Rank if any
  //
  ClearPclsOnCh (SparingRegionPtr->NodeId, SparingRegionPtr->ChOnNode);

  //
  //Free this region firstly
  //
  FreeAdddcRegion (NODE_TO_SKT (SparingRegionPtr->NodeId),
    NODECHA_TO_SKTCHA (SparingRegionPtr->NodeId, SparingRegionPtr->ChOnNode),
    SparingRegionPtr->RegionNum);

  Status = VlsSparingCopy (SparingRegionPtr, RankAdddcSparingMR);
  SetNodeProgress (SparingRegionPtr->NodeId, TRUE);
  return Status;
}

/**
  Do RANK VLS reverse operation

  @param  SparingRegionPtr    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/
EFI_STATUS
ActionOnRankVlsReverse (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  EFI_STATUS Status;

  Status = EFI_SUCCESS;
  RAS_DEBUG ((LEVEL_BASIC_FLOW,
    "\t[ActionOnRankVlsReverse] Region %d, NodeId %d, Ch %d, PRank %d - BRank %d.\n",
    SparingRegionPtr->RegionNum,
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    SparingRegionPtr->NonFailedRank));

  SetRankMode (
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->FailedRank,
    NonRankVlsMode
    );

  SetRankMode (
    SparingRegionPtr->NodeId,
    SparingRegionPtr->ChOnNode,
    SparingRegionPtr->NonFailedRank,
    NonRankVlsMode
    );

  Status = VlsSparingCopy (SparingRegionPtr, RankAdddcSparingMR);
  SetNodeProgress (SparingRegionPtr->NodeId, TRUE);
  return Status;
}

/**
  Clean up operation for previous VLS event

  @param  SparingRegionPtr    - Pointer to ADDDC region structure

  @retval Status          - EFI_SUCCESS
                          - No zero value for error

**/

EFI_STATUS
CleanPreVlsEvent (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  UINT8       NodeId;

  NodeId = SparingRegionPtr->NodeId;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t[CleanPreVlsEvent]: NodeId %d.\n", NodeId));

  ClearSparing2LmWa (NodeId, SparingRegionPtr->ChOnNode);
  PatrolScrubInfoSaveOrRestore (NodeId, PATROL_RESTORE | GV_C6FLAG);
  SetNodeProgress (SparingRegionPtr->NodeId,FALSE);
  DumpAdddcStatus (SparingRegionPtr->NodeId, SparingRegionPtr->ChOnNode);
  SetErrorThresholdAfterCopyComplete (NodeId, SparingRegionPtr->ChOnNode);

  return EFI_SUCCESS;
}


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
  )
{
  BOOLEAN           Status;

  Status = FALSE;
  if (GetDevStatusBit (NodeId, ChOnNode, Rank, Device) != 0) {
    Status = TRUE;
  }

  return Status;
}

/**
  Get the failed rank/bank/device information for corrected errors on DDR4
  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number

  @retval Status

**/
EFI_STATUS
GetFailedDeviceInfo (
  IN   UINT8                  NodeId,
  IN   UINT8                  ChOnNode,
  OUT  SPARING_REGION_STRUCT  *SparingRegionPtr
  )
{
  UINT8             Rank;
  BOOLEAN           ErrorExsited;
  FAILED_DIMM_INFO  FailedDimmInfo;
  EFI_STATUS        Status;

  ErrorExsited = FALSE;
  //
  //Check whether the error existed in this channel
  //
  for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank++) {
    //
    //Check whether the rank has been enabled
    //
    if (!IsRankEnabled (NodeId, ChOnNode, Rank)) {
      continue;
    }
    if (IsErrorExcdForThisRank (NodeId, ChOnNode, Rank) &&
       (!IsDcpmmRank (NODE_TO_SKT (NodeId), NODECHA_TO_SKTCHA (NodeId, ChOnNode), Rank))) {
      ErrorExsited = TRUE;
    }
  }
  if (!ErrorExsited) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t[GetFailedDeviceInfo]:No error found!\n"));
    return EFI_NOT_FOUND;
  }

  Status = GetChannelErrorInfo (NodeId, ChOnNode, (UINT64)-1, &FailedDimmInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "*******************************************************************\n"));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t[GetFailedDeviceInfo]: Node: %d, Ch %d, Rank %d, Bank %d, BankGrp %d, Device %d.\n",
              NodeId,
              ChOnNode,
              FailedDimmInfo.FailedRank,
              FailedDimmInfo.FailedBank,
              FailedDimmInfo.FailedBG,
              FailedDimmInfo.FailedDev));
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "*******************************************************************\n"));
  SparingRegionPtr->Valid         = 0;     // The region is not ready for VLS now.
  SparingRegionPtr->NodeId        = NodeId;
  SparingRegionPtr->ChOnNode      = ChOnNode;
  SparingRegionPtr->FailedRank    = FailedDimmInfo.FailedRank;
  SparingRegionPtr->FailedSubRank = FailedDimmInfo.FailedSubRank;
  SparingRegionPtr->FailedBank    = GET_BANK_NUM (FailedDimmInfo.FailedBank,
                                      FailedDimmInfo.FailedBG);
  SparingRegionPtr->FailedDevice  = FailedDimmInfo.FailedDev;
  SparingRegionPtr->FailedRow     = FailedDimmInfo.FailedRow;
  SparingRegionPtr->FailedCol     = FailedDimmInfo.FailedCol;
  SparingRegionPtr->Transfer      = FailedDimmInfo.Transfer;
  SparingRegionPtr->SingleBit     = FailedDimmInfo.SingleBit;
  SparingRegionPtr->InterLocSyn   = FailedDimmInfo.InterLocSyn;

  return EFI_SUCCESS;
}

UINT8 *
EFIAPI
BankModeToStr (
  IN   UINT8                 NodeId,
  IN   UINT8                 ChOnNode,
  IN   UINT8                 Rank,
  IN   UINT8                 Bank
  )
{
  ADDDC_BANK_MODE           Mode;
  CHAR8                     *Str;

  Mode = GetBankMode (NodeId, ChOnNode, Rank, Bank);
  switch (Mode) {
    case NonBankVlsMode:
      Str = "N";
      break;
    case BankVlsPrimaryMode:
      Str = "P";
      break;
    case BankVlsBuddyMode:
      Str = "B";
      break;
    case BankInvalidMode:
      Str = "I";
      break;
    default:
      Str = "-";
      break;
  }
  return (UINT8 *)Str;
}

UINT8 *
EFIAPI
RankModeToStr (
  IN   UINT8                 NodeId,
  IN   UINT8                 ChOnNode,
  IN   UINT8                 Rank
  )
{
  ADDDC_RANK_MODE          Mode;
  CHAR8                    *Str;

  Mode = GetRankMode (NodeId, ChOnNode, Rank);
  switch (Mode) {
    case NonRankVlsMode:
      Str = "N";
      break;
    case RankVlsPrimaryMode:
      Str = "P";
      break;
    case RankVlsBuddyMode:
      Str = "B";
      break;
    case RankInvalidMode:
      Str = "I";
      break;
    case SddcPlusOneMode:
      Str = "S";
      break;
    default:
      Str = "-";
      break;
  }

  return (UINT8 *)Str;
}

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
  )
{
  UINT8 Rank, Bank;

  if (mAdddcStatus == NULL) {
    return;
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW,
              "===================== Dump Adddc Status: Node %d, Ch %d =========================\n",
              NodeId, ChOnNode));

  for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank++) {
    if (GetRankMode (NodeId, ChOnNode, Rank) == RankInvalidMode) {
      continue;
    }
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "- Rank %d, RMode %02s, DevSts 0x%08x -\n",
                Rank,
                RankModeToStr (NodeId, ChOnNode, Rank),
                GetDevStatus (NodeId, ChOnNode, Rank)));
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"- Bank     : "));
    for (Bank = 0; Bank < MAX_BANK_RANK; Bank++) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW,"%02d ", Bank));
    }
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "\n"));
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"- Bank Mode: "));
    for (Bank = 0; Bank < MAX_BANK_RANK; Bank++) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW,"%02s ", BankModeToStr (NodeId, ChOnNode, Rank, Bank)));
    }
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "\n"));
  }
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "\n"));
}

/**
  Whether there is potential pair rank except itself for the failed rank in the channel

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  FailedRank      - Failed Rank number

  @retval Status          - TRUE - there is potential pair rank for the failed rank
                          - FALSE- no potential pair rank for the failed rank

**/
BOOLEAN
EFIAPI
IsThereAnyPotentialPairRank (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         FailedRank
  )
{
  UINT8             Socket;
  UINT8             ChOnSkt;
  MEM_TOPOLOGY      *MemTopology;
  UINT8             PairRank;
  UINT8             Dimm;
  UINT8             Rank;

  RAS_ASSERT (NodeId < MC_MAX_NODE);
  RAS_ASSERT (ChOnNode < MAX_MC_CH);

  Socket = NODE_TO_SKT (NodeId);
  ChOnSkt= NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  MemTopology = GetMemTopology ();

  PairRank = 0;
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm ++) {
    if (MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].DimmInfo[Dimm].AepDimmPresent) {
      continue;
    }
    for (Rank = 0; Rank < (MAX_RANK_DIMM * SUB_CH); Rank++) {
      if (MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].DimmInfo[Dimm].rankList[Rank].enabled) {
        //
        // Ensure that the non failed rank chosen is of the same size as failed rank.
        //
        if ( (GetRankSize (NodeId, ChOnNode, Rank)) != (GetRankSize (NodeId, ChOnNode, FailedRank))) {
          continue;
        }
        if ((FailedRank + Rank) % SUB_CH != 0) {
          //
          // Failed rank and buddy rank must be in the same sub-channel for DDR5.
          //
          continue;
        }
        PairRank++;
      }
    }
  }

  if (PairRank > 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Find out pair rank for bank VLS

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  Rank            - Rank number
  @param  pPairRank       - Pointer to pair rank of the failed rank

  @retval Status          - EFI_SUCCESS      - found pair rank
                          - EFI_UNSUPPORTED  - non pair rank found

**/
EFI_STATUS
EFIAPI
FindBankVlsPairRank (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank,
  OUT UINT8         *pPairRank OPTIONAL
  )
{
  EFI_STATUS          Status;
  UINT8               RegionNum;
  UINT8               Skt;
  UINT8               ChOnSkt;
  UINT8               FailCs;
  UINT8               NonFailCs;
  UINT8               RegionSize;

  //
  //Locate the region which has done VLS for the rank
  //
  Status = FindRegionWithRank (NodeId, ChOnNode, Rank, &RegionNum);
  RAS_DEBUG ( (LEVEL_BASIC_FLOW, "\t FindRegionWithRankBank(Node %d, Ch %d, Rank %d, RegionNum = %d) = %r\n",
              NodeId, ChOnNode, Rank, RegionNum, Status));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Skt = NODE_TO_SKT (NodeId);
  ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  GetAdddcRegion (Skt, ChOnSkt, RegionNum, NULL, &FailCs, NULL, &NonFailCs, NULL, &RegionSize, NULL, NULL);

  if (RegionSize == ADDDC_REGION_SIZE_BANK) {
    if (pPairRank != NULL) {
      if (Rank == FailCs) {
        *pPairRank = NonFailCs;
      } else {
        *pPairRank = FailCs;
      }
    }
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}

/**
  Checks if the rank is in ADDDC region

  @param NodeId         - Memory controller ID
  @param ChOnNode       - Channel number
  @param Rank           - Rank number

  @retval TRUE          - Rank is in ADDDC
  @retval TRUE          - Rank not in ADDDC
**/
BOOLEAN
IsRankinAdddc (
  IN  UINT8   NodeId,
  IN  UINT8   ChOnNode,
  IN  UINT8   Rank
  )
{
  ADDDC_RANK_MODE   RankMode;

  RankMode = GetRankMode (NodeId, ChOnNode, Rank);
  if ((RankMode == RankVlsPrimaryMode) || (RankMode == RankVlsBuddyMode)) {
    return TRUE;
  }
  if ((RankMode == NonRankVlsMode) && GetBankVlsCount (NodeId, ChOnNode, Rank)) {
    return TRUE;
  }
  return FALSE;
}
