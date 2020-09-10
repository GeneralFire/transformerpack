/** @file
  HECI common library. (DXE, SMM).

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
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/HeciCommonLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/PrintLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/HeciAccessProtocol.h>

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
  EFI_SMM_SYSTEM_TABLE2     *Smst2;
  EFI_STATUS                Status;


  IsInSmm (&Smst2);
  ReturnAllocatedMemory = NULL;
  if (Smst2 != NULL) {
    Status = Smst2->SmmAllocatePool (
                      EfiRuntimeServicesData,
                      AllocationSize,
                      (VOID**)&ReturnAllocatedMemory
                      );
    if (!EFI_ERROR (Status)) {
      ZeroMem (
        ReturnAllocatedMemory,
        AllocationSize
        );
    }
  } else {
    ReturnAllocatedMemory = AllocateZeroPool (AllocationSize);
  }

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
  EFI_SMM_SYSTEM_TABLE2     *Smst2;

  IsInSmm (&Smst2);
  if (Smst2 != NULL) {
    Smst2->SmmFreePool (Memory);
  } else {
    FreePool (Memory);
  }
}

/**
  Function installs DXE or SMM protocol depending on mode we are running in

  @param[in]     HeciGuidProtocol    Pointer to Dxe/Smm protocol guid to install
  @param[in]     HeciProtocol        Pointer to HECI protocol to be installed

  @retval Protocol installation status
  @retval EFI_UNSUPPORTED if called in PEI
**/
EFI_STATUS
InstallHeciProtocol (
  EFI_GUID                *HeciGuidProtocol,
  VOID                    *HeciProtocol
  )
{
  EFI_STATUS                Status;
  EFI_SMM_SYSTEM_TABLE2     *Smst2;
  EFI_HANDLE                Handle;

  if (sizeof (UINTN) == sizeof (UINT32)) {
    ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
    return EFI_UNSUPPORTED;
  }

  if ((HeciProtocol == NULL) || (HeciGuidProtocol == NULL)) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  Handle = NULL;
  IsInSmm (&Smst2);
  if (Smst2 != NULL) {
    Status = Smst2->SmmInstallProtocolInterface (
                      &Handle,
                      HeciGuidProtocol,
                      EFI_NATIVE_INTERFACE,
                      HeciProtocol
                      );
  } else {
    Status = gBS->InstallMultipleProtocolInterfaces (
                    &Handle,
                    HeciGuidProtocol,
                    HeciProtocol,
                    NULL
                    );
  }

  return Status;
}

/**
  Function returns CHAR16 string pointer (DXE/SMM/PEI)
  for module identification in debug output

  @retval Pointer to CHAR16 module identification string
  @retval NULL pointer indicates error
**/
CHAR16 *
GetModuleIdentificationString (
  VOID
  )
{
  BOOLEAN     InSmm;
  CHAR8       *HeciModuleSelector;
  CHAR16      *HeciModuleDebugPrefix;
  UINTN       StringLength;
  UINTN       BufferSize;

  ///
  /// Select string
  ///
  HeciModuleSelector = NULL;
  // Check for PEI
  if (sizeof (UINTN) == sizeof (UINT32)) {
    HeciModuleSelector = DEBUG_PEI_DRIVER_STRING;
  } else {
    InSmm = IsInSmm (NULL);
    if (InSmm) {
      HeciModuleSelector = DEBUG_SMM_DRIVER_STRING;
    } else {
      HeciModuleSelector = DEBUG_DXE_DRIVER_STRING;
    }
  }

  ///
  /// Create output string
  ///
  HeciModuleDebugPrefix = NULL;
  StringLength = AsciiStrLen (HeciModuleSelector);
  if (StringLength > 0) {
    BufferSize = (StringLength + 1) * sizeof (CHAR16);
    HeciModuleDebugPrefix = HeciAllocateMemory (BufferSize);
    ASSERT (HeciModuleDebugPrefix != NULL);
    if (HeciModuleDebugPrefix != NULL) {
      UnicodeSPrintAsciiFormat (HeciModuleDebugPrefix, BufferSize, "%a", HeciModuleSelector);
    }
  }

  return HeciModuleDebugPrefix;
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
  EFI_SMM_SYSTEM_TABLE2  *SmmSystemTable2;
  HECI_ACCESS            *ReturnHeciAccess;

  ReturnHeciAccess = NULL;
  if (IsInSmm (&SmmSystemTable2)) {
    SmmSystemTable2->SmmLocateProtocol (&gHeciAccessSmmProtocolGuid, NULL, &ReturnHeciAccess);
  } else {
    gBS->LocateProtocol (&gHeciAccessProtocolGuid, NULL, &ReturnHeciAccess);
  }
  if (ReturnHeciAccess != NULL) {
    ReturnHeciAccess = ReturnHeciAccess->GetNextHeciAccess (ReturnHeciAccess, NULL);
  }

  return ReturnHeciAccess;
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
  EFI_SMM_SYSTEM_TABLE2  *SmmSystemTable2;
  HECI_TRANSPORT         *ReturnHeciTransport;

  ReturnHeciTransport = NULL;
  if (IsInSmm (&SmmSystemTable2)) {
    SmmSystemTable2->SmmLocateProtocol (&gHeciTransportSmmProtocolGuid, NULL, &ReturnHeciTransport);
  } else {
    gBS->LocateProtocol (&gHeciTransportProtocolGuid, NULL, &ReturnHeciTransport);
  }
  if (ReturnHeciTransport != NULL) {
    ReturnHeciTransport = ReturnHeciTransport->GetNextHeciTransport (ReturnHeciTransport, NULL);
  }

  return ReturnHeciTransport;
}