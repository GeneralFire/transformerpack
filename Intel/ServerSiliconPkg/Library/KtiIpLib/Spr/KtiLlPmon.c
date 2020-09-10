/** @file
  KTI *_KTI_LLPMON_REG related registers access special for SPR.

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

//
// Include files
//
#include <Uefi.h>
#include <Base.h>
#include "Include/KtiRegs.h"
#include <Library/UsraCsrLib.h>
#include <Guid/KtiIpInterface.h>
#include <Library/EmulationConfigurationLib.h>
#include <KtiSetupDefinitions.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KtiApi.h>
#include <Library/DebugLib.h>
#include <KtiMisc.h>
#include <Library/UpimdfIpAccessLib.h>

UINT32 KtiSncBaseRegisterOffset[MAX_CLUSTERS + 1] = {
  KTILK_SNC_BASE_1_KTI_LLPMON_REG,
  KTILK_SNC_BASE_2_KTI_LLPMON_REG,
  KTILK_SNC_BASE_3_KTI_LLPMON_REG,
  KTILK_SNC_BASE_4_KTI_LLPMON_REG,
  KTILK_SNC_BASE_5_KTI_LLPMON_REG
};

/**
  Returns the total Vendor ID of this KTI LLPMON.
  Called from GetKtiAgentCount() and several other places

  @param SocketId          - Socket ID With KTI LLPMON
  @param Port              - Port ID of the KTI Agent

  @retval 8086 - Intel KTI LLPMON
  @retval FFFF - No KTI LLPMON Present

**/
UINT16
EFIAPI
GetKtiLlpmonVendorId (
  UINT8                      SocketId,
  UINT8                      Port
  )
{
  UINT16 Data16 = 0xFFFF;
  Data16 = (UINT16) UpimdfIpReadAccess32 (SocketId, Port, 0, VID_KTI_LLPMON_REG);
  return Data16;
}

/**
  Routine to program program KTI Agent prefetch enable CSR
  Called by SetupUpiMiniSad()

  @param SocketId               - Socket to program
  @param Port                   - Port to program
  @param BusLocalTgtLst         - target for each enabled stack
  @param BusLocalLimits[2]      - limit for each enabled stack

  @retval None
**/
VOID
EFIAPI
ProgramKtiBusTad (
  IN UINT8                 SocketId,
  IN UINT8                 Port,
  IN UINT32                *BusLocalTgtLst,
  IN UINT32                *BusLocalLimits
  )
{
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_CFG_TARGET0_KTI_LLPMON_REG, BusLocalTgtLst[0]);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_CFG_TARGET1_KTI_LLPMON_REG, BusLocalTgtLst[1]);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_CFG1_KTI_LLPMON_REG, BusLocalLimits[0]);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_CFG2_KTI_LLPMON_REG, BusLocalLimits[1]);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_CFG3_KTI_LLPMON_REG, BusLocalLimits[2]);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_CFG4_KTI_LLPMON_REG, BusLocalLimits[3]);
}

/**
Program Number of CHA per Cluster in UPI Uncore_Snc_Config
This routine is to program UMA Cluster register in Kti Agents
Called from ProgramNumOfChaPerCluster() in a loop for all Sockets and Ports

@param  SocketId            -  Socket Id
@param  Port                -  Port Id
@param  SncConfigData       -  Value program to KTI UNCORE_SNC_CONFIG register

@retval None

**/
VOID
EFIAPI
KtiUncoreChaPerCluster (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN UINT32                   SncConfigData
  )
{
  //
  // Program KTILK_UNCORE_SNC_CONFIG_KTI_LLPMON_REG
  //
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_UNCORE_SNC_CONFIG_KTI_LLPMON_REG, SncConfigData);
}

/**
  Program UMA Cluster register in Kti Agents
  Called from ProgramUmaClusterConfig(), loop for each socket and each port

  @param  SocketId             -  Socket Id
  @param  Port                 -  Port Id
  @param  UmaClusterEnable     -  Enable bit for UMA based clustering mode
  @param  DefeatureXor         -  Defeature Xor

  @retval None

**/
VOID
EFIAPI
ProgramKtiAgentUmaCluster (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN UINT8                    UmaClusterEnable,
  IN UINT8                    DefeatureXor
  )
{
  //
  // Program KTILK_UMA_CLUSTER_CONFIG_KTI_LLPMON_REG
  //
  KTILK_UMA_CLUSTER_CONFIG_KTI_LLPMON_STRUCT  UmaClusterCfg;

  UmaClusterCfg.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILK_UMA_CLUSTER_CONFIG_KTI_LLPMON_REG);
  UmaClusterCfg.Bits.uma_clustering_enable = ((UmaClusterEnable == 0) ? 0 : 3);
  UmaClusterCfg.Bits.defeature_xor = DefeatureXor;
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_UMA_CLUSTER_CONFIG_KTI_LLPMON_REG, UmaClusterCfg.Data);
}

/**
  Routine to program SNC Upper Base in KTI Agent
  called by ProgramSncUpperBaseFor52BitExtend() on cold and warm reset flows

  @param SocketId          - Socket Id
  @param Port              - Port Id
  @param SncUpperBase      - SNC Upper Base value

**/
VOID
EFIAPI
KtiAgentSetSncUpperBase (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN UINT32                   SncUpperBase
  )
{
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_SNC_BASE_UPPER_KTI_LLPMON_REG, SncUpperBase);
}

/**
Program SNC_Base_1 and SNC_BASE_2, for 2LM case
Called from Program2LMHashRegister()

@param  SocketId             -  Socket Id
@param  Port                 -  Port Id
@param  TwoLM_Address_Mask   -  Pointer to KTI_AGENT_2LM_ADDRESS_MASK

@retval None
**/
VOID
EFIAPI
ProgramKtiAgent2LMAddrMask (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN UINT32                   *TwoLM_Address_Mask
) {
  KTILK_SNC_BASE_1_KTI_LLPMON_STRUCT  SncBase1_Kti;
  KTILK_SNC_BASE_2_KTI_LLPMON_STRUCT  SncBase2_Kti;

  SncBase1_Kti.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILK_SNC_BASE_1_KTI_LLPMON_REG);
  SncBase1_Kti.Bits.two_lm_mask_45_40 = ((KTI_AGENT_2LM_ADDRESS_MASK_10nm*)TwoLM_Address_Mask)->Address_Mask_45_40;
  SncBase1_Kti.Bits.hi_two_lm_mask_51_46 = ((KTI_AGENT_2LM_ADDRESS_MASK_10nm*)TwoLM_Address_Mask)->Address_Mask_51_46;
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_SNC_BASE_1_KTI_LLPMON_REG, SncBase1_Kti.Data);

  SncBase2_Kti.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILK_SNC_BASE_2_KTI_LLPMON_REG);
  SncBase2_Kti.Bits.two_lm_mask_28 = ((KTI_AGENT_2LM_ADDRESS_MASK_10nm*)TwoLM_Address_Mask)->Address_Mask_39_28 & 1;
  SncBase2_Kti.Bits.two_lm_mask_39_29 = ((KTI_AGENT_2LM_ADDRESS_MASK_10nm*)TwoLM_Address_Mask)->Address_Mask_39_28 >> 1;
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_SNC_BASE_2_KTI_LLPMON_REG, SncBase2_Kti.Data);
}

/**
  Program all SNC_BASE for all clusters, called on warm-reset-path only
  Called from ConfigureSncMode()

  @param SocketId          - Socket ID
  @param Port              - Port ID
  @param Cluster           - Cluster Index
  @param SncBase           - Cluster SNC Base

**/
VOID
EFIAPI
KtiAgentSetSncBaseAddr (
  IN UINT8  SocketId,
  IN UINT8  Port,
  IN UINT8  Cluster,
  IN UINT32 ClusterMemBase
  )
{
  UINT32  SncBase;

  SncBase = UpimdfIpReadAccess32 (SocketId, Port, 0, KtiSncBaseRegisterOffset[Cluster]);
  SncBase &= 0xFFFF0000;
  SncBase |= ClusterMemBase & 0x0000FFFF;
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KtiSncBaseRegisterOffset[Cluster], SncBase);
  DEBUG ((DEBUG_INFO, "\nS%d Cluster%d KTI%d SncBase=0x%x\n", SocketId, Cluster, Port, SncBase));
  return;
}

/**
Dump SNC related register in Kti Agents

This routine is to dump SNC related register in Kti Agents
@param  SocketId             - Socket Id
@param  Port                 - Port ID

@retval None
**/
VOID
EFIAPI
DumpKtiLlPmonSncXptRegister (
  UINT8 SocketId,
  UINT8 Port
) {
  UINT8 Cluster;
  DEBUG ((DEBUG_INFO, "\nKTI_%u_SNC_CONFIG_KTI : 0x%08X", Port, UpimdfIpReadAccess32 (SocketId, Port, 0, KTILK_SNC_CONFIG_KTI_LLPMON_REG)));
  DEBUG ((DEBUG_INFO, "\nKTI_%u_KTIAGCTRL      : 0x%08X", Port, UpimdfIpReadAccess32 (SocketId, Port, 0, KTIAGCTRL_KTI_LLPMON_REG)));

  for (Cluster = 0; Cluster <= MAX_CLUSTERS; Cluster++) {
    DEBUG ((DEBUG_INFO, "\nKTI_%u_KTI_SNC_BASE_%x : 0x%08X", Port, (Cluster + 1), UpimdfIpReadAccess32 (SocketId, Port, 0, KtiSncBaseRegisterOffset[Cluster])));
  }
}

/**
  Program MMIOL TAD CSRs for each KTI Port

  @param SocketId               - Socket to program
  @param Port                   - Port to program
  @param CpuResourceMap         - Pointer to the CPU Resource Map for this Socket

  @retval None
**/
VOID
EFIAPI
ProgramKtiMmiolTad (
  IN UINT8                 SocketId,
  IN UINT8                 Port,
  KTI_CPU_RESOURCE         *CpuResourceMap
  )
{
  KTILK_TAD_MMIOL_TARGET0_KTI_LLPMON_STRUCT MmiolLocalTgt0Lst;
  KTILK_TAD_MMIOL_TARGET1_KTI_LLPMON_STRUCT MmiolLocalTgt1Lst;
  KTILK_TAD_MMIOL1_KTI_LLPMON_STRUCT  MmiolLocalLimits[6];
  KTILK_TAD_UBOX_KTI_LLPMON_STRUCT    KtiLkTadUbox;
  UINT32  KtilkTadMmillRegList[] = {
                                      KTILK_TAD_MMIOL1_KTI_LLPMON_REG,
                                      KTILK_TAD_MMIOL2_KTI_LLPMON_REG,
                                      KTILK_TAD_MMIOL3_KTI_LLPMON_REG,
                                      KTILK_TAD_MMIOL4_KTI_LLPMON_REG,
                                      KTILK_TAD_MMIOL5_KTI_LLPMON_REG,
                                      KTILK_TAD_MMIOL6_KTI_LLPMON_REG
                                    };
  UINT8 Stack;
  UINT8 Idx;
  UINT8 TargetIndex;
  UINT32 MmiolBaseTmp;
  UINT32 MmiolLimit;

  TargetIndex = 0;
  MmiolLocalTgt0Lst.Data = 0;
  MmiolLocalTgt1Lst.Data = 0;
  KtiLkTadUbox.Data = 0;
  ZeroMem ((UINT8 *) &MmiolLocalLimits, sizeof (KTILK_TAD_MMIOL1_KTI_LLPMON_STRUCT) * 6);

  MmiolLimit = CpuResourceMap->StackRes[0].MmiolLimit >> 22;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    //
    // Program a target and limit for each enabled stack
    //
    if (StackPresent (SocketId, Stack)) {
      MmiolBaseTmp = CpuResourceMap->StackRes[Stack].MmiolBase;
      if (MmiolBaseTmp != 0xFFFFFFFF) {
        if (TargetIndex < 8) {
          MmiolLocalTgt0Lst.Data |= (Stack << (TargetIndex * 4));
        } else {
          MmiolLocalTgt1Lst.Data |= (Stack << ((TargetIndex - 8) * 4));
        }

        MmiolLimit = CpuResourceMap->StackRes[Stack].MmiolLimit >> 22;
        ASSERT ((TargetIndex / 2) < (sizeof (MmiolLocalLimits) / sizeof (MmiolLocalLimits[0])));//, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_7F);
        if (TargetIndex % 2) {
          MmiolLocalLimits[TargetIndex / 2].Bits.mmiol_limit1 = MmiolLimit;
        } else {
          MmiolLocalLimits[TargetIndex / 2].Bits.mmiol_limit0 = MmiolLimit;
        }
        TargetIndex++;
      }
    }
  }

  //
  // Program unused stack limit to the last stack limit because UBOX mmiol base uses last stack mmiol limit as base.
  //
  if (TargetIndex < MAX_IIO_STACK) {
    for (Stack = TargetIndex; Stack < MAX_IIO_STACK; Stack++) {
      ASSERT ((TargetIndex / 2) < (sizeof(MmiolLocalLimits) / sizeof(MmiolLocalLimits[0])));//, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_7F);
      if (TargetIndex % 2) {
        MmiolLocalLimits[TargetIndex / 2].Bits.mmiol_limit1 = MmiolLimit;
      }
      else {
        MmiolLocalLimits[TargetIndex / 2].Bits.mmiol_limit0 = MmiolLimit;
      }
      TargetIndex++;
    }
  }

  KtiLkTadUbox.Bits.ubox_mmiol_limit = CpuResourceMap->MmiolLimit >> 22;

  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_MMIOL_TARGET0_KTI_LLPMON_REG, MmiolLocalTgt0Lst.Data);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_MMIOL_TARGET1_KTI_LLPMON_REG, MmiolLocalTgt1Lst.Data);
  for (Idx = 0; Idx < sizeof (KtilkTadMmillRegList) / sizeof (KtilkTadMmillRegList[0]); Idx++) {
    UpimdfIpWriteAccess32(SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KtilkTadMmillRegList[Idx], MmiolLocalLimits[Idx].Data);
  }

  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_UBOX_KTI_LLPMON_REG, KtiLkTadUbox.Data);
}

/**
  Program MMIOH TAD CSRs for each KTI Port

  @param SocketId               - Socket to program
  @param Port                   - Port to program
  @param CpuResourceMap         - Pointer to the CPU Resource Map for this Socket

  @retval None
**/
VOID
EFIAPI
ProgramKtiMmiohTad (
  IN UINT8                 SocketId,
  IN UINT8                 Port,
  KTI_CPU_RESOURCE         *CpuResourceMap
  )
{
  UINT8 Stack;
  UINT64_STRUCT MmiohBaseTmp;
  UINT64_STRUCT TempMmiohLimit;
  UINT32 MmiohLimit;
  UINT8 TargetIndex;
  KTILK_TAD_MMIOH_TARGET0_KTI_LLPMON_STRUCT MmiohLocalTgt0Lst;
  KTILK_TAD_MMIOH_TARGET1_KTI_LLPMON_STRUCT MmiohLocalTgt1Lst;
  KTILK_TAD_MMIOH1_KTI_LLPMON_STRUCT MmiohLocalLimits[12];
  UINT32        KtiMmiohTargetOffset[12] = {
    KTILK_TAD_MMIOH1_KTI_LLPMON_REG,
    KTILK_TAD_MMIOH2_KTI_LLPMON_REG,
    KTILK_TAD_MMIOH3_KTI_LLPMON_REG,
    KTILK_TAD_MMIOH4_KTI_LLPMON_REG,
    KTILK_TAD_MMIOH5_KTI_LLPMON_REG,
    KTILK_TAD_MMIOH6_KTI_LLPMON_REG,
    KTILK_TAD_MMIOH7_KTI_LLPMON_REG,
    KTILK_TAD_MMIOH8_KTI_LLPMON_REG,
    KTILK_TAD_MMIOH9_KTI_LLPMON_REG,
    KTILK_TAD_MMIOH10_KTI_LLPMON_REG,
    KTILK_TAD_MMIOH11_KTI_LLPMON_REG,
    KTILK_TAD_MMIOH12_KTI_LLPMON_REG
  };

  ZeroMem ((UINT8 *)&MmiohLocalLimits, sizeof (KTILK_TAD_MMIOH1_KTI_LLPMON_STRUCT) * 12);
  MmiohLocalTgt0Lst.Data = 0;
  MmiohLocalTgt1Lst.Data = 0;
  TargetIndex = 0;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    //
    // Program a target and limit for each enabled stack
    //
    if (StackPresent (SocketId, Stack)) {
      MmiohBaseTmp = CpuResourceMap->StackRes[Stack].MmiohBase;
      if ((MmiohBaseTmp.hi != 0xFFFFFFFF) && (MmiohBaseTmp.lo != 0xFFFFFFFF)) {
        if (TargetIndex < 8) {
          MmiohLocalTgt0Lst.Data |= (Stack << (TargetIndex * 4));
        } else {
          MmiohLocalTgt1Lst.Data |= (Stack << ((TargetIndex - 8) * 4));
        }

        TempMmiohLimit = CpuResourceMap->StackRes[Stack].MmiohLimit;
        MmiohLimit = TempMmiohLimit.lo >> 30;
        MmiohLimit |= TempMmiohLimit.hi << 2;

        ASSERT (TargetIndex < (sizeof (MmiohLocalLimits) / sizeof (MmiohLocalLimits[0])));//, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_80);
        MmiohLocalLimits[TargetIndex].Bits.mmioh_limit0 = MmiohLimit;
        TargetIndex++;
      }
    }
  }

  TargetIndex = 0;
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_MMIOH_TARGET0_KTI_LLPMON_REG, MmiohLocalTgt0Lst.Data);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_MMIOH_TARGET1_KTI_LLPMON_REG, MmiohLocalTgt1Lst.Data);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILK_TAD_UBOX_MMIOH_BASE_KTI_LLPMON_REG, (CpuResourceMap->MmiohBase.hi) << 10);
  for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
    if ((CpuResourceMap->StackRes[Stack].Personality == TYPE_UBOX_IIO)
      || (CpuResourceMap->StackRes[Stack].Personality == TYPE_HFI)
      || (CpuResourceMap->StackRes[Stack].Personality == TYPE_DINO))
    {
      ASSERT (Stack < (sizeof (KtiMmiohTargetOffset) / sizeof (KtiMmiohTargetOffset[0])));//, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_81);
      ASSERT (Stack < (sizeof (MmiohLocalLimits) / sizeof (MmiohLocalLimits[0])));//, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_82);
      UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KtiMmiohTargetOffset[TargetIndex], MmiohLocalLimits[TargetIndex].Data);
      TargetIndex++;
    }
  }
}

/**
  Routine to program program KTI Agent prefetch enable CSR
  Called by SetupUpiMiniSad()

  @param SocketId              - Socket Id to program
  @param Port                  - Port to program
  @param RdCurForXptPrefetchEn - Setup option RdCur for XPT Prefetch
  @param KtiOrRemoteXptPrefetchEnable:
           0: KtiXptPrefetch Enabled
           1: RemoteXptPrefetch Enabled
           2: KtiXptPrefetch Disabled
           3: RemoteXptPrefetch Disabled

  @retval None
**/
VOID
EFIAPI
ProgramKtiAgentPrefetch (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN UINT8                    RdCurForXptPrefetchEn,
  IN UINT8                    KtiOrRemoteXptPrefetchEnable
  )
{
  KTIAGCTRL_KTI_LLPMON_STRUCT KtiAgCtrl;
  KtiAgCtrl.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTIAGCTRL_KTI_LLPMON_REG);

  if (KtiOrRemoteXptPrefetchEnable == KTI_XPT_PREFETCH_DISABLE) {
    KtiAgCtrl.Bits.enableprefetch = 0;
  } else if (KtiOrRemoteXptPrefetchEnable == REMOTE_XPT_PREFETCH_DISABLE) {
    KtiAgCtrl.Bits.xpt_prefetch_enable = 0;
  } else if (KtiOrRemoteXptPrefetchEnable == KTI_XPT_PREFETCH_EN) {
    KtiAgCtrl.Bits.enableprefetch = 1;
    if (RdCurForXptPrefetchEn < KTI_AUTO) {
      if (RdCurForXptPrefetchEn == KTI_ENABLE) {
        KtiAgCtrl.Bits.suppress_mem_rd_prefetch_rdcur = 0;          //enable RdCur
      } else {
        KtiAgCtrl.Bits.suppress_mem_rd_prefetch_rdcur = 1;          //disable RdCur
      }
    }
  } else {
    //
    // Program xpt_prefetch_enable for XptRemote Prefetch
    //
    if (KtiAgCtrl.Bits.xpt_prefetch_enable == 1) {
      // already enabled, no need to program it again
      return ;
    }
    KtiAgCtrl.Bits.xpt_prefetch_enable = 1;
  }
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTIAGCTRL_KTI_LLPMON_REG, KtiAgCtrl.Data);
}

/**
  Routine to collect and clear KTI Agent errors

  @param SocketId               - Socket to program
  @param Port                   - Port to program
  @param LinkValid              - Is there a valid link on this port - True/False
  @param OutKtiFpgaPresent      - Is FPGA Present for this socket - True/False
  @param OutKtiFpgaEnable       - Is FPGA enabled by KTIRC - True/False
  @param SocketFpgasInitialized - Is FPGA Initialized - True/False (SocketFpgasInitialized() adds a scratchpad read each time this function is called)

  @retval 0     - Successful
  @retval Other - failure

**/
VOID
EFIAPI
CollectAndClearKtiPortErrors (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN BOOLEAN                  LinkValid,
  IN BOOLEAN                  OutKtiFpgaPresent,
  IN BOOLEAN                  OutKtiFpgaEnable,
  IN BOOLEAN                  SocketFpgasInitialized
  )
{
  UINT32 Data32;
  KTIDBGERRSTDIS1_KTI_CIOPHYDFX_STRUCT   KtiDbgErrStDis1;
  KTIERRCNT0_MASK_KTI_LLPMON_STRUCT      KtiErrCnt0Mask;
  KTIERRCNT1_MASK_KTI_LLPMON_STRUCT      KtiErrCnt1Mask;
  KTIERRCNT2_MASK_KTI_LLPMON_STRUCT      KtiErrCnt2Mask;
  BOOLEAN                                IsUpiMdfPort;

  IsUpiMdfPort = IsUpiPortUpimdf (SocketId, Port);

  if (LinkValid != TRUE) {
    if (!(UbiosGenerationOrHsleEnabled ())) {
      //
      // Link is unused, disable all error reporting for it
      //
      Data32 = 0xFFFFFFFF;
      if (!IsUpiMdfPort) {
        UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTIERRDIS0_KTI_LLPMON_REG, Data32);
      }
      UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTIERRDIS1_KTI_LLPMON_REG, Data32);
    }
  }

  if (LinkValid == TRUE && !IsUpiMdfPort) {
    //
    // A0 Stepping workaround: s4929014
    // B0 WA: s5371481
    // All bits in disable_ctrl_flowqoverunder were set before programming KTIA2RCRCTRL and need to
    // be cleared after WR.
    //
    KtiDbgErrStDis1.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTIDBGERRSTDIS1_KTI_CIOPHYDFX_REG);
    KtiDbgErrStDis1.Bits.disable_ctrl_flowqoverunder = 0;
    UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTIDBGERRSTDIS1_KTI_CIOPHYDFX_REG, KtiDbgErrStDis1.Data);
  }

  //
  // (SKX A0): s5330420: UPI PO : Set UPI Error Counter0/1/2 Masks
  // (SKX B0): s5370330
  //
  KtiErrCnt0Mask.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTIERRCNT0_MASK_KTI_LLPMON_REG);
  KtiErrCnt0Mask.Bits.mask = 0x1000;
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTIERRCNT0_MASK_KTI_LLPMON_REG, KtiErrCnt0Mask.Data);

  //
  //s5370330
  //
  KtiErrCnt1Mask.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTIERRCNT1_MASK_KTI_LLPMON_REG);
  KtiErrCnt1Mask.Bits.mask = 0x13c000;
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTIERRCNT1_MASK_KTI_LLPMON_REG, KtiErrCnt1Mask.Data);

  if (!IsUpiMdfPort) {
    KtiErrCnt2Mask.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTIERRCNT2_MASK_KTI_LLPMON_REG);
    KtiErrCnt2Mask.Bits.mask = 0x9e7;
    UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTIERRCNT2_MASK_KTI_LLPMON_REG, KtiErrCnt2Mask.Data);
  }
}

/**
  Program KTIQRT CSR route
  Called from ProgramM3KtiAndKtiAgentRouteTable()

  @param SocketId   - Socket Id
  @param Port       - Port Id
  @param KtiAgentRt - encoded route from KtiAgentRt in KTI_CPU_RT structure
  @param ProgramPeMode     - PE bit setting. True/False.

  @retval None
**/
VOID
EFIAPI
ProgramKtiAgentRouteTable (
  UINT8  SocketId,
  UINT8  Port,
  UINT32 KtiAgentRt,
  BOOLEAN  ProgramPeMode
  )
{
  //
  // Per HSD-ES 2205994811 request, need to replicate lower 8 bits in upper 8 bits for ktiqrt.routeskt,
  // when PE mode is enable.
  //
  if (ProgramPeMode == TRUE) {
    KtiAgentRt |= (KtiAgentRt << 8);
  }

  UpimdfIpWriteAccess32 ((UINT8)SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTIQRT_KTI_LLPMON_REG, KtiAgentRt);
}

/**
  Set the enable_targeting_iioX of KTILK_TAD_IIO_STACKID_ENABLE_KTI_LLPMON_REG

  @param SocId       - CPU Socket Node number (Socket ID)
  @param Port        - UPI PORT
  @param StackBitmap - Stack Bitmap
**/
VOID
EFIAPI
ProgramKtiIioStackIdEnable (
  UINT8                 SocId,
  UINT8                 Port,
  UINT32                StackBitmap
  )
{
  //
  // 1p0 crif WA:
  // The register is moved to SV_HOOK, and can only be accessed by P_U_CODE, need to confirm.
  //
  //UINT32    FieldMask = 0;

  //FieldMask = FLD_MASK (MAX_IIO_STACK, 0);
  //StackBitmap &= FieldMask;
  //UpimdfIpWriteAccess32 (SocId, Port, KTILK_TAD_IIO_STACKID_ENABLE_KTI_LLPMON_REG, StackBitmap);
}

/**
  Routine to set the mask / enable the COR PHY RESET error in cor_mask & cor_cnt_mask of
  KTIERRDIS0_KTI_LLPMON_REG

  @param SocketId               - Socket to program
  @param Port                   - Port to program
  @param MaskFlag               - It is used to indicate if it is going to mask the COR PHY RESET error in
                                  cor_mask & cor_cnt_mask; or enable the COR PHY RESET error in cor_mask &
                                  cor_cnt_mask

  @retval

**/
VOID
EFIAPI
ConfiguCorPhyResetErrOnKtiErrDis0 (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port,
  IN  BOOLEAN                  MaskFlag
  )
{
  KTIERRDIS0_KTI_LLPMON_STRUCT     KtiErrDis0;

  if (IsUpiPortUpimdf (SocketId, Port)) {
    //
    // SPR-UCC: UPIMDF does not have this register.
    //
    return;
  }

  KtiErrDis0.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTIERRDIS0_KTI_LLPMON_REG);
  if (MaskFlag) {
    KtiErrDis0.Bits.cor_mask |= COR_PHY_RESET_BIT_IN_KTIERRDIS0;
    KtiErrDis0.Bits.cor_cnt_mask |= COR_PHY_RESET_BIT_IN_KTIERRDIS0;
  } else {
    KtiErrDis0.Bits.cor_mask &= ~ COR_PHY_RESET_BIT_IN_KTIERRDIS0;
    KtiErrDis0.Bits.cor_cnt_mask &= ~ COR_PHY_RESET_BIT_IN_KTIERRDIS0;
  }
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTIERRDIS0_KTI_LLPMON_REG, KtiErrDis0.Data);
}


/**
 * Locks or Unlocks PHY CSR block of registers.
 * dfxuniphy registers are locked for security reasons. Registers must be unlocked
 * before attempting to write CSR
 * This is single function to handle both Lock and Unlock request. The 3rd parameter
 * is boolean with the following logic:
 * TURE = Locks the PHY registers [Writes 1 to dfxuniphylck]
 * FALSE = Unlocks the PHY registers [Writes 0 to dfxuniphylck]
 *
 * @param[in] Socket - Socket Number to be programmed
 * @param[in] KTI Link - KTI Link to be programmed
 * @param[in] LockManager - TRUE = Lock; FALSE = Unlock
 **/

VOID
PhyCsrLockManager (
  IN    UINT8                     SocketIndex,
  IN    UINT8                     LinkIndex,
  IN    BOOLEAN                   LockManager
  )
{

}

/**
  Increasing the L0p lock time to workaround the issue where UPI L0p transitions
  appear to be hitting a "phase wander" condition where the inactive lanes are
  not tracking like the active ones are and upon exit a relock is necessary
  creating a correctable CRC.

  @param[in] SocketId      Socket ID
  @param[in] Port          UPI Port number
**/
VOID
EFIAPI
ExtendKtiL0pExitTime (
  IN  UINT8                    SocketId,
  IN  UINT8                    Port
  )
{
  //
  // Empty function to keep API consistency
  //
}

/**
  Routine to program D2C register

  @param SocketId          - Socket to program
  @param Port              - Port to program
  @param D2cEnable         - enable or disable D2C for this KTI Port

  @retval None

**/
VOID
EFIAPI
ProgramD2CRegister (
  IN UINT8                    SocketId,
  IN UINT8                    Port,
  IN BOOLEAN                  D2cEnable
  )
{
  KTIAGCTRL_KTI_LLPMON_STRUCT         KtiAgCtrl;
  KTILKPRIVC3_KTI_CIOPHYDFX_STRUCT    KtiLkPrivc3;

  KtiAgCtrl.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTIAGCTRL_KTI_LLPMON_REG);
  KtiLkPrivc3.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILKPRIVC3_KTI_CIOPHYDFX_REG);
  if (D2cEnable == TRUE) {
    KtiAgCtrl.Bits.rbt_rd_enable = 1;
    KtiLkPrivc3.Bits.rbt_wr_enable = 1;
  } else {
    KtiAgCtrl.Bits.rbt_rd_enable = 0;
    KtiLkPrivc3.Bits.rbt_wr_enable = 0;
  }
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTIAGCTRL_KTI_LLPMON_REG, KtiAgCtrl.Data);
  UpimdfIpWriteAccess32 (SocketId, Port, UPIMDF_INSTANCE_BROADCAST, KTILKPRIVC3_KTI_CIOPHYDFX_REG, KtiLkPrivc3.Data);
}

/**
  Get the KTI Link Layer CRC setting
  called from PrimeHostStructurePhyLinkLayer():

  @param IN  SocketId      - Socket ID
  @param IN  Port          - Link number

  @retval OutKtiCrcMode    - KTI RC determined CRC mode (CRC_MODE_16BIT, or CRC_MODE_ROLLING_32BIT)

**/
UINT32
EFIAPI
GetCrcMode (
  UINT8                SocketId,
  UINT8                Port
  )
{
  KTILCP_KTI_LLPMON_SPRA0_SPRB0_SPRHBM_SPRC0_SPRMCC_STRUCT       KtiLcp;
  KtiLcp.Data = UpimdfIpReadAccess32 (SocketId, Port, 0, KTILCP_KTI_LLPMON_REG);
  return KtiLcp.Bits.crc_mode_supported;
}
