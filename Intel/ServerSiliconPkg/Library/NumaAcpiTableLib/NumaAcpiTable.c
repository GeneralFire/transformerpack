/** @file
  This file implements the major algorithms of NumaAcpiTableLib

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/MemTypeLib.h>
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>
#include <Include/SysHost.h>
#include <Include/MemMapHost.h>
#include <Library/SysHostPointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/NumaAcpiTableLib.h>
#include "NumaAcpiTableInternal.h"

/**
  Memory Type Rank.
  1. Memory type rank is sorted based on the memory default pool policy of Windows OS and Linux OS.
  2. Memory types that are not listed below will be ignored by proximity domain assignment algorithm.
**/
STATIC CONST MEM_TYPE mMemTypeRank[] = {
  MemType2lmDdrCacheMemoryMode, MemType2lmHbmCacheMemoryMode, MemType2lmDdrWbCacheAppDirect,
  MemType1lmDdr,                MemType2lmHbmCacheDdr,        MemType1lmHbm,
  MemType1lmAppDirect,          MemTypeCxlExpVolatileMem,     MemTypeCxlExpPersistentMem
};


/**
  This function gets the rank of the given memory type.

  @param[in] MemType     Memory type.

  @retval  Rank of given memory type.
**/
UINT8
GetMemTypeRank (
  MEM_TYPE MemType
  )
{
  UINT8    Index;
  UINT8    MaxRank;

  MaxRank = sizeof (mMemTypeRank) / sizeof (MEM_TYPE);
  for (Index = 0; Index < MaxRank; Index++) {
    if (mMemTypeRank[Index] == MemType) {
      return Index;
    }
  }
  return MaxRank;
}

/**
  This function compares two SAD entries based on the memory rank to
  determine which SAD entry takes precedence.

  @param[in] SadEntry1      Pointer to the first SAD entry.
  @param[in] SadEntry2      Pointer to the second SAD entry.

  @retval  0                SadEntry1 equals to SadEntry2.
  @retval <0                SadEntry1 takes precedence.
  @retval >0                SadEntry2 takes precedence.
**/
INT8
CompareSadEntry (
  IN SAD_TABLE        *SadEntry1,
  IN SAD_TABLE        *SadEntry2
  )
{
  UINT8       Rank1;
  UINT8       Rank2;

  if ((SadEntry1 == NULL) || (SadEntry1->Enable == 0) || (!SadEntry1->local)) {
    Rank1 = sizeof (mMemTypeRank) / sizeof (MEM_TYPE);
  } else {
    Rank1 = GetMemTypeRank (SadEntry1->type);
  }

  if ((SadEntry2 == NULL) || (SadEntry2->Enable == 0) || (!SadEntry2->local)) {
    Rank2 = sizeof (mMemTypeRank) / sizeof (MEM_TYPE);
  } else {
    Rank2 = GetMemTypeRank (SadEntry2->type);
  }

  return Rank1 - Rank2;
}

/**
  This function checks if the given SAD entry can be merged to one memory proximity
  domain with other SAD entries that have the same precedence.

  @param[in] SadEntry      Pointer to the given SAD entry.

  @retval TRUE             SAD entries with same precedence is mergeable.
  @retval FALSE            SAD entries with same precedence is not mergeable.
**/
BOOLEAN
IsSadEntryMergeable (
  IN SAD_TABLE      *SadEntry
  )
{
  if (SadEntry == NULL) {
    return FALSE;
  }

  if (SadEntry->type == MemType1lmAppDirect) {
    return FALSE;
  }

  return TRUE;
}


/**
  This function adds memory proximity domain for the specified SAD entries.

  @param[in] SocketId                 Socket index.
  @param[in] ClusterId                Cluster index within socket.
  @param[in] SadBitMap                Bitmap of specified SAD entry indexes within cluster.
  @param[in, out] NextDomainId        Index of next available proximity domain.

  @retval EFI_SUCCESS                 This function is executed successfully.
  @retval EFI_OUT_OF_RESOURCES        Index exceeds the allowed maximum number.
**/
EFI_STATUS
AddMemoryProximityDomain (
  IN     UINT8            SocketId,
  IN     UINT8            ClusterId,
  IN     UINT32           SadBitMap,
  IN OUT UINT32           *NextDomainId
  )
{
  SYSHOST                 *Host;
  MEMORY_MAP_HOST         *MemMapHost;
  UINT16                  Tolm;
  UINT8                   SadId;
  UINT8                   BeginSadId;
  UINT8                   HighestBit;
  SAD_TABLE               *SadEntry;
  EFI_STATUS              Status;
  MEMORY_PROXIMITY_DOMAIN MemDomainData;

  if (SadBitMap == 0) {
    DEBUG ((DEBUG_WARN, "No memory proximity domain has been created since SadBitMap is 0\n"));
    return EFI_SUCCESS;
  }

  if (*NextDomainId >= MAX_NUMA_PROXIMITY_DOMAIN) {
    DEBUG ((DEBUG_ERROR, "Domain index exceeds the maximum number of NUMA proximity domains\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Host = GetSysHostPointer ();
  MemMapHost = GetMemMapHostPointer ();
  HighestBit = (UINT8) HighBitSet32 (SadBitMap);
  BeginSadId = (MemMapHost->NumberofChaDramClusters > 1) ? ClusterId * MemMapHost->MaxSadRules : 0;
  MemDomainData.MemRangeNum = 0;
  Tolm = GetTolmAddress ();

  for (SadId = 0; SadId <= HighestBit; SadId++) {
    if ((SadBitMap & (BIT0 << SadId)) == 0) {
      continue;
    }

    if (MemDomainData.MemRangeNum >= MAX_MEM_RANGE) {
      DEBUG ((DEBUG_ERROR, "MemRangeNum exceeds the maximum number of memory ranges per memory proximity domain\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    SadEntry = &Host->var.mem.socket[SocketId].SAD[SadId + BeginSadId];

    //
    // Remove MMIO hole for the SAD entry below 4GB.
    //
    if (SadEntry->Limit == FOUR_GB_MEM) {
      MemDomainData.LimitAddr[MemDomainData.MemRangeNum] = SadEntry->Limit - Tolm;
    } else {
      MemDomainData.LimitAddr[MemDomainData.MemRangeNum] = SadEntry->Limit;
    }

    MemDomainData.BaseAddr[MemDomainData.MemRangeNum] = SadEntry->Base;
    MemDomainData.MemType = SadEntry->type;
    MemDomainData.MemRangeNum++;
  }

  Status = AddProximityDomain (*NextDomainId, SocketId, ClusterId, PROXIMITY_DOMAIN_MEMORY, &MemDomainData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to add memory proximity domain %d, Status: %r\n", *NextDomainId, Status));
    return Status;
  }

  (*NextDomainId)++;

  return EFI_SUCCESS;
}

/**
  This function assigns the memory proximity domains for all system memory resources.

  @retval EFI_SUCCESS       This function is executed successfully.
  @retval !EFI_SUCCESS      This function is failed to be executed.
**/
EFI_STATUS
AssignMemoryProximityDomain (
  VOID
  )
{
  SYSHOST               *Host;
  MEMORY_MAP_HOST       *MemMapHost;
  NUMA_ACPI_DATA        *NumaAcpiData;
  UINT8                 SocketId;
  UINT8                 ClusterId;
  UINT8                 MaxCluster;
  UINT8                 SadId;
  UINT8                 BeginSadId;
  SAD_TABLE             *SadEntry;
  SAD_TABLE             *TopSadEntry;
  UINT32                TopSadBitMap;
  UINT32                CheckedSadBitMap[MAX_SOCKET][MAX_CLUSTERS];
  UINT32                PreMemDomainId;
  UINT32                NextMemDomainId;
  INT8                  ComparedResult;
  EFI_STATUS            Status;

  Host = GetSysHostPointer ();
  MemMapHost = GetMemMapHostPointer ();
  NumaAcpiData = GetNumaAcpiDataPointer ();
  if (NumaAcpiData == NULL) {
    DEBUG ((DEBUG_ERROR, "Cannot get the NUMA ACPI data pointer\n"));
    return EFI_LOAD_ERROR;
  }

  PreMemDomainId = (UINT32)-1;
  NextMemDomainId = 0;
  ZeroMem (CheckedSadBitMap, sizeof (CheckedSadBitMap));
  MaxCluster = IfFeatureEnabled (TypeSnc) ? GetNumOfClusterPerSystem () : 1;

  while (PreMemDomainId != NextMemDomainId) {
    PreMemDomainId = NextMemDomainId;
    for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
      if (!CheckSocketEnabled (SocketId)) {
        continue;
      }

      for (ClusterId = 0; ClusterId < MaxCluster; ClusterId++) {
        BeginSadId = (MemMapHost->NumberofChaDramClusters > 1) ? ClusterId * MemMapHost->MaxSadRules : 0;
        TopSadBitMap = 0;
        TopSadEntry = NULL;

        for (SadId = 0; SadId < MemMapHost->MaxSadRules; SadId++) {
          SadEntry = &Host->var.mem.socket[SocketId].SAD[SadId + BeginSadId];
          if ((SadEntry->Enable == 0) || CheckedSadBitMap[SocketId][ClusterId] == (UINT32)-1) {
            break;     // Break if there is no more available or unchecked SAD entry.
          }

          if ((SadEntry->Cluster != ClusterId) || ((CheckedSadBitMap[SocketId][ClusterId] & (BIT0 << SadId)) != 0)) {
            continue;  // Skip if SAD entry doesn't belong to the specified cluster or has been checked.
          }

          ComparedResult = CompareSadEntry (TopSadEntry, SadEntry);
          if (ComparedResult > 0) {
            TopSadBitMap = (BIT0 << SadId) ;   // Renew the top SAD entry.
            TopSadEntry = SadEntry;
          } else if ((ComparedResult == 0) && IsSadEntryMergeable (TopSadEntry)) {
            TopSadBitMap |= (BIT0 << SadId);   // Record SAD entries with the same precedence.
          }
        }

        if (TopSadBitMap == 0) {
          if (CheckedSadBitMap[SocketId][ClusterId] == 0) {
            //
            // Corner case: there is no available SAD entry in this cluster even NUMA mode is enabled.
            // Reserve a proximity domain for the processors of this cluster.
            //
            NextMemDomainId++;
          }
          CheckedSadBitMap[SocketId][ClusterId] = (UINT32)-1;
          continue;
        }

        Status = AddMemoryProximityDomain (SocketId, ClusterId, TopSadBitMap, &NextMemDomainId);
        if (EFI_ERROR (Status)) {
          return Status;
        }

        //
        // Record the index of assigned SAD entries.
        //
        CheckedSadBitMap[SocketId][ClusterId] |= TopSadBitMap;
      }  // Cluster loop.
    }  // Socket loop.
  }

  NumaAcpiData->ProximityDomainNum = MAX (NumaAcpiData->ProximityDomainNum, NextMemDomainId);
  return EFI_SUCCESS;
}

/**
  This function initializes the NUMA ACPI table data.

  @retval EFI_SUCCESS           This function is executed successfully.
  @retval EFI_UNSUPPORTED       NUMA mode is not enabled on the system.
**/
EFI_STATUS
EFIAPI
InitNumaAcpiData (
  VOID
  )
{
  EFI_STATUS            Status;

  if (!IsNumaAware ()) {
    DEBUG ((DEBUG_WARN, "\nNUMA ACPI table data initialization is skipped since NUMA mode is not enabled\n"));
    return EFI_UNSUPPORTED;
  }

  Status = SetNumaAcpiDataPointer ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Memory proximity domain assignment.
  //
  Status = AssignMemoryProximityDomain ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}
