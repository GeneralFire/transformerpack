/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef  _DDRT_CORE_API_H_
#define  _DDRT_CORE_API_H_

#include "SysHost.h"
#include <Library/UsraAccessApi.h>
#include <Library/TimerLib.h>
#include <Memory/FnvOpcode.h>
#include <Memory/FmcOptime.h>

// ********************************************************************************************************************
//                                                                                                                    *
// Public DDRT Core interfaces                                                                                        *
//                                                                                                                    *
// ********************************************************************************************************************

/**
  Detect DIMM population

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure
  @param[in] socket - Processor socket within the system (0-based)
  @param[in] ch     - DDR channel number within the processor socket (0-based)
  @param[in] dimm   - DIMM number within a DDR channel (0-based, farthest slot == 0)

  @retval SUCCESS

**/
UINT32
DetectAepDIMMConfig (
  IN  PSYSHOST    Host,   // Pointer to Host structure (outside scope of this spec)
  IN  UINT8       socket, // Processor socket within the system (0-based)
  IN  UINT8       ch,     // DDR channel number within the processor socket (0-based)
  IN  UINT8       dimm    // DIMM number within a DDR channel (0-based, farthest slot == 0)
  );

/**
  Calculate DDRT Dimm Size

  @param[in]  Density     - NVM Die Density with unit MBytes
  @param[in]  DataWidth   - NVM Data Width with unit bits
  @param[in]  DeviceType  - NVM Package Type and Loading
  @param[in]  ModuleOrg   - Module Organization

  @retval
**/
UINT32
CalculateDdrtDimmSize (
  IN UINT32   Density,
  IN UINT32   DataWidth,
  IN UINT8    DeviceType,
  IN UINT8    ModuleOrg
  );

/**

  Basic Fine ERID training uses a HW generated pattern from NVMCTLR

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
EarlyReadID (
  IN  PSYSHOST  Host    // Pointer to the system Host (root) structure
  );

/**

  Collects data from SPD for DDRT LRDIMM

  @param[in] Host   - Point to sysHost
  @param[in] Socket - CPU socket ID
  @param[in] Ch     - Channel to be accessed
  @param[in] Dimm   - DIMM to be accessed

  @retval SUCCESS

**/
UINT32
GetLrdimmSpdDataDdrt (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Dimm
  );

/**

  Trains Request Pin relative to clock

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
EarlyReqClk (
  PSYSHOST  Host
  );

/**

  Check if Current setup Freq limit same as DIMM init freq during warm reboot, and log an error if not same.

  @param[in] Host     - Point to sysHost
  @param[in] Socket   - CPU socket ID

  @retval None

**/
VOID
CheckDdrtFreqChange (
  IN PSYSHOST       Host,
  IN UINT8          Socket
  );
#endif // _DDRT_CORE_API_H_
