/** @file
  Includes implementation for API calls of Memory Throttling\Power Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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
#include "SysHost.h"
#include "Chip10nm/Include/MemHostChip10nm.h"
#include "Include/MemThrot.h"
#include <SysHostChip.h>
#include <UncoreCommonIncludes.h>
#include <SysRegs.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/KtiApi.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemPwrTempTableLib.h>
#include <Library/MemThermManageLib.h>
#include <Include/MemTsod.h>
#include <Include/MemRapl.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/SpdDecodeLib.h>

//
//  Function Prototypes
//

/**

  Initialize memory throttling

  @param[in] Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
InitThrottlingEarly (
  IN PSYSHOST  Host
  )
{
  UINT8                                 Socket;
  UINT8                                 Channel;
  UINT8                                 mcId;
  UINT8                                 Dimm;
  THROTTLE_VALUE                        ThrottleValue;
  UINT8                                 MaxChDdr;
  UINT8                                 MaxImc;
  UINT8                                 OriginalUbiosOutputMode;
  SYS_SETUP                             *Setup = NULL;
  UINT32                                TsodPollRate = DEFAULT_TSOD_POLL_RATE;

  //
  // Lookup tables for per-DIMM register addresses
  //

  if (!IsMemFlowEnabled (EarlyInitThermalThrottling)) {
    return SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();
  MaxImc   = GetMaxImc ();
  Socket   = Host->var.mem.currentSocket;
  Setup = GetSysSetupPointer ();

  OriginalUbiosOutputMode = EnableI3cAsmOutput (TRUE);

  if ((Setup->mem.thermalThrottlingOptions & CLTT_PECI_EN) != 0) {
    //Set poll rate to 0 as per whitley white paper for CLTT_PECI_EN
    TsodPollRate = 0;
  }

  for (mcId = 0; mcId < MaxImc; mcId++) {
    if (Host->var.mem.socket[Socket].imcEnabled[mcId] == 0) {
      continue;
    }

    SetTsodPollRateSmb (Socket, GetProcSmbInstanceFromMcId (mcId), TsodPollRate);
    SetTsodPollEnSmb (Socket, GetProcSmbInstanceFromMcId (mcId), SMBUS_TSOD_DISABLE);
  } // mcId loop

  RestoreI3cAsmOutput (OriginalUbiosOutputMode, TRUE);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (!IsChannelValid (Host, Channel)) {
      continue;
    }

    //
    // Disable throttling during MRC training
    //
    DisableThrottling (Socket, Channel);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      //
      // Initialize the temp threshold to safe defaults
      //
      ThrottleValue.TempThersholdLow = DEFAULT_TEMP_THERSHOLD_LOW;
      ThrottleValue.TempThersholdMid = DEFAULT_TEMP_THERSHOLD_MID;
      ThrottleValue.TempThersholdHigh = DEFAULT_TEMP_THERSHOLD_HIGH;

      GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThersholdLow, &ThrottleValue);
      GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThersholdMid, &ThrottleValue);
      GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThersholdHigh, &ThrottleValue);
      //
      // Initialize threshold limits to maximum settings, effectively disabling bandwidth throttling
      //
      ThrottleValue.TempThrottleMid = DEFAULT_TEMP_THROTTLE_MID;
      ThrottleValue.TempThrottleHigh = DEFAULT_TEMP_THROTTLE_HIGH;
      ThrottleValue.TempThrottleCritical = DEFAULT_TEMP_THROTTLE_CRITICAL;
      GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThrottleMid, &ThrottleValue);
      GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThrottleHigh, &ThrottleValue);
      GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTempThrottleCritical, &ThrottleValue);

      //
      // Initialize TempOffset and 2xRefresh to default
      //
      ThrottleValue.ThrottlingDimmTempOffsetValue = 0;
      GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumThrottlingDimmTempOffsetValue, &ThrottleValue);

      //Program halfX refresh value
      ThrottleValue.HalfxRefresh = FALSE;
      GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumHalfxRefresh, &ThrottleValue);

      // Disable 2x Refresh
      ThrottleValue.TwoxRefresh = FALSE;
      GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumTwoxRefresh, &ThrottleValue);

      // Disable 4x Refresh
      ThrottleValue.FourxRefresh = FALSE;
      GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, WRITE_THROTTLE_VALUES, EnumFourxRefresh, &ThrottleValue);

    } // Dimm loop
  } // Channel loop

  return SUCCESS;
} //InitThrottlingEarly

/**
  Check for 3ds dimm with 4H, 16Gb config

  @param[in]  Socket  - Socket number to program
  @param[in]  Channel - Channel number to program
  @param[in]  Dimm    - Dimm number to program

  @retval TRUE - Dimm is a 3DS 4H 16Gb dimm
  @retval FALSE - Other Dimms.
**/
BOOLEAN
EFIAPI
Is3Ds4H16GbDimm  (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  )
{
  struct dimmNvram (*DimmNvList)[MAX_DIMM];
  UINT8            Density;
  UINT8            DeviceType;
  UINT8            DieCount;

  PSYSHOST Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  if ((*DimmNvList)[Dimm].dimmPresent != 0) {
    Density = (*DimmNvList)[Dimm].sdramCapacity;
    DeviceType = (*DimmNvList)[Dimm].SPDDeviceType & SPD_SIGNAL_LOADING;
    DieCount = (*DimmNvList)[Dimm].dieCount;

    // Disable 2x Refresh for 4H, 16Gb,4H (For RDIMM or UDIMM)
    if (( Density == SPD_16Gb) && (DeviceType == SPD_3DS_TYPE) && (DieCount == 4)) {
      return TRUE;
    }
  }

  return FALSE;
}

