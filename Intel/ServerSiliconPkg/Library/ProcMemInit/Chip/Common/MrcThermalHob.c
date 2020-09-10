/** @file

  Creates MRC Thermal Hob to be consumed in DXE phase

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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
#include "MrcThermalInclude.h"

/**
  Function to save MRC Thermal register information to HOB.

  @param[in] Host - Pointer to System Host data structure

  @retval None
**/
VOID
SaveMrcThermalInfoToHob (
  IN PSYSHOST Host
  )
{
  MRC_THERMAL_HOB   ThermalHob;
  UINT8             Socket;
  UINT8             Channel;
  UINT8             Dimm;
  THROTTLE_VALUE    ThrottleValue;

  DEBUG ((EFI_D_INFO, "Populating MRC Thermal Hob\n"));

  ZeroMem ((UINT8*)&ThermalHob, sizeof (MRC_THERMAL_HOB));
  ZeroMem ((UINT8*)&ThrottleValue, sizeof (THROTTLE_VALUE));

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Skip disabled sockets or If Socket does not have Dimms
    //
    if (!IsSocketDimmPopulated (Host, Socket)) {
      continue;
    }

    for (Channel = 0; Channel < MAX_CH; Channel++) {
      //
      // Skip disabled channels
      //
      if (!IsChannelEnabled (Socket, Channel)) {
        continue;
      }

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        //
        // Skip if DIMM is not present
        //
        if (!IsDimmPresent (Socket, Channel, Dimm)) {
          continue;
        }

        ThermalHob.DimmThrottleValues[Socket][Channel][Dimm].DimmValid = TRUE;

        GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, READ_THROTTLE_VALUES, EnumTempThersholdLow, &ThrottleValue);
        ThermalHob.DimmThrottleValues[Socket][Channel][Dimm].DimmTempThrottleThresholdLow = ThrottleValue.TempThersholdLow;

        GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, READ_THROTTLE_VALUES, EnumTempThersholdMid, &ThrottleValue);
        ThermalHob.DimmThrottleValues[Socket][Channel][Dimm].DimmTempThrottleThresholdMid = ThrottleValue.TempThersholdMid;

        GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, READ_THROTTLE_VALUES, EnumTempThersholdHigh, &ThrottleValue);
        ThermalHob.DimmThrottleValues[Socket][Channel][Dimm].DimmTempThrottleThresholdHigh = ThrottleValue.TempThersholdHigh;

        GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, READ_THROTTLE_VALUES, EnumThrottlingDimmTempOffsetValue, &ThrottleValue);
        ThermalHob.DimmThrottleValues[Socket][Channel][Dimm].DimmTempOffset = ThrottleValue.ThrottlingDimmTempOffsetValue;

        GetSetThrottleValues (MemTechDdr, Socket, Channel, Dimm, READ_THROTTLE_VALUES, EnumTempThrottleHysteresis, &ThrottleValue);
        ThermalHob.DimmThrottleValues[Socket][Channel][Dimm].DimmTempThrottleHysteresis = ThrottleValue.TempThrottleHysteresis;

        DEBUG ((EFI_D_INFO, "Socket = %d, Channel = %d, Dimm = %d,", Socket, Channel, Dimm));

        DEBUG ((EFI_D_INFO, " Threshold low = %x, Mid = %x, High = %x,",
            ThermalHob.DimmThrottleValues[Socket][Channel][Dimm].DimmTempThrottleThresholdLow,
            ThermalHob.DimmThrottleValues[Socket][Channel][Dimm].DimmTempThrottleThresholdMid,
            ThermalHob.DimmThrottleValues[Socket][Channel][Dimm].DimmTempThrottleThresholdHigh));
 
        DEBUG ((EFI_D_INFO, " Temperature Offset = %x, TempThrottleHysteresis = %x\n",
            ThermalHob.DimmThrottleValues[Socket][Channel][Dimm].DimmTempOffset,
            ThermalHob.DimmThrottleValues[Socket][Channel][Dimm].DimmTempThrottleHysteresis));
      }
    }
  }

  BuildGuidDataHob (&gMrcThermalHobGuid, &ThermalHob, sizeof(MRC_THERMAL_HOB));
  DEBUG ((EFI_D_INFO, "Completed populating MRC Thermal Hob\n"));
}