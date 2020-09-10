/** @file
  ME config block for PEI phase

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

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

#ifndef _SPS_PEI_CONFIG_H_
#define _SPS_PEI_CONFIG_H_

#include <ConfigBlock.h>
#include <SpsCommonConfig.h>

#define SPS_PEI_CONFIG_REVISION 2
extern EFI_GUID gSpsPeiPreMemConfigGuid;

#pragma pack (push,1)

/**
  ME Pei Pre-Memory Configuration Structure.

  <b>Revision 1:</b>
  - Initial version.

  <b>Revision 2:</b>
  - Divide ConfigBlocks in to PEI, DXE and Common
  - remove references between ConfigBlocks in SPS
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                 ///< Config Block Header
} SPS_PEI_PREMEM_CONFIG;
#pragma pack (pop)


extern EFI_GUID gSpsPeiConfigGuid;

#pragma pack (push,1)

/**
  SPS Pei Post-Memory Configuration Structure.

  <b>Revision 1:</b>
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                        ///< Config Block Header

  // dword 0
  UINT32                IderDeviceEnable         : 2;  ///< 0: Disabled; 1: Enabled; 2: Auto
  UINT32                HsioMessaging            : 1;  ///< <b>(Test)</b> <b>0: Disable</b>; 1: Enable - Enable/Disable HSIO synchronisation with ME.
  UINT32                HeciCommunication4       : 2;  ///< 0: Disabled; 1: Enabled; 2: Auto
  UINT32                DWord0Reserved           : 27;

  // dword 1
  UINT32                DWord1Reserved;

  // dword 2
  UINT32                PreDidMeResetEnabled     : 1;  ///< 0: disabled, 1: enabled
  UINT32                DWord2Reserved           : 31;

  // dword 3
  UINT32                MeTimeout                : 8;  ///< ME initialization timeout
  UINT32                DWord3Reserved1          : 5;
  UINT32                SpsIccClkSscSetting      : 1;  ///< Enable/disable spread spectrum
  UINT32                DWord3Reserved2          : 17;

  // dword 4
  UINT32                DWord4Reserved1          : 6;
  UINT32                FiaMuxMessageEnabled     : 1;
  UINT32                DWord4Reserved2          : 25;

  // dword 5
  UINT32                SolProvisioningState     : 1; ///< SOL last boot provisioning state (to use when no MBP on S3/S4)
  UINT32                DWord5Reserved           : 31;

  // Common data
  SPS_COMMON_CONFIG     Common;
} SPS_PEI_CONFIG;

#pragma pack (pop)

#endif // _SPS_PEI_CONFIG_H_
