/** @file
  Header file for DXE TraceHub Lib.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

#ifndef _DXE_TRACEHUB_LIB_H_
#define _DXE_TRACEHUB_LIB_H_

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
  The avaliable memory size options are: <b>0:0MB (none)</b>, 1MB, 8MB, 64MB, 128MB, 256MB, 512MB.
  Refer to TRACE_BUFFER_SIZE.
  **/
  UINT32                     MemReg0Size;
  UINT32                     MemReg1Size;
  UINT32                     PlatformDebugConsent;
  UINT8                      AetEnableMode;
} TRACE_HUB_INIT_CONFIGS;

/**
  Trace Hub initialization.

  @returns  Standard EFI_STATUS is returned.
**/
EFI_STATUS
IioTraceHubInitialize (
  VOID
  );

EFI_STATUS
PchTraceHubGetConfig (
  IN OUT  TRACE_HUB_INIT_CONFIGS        *PchTraceHubConfig
  );

#endif // _DXE_TRACEHUB_LIB_H_
