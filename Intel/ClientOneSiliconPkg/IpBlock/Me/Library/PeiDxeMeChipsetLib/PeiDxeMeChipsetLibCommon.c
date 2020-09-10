/** @file
  Me Chipset Lib implementation.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/MeRegs.h>
#include <Register/HeciRegs.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PsfLib.h>
#include <Register/PchRegs.h>
#include <PcieRegs.h>
#include <Library/PostCodeLib.h>


/**
  Checks if the given PCIe ME Device Function is HECI Device Function

  @param[in]     Function  ME Device Function

  @retval TRUE   This is a HECI Device Function
  @retval FALSE  This is not a HECI Device Function
**/
BOOLEAN
IsHeciDeviceFunction (
  IN ME_DEVICE Function
  )
{
  switch (Function) {
    case HECI1:
    case HECI2:
    case HECI3:
    case HECI4:
      return TRUE;

    default:
      return FALSE;
  }
}

/**
  Put ME device out of D0I3

  @param[in] Function  ME function where D0I3 is to be changed
**/
VOID
ClearD0I3Bit (
  IN  UINT32   Function
  )
{
  UINT64       DevicePciCfgBase;
  UINT32       DeviceBar[2];
  UINTN        *pBar;
  UINT8        Cmd;
  UINTN        Timeout;
  UINT32       D0I3Ctrl;

  DEBUG ((DEBUG_INFO, "[HECI%d] Clearing D0I3 bit\n", HECI_NAME_MAP (Function)));
  ///
  /// Get Device MMIO address
  ///
  DevicePciCfgBase = PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, Function, 0);
  if (PciSegmentRead16 (DevicePciCfgBase + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "[HECI%d] Function is disabled, cannot clear D0I3 bit!\n", HECI_NAME_MAP (Function)));
    return;
  }
  DeviceBar[0] = PciSegmentRead32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
  DeviceBar[1] = 0x0;
  if ((PciSegmentRead32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & 0x6) == 0x4) {
    DeviceBar[1] = PciSegmentRead32 (DevicePciCfgBase + (PCI_BASE_ADDRESSREG_OFFSET + 4));
  }

  ///
  /// Put CSME Device out of D0I3
  /// (1) Poll D0I3C[0] CIP bit is 0 with timeout 5 seconds
  /// (2) Write D0I3C[2] = 0
  /// (3) Poll D0I3C[0] CIP bit is 0 with timeout 5 seconds
  ///
  if (!(DeviceBar[0] == 0x0 && DeviceBar[1] == 0x0) && !(DeviceBar[0] == 0xFFFFFFF0 && DeviceBar[1] == 0xFFFFFFFF)) {
    Cmd = PciSegmentRead8 (DevicePciCfgBase + PCI_COMMAND_OFFSET);
    if ((Cmd & EFI_PCI_COMMAND_MEMORY_SPACE) != EFI_PCI_COMMAND_MEMORY_SPACE) {
      PciSegmentOr8 (DevicePciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
    }
    pBar = (UINTN*) DeviceBar;
    D0I3Ctrl = MmioRead32 (*pBar + D0I3C);
    if ((D0I3Ctrl & BIT2) == BIT2) {
      ///
      /// (1) If entering D0I3 is in progress wait till it finishes. Let's give it 5000 ms timeout.
      ///
      Timeout = 5000;
      while ((D0I3Ctrl & BIT0) == BIT0 && Timeout-- > 0) {
        MicroSecondDelay (1000);
        D0I3Ctrl = MmioRead32 (*pBar + D0I3C);
      }

      ///
      /// (2) Write D0I3C[2] = 0
      ///
      MmioWrite32 (*pBar + D0I3C, D0I3Ctrl & ~BIT2);

      D0I3Ctrl = MmioRead32 (*pBar + D0I3C);
      ///
      /// (3) If exiting D0I3 is in progress wait till it finishes. Let's give it 5000 ms timeout.
      ///
      Timeout = 5000;
      while ((D0I3Ctrl & BIT0) == BIT0 && Timeout-- > 0) {
        MicroSecondDelay (1000);
        D0I3Ctrl = MmioRead32 (*pBar + D0I3C);
      }
    }
    DEBUG ((DEBUG_INFO, "[HECI%d] D0I3C register = %08X\n", HECI_NAME_MAP (Function), MmioRead32 (*pBar + D0I3C)));
    PciSegmentWrite8 (DevicePciCfgBase + PCI_COMMAND_OFFSET, Cmd);
  }
}


/**
  Put ME device into D0I3

  @param[in] Function          Select of Me device
**/
VOID
SetD0I3Bit (
  IN  UINT32   Function
  )
{
  UINT64                          DevicePciCfgBase;
  UINT32                          DeviceBar[2];
  UINTN                           *Bar;

  DEBUG ((DEBUG_INFO, "[HECI%d] Setting D0I3 bit\n", HECI_NAME_MAP (Function)));
  ///
  /// Get Device MMIO address
  ///
  DevicePciCfgBase = PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, Function, 0);
  if (PciSegmentRead16 (DevicePciCfgBase + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "[HECI%d] Function is disabled, can't set D0I3 bit!\n", HECI_NAME_MAP (Function)));
    return;
  }
  if ((PciSegmentRead8 (DevicePciCfgBase + R_ME_HIDM) & B_ME_HIDM_MODE) != V_ME_HIDM_MSI) {
    DEBUG ((DEBUG_WARN, "[HECI%d] HIDM is not legacy/MSI, do not set D0I3 bit!\n", HECI_NAME_MAP (Function)));
    return;
  }
  DeviceBar[0] = PciSegmentRead32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
  DeviceBar[1] = 0x0;
  if ((PciSegmentRead32 (DevicePciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & 0x6) == 0x4) {
    DeviceBar[1] = PciSegmentRead32 (DevicePciCfgBase + (PCI_BASE_ADDRESSREG_OFFSET + 4));
  }

  ///
  /// Put CSME Device in D0I3
  ///
  PciSegmentOr8 (DevicePciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  if (!(DeviceBar[0] == 0x0 && DeviceBar[1] == 0x0) && !(DeviceBar[0] == 0xFFFFFFF0 && DeviceBar[1] == 0xFFFFFFFF)) {
    Bar = (UINTN*) DeviceBar;
    MmioOr32 (*Bar + D0I3C, BIT2);
    DEBUG ((DEBUG_INFO, "[HECI%d] D0I3C register = %08X\n", HECI_NAME_MAP (Function), MmioRead32 (*Bar + D0I3C)));
  }

  PciSegmentAnd8 (DevicePciCfgBase + PCI_COMMAND_OFFSET,(UINT8) ~(EFI_PCI_COMMAND_MEMORY_SPACE));
}
