/** @file
  File includes common functions used to determine clustering mode

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/OemProcMemInitLib.h>
#include <Uefi.h>
#include <Library/PeiServicesLib.h>
#include <KtiMisc.h>
#include <KtiSetupDefinitions.h>
#include <Upi/KtiSi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/IoAccessLib.h>
#include <ScratchpadList.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiSimLib.h>
#include <Library/IoLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemTypeLib.h>
#include <Guid/MemoryMapData.h>
#include <Guid/SocketCommonRcVariable.h>
#include "SncPrefetchLibInternal.h"


const   UINT16 rankSizeDDR4_1[MAX_TECH] = {  64,  32,  16, 128,  64,  32, 256, 128,  64, 512, 256, 128};   // Rank size in 64 MB units

/**
  Read Channel Dimm Information

  This routine read nvram dimm info for this channel

  @param  Socket                  Socket
  @param  Mc                      Memory controller
  @param  Ch                      Channel
  @param  ChannelMemSize          Pointer to ChannelMemSize
  @param  DcpmmPresent          Pointer to DcpmmPresent
  @param  NvDimmPresent           Pointer to NvDimmPresent

  @retval None

**/
VOID
ReadChannelDimmInfo (
  UINT8  Socket,
  UINT8  Mc,
  UINT8  Ch,
  UINT16 *ChannelMemSize,
  UINT8  *DcpmmPresent,
  UINT8  *NvDimmPresent
  )
{
  UINT8 Dimm;
  UINT8 Rank;
  UINT8 TechIndex = 0;
  UINT8 ChannelNumber;
  UINT16 DimmSize, DdrRankSize = 0;
  MEM_NVRAM_STRUCT     *nvramMem;
  DIMM_NVRAM_STRUCT    (*dimmNvList)[MAX_DIMM];
  CHANNEL_NVRAM_STRUCT (*channelNvList)[MAX_CH];

  nvramMem = GetMemNvramPointer ();
  channelNvList = &nvramMem->socket[Socket].channelList;
  ChannelNumber = Ch + Mc * MAX_MC_CH;

  dimmNvList = &nvramMem->socket[Socket].channelList[ChannelNumber].dimmList;
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    DimmSize = 0;
    if ((*dimmNvList)[Dimm].dimmPresent) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Socket %d Mc %d Ch %d maxDimm %d Dimm = %d numDramRanks= %d ",
                            Socket,
                            Mc,
                            Ch,
                            (*channelNvList)[ChannelNumber].maxDimm,
                            Dimm,
                            (*channelNvList)[ChannelNumber].dimmList[Dimm].numDramRanks
                            ));

      *NvDimmPresent = FALSE;
      if (PcdGetBool (PcdNvDimmEn)) {
        if ((*dimmNvList)[Dimm].NvDimmNPresent) {
          *NvDimmPresent = TRUE;
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n NvDimmNPresent= TRUE \n"));
        }
      }
      //get dimm size for this dimm
      if ((*dimmNvList)[Dimm].DcpmmPresent) {
        *DcpmmPresent = TRUE;
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "Pmem TechIndex = "));
        TechIndex = (*dimmNvList)[Dimm].aepTechIndex;
        DdrRankSize = 0;
      }
      if ((*dimmNvList)[Dimm].DcpmmPresent == 0) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "    TechIndex = "));
        TechIndex = (*dimmNvList)[Dimm].techIndex;
        DdrRankSize = rankSizeDDR4_1[TechIndex];
      }
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "%d", TechIndex));

      for (Rank = 0; Rank < (*dimmNvList)[Dimm].numDramRanks; Rank++) {
        DimmSize += DdrRankSize;
      }  // Rank Loop

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " DimmSize 0x%x", DimmSize));
      // update mem size for this channel
      *ChannelMemSize += DimmSize;
    }
  }  // dimm loop
}

/**
  Get memory channel Information.

  This routine is to scan memory channel information and update into MemInfo

  @param  MemInfo            -   Memory Information structure.
  @param  Socket             -   Socket

  @retval None

**/

VOID
GetMemoryChannelInfo (
  MEM_INFO                   *MemInfo,
  UINT8                      Socket
  )
{
  UINT8  Mc, Ch, ChannelNumber;
  UINT32 McDdr4MemSize, ChannelDdr4MemSize;
  UINT32 McAepMemSize, ChannelAepMemSize;
  DDR_CHANNEL_STRUCT (*channelList)[MAX_CH];
  UINT8  MaxIMC;

  MaxIMC   = GetMaxImc ();
  //
  // The following code to scan each channel is for debug purpose
  //
  channelList = &(GetMemVarPointer ()->socket[Socket].channelList);

  for (Mc = 0; Mc < MaxIMC; Mc++) {
    McDdr4MemSize = McAepMemSize = 0;
    for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
      // Detects total memory installed on this channel
      ChannelDdr4MemSize = ChannelAepMemSize = 0;
      ChannelNumber = Ch + Mc * MAX_MC_CH;

      ChannelDdr4MemSize = GetChannelMemSize (Socket, ChannelNumber);     //get ddr4 value
      ChannelAepMemSize = (*channelList)[ChannelNumber].volSize + (*channelList)[ChannelNumber].perSize;     //get 2LM size and per size

      MemInfo->McInfo[Socket][Mc].ChannelMemSize[Ch] = ChannelDdr4MemSize;
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  channel %d DDR4 Size = %dG volSize = %dG perSize = %dG", ChannelNumber, ChannelDdr4MemSize >>4,
                             (*channelList)[ChannelNumber].volSize >> 4,
                             (*channelList)[ChannelNumber].perSize >> 4));

      if (ChannelDdr4MemSize) {
        McDdr4MemSize +=  ChannelDdr4MemSize >> 4;   //in GB
      }

      if (ChannelAepMemSize) {
        McAepMemSize +=  ChannelAepMemSize >> 4;     //in GB
      }
    }  // Ch loop

    MemInfo->McInfo[Socket][Mc].McMemSize = ((McAepMemSize !=0 ) ? McAepMemSize: McDdr4MemSize);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Mc %d MemorySize = %dG", Mc, MemInfo->McInfo[Socket][Mc].McMemSize));
  }  // Mc loop
}

/**
  Set the SNC cluster for DDR memory

  @param  KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket            - Socket ID
  @param  MemInfo           - Memory Information structure.

  @retval KTI_FAILURE       - MC doens't have same memory population
  @retval KTI_SUCCESS       - Success to return number of Cluster

**/
KTI_STATUS
SetSncCluster (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket,
  MEM_INFO                   *MemInfo
  )
{
  UINT8   Ch;
  UINT8   UmaClustering;
  UINT8   NumOfCluster;
  UINT8   NumOfHbmioEn;
  UINT8   NumOfDDRMcEn;
  UINT8   MaxImc;

  UmaClustering = KTIVAR->OutUmaClustering;
  NumOfCluster  = 1;
  MaxImc        = GetMaxImc ();
  NumOfDDRMcEn  = MemInfo->SncInfo[Socket].NumOfMcEnabled;

  //
  // initialize safe defaults in case clusters are not supported
  //
  MemInfo->SncInfo[Socket].NumOfChaPerCluster = KTIVAR->CpuInfo[Socket].TotCha;
  MemInfo->SncInfo[Socket].NumOfCluster = 1;

  if (MaxImc > 2) {
    if ((KTIVAR->OutSncEn == 4) || (KTIVAR->OutSncEn == 2)) {
      NumOfCluster = KTIVAR->OutSncEn;
    } else if (UmaClustering == UMABASEDCLUSTERING_HEMISPHERE || UmaClustering == UMABASEDCLUSTERING_QUADRANT) {
      NumOfCluster = UmaClustering;
    } else {
      for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
        if (MemInfo->McInfo[Socket][0].ChannelMemSize[Ch] != MemInfo->McInfo[Socket][1].ChannelMemSize[Ch]) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Interleave Enable -> Socket %d channel %d has different size between MC", Socket, Ch));
        }
      }
      NumOfCluster = 1;
    }

    //
    // SNC2/Hemi (2-clusters): Need 2 or 4 MCs are populated. Each side should have at least 1 Mc
    // SNC4/Quad (4-clusters): Need 4 MCs are populated, Quad mode and 2MC, need to downgrade to Hemi mode
    //
    if (KTIVAR->HbmSku && (NumOfDDRMcEn == 0)) {
      // HBM only
      NumOfHbmioEn = MemInfo->SncInfo[Socket].NumOfHbmioEnabled;
      if (NumOfCluster == 2) {
        if (NumOfHbmioEn == 1) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n WARN: Cannot enable SNC2/Hemi - SNC2/Hemi cannot be enabled by Single HBMIO!!!\n"));
          return KTI_FAILURE;
        }
        if (((MemInfo->HbmInfo[Socket][0].Enabled == 0) && (MemInfo->HbmInfo[Socket][1].Enabled == 0)) || ((MemInfo->HbmInfo[Socket][2].Enabled == 0) && (MemInfo->HbmInfo[Socket][3].Enabled == 0))) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n WARN: Cannot enable SNC2/Hemi - SNC2/Hemi cannot be enabled by enabling HBMIO in one side.!!!\n"));
          return KTI_FAILURE;
        }
      } else if (NumOfCluster == 4) {
        if ((UmaClustering == UMABASEDCLUSTERING_QUADRANT) && (NumOfHbmioEn == 2)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n WARN: Quad Mode but 2 HBMIO enabled, downgrade to Hemi mode!!!\n"));
          KTIVAR->OutUmaClustering = UMABASEDCLUSTERING_HEMISPHERE;
          NumOfCluster  = UMABASEDCLUSTERING_HEMISPHERE;
        } else if (NumOfHbmioEn < 4) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n WARN: Cannot enable SNC4/Quad - SNC4/Quad cannot be enabled without HBMIO enabled!!!\n"));
          return KTI_FAILURE;
        }
      }
    } else {
      if (NumOfCluster == 2) {
        if (NumOfDDRMcEn == 1) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n WARN: Cannot enable SNC2/Hemi - SNC2/Hemi cannot be enabled by Single MC!!!\n"));
          return KTI_FAILURE;
        }
        if (((MemInfo->McInfo[Socket][0].Enabled == 0) && (MemInfo->McInfo[Socket][1].Enabled == 0)) || ((MemInfo->McInfo[Socket][2].Enabled == 0) && (MemInfo->McInfo[Socket][3].Enabled == 0))) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n WARN: Cannot enable SNC2/Hemi - SNC2/Hemi cannot be enabled by enabling MC in one side.!!!\n"));
          return KTI_FAILURE;
        }
      } else if (NumOfCluster == 4) {
        if ((UmaClustering == UMABASEDCLUSTERING_QUADRANT) && (NumOfDDRMcEn == 2)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n WARN: Quad Mode but 2MC enabled, downgrade to Hemi mode!!!\n"));
          KTIVAR->OutUmaClustering = UMABASEDCLUSTERING_HEMISPHERE;
          NumOfCluster  = UMABASEDCLUSTERING_HEMISPHERE;
        } else if (NumOfDDRMcEn < 4) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n WARN: Cannot enable SNC4/Quad - SNC4/Quad cannot be enabled without 4MCs is enabled!!!\n"));
          return KTI_FAILURE;
        }
      }
    }
  } else if (MaxImc == 2) {
    //
    // For 2MC SKU
    //
    if (NumOfDDRMcEn == 2) {
      if (KTIVAR->OutSncEn == 2 || UmaClustering == UMABASEDCLUSTERING_HEMISPHERE) {
        //
        // 2-clusters DDR4 only
        //
        NumOfCluster = 2;
      } else {
        //
        // 2MCs enabled but SNC disabled, verify the ch symmetric
        //
        for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
          if (MemInfo->McInfo[Socket][0].ChannelMemSize[Ch] != MemInfo->McInfo[Socket][1].ChannelMemSize[Ch]) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Interleave Enable -> Socket %d channel %d has different size between MC", Socket, Ch));
            return KTI_FAILURE;
          }
        }
        NumOfCluster = 1;
      }
    } else if (NumOfDDRMcEn == 1) {
      //
      // 1-cluster
      //
      NumOfCluster = 1;
    }
  } else {
    //
    // SNR, ICXD and SPR-1Die always a 1-cluster, NumOfCluster = 1
    //
    NumOfCluster = 1;
  }
  //
  // update NumOfCluster and NumOfChaPerCluster based on discovered memory
  //
  MemInfo->SncInfo[Socket].NumOfChaPerCluster = (KTIVAR->CpuInfo[Socket].TotCha / NumOfCluster);
  MemInfo->SncInfo[Socket].NumOfCluster = NumOfCluster;

  return KTI_SUCCESS;
}

/**
  Verify the number of HBM MC in each socket

  @param  KtiInternalGlobal   -   KTIRC Internal Global data
  @param  MemInfo             -   Memory Information structure.

  @retval KTI_FAILURE         -   Sockets don't have the same HBM population
  @retval KTI_SUCCESS         -   Sockets have the same HBM population

**/
KTI_STATUS
VerifyNumberofHbm (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  )
{
  UINT8   NumOfHbmEn, Soc;

  NumOfHbmEn = 0xFF;
  if (!KTIVAR->HbmSku) {
    return KTI_SUCCESS;
  }
  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    if (SocketPresent (Soc)) {
      if (NumOfHbmEn == 0xFF){            // 1st time to update this variable
         NumOfHbmEn = MemInfo->SncInfo[Soc].NumOfHbmioEnabled;
      } else if (NumOfHbmEn != MemInfo->SncInfo[Soc].NumOfHbmioEnabled) {
         KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n HBM Mc are not identical on existing sockets. Disable SNC!!"));
         return KTI_FAILURE;
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Verify the number of HBM MC in each socket

  @param  KtiInternalGlobal   -   KTIRC Internal Global data
  @param  MemInfo             -   Memory Information structure.

  @retval KTI_FAILURE         -   Sockets don't have the same HBM population
  @retval KTI_SUCCESS         -   Sockets have the same HBM population

**/
KTI_STATUS
VerifySameNumberOfEnabledMcAcrossSkts (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  )
{
  UINT8         TempNumOfMcEn;
  UINT8         Soc;
  UINT8         CpuType;

  TempNumOfMcEn = 0xFF;
  CpuType     = KtiInternalGlobal->CpuType;

  if (IsSkxFamilyCpu (CpuType) || IsIcxFamilyCpu (CpuType)) {
    for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
      if (!SocketPresent (Soc) || (KTIVAR->OutKtiFpgaPresent[Soc] == 1)) {
        continue;
      }

      if (TempNumOfMcEn == 0xFF) {            // 1st time to update this variable
        TempNumOfMcEn = MemInfo->SncInfo[Soc].NumOfMcEnabled;
      } else if (TempNumOfMcEn != MemInfo->SncInfo[Soc].NumOfMcEnabled) {  //if any sockets have different number of Mc then exit to disable SNC
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nDifferent Mc numbers on existing sockets. Disable SNC!!"));
        return KTI_FAILURE;
      }
    }
  }
  if (TempNumOfMcEn == 0) {
    if (!KTIVAR->HbmSku) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nNo DDR memory populated in any socket. Disable SNC and prefetch!!"));
      KTIVAR->OutSncPrefetchEn = 0;
      return KTI_FAILURE;
    }
  }

  return KTI_SUCCESS;
}

/**
  Determine SNC setting

  This routine determiness SNC CHA slices for each cluster and number of clusters within this socket.
  This setting is based on mc0/1 population and NUMA non-interleave cross mc or interleave cross mc

  @param  KtiInternalGlobal - KTIRC Internal Global data
  @param  MemInfo           - Memory Information structure.

  @retval KTI_SUCCESS - on successful

**/
KTI_STATUS
DetermineSncSetting (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  MEM_INFO                   *MemInfo
  )
{
  UINT8         TempNumOfMcEn, Ch, Soc;
  KTI_STATUS    Status;
  UINT8         CpuType;

  Ch = 0;
  TempNumOfMcEn = 0xFF;

  if ((Status = VerifySameNumberOfEnabledMcAcrossSkts (KtiInternalGlobal, MemInfo)) != KTI_SUCCESS) {
    return Status;
  }

  CpuType     = KtiInternalGlobal->CpuType;
  //
  // if both Mc have memory on all sockets, set to 2 clusters
  //
  for (Soc = 0; Soc < MAX_SOCKET; Soc++) {
    if (!SocketPresent (Soc) || (KTIVAR->OutKtiFpgaPresent[Soc] == 1)) {
      continue;
    }

    TempNumOfMcEn = MemInfo->SncInfo[Soc].NumOfMcEnabled;
    if (IsSkxFamilyCpu (CpuType)) {
      if (TempNumOfMcEn == 2) {
        if (KTIVAR->OutSncEn != KTI_DISABLE) {       // NUMA non-interleave, set SNC enabled, 2 clusters
          MemInfo->SncInfo[Soc].NumOfChaPerCluster = (KTIVAR->CpuInfo[Soc].TotCha)/2;
          MemInfo->SncInfo[Soc].NumOfCluster = 2;
        } else {                                         //NUMA & interleave , then set SNC enabled, 1 cluster
          for (Ch = 0; Ch < MAX_MC_CH; Ch++) {           //scan all channels, to validate channel map and size are equal on both Mc
            if (MemInfo->McInfo[Soc][0].ChannelMemSize[Ch] != MemInfo->McInfo[Soc][1].ChannelMemSize[Ch]) {
              KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Interleave Enable -> Socket %d channel %d has different size between MC", Soc, Ch));
              return KTI_FAILURE;
            }
          }
          MemInfo->SncInfo[Soc].NumOfChaPerCluster = (KTIVAR->CpuInfo[Soc].TotCha);
          MemInfo->SncInfo[Soc].NumOfCluster = 1;
        }
      //
      // if either Mc0 or Mc1 has memory only on all sockets, set to 1 cluster
      //
      } else if (TempNumOfMcEn == 1) {
        MemInfo->SncInfo[Soc].NumOfChaPerCluster = (KTIVAR->CpuInfo[Soc].TotCha);
        MemInfo->SncInfo[Soc].NumOfCluster = 1;
      }

      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Snc Enable = %d, Num of Cha Per Cluster = %d, Num of Cluster = %d",
                           Soc, KTIVAR->OutSncEn, MemInfo->SncInfo[Soc].NumOfChaPerCluster, MemInfo->SncInfo[Soc].NumOfCluster));

      if (MemInfo->SncInfo[Soc].NumOfChaPerCluster == 0) {
        return KTI_FAILURE;
      }

      //
      // 1-cluster : Enable SNC_IND if any Prefetch is enabled
      // 2-clusters: Enable SNC_FULL by the SNC_EN knob is enabled
      //
      if (MemInfo->SncInfo[Soc].NumOfCluster == 1) {
        if (((KTIVAR->OutKtiPrefetch == KTI_DISABLE) && (KTIVAR->OutXptPrefetch == KTI_DISABLE)) || (KTIVAR->OutSncEn == KTI_ENABLE)) {
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nWARN: Cannot enable SNC_IND - all prefetch knobs are disabled!!!\n"));
          return KTI_FAILURE;
        }
      } else if ((MemInfo->SncInfo[Soc].NumOfCluster == 2) || (MemInfo->SncInfo[Soc].NumOfCluster == 4)) {   //if 2 or 4 clusters can be setup, but setup SNC disabled or not capable to set (e.g. 1.75G mmcfg)
        if (KTIVAR->OutSncEn == KTI_DISABLE) {
          return KTI_FAILURE;
        }
      }
    } else {
      //
      // ICX support SNC1/SNC2/SNC4/Hemisphere
      //
      if (TempNumOfMcEn) {
        Status = SetSncCluster (KtiInternalGlobal, Soc, MemInfo);
        if (Status != KTI_SUCCESS) {
          return KTI_FAILURE;
        }
      }
    }
  }  // scan socket

  return KTI_SUCCESS;
}