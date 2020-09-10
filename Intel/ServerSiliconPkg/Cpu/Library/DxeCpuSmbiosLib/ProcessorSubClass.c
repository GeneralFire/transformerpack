/** @file
  Code to log processor subclass data with Smbios protocol.

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

#include "CpuSmbios.h"
#include "Processor.h"

UINTN mCpuSocketStrNumber = 1;
UINTN mCpuAssetTagStrNumber = 4;

//
// Below enum value will be moved to PROCESSOR_UPGRADE enum in MdePkg/Include/IndustryStandard/SmBios.h
//  after the SMBIOS spec is updated to include the LGA4189 socket type (which will happen in SMBIOS v3.4).
//
enum {ProcessorUpgradeSocketLGA4189 = 0x3D};

/**
  Add Type 4 SMBIOS Record for Socket Unpopulated.
**/
VOID
AddUnpopulatedSmbiosProcessorTypeTable (
  VOID
  )
{
  EFI_STATUS            Status;
  EFI_SMBIOS_HANDLE     SmbiosHandle;
  UINTN                 TotalSize;
  SMBIOS_TABLE_TYPE4    *SmbiosRecord;
  CHAR8                 *OptionalStrStart;
  EFI_STRING_ID         Token;
  EFI_STRING            CpuSocketStr;
  UINTN                 CpuSocketStrLen;
  UINT32                FamilyId;
  UINT32                ModelId;

  //
  // Get CPU Socket string, it will be updated when PcdPlatformCpuSocketNames is set.
  //
  Token = STRING_TOKEN (STR_UNKNOWN);
  CpuSocketStr = HiiGetPackageString (&gEfiCallerIdGuid, Token ,NULL);
  if (CpuSocketStr == NULL) {
    ASSERT (CpuSocketStr != NULL);
    return;
  }
  CpuSocketStrLen = StrLen (CpuSocketStr);
  ASSERT (CpuSocketStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Report Processor Information to Type 4 SMBIOS Record.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE4) + CpuSocketStrLen + 1 + 1;
  SmbiosRecord = AllocatePool (TotalSize);
  if (SmbiosRecord == NULL) {
    ASSERT (SmbiosRecord != NULL);
    return;
  }
  ZeroMem (SmbiosRecord, TotalSize);

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION;
  SmbiosRecord->Hdr.Length = (UINT8) sizeof (SMBIOS_TABLE_TYPE4);

  //
  // Make handle chosen by smbios protocol.add automatically.
  //
  SmbiosRecord->Hdr.Handle = 0;

  //
  // Socket will be the 1st optional string following the formatted structure.
  //
  SmbiosRecord->Socket = (UINT8) mCpuSocketStrNumber;
  SmbiosRecord->ProcessorType = CentralProcessor;

  //
  // Just indicate CPU Socket Unpopulated.
  // CPU_PROCESSOR_STATUS_DATA.SocketPopulated: 1- CPU Socket populated and 0 - CPU Socket Unpopulated
  //
  SmbiosRecord->Status &= ~SMBIOS_TYPE4_CPU_SOCKET_POPULATED;
  SmbiosRecord->MaxSpeed = (UINT16) PcdGet32 (PcdPlatformCpuMaxCoreFrequency);
  GetProcessorVersionInfo (0, &FamilyId, &ModelId, NULL, NULL);

  //
  // The upgrade depends on processor family id and model id.
  //
  if ((FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (FamilyId, ModelId)) ||
      (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_D_SERVER_PROC (FamilyId, ModelId)) ||
      (FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) && IS_SAPPHIRERAPIDS_SERVER_PROC (FamilyId, ModelId)) ||
      (FeaturePcdGet (PcdCpuSnowridgeFamilyFlag) && IS_SNOWRIDGE_SERVER_PROC (FamilyId, ModelId))) {
    SmbiosRecord->ProcessorUpgrade = ProcessorUpgradeSocketLGA3647_1;
  } else if (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_SERVER_PROC (FamilyId, ModelId)) {
    SmbiosRecord->ProcessorUpgrade = ProcessorUpgradeSocketLGA4189;
  } else {
    SmbiosRecord->ProcessorUpgrade = ProcessorUpgradeSocketLGA775;
  }

  SmbiosRecord->L1CacheHandle = 0xFFFF;
  SmbiosRecord->L2CacheHandle = 0xFFFF;
  SmbiosRecord->L3CacheHandle = 0xFFFF;

  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);
  UnicodeStrToAsciiStrS (CpuSocketStr, OptionalStrStart, CpuSocketStrLen + 1);
  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = mSmbios->Add (mSmbios, NULL, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
  FreePool (SmbiosRecord);
  FreePool (CpuSocketStr);
  ASSERT_EFI_ERROR (Status);
}

/**
  Add Processor Information to Type 4 SMBIOS Record for Socket Populated.

  @param[in]    ProcessorNumber     Processor number of specified processor.
  @param[in]    CoreCount           Number of cores of the specified package.
  @param[in]    ThreadCount         Number of threads of the specified package.
  @param[in]    L1CacheHandle       The handle of the L1 Cache SMBIOS record.
  @param[in]    L2CacheHandle       The handle of the L2 Cache SMBIOS record.
  @param[in]    L3CacheHandle       The handle of the L3 Cache SMBIOS record.

**/
VOID
AddSmbiosProcessorTypeTable (
  IN UINTN              ProcessorNumber,
  IN UINTN              CoreCount,
  IN UINTN              ThreadCount,
  IN EFI_SMBIOS_HANDLE  L1CacheHandle,
  IN EFI_SMBIOS_HANDLE  L2CacheHandle,
  IN EFI_SMBIOS_HANDLE  L3CacheHandle
  )
{
  EFI_STATUS                    Status;
  EFI_SMBIOS_HANDLE             SmbiosHandle;
  UINTN                         TotalSize;
  EFI_STRING_ID                 Token;
  CHAR8                         *OptionalStrStart;
  EFI_STRING                    CpuManuStr;
  EFI_STRING                    CpuVerStr;
  EFI_STRING                    CpuSocketStr;
  EFI_STRING                    CpuAssetTagStr;
  UINTN                         CpuManuStrLen;
  UINTN                         CpuVerStrLen;
  UINTN                         CpuSocketStrLen;
  UINTN                         CpuAssetTagStrLen;
  SMBIOS_TABLE_TYPE4            *SmbiosRecord;
  EFI_CPUID_REGISTER            *CpuidRegister;
  UINT16                        ProcessorVoltage;
  CPU_PROCESSOR_VERSION_INFORMATION     Version;
  CPU_PROCESSOR_STATUS_DATA     ProcessorStatus;
  CPU_PROCESSOR_CHARACTERISTICS_DATA    ProcessorCharacteristics;
  UINT32                        FamilyId;
  UINT32                        ModelId;
  UINT64                        MsrValue;

  ZeroMem (&ProcessorCharacteristics, sizeof (CPU_PROCESSOR_CHARACTERISTICS_DATA));

  //
  // Get CPU Socket string, it will be updated when PcdPlatformCpuSocketNames is set.
  //
  Token = STRING_TOKEN (STR_UNKNOWN);
  CpuSocketStr = HiiGetPackageString (&gEfiCallerIdGuid, Token ,NULL);
  if (CpuSocketStr == NULL) {
    ASSERT (CpuSocketStr != NULL);
    return;
  }
  CpuSocketStrLen = StrLen (CpuSocketStr);
  ASSERT (CpuSocketStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get CPU Manufacture string.
  //
  Token = GetProcessorManufacturer (ProcessorNumber);
  CpuManuStr = HiiGetPackageString (&gEfiCallerIdGuid, Token, NULL);
  if (CpuManuStr == NULL) {
    ASSERT (CpuManuStr != NULL);
    return;
  }
  CpuManuStrLen = StrLen (CpuManuStr);
  ASSERT (CpuManuStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get CPU Version string.
  //
  GetProcessorVersion (ProcessorNumber, &Version);
  if (Version.StringValid) {
    Token = HiiSetString (mStringHandle, 0, Version.BrandString, NULL);
    if (Token == 0) {
      Token = Version.StringRef;
    }
  } else {
    Token = Version.StringRef;
  }
  CpuVerStr = HiiGetPackageString (&gEfiCallerIdGuid, Token, NULL);
  if (CpuVerStr == NULL) {
    ASSERT (CpuVerStr != NULL);
    return;
  }
  CpuVerStrLen = StrLen (CpuVerStr);
  ASSERT (CpuVerStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Get CPU Asset Tag string, it will be updated when PcdPlatformCpuAssetTags is set.
  //
  Token = STRING_TOKEN (STR_UNKNOWN);
  CpuAssetTagStr = HiiGetPackageString (&gEfiCallerIdGuid, Token ,NULL);
  if (CpuAssetTagStr == NULL) {
    ASSERT (CpuAssetTagStr != NULL);
    return;
  }
  CpuAssetTagStrLen = StrLen (CpuAssetTagStr);
  ASSERT (CpuAssetTagStrLen <= SMBIOS_STRING_MAX_LENGTH);

  //
  // Report Processor Information to Type 4 SMBIOS Record.
  //
  TotalSize = sizeof (SMBIOS_TABLE_TYPE4) + CpuSocketStrLen + 1 + CpuManuStrLen + 1 + CpuVerStrLen + 1 + CpuAssetTagStrLen + 1 + 1;
  SmbiosRecord = AllocatePool (TotalSize);
  if (SmbiosRecord == NULL) {
    ASSERT (SmbiosRecord != NULL);
    return;
  }
  ZeroMem (SmbiosRecord, TotalSize);

  SmbiosRecord->Hdr.Type = EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION;
  SmbiosRecord->Hdr.Length = (UINT8) sizeof (SMBIOS_TABLE_TYPE4);

  //
  // Make handle chosen by smbios protocol.add automatically.
  //
  SmbiosRecord->Hdr.Handle = 0;

  //
  //set STRING index explicitly (BEGIN).
  //

  //
  // Socket will be the 1st optional string following the formatted structure.
  //
  SmbiosRecord->Socket = (UINT8) mCpuSocketStrNumber;
  //
  // Manu will be the 2nd optional string following the formatted structure.
  //
  SmbiosRecord->ProcessorManufacture = 2;
  //
  // Version will be the 3rd optional string following the formatted structure.
  //
  SmbiosRecord->ProcessorVersion = 3;
  //
  // Serial Num will is NOT currently set.
  //
  SmbiosRecord->SerialNumber = 0;
  //
  // AssetTag will be the 4th optional string following the formatted structure.
  //
  SmbiosRecord->AssetTag = (UINT8) mCpuAssetTagStrNumber;
  //
  // PartNumber is NOT currently set.
  //
  SmbiosRecord->PartNumber = 0;

  //
  //set STRING index explicitly (END).
  //

  SmbiosRecord->ProcessorType = CentralProcessor;
  SmbiosRecord->ProcessorFamily2 = (UINT8) GetProcessorFamily (ProcessorNumber);
  if (SmbiosRecord->ProcessorFamily2 > ProcessorFamilyIndicatorFamily2) {
    SmbiosRecord->ProcessorFamily = ProcessorFamilyIndicatorFamily2;
  } else {
    SmbiosRecord->ProcessorFamily = (UINT8) SmbiosRecord->ProcessorFamily2;
  }

  CpuidRegister = GetProcessorCpuid (ProcessorNumber, CPUID_VERSION_INFO);
  if (CpuidRegister == NULL) {
    ASSERT (CpuidRegister != NULL);
    return;
  }
  *(UINT32 *) &SmbiosRecord->ProcessorId.Signature = CpuidRegister->RegEax;
  *(UINT32 *) &SmbiosRecord->ProcessorId.FeatureFlags = CpuidRegister->RegEdx,

  ProcessorVoltage = GetProcessorVoltage (ProcessorNumber); // mV unit
  ProcessorVoltage = (UINT16) ((ProcessorVoltage * 10) / 1000);
  *(UINT8 *) &SmbiosRecord->Voltage = (UINT8) ProcessorVoltage;
  SmbiosRecord->Voltage.ProcessorVoltageIndicateLegacy = 1;

  SmbiosRecord->ExternalClock = (UINT16) (GET_CPU_MISC_DATA (ProcessorNumber, IntendedFsbFrequency));
  SmbiosRecord->MaxSpeed = (UINT16) PcdGet32 (PcdPlatformCpuMaxCoreFrequency);
  SmbiosRecord->CurrentSpeed = (UINT16) (GET_CPU_MISC_DATA (ProcessorNumber, IntendedFsbFrequency) * GET_CPU_MISC_DATA (ProcessorNumber, MaxCoreToBusRatio));

  //
  // Just indicate CPU Socket populated.
  // CPU_PROCESSOR_STATUS_DATA.SocketPopulated: BIT6 - CPU Socket populated and 0 - CPU Socket Unpopulated
  //
  SmbiosRecord->Status |= SMBIOS_TYPE4_CPU_SOCKET_POPULATED;

  ProcessorStatus.CpuStatus = 1;        // CPU Enabled
  ProcessorStatus.Reserved1 = 0;
  ProcessorStatus.SocketPopulated = 1;  // CPU Socket Populated
  ProcessorStatus.Reserved2 = 0;
  CopyMem (&SmbiosRecord->Status, &ProcessorStatus, 1);

  GetProcessorVersionInfo (ProcessorNumber, &FamilyId, &ModelId, NULL, NULL);

  //
  // The upgrade depends on processor family id and model id.
  //
  if ((FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (FamilyId, ModelId)) ||
      (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_D_SERVER_PROC (FamilyId, ModelId)) ||
      (FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) && IS_SAPPHIRERAPIDS_SERVER_PROC (FamilyId, ModelId)) ||
      (FeaturePcdGet (PcdCpuSnowridgeFamilyFlag) && IS_SNOWRIDGE_SERVER_PROC (FamilyId, ModelId))) {
    SmbiosRecord->ProcessorUpgrade = ProcessorUpgradeSocketLGA3647_1;
  } else if (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_SERVER_PROC (FamilyId, ModelId)) {
    SmbiosRecord->ProcessorUpgrade = ProcessorUpgradeSocketLGA4189;
  } else {
    SmbiosRecord->ProcessorUpgrade = ProcessorUpgradeSocketLGA775;
  }

  SmbiosRecord->L1CacheHandle = L1CacheHandle;
  SmbiosRecord->L2CacheHandle = L2CacheHandle;
  SmbiosRecord->L3CacheHandle = L3CacheHandle;

  SmbiosRecord->CoreCount2 = (UINT16)CoreCount;
  SmbiosRecord->CoreCount = (CoreCount > 255) ? 0xFF : (UINT8) (CoreCount);

  if ((FeaturePcdGet (PcdCpuSkylakeFamilyFlag) && IS_SKYLAKE_SERVER_PROC (FamilyId, ModelId)) ||
      (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_SERVER_PROC (FamilyId, ModelId)) ||
      (FeaturePcdGet (PcdCpuIcelakeFamilyFlag) && IS_ICELAKE_D_SERVER_PROC (FamilyId, ModelId)) ||
      (FeaturePcdGet (PcdCpuSapphireRapidsFamilyFlag) && IS_SAPPHIRERAPIDS_SERVER_PROC (FamilyId, ModelId)) ||
      (FeaturePcdGet (PcdCpuSnowridgeFamilyFlag) && IS_SNOWRIDGE_SERVER_PROC (FamilyId, ModelId))) {
    //
    // This MSR register depends on processor family id and model id.
    //
    MsrValue = GET_CPU_MISC_DATA (ProcessorNumber, EnabledThreadCountMsr); // PURLEY_OVERRIDE_5371439
    SmbiosRecord->EnabledCoreCount2 = (UINT16)(RShiftU64 (MsrValue, 16) & 0xFFFF);
    SmbiosRecord->ThreadCount2 = (UINT16)(MsrValue & 0xFFFF);
  } else {
    SmbiosRecord->EnabledCoreCount2 = (UINT16)CoreCount;
    SmbiosRecord->ThreadCount2 = (UINT16)ThreadCount;
  }

  SmbiosRecord->EnabledCoreCount = (SmbiosRecord->EnabledCoreCount2 > 255) ? 0xFF : (UINT8) (SmbiosRecord->EnabledCoreCount2);
  SmbiosRecord->ThreadCount = (SmbiosRecord->ThreadCount2 > 255) ? 0xFF : (UINT8) (SmbiosRecord->ThreadCount2);

  ProcessorCharacteristics.Reserved = 0;
  ProcessorCharacteristics.Capable64Bit = 1; // 64-bit Capable
  ProcessorCharacteristics.Unknown = 0;
  ProcessorCharacteristics.MultiCore = 1;

  ProcessorCharacteristics.EnhancedVirtualization = 0;
  if (((CPUID_VERSION_INFO_ECX*)&CpuidRegister->RegEcx)->Bits.VMX != 0) {
    ProcessorCharacteristics.EnhancedVirtualization = 1;
  }
  ProcessorCharacteristics.HardwareThread = 0;
  if (((CPUID_VERSION_INFO_EDX*)&CpuidRegister->RegEdx)->Bits.HTT != 0) {
    ProcessorCharacteristics.HardwareThread = 1;
  }
  ProcessorCharacteristics.ExecuteProtection = 0;
  if (GetNumberOfCpuidLeafs (ProcessorNumber, ExtendedCpuidLeaf) > 2) {
    //
    // Check CPUID(0x80000001).EDX[20]
    //
    CpuidRegister = GetProcessorCpuid (ProcessorNumber, CPUID_EXTENDED_CPU_SIG);
    if (CpuidRegister == NULL) {
      ASSERT (CpuidRegister != NULL);
      return;
    }

    if (((CPUID_EXTENDED_CPU_SIG_EDX*)&CpuidRegister->RegEdx)->Bits.NX != 0) {
      ProcessorCharacteristics.ExecuteProtection = 1;
    }
  }
  ProcessorCharacteristics.PowerPerformanceControl = 1;
  ProcessorCharacteristics.Reserved2 = 0;
  CopyMem (&SmbiosRecord->ProcessorCharacteristics, &ProcessorCharacteristics, 2);

  OptionalStrStart = (CHAR8 *) (SmbiosRecord + 1);
  UnicodeStrToAsciiStrS (CpuSocketStr, OptionalStrStart, CpuSocketStrLen + 1);
  UnicodeStrToAsciiStrS (CpuManuStr, OptionalStrStart + CpuSocketStrLen + 1, CpuManuStrLen + 1);
  UnicodeStrToAsciiStrS (CpuVerStr, OptionalStrStart + CpuSocketStrLen + 1 + CpuManuStrLen + 1, CpuVerStrLen + 1);
  UnicodeStrToAsciiStrS (CpuAssetTagStr, OptionalStrStart + CpuSocketStrLen + 1 + CpuManuStrLen + 1 + CpuVerStrLen + 1, CpuAssetTagStrLen + 1);
  //
  // Now we have got the full smbios record, call smbios protocol to add this record.
  //
  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  Status = mSmbios->Add (mSmbios, NULL, &SmbiosHandle, (EFI_SMBIOS_TABLE_HEADER *) SmbiosRecord);
  FreePool (SmbiosRecord);
  FreePool (CpuSocketStr);
  FreePool (CpuManuStr);
  FreePool (CpuVerStr);
  FreePool (CpuAssetTagStr);
  ASSERT_EFI_ERROR (Status);
}

/**
  Get Type 4 SMBIOS Record table.

  @param[in, out]   SmbiosHandle    On entry, points to the previous handle of the SMBIOS record. On exit, points to the
                                    next SMBIOS record handle. If it is zero on entry, then the first SMBIOS record
                                    handle will be returned. If it returns zero on exit, then there are no more SMBIOS records.
  @param[out]       Record          Returned pointer to record buffer .

**/
VOID
GetSmbiosProcessorTypeTable (
  IN OUT EFI_SMBIOS_HANDLE      *SmbiosHandle,
  OUT EFI_SMBIOS_TABLE_HEADER   *Record OPTIONAL
  )
{
  EFI_STATUS                 Status;
  EFI_SMBIOS_TYPE            RecordType;
  EFI_SMBIOS_TABLE_HEADER    *Buffer;

  RecordType = EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION;
  do {
    Status = mSmbios->GetNext (
                        mSmbios,
                        SmbiosHandle,
                        &RecordType,
                        &Buffer,
                        NULL
                        );
    if (!EFI_ERROR(Status)) {
      if (Record != NULL) {
        Record = Buffer;
      }
      return;
    }
  } while (!EFI_ERROR(Status));

}

/**
  Set the CPU socket name in SMBIOS table Type 4 (Processor Information).

  @param    PackageIndexList      Package indexes of populated CPU's
  @param    PopulatedPackageCount Count of populated packages.
**/
VOID
SetCpuSmbiosSocketNames (
  UINTN  *PackageIndexList,
  UINT32 PopulatedPackageCount
  )
{
  CHAR16            **CpuSocketNames;
  CHAR8             *CpuSocketStr;
  UINTN             CpuSocketStrLen;
  UINT32            PackageIndex;
  UINT32            SocketIndex;
  EFI_SMBIOS_HANDLE SmbiosHandle;

  CpuSocketNames = (CHAR16 **) (UINTN) (mCpuSmbiosLibCpuPolicyConfiguration->PlatformCpuSocketNames);
  if (CpuSocketNames == NULL) {
    return;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  //
  // Update CPU Socket string for Socket populated.
  //
  for (PackageIndex = 0; PackageIndex < PopulatedPackageCount; PackageIndex++) {
    GetSmbiosProcessorTypeTable (&SmbiosHandle, NULL);
    if (SmbiosHandle == SMBIOS_HANDLE_PI_RESERVED) {
      return;
    }
    CpuSocketStrLen = StrnLenS (CpuSocketNames[PackageIndexList[PackageIndex]], SMBIOS_STRING_MAX_LENGTH + 1);
    ASSERT (CpuSocketStrLen <= SMBIOS_STRING_MAX_LENGTH);
    CpuSocketStr = AllocatePool (CpuSocketStrLen + 1);
    UnicodeStrToAsciiStrS (CpuSocketNames[PackageIndexList[PackageIndex]], CpuSocketStr, CpuSocketStrLen + 1);
    mSmbios->UpdateString (mSmbios, &SmbiosHandle, &mCpuSocketStrNumber, CpuSocketStr);
    FreePool (CpuSocketStr);
  }

  //
  // Update CPU Socket string for Socket un-populated.
  //
  for (SocketIndex = 0; SocketIndex < PcdGet32 (PcdPlatformCpuSocketCount); SocketIndex++) {

    for (PackageIndex = 0; PackageIndex < PopulatedPackageCount; PackageIndex++) {
      if (PackageIndexList[PackageIndex] == SocketIndex) {
        break;
      }
    }
    if (PackageIndex < PopulatedPackageCount) {
      continue;
    }

    //
    // The SocketIndex doesn't exist in PackageIndexList
    //
    GetSmbiosProcessorTypeTable (&SmbiosHandle, NULL);
    if (SmbiosHandle == SMBIOS_HANDLE_PI_RESERVED) {
      return;
    }
    CpuSocketStrLen = StrnLenS (CpuSocketNames[SocketIndex], SMBIOS_STRING_MAX_LENGTH + 1);
    ASSERT (CpuSocketStrLen <= SMBIOS_STRING_MAX_LENGTH);
    CpuSocketStr = AllocatePool (CpuSocketStrLen + 1);
    UnicodeStrToAsciiStrS (CpuSocketNames[SocketIndex], CpuSocketStr, CpuSocketStrLen + 1);
    mSmbios->UpdateString (mSmbios, &SmbiosHandle, &mCpuSocketStrNumber, CpuSocketStr);
    FreePool (CpuSocketStr);
  }
}

/**
  Set the CPU asset tag in SMBIOS table Type 4 (Processor Information).

  @param    PackageIndexList       Package indexes of populated CPU's.
  @param    PopulatedPackageCount  Count of populated package.
**/
VOID
SetCpuSmbiosAssetTags (
  UINTN  *PackageIndexList,
  UINT32 PopulatedPackageCount
  )
{
  CHAR16            **CpuAssetTags;
  CHAR8             *CpuAssetTagStr;
  UINTN             CpuAssetTagStrLen;
  UINT32            Index;
  EFI_SMBIOS_HANDLE SmbiosHandle;

  CpuAssetTags = (CHAR16 **) (UINTN) (mCpuSmbiosLibCpuPolicyConfiguration->PlatformCpuAssetTags);
  if (CpuAssetTags == NULL) {
    return;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  //
  // Update CPU Asset Tag string for Socket populated.
  //
  for (Index = 0; Index < PopulatedPackageCount; Index++) {
    GetSmbiosProcessorTypeTable (&SmbiosHandle, NULL);
    if (SmbiosHandle == SMBIOS_HANDLE_PI_RESERVED) {
      return;
    }
    CpuAssetTagStrLen = StrnLenS (CpuAssetTags[PackageIndexList[Index]], SMBIOS_STRING_MAX_LENGTH + 1);
    ASSERT (CpuAssetTagStrLen <= SMBIOS_STRING_MAX_LENGTH);
    CpuAssetTagStr = AllocatePool (CpuAssetTagStrLen + 1);
    UnicodeStrToAsciiStrS(CpuAssetTags[PackageIndexList[Index]], CpuAssetTagStr, CpuAssetTagStrLen + 1);
    mSmbios->UpdateString (mSmbios, &SmbiosHandle, &mCpuAssetTagStrNumber, CpuAssetTagStr);
    FreePool (CpuAssetTagStr);
  }
}

