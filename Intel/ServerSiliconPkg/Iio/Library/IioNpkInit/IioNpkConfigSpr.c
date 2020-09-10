/** @file
  This file implements IIO Trace Hub function specific for SPR.

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
**/

#include <Base.h>
#include <RcRegs.h>
#include <Library/IioAccessLib.h>

/**
  The PSF shadowing register programming with FW_BAR value.

  @param[in]  IioIndex                   IIO instance (usually socket)
  @param[in]  TraceHubIndex              Trace Hub Index in socket
  @param[in]  FwLbar                     FW_BAR lower 32 bits
  @param[in]  FwUbar                     FW_BAR upper 32 bits
**/
VOID
IioTraceHubPsfShadowPciFunc (
  IN  UINT8                              IioIndex,
  IN  UINT8                              TraceHubIndex,
  IN  UINT32                             FwLbar,
  IN  UINT32                             FwUbar
  )
{
  PSF_1_AGNT_T0_SHDW_PCIEN_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_STRUCT     Psf1ShdwPcien;
  PSF_1_AGNT_T0_SHDW_BAR0_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_STRUCT      Psf1ShdwBar0;
  PSF_1_AGNT_T0_SHDW_BAR1_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_STRUCT      Psf1ShdwBar1;

  Psf1ShdwPcien.Data = IioReadCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_PCIEN_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_REG);
  Psf1ShdwPcien.Bits.memen = 1;
  IioWriteCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_PCIEN_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_REG, Psf1ShdwPcien.Data);

  Psf1ShdwBar0.Data = IioReadCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_BAR0_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_REG);
  Psf1ShdwBar0.Bits.addrbase = FwLbar >> 21;
  IioWriteCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_BAR0_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_REG, Psf1ShdwBar0.Data);

  Psf1ShdwBar1.Data = IioReadCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_BAR1_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_REG);
  Psf1ShdwBar1.Bits.addrbase = FwUbar;
  IioWriteCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_BAR1_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_REG, Psf1ShdwBar1.Data);
}

/**
  Hide CPU Trace Hub Config Space. Used when Trace Hub is in ACPI mode.

  @param[in] IioIndex                   IIO instance (usually socket)
  @param[in] TraceHubIndex              Trace Hub Index in socket
**/
VOID
HideCpuTraceHub (
  IN UINT8                              IioIndex,
  IN UINT8                              TraceHubIndex
  )
{
  PSF_1_AGNT_T0_SHDW_CFG_DIS_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_STRUCT     Psf1ShdwCfgDis;

  Psf1ShdwCfgDis.Data = IioReadCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_CFG_DIS_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_REG);
  Psf1ShdwCfgDis.Bits.cfgdis = 1;
  IioWriteCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_CFG_DIS_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_REG, Psf1ShdwCfgDis.Data);
}

/**
  Disable CPU Trace Hub PSF function.
  Used when disabling Fabric decode for source decode or abort access when in Fabric decode.

  @param[in] IioIndex                   IIO instance (usually socket)
  @param[in] TraceHubIndex              Trace Hub Index in socket
**/
VOID
IioCpuTraceHubPsfFunctionDisable (
  IN UINT8                              IioIndex,
  IN UINT8                              TraceHubIndex
  )
{
  PSF_1_AGNT_T0_SHDW_PCIEN_NPK_RS0_D4_F0_OFFSET7_SB_PSF_DINO_STRUCT     Psf1ShdwPciEn;
  PSF_1_AGNT_T0_SHDW_PCIEN_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_STRUCT     Psf1ShdwPciEnFwBar;

  Psf1ShdwPciEn.Data = IioReadCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_PCIEN_NPK_RS0_D4_F0_OFFSET7_SB_PSF_DINO_REG);
  Psf1ShdwPciEn.Bits.fundis = 1;
  IioWriteCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_PCIEN_NPK_RS0_D4_F0_OFFSET7_SB_PSF_DINO_REG, Psf1ShdwPciEn.Data);

  Psf1ShdwPciEnFwBar.Data = IioReadCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_PCIEN_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_REG);
  Psf1ShdwPciEnFwBar.Bits.fundis = 1;
  IioWriteCpuCsr32 (IioIndex, TraceHubIndex, PSF_1_AGNT_T0_SHDW_PCIEN_NPK_RS0_D4_F1_OFFSET8_SB_PSF_DINO_REG, Psf1ShdwPciEnFwBar.Data);
}
