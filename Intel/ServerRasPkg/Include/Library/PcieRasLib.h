/** @file
  Interface of PcieRasLib Interface.

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

#ifndef __PCIE_RAS_H__
#define __PCIE_RAS_H__

#include <Library/MmPciExtLib.h>

/*
  Specific device may have compliance conflict with PCIE Global RAS policy , need a way to handle
  such kind of conflict, that is what PCIE ERROR FILTER to do
*/

typedef struct {
  UINT16   Vid;                // PCIE Device Vendor ID
  UINT16   DidMin;             // Minimum PCIE Device ID
  UINT16   DidMax;             // Maximum PCIE Device ID
  UINT32   AerCorrMskOr;       // AER Correctable Error Msk OR Value
  UINT32   AerCorrMskAnd;      // AER Correctable Error Msk And Value
  UINT32   AerUnCorrMskOr;     // AER UnCorrectable Error Msk OR Value
  UINT32   AerUnCorrMskAnd;    // AER UnCorrectable Error Msk And Value
  UINT32   AerErrSevOr;        // AER UnCorrectable Error Severity OR Value
  UINT32   AerErrSevAnd;       // AER UnCorrectable Error Severity And Value
  UINT16   DevCtlOr;           // Device Control OR Value
  UINT16   DevCtlAnd;          // Device Control And Value
  UINT32   AeccConfOr;         // Advanced Error Capabilities and Control OR Value
  UINT32   AeccConfAnd;        // Advanced Error Capabilities and Control And Value
} PCIE_ERROR_FILTER;

/**
  This function checks if a PCI or PCIe device exists at the specified PCI address.

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        True   Device present at the specified address.
                            False  Device not present at the specified address.
**/
BOOLEAN
EFIAPI
PcieIsDevicePresent (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Dump the Errors of a PCIE device
  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval none.

**/
VOID
DumpPcieRegisters (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function is to dump the errors registers of a PCIE device and if it is a
  P2P bridge, also dump the devices error on secondary bus or on a subordinate bus.

  @param[in] Socket         P2P bridge's Socket number.
  @param[in] Bus            P2P bridge's bus number.
  @param[in] Device         P2P bridge's device number.
  @param[in] Function       P2P bridge's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval   None
--*/
VOID
EFIAPI
DumpPcieBridgeRegisters (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function clears the specified device's primary and secondary (if appropriate)
  legacy PCI status registers.

  @param[in] Bus         Device's bus number.
  @param[in] Device      Device's device number.
  @param[in] Function    Device's function number.
  @param[in] MmInfo      Memory Map information of target device.

  @retval  None.
**/
VOID
EFIAPI
PciClearLegacyDeviceStatus (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function clears the legacy and standard PCI Express status registers for the
  specified PCI Express device.

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    None
**/
VOID
EFIAPI
PcieClearDeviceStatus (
  IN UINT8           Socket,
  IN UINT8           Bus,
  IN UINT8           Device,
  IN UINT8           Function,
  IN PCI_EXT_MM_INFO *MmInfo,
  IN UINT32          ErrSeverity
  );

/**
  This function will verify whether a port is PCIe Dmi port

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        True   if PCIe is root port.
                            False  if PCIe is not root port.
**/
BOOLEAN
EFIAPI
PcieIsDmiPort (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function will verify whether a port is Rlink DMI

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        True   if PCIe is root port.
                            False  if PCIe is not root port.
**/
BOOLEAN
EFIAPI
PcieIsRlink (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function will verify whether a port is IIO root port


  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        True   if PCIe is root port.
                            False  if PCIe is not root port.
**/
BOOLEAN
EFIAPI
IsIioRootPort (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Get PCIE device's AER correctable error masks or IIO Root Port Correctable Error Counter Mask.

  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device

  @retval          - Pcie AER correctable error status/IIO Root Port CorrErrCntMsk.
**/
UINT32
PcieGetCorrErrMsk (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
);

/**
  This function checks to see if PCIe error reporting is enabled in the specified
  PCIe root port device.

  @param[in] Bus            PCIe root port device's bus number.
  @param[in] Device         PCIe root port device's device number.
  @param[in] Function       PCIe root port device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        TRUE   - Error reporting is enabled.
                            FALSE  - Error reporting is not enabled or it is under OS control.
--*/
BOOLEAN
EFIAPI
IsPcieRootPortErrReportingEnabled (
  IN UINT8           Socket,
  IN UINT8           Bus,
  IN UINT8           Device,
  IN UINT8           Function,
  IN PCI_EXT_MM_INFO *MmInfo
  );

/**
  This function gets the slot number.

  @param[in] Bus            PCIe device's bus number.
  @param[in] Device         PCIe device's device number.
  @param[in] Function       PCIe device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    Value          PcieSlotNum  - if applicable
                            0            - for Ports connected to devices that are either
                                           integrated on the system board or integrated within
                                           the same silicon as the Switch device or Root Port.
--*/
UINT16
EFIAPI
PcieGetSlotNum (
  IN UINT8           Socket,
  IN UINT8           Bus,
  IN UINT8           Device,
  IN UINT8           Function,
  IN PCI_EXT_MM_INFO *MmInfo
  );

/**
  This function gets suppress UR escalation and clear UR status if specified.

  @param[in] Socket         PCIe device's socket number.
  @param[in] Bus            PCIe device's bus number.
  @param[in] Device         PCIe device's device number.
  @param[in] Function       PCIe device's function number.
  @param[in] MmInfo         Memory Map information of target device.
  @param[in] OriginDevSts   Original Device status before UR happens due to PCI probing
  @param[in] SevPerStack    Error Severity that we are processing

  @retval    Status
--*/

EFI_STATUS
EFIAPI
PcieSuppressUnsupportReqest (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT16           OriginDevSts,
  IN UINT8            SevPerStack
  );

/**
  This function checks for, and handles, errors on standard PCI devices.

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        TRUE     - An error was detected.
                            FALSE    - An error was not detected.
--*/
BOOLEAN
EFIAPI
PciDeviceErrorHandler (
  IN UINT8           Socket,
  IN UINT8           Bus,
  IN UINT8           Device,
  IN UINT8           Function,
  IN PCI_EXT_MM_INFO *MmInfo
  );

/**
  This function is to handle multiple errors happen on RCEC or the RCEC owned RCiEP.

  @param[in] RcecBus            PCIe RCEC device's bus number.
  @param[in] RcecDevice         PCIe RCEC device's device number.
  @param[in] RcecFunction       PCIe RCEC device's function number.
  @param[in] MmInfo             Memory Map information of target device.

  @retval    BOOLEAN        TRUE   - An error was detected.
                            FALSE  - An error was not detected.
--*/
BOOLEAN
EFIAPI
PcieRcecMultiErrorsHandler (
  IN UINT8            Socket,
  IN UINT8            RcecBus,
  IN UINT8            RcecDevice,
  IN UINT8            RcecFunction,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT8            SevPerStack
  );

/**
  PCI Express error handler for ESB2 and BNB PCI Express Root Ports.

  @param[in] RootPortBus            PCIe root port device's bus number.
  @param[in] RootPortDevice         PCIe root port device's device number.
  @param[in] RootPortFunction       PCIe root port device's function number.
  @param[in] MmInfo                 Memory Map information of target device.

  @retval    BOOLEAN        TRUE   - An error was detected.
                            FALSE  - An error was not detected.
--*/
BOOLEAN
EFIAPI
PcieRootPortErrorHandler (
  IN UINT8            Socket,
  IN UINT8            RootPortBus,
  IN UINT8            RootPortDevice,
  IN UINT8            RootPortFunction,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT8            SevPerStack
  );


/**
  This function initializes and enables error reporting in the specified PCI or PCIe device.

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    None
--*/
VOID
EFIAPI
PcieConfigDevice (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );


/**
  This function initializes and enables error reporting in the specified PCI or PCIe
  device and all subordinate PCI or PCIe devices.

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    None
--*/
VOID
EFIAPI
PcieConfigDeviceAndSubDevices (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function initializes and enables error reporting in the RCEC
  device and all RCiEP devices associated with it.

  @param[in] Socket         Device's Socket Number
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    None
--*/
VOID
EFIAPI
PcieConfigRcecAndAssociateRcieps (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function get secondary bus for the bridge

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    Secondary bus
**/
UINT8
EFIAPI
PcieGetSecondaryBus (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function checks if a PCI device is present on the specified bus.

  @param[in] Bus        Device's bus number
  @param[in] MmInfo     Memory Map information of target device.

  @retval TRUE        A PCI device was found on the bus

**/
BOOLEAN
EFIAPI
PciDeviceOnBus (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function is to check if the  PCIe Correctable errors over the limit.

  @param[in] Socket         Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @retval    TRUE -The error over limitation, corretable error should be disabled for this device.
             FALSE -The error count is under control, will report the error only, not to disable it.
--*/
BOOLEAN
IsPcieCorrectedErrorOverLimit (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  );

/**
  This function determines if the DMI device is reporting an error and handles the
  error if one is detected.

  @param[in] Socket         PCIe device's Socket number.
  @param[in] Bus            PCIe device's bus number.
  @param[in] Device         PCIe device's device number.
  @param[in] Function       PCIe device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        TRUE   - An error was detected.
                            FALSE  - An error was not detected.
--*/
BOOLEAN
EFIAPI
PcieDmiDeviceErrorHandler (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT8            SevPerStack
  );

/**
  This function determines if the specified device is reporting an error and handles the
  error if one is detected.

  @param[in] Bus            PCIe device's bus number.
  @param[in] Device         PCIe device's device number.
  @param[in] Function       PCIe device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        TRUE   - An error was detected.
                            FALSE  - An error was not detected.
--*/
BOOLEAN
EFIAPI
PcieDeviceErrorHandler (
  IN UINT8           Socket,
  IN UINT8           Bus,
  IN UINT8           Device,
  IN UINT8           Function,
  IN PCI_EXT_MM_INFO *MmInfo,
  IN UINT8           SevPerStack
  );

/**
  Returns the  sev by error type.

  @param[in] ErrorType   The ErrorType

  @retval The sev of the error type.

**/
UINT32
GetSevByErrType (
  IN      UINT32    ErrorType
  );

/**
  This function determines if Ler is supported or not.

  @param[in] Socket         Socket number.
  @param[in] Bus            PCIe device's bus number.
  @param[in] Device         PCIe device's device number.
  @param[in] Function       PCIe device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        TRUE   - Ler was supported.
                            FALSE  - Ler was not supported.
--*/
BOOLEAN
EFIAPI
IsLerSupported (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );
#endif
