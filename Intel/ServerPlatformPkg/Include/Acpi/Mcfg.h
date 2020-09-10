/** @file
  ACPI Memory mapped configuration space base address Description Table
  implementation, based on PCI Firmware Specification Revision 3.0 final draft,
  downloadable at http://www.pcisig.com/home

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#ifndef _MCFG_H_
#define _MCFG_H_

//
// Statements that include other files
//
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/MemoryMappedConfigurationSpaceAccessTable.h>
#include "Platform.h"
//
// "MCFG" Static Resource Affinity Table
//
#define EFI_ACPI_6_2_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_SIGNATURE 0x4746434D

//
// MCFG Definitions, see specification for details.
//
#define EFI_ACPI_OEM_MCFG_REVISION  0x00000001

//
// Define the number of each table type.
// This is where the table layout is modified.
//
#define EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_BASE_ADDRESS_STRUCTURE_COUNT MAX_SOCKET

//
// MCFG Table definition.  The table must be defined in a platform
// specific manner.
//
//
// Ensure proper structure formats
//
#pragma pack(1)

typedef struct {
  EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE_HEADER        Header;

#if EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_BASE_ADDRESS_STRUCTURE_COUNT > 0
  EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_SPACE_BASE_ADDRESS_ALLOCATION_STRUCTURE  Segment[
    EFI_ACPI_MEMORY_MAPPED_ENHANCED_CONFIGURATION_BASE_ADDRESS_STRUCTURE_COUNT];
#endif

} EFI_ACPI_MEMORY_MAPPED_CONFIGURATION_BASE_ADDRESS_TABLE;

#pragma pack()

#endif // _MCFG_H_
