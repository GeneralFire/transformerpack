/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2017 Intel Corporation. <BR>

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

#include <Library/PlatformSetupVariableSyncLib.h>


/*++
    Description:

      This function will parse the variable hob and find three vairables:
      RP variable
      PC common variable
      PC generation variable
      This is used to sync Pc variable to RP variable value

    Arguments:
      PeiServices - PeiServices
      Header -  VARIABLE_STORE_HEADER
      CreateHobDataForRpDefaults - will create a hob for RP defaults,
                                   this is used in normal post case,
                                   cannot be used in specicfic hob event


    Returns:

      EFI_SUCCESS -  Sync to RP variable Success
      Other -Sync to RP variable  Failure


    --*/

EFI_STATUS SyncSetupVariable  (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN OUT VOID* Header,
  IN BOOLEAN CreateHobDataForRpDefaults
)
{
   EFI_STATUS Status = EFI_SUCCESS;



   return Status;

}



/*++
Description:

  This function finds the matched default data and create GUID hob only for RP variable .
  This is used to sync Pc variable to RP variable value

Arguments:

  DefaultId - Specifies the type of defaults to retrieve.
  BoardId   - Specifies the platform board of defaults to retrieve.

Returns:

  EFI_SUCCESS - The matched default data is found.
  EFI_NOT_FOUND - The matched default data is not found.
  EFI_OUT_OF_RESOURCES - No enough resource to create HOB.

--*/

EFI_STATUS
CreateRPVariableHob (
  IN UINT16  DefaultId,
  IN UINT16   BoardId
  ){
   EFI_STATUS Status = EFI_SUCCESS;

   return Status;

  }
