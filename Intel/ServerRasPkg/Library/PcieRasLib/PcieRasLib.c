/** @file
  Implementation of Pcie Lib.

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

#include <Uefi.h>
#include <Library/ErrorReportLib.h>
#include <Library/PcieStdLib.h>
#include <Library/PciLib.h>
#include <Library/RasDebugLib.h>
#include <IndustryStandard/Pci.h>
#include <Library/PcieRasLib.h>
#include <Register/IioRegDef.h>
#include <Register/PcieRegDef.h>
#include <Library/RasMailBoxLib.h>
#include <Library/IioSiliconLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/MmPciExtLib.h>
#include "PcieRasLibInternal.h"
#include <PCIeErrorTypes.h>
#include <Library/SiliconWorkaroundLib.h>
#include <RasAcpi.h>
#include <Library/WheaSciLib.h>
#include <Library/RasAcpiLib.h>
#include <Library/IioTopologyLib.h>
#include <Library/RasGlobalDataLib.h>
#include <IndustryStandard/Acpi62.h>
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
  )
{
  //
  // Check if a standard PCIE device is present at this location...
  //
  if (PcieIsStdDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
    return TRUE;
  } else {
    //
    // Check if a incompatible PCIE device is present at this location...
    //
    if (IsSiSpecPcieDeviceExist (Socket, Bus, Device, Function, MmInfo)) {
      return TRUE;
    }
  }
  return FALSE;
}

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
  )
{
  UINT32      Data32;
  UINT16      AerCap;
  UINT8       CapabilitiesOffset;
  BOOLEAN     RootPortOrRcec;
  UINT16      Data16;

  if (!PcieIsDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
    return;
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "DumpPcieRegisters Socket %x, Bus %x, Dev %x, Func%x\n", Socket, Bus, Device, Function));

  AerCap = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);

  if (AerCap) {
    Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCap + PCIE_AER_OFFSET_REG_UES, MmInfo));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCIE_AER_UNCERRSTS:  %x\n", Data32));
    Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCap + PCIE_AER_OFFSET_REG_CES, MmInfo));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCIE_AER_CORERRSTS:  %x\n", Data32));
    Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCap + PCIE_AER_OFFSET_REG_UEM, MmInfo));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCIE_AER_UNCERRMSK:  %x\n", Data32));
    Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCap + PCIE_AER_OFFSET_REG_CEM, MmInfo));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCIE_AER_CORERRMSK:  %x\n", Data32));
    Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCap + PCIE_AER_OFFSET_REG_UESEV, MmInfo));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCIE_AER_UNCERRSEV:  %x\n", Data32));
  }

  if (PcieIsDmiPort (Socket, Bus, Device, Function, MmInfo)) {
    Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, DMI_DEV_STS_OFFSET, MmInfo));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "DMI_DEV_STS:  %x\n", Data16));
    Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, DMI_DEV_CTL_OFFSET, MmInfo));
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "DMI_DEV_CTL:  %x\n", Data16));
  }
  //
  // Check if it is Root Port or RCEC device
  //
  RootPortOrRcec = IsPcieRootPort(Socket, Bus, Device, Function, MmInfo) || IsPcieRcecSupported(Socket, Bus, Device, Function, MmInfo);

  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (CapabilitiesOffset != 0) {
      Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET, MmInfo));
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCIE_DEVICE_STATUS:  %x\n", Data16));
      Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET, MmInfo));
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCIE_DEVICE_CONTROL:  %x\n", Data16));

    if (RootPortOrRcec) {
      Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function,CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET, MmInfo));
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCIE_ROOT_CONTROL:  %x\n", Data16));
      Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCap + PCIE_AER_OFFSET_REG_RES, MmInfo));
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCIE_AER_ROOT_ERROR_STATUS:  %x\n", Data32));
      Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCap + PCIE_AER_OFFSET_REG_REC, MmInfo));
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCIE_AER_ROOT_ERROR_CONTROL:  %x\n", Data32));
    }
  }

  Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET, MmInfo));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCI_PRIMARY_STATUS:  %x\n", Data16));
  Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_BRIDGE_STATUS_REGISTER_OFFSET, MmInfo));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCI_BRIDGE_STATUS_REG:  %x\n", Data16));
  Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_COMMAND_OFFSET, MmInfo));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PCI_COMMAND:  %x\n", Data16));
}

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
  )
{
  UINT8     SecondaryBusNumber;
  UINT8     SubordinateBusNumber;
  UINT8     FuncExist;
  UINT16    BusNumber;
  UINT8     DeviceNumber;
  UINT8     FunctionNumber;

  DumpPcieRegisters (Socket, Bus, Device, Function, MmInfo);

  if (PcieIsPciBridgeDevice (Socket, Bus, Device, Function, MmInfo)) {
    SecondaryBusNumber = MmioRead8 (MmPciAddressExt (Socket, Bus,
                                      Device,
                                      Function,
                                      PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET,
                                      MmInfo
                                      ));

    SubordinateBusNumber = MmioRead8 (MmPciAddressExt (Socket, Bus,
                                        Device,
                                        Function,
                                        PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET,
                                        MmInfo
                                        ));
    if (SecondaryBusNumber && SubordinateBusNumber) {
      for (BusNumber = SecondaryBusNumber; BusNumber <= SubordinateBusNumber; BusNumber++) {
        for (DeviceNumber = 0; DeviceNumber <= PCI_MAX_DEVICE; DeviceNumber++) {
          FuncExist = 0;
          for (FunctionNumber = 0; FunctionNumber <= PCI_MAX_FUNC; FunctionNumber++) {
            if (PcieIsDevicePresent (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo)) {
              DumpPcieRegisters (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo);
              FuncExist |= (BIT0 << FunctionNumber);
              //
              // If it is non nulti-function device, break function loop
              //
              if (FunctionNumber == 0 && !IsMultiFunctionDevice (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo)) {
                break;
              }
            } else {
              if (FunctionNumber == 0) {
                break;
              } else {
                continue;
              }
            }
          }
          //
          // During PCI probing, UR error will be logged in exist functions, need to clear
          // them before going forward.
          //
          for (FunctionNumber = 0; FunctionNumber <= PCI_MAX_FUNC; FunctionNumber++) {
            if (FuncExist & (BIT0 << FunctionNumber)) {
              if (PcieIsDevicePresent (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo)) {
                PcieSuppressUnsupportReqest(Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo, 0, COR_ERROR_TYPE);
              }
            }
          }
        }
      } // BUS
      //
      // During PCI probing, UR error will be logged in exist functions, need to clear
      // them before going forward.
      //
      PcieSuppressUnsupportReqest(Socket, Bus, Device, Function, MmInfo, 0, COR_ERROR_TYPE);
    }
  }
}

/**
  Returns the  sev by error type.

  @param[in] ErrorType   The ErrorType

  @retval The sev of the error type.

**/
UINT32
GetSevByErrType (
  IN      UINT32    ErrorType
  )
{
  UINT32    Sev = EFI_ACPI_6_2_ERROR_SEVERITY_NONE;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "GetSevByErrType ErrorType = 0x%x\n",ErrorType));

  switch (ErrorType) {
    case SERROR_TYPE:
      Sev = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
      break;

    case PERROR_TYPE:
      Sev = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
      break;

    case COR_ERROR_TYPE:
      Sev = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED;
      break;

    case NONFATAL_ERROR_TYPE:
      Sev = EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE;
      break;

    case FATAL_ERROR_TYPE:
      Sev = EFI_ACPI_6_2_ERROR_SEVERITY_FATAL;
      break;
    default:
      break;
  }
  return Sev;
}

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
  )
{
  BOOLEAN   DmiPort = FALSE;
  UINT32    Data32;
  UINT32    ClassCode;

  Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_VENDOR_ID_OFFSET, MmInfo));
  if ((Data32 & 0xffff) != INTEL_PCI_VENDOR_ID) {
    return DmiPort;
  }

  //
  //DMI and IIO root port have ERRINJ device ID to judge IIO root port.
  //
  if (PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID, PCIE_EXT_CAP_HEADER_EINJ_VSECID) == 0) {
    return DmiPort;
  }

  ClassCode = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_REVISION_ID_OFFSET, MmInfo));

  if ((ClassCode >> 8) == V_PCIE_CCR_PCI_HOST_BRIDGE) {
    RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieIsDmiPort - It is Dmi Port!\n"));
    DmiPort = TRUE;
  }
  return DmiPort;
}

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
  )
{
  UINT32          Data32;
  UINT32          ClassCode;

  Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_VENDOR_ID_OFFSET, MmInfo));
  if (Data32 == 0xffffffff) {
    //
    // For R-Link DP in DMI mode, the DID/VID is set to 0xffff. Need further check if
    // it is R-Link DP
    //
    ClassCode = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_REVISION_ID_OFFSET, MmInfo));
    if ((ClassCode >> 8) == V_PCIE_CCR_PCI_HOST_BRIDGE) {
      //
      // Rlink also have ERRINJ capability
      //
      if (PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID, PCIE_EXT_CAP_HEADER_EINJ_VSECID) == 0) {
        return FALSE;
      } else {
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieIsRlink - It is Rlink!\n"));
        return TRUE;
      }
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}

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
  )
{
  UINT32  Data32;

  Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_VENDOR_ID_OFFSET, MmInfo));
  if ((Data32 & 0xffff) != INTEL_PCI_VENDOR_ID) {
    return FALSE;
  }

  //
  //DMI and IIO root port have ERRINJ device ID to judge IIO root port.
  //
  if (PcieGetExtendedCapabilitiesOffset(Socket, Bus, Device, Function, MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID, PCIE_EXT_CAP_HEADER_EINJ_VSECID) == 0){
    return FALSE;
  }

  Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_REVISION_ID_OFFSET, MmInfo));

  if ((Data32 >> 8)== V_PCIE_CCR_PCI_TO_PCI_BRIDGE ) {
    return TRUE;
  }
  return FALSE;
}

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
)
{
  RAS_MAIL_BOX              *MailBox = NULL;

  MailBox = GetRasMailBox ();

  if (IsIioRootPort (Socket, Bus, Device, Function, MmInfo) && (MailBox != NULL)) {
     if (MailBox->PcieInitPar.PcieCorErrCntr == 1) {
       return IioGetCorrCntErrMsk (Socket, Bus, Device, Function, MmInfo);
     } else {
       return PcieAerGetCorrErrMsk(Socket, Bus, Device, Function, MmInfo);
     }
  } else {
    return PcieAerGetCorrErrMsk(Socket, Bus, Device, Function, MmInfo);
  }

}
/**
  This function checks the PCIe device status and the correctable error registers to see
  if a PCIe Correctable error has occurred.

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device

  @retval    BOOLEAN        TRUE  - A PCIe Correctable error was detected.
                            FALSE - A PCIe Correctable error was not detected.
--*/
BOOLEAN
EFIAPI
PcieIsCorrectableDeviceError (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  UINT8           CapabilitiesOffset;
  UINT16          DeviceStatus;
  UINT16          DeviceControl;
  BOOLEAN         ErrorFound;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieIsCorrectableDeviceError  Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n", Socket, Bus, Device, Function));

  ErrorFound = FALSE;
  if (!IsPcieDevice(Socket, Bus, Device, Function, MmInfo)) {
    return FALSE;
  }

  // Get the capabilities offset
  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }
  // Check if the device status indicates that a CORRECTABLE error occurred
  DeviceStatus = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET, MmInfo));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieIsCorrectableDeviceError PCIE_DEVICE_STATUS = 0x%x\n",DeviceStatus));
  DeviceControl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET, MmInfo));
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieIsCorrectableDeviceError PCIE_DEVICE_CONTROL = 0x%x\n",DeviceControl));
  if (DeviceStatus & DeviceControl & B_PCIE_DEVICE_CONTROL_COR_ERR_EN) {
    ErrorFound = TRUE;
  }

  return ErrorFound;
}

/**
  This function checks the SERR and PCIe device status to see if a PCIe or PCI Uncorrectable Fatal
  error has occurred.

  @param[in] Bus            PCIe device's bus number.
  @param[in] Device         PCIe device's device number.
  @param[in] Function       PCIe device's function number.
  @param[in] MmInfo         Memory Map information of target device

  @retval    BOOLEAN        TRUE   - A PCIe Uncorrectable Fatal error was detected.
                            FALSE  - A PCIe Uncorrectable Fatal error was not detected.
--*/
BOOLEAN
EFIAPI
IsFatalNonFatalDeviceError (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  UINT8     CapabilitiesOffset;
  UINT16    DeviceStatus;
  UINT16    DeviceControl;
  UINT16    PrimaryStatus;
  UINT16    PciCmd;
  BOOLEAN   ErrorFound;

  ErrorFound = FALSE;
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "IsFatalNonFatalDeviceError  Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n", Socket, Bus, Device, Function));

  if (IsPcieDevice(Socket, Bus, Device, Function, MmInfo)) {
    CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
    if (CapabilitiesOffset == 0) {
      return FALSE;
    }

    DeviceStatus = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET, MmInfo));
    DeviceControl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET, MmInfo));
    RAS_DEBUG((LEVEL_FUNC_FLOW, "IsFatalNonFatalDeviceError PCIE_DEVICE_STATUS = 0x%x\n",DeviceStatus));
    RAS_DEBUG((LEVEL_FUNC_FLOW, "IsFatalNonFatalDeviceError PCIE_DEVICE_CONTROL = 0x%x\n",DeviceControl));

    if ((DeviceStatus & B_PCIE_DEVICE_STATUS_FAT_ERR) && (DeviceControl & B_PCIE_DEVICE_CONTROL_FAT_ERR_EN)) {
      ErrorFound = TRUE;
    }
    if ((DeviceStatus & B_PCIE_DEVICE_STATUS_NF_ERR) && (DeviceControl & B_PCIE_DEVICE_CONTROL_NF_ERR_EN)) {
      ErrorFound = TRUE;
    }

  } else {
    PrimaryStatus = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET, MmInfo));
    PciCmd = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_COMMAND_OFFSET, MmInfo));

    if ((PrimaryStatus & EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR) && (PciCmd & EFI_PCI_COMMAND_SERR)) {
      ErrorFound = TRUE;
    }
  }

  return ErrorFound;
}

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
  )
{
  UINT16    StatusReg16;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciClearLegacyDeviceStatus Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Bus,Device,Function));

  StatusReg16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET, MmInfo));
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetCapabilitiesOffset PCI_PRIMARY_STATUS_OFFSET = 0x%x\n",StatusReg16));
  MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET, MmInfo), StatusReg16);

  if (PcieIsPciBridgeDevice (Socket,Bus, Device, Function, MmInfo)) {
    StatusReg16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_BRIDGE_STATUS_REGISTER_OFFSET, MmInfo));
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetCapabilitiesOffset PCI_BRIDGE_STATUS_REGISTER_OFFSET = 0x%x\n",StatusReg16));
    MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_BRIDGE_STATUS_REGISTER_OFFSET, MmInfo), StatusReg16);
  }
}

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
  )
{
  UINT8     CapabilitiesOffset;
  UINT16    StatusReg16;
  UINT16    RegTmp16;
  BOOLEAN   DmiPort;

  if (!PcieIsDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
    return;
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieClearDeviceStatus Socket %x, Bus %x, Dev %x, Func%x\n", Socket, Bus, Device, Function));

  // DMI Port PCIe capability structure does not follow the PCIe spec.
  DmiPort = PcieIsDmiPort (Socket,Bus, Device, Function, MmInfo);

  //
  //clear AER status
  //
  if (ErrSeverity & (NONFATAL_ERROR_TYPE | FATAL_ERROR_TYPE)) {
    PcieAerClearUncErrSts (Socket, Bus, Device, Function, MmInfo);
  }
  if (ErrSeverity & COR_ERROR_TYPE) {
    PcieAerClearCorrErrSts (Socket, Bus, Device, Function, MmInfo);
  }

  //
  // PcieAerClearRootErrSts only clear for Root Port and RCEC,  DMI Port type = 9 which is RCiEP device,
  // so use PcieAerClearRootErrStsAtom to clear DMI RPERRSTS without checking port type
  //
  if (DmiPort) {
    PcieAerClearRootErrStsAtom (Socket, Bus, Device, Function, MmInfo, ErrSeverity);
  } else {
    PcieAerClearRootErrSts (Socket, Bus, Device, Function, MmInfo, ErrSeverity);
  }

  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket,Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (CapabilitiesOffset != 0) {
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieClearDeviceStatus EFI_PCI_CAPABILITY_ID_PCIEXP = 0x%x\n",CapabilitiesOffset));
    RegTmp16 = 0;
    if (ErrSeverity & COR_ERROR_TYPE) {
        RegTmp16 |= B_PCIE_DEVICE_STATUS_COR_ERR;
      }
    if (ErrSeverity & NONFATAL_ERROR_TYPE) {
        RegTmp16 |= B_PCIE_DEVICE_STATUS_NF_ERR;
      }
    if (ErrSeverity & FATAL_ERROR_TYPE) {
        RegTmp16 |= B_PCIE_DEVICE_STATUS_FAT_ERR;
      }

    //
    // Unsupported Request Error could be COR or UNC, so UR status
    // will be cleared in both way.
    //

    RegTmp16 |= B_PCIE_DEVICE_STATUS_UNSUP_REQ;

    //
    // Clear the PCIe device status register
    //
    if (DmiPort) {
      StatusReg16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, DMI_DEV_STS_OFFSET, MmInfo));
    } else {
      StatusReg16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET, MmInfo));
    }

    if ((StatusReg16 & RegTmp16) != 0) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "      [Helper] PcieClearDeviceStatus RegTmp16 = 0x%x\n", RegTmp16));
      if (DmiPort) {
        MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, DMI_DEV_STS_OFFSET, MmInfo), RegTmp16);
      } else {
        MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET, MmInfo), RegTmp16);
      }
    }
  } //if (CapabilitiesOffset != 0)

  PciClearLegacyDeviceStatus (Socket,Bus, Device, Function, MmInfo);
}

/**
  This function Get the error type of PCI or PCIe device .

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device

  @retval    UINT8            SERROR_TYPE.
                              PERROR_TYPE.
**/
UINT8
EFIAPI
PcieFoundError (
  IN UINT8           Socket,
  IN UINT8           Bus,
  IN UINT8           Device,
  IN UINT8           Function,
  IN PCI_EXT_MM_INFO *MmInfo
  )
{
  UINT16    PrimaryStatus;
  UINT8      PciErrorType;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieFoundError Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));

  PrimaryStatus = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET, MmInfo));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieFoundError PCI_PRIMARY_STATUS_OFFSET = 0x%x\n",PrimaryStatus));
  // Error signaled on the primary bus...
  if (PrimaryStatus & EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR) {
    PciErrorType = SERROR_TYPE;
  } else {
    PciErrorType = PERROR_TYPE;
  }
  return PciErrorType;
}

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
  )
{
  UINT8     CapabilitiesOffset;
  UINT16    RootControl;
  BOOLEAN   IsEnabled;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "IsPcieRootPortErrReportingEnabled Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));

  IsEnabled = FALSE;

  if (PcieIsDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
    // Get the capabilities offset
    CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
    if (CapabilitiesOffset == 0) {
      return FALSE;
    }
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "IsPcieRootPortErrReportingEnabled EFI_PCI_CAPABILITY_ID_PCIEXP = 0x%x\n",CapabilitiesOffset));
    RootControl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET, MmInfo));
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "IsPcieRootPortErrReportingEnabled PCIE_ROOT_CONTROL_OFFSET = 0x%x\n",RootControl));
    if (RootControl & (B_PCIE_ROOT_CONTROL_FAT_ERR_EN | B_PCIE_ROOT_CONTROL_NF_ERR_EN | B_PCIE_ROOT_CONTROL_COR_ERR_EN)) {
      IsEnabled = TRUE;
    }
  }

  return IsEnabled;
}

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
  )
{
  UINT8   CapabilitiesOffset;
  UINT32  PcieSlotNum;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetSlotNum Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));

  PcieSlotNum = 0;

  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket,Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetSlotNum EFI_PCI_CAPABILITY_ID_PCIEXP = 0x%x\n",CapabilitiesOffset));
  if (CapabilitiesOffset) {
    PcieSlotNum = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCI_EXP_CAP_SLCAP, MmInfo));
    // RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetSlotNum PCI_EXP_CAP_SLCAP = 0x%x\n",PcieSlotNum));
  }

  return (UINT16) (PcieSlotNum >> 19);
}

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
  )
{
  UINT16    PrimaryStatus;
  UINT8     PciErrorType;
  UINT16    ErrorMap;
  BOOLEAN   ErrorFound;
  UINT32    Sev;
  RAS_MAIL_BOX  *MailBox = NULL;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PciDeviceErrorHandler Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));

  ErrorFound = FALSE;

  if (!IsFatalNonFatalDeviceError (Socket, Bus, Device, Function, MmInfo)) {
    return FALSE;
  }

  MailBox = GetRasMailBox ();

  ErrorMap = EFI_PCI_STATUS_DETECTED_PARITY_ERROR |
    EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR |
    EFI_PCI_STATUS_RECEIVED_TARGET_ABORT |
    EFI_PCI_MASTER_DATA_PARITY_ERROR;

  // The standard behavior is outlined below:
  // 1. #SERR generation is controlled thru CMD.SEE and CMD.PERE.
  // 2. #SERR generation can also occur if the device is capable of being a bus master
  //    on the primary bus and can detect parity errors. Optionally, #SERR can also be
  //    triggered by setting a chipset-specific policy for bridges and bus masters for
  //    target and master aborts...
  //
  PrimaryStatus = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET, MmInfo));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PciDeviceErrorHandler PCI_PRIMARY_STATUS_OFFSET = 0x%x\n",PrimaryStatus));
  if (PrimaryStatus & ErrorMap) {
    ErrorFound = TRUE;
    // Error signaled on the primary bus...
    if (PrimaryStatus & EFI_PCI_STATUS_SIGNALED_SYSTEM_ERROR) {
      PciErrorType = SERROR_TYPE;
    } else {
      PciErrorType = PERROR_TYPE;
    }

    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciDeviceErrorHandler ReportPcieErrorLog\n"));
    // Report the error...
    ReportPcieErrorLog (Socket, Bus, Device, Function, MmInfo, PciErrorType);
    Sev = GetSevByErrType (PciErrorType);
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciDeviceErrorHandler Sev = 0x%x\n",Sev));
    AdjustOutputSeverity (Sev);

    // Clear error status
    if (MailBox->IioInitPar.IioErrRegistersClearEn) {
      PciClearLegacyDeviceStatus (Socket, Bus, Device, Function, MmInfo);
    }
  }

  return ErrorFound;
}

/**
  This function handles PCIe Uncorrectable (Fatal and Non-Fatal) errors.
  An error is reported and the PCIe error detection status is updated.

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device

  @retval    None
--*/
VOID
EFIAPI
PcieHandleUceDevError (
  IN UINT8           Socket,
  IN UINT8           Bus,
  IN UINT8           Device,
  IN UINT8           Function,
  IN PCI_EXT_MM_INFO *MmInfo,
  IN UINT8           ErrSeverity
  )
{
  UINT8                     CapabilitiesOffset;
  UINT16                    DeviceStatus;
  UINT32                    Sev;
  RAS_MAIL_BOX              *MailBox = NULL;

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieHandleUceDevError Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));

  MailBox = GetRasMailBox ();

  // Get the capabilities offset
  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (CapabilitiesOffset == 0) {
    return;
  }
  // Not a PCI error, report a FATAL PCIe error
  // Check if the device status indicates that a FATAL error or uncorrected error occurred
  DeviceStatus = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET, MmInfo));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieHandleUceDevError DeviceStatus = 0x%x\n",DeviceStatus));
  if (ErrSeverity & FATAL_ERROR_TYPE) {
    if ((DeviceStatus & B_PCIE_DEVICE_STATUS_FAT_ERR)) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieHandleUceDevError ReportPcieErrorLog\n"));
      ReportPcieErrorLog (Socket, Bus, Device, Function, MmInfo, FATAL_ERROR_TYPE);
      if (MailBox->IioInitPar.IioErrRegistersClearEn) {
        PcieClearDeviceStatus (Socket, Bus, Device, Function, MmInfo, FATAL_ERROR_TYPE);
      }
      Sev = GetSevByErrType (FATAL_ERROR_TYPE);
      AdjustOutputSeverity (Sev);
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieHandleUceDevError Sev = 0x%x\n",Sev));
    }
  }

  if (ErrSeverity & NONFATAL_ERROR_TYPE) {
    if ((DeviceStatus & B_PCIE_DEVICE_STATUS_NF_ERR)) {
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieHandleUceDevError ReportPcieErrorLog\n"));
      ReportPcieErrorLog (Socket, Bus, Device, Function, MmInfo, NONFATAL_ERROR_TYPE);
      if (MailBox->IioInitPar.IioErrRegistersClearEn) {
        PcieClearDeviceStatus (Socket, Bus, Device, Function, MmInfo, NONFATAL_ERROR_TYPE);
      }
      Sev = GetSevByErrType (NONFATAL_ERROR_TYPE);
      AdjustOutputSeverity (Sev);
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieHandleUceDevError Sev = 0x%x\n",Sev));
    }
  }
}

/**
  This function is to check if the  PCIe Correctable errors over the limit.

  @param[in] Socket         Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @retval    TRUE   The error over limitation, corretable error should be disabled for this device.
  @retval    FALSE  The error count is under control, will report the error only, not to disable it.
**/
BOOLEAN
IsPcieCorrectedErrorOverLimit (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{

  PCIE_CE_COUNTER_LIST    *PcieCeCounter = NULL;
  LIST_ENTRY               *Link;
  BOOLEAN                  Found;
  RAS_MAIL_BOX             *MailBox = NULL;

  MailBox = GetRasMailBox ();
  if (MailBox->PcieInitPar.PcieCorErrLimitEn == 0) {
    return FALSE;
  }

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "IsPcieCorrectedErrorOverLimit Socket = 0x%x Bus = 0x%x Device = 0x%x Function = 0x%x\n",
    Socket,
    Bus,
    Device,
    Function
    ));

  //
  // Find first PCIE error counter
  //
  Link = GetFirstNode (MailBox->MiscPar.PtrPcieCeCounterList);

  //
  // Search the corresponding error counter
  //
  Found = FALSE;
  while (!IsNull (MailBox->MiscPar.PtrPcieCeCounterList, Link)) {
    PcieCeCounter = PCIE_CE_COUNTER_FROM_LINK (Link);

    if (PcieCeCounter->Socket == Socket &&
        PcieCeCounter->Bus == Bus &&
        PcieCeCounter->Device == Device &&
        PcieCeCounter->Function == Function) {
      Found = TRUE;
      PcieCeCounter->CeCount++;
      break;
    }

    Link = GetNextNode (MailBox->MiscPar.PtrPcieCeCounterList, Link);
  }

  if (!Found) {
    //
    // Insert to new counter to the list
    //
    PcieCeCounter = AllocateZeroPool (sizeof (PCIE_CE_COUNTER_LIST));
    if (PcieCeCounter == NULL) {
     RAS_DEBUG ((LEVEL_BASIC_FLOW, "PcieCeCounter is NULL \n"));
     RAS_ASSERT (FALSE);
     return FALSE;
    }

    if (PcieCeCounter != NULL) {
      PcieCeCounter->Signature = PCIE_CE_COUNTER_SIGNATURE;
      PcieCeCounter->Socket = Socket;
      PcieCeCounter->Bus = Bus;
      PcieCeCounter->Device = Device;
      PcieCeCounter->Function = Function;
      PcieCeCounter->CeCount = 1;
      InsertTailList (MailBox->MiscPar.PtrPcieCeCounterList, &PcieCeCounter->Link);
    }
  }

  RAS_DEBUG ((
    LEVEL_FUNC_FLOW,
    "IsPcieCorrectedErrorOverLimit CeCount = %d Limit = %d\n",
    PcieCeCounter->CeCount,
    MailBox->PcieInitPar.PcieCorErrLimit
    ));

  if (PcieCeCounter->CeCount < MailBox->PcieInitPar.PcieCorErrLimit) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  This function handles PCIe Correctable errors.
  An error is reported if the correctable error threshold has not been reached.
  If the threshold has been reached, correctable errors are masked in the device.

  @param[in] Socket         Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    None
--*/
VOID
EFIAPI
PcieHandleCeDevError (
  IN UINT8           Socket,
  IN UINT8           Bus,
  IN UINT8           Device,
  IN UINT8           Function,
  IN PCI_EXT_MM_INFO *MmInfo
  )
{
  UINT8                    CapabilitiesOffset;
  UINT16                   DeviceControl;
  UINT16                   RootControl;
  BOOLEAN                  RootPortOrRcec;
  UINT32                   Sev;
  RAS_MAIL_BOX             *MailBox = NULL;

  MailBox = GetRasMailBox ();

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieHandleCeDevError Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));

  Sev = GetSevByErrType (COR_ERROR_TYPE);
  AdjustOutputSeverity (Sev);
  ReportPcieErrorLog (Socket, Bus, Device, Function, MmInfo, COR_ERROR_TYPE);

  // Check if the correctable-error reporting limit has already been reached.
  if (IsPcieCorrectedErrorOverLimit (Socket, Bus, Device, Function)) {
    // Error limit has been reached.  Mask correctable errors in the device.
    CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
    if (CapabilitiesOffset == 0) {
      return;
    }

    DeviceControl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET, MmInfo));

    DeviceControl &= ~(B_PCIE_DEVICE_CONTROL_COR_ERR_EN);
    MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET, MmInfo), DeviceControl);

    RootPortOrRcec = IsPcieRootPort(Socket, Bus, Device, Function, MmInfo) || IsPcieRcecSupported(Socket, Bus, Device, Function, MmInfo);
    //
    // Update the Root Control register if this is a root port or RCEC.
    //
    if (RootPortOrRcec) {
      RootControl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET, MmInfo));
      RootControl &= ~(B_PCIE_ROOT_CONTROL_COR_ERR_EN);
      MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET, MmInfo), RootControl);
    }
  }

  if (MailBox->IioInitPar.IioErrRegistersClearEn) {
    PcieClearDeviceStatus (Socket, Bus, Device, Function, MmInfo, COR_ERROR_TYPE);
  }
}

/**
  This function handles DPC event.

  @param[in] Socket         Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    None
--*/
VOID
EFIAPI
PcieHandleEdpc (
  IN UINT8           Socket,
  IN UINT8           Bus,
  IN UINT8           Device,
  IN UINT8           Function,
  IN PCI_EXT_MM_INFO *MmInfo
  )
{

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieHandleEdpc Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n", Socket, Bus, Device, Function));

  //
  //A surprise hot remove may trigger containment event as well. Since surprise hot remove
  //event is signaled to OS via hot plug interrupt, FW should not issue Notify(0xF) to avoid
  //double reporting.
  //
  if (IsPcieSurpriseHotRemoveEvent (Socket, Bus, Device, Function, MmInfo)) {
    return;
  }

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieHandleEdpc Init ACPI Parameters...\n"));
  EdpcInitRasAcpiParam (Socket, Bus, Device, Function);

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieHandleEdpc Triggering SW SCI...\n"));
  SignalWheaSci();

}

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
  )
{
  BOOLEAN   ErrorDetected = FALSE;
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieDeviceErrorHandler Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x, SevPerStack = 0x%x\n",Socket,Bus,Device,Function,SevPerStack));


  // Check for an Uncorrectable PCIe error
  if ((SevPerStack & NONFATAL_ERROR_TYPE) || (SevPerStack & FATAL_ERROR_TYPE)) {
    if (IsFatalNonFatalDeviceError (Socket, Bus, Device, Function, MmInfo)) {
      ErrorDetected = TRUE;
      PcieHandleUceDevError (Socket, Bus, Device, Function, MmInfo, SevPerStack);
    }
  }

  // Check for a Correctable PCIe error
  if ((SevPerStack & COR_ERROR_TYPE)) {
    if (PcieIsCorrectableDeviceError (Socket, Bus, Device, Function, MmInfo)) {
      ErrorDetected = TRUE;
      PcieHandleCeDevError (Socket, Bus, Device,Function, MmInfo);
    }
    if (PcieIsEdpcTiggered (Socket, Bus, Device, Function, MmInfo)) {
      PcieHandleEdpc (Socket, Bus, Device,Function, MmInfo);
    }
  }

  return ErrorDetected;
}

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
  )
{
  BOOLEAN                  ErrorDetected = FALSE;
  UINT16                   DeviceStatus;
  UINT16                   DeviceControl;
  UINT32                   Sev;
  RAS_MAIL_BOX             *MailBox = NULL;

  MailBox = GetRasMailBox ();

  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieDmiDeviceErrorHandler Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));

  DeviceStatus = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (DMI_DEV_STS_OFFSET), MmInfo));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieDmiDeviceErrorHandler DMI_DEV_STS = 0x%x\n",DeviceStatus));
  if (SevPerStack & FATAL_ERROR_TYPE) {
    if ((DeviceStatus & B_PCIE_DEVICE_STATUS_FAT_ERR)) {
      ErrorDetected = TRUE;
      // FATAL error reported
      MailBox->MiscPar.PcieFatalErrDetected = TRUE;
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieDmiDeviceErrorHandler ReportPcieErrorLog\n"));
      ReportPcieErrorLog (Socket, Bus, Device, Function, MmInfo, FATAL_ERROR_TYPE);
      Sev = GetSevByErrType (FATAL_ERROR_TYPE);
      AdjustOutputSeverity (Sev);
    }
  }

  if (SevPerStack & NONFATAL_ERROR_TYPE) {
    if (DeviceStatus & B_PCIE_DEVICE_STATUS_NF_ERR) {
      ErrorDetected = TRUE;
      MailBox->MiscPar.PcieNonFatalErrDetected = TRUE;
      RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieDmiDeviceErrorHandler ReportPcieErrorLog\n"));
      ReportPcieErrorLog (Socket, Bus, Device, Function, MmInfo, NONFATAL_ERROR_TYPE);
      Sev = GetSevByErrType (NONFATAL_ERROR_TYPE);
      AdjustOutputSeverity (Sev);
    }
  }

  if (SevPerStack & COR_ERROR_TYPE) {
    if (DeviceStatus & B_PCIE_DEVICE_STATUS_COR_ERR) {
      ErrorDetected = TRUE;
      // Check if the correctable-error reporting limit has already been reached.
      if (!(IsPcieCorrectedErrorOverLimit (Socket, Bus, Device, Function))) {
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieDmiDeviceErrorHandler ReportPcieErrorLog\n"));

        Sev = GetSevByErrType (COR_ERROR_TYPE);
        AdjustOutputSeverity (Sev);
        ReportPcieErrorLog (Socket, Bus, Device, Function, MmInfo, COR_ERROR_TYPE);
      } else {// Error limit has been reached.  Mask correctable errors in the device.
        DeviceControl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (DMI_DEV_CTL_OFFSET), MmInfo));
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieDmiDeviceErrorHandler DMI_DEV_CTL = 0x%x\n",DeviceControl));
        DeviceControl &= ~(B_PCIE_DEVICE_CONTROL_COR_ERR_EN);
        MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, ONLY_REGISTER_OFFSET (DMI_DEV_CTL_OFFSET), MmInfo), DeviceControl);
      }
    }
  }

  if (ErrorDetected && MailBox->IioInitPar.IioErrRegistersClearEn) {
    PcieClearDeviceStatus (Socket, Bus, Device, Function, MmInfo, SevPerStack);
  }
  return ErrorDetected;
}

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
  )
{
  UINT16    AerCapOffset;
  UINT8     PcieCapOffset;
  UINT16    Reg16;
  UINT32    Reg32;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus,Device, Function, MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);

  if (!AerCapOffset) {
    return EFI_NOT_FOUND;
  }

  PcieCapOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PcieCapOffset == 0) {
    return EFI_NOT_FOUND;
  }
  //
  // Write UNCERRSTS[20] = 1 to clear AER UR.
  //
  Reg32 = B_PCIE_AER_UES_UNS_REQ_ERR;
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UES, MmInfo), Reg32);

  Reg32 = B_PCIE_AER_CES_ADV_NON_FAT_ERR;
  MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_CES, MmInfo), Reg32);

  //
  // Write DEVSTS[3] = 1 to clear DEVSTS UR
  //
  Reg16 = B_PCIE_DEVICE_STATUS_UNSUP_REQ;

  //
  // PCI Probing will cause Advisory Non-Fatal error, which is a correctable error
  // we need to clear B_PCIE_DEVICE_STATUS_COR_ERR according to original device error status
  // SevPerStack == COR_ERROR_TYPE      OriginDevSts & B_PCIE_DEVICE_STATUS_COR_ERR == 1  clear B_PCIE_DEVICE_STATUS_COR_ERR
  // SevPerStack == COR_ERROR_TYPE      OriginDevSts & B_PCIE_DEVICE_STATUS_COR_ERR == 0  clear B_PCIE_DEVICE_STATUS_COR_ERR
  // SevPerStack == NONFATAL_ERROR_TYPE OriginDevSts & B_PCIE_DEVICE_STATUS_COR_ERR == 1  don't clear B_PCIE_DEVICE_STATUS_COR_ERR
  // SevPerStack == NONFATAL_ERROR_TYPE OriginDevSts & B_PCIE_DEVICE_STATUS_COR_ERR == 0  clear B_PCIE_DEVICE_STATUS_COR_ERR
  // SevPerStack == FATAL_ERROR_TYPE    OriginDevSts & B_PCIE_DEVICE_STATUS_COR_ERR == 1  don't clear B_PCIE_DEVICE_STATUS_COR_ERR
  // SevPerStack == FATAL_ERROR_TYPE    OriginDevSts & B_PCIE_DEVICE_STATUS_COR_ERR == 0  clear B_PCIE_DEVICE_STATUS_COR_ERR
  if ((SevPerStack == COR_ERROR_TYPE) ||
       ((SevPerStack == NONFATAL_ERROR_TYPE) && ((OriginDevSts & B_PCIE_DEVICE_STATUS_COR_ERR) == 0)) ||
       ((SevPerStack == FATAL_ERROR_TYPE) && ((OriginDevSts & B_PCIE_DEVICE_STATUS_COR_ERR) == 0))) {
    Reg16 |= B_PCIE_DEVICE_STATUS_COR_ERR;
  }
  MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, PcieCapOffset + PCIE_DEVICE_STATUS_OFFSET, MmInfo), Reg16);

  return EFI_SUCCESS;
}

/**
  This function is to handle multiple errors happen on PCI Express Root Ports.

  @param[in] RootPortBus            PCIe root port device's bus number.
  @param[in] RootPortDevice         PCIe root port device's device number.
  @param[in] RootPortFunction       PCIe root port device's function number.
  @param[in] MmInfo                 Memory Map information of target device.
  @param[in] SevPerStack            Error Severity that we are processing.

  @retval    BOOLEAN        TRUE   - An error was detected.
                            FALSE  - An error was not detected.
--*/
BOOLEAN
EFIAPI
PcieRootPortMultiErrorsHandler (
  IN UINT8            Socket,
  IN UINT8            RootPortBus,
  IN UINT8            RootPortDevice,
  IN UINT8            RootPortFunction,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT8            SevPerStack
  )
{
  UINT8           Bus;
  UINT8           Device;
  UINT8           Function;
  BOOLEAN         ErrorDetected = FALSE;
  UINT8           FuncExist = 0;
  UINT8           FuncMap;
  UINT8           SecondaryBusNumber;
  UINT8           SubordinateBusNumber;
  RAS_MAIL_BOX    *MailBox = NULL;
  UINT16          RpDevSts;
  UINT16          EpDevSts[PCI_MAX_FUNC + 1];
  UINT32          RpUncerrmsk;
  UINT32          RpCorerrmsk;
  UINT32          EpUncerrmsk[PCI_MAX_FUNC + 1];
  UINT32          EpCorerrmsk[PCI_MAX_FUNC + 1];
  UINT32          Data32;
  UINT16          TempBus;

  MailBox = GetRasMailBox ();

  //
  // In case mulitple errors happened, Root Port also possible have its own error even there are
  // some errors on its child device. So Root Port also need to be checked. And more importantly,
  // PCIE Root Port erros should be handled firstly as following PCI scoping might cause
  // UR on this root port.
  //

  //
  // Save Device status, Error Masks for later use
  //
  PcieGetDevCtlAndDevSts (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo, NULL, &RpDevSts);
  RpUncerrmsk = PcieAerGetUncErrMsk (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo);
  RpCorerrmsk = PcieAerGetCorrErrMsk (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo);

  if (PcieIsDmiPort (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo)) {
    //
    // DMI port is different with common pcie port, its PCIEXP capbilities devsts register can't reflect the real devsts.
    //
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "PcieRootPortMultiErrorsHandler DMI Port Error handler .\n"));
    ErrorDetected = PcieDmiDeviceErrorHandler (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo, SevPerStack);
  } else {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "PcieRootPortMultiErrorsHandler RP Error handler.\n"));
    //
    // Handle Root Port Error
    //
    ErrorDetected = PcieDeviceErrorHandler (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo, SevPerStack);
    //
    // Mask UR and ANF error
    //
    Data32 = RpUncerrmsk | B_PCIE_AER_UES_UNS_REQ_ERR;
    PcieAerProgramUncErrMsk (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo, Data32);
    Data32 = RpCorerrmsk | B_PCIE_AER_CES_ADV_NON_FAT_ERR;
    PcieAerProgramCorrErrMsk (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo, Data32);
  }

  //
  // Check for errors on subordinate devices
  //
  SecondaryBusNumber = MmioRead8 (MmPciAddressExt (Socket, RootPortBus, RootPortDevice, RootPortFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, MmInfo));
  SubordinateBusNumber = MmioRead8 (MmPciAddressExt (Socket, RootPortBus, RootPortDevice, RootPortFunction, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, MmInfo));

  if (SecondaryBusNumber && SubordinateBusNumber) {
    for (TempBus = SecondaryBusNumber; TempBus <= SubordinateBusNumber; TempBus++) {
      Bus = (UINT8) TempBus;
      for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
        FuncExist = 0;
        for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {
          if (PcieIsDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
            //
            // Save Device status and error masks for later use
            //
            PcieGetDevCtlAndDevSts (Socket, Bus, Device, Function, MmInfo, NULL, &EpDevSts[Function]);
            EpUncerrmsk[Function] = PcieAerGetUncErrMsk (Socket, Bus, Device, Function, MmInfo);
            EpCorerrmsk[Function] = PcieAerGetCorrErrMsk (Socket, Bus, Device, Function, MmInfo);
            //
            // If there was a gap in function number, the function number behind the gap
            // will have UR because of PCI probing, need to clear it to void reporting it
            // mistakenly
            //
            FuncMap = (BIT0 << Function)-1;
            if ((FuncExist & FuncMap) != FuncMap) {
              PcieSuppressUnsupportReqest (Socket, Bus, Device, Function, MmInfo, 0, 0);
              RAS_DEBUG ((LEVEL_BASIC_FLOW, "PcieRootPortMultiErrorsHandler : UR Cleared on B: %x, D: %x, F:%x.\n"));
            }
            FuncExist |= (BIT0 << Function);
            ErrorDetected = IsPcieDevice (Socket, Bus, Device, Function, MmInfo) ? PcieDeviceErrorHandler (Socket, Bus, Device, Function, MmInfo, SevPerStack) : PciDeviceErrorHandler (Socket, Bus, Device, Function, MmInfo);
            //
            // Mask UR and ANF error
            //
            Data32 = EpUncerrmsk[Function] | B_PCIE_AER_UES_UNS_REQ_ERR;
            PcieAerProgramUncErrMsk (Socket, Bus, Device, Function, MmInfo, Data32);
            Data32 = EpCorerrmsk[Function] | B_PCIE_AER_CES_ADV_NON_FAT_ERR;
            PcieAerProgramCorrErrMsk (Socket, Bus, Device, Function, MmInfo, Data32);
            //
            // If it is non nulti-function device, break function loop
            //
            if (Function == 0 && !IsMultiFunctionDevice (Socket, Bus, Device, Function, MmInfo)) {
              break;
            }
          } else {
            if (Function == 0) {
              break;
            } else {
              continue;
            }
          }
        }// for (Function = 00; Function <= PCI_MAX_FUNC; Function++)

        //
        // During PCI probing, UR error will be logged in exist functions, need to clear
        // them before going forward.
        //
        for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {
          if (FuncExist & (BIT0 << Function)) {
          if (PcieIsDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
              PcieSuppressUnsupportReqest(Socket, Bus, Device, Function, MmInfo, EpDevSts[Function], SevPerStack); // Restore AER UR mask
              RAS_DEBUG ((LEVEL_BASIC_FLOW, "PcieRootPortMultiErrorsHandler : UR Cleared on B: %x, D: %x F:%x.\n", Bus, Device, Function));
              //
              // Restore UR and ANF error
              //
              PcieAerProgramUncErrMsk (Socket, Bus, Device, Function, MmInfo, EpUncerrmsk[Function]);
              PcieAerProgramCorrErrMsk (Socket, Bus, Device, Function, MmInfo, EpCorerrmsk[Function]);
          }
        }
        }
      }// for (Device = 00; Device <= PCI_MAX_DEVICE; Device++)
    } // for (TempBus = SecondaryBusNumber; TempBus <= SubordinateBusNumber; TempBus++)
  } // if (SecondaryBusNumber && SubordinateBusNumber)

  //
  // During PCI probing, UR error will be logged in exist functions, need to clear
  // them before going forward.
  //
  PcieSuppressUnsupportReqest(Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo, RpDevSts, SevPerStack);
  //
  // Restore UR and ANF error
  //
  PcieAerProgramUncErrMsk (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo, RpUncerrmsk);
  PcieAerProgramCorrErrMsk (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo, RpCorerrmsk);

  return ErrorDetected;
}

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
  )
{
  UINT8           Bus;
  UINT8           Device;
  UINT8           Function;
  BOOLEAN         ErrorDetected = FALSE;
  UINT8           FuncExist = 0;
  UINT8           FuncMap;
  UINT8           NextBusNumber;
  UINT8           LastBusNumber;
  RAS_MAIL_BOX    *MailBox = NULL;
  UINT16          RpDevSts;
  UINT16          EpDevSts[PCI_MAX_FUNC + 1];
  UINT32          RpUncerrmsk;
  UINT32          RpCorerrmsk;
  UINT32          EpUncerrmsk[PCI_MAX_FUNC + 1];
  UINT32          EpCorerrmsk[PCI_MAX_FUNC + 1];
  UINT32          Data32;
  UINT32          Abm;
  UINT32          DeviceMap;
  UINT16          TempBus;

  MailBox = GetRasMailBox ();

  //
  // In case mulitple errors happened, RCEC also possible have its own error even there are
  // some errors on its child device. So RCEC also need to be checked. And more importantly,
  // PCIE RCEC erros should be handled firstly as following PCI scoping might cause
  // UR on this RCEC.
  //

  //
  // Save Device status, Error Masks for later use
  //
  PcieGetDevCtlAndDevSts (Socket, RcecBus, RcecDevice, RcecFunction, MmInfo, NULL, &RpDevSts);
  RpUncerrmsk = PcieAerGetUncErrMsk (Socket, RcecBus, RcecDevice, RcecFunction, MmInfo);
  RpCorerrmsk = PcieAerGetCorrErrMsk (Socket, RcecBus, RcecDevice, RcecFunction, MmInfo);

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "PcieRcecMultiErrorsHandler RP Error handler.\n"));
  //
  // Handle RCEC Error
  //
  ErrorDetected = PcieDeviceErrorHandler (Socket, RcecBus, RcecDevice, RcecFunction, MmInfo, SevPerStack);
  //
  // Mask UR and ANF error
  //
  Data32 = RpUncerrmsk | B_PCIE_AER_UES_UNS_REQ_ERR;
  PcieAerProgramUncErrMsk (Socket, RcecBus, RcecDevice, RcecFunction, MmInfo, Data32);
  Data32 = RpCorerrmsk | B_PCIE_AER_CES_ADV_NON_FAT_ERR;
  PcieAerProgramCorrErrMsk (Socket, RcecBus, RcecDevice, RcecFunction, MmInfo, Data32);

  //
  // Check for errors on RCEC associated devices
  //
  GetRcecNbnLbnAbm (Socket, RcecBus, RcecDevice, RcecFunction, MmInfo, &NextBusNumber, &LastBusNumber, &Abm);
  if (NextBusNumber == 0xff && LastBusNumber == 0) {
    //
    // In this case, all the RCiEP devices on the same bus number of RCEC
    //
    NextBusNumber = RcecBus;
    LastBusNumber = RcecBus;
  }

  //
  // Start from the bus number of RCEC, till last bus number
  //
  for (TempBus = RcecBus; TempBus <= LastBusNumber; TempBus++) {
    Bus = (UINT8) TempBus;
    //
    // Need to check ABM (Associated Bit Map) for the RCiEP on the same bus of RCEC
    //
    if (Bus == RcecBus) {
      DeviceMap = Abm;
    } else {
      DeviceMap = 0xFFFFFFFF;
    }

    for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
      if ((DeviceMap & (1 << Device)) == 0) {
          continue;
      }
      FuncExist = 0;
      for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {
        if (PcieIsDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
          //
          // Save Device status and error masks for later use
          //
          PcieGetDevCtlAndDevSts (Socket, Bus, Device, Function, MmInfo, NULL, &EpDevSts[Function]);
          EpUncerrmsk[Function] = PcieAerGetUncErrMsk (Socket, Bus, Device, Function, MmInfo);
          EpCorerrmsk[Function] = PcieAerGetCorrErrMsk (Socket, Bus, Device, Function, MmInfo);
          //
          // If there was a gap in function number, the function number behind the gap
          // will have UR because of PCI probing, need to clear it to void reporting it
          // mistakenly
          //
          FuncMap = (BIT0 << Function)-1;
          if ((FuncExist & FuncMap) != FuncMap) {
            PcieSuppressUnsupportReqest (Socket, Bus, Device, Function, MmInfo, 0, 0);
            RAS_DEBUG ((LEVEL_BASIC_FLOW, "PcieRcecMultiErrorsHandler : UR Cleared on B: %x, D: %x, F:%x.\n"));
          }
          FuncExist |= (BIT0 << Function);
          ErrorDetected = IsPcieDevice (Socket, Bus, Device, Function, MmInfo) ? PcieDeviceErrorHandler (Socket, Bus, Device, Function, MmInfo, SevPerStack) : PciDeviceErrorHandler (Socket, Bus, Device, Function, MmInfo);
          //
          // Mask UR and ANF error
          //
          Data32 = EpUncerrmsk[Function] | B_PCIE_AER_UES_UNS_REQ_ERR;
          PcieAerProgramUncErrMsk (Socket, Bus, Device, Function, MmInfo, Data32);
          Data32 = EpCorerrmsk[Function] | B_PCIE_AER_CES_ADV_NON_FAT_ERR;
          PcieAerProgramCorrErrMsk (Socket, Bus, Device, Function, MmInfo, Data32);
          //
          // If it is non nulti-function device, break function loop
          //
          if (Function == 0 && !IsMultiFunctionDevice (Socket, Bus, Device, Function, MmInfo)) {
            break;
          }
        } else {
          if (Function == 0) {
            break;
          } else {
            continue;
          }
        }
      }// for (Function = 00; Function <= PCI_MAX_FUNC; Function++)

      //
      // During PCI probing, UR error will be logged in exist functions, need to clear
      // them before going forward.
      //
      for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {
        if (FuncExist & (BIT0 << Function)) {
        if (PcieIsDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
            PcieSuppressUnsupportReqest(Socket, Bus, Device, Function, MmInfo, EpDevSts[Function], SevPerStack); // Restore AER UR mask
            RAS_DEBUG ((LEVEL_BASIC_FLOW, "PcieRcecMultiErrorsHandler : UR Cleared on B: %x, D: %x F:%x.\n", Bus, Device, Function));
            //
            // Restore UR and ANF error
            //
            PcieAerProgramUncErrMsk (Socket, Bus, Device, Function, MmInfo, EpUncerrmsk[Function]);
            PcieAerProgramCorrErrMsk (Socket, Bus, Device, Function, MmInfo, EpCorerrmsk[Function]);
        }
      }
      }
    }// for (Device = 00; Device <= PCI_MAX_DEVICE; Device++)
  } // for (TempBus = NextBusNumber; TempBus <= LastBusNumber; TempBus++)

  //
  // During PCI probing, UR error will be logged in exist functions, need to clear
  // them before going forward.
  //
  PcieSuppressUnsupportReqest(Socket, RcecBus, RcecDevice, RcecFunction, MmInfo, RpDevSts, SevPerStack);
  //
  // Restore UR and ANF error
  //
  PcieAerProgramUncErrMsk (Socket, RcecBus, RcecDevice, RcecFunction, MmInfo, RpUncerrmsk);
  PcieAerProgramCorrErrMsk (Socket, RcecBus, RcecDevice, RcecFunction, MmInfo, RpCorerrmsk);

  return ErrorDetected;
}

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
  )
{
  UINT8           Bus;
  UINT8           Device;
  UINT8           Function;
  BOOLEAN         ErrorDetected = FALSE;
  UINT8           Index;
  UINT32          RootErrSts;
  UINT32          AerErrSid;
  EFI_STATUS      Status;
  RAS_MAIL_BOX    *MailBox = NULL;

  MailBox = GetRasMailBox ();

  //
  // Check if system error reporting is enabled for this device
  //
  if (!IsPcieRootPortErrReportingEnabled (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo)) {
    return FALSE;
  }

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "PcieRootPortErrorHandler MailBox->PcieInitPar.SerrEmuTestEn = 0x%x\n", MailBox->PcieInitPar.SerrEmuTestEn));
  if (MailBox->PcieInitPar.SerrEmuTestEn) {
    if (MailBox->PcieInitPar.SerrPropEn) {
      PciDeviceErrorHandler (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo);
      return TRUE;
    } else {
      OverrideOutputSeverity (EFI_ACPI_6_2_ERROR_SEVERITY_NONE);
      return TRUE;
    }
  }

  Status = PcieAerGetRpErrSts (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo, &RootErrSts);
  RAS_ASSERT (Status == EFI_SUCCESS);
  //
  //  Loop COR and NON-FAT/FAT Error Severity
  //
  for (Index = 0; Index < 2; Index++) {
    //
    // Check how many errors happened
    //
    switch ((RootErrSts >> (Index*2)) & (BIT1 + BIT0)) {
      case NO_ERROR:
        //
        // There is no error on this severity, try next severity
        //
        continue;
      case SINGLE_ERROR:
        //
        // This is a single error, get the error source from AER Error Source ID
        //
        AerErrSid = PcieAerGetErrSid (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo);
        AerErrSid = AerErrSid >> (Index*16);
        Bus = (UINT8) (AerErrSid >> 8);
        //
        // CPX ESID got wrong device/function number for the error detected by Root Port, need to have
        // a workaround for this case
        //
        if ((MailBox->PcieInitPar.PcieAerErrsidWa) && (Bus == RootPortBus)) {
          Device = RootPortDevice;
          Function = RootPortFunction;
        } else {
          Device = (UINT8) ((AerErrSid >> 3) & 0x1f);
          Function = (UINT8) (AerErrSid  & 0x07);
        }
        ErrorDetected = IsPcieDevice (Socket, Bus, Device, Function, MmInfo) ? PcieDeviceErrorHandler (Socket, Bus, Device, Function, MmInfo, SevPerStack) : PciDeviceErrorHandler (Socket, Bus, Device, Function, MmInfo);
        break;
      case MULTIPLE_ERRORS:
        //
        // Check each PCIE devices and handle the error
        //
        ErrorDetected = PcieRootPortMultiErrorsHandler (Socket, RootPortBus, RootPortDevice, RootPortFunction, MmInfo, SevPerStack);
        break;
      default:
        RAS_DEBUG ((LEVEL_BASIC_FLOW, "PcieRootPortErrorHandler - Socket %x, Bus %x, device %x, fuction %x, invalid RootErrorStatus\n", Socket, RootPortBus, RootPortDevice, RootPortFunction));
        RAS_ASSERT (FALSE);
        break;
    }
  }
  return ErrorDetected;
}

/**
  This function enable pcie error loging for correctable uncorrectable and fata
  error.

  @param[in] Socet          Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.
  @param[in] DevCtl         Device Control Register Setting.

  @retval    None
**/
VOID
EFIAPI
PcieConfigErrCtrl (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT16           DevCtl
  )
{
  UINT8     CapabilitiesOffset;
  UINT32    DeviceDapabilities;
  UINT16    DeviceControl;
  UINT16    RootControl;
  BOOLEAN   DmiPort;
  BOOLEAN   RootPortOrRcec;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieConfigErrCtrl Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x,PcieCorrErrEn = 0x%x, PcieUncorrErrEn = 0x%x,PcieFatalErrEn = 0x%x,\n",Socket,Bus,Device,Function,PcieCorrErrEn,PcieUncorrErrEn,PcieFatalErrEn));
  // Enable SERR on Fatal, Non-Fatal, and Correctable errors for error reporting.
  // An NMI will not be generated for correctable errors.

  PcieClearDeviceStatus (Socket, Bus, Device, Function, MmInfo, DevCtl);

  // DMI Port PCIe capability structure does not follow the PCIe spec.
  DmiPort = PcieIsDmiPort (Socket, Bus, Device, Function, MmInfo);

  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (CapabilitiesOffset == 0) {
    return;
  }
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieConfigErrCtrl EFI_PCI_CAPABILITY_ID_PCIEXP = 0x%x\n",CapabilitiesOffset));
  DeviceDapabilities = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CAPABILITY_OFFSET, MmInfo));
  //
  // Check if it is Root Port or RCEC
  //
  RootPortOrRcec = IsPcieRootPort(Socket, Bus, Device, Function, MmInfo) || IsPcieRcecSupported(Socket, Bus, Device, Function, MmInfo) ;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieConfigErrCtrl DmiPort = 0x%x\n",DmiPort));
  if (DmiPort) {
    DeviceControl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, DMI_DEV_CTL_OFFSET, MmInfo));
    DeviceControl |= DevCtl;
    if (DeviceDapabilities & ROLE_BASED_ERROR_REPORTING) {
      DeviceControl |= B_PCIE_DEVICE_CONTROL_UNSUP_REQ_EN;
    } else {
      DeviceControl &= ~B_PCIE_DEVICE_CONTROL_UNSUP_REQ_EN;
    }
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieConfigErrCtrl DeviceControl = 0x%x\n",DeviceControl));
    MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, DMI_DEV_CTL_OFFSET, MmInfo), DeviceControl);
  } else {
    DeviceControl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET, MmInfo));
    DeviceControl |= DevCtl;
    if (DeviceDapabilities & ROLE_BASED_ERROR_REPORTING) {
      DeviceControl |= B_PCIE_DEVICE_CONTROL_UNSUP_REQ_EN;
    } else {
      DeviceControl &= ~B_PCIE_DEVICE_CONTROL_UNSUP_REQ_EN;
    }
    MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET, MmInfo), DeviceControl);
  }

  if (RootPortOrRcec || DmiPort) {
    RootControl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET, MmInfo));
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieConfigErrCtrl RootControl = 0x%x\n",RootControl));
    RootControl |= DevCtl;
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieConfigErrCtrl RootControl = 0x%x\n",DeviceControl));
    MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_ROOT_CONTROL_OFFSET, MmInfo), RootControl);
  }

}

/**
  This function is to enable SERR and PERR for PCI Bridge if applicable
  error.

  @param[in] Socet          Socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    None
**/
VOID
PciEnableBridgeControl (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  UINT8 Data8;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciEnableBridgeControl Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));

  RAS_MAIL_BOX  *MailBox = NULL;

  MailBox = GetRasMailBox ();

  Data8 = MmioRead8 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_BRIDGE_CONTROL_REGISTER_OFFSET, MmInfo));

  // Enable SERR if selected in Setup
  if (MailBox->PcieInitPar.SerrPropEn) {
    Data8 |= EFI_PCI_BRIDGE_CONTROL_SERR;

    // enable PERR if selected in Setup
    if (MailBox->PcieInitPar.PerrPropEn) {
      Data8 |= EFI_PCI_BRIDGE_CONTROL_PARITY_ERROR_RESPONSE;
    } else {

      // if PERR is disabled in Setup, disable PERR alone
      Data8 &= ~EFI_PCI_BRIDGE_CONTROL_PARITY_ERROR_RESPONSE;
    }
  } else {

    // if SERR is disabled in setup, disable both SERR and PERR generation
    Data8 &= ~(EFI_PCI_BRIDGE_CONTROL_SERR | EFI_PCI_BRIDGE_CONTROL_PARITY_ERROR_RESPONSE);
  }

  MmioWrite8 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_BRIDGE_CONTROL_REGISTER_OFFSET, MmInfo), Data8);
}

/**
  This function enables SERR reporting in the PCIE/P2P bridge's

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    None
--*/
VOID
EFIAPI
PciConfigSerr (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN BOOLEAN          EnableFlag
  )
{
  UINT16   Data16;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciConfigSerr Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));
  Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_COMMAND_OFFSET, MmInfo));
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciConfigSerr PCI_COMMAND_OFFSET = 0x%x,EnableFlag = 0x%x.\n",Data16,EnableFlag));
  if (EnableFlag) {
    Data16 |= EFI_PCI_COMMAND_SERR;
  } else{
    Data16 &= ~(EFI_PCI_COMMAND_SERR | EFI_PCI_COMMAND_PARITY_ERROR_RESPOND);
  }
  MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_COMMAND_OFFSET, MmInfo), Data16);

}

/**
  This function enables PERR reporting in the PCIE/P2P bridge's

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    None
--*/
VOID
EFIAPI
PciConfigPerr (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN BOOLEAN          EnableFlag
  )
{
  UINT8    Data8;
  UINT16   Data16;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciConfigPerr Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));

  if (PcieIsPciBridgeDevice (Socket,Bus, Device, Function, MmInfo)) {
    Data8 = MmioRead8 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_BRIDGE_CONTROL_REGISTER_OFFSET, MmInfo));
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciConfigPerr PCI_BRIDGE_CONTROL_REGISTER_OFFSET = 0x%x\n",Data8));
    if (EnableFlag) {
      Data8 |= EFI_PCI_BRIDGE_CONTROL_PARITY_ERROR_RESPONSE;
    } else{
      Data8 &= ~(EFI_PCI_BRIDGE_CONTROL_PARITY_ERROR_RESPONSE);
    }

    MmioWrite8 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_BRIDGE_CONTROL_REGISTER_OFFSET, MmInfo), Data8);
  }

  Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_COMMAND_OFFSET, MmInfo));
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciConfigPerr PCI_COMMAND_OFFSET = 0x%x,EnableFlag = 0x%x\n",Data16,EnableFlag));
  if (EnableFlag) {
    Data16 |= EFI_PCI_COMMAND_PARITY_ERROR_RESPOND;
  } else{
    Data16 &= ~(EFI_PCI_COMMAND_PARITY_ERROR_RESPOND);
  }
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciConfigPerr Data16 = 0x%x\n",Data16));
  MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_COMMAND_OFFSET, MmInfo), Data16);
  Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_COMMAND_OFFSET, MmInfo));
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciConfigPerr IsPcieDevice PCI_COMMAND_OFFSET = 0x%x\n",Data16));

}

/**
  This function check the PCIE Error Filter List, and apply the PCIE Error Filter Setting

  @oaram[in] Socket         Device's socket number.
  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] *CorrErrMask   Pcie AER Corretable Error Mask
  @param[in] *UnCorrErrMask Pcie AER UnCorretable Error Mask
  @param[in] *UnCorrErrSev  Pcie AER Severity
  @param[in] *DevCtl        Pcie Device Control
  @param[in] *AeccConf      Advanced Error Capabilities and Control
  @param[in] MmInfo         Memory Map information of target device.

  @retval None
--*/
VOID
ApplyPcieErrorFilter (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN UINT32           *CorrErrMask,
  IN UINT32           *UnCorrErrMask,
  IN UINT32           *UnCorrErrSev,
  IN UINT16           *DevCtl,
  IN UINT32           *AeccConf,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  RAS_MAIL_BOX          *MailBox = NULL;
  UINT32                VidDid;
  UINT16                Vid;
  UINT16                Did;
  UINT16                i;
  PCIE_ERROR_FILTER     *PcieErrorFilter;

  MailBox = GetRasMailBox ();

  if (MailBox != NULL) {
    if (MailBox->PcieInitPar.PcieErrFilterCount != 0) {
      VidDid = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, 0, MmInfo));
      Vid = (UINT16) VidDid;
      Did = (UINT16) (VidDid >> 16);
      PcieErrorFilter = (PCIE_ERROR_FILTER*) MailBox->PcieInitPar.PcieErrFilterTable;
      for (i = 0; i < MailBox->PcieInitPar.PcieErrFilterCount; i++) {
        if ((PcieErrorFilter->Vid == Vid) &&
             ((Did >= PcieErrorFilter->DidMin) && (Did <= PcieErrorFilter->DidMax))) {
          *CorrErrMask |= PcieErrorFilter->AerCorrMskOr;
          *CorrErrMask &= PcieErrorFilter->AerCorrMskAnd;
          *UnCorrErrMask |= PcieErrorFilter->AerUnCorrMskOr;
          *UnCorrErrMask &= PcieErrorFilter->AerUnCorrMskAnd;
          *UnCorrErrSev |= PcieErrorFilter->AerErrSevOr;
          *UnCorrErrSev &= PcieErrorFilter->AerErrSevAnd;
          *DevCtl |= PcieErrorFilter->DevCtlOr;
          *DevCtl &= PcieErrorFilter->DevCtlAnd;
          *AeccConf |= PcieErrorFilter->AeccConfOr;
          *AeccConf &= PcieErrorFilter->AeccConfAnd;
          //RAS_DEBUG((LEVEL_FUNC_FLOW,  "Applied Error Filter for PCIE Dev %x:%x:%x:%x\n", Socket, Bus, Device, Function));
          return;
        }
        PcieErrorFilter++;
      }
    }
  }
}

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
  )
{
  UINT32   CorrErrMask;
  UINT32   UnCorrErrMask;
  UINT32   ErrorSev;
  UINT16   DevCtl = 0;
  UINT32   AeccConf = 0;
  RAS_MAIL_BOX  *MailBox = NULL;

  MailBox = GetRasMailBox ();

  CorrErrMask = PCIEAER_CORRERR_MSK;
  UnCorrErrMask = PCIEAER_UNCORRERR_MSK;
  ErrorSev = 0;

  if (MailBox->PcieInitPar.PcieAerCorrErrEn == 1) {
    CorrErrMask &= ~MailBox->PcieInitPar.PcieAerCorrErrBitMap;
  }

  if (MailBox->PcieInitPar.PcieAerNfatErrEn == 1) {
    UnCorrErrMask &= ~MailBox->PcieInitPar.PcieAerNfatErrBitMap;
  }

  if (MailBox->PcieInitPar.PcieAerFatErrEn == 1) {
    UnCorrErrMask &= ~MailBox->PcieInitPar.PcieAerFatErrBitMap;
    ErrorSev = MailBox->PcieInitPar.PcieAerFatErrBitMap & PCIEAER_UNCORRERR_MSK;
  }

  if (MailBox->PcieInitPar.PcieAerEcrcEn) {
    UnCorrErrMask &= ~BIT19;
    //
    // WA b307207: ECRC error needs to be treated as fatal.
    //
    if (IsSiliconWorkaroundEnabled ("S2204311532")) {
      ErrorSev |= BIT19;
    }
    AeccConf |= (B_PCIE_AER_AECC_ECRC_CHK_EN | B_PCIE_AER_AECC_ECRC_GEN_EN);
  }

  if (MailBox->PcieInitPar.PcieCorrErrEn == 1) {
    DevCtl |= B_PCIE_ROOT_CONTROL_COR_ERR_EN;
  }
  if (MailBox->PcieInitPar.PcieUncorrErrEn == 1) {
    DevCtl |= B_PCIE_ROOT_CONTROL_NF_ERR_EN;
  }
  if (MailBox->PcieInitPar.PcieFatalErrEn == 1) {
    DevCtl |= B_PCIE_ROOT_CONTROL_FAT_ERR_EN;
  }

  if (PcieIsDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
    //
    // Check if this is a PCIe device...
    //
    if (IsPcieDevice (Socket, Bus, Device, Function, MmInfo)) {
      //
      // Check PCIe Error Filter List, in case individual device have special setting
      //
      ApplyPcieErrorFilter (Socket, Bus, Device, Function, &CorrErrMask, &UnCorrErrMask, &ErrorSev, &DevCtl, &AeccConf, MmInfo);

      //
      // 1. config PCIe AER cap
      //
      PcieAerConfig (Socket, Bus, Device, Function, MmInfo, CorrErrMask, UnCorrErrMask, ErrorSev, AeccConf);

      //
      // 2. config PCIe DPC cap
      //
      if (IsPcieEdpcSupported(Socket, Bus, Device, Function, MmInfo)) {
        PcieEdpcConfig (Socket, Bus, Device, Function, MmInfo, MailBox->PcieInitPar.EdpcEn, MailBox->PcieInitPar.EdpcInterrupt, MailBox->PcieInitPar.EdpcErrCorMsg);
      }

      //
      // 3. config PCIe error loging for correctable, uncorrectable, and fatal
      //
      PcieConfigErrCtrl (Socket, Bus, Device, Function, MmInfo, DevCtl);
      PciClearLegacyDeviceStatus (Socket, Bus, Device, Function, MmInfo);
      PciConfigSerr (Socket, Bus, Device, Function, MmInfo, MailBox->PcieInitPar.SerrPropEn);
      PciConfigPerr (Socket, Bus, Device, Function, MmInfo, MailBox->PcieInitPar.PerrPropEn);
    } else {
      //
      // Program SERR and PERR for PCI devices
      //
      PciClearLegacyDeviceStatus (Socket, Bus, Device, Function, MmInfo);
      PciConfigSerr (Socket, Bus, Device, Function, MmInfo, MailBox->PcieInitPar.SerrPropEn);
      PciConfigPerr (Socket, Bus, Device, Function, MmInfo, MailBox->PcieInitPar.PerrPropEn);
    }

    if (PcieIsPciBridgeDevice (Socket, Bus, Device, Function, MmInfo)) {
      PciEnableBridgeControl (Socket, Bus, Device, Function, MmInfo);
    }
  }
}

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
  )
{
  UINT8           SecondaryBusNumber;
  UINT8           SubordinateBusNumber;
  UINT16          BusNumber;
  UINT8           DeviceNumber;
  UINT8           FunctionNumber;
  UINT8           FuncExist;
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieConfigDeviceAndSubDevices Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));

  //
  // Enable error logging in the current device...
  //
  PcieConfigDevice (Socket, Bus,Device,Function, MmInfo);

  //
  // Enable error logging in the devices behind a bridge...
  //

  if (PcieIsPciBridgeDevice (Socket, Bus, Device, Function, MmInfo)) {
    SecondaryBusNumber = MmioRead8 (
                            MmPciAddressExt (Socket, Bus,
                              Device,
                              Function,
                              PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET,
                              MmInfo
                              ));

    SubordinateBusNumber = MmioRead8 (
                             MmPciAddressExt (Socket, Bus,
                               Device,
                               Function,
                               PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET,
                               MmInfo
                               ));
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieConfigDeviceAndSubDevices PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET = 0x%x\n",SecondaryBusNumber));
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieConfigDeviceAndSubDevices PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET = 0x%x\n",SubordinateBusNumber));

    if (SecondaryBusNumber && SubordinateBusNumber) {
      for (BusNumber = SecondaryBusNumber; BusNumber <= SubordinateBusNumber; BusNumber++) {
        for (DeviceNumber = 0; DeviceNumber <= PCI_MAX_DEVICE; DeviceNumber++) {
          FuncExist = 0;
          for (FunctionNumber = 0; FunctionNumber <= PCI_MAX_FUNC; FunctionNumber++) {
            if (PcieIsDevicePresent (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo)) {
              PcieConfigDevice (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo);
              FuncExist |= (BIT0 << FunctionNumber);
              //
              // If it is non nulti-function device, break function loop
              //
              if (FunctionNumber == 0 && !IsMultiFunctionDevice (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo)) {
                break;
              }
            } else {
              if (FunctionNumber == 0) {
                break;
              } else {
                continue;
              }
            }
          }//Func
          //
          // During PCI probing, UR error will be logged in exist functions, need to clear
          // them before going forward.
          //
          for (FunctionNumber = 0; FunctionNumber <= PCI_MAX_FUNC; FunctionNumber++) {
            if (FuncExist & (BIT0 << FunctionNumber)) {
              if (PcieIsDevicePresent (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo)) {
                PcieSuppressUnsupportReqest(Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo, 0, COR_ERROR_TYPE);
              }
            }
          }
        }//Device
      }//bus
      //
      // During PCI probing, UR error will be logged in exist functions, need to clear
      // them before going forward.
      //
      PcieSuppressUnsupportReqest(Socket, Bus, Device, Function, MmInfo, 0, COR_ERROR_TYPE);
    }
  }
}

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
  )
{
  UINT8           NextBusNumber;
  UINT8           LastBusNumber;
  UINT16          BusNumber;
  UINT8           DeviceNumber;
  UINT8           FunctionNumber;
  UINT8           FuncExist;
  UINT32          Abm;
  UINT32          DeviceMap;

  //
  // Enable error logging in the RCEC device...
  //
  PcieConfigDevice (Socket, Bus, Device, Function, MmInfo);

  //
  // Enable error logging in the RCiEP devices Associated with this RCEC...
  //

  if (IsPcieRcecSupported (Socket, Bus, Device, Function, MmInfo)) {

    GetRcecNbnLbnAbm (Socket, Bus, Device, Function, MmInfo, &NextBusNumber, &LastBusNumber, &Abm);
    if (NextBusNumber == 0xff && LastBusNumber == 0) {
      //
      // In this case, all the RCiEP devices on the same bus number of RCEC
      //
      NextBusNumber = Bus;
      LastBusNumber = Bus;
    }

    //
    // Start from the bus number of RCEC, till last bus number
    //
    for (BusNumber = Bus; BusNumber <= LastBusNumber; BusNumber++) {
      //
      // Need to check ABM (Associated Bit Map) for the RCiEP on the same bus of RCEC
      //
      if (BusNumber == Bus) {
        DeviceMap = Abm;
      } else {
        DeviceMap = 0xFFFFFFFF;
      }
      //
      // Loop devices
      //
      for (DeviceNumber = 0; DeviceNumber <= PCI_MAX_DEVICE; DeviceNumber++) {
        if ((DeviceMap & (1 << DeviceNumber)) == 0) {
          continue;
        }
        FuncExist = 0;
        for (FunctionNumber = 0; FunctionNumber <= PCI_MAX_FUNC; FunctionNumber++) {
          if (PcieIsDevicePresent (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo)) {
            PcieConfigDevice (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo);
            FuncExist |= (BIT0 << FunctionNumber);
            //
            // If it is non nulti-function device, break function loop
            //
            if (FunctionNumber == 0 && !IsMultiFunctionDevice (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo)) {
              break;
            }
          } else {
            if (FunctionNumber == 0) {
              break;
            } else {
              continue;
            }
          }
        }//Func
        //
        // During PCI probing, UR error will be logged in exist functions, need to clear
        // them before going forward.
        //
        for (FunctionNumber = 0; FunctionNumber <= PCI_MAX_FUNC; FunctionNumber++) {
          if (FuncExist & (BIT0 << FunctionNumber)) {
            if (PcieIsDevicePresent (Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo)) {
              PcieSuppressUnsupportReqest(Socket, (UINT8) BusNumber, DeviceNumber, FunctionNumber, MmInfo, 0, COR_ERROR_TYPE);
            }
          }
        }
      }//Device
    }//bus
    //
    // During PCI probing, UR error will be logged in exist functions, need to clear
    // them before going forward.
    //
    PcieSuppressUnsupportReqest(Socket, Bus, Device, Function, MmInfo, 0, COR_ERROR_TYPE);
  }

}

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
  )
{
  UINT8  SecondaryBusNumber;

  // Do not enable error logging if there are no Devices behind the bridge.
  SecondaryBusNumber = MmioRead8 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, MmInfo));
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetSecondaryBus PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET = 0x%x\n",SecondaryBusNumber));

  return SecondaryBusNumber;
}

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
  )
{
  UINT8   Device;
  UINT8   Function;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PciDeviceOnBus Socket = 0x%x,Bus = 0x%x\n",Socket,Bus));

  for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
    for (Function = 0; Function <= PCI_MAX_FUNC; Function++) {
      if (PcieIsDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

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
  )
{
  UINT16        CapabilitiesOffset;

    // Get the capability offset for LER
  CapabilitiesOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, PCIE_EXT_CAP_HEADER_LERC, PCIE_EXT_CAP_HEADER_LER_VSECID);
  RAS_DEBUG((LEVEL_FUNC_FLOW,"IioLerEnable CapabilitiesOffset = 0x%x! \n",CapabilitiesOffset));
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }
  return TRUE;
}
