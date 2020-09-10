//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiPcieSegBusDxe.c
    Contains the DXE library functions to get the segment and bus number
    of the socket and stack.
*/

#include "IioUniversalData.h"
#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <AmiPcieSegBusLib/AmiPcieSegBusLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>

//Global Variables
PCIE_SEG_BUS_TABLE      gPcieSegBusTable = {0};

/**

  Update Segment# Bus# of all IIO Stacks from IIO UDS HOB in PCD Seg Bus table.

  @param[in] *PcieSegBusTable         - Pointer to a PCIe segment Bus table.

  @retval EFI_STATUS
  
**/
EFI_STATUS
EFIAPI
AmiUpdatePcieSegBusDxeSmmConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINTN                     TableSize;
  PCIE_SEG_BUS_TABLE        *PcieSegBusTable;
  
  PcieSegBusTable = (PCIE_SEG_BUS_TABLE *) PcdGetPtr (PcdPcieSegBusTablePtr);
  
  ASSERT (sizeof (PCIE_SEG_BUS_TABLE) >= PcdGetSize (PcdPcieSegBusTablePtr));
  
  TableSize = PcdGetSize (PcdPcieSegBusTablePtr);
  
  CopyMem (&gPcieSegBusTable, PcieSegBusTable, TableSize);
  
  return EFI_SUCCESS;
}

/**

  Return Socket presence state.

  @param[in] SocketId               - Socket number of a multi socket system

  @retval BOOLEAN

**/
BOOLEAN
AmiIsSocketValid (
  IN UINT8          SocketId
  )
{
  if (!gPcieSegBusTable.ValidFlag)
    return FALSE;
  
  return (gPcieSegBusTable.SocketPresentBitMap & (1 << SocketId));
    
}

/**

  Return Stack presence state.

  @param[in] SocketId             - Socket number of a multi socket system
  @param[in] StackId              - Stack number in a Socket

  @retval BOOLEAN

**/
BOOLEAN
AmiIsStackValid (
  IN UINT8          SocketId,
  IN UINT8          StackId
  )
{
  if (!gPcieSegBusTable.ValidFlag)
    return FALSE; 
  
  return (gPcieSegBusTable.SocketSegBus[SocketId].StackPresentBitMap & (1 << StackId));
}

/**

  Return the PCI segment and bus number of specified Socket and stack.

  @param[in] SocketId               - Socket number of a multi socket system
  @param[in] StackId                - Stack number in a Socket
  @param[in,out] *Seg               - PCI Segment number
  @param[in,out] *Bus               - PCI Bus number

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
AmiGetPcieSegmentBus (
  IN UINT8          SocketId,
  IN UINT8          StackId,
  IN OUT UINT8      *Seg,
  IN OUT UINT8      *Bus
  )
{
  if (!gPcieSegBusTable.ValidFlag)
    return EFI_NOT_READY;
  
  //check if requested Socket ID is valid
  if (!(gPcieSegBusTable.SocketPresentBitMap & (1 << SocketId)))
    return EFI_UNSUPPORTED;
  
  //check if requested Stack ID is valid
  if (!(gPcieSegBusTable.SocketSegBus[SocketId].StackPresentBitMap & (1 << StackId)))
    return EFI_UNSUPPORTED;
  
  *Seg = gPcieSegBusTable.SocketSegBus[SocketId].SocketSegmentNumber;
  *Bus = gPcieSegBusTable.SocketSegBus[SocketId].StackBusNumber[StackId];
  
  return EFI_SUCCESS;
}

/**

  Return the PCI segment number of specified Socket and stack.

  @param[in] SocketId               - Socket number of a multi socket system
  @param[in,out] *Seg               - PCI Segment number

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
AmiGetPcieSegment (
  IN UINT8          SocketId,
  IN OUT UINT8      *Seg
  )
{
  if (!gPcieSegBusTable.ValidFlag)
    return EFI_NOT_READY;  
  
  //check if requested Socket ID is valid
  if (!(gPcieSegBusTable.SocketPresentBitMap & (1 << SocketId)))
    return EFI_UNSUPPORTED;
    
  *Seg = gPcieSegBusTable.SocketSegBus[SocketId].SocketSegmentNumber;
    
  return EFI_SUCCESS;
}

