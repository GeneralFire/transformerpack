/** @file
  This file provides service functions for NUMA ACPI table data.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/NumaAcpiTableLib.h>

/**
  This function sets the NUMA ACPI table data buffer pointer.

  @retval EFI_SUCCESS              This function is executed successfully.
  @retval EFI_BAD_BUFFER_SIZE      Failed to allocate memory for NUMA ACPI data.
**/
EFI_STATUS
SetNumaAcpiDataPointer (
  VOID
  )
{
  EFI_STATUS   Status;
  UINTN        BufferSize;
  VOID         *NumaAcpiDataPtr;

  BufferSize = sizeof (NUMA_ACPI_DATA);
  NumaAcpiDataPtr = AllocatePages (EFI_SIZE_TO_PAGES (BufferSize));
  if (NumaAcpiDataPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate memory for NUMA ACPI data\n"));
    return EFI_BAD_BUFFER_SIZE;
  }

  ZeroMem (NumaAcpiDataPtr, BufferSize);

  Status = PcdSet64S (PcdNumaAcpiDataStaticPointer, (UINT64) NumaAcpiDataPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to set the PCD token with NUMA ACPI data pointer\n"));
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  This function retrieves the pointer of NUMA ACPI table data buffer.
  If the buffer pointer has not been set yet, this function will allocate
  a buffer and set the buffer pointer. NULL will be returned if the pointer
  is failed to be set.

  @retval Pointer to the NUMA ACPI table data buffer or NULL.
**/
VOID*
GetNumaAcpiDataPointer (
  VOID
  )
{
  EFI_STATUS   Status;

  if (PcdGet64 (PcdNumaAcpiDataStaticPointer) == 0) {
    Status = SetNumaAcpiDataPointer ();
    if (EFI_ERROR (Status)) {
      return NULL;
    }
  }

  return (VOID *) (UINTN) PcdGet64 (PcdNumaAcpiDataStaticPointer);
}

/**
  This function gets the number of assigned proximity domains from the NUMA
  ACPI table data buffer.

  @retval Number of assigned proximity domains.
**/
UINT32
EFIAPI
GetProximityDomainNum (
  VOID
  )
{
  NUMA_ACPI_DATA      *NumaAcpiData;

  NumaAcpiData = GetNumaAcpiDataPointer ();
  if (NumaAcpiData == NULL) {
    return 0;
  }

  return NumaAcpiData->ProximityDomainNum;
}

/**
  This function retrieves the information of the specified proximity domain
  from the NUMA ACPI table data buffer. NULL will be returned if the specified
  domain index exceeds the allowed maximum number or the data buffer pointer can
  not be located.

  @param[in] DomainId      Proximity domain index.

  @retval Pointer to the specified proximity domain data buffer or NULL.
**/
PROXIMITY_DOMAIN*
EFIAPI
GetProximityDomain (
  IN UINT32        DomainId
  )
{
  NUMA_ACPI_DATA   *NumaAcpiData;

  if (DomainId >= MAX_NUMA_PROXIMITY_DOMAIN) {
    return NULL;
  }

  NumaAcpiData = GetNumaAcpiDataPointer ();
  if (NumaAcpiData == NULL) {
    return NULL;
  }

  return &NumaAcpiData->ProximityDomainList[DomainId];
}

/**
  This function adds the system resource information into the proximity domain.
  A new proximity domain will be created if the specified one is uninitialized.
  Otherwise, the specified proximity domain will be updated to include the new
  resource information.

  @param[in] DomainId            Proximity domain index.
  @param[in] SocketId            Socket index.
  @param[in] ClusterId           Cluster index within socket.
  @param[in] DomainAttr          Proximity domain attribute.
  @param[in] DomainData          Pointer to the proximity domain data to be added.

  @retval EFI_SUCCESS            This function is executed successfully.
  @retval EFI_LOAD_ERROR         Failed to get the proximity domain data buffer pointer.
  @retval EFI_ACCESS_DENIED      Updating is denied due to socket and/or cluster mismatched.
  @retval EFI_INVALID_PARAMETER  Some of the input parameters are invalid.
**/
EFI_STATUS
AddProximityDomain (
  IN UINT32                    DomainId,
  IN UINT8                     SocketId,
  IN UINT8                     ClusterId,
  IN PROXIMITY_DOMAIN_ATTR     DomainAttr,
  IN VOID                      *DomainData
  )
{
  PROXIMITY_DOMAIN             *ProximityDomain;

  if ((DomainId >= MAX_NUMA_PROXIMITY_DOMAIN) || (DomainData == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ProximityDomain = GetProximityDomain (DomainId);
  if (ProximityDomain == NULL) {
    return EFI_LOAD_ERROR;
  }

  //
  // Check if the specified socket index and cluster index match the ones of specified proximity
  // domain when the specified proximity domain has been initialized.
  //   1. Deny the proximity domain update if mismatched.
  //   2. Update socket index and cluster index if uninitialized.
  //
  if (ProximityDomain->Attr != 0) {
    if ((ProximityDomain->SocketId != SocketId) || (ProximityDomain->ClusterId != ClusterId)) {
      DEBUG ((DEBUG_ERROR, "Socket and/or cluster mismatched when updating proximity domain: "));
      DEBUG ((DEBUG_ERROR, "SocketId=%d, ClusterId=%d, Proximity domain index=%d socket=%d cluster=%d\n",
        SocketId,
        ClusterId,
        DomainId,
        ProximityDomain->SocketId,
        ProximityDomain->ClusterId
        ));
      return EFI_ACCESS_DENIED;
    }
  } else {
    ProximityDomain->SocketId  = SocketId;
    ProximityDomain->ClusterId = ClusterId;
  }


  switch (DomainAttr) {

  case PROXIMITY_DOMAIN_PROCESSOR:
    ProximityDomain->ProcessorDomain = *((PROCESSOR_PROXIMITY_DOMAIN *) DomainData);
    break;

  case PROXIMITY_DOMAIN_MEMORY:
    ProximityDomain->MemoryDomain = *((MEMORY_PROXIMITY_DOMAIN *) DomainData);
    break;

  default:
    DEBUG ((DEBUG_ERROR, "Invalid proximity domain attribute: 0x%x\n", DomainAttr));
    return EFI_INVALID_PARAMETER;
  }

  ProximityDomain->Attr |= DomainAttr;

  return EFI_SUCCESS;
}
