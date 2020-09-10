/** @file
  Implementation of Pcie Aer Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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
  )
{
  //RAS_DEBUG((LEVEL_REG,"PcieRasLib IsPcieEdpcSupported."));

  if (PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
        MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_DOWNSTREAM_PORT_CONTAINMENT_ID, 0xff) != 0) {
    return TRUE;
  }

  return FALSE;
}

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
  )
{
  UINT16            EdpcExtCapOffset;
  UINT16            Data16;

  EdpcExtCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                       MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_DOWNSTREAM_PORT_CONTAINMENT_ID, 0xff);
  if (EdpcExtCapOffset == 0) {
    return;
  }

  Data16 = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, EdpcExtCapOffset + PCIE_DPC_OFFSET_REG_CTL, MmInfo));
  Data16 &= ~(B_PCIE_DPC_CTL_TRG_EN + B_PCIE_DPC_CTL_INT_EN + B_PCIE_DPC_CTL_COR_MSG_EN);
  Data16 |= (UINT16)EdpcTrigger;
  if (EdpcTrigger != 0) {
    if (EdpcInterrupt) {
      Data16 |= B_PCIE_DPC_CTL_INT_EN;
    }
    if (EdpcCorMsg) {
      Data16 |= B_PCIE_DPC_CTL_COR_MSG_EN;
    }
    PcieDisableSurpriseHotPlugCap (Socket, Bus, Device, Function, MmInfo);
  }
  MmioWrite16 (MmPciAddressExt (Socket, Bus, Device, Function, EdpcExtCapOffset + PCIE_DPC_OFFSET_REG_CTL, MmInfo), Data16);
  RAS_DEBUG((LEVEL_FUNC_FLOW, "PcieEdpcConfig Socket %x, Bus %x, Device %x, fun %x, DPC Control = 0x%x\n", Socket, Bus, Device, Function, Data16));
}

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
  )
{
  UINT8           PcieCapOffset;
  UINT16          DpcExtCapOffset;
  UINT16          DpcStatus;
  UINT16          DpcControl;
  UINT16          DeviceControl;
  BOOLEAN         DpcTriggered;

  DpcTriggered = FALSE;
  if (!IsPcieEdpcSupported(Socket, Bus, Device, Function, MmInfo)) {
    return FALSE;
  }

  //
  // Get the PCIE capabilities offset
  //
  PcieCapOffset = PcieGetCapabilitiesOffset (Socket,Bus, Device, Function, MmInfo, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (PcieCapOffset == 0) {
    return FALSE;
  }
  DpcExtCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_DOWNSTREAM_PORT_CONTAINMENT_ID, 0xff);
  if (DpcExtCapOffset == 0) {
    return FALSE;
  }

  //
  // Check if the DPC status indicates that DPC was triggered
  //
  DpcStatus = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, DpcExtCapOffset + PCIE_DPC_OFFSET_REG_STS, MmInfo));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieIsEdpcTiggered PCIE_DPC_OFFSET_REG_STS = 0x%x\n", DpcStatus));

  //
  // Check if ERR_COR was enabled in DPC Control register
  //
  DpcControl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, DpcExtCapOffset + PCIE_DPC_OFFSET_REG_CTL, MmInfo));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieIsEdpcTiggered PCIE_DPC_OFFSET_REG_CTL = 0x%x\n", DpcControl));

  //
  // check if correctable error was enabled in Device control register
  //
  DeviceControl = MmioRead16 (MmPciAddressExt (Socket, Bus, Device, Function, PcieCapOffset + PCIE_DEVICE_CONTROL_OFFSET, MmInfo));
  RAS_DEBUG ((LEVEL_FUNC_FLOW, "PcieIsEdpcTiggered PCIE_DEVICE_CONTROL = 0x%x\n", DeviceControl));

  if ((DpcStatus & B_PCIE_DPC_STS_DPC_TRIGGER_STS) && (DpcControl & B_PCIE_DPC_CTL_TRG_EN)
                     && (DpcControl & B_PCIE_DPC_CTL_COR_MSG_EN) && (DeviceControl & B_PCIE_DEVICE_CONTROL_COR_ERR_EN)) {
    DpcTriggered = TRUE;
  }

  return DpcTriggered;
}