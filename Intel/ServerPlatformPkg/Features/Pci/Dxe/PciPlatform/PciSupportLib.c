/** @file
  Support PCI chipset initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#include "PiDxe.h"
#include <Base.h>
#include <Guid/SocketIioVariable.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "IndustryStandard/Pci.h"
#include "PciSupportLib.h"
#include <Library/PreSiliconEnvDetectLib.h>

PCIE_STACK  mPcieStack;


/**

  This routine is used to check whether the pci device is present

  @retval None

**/
BOOLEAN
IsPciDevicePresent (
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL  *PciRootBridgeIo,
  OUT PCI_TYPE00                      *Pci,
  UINT8                               Bus,
  UINT8                               Device,
  UINT8                               Func
  )
// TODO:    PciRootBridgeIo - add argument and description to function comment
// TODO:    Pci - add argument and description to function comment
// TODO:    Bus - add argument and description to function comment
// TODO:    Device - add argument and description to function comment
// TODO:    Func - add argument and description to function comment
// TODO:    EFI_SUCCESS - add return value to function comment
// TODO:    EFI_NOT_FOUND - add return value to function comment
{
  UINT64      Address;
  UINT32      Dummy;
  EFI_STATUS  Status;

  Dummy=0xFFFFFFFF;
  //
  // Create PCI address map in terms of Bus, Device and Func
  //
  Address = EFI_PCI_ADDRESS (Bus, Device, Func, 0);

  //
  // Read the Vendor Id register
  //
  Status = PciRootBridgeIo->Pci.Read (
                                  PciRootBridgeIo,
                                  EfiPciWidthUint32,
                                  Address,
                                  1,
                                  Pci
                                  );
  /// WA Sighting 4168813. Add an extra write cycle as for LSI SAS cards
  if ((Pci->Hdr).VendorId == 0xffff) {
    /// PCIe card could have been assigned a temporary bus number.
  /// An write cycle can be used to try to rewrite the Bus number in the card
    /// Try to write the Vendor Id register, and recheck if the card is present.
    Status = PciRootBridgeIo->Pci.Write(
                                    PciRootBridgeIo,
                                    EfiPciWidthUint32,
                                    Address,
                                    1,
                                    &Dummy
                                    );

  // Retry the previous read after the PCI cycle has been tried.
    Status = PciRootBridgeIo->Pci.Read (
                                    PciRootBridgeIo,
                                    EfiPciWidthUint32,
                                    Address,
                                    1,
                                    Pci
                                    );
  }

  if (!EFI_ERROR (Status) && (Pci->Hdr).VendorId != 0xffff) {

    //
    // Read the entire config header for the device
    //

    Status = PciRootBridgeIo->Pci.Read (
                                    PciRootBridgeIo,
                                    EfiPciWidthUint32,
                                    Address,
                                    sizeof (PCI_TYPE00) / sizeof (UINT32),
                                    Pci
                                    );

    return TRUE;
  }

  return FALSE;
}
