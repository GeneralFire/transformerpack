/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Library/MemMcIpLib.h>
#include "Include/MemMcRegs.h"
#include <Library/MemAccessLib.h>
#include <Mem/Include/MemIpVer.h>
#include <Library/RcDebugLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CpuAndRevisionLib.h>

/**
  Internal worker function to configure XOR mode bit fields in MCMTR_MC_MAIN register

  @param[in, out] McMainReg    Pointer to MC main register
  @param[in]      XorMode      Bitmap for XOR fields
  @param[in]      DimmNvList   Pointer to DIMM_NVRAM_STRUCT array

**/
VOID
McSetXorModeCfg (
  IN OUT MCMTR_MC_MAIN_STRUCT   *McMainReg,
  IN     UINT8                  XorMode,
  IN     DIMM_NVRAM_STRUCT      (*DimmNvList)[MAX_DIMM]
  )
{
  UINT8    DimmId;

  McMainReg->Bits.pch_xor_enable = (XorMode & PSEUDO_CHANNEL_XOR) != 0 ? 1 : 0;
  McMainReg->Bits.channel_xor_enable = (XorMode & CHANNEL_XOR) != 0 ? 1 : 0;
  McMainReg->Bits.cluster_xor_enable = (XorMode & CLUSTER_XOR) != 0 ? 1 : 0;
  McMainReg->Bits.bank_xor_enable = (XorMode & BANK_XOR) != 0 ? 1 : 0;
  McMainReg->Bits.sid_xor_enable = (XorMode & SID_XOR) != 0 ? 1 : 0;

  //
  // Enable rank XOR only if all DIMMs installed in the channel are Dual Rank.
  //
  if ((XorMode & RANK_XOR) != 0) {
    McMainReg->Bits.rank_xor_enable = 1;
    for (DimmId = 0; DimmId < MAX_DIMM; DimmId++) {
      if (((*DimmNvList)[DimmId].dimmPresent != 0) && ((*DimmNvList)[DimmId].numRanks != 2)) {
        McMainReg->Bits.rank_xor_enable = 0;
        break;
      }
    }
  } else {
    McMainReg->Bits.rank_xor_enable = 0;
  }

  //
  // Enabled DIMM XOR only if there are 2 identical DDR5 DIMMs populated.
  //
  if (((XorMode & DIMM_XOR) != 0) && ((*DimmNvList)[0].dimmMemTech == (*DimmNvList)[1].dimmMemTech) &&
      ((*DimmNvList)[0].dimmPresent != 0) && ((*DimmNvList)[0].DcpmmPresent == 0)) {
    McMainReg->Bits.dimm_xor_enable = 1;
  } else {
    McMainReg->Bits.dimm_xor_enable = 0;
  }

}

/**
  This function programs the MC 2LM near memory cache configuration register.

  @param[in] MemTechType  Memory technology type.
  @param[in] ScktId       Socket index.
  @param[in] McId         Memory controller index on the socket.
  @param[in] ChId         Channel index on the memory controller.
  @param[in] NmData       Information required to define a 2LM near memory region.

  @retval N/A
**/
VOID
McSetNm2lmCacheCfg (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          ScktId,
  IN UINT8          McId,
  IN UINT8          ChId,
  IN NM_2LM_DATA    *NmData
  )
{
  MCNMCACHINGCFG_MC_2LM_STRUCT      McNm2lmCachCfg;
  UINT8                             NumChPerMc;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
  }

  McNm2lmCachCfg.Data = 0;

  //
  // When biased 2-way near memory cache is enabled, UEFI FW can only use half of the discovered near memory size.
  //
  if (NmData->EnableTwoWay && (MemTechType != MemTechHbm)) {
    McNm2lmCachCfg.Bits.chn_cap = NmData->NmChSize - 1;
  } else {
    McNm2lmCachCfg.Bits.chn_cap = NmData->NmChSize;
  }

  McNm2lmCachCfg.Bits.nm_ratio_chn_cap = 0;       // non-power of 2 is disabled or all channel capacity is used for 2LM.
  McNm2lmCachCfg.Bits.en_2way          = NmData->EnableTwoWay;

  //
  // Select the address bit used to determine preferred way based on the NM size.
  //
  if (McNm2lmCachCfg.Bits.en_2way != 0) {
    if (McNm2lmCachCfg.Bits.chn_cap < Log2x32 (MEM_256GB_IN_4GB_UNIT)) {
      McNm2lmCachCfg.Bits.prefer_way_sel = 1;  // Select bit 31 if NM size is less than 256GB.
    } else {
      McNm2lmCachCfg.Bits.prefer_way_sel = 0;  // Select bit 37 if NM size is greater than or equal to 256GB.
    }
  }

  ChRegisterWrite (
    MemTechType,
    ScktId,
    ChId + (McId * NumChPerMc),
    MCNMCACHINGCFG_MC_2LM_REG,
    McNm2lmCachCfg.Data
    );

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChId, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    " MCNMCACHINGCFG_MC_2LM_REG:\n"
    "  en_2way: %d  prefer_way_sel: %d  chn_cap: 0x%x  nm_ratio_chn_cap: %d\n",
    McNm2lmCachCfg.Bits.en_2way,
    McNm2lmCachCfg.Bits.prefer_way_sel,
    McNm2lmCachCfg.Bits.chn_cap,
    McNm2lmCachCfg.Bits.nm_ratio_chn_cap
    );
}

/**
  This function programs the MC 2LM near memory caching offset register.

  @param[in] MemTechType  Memory technology type.
  @param[in] ScktId       Socket index.
  @param[in] McId         Memory controller index on the socket.
  @param[in] ChId         Channel index on the memory controller.
  @param[in] NmData       Information required to define a 2LM near memory region.

  @retval N/A
**/
VOID
McSetNmCachingOffset (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          ScktId,
  IN UINT8          McId,
  IN UINT8          ChId,
  IN NM_2LM_DATA    *NmData
  )
{
  MCNMCACHINGOFFSET_MC_2LM_STRUCT   McNmCachingOffset;
  UINT8                             NumChPerMc;

  if (MemTechType == MemTechHbm) {
    NumChPerMc = GetNumChannelPerMcHbm ();
  } else {
    NumChPerMc = GetNumChannelPerMc ();
  }

  McNmCachingOffset.Data = 0;
  McNmCachingOffset.Bits.mcnmcachingoffset = NmData->NmCacheBase >> CONVERT_64MB_TO_32GB_GRAN;

  //
  // Extra lower 5 bits of near memory cache base address used to conjunct with NmCachingOffset.
  //
  McNmCachingOffset.Bits.mcnmcachingoffsetlower = (NmData->NmCacheBase >> CONVERT_64MB_TO_GB_GRAN) & 0x1f;

  if ((McNmCachingOffset.Bits.mcnmcachingoffsetlower != 0) || (McNmCachingOffset.Bits.mcnmcachingoffset != 0)) {
    McNmCachingOffset.Bits.mcnmcachingoffseten = 1;
  }

  ChRegisterWrite (
    MemTechType,
    ScktId,
    ChId + (McId * NumChPerMc),
    MCNMCACHINGOFFSET_MC_2LM_REG,
    McNmCachingOffset.Data
    );

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChId, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "MCNMCACHINGOFFSET:\n"
    "  mcnmcachingoffsetlower: 0x%x  mcnmcachingoffset: 0x%x  mcnmcachingoffseten: %d\n",
    McNmCachingOffset.Bits.mcnmcachingoffsetlower,
    McNmCachingOffset.Bits.mcnmcachingoffset,
    McNmCachingOffset.Bits.mcnmcachingoffseten
    );
}

/**
  This function enables metadata ECC correction.

  @param[in] ScktId           Socket index.
  @param[in] McId             Memory controller index on socket.
  @param[in] ChId             Channel index on memory controller.

  @retval N/A
**/
VOID
EFIAPI
McSetMetaEccCorrBit (
  IN UINT8   ScktId,
  IN UINT8   McId,
  IN UINT8   ChId
  )
{
  UINT8                              MaxChPerMc;
  MC0_DP_CHKN_BIT3_MCDDC_DP_STRUCT   ChknBitReg;

  MaxChPerMc = GetNumChannelPerMc ();

  ChknBitReg.Data = MemReadPciCfgEp (ScktId, ChId + (McId * MaxChPerMc), MC0_DP_CHKN_BIT3_MCDDC_DP_REG);
  if (IsCpuAndRevision (CPU_SPRSP, REV_A0)) {
    ChknBitReg.Bits.dis_meta_ecc_corr = 1;
  } else {
    ChknBitReg.Bits.dis_meta_ecc_corr = 0;
  }
  MemWritePciCfgEp (ScktId, ChId + (McId * MaxChPerMc), MC0_DP_CHKN_BIT3_MCDDC_DP_REG, ChknBitReg.Data);
}

/**
  This function enables VC preemption by setting scheduler_enables_bs.enable_vc_preemption.

  @param[in] ScktId           Socket index.
  @param[in] McId             Memory controller index on socket.
  @param[in] ChId             Channel index on memory controller.
  @param[in] EnVcPreemption   Flag indicates if VC preemption needs to be enabled.

  @retval N/A
**/
VOID
EFIAPI
McSetVcPreemptionBit (
  IN UINT8   ScktId,
  IN UINT8   McId,
  IN UINT8   ChId,
  IN BOOLEAN EnVcPreemption
  )
{
  UINT8                                  MaxChPerMc;
  SCHEDULER_ENABLES_BS_MCDDC_CTL_STRUCT  SchedulerEnBsReg;

  MaxChPerMc = GetNumChannelPerMc ();

  SchedulerEnBsReg.Data = MemReadPciCfgEp (ScktId, ChId + (McId * MaxChPerMc), SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
  SchedulerEnBsReg.Bits.enable_vc_preemption = EnVcPreemption;
  MemWritePciCfgEp (ScktId, ChId + (McId * MaxChPerMc), SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnBsReg.Data);
}

/**
  This function disables the ADR actions per channel.

  @param[in] ScktId            Socket index.
  @param[in] McId              Memory controller index on socket.
  @param[in] AdrDisableBitMap  Bitmap indicates if ADR needs to be disabled on the channels of the specified MC.
                               Disable = 1 and Enable = 0. One bit corresponds to one channel.

  @retval N/A
**/
VOID
EFIAPI
McSetAdrDisableBits (
  IN UINT8   ScktId,
  IN UINT8   McId,
  IN UINT8   AdrDisableBitMap
  )
{
  UINT8                               ChBitMask;
  RCOMP_GLOBALCTRL1_MC_GLOBAL_STRUCT  RcompGlobalCtrlReg;

  ChBitMask = (BIT0 << GetNumChannelPerMc ()) - 1;

  RcompGlobalCtrlReg.Data = MemReadPciCfgMC (ScktId, McId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG);
  RcompGlobalCtrlReg.Bits.eadr_per_chnl_disable = AdrDisableBitMap & ChBitMask;
  MemWritePciCfgMC (ScktId, McId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG, RcompGlobalCtrlReg.Data);
}

/**
  This function checks if the fuse of Persistent Memory with Crystal Ridge is
  disabled on system.

  @param VOID

  @retval TRUE                 The fuse of persistent memory is disabled.
  @retval FALSE                The fuse of persistent memory is enabled.
**/
BOOLEAN
EFIAPI
McCheckCrPmemFuseDisabled (
  VOID
  )
{
  UINT8                                    ScktId;
  UINT8                                    ChId;
  UINT8                                    MaxCh;
  IMC_FUSE_DOWNLOAD_SHADOW_MC_MAIN_STRUCT  FuseShadowReg;

  MaxCh = GetMaxImc () * GetNumChannelPerMc ();

  for (ScktId = 0; ScktId < MAX_SOCKET; ScktId++) {
    if (!CheckSocketEnabled (ScktId)) {
      continue;
    }

    for (ChId = 0; ChId < MaxCh; ChId++) {
      if (!IsChannelEnabled (ScktId, ChId)) {
        continue;
      }

      FuseShadowReg.Data = MemReadPciCfgEp (ScktId, ChId, IMC_FUSE_DOWNLOAD_SHADOW_MC_MAIN_REG);
      if (FuseShadowReg.Bits.fuse_shadow_disable_cr_pmem == 1) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
  This function sets the wcilfoptdis bit field of MCNMCACHINGCFG_MC_MAIN_STRUCT register.

  @param[in] SocketId         Socket index.
  @param[in] ChId             Channel index within socket.
  @param[in] WcilfOptDis      Flag indicates if Wcilf optimization needs to be disabled.

**/
VOID
EFIAPI
McSetNmCachingCfgWcilfOptDis (
  IN UINT8    SocketId,
  IN UINT8    ChId,
  IN BOOLEAN  WcilfOptDis
  )
{
  MCNMCACHINGCFG_MC_MAIN_STRUCT   McNmCachingCfgReg;

  if ((SocketId >= MAX_SOCKET) || (ChId >= GetMaxChDdr ())) {
    return;
  }

  McNmCachingCfgReg.Data = MemReadPciCfgEp (SocketId, ChId, MCNMCACHINGCFG_MC_MAIN_REG);
  McNmCachingCfgReg.Bits.wcilfoptdis = WcilfOptDis;
  MemWritePciCfgEp (SocketId, ChId, MCNMCACHINGCFG_MC_MAIN_REG, McNmCachingCfgReg.Data);
}
