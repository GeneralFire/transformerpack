/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <KtiMisc.h>
#include <KtiSetupDefinitions.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <UncoreCommonIncludes.h>
#include <SiliconSetting.h>
#include <Library/KtiApi.h>

KTI_DFX_PARM DfxParmDefaults = {
  0,             // DfxSystemWideParmStart
  KTI_AUTO,      // DfxHaltLinkFailReset
  KTI_AUTO,      // DfxKtiMaxInitAbort
  KTI_AUTO,      // DfxLlcShareDrdCrd
  BIAS_FWD_AUTO, // DfxBiasFwdMode
  KTI_AUTO,      // DfxSnoopFanoutEn
  KTI_AUTO,      // DfxHitMeEn
  KTI_AUTO,      // DfxFrcfwdinv
  KTI_AUTO,      // DfxDbpEnable
  KTI_AUTO,      // DfxCleanEvictAlwaysLive
  KTI_AUTO,      // DfxModifiedEvictAlwaysLive
  KTI_AUTO,      // DfxOsbEn
  KTI_AUTO,      // DfxHitMeRfoDirsEn
  KTI_AUTO,      // DfxGateOsbIodcAllocEn
  KTI_AUTO,      // DfxDualLinksInterleavingMode
  DEGRADE_SUPPORTED_TOPOLOGY, // DfxSystemDegradeMode
  KTI_AUTO,      // DfxVn1En
  KTI_AUTO,      // DfxD2cEn
  KTI_AUTO,      // DfxD2kEn
  LOCK_MASTER_AUTO, // DfxLockMaster
  KTI_AUTO,      // DfxOsbLocRd
  KTI_AUTO,      // DfxOsbLocRdCur
  KTI_AUTO,      // DfxOsbRmtRd
  KTI_AUTO,      // DfxM3KtiCountMismatchEn
  KTI_AUTO,      // DfxSnoopFanoutChaInterleaveEn
  XPT_FIFO_CREDIT_AUTO, // DfxXptFifoEnabledCredit
  KTI_AUTO,      // DfxClockModulationEn
  KTI_AUTO,      // DfxMdfisAdvancedTrainingEn
  CXL_SECURITY_AUTO, //DfxCxlSecLvl
  KTI_AUTO,      // DfxCxlStcge
  KTI_AUTO,      // DfxCxlSdcge
  KTI_AUTO,      // DfxCxlDlcge
  KTI_AUTO,      // DfxCxlLtcge
  KTI_AUTO,      // DfxCxlVid
  KTI_PMON_CONFIG_FULL,//DfxPmonConfig
  KTI_PMON_ACC_CTRL_AUTO,//DfxM2IosfPmonAccessControl
  KTI_DISABLE, //DfxIioDfxEnabled
  DFX_IIO_STACK_EN_AUTO, //DfxIioStackEn;
  KTI_AUTO,       //DfxHqmEn;
  0              // DfxSystemWideParmEnd
};


/**

  Update KTI specific default policy into KtiHostInDataHob

  @param none

  @retval VOID

**/
VOID
GetKtiSetupOptions (
  VOID
  )
{
  UINT8         Ctr1;
  UINT8         Ctr2;
  KTI_HOST_IN   *KtiHostInPtr;

  KtiHostInPtr  = KTI_HOST_IN_PTR;


  //
  // Fill-in the system wide options
  //
  for (Ctr1 = 0; Ctr1 < MAX_SOCKET; Ctr1++) {
    //
    // Assign resources equally among all IIOs
    //
    KtiHostInPtr->BusRatio[Ctr1] = BUS_RATIO_DFLT;
    //
    // Default enable discovered Fpgas
    //
    KtiHostInPtr->KtiFpgaEnable[Ctr1] = KTI_AUTO;
  }

  KtiHostInPtr->D2KCreditConfig       = KTI_D2K_MED;          // D2K credit default setting.
  KtiHostInPtr->SnoopThrottleConfig   = KTI_STC_AUTO;         // Snoop throttle default setting.
  KtiHostInPtr->LegacyVgaSoc      = LEGACY_VGA_SOCKET_DFLT;   // Legacy VGA range target socket
  KtiHostInPtr->LegacyVgaStack    = LEGACY_VGA_STACK_DFLT;    // Legacy VGA range target stack
  KtiHostInPtr->P2pRelaxedOrdering = KTI_DISABLE;             // Disable PCIE P2P performance mode
  KtiHostInPtr->DebugPrintLevel   = KTI_DEBUG_PRINT_ALL;      // Enable all levels of debug messages
  KtiHostInPtr->IoDcMode          = IODC_AUTO;                // Auto by default, depends on topology
  KtiHostInPtr->DegradePrecedence = TOPOLOGY_PRECEDENCE;
  KtiHostInPtr->Degrade4SPreference = TOPOLOGY_4S_FULLY_CONNECT;
  KtiHostInPtr->DirectoryModeEn   = KTI_ENABLE;
  KtiHostInPtr->SncEn             = KTI_ENABLE;               // Snc Enabled
  KtiHostInPtr->XptPrefetchEn     = KTI_AUTO;                 // XptPrefetch enabled
  KtiHostInPtr->KtiPrefetchEn     = KTI_AUTO;                 // KtiPrefetch enabled
  KtiHostInPtr->XptRemotePrefetchEn  = KTI_AUTO;              // XptRemotePrefetch enabled
  KtiHostInPtr->RdCurForXptPrefetchEn = KTI_AUTO;
  KtiHostInPtr->TscSyncEn         = KTI_AUTO;
  KtiHostInPtr->StaleAtoSOptEn    = KTI_AUTO;
  KtiHostInPtr->LLCDeadLineAlloc  = KTI_ENABLE;
  KtiHostInPtr->SplitLock         = KTI_DISABLE;
  KtiHostInPtr->DdrtQosMode         = KTI_DISABLE;
  KtiHostInPtr->TorThresLoctoremNorm = THRESHOLDS_AUTO;     // 0 -- disable, 1 -- Auto, 2 -- Low, 3 -- Med, 4 -- High
  KtiHostInPtr->TorThresLoctoremEmpty = THRESHOLDS_AUTO;    // 0 -- disable, 1 -- Auto, 2 -- Low, 3 -- Med, 4 -- High

  //
  // Phy/LL system wide settings
  //
  KtiHostInPtr->KtiLinkSpeedMode         = KTI_LINK_FULL_SPEED_MODE;
  KtiHostInPtr->KtiLinkSpeed             = MAX_KTI_LINK_SPEED;
  KtiHostInPtr->KtiAdaptationEn          = KTI_DISABLE;
  KtiHostInPtr->KtiAdaptationSpeed       = SPEED_REC_160GT;
  KtiHostInPtr->KtiLinkL0pEn             = KTI_AUTO;
  KtiHostInPtr->KtiLinkL1En              = KTI_AUTO;
  KtiHostInPtr->KtiFailoverEn            = KTI_AUTO;
  KtiHostInPtr->KtiLbEn                  = KTI_DISABLE;
  KtiHostInPtr->KtiCrcMode               = CRC_MODE_AUTO;
  KtiHostInPtr->KtiCpuSktHotPlugEn       = KTI_DISABLE;
  KtiHostInPtr->KtiCpuSktHotPlugTopology = HOT_PLUG_TOPOLOGY_4S;
  KtiHostInPtr->KtiSkuMismatchCheck      = KTI_ENABLE;
  //
  // Phy/LL per link settings
  //
  for (Ctr1 = 0; Ctr1 < MAX_SOCKET; Ctr1++) {
    for (Ctr2 = 0; Ctr2 < (GetKtiPortCnt()); Ctr2++){
      KtiHostInPtr->PhyLinkPerPortSetting[Ctr1].Link[Ctr2].KtiPortDisable = KTI_DISABLE; //Assume all ports are enabled
      if (KtiHostInPtr->KtiLinkSpeed < FREQ_PER_LINK) {
        KtiHostInPtr->PhyLinkPerPortSetting[Ctr1].Phy[Ctr2].KtiLinkSpeed = KtiHostInPtr->KtiLinkSpeed;
      } else {
        KtiHostInPtr->PhyLinkPerPortSetting[Ctr1].Phy[Ctr2].KtiLinkSpeed = SPEED_REC_96GT;
      }

      KtiHostInPtr->PhyLinkPerPortSetting[Ctr1].Link[Ctr2].KtiLinkVnaOverride = KTI_LL_VNA_SETUP_MAX;
    }
  }

  //
  // Copy system-wide Dfx options.  These should not be changed
  //
  CopyMem ((UINT8*)&KtiHostInPtr->DfxParm, (UINT8*)&DfxParmDefaults, sizeof (KTI_DFX_PARM));

  //
  // Copy DFX Phy/LL per link settings
  //
  for (Ctr1 = 0; Ctr1 < MAX_SOCKET; Ctr1++) {
    for (Ctr2 = 0; Ctr2 < (GetKtiPortCnt()); Ctr2++){
      KtiHostInPtr->DfxCpuCfg[Ctr1].Link[Ctr2].DfxCrcMode = CRC_MODE_AUTO;
      KtiHostInPtr->DfxCpuCfg[Ctr1].Link[Ctr2].DfxL0pEnable = KTI_AUTO;
      KtiHostInPtr->DfxCpuCfg[Ctr1].Link[Ctr2].DfxL1Enable = KTI_AUTO;
      KtiHostInPtr->DfxCpuCfg[Ctr1].Phy[Ctr2].DfxKtiFailoverEn = KTI_AUTO;
    }
  }

  KtiHostInPtr->WaSerializationEn = 0;

  return;
}

