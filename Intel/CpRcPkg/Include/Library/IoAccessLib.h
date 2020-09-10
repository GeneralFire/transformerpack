/** @file
  Interface header file for the IoAccess library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef _IO_ACCESS_LIB_H_
#define _IO_ACCESS_LIB_H_

#include <Uefi.h>
#include <SysHost.h>

//
// Units for use with TimeDiff function
//

#define  TDIFF_UNIT_S     1
#define  TDIFF_UNIT_MS    1000
#define  TDIFF_UNIT_US    1000000
#define  TDIFF_UNIT_NS    1000000000

/**

  Function to initialize the CPU timer

  @retval None

**/

VOID
InitCpuTimer (
  VOID
  );

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
  );

/**

  Reads 8 bit IO port with serial debug output

  @param[in] IoAddress  - IO Address to use

  @retval Value read from ioAddress

**/

UINT8
InPort8 (
  IN UINT16   IoAddress
  );

/**

  Reads 16 bit IO port with serial debug output

  @param[in] IoAddress - IO Address to use

  @retval Value read from ioAddress

**/
UINT16
InPort16 (
  IN UINT16   IoAddress
  );

/**

  Reads 32 bit IO port with serial debug output

  @param[in] IoAddress - IO Address to use

  @retval Value read from ioAddress

**/
UINT32
InPort32 (
  IN UINT16   IoAddress
  );

/**

  Writes 8 bit IO port with serial debug output

  @param[in] IoAddress - IO Address to use
  @param[in] Data      - Data to write to IO Address

  @retval None

**/

VOID
OutPort8 (
  IN UINT16   IoAddress,
  IN UINT8    Data
  );

/**

  Writes 16 bit IO port with serial debug output

  @param[in] IoAddress - IO Address to use
  @param[in] Data      - Data to write to IO Address

  @retval None

**/

VOID
OutPort16 (
  IN UINT16   IoAddress,
  IN UINT16   Data
  );

/**

  Writes 32 bit IO port with serial debug output

  @param[in] IoAddress - IO Address to use
  @param[in] Data      - Data to write to IO Address

  @retval None

**/

VOID
OutPort32 (
  IN UINT16   IoAddress,
  IN UINT32   Data
  );

/**

  Initialize the power management timer

  @param  None

  @retval None

**/

VOID
InitPmTimer (
  VOID
  );

/**

  Delay time in 1 us units

  @param[in] usDelay - number of us units to delay

  @retval None

**/
VOID
FixedDelayMicroSecond (
  IN UINT32 UsDelay
  );

/**

  Delay time in 1 us units

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] usDelay - number of us units to delay

  @retval None

**/
VOID
EFIAPI
FixedDelayMem (
  IN PSYSHOST Host,
  IN UINT32 UsDelay
  );

/**

  Gets the Time stamp counter value

  @param - None

  @retval 64-bit counter value

**/
UINT64
EFIAPI
GetCountTsc (
  VOID
  );

/**

  Determines the delay since GetPerformanceCounter was called

  @param[in] - 64-bit counter value from GetPerformanceCounter()

  @retval delay time in 1 us units

**/
UINT32
EFIAPI
GetDelayTsc (
  IN UINT64  StartTsc
  );

/**
  Get an environment-specific counter value.

  The units of the return value are dependent on the environment.

  @retval   The counter value
**/
UINT64
EFIAPI
GetCount (
  VOID
  );

/**
  Determines delay since the GetCount was called

  @param[in]  StartCount   A counter value returned by GetCount. (Units are environment-dependent.)

  @return Delay in microseconds
**/
UINT32
EFIAPI
GetDelay (
  IN  UINT64  StartCount
  );

/**

  Function converts a BCD to a decimal value.

  @param[in] BcdValue - An 8 bit BCD value

  @retval The decimal value of the BcdValue

**/
UINT8
BCDtoDecimal (
  IN UINT8 BcdValue
  );

/**

  Calculate Unix format timestamp which fits in 32 bit format for given time

  @param[in]  Time         - time data structure
  @param[out] NumOfSeconds - pointer to return calculated time

  @retval SUCCESS - calculation completed successfully
  @retval FAILURE - calculation failed

**/
UINT32
CalculateTimeStamp (
  IN  EFI_TIME Time,
  OUT UINT32   *NumOfSeconds
  );

/**

  Function for getting the system time from RTC to update System time for Memory devices
  (Format is number of seconds from current date to 1/1/1970 12:00:00

  @retval NumOfSeconds  - Return Number of seconds from Current date to 1/1/1970 12:00:00

**/
UINT32
GetTimeInSecondsFrom1970 (
  VOID
  );

/**
Routine Description:
  Retrieves system time

  @param[out] TimeInSecond - pointer to return calculated time

  @retval 0 SUCCESS
  @retval 1 FAILURE
**/
UINT8
GetSystemTime (
  OUT UINT32  *TimeInSecond
  );

#endif  // _IO_ACCESS_LIB_H_
