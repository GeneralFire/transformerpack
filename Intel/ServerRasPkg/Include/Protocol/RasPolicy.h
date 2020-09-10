/** @file
  Interface of RAS policy protocol GUID.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef __RAS_POLICY_PROTOCOL_H__
#define __RAS_POLICY_PROTOCOL_H__


#pragma pack(1)
typedef struct{
  //Cloaking
  UINT8  CloakingEn;


  //Enhance logging
  UINT8  ElogCorrErrEn;

  //Local MCE
  UINT8  LmceEn;

  //EMCA2
  UINT8  EmcaEn;
  UINT8  EmcaMsmiEn;
  UINT32 MSMIBankBitFieldEn;
  UINT8  EmcaCsmiEn;
  UINT16 EmcaCsmiThreshold;
  UINT8  CsmiDynamicDisable;
  UINT32 CSMIBankBitFieldEn;
  UINT8  ElogMemErrEn;
  UINT8  EmcaIgnOptin;
  UINT8  ElogProcErrEn;
  UINT8  McBankWarmBootClearEn;
  UINT8  OscEn;
  UINT8  McaSpinLoop;
  UINT8  UboxErrorMask;
} MCA_POLICY;

typedef struct{
  //total correctable error
  UINT8  MemErrEn;
  UINT8  CorrMemErrEn;
  UINT8  SpareIntSelect;
  UINT8  MemCeFloodPolicy;
  UINT8  CorrecErrTh;
  // timestamp +>>
  UINT16 SpareSwErrTh;
  UINT16 TimeWindow;
  UINT8  TriggerSWErrThEn;
  // timestamp +<<



  //ADDDC
  UINT8  ADDDCEn;
  BOOLEAN  RankVlsAlways;
  //PCLS
  UINT8  PclsEn;



  //Crystal Ridge
  UINT8  FnvErrorEn;
  UINT8  FnvErrorLowPrioritySignal;
  UINT8  FnvErrorHighPrioritySignal;
  UINT8  NgnAddressRangeScrub;
  UINT8  NgnHostAlertDpa;
  UINT8  NgnHostAlertPatrolScrubUNC;
  UINT8  ReportAlertSPA;
  UINT8  DcpmmEccModeSwitch;
  UINT8  DcpmmUncPoison;
  UINT8  DdrtInternalAlertEn;

  //PPR
  UINT8  PprType;
  UINT8  PprErrInjTest;



  //Misc
  UINT8  SaiPolicyGroupWaBiosW;
  UINT16 SpareErrTh;

} IMC_POLICY;

typedef struct {
  UINT8                   IioErrorEn;
  UINT8                   OsNativeAerSupport;
  UINT8                   IoMcaEn;
  UINT8                   IioSev1Pcc;
  UINT8                   IioErrorPin0En;
  UINT8                   IioErrorPin1En;
  UINT8                   IioErrorPin2En;
  UINT8                   LerEn;
  UINT8                   DisableLerMAerrorLogging;
  UINT8                   IioIrpErrorEn;
  UINT32                  IioIrpp0ErrCtlBitMap;
  UINT64                  IioCohSevBitMap;             // IRPPERRSV
  UINT8                   IioMiscErrorEn;
  UINT32                  IioMiscErrorBitMap;          // MIERRCTL
  UINT32                  IioMiscErrorSevMap;          // MIERRSV
  UINT8                   IioVtdErrorEn;
  UINT32                  IioItcErrSevBitMap;          // ITCERRSEV
  UINT32                  IioOtcErrSevBitMap;          // OTCERRSEV
  UINT8                   IioDmaErrorEn;
  UINT8                   IioDmiErrorEn;
  UINT32                  IioDmaBitMap;                // CHANERRMSK
  UINT32                  IioDmaSevBitMap;             // CHANERRSEV
  UINT8                   IioPcieAddCorrErrorEn;
  UINT8                   IioPcieAddUnCorrEn;
  UINT32                  IioPcieAddUnCorrBitMap;      // XPUNCEDMASK, XPUNCERRMSK
  UINT32                  IioPcieAddUnCorrSevBitMap;   // XPUNCERRSEV
  UINT8                   IioPcieAddRcvComWithUr;
  UINT8                   IioPcieAerSpecCompEn;
  UINT8                   IioErrRegistersClearEn;
  UINT32                  IehLocalUcMask;
  UINT32                  IehLocalUcSeverity;
  UINT32                  IehLocalCeMask;
  UINT8                   IioRootCtlOverride;
  UINT8                   ItcOtcCaMaEnable;
  UINT8                   IioOOBMode;
} IIO_POLICY;

typedef struct {
  UINT8                   PcieErrEn;
  UINT8                   PcieCorrErrEn;
  UINT8                   PcieUncorrErrEn;
  UINT8                   PcieFatalErrEn;
  UINT8                   PcieCorErrCntr;
  UINT16                  PcieCorErrThres;
  UINT8                   PcieCorErrLimitEn;
  UINT32                  PcieCorErrLimit;
  UINT8                   PcieAerCorrErrEn;
  UINT32                  PcieAerCorrErrBitMap;     // COREDMASK, CORERRMSK
  UINT8                   PcieAerAdNfatErrEn;
  UINT8                   PcieAerNfatErrEn;
  UINT32                  PcieAerNfatErrBitMap;     // UNCERRMSK, UNCEDMASK, UNCERRSEV
  UINT8                   PcieAerFatErrEn;
  UINT32                  PcieAerFatErrBitMap;      // UNCERRMSK, UNCEDMASK, UNCERRSEV
  UINT8                   PcieAerEcrcEn;
  UINT8                   PcieAerUreEn;
  UINT8                   PcieAerSurpriseLinkDownEn;
  UINT8                   PcieAerErrsidWa;
  UINT8                   SerrPropEn;
  UINT8                   PerrPropEn;
  UINT8                   SerrEmuTestEn; //per FW eval team's request, add this option for them to use Pcie Agilent card  to emulate to test SERR/PERR function.
  //
  // PCIe Error Filter Support
  //
  VOID                   *PcieErrFilterTable;
  UINT8                  PcieErrFilterCount;
  //
  // PCIe Leaky Bucket Feature (requires Gen4 IP)
  //
  UINT64                  ExpectedBer;
  UINT32                  Gen12TimeWindow;
  UINT8                   Gen34TimeWindow;
  UINT8                   Gen12ErrorThreshold;
  UINT8                   Gen34ErrorThreshold;
  UINT8                   Gen34ReEqualization;
  UINT8                   Gen2LinkDegradation;
  UINT8                   Gen3LinkDegradation;
  UINT8                   Gen4LinkDegradation;
  //
  // PCie DPC Support
  //
  UINT8                   EdpcEn;
  UINT8                   EdpcInterrupt;
  UINT8                   EdpcErrCorMsg;
} PCIE_POLICY;

typedef struct {
  UINT8                   WheaSupportEn;
  UINT8                   WheaLogMemoryEn;
  UINT8                   WheaLogProcEn;
  UINT8                   WheaLogPciEn;
  //
  //error injection
  //
  UINT8                   AEPErrorInjEn;
  UINT8                   WheaErrorInjSupportEn;
  UINT8                   McaBankErrInjEn;
  UINT8                   WheaACPI50ErrInjEn;
  UINT8                   WheaPcieErrInjEn;
  UINT8                   PcieErrInjActionTable;
  UINT8                   MeSegErrorInjEn;
} WHEA_POLICY;

typedef struct {
  UINT8   PatrolErrorDowngradeEn;
  UINT8   Ce2LmLoggingEn;
  UINT8   KtiFirstCeLatchEn;
  UINT8   LmceDowngradeEn;
  UINT8   LlcEwbErrorControl;
} ERROR_CONTROL_POLICY;

// APTIOV_SERVER_OVERRIDE_RC_START: Added support to Unlock MSR 0x790 for Error Injection Tools to work.
typedef struct {
  UINT8                   UnlockMsr;
} AMI_RAS_POLICY;
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to Unlock MSR 0x790 for Error Injection Tools to work.

typedef struct{
  UINT32                  RasRevision;
  UINT8                   SystemErrorEn;
  UINT8                   FatalErrSpinLoopEn;
  MCA_POLICY              McaPolicy;
  IMC_POLICY              ImcPolicy;
  IIO_POLICY              IioPolicy;
  PCIE_POLICY             PciePolicy;
  WHEA_POLICY             WheaPolicy;
  ERROR_CONTROL_POLICY    ErrorControlPolicy;
  UINT8                   PoisonEn;
  UINT8                   ViralEn;
  UINT8                   CloakDevHideRegistersOs;
  UINT8                   ClearViralStatus;
  UINT8                   SmbusErrorRecovery;
  UINT8                   McBankErrorInjection;
  UINT8                   CrashLogFeature;
  UINT8                   CrashLogOnAllReset;
  UINT8                   CrashLogClear;
  UINT8                   CrashLogReArm;
  UINT8                   ShutdownSuppression;
// APTIOV_SERVER_OVERRIDE_RC_START: Added support to Unlock MSR 0x790 for Error Injection Tools to work.
  AMI_RAS_POLICY          AmiRasPolicy;
// APTIOV_SERVER_OVERRIDE_RC_END: Added support to Unlock MSR 0x790 for Error Injection Tools to work.
} RAS_POLICY;
#pragma pack()

extern EFI_GUID   gRasPolicyProtocolGuid;

#endif
