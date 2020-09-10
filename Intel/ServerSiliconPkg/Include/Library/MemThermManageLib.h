/** @file
  API Interface header for Memory thermal\power  management feature

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

#ifndef _MEMORY_THERMAL_MANAGEMENT_LIB_H_
#define _MEMORY_THERMAL_MANAGEMENT_LIB_H_

//
// SMBUS Instance 0
//
#define SMBUS_INSTANCE_0               0

//Dimm in Thermal throttling table strcuture
typedef struct {
  BOOLEAN TableEntryPresent;       ///TRUE = Table entry present, FALSE = Table entry absent
  BOOLEAN CfgValid;               ///TRUE = Configuration valid, FALSE = Configuration invalid
} DIMM_IN_TABLE;

/**

  Initialize memory throttling

  @param[in] Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
InitThrottlingEarly (
  IN PSYSHOST  Host
  );

/**

  Initialize memory throttling

  @param[in] Host  - Point to sysHost

  @retval SUCCESS
**/
UINT32
EFIAPI
InitThrottlingLate (
  IN PSYSHOST  Host
  );

/**

  Initialize memory throttling

  @param[in] Host  - Point to sysHost

  @retval SUCCESS

**/
UINT32
InitThrottling (
  IN PSYSHOST  Host
  );

/**

  Initialize HBM memory throttling

  @param[in] Socket   - Socket Number

  @retval SUCCESS

**/
UINT32
HbmInitThrottling (
  IN UINT8   Socket
  );

/**

  This API programs 2X refresh functionality for HBM in current Socket
  pointed by SysHost Structure. Based on setup option the function
  can enable\disable 2x refresh programming.

  @retval N/A

**/
VOID
EFIAPI
HbmProgramTemperatureRefresh (
  VOID
  );

/**

  This function configures registers for Memory TSOD feature

  @param[in]  Socket               Socket Number

  @retval N/A

**/
VOID
EnableTsod (
  IN UINT8    Socket
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
EFIAPI
Is3Ds4H16GbDimm  (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm
  );
#endif  // ifndef _MEMORY_THERMAL_MANAGEMENT_LIB_H_
