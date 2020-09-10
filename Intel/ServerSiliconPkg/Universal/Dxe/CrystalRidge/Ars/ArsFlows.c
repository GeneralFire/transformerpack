/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Chip/Include/FnvAccessCommon.h>

#include "CrystalRidge.h"
#include "Dsm.h"
#include "Ars/ArsFlows.h"
#include "Ars/ArsFlowsPrivate.h"
#include "Ars/ArsFlowsDebug.h"

#include "RasHelper.h"

#include "Ars/ArsPatrolScrubber.h"
#include "Ars/ShortArs.h"

#include "AcpiNotify.h"

#define IDLE_SCRUB_SPEED 0x77359400ULL


//
// Module variables
//
STATIC ARS_DATA  mArsData;  // main data structure for ARS flows
STATIC ARS_STATE mArsState; // ARS state for external consumers
STATIC BOOLEAN   mArsInBackground;

//
// Internal helper functions
//

/**

  Routine description: starts ARS for next interleave set, stored in mArsData.

  @retval EFI_NOT_FOUND No interleave sets found.
  @retval EFI_SUCCESS   Successfully started ARS.
  @retval other         Other HW error

**/
EFI_STATUS
StartArsForNextIntSet (
  VOID
  )
{
  EFI_STATUS      Status;
  SPA_INT_SET     *CurrentIntSet;
  BOOLEAN         EndedPrematurely;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "StartArsForNextIntSet()\n"));

  CurrentIntSet = GetNextInterleaveSet (
    mArsData.InterleaveSets,
    mArsData.InterleaveSetsCount,
    &mArsData.CurrentInterleaveSetIndex
    );

  if (CurrentIntSet == NULL) {
    return EFI_NOT_FOUND;
  }

  mArsData.SystemRestartSpa = CurrentIntSet->StartSpa;

  Status = StartArsForInterleaveSet (
    CurrentIntSet,
    mArsData.DimmsInCurrentIntSetList,
    ARRAY_SIZE (mArsData.DimmsInCurrentIntSetList),
    &mArsData.DimmsInCurrentIntSetCount
    );

  if (!EFI_ERROR (Status)) {
    mArsData.SystemRestartSpa = CurrentIntSet->EndSpa;
  } else {
    EndedPrematurely = CurrentIntSet->EndedPrematurely;
    ZeroMem (mArsData.InterleaveSets, sizeof (mArsData.InterleaveSets));
    mArsData.EndedPrematurely = EndedPrematurely;
  }


  return Status;
}

/**

  Routine description: comparison function for ARS_ERROR_RECORD type structs.
  Compares SpaOfErrLoc. Used for sorting.

  @param[in] Record1 - The pointer to first buffer.
  @param[in] Record2 - The pointer to second buffer.

  @retval 0            Record1 equal to Record2.
  @return <0           Record1 is less than Record2.
  @return >0           Record1 is greater than Record2.

**/
INTN
ArsErrorRecordCompare (
  IN CONST ARS_ERROR_RECORD *Record1,
  IN CONST ARS_ERROR_RECORD *Record2
  )
{
  return Record1->SpaOfErrLoc - Record2->SpaOfErrLoc;
}

/**

  Routine description: swaps the contents of error records in place.

  @param[in,out] Record1 - first record.
  @param[in,out] Record2 - second record.

**/
VOID
SwapErrorRecords (
  IN OUT ARS_ERROR_RECORD *Record1,
  IN OUT ARS_ERROR_RECORD *Record2
  )
{
  ARS_ERROR_RECORD TmpRecord;

  TmpRecord = *Record1;
  *Record1  = *Record2;
  *Record2  = TmpRecord;
}

/**

  Routine description: sorts error records using the CompareFunction.
  Implemented here, since Quicksort implementation from EDK's SortLib
  (PerformQuickSort) is not intended to use in SMM drivers.
  It's an implementation of the quicksort algorithm
  used in MdeModulePkg\Library\BaseSortLib\BaseSortLib.c.

  @param[in,out] BufferToSort - ARS error records buffer to sort
  @param[in]     Count        - ARS error records in the buffer
  @param[in]     CompareFunction - ARS error record comparison function

**/
VOID
SortErrors (
  IN OUT ARS_ERROR_RECORD *BufferToSort,
  IN UINTN                Count,
  IN SORT_ERRORS_COMPARE  CompareFunction
  )
{
  UINTN       Pivot;
  UINTN       LoopCount;
  UINTN       NextSwapLocation;

  if ((BufferToSort == NULL) || (CompareFunction == NULL)) {
    ASSERT (BufferToSort != NULL);
    ASSERT (CompareFunction != NULL);
    return;
  }

  if (Count < 2) {
    return;
  }

  // We chose pivot to be the last element
  Pivot = Count - 1;

  NextSwapLocation = 0;

  for (LoopCount = 0; LoopCount < Count - 1; LoopCount++) {

    if (CompareFunction (&BufferToSort[LoopCount], &BufferToSort[Pivot]) <= 0) {
      SwapErrorRecords (&BufferToSort[LoopCount], &BufferToSort[NextSwapLocation]);

      NextSwapLocation++;
    }
  }

  //
  // swap pivot to it's final position (NextSwapLocaiton)
  //
  SwapErrorRecords (&BufferToSort[NextSwapLocation], &BufferToSort[Pivot]);

  //
  // Now recurse on 2 paritial lists.  neither of these will have the 'pivot' element
  // IE list is sorted left half, pivot element, sorted right half...
  //
  if (NextSwapLocation >= 2) {
    SortErrors (
      &BufferToSort[0],
      NextSwapLocation,
      CompareFunction
      );
  }

  if ((Count - NextSwapLocation - 1) >= 2) {
    SortErrors (
      &BufferToSort[NextSwapLocation + 1],
      Count - NextSwapLocation - 1,
      CompareFunction
      );
  }
}

/**

  Routine descritption: add records to mArsData sturcture and perform sorting.

  @param[in]  ErrorRecords  - new error records to add
  @param[in]  Count         - number of error records in ErrorRecords array

  @retval EFI_INVALID_PARAMETER   Given parameters were invalid
  @retval EFI_BUFFER_TOO_SMALL    Too many errors to add (mArsData buffer size exceeded).
                                  None added
  @retval EFI_SUCCESS             Errors added successfully

**/
EFI_STATUS
AddErrorRecords (
  IN CONST ARS_ERROR_RECORD  *ErrorRecords,
  IN UINT16                  Count
  )
{
  EFI_STATUS Status;

  if (ErrorRecords == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Count == 0) {
    //
    // nothing to add
    return EFI_SUCCESS;
  }

  if (ARRAY_SIZE (mArsData.ErrorRecords) - mArsData.NumberOfErrorRecords >= Count) {
    CopyMem (
      &mArsData.ErrorRecords[mArsData.NumberOfErrorRecords],
      ErrorRecords,
      Count * sizeof (ErrorRecords[0])
      );

    mArsData.NumberOfErrorRecords += Count;

    SortErrors (
      mArsData.ErrorRecords,
      mArsData.NumberOfErrorRecords,
      ArsErrorRecordCompare
      );

    Status = EFI_SUCCESS;
  } else {
    Status = EFI_BUFFER_TOO_SMALL;
  }

  return Status;
}

//
// API functions
//

/**

  Routine descrition: checks whether the range contains scrubbable regions.

  @param[in] StartSpa - starting address to check
  @param[in] EndSpa   - ending address to check

  @return TRUE if region is scrubbable, FALSE otherwise.

**/
BOOLEAN
ArsIsScrubbable (
  IN UINT64 StartSpa,
  IN UINT64 EndSpa
  )
{
  //
  // We're looking for at least one scrubbable region
  if (GetScrubbableInterleaveSets (&StartSpa, &EndSpa, NULL, 1) != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**

  Routine description: checks whether the ARS operation is in progress

  @return TRUE if operation is underway, FALSE otherwise.

**/
BOOLEAN
ArsIsInProgress (
  VOID
  )
{
  return mArsData.InProgress;
}

BOOLEAN
ArsIsOverflow (
  VOID
  )
{
  return mArsData.OverflowCondition;
}

/**

  Calculates estimated time to ARS completion on all requested interleave sets.

  @returns Estimated time in seconds

**/
UINT32
ArsGetEstimatedTime (
  )
{
  UINT8               Index;
  UINT32              EstimatedTime = 0;

  for (Index = 0; Index < mArsData.InterleaveSetsCount; Index++) {
    if (mArsData.InterleaveSets[Index].DimmCount != 0) {
      EstimatedTime += (UINT32) ((mArsData.InterleaveSets[Index].EndSpa - mArsData.InterleaveSets[Index].StartSpa) / (UINT64) mArsData.InterleaveSets[Index].DimmCount / IDLE_SCRUB_SPEED);
    }
  }
  EstimatedTime += 1;

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR("ARS") "EstimatedTime: %d\n", EstimatedTime));
  return EstimatedTime;
}


/**

  Routine description: starts the ARS operation on given SPA range. Finds the
  scrubbable regions inside the given range. It could start the ARS on first
  interleave set found or on multiple sets. Currently supports scrubbing only
  the PMEM regions.

  @param[in]  StartSpa          starting address of region to scrub
  @param[in]  EndSpa            ending address of region to scrub
  @param[in]  SingleIntSt       if TRUE it performs the scrub only for the first
                                interleave set it finds. If FALSE, it searches
                                for multiple interleave sets.
  @param[out] EstimatedTime     returns estimated time of ARS operation in seconds
  @param[in]  UsePreviousData   TRUE: prepare matching data from previous scrub
                                      including patrol scrubber/short ARS errors;
                                      do not start ARS
                                FALSE: start regular ARS


  @retval EFI_INVALID_PARAMETER Parameters are invalid (EndSpa < StartSpa)
  @retval EFI_NOT_FOUND         Region is not scrubbable
  @retval EFI_ALREADY_STARTED   ARS operation already in progress
  @retval EFI_SUCCESS           ARS operation started successfully
  @retval other                 Other device related failure

**/
EFI_STATUS
ArsStart (
  IN UINT64           StartSpa,
  IN UINT64           EndSpa,
  IN BOOLEAN          SingleIntSet,
  OPTIONAL OUT UINT32 *EstimatedTime,
  IN BOOLEAN          UsePreviousData
  )
{
  EFI_STATUS  Status;
  UINT8       NumOfIntSets;
  UINT16      ErrorRecordsSize;

  if (EndSpa < StartSpa) {
    return EFI_INVALID_PARAMETER;
  }

  if (ArsIsInProgress () && !ArsInBackground ()) {
    return EFI_ALREADY_STARTED;
  }

  if (!ArsIsScrubbable (StartSpa, EndSpa)) {
    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "ArsStart: region not scrubbable!\n"));
    return EFI_NOT_FOUND;
  }

  ZeroMem (&mArsState, sizeof (mArsState));
  if (UsePreviousData || ArsIsInProgress ()) {
    ErrorRecordsSize = ARRAY_SIZE (mArsState.ErrorRecords);
    ArsPatrolScrubberGet (StartSpa, EndSpa, mArsState.ErrorRecords, &ErrorRecordsSize);
    mArsState.NumberOfErrorRecords = ErrorRecordsSize;
    mArsState.IsInProgress         = FALSE;
    mArsState.StoppedPrematurely   = FALSE;
    mArsState.StartSpa             = StartSpa;
    mArsState.RestartSpa           = EndSpa;
    mArsState.EndSpa               = EndSpa;
    mArsState.OverflowCondition    = FALSE;
    if (EstimatedTime) {
      *EstimatedTime = 0;
    }
    DsmUpdateArsStatusData (FALSE);
    return EFI_SUCCESS;
  }

  ZeroMem (&mArsData, sizeof (mArsData));

  NumOfIntSets = SingleIntSet ? 1 : ARRAY_SIZE (mArsData.InterleaveSets);

  mArsData.InterleaveSetsCount = GetScrubbableInterleaveSets (
    &StartSpa,
    &EndSpa,
    mArsData.InterleaveSets,
    NumOfIntSets
    );

  Status = StartArsForNextIntSet ();

  if (EFI_ERROR (Status)) {
    if (Status == EFI_NOT_FOUND) {
      CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "ArsStart: no interleave sets found in given SPA range!\n"));
    } else {
      CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "ArsStart: failure to start ARS for next interleave set!\n"));
    }
    mArsData.InProgress = FALSE;
    mArsData.SystemStartSpa = StartSpa;
    mArsData.SystemEndSpa = EndSpa;
  } else {
    mArsData.InProgress = TRUE;
    mArsData.SystemStartSpa = StartSpa;
    mArsData.SystemEndSpa = EndSpa;

    if (EstimatedTime != NULL) {
      *EstimatedTime = ArsGetEstimatedTime ();
    }
  }

  DumpArsData (&mArsData);

  // ArsStart modifies internal ARS State so we must update Query ARS Status Data
  if (!ArsInBackground ()) {
    DsmUpdateArsStatusData (TRUE);
  }
  return Status;
}

/**

  Returns current state of ARS.

  @param[out]  StatePtr  Address of pointer to a structure describing the state of ARS

  @retval EFI_INVALID_PARAMETER State is NULL
  @retval EFI_NOT_READY         ARS was not run in current boot
  @retval EFI_SUCCESS           State returned successfully

**/
EFI_STATUS
ArsState (
     OUT ARS_STATE **StatePtr,
  IN     BOOLEAN   UpdateState
  )
{
  if (StatePtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (UpdateState) {
    if (mArsData.SystemStartSpa == 0 && mArsData.SystemEndSpa == 0) {
      //
      // ARS was not performed yet.
      //
      return EFI_NOT_READY;
    }
    mArsState.IsInProgress         = ArsIsInProgress ();
    mArsState.StoppedPrematurely   = mArsData.EndedPrematurely;
    mArsState.StartSpa             = mArsData.SystemStartSpa;
    mArsState.RestartSpa           = mArsData.SystemRestartSpa;
    mArsState.EndSpa               = mArsData.SystemEndSpa;
    mArsState.OverflowCondition    = mArsData.OverflowCondition;

    mArsState.NumberOfErrorRecords = ARRAY_SIZE (mArsState.ErrorRecords);
    ArsPatrolScrubberGet (mArsData.SystemStartSpa, mArsData.SystemEndSpa, mArsState.ErrorRecords, &mArsState.NumberOfErrorRecords);
  }
  *StatePtr = &mArsState;
  return EFI_SUCCESS;
}


/**
  Removes error entry from ARS internal data.

  This is used to remove cleared errors from ARS.

  @param[in]  Spa  SPA address to remove

  @retval  EFI_SUCCESS    Entry removed.
  @retval  EFI_NOT_FOUND  An ARS is in progress or entry not found.

**/
EFI_STATUS
ArsRemoveSpaFromInternalData (
  IN UINT64 Spa
  )
{
  UINT16 Index;

  if (ArsIsInProgress () && !ArsInBackground ()) {
    return EFI_NOT_FOUND;
  }

  for (Index = 0; Index < mArsData.NumberOfErrorRecords; Index++) {
    if (mArsData.ErrorRecords[Index].SpaOfErrLoc == Spa) {
      break;
    }
  }
  if (Index == mArsData.NumberOfErrorRecords) {
    return EFI_NOT_FOUND;
  }
  CopyMem (&mArsData.ErrorRecords[Index], &mArsData.ErrorRecords[Index + 1], (mArsData.NumberOfErrorRecords - Index - 1) * sizeof (mArsData.ErrorRecords[Index]));
  mArsData.NumberOfErrorRecords--;

  return EFI_SUCCESS;
}

/**

  Routine description: updates the state of ARS for given DIMM.

  @param[in]  Socket    Socket ID for given DIMM (0 based)
  @param[in]  Channel   Channel ID (0 based, socket based)
  @param[in]  Dimm      Slot ID
  @param[in]  AbortAll  ARS Abort behavior when background ARS is active
                        TRUE:  Abort ARS on whole iset if ARS on specified DIMM
                               was stopped administratively
                        FALSE: Do not abort ARS

  @retval EFI_INVALID_PARAMETER Nvdimm was not found
  @retval EFI_NOT_READY         ARS on Dimm still in progress
  @retval EFI_SUCCESS           State updated
  @retval EFI_NOT_STARTED       Background ARS is active and ARS was aborted
                                on all iset member DIMMs
  @retval other                 Other HW failure

**/
EFI_STATUS
ArsProcessResults (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN BOOLEAN AbortAll
  )
{
  SPA_INT_SET       *CurrentIntSet;
  ARS_DIMM          *CurrentDimm;
  EFI_STATUS        Status;
  UINT16            ErrorRecordsFound = 0;
  UINT64            CurrentSpa;
  ARS_ERROR_RECORD  DimmErrorRecords[MAX_REPORTED_ERRORS_PER_DIMM];

  CurrentIntSet = &mArsData.InterleaveSets[mArsData.CurrentInterleaveSetIndex];

  if (!CurrentIntSet->Enabled || !CurrentIntSet->Running) {

    CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "S%d.C%d.D%d: ArsProcessResults: ARS is not running\n", Socket, Channel, Dimm));
    return EFI_SUCCESS;
  }
  //
  // Ensure DimmsInCurrentIntSetCount is not bigger than DimmsInCurrentIntSetList size
  //
  mArsData.DimmsInCurrentIntSetCount =
    (mArsData.DimmsInCurrentIntSetCount > ARRAY_SIZE (mArsData.DimmsInCurrentIntSetList)) ?
    ARRAY_SIZE (mArsData.DimmsInCurrentIntSetList) :
    mArsData.DimmsInCurrentIntSetCount;

  CurrentDimm = GetDimmFromArsDimmList (
    mArsData.DimmsInCurrentIntSetList,
    mArsData.DimmsInCurrentIntSetCount,
    Socket,
    Channel,
    Dimm);

  if (CurrentDimm == NULL) {

    CRDEBUG ((DEBUG_ERROR, ARS_ERROR_STR "S%d.C%d.D%d: ArsProcessResults: DIMM not found\n", Socket, Channel, Dimm));
    return EFI_INVALID_PARAMETER;
  }

  Status = UpdateStatusOfDimm (CurrentDimm);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsDimmInProgress (CurrentDimm)) {
    return EFI_NOT_READY;
  }

  //
  // Here we are certain the ARS operation is not running on the DIMM
  //

  //
  // Get and add error records to global error log
  //
  Status = GetErrorRecordsFromDimm (
    CurrentDimm,
    &ErrorRecordsFound,
    DimmErrorRecords,
    ARRAY_SIZE (DimmErrorRecords)
    );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if (ErrorRecordsFound > 0) {

    CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "S%d.C%d.D%d: ArsProcessResults: found %d new errors\n", Socket, Channel, Dimm, ErrorRecordsFound));
  }

  Status = AddErrorRecords (DimmErrorRecords, ErrorRecordsFound);

  //
  // Check and indicate premature end
  //
  if (IsDimmEndedPrematurely (CurrentDimm) || (Status == EFI_BUFFER_TOO_SMALL)) {
    CurrentSpa = GetCurrentSpaFromDimm (CurrentDimm);
    if (CurrentSpa < mArsData.SystemRestartSpa) {
      mArsData.SystemRestartSpa = CurrentSpa;
    }

    if (IsDimmOverflow (CurrentDimm)) {
      if (mArsData.EndedPrematurely == FALSE) {
        mArsData.OverflowCondition = TRUE;
      }
    } else {
      mArsData.OverflowCondition = FALSE;
    }

    if (Status == EFI_BUFFER_TOO_SMALL) {
      //
      // Restart SPA, should be the SPA of first error found, that was not added
      // to the buffer.
      //
      if (DimmErrorRecords[0].SpaOfErrLoc < mArsData.SystemRestartSpa) {
        mArsData.SystemRestartSpa = DimmErrorRecords[0].SpaOfErrLoc;
      }

      if (mArsData.EndedPrematurely == FALSE) {
        mArsData.OverflowCondition = TRUE;
      }
    }

    CurrentIntSet->EndedPrematurely = TRUE;
    mArsData.EndedPrematurely       = TRUE;

    Status = EFI_SUCCESS;
  }

  //
  // Check the global state of ARS and start the next interleave set (if present)
  //
  if (AreAllDimmsFinished (mArsData.DimmsInCurrentIntSetList, mArsData.DimmsInCurrentIntSetCount)) {

    CurrentIntSet->Running = FALSE;

    if (mArsData.EndedPrematurely) {

      CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "S%d.C%d.D%d: ArsProcessResults: all dimms finished, ended prematurely\n", Socket, Channel, Dimm));
      //
      // if ended prematurely, do not start next interleave set
      Status = EFI_NOT_FOUND;

    } else {

      CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "S%d.C%d.D%d: ArsProcessResults: all dimms finished, starting next int set\n", Socket, Channel, Dimm));
      //
      // we are done with this interleave set, get another one
      CurrentIntSet->Enabled = FALSE;
      Status = StartArsForNextIntSet ();

      if (Status == EFI_NOT_FOUND) {
        mArsData.SystemRestartSpa = mArsData.SystemEndSpa + 1;
      }
    }

    if (EFI_ERROR (Status)) {
      mArsData.InProgress = FALSE;
      DumpArsData (&mArsData);
      ArsPatrolScrubberAddMany (mArsData.ErrorRecords, mArsData.NumberOfErrorRecords);

      if (ArsInBackground ()) {
        if (mArsData.OverflowCondition == TRUE || mArsData.SystemRestartSpa == mArsData.SystemEndSpa + 1) {
          AcpiRootNotifyEx (TRUE);
        }
      } else {
        DsmUpdateArsStatusData (TRUE);
      }

      if (Status == EFI_NOT_FOUND) {
        CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "S%d.C%d.D%d: ArsProcessResults: scrub complete, all interleave sets done\n", Socket, Channel, Dimm));
        if (ArsInBackground ()) {
          if (mArsData.OverflowCondition == TRUE) {
            Status = ArsRestart ();
            if (EFI_ERROR (Status)) {
              ArsBackgroundDisable ();
            }
          } else {
            if (mArsData.SystemRestartSpa == mArsData.SystemEndSpa + 1) {
              ArsBackgroundDisable ();
            }
          }
        }
        return EFI_SUCCESS;
      }
    } else {
      DumpArsData (&mArsData);
    }
  } else {
    if (ArsInBackground () && IsDimmEndedPrematurely (CurrentDimm) && !IsDimmOverflow (CurrentDimm) && AbortAll) {
      ArsAbort ();
      return EFI_NOT_STARTED;
    }
  }

  return Status;
}

/**

  Routine description: process the results for all NVDIMMs in current
  interleave set under ARS.

  @return VOID

**/
VOID
ArsProcessForAllDimms (
  VOID
  )
{
  UINT8       i;
  UINT8       TotalDimms = mArsData.DimmsInCurrentIntSetCount;
  EFI_STATUS  Status;

  CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "ArsProcessForAllDimms - all NVDIMM's status check commencing...\n"));
  for (i = 0; i < TotalDimms; i++) {
    Status = NvmCtlrLongOpComplete (
      mArsData.DimmsInCurrentIntSetList[i].Socket,
      mArsData.DimmsInCurrentIntSetList[i].Channel,
      mArsData.DimmsInCurrentIntSetList[i].Dimm
      );

    CRDEBUG ((DEBUG_INFO, ARS_DEBUG_STR "S%d.C%d.D%d: ArsProcessForAllDimms - checking NVDIMM - %r\n",
      mArsData.DimmsInCurrentIntSetList[i].Socket,
      mArsData.DimmsInCurrentIntSetList[i].Channel,
      mArsData.DimmsInCurrentIntSetList[i].Dimm,
      Status
      ));
  }
}

EFI_STATUS
ArsAbort (
  VOID
  )
{
  return AbortArsForDimms (mArsData.DimmsInCurrentIntSetList, mArsData.DimmsInCurrentIntSetCount);
}

/**

  Routine description: performs ARS on first interleave set found. Ars on boot
  knob must be enabled in setup menu.

  @return Void.
**/
VOID
ArsOnBoot (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT64      ArsStartSpa = 0x0ULL;
  UINT64      ArsEndSpa = 0xFFFFFFFFFFFFFFFFULL;

  if (mSystemMemoryMap == NULL) {

    ASSERT (FALSE);
    return;
  }
  Status = CheckPreviousDcpmmError (NULL);
  if (EFI_ERROR (Status)) {

    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("ARS") "CheckPreviousPMemError() failed: %r\n", Status));
  }
  ShortArsGatherMediaLogErrors ();
  RunArsOnShortArsScrubList ();

  if ((mSystemMemoryMap->MemSetup.dfxMemSetup.dfxOptions & CR_SKIP_ARS_ON_BOOT) == 0) {
    //
    // ArsStart will find the first interleave set in the full 64bit memory range,
    // so no need to find it explicitly.
    //
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "Performing ARS on boot\n"));
    Status = ArsStart (ArsStartSpa, ArsEndSpa, MULTIPLE_INT_SETS, NULL, FALSE);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("ARS") "ARS on boot failed%a (%r)\n",
        (Status == EFI_NOT_FOUND) ? ", scrubbable region not found" : "", Status));
    }
  }
  DsmUpdateArsStatusData (TRUE);
}

/**

  Routine description: aligns the address for ARS usage.

  @param[in,out] Address       - address to align
  @param[in]     CeilAlignment - take the address aligned to the next alignment step

**/
VOID
AlignAddressForArs (
  IN OUT UINT64 *Address,
  IN BOOLEAN    CeilAlignment
  )
{
  if (CeilAlignment) {
    *Address += ARS_ALIGNMENT_SIZE - 1;
  }
  *Address = (*Address / ARS_ALIGNMENT_SIZE) * ARS_ALIGNMENT_SIZE;
}

/**
  Reports if platform is capable of running background ARS

  @retval  TRUE   Platform is capable of running background ARS
  @retval  FALSE  Platform not capable of running background ARS
                  or background ARS administratively disabled
**/
BOOLEAN
ArsBackgroundCapable (
  VOID
  )
{
  return (mSystemMemoryMap->MemSetup.dfxMemSetup.dfxOptions & CR_NO_BACKGROUND_ARS) == 0 && mCrInfo.DdrtAlertSmiEn != 0;
}

/**
  Reports background ARS status

  @retval  TRUE   Background ARS active
  @retval  FALSE  Background ARS inactive

**/
BOOLEAN
ArsInBackground (
  VOID
  )
{
  return mArsInBackground;
}

/**
  Enables background ARS
**/
VOID
ArsBackgroundEnable (
  VOID
  )
{
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ArsBackgroundEnable\n"));
  mArsInBackground = TRUE;
  mNvdimmAcpiSmmInterface->BackgroundArs = 1;
}

/**
  Disables background ARS
**/
VOID
ArsBackgroundDisable (
  VOID
  )
{
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("ARS") "ArsBackgroundDisable\n"));
  mArsInBackground = FALSE;
  mNvdimmAcpiSmmInterface->BackgroundArs = 0;
}

/**
  Restarts interrupted ARS

  @retval  EFI_SUCCESS            ARS has been restarted
  @retval  EFI_INVALID_PARAMETER  ARS already running or completed
  @retval  other                  Other device failure

**/
EFI_STATUS
ArsRestart (
  VOID
  )
{
  if (ArsIsInProgress () || mArsData.EndedPrematurely == FALSE) {
    return EFI_INVALID_PARAMETER;
  }
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR("ARS") "ArsRestart: restarting: <0x%llx:0x%llx>\n",
            mArsData.SystemRestartSpa, mArsData.SystemEndSpa));
  return ArsStart (mArsData.SystemRestartSpa, mArsData.SystemEndSpa, MULTIPLE_INT_SETS, NULL, FALSE);
}

/**
  Checks is specified DIMM is member of current ARS interleave set

  @param[in]  Socket  Socket ID
  @param[in]  Ch      Channel ID (within a socket)
  @param[in]  Dimm    DIMM ID (within a channel)

  @retval  TRUE   Specified DIMM is a member of current ARS interleave set
  @retval  FALSE  Specified DIMM is not a member of current ARS interleave set

**/
BOOLEAN
ArsDimmInCurrentIntSet (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm
  )
{
  UINT8 Index;

  for (Index = 0; Index < mArsData.DimmsInCurrentIntSetCount; Index++) {
    if (mArsData.DimmsInCurrentIntSetList[Index].Socket == Socket &&
        mArsData.DimmsInCurrentIntSetList[Index].Channel == Ch &&
        mArsData.DimmsInCurrentIntSetList[Index].Dimm == Dimm) {
      return TRUE;
    }
  }
  return FALSE;
}
