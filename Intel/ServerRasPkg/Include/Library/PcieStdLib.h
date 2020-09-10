/** @file
  Interface of PCIE Aer library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#ifndef __PCIE_STD_LIB_H__
#define __PCIE_STD_LIB_H__

#include <Library/MmPciExtLib.h>

/**
  This function checks if a standard PCI or PCIe device exists at the specified PCI address.

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
PcieIsStdDevicePresent (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function checks if a specified device is a PCI Express device.

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        TRUE   - PCIe device present at the specified address.
                            FALSE  - PCIe device not present at the specified address.
--*/
BOOLEAN
EFIAPI
IsPcieDevice (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function checks if a specified device implemented mutiple functions.

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        TRUE   - It is Multi-Function Device
                            FALSE  - It is single function Device
--*/
BOOLEAN
EFIAPI
IsMultiFunctionDevice (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function checks if a PCI-to-PCI bridge exists at the specified PCI address.

  @param[in] Socket      Device's Socket number.
  @param[in] Bus         Device's bus number.
  @param[in] Device      Device's device number.
  @param[in] Function    Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval  BOOLEAN       TRUE  - P2P present at the specified address.
                         FALSE - P2P not present at the specified address.
**/
BOOLEAN
EFIAPI
PcieIsPciBridgeDevice (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function will verify whether a port is PCIE root port

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        True   if PCIe is root port.
                            False  if PCIe is not root port.
**/
BOOLEAN
EFIAPI
IsPcieRootPort (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function will verify whether a port is PCIE downstream port

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        True   if PCIe is downstream port.
                            False  if PCIe is not downstream port.
**/
BOOLEAN
EFIAPI
IsPcieDownstreamPort (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function will verify whether an event is surprise hot remove or not

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        True   if PCIe suprise link down was set
                            False  if PCIe suprise link down was not set
**/
BOOLEAN
EFIAPI
IsPcieSurpriseHotRemoveEvent (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Check if PCIE device support RCEC or not.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device

  @retval BOOLEAN  - TRUE: RCEC is supported; FALSE:RCEC is not supported.

**/
BOOLEAN
EFIAPI
IsPcieRcecSupported (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Get RCEC Next Bus, Last Bus Number and Associated Bit Map

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @param NextBus   - Next Bus Number of associated RCiEPs
  @param LastBus   - Last Bus Number of associated RCiEPs
  @param Abm       - Associate Bit Map of the device on the same Bus Number of RCEC
  @retval none
**/
VOID
EFIAPI
GetRcecNbnLbnAbm (
  IN  UINT8            Socket,
  IN  UINT8            Bus,
  IN  UINT8            Device,
  IN  UINT8            Function,
  IN  PCI_EXT_MM_INFO  *MmInfo,
  OUT UINT8            *NextBus,
  OUT UINT8            *LastBus,
  OUT UINT32           *Abm
  );

/**
  Check if PCIE device support eDPC or not.

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.
  @retval BOOLEAN  - TRUE: eDPC is supported; FALSE:eDPC is not supported.

**/
BOOLEAN
EFIAPI
IsPcieEdpcSupported (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function is to program eDPC setting according to eDPC Policy
  error.

  @param[in] Socet          Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.
  @param[in] CorrErrEn      Pcie Correctable enable.
  @param[in] UncorrErrEn    Pcie Uncorrectabe enable.
  @param[in] FatalErrEn     Pcie Fata error enable.

  @retval    None
**/
VOID
EFIAPI
PcieEdpcConfig (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT8            EdpcTrigger,
  IN BOOLEAN          EdpcInterrupt,
  IN BOOLEAN          EdpcCorMsg
  );

/**
  This function checks the PCIe Root Port/Downstream Port DPC status register, DPC control register and Device control register
  to see if DPC was triggered

  @param[in] Socket         Device's Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    BOOLEAN        TRUE  - A PCIe Correctable error was detected.
                            FALSE - A PCIe Correctable error was not detected.
--*/
BOOLEAN
EFIAPI
PcieIsEdpcTiggered (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function get the Secondary Bus Number, Subordinate Bus Number of
  the PCIE bridge if device exists at the specified PCI address.
  @param[in] Socket               Device's Socket number.
  @param[in] Bus                  Device's bus number.
  @param[in] Device               Device's device number.
  @param[in] Function             Device's function number.
  @param[in] MmInfo               Memory Map information of target device.

  @param[in out] SecondaryBus     Device's Secondary Bus.
  @param[in out] SubordinateBus   Device's SubordinateBus.

  @retval    BOOLEAN        True   Device present at the specified address.
                            False  Device not present at the specified address.
**/
BOOLEAN
EFIAPI
PcieGetSecondarySubordinateBus (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT8            *SecondaryBus,
  IN UINT8            *SubordinateBus
  );

/**
  This function gets the capability structure.

  @param[in] CapBuf         a pointer to the capability array in the PCIE error record structure.
  @param[in] Bus            PCIe device's bus number.
  @param[in] Device         PCIe device's device number.
  @param[in] Function       PCIe device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    Status
--*/
RETURN_STATUS
EFIAPI
PcieGetCapabilities (
  IN OUT UINT8       *CapBuf,
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
   This function obtains the PCI configuration space register offset of the specified
   Capabilities register set.

  @param[in] Bus         Device's bus number.
  @param[in] Device      Device's device number.
  @param[in] Function    Device's function number.
  @param[in] MmInfo      Memory Map information of target device.
  @param[in] Cap         Desired Capability structure

  @retval  CapabilityOffset.
**/
UINT8
EFIAPI
PcieGetCapabilitiesOffset (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT8            Cap
  );

/**
  This function obtains the extended PCI configuration space register offset for a
  specified Extended Capability for the specified PCI device.

  @param[in] Bus                  Device's bus number.
  @param[in] Device               Device's device number.
  @param[in] Function             Device's function number.
  @param[in] MmInfo               Memory Map information of target device.
  @param[in] ExtCapabilityId      ID of the desired Extended Capability.
  @param[in] VendoreSpecificId    vendor ID that is at offset ExtCapabilityoffset + 4.

  @retval    Value                0         PCIe extended capability ID not found
                                  non-zero  Extended PCI configuration space offset of the specified Ext Capability block
**/
UINT16
EFIAPI
PcieGetExtendedCapabilitiesOffset (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT16           ExtCapabilityId,
  IN UINT16           VendorSpecificId
  );

/**
  This function to disable surprise hot plug feature in slot capibilties register
  this is need slot capbility register was unlocked

  @param[in] Socket               Device's Socket number.
  @param[in] Bus                  Device's bus number.
  @param[in] Device               Device's device number.
  @param[in] Function             Device's function number.
  @param[in] MmInfo               Memory Map information of target device.

**/
VOID
EFIAPI
PcieDisableSurpriseHotPlugCap (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  This function gets device status and device control register value.

  @param[in] Socket         PCIe device's socket number.
  @param[in] Bus            PCIe device's bus number.
  @param[in] Device         PCIe device's device number.
  @param[in] Function       PCIe device's function number.
  @param[in] MmInfo         Memory Map information of target device.
  @param[in] *DevCtl        To store Device Control register's value
  @param[in] *DevSts        To store Device Status register's value
  @retval    TRUE           The device status, device control register value was gotten
             FALSE          Something wrong
--*/

BOOLEAN
EFIAPI
PcieGetDevCtlAndDevSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  OUT UINT16          *DevCtl,
  OUT UINT16          *DevSts
  );

/**
  This function gets the port type.

  @param[in] Bus            PCIe device's bus number.
  @param[in] Device         PCIe device's device number.
  @param[in] Function       PCIe device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    Value          Port Type  - if applicable
                            0xff       - if no port type
--*/
UINT32
EFIAPI
PcieGetPortType (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Check if PCIE device support AER or not.

  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device.

  @retval BOOLEAN  - TRUE: AER is supported; FALSE:AER is not supported.

**/
BOOLEAN
EFIAPI
IsPcieAerSupported (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Get PCIE device's AER cap structer.

  @param Socket          - Socket
  @param Bus             - Bus
  @param Device          - Device
  @param Function        - Function
  @param MmInfo          - Memory Map information of target device
  @param AerErrDataCap   - Point to get Pcie error data capability.
**/
EFI_STATUS
EFIAPI
PcieAerGetErrCapData (
  IN  UINT8            Socket,
  IN  UINT8            Bus,
  IN  UINT8            Device,
  IN  UINT8            Function,
  IN  PCI_EXT_MM_INFO  *MmInfo,
  OUT VOID             *AerErrDataCap
  );

/**
  Get PCIE device's AER uncorrectable error status.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @retval UINT32   - Pcie AER uncorrectable error status.
**/
UINT32
EFIAPI
PcieAerGetUncErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Get PCIE device's AER uncorrectable error masks.

  @param Socket    - Socket ID
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @retval UINT32   - Pcie AER uncorrectable error masks.
**/
UINT32
EFIAPI
PcieAerGetUncErrMsk (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Get PCIE device's AER error severity.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @param ErrSev    - Error severity

  return    FALSE - No severity return,  TRUE - Severity return
**/
BOOLEAN
EFIAPI
PcieAerGetErrSev (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           *ErrSev
  );

/**
  Get PCIE device's AER correctable error status.

  @param Socket        - Socket
  @param Bus           - Bus
  @param Device        - Device
  @param Function      - Function
  @param MmInfo        - Memory Map information of target device
  @retval EFI_STATUS   - Pcie AER correctable error status.
**/
UINT32
EFIAPI
PcieAerGetCorrErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Get PCIE device's AER correctable error masks.

  @param Socket        - Socket
  @param Bus           - Bus
  @param Device        - Device
  @param Function      - Function
  @param MmInfo        - Memory Map information of target device
  @retval EFI_STATUS   - Pcie AER correctable error status.
**/
UINT32
EFIAPI
PcieAerGetCorrErrMsk (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Get PCIE device's AER correctable error status.

  @param Socket        - Socket
  @param Bus           - Bus
  @param Device        - Device
  @param Function      - Function
  @param MmInfo        - Memory Map information of target device
  @param CorrErrMask   - CorrErrMask
  @param UnCorrErrMask - UnCorrErrMask
  @param ErrorSev      - ErrorSev
  @param AeccConf      - AeccConf
  @retval VOID
**/
VOID
PcieAerConfig (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           CorrErrMask,
  IN UINT32           UnCorrErrMask,
  IN UINT32           ErrorSev,
  IN UINT32           AeccConf
  );

/**
  Get PCIE device's AER root port error status.

  @param Socket      - Socket
  @param Bus         - Bus
  @param Device      - Device
  @param Function    - Function
  @param MmInfo      - Memory Map information of target device
  @param RpErrSts    - Root Error Status
  @retval EFI_STATUS - EFI_SUCCESS Root Error Status returned, EFI_UNSUPPORTED AER or Root Status Not supported.
**/
EFI_STATUS
EFIAPI
PcieAerGetRpErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN OUT  UINT32      *RpErrSts
  );

/**
  Clear PCIE device's AER correctable error status.
  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
**/
VOID
EFIAPI
PcieAerClearCorrErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Clear PCIE device's AER uncorrectable error status.
  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
**/
VOID
EFIAPI
PcieAerClearUncErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );

/**
  Clear PCIE device's Root Error Status.
  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @param Severity  - Severity
**/
VOID
EFIAPI
PcieAerClearRootErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           Severity
  );

/**
  Clear PCIE device's Root Error Status without checking Port Type.
  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @param Severity  - Severity
**/
VOID
EFIAPI
PcieAerClearRootErrStsAtom (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           Severity
  );

/**
  Program PCIE device's AER Correctable error mask.

  @param Bus         - Bus
  @param Device      - Device
  @param Function    - Function
  @param MmInfo      - Memory Map information of target device
  @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
PcieAerProgramCorrErrMsk (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           CorrErrMsk
  );


/**
  Program PCIE device's AER Uncorrectable error mask.

  @param Bus           - Bus
  @param Device        - Device
  @param Function      - Function
  @param MmInfo        - Memory Map information of target device
  @param UnCorrErrMsk  - Uncorrectable error mask
**/
VOID
EFIAPI
PcieAerProgramUncErrMsk (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           UncErrMsk
  );

/**
  Program PCIE device's AER error severity.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @param ErrSev    - Error severity
**/
VOID
EFIAPI
PcieAerProgramErrSev (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           ErrSev
  );

/**
  Get PCIE device's AER error source identification register.
  Only root port or error collector implements ERRSID register.
  Caller should pay attention about it. DMI down stream also implements it.

  @param Socket        - Socket
  @param Bus           - Bus
  @param Device        - Device
  @param Function      - Function
  @param MmInfo        - Memory Map information of target device
  @retval EFI_STATUS   - Pcie AER error source identification register.
**/
UINT32
EFIAPI
PcieAerGetErrSid (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  );
#endif
