/** @file

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
#include <Guid/M2UPcieIpInterface.h>
#include <Guid/IioIpInterface.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/BaseMemoryLib.h>

STATIC UINT8 NcbDedicated[MAX_IIO_STACK][MAX_IIO_STACK] = {
                                                          {1, 1, 1, 1, 1, 1},
                                                          {1, 1, 1, 2, 8, 3},
                                                          {1, 1, 1, 2, 1, 3},
                                                          {1, 1, 1, 1, 1, 3},
                                                          {1, 8, 3, 2, 1, 3},
                                                          {1, 2, 5, 5, 2, 1}
                                                          };

STATIC UINT8 NcsDedicated[MAX_IIO_STACK][MAX_IIO_STACK] = {
                                                          {0, 0, 0, 0, 0, 0},
                                                          {0, 0, 0, 1, 6, 1},
                                                          {0, 0, 0, 0, 0, 3},
                                                          {0, 0, 0, 0, 0, 1},
                                                          {0, 6, 2, 1, 0, 1},
                                                          {0, 0, 6, 5, 0, 0}
                                                          };
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
        M2UPcieSetR2CrdErrMask (Soc, Src, Mask);
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

  ProgramChaCreditSetting (KtiInternalGlobal, Sbsp);            //CHA
  ProgramM2PcieCreditSetting (KtiInternalGlobal, Sbsp);         //M2PCIE

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
      ProgramM2PcieCreditSetting (KtiInternalGlobal, Soc);        //M2PCIE
      ProgramChaCreditSetting (KtiInternalGlobal, Soc);           //CHA
      ProgramM3KtiCreditSetting (KtiInternalGlobal, Soc);         //M3KTI
      ProgramKtiCreditSetting (KtiInternalGlobal, Soc);           //KTI
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
  UINT8   NumOfCha, Mc;
  UINT32  AdCreditsToM2mPerCha =0;
  UINT32  BlCreditsToM2mPerCha =0;
  UINT8   AdCreditsLeftOver = 0;
  UINT8   BlCreditsLeftOver = 0;
  UINT8   AdCreditsReserved, BlCreditsReserved;
  UINT8   MaxIMC;
  BOOLEAN AllM2MEnabled = TRUE;
  UINT8   CpuType;
  BOOLEAN AepDimmPresent, Mem2LMEnabled;

  MaxIMC   = GetMaxImc ();
  NumOfCha = KTIVAR->CpuInfo[SocId].TotCha;

  CpuType  = KtiInternalGlobal->CpuType;
  ChaCheck2LMEnabled (SocId, 0, &Mem2LMEnabled);

  if (CpuType == CPU_ICXD) {
    if (Mem2LMEnabled && NumOfCha <= LCC_CHA_MAX_NUMBER_ICXD) {
      AdCreditsReserved = RESERVED_AD_TRANSGRESS_XPTPREFETCH_2LM_ICXD_LCC;
    } else {
      AdCreditsReserved = RESERVED_AD_TRANSGRESS_XPTPREFETCH_ICXD;
    }
    BlCreditsReserved = RESERVED_BL_TRANSGRESS_ICXD;
  } else if (CpuType == CPU_SNR || CpuType == CPU_TNR) {
    AdCreditsReserved = RESERVED_AD_TRANSGRESS_XPTPREFETCH_SNR;
    BlCreditsReserved = RESERVED_BL_TRANSGRESS_SNR;
  } else {
    BlCreditsReserved = RESERVED_TRANSGRESS_BL_ICX;

    if (!Mem2LMEnabled) {
      //
      //  Flat mode or 1LM mode & no SNC
      //
      AepDimmPresent = CheckSocketDcpmmPresent (SocId);

      if (AepDimmPresent) {
        //
        // ICX 1LM DDR + DDRT, follows the credit settings of 2LM && all M2Ms populated config.
        //
        if (NumOfCha <= LCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC4_NOSNC_LCC;
        } else if (NumOfCha <= HCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC2_NOSNC_HCC;
        } else {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX;
        }
      } else {
        if (NumOfCha <= LCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC2_NOSNC_LCC;
          BlCreditsReserved = RESERVED_TRANSGRESS_BL_FLAT_ICX_SNC2_NOSNC_LCC;
        } else if (NumOfCha <= HCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_NOSNC_HCC;
        } else {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC2_NOSNC;
        }
      }
    } else {
      //
      // 2LM mode
      //
      if (NumOfCha <= LCC_CHA_MAX_NUMBER) {
        AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC4_NOSNC_LCC;
      } else if (NumOfCha <= HCC_CHA_MAX_NUMBER) {
        AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC2_NOSNC_HCC;
      } else {
        for (Mc = 0; Mc < MaxIMC; Mc++) {
          if ((MemInfo->imcBitmap[SocId][MC_TECH_DDR] & (BIT0 << Mc)) == 0) {
            AllM2MEnabled = FALSE;
            break;
          }
        }
        if (AllM2MEnabled) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX;
        } else {
          //
          // Some M2Ms populated condition: has different AD reserved credit
          //
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SM2M;
        }
      }
    }
  }
  AdCreditsToM2mPerCha = (TOTAL_M2M_AD_CREDIT_10nm - AdCreditsReserved) / NumOfCha;
  AdCreditsLeftOver = (TOTAL_M2M_AD_CREDIT_10nm - AdCreditsReserved) % NumOfCha;

  BlCreditsToM2mPerCha = (TOTAL_M2M_BL_CREDIT_10nm - BlCreditsReserved) / (NumOfCha);
  BlCreditsLeftOver = (TOTAL_M2M_BL_CREDIT_10nm - BlCreditsReserved) % (NumOfCha);

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

  Program ICX CHA->M2M credits when there are clusters

  @param  MemInfo          - Memory Information structure.
  @param  SocId            - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramChaToM2mCreditWithClusterIcx(
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
  BOOLEAN AepDimmPresent, Mem2LMEnabled;
  UINT8   CpuType;
  UINT32  AdCreditMask, BlCreditMask;

  NumOfCha = KTIVAR->CpuInfo[SocId].TotCha;
  NumOfChaPerCluster = MemInfo->SncInfo[SocId].NumOfChaPerCluster;
  NumOfCluster = MemInfo->SncInfo[SocId].NumOfCluster;
  MaxIMC = GetMaxImc ();
  CpuType = KtiInternalGlobal->CpuType;

  if (NumOfCluster == 0) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nThis function is for NumOfCluster!=0"));
    return KTI_FAILURE;
  }

  if (NumOfChaPerCluster == 0) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nNumOfChaPerCluster can't be 0"));
    return KTI_FAILURE;
  }

  ChaCheck2LMEnabled (SocId, 0, &Mem2LMEnabled);

  if (CpuType == CPU_ICXD) {
    if (Mem2LMEnabled) {
      AdCreditsReserved = RESERVED_AD_TRANSGRESS_XPTPREFETCH_ICXD;
      NonClusterChaCredit = 1;
    } else {
      AdCreditsReserved = RESERVED_AD_TRANSGRESS_XPTPREFETCH_SNC_ICXD;
    }
    BlCreditsReserved = RESERVED_BL_TRANSGRESS_ICXD;
    AdCreditMask = BlCreditMask = 0xFFFFFF00;
  } else {
    BlCreditsReserved = RESERVED_TRANSGRESS_BL_ICX;
    AepDimmPresent = CheckSocketDcpmmPresent (SocId);

    if (!Mem2LMEnabled && !AepDimmPresent) {
      //
      // SNC Mode and FLAT Mode without DDRT
      //
      if (NumOfCluster == 4) {
        //
        // SNC4 Mode
        //
        if (NumOfCha <= LCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC4_LCC;
        } else if (NumOfCha <= HCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC4_HCC;
        } else {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC4;
        }
      } else {
        //
        //  SNC2 Mode
        //
        if (NumOfCha <= LCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC2_NOSNC_LCC;
          BlCreditsReserved = RESERVED_TRANSGRESS_BL_FLAT_ICX_SNC2_NOSNC_LCC;
        } else if (NumOfCha <= HCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC2_HCC;
        } else {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC2_NOSNC;
        }
      }
    } else {
      //
      //  SNC Mode --- 2LM Mode or 1LM DDRT + DDRT
      //
      if (NumOfCluster == 4) {
        //
        // SNC4
        //
        if (NumOfCha <= LCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC4_NOSNC_LCC;
        } else if (NumOfCha <= HCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC4_HCC;
        } else {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX;
        }
        NonClusterChaCredit = 1;
      } else {
        //
        // SNC2
        //
        if (NumOfCha <= LCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC2_LCC;
        } else if (NumOfCha <= HCC_CHA_MAX_NUMBER) {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC2_NOSNC_HCC;
        } else {
          AdCreditsReserved = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX;
        }
        NonClusterChaCredit = 1;
      }
    }
    AdCreditMask = BlCreditMask = 0xFFFF0000;
  }
  AdCreditsToM2mPerCha = (TOTAL_M2M_AD_CREDIT_10nm - AdCreditsReserved - NonClusterChaCredit * (NumOfCha - NumOfChaPerCluster)) / (NumOfChaPerCluster);
  AdCreditsLeftOver = (TOTAL_M2M_AD_CREDIT_10nm - AdCreditsReserved - NonClusterChaCredit * (NumOfCha - NumOfChaPerCluster)) % (NumOfChaPerCluster);
  BlCreditsToM2mPerCha = (TOTAL_M2M_BL_CREDIT_10nm - BlCreditsReserved - NonClusterChaCredit * (NumOfCha - NumOfChaPerCluster)) / (NumOfChaPerCluster);
  BlCreditsLeftOver = (TOTAL_M2M_BL_CREDIT_10nm - BlCreditsReserved - NonClusterChaCredit * (NumOfCha - NumOfChaPerCluster)) % (NumOfChaPerCluster);

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
    AdCreditMask,
    BlCreditMask
    );

  return KTI_SUCCESS;
}

/**
  Program CHA->M2M credits when there are clusters

  @param  KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param  MemInfo           - Memory Information structure.
  @param  SocId             - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramChaToM2mCreditWithCluster (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo,
  UINT8                      SocId
  )
{
  ProgramChaToM2mCreditWithClusterIcx (KtiInternalGlobal, MemInfo, SocId);

  return KTI_SUCCESS;
}

/**
  Program M2Pcie credits

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramM2PcieCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  UINT8  Src;
  UINT8  PrqPerIoStack;
  UINT8  RemainingPrq;
  UINT8  Idx1;
  UINT8  SkipRemainderStackId;

  //
  // M2IOSF->M2UPI credit programming
  //
  ProgramM2iosfToM2upiCreditSetting (KtiInternalGlobal, Soc);

  //
  // M2IOSF->CHA Credits
  //
  RemainingPrq = TOTAL_PRQ_10nm % KtiInternalGlobal->TotActiveM2Pcie[Soc];

  //
  // If there is a remainder after even distribution of credits among stacks, one stack can waive its claim to a remainder credit.
  // else, one remainder credit (if available) will be assigned to each stack in ascending order.
  //
  SkipRemainderStackId = 0xFF;
  if (KtiInternalGlobal->CpuType == CPU_SNR || KtiInternalGlobal->CpuType == CPU_TNR) {
    //
    // In SNR, M2IOSF connected to Rlink (M2IOSF0) is chosen to waive extra remainder credit.
    //
    SkipRemainderStackId = IIO_STACK0;
  }

  //
  //(24-(total # of credits for FXR+total # of credits for GT))/# of independent MS2IOSF
  //
  for (Src = 0; Src < MAX_IIO_STACK; Src++) {
    if (KtiInternalGlobal->M2PcieActiveBitmap[Soc] & (1 << Src)) {
      PrqPerIoStack = TOTAL_PRQ_10nm / KtiInternalGlobal->TotActiveM2Pcie[Soc];
      if ((Src != SkipRemainderStackId) && (RemainingPrq)) {
        //
        //if still has remaining PRQ left, then assign one to current M2Pcie
        //
        PrqPerIoStack++;
        RemainingPrq--;
      }
      M2UPcieSetR2InitialCrd (Soc, Src, KtiInternalGlobal->TotCha[Soc], PrqPerIoStack);
    }
  }

  for (Idx1 = 0; Idx1 < MAX_IIO_STACK; Idx1++) {
    Src = Idx1 - 1;
    if (KtiInternalGlobal->M2PcieActiveBitmap[Soc] & (1 << Src)) {
      //
      // Program MS2IOSF->UBOX credits
      //
      M2UPcieSetIio2UboxInitCrd (Soc, Src);
    }//if M2PcieActiveBitmap valid
  }//for M2PCIE

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
  UINT8                                 Src;
  UINT32                                R2p2pInitShCred0;
  UINT32                                R2p2pInitShCred1;
  UINT32                                R2p2pInitShCred2;

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    if (SocketPresent (Soc)) {
      //
      // Program M2IOSF->M2IOSF P2P credit ring bitfields
      //
      M2UPcieProgramM2iosfToM2iosfP2pCredits (Soc, KTI_HOST_OUT_PTR->M2PciePresentBitmap[Soc]);
      //
      // Program M2IOSF->M2UPI P2P credit ring bitfields (multisocket build only)
      //
      ProgramM2iosfToM2upiP2pCredits (KtiInternalGlobal, Soc);
    }
  }

  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    for (Src = 0; Src < MAX_IIO_STACK; Src++) {
      if (KTI_HOST_OUT_PTR->M2PciePresentBitmap[Soc] & (1 << Src)) {
        //
        // Program dedicated credits which requires setting the defeature
        // bit to take effect.
        //
        M2UPcieSetNcsDefeature (Soc, Src, 0x1);
        IioSetDedicatedNcsNcbDrs (Soc, Src, NcsDedicated[Src], NcbDedicated[Src], NULL);
        //
        // Program meshcreditupdate
        //
        M2UPcieSetMeshCrdUpdate (Soc, Src, 0x1);
        //
        // Restore the defeature bit right after doing the mesh
        // credit update.
        //
        M2UPcieSetNcsDefeature (Soc, Src, 0x0);
        //
        // Zero out M2IOSF->M2PCIE Credits
        //
        M2UPcieGetR2InitShrCrd (Soc, Src, &R2p2pInitShCred0, &R2p2pInitShCred1, &R2p2pInitShCred2);
        R2p2pInitShCred0 = 0;
        R2p2pInitShCred1 = 0;
        R2p2pInitShCred2 = 0;
        M2UPcieSetR2InitShrCrd (Soc, Src, R2p2pInitShCred0, R2p2pInitShCred1, R2p2pInitShCred2);
      }
    }
  }
}

/**
  Program CHA credits.

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param Soc               - Socket ID

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
ProgramChaCreditSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc
  )
{
  UINT8  Cha;
  UINT8  NumOfCha;
  UINT8  NumofActiveKtiLink;
  UINT8  ReservedCredits;
  UINT32 BlCreditsToM2pciePerCha;
  UINT32 BlCreditsToM2pciePerChaLimit;

  //CHA->M2PCIe and CHA->UBOX

  ProgramChaToM2upiCreditSetting (KtiInternalGlobal, Soc);

  NumofActiveKtiLink = CountActiveKtiLink (KtiInternalGlobal, Soc);
  NumOfCha = KtiInternalGlobal->TotCha[Soc];
  // Force to min of 2CHA if detect 1CHA
  if ( NumOfCha < 2) {
    NumOfCha  = 2;
  }

  //
  //CHA->M2PCIE BL VNA
  //
  if (KtiInternalGlobal->CpuType == CPU_ICXSP) {
    ReservedCredits = 14 - (2 * NumofActiveKtiLink); // Remainder of M2PCIE credit buffer, variable based on number of KTI links
  } else {
    ReservedCredits = 1;
  }

  BlCreditsToM2pciePerCha = (MAX_CHA_TO_M2PCIE_BL_VNA_10nm -
                              (SHARED_P2P_CREDIT_SAME_RING + SHARED_P2P_CREDIT_DIFF_RING  /// Shared P2P M2PCIE BL Credit
                              + UBOX_TO_M2PCIE_VN0_BL_INGRESS_10nm          //VN0 credits for Ubox+ fixed VN0
                              + CHA_TO_M2PCIE_VN0_BL_INGRESS_10nm           //fixed VN0 credits for CHAs
                              + (KtiInternalGlobal->TotActiveM2Pcie[Soc]-1) //VN0 for MS2IOSF
                              + (2 * NumofActiveKtiLink)                    //VN0 for UPI
                              + ReservedCredits                             //reserved credit
                              )
                            );

  if (KtiInternalGlobal->CpuType == CPU_ICXD) {
    BlCreditsToM2pciePerChaLimit = (MAX_M2IOSF_INGRESS_BUFFER_CHA_BL_PKTS -
                                    (UBOX_TO_M2PCIE_VN0_BL_INGRESS_10nm +
                                     CHA_TO_M2PCIE_VN0_BL_INGRESS_10nm)
                                   );
    if (BlCreditsToM2pciePerCha > BlCreditsToM2pciePerChaLimit) {
      BlCreditsToM2pciePerCha = BlCreditsToM2pciePerChaLimit;
    }
  }

  BlCreditsToM2pciePerCha = BlCreditsToM2pciePerCha / NumOfCha;
  //
  //Program CHA registers
  //
  for (Cha = 0; Cha < KtiInternalGlobal->TotCha[Soc]; Cha ++) {
    ChaConfigureChaToM2PCIeCredit (Soc, Cha, BlCreditsToM2pciePerCha);
  }//CHA loop

  return KTI_SUCCESS;
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

  ProgramM2memToM2upiCreditSetting (KtiInternalGlobal, Soc);

  for (Ctr = 0; Ctr < KTIVAR->CpuInfo[Soc].TotM2m; Ctr++) {
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

  return KTI_SUCCESS;
}

/*++
Program Transgress M2MEM Credits

  @param  KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param  MemInfo           - Memory Information structure.

  @retval None

**/
VOID
ProgramM2mTransgressCredits(
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  )
{
  UINT8   AdTransgressCredits = 0;
  UINT8   BlTransgressCredits = 0;
  UINT8   Socket, Mc;
  UINT8   MaxIMC;
  UINT8   CpuType;
  UINT8   NumOfCha, NumOfCluster;
  BOOLEAN AepDimmPresent, Mem2LMEnabled;
  UINT32  AkwrCmphyst;
  UINT8   FakeTgr;
  WRTRKRALLOC_M2MEM_MIRROR_MODE MirrorWrtrkrAllocMode;

  MaxIMC = GetMaxImc ();

  CpuType = KtiInternalGlobal->CpuType;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket)) {
      NumOfCha = KTIVAR->CpuInfo[Socket].TotCha;
      NumOfCluster = MemInfo->SncInfo[Socket].NumOfCluster;

      ChaCheck2LMEnabled (Socket, 0, &Mem2LMEnabled);
      if (Mem2LMEnabled) {
        //2LM - DDR4 Cache DDRT
        if (CpuType == CPU_ICXD) {
          if (NumOfCha <= LCC_CHA_MAX_NUMBER_ICXD) {
            AdTransgressCredits = RESERVED_AD_TRANSGRESS_2LM_ICXD_LCC;
          } else {
            AdTransgressCredits = RESERVED_AD_TRANSGRESS_2LM_ICXD;
          }
          BlTransgressCredits = RESERVED_BL_TRANSGRESS_ICXD;
        } else {
          //
          // 2LM Mode
          // XCC & HCC & LCC has the same setting on Transgress AD/BL
          //
          AdTransgressCredits = RESERVED_TRANSGRESS_2LM_ICX;
          BlTransgressCredits = RESERVED_TRANSGRESS_BL_ICX;
        }
      } else {
        //
        // 1LM Mode
        //
        AepDimmPresent = CheckSocketDcpmmPresent (Socket);

        if (CpuType == CPU_ICXD) {
          AdTransgressCredits = RESERVED_AD_TRANSGRESS_ICXD;
          BlTransgressCredits = RESERVED_BL_TRANSGRESS_ICXD;
        } else if (CpuType == CPU_SNR || CpuType == CPU_TNR) {
          AdTransgressCredits = RESERVED_AD_TRANSGRESS_SNR;
          BlTransgressCredits = RESERVED_BL_TRANSGRESS_SNR;
        } else {
          //
          // FLAT Mode --- XCC & HCC & LCC has the same setting on Transgress AD
          //
          AdTransgressCredits = RESERVED_TRANSGRESS_FLAT_ICX;
          if ((NumOfCha <= LCC_CHA_MAX_NUMBER) && (NumOfCluster != 4) && !AepDimmPresent) {
            //
            // LLC has different BL Transgress credit in the FLAT mode (NOSNC and SNC2 mode) without DDRT
            //
            BlTransgressCredits = RESERVED_TRANSGRESS_BL_FLAT_ICX_SNC2_NOSNC_LCC;
          } else {
            BlTransgressCredits = RESERVED_TRANSGRESS_BL_ICX;
          }
        }
      } //end 2LM/1LM mode

      //
      // Loop for MC0 and MC1
      //
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Socket %x AdTransgressCredits: 0x%x, BlTransgressCredits: 0x%x \n", Socket, AdTransgressCredits, BlTransgressCredits));
      for (Mc = 0; Mc < MaxIMC; Mc++) {
        if ((MemInfo->imcBitmap[Socket][MC_TECH_DDR] & (BIT0 << Mc)) == 0) {
          continue;
        }
        SetM2mCreditReg (Socket, Mc, 0, 0xC, AdTransgressCredits);
        SetM2mCreditReg (Socket, Mc, 0, 0xD, BlTransgressCredits);

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
    } //end SocketPresent
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
  UINT8   XptPrefetchCredits = 0;
  UINT8   Socket, Mc, MaxIMC;
  BOOLEAN AllM2MEnabled = TRUE;
  UINT8   CpuType;
  UINT8   NumOfCha, NumOfCluster;
  BOOLEAN AepDimmPresent, Mem2LMEnabled;

  MaxIMC = GetMaxImc ();

  CpuType    = KtiInternalGlobal->CpuType;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket)) {
      NumOfCha = KTIVAR->CpuInfo[Socket].TotCha;
      NumOfCluster = MemInfo->SncInfo[Socket].NumOfCluster;

      ChaCheck2LMEnabled (Socket, 0, &Mem2LMEnabled);
      if (Mem2LMEnabled) {
        //2LM - DDR4 Cache DDRT
        if (CpuType == CPU_ICXD) {
          if (NumOfCha <= LCC_CHA_MAX_NUMBER_ICXD) {
            XptPrefetchCredits = RESERVED_XPTPREFETCH_2LM_ICXD_LCC;
          } else {
            XptPrefetchCredits = RESERVED_XPTPREFETCH_ICXD_2LM;
          }
        } else {
          //
          // 2LM Mode
          //
          if (NumOfCha <= LCC_CHA_MAX_NUMBER) {
            if (NumOfCluster == 2) {
              //
              // SNC2
              //
              XptPrefetchCredits = RESERVED_XPTPREFETCH_2LM_ICX_SNC2_LCC;
            } else {
              XptPrefetchCredits = RESERVED_XPTPREFETCH_2LM_ICX_SNC4_NOSNC_LCC;
            }
          } else if (NumOfCha <= HCC_CHA_MAX_NUMBER) {
            if (NumOfCluster == 4) {
              //
              // SNC4
              //
              XptPrefetchCredits = RESERVED_XPTPREFETCH_2LM_ICX_SNC4_HCC;
            } else {
              XptPrefetchCredits = RESERVED_XPTPREFETCH_2LM_ICX_SNC2_NOSNC_HCC;
            }
          } else {
            for (Mc = 0; Mc < MaxIMC; Mc++) {
              if ((MemInfo->imcBitmap[Socket][MC_TECH_DDR] & (BIT0 << Mc)) == 0) {
                AllM2MEnabled = FALSE;
                break;
              }
            }
            if ((!AllM2MEnabled) && (NumOfCluster <= 1)) {
              //
              // Non-SNC Mode && some M2Ms are populated: has different Prefectch Credit
              //
              XptPrefetchCredits = RESERVED_XPTPREFETCH_2LM_ICX_SM2M;
            } else {
              XptPrefetchCredits = RESERVED_XPTPREFETCH_2LM_ICX;
            }
          }
        }
      } else {
        //
        // 1LM Mode
        //
        AepDimmPresent = CheckSocketDcpmmPresent (Socket);

        if (CpuType == CPU_ICXD) {
          if (NumOfCluster == 2) {
            //
            // SNC2 Mode
            //
            XptPrefetchCredits = RESERVED_XPTPREFETCH_SNC_ICXD;
          } else {
            XptPrefetchCredits = RESERVED_XPTPREFETCH_ICXD;
          }
        } else if (CpuType == CPU_SNR || CpuType == CPU_TNR) {
          XptPrefetchCredits = RESERVED_XPTPREFETCH_SNR;
        } else {
          //
          // if 1LM DDR + DDRT on ICX, Transgress gets 0 credit, all credits allocated to Transgress go to XPT Prefetch
          //
          if (AepDimmPresent) {
            if (NumOfCha <= LCC_CHA_MAX_NUMBER) {
              if (NumOfCluster == 2) {
                //
                // SNC2
                //
                XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC2_LCC;
              } else {
                XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC4_NOSNC_LCC;
              }
            } else if (NumOfCha <= HCC_CHA_MAX_NUMBER) {
              if (NumOfCluster == 4) {
                //
                // SNC4
                //
                XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC4_HCC;
              } else {
                XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX_SNC2_NOSNC_HCC;
              }
            } else {
              XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_2LM_ICX;
            }
          } else {
            if (NumOfCha <= LCC_CHA_MAX_NUMBER) {
              if (NumOfCluster == 4) {
                //
                // SNC4
                //
                XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC4_LCC;
              } else {
                XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC2_NOSNC_LCC;
              }
            } else if (NumOfCha <= HCC_CHA_MAX_NUMBER) {
              if ((!KTIVAR->OutSncPrefetchEn) || (NumOfCluster == 1)) {
                XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_NOSNC_HCC;
              } else {
                if (NumOfCluster == 4) {
                  XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC4_HCC;
                } else {
                  XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC2_HCC;
                }
              }
            } else {
              if (NumOfCluster == 4) {
                XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC4;
              } else {
                XptPrefetchCredits = RESERVED_TRANSGRESS_XPTPREFETCH_FLAT_ICX_SNC2_NOSNC;
              }
            }
          } //  if (AepDimmPresent)
        }  // if (CpuSubType == CPU_ICXD)
      }

      //
      // for Mc0 and Mc1
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
  UINT8  SkipRemainderStackId;
  UINT8  NumOfRegister;
  UINT8  CreditsPerCha;
  UINT32 Credit;
  BOOLEAN  AepDimmPresent;

  //
  // If there is a remainder after even distribution of credits among stacks, one stack can waive its claim to a remainder credit.
  // else, one remainder credit (if available) will be assigned to each stack in ascending order.
  //
  SkipRemainderStackId = 0xFF;
  if (KtiInternalGlobal->CpuType == CPU_SNR || KtiInternalGlobal->CpuType == CPU_TNR) {
    //
    // In SNR, M2IOSF connected to Rlink (M2IOSF0) is chosen to waive extra remainder credit.
    //
    SkipRemainderStackId = IIO_STACK0;
  }

  //
  // Program  M2IOSF->CHA credits, row 73
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketPresent (Socket)) {
      //
      // Calculate expected number of PRQ credits per IO stack
      //
      NumOfRegister = KTIVAR->CpuInfo[Socket].TotCha / 8;
      PrqPerIoStack = (TOTAL_PRQ_10nm / KTIVAR->CpuInfo[Socket].TotM2Pcie);
      RemainingPrq = TOTAL_PRQ_10nm % KTIVAR->CpuInfo[Socket].TotM2Pcie;
      AepDimmPresent = CheckSocketDcpmmPresent (Socket);
      //
      // Need to reprogram the PRQ credits for this Socket
      //
      for (M2Pcie = 0; M2Pcie < MAX_IIO_STACK; M2Pcie++) {
        if (KTI_HOST_OUT_PTR->M2PciePresentBitmap[Socket] & (1 << M2Pcie)) {
          if (KTIVAR->CpuInfo[Socket].CpuRes.StackRes[M2Pcie].Personality == TYPE_UBOX_IIO) {
            CreditsPerCha = PrqPerIoStack;
            if ((M2Pcie != SkipRemainderStackId) && (RemainingPrq)) {
              CreditsPerCha++;
              RemainingPrq--;
              //
              // Credit Spreadsheet v6.5
              //
              if (IsCpuAndRevision (CPU_ICXSP, REV_R0)) {
                if ((1 << M2Pcie) == 0x01) {
                  CreditsPerCha--;
                  RemainingPrq++;
                }
              }
            }
            if (AepDimmPresent) {
              CreditsPerCha = CreditsPerCha - DDRT_RESERVED_PRQ_CREDITS;
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
                M2UPcieSetPrqInitCrdIdx (Socket, M2Pcie, RegisterIndex, Credit); //M2PCIE->CHA
              }
            }
            //
            // Program credits for remaining CHAs in the final R2PRQINITCRED register
            //
            Credit = 0;
            for (Cha = 0; Cha < (KTIVAR->CpuInfo[Socket].TotCha % 8); Cha++) {
              Credit = Credit | (CreditsPerCha << (4 * Cha));
            }
            M2UPcieSetPrqInitCrdIdx (Socket, M2Pcie, NumOfRegister, Credit); //remaining M2PCIE-> CHA
          } else if (KTIVAR->CpuInfo[Socket].CpuRes.StackRes[M2Pcie].Personality == TYPE_HFI) {
            //
            // HFI stacks get 1 PRQ credit, remainder will go to FXR MS2IDI
            //
            CreditsPerCha = 1;
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
                M2UPcieSetPrqInitCrdIdx (Socket, M2Pcie, RegisterIndex, Credit); //M2PCIE->CHA
              }
            }
            Credit = 0;
            for (Cha = 0; Cha < (KTIVAR->CpuInfo[Socket].TotCha % 8); Cha++){
              Credit = Credit | (CreditsPerCha << (4 * Cha));
            }
            M2UPcieSetPrqInitCrdIdx (Socket, M2Pcie, NumOfRegister, Credit);//remaining M2PCIE-> CHA
          }
        }
      }
    }
  }

  return KTI_SUCCESS;
}