/** @file

  Interface header file for Performance Tracker library class.

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

#ifndef _PERFORMANCE_TRACKER_LIB_H_
#define _PERFORMANCE_TRACKER_LIB_H_

#include <Uefi.h>
#include <ReferenceCodeDataTypes.h>
#include <MemCommon.h>
#include <Memory/PerformanceTrackerDefinitions.h>

//
// Data type definitions for these functions are
// located in PerformanceTrackerDefinitions.h
//

/**

  Counts a performance tracker zone's accesses

  @param[in] Counter      - The performance counter
  @param[in] Data         - Data to add to counter

  @retval N/A

**/

VOID
EFIAPI
CountTrackingData (
  IN    PERFORMANCE_COUNTER     Counter,
  IN    UINT32                  Data
  );

/**

  Gets the current memory phase

  @param[out] MemPhase        - The current memory phase

  @retval MRC_STATUS_SUCCESS if memory phase is known
          MRC_STATUS_NOT_FOUND if unable to determine memory phase

**/

MRC_STATUS
EFIAPI
GetMemPhase (
  OUT PT_ZONE *Phase
  );

/**

  Sets the current memory phase

  @param[in] Phase - Current memory phase

  @retval MRC_STATUS_SUCCESS if memory phase set
          MRC_STATUS_UNSUPPORTED otherwise

**/

MRC_STATUS
EFIAPI
SetMemPhase (
  IN      PT_ZONE   Phase
  );

/**

  Sets the current memory phase based on checkpoint code

  @param[in] MajorCheckPoint  - Major Check Point to set
  @param[in] MinorCheckPoint  - Minor Check Point to set

  @retval MRC_STATUS_SUCCESS if memory phase is set
          MRC_STATUS_NOT_FOUND if unable to set memory phase;
          MRC_STATUS_UNSUPPORTED if performance tracker is not supported

**/

MRC_STATUS
EFIAPI
SetMemPhaseCP (
  IN    UINT8     MajorCheckPoint,
  IN    UINT8     MinorCheckPoint
  );

/**

  Clears the current memory phase

  @param[in] Phase - Current memory phase to clear

  @retval MRC_STATUS_SUCCESS if memory phase is cleared
          MRC_STATUS_UNSUPPORTED if performance tracker is not supported

**/

MRC_STATUS
EFIAPI
ClearMemPhase (
  IN      PT_ZONE   Phase
  );

/**

  Print statistics

  @param[in] Socket         - Socket
  @param[in] FmcMaxCached   - FMC Max Cached
  @param[in] FmcCachedReads - FMC Cached Reads

  @retval N/A

**/

VOID
EFIAPI
PrintPerformanceStats (
  IN UINT8 Socket,
  IN UINT8 FmcMaxCached,
  IN UINT32 FmcCachedReads
  );

/**

  Build Hob to be consumed in DXE to print performance statistics.

  @param None

  @retval MRC_STATUS_SUCCESS   - successfully built Hob containing performance tracker statistics
  @retval MRC_STATUS_NOT_FOUND - unable to locate performance tracker statistics

**/

MRC_STATUS
EFIAPI
BuildPerfStatsHob (
  VOID
  );

#endif // #ifndef _PERFORMANCE_TRACKER_LIB_H_
