/** @file ArsFlowsErrorInject.c
  Implementation of ARS Error Injection functionality

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
#include <Pi/PiSmmCis.h>
#include "CrystalRidge.h"
#include "Dsm.h"
#include "Ars/ArsErrorInject.h"
#include "Ars/ArsFlows.h"
#include "Ars/ArsFlowsPrivate.h"


#define MAX_INJECTED_ERROR_RECORDS  ((NVDIMM_MAX_OUTPUT_BUF_SIZE - sizeof (ROOT_DSM_FNOUT_ARS_ERROR_INJECT_STATUS_QUERY) - 4) / \
                                    sizeof (ARS_ERROR_INJECT_STATUS_QUERY_ERROR_RECORD))
#define RECORD_END_ADDRESS(_RECPTR) ((_RECPTR)->SpaRangeBase + (_RECPTR)->SpaRangeLength)

STATIC ARS_ERROR_INJECT_STATUS_QUERY_ERROR_RECORD *mErrorRecords;
STATIC UINT32                                     mUsedErrorRecords;

/**
  Initializes ARS Error Inject code.

  Currently it only allocates Error Records array.

  @param[in]  Smst                Pointer to EFI_SMM_SYSTEM_TABLE2.

  @retval     EFI_SUCCESS         Initialization OK.
  @retval     <EFI error status>  An error occurred. Couldn't allocate memory.

**/
EFI_STATUS
InitArsErrorInject (
  IN EFI_SMM_SYSTEM_TABLE2 *Smst
  )
{
  EFI_STATUS Status;

  if (Smst == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  Status = Smst->SmmAllocatePool (EfiRuntimeServicesData,
                   sizeof (mErrorRecords[0]) * MAX_INJECTED_ERROR_RECORDS,
                   &mErrorRecords);
  return Status;
}

/**
  Binary searches for first error record that overlaps specified address.

  If no record overlaps specified address, returns index of highest record below specified address.
  If all records are below specified address, returns first index beyond specified range.

  @param[in]  Address     Address to match.
  @param[in]  StartIndex  Search start (error records array index).
  @param[in]  NumRecords  Search range (number of records).

**/
STATIC
UINT32
ArsErrorInjectFindOverlapIndex (
  IN UINT64 Address,
  IN UINT32 StartIndex,
  IN UINT32 NumRecords
  )
{
  ASSERT (StartIndex + NumRecords <= MAX_INJECTED_ERROR_RECORDS);
  if (NumRecords == 0) {
    return StartIndex;
  }
  if (NumRecords == 1) {
    return Address >= RECORD_END_ADDRESS (&mErrorRecords[StartIndex]) ? StartIndex + 1 : StartIndex;
  }
  if (Address >= RECORD_END_ADDRESS (&mErrorRecords[StartIndex + NumRecords / 2])) {
    return ArsErrorInjectFindOverlapIndex (Address, StartIndex + NumRecords / 2 + 1, NumRecords - NumRecords / 2 - 1);
  }
  return ArsErrorInjectFindOverlapIndex (Address, StartIndex, NumRecords / 2);
}

/**
  Binary searches for insertion index for specified address so error records array remains sorted.

  @param[in]  StartAddress  Start address to insert.
  @param[in]  StartIndex    Search start (index to error records array).
  @param[in]  NumRecords    Search length (number of records).

**/
STATIC
UINT32
ArsErrorInjectFindInsertionIndex (
  IN UINT64 StartAddress,
  IN UINT32 StartIndex,
  IN UINT32 NumRecords
  )
{
  if (NumRecords == 0) {
    return StartIndex;
  }
  if (NumRecords == 1) {
    return StartAddress > mErrorRecords[StartIndex].SpaRangeBase ? StartIndex + 1 : StartIndex;
  }
  if (StartAddress > mErrorRecords[StartIndex + NumRecords / 2].SpaRangeBase) {
    return ArsErrorInjectFindInsertionIndex (StartAddress,
             StartIndex + NumRecords / 2 + 1,
             NumRecords - NumRecords / 2 - 1);
  }
  return ArsErrorInjectFindInsertionIndex (StartAddress,
           StartIndex,
           NumRecords / 2);
}

/**
  Searches for insertion index for specified range so error records array remains sorted.

  Returns insertion index.
  If specified range overlaps any existing record, returns (UINT32)-1.

  @param[in]  StartAddress  Range start.
  @param[in]  NumBlocks     Range length.

**/
STATIC
UINT32
ArsErrorInjectFindInsertionIndexCheckOverlap (
  IN UINT64 StartAddress,
  IN UINT64 NumBlocks
  )
{
  UINT64 EndAddress;
  UINT32 Index;

  EndAddress = StartAddress + NumBlocks * UNCORRECTABLE_UNIT_SIZE;
  Index = ArsErrorInjectFindInsertionIndex (StartAddress, 0, mUsedErrorRecords);
  if (Index < mUsedErrorRecords && EndAddress > mErrorRecords[Index].SpaRangeBase) {
    return (UINT32) -1;
  }
  if (Index > 0 && StartAddress < RECORD_END_ADDRESS (&mErrorRecords[Index - 1])) {
    return (UINT32) -1;
  }
  return Index;
}

/**
  Inserts new record into error records array.

  @param[in]  Index         Insertion index.
  @param[in]  StartAddress  New record start address.
  @param[in]  NumBlocks     New record length.

**/
STATIC
VOID
ArsErrorInjectInsertRecord (
  IN UINT32 Index,
  IN UINT64 StartAddress,
  IN UINT64 NumBlocks
  )
{
  ASSERT (mUsedErrorRecords < MAX_INJECTED_ERROR_RECORDS);
  if (Index < mUsedErrorRecords) {
    CopyMem (&mErrorRecords[Index + 1], &mErrorRecords[Index], sizeof (mErrorRecords[0]) * (mUsedErrorRecords - Index));
  }
  mErrorRecords[Index].SpaRangeBase = StartAddress;
  mErrorRecords[Index].SpaRangeLength = NumBlocks * UNCORRECTABLE_UNIT_SIZE;
  mUsedErrorRecords++;
}

/**
  Removes specified record from error records array.

  @param[in]  Index  Index of the record to remove.

**/
STATIC
VOID
ArsErrorInjectTrimRecord (
  IN UINT32 Index
  )
{
  ASSERT (Index < mUsedErrorRecords);
  CopyMem (&mErrorRecords[Index],
    &mErrorRecords[Index + 1],
    sizeof (mErrorRecords[0]) * (mUsedErrorRecords - (Index + 1)));
  mUsedErrorRecords--;
}

/**
  Injects errors of specified transaction type into specified SPA range.

  This function injects errors on all-or-nothing basis. If injection of complete range isn't possible,
  any partial injection is cleared and error status is returned.

  @param[in]  StartAddress          Range start.
  @param[in]  NumBlocks             Range length.
  @param[in]  MemType               Memory Transaction Type, see POISON_MEMORY_TYPE_* and FIS for details

  @retval     EFI_SUCCESS           Injection succeeded.
  @retval     EFI_INVALID_PARAMETER Range overlaps already injected error(s).
  @retval     EFI_BUFFER_TOO_SMALL  Reached maximal number of injected ranges. No errors injected.
  @retval     <EFI error status>    Failure. No errors injected.

**/
EFI_STATUS
ArsErrorInject (
  IN UINT64 StartAddress,
  IN UINT64 NumBlocks,
  IN UINT8  MemType
  )
{
  UINT32     Index;
  EFI_STATUS Status;
  UINT64     InjectedBlocks;

  if (NumBlocks == 0) {
    return EFI_SUCCESS;
  }
  Index = ArsErrorInjectFindInsertionIndexCheckOverlap (StartAddress, NumBlocks);
  if (Index == (UINT32) -1) {

    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "ArsErrorInject: range <0x%llx:0x%llx) overlaps with existing range\n",
      StartAddress,
      StartAddress + NumBlocks * UNCORRECTABLE_UNIT_SIZE));
    return EFI_INVALID_PARAMETER;
  }
  if (Index >= MAX_INJECTED_ERROR_RECORDS) {

    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "ArsErrorInject: record list full\n"));
    return EFI_BUFFER_TOO_SMALL;
  }
  Status = InjectErrorRange (StartAddress, NumBlocks, POISON_ENABLE_SET, MemType, &InjectedBlocks);
  if (EFI_ERROR (Status)) {

    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "ArsErrorInject: Injection failed (%r), reverting 0x%llx injected blocks\n", Status, InjectedBlocks));
    InjectErrorRange (StartAddress, InjectedBlocks, POISON_ENABLE_CLEAR, POISON_MEMORY_TYPE_CLEAR, NULL);
    return Status;
  }
  ArsErrorInjectInsertRecord (Index, StartAddress, InjectedBlocks);
  return EFI_SUCCESS;
}

/**
  Clears errors from specified SPA range.

  @param[in]  StartAddress          Range start.
  @param[in]  NumBlocks             Range length.
  @param[in]  DataOnly              TRUE:  operate on data structures only;
                                           do not clear error
                                    FALSE: normal operation

  @retval     EFI_BUFFER_TOO_SMALL  Internal data structure too small to accommodate
                                    new error list. No errors cleared.

**/
EFI_STATUS
ArsErrorInjectClear (
  IN  UINT64   StartAddress,
  IN  UINT64   NumBlocks,
  IN  BOOLEAN  DataOnly
  )
{
  UINT32     Index;
  EFI_STATUS Status = EFI_SUCCESS;
  UINT64     EndAddress;
  UINT64     SpaRangeBase;
  UINT64     SpaRangeEnd;
  UINT64     CurrentRecordNewSpaRangeEnd;
  UINT64     NewRecordSpaRangeBase;

  EndAddress = StartAddress + NumBlocks * UNCORRECTABLE_UNIT_SIZE;
  CRDEBUG ((DEBUG_INFO, "ArsErrorInjectClear: range: <0x%llx:0x%llx) [0x%llx]\n",
    StartAddress,
    EndAddress,
    EndAddress - StartAddress));
  for (Index = ArsErrorInjectFindOverlapIndex (StartAddress, 0, mUsedErrorRecords);
       Index < mUsedErrorRecords;
       Index++) {
    SpaRangeBase = mErrorRecords[Index].SpaRangeBase;
    SpaRangeEnd = RECORD_END_ADDRESS (&mErrorRecords[Index]);

    if (EndAddress <= SpaRangeBase) {
      break;
    }
    if (StartAddress >= SpaRangeEnd) {
      continue;
    }
    CurrentRecordNewSpaRangeEnd = StartAddress;
    if (CurrentRecordNewSpaRangeEnd < SpaRangeBase) {
      CurrentRecordNewSpaRangeEnd = SpaRangeBase;
      CRDEBUG ((DEBUG_INFO, "ArsErrorInjectClear: not clearing[1]: <0x%llx:0x%llx) [0x%llx]\n",
        StartAddress,
        SpaRangeBase,
        SpaRangeBase - StartAddress));
    }
    NewRecordSpaRangeBase = EndAddress;
    if (NewRecordSpaRangeBase > SpaRangeEnd) {
      NewRecordSpaRangeBase = SpaRangeEnd;
    }
    // current => <SpaRangeBase:CurrentRecordNewSpaRangeEnd)
    // hole    => <CurrentRecordSpaRangeEnd:NewRecordSpaRangeBase)
    // new     => <NewRecordSpaRangeBase:SpaRangeEnd)
    CRDEBUG ((DEBUG_INFO, "ArsErrorInjectClear: left split: <0x%llx:0x%llx) [0x%llx] (orig: <0x%llx:0x%llx) [0x%llx])\n",
      SpaRangeBase,
      CurrentRecordNewSpaRangeEnd,
      CurrentRecordNewSpaRangeEnd - SpaRangeBase,
      SpaRangeBase,
      SpaRangeEnd,
      SpaRangeEnd - SpaRangeBase));
    CRDEBUG ((DEBUG_INFO, "ArsErrorInjectClear: clear range: <0x%llx:0x%llx) [0x%llx]\n",
      CurrentRecordNewSpaRangeEnd,
      NewRecordSpaRangeBase,
      NewRecordSpaRangeBase - CurrentRecordNewSpaRangeEnd));
    CRDEBUG ((DEBUG_INFO, "ArsErrorInjectClear: right split: <0x%llx:0x%llx) [0x%llx]\n",
      NewRecordSpaRangeBase,
      SpaRangeEnd,
      SpaRangeEnd - NewRecordSpaRangeBase));
    if (CurrentRecordNewSpaRangeEnd - SpaRangeBase == 0) {
      if (SpaRangeEnd - NewRecordSpaRangeBase == 0) {
        // left and right go away
        ArsErrorInjectTrimRecord (Index);
        Index--;
      } else {
        // only left goes away
        mErrorRecords[Index].SpaRangeBase = NewRecordSpaRangeBase;
        mErrorRecords[Index].SpaRangeLength = SpaRangeEnd - NewRecordSpaRangeBase;
      }
    } else {
      if (SpaRangeEnd - NewRecordSpaRangeBase != 0) {
        // left and right stay
        if (mUsedErrorRecords == MAX_INJECTED_ERROR_RECORDS) {
          // splitting would overflow the list
          CRDEBUG ((DEBUG_ERROR, "ArsErrorInjectClear: splitting record would overflow the list!\n"));
          Status = EFI_BUFFER_TOO_SMALL;
          break;
        }
        mErrorRecords[Index].SpaRangeLength = CurrentRecordNewSpaRangeEnd - SpaRangeBase;
        ArsErrorInjectInsertRecord (Index + 1,
          NewRecordSpaRangeBase,
          (SpaRangeEnd - NewRecordSpaRangeBase) / UNCORRECTABLE_UNIT_SIZE);
        Index++;
      } else {
        // left stays
        mErrorRecords[Index].SpaRangeLength = CurrentRecordNewSpaRangeEnd - SpaRangeBase;
      }
    }
    // eveyrthing from CurrentRecordNewSpaRangeEnd to NewRecordSpaRangeBase needs to go
    if (!DataOnly) {
      InjectErrorRange (CurrentRecordNewSpaRangeEnd,
        (NewRecordSpaRangeBase - CurrentRecordNewSpaRangeEnd) / UNCORRECTABLE_UNIT_SIZE,
        0,
        0,
        NULL);
    }
    StartAddress = SpaRangeEnd;
    if (StartAddress >= EndAddress) {
      StartAddress = EndAddress;
    } else {
      // may be cleared by next iteration
      CRDEBUG ((DEBUG_INFO, "ArsErrorInjectClear: remaining: <0x%llx:0x%llx) [0x%llx]\n",
        StartAddress,
        EndAddress,
        EndAddress - StartAddress));
    }
  }
  if (StartAddress != EndAddress) {
    CRDEBUG ((DEBUG_INFO, "ArsErrorInjectClear: not clearing[2]: <0x%llx:0x%llx) [0x%llx]\n",
      StartAddress,
      EndAddress,
      EndAddress - StartAddress));
  }
  return Status;
}


/**
  Stores injected error records into specified array.

  Returns number of stored entries.

  @param[out]  OutputArray      Output array.
  @param[in]   OutputArraySize  Number of available elements in output array.

**/
UINT32
ArsErrorInjectStatusQuery (
  ARS_ERROR_INJECT_STATUS_QUERY_ERROR_RECORD *OutputArray,
  UINT32                                     OutputArraySize
  )
{
  UINT32 ToCopy;

  ToCopy = mUsedErrorRecords;
  if (ToCopy > OutputArraySize) {
    ASSERT (FALSE);
    ToCopy = OutputArraySize;
  }
  CopyMem (OutputArray, mErrorRecords, ToCopy * sizeof (mErrorRecords[0]));
  return ToCopy;
}
