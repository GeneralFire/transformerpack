/** @file
  Code File for power control initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#include "Common/CpuPpmIncludes.h"


/**

  This routine is called to program Power_CTL not programed elsewhere.
  It will be call multiple time passing any the node to be programmed.

  @param PPMPolicy       - Pointer to PPM Policy protocol instance
  @param PPMPolicy       - Pointer to POWER_CTL instance
  @param ProcessorNumber - Processor index that is executing

  @retval None

**/
VOID
EFIAPI
InitializePowerControl (
  EFI_CPU_PM_STRUCT   *ppm,
  POWER_CTL           *PowerCtrl,
  UINTN               ProcessorNumber
  )
{
  ICX_MSR_VR_MISC_CONFIG_REGISTER       VrConfigMsr;
  ICX_MSR_POWER_CTL1_REGISTER           PowerCtrlMsr;

  //
  // Program MSR_POWER_CTL
  //
  PowerCtrlMsr.Uint64 = AsmReadMsr64 (ICX_MSR_POWER_CTL1);

  PowerCtrlMsr.Bits.PchNegDisable       = PowerCtrl->PkgCLatNeg;
  PowerCtrlMsr.Bits.LtrIioDisable       = PowerCtrl->LtrSwInput;
  PowerCtrlMsr.Bits.PwrPerfTuningEnableDynSwitching = PowerCtrl->PwrPerfSwitch;
  PowerCtrlMsr.Bits.PwrPerfTuningDisableSapmCtrl    = PowerCtrl->SapmControl;
  PowerCtrlMsr.Bits.EeTurboDisable      = PowerCtrl->EeTurboDisable;
  PowerCtrlMsr.Bits.C1eEnable           = PowerCtrl->C1eEnable;
  PowerCtrlMsr.Bits.DisProchotOut       = PowerCtrl->ProchotOutputDisable;
  PowerCtrlMsr.Bits.EnableBidirProchot  = PowerCtrl->BidirProchotEnable;

  if (PowerCtrl->PwrPerfTuning < PWR_PERF_TUNING_PECI_CONTROL) {
    PowerCtrlMsr.Bits.PwrPerfTuningCfgMode = PowerCtrl->PwrPerfTuning;
  } else {
    PowerCtrlMsr.Bits.PwrPerfTuningCfgMode = 0;
  }

  PowerCtrlMsr.Bits.SapmImcC2Policy = 0;
  PowerCtrlMsr.Bits.ProchotLock = 0;

  AsmWriteMsr64 (ICX_MSR_POWER_CTL1, PowerCtrlMsr.Uint64);
  //save for S3
  WriteRegisterTable (ProcessorNumber, Msr, ICX_MSR_POWER_CTL1, 0, 64, PowerCtrlMsr.Uint64);

  // Enables decay mode on package C-state entry
  VrConfigMsr.Uint64 = AsmReadMsr64 (ICX_MSR_VR_MISC_CONFIG);

  return;
}

