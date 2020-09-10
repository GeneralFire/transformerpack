/** @file
  Implementation of generate RAS global data access for silicon library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016-2020 Intel Corporation. <BR>

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

#include <Library/RasDebugLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/RasMailBoxLib.h>
#include <IndustryStandard/Acpi62.h>

RAS_MAIL_BOX  *mMailBox = NULL;
LIST_ENTRY    PtrPcieCeCounterList;

/**
  This function is implementated to return RAS_MAIL_BOX data.

  @return   Pointer to RAS_MAIL_BOX data.
**/
RAS_MAIL_BOX*
EFIAPI
GetRasMailBox (
  VOID
  )
{
  ASSERT (mMailBox);
  return mMailBox;
}

/**
  Returns the more severe of the two inputs.

  @param[in] OldSev   The old severity
  @param[in] NewSev   The newer severity

  @return The more severe of the two severities (OldDev / NewSev)

**/
UINT32
GetHighestSeverity (
  IN      UINT32    OldSev,
  IN      UINT32    NewSev
  )
{
  switch (NewSev) {
    case EFI_ACPI_6_2_ERROR_SEVERITY_FATAL:
      return NewSev;
      break;

    case EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTABLE:
      if (OldSev == EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED || OldSev == EFI_ACPI_6_2_ERROR_SEVERITY_NONE) {
        return NewSev;
      }
      break;

    case EFI_ACPI_6_2_ERROR_SEVERITY_CORRECTED:
      if (OldSev == EFI_ACPI_6_2_ERROR_SEVERITY_NONE) {
        return NewSev;
      }
      break;

    case EFI_ACPI_6_2_ERROR_SEVERITY_NONE:
      return OldSev;

    default:
      RAS_ASSERT (FALSE);
      break;
  }
  return OldSev;
}

/**
  This implementation is to Adjust output severity for interrupt.

  @param[in]  Severity  -- severity value.
**/
VOID
EFIAPI
AdjustOutputSeverity (
  UINT32  Severity
  )
{
  mMailBox->MiscPar.OutputSeverity = GetHighestSeverity (mMailBox->MiscPar.OutputSeverity, Severity);
  return;
}

/**
  This implementation is to override output severity for interrupt.

  @param[in]  Severity  -- severity value.
**/
VOID
EFIAPI
OverrideOutputSeverity (
  UINT32  Severity
  )
{
  mMailBox->MiscPar.OutputSeverity = Severity;
  return;
}

/**
  This implementation is to override output severity for interrupt.

  @param   none

  return  current global severity
**/
UINT32
EFIAPI
GetOutputSeverity (
  VOID
  )
{
  return mMailBox->MiscPar.OutputSeverity;
}

/**

  The constructor function initialize RAS MailBox library.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
InitRasMailBox (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
  BOOLEAN  NewAllocation;

  mMailBox = GetRasGlobalData (L"RasMailBox", sizeof (RAS_MAIL_BOX), &NewAllocation);
  if (NewAllocation){
    mMailBox->MiscPar.OutputSeverity = EFI_ACPI_6_2_ERROR_SEVERITY_NONE;
    mMailBox->MiscPar.PtrPcieCeCounterList = &PtrPcieCeCounterList;
    InitializeListHead (mMailBox->MiscPar.PtrPcieCeCounterList);
  }
  return EFI_SUCCESS;
}

