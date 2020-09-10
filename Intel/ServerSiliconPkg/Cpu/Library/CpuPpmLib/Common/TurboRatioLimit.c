/** @file
  Code for Enhanced Intel Speedstep Technology

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

BOOLEAN
RatioLimitCheck (
  UINT32 *RatioLimit,
  UINT8   CoreCount
  );

/**

  This function check the user RatioLimit option inputs

  @param RatioLimit - pointer to user ratiolimit inputs
  @param CoreCount  - socket's active core counts

  @retval RatioLimitState - TRUE if user inputs are valid otherwise FALSE

**/
BOOLEAN
RatioLimitCheck (
  UINT32 *RatioLimit,
  UINT8   CoreCount
  )
{
  BOOLEAN RatioLimitState = TRUE;
  UINT32   i;

  for(i = 1; i < CoreCount; i++) {
    RatioLimitState = (RatioLimit[0] >= RatioLimit[i]);
    if(!RatioLimitState) {
        break;
    }
  }

  return RatioLimitState;

}

/**

  This function checks whether capable for TurboLimit update

  @param  none

  @retval RatioLimitState - TRUE if processor turbo-ratio can be overriden otherwise FALSE

**/
BOOLEAN
IsRatioLimitUpdateCapable (
  VOID
  )
{
  ICX_MSR_PLATFORM_INFO_REGISTER PlatformInfoMsr;

  //
  // Check if XE capable
  //
  PlatformInfoMsr.Uint64 = AsmReadMsr64 (ICX_MSR_PLATFORM_INFO);

  //
  // Check if processor turbo-ratio can be overriden:
  // SandyBridge BWG Section 14.14.2
  // If PLATFORM INFO MSR [28] == 1 and FLEX_RATIO MSR [19:17] != 0
  // Processor with capability to override turbo-ratio detected (either XE or Overclocking part detected)
  //
  if (PlatformInfoMsr.Bits.PrgTurboRatioEn == 0) {
    //
    // Not XE or Over clocking Capable processor.
    //
    return FALSE;
  }

  return TRUE;
}

/**

  Initializes XE OR Overclocking support in the processor.

  @param PPMPolicy        - Pointer to PPM Policy protocol instance
  @param TurboRatioLimit  - Pointer to Turbo Ratio Limit instance
  @param PpmXe            - Pointer to Ppm Xe instance
  @param ProcessorNumber  - Processor index that is executing

  @retval EFI_SUCCESS

**/
VOID
EFIAPI
InitializeTurboRatioLimitCores (
  EFI_CPU_PM_STRUCT *ppm,
  TURBO_RATIO_LIMIT *TurboRatioLimit,
  PPM_XE_STRUCT     *PpmXe,
  UINTN             ProcessorNumber
  )
{
  SKX_MSR_TURBO_RATIO_LIMIT_RATIOS_REGISTER   TurboRatioLimitRatioMsr;
  MSR_TURBO_RATIO_LIMIT_CORES_REGISTER        TurboRatioLimitCoresMsr;
  MSR_CORE_THREAD_COUNT_REGISTER              CoreThreadCountMsr;
  MSR_FLEX_RATIO_REGISTER                     FlexRatioMsr;

  //
  // Now initialize turbo ratio limit MSR
  // Find the number of active cores and initialize the ratio limits only if they are available.
  // Also program the VID value for the new max Turbo ratio by programming Flex Ratio MSR.
  //
  CoreThreadCountMsr.Uint64      = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);

  TurboRatioLimitRatioMsr.Uint64 = AsmReadMsr64 (ICX_MSR_TURBO_RATIO_LIMIT);
  TurboRatioLimitCoresMsr.Uint64 = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT_CORES);

  if(!IsRatioLimitUpdateCapable ()) {
    return;
  }

  FlexRatioMsr.Uint64 = AsmReadMsr64 (MSR_FLEX_RATIO);
  if (FlexRatioMsr.Bits.OcLock == 1) {
    //
    // Clear OC_LOCK if it is set
    //
    FlexRatioMsr.Bits.OcLock = 0;
    AsmWriteMsr64 (MSR_FLEX_RATIO, FlexRatioMsr.Uint64);
    //save for S3
    WriteRegisterTable (ProcessorNumber, Msr, MSR_FLEX_RATIO, 0, 64, FlexRatioMsr.Uint64);
    //
    // Program VID for the new Max Turbo Ratio
    //
    FlexRatioMsr.Bits.OcExtraVoltage = PpmXe->Voltage;
    AsmWriteMsr64 (MSR_FLEX_RATIO, FlexRatioMsr.Uint64);
    //save for S3
    WriteRegisterTable (ProcessorNumber, Msr, MSR_FLEX_RATIO, 0, 64, FlexRatioMsr.Uint64);
  }

  //now program TurboRatioLimitRatio and TurboRatioLimitCores Msr per user options
  TurboRatioLimitRatioMsr.Uint64 &= (UINT64)*(UINT64 *) TurboRatioLimit->RatioLimitRatioMask;
  TurboRatioLimitCoresMsr.Uint64 &= (UINT64)*(UINT64 *) TurboRatioLimit->RatioLimitCoresMask;
  TurboRatioLimitRatioMsr.Uint64 |= (UINT64)*(UINT64 *) TurboRatioLimit->RatioLimitRatio;
  TurboRatioLimitCoresMsr.Uint64 |= (UINT64)*(UINT64 *) TurboRatioLimit->RatioLimitCores;

  AsmWriteMsr64 (ICX_MSR_TURBO_RATIO_LIMIT, TurboRatioLimitRatioMsr.Uint64);
  //save for S3
  WriteRegisterTable (ProcessorNumber, Msr, ICX_MSR_TURBO_RATIO_LIMIT, 0, 64, TurboRatioLimitRatioMsr.Uint64);

  AsmWriteMsr64 (MSR_TURBO_RATIO_LIMIT_CORES, TurboRatioLimitCoresMsr.Uint64);
  //save for S3
  WriteRegisterTable (ProcessorNumber, Msr, MSR_TURBO_RATIO_LIMIT_CORES, 0, 64, TurboRatioLimitCoresMsr.Uint64);

  return;
}
