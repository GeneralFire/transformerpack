/** @file
  This driver implements functions for updating the boot status for SPS Ignition.

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
#include <Base.h>
#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>

#include <Me/MeAccess.h>

#include "SpsIgnition.h"


/**
  This function clears all Ignition FW related fields of HECI-1 H_GS register.
**/
VOID
IgnitionClearIndicators (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "[%a] %08X", __FUNCTION__, HeciPciRead32 (R_ME_H_GS)));

  HeciPciAnd32 (R_ME_H_GS, ~B_IGN_INDICATORS);

  DEBUG ((DEBUG_INFO, " -> %08X\n", HeciPciRead32 (R_ME_H_GS)));
}

/**
  This function clears Ignition FW related fields of HECI-1 H_GS register which
  are supposed to be set in DXE.
**/
VOID
IgnitionClearDxeIndicators (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "[%a] %08X", __FUNCTION__, HeciPciRead32 (R_ME_H_GS)));

  HeciPciAnd32 (R_ME_H_GS, ~B_IGN_DXE_INDICATORS);

  DEBUG ((DEBUG_INFO, " -> %08X\n", HeciPciRead32 (R_ME_H_GS)));
}

/**
  This function sets END_OF_POST bit in Boot Status field of HECI-1 H_GS
  register.
**/
VOID
IgnitionIndicateEndOfPost (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "[%a] %08X", __FUNCTION__, HeciPciRead32 (R_ME_H_GS)));

  HeciPciOr32 (R_ME_H_GS, V_IGN_END_OF_POST);

  DEBUG ((DEBUG_INFO, " -> %08X\n", HeciPciRead32 (R_ME_H_GS)));
}

/**
  This function sets CORE_BIOS_DONE bit in Boot Status field of HECI-1 H_GS
  register.
**/
VOID
IgnitionIndicateCoreBiosDone (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "[%a] %08X", __FUNCTION__, HeciPciRead32 (R_ME_H_GS)));

  HeciPciOr32 (R_ME_H_GS, V_IGN_CORE_BIOS_DONE);

  DEBUG ((DEBUG_INFO, " -> %08X\n", HeciPciRead32 (R_ME_H_GS)));
}

/**
  This function sets DRAM_INIT_DONE bit in Boot Status field of HECI-1 H_GS
  register.
**/
VOID
IgnitionIndicateDramInitDone (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "[%a] %08X", __FUNCTION__, HeciPciRead32 (R_ME_H_GS)));

  HeciPciOr32 (R_ME_H_GS, V_IGN_DRAM_INIT_DONE);

  DEBUG ((DEBUG_INFO, " -> %08X\n", HeciPciRead32 (R_ME_H_GS)));
}

/**
  This function sets SSC Configuration field of HECI-1 H_GS register
  to SSC disable.
**/
VOID
IgnitionIndicateSscDisable (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "[%a] %08X", __FUNCTION__, HeciPciRead32 (R_ME_H_GS)));

  HeciPciAndThenOr32 (R_ME_H_GS, ~B_IGN_SSC_CONFIGURATION, V_IGN_SSC_DISABLE);

  DEBUG ((DEBUG_INFO, " -> %08X\n", HeciPciRead32 (R_ME_H_GS)));
}
