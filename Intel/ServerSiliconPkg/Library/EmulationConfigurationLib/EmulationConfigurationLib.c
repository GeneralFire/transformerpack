/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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
#include <Library/HobLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <EmulationConfiguration.h>
#include <Library/SysHostPointerLib.h>

/**

  Get Emulation hob or Create Emulation Hob if it is not exist

  @param[ in ]               A pointer to the Emulation Setting Structure

  @retval EFI_SUCCESS        The function completed successfully
  @retval EFI_NOT_FOUND      Hob not found.

**/
EFI_STATUS
EFIAPI
GetEmulationHobData (
  EMULATION_SETTING       **EmulationSetting
  )
{
  EFI_HOB_GUID_TYPE    *GuidHob = NULL;

  *EmulationSetting = (EMULATION_SETTING *)GetEmulationSettingPointer();
  if (NULL != *EmulationSetting) {
    return EFI_SUCCESS;
  }

  //
  // Get Emulation hob
  //
  GuidHob     = GetFirstGuidHob (&gEmulationHobGuid);
  if (GuidHob != NULL) {
    *EmulationSetting = GET_GUID_HOB_DATA (GuidHob);
    SetEmulationSettingPointer(*EmulationSetting);
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}


/**

   Return if Hybrid System Level Emulation (HSLE) is enabled or not

  @param none

  @retval True     Hybrid System Level Emulation is enabled
  @retval False    Hybrid System Level Emulation is disabled

**/
BOOLEAN
EFIAPI
HybridSystemLevelEmulationEnabled (
  VOID
  )
{
  EMULATION_SETTING       *EmulationSetting;
  EFI_STATUS              Status;

  Status = GetEmulationHobData (&EmulationSetting);
  if ((!EFI_ERROR (Status)) && (EmulationSetting != NULL)) {
    if (EmulationSetting->HybridSystemLevelEmulationSetting == EMULATION_ENABLE) {
      return TRUE;
    }
  }
  return FALSE;
}

/**

   Return if Ubios Generation (SLE) is enabled or not

  @param none

  @retval True     Ubios Generation is enabled
  @retval False    Ubios Generation is disabled

**/
BOOLEAN
EFIAPI
UbiosGenerationEnabled (
  VOID
  )
{
  EMULATION_SETTING       *EmulationSetting;
  EFI_STATUS              Status;

  Status = GetEmulationHobData (&EmulationSetting);
  if ((!EFI_ERROR (Status)) && (EmulationSetting != NULL)) {
    if (EmulationSetting->UbiosGenerationSetting == EMULATION_ENABLE) {
      return TRUE;
    }
  }
  return FALSE;
}

/**

   Check the setting of Ubios Generation (SLE) and Hybrid System Level Emulation.

  @param none

  @retval True     Ubios Generation or Hybrid System Level Emulation is enabled
  @retval False    Ubios Generation or Hybrid System Level Emulation is disabled

**/
BOOLEAN
EFIAPI
UbiosGenerationOrHsleEnabled (
  VOID
  )
{
  if (UbiosGenerationEnabled () || HybridSystemLevelEmulationEnabled ()) {
    return TRUE;
  }
  return FALSE;
}

/**

   Get the uBIOS output mode

  @param    none

  @retval   uBIOS output mode

**/

UINT8
EFIAPI
GetUbiosOutputMode (
  VOID
  )
{
  EMULATION_SETTING       *EmulationSetting;
  EFI_STATUS              Status;

  Status = GetEmulationHobData (&EmulationSetting);
  if ((!EFI_ERROR (Status)) && (EmulationSetting != NULL)) {
    if (EmulationSetting->UbiosGenerationSetting == EMULATION_ENABLE) {
      return EmulationSetting->UbiosOutputMode;
    }
  }
  return ASM_OUTPUT_DISABLE;
}

/**

   Set the uBIOS output mode

  @param[in] Value         uBIOS output moder

  @retval EFI_SUCCESS      Success to set uBIOS output moder
  @retval Other            Fail to set uBIOS output moder

**/

EFI_STATUS
EFIAPI
SetUbiosOutputMode (
  IN UINT8 Value
  )
{
  EMULATION_SETTING       *EmulationSetting;
  EFI_STATUS              Status;

  Status = GetEmulationHobData (&EmulationSetting);
  if ((!EFI_ERROR (Status)) && (EmulationSetting != NULL)) {
    if (EmulationSetting->UbiosGenerationSetting == EMULATION_ENABLE) {
      EmulationSetting->UbiosOutputMode = Value;
      return EFI_SUCCESS;
    }
  }
  return EFI_UNSUPPORTED;
}


/**

   Get the Label Number which is used in the ASM output loop back

  @param[in] Value         Loop Back Label Number

  @retval EFI_SUCCESS      Success to get the Label Number
  @retval Other            Fail to get the Label Number

**/

EFI_STATUS
EFIAPI
GetUbiosLoopbackLabelNumber (
  OUT UINT32 *Value
  )
{
  EMULATION_SETTING       *EmulationSetting;
  EFI_STATUS              Status;

  if(Value == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetEmulationHobData (&EmulationSetting);
  if ((!EFI_ERROR (Status)) && (EmulationSetting != NULL)) {
    if (EmulationSetting->UbiosGenerationSetting == EMULATION_ENABLE) {
      *Value = EmulationSetting->LoopBackLabelNumber;
      return EFI_SUCCESS;
    }
  }
  return EFI_UNSUPPORTED;
}

/**

   Set the Label Number which is used in the ASM output loop back

  @param[in] Value         Loop Back Label Number

  @retval EFI_SUCCESS      Success to set the Label Number
  @retval Other            Fail to set the Label Number

**/

EFI_STATUS
EFIAPI
SetUbiosLoopbackLabelNumber (
  IN UINT32 Value
  )
{
  EMULATION_SETTING       *EmulationSetting;
  EFI_STATUS              Status;

  Status = GetEmulationHobData (&EmulationSetting);
  if ((!EFI_ERROR (Status)) && (EmulationSetting != NULL)) {
    if (EmulationSetting->UbiosGenerationSetting == EMULATION_ENABLE) {
      EmulationSetting->LoopBackLabelNumber = Value;
      return EFI_SUCCESS;
    }
  }
  return EFI_UNSUPPORTED;
}

/**

   Get the value of FnvAccess

  @param    none

  @retval   FnvAccess value

**/

UINT8
EFIAPI
GetUbiosFnvAccessValue (
  VOID
  )
{
  EMULATION_SETTING       *EmulationSetting;
  EFI_STATUS              Status;

  Status = GetEmulationHobData (&EmulationSetting);
  if ((!EFI_ERROR (Status)) && (EmulationSetting != NULL)) {
    if (EmulationSetting->UbiosGenerationSetting == EMULATION_ENABLE) {
      return EmulationSetting->FnvAccessValue;
    }
  }
  return FNV_ACCESS_DISABLE;
}

/**

   Set the value of FnvAccess

  @param[in] Value         Loop Back Label Number

  @retval EFI_SUCCESS      Success to set the value of FnvAccess
  @retval Other            Fail to set the value of FnvAccess

**/

EFI_STATUS
EFIAPI
SetUbiosFnvAccessValue (
  IN UINT8 Value
  )
{
  EMULATION_SETTING       *EmulationSetting;
  EFI_STATUS              Status;

  Status = GetEmulationHobData (&EmulationSetting);
  if ((!EFI_ERROR (Status)) && (EmulationSetting != NULL)) {
    if (EmulationSetting->UbiosGenerationSetting == EMULATION_ENABLE) {
      EmulationSetting->FnvAccessValue = Value;
      return EFI_SUCCESS;
    }
  }
  return EFI_UNSUPPORTED;
}

/**

   Return if Msr Trace is enabled or not

  @param none

  @retval True     Msr Trace is enabled
  @retval False    Msr Trace is disabled

**/

BOOLEAN
MsrTraceEnabled (
  VOID
  )
{
  EMULATION_SETTING       *EmulationSetting;
  EFI_STATUS              Status;

  Status = GetEmulationHobData (&EmulationSetting);
  if ((!EFI_ERROR (Status)) && (EmulationSetting != NULL)) {
    if ((EmulationSetting->UbiosGenerationSetting == EMULATION_ENABLE) && (EmulationSetting->MsrTraceEnable == EMULATION_ENABLE)) {
      return TRUE;
    }
  }
  return FALSE;
}


/**

   Get the Msr Trace output mode

  @param    none

  @retval   Msr Trace output mode

**/

UINT8
EFIAPI
GetMsrTraceOutputMode (
  VOID
  )
{
  EMULATION_SETTING       *EmulationSetting;
  EFI_STATUS              Status;

  Status = GetEmulationHobData (&EmulationSetting);
  if ((!EFI_ERROR (Status)) && (EmulationSetting != NULL)) {
    if ((EmulationSetting->UbiosGenerationSetting == EMULATION_ENABLE) && (EmulationSetting->MsrTraceEnable == EMULATION_ENABLE)) {
      return EmulationSetting->MsrTraceOutputMode;
    }
  }
  return MSR_OUTPUT_DISABLE;
}

/**

   Set the Msr Trace output mode

  @param[in] Value         Msr Trace output moder

  @retval EFI_SUCCESS      Success to set Msr Trace output moder
  @retval Other            Fail to set Msr Trace output moder

**/

EFI_STATUS
EFIAPI
SetMsrTraceOutputMode (
  IN UINT8 Value
  )
{
  EMULATION_SETTING       *EmulationSetting;
  EFI_STATUS              Status;

  Status = GetEmulationHobData (&EmulationSetting);
  if ((!EFI_ERROR (Status)) && (EmulationSetting != NULL)) {
    if ((EmulationSetting->UbiosGenerationSetting == EMULATION_ENABLE) && (EmulationSetting->MsrTraceEnable == EMULATION_ENABLE)) {
      EmulationSetting->MsrTraceOutputMode = Value;
      return EFI_SUCCESS;
    }
  }
  return EFI_UNSUPPORTED;
}

/**

  Prints address as an ASM DD section. Used for SGX and MKTME uBIOS log captures of sections of code.

  @param RegionName  - Name for the section

  @retval None

**/
VOID
DataRegionAsmDump (
  IN UINT8 *RegionName,
  IN UINT8 *address,
  UINT64    size
  )

{
  UINT32       Index;
  UINT8       *Ptr;

  if (!MsrTraceEnabled ()) {
    return;
  }
  Ptr = address;

  DEBUG ((EFI_D_ERROR, "\n%a START\n", RegionName));
  DEBUG ((EFI_D_ERROR, "\nADDRESS %08xh\n", address));

  if((size % 4) == 0) {
    for (Index = 0; Index < (size / 4); Index++) {
      DEBUG ((EFI_D_ERROR, "dd 0%08xh\n", *(UINT32*)(Ptr + (Index*4))));
    }
  } else if((size % 2) == 0) {
    for (Index = 0; Index < (size / 2); Index++) {
      DEBUG ((EFI_D_ERROR, "dw 0%04xh\n", *(UINT16*)(Ptr + (Index*2))));
    }
  } else {
    for (Index = 0; Index < size ; Index++) {
      DEBUG ((EFI_D_ERROR, "db 0%02xh\n", *(UINT8*)(Ptr + Index)));
    }
  }

  DEBUG ((EFI_D_ERROR, "\n%a END\n", RegionName));

}

/**

  EmulationPolling is an assembly polling loop for reading CSRs
  EmulationPolling allows you to poll multiple bits

  @param[in] *Address    - Address to read
  @param[in] Mask        - Bit mask for the value read
  @param[in] Logic       - polling for the register bit to become 0 or 1
  @param[in] PowerGood   - Reset Type for choosing emulation loop label.

  @retval N/A

**/
VOID
EFIAPI
EmulationPolling (
  IN UINT8    *Address,
  IN UINT32   Mask,
  IN UINT8    Logic,
  IN BOOLEAN  PowerGood
  )
{
  UINT32     LabelNumber;
  DEBUG ((DEBUG_ERROR, "Emulation Polling Start\n"));
  if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
    if (PowerGood) {
      DEBUG ((DEBUG_ERROR, "\nLABEL_c_mbox%d:\n", LabelNumber));
    } else {
      DEBUG ((DEBUG_ERROR, "\nLABEL_w_mbox%d:\n", LabelNumber));
    }
    SetUbiosLoopbackLabelNumber (LabelNumber + 1);
    DEBUG ((DEBUG_ERROR, "mov eax, DWORD PTR ds:[0%8xh]\n", Address));
    if (Logic == 0) {
      DEBUG ((DEBUG_ERROR, "not eax\n"));
    }
    DEBUG ((DEBUG_ERROR, "and eax, 0%08xh\n", Mask));
    DEBUG ((DEBUG_ERROR, "cmp eax, 0%08xh\n", Mask));
    if (PowerGood) {
      DEBUG ((DEBUG_ERROR, "jnz LABEL_c_mbox%d\n", LabelNumber));
    } else {
      DEBUG ((DEBUG_ERROR, "jnz LABEL_w_mbox%d\n", LabelNumber));
    }
  }
  DEBUG ((DEBUG_ERROR, "Emulation Polling End\n"));
}

/**

  Surround ubios call print with scope. This is for the ubios replay table support.

  @param CallName   Str with the name of the function/proc to call
  @param Scope      The call should be made in the defined scope.
                    SCOPE_AUTO - Default/global scope is used so delimiters for scope are skipped.
                    SCOPE_THREAD - use THREAD SCOPE which will cause it to run on all available threads
                    SCOPE_CORE - use CORE SCOPE which will cause it to run on all available cores
                    SCOPE_PACKAGE - use CORE SCOPE which will cause it to run on all available PACKAGES
                    SCOPE_BSP - only BSP will execute
  @retval NONE
**/
EFI_STATUS
UbiosCall (
  IN CHAR8 *CallName,
  IN UINT8 Scope
  )
{
  CHAR8 *ScopeMap[SCOPE_MAX] = {"bsp","thread","core","package", "auto"};
  BOOLEAN ScopeDelim;
  CHAR8 *ScopeName;

  if(Scope >= SCOPE_MAX || CallName == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ScopeDelim = Scope != SCOPE_AUTO;
  ScopeName = ScopeMap[Scope];

  if(ScopeDelim) {
    DEBUG((EFI_D_ERROR, ";SCOPE_BEGIN\n"));
    DEBUG((EFI_D_ERROR, ";scope: %a\n", ScopeName));
  }
  DEBUG((EFI_D_ERROR, "call %a\n", CallName));
  if(ScopeDelim) {
    DEBUG((EFI_D_ERROR, ";SCOPE_END\n"));
  }
  return EFI_SUCCESS;
}
