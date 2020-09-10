/** @file
  Code to record cache subclass data with Smbios protocol.

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

#include "Processor.h"
#include "Cache.h"
#include "CpuSmbios.h"
#include <Library/PrintLib.h>

CPU_CACHE_CONVERTER mCacheConverter[] = {
  {
    1,
    0x06,
    8,
    CacheAssociativity4Way,
    CacheTypeInstruction
  },
  {
    1,
    0x08,
    16,
    CacheAssociativity4Way,
    CacheTypeInstruction
  },
  {
    1,
    0x0A,
    8,
    CacheAssociativity2Way,
    CacheTypeData
  },
  {
    1,
    0x0C,
    16,
    CacheAssociativity4Way,
    CacheTypeData
  },
  {
    3,
    0x22,
    512,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    3,
    0x23,
    1024,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    3,
    0x25,
    2048,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    3,
    0x29,
    4096,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    1,
    0x2C,
    32,
    CacheAssociativity8Way,
    CacheTypeData
  },
  {
    1,
    0x30,
    32,
    CacheAssociativity8Way,
    CacheTypeInstruction
  },
  {
    2,
    0x39,
    128,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x3B,
    128,
    CacheAssociativity2Way,
    CacheTypeUnified
  },
  {
    2,
    0x3C,
    256,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x41,
    128,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x42,
    256,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x43,
    512,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x44,
    1024,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x45,
    2048,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x49,
    4096,
    CacheAssociativity16Way,
    CacheTypeUnified
  },
  {
    1,
    0x60,
    16,
    CacheAssociativity8Way,
    CacheTypeData
  },
  {
    1,
    0x66,
    8,
    CacheAssociativity4Way,
    CacheTypeData
  },
  {
    1,
    0x67,
    16,
    CacheAssociativity4Way,
    CacheTypeData
  },
  {
    1,
    0x68,
    32,
    CacheAssociativity4Way,
    CacheTypeData
  },
  {
    2,
    0x78,
    1024,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x79,
    128,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x7A,
    256,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x7B,
    512,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x7C,
    1024,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x7D,
    2048,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x7F,
    512,
    CacheAssociativity2Way,
    CacheTypeUnified
  },
  {
    2,
    0x82,
    256,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x83,
    512,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x84,
    1024,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x85,
    2048,
    CacheAssociativity8Way,
    CacheTypeUnified
  },
  {
    2,
    0x86,
    512,
    CacheAssociativity4Way,
    CacheTypeUnified
  },
  {
    2,
    0x87,
    1024,
    CacheAssociativity8Way,
    CacheTypeUnified
  }
};

/**
  Get number of CPU modules using CPUID

  @return Number of CPU Modules        Return the number of CPU modules in the SoC.

**/
UINT8
GetNumberOfCPUModules (
  VOID
  )
{
  EFI_CPUID_REGISTER        *CpuidRegister;
  UINT8                     ModuleID;
  UINT8                     ModuleIndex;
  UINT8                     ModuleBitMap = 0;
  UINT8                     ApicID;
  UINT8                     NumberModules = 0;
  UINTN                     ProcessorIndex;

  //
  // Get number of CPU Modules decoding the APICID for each core
  //
  for (ProcessorIndex = 0; ProcessorIndex < mCpuConfigLibConfigContextBuffer->NumberOfProcessors; ProcessorIndex++) {

    CpuidRegister = GetProcessorCpuid (ProcessorIndex, CPUID_VERSION_INFO);
    if (CpuidRegister == NULL) {
      return 0;
    }

    // Get APICID - CPUID[31:24] 
    ApicID = (UINT8) (CpuidRegister->RegEbx>>24);
    // All CPUs in the same Tremont Module has the same value in bits [7:3]
    ModuleID = ApicID >> 3;
    // ModuleBitMap - each bit reperesent a CPU Module
    ModuleBitMap = ModuleBitMap | BIT0 << ModuleID;

  }

  //
  // Counting the number of CPU Modules
  //
  for (ModuleIndex = 0; ModuleIndex < SNR_MAX_TMT_MODULES; ModuleIndex++) {
    if (ModuleBitMap & BIT0) {
      NumberModules++;
    }
    ModuleBitMap = ModuleBitMap >> 1;
  }

  return NumberModules;
}

/**
  Get cache data from CPUID EAX = 2.

  @param[in]    ProcessorNumber     Processor number of specified processor.
  @param[out]   CacheData           Pointer to the cache data gotten from CPUID EAX = 2.

**/
VOID
GetCacheDataFromCpuid2 (
  IN UINTN              ProcessorNumber,
  OUT CPU_CACHE_DATA    *CacheData
  )
{
  UINT8                     CacheLevel;
  EFI_CPUID_REGISTER        *CacheInformation;
  UINTN                     CacheDescriptorNum;
  UINT32                    RegPointer[4];
  UINT8                     RegIndex;
  UINT32                    RegValue;
  UINT8                     ByteIndex;
  UINT8                     Descriptor;
  UINTN                     DescriptorIndex;

  DEBUG ((EFI_D_INFO, "Get cache data from CPUID EAX = 2\n"));

  CacheDescriptorNum = (UINTN) (sizeof (mCacheConverter) / sizeof (mCacheConverter[0]));
  CacheInformation = GetProcessorCpuid (ProcessorNumber, CPUID_CACHE_INFO);
  if (CacheInformation == NULL) {
    return;
  }

  CopyMem (RegPointer, CacheInformation, sizeof (EFI_CPUID_REGISTER));
  RegPointer[0] &= 0xFFFFFF00;

  for (RegIndex = 0; RegIndex < 4; RegIndex++) {
    RegValue = RegPointer[RegIndex];
    //
    // The most significant bit (bit 31) of each register indicates whether the register
    // contains valid information (set to 0) or is reserved (set to 1).
    //
    if ((RegValue & BIT31) != 0) {
      continue;
    }

    for (ByteIndex = 0; ByteIndex < 4; ByteIndex++) {
      Descriptor = (UINT8) ((RegValue >> (ByteIndex * 8)) & 0xFF);
      for (DescriptorIndex = 0; DescriptorIndex < CacheDescriptorNum; DescriptorIndex++) {
        if (mCacheConverter[DescriptorIndex].CacheDescriptor == Descriptor) {
          CacheLevel = mCacheConverter[DescriptorIndex].CacheLevel; // 1 based
          ASSERT (CacheLevel >= 1 && CacheLevel <= CPU_CACHE_LMAX);
          /**
          Since L1 cache is shared, add an entry to CacheData to distinguish Data cache vs. Instruction cache.
          CacheData[0] = L1 Data Cache
          CacheData[1] = L1 Instruction Cache
          CacheData[2] = L2 Unified Cache
          CacheData[3] = L3 Unified Cache
          CacheLevel 0 = L1 Data Cache
          CacheLevel 1 = L1 Instruction Cache
          **/
          if (CacheLevel == 1 && mCacheConverter[DescriptorIndex].SystemCacheType == 4) {
            CacheData[CacheLevel - 1].CacheSizeinKB = CacheData[CacheLevel - 1].CacheSizeinKB + mCacheConverter[DescriptorIndex].CacheSizeinKB;
            CacheData[CacheLevel - 1].SystemCacheType = mCacheConverter[DescriptorIndex].SystemCacheType;
            CacheData[CacheLevel - 1].Associativity = mCacheConverter[DescriptorIndex].Associativity;
          } else {
            CacheData[CacheLevel].CacheSizeinKB = CacheData[CacheLevel].CacheSizeinKB + mCacheConverter[DescriptorIndex].CacheSizeinKB;
            CacheData[CacheLevel].SystemCacheType = mCacheConverter[DescriptorIndex].SystemCacheType;
            CacheData[CacheLevel].Associativity = mCacheConverter[DescriptorIndex].Associativity;
          }
          DEBUG ((
            EFI_D_INFO,
            "CacheData: CacheLevel = 0x%x CacheSizeinKB = 0x%xKB SystemCacheType = 0x%x Associativity = 0x%x\n",
            CacheLevel,
            CacheData[CacheLevel].CacheSizeinKB,
            CacheData[CacheLevel].SystemCacheType,
            CacheData[CacheLevel].Associativity
            ));
        }
      }
    }
  }
}

/**
  Get cache data from CPUID EAX = 4.

  CPUID EAX = 4 is Deterministic Cache Parameters Leaf.

  @param[in]    ProcessorNumber     Processor number of specified processor.
  @param[in]    CoreCount           Number of Cores per Package.
  @param[in]    DieCount            Number of Dies per Package.
  @param[out]   CacheData           Pointer to the cache data gotten from CPUID EAX = 4.

**/
VOID
GetCacheDataFromCpuid4 (
  IN UINTN              ProcessorNumber,
  IN UINTN              CoreCount,
  IN UINTN              DieCount,
  OUT CPU_CACHE_DATA    *CacheData
  )
{
  EFI_CPUID_REGISTER            *CpuidRegisters;
  UINT8                         Index;
  UINT8                         NumberOfDeterministicCacheParameters;
  UINT32                        Ways;
  UINT32                        Partitions;
  UINT32                        LineSize;
  UINT32                        Sets;
  CACHE_TYPE_DATA               SystemCacheType;
  CACHE_ASSOCIATIVITY_DATA      Associativity;
  UINT8                         CacheLevel;
  UINT32                        FamilyId;
  UINT32                        ModelId;

  DEBUG ((EFI_D_INFO, "Get cache data from CPUID EAX = 4\n"));

  NumberOfDeterministicCacheParameters = (UINT8) GetNumberOfCpuidLeafs (ProcessorNumber, DeterministicCacheParametersCpuidLeafs);

  for (Index = 0; Index < NumberOfDeterministicCacheParameters; Index++) {
    CpuidRegisters = GetDeterministicCacheParametersCpuidLeaf (ProcessorNumber, Index);
    if (CpuidRegisters == NULL) {
      return;
    }
    if ((CpuidRegisters->RegEax & CPU_CACHE_TYPE_MASK) == 0) {
      continue;
    }

    switch (CpuidRegisters->RegEax & CPU_CACHE_TYPE_MASK) {
      case 1:
        SystemCacheType = CacheTypeData;
        break;
      case 2:
        SystemCacheType = CacheTypeInstruction;
        break;
      case 3:
        SystemCacheType = CacheTypeUnified;
        break;
      default:
        SystemCacheType = CacheTypeUnknown;
    }

    Ways = ((CpuidRegisters->RegEbx & CPU_CACHE_WAYS_MASK) >> CPU_CACHE_WAYS_SHIFT) + 1;
    Partitions = ((CpuidRegisters->RegEbx & CPU_CACHE_PARTITIONS_MASK) >> CPU_CACHE_PARTITIONS_SHIFT) + 1;
    LineSize = (CpuidRegisters->RegEbx & CPU_CACHE_LINESIZE_MASK) + 1;
    Sets = CpuidRegisters->RegEcx + 1;

    switch (Ways) {
      case 2:
        Associativity = CacheAssociativity2Way;
        break;
      case 4:
        Associativity = CacheAssociativity4Way;
        break;
      case 8:
        Associativity = CacheAssociativity8Way;
        break;
      case 12:
        Associativity = CacheAssociativity12Way;
        break;
      case 16:
        Associativity = CacheAssociativity16Way;
        break;
      case 20:
        Associativity = CacheAssociativity20Way;
        break;
      case 24:
        Associativity = CacheAssociativity24Way;
        break;
      case 32:
        Associativity = CacheAssociativity32Way;
        break;
      case 48:
        Associativity = CacheAssociativity48Way;
        break;
      case 64:
        Associativity = CacheAssociativity64Way;
        break;
      default:
        Associativity = CacheAssociativityFully;
        break;
    }

    CacheLevel = (UINT8) ((CpuidRegisters->RegEax & CPU_CACHE_LEVEL_MASK) >> CPU_CACHE_LEVEL_SHIFT); // 1 based
    ASSERT (CacheLevel >= 1 && CacheLevel <= CPU_CACHE_LMAX);

    /**
      Since L1 cache contains Data cache and Instruction cache, add an entry to CacheData to distinguish Data cache vs. Instruction cache.
      CacheData[0] = L1 Data Cache
      CacheData[1] = L1 Instruction Cache
      CacheData[2] = L2 Unified Cache
      CacheData[3] = L3 Unified Cache
      CacheLevel 0 = L1 Data Cache
      CacheLevel 1 = L1 Instruction Cache
    **/
    if (CacheLevel == 1 && SystemCacheType == CacheTypeData) {
      CacheData[CacheLevel - 1].CacheSizeinKB = CacheData[CacheLevel - 1].CacheSizeinKB + (Ways * Partitions * LineSize * Sets) / 1024;
      CacheData[CacheLevel - 1].SystemCacheType = SystemCacheType;
      CacheData[CacheLevel - 1].Associativity = Associativity;
    } else {
      CacheData[CacheLevel].CacheSizeinKB = CacheData[CacheLevel].CacheSizeinKB + (Ways * Partitions * LineSize * Sets) / 1024;
      CacheData[CacheLevel].SystemCacheType = SystemCacheType;
      CacheData[CacheLevel].Associativity = Associativity;
    }
  }

  GetProcessorVersionInfo (ProcessorNumber, &FamilyId, &ModelId, NULL, NULL);
  for (CacheLevel = 0; CacheLevel <= CPU_CACHE_L2; CacheLevel++) {
    //
    // Per BWG for CPUID Function 04h, cache size should be "multiply the L1/L2/L3 Unified size by the number of cores
    // in the physical package divided by the number of cores sharing this cache."
    // The number of cores sharing L1 and L2 level cache is 1.
    // So for L1 and L2 cache size, multiply the L1/L2 Unified size by the number of cores.

    //
    if (FeaturePcdGet (PcdCpuSnowridgeFamilyFlag) &&
       IS_SNOWRIDGE_SERVER_PROC (FamilyId, ModelId) &&
       (CacheLevel == CPU_CACHE_L2 ) ) {
      CacheData[CacheLevel].CacheSizeinKB = CacheData[CacheLevel].CacheSizeinKB * GetNumberOfCPUModules();
    } else {
      CacheData[CacheLevel].CacheSizeinKB = (UINT32)(CacheData[CacheLevel].CacheSizeinKB * CoreCount);
    }
  }

  //
  // L3 level cache is shared by all cores in one die (multiple-dies package case) or one package
  // (one-die package case). So for L3 cache size per package, multiply the L3 Unified size by
  // DieCount.
  //
  CacheData[3].CacheSizeinKB = (UINT32)(CacheData[3].CacheSizeinKB * DieCount);
}

/**
  Add Type 7 SMBIOS Record for Cache Information.

  @param[in]    ProcessorNumber     Processor number of specified processor.
  @param[in]    CoreCount           Number of cores of the specified package.
  @param[in]    DieCount            Number of dies of the specified package.
  @param[out]   L1CacheHandle       Pointer to the handle of the L1 Cache SMBIOS record.
  @param[out]   L2CacheHandle       Pointer to the handle of the L2 Cache SMBIOS record.
  @param[out]   L3CacheHandle       Pointer to the handle of the L3 Cache SMBIOS record.

**/
VOID
AddSmbiosCacheTypeTable (
  IN UINTN              ProcessorNumber,
  IN UINTN              CoreCount,
  IN UINTN              DieCount,
  OUT EFI_SMBIOS_HANDLE *L1CacheHandle,
  OUT EFI_SMBIOS_HANDLE *L2CacheHandle,
  OUT EFI_SMBIOS_HANDLE *L3CacheHandle
  )
{
  EFI_STATUS                    Status;
  SMBIOS_TABLE_TYPE7            *SmbiosRecord;
  EFI_SMBIOS_HANDLE             SmbiosHandle;
  UINT8                         CacheLevel;
  CPU_CACHE_DATA                CacheData[CPU_CACHE_LMAX];
  CHAR8                         *OptionalStrStart;
  UINTN                         StringBufferSize;
  UINTN                         CacheSocketStrLen;
  EFI_STRING                    CacheSocketStr;
  CACHE_SRAM_TYPE_DATA          CacheSramType;
  CPU_CACHE_CONFIGURATION_DATA  CacheConfig;
  UINT32                        TCacheSize;

  TCacheSize = 0;
  ZeroMem (CacheData, CPU_CACHE_LMAX * sizeof (CPU_CACHE_DATA));

  //
  // Check whether the CPU supports CPUID EAX = 4, if yes, get cache data from CPUID EAX = 4,
  // or no, get cache data from CPUID EAX = 2 to be compatible with the earlier CPU.
  //
  if (GetNumberOfCpuidLeafs (ProcessorNumber, BasicCpuidLeaf) > 4 ) {
    GetCacheDataFromCpuid4 (ProcessorNumber, CoreCount, DieCount, CacheData);
  } else {
    GetCacheDataFromCpuid2 (ProcessorNumber, CacheData);
  }

  //
  // Now cache data has been ready.
  //
  for (CacheLevel = 1; CacheLevel < CPU_CACHE_LMAX; CacheLevel++) {
    //
    // NO smbios record for zero-sized cache.
    //
    if (CacheData[CacheLevel].CacheSizeinKB == 0) {
      continue;
    }

    if (CacheLevel == 1) {
      //
      // CPUID returns deterministic cache values for L1 data and L1 instruction cache.  These values
      // are stored in CacheData[0] and CacheData[1] respectively.  Because these are the same level
      // cache, they should be combined into a single SmBios Record with the SystemCacheType
      // specificed as "CacheTypeUnified"
      //
      DEBUG ((
        EFI_D_INFO,
        "CacheData: CacheLevel = 0x%x CacheSizeinKB = %dKB SystemCacheType = 0x%x Associativity = 0x%x\n",
        CacheLevel,
        CacheData[0].CacheSizeinKB,  // Total size of L1 for this cpu.
        CacheTypeUnified, // Combined L1 Data and L1 Instruction
        CacheData[CacheLevel].Associativity // Common between both Data and Instruction cache.
        ));
    }
    DEBUG ((
      EFI_D_INFO,
      "CacheData: CacheLevel = 0x%x CacheSizeinKB = %dKB SystemCacheType = 0x%x Associativity = 0x%x\n",
      CacheLevel,
      CacheData[CacheLevel].CacheSizeinKB,
      CacheData[CacheLevel].SystemCacheType,
      CacheData[CacheLevel].Associativity
      ));

    StringBufferSize = sizeof (CHAR16) * SMBIOS_STRING_MAX_LENGTH;
    CacheSocketStr = AllocateZeroPool (StringBufferSize);
    ASSERT (CacheSocketStr != NULL);
    CacheSocketStrLen = UnicodeSPrint (CacheSocketStr, StringBufferSize, L"L%x-Cache", CacheLevel);

    //
    // Report Cache Information to Type 7 SMBIOS Record.
    //
    SmbiosRecord = AllocatePool (sizeof (SMBIOS_TABLE_TYPE7) + CacheSocketStrLen + 1 + 1);
    if (SmbiosRecord == NULL) {
      DEBUG ((DEBUG_ERROR, "AllocateZeroPool Returned NULL pointer\n"));
      ASSERT (SmbiosRecord != NULL);
      return;
    }

    ZeroMem (SmbiosRecord, sizeof (SMBIOS_TABLE_TYPE7) + CacheSocketStrLen + 1 + 1);
    SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_CACHE_INFORMATION;
    SmbiosRecord->Hdr.Length = (UINT8) sizeof (SMBIOS_TABLE_TYPE7);
    //
    // Make handle chosen by smbios protocol.add automatically.
    //
    SmbiosRecord->Hdr.Handle = 0;
    //
    // Socket will be the 1st optional string following the formatted structure.
    //
    SmbiosRecord->SocketDesignation = 1;

    //
    // Cache Level - 1 through 8, e.g. an L1 cache would use value 000b and an L3 cache would use 010b.
    // Convert 1-based to 0-based before writing CacheConfig.Level field
    //
    CacheConfig.Level = CacheLevel - 1;

    CacheConfig.Socketed = 0;           // Not Socketed
    CacheConfig.Reserved2 = 0;
    CacheConfig.Location = 0;           // Internal Cache
    CacheConfig.Enable = 1;             // Cache enabled

    if (CacheLevel == 1) {
       CacheConfig.OperationalMode = 1;    // Write Back(L1 cache)
    } else {
       CacheConfig.OperationalMode = 2;    // Varies with Memory Address(L2 and L3 cache)
    }
    CacheConfig.Reserved1 = 0;
    CopyMem (&SmbiosRecord->CacheConfiguration, &CacheConfig, 2);
    //
    // Only 1K granularity assumed here.
    //
    if (CacheLevel == 1) {
      // Combined L1 Data and L1 Instruction cache sizes.
      TCacheSize = CacheData[CacheLevel].CacheSizeinKB + CacheData[0].CacheSizeinKB;
    } else {
      TCacheSize = CacheData[CacheLevel].CacheSizeinKB;
    }
    //
    // Check to see if Cache size is 2047 MB or smaller
    //
    if ((TCacheSize >> 31) == 0) {
      //
      //64K Granularity considered.
      //
      if((TCacheSize >> 15) == 0){
          SmbiosRecord->MaximumCacheSize = (UINT16)TCacheSize;
          SmbiosRecord->InstalledSize = (UINT16)TCacheSize;
          SmbiosRecord->MaximumCacheSize2 = (UINT32)TCacheSize;
          SmbiosRecord->InstalledSize2 = (UINT32)TCacheSize;
      } else { // Use 64K Granularity if cache size is too large
          SmbiosRecord->MaximumCacheSize = (UINT16)(TCacheSize >> 6);
          SmbiosRecord->MaximumCacheSize |= BIT15;
          SmbiosRecord->InstalledSize = SmbiosRecord->MaximumCacheSize;
          SmbiosRecord->MaximumCacheSize2 = (UINT32)(TCacheSize >> 6);
          SmbiosRecord->MaximumCacheSize2 |= BIT31;
          SmbiosRecord->InstalledSize2 = SmbiosRecord->MaximumCacheSize2;
      }
    //
    // Cache size is bigger than 2047 MB
    //
    } else {
      SmbiosRecord->MaximumCacheSize = (UINT16)(-1);
      SmbiosRecord->InstalledSize = (UINT16)(-1);
      SmbiosRecord->MaximumCacheSize2 = (TCacheSize >> 6);
      SmbiosRecord->MaximumCacheSize2 |= BIT31;
      SmbiosRecord->InstalledSize2 = SmbiosRecord->MaximumCacheSize2;
    }

    ZeroMem (&CacheSramType, sizeof (CACHE_SRAM_TYPE_DATA));
    CacheSramType.Synchronous = 1;
    CopyMem (&SmbiosRecord->SupportedSRAMType, &CacheSramType, 2);
    CopyMem (&SmbiosRecord->CurrentSRAMType, &CacheSramType, 2);

    SmbiosRecord->CacheSpeed = 0;
    SmbiosRecord->ErrorCorrectionType = CacheErrorSingleBit;
    if (CacheLevel == 1) {
      SmbiosRecord->SystemCacheType = (UINT8) CacheTypeUnified;
    } else {
      SmbiosRecord->SystemCacheType = (UINT8) CacheData[CacheLevel].SystemCacheType;
    }
    SmbiosRecord->Associativity = (UINT8) CacheData[CacheLevel].Associativity;

    OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);
    UnicodeStrToAsciiStrS (CacheSocketStr, OptionalStrStart, CacheSocketStrLen + 1);

    //
    // Now we have got the full smbios record, call smbios protocol to add this record.
    //
    SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    Status = mSmbios->Add (mSmbios, NULL, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER*) SmbiosRecord);

    //
    // Record L1/L2/L3 Cache Smbios Handle, Type 4 SMBIOS Record needs it.
    // CacheData[0] = L1 Data Cache and CacheData[1] = L1 Instruction Cache.
    // Both Cachelevel == 0 and Cachelevel == 1 indicate Level 1 cache.
    if (CacheLevel == CPU_CACHE_L1 - 1 || CacheLevel == (CPU_CACHE_L1)) {
      *L1CacheHandle = SmbiosHandle;
    } else if (CacheLevel == CPU_CACHE_L2) {
      *L2CacheHandle = SmbiosHandle;
    } else if (CacheLevel == CPU_CACHE_L3) {
      *L3CacheHandle  = SmbiosHandle;
    }
    FreePool (SmbiosRecord);
    FreePool (CacheSocketStr);
    ASSERT_EFI_ERROR (Status);
  }
}

