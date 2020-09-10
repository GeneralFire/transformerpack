/** @file
  Code to retrieve processor sublcass data.

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

/**
  Returns the procesor version string token installed in the system.

  @param    ProcessorNumber  Processor number of specified processor.
  @param    Version          Pointer to the output processor version.

**/
VOID
GetProcessorVersion (
  IN UINTN                                  ProcessorNumber,
  OUT CPU_PROCESSOR_VERSION_INFORMATION     *Version
  )
{
  CHAR16                BrandIdString[MAXIMUM_CPU_BRAND_STRING_LENGTH + 1];
  EFI_CPUID_REGISTER    *CpuBrandString;
  CHAR8                 TempCpuBrandString[sizeof (EFI_CPUID_REGISTER) + 1];
  UINT8                 Index;

  //
  // Create the string using Brand ID String.
  //
  Version->StringValid = FALSE;

  if (IsIntelProcessor (ProcessorNumber)) {
    Version->StringRef = STRING_TOKEN (STR_INTEL_GENUINE_PROCESSOR);

    CpuBrandString = GetProcessorCpuid (ProcessorNumber, CPUID_BRAND_STRING1);
    ASSERT (CpuBrandString != NULL);
    if (CpuBrandString == NULL) {
      return; // Failed to get CPUID.
    }

    //
    // Check if Brand ID String is supported or filled up
    //
    TempCpuBrandString[sizeof (EFI_CPUID_REGISTER)] = 0;
    if (CpuBrandString->RegEax != 0) {
      CopyMem (TempCpuBrandString, CpuBrandString, sizeof (EFI_CPUID_REGISTER));
      AsciiStrToUnicodeStrS (TempCpuBrandString, (CHAR16 *) &BrandIdString[0], 16 + 1);

      CpuBrandString = GetProcessorCpuid (ProcessorNumber, CPUID_BRAND_STRING2);
      ASSERT (CpuBrandString != NULL);
      CopyMem (TempCpuBrandString, CpuBrandString, sizeof (EFI_CPUID_REGISTER));
      AsciiStrToUnicodeStrS (TempCpuBrandString, (CHAR16 *) &BrandIdString[16], 16 + 1);

      CpuBrandString = GetProcessorCpuid (ProcessorNumber, CPUID_BRAND_STRING3);
      ASSERT (CpuBrandString != NULL);
      CopyMem (TempCpuBrandString, CpuBrandString, sizeof (EFI_CPUID_REGISTER));
      AsciiStrToUnicodeStrS (TempCpuBrandString, (CHAR16 *) &BrandIdString[32], 16 + 1);

      //
      // Remove preceeding spaces
      //
      Index = 0;
      while (((Index < MAXIMUM_CPU_BRAND_STRING_LENGTH) && (BrandIdString[Index] == 0x20)) != 0) {
        Index++;
      }

      ASSERT (Index <= MAXIMUM_CPU_BRAND_STRING_LENGTH);
      CopyMem (
        Version->BrandString,
        &BrandIdString[Index],
        (MAXIMUM_CPU_BRAND_STRING_LENGTH - Index) * sizeof (CHAR16)
        );
      Version->BrandString[MAXIMUM_CPU_BRAND_STRING_LENGTH - Index] = 0;
      Version->StringValid = TRUE;
    }
  } else {
    Version->StringRef = STRING_TOKEN (STR_UNKNOWN);
  }
}

/**
  Returns the procesor manufaturer string token installed in the system.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   Processor Manufacturer string token.

**/
EFI_STRING_ID
GetProcessorManufacturer (
  IN UINTN  ProcessorNumber
  )
{
  if (IsIntelProcessor (ProcessorNumber)) {
    return STRING_TOKEN (STR_INTEL_CORPORATION);
  } else {
    return STRING_TOKEN (STR_UNKNOWN);
  }
}

/**
  Checks if processor is Intel or not.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   TRUE                Intel Processor.
  @return   FALSE               Not Intel Processor.

**/
BOOLEAN
IsIntelProcessor (
  IN UINTN  ProcessorNumber
  )
{
  EFI_CPUID_REGISTER  *Reg;

  Reg = GetProcessorCpuid (ProcessorNumber, CPUID_SIGNATURE);
  ASSERT (Reg != NULL);
  if (Reg == NULL) {
    return FALSE; // Can't get CPUID to confirm Intel processor.
  }

  //
  // After CPUID(0), check if EBX contians 'uneG', ECX contains 'letn', and EDX contains 'Ieni'
  //
  if ((Reg->RegEbx != 0x756e6547) || (Reg->RegEcx != 0x6c65746e) || (Reg->RegEdx != 0x49656e69)) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Returns the processor family of the processor installed in the system.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   Processor Family

**/
PROCESSOR_FAMILY_DATA
GetProcessorFamily (
  IN UINTN  ProcessorNumber
  )
{
  UINT32  FamilyId;
  UINT32  ModelId;
  UINT8   mPlatformType;

  if (IsIntelProcessor (ProcessorNumber)) {

    GetProcessorVersionInfo (ProcessorNumber, &FamilyId, &ModelId, NULL, NULL);
    //
    // If Merom, Conroe, Haswell, KNL, KNH, Icelake, Sapphire-rapid or Skylake processor family
    //
    if (FamilyId == 0x06 && (ModelId == 0x0F || ModelId == 0x16 || ModelId == 0x3F || ModelId == 0x55 ||
        ModelId == SNOWRIDGE_SERVER_MODEL_ID ||
        ModelId == ICELAKE_SERVER_MODEL_ID ||
        ModelId == SAPPHIRERAPIDS_SERVER_MODEL_ID )) {

      mPlatformType = PcdGet8 (PcdPlatformType);
      switch (mPlatformType) {
      case PLATFORM_DESKTOP:
        return ProcessorFamilyIntelPentiumD;

      case PLATFORM_MOBILE:
        return ProcessorFamilyIntelPentiumM;

      case PLATFORM_SERVER:
        return ProcessorFamilyIntelXeon;

      default:
        return ProcessorFamilyUnknown;
      }
    }

    return ProcessorFamilyPentium4;
  }

  return ProcessorFamilyUnknown;
}

/**
  Returns the processor voltage of the processor installed in the system.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   Processor Voltage in mV

**/
UINT16
GetProcessorVoltage (
  IN UINTN  ProcessorNumber
  )
{
  UINT16             VoltageInmV;
  EFI_CPUID_REGISTER *Reg;
  UINT32             FamilyId;
  UINT32             ModelId;

  Reg = GetProcessorCpuid (ProcessorNumber, CPUID_VERSION_INFO);
  ASSERT (Reg != NULL);
  if (Reg == NULL) {
    return 0; // Can't get CPUID.
  }
  GetProcessorVersionInfo (ProcessorNumber, &FamilyId, &ModelId, NULL, NULL);

  if ((Reg->RegEax >> 8 & 0x3F) == 0xF) {
    VoltageInmV = 3000;
  } else {
    VoltageInmV = 1600;
  }

  return VoltageInmV;
}

