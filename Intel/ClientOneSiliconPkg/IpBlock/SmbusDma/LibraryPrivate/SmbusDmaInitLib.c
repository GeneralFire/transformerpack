/** @file
  PCH Smbus Dma Driver

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <PiDxe.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/SmbusDmaRegs.h>
#include <PchReservedResources.h>
#include <Library/PeiItssLib.h>
#include <Library/BaseLib.h>
#include <Library/PchPciBdfLib.h>

/**
  Configure SMBus DMA

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI

  @retval EFI_SUCCESS             Driver initializes successfully

**/
EFI_STATUS
EFIAPI
SmbusDmaConfigure (
  IN  SI_POLICY_PPI           *SiPolicyPpi
  )
{

  UINT32                   SmbusHostMemBase;
  UINT32                   SmbusHostPcieBase;
  UINT8                    InterruptPin;

  DEBUG ((DEBUG_INFO, "InitializePchSmbusDma() Start\n"));

  SmbusHostPcieBase = (UINT32)SmbusDmaPciCfgBase ();

  if (PciSegmentRead16 (SmbusHostPcieBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "Smbus Dma is disabled.\n"));
    return EFI_SUCCESS;
  }
  InterruptPin = ItssGetDevIntPin (SiPolicyPpi, SmbusDmaDevNumber (), SmbusDmaFuncNumber ());
  PciSegmentWrite8 (SmbusHostPcieBase + PCI_INT_PIN_OFFSET, InterruptPin);

  //
  // Setup Temporary base address
  //
  PciSegmentWrite32 (SmbusHostPcieBase + PCI_BASE_ADDRESSREG_OFFSET, PCH_TEMP_BASE_ADDRESS);
  PciSegmentWrite32 (SmbusHostPcieBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
  SmbusHostMemBase = PciSegmentRead32 (SmbusHostPcieBase + PCI_BASE_ADDRESSREG_OFFSET) & B_SMBUS_HOST_SMTBAR_MBA;

  //
  // SMBus (Host) controller is not intended for SPD so BIOS must disable it using SPDDIS
  //
  MmioOr32 (SmbusHostMemBase + R_SMBUS_SMTBAR_MCTRL, B_SMBUS_SMTBAR_MCTRL_SPDDIS);

  //
  // Reset BAR and PCI command register to 0
  //
  PciSegmentWrite32 (SmbusHostPcieBase + PCI_BASE_ADDRESSREG_OFFSET, 0);
  PciSegmentAnd32 (SmbusHostPcieBase + PCI_COMMAND_OFFSET, ~(UINT32) (EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER));

  DEBUG ((DEBUG_INFO, "InitializePchSmbusDma() End\n"));

  return EFI_SUCCESS;
}
