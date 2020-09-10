/** @file
  Implementation of handle poison and viral error.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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
#include <Library/PlatMemTopologyLib.h>
#include <Library/RasDebugLib.h>
#include <Library/CrystalRidgeLib.h>
#include <Library/WorkaroundLib.h>
#include <Library/ProcessorRasLib.h>
#include <RcRegs.h>
#include <Library/MemMcIpRasLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/IioRasLib.h>
#include <Library/IioTopologyLib.h>
#include <Library/IoLib.h>
#include <Library/RasSiliconLib.h>
#include <Library/MmPciExtLib.h>
#include <Protocol/CrystalRidge.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/KtiApi.h>

STATIC BOOLEAN mViralEn;
STATIC BOOLEAN mPoisonEn;

/**
  Enable Viral on each KTI link.

  @param[in] Skt       - The socket to enable
  @param[in] ClearFlag - clear flag

  @retval None

**/
VOID
EnableKtiViralPerSkt (
  IN        UINT8                   Skt,
  IN        BOOLEAN                 ClearFlag
  )
{

  UINT8                             LinkIndex;
  UINT8                             Ctr;
  UINT8                             MaxM3Kti = 0;

  MEM_TOPOLOGY                       *MemTopology;

  MemTopology = GetMemTopology ();

  for (LinkIndex = 0; LinkIndex < (GetKtiPortCnt()); LinkIndex++) {
    if (MemTopology->Socket[Skt].UPIPortValid[LinkIndex]) {
      EnableKtiViralPerLink (Skt, LinkIndex, ClearFlag);
    } else {
      if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
        if (ClearFlag) {
          ClearKtiViralStatusPerLink (Skt, LinkIndex);
        }
      }
    }
  } // for (LinkIndex = 0; LinkIndex < (GetKtiPortCnt()); LinkIndex++)

  //
  // Enable Viral in KTI and PCIe logic
  //
  for (Ctr = 0; Ctr < MAX_IIO_STACK; Ctr++) {
    EnableM2PciViral (Skt, Ctr);
  }

  //
  // Enable Viral in M3KTI
  //

  if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      MaxM3Kti = MAX_CPX_M3KTI;
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL)) {
      MaxM3Kti = MAX_SKX_M3KTI;
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    MaxM3Kti = MAX_ICX_M3KTI;
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    MaxM3Kti = MAX_SPR_M3KTI;
  }

  for (Ctr = 0; Ctr < MaxM3Kti; Ctr++) {
    EnableM3KTIViral (Skt, Ctr);
  }
}

/**
  Implementation of enable viral on AEP
**/
VOID
EFIAPI
AepViralCrystalRidgeCallback (
  VOID
  )
{
  UINT8         Socket;
  UINT8         ChOnSkt;
  UINT8         ChOnMc;
  UINT8         Dimm;
  UINT8         Mc;
  RETURN_STATUS Status;
  UINT32        Data;
  MEM_TOPOLOGY  *MemTopology;
  NVDIMM_INFO   NvdimmInfo;
  UINT8         McaCorrOnDataUc;
  BOOLEAN       DdrtDimmPresent;
  UINT32        nonVolSize;

  MemTopology = GetMemTopology ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!IsSocketPresent (Socket)) {
      continue;
    }
    if (mPoisonEn) {
      for (Mc = 0; Mc < MAX_IMC; Mc++) {
        if (MemTopology->VolMemMode == VOL_MEM_MODE_1LM) {
          McaCorrOnDataUc = 1;
        } else if (MemTopology->VolMemMode == VOL_MEM_MODE_2LM) {
          McaCorrOnDataUc = 0;
        } else {
          // VOL_MEM_MODE_MIX_1LM2LM
          McaCorrOnDataUc = 1;
          for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc++) {
            ChOnSkt = NODECHA_TO_SKTCHA (Mc, ChOnMc);
            if (MemTopology->DdrCacheSize[Socket][ChOnSkt] != 0) {
              McaCorrOnDataUc = 0;
              break;
            }
          }
        }
        SetM2mExRasConfigMcaCorrOnDataUc (Socket, Mc, McaCorrOnDataUc);
      }
    }
    for (ChOnSkt = 0; ChOnSkt < IMC_MAX_CH; ChOnSkt++) {
      DdrtDimmPresent = FALSE;
      nonVolSize = 0;

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (!MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].DimmInfo[Dimm].Enabled) {
          continue;
        }
        if (!(CheckForDdrtDimm (Socket, ChOnSkt, Dimm))) {
          continue;
        }

        DdrtDimmPresent = TRUE;
        nonVolSize += MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].DimmInfo[Dimm].nonVolCap;

        Status = GetIdentifyDimm (Socket, ChOnSkt, Dimm, &NvdimmInfo);
        if (NvdimmInfo.FisVersion >= FIS_VIRAL_STANDALONE) {
          Status = GetViralPolicy (Socket, ChOnSkt, Dimm, &Data);
          if (EFI_ERROR (Status)) {
            RAS_DEBUG ((LEVEL_BASIC_FLOW, "Viral config not set: Get Viral Status fail!! \n"));
          } else if ((mViralEn && !(Data & CFG_VIRAL_POLICY_ENABLE_STANDALONE)) || (!mViralEn && (Data & CFG_VIRAL_POLICY_ENABLE_STANDALONE))) {
            Data = mViralEn ? (Data | CFG_VIRAL_POLICY_ENABLE_STANDALONE) : (Data & ~CFG_VIRAL_POLICY_ENABLE_STANDALONE);
            Status = SetViralPolicy (Socket, ChOnSkt, Dimm, Data);
            if (EFI_ERROR (Status)) {
              RAS_DEBUG((LEVEL_BASIC_FLOW, "Viral config not set: Set Viral Status fail!! \n"));
            }
          }
        } else {
          Status = GetCfgDataPolicy (Socket, ChOnSkt, Dimm, &Data);
          if (EFI_ERROR (Status)) {
            RAS_DEBUG((LEVEL_BASIC_FLOW, "Viral config not set: Get Config fail!! \n"));
            continue;
          }
          if ((mViralEn && !(Data & CFG_DATA_VIRAL_POLICY_ENABLE)) || (!mViralEn && (Data & CFG_DATA_VIRAL_POLICY_ENABLE))) {
            Data = mViralEn ? (Data | CFG_DATA_VIRAL_POLICY_ENABLE) : (Data & ~CFG_DATA_VIRAL_POLICY_ENABLE);
            Status = SetCfgDataPolicy (Socket, ChOnSkt, Dimm, Data);
            if (EFI_ERROR (Status)) {
              RAS_DEBUG ((LEVEL_BASIC_FLOW, "Viral config not set: Set Config fail!! \n"));
            }
          }
        }
      }

      if (mViralEn && DdrtDimmPresent) {
        if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
          DdrtViralConfig (Socket, ChOnSkt);
        } else {
          if ((MemTopology->VolMemMode == VOL_MEM_MODE_1LM) && (nonVolSize > 0)) {
            //
            // clear ddrt_defeature.ignore_viral bit only when ViralEn=1 and DDRT is in 1LM-AppDirect mode.
            //
            DdrtViralConfig (Socket, ChOnSkt);
          }
        }
      }
    }
  }
} // AepViralCrystalRidgeCallback



/**
  This function enable forwarding of Viral indication

   @param[in] Skt       - The socket to enable
   @param[in] ClearFlg  - Viral status clear flag
   @param[in] IoMcaEn   - IoMca Enable

   @retval    none.
**/
EFI_STATUS
EFIAPI
IioEnableViral (
  IN      UINT8      Skt,
  IN      BOOLEAN    ClearFlg,
  IN      UINT8      IoMcaEn
  )
{
  VIRAL_IIO_RAS_STRUCT      IioViral; //VIRAL_IIO_RAS_REG
  UINT32                    IioViralMask;
  UINT8                     IioStack;
  IIO_TOPOLOGY              *pIioTopology;
  BOOLEAN                   Is14nm;

  pIioTopology = GetIioTopology ();
  Is14nm = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);

  for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
    if (!IsIioStackValid (Skt, IioStack)) {
      continue;
    }
    //
    // Set Viral Enable on the IIO
    //
    IioViral.Data = ReadCpuCsr (Skt, IioStack, VIRAL_IIO_RAS_REG);
    RAS_DEBUG((LEVEL_FUNC_FLOW, "IioEnableViral VIRAL_IIO_RAS_REG = 0x%x!\n", IioViral.Data));
    if (ClearFlg) {
      IioViral.Bits.iio_viral_status = 1;
      IioViralMask = 0xc0000000;
    } else {
      IioViral.Bits.iio_viral_status = 0;
      IioViralMask = 0x80000000;
    }
    IioViral.Bits.iio_viral_state = 1;
    do {
      WriteCpuCsr (Skt, IioStack, VIRAL_IIO_RAS_REG, IioViral.Data);
      IioViral.Data = ReadCpuCsr (Skt, IioStack, VIRAL_IIO_RAS_REG) & IioViralMask;
      RAS_DEBUG((LEVEL_FUNC_FLOW, "IioEnableViral VIRAL_IIO_RAS_REG = 0x%x!\n", IioViral.Data));
    } while (IioViral.Data);

    IioViral.Data =  ReadCpuCsr (Skt, IioStack, VIRAL_IIO_RAS_REG) ;
    RAS_DEBUG((LEVEL_FUNC_FLOW, "IioEnableViral VIRAL_IIO_RAS_REG = 0x%x!\n", IioViral.Data));
    if (IioViral.Bits.iio_viral_status == 1){
      RAS_DEBUG((LEVEL_REG, "Iio viral status found in asserted state. Iio Socket ID :  %d \n", Skt));
      IioViral.Bits.iio_viral_status = 0;
    }

    IioViral.Bits.iio_signal_global_fatal = 1;

    IioViral.Bits.iio_global_viral_mask = Is14nm ? 1 : 0;
    IioViral.Bits.iio_fatal_viral_alert_enable = 1;
    IioViral.Bits.generate_viral_alert = 0;
    WriteCpuCsr (Skt, IioStack, VIRAL_IIO_RAS_REG, IioViral.Data);
  } //for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++)

  return EFI_SUCCESS;
}


/**
  This function check for viral errors.

  @param[in] Ioh            The IOH number.

  @retval    Status         return TRUE if viral error is reported.
**/
BOOLEAN
SocketInViralState (
  UINT8 Socket
  )
{
  BOOLEAN                                   ViralState = FALSE;
  VIRAL_IIO_RAS_STRUCT                      ViralIioRas;
  UINT8                                     IioStack;
  IIO_TOPOLOGY                              *pIioTopology;

  pIioTopology = GetIioTopology ();

  for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
    if (!IsIioStackValid (Socket, IioStack)) {
      continue;
    }
    //
    // read the VIRAL_IIO_RAS to check for Viral State
    //
    ViralIioRas.Data = ReadCpuCsr (Socket, IioStack, VIRAL_IIO_RAS_REG);

    if (ViralIioRas.Bits.iio_viral_state) {
      ViralState = TRUE;
      break;
    }
  }

  return ViralState;
}


/**
  Implementation of poison and Viral.

  @param  ViralFlag   TRUE -- Viral enable
  @param  PoisonFlag  TRUE -- Poison enable
  @param  ClearFlag   TRUE -- clear Viral status.

**/
VOID
EFIAPI
EnableSystemViralAndPoison (
  IN   BOOLEAN  ViralFlag,
  IN   BOOLEAN  PoisonFlag,
  IN   BOOLEAN  ClearFlag,
  IN   UINT8    IoMcaEn
  )
{
  UINT8                     Skt;

  mViralEn = ViralFlag;
  mPoisonEn = PoisonFlag;
  for (Skt = 0; Skt < MAX_SOCKET; Skt ++) {
    if (!IsSocketPresent (Skt)) {
      continue;
    }

    if (PoisonFlag) {
      Wa_5331846 (Skt);
      EnableCxlPosionPerSkt (Skt);
    }

    if (ViralFlag) {
      EnableKtiViralPerSkt (Skt, ClearFlag);
      EnableUBoxViralPerSkt (Skt);
      EnablePunitErrorSourceWa1409498645 (Skt);
    }
  }

  RegisterCrystalRidgeReadyCallback (AepViralCrystalRidgeCallback);
}

/**
  Viral handler implementation for KTI, UBOX, and IIO

  @retval none.

**/
VOID
EFIAPI
ProcessKtiFatalAndViralError (
  VOID
  )
{
  UINT8                       Skt;
  UINT8                       LinkIndex;
  MEM_TOPOLOGY               *MemTopology;


  LinkIndex = 0;
  MemTopology = GetMemTopology ();

  for (Skt = 0; Skt < MAX_SOCKET; Skt ++) {
    if (IsSocketPresent (Skt) == FALSE) {
      continue;
    }

    //
    // kti viral
    //
    for (LinkIndex = 0; LinkIndex < (GetKtiPortCnt()); LinkIndex++) {
      if (!MemTopology->Socket[Skt].UPIPortValid[LinkIndex]) {
        continue;
      }

      if (IsKtiViralState (Skt, LinkIndex)) {
          RAS_DEBUG((LEVEL_BASIC_FLOW, "Viral: Socket: %d  KTI port:%d in viral state \n", Skt, LinkIndex));
          CpuDeadLoop ();
      }
    }

    //
    //ubox viral
    //
    if (IsUboxViral (Skt)) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "Viral: ubox unsupported opcode\n"));
      CpuDeadLoop ();
    }

    //
    //iio viral
    //
    if (SocketInViralState (Skt)) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "Viral: socket:%d  iio:%d detect viral\n", Skt));
      CpuDeadLoop ();
    }
  }
}
