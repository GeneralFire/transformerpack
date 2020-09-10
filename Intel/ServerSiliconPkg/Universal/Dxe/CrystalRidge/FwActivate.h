/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef __FW_ACTIVATE_H__
#define __FW_ACTIVATE_H__

#define QUIESCING_AGENT_NONE 0
#define QUIESCING_AGENT_NVDIMM 1
#define QUIESCING_AGENT_FW_OS 2

#define DSM_ROOT_INTEL_GET_FW_RUNTIME_ACTIVATION_INFO_EXTSTATUS_ACTVATION_NOT_SUPPORTED 1
#define DSM_ROOT_INTEL_GET_FW_RUNTIME_ACTIVATION_INFO_EXTSTATUS_NO_DEVICES_ARMED        2

#define DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_SUCCESS 0
#define DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_NO_ARMED_DEVICES 1
#define DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_UNKNOWN_ERROR 2
#define DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_NO_NEW_FIRMWARE 3
#define DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_UNABLE_TO_MEET_MEMORY_TIMING_REQS 4
#define DSM_ROOT_INTEL_ACTIVATE_FIRMWARE_EXTSTATUS_UNABLE_TO_VERIFY_IO_DEVICES_IN_IDLE 5

#define DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_NONE 0
#define DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_SUCCESS 1
#define DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_NO_NEW_STAGED_FIRMWARE 2
#define DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_NVDIMM_RESET_REQUIRED 3
#define DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_MEDIA_DISABLED 4
#define DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_ACTIVATION_ABORTED_DUE_TO_THROTTLING 5
#define DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_FW_DOES_NOT_MEET_ACTIVATION_REQS 6
#define DSM_GET_DEVICE_RUNTIME_FW_ACTIVATION_STATUS_UNCLASSIFIED_FW_ACTIVATION_ERROR 7

#define PLATFORM_FIRMWARE_SUPPORTS_LIVE_FW_ACTIVATION                                BIT0
#define PLATFORM_FIRMWARE_SUPPORTS_OS_ASSISTED_IO_DEVICE_QUIESCE_BASED_FW_ACTIVATION BIT1
#define PLATFORM_FIRMWARE_SUPPORTS_ACTIVATION_FIRMWARE_ON_FAST_WARM_RESET            BIT2

#define DSM_INTEL_DEVICE_RUNTIME_FW_ACTIVATION_EXTSTATUS_NOT_SUPPORTED 1

#define PLATFORM_FW_MANAGED_IO_CPU_QUIESCE  0
#define OS_MANAGED_IO_CPU_QUIESCE           1

#define MEMORY_QUIESCE_TIME      500000 // us
#define BMC_QUIESCE_DIMM_PENALTY 100000 // us

#include "Pci.h"
#include <Protocol/CrFwActivate.h>

extern UINT8             mFwActivationSupported;
extern CTO_DEVICE_ENTRY *mCtoDeviceArray;
extern UINT32            mCtoDeviceArraySize;

extern CR_FW_ACTIVATE_PROTOCOL *mCrFwActivateProtocol;
extern CR_HANDLE               mCrFwActivateProtocolHandle;

EFI_STATUS
EFIAPI
AepFwActivateViaBmc (
  UINT32 QuiesceTimeoutValue,
  UINT32 DMATimeoutValue,
  UINT32 ActivationFlowEn,
  UINT8  IoDeviceQuiesceSource
  );

VOID
ArmAllDimms (
  );

UINT8
GetDimmState (
  IN UINT8 Socket,
  IN UINT8 Imc,
  IN UINT8 Channel,
  IN UINT8 Dimm
  );

EFI_STATUS
DsmRootIntelGetDevicesFwRuntimeActivationInfo (
  VOID
  );

EFI_STATUS
DsmRootIntelActivateFirmware (
  VOID
  );

EFI_STATUS
DsmGetDeviceRuntimeFwActivationInfo (
  VOID
  );

EFI_STATUS
DsmSetDeviceRuntimeFwActivationArmState (
  VOID
  );
BOOLEAN
FwActivateInitialize (
  VOID
  );

VOID
FwActivateAtEndOfDxe (
  VOID
  );

#endif // __FW_ACTIVATE_H__
