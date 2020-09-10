/** @file
  Implementation of Error Control Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/RasDebugLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <RcRegs.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/CsrRegTableLib.h>
#include <Library/KtiApi.h>

/**
  Enable or disable Patrol Error Downgrade

  @param[in]  PatrolErrorDowngradeEn     Patrol Error Downgrade setting.
**/
VOID
EFIAPI
PatrolErrorDowngradeSetting (
  IN UINT8 PatrolErrorDowngradeEn
  )
{
  UINT8                                       Socket;
  UINT8                                       Mc;
  UINT8                                       ChOnMc;
  UINT8                                       ChOnSkt;
  UINT64                                      MmioAddr;
  UINT32                                      RegAddr;
  UINT32                                      RegValue;
  UINT8                                       Size = 4;


  for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

    for (Mc=0; Mc < MAX_IMC; Mc ++) {
      if (IsNodePresent (SKTMC_TO_NODE(Socket, Mc)) == FALSE) {
        continue;
      }

      for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc ++) {
        ChOnSkt = NODECHA_TO_SKTCHA (Mc, ChOnMc);
        if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
          continue;
        }
        SetPatrolErrorDowngrade (Socket, ChOnSkt, PatrolErrorDowngradeEn, &RegAddr, &RegValue);
        MmioAddr = GetCpuCsrAddress (Socket, ChOnSkt, RegAddr, &Size);
        WriteCsrRegTable (MmioAddr, RegValue);
      }
    }
  }
}

/**
  Enable or disable 2LM Correctable Error Logging in m2mem

  @param[in]  Ce2LmLoggingEn      2LM Correctable Error Logging setting.
**/
VOID
EFIAPI
Ce2LmLoggingSetting (
  IN UINT8 Ce2LmLoggingEn
  )
{
  UINT8                          Mc;
  UINT8                          Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

    for (Mc = 0; Mc < MAX_IMC; Mc++) {
      if (IsNodePresent (SKTMC_TO_NODE(Socket, Mc)) == FALSE) {
        continue;
      }
        SetM2mExRasConfigMcaLogMemCorr (Socket, Mc, Ce2LmLoggingEn);
    }
  }
}

/**
  Enable or disable latch first corrected error in KTI

  @param[in]  KtiFirstCeLatchEn   Latch first corrected error setting.
**/
VOID
EFIAPI
KtiFirstCeLatchSetting (
  IN UINT8 KtiFirstCeLatchEn
  )
{
  UINT8                          LinkIndex;
  UINT8                          Skt;
  MEM_TOPOLOGY                   *MemTopology;
  KTI_MCA_N0_KTI_LLPMON_STRUCT   KtiMcaN0Reg;

  MemTopology = GetMemTopology ();
  for (Skt = 0; Skt < MAX_SOCKET; Skt ++) {
    if (IsSocketPresent (Skt) == FALSE) {
      continue;
    }
    for (LinkIndex = 0; LinkIndex < (GetKtiPortCnt()); LinkIndex++) {
      if (!MemTopology->Socket[Skt].UPIPortValid[LinkIndex]) {
        continue;
      }

      KtiMcaN0Reg.Data = ReadCpuCsr (Skt, LinkIndex, KTI_MCA_N0_KTI_LLPMON_REG);
      KtiMcaN0Reg.Bits.latch_first_ce = KtiFirstCeLatchEn;
      WriteCpuCsr (Skt, LinkIndex, KTI_MCA_N0_KTI_LLPMON_REG, KtiMcaN0Reg.Data);
    }
  }
}

/**
  Enable or disable LMCE downgrade

  @param[in]  LmceDowngradeEn     LMCE downgrade downgrade setting.
**/
VOID
EFIAPI
LmceDowngradeSetting (
  IN UINT8 LmceDowngradeEn
  )
{
  UINT8                                       Socket;
  UINT8                                       Mc;
  UINT8                                       ChOnMc;
  UINT8                                       ChOnSkt;
  UINT64                                      MmioAddr;
  UINT32                                      RegAddr;
  UINT32                                      RegValue;
  UINT8                                       Size = 4;


  for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

    for (Mc=0; Mc < MAX_IMC; Mc ++) {
      if (IsNodePresent (SKTMC_TO_NODE(Socket, Mc)) == FALSE) {
        continue;
      }

      for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc ++) {
        ChOnSkt = NODECHA_TO_SKTCHA (Mc, ChOnMc);
        if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
          continue;
        }
        SetLmceDowngrade (Socket, ChOnSkt, LmceDowngradeEn, &RegAddr, &RegValue);
        MmioAddr = GetCpuCsrAddress (Socket, ChOnSkt, RegAddr, &Size);
        WriteCsrRegTable (MmioAddr, RegValue);
      }
    }
  }
}

/**
  Control the signaling of EWB errors as UCNA or SRAO

  @param[in]  LlcEwbErrorControl   LLC EWB Error Control setting.
**/
VOID
EFIAPI
LlcEwbErrorControlSetting (
  IN UINT8 LlcEwbErrorControl
  )
{

}

/**
  Clear Fnv viral Received flag

**/
VOID
EFIAPI
ClearFnvViral (
  VOID
  )
{
  UINT8                                       Socket;
  UINT8                                       Mc;
  UINT8                                       ChOnMc;
  UINT8                                       ChOnSkt;
  UINT64                                      MmioAddr;
  UINT32                                      RegAddr;
  UINT32                                      RegValue;
  UINT8                                       Size = 4;


  for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

    for (Mc=0; Mc < MAX_IMC; Mc ++) {
      if (IsNodePresent (SKTMC_TO_NODE(Socket, Mc)) == FALSE) {
        continue;
      }

      for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc ++) {
        ChOnSkt = NODECHA_TO_SKTCHA (Mc, ChOnMc);
        if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
          continue;
        }
        ClearFnvViralReceivedFlag (Socket, ChOnSkt, &RegAddr, &RegValue);
        MmioAddr = GetCpuCsrAddress (Socket, ChOnSkt, RegAddr, &Size);
        WriteCsrRegTable (MmioAddr, RegValue);
      }
    }
  }
}