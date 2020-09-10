/** @file
  Include file for record processor subclass data with Smbios protocol.

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

#ifndef _PROCESSOR_H_
#define _PROCESSOR_H_

#include <PiDxe.h>

#include <Protocol/Smbios.h>
#include <AcpiCpuData.h>
#include <IndustryStandard/SmBios.h>
#include <Library/CpuConfigLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Cpu/CpuCoreRegs.h>

//
// This is the string tool generated data representing our strings.
//
extern EFI_SMBIOS_PROTOCOL      *mSmbios;
extern EFI_HII_HANDLE           mStringHandle;

//
// This constant defines the maximum length of the CPU brand string. According to the
// IA manual, the brand string is in EAX through EDX (thus 16 bytes) after executing
// the CPUID instructions with EAX as 80000002, 80000003, 80000004.
//
#define MAXIMUM_CPU_BRAND_STRING_LENGTH 48
#define SMBIOS_TYPE4_CPU_SOCKET_POPULATED           BIT6

typedef struct {
  BOOLEAN           StringValid;
  CHAR16            BrandString[MAXIMUM_CPU_BRAND_STRING_LENGTH + 1];
  EFI_STRING_ID     StringRef;
} CPU_PROCESSOR_VERSION_INFORMATION;

//
// It is defined for SMBIOS_TABLE_TYPE4.Status.
//
typedef struct {
  UINT8 CpuStatus       :3; // Indicates the status of the processor.
  UINT8 Reserved1       :3; // Reserved for future use. Should be set to zero.
  UINT8 SocketPopulated :1; // Indicates if the processor socket is populated or not.
  UINT8 Reserved2       :1; // Reserved for future use. Should be set to zero.
} CPU_PROCESSOR_STATUS_DATA;

//
// It is defined for SMBIOS_TABLE_TYPE4.ProcessorCharacteristics.
//
typedef struct {
  UINT16 Reserved                 :1;
  UINT16 Unknown                  :1;
  UINT16 Capable64Bit             :1;
  UINT16 MultiCore                :1;
  UINT16 HardwareThread           :1;
  UINT16 ExecuteProtection        :1;
  UINT16 EnhancedVirtualization   :1;
  UINT16 PowerPerformanceControl  :1;
  UINT16 Reserved2                :8;
} CPU_PROCESSOR_CHARACTERISTICS_DATA;

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
  );

/**
  Add Type 4 SMBIOS Record for Socket Unpopulated.
**/
VOID
AddUnpopulatedSmbiosProcessorTypeTable (
  VOID
  );

/**
  Set the CPU socket name in SMBIOS table Type 4 (Processor Information).

  @param    PackageIndexList      Package indexes of populated CPU's
  @param    PopulatedPackageCount Count of populated packages.
**/
VOID
SetCpuSmbiosSocketNames (
  UINTN  *PackageIndexList,
  UINT32 PopulatedPackageCount
  );

/**
  Set the CPU asset tag in SMBIOS table Type 4 (Processor Information).

  @param    PackageIndexList       Package indexes of populated CPU's.
  @param    PopulatedPackageCount  Count of populated package.
**/
VOID
SetCpuSmbiosAssetTags (
  UINTN  *PackageIndexList,
  UINT32 PopulatedPackageCount
  );

/**
  Returns the processor voltage of the processor installed in the system.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   Processor Voltage in mV

**/
UINT16
GetProcessorVoltage (
  IN UINTN  ProcessorNumber
  );

/**
  Returns the procesor version string token installed in the system.

  @param    ProcessorNumber  Processor number of specified processor.
  @param    Version          Pointer to the output processor version.

**/
VOID
GetProcessorVersion (
  IN UINTN                                  ProcessorNumber,
  OUT CPU_PROCESSOR_VERSION_INFORMATION     *Version
  );

/**
  Returns the processor family of the processor installed in the system.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   Processor Family

**/
PROCESSOR_FAMILY_DATA
GetProcessorFamily (
  IN UINTN  ProcessorNumber
  );

/**
  Returns the procesor manufaturer string token installed in the system.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   Processor Manufacturer string token.

**/
EFI_STRING_ID
GetProcessorManufacturer (
  IN UINTN  ProcessorNumber
  );

/**
  Checks if processor is Intel or not.

  @param    ProcessorNumber     Processor number of specified processor.

  @return   TRUE                Intel Processor.
  @return   FALSE               Not Intel Processor.

**/
BOOLEAN
IsIntelProcessor (
  IN UINTN  ProcessorNumber
  );

#endif

