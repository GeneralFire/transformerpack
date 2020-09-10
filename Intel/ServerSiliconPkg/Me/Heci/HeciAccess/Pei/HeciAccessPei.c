/** @file
  HECI Access protocol driver for use in Pei

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
#include <IndustryStandard/Pci22.h>
#include <Library/PcdLib.h>
#include <Library/HeciAccessCoreLib.h>
#include <Library/HeciCommonLib.h>
#include <Library/PmcLib.h>
#include <Ppi/HeciAccessPpi.h>
#include <Ppi/MemoryDiscovered.h>
#include <Library/HeciAccessDeviceConfigLib.h>

#define HECI_ACCESS_DEBUG                     "[HECI Access]"

/**
  HeciAccessPeiPreMem.inf   PreMem driver, doesn't set gEfiPeiMemoryDiscoveredPpiGuid callback
  HeciAccessPei.inf         PreMem/Postmem driver sets gEfiPeiMemoryDiscoveredPpiGuid callback

  Possible usage combination:
   1. Single driver
      Load HeciAccessPei.inf in premem (ReinitializeHeciInPostMem == TRUE)
   2. Dual driver
      Load HeciAccessPeiPreMem.inf in premem (ReinitializeHeciInPostMem == FALSE)
      and load HeciAccessPei.inf in postmem  (ReinitializeHeciInPostMem == TRUE)
**/
extern CONST BOOLEAN ReinitializeHeciInPostMem;

//
// Global driver data
//
GLOBAL_REMOVE_IF_UNREFERENCED CHAR16          *mHeciModuleDebugPrefix = (CHAR16 *)L"PEI";
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN         mGotSmmReadyToLockEvent = FALSE;

EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

// Notify Descriptor used to register a callback
static EFI_PEI_NOTIFY_DESCRIPTOR  mReinitializeHeciPpiPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  MemoryDiscoveredPpiNotifyCallback
  };

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
  return GetNextPpi (HeciAccess, &gHeciAccessPpiGuid);
}

/**
  Once main memory has been initialized, locate and re-initialize HECI access
**/
VOID
ReinstallHeciAccessPpis (
  VOID
  )
{
  EFI_STATUS                Status;
  UINTN                     InstanceCount;
  HECI_ACCESS_PRIVATE       *HeciAccessOld;
  EFI_PEI_PPI_DESCRIPTOR    *HeciAccessOldPpi;
  HECI_ACCESS_PRIVATE       *HeciAccess;
  EFI_PEI_PPI_DESCRIPTOR    *HeciAccessPpi;

  Status = PeiServicesLocatePpi (
             &gEfiPeiMemoryDiscoveredPpiGuid,
             0,
             NULL,
             &HeciAccess // temporary used
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  InstanceCount = 0;
  Status = PeiServicesLocatePpi (
             &gHeciAccessPpiGuid,
             InstanceCount,
             &HeciAccessOldPpi,
             &HeciAccessOld
             );
  while (!EFI_ERROR (Status)) {
    HeciAccess = AllocateZeroPool (sizeof (HECI_ACCESS_PRIVATE));
    if (HeciAccess == NULL) {
      ASSERT (HeciAccess != NULL);
      break;
    }
    // Copy data
    CopyMem (HeciAccess, HeciAccessOld, sizeof (HECI_ACCESS_PRIVATE));
    // Reinitialize functions
    SetHeciAccessFunctions (HeciAccess);
    HeciAccessPpi = (EFI_PEI_PPI_DESCRIPTOR*) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
    if (HeciAccessPpi == NULL) {
      ASSERT (HeciAccessPpi != NULL);
      FreePool (HeciAccess);
      break;
    }
    HeciAccessPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    HeciAccessPpi->Guid  = &gHeciAccessPpiGuid;
    HeciAccessPpi->Ppi   = HeciAccess;
    Status = PeiServicesReInstallPpi (HeciAccessOldPpi, HeciAccessPpi);
    if (EFI_ERROR (Status)) {
      FreePool (HeciAccess);
      FreePool (HeciAccessPpi);
      ASSERT_EFI_ERROR (Status);
      break;
    }
    DEBUG ((
      DEBUG_INFO,
      "%a Reinstalled HeciAccess PPI instance %d (%r)\n",
      HECI_ACCESS_DEBUG,
      InstanceCount,
      Status
      ));
    InstanceCount++;
    Status = PeiServicesLocatePpi (
               &gHeciAccessPpiGuid,
               InstanceCount,
               &HeciAccessOldPpi,
               &HeciAccessOld
               );
  }
  DEBUG ((
    DEBUG_INFO,
    "%a %a (): Reinstalled : %d\n",
    HECI_ACCESS_DEBUG,
    __FUNCTION__,
    InstanceCount
    ));
}

/**
  Once main memory has been initialized, locate and re-link HECI transport and access
  Add new PPI if any new interfaces are available

  @param PeiServices      - General purpose services available to every PEIM.
  @param NotifyDescriptor - Information about the notify event.
  @param Ppi              - Context

  @retval EFI_SUCCESS   - Successfully initialized PPI
  @retval EFI_NOT_FOUND - Unable to perform initialization
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                                    Status;
  HECI_ACCESS                                   *HeciAccess;
  EFI_PEI_PPI_DESCRIPTOR                        *HeciAccessPpi;
  UINT8                                         Index;
  UINT8                                         HeciCount;
  HECI_ACCESS_DEVICE_INITIALIZATION_DEFINITION  *HeciDeviceInitInfo;


  ReinstallHeciAccessPpis ();

  // Read Heci configuration
  Status = EFI_NOT_FOUND;
  HeciCount = GetHeciInitializationInfo (&HeciDeviceInitInfo);
  if (HeciCount == 0) {
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    return EFI_NOT_FOUND;
  }

  //
  // Install/re-install HECI access protocol
  //
  for (Index = 0; Index < HeciCount; Index++) {
    if (HeciDeviceInitInfo[Index].MemoryBarPei != HECI_DISABLED_MBAR) {
      // look is this HeciAccess is already installed
      Status = EFI_NOT_FOUND;
      HeciAccess = GetFirstHeciAccess ();
      while (HeciAccess != NULL) {
        if ((((HECI_ACCESS_PRIVATE*)HeciAccess)->PciSegment == HeciDeviceInitInfo[Index].PciSegment) &&
            (((HECI_ACCESS_PRIVATE*)HeciAccess)->PciBus == HeciDeviceInitInfo[Index].PciBus) &&
            (((HECI_ACCESS_PRIVATE*)HeciAccess)->PciDevice == HeciDeviceInitInfo[Index].PciDevice) &&
            (((HECI_ACCESS_PRIVATE*)HeciAccess)->PciFunction == HeciDeviceInitInfo[Index].PciFunction)) {
          // Instance for this HeciAccess has been found
          Status = EFI_SUCCESS;
          break;
        }
        HeciAccess = HeciAccess->GetNextHeciAccess (HeciAccess, HeciAccess);
      }
      if (!EFI_ERROR (Status)) {
        // already installed, keep it
        DEBUG ((
          DEBUG_INFO,
          "%a Heci Access %d already installed, skipping\n",
          HECI_ACCESS_DEBUG,
          Index
        ));
      } else {
        // Not found, install new
        DEBUG ((
          DEBUG_INFO,
          "%a Install Heci Access %d\n",
          HECI_ACCESS_DEBUG,
          Index
        ));
        HeciAccess = AllocateZeroPool (sizeof (HECI_ACCESS_PRIVATE));
        ASSERT (HeciAccess != NULL);
        if (HeciSetNewDevice ((HECI_ACCESS_PRIVATE*)HeciAccess, &HeciDeviceInitInfo[Index], FALSE)) {
          HeciAccessPpi = (EFI_PEI_PPI_DESCRIPTOR*)AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
          ASSERT (HeciAccessPpi != NULL);
          if (HeciAccessPpi != NULL) {
            HeciAccessPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
            HeciAccessPpi->Guid  = &gHeciAccessPpiGuid;
            HeciAccessPpi->Ppi   = HeciAccess;

            Status = PeiServicesInstallPpi (HeciAccessPpi);
            ASSERT_EFI_ERROR (Status);
            if (!EFI_ERROR (Status)) {
              DEBUG ((
                DEBUG_INFO,
                "%a Heci Access %d has been installed\n",
                HECI_ACCESS_DEBUG,
                Index
              ));
            } else {
              FreePool (HeciAccess);
              FreePool (HeciAccessPpi);
            }
          } else {
            FreePool (HeciAccess);
          }
        } else {
          FreePool (HeciAccess);
        }
      }
    }
  }

  return Status;
}

/**
  HECI driver entry point used to initialize support for the HECI device.

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
HeciAccessEntryPoint (
  IN       EFI_PEI_FILE_HANDLE   FileHandle,
  IN CONST EFI_PEI_SERVICES      **PeiServices
  )
{
  EFI_STATUS                                    Status;

  if (PmcIsDwrBootMode ()) {
    return EFI_SUCCESS;
  }


  Status = MemoryDiscoveredPpiNotifyCallback (NULL, NULL, NULL);
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status) && ReinitializeHeciInPostMem) {
    // Register a callback  to reinitialize PPI
    Status = PeiServicesNotifyPpi (&mReinitializeHeciPpiPeiNotifyList);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "%a ERROR installing callback %r.\n",
        HECI_ACCESS_DEBUG,
        Status
        ));
      ASSERT_EFI_ERROR (Status);
      Status = EFI_SUCCESS;
    }
  }

  DEBUG ((
    DEBUG_INFO,
    "%a HECI Access PEI Exit : %r.\n",
    HECI_ACCESS_DEBUG,
    Status
    ));

  ASSERT_EFI_ERROR (Status);

  return Status;
}
