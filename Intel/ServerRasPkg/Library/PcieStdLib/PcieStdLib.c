/** @file
  Implementation of Pcie Aer Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/PciExpress31.h>
#include <Library/PcieStdLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/RasDebugLib.h>
#include <Library/BaseLib.h>
#include <IndustryStandard/Pci.h>
#include "Register/PcieRegDef.h"
#include <Guid/Cper.h>
#include <Library/IoLib.h>


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
  )
{
  UINT32    Data32;
  //
  // Check if a device is present at this location...
  Data32= MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_VENDOR_ID_OFFSET, MmInfo));

  if (Data32 == 0xffffffff) {
    return FALSE;
  }
  return TRUE;
}

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
  )
{
  UINT8   CapabilitiesOffset;

  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "IsPcieDevice EFI_PCI_CAPABILITY_ID_PCIEXP = 0x%x!\n",CapabilitiesOffset));
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }
  return TRUE;
}

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
  )
{
  UINT8     PciHeaderType;

  PciHeaderType = MmioRead8 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_HEADER_TYPE_OFFSET, MmInfo));

  if ((PciHeaderType == 0xFF) || ((PciHeaderType & HEADER_TYPE_MULTI_FUNCTION) == 0)) {
    return FALSE;
  }
  return TRUE;
}

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
  )
{
  BOOLEAN   BridgeFlag;
  UINT8     PciHeaderType;

  BridgeFlag = FALSE;

  PciHeaderType = MmioRead8 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_HEADER_TYPE_OFFSET, MmInfo));

  if ((PciHeaderType & HEADER_LAYOUT_CODE) == 1) {
    BridgeFlag = TRUE;
  }

  return BridgeFlag;
}

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
  )
{
  UINT8     CapabilitiesOffset;
  UINT16    PcieCapabilities;

  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }

  PcieCapabilities = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_CAPABILITY_OFFSET, MmInfo));
  if ((PcieCapabilities & 0xf0) == V_PCIE_CAPABILITY_DPT_ROOT_PORT) {
    return TRUE;
  } else {
    return FALSE;
  }

}

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
  )
{
  UINT8     CapabilitiesOffset;
  UINT16    PcieCapabilities;

  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }

  PcieCapabilities = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_CAPABILITY_OFFSET, MmInfo));
  if ((PcieCapabilities & 0xf0) == V_PCIE_CAPABILITY_DPT_DOWNSTREAM_PORT) {
    return TRUE;
  } else {
    return FALSE;
  }

}

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
  )
{
  UINT32        UncErrSts;

  UncErrSts = PcieAerGetUncErrSts (Socket, Bus, Device, Function, MmInfo);
  if (UncErrSts & B_PCIE_AER_UES_SUPRISE_LINK_DOWN_ERR) {
    return TRUE;
  } else {
    return FALSE;
  }
}

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
  )
{

  if (!PcieIsStdDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
    return FALSE;
  }
  if (!PcieIsPciBridgeDevice (Socket, Bus, Device, Function, MmInfo)) {
    return FALSE;
  }

  *SecondaryBus = MmioRead8 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, MmInfo));
  *SubordinateBus = MmioRead8 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, MmInfo));
  return TRUE;
}

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
  )
{
  UINT8   PciPrimaryStatus;
  UINT8   CapabilityOffset;
  UINT8   CapId;
  UINT8   *CapPointer;
  UINT8   i;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetCapabilities Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x\n",Socket,Bus,Device,Function));

  CapPointer = CapBuf;

  PciPrimaryStatus = MmioRead8 (MmPciAddressExt (Socket,Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET, MmInfo));
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetCapabilities PCI_PRIMARY_STATUS_OFFSET = 0x%x!\n",PciPrimaryStatus));
  if (PciPrimaryStatus & EFI_PCI_STATUS_CAPABILITY) {
    CapabilityOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetCapabilities EFI_PCI_CAPABILITY_ID_PCIEXP = 0x%x!\n",CapabilityOffset));
    if (CapabilityOffset == 0) {
      return EFI_UNSUPPORTED;
    }

    for (i = 0; i < 60; i++) {
      CapId = MmioRead8 (MmPciAddressExt (Socket,Bus, Device, Function, CapabilityOffset + i, MmInfo));
      *CapPointer++ = CapId;
    }
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}

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
  )
{
  UINT16   PciPrimaryStatus;
  UINT8    CapabilityOffset = 0;
  UINT16   data16;

  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetCapabilitiesOffset Socket = 0x%x,Bus = 0x%x,Device = 0x%x,Function = 0x%x,Cap = 0x%x \n",Socket,Bus,Device,Function,Cap));

  PciPrimaryStatus = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET, MmInfo));
  //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetCapabilitiesOffset PCI_PRIMARY_STATUS_OFFSET = 0x%x\n",PciPrimaryStatus));
  if (PciPrimaryStatus & EFI_PCI_STATUS_CAPABILITY) {
    CapabilityOffset = MmioRead8 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_CAPBILITY_POINTER_OFFSET, MmInfo));
    //RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieGetCapabilitiesOffset EFI_PCI_CAPABILITY_PTR = 0x%x\n",CapabilityOffset));
    while(TRUE) { // maximum possible number of items in list
      // format check, normal end of list is next offset = 0
      //   PCI 3.0 section 6.7 Capability List
      // "Each capability must be DWORD aligned. The bottom two bits of all pointers
      // (including the initial pointer at 34h) are reserved and must be implemented as 00b
      // although software must mask them to allow for future uses of these bits."

      if ((CapabilityOffset & DWORD_MSK) || (CapabilityOffset < CAP_OFFSET_BASE)) { // dword alignment; lower limit
        CapabilityOffset = 0;  // bad format or normal end of list, set not found
        break;          // terminate search
      }
      // CapOffset &= ~DWORD_MSK;  // not needed if b[1::0] must = 0
      // offset is good, get capabilities ID and next offset
      data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilityOffset, MmInfo));
      if ((UINT8) (data16 & 0xff) == Cap) {
        break;  // requested ID found
      }

      // next item
      CapabilityOffset = (UINT8)(data16 >> 8);   // b[15::8] = next ptr

    }
  }
  return CapabilityOffset;
}

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
  )
{
  UINT8                    CapabilitiesOffset;
  PCI_REG_PCIE_CAPABILITY  PcieCapabilities;

  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (CapabilitiesOffset == 0) {
    return 0xff;
  }

  PcieCapabilities.Uint16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_CAPABILITY_OFFSET, MmInfo));

  return PcieCapabilities.Bits.DevicePortType;
}

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
  )
{
  UINT16             ExCapOffset = 0;
  UINT32             data32;
  BOOLEAN            SearchingDmiAer = FALSE;

  if (PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP)) {
    ExCapOffset = ((MmInfo != NULL) && (MmInfo->PciExtMmType == PCI_EXT_MM_TYPE_RCRB)) ? 0 : EFI_PCIE_CAPABILITY_BASE_OFFSET;
    while (TRUE) {  // maximum possible number of items
      // format check, normal end of list is 0
      //   PCIe 3.1 section 7.9.3 PCI Express Extended Capability Header
      // "For Extended Capabilities implemented in Configuration Space,
      // this offset is relative to the beginning of PCI compatible Configuration Space
      // and thus must always be either 000h (for terminating list of Capabilities)
      // or greater than 0FFh.
      // The bottom 2 bits of this offset are Reserved and must be implemented as 00b
      // although software must mask them to allow for future uses of these bits."
      if ((ExCapOffset & DWORD_MSK) || (ExCapOffset < EFI_PCIE_CAPABILITY_BASE_OFFSET)) {
        // dword alignment; lower limit
        if (ExtCapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID) {
          ExCapOffset = 0;  // if bad format or normal end of list, set not found
          break;            //  terminate search
        } else {
          //
          // In case for AER, DMI AER is a vendor specific capability with VSECID 04, we need to
          // search again for this case.
          //
          ExCapOffset = EFI_PCIE_CAPABILITY_BASE_OFFSET;
          ExtCapabilityId = PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID;
          VendorSpecificId = 4;
          SearchingDmiAer = TRUE;
        }
      }

      // offset is good, get capabilities ID and next offset
      // capabilities ID check
      data32 = MmioRead32(MmPciAddressExt(Socket, Bus, Device, Function, ExCapOffset, MmInfo));   // get ID
      if ((data32 & EXCAPABILITIES_MSK) == ExtCapabilityId) {
        if (ExtCapabilityId != PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID) {
          break;  // requested Extended ID found
        } else {  // check VSEC ID
          if (MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, ExCapOffset + 4, MmInfo)) == VendorSpecificId) {
            //
            // if it is DMI AER capability, need to return vendor specific header offset
            //
            if (SearchingDmiAer) {
              ExCapOffset += 4;
            }
            break;  // requested VSCE ID found
          }
        }
      }
      // next item
      ExCapOffset = (UINT16)(data32 >> 20);  // b[31::20] = next ptr

    }
  }
  return ExCapOffset;
}

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
  )
{
  UINT8                         CapabilitiesOffset;
  PCI_REG_PCIE_SLOT_CAPABILITY  PcieSlotCapabilities;

  if (!PcieIsStdDevicePresent (Socket, Bus, Device, Function, MmInfo)) {
    return;
  }
  if (!IsPcieRootPort (Socket, Bus, Device, Function, MmInfo)) {
    return;
  }

  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (CapabilitiesOffset == 0) {
    return;
  }

  PcieSlotCapabilities.Uint32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_SLOT_CAPABILITY_OFFSET, MmInfo));

  if (PcieSlotCapabilities.Bits.HotPlugSurprise) {
    PcieSlotCapabilities.Bits.HotPlugSurprise = 0;
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_SLOT_CAPABILITY_OFFSET, MmInfo), PcieSlotCapabilities.Uint32);
  }
}

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
  )
{
  UINT8     CapabilitiesOffset;

  CapabilitiesOffset = PcieGetCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (CapabilitiesOffset == 0) {
    return FALSE;
  }

  if (DevSts != NULL) {
    *DevSts = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_STATUS_OFFSET, MmInfo));
  }
  if (DevCtl != NULL) {
    *DevCtl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, CapabilitiesOffset + PCIE_DEVICE_CONTROL_OFFSET, MmInfo));
  }
  return TRUE;
}
