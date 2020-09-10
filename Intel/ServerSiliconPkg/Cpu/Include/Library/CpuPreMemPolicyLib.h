/** @file
  Interface of CPU Pre Mem Policy library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef __CPU_PRE_MEM_POLICY_LIB_H__
#define __CPU_PRE_MEM_POLICY_LIB_H__

#include <CpuDataStruct.h>
#include "SysHost.h"


//+
//  CPU_PRE_MEM_POLICY
//  CPU init related Setup options
//
typedef struct {                      // CPU init related Setup options
  UINT8   dcuModeSelect;                ///< 0: 32KB 8-way (hardware default). Non-zero: 16KB 4-way with ECC (CPU MSR 031h)
  UINT8   EnableGv;                     ///< GV3 Enable
  UINT8   flexRatioEn;                  ///< FLEX_RATIO Override Enable
  UINT8   flexRatioNext;                ///< FLEX_RATIO, common for all CPU sockets
                                        ///<  0=Don't change flex ratio (default) 0xff = Max Non-turbo ratio. Other values defines target flex ratio
  UINT8   IssTdpLevel;                  ///< 0 - 2: 0 = Normal; 1 = Level 1; 2 = Level 2
  UINT8   DynamicIss;                   ///< 0/1 Disable/Enable Dynamic ISS
  UINT8   ActivePbf;                    ///< 1: Active PBF if capable
  UINT8   ConfigTdpLevel;               ///< 0, 3 - 4: 0 = Base; 3 = Level 3; 4 = Level 4
  UINT16  NumberOfCores2Disable[MAX_SOCKET]; ///< Number of processor cores to disable for each CPU socket
  UINT64  CoreDisableMask[MAX_SOCKET];///< CoreOffMask value for each CPU socket (64bits)
  UINT64  OscCoreDisMask[MAX_SOCKET];   ///< CoreOffMask value for Opportunistic Spare Core
  UINT8   smtEnable;                    ///< 0/1 Disable/Enable SMT(HT). common for all CPU sockets
  UINT8   vtEnable;                     ///< 0/1 Disable/Enable VMX. Common for all CPU sockets
  UINT8   IotEn[MAX_SOCKET];            ///< IOT/OCLA Config Disable/Enable,
  UINT8   OclaTorEntry[MAX_SOCKET];     ///< IOT/OCLA MaxTorEntry
  UINT8   OclaWay[MAX_SOCKET];          ///< IOT/OCLA LLC Ways
  UINT8   AllowMixedPowerOnCpuRatio;    ///< Keep CPU ratios at power-on default without forcing common ratio among CPU socekts
  UINT8   CheckCpuBist;                 ///< Check BIST result and disable failed cores when enabled. Otherwise, ignore BIST result
  UINT8   CoreFailover;                 ///< Enable spare core(s) in place of core(s) that fail BIST
  UINT64  DfxBistFailureEmulation;      ///< Emulate core BIST failure to test core sparing
  UINT8   debugInterfaceEn;             // 1: Enable Debug Inerface for DFX
  UINT8   WFRWAEnable;
  UINT8   PmaxDisable;
  UINT8   CpuPaLimit;
  UINT8   RdtCatOpportunisticTuning;    ///< CLX-mode tuning options that might give better CAT results in some cases - corresponds with cbsads_unicast_cfg_spare_cfg.reserved CSR value
  UINT8   EarlyC1eEnable;
  UINT8   LlcPrefetchEnable;
  UINT8   ProcessorMsrLockControl;
  UINT8   ProcessorMsrPkgCstConfigControlLock;
  UINT8   FadrSupport;
  UINT8   TscResetEnable;
} CPU_PRE_MEM_POLICY;

/**
  Set CPU Pre Mem policy.

  @param  CpuPolicy             Pointer to Cpu Pre Mem Policy Info.
  @param  DataSize              The size of Cpu Pre Mem Policy.

  @retval EFI_SUCCESS           Set CPU Pre Mem policy successfully.
  @retval EFI_OUT_OF_RESOURCES  Out of resource.

**/
EFI_STATUS
EFIAPI
SetCpuPreMemPolicy (
  IN VOID   *CpuPolicy,
  IN UINTN  DataSize
  );


/**
  Get CPU Pre Mem policy.

  @retval Pointer to CPU_PRE_MEM_POLICY Get CPU Pre Mem policy successfully.
  @retval NULL                  Can't find the Cpu Pre Mem Policy.

**/
CPU_PRE_MEM_POLICY *
EFIAPI
GetCpuPreMemPolicy (
  );

#endif
