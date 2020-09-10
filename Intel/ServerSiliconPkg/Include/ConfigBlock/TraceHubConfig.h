/** @file
  PCH Trace Hub policy

@copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#ifndef _TRACEHUB_CONFIG_H_
#define _TRACEHUB_CONFIG_H_

#include <ConfigBlock.h>

#define CPU_TRACEHUB_PREMEM_CONFIG_REVISION 1
#define PCH_TRACEHUB_PREMEM_CONFIG_REVISION 1

extern EFI_GUID gPchTraceHubPreMemConfigGuid;

///
/// The TRACE_HUB_ENABLE_MODE describes TraceHub mode of operation
///
typedef enum {
  TraceHubModeDisabled       = 0,
  TraceHubModeTargetDebugger = 1,
  TraceHubModeHostDebugger   = 2,
  TraceHubModeMax
} TRACE_HUB_ENABLE_MODE;

///
/// The TRACE_BUFFER_SIZE describes the desired TraceHub buffer size
///
typedef enum {
  TraceBufferNone = 0,
  TraceBuffer1M = SIZE_1MB,
  TraceBuffer8M = SIZE_8MB,
  TraceBuffer64M = SIZE_64MB,
  TraceBuffer128M = SIZE_128MB,
  TraceBuffer256M = SIZE_256MB,
  TraceBuffer512M = SIZE_512MB
} TRACE_BUFFER_SIZE;

#pragma pack (push,1)

///
/// TRACE_HUB_CONFIG block describes TraceHub settings.
///
typedef struct {
  /**
  Trace hub mode. Default is disabled.
  Target Debugger mode refers to debug tool running on target device itself and it works as a conventional PCI device;
  Host Debugger mode refers to SUT debugged via probe on host, configured as ACPI device with PCI configuration sapce hidden.
  <b>0 = Disable</b>; 1 = Target Debugger mode; 2 = Host Debugger mode
  Refer to TRACE_HUB_ENABLE_MODE
  **/
  UINT32                     EnableMode;
  /**
  Trace hub memory buffer region size policy.
  The avaliable memory size options are: <b>0:0MB (none)</b>, 1:1MB, 2:8MB, 3:64MB, 4:128MB, 5:256MB, 6:512MB.
  Refer to TRACE_BUFFER_SIZE.
  **/
  UINT32                     MemReg0Size;
  UINT32                     MemReg1Size;
} TRACE_HUB_CONFIG;


typedef struct {
  CONFIG_BLOCK_HEADER        Header;           ///< Config Block Header
  TRACE_HUB_CONFIG           TraceHub;
} CPU_TRACE_HUB_PREMEM_CONFIG;

typedef struct {
  CONFIG_BLOCK_HEADER        Header;           ///< Config Block Header
  TRACE_HUB_CONFIG           TraceHub;
} PCH_TRACE_HUB_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _TRACEHUB_CONFIG_H_
