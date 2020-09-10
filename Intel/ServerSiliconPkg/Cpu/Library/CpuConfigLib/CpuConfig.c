/** @file
  CPU Configuration Library.

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

#include "CpuConfig.h"

CPU_CONFIG_CONTEXT_BUFFER      *mCpuConfigLibConfigContextBuffer;

/**
  Allocate any pages.

  @param  Size         Size of memory to allocate.

  @return Allocated address for output.

**/
VOID*
CpuConfigAllocateAnyPages (
  IN   UINTN   Size
  )
{
  UINTN                 Pages;
  EFI_PHYSICAL_ADDRESS  Address;
  EFI_STATUS            Status;
  VOID*                 Buffer;

  Pages = EFI_SIZE_TO_PAGES (Size);

  Status  = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData, Pages, &Address);
  ASSERT_EFI_ERROR (Status);

  Buffer = (VOID *) (UINTN) Address;
  ZeroMem (Buffer, Size);

  return Buffer;
}

/**
  Get the pointer to Smst.

  @retval The pointer to Smst if it is in SMM environment.
  @retval NULL if it is not in SMM environment.

**/
EFI_SMM_SYSTEM_TABLE2 *
CpuConfigGetSmst (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_SMM_BASE2_PROTOCOL    *SmmBase2;
  BOOLEAN                   InSmm;
  EFI_SMM_SYSTEM_TABLE2     *Smst;

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID **) &SmmBase2);
  if (EFI_ERROR (Status)) {
    return NULL;
  }
  Status = SmmBase2->InSmm (SmmBase2, &InSmm);
  ASSERT_EFI_ERROR (Status);
  if (!InSmm) {
    return NULL;
  }
  Status = SmmBase2->GetSmstLocation (SmmBase2, &Smst);
  ASSERT_EFI_ERROR (Status);
  if ((Smst == NULL) || (Smst->NumberOfCpus == 0)) {
    //
    // The Smst or Smst->NumberOfCpus is not initialized yet.
    // It is the case that the code is running in SMRAM,
    // but not in SMM mode (PiSmmCpuDxeSmm is not run yet).
    //
    return NULL;
  } else {
    return Smst;
  }
}

/**
  The constructor function of CPU Configuration Library.

  The constructor function caches the value of PCD entry PcdCpuConfigContextBuffer.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The constructor always returns EFI_SUCCESS.
  @return EFI_OUT_OF_RESOURCES  Could not allocate needed resources.

**/
EFI_STATUS
EFIAPI
CpuConfigLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_SMM_SYSTEM_TABLE2     *Smst;
  EFI_MP_SERVICES_PROTOCOL  *MpServices;
  UINTN                     NumberOfProcessors;
  UINTN                     NumberOfEnabledProcessors;
  UINTN                     Index;
  CPU_REGISTER_TABLE        *RegisterTable;
  CPU_REGISTER_TABLE        *PreSmmInitRegisterTable;
  UINTN                     EntryBufferSize;

  //
  // Get the pointer to CPU Configuration Context Buffer from PcdCpuConfigContextBuffer.
  //
  mCpuConfigLibConfigContextBuffer = (CPU_CONFIG_CONTEXT_BUFFER *) (UINTN) PcdGet64 (PcdCpuConfigContextBuffer);
  if (mCpuConfigLibConfigContextBuffer == NULL) {
    //
    // The buffer is not allocated yet.
    //
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof (CPU_CONFIG_CONTEXT_BUFFER),
                    (VOID **)&mCpuConfigLibConfigContextBuffer
                    );
    ASSERT_EFI_ERROR (Status);
    ASSERT (mCpuConfigLibConfigContextBuffer != NULL);
    if (mCpuConfigLibConfigContextBuffer == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    ZeroMem (mCpuConfigLibConfigContextBuffer, sizeof (CPU_CONFIG_CONTEXT_BUFFER));
    Status = PcdSet64S (PcdCpuConfigContextBuffer, (UINT64) (UINTN) mCpuConfigLibConfigContextBuffer);
    ASSERT_EFI_ERROR (Status);

    //
    // Initialize data for CPU configuration context buffer
    //

    Smst = CpuConfigGetSmst ();
    if (Smst != NULL) {
      //
      // It is in SMM environment, the main processor running in SMM environment
      // may be not the same processor with the BSP for DXE environment, so it
      // could not call DXE MpServices GetNumberOfProcessors, Smst->NumberOfCpus
      // could be used.
      //
      NumberOfProcessors = Smst->NumberOfCpus;
    } else {
      Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **) &MpServices);
      ASSERT_EFI_ERROR (Status);
      Status = MpServices->GetNumberOfProcessors (MpServices, &NumberOfProcessors, &NumberOfEnabledProcessors);
      ASSERT_EFI_ERROR (Status);
    }
    mCpuConfigLibConfigContextBuffer->NumberOfProcessors = NumberOfProcessors;

    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    sizeof (CPU_COLLECTED_DATA) * NumberOfProcessors,
                    (VOID **)&mCpuConfigLibConfigContextBuffer->CollectedDataBuffer
                    );
    ASSERT_EFI_ERROR (Status);
    ASSERT (mCpuConfigLibConfigContextBuffer->CollectedDataBuffer != NULL);
    ZeroMem (mCpuConfigLibConfigContextBuffer->CollectedDataBuffer, sizeof (CPU_COLLECTED_DATA) * NumberOfProcessors);

    EntryBufferSize = sizeof (CPU_REGISTER_TABLE_ENTRY) * PcdGet16 (PcdCpuConfigRegTblEntryMaxCount);

    RegisterTable = CpuConfigAllocateAnyPages (sizeof (CPU_REGISTER_TABLE) * NumberOfProcessors);
    mCpuConfigLibConfigContextBuffer->RegisterTable = RegisterTable;
    for (Index = 0; Index < NumberOfProcessors; Index++) {
      RegisterTable[Index].RegisterTableEntry = (EFI_PHYSICAL_ADDRESS) (UINTN) CpuConfigAllocateAnyPages (EntryBufferSize);
      RegisterTable[Index].AllocatedSize = (UINT32) EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (EntryBufferSize));
    }

    PreSmmInitRegisterTable = CpuConfigAllocateAnyPages (sizeof (CPU_REGISTER_TABLE) * NumberOfProcessors);
    mCpuConfigLibConfigContextBuffer->PreSmmInitRegisterTable = PreSmmInitRegisterTable;
    for (Index = 0; Index < NumberOfProcessors; Index++) {
      PreSmmInitRegisterTable[Index].RegisterTableEntry = (EFI_PHYSICAL_ADDRESS) (UINTN) CpuConfigAllocateAnyPages (EntryBufferSize);
      PreSmmInitRegisterTable[Index].AllocatedSize = (UINT32) EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (EntryBufferSize));
    }
  }
  return EFI_SUCCESS;
}

/**
  Get the number of CPUID leafs of various types.

  This function get the number of CPUID leafs of various types.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  CpuidType         The type of the cpu id.

  @return Maximal index of CPUID instruction for basic leafs.

**/
UINTN
EFIAPI
GetNumberOfCpuidLeafs (
  IN  UINTN               ProcessorNumber,
  IN  CPUID_TYPE          CpuidType
  )
{
  UINTN                NumberOfLeafs;
  CPU_CPUID_DATA       *CpuidData;

  ASSERT (mCpuConfigLibConfigContextBuffer != NULL);
  if (mCpuConfigLibConfigContextBuffer == NULL) {
    return 0;
  }

  CpuidData = &(mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuidData);

  switch (CpuidType) {
  case BasicCpuidLeaf:
    NumberOfLeafs = CpuidData->NumberOfBasicCpuidLeafs;
    break;
  case ExtendedCpuidLeaf:
    NumberOfLeafs = CpuidData->NumberOfExtendedCpuidLeafs;
    break;
  case CacheAndTlbCpuidLeafs:
    NumberOfLeafs = CpuidData->NumberOfCacheAndTlbCpuidLeafs;
    break;
  case DeterministicCacheParametersCpuidLeafs:
    NumberOfLeafs = CpuidData->NumberOfDeterministicCacheParametersCpuidLeafs;
    break;
  case ExtendedTopologyEnumerationCpuidLeafs:
    NumberOfLeafs = CpuidData->NumberOfExtendedTopologyEnumerationLeafs;
    break;
  default:
    NumberOfLeafs = 0;
    break;
  }

  return NumberOfLeafs;
}

/**
  Get the pointer to specified CPUID leaf.

  This function gets the pointer to specified CPUID leaf.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  Index             Index of the CPUID leaf.

  @return Pointer to specified CPUID leaf

**/
EFI_CPUID_REGISTER*
EFIAPI
GetProcessorCpuid (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               Index
)
{
  CPU_CPUID_DATA       *CpuidData;

  ASSERT (mCpuConfigLibConfigContextBuffer != NULL);
  if (mCpuConfigLibConfigContextBuffer == NULL) {
    return NULL;
  }

  CpuidData = &(mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuidData);

  //
  // If Index specifies basic CPUID leafs, then locate basic leaf to return.
  //
  if (Index < CpuidData->NumberOfBasicCpuidLeafs) {
    return (&CpuidData->CpuIdLeaf[Index]);
  }

  //
  // If Index specifies extended CPUID leafs, then locate extended leaf to return.
  //
  if (Index >= 0x80000000 && Index < 0x80000000 + CpuidData->NumberOfExtendedCpuidLeafs) {
    return (&CpuidData->CpuIdLeaf[Index - 0x80000000 + CpuidData->NumberOfBasicCpuidLeafs]);
  }

  //
  // If Index specifies invalid CPUID index, then return NULL.
  //
  return NULL;
}

/**
  Get the pointer to specified CPUID leaf of cache and TLB parameters.

  This function gets the pointer to specified CPUID leaf of cache and TLB parameters.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  Index             Index of the CPUID leaf.

  @return Pointer to specified CPUID leaf.

**/
EFI_CPUID_REGISTER*
EFIAPI
GetCacheAndTlbCpuidLeaf (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               Index
)
{
  UINTN                StartIndex;
  CPU_CPUID_DATA       *CpuidData;

  ASSERT (mCpuConfigLibConfigContextBuffer != NULL);
  if (mCpuConfigLibConfigContextBuffer == NULL) {
    return NULL;
  }

  CpuidData = &(mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuidData);

  //
  // The first leaf resides in basic leaf 2
  //
  if (Index == 0) {
    return (&CpuidData->CpuIdLeaf[2]);
  }

  //
  // If index is greater than zero, calculate the start index of deterministic cache CPUID leafs.
  //
  StartIndex = CpuidData->NumberOfBasicCpuidLeafs + CpuidData->NumberOfExtendedCpuidLeafs;

  return (&CpuidData->CpuIdLeaf[Index + StartIndex - 1]);
}

/**
  Get the pointer to specified CPUID leaf of deterministic cache parameters.

  This function gets the pointer to specified CPUID leaf of deterministic cache parameters.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  Index             Index of the CPUID leaf.

  @return Pointer to specified CPUID leaf.

**/
EFI_CPUID_REGISTER*
EFIAPI
GetDeterministicCacheParametersCpuidLeaf (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               Index
)
{
  UINTN                StartIndex;
  CPU_CPUID_DATA       *CpuidData;

  ASSERT (mCpuConfigLibConfigContextBuffer != NULL);
  if (mCpuConfigLibConfigContextBuffer == NULL) {
    return NULL;
  }

  CpuidData = &(mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuidData);

  //
  // Calculate the start index of deterministic cache CPUID leafs.
  //
  StartIndex =  CpuidData->NumberOfBasicCpuidLeafs;
  StartIndex += CpuidData->NumberOfExtendedCpuidLeafs;
  StartIndex += CpuidData->NumberOfCacheAndTlbCpuidLeafs - 1;

  return (&CpuidData->CpuIdLeaf[Index + StartIndex]);
}

/**
  Get the pointer to specified CPUID leaf of Extended Topology Enumeration.

  This function gets the pointer to specified CPUID leaf of Extended Topology Enumeration.

  @param  ProcessorNumber   Handle number of specified logical processor.
  @param  Index             Index of the CPUID leaf.

  @return Pointer to specified CPUID leaf.

**/
EFI_CPUID_REGISTER*
EFIAPI
GetExtendedTopologyEnumerationCpuidLeafs (
  IN  UINTN               ProcessorNumber,
  IN  UINTN               Index
  )
{
  UINTN                StartIndex;
  CPU_CPUID_DATA       *CpuidData;

  ASSERT (mCpuConfigLibConfigContextBuffer != NULL);
  if (mCpuConfigLibConfigContextBuffer == NULL) {
    return NULL;
  }

  CpuidData = &(mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuidData);

  //
  // Calculate the start index of Extended Topology Enumeration CPUID leafs.
  //
  StartIndex =  CpuidData->NumberOfBasicCpuidLeafs;
  StartIndex += CpuidData->NumberOfExtendedCpuidLeafs;
  StartIndex += CpuidData->NumberOfCacheAndTlbCpuidLeafs - 1;
  StartIndex += CpuidData->NumberOfDeterministicCacheParametersCpuidLeafs;

  return (&CpuidData->CpuIdLeaf[Index + StartIndex]);
}

/**
  Get the version information of specified logical processor.

  This function gets the version information of specified logical processor,
  including family ID, model ID, stepping ID and processor type.

  @param  ProcessorNumber   Handle number of specified logical processor
  @param  DisplayedFamily   Pointer to family ID for output
  @param  DisplayedModel    Pointer to model ID for output
  @param  SteppingId        Pointer to stepping ID for output
  @param  ProcessorType     Pointer to processor type for output

**/
VOID
EFIAPI
GetProcessorVersionInfo (
  IN  UINTN               ProcessorNumber,
  OUT UINT32              *DisplayedFamily OPTIONAL,
  OUT UINT32              *DisplayedModel  OPTIONAL,
  OUT UINT32              *SteppingId      OPTIONAL,
  OUT UINT32              *ProcessorType   OPTIONAL
  )
{
  EFI_CPUID_REGISTER      *VersionInfo;
  UINT32                  RegEax;
  UINT32                  FamilyId;
  UINT32                  ExtendedFamilyId;
  UINT32                  ExtendedModelId;

  //
  // Get CPUID(1).EAX
  //
  VersionInfo = GetProcessorCpuid (ProcessorNumber, CPUID_VERSION_INFO);
  ASSERT (VersionInfo != NULL);
  if (VersionInfo == NULL) {
    return; // Do not populate version data.
  }

  RegEax      = VersionInfo->RegEax;

  //
  // Processor Type is CPUID(1).EAX[12:13]
  //
  if (ProcessorType != NULL) {
    *ProcessorType = BitFieldRead32 (RegEax, 12, 13);
  }

  //
  // Stepping ID is CPUID(1).EAX[0:3]
  //
  if (SteppingId != NULL) {
    *SteppingId = BitFieldRead32 (RegEax, 0, 3);
  }

  //
  // The Extended Family ID needs to be examined only when the Family ID is 0FH.
  // If Family ID is 0FH, Displayed Family ID = Family ID + Extended Family ID.
  // Otherwise, Displayed Family ID is Family ID
  //
  FamilyId = BitFieldRead32 (RegEax, 8, 11);
  if (DisplayedFamily != NULL) {
    *DisplayedFamily = FamilyId;
    if (FamilyId == 0x0f) {
      ExtendedFamilyId = BitFieldRead32 (RegEax, 20, 27);
      *DisplayedFamily += ExtendedFamilyId;
    }
  }

  //
  // The Extended Model ID needs to be examined only when the Family ID is 06H or 0FH.
  // If Family ID is 06H or 0FH, Displayed Model ID = Model ID + (Extended Model ID << 4).
  // Otherwise, Displayed Model ID is Model ID.
  //
  if (DisplayedModel != NULL) {
    *DisplayedModel = BitFieldRead32 (RegEax, 4, 7);
    if (FamilyId == 0x06 || FamilyId == 0x0f) {
      ExtendedModelId = BitFieldRead32 (RegEax, 16, 19);
      *DisplayedModel += (ExtendedModelId << 4);
    }
  }
}

/**
  Get initial local APIC ID of specified logical processor

  This function gets initial local APIC ID of specified logical processor.

  @param  ProcessorNumber Handle number of specified logical processor

  @return Initial local APIC ID of specified logical processor

**/
UINT32
EFIAPI
GetInitialLocalApicId (
  UINTN    ProcessorNumber
  )
{
  CPU_COLLECTED_DATA          *CpuCollectedData;

  ASSERT (mCpuConfigLibConfigContextBuffer != NULL);
  if (mCpuConfigLibConfigContextBuffer == NULL) {
    return 0xFFFFFFFF; // Return an invalid ID.
  }

  CpuCollectedData   = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber];

  return (CpuCollectedData->CpuidData.CpuIdLeaf[1].RegEbx >> 24) & 0xff;
}

/**
  Get the location of specified processor.

  This function gets the location of specified processor, including
  package number, core number within package, thread number within core.

  @param  ProcessorNumber Handle number of specified logical processor.
  @param  PackageNumber   Pointer to the output package number.
  @param  CoreNumber      Pointer to the output core number.
  @param  ThreadNumber    Pointer to the output thread number.

**/
VOID
EFIAPI
GetProcessorLocation (
  IN    UINTN     ProcessorNumber,
  OUT   UINT32    *PackageNumber   OPTIONAL,
  OUT   UINT32    *CoreNumber      OPTIONAL,
  OUT   UINT32    *ThreadNumber    OPTIONAL
)
{
  CPU_COLLECTED_DATA          *CpuCollectedData;

  ASSERT (mCpuConfigLibConfigContextBuffer != NULL);
  if (mCpuConfigLibConfigContextBuffer == NULL) {
    return; // Do not populate processor data.
  }

  CpuCollectedData   = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber];

  //
  // If PackageNumber is not NULL, set the package number of the specified processor to it.
  //
  if (PackageNumber != NULL) {
    *PackageNumber = CpuCollectedData->ProcessorLocation.Package;
  }

  //
  // If CoreNumber is not NULL, set the core number within package to it.
  //
  if (CoreNumber != NULL) {
    *CoreNumber    = CpuCollectedData->ProcessorLocation.Core;
  }

  //
  // If ThreadNumber is not NULL, set the thread number within core to it.
  //
  if (ThreadNumber != NULL) {
    *ThreadNumber  = CpuCollectedData->ProcessorLocation.Thread;
  }
}

/**
  Add an entry in specified register table.

  This function adds an entry in specified register table, with given register type,
  register index, bit section and value.

  No entry will be added after SaveRegisterTable is called.

  @param  PreSmmInit         Specify the target register table.
                             If TRUE, the target is the pre-SMM-init register table.
                             If FALSE, the target is the post-SMM-init register table.
  @param  ProcessorNumber    Handle number of specified logical processor
  @param  RegisterType       Type of the register to program
  @param  Index              Index of the register to program
  @param  ValidBitStart      Start of the bit section
  @param  ValidBitLength     Length of the bit section
  @param  Value              Value to write

**/
VOID
EFIAPI
WriteRegisterTableEx (
  IN BOOLEAN              PreSmmInit,
  IN UINTN                ProcessorNumber,
  IN REGISTER_TYPE        RegisterType,
  IN UINT32               Index,
  IN UINT8                ValidBitStart,
  IN UINT8                ValidBitLength,
  IN UINT64               Value
  )
{
  CPU_REGISTER_TABLE       *RegisterTable;
  CPU_REGISTER_TABLE_ENTRY *RegisterTableEntry;

  ASSERT (mCpuConfigLibConfigContextBuffer != NULL);
  if (mCpuConfigLibConfigContextBuffer == NULL) {
    return; // No register table to write to.
  }

  if (mCpuConfigLibConfigContextBuffer->RegisterTableSaved) {
    DEBUG ((DEBUG_ERROR, "ERROR: No register table should be written after register table is saved\n"));
    return;
  }

  if (PreSmmInit) {
    RegisterTable = &mCpuConfigLibConfigContextBuffer->PreSmmInitRegisterTable[ProcessorNumber];
  } else {
    RegisterTable = &mCpuConfigLibConfigContextBuffer->RegisterTable[ProcessorNumber];
  }

  if (RegisterTable->TableLength == RegisterTable->AllocatedSize / sizeof (CPU_REGISTER_TABLE_ENTRY)) {
    DEBUG ((DEBUG_ERROR, "ERROR: Register table is full\n"));
    DEBUG ((DEBUG_ERROR, "  PcdCpuConfigRegTblEntryMaxCount should be configured to > 0x%x\n", RegisterTable->TableLength));
    ASSERT (FALSE);
    return;
  }

  //
  // Append entry in the register table.
  //
  RegisterTableEntry = (CPU_REGISTER_TABLE_ENTRY *) (UINTN) RegisterTable->RegisterTableEntry;
  RegisterTableEntry[RegisterTable->TableLength].RegisterType   = RegisterType;
  RegisterTableEntry[RegisterTable->TableLength].Index          = Index;
  RegisterTableEntry[RegisterTable->TableLength].ValidBitStart  = ValidBitStart;
  RegisterTableEntry[RegisterTable->TableLength].ValidBitLength = ValidBitLength;
  RegisterTableEntry[RegisterTable->TableLength].Value          = Value;

  RegisterTable->TableLength++;
}

/**
  Add an entry in the post-SMM-init register table.

  This function adds an entry in the post-SMM-init register table, with given register type,
  register index, bit section and value.

  No entry will be added after SaveRegisterTable is called.

  @param  ProcessorNumber Handle number of specified logical processor
  @param  RegisterType    Type of the register to program
  @param  Index           Index of the register to program
  @param  ValidBitStart   Start of the bit section
  @param  ValidBitLength  Length of the bit section
  @param  Value           Value to write

**/
VOID
EFIAPI
WriteRegisterTable (
  IN  UINTN               ProcessorNumber,
  IN  REGISTER_TYPE       RegisterType,
  IN  UINT32              Index,
  IN  UINT8               ValidBitStart,
  IN  UINT8               ValidBitLength,
  IN  UINT64              Value
  )
{
  WriteRegisterTableEx (
    FALSE,
    ProcessorNumber,
    RegisterType,
    Index,
    ValidBitStart,
    ValidBitLength,
    Value
    );
}

/**
  Add an entry in the pre-SMM-init register table.

  This function adds an entry in the pre-SMM-init register table, with given register type,
  register index, bit section and value.

  No entry will be added after SaveRegisterTable is called.

  @param  ProcessorNumber Handle number of specified logical processor
  @param  RegisterType    Type of the register to program
  @param  Index           Index of the register to program
  @param  ValidBitStart   Start of the bit section
  @param  ValidBitLength  Length of the bit section
  @param  Value           Value to write

**/
VOID
EFIAPI
WritePreSmmInitRegisterTable (
  IN  UINTN               ProcessorNumber,
  IN  REGISTER_TYPE       RegisterType,
  IN  UINT32              Index,
  IN  UINT8               ValidBitStart,
  IN  UINT8               ValidBitLength,
  IN  UINT64              Value
  )
{
  WriteRegisterTableEx (
    TRUE,
    ProcessorNumber,
    RegisterType,
    Index,
    ValidBitStart,
    ValidBitLength,
    Value
    );
}

/**
  Combine the register table entries from Register Table 2 to 1.

  @param RegisterTable1     Pointer to Register Table 1.
  @param RegisterTable2     Pointer to Register Table 2.

**/
VOID
CombineRegisterTable (
  IN OUT CPU_REGISTER_TABLE *RegisterTable1,
  IN OUT CPU_REGISTER_TABLE *RegisterTable2
  )
{
  EFI_STATUS                Status;
  CPU_REGISTER_TABLE_ENTRY  *RegisterTableEntry1;
  CPU_REGISTER_TABLE_ENTRY  *RegisterTableEntry2;
  UINT32                    TotalTableLength;
  EFI_PHYSICAL_ADDRESS      Address;
  UINTN                     RequiredSize;
  UINTN                     AllocatePages;

  if (RegisterTable2->TableLength == 0) {
    //
    // No any entry in Register Table 2, so no need to combine.
    //
    if (RegisterTable2->AllocatedSize != 0) {
      //
      // Free the pre-allocated RegisterTableEntry2 buffer.
      //
      Status = gBS->FreePages (
                      RegisterTable2->RegisterTableEntry,
                      EFI_SIZE_TO_PAGES (RegisterTable2->AllocatedSize)
                      );
      ASSERT_EFI_ERROR (Status);
      RegisterTable2->AllocatedSize      = 0;
      RegisterTable2->RegisterTableEntry = 0;
    }
    return;
  }

  RegisterTableEntry1 = (CPU_REGISTER_TABLE_ENTRY *) (UINTN) RegisterTable1->RegisterTableEntry;
  RegisterTableEntry2 = (CPU_REGISTER_TABLE_ENTRY *) (UINTN) RegisterTable2->RegisterTableEntry;
  TotalTableLength = RegisterTable1->TableLength + RegisterTable2->TableLength;
  RequiredSize = TotalTableLength * sizeof (CPU_REGISTER_TABLE_ENTRY);
  if (RegisterTable1->AllocatedSize >= RequiredSize) {
    //
    // Table 1 is big enough to hold the combined table.
    //
    CopyMem (
      &RegisterTableEntry1[RegisterTable1->TableLength],
      RegisterTableEntry2,
      RegisterTable2->TableLength * sizeof (CPU_REGISTER_TABLE_ENTRY)
      );

    //
    // Free RegisterTableEntry2 buffer.
    //
    Status = gBS->FreePages (
                    RegisterTable2->RegisterTableEntry,
                    EFI_SIZE_TO_PAGES (RegisterTable2->AllocatedSize)
                    );
    ASSERT_EFI_ERROR (Status);
  } else if (RegisterTable2->AllocatedSize >= RequiredSize) {
    //
    // Table 2 is big enough to hold the combined table.
    //
    if (RegisterTable1->TableLength != 0) {
      CopyMem (
        &RegisterTableEntry2[RegisterTable2->TableLength],
        RegisterTableEntry1,
        RegisterTable1->TableLength * sizeof (CPU_REGISTER_TABLE_ENTRY)
        );

      //
      // Free RegisterTableEntry1 buffer.
      //
      Status = gBS->FreePages (
                      RegisterTable1->RegisterTableEntry,
                      EFI_SIZE_TO_PAGES (RegisterTable1->AllocatedSize)
                      );
      ASSERT_EFI_ERROR (Status);
    }

    //
    // Let RegisterTable1->RegisterTableEntry point to RegisterTableEntry2 buffer.
    //
    RegisterTable1->RegisterTableEntry = RegisterTable2->RegisterTableEntry;
    RegisterTable1->AllocatedSize = RegisterTable2->AllocatedSize;
  } else {
    //
    // Allocate new buffer to hold the combined table.
    //
    AllocatePages = EFI_SIZE_TO_PAGES (RequiredSize);
    Status = gBS->AllocatePages (
                    AllocateAnyPages,
                    EfiBootServicesData,
                    AllocatePages,
                    &Address
                    );
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return;
    }
    CopyMem (
      (VOID *) (UINTN) Address,
      RegisterTableEntry1,
      RegisterTable1->TableLength * sizeof (CPU_REGISTER_TABLE_ENTRY)
      );
    CopyMem (
      (VOID *) ((UINTN) Address + RegisterTable1->TableLength * sizeof (CPU_REGISTER_TABLE_ENTRY)),
      RegisterTableEntry2,
      RegisterTable2->TableLength * sizeof (CPU_REGISTER_TABLE_ENTRY)
      );

    //
    // Free RegisterTableEntry1 buffer.
    //
    Status = gBS->FreePages (
                    RegisterTable1->RegisterTableEntry,
                    EFI_SIZE_TO_PAGES (RegisterTable1->AllocatedSize)
                    );
    ASSERT_EFI_ERROR (Status);
    //
    // Free RegisterTableEntry2 buffer.
    //
    Status = gBS->FreePages (
                    RegisterTable2->RegisterTableEntry,
                    EFI_SIZE_TO_PAGES (RegisterTable2->AllocatedSize)
                    );
    ASSERT_EFI_ERROR (Status);

    //
    // Let RegisterTable1->RegisterTableEntry point to the new allocated buffer.
    //
    RegisterTable1->RegisterTableEntry = Address;
    RegisterTable1->AllocatedSize = (UINT32) EFI_PAGES_TO_SIZE (AllocatePages);
  }
  //
  // Update RegisterTable1->TableLength.
  //
  RegisterTable1->TableLength        = TotalTableLength;
  //
  // The register table entries in Register Table 2 have been combined
  // to Register Table 1.
  //
  RegisterTable2->TableLength        = 0;
  RegisterTable2->AllocatedSize      = 0;
  RegisterTable2->RegisterTableEntry = 0;
}

/**
  Save RegisterTable and PreSmmInitRegisterTable in mCpuConfigLibConfigContextBuffer
  to RegisterTable and PreSmmInitRegisterTable in PcdCpuS3DataAddress.

  It will combine the table entries from mCpuConfigLibConfigContextBuffer
  to PcdCpuS3DataAddress.

  It should be called at DxeSmmReadyToLock event with TPL_NOTIFY to make sure
  that it is after all Write(PreSmmInit)RegisterTable callings and before
  PcdCpuS3DataAddress is consumed by PiSmmCpuDxeSmm for CPU S3.

**/
VOID
EFIAPI
SaveRegisterTable (
  VOID
  )
{
  ACPI_CPU_DATA           *AcpiCpuData;
  CPU_REGISTER_TABLE      *RegisterTableArray1;
  CPU_REGISTER_TABLE      *RegisterTableArray2;
  CPU_REGISTER_TABLE      *PreSmmInitRegisterTable1;
  CPU_REGISTER_TABLE      *PreSmmInitRegisterTable2;
  UINTN                   Index;

  AcpiCpuData = (ACPI_CPU_DATA *) (UINTN) PcdGet64 (PcdCpuS3DataAddress);
  ASSERT (AcpiCpuData != NULL);
  if (AcpiCpuData == NULL) {
    return;
  }

  RegisterTableArray1 = (CPU_REGISTER_TABLE *) (UINTN) AcpiCpuData->RegisterTable;
  RegisterTableArray2 = mCpuConfigLibConfigContextBuffer->RegisterTable;
  PreSmmInitRegisterTable1 = (CPU_REGISTER_TABLE *) (UINTN) AcpiCpuData->PreSmmInitRegisterTable;
  PreSmmInitRegisterTable2 = mCpuConfigLibConfigContextBuffer->PreSmmInitRegisterTable;
  for (Index = 0; Index < AcpiCpuData->NumberOfCpus; Index++) {
    CombineRegisterTable (
      &RegisterTableArray1[Index],
      &RegisterTableArray2[Index]
      );
    CombineRegisterTable (
      &PreSmmInitRegisterTable1[Index],
      &PreSmmInitRegisterTable2[Index]
      );
  }

  mCpuConfigLibConfigContextBuffer->RegisterTableSaved = TRUE;
}
