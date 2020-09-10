/** @file
  This file is PeiUpiPolicyLib library creates default settings of RC
  Policy and installs RC Policy PPI.

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
#include <KtiSetupDefinitions.h>
#include <Ppi/UpiPolicyPpi.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <SiliconSetting.h>
#include <Upi/KtiSi.h>
#include <Library/PerformanceLib.h>
#include <Library/KtiApi.h>
#include <PlatformInfoTypes.h>

#define WAIT_TIME_FOR_PSBP_DEFAULT  30000    // Wait for a maximum of 30 sec

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
  KTI_PMON_CONFIG_AUTO,//DfxPmonConfig
  KTI_PMON_ACC_CTRL_AUTO,//DfxM2IosfPmonAccessControl
  KTI_DISABLE, //DfxIioDfxEnabled
  DFX_IIO_STACK_EN_AUTO, //DfxIioStackEn;
  KTI_AUTO,       //DfxHqmEn;
  0              // DfxSystemWideParmEnd
};

/**
  UpiCreatePolicyDefaults generates the default Upi Pei Policy.

  @param[in, out] UpiPolicyPpi          The pointer to UPI Policy PPI instance

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
UpiCreatePolicyDefaults (
  IN OUT UPI_POLICY_PPI            *UpiPolicyPpi
  )
{
  UINT8              Ctr1, Ctr2;
  UINT8              KtiPortCnt;
  BOOLEAN            IsSnrOrIcxd;

  IsSnrOrIcxd = IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL) ||
                IsCpuAndRevision (CPU_TNR, REV_ALL);

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    KtiPortCnt = SI_MAX_KTI_PORTS_SPR;
  } else if (IsCpuAndRevision (CPU_GNRSP, REV_ALL) || IsCpuAndRevision (CPU_SRFSP, REV_ALL)) {
    KtiPortCnt = SI_MAX_KTI_PORTS_GNRSRF;
  } else if (IsSnrOrIcxd) {
    KtiPortCnt = 0;
  } else if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL)) {
    KtiPortCnt = SI_MAX_KTI_PORTS_SKX;
  } else if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    KtiPortCnt = SI_MAX_KTI_PORTS_CPX;
  } else {
    KtiPortCnt = SI_MAX_KTI_PORTS_ICX;
  }

  //
  // Policy not listed here are set to 0/FALSE as default.
  //

  UpiPolicyPpi->Revision                = UPI_POLICY_PPI_REVISION;
  //
  // Fill-in the system wide options
  //
  for (Ctr1 = 0; Ctr1 < MAX_SOCKET; Ctr1++) {
    //
    // Assign resources equally among all IIOs
    //
    UpiPolicyPpi->Upi.BusRatio[Ctr1] = BUS_RATIO_DFLT;
    //
    // Default enable discovered Fpgas
    //
    UpiPolicyPpi->Upi.KtiFpgaEnable[Ctr1] = KTI_AUTO;
  }

  UpiPolicyPpi->Upi.LegacyVgaSoc        = LEGACY_VGA_SOCKET_DFLT;   // Legacy VGA range target socket
  UpiPolicyPpi->Upi.LegacyVgaStack      = LEGACY_VGA_STACK_DFLT;    // Legacy VGA range target stack
  UpiPolicyPpi->Upi.P2pRelaxedOrdering  = KTI_DISABLE;              // Disable PCIE P2P performance mode
  UpiPolicyPpi->Upi.DebugPrintLevel     = KTI_DEBUG_PRINT_ALL;      // Enable all levels of debug messages
  UpiPolicyPpi->Upi.IoDcMode            = IODC_AUTO;                // Auto by default, depends on topology
  UpiPolicyPpi->Upi.DegradePrecedence   = TOPOLOGY_PRECEDENCE;
  UpiPolicyPpi->Upi.Degrade4SPreference = TOPOLOGY_4S_FULLY_CONNECT;
  UpiPolicyPpi->Upi.DirectoryModeEn     = KTI_ENABLE;
  UpiPolicyPpi->Upi.KtiPrefetchEn       = KTI_AUTO; // KtiPrefetch enable
  UpiPolicyPpi->Upi.XptRemotePrefetchEn = KTI_AUTO; // XptRemotePrefetch enable
  if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    UpiPolicyPpi->Upi.SncEn               = KTI_DISABLE; // Snc disabled
  } else {
    UpiPolicyPpi->Upi.SncEn               = KTI_ENABLE; // Snc Enabled
  }
  UpiPolicyPpi->Upi.D2KCreditConfig       = KTI_D2K_MED;
  UpiPolicyPpi->Upi.SnoopThrottleConfig   = KTI_STC_AUTO;
  UpiPolicyPpi->Upi.XptPrefetchEn         = KTI_AUTO;
  UpiPolicyPpi->Upi.RdCurForXptPrefetchEn = KTI_AUTO;
  UpiPolicyPpi->Upi.TorThresLoctoremNorm  = THRESHOLDS_AUTO;
  UpiPolicyPpi->Upi.TorThresLoctoremEmpty = THRESHOLDS_AUTO;
  UpiPolicyPpi->Upi.TscSyncEn             = KTI_AUTO;
  UpiPolicyPpi->Upi.StaleAtoSOptEn        = KTI_DISABLE;
  UpiPolicyPpi->Upi.LLCDeadLineAlloc      = KTI_ENABLE;
  UpiPolicyPpi->Upi.SplitLock             = KTI_DISABLE;
  UpiPolicyPpi->Upi.DdrtQosMode           = KTI_DISABLE;

  //
  // Phy/LL system wide settings
  //
  UpiPolicyPpi->Upi.KtiLinkSpeedMode          = KTI_LINK_FULL_SPEED_MODE;
  UpiPolicyPpi->Upi.KtiLinkSpeed              = MAX_KTI_LINK_SPEED;
  UpiPolicyPpi->Upi.KtiAdaptationEn           = KTI_DISABLE;
  UpiPolicyPpi->Upi.KtiAdaptationSpeed        = MAX_KTI_LINK_SPEED;
  UpiPolicyPpi->Upi.KtiLinkL0pEn              = KTI_AUTO;
  UpiPolicyPpi->Upi.KtiLinkL1En               = KTI_AUTO;
  UpiPolicyPpi->Upi.KtiFailoverEn             = KTI_AUTO;
  UpiPolicyPpi->Upi.KtiLbEn                   = KTI_DISABLE;
  UpiPolicyPpi->Upi.KtiCrcMode                = CRC_MODE_AUTO;
  UpiPolicyPpi->Upi.KtiCpuSktHotPlugEn        = KTI_DISABLE;
  UpiPolicyPpi->Upi.KtiCpuSktHotPlugTopology  = HOT_PLUG_TOPOLOGY_4S;
  UpiPolicyPpi->Upi.KtiSkuMismatchCheck       = KTI_ENABLE;

  //
  // Phy/LL per link settings
  //
  for (Ctr1 = 0; Ctr1 < MAX_SOCKET; Ctr1++) {
    for (Ctr2 = 0; Ctr2 < KtiPortCnt; Ctr2++){
      UpiPolicyPpi->Upi.PhyLinkPerPortSetting[Ctr1].Link[Ctr2].KtiPortDisable = KTI_DISABLE; //Assume all ports are enabled

      if (UpiPolicyPpi->Upi.KtiLinkSpeed < FREQ_PER_LINK) {
        UpiPolicyPpi->Upi.PhyLinkPerPortSetting[Ctr1].Phy[Ctr2].KtiLinkSpeed = UpiPolicyPpi->Upi.KtiLinkSpeed;
      } else {
        UpiPolicyPpi->Upi.PhyLinkPerPortSetting[Ctr1].Phy[Ctr2].KtiLinkSpeed = SPEED_REC_96GT;
      }

      UpiPolicyPpi->Upi.PhyLinkPerPortSetting[Ctr1].Link[Ctr2].KtiLinkVnaOverride = KTI_LL_VNA_SETUP_MAX;
    }
  }

  //
  // Copy system-wide Dfx options.  These should not be changed
  //
  CopyMem ((UINT8*)&UpiPolicyPpi->Upi.DfxParm, (UINT8*)&DfxParmDefaults, sizeof (KTI_DFX_PARM));

  //
  // Copy DFX Phy/LL per link settings
  //
  for (Ctr1 = 0; Ctr1 < MAX_SOCKET; Ctr1++) {
    for (Ctr2 = 0; Ctr2 < KtiPortCnt; Ctr2++){
      UpiPolicyPpi->Upi.DfxCpuCfg[Ctr1].Link[Ctr2].DfxCrcMode = CRC_MODE_AUTO;
      UpiPolicyPpi->Upi.DfxCpuCfg[Ctr1].Link[Ctr2].DfxL0pEnable = KTI_AUTO;
      UpiPolicyPpi->Upi.DfxCpuCfg[Ctr1].Link[Ctr2].DfxL1Enable = KTI_AUTO;
      UpiPolicyPpi->Upi.DfxCpuCfg[Ctr1].Phy[Ctr2].DfxKtiFailoverEn = KTI_AUTO;
    }
  }

  UpiPolicyPpi->Upi.mmCfgBase  = MMCFG_BASE_AUTO;
  UpiPolicyPpi->Upi.mmCfgSize  = MMCFG_SIZE_AUTO;

  //
  // ICX-S supports 52b addressing
  //
  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)){
    UpiPolicyPpi->Upi.mmiohBase = MMIOH_BASE_ICX;
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)){
    //
    // SNR supports 42b addressing
    //
    UpiPolicyPpi->Upi.mmiohBase = MMIOH_BASE_SNR;
  } else {
    //
    // ICX-D, ICX-G and KNH support 46b addressing
    //
    UpiPolicyPpi->Upi.mmiohBase = MMIOH_BASE;
  }

  UpiPolicyPpi->Upi.mmiohSize  = MMIOH_SIZE;
  UpiPolicyPpi->Upi.highGap    = HIGH_GAP;            // High MMIO gap = 256 MB

  UpiPolicyPpi->Upi.isocEn     = 0;
  UpiPolicyPpi->Upi.dcaEn      = 1;

  //
  // Allow standard CPUs on all possible 8 slots in any board.
  //
  UpiPolicyPpi->Upi.BoardTypeBitmask = CPU_TYPE_STD_MASK | (CPU_TYPE_STD_MASK << 4) | (CPU_TYPE_STD_MASK << 8) |
                                       (CPU_TYPE_STD_MASK << 12) | (CPU_TYPE_STD_MASK << 16) | (CPU_TYPE_STD_MASK << 20) |
                                       (CPU_TYPE_STD_MASK << 24) | (CPU_TYPE_STD_MASK << 28);

  //
  // It doesn't provide default Eparam table, as this table is board specific.
  //
  UpiPolicyPpi->Upi.AllLanesPtr = 0;
  UpiPolicyPpi->Upi.AllLanesSizeOfTable = 0;

  UpiPolicyPpi->Upi.PerLanePtr = 0;
  UpiPolicyPpi->Upi.PerLaneSizeOfTable = 0;

  UpiPolicyPpi->Upi.WaitTimeForPSBP = WAIT_TIME_FOR_PSBP_DEFAULT;
  UpiPolicyPpi->Upi.IsKtiNvramDataReady = FALSE;

  UpiPolicyPpi->Upi.WaSerializationEn = 0;

  return EFI_SUCCESS;
}

/**
  UpiInstallPolicyPpi installs UpiPolicyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] UpiPolicyPpi               The pointer to UPI Policy PPI instance

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
UpiInstallPolicyPpi (
  IN  UPI_POLICY_PPI *UpiPolicyPpi
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_PPI_DESCRIPTOR      *InstallUpiPolicyPpiDesc;
  PERF_START (NULL, "UpiInstallPolicyPpi", "PEI", 0);
  Status = EFI_SUCCESS;
  InstallUpiPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (InstallUpiPolicyPpiDesc != NULL);
  if (InstallUpiPolicyPpiDesc != NULL) {
    InstallUpiPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    InstallUpiPolicyPpiDesc->Guid  = &gUpiSiPolicyPpiGuid;
    InstallUpiPolicyPpiDesc->Ppi   = UpiPolicyPpi;

    DEBUG ((DEBUG_INFO, "Installing UpiPolicyPpi \n"));
    Status = PeiServicesInstallPpi (InstallUpiPolicyPpiDesc);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Installing UpiPolicyPpi Error = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
    }
  }
  PERF_END (NULL, "UpiInstallPolicyPpi", "PEI", 0);
  return Status;
}
