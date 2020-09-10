/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
// Module global data
//
CXL_DATA gCxlData = {
  CXL_DATA_SIGNATURE,                 // Signature
  0,                                  // NodeCount
  0,                                  // MemoryNodeCount
  {NULL, NULL}                        // List
};

//
// Module private data
//
EFI_EVENT                  mEvent = NULL;
EFI_ACPI_SDT_PROTOCOL      *mAcpiSdt = NULL;
EFI_ACPI_TABLE_PROTOCOL    *mAcpiTable = NULL;

/**
  Initializes CXL node private data structure

  @param[in] CxlNodeHob           Pointer to CXL node HOB.

  @retval EFI_SUCCESS             CXL node private data structure is initialized successfully.
  @retval Others                  Failure to initialize private data structure (e.g. due to lack of memory resources).

**/
EFI_STATUS
InitializeCxlNodeData (
  IN CXL_NODE_SOCKET       *CxlNodeHob
  )
{
  UINT8                    SocketId;
  UINT8                    CxlNodeId;
  CXL_NODE_DATA            *CxlNodeData;
  EFI_STATUS               Status;

  InitializeListHead (&gCxlData.List);
  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    for (CxlNodeId = 0; CxlNodeId < CxlNodeHob[SocketId].CxlNodeCount; CxlNodeId++) {
      Status = gBS->AllocatePool (EfiBootServicesData, sizeof (CXL_NODE_DATA), (VOID **) &CxlNodeData);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }
      CxlNodeData = ZeroMem (CxlNodeData, sizeof (CXL_NODE_DATA));
      CxlNodeData->Signature = CXL_NODE_SIGNATURE;
      CopyMem (&CxlNodeData->Info, &CxlNodeHob[SocketId].CxlNodeInfo[CxlNodeId], sizeof (CXL_NODE_INFO));
      CxlNodeData->SocketId = SocketId;
      InsertTailList (&gCxlData.List, &CxlNodeData->ListEntry);
      gCxlData.NodeCount++;
      if ((CxlNodeData->Info.Attr & CXL_NODE_ATTR_MEM) != 0) {
        gCxlData.MemoryNodeCount++;
      }
    }
  }

  return EFI_SUCCESS;
}


/**
  Reinstalls ACPI table if the specified ACPI table has been updated successfully.

  @param[in] Signature       The signature of ACPI table.
  @param[in] Handler         The routine to handle the corresponding ACPI table.

**/
EFI_STATUS
UpdateAcpiTable (
  IN UINT32                  Signature,
  IN CXL_ACPI_TABLE_HANDLER  Handler
  )
{
  EFI_ACPI_TABLE_VERSION     Version;
  EFI_ACPI_SDT_HEADER        *Table;
  EFI_STATUS                 Status;
  UINTN                      TableIndex;
  UINTN                      TableKey;

  TableIndex = 0;
  while (TRUE) {
    Status = mAcpiSdt->GetAcpiTable (TableIndex++, &Table, &Version, &TableKey);
    if (EFI_ERROR (Status)) {
      break; // No more ACPI SDT table to find
    }
    if (Table->Signature == Signature) {
      Status = Handler (&Table);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[CXL] Failed to update ACPI Table %x, Status %r\n", Signature, Status));
        return Status;
      }
      Status = mAcpiTable->UninstallAcpiTable (mAcpiTable, TableKey);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[CXL] Failed to remove old ACPI Table %x, Status %r\n", Signature, Status));
        return Status;
      }
      Table->Checksum = 0;
      Table->Checksum = CalculateCheckSum8 ((UINT8 *) Table, Table->Length);
      Status = mAcpiTable->InstallAcpiTable (mAcpiTable, Table, Table->Length, &TableKey);
      DEBUG ((DEBUG_INFO, "[CXL] Install new ACPI Table %x, Status %r\n", Signature, Status));
      FreePool (Table);
      return Status;
    }
  }
  DEBUG ((DEBUG_ERROR, "[CXL] Failed to get the original ACPI Table %x, Status %r\n", Signature, Status));
  return Status;
}

/**
  CXL late initialization call back function.

  This function is triggered after PCI enumeration and ACPI tables have been published.
  It updates the original ACPI tables to attach CXL specific ACPI table requirements.

  @param Event                    The instance of the event triggering this function.
  @param Context                  Pointer to Context, not used.

**/
VOID
EFIAPI
UpdateCxlAcpiTablesCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS      Status;

  gBS->CloseEvent (mEvent);

  CalculateCxlPciHandles ();

  Status = UpdateAcpiTable (EFI_ACPI_6_3_SYSTEM_RESOURCE_AFFINITY_TABLE_SIGNATURE, UpdateSratTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[CXL] Failed to update SRAT table %r\n", Status));
  }

  return;
}


/**
  Standard EFI driver point.

  This driver publish CXL UEFI memory map and publish CXL related ACPI tables.

  @param ImageHandle         -  A handle for the image that is initializing this driver.
  @param SystemTable         -  A pointer to the EFI system table.

  @retval EFI_SUCCESS        -  Driver initialized successfully.
  @retval Others             -  Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
CxlDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_HOB_GUID_TYPE     *GuidHob;
  CXL_NODE_SOCKET       *CxlNodeHob;

  GuidHob = GetFirstGuidHob (&gCxlNodeHobGuid);
  if (GuidHob != NULL) {
    DEBUG ((DEBUG_INFO, "[CXL] CXL device(s) are present\n"));
    CxlNodeHob = GET_GUID_HOB_DATA (GuidHob);

    ConstructCxlUefiMemoryMap (CxlNodeHob);

    Status = gBS->LocateProtocol (&gEfiAcpiSdtProtocolGuid, NULL, (VOID **) &mAcpiSdt);
    if (EFI_ERROR (Status)) {
      ASSERT (FALSE);  // Protocol is in driver dependency expression.
      return Status;
    }

    Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &mAcpiTable);
    if (EFI_ERROR (Status)) {
      ASSERT (FALSE); // Protocol is in driver dependency expression.
      return Status;
    }

    Status = InitializeCxlNodeData (CxlNodeHob);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    EfiCreateEventReadyToBootEx (TPL_CALLBACK, UpdateCxlAcpiTablesCallback, NULL, &mEvent);
  }
  return EFI_SUCCESS;
}
