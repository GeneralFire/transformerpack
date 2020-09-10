/** @file
  HECI Access driver for Dxe and Smm

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019  - 2020 Intel Corporation.

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

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PerformanceLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/HeciAccessCoreLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/HeciAccessProtocol.h>
#include <Protocol/SmmReadyToLock.h>
#include <Library/HeciCommonLib.h>
#include <Library/HeciAccessDeviceConfigLib.h>
#include <Library/MeUtilsLib.h>

#define HECI_ACCESS_DEBUG                         "[HECI Access %s]"

//
// Global driver data
//
GLOBAL_REMOVE_IF_UNREFERENCED CHAR16              *mHeciModuleDebugPrefix;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN             mGotSmmReadyToLockEvent = FALSE;

/**
  Get Next HECI Access

  @param[in]     This          Pointer to HECI Access device context structure
  @param[in]     HeciAccess    Pointer to HECI Access device context structure
                               the next element should be relative to
                               Passing NULL requests first element

  @retval HECI_ACCESS Next HeciAccess object
**/
HECI_ACCESS*
GetNextHeciAccess (
  IN      HECI_ACCESS   *This,
  IN      HECI_ACCESS   *HeciAccess
  )
{
  HECI_ACCESS  *ReturnHeciAccess;

  if (This == NULL) {
    return NULL;
  }

  if (HeciAccess == NULL) {
    ReturnHeciAccess = ((HECI_ACCESS_PRIVATE*)This)->HeciAccessRoot;
  } else {
    ReturnHeciAccess = ((HECI_ACCESS_PRIVATE*)HeciAccess)->HeciAccessNext;
  }

  return ReturnHeciAccess;
}

/**
  Smm ReadyToLock event notification handler.
  Installed for SMM only.

  Update HECI Memory BAR after PCI enumerator

  @param[in] Protocol     Points to the protocol's unique identifier.
  @param[in] Interface    Points to the interface instance.
  @param[in] NotifyHandle The handle on which the interface was installed.

  @retval EFI_SUCCESS     Notification handler runs successfully.
 **/
EFI_STATUS
EFIAPI
SmmReadyToLockEventNotify (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      NotifyHandle
  )
{
  HECI_ACCESS     *HeciAccess;
  UINTN           MemoryBar;
  UINTN           Counter;

  DEBUG ((DEBUG_INFO, HECI_ACCESS_DEBUG, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " Enter %a.\n", __FUNCTION__));

  Counter = 0;
  // Check for multiple times handler trigger
  if (!mGotSmmReadyToLockEvent) {
    HeciAccess = GetFirstHeciAccess ();
    while (HeciAccess != NULL) {
      MemoryBar = HeciMbarRead (
                    (HECI_ACCESS_PRIVATE*)HeciAccess,
                    NULL
                    );
      if (MemoryBar != 0) {
        ((HECI_ACCESS_PRIVATE*)HeciAccess)->MemoryBar = MemoryBar;
        Counter++;
      }
      HeciAccess = HeciAccess->GetNextHeciAccess (HeciAccess, HeciAccess);
    }
    // Lock Heci MemoryBar configuration
    mGotSmmReadyToLockEvent = TRUE;
  }

  DEBUG ((DEBUG_INFO, HECI_ACCESS_DEBUG, mHeciModuleDebugPrefix));
  DEBUG ((
    DEBUG_INFO,
    " Exit %a (updated %d MemoryBars).\n",
    __FUNCTION__,
    Counter
    ));

  return EFI_SUCCESS;
}

/**
  HECI driver entry point used to initialize support for the HECI device.

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES    Can't allocate PPI data
**/
EFI_STATUS
EFIAPI
HeciAccessEntryPoint (
  IN EFI_HANDLE                  ImageHandle,
  IN EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                                    Status;
  EFI_SMM_SYSTEM_TABLE2                         *Smst2;
  HECI_ACCESS_PRIVATE                           *HeciAccess;
  UINT8                                         Index;
  UINT8                                         HeciCount;
  HECI_ACCESS_DEVICE_INITIALIZATION_DEFINITION  *HeciDeviceInitInfo;
  VOID                                          *Registration;
  HECI_ACCESS_PRIVATE                           *HeciAccessRoot;
  HECI_ACCESS_PRIVATE                           *HeciAccessPrevious;
  EFI_GUID                                      *SelectedHeciGuid;

  mHeciModuleDebugPrefix = GetModuleIdentificationString ();
  if (mHeciModuleDebugPrefix == NULL) {
    ASSERT (mHeciModuleDebugPrefix != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, HECI_ACCESS_DEBUG, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " %a\n", __FUNCTION__));

  HeciCount = GetHeciInitializationInfo (&HeciDeviceInitInfo);
  if (HeciCount == 0) {
    DEBUG ((DEBUG_ERROR, HECI_ACCESS_DEBUG, mHeciModuleDebugPrefix));
    DEBUG ((DEBUG_ERROR, " ERROR: No HECI devices defined\n"));
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    return EFI_NOT_FOUND;
  }

  if (IsInSmm (&Smst2)) {
    SelectedHeciGuid = &gHeciAccessSmmProtocolGuid;
  } else {
    SelectedHeciGuid = &gHeciAccessProtocolGuid;
  }
  HeciAccessRoot = NULL;
  HeciAccessPrevious = NULL;
  //
  // Install HECI access protocol
  //
  for (Index = 0; Index < HeciCount; Index++) {
    HeciAccess = NULL;
    if (((HeciDeviceInitInfo[Index].MemoryBarSmm != HECI_DISABLED_MBAR) && (Smst2 != NULL)) ||
        ((HeciDeviceInitInfo[Index].MemoryBarDxe != HECI_DISABLED_MBAR) && (Smst2 == NULL))) {
      HeciAccess = HeciAllocateMemory (sizeof (HECI_ACCESS_PRIVATE));
    }
    if (HeciAccess != NULL) {
      if (HeciSetNewDevice (
            HeciAccess,
            &HeciDeviceInitInfo[Index],
            (Smst2 != NULL)
            ) == FALSE) {
        HeciFreeMemory (HeciAccess);
        HeciAccess = NULL;
      } else {
        Status = InstallHeciProtocol (
                   SelectedHeciGuid,
                   (VOID*)HeciAccess
                   );
        if (!EFI_ERROR (Status)) {
          if (HeciAccessRoot == NULL) {
            HeciAccessRoot = HeciAccess;
          }
          HeciAccess->HeciAccessRoot = HeciAccessRoot;
          if (HeciAccessPrevious != NULL) {
            HeciAccessPrevious->HeciAccessNext = HeciAccess;
          }
          HeciAccessPrevious = HeciAccess;
        } else {
          ASSERT_EFI_ERROR (Status);
          HeciFreeMemory (HeciAccess);
          HeciAccess = NULL;
        }
      }
    }
  }

  if ((!EFI_ERROR (Status)) && (Smst2 != NULL)) {
    // Add ReadyToLockEvent notification handler
    // for MemoryBar Update
    Registration = NULL;
    Status = Smst2->SmmRegisterProtocolNotify (
                      &gEfiSmmReadyToLockProtocolGuid,
                      SmmReadyToLockEventNotify,
                      &Registration
                      );
    DEBUG ((DEBUG_INFO, HECI_ACCESS_DEBUG, mHeciModuleDebugPrefix));
    DEBUG ((
      DEBUG_INFO,
      " Install SmmReadyToLockEventNotify status : %r.\n",
      Status
      ));
    ASSERT_EFI_ERROR (Status);
    // Do not unload driver while HECI is installed
    Status = EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, HECI_ACCESS_DEBUG, mHeciModuleDebugPrefix));
  DEBUG ((
    DEBUG_INFO,
    " HECI Access DXE Protocol Exit : %r.\n",
    Status
    ));

  return Status;
}
