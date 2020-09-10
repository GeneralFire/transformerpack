/** @file
  This file provides the interfaces of NumaAcpiTableLib

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _NUMA_ACPI_TABLE_LIB_H_
#define _NUMA_ACPI_TABLE_LIB_H_

#include <Library/MemTypeLib.h>
#include <UncoreCommonIncludes.h>


#define MAX_NUMA_PROXIMITY_DOMAIN   ((MAX_SOCKET) * (MAX_DRAM_CLUSTERS) * (MAX_SAD_RULES))   // Maximum number of NUMA proximity domains.
#define MAX_THREAD_PER_SOCKET       ((MAX_CPU_NUM) / (MAX_SOCKET))                           // Maximum number of threads per socket.
#define MAX_MEM_RANGE               MAX_SAD_RULES                                            // Maximum number of memory ranges per memory proximity domain.

#define PROXIMITY_DOMAIN_ATTR       UINT8
#define PROXIMITY_DOMAIN_PROCESSOR  BIT0
#define PROXIMITY_DOMAIN_MEMORY     BIT1


typedef struct {
  UINT8        ApicId;              ///< APIC ID of the thread.
  UINT8        LocalSapicEid;       ///< Local SAPIC EID of the thread.
} THREAD_NODE;

typedef struct {
  UINT8        MemRangeNum;                        ///< Number of memory ranges that belong to the memory proximity domain.
  UINT32       BaseAddr[MAX_MEM_RANGE];            ///< Base address of the memory ranges in 64MB granularity.
  UINT32       LimitAddr[MAX_MEM_RANGE];           ///< Limit address of the memory ranges in 64MB granularity.
  MEM_TYPE     MemType;                            ///< Memory type of the memory proximity domain.
} MEMORY_PROXIMITY_DOMAIN;

typedef struct {
  UINT16       ThreadNum;                          ///< Number of thread nodes that belong to the processor proximity domain.
  THREAD_NODE  ThreadList[MAX_THREAD_PER_SOCKET];  ///< List of thread nodes that belong to the processor proximity domain.
} PROCESSOR_PROXIMITY_DOMAIN;

typedef struct {
  UINT8                       SocketId;            ///< Socket index of the proximity domain.
  UINT8                       ClusterId;           ///< Cluster index within socket of the proximity domain.
  PROXIMITY_DOMAIN_ATTR       Attr;                ///< Flag indicates the type of resources the proximity domain contains.
  PROCESSOR_PROXIMITY_DOMAIN  ProcessorDomain;     ///< Processor proximity domain data.
  MEMORY_PROXIMITY_DOMAIN     MemoryDomain;        ///< Memory proximity domain data.
} PROXIMITY_DOMAIN;

typedef struct {
  UINT32                      ProximityDomainNum;                              ///< Proximity domain number.
  PROXIMITY_DOMAIN            ProximityDomainList[MAX_NUMA_PROXIMITY_DOMAIN];  ///< Proximity domain list.
} NUMA_ACPI_DATA;


/**
  This function initializes the NUMA ACPI table data.

  @retval EFI_SUCCESS           This function is executed successfully.
  @retval EFI_UNSUPPORTED       NUMA mode is not enabled on the system.
**/
EFI_STATUS
EFIAPI
InitNumaAcpiData (
  VOID
  );

/**
  This function gets the number of assigned proximity domains from the NUMA
  ACPI table data buffer.

  @retval  Number of assigned proximity domains.
**/
UINT32
EFIAPI
GetProximityDomainNum (
  VOID
  );

/**
  This function retrieves the information of the specified proximity domain
  from the NUMA ACPI table data buffer. NULL will be returned if the specified
  domain index exceeds the allowed maximum number or the data buffer pointer can
  not be located.

  @param[in] DomainId      Proximity domain index.

  @retval Pointer to the specified proximity domain data buffer or NULL.
**/
PROXIMITY_DOMAIN*
EFIAPI
GetProximityDomain (
  IN UINT32        DomainId
  );

/**

  This function displays all the information of proximity domains.

**/
VOID
EFIAPI
DisplayProximityDomain (
  VOID
  );

#endif  // _NUMA_ACPI_TABLE_LIB_H_
