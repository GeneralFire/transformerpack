/** @file

  Internal data structures for PerformanceTrackerLib.

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

#ifndef _PERFORMANCE_TRACKER_LIB_INTERNAL_H_
#define _PERFORMANCE_TRACKER_LIB_INTERNAL_H_

#include <Memory/PerformanceTrackerDefinitions.h>

//
// Associate performance tracker zone/phase with checkpoint codes
//
typedef struct _CHECK_POINT_TO_PHASE {
  UINT32        MajorCheckPoint;
  UINT32        MinorCheckPoint;
  PT_ZONE       Phase;
} CHECK_POINT_TO_PHASE;

#define PERF_TRACKER_CONTEXT_SIGNATURE 0x1F3F5F7F   // Random number we use to identify our context struct

typedef struct _PERF_TRACKER_CONTEXT {
  UINT32                    Signature;
  BOOLEAN                   PerfTrackerEnabled;
  PT_ZONE                   CurrentMemPhase;
  UINT8                     PerfTrackerActive;
  UINT16                    LastIndex;
  UINT16                    ZoneCount;
  UINT8                     NoZoneIndex;
  UINT8                     NoZoneActive;
  PERF_DATA                 Totals;
  PERF_DATA                 NoZone[MAX_NOZONE];
  PERFORMANCE_TRACKER_DATA  *PerformanceData;
} PERF_TRACKER_CONTEXT;

#endif // #ifndef _PERFORMANCE_TRACKER_LIB_INTERNAL_H_
