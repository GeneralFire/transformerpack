/** @file
  This is a library to provide Heci PPI Service.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

@par Specification Reference:
**/

#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PostCodeLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HeciInitLib.h>
#include <Library/MeTypeLib.h>
#include <Ppi/HeciPpi.h>
#include <Ppi/HeciControlPpi.h>
#include <Register/HeciRegs.h>
#include <HeciLegacyCore.h>

/**
  Initialize MMIO BAR address for HECI devices
  For SPS HeciControl is used (it has self initialization)

  @retval EFI_SUCCESS             HECI devices initialize successfully
  @retval others                  Error occur
**/
EFI_STATUS
InstallHeciPpiNofify (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR mInstallHeciPpiNofifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gHeciControlPpiGuid,
    InstallHeciPpiNofify
  }
};

/**
  Internal function performing Heci PPIs init needed in PEI phase

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_DEVICE_ERROR        ME FPT is bad
**/
EFI_STATUS
InstallHeciPpiNofify (
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  EFI_STATUS              Status;
  HECI_CONTROL            *HeciControl;
  HECI_PPI                *HeciPpi;
  HECI_PPI                *HeciOldPpi;
  EFI_PEI_PPI_DESCRIPTOR  *HeciLegacyOldPpi;
  EFI_PEI_PPI_DESCRIPTOR  *HeciLegacyPpi;

  Status = PeiServicesLocatePpi (&gHeciControlPpiGuid, 0, NULL, (VOID**)&HeciControl);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ME-BIOS: HECI PPI Callback.\n"));
    PostCode (0xE03);
    HeciLegacyPpi = (EFI_PEI_PPI_DESCRIPTOR*)AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
    if (HeciLegacyPpi != NULL) {
      HeciPpi = AllocateZeroPool (sizeof (HECI_PPI));
      if (HeciPpi != NULL) {
        HeciPpi->SendwAck = HeciSendwAckWrapper;
        HeciPpi->ReadMsg = HeciReceiveWrapper;
        HeciPpi->SendMsg = HeciSendWrapper;
        HeciPpi->InitializeHeci = HeciInitializeWrapper;
        HeciPpi->GetMeStatus = HeciGetMeStatusWrapper;
        HeciPpi->GetMeMode = HeciGetMeModeWrapper;

        HeciLegacyPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
        HeciLegacyPpi->Guid = &gHeciPpiGuid;
        HeciLegacyPpi->Ppi = HeciPpi;
        Status = PeiServicesLocatePpi (&gHeciPpiGuid, 0, &HeciLegacyOldPpi, (VOID**)&HeciOldPpi);
        if (EFI_ERROR (Status)) {
          Status = PeiServicesInstallPpi (HeciLegacyPpi);
        } else {
          Status = PeiServicesReInstallPpi (HeciLegacyOldPpi, HeciLegacyPpi);
        }
      }
    }
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "ME-BIOS: HECI PPI Exit - Success.\n"));
      PostCode (0xE23);
    } else {
      DEBUG ((DEBUG_ERROR, "ME-BIOS: HECI PPI Exit - Error by install HeciPpi fail, Status: %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      PostCode (0xEA3);
    }
  } else {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: HECI PPI Callback Exit - Error by HECI device error.\n"));
    PostCode (0xE83);
  }

  return EFI_SUCCESS;
}

/**
  Internal function performing Heci PPIs init needed in PEI phase

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_DEVICE_ERROR        ME FPT is bad
**/
EFI_STATUS
InstallHeciPpi (
  VOID
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "ME-BIOS: HECI PPI Entry.\n"));
  PostCode (0xE03);

  Status = PeiServicesNotifyPpi (&mInstallHeciPpiNofifyList[0]);
  DEBUG ((DEBUG_INFO, "ME-BIOS: InstallHeciPpiNofify callback installation status %r\n", Status));
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ME-BIOS: HECI PPI Exit - Success installing Callback.\n"));
    PostCode (0xE23);
  } else {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: HECI PPI Exit - Error by install HeciPpi Callback fail, Status: %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    PostCode (0xEA3);
  }

  return Status;
}
