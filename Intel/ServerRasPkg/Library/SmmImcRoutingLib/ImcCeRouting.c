/** @file
  Implementation of memory correctable error basic routine library. In case the routine is called by
  ADDDC/SDDC/Rank Sparing, put the routine in this file.

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
#include <Library/CpuCsrAccessLib.h>
#include <Library/RasDebugLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <SiliconSetting.h>
#include <Library/MemMcIpRasLib.h>
#include "ImcRoutingPrivate.h"
#include <Library/CrystalRidgeLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcuMailBoxRasLib.h>
#include <Library/RasSiliconLib.h>
#include <Library/CpuAndRevisionLib.h>

#define CORRERRTHRSHLD_MCDDC_DP_LIST  {                         \
  CORRERRTHRSHLD_0_MCDDC_DP_REG, CORRERRTHRSHLD_1_MCDDC_DP_REG, \
  CORRERRTHRSHLD_2_MCDDC_DP_REG, CORRERRTHRSHLD_3_MCDDC_DP_REG  \
}

#define CORRERRCNT_MCDDC_DP_LIST    {CORRERRCNT_0_MCDDC_DP_REG, CORRERRCNT_1_MCDDC_DP_REG, \
                                     CORRERRCNT_2_MCDDC_DP_REG, CORRERRCNT_3_MCDDC_DP_REG}

IMC_RAS_STRUCTURE  *mImcRasData;
BOOLEAN            mIs14nmCpu;


/**
    Clears error counters and over flow status for specified
    rank in CPU

  @param Socket    - Socket ID
  @param SktCh     - Channel ID on socket
  @param Rank      - Rank ID

  @retval None

**/
VOID
ClearRankErrors (
  IN UINT8 Socket,
  IN UINT8 SktCh,
  IN UINT8 Rank
  )
{
  CORRERRCNT_0_MCDDC_DP_STRUCT    CorrErrCntReg;
  CORRERRORSTATUS_MCDDC_DP_STRUCT CorrErrStsReg;
  UINT32                          CorrErrCntRegBaseAddr[] = CORRERRCNT_MCDDC_DP_LIST;

  if((Rank / 2) >= (sizeof(CorrErrCntRegBaseAddr) / sizeof(CorrErrCntRegBaseAddr[0]))) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "\n Unsupported Rank access\n"));
  } else {
    CorrErrCntReg.Data = ReadCpuCsr (Socket, SktCh, CorrErrCntRegBaseAddr[(Rank / 2)]);
    CorrErrStsReg.Data = ReadCpuCsr (Socket, SktCh, CORRERRORSTATUS_MCDDC_DP_REG);
    if (CorrErrStsReg.Bits.err_overflow_stat & (1 << Rank)) {
      if ((Rank % 2) == 0) {
        CorrErrCntReg.Bits.overflow_0     = 1;
        CorrErrCntReg.Bits.overflow_1     = 0;
        CorrErrCntReg.Bits.cor_err_cnt_0  = 0;
      }

      if ((Rank % 2) == 1) {
        CorrErrCntReg.Bits.overflow_1     = 1;
        CorrErrCntReg.Bits.overflow_0     = 0;
        CorrErrCntReg.Bits.cor_err_cnt_1  = 0;
      }
      WriteCpuCsr (Socket, SktCh, CorrErrCntRegBaseAddr[(Rank / 2)], CorrErrCntReg.Data);

      CorrErrStsReg.Bits.err_overflow_stat = 1 << Rank;
      WriteCpuCsr (Socket, SktCh, CORRERRORSTATUS_MCDDC_DP_REG, CorrErrStsReg.Data);

      CorrErrCntReg.Data = ReadCpuCsr (Socket, SktCh, CorrErrCntRegBaseAddr[(Rank / 2)]);
      RAS_DEBUG((LEVEL_FUNC_FLOW, "S:0x%x C:0x%x R:0x%x error status after clear: 0x%x\n", Socket, SktCh, Rank, CorrErrCntReg.Data));
      CorrErrStsReg.Data = ReadCpuCsr (Socket, SktCh, CORRERRORSTATUS_MCDDC_DP_REG);
      RAS_DEBUG((LEVEL_FUNC_FLOW, "S:0x%x C:0x%x R:0x%x overflow status after clear: 0x%x\n",Socket, SktCh, Rank, CorrErrStsReg.Data));
    }
  }
  //
  //to check: if dimm_alert exist
  //
}

/**
    Checks if the specified rank has error overflow
    If the CPU error counter overflows clear overflow
    status in CPU; counters are not cleared

  @param NodeId    - Memory controller ID
  @param Ch        - Channel ID
  @param Rank      - Rank ID

  @retval TRUE      - if the specified rank has error overflow
  @retval FALSE     - specified rank has no error overflow

**/
BOOLEAN
EFIAPI
IsErrorExcdForThisRank (
  IN UINT8 Node,
  IN UINT8 NodeCh,
  IN UINT8 Rank
  )
{
  BOOLEAN                         IsErrorExcd    = FALSE;
  CORRERRORSTATUS_MCDDC_DP_STRUCT CorrErrStsReg;
  UINT8                           Socket = NODE_TO_SKT(Node);
  UINT8                           SktCh = NODECHA_TO_SKTCHA(Node,NodeCh);

  CorrErrStsReg.Data = ReadCpuCsr (Socket, SktCh, CORRERRORSTATUS_MCDDC_DP_REG);
  if (CorrErrStsReg.Bits.err_overflow_stat & (1 << Rank)) {
    IsErrorExcd = TRUE;
  }

  return IsErrorExcd;
}

/**
  This routine will check if the Rank belongs to a Dcpmm dimm

  @param NodeId    - Memory controller ID
  @param Ch        - Channel ID
  @param Rank      - Rank ID

  @retval TRUE      - if the specified rank has error overflow
  @retval FALSE     - specified rank has no error overflow

**/
BOOLEAN
EFIAPI
IsDcpmmRank (
  IN UINT8 Socket,
  IN UINT8 ChOnSkt,
  IN UINT8 Rank
  )
{
  UINT8 Dimm;
  Dimm = Rank / (MAX_RANK_DIMM * SUB_CH);
  if (IsDcpmm (Socket,ChOnSkt, Dimm)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "IsPMemRank - Found PMem Dimm:0x%x Rank 0x%x \n", Dimm, Rank));
    return TRUE;
  }
  return FALSE;
}

BOOLEAN
GetNodeProgress (
  IN  UINT8    NodeId
  )
{
  IMC_RAS_STRUCTURE   *ImcRas;
  ImcRas = GetImcRasData ();

  RAS_DEBUG((LEVEL_REG, " current got pending bit map:0x%x \n", ImcRas->EventProgressBitmap));
  if (ImcRas->EventProgressBitmap & (1 << NodeId)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

VOID
SetNodeProgress (
  IN  UINT8    NodeId,
  IN  BOOLEAN  ProgressFlag
  )
{
  IMC_RAS_STRUCTURE   *ImcRas;
  ImcRas = GetImcRasData ();

  if (ProgressFlag) {
    ImcRas->EventProgressBitmap |= 1 << NodeId;
  } else {
    ImcRas->EventProgressBitmap &= ~(1 << NodeId);
  }
  RAS_DEBUG((LEVEL_REG, "  node bit map after setting:0x%x \n", ImcRas->EventProgressBitmap));
}


/**
  Disable Rank SMI interrupt
**/
VOID
EFIAPI
DisableRankError (
  IN  UINT8     Node,
  IN  UINT8     ChOnMc,
  IN  UINT8     Rank
  )
{
  DIS_CORR_ERR_LOG_MCDDC_DP_STRUCT     CorrErrLog;
  UINT8                                Skt;
  UINT8                                ChOnSkt;

  Skt = NODE_TO_SKT(Node);
  ChOnSkt = NODECHA_TO_SKTCHA(Node, ChOnMc);

  if (IsSiliconWorkaroundEnabled ("S1409866138")) {
    CorrErrLog.Data = ReadCRProxyRegister (Skt, ChOnSkt, MAILBOX_BIOS_CMD_CR_PROXY_DIS_CORR_ERR_LOG_MCDDC_DP_INDEX);
  } else {
    CorrErrLog.Data = ReadCpuCsr (Skt, ChOnSkt, DIS_CORR_ERR_LOG_MCDDC_DP_REG);
  }

  CorrErrLog.Data |= (1 << Rank);

  if (IsSiliconWorkaroundEnabled ("S1409866138")) {
    WriteCRProxyRegister (Skt, ChOnSkt, MAILBOX_BIOS_CMD_CR_PROXY_DIS_CORR_ERR_LOG_MCDDC_DP_INDEX, CorrErrLog.Data);
  } else {
    WriteCpuCsr (Skt, ChOnSkt, DIS_CORR_ERR_LOG_MCDDC_DP_REG, CorrErrLog.Data);
  }
}

VOID
EFIAPI
InitializeImcRasTopology (
  IMC_RAS_STRUCTURE * pImcRasData
  )
{
  //
  //This table may change per platform.
  //
  pImcRasData->RasEventHndlrTable[PCLS_INDEX]           = DummyHandler;
  pImcRasData->RasEventHndlrTable[ADDDC_INDEX]          = DummyHandler;
  pImcRasData->RasEventHndlrTable[RANK_SPARE_INDEX]     = DummyHandler;
  pImcRasData->RasEventHndlrTable[SDDC_INDEX]           = DummyHandler;
  pImcRasData->RasEventHndlrTable[ECC_FLOW_SWITCH]      = DummyHandler;
  pImcRasData->RasEventHndlrTable[INDEPENDENT_INDEX]    = DummyHandler;
  pImcRasData->RasEventHndlrTable[END_INDEX]            = END_OF_TABLE;

  pImcRasData->RankIntervalError.RankIntervalErrorrCnt = PcdGet32 (PcdMaxCEoverflowCount);
  pImcRasData->RankIntervalError.ErrorInterval = 24;

  //
  // Check the Si WA
  //
  pImcRasData->WaStatusS2207801634 = IsSiliconWorkaroundEnabled ("S2207801634");
  SetMem (pImcRasData->DdrtPreviousErrorId, sizeof (pImcRasData->DdrtPreviousErrorId), 0xFF);
}

/**
  Get imc RAS data, in case first one invoke this routine, it will initialize topology data.

  @retval  point of memoty topology info

**/
IMC_RAS_STRUCTURE *
EFIAPI
GetImcRasData (
  VOID
  )
{
  return mImcRasData;
}


/**
    This routine is to check if parcicular rank located in bank VLS region.

    @param Skt          - CPU socket id
    @param ChOnSkt      - channel on socket
    @param Rank         - Rank number

    @retval BOOLEAN     - TRUE if rank is in parcicular rank.

**/
BOOLEAN
EFIAPI
IsRankInBankVLS (
  IN  UINT8  Skt,
  IN  UINT8  ChOnSkt,
  IN  UINT8  Rank,
  OUT UINT8 *Plus1PeerRank
  )
{
  BOOLEAN Flag = FALSE;
  UINT8   RegNum;
  UINT8   PrimaryRank;
  UINT8   BuddyRank;
  UINT8   AdditionPeerRank;
  UINT8   RegionEnable;

  for (RegNum=0; RegNum < MAX_ADDDC_REGION; RegNum++) {

    GetAdddcRegion (Skt, ChOnSkt, RegNum, &RegionEnable, &PrimaryRank, NULL, &BuddyRank, NULL, NULL, NULL, NULL );

    if (RegionEnable == 1 ) {
      if (( Rank == PrimaryRank ) || ( Rank == BuddyRank)) {
          if (!Flag) {
            Flag = TRUE;
            *Plus1PeerRank = ( Rank == PrimaryRank ) ?  BuddyRank: PrimaryRank;
             RAS_DEBUG((LEVEL_BASIC_FLOW, "Rank-0x%x appears in bank VLS at region 0x%x PeerRank:0x%x \n",
                Rank,RegNum,*Plus1PeerRank));

          } else {
            AdditionPeerRank = ( Rank == PrimaryRank ) ?  BuddyRank: PrimaryRank;
            if (*Plus1PeerRank != AdditionPeerRank) {
              RAS_DEBUG((LEVEL_BASIC_FLOW, "Error! Rank-0x%x appears in 2 bank VLS with different peer ranks 0x%x - 0x%x\n",
                Rank,*Plus1PeerRank,AdditionPeerRank));
              Flag = FALSE;
            }
          }

      }
    }
  }
  return Flag;
}




EFI_STATUS
EFIAPI
InitializeRASData (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
 BOOLEAN             NewAllocation;
 mImcRasData = GetRasGlobalData (L"IMCRasData", sizeof(IMC_RAS_STRUCTURE), &NewAllocation);

 if (NewAllocation) {
   InitializeImcRasTopology (mImcRasData);
 }

 mIs14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);


 return EFI_SUCCESS;
}




VOID
EFIAPI
SettingScratchPadReg (
  IN   UINT8     Skt,
  IN   UINT8     SktCh,
  IN   UINT32    ScratchPadValue
  )
{
  SCRATCHPAD2_MC_MAIN_STRUCT ChScratchPad2;
  ChScratchPad2.Data = ReadCpuCsr (Skt, SktCh, SCRATCHPAD2_MC_MAIN_REG);
  RAS_DEBUG((LEVEL_REG, "UpdateScratchPadReg - SktCh=0x%x SCRATCHPAD2=0x%0x\n", SktCh, ChScratchPad2.Data));

  ChScratchPad2.Data &= 0xFFFFFF00; // clear LSB
  ChScratchPad2.Data |= ScratchPadValue;

  //
  // if no more spare ranks available, clear enable flag
  //
  if (ScratchPadValue == 0) {
    ChScratchPad2.Data &= ~(UINT32) (1 << (MAX_RANK_CH * SUB_CH));
  }

  WriteCpuCsr (Skt, SktCh, SCRATCHPAD2_MC_MAIN_REG, ChScratchPad2.Data);
  RAS_DEBUG((LEVEL_REG, "UpdateScratchPadReg - SktCh=0x%x SCRATCHPAD2=0x%x\n", SktCh, ChScratchPad2.Data));
}

/**
  Clears the error counter and update error threshold
  that was pre-configured for all rank on the channel

  @param Node           - Memory controller ID
  @param ChOnNode       - Channel number on this node.
  @param ErrThreshold   - Threshold value to be set

  @retval N/A
**/
VOID
SetErrorThresholdBeforeSpareCopyStart (
  UINT8    Node,
  UINT8    ChOnNode,
  UINT16   ErrThreshold
)
{
  UINT8   Rank;
  UINT8   Socket;
  UINT8   ChOnSkt;

  if (mIs14nmCpu == FALSE) {
    return;
  }

  RAS_ASSERT (Node < MC_MAX_NODE);
  RAS_ASSERT (ChOnNode < MAX_MC_CH);

  Socket = NODE_TO_SKT (Node);
  ChOnSkt= NODECHA_TO_SKTCHA (Node, ChOnNode);

  for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank++) {
    SetErrorThresholdPerRank (Socket, ChOnSkt, Rank, ErrThreshold);
    SetErrorCounterPerRank (Socket, ChOnSkt, Rank, 0);
  }
}

/**
  This routine changes corrected error threshold for the ranks in
  a channel according to the rank mode.

  @param Node           - Memory controller ID
  @param ChOnNode       - Channel number on this node.

  @retval N/A
**/
VOID
SetErrorThresholdAfterCopyComplete (
  UINT8    Node,
  UINT8    ChOnNode
)
{

  UINT8               Rank;
  UINT8               Socket;
  UINT8               ChOnSkt;
  UINT16              ErrorThresholdForRankinADDDC;
  IMC_RAS_STRUCTURE   *ImcRas;

  if (mIs14nmCpu == FALSE) {
    return;
  }

  RAS_ASSERT (Node < MC_MAX_NODE);
  RAS_ASSERT (ChOnNode < MAX_MC_CH);

  Socket = NODE_TO_SKT (Node);
  ChOnSkt= NODECHA_TO_SKTCHA (Node, ChOnNode);

  ImcRas = GetImcRasData ();

  ErrorThresholdForRankinADDDC = (ImcRas->SpareErrTh / 2) + (ImcRas->SpareErrTh % 2);

  for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank++) {
    if (IsRankInPlusoneState (Node, ChOnNode, Rank)) {
      //
      //Rank in Pluse One state. Set Threshold to Maximum
      //
      SetErrorThresholdPerRank (Socket, ChOnSkt, Rank, MAX_RANK_ERROR_THRESHOLD);
    } else if (IsRankinAdddc (Node, ChOnNode, Rank)) {
      //
      //Rank in ADDDC region. Set Threshold to half of original threshold
      //
      SetErrorThresholdPerRank (Socket, ChOnSkt, Rank, ErrorThresholdForRankinADDDC);
    } else {
      //
      //Rank neither in +1 nor in ADDDC state. Restore the original threshold
      //
      SetErrorThresholdPerRank (Socket, ChOnSkt, Rank, ImcRas->SpareErrTh);
    }
  }

}
