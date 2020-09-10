/** @file
  Interface of get MailBoxLib.

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

#ifndef __RAS_MAIL_BOX_LIB_H__
#define __RAS_MAIL_BOX_LIB_H__


#pragma pack(1)

typedef struct {
  UINT8   PcieAerCorrErrEn;
  UINT32  PcieAerCorrErrBitMap;
  UINT8   PcieAerAdNfatErrEn;
  UINT8   PcieAerNfatErrEn;
  UINT32  PcieAerNfatErrBitMap;
  UINT8   PcieAerFatErrEn;
  UINT32  PcieAerFatErrBitMap;
  UINT8   PcieCorrErrEn;
  UINT8   PcieUncorrErrEn;
  UINT8   PcieFatalErrEn;
  UINT8   PcieAerEcrcEn;
  UINT8   PcieAerUreEn;
  UINT8   PcieAerSurpriseLinkDownEn;
  UINT8   PcieAerErrsidWa;
  UINT8   SerrPropEn;
  UINT8   PerrPropEn;
  UINT8   SerrEmuTestEn;
  UINT8   PcieCorErrCntr;
  UINT16  PcieCorErrThres;
  UINT8   PcieCorErrLimitEn;
  UINT32  PcieCorErrLimit;
  UINT8   PcieErrEn;
  //
  // PCIe Error Filter Support
  //
  VOID    *PcieErrFilterTable;
  UINT8   PcieErrFilterCount;
  //
  // PCIe Leaky Bucket Feature (requires Gen4 IP)
  //
  UINT64  ExpectedBer;
  UINT32  Gen12TimeWindow;
  UINT8   Gen34TimeWindow;
  UINT8   Gen12ErrorThreshold;
  UINT8   Gen34ErrorThreshold;
  UINT8   Gen34ReEqualization;
  UINT8   Gen2LinkDegradation;
  UINT8   Gen3LinkDegradation;
  UINT8   Gen4LinkDegradation;
  //
  //      PCIe DPC support
  //
  UINT8   EdpcEn;
  UINT8   EdpcInterrupt;
  UINT8   EdpcErrCorMsg;
} PCIE_INIT_PAR;

typedef struct  {
  UINT8   IioPcieAddCorrErrorEn;
  UINT8   IioPcieAddUnCorrEn ;
  UINT32  IioPcieAddUnCorrBitMap;
  UINT32  IioPcieAddUnCorrSevBitMap;
  UINT8   IioErrRegistersClearEn;
  UINT8   DisableLerMAerrorLogging;
  UINT8   LerEn;
  UINT8   IioVtdErrorEn;
  UINT8   IioIrpErrorEn;
  UINT32  IioIrpp0ErrCtlBitMap;
  UINT32  IioCohSevBitMap;
  UINT8   IioMiscErrorEn;
  UINT32  IioMiscErrorBitMap;          // MIERRCTL
  UINT32  IioMiscErrorSevMap;          // MIERRSV
  UINT32  IioItcErrSevBitMap;
  UINT8   IioErrorEn;
  UINT8   IoMcaEn;
  UINT8   IioSev1Pcc;
  UINT8   IioErrorPin0En;
  UINT8   IioErrorPin1En;
  UINT8   IioErrorPin2En;
  UINT8   IioDmiErrorEn;
  UINT32  IioOtcErrSevBitMap;
  UINT8   IioDmaErrorEn;
  UINT32  IehLocalUcMask;
  UINT32  IehLocalUcSeverity;
  UINT32  IehLocalCeMask;
  UINT32  IioDmaBitMap;                // CHANERRMSK
  UINT32  IioDmaSevBitMap;             // CHANERRSEV
  UINT32  IioLocalUcMask;
  UINT32  IioLocalUcSeverity;
  UINT32  IioLocalCEMask;
  UINT8   IioPoisonEn;
  UINT8   IioViralEn;
  UINT8   IioClearViralStatus;
  UINT8   IioRootCtlOverride;
  UINT8   OsNativeAerSupport;
  UINT8   ItcOtcCaMaEnable;
  UINT8   IioOOBMode;
  UINT8   IioPcieAerSpecCompEn;
}  IIO_INIT_PAR;

typedef struct _MISC_PAR {
   BOOLEAN  PcieNonFatalErrDetected;
   BOOLEAN  PcieFatalErrDetected;
   UINT8    PciXPUnsupportedRequest;
   UINT32   OutputSeverity;
   LIST_ENTRY   *PtrPcieCeCounterList;
 } MISC_PAR;

typedef struct {
  PCIE_INIT_PAR  PcieInitPar;
  IIO_INIT_PAR   IioInitPar;
  MISC_PAR       MiscPar;
} RAS_MAIL_BOX;

#pragma pack()


/**
  This function is implemented to return RAS_MAIL_BOX data.

  @return   Pointer to RAS_MAIL_BOX data.
**/
RAS_MAIL_BOX*
EFIAPI
GetRasMailBox (
  VOID
  );


/**
  This implementation is to set output severity for interrupt.

  @param[in]  Severity  -- severity value
**/
VOID
EFIAPI
AdjustOutputSeverity (
  UINT32  Severity
  );

/**
  This implementation is to override output severity for interrupt.

  @param[in]  Severity  -- severity value.
**/
VOID
EFIAPI
OverrideOutputSeverity (
  UINT32  Severity
  );

/**
  This implementation is to override output severity for interrupt.

  @param   none

  return  current global severity
**/
UINT32
EFIAPI
GetOutputSeverity (
  VOID
  );

#endif

