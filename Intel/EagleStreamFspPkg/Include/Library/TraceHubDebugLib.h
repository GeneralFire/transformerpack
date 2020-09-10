/** @file
  Definitions for the TraceHub Debug Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#ifndef __TRACE_HUB_DEBUG_LIB_H__
#define __TRACE_HUB_DEBUG_LIB_H__

typedef enum {
  SeverityNone = 0,
  SeverityFatal = 1,
  SeverityError = 2,
  SeverityWarning = 3,
  SeverityNormal = 4,
  SeverityUser1 = 5,
  SeverityUser2 = 6,
  SeverityUser3 = 7,
  SeverityMax
} TRACE_HUB_SEVERITY_TYPE;

/**
  Write data from buffer and Severity to TraceHub.

  Writes NumberOfBytes data bytes from Buffer and Severity to TraceHub.
  If Buffer is NULL and NumberOfBytes is larger than zero, then ASSERT().
  If NumberOfBytes is zero, no buffer is written.

  @param  SeverityType     Severity type of the written data buffer.
  @param  Buffer           Pointer to the data buffer to be written.
  @param  NumberOfBytes    Number of bytes to written to TraceHub device.

  @retval RETURN_SUCCESS   The number of bytes written to TraceHub.
  @retval Other            Fail to write the buffer to TraceHub.

**/
RETURN_STATUS
EFIAPI
TraceHubDebugWrite (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType,
  IN UINT8                      *Buffer,
  IN UINTN                      NumberOfBytes
  );

/**
  Write catalog64 message SVEN event with 6 parameters to TraceHub

  @param  SeverityType   Severity type of the catid message event.
  @param  Id             Catalog ID
  @param  Param1         Parameter 1 passed to catid message event.
  @param  Param2         Parameter 2 passed to catid message event.
  @param  Param3         Parameter 3 passed to catid message event.
  @param  Param4         Parameter 4 passed to catid message event.
  @param  Param5         Parameter 5 passed to catid message event.
  @param  Param6         Parameter 6 passed to catid message event.

**/
VOID
EFIAPI
TraceHubWriteCataLog64_6 (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType,
  IN UINT64                     Id,
  IN UINT32                     Param1,
  IN UINT32                     Param2,
  IN UINT32                     Param3,
  IN UINT32                     Param4,
  IN UINT32                     Param5,
  IN UINT32                     Param6
  );

#endif // _TRACE_HUB_DEBUG_LIB_H_
