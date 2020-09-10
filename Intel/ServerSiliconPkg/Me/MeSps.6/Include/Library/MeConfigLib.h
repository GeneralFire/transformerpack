/** @file

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

#ifndef _ME_CONFIG_LIB_H_
#define _ME_CONFIG_LIB_H_

#include <Register/MeRegs.h>
#include <MeDefines.h>
#include <ConfigBlock.h>
#include <ConfigBlock/Sps/SpsPeiConfig.h>
#include <ConfigBlock/Sps/SpsDxeConfig.h>

typedef enum {
  ME_AUTO_CONFIG_HEC1         = HECI_FUNCTION_NUMBER,
  ME_AUTO_CONFIG_HEC2         = HECI2_FUNCTION_NUMBER,
  ME_AUTO_CONFIG_HEC3         = HECI3_FUNCTION_NUMBER,
  ME_AUTO_CONFIG_HEC4         = HECI4_FUNCTION_NUMBER,
  ME_AUTO_CONFIG_SOL          = SOL_FUNCTION_NUMBER,
  ME_AUTO_CONFIG_IDER         = IDER_FUNCTION_NUMBER
} ME_AUTO_CONFIG;


/**

  @brief
  Function resolves AutoSettings for particular configuration entry

  @param[in] Device               Device to resolve auto-configuration
  @param[in] ConfigToResolve      Auto-configuration to be resolved
  @param[in] MeIsCorporateAmt     True if it is corporate AMT running in ME

  @retval resolved auto-configuration

**/
UINT32
MeConfigDeviceAutoResolve(
  IN     ME_AUTO_CONFIG Device,
  IN     UINT32         ConfigToResolve,
  IN     BOOLEAN        MeIsCorporateAmt
  );

/**
  Function sets default values in SPS_PEI_CONFIG.

  @param[in] SpsPeiConfig      pointer to SPS_PEI_CONFIG
**/
VOID SetSpsPeiPolicyDefaults (
  IN     SPS_PEI_CONFIG       *SpsPeiConfig
  );

/**
  Function sets default values in SPS_DXE_CONFIG.

  @param[in] SpsPeiConfig      pointer to SPS_DXE_CONFIG
**/
VOID
SetSpsDxePolicyDefaults (
  IN     SPS_DXE_CONFIG       *SpsDxeConfig
  );

/**
  Function asks Client's ME if provisioning is active.
  Should be called for Client's ME only

  @retval TRUE    Provisioning is active.
  @retval FALSE   Provisioning is not active.
**/
BOOLEAN
ClientGetProvisioningState (
  VOID
  );

#endif // _ME_CONFIG_LIB_H_

