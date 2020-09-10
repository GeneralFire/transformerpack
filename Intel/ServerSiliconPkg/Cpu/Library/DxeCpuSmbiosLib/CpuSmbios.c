/** @file
  Code to log processor and cache subclass data to smbios record.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "Processor.h"
#include "Cache.h"
#include <Protocol/MpService.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/CpuSmbiosLib.h>

EFI_SMBIOS_PROTOCOL     *mSmbios;
EFI_HII_HANDLE          mStringHandle;

/**
  Get number of cores and threads in the specified package.

  @param[in]  Mp                 MP_SERVICES protocol instance.
  @param[in]  ProcessorIndex     Processor index
  @param[in]  NumberOfProcessors Total number of threads.
  @param[out] DieCount           Return number of dies in the package that ProcessorIndex resides.
  @param[out] CoreCount          Return number of cores in the package that ProcessorIndex resides.
  @param[out] ThreadCount        Return number of threads in the package that ProcessorIndex resides.

  @retval TRUE  Number of cores and threads are returned.
  @retval FALSE Number of cores and threads are not returned.
**/
BOOLEAN
GetDieCoreThreadCountOfPackage (
  EFI_MP_SERVICES_PROTOCOL  *Mp,
  UINTN                     ProcessorIndex,
  UINTN                     NumberOfProcessors,
  UINTN                     *DieCount,
  UINTN                     *CoreCount,
  UINTN                     *ThreadCount
  )
{
  EFI_STATUS                 Status;
  EFI_PROCESSOR_INFORMATION  Info;
  UINTN                      Index;
  UINTN                      ThreadCountOfPackage;
  UINTN                      ThreadCountOfDie;
  UINTN                      ThreadCountOfCore;
  EFI_CPU_PHYSICAL_LOCATION2 My;

  ASSERT (DieCount != NULL);
  ASSERT (CoreCount != NULL);
  ASSERT (ThreadCount != NULL);
  if ((DieCount == NULL) || (CoreCount == NULL) || (ThreadCount == NULL)) {
    return FALSE;
  }

  //
  // Get the package/core index of the specified processor.
  //
  Status = Mp->GetProcessorInfo (Mp, ProcessorIndex | CPU_V2_EXTENDED_TOPOLOGY, &Info);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  CopyMem (&My, &Info.ExtendedInformation.Location2, sizeof (My));

  //
  // Get the thread count of the specified package and the specified core.
  //
  ThreadCountOfPackage = 0;
  ThreadCountOfDie = 0;
  ThreadCountOfCore = 0;
  for (Index = 0; Index < NumberOfProcessors; Index++) {
    Status = Mp->GetProcessorInfo (Mp, Index | CPU_V2_EXTENDED_TOPOLOGY, &Info);
    if (EFI_ERROR (Status)) {
      return FALSE;
    }
    if (Info.ExtendedInformation.Location2.Package == My.Package) {
      ThreadCountOfPackage++;
      if (Info.ExtendedInformation.Location2.Module == My.Module &&
          Info.ExtendedInformation.Location2.Tile   == My.Tile &&
          Info.ExtendedInformation.Location2.Die    == My.Die) {
        ThreadCountOfDie++;
        if (Info.ExtendedInformation.Location2.Core == My.Core) {
          ThreadCountOfCore++;
        }
      }
    }
  }
  DEBUG ((
    DEBUG_ERROR, "CPU[%03d]: ThreadCountOfPackage=%d ThreadCountOfDie=%d ThreadCountOfCore=%d\n",
    ProcessorIndex, ThreadCountOfPackage, ThreadCountOfDie, ThreadCountOfCore
    ));
  if (ThreadCountOfPackage == 0 || ThreadCountOfDie == 0 || ThreadCountOfCore == 0) {
    return FALSE;
  }

  *DieCount = ThreadCountOfPackage / ThreadCountOfDie;
  *CoreCount = ThreadCountOfPackage / ThreadCountOfCore;
  *ThreadCount = ThreadCountOfPackage;
  return TRUE;
}

/**
  Add SMBIOS Processor Type and Cache Type tables for the CPU.
**/
VOID
AddCpuSmbiosTables (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_SMBIOS_HANDLE         L1CacheHandle;
  EFI_SMBIOS_HANDLE         L2CacheHandle;
  EFI_SMBIOS_HANDLE         L3CacheHandle;
  UINTN                     PackageIndex;
  UINTN                     ProcessorIndex;
  UINTN                     *ProcessorInPackage;
  UINTN                     DieCount;
  UINTN                     CoreCount;
  UINTN                     ThreadCount;
  BOOLEAN                   Success;
  UINTN                     *Packages;
  UINT32                    PopulatedPackageCount;
  EFI_MP_SERVICES_PROTOCOL  *Mp;
  UINTN                     NumberOfProcessors;
  UINTN                     NumberOfEnabledProcessors;
  EFI_PROCESSOR_INFORMATION Info;

  L1CacheHandle = 0xFFFF;
  L2CacheHandle = 0xFFFF;
  L3CacheHandle = 0xFFFF;

  //
  // Initialize the mSmbios to contain the SMBIOS protocol,
  //
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &mSmbios);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID**)&Mp);
  ASSERT_EFI_ERROR (Status);

  Status = Mp->GetNumberOfProcessors (Mp, &NumberOfProcessors, &NumberOfEnabledProcessors);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Initialize strings to HII database
  //
  mStringHandle = HiiAddPackages (
                    &gEfiCallerIdGuid,
                    NULL,
                    DxeCpuSmbiosLibStrings,
                    NULL
                    );
  if (mStringHandle == NULL) {
    return;
  }

  ProcessorInPackage = AllocateZeroPool (PcdGet32 (PcdPlatformCpuSocketCount) * sizeof (UINTN));
  if (ProcessorInPackage == NULL) {
    DEBUG ((DEBUG_ERROR, "AllocateZeroPool Returned NULL pointer\n"));
    ASSERT (ProcessorInPackage != NULL);

    HiiRemovePackages (mStringHandle);
    return;
  }

  Packages = AllocateZeroPool (PcdGet32 (PcdPlatformCpuSocketCount) * sizeof (UINTN));
  if (Packages == NULL) {
    DEBUG ((DEBUG_ERROR, "AllocateZeroPool Returned NULL pointer\n"));
    ASSERT (Packages != NULL);

    FreePool (ProcessorInPackage);
    HiiRemovePackages (mStringHandle);
    return;
  }

  //
  // Detect populated sockets (comparing the processors' PackageNumber) and record their ProcessorNumber.
  // For example:
  //   ProcessorNumber: 0 1 2 3 (PackageNumber 0) 4 5 6 7 (PackageNumber 1)
  //   And then, populated socket count will be 2 and record ProcessorNumber 0 (or 1, 2, 3) for Socket 0, ProcessorNumber 4 (or 5, 6, 7) for Socket 1
  //
  for (ProcessorIndex = 0, PopulatedPackageCount = 0; ProcessorIndex < NumberOfProcessors; ProcessorIndex++) {
    Status = Mp->GetProcessorInfo (Mp, ProcessorIndex | CPU_V2_EXTENDED_TOPOLOGY, &Info);
    if (EFI_ERROR (Status)) {
      FreePool (Packages);
      FreePool (ProcessorInPackage);
      HiiRemovePackages (mStringHandle);
      return;
    }

    for (PackageIndex = 0; PackageIndex < PopulatedPackageCount; PackageIndex++) {
      if (Info.ExtendedInformation.Location2.Package == Packages[PackageIndex]) {
        break;
      }
    }
    if (PackageIndex != PopulatedPackageCount) {
      continue;
    }

    //
    // Fatal error when:
    //  1. populated package count >= platform defined socket count or,
    //  2. package index >= platform defined socket count.
    //
    if ((PopulatedPackageCount >= PcdGet32 (PcdPlatformCpuSocketCount)) ||
        (Info.ExtendedInformation.Location2.Package >= PcdGet32 (PcdPlatformCpuSocketCount))) {
      FreePool (Packages);
      FreePool (ProcessorInPackage);
      HiiRemovePackages (mStringHandle);
      return;
    }
    //
    // Record the new package index and index of a processor (any) inside the package.
    //
    ProcessorInPackage[PopulatedPackageCount] = ProcessorIndex;
    Packages          [PopulatedPackageCount] = Info.ExtendedInformation.Location2.Package;
    PopulatedPackageCount++;
  }

  for (PackageIndex = 0; PackageIndex < PopulatedPackageCount; PackageIndex++) {
    DEBUG ((DEBUG_ERROR, "SMBIOS Package[%d] - Processor[%d]\n", Packages[PackageIndex], ProcessorInPackage[PackageIndex]));
  }

  //
  // Add SMBIOS tables for populated sockets.
  //
  for (PackageIndex = 0; PackageIndex < PopulatedPackageCount; PackageIndex++) {
    Success = GetDieCoreThreadCountOfPackage (Mp, ProcessorInPackage[PackageIndex], NumberOfProcessors, &DieCount, &CoreCount, &ThreadCount);
    if (Success) {
      AddSmbiosCacheTypeTable (ProcessorInPackage[PackageIndex], CoreCount, DieCount, &L1CacheHandle, &L2CacheHandle, &L3CacheHandle);
      AddSmbiosProcessorTypeTable (ProcessorInPackage[PackageIndex], CoreCount, ThreadCount, L1CacheHandle, L2CacheHandle, L3CacheHandle);
    }
  }
  FreePool (ProcessorInPackage);

  //
  // Add Type 4 SMBIOS Record for Socket Unpopulated.
  //
  for (PackageIndex = PopulatedPackageCount; PackageIndex < PcdGet32 (PcdPlatformCpuSocketCount); PackageIndex++) {
    AddUnpopulatedSmbiosProcessorTypeTable ();
  }

  SetCpuSmbiosSocketNames (Packages, PopulatedPackageCount);
  SetCpuSmbiosAssetTags (Packages, PopulatedPackageCount);
  FreePool (Packages);
}

