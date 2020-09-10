/** @file
  Source code file for the implementation of DxePciMmio32AssignLib

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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
  This API function is called to clean the assigned MMIO resource by this library.

  @return  EFI_SUCCESS           This function returned successfully.
**/
VOID
EFIAPI
CleanAssignedTempMmio(
  VOID
  )
{
  UINT32                    NodeIndex;
  UINT32                    WriteValue;
  UINT8                     TempBus, TempDev, TempFunc;
  UINT16                    TempSeg, TempOffset;
  USRA_ADDRESS              UsraPciAddress;
  PCI_BAR_NODE_TYPE         TempBarType;
  MMIO_ALLOCATION_INFO_HOB  *MmioAllocationInfoHobPtr;

  WriteValue = 0;
  MmioAllocationInfoHobPtr = NULL;

  MmioAllocationInfoHobPtr = (MMIO_ALLOCATION_INFO_HOB*) GetFirstGuidHob (&gPeiPciMmioResMapHobGuid);

  if (MmioAllocationInfoHobPtr != NULL) {

    if (MmioAllocationInfoHobPtr->BarIsCleaned == TRUE) {
      return;
    }

    DEBUG ((EFI_D_VERBOSE, "\n====  Clean the temp allocated MMIO resource - Begin  ====\n\n"));
    DEBUG ((EFI_D_VERBOSE, "  Number of allocated BAR: %04d\n", MmioAllocationInfoHobPtr->Count));

    for (NodeIndex =0; NodeIndex < MmioAllocationInfoHobPtr->Count; NodeIndex++ ) {
      TempSeg     = MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].Seg;
      TempBus     = MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].Bus;
      TempDev     = MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].Dev;
      TempFunc    = MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].Func;
      TempOffset  = MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].Offset;
      TempBarType = MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].BarType;

      USRA_PCIE_SEG_ADDRESS (UsraPciAddress, UsraWidth32, TempSeg, TempBus, TempDev, TempFunc, TempOffset);
      RegisterWrite (&UsraPciAddress, &WriteValue);

      if ( (ReadBarRegister (&MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex]) & PCI_MEM_BAR_BASE_ADDR_MASK) == MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].MmioBase ) {
        DEBUG ((EFI_D_ERROR, "    [PciParseBar] Failed to clear the BAR Register!!!\n"));
      }

      if (TempBarType == PciBarMem64) {
        UsraPciAddress.Pcie.Offset += 4;
        RegisterWrite (&UsraPciAddress, &WriteValue);
      }

      DEBUG ((EFI_D_VERBOSE, "    [%04d]: Successfully cleaned for [Seg:0x%x Bus:0x%x Dev:0x%x Func:0x%x Offset:0x%x]\n",
        NodeIndex,
        MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].Seg,
        MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].Bus,
        MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].Dev,
        MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].Func,
        MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].Offset
      ));

      DEBUG ((EFI_D_VERBOSE, "            %S,    MmioBase:0x%x,    MmioSize:0x%x\n",
        MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].BarType == PciBarMem32?L"32-Bit Memory BAR":L"64-Bit Memory BAR",
        MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].MmioBase,
        MmioAllocationInfoHobPtr->PciBarInfo[NodeIndex].MmioSize
      ));
    }
    DEBUG ((EFI_D_VERBOSE, "\n====  Clean the temp allocated MMIO resource - End  ====\n\n"));


    MmioAllocationInfoHobPtr->BarIsCleaned = TRUE;
    MmioAllocationInfoHobPtr->Count = 0;
  }

  return;
}

