/** @file
  Header file for CpuPlatform Lib.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2020 Intel Corporation.

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

#ifndef _CPU_PLATFORM_LIB_H_
#define _CPU_PLATFORM_LIB_H_

#include <Uefi.h>
#include <CpuRegs.h>
#include <CpuDataStruct.h>
#include <CpuPowerMgmt.h>

#define NO_NR_PPAM_SUPPORT      0
#define NR_PPAM_10_SUPPORT      10
#define NR_PPAM_11_SUPPORT      11
#define END_OF_TIMETABLE        0x3FF

///
/// Table to convert Seconds into equivalent MSR values
/// This table is used for PL1, Pl2 and RATL TDP Time Window programming
///
extern GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mSecondsToMsrValueMapTable[][2];

/**
  Check CPU Type of the platform

  @retval CPU_FAMILY              CPU type
**/
CPU_FAMILY
EFIAPI
GetCpuFamily (
  VOID
  );

/**
  Return Cpu stepping type

  @retval CPU_STEPPING                   Cpu stepping type
**/
CPU_STEPPING
EFIAPI
GetCpuStepping (
  VOID
  );

/**
  Return CPU Sku

  @retval UINT8              CPU Sku
**/
UINT8
EFIAPI
GetCpuSku (
  VOID
  );

/**
  Check on the processor if SMX (Safer Mode Extensions) is supported.

  @retval  TRUE        CPU supports SMX.
  @retval  FALSE       CPU do not support SMX.
**/
BOOLEAN
IsSmxSupported (
  VOID
  );

/**
  Return CPU Identifier used to identify various CPU types

  @retval CPU_IDENTIFIER           CPU Identifier
**/
UINT32
GetCpuIdentifier (
  );

/**
  Return CPU Identifier used to identify various CPU types

  @param[in] SelectedCtdpLevel    - Ctdp Level Selected in BIOS

  @retval CPU_IDENTIFIER           CPU Identifier
**/
UINT32
GetCpuIdentifierWithCtdp (
  IN UINT16               SelectedCtdpLevel
  );

/**
  Returns the processor microcode revision of the processor installed in the system.

  @retval Processor Microcode Revision
**/
UINT32
GetCpuUcodeRevision (
  VOID
  );

/**
  Check if this microcode is correct one for processor

  @param[in] Cpuid               - processor CPUID
  @param[in] MicrocodeEntryPoint - entry point of microcode
  @param[in] Revision            - revision of microcode

  @retval CorrectMicrocode if this microcode is correct
**/
BOOLEAN
CheckMicrocode (
  IN UINT32               Cpuid,
  IN CPU_MICROCODE_HEADER *MicrocodeEntryPoint,
  IN UINT32               *Revision
  );

/**
  This function is to program Trace Hub ACPI base address to processor's MSR TRACE_HUB_STH_ACPIBAR_BASE.

  @param[in]  TraceHubAcpiBase - Base address of Trace Hub ACPI Base address
**/
VOID
EFIAPI
CpuWriteTraceHubAcpiBase (
  IN UINT64  TraceHubAcpiBase
  );

/**
  Check on the processor if SGX is supported.

  @retval True if SGX supported or FALSE if not
**/
BOOLEAN
IsSgxSupported (
  VOID
  );

/**
  Detect if C6DRAM supported or not by reading it from PCODE mailbox.

  @retval TRUE - Supported
  @retval FALSE - Not Supported
**/
BOOLEAN
IsC6dramSupported (
  VOID
  );

/**
  Get processor generation

  @retval CPU_GENERATION  Returns the executing thread's processor generation.
**/
CPU_GENERATION
GetCpuGeneration (
  VOID
  );

/**
  This function returns Number of enabled cores in the package.

  @retval Number of enabled cores in the package.
**/
UINT16
GetEnabledCoreCount (
  VOID
  );

/**
  This function returns Number of enabled Threads in the package.

  @retval Number of enabled threads in the package.
**/
UINT16
GetEnabledThreadCount (
  VOID
  );

/**
  Check if this is non-core processor - HT AP thread

  @retval TRUE if this is HT AP thread
  @retval FALSE if this is core thread
**/
BOOLEAN
IsSecondaryThread (
  VOID
  );

/**
  This function checks for Whiskey Lake CPU presence.

  @retval TRUE  - WHL CPU Stepping W0 or V0 detected
  @retval FALSE - no WHL CPU detected
**/
BOOLEAN
IsWhlCpu (
  VOID
  );

/**
  This function checks for Rocket Lake CPU presence.

  @retval TRUE  - RKL CPU detected
  @retval FALSE - no RKL CPU detected
**/
BOOLEAN
IsRklCpu (
  VOID
  );

/**
  This function checks for CML CPU presence.

  @retval TRUE  - CML CPU detected
  @retval FALSE - no CML CPU detected
**/
BOOLEAN
IsCmlCpu (
  VOID
  );

/**
  This function checks whether uCode loaded from FIT.

  @retval TRUE  - uCode loaded from FIT successful
  @retval FALSE - Failed on FIT to load uCode
**/
BOOLEAN
IsValiduCodeEntry (
  VOID
  );

/**
  Return if CPU supports PFAT

  @retval TRUE             If CPU Supports
  @retval FALSE            If CPU doesn't Supports
**/
BOOLEAN
IsPfatEnabled (
  VOID
  );

/**
  Is BIOS GUARD enabled.

  @retval TRUE   BIOS GUARD is supported and enabled.
  @retval FALSE  BIOS GUARD is disabled.
**/
BOOLEAN
IsBiosGuardEnabled (
  VOID
  );

/**
  Determine if CPU supports Intel Turbo Boost Max Technology 3.0 (ITBM).

  @retval TRUE   ITBM is supported and enabled.
  @retval FALSE  ITBM is disabled.
**/
BOOLEAN
IsItbmSupported (
  VOID
);

/**
  Determine if the CPU is 10 Cores SKU.

  @retval TRUE    CPU sku is equal or above 10 Cores
  @retval FALSE   CPU sku is below 10 Cores
**/
BOOLEAN
Is10CoreSku (
  VOID
);
/**
  Determine if CPU supports Overclocking by reading the number of bins in MSR FLEX_RATIO (194h)

  @retval TRUE   OC is supported and enabled.
  @retval FALSE  OC is disabled.
**/
BOOLEAN
IsOcSupported (
  VOID
);

/**
  Determine if CPU supports Programmable Core Ratio Limit for the Turbo mode.

  @retval TRUE   Core Ratio Limit for the Turbo mode is supported and enabled.
  @retval FALSE  Core Ratio Limit for the Turbo mode is disabled.
**/
BOOLEAN
IsCoreRatioLimitSupported (
  VOID
);

/**
  Determine if CPU supports Programmable TDC/TDP Limit for the Turbo mode.

  @retval TRUE   TDC/TDP Limit is supported and enabled.
  @retval FALSE  TDC/TDP Limit is disabled.
**/
BOOLEAN
IsXETdcTdpLimitSupported (
  VOID
);

/**
  Determine if CPU supports Turbo mode.

  @retval TRUE   Turbo mode is supported and enabled.
  @retval FALSE  Turbo mode is disabled.
**/
BOOLEAN
IsTurboModeSupported (
  VOID
);

/**
  Returns number of CPUSockets.
**/
UINT32
NumberOfCpuSocketsPopulated (
  VOID
  );

/**
  Determine if CPU supports PPIN (Protected Processor Inventory Number)

  @retval TRUE   PPIN feature is available.
  @retval FALSE  PPIN feature is not available.
**/
BOOLEAN
IsPpinFeatureAvailable (
  VOID
  );

/**
  Determine if CPU supports Hardware P-States.

  @retval TRUE   Hardware P-States is supported and enabled.
  @retval FALSE  Hardware P-States is disabled.
**/
BOOLEAN
IsHwpSupported (
  VOID
  );

/**
  Determine if CPU supports Mwait.

  @retval TRUE   Mwait is supported and enabled.
  @retval FALSE  Mwait is disabled.
**/
BOOLEAN
IsTimedMwaitSupported (
  VOID
  );

/**
  Determine if CPU supports LPM.

  @retval TRUE   LPM is supported and enabled.
  @retval FALSE  LPM is disabled.
**/
BOOLEAN
IsLpmSupported (
  VOID
  );

/**
  Determine if CPU supports ConfigTdp.

  @retval TRUE   ConfigTdp is supported and enabled.
  @retval FALSE  ConfigTdp is disabled.
**/
BOOLEAN
IsConfigTdpSupported (
  VOID
  );

/**
  Determine if CPU supports Turbo mode.

  @retval TRUE   Efficiency Turbo mode is supported and enabled.
  @retval FALSE  Efficiency Turbo mode is disabled.
**/
BOOLEAN
IsEnergyEfficientTurboSupported (
  VOID
  );

/**
  Determine if CPU supports Hyper-Threading

  @retval TRUE   Hyper-Threading is supported and enabled.
  @retval FALSE  Hyper-Threading is disabled.
**/
BOOLEAN
IsHyperThreadingSupported (
  VOID
  );

/**
  Determines what Ctdp levels are supported on the silicon.

  @param[in] CtdpDownSupported   - Pointer to CtdpDownSupported
  @param[in] CtdpUpSupported     - Pointer to CtdpUpSupported

  @retval VOID
**/
VOID GetConfigTdpLevelsSupported (
  IN UINT8     *CtdpDownSupported,
  IN UINT8     *CtdpUpSupported
  );

/**
  Determines what Ctdp levels are supported on the silicon.

  @param[in] MwaitEcx     - Pointer to MwaitEcx (CPUID_MONITOR_MWAIT_ECX)
  @param[in] MwaitEdx     - Pointer to MwaitEdx (CPUID_MONITOR_MWAIT_EDX)

  @retval VOID
**/
VOID GetSubCStateSupported (
  IN UINT32     *MwaitEcx,
  IN UINT32     *MwaitEdx
  );

/**
  Determine Number of system supported cores.
  Note: It's not suitable for the hybrid CPU platform.

  @retval Number of system supported cores.
**/
UINT8
GetCpuNumberofCores (
  VOID
  );

/**
  Determine Number of system supported cores.
  Note: It's suitable for both hybrid CPU and Non-hybrid platform.

  @param[out] *NumberOfSupportedCores              - variable that will store Maximum supported cores.
  @param[out] *NumberOfSupportedAtomCores          - variable that will store Maximum supported Atom cores.

  @retval     EFI_SUCCESS     Get the supported core & atom cores.
  @retval     EFI_UNSUPPORTED Doesn't support to get the supported core & atom cores.
**/
EFI_STATUS
GetCpuSupportedCoresAndAtomCores (
  OUT UINT8                     *NumberOfSupportedCores,            OPTIONAL
  OUT UINT8                     *NumberOfSupportedAtomCores         OPTIONAL
  );

/**
  Determine Number of system supported cores for Soc-North.

  @param[out] *NumberOfSocNorthSupportedAtomCores              - variable that will store Maximum supported Soc-North Atom cores.

  @retval     EFI_SUCCESS     Get the supported core & atom cores.
  @retval     EFI_UNSUPPORTED Doesn't support to get the supported core & atom cores.
**/
EFI_STATUS
GetSupportedSocNorthAtomCores (
  OUT UINT8                     *NumberOfSocNorthSupportedAtomCores
  );

/**
  This function gets number of SOC Atom cores enabled in the platform.

  @retval      Number of active soc atom core.
**/
UINT8
GetActiveSocNorthAtomCoreCount (
  VOID
  );

/**
  Detect Supported CPU Features

  @param[InOut] CpuEcx   Pointer to CpuEcx (CPUID_VERSION_INFO_ECX).
**/
VOID
GetSupportedCpuFeatures (
  IN UINT32     *CpuEcx
  );

/**
  Detect if Processor Trace supported or not

  @retval TRUE   IntelProcessorTrace is supported and enabled.
  @retval FALSE  IntelProcessorTrace is disabled.
**/
BOOLEAN
IsIntelProcessorTraceSupported (
  VOID
  );


/**
  Returns Generation string of the respective CPU

  @retval      Character pointer of Generation string
**/
CONST CHAR8*
GetGenerationString (
  VOID
  );

/**
  Returns Revision Table

  @retval      Character pointer of Revision Table String
**/
CONST CHAR8*
GetRevisionTable (
  VOID
  );

/**
  Returns Sku String

  @retval      Character pointer of Sku String
**/
CONST CHAR8*
GetSkuString (
  VOID
  );

/**
  Determine if CPU supports Telemetry.

  @retval TRUE   if CPU Supports Telemetry.
  @retval FALSE  if CPU doesn't supports Telemetry.
**/
BOOLEAN
IsTelemetrySupported (
  VOID
  );

/**
  Returns PPAM version details to support Nifty Rock feature based on SKU.

  @retval 0    Nifty Rock feature is not supported, No need to load PPAM.
  @retval 11   Nifty Rock feature is supported with PPAM Version 1.1.
**/
UINT8
NiftyRockSupportLevel (
  VOID
  );

/**
  This function return max and min bus ratio.

  @param[out]  MaxBusRatio
  @param[out]  MinBusRatio
**/
VOID
GetBusRatio (
  OUT UINT8 *MaxBusRatio, OPTIONAL
  OUT UINT8 *MinBusRatio OPTIONAL
  );

/**
  This function return max Efficiency Ratio.

  @retval Max efficiency ratio
**/
UINT8
GetMaxEfficiencyRatio (
  VOID
  );

/**
  This function return max Non-Turbo Ratio.

  @retval Max Non-Turbo Ratio
**/
UINT8
GetMaxNonTurboRatio (
  VOID
  );

/**
  This function return the supported Config TDP Levels.

  @retval number of config TDP levels
**/
UINT8
GetConfigTdpLevels (
  VOID
  );

/**
  This function return the supported Prmmr Size

  @retval  Supported Prmrr Size
**/
UINT32
GetMaxSupportedPrmrrSize (
  VOID
  );

/**
  This function returns the supported Physical Address Size

  @retval supported Physical Address Size.
**/
UINT8
GetMaxPhysicalAddressSize (
  VOID
  );

/**
  Return if Edram Enable

  @retval TRUE             If Edram Enable
  @retval FALSE            If Edram Disable
**/
BOOLEAN
IsEdramEnable (
  VOID
  );

#endif
