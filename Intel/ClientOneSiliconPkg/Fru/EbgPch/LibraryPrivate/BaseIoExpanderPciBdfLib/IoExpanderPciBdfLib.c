/** @file
  I/O Expander PCIe Bus Device Function Library.
  All functions from this library are available in PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Register/PchRegs.h>
#include <PchBdfAssignment.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchIoExpanderBdfLib.h>
#include <Library/PchIoExpander.h>

/**
  Returns PCH LPC device PCI base address.

  @param[in]  IoExpanderId         I/O Expander Id (0 ... n - I/O Expander PCH)

  @retval                   PCH LPC PCI base address.
**/
UINT64
LpcPciCfgBaseByIoExpanderId (
  IN  UINT32    IoExpanderId
  )
{
  EFI_STATUS Status;
  UINT8      Segment;
  UINT8      Bus;

  Status = PchGetSegmentByIoExpanderId (IoExpanderId, &Segment);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  Status = PchGetBusByIoExpanderId (IoExpanderId, &Bus);
  if (EFI_ERROR (Status)) {
    return 0;
  }

  return PCI_SEGMENT_LIB_ADDRESS (
           Segment,
           Bus,
           LpcDevNumber (),
           LpcFuncNumber (),
           0
           );
}

/**
  Get GbE controller address that can be passed to the PCI Segment Library functions.

  @param[in]  IoExpanderId         I/O Expander Id (0 ... n - I/O Expander PCH)

  @retval GbE controller address in PCI Segment Library representation
**/
UINT64
GbePciCfgBaseByIoExpanderId (
  IN  UINT32    IoExpanderId
  )
{
  EFI_STATUS Status;
  UINT8      Segment;
  UINT8      Bus;

  Status = PchGetSegmentByIoExpanderId (IoExpanderId, &Segment);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  Status = PchGetBusByIoExpanderId (IoExpanderId, &Bus);
  if (EFI_ERROR (Status)) {
    return 0;
  }

  return PCI_SEGMENT_LIB_ADDRESS (
            Segment,
            Bus,
            GbeDevNumber (),
            GbeFuncNumber (),
            0
            );
}

/**
  Get P2SB controller address that can be passed to the PCI Segment Library functions.

  @param[in]  IoExpanderId         I/O Expander Id (0 ... n - I/O Expander PCH)

  @retval P2SB controller address in PCI Segment Library representation
**/
UINT64
P2sbPciCfgBaseByIoExpanderId (
  IN  UINT32    IoExpanderId
  )
{
  EFI_STATUS Status;
  UINT8      Segment;
  UINT8      Bus;

  Status = PchGetSegmentByIoExpanderId (IoExpanderId, &Segment);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  Status = PchGetBusByIoExpanderId (IoExpanderId, &Bus);
  if (EFI_ERROR (Status)) {
    return 0;
  }

  return PCI_SEGMENT_LIB_ADDRESS (
      Segment,
      Bus,
      P2sbDevNumber (),
      P2sbFuncNumber (),
      0
      );
}

/**
  Get PMC controller address that can be passed to the PCI Segment Library functions.

  @param[in]  IoExpanderId         I/O Expander Id (0 ... n - I/O Expander PCH)

  @retval PMC controller address in PCI Segment Library representation
**/
UINT64
PmcPciCfgBaseByIoExpanderId (
  IN  UINT32    IoExpanderId
  )
{
  EFI_STATUS Status;
  UINT8      Segment;
  UINT8      Bus;

  Status = PchGetSegmentByIoExpanderId (IoExpanderId, &Segment);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  Status = PchGetBusByIoExpanderId (IoExpanderId, &Bus);
  if (EFI_ERROR (Status)) {
    return 0;
  }

  return PCI_SEGMENT_LIB_ADDRESS (
      Segment,
      Bus,
      PmcDevNumber (),
      PmcFuncNumber (),
      0
      );
}

/**
  Returns PCH SPI PCI Config Space base address

  @param[in]  IoExpanderId         I/O Expander Id (0 ... n - I/O Expander PCH)

  @retval  UINT64  PCH SPI Config Space base address
**/
UINT64
SpiPciCfgBaseByIoExpanderId (
  IN  UINT32    IoExpanderId
  )
{
  EFI_STATUS Status;
  UINT8      Segment;
  UINT8      Bus;

  Status = PchGetSegmentByIoExpanderId (IoExpanderId, &Segment);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  Status = PchGetBusByIoExpanderId (IoExpanderId, &Bus);
  if (EFI_ERROR (Status)) {
    return 0;
  }

  return PCI_SEGMENT_LIB_ADDRESS (
      Segment,
      Bus,
      SpiDevNumber (),
      SpiDevNumber (),
      0
      );
}

/**
  Get XHCI controller address that can be passed to the PCI Segment Library functions.

  @param[in]  IoExpanderId         I/O Expander Id (0 ... n - I/O Expander PCH)

  @retval XHCI controller address in PCI Segment Library representation
**/
UINT64
PchXhciPciCfgBaseByIoExpanderId (
  IN  UINT32    IoExpanderId
  )
{
  EFI_STATUS Status;
  UINT8      Segment;
  UINT8      Bus;

  Status = PchGetSegmentByIoExpanderId (IoExpanderId, &Segment);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  Status = PchGetBusByIoExpanderId (IoExpanderId, &Bus);
  if (EFI_ERROR (Status)) {
    return 0;
  }

  return PCI_SEGMENT_LIB_ADDRESS (
      Segment,
      Bus,
      PchXhciDevNumber (),
      PchXhciFuncNumber (),
      0
      );
}

/**
  Get SATA controller address that can be passed to the PCI Segment Library functions.

  @param[in]  IoExpanderId        I/O Expander Id (0 ... n - I/O Expander PCH)
  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller address in PCI Segment Library representation
**/
UINT64
SataPciCfgBaseByIoExpanderId (
  IN UINT32  IoExpanderId,
  IN UINT32  SataCtrlIndex
  )
{
  EFI_STATUS Status;
  UINT8      Segment;
  UINT8      Bus;

  Status = PchGetSegmentByIoExpanderId (IoExpanderId, &Segment);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  Status = PchGetBusByIoExpanderId (IoExpanderId, &Bus);
  if (EFI_ERROR (Status)) {
    return 0;
  }

  return PCI_SEGMENT_LIB_ADDRESS (
      Segment,
      Bus,
      SataDevNumber (SataCtrlIndex),
      SataFuncNumber (SataCtrlIndex),
      0
      );
}

/**
  Get PCH PCIe controller address that can be passed to the PCI Segment Library functions.

  @param[in]  IoExpanderId  I/O Expander Id (0 ... n - I/O Expander PCH)
  @param[in]  RpIndex       PCH PCIe Root Port physical number. (0-based)

  @retval PCH PCIe controller address in PCI Segment Library representation
**/
UINT64
PchPcieRpPciCfgBaseByIoExpanderId (
  IN  UINT32   IoExpanderId,
  IN  UINT32   RpIndex
  )
{
  EFI_STATUS Status;
  UINT8      Segment;
  UINT8      Bus;

  Status = PchGetSegmentByIoExpanderId (IoExpanderId, &Segment);
  if (EFI_ERROR (Status)) {
    return 0;
  }
  Status = PchGetBusByIoExpanderId (IoExpanderId, &Bus);
  if (EFI_ERROR (Status)) {
    return 0;
  }

  return PCI_SEGMENT_LIB_ADDRESS (
      Segment,
      Bus,
      PchPcieRpDevNumber (RpIndex),
      PchPcieRpFuncNumber (RpIndex),
      0
      );
}
