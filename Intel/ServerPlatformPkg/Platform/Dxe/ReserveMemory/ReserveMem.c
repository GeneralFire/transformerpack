/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2018 Intel Corporation. <BR>

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

#include "ReserveMem.h"
#include <Library/SetupLib.h>
#include <Guid/SetupVariable.h>

/**

  This function will reserve the memory page specified by the setup question.

  @param VOID

  @retval EFI_SUCCESS  -  Policy decisions set.

**/
EFI_STATUS
SetupReserveMemory (
  VOID
)
{
  EFI_STATUS                            Status;
  UINTN                                 VarSize;
  EFI_PHYSICAL_ADDRESS                  ReservedAddr;
  BOOLEAN                               ReserveMemFlag = FALSE;
  BOOLEAN                               ReserveMemFlagTemp;
  UINT8                                 ReserveMem;
  UINT64                                ReserveStartAddr;

  Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, ReserveMem), &ReserveMem, sizeof(UINT8));
  if (EFI_ERROR(Status)) {
    ReserveMem = 0;
  }

  Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, ReserveStartAddr), &ReserveStartAddr, sizeof(UINT64));
  if (EFI_ERROR(Status)) {
    ReserveStartAddr = 0;
  }

  if(ReserveMem) {

    ReservedAddr  = (EFI_PHYSICAL_ADDRESS) ReserveStartAddr;


    //
    //  Make sure page is alligned
    //
    if((ReservedAddr % EFI_PAGE_SIZE) != 0) {
      ReservedAddr += (EFI_PAGE_SIZE - (ReservedAddr % EFI_PAGE_SIZE));
    }

    //
    //  Try to allocate the address specified by SystemConfiguration.ReserveStartAddr.
    //
    Status = gBS->AllocatePages (
      AllocateAddress,
      EfiReservedMemoryType,
      EFI_SIZE_TO_PAGES (EFI_PAGE_SIZE),
      &ReservedAddr
    );

    if(Status == EFI_SUCCESS) {
      DEBUG((EFI_D_INFO, "SetupReserveMemory(): Page of memory reserved at 0x%x\n", (UINT64) ReservedAddr));
      ReserveMemFlag = TRUE;
    }
    else {
      //
      //  No memory could be reserved, disable the functionality
      //
      DEBUG((EFI_D_ERROR, "SetupReserveMemory(): Failed to reserve a page of memory.\n"));
      ReserveMemFlag = FALSE;
    }
  }

  //
  //  Save Reserve memory result if the GetVariable error or value gotten is different.
  //
  VarSize = 1;
  Status = gRT->GetVariable (L"ReserveMemFlag",
    &gReserveMemFlagVariableGuid,
    NULL,
    &VarSize,
    &ReserveMemFlagTemp);

  if ( ( Status != EFI_SUCCESS ) || ( ReserveMemFlag != ReserveMemFlagTemp ) ) {
    Status = gRT->SetVariable (
      L"ReserveMemFlag",
      &gReserveMemFlagVariableGuid,
      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
      sizeof(BOOLEAN),
      &ReserveMemFlag
    );
  }

  return Status;
}

/**

  Reserve 16M for TAGEC

  @param None.

  @retval None.

**/
EFI_STATUS
ReserveTagecMem (
  VOID
)
{
  EFI_STATUS                            Status;
  EFI_PHYSICAL_ADDRESS                  TagecAddr = 0x0F000000;
  UINT8                                 count;
  UINT8                                 TagecMem;

  Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, TagecMem), &TagecMem, sizeof(UINT8));

  if (EFI_ERROR(Status)) {
    return EFI_UNSUPPORTED;
  }

  if(TagecMem == 1) {
    //
    //  Try to allocate memory starting at 0xF000000.
    //
    for(count = 0; count < 4; count++) {
      Status = gBS->AllocatePages (
        AllocateAddress,
        EfiReservedMemoryType,
        EFI_SIZE_TO_PAGES (SIZE_16MB),
        &TagecAddr
      );
      if (EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR, "Unable to reserve memory for TAGEC at 0x%x!\n", TagecAddr));  //Unable to reserve 16MB for TAGEC!
        TagecAddr =+ SIZE_16MB;
      }
      else {
        DEBUG((EFI_D_INFO, "Successfully reserved 16MB for TAGEC at 0x%x!\n", TagecAddr));
        break;
      }
    }
  }
  return Status;
}

/**

  This is the standard EFI driver entry point for the driver. This
  driver is responsible for reserving memory and initialization information.

  @param ImageHandle  -  Handle for the image of this driver.
  @param SystemTable  -  Pointer to the EFI System Table.

  @retval EFI_SUCCESS  -  Policy decisions set.

**/
EFI_STATUS
EFIAPI
ReserveMemEntryPoint (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
)
{
  EFI_STATUS                      Status;

  //
  //  Function to reserve memory.
  //
  Status = SetupReserveMemory();

  //
  // Reserve TAGEC Memory
  //
  Status = ReserveTagecMem();

  return EFI_SUCCESS;
}

