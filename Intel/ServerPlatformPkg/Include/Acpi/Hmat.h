/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#ifndef _HMAT_HMAT_H_
#define _HMAT_HMAT_H_

#include <IndustryStandard/Acpi.h>
#include <UncoreCommonIncludes.h>
#include "Platform.h"
#include "Msars.h"
#include "Sllbis.h"
#include "Mscis.h"

#define EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE_REVISION 0x01
#define EFI_ACPI_OEM_HMAT_REVISION  0x00000001

#define EFI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE_SIGNATURE  SIGNATURE_32('H', 'M', 'A', 'T')
#define MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE_TYPE 0
#define SYSTEM_LOCALITY_LATENCY_BANDWIDTH_INFORMATION_STRUCTURE_TYPE 1
#define MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE_TYPE 2

#define EFI_ACPI_HMAT_MSARS_COUNT  (MC_MAX_NODE * MAX_CRS_ENTRIES_PER_NODE)
#define EFI_ACPI_HMAT_LBIS_COUNT   (EFI_ACPI_HMAT_MAX_SLLBIS_DATA_TYPES * MAX_HMAT_MEMORY_HIERACHY_LEVELS)
#define EFI_ACPI_HMAT_MSCIS_COUNT  (MC_MAX_NODE * MAX_CRS_ENTRIES_PER_NODE)

//
// HMAT Definitions
//
//
// Latency Data From Performance Team in ns.
//
#define MCDRAM_FLAT_LATENCY  152
#define MCDRAM_CACHE_LATENCY 242 // Assuming 100% Miss
#define HBM_FLAT_LATENCY     MCDRAM_FLAT_LATENCY
#define HBM_CACHE_LATENCY    MCDRAM_CACHE_LATENCY

#define DDR_LATENCY 129

//
// BW Data From Performance Team in Mb/s
//
#define SNC4_0_4_MCDRAM_BW 132090
#define SNC4_0_5_MCDRAM_BW 107477
#define SNC4_0_6_MCDRAM_BW 120639
#define SNC4_0_7_MCDRAM_BW 72496
#define SNC4_1_4_MCDRAM_BW 111967
#define SNC4_1_5_MCDRAM_BW 132738
#define SNC4_1_6_MCDRAM_BW 84004
#define SNC4_1_7_MCDRAM_BW 123338
#define SNC4_2_4_MCDRAM_BW 111637
#define SNC4_2_5_MCDRAM_BW 84380
#define SNC4_2_6_MCDRAM_BW 125716
#define SNC4_2_7_MCDRAM_BW 96188
#define SNC4_3_4_MCDRAM_BW 86456
#define SNC4_3_5_MCDRAM_BW 120387
#define SNC4_3_6_MCDRAM_BW 100434
#define SNC4_3_7_MCDRAM_BW 125238

//
// Calculate close proximity Domain as the avarage of higher BW.
// For SNC2Node0 use higher BW from SNC4Node0,1; for SNC2Node1 use higher BW from SNC4Node2,3.
// Calculate far proximity Domain as the avarage of lower BW.
// For SNC2Node0 use lower BW Data from SNC4Node0,1; for SNC2Node1 use lower BW Data from SNC4Node2,3.
//
#define SNC2_0_0_MCDRAM_AV_BW ((SNC4_0_4_MCDRAM_BW + SNC4_0_6_MCDRAM_BW + SNC4_1_5_MCDRAM_BW + SNC4_1_7_MCDRAM_BW) / 2)
#define SNC2_0_1_MCDRAM_AV_BW ((SNC4_0_5_MCDRAM_BW + SNC4_0_7_MCDRAM_BW + SNC4_1_4_MCDRAM_BW + SNC4_1_6_MCDRAM_BW) / 2)
#define SNC2_1_0_MCDRAM_AV_BW ((SNC4_2_5_MCDRAM_BW + SNC4_2_7_MCDRAM_BW + SNC4_3_4_MCDRAM_BW + SNC4_3_6_MCDRAM_BW) / 2)
#define SNC2_1_1_MCDRAM_AV_BW ((SNC4_2_4_MCDRAM_BW + SNC4_2_6_MCDRAM_BW + SNC4_3_5_MCDRAM_BW + SNC4_3_7_MCDRAM_BW) / 2)

//
// Calculate Avarage All2All BW as the sum of all SNC4 Avarages BW divided by 4.
//
#define ALL2ALL_MCDRAM_AV_BW ((SNC4_0_4_MCDRAM_BW + SNC4_0_5_MCDRAM_BW + SNC4_0_6_MCDRAM_BW + SNC4_0_7_MCDRAM_BW + \
                               SNC4_1_4_MCDRAM_BW + SNC4_1_5_MCDRAM_BW + SNC4_1_6_MCDRAM_BW + SNC4_1_7_MCDRAM_BW + \
                               SNC4_2_4_MCDRAM_BW + SNC4_2_5_MCDRAM_BW + SNC4_2_6_MCDRAM_BW + SNC4_2_7_MCDRAM_BW + \
                               SNC4_3_4_MCDRAM_BW + SNC4_3_5_MCDRAM_BW + SNC4_3_6_MCDRAM_BW + SNC4_3_7_MCDRAM_BW) / 4)
#define ALL2ALL_HBM_AV_BW  ALL2ALL_MCDRAM_AV_BW

#define HMAT_BW_BASE_UNIT      1024 // 1024 MB/S
#define HMAT_LATENCY_BASE_UNIT 1    // 1ns

#define HBM_4_NODES_CASE  4
#define HBM_2_NODES_CASE  2
#define HBM_1_NODE_CASE   1

//
// In HBM as Cache mode, Cache Flags are only for Last Level of Cache (Flags = 1), and 1st Level of Cache (Flags = 2).
// So Flag = 0 should be skipped.
//
#define HMAT_HBM_CACHE_FLAG_OFFSET 1

#pragma pack(1)
typedef enum {
  TypeFlatMemoryMode        = 1,
  TypeCacheMemoryMode       = 2,
  TypeHybridMemoryMode      = 3,
} LBIS_MEMORY_FLAGS_TYPE;

typedef struct {
  UINT8   Valid;
  UINT32  ElementId;
  UINT64  MemMapIndexMap;
  UINT8   Cacheable;
  UINT64  MemorySideCacheSize;
  UINT16  NumSmbiosHandles;
  UINT16  SmbiosHandles[MAX_TYPE17_CACHE_DEVICES];
} MEMORY_DOMAIN_LIST_INFO;

typedef struct {
  UINT32                   ProcessorDomainNumber;
  UINT32                   MemoryDomainNumber;
  UINT16                   ProcessorDomainList [EFI_ACPI_HMAT_NUMBER_OF_PROCESSOR_DOMAINS];
  MEMORY_DOMAIN_LIST_INFO  MemoryDomainList [EFI_ACPI_HMAT_NUMBER_OF_MEMORY_DOMAINS];
  UINT8                    SncEnabled;
  UINT8                    SncNumOfCluster;
} HMAT_PROXIMITY_DOMAIN_DATA_STRUCTURE;

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER Header;
  UINT32 Reserved; // To make the structures 8 byte aligned
} EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE_HEADER;

typedef struct {
  EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE_HEADER HmatHeader;
  MEMORY_SUBSYSTEM_ADDRESS_RANGE_STRUCTURE Msars[EFI_ACPI_HMAT_MSARS_COUNT];
  LATENCY_BANDWIDTH_INFO_STRUCTURE Lbis[EFI_ACPI_HMAT_LBIS_COUNT];
  MEMORY_SIDE_CACHE_INFORMATION_STRUCTURE MemSideCache[EFI_ACPI_HMAT_MSCIS_COUNT];
} EFI_ACPI_HETEROGENEOUS_MEMORY_ATTRIBUTE_TABLE;
#pragma pack()

#endif /* _HMAT_H_ */
