/** @file
  Implementation of Iio  lib.

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

#include "IioRasLibInternal.h"
#include <Library/ErrorReportLib.h>

/**
  This function clears the IIO root port status registers for the
  specified PCI Express device.

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.

  @retval    None
**/
VOID
EFIAPI
ClearRootPortStatus (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      UINT32  ErrSeverity
  )
{
  PcieClearDeviceStatus(Socket, Bus,  Device, Function, NULL, ErrSeverity);

  if (IsIioRootPort(Socket, Bus, Device, Function, NULL)) {
    IioXpClearErr (Socket, Bus, Device, Function);
  }
}

 /**
   This function masks PCI exp errors reporting.

   @retval    Status.
 **/
 VOID
 EFIAPI
 IioPcieMaskErrors (
  IN   UINT8                    Socket,
  IN   UINT8                    Bus,
  IN   UINT8                    Device,
  IN   UINT8                    Function
  )
{
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioPcieMaskErrors!\n"));

  if ((PcieIsDevicePresent (Socket, Bus, Device, Function, NULL)) &&
      (IsIioRootPort (Socket, Bus, Device, Function, NULL) || PcieIsDmiPort (Socket, Bus, Device, Function, NULL))) {
    SetXPMaskError (Socket, Bus, Device, Function);

    PcieAerProgramCorrErrMsk (Socket, Bus, Device, Function, NULL, PCIEAER_CORRERR_MSK);
    PcieAerProgramUncErrMsk (Socket, Bus, Device, Function, NULL, PCIEAER_UNCORRERR_MSK);
  }
}

/**
  Enable IIO PCIe Root Port error reporting.

  @param[in]  Skt       The socket number of the root port.
  @param[in]  Bus       The PCI bus number of the root port.
  @param[in]  Device    The PCI device number of the root port.
  @param[in]  Function  The PCI function number of the root port.
**/
VOID
EFIAPI
IioRootPortErrorEnable (
  IN      UINT8   Skt,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  UINT8                            SecondaryBusNumber;
  RAS_MAIL_BOX  *MailBox = NULL;

  MailBox = GetRasMailBox ();

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IioRootPortErrorEnable Bus:0x%x, Device:0x%x, Function:0x%x.\n", Bus, Device, Function));
  if (!PcieIsDevicePresent (Skt, Bus, Device, Function, NULL)) {
    return;
  }

  //
  // Clear the status registers
  //
  ClearRootPortStatus (Skt, Bus, Device, Function, NONFATAL_ERROR_TYPE | FATAL_ERROR_TYPE | COR_ERROR_TYPE);

  //
  //Ony FFM mode, disable MSI capability. In case OS native mode, leave MSI capabilty enabled (default value).
  //BIOS default initialize FFM whatever OsNativeAerSupport value.
  //In case OS has AER capability and setup opt-in, ASL code will disable SMI deliver.
  //
  if(MailBox->IioInitPar.OsNativeAerSupport == 0) {
    IioDisMsiError (Skt, Bus, Device, Function);

    //
    //only FFM need to set override, OS native mode leave it as default value(No overide), hence give ASL chance to do switch.
    //
    IioProgramErrors (Skt, Bus, Device, Function);
  }

  IioXPProgramCorrErrors (Skt, Bus, Device, Function);

  IioXPProgramUnCorrErrors (Skt, Bus, Device, Function);

  IioXPProgramErrSev (Skt, Bus, Device, Function);

  RppIoErrEnable (Skt, Bus, Device, Function);

  IioProgramLeakyBucket (Skt, Bus, Device, Function);

  SecondaryBusNumber = PcieGetSecondaryBus (Skt, Bus, Device, Function, NULL);

  if (SecondaryBusNumber != 0) {
    if (PciDeviceOnBus (Skt, SecondaryBusNumber, NULL)) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "Secondary device on B%x:D%x:F%x\n", Bus, Device, Function));
      PcieConfigDeviceAndSubDevices (Skt, Bus, Device, Function, NULL);
    } else {
      PcieConfigDevice (Skt, Bus, Device, Function, NULL);
    }
  }

  //
  // Enable PCIe corrected Error counter threshold and set the mask if counter is enabled
  // Put program correctable error counter after Root Port AER Config, because PCIE GEN4 Root Port
  // need to reconfigure AER CORERRMSK once correctable error counter enabled.
  //
  IioProgramCorrErrCnt (Skt, Bus, Device, Function);


  DumpIohPcieRegisters (Skt, Bus, Device, Function);
}

/**
  This function is to handle IIO Root Port device XP Error

  @param[in]  Socket      Socket number.
  @param[in]  Bus         PCI bus number.
  @param[in]  Device      PCI device number.
  @param[in]  Function    PCI function number.
  @param[in]  ErrSeverity IIO Internal Error Severity.
**/
VOID
EFIAPI
IioXpErrHandler (
  IN    UINT8                        Socket,
  IN    UINT8                        Bus,
  IN    UINT8                        Device,
  IN    UINT8                        Function,
  IN    UINT8                        ErrSeverity
  )
{
  BOOLEAN            ErrorFound;
  UINT32             XpCorErrSts;
  UINT32             XpUncErrSts;
  UINT32             XpGlbErrSts;

  ErrorFound = IioGetXpErrorStatus (Socket, Bus, Device, Function, &XpCorErrSts, &XpUncErrSts, &XpGlbErrSts);

  if (ErrorFound) {
    ReportIioXpErrorLog (
      Socket,
      Bus,
      Device,
      Function,
      XpCorErrSts,
      XpUncErrSts,
      ErrSeverity);
  }

}

/**
  IIO PCIe leaky bucket handler.

  @param[in]  Socket    Socket number.
  @param[in]  Bus       PCI bus number.
  @param[in]  Device    PCI device number.
  @param[in]  Function  PCI function number.
  @param[in]  Severity  Error severity.
**/
VOID
EFIAPI
IioPcieLeakyBucketHandler (
  IN    UINT8   Socket,
  IN    UINT8   Bus,
  IN    UINT8   Device,
  IN    UINT8   Function,
  IN    UINT32  Severity
  )
{
  UINT16      LaneErrorMask;
  UINT8       LaneErrorCount[16];
  BOOLEAN     Gen34ReEqualize;
  BOOLEAN     Gen2LinkDegrade;
  BOOLEAN     Gen3LinkDegrade;
  BOOLEAN     Gen4LinkDegrade;
  EFI_STATUS  Status;

  if ((Severity & COR_ERROR_TYPE) == 0) {
    return;
  }

  LaneErrorMask = GetLeakyBucketLaneErrorMask (
    Socket,
    Bus,
    Device,
    Function,
    LaneErrorCount,
    &Gen34ReEqualize,
    &Gen2LinkDegrade,
    &Gen3LinkDegrade,
    &Gen4LinkDegrade
    );

  if (LaneErrorMask == 0) {
    return;
  }

  Status = ReportLeakyBucketErrorLog (
    0,  // Segment
    Bus,
    Device,
    Function,
    LaneErrorMask,
    LaneErrorCount,
    Gen34ReEqualize,
    Gen2LinkDegrade,
    Gen3LinkDegrade,
    Gen4LinkDegrade
    );
  ASSERT_EFI_ERROR (Status);  // just continue if ASSERT is disabled

  ClearLeakyBucketStatus (Socket, Bus, Device, Function);
}

/**
  This function is to handle IIO Internal Device (VTD, CBDMA, IRP, IRPRING, ITC, OTC, XP, MISC ) Error

  @param[in]  Socket      Socket number.
  @param[in]  IioStack    Stack number.
  @param[in]  Bus         PCI bus number.
  @param[in]  Device      PCI device number.
  @param[in]  Function    PCI function number.
  @param[in]  ErrSeverity IIO Internal Error Severity.
  @param[in]  ErrType     IIO Internal Error Type.
**/
VOID
EFIAPI
IioIntDevErrHandler (
  IN    UINT8                        Socket,
  IN    UINT8                        IioStack,
  IN    UINT8                        Bus,
  IN    UINT8                        Device,
  IN    UINT8                        Function,
  IN    UINT8                        ErrSeverity,
  IN    RAS_IIO_INTERNAL_ERROR_TYPE  ErrType
  )
{
  BOOLEAN            ErrorFound = FALSE;

  switch (ErrType) {
    case RasIioInternalErrorVtd:
      if (IsIioVtdError (Socket, Bus, Device, Function, ErrSeverity)) {
        ErrorFound = TRUE;
      }
      break;
    case RasIioInternalErrorCBDma:
      if (IsIioCbdmaError (Socket, Bus, Device, Function, ErrSeverity)) {
        ErrorFound = TRUE;
      }
      break;
    case RasIioInternalErrorM2PCIE:
      if (IsIioM2pcieError (Socket, Bus, Device, Function, ErrSeverity)) {
        ErrorFound = TRUE;
      }
      break;
    case RasIioInternalErrorIrp:
      if (IsIioIrpError (Socket, Bus, Device, Function, ErrSeverity)) {
        ErrorFound = TRUE;
      }
      break;
    case RasIioInternalErrorRing:
      if (IsIioIrpRingError (Socket, Bus, Device, Function, ErrSeverity)) {
        ErrorFound = TRUE;
      }
      break;
    case RasIioInternalErrorItc:
      if (IsIioItcError (Socket, Bus, Device, Function, ErrSeverity)) {
        ErrorFound = TRUE;
      }
      break;
    case RasIioInternalErrorOtc:
      if (IsIioOtcError (Socket, Bus, Device, Function, ErrSeverity)) {
        ErrorFound = TRUE;
      }
      break;
    default:
      ErrorFound = FALSE;
      break;
  }

  if (ErrorFound) {
    ReportIioInternalErrorLog (Socket, IioStack, ErrType, ErrSeverity);
  }

}
