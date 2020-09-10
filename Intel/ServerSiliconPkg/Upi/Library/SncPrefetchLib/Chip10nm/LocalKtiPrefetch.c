/** @file
  File includes routines to configure Xpt local prefetch, KTI prefetch.

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
#include <Library/MemMcIpLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/IoAccessLib.h>
#include <Library/MultiSocketLib.h>
#include <ScratchpadList.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiSimLib.h>
#include <Library/IoLib.h>
#include <RcRegs.h>
#include <Library/ChaLib.h>
#include <Guid/UboxIpInterface.h>
#include <Library/Ms2IdiLib.h>
#include <Library/OobIpLib.h>
#include <Library/PcuIpLib.h>
#include <Library/M2IalIpLib.h>
#include <Guid/KtiIpInterface.h>
#include <Guid/IioIpInterface.h>
#include <Guid/MsmIpInterface.h>
#include <Library/CsrAccessLib.h>
#include <Library/PmcSetStrapMessageLib.h>
#include <Library/MemTypeLib.h>
#include <Guid/MemoryMapData.h>
#include <Library/CxlLib.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Library/SncPrefetchLib.h>
#include "SncPrefetchLibInternal.h"


/**
  Disable XPT Prefetech

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket           - Socket Id

  @retval None

**/
VOID
DisableXptPrefetch (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Socket
  )
{
  UINT8 Ctr;

  //MS2IDI
  Ms2IdiSetLocXptPrefDis (Socket, 1);

  // M2M
  for (Ctr = 0; Ctr < KTIVAR->CpuInfo[Socket].TotM2m; Ctr++) {
    SetM2mSysFeatures0PreftchDis (Socket, Ctr, TRUE);
  }

  KTIVAR->OutXptPrefetch = KTI_DISABLE;

  return;
}

/**
  Updae miniSAD informtion for memory controller

  This routine is to enable xpt prefetch feature

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket           - Socket
  @param  MemInfo          - Memory Information structure
  @param  MiniSadInfo      - MiniSad Information structure

  @retval None

**/
VOID
UpdateMiniSadInForMc (
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  UINT8                    Socket,
  MEM_INFO                 *MemInfo,
  MINISAD_INFO             *MiniSadInfo
  )
{
  UINT8            Ctr;
  UINT8            Mc0, Mc1, Mc2, Mc3, OneMc;
  UINT8            LeftMc,RightMc;

  Mc0 = Mc1 = Mc2 = Mc3 = 0;
  LeftMc = RightMc = 0;
  OneMc = 0;

  //
  // XPT miniSad Modeselect should come from MRC: Xptminisadmodeselect
  //

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Socket %d: Xptminisadmodeselect = %x \n", Socket, MiniSadInfo->Xptminisadmodeselect));

  //
  // Disable and not use xpt 2 entry mini Sad Table
  //
  MiniSadInfo->MicMode = 0;                      //KNH sets this bit to 1. All other SOC's set this bit to 0

  MiniSadInfo->XptMcMiniSadMode = 0;             //Disable and not use xpt 2 entry mini Sad Table

  //
  //Update XPT 32 MiniSAD entries and UPI MiniSAD table
  //
  if (GetMaxImc () > 2) {
    //
    //                        SNC4 w/ 4MCs  |     SNC2 w/ 2MCs               |SNC2+4MCs      |              1-cluster
    //                       MC0+MC1+MC2+MC3| MC0+MC2 MC0+MC3 MC1+MC2 MC2+MC3|MC0+MC1+MC2+Mc3|  MC0|MC1 MC2 MC3 MC0+MC1 MC0+MC2 MC0+MC3 MC1+MC2 MC1+MC3 MC0+MC1+MC2+Mc3
    // XPT_32_Entry MiniSAD:
    //              Entry0        0              0       0       1       2           0          0    1   2   3     0       0       0       1       1        0
    //              Entry1        0              0       0       1       2           1          0    1   2   3     1       2       3       2       3        1
    //              Entry2        0              0       0       1       2           0          0    1   2   3     0       0       0       1       1        2
    //              Entry3        0              0       0       1       2           1          0    1   2   3     1       2       3       2       3        3
    //              Entry4        0              0       0       1       2           0          0    1   2   3     0       0       0       1       1        0
    //              Entry5        0              0       0       1       2           1          0    1   2   3     1       2       3       2       3        1
    //              Entry6        0              0       0       1       2           0          0    1   2   3     0       0       0       1       1        2
    //              Entry7        0              0       0       1       2           1          0    1   2   3     1       2       3       2       3        3
    //              Entry8        1              2       3       2       3           2
    //              Entry9        1              2       3       2       3           3
    //              Entry10       1              2       3       2       3           2
    //              Entry11       1              2       3       2       3           3
    //              Entry12       1              2       3       2       3           2
    //              Entry13       1              2       3       2       3           3
    //              Entry14       1              2       3       2       3           2
    //              Entry15       1              2       3       2       3           3
    //              Entry16       2
    //              Entry17       2
    //              Entry18       2
    //              Entry19       2
    //              Entry20       2
    //              Entry21       2
    //              Entry22       2
    //              Entry23       2
    //              Entry24       3
    //              Entry25       3
    //              Entry26       3
    //              Entry27       3
    //              Entry28       3
    //              Entry29       3
    //              Entry30       3
    //              Entry31       3
    //
    //
    // Based on 4MC_8Ch_SAD, there is no 256BXOR mode right now, will check memory map team
    // Mode 0: PA[6]
    // Mode 1: PA[8]
    // Mode 2: PA[12]
    // Mode 3: 256B XOR A[8] or A[9:8]
    // Mode 4: 256B mode3
    // Mode 5: 256B (3-way) A[51:8]%3
    // Mode 6: 256B (3-Way) A[51:8]%3
    // Mode 7: 2MCx on both sides  for PA[6] or PA[8] or
    //

    if (MemInfo->MiniSadInfo.IntGranularity == 6) {             // 2LM case
      MemInfo->MiniSadInfo.UpiMiniSADMode[Socket] = 2;
    } else if ((GetSocketXorMode (Socket) & CHANNEL_XOR) == 0) {
      MemInfo->MiniSadInfo.UpiMiniSADMode[Socket] = 6;
    } else {
      MemInfo->MiniSadInfo.UpiMiniSADMode[Socket] = 3;
    }

    // Check left side MCs population status(Mc0 & Mc1)
    //
    if ((MemInfo->imcBitmap[Socket][MC_TECH_DDR] & (KTI_MC0_BITMAP | KTI_MC1_BITMAP)) == (KTI_MC0_BITMAP | KTI_MC1_BITMAP)) {
      LeftMc = 0xFF;
    } else if ((MemInfo->imcBitmap[Socket][MC_TECH_DDR] & KTI_MC0_BITMAP) == KTI_MC0_BITMAP) {
      LeftMc = KTI_MC0_BITMAP;
    } else if ((MemInfo->imcBitmap[Socket][MC_TECH_DDR] & KTI_MC1_BITMAP) == KTI_MC1_BITMAP) {
      LeftMc = KTI_MC1_BITMAP;
    }

    //
    // Check Right side MCs population status(Mc2 & Mc3)
    //
    if ((MemInfo->imcBitmap[Socket][MC_TECH_DDR] & (KTI_MC2_BITMAP | KTI_MC3_BITMAP)) == (KTI_MC2_BITMAP | KTI_MC3_BITMAP)) {
      RightMc = 0xFF;
    } else if ((MemInfo->imcBitmap[Socket][MC_TECH_DDR] &  KTI_MC2_BITMAP) == KTI_MC2_BITMAP) {
      RightMc = KTI_MC2_BITMAP;
    } else if ((MemInfo->imcBitmap[Socket][MC_TECH_DDR] &  KTI_MC3_BITMAP) == KTI_MC3_BITMAP) {
      RightMc = KTI_MC3_BITMAP;
    }

    MemInfo->SncInfo[Socket].LeftMc = LeftMc;
    MemInfo->SncInfo[Socket].RightMc = RightMc;

    //
    // Update XPT_32_Entry_MiniSAD and UPI Prefetch MiniSAD tables
    //
    if (MemInfo->SncInfo[Socket].NumOfCluster == 4) {
      //
      //SNC4 w/ 4MCs
      //
      for (Ctr = 0; Ctr < MAX_MINISAD_32_ENTRY; Ctr++) {
        MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | ((Ctr & 0xF8) >> 3);
      }

      MemInfo->MiniSadInfo.UPIMiniSad[Socket] = 0x33221100;
      MemInfo->MiniSadInfo.UpiMiniSADMode[Socket] = 7;
    } else if ((MemInfo->SncInfo[Socket].NumOfCluster == 2)) {
      //
      // SNC2 w/ 2MCs
      //
      if (MemInfo->SncInfo[Socket].NumOfMcEnabled < 4) {
        if (RightMc  == KTI_MC2_BITMAP) {
          RightMc = KTI_MC2;
        } else {
          RightMc = KTI_MC3;
        }
        if (LeftMc  == KTI_MC0_BITMAP) {
          LeftMc = KTI_MC0;
        } else {
          LeftMc = KTI_MC1;
        }

        for (Ctr = 0; Ctr < MAX_MINISAD_32_ENTRY; Ctr++) {
          if ((Ctr & 0x08) == 0) {
            MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | LeftMc;
          } else {
            MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | RightMc;
          }
        }

        for (Ctr = 0; Ctr < 8; Ctr++) {
          MemInfo->MiniSadInfo.UPIMiniSad[Socket] |= ((Ctr & 0x01) ? RightMc : LeftMc) << (Ctr * 4);
        }
      } else {
        //
        // SNC2 w/ 4MCs
        //
        for (Ctr = 0; Ctr < MAX_MINISAD_32_ENTRY; Ctr++) {
          if ((Ctr & 0x18) == 0) {
            MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | (Ctr & 0x01 );
          } else {
            MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | ((Ctr & 0x01) | 0x02);
          }
        }
        MemInfo->MiniSadInfo.UPIMiniSad[Socket] = 0x32321010;
      }
    } else {
      if (MemInfo->SncInfo[Socket].NumOfMcEnabled == 4) {
        //
        // 1-cluster w/ 4MCs
        //
        for (Ctr = 0; Ctr < MAX_MINISAD_32_ENTRY; Ctr++) {
          MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | ((Ctr & 03));
        }

        if (MemInfo->MiniSadInfo.UpiMiniSADMode[Socket] != 6) {
          MemInfo->MiniSadInfo.UPIMiniSad[Socket] = 0x32103210;
        } else {
          //
          // Mode-6: MC0, MC0, MC1, x, MC2, MC2, MC3, x
          //
          MemInfo->MiniSadInfo.UPIMiniSad[Socket] = 0x33221100;
        }
      } else if (MemInfo->SncInfo[Socket].NumOfMcEnabled == 2) {
        //
        // 1 cluster w/ 2MCs
        //
        if ((LeftMc != 0xFF) && (RightMc != 0xFF)) {
          //
          // 1-cluster, 2MCs on both sides (Mc0+Mc2 or Mc0+Mc3 or Mc1+Mc2 or Mc1+Mc3)
          //
          if (RightMc  == KTI_MC2_BITMAP) {
            RightMc = KTI_MC2;
          } else {
            RightMc = KTI_MC3;
          }
          if (LeftMc  == KTI_MC0_BITMAP) {
            LeftMc = KTI_MC0;
          } else {
            LeftMc = KTI_MC1;
          }
          for (Ctr = 0; Ctr < MAX_MINISAD_32_ENTRY; Ctr++) {
            MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | ((Ctr & 0x01) ? RightMc : LeftMc);
          }

          for (Ctr = 0; Ctr < 8; Ctr++) {
            MemInfo->MiniSadInfo.UPIMiniSad[Socket] |= ((Ctr & 0x01) ? RightMc : LeftMc) << (Ctr * 4);
          }
        } else if ((LeftMc != 0xFF) && (RightMc == 0xFF)) {
          //
          // 1-cluster, 2MCs on the right ides (Mc2+Mc3)
          //
          for (Ctr = 0; Ctr < MAX_MINISAD_32_ENTRY; Ctr++) {
            MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | ((Ctr & 0x01) ? KTI_MC3 : KTI_MC2);
          }

          for (Ctr = 0; Ctr < 8; Ctr++) {
            MemInfo->MiniSadInfo.UPIMiniSad[Socket] |= ((Ctr & 0x01) ? KTI_MC3 : KTI_MC2) << (Ctr * 4);
          }
        } else {
          //
          // 1-luster, 2MCs on the left ides (Mc0+Mc1)
          //
          for (Ctr = 0; Ctr < MAX_MINISAD_32_ENTRY; Ctr++) {
            MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | ((Ctr & 0x01) ? KTI_MC1 : KTI_MC0);
          }

          for (Ctr = 0; Ctr < 8; Ctr++) {
            MemInfo->MiniSadInfo.UPIMiniSad[Socket] |= ((Ctr & 0x01) ? KTI_MC1 : KTI_MC0) << (Ctr * 4);
          }
        }
      } else if (MemInfo->SncInfo[Socket].NumOfMcEnabled ==1) {
        //
        // 1-cluster, 1MC
        //
        OneMc = (UINT8)MemInfo->imcBitmap[Socket][MC_TECH_DDR];
        for (Ctr = 0; Ctr < GetMaxImc (); Ctr++) {
          if ((OneMc & 1) != 0) {
            break;
          }
          OneMc = OneMc >> 1;
        }
        OneMc = Ctr;

        for (Ctr = 0; Ctr < MAX_MINISAD_32_ENTRY; Ctr++) {
          MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | OneMc;
        }

        for (Ctr = 0; Ctr < 8; Ctr++) {
          MemInfo->MiniSadInfo.UPIMiniSad[Socket] |= OneMc << (Ctr * 4);
        }
      }
    }
  } else {
    //
    // 2MC SKU
    //
    if (MemInfo->SncInfo[Socket].InterleaveEn == 0) {
      //
      // NUMA non-interleave (1 way interleave)
      //

      //
      //check if MC0 or MC1 only
      //
      if (((MemInfo->McInfo[Socket][0].Enabled == 0) && (MemInfo->McInfo[Socket][1].Enabled == 1)) ||
          ((MemInfo->McInfo[Socket][0].Enabled == 1) && (MemInfo->McInfo[Socket][1].Enabled == 0))) {
        //
        // If only MC1 present: Entry 0-7 -> EnGenerate_Prefetch with MC1, Entry 16-23 -> EnGenerate_Prefetch with MC1, else
        // If only MC0 present: Entry 0-7 -> EnGenerate_Prefetch with MC0, Entry 16-23 -> EnGenerate_Prefetch with MC0
        //
        OneMc = 1;
        if (MemInfo->McInfo[Socket][0].Enabled == 1) {
          OneMc = 0;
        }
        for (Ctr = 0; Ctr < 8; Ctr++) {
          MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | OneMc;
          MiniSadInfo->MiniSadTable32Entry[Socket][Ctr + 16] = MiniSadInfo->MiniSadTable32Entry[Socket][Ctr];
        }
        if (IsSiliconWorkaroundEnabled ("S2204036982")) {
          for (Ctr = 0; Ctr < 8; Ctr++) {
            //
            // Populate all 32 entries (especially Cluster 3 entries) to point to MC0
            //
            MiniSadInfo->MiniSadTable32Entry[Socket][Ctr + 8] = MiniSadInfo->MiniSadTable32Entry[Socket][Ctr];
            MiniSadInfo->MiniSadTable32Entry[Socket][Ctr + 24] = MiniSadInfo->MiniSadTable32Entry[Socket][Ctr];
          }
        }
      } else {
        //
        //Entry   0-7 -> EnGenerate_Prefetch with MC0   Entry  8-15 -> EnGenerate_Prefetch with MC1
        //Entry 16-23 -> EnGenerate_Prefetch with MC0   Entry 24-31 -> EnGenerate_Prefetch with MC1
        //
        for (Ctr = 0; Ctr < MAX_MINISAD_32_ENTRY; Ctr++) {
          MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | ((Ctr & 0x08) >> 3);
        }
      }
    } else {
      //
      // NUMA interleave (2 way interleave)
      //
      for (Ctr = 0; Ctr < MAX_MINISAD_32_ENTRY; Ctr++) {
        if (Ctr & 0x1) {
          //
          //odd Entry  > EnGenerate_Prefetch with MC1
          //
           MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE | 0x1;
        } else {
          //
          //even Entry > EnGenerate_Prefetch with MC0
          //
          MiniSadInfo->MiniSadTable32Entry[Socket][Ctr] = MINISAD_ENTRY_ENABLE;
        }
      }
    }
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n Socket%d: UpiMiniSADMode = %x\n", Socket, MemInfo->MiniSadInfo.UpiMiniSADMode[Socket]));

  //
  // Update XPT Prefetch Base
  //
  for (Ctr = 0; Ctr < NUM_OF_REGSTERS_32_ENTRY_PREFETCH_BASE; Ctr++) {
    if (KTIVAR->OutUmaClustering) {
      // 1) UMA based clustering case.
      // XPT_32_ENTRY_PREFETCH_BASE_0/1/2/3 all have same value, = prefetch_range_base of cluster0
      MiniSadInfo->Xpt32EntryPrefetchBase[Socket][Ctr] = MemInfo->McInfo[Socket][0].PrefetchRange[0].Base >> 4;
    } else {
      // 2) All2All, prefetch enabled
      //   XPT_32_ENTRY_PREFETCH_BASE_0 = prefetch_range_base of cluster0
      //   XPT_32_ENTRY_PREFETCH_BASE_1/2/3 = prefetch_range_limit of cluster0
      // 3) SNC2 or SNC4
      if (MemInfo->McInfo[Socket][Ctr].PrefetchRange[0].Enabled) {
        MiniSadInfo->Xpt32EntryPrefetchBase[Socket][Ctr] = (MemInfo->McInfo[Socket][Ctr].PrefetchRange[0].Base >> 4);
      } else {
        MiniSadInfo->Xpt32EntryPrefetchBase[Socket][Ctr] = MemInfo->ClusterMemBase[Socket][Ctr];
      }
    }
    if ((KTIVAR->OutSncEn == 0) && (KtiInternalGlobal->CpuType == CPU_SNR || KtiInternalGlobal->CpuType == CPU_TNR)) {
      // For SNR, all2all, need to program XPT_32_ENTRY_PREFETCH_BASE_0/1/2/3
      // to 0
      MiniSadInfo->Xpt32EntryPrefetchBase[Socket][Ctr] = 0;
    }
  }
}

/**
  Check whether XptKti Prefetch need to be disable or not.

  This routine is to check whether XPT/KTI prefetch need to be disable or not

  @param  KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket            - Socket Id
  @param  MemInfo           - Memory Information structure.

  @retval TRUE/FALSE

**/
BOOLEAN
CheckXptKtiPrefetchNeedToDisable (
  IN OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                          Socket,
  IN MEM_INFO                       *MemInfo
  )
{
  UINT8   RasMode;
  RasMode = GetRasMode ();

  //
  // Check xpt prefetch if need to be disabled
  //
  if (((KTIVAR->OutXptPrefetch == KTI_DISABLE) && (KTIVAR->OutKtiPrefetch == KTI_DISABLE) && (KTIVAR->OutXptRemotePrefetch == KTI_DISABLE)) ||
      (MemInfo->SncInfo[Socket].PrefetchEn == 0) || (MemInfo->PrefetchAll == 0)) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d XPT and KTI prefetch Disabled\n", Socket));
    return TRUE;
  }

  if (RasMode & FULL_MIRROR_1LM || RasMode & FULL_MIRROR_2LM || RasMode & PARTIAL_MIRROR_1LM || RasMode & PARTIAL_MIRROR_2LM) {
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  KTI prefetch Disabled - cannot enable KTI prefetch and Mirror together\n"));
    return TRUE;
  }

  if (GetSocketMemSize (Socket) == 0) {
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING: Socket %d has no memory - disable prefetch\n", Socket));
    return TRUE;
  }

  return FALSE;
}

/**
  Enable XPT/KTI/Remote prefetch features.

  This routine is to enable all prefetch features

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket           - Socket Id
  @param  MemInfo          - Memory Information structure.

  @retval None

**/
VOID
ProgramPrefetchEntry (
  IN OUT KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                          Socket,
  IN MEM_INFO                       *MemInfo
  )
{
  UINT32  XptMemInterleave;
  UINT8   Ctr, RegIndex, RegStartBit;
  UINT8   MiniSadModeSelect;
  UINT8   McMiniSadMode;
  UINT8   XptFifoEnabledCredit;
  UINT8   RemoteSktId;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d XPT or KTI Enabled(Interleave En = %d)\n",
                       Socket, MemInfo->SncInfo[Socket].InterleaveEn));

  //
  // Update 32 mini sad entries and UPI Prefetch MiniSAD
  //
  if (!KtiInternalGlobal->UpiMinisadCfgDone[Socket]) {
    UpdateMiniSadInForMc (KtiInternalGlobal, Socket, MemInfo, &MemInfo->MiniSadInfo);
  }

  //
  //Convert 32 Entry value To Register Format
  //Indentify which register will be for this entry
  //Indentify which start bit on this register
  //
  for (Ctr = 0; Ctr < MAX_MINISAD_32_ENTRY; Ctr++) {
    RegIndex = Ctr / 6;
    RegStartBit = 5 * (Ctr % 6);
    MemInfo->MiniSadInfo.MiniSadTable32EntryRegValue[Socket][RegIndex] |= MemInfo->MiniSadInfo.MiniSadTable32Entry[Socket][Ctr] << RegStartBit;
  }

  //
  // Program 32 minisad entry registers
  //
  for (Ctr = 0; Ctr < NUM_OF_REGSTERS_MINISAD_32_ENTRY; Ctr++) {
    Ms2IdiSetXptEntryTable (Socket, Ctr, MemInfo->MiniSadInfo.MiniSadTable32EntryRegValue[Socket][Ctr]);
  }

  //
  // Program 32 entry prefetch base registers
  //
  for (Ctr = 0; Ctr < NUM_OF_REGSTERS_32_ENTRY_PREFETCH_BASE; Ctr++) {
    Ms2IdiSetXptEntryPrefBase (Socket, Ctr, MemInfo->MiniSadInfo.Xpt32EntryPrefetchBase[Socket][Ctr]);
  }

  //
  // Set MiniSAD Mode
  //
  if (KTIVAR->OutXptPrefetch == KTI_ENABLE) {
    MiniSadModeSelect   = MemInfo->MiniSadInfo.Xptminisadmodeselect;
    McMiniSadMode       = (UINT8)MemInfo->MiniSadInfo.XptMcMiniSadMode;
    Ms2IdiSetMiniSadMode (Socket, MiniSadModeSelect, McMiniSadMode, MemInfo->MiniSadInfo.XorDefeature);
  }

  //
  // Set xpt_memory_interleave_granularity
  //
  XptMemInterleave = MemInfo->MiniSadInfo.IntGranularity;
  Ms2IdiSetXptMemInterleave (Socket, XptMemInterleave);

  //
  // M2MEM
  //
  SetM2mPrefetchConfigs (Socket, KTIVAR->OutUmaClustering, (VOID *)MemInfo);

  //
  // Enable xpt_prefetch, MS2IDI
  //
  Ms2IdiEnLocXptPref (Socket, (KTIVAR->OutXptPrefetch == KTI_ENABLE));

  //
  // Set XPT FIFO credit
  //
  XptFifoEnabledCredit = XPT_FIFO_CREDIT_2;
  if (KTISETUP->DfxParm.DfxXptFifoEnabledCredit < XPT_FIFO_CREDIT_AUTO) {
    XptFifoEnabledCredit = KTISETUP->DfxParm.DfxXptFifoEnabledCredit;
  }
  Ms2IdiXptCreditConfig (Socket, XptFifoEnabledCredit);

  //
  // Setup UPI MiniSAD
  //
  if (KTIVAR->OutKtiPrefetch == KTI_ENABLE) {
    if (!KtiInternalGlobal->UpiMinisadCfgDone[Socket]) {
      SetupUpiMiniSad (
        KtiInternalGlobal,
        Socket,
        MemInfo->MiniSadInfo.UpiMiniSADMode[Socket],
        MemInfo->MiniSadInfo.UPIMiniSad[Socket]
        );
    }
  }
  // Enable or Disable KTI prefetch
  EnablePrefetchInKtiAgent (
    KtiInternalGlobal,
    Socket,
    (KTIVAR->OutKtiPrefetch == KTI_ENABLE) ? KTI_XPT_PREFETCH_EN : KTI_XPT_PREFETCH_DISABLE
    );

  //
  // Set remote XPT prefetch
  //
  if (KTIVAR->OutXptRemotePrefetch == KTI_ENABLE) {
    if (GetSysResetRequired () == 0) {
      //
      // Enable XPT Remote Prefetch in UPI port first, then in MS2IDI
      //
      for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
        if (KTIVAR->CpuInfo[Socket].LepInfo[Ctr].Valid == TRUE) {
          RemoteSktId = (UINT8) KTIVAR->CpuInfo[Socket].LepInfo[Ctr].PeerSocId;
          if (!KtiInternalGlobal->UpiMinisadCfgDone[RemoteSktId]) {
            UpdateMiniSadInForMc (KtiInternalGlobal, RemoteSktId, MemInfo, &MemInfo->MiniSadInfo);
            SetupUpiMiniSad (
              KtiInternalGlobal,
              RemoteSktId,
              MemInfo->MiniSadInfo.UpiMiniSADMode[RemoteSktId],
              MemInfo->MiniSadInfo.UPIMiniSad[RemoteSktId]
              );
          }
          //
          // Enable XPT prefetch support in the UPI Link Layer Rx.
          // Reception of RemMemSpecRd when it's disabled will result in an
          // uncorrectable error.
          //
          EnablePrefetchInKtiAgent (KtiInternalGlobal, RemoteSktId, REMOTE_XPT_PREFETCH_EN);
        }
      }
      ProgramRemoteXpt (KtiInternalGlobal, Socket, MemInfo);
    }
  } else {
    // Disable Remote
    DisableRemoteXpt (KtiInternalGlobal, Socket);
  }
}