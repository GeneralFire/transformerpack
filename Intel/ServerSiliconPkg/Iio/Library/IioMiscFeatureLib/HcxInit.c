/** @file
  This file initialize the HCx for SPR

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Library/IioSysInfoLib.h>
#include <Library/KtiApi.h>
#include <LibraryPrivate/IioDebug.h>

#define CPM_PARAM_BAR_LENGTH    0x80000             // CPM BAR0
#define CPM_PMISC_BAR_LENGTH    0x800000            // CPM BAR1
#define CPM_CSR_BAR_LENGTH      0x200000            // CPM BAR2
#define CPM_VF_BAR0_LENGTH      (0x8000 * 0x80)     // HQM VFBAR0: VF number is 0x80
#define CPM_VF_BAR1_LENGTH      (0x2000 * 0x80)     // HQM VFBAR1: VF number is 0x80
#define CPM_VF_BAR2_LENGTH      (0x8000 * 0x80)     // HQM VFBAR2: VF number is 0x80

#define HQM_PF_BAR_LENGTH       0x4000000           // HQM BAR0
#define HQM_CSR_BAR_LENGTH      0x100000000         // HQM BAR1
#define HQM_VF_BAR_LENGTH       (0x4000000 * 0x10)  // HQM VFBAR0: VF number is 0x10

/**
  Allocate BUS Resource for HCA PSF
  @param[in] IioIndex           - Index to Socket
  @param[in] HcxInstance        - HCx PSF Box Instance
  @param[in] RootBus            - HCx Root Bus number

  @retval None
**/
VOID
EFIAPI
IioAllocateBusResourceForHca (
  IN  UINT8              IioIndex,
  IN  UINT8              HcxInstance,
  IN  UINT8              RootBus
  )
{
  PSF_2_BUS_SHADOW_RS0_SB_PSF_HCX_STRUCT                            Psf2BusShadowRs0Config;
  PSF_2_AGNT_T1_SHDW_BUS_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_STRUCT  Cpm0BusConfig;
  PSF_2_AGNT_T1_SHDW_BUS_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_STRUCT  Hqm0BusConfig;

  Psf2BusShadowRs0Config.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_BUS_SHADOW_RS0_SB_PSF_HCX_REG);
  Cpm0BusConfig.Data          = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG);
  Hqm0BusConfig.Data          = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG);

  Psf2BusShadowRs0Config.Bits.busnumber = RootBus;
  Cpm0BusConfig.Bits.primbus            = RootBus;
  Cpm0BusConfig.Bits.secbus             = RootBus + CPM_BUS_OFFSET;
  Cpm0BusConfig.Bits.subbus             = RootBus + CPM_BUS_OFFSET;
  Hqm0BusConfig.Bits.primbus            = RootBus;
  Hqm0BusConfig.Bits.secbus             = RootBus + HQM_BUS_OFFSET;
  Hqm0BusConfig.Bits.subbus             = RootBus + HQM_BUS_OFFSET;

  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_BUS_SHADOW_RS0_SB_PSF_HCX_REG, Psf2BusShadowRs0Config.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG, Cpm0BusConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG, Hqm0BusConfig.Data);

  return;
}

/**
  Allocate BUS Resource for HCB PSF
  @param[in] IioIndex           - Index to Socket
  @param[in] HcxInstance        - HCx PSF Box Instance
  @param[in] RootBus            - HCx Root Bus number

  @retval None
**/
VOID
EFIAPI
IioAllocateBusResourceForHcb (
  IN  UINT8              IioIndex,
  IN  UINT8              HcxInstance,
  IN  UINT8              RootBus
  )
{
  PSF_2_BUS_SHADOW_RS0_SB_PSF_HCB_STRUCT                            Psf2BusShadowRs0Config;
  PSF_2_AGNT_T1_SHDW_BUS_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT  Cpm0BusConfig;
  PSF_2_AGNT_T1_SHDW_BUS_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT  Cpm1BusConfig;
  PSF_2_AGNT_T1_SHDW_BUS_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT  Hqm0BusConfig;
  PSF_2_AGNT_T1_SHDW_BUS_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT  Hqm1BusConfig;

  Psf2BusShadowRs0Config.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_BUS_SHADOW_RS0_SB_PSF_HCB_REG);
  Cpm0BusConfig.Data          = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG);
  Cpm1BusConfig.Data          = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG);
  Hqm0BusConfig.Data          = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG);
  Hqm1BusConfig.Data          = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG);

  Psf2BusShadowRs0Config.Bits.busnumber = RootBus;
  Cpm0BusConfig.Bits.primbus            = RootBus;
  Cpm0BusConfig.Bits.secbus             = RootBus + CPM_BUS_OFFSET;
  Cpm0BusConfig.Bits.subbus             = RootBus + CPM_BUS_OFFSET;
  Hqm0BusConfig.Bits.primbus            = RootBus;
  Hqm0BusConfig.Bits.secbus             = RootBus + HQM_BUS_OFFSET;
  Hqm0BusConfig.Bits.subbus             = RootBus + HQM_BUS_OFFSET;
  Cpm1BusConfig.Bits.primbus            = RootBus;
  Cpm1BusConfig.Bits.secbus             = RootBus + CPM1_BUS_OFFSET;
  Cpm1BusConfig.Bits.subbus             = RootBus + CPM1_BUS_OFFSET;
  Hqm1BusConfig.Bits.primbus            = RootBus;
  Hqm1BusConfig.Bits.secbus             = RootBus + HQM1_BUS_OFFSET;
  Hqm1BusConfig.Bits.subbus             = RootBus + HQM1_BUS_OFFSET;

  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_BUS_SHADOW_RS0_SB_PSF_HCB_REG, Psf2BusShadowRs0Config.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG, Cpm0BusConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG, Cpm1BusConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG, Hqm0BusConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_BUS_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG, Hqm1BusConfig.Data);

  return;
}

/**
  Allocate BUS Resource for HCx PSF
  @param[in] IioIndex           - Index to Socket
  @param[in] HcxType            - HCx sub-system type
  @param[in] HcxInstance        - HCx PSF Box Instance
  @param[in] RootBus            - HCx Root Bus number

  @retval None
**/
VOID
EFIAPI
IioAllocateBusResourceForHcx (
  IN  UINT8              IioIndex,
  IN  IIO_HCX_TYPE       HcxType,
  IN  UINT8              HcxInstance,
  IN  UINT8              RootBus
  )
{
  if (HcxType == IIO_HCB) {
    IioAllocateBusResourceForHcb (IioIndex, HcxInstance, RootBus);
  } else {
    IioAllocateBusResourceForHca (IioIndex, HcxInstance, RootBus);
  }
  return;
}

/**
  Set HCA PSF PciEn
  @param[in] IioIndex           - Index to Socket
  @param[in] HcxInstance        - HCx PSF Box Instance

  @retval None
**/
VOID
EFIAPI
IioSetHcaPciEn (
  IN  UINT8              IioIndex,
  IN  UINT8              HcxInstance
  )
{
  PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_STRUCT  Cpm0PciEnConfig;
  PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_STRUCT  Hqm0PciEnConfig;

  Cpm0PciEnConfig.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG);
  Hqm0PciEnConfig.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG);

  Cpm0PciEnConfig.Bits.ioen  = 0;
  Cpm0PciEnConfig.Bits.memen = 1;
  Hqm0PciEnConfig.Bits.ioen  = 0;
  Hqm0PciEnConfig.Bits.memen = 1;

  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG, Cpm0PciEnConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG, Hqm0PciEnConfig.Data);

  return;
}

/**
  Allocate MMIO Resource for HCA PSF
  @param[in] IioIndex           - Index to Socket
  @param[in] HcxInstance        - HCx PSF Box Instance
  @param[in] HcxMmioBase        - HCx MMIO Base

  @retval None
**/
VOID
EFIAPI
IioAllocateMmioResourceForHca (
  IN  UINT8              IioIndex,
  IN  UINT8              HcxInstance,
  IN  UINT64             HcxMmioBase
  )
{
  UINT64         CpmMmioBase;
  UINT64         CpmMmioLimit;
  UINT64         HqmMmioBase;
  UINT64         HqmMmioLimit;
  PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_STRUCT      Cpm0PrefMmioConfig;
  PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_STRUCT   Cpm0PrefMmioBaseConfigExt;
  PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_STRUCT  Cpm0PrefMmioLimitConfigExt;
  PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_STRUCT      Hqm0PrefMmioConfig;
  PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_STRUCT   Hqm0PrefMmioBaseConfigExt;
  PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_STRUCT  Hqm0PrefMmioLimitConfigExt;

  CpmMmioBase  = HcxMmioBase;
  CpmMmioLimit = CpmMmioBase + CPM_MMIO_SIZE - 1;
  HqmMmioBase  = HcxMmioBase + CPM_MMIO_SIZE;
  HqmMmioLimit = HqmMmioBase + HQM_MMIO_SIZE - 1;

  Cpm0PrefMmioConfig.Data         = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG);
  Cpm0PrefMmioBaseConfigExt.Data  = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG);
  Cpm0PrefMmioLimitConfigExt.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG);
  Hqm0PrefMmioConfig.Data         = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG);
  Hqm0PrefMmioBaseConfigExt.Data  = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG);
  Hqm0PrefMmioLimitConfigExt.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG);

  // Set CPM MMIO base
  Cpm0PrefMmioConfig.Bits.addrbase         = ((UINT32) CpmMmioBase) >> 20;
  Cpm0PrefMmioBaseConfigExt.Bits.addrbase  = (UINT32) RShiftU64 (CpmMmioBase, 32);
  // Set CPM MMIO limit
  Cpm0PrefMmioConfig.Bits.addrlimit        = ((UINT32) CpmMmioLimit) >> 20;
  Cpm0PrefMmioLimitConfigExt.Bits.addrbase = (UINT32) RShiftU64 (CpmMmioLimit, 32);
  // Set HQM MMIO base
  Hqm0PrefMmioConfig.Bits.addrbase         = ((UINT32) HqmMmioBase) >> 20;
  Hqm0PrefMmioBaseConfigExt.Bits.addrbase  = (UINT32) RShiftU64 (HqmMmioBase, 32);
  // Set HQM MMIO limit
  Hqm0PrefMmioConfig.Bits.addrlimit        = ((UINT32) HqmMmioLimit) >> 20;
  Hqm0PrefMmioLimitConfigExt.Bits.addrbase = (UINT32) RShiftU64 (HqmMmioLimit, 32);

  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG, Cpm0PrefMmioConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG, Cpm0PrefMmioBaseConfigExt.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG, Cpm0PrefMmioLimitConfigExt.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG, Hqm0PrefMmioConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG, Hqm0PrefMmioBaseConfigExt.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG, Hqm0PrefMmioLimitConfigExt.Data);

  // Set HCA PSF PciEn
  IioSetHcaPciEn (IioIndex, HcxInstance);
  return;
}

/**
  Set HCB PSF PciEn
  @param[in] IioIndex           - Index to Socket
  @param[in] HcxInstance        - HCx PSF Box Instance

  @retval None
**/
VOID
EFIAPI
IioSetHcbPciEn (
  IN  UINT8              IioIndex,
  IN  UINT8              HcxInstance
  )
{
  PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT  Cpm0PciEnConfig;
  PSF_2_AGNT_T1_SHDW_PCIEN_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT  Cpm1PciEnConfig;
  PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT  Hqm0PciEnConfig;
  PSF_2_AGNT_T1_SHDW_PCIEN_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT  Hqm1PciEnConfig;

  Cpm0PciEnConfig.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG);
  Cpm1PciEnConfig.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG);
  Hqm0PciEnConfig.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG);
  Hqm1PciEnConfig.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG);

  Cpm0PciEnConfig.Bits.ioen  = 0;
  Cpm0PciEnConfig.Bits.memen = 1;
  Hqm0PciEnConfig.Bits.ioen  = 0;
  Hqm0PciEnConfig.Bits.memen = 1;
  Cpm1PciEnConfig.Bits.ioen  = 0;
  Cpm1PciEnConfig.Bits.memen = 1;
  Hqm1PciEnConfig.Bits.ioen  = 0;
  Hqm1PciEnConfig.Bits.memen = 1;

  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG, Cpm0PciEnConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG, Cpm1PciEnConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG, Hqm0PciEnConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PCIEN_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG, Hqm1PciEnConfig.Data);

  return;
}

/**
  Allocate MMIO Resource for HCB PSF
  @param[in] IioIndex           - Index to Socket
  @param[in] HcxInstance        - HCx PSF Box Instance
  @param[in] HcxMmioBase        - HCx MMIO Base

  @retval None
**/
VOID
EFIAPI
IioAllocateMmioResourceForHcb (
  IN  UINT8              IioIndex,
  IN  UINT8              HcxInstance,
  IN  UINT64             HcxMmioBase
  )
{
  UINT64         Cpm0MmioBase;
  UINT64         Cpm0MmioLimit;
  UINT64         Cpm1MmioBase;
  UINT64         Cpm1MmioLimit;
  UINT64         Hqm0MmioBase;
  UINT64         Hqm0MmioLimit;
  UINT64         Hqm1MmioBase;
  UINT64         Hqm1MmioLimit;
  PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT      Cpm0PrefMmioConfig;
  PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT   Cpm0PrefMmioBaseConfigExt;
  PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_STRUCT  Cpm0PrefMmioLimitConfigExt;
  PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT      Cpm1PrefMmioConfig;
  PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT   Cpm1PrefMmioBaseConfigExt;
  PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_STRUCT  Cpm1PrefMmioLimitConfigExt;
  PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT      Hqm0PrefMmioConfig;
  PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT   Hqm0PrefMmioBaseConfigExt;
  PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_STRUCT  Hqm0PrefMmioLimitConfigExt;
  PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT      Hqm1PrefMmioConfig;
  PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT   Hqm1PrefMmioBaseConfigExt;
  PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_STRUCT  Hqm1PrefMmioLimitConfigExt;

  Cpm0MmioBase  = HcxMmioBase;
  Cpm0MmioLimit = Cpm0MmioBase + CPM_MMIO_SIZE - 1;
  Hqm0MmioBase  = Cpm0MmioBase + CPM_MMIO_SIZE;
  Hqm0MmioLimit = Hqm0MmioBase + HQM_MMIO_SIZE - 1;
  Cpm1MmioBase  = Hqm0MmioBase + HQM_MMIO_SIZE;
  Cpm1MmioLimit = Cpm1MmioBase + CPM_MMIO_SIZE - 1;
  Hqm1MmioBase  = Cpm1MmioBase + CPM_MMIO_SIZE;
  Hqm1MmioLimit = Hqm1MmioBase + HQM_MMIO_SIZE - 1;

  Cpm0PrefMmioConfig.Data         = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG);
  Cpm0PrefMmioBaseConfigExt.Data  = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG);
  Cpm0PrefMmioLimitConfigExt.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG);
  Cpm1PrefMmioConfig.Data         = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG);
  Cpm1PrefMmioBaseConfigExt.Data  = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG);
  Cpm1PrefMmioLimitConfigExt.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG);
  Hqm0PrefMmioConfig.Data         = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG);
  Hqm0PrefMmioBaseConfigExt.Data  = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG);
  Hqm0PrefMmioLimitConfigExt.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG);
  Hqm1PrefMmioConfig.Data         = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG);
  Hqm1PrefMmioBaseConfigExt.Data  = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG);
  Hqm1PrefMmioLimitConfigExt.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG);

  // Set CPM0 MMIO base
  Cpm0PrefMmioConfig.Bits.addrbase         = ((UINT32) Cpm0MmioBase) >> 20;
  Cpm0PrefMmioBaseConfigExt.Bits.addrbase  = (UINT32) RShiftU64 (Cpm0MmioBase, 32);
  // Set CPM0 MMIO limit
  Cpm0PrefMmioConfig.Bits.addrlimit        = ((UINT32) Cpm0MmioLimit) >> 20;
  Cpm0PrefMmioLimitConfigExt.Bits.addrbase = (UINT32) RShiftU64 (Cpm0MmioLimit, 32);
  // Set HQM0 MMIO base
  Hqm0PrefMmioConfig.Bits.addrbase         = ((UINT32) Hqm0MmioBase) >> 20;
  Hqm0PrefMmioBaseConfigExt.Bits.addrbase  = (UINT32) RShiftU64 (Hqm0MmioBase, 32);
  // Set HQM0 MMIO limit
  Hqm0PrefMmioConfig.Bits.addrlimit        = ((UINT32) Hqm0MmioLimit) >> 20;
  Hqm0PrefMmioLimitConfigExt.Bits.addrbase = (UINT32) RShiftU64 (Hqm0MmioLimit, 32);

  // Set CPM1 MMIO base
  Cpm1PrefMmioConfig.Bits.addrbase         = ((UINT32) Cpm1MmioBase) >> 20;
  Cpm1PrefMmioBaseConfigExt.Bits.addrbase  = (UINT32) RShiftU64 (Cpm1MmioBase, 32);
  // Set CPM1 MMIO limit
  Cpm1PrefMmioConfig.Bits.addrlimit        = ((UINT32) Cpm1MmioLimit) >> 20;
  Cpm1PrefMmioLimitConfigExt.Bits.addrbase = (UINT32) RShiftU64 (Cpm1MmioLimit, 32);
  // Set HQM1 MMIO base
  Hqm1PrefMmioConfig.Bits.addrbase         = ((UINT32) Hqm1MmioBase) >> 20;
  Hqm1PrefMmioBaseConfigExt.Bits.addrbase  = (UINT32) RShiftU64 (Hqm1MmioBase, 32);
  // Set HQM1 MMIO limit
  Hqm1PrefMmioConfig.Bits.addrlimit        = ((UINT32) Hqm1MmioLimit) >> 20;
  Hqm1PrefMmioLimitConfigExt.Bits.addrbase = (UINT32) RShiftU64 (Hqm1MmioLimit, 32);

  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG, Cpm0PrefMmioConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG, Cpm0PrefMmioBaseConfigExt.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCB_REG, Cpm0PrefMmioLimitConfigExt.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG, Cpm1PrefMmioConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG, Cpm1PrefMmioBaseConfigExt.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_CPM1_RS0_D7_F0_OFFSET65_SB_PSF_HCB_REG, Cpm1PrefMmioLimitConfigExt.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG, Hqm0PrefMmioConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG, Hqm0PrefMmioBaseConfigExt.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM0_RS0_D8_F0_OFFSET66_SB_PSF_HCB_REG, Hqm0PrefMmioLimitConfigExt.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMRANGE_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG, Hqm1PrefMmioConfig.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMBASE_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG, Hqm1PrefMmioBaseConfigExt.Data);
  IioWriteCpuCsr32 (IioIndex, HcxInstance, PSF_2_AGNT_T1_SHDW_PREF_MEMLIMIT_EXT_HQM1_RS0_D9_F0_OFFSET67_SB_PSF_HCB_REG, Hqm1PrefMmioLimitConfigExt.Data);

  // Set HCB PSF PciEn
  IioSetHcbPciEn (IioIndex, HcxInstance);
  return;
}

/**
  Allocate MMIO Resource for HCx PSF
  @param[in] IioIndex           - Index to Socket
  @param[in] HcxType            - HCx sub-system type
  @param[in] HcxInstance        - HCx PSF Box Instance
  @param[in] HcxMmioBase        - HCx MMIO Base

  @retval None
**/
VOID
EFIAPI
IioAllocateMmioResourceForHcx (
  IN  UINT8              IioIndex,
  IN  IIO_HCX_TYPE       HcxType,
  IN  UINT8              HcxInstance,
  IN  UINT64             HcxMmioBase
  )
{
  if (HcxType == IIO_HCB) {
    IioAllocateMmioResourceForHcb (IioIndex, HcxInstance, HcxMmioBase);
  } else {
    IioAllocateMmioResourceForHca (IioIndex, HcxInstance, HcxMmioBase);
  }
  return;
}

/**
  Init CPM BARs
  @param[in] IioIndex           - Index to Socket
  @param[in] Instance           - HCx IP Instance
  @param[in] CpmMmioBase        - CPM MMIO Base Address

  @retval None
**/
VOID
EFIAPI
IioCpmBarInit (
  IN  UINT8              IioIndex,
  IN  UINT8              Instance,
  IN  UINT64             CpmMmioBase
  )
{
  UINT64                                  MmioBase;
  PARAMLBAR_IIO_CPM_PF_STRUCT             CpmParamLBar;
  PARAMUBAR_IIO_CPM_PF_STRUCT             CpmParamUBar;
  PMISCLBAR_IIO_CPM_PF_STRUCT             CpmPmiscLBar;
  PMISCUBAR_IIO_CPM_PF_STRUCT             CpmPmiscUBar;
  PWQMRINGCSRLBAR_IIO_CPM_PF_STRUCT       CpmPwqMringCsrLBar;
  PWQMRINGCSRUBAR_IIO_CPM_PF_STRUCT       CpmPwqMringCsrUBar;
  PSRIOVLBAR0_IIO_CPM_PF_STRUCT           CpmSriovLBar0;
  PSRIOVUBAR0_IIO_CPM_PF_STRUCT           CpmSriovUBar0;
  PSRIOVLBAR1_IIO_CPM_PF_STRUCT           CpmSriovLBar1;
  PSRIOVUBAR1_IIO_CPM_PF_STRUCT           CpmSriovUBar1;
  PSRIOVLBAR2_IIO_CPM_PF_STRUCT           CpmSriovLBar2;
  PSRIOVUBAR2_IIO_CPM_PF_STRUCT           CpmSriovUBar2;

  MmioBase          = CpmMmioBase;
  CpmPmiscLBar.Data = IioReadCpuCsr32 (IioIndex, Instance, PMISCLBAR_IIO_CPM_PF_REG);
  CpmPmiscUBar.Data = IioReadCpuCsr32 (IioIndex, Instance, PMISCUBAR_IIO_CPM_PF_REG);
  CpmPmiscLBar.Bits.addr = ((UINT32) MmioBase) >> 23;
  CpmPmiscUBar.Bits.addr = (UINT32) RShiftU64 (MmioBase, 32);
  IioWriteCpuCsr32 (IioIndex, Instance, PMISCLBAR_IIO_CPM_PF_REG, CpmPmiscLBar.Data);
  IioWriteCpuCsr32 (IioIndex, Instance, PMISCUBAR_IIO_CPM_PF_REG, CpmPmiscUBar.Data);

  MmioBase         += CPM_PMISC_BAR_LENGTH;
  CpmPwqMringCsrLBar.Data = IioReadCpuCsr32 (IioIndex, Instance, PWQMRINGCSRLBAR_IIO_CPM_PF_REG);
  CpmPwqMringCsrUBar.Data = IioReadCpuCsr32 (IioIndex, Instance, PWQMRINGCSRUBAR_IIO_CPM_PF_REG);
  CpmPwqMringCsrLBar.Bits.addr = ((UINT32) MmioBase) >> 21;
  CpmPwqMringCsrUBar.Bits.addr = (UINT32) RShiftU64 (MmioBase, 32);
  IioWriteCpuCsr32 (IioIndex, Instance, PWQMRINGCSRLBAR_IIO_CPM_PF_REG, CpmPwqMringCsrLBar.Data);
  IioWriteCpuCsr32 (IioIndex, Instance, PWQMRINGCSRUBAR_IIO_CPM_PF_REG, CpmPwqMringCsrUBar.Data);

  MmioBase         += CPM_CSR_BAR_LENGTH;
  CpmParamLBar.Data = IioReadCpuCsr32 (IioIndex, Instance, PARAMLBAR_IIO_CPM_PF_REG);
  CpmParamUBar.Data = IioReadCpuCsr32 (IioIndex, Instance, PARAMUBAR_IIO_CPM_PF_REG);
  CpmParamLBar.Bits.addr = ((UINT32) MmioBase) >> 19;
  CpmParamUBar.Bits.addr = (UINT32) RShiftU64 (MmioBase, 32);
  IioWriteCpuCsr32 (IioIndex, Instance, PARAMLBAR_IIO_CPM_PF_REG, CpmParamLBar.Data);
  IioWriteCpuCsr32 (IioIndex, Instance, PARAMUBAR_IIO_CPM_PF_REG, CpmParamUBar.Data);

  MmioBase          += CPM_PARAM_BAR_LENGTH;
  CpmSriovLBar0.Data = IioReadCpuCsr32 (IioIndex, Instance, PSRIOVLBAR0_IIO_CPM_PF_REG);
  CpmSriovUBar0.Data = IioReadCpuCsr32 (IioIndex, Instance, PSRIOVUBAR0_IIO_CPM_PF_REG);
  CpmSriovLBar0.Bits.vfbar0 = ((UINT32) MmioBase) >> 15;
  CpmSriovUBar0.Bits.vfbar0 = (UINT32) RShiftU64 (MmioBase, 32);
  IioWriteCpuCsr32 (IioIndex, Instance, PSRIOVLBAR0_IIO_CPM_PF_REG, CpmSriovLBar0.Data);
  IioWriteCpuCsr32 (IioIndex, Instance, PSRIOVUBAR0_IIO_CPM_PF_REG, CpmSriovUBar0.Data);

  MmioBase          += CPM_VF_BAR0_LENGTH;
  CpmSriovLBar2.Data = IioReadCpuCsr32 (IioIndex, Instance, PSRIOVLBAR2_IIO_CPM_PF_REG);
  CpmSriovUBar2.Data = IioReadCpuCsr32 (IioIndex, Instance, PSRIOVUBAR2_IIO_CPM_PF_REG);
  CpmSriovLBar2.Bits.vfbar2 = ((UINT32) MmioBase) >> 15;
  CpmSriovUBar2.Bits.vfbar2 = (UINT32) RShiftU64 (MmioBase, 32);
  IioWriteCpuCsr32 (IioIndex, Instance, PSRIOVLBAR2_IIO_CPM_PF_REG, CpmSriovLBar2.Data);
  IioWriteCpuCsr32 (IioIndex, Instance, PSRIOVUBAR2_IIO_CPM_PF_REG, CpmSriovUBar2.Data);

  MmioBase          += CPM_VF_BAR2_LENGTH;
  CpmSriovLBar1.Data = IioReadCpuCsr32 (IioIndex, Instance, PSRIOVLBAR1_IIO_CPM_PF_REG);
  CpmSriovUBar1.Data = IioReadCpuCsr32 (IioIndex, Instance, PSRIOVUBAR1_IIO_CPM_PF_REG);
  CpmSriovLBar1.Bits.vfbar1 = ((UINT32) MmioBase) >> 13;
  CpmSriovUBar1.Bits.vfbar1 = (UINT32) RShiftU64 (MmioBase, 32);
  IioWriteCpuCsr32 (IioIndex, Instance, PSRIOVLBAR1_IIO_CPM_PF_REG, CpmSriovLBar1.Data);
  IioWriteCpuCsr32 (IioIndex, Instance, PSRIOVUBAR1_IIO_CPM_PF_REG, CpmSriovUBar1.Data);

}

/**
  Init HQM BARs
  @param[in] IioIndex           - Index to Socket
  @param[in] Instance           - HCx IP Instance
  @param[in] CpmMmioBase        - HQM MMIO Base Address

  @retval None
**/
VOID
EFIAPI
IioHqmBarInit (
  IN  UINT8              IioIndex,
  IN  UINT8              Instance,
  IN  UINT64             HqmMmioBase
  )
{
  UINT64                                  MmioBase;
  FUNC_BAR_L_IIO_HQM_PF_STRUCT            HqmFuncLBar;
  FUNC_BAR_U_IIO_HQM_PF_STRUCT            HqmFuncUBar;
  CSR_BAR_U_IIO_HQM_PF_STRUCT             HqmCsrUBar;
  SRIOV_CAP_FUNC_BAR_L_IIO_HQM_PF_STRUCT  HqmSriovCapFuncLBar;
  SRIOV_CAP_FUNC_BAR_U_IIO_HQM_PF_STRUCT  HqmSriovCapFuncUBar;

  //
  // Init HQM BAR
  //
  MmioBase        = HqmMmioBase;
  HqmCsrUBar.Data = IioReadCpuCsr32 (IioIndex, Instance, CSR_BAR_U_IIO_HQM_PF_REG);
  HqmCsrUBar.Bits.addr = (UINT32) RShiftU64 (MmioBase, 32);
  IioWriteCpuCsr32 (IioIndex, Instance, CSR_BAR_U_IIO_HQM_PF_REG, HqmCsrUBar.Data);

  MmioBase                     += HQM_CSR_BAR_LENGTH;
  HqmSriovCapFuncLBar.Data      = IioReadCpuCsr32 (IioIndex, Instance, SRIOV_CAP_FUNC_BAR_L_IIO_HQM_PF_REG);
  HqmSriovCapFuncUBar.Data      = IioReadCpuCsr32 (IioIndex, Instance, SRIOV_CAP_FUNC_BAR_U_IIO_HQM_PF_REG);
  HqmSriovCapFuncLBar.Bits.vfbar0_l  = ((UINT32) MmioBase) >> 26;
  HqmSriovCapFuncUBar.Bits.vfbar0    = (UINT32) RShiftU64 (MmioBase, 32);
  IioWriteCpuCsr32 (IioIndex, Instance, SRIOV_CAP_FUNC_BAR_L_IIO_HQM_PF_REG, HqmSriovCapFuncLBar.Data);
  IioWriteCpuCsr32 (IioIndex, Instance, SRIOV_CAP_FUNC_BAR_U_IIO_HQM_PF_REG, HqmSriovCapFuncUBar.Data);

  MmioBase        += HQM_VF_BAR_LENGTH;
  HqmFuncLBar.Data = IioReadCpuCsr32 (IioIndex, Instance, FUNC_BAR_L_IIO_HQM_PF_REG);
  HqmFuncUBar.Data = IioReadCpuCsr32 (IioIndex, Instance, FUNC_BAR_U_IIO_HQM_PF_REG);
  HqmFuncLBar.Bits.addr_l = ((UINT32) MmioBase) >> 26;
  HqmFuncUBar.Bits.addr = (UINT32) RShiftU64 (MmioBase, 32);
  IioWriteCpuCsr32 (IioIndex, Instance, FUNC_BAR_L_IIO_HQM_PF_REG, HqmFuncLBar.Data);
  IioWriteCpuCsr32 (IioIndex, Instance, FUNC_BAR_U_IIO_HQM_PF_REG, HqmFuncUBar.Data);

}

/**
  IIO HCx Init.
  @param[in] IioGlobalData  - IioGlobalData Pointer
  @param[in] IioIndex       - Index to Socket

  @retval None
**/
VOID
EFIAPI
IioHcxInit (
  IN  IIO_GLOBALS         *IioGlobalData,
  IN  UINT8               IioIndex
  )
{
  UINT8                 RootBus;
  UINT64                MmioBase;
  UINT64                MmioLimit;
  UINT64                HcxMmioBase;
  UINT64                CpmMmioBase;
  UINT64                HqmMmioBase;
  UINT8                 StackIndex;
  UINT8                 HcxInstance;
  KTI_STACK_RESOURCE    StackResource;
  IIO_HCX_TYPE          HcxType;
  UINT32                Psf2GlobalConfigAddress;
  PSF_2_PSF_GLOBAL_CONFIG_SB_PSF_HCX_STRUCT     Psf2GlobalConfigHcx;

  IIO_D_LOG ("IIO_HCX_INIT_START\n");

  HcxType = GetHcxType (IioIndex);
  if ((HcxType != IIO_HCA) && (HcxType != IIO_HCB)) {
    return;
  }

  for (StackIndex = IIO_STACK8; StackIndex < MAX_IIO_STACK_SPR; StackIndex++) {
    if (IfStackPresent (IioIndex, StackIndex)) {
      HcxInstance = StackIndex - IIO_STACK8;
      //
      // PSF2 global config
      //
      if (HcxType == IIO_HCB) {
        Psf2GlobalConfigAddress = PSF_2_PSF_GLOBAL_CONFIG_SB_PSF_HCB_REG;
      } else {
        Psf2GlobalConfigAddress = PSF_2_PSF_GLOBAL_CONFIG_SB_PSF_HCX_REG;
      }
      Psf2GlobalConfigHcx.Data = IioReadCpuCsr32 (IioIndex, HcxInstance, Psf2GlobalConfigAddress);
      // Enable Command Parity Detection
      Psf2GlobalConfigHcx.Bits.encmdpardetingress = 1;
      Psf2GlobalConfigHcx.Bits.encmdpardetegress = 1;
      // Enable Early Exit From Idle
      Psf2GlobalConfigHcx.Bits.eneei = 1;
      IioWriteCpuCsr32 (IioIndex, HcxInstance, Psf2GlobalConfigAddress, Psf2GlobalConfigHcx.Data);

      GetStackResouce (IioIndex, StackIndex, &StackResource);

      //
      // Allocate BUS Resource for HCx PSF
      //
      RootBus  = StackResource.BusBase;
      IioAllocateBusResourceForHcx (IioIndex, HcxType, HcxInstance, RootBus);

      MmioBase  = ((UINT64) LShiftU64 (StackResource.MmiohBase.hi, 32)) | StackResource.MmiohBase.lo;
      MmioLimit = ((UINT64) LShiftU64 (StackResource.MmiohLimit.hi, 32)) | StackResource.MmiohLimit.lo;
      if ((MmioLimit - MmioBase) < (HQM_MMIO_SIZE + CPM_MMIO_SIZE)) { // DINO didn't have enough MMIO resources for HCx.
        return;
      }

      //
      // Allocate MMIO Resource for HCx PSF
      //
      HcxMmioBase = MmioBase;
      IioAllocateMmioResourceForHcx (IioIndex, HcxType, HcxInstance, HcxMmioBase);

      //
      // Init CPM/HQM BARs
      //
      CpmMmioBase = HcxMmioBase;
      IioCpmBarInit (IioIndex, HcxInstance, CpmMmioBase);
      HqmMmioBase = HcxMmioBase + CPM_MMIO_SIZE;
      IioHqmBarInit (IioIndex, HcxInstance, HqmMmioBase);
      if (HcxType == IIO_HCB) {
        CpmMmioBase = HqmMmioBase + HQM_MMIO_SIZE;
        IioCpmBarInit (IioIndex, HcxInstance + 1, CpmMmioBase);
        HqmMmioBase = CpmMmioBase + CPM_MMIO_SIZE;
        IioHqmBarInit (IioIndex, HcxInstance + 1, HqmMmioBase);
      }
    }
  }
  IIO_D_LOG ("IIO_HCX_INIT_END\n");
  return;
}

