/** @file
  Header file for Cpu Common Lib implementation.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation.

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

#ifndef _CPU_COMMON_LIB_H_
#define _CPU_COMMON_LIB_H_

typedef UINT32 CPU_RESET_TYPE;

#define NO_RESET                              0
#define WARM_RESET                            BIT0
#define COLD_RESET                            (BIT0 | BIT1)

///
/// Enums for Time Window Convert Type
///
typedef enum {
  SecondsTimeWindowConvert = 1,
  MilliSecondsTimeWindowConvert,
  TimeWindowConvertMaximum
} TIME_WINDOW_CONV;

/**
  Set up flags in CR4 for XMM instruction enabling
**/
VOID
EFIAPI
XmmInit (
  VOID
  );

/**
  Enable "Machine Check Enable"
**/
VOID
EFIAPI
EnableMce (
  VOID
  );

/**
  Private helper function to convert various Turbo Power Limit Time from Seconds to CPU units

  @param[in] TimeInSeconds       Time in seconds
  @param[in] TimeWindowConvType  Time Window Convert Type

  @retval UINT8 Converted time in CPU units
**/
UINT8
GetConvertedTime (
  IN UINT32            TimeInSeconds,
  IN TIME_WINDOW_CONV  TimeWindowConvType
  );

/**
  Get APIC ID of processor

  @retval APIC ID of processor
**/
UINT32
GetCpuApicId (
  VOID
  );

/**
  Programs XAPIC registers.

  @param[in] Bsp             - Is this BSP?
**/
VOID
ProgramXApic (
  BOOLEAN Bsp
  );

/**
  This function returns the maximum number of cores supported in this physical processor package
  by leverging CPUID_CACHE_PARAMS with offset 26.

  @retval Maximum number of supported cores in the package.
**/
UINT8
EFIAPI
GetMaxSupportedCoreCount (
  VOID
  );

/**
  This function returns the supported number of threads per core,
  and supported total threads in the physical processor package
  by leverging CPUID_EXTENDED_TOPOLOGY with index 1 & 2.

  @param[out] *ThreadsPerCore    - variable that will store Maximum enabled threads per core.
  @param[out] *Threads           - variable that will store supported total threads.
**/
VOID
EFIAPI
GetSupportedThreadCount (
  OUT UINT8 *ThreadsPerCore,  OPTIONAL
  OUT UINT8 *Threads          OPTIONAL
  );

/**
  This function returns the enabled Cores and Threads
  by leverging MSR_CORE_THREAD_COUNT.

  @param[out] *CoreCount            - variable that will store enabled cores.
  @param[out] *ThreadCount          - variable that will store enabled threads.
**/
VOID
EFIAPI
GetEnabledCoreThreadCount (
  OUT UINT8 *CoreCount,   OPTIONAL
  OUT UINT8 *ThreadCount  OPTIONAL
  );

/**
  This function returns the maximum number of dies and packages.
  Currently, the number of dies and packages will be one for client.

  @param[out] *NumberOfDiesPerPackage               - variable that will store Maximum dies per package
  @param[out] *NumberOfPackages                     - variable that will store Maximum Packages
**/
VOID
EFIAPI
CpuGetNumberofDiesAndPackages (
  OUT UINT16                    *NumberOfDiesPerPackage,             OPTIONAL
  OUT UINT16                    *NumberOfPackages                    OPTIONAL
  );

/**
  Check to see if the executing thread is BSP

  @retval TRUE   Executing thread is BSP
  @retval FALSE  Executing thread is AP
**/
BOOLEAN
IsBsp (
  VOID
  );

/**
  Stop PBE timer if system is in Boot Guard boot

  @retval EFI_SUCCESS        - Stop PBE timer
  @retval EFI_UNSUPPORTED    - Not in Boot GuardSupport mode.
**/
EFI_STATUS
StopPbeTimer (
  VOID
  );

/**
  Return if C6DRAM is Enabled.

  @retval TRUE  - C6DRAM is enabled.
  @retval FALSE - C6DRAM is disabled.
**/
BOOLEAN
GetC6DramStatus (
  VOID
  );


/**
  Set C6DRAM Enable/Disable and return if enabled or not.

  @param[in] C6DramStateRequest - Policy setting of C6DRAM

  @retval TRUE  - C6DRAM is enabled.
  @retval FALSE - C6DRAM is disabled.
**/
BOOLEAN
SetC6Dram (
  UINT32 C6DramStateRequest
  );

/**
  Initialize PRMRR core MSRs.

  @param[in]  PrmrrBase - Base address of PRMRR range. Must be naturaly aligned.
  @param[in]  PrmrrSize - Size of the PRMRR range in Bytes
**/
VOID
SetCorePrmrr (
  UINT64 PrmrrBase,
  UINT32 PrmrrSize
  );

/**
  Check on the processor if PRMRR is supported.

  @param[in]  IsBspInt  Check to see if the executing thread is BSP.

  @retval TRUE  if PRMRR supported
  @retval FALSE if PRMRR is not supported
**/
BOOLEAN
IsPrmrrSupported (
  BOOLEAN    IsBspInt
  );

/**
  This function will set and lock Uncore PRMRR which is required to be locked before enabling normal mode
  for memory.

  @param[in]  PrmrrBase - Base address of PRMRR range. Must be naturally aligned.
  @param[in]  PrmrrSize - Size of the PRMRR range in Bytes
**/
VOID
SetUncorePrmrr (
  UINT32 PrmrrBase,
  UINT32 PrmrrSize
  );

/**
  Initialize SGX PRMRR core MSRs.

  @param[in]  IsBspInt  Check to see if the executing thread is BSP.
**/
VOID
InitializeCorePrmrr (
  IN  BOOLEAN IsBspInt
  );

/**
  Return TRUE if PRMRR base was already set on this core and was
  locked

  @retval TRUE  PRMRR base was already set
  @retval FALSE PRMRR base wasn't set
**/
BOOLEAN
IsPrmrrAlreadySet (
  VOID
  );

/**
  Based on ResetType, perform warm or cold reset using PCH reset PPI.

  @param[in] ResetType    - CPU_RESET_TYPE to indicate which reset should be performed.

  @retval EFI_SUCCESS     - Function successful (system should already reset).
  @retval EFI_UNSUPPORTED - Reset type unsupported.
**/
EFI_STATUS
PerformWarmOrColdReset (
  IN CPU_RESET_TYPE ResetType
  );
#endif
