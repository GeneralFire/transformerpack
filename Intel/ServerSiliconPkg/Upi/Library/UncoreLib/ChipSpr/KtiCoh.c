/** @file
  This file includes KTI coherency related functions which are specific to SPR.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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


#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <KtiMisc.h>
#include <Library/MemoryCoreLib.h>
#include <RcRegs.h>
#include <KtiSetupDefinitions.h>
#include <UncoreCommonIncludes.h>
#include <Library/MemRcLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/ChaLib.h>
#include <Guid/IioIpInterface.h>
#include <Library/M2IalIpLib.h>
#include <Guid/UboxIpInterface.h>
#include <Guid/KtiIpInterface.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SiliconWorkaroundLib.h>

//
// SPR UBOX destination credit arrays
//
static UINT8 Spr_UboxDestinationCreditBl[SPR_TOPOLOGY_MAX][3] = {
// MS2IOSF VNA, M3KTI VN0, UBOX VN0
// NCB/NCS,     (NCB+NCS), NCB/NCS
  { 1,           (0+0),     1      },     // 1S
  { 1,           (1+1),     1      },     // 2S1L
  { 1,           (1+1),     1      },     // 2S2L
  { 1,           (1+1),     1      },     // 2S3L
  { 1,           (1+1),     1      },     // 2S4L
  { 1,           (1+1),     1      },     // 4S3L
  { 1,           (1+1),     1      },     // 4S2L
  { 1,           (1+1),     1      }      // 8S
};

static UINT8 Spr_UboxDestinationCreditBl_Cha2UboxVn0[4] = { 2, 2, 4, 8}; //LCC, MCC, HCC, XCC

#define  CHA2M2IAL_VNA_AD_CREDIT_TOTAL      120

#define  UPI2M2IAl_VN0_NCB_CREDIT_FOR_1D    12
#define  UPI2M2IAl_VN0_NCS_CREDIT_FOR_1D    12

static UINT8 Spr_M2IalDestinationCreditBl[SPR_TOPOLOGY_MAX][5] = {
//UBOX VN0, MS2IOSF VN0, UPI VN0 NCB, UPI VN0 NCS, Reserved
  {1,        1,           0,           0,           1        },  // 1S
  {1,        1,           1,           1,           1        },  // 2S1L
  {1,        1,           1,           1,           1        },  // 2S2L
  {1,        1,           1,           1,           1        },  // 2S3L
  {1,        1,           1,           1,           1        },  // 2S4L
  {1,        1,           1,           1,           1        },  // 4S3L
  {1,        1,           1,           1,           1        },  // 4S2L
  {1,        1,           1,           1,           1        }   // 8S
};

static UINT8 Spr_M2IalDestinationCreditBl_Cha2M2IalVn0[4] = { 5, 5, 10, 20 }; //LCC, MCC, HCC, XCC

static UINT8 Spr_SharedP2pCreditBlVnaM2ial2M2ial[SPR_TOPOLOGY_MAX][4] = {
// LCC,        MCC,         HCC,         XCC (LCC need to be updated)
  {115,        113,         108,         97       },  // 1S
  {113,        107,         106,         94       },  // 2S1L
  {112,        107,         104,         92       },  // 2S2L
  {109,        107,         102,         90       },  // 2S3L
  {0,          0,           100,         88       },  // 2S4L
  {109,        107,         102,         90       },  // 4S3L
  {112,        107,         103,         92       },  // 4S2L
  {109,        107,         102,         90       }   // 8S
};

UINT8 Spr_SharedP2pCreditBlVnaM2ial2M3kti[SPR_TOPOLOGY_MAX][4] = {
// LCC,        MCC,         HCC,         XCC (LCC/HCC need to be updated)
  {0,          0,           0,           0        },  // 1S
  {69,         71,          69,          69       },  // 2S1L
  {69,         71,          69,          69       },  // 2S2L
  {69,         71,          69,          69       },  // 2S3L
  {69,         0,           69,          69       },  // 2S4L
  {69,         71,          69,          69       },  // 4S3L
  {59,         61,          59,          59       },  // 4S2L
  {46,         48,          46,          46       }   // 8S
};

#define  P2P_MESH_AGENTS_INDEX_MS2IOSF0         0
#define  P2P_MESH_AGENTS_INDEX_MS2IOSF1         1
#define  P2P_MESH_AGENTS_INDEX_MS2IOSF8         8
#define  P2P_MESH_AGENTS_INDEX_UPI0             12
//
// Dedicated credit NCB/NCS MS2IOSF->MS2IOSF
//
#define  DEDICATED_CREDIT_MS2IOSF_TO_MS2IOSF    15

//
// Dedicated Credit NCB/NCS/DRS MS2IOSF -> UPI0
//
#define  DEDICATED_CREDIT_MS2IOSF0_TO_UPI0      10
#define  DEDICATED_CREDIT_MS2IOSF1_TO_UPI0      12
#define  DEDICATED_CREDIT_MS2IOSF2_TO_UPI0      13

static UINT8 DedicatedCrditToUpi0Tbl[MAX_IIO_STACK] = {
  10, //Stack0
  12, //Stack1
  0,  //Stack2
  0,  //Stack3
  0,  //Stack4
  0,  //Stack5
  0,  //Stack6
  0,  //Stack7
  13, //Stack8
  0,  //Stack9
  0,  //Stack10
  0,  //Stack11
};

/**
  Program all the credits for destination M2IAL.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
ProgramCreditForDestM2Ial (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  UINT8           NumofActiveKtiLink;
  UINT8           NumOfCha;
  UINT8           Cha;
  UINT8           Port;

  UINT8           BlCha2M2IalVnaCredit;
  UINT8           AdCha2M2IalVnaCredit;
  UINT8           BlUpi2M2IalVn0NcbCredit;
  UINT8           BlUpi2M2IalVn0NcsCredit;
  UINT8           ReservedCredit;
  TOPOLOGY4CREDIT Topology;

  NumofActiveKtiLink = CountActiveKtiLink (KtiInternalGlobal, Soc);
  Topology = GetTopology4Credit (KtiInternalGlobal, Soc);
  NumOfCha = KtiInternalGlobal->TotCha[Soc];

  BlUpi2M2IalVn0NcbCredit  = Spr_M2IalDestinationCreditBl[Topology][2];
  BlUpi2M2IalVn0NcsCredit  = Spr_M2IalDestinationCreditBl[Topology][3];
  //
  // Per reqeust from silcon team, need to use Ncb/Ncs new credit value if it is one die.
  //
  if ((KTIVAR->CpuInfo[Soc].DieCount == ONE_DIE_IN_SOCKET) && (Topology == SPR_2S1L)) {
    BlUpi2M2IalVn0NcbCredit = UPI2M2IAl_VN0_NCB_CREDIT_FOR_1D;
    BlUpi2M2IalVn0NcsCredit = UPI2M2IAl_VN0_NCS_CREDIT_FOR_1D;
  }

  ReservedCredit           = Spr_M2IalDestinationCreditBl[Topology][4];

  BlCha2M2IalVnaCredit = (128 - ReservedCredit) / NumOfCha;

  if (BlCha2M2IalVnaCredit > 15) {
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n\nWARNING: BL Cha2M2IAL Vna credit value is out of range !!!"));
    BlCha2M2IalVnaCredit = 15;
  }

  if ((KTIVAR->CpuInfo[Soc].DieCount == ONE_DIE_IN_SOCKET) && (KtiInternalGlobal->ChopType[Soc] == TypeXcc)) {
   BlCha2M2IalVnaCredit = 2;
  }

  AdCha2M2IalVnaCredit = CHA2M2IAL_VNA_AD_CREDIT_TOTAL / NumOfCha;

  if (AdCha2M2IalVnaCredit > 15) {
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n\nWARNING: AD Cha2M2IAL Vna credit value is out of range !!!"));
    AdCha2M2IalVnaCredit = 15;
  }

  //
  //Program CHA to M2IAL credit registers
  //
  for (Cha = 0; Cha < KtiInternalGlobal->TotCha[Soc]; Cha ++) {
    ChaConfigureChaToM2PCIeCredit (Soc, Cha, BlCha2M2IalVnaCredit);
    ChaConfigureChaToM2IalAdVnaCredit (Soc, Cha, AdCha2M2IalVnaCredit, KTIVAR->HbmSku);
  }//CHA loop

  //
  // Program UPI->M2IAL VN0 credit registers
  //
  for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
    if (KTIVAR->CpuInfo[Soc].LepInfo[Port].Valid == TRUE) {
      M3KtiConfigureM3KtiToM2IalCredit (Soc, Port, BlUpi2M2IalVn0NcbCredit, BlUpi2M2IalVn0NcsCredit);
    }
  }

  return KTI_SUCCESS;
 }
/**
Program all the credits for destination UBOX.

@param KtiInternalGlobal - pointer to the KTI RC internal global structure
@param Soc               - Socket ID

@retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
ProgramCreditForDestUbox (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Soc
  )
{
  UINT8           NumOfCha;
  UINT8           Cha;
  UINT8           Stack;
  UINT8           BlCha2UboxVnaCredit;
  UINT8           BlCha2UboxVnaCreditTmp;
  UINT8           BlCha2UboxVnaCreditLeftOver;
  UINT8           TotalCha2UboxVnaCredit;

  NumOfCha = KtiInternalGlobal->TotCha[Soc];

  if ((KTIVAR->CpuInfo[Soc].DieCount == ONE_DIE_IN_SOCKET) && (KtiInternalGlobal->ChopType[Soc] == TypeXcc)) {
    BlCha2UboxVnaCredit = 1;
    for (Cha = 0; Cha < NumOfCha; Cha++) {
      ChaConfigureChaToUboxCredit (Soc, Cha, BlCha2UboxVnaCredit);
    }
  } else {
    TotalCha2UboxVnaCredit = TOTAL_CHA_TO_UBOX_BL_VNA_SPR;
    if (IsSiliconWorkaroundEnabled ("S14011088832")) {
      TotalCha2UboxVnaCredit--;
    }

    BlCha2UboxVnaCreditTmp = TotalCha2UboxVnaCredit / NumOfCha;
    BlCha2UboxVnaCreditLeftOver = TotalCha2UboxVnaCredit % NumOfCha;

    for (Cha = 0; Cha < NumOfCha; Cha++) {
      BlCha2UboxVnaCredit = BlCha2UboxVnaCreditTmp;
      if (BlCha2UboxVnaCreditLeftOver != 0) {
        BlCha2UboxVnaCredit++;
        BlCha2UboxVnaCreditLeftOver--;
      }

      if (BlCha2UboxVnaCredit > 15) {
        BlCha2UboxVnaCredit = 15;
      }
      ChaConfigureChaToUboxCredit (Soc, Cha, BlCha2UboxVnaCredit);
    }
  }
  //
  // Program MS2IOSF->UBOX VNA credit registers
  //
  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if ((KtiInternalGlobal->M2PcieActiveBitmap[Soc] & (1 << Stack)) != 0) {
      M2IalSetIio2UboxInitCrd (Soc, Stack);
    }
  }
  //
  // Program Ubox -> Ubox Init redit (hardcode 1).
  //
  UboxSetUbox2M2UInitCrd (Soc, 0, 1);

  return KTI_SUCCESS;
}
/**
  Program mesh credits.

  @param SocketData        - pointer to socket data structure
  @param KtiInternalGlobal - pointer to KTIRC Internal Global data

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramMeshCredits (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  MCAErrWa (TRUE);

  if ((KTIVAR->SysConfig == SYS_CONFIG_1S) && (KtiInternalGlobal->ProgramCreditFpga == 0)) {
    ProgramMeshCredits1S (KtiInternalGlobal);
  } else {
    ProgramMeshCreditsCommon (SocketData, KtiInternalGlobal);
  }
  SetNewResetRequired (POST_RESET_WARM);

  return KTI_SUCCESS;
}

/**
  Count the number of active links

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval number of active links

**/
UINT8
CountActiveKtiLink (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Soc
  )
{
  UINT8 NumofActiveKtiLink, Port;

  if (KTIVAR->OutKtiCpuSktHotPlugEn == FALSE) {
    NumofActiveKtiLink = 0;
    for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
      if (KTIVAR->CpuInfo[Soc].LepInfo[Port].Valid == TRUE){
          NumofActiveKtiLink ++;
       }
    }
  } else {     //if hotplug enabled, need to reserved all credit for even unused credit
    NumofActiveKtiLink = GetKtiPortCnt ();
  }

  return  NumofActiveKtiLink;
}

/**
  Mask R2EGRPRQERRMSK0_M2UPCIE_MAIN_REG when program M2PCIE credits
  Unmaks after warm reset

  @param Mask              - Mask or Unmask the R2EGRPRQERRMSK0_M2UPCIE_MAIN_REG

**/
VOID
MCAErrWa (
  IN BOOLEAN             Mask
  )
{
  UINT8 Src, Idx1, Soc;

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    if (!SocketPresent (Soc)) {
      continue;
    }
    for (Idx1 = MAX_IIO_STACK; Idx1 != 0; Idx1--) {
      Src = Idx1 - 1;
      if (KTI_HOST_OUT_PTR->M2PciePresentBitmap[Soc] & (1 << Src)) {
        M2IalSetR2CrdErrMask (Soc, Src, Mask);
      }
    }
  }
}

/**
  Program M2Mem credits.

  Program mesh credits that require reset to take effect.
  For 1S topologies only.

  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramMeshCredits1S (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{

  UINT8                 Sbsp;
  Sbsp = KtiInternalGlobal->SbspSoc;

  ProgramCreditForDestCha (KtiInternalGlobal, Sbsp);                  // CHA destination
  ProgramCreditForDestM2Ial (KtiInternalGlobal, Sbsp);                // M2IAL destination
  ProgramCreditForDestUbox (KtiInternalGlobal, Sbsp);                 // UBOX destination
  return KTI_SUCCESS;
}

/**
  Program the mesh credits that require reset for it to take effect.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramMeshCreditsCommon (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8    Soc;

  //
  // Allocate credits for all the mesh agents communication.
  //

  //
  // Program the values into each socket.
  //
  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    if ((SocketData->Cpu[Soc].Valid == TRUE) && (SocketData->Cpu[Soc].SocType == SOCKET_TYPE_CPU)) {
      ProgramCreditForDestM2Upi (KtiInternalGlobal, Soc);                // M2UPI destination
      ProgramCreditForDestCha (KtiInternalGlobal, Soc);                  // CHA destination
      ProgramCreditForDestM2Ial (KtiInternalGlobal, Soc);                // M2IAL destination
      ProgramCreditForDestUbox (KtiInternalGlobal, Soc);                 // UBOX destination
    } //if ((SocketData->Cpu[Soc].Valid == TRUE)...
  } //for (Soc = 0; Soc < MAX_SOCKET; Soc++) {

  return KTI_SUCCESS;
}

/**
  Program CHA -> M2M credits

  @param  KtiInternalGlobal - KTIRC Internal Global data
  @param  MemInfo           - Memory Information structure.
  @param  SocId             - socket Id

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramChaToM2mCreditWithoutCluster (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  UINT8                      SocId
  )
{
  UINT8   NumOfCha;
  UINT32  AdCreditsToM2mPerCha =0;
  UINT32  BlCreditsToM2mPerCha =0;
  UINT8   AdCreditsLeftOver = 0;
  UINT8   BlCreditsLeftOver = 0;
  UINT8   AdCreditsReserved, BlCreditsReserved;
  UINT8   MaxIMC;
  UINT8   ChopType;
  BOOLEAN Mem2LMEnabled;

  MaxIMC   = GetMaxImc ();
  NumOfCha = KTIVAR->CpuInfo[SocId].TotCha;
  ChopType = KtiInternalGlobal->ChopType[KtiInternalGlobal->SbspSoc];
  ChaCheck2LMEnabled (SocId, 0, &Mem2LMEnabled);

  if (KTIVAR->HbmSku == TRUE) {
    ProgramChaToHbmM2mCredit(KtiInternalGlobal, MemInfo, SocId);
  }

  if (!Mem2LMEnabled) {
    if ((ChopType == TypeXcc) && (KTIVAR->CpuInfo[SocId].DieCount == ONE_DIE_IN_SOCKET)) {
      //
      // One Die configuration
      //
      AdCreditsReserved = RESERVED_XPTPREFETCH_FLAT_SPR_XCC_1D_AD;
      BlCreditsReserved = RESERVED_TRANSGRESS_FLAT_SPR_XCC_1D_BL;
    } else if (ChopType == TypeMcc) {
      AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_SPR_MCC;
      BlCreditsReserved = RESERVED_TRANSGRESS_BL_FLAT_SPR_MCC;
    } else {
      //
      // XCC 4-way and HCC 2-way
      //
      AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_SPR_MD;
      BlCreditsReserved = RESERVED_TRANSGRESS_BL_FLAT_SPR_SNC2_NOSNC;
    }

    if (KTIVAR->HbmSku == TRUE) {
      AdCreditsReserved = RESERVED_TRANSGRESS_AD_HBM_1LM_All_TO_ALL;
      BlCreditsReserved = RESERVED_TRANSGRESS_BL_HBM_1LM_All_TO_ALL;
    }
  } else {
    //
    // 2LM mode
    //
    if ((ChopType == TypeXcc) && (KTIVAR->CpuInfo[SocId].DieCount == ONE_DIE_IN_SOCKET)) {
      //
      // One Die configuration
      //
      AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MD;
      BlCreditsReserved = RESERVED_TRANSGRESS_BL_2LM_SPR_MD_XCC;
    } else if (ChopType == TypeMcc) {
      AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MCC;
      BlCreditsReserved = RESERVED_TRANSGRESS_BL_2LM_SPR_MCC;
    } else if (ChopType == TypeHcc) {
        //
        // HCC 2 way, Non-SNC mode
        //
        AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MD_HCC;
        BlCreditsReserved = RESERVED_TRANSGRESS_BL_2LM_SPR_MD;
    } else {
        //
        // XCC 4-way
        //
        AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MD;
        BlCreditsReserved = RESERVED_TRANSGRESS_BL_2LM_SPR_MD_XCC;
    }

    if (KTIVAR->HbmSku == TRUE) {
      AdCreditsReserved = RESERVED_TRANSGRESS_AD_HBM_2LM_All_TO_ALL;
      BlCreditsReserved = RESERVED_TRANSGRESS_BL_HBM_2LM_All_TO_ALL;
    }
  }

  //TODO: HSD "1508078964" For HBM
  //1. Total Ad credit can reach 119 and Bl credit can reach 121, but we still follow XCC_4_Way here, Ad 114 and Bl 112.
  //2. It's claimed "XPT Prefetch is not supported for both HBM and DDR", but we can see XPT credit
  //Wait for feedback from silicon team
  AdCreditsToM2mPerCha = (TOTAL_M2M_AD_CREDIT_SPR - AdCreditsReserved) / NumOfCha;
  AdCreditsLeftOver = (TOTAL_M2M_AD_CREDIT_SPR - AdCreditsReserved) % NumOfCha;

  BlCreditsToM2mPerCha = (TOTAL_M2M_BL_CREDIT_SPR - BlCreditsReserved) / (NumOfCha);
  BlCreditsLeftOver = (TOTAL_M2M_BL_CREDIT_SPR - BlCreditsReserved) % (NumOfCha);

  //
  // (only 4 bits for this field , max to 15)
  //
  if ( AdCreditsToM2mPerCha > MAX_CREDIT) {
    AdCreditsToM2mPerCha = MAX_CREDIT;
  }
  if (BlCreditsToM2mPerCha > MAX_CREDIT) {
    BlCreditsToM2mPerCha = MAX_CREDIT;
  }

  ChaConfigureChaToM2MCreditWithoutCluster (
    SocId,
    NumOfCha,
    MaxIMC,
    AdCreditsToM2mPerCha,
    BlCreditsToM2mPerCha,
    AdCreditsLeftOver,
    BlCreditsLeftOver,
    0xFFFF0000,
    0xFFFF0000
    );

  return KTI_SUCCESS;
}

/**

  Check whether AppDirect memory type exist or not for socket id SocId

  @param  SocId             - Socket ID

  @retval TRUE              - AppDirect memory type exist
          FALSE             - AppDirect memory type doesn't exist

**/
BOOLEAN
CheckMemTypeAppDirectExist (
  UINT8                      SocId
  )
{
  SAD_TABLE      *SADEntry;
  UINT8          ClusterIdx;
  UINT8          SadRuleIdx;
  UINT8          SadRuleIdxInCluster;
  MEM_VAR_STRUCT *MemVar;
  BOOLEAN        UnusedSadClusterFound = FALSE;

  MemVar = GetMemVarPointer ();

  for (ClusterIdx = 0; ClusterIdx < MAX_DRAM_CLUSTERS; ClusterIdx++) {
    if (UnusedSadClusterFound) {
      break;
    }

    for (SadRuleIdxInCluster = 0; SadRuleIdxInCluster < MAX_SAD_RULES; SadRuleIdxInCluster++) {
      SadRuleIdx = ClusterIdx * MAX_SAD_RULES + SadRuleIdxInCluster;
      SADEntry = &MemVar->socket[SocId].SAD[SadRuleIdx];

      if (SADEntry->Enable == 0) {
        if (SadRuleIdxInCluster == 0) {
          UnusedSadClusterFound = TRUE;
        }
        break; // end of SAD table in the cluster "ClusterIdx"
      }
      if (IsMemTypeAppDirect (SADEntry->type)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**

  Program ICX CHA->M2M credits when there are clusters

  @param  MemInfo          - Memory Information structure.
  @param  SocId            - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramChaToM2mCreditWithCluster (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO       *MemInfo,
  UINT8           SocId
  )
{
  UINT8   AdCreditsReserved, BlCreditsReserved, AdCreditsLeftOver, BlCreditsLeftOver;
  UINT8   NumOfCha, NumOfChaPerCluster, NumOfCluster;
  UINT32  AdCreditsToM2mPerCha, BlCreditsToM2mPerCha;
  UINT32  NonClusterChaCredit = 0;
  UINT8   MaxIMC;
  BOOLEAN AepDimmPresent, Mem2LMEnabled, ADMemPresent;
  CLUSTERING_MODE DdrClustingMode;
  UINT8  ChopType;
  ChopType = KtiInternalGlobal->ChopType[KtiInternalGlobal->SbspSoc];

  NumOfCha = KTIVAR->CpuInfo[SocId].TotCha;
  NumOfChaPerCluster = MemInfo->SncInfo[SocId].NumOfChaPerCluster;
  NumOfCluster = MemInfo->SncInfo[SocId].NumOfCluster;
  MaxIMC = GetMaxImc ();
  ADMemPresent = FALSE;

  if (NumOfCluster == 0) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nThis function is for NumOfCluster!=0"));
    return KTI_FAILURE;
  }

  if (NumOfChaPerCluster == 0) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nNumOfChaPerCluster can't be 0"));
    return KTI_FAILURE;
  }

  //
  //  SPR XCC BL 2LM mode is Absent in V1.0
  //
  AepDimmPresent = CheckSocketDcpmmPresent (SocId);
  ChaCheck2LMEnabled (SocId, 0, &Mem2LMEnabled);
  ADMemPresent = CheckMemTypeAppDirectExist (SocId);

  if (Mem2LMEnabled) {
    NonClusterChaCredit = 1;
  } else {
    if (AepDimmPresent) {
      NonClusterChaCredit = 1;
    }
  }

  if (KTIVAR->HbmSku == TRUE) {
    ProgramChaToHbmM2mCredit(KtiInternalGlobal, MemInfo, SocId);
    if (Mem2LMEnabled) {
      //2LM, HBM caching DDR and DDR is not All2All, no credit from CHA to DDR's M2M, just return.
      //If we program credit in such case anyway, it will not take effect, it's still default value 1.
      //If DDRT comes in, the story change. Reminder here.
      return KTI_SUCCESS;
    }
  }

  if (!Mem2LMEnabled) {
    //
    // SNC Mode and FLAT Mode DDR / DDR + DDRT
    //
  if (ChopType == TypeMcc) {
      AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_SPR_MCC;
      BlCreditsReserved = RESERVED_TRANSGRESS_BL_FLAT_SPR_MCC;
    } else {
      if (NumOfCluster == 4) {
        //
        // SNC4 Mode, XCC 4-way
        //
        AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_SPR_SNC4_MD_XCC;
        BlCreditsReserved = RESERVED_TRANSGRESS_BL_FLAT_SPR_SNC4_MD;
      } else {
        //
        // SNC2 Mode
        // XCC 4-way and HCC 2-way
        //
        AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_SPR_MD;
        BlCreditsReserved = RESERVED_TRANSGRESS_BL_FLAT_SPR_SNC2_NOSNC;
      }
    }
    if (KTIVAR->HbmSku == TRUE) {
      NonClusterChaCredit = 0;
      DdrClustingMode = GetDdrClusteringMode();
      switch (DdrClustingMode) {
        case QuadClusteringMode:
        case Snc4ClusteringMode:
        case MaxClusteringMode:
          AdCreditsReserved = RESERVED_TRANSGRESS_AD_HBM_1LM_SNC4;
          BlCreditsReserved = RESERVED_TRANSGRESS_BL_HBM_1LM_SNC4;
          break;
        case HemiClusteringMode:
        case Snc2ClusteringMode:
          AdCreditsReserved = RESERVED_TRANSGRESS_AD_HBM_1LM_SNC2;
          BlCreditsReserved = RESERVED_TRANSGRESS_BL_HBM_1LM_SNC2;
          break;
        default:
          break;
      }
    }
  } else {
    //
    //  SNC Mode --- 2LM Mode
    //
    if (ChopType == TypeMcc) {
      AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MCC;
      BlCreditsReserved = RESERVED_TRANSGRESS_BL_2LM_SPR_MCC;
    } else {
      if (ADMemPresent && (ChopType == TypeHcc)) {
        //
        //  HCC 2-way with AD memory
        //
        AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MD_HCC;
        BlCreditsReserved = RESERVED_TRANSGRESS_BL_2LM_SPR_MD;
      } else {
        //
        // XCC 4-way and HCC 2-way without AD memory
        //
        AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_SPR_MD;
        BlCreditsReserved = RESERVED_TRANSGRESS_BL_2LM_SPR_MD_XCC;
      }
    }
  }
  AdCreditsToM2mPerCha = (TOTAL_M2M_AD_CREDIT_SPR - AdCreditsReserved - NonClusterChaCredit * (NumOfCha - NumOfChaPerCluster)) / (NumOfChaPerCluster);
  AdCreditsLeftOver = (TOTAL_M2M_AD_CREDIT_SPR - AdCreditsReserved - NonClusterChaCredit * (NumOfCha - NumOfChaPerCluster)) % (NumOfChaPerCluster);
  BlCreditsToM2mPerCha = (TOTAL_M2M_BL_CREDIT_SPR - BlCreditsReserved - NonClusterChaCredit * (NumOfCha - NumOfChaPerCluster)) / (NumOfChaPerCluster);
  BlCreditsLeftOver = (TOTAL_M2M_BL_CREDIT_SPR - BlCreditsReserved - NonClusterChaCredit * (NumOfCha - NumOfChaPerCluster)) % (NumOfChaPerCluster);

  //
  // (only 4 bits for this field , max to 15)
  //
  if ( AdCreditsToM2mPerCha > MAX_CREDIT) {
    AdCreditsToM2mPerCha = MAX_CREDIT;
  }
  if (BlCreditsToM2mPerCha > MAX_CREDIT) {
    BlCreditsToM2mPerCha = MAX_CREDIT;
  }

  ChaConfigureChaToM2MCreditWithCluster (
    SocId,
    NumOfCha,
    NumOfCluster,
    NumOfChaPerCluster,
    MaxIMC,
    AdCreditsToM2mPerCha,
    BlCreditsToM2mPerCha,
    NonClusterChaCredit,
    AdCreditsLeftOver,
    BlCreditsLeftOver,
    0xFFFF0000,
    0xFFFF0000
    );

  return KTI_SUCCESS;
}
/**
  Program credits for shared P2P - M2IAL to M2IAL & M2IAL to M3KTI P2P Creidts

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID
  @param R2p2pInitShCred0  - final register value 0
  @param R2p2pInitShCred1  - final register value 1
  @param R2p2pInitShCred2  - final register value 2
  @param R2p2pInitShCred3  - final register value 3

  @retval  KTI_SUCCESS --- programmed successfully
           Others      --- programmed failed

**/
KTI_STATUS
ProgramP2pCreditValue (
  IN   KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN   UINT8                    Soc,
  IN   UINT32                   R2p2pInitShCred0,
  IN   UINT32                   R2p2pInitShCred1,
  IN   UINT32                   R2p2pInitShCred2,
  IN   UINT32                   R2p2pInitShCred3
  )
{
  UINT8       IioStack;
  BOOLEAN     P2pProgramed;

  P2pProgramed = FALSE;

  for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
    if (KtiInternalGlobal->M2PcieActiveBitmap[Soc] & (1 << IioStack)) {
        if (P2pProgramed == FALSE) {
           M2IalSetR2InitShrCrd (Soc, IioStack, R2p2pInitShCred0, R2p2pInitShCred1, R2p2pInitShCred2, R2p2pInitShCred3);
           P2pProgramed =  TRUE;
        }
    }
  }
  return KTI_SUCCESS;
}

/**
  Combine the M2IAL to M2IAL and M2IAL to M3KTI P2P credit to the final register value according to the
  register bit field definition.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID
  @param M2ial2M2ialP2pCrd - M2IAL to M2IAL P2P credit
  @param M2ial2M3ktiP2pCrd - M2IAL to M3KTI P2P credit
  @param R2p2pInitShCred0  - pointer to final register value 0
  @param R2p2pInitShCred1  - pointer to final register value 1
  @param R2p2pInitShCred2  - pointer to final register value 2
  @param R2p2pInitShCred3  - pointer to final register value 3

  @retval  KTI_SUCCESS --- Combine successfully
           Others      --- Combine failed

**/
KTI_STATUS
CombineP2pCreditValue (
  IN   KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN   UINT8                    Soc,
  IN   UINT8                    M2ial2M2ialP2pCrd,
  IN   UINT8                    M2ial2M3ktiP2pCrd,
  OUT  UINT32                   *R2p2pInitShCred0,
  OUT  UINT32                   *R2p2pInitShCred1,
  OUT  UINT32                   *R2p2pInitShCred2,
  OUT  UINT32                   *R2p2pInitShCred3
  )
{
  UINT8                         IioStack;
  UINT8                         KtiPort;
  UINT8                         M2IalCredit;
  UINT8                         M3KtiCredit;

  //
  // There are four 32bit R2P2PINITSHCRED register (R2P2PINITSHCRED0,R2P2PINITSHCRED1,R2P2PINITSHCRED2,R2P2PINITSHCRED3)
  // and each register covers 4 mesh agent credit data. So, there are 16 mesh agents in total.
  // The mapping between the bit fields meshagentx_initcr of R2P2PINITSHCREDx and the actual P2P agents on the P2P ring
  // (MS2IOSFs and M2UPIs), are listed as below:
  //
  //------------------------------------------------
  // Assigned Agent       |  R2P2PINITSHCRED* entry
  // MS2IOSF Stack ID 0   |  0
  // MS2IOSF Stack ID 1   |  1
  // MS2IOSF Stack ID 2   |  2
  // MS2IOSF Stack ID 3   |  3
  // MS2IOSF Stack ID 4   |  4
  // MS2IOSF Stack ID 5   |  5
  // UPI Port ID 0        |  12
  // UPI Port ID 1        |  13
  // MS2IOSF Stack ID 6   |  6
  // MS2IOSF Stack ID 7   |  7
  // MS2IOSF Stack ID 8   |  8
  // MS2IOSF Stack ID 9   |  9
  // MS2IOSF Stack ID 10  | 10
  // MS2IOSF Stack ID 11  | 11
  // UPI Port ID 2        | 14
  // UPI Port ID 3        | 15
  //------------------------------------------------

  for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
    if (KtiInternalGlobal->M2PcieActiveBitmap[Soc] & (1 << IioStack)) {
      M2IalCredit = M2ial2M2ialP2pCrd;
    } else {
      M2IalCredit = 0;
    }
    if (IioStack < 4) {
      *R2p2pInitShCred0 |= M2IalCredit << (IioStack * 8);
    } else if (IioStack < 8) {
      *R2p2pInitShCred1 |= M2IalCredit << ((IioStack % 4) * 8);
    } else if (IioStack < MAX_IIO_STACK) {
      *R2p2pInitShCred2 |= M2IalCredit << ((IioStack % 4) * 8);
    }
  }

  for (KtiPort = 0; KtiPort < KtiInternalGlobal->KtiPortCnt; KtiPort++) {
    if (KTIVAR->CpuInfo[Soc].LepInfo[KtiPort].Valid == TRUE) {
      M3KtiCredit = M2ial2M3ktiP2pCrd;
    } else {
      M3KtiCredit = 0;
    }

    *R2p2pInitShCred3 |= M2ial2M3ktiP2pCrd << (KtiPort * 8);
  }

  return KTI_SUCCESS;
}

/**
  Program M2IOSF P2P credits

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
VOID
ProgramM2iosfP2pCredits (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  )
{
  UINT8                                 Soc;
  UINT8                                 IioStack;
  UINT32                                R2p2pInitShCred0;
  UINT32                                R2p2pInitShCred1;
  UINT32                                R2p2pInitShCred2;
  UINT32                                R2p2pInitShCred3;
  TOPOLOGY4CREDIT                       Topology;
  UINT8                                 BlSharedP2PVnaCreditM2ial2M2ial;
  UINT8                                 BlSharedP2PVnaCreditM2ial2M3kti;
  UINT8                                 ChopType;
  BOOLEAN                               NeedSetDedicatedNcbNcsDrsCredit;

  UINT8                                 NcbDedicatedCredit[MAX_IIO_STACK][MAX_P2P_MESH_AGENTS_SPR];
  UINT8                                 NcsDedicatedCredit[MAX_IIO_STACK][MAX_P2P_MESH_AGENTS_SPR];
  UINT8                                 DrsDedicatedCredit[MAX_IIO_STACK][MAX_P2P_MESH_AGENTS_SPR];
  UINT8                                 IioStackIndex;
  UINT8                                 MeshAgentIndex;

  R2p2pInitShCred0 = R2p2pInitShCred1 = R2p2pInitShCred2 = R2p2pInitShCred3 = 0;
  ChopType = KtiInternalGlobal->ChopType[KtiInternalGlobal->SbspSoc];

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    if (!SocketPresent (Soc)) {
      continue;
    }

    Topology = GetTopology4Credit (KtiInternalGlobal, Soc);
    BlSharedP2PVnaCreditM2ial2M2ial = Spr_SharedP2pCreditBlVnaM2ial2M2ial[Topology][ChopType];
    BlSharedP2PVnaCreditM2ial2M3kti = Spr_SharedP2pCreditBlVnaM2ial2M3kti[Topology][ChopType];

    CombineP2pCreditValue (KtiInternalGlobal,
                           Soc,
                           BlSharedP2PVnaCreditM2ial2M2ial,
                           BlSharedP2PVnaCreditM2ial2M3kti,
                           &R2p2pInitShCred0,
                           &R2p2pInitShCred1,
                           &R2p2pInitShCred2,
                           &R2p2pInitShCred3
                           );

    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Soc %d, Shared P2P BL VNA creidts: %x, %x, %x, %x.", Soc, R2p2pInitShCred0,
                      R2p2pInitShCred1, R2p2pInitShCred2, R2p2pInitShCred3));

    //
    // Program credits for shared P2P - M2IAL to M2IAL & M2IAL to M3KTI P2P Creidts
    //
    ProgramP2pCreditValue (KtiInternalGlobal, Soc, R2p2pInitShCred0, R2p2pInitShCred1, R2p2pInitShCred2, R2p2pInitShCred3);

    //
    // Need to set P2P dedicated Ncb/NCS/DRS credit if the system is 1D1S and 2D2S (each socket has 1 Die).
    //
    if ((KTIVAR->CpuInfo[Soc].DieCount == ONE_DIE_IN_SOCKET) && ((Topology == SPR_2S1L) || (Topology == SPR_1S))) {

      NeedSetDedicatedNcbNcsDrsCredit = TRUE;

      ZeroMem ((VOID*) NcbDedicatedCredit, sizeof (NcbDedicatedCredit) / sizeof (NcbDedicatedCredit[0][0]));
      ZeroMem ((VOID*) NcsDedicatedCredit, sizeof (NcsDedicatedCredit) / sizeof (NcsDedicatedCredit[0][0]));
      ZeroMem ((VOID*) DrsDedicatedCredit, sizeof (DrsDedicatedCredit) / sizeof (DrsDedicatedCredit[0][0]));

      for (IioStackIndex = IIO_STACK0; IioStackIndex < MAX_IIO_STACK; IioStackIndex++) {
        if ((IioStackIndex != IIO_STACK0) && (IioStackIndex != IIO_STACK1) && (IioStackIndex != IIO_STACK8)) {
          continue;
        }
        for (MeshAgentIndex = 0; MeshAgentIndex < MAX_P2P_MESH_AGENTS_SPR; MeshAgentIndex++) {
          if ((MeshAgentIndex != P2P_MESH_AGENTS_INDEX_MS2IOSF0) &&
             (MeshAgentIndex != P2P_MESH_AGENTS_INDEX_MS2IOSF1) &&
             (MeshAgentIndex != P2P_MESH_AGENTS_INDEX_MS2IOSF8)) {
            continue;
          }
          NcsDedicatedCredit[IioStackIndex][MeshAgentIndex] = DEDICATED_CREDIT_MS2IOSF_TO_MS2IOSF;
          NcbDedicatedCredit[IioStackIndex][MeshAgentIndex] = DEDICATED_CREDIT_MS2IOSF_TO_MS2IOSF;
        }
      }

      if (Topology == SPR_2S1L) {
        for (IioStackIndex = IIO_STACK0; IioStackIndex < MAX_IIO_STACK; IioStackIndex++) {
          if ((IioStackIndex != IIO_STACK0) && (IioStackIndex != IIO_STACK1) && (IioStackIndex != IIO_STACK8)) {
            continue;
          }
          NcsDedicatedCredit[IioStackIndex][P2P_MESH_AGENTS_INDEX_UPI0] = DedicatedCrditToUpi0Tbl[IioStackIndex];
          NcbDedicatedCredit[IioStackIndex][P2P_MESH_AGENTS_INDEX_UPI0] = DedicatedCrditToUpi0Tbl[IioStackIndex];
          DrsDedicatedCredit[IioStackIndex][P2P_MESH_AGENTS_INDEX_UPI0] = DedicatedCrditToUpi0Tbl[IioStackIndex];
        }
      } // (Topology == SPR_2S1L)

    } else {
       NeedSetDedicatedNcbNcsDrsCredit = FALSE;
    }

    for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
      if (KtiInternalGlobal->M2PcieActiveBitmap[Soc] & (1 << IioStack)) {
        //
        // Program dedicated credits which requires setting the defeature
        // bit to take effect;Program dedicated credits for NCB,NCS and DRS
        //
        if (NeedSetDedicatedNcbNcsDrsCredit) {
          M2IALSetNcsNcbDrsDefeature (Soc, IioStack, 0x1);
          IioSetDedicatedNcsNcbDrs (Soc, IioStack, NcsDedicatedCredit[IioStack], NcbDedicatedCredit[IioStack], DrsDedicatedCredit[IioStack]);
        }
        //
        // Program meshcreditupdate
        //
        M2IalSetMeshCrdUpdate(Soc, IioStack, 0x1);
        //
        // Cleare dedicated credits defeature
        //
        if (NeedSetDedicatedNcbNcsDrsCredit) {
          M2IALSetNcsNcbDrsDefeature (Soc, IioStack, 0x0);
        }
      }
    }
  }
  //
  // Zero out shared P2P - M2IAL to M2IAL & M2IAL to M3KTI P2P Creidts
  //
  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
      if (KtiInternalGlobal->M2PcieActiveBitmap[Soc] & (1 << IioStack)) {
        M2IalGetR2InitShrCrd (Soc,
                              IioStack,
                              &R2p2pInitShCred0,
                              &R2p2pInitShCred1,
                              &R2p2pInitShCred2,
                              &R2p2pInitShCred3
                              );

        R2p2pInitShCred0 = 0;
        R2p2pInitShCred1 = 0;
        R2p2pInitShCred2 = 0;
        R2p2pInitShCred3 = 0;

        M2IalSetR2InitShrCrd (Soc,
                              IioStack,
                              R2p2pInitShCred0,
                              R2p2pInitShCred1,
                              R2p2pInitShCred2,
                              R2p2pInitShCred3
                              );
      }
    }
  }

}

/**
  Program M2M link credits

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM2mCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  UINT8 Ctr;
  UINT8 Ch;

  for (Ctr = 0; Ctr < KTIVAR->MaxSupportedM2m; Ctr++) {
    if ((KTIVAR->CpuInfo[Soc].M2mBitmap & (1 << Ctr)) == 0) {
      continue;
    }
    for (Ch = 0; Ch < MAX_MC_CH; Ch ++) {
      //
      // Scoreboard credits not set in 2LM BIOS
      //   Set M2Mem's SB credits to 126 for each MC channel:
      //   - Chnl0: Credits.CrdtWrEn = 1, Credits.CrdtType = 0x0A, Credits.CrdtCnt = 126
      //   - Chnl1: Credits.CrdtWrEn = 1, Credits.CrdtType = 0x1A, Credits.CrdtCnt = 126
      //   - Chnl2: Credits.CrdtWrEn = 1, Credits.CrdtType = 0x2A, Credits.CrdtCnt = 126
      //
      SetM2mCreditReg (Soc, Ctr, Ch, 0xa, 126);
    }
  }

  ProgramM2MVnaBlCreditForDestM2Upi (KtiInternalGlobal, Soc);

  return KTI_SUCCESS;
}

/*++
Program Transgress M2MEM Credits

  @param  KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param  MemInfo           - Memory Information structure.

  @retval None

**/
VOID
ProgramM2mTransgressCredits (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  )
{
  UINT8 AdTransgressCredits = 0;
  UINT8 BlTransgressCredits = 0;
  UINT8 Socket, Mc;
  UINT8 MaxIMC;
  UINT8 NumOfCluster;
  BOOLEAN Mem2LMEnabled;
  UINT32 AkwrCmphyst;
  UINT8 FakeTgr;
  WRTRKRALLOC_M2MEM_MIRROR_MODE MirrorWrtrkrAllocMode;
  MEM_VAR_STRUCT *mem;
  CLUSTERING_MODE DdrClustingMode;

  UINT8 HbmM2m_to_DdrM2m_Ad = 0; //2LM only
  UINT8 HbmM2m_to_DdrM2m_Bl = 0; //2LM only

  MaxIMC = GetMaxImc ();
  mem = GetMemVarPointer();
  DdrClustingMode = GetDdrClusteringMode();
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket) == FALSE)  {
      continue;
    }

    NumOfCluster = MemInfo->SncInfo[Socket].NumOfCluster;
    ChaCheck2LMEnabled (Socket, 0, &Mem2LMEnabled);
    if (KTIVAR->HbmSku == FALSE) {
      if (KTIVAR->CpuInfo[Socket].DieCount != ONE_DIE_IN_SOCKET) {
        //
        // SPR XCC 4 WAY
        //
        if (Mem2LMEnabled) {
            //
            // XCC type, 2LM
            //
          AdTransgressCredits = RESERVED_TRANSGRESS_2LM_SPR_MD / MaxIMC;
          BlTransgressCredits = RESERVED_TRANSGRESS_BL_2LM_SPR_MD_XCC / MaxIMC;
        } else { //1LM
          //
          // FLAT mode
          //
          AdTransgressCredits = RESERVED_TRANSGRESS_FLAT_SPR_MD;
          if (NumOfCluster <= 2) {
            //
            // SNC2 (Hemi mode) and Non-SNC mode
            //
            BlTransgressCredits = RESERVED_TRANSGRESS_BL_FLAT_SPR_SNC2_NOSNC;
          } else {
            //
            // SNC4 mode / Quad mode
            //
            BlTransgressCredits = RESERVED_TRANSGRESS_BL_FLAT_SPR_SNC4_MD;
          }
        }
      } else {
        //
        // V5.0 only defined XCC-4way, XCC-1D, HCC and MCC, LCC is WIP
        //
        if (KtiInternalGlobal->ChopType[Socket] == TypeXcc) {
          //
          // One Die XCC configuration
          //
          if (Mem2LMEnabled) {
            AdTransgressCredits = RESERVED_TRANSGRESS_2LM_SPR_XCC_1D;
          } else {
            AdTransgressCredits = 0;
          }
          BlTransgressCredits = RESERVED_TRANSGRESS_BL_FLAT_SPR_SNC2_NOSNC;
        } else if (KtiInternalGlobal->ChopType[Socket] == TypeMcc) {
          if (Mem2LMEnabled) {
            AdTransgressCredits = 0;
            BlTransgressCredits = 0;
          } else {
            AdTransgressCredits = RESERVED_TRANSGRESS_FLAT_SPR_MCC;
            BlTransgressCredits = RESERVED_TRANSGRESS_BL_FLAT_SPR_MCC;
          }
        } else {
          //
          // All other one die configurations uses the MCC setting for now (SPR_MCC_CHA_MAX_NUMBER)
          //
          if (Mem2LMEnabled) {
            AdTransgressCredits = RESERVED_TRANSGRESS_AD_2LM_SPR_1DIE;
            BlTransgressCredits = RESERVED_TRANSGRESS_BL_2LM_SPR_1DIE;
          } else {
            AdTransgressCredits = RESERVED_TRANSGRESS_FLAT_SPR_MCC;
            BlTransgressCredits = RESERVED_TRANSGRESS_BL_FLAT_SPR_MCC;
          }
        }
      }
    } else if (MemInfo->SncInfo[Socket].NumOfMcEnabled != 0) {
      if (Mem2LMEnabled == FALSE) {
        switch (DdrClustingMode) {
          case All2All:
            BlTransgressCredits = DDR_ALL2ALL_BL_TRANSGRESS_1LM;
            break;
          case HemiClusteringMode:
            BlTransgressCredits = DDR_HEMI_BL_TRANSGRESS_1LM;
            break;
          case QuadClusteringMode:
          case Snc4ClusteringMode:
          case MaxClusteringMode:
            BlTransgressCredits = DDR_SNC4_QUAD_BL_TRANSGRESS_1LM;
            break;
          default:
            break;
        }
      } else {
        BlTransgressCredits = 0;

        // Credit between HBM M2M and DDR M2M
        switch (DdrClustingMode) {
          case All2All:
            HbmM2m_to_DdrM2m_Ad = HBMM2M_TO_DDRM2M_AD_2LM_DDR_ALL;
            HbmM2m_to_DdrM2m_Bl = HBMM2M_TO_DDRM2M_BL_2LM_DDR_ALL;
            break;
          case HemiClusteringMode:
            break;
          case QuadClusteringMode:
          case Snc4ClusteringMode:
          case MaxClusteringMode:
            HbmM2m_to_DdrM2m_Ad = HBMM2M_TO_DDRM2M_AD_2LM_DDR_SNC4;
            HbmM2m_to_DdrM2m_Bl = HBMM2M_TO_DDRM2M_BL_2LM_DDR_SNC4;
            break;
          default:
            HbmM2m_to_DdrM2m_Ad = 0;
            HbmM2m_to_DdrM2m_Bl = 0;
            break;
        }
      }
      AdTransgressCredits = 0;
    }
      //
      // Loop for MC0, MC1, MC2 & MC3
      //
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Socket %x AdTransgressCredits: 0x%x, BlTransgressCredits: 0x%x \n", Socket, AdTransgressCredits, BlTransgressCredits));
    for (Mc = 0; Mc < MaxIMC; Mc++) {
      if ((MemInfo->imcBitmap[Socket][MC_TECH_DDR] & (BIT0 << Mc)) == 0) {
        continue;
      }
      if ((Mem2LMEnabled) && (KTIVAR->CpuInfo[Socket].DieCount != ONE_DIE_IN_SOCKET)) {
        // Credit between NMC and FMC when 2LM
        if (KTIVAR->HbmSku == FALSE) {
          SetM2mCreditReg (Socket, Mc, 0, 0x6, AdTransgressCredits); //NMC->FMC AD
          SetM2mCreditReg (Socket, Mc, 0, 0x7, BlTransgressCredits); //NMC->FMC BL
        } else {
          //credits between HbmM2m and DdrM2m
          ProgramM2mCreditBetweenHbmAndDdr(KtiInternalGlobal, Socket, HbmM2m_to_DdrM2m_Ad, HbmM2m_to_DdrM2m_Bl, DDRM2M_TO_HBMM2M_BL_2LM);
        }
      } else {
        SetM2mCreditReg (Socket, Mc, 0, 0xC, AdTransgressCredits); //Transgress AD
        SetM2mCreditReg (Socket, Mc, 0, 0xD, BlTransgressCredits); //Transgress BL
      }
      if ((MemInfo->McMirrorBitmap[Socket][MC_TECH_DDR] & (BIT0 << Mc)) != 0) {
        ZeroMem (&MirrorWrtrkrAllocMode, sizeof (WRTRKRALLOC_M2MEM_MIRROR_MODE));

        AkwrCmphyst = GetM2mWrTrKrAllocAkwrCmphyst (Socket, Mc);
        FakeTgr = 0;  // use zero for 1LM mirror

        MirrorWrtrkrAllocMode.AkwrCmphystHi         = AkwrCmphyst >> 16;
        MirrorWrtrkrAllocMode.AkwrCmphystLo         = (UINT16) AkwrCmphyst;
        MirrorWrtrkrAllocMode.MirrTrkrPwrThresh     = 0x0;
        MirrorWrtrkrAllocMode.MirrTrkrThresh        = 29 - BlTransgressCredits - FakeTgr;
        MirrorWrtrkrAllocMode.MirrTrkrFakeTgrThresh = FakeTgr;

        SetM2mWrTrKrAllocMirrorMode (Socket, Mc, &MirrorWrtrkrAllocMode);
      }
    }//end Ctr loop
  } //end Socket loop
}

/*++
  Program Prefetech M2MEM Credits

  @param  Host                     Pointer to the system Host (root) structure
  @param  Socket                   Socket Id
  @param  MemInfo                  Memory Information structure.
  @param  Ctr                      MC Number.

  @retval None
--*/
VOID
ProgramM2mPrefetchCredit (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  )
{
  UINT8 XptPrefetchCredits = 0;
  UINT8 Socket, Mc, MaxIMC;
  UINT8 NumOfCha, NumOfCluster;
  BOOLEAN Mem2LMEnabled, ADMemPresent;
  SAD_TABLE  *SADEntry;
  UINT8   i;
  MEM_VAR_STRUCT *mem;

  MaxIMC = GetMaxImc ();
  ADMemPresent = FALSE;
  mem = GetMemVarPointer();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket)) {
      NumOfCha = KTIVAR->CpuInfo[Socket].TotCha;
      NumOfCluster = MemInfo->SncInfo[Socket].NumOfCluster;
      ChaCheck2LMEnabled (Socket, 0, &Mem2LMEnabled);

      if (KTIVAR->CpuInfo[Socket].DieCount != ONE_DIE_IN_SOCKET) {
        //
        // SPR XCC 4 WAY / HCC 2 Way
        //
        if (Mem2LMEnabled) {
          //
          // Check if AD Memory exist when it is HCC 2-way
          //
          if (NumOfCha <= SPR_HCC_CHA_MAX_NUMBER) {
            for (i = 0; i < MAX_SAD_RULES; i++) {
              SADEntry = &mem->socket[Socket].SAD[i];
              if (SADEntry->Enable == 0) {
                break; // end of SAD table
              }
              ADMemPresent = IsMemTypeAppDirect (SADEntry->type);
              if (ADMemPresent) {
                break;
              }
            }

            if ((!ADMemPresent) && (NumOfCluster >= 2)) {
              //
              // without AD Memory and in SNC mode (2LM)
              //
              XptPrefetchCredits = RESERVED_XPTPREFETCH_2LM_SPR_MD;
            } else {
              XptPrefetchCredits = RESERVED_XPTPREFETCH_2LM_SPR_MD_HCC;
            }
          } else {
            //
            // XCC type, 2LM
            //
            XptPrefetchCredits = RESERVED_XPTPREFETCH_2LM_SPR_MD;
          }
        } else {
          //
          // FLAT mode
          //
          if (((NumOfCha <= SPR_XCC_CHA_MAX_NUMBER) && (NumOfCha > SPR_HCC_CHA_MAX_NUMBER)) &&
              (NumOfCluster == 4)) {
            XptPrefetchCredits = RESERVED_XPTPREFETCH_FLAT_SPR_SNC4_MD_XCC;
          } else {
            XptPrefetchCredits = RESERVED_XPTPREFETCH_FLAT_SPR_MD;
          }
        }
      } else {
        //
        // V5.0 only defined XCC-4way, XCC-1D, HCC and MCC, LCC is WIP
        //
        if (KtiInternalGlobal->ChopType[Socket] == TypeXcc) {
          //
          // One Die XCC configuration
          //
          if (Mem2LMEnabled) {
            XptPrefetchCredits = RESERVED_XPTPREFETCH_2LM_SPR_MD;
          } else {
            XptPrefetchCredits = RESERVED_XPTPREFETCH_FLAT_SPR_MD;
          }
        } else {
          //
          // All other one die configurations uses the MCC setting for now (SPR_MCC_CHA_MAX_NUMBER)
          //
          XptPrefetchCredits = RESERVED_XPTPREFETCH_SPR_MCC;
        }
      }

      //
      // for Mc0, Mc1, Mc2 and Mc3
      //
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Socket %x XptPrefetchCredits: 0x%x \n", Socket, XptPrefetchCredits));
      for (Mc = 0; Mc < MaxIMC; Mc++) {
        if ((MemInfo->imcBitmap[Socket][MC_TECH_DDR] & (BIT0 << Mc)) == 0) {
          continue;
        }
        SetM2mCreditReg (Socket, Mc, 0, 0xb, XptPrefetchCredits);
      }
    } //end SocketPresent
  } //end Socket Loop
}

/**
  Program CHA to HBM M2MEM Credits

  @param  MemInfo          - Memory Information structure.
  @param  SocId            - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramChaToHbmM2mCredit(
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  UINT8                      SocId
  )
{
  UINT8 NumOfCha;
  BOOLEAN Mem2LMEnabled;

  UINT8 HbmAdClusterCHAsCloseVal;
  UINT8 HbmAdClusterCHAsDistantVal;
  UINT8 HbmBlClusterCHAsCloseVal;
  UINT8 HbmBlClusterCHAsDistantVal;


  NumOfCha = KTIVAR->CpuInfo[SocId].TotCha;
  ChaCheck2LMEnabled(SocId, 0, &Mem2LMEnabled);


  if (!Mem2LMEnabled) { //1LM
    //HBM SNC4/QUAD
    //HBM SNC4/QUAD + DDR SNC4/QUAD
    //HBM SNC4/QUAD + DDR Hemi
    //HBM SNC4/QUAD + DDR All-to-All
    HbmAdClusterCHAsCloseVal = HBM_AD_CLUSTER_CHAS_CLOSE;
    HbmAdClusterCHAsDistantVal = HBM_AD_CLUSTER_CHAS_DISTANT;
    HbmBlClusterCHAsCloseVal = HBM_BL_CLUSTER_CHAS_CLOSE;
    HbmBlClusterCHAsDistantVal = HBM_BL_CLUSTER_CHAS_DISTANT;
  } else { //2LM
    //HBM SNC4/QUAD + DDR SNC4/QUAD
    //HBM SNC4/QUAD + DDR All to All

    HbmAdClusterCHAsCloseVal = HBM_AD_CLUSTER_CHAS_CLOSE_2LM;
    HbmAdClusterCHAsDistantVal = HBM_AD_CLUSTER_CHAS_DISTANT_2LM;
    HbmBlClusterCHAsCloseVal = HBM_BL_CLUSTER_CHAS_CLOSE_2LM;
    HbmBlClusterCHAsDistantVal = HBM_BL_CLUSTER_CHAS_DISTANT_2LM;
  } //2LM

  //Program CHA to HBM M2M crdeit
  ChaConfigureChaToHbmM2mCredit(
      SocId,
      NumOfCha,
      HbmAdClusterCHAsCloseVal,
      HbmAdClusterCHAsDistantVal,
      HbmBlClusterCHAsCloseVal,
      HbmBlClusterCHAsDistantVal);

  return KTI_SUCCESS;
}

/*++
  Program credits between HBM's M2M and DDR's M2M.
  This is for 2LM case (HBM caching DDR)

  @param  KtiInternalGlobal         pointer to the KTI RC internal global structure
  @param SocId                      CPU Socket Node number (Socket ID)
  @param AdCreditsHbmM2mToDdrM2m    Ad credit from HBM's M2M to DDR's M2M
  @param BlCreditsHbmM2mToDdrM2m    Bl credit from HBM's M2M to DDR's M2M
  @param BlCreditsDdrM2mToHbmM2m    Bl credit from DDR's M2M to HBM's M2M

  @retval None
--*/
VOID
ProgramM2mCreditBetweenHbmAndDdr (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId,
  UINT8                      AdCreditsHbmM2mToDdrM2m,
  UINT8                      BlCreditsHbmM2mToDdrM2m,
  UINT8                      BlCreditsDdrM2mToHbmM2m
  )
{
  UINT8 Mc, MaxIMC, CrdtType, NmId, FmId;

  MaxIMC = GetMaxImc ();

  //mc index from 0 to MaxIMC is for DDR, i.e., far mem (FM)
  //mc index from MaxIMC to MaxSupportedM2m is for HBM, i.e., near mem (NM)
  for (Mc = 0; Mc < KTIVAR->MaxSupportedM2m; Mc++) {
    if ((KTIVAR->CpuInfo[SocId].M2mBitmap & (1 << Mc)) == 0) {
      continue;
    }

    if (Mc < MaxIMC) {
      //credit for DDR -> HBM traffic
      for (NmId = 0; NmId < 4; NmId++) {
        //CrdtType[6]: 1
        //CrdtType[5:4]: 11, XTILE_NM_BL credits on this FMCTILE for each NMC xtile target
        //CrdtType[3:0]: the logical NMC id (0...11).
        CrdtType = 0x70 | NmId;
        SetM2mCreditReg(SocId, Mc, 0, CrdtType, BlCreditsDdrM2mToHbmM2m); //FMC->NMC Fills
      }
    } else {
      //credit for HBM -> DDR traffic
      for (FmId = 0; FmId < MaxIMC; FmId++) {
        //CrdtType[5:4]: FM ID; [3:0]: 0110 for AD; 0111 for BL
        SetM2mCreditReg(SocId, Mc, FmId, 0x6, AdCreditsHbmM2mToDdrM2m); //NMC->FMC AD
        SetM2mCreditReg(SocId, Mc, FmId, 0x7, BlCreditsHbmM2mToDdrM2m); //NMC->FMC BL
      }
    }
  }
}

/**
  Program Cha->M2MEM credits for Non-SNC mode in cold rest after MRC

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param MemInfo           - Memory Information structure.
  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
ProgramNonSncChaToM2mCredit (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN MEM_INFO                *MemInfo
  )
{
  UINT8    Soc;

  //
  // Program the values into each socket.
  //
  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    if (SocketPresent (Soc)) {
      ProgramChaToM2mCreditWithoutCluster (KtiInternalGlobal, MemInfo, Soc);
    }
  }
  return KTI_SUCCESS;
}

/**
  Update M2Pcie credits

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
UpdateM2PcieCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8  Socket;
  UINT8  Cha;
  UINT8  M2Pcie;
  UINT8  RegisterIndex;
  UINT8  PrqPerIoStack;
  UINT8  RemainingPrq;
  UINT8  NumOfRegister;
  UINT8  CreditsPerCha;
  UINT32 Credit;
  BOOLEAN  AepDimmPresent;

  //
  // Program  M2IOSF->CHA credits, row 73
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket)) {
      //
      // Calculate expected number of PRQ credits per IO stack
      //
      NumOfRegister = KTIVAR->CpuInfo[Socket].TotCha / 8;
      PrqPerIoStack = (TOTAL_MS2IOSF2CHA_CREDIT_SPR / KTIVAR->CpuInfo[Socket].TotM2Pcie);
      RemainingPrq = TOTAL_MS2IOSF2CHA_CREDIT_SPR % KTIVAR->CpuInfo[Socket].TotM2Pcie;
      AepDimmPresent = CheckSocketDcpmmPresent (Socket);
      //
      // Need to reprogram the PRQ credits for this Socket
      //
      for (M2Pcie = 0; M2Pcie < MAX_IIO_STACK; M2Pcie++) {
        if (KTI_HOST_OUT_PTR->M2PciePresentBitmap[Socket] & (1 << M2Pcie)) {
          if ((KTIVAR->CpuInfo[Socket].CpuRes.StackRes[M2Pcie].Personality == TYPE_UBOX_IIO)
            || (KTIVAR->CpuInfo[Socket].CpuRes.StackRes[M2Pcie].Personality == TYPE_DINO))
          {
            CreditsPerCha = PrqPerIoStack;
            if (RemainingPrq) {
              CreditsPerCha++;
              RemainingPrq--;
            }
            if (AepDimmPresent) {
              CreditsPerCha = CreditsPerCha - RESERVED_MS2IOSF2CHA_DDRT;
            }

            if (CreditsPerCha > 15) {
              CreditsPerCha = 15;
            }

            if (KTIVAR->CpuInfo[Socket].TotCha >= 8) {
              Credit = 0;
              //
              // Fill all fields for all 8 CHAs
              //
              for (Cha = 0; Cha < 8; Cha++) {
                Credit = Credit | (CreditsPerCha << (4 * (Cha % 8)));
              }
              //
              // Program registers R2PRQINITCRED* registers
              //
              for (RegisterIndex = 0; RegisterIndex < NumOfRegister; RegisterIndex++) {
                M2IalSetPrqInitCrdIdx (Socket, M2Pcie, RegisterIndex, Credit); //M2PCIE->CHA
              }
            }
            //
            // Program credits for remaining CHAs in the final R2PRQINITCRED register
            //
            if (KTIVAR->CpuInfo[Socket].TotCha % 8 != 0) {
              Credit = 0;
              for (Cha = 0; Cha < (KTIVAR->CpuInfo[Socket].TotCha % 8); Cha++){
                Credit = Credit | (CreditsPerCha << (4 * Cha));
              }
              M2IalSetPrqInitCrdIdx (Socket, M2Pcie, NumOfRegister, Credit);//remaining M2PCIE-> CHA
            }
          }
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Program system coherency config register

  @param  KtiInternalGlobal - pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS - on successful completion

**/

EFI_STATUS
ProgramSystemCoherencyConfig (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  BOOLEAN                   Mem2LMEnabled;
  BOOLEAN                   IpqDisable;
  BOOLEAN                   EnablePushMtoIIodcTwoLMOpt;
  UINT8                     Socket;
  UINT8                     Ctr;

  if (KTIVAR->SysConfig == SYS_CONFIG_1S) {
    IpqDisable = 1;
  } else {
    IpqDisable = 0;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket)) {

      ChaCheck2LMEnabled (Socket, 0, &Mem2LMEnabled);  // Check if a socket is in 2LM Mode

      if ((KTIVAR->SysConfig != SYS_CONFIG_1S) && Mem2LMEnabled) {
        EnablePushMtoIIodcTwoLMOpt = 1;
      } else {
        EnablePushMtoIIodcTwoLMOpt = 0;
      }

      for (Ctr = 0; Ctr < KtiInternalGlobal->TotCha[Socket]; Ctr++) {
        ChaConfigureIpqAndEnablePushMtoIIodcTwoLMOpt (
            Socket,
            Ctr,
            IpqDisable,
            EnablePushMtoIIodcTwoLMOpt
            );
      }
    }
  }

  return KTI_SUCCESS;
}
