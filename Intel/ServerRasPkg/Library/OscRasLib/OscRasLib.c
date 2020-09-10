/** @file
  Implementation of Opportunistic Spare Core feature.

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
#include <PiSmm.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/LocalApicLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/ErrorReportLib.h>
#include <Library/RasDebugLib.h>
#include <Protocol/MpService.h>
#include <Protocol/SmmVariable.h>

//
// Global variable to cache pointer to SMM Variable protocol.
//
static EFI_SMM_VARIABLE_PROTOCOL      *mSmmVariableProtocol = NULL;

SPIN_LOCK                      mOscCoreDisableMaskSpinLock;

#define                        OSC_CORE_DISABLE_MASK_LEN (sizeof(UINT64) * MAX_SOCKET)

/**
  The constructor function caches the pointer to SMM Variable protocol and
  create EFI variable to store faulty CPU core index.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
OscRasLibConstructor (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  EFI_STATUS  Status;
  UINT64      OscCoreDisableMask[MAX_SOCKET];
  UINTN       VariableSize = OSC_CORE_DISABLE_MASK_LEN;

  Status = gSmst->SmmLocateProtocol (&gEfiSmmVariableProtocolGuid, NULL, (VOID**) &mSmmVariableProtocol);
  ASSERT_EFI_ERROR (Status);

  if (mSmmVariableProtocol == NULL) {
   RAS_DEBUG ((LEVEL_FUNC_FLOW, "mSmmVariableProtocol is NULL\n"));
   RAS_ASSERT (FALSE);
   return EFI_INVALID_PARAMETER;
  }

  Status = mSmmVariableProtocol->SmmGetVariable (
                                  L"OscCoreDisableMask",
                                  &gRasGlobalDataVariableGuid,
                                  NULL,
                                  &VariableSize,
                                  &OscCoreDisableMask
                                  );
  if (Status == EFI_NOT_FOUND) {
    //
    // Create EFI variable to store faulty CPU core index.
    //
    SetMem (OscCoreDisableMask, OSC_CORE_DISABLE_MASK_LEN, 0x00);
    Status = mSmmVariableProtocol->SmmSetVariable (
                                    L"OscCoreDisableMask",
                                    &gRasGlobalDataVariableGuid,
                                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                                    VariableSize,
                                    &OscCoreDisableMask
                                    );
    ASSERT_EFI_ERROR (Status);
  } else {
    ASSERT_EFI_ERROR (Status);
  }

  InitializeSpinLock (&mOscCoreDisableMaskSpinLock);

  return EFI_SUCCESS;
}

/**
  This routine purpose is to handle Opportunistic Spare Core for MLC persistent error.
  Store faulty CPU core index in EFI variable for CPU Init code to do CPU core failover on next boot.

  @param   NONE

  @retval  none
**/
VOID
EFIAPI
OscErrorhandler (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_CPU_PHYSICAL_LOCATION     CoreLocation;
  UINT32                        InitialApicId;
  UINT64                        OscCoreDisableMask[MAX_SOCKET];
  UINTN                         VariableSize = OSC_CORE_DISABLE_MASK_LEN;

  AcquireSpinLock (&mOscCoreDisableMaskSpinLock);
  Status = mSmmVariableProtocol->SmmGetVariable (
                                  L"OscCoreDisableMask",
                                  &gRasGlobalDataVariableGuid,
                                  NULL,
                                  &VariableSize,
                                  &OscCoreDisableMask
                                  );
  ASSERT_EFI_ERROR (Status);

  InitialApicId = GetInitialApicId ();

  //
  // Get faulty CPU core location information
  //
  GetProcessorLocationByApicId (
    InitialApicId,
    &CoreLocation.Package,
    &CoreLocation.Core,
    &CoreLocation.Thread
    );
  OscCoreDisableMask[CoreLocation.Package] |= (UINT64) (1 << CoreLocation.Core);

  //
  // Store faulty CPU core index to EFI variable.
  //
  Status = mSmmVariableProtocol->SmmSetVariable (
                                  L"OscCoreDisableMask",
                                  &gRasGlobalDataVariableGuid,
                                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                                  VariableSize,
                                  &OscCoreDisableMask
                                  );
  ASSERT_EFI_ERROR (Status);
  ReportSpareCoreErrorLog (CoreLocation.Package, CoreLocation.Core);
  ReleaseSpinLock (&mOscCoreDisableMaskSpinLock);
}