/** @file
  PlatformPowerButton.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2017 Intel Corporation. <BR>

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

#include <Protocol/SmmPowerButtonDispatch2.h>
#include <Library/BaseLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PmcLib.h>
#include <Register/PchRegsPmc.h>
#include <Register/PchRegsLpc.h>
#include <PchAccess.h>
#include <Library/MmPciBaseLib.h>

/**
  When a power button event happens, it shutdown the machine
  
Arguments:

  @param[in] DispatchHandle       Dispatch handle
  @param[in] Context              Context information
  @param[in] CommBuffer           Buffer used for communication between caller/callback function
  @param[in] CommBufferSize       Size of communication buffer

  @retval Status.

**/
EFI_STATUS 
EFIAPI
PowerButtonHandler(
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID  *Context         OPTIONAL,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
  )
{
  UINT16  PchPmioBase;
  UINT16  Data16;
  UINT32  Data32;

  PchPmioBase = PmcGetAcpiBase();

  //
  // First, GPE0_EN should be disabled to prevent any GPI waking up the system from S5
  // 
  Data16 = 0;

  IoWrite16 ((UINTN)(PchPmioBase + R_ACPI_IO_GPE0_EN_127_96), (UINT16)Data16);

  //
  // Second, PwrSts register must be clear
  //  


  Data16 = IoRead16 ((UINTN)(PchPmioBase + R_ACPI_IO_PM1_STS));
  Data16 |= B_ACPI_IO_PM1_STS_PWRBTN;
  IoWrite16 ((UINTN)(PchPmioBase + R_ACPI_IO_PM1_STS), (UINT16)Data16);

  //
  // Finally, transform system to S5 sleep state
  //
  Data32 = IoRead32 ((UINTN)(PchPmioBase + R_ACPI_IO_PM1_CNT));
  Data32  = (UINT32) ((Data32 & ~(B_ACPI_IO_PM1_CNT_SLP_TYP  + B_ACPI_IO_PM1_CNT_SLP_EN)) | B_ACPI_IO_PM1_CNT_SLP_TYP);

  DEBUG ((DEBUG_INFO, "Write S5 to SLP_TYPE in PM1_CNT\n"));
  IoWrite32 ((UINTN) (PchPmioBase + R_ACPI_IO_PM1_CNT), (UINT32)Data32);

  Data32 = Data32 | B_ACPI_IO_PM1_CNT_SLP_EN;

  DEBUG ((DEBUG_INFO, "Write SLP_EN in PM1_CNT to go to S5\n"));
  IoWrite32 ((UINTN) (PchPmioBase + R_ACPI_IO_PM1_CNT), (UINT32)Data32);

  return EFI_SUCCESS;
}

/**
  The Driver Entry Point.
  
  The function is the driver Entry point.
  
  @param ImageHandle   A handle for the image that is initializing this driver
  @param SystemTable   A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @retval EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  @retval EFI_OUT_OF_RESOURCES      Could not allocate needed resources

**/
EFI_STATUS 
EFIAPI
PowerButtonHandlerEntry(
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                              Status;
  EFI_HANDLE                              DispatchHandle;
  EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL *SmmPowerbuttonDispatch2;
  EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT   Context = {EfiPowerButtonEntry};

  Status = gSmst->SmmLocateProtocol(
                    &gEfiSmmPowerButtonDispatch2ProtocolGuid,
                    NULL,
                    &SmmPowerbuttonDispatch2
                    );
  if (!EFI_ERROR (Status)) {
    Status = SmmPowerbuttonDispatch2->Register(
                                      SmmPowerbuttonDispatch2,
                                      PowerButtonHandler,
                                      &Context,
                                      &DispatchHandle
                                      );    
  }

  return Status;
}
