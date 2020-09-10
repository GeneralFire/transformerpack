/** @file
  Configures PCH McSmb Handle and allows McSmb device initialization

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

#include <Library/PeiMcSmbLib.h>
#include <Ppi/SiPolicy.h>
#include <Register/PchPcrRegs.h>
#include <Register/McSmbRegs.h>

/**
  Initializes McSmb device handle

  @param[in]  SiPolicy          Pointer to silicon policy structure
  @param[in]  McSmbHandle       Pointer to McSmb controller configuration structure

  @retval None
**/
VOID
McSmbHandleInit (
  IN  SI_POLICY_PPI       *SiPolicy,
  IN  MCSMB_HANDLE        *McSmbHandle
)
{
  MCSMB_CONTROLLER        *Controller;
  MCSMB_PRIVATE_CONFIG    *PrivateConfig;

  Controller                    = McSmbHandle->Controller;
  PrivateConfig                 = McSmbHandle->PrivateConfig;

  Controller->Pid               = PID_MCSMB;

  PrivateConfig->DeGlitchPulse  = V_PCH_MCSMB_PCR_SPGT_DG_PULSE_370NS;
  PrivateConfig->SpeedMode      = Standard1;

  PrivateConfig->TimingTsusto   = V_PCH_MCSMB_PCR_SPMT_TIMING_70NS;
  PrivateConfig->TimingTsusta   = V_PCH_MCSMB_PCR_SPMT_TIMING_70NS;
  PrivateConfig->TimingTbuf     = V_PCH_MCSMB_PCR_SPMT_TIMING_70NS;
  PrivateConfig->TimingThdsta   = V_PCH_MCSMB_PCR_SPMT_TIMING_70NS;
}

/**
  Initializes McSmb device

  @param[in]  SiPolicy          Pointer to silicon policy structure
**/
VOID
PchMcSmbInit (
  IN  SI_POLICY_PPI       *SiPolicy
)
{
  MCSMB_HANDLE            McSmbHandle;
  MCSMB_PRIVATE_CONFIG    McSmbPrivateConfig;
  MCSMB_CONTROLLER        McSmbController;

  McSmbHandle.Controller    = &McSmbController;
  McSmbHandle.PrivateConfig = &McSmbPrivateConfig;

  McSmbHandleInit (SiPolicy, &McSmbHandle);

  ConfigureMcSmb (&McSmbHandle);
}