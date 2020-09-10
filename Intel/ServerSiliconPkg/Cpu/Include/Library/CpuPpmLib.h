/** @file
  This is an implementation of the BootScript at run time.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#ifndef _CPU_PPM_LIB_H_
#define _CPU_PPM_LIB_H_

#include <PiDxe.h>
#include <Protocol/PpmPolicyProtocol.h>
#include <Library/TimerLib.h>
#include "SysHost.h"
#include "UncoreCommonIncludes.h"
#include <RcRegs.h>


//CSR_PKG_CST_ENTRY_CRITERIA_MASK bit definition (For SKX)
#define SET_PCIEx_MASK                  0xF

// ratio in Performance Control MSR (MSR_IA32_PERF_CTL)
#define B_IA32_PERF_CTRLP_STATE_TARGET  (0x7F << 8)

//
// BIOS_RESET_CPL_PCU_FUN1_REG
//
#define SET_CPL_1     1
#define SET_CPL_2     2
#define SET_CPL_3     3
#define SET_CPL_4     4

#pragma pack(1)


/**************************
 Processor Power Management Data structs
***************************/

typedef struct _PPM_FROM_PPMINFO_HOB {
  UINT32      SocketPresentBitMap;                  // bitmap of socket Id with CPUs present detected by KTI RC
  UINT32      SlaveDieBitMap;                       // bitmap of Slave Dies present in the system. 1 indicates Slave Die, 0 indicates Master Die.
  UINT8       PackageDieCount[MAX_SOCKET];          // Package die count
  UINT8       ProcessorPowerUnit[MAX_SOCKET];       //
  UINT32      ProcessorTimeUnit[MAX_SOCKET];        //
  UINT16      PackageTdp[MAX_SOCKET];               // Package TDP
  UINT32      NumOfBitShift;                        // # Bits to shift right APIC ID to get next level APIC ID
  UINT8       Bios_Reset_Cpl_Phase;
  UINT8       DfxRstCplBitsEn;
  BOOLEAN     HwpmSupport;                          // HWPM support flag
  CAPID4_PCU_FUN3_STRUCT      CapId4Csr;            // CapId4 CSR value
  UINT8       SkipSignalPpmDone;                    // Signal to skip PpmDone (Ppm done by SGX MCHECK)
} PPM_FROM_PPMINFO_HOB;

typedef struct {
  UINT8           Major;         // Major Vesion
  UINT8           Minor;         // Minor Vesion
  UINT8           Rev;           // Release Version
  UINT8           Build;         // Build version
} PPM_VERSION;

typedef struct {
  PPM_VERSION                   Version;
  PPM_FROM_PPMINFO_HOB          *Info;
} EFI_CPU_PM_STRUCT;

//
// Temporary wrap pointer for StartupThisAp, which only allow one parameter
// Will remove after feature refactor done
//
typedef struct {
  EFI_CPU_PM_STRUCT             *Ppm;
  PPM_POLICY_CONFIGURATION      *PpmPolicy;
} PPM_WRAP_STRUCT;

#pragma pack()

VOID
EFIAPI
InitializeCpuPPMLib (
  EFI_CPU_PM_STRUCT   *ppm,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  );

VOID
EFIAPI
SetBiosInitDone (
  UINT8             SocketNumber,
  PPM_WRAP_STRUCT   *PpmWrapPtr
  );

VOID
EFIAPI
SetLockBitAfterBiosInitDone (
  UINT8             SocketNumber,
  PPM_WRAP_STRUCT   *PpmWrapPtr
  );

VOID
EFIAPI
SetPackageLockBit(
  UINT8                     SocketNumber,
  EFI_CPU_PM_STRUCT         *ppm,
  PPM_POLICY_CONFIGURATION  *PpmPolicy
  );

VOID
EFIAPI
SetThreadLockBit (
  PPM_WRAP_STRUCT   *PpmWrapPtr
  );

VOID
EFIAPI
InitializeRaplPowerLimitCsr (
  UINT8                  SocketNumber,
  EFI_CPU_PM_STRUCT     *ppm,
  TURBO_POWRER_LIMIT    *TurboPowerLimit,
  UINT32                 C2C3tt
  );

VOID
EFIAPI
InitializeDynamicPerfPowerCtl (
  UINT8                     SocketNumber,
  DYNAMIC_PER_POWER_CTL     *DynamicPerPowerCtl
  );

VOID
EFIAPI
InitializeSapmControl (
  UINT8                     SocketNumber,
  SAPM_CTL                  *SapmCtl
  );

VOID
EFIAPI
InitializeSwLtrOverride (
  UINT8                    SocketNumber,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  );

VOID
EFIAPI
InitializePkgCstEntryCriteriaMask (
  UINT8                    SocketNumber,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  );

VOID
EFIAPI
InitializeProchotResponseRatio (
  UINT8               SocketNumber,
  UINT32              ProchotRatio
  );

VOID
EFIAPI
SetupPCIEPkgCstEntryCriteria (
  UINT8                    SocketNumber,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  );

VOID
EFIAPI
InitializePerfPlimitControl (
  UINT8                     SocketNumber,
  PERF_PLIMIT_CTL           *PerPLimitCtl
  );

VOID
EFIAPI
InitializePmaxConfiguration (
  UINT8               SocketNumber,
  EFI_CPU_PM_STRUCT   *ppm,
  PMAX_CONFIG         *PmaxConfig
  );

VOID
EFIAPI
InitializeThermalReport (
  UINT8               SocketNumber,
  TM_STATUS_Filter    *ThermalReport,
  UINT8               CpuThermalManagement
  );

VOID
EFIAPI
InitializePowerControl (
  EFI_CPU_PM_STRUCT   *ppm,
  POWER_CTL           *PowerCtrl,
  UINTN               ProcessorNumber
  );

VOID
EFIAPI
InitializeRaplPowerLimitMsr (
  EFI_CPU_PM_STRUCT   *ppm,
  TURBO_POWRER_LIMIT    *TurboPowerLimit,
  UINTN                 ProcessorNumber
  );

/**
  Init Psys power limit MSR register.

  @param ppm              - Pointer to policy protocol instance
  @param PsysPowerLimit   - Pointer to PSYS_POWRER_LIMIT instance
  @param ProcessorNumber  - Processor index that is executing

  @retval None

**/
VOID
EFIAPI
InitializePlatformPowerLimitMsr (
  EFI_CPU_PM_STRUCT     *ppm,
  PSYS_POWRER_LIMIT     *PsysPowerLimit,
  UINTN                 ProcessorNumber
  );

/**
  Init Psys power limit CSR register.
  Set PPL1 and PPL2 power limit and time window.

  @param SocketNumber  -    Socket index that is executing
  @param ppm              - Pointer to policy protocol instance
  @param PsysPowerLimit   - Pointer to PSYS_POWRER_LIMIT instance

  @retval None

**/
VOID
EFIAPI
InitializePlatformPowerLimitCsr (
  UINT8                 SocketNumber,
  EFI_CPU_PM_STRUCT     *ppm,
  PSYS_POWRER_LIMIT     *PsysPowerLimit
  );

VOID
EFIAPI
InitializeEnergyPerformanceBias (
  PERF_BIAS_CONFIG    *PerfBiasConfig,
  UINTN               ProcessorNumber
  );

VOID
EFIAPI
InitializeVrCurrentConfig (
  ADV_PWR_CTL       *AdvancePowerCtrl,
  PPO_CURRENT_CFG   *PpoCurrent,
  UINTN               ProcessorNumber
  );

VOID
EFIAPI
InitializeTurboRatioLimitCores (
  EFI_CPU_PM_STRUCT *ppm,
  TURBO_RATIO_LIMIT *TurboRatioLimit,
  PPM_XE_STRUCT     *PpmXe,
  UINTN             ProcessorNumber
  );

VOID
EFIAPI
InitializeFastRaplDutyCycle (
  UINT8             SocketNumber,
  UINT8             FastRaplDutyCycle
  );

/**

  This function is to detect HwpmSupport.

  @param[in]          SocketPresentBitMap   Bitmap of socket Id with CPUs present detected by KTI RC.

  @retval TRUE        If Hwpm Supported
  @retval FALSE       If Hwpm doesn't Supported

**/
BOOLEAN
EFIAPI
IsHwpmSupported (
  IN    UINT32   SocketPresentBitMap
  );

VOID
EFIAPI
InitialzeHwpFeatures (
  EFI_CPU_PM_STRUCT   *ppm,
  PPM_HWPM_STRUCT     *Hwpm,
  UINTN               ProcessorNumber
  );

VOID
EFIAPI
InitializeHWPMMiscConfig (
  UINT8             SocketNumber,
  POWER_CTL         *PowerCtrl,
  PPM_HWPM_STRUCT   *Hwpm
  );

VOID
EFIAPI
InitializeCStateLatency (
  EFI_CPU_PM_STRUCT     *ppm,
  PPM_CSTATE_STRUCT     *PpmCst,
  UINTN                 ProcessorNumber
  );

VOID
EFIAPI
InitializeDynamicL1 (
  UINT8                    SocketNumber,
  PPM_POLICY_CONFIGURATION *PpmPolicy
  );

VOID
EFIAPI
SetConfigControlLockBit (
  PPM_POLICY_CONFIGURATION *PpmPolicy
  );

VOID
EFIAPI
ForceUncoreAndMeshRatio (
  EFI_CPU_PM_STRUCT *ppm
  );

VOID
EFIAPI
MiscWorkaroundEnable (
  UINT8                  SocketNumber,
  ADV_PWR_CTL            *AdvPwrMgtCtl
  );

VOID
EFIAPI
AvxDisConfig (
  EFI_CPU_PM_STRUCT     *ppm,
  UINT8                 AvxSupport,
  UINTN                 ProcessorNumber
  );

VOID
EFIAPI
InitializePkgcSaPsCriteria (
  UINT8                       SocketNumber,
  PKGC_SA_PS_CRITERIA_STRUCT  *PkgcCriteria
  );

VOID
EFIAPI
InitializeAVXICCPPreGrantLevel (
  UINT8                 SocketNumber,
  UINT8                 AvxLicensePreGrant,
  UINT8                 AvxIccpLevel
  );

VOID
EFIAPI
InitializeLimca (
  UINT8                 SocketNumber
  );

VOID
EFIAPI
InitializeGpssTimer (
  UINT8                 SocketNumber,
  UINT8                 GpssTimer
  );

VOID
EFIAPI
InitializeDcstLuts (
  UINT8             SocketNumber
  );

VOID
EFIAPI
InitialzeConfigurePbf (
  EFI_CPU_PM_STRUCT   *ppm,
  PPM_HWPM_STRUCT     *Hwpm,
  UINTN               ProcessorNumber
  );

VOID
EFIAPI
CommonPpmProgramMsrs (
  EFI_CPU_PM_STRUCT         *ppm,
  PPM_POLICY_CONFIGURATION  *PpmPolicy,
  UINTN                     ProcessorNumber
);

VOID
EFIAPI
CommonPpmProgramCsrs (
  EFI_CPU_PM_STRUCT         *ppm,
  PPM_POLICY_CONFIGURATION  *PpmPolicy,
  UINT32                    SocketPresentBitMap
);

#endif
