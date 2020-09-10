/** @file
  Implementation of KTI IP

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <RcRegs.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/KtiApi.h>

/**
  enable/disable KTI error log

  @param Socket  -- socket index
  @param LinkIndex -- Kti link index
  @param EnableFlag  -- Enable/disable flag

  @return none.
**/
VOID
EFIAPI
EnableDisableKtiElog (
  IN  UINT8    Socket,
  IN  UINT8    LinkIndex,
  IN  BOOLEAN  EnableFlag
  )
{
  KTIERRDIS0_KTI_LLPMON_STRUCT                KtierrdisReg;
  KtierrdisReg.Data = ReadCpuCsr (Socket, LinkIndex, KTIERRDIS0_KTI_LLPMON_REG);

  //
  //Clear smi enable bit (SMI support is not POR so always disable)
  //
  KtierrdisReg.Bits.smi_en = EnableFlag;
  WriteCpuCsr (Socket, LinkIndex, KTIERRDIS0_KTI_LLPMON_REG, KtierrdisReg.Data);
}


/**
  get KTI port viral status

  @param Socket -- socket index
  @param LinkIndex -- kti port

  @retval none.
**/
BOOLEAN
EFIAPI
IsKtiViralState (
  IN  UINT8   Socket,
  IN  UINT8   LinkIndex
  )
{
  KTIVIRAL_KTI_LLPMON_STRUCT  KtiviralReg;
  KtiviralReg.Data = ReadCpuCsr (Socket, LinkIndex, KTIVIRAL_KTI_LLPMON_REG);

  if (KtiviralReg.Bits.kti_viral_state == 1) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Clear KTI viral status per link

  @param  Skt  -- socket index
  @param  LinkIndex  -- KTI link port

  @return none
**/
VOID
EFIAPI
ClearKtiViralStatusPerLink (
  IN  UINT8    Skt,
  IN  UINT8    LinkIndex
  )
{
}


/**
  enable KTI viral per link

  @param  Skt  -- socket index
  @param  LinkIndex  -- KTI link port
  @param  ClearFlag -- clear flag

  @return none
**/
VOID
EFIAPI
EnableKtiViralPerLink (
  IN  UINT8    Skt,
  IN  UINT8    LinkIndex,
  IN  BOOLEAN  ClearFlag
  )
{
  KTIVIRAL_KTI_LLPMON_STRUCT        KtiviralReg;
  KTIVIRAL_KTI_LLPMON_STRUCT        KtiViralMask;
  //
  // Clear failover status
  //
  KtiViralMask.Data = 0;
  KtiviralReg.Data = ReadCpuCsr (Skt, LinkIndex, KTIVIRAL_KTI_LLPMON_REG);
  if (KtiviralReg.Bits.kti_unc_err_set   ||
      KtiviralReg.Bits.kti_pkt_viral_set ||
      KtiviralReg.Bits.kti_viral_status  ||
      KtiviralReg.Bits.kti_viral_state)
  {
      KtiviralReg.Bits.kti_unc_err_set = 1;
      KtiViralMask.Bits.kti_unc_err_set |= KtiviralReg.Bits.kti_unc_err_set;

      KtiviralReg.Bits.kti_pkt_viral_set = 1;
      KtiViralMask.Bits.kti_pkt_viral_set |= KtiviralReg.Bits.kti_pkt_viral_set;

      if (ClearFlag) {
        KtiviralReg.Bits.kti_viral_status = 1;
        KtiViralMask.Bits.kti_viral_status |= KtiviralReg.Bits.kti_viral_status;
      } else {
        KtiviralReg.Bits.kti_viral_status = 0;
        KtiViralMask.Bits.kti_viral_status |= KtiviralReg.Bits.kti_viral_status;
      }
      KtiviralReg.Bits.kti_viral_state = 1;
      KtiViralMask.Bits.kti_viral_state |= KtiviralReg.Bits.kti_viral_state;

      WriteCpuCsr (Skt, LinkIndex, KTIVIRAL_KTI_LLPMON_REG, KtiviralReg.Data);

      while (1) {
        KtiviralReg.Data = ReadCpuCsr (Skt, LinkIndex, KTIVIRAL_KTI_LLPMON_REG);
        if ((KtiviralReg.Data & KtiViralMask.Data) == 0) {
          break;
        }
        KtiviralReg.Data |= KtiViralMask.Data;
        WriteCpuCsr (Skt, LinkIndex, KTIVIRAL_KTI_LLPMON_REG, KtiviralReg.Data);
      }
      KtiviralReg.Data = ReadCpuCsr (Skt, LinkIndex, KTIVIRAL_KTI_LLPMON_REG);

      if (KtiviralReg.Bits.kti_viral_status == 1) {

        KtiviralReg.Bits.kti_viral_status = 0;
      }
    }

    KtiviralReg.Bits.kti_viral_alert_enable = 1;
    KtiviralReg.Bits.kti_signal_global_fatal = 1;
    WriteCpuCsr (Skt, LinkIndex, KTIVIRAL_KTI_LLPMON_REG, KtiviralReg.Data);

    KtiviralReg.Bits.kti_global_viral_mask = 1;
    WriteCpuCsr (Skt, LinkIndex, KTIVIRAL_KTI_LLPMON_REG, KtiviralReg.Data);

    //
    //Clear smi enable bit (SMI support is not POR so always disable)
    //
    EnableDisableKtiElog (Skt, LinkIndex, FALSE);
}



/**
  Enable KTI common mesh stop viral

  @param  Socket -- socket index
  @param  LinkIndex -- Link index

  @return none.
**/
VOID
EFIAPI
EnableM3KTIViral (
  IN   UINT8     Socket,
  IN   UINT8     LinkIndex
  )
{
#ifndef GNRSRF_HOST
  M3GLERRCFG_M3KTI_MAIN_STRUCT  M3glerrcfgReg;

  M3glerrcfgReg.Data = ReadCpuCsr (Socket, LinkIndex, M3GLERRCFG_M3KTI_MAIN_REG);
  M3glerrcfgReg.Bits.resetglobalfatalerror = 0;
  M3glerrcfgReg.Bits.resetglobalviral = 0;
  M3glerrcfgReg.Bits.maskm3fatalerror = 0;
  M3glerrcfgReg.Bits.maskktifatalerrorin = 0;
  M3glerrcfgReg.Bits.maskktiviralout = 0;
  M3glerrcfgReg.Bits.maskktiiviralin = 0;
  M3glerrcfgReg.Bits.maskpcufatalerrorout = 0;
  WriteCpuCsr (Socket, LinkIndex, M3GLERRCFG_M3KTI_MAIN_REG, M3glerrcfgReg.Data);
#endif // !GNRSRF_HOST
}

/**
  Set KTI CSMI threshold

  @param[in]  CsmiThreshold    KTI CSMI threshold setting
  @param[in]  CloakingEn       Indicates whether cloaking is enabled or not

**/
VOID
EFIAPI
SetKtiCsmiThreshold (
  IN UINT16    CsmiThreshold,
  IN BOOLEAN   CloakingEn
  )
{
  UINT8                          Socket;
  UINT8                          LinkIndex;
  MEM_TOPOLOGY                   *MemTopology;
  KTICSMITHRES_KTI_LLPMON_STRUCT KtiCsmiThresCfg;

  MemTopology = GetMemTopology ();

  KtiCsmiThresCfg.Data                   = 0;
  KtiCsmiThresCfg.Bits.bios_ce_threshold = CsmiThreshold;
  if (CloakingEn) {
    KtiCsmiThresCfg.Bits.match_en          = 1;
  } else {
    KtiCsmiThresCfg.Bits.match_en          = CsmiThreshold == 0 ? 0 : 1;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }
    for (LinkIndex = 0; LinkIndex < (GetKtiPortCnt()); LinkIndex++) {
      if (!MemTopology->Socket[Socket].UPIPortValid[LinkIndex]) {
        continue;
      }
      WriteCpuCsr (Socket, LinkIndex, KTICSMITHRES_KTI_LLPMON_REG, KtiCsmiThresCfg.Data);
    }
  }
}
