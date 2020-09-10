/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/TimerLib.h>

/**

  Function to initialize the CPU timer

  @retval None

**/

VOID
InitCpuTimer (
  VOID
  )
{
  SetCpuFreq (GetPerformanceCounterProperties (NULL, NULL));
}

/**

  Finds the time delta between two Time Stamp Counter values in the given time units.

  @param[in] StartCount - First Time Stamp Counter value
  @param[in] EndCount   - Second Time Stamp Counter value
  @param[in] Unit       - For Time in Seconds (TDIFF_UNIT_S) = 1 \n
                          For Time in milliseconds (TDIFF_UNIT_MS) = 1000 \n
                          For Time in microseconds (TDIFF_UNIT_US) = 1000000 \n
                          For Time in nanoseconds (TDIFF_UNIT_NS) = 1000000000


  @retval Time delta in provided units

**/
UINT32
TimeDiff (
  IN UINT64    StartCount,
  IN UINT64    EndCount,
  IN UINT32    Unit
  )
{
  if (GetCpuFreq () == 0) {
    return 0;
  }
  return (UINT32) DivU64x64Remainder (MultU64x32 (EndCount - StartCount, Unit), GetCpuFreq (), NULL);
}
