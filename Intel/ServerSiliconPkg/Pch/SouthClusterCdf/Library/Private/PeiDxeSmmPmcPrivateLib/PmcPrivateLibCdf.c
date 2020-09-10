/** @file
  PCH PMC Private Library implementation for IceLake PCH.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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
#include <Private/Library/PmcPrivateLib.h>
#include <Library/SataLib.h>
#include <Library/PchInfoLib.h>
#include <Register/PchRegsPmc.h>

#include "PmcPrivateLibInternal.h"
#include <Library/PciSegmentLib.h>

#include <Library/TimerLib.h>
#include <Library/SataSocLib.h>

/**
  This function returns PMC, PUNIT and CPU Crashlog Size allocated on PMC SSRAM
  specific for a given generation
  Not supported for CDF

  @param[out] PmcLogSize        Pointer to PMC CrashLog Size
  @param[out] PunitLogSize      Pointer to PUNIT CrashLog Size
  @param[out] CpuLogSize        Pointer to CPU CrashLog Size

**/
VOID
PmcSsramSocCrashLogSize (
  OUT UINT32     *PmcLogSize,
  OUT UINT32     *PunitLogSize,
  OUT UINT32     *CpuLogSize
  )
{
  return;
}

/**
  This function is part of PMC init and configures which clock wake signals should
  set the SLOW_RING, SA, FAST_RING_CF and SLOW_RING_CF indication sent up to the CPU/PCH
**/
VOID
PmcInitClockWakeEnable (
  VOID
  )
{
  UINT32                    PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

    ///
    /// PWRMBASE + 0x1880
    ///
    MmioWrite32 (PchPwrmBase + R_PMC_PWRM_EN_CW_SLOW_RING, V_PMC_PWRM_EN_CW_SLOW_RING);

    ///
    /// PWRMBASE + 0x1884
    ///
    MmioWrite32 (PchPwrmBase + R_PMC_PWRM_EN_CW_SLOW_RING2, V_PMC_PWRM_EN_CW_SLOW_RING2);

    ///
    /// PWRMBASE + 0x1888
    ///
    MmioWrite32 (PchPwrmBase + R_PMC_PWRM_EN_CW_SA, V_PMC_PWRM_EN_CW_SA);

    ///
    /// PWRMBASE + 0x188C
    ///
    MmioWrite32 (PchPwrmBase + R_PMC_PWRM_EN_CW_SA2, V_PMC_PWRM_EN_CW_SA2);

    ///
    /// PWRMBASE + 0x1898
    ///
    MmioWrite32 (PchPwrmBase + R_PMC_PWRM_EN_CW_SLOW_RING_CF, V_PMC_PWRM_EN_CW_SLOW_RING_CF);
}

/**
  This function configures PWRMBASE + 0x1E00 register
**/
VOID
PmcConfigureRegPwrm1E00 (
  VOID
  )
{
  ///
  /// PWRMBASE + 0x1E00[31,30] = 1,1
  /// PWRMBASE + 0x1E00[29] = 0
  /// PWRMBASE + 0x1E00[10:6] = 0
  /// PWRMBASE + 0x1E00[3:0] = 2
  ///
  MmioAndThenOr32 (
    PmcGetPwrmBase () + R_PMC_PWRM_1E00,
    (UINT32) ~(BIT29 | (0x1F << 6) | 0xF),
    BIT31 | BIT30 | 2
    );
}

/**
  This function configures Misc PM_SYNC events settings
**/
VOID
PmcConfigurePmSyncEventsSettings (
  VOID
  )
{
  ///
  /// PWRMBASE + 0x18C0 = 0x00000A20
  ///
  MmioWrite32 (PmcGetPwrmBase () + R_PMC_PWRM_EN_MISC_EVENT, 0x00000A20);
}

/**
  This function disables (non-static power gating) SCS eMMC controller and enables ModPHY SPD gating (PCH-LP only).
**/
VOID
PmcDisableScsEmmc (
  VOID
  )
{
  //
  // Set PWRMBASE + NST_PG_FDIS_1 to disable SCS Controller in PMC
  //
  MmioOr32 (PmcGetPwrmBase () + R_PMC_PWRM_NST_PG_FDIS_1, B_PCH_LP_PMC_PWRM_NST_PG_FDIS_1_EMMC_FDIS_PMC);
}

/**
  This function checks if SCS eMMC device is supported (not disabled by fuse)

  @retval SCS device support state
**/
BOOLEAN
PmcIsScsEmmcSupported (
  VOID
  )
{
  //
  // Get fuse info from PWRMBASE + FUSE_SS_DIS_RD_2
  //
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_FUSE_DIS_RD_2) & B_PMC_PWRM_FUSE_DIS_RD_2_EMMC_FUSE_SS_DIS) == 0);
}


/**
  This function checks if SCS SD Card device is supported

  @retval SCS device support state
**/
BOOLEAN
PmcIsScsSdCardSupported (
  VOID
  )
{
  return FALSE;
}


/**
  This function disables (non-static power gating) SCS SD Card controller and enables ModPHY SPD gating (PCH-LP only).
  Not applicable to CDF
**/
VOID
PmcDisableScsSdCard (
  VOID
  )
{
  ASSERT(FALSE);
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

  //
  // Set PWRMBASE + NST_PG_FDIS_1 [22] = 1b to disable SATA Controller in PMC
  //
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
  UINT64        PciSataRegBase;

  ASSERT (SataCtrlIndex < MaxSataControllerNum ());


  PciSataRegBase    = GetSataRegBase (SataCtrlIndex);
  return (PciSegmentRead32 (PciSataRegBase) != 0xFFFFFFFF);
}

/**
  Enable OS IDLE Mode
  Not supported for CDF
**/
VOID
PmcEnableOsIdleMode (
  VOID
)
{
  ASSERT(FALSE);
}


/**
  Lock OS IDLE Mode register
**/
VOID
PmcLockOsIdleMode (
  VOID
)
{
  ASSERT(FALSE);
}

/**
  Check if OS IDLE Mode is supported by PCH

  @retval OS IDLE Mode support state
**/
BOOLEAN
PmcIsOsIdleModeSupported (
  VOID
)
{
  return FALSE;
}

