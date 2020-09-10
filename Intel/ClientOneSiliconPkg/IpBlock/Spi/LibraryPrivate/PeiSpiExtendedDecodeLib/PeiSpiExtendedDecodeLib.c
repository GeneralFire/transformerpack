/** @file
  PCH SPI Extended BIOS Direct Read range library

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

#include <Register/SpiRegs.h>
#include <Library/PciSegmentLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiPchDmiLib.h>
#include <SpiConfig.h>
#include <Library/PchPciBdfLib.h>
#include <IndustryStandard/Pci30.h>

/**
  Enables Extended BIOS Range decoding in SPI controller by programming SPI
  BAR1 and enable bits

  @param[in]  RangeBase  Base of Source Decode Range, bits[24:0] of range are ignored

  @retval   EFI_SUCCESS               Range was programmed and enabled
  @retval   EFI_INVALID_PARAMETER     One of arguments is incorrect
**/
EFI_STATUS
SpiEnableExtendedBiosRangeSupport (
  UINT32    RangeBase
  )
{
  UINT64  PchSpiPciBaseAddress;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  PchSpiPciBaseAddress = SpiPciCfgBase ();

  //
  // Check for SPI controller presence before programming
  //
  if (PciSegmentRead16 (PchSpiPciBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return EFI_UNSUPPORTED;
  }

  //
  // Disable MSE
  //
  PciSegmentAnd16 (
    PchSpiPciBaseAddress + PCI_COMMAND_OFFSET,
    (UINT16)~(BIT1)
    );

  //
  // Program BAR1 with decode range base address
  //
  PciSegmentWrite32 (
    PchSpiPciBaseAddress + R_SPI_CFG_BAR1,
    (RangeBase & B_SPI_CFG_BAR1_MEMBAR)
    );

  //
  // Program EXT_BIOS_LIMIT_OFFSET to 16MB
  //
  PciSegmentAndThenOr32 (
    PchSpiPciBaseAddress + R_SPI_CFG_BC,
    (UINT32)~(B_SPI_CFG_BC_EXT_BIOS_LIMIT_OFFSET << N_SPI_CFG_BC_EXT_BIOS_LIMIT_OFFSET),
    (UINT32) (1 << 24)
    );

  //
  // Program EXT_BIOS_EN
  //
  PciSegmentOr32 (
    PchSpiPciBaseAddress + R_SPI_CFG_BC,
    (UINT32) (B_SPI_CFG_BC_EXT_BIOS_EN)
    );

  //
  // Enable MSE
  //
  PciSegmentOr16 (
    PchSpiPciBaseAddress + PCI_COMMAND_OFFSET,
    (UINT16) BIT1
    );

  return EFI_SUCCESS;
}

/**
  Performs PCH SPI Controller initialization

  @param[in] SiPolicy     The Silicon Policy PPI instance
**/
VOID
PchSpiExtendedBiosDecodeRangeInit (
  IN  SI_POLICY_PPI               *SiPolicy
  )
{
  SPI_CONFIG      *SpiConfig;
  EFI_STATUS      Status;
  UINT16          DecodeTarget;

  if (!IsExtendedBiosRangeDecodeSupported ()) {
    DEBUG ((DEBUG_INFO, "%a - Extended BIOS Decode Range is not supported.\n", __FUNCTION__));
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPolicy, &gSpiConfigGuid, (VOID *) &SpiConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a - Cannot find SPI config block, Status %r\n",
      __FUNCTION__, Status
      ));
    return;
  }

  if (SpiConfig->ExtendedBiosDecodeRangeEnable == FALSE) {
    DEBUG ((
      DEBUG_INFO,
      "%a - Extended BIOS decode range is disabled. Skipping initialization.\n",
       __FUNCTION__
       ));
    return;
  }

  //
  // Configure PCH SPI extended BIOS range decoding
  //
  Status = SpiEnableExtendedBiosRangeSupport (SpiConfig->ExtendedBiosDecodeRangeBase);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a - Error while configuring Extended BIOS range in SPI. Status = %r\n", __FUNCTION__, Status
      ));
    return;
  }

  //
  // Configure DMI Source Decode to make Extended BIOS region direct read access
  //
  DecodeTarget = GetPchSpiDmiTarget ();
  Status = PchDmiSetGeneralPurposeMemoryRange (0,
             SpiConfig->ExtendedBiosDecodeRangeBase,
             SpiConfig->ExtendedBiosDecodeRangeLimit,
             DecodeTarget
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a - Error while configuring Extended BIOS range in DMI. Status = %r\n", __FUNCTION__, Status
      ));
  }
}