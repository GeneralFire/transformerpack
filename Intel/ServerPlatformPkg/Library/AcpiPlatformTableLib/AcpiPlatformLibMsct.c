/** @file
  ACPI Platform Driver Hooks

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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
// Statements that include other files
//
#include "AcpiPlatformLibLocal.h"
#include <Library/KtiApi.h>

/**

    Update the MSCT ACPI table

    @param *MsctAcpiTable - The table to be set

    @retval EFI_SUCCESS -  Returns Success

**/
EFI_STATUS
PatchMsctAcpiTable (
  IN OUT   EFI_ACPI_COMMON_HEADER   *Table
  )
{
  UINTN                       idx;
  UINT8                       checksum;
  UINT32                      MaxPhysicalAddressBit;
  EFI_CPUID_REGISTER          CpuidLeafInfo;
  EFI_ACPI_MAXIMUM_SYSTEM_CHARACTERISTICS_TABLE *MsctAcpiTable = NULL;
  UINT32                      MaxThreadCapacity;
  UINT32                      MaxSocketCount;

  MsctAcpiTable = (EFI_ACPI_MAXIMUM_SYSTEM_CHARACTERISTICS_TABLE *)Table;

  //
  // If SNC is enabled set the Maximum number of Proximity domains accordingly
  //
  MaxSocketCount = FixedPcdGet32 (PcdMaxCpuSocketCount);

  if (GetNumOfClusterPerSystem () != 0) {
    MsctAcpiTable->MaxNumProxDom = MaxSocketCount * GetNumOfClusterPerSystem () - 1;
  }



  // Update Maximum Physical Address
  // Get the number of address lines; Maximum Physical Address is 2^MaxPhysicalAddressBit - 1.
  // If CPUID does not support reporting the max physical address, then use a max value of 36 as per SDM 3A, 4.1.4.
  AsmCpuid (CPUID_EXTENDED_FUNCTION, &CpuidLeafInfo.RegEax, NULL, NULL, NULL);
  MaxPhysicalAddressBit = 36;
  if (CpuidLeafInfo.RegEax >= (UINT32) CPUID_VIR_PHY_ADDRESS_SIZE) {
    AsmCpuid (CPUID_VIR_PHY_ADDRESS_SIZE, &CpuidLeafInfo.RegEax, NULL, NULL, NULL);
    MaxPhysicalAddressBit = (UINT8) CpuidLeafInfo.RegEax;
  }

  MsctAcpiTable->MaxPhysicalAddress = (LShiftU64 (0x01, MaxPhysicalAddressBit) - 1);

  MaxPhysicalAddressBit = GetMaxPhysicalAddrBits ();

  MsctAcpiTable->MaxPhysicalAddress = (LShiftU64 (0x01, MaxPhysicalAddressBit) - 1);

  // First Proximity Domain Information Structure reports characteristics for all proximity domains,
  // since the characteristics are the same for all proximity domains.
  MsctAcpiTable->ProxDomInfoStructure[0].ProxDomRangeLow  = 0;
  MsctAcpiTable->ProxDomInfoStructure[0].ProxDomRangeHigh = MsctAcpiTable->MaxNumProxDom;

  // Max Number of Threads that the processor can have MaxThreadCapacity
  AsmCpuidEx (CPUID_EXTENDED_TOPOLOGY, 1, NULL, &MaxThreadCapacity, NULL, NULL);
  MsctAcpiTable->ProxDomInfoStructure[0].MaxProcessorCapacity = MaxThreadCapacity;

  // Max Memory capacity per proximity domain
  MsctAcpiTable->ProxDomInfoStructure[0].MaxMemoryCapacity = MsctAcpiTable->MaxPhysicalAddress;

  // Update Checksum
  MsctAcpiTable->Header.Checksum = 0;
  checksum = 0;
  for(idx = 0; idx < sizeof(EFI_ACPI_MAXIMUM_SYSTEM_CHARACTERISTICS_TABLE); idx++) {
    checksum = checksum + (UINT8) (((UINT8 *)(MsctAcpiTable))[idx]);
  }
  MsctAcpiTable->Header.Checksum = (UINT8) (0 - checksum);

  return EFI_SUCCESS;
}
