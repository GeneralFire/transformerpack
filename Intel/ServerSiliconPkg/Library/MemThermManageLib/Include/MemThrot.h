/** @file
  Interface for generic throttling function

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_THROT_H_
#define _MEM_THROT_H_

//
// Bandwidth limit mask
//
#define BAND_WIDTH_LIMIT_THROTTLE_MASK 0x7FF

#define DRAM_THERMAL_DATA_MC_ENABLE_MASK (MAILBOX_BIOS_CMD_ENABLE_DRAM_THERMAL_DATA_MC_ENABLE_MASK_MC_0_MASK | MAILBOX_BIOS_CMD_ENABLE_DRAM_THERMAL_DATA_MC_ENABLE_MASK_MC_1_MASK | \
  MAILBOX_BIOS_CMD_ENABLE_DRAM_THERMAL_DATA_MC_ENABLE_MASK_MC_2_MASK | MAILBOX_BIOS_CMD_ENABLE_DRAM_THERMAL_DATA_MC_ENABLE_MASK_MC_3_MASK)

#define DRAM_THERMAL_DATA_MC_DISABLE_MASK (MAILBOX_BIOS_CMD_DISABLE_DRAM_THERMAL_DATA_MC_DISABLE_MASK_MC_0_MASK | MAILBOX_BIOS_CMD_DISABLE_DRAM_THERMAL_DATA_MC_DISABLE_MASK_MC_1_MASK | \
  MAILBOX_BIOS_CMD_DISABLE_DRAM_THERMAL_DATA_MC_DISABLE_MASK_MC_2_MASK | MAILBOX_BIOS_CMD_DISABLE_DRAM_THERMAL_DATA_MC_DISABLE_MASK_MC_3_MASK)

/**
  Control PM and thermal enable using Pcode

  @param[in] flag    Flag to indicate enable\disable Dram power and thermal management

  @retval   N/A

**/
VOID
ControlDramPMThermalViaPcode (
  IN UINT8     Flag
  );

/**

  This function checks for the dimm in thermal throttling mode and returns the result and the Index in the table

  @param[in] Socket    Socket number
  @param[in] Channel     Channel to test
  @param[out] DimmInThermalThrottlingMode   Result for search in table
  @param[out] TtmRowIndex                   Index in the table

  @retval    N/A

**/
VOID
CheckDimmInThermalThrottlingMode (
  IN UINT8 Socket,
  IN UINT8 Channel,
  OUT BOOLEAN *DimmInThermalThrottlingMode,
  OUT UINT8 *ThermalThrottlingRowIndex
  );

/**

  This function gets throttle window size

  @param[in] Socket    Socket number
  @param[in] Channel     Channel to test
  @param[in] TtmRow   Result for search in table
  @param[in] BwLimitTfMultiPlier                   Index in the table
  @param[out] BwLimitTfData       Data returned by the function

  @retval    N/A

**/
VOID
GetBwThrottleWindowSize (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 BwLimitTfMultiPlier,
  IN UINT32 *BwLimitTfData
  );

/**

  This function initializes temperature thershold values

  @param[in] Socket    Socket number
  @param[in] Channel     Channel Number
  @param[in] Dimm   Dimm Number
  @param[in] ThersholdHi    Thershold High value
  @param[in] DimmInTTModeTable    Result of dimm in TT mode table

  @retval    N/A

**/
VOID
InitalizeTempThreshold (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN UINT8 ThersholdHi,
  IN BOOLEAN DimmInTTModeTable
  );

/**

  This function issues PCODE command for Dram Power management

  @param[in]  Socket               Socket Number
  @param[in]  ThrottlingOption     CLTT_EN\CLTT_PECI_EN option

  @retval N/A

**/
VOID
IssueDramThermalManagementCommandToPcu (
  IN UINT8 Socket,
  IN UINT8 ThrottlingOption
  );

/**

  This function issues PCODE command for Dram Power management

  @param[in]  Socket               Socket Number

  @retval N/A

**/
VOID
IssueDramPowerManagementCommandToPcu (
  IN UINT8 Socket
  );

/**
  Check for 3ds dimm with 4H, 16Gb config

  @param[in]  Socket  - Socket number to program
  @param[in]  Channel - Channel number to program
  @param[in]  Dimm    - Dimm number to program

  @retval TRUE - Dimm is a 3DS 4H 16Gb dimm
  @retval FALSE - Other Dimms.
**/
BOOLEAN
Is3Ds4H16GbDimm  (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  );

#endif  // _MEM_THROT_H_
