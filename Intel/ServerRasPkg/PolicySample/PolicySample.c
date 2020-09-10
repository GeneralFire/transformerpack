/** @file
  Implementation of RAS policy sample driver. Customer can override this driver as own code base.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#include <PiSmm.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/RasDebugLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/RasPolicy.h>
#include <Library/SetupLib.h>
#include <SetupTable.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/AddressDecodeLib.h>
#include <Upi/RasTypes.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/GpioLib.h>
#include <Library/UbaGpioPlatformConfig.h>
#include <Library/RasRevision.h>
#include <Memory/MemDefaults.h>
#include <Library/MemMcIpRasLib.h>
#include <Emca.h>
#include <Library/ProcessorRasLib.h>
#include <Library/PcieRasLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Register/PcieRegDef.h>
// APTIOV_SERVER_OVERRIDE_RC_START: Added support to Unlock MSR 0x790 for Error Injection Tools to work.
#include <Token.h>
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to Unlock MSR 0x790 for Error Injection Tools to work.

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#include <Library/AmiOemRasLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

// APTIOV_SERVER_OVERRIDE_RC_START: To resolve Optimization build error
// Define intrinsic functions to satisfy the .NET 2008 compiler with size optimization /O1
// compiler automatically inserts memcpy/memset fn calls in certain cases
void *memcpy(void *dst, void *src, unsigned int cnt);
void *memset(void *dst, char value, unsigned int cnt);
// APTIOV_SERVER_OVERRIDE_RC_END: To resolve Optimization build error
EFI_HANDLE   mHandle = NULL;
RAS_POLICY   mRasPolicy;

EFI_STATUS
EFIAPI
InitializePolicyData (
  VOID
  )
{
    // APTIOV_SERVER_OVERRIDE_RC_START: To solve redefinition error
    INTEL_SETUP_DATA                SetupData;
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve redefinition error
  EFI_STATUS                Status;
  PCIE_ERROR_FILTER         PcieErrFilterTable[] = {
                              //Vid, DidMin, DidMax, AerCorrMskOr, AerCorrMskAnd, AerUnCorrMskOr, AerUnCorrMskAnd, AerErrSevOr, AerErrSevAnd, DevCtlOr, DevCtlAnd, AeccConfOr, AeccConfAnd
                              {0x8086, 0x5375, 0x5375, 0, 0xffffffff, 0, 0xffffffff, 0, 0xffffffff, 0, 0xfffe, 0, 0xffffffff}, //Florin Test Card,  disable correctable error
                              {0x8086, 0x18ee, 0x18ef, 0, 0xffffffff, 0, 0xffffffff, 0, 0xffffffff, 0, 0xffff, 0, 0xfffffebf}, // CPM, disable ECE and EGE, Silicon WA "S14010981220"
                              {0x8086, 0x18a0, 0x18a0, 0, 0xffffffff, 0, 0xffffffff, 0, 0xffffffff, 0, 0xffff, 0, 0xfffffebf}, // CPM, disable ECE and EGE, Silicon WA "S14010981220"
                              {0x8086, 0x1888, 0x189f, 0, 0xffffffff, 0, 0xffffffff, 0, 0xffffffff, 0, 0xffff, 0, 0xfffffebf}, // CPK, disable ECE and EGE, Silicon WA "S14010981220"
                            };
  MEM_TOPOLOGY              *MemTopology;

  RAS_DEBUG ((LEVEL_BASIC_FLOW,"InitializePolicyData \n"));


  Status = GetEntireConfig (&SetupData);
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "Failed to get setup variable, status = %r\n", Status));
    return Status;
  }

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetMemTopology failed \n"));
    return EFI_UNSUPPORTED;
  }

  mRasPolicy.RasRevision = RAS_REVISION;
  mRasPolicy.SystemErrorEn = SetupData.SystemConfig.SystemErrorEn;
  mRasPolicy.CloakDevHideRegistersOs = SetupData.SystemConfig.CloakDevHideRegistersOs;
  mRasPolicy.FatalErrSpinLoopEn = SetupData.SystemConfig.FatalErrSpinLoopEn;

  mRasPolicy.McaPolicy.CloakingEn = SetupData.SystemConfig.CloakingEn;


  mRasPolicy.McaPolicy.ElogCorrErrEn = SetupData.SystemConfig.ElogCorrErrEn;
  mRasPolicy.McaPolicy.LmceEn = SetupData.SystemConfig.LmceEn;
  mRasPolicy.McaPolicy.EmcaEn = SetupData.SystemConfig.EmcaEn;
  mRasPolicy.McaPolicy.EmcaMsmiEn = SetupData.SystemConfig.EmcaMsmiEn;
  mRasPolicy.McaPolicy.MSMIBankBitFieldEn = 0xFFFFFFFF;
  mRasPolicy.McaPolicy.EmcaCsmiEn = SetupData.SystemConfig.EmcaCsmiEn;
  mRasPolicy.McaPolicy.EmcaCsmiThreshold = SetupData.SystemConfig.EmcaCsmiThreshold;

  mRasPolicy.McaPolicy.CsmiDynamicDisable = SetupData.SystemConfig.CsmiDynamicDisable;
  if (MemTopology->VolMemMode == VOL_MEM_MODE_2LM) {
    mRasPolicy.McaPolicy.CsmiDynamicDisable = 0;
  }
  mRasPolicy.McaPolicy.McBankWarmBootClearEn = SetupData.SystemConfig.McBankWarmBootClearError;

  //
  // Enable CSMI error
  //
  if (mRasPolicy.McaPolicy.EmcaCsmiEn == 2) {
    mRasPolicy.McaPolicy.CSMIBankBitFieldEn = 0xFFFFFFFF;
  }
  mRasPolicy.McaPolicy.EmcaIgnOptin = SetupData.SystemConfig.EmcaIgnOptin;
  mRasPolicy.McaPolicy.ElogMemErrEn = SetupData.SystemConfig.ElogMemErrEn;
  mRasPolicy.McaPolicy.ElogProcErrEn = SetupData.SystemConfig.ElogProcErrEn;

  mRasPolicy.McaPolicy.OscEn = SetupData.SystemConfig.OscEn;
  mRasPolicy.McaPolicy.McaSpinLoop = SetupData.SystemConfig.McaSpinLoop;
  mRasPolicy.McaPolicy.UboxErrorMask = SetupData.SystemConfig.UboxErrorMask;

  //
  //memory part
  //
  mRasPolicy.ImcPolicy.MemErrEn = SetupData.SystemConfig.MemErrEn;
  mRasPolicy.ImcPolicy.CorrMemErrEn = SetupData.SystemConfig.CorrMemErrEn;
  mRasPolicy.ImcPolicy.SpareIntSelect = SetupData.SystemConfig.SpareIntSelect;
  mRasPolicy.ImcPolicy.MemCeFloodPolicy = SetupData.SocketConfig.MemoryConfig.MemCeFloodPolicy;


  mRasPolicy.ImcPolicy.SaiPolicyGroupWaBiosW = SetupData.SocketConfig.MemoryConfig.SaiPolicyGroupWaBiosW;

  if (IsSiliconWorkaroundEnabled ("S22010160600")) {
    mRasPolicy.ImcPolicy.DcpmmEccModeSwitch = 0;
  } else {
    mRasPolicy.ImcPolicy.DcpmmEccModeSwitch = SetupData.SocketConfig.MemoryConfig.DcpmmEccModeSwitch;
  }

  // check whether a DDRT DIMM is populated in system
  if (MemTopology->EkvPresent || MemTopology->BwvPresent) {
    mRasPolicy.ImcPolicy.FnvErrorEn = SetupData.SystemConfig.FnvErrorEn;
  } else { // disable the error en
    mRasPolicy.ImcPolicy.FnvErrorEn = FALSE;
  }
  mRasPolicy.ImcPolicy.FnvErrorLowPrioritySignal = SetupData.SystemConfig.FnvErrorLowPrioritySignal;
  mRasPolicy.ImcPolicy.FnvErrorHighPrioritySignal = SetupData.SystemConfig.FnvErrorHighPrioritySignal;
  mRasPolicy.ImcPolicy.NgnAddressRangeScrub =   SetupData.SystemConfig.NgnAddressRangeScrub;
  mRasPolicy.ImcPolicy.NgnHostAlertDpa = SetupData.SystemConfig.NgnHostAlertDpa;
  mRasPolicy.ImcPolicy.NgnHostAlertPatrolScrubUNC = SetupData.SystemConfig.NgnHostAlertPatrolScrubUNC;
  mRasPolicy.ImcPolicy.ReportAlertSPA = SetupData.SystemConfig.ReportAlertSPA;
  mRasPolicy.ImcPolicy.DcpmmUncPoison = SetupData.SystemConfig.DcpmmUncPoison;
  mRasPolicy.ImcPolicy.DdrtInternalAlertEn = SetupData.SystemConfig.DdrtInternalAlertEn;
  mRasPolicy.ImcPolicy.PprType = SetupData.SocketConfig.MemoryConfig.pprType;
  mRasPolicy.ImcPolicy.PprErrInjTest = SetupData.SocketConfig.MemoryConfig.pprErrInjTest;
  mRasPolicy.ImcPolicy.SpareErrTh = SetupData.SocketConfig.MemoryConfig.spareErrTh;

  // Bank Level Thresold with timestamp +>>
  mRasPolicy.ImcPolicy.TriggerSWErrThEn = SetupData.SocketConfig.MemoryConfig.TriggerSWErrThEn;
  mRasPolicy.ImcPolicy.SpareSwErrTh = SetupData.SocketConfig.MemoryConfig.SpareSwErrTh;
  mRasPolicy.ImcPolicy.TimeWindow = SetupData.SocketConfig.MemoryConfig.timeWindow;
  // Bank Level Thresold with timestamp +<<

  //
  // Ras policy to do Rank VLS only instead of attempting to do Bank VLS
  // This is always set to TRUE based on silicon workaround. It can be connected to setup knob in future.
  //
  if (IsSiliconWorkaroundEnabled ("S1706995555")) {
    mRasPolicy.ImcPolicy.RankVlsAlways = TRUE;
  } else {
    mRasPolicy.ImcPolicy.RankVlsAlways = FALSE;
  }

  //
  //Misc part
  //
  mRasPolicy.PoisonEn = SetupData.SystemConfig.PoisonEn;
  if ((IsSiliconWorkaroundEnabled ("S1606891917") && CheckMcMcaRecovery ()) || (MemTopology->RasModesEnabled & CH_ALL_MIRROR)) {
    //
    //1606891917: when detect  ICX-SP, ICX-D, SNR A0 stepping CPU with recovery fuse enabled, force enable poison feature irrespective SETUP option on poison feature.
    //14010087183: Poison should be enabled and exras_config.corr2corroverwritedis should be 0 when mirroring is enabled
    //
    mRasPolicy.PoisonEn = TRUE;
  }
  
  mRasPolicy.ViralEn = SetupData.SystemConfig.ViralEn;
  mRasPolicy.ClearViralStatus = SetupData.SystemConfig.ClearViralStatus;
  mRasPolicy.SmbusErrorRecovery = SetupData.SocketConfig.SocketProcessorCoreConfiguration.SmbusErrorRecovery;

  //
  // IIO part.
  //
  if (IsSiliconWorkaroundEnabled ("S2208074954") && SetupData.SystemConfig.IioErrorEn && SetupData.SocketConfig.IioConfig.VTdSupport) {

    mRasPolicy.IioPolicy.IioErrorEn = FALSE;
    RAS_DEBUG ((LEVEL_BASIC_FLOW, RAS_WARN_STR ("IIO") "ICX R0 VT-d workaround enforces IIO errors disable\n"));

  } else {

    mRasPolicy.IioPolicy.IioErrorEn = SetupData.SystemConfig.IioErrorEn;
  }
  mRasPolicy.IioPolicy.OsNativeAerSupport = SetupData.SystemConfig.OsNativeAerSupport;

  if (mRasPolicy.IioPolicy.IioErrorEn != 0x00) {

    mRasPolicy.IioPolicy.IioErrRegistersClearEn = SetupData.SystemConfig.IioErrRegistersClearEn;
    mRasPolicy.IioPolicy.IoMcaEn = SetupData.SystemConfig.IoMcaEn;
    if((SetupData.SocketConfig.SocketProcessorCoreConfiguration.EnableSgx == 0x01) && IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      mRasPolicy.IioPolicy.IioSev1Pcc = 0x00;
    } else {
      mRasPolicy.IioPolicy.IioSev1Pcc = SetupData.SystemConfig.IioSev1Pcc;
    }
    mRasPolicy.IioPolicy.IioOOBMode = SetupData.SystemConfig.IioOOBMode;

    if (mRasPolicy.IioPolicy.IoMcaEn == 0x00) {
      mRasPolicy.IioPolicy.IioErrorPin0En = SetupData.SystemConfig.IioErrorPin0En;
      mRasPolicy.IioPolicy.IioErrorPin1En = SetupData.SystemConfig.IioErrorPin1En;
      mRasPolicy.IioPolicy.IioErrorPin2En = SetupData.SystemConfig.IioErrorPin2En;
      mRasPolicy.McaPolicy.MSMIBankBitFieldEn &= ~MCA_UNIT_TYPE_IIO; // Disable IIO bank
    } else {
      mRasPolicy.IioPolicy.IioErrorPin0En = SetupData.SystemConfig.IioErrorPin0En;
      mRasPolicy.IioPolicy.IioErrorPin1En = 0;
      mRasPolicy.IioPolicy.IioErrorPin2En = 0;
      mRasPolicy.McaPolicy.MSMIBankBitFieldEn |= MCA_UNIT_TYPE_IIO;
    }

    mRasPolicy.IioPolicy.LerEn = SetupData.SystemConfig.LerEn;
    if (mRasPolicy.IioPolicy.LerEn != 0x00) {
      mRasPolicy.IioPolicy.DisableLerMAerrorLogging = SetupData.SystemConfig.DisableMAerrorLoggingDueToLER;
    }

    mRasPolicy.IioPolicy.IioDmiErrorEn = SetupData.SystemConfig.IioDmiErrorEn;
    mRasPolicy.IioPolicy.IioIrpErrorEn = SetupData.SystemConfig.IioIrpErrorEn;
    if (mRasPolicy.IioPolicy.IioIrpErrorEn != 0x00) {
      //
      // IRPP0ERRCTL
      //
      mRasPolicy.IioPolicy.IioIrpp0ErrCtlBitMap =
      (UINT32) (SetupData.SystemConfig.irpp0_parityError << 14) |
      (UINT32) (SetupData.SystemConfig.irpp0_qtOverflow << 13) |
      (UINT32) (SetupData.SystemConfig.irpp0_unexprsp << 10) |
      (UINT32) (SetupData.SystemConfig.irpp0_csraccunaligned << 6) |
      (UINT32) (SetupData.SystemConfig.irpp0_unceccCs1 << 5) |
      (UINT32) (SetupData.SystemConfig.irpp0_unceccCs0 << 4) |
      (UINT32) (SetupData.SystemConfig.irpp0_rcvdpoison << 3) |
      (UINT32) (SetupData.SystemConfig.irpp0_crreccCs1 << 2) |
      (UINT32) (SetupData.SystemConfig.irpp0_crreccCs0 << 1);

      //
      // IRPPERRSV
      //
      mRasPolicy.IioPolicy.IioCohSevBitMap = (BIT4 | BIT6 | BIT8 | BIT21 | BIT27 | BIT29);
    }

    mRasPolicy.IioPolicy.IioMiscErrorEn = SetupData.SystemConfig.IioMiscErrorEn;
    if (mRasPolicy.IioPolicy.IioMiscErrorEn != 0x00) {
      mRasPolicy.IioPolicy.IioMiscErrorBitMap = (BIT0 | BIT2 | BIT3 | BIT4);// MIERRCTL
      mRasPolicy.IioPolicy.IioMiscErrorSevMap = (BIT1 | BIT6);// MIERRSV
    }

    mRasPolicy.IioPolicy.IioVtdErrorEn = SetupData.SystemConfig.IioVtdErrorEn;

    mRasPolicy.IioPolicy.IioItcErrSevBitMap = 0;
    mRasPolicy.IioPolicy.IioOtcErrSevBitMap = 0;

    mRasPolicy.IioPolicy.ItcOtcCaMaEnable = SetupData.SystemConfig.ItcOtcCaMaEnable;

    mRasPolicy.IioPolicy.IioDmaErrorEn = SetupData.SystemConfig.IioDmaErrorEn;
    if (mRasPolicy.IioPolicy.IioDmaErrorEn != 0x00) {
      //
      // CHANERRMSK, CHANERRSEV
      //
      mRasPolicy.IioPolicy.IioDmaBitMap = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9
                                      | BIT10 | BIT11 | BIT12 | BIT13 | BIT14 | BIT15 | BIT16);
      mRasPolicy.IioPolicy.IioDmaSevBitMap = (BIT16);
    }

    mRasPolicy.PciePolicy.PcieErrEn = SetupData.SystemConfig.PcieErrEn;
    mRasPolicy.IioPolicy.IioPcieAddCorrErrorEn = SetupData.SystemConfig.IioPcieAddCorrErrorEn;

    mRasPolicy.IioPolicy.IioPcieAddUnCorrEn = SetupData.SystemConfig.IioPcieAddUnCorrEn;
    if (mRasPolicy.IioPolicy.IioPcieAddUnCorrEn != 0x00) {
      //
      // XPUNCEDMASK, XPUNCERRMSK, XPUNCERRSEV
      //
      mRasPolicy.IioPolicy.IioPcieAddUnCorrBitMap = (BIT1 | BIT3 | BIT8 | BIT9); //bit 5 and 6 values shouldn't be 1, System hangs on "GeneratePciDevInfo"
      mRasPolicy.IioPolicy.IioPcieAddUnCorrSevBitMap = BIT1;
    }
    mRasPolicy.IioPolicy.IioPcieAddRcvComWithUr = SetupData.SystemConfig.IioPcieAddRcvComWithUr;

    if (mRasPolicy.IioPolicy.IioPcieAddRcvComWithUr) {
      mRasPolicy.IioPolicy.IioPcieAddUnCorrBitMap |= BIT6;
    }

    mRasPolicy.IioPolicy.IioPcieAerSpecCompEn = SetupData.SystemConfig.IioPcieAerSpecCompEn;

    mRasPolicy.IioPolicy.IehLocalUcMask = 0;
    mRasPolicy.IioPolicy.IehLocalUcSeverity = 0x000FFF00; // enable all IEH PSF severity bits
    mRasPolicy.IioPolicy.IehLocalCeMask = 0;

    //
    // Mask PSF Unsupported Request Error according to setup knob
    //
    if (!SetupData.SystemConfig.PsfUrEnable) {
      mRasPolicy.IioPolicy.IehLocalUcMask |= BIT10 + BIT13 + BIT16 + BIT19;
      mRasPolicy.IioPolicy.IehLocalCeMask |= BIT10 + BIT13 + BIT16 + BIT19;
    }

    //
    // Mask PMSB Router Parity Error according to setup knob
    //
    if (!SetupData.SystemConfig.PmsbRouterParityErrEn) {
      mRasPolicy.IioPolicy.IehLocalUcMask |= BIT31;
      mRasPolicy.IioPolicy.IehLocalCeMask |= BIT31;
    }

    mRasPolicy.IioPolicy.IioRootCtlOverride = 0;
  }

  //
  // PCI-EX
  //
  if (mRasPolicy.PciePolicy.PcieErrEn != 0x00) {

    mRasPolicy.PciePolicy.PcieCorrErrEn = SetupData.SystemConfig.PcieCorrErrEn;
    mRasPolicy.PciePolicy.PcieUncorrErrEn = SetupData.SystemConfig.PcieUncorrErrEn;
    mRasPolicy.PciePolicy.PcieFatalErrEn = SetupData.SystemConfig.PcieFatalErrEn;
    mRasPolicy.PciePolicy.PcieCorErrCntr = SetupData.SystemConfig.PcieCorErrCntr;
    mRasPolicy.PciePolicy.PcieCorErrLimitEn = SetupData.SystemConfig.PcieCorErrLimitEn;
    mRasPolicy.PciePolicy.PcieCorErrLimit = SetupData.SystemConfig.PcieCorErrLimit;

    mRasPolicy.PciePolicy.PcieCorErrThres = SetupData.SystemConfig.PcieCorErrThres;

    mRasPolicy.PciePolicy.PcieAerCorrErrEn = SetupData.SystemConfig.PcieAerCorrErrEn;
    if (mRasPolicy.PciePolicy.PcieAerCorrErrEn != 0x00) {
      //
      // COREDMASK, CORERRMSK
      //
      mRasPolicy.PciePolicy.PcieAerCorrErrBitMap = (B_PCIE_AER_CES_RCV_ERR |
                                                    B_PCIE_AER_CES_BAD_TLP_ERR |
                                                    B_PCIE_AER_CES_BAD_DLLP_ERR |
                                                    B_PCIE_AER_CES_REP_NUM_ROV_ERR |
                                                    B_PCIE_AER_CES_REP_TMR_TOT_ERR |
                                                    B_PCIE_AER_CES_ADV_NON_FAT_ERR |
                                                    B_PCIE_AER_CES_COR_INT_ERR |
                                                    B_PCIE_AER_CES_HLOG_OVRF_ERR);
    }

    mRasPolicy.PciePolicy.PcieAerNfatErrEn = SetupData.SystemConfig.PcieAerNfatErrEn;
    if (mRasPolicy.PciePolicy.PcieAerNfatErrEn != 0x00) {
      //
      // UNCERRMSK, UNCEDMASK, UNCERRSEV
      //
      mRasPolicy.PciePolicy.PcieAerNfatErrBitMap = (B_PCIE_AER_UES_CTO_ERR |
                                                    B_PCIE_AER_UES_ACS_VIO_ERR |
                                                    B_PCIE_AER_UES_CPL_ABT_ERR |
                                                    B_PCIE_AER_UES_UNE_CPL_ERR |
                                                    B_PCIE_AER_UES_UNS_REQ_ERR |
                                                    B_PCIE_AER_UES_POI_TLP_EGR_BLK_ERR |
                                                    B_PCIE_AER_UES_TLP_PRE_BLK_ERR |
                                                    B_PCIE_AER_UES_ATO_EGR_BLK_ERR |
                                                    B_PCIE_AER_UES_MC_BLK_TLP_ERR);
    }

    mRasPolicy.PciePolicy.PcieAerFatErrEn = SetupData.SystemConfig.PcieAerFatErrEn;
    if (mRasPolicy.PciePolicy.PcieAerFatErrEn != 0x00) {
      //
      // UNCERRMSK, UNCEDMASK, UNCERRSEV
      //
      mRasPolicy.PciePolicy.PcieAerFatErrBitMap = (B_PCIE_AER_UES_DAT_LNK_ERR |
                                                   B_PCIE_AER_UES_SUPRISE_LINK_DOWN_ERR |
                                                   B_PCIE_AER_UES_POI_TLP_ERR |
                                                   B_PCIE_AER_UES_FLW_CTL_PRO_ERR |
                                                   B_PCIE_AER_UES_RCV_OVR_ERR |
                                                   B_PCIE_AER_UES_MAL_TLP_ERR |
                                                   B_PCIE_AER_UES_UNC_INT_ERR);
    }

    mRasPolicy.PciePolicy.PcieAerEcrcEn = SetupData.SystemConfig.PcieAerEcrcEn;
    mRasPolicy.PciePolicy.PcieAerUreEn = SetupData.SystemConfig.PcieAerUreEn;
    mRasPolicy.PciePolicy.PcieAerAdNfatErrEn = SetupData.SystemConfig.PcieAerAdNfatErrEn;
    mRasPolicy.PciePolicy.PcieAerSurpriseLinkDownEn = SetupData.SystemConfig.PcieAerSurpriseLinkDownEn;

    if (!mRasPolicy.PciePolicy.PcieAerAdNfatErrEn) {
      mRasPolicy.PciePolicy.PcieAerCorrErrBitMap &= ~B_PCIE_AER_CES_ADV_NON_FAT_ERR;
    }

    if (!mRasPolicy.PciePolicy.PcieAerUreEn) {
      mRasPolicy.PciePolicy.PcieAerNfatErrBitMap &= ~B_PCIE_AER_UES_UNS_REQ_ERR;
    }

    if (!mRasPolicy.PciePolicy.PcieAerSurpriseLinkDownEn ) {
      mRasPolicy.PciePolicy.PcieAerFatErrBitMap &= ~B_PCIE_AER_UES_SUPRISE_LINK_DOWN_ERR;
    }

    //
    // CPX CPU Got wrong ERRSID for PCIE ROOT PORT Error,
    // Need to make a workaround for issue
    //
    if (IsSiliconWorkaroundEnabled ("S2209292379")){
      mRasPolicy.PciePolicy.PcieAerErrsidWa = 1;
    } else {
      mRasPolicy.PciePolicy.PcieAerErrsidWa = 0;
    }

    mRasPolicy.PciePolicy.SerrPropEn = SetupData.SystemConfig.PropagateSerr;
    mRasPolicy.PciePolicy.PerrPropEn = SetupData.SystemConfig.PropagatePerr;
    mRasPolicy.PciePolicy.SerrEmuTestEn = 0;

    mRasPolicy.PciePolicy.PcieErrFilterCount = sizeof (PcieErrFilterTable) / sizeof (PCIE_ERROR_FILTER);
    if (mRasPolicy.PciePolicy.PcieErrFilterCount != 0) {
      mRasPolicy.PciePolicy.PcieErrFilterTable = AllocateZeroPool (sizeof (PcieErrFilterTable));
      if (mRasPolicy.PciePolicy.PcieErrFilterTable != NULL) {
        CopyMem(mRasPolicy.PciePolicy.PcieErrFilterTable, PcieErrFilterTable, sizeof (PcieErrFilterTable));
      } else {
        mRasPolicy.PciePolicy.PcieErrFilterCount = 0;
      }
    }
    //
    // PCIe DPC Support
    //
    mRasPolicy.PciePolicy.EdpcEn = SetupData.SystemConfig.EdpcEn;
    mRasPolicy.PciePolicy.EdpcInterrupt = SetupData.SystemConfig.EdpcInterrupt;
    mRasPolicy.PciePolicy.EdpcErrCorMsg = SetupData.SystemConfig.EdpcErrCorMsg;

    //
    // Leaky Bucket Feature (requires Gen4 IP)
    //
    mRasPolicy.PciePolicy.ExpectedBer = SetupData.SystemConfig.ExpectedBer;
    mRasPolicy.PciePolicy.Gen12TimeWindow = SetupData.SystemConfig.Gen12TimeWindow;
    mRasPolicy.PciePolicy.Gen34TimeWindow = SetupData.SystemConfig.Gen34TimeWindow;
    mRasPolicy.PciePolicy.Gen12ErrorThreshold = SetupData.SystemConfig.Gen12ErrorThreshold;
    mRasPolicy.PciePolicy.Gen34ErrorThreshold = SetupData.SystemConfig.Gen34ErrorThreshold;
    mRasPolicy.PciePolicy.Gen34ReEqualization = SetupData.SystemConfig.Gen34ReEqualization;
    mRasPolicy.PciePolicy.Gen2LinkDegradation = SetupData.SystemConfig.Gen2LinkDegradation;
    mRasPolicy.PciePolicy.Gen3LinkDegradation = SetupData.SystemConfig.Gen3LinkDegradation;
    mRasPolicy.PciePolicy.Gen4LinkDegradation = SetupData.SystemConfig.Gen4LinkDegradation;
  }

  //
  //AMEI
  //
  mRasPolicy.McBankErrorInjection = SetupData.SystemConfig.McaBankErrInjEn;

  //
  // WHEA
  //
  mRasPolicy.WheaPolicy.WheaSupportEn = SetupData.SystemConfig.WheaSupportEn;
  if (mRasPolicy.WheaPolicy.WheaSupportEn != 0) {
    mRasPolicy.WheaPolicy.WheaLogMemoryEn = SetupData.SystemConfig.WheaLogMemoryEn;
    mRasPolicy.WheaPolicy.WheaLogProcEn   = SetupData.SystemConfig.WheaLogProcEn;
    mRasPolicy.WheaPolicy.WheaLogPciEn    = SetupData.SystemConfig.WheaLogPciEn;

    mRasPolicy.WheaPolicy.AEPErrorInjEn = SetupData.SystemConfig.AEPErrorInjEn;
    mRasPolicy.WheaPolicy.WheaErrorInjSupportEn = SetupData.SystemConfig.WheaErrorInjSupportEn;
    mRasPolicy.WheaPolicy.McaBankErrInjEn = SetupData.SystemConfig.McaBankErrInjEn;
    mRasPolicy.WheaPolicy.WheaACPI50ErrInjEn = SetupData.SystemConfig.WheaErrInjEn;
    mRasPolicy.WheaPolicy.WheaPcieErrInjEn = SetupData.SystemConfig.WheaPcieErrInjEn;
    mRasPolicy.WheaPolicy.PcieErrInjActionTable = SetupData.SystemConfig.PcieErrInjActionTable;
    mRasPolicy.WheaPolicy.MeSegErrorInjEn = SetupData.SystemConfig.MeSegErrorInjEn;
  }

  //
  // CrashLog
  //
  mRasPolicy.CrashLogFeature = SetupData.SystemConfig.CrashLogFeature;
  mRasPolicy.CrashLogOnAllReset = SetupData.SystemConfig.CrashLogOnAllReset;
  mRasPolicy.CrashLogClear = SetupData.SystemConfig.CrashLogClear;
  mRasPolicy.CrashLogReArm = SetupData.SystemConfig.CrashLogReArm;

  //
  // Error Control
  //
  if (SetupData.SystemConfig.PatrolScrubErrorReporting == 0) {
    mRasPolicy.ErrorControlPolicy.PatrolErrorDowngradeEn = 1;
    mRasPolicy.ErrorControlPolicy.LmceDowngradeEn = 0;
  }
  if (SetupData.SystemConfig.PatrolScrubErrorReporting == 1) {
    mRasPolicy.ErrorControlPolicy.PatrolErrorDowngradeEn = 0;
    mRasPolicy.ErrorControlPolicy.LmceDowngradeEn = 1;
  }
  if (SetupData.SystemConfig.PatrolScrubErrorReporting == 2) {
    mRasPolicy.ErrorControlPolicy.PatrolErrorDowngradeEn = 0;
    mRasPolicy.ErrorControlPolicy.LmceDowngradeEn = 0;
  }
  mRasPolicy.ErrorControlPolicy.Ce2LmLoggingEn = SetupData.SystemConfig.Ce2LmLoggingEn;
  mRasPolicy.ErrorControlPolicy.KtiFirstCeLatchEn = SetupData.SystemConfig.KtiFirstCeLatchEn;
  mRasPolicy.ErrorControlPolicy.LlcEwbErrorControl = SetupData.SystemConfig.LlcEwbErrorControl;

  //
  // Shutdown Suppression
  //
  mRasPolicy.ShutdownSuppression  = SetupData.SystemConfig.ShutdownSuppression;
  
  // APTIOV_SERVER_OVERRIDE_RC_START: Added support to Unlock MSR 0x790 for Error Injection Tools to work.
    //
    // Update UnlockMsr Policy based on setup data and ENABLE_ERROR_INJECTOR_SUPPORT sdl token
    //
#if ENABLE_ERROR_INJECTOR_SUPPORT
    mRasPolicy.AmiRasPolicy.UnlockMsr                       = SetupData.SystemConfig.UnlockMsr;
#else
    mRasPolicy.AmiRasPolicy.UnlockMsr                       = 0;
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to Unlock MSR 0x790 for Error Injection Tools to work.

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
  OemOverrideRasPolicyData (&mRasPolicy, &SetupData);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support


  if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
   mRasPolicy.McaPolicy.LmceEn =0;
   mRasPolicy.ErrorControlPolicy.LmceDowngradeEn = 0;
   mRasPolicy.McBankErrorInjection = 0;
   mRasPolicy.WheaPolicy.McaBankErrInjEn = 0;
  }

  return EFI_SUCCESS;
}


VOID
DisplayRasPolicy (
  VOID
  )
{
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Display RAS policy\n"));

  //
  //RAS Version
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW,  "  0x%02x.%02x.%02x", (mRasPolicy.RasRevision >> 24) & 0xff, (mRasPolicy.RasRevision >> 16) & 0xff, (mRasPolicy.RasRevision >> 8) & 0xff));

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CloakingEn: 0x%x\n", mRasPolicy.McaPolicy.CloakingEn));


  //
  //memory
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "ElogCorrErrEn: 0x%x\n", mRasPolicy.McaPolicy.ElogCorrErrEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "LmceEn: 0x%x\n", mRasPolicy.McaPolicy.LmceEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "EmcaEn: 0x%x\n", mRasPolicy.McaPolicy.EmcaEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "EmcaMsmiEn: 0x%x\n", mRasPolicy.McaPolicy.EmcaMsmiEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "MSMIBankBitFieldEn: 0x%x\n", mRasPolicy.McaPolicy.MSMIBankBitFieldEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "OscEn: 0x%x\n", mRasPolicy.McaPolicy.OscEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "McaSpinLoop: 0x%x\n", mRasPolicy.McaPolicy.McaSpinLoop));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "UboxErrorMask: 0x%x\n", mRasPolicy.McaPolicy.UboxErrorMask));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "EmcaCsmiEn: 0x%x\n", mRasPolicy.McaPolicy.EmcaCsmiEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CsmiDynamicDisable: 0x%x\n", mRasPolicy.McaPolicy.CsmiDynamicDisable));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CSMIBankBitFieldEn: 0x%x\n", mRasPolicy.McaPolicy.CSMIBankBitFieldEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CSMIBankBitFieldEn: 0x%x\n", mRasPolicy.McaPolicy.McBankWarmBootClearEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "ShutdownSuppression: 0x%x\n", mRasPolicy.ShutdownSuppression));

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "MemErrEn: 0x%x\n", mRasPolicy.ImcPolicy.MemErrEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CorrMemErrEn: 0x%x\n", mRasPolicy.ImcPolicy.CorrMemErrEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "SpareIntSelect: 0x%x\n", mRasPolicy.ImcPolicy.SpareIntSelect));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "CorrecErrTh: 0x%x\n", mRasPolicy.ImcPolicy.CorrecErrTh));


  RAS_DEBUG ((LEVEL_FUNC_FLOW, "ADDDCEn: 0x%x\n", mRasPolicy.ImcPolicy.ADDDCEn));


  RAS_DEBUG ((LEVEL_FUNC_FLOW, "FnvErrorEn: 0x%x\n", mRasPolicy.ImcPolicy.FnvErrorEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "FnvErrorLowPrioritySignal: 0x%x\n", mRasPolicy.ImcPolicy.FnvErrorLowPrioritySignal));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "FnvErrorHighPrioritySignal: 0x%x\n", mRasPolicy.ImcPolicy.FnvErrorHighPrioritySignal));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "NgnAddressRangeScrub: 0x%x\n", mRasPolicy.ImcPolicy.NgnAddressRangeScrub));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "NgnHostAlertDpa: 0x%x\n", mRasPolicy.ImcPolicy.NgnHostAlertDpa));



  //
  // Iio part
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioErrorEn:  0x%x\n", mRasPolicy.IioPolicy.IioErrorEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IoMcaEn:  0x%x\n", mRasPolicy.IioPolicy.IoMcaEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioOOBMode:  0x%x\n", mRasPolicy.IioPolicy.IioOOBMode));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioSev1Pcc:  0x%x\n", mRasPolicy.IioPolicy.IioSev1Pcc));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioErrRegistersClearEn:  0x%x\n", mRasPolicy.IioPolicy.IioErrRegistersClearEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioErrorPin0En:  0x%x\n", mRasPolicy.IioPolicy.IioErrorPin0En));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioErrorPin1En:  0x%x\n", mRasPolicy.IioPolicy.IioErrorPin1En));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioErrorPin2En:  0x%x\n", mRasPolicy.IioPolicy.IioErrorPin2En));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "LerEn:  0x%x\n", mRasPolicy.IioPolicy.LerEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "DisableLerMAerrorLogging:  0x%x\n", mRasPolicy.IioPolicy.DisableLerMAerrorLogging));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioDmiErrorEn:  0x%x\n", mRasPolicy.IioPolicy.IioDmiErrorEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioIrpErrorEn:  0x%x\n", mRasPolicy.IioPolicy.IioIrpErrorEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioIrpp0ErrCtlBitMap:  0x%x\n", mRasPolicy.IioPolicy.IioIrpp0ErrCtlBitMap));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioCohSevBitMap:  0x%x\n", mRasPolicy.IioPolicy.IioCohSevBitMap));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioMiscErrorEn:  0x%x\n", mRasPolicy.IioPolicy.IioMiscErrorEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioMiscErrorBitMap:  0x%x\n", mRasPolicy.IioPolicy.IioMiscErrorBitMap));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioMiscErrorSevMap:  0x%x\n", mRasPolicy.IioPolicy.IioMiscErrorSevMap));

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioVtdErrorEn:  0x%x\n", mRasPolicy.IioPolicy.IioVtdErrorEn ));

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioItcErrSevBitMap:  0x%x\n", mRasPolicy.IioPolicy.IioItcErrSevBitMap));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioOtcErrSevBitMap:  0x%x\n", mRasPolicy.IioPolicy.IioOtcErrSevBitMap));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioDmaErrorEn:  0x%x\n", mRasPolicy.IioPolicy.IioDmaErrorEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioDmaBitMap:  0x%x\n", mRasPolicy.IioPolicy.IioDmaBitMap));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioDmaSevBitMap:  0x%x\n", mRasPolicy.IioPolicy.IioDmaSevBitMap));

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieErrEn:  0x%x\n", mRasPolicy.PciePolicy.PcieErrEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioPcieAddCorrErrorEn:  0x%x\n", mRasPolicy.IioPolicy.IioPcieAddCorrErrorEn ));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioPcieAddUnCorrEn:  0x%x\n", mRasPolicy.IioPolicy.IioPcieAddUnCorrEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioPcieAddUnCorrBitMap:  0x%x\n", mRasPolicy.IioPolicy.IioPcieAddUnCorrBitMap));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioPcieAddUnCorrSevBitMap:  0x%x\n", mRasPolicy.IioPolicy.IioPcieAddUnCorrSevBitMap));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioPcieAddRcvComWithUr:  0x%x\n", mRasPolicy.IioPolicy.IioPcieAddRcvComWithUr));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioPcieAerSpecCompEn:  0x%x\n", mRasPolicy.IioPolicy.IioPcieAerSpecCompEn));

  //
  // PCI-EX
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieCorrErrEn:  0x%x\n", mRasPolicy.PciePolicy.PcieCorrErrEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieUncorrErrEn:  0x%x\n", mRasPolicy.PciePolicy.PcieUncorrErrEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieFatalErrEn:  0x%x\n", mRasPolicy.PciePolicy.PcieFatalErrEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieCorErrCntr:  0x%x\n", mRasPolicy.PciePolicy.PcieCorErrCntr));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieCorErrThres:  0x%x\n", mRasPolicy.PciePolicy.PcieCorErrThres));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieCorErrLimitEn:  0x%x\n", mRasPolicy.PciePolicy.PcieCorErrLimitEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieCorErrLimit:  0x%x\n", mRasPolicy.PciePolicy.PcieCorErrLimit));

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieAerCorrErrEn:  0x%x\n", mRasPolicy.PciePolicy.PcieAerCorrErrEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieAerCorrErrBitMap:  0x%x\n", mRasPolicy.PciePolicy.PcieAerCorrErrBitMap));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieAerAdNfatErrEn:  0x%x\n", mRasPolicy.PciePolicy.PcieAerAdNfatErrEn));

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieAerNfatErrEn:  0x%x\n", mRasPolicy.PciePolicy.PcieAerNfatErrEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieAerNfatErrBitMap:  0x%x\n", mRasPolicy.PciePolicy.PcieAerNfatErrBitMap));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieAerFatErrEn:  0x%x\n", mRasPolicy.PciePolicy.PcieAerFatErrEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieAerFatErrBitMap:  0x%x\n", mRasPolicy.PciePolicy.PcieAerFatErrBitMap));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieAerEcrcEn:  0x%x\n", mRasPolicy.PciePolicy.PcieAerEcrcEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieAerUreEn:  0x%x\n", mRasPolicy.PciePolicy.PcieAerUreEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieAerSurpriseLinkDownEn:  0x%x\n", mRasPolicy.PciePolicy.PcieAerSurpriseLinkDownEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "SerrPropEn:  0x%x\n", mRasPolicy.PciePolicy.SerrPropEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PerrPropEn:  0x%x\n", mRasPolicy.PciePolicy.PerrPropEn));

  //
  // Leaky Bucket Feature (requires Gen4 IP)
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "ExpectedBer:  %ld\n", mRasPolicy.PciePolicy.ExpectedBer));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Gen12TimeWindow:  %d\n", mRasPolicy.PciePolicy.Gen12TimeWindow));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Gen34TimeWindow:  %d\n", mRasPolicy.PciePolicy.Gen34TimeWindow));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Gen12ErrorThreshold:  %d\n", mRasPolicy.PciePolicy.Gen12ErrorThreshold));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Gen34ErrorThreshold:  %d\n", mRasPolicy.PciePolicy.Gen34ErrorThreshold));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Gen34ReEqualization:  %d\n", mRasPolicy.PciePolicy.Gen34ReEqualization));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Gen2LinkDegradation:  %d\n", mRasPolicy.PciePolicy.Gen2LinkDegradation));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Gen3LinkDegradation:  %d\n", mRasPolicy.PciePolicy.Gen3LinkDegradation));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Gen4LinkDegradation:  %d\n", mRasPolicy.PciePolicy.Gen4LinkDegradation));
  //
  // PCIe DPC Support
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "EdpcEn:  0x%x\n", mRasPolicy.PciePolicy.EdpcEn));
  //
  // PPR
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PprType:  0x%x\n", mRasPolicy.ImcPolicy.PprType));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PprErrInjTest:  0x%x\n", mRasPolicy.ImcPolicy.PprErrInjTest));

  //
  // WHEA
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "WheaSupportEn:  0x%x\n", mRasPolicy.WheaPolicy.WheaSupportEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PMEMErrorInjEn:  0x%x\n", mRasPolicy.WheaPolicy.AEPErrorInjEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "WheaErrorInjSupportEn:  0x%x\n", mRasPolicy.WheaPolicy.WheaErrorInjSupportEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "WheaLogMemoryEn:  0x%x\n", mRasPolicy.WheaPolicy.WheaLogMemoryEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "WheaLogProcEn:  0x%x\n", mRasPolicy.WheaPolicy.WheaSupportEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "WheaLogPciEn:  0x%x\n", mRasPolicy.WheaPolicy.WheaLogMemoryEn));

  //
  // Error Control
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PatrolErrorDowngradeEn:  0x%x\n", mRasPolicy.ErrorControlPolicy.PatrolErrorDowngradeEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "Ce2LmLoggingEn:  0x%x\n", mRasPolicy.ErrorControlPolicy.Ce2LmLoggingEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "KtiFirstCeLatchEn:  0x%x\n", mRasPolicy.ErrorControlPolicy.KtiFirstCeLatchEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "LmceDowngradeEn:  0x%x\n", mRasPolicy.ErrorControlPolicy.LmceDowngradeEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "LlcEwbErrorControl:  0x%x\n", mRasPolicy.ErrorControlPolicy.LlcEwbErrorControl));
}

/**
  This implementation is to handle current RAS Policy items. If policies has potential conflict, it will be
  corrected in this procedure.

  @retval EFI_SUCCESS       Conflicts, if any, were resolved.
  @retval EFI_UNSUPPORTED   Could not get MemTopolgy.
**/
EFI_STATUS
EFIAPI
HandlePolicyConflict (
  VOID
  )
{
  MEM_TOPOLOGY            *MemTopology;
  BOOLEAN                 EmcaCap;

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
     RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetMemTopology failed \n"));
    return EFI_UNSUPPORTED;
  }

  //
  // Handle poison and viral conflicts.
  //
  if (mRasPolicy.PoisonEn == 0) {
    mRasPolicy.ViralEn = 0;
    mRasPolicy.ClearViralStatus = 0;
    mRasPolicy.McaPolicy.LmceEn = 0;
  }

  if ((MemTopology->SystemRasType != ADVANCED_RAS)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "Disable Advanced RAS feature on non-ADV RAS SKU\n"));
    mRasPolicy.ViralEn = 0;
    mRasPolicy.McBankErrorInjection = 0;
    if ((MemTopology->SystemRasType != STANDARD_PLUS_RAS)) {
      // do not clear LMCE for 14nm platforms and Standard part
      if ((IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) && (MemTopology->SystemRasType == STANDARD_RAS)) {
        // do nothing
      } else {
        mRasPolicy.McaPolicy.LmceEn = 0;
      }
    }
  }
  if ((MemTopology->SystemRasType != ADVANCED_RAS) && (MemTopology->SystemRasType != FPGA_RAS)){
    mRasPolicy.ImcPolicy.RankVlsAlways = FALSE;
  }

  if (mRasPolicy.McaPolicy.LmceEn == 1) {
    //
    // BIOS must ensure LLC EWB and MC patrol scrub are programmed to UCNA when enable LMCE.
    //
    mRasPolicy.ErrorControlPolicy.LmceDowngradeEn = 1;
    mRasPolicy.ErrorControlPolicy.LlcEwbErrorControl = 0;
  }

  if (mRasPolicy.ViralEn == 0) {
    mRasPolicy.ClearViralStatus = 0;
  }

  EmcaCap = (IsEmcaLogCap () || IsEmcaGen2Cap ());

  if (mRasPolicy.McaPolicy.CloakingEn) {
    //
    //Set Threshold to 0 when Cloaking is enable
    //
    mRasPolicy.McaPolicy.EmcaCsmiThreshold = 0;
    //
    //If Cloaking is enabled and Emca is disbled then disable cloaking.
    //
    if ((mRasPolicy.McaPolicy.EmcaEn == 0) || (EmcaCap == FALSE)) {
      mRasPolicy.McaPolicy.CloakingEn = 0;
    }
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "After HandlePolicyConflict:\n"));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "  PoisonEn:%x\n", mRasPolicy.PoisonEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "  ViralEn:%x\n", mRasPolicy.ViralEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "  ClearViralStatus:%x\n", mRasPolicy.ClearViralStatus));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "  IoMcaEn:%x\n", mRasPolicy.IioPolicy.IoMcaEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "  LmceEn:%x\n", mRasPolicy.McaPolicy.LmceEn));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "  EmcaCsmiThreshold:%x\n", mRasPolicy.McaPolicy.EmcaCsmiThreshold));

  return EFI_SUCCESS;
}

/*
VOID
EFIAPI
TestAddressDecode (
  VOID
  )
{
  UINT64  SystemAddressMatrix[8] = {
    0x100000,     0x120003200, 0x2400006400, 0x1a000aa00,
    0x160000bb0,  0x980000cc0, 0xaa00000dd0, 0x1a0044a0
};
  TRANSLATED_ADDRESS  Ta;
  UINT8               Index;

  for (Index =0; Index < 8; Index ++) {
    SystemAddressToDimmAddress((UINTN)SystemAddressMatrix[Index], &Ta);

    RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Test] Address: %lx  S:%d  Ch:%d dimm:%d Rank:%d  Bank GRP:%d  BankInGrp:%d  Col:%x  Row:%x \n",
        SystemAddressMatrix[Index], Ta.SocketId, Ta.ChannelId, Ta.DimmSlot, Ta.PhysicalRankId, Ta.BankGroup, Ta.Bank, Ta.Col, Ta.Row));
  }
}

*/


/**
  Entry point for the RAS policy driver.

  This function initializes RAS policy which all RAS drivers will leverage its produced data.


  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.

**/
EFI_STATUS
EFIAPI
InitializePolicySample (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS                Status;
  UINT32                    DebugLevel;
  UINT32                    SetupOptionLevel;
  SYSTEM_CONFIGURATION      SystemConfig;


  Status = GetSpecificConfigGuid (&gEfiSetupVariableGuid, &SystemConfig);
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "Failed to get setup variable, status = %r\n", Status));
    return Status;
  }

  SetupOptionLevel = SystemConfig.RasLogLevel;
  DebugLevel = GetDebugPrintErrorLevel () & (~(LEVEL_BASIC_FLOW | LEVEL_FUNC_FLOW | LEVEL_REG));

  if (SetupOptionLevel == 3) {
    DebugLevel |= LEVEL_BASIC_FLOW | LEVEL_FUNC_FLOW | LEVEL_REG;
  }
  else if (SetupOptionLevel == 2) {
    DebugLevel |= LEVEL_BASIC_FLOW | LEVEL_FUNC_FLOW;
  }
  else if (SetupOptionLevel == 1) {
    DebugLevel |= LEVEL_BASIC_FLOW;
  }

  // Override RasLogLevel when serial message is disabled
  if (SystemConfig.serialDebugMsgLvl == 0)
  {
    DebugLevel = 0;
  }

  SetDebugPrintErrorLevel (DebugLevel);

  RAS_DEBUG ((LEVEL_FUNC_FLOW,"InitializePolicySample!\n"));

  Status = InitializePolicyData ();
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "%a: InitializePolicyData failed, Status =%r\n", __FUNCTION__, Status));
    return Status;
  }

  Status = HandlePolicyConflict ();
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "HandlePolicyConflict failed!\n"));
  }

  DisplayRasPolicy ();

  Status = gSmst->SmmInstallProtocolInterface (
                    &mHandle,
                    &gRasPolicyProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mRasPolicy
                    );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "%a: Install gRasPolicyProtocolGuid failed, Status =%r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
  }


  return Status;
}
