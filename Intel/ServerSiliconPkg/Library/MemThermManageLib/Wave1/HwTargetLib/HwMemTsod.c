/** @file
  Implements Hardware version of Memory TSOD feature

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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
#include <SysHost.h>
#include <SysHostChip.h>
#include <Library/SysHostPointerLib.h>
#include <SysRegs.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/KtiApi.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemThermManageTargetLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/PcdLib.h>
#include <Library/BasicIpBlockServicesLib.h>

#include <Guid/MrcPlatformHooksGuid.h>

#include <ReferenceCodeFatalErrors.h>

/**
This function Initializes registers for Memory TSOD feature

@param[in]  Socket             - Socket Number
@param[in]  Channel            - Channel Number
@param[in]  Dimm               - Dimm Number

@retval     EFI_DEVICE_ERROR    Error in function execution
@retval     EFI_SUCCESS         Function executed successfully
**/

EFI_STATUS
EFIAPI
InitializeTsodTarget (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  )
{
  EFI_STATUS         Status;
  PSYSHOST           Host = NULL;
  SMB_DEVICE_STRUCT  Tsod;
  UINT16             MstData;
  THROTTLE_VALUE     ThrottleValue;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Host = GetSysHostPointer ();
  Tsod.compId = MTS;
  Tsod.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  Tsod.address.deviceType = DTI_TSOD;

  if (EFI_ERROR (GetSmbAddress (Socket, Channel, Dimm, &Tsod))) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Error retrieving DIMM SMBus address. Cannot initialize TSOD.\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
    return EFI_DEVICE_ERROR;
  }

#ifdef DEBUG_CODE_BLOCK
  if (ReadProcSmb (Tsod, MTS_MFGID, (UINT8 *)&MstData) != EFI_SUCCESS) {
    return EFI_DEVICE_ERROR;
  }
  if (ReadProcSmb (Tsod, MTS_DID_RID, (UINT8 *)&MstData) != EFI_SUCCESS) {
    return EFI_DEVICE_ERROR;
  }
  if (ReadProcSmb (Tsod, MTS_CAPABILITY, (UINT8 *)&MstData) != EFI_SUCCESS) {
    return EFI_DEVICE_ERROR;
  }
  if (ReadProcSmb (Tsod, MTS_TEMP, (UINT8 *)&MstData) != EFI_SUCCESS) {
    return EFI_DEVICE_ERROR;
  }
#endif //DEBUG_CODE_BLOCK
  GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, READ_THROTTLE_VALUES, EnumThrottlingDimmTempOffsetValue, &ThrottleValue);
  MstData = (((PcdGet8 (PcdMaxTCriticalValue)  - (UINT16)ThrottleValue.ThrottlingDimmTempOffsetValue) << 4) & 0x0FFF);
  if (WriteProcSmb (Tsod, MTS_CRITICAL, (UINT8 *)&MstData) != EFI_SUCCESS) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Initialize the config register
  //
  MstData = PcdGet16(PcdOemMtsConfigValue);
  if (WriteProcSmb (Tsod, MTS_CONFIG, (UINT8 *)&MstData) != EFI_SUCCESS ) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Added OEM hook for customer to do override
  //

  Status = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (Status)) {
    MrcPlatformHooksInterface->PlatformHookMst (Socket, Tsod, MTS_CONFIG, (UINT8 *)&MstData);
  }

  return EFI_SUCCESS;
}

