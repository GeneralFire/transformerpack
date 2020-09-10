/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2019 Intel Corporation. <BR>

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
// The version of Nvdimm F/W Interface Table (Nfit) supported has been updated to ASWG-ECR-Draft4-RC6_93.doc
//

#ifndef _NFIT_H_
#define _NFIT_H_

#include "JedecNvdimm.h"
#include <IndustryStandard/Acpi62.h>

#define NFIT_HEADER                         EFI_ACPI_6_2_NVDIMM_FIRMWARE_INTERFACE_TABLE

#define NVDIMM_FW_INTERFACE_TABLE_SIGNATURE EFI_ACPI_6_2_NVDIMM_FIRMWARE_INTERFACE_TABLE_STRUCTURE_SIGNATURE
#define NVDIMM_FW_INTERFACE_TABLE_REVISION  EFI_ACPI_6_2_NVDIMM_FIRMWARE_INTERFACE_TABLE_REVISION

#define VOLATILE_REGION_GUID                EFI_ACPI_6_2_NFIT_GUID_VOLATILE_MEMORY_REGION
#define PMEM_REGION_GUID                    EFI_ACPI_6_2_NFIT_GUID_BYTE_ADDRESSABLE_PERSISTENT_MEMORY_REGION
#define CONTROL_REGION_GUID                 EFI_ACPI_6_2_NFIT_GUID_NVDIMM_CONTROL_REGION
#define BLOCK_DATA_WINDOW_REGION_GUID       EFI_ACPI_6_2_NFIT_GUID_NVDIMM_BLOCK_DATA_WINDOW_REGION
#define VOLATILE_VIRTUAL_DISK_REGION_GUID   EFI_ACPI_6_2_NFIT_GUID_RAM_DISK_SUPPORTING_VIRTUAL_DISK_REGION_VOLATILE
#define VOLATILE_VIRTUAL_CD_REGION_GUID     EFI_ACPI_6_2_NFIT_GUID_RAM_DISK_SUPPORTING_VIRTUAL_CD_REGION_VOLATILE
#define PMEM_VIRTUAL_DISK_REGION_GUID       EFI_ACPI_6_2_NFIT_GUID_RAM_DISK_SUPPORTING_VIRTUAL_DISK_REGION_PERSISTENT
#define PMEM_VIRTUAL_CD_REGION_GUID         EFI_ACPI_6_2_NFIT_GUID_RAM_DISK_SUPPORTING_VIRTUAL_CD_REGION_PERSISTENT

#define MAX_LINES                   64
#define MAX_SADS                    64
#define INT_TBL_HDR_SIZE            16
#define BITS_64MB_TO_BYTES          26

// Max Nfit size is set so that the total Nfit is slightly below 4k (as the header takes some).
#define MAX_NFIT_SIZE               0x18000

// Defines for Descriptor 'Type's
#define TYPE_SPA_RANGE_DESCRIPTOR               EFI_ACPI_6_2_NFIT_SYSTEM_PHYSICAL_ADDRESS_RANGE_STRUCTURE_TYPE
#define TYPE_NVDIMM_REGION_MAPPING_DESCRIPTOR   EFI_ACPI_6_2_NFIT_NVDIMM_REGION_MAPPING_STRUCTURE_TYPE
#define TYPE_INTERLEAVE_DESCRIPTOR              EFI_ACPI_6_2_NFIT_INTERLEAVE_STRUCTURE_TYPE
#define TYPE_SMBIOS_MGMT_INFO_DESCRIPTOR        EFI_ACPI_6_2_NFIT_SMBIOS_MANAGEMENT_INFORMATION_STRUCTURE_TYPE
#define TYPE_CNTRL_REGION_DESCRIPTOR            EFI_ACPI_6_2_NFIT_NVDIMM_CONTROL_REGION_STRUCTURE_TYPE
#define TYPE_BLK_WINDOW_DESCRIPTOR              EFI_ACPI_6_2_NFIT_NVDIMM_BLOCK_DATA_WINDOW_REGION_STRUCTURE_TYPE
#define TYPE_FLUSH_HINT_DESCRIPTOR              EFI_ACPI_6_2_NFIT_FLUSH_HINT_ADDRESS_STRUCTURE_TYPE

// There could be at most 2 Control Region Description Tables per channel (for BLOCK and PMEM)
#define MAX_CTRL_RGNS_PER_CHANNEL   2

// Control region size difference, when Number of Block Control Windows is 0
#define CONTROL_REGION_SIZE_DIFF_NO_BLOCK_WINDOWS   48

// Control Region's DPA in dimm is 0.
#define CTRL_REG_DPA_OFFSET                     0

#define NUM_BLOCK_CTRL_WINDOWS                  256
// Block Control Window Size is 8kb
#define BLOCK_CTRL_WIN_SIZE                     0x2000
// Offset from the start of the CTRL region to the start of the BIOS mailbox
#define BLK_CONTROL_OFFSET                      0x800000
// Status Register Offset is 4k from the start of the Control Region
#define BLK_CONTROL_STATUS_REG_OFFSET           0x801000
#define STATUS_REG_SIZE                         4

//
// Memory Device To System Physical Address Range Mapping Table
// NfitDevHandle:
// Bit [3:0]   - Dimm number,
// Bit [7:4]   - Channel #,
// Bit [11:8]  - iMC iD,
// Bit [15:12] - Socket ID,
// Bit [27:16] - Node ID
// Bit [31:28] - reserved
#define NFIT_DEVICE_HANDLE              EFI_ACPI_6_2_NFIT_DEVICE_HANDLE

//
// System Physical Address Range Description Table struct
//
// Flags field definitions:
// Bit [0] set to 1 indicates that Control region is strictly
//   for management during hot add/online operation.
// Bit [1] set to 1 to indicate that data in Proximity
//   Domain field is valid.
// Bits [15:2] : Reserved
// Flag bit field definitions for SPA table
#define CNTRL_RGN_FOR_HOT_ADD           EFI_ACPI_6_2_NFIT_SYSTEM_PHYSICAL_ADDRESS_RANGE_FLAGS_CONTROL_REGION_FOR_MANAGEMENT
#define PROX_DOMAIN_VALID               EFI_ACPI_6_2_NFIT_SYSTEM_PHYSICAL_ADDRESS_RANGE_FLAGS_PROXIMITY_DOMAIN_VALID

#define SPA_RANGE_DESC_TABLE            EFI_ACPI_6_2_NFIT_SYSTEM_PHYSICAL_ADDRESS_RANGE_STRUCTURE

// Flag bit field definitions used for MEMDEV table
#define PREVIOUS_SAVE_FAIL              EFI_ACPI_6_2_NFIT_MEMORY_DEVICE_STATE_FLAGS_PREVIOUS_SAVE_FAIL
#define PLATFORM_FLUSH_FAIL             EFI_ACPI_6_2_NFIT_MEMORY_DEVICE_STATE_FLAGS_PLATFORM_FLUSH_FAIL
#define SMART_EVENTS_OBSERVED           EFI_ACPI_6_2_NFIT_MEMORY_DEVICE_STATE_FLAGS_SMART_HEALTH_EVENTS_PRIOR_OSPM_HAND_OFF

#define NVDIMM_REGION_MAPPING_TABLE     EFI_ACPI_6_2_NFIT_NVDIMM_REGION_MAPPING_STRUCTURE

#define INTERLEAVE_DESC_TABLE           EFI_ACPI_6_2_NFIT_INTERLEAVE_STRUCTURE
#define SMBIOS_MGMT_INFO_TABLE          EFI_ACPI_6_2_NFIT_SMBIOS_MANAGEMENT_INFORMATION_STRUCTURE
#define BLK_WINDOW_DESC_TABLE           EFI_ACPI_6_2_NFIT_NVDIMM_BLOCK_DATA_WINDOW_REGION_STRUCTURE
#define FLUSH_HINT_ADDR_TABLE           EFI_ACPI_6_2_NFIT_FLUSH_HINT_ADDRESS_STRUCTURE


//
// Definition for NVDIMM Control Region Structure from ACPI6.1 industry standard include
//

#pragma pack(1)

#define MANUFACTURING_VALID_FIELDS  EFI_ACPI_6_2_NFIT_NVDIMM_CONTROL_REGION_VALID_FIELDS_MANUFACTURING
#define BLOCK_DATA_WINDOWS_BUFFERED EFI_ACPI_6_2_NFIT_NVDIMM_CONTROL_REGION_FLAGS_BLOCK_DATA_WINDOWS_BUFFERED

#define IFC_STORAGE_MODE            0x0201
#define IFC_APPDIRECT_MODE          0x0301
#define IFC_AUTO                    0x0

#define CTRL_REGION_TABLE           EFI_ACPI_6_2_NFIT_NVDIMM_CONTROL_REGION_STRUCTURE

//
// NVDIMM Firmware Interface Table struct
// IMPORTANT NOTE: Num of various sub-table fields of the Nfit has been eliminated in the latest Nfit Spec (0.8k)
// And also since there is NO memory allocation scheme for Nfit (all globals), Adding a buffer NfitTables with the,
// max nfit size currently set to 4k. Will need to watch if we will ever overrun this buffer. It is expected that
// Nfit parser will need to parse the NfitTables field to understand the various other Nfit Tables in this buffer
// based on the Type and Length field of these tables.
// A BIG NOTE: When ever the NVDIMM_FW_INTERFACE_TABLE defined either here or what is in Nfit.h of PlatPkg/Include/Acpi
// changes, the same changes need to relect in both the files.
//
typedef struct _NVDIMM_FW_INT_TBL {
  UINT32       Signature;                    // 'NFIT' should be the signature for this table
  UINT32       Length;                       // Length in bytes fir the entire table
  UINT8        Revision;                     // Revision # of this table, initial is '1'
  UINT8        Checksum;                     // Entire Table Checksum must sum to 0
  UINT8        OemID[6];                     // OemID
  UINT64       OemTblID;                     // Should be Manufacturer's Model #
  UINT32       OemRevision;                  // Oem Revision of for Supplied OEM Table ID
  UINT32       CreatorID;                    // Vendor ID of the utility that is creating this table
  UINT32       CreatorRev;                   // Revision of utility that is creating this table
  UINT32       Reserved;
  UINT8        NfitTables[MAX_NFIT_SIZE];    // See Description above (IMPORTANT).
} NVDIMM_FW_INTERFACE_TABLE, *PNVDIMM_FW_INTERFACE_TABLE;

#pragma pack()

EFI_STATUS
BuildNfitTable (
  UINT64 *JedecNvdimmTablePtr,
  UINTN  JedecNvdimmTableSize,
  PMEM_REGION   *PmemRegions
  );

EFI_STATUS
UpdateNfitTableChecksum (
  UINT64 *JedecNvdimmTablePtr
  );

#endif  // _NFIT_H_

