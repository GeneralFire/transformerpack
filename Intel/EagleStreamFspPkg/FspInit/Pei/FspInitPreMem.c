/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2019 Intel Corporation. <BR>

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

#include "BootMode.h"
#include <Ppi/ReadOnlyVariable2.h>
#include "FspInitPreMem.h"
#include <CpuRegs.h>
#include <Library/MemoryAllocationLib.h>

#define EFI_MAX_ADDRESS   0xFFFFFFFF        /* Definition in EfiBind.h */
#define PEI_MIN_MEMORY_SIZE             (EFI_PHYSICAL_ADDRESS) ((320 * 0x100000))

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR mPpiBootMode = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMasterBootModePpiGuid,
  NULL
};

EFI_PEI_NOTIFY_DESCRIPTOR mMemoryDiscoveredNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  MemoryDiscoveredPpiNotifyCallback
};


PEI_PLATFORM_MEMORY_SIZE_PPI mMemoryMemorySizePpi = {
    GetPlatformMemorySize
};

PEI_BASE_MEMORY_TEST_PPI     mPeiBaseMemoryTestPpi = {
    BaseMemoryTest
};

EFI_PEI_STALL_PPI            mStallPpi = {
    PEI_STALL_RESOLUTION,
    Stall
};

EFI_PEI_RESET_PPI            mResetPpi = {
    ResetSystem
};

EFI_PEI_PPI_DESCRIPTOR       mPeiPlatformPpis[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI),
    &gEfiPeiStallPpiGuid,
    &mStallPpi
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gPeiBaseMemoryTestPpiGuid,
    &mPeiBaseMemoryTestPpi
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gPeiPlatformMemorySizePpiGuid,
    &mMemoryMemorySizePpi
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiResetPpiGuid,
    &mResetPpi
  }
};

EFI_PEI_PPI_DESCRIPTOR           mPeiCpuMpPpiDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMpServicesPpiGuid,
  NULL
};


EFI_MEMORY_TYPE_INFORMATION mDefaultMemoryTypeInformation[] = {
//  {
//    EfiACPIReclaimMemory,
//    0x100
//  },
//  {
//    EfiACPIMemoryNVS,
//    0xA30
//  },
//  {
//    EfiReservedMemoryType,
//    0x100
//  },
//  {
//    EfiRuntimeServicesCode,
//    0x100
//  },
//  {
//    EfiRuntimeServicesData,
//    0x100
//  },
  {
    EfiMaxMemoryType,
    0
  }
};

/**

  Build Memory type information.

  @param PeiServices - Pointer to Pei Services.
  @param This        - GC_TODO: add argument description
  @param MemorySize  - GC_TODO: add argument description

  @retval EFI_SUCCESS  -  Success.
  @retval Others       -  Errors have occurred.
**/
EFI_STATUS
EFIAPI
GetPlatformMemorySize (
  IN      EFI_PEI_SERVICES                       **PeiServices,
  IN      PEI_PLATFORM_MEMORY_SIZE_PPI           *This,
  IN OUT  UINT64                                 *MemorySize
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *Variable;
  UINTN                           DataSize;
  EFI_MEMORY_TYPE_INFORMATION     MemoryData[EfiMaxMemoryType + 1];
  UINTN                           Index;
  EFI_BOOT_MODE                   BootMode;
  UINTN                           IndexNumber;
  UINTN                           *HobPointer;

  *MemorySize = PEI_MIN_MEMORY_SIZE;
  Status = PeiServicesLocatePpi (
           &gEfiPeiReadOnlyVariable2PpiGuid,
            0,
            NULL,
            &Variable
            );

  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  DataSize = sizeof (MemoryData);

  Status = Variable->GetVariable (
                      Variable,
                      EFI_MEMORY_TYPE_INFORMATION_VARIABLE_NAME,
                      &gEfiMemoryTypeInformationGuid,
                      NULL,
                      &DataSize,
                      &MemoryData
                      );

  IndexNumber = sizeof (mDefaultMemoryTypeInformation) / sizeof (EFI_MEMORY_TYPE_INFORMATION);

  //
  // Accumulate maximum amount of memory needed
  //

  DEBUG((EFI_D_ERROR, "PEI_MIN_MEMORY_SIZE:%dKB \n", DivU64x32(*MemorySize,1024)));
  DEBUG((EFI_D_ERROR, "IndexNumber:%d MemoryDataNumber%d \n", IndexNumber,DataSize/ sizeof (EFI_MEMORY_TYPE_INFORMATION)));
  if (EFI_ERROR (Status)) {
    //
    // Start with minimum memory
    //
    for (Index = 0; Index < IndexNumber; Index++) {
      DEBUG((EFI_D_ERROR, "Index[%d].Type = %d .NumberOfPages=0x%x\n", Index,mDefaultMemoryTypeInformation[Index].Type,mDefaultMemoryTypeInformation[Index].NumberOfPages));
      *MemorySize += mDefaultMemoryTypeInformation[Index].NumberOfPages * EFI_PAGE_SIZE;
    }
    DEBUG((EFI_D_ERROR, "No memory type,  Total platform memory:%dKB \n", DivU64x32(*MemorySize,1024)));
    //
    // Build the GUID'd HOB for DXE
    //
    HobPointer = BuildGuidDataHob (
                  &gEfiMemoryTypeInformationGuid,
                  mDefaultMemoryTypeInformation,
                  sizeof (mDefaultMemoryTypeInformation)
                  );
  } else {
    //
    // Start with at least 0x200 pages of memory for the DXE Core and the DXE Stack
    //
    for (Index = 0; Index < IndexNumber; Index++) {
      DEBUG((EFI_D_ERROR, "Index[%d].Type = %d .NumberOfPages=0x%x\n", Index,MemoryData[Index].Type,MemoryData[Index].NumberOfPages));
      *MemorySize += MemoryData[Index].NumberOfPages * EFI_PAGE_SIZE;

    }
    DEBUG((EFI_D_ERROR, "has memory type,  Total platform memory:%dKB \n", DivU64x32(*MemorySize,1024)));
    //
    // Build the GUID'd HOB for DXE
    //
    HobPointer = BuildGuidDataHob (
                  &gEfiMemoryTypeInformationGuid,
                  MemoryData,
                  DataSize
                  );
  }

  ASSERT (HobPointer);

  DEBUG((EFI_D_ERROR, " MemorySize: 0x%x\n", (*MemorySize)));

  return EFI_SUCCESS;
}

/**

  This function checks the memory range in PEI.

  @param PeiServices     Pointer to PEI Services.
  @param This            Pei memory test PPI pointer.
  @param BeginAddress    Beginning of the memory address to be checked.
  @param MemoryLength    Bytes of memory range to be checked.
  @param Operation       Type of memory check operation to be performed.
  ErrorAddress    Return the address of the error memory address.

  @retval EFI_SUCCESS         The operation completed successfully.
  @retval EFI_DEVICE_ERROR    Memory test failed. It's not safe to use this range of memory.

**/
EFI_STATUS
EFIAPI
BaseMemoryTest (
  IN  EFI_PEI_SERVICES                   **PeiServices,
  IN  PEI_BASE_MEMORY_TEST_PPI           *This,
  IN  EFI_PHYSICAL_ADDRESS               BeginAddress,
  IN  UINT64                             MemoryLength,
  IN  PEI_MEMORY_TEST_OP                 Operation,
  OUT EFI_PHYSICAL_ADDRESS               *ErrorAddress
  )
{
  UINT32                TestPattern;
  UINT32                TestMask;
  UINT32                SpanSize;
  EFI_PHYSICAL_ADDRESS  TempAddress;

  TestPattern = MEMORY_TEST_PATTERN;
  TestMask    = 0;
  SpanSize    = 0;

  //
  // Make sure we don't try and test anything above the max physical address range
  //
  ASSERT (BeginAddress + MemoryLength < EFI_MAX_ADDRESS);

  switch (Operation) {
  case Extensive:
    SpanSize = 0x4;
    break;

  case Sparse:
  case Quick:
    SpanSize = MEMORY_TEST_COVER_SPAN;
    break;

  case Ignore:
    goto Done;
    break;
  }
  //
  // Write the test pattern into memory range
  //
  TempAddress = BeginAddress;
  while (TempAddress < BeginAddress + MemoryLength) {
    (*(UINT32 *) (UINTN) TempAddress) = TestPattern;
    TempAddress += SpanSize;
  }
  //
  // Read pattern from memory and compare it
  //
  TempAddress = BeginAddress;
  while (TempAddress < BeginAddress + MemoryLength) {
    if ((*(UINT32 *) (UINTN) TempAddress) != TestPattern) {
      *ErrorAddress = TempAddress;
      return EFI_DEVICE_ERROR;
    }

    TempAddress += SpanSize;
  }

Done:

  return EFI_SUCCESS;
}


VOID
PlatformMiscTasks(
  IN EFI_PEI_SERVICES           **PeiServices
  )
/**
  This function will be called when MRC is done.

**/
{
  EFI_STATUS          Status;
  EFI_BOOT_MODE       BootMode;
  EFI_HOB_GUID_TYPE      *GuidHob;

  //
  // Determine boot mode
  //
  Status = (*PeiServices)->GetBootMode(
    PeiServices,
    &BootMode
    );
  ASSERT_EFI_ERROR(Status);

  //
  // Build FSP Non-Volatile Storage Hob
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    GuidHob = GetFirstGuidHob(&gEfiMemoryConfigDataGuid);
    if (GuidHob != NULL) {
      //
      // Include the EFI_HOB_GENERIC_HEADER header in HOB.
      //
      BuildGuidDataHob(&gFspNonVolatileStorageHobGuid, (void *)((UINTN)GET_GUID_HOB_DATA(GuidHob) - sizeof (EFI_HOB_GUID_TYPE)), GET_GUID_HOB_DATA_SIZE(GuidHob) + sizeof (EFI_HOB_GUID_TYPE));
    }
  }
}

/**
This function determines if MTRRs have been programmed.
@retval  TRUE if MTRRs are programmed.
@retval  FALSE if MTRRs are not programmed.
**/
BOOLEAN
CheckIfMtrrsProgrammed()
{
  static UINT32 FixedMtrrs[] = {
    EFI_IA32_MTRR_FIX64K_00000,
    EFI_IA32_MTRR_FIX16K_80000,
    EFI_IA32_MTRR_FIX16K_A0000,
    EFI_IA32_MTRR_FIX4K_C0000,
    EFI_IA32_MTRR_FIX4K_C8000,
    EFI_IA32_MTRR_FIX4K_D0000,
    EFI_IA32_MTRR_FIX4K_D8000,
    EFI_IA32_MTRR_FIX4K_E0000,
    EFI_IA32_MTRR_FIX4K_E8000,
    EFI_IA32_MTRR_FIX4K_F0000,
    EFI_IA32_MTRR_FIX4K_F8000
  };

  UINT32  i;
  UINT32  EndIndex;
  UINT32  Msr;

  for (i = 0; i < sizeof(FixedMtrrs) / sizeof(UINT32); ++i) {
    if (AsmReadMsr64(FixedMtrrs[i]) != 0) return TRUE;
  }

  EndIndex = (UINT8)AsmReadMsr64(EFI_IA32_MTRR_CAP) * 2;
  for (i = 0; i < EndIndex; ++i) {
    Msr = EFI_CACHE_VARIABLE_MTRR_BASE + i;
    if (AsmReadMsr64(Msr) != 0) return TRUE;
  }

  return FALSE;
}

/**
This function prints MTRRs.
**/
VOID
PrintMtrrs(
VOID
)
{
  DEBUG_CODE_BEGIN();

  typedef struct {
    char    *Desc;
    UINT32  Msr;
  } MTRR_INFO;

  UINT32  i;
  UINT32  EndIndex;
  UINT32  Msr;

  static MTRR_INFO MtrrInfo[] = {
    { "MSR_IA32_MTRR_FIX64K_00000", EFI_IA32_MTRR_FIX64K_00000 },
    { "MSR_IA32_MTRR_FIX16K_80000", EFI_IA32_MTRR_FIX16K_80000 },
    { "MSR_IA32_MTRR_FIX16K_A0000", EFI_IA32_MTRR_FIX16K_A0000 },
    { "MSR_IA32_MTRR_FIX4K_C0000 ", EFI_IA32_MTRR_FIX4K_C0000 },
    { "MSR_IA32_MTRR_FIX4K_C8000 ", EFI_IA32_MTRR_FIX4K_C8000 },
    { "MSR_IA32_MTRR_FIX4K_D0000 ", EFI_IA32_MTRR_FIX4K_D0000 },
    { "MSR_IA32_MTRR_FIX4K_D8000 ", EFI_IA32_MTRR_FIX4K_D8000 },
    { "MSR_IA32_MTRR_FIX4K_E0000 ", EFI_IA32_MTRR_FIX4K_E0000 },
    { "MSR_IA32_MTRR_FIX4K_E8000 ", EFI_IA32_MTRR_FIX4K_E8000 },
    { "MSR_IA32_MTRR_FIX4K_F0000 ", EFI_IA32_MTRR_FIX4K_F0000 },
    { "MSR_IA32_MTRR_FIX4K_F8000 ", EFI_IA32_MTRR_FIX4K_F8000 }
  };

  for (i = 0; i < sizeof(MtrrInfo) / sizeof(MTRR_INFO); ++i) {
    DEBUG((DEBUG_INFO | DEBUG_INIT, "%a Msr %x = %016llx\n", MtrrInfo[i].Desc, MtrrInfo[i].Msr, AsmReadMsr64(MtrrInfo[i].Msr)));
  }

  EndIndex = (UINT8)AsmReadMsr64(EFI_IA32_MTRR_CAP);
  for (i = 0; i < EndIndex; ++i) {
    Msr = EFI_CACHE_VARIABLE_MTRR_BASE + 2 * i;
    DEBUG((DEBUG_INFO | DEBUG_INIT, "MSR_IA32_MTRR_PHYSBASE%d Msr %x = %016llx\n", i, Msr, AsmReadMsr64(Msr)));
    DEBUG((DEBUG_INFO | DEBUG_INIT, "MSR_IA32_MTRR_PHYSMASK%d Msr %x = %016llx\n", i, Msr + 1, AsmReadMsr64(Msr + 1)));
  }
  DEBUG_CODE_END();
}


/**
This function programs Cache Attributes.
**/
VOID ProgramCacheAttributes(
  VOID
  )
{
  UINT64      LowMemoryLength;
  UINT64      HighMemoryLength;
  UINT64      MaxLowMemoryLength;
  FSP_GLOBAL_DATA *FspData;

  //
  // Get system memory from HOB
  //
  FspGetSystemMemorySize (&LowMemoryLength, &HighMemoryLength);

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "ProgramCacheAttributes()\n")); // dgucks
  DEBUG ((DEBUG_INFO | DEBUG_INIT, "[FSP] Memory Length (Below 4GB) = %lx.\n", LowMemoryLength)); // dgucks
  DEBUG ((DEBUG_INFO | DEBUG_INIT, "[FSP] Memory Length (Above 4GB) = %lx.\n", HighMemoryLength)); // dgucks
  //
  // Set fixed MTRR values
  //
  SetCacheAttributes (
        0x00000,
        0xA0000,
        EFI_CACHE_WRITEBACK
        );

  SetCacheAttributes (
        0xA0000,
        0x20000,
        EFI_CACHE_UNCACHEABLE
        );

  SetCacheAttributes (
        0xC0000,
        0x40000,
        EFI_CACHE_WRITEBACK
        );

  //
  // Set the code region as cachable for performance
  //
  FspData  = GetFspGlobalDataPointer ();
  if (FspData->PlatformData.CodeRegionSize > 0  && FspData->PlatformData.CodeRegionBase > 0) {
    SetCacheAttributes (
        FspData->PlatformData.CodeRegionBase,
        FspData->PlatformData.CodeRegionSize,
        EFI_CACHE_WRITEPROTECTED
        );
  }

  //
  // Set the largest range as WB and then patch smaller ranges with UC
  // It can reduce the MTRR register usage
  //
  MaxLowMemoryLength = GetPowerOfTwo64 (LowMemoryLength);
  if (LowMemoryLength != MaxLowMemoryLength) {
    MaxLowMemoryLength = LShiftU64 (MaxLowMemoryLength, 1);
  }

  SetCacheAttributes (
        0,
        MaxLowMemoryLength,
        EFI_CACHE_WRITEBACK
        );

  if (LowMemoryLength != MaxLowMemoryLength) {
    SetCacheAttributes (
        LowMemoryLength,
        MaxLowMemoryLength - LowMemoryLength,
        EFI_CACHE_UNCACHEABLE
        );
  }

  if (HighMemoryLength) {
    SetCacheAttributes (
        0x100000000,
        HighMemoryLength,
        EFI_CACHE_WRITEBACK
        );
  }
}

/**
  This function reports and installs new FV

  @retval     EFI_SUCCESS          The function completes successfully
**/
EFI_STATUS
ReportAndInstallNewFv (
  VOID
  )
{
  FSP_INFO_HEADER                *FspInfoHeader;
  EFI_FIRMWARE_VOLUME_HEADER     *FvHeader;
  UINT8                          *CurPtr;
  UINT8                          *EndPtr;
  FSP_GLOBAL_DATA                *FspData;

  FspData = GetFspGlobalDataPointer ();
  if (FspData->FspMode == FSP_IN_API_MODE) {
  FspInfoHeader = GetFspInfoHeaderFromApiContext();
  if (FspInfoHeader->Signature != FSP_INFO_HEADER_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "The signature of FspInfoHeader getting from API context is invalid at 0x%08X.\n", (UINT32)FspInfoHeader));
    FspInfoHeader = GetFspInfoHeader();
  }

  CurPtr = (UINT8 *)FspInfoHeader->ImageBase;
  EndPtr = CurPtr + FspInfoHeader->ImageSize - 1;

  while (CurPtr < EndPtr) {

    FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *)CurPtr;
    if (FvHeader->Signature != EFI_FVH_SIGNATURE) {
      break;
    }
    DEBUG ((DEBUG_ERROR, "Installing FV at 0x%08X-0x%X.\n", (UINT32)FvHeader, (UINT32) FvHeader->FvLength));
    PeiServicesInstallFvInfoPpi (
      NULL,
      (VOID *)FvHeader,
      (UINTN) FvHeader->FvLength,
      NULL,
      NULL
      );
    CurPtr += FvHeader->FvLength;
  }
  }
  return EFI_SUCCESS;
}

VOID
StoreUsefulPcdToHob()
{
  EFI_PHYSICAL_ADDRESS             HobBuffer[2];
  EFI_PHYSICAL_ADDRESS             Buffer;

  Buffer = PcdGet64 (PcdSyshostMemoryAddress);
  DEBUG((EFI_D_ERROR, "SaveTomem Set PcdSyshostMemoryAddress %x\n", (UINTN)Buffer));
  HobBuffer[0] = Buffer;
  Buffer = PcdGet64 (PcdMemMapHostMemoryAddress);
  DEBUG((EFI_D_ERROR, "SaveTomem Set PcdMemMapHostMemoryAddress %x\n", (UINTN)Buffer));
  HobBuffer[1] = Buffer;
  BuildGuidDataHob(&gSaveHostToMemoryGuid, &HobBuffer, sizeof(HobBuffer));
}

/**
  This function will be called when MRC is done.

  @param  PeiServices General purpose services available to every PEIM.

  @param  NotifyDescriptor Information about the notify event..

  @param  Ppi The notify context.

  @retval EFI_SUCCESS If the function completed successfully.
**/
EFI_STATUS
EFIAPI
MemoryDiscoveredPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  FSPM_UPD        *FspmUpdOriginal, *FspmUpdCopy;
  VOID            **HobListPtr;
  EFI_STATUS      Status;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "Memory Discovered Notify invoked ...\n"));
  //
  // MISC Tasks
  //
  PlatformMiscTasks(PeiServices);

  //
  // Save a copy of FSP MemoryInit UPD data in permanent memory for use later
  //
  FspmUpdOriginal = GetFspMemoryInitUpdDataPointer ();

  FspmUpdCopy = NULL;
  Status = PeiServicesAllocatePool (sizeof(FSPM_UPD), &FspmUpdCopy);
  ASSERT_EFI_ERROR (Status);
  ASSERT (FspmUpdCopy != NULL);

  if (FspmUpdOriginal != NULL) {
    CopyMem (FspmUpdCopy, FspmUpdOriginal, sizeof(FSPM_UPD));
  } else {
    ZeroMem(FspmUpdCopy, sizeof(FSPM_UPD));
  }
  SetFspMemoryInitUpdDataPointer (FspmUpdCopy);

  DEBUG ((DEBUG_INFO, "Migrated MemoryInit UPD from 0x%p to 0x%p\n",
                      FspmUpdOriginal,
                      FspmUpdCopy));

  StoreUsefulPcdToHob();

  Status = PeiServicesInstallPpi(&mPeiCpuMpPpiDesc);
  ASSERT_EFI_ERROR (Status);


  HobListPtr = (VOID **)GetFspApiParameter2();
  if (HobListPtr != NULL) {
    *HobListPtr = (VOID *)GetHobList();
  }

  //
  // This is the end of the FspMemoryInit API
  // Give control back to the boot loader
  //
  SetFspApiReturnStatus (EFI_SUCCESS);
  FspMemoryInitDone (HobListPtr);

  if (GetFspApiCallingIndex() == TempRamExitApiIndex) {
    // Disable CAR and give control back to the boot loader
    ResetCacheAttributes ();
    //
    // Program MTRR values.
    //
    ProgramCacheAttributes ();
    DEBUG ((DEBUG_INFO | DEBUG_INIT, "MTRR programming:\n"));
    PrintMtrrs ();
    FspTempRamExitDone ();
  }

  //MTRRs should be programmed in FSP flow 1.0. Also, MTRRs are not already programmed in FSP flow 1.1.
  if (!CheckIfMtrrsProgrammed()) {
    ProgramCacheAttributes();
    DEBUG((DEBUG_INFO | DEBUG_INIT, "MTRR programming:\n"));
    PrintMtrrs();
  }

  // Got here via stack switch in SiliconInit API entry point
  ReportAndInstallNewFv ();

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "Memory Discovered Notify completed ...\n"));
  return EFI_SUCCESS;
}

/**
  This function provides a blocking stall for reset at least the given number of microseconds
  stipulated in the final argument.

  @param  PeiServices General purpose services available to every PEIM.

  @param  this Pointer to the local data for the interface.

  @param  Microseconds number of microseconds for which to stall.

  @retval EFI_SUCCESS the function provided at least the required stall.
**/
EFI_STATUS
EFIAPI
Stall (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN CONST EFI_PEI_STALL_PPI  *This,
  IN UINTN                    Microseconds
  )
{
  MicroSecondDelay (Microseconds);
  return EFI_SUCCESS;
}


/**
  This function reset the entire platform, including all processor and devices, and
  reboots the system.

  @param  PeiServices General purpose services available to every PEIM.

  @retval EFI_SUCCESS if it completed successfully.
**/
EFI_STATUS
EFIAPI
ResetSystem (
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  IoWrite8 (0xCF9, 0x06);
  CpuDeadLoop ();

  return EFI_SUCCESS;
}

/**

  Do platform initialization

  @param  FfsHeader    Not used.
  @param  PeiServices  General purpose services available to every PEIM.

  @return EFI_SUCCESS  Memory initialization completed successfully.
          All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
PeimPlatformInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS         Status;
//  PCH_POLICY_PPI     *PchPolicyPpi;
  UPI_POLICY_PPI     *UpiPolicyPpi;
  RAS_RC_POLICY_PPI  *RasRcPolicyPpi;
  MEMORY_POLICY_PPI  *MemoryPolicyPpi;
  SYS_SETUP          *SysSetup;
  FSPM_UPD           *FspmUpdDataPtr;
  IIO_SI_POLICY_PPI  *IioSiPolicyPpi;

//#if ME_SUPPORT_FLAG
//  ME_POLICY_PPI      *MePolicyPpi;
//#endif // ME_SUPPORT_FLAG
//#if AMT_SUPPORT
//  AMT_POLICY_PPI     *AmtPolicyPpi;
//#endif // AMT_SUPPORT
//#if ME_SPS_SUPPORT
//  SPS_POLICY_PPI     *SpsPolicyPpi;
//#endif // ME_SPS_SUPPORT
#if IE_SUPPORT
  PEI_IE_PLATFORM_POLICY_PPI  *PeiIePlatformPolicyPpi;
#endif // IE_SUPPORT


  SetPhaseStatusCode (FSP_STATUS_CODE_MEMORY_INIT);
  // REPORT_STATUS_CODE (EFI_PROGRESS_CODE, FSP_STATUS_CODE_MEMORY_INIT | FSP_STATUS_CODE_COMMON_CODE | FSP_STATUS_CODE_API_ENTRY);
  DEBUG ((EFI_D_INFO, "FspInitPreMemEntryPoint() - Start\n"));

  if (GetFspGlobalDataPointer()->FspMode == FSP_IN_API_MODE) {
    Status = (*PeiServices)->RegisterForShadow(FileHandle);

    if (Status == EFI_ALREADY_STARTED) {
      //
      // Image is running from DRAM now
      //
      Status = PeiServicesInstallPpi (&mPpiBootMode);
      ASSERT_EFI_ERROR (Status);

      //
      // Now that all of the pre-permanent memory activities have been taken care
      // of, post a call-back for the permanent-memory resident services, such as
      // HOB construction. PEI Core will switch stack after this PEIM exit.  After
      // that the MTRR can be set.
      //
      Status = PeiServicesNotifyPpi (&mMemoryDiscoveredNotifyList);
      ASSERT_EFI_ERROR (Status);
    } else {
      //
      // Image is not running from DRAM yet!
      //
      if (Status == EFI_NOT_FOUND) {
        ASSERT_EFI_ERROR (Status);
      }

      //
      // Install serivce PPIs
      //
      Status = (**PeiServices).InstallPpi (PeiServices, mPeiPlatformPpis);
      ASSERT_EFI_ERROR (Status);

      //
      // Update Boot Mode
      //
      Status = UpdateBootMode (PeiServices);
      ASSERT_EFI_ERROR (Status);

      FspmUpdDataPtr = NULL;

      //
      // Get the UPD pointer.
      //
      FspmUpdDataPtr = (FSPM_UPD *) GetFspMemoryInitUpdDataPointer ();
      SetFspSiliconInitUpdDataPointer ((void *)NULL);

//      //
//      // Install policy Ppis.
//      //
//      //
//      // Create Pch policy Ppi.
//      //
//      Status = PchCreatePolicyDefaults  (&PchPolicyPpi);
//      ASSERT_EFI_ERROR (Status);
//      if (EFI_ERROR (Status)) {
//        return EFI_OUT_OF_RESOURCES;
//      }
//      FspUpdatePeiPchPolicyPreMem (PchPolicyPpi, FspmUpdDataPtr);
//      PchInstallPolicyPpi (PchPolicyPpi);
//      DEBUG ((EFI_D_INFO, "FspInitPreMemEntryPoint() - Pch Policy Ppi Installed!\n"));

      //
      // Create UPI policy Ppi.
      //
      UpiPolicyPpi = (UPI_POLICY_PPI *) AllocateZeroPool (sizeof (UPI_POLICY_PPI));
      ASSERT (UpiPolicyPpi != NULL);
      if (UpiPolicyPpi == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      UpiCreatePolicyDefaults (UpiPolicyPpi);
      FspUpdatePeiUpiPolicyPreMem (UpiPolicyPpi, FspmUpdDataPtr);
      UpiInstallPolicyPpi (UpiPolicyPpi);
      DEBUG ((EFI_D_INFO, "FspInitPreMemEntryPoint() - UPI Policy Ppi Installed!\n"));

      //
      // Create Ras policy Ppi.
      //
      RasRcPolicyPpi = (RAS_RC_POLICY_PPI *) AllocatePool (sizeof (RAS_RC_POLICY_PPI));
      ASSERT (RasRcPolicyPpi != NULL);
      if (RasRcPolicyPpi == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }

      SetRasRcPolicyPpiDefaults (RasRcPolicyPpi);
      FspUpdatePeiRasRcPolicyPreMem (RasRcPolicyPpi, FspmUpdDataPtr);
      InstallRasRcPolicyPpi (RasRcPolicyPpi);
      DEBUG ((EFI_D_INFO, "FspInitPreMemEntryPoint() - Ras Rc Policy Ppi Installed!\n"));

      //
      // Create Memory policy Ppi.
      //
      MemoryPolicyPpi = (MEMORY_POLICY_PPI *) AllocateZeroPool (sizeof (MEMORY_POLICY_PPI));
      ASSERT (MemoryPolicyPpi != NULL);
      if (MemoryPolicyPpi == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      SysSetup = (SYS_SETUP *) RcAllocatePool (sizeof (SYS_SETUP));
      ASSERT (SysSetup != NULL);
      if (SysSetup == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      MemoryPolicyPpi->SysSetup = SysSetup;

      SetMemoryPolicyDefaults (MemoryPolicyPpi);
      FspUpdatePeiMemoryPolicyPreMem (MemoryPolicyPpi, FspmUpdDataPtr);
      InstallMemoryPolicyPpi (MemoryPolicyPpi);
      DEBUG ((EFI_D_INFO, "FspInitPreMemEntryPoint() - Memory Policy Ppi Installed!\n"));

      //
      // Create IIO policy Ppi.
      //
      IioSiPolicyPpi = (IIO_SI_POLICY_PPI*)AllocateZeroPool (sizeof (IIO_SI_POLICY_PPI));
      ASSERT (IioSiPolicyPpi != NULL);
      if (IioSiPolicyPpi == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      CreateDefaultIioSiPolicy ();
      //
      // Note: IIO install the policy PPI in FspUpdatePeiIioPolicyPreMem function.
      //
      FspUpdatePeiIioPolicyPreMem (IioSiPolicyPpi, FspmUpdDataPtr);
      DEBUG ((EFI_D_INFO, "FspInitPreMemEntryPoint() - IIO Policy Ppi Installed!\n"));

      //
      // Update CPU policy.
      //
      FspUpdatePeiCpuPolicyPreMem (FspmUpdDataPtr);

//#if ME_SUPPORT_FLAG
//      //
//      // Create ME related policy Ppi.
//      //
//      Status = SrvMeCreatePolicyDefaults (&MePolicyPpi);
//      ASSERT_EFI_ERROR (Status);
//      if (EFI_ERROR (Status)) {
//        return EFI_OUT_OF_RESOURCES;
//      }
//      // @todo: update policy with UPD data.
//      MePolicyPpi->MeConfig.SendDidMsg = 0;
//      SrvMeInstallPolicyPpi (MePolicyPpi);
//      DEBUG ((DEBUG_INFO, "FspInitPreMemEntryPoint() - Me Policy Ppi Installed!\n"));
//#endif // ME_ENABLE
//
//#if AMT_SUPPORT
//      Status = SrvAmtCreatePolicyDefaults (&AmtPolicyPpi);
//      ASSERT_EFI_ERROR (Status);
//      if (EFI_ERROR(Status)) {
//        return EFI_OUT_OF_RESOURCES;
//      }
//      // @todo: update policy with UPD data.
//      SrvAmtInstallPolicyPpi (AmtPolicyPpi);
//      DEBUG ((DEBUG_INFO, "FspInitPreMemEntryPoint() - AMT Policy Ppi Installed\n"));
//#endif // AMT_SUPPORT
//
//#if ME_SPS_SUPPORT
//      Status = SpsInstallPolicyPpi (PeiServices);
//      if (!EFI_ERROR (Status)) {
//        SpsPolicyPpi = GetSpsPolicyPpi ();
//        if (SpsPolicyPpi != NULL) {
//          // @todo: update policy with UPD data.
//        }
//      }
//      ASSERT_EFI_ERROR (Status);
//#endif // ME_SPS_SUPPORT
//
//#if IE_SUPPORT
//      //
//      // Create IE related policy Ppi.
//      //
//      Status = IeCreatePolicyDefaults (&PeiIePlatformPolicyPpi);
//      ASSERT_EFI_ERROR (Status);
//      if (EFI_ERROR (Status)) {
//        return EFI_OUT_OF_RESOURCES;
//      }
//      // @todo: update policy with UPD data.
//      IeInstallPolicyPpi (PeiIePlatformPolicyPpi);
//#endif // IE_ENABLE

    }
  }

  DEBUG ((EFI_D_INFO, "FspInitPreMemEntryPoint() - End\n"));
  return EFI_SUCCESS;
}
