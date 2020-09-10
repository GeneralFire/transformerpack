/** @file
  Intel CPU PPM policy common structures and macros for both
  CPU late policy PPI and CPU policy protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef __CPU_POLICY_PEI_DXE_COMMON_HEADER__
#define __CPU_POLICY_PEI_DXE_COMMON_HEADER__

typedef struct {
  BOOLEAN   CpuTStateEnable;
  UINT8     CpuClockModulationDutyCycle;
  BOOLEAN   CpuAesEnable;
  BOOLEAN   CpuFastStringEnable;
  BOOLEAN   CpuMaxCpuidValueLimitEnable;
  BOOLEAN   CpuMachineCheckEnable;
  BOOLEAN   CpuMonitorMwaitEnable;
  BOOLEAN   CpuVtEnable;
  BOOLEAN   CpuLtEnable;
  BOOLEAN   CpuX2ApicEnable;
  BOOLEAN   CpuEistEnable;
  BOOLEAN   CpuTurboModeEnable;
  BOOLEAN   CpuHwCoordinationEnable;
  UINT8     CpuBootPState;
  BOOLEAN   CpuPpinControlEnable;
  BOOLEAN   CpuPeciDownstreamWriteEnable;
  BOOLEAN   CpuL1NextPagePrefetcherDisable;
  BOOLEAN   CpuDcuPrefetcherEnable;
  BOOLEAN   CpuIpPrefetcherEnable;
  BOOLEAN   CpuMlcStreamerPrefetecherEnable;
  BOOLEAN   CpuMlcSpatialPrefetcherEnable;
  BOOLEAN   CpuAmpPrefetchEnable;
  BOOLEAN   CpuThreeStrikeCounterEnable;
  BOOLEAN   CpuCStateEnable;
  UINT8     CpuPackageCStateLimit;
  BOOLEAN   CpuC1AutoDemotionEnable;
  BOOLEAN   CpuC1AutoUndemotionEnable;
  UINT8     CpuCoreCStateValue;
  UINT16    CpuAcpiLvl2Addr;
  BOOLEAN   CpuThermalManagementEnable;
  UINT8     CpuTccActivationOffset;
  BOOLEAN   CpuDbpfEnable;
  BOOLEAN   CpuEnergyPerformanceBiasEnable;
  UINT32    CpuIioLlcWaysBitMask;
  UINT32    CpuExpandedIioLlcWaysBitMask;
  UINT32    CpuRemoteWaysBitMask;
  UINT32    CpuRrqCountThreshold;
  UINT8     CpuMtoIWa;
  BOOLEAN   RunCpuPpmInPei;
  BOOLEAN   AcExceptionOnSplitLockEnable;
} CPU_POLICY_COMMON;

#endif
