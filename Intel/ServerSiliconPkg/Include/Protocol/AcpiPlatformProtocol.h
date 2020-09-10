/** @file
  EFI ACPI Platform Protocol

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#ifndef _ACPI_PLATFORM_PROTOCOL_H
#define _ACPI_PLATFORM_PROTOCOL_H

#include <IndustryStandard/Acpi62.h>

///
/// ACPI Platform protocol provided for DXE phase
///
typedef struct _ACPI_PLATFORM_PROTOCOL  ACPI_PLATFORM_PROTOCOL;

typedef EFI_ACPI_6_2_MEMORY_AFFINITY_STRUCTURE ACPI_MEMORY_AFFINITY_DATA;
#define ACPI_MEMORY_NONVOLATILE   EFI_ACPI_6_2_MEMORY_NONVOLATILE

/**
  Function retrieves selected data of ACPI SRAT Memory Affinity Structures
  (please note that data will not be available until SRAT table installation)

  @param[out] *MemAffData         ACPI Memory Affinity Data
  @param[out] *MemAffDataLength   ACPI Memory Affinity Data Length

  @retval EFI_SUCCESS             ACPI Memory Affinity Data retrieved successfully
  @retval EFI_NOT_FOUND           ACPI Memory Affinity Data not found (SRAT ACPI table was not published)
  @retval EFI_INVALID_PARAMETER   One or more of input arguments is NULL
**/
typedef
EFI_STATUS
(EFIAPI *GET_ACPI_MEMORY_AFFINITY_DATA) (
  OUT ACPI_MEMORY_AFFINITY_DATA **MemAffData,
  OUT UINTN                     *MemAffDataLength
  );


/**
  ACPI Platform protocol provided for DXE phase
**/
struct _ACPI_PLATFORM_PROTOCOL {
  GET_ACPI_MEMORY_AFFINITY_DATA GetAcpiMemoryAffinityData;
};

extern EFI_GUID gAcpiPlatformProtocolGuid;

#endif // _ACPI_PLATFORM_PROTOCOL_H

