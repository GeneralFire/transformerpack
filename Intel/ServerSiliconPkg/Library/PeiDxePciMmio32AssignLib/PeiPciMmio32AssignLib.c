/** @file
  Source code file for the implementation of PeiPciMmio32AssignLib

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include "PciMemBarAllocation.h"
#include <Library/CpuAndRevisionLib.h>

CHAR8 DUMP_MEM_MAP_TABLE[] = "\n==== Dump PCI MMIO Map Table %a ====\n\n";
CHAR8 DUMP_MEM_MAP_RECORDS_COUNT[] = "MMIO Map Table: %a records count: %04d\n";

INCOMPATIBLE_PCIE_BAR mIncompatiblePcieBarTable[] = {
  {CPU_SPRSP, REV_AX, 0x8086, 0x09A6, 0x10, 0x1FFF    }, // Device OOB-MSM BAR0
  {CPU_SPRSP, REV_AX, 0x8086, 0x09A7, 0x10, 0x3FFF    }, // Device PMON OOB-MSM BAR0
  {CPU_SPRSP, REV_AX, 0x8086, 0x09A7, 0x14, 0xFFFFF   }, // Device PMON OOB-MSM BAR1
  {CPU_SPRSP, REV_AX, 0x8086, 0x09A8, 0x10, 0xFFFFF   }  // Device DFX OOB-MSM BAR0
};

UINT8 mIncompatiblePcieBarTable_length = sizeof(mIncompatiblePcieBarTable)/sizeof(mIncompatiblePcieBarTable[0]);

/**
  Dump the memory map of an IIO Stack.

  @param[in] Buffer                         A pointer to the Buffer that contains the Memory Map Table.
  @param[in] NumberOfMemoryMapRecords       The number of Memory Map Recourds in the Memory Map Table.

**/
VOID
EFIAPI
DumpMemoryMapTable (
  IN MMIO_MAP_INFO            *MmioMapTable,
  IN UINT16                    NumberOfMemoryMapRecords,
  IN MMIO_ALLOCATION_INFO_HOB *MmioAllocationInfoHobPtr
  )
{
  UINT16  DevIndex;
  UINT16  NodeIndex;
  UINT8   NumOfUsedMmioRange;

  NumOfUsedMmioRange = 0;

  DEBUG ((EFI_D_INFO, DUMP_MEM_MAP_TABLE, "Begin"));
  DEBUG ((EFI_D_INFO, DUMP_MEM_MAP_RECORDS_COUNT, "Total", NumberOfMemoryMapRecords));

  for (NodeIndex = 0; NodeIndex < NumberOfMemoryMapRecords; NodeIndex++) {

    DEBUG ((EFI_D_INFO, "Record [%03d] Base: 0x%08X, Limit: 0x%08X, ",
            NodeIndex, MmioMapTable[NodeIndex].BaseAddress, MmioMapTable[NodeIndex].LimitAddress));
    if (MmioMapTable[NodeIndex].Used) {

      NumOfUsedMmioRange++;
      for (DevIndex = 0; DevIndex < MmioAllocationInfoHobPtr->Count; DevIndex++) {

        if (MmioMapTable[NodeIndex].BaseAddress == MmioAllocationInfoHobPtr->PciBarInfo[DevIndex].MmioBase) {

          DEBUG ((EFI_D_INFO, "S%02X:B%02X:D%02X:F%X BAR 0x%X\n",
                  MmioAllocationInfoHobPtr->PciBarInfo[DevIndex].Seg,
                  MmioAllocationInfoHobPtr->PciBarInfo[DevIndex].Bus,
                  MmioAllocationInfoHobPtr->PciBarInfo[DevIndex].Dev,
                  MmioAllocationInfoHobPtr->PciBarInfo[DevIndex].Func,
                  MmioAllocationInfoHobPtr->PciBarInfo[DevIndex].Offset));
          break;
        }
      }
      if (DevIndex >= MmioAllocationInfoHobPtr->Count) {
        DEBUG ((EFI_D_INFO, "???\n"));
      }
    } else {
      DEBUG ((EFI_D_INFO, "Unused\n"));
    }
  }
  DEBUG ((EFI_D_INFO, DUMP_MEM_MAP_RECORDS_COUNT, "Used", NumOfUsedMmioRange));
  DEBUG ((EFI_D_INFO, DUMP_MEM_MAP_TABLE, "End"));

  return;
}

/**
  Program MMIO base address to the PCI BAR Register.

  @param[in] PciBarNodePtr     A pointer to the PCI_BAR_INFO variable.

**/
VOID
EFIAPI
SetBarAddr (
  IN PCI_BAR_INFO    *PciBarNodePtr
  )
{
  USRA_ADDRESS  UsraPciAddress;
  UINT32        TempRegValue;

  USRA_PCIE_SEG_ADDRESS (UsraPciAddress, UsraWidth32, PciBarNodePtr->Seg, PciBarNodePtr->Bus, PciBarNodePtr->Dev, PciBarNodePtr->Func, PciBarNodePtr->Offset);
  RegisterRead (&UsraPciAddress, &TempRegValue);
  TempRegValue &= PCI_BAR_MASK;
  TempRegValue = PciBarNodePtr->MmioBase | TempRegValue;
  RegisterWrite (&UsraPciAddress, &TempRegValue);

  if (PciBarNodePtr->BarType == PciBarMem64) {
    UsraPciAddress.Pcie.Offset += 4;
    TempRegValue = 0;  //Always set the high 32bit Bar register to be 0 as this library does not support 64bit MMIO
    RegisterWrite (&UsraPciAddress, &TempRegValue);
  }

  return;
}


/**
  Read a PCI BAR Register.

  @param[in]  PciBarNodePtr     A pointer to the PCI_BAR_INFO variable.

  @return     The data read from the PCI BAR Register.

**/
UINT32
EFIAPI
ReadBarRegister (
  IN PCI_BAR_INFO    *PciBarNodePtr
  )
{
  USRA_ADDRESS  UsraPciAddress;
  UINT32        TempRegValue;

  USRA_PCIE_SEG_ADDRESS (UsraPciAddress, UsraWidth32, PciBarNodePtr->Seg, PciBarNodePtr->Bus, PciBarNodePtr->Dev, PciBarNodePtr->Func, PciBarNodePtr->Offset);
  RegisterRead (&UsraPciAddress, &TempRegValue);
  DEBUG ((EFI_D_VERBOSE, "[ReadBarRegister] Data read back from this BAR Register = 0x%x\n", TempRegValue));

  return TempRegValue;
}

/**
  Override Bar Size for Imcompatible PCIe Device.

  @param[in]  PciBarNodePtr     A pointer to the PCI_BAR_INFO variable.
  @param[in]  Data32            A pointer to the PCI BAR value.
**/
VOID
EFIAPI
OverrideBarSize (
  IN PCI_BAR_INFO    *PciBarNodePtr,
  IN OUT UINT32      *Data32
  )
{
  USRA_ADDRESS  UsraPciAddress;
  UINT8         Index;
  UINT16        Did;
  UINT16        Vid;
  UINT16        Offset;
  UINT32        BarValue;

  USRA_PCIE_SEG_ADDRESS (UsraPciAddress, UsraWidth16, PciBarNodePtr->Seg, PciBarNodePtr->Bus, PciBarNodePtr->Dev, PciBarNodePtr->Func, PCI_VENDOR_ID_OFFSET);
  RegisterRead (&UsraPciAddress, &Vid);

  USRA_PCIE_SEG_ADDRESS (UsraPciAddress, UsraWidth16, PciBarNodePtr->Seg, PciBarNodePtr->Bus, PciBarNodePtr->Dev, PciBarNodePtr->Func, PCI_DEVICE_ID_OFFSET);
  RegisterRead (&UsraPciAddress, &Did);

  Offset = PciBarNodePtr->Offset;
  BarValue = *Data32;

  for (Index = 0; Index < mIncompatiblePcieBarTable_length; Index++) {

    if (!IsCpuAndRevision (mIncompatiblePcieBarTable[Index].CpuType, mIncompatiblePcieBarTable[Index].CpuRevision)) {
      continue;
    }
    if ((Vid == mIncompatiblePcieBarTable[Index].Vid) &&
        (Did == mIncompatiblePcieBarTable[Index].Did) &&
        (Offset == mIncompatiblePcieBarTable[Index].BarOffset)) {
      BarValue = (~mIncompatiblePcieBarTable[Index].BarAlignment) | (BarValue & 0x0F);
      DEBUG ((EFI_D_VERBOSE, "Override BarSize(offset:0x%x, BarValue:0x%x) for incompatible PCIe device(Vid:0x%x, Did:0x%x).\n", Offset, BarValue, Vid, Did));
    }
  }

  *Data32 = BarValue;
  return;
}

/**
  Read a PCI BAR Register's respond data.

  First save the original value of this PCI BAR Register;
  Then program 0xFFFFFFFF to this BAR, and read the BAR Register again;
  Finally restore the saved data to this BAR Register.

  @param[in]  PciBarNodePtr     A pointer to the PCI_BAR_INFO variable.

  @return     The data immediately returned from the BAR after programming 0xFFFFFFFF to it.

**/
UINT32
EFIAPI
GetBarSizingRespondData (
  IN     PCI_BAR_INFO    *PciBarNodePtr
  )
{
  USRA_ADDRESS  UsraPciAddress;
  UINT32  OriginalRegValue;
  UINT32  Data32;

  USRA_PCIE_SEG_ADDRESS (UsraPciAddress, UsraWidth32, PciBarNodePtr->Seg, PciBarNodePtr->Bus, PciBarNodePtr->Dev, PciBarNodePtr->Func, PciBarNodePtr->Offset);

  RegisterRead (&UsraPciAddress, &OriginalRegValue);
  DEBUG ((EFI_D_VERBOSE, "Save original BAR Register value: 0x%x\n", OriginalRegValue));

  Data32 = (BAR_WRITE_ALL_ONE_WA | (OriginalRegValue & 0x0F));
  RegisterWrite (&UsraPciAddress, &Data32);

  RegisterRead (&UsraPciAddress, &Data32);
  DEBUG ((EFI_D_VERBOSE, "Read back from the BAR Register, Value: 0x%x\n", Data32));

  RegisterWrite (&UsraPciAddress, &OriginalRegValue);

  OverrideBarSize (PciBarNodePtr, &Data32);

  return Data32;
}

/**
  Sizing a PCI BAR Register.

  Read a PCI BAR Register's respond data, then perform the BAR Sizing to calculate the resource size.
  1) If the BAR Register's respond data equals to 0, then this BAR Register is not implemented, EFI_ABORTED is returned;
  2) If the BAR Register's respond data with bit 0 hardwired to a 1b, this is an IO BAR, EFI_ABORTED is returned;
  3) If the BAR Register's respond data with bit 0 hardwired to a 0b, this is a Memory BAR, check the bit2:1
     3.1) If Bit2:1 = 00b, this is a valid 32bit Memory BAR, update BAR Type and BAR Size to PciBarNodePtr, EFI_SUCCESS is returned;
     3.2) If Bit2:1 = 10b, this is a valid 64bit Memory BAR, first clearing the encoding information bits (bits 0-3 for Memory BAR)
          3.2.1) If the remaining data equals to 0, it means this 64bit BAR requests >= 4GB MMIO, it is not supported in the PEI phase, EFI_UNSUPPORTED is returned;
          3.2.2) If the remaining data does not equal to 0, update BAR Type and BAR Size to PciBarNodePtr, EFI_SUCCESS is returned.
     3.3) If Bit2:1 = 01b or 11b, this is an error since both 01b and 11b are reserved in PCI Specification, EFI_DEVICE_ERROR is returned.
  Also, it will move RegOffsetPtr to next BAR.

  @param[in out]  PciBarNodePtr     A pointer to the PCI_BAR_INFO variable.
  @param[in out]  RegOffsetPtr      A pointer to the BAR Register Offset

  @retval  EFI_SUCCESS          PCI BAR Sizing is susscussful.
  @retval  EFI_ABORTED          This BAR is not implemented or this is an IO BAR.
  @retval  EFI_UNSUPPORTED      This is 64bit Memory BAR requests >=4GB MMIO.
  @retval  EFI_DEVICE_ERROR     The BAR register violates the PCI Specification.

**/
EFI_STATUS
EFIAPI
PciBarSizing (
  IN OUT PCI_BAR_INFO              *PciBarNodePtr,
  IN OUT UINT16                    *RegOffsetPtr
  )
{
  UINT32  BarLength;

  BarLength = GetBarSizingRespondData (PciBarNodePtr);
  if (BarLength == 0) {
    //This Memory Bar register is not implemented, move to the next BAR Register
    *RegOffsetPtr += 4;
    return EFI_ABORTED;
  }

  if ((BarLength & BIT0) == BIT0) {
    //IO BAR Register is not cared, move to the next BAR Register
    *RegOffsetPtr += 4;
    return EFI_ABORTED;
  }

  //
  // Memory BAR Register
  //
  switch (BarLength & 0x07) {
    case 0x00:
      //
      // 32bit memory address
      //
      PciBarNodePtr->BarType = PciBarMem32;
      PciBarNodePtr->MmioSize = (UINT32)(~(BarLength & PCI_MEM_BAR_BASE_ADDR_MASK)) + 1;
      *RegOffsetPtr += 4;  //Move to the next BAR Register
      DEBUG ((EFI_D_VERBOSE, "This 32bit Memory Bar requests [0x%x Bytes] MMIO.\n", PciBarNodePtr->MmioSize));
      break;

    case 0x04:
      //
      // 64bit memory address
      //
      if ((BarLength & PCI_MEM_BAR_BASE_ADDR_MASK) == 0) {
        DEBUG ((EFI_D_VERBOSE, "This 64bit Memory Bar requests >= 4G Bytes MMIO. Not supported!\n"));
        *RegOffsetPtr += 8;  //Skip this 64-bit Memory BAR, move to the next BAR Register
        return EFI_UNSUPPORTED;
      } else {
        PciBarNodePtr->BarType = PciBarMem64;
        PciBarNodePtr->MmioSize = (UINT32)(~(BarLength & PCI_MEM_BAR_BASE_ADDR_MASK)) + 1;
        *RegOffsetPtr += 8; //Move to the next BAR Register
        DEBUG ((EFI_D_VERBOSE, "This 64bit Memory Bar requests [0x%x Bytes] MMIO.\n", PciBarNodePtr->MmioSize));
      }
      break;

    default:
      //Bit2:1: Both 01b and 11b are reserved in PCI Specification.
      DEBUG ((EFI_D_ERROR, "[Violation detected] The encoding bits[2:1] of BAR Register violates the PCI Specification.\n"));
      *RegOffsetPtr += 4;  //Move to the next BAR Register
      return EFI_DEVICE_ERROR;
      break;
  } //switch

  return EFI_SUCCESS;
}

/**
  Check a BAR register if it is pre-assigned with MMIO resource.

  Read this PCI BAR Register.
    If this is IO BAR Register, just ignore it and return EFI_ABORTED;
    Else check the bit2:1 of the data read from this BAR Register
     (1) If Bit2:1 = 00b, this may be a 32bit Memory BAR or unimplemented BAR register, then clear the encoding informantion bits (bits 0-3 for Memory BAR)
         (1.1) If the remaining data does not equal to 0, this BAR is pre-assigned with MMIO, return EFI_SUCCESS if no coflict detected, else return EFI_DEVICE_ERROR;
         (1.2) If the remaining data equals to 0,  return EFI_NO_MAPPING;
     (2) If Bit2:1 = 10b, this is a 64bit Memory BAR, also clear the encoding information bits (bits 0-3 for Memory BAR)
         (2.1) If the remaining data does not equal to 0, this BAR is pre-assigned with MMIO, return EFI_SUCCESS if no coflict detected, else return EFI_DEVICE_ERROR;
         (2.2) If the remaining data equals to 0,  return EFI_NO_MAPPING;
     (3) If Bit2:1 = 01b or 11b, this is an error since both 01b and 11b are reserved in PCI Specification, return EFI_DEVICE_ERROR.
  Before return, it will move RegOffsetPtr to next BAR.

  @param[in]      PciBarNodePtr     A pointer to the PCI_BAR_INFO variable.
  @param[in out]  RegOffsetPtr      A pointer to the BAR Register Offset
  @param[in]      StartMmioAddr     The start address of this IIO Stack's MMIO resource
  @param[in]      EndMmioAddr       The end address of this IIO Stack's MMIO resource

  @retval  EFI_SUCCESS          PCI BAR is pre-assigned with MMIO resource.
  @retval  EFI_ABORTED          This BAR is an IO BAR.
  @retval  EFI_NO_MAPPING       This BAR is not implemented or not pre-assigned with MMIO resource.
  @retval  EFI_DEVICE_ERROR     The BAR register violates the PCI Specification or MMIO conflict detected.

**/
EFI_STATUS
EFIAPI
IsPciMemBarPreAssigned (
  IN       PCI_BAR_INFO        *PciBarNodePtr,
  IN  OUT  UINT16              *RegOffsetPtr,
  IN       UINT32              StartMmioAddr,
  IN       UINT32              EndMmioAddr
  )
{
  UINT32  RegValue, PreAssignedMmioBaseLow, PreAssignedMmioBaseHigh;
  UINT64  BaseAddress64;

  RegValue = ReadBarRegister (PciBarNodePtr);

  if ((RegValue & BIT0) == BIT0) {
    //IO BAR Register is not cared, move to the next BAR Register
    *RegOffsetPtr += 4;
    return EFI_ABORTED;
  }

  PreAssignedMmioBaseLow = RegValue & PCI_MEM_BAR_BASE_ADDR_MASK;
  PreAssignedMmioBaseHigh = 0;

  //
  // Memory BAR Register
  //
  switch (RegValue & 0x07) {
    case 0x00:
      //
      // 32bit memory address or unimplemented BAR Register
      //
      *RegOffsetPtr += 4;  //Move to the next BAR Register
      break;

    case 0x04:
      //
      // 64bit memory address
      //
      PciBarNodePtr->Offset += 4;
      PreAssignedMmioBaseHigh = ReadBarRegister (PciBarNodePtr);
      *RegOffsetPtr += 8;  //Move to the next BAR Register
      break;

    default:
      //Bit2:1: Both 01b and 11b are reserved in PCI Specification.
      DEBUG ((EFI_D_ERROR, "[Violation detected] The encoding bits[2:1] of BAR Register violates the PCI Specification.\n"));
      *RegOffsetPtr += 4;  //Move to the next BAR Register
      return EFI_DEVICE_ERROR;
      break;
  } //switch

  if (PreAssignedMmioBaseHigh != 0) {
    BaseAddress64 = PreAssignedMmioBaseHigh;
    BaseAddress64 = LShiftU64 (BaseAddress64, 32);
    BaseAddress64 |= (UINT64) PreAssignedMmioBaseLow;
    DEBUG ((EFI_D_VERBOSE, "PreAssigned 64bit MmioBase @ 0x%x\n", BaseAddress64));
    // Since the BAR Base > 4GB, so it will never conflict with any Stack's MMIO resource below 4GB, just return SUCCESS.
    return EFI_SUCCESS;
  }

  if (PreAssignedMmioBaseLow != 0) {
    DEBUG ((EFI_D_VERBOSE, "PreAssigned 32bit MmioBase @ 0x%x\n", PreAssignedMmioBaseLow));

    //
    //Note:
    // Since this device with BME bit set, it is not able to do PciBarSizing for its BAR registers, so not know the BAR size!
    // Here it just check the pre-assigned MmioBase to see if the MmioBase conflicts with current stack's MMIO resource.
    //
    if ((PreAssignedMmioBaseLow >= StartMmioAddr && PreAssignedMmioBaseLow <= EndMmioAddr)) {
      //Pre-assigned but conflict detected
      DEBUG ((EFI_D_ERROR, "[Conflict detected] The preassigned MMIO conflict with this Stack's MMIO.\n"));
      return EFI_DEVICE_ERROR;
    }

    return EFI_SUCCESS;
  } else {
    DEBUG ((EFI_D_VERBOSE, "Not implemented or not PreAssigned with MMIO!\n"));
    return EFI_NO_MAPPING;
  }
}

/**
  Check a PCI BAR(Assume BAR Sizing is already performed).

  1) If the BAR Size equals to 0, it means BAR Sizing is not performed yet or BAR Sizing is failed, return EFI_ACCESS_DENIED;
  2) If the BAR Size does not equal to 0, get the pre-assigned MMIO Base by reading this BAR and clearing the encoding information bits (bits 0-3 for Memory BAR)
     2.1) If the pre-assigned MMIO Base equals to 0: This BAR is not pre-assigned with MMIO Resource, return EFI_NO_MAPPING;
     2.2) If the pre-assigned MMIO Base does not equal to 0: This BAR is pre-assigned with MMIO Resource
          2.2.1) If the pre-assigned MMIO conflicts with the IIO Stack's MMIO, return EFI_DEVICE_ERROR;
          2.2.2) If the pre-assigned MMIO does not conflict with the IIO Stack's MMIO, return EFI_SUCCESS.

  @param[in]  PciBarNodePtr     A pointer to the PCI_BAR_INFO variable.
  @param[in]  StartMmioAddr     The start address of this IIO Stack's MMIO resource
  @param[in]  EndMmioAddr       The end address of this IIO Stack's MMIO resource

  @retval  EFI_SUCCESS          The BAR is pre-assigned with MMIO and no conflict is detected.
  @retval  EFI_NO_MAPPING       The BAR is not pre-assigned with MMIO Resource.
  @retval  EFI_ACCESS_DENIED    BAR Sizing is not performed before call this routine or BAR Sizing is failed.
  @retval  EFI_DEVICE_ERROR     The BAR is pre-assigned with MMIO but conflict is detected.

**/
EFI_STATUS
EFIAPI
PciCheckBarAdaptive (
  IN     PCI_BAR_INFO              *PciBarNodePtr,
  IN     UINT32                    StartMmioAddr,
  IN     UINT32                    EndMmioAddr
  )
{
  UINT32        PreAssignedMmioBase;

  if (PciBarNodePtr->MmioSize == 0) {
    DEBUG ((EFI_D_VERBOSE, "[WARNING] BAR Sizing is not performed or BAR Sizing is failed.\n"));
    return EFI_ACCESS_DENIED;
  }

  PreAssignedMmioBase = ReadBarRegister (PciBarNodePtr) & PCI_MEM_BAR_BASE_ADDR_MASK;

  if (PreAssignedMmioBase == 0) {
    //Not pre-assigned and no conflict detected
    return EFI_NO_MAPPING;
  }

  DEBUG ((EFI_D_VERBOSE, "Pre-assigned MMIO Base is 0x%08x\n", PreAssignedMmioBase));

  if ((PreAssignedMmioBase >= StartMmioAddr && PreAssignedMmioBase <= EndMmioAddr) ||
      ((PreAssignedMmioBase + PciBarNodePtr->MmioSize) >= StartMmioAddr && (PreAssignedMmioBase + PciBarNodePtr->MmioSize) <= EndMmioAddr)) {
    //Pre-assigned but conflict detected
    DEBUG ((EFI_D_ERROR, "[Conflict detected] The preassigned MMIO conflict with this Stack's MMIO.\n"));
    return EFI_DEVICE_ERROR;
  }

  //Pre-assigned and no conflict detected
  return EFI_SUCCESS;
}

/**
  Check a BAR size from its lowest bit to know how many consecutive bits are 0b

  @param[in]  ResourceSize     The BAR size of a BAR Register.

  @return     Return the count of bits that is consecutive 0b.

**/
UINT8
EFIAPI
ParseBarSize (
  IN     UINT32                    ResourceSize
  )
{
  UINT8      Count;
  UINT32     Data32;

  if (ResourceSize == 0) {
    DEBUG ((EFI_D_ERROR, "[Invalid Input] ResourceSize = 0.\n"));
    return 0;
  }

  Count  = 0;
  Data32 = ResourceSize;

  while ((Data32 & BIT0) == 0) {
    Data32 = Data32 >> 1;
    Count++;
  }

  return Count;
}

/**
  Assign MMIO to a BAR register(Assume BAR Sizing is already performed).

  1) If the BAR Size equals to 0, it means BAR Sizing is not performed yet or BAR Sizing is failed, return EFI_ACCESS_DENIED;
  2) If the BAR Size does not equal to 0, always reserve multiple 4KB of MMIO resource;
  3) If the remaining MMIO resource is based on lower boundary but this BAR requires bigger aperture, then adjust the start address of MMIO to be assigned to this BAR;
  4) Append new Memory Map records to this IIO Stack's Memory Map table;
  5) Append new PCI_BAR_INFO nodes to the MMIO_ALLOCATION_INFO_HOB.

  @param[in out]  PciBarNodePtr              A pointer to the PCI_BAR_INFO variable.
  @param[in out]  RemainingMmioStartAddrPtr  The start address of this IIO Stack's remaining MMIO resource
  @param[in]      EndMmioAddr                The end address of this IIO Stack's MMIO resource
  @param[in out]  MmioMapArray               A pointer to the Buffer that contains the Memory Map Table.
  @param[in out]  MmioAllocationInfoHobPtr   A pointer to the MMIO_ALLOCATION_INFO_HOB

  @retval  EFI_SUCCESS           This function returned successfully.
  @retval  EFI_OUT_OF_RESOURCES  This IIO Stack's MMIO is out of resources.
  @retval  EFI_ACCESS_DENIED     BAR Sizing is not performed before call this routine or BAR Sizing is failed.
  @retval  EFI_BUFFER_TOO_SMALL  The Buffer of Memory Map Table is too small or the MMIO_ALLOCATION_INFO_HOB is too small.

**/
EFI_STATUS
EFIAPI
PciAssignBarAdaptive (
  IN OUT PCI_BAR_INFO              *PciBarNodePtr,
  IN OUT UINT32                    *RemainingMmioStartAddrPtr,
  IN     UINT32                    EndMmioAddr,
  IN OUT MMIO_MAP_INFO             *MmioMapArray,
  IN OUT MMIO_ALLOCATION_INFO_HOB  *MmioAllocationInfoHobPtr
  )
{
  UINT32        ActuallyConsumedMmio;
  UINT8         LeftShiftCount, NewMemMapRecordsCount;
  UINT32        AllignedMmioBaseMask;

  if (PciBarNodePtr->MmioSize == 0) {
    DEBUG ((EFI_D_ERROR, "[PCI] WARNING: BAR Sizing is not performed or BAR Sizing failed\n"));
    return EFI_ACCESS_DENIED;
  }

  ActuallyConsumedMmio = PciBarNodePtr->MmioSize;
  if ((ActuallyConsumedMmio & 0xFFF) != 0) {
    //Make sure the actually assigned MMIO resource size is multiple 4KB
    ActuallyConsumedMmio = (ActuallyConsumedMmio & ~0xFFF) + 0x1000;
  }

  PciBarNodePtr->MmioBase = *RemainingMmioStartAddrPtr;
  LeftShiftCount = ParseBarSize (PciBarNodePtr->MmioSize);
  AllignedMmioBaseMask = (1 << LeftShiftCount) - 1;
  if ((*RemainingMmioStartAddrPtr & AllignedMmioBaseMask) != 0) {
    //Make sure the assigned MMIO start address is aligned
    PciBarNodePtr->MmioBase = (*RemainingMmioStartAddrPtr & ~AllignedMmioBaseMask) + (1 << LeftShiftCount);
  }

  if (PciBarNodePtr->MmioBase + ActuallyConsumedMmio > EndMmioAddr) {

    DEBUG ((EFI_D_ERROR, "[PCI] ERROR: No enough MMIO resource!\n"));
    return EFI_OUT_OF_RESOURCES;

  } else {

    SetBarAddr (PciBarNodePtr);
    if ((ReadBarRegister (PciBarNodePtr) & PCI_MEM_BAR_BASE_ADDR_MASK) != PciBarNodePtr->MmioBase) {

      DEBUG ((EFI_D_ERROR, "[PCI] ERROR: Failed to program BAR register for %02X:%02X:%02X.%X\n",
              PciBarNodePtr->Seg, PciBarNodePtr->Bus, PciBarNodePtr->Dev, PciBarNodePtr->Func));
    }
  }

  if (PciBarNodePtr->MmioBase != 0) {

    NewMemMapRecordsCount = 1;
    if (*RemainingMmioStartAddrPtr < PciBarNodePtr->MmioBase) {
      NewMemMapRecordsCount = 2;
    }

    if ( (sizeof (MMIO_MAP_INFO) * (PciBarNodePtr->NodeIndex + NewMemMapRecordsCount)) > MEMORY_MAP_TABLE_SIZE ) {
      DEBUG ((EFI_D_ERROR, "[PCI] ERROR: Memory Map buffer is too small!\n"));
      return EFI_BUFFER_TOO_SMALL;
    }

    if ( (MmioAllocationInfoHobPtr->Count + 1) > MAX_PCI_INFO_NODE_COUNT ) {
      DEBUG ((EFI_D_ERROR, "[PCI] ERROR: MMIO_ALLOCATION_INFO_HOB is too small!\n"));
      return EFI_BUFFER_TOO_SMALL;
    }

    if (*RemainingMmioStartAddrPtr < PciBarNodePtr->MmioBase) {
      //Create one Memory Map record for memory hole
      MmioMapArray[PciBarNodePtr->NodeIndex].BaseAddress      = *RemainingMmioStartAddrPtr;
      MmioMapArray[PciBarNodePtr->NodeIndex].LimitAddress     = PciBarNodePtr->MmioBase - 1;
      MmioMapArray[PciBarNodePtr->NodeIndex].Used             = FALSE;

      PciBarNodePtr->NodeIndex++;
    }

    //Update the start address of unused MMIO resource
    *RemainingMmioStartAddrPtr = PciBarNodePtr->MmioBase + ActuallyConsumedMmio;

    //Create Memory Map record for the MMIO Resource that is assigned to this PCI Memory Bar Register
    MmioMapArray[PciBarNodePtr->NodeIndex].BaseAddress      = PciBarNodePtr->MmioBase;
    MmioMapArray[PciBarNodePtr->NodeIndex].LimitAddress     = PciBarNodePtr->MmioBase + ActuallyConsumedMmio - 1;
    MmioMapArray[PciBarNodePtr->NodeIndex].Used             = TRUE;

    MmioAllocationInfoHobPtr->PciBarInfo[MmioAllocationInfoHobPtr->Count].NodeIndex = PciBarNodePtr->NodeIndex;
    MmioAllocationInfoHobPtr->PciBarInfo[MmioAllocationInfoHobPtr->Count].Seg       = PciBarNodePtr->Seg;
    MmioAllocationInfoHobPtr->PciBarInfo[MmioAllocationInfoHobPtr->Count].Bus       = PciBarNodePtr->Bus;
    MmioAllocationInfoHobPtr->PciBarInfo[MmioAllocationInfoHobPtr->Count].Dev       = PciBarNodePtr->Dev;
    MmioAllocationInfoHobPtr->PciBarInfo[MmioAllocationInfoHobPtr->Count].Func      = PciBarNodePtr->Func;
    MmioAllocationInfoHobPtr->PciBarInfo[MmioAllocationInfoHobPtr->Count].Offset    = PciBarNodePtr->Offset;
    MmioAllocationInfoHobPtr->PciBarInfo[MmioAllocationInfoHobPtr->Count].BarType   = PciBarNodePtr->BarType;
    MmioAllocationInfoHobPtr->PciBarInfo[MmioAllocationInfoHobPtr->Count].MmioBase  = PciBarNodePtr->MmioBase;
    MmioAllocationInfoHobPtr->PciBarInfo[MmioAllocationInfoHobPtr->Count].MmioSize  = PciBarNodePtr->MmioSize;

    MmioAllocationInfoHobPtr->Count++;
    PciBarNodePtr->NodeIndex++;

    //Create Memory Map record for the unused MMIO Resource
    MmioMapArray[PciBarNodePtr->NodeIndex].BaseAddress  = PciBarNodePtr->MmioBase + ActuallyConsumedMmio;
    MmioMapArray[PciBarNodePtr->NodeIndex].LimitAddress = EndMmioAddr;
    MmioMapArray[PciBarNodePtr->NodeIndex].Used         = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  Initialize a PCI_BAR_INFO variable.

  @param[in] PciBarNodePtr     A pointer to a PCI_BAR_INFO variable.

**/
VOID
EFIAPI
InitPciBarNode (
  PCI_BAR_INFO  *PciBarNodePtr
  )
{
  PciBarNodePtr->NodeIndex = 0;
  PciBarNodePtr->Seg       = 0;
  PciBarNodePtr->Bus       = 0;
  PciBarNodePtr->Dev       = 0;
  PciBarNodePtr->Func      = 0;
  PciBarNodePtr->Offset    = 0;
  PciBarNodePtr->BarType   = PciBarUnknown;
  PciBarNodePtr->MmioBase  = 0;
  PciBarNodePtr->MmioSize  = 0;
}

/**
  If first time call to this library, build a MMIO_ALLOCATION_INFO_HOB and allocate memory pool for Memory Map table, then return to caller.
  else get the MMIO_ALLOCATION_INFO_HOB and return the Memory Map Table address saved in the HOB.

  @param[in] MmioAllocationInfoHob        A pointer to a pointer to MMIO_ALLOCATION_INFO_HOB.
  @param[in] MemoryMapTable               A pointer to a pointer to Memory Map Table.

  @retval  EFI_SUCCESS           This function returned successfully.
  @retval  Others                This function returned with some error.

**/
EFI_STATUS
EFIAPI
PciBarAllocaionSupport (
  IN  OUT  MMIO_ALLOCATION_INFO_HOB  **MmioAllocationInfoHob,
  IN  OUT  MMIO_MAP_INFO             **MemoryMapTable
  )
{
  EFI_STATUS        Status;

  if (MmioAllocationInfoHob == NULL) {
    DEBUG ((EFI_D_ERROR, "Invalid input parameter: MmioAllocationInfoHob!\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (MemoryMapTable == NULL) {
    DEBUG ((EFI_D_ERROR, "Invalid input parameter: MemoryMapTable!\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if MMIO Allocation Info Hob has been created
  //
  Status = PeiServicesGetHobList ((VOID **) MmioAllocationInfoHob);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[ERROR] Get Hob List failed, exit!\n"));
    return Status;
  }

  *MmioAllocationInfoHob = GetNextGuidHob (&gPeiPciMmioResMapHobGuid, *MmioAllocationInfoHob);
  if (*MmioAllocationInfoHob == NULL) {
    Status = PeiServicesCreateHob (EFI_HOB_TYPE_GUID_EXTENSION, sizeof (MMIO_ALLOCATION_INFO_HOB), (VOID **) MmioAllocationInfoHob);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[ERROR] Failed to create MMIO_ALLOCATION_INFO_HOB, exit!\n"));
      return Status;
    }

    //
    // Allocate memory buffer to store MMIO Map Records
    //
    Status = PeiServicesAllocatePool (MEMORY_MAP_TABLE_SIZE, (VOID **)MemoryMapTable);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "[ERROR] Failed to allocate memory map buffer, exit!\n"));
      return Status;
    }

    DEBUG ((EFI_D_VERBOSE, "Successfully allocate memory pool at: 0x%p\n", *MemoryMapTable));

    (*MmioAllocationInfoHob)->EfiHobGuidType.Name = gPeiPciMmioResMapHobGuid;
    (*MmioAllocationInfoHob)->Count = 0;
    (*MmioAllocationInfoHob)->MemoryMapRecordTablePtr = (UINTN) *MemoryMapTable;
    (*MmioAllocationInfoHob)->BarIsCleaned = FALSE;
  } else {
    *MemoryMapTable = (MMIO_MAP_INFO*)(UINTN)(*MmioAllocationInfoHob)->MemoryMapRecordTablePtr;

    DEBUG ((EFI_D_VERBOSE, "Successfully get memory pool address: 0x%p from HOB\n", (*MmioAllocationInfoHob)->MemoryMapRecordTablePtr));
  }

  return EFI_SUCCESS;
}

/**
  This API function is called in PEI Phase to assign MMIO resources(<4GB) to all PCI Devices blong to every IIO Stack.

  This API will be called per every IIO Stack with the Stack's Bus and MMIO resource as input parameters.
  This API need to allocate memory buffer to store the Memory Map Table of each IIO Stack, and return the Memory Map Table address
  along with the number of Memory Map Records to the caller; also need to record the PCI BAR allocation information into a HOB which will be used
  when clean the assigned MMIO resource by this library.

  @param[in]  Segment                      The segment number of this IIO.
  @param[in]  StartBusNo                   The start bus number of this IIO Stack.
  @param[in]  EndBusNo                     The end bus number of this IIO Stack.
  @param[in]  StartMmioAddr                The start address of this IIO Stack's MMIO resource.
  @param[in]  EndMmioAddr                  The end address of this IIO Stack's MMIO resource.
  @param[out] MemoryMapTable               A pointer to a pointer to the Buffer that contains the Memory Map Table.
  @param[out] NumberOfMemoryMapRecords     A pointer to the number of Memory Map Records.

  @retval  EFI_SUCCESS           This function returned successfully.
  @retval  EFI_OUT_OF_RESOURCES  This IIO Stack's MMIO is out of resources.
  @retval  EFI_INVALID_PARAMETER There is invalid input parameter detected.
  @retval  EFI_BUFFER_TOO_SMALL  Either the Hob size or the MemoryMap Buffer size is too small.
  @retval  EFI_ACCESS_DENIED     Cannot proceed to the PCI BAR assignment due to not know the BAR Size.
  @retval  EFI_DEVICE_ERROR      Detect something violate the PCI Specification or pre-assigned MMIO resource has conflict been detected.
  @retval  EFI_NOT_READY         Something is not ready to proceed to the PCI BAR assignment.

**/
EFI_STATUS
EFIAPI
AssignMmio32_PciBusRange (
  IN      UINT16             Segment,
  IN      UINT8              StartBusNo,
  IN      UINT8              EndBusNo,
  IN      UINT32             StartMmioAddr,
  IN      UINT32             EndMmioAddr,
  OUT     MMIO_MAP_INFO      **MemoryMapTable,
  OUT     UINT16             *NumberOfMemoryMapRecords
  )
{
  UINT16                    CurrentBus;
  UINT8                     CurrentDev, CurrentFun;
  UINT16                    RegOffset, MaxBarRegOff;
  UINT16                    VendorID, DeviceID, CmdReg;
  UINT8                     PciHeaderType;
  UINT32                    RemainingMmioStartAddr;
  BOOLEAN                   IsMemorySpaceEnabled, IsBusMasterEnabled, BypassThisBar;
  EFI_STATUS                Status;
  USRA_ADDRESS              UsraPciAddress;
  PCI_BAR_INFO              PciBarNode;
  MMIO_ALLOCATION_INFO_HOB  *MmioAllocationInfoHobPtr;

  if (StartBusNo > EndBusNo) {
    DEBUG ((EFI_D_ERROR, "Invalid input parameter: StartBusNo and EndBusNo!\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (StartMmioAddr == 0) {
    DEBUG ((EFI_D_ERROR, "Invalid input parameter: StartMmioAddr!\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (StartMmioAddr > EndMmioAddr) {
    DEBUG ((EFI_D_ERROR, "Invalid input parameter: StartMmioAddr and EndMmioAddr!\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (MemoryMapTable == NULL) {
    DEBUG ((EFI_D_ERROR, "Invalid input parameter: MemoryMapTable!\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (NumberOfMemoryMapRecords == NULL) {
    DEBUG ((EFI_D_ERROR, "Invalid input parameter: NumberOfMemoryMapRecords!\n"));
    return EFI_INVALID_PARAMETER;
  }

  MmioAllocationInfoHobPtr = NULL;

  Status = PciBarAllocaionSupport (&MmioAllocationInfoHobPtr, MemoryMapTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[ERROR] Can not proceed to the PCI BAR assignment!\n"));

    *MemoryMapTable = NULL;
    *NumberOfMemoryMapRecords = 0;
    return EFI_NOT_READY;
  }

  //
  // parsing pci bar
  //
  InitPciBarNode (&PciBarNode);
  PciBarNode.Seg = Segment;

  RemainingMmioStartAddr = StartMmioAddr;

  //Initialize a Memory Map record for the MMIO resource that is for this Stack
  (*MemoryMapTable)[PciBarNode.NodeIndex].BaseAddress      = StartMmioAddr;
  (*MemoryMapTable)[PciBarNode.NodeIndex].LimitAddress     = EndMmioAddr;
  (*MemoryMapTable)[PciBarNode.NodeIndex].Used             = FALSE;

  if ((RemainingMmioStartAddr & 0xFFF) != 0) {
    RemainingMmioStartAddr = (RemainingMmioStartAddr & ~0xFFF) + 0x1000;

    //Create a Memory Map record for the MMIO resource that is skipped due to 4KB alignment adjust
    (*MemoryMapTable)[PciBarNode.NodeIndex].BaseAddress      = StartMmioAddr;
    (*MemoryMapTable)[PciBarNode.NodeIndex].LimitAddress     = RemainingMmioStartAddr - 1;
    (*MemoryMapTable)[PciBarNode.NodeIndex].Used             = FALSE;

    PciBarNode.NodeIndex++;

    //Create a Memory Map record for the remaining unused MMIO resource
    (*MemoryMapTable)[PciBarNode.NodeIndex].BaseAddress  = RemainingMmioStartAddr;
    (*MemoryMapTable)[PciBarNode.NodeIndex].LimitAddress = EndMmioAddr;
    (*MemoryMapTable)[PciBarNode.NodeIndex].Used         = FALSE;
  }

  for (CurrentBus = StartBusNo; CurrentBus <= EndBusNo; CurrentBus++) {
    for (CurrentDev = 0; CurrentDev <= PCI_MAX_DEVICE; CurrentDev++) {
      for (CurrentFun = 0; CurrentFun <= PCI_MAX_FUNC; CurrentFun++) {
        DEBUG ((EFI_D_VERBOSE, "\nCurrent Seg:0x%x, Bus:0x%x, Dev:0x%x, Fun:0x%x\n", Segment, CurrentBus, CurrentDev, CurrentFun));
        //
        // Check if this PCI Device is present
        //
        USRA_PCIE_SEG_ADDRESS (UsraPciAddress, UsraWidth16, Segment, CurrentBus, CurrentDev, CurrentFun, PCI_VENDOR_ID_OFFSET);
        RegisterRead (&UsraPciAddress, &VendorID);
        DEBUG ((EFI_D_VERBOSE, "Vendor Id: 0x%x\n", VendorID));
        if (VendorID == 0xFFFF) {  //This PCI Device is not present
          if (CurrentFun == 0 ) {  //Go to next device
            break;
          } else {  //Go to next function
            continue;
          }
        }

        UsraPciAddress.Attribute.AccessWidth = UsraWidth16,
        UsraPciAddress.Pcie.Offset = PCI_DEVICE_ID_OFFSET;
        RegisterRead (&UsraPciAddress, &DeviceID);

        //
        // Check the PCI Header Type
        //
        UsraPciAddress.Attribute.AccessWidth = UsraWidth8,
        UsraPciAddress.Pcie.Offset = PCI_HEADER_TYPE_OFFSET;
        RegisterRead (&UsraPciAddress, &PciHeaderType);
        DEBUG ((EFI_D_VERBOSE, "Header Type: 0x%x\n", PciHeaderType));
        if ( (PciHeaderType & HEADER_LAYOUT_CODE) == HEADER_TYPE_DEVICE ) {
          MaxBarRegOff = PCI_BASE_ADDRESSREG_OFFSET + PCI_BAR_IDX5 * sizeof (UINT32);
        } else if ( (PciHeaderType & HEADER_LAYOUT_CODE) == HEADER_TYPE_PCI_TO_PCI_BRIDGE ) {
          MaxBarRegOff = PCI_BASE_ADDRESSREG_OFFSET + PCI_BAR_IDX1 * sizeof (UINT32);
        } else {  //Go to next function
          DEBUG ((EFI_D_VERBOSE, "PCI CardBus Bridge is not supported here!\n"));
          continue;
        }

        UsraPciAddress.Attribute.AccessWidth = UsraWidth16,
        UsraPciAddress.Pcie.Offset = PCI_COMMAND_OFFSET;
        RegisterRead (&UsraPciAddress, &CmdReg);
        DEBUG ((EFI_D_VERBOSE, "Command Register: 0x%x\n", CmdReg));

        IsBusMasterEnabled = FALSE;
        IsMemorySpaceEnabled = FALSE;

        //
        // This is WA for ICX-SP R0 only for now:
        // OOB-MSM func. 2. has BSE/MSE bits set and RO what prevents BAR initialization
        //
        if (IsSiliconWorkaroundEnabled ("S1408726938") && CurrentDev == OOB_MSM_DEV &&
            (CurrentFun == OOB_MSM_FUNC || CurrentFun == OOB_MSM_FUNC_PMON || CurrentFun == OOB_MSM_FUNC_DFX) &&
            VendorID == OOB_MSM_VID && ( DeviceID == OOB_MSM_DID ||  DeviceID == OOB_MSM_DID_NEW)) {

          DEBUG ((EFI_D_INFO, "WA: BME/MSE checking for OOB-MSM dev B/D/F = [%X/%X/%X] skipped.\n",
              CurrentBus, CurrentDev, CurrentFun));
        } else {
          if (CmdReg & EFI_PCI_COMMAND_BUS_MASTER) {
            DEBUG ((EFI_D_VERBOSE, "[WARNING] BME bit is set!\n"));
            IsBusMasterEnabled = TRUE;
          } else {
            if (CmdReg & EFI_PCI_COMMAND_MEMORY_SPACE) {
              DEBUG ((EFI_D_VERBOSE, "[WARNING] MSE bit is set!\n"));
              //
              // Check the MSE bit(Bit 1) in PCI Command Register
              //
              CmdReg &= (UINT16)(~EFI_PCI_COMMAND_MEMORY_SPACE);
              RegisterWrite (&UsraPciAddress, &CmdReg);
              IsMemorySpaceEnabled = TRUE;
            } else {
              DEBUG ((EFI_D_VERBOSE, "    MSE bit is NOT set!\n"));
            }
          }
        }

        PciBarNode.Bus = (UINT8)CurrentBus;
        PciBarNode.Dev = CurrentDev;
        PciBarNode.Func = CurrentFun;

        for (RegOffset = PCI_BASE_ADDRESSREG_OFFSET; RegOffset <= MaxBarRegOff;) {
          DEBUG ((EFI_D_VERBOSE, "BAR Register Offset: %x\n", RegOffset));
          if (IsBusMasterEnabled == TRUE) {
            PciBarNode.Offset = RegOffset;

            Status = IsPciMemBarPreAssigned (&PciBarNode, &RegOffset, StartMmioAddr, EndMmioAddr);

            if (Status == EFI_DEVICE_ERROR) {
              //Status == EFI_DEVICE_ERROR: The BAR register violates the PCI Specification or MMIO conflict detected.
              DEBUG ((EFI_D_ERROR, "[ERROR] Device with BME bit set! Should not go to here!\n"));
              return EFI_DEVICE_ERROR;
            }
            continue;
          }

          BypassThisBar = FALSE;

          PciBarNode.Offset   = RegOffset;
          PciBarNode.BarType  = PciBarUnknown;
          PciBarNode.MmioBase = 0;
          PciBarNode.MmioSize = 0;

          Status = PciBarSizing (&PciBarNode, &RegOffset);

          if (Status == EFI_ABORTED || Status == EFI_UNSUPPORTED) {
            //Status == EFI_ABORTED: Either this BAR is not implemented or this BAR is IO BAR.
            //Status == EFI_UNSUPPORTED: This is 64bit Memory Bar that requests >=4GB MMIO.
            BypassThisBar = TRUE;
          } else if (Status == EFI_DEVICE_ERROR) {
            //Status == EFI_DEVICE_ERROR: The BAR register violates the PCI Specification.
            return EFI_DEVICE_ERROR;
          } else {  //Status == EFI_SUCCESS
            if (IsMemorySpaceEnabled == TRUE) {
              Status = PciCheckBarAdaptive (&PciBarNode, StartMmioAddr, EndMmioAddr);

              if (Status == EFI_NO_MAPPING) {
                //Status == EFI_NO_MAPPING: This BAR is valid but not pre-assigned with MMIO.
                DEBUG ((EFI_D_VERBOSE, "[WARNING] Not pre-assigned with MMIO, need to assign.\n"));
              } else if (Status == EFI_DEVICE_ERROR) {
                //Status == EFI_DEVICE_ERROR: This BAR is pre-assigned with MMIO and resource conflict detected, need to re-assign.
                DEBUG ((EFI_D_VERBOSE, "[Conflict detected] Pre-assigned with MMIO and resource conflict detected, need to re-assign.\n"));
              } else if (Status == EFI_SUCCESS) {
                //Status == EFI_SUCCESS: This BAR is pre-assigned with MMIO and no resource conflict detected.
                DEBUG ((EFI_D_VERBOSE, "[WARNING] This BAR is pre-assigned with MMIO, and no resource conflict detected.\n"));
                BypassThisBar = TRUE;
              } else {
                //Status == EFI_ACCESS_DENIED
                DEBUG ((EFI_D_ERROR, "[ERROR] Device with MSE bit set! Should not go to here!\n"));
                BypassThisBar = TRUE;
              }
            }
          }

          if (BypassThisBar == TRUE) {
            DEBUG ((EFI_D_VERBOSE, "Bypass this BAR, go to next BAR.\n"));
          } else {
            Status = PciAssignBarAdaptive (&PciBarNode, &RemainingMmioStartAddr, EndMmioAddr, *MemoryMapTable, MmioAllocationInfoHobPtr);

            if (Status != EFI_SUCCESS) {
              if (Status == EFI_OUT_OF_RESOURCES) {
                DEBUG ((EFI_D_ERROR, "[Out of resource] Memory left: 0x%x, but asking for: 0x%x\n", EndMmioAddr - RemainingMmioStartAddr + 1, PciBarNode.MmioSize));
                return EFI_OUT_OF_RESOURCES;
              }
              if (Status == EFI_BUFFER_TOO_SMALL) {
                DEBUG ((EFI_D_ERROR, "[Buffer too small] Either Hob size or the MemoryMap Buffer size is too small!\n"));
                return EFI_BUFFER_TOO_SMALL;
              }
              if (Status == EFI_ACCESS_DENIED) {
                DEBUG ((EFI_D_ERROR, "[ERROR] Assign MMIO resource failed! Should not go to here!\n"));
                return EFI_ACCESS_DENIED;
              }
            }
          }
        }

        if (IsMemorySpaceEnabled == TRUE) {
          CmdReg |= EFI_PCI_COMMAND_MEMORY_SPACE;
          RegisterWrite (&UsraPciAddress, &CmdReg);
        }

        //
        // if BX:DX:F0 is single function, then skip function 1~7 to optimize PCI enumeration process
        //
        if ((CurrentFun == 0) && ((PciHeaderType & HEADER_TYPE_MULTI_FUNCTION) == 0)) {  //Single function
          CurrentFun = PCI_MAX_FUNC;
        }
      }
    }
  }

  *NumberOfMemoryMapRecords = PciBarNode.NodeIndex + 1;
  DumpMemoryMapTable (*MemoryMapTable, *NumberOfMemoryMapRecords, MmioAllocationInfoHobPtr);

  return EFI_SUCCESS;
}

