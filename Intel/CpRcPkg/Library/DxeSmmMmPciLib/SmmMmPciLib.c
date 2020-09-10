/** @file
  This file contains routines that get PCI Express Address

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2017 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SiliconRegAccess.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>

USRA_PROTOCOL     *mPciUsra = NULL;

/**
  The constructor function initialize UsraProtocol.

  @param[in] ImageHandle To be added.
  @param[in] SystemTable To be added.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
SmmMmPciLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  if ((mPciUsra == NULL) && (gSmst != NULL)) {
    //
    // USRA protocol need to be installed before the module access USRA.
    //
    Status = gSmst->SmmLocateProtocol (&gSmmUsraProtocolGuid, NULL, (VOID**) &mPciUsra);
    ASSERT_EFI_ERROR (Status);
    if (Status != EFI_SUCCESS) {
      return Status;
    }
    ASSERT (mPciUsra != NULL);
    if (mPciUsra == NULL) {
      return EFI_UNSUPPORTED;
    }
  }
  return EFI_SUCCESS;
}

/**
  This procedure will get PCIE address.

  @param[in] Bus                  Pci Bus Number.
  @param[in] Device               Pci Device Number.
  @param[in] Function             Pci Function Number.

  @retval PCIE address.
**/
UINTN
MmPciBase (
  IN UINT32                       Bus,
  IN UINT32                       Device,
  IN UINT32                       Function
  )
{
  USRA_ADDRESS Address;
  USRA_PCIE_ADDRESS (Address, UsraWidth32, Bus, Device, Function, 0);
  return mPciUsra->GetRegAddr (&Address);
}

/**
  This procedure will get PCIE address.

  @param[in] Seg                  Pcie Segment Number.
  @param[in] Bus                  Pcie Bus Number.
  @param[in] Device               Pcie Device Number.
  @param[in] Function             Pcie Function Number.
  @param[in] Register             To be added.

  @retval PCIE address.
**/
UINTN
MmPciAddress (
  IN UINT32                       Seg,
  IN UINT32                       Bus,
  IN UINT32                       Device,
  IN UINT32                       Function,
  IN UINT32                       Register
  )
{
  USRA_ADDRESS Address;
  USRA_PCIE_SEG_ADDRESS (Address, UsraWidth32, Seg, Bus, Device, Function, Register);
  return mPciUsra->GetRegAddr (&Address);
}
