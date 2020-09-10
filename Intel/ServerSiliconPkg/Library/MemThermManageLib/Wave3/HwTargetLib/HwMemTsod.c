/** @file
  Implements Hardware version of DDR5 Memory TSOD feature

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

#include <Library/MemoryCoreLib.h>
#include <SysHost.h>
#include <SysHostChip.h>
#include <Library/SysHostPointerLib.h>
#include <SysRegs.h>
#include <Memory/I3cDefinitions.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/KtiApi.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemThermManageTargetLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/PcdLib.h>
#include <Library/BasicIpBlockServicesLib.h>

#include <ReferenceCodeFatalErrors.h>

/**
  This function Initializes registers for DDR5 Memory TSOD feature

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
  EFI_STATUS                    Status = EFI_SUCCESS;
  SMB_DEVICE_STRUCT             Tsod;
#ifdef DEBUG_CODE_BLOCK
  EFI_STATUS                    Ts0Status = EFI_SUCCESS;
  UINT8                         TempSensor;
  UINT16                        MstData;
  UINT8                         TsodDeviceTypeId[MAX_TS_PER_DIMM] = {DDR5_TS0, DDR5_TS1};
#endif // DEBUG_CODE_BLOCK

  if (EFI_ERROR (GetSmbAddress (Socket, Channel, Dimm, &Tsod))) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Error retrieving DIMM SMBus address. Cannot initialize TSOD.\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
    return EFI_DEVICE_ERROR;
  }

  //
  // Below code is going to Read the DIMM temperature sense to test the connection for debug purposes
  //
#ifdef DEBUG_CODE_BLOCK

  Tsod.compId = MTS;
  Tsod.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;

  for (TempSensor = 0; TempSensor < MAX_TS_PER_DIMM; TempSensor++) {

    // Store the return status from Temp Sensor 0 if not success
    if (EFI_ERROR (Status)) {
      Ts0Status = Status;
    }
    Status = EFI_SUCCESS;

    Tsod.address.deviceType = TsodDeviceTypeId[TempSensor];

    if (ReadProcSmb (Tsod, TS5_MR0_DEVICE_TYPE_MSB, (UINT8 *)&MstData) != EFI_SUCCESS) {
      Status = EFI_DEVICE_ERROR;
      continue;
    }
    if (ReadProcSmb (Tsod, TS5_MR2_DEVICE_REVISION, (UINT8 *)&MstData) != EFI_SUCCESS) {
      Status = EFI_DEVICE_ERROR;
      continue;
    }
    if (ReadProcSmb (Tsod, TS5_MR3_VENDOR_ID_0, (UINT8 *)&MstData) != EFI_SUCCESS) {
      Status = EFI_DEVICE_ERROR;
      continue;
    }
    if (ReadProcSmb (Tsod, TS5_MR49_TEMP, (UINT8 *)&MstData) != EFI_SUCCESS) {
      Status = EFI_DEVICE_ERROR;
      continue;
    }
  }  // TempSensor

  // Return EFI_DEVICE_ERROR if one of the TS failed
  if (EFI_ERROR (Status) || EFI_ERROR (Ts0Status)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "\nDEVICE ERROR returned to initialize DIMM TSOD: TS0: %r,  TS1: %r\n ", Ts0Status, Status);
    Status = EFI_DEVICE_ERROR;
  }

#endif // DEBUG_CODE_BLOCK

  //
  // Unlike previous generation, no more SPD register programming needed since there is no event pin on DDR5 modules.
  //

  return Status;
}

