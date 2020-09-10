/** @file
  This file contains functions that initialize PCI Express clock sources in PCH.
  This applies to TGP project and beyond

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

#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PmcPrivateLib.h>
#include <Register/PchRegsClk.h>
#include <Register/PchPcrRegs.h>
#include <Library/PeiPchPcieClocksLib.h>

/**
  Checks if the IPC Command to manage PCIE CLKREQ Override is supported.

  @retval TRUE        It's supported
  @retval FALSE       It's not supported
**/
BOOLEAN
IsClkreqIpcCmdSupported (
  VOID
  )
{
  //
  // @todo: Confirmation required for ADL P and ADL M
  //
  if ((IsPchS () && IsAdlPch ()) || IsMtlSoc ()) {
    return FALSE;
  }
  return TRUE;
}

/**
  Disables one PCIe clock.

  @param[in] ClockUsage     Type and number of PCIe port for which Clock should be disabled
  @param[in] ClockNumber    The clock which needs to be disabled.
**/
VOID
DisableClockBufferProgramming (
  PCH_PCIE_CLOCK_USAGE ClockUsage,
  UINT32               ClockNumber
  )
{
  UINT32  PortClkreqModifyMask;

  if (ClockNumber >= GetPchMaxPcieClockNum ()) {
    ASSERT (FALSE);
    return;
  }
  if (((IsAdlPch ()) && (IsPchH ())) || ((IsTglPch ()) && (IsPchH ())) || IsMtlSoc ()) {
    if (ClockNumber < 7) {
      PchPcrAndThenOr32 (PID_ICLK, R_ICLK_PCR_BIOS_BUFFEN, ~(BIT0 << (ClockNumber + N_ICLK_PCR_BIOS_BUFFEN_SRC0)), 0);
    } else {
      PchPcrAndThenOr32 (PID_ICLK, R_ICLK_PCR_BIOS_BUFFEN_H, ~(BIT0 << (ClockNumber - 7 + N_ICLK_PCR_BIOS_BUFFEN_H_SRC7)), 0);
    }
  } else {
    if (ClockUsage <= PchClockUsageCpuPcie3) {
      PortClkreqModifyMask = BIT0 << (ClockUsage & 0xF);
      if (ClockUsage >= PchClockUsageCpuPcie0) {
        // CPU ports are shifted by 24 bits in the mask.
        PortClkreqModifyMask = (PortClkreqModifyMask << 24);
      }
    } else {
      PortClkreqModifyMask = 0;
    }
    PmcSetPcieClockEnableMask ((BIT0 << ClockNumber), 0, PortClkreqModifyMask, 0);
  }
}

