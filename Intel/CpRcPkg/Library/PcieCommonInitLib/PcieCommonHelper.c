/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PciLib.h>
#include <Library/PciExpressLib.h>
#include <Library/DebugLib.h>
#include "Library/PcieCommonInitLib.h"
#include <Library/UsraAccessApi.h>

#include "PcieCommonHelper.h"


/******************************************************************************
 * Functions.
 ******************************************************************************/
/**
  Find the offset in PCI device config space of PCIe capabilities structure of given ID.
  Example CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability
  See "PCI Code and ID Assignment Specification" Table 2-1: Capability IDs available at
  pcisig.com for full list of PCIe capabilities.

  @param[in] PciAddr  PCI device address.
  @param[in] CapId    CAPID to search for.

  @return Capability structure offset is returned, or zero if CAPID not found.
**/
UINT8
PciFindCapId (
  IN USRA_ADDRESS   *PciAddrPtr,
  IN UINT8           CapId
  )
{
  struct {
    UINT16 Vid;
    UINT16 Did;
    UINT16 Cmd;
    UINT16 Sts;
  } PciHdr;
  USRA_ADDRESS       PciAddr;
  UINT8              CapHeaderOffset;
  UINT8              CapHeaderId;

  CapHeaderId = 0;

  USRA_ADDRESS_COPY (&PciAddr, PciAddrPtr);
  //
  // Read VID/DID and status register.
  //
  PciAddr.Attribute.AccessWidth = UsraWidth32;
  PciAddr.Attribute.AddrType = AddrTypePCIEBLK;
  PciAddr.PcieBlk.Count = sizeof(PciHdr) / sizeof(UINT32);
  PciAddr.Pcie.Offset = PCI_VENDOR_ID_OFFSET;
  RegisterRead (&PciAddr, &PciHdr);
  //
  // Check capabilities list enable
  //
  if (PciHdr.Vid == 0xFFFF || !(PciHdr.Sts & BIT4)) {
    return 0;
  }
  //
  // Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
  //
  PciAddr.Attribute.AccessWidth = UsraWidth8;
  PciAddr.Attribute.AddrType = AddrTypePCIE;
  PciAddr.Pcie.Offset = PCI_CAPBILITY_POINTER_OFFSET;
  RegisterRead (&PciAddr, &CapHeaderOffset);

  //
  // Keep looking for a pointer ID that match the flag
  // Read the Capabilities Pointer
  //
  while (CapHeaderOffset != 0 && CapHeaderId != 0xFF) {
    PciAddr.Pcie.Offset = CapHeaderOffset;
    RegisterRead (&PciAddr, &CapHeaderId);

    if (CapHeaderId == CapId) {
      return CapHeaderOffset;
    }
    PciAddr.Pcie.Offset = CapHeaderOffset + 1;
    RegisterRead (&PciAddr, &CapHeaderOffset);
  }
  return 0;
}


/**
  Find the offset in PCI device config space of capabilities structure of given ID.
  Example Extended CAPID list:
    0x0001 = Advanced Error Reporting capability
    0x0002 = Virtual Channel capability
    0x0003 = Device Serial Number capability
    0x0004 = Power Budgeting capability
    0x001F = Precision Time Measurement capability
  See "PCI Code and ID Assignment Specification" Table 3-1: Extended Capability IDs
  available at pcisig.com for full list of extended PCIe capabilities.

  @param[in] PciAddr  PCI device address.
  @param[in] CapId    CAPID to search for.

  @return Capability structure offset is returned, or zero if CAPID not found.
**/
UINT16
PciFindExtendedCapId (
  IN USRA_ADDRESS   *PciAddrPtr,
  IN UINT16          CapId
  )
{
  UINT16             CapHeaderOffset;
  UINT16             CapHeaderId;
  USRA_ADDRESS       PciAddr;

  USRA_ADDRESS_COPY (&PciAddr, PciAddrPtr);
  //
  // Start to search at Offset 0x100
  // Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
  //
  CapHeaderId = 0;
  CapHeaderOffset = 0x100;
  PciAddr.Attribute.AccessWidth = UsraWidth16;
  PciAddr.Attribute.AddrType = AddrTypePCIE;
  while (CapHeaderOffset != 0 && CapHeaderId != 0xFFFF) {

    PciAddr.Pcie.Offset = CapHeaderOffset;
    RegisterRead (&PciAddr, &CapHeaderId);
    if (CapHeaderId == CapId) {
      return CapHeaderOffset;
    }
    //
    // Each capability must be DWORD aligned.
    // The bottom two bits of all pointers are reserved and must be implemented as 00b
    // although software must mask them to allow for future uses of these bits.
    //
    PciAddr.Pcie.Offset = CapHeaderOffset + 2;
    RegisterRead (&PciAddr, &CapHeaderOffset);
    CapHeaderOffset = (CapHeaderOffset>>4) & ((UINT16) ~(BIT0 | BIT1));
  }
  return 0;
}


/**
  Check PCI config space, whether there is PCI device present.

  @param[in]  PciAddrPtr - Pointer to USRA address of PCI device config space.
  @param[out] PciHdrPtr  - Optional pointer to a buffer for PCI device header.

  @return If PCI device at PcieAddrPtr exists true is returned, otherwise false.
**/
BOOLEAN
PciIsDevicePresent (
  IN     USRA_ADDRESS     *PciAddrPtr,
     OUT PCI_TYPE_GENERIC *PciHdrPtr
  )
{
  PCI_TYPE_GENERIC         PciHdr;
  USRA_ADDRESS             PciAddr;

  USRA_ADDRESS_COPY (&PciAddr, PciAddrPtr);

  PciAddr.Attribute.AccessWidth = UsraWidth32;
  PciAddr.Attribute.AddrType = AddrTypePCIEBLK;
  PciAddr.PcieBlk.Count = sizeof(PciHdr) / sizeof(UINT32);
  PciAddr.Pcie.Offset = 0;
  RegisterRead (&PciAddr, &PciHdr);

  if (PciHdrPtr != NULL) {

    CopyMem (PciHdrPtr, &PciHdr, sizeof(*PciHdrPtr));
  }
  return PciHdr.Device.Hdr.VendorId != 0xFFFF;
}

