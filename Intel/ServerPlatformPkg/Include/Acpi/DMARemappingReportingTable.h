/** @file
  The definition for ACPI DMA-Remapping Reporting (DMAR) Table.
  It is defined in "Intel VT for Direct IO Architecture Specification".

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

#ifndef _EFI_DMA_REMAPPING_REPORTING_TABLE_H_
#define _EFI_DMA_REMAPPING_REPORTING_TABLE_H_

#include <IndustryStandard/Acpi.h>

//
// "DMAR" DMAR Description Table Signature
//
#define EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE  0x52414d44

//
// DMAR Revision
//
#define EFI_ACPI_DMAR_DESCRIPTION_TABLE_REVISION   0x01

//
// Ensure proper structure formats
//
#pragma pack (1)

//
// Definition for DMA Remapping Structure Types
//
#define EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_DRHD  0
#define EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_RMRR  1
#define EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_ATSR  2
#define EFI_ACPI_DMA_REMAPPING_STRUCTURE_TYPE_RHSA  3

//
// Definition for DMA Remapping Structure Header
//
typedef struct {
  UINT16                                      Type;
  UINT16                                      Length;
} EFI_ACPI_DMAR_STRUCTURE_HEADER;

//
// Definition for DMA-Remapping PCI Path
//
typedef struct {
  UINT8         Device;
  UINT8         Function;
} EFI_ACPI_DMAR_PCI_PATH;

//
// Definition for DMA-Remapping Device Scope Entry Structure
//
#define EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_ENDPOINT                 0x01
#define EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_BRIDGE                   0x02
#define EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_IOAPIC                   0x03
#define EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_MSI_CAPABLE_HPET         0x04
typedef struct {
  UINT8                   DeviceScopeEntryType;
  UINT8                   Length;
  UINT16                  Reserved_2;
  UINT8                   EnumerationID;
  UINT8                   StartingBusNumber;
} EFI_ACPI_DMAR_DEVICE_SCOPE_ENTRY_STRUCTURE;

//
// Definition for DMA-Remapping Hardware Definition (DRHD) Structure
//
#define EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_ALL_SET     0x1
#define EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_ALL_CLEAR   0x0
typedef struct {
  UINT16                                      Type;
  UINT16                                      Length;
  UINT8                                       Flags;
  UINT8                                       Reserved_5;
  UINT16                                      SegmentNumber;
  UINT64                                      RegisterBaseAddress;
} EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE;

//
// Definition for Reserved Memory Region Reporting (RMRR) Structure
//
typedef struct {
  UINT16                                      Type;
  UINT16                                      Length;
  UINT8                                       Reserved_4[2];
  UINT16                                      SegmentNumber;
  UINT64                                      ReservedMemoryRegionBaseAddress;
  UINT64                                      ReservedMemoryRegionLimitAddress;
} EFI_ACPI_DMAR_RESERVED_MEMORY_REGION_REPORTING_STRUCTURE;

//
// Definition for Root Port ATS Capability Reporting (ATSR) Structure
//
#define EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS_SET     0x1
#define EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS_CLEAR   0x0
typedef struct {
  UINT16                                      Type;
  UINT16                                      Length;
  UINT8                                       Flags;
  UINT8                                       Reserved_5;
  UINT16                                      SegmentNumber;
} EFI_ACPI_DMAR_ROOT_PORT_ATS_CAPABILITY_REPORTING_STRUCTURE;

//
// Definition for Remapping Hardware Static Affinity(RHSA) Structure
//
typedef struct {
  UINT16                                      Type;
  UINT16                                      Length;
  UINT32                                      Reserved;
  UINT64                                      RegisterBaseAddress;
  UINT32                                      ProximityDomain;
} EFI_ACPI_DMAR_REMAPPING_HARDWARE_STATIC_AFFINITY_STRUCTURE;

//
// Definition for DMA Remapping Structure
//
typedef union {
  EFI_ACPI_DMAR_STRUCTURE_HEADER                               DMARStructureHeader;
  EFI_ACPI_DMAR_HARDWARE_UNIT_DEFINITION_STRUCTURE             DMARHardwareUnitDefinition;
  EFI_ACPI_DMAR_RESERVED_MEMORY_REGION_REPORTING_STRUCTURE     DMARReservedMemoryRegionReporting;
  EFI_ACPI_DMAR_ROOT_PORT_ATS_CAPABILITY_REPORTING_STRUCTURE   DMARRootPortATSCapabilityReporting;
} EFI_ACPI_DMA_REMAPPING_STRUCTURE;

//
// Definition for DMA-Remapping Reporting ACPI Table
//
#define EFI_ACPI_DMAR_TABLE_FLAGS_INTR_REMAP_CLEAR              0x00
#define EFI_ACPI_DMAR_TABLE_FLAGS_INTR_REMAP_SET                0x01
#define EFI_ACPI_DMAR_TABLE_FLAGS_X2APIC_OPT_OUT                0x02
#define EFI_ACPI_DMAR_TABLE_FLAGS_DMA_CTRL_PLATFORM_OPT_IN_FLAG 0x04


typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER                               Header;
  UINT8                                                     HostAddressWidth;
  UINT8                                                     Flags;
  UINT8                                                     Reserved_38[10];
} EFI_ACPI_DMAR_DESCRIPTION_TABLE;

//
// The Platform specific definition can be as follows:
//   NOTE: we use /**/ as comment for user convenience to copy it.
//


#pragma pack()

#endif

