/** @file
  SpsDxe Dependency Check

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

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/SpsDxeLib.h>
#include <SpsDxeDependencyCheck.h>

extern SPS_DEPENDENCY_CHECK CheckTable[];

/**
  Function verifies dependency rules

  @param[in] Callback          StartAction callback function pointer.
                               If dependency is not satisfied event callback is set
                               to verify it again.
                               Context of the callback is EFI_STATUS pointer

  @retval TRUE                 All dependency elements has been found
  @retval FALSE                Not all dependency elements has been found
**/
BOOLEAN
VerifyDependencyRules (
  IN SPS_DXE_START_ACTION_CALLBACK  Callback
  )
{
  UINT8                   Index;
  EFI_STATUS              Status;
  VOID                    *Protocol;
  BOOLEAN                 DependencyResult;
  IN SPS_DEPENDENCY_CHECK *ElementToCheck;
  EFI_EVENT               Event;

  if (CheckTable == NULL) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: %a - wrong input pointer\n", __FUNCTION__));
    return FALSE;
  }
  DependencyResult = TRUE;
  DEBUG ((DEBUG_INFO, "[SPS] Dependency list status:\n"));
  DEBUG ((DEBUG_INFO, "===============================================\n"));
  Index = 0;
  ElementToCheck = CheckTable;
  while ((ElementToCheck != NULL) &&
         (ElementToCheck->GuidToVerify != NULL) &&
         (ElementToCheck->Text != NULL)) {
    if (EFI_ERROR (ElementToCheck->Status)) {
      ElementToCheck->Status = gBS->LocateProtocol (ElementToCheck->GuidToVerify, NULL, &Protocol);
    }
    DEBUG ((DEBUG_INFO, "\t GUID : %a : %r\n", ElementToCheck->Text, ElementToCheck->Status));
    if (EFI_ERROR (ElementToCheck->Status)) {
      DependencyResult = FALSE;
      if (Callback != NULL) {
        Event = NULL;
        Status = gBS->CreateEventEx (
                        EVT_NOTIFY_SIGNAL,
                        TPL_CALLBACK,
                        Callback,
                        &ElementToCheck->Status,
                        ElementToCheck->GuidToVerify,
                        &Event
                        );
        ASSERT_EFI_ERROR (Status);
      }
    }
    Index++;
    ElementToCheck = CheckTable + Index;
  }
  DEBUG ((DEBUG_INFO, "===============================================\n"));
  DEBUG ((
    DEBUG_INFO,
    "[SPS] %d dependency check result: %a\n",
    Index,
    DependencyResult == 1 ? "SUCCESS" : "FAILED"
    ));
  DEBUG ((DEBUG_INFO, "===============================================\n"));

  return DependencyResult;
}
