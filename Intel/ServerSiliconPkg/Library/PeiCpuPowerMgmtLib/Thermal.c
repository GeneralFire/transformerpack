/** @file
  This library contains power management configuration functions for processors.

  <b>Acronyms:</b>
     - PPM:  Processor Power Management
     - TM:   Thermal Monitor
     - IST:  Intel(R) Speedstep technology
     - HT:   Hyper-Threading Technology

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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

#include <Cpu/CpuCoreRegs.h>

/**
  Get TCC cross throttling teamperature.
  Set Thermal trip point temperature indicated by MSR 1A2h
  If (RATL mode), T0L = MSR 1A2h[23:16]
  Else          , T0L = MSR 1A2h[23:16] - MSR 1A2h[29:24]

  @retval  Value of the activation temperature
**/
UINT32
CpuGetCrossThrottlingTripPoint (
  VOID
  )
{
  SPR_MSR_TEMPERATURE_TARGET_REGISTER   TemperatureTargetMsr;
  UINT32                                Temperature = 0;

  TemperatureTargetMsr.Uint64 = AsmReadMsr64 (SPR_MSR_TEMPERATURE_TARGET);
  if (TemperatureTargetMsr.Bits.TccOffsetTimeWindow != 0) {
    Temperature = TemperatureTargetMsr.Bits.RefTemp;
  } else {
    Temperature = TemperatureTargetMsr.Bits.RefTemp - TemperatureTargetMsr.Bits.TjMaxTccOffset;
  }

  return Temperature;
}
