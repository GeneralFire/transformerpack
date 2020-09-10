/** @file

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

#ifndef _ARSPATROLSCRUBBER_H_
#define _ARSPATROLSCRUBBER_H_

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
  );

/**
  Removes SPA from Patrol Scrubber error list

  @param[in]  Spa  SPA

  @retval  EFI_SUCCESS    Error removed successfully
  @retval  EFI_NOT_FOUND  SPA not found in the error list

**/
EFI_STATUS
ArsPatrolScrubberRemove (
  IN UINT64 Spa
  );

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
  );

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
  );

#endif // !_ARSPATROLSCRUBBER_H_
