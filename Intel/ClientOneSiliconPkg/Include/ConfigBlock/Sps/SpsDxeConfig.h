/** @file
  ME config block for DXE phase

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

#ifndef _SPS_DXE_CONFIG_H_
#define _SPS_DXE_CONFIG_H_

#include <ConfigBlock.h>
#include <MeDxeConfig.h>        // For ME_REPORT_ERROR definition
#include <SpsCommonConfig.h>

#define SPS_DXE_CONFIG_REVISION 3
extern EFI_GUID gSpsDxeConfigGuid;

#pragma pack (push,1)

typedef struct _SPS_DXE_CONFIG_ SPS_DXE_CONFIG;

typedef
EFI_STATUS
(EFIAPI *SPS_SAVE_POLICY) (
   SPS_DXE_CONFIG *This
  );

/**
  SPS Dxe Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.

  <b>Revision 2:</b>
  - Divide ConfigBlocks in to PEI, DXE and Common
  - remove references between ConfigBlocks in SPS

  <b>Revision 3:</b>
  - Add InvokeArbSvnCommit field
**/
typedef struct _SPS_DXE_CONFIG_ {
  CONFIG_BLOCK_HEADER   Header;                                      ///< Config Block Header

  // dword 0
  UINT32                DelayedAuthenticationMode              : 8;  ///< DAM option
  UINT32                DelayedAuthenticationModeOverride      : 1;  ///< DAM override option
  UINT32                MeHmrfpoViaHeci3                       : 1;
  UINT32                MeGrLockEnabled                        : 1;  ///< Suppress global reset locking for testing purpose
  UINT32                MeGrPromotionEnabled                   : 1;  ///< Promote upcoming reset to global reset
  UINT32                BreakRtcEnabled                        : 1;  ///< Test option for breaking RTC configuration
  UINT32                NmPowerMsmtOverride                    : 1;  ///< NM PTU power measurement test options
  UINT32                NmHwChangeOverride                     : 1;
  UINT32                NmPtuLoadOverride                      : 1;  ///< For MROM-less systems only
  UINT32                InvokeArbSvnCommit                     : 1;  ///< Send ARBH_SVN_COMMIT_NONCE command
  UINT32                DWord0Reserved                         : 15;

  // dword 1
  UINT16                SpsAltitude;
  UINT16                Word1Reserved2;

  // dword 2
  UINT32                MeHmrfpoEnableRegion;                         ///< Selector for opening a region

  // dword 3
  UINT32                DWord3Reserved                         : 32;

  // dword 4
  UINT32                DWord4Reserved                         : 32;

  // dword 5
  UINT32                DWord5Reserved                         : 32;

  // Common data
  SPS_COMMON_CONFIG     Common;

  /**
    Policy functions pointers
  **/
  ME_REPORT_ERROR       MeReportError;           // Function pointer for displaying error message on screen
  SPS_SAVE_POLICY       SpsSavePolicy;           // Save SPS Policy

} SPS_DXE_CONFIG;
#pragma pack (pop)

#endif // _SPS_DXE_CONFIG_H_
