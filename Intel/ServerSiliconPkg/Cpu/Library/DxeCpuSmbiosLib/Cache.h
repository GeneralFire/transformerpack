/** @file
  Include file for record cache subclass data with Smbios protocol.

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

#ifndef _CACHE_H_
#define _CACHE_H_

//
// Bit field definitions for return registers of CPUID EAX = 4
//
// EAX
#define CPU_CACHE_TYPE_MASK                0x1F
#define CPU_CACHE_LEVEL_MASK               0xE0
#define CPU_CACHE_LEVEL_SHIFT              5
// EBX
#define CPU_CACHE_LINESIZE_MASK            0xFFF
#define CPU_CACHE_PARTITIONS_MASK          0x3FF000
#define CPU_CACHE_PARTITIONS_SHIFT         12
#define CPU_CACHE_WAYS_MASK                0xFFC00000
#define CPU_CACHE_WAYS_SHIFT               22

#define CPU_CACHE_64K_GRANULARITY          0x8000

#define CPU_CACHE_L1        1
#define CPU_CACHE_L2        2
#define CPU_CACHE_L3        3
#define CPU_CACHE_L4        4
//
// Define the max cache level per IA manual
//
#define CPU_CACHE_LMAX      7

typedef struct {
  UINT8                         CacheLevel;
  UINT8                         CacheDescriptor;
  UINT32                        CacheSizeinKB;
  CACHE_ASSOCIATIVITY_DATA      Associativity;
  CACHE_TYPE_DATA               SystemCacheType;
} CPU_CACHE_CONVERTER;


typedef struct {
  UINT32                        CacheSizeinKB;
  CACHE_ASSOCIATIVITY_DATA      Associativity;
  CACHE_TYPE_DATA               SystemCacheType;
  //
  // Can extend the structure here.
  //
} CPU_CACHE_DATA;

//
// It is defined for SMBIOS_TABLE_TYPE7.CacheConfiguration.
//
typedef struct {
  UINT16    Level           :3;
  UINT16    Socketed        :1;
  UINT16    Reserved2       :1;
  UINT16    Location        :2;
  UINT16    Enable          :1;
  UINT16    OperationalMode :2;
  UINT16    Reserved1       :6;
} CPU_CACHE_CONFIGURATION_DATA;

/**
  Add Type 7 SMBIOS Record for Cache Information.

  @param[in]    ProcessorNumber     Processor number of specified processor.
  @param[in]    CoreCount           Number of cores of the specified package.
  @param[in]    DieCount            Number of dies of the specified package.
  @param[out]   L1CacheHandle       Pointer to the handle of the L1 Cache SMBIOS record.
  @param[out]   L2CacheHandle       Pointer to the handle of the L2 Cache SMBIOS record.
  @param[out]   L3CacheHandle       Pointer to the handle of the L3 Cache SMBIOS record.

**/
VOID
AddSmbiosCacheTypeTable (
  IN UINTN              ProcessorNumber,
  IN UINTN              CoreCount,
  IN UINTN              DieCount,
  OUT EFI_SMBIOS_HANDLE* L1CacheHandle,
  OUT EFI_SMBIOS_HANDLE* L2CacheHandle,
  OUT EFI_SMBIOS_HANDLE* L3CacheHandle
  );

#endif

