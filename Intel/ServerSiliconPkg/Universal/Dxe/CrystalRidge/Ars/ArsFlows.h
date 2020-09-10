/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#ifndef _ARSFLOWS_H_
#define _ARSFLOWS_H_

#define SINGLE_INT_SET      TRUE
#define MULTIPLE_INT_SETS   FALSE

#define ARS_MAX_OUTPUT_BUF_SIZE (NVDIMM_MAX_OUTPUT_BUF_SIZE - sizeof (UINT32))
#define MAX_ERROR_LOGS          (((ARS_MAX_OUTPUT_BUF_SIZE) - sizeof (ROOT_DSM_FNOUT_QUERY_ARS_STATUS)) / sizeof (ARS_ERROR_RECORD))
#define MAX_REPORTED_ARS_ERRORS ((MAX_SOCKET) * (MAX_MC_CH) * (MAX_IMC) * (MAX_REPORTED_ERRORS_PER_DIMM))
#define MAX_PATROL_ERRORS       MAX_ERROR_LOGS

typedef struct _ARS_STATE_ {
  BOOLEAN             IsInProgress;                 // is ARS in progress
  BOOLEAN             StoppedPrematurely;           // is ARS has stopped prematurely
  BOOLEAN             OverflowCondition;            // ErrorRecords array was overflown
  UINT64              StartSpa;                     // start SPA address of current ARS operation
  UINT64              RestartSpa;                   // restart SPA address
  UINT64              EndSpa;                       // ending SPA address of current ARS operation
  UINT16              NumberOfErrorRecords;         // number of error records found
  ARS_ERROR_RECORD    ErrorRecords[MAX_ERROR_LOGS]; // error records array
} ARS_STATE;

/* ARS API */

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
  );

/**

  Routine description: checks whether the ARS operation is in progress

  @return TRUE if operation is underway, FALSE otherwise.

**/
BOOLEAN
ArsIsInProgress (
  VOID
  );


BOOLEAN
ArsIsOverflow (
  VOID
  );

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
  );

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
  );

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
  );

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
  );

/**

  Routine description: process the results for all NVDIMMs in current
  interleave set under ARS.

  @return VOID

**/
VOID
ArsProcessForAllDimms (
  VOID
  );

EFI_STATUS
ArsAbort (
  VOID
  );

/**

  Routine description: performs ARS on first interleave set found. Ars on boot
  knob must be enabled in setup menu.

  @return Void.
**/
VOID
ArsOnBoot (
  VOID
  );

/**

  Routine description: aligns the address for ARS usage.

  @param[in,out] Address       - address to align
  @param[in]     CeilAlignment - take the address aligned to the next alignment step

**/
VOID
AlignAddressForArs (
  IN OUT UINT64 *Address,
  IN BOOLEAN    CeilAlignment
  );
  

/**
  Reports if platform is capable of running background ARS

  @retval  TRUE   Platform is capable of running background ARS
  @retval  FALSE  Platform not capable of running background ARS
                  or background ARS administratively disabled
**/
BOOLEAN
ArsBackgroundCapable (
  VOID
  );

/**
  Reports background ARS status

  @retval  TRUE   Background ARS active
  @retval  FALSE  Background ARS inactive

**/
BOOLEAN
ArsInBackground (
  VOID
  );

/**
  Enables background ARS
**/
VOID
ArsBackgroundEnable (
  VOID
  );

/**
  Disables background ARS
**/
VOID
ArsBackgroundDisable (
  VOID
  );

/**
  Restarts interrupted ARS

  @retval  EFI_SUCCESS            ARS has been restarted
  @retval  EFI_INVALID_PARAMETER  ARS already running or completed
  @retval  other                  Other device failure

**/
EFI_STATUS
ArsRestart (
  VOID
  );

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
  );

#endif // _ARSFLOWS_H_
