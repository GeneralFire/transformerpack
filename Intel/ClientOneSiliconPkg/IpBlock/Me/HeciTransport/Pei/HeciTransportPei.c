/** @file
  HECI Transport protocol driver (PEI))

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
#include <Library/HeciTransportCoreLib.h>
#include <Library/HeciCommonLib.h>
#include <Ppi/HeciTransportPpi.h>
#include <Ppi/HeciAccessPpi.h>
#include <Ppi/MemoryDiscovered.h>

//
// Global driver data
//
#define HECI_TRANSPORT_DEBUG                  "[HECI Transport]"
GLOBAL_REMOVE_IF_UNREFERENCED const CHAR16    *mHeciModuleDebugPrefix = (CHAR16 *)L"PEI";

/**
  HeciTransportPeiPreMem.inf   PreMem driver, doesn't reinitialize on gEfiPeiMemoryDiscoveredPpiGuid callback
  HeciTransportPei.inf         PreMem/Postmem driver reinitializes on gEfiPeiMemoryDiscoveredPpiGuid callback

  Possible usage combination:
   1. Single driver
      Load HeciTransportPei.inf in premem (ReinitializeHeciInPostMem == TRUE)
   2. Dual driver
      Load HeciTransportPeiPreMem.inf in premem (ReinitializeHeciInPostMem == FALSE)
      and load HeciTransportPei.inf in postmem  (ReinitializeHeciInPostMem == TRUE)
**/
extern CONST BOOLEAN  ReinitializeHeciInPostMem;

EFI_STATUS
EFIAPI
HeciTransportUpdateGuidPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

// Notify Descriptor used to register a callback
static EFI_PEI_NOTIFY_DESCRIPTOR  mReinitializeHeciPpiPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gHeciAccessPpiGuid,
  HeciTransportUpdateGuidPpiNotifyCallback
};

/**
  Install HeciTransport PPIs for each HeciAccess PPI instance.
**/
VOID
InstallHeciTransportPpis (
  VOID
  )
{
  HECI_ACCESS                   *HeciAccess;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_ERROR, "%a, start\n", __FUNCTION__));

  // get first HeciAccess
  HeciAccess = GetFirstHeciAccess ();

  while (HeciAccess != NULL) {
    Status = HeciTransportUpdateGuidPpiNotifyCallback (NULL, NULL, HeciAccess);
    if (EFI_ERROR (Status)) {
      break;
    }
    HeciAccess = HeciAccess->GetNextHeciAccess (HeciAccess, HeciAccess);
  }

  DEBUG ((DEBUG_ERROR, "%a, end\n", __FUNCTION__));
}


/**
  Once main memory has been initialized, locate and re-link HECI transport and access
  Add new PPI if any new interfaces are available
  For sending update signal responsible is HeciAccess.

  @param PeiServices      - General purpose services available to every PEIM.
  @param NotifyDescriptor - Information about the notify event.
  @param Ppi              - Context

  @retval EFI_SUCCESS           - Successfully initialized PPI
  @retval EFI_ABORTED           - No need to install/reinstall PPI
  @retval EFI_NOT_FOUND         - Unable to perform initialization
  @retval EFI_OUT_OF_RESOURCES  - Unable to allocate memory
  @retval EFI_INVALID_PARAMETER - Pointer to Ppi is NULL
**/
EFI_STATUS
EFIAPI
HeciTransportUpdateGuidPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                     Status;
  EFI_PEI_PPI_DESCRIPTOR         *TransportHeciPpi;
  HECI_ACCESS                    *HeciAccess;
  HECI_TRANSPORT_PRIVATE         *HeciTransport;
  HECI_TRANSPORT_PRIVATE         *HeciTransportOld;
  EFI_PEI_PPI_DESCRIPTOR         *HeciTransportOldPpi;
  UINTN                          HeciTransportInstance;
  BOOLEAN                        Reinstall;
  VOID                           *MemoryDiscoveredPpi;

  Reinstall = FALSE;

  DEBUG ((DEBUG_INFO, "%a %a (ReinitializeHeciInPostMem = %d)\n", HECI_TRANSPORT_DEBUG, __FUNCTION__, ReinitializeHeciInPostMem));
  if (Ppi == NULL) {
    ASSERT (Ppi != NULL);
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = (HECI_ACCESS*) Ppi;

  if (!ReinitializeHeciInPostMem) {
    // Verify if driver runs in premem
    Status = PeiServicesLocatePpi (
               &gEfiPeiMemoryDiscoveredPpiGuid,
               0,
               NULL,
               &MemoryDiscoveredPpi
               );
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a %a gEfiPeiMemoryDiscoveredPpiGuid found, exit\n", HECI_TRANSPORT_DEBUG, __FUNCTION__));
      return EFI_SUCCESS;
    }
  }

  HeciTransportOld = (HECI_TRANSPORT_PRIVATE*) GetHeciTransportForHeciAccess (
                                                 HeciAccess,
                                                 &HeciTransportInstance
                                                 );

  // if the HeciTransport was found, then there is a need to reinstall it
  if (HeciTransportOld != NULL) {
    Reinstall = TRUE;
  }

  TransportHeciPpi = (EFI_PEI_PPI_DESCRIPTOR*) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (TransportHeciPpi == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  HeciTransport = AllocateZeroPool (sizeof (HECI_TRANSPORT_PRIVATE));
  if (HeciTransport == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if (Reinstall) {
    CopyMem (HeciTransport, HeciTransportOld, sizeof (HECI_TRANSPORT_PRIVATE));
    SetHeciTransportFunctions (HeciTransport);
    Status = EFI_SUCCESS;
  } else {
    Status = HeciInitializeNewTransportInstance (
               HeciTransport,
               HeciAccess
               );
    ASSERT_EFI_ERROR (Status);
  }

  if (!EFI_ERROR (Status)) {
    // Add link to selected HeciAccess
    if (!Reinstall) {
      HeciTransport->HeciAccessIndex = GetPpiInstanceIndex (
                                         (VOID*)HeciAccess,
                                         &gHeciAccessPpiGuid
                                         );
    }

    // Create new HeciTransport PPI
    TransportHeciPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    TransportHeciPpi->Guid  = &gHeciTransportPpiGuid;
    TransportHeciPpi->Ppi   = HeciTransport;

    if (Reinstall) {
      Status = PeiServicesLocatePpi (
                 &gHeciTransportPpiGuid,
                 HeciTransport->HeciAccessIndex,
                 &HeciTransportOldPpi,
                 &HeciTransportOld
                 );
      ASSERT_EFI_ERROR (Status);
      if (!EFI_ERROR (Status)) {
        Status = PeiServicesReInstallPpi (HeciTransportOldPpi, TransportHeciPpi);
      }
    } else {
      Status = PeiServicesInstallPpi (TransportHeciPpi);
    }
    ASSERT_EFI_ERROR (Status);
    DEBUG ((
      DEBUG_INFO,
      "%a HeciTransport %ainstalled (for HeciAccess instance: %d)\n",
      HECI_TRANSPORT_DEBUG,
      Reinstall ? "re" : "",
      HeciTransport->HeciAccessIndex
      ));
  }
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    FreePool (TransportHeciPpi);
    TransportHeciPpi = NULL;
    FreePool (HeciTransport);
    HeciTransport = NULL;
  }

  ASSERT (HeciTransport != NULL);

  return Status;
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
HeciTransportEntryPoint (
  IN       EFI_PEI_FILE_HANDLE   FileHandle,
  IN CONST EFI_PEI_SERVICES      **PeiServices
  )
{
  EFI_STATUS                     Status;

  DEBUG ((DEBUG_INFO, "%a %a\n", HECI_TRANSPORT_DEBUG, __FUNCTION__));

  InstallHeciTransportPpis ();

  // Register a callback  to reinitialize PPI
  Status = PeiServicesNotifyPpi (&mReinitializeHeciPpiPeiNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a ERROR installing callback %r.\n",
      HECI_TRANSPORT_DEBUG,
      Status
      ));
    ASSERT_EFI_ERROR (Status);
    Status = EFI_SUCCESS;
  }

  DEBUG ((
    DEBUG_INFO,
    "%a HECI Transport PEI Exit : %r.\n",
    HECI_TRANSPORT_DEBUG,
    Status
    ));

  return Status;
}
