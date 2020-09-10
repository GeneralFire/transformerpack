/** @file
  ME config block for PEI/DXE phase

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

#ifndef _SPS_COMMON_CONFIG_H_
#define _SPS_COMMON_CONFIG_H_

#pragma pack (push,1)
/**
  SPS Pei/Dxec (Common) Configuration Structure.

  <b>Revision 1:</b>
  - Initial version.
**/

typedef struct {
  // dword 0
  UINT32 NmCores2Disable          : 7;
  UINT32 NmCores2DisableOverride  : 1;
  UINT32 NmPowerMsmtSupport       : 1;
  UINT32 NmHwChangeStatus         : 1;
  UINT32 NmPwrOptBootOverride     : 1;  ///< NM boot time policy test options
  UINT32 NmPwrOptBoot             : 1;
  UINT32 CoreBiosDoneEnabled      : 1;  ///< CoreBiosDone enable/disable for PEI (e.g. FSP)
  UINT32 MeWatchdogControlEnabled : 1;  ///< Enable DualBios Watchdog Control msg sending to ME
  UINT32 MeWatchdogDisable        : 1;  ///< Enable DualBios Watchdog unconditional disable
  UINT32 MeRegionSelectEnabled    : 1;  ///< Enable BiosUpdate RegionSelect msg sending to ME
  UINT32 Heci1HideInMe            : 2;  ///< 0: do not set, 1 - hide, 2 disable
  UINT32 Heci2HideInMe            : 2;
  UINT32 Heci3HideInMe            : 2;
  UINT32 Heci4HideInMe            : 2;
  UINT32 MeHmrfpoLockEnabled      : 1;  ///< Host ME Region Flash Protection Override test options
  UINT32 MeHmrfpoEnableEnabled    : 1;
  UINT32 DWord0Reserved           : 10;

  // dword 1
  UINT16 SpsMctpBusOwner;
  UINT16 DWord1Reserved;

  // dword 2
  UINT32 DWord2Reserved           : 32;

  // dword 3
  UINT32 DWord3Reserved           : 32;
} SPS_COMMON_CONFIG;
#pragma pack (pop)

#endif // _SPS_COMMON_CONFIG_H_
