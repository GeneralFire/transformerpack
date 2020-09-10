/** @file
  HECI Control protocol driver (Pei)

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
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PerformanceLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/HeciRegs.h>
#include <Ppi/HeciTransportPpi.h>
#include <Ppi/HeciControlPpi.h>
#include <Library/HeciControlCoreLib.h>
#include <Library/HeciCommonLib.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Library/MeChipsetLib.h>
#include <IndustryStandard/Pci22.h>
#include <Ppi/MemoryDiscovered.h>

#define HECI_CONTROL_DEBUG   "[HECI CONTROL PEI]"

//
// Global driver data
//
GLOBAL_REMOVE_IF_UNREFERENCED const CHAR16         *mHeciModuleDebugPrefix = (CHAR16 *)L"PEI";

/**
  HeciControlPeiPreMem.inf   PreMem driver, doesn't reinitialize on gEfiPeiMemoryDiscoveredPpiGuid callback
  HeciControlPei.inf         PreMem/Postmem driver reinitializes on gEfiPeiMemoryDiscoveredPpiGuid callback

  Possible usage combination:
   1. Single driver
      Load HeciControlPei.inf in premem (ReinitializeHeciInPostMem == TRUE)
   2. Dual driver
      Load HeciControlPeiPreMem.inf in premem (ReinitializeHeciInPostMem == FALSE)
      and load HeciControlPei.inf in postmem  (ReinitializeHeciInPostMem == TRUE)
**/
extern CONST BOOLEAN  ReinitializeHeciInPostMem;

EFI_STATUS
EFIAPI
HeciTransportControlNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

// Notify Descriptor used to register a callback on HeciTransport
static EFI_PEI_NOTIFY_DESCRIPTOR  ReinitializeHeciPpiPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gHeciTransportPpiGuid,
  HeciTransportControlNotifyCallback
};

/**
  HECI Control driver entry point used to initialize support for the HECI device.

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
  @retval EFI_OUT_OF_RESOURCES    Can't allocate PPI data
**/
EFI_STATUS
EFIAPI
HeciControlEntryPoint (
  IN       EFI_PEI_FILE_HANDLE   FileHandle,
  IN CONST EFI_PEI_SERVICES      **PeiServices
  )
{
  EFI_STATUS                     Status;
  HECI_CONTROL_PRIVATE           *HeciControl;
  EFI_PEI_PPI_DESCRIPTOR         *HeciControlPpi;
  HECI_CONTROL_PRIVATE           *HeciControlOld;
  EFI_PEI_PPI_DESCRIPTOR         *HeciControlOldPpi;
  UINTN                          HeciTransportCounter;
  HECI_TRANSPORT                 *HeciTransport;

  DEBUG ((DEBUG_INFO, "%a %a\n", HECI_CONTROL_DEBUG, __FUNCTION__));

  // Set initial failed status
  SetMeUefiFwHealthEfiStatus (Heci1PpiInstalledStatus, EFI_NOT_FOUND);
  SetMeUefiFwHealthEfiStatus (Heci2PpiInstalledStatus, EFI_NOT_FOUND);

  HeciTransportCounter = 0;
  HeciTransport = GetFirstHeciTransport ();
  while (HeciTransport != NULL) {
    HeciTransportCounter++;
    HeciTransport = HeciTransport->GetNextHeciTransport (HeciTransport, HeciTransport);
  }
  DEBUG ((
    DEBUG_ERROR,
    "%a HECI Transport found: %d\n",
    HECI_CONTROL_DEBUG,
    HeciTransportCounter
    ));
  if (HeciTransportCounter == 0) {
    Status = PeiServicesNotifyPpi (&ReinitializeHeciPpiPeiNotifyList);
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  HeciControlPpi = (EFI_PEI_PPI_DESCRIPTOR*)AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (HeciControlPpi == NULL) {
    ASSERT (HeciControlPpi != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  HeciControl = AllocateZeroPool (sizeof (HECI_CONTROL_PRIVATE));
  if (HeciControl != NULL) {
    HeciInitializeNewControlInstance (HeciControl);

    HeciControlPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    HeciControlPpi->Guid  = &gHeciControlPpiGuid;
    HeciControlPpi->Ppi   = HeciControl;

    Status = PeiServicesLocatePpi (
      &gHeciControlPpiGuid,
      0,
      &HeciControlOldPpi,
      &HeciControlOld
      );
    if (EFI_ERROR (Status)) {
      Status = PeiServicesInstallPpi (HeciControlPpi);
    } else {
      DEBUG ((DEBUG_INFO, "%a Reinstalling PPI instance\n", HECI_CONTROL_DEBUG));
      Status = PeiServicesReInstallPpi (HeciControlOldPpi, HeciControlPpi);
    }
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      FreePool (HeciControlPpi);
      FreePool (HeciControl);
    } else {
      HeciControlInitalizeAllCommunicationDevices (HeciControl);
    }
    // Register a callback  to initialize not initialized interfaces
    Status = PeiServicesNotifyPpi (&ReinitializeHeciPpiPeiNotifyList);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a ERROR installing callback %r.\n", HECI_CONTROL_DEBUG, Status));
      ASSERT_EFI_ERROR (Status);
    }
    Status = EFI_SUCCESS;
  } else {
    ASSERT (HeciControl != NULL);
    FreePool (HeciControlPpi);
    Status = EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "%a HECI Control PEI Exit : %r.\n", HECI_CONTROL_DEBUG, Status));

  return Status;
}

/**
  Once HeciTransport is reinitialized, reinitialize HeciControl

  @param Ppi                    Context

  @retval EFI_SUCCESS           Successfully reinitialized
  @retval EFI_OUT_OF_RESOURCES  Allocation error occurred
  @retval EFI_INVALID_PARAMETER Passed NULL pointer
**/
EFI_STATUS
EFIAPI
HeciControlReinitialize (
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                Status;
  HECI_CONTROL_PRIVATE      *HeciControlOld;
  EFI_PEI_PPI_DESCRIPTOR    *HeciControlOldPpi;
  HECI_CONTROL_PRIVATE      *HeciControl;
  EFI_PEI_PPI_DESCRIPTOR    *HeciControlPpi;
  HECI_TRANSPORT            *HeciTransport;
  UINTN                     HeciNumber;

  Status = PeiServicesLocatePpi (
             &gEfiPeiMemoryDiscoveredPpiGuid,
             0,
             NULL,
             &HeciControl // temporary used
             );
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  HeciTransport = (HECI_TRANSPORT*) Ppi;
  if (HeciTransport == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = PeiServicesLocatePpi (
             &gHeciControlPpiGuid,
             0,
             &HeciControlOldPpi,
             &HeciControlOld
             );
  if (!EFI_ERROR (Status)) {
    HeciControlOld->LastUsedHeciTransport = NULL;
    if (HeciControlOld->PeiReinitialize) {
      // Reinitialize HECI1 interface
      Status = GetHeciNumber (HECI1, &HeciNumber);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return Status;
      }

      if (HeciNumber != 1) {
        // Reinitialize HeciControl once for HECI 1 only
        DEBUG ((
          DEBUG_INFO,
          "%a %a (): Skip reinitialization for HECI %d\n",
          HECI_CONTROL_DEBUG,
          __FUNCTION__,
          HeciNumber
          ));
      } else {
        HeciControl = AllocateZeroPool (sizeof (HECI_CONTROL_PRIVATE));
        if (HeciControl != NULL) {
          // Copy data
          CopyMem (HeciControl, HeciControlOld, sizeof (HECI_CONTROL_PRIVATE));
          HeciControl->PeiReinitialize = FALSE;
          // Reinitialize functions
          SetHeciControlFunctions (HeciControl);
          HeciControlPpi = (EFI_PEI_PPI_DESCRIPTOR*) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
          if (HeciControlPpi == NULL) {
            ASSERT (HeciControlPpi != NULL);
            FreePool (HeciControl);
            return EFI_OUT_OF_RESOURCES;
          }
          HeciControlPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
          HeciControlPpi->Guid  = &gHeciControlPpiGuid;
          HeciControlPpi->Ppi   = HeciControl;
          Status = PeiServicesReInstallPpi (HeciControlOldPpi, HeciControlPpi);
          if (EFI_ERROR (Status)) {
            FreePool (HeciControl);
            FreePool (HeciControlPpi);
            ASSERT_EFI_ERROR (Status);
            return EFI_OUT_OF_RESOURCES;
          }
          DEBUG ((
            DEBUG_INFO,
            "%a Reinstalled HeciControl PPI instance (%r)\n",
            HECI_CONTROL_DEBUG,
            Status
            ));
        }
      }
    }
  }
  DEBUG ((
    DEBUG_INFO,
    "%a %a (): Reinstalled (%r)\n",
    HECI_CONTROL_DEBUG,
    __FUNCTION__,
    Status
    ));

  return Status;
}

/**
  Once HeciTransport has been updated, locate an uninitialized HECI interface
  and initialize them

  @param PeiServices      - General purpose services available to every PEIM.
  @param NotifyDescriptor - Information about the notify event.
  @param Ppi              - Context

  @retval EFI_SUCCESS   - Successfully initialized PPI
  @retval EFI_NOT_FOUND - Unable to perform initialization
**/
EFI_STATUS
EFIAPI
HeciTransportControlNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                 Status;
  HECI_CONTROL_PRIVATE       *HeciControl;

  DEBUG ((DEBUG_INFO, "%a %a (ReinitializeHeciInPostMem = %d)\n", HECI_CONTROL_DEBUG, __FUNCTION__, ReinitializeHeciInPostMem));

  if (!ReinitializeHeciInPostMem) {
    // Verify if driver runs in premem
    Status = PeiServicesLocatePpi (
               &gEfiPeiMemoryDiscoveredPpiGuid,
               0,
               NULL,
               &HeciControl // temporary used
               );
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a %a gEfiPeiMemoryDiscoveredPpiGuid found, exit\n", HECI_CONTROL_DEBUG, __FUNCTION__));
      return EFI_SUCCESS;
    }
  }

  Status = PeiServicesLocatePpi (
             &gHeciControlPpiGuid,
             0,
             NULL,
             &HeciControl
             );
  if (!EFI_ERROR (Status)) {
    // Set Last  Transport from notification
    HeciControl->LastUsedHeciTransport = Ppi;
    HeciControlInitalizeCommunicationDevice (HeciControl, (HECI_TRANSPORT*) Ppi);

    Status = HeciControlReinitialize (Ppi);
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
