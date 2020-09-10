/** @file

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

#include "CxlDxe.h"

//
// Generic initiator affinity template.
//
EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE mGenericInitiatorTemplate = {
  EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY,                   // Type
  sizeof (EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE),// Length
  0,                                                         // Reserved1
  EFI_ACPI_6_3_PCI_DEVICE_HANDLE,                            // DeviceHandleType
  {0,},                                                      // ProximityDomain
  EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE_ENABLED, // Flags
  {0,}                                                       // Reserved2
};

//
// Memory affinity template.
//
EFI_ACPI_6_3_MEMORY_AFFINITY_STRUCTURE mMemoryTemplate = {
  EFI_ACPI_6_3_MEMORY_AFFINITY,                               // Type
  sizeof (EFI_ACPI_6_3_MEMORY_AFFINITY_STRUCTURE),            // Length
  0,                                                          // ProximityDomain
  0,                                                          // Reserved1
  0,                                                          // AddressBaseLow
  0,                                                          // AddressBaseHigh
  0,                                                          // LengthLow
  0,                                                          // LengthHigh
  0,                                                          // Reserved2
  0,                                                          // Flags
  0                                                           // Reserved3
};


/**
  Calculates next available proximity domain by parsing old table.

  @param[in]  Table                Old SRAT table.
  @param[out] ProximityDomainPtr   Output buffer for next available proximity domain.

  @retval EFI_UNSUPPORTED          The old SRAT table is not formatted correctly.
  @retval EFI_SUCCESS              The next available proximity domain is calculated successfully.

**/
EFI_STATUS
GetNextProximityDomain (
  IN  EFI_ACPI_SDT_HEADER         *Table,
  OUT UINT32                      *ProximityDomainPtr
  )
{
  UINTN                           Offset;
  UINT8                           *Ptr;
  UINT32                          ProximityDomain;
  UINT32                          NextProximityDomain;

  NextProximityDomain = 0;
  ProximityDomain = 0;
  Offset = sizeof (EFI_ACPI_6_3_SYSTEM_RESOURCE_AFFINITY_TABLE_HEADER);
  while ((ProximityDomain < MAX_UINT32) && (Offset <= Table->Length)) {
    if (Offset == Table->Length) {
      *ProximityDomainPtr = NextProximityDomain;
      return EFI_SUCCESS;
    }
    Ptr = (UINT8 *) Table + Offset;
    switch (*Ptr) {
    case EFI_ACPI_6_3_PROCESSOR_LOCAL_APIC_SAPIC_AFFINITY:
      ProximityDomain  = ((EFI_ACPI_6_3_PROCESSOR_LOCAL_APIC_SAPIC_AFFINITY_STRUCTURE *) Ptr)->ProximityDomain7To0;
      ProximityDomain += ((EFI_ACPI_6_3_PROCESSOR_LOCAL_APIC_SAPIC_AFFINITY_STRUCTURE *) Ptr)->ProximityDomain31To8[0] << 8;
      ProximityDomain += ((EFI_ACPI_6_3_PROCESSOR_LOCAL_APIC_SAPIC_AFFINITY_STRUCTURE *) Ptr)->ProximityDomain31To8[1] << 16;
      ProximityDomain += ((EFI_ACPI_6_3_PROCESSOR_LOCAL_APIC_SAPIC_AFFINITY_STRUCTURE *) Ptr)->ProximityDomain31To8[2] << 24;
      break;
    case EFI_ACPI_6_3_MEMORY_AFFINITY:
      ProximityDomain = ((EFI_ACPI_6_3_MEMORY_AFFINITY_STRUCTURE *) Ptr)->ProximityDomain;
      break;
    case EFI_ACPI_6_3_PROCESSOR_LOCAL_X2APIC_AFFINITY:
      ProximityDomain = ((EFI_ACPI_6_3_PROCESSOR_LOCAL_X2APIC_AFFINITY_STRUCTURE *) Ptr)->ProximityDomain;
      break;
    case EFI_ACPI_6_3_GICC_AFFINITY:
      ProximityDomain = ((EFI_ACPI_6_3_GICC_AFFINITY_STRUCTURE *) Ptr)->ProximityDomain;
      break;
    case EFI_ACPI_6_3_GIC_ITS_AFFINITY:
      ProximityDomain = ((EFI_ACPI_6_3_GIC_ITS_AFFINITY_STRUCTURE *) Ptr)->ProximityDomain;
      break;
    case EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY:
      ProximityDomain = ((EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE *) Ptr)->ProximityDomain;
      break;
    default:
      //
      // Make the flag that the proximity domain is invalid and the SRAT table is not supported.
      //
      ProximityDomain = MAX_UINT32;
      break;
    }
    if (ProximityDomain >= NextProximityDomain) {
      NextProximityDomain = ProximityDomain + 1;
    }
    Offset += *(UINT8 *)(Ptr + 1);
  }

  DEBUG ((DEBUG_ERROR, "[CXL] ACPI table %p is not a valid SRAT table!\n", Table));
  return EFI_UNSUPPORTED;
}

/**
  Fills CXL SRAT entry with CXL node info.

  @param[in]  CxlNode             CXL node info data.
  @param[out] Buffer              Pointer to current buffer.

  @return the buffer to the next SRAT table entry to fill.

**/
UINT8 *
FillCxlSratEntry (
  IN  CXL_NODE_DATA               *CxlNode,
  OUT UINT8                       *Buffer
  )
{
  EFI_ACPI_6_3_GENERIC_INITIATOR_AFFINITY_STRUCTURE *GenericInitiatorAffinity;
  EFI_ACPI_6_3_MEMORY_AFFINITY_STRUCTURE            *MemoryAffinity;
  UINT8                                             *Ptr;

  GenericInitiatorAffinity = CopyMem (Buffer, &mGenericInitiatorTemplate, sizeof (mGenericInitiatorTemplate));
  GenericInitiatorAffinity->ProximityDomain = CxlNode->ProximityDomain;
  if (CxlNode->HandleCount > 0) {
    GenericInitiatorAffinity->DeviceHandle.Pci.PciSegment = CxlNode->Handles[0].Segment;
    GenericInitiatorAffinity->DeviceHandle.Pci.PciBdfNumber = CxlNode->Handles[0].Bus;
  } else {
    GenericInitiatorAffinity->Flags = 0;
  }

  Ptr = (UINT8 *)(GenericInitiatorAffinity + 1);
  if ((CxlNode->Info.Attr & CXL_NODE_ATTR_MEM) != 0) {
    MemoryAffinity = CopyMem (Ptr, &mMemoryTemplate, sizeof (mMemoryTemplate));
    MemoryAffinity->ProximityDomain = CxlNode->ProximityDomain;
    MemoryAffinity->AddressBaseLow = CxlNode->Info.Address << CONVERT_64MB_TO_B;
    MemoryAffinity->AddressBaseHigh = CxlNode->Info.Address >> CONVERT_4GB_TO_64MB_GRAN;
    MemoryAffinity->LengthLow = CxlNode->Info.Size << CONVERT_64MB_TO_B;
    MemoryAffinity->LengthHigh = CxlNode->Info.Size >> CONVERT_4GB_TO_64MB_GRAN;
    if ((CxlNode->Info.Attr & CXL_NODE_ATTR_MEM_HW_INIT) != 0) {
      MemoryAffinity->Flags |= EFI_ACPI_6_3_MEMORY_ENABLED;
    }
    if ((CxlNode->Info.Attr & CXL_NODE_ATTR_PERSISTENT) != 0) {
      MemoryAffinity->Flags |= EFI_ACPI_6_3_MEMORY_NONVOLATILE;
    }
    Ptr = (UINT8 *)(MemoryAffinity + 1);
  }

  return Ptr;
}

/**
  Generate new SRAT table.

  This function generates the new SRAT table by appending new generic initiator affinity and
  memory affinity entries.

  @param[in]  OldTable            Old ACPI SRAT table.
  @param[in]  NextProximityDomain Next available proximity domain.
  @param[out] NewTablePtr         Pointer to the new ACPI SRAT table.

**/
EFI_STATUS
GenerateNewSratTable (
  IN  EFI_ACPI_SDT_HEADER         *OldTable,
  IN  UINT32                      NextProximityDomain,
  OUT EFI_ACPI_SDT_HEADER         **NewTablePtr
  )
{
  UINT32                          NewTableLength;
  EFI_ACPI_SDT_HEADER             *NewTable;
  CXL_NODE_DATA                   *CxlNode;
  LIST_ENTRY                      *Link;
  LIST_ENTRY                      *Header;
  UINT8                           *Ptr;

  NewTableLength = OldTable->Length;
  NewTableLength += sizeof (mGenericInitiatorTemplate) * gCxlData.NodeCount;
  NewTableLength += sizeof (mMemoryTemplate) * gCxlData.MemoryNodeCount;
  NewTable = AllocateCopyPool (NewTableLength, OldTable);
  if (NewTable == NULL) {
    DEBUG ((DEBUG_ERROR, "[CXL] Not enough memory for new SRAT table allocation\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Ptr = (UINT8 *) NewTable + OldTable->Length;
  Header = &gCxlData.List;
  for (Link = GetFirstNode (Header); !IsNull (Header, Link); Link = GetNextNode (Header, Link)) {
    CxlNode = CXL_NODE_FROM_LIST (Link);
    CxlNode->ProximityDomain = NextProximityDomain++;
    Ptr = FillCxlSratEntry (CxlNode, Ptr);
  }

  NewTable->Length = NewTableLength;
  NewTable->Revision = EFI_ACPI_6_3_SYSTEM_RESOURCE_AFFINITY_TABLE_REVISION;
  *NewTablePtr = NewTable;
  return EFI_SUCCESS;
}

/**
  Update ACPI SRAT table for CXL node.

  @param[in, out] TablePtr        Pointer to ACPI buffer pointer.

  @retval EFI_SUCCESS             The ACPI table has been updated and saved in TablePtr.
  @retval Others                  Some error happens when update the ACPI table.

**/
EFI_STATUS
UpdateSratTable (
  IN OUT EFI_ACPI_SDT_HEADER      **TablePtr
  )
{
  EFI_ACPI_SDT_HEADER             *OldTable;
  EFI_ACPI_SDT_HEADER             *NewTable;
  UINT32                          NextProximityDomain;
  EFI_STATUS                      Status;

  OldTable = *TablePtr;
  Status = GetNextProximityDomain (OldTable, &NextProximityDomain);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  DEBUG ((DEBUG_INFO, "[CXL] Next available proximity domain is %d\n", NextProximityDomain));

  if (NextProximityDomain > MAX_UINT32 - gCxlData.NodeCount) {
    DEBUG ((DEBUG_ERROR, "[CXL] Cannot add more (%d) proximity domain!\n", gCxlData.NodeCount));
    return EFI_OUT_OF_RESOURCES;
  }

  Status = GenerateNewSratTable (OldTable, NextProximityDomain, &NewTable);
  if (!EFI_ERROR (Status)) {
    *TablePtr = NewTable;
    DEBUG ((DEBUG_INFO, "[CXL] New SRAT ACPI table is updated %p\n", NewTable));
  }

  return Status;
}
