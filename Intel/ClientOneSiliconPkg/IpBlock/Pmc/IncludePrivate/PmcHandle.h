/** @file
  PMC SoC configuration

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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#ifndef _PMC_CONTROLLER_H_
#define _PMC_CONTROLLER_H_

#include <PmcSocConfig.h>
#include <PmConfig.h>
#include <FivrConfig.h>

typedef struct _PMC_HANDLE PMC_HANDLE;

/**
  Configures which PM_SYNC messages are allowed to be
  sent over PM_SYNC interface.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PM_SYNC_INIT_MESSAGES) (
  IN PMC_HANDLE  *PmcHandle
  );

/**
  Perform SoC specific SLP_S0 configuration.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_SLPS0) (
  IN PMC_HANDLE *PmcHandle
  );

/**
  Configures modPHY SUS power gating.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_MODPHY_SUS_PG) (
  IN PMC_HANDLE  *PmcHandle
  );

/**
  Configure CPPM forced alignment after CPPM configuration
  has been done in PMC.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_CPPM_FORCE_ALIGNMENT) (
  IN PMC_HANDLE  *PmcHandle
  );

/**
  Called after CPU C10 gate is configured on PMC side to
  finish up feature configuration on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_POWER_BUTTON_DEBOUNCE) (
  IN PMC_HANDLE  *PmcHandle
  );

/**
  Called after CPU C10 gate is configured on PMC side to
  finish up feature configuration on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_CPU_C10_GATE) (
  IN PMC_HANDLE  *PmcHandle
  );

/**
  Called after PMC is done configuring VR alert to finish up
  init on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_VR_ALERT) (
  IN PMC_HANDLE  *PmcHandle
  );

typedef struct {
  PM_SYNC_INIT_MESSAGES                PmSyncInitMessages;
  PMC_CONFIGURE_SLPS0                  PmcConfigureSlpS0;
  PMC_CONFIGURE_MODPHY_SUS_PG          PmcConfigureModPhySusPg;
  PMC_CONFIGURE_CPPM_FORCE_ALIGNMENT   PmcConfigureCppmForceAlignment;
  PMC_CONFIGURE_POWER_BUTTON_DEBOUNCE  PmcConfigurePowerButtonDebounce;
  PMC_CONFIGURE_CPU_C10_GATE           PmcConfigureCpuC10Gate;
  PMC_CONFIGURE_VR_ALERT               PmcConfigureVrAlert;
} PMC_CALLBACK;

struct _PMC_HANDLE {
  UINT32          PwrmBase;
  PMC_CALLBACK    *PmcCallback;
  PMC_SOC_CONFIG  *PmcSocConfig;
  PCH_PM_CONFIG   *PmConfig;
};

typedef struct {
  UINT32               PwrmBase;
  PMC_FIVR_SOC_CONFIG  *PmcFivrSocConfig;
  PCH_FIVR_CONFIG      *FivrConfig;
  PCH_PM_CONFIG        *PmConfig;
} PMC_FIVR_HANDLE;

#endif

