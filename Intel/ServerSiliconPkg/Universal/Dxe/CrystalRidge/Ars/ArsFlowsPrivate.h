/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016- 2020 Intel Corporation. <BR>

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

#ifndef _ARSFLOWSPRIVATE_H_
#define _ARSFLOWSPRIVATE_H_

#define ARS_DEBUG_STR CR_INFO_STR("ARS")
#define ARS_ERROR_STR CR_ERROR_STR("ARS")

#define PRINT_BOOL(x) ((x) ? L"True" : L"False")

#define START_ARS TRUE
#define STOP_ARS  FALSE

#define ARS_ALIGNMENT_SIZE             256

typedef enum _DIMM_ARS_STATE_ {
  NOT_STARTED,                // ARS operation on dimm not started
  IN_PROGRESS,                // ARS operation underway
  FINISHED,                   // ARS operation finished
  ENDED_PREMATURELY_OVERFLOW, // ARS stopped prematurely due to GetLongOpStatus buffer overflow
  ENDED_PREMATURELY_OTHER,    // ARS stopped prematurely due to other reasons
  ERROR                       // DIMM in error state
} DIMM_ARS_STATE;

typedef struct _SPA_INT_SET_ {
  UINT64      StartSpa;         // Starting address of the interleave set for the ARS operation
  UINT64      EndSpa;           // Ending address of the interleave set for the ARS operation
  UINT64      SpaBase;          // Base SAD address for SPA of the interleave set
  BOOLEAN     Enabled;          // Interleave set enabled for scrubbing
  BOOLEAN     Running;          // ARS running on the interleave set
  BOOLEAN     EndedPrematurely; // ARS operation ended prematurely for interleave set
  UINT8       DimmCount;
} SPA_INT_SET;

typedef struct _ARS_DIMM_ {
  UINT8           Socket;       // Socket ID
  UINT8           Channel;      // Channel ID, socket based
  UINT8           Dimm;         // Slot ID
  UINT64          StartDpa;     // Starting DPA address for the ARS
  UINT64          EndDpa;       // Ending DPA address for the ARS
  UINT64          CurrentSpa;   // Current SPA address of the scrub
  UINT64          SpaBase;      // Base SAD address for SPA of the region to scrub
  UINT64          DpaBase;      // Base DPA address for the region
  DIMM_ARS_STATE  State;        // ARS state on DIMM
  UINT8           ErrorsFound;  // Number of errors found
  UINT64          DpaErrorAddress[MAX_REPORTED_ERRORS_PER_DIMM]; // List of erronous addresses found
} ARS_DIMM;

typedef struct _ARS_DATA {
  UINT64              SystemStartSpa;     // SPA of the starting address for the ARS, system wise
  UINT64              SystemEndSpa;       // SPA of the ending address for the ARS, system wise
  UINT64              SystemRestartSpa;   // SPA of the last address scubbed before stopped prematurely
  BOOLEAN             InProgress;         // Is ARS underway
  BOOLEAN             EndedPrematurely;   // Is ARS stopped prematurely
  BOOLEAN             OverflowCondition;          // ErrorRecords was overflown
  UINT8               InterleaveSetsCount;        // Number of interleave set found in given SPA range
  UINT8               CurrentInterleaveSetIndex;  // Current interleave set scrubbed
  SPA_INT_SET         InterleaveSets[MAX_PMEM_INTERLEAVE_SET];  // List of interleave sets found
  UINT8               DimmsInCurrentIntSetCount;                // Number of dimms in current interleave set
  ARS_DIMM            DimmsInCurrentIntSetList[MAX_DCPMMS];  // List of dimms in current interleave set
  UINT16              NumberOfErrorRecords;                     // number of error records found
  ARS_ERROR_RECORD    ErrorRecords[MAX_REPORTED_ARS_ERRORS];    // error records array
} ARS_DATA;

/**
  Prototype for comparison function for ARS_ERROR_RECORD element types.

  @param[in] Record1                  The pointer to first buffer.
  @param[in] Record2                  The pointer to second buffer.

  @retval 0                           Record1 equal to Record2.
  @return <0                          Record1 is less than Record2.
  @return >0                          Record1 is greater than Record2.
**/
typedef
INTN
(*SORT_ERRORS_COMPARE) (
  IN CONST ARS_ERROR_RECORD *Record1,
  IN CONST ARS_ERROR_RECORD *Record2
  );

/**
  @brief Find scrubbable interleave-sets for given SPA range.

  Supports PMEM regions only. Narrows down *StartSpaPtr and *EndSpaPtr to align with
  the first and the last interleave set found. Never extends the range.

  @param[in,out] StartSpaPtr            - Starting SPA address of the range for search. On exit can be
                                          changed to reflect the start SPA of the first interleave-set
                                          if it pointed before the interleave-set base address.
  @param[in,out] EndSpaPtr              - Ending SPA address of the range for search. On exit can be
                                          changed to reflect the limit of the last interleave-set found
                                          if it pointed behind the interleave-set limit.
  @param[out]    InterleaveSetsTable    - Optional buffer for the found interleave sets.
  @param[in]     InterleaveSetsTableLen - Number of interleave sets to find, also
                                          length of buffer if provided.

  @returns Number of interleave sets found is returned. Zero is returned also for error.
**/
UINT8
GetScrubbableInterleaveSets (
  IN OUT UINT64            *StartSpaPtr,
  IN OUT UINT64            *EndSpaPtr,
  OPTIONAL OUT SPA_INT_SET *InterleaveSetsTable,
  IN     UINT8              InterleaveSetsTableLen
  );

/**

  Routine description: returns next enabled and not running interleave set to scrub.

  @param[in]  InterleaveSetsList    - List of interleave sets
  @param[in]  InterleaveSetsListLen - Length of InterleaveSetsList passed in
  @param[out] IntSetIndex           - Index of interleave set found

  @returns  Next interleve set found or NULL, if not found.

**/
SPA_INT_SET *
GetNextInterleaveSet (
  IN SPA_INT_SET      *InterleaveSetsList,
  IN UINT8            InterleaveSetsListLen,
  OUT UINT8           *IntSetIndex
  );

/**

  Routine description: finds all NvDimms in given interleave set.

  @param[in]  IntSet      - interleave set for which to find all dimms
  @param[out] DimmList    - List of dimms found
  @param[in]  DimmListLen - Length of DimmList array passed in

  @returns Number of dimms found, 0 also for error in parameters.

**/
UINT8
GetDimmsInInterleaveSet (
  IN CONST SPA_INT_SET *IntSet,
  OUT ARS_DIMM         *DimmList,
  IN UINT8             DimmListLen
  );

/**

  Routine description: starts ARS for interleave set and updates its state.
  Finds and updates the state for each of the Dimm found in interleave set.

  @param[in,out] IntSet      - Interleave set for which to start ARS
  @param[out]    DimmList    - List of the dimms found in interleave set
  @param[in]     DimmListLen - Length of DimmList array passed in
  @param[out]    DimmsCount  - Number of dimms found in interleave set

  @retval EFI_INVALID_PARAMETER One of parameters were invalid.
  @retval EFI_SUCCESS           ARS started successfully
  @retval other                 Other HW error

**/
EFI_STATUS
StartArsForInterleaveSet (
  IN OUT SPA_INT_SET  *IntSet,
  OUT ARS_DIMM        *DimmList,
  IN UINT8            DimmListLen,
  OUT UINT8           *DimmsCount
  );

/**

  Routine description: gets estimated time for ARS for all dimms. Gets the highest
  value from all the dimms in the list.

  @param[in] DimmList    - List of dimms
  @param[in] DimmListLen - Length of DimmList array passed in

  @returns Estimated time in seconds

**/
UINT32
GetEstimatedTimeForDimms (
  IN CONST ARS_DIMM    *DimmList,
  IN UINT8             DimmListLen
  );

/**

  Routine description: update the state for the dimm. Uses Get Long Operation
  Status.

  @param[in,out] ArsDimm - dimm to update

  @retval EFI_INVALID_PARAMETER Dimm not found or NULL given.
  @retval EFI_SUCCESS           Status updated.
  @retval other                 Other HW failure.

**/
EFI_STATUS
UpdateStatusOfDimm (
  IN OUT ARS_DIMM    *ArsDimm
  );

/**

  Routine description: finds a Nvdimm in Dimm list, given the location.

  @param[in]  DimmList    - List of the dimms
  @param[in]  DimmListLen - Length of DimmList array passed in
  @param[in]  Socket      - Socket ID (0 based)
  @param[in]  Channel     - Channel ID (0 based, socket based)
  @param[in]  Dimm        - Slot ID (0 based, channel based)

  @returns ARS_DIMM structure for the dimm found or NULL, if not found.

**/
ARS_DIMM *
GetDimmFromArsDimmList (
  IN ARS_DIMM   *DimmList,
  IN UINT8      DimmListLen,
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      Dimm
  );

/**

  Routine description: checks whether the ARS operation is finished on all given
  dimms.

  @param[in]  DimmList    - List of the dimms
  @param[in]  DimmListLen - Length of DimmList array passed in

  @returns TRUE if ARS is finished, FALSE otherwise.

**/
BOOLEAN
AreAllDimmsFinished (
  IN CONST ARS_DIMM    *DimmList,
  IN UINT8             DimmListLen
  );

/**

  Routine description: gets error records from the Nvdimm.

  @param[in]  ArsDimm           - dimm to get the errors from.
  @param[out] ErrorsFound       - number of errors found.
  @oaram[out] ErrorRecords      - list of error records found.
  @param[in]  ErrorRecordsLen   - length of ErrorRecords array passed in.

  @retval EFI_INVALID_PARAMETER Dimm not found or NULL given.
  @retval EFI_SUCCESS           Status updated.
  @retval other                 Other HW failure.

**/
EFI_STATUS
GetErrorRecordsFromDimm (
  IN CONST ARS_DIMM     *ArsDimm,
  OUT UINT16            *ErrorsFound,
  OUT ARS_ERROR_RECORD  *ErrorRecords,
  IN UINT16             ErrorRecordsLen
  );

/**

  Routine description: returns the state whether ARS for given dimm is in progress.

  @param[in]  ArsDimm           - dimm to check.

  @returns TRUE if in progress, FALSE otherwise.

**/
BOOLEAN
IsDimmInProgress (
  IN CONST ARS_DIMM   *ArsDimm
  );

/**

  Routine description: returns the state whether ARS for given dimm ended prematurely.

  @param[in]  ArsDimm           - dimm to check.

  @returns TRUE if ended prematurely, FALSE otherwise.

**/
BOOLEAN
IsDimmEndedPrematurely (
  IN CONST ARS_DIMM   *ArsDimm
  );

BOOLEAN
IsDimmOverflow (
  IN CONST ARS_DIMM   *ArsDimm
  );


/**

  Routine description: returns SPA address of the current location of ARS operation
  for given dimm.

  @param[in]  ArsDimm           - dimm to check.

  @returns SPA address or 0, when problem occured.

**/
UINT64
GetCurrentSpaFromDimm (
  IN CONST ARS_DIMM   *ArsDimm
  );

  /**

  Routine description: aborts the ARS for given dimms. Dimms' state in DimmsList
  is updated. Wrapper around SetArsForDimms.

  @param[in,out]  DimmsList - list of the dimms.
  @param[in]      DimmsCount - number of dimms in the list.

  @retval EFI_SUCCESS ARS successfully aborted.
  @retval other       ARS start failed. See status codes for NvmCtlrSetAddressRangeScrub.

**/
EFI_STATUS
AbortArsForDimms (
  IN OUT ARS_DIMM   *DimmsList,
  IN UINT8          DimmsCount
  );

/**
  @brief Get long operation status data for ARS operation from NVDIMM.

  @param[in]  Socket    - socket id (0 based)
  @param[in]  Channel   - channel id in socket (0 based)
  @param[in]  Dimm      - dimm id in channel (0 based)
  @param[out] ArsStatus - status for long operation

  @return Status returned by firmware, EFI_INVALID_PARAMETER when dimm not found
          or ArsStatus is NULL.
**/
EFI_STATUS
GetLongOperationStatusDataForArs (
  UINT8          Socket,
  UINT8          Channel,
  UINT8          Dimm,
  ARS_OP_STATUS *ArsStatus
  );

#endif // _ARSFLOWSPRIVATE_H_
