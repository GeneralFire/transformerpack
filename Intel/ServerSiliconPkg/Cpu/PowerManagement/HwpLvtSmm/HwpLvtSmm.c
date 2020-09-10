/** @file
  This is the SMM driver HWP SMI interrupt

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

#include "HwpLvtSmm.h"
#include <Library/IoLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <PchAccess.h>
#include <Library/PmcLib.h>

BIOS_ACPI_PARAM   *mAcpiParameter;

/**
  Checks for a HWP interrupt by reading MSR.

  This function must be MP safe.

  @param[in] Buffer    Pointer to Buffer -return HWP status
**/
VOID
EFIAPI
HwpLvtEventCheckMsr (
  IN VOID *Buffer
  )
{
  UINT32  *HwpEvent;
  ICX_MSR_IA32_HWP_STATUS_REGISTER HwpStatusMsr;

  ///
  /// Cast to enhance readability.
  ///
  HwpEvent = (UINT32 *) Buffer;
  HwpStatusMsr.Uint64 = AsmReadMsr64 (MSR_IA32_HWP_STATUS);
  if(HwpStatusMsr.Bits.ChangeToGuaranteed | HwpStatusMsr.Bits.ExcursionToMinimum) {
    *HwpEvent = 1;
    mAcpiParameter->HwpInterrupt |= TRUE;    //update Acpi parameter
  }
  return;
}

/**
  Clear HWP interrupt Status

  @param[in] Buffer
**/
VOID
EFIAPI
HwpLvtEventClearStatus (
  IN VOID *Buffer
  )
{
  ICX_MSR_IA32_HWP_STATUS_REGISTER HwpStatusMsr;

  HwpStatusMsr.Uint64 = AsmReadMsr64 (MSR_IA32_HWP_STATUS);
  HwpStatusMsr.Bits.ChangeToGuaranteed = 0;
  HwpStatusMsr.Bits.ExcursionToMinimum = 0;
  AsmWriteMsr64 (MSR_IA32_HWP_STATUS, HwpStatusMsr.Uint64);
  return;
}

/**
  Enable/Disable HWP Interrupt


  @param[in] Buffer    Pointer to Buffer -return HWP status
**/
VOID
EFIAPI
EnableDisableHwpInterrupt(
  IN VOID *HwpInterruptEnableFlag
  )
{
  MSR_IA32_HWP_INTERRUPT_REGISTER   HwpIntMsr;
  ///
  /// Set/Clear bits 0 and 1 of MSR IA32_HWP_INTERRUPT (773h) to enable/disable HWP interrupts
  ///
  HwpIntMsr.Uint64 = AsmReadMsr64 (MSR_IA32_HWP_INTERRUPT);
  if (*((UINT32 *)HwpInterruptEnableFlag)== 1) {
    HwpIntMsr.Bits.EN_Guaranteed_Performance_Change = 1;
    HwpIntMsr.Bits.EN_Excursion_Minimum = 1;
  } else {
    HwpIntMsr.Bits.EN_Guaranteed_Performance_Change = 0;
    HwpIntMsr.Bits.EN_Excursion_Minimum = 0;
  }
  AsmWriteMsr64 (MSR_IA32_HWP_INTERRUPT, HwpIntMsr.Uint64);

  return;
}

/**
  SMI handler to handle HWP CPU Local APIC SMI
  for HWP LVT interrupt

  @param[in] SmmImageHandle        Image handle returned by the SMM driver.
  @param[in] CommunicationBuffer   Pointer to the buffer that contains the communication Message
  @param[in] SourceSize            Size of the memory image to be used for handler.

  @retval EFI_SUCCESS           Callback Function Executed
**/
EFI_STATUS
EFIAPI
HwpLvtSmiCallback (
  IN EFI_HANDLE  SmmImageHandle,
  IN CONST VOID  *ContextData,         OPTIONAL
  IN OUT VOID    *CommunicationBuffer, OPTIONAL
  IN OUT UINTN   *SourceSize           OPTIONAL
)
{
  UINT32  HwpEvent;
  UINT32  HwpInterrupt;
  UINTN   Thread;

  ///
  /// Check whether SMI is due to HWP interrupt.
  ///
  HwpEvent = 0;
  mAcpiParameter->HwpInterrupt |= FALSE;

  for (Thread = 0; Thread < gSmst->NumberOfCpus; Thread++) {
    gSmst->SmmStartupThisAp (HwpLvtEventCheckMsr,
                             Thread,
                             &HwpEvent);
  }

  if(HwpEvent == 1) {
    ///
    /// Disable HWP Interrupts
    ///
    HwpInterrupt = 0;
    for (Thread = 0; Thread < gSmst->NumberOfCpus; Thread++) {
      gSmst->SmmStartupThisAp (EnableDisableHwpInterrupt,
                             Thread,
                             &HwpInterrupt);
    }
    ///
    /// If HWP interrupt Assert SCI
    ///
    PmcSetSwGpeSts();

    ///
    /// Clear the HWP Interrupt Status
    ///
    for (Thread = 0; Thread < gSmst->NumberOfCpus; Thread++) {
      gSmst->SmmStartupThisAp (HwpLvtEventClearStatus,
                             Thread,
                             NULL);
    }
    ///
    /// Enable HWP Interrupts
    ///
    HwpInterrupt = 1;
    for (Thread = 0; Thread < gSmst->NumberOfCpus; Thread++) {
      gSmst->SmmStartupThisAp (EnableDisableHwpInterrupt,
                             Thread,
                             NULL);
    }
  }

  return EFI_SUCCESS;
}

/**
  Initialize the HWP SMI Handler.

  @param[in] ImageHandle   - Pointer to the loaded image protocol for this driver
  @param[in] SystemTable   - Pointer to the EFI System Table

  @retval EFI_SUCCESS   The driver installes/initialized correctly.
**/

EFI_STATUS
EFIAPI
HwpLvtSmmInitialization (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS              Status;
  BOOLEAN                 InSmm;
  EFI_GLOBAL_NVS_AREA_PROTOCOL    *AcpiNvsProtocol;
  EFI_HANDLE             Handle;
  EFI_SMM_BASE2_PROTOCOL *SmmBase;


  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID **) &SmmBase);
  ASSERT_EFI_ERROR (Status);

  Status = SmmBase->InSmm (SmmBase, &InSmm);
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }
  if (!InSmm) {
    return EFI_SUCCESS;
  }

  ///
  /// Locate CPU global NVS area
  ///
  Status = gBS->LocateProtocol (&gEfiGlobalNvsAreaProtocolGuid, NULL, (VOID **) &AcpiNvsProtocol);
  ASSERT_EFI_ERROR (Status);

  mAcpiParameter = AcpiNvsProtocol->Area;

  if((AcpiNvsProtocol->Area->HWPMEnable) && (AcpiNvsProtocol->Area->HwpInterrupt)) {
    Status = gSmst->SmiHandlerRegister (HwpLvtSmiCallback, NULL, &Handle);
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}
