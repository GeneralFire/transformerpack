/** @file
  Implementation of IIO Error Handler.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2019 Intel Corporation. <BR>

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

#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/IehRasLib.h>
#include <Library/IioRasLib.h>
#include <Library/RasDebugLib.h>
#include <Library/RasMailBoxLib.h>
#include <Library/PlatPolicyLib.h>
#include <Protocol/SmmReadyToLock.h>
#include <Protocol/GlobalNvsArea.h>
#include <PCIeErrorTypes.h>
#include <Library/IioSiliconLib.h>
#include <Library/RasAcpiLib.h>
#include <Library/CpuAndRevisionLib.h>
#include "IioErrorHandlerPrivate.h"

BIOS_ACPI_PARAM                 *mAcpiParameter;

/**
  Entry point for the PCI Express Error Handler.

  This function initializes the error handling and enables PCI
  Express error sources.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval Status.
**/
EFI_STATUS
EFIAPI
InitializeIioErrorHandler (
  IN      EFI_HANDLE          ImageHandle,
  IN      EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_HANDLE                      Handle;
  VOID                            *Registration;
  EFI_GLOBAL_NVS_AREA_PROTOCOL    *AcpiNvsProtocol = NULL;
  RAS_POLICY                      *PolicyData = NULL;
  RAS_MAIL_BOX                    *MailBox = NULL;

  PolicyData = GetRasPolicy ();
  MailBox = GetRasMailBox ();
  ASSERT (MailBox);

  GetPcieInitPars (PolicyData, MailBox);
  GetIioInitPars (PolicyData, MailBox);

  if ((PolicyData->SystemErrorEn == FALSE) ||
      (PolicyData->IioPolicy.IioErrorEn == FALSE)) {
    //
    //in case some device register need to be disabled when iio setup item disable, please add here.
    //
    //IehErrorDisable();
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"IIO SETUP selected disabled! \n"));
    return EFI_NOT_READY;
  }

  if (IioRasDeviceExist () == FALSE) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW,"No Iio Ras device at system! IEH initialization exit."));
    return EFI_NOT_READY;
  }

  Status = gBS->LocateProtocol(
                  &gEfiGlobalNvsAreaProtocolGuid,
                  NULL,
                  &AcpiNvsProtocol
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "AcpiNvsProtocol is not installed\n", Status));
    return Status;
  }
  mAcpiParameter = AcpiNvsProtocol->Area;

  //
  // IIO Initialize RAS ACPI BIOS Parameters.
  //
  IioInitRasAcpiParam ();

  //
  //install IIO error handler.
  //
  if (PolicyData->IioPolicy.IioErrorEn) {
    Status = gSmst->SmiHandlerRegister (IioErrorDetectAndHandle, NULL, &Handle);
  }

  //
  //  Register IioErrorEnableCallBack to gEfiSmmEndOfDxeProtocolGuid event start
  // from eaglestream
  //
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    //
    //enable IIO Error CallBack funciton.
    //
    Status = gSmst->SmmRegisterProtocolNotify (
                      &gEfiSmmEndOfDxeProtocolGuid,
                      IioErrorEnableCallBack,
                      &Registration
                      );
  } else {
    //
    //enable IIO Error CallBack funciton.
    //
    Status = gSmst->SmmRegisterProtocolNotify (
                      &gEfiSmmReadyToLockProtocolGuid,
                      IioErrorEnableCallBack,
                      &Registration
                      );
  }
  return EFI_SUCCESS;
}
