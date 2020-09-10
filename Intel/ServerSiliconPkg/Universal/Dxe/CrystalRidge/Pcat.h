/** @file
  Pcat.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#ifndef _ACPI_PCAT_H_
#define _ACPI_PCAT_H_

#include <Uefi/UefiBaseType.h>
#include <Library/PmemMiscLib.h>

/// @brief PCAT Signature to put in table header
#define NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_SIGNATURE SIGNATURE_32('P', 'C', 'A', 'T')

/// @brief Table type for SSKU Attribute Extension Table
#define SSKU_ATTRIBUTE_EXTENSION_TABLE_TYPE              0x03
/// @brief Vendor ID for SSKU Attribute Extension Table
#define SSKU_ATTRIBUTE_EXTENSION_TABLE_VENDOR_ID         0x8086
// Allocated Max size for Platform Config Attribute Table
#define MAX_PCAT_SIZE                                    0x200
// Maximum Interleave list seen so far is as below
#define MAX_INT_LIST                                     4
// Interleave Capability Header Size
#define INTERLEAVE_CAP_HEADER_SIZE                       16
// Maximum size of all the PCAT Tables = MAX_PCAT_SIZE - PCAT Header size (40 bytes)
#define PCAT_MAX_TBL_SIZE                                472

// Platform Capabilities Information Table
#define TYPE_PLAT_CAP_INFO_TABLE                       0
#define PLAT_CAP_MEM_MODE_CAP_1LM                      BIT0
#define PLAT_CAP_MEM_MODE_CAP_2LM                      BIT1
#define PLAT_CAP_MEM_MODE_CAP_AD                       BIT2
#define PLAT_CAP_MEM_MODE_CAP_ADWB                     BIT3
#define PLAT_CAP_CURRENT_VOL_MEM_MODE_1LM              0
#define PLAT_CAP_CURRENT_VOL_MEM_MODE_2LM              1
#define PLAT_CAP_CURRENT_VOL_MEM_MODE_MIX_1LM2LM       2
#define PLAT_CAP_CURRENT_PER_MEM_MODE_NONE             0
#define PLAT_CAP_CURRENT_PER_MEM_MODE_AD               1
#define PLAT_CAP_CURRENT_PER_MEM_MODE_ADWB             2
#define PLAT_CAP_CURRENT_PER_MEM_MODE_AD_OR_ADWB       3
#define PLAT_CAP_CURRENT_ALLOW_VOL_MEM_MODE_1LM        0
#define PLAT_CAP_CURRENT_ALLOW_VOL_MEM_MODE_1LM_OR_2LM 1
#define PLAT_CAP_CURRENT_ALLOW_VOL_MEM_MODE_MIX_1LM2LM 2

// Memory Interleave Capability Information Table
#define TYPE_INTERLEAVE_CAPABILITY_DESCRIPTOR 1
#define CH_INT_SIZE_256B                      BIT2
#define CH_INT_SIZE_4KB                       BIT6
#define IMC_INT_SIZE_256B                     BIT10
#define IMC_INT_SIZE_4KB                      BIT14
#define AD_MODE                               3
#define ADWB_MODE                             4

// Die SKU Information Table
#define TYPE_SOCKET_SKU_INFORMATION_TABLE 6


#pragma pack(push, 1)

//
// Memory Interleave Capability Information struct - Per DCPMM
// Note:
//  IntAlignSize: Interleave alignment size in 2^n bytes, n = 26 for 64MB, n = 27 for 128MB
//  InterleaveSize:
//  Byte0: Channel Interleave Size
//    Bits[7:0] - Bit2 - 256B, Bit6 - 4KB, others bits are reserved
//  Byte1: iMC Interleave Size
//    Bits[7:0] - Bit2 - 256B, Bit6 - 4KB, other bits are reserved
//  Byte2-3: Number of channel ways, Recommended Interleave Format
//    Bits[14:0] - Bit0 - 1way, Bit1 - 2way, Bit2 - 3way, Bit3 - 4way, Bit4 - 6way, Bit5 - 8way, Bit6 - 12way, Bit7 - 16way, Bit8 - 24way
//    Bits[14:9] - Reserved
//    Bit15 - If clear, the interleave format is supported but not recommended
//
//               - certain interleave size combinations
//    Bit[15] - If clear, the interleave format is supported but not recommended. If set, the interleave format is recommended.
//
typedef struct _INT_CAP {
  UINT16        Type;             // Should be 1 for Interleave Capability
  UINT16        Length;           // Length in bytes of this entire table
  UINT8         MemMode;          // 3 = App Direct, 4 = App Direct WB, Other values are reserved
  UINT8         Rsvrd[3];
  UINT16        IntAlignSize;     // Described above
  UINT16        InterleaveSize;   // Described above
  MAX_INT_SET   MaxInterleaveSet; // The maximum number of interleave sets supported for this memory type
  UINT16        NumIntFormats;    // Number of Interleave formats supported by BIOS for the above memory mode.
  PM_INTERLEAVE IntFormatList[0]; // Interleave Format List
} INTERLEAVE_CAPABILITIES, *PINTERLEAVE_CAPABILITIES;

//
// Platform Capability Information Table
// DcpmmMgmtSWConfigInput
//   Bit0:If set BIOS supports changing configuration thru mgmt software, else otherwise
//   Bits[7:1]:Reserved
// MemModeCap
//   Bit0:1LM Mode
//   Bit1:2LM Mode
//   Bit2:App Direct Mode
//   Bit3:App Direct WB Mode
//   Bits[7:4]:Reserved
// CurrentMemMode
//   Bits[1:0] - Current Volatile Memory Mode
//     00b:1LM Mode
//     01b:2LM Mode
//     10b:1LM + 2LM Mode
//     11b:Reserved
//   Bits[3:2] - Allowed Persistent Memory Mode
//     00b:None
//     01b:App Direct Mode
//     10b:App Direct WB Mode
//     11b:App Direct or App Direct WB Mode
//   Bits[5:4] - Allowed Volatile Memory Mode (based on setup menu)
//     00b:1LM Mode Only
//     01b:1LM or 2LM Mode
//     10b:1LM + 2LM Mode
//     11b:Reserved
//   Bits[7:6]:Reserved
// MaxPmemIntSet
// The maximum number of interleave sets of any memory type:
//   Note: 0 means there is no limit defined.
//   Bits[3-0]:  per CPU Die
//   Bits[7-4]:  per DCPMM
//   Bits[15-8]: Reserved
// DdrCacheSize
// Capacity in GiB per channel for use as near memory cache if 2LM is enabled.
// The remaining DDR capacity will be used as 1LM.
// Note: A value of 0 indicates the DDR capacity is not setup for use as both 1LM memory and 2LM near memory cache.
// CacheCapabilities
//   Bits[3-0] - Memory Mode Cache Capabilities
//    00b - In-tile
//    01b - Cross-tile
//    Other values reserved
//   Bits[7-4]: App Direct WB Mode Cache Capabilities
//    00b - In-tile
//    01b - Cross-tile
//    Other values reserved
//

//
// The requested DDR Cache Size is always a power of 2 and is calculated using the formula:
// 2^(33 + DdrCacheSize). This is then converted from Byte to GiB Granularity by right shifting
// by 30.
//
#define PCAT_DDR_CACHE_SIZE_8GB          (BIT0 << 3)
#define PCAT_DDR_CACHE_SIZE_16GB         (BIT0 << 4)
#define PCAT_DDR_CACHE_SIZE_32GB         (BIT0 << 5)
#define PCAT_DDR_CACHE_SIZE_64GB         (BIT0 << 6)
#define PCAT_DDR_CACHE_SIZE_128GB        (BIT0 << 7)
#define PCAT_DDR_CACHE_SIZE_256GB        (BIT0 << 8)


typedef struct _PLATFORM_CAP {
  UINT16      Type;                   // Should be 0 for Plat Cap Info table
  UINT16      Length;                 // Length in bytes of this entire table
  UINT8       DcpmmMgmtSwConfigInput; // Defined above
  UINT8       MemModeCap;             // Memory Mode capabilities defined above
  UINT8       CurrentMemMode;         // Current Memory enabled in BIOS
  MAX_INT_SET MaxPmemIntSet;          // The maximum number of interleave sets of any memory type
  UINT32      DdrCacheSize;           // Capacity in GiB per channel for use as near memory cache
                                      // if 2LM is enabled. The remaining DDR capacity will be
                                      // used as 1LM. Only valid if Current Volatile Memory Mode
                                      // is 1LM+2LM.
  union {
    UINT8     CacheCapabilities;      // 2LM (MM and ADWB) cache capabilities
    UINT8     Reserved[3];            // Reserved
  } Data;
} PLAT_CAPABILITIES_INFO, *PPLAT_CAPABILITIES_INFO;

//
// Socket SKU Information Table (Type 6) - Per Socket
// Note:
//  MemMapSizeLimit: The total amount of physical memory in bytes that is allowed to be mapped into the SPA based
//                   on the SKU of the CPU specified by Socket ID.
//  TotMemSizeMapSPA: Total amount of physical memory in bytes that is currently mapped into the SPA for the CPU specified by Socket ID.
//                    Note: Usable capacity may be further reduced in some cases (e.g. mirrored capacity).
//  CacheMemSize2LM: Total amount of physical memory in bytes that is used for caching when the system is in 2LM mode.
//
typedef struct {
  UINT16 Type;             // Should be 6 for Socket SKU information table
  UINT16 Length;           // Length in bytes of this entire table
  UINT16 SocketId;         // Zero indexed socket identifier
  UINT16 DieId;            // Zero indexed die identifier
  UINT64 MemMapSizeLimit;  // Described as above.
  UINT64 TotMemSizeMapSPA; // Described as above.
  UINT64 CacheMemSize2LM;  // Described as above.
} SOCKET_SKU_INFO_TABLE, *PSOCKET_SKU_INFO_TABLE;

// A BIG NOTE: When ever the NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE defined either here or
// what is in Pcat.h in PlatPkg/Include/Acpi changes, the same changes need to relect in both the files.
typedef struct {
  UINT32 Signature;   // 'PCAT' should be the signature for this table
  UINT32 Length;      // Length in bytes for the entire table
  UINT8  Revision;    // Revision # of this table, initial is '1'
  UINT8  Checksum;    // Entire Table Checksum must sum to 0
  UINT8  OemID[6];    // OemID
  UINT8  OemTblID[8]; // Should be Manufacturer's Model #
  UINT32 OemRevision; // Oem Revision of for Supplied OEM Table ID
  UINT32 CreatorID;   // Vendor ID of the utility that is creating this table
  UINT32 CreatorRev;  // Revision of utility that is creating this table
  UINT32 Reserved;    // Alignement for size modulo 8 = 0
} NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE, *PNVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE;
#pragma pack(pop)

/// @brief Layout of SSKU Attribute Extension Table header
#pragma pack(push, 1)
typedef struct {
  UINT16   Type;     // Type for Mgmt Info Struct, type should be 3
  UINT16   Length;   // Length in Bytes for Entire SSKU Attribute Extension Table size.
  UINT16   Rsvd;
  UINT16   VendorID; // Vendor ID
  EFI_GUID Guid;     // Intel defined SSKU Attribute Extension Table. guid: F93032E5-B045-40ef-91C8-F02B06AD948D
} NVDIMM_SSKU_ATTR_EXT_TABLE, *PNVDIMM_SSKU_ATTR_EXT_TABLE;
#pragma pack(pop)

/**
  @brief Install protocol to access ACPI PCAT supporting functions
*/
EFI_STATUS
InstallAcpiPcatProtocol (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  );


#endif //_ACPI_PCAT_H_

