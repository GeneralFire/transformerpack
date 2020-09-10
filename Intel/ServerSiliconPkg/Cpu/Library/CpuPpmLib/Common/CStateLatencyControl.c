/** @file
  Code for Enhanced Intel Speedstep Technology

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

#include "CpuPpmIncludes.h"

static const UINT32 C_LATENCY_MSR[NUM_CST_LAT_MSR] = {
  MSR_C_STATE_LATENCY_CONTROL_0,
  MSR_C_STATE_LATENCY_CONTROL_1,
  MSR_C_STATE_LATENCY_CONTROL_2
};

/**

  Program C-State Latency MSRs.

  @param ppm              Pointer to EFI_CPU_PM_STRUCT instance
  @param PpmCst           Pointer to PPM_CSTATE_STRUCT instance
  @param ProcessorNumber  Processor index that is executing

  @retval EFI_SUCCESS

**/
VOID
EFIAPI
InitializeCStateLatency (
  EFI_CPU_PM_STRUCT     *ppm,
  PPM_CSTATE_STRUCT     *PpmCst,
  UINTN                 ProcessorNumber
  )
{
  UINT8              Index;
  MSR_C_STATE_LATENCY_CONTROL_0_REGISTER CStateLatencyCtlMsr;

  for (Index = 0; Index < NUM_CST_LAT_MSR; Index++) {
    if (PpmCst->LatencyCtrl[Index].Valid == 1) {
      CStateLatencyCtlMsr.Uint64          = AsmReadMsr64 (C_LATENCY_MSR[Index]);
      CStateLatencyCtlMsr.Bits.Value      = PpmCst->LatencyCtrl[Index].Value;
      CStateLatencyCtlMsr.Bits.Multiplier = PpmCst->LatencyCtrl[Index].Multiplier;
      CStateLatencyCtlMsr.Bits.Valid      = PpmCst->LatencyCtrl[Index].Valid;

      AsmWriteMsr64 (C_LATENCY_MSR[Index], CStateLatencyCtlMsr.Uint64);   //program bit15:0 per user option

      //save for S3
      WriteRegisterTable (ProcessorNumber, Msr, C_LATENCY_MSR[Index], 0, 64, CStateLatencyCtlMsr.Uint64);
    }
  }
  return;
}
