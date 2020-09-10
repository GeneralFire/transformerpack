/** @file
  TGL PCH PMC SoC library.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PmcLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/SataSocLib.h>
#include <Register/PmcRegs.h>
#include <Register/PmcRegsEbg.h>

#include <Library/PmcSocLib.h>

/**
  This function enables all SerailIo devices.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableSerialIo (
  VOID
  )
{
  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_2, (UINT32)~B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO);
}

/**
  This function disables (static power gating) all SerailIo devices.
  For SerialIo controllers they can be power gated only if all of them are to be disabled.
  They cannot be statically power gated separately.
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.
**/
VOID
PmcStaticDisableSerialIo (
  VOID
  )
{
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_2, B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO);
}

/**
  This function checks if all SerialIo devices are statically disabled (static power gating)

  @retval SerialIo disable state
**/
BOOLEAN
PmcIsSerialIoStaticallyDisabled (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_2) & B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO) == B_PMC_PWRM_ST_PG_FDIS_PMC_2_SERIALIO);
}

/**
  This function checks if SerialIo device is supported (not disabled by fuse)

  @retval SerialIo support state
**/
BOOLEAN
PmcIsSerialIoSupported (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_FUSE_DIS_RD_2) & B_PMC_PWRM_FUSE_DIS_RD_2_SERIALIO_FUSE_SS_DIS) == 0);
}

/**
  This function disables (non-static power gating) SATA and enables ModPHY SPD gating (PCH-LP only).

  @param[in]  SataCtrlIndex     SATA controller index
**/
VOID
PmcDisableSata (
  IN UINT32     SataCtrlIndex
  )
{
  UINT32 PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  ASSERT (SataCtrlIndex < MaxSataControllerNum ());

  MmioOr32 (PchPwrmBase + R_PMC_PWRM_NST_PG_FDIS_1, B_PMC_PWRM_NST_PG_FDIS_1_SATA_FDIS_PMC);
}

/**
  This function checks if SATA device is supported (not disabled by fuse)

  @param[in] SataCtrlIndex SATA controller index

  @retval SATA support state
**/
BOOLEAN
PmcIsSataSupported (
  UINT32  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < MaxSataControllerNum ());

  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_FUSE_DIS_RD_2) & B_PMC_PWRM_FUSE_DIS_RD_2_SATA_FUSE_SS_DIS) == 0);
}

/**
  This function checks if GbE is function disabled
  by static power gating

  @retval GbE device state
**/
BOOLEAN
PmcIsGbeFunctionDisabled (
  VOID
  )
{
  return ((MmioRead32 ((UINTN) (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1)) & B_PMC_PWRM_ST_PG_FDIS_PMC_1_GBE_FDIS_PMC) != 0);
}

/**
  This function enables GBE ModPHY SPD gating.
**/
VOID
PmcGbeModPhyPowerGating (
  VOID
  )
{
}

/**
  This function disables GbE device by static power gating and enables ModPHY SPD gating (PCH-LP only).
  For static power gating to take place Global Reset, G3 or DeepSx transition must happen.
**/
VOID
PmcStaticDisableGbe (
  VOID
  )
{
  UINT32 PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  ASSERT (!PmcIsFunctionDisableConfigLocked ());

  MmioOr32 (PchPwrmBase + R_PMC_PWRM_ST_PG_FDIS_PMC_1, B_PMC_PWRM_ST_PG_FDIS_PMC_1_GBE_FDIS_PMC);
}

/**
  This function enables GbE device by disabling static power gating.
  Static power gating disabling takes place after Global Reset, G3 or DeepSx transition.
**/
VOID
PmcEnableGbe (
  VOID
  )
{
  //
  // Before modifying LAN Disable bit, make sure it's not locked.
  //
  ASSERT (!PmcIsFunctionDisableConfigLocked ());

  MmioAnd32 (PmcGetPwrmBase () + R_PMC_PWRM_ST_PG_FDIS_PMC_1, (UINT32) ~B_PMC_PWRM_ST_PG_FDIS_PMC_1_GBE_FDIS_PMC);
}

/**
  This function checks if GbE device is supported (not disabled by fuse)

  @retval GbE support state
**/
BOOLEAN
PmcIsGbeSupported (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_FUSE_DIS_RD_2) & B_PMC_PWRM_FUSE_DIS_RD_2_GBE_FUSE_SS_DIS) == 0);
}

/**
  This function disables (non-static power gating) PCIe Root Port and enables ModPHY SPD gating (PCH-LP only).

  @param[in] RpIndex        PCIe Root Port Index (0 based)
**/
VOID
PmcDisablePcieRootPort (
  IN UINT32  RpIndex
  )
{
  UINT32   NstPgFdis1Mask;
  UINT32   PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();
  NstPgFdis1Mask = B_PMC_PWRM_NST_PG_FDIS_1_PCIE_A0_FDIS_PMC << RpIndex;
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_NST_PG_FDIS_1, NstPgFdis1Mask);
}

/**
  This function disables (non-static power gating) xHCI and enables ModPHY SPD gating (PCH-LP only).
**/
VOID
PmcDisableXhci (
  VOID
  )
{
  UINT32   PchPwrmBase;
  PchPwrmBase = PmcGetPwrmBase ();

  MmioOr32 (PchPwrmBase + R_PMC_PWRM_NST_PG_FDIS_1, B_PMC_PWRM_NST_PG_FDIS_1_XHCI_FDIS_PMC);
}

/**
  This function disables (non-static power gating) XDCI and enables ModPHY SPD gating (PCH-LP only).
**/
VOID
PmcDisableXdci (
  VOID
  )
{
}

/**
  This function checks if XDCI device is supported (not disabled by fuse)

  @retval XDCI support state
**/
BOOLEAN
PmcIsXdciSupported (
  VOID
  )
{
  return FALSE;
}

/**
  This function disables (non-static power gating) HDA device
**/
VOID
PmcDisableHda (
  VOID
  )
{
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_NST_PG_FDIS_1, B_PMC_PWRM_NST_PG_FDIS_1_ADSP_FDIS_PMC);
}

/**
  Check if PMC Extended straps are supported

  @retval  Status of Extended Straps support
**/
BOOLEAN
PmcIsExtendedStrapsSupported (
  VOID
  )
{
  return FALSE;
}
