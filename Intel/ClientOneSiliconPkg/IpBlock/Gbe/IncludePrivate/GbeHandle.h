/** @file
  SATA routines

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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

@par Specification
**/
#ifndef _GBE_HANDLE_H_
#define _GBE_HANDLE_H_

#include <ConfigBlock.h>
#include <GbeController.h>
#include <GbeConfig.h>

/**
  Internal GBE policy options
**/
typedef struct {
  /**
    Corresponds to the "WOL Enable Override" bit in the General PM Configuration B (GEN_PMCON_B) register. 0: Disable; <b>1: Enable</b>.
   **/
  BOOLEAN               WolEnableOverride;
  /**
    Description: GBE controller interrupt pin
    Possible values: 0 - 255
  **/
  UINT8                 InterruptPin;

  /**
    Description: Lane number that is owned by GbE

    Value 0xFF means no lane assigned
  **/
  UINT8                 LaneNum;

  /**
   * Description: GBE PCIe source clock usage
   */
  BOOLEAN               HasClkReq;

  /**
   * Description: Sle flow enabled
   */
  BOOLEAN               SleEnabled;
} GBE_PRIVATE_CONFIG;

/**
  This function checks if GbE controller is supported (not disabled by fuse)

  @param[in]  GbeController      Pointer to GBE controller structure

  @retval GbE support state
**/
typedef
BOOLEAN
(*GBE_IS_SUPPORTED) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  This function checks if GbE is function disabled
  by static power gating

  @param[in]  GbeController      Pointer to GBE controller structure

  @retval GbE device state
**/
typedef
BOOLEAN
(*GBE_IS_DISABLED) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  This function enables GbE device by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.

  @param[in]  GbeController      Pointer to GBE controller structure
**/
typedef
VOID
(*GBE_ENABLE) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  This function disables GbE device by static power gating and enables ModPHY SPD gating (PCH-LP only).
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.

  @param[in]  GbeController      Pointer to GBE controller structure
**/
typedef
VOID
(*GBE_STATIC_DISABLE) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  This function enables GBE ModPHY SPD gating.

  @param[in]  GbeController  Pointer to GBE controller structure
**/
typedef
VOID
(*GBE_MOD_PHY_POWER_GATING) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  Disables one PCIe clock.

  @param[in] GbeController     Pointer to GBE controller structure
**/
typedef
VOID
(*GBE_DISABLE_CLK) (
  IN  GBE_CONTROLLER *GbeController
  );

/**
  GBE callbacks
  List of function pointers can passed to IB Block driver
**/
typedef struct {
  GBE_IS_SUPPORTED           IsSupported;
  GBE_IS_DISABLED            IsFunctionDisabled;
  GBE_ENABLE                 Enable;
  GBE_STATIC_DISABLE         Disable;
  GBE_MOD_PHY_POWER_GATING   ModPhyPowerGating;
  GBE_DISABLE_CLK            DisableClk;
} GBE_CALLBACK;

/**
  GBE device structure
  Stores all data necessary to initialize GBE IP block
**/
typedef struct {
  GBE_CONTROLLER             *Controller;
  GBE_CONFIG                 *GbeConfig;
  GBE_PRIVATE_CONFIG         *PrivateConfig;
  GBE_CALLBACK               *Callback;
  /**
    Temporary Bar
  **/
  UINT64                     TempBar;
} GBE_HANDLE;

#endif // _GBE_HANDLE_H_
