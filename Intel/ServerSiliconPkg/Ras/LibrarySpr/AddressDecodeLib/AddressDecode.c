/** @file
  Implementation of system address translation.

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
#include <RcRegs.h>
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#include <Library/AddressDecodeLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/SystemInfoLib.h>
#include <MemDecodeCommonIncludes.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Upi/KtiHost.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/BaseMemoryLib.h>
#include "AddressDecodeInternal.h"
#include "Mirror.h"

extern EFI_GUID gEfiKtiHostOutDataHobGuid;

#define MC_ROUTE_TABLE          0
#define CH_ROUTE_TABLE          1
#define MAX_REGION_SVL          4

#define VGA_RAM_BASE            0xA0000

#define MAX_MODE_TABLES_IMC   2  //Mode0 and Mode1 per IMC

#define RT0_INTERLEAVE_MODE_XOR       0 // RT0 index = [10:8] ^ [16:14] ^ [24:22]
#define RT0_INTERLEAVE_MODE_DEFEATURE 1 // RT0 index = [10:8]

#define RTx_INTLV_SHIFT_6 0
#define RTx_INTLV_SHIFT_7 1
#define RTx_INTLV_SHIFT_8 2
#define RTx_INTLV_SHIFT_9 3
#define RTx_INTLV_SHIFT_10 4
#define RTx_INTLV_SHIFT_11 5
#define RTx_INTLV_SHIFT_12 6
#define RTx_INTLV_SHIFT_13 7

#define INTERNAL_TAD_RULES  20


DRAM_SCOPE                  *mDramScope;
AD_SOCKET_INFO              *mSadTadScope;
struct SystemMemoryMapHob  *SystemMemoryMap = NULL;
UINT32                      mSocketPresentBitMap;

/**
  check whether this system address belong to CXL bus.

  @param[in]       SystemAddress    System address to be translated
  @param[in/out]  Socket           Point to socket. If not NULL, it tells this CXL bus belong to which socket.

  @retval          TRUE  ---  this address belong to CXL bus.
                   FALSE ---  this address not belong to CXL bus.

**/
BOOLEAN
EFIAPI
IsCxlAddress (
  IN        UINTN               SystemAddress,
  IN OUT    UINT8              *Socket
  )
{
  UINT8     TargetId;
  UINT8     TargetSocket;
  UINT8     ClusterIndex;
  UINT8     DramRuleIndex;
  BOOLEAN   Flag;

  if (IsDramMemoryAddress(SystemAddress) == FALSE) {
    DEBUG ((EFI_D_ERROR, "System address:%lx is not DRAM address \n", SystemAddress));
    return FALSE;
  }

  if (ForwardDecodeBlockAddress (SystemAddress, NULL, NULL, NULL) == TRUE) {
    DEBUG ((EFI_D_ERROR, "System address:%lx is block mode address \n", SystemAddress));
    return FALSE;
  }

  TargetSocket = GetSocketId (SystemAddress);

  ClusterIndex = GetClusterPerSystemMode (SystemAddress, TargetSocket);

  if (GetTargetId(SystemAddress, TargetSocket, &ClusterIndex, &TargetId, &DramRuleIndex)) {
    if (IsRemoteSocketTargetId (TargetId, NULL)) {
      DEBUG ((EFI_D_ERROR, "[IsCxlAddress] error on socket:%d cluster:%d\n. It should not be remote target ID.", TargetSocket, ClusterIndex));
      return FALSE;
    }

  } else {
      DEBUG ((EFI_D_ERROR, "[IsCxlAddress] error on socket:%d cluster:%d\n", TargetSocket, ClusterIndex));
      return FALSE;
  }

  Flag = IsCxlDramRule (TargetSocket, ClusterIndex, DramRuleIndex);

  if (Flag) {
    if (Socket != NULL) {
      *Socket = TargetSocket;
    }
  }

  return Flag;
}




/**
  Determines TargetId and ChannelId that maps given system Address
  Modifies the following fields in ADDRESS_TRANSATION structure:
    SocketId
    MemoryControllerId
    ChannelId

 @param  [in/out] TranslatedAddress    -- pointer to the structure containing DIMM Address
 @param  [out]    SktId                -- index of the socket
 @param  [out]    McId                 -- physical memory node index
 @param  [out]    ChId                 -- physical chanel index

  @retval EFI_SUCCESS                   SAD translation is successful
          EFI_INVALID_PARAMETER         SAD translation is not successful


**/
EFI_STATUS
EFIAPI
TranslateSad (
  IN OUT TRANSLATED_ADDRESS * CONST TranslatedAddress,
  OUT UINT8 *SktId,
  OUT UINT8 *McId,
  OUT UINT8 *ChId

 )
{
  UINT64    SystemAddress;
  UINT8     TargetId;
  UINT8     McTargetId;
  UINT8     McChId;
  UINT8     TargetSocket;
  UINT8     ClusterIndex;
  UINT8     DramRuleIndex;

  SystemAddress = TranslatedAddress->SystemAddress;
  TargetId = 0;
  McTargetId = 0;
  McChId = 0;
  TargetSocket = 0;

  TargetSocket = GetSocketId (SystemAddress);
  ClusterIndex = GetClusterPerSystemMode (SystemAddress, TargetSocket);

  if (GetTargetId(SystemAddress, TargetSocket, &ClusterIndex, &TargetId, &DramRuleIndex)) {
    if (IsRemoteSocketTargetId (TargetId, NULL)) {
      DEBUG ((EFI_D_ERROR, "[TranslateSad] error on socket:%d cluster:%d\n. It should not be remote target ID.\n", TargetSocket, ClusterIndex));
    }

  } else {
      DEBUG ((EFI_D_ERROR, "[TranslateSad] error on socket:%d cluster:%d\n", TargetSocket, ClusterIndex));
  }

  if (IsCxlDramRule (TargetSocket, ClusterIndex, DramRuleIndex)) {
    DEBUG ((EFI_D_ERROR, "[TranslateSad] This address locates at CXL device of S:%d Cluster:%d DramRule:%d \n", TargetSocket, ClusterIndex, DramRuleIndex));
    return EFI_UNSUPPORTED; // SAD end.
  }

  if (IsNxmDramRule (TargetSocket, ClusterIndex, DramRuleIndex)) {
    DEBUG ((EFI_D_ERROR, "[TranslateSad] This address locates at NXM range of S:%d Cluster:%d DramRule:%d \n", TargetSocket, ClusterIndex, DramRuleIndex));
    return EFI_UNSUPPORTED;  // SAD end.
  }

  McTargetId = GetMcId (SystemAddress, TargetSocket, ClusterIndex, DramRuleIndex, TargetId & 0x07);
  McChId = GetChId (SystemAddress, TargetSocket, ClusterIndex, DramRuleIndex, TargetId & 0x07);

  if (IsNmCache(TargetSocket, ClusterIndex, DramRuleIndex) == FALSE) {
    if (IsHbmDimm (TargetSocket, McTargetId)) {
      TranslatedAddress->MemType = MemType1lmHbm;
    } else {
      TranslatedAddress->MemType = MemType1lmDdr;
    }
    TranslatedAddress->Flag  |= MEMTYPE_VALID;

    TranslatedAddress->SocketId = TargetSocket;
    TranslatedAddress->MemoryControllerId = McTargetId;
    TranslatedAddress->ChannelId = McChId;
    TranslatedAddress->Flag |= SOCKET_VALID | MC_VALID | CHN_VALID;

    DEBUG ((EFI_D_ERROR, "TranslatedAddress->SocketId is 0x%x \n", TranslatedAddress->SocketId));
    DEBUG ((EFI_D_ERROR, "TranslatedAddress->MemoryControllerId is 0x%x \n", TranslatedAddress->MemoryControllerId));
    DEBUG ((EFI_D_ERROR, "TranslatedAddress->ChannelId is 0x%x \n", TranslatedAddress->ChannelId));

  } else {
    TranslatedAddress->MemType = MemType2lmDdrCacheMemoryMode;
    TranslatedAddress->Flag  |= MEMTYPE_VALID;

    TranslatedAddress->SocketId = TargetSocket;
    TranslatedAddress->NmMemoryControllerId = McTargetId;
    TranslatedAddress->NmChannelId = McChId;
    TranslatedAddress->Flag |= SOCKET_VALID | NM_MC_VALID | NM_CHN_VALID;

    DEBUG ((EFI_D_ERROR, "TranslatedAddress->SocketId is 0x%x \n", TranslatedAddress->SocketId));
    DEBUG ((EFI_D_ERROR, "TranslatedAddress->NmMemoryControllerId is 0x%x \n", TranslatedAddress->NmMemoryControllerId));
    DEBUG ((EFI_D_ERROR, "TranslatedAddress->NmChannelId is 0x%x \n", TranslatedAddress->NmChannelId));
  }


  if (SktId != NULL) {
    *SktId = TargetSocket;
  }
  if (McId != NULL) {
    *McId = McTargetId;
  }
  if (ChId != NULL) {
    *ChId = McChId;
  }

  return EFI_SUCCESS;

 }



 /**
 Translates ChannelAddress to get RankAddress and PhysicalRankId

 @param  [in/out] pTranslatedAddress   -- pointer to the structure containing DIMM Address
 @param  [in] ChnAddress               -- channel address
 @param  [in] SktId                    -- index of the socket
 @param  [in] McTgtId                  -- index of memory controller
 @param  [in] McChId                   -- index of memory channel
 @param  [in] DdrtDimm                 -- ddrt dimm. TRUE -- it is DDRT dimm.

  @retval EFI_SUCCESS                   RIR translation is successful
          EFI_INVALID_PARAMETER         RIR translation is not successful


**/
EFI_STATUS
EFIAPI
TranslateRir (
  IN OUT TRANSLATED_ADDRESS * CONST TranslatedAddress,
  IN     UINT64                     ChnAddress,
  IN     UINT8                      SktId,
  IN     UINT8                      McTgtId,
  IN     UINT8                      McChId,
  IN     BOOLEAN                    DdrtDimm
 )
{

  UINT64    RankAddress;
  UINT8     Rank;
  UINT8     SubRank;
  UINT8     Bank;
  UINT32    Row;
  UINT32    Column;

  if (DdrtDimm == FALSE) {
    //
    // this flow cover following case.
    // DDR5/HBM 1LM
    // DDR5/HBM NM
    // DDR5/HBM FM
    //
    ForwardRirDecode (TranslatedAddress->SystemAddress, SktId, McTgtId, McChId, ChnAddress, &Rank , &RankAddress);
    ForwardAmapDecode (RankAddress, SktId, McTgtId, McChId, Rank >> 2, &SubRank, &Bank, &Row, &Column);

    //
    // DDR5/HBM NM
    //
    if ((TranslatedAddress->Flag & (NM_CHN_ADDR_VALID | CHN_ADDR_VALID) ) == NM_CHN_ADDR_VALID) {
      TranslatedAddress->NmRankAddress = RankAddress;
      TranslatedAddress->NmChipSelect = Rank;
      if (IsHbmDimm (SktId, McTgtId)) {
        TranslatedAddress->NmPhysicalRankId = Rank % MAX_RANK_DIMM;
        TranslatedAddress->NmDimmSlot = Rank >> 2;
      } else {
        TranslatedAddress->NmPhysicalRankId = (Rank >> 1) % MAX_RANK_DIMM;
        TranslatedAddress->NmDimmSlot = (Rank >> 1) >> 2;
      }

      DEBUG ((EFI_D_ERROR, "    NmRankAddress: 0x%lx  NmChipSelect:0x%x   NmPhysicalRankId:0x%x  NmDimmSlot:0x%x\n",
               TranslatedAddress->NmRankAddress, TranslatedAddress->NmChipSelect , TranslatedAddress->NmPhysicalRankId, TranslatedAddress->NmDimmSlot));

      TranslatedAddress->NmCol = Column;
      TranslatedAddress->NmRow = Row;
      TranslatedAddress->NmBank = Bank & 3;
      TranslatedAddress->NmBankGroup = Bank >> 2;
      DEBUG ((EFI_D_ERROR, "    NmCol:0x%x  NmRow:0x%x   NmBank:0x%x  NmBankGroup:0x%x \n", TranslatedAddress->NmCol,
                           TranslatedAddress->NmRow, TranslatedAddress->NmBank, TranslatedAddress->NmBankGroup));
      TranslatedAddress->Flag |=  NM_RANK_ADDRESS_VALID | NM_CHIP_SELECT_VALID | NM_PYSICAL_RANK_VALID | NM_DIMM_VALID | NM_COLUMN_VALID | NM_ROW_VALID
                                  | NM_BANK_VALID | NM_BANK_GROUP_VALID;
      if (SubRank != 0xff) {
        TranslatedAddress->NmChipId = SubRank;
        DEBUG ((EFI_D_ERROR, "    TranslatedAddress->ChipId  is 0x%x\n", TranslatedAddress->ChipId ));
        TranslatedAddress->Flag |=SUB_RANK_VALID;
      }
    } else {
      TranslatedAddress->RankAddress = RankAddress;
      TranslatedAddress->ChipSelect = Rank;

      TranslatedAddress->PhysicalRankId = (Rank ) % MAX_RANK_DIMM;
      TranslatedAddress->DimmSlot = Rank >> 2;



      DEBUG ((EFI_D_ERROR, "    RankAddress:0x%lx   ChipSelect:0x%x   PhysicalRankId:0x%x   DimmSlot:0x%x\n",
        TranslatedAddress->RankAddress,TranslatedAddress->ChipSelect, TranslatedAddress->PhysicalRankId, TranslatedAddress->DimmSlot));


      TranslatedAddress->Col = Column;
      TranslatedAddress->Row = Row;
      TranslatedAddress->Bank = Bank & 3;
      TranslatedAddress->BankGroup = Bank >> 2;
      DEBUG ((EFI_D_ERROR, "    Col:0x%x   Row:0x%x    BankGroup:0x%x   Bank:0x%x\n",
        TranslatedAddress->Col, TranslatedAddress->Row, TranslatedAddress->BankGroup, TranslatedAddress->Bank));

      TranslatedAddress->Flag |=  RANK_ADDRESS_VALID | CHIP_SELECT_VALID | PYSICAL_RANK_VALID | DIMM_VALID | COLUMN_VALID | ROW_VALID | BANK_VALID | BANK_GROUP_VALID;
      if (SubRank != 0xff) {
        TranslatedAddress->ChipId = SubRank;
        DEBUG ((EFI_D_ERROR, "    TranslatedAddress->ChipId  is 0x%x\n", TranslatedAddress->ChipId ));
        TranslatedAddress->Flag |=SUB_RANK_VALID;
      }
    }
  } else {
    ForwardDdrtRirDecode (TranslatedAddress->SystemAddress, SktId, McTgtId, McChId, ChnAddress, &Rank , &RankAddress);
    TranslatedAddress->DPA = RankAddress;
    TranslatedAddress->ChipSelect = Rank;
    TranslatedAddress->PhysicalRankId = Rank  % MAX_RANK_DIMM;
    TranslatedAddress->DimmSlot = Rank >> 2;

    DEBUG ((EFI_D_ERROR, "    ChipSelect:0x%x   PhysicalRankId:0x%x   DPA:0x%x\n",
       TranslatedAddress->ChipSelect, TranslatedAddress->PhysicalRankId,TranslatedAddress->DPA));

    TranslatedAddress->Flag |= DPA_VALID | CHIP_SELECT_VALID | PYSICAL_RANK_VALID | DIMM_VALID;
  }
  return EFI_SUCCESS;
}

VOID
EFIAPI
UpdateMemType (
  IN OUT   TRANSLATED_ADDRESS * CONST TranslatedAddress,
  IN       UINT8       MemType,
  IN       UINT8       Socket,
  IN       UINT8       Mc,
  IN       UINT8       Sad2TadIndex
)
{
  UINT8    DimmType = ddr4dimmType;
  UINT8    UpdatedMemType = MemTypeNone;

  if(mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].PmemVld == 1) {
    if (MemType == MemType2lmDdrCacheMemoryMode) {
      UpdatedMemType = MemType2lmDdrWbCacheAppDirect;

    } else {
      UpdatedMemType = MemType1lmAppDirect;
    }
    DimmType = ddrtdimmType;
  }

  if ((mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Ddr4Flag == 0) &&
      (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].NmCacheableVld == 1) &&
      (MemType == MemType2lmDdrCacheMemoryMode)) {
        UpdatedMemType = MemType2lmDdrCacheMemoryMode;
        DimmType = ddrtdimmType;
   }

  if ((mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Ddr4Flag == 1) &&
      (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].NmCacheableVld == 1) &&
      (MemType == MemType2lmDdrCacheMemoryMode)) {
        UpdatedMemType = MemType2lmHbmCacheDdr;
        DimmType = ddr4dimmType;
   }

  if ((mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].Ddr4Flag == 1) &&
      (mSadTadScope[Socket].McInfo[Mc].Sad2Tad[Sad2TadIndex].NmCacheableVld == 0) &&
      (MemType != MemType2lmDdrCacheMemoryMode)) {
        UpdatedMemType = MemType1lmDdr;
        DimmType = ddr4dimmType;
   }

   if (IsHbmDimm(Socket, Mc)) {
        UpdatedMemType = MemType1lmHbm;
        DimmType = HbmdimmType;
   }
   TranslatedAddress->MemType = UpdatedMemType;
   TranslatedAddress->DimmType = DimmType;
   TranslatedAddress->Flag |= MEMTYPE_VALID | DIMMTYPE_VALID;
   switch (TranslatedAddress->MemType) {
     case MemType1lmDdr:
       DEBUG ((EFI_D_ERROR, "MemType1lmDdr\n"));
       break;
     case MemType1lmAppDirect:
       DEBUG ((EFI_D_ERROR, "MemType1lmAppDirect\n"));
       break;
     case MemType1lmCtrl:
       DEBUG ((EFI_D_ERROR, "MemType1lmCtrl\n"));
       break;
     case MemType1lmHbm:
       DEBUG ((EFI_D_ERROR, "MemType1lmHbm\n"));
       break;
     case MemType2lmDdrCacheMemoryMode:
       DEBUG ((EFI_D_ERROR, "MemType2lmDdrCacheMemoryMode\n"));
       break;
     case MemType2lmDdrWbCacheAppDirect:
       DEBUG ((EFI_D_ERROR, "MemType2lmDdrWbCacheAppDirect\n"));
       break;
     case MemType2lmHbmCacheDdr:
       DEBUG ((EFI_D_ERROR, "MemType2lmHbmCacheDdr\n"));
       break;
     case MemType2lmHbmCacheMemoryMode:
       DEBUG ((EFI_D_ERROR, "MemType2lmHbmCacheMemoryMode\n"));
       break;
     default:
       DEBUG ((EFI_D_ERROR, "unknown memory type\n"));
   }

   switch (TranslatedAddress->DimmType) {
     case ddr4dimmType:
       DEBUG ((EFI_D_ERROR, "ddr4dimmType\n"));
       break;
     case ddrtdimmType:
       DEBUG ((EFI_D_ERROR, "ddrtdimmType\n"));
       break;
     case HbmdimmType:
       DEBUG ((EFI_D_ERROR, "HbmdimmType\n"));
       break;
     default:
       DEBUG ((EFI_D_ERROR, "unknown dimm type\n"));
   }
}

 /**
 Update Translated Address information with matched AdddcCtrRegion.
 The original address has been remaped by adddc.
 This function is for ddr5 acting as 1LM or FM.

 @param  [in/out] TA                   -- pointer to the structure containing DIMM Address
 @param  [in] AdddcCtrRegion           -- pointer to matched AdddcCtrRegion.
 @param  [in] MatchedRegionType        -- rank/bank matched. 1 - rank, 2 - bank.
 @param  [in] SwapFlag                 -- failed/non-failed matched. 0 - failed, 1 - non-failed.

 @retval None


**/
VOID
EFIAPI
Adddc1lmUpdate (
  IN OUT  TRANSLATED_ADDRESS                    * CONST TA,
  IN      ADDDC_REGION0_CONTROL_MC_MAIN_STRUCT  * AdddcCtrRegion,
  IN      UINT8                                   MatchedRegionType,
  IN      BOOLEAN                                 SwapFlag
)
{
  // failed -> non-failed
  if (SwapFlag == 0) {
    if (MatchedRegionType >= 1) {
      TA->LockStepRank          = ((UINT8)AdddcCtrRegion->Bits.nonfailed_cs & 0x3) ;
      TA->LockStepPhysicalRank  = ((TA->LockStepRank >> 1) % MAX_RANK_DIMM);
      TA->RasModesApplied       |= BIT1; // Rank VLS

      if (MatchedRegionType == 2) {
        TA->LockStepBank          = (UINT8)AdddcCtrRegion->Bits.nonfailed_ba;
        TA->LockStepBG            = (UINT8)AdddcCtrRegion->Bits.nonfailed_bg;
        TA->RasModesApplied       &= ~(BIT1);
        TA->RasModesApplied       |= BIT2;  //Bank VLS
      }

      //Update Rank/Bank/BankGroup if SystemAddress is located in high CL
      if ((TA->SystemAddress & BIT5) == 0) {
        return;
      }

      TA->ChipSelect        = TA->LockStepRank;
      TA->PhysicalRankId    = TA->LockStepPhysicalRank;
      TA->DimmSlot          = TA->LockStepRank >> 2;

      if (MatchedRegionType == 2) {
        TA->Bank              = (UINT8)AdddcCtrRegion->Bits.nonfailed_ba;
        TA->BankGroup         = (UINT8)AdddcCtrRegion->Bits.nonfailed_bg;
        TA->ChipId            = (UINT8)AdddcCtrRegion->Bits.nonfailed_c;
      }
    }
  } else if (SwapFlag == 1) {
    //Non-failed->failed
    if (MatchedRegionType >= 1) {
      TA->LockStepRank          = ((UINT8)AdddcCtrRegion->Bits.failed_cs & 0x3) ;
      TA->LockStepPhysicalRank  = ((TA->LockStepRank >> 1) % MAX_RANK_DIMM);
      TA->RasModesApplied       |= BIT1; // Rank VLS

      if (MatchedRegionType == 2) {
        TA->LockStepBank          = (UINT8)AdddcCtrRegion->Bits.failed_ba;
        TA->LockStepBG            = (UINT8)AdddcCtrRegion->Bits.failed_bg;
        TA->RasModesApplied       &= ~(BIT1);
        TA->RasModesApplied       |= BIT2;  //Bank VLS
      }

      //Update Rank/Bank/BankGroup if SystemAddress is located in high CL
      if ((TA->SystemAddress & BIT5) == 0) {
        return;
      }

      TA->ChipSelect        = TA->LockStepRank;
      TA->PhysicalRankId    = TA->LockStepPhysicalRank;
      TA->DimmSlot          = TA->LockStepRank >> 2;

      if (MatchedRegionType == 2) {
        TA->Bank              = (UINT8)AdddcCtrRegion->Bits.failed_ba;
        TA->BankGroup         = (UINT8)AdddcCtrRegion->Bits.failed_bg;
        TA->ChipId            = (UINT8)AdddcCtrRegion->Bits.failed_c;
      }
    }
  }
}

 /**
 Update Translated Address information with matched AdddcCtrRegion.
 The original address has been remaped by adddc.
 This function is for ddr5 acting as NM.

 @param  [in/out] TA                   -- pointer to the structure containing DIMM Address
 @param  [in] AdddcCtrRegion           -- pointer to matched AdddcCtrRegion.
 @param  [in] MatchedRegionType        -- rank/bank matched. 1 - rank, 2 - bank.
 @param  [in] SwapFlag                 -- failed/non-failed matched. 0 - failed, 1 - non-failed.

 @retval None


**/
VOID
EFIAPI
Adddc2lmUpdate (
  IN OUT  TRANSLATED_ADDRESS                    * CONST TA,
  IN      ADDDC_REGION0_CONTROL_MC_MAIN_STRUCT  * AdddcCtrRegion,
  IN      UINT8                                   MatchedRegionType,
  IN      BOOLEAN                                 SwapFlag
)
{
  // failed -> non-failed
  if (SwapFlag == 0) {
    if (MatchedRegionType >= 1) {
      TA->LockStepRank          = ((UINT8)AdddcCtrRegion->Bits.nonfailed_cs & 0x3) ;
      TA->LockStepPhysicalRank  = ((TA->LockStepRank >> 1) % MAX_RANK_DIMM);
      TA->RasModesApplied       |= BIT1; // Rank VLS

      if (MatchedRegionType == 2) {
        TA->LockStepBank          = (UINT8)AdddcCtrRegion->Bits.nonfailed_ba;
        TA->LockStepBG            = (UINT8)AdddcCtrRegion->Bits.nonfailed_bg;
        TA->RasModesApplied       &= ~(BIT1);
        TA->RasModesApplied       |= BIT2;  //Bank VLS
      }

      //Update Rank/Bank/BankGroup if SystemAddress is located in high CL,
      //Using the SystemAddress here is because the NM is aligned with FM.
      if ((TA->SystemAddress & BIT5) == 0) {
        return;
      }

      TA->NmChipSelect        = TA->LockStepRank;
      TA->NmPhysicalRankId    = TA->LockStepPhysicalRank;
      TA->NmDimmSlot          = TA->LockStepRank >> 2;

      if (MatchedRegionType == 2) {
        TA->NmBank              = (UINT8)AdddcCtrRegion->Bits.nonfailed_ba;
        TA->NmBankGroup         = (UINT8)AdddcCtrRegion->Bits.nonfailed_bg;
        TA->NmChipId            = (UINT8)AdddcCtrRegion->Bits.nonfailed_c;
      }
    }
  } else if (SwapFlag == 1) {
    //Non-failed->failed
    if (MatchedRegionType >= 1) {
      TA->LockStepRank          = ((UINT8)AdddcCtrRegion->Bits.failed_cs & 0x3) ;
      TA->LockStepPhysicalRank  = ((TA->LockStepRank >> 1) % MAX_RANK_DIMM);
      TA->RasModesApplied       |= BIT1; // Rank VLS

      if (MatchedRegionType == 2) {
        TA->LockStepBank          = (UINT8)AdddcCtrRegion->Bits.failed_ba;
        TA->LockStepBG            = (UINT8)AdddcCtrRegion->Bits.failed_bg;
        TA->RasModesApplied       &= ~(BIT1);
        TA->RasModesApplied       |= BIT2;  //Bank VLS
      }

      //Update Rank/Bank/BankGroup if SystemAddress is located in high CL
      if ((TA->SystemAddress & BIT5) == 0) {
        return;
      }

      TA->NmChipSelect        = TA->LockStepRank;
      TA->NmPhysicalRankId    = TA->LockStepPhysicalRank;
      TA->NmDimmSlot          = TA->LockStepRank >> 2;

      if (MatchedRegionType == 2) {
        TA->NmBank              = (UINT8)AdddcCtrRegion->Bits.failed_ba;
        TA->NmBankGroup         = (UINT8)AdddcCtrRegion->Bits.failed_bg;
        TA->NmChipId            = (UINT8)AdddcCtrRegion->Bits.failed_c;
      }
    }
  }
}

 /**

 If ADDDC happened, update Translated Address information with matched AdddcCtrRegion.

 @param  [in/out] TA                   -- pointer to the structure containing DIMM Address

 @retval None


**/
VOID
EFIAPI
AdddcSupportForAddressTranslation (
  IN OUT TRANSLATED_ADDRESS * CONST TA
)
{
  UINT8             SktId, McId, ChId, NodeId;
  UINT8             Rank, ChipId;
  UINT8             Bank;
  UINT8             BankGroup;
  UINT8             SktCh;
  UINT8             RegionNum;
  UINT8             MatchedRegionType; // 1-rank, 2-bank
  BOOLEAN           SwapFlag; // update TA 0: failed->nonfailed; 1: nonfailed->failed

  ADDDC_REGION0_CONTROL_MC_MAIN_STRUCT  AdddcCtrRegion;
  UINT32 AdddcRegionReg[MAX_REGION_SVL] = {
    ADDDC_REGION0_CONTROL_MC_MAIN_REG,
    ADDDC_REGION1_CONTROL_MC_MAIN_REG,
    ADDDC_REGION2_CONTROL_MC_MAIN_REG,
    ADDDC_REGION3_CONTROL_MC_MAIN_REG
  };

  SktId         = TA->SocketId;
  NodeId        = TA->Node;
  if (TA->MemType == MemType2lmDdrCacheMemoryMode) {
    ChId          = TA->NmChannelId;
    Bank          = TA->NmBank;
    BankGroup     = TA->NmBankGroup;
    McId          = TA->NmMemoryControllerId;
    SktCh         = (McId * MAX_MC_CH) + ChId;
    Rank          = TA->NmChipSelect; //Chip Select
    ChipId        = TA->NmChipId;
  } else {
    ChId          = TA->ChannelId;
    Bank          = TA->Bank;
    BankGroup     = TA->BankGroup;
    McId          = TA->MemoryControllerId;
    SktCh         = (McId * MAX_MC_CH) + ChId;
    Rank          = TA->ChipSelect; //Chip Select
    ChipId        = TA->ChipId;
  }
  if (ChipId == 0xFF) {
    ChipId = 0;//subrank is zero in adddcCtlRegion when subrank is not enabled.
  }
  DEBUG ((EFI_D_INFO, "\nADDDCSupportForAddrTrans\n"));
  for (RegionNum = 0; RegionNum < MAX_REGION_SVL; RegionNum++) {
    AdddcCtrRegion.Data = ReadCpuCsr(SktId, SktCh, AdddcRegionReg[RegionNum]);

    if (AdddcCtrRegion.Bits.region_enable == 0) {
      continue;
    }
    DEBUG ((EFI_D_INFO, "\nADDDCSupportForAddrTrans: Region %d enabled\n",RegionNum));

    MatchedRegionType = 0;
    if ((Rank == AdddcCtrRegion.Bits.failed_cs)) {
      SwapFlag = 0;
      if (AdddcCtrRegion.Bits.region_size == ADDDC_REGION_SIZE_RANK) {
        MatchedRegionType = 1; // Rank matched
      }
      if ((AdddcCtrRegion.Bits.region_size == ADDDC_REGION_SIZE_BANK) &&
          (Bank == AdddcCtrRegion.Bits.failed_ba) &&
          (ChipId == AdddcCtrRegion.Bits.failed_c) &&
          (BankGroup == AdddcCtrRegion.Bits.failed_bg)) {
        MatchedRegionType = 2; // Bank matched
      }
    } else if ((Rank == AdddcCtrRegion.Bits.nonfailed_cs)) {
      SwapFlag = 1;
      if (AdddcCtrRegion.Bits.region_size == ADDDC_REGION_SIZE_RANK) {
        MatchedRegionType = 1; // Rank matched
      }
      if ((AdddcCtrRegion.Bits.region_size == ADDDC_REGION_SIZE_BANK) &&
          (Bank == AdddcCtrRegion.Bits.nonfailed_ba) &&
          (ChipId == AdddcCtrRegion.Bits.nonfailed_c) &&
          (BankGroup == AdddcCtrRegion.Bits.nonfailed_bg)) {
        MatchedRegionType = 2; // Bank matched
      }
    } else {
      continue;
    }

    if (TA->MemType == MemType2lmDdrCacheMemoryMode) {
      Adddc2lmUpdate(TA, &AdddcCtrRegion, MatchedRegionType, SwapFlag);
    } else {
      Adddc1lmUpdate(TA, &AdddcCtrRegion, MatchedRegionType, SwapFlag);
    }
    break;
  } // Region loop
  if (TA->RasModesApplied & (BIT1|BIT2)) {
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->LockStepRank:%08lx\n",TA->LockStepRank));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->LockStepPhysicalRank:%08lx\n",TA->LockStepPhysicalRank));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->LockStepBank:%08lx\n",TA->LockStepBank));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->LockStepBG:%08lx\n",TA->LockStepBG));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->RasModesApplied:%08lx\n",TA->RasModesApplied));
    DEBUG ((EFI_D_INFO, "\nADDDCSupportForAddrTrans: TA->ChipSelect:%08lx\n",TA->ChipSelect));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->PhysicalRankId:%08lx\n",TA->PhysicalRankId));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->DimmSlot:%08lx\n",TA->DimmSlot));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->Bank:%08lx\n",TA->Bank));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->BankGroup:%08lx\n",TA->BankGroup));
    DEBUG ((EFI_D_INFO, "\nADDDCSupportForAddrTrans: TA->NmChipSelect:%08lx\n",TA->NmChipSelect));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->NmPhysicalRankId:%08lx\n",TA->NmPhysicalRankId));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->NmDimmSlot:%08lx\n",TA->NmDimmSlot));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->NmBank:%08lx\n",TA->NmBank));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->NmBankGroup:%08lx\n",TA->NmBankGroup));
    DEBUG ((EFI_D_INFO, "ADDDCSupportForAddrTrans: TA->NmChipId:%08lx\n",TA->NmChipId));
  }
  return;
}

/**

   Translate System Address at M2M/MC level. Covers tad/rir/amap.

    @param  [in/out] TranslatedAddress   -- pointer to the structure containing DIMM Address

    @retval EFI_SUCCESS / Error code


**/
EFI_STATUS
EFIAPI
TranslateMc (
  IN OUT TRANSLATED_ADDRESS * CONST TranslatedAddress
  )
{
  EFI_STATUS Status;
  UINT64    SystemAddress;
  UINT64    ChannelAddress = 0;
  UINT8     SktId;
  UINT8     McTgtId;
  UINT8     McChId;
  UINT8     TadIndex;
  UINT8     MemType;
  UINT64    NmChnAddr;
  UINT8     DdrtFlag = FALSE;
  BOOLEAN   NmDecodeDoneFlag;
  UINT8     FmMc;
  UINT8     FmChn;
  UINT8     TargetWays;
  UINT8     ChannelWays;

  SystemAddress = TranslatedAddress->SystemAddress;
  SktId         = TranslatedAddress->SocketId;
  MemType       = TranslatedAddress->MemType;

  DEBUG ((EFI_D_INFO, "[TranslateMc] MemType:%d \n", MemType));

  NmDecodeDoneFlag = (TranslatedAddress->Flag & NM_CHN_ADDR_VALID)? TRUE:FALSE;

  if ((MemType == MemType2lmDdrCacheMemoryMode) && (NmDecodeDoneFlag == FALSE)) {
    if ((TranslatedAddress->Flag & (NM_MC_VALID | NM_CHN_VALID)) == (NM_MC_VALID | NM_CHN_VALID)) {
      McTgtId       = TranslatedAddress->NmMemoryControllerId;
      McChId        = TranslatedAddress->NmChannelId;
    } else {
      DEBUG ((EFI_D_ERROR, "[TranslateMc] wrong flag on NM tad \n"));
      return EFI_INVALID_PARAMETER;
    }
    NmChnAddr = Forward2lmDecodeGetCa(SystemAddress, SktId, McTgtId, McChId);

    TranslatedAddress->NmChannelAddress = NmChnAddr;
    TranslatedAddress->Flag |= NM_CHN_ADDR_VALID;

    TranslateRir (TranslatedAddress, NmChnAddr, SktId, McTgtId, McChId, FALSE);
    if (IsHbmDimm(SktId, McTgtId) == FALSE) {
      // NM is not HBM.
      AdddcSupportForAddressTranslation(TranslatedAddress);
    }

    //
    //decode FM address.
    //
    if (GetFmInfoFromNmChn(SystemAddress, SktId, McTgtId, McChId, &FmMc, &FmChn)) {
      TranslatedAddress->MemoryControllerId = FmMc;
      TranslatedAddress->ChannelId = FmChn;
      TranslatedAddress->Flag |=  MC_VALID | CHN_VALID;

      DEBUG ((EFI_D_ERROR, "[Decode FM address]\n TranslatedAddress->MemoryControllerId is 0x%x\n", TranslatedAddress->MemoryControllerId));
      DEBUG ((EFI_D_ERROR, "TranslatedAddress->ChannelId is 0x%x \n", TranslatedAddress->ChannelId));

      return TranslateMc(TranslatedAddress);

    } else {
      DEBUG ((EFI_D_ERROR, "[TranslateMc] wrong on NM2FM decode \n"));
      return EFI_UNSUPPORTED;
    }


  } else {
    if ((TranslatedAddress->Flag & (MC_VALID | CHN_VALID)) == (MC_VALID | CHN_VALID)) {
      McTgtId       = TranslatedAddress->MemoryControllerId;
      McChId        = TranslatedAddress->ChannelId;
    } else {
      DEBUG ((EFI_D_ERROR, "[TranslateMc] wrong flag on normal tad \n"));
      return EFI_INVALID_PARAMETER;
    }

    TadIndex = GetChTad (SystemAddress, SktId, McTgtId);
    DEBUG ((EFI_D_INFO, "[TranslateMc] ch tad:%d found \n", TadIndex));

    if (TadIndex == 0xff) {
      DEBUG ((EFI_D_ERROR, "wrong Tad index got \n"));
      return EFI_INVALID_PARAMETER;
    }

    ASSERT (TadIndex < MAX_TAD);

    ChannelAddress = Forward1lmDecodeGetCa (SystemAddress, SktId, McTgtId, McChId, TadIndex);

    if (ChannelAddress == (UINT64) -1) {
      DEBUG ((EFI_D_ERROR, "wrong channel address:%lx calculated \n", ChannelAddress));
      return EFI_INVALID_PARAMETER;
    }

    TranslatedAddress->ChannelAddress = ChannelAddress;
    TranslatedAddress->Flag |= CHN_ADDR_VALID;
    GetTadInfo (SktId, McTgtId, McChId, TadIndex, &TargetWays, &ChannelWays);
    TranslatedAddress->TargetWays = TargetWays;
    TranslatedAddress->ChannelWays = ChannelWays;
    TranslatedAddress->Flag |=  TARGET_WAY_VALID | CHANNEL_WAY_VALID;

    if (MemType == MemType1lmCtrl) {
      DdrtFlag = TRUE;
    } else {
      DdrtFlag = GetDdrFlag (SktId, McTgtId, TadIndex);
      UpdateMemType (TranslatedAddress, MemType, SktId, McTgtId, TadIndex);
    }
    if (IsMirrorFailover (SktId, McTgtId, McChId) && (TranslatedAddress->DimmType == ddr4dimmType) ) {
      //
      // Mirror failover happend and ddr act as FM or 1LM, dont need the primary info
      //
      Status = TranslateMirror (TranslatedAddress);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "TranslateMc is failing on tanslate mirror\n"));
        return Status;
      }
      MoveSec2Pri (TranslatedAddress);
      return EFI_SUCCESS;
    }
    TranslateRir (TranslatedAddress, ChannelAddress,  SktId, McTgtId, McChId, DdrtFlag);
    if (TranslatedAddress->DimmType == ddr4dimmType) {
      // ddr act as FM or 1LM
      TranslateMirror (TranslatedAddress);
      AdddcSupportForAddressTranslation(TranslatedAddress);
    }
    return EFI_SUCCESS;
  }
}

/**

   Translate System Address to DIMM Address

    @param  [in]  SystemAddress  - System Address
    @param  [out] TranslatedAddress   -- pointer to the structure containing DIMM Address

    @retval EFI_SUCCESS / Error code


**/
EFI_STATUS
EFIAPI
TranslateSystemAddress (
  IN UINTN SystemAddress,
  OUT PTRANSLATED_ADDRESS TranslatedAddress
)
{

  EFI_STATUS                  Status;
  UINT8                       SktId, McId, ChId;

  // Initialize  the fields in TA with -1
  TranslatedAddress->SystemAddress      = (UINT64)SystemAddress;
  TranslatedAddress->SocketId           = (UINT8)-1;
  TranslatedAddress->MemoryControllerId = (UINT8)-1;
  TranslatedAddress->ChannelId          = (UINT8)-1;
  TranslatedAddress->DimmSlot           = (UINT8)-1;
  TranslatedAddress->PhysicalRankId     = (UINT8)-1;
  TranslatedAddress->Row                = (UINT32)-1;
  TranslatedAddress->Col                = (UINT32)-1;
  TranslatedAddress->Bank               = (UINT8)-1;
  TranslatedAddress->BankGroup          = (UINT8)-1;
  TranslatedAddress->ChipSelect         = (UINT8)-1;
  TranslatedAddress->ChipId             = (UINT8)-1;

  TranslatedAddress->SecChannelId       = (UINT8)-1;
  TranslatedAddress->SecChannelAddress  = (UINT64)-1;
  TranslatedAddress->SecRankAddress     = (UINT64)-1;
  TranslatedAddress->SecDimmSlot        = (UINT8)-1;
  TranslatedAddress->SecPhysicalRankId  = (UINT8)-1;
  TranslatedAddress->SecRow             = (UINT32)-1;
  TranslatedAddress->SecCol             = (UINT32)-1;
  TranslatedAddress->SecBank            = (UINT8)-1;
  TranslatedAddress->SecBankGroup       = (UINT8)-1;
  TranslatedAddress->SecChipSelect      = (UINT8)-1;
  TranslatedAddress->SecChipId          = (UINT8)-1;

  TranslatedAddress->NmMemoryControllerId = (UINT8)-1;
  TranslatedAddress->NmChannelAddress     = (UINT64)-1;
  TranslatedAddress->NmChannelId          = (UINT8)-1;
  TranslatedAddress->NmRankAddress        = (UINT64)-1;
  TranslatedAddress->NmPhysicalRankId     = (UINT8)-1;
  TranslatedAddress->NmDimmSlot           = (UINT8)-1;
  TranslatedAddress->NmBankGroup          = (UINT8)-1;
  TranslatedAddress->NmRow                = (UINT32)-1;
  TranslatedAddress->NmCol                = (UINT32)-1;
  TranslatedAddress->NmBank               = (UINT8)-1;
  TranslatedAddress->NmChipSelect         = (UINT8)-1;
  TranslatedAddress->NmChipId             = (UINT8)-1;


  TranslatedAddress->Flag = 0;
  TranslatedAddress->RasModesApplied      = 0;

  DEBUG ((EFI_D_ERROR, "\nForward Address Translation start for 0x%lx\n", TranslatedAddress->SystemAddress));

  if (IsDramMemoryAddress(SystemAddress) == FALSE) {
    DEBUG ((EFI_D_ERROR, "System address:%lx is not DRAM address \n", SystemAddress));
    return EFI_INVALID_PARAMETER;
  }

  if (ForwardDecodeBlockAddress (SystemAddress, &SktId, &McId, &ChId) == TRUE) {
    TranslatedAddress->SocketId = SktId;
    TranslatedAddress->MemoryControllerId = McId;
    TranslatedAddress->ChannelId = ChId;
    TranslatedAddress->MemType = MemType1lmCtrl;
    TranslatedAddress->Flag |= SOCKET_VALID | MC_VALID | CHN_VALID | DPA_VALID | MEMTYPE_VALID;
    DEBUG ((EFI_D_ERROR, "block region: Socket:%d  Mc:%d  Ch:%d\n", SktId, McId, ChId));

    Status = TranslateMc (TranslatedAddress);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "TranslateMc is failing on block region\n"));
      return EFI_INVALID_PARAMETER;
    }
    return Status;
  }

  Status = TranslateSad (TranslatedAddress, NULL, NULL, NULL);
  if (Status == EFI_UNSUPPORTED) {
    DEBUG ((EFI_D_ERROR, "TranslateSad End due to address locates at CXL or NXM!\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "TranslateSad is failing\n"));
    return EFI_INVALID_PARAMETER;
  }


  Status = TranslateMc (TranslatedAddress);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "TranslateMc is failing\n"));
    return EFI_INVALID_PARAMETER;
  }
  DEBUG ((EFI_D_INFO, "TranslateMc is successful\n"));

  return Status;
}

/**
  Translate a system address to a DIMM address.

  @param[in]      SystemAddress       System address to be translated
  @param[out]     TranslatedAddress   Pointer to the DIMM Address structure

  @retval EFI_SUCCESS                 Translation was successful
  @retval EFI_INVALID_PARAMETER       System address was invalid
**/
EFI_STATUS
EFIAPI
SystemAddressToDimmAddress (
  IN      UINTN               SystemAddress,
  OUT     PTRANSLATED_ADDRESS TranslatedAddress
  )
{

  EFI_STATUS Status;

  Status = TranslateSystemAddress (SystemAddress, TranslatedAddress);

  if (Status == EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR,"\nForward Address Translation Successful!!\n"));
  } else {
    DEBUG ((EFI_D_ERROR,"\nForward Address Translation is not Successful!!\n"));
  }

  return Status;

}

/**
Check Dimm Address Parameters.

@param  [in] TA   -- pointer to the structure containing DIMM Address

@retval EFI_SUCCESS / Error code
**/
EFI_STATUS
EFIAPI
CheckDimmAddressParams(
  IN  PTRANSLATED_ADDRESS TA
)
{
  UINT8   Skt;
  UINT8   Mc;
  UINT8   Channel;
  UINT8   Rank = 0;
  UINT8   SubRank;
  UINT8   Bank;
  UINT32  Row;
  UINT32  Column;
  UINT8   DimmIndex;
  UINT8   DimmType;
  UINT64  Dpa;
  UINT8   ColBitsNum;
  UINT8   RowBitsNum;
  UINT8   RirIndex;
  UINT64  RirLimit;
  UINT64  MaxRirLimit = 0;

  Skt = TA->SocketId;
  Mc = TA->MemoryControllerId;
  Channel = TA->ChannelId;
  SubRank = TA->ChipId;
  Bank = TA->Bank | (TA->BankGroup << 2);
  Row = TA->Row;
  Column = TA->Col;
  Dpa = TA->DPA;

  if (Skt >= MAX_SOCKET) {
    DEBUG ((EFI_D_ERROR, "Skt %d is not valid, it should be smaller than the max socket number %d \n", Skt, MAX_SOCKET));
    return EFI_INVALID_PARAMETER;
  } else if (IsSocketExist (Skt) == FALSE) {
    DEBUG ((EFI_D_ERROR, "Socket %d is not present \n", Skt));
    return EFI_INVALID_PARAMETER;
  }

  if (Mc >= MAX_SYS_IMC) {
    DEBUG ((EFI_D_ERROR, "Mc %d is not valid, it should be smaller than the max IMC number %d \n", Mc, MAX_SYS_IMC));
    return EFI_INVALID_PARAMETER;
  } else if (mSadTadScope[Skt].McInfo[Mc].McEnabled == FALSE) {
    DEBUG ((EFI_D_ERROR, "MemoryController %d is not enabled \n", Mc));
    return EFI_INVALID_PARAMETER;
  }

  if (Channel >= MAX_CH_PER_MC) {
    DEBUG ((EFI_D_ERROR, "Channel %d is not valid, it should be smaller than the max Channel number %d \n", Channel, MAX_CH_PER_MC));
    return EFI_INVALID_PARAMETER;
  } else if (mSadTadScope[Skt].McInfo[Mc].ChInfo[Channel].ChEnabled == FALSE) {
    DEBUG ((EFI_D_ERROR, "Channel %d is not enabled \n", Channel));
    return EFI_INVALID_PARAMETER;
  }

  //
  //get rank from either ChipSelect or DIMM + rankOnDimm
  //
  if (TA->ChipSelect != 0xFF) {
    Rank = TA->ChipSelect;
  } else if ((TA->DimmSlot != 0xFF) && (TA->PhysicalRankId != 0xFF)) {
    Rank = (TA->DimmSlot << 2) | TA->PhysicalRankId;
  } else {
    DEBUG ((EFI_D_ERROR, "either DimmSlot, PhysicalRankId or ChipSelect is not correct \n"));
    return EFI_INVALID_PARAMETER;
  }
  DimmIndex = Rank >> 2;

  if (Rank >= (MAX_RANK_CH * SUB_CH)) {
    DEBUG ((EFI_D_ERROR, "Rank %d is not valid, it should be smaller than the max Rank number %d in each channel \n", Rank, MAX_RANK_CH * SUB_CH));
    return EFI_INVALID_PARAMETER;
  }

  DimmType = GetDimmType (Skt, Mc, Channel, Rank);
  TA->DimmType = DimmType;

  if (DimmType == dimmTypeUnknown) {
    DEBUG ((EFI_D_ERROR, "wrong DIMM type \n"));
    return EFI_INVALID_PARAMETER;
  }

  if ((TA->DimmType == ddr4dimmType) || (TA->DimmType == HbmdimmType)) {
    if ((Column != (UINT32) -1) ||
        (Row != (UINT32) -1) ||
        (Bank != (UINT8) -1) ||
        (TA->RankAddress == (UINT64) -1)) {
      if ((HighBitSet64 (Bank) > 5) || ((HighBitSet64 (SubRank) > 2) && (SubRank != 0xff))) {
        DEBUG ((EFI_D_ERROR, "\n wrong input bank:%x or SubRank:%x \n", Bank, SubRank));
        return EFI_INVALID_PARAMETER;
      }
    }
  }

  if (DimmType == ddrtdimmType) {
    for (RirIndex = 0; RirIndex < MAX_RIR_DDRT; RirIndex++) {
      if (mSadTadScope[Skt].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirValid == FALSE) {
        continue;
      }
      RirLimit = mSadTadScope[Skt].McInfo[Mc].ChInfo[Channel].DdrtRirInfo[RirIndex].RirLimit;
      if (RirLimit > MaxRirLimit) {
      MaxRirLimit = RirLimit;
      }
    }
    if (Dpa >= MaxRirLimit) {
      DEBUG ((EFI_D_ERROR, "Dpa value: 0x%lx is wrong, Dpa should be smaller than the max RirLimit: 0x%lx\n",Dpa,MaxRirLimit));
      return EFI_INVALID_PARAMETER;
    }
    return EFI_SUCCESS;
  } else {
    for (RirIndex = 0; RirIndex < MAX_RIR; RirIndex++) {
      if (mSadTadScope[Skt].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirValid == FALSE) {
        continue;
      }
      RirLimit = mSadTadScope[Skt].McInfo[Mc].ChInfo[Channel].RirInfo[RirIndex].RirLimit;
      if (RirLimit > MaxRirLimit) {
        MaxRirLimit = RirLimit;
      }
    }
    if ((Column == (UINT32) -1) &&
        (Row == (UINT32) -1) &&
        (Bank == (UINT8) -1) &&
        (TA->RankAddress != (UINT64) -1)) {
      if (TA->RankAddress >= MaxRirLimit) {
        DEBUG ((EFI_D_ERROR, "RankAddress: 0x%lx is wrong, RankAddress should be smaller than the max RirLimit: 0x%lx\n",TA->RankAddress,MaxRirLimit));
        return EFI_INVALID_PARAMETER;
      }
      return EFI_SUCCESS;
    }
  }

  ColBitsNum = mSadTadScope[Skt].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].ColWidth;
  RowBitsNum = mSadTadScope[Skt].McInfo[Mc].ChInfo[Channel].DimmInfo[DimmIndex].RowWidth;
  if (Column >= (UINT32)(1 << ColBitsNum)) {
    DEBUG ((EFI_D_ERROR, "Column is 0x%lx, it's not less than 0x%lx, wrong Column value.\n", Column, 1 << ColBitsNum));
    return EFI_INVALID_PARAMETER;
  }

  if (Row >= (UINT32)(1 << RowBitsNum)) {
    DEBUG ((EFI_D_ERROR, "Row is 0x%lx, it's not less than 0x%lx, wrong Row value.\n", Row, 1 << RowBitsNum));
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/**
Translate Dimm Address to System Address

@param  [in/out] TA   -- pointer to the structure containing DIMM Address

@retval EFI_SUCCESS / Error code
**/
EFI_STATUS
EFIAPI
TranslateDimmAddress(
  IN OUT  PTRANSLATED_ADDRESS TA
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8     Skt;
  UINT8     Mc;
  UINT8     Channel;
  UINT8     Rank = 0;
  UINT8     DimmIndex;
  UINT64    Ra = 0;
  UINT8     SubRank;
  UINT8     Bank;
  UINT32    Row;
  UINT32    Column;
  UINT8     DimmType;
  UINT64    Ca;
  UINT64    Dpa;
  BOOLEAN   Ra2SaFlag = FALSE;

  DEBUG ((EFI_D_ERROR, "\nThe SocketId is 0x%lx", TA->SocketId));
  DEBUG ((EFI_D_ERROR, "\nThe MemoryControllerId is 0x%lx", TA->MemoryControllerId));
  DEBUG ((EFI_D_ERROR, "\nThe ChannelId is 0x%lx", TA->ChannelId));
  DEBUG ((EFI_D_ERROR, "\nThe DimmSlot is 0x%lx", TA->DimmSlot));
  DEBUG ((EFI_D_ERROR, "\nThe PhysicalRankId is 0x%lx", TA->PhysicalRankId));
  DEBUG ((EFI_D_ERROR, "\nrank 0x%lx", TA->ChipSelect));
  DEBUG ((EFI_D_ERROR, "\nThe Row is 0x%lx", TA->Row));
  DEBUG ((EFI_D_ERROR, "\nThe Col is 0x%lx", TA->Col));
  DEBUG ((EFI_D_ERROR, "\nThe Bank is 0x%lx", TA->Bank));
  DEBUG ((EFI_D_ERROR, "\nThe BankGroup is 0x%lx", TA->BankGroup));
  DEBUG ((EFI_D_ERROR, "\nThe DPA is 0x%lx \n", TA->DPA));
  DEBUG ((EFI_D_ERROR, "\nThe Rank address is 0x%lx \n", TA->RankAddress));

  TA->SadId = (UINT8)-1;
  TA->TadId = (UINT8)-1;
  TA->TargetId = (UINT8)-1;
  TA->LogChannelId = (UINT8)-1;
  TA->SystemAddress = (UINT64)-1;
  TA->NmSystemAddress = (UINT64)-1;
  TA->ChannelAddress = (UINT64)-1;
  TA->NmChannelAddress = (UINT64)-1;
  TA->NmRankAddress = (UINT64)-1;

  Skt = TA->SocketId;
  Mc = TA->MemoryControllerId;
  Channel = TA->ChannelId;
  SubRank = TA->ChipId;
  Bank = TA->Bank | (TA->BankGroup << 2);
  Row = TA->Row;
  Column = TA->Col;
  Dpa = TA->DPA;

  TA->Flag = 0;

  Status = CheckDimmAddressParams (TA);
  if (Status == EFI_INVALID_PARAMETER) {
    return Status;
  }
  //
  //get rank from either ChipSelect or DIMM + rankOnDimm
  //
  if (TA->ChipSelect != 0xFF) {
    Rank = TA->ChipSelect;
  } else if ((TA->DimmSlot != 0xFF) && (TA->PhysicalRankId != 0xFF)) {
    Rank = (TA->DimmSlot << 2) | TA->PhysicalRankId;
  }
  DimmIndex = Rank >> 2;

  DimmType = GetDimmType (Skt, Mc, Channel, Rank);
  TA->DimmType = DimmType;

  if (DimmType != dimmTypeUnknown) {
    TA->Flag |= DIMMTYPE_VALID;
  }

  //
  //do RA -> SA translation when col/row/bank == -1.
  //
  if ((TA->DimmType == ddr4dimmType) || (TA->DimmType == HbmdimmType)) {
    if ((Column == (UINT32) -1) &&
        (Row == (UINT32) -1) &&
        (Bank == (UINT8) -1) &&
        (TA->RankAddress != (UINT64) -1)) {
      Ra2SaFlag = TRUE;
      Ra = TA->RankAddress;
    }
  }

  if (((TA->DimmType == ddr4dimmType) || (TA->DimmType == HbmdimmType)) && (Ra2SaFlag == FALSE)) {
    if (ReverseAmapDecode (&Ra, Skt, Mc, Channel, DimmIndex, SubRank, Bank, Row, Column)) {
      TA->RankAddress = Ra;
      TA->Flag |= RANK_ADDRESS_VALID;
      DEBUG ((EFI_D_ERROR, "The RankAddress is 0x%lx \n", TA->RankAddress));
    } else {
      DEBUG ((EFI_D_ERROR, "reverse AMAP fails \n"));
      return EFI_INVALID_PARAMETER;
    }
  }

  if (ReverseRirDecode (&Ca, DimmType, Skt, Mc, Channel, Rank, Ra, Dpa)) {
    TA->ChannelAddress = Ca;
    TA->Flag |= CHN_ADDR_VALID;
    DEBUG ((EFI_D_ERROR, "\nThe ChannelAddress is 0x%lx, bit 6 is not determined!\n", TA->ChannelAddress));
  } else {
    DEBUG ((EFI_D_ERROR, "\nError occurred in getting the ChannelAddress\n"));
    return EFI_INVALID_PARAMETER;
  }

  TA->SystemAddress = ReverseTadDecode (Skt, Mc, Channel, Rank, Ca);

  if (TA->SystemAddress == (UINT64)-1) {
    DEBUG ((EFI_D_ERROR, "\nError occurred in getting the SystemAddress\n"));
    return EFI_INVALID_PARAMETER;
  }
  DEBUG ((EFI_D_ERROR, "\nThe SystemAddress is 0x%lx\n", TA->SystemAddress));
  return Status;

}

/**

Validate Parameters passed for Reverse Address Translation

@param  [in/out] TA   -- pointer to the structure containing DIMM Address

@retval EFI_SUCCESS / Error code


**/
EFI_STATUS
EFIAPI
ValidateAddressTranslationParams (
  IN OUT  PTRANSLATED_ADDRESS TA
)
{

  TA->StatusToDsmInterface = 0;

  if (TA->SocketId == 0xFF) {
    TA->StatusToDsmInterface = 0x00010004;
  } else {
    if (TA->SocketId >= MAX_SOCKET) {
      TA->StatusToDsmInterface = 0x00020004;
    }
  }

  if (TA->MemoryControllerId == 0xFF) {
    TA->StatusToDsmInterface = 0x00010005;
  } else {
    if (TA->MemoryControllerId >= MAX_SYS_IMC) {
      TA->StatusToDsmInterface = 0x00020005;
    }
  }

  if (TA->ChannelId == 0xFF) {
    TA->StatusToDsmInterface = 0x0001000A;
  } else {
    if (TA->ChannelId >= MAX_MC_CH) {
      TA->StatusToDsmInterface = 0x0002000A;
    }
  }

 // if (TA->DimmSlot == 0xFF) {
 //   TA->StatusToDsmInterface = 0x00010010;
 // } else {
 //   if (TA->DimmSlot >= MAX_DIMM) {
 //     TA->StatusToDsmInterface = 0x00020010;
 //   }
  //}


  if (((TA->DimmSlot == 0xFF) || (TA->PhysicalRankId == 0xFF)) && (TA->ChipSelect == 0xFF)) {
    TA->StatusToDsmInterface = 0x00010011;
  }
  //else {
  //  if (TA->DimmSlot >= MAX_DIMM) {
  //    TA->StatusToDsmInterface = 0x00020011;
  //  }
  // }

  if (TA->StatusToDsmInterface != 0) {
    DEBUG ((EFI_D_ERROR, "StatusToDsmInterface:%x \n", TA->StatusToDsmInterface));
    return EFI_INVALID_PARAMETER;
  }
  return EFI_SUCCESS;

}

/**
  Translate a DIMM address to system address.

  @param[in/out]  TranslatedAddress   Pointer to the DIMM Address structure

  @retval EFI_SUCCESS                 Translation was successful
  @retval EFI_INVALID_PARAMETER       DIMM address was null or invalid
**/
EFI_STATUS
EFIAPI
DimmAddressToSystemAddress (
  IN OUT  PTRANSLATED_ADDRESS TranslatedAddress
  )
{

  EFI_STATUS Status;

  Status = ValidateAddressTranslationParams (TranslatedAddress);

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "\nInput Parameters for Reverse Address Translation are not valid!!\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = TranslateDimmAddress (TranslatedAddress);

  if (Status == EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, "\nReverse Address Translation is Successful!!\n"));
  }
  else {
    DEBUG ((EFI_D_ERROR, "\nReverse Address Translation is not Successful!!\n"));
  }

  return Status;

}

VOID
EFIAPI
InitMemTopology (
  IN     UINT8     Socket
  )
{

  UINT8                       Mc;
  UINT8                       Chn;
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();



  for (Mc = 0; Mc < MAX_SYS_IMC; Mc ++) {
    if (Mc < MAX_IMC) {
      mSadTadScope[Socket].McInfo[Mc].McEnabled = SystemMemoryMap->Socket[Socket].imcEnabled[Mc];

      for (Chn = 0; Chn < MAX_CH_PER_MC; Chn ++) {
        mSadTadScope[Socket].McInfo[Mc].ChInfo[Chn].ChEnabled = SystemMemoryMap->Socket[Socket].ChannelInfo[NODECHA_TO_SKTCHA (Mc, Chn)].Enabled;
      }
    } else {
      if (CpuCsrAccessVarPtr->HbmSku) {
        //
        // HBM IO is enabled
        //
        if (SystemMemoryMap->Socket[Socket].HbmIoEn[(Mc - MAX_IMC) / GetMaxImcPerIoHbm()] == FALSE) {
          continue;
        }
        mSadTadScope[Socket].McInfo[Mc].McEnabled = TRUE;
        for (Chn = 0; Chn < MAX_CH_PER_MC; Chn ++) {
          mSadTadScope[Socket].McInfo[Mc].ChInfo[Chn].ChEnabled = TRUE;
        }
      }
    }
  }

}


/**

  The constructor function initialize Address Decode Libary.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitAddressDecodeLib (
  VOID
  )
{

  EFI_HOB_GUID_TYPE           *GuidHob;

  KTI_HOST_OUT               *KtiHostOutDataPtr;
  UINT8                       Index;
  BOOLEAN                     NewAllocation;

  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;


  GuidHob = GetFirstGuidHob (&gEfiKtiHostOutDataHobGuid);
  ASSERT (GuidHob != NULL);
  KtiHostOutDataPtr = (KTI_HOST_OUT *)(GET_GUID_HOB_DATA (GuidHob));

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
  mSocketPresentBitMap = CpuCsrAccessVarPtr->socketPresentBitMap;
  DEBUG ((EFI_D_INFO, "\nmSocketPresentBitMap = 0x%lx", mSocketPresentBitMap));

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    DEBUG ((EFI_D_INFO, "  [Error] Get memory map data error!"));
  }



  mSadTadScope = GetRasGlobalData (L"AddressDecodeSocketInfo", sizeof (AD_SOCKET_INFO) * MAX_SOCKET, &NewAllocation);
  if (mSadTadScope == NULL) {
    return EFI_SUCCESS;
  }

  if (NewAllocation) {
    for (Index = 0; Index < MAX_SOCKET; Index ++) {

      if(IsSocketExist (Index) == FALSE) {
        continue;
      }

      InitMemTopology (Index);

      mSadTadScope[Index].TotalCha = KtiHostOutDataPtr->CpuInfo[Index].TotCha;
      mSadTadScope[Index].NumCluster =KtiHostOutDataPtr->OutNumOfCluster[Index];
      mSadTadScope[Index].NumUmaCluster =KtiHostOutDataPtr->OutUmaClustering;
      if (CpuCsrAccessVarPtr->HbmSku) {
        //
        // Cluster is 4 in HBM sku
        //
        mSadTadScope[Index].NumCluster = 4; // SNC4 or Quadrant
        if (KtiHostOutDataPtr->OutSncEn <= 1) {
          mSadTadScope[Index].NumUmaCluster = 4; // Quadrant
        }
      }
      DEBUG ((EFI_D_INFO, "  [SocketInfo] Socket: %x,  Total Cha: %x, Total Cluster:%x  UMA Cluster:%x \n",
                           Index, mSadTadScope[Index].TotalCha, mSadTadScope[Index].NumCluster, mSadTadScope[Index].NumUmaCluster));

      InitChaDramRule(Index, &mSadTadScope[Index].ClusterInfo[0]);


      if(Index == 0) {
        InitRemoteDramRule(&mSadTadScope[0].RemoteDramRule[0]);
        InitBlockMode(Index);
      }
      InitM2mSadTad(Index);
      InitChTad(Index);
    }
  }

  //
  //put it after CHA init.
  //
  mDramScope = GetRasGlobalData (L"AddressDecodeDramScope", sizeof (DRAM_SCOPE) * DRAM_SCOPE_NUMBER, &NewAllocation);
  ASSERT (mDramScope != NULL);
  if (NewAllocation) {
    DEBUG ((EFI_D_INFO, "[AddressTranslation] Init start. \n"));
    InitDramScope(mDramScope);
  }


  return EFI_SUCCESS;
}


/**
 This routine is to do RAT(FAT(X)).

 @param[in]    SA -- system address


 @retval       TRUE --  RAT(FAT(X))  ==  X; FALSE -- RAT(FAT(X))  1=  X

**/
BOOLEAN
EFIAPI
TestFatRat (
  IN   UINT64    Sa
  )
{
  TRANSLATED_ADDRESS   Ta1;
  TRANSLATED_ADDRESS   Ta2;
  ZeroMem (&Ta1, sizeof (TRANSLATED_ADDRESS));
  ZeroMem (&Ta2, sizeof (TRANSLATED_ADDRESS));

  if (SystemAddressToDimmAddress (Sa, &Ta1) != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, " -------  FAT(X) fail  ------ \n"));
    return FALSE;
  }

   Ta2.SocketId = Ta1.SocketId;
   Ta2.MemoryControllerId = Ta1.MemoryControllerId;
   Ta2.ChannelId = Ta1.ChannelId ;
   Ta2.ChipSelect = Ta1.ChipSelect;
   Ta2.DimmSlot = Ta1.DimmSlot;
   Ta2.PhysicalRankId = Ta1.PhysicalRankId;
   Ta2.ChipId = Ta1.ChipId;
   Ta2.Bank = Ta1.Bank;
   Ta2.BankGroup = Ta1.BankGroup;
   Ta2.Row = Ta1.Row;
   Ta2.Col = Ta1.Col;
   Ta2.DPA = Ta1.DPA ;

   if (DimmAddressToSystemAddress(&Ta2) != EFI_SUCCESS) {
    DEBUG ((EFI_D_ERROR, " -------  RAT(X) fail  ------ \n"));
    return FALSE;
   }

   if (Sa == Ta2.SystemAddress) {
     DEBUG ((EFI_D_ERROR, " -------  RAT(FAT(X)) pass  ------ \n"));
     return TRUE;
   } else {
     DEBUG ((EFI_D_ERROR, " ------   RAT(FAT(X)) fail  ------ \n"));
     return FALSE;
   }
}


/**
 This routine is to do RAT(FAT(X)) for NM as well. In case 2LMM mode, use NM as input.

 @param[in]    SA -- system address


 @retval       TRUE --  RAT(FAT(X))  ==  X; FALSE -- RAT(FAT(X))  1=  X

**/
BOOLEAN
EFIAPI
TestNmFatRat (
  IN   UINT64    Sa
  )
{
  TRANSLATED_ADDRESS   Ta1;
  TRANSLATED_ADDRESS   Ta2;
  ZeroMem (&Ta1, sizeof (TRANSLATED_ADDRESS));
  ZeroMem (&Ta2, sizeof (TRANSLATED_ADDRESS));

  SystemAddressToDimmAddress (Sa, &Ta1);

  if (Ta1.Flag & NM_MC_VALID) {

     Ta2.SocketId = Ta1.SocketId;
     Ta2.MemoryControllerId = Ta1.NmMemoryControllerId;
     Ta2.ChannelId = Ta1.NmChannelId ;
     Ta2.ChipSelect = Ta1.NmChipSelect;
     Ta2.DimmSlot = Ta1.NmDimmSlot;
     Ta2.PhysicalRankId = Ta1.NmPhysicalRankId;
     Ta2.ChipId = Ta1.NmChipId;
     Ta2.Bank = Ta1.NmBank;
     Ta2.BankGroup = Ta1.NmBankGroup;
     Ta2.Row = Ta1.NmRow;
     Ta2.Col = Ta1.NmCol;
     Ta2.DPA = Ta1.DPA ;
  } else {
     Ta2.SocketId = Ta1.SocketId;
     Ta2.MemoryControllerId = Ta1.MemoryControllerId;
     Ta2.ChannelId = Ta1.ChannelId ;
     Ta2.ChipSelect = Ta1.ChipSelect;
     Ta2.DimmSlot = Ta1.DimmSlot;
     Ta2.PhysicalRankId = Ta1.PhysicalRankId;
     Ta2.ChipId = Ta1.ChipId;
     Ta2.Bank = Ta1.Bank;
     Ta2.BankGroup = Ta1.BankGroup;
     Ta2.Row = Ta1.Row;
     Ta2.Col = Ta1.Col;
     Ta2.DPA = Ta1.DPA ;
  }

   DimmAddressToSystemAddress(&Ta2);

   if (Sa == Ta2.SystemAddress) {
     DEBUG ((EFI_D_ERROR, " ------- NM RAT(FAT(X)) pass  ------ \n"));
     return TRUE;
   } else {
     DEBUG ((EFI_D_ERROR, " ------  NM RAT(FAT(X)) fail  ------ \n"));
     return FALSE;
   }
}
