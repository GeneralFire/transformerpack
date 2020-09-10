/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2019 Intel Corporation. <BR>

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

#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/RasDebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SmmVariable.h>
#include <Protocol/VariableLock.h>
#include "PartialMirrorHandler.h"


EFI_STATUS
EFIAPI
InitPartialMirrorHandler (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
/*++

Routine Description:

  Initializes the Partial Mirror.

Arguments:

Returns:

  EFI_OUT_OF_RESOURCES  -  Insufficient resources to complete function.
  EFI_SUCCESS           -  Function has completed successfully.
  Other                 -  Error occured during execution.

--*/
{
  EFI_STATUS                          Status = EFI_SUCCESS;
  EFI_SMM_VARIABLE_PROTOCOL           *SmmVariable = NULL;
  EDKII_VARIABLE_LOCK_PROTOCOL        *VariableLock = NULL;
  ADDR_RANGE_MIRROR_VARIABLE_DATA     *MemoryCurrent = NULL;
  MEM_TOPOLOGY                        *MemTopology;
  UINTN                               AddressBasedMirrorDataSize = 0;
  UINT32                              AddressBaseMirrorAttribute = ADDRESS_BASED_MIRROR_VARIABLE_ATTRIBUTE;

  //
  // Get the Partial Mirror Info from Memory Topology.
  // Dont forget to update this structure after mem hotAdd/hotRemove/Migration otherwise PatrolScrub and MemCopy engine would not work correctly
  //
  MemTopology = GetMemTopology();

  //
  // Create MemoryCurrent variable
  //
  Status = gSmst->SmmLocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, (VOID**)&SmmVariable);
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "%a:Locate gEfiSmmVariableProtocolGuid failed, Status=%r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **) &VariableLock);
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "%a:Locate gEdkiiVariableLockProtocolGuid failed, Status=%r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  AddressBasedMirrorDataSize = sizeof(MemTopology->MirrorCurrentType);
  MemoryCurrent = &(MemTopology->MirrorCurrentType);
  Status = SmmVariable->SmmSetVariable (
                           ADDRESS_RANGE_MIRROR_VARIABLE_CURRENT,
                           &gAddressBasedMirrorGuid,
                           AddressBaseMirrorAttribute,
                           AddressBasedMirrorDataSize,
                           MemoryCurrent
                           );
  if (EFI_ERROR (Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "%a: Set variable gAddressBasedMirrorGuid failed, Status=%r\n", __FUNCTION__, Status));
    ASSERT_EFI_ERROR (Status);
  }
  if (Status == EFI_SUCCESS) {
    RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryCurrent->MirrorVersion %x\n", MemoryCurrent->MirrorVersion));
    RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryCurrent->MirroredAmountAbove4GB 0x%x\n", MemoryCurrent->MirroredAmountAbove4GB));
    RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryCurrent->MirrorMemoryBelow4GB 0x%x\n", MemoryCurrent->MirrorMemoryBelow4GB));
    RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryCurrent->MirrorStatus %x\n", MemoryCurrent->MirrorStatus));

    RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryRequest->MirrorVersion %x\n", MemTopology->MirrorRequestType.MirrorVersion));
    RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryRequest->MirroredAmountAbove4GB 0x%x\n", MemTopology->MirrorRequestType.MirroredAmountAbove4GB));
    RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryRequest->MirrorMemoryBelow4GB 0x%x\n", MemTopology->MirrorRequestType.MirrorMemoryBelow4GB));
    RAS_DEBUG((LEVEL_FUNC_FLOW, "MemoryRequest->MirrorStatus %x\n", MemTopology->MirrorRequestType.MirrorStatus));

    //
    // Lock this variable to prevent attempting to changed by malicious SW.
    //
    Status = VariableLock->RequestToLock (VariableLock, ADDRESS_RANGE_MIRROR_VARIABLE_CURRENT, &gAddressBasedMirrorGuid);
    if (EFI_ERROR (Status)) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "%a: Request to lock Address range mirror variable failed, Status=%r\n", __FUNCTION__, Status));
      ASSERT_EFI_ERROR (Status);
    }
  }

  return Status;
}

