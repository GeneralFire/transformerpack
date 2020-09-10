/** @file
  Null instance of Sec Platform Hook Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2018 Intel Corporation. <BR>

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

//
// The package level header files this module uses
//
#include "ProcessorStartup.h"
#include <Library/MtrrLib.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/PcdLib.h>
#include <Library/OemProcessorStartup.h>

EFI_PEI_CORE_FV_LOCATION_PPI mPeiCoreFvLocationPpi = {
  (VOID *) (UINTN) FixedPcdGet32 (PcdPeiCoreFv)
};

EFI_PEI_PPI_DESCRIPTOR  mPeiSecPlatformPpi[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPeiCoreFvLocationPpiGuid,
    &mPeiCoreFvLocationPpi
  },
};

/**
  A developer supplied function to perform platform specific operations.

  It's a developer supplied function to perform any operations appropriate to a
  given platform. It's invoked just before passing control to PEI core by SEC
  core. Platform developer may modify the SecCoreData and PPI list that is
  passed to PEI Core.

  @param  SecCoreData           The same parameter as passing to PEI core. It
                                could be overridden by this function.

  @return The final PPI list that platform wishes to passed to PEI core.

**/
EFI_PEI_PPI_DESCRIPTOR *
EFIAPI
SecPlatformMain (
  IN OUT   EFI_SEC_PEI_HAND_OFF        *SecCoreData
  )
{
  EFI_PEI_PPI_DESCRIPTOR      *PpiList;
  UINTN                       ReservedSize;

  PpiList = NULL;
  ReservedSize = ALIGN_VALUE (PcdGet32 (PcdPeiTemporaryRamRcHeapSize), SIZE_4KB);

  SecCoreData->PeiTemporaryRamBase   = (UINT8 *) SecCoreData->PeiTemporaryRamBase + ReservedSize;
  SecCoreData->PeiTemporaryRamSize  -= ReservedSize;

//APTIOV_SERVER_OVERRIDE_RC_START : Changes to Launch PeiCore from FSP
// Right now we dont publish gEfiPeiCoreFvLocationPpiGuid of FSP PeiCore as SecMain of UefiCpuPkg is not upgraded. 
//so the below code is commented out, come back to it later
#if 0
#if FSP_DISPATCH_MODE_ENABLE
  //
  // Use end of Heap as temp buffer, it will be copied by caller.
  // Do not use Stack, because it will cause wrong calculation on stack by PeiCore
  //
  PpiList = (VOID *)((UINTN)SecCoreData->PeiTemporaryRamBase + (UINTN)SecCoreData->PeiTemporaryRamSize - 0x1000);
  CopyMem (PpiList, mPeiSecPlatformPpi, sizeof(mPeiSecPlatformPpi));
#endif // FSP_DISPATCH_MODE_ENABLE
#endif
//APTIOV_SERVER_OVERRIDE_RC_END : Changes to Launch PeiCore from FSP

  PlatformSecMain_OemHook (SecCoreData, PpiList);

  return PpiList;
}


/**
  This interface conveys state information out of the Security (SEC) phase into PEI.

  @param  PeiServices               Pointer to the PEI Services Table.
  @param  StructureSize             Pointer to the variable describing size of the input buffer.
  @param  PlatformInformationRecord Pointer to the EFI_SEC_PLATFORM_INFORMATION_RECORD.

  @retval EFI_SUCCESS           The data was successfully returned.
  @retval EFI_BUFFER_TOO_SMALL  The buffer was too small.

**/
EFI_STATUS
EFIAPI
SecPlatformInformation (
  IN CONST EFI_PEI_SERVICES                      **PeiServices,
  IN OUT   UINT64                                *StructureSize,
  OUT      EFI_SEC_PLATFORM_INFORMATION_RECORD   *PlatformInformationRecord
  )
{
  UINT32  *BIST;
  UINT32  Size;
  UINT32  Count;
  UINT32  *TopOfCar = (UINT32 *)(PcdGet32 (PcdTemporaryRamBase) + PcdGet32 (PcdTemporaryRamSize));

  //
  // The entries of BIST information, together with the number of them,
  // reside in the bottom of stack, left untouched by normal stack operation.
  // This routine copies the BIST information to the buffer pointed by
  // PlatformInformationRecord for output.
  //
  Count = *(UINT32*)(TopOfCar - (sizeof (UINT32)*4) - 1);
  Size  = Count * sizeof (UINT32);

  if ((*StructureSize) < (UINT64) Size) {
    *StructureSize = Size;
    return EFI_BUFFER_TOO_SMALL;
  }

  *StructureSize  = Size;
  BIST            = (UINT32*)(TopOfCar - (sizeof (UINT32)*4) - 1 - Count);

  CopyMem (PlatformInformationRecord, BIST, Size);
  PlatformInformationSecMain_OemHook (PeiServices, StructureSize, PlatformInformationRecord);

  return EFI_SUCCESS;
}

/**
  This interface disables temporary memory in SEC Phase.
**/
VOID
EFIAPI
SecPlatformDisableTemporaryMemory (
  VOID
  )
{
  UINT64  MsrValue;
  UINT64  MtrrDefaultType;
  MSR_IA32_MTRR_DEF_TYPE_REGISTER   DefType;

  //
  // Force and INVD.
  //
  AsmInvd ();

  //
  // Disable MTRRs.
  //
  DefType.Uint64 = AsmReadMsr64 (MSR_IA32_MTRR_DEF_TYPE);
  MtrrDefaultType = DefType.Uint64;
  DefType.Bits.E = 0;
  AsmWriteMsr64 (MSR_IA32_MTRR_DEF_TYPE, DefType.Uint64);

  //
  // Force and INVD to prevent MCA error.
  //
  AsmInvd ();

  //
  // Clear NEM Run and NEM Setup bits individually.
  //
  MsrValue = AsmReadMsr64 (MSR_NEM);
  MsrValue &= ~((UINT64) BIT1);
  AsmWriteMsr64 (MSR_NEM, MsrValue);
  MsrValue &= ~((UINT64) BIT0);
  AsmWriteMsr64 (MSR_NEM, MsrValue);

  //
  // Restore MTRR default setting
  //
  AsmWriteMsr64 (MSR_IA32_MTRR_DEF_TYPE, MtrrDefaultType);
}

