/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include "BzmDxe.h"
#include <Library/PcdLib.h>
#include <Library/ChaLib.h>
#include <Library/BaseLib.h>
#include <Library/MemRcLib.h>
#include <Include/MemMapHost.h>
#include <Protocol/AcpiTable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  This function updates the header information of BZMI ACPI table.

  @param[out] BzmiHeaderPtr            Pointer to the BZMI header.

  @retval EFI_SUCCESS                  This function is executed successfully.
  @retval EFI_INVALID_PARAMETER        Some of input parameters are invalid.

**/
EFI_STATUS
UpdateBzmiAcpiHeader (
  OUT EFI_ACPI_DESCRIPTION_HEADER  *BzmiHeaderPtr
  )
{
  UINT64   OemTableId;

  if (BzmiHeaderPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  OemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);

  BzmiHeaderPtr->Signature = BOOTTIME_ZEROED_MEMORY_INTERFACE_TABLE_SIGNATURE;
  BzmiHeaderPtr->Length    = sizeof (BOOTTIME_ZEROED_MEMORY_INTERFACE);
  BzmiHeaderPtr->Revision  = BOOTTIME_ZEROED_MEMORY_INTERFACE_TABLE_REVISION;

  CopyMem (BzmiHeaderPtr->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (BzmiHeaderPtr->OemId));
  CopyMem (&BzmiHeaderPtr->OemTableId, &OemTableId, sizeof (BzmiHeaderPtr->OemTableId));

  BzmiHeaderPtr->OemRevision      = PcdGet32 (PcdAcpiDefaultOemRevision);
  BzmiHeaderPtr->CreatorId        = PcdGet32 (PcdAcpiDefaultCreatorId);
  BzmiHeaderPtr->CreatorRevision  = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  return EFI_SUCCESS;
}

/**
  This function updates the Body fields of BZMI ACPI table.

  @param[in,out] BzmiTablePtr          Pointer to the BZMI table.

  @retval EFI_SUCCESS                  This function is executed successfully.
  @retval EFI_INVALID_PARAMETER        Some of input parameters are invalid.

**/
EFI_STATUS
UpdateBzmiAcpiBody (
  IN OUT BOOTTIME_ZEROED_MEMORY_INTERFACE   *BzmiTablePtr
  )
{
  UINT8               SocketId;
  UINT8               ClusterId;
  UINT8               DomainNum;
  UINT64              DomainHandle;
  EFI_STATUS          Status;
  MEMORY_MAP_HOST     *MemMapHost;

  if (BzmiTablePtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DomainNum = 0;
  MemMapHost = GetMemMapHostPointer ();

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!CheckSocketEnabled (SocketId)) {
      continue;
    }

    for (ClusterId = 0; ClusterId < MemMapHost->NumberofChaDramClusters; ClusterId++) {
      Status = ChaGetClusterDomainHandle (SocketId, ClusterId, &DomainHandle);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR,
          "[BZM] Socket %d, Cluster %d, Failed to get the dommain handle, Status: %r\n",
          SocketId,
          ClusterId,
          Status
          ));
        return Status;
      }

      BzmiTablePtr->DomainIdentifier[DomainNum] = DomainHandle;
      DomainNum++;
    }
  }

  BzmiTablePtr->DomainNumber = DomainNum;

  return EFI_SUCCESS;
}

/**
  This function displays the information of BZMI ACPI table.

  @param[in,out] BzmiTablePtr     Pointer to the BZMI table.

**/
VOID
DisplayBzmiAcpiTable (
  IN BOOTTIME_ZEROED_MEMORY_INTERFACE   *BzmiTablePtr
  )
{
  UINT8    DomainId;

  DEBUG ((DEBUG_INFO, "[BZM] ********* BZMI ACPI table *********\n"));
  DEBUG ((DEBUG_INFO, "[BZM] Length : 0x%x\n", BzmiTablePtr->Header.Length));
  DEBUG ((DEBUG_INFO, "[BZM] Revision:  0x%x\n", BzmiTablePtr->Header.Revision));
  DEBUG ((DEBUG_INFO, "[BZM] Checksum: 0x%x\n", BzmiTablePtr->Header.Checksum));
  DEBUG ((DEBUG_INFO, "[BZM] OemId: %a\n", BzmiTablePtr->Header.OemId));
  DEBUG ((DEBUG_INFO, "[BZM] OemTableId: 0x%lx\n", BzmiTablePtr->Header.OemTableId));
  DEBUG ((DEBUG_INFO, "[BZM] OemRevision: 0x%x\n", BzmiTablePtr->Header.OemRevision));
  DEBUG ((DEBUG_INFO, "[BZM] CreatorId: 0x%x\n", BzmiTablePtr->Header.CreatorId));
  DEBUG ((DEBUG_INFO, "[BZM] CreatorRevision: 0x%x\n", BzmiTablePtr->Header.CreatorRevision));
  DEBUG ((DEBUG_INFO, "[BZM] DomainNumber: %d\n", BzmiTablePtr->DomainNumber));

  for (DomainId = 0; DomainId < BzmiTablePtr->DomainNumber; DomainId++) {
    DEBUG ((DEBUG_INFO, "[BZM] Domain-%d Identifier: 0x%lx\n", DomainId, BzmiTablePtr->DomainIdentifier[DomainId]));
  }
}

/**
  This function installs the BZMI ACPI table.

  @retval EFI_SUCCESS     This function is executed successfully.
  @retval Others          This fuction failed to be executed.

**/
EFI_STATUS
InstallBzmiAcpiTable (
  VOID
  )
{
  UINTN                             BzmiAcpiTableKey;
  EFI_STATUS                        Status;
  EFI_ACPI_TABLE_PROTOCOL           *AcpiTableProtocol;
  BOOTTIME_ZEROED_MEMORY_INTERFACE  BzmiAcpiTable;

  //
  // Locate ACPI table protocol.
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTableProtocol);
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize BZMI ACPI table.
  //
  ZeroMem (&BzmiAcpiTable, sizeof (BOOTTIME_ZEROED_MEMORY_INTERFACE));

  //
  // Update the Header information of BZMI ACPI table.
  //
  Status = UpdateBzmiAcpiHeader (&BzmiAcpiTable.Header);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Update the Body fields of BZMI ACPI table.
  //
  Status = UpdateBzmiAcpiBody (&BzmiAcpiTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Calculate CheckSum of BZMI ACPI table.
  //
  BzmiAcpiTable.Header.Checksum = CalculateCheckSum8 ((UINT8 *) &BzmiAcpiTable, BzmiAcpiTable.Header.Length);

  //
  // Publish BZMI ACPI table.
  //
  BzmiAcpiTableKey = 0;
  Status = AcpiTableProtocol->InstallAcpiTable (
                                AcpiTableProtocol,
                                &BzmiAcpiTable,
                                BzmiAcpiTable.Header.Length,
                                &BzmiAcpiTableKey
                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[BZM] Failed to install BZMI ACPI table, Status: %r\n", Status));
    return Status;
  }

  //
  // Display BZMI ACPI table.
  //
  DisplayBzmiAcpiTable (&BzmiAcpiTable);
  DEBUG ((DEBUG_INFO, "[BZM] BZMI ACPI table has been installed successfully\n"));

  return EFI_SUCCESS;
}
