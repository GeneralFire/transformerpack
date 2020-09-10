/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#include "SiliconAccessLibInternal.h"


//
// Declare I/O Ports used to perform PCI Confguration Cycles
//

/**
  This API gets the PciIo address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

**/
VOID
EFIAPI
GetPciIoAccessAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  )
{
  *AlignedAddress = (UINTN) (Address->Attribute.RawData32[0]);
  return;
}

/**
  Perform PCIIO read

  @param[in] AccessWidth          Access Width
  @param[in] AlignedAddress       An address to be read out
  @param[in] Buffer               A pointer of buffer contains the data to be read out

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
UsraPciIoAlignedRead (
  IN UINT32                   AccessWidth,
  IN UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  )
{
  BOOLEAN  InterruptState;

  InterruptState = SaveAndDisableInterrupts ();
  IoWrite32 (PCI_CONFIGURATION_ADDRESS_PORT, (UINT32)(AlignedAddress & 0xFFFFFFFC));
  switch (AccessWidth) {
    case  UsraWidth8:
      *((UINT8*) Buffer) = IoRead8 (PCI_CONFIGURATION_DATA_PORT + (UINT16)(AlignedAddress & 3));
      break;
    case  UsraWidth16:
      *((UINT16*) Buffer) = IoRead16 (PCI_CONFIGURATION_DATA_PORT + (UINT16)(AlignedAddress & 2));
      break;
    case  UsraWidth32:
      *((UINT32*) Buffer) = IoRead32 (PCI_CONFIGURATION_DATA_PORT);
      break;
    default:
      *((UINT32*) Buffer) = IoRead32 (PCI_CONFIGURATION_DATA_PORT);
      break;
      }
  SetInterruptState (InterruptState);

  return RETURN_SUCCESS;
}

/**
  Perform PCIIO write

  @param[in] AccessWidth          Access Width
  @param[in] AlignedAddress       An address to be read out
  @param[in] Buffer               A pointer of buffer contains the data to be read out

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
UsraPciIoAlignedWrite (
  IN UINT32                   AccessWidth,
  IN UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  )
{
  BOOLEAN  InterruptState;

  InterruptState = SaveAndDisableInterrupts ();
  IoWrite32 (PCI_CONFIGURATION_ADDRESS_PORT, (UINT32)(AlignedAddress & 0xFFFFFFFC));
  switch (AccessWidth) {
    case  UsraWidth8:
      IoWrite8 (PCI_CONFIGURATION_DATA_PORT + (UINT16)(AlignedAddress & 3), *((UINT8*) Buffer));
      break;
    case  UsraWidth16:
      IoWrite16 (PCI_CONFIGURATION_DATA_PORT + (UINT16)(AlignedAddress & 2), *((UINT16*) Buffer));
      break;
    case  UsraWidth32:
      IoWrite32 (PCI_CONFIGURATION_DATA_PORT, *((UINT32*) Buffer));
      break;
    default:
      IoWrite32 (PCI_CONFIGURATION_DATA_PORT, *((UINT32*) Buffer));
      break;
      }
  SetInterruptState (InterruptState);

  return RETURN_SUCCESS;
}
