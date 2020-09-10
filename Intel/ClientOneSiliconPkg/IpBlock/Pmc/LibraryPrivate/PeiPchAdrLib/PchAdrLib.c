/** @file
  PCH ADR setup library

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Register/PmcRegs.h>
#include <Register/PmcRegsVer3.h>
#include <AdrConfig.h>
#include <Library/PchAdrLib.h>
#include "PchAdrConfig.h"
#include <Library/PmcLib.h>

/**
  Configures ADR in the PMC depending on policy

  @param[in] PchAdrConfig     PCH ADR configuration structure
  @param[in] PmcAdrSocConfig  Pointer to the SoC configuration of the ADR
**/
VOID
PchAdrConfigurePmc (
  IN ADR_CONFIG          *PchAdrConfig,
  IN PMC_ADR_SOC_CONFIG  *PmcAdrSocConfig
  )
{
  UINT32 PchPwrmBase;
  UINT32 OrMask;
  UINT32 AndMask;

  PchPwrmBase = PmcGetPwrmBase ();

  if (PmcAdrSocConfig->AdrPhaseType == AdrDualPhase) {
    OrMask = B_PMC_PWRM_ADR_GEN_CFG_ADR_P2_EN;
    AndMask = (UINT32) ~0;
  } else {
    OrMask = 0;
    AndMask = (UINT32) ~B_PMC_PWRM_ADR_GEN_CFG_ADR_P2_EN;
  }

  if (PmcAdrSocConfig->AdrGenCfgMoved) {
    MmioAndThenOr32 (
      PchPwrmBase + R_PMC_PWRM_ADR_GEN_CFG_V2,
      AndMask,
      OrMask
     );
  } else {
    MmioAndThenOr32 (
      PchPwrmBase + R_PMC_PWRM_ADR_GEN_CFG,
      AndMask,
      OrMask
     );
  }

  if (PchAdrConfig->AdrHostPartitionReset == FORCE_ENABLE) {
    OrMask = B_PMC_PWRM_ADR_EN_HPR_ADR_EN;
    AndMask = (UINT32) ~0;
  } else {
    OrMask = 0;
    AndMask = (UINT32) ~(B_PMC_PWRM_ADR_EN_HPR_ADR_EN);
  }

  //
  // GPIO selection is only valid in single phase ADR
  //
  if (PmcAdrSocConfig->AdrPhaseType == AdrSinglePhase) {
    AndMask &= (UINT32) ~(B_PMC_PWRM_ADR_GPIO_SEL);
    switch (PmcAdrSocConfig->AdrGpio) {
      case AdrGpioB:
        OrMask |= (UINT32) (V_PMC_PWRM_ADR_GPIO_B << N_PMC_PWRM_ADR_GPIO_SEL);
        break;
      case AdrGpioC:
        OrMask |= (UINT32) (V_PMC_PWRM_ADR_GPIO_C << N_PMC_PWRM_ADR_GPIO_SEL);
        break;
    }
  }

  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_ADR_EN,
    AndMask,
    OrMask
   );
}

/**
  Configures ADR sources in the PMC

  @param[in] PchAdrConfig  PCH ADR configuration structure
**/
VOID
PchAdrConfigureSources (
  IN ADR_CONFIG *PchAdrConfig
  )
{
  UINT32 PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  if (PchAdrConfig->AdrSrcOverride == TRUE) {
    MmioWrite32 (
      PchPwrmBase + R_PMC_PWRM_ADR_GBLRST_EN_0,
      PchAdrConfig->AdrSrcSel.Value
    );
  }
}

/**
  Configures ADR timer in PMC.

  @param[in] PchAdrConfig     PCH ADR configuration structure
  @param[in] PmcAdrSocConfig  PMC ADR SOC configuration
**/
VOID
PchAdrConfigureTimer (
  IN ADR_CONFIG          *PchAdrConfig,
  IN PMC_ADR_SOC_CONFIG  *PmcAdrSocConfig
  )
{
  UINT32          PchPwrmBase;
  UINT32          OrMask;
  UINT32          AndMask;

  if (PchAdrConfig->AdrTimerEn != FORCE_ENABLE) {
    return;
  }

  PchPwrmBase = PmcGetPwrmBase ();

  AndMask = ~(UINT32) (B_PMC_PWRM_ADR_GEN_CFG_ADR_P2_TIMER_SCALE |
                       B_PMC_PWRM_ADR_GEN_CFG_ADR_P2_TIMER_TOV |
                       B_PMC_PWRM_ADR_GEN_CFG_ADR_P1_TIMER_SCALE |
                       B_PMC_PWRM_ADR_GEN_CFG_ADR_P1_TIMER_TOV);

  OrMask = (UINT32)((PchAdrConfig->AdrTimer1Val << N_PMC_PWRM_ADR_GEN_CFG_ADR_P1_TIMER_TOV) |
                    (PchAdrConfig->AdrMultiplier1Val << N_PMC_PWRM_ADR_GEN_CFG_ADR_P1_TIMER_SCALE) |
                    (PchAdrConfig->AdrTimer2Val << N_PMC_PWRM_ADR_GEN_CFG_ADR_P2_TIMER_TOV) |
                    (PchAdrConfig->AdrMultiplier2Val << N_PMC_PWRM_ADR_GEN_CFG_ADR_P2_TIMER_SCALE));

  if (PmcAdrSocConfig->AdrGenCfgMoved) {
    MmioAndThenOr32 (
      PchPwrmBase + R_PMC_PWRM_ADR_GEN_CFG_V2,
      AndMask,
      OrMask
    );
  } else {
    MmioAndThenOr32 (
      PchPwrmBase + R_PMC_PWRM_ADR_GEN_CFG,
      AndMask,
      OrMask
    );
  }
}

/**
  PCH ADR Enable master switch.

  This function enables/disables PCH-side ADR.

  @param[in] EnableAdr  TRUE:  Enables PCH-side ADR
                        FALSE: Disables PCH-side ADR
**/
VOID
PchAdrEnableAdr (
  IN BOOLEAN  EnableAdr
  )
{
  UINT32         PchPwrmBase;
  UINT32         OrMask;
  UINT32         AndMask;

  OrMask = 0;
  AndMask = (UINT32) ~0;

  PchPwrmBase = PmcGetPwrmBase ();
  if (EnableAdr) {
    OrMask = B_PMC_PWRM_ADR_EN_ADR_FEAT_EN;
  } else {
    AndMask = (UINT32) ~B_PMC_PWRM_ADR_EN_ADR_FEAT_EN;
  }
  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_ADR_EN,
    AndMask,
    OrMask
  );
}

/**
  Configures PCH-side ADR.

  This function must always be called by MRC even if ADR is not being enabled.
  This is to allow PCH DFX overrides to work.

  @param[in] PmcAdrSocConfig   Pointer to the SoC configuration of the ADR
**/
VOID
PchAdrConfigure (
  IN PMC_ADR_SOC_CONFIG  *PmcAdrSocConfig
  )
{
  ADR_CONFIG *PchAdrConfig;

  PchAdrGetConfig (&PchAdrConfig);

  //
  // Disable ADR before configuring PMC
  //
  PchAdrEnableAdr (FALSE);

  if (PchAdrConfig->AdrEn != FORCE_ENABLE) {
    return;
  }

  PchAdrConfigurePmc (PchAdrConfig, PmcAdrSocConfig);
  PchAdrConfigureSources (PchAdrConfig);
  PchAdrConfigureTimer (PchAdrConfig, PmcAdrSocConfig);
  PchAdrClearAdrResetStatus ();

  PchAdrEnableAdr (TRUE);
}

/**
  @brief Reconfigure PCH-side ADR for NVDIMMs.

  @param[in] TimerVal Timer value [us] to configure, a finite set of values is supported (see PCH_ADR_TIMER_<*>).

  @return Void.
**/
VOID
PchAdrConfigureForNvdimm (
  IN UINT32 TimerVal
  )
{

}

/**
  @brief Reconfigure PCH-side ADR for DDRT DIMMs.

  @param[in] TimerVal Not used, kept for compability with previous projects.

  @return Void.
**/
VOID
PchAdrConfigureForDdrt (
  IN UINT32 TimerVal
  )
{

}

/**
  Clears PCH ADR Reset Status
**/
VOID
PchAdrClearAdrResetStatus (
  VOID
  )
{
  UINT32     PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();
  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_ADR_CTRL_STS, B_PMC_PWRM_ADR_CTRL_STS_ADR_RST_STS);
}

/**
  Gets PCH ADR Reset Status

  @retval TRUE  ADR flow was executed during a prior global reset entry.
  @retval FALSE ADR flow was *not* executed during a prior global reset entry.

**/
BOOLEAN
PchAdrGetAdrResetStatus (
  VOID
  )
{
  UINT32     PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();
  return !!(MmioRead32 (PchPwrmBase + R_PMC_PWRM_ADR_CTRL_STS) & B_PMC_PWRM_ADR_CTRL_STS_ADR_RST_STS);
}
