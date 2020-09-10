/** @file
  Code File for CPU Power Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

/**

  This routine is called to program ENERGY_PERF_BIAS_CONFIG.

  @param PerfBiasConfig  - Pointer to PERF_BIAS_CONFIG instance
  @param ProcessorNumber - Processor index that is executing

  @retval None

**/
VOID
EFIAPI
InitializeEnergyPerformanceBias (
  PERF_BIAS_CONFIG    *PerfBiasConfig,
  UINTN               ProcessorNumber
  )
{
  MSR_ENERGY_PERF_BIAS_CONFIG_REGISTER  EnergyPerfBiasMsr;

  //
  // Program ENERGY_PERF_BIAS_CONFIG(0xA01)
  //
  EnergyPerfBiasMsr.Uint64 = AsmReadMsr64 (MSR_ENERGY_PERF_BIAS_CONFIG);
  EnergyPerfBiasMsr.Bits.WorkldConfig             = PerfBiasConfig->WorkLdConfig;
  EnergyPerfBiasMsr.Bits.AltEnergyPerfBias        = PerfBiasConfig->AltEngPerfBIAS;
  EnergyPerfBiasMsr.Bits.P0TotalTimeThresholdHigh = PerfBiasConfig->P0TtlTimeHigh1;
  EnergyPerfBiasMsr.Bits.P0TotalTimeThresholdLow  = PerfBiasConfig->P0TtlTimeLow1;
  EnergyPerfBiasMsr.Bits.AvgTimeWindow            = PerfBiasConfig->EngAvgTimeWdw1;
  AsmWriteMsr64 (MSR_ENERGY_PERF_BIAS_CONFIG, EnergyPerfBiasMsr.Uint64);

  //save for S3
  WriteRegisterTable (ProcessorNumber, Msr, MSR_ENERGY_PERF_BIAS_CONFIG, 0, 64, EnergyPerfBiasMsr.Uint64);
}
