 /** @file
  This file contains Cpu Information for specific generation.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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
#ifndef _CPU_INFO_FRU_LIB_H_
#define _CPU_INFO_FRU_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <CpuRegs.h>
#include <Register/SaRegsHostBridge.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MmPciLib.h>
#include <Library/PcdLib.h>
#include <Library/PchInfoLib.h>

#define SMBIOS_TYPE4_SOCKET_BGA1392           0x3A
#define SMBIOS_TYPE4_SOCKET_BGA1510           0x3B
#define SMBIOS_TYPE4_SOCKET_BGA1528           0x3C

///
/// Override table structure for cTDP and non-cTDP skus.
/// Non-cTDP parts would have '0' data for TDP level information.
///
typedef struct {
  UINTN  CpuIdentifier;
  UINT16 SkuPackageTdp;
  UINTN  MsrPowerLimit1;
  UINTN  MsrPowerLimit2;
  UINTN  CtdpUpPowerLimit1;
  UINTN  CtdpUpPowerLimit2;
  UINTN  CtdpNominalPowerLimit1;
  UINTN  CtdpNominalPowerLimit2;
  UINTN  CtdpDownPowerLimit1;
  UINTN  CtdpDownPowerLimit2;
  UINTN  MsrPowerLimit4;
} PPM_OVERRIDE_TABLE;

/**
  Return CPU Sku

  @param[in]  UINT32             CpuFamilyModel
  @param[in]  UINT16             CpuDid

  @retval     UINT8              CPU Sku
**/
UINT8
GetCpuSkuInfo (
  IN UINT32    CpuFamilyModel,
  IN UINT16    CpuDid
  );

/**
  Get processor generation

  @param[in]  CPU_FAMILY         CpuFamilyModel
  @retval     CPU_GENERATION     Returns the executing thread's processor generation.
**/
CPU_GENERATION
GetCpuSkuGeneration (
  IN CPU_FAMILY         CpuFamilyModel
  );

/**
  This function returns the supported cores count info.

  @param[out] *NumberOfSupportedCores              - variable that will store Maximum supported cores.
  @param[out] *NumberOfSupportedAtomCores          - variable that will store Maximum supported Atom cores.

  @retval     EFI_SUCCESS     Get the supported core count info successfully.
  @retval     EFI_UNSUPPORTED Doesn't support to get the supported core count info.
**/
EFI_STATUS
GetCpuSupportedCoreCountInfo (
  OUT UINT8                     *NumberOfSupportedCores,            OPTIONAL
  OUT UINT8                     *NumberOfSupportedAtomCores         OPTIONAL
  );

/**
  This function returns the Soc-North supported cores count info.

  @param[out] *NumberOfSupportedSocNorthAtomCores              - variable that will store Maximum supported cores.

  @retval     EFI_SUCCESS     Get the supported core count info successfully.
  @retval     EFI_UNSUPPORTED Doesn't support to get the supported core count info.
**/
EFI_STATUS
GetSocNorthSupportedAtomCoresFru (
  OUT UINT8                     *NumberOfSupportedSocNorthAtomCores            OPTIONAL
  );

/**
  This function returns the Soc-North active cores count info.

  @retval     get the active core count info.
**/
UINT8
GetSocNorthActiveAtomCoresFru (
  VOID
  );

/**
  Returns Generation string of the respective CPU

  @param[in]   CpuFamilyId

  @retval      Character pointer of Generation string
**/
CONST CHAR8*
GetFruGenerationString (
  IN   UINT32   CpuFamilyId
  );

/**
  Returns Revision Table string

  @param[in]   CpuFamilyId

  @retval      Character pointer of Revision Table string
**/
CONST CHAR8*
GetRevisionTableString (
  UINT32                CpuId
  );

/**
  Determine if CPU supports Intel Turbo Boost Max Technology 3.0 (ITBM).

  @retval Bit is set if ITBM is supported
**/
BOOLEAN
GetItbmSupportedStatus (
  VOID
);

/**
  Determine if the CPU is 10 Cores SKU.

  @retval TRUE    CPU sku is equal or above 10 Cores
  @retval FALSE   CPU sku is below 10 Cores
**/
BOOLEAN
Is10CoreSkuFru (
  VOID
);

/**
  Determine if CPU supports Telemetry.

  @retval TRUE   if CPU Supports Telemetry.
  @retval FALSE  if CPU doesn't supports Telemetry.
**/
BOOLEAN
IsFruSupportedTelemetry (
  VOID
  );

/**
  This function returns if CPU support SyncFeatures

  @retval TRUE             SyncFeature is supported
  @retval FALSE            SyncFeature is not Supported
**/
BOOLEAN
IsSmmSyncFeatureSupported (
  VOID
  );

/**
  This function returns Number of CBO0 Bank Index.

  @retval Number of CBO0 Bank Index.
**/
UINT8
GetCbo0BankIndex (
  VOID
  );

/**
  Detect if Hetero Core is supported.

  @retval TRUE - Processor Support HeteroCore
  @retval FALSE - Processor doesnt support HeteroCore
**/
BOOLEAN
IsHeteroCoreSupported (
  VOID
  );

/**
  Detect the type of core.

  @retval the core type which is running
**/
UINT8
DetectCoreType (
  VOID
  );

/**
  Returns power limits Table

  @param[in]      NoOfOverrides

  @retval         override table pointer of power limits Table
**/
PPM_OVERRIDE_TABLE*
GetFruPowerLimits (
  IN UINTN        *NoOfOverrides
  );

/**
  This function checks for HWP advanced features like EnableFastMsrHwpReq, EnableHwpAutoEppGrouping, EnableHwpAutoPerCorePstate, EnablePerCorePState.

  @retval TRUE  - Advanced HWP features supported.
  @retval FALSE - Advanced HWP features not supported.
**/
BOOLEAN
IsHwpAdvancedFeatureSupport (
  VOID
  );

/**
  Programs Processor Upgrade for type 4 SMBIOS Processor Info HOB.

  @retval Returns Processor Upgrade value for type 4 SMBIOS Processor Info HOB.
**/
UINT8
SmbiosProcessorInfoHobType4 (
  VOID
  );

/**
  Get CPU default C state latency value.

  @param[out] CstateLatencyControl0Irtl  Interrupt Response Time Limit of LatencyContol0
  @param[out] CstateLatencyControl1Irtl  Interrupt Response Time Limit of LatencyContol1
  @param[out] CstateLatencyControl2Irtl  Interrupt Response Time Limit of LatencyContol2
  @param[out] CstateLatencyControl3Irtl  Interrupt Response Time Limit of LatencyContol3
  @param[out] CstateLatencyControl4Irtl  Interrupt Response Time Limit of LatencyContol4
  @param[out] CstateLatencyControl5Irtl  Interrupt Response Time Limit of LatencyContol5
**/
VOID
CpuGetCstateLatencyDefault (
  OUT UINT16 *CstateLatencyControl0Irtl,
  OUT UINT16 *CstateLatencyControl1Irtl,
  OUT UINT16 *CstateLatencyControl2Irtl,
  OUT UINT16 *CstateLatencyControl3Irtl,
  OUT UINT16 *CstateLatencyControl4Irtl,
  OUT UINT16 *CstateLatencyControl5Irtl
  );

/**
  This function is used to Patch SmmSupovrStateLock.

  @retval This corresponds to bit 2 of MSR_SMM_SUPOVR_STATE_LOCK_REGISTER. When set, prevents WRMSR to IA32_SMM_MONITOR_CTL (aka MSEG) MSR.

**/
BOOLEAN
SmmSupovrStateLockPatch (
  VOID
  );

/**
  This function returns the supported Physical Address Size

  @retval returns the supported Physical Address Size.
**/
UINT8
GetMaxPhysicalAddressSizeFru (
  VOID
  );

/**
  This function return the Heuristics Policy Data for C6DRAM.

  @retval  Return the Heuristics Policy Data for C6DRAM.
**/
UINT8
GetC6DramHeuristicsPolicyData (
  VOID
  );

/**
  This function is used to return 32 bit PRMRR physical base MSR

  @retval 32 bit PRMRR physical base MSR value
**/
UINT32
GetPrmrrBaseMsrAddress (
  VOID
  );

/**
  This function check if CPU support BiosDone MSR

  @retval TRUE    CPU support BiosDone MSR
  @retval FALSE   CPU does not support BiosDone MSR
**/
BOOLEAN
IsMsrSetBiosDoneSupport (
  VOID
  );

/**
  Returns USB2 PHY Reference Clock frequency value
  This defines what electrical tuning parameters shall be used
  during USB2 PHY initialization programming

  @retval Frequency reference clock for USB2 PHY
**/
USB2_PHY_REF_FREQ
GetCpuUsb2PhyRefFreq (
  VOID
  );

/**
  This function checks if CPU support VCCIO VR

  @retval TRUE    CPU support VCCIO VR
  @retval FALSE   CPU does not support VCCIO VR
**/
BOOLEAN
IsCpuVccIoVrSupported (
  VOID
  );

/**
  This function is to check whether Avx enable/disable is supported or not.

  @param[in]      ActiveSmallCoreCount
  @param[in]      IsSimics

  @retval TRUE    Avx enable/disable feature is supported.
  @retval FALSE   Avx enable/disable feature is unsupported.
**/
BOOLEAN
IsAvxSetSupported (
  UINT8   ActiveSmallCoreCount,
  BOOLEAN IsSimics
  );
#endif // _CPU_INFO_FRU_LIB_H_
