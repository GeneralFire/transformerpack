/** @file
  Implementation of mailbox initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2019 Intel Corporation. <BR>

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

#include <Library/RasDebugLib.h>
#include <Library/RasMailBoxLib.h>
#include <Library/PlatPolicyLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Upi/RasTypes.h>
/**
  This routine purpose is to pass policy data to mailbox

  @param  PolicyData - pointer to policy data
  @param  MailBox - pointer to mailbox
  @retval none
**/
VOID
EFIAPI
GetPcieInitPars (
  RAS_POLICY       *PolicyData,
  RAS_MAIL_BOX     *MailBox
  )
{
  if (MailBox != NULL) {
    MailBox->PcieInitPar.PcieAerCorrErrEn = PolicyData->PciePolicy.PcieAerCorrErrEn;
    MailBox->PcieInitPar.PcieAerCorrErrBitMap = PolicyData->PciePolicy.PcieAerCorrErrBitMap;
    MailBox->PcieInitPar.PcieAerAdNfatErrEn = PolicyData->PciePolicy.PcieAerAdNfatErrEn;
    MailBox->PcieInitPar.PcieAerNfatErrEn = PolicyData->PciePolicy.PcieAerNfatErrEn;
    MailBox->PcieInitPar.PcieAerNfatErrBitMap = PolicyData->PciePolicy.PcieAerNfatErrBitMap;
    MailBox->PcieInitPar.PcieAerFatErrEn = PolicyData->PciePolicy.PcieAerFatErrEn ;
    MailBox->PcieInitPar.PcieAerFatErrBitMap = PolicyData->PciePolicy.PcieAerFatErrBitMap;
    MailBox->PcieInitPar.PcieAerEcrcEn = PolicyData->PciePolicy.PcieAerEcrcEn;
    MailBox->PcieInitPar.PcieAerUreEn = PolicyData->PciePolicy.PcieAerUreEn;
    MailBox->PcieInitPar.PcieAerSurpriseLinkDownEn = PolicyData->PciePolicy.PcieAerSurpriseLinkDownEn;
    MailBox->PcieInitPar.PcieAerErrsidWa = PolicyData->PciePolicy.PcieAerErrsidWa;
    MailBox->PcieInitPar.PcieCorrErrEn = PolicyData->PciePolicy.PcieCorrErrEn;
    MailBox->PcieInitPar.PcieUncorrErrEn = PolicyData->PciePolicy.PcieUncorrErrEn;
    MailBox->PcieInitPar.PcieFatalErrEn = PolicyData->PciePolicy.PcieFatalErrEn;
    MailBox->PcieInitPar.SerrPropEn = PolicyData->PciePolicy.SerrPropEn;
    MailBox->PcieInitPar.PerrPropEn = PolicyData->PciePolicy.PerrPropEn;
    MailBox->PcieInitPar.SerrEmuTestEn = PolicyData->PciePolicy.SerrEmuTestEn;
    MailBox->PcieInitPar.PcieCorErrCntr = PolicyData->PciePolicy.PcieCorErrCntr;
    MailBox->PcieInitPar.PcieCorErrThres = PolicyData->PciePolicy.PcieCorErrThres;
    MailBox->PcieInitPar.PcieCorErrLimitEn = PolicyData->PciePolicy.PcieCorErrLimitEn;
    MailBox->PcieInitPar.PcieCorErrLimit = PolicyData->PciePolicy.PcieCorErrLimit;
    MailBox->PcieInitPar.PcieErrEn = PolicyData->PciePolicy.PcieErrEn;
    //
    // PCIe Error Filter Support
    //
    MailBox->PcieInitPar.PcieErrFilterCount = PolicyData->PciePolicy.PcieErrFilterCount;
    MailBox->PcieInitPar.PcieErrFilterTable = PolicyData->PciePolicy.PcieErrFilterTable;
    //
    // Leaky Bucket Feature (requires Gen4 IP)
    //
    MailBox->PcieInitPar.ExpectedBer = PolicyData->PciePolicy.ExpectedBer;
    MailBox->PcieInitPar.Gen12TimeWindow = PolicyData->PciePolicy.Gen12TimeWindow;
    MailBox->PcieInitPar.Gen34TimeWindow = PolicyData->PciePolicy.Gen34TimeWindow;
    MailBox->PcieInitPar.Gen12ErrorThreshold = PolicyData->PciePolicy.Gen12ErrorThreshold;
    MailBox->PcieInitPar.Gen34ErrorThreshold = PolicyData->PciePolicy.Gen34ErrorThreshold;
    MailBox->PcieInitPar.Gen34ReEqualization = PolicyData->PciePolicy.Gen34ReEqualization;
    MailBox->PcieInitPar.Gen2LinkDegradation = PolicyData->PciePolicy.Gen2LinkDegradation;
    MailBox->PcieInitPar.Gen3LinkDegradation = PolicyData->PciePolicy.Gen3LinkDegradation;
    MailBox->PcieInitPar.Gen4LinkDegradation = PolicyData->PciePolicy.Gen4LinkDegradation;
    //
    // PCIE DPC Support
    //
    MailBox->PcieInitPar.EdpcEn = PolicyData->PciePolicy.EdpcEn;
    MailBox->PcieInitPar.EdpcInterrupt = PolicyData->PciePolicy.EdpcInterrupt;
    MailBox->PcieInitPar.EdpcErrCorMsg = PolicyData->PciePolicy.EdpcErrCorMsg;

    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieAerCorrErrEn = 0x%x\n", MailBox->PcieInitPar.PcieAerCorrErrEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieAerCorrErrBitMap = 0x%x\n", MailBox->PcieInitPar.PcieAerCorrErrBitMap));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieAerAdNfatErrEn = 0x%x\n", MailBox->PcieInitPar.PcieAerAdNfatErrEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieAerNfatErrEn = 0x%x\n", MailBox->PcieInitPar.PcieAerNfatErrEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieAerNfatErrBitMap = 0x%x\n", MailBox->PcieInitPar.PcieAerNfatErrBitMap));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieAerFatErrEn = 0x%x\n", MailBox->PcieInitPar.PcieAerFatErrEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieAerFatErrBitMap = 0x%x\n", MailBox->PcieInitPar.PcieAerFatErrBitMap));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieCorrErrEn = 0x%x\n", MailBox->PcieInitPar.PcieCorrErrEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieUncorrErrEn = 0x%x\n", MailBox->PcieInitPar.PcieUncorrErrEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieFatalErrEn = 0x%x\n", MailBox->PcieInitPar.PcieFatalErrEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieAerEcrcEn = 0x%x \n",MailBox->PcieInitPar.PcieAerEcrcEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieAerUreEn = 0x%x \n",MailBox->PcieInitPar.PcieAerUreEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieAerSurpriseLinkDownEn = 0x%x \n",MailBox->PcieInitPar.PcieAerSurpriseLinkDownEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars SerrPropEn = 0x%x\n", MailBox->PcieInitPar.SerrPropEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PerrPropEn = 0x%x\n", MailBox->PcieInitPar.PerrPropEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieCorErrCntr = 0x%x\n", MailBox->PcieInitPar.PcieCorErrCntr));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars PcieCorErrThres = 0x%x\n", MailBox->PcieInitPar.PcieCorErrThres));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars SerrEmuTestEn = 0x%x\n", MailBox->PcieInitPar.SerrEmuTestEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars ExpectedBer = %ld\n", MailBox->PcieInitPar.ExpectedBer));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars Gen12TimeWindow = %d\n", MailBox->PcieInitPar.Gen12TimeWindow));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars Gen34TimeWindow = %d\n", MailBox->PcieInitPar.Gen34TimeWindow));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars Gen12ErrorThreshold = %d\n", MailBox->PcieInitPar.Gen12ErrorThreshold));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars Gen34ErrorThreshold = %d\n", MailBox->PcieInitPar.Gen34ErrorThreshold));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars Gen34ReEqualization = %d\n", MailBox->PcieInitPar.Gen34ReEqualization));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars Gen2LinkDegradation = %d\n", MailBox->PcieInitPar.Gen2LinkDegradation));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars Gen3LinkDegradation = %d\n", MailBox->PcieInitPar.Gen3LinkDegradation));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars Gen4LinkDegradation = %d\n", MailBox->PcieInitPar.Gen4LinkDegradation));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars EdpcEn = %d\n", MailBox->PcieInitPar.EdpcEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars EdpcInterrupt = %d\n", MailBox->PcieInitPar.EdpcInterrupt));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars EdpcErrCorMsg = %d\n", MailBox->PcieInitPar.EdpcErrCorMsg));

  } else {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "GetPcieInitPars invalid parameters!\n"));
  }
}

/**
  This routine purpose is to pass policy data to mailbox

  @param  PolicyData - pointer to policy data
  @param  MailBox - pointer to mailbox
  @retval none
**/
VOID
EFIAPI
GetIioInitPars(
  RAS_POLICY       *PolicyData,
  RAS_MAIL_BOX     *MailBox
  )
{
  if (MailBox != NULL) {
    MailBox->IioInitPar.IioVtdErrorEn = PolicyData->IioPolicy.IioVtdErrorEn;
    MailBox->IioInitPar.IioPcieAddCorrErrorEn = PolicyData->IioPolicy.IioPcieAddCorrErrorEn;
    MailBox->IioInitPar.IioPcieAddUnCorrEn =  PolicyData->IioPolicy.IioPcieAddUnCorrEn;
    MailBox->IioInitPar.IioPcieAddUnCorrBitMap = PolicyData->IioPolicy.IioPcieAddUnCorrBitMap;
    MailBox->IioInitPar.IioPcieAddUnCorrSevBitMap  = PolicyData->IioPolicy.IioPcieAddUnCorrSevBitMap;
    MailBox->IioInitPar.DisableLerMAerrorLogging = PolicyData->IioPolicy.DisableLerMAerrorLogging;
    MailBox->IioInitPar.IioErrRegistersClearEn = PolicyData->IioPolicy.IioErrRegistersClearEn;
    MailBox->IioInitPar.LerEn = PolicyData->IioPolicy.LerEn;
    MailBox->IioInitPar.IioItcErrSevBitMap = PolicyData->IioPolicy.IioItcErrSevBitMap;
    MailBox->IioInitPar.IioMiscErrorEn = PolicyData->IioPolicy.IioMiscErrorEn;
    MailBox->IioInitPar.IioMiscErrorSevMap = PolicyData->IioPolicy.IioMiscErrorSevMap;
    MailBox->IioInitPar.IioMiscErrorBitMap = PolicyData->IioPolicy.IioMiscErrorBitMap;
    MailBox->IioInitPar.IioErrorEn = PolicyData->IioPolicy.IioErrorEn;
    MailBox->IioInitPar.IoMcaEn = PolicyData->IioPolicy.IoMcaEn;
    MailBox->IioInitPar.IioOOBMode = PolicyData->IioPolicy.IioOOBMode;
    MailBox->IioInitPar.IioSev1Pcc = PolicyData->IioPolicy.IioSev1Pcc;
    MailBox->IioInitPar.IioErrorPin0En = PolicyData->IioPolicy.IioErrorPin0En;
    MailBox->IioInitPar.IioErrorPin1En = PolicyData->IioPolicy.IioErrorPin1En;
    MailBox->IioInitPar.IioErrorPin2En = PolicyData->IioPolicy.IioErrorPin2En;
    MailBox->IioInitPar.IioDmiErrorEn = PolicyData->IioPolicy.IioDmiErrorEn;
    MailBox->IioInitPar.IioOtcErrSevBitMap = PolicyData->IioPolicy.IioOtcErrSevBitMap;
    MailBox->IioInitPar.IioDmaErrorEn = PolicyData->IioPolicy.IioDmaErrorEn;
    MailBox->IioInitPar.IioDmaBitMap = PolicyData->IioPolicy.IioDmaBitMap;
    MailBox->IioInitPar.IioDmaSevBitMap = PolicyData->IioPolicy.IioDmaSevBitMap;
    MailBox->IioInitPar.IioIrpErrorEn  = PolicyData->IioPolicy.IioIrpErrorEn;
    MailBox->IioInitPar.IehLocalUcMask = PolicyData->IioPolicy.IehLocalUcMask;
    MailBox->IioInitPar.IehLocalUcSeverity = PolicyData->IioPolicy.IehLocalUcSeverity;
    MailBox->IioInitPar.IehLocalCeMask = PolicyData->IioPolicy.IehLocalCeMask;
    MailBox->IioInitPar.IioPoisonEn = PolicyData->PoisonEn;
    MailBox->IioInitPar.IioViralEn = PolicyData->ViralEn;
    MailBox->IioInitPar.IioClearViralStatus = PolicyData->ClearViralStatus;
    MailBox->IioInitPar.IioRootCtlOverride = PolicyData->IioPolicy.IioRootCtlOverride;
    MailBox->IioInitPar.OsNativeAerSupport = PolicyData->IioPolicy.OsNativeAerSupport;
    MailBox->IioInitPar.ItcOtcCaMaEnable = PolicyData->IioPolicy.ItcOtcCaMaEnable;
    MailBox->IioInitPar.IioPcieAerSpecCompEn = PolicyData->IioPolicy.IioPcieAerSpecCompEn;

    RAS_DEBUG ((LEVEL_FUNC_FLOW, "MailBox->IioInitPar.IioPcieAddCorrErrorEn = 0x%x\n", MailBox->IioInitPar.IioPcieAddCorrErrorEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "MailBox->IioInitPar.IioPcieAddUnCorrEn = 0x%x\n", MailBox->IioInitPar.IioPcieAddUnCorrEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "MailBox->IioInitPar.IioPcieAddUnCorrBitMap = 0x%x\n", MailBox->IioInitPar.IioPcieAddUnCorrBitMap));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "MailBox->IioInitPar.IioPcieAddUnCorrSevBitMap = 0x%x\n", MailBox->IioInitPar.IioPcieAddUnCorrSevBitMap));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "MailBox->IioInitPar.OsNativeAerSupport = 0x%x\n", MailBox->IioInitPar.OsNativeAerSupport));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "MailBox->IioInitPar.IioPcieAddCorrErrorEn = 0x%x\n", MailBox->IioInitPar.IioPcieAddCorrErrorEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "MailBox->IioInitPar.IioPcieAddUnCorrEn = 0x%x\n", MailBox->IioInitPar.IioPcieAddUnCorrEn));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "MailBox->IioInitPar.IioPcieAddUnCorrBitMap = 0x%x\n", MailBox->IioInitPar.IioPcieAddUnCorrBitMap));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "MailBox->IioInitPar.IioPcieAddUnCorrSevBitMap = 0x%x\n", MailBox->IioInitPar.IioPcieAddUnCorrSevBitMap));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "MailBox->IioInitPar.IioPcieAerSpecCompEn = 0x%x\n", MailBox->IioInitPar.IioPcieAerSpecCompEn));
 } else {
    RAS_DEBUG((LEVEL_FUNC_FLOW, "GetIioInitPars invalide parameters!\n"));
  }
}




