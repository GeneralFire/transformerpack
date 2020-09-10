//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiPcieSegBusPei.c
    Contains the PEI library functions to get the segment and bus number
    of the socket and stack.
*/

#include <AmiPcieSegBusLib/AmiPcieSegBusLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/SystemInfoLib.h>

/**

  Update Segment# Bus# of all IIO Stacks from IIO UDS HOB in PCD Seg Bus table.

  @param[in] *PcieSegBusTable         - Pointer to a PCIe segment Bus table.

  @retval EFI_STATUS
  
**/
EFI_STATUS
EFIAPI
AmiUpdatePcieSegmentBusTable (
  PCIE_SEG_BUS_TABLE    *PcieSegBusTable
  )
{
  UINT8                   SocIndex;
  UINT8                   StackIndex;
  CPU_CSR_ACCESS_VAR      *CpuCsrAccessVarPtr;
  
  ASSERT (sizeof (PCIE_SEG_BUS_TABLE) >= PcdGetSize (PcdPcieSegBusTablePtr));
  
  //If table is not valid and updated, Get CPU_CSR_ACCESS_VAR and update the PCIe Segment Bus Table values from the structures
  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
  
  PcieSegBusTable->ValidFlag = TRUE;
  PcieSegBusTable->SocketPresentBitMap = CpuCsrAccessVarPtr->socketPresentBitMap;
  
  for (SocIndex = 0; SocIndex < MAX_SOCKET; SocIndex++) {
    PcieSegBusTable->SocketSegBus[SocIndex].SocketSegmentNumber = CpuCsrAccessVarPtr->segmentSocket[SocIndex];
    PcieSegBusTable->SocketSegBus[SocIndex].StackPresentBitMap = CpuCsrAccessVarPtr->stackPresentBitmap[SocIndex];
    
    for (StackIndex = 0; StackIndex < MAX_LOGIC_IIO_STACK; StackIndex++) {
      PcieSegBusTable->SocketSegBus[SocIndex].StackBusNumber[StackIndex] = CpuCsrAccessVarPtr->StackBus[SocIndex][StackIndex];
    }
  }
  
  DEBUG ((DEBUG_INFO, "AmiUpdatePcieSegmentBusTable Updated SEG BUS table PCD with CPU_CSR_ACCESS_VAR structure\n"));
  
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
  EFI_STATUS              Status;
  PCIE_SEG_BUS_TABLE      *PcieSegBusTable;
  
  //Get the PcdPcieSegBusTablePtr
  PcieSegBusTable = (PCIE_SEG_BUS_TABLE *) PcdGetPtr (PcdPcieSegBusTablePtr);
  
  if (!PcieSegBusTable->ValidFlag) {
      Status = AmiUpdatePcieSegmentBusTable (PcieSegBusTable);
      if (EFI_ERROR(Status))
          return FALSE;  
      }
  return (PcieSegBusTable->SocketPresentBitMap & (1 << SocketId));
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
  EFI_STATUS              Status;    
  PCIE_SEG_BUS_TABLE      *PcieSegBusTable;
    
  //Get the PcdPcieSegBusTablePtr
  PcieSegBusTable = (PCIE_SEG_BUS_TABLE *) PcdGetPtr (PcdPcieSegBusTablePtr);
  
  if (!PcieSegBusTable->ValidFlag) {
    Status = AmiUpdatePcieSegmentBusTable (PcieSegBusTable);
    if (EFI_ERROR(Status))
      return FALSE;  
  }
    
  return (PcieSegBusTable->SocketSegBus[SocketId].StackPresentBitMap & (1 << StackId));
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
  EFI_STATUS              Status;
  PCIE_SEG_BUS_TABLE      *PcieSegBusTable;
  
  //Get the PcdPcieSegBusTablePtr
  PcieSegBusTable = (PCIE_SEG_BUS_TABLE *) PcdGetPtr (PcdPcieSegBusTablePtr);
  
  if (!PcieSegBusTable->ValidFlag) {
    Status = AmiUpdatePcieSegmentBusTable (PcieSegBusTable);
    if (EFI_ERROR(Status))
      return EFI_NOT_READY;  
  }  
  
  //check if requested Socket ID is valid
  if (!(PcieSegBusTable->SocketPresentBitMap & (1 << SocketId)))
    return EFI_UNSUPPORTED;
  
  //check if requested Stack ID is valid
  if (!(PcieSegBusTable->SocketSegBus[SocketId].StackPresentBitMap & (1 << StackId)))
    return EFI_UNSUPPORTED;
  
  *Seg = PcieSegBusTable->SocketSegBus[SocketId].SocketSegmentNumber;
  *Bus = PcieSegBusTable->SocketSegBus[SocketId].StackBusNumber[StackId];
  
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
  EFI_STATUS              Status;
  PCIE_SEG_BUS_TABLE      *PcieSegBusTable;
    
  //Get the PcdPcieSegBusTablePtr
  PcieSegBusTable = (PCIE_SEG_BUS_TABLE *) PcdGetPtr (PcdPcieSegBusTablePtr);
  
  if (!PcieSegBusTable->ValidFlag) {
    Status = AmiUpdatePcieSegmentBusTable (PcieSegBusTable);
    if (EFI_ERROR(Status))
      return EFI_NOT_READY;  
  }
      
  //check if requested Socket ID is valid
  if (!(PcieSegBusTable->SocketPresentBitMap & (1 << SocketId)))
    return EFI_UNSUPPORTED;
    
  *Seg = PcieSegBusTable->SocketSegBus[SocketId].SocketSegmentNumber;
    
  return EFI_SUCCESS;
}
