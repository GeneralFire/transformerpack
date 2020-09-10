/** @file
  Implementation of SystemInfoLib. These functions are for PEI phase

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/PcdLib.h>

/**
  This function returns the pointer to SYSTEM_INFO_VAR.

  @param VOID

  @retval SYSTEM_INFO_VAR*   The pointer to SYSTEM_INFO_VAR.
**/
SYSTEM_INFO_VAR *
GetSystemInfoVar (
  VOID
  )
{
  return (SYSTEM_INFO_VAR *) GetSysInfoVarPointer();
}

/**
  This API gets UsraPpiPtr info.

  @param  None

  @retval UINTN  The data for UsraPpiPtr returned.
**/
UINTN
EFIAPI
GetUsraPpiPtr (
  VOID
  )
{
  return GetSystemInfoVar()->UsraPpiPtr;
}

/**
  This API sets the value for UsraPpiPtr.

  @param[in] UINTN  Value to set for UsraPpiPtr.

  @retval None
**/
VOID
EFIAPI
SetUsraPpiPtr (
  IN UINTN  UsraPpiPtr
  )
{
  GetSystemInfoVar()->UsraPpiPtr = UsraPpiPtr;
  return;
}

/**
  Enable the posted CSR access method
  Subsequent CSR accesses will be sent using Posted semantics.

  A call to this function must always be paired with a call
  to PostedMethodDisable to avoid unintentionally sending all
  future CSR accesses using posted semantics.

  @retval EFI_UNSUPPORTED if posted accesses are not supported
  @retval EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
PostedMethodEnable (
  VOID
  )
{
  EFI_STATUS                  Status = EFI_UNSUPPORTED;
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;

  if (!PcdGetBool (PcdPostedCsrAccessSupported)) {
    return Status;
  }

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();

  if (CpuCsrAccessVarPtr->PostedCsrAccessAllowed) {
    Status = EFI_SUCCESS;
    if (CpuCsrAccessVarPtr->PostedWritesEnabled == TRUE) {
      DEBUG ((EFI_D_ERROR, "Warning - PostedMethodEnable called while PostedWritesEnable is already TRUE.\n"));
    } else {
      CpuCsrAccessVarPtr->PostedWritesEnabled = TRUE;
    }
  }

  return Status;
}

/**
  Disable the posted CSR access method
  Subsequent CSR writes will be sent using non-posted semantics

  @retval EFI_UNSUPPORTED if posted accesses are not supported
  @retval EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
PostedMethodDisable (
  VOID
  )
{
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;

  if (!PcdGetBool (PcdPostedCsrAccessSupported)) {
    return EFI_UNSUPPORTED;
  }

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();

  if (CpuCsrAccessVarPtr->PostedCsrAccessAllowed) {
    if (CpuCsrAccessVarPtr->PostedWritesEnabled == FALSE) {
      DEBUG ((EFI_D_ERROR, "Warning - PostedMethodDisable called while PostedWritesEnable is already FALSE.\n"));
    } else {
      CpuCsrAccessVarPtr->PostedWritesEnabled = FALSE;
    }
  }

  return EFI_SUCCESS;
}
