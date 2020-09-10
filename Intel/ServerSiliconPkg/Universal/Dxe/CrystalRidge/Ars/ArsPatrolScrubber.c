/** @file ArsPatrolScrubber.c
  Patrol Scrubber error handling

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2018 Intel Corporation. <BR>

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

#include <Uefi/UefiBaseType.h>
#include "CrystalRidge.h"
#include "Dsm.h"
#include "Ars/ArsFlows.h"
#include "Ars/ArsPatrolScrubber.h"

#define INDEX_NOT_FOUND ((UINT16)(~0))

STATIC ARS_ERROR_RECORD mArsPatrolScrubberError[MAX_PATROL_ERRORS];
STATIC UINT16           mArsPatrolScrubberErrorUsed;
STATIC UINT16           mArsPatrolScrubberFreeIndex = INDEX_NOT_FOUND;

/**
  Finds index of free item in patrol scrubber error array

  @param[out]  FreeIndex  Pointer to store found index under

  @retval EFI_SUCCESS    Free index found and stored under *FreeIndex.
  @retval EFI_NOT_FOUND  No free index found. Array is full.

**/
STATIC
EFI_STATUS
ArsPatrolScrubberFindFreeIndex (
  OUT UINT16 *FreeIndex
  )
{
  UINT16 Index;

  if (mArsPatrolScrubberErrorUsed < ARRAY_SIZE (mArsPatrolScrubberError)) {
    *FreeIndex = mArsPatrolScrubberErrorUsed;
    return EFI_SUCCESS;
  }
  if (mArsPatrolScrubberFreeIndex == INDEX_NOT_FOUND) {
    for (Index = 0; Index < mArsPatrolScrubberErrorUsed; Index++) {
      if (mArsPatrolScrubberError[Index].Length == 0) {
        mArsPatrolScrubberFreeIndex = Index;
        break;
      }
    }
  }
  if (mArsPatrolScrubberFreeIndex == INDEX_NOT_FOUND) {
    return EFI_NOT_FOUND;
  }
  *FreeIndex = mArsPatrolScrubberFreeIndex;
  return EFI_SUCCESS;
}

/**
  Adds SPA to Patrol Scrubber error list

  @param[in]  NfitHandle  DIMM NFIT handle
  @param[in]  Spa         SPA

  @retval  EFI_SUCCESS           Error added successfully
  @retval  EFI_BUFFER_TOO_SMALL  Error list is full

**/
EFI_STATUS
ArsPatrolScrubberAdd (
  IN UINT32 NfitHandle,
  IN UINT64 Spa
  )
{
  EFI_STATUS Status;
  UINT16     Index;

  Status = ArsPatrolScrubberFindFreeIndex (&Index);
  if (EFI_ERROR (Status)) {
    return EFI_BUFFER_TOO_SMALL;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "A: [%d] <- 0x%llx+0x%llx\n", Index, Spa, (UINT64) UNCORRECTABLE_UNIT_SIZE));
  mArsPatrolScrubberError[Index].NfitHandle = NfitHandle;
  mArsPatrolScrubberError[Index].SpaOfErrLoc = Spa;
  mArsPatrolScrubberError[Index].Length = UNCORRECTABLE_UNIT_SIZE;
  if (Index == mArsPatrolScrubberErrorUsed) {
    mArsPatrolScrubberErrorUsed++;
  }
  return EFI_SUCCESS;
}

/**
  Removes SPA from Patrol Scrubber error list

  @param[in]  Spa  SPA

  @retval  EFI_SUCCESS    Error removed successfully
  @retval  EFI_NOT_FOUND  SPA not found in the error list

**/
EFI_STATUS
ArsPatrolScrubberRemove (
  IN UINT64 Spa
  )
{
  UINT16 Index;

  for (Index = 0; Index < mArsPatrolScrubberErrorUsed; Index++) {
    if (mArsPatrolScrubberError[Index].Length == 0) {
      continue;
    }
    if (mArsPatrolScrubberError[Index].SpaOfErrLoc == Spa) {
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ArsPatrolScrubberRemove: R: [%d] <- +0x0\n", Index));
      mArsPatrolScrubberError[Index].Length = 0;
      mArsPatrolScrubberFreeIndex = Index;
      break;
    }
  }
  if (Index == mArsPatrolScrubberErrorUsed) {
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}

/**
  Retrieves entries matching specified range from Patrol Scrubber error list

  @param[in]      StartSpa      Filter start SPA
  @param[in]      EndSpa        Filter end SPA (inclusive)
  @param[out]     OutArray      Output array
  @param[in,out]  OutArraySize  Output array size, written back with number of copied entries

  @retval  EFI_SUCCESS           All found entries copied to OutArray
  @retval  EFI_BUFFER_TOO_SMALL  Output array too small (partial result is returned)

**/
EFI_STATUS
ArsPatrolScrubberGet (
  IN     UINT64           StartSpa,
  IN     UINT64           EndSpa,
     OUT ARS_ERROR_RECORD *OutArray,
  IN OUT UINT16           *OutArraySize
  )
{
  UINT16 Index;
  UINT16 Copied;

  if (StartSpa > EndSpa) {
    *OutArraySize = 0;
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("ARS") "ArsPatrolScrubberGet: %llx > %llx, returning %d errors\n", StartSpa, EndSpa, *OutArraySize));
    return EFI_INVALID_PARAMETER;
  }
  for (Index = 0, Copied = 0; Index < mArsPatrolScrubberErrorUsed && Copied < *OutArraySize; Index++) {
    if (mArsPatrolScrubberError[Index].Length == 0) {
      continue;
    }
    if (mArsPatrolScrubberError[Index].SpaOfErrLoc - 1 + mArsPatrolScrubberError[Index].Length < StartSpa ||
        mArsPatrolScrubberError[Index].SpaOfErrLoc > EndSpa) {
      continue;
    }
    CopyMem (&OutArray[Copied], &mArsPatrolScrubberError[Index], sizeof (OutArray[Copied]));
    Copied++;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ArsPatrolScrubberGet: returning %d/%d errors\n", Copied, *OutArraySize));
  if (Copied == *OutArraySize) {
    return EFI_BUFFER_TOO_SMALL;
  }
  *OutArraySize = Copied;
  return EFI_SUCCESS;
}

/**
  Adds multiple entries to Patrol Scrubber error list.
  Duplicates are ignored and their length is reset to 0 in ErrorRecord array.

  @param[in,out]  ErrorRecord     Array of entries to add
  @param[in]      ErrorRecordNum  Number of entries in the array

**/
EFI_STATUS
ArsPatrolScrubberAddMany (
  IN OUT ARS_ERROR_RECORD *ErrorRecord,
  IN     UINT16           ErrorRecordNum
  )
{
  UINT16 Index;
  UINT16 SrcIndex;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ArsPatrolScrubberAddMany: Start\n"));
  for (Index = 0; Index < mArsPatrolScrubberErrorUsed; Index++) {
    if (mArsPatrolScrubberError[Index].Length == 0) {
      continue;
    }
    for (SrcIndex = 0; SrcIndex < ErrorRecordNum; SrcIndex++) {
      if (ErrorRecord[SrcIndex].SpaOfErrLoc == mArsPatrolScrubberError[Index].SpaOfErrLoc &&
          ErrorRecord[SrcIndex].Length      == mArsPatrolScrubberError[Index].Length) {
        CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ArsPatrolScrubberAddMany: source 0x%llx+0x%llx already exists\n", ErrorRecord[SrcIndex].SpaOfErrLoc, ErrorRecord[SrcIndex].Length));
        ErrorRecord[SrcIndex].Length = 0;
      }
    }
  }
  for (SrcIndex = 0; SrcIndex < ErrorRecordNum; SrcIndex++) {
    if (ErrorRecord[SrcIndex].Length == 0) {
      continue;
    }
    ArsPatrolScrubberAdd  (ErrorRecord[SrcIndex].NfitHandle, ErrorRecord[SrcIndex].SpaOfErrLoc);
  }
  for (Index = 0; Index < mArsPatrolScrubberErrorUsed; Index++) {
    if (mArsPatrolScrubberError[Index].Length == 0) {
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ArsPatrolScrubberAddMany: [%d] == <empty>\n", Index));
    } else {
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ArsPatrolScrubberAddMany: [%d] == 0x%llx+0x%llx\n", Index, mArsPatrolScrubberError[Index].SpaOfErrLoc, mArsPatrolScrubberError[Index].Length));
    }
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ArsPatrolScrubberAddMany: End: [%d/%d]\n", Index, ARRAY_SIZE (mArsPatrolScrubberError)));
  return EFI_SUCCESS;
}
