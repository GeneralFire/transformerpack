/** @file
  HECI common library (PEI).

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

**/
#include <PiPei.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Ppi/HeciAccessPpi.h>
#include <Ppi/HeciTransportPpi.h>

/**
  Get instance index of given PPI for given GUID.

  @param[in]     InputPpi         PPi instance pointer to look for index
  @param[in]     InputPpiPpiGuid  PPi Guid to search for


  @retval PPI instance index
  @retval MAX_UINT32 if found no instance
**/
UINTN
GetPpiInstanceIndex (
  IN     VOID              *InputPpi,
  IN     EFI_GUID          *InputPpiPpiGuid
  )
{
  EFI_STATUS       Status;
  UINTN            PpiInstance;
  VOID             *ReturnPpi;

  if ((sizeof (UINTN) != sizeof (UINT32)) || // called not in PEI
      (InputPpi == NULL) ||                  // bad input
      (InputPpiPpiGuid == NULL)) {           // bad input
    return MAX_UINT32;
  }

  PpiInstance = 0;
  ReturnPpi = NULL;

  // find provided PPI
  PeiServicesLocatePpi (
    InputPpiPpiGuid,
    PpiInstance,
    NULL,
    &ReturnPpi
    );

  while (InputPpi != ReturnPpi) {
    ReturnPpi = NULL;
    PpiInstance++;
    Status = PeiServicesLocatePpi (
               InputPpiPpiGuid,
               PpiInstance,
               NULL,
               &ReturnPpi
               );
    if (EFI_ERROR (Status) || (ReturnPpi == NULL)) {
      break;
    }
  }
  if (InputPpi != ReturnPpi) {
    PpiInstance = MAX_UINT32;
  }

  return PpiInstance;
}

/**
  Get next instance of given PPI for given GUID.

  @param[in]     InputPpi         PPi instance pointer to look for next instance
  @param[in]     InputPpiPpiGuid  PPi Guid to search for

  @retval PPI pointer Next to input PPI object
  @retval NULL in error case
**/
VOID*
GetNextPpi (
  IN     VOID              *InputPpi,
  IN     EFI_GUID          *InputPpiPpiGuid
  )
{
  UINTN            PpiInstance;
  VOID             *ReturnPpi;

  ReturnPpi = NULL;
  if (InputPpi == NULL) {
    PpiInstance = 0;
  } else {
    PpiInstance = GetPpiInstanceIndex (InputPpi, InputPpiPpiGuid);
  }
  if (PpiInstance != MAX_UINT32) {
    if (InputPpi != NULL) {
      // get next
      PpiInstance++;
    }
    PeiServicesLocatePpi (
      InputPpiPpiGuid,
      PpiInstance,
      NULL,
      &ReturnPpi
      );
  }

  return ReturnPpi;
}

/**
  Get first HeciAccess protocol pointer

  @retval Pointer to first HeciAccess protocol
  @retval NULL pointer indicates error
**/
HECI_ACCESS*
GetFirstHeciAccess (
  VOID
  )
{
  HECI_ACCESS *HeciAccess;

  HeciAccess = NULL;
  PeiServicesLocatePpi (
    &gHeciAccessPpiGuid,
    0,
    NULL,
    &HeciAccess
    );

  return HeciAccess;
}

/**
  Get first HeciTransport protocol pointer

  @retval Pointer to first HeciTransport protocol
  @retval NULL pointer indicates error
**/
HECI_TRANSPORT*
GetFirstHeciTransport (
  VOID
  )
{
  HECI_TRANSPORT *HeciTransport;

  HeciTransport = NULL;
  PeiServicesLocatePpi (
    &gHeciTransportPpiGuid,
    0,
    NULL,
    &HeciTransport
    );

  return HeciTransport;
}

/**
  Allocate memory for HeciControl

  @param[in]      AllocationSize   Amount memory to allocate

  @retval   Allocated memory pointer, NULL on failure
**/
VOID*
HeciAllocateMemory (
  IN UINTN   AllocationSize
  )
{
  VOID                      *ReturnAllocatedMemory;

  ReturnAllocatedMemory = AllocateZeroPool (AllocationSize);

  return ReturnAllocatedMemory;
}

/**
  Free memory for HeciControl

  @param[in]      Memory   Memory pointer to free
**/
VOID
HeciFreeMemory (
  IN VOID   *Memory
  )
{
  FreePool (Memory);
}