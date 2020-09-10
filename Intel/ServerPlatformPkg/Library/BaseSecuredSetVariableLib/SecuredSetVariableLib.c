/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/SecuredSetVariableLib.h>

SECURED_VARIABLE_ENTRY mSecuredVariableList[] = {
  {L"PchInit",           &gPchInitVariableGuid,         CheckPoint_EndOfDxe},
  {L"ReserveMemFlag",    &gReserveMemFlagVariableGuid,  CheckPoint_EndOfDxe},
  {NULL,                 NULL,                          CheckPoint_NotSecured}
};

/**
  Checks whether the variable is a secured variable.

  @param  VariableName      The name of the variable.
  @param  VariableGuid      The vendor GUID of the variable.
  @param  CheckPoint        The variable check point.

  @retval TRUE              The variable is a secured variable.
  @retval FALSE             The variable is not a secured variable.

**/
BOOLEAN
EFIAPI
IsSecuredVariable (
  IN CHAR16      *VariableName,
  IN GUID        *VariableGuid,
  IN UINT8       CheckPoint
  )
{
  SECURED_VARIABLE_ENTRY   *VariablePtr;

  for (VariablePtr = mSecuredVariableList; VariablePtr->VariableName != NULL; VariablePtr++) {
    if ((VariablePtr->CheckPoint == CheckPoint) &&
        (StrCmp (VariablePtr->VariableName, VariableName) == 0) &&
         CompareGuid(VariablePtr->VariableGuid, VariableGuid)) {
       return TRUE;
    }
  }

  return FALSE;
}