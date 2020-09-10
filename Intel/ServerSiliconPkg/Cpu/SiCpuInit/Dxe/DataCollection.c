/** @file
  Code for Data Collection phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include "SiCpuInitDxe.h"

/**
  Collects Local APIC data of the processor.

  This function collects Local APIC base, verion, and APIC ID of the processor.

  @param  ProcessorNumber    Handle number of specified logical processor

**/
VOID
CollectApicData (
  UINTN    ProcessorNumber
  )
{
  CPU_MISC_DATA          *CpuMiscData;
  UINT32                 LocalApicBaseAddress;

  CpuMiscData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData;

  LocalApicBaseAddress = (UINT32) GetLocalApicBaseAddress();
  CpuMiscData->ApicBase    = LocalApicBaseAddress;
  //
  // Read bits 0..7 of Local APIC Version Register for Local APIC version.
  //
  CpuMiscData->ApicVersion = GetApicVersion () & 0xff;
  //
  // Read bits 24..31 of Local APIC ID Register for Local APIC ID
  //
  CpuMiscData->ApicID        = GetApicId ();
  CpuMiscData->InitialApicID = GetInitialApicId ();
}

/**
  Collects all CPUID leafs the processor.

  This function collects all CPUID leafs the processor.

  @param  ProcessorNumber    Handle number of specified logical processor

**/
VOID
CollectCpuidLeafs (
  UINTN    ProcessorNumber
  )
{
  CPU_COLLECTED_DATA   *CpuCollectedData;
  EFI_CPUID_REGISTER   *CpuidRegisters;
  UINT32               Index;

  CpuCollectedData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber];
  //
  // Collect basic CPUID information.
  //
  CpuidRegisters = CpuCollectedData->CpuidData.CpuIdLeaf;
  for (Index = 0; Index < GetNumberOfCpuidLeafs (ProcessorNumber, BasicCpuidLeaf); Index++) {
    AsmCpuid (
      Index,
      &CpuidRegisters->RegEax,
      &CpuidRegisters->RegEbx,
      &CpuidRegisters->RegEcx,
      &CpuidRegisters->RegEdx
      );
    CpuidRegisters++;
  }

  //
  // Collect extended function CPUID information.
  //
  for (Index = 0; Index < GetNumberOfCpuidLeafs (ProcessorNumber, ExtendedCpuidLeaf); Index++) {
    AsmCpuid (
      Index + 0x80000000,
      &CpuidRegisters->RegEax,
      &CpuidRegisters->RegEbx,
      &CpuidRegisters->RegEcx,
      &CpuidRegisters->RegEdx
      );
    CpuidRegisters++;
  }

  //
  // Collect additional Cache & TLB information, if exists.
  //
  for (Index = 1; Index < GetNumberOfCpuidLeafs (ProcessorNumber, CacheAndTlbCpuidLeafs); Index++) {
    AsmCpuid (
      2,
      &CpuidRegisters->RegEax,
      &CpuidRegisters->RegEbx,
      &CpuidRegisters->RegEcx,
      &CpuidRegisters->RegEdx
      );
    CpuidRegisters++;
  }

  //
  // Collect Deterministic Cache Parameters Leaf.
  //
  for (Index = 0; Index < GetNumberOfCpuidLeafs (ProcessorNumber, DeterministicCacheParametersCpuidLeafs); Index++) {
    AsmCpuidEx (
      4,
      Index,
      &CpuidRegisters->RegEax,
      &CpuidRegisters->RegEbx,
      &CpuidRegisters->RegEcx,
      &CpuidRegisters->RegEdx
      );
    CpuidRegisters++;
  }

  //
  // Collect Extended Topology Enumeration Leaf.
  //
  for (Index = 0; Index < GetNumberOfCpuidLeafs (ProcessorNumber, ExtendedTopologyEnumerationCpuidLeafs); Index++) {
    AsmCpuidEx (
      CPUID_EXTENDED_TOPOLOGY,
      Index,
      &CpuidRegisters->RegEax,
      &CpuidRegisters->RegEbx,
      &CpuidRegisters->RegEcx,
      &CpuidRegisters->RegEdx
      );
    CpuidRegisters++;
  }
}

/**
  Collects physical location of the processor.

  This function gets Package ID/Core ID/Thread ID of the processor.

  The algorithm below assumes the target system has symmetry across physical package boundaries
  with respect to the number of logical processors per package, number of cores per package.

  @param  InitialApicId  Initial APIC ID for determing processor topology.
  @param  Location       Pointer to EFI_CPU_PHYSICAL_LOCATION structure.
  @param  ThreadIdBits   Number of bits occupied by Thread ID portion.
  @param  CoreIdBits     Number of bits occupied by Core ID portion.

**/
VOID
CpuExtractProcessorLocation (
  IN  UINT32                    InitialApicId,
  OUT EFI_CPU_PHYSICAL_LOCATION *Location,
  OUT UINTN                     *ThreadIdBits,
  OUT UINTN                     *CoreIdBits
  )
{
  BOOLEAN  TopologyLeafSupported;
  UINTN    ThreadBits;
  UINTN    CoreBits;
  UINT32   RegEax;
  UINT32   RegEbx;
  UINT32   RegEcx;
  UINT32   RegEdx;
  UINT32   MaxCpuIdIndex;
  UINT32   SubIndex;
  UINTN    LevelType;
  UINT32   MaxLogicProcessorsPerPackage;
  UINT32   MaxCoresPerPackage;

  //
  // Check if the processor is capable of supporting more than one logical processor.
  //
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, NULL, &RegEdx);
  if ((RegEdx & BIT28) == 0) {
    Location->Thread  = 0;
    Location->Core    = 0;
    Location->Package = 0;
    *ThreadIdBits     = 0;
    *CoreIdBits       = 0;
    return;
  }

  ThreadBits = 0;
  CoreBits = 0;

  //
  // Assume three-level mapping of APIC ID: Package:Core:SMT.
  //

  TopologyLeafSupported = FALSE;
  //
  // Get the max index of basic CPUID
  //
  AsmCpuid (CPUID_SIGNATURE, &MaxCpuIdIndex, NULL, NULL, NULL);

  //
  // If the extended topology enumeration leaf is available, it
  // is the preferred mechanism for enumerating topology.
  //
  if (MaxCpuIdIndex >= CPUID_EXTENDED_TOPOLOGY) {
    AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 0, &RegEax, &RegEbx, &RegEcx, NULL);
    //
    // If CPUID.(EAX=0BH, ECX=0H):EBX returns zero and maximum input value for
    // basic CPUID information is greater than 0BH, then CPUID.0BH leaf is not
    // supported on that processor.
    //
    if (RegEbx != 0) {
      TopologyLeafSupported = TRUE;

      //
      // Sub-leaf index 0 (ECX= 0 as input) provides enumeration parameters to extract
      // the SMT sub-field of x2APIC ID.
      //
      LevelType = (RegEcx >> 8) & 0xff;
      ASSERT (LevelType == CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_SMT);

//
// PURLEY_OVERRIDE_BEGIN
//
      //
      //HSD: 5330351  for SKX PO, core location should right shift one on Purley whatever HT enable or not.
      //
      ThreadBits = RegEax & 0x1f;
//
// PURLEY_OVERRIDE_END
//
      //
      // Software must not assume any "level type" encoding
      // value to be related to any sub-leaf index, except sub-leaf 0.
      //
      SubIndex = 1;
      do {
        AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, SubIndex, &RegEax, NULL, &RegEcx, NULL);
        LevelType = (RegEcx >> 8) & 0xff;
        if (LevelType == CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_CORE) {
          CoreBits = (RegEax & 0x1f) - ThreadBits;
          break;
        }
        SubIndex++;
      } while (LevelType != CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_INVALID);
    }
  }

  if (!TopologyLeafSupported) {
    AsmCpuid (CPUID_VERSION_INFO, NULL, &RegEbx, NULL, NULL);
    MaxLogicProcessorsPerPackage = (RegEbx >> 16) & 0xff;
    if (MaxCpuIdIndex >= CPUID_CACHE_PARAMS) {
      AsmCpuidEx (CPUID_CACHE_PARAMS, 0, &RegEax, NULL, NULL, NULL);
      MaxCoresPerPackage = (RegEax >> 26) + 1;
    } else {
      //
      // Must be a single-core processor.
      //
      MaxCoresPerPackage = 1;
    }

    ThreadBits = (UINTN) (HighBitSet32 (MaxLogicProcessorsPerPackage / MaxCoresPerPackage - 1) + 1);
    CoreBits = (UINTN) (HighBitSet32 (MaxCoresPerPackage - 1) + 1);
  }

  Location->Thread  = InitialApicId & ~((-1) << ThreadBits);
  Location->Core    = (InitialApicId >> ThreadBits) & ~((-1) << CoreBits);
  Location->Package = (InitialApicId >> (ThreadBits+ CoreBits));
  *ThreadIdBits     = ThreadBits;
  *CoreIdBits       = CoreBits;
}

/**
  Collects physical location of the processor.

  This function collects physical location of the processor.

  @param  ProcessorNumber    Handle number of specified logical processor

**/
VOID
CollectProcessorLocation (
  UINTN    ProcessorNumber
  )
{
  CPU_COLLECTED_DATA  *CpuCollectedData;
  UINT32              InitialApicID;
  UINTN               ThreadIdBits;
  UINTN               CoreIdBits;

  CpuCollectedData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber];
  InitialApicID    = CpuCollectedData->CpuMiscData.InitialApicID;

  CpuExtractProcessorLocation (
    InitialApicID,
    &CpuCollectedData->ProcessorLocation,
    &ThreadIdBits,
    &CoreIdBits
    );

  CpuCollectedData->PackageIdBitOffset = (UINT8)(ThreadIdBits+ CoreIdBits);
}

/**
  Collects intended FSB frequency and core to bus ratio.

  This function collects intended FSB frequency and core to bus ratio.

  @param  ProcessorNumber    Handle number of specified logical processor

**/
VOID
CollectFrequencyData (
  UINTN    ProcessorNumber
  )
{
  CPU_MISC_DATA          *CpuMiscData;
  UINT8                  FsbFrequencyEncoding;
  UINT32                 FamilyId;
  UINT32                 ModelId;
  UINT32                 SteppingId;
  UINT8                  LevelSelect;
  BOOLEAN                OldInterruptState;
  UINT64                 BeginValue;
  UINT64                 EndValue;
  UINT64                 ActualFrequency;
  UINT64                 ActualFsb;
  UINT32                 PkgMaxPwr;
  UINT32                 PkgMinPwr;
  MSR_PACKAGE_POWER_SKU_UNIT_REGISTER       PkgPowerSkuUnitMsr;
  MSR_PACKAGE_POWER_LIMIT_REGISTER          PkgPowerLimitMsr;
  ICX_MSR_TURBO_RATIO_LIMIT_REGISTER        TurboRatioLimitMsr;
  ICX_MSR_PLATFORM_INFO_REGISTER            PlatformInfoMsr;
  UINT32                 RegEax;
  UINT32                 RegEdx;
  UINT8                  SocketNum;
  UINT8                  CurrentSocket;
  CPU_CSR_ACCESS_VAR     *CpuCsrAccessVarPtr;

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar();
  AsmCpuidEx(CPUID_EXTENDED_TOPOLOGY, 1, &RegEax, NULL, NULL, &RegEdx);
  CurrentSocket = (UINT8) (RegEdx >> (RegEax & 0x1F)) ;

  CpuMiscData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData;

  PkgMaxPwr = 0;
  PkgMinPwr = 0;

  GetProcessorVersionInfo (ProcessorNumber, &FamilyId, &ModelId, &SteppingId, NULL);
  FsbFrequencyEncoding = 0xff;

  switch (FamilyId) {

  case 0x06:
    switch (ModelId) {

    case SKYLAKE_SERVER_MODEL_ID:
    case ICELAKE_SERVER_MODEL_ID:
    case SAPPHIRERAPIDS_SERVER_MODEL_ID:
    case SNOWRIDGE_SERVER_MODEL_ID:
    case ICELAKE_D_SERVER_MODEL_ID:

      if (FeaturePcdGet (PcdCpuSkylakeFamilyFlag) ||
          FeaturePcdGet (PcdCpuIcelakeFamilyFlag) ||
          FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) ||
          FeaturePcdGet (PcdCpuSnowridgeFamilyFlag)
          ) {

        //
        // Force 100MHz BCLK value.
        //
        FsbFrequencyEncoding = 5;

        //
        // Get the minimum and maximum non-turbo core ratios and maximum turbo ratio.
        //
        PlatformInfoMsr.Uint64 = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);
        CpuMiscData->MinCoreToBusRatio = PlatformInfoMsr.Bits.MaxEfficiencyRatio;
        CpuMiscData->MaxCoreToBusRatio = PlatformInfoMsr.Bits.MaxNonTurboLimRatio;
        TurboRatioLimitMsr.Uint64      = AsmReadMsr64 (ICX_MSR_TURBO_RATIO_LIMIT);
        CpuMiscData->MaxTurboRatio     = (UINTN) TurboRatioLimitMsr.Bits.MaxTurbo1Core;

        //
        // Temporary fix to force CpuMiscData->MaxTurboRatio value above MaxCoreToBusRatio
        // if TurboRatioLimitMsr.Bits.MaxTurbo1Core is "0"
        //
        if (CpuMiscData->MaxTurboRatio == 0) {
          CpuMiscData->MaxTurboRatio = (UINTN) CpuMiscData->MaxCoreToBusRatio + 4;
        }

        //
        // Get power scaling information.
        //
        if ((CpuCsrAccessVarPtr->SlaveDieBitMap > 0) && (CurrentSocket > 0)) {
          if (CpuCsrAccessVarPtr->SlaveDieBitMap & (1 << CurrentSocket)) {
            SocketNum = CurrentSocket - 1;
          } else {
            SocketNum = CurrentSocket;
          }
            PkgPowerLimitMsr.Uint64 = (UINT64) mCpuCsrAccess->ReadCpuCsr(SocketNum, 0, PACKAGE_RAPL_LIMIT_N0_PCU_FUN0_REG);
            PkgPowerSkuUnitMsr.Uint64 = (UINT64) mCpuCsrAccess->ReadCpuCsr(SocketNum, 0, PACKAGE_POWER_SKU_UNIT_PCU_FUN0_REG);
        } else {
          PkgPowerLimitMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_POWER_LIMIT);
          PkgPowerSkuUnitMsr.Uint64 = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU_UNIT);
        }
        //
        // Convert to milliWatt
        //
        CpuMiscData->PackageTdp = (UINTN) PkgPowerLimitMsr.Bits.PkgPwrLim1 * 1000;
        CpuMiscData->PackageTdp >>= (UINTN) PkgPowerSkuUnitMsr.Bits.PwrUnit;

        if ((FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (FamilyId, ModelId)) ||
            (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_SERVER_PROC (FamilyId, ModelId)) ||
            (FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) && IS_SAPPHIRERAPIDS_SERVER_PROC (FamilyId, ModelId)) ||
            (FeaturePcdGet (PcdCpuSnowridgeFamilyFlag) && IS_SNOWRIDGE_SERVER_PROC (FamilyId, ModelId)) ||
            (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_D_SERVER_PROC (FamilyId, ModelId))) {
          //
          // Collect Misc Enables MSR for this processor
          //
          CpuMiscData->MiscEnablesMsr = AsmReadMsr64 (MSR_MISC_ENABLES);

          //
          // Get ISS/config TDP info, assuming one is in effect
          //
          LevelSelect = mCpuVarDataPtr->IssConfigTdpCurrentLevel;
          CpuMiscData->PackageTdp = (UINTN) (mCpuVarDataPtr->IssConfigTdpPower[CurrentSocket][LevelSelect]) * 1000;
          CpuMiscData->MaxCoreToBusRatio = (UINTN) (mCpuVarDataPtr->IssConfigTdpRatio[CurrentSocket][LevelSelect]);
        }

        //
        // PURLEY_OVERRIDE_BEGIN_5371439
        //
        if ((FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (FamilyId, ModelId)) ||
        (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_SERVER_PROC (FamilyId, ModelId)) ||
        (FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) && IS_SAPPHIRERAPIDS_SERVER_PROC (FamilyId, ModelId)) ||
        (FeaturePcdGet (PcdCpuSnowridgeFamilyFlag) && IS_SNOWRIDGE_SERVER_PROC (FamilyId, ModelId)) ||
        (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_D_SERVER_PROC (FamilyId, ModelId))) {
          //
          // Collect enabled core and thread count MSR for this processor
          //
          CpuMiscData->EnabledThreadCountMsr = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);
        }
        //
        // PURLEY_OVERRIDE_END_5371439
        //
      }
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }

  //
  // Collect intended FSB frequency
  //
  switch (FsbFrequencyEncoding) {
  case 0:
    CpuMiscData->IntendedFsbFrequency = 267;
    break;
  case 1:
    CpuMiscData->IntendedFsbFrequency = 133;
    break;
  case 2:
    CpuMiscData->IntendedFsbFrequency = 200;
    break;
  case 3:
    CpuMiscData->IntendedFsbFrequency = 166;
    break;
  case 4:
    CpuMiscData->IntendedFsbFrequency = 333;
    break;
  case 5:
    CpuMiscData->IntendedFsbFrequency = 100;
    break;
  case 6:
    CpuMiscData->IntendedFsbFrequency = 83;
    break;
  case 7:
    CpuMiscData->IntendedFsbFrequency = 117;
    break;
  default:
    CpuMiscData->IntendedFsbFrequency = 0;
    break;
  }

  //
  // Calculate actual FSB frequency
  // First calculate actual frequency by sampling some time and counts TSC
  // Use spinlock mechanism because timer library cannot handle concurrent requests.
  //
  AcquireSpinLock (&mMPSystemData.APSerializeLock);
  OldInterruptState = SaveAndDisableInterrupts ();
  BeginValue  = AsmReadTsc ();
  MicroSecondDelay (1000);
  EndValue    = AsmReadTsc ();
  SetInterruptState (OldInterruptState);
  ReleaseSpinLock (&mMPSystemData.APSerializeLock);

//
// PURLEY_OVERRIDE_BEGIN
//
  //
  // MaxCoreToBusRatio is used to calculate FSB frequency and with value zero, BIOS asserts. Hence as a workaround
  // if MaxCoreToBusRatio is zero, updating MaxCoreToBusRatio, MinCoreToBusRatio and
  // MaxTurboRatio with the values read from actual hardware.
  //
  if (CpuMiscData->MaxCoreToBusRatio == 0) {
    CpuMiscData->MinCoreToBusRatio = 0xc;
    CpuMiscData->MaxCoreToBusRatio = 0x17;
    CpuMiscData->MaxTurboRatio = 0x19;
  }
//
// PURLEY_OVERRIDE_END
//
  //
  // Calculate raw actual FSB frequency
  //
  ActualFrequency = DivU64x32 (EndValue - BeginValue, 1000);
  ActualFsb = DivU64x32 (ActualFrequency, (UINT32) CpuMiscData->MaxCoreToBusRatio);
  //
  // Round the raw actual FSB frequency to standardized value
  //
  ActualFsb = ActualFsb + RShiftU64 (ActualFsb, 5);
  ActualFsb = DivU64x32 (MultU64x32 (ActualFsb, 3), 100);
  ActualFsb = DivU64x32 (MultU64x32 (ActualFsb, 100), 3);

  CpuMiscData->ActualFsbFrequency = (UINTN) ActualFsb;

  //
  // Default number of P-states is 1
  //
  CpuMiscData->NumberOfPStates = 1;
  CpuMiscData->GreaterNumberOfPStates = 1;
}

/**
  Collects processor microcode revision of the processor installed in the system.

  This function collects processor microcode revision of the processor installed in the system.

  @param  ProcessorNumber    Handle number of specified logical processor

**/
VOID
CollectMicrocodeRevision (
  UINTN    ProcessorNumber
  )
{
  CPU_MISC_DATA          *CpuMiscData;

  CpuMiscData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData;

  AsmWriteMsr64 (MSR_BIOS_SIGN_ID, 0);
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, NULL, NULL);

  CpuMiscData->MicrocodeRevision = (UINT32) RShiftU64 (AsmReadMsr64 (MSR_BIOS_SIGN_ID), 32);
}

/**
  Collects basic processor data for calling processor.

  This function collects basic processor data for calling processor.

  @param  ProcessorNumber    Handle number of specified logical processor.

**/
VOID
CollectBasicProcessorData (
  IN UINTN  ProcessorNumber
  )
{
  CPU_MISC_DATA        *CpuMiscData;

  CpuMiscData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData;

  //
  // A loop to check APIC ID and processor number
  //
  CollectApicData (ProcessorNumber);

  //
  // Get package number, core number and thread number.
  //
  CollectProcessorLocation (ProcessorNumber);
}

/**
  Collects processor data for calling processor.

  This function collects processor data for calling processor.

  @param  ProcessorNumber    Handle number of specified logical processor.

**/
VOID
EFIAPI
CollectProcessorData (
  IN VOID               *Buffer
  )
{
  EFI_STATUS         Status;
  UINTN              ProcessorNumber;

  Status = mMpServices->WhoAmI(mMpServices, &ProcessorNumber);
  ASSERT_EFI_ERROR (Status);

  //
  // Collect all leafs for CPUID after second time microcode load.
  //
  CollectCpuidLeafs (ProcessorNumber);

  //
  // Detect microcode and fill microcode info.
  //
  MicrocodeDetect (ProcessorNumber);

  //
  // Get intended FSB frequency and core to bus ratio
  //
  CollectFrequencyData (ProcessorNumber);
}

/**
  Checks the number of CPUID leafs need by a processor.

  This function check the number of CPUID leafs need by a processor.

  @param  ProcessorNumber    Handle number of specified logical processor.

**/
VOID
EFIAPI
CountNumberOfCpuidLeafs (
  IN VOID              *Buffer
  )
{
  UINT32               MaxCpuidIndex;
  UINT32               MaxExtendedCpuidIndex;
  UINT32               NumberOfCacheAndTlbRecords;
  UINT32               NumberOfDeterministicCacheParametersLeafs;
  UINT32               NumberOfExtendedTopologyEnumerationLeafs;
  UINT32               RegValue;
  CPU_COLLECTED_DATA   *CpuCollectedData;

  EFI_STATUS         Status;
  UINTN              ProcessorNumber;
  Status = mMpServices->WhoAmI(mMpServices, &ProcessorNumber);
  ASSERT_EFI_ERROR (Status);

  CpuCollectedData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber];

  //
  // Get the max index of basic CPUID
  //
  AsmCpuid (0, &MaxCpuidIndex, NULL, NULL, NULL);
  //
  // Get the max index of extended CPUID
  //
  AsmCpuid (0x80000000, &MaxExtendedCpuidIndex, NULL, NULL, NULL);
  //
  // Get the number of cache and TLB CPUID leafs
  //
  AsmCpuid (2, &NumberOfCacheAndTlbRecords, NULL, NULL, NULL);
  NumberOfCacheAndTlbRecords = NumberOfCacheAndTlbRecords & 0xff;

  //
  // Get the number of deterministic cache parameter CPUID leafs
  //
  NumberOfDeterministicCacheParametersLeafs = 0;
  do {
    AsmCpuidEx (4, NumberOfDeterministicCacheParametersLeafs++, &RegValue, NULL, NULL, NULL);
  } while ((RegValue & 0x0f) != 0);

  //
  // Get the number of Extended Topology Enumeration CPUID leafs
  //
  NumberOfExtendedTopologyEnumerationLeafs = 0;
  if (MaxCpuidIndex >= CPUID_EXTENDED_TOPOLOGY) {
    do {
      AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, NumberOfExtendedTopologyEnumerationLeafs++, NULL, &RegValue, NULL, NULL);
    } while ((RegValue & 0x0FFFF) != 0);
  }

  //
  // Save collected data in Processor Configuration Context Buffer
  //
  CpuCollectedData->CpuidData.NumberOfBasicCpuidLeafs                        = MaxCpuidIndex + 1;
  CpuCollectedData->CpuidData.NumberOfExtendedCpuidLeafs                     = (MaxExtendedCpuidIndex - 0x80000000) + 1;
  CpuCollectedData->CpuidData.NumberOfCacheAndTlbCpuidLeafs                  = NumberOfCacheAndTlbRecords;
  CpuCollectedData->CpuidData.NumberOfDeterministicCacheParametersCpuidLeafs = NumberOfDeterministicCacheParametersLeafs;
  CpuCollectedData->CpuidData.NumberOfExtendedTopologyEnumerationLeafs       = NumberOfExtendedTopologyEnumerationLeafs;
}

/**
  Checks the number of CPUID leafs of all logical processors, and allocate memory for them.

  This function checks the number of CPUID leafs of all logical processors, and allocates memory for them.

**/
VOID
AllocateMemoryForCpuidLeafs (
  VOID
  )
{
  CPU_COLLECTED_DATA   *CpuCollectedData;
  UINTN                ProcessorNumber;
  UINTN                NumberOfLeafs;
  EFI_STATUS           Status;

  Status = mMpServices->StartupAllAPs (
                 mMpServices,
                 CountNumberOfCpuidLeafs,
                 FALSE,
                 NULL,
                 0,
                 NULL,
                 NULL
                 );
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Error: Not able to StartupAllAPs.  Status: %r\n", Status));
  }

  //
  // Check number of CPUID leafs for BSP.
  // Try to accomplish in first wakeup, and MTRR.
  //
  CountNumberOfCpuidLeafs (NULL);

  //
  // Allocate memory for CPUID leafs of all processors
  //
  for (ProcessorNumber = 0; ProcessorNumber < mCpuConfigLibConfigContextBuffer->NumberOfProcessors; ProcessorNumber++) {

    CpuCollectedData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber];

    //
    // Get the number of basic CPUID leafs.
    //
    NumberOfLeafs = CpuCollectedData->CpuidData.NumberOfBasicCpuidLeafs;
    //
    // Get the number of extended CPUID leafs.
    //
    NumberOfLeafs += CpuCollectedData->CpuidData.NumberOfExtendedCpuidLeafs;
    //
    // Get the number of cache and TLB CPUID leafs.
    //
    NumberOfLeafs += CpuCollectedData->CpuidData.NumberOfCacheAndTlbCpuidLeafs - 1;
    //
    // Get the number of deterministic cache parameters CPUID leafs.
    //
    NumberOfLeafs += CpuCollectedData->CpuidData.NumberOfDeterministicCacheParametersCpuidLeafs;
    //
    // Get the number of Extended Topology Enumeration CPUID leafs
    //
    NumberOfLeafs += CpuCollectedData->CpuidData.NumberOfExtendedTopologyEnumerationLeafs;

    CpuCollectedData->CpuidData.CpuIdLeaf = AllocateZeroPool (sizeof (EFI_CPUID_REGISTER) * NumberOfLeafs);
  }
}

/**
  Calculate the FVID table for one logical processor based on CPU index.

  @param[in] ProcessorNumber    The index of the CPU.
  @param[in] TurboMode          TRUE/FALSE for enable/disable.

**/
VOID
CalculateFvidTable (
  IN UINTN          ProcessorNumber,
  IN BOOLEAN        TurboMode
  )
{
  UINTN             Index;

  UINTN             MaxRatio;
  UINTN             MinRatio;
  UINTN             RatioStepSize;

  UINTN             NumberOfPStates;
  FVID_ENTRY        *FvidEntry;
  CPU_MISC_DATA     *CpuMiscData;

  UINT64            PowerSlope;
  UINT64            RatioPower;
  UINTN             CurrentRatio;
  UINTN             PackageTdp;
  UINTN             TableIndex;
  UINTN             NumGaps;
  UINTN             RatioRange;
  UINT32            DebugOutputLevel;

  NumGaps = 0;

  //
  // Get the core to bus ratio and VID range for the processor
  //
  CpuMiscData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData;
  MaxRatio    = CpuMiscData->MaxCoreToBusRatio;
  MinRatio    = CpuMiscData->MinCoreToBusRatio;
  PackageTdp  = CpuMiscData->PackageTdp;
  RatioRange  = MaxRatio - MinRatio;

  RatioStepSize = 1;

  //
  // Compute the number of P-States.
  //
  CpuMiscData->NumberOfPStates = (RatioRange / RatioStepSize) + 1;
  if (TurboMode) {
    CpuMiscData->NumberOfPStates += 1;
  }

  if (CpuMiscData->NumberOfPStates > 16) {
    CpuMiscData->NumberOfPStates = 16;
  }
  NumGaps = CpuMiscData->NumberOfPStates - 1;
  if (TurboMode) {
    NumGaps -= 1;
  }

  //
  // Allocate buffer to put P-State information in.
  //
  CpuMiscData->FvidTable = AllocatePool (CpuMiscData->NumberOfPStates * sizeof (FVID_ENTRY));
  ASSERT (CpuMiscData->FvidTable != NULL);
  if (CpuMiscData->FvidTable == NULL) {
    return;
  }

  //
  // According to BWG,
  // 1. If Turbo Mode is available and enabled, P(0) = Max Turbo Ratio, P(1) = Max Non-Turbo Ratio,
  //   else P(0) = Max Non-Turbo Ratio and P(1) is computed below based on RatioStepSize.
  // 2. P(n) = Min Non-Turbo Ration, and P(n-1) = P(n) + RatioStepSize.
  //

  //
  // Fill in Turbo Mode information if enabled.
  //
  Index = 0;
  if (TurboMode) {
    //
    // If the Turbo Mode is available and enabled, then the maximum performance state is
    // represented as read from the TURBO_RATIO_LIMIT MSR bits [7:0].
    //
    CpuMiscData->FvidTable[Index].Ratio = CpuMiscData->MaxTurboRatio;
    //
    // Operating Point Protection is always enabled on Sandy Bridge processors. The end result is
    // that BIOS can not configure the processor VID.
    //
    CpuMiscData->FvidTable[Index].Vid = 0;
    //
    // Processor specific latency information from BWG.
    //
    CpuMiscData->FvidTable[Index].TransitionLatency = 10;
    CpuMiscData->FvidTable[Index].BusMasterLatency = 10;
    CpuMiscData->FvidTable[Index].Power = PackageTdp;
    Index++;
  }

  //
  // TO-DO: Get factory-configured maximum Non-Turbo Ratio if FLEX RATIO is enabled.
  //

  //
  // Fill in ratio information for non-Turbo ratios.
  //
  CpuMiscData->FvidTable[Index].Ratio = MaxRatio;
  //
  // Operating Point Protection is always enabled on Sandy Bridge processors. The end result is
  // that BIOS can not configure the processor VID.
  //
  CpuMiscData->FvidTable[Index].Vid = 0;
  //
  // Processor specific latency information from BWG.
  //
  CpuMiscData->FvidTable[Index].TransitionLatency = 10;
  CpuMiscData->FvidTable[Index].BusMasterLatency = 10;
  CpuMiscData->FvidTable[Index].Power = PackageTdp;
  Index++;

  for (TableIndex = Index; TableIndex < CpuMiscData->NumberOfPStates; TableIndex++) {
    //
    // Fill in ratio data.
    //
    ASSERT (NumGaps != 0);
    if (TurboMode) {
      CurrentRatio = MaxRatio - ((((TableIndex - 1) * RatioRange) + (NumGaps / 2)) / NumGaps);
    } else {
      CurrentRatio = MaxRatio - (((TableIndex * RatioRange) + (NumGaps / 2)) / NumGaps);
    }
    CpuMiscData->FvidTable[TableIndex].Ratio = CurrentRatio;
    //
    // Operating Point Protection is always enabled on Sandy Bridge processors. The end result is
    // that BIOS can not configure the processor VID.
    //
    CpuMiscData->FvidTable[TableIndex].Vid = 0;

    //
    // Processor specific latency information from BWG.
    //
    CpuMiscData->FvidTable[TableIndex].TransitionLatency = 10;
    CpuMiscData->FvidTable[TableIndex].BusMasterLatency = 10;

    //
    // Compute power slope.  Floating point math done using fixed point
    // computations (5 decimal places).
    //
    // M = ((1.1 - (P1_ratio - Ratio) * 0.00625) / 1.1)^2
    //
    PowerSlope = 110000 - (MultU64x32 ((MaxRatio - CurrentRatio), 625));
    PowerSlope = DivU64x32 (MultU64x32 (PowerSlope, 100), 110);
    PowerSlope = DivU64x32 (MultU64x64 (PowerSlope, PowerSlope), 100000);

    //
    // pstate_power[ratio]=((ratio/P1_ratio)*M*TDP)
    //
    RatioPower = DivU64x64Remainder (MultU64x32 (CurrentRatio, 100), MaxRatio, NULL);
    RatioPower = DivU64x32 (MultU64x64 (RatioPower, PowerSlope), 100);
    RatioPower = DivU64x32 (MultU64x64 (RatioPower, PackageTdp), 100000);
    CpuMiscData->FvidTable[TableIndex].Power = (UINTN) RatioPower;
  }

  //
  // Print debug info for FVID table.
  //
  if (ProcessorNumber == mCpuConfigLibConfigContextBuffer->BspNumber) {
    DebugOutputLevel = DEBUG_INFO;
  } else {
    DebugOutputLevel = DEBUG_VERBOSE;
  }
  DEBUG ((DebugOutputLevel, "Proc: %8d\n", ProcessorNumber));
  CpuMiscData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData;
  NumberOfPStates = CpuMiscData->NumberOfPStates;
  for (Index = 0; Index < NumberOfPStates; Index++) {
    FvidEntry = &(CpuMiscData->FvidTable[Index]);
    DEBUG ((DebugOutputLevel, "  State: %8d\n", Index));
    DEBUG ((DebugOutputLevel, "    Ratio: %8d\n", FvidEntry->Ratio));
    DEBUG ((DebugOutputLevel, "    VID:   %8d\n", FvidEntry->Vid));
    DEBUG ((DebugOutputLevel, "    Power: %8d\n", FvidEntry->Power));
  }
}

/**
  Calculate the FVID table for one logical processor based on CPU index. (Greater than 16 p-state support)

  @param[in] ProcessorNumber    The index of the CPU.
  @param[in] TurboMode          TRUE/FALSE for enable/disable.

**/
VOID
CalculateFvidTableForGreaterPstate (
  IN UINTN          ProcessorNumber,
  IN BOOLEAN        TurboMode
  )
{
  UINTN             Index;

  UINTN             MaxRatio;
  UINTN             MinRatio;
  UINTN             RatioStepSize;

  UINTN             GreaterNumberOfPStates;
  FVID_ENTRY        *FvidEntry;
  CPU_MISC_DATA     *CpuMiscData;

  UINT64            PowerSlope;
  UINT64            RatioPower;
  UINTN             CurrentRatio;
  UINTN             PackageTdp;
  UINTN             TableIndex;
  UINTN             NumGaps;
  UINTN             RatioRange;
  UINT32            DebugOutputLevel;

  NumGaps = 0;

  //
  // Get the core to bus ratio and VID range for the processor
  //
  CpuMiscData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData;
  MaxRatio    = CpuMiscData->MaxCoreToBusRatio;
  MinRatio    = CpuMiscData->MinCoreToBusRatio;
  PackageTdp  = CpuMiscData->PackageTdp;
  RatioRange  = MaxRatio - MinRatio;

  RatioStepSize = 1;

  //
  // Compute the number of P-States.
  //
  CpuMiscData->GreaterNumberOfPStates = (RatioRange / RatioStepSize) + 1;
  if (TurboMode) {
    CpuMiscData->GreaterNumberOfPStates += 1;
  }

  NumGaps = CpuMiscData->GreaterNumberOfPStates - 1;
  if (TurboMode) {
    NumGaps -= 1;
  }

  //
  // Allocate buffer to put P-State information in.
  //
  CpuMiscData->GreaterFvidTable = AllocatePool (CpuMiscData->GreaterNumberOfPStates * sizeof (FVID_ENTRY));
  ASSERT (CpuMiscData->GreaterFvidTable != NULL);
  if (CpuMiscData->GreaterFvidTable == NULL) {
    return;
  }

  //
  // According to BWG,
  // 1. If Turbo Mode is available and enabled, P(0) = Max Turbo Ratio, P(1) = Max Non-Turbo Ratio,
  //   else P(0) = Max Non-Turbo Ratio and P(1) is computed below based on RatioStepSize.
  // 2. P(n) = Min Non-Turbo Ration, and P(n-1) = P(n) + RatioStepSize.
  //

  //
  // Fill in Turbo Mode information if enabled.
  //
  Index = 0;
  if (TurboMode) {
    //
    // If the Turbo Mode is available and enabled, then the maximum performance state is
    // represented as read from the TURBO_RATIO_LIMIT MSR bits [7:0].
    //
    CpuMiscData->GreaterFvidTable[Index].Ratio = CpuMiscData->MaxTurboRatio;
    //
    // Operating Point Protection is always enabled on Sandy Bridge processors. The end result is
    // that BIOS can not configure the processor VID.
    //
    CpuMiscData->GreaterFvidTable[Index].Vid = 0;
    //
    // Processor specific latency information from BWG.
    //
    CpuMiscData->GreaterFvidTable[Index].TransitionLatency = 10;
    CpuMiscData->GreaterFvidTable[Index].BusMasterLatency = 10;
    CpuMiscData->GreaterFvidTable[Index].Power = PackageTdp;
    Index++;
  }

  //
  // TO-DO: Get factory-configured maximum Non-Turbo Ratio if FLEX RATIO is enabled.
  //

  //
  // Fill in ratio information for non-Turbo ratios.
  //
  CpuMiscData->GreaterFvidTable[Index].Ratio = MaxRatio;
  //
  // Operating Point Protection is always enabled on Sandy Bridge processors. The end result is
  // that BIOS can not configure the processor VID.
  //
  CpuMiscData->GreaterFvidTable[Index].Vid = 0;
  //
  // Processor specific latency information from BWG.
  //
  CpuMiscData->GreaterFvidTable[Index].TransitionLatency = 10;
  CpuMiscData->GreaterFvidTable[Index].BusMasterLatency = 10;
  CpuMiscData->GreaterFvidTable[Index].Power = PackageTdp;
  Index++;

  for (TableIndex = Index; TableIndex < CpuMiscData->GreaterNumberOfPStates; TableIndex++) {
    //
    // Fill in ratio data.
    //
    ASSERT (NumGaps != 0);
    if (TurboMode) {
      CurrentRatio = MaxRatio - ((((TableIndex - 1) * RatioRange) + (NumGaps / 2)) / NumGaps);
    } else {
      CurrentRatio = MaxRatio - (((TableIndex * RatioRange) + (NumGaps / 2)) / NumGaps);
    }
    CpuMiscData->GreaterFvidTable[TableIndex].Ratio = CurrentRatio;
    //
    // Operating Point Protection is always enabled on Sandy Bridge processors. The end result is
    // that BIOS can not configure the processor VID.
    //
    CpuMiscData->GreaterFvidTable[TableIndex].Vid = 0;

    //
    // Processor specific latency information from BWG.
    //
    CpuMiscData->GreaterFvidTable[TableIndex].TransitionLatency = 10;
    CpuMiscData->GreaterFvidTable[TableIndex].BusMasterLatency = 10;

    //
    // Compute power slope.  Floating point math done using fixed point
    // computations (5 decimal places).
    //
    // M = ((1.1 - (P1_ratio - Ratio) * 0.00625) / 1.1)^2
    //
    PowerSlope = 110000 - (MultU64x32 ((MaxRatio - CurrentRatio), 625));
    PowerSlope = DivU64x32 (MultU64x32 (PowerSlope, 100), 110);
    PowerSlope = DivU64x32 (MultU64x64 (PowerSlope, PowerSlope), 100000);

    //
    // pstate_power[ratio]=((ratio/P1_ratio)*M*TDP)
    //
    RatioPower = DivU64x64Remainder (MultU64x32 (CurrentRatio, 100), MaxRatio, NULL);
    RatioPower = DivU64x32 (MultU64x64 (RatioPower, PowerSlope), 100);
    RatioPower = DivU64x32 (MultU64x64 (RatioPower, PackageTdp), 100000);
    CpuMiscData->GreaterFvidTable[TableIndex].Power = (UINTN) RatioPower;
  }

  //
  // Print debug info for FVID table.
  //
  if (ProcessorNumber == mCpuConfigLibConfigContextBuffer->BspNumber) {
    DebugOutputLevel = DEBUG_INFO;
  } else {
    DebugOutputLevel = DEBUG_VERBOSE;
  }
  DEBUG ((DebugOutputLevel, "Proc: %8d\n", ProcessorNumber));
  CpuMiscData = &mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData;
  GreaterNumberOfPStates = CpuMiscData->GreaterNumberOfPStates;
  for (Index = 0; Index < GreaterNumberOfPStates; Index++) {
    FvidEntry = &(CpuMiscData->GreaterFvidTable[Index]);
    DEBUG ((DebugOutputLevel, "  State: %8d\n", Index));
    DEBUG ((DebugOutputLevel, "    Ratio: %8d\n", FvidEntry->Ratio));
    DEBUG ((DebugOutputLevel, "    VID:   %8d\n", FvidEntry->Vid));
    DEBUG ((DebugOutputLevel, "    Power: %8d\n", FvidEntry->Power));
  }
}

/**
  Create FVID table for EIST.

**/
VOID
CreateFvidTable (
  VOID
  )
{
  UINTN                         ProcessorNumber;
  EFI_CPUID_REGISTER            *CpuidRegister;
  MSR_MISC_ENABLES_REGISTER     MiscEnablesMsr;
  BOOLEAN                       TurboSupported;
  BOOLEAN                       TurboSetting;

  for (ProcessorNumber = 0; ProcessorNumber < mCpuConfigLibConfigContextBuffer->NumberOfProcessors; ProcessorNumber++) {
    CpuidRegister = GetProcessorCpuid (ProcessorNumber, CPUID_VERSION_INFO);
    if (((CpuidRegister != NULL) && (((CPUID_VERSION_INFO_ECX *) &CpuidRegister->RegEcx)->Bits.EIST == 1)) &&
        mCpuPolicyConfiguration->Policy.CpuEistEnable) {
      //
      // EIST is supported and user wish to enable it.
      //
      CpuidRegister = GetProcessorCpuid (ProcessorNumber, CPUID_THERMAL_POWER_MANAGEMENT);
      MiscEnablesMsr.Uint64 = mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[ProcessorNumber].CpuMiscData.MiscEnablesMsr;
      //
      //  +----------------------------------------------------------------------------+
      //  | CPUID.(EAX=06):EAX[1] | MSR_MISC_ENABLES.Bits[38]    | Turbo Mode          |
      //  +-----------------------|------------------------------|---------------------+
      //  |          0            |       0                      |Not Available        |
      //  +-----------------------|------------------------------|---------------------+
      //  |          0            |       1                      |Available but hidden |
      //  +-----------------------|------------------------------|---------------------+
      //  |          1            |       0                      |Available and visible|
      //  +-----------------------|------------------------------|---------------------+
      //
      TurboSupported = (((CpuidRegister != NULL) && (((CPUID_THERMAL_POWER_MANAGEMENT_EAX *) &CpuidRegister->RegEax)->Bits.TurboBoostTechnology != 0)) ||
                        (MiscEnablesMsr.Bits.TurboModeDisable != 0));
      if (TurboSupported && mCpuPolicyConfiguration->Policy.CpuTurboModeEnable) {
        //
        // Turbo Mode is supported and user wish to enable it.
        //
        TurboSetting = TRUE;
      } else {
        TurboSetting = FALSE;
      }
      //
      // Calculate the FVID table for one logical processor.
      //
      CalculateFvidTable (ProcessorNumber, TurboSetting);
      CalculateFvidTableForGreaterPstate (ProcessorNumber, TurboSetting);
    }
  }
}

/**
  Collects data from all logical processors.

  This function collects data from all logical processors.

**/
VOID
DataCollectionPhase (
  VOID
  )
{
  EFI_STATUS                Status;

  //
  // Check the number of CPUID leafs of all logical processors, and allocate memory for them.
  //
  AllocateMemoryForCpuidLeafs ();

  //
  // Wakeup all APs for data collection.
  //
  Status = mMpServices->StartupAllAPs (
                 mMpServices,
                 CollectProcessorData,
                 FALSE,
                 NULL,
                 0,
                 NULL,
                 NULL
                 );
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Error: Not able to StartupAllAPs.  Status: %r\n", Status));
  }

  //
  // Collect data for BSP.
  //
  CollectProcessorData (NULL);

  //
  // Create FVID table for EIST.
  //
  CreateFvidTable ();
}
