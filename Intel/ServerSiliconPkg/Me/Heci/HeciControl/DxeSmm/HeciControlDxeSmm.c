/** @file
  HECI control protocol driver (Dxe, Smm)

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

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Guid/EventGroup.h>
#include <Guid/EventLegacyBios.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/HeciTransportProtocol.h>
#include <Protocol/HeciControlProtocol.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmBase2.h>
#include <Register/HeciRegs.h>
#include <Library/HeciControlCoreLib.h>
#include <Library/HeciCommonLib.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Library/MeUtilsLib.h>
#include <Register/MeRegs.h>
// Message filtering
#include <Sps.h>

#define HECI_CONTROL_DEBUG   "[HECI Control %s]"
//
// Global driver data
//

GLOBAL_REMOVE_IF_UNREFERENCED CHAR16                *mHeciModuleDebugPrefix;

/**
  HECI driver entry point used to initialize support for the HECI device
  for DXE

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             Protocol installed successfully
  @retval EFI_OUT_OF_RESOURCES    Can't allocate protocol data
**/
EFI_STATUS
EFIAPI
HeciControlEntryPoint (
  IN EFI_HANDLE                  ImageHandle,
  IN EFI_SYSTEM_TABLE            *SystemTable
  )
{
  EFI_STATUS                    Status;
  HECI_CONTROL_PRIVATE          *HeciControl;
  HECI_TRANSPORT                *HeciTransport;
  EFI_GUID                      *SelectedHeciGuid;

  HeciTransport = NULL;

  // Set initial failed status
  if (IsInSmm (NULL)) {
    SelectedHeciGuid = &gHeciControlSmmProtocolGuid;
    SetMeUefiFwHealthEfiStatus (Heci1SmmInstalledStatus, EFI_NOT_FOUND);
    SetMeUefiFwHealthEfiStatus (Heci3SmmInstalledStatus, EFI_NOT_FOUND);
  } else {
    SetMeUefiFwHealthEfiStatus (Heci1DxeInstalledStatus, EFI_NOT_FOUND);
    SelectedHeciGuid = &gHeciControlProtocolGuid;
  }

  if (GetFirstHeciTransport () == NULL) {
    DEBUG ((
      DEBUG_ERROR,
      "[HECI Control Protocol]  ERROR: Can't find HECI Transport\n"
      ));
    ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
    return EFI_UNSUPPORTED;
  }

  mHeciModuleDebugPrefix = GetModuleIdentificationString ();
  if (mHeciModuleDebugPrefix == NULL) {
    ASSERT (mHeciModuleDebugPrefix != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " %a\n", __FUNCTION__));

  HeciControl = HeciAllocateMemory (sizeof (HECI_CONTROL_PRIVATE));
  if (HeciControl != NULL) {
    HeciInitializeNewControlInstance (HeciControl);

    Status = InstallHeciProtocol (
               SelectedHeciGuid,
               (VOID*)HeciControl
               );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      HeciFreeMemory (HeciControl);
    } else {
      HeciControlInitalizeAllCommunicationDevices (HeciControl);
    }
  } else {
    ASSERT (HeciControl != NULL);
    Status = EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " HECI Protocol Exit : %r.\n", Status));

  return Status;
}
