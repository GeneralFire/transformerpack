/** @file
  BIOS Debug Message Tracing Catalog

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#ifndef __ENABLE_TRACE_HUB_DATA_H__
#define __ENABLE_TRACE_HUB_DATA_H__

#include <Library/TraceHubDebugLib.h>

typedef enum {
  TraceHubRoutingDisable = 0,
  TraceHubRoutingEnable,
  TraceHubRoutingMax
} TRACE_HUB_ROUTING;

typedef enum {
  TraceHubDebugLevelError = 0,
  TraceHubDebugLevelErrorWarning,
  TraceHubDebugLevelErrorWarningInfo,
  TraceHubDebugLevelErrorWarningInfoVerbose,
  TraceHubDebugLevelMax
} TRACE_HUB_DEBUG_LEVEL;

/**
  Determine whether to enable BIOS Debug Message tracing to Trace Hub.

  @param  SeverityType    The severity level of data.

  @retval TRUE            Enable trace hub data.
  @retval FALSE           Disable trace hub data.

**/
BOOLEAN
EFIAPI
EnableTraceHubData (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType
  );

#endif // __TRACE_HUB_DISCARD_DATA_H__
