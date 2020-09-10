/** @file
  PCIE root port library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2019 Intel Corporation.

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
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchPcieRpLib.h>
#include <PcieRegs.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsClk.h>
#include <PchPcieRpInfo.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/PchPcrRegs.h>
#include <Library/PchPciBdfLib.h>

#include "PchPcieRpLibInternal.h"

/**
  Gets pci segment base address of PCIe root port.

  @param RpIndex    Root Port Index (0 based)
  @return PCIe port base address.
**/
UINT64
PchPcieBase (
  IN  UINT32   RpIndex
  )
{
  return PchPcieRpPciCfgBase (RpIndex);
}

/**
  Checks which CLK_REQ signal is assigned to given CLK_SRC

  @param ClkSrc    number of CLK_SRC, 0-based
  @retval          number of CLK_REQ, 0-based
**/
UINT8
GetClkReqForClkSrc (
  IN UINT8 ClkSrc
  )
{
  UINT32 ClkMuxRegister;
  UINT32 ClkReq;
  UINT32 Offset;

  ASSERT (ClkSrc < GetPchMaxPcieClockNum ());
  if (!(IsAdlPch () && IsPchS ())){
    if (ClkSrc < S_ICLK_PCR_MUXSEL_FIELDS_PER_REG) {
      ClkMuxRegister = PchPcrRead32 (PID_ICLK, R_ICLK_PCR_CMU_ONE_DWORD25);
      ClkReq = (ClkMuxRegister >> (ClkSrc * S_ICLK_PCR_MUXSEL_BITS_PER_FIELD)) & B_ICLK_PCR_MUXSEL_BITS_PER_FIELD;
    } else {
      ClkMuxRegister = PchPcrRead32 (PID_ICLK, R_ICLK_PCR_CMU_ONE_DWORD26);
      ClkReq = (ClkMuxRegister >> ((ClkSrc - S_ICLK_PCR_MUXSEL_FIELDS_PER_REG) * S_ICLK_PCR_MUXSEL_BITS_PER_FIELD)) & B_ICLK_PCR_MUXSEL_BITS_PER_FIELD;
    }
    DEBUG ((DEBUG_INFO, "GetClkReqForClkSrc %d->%d, register %08x\n", ClkSrc, ClkReq, ClkMuxRegister));
    return (UINT8)ClkReq;
  } else {
    Offset = 0;
    switch (ClkSrc) {
      case 0:
      case 1:
      case 2:
      case 3:
        Offset = R_ICLK_PCR_SRC_MUXSEL;
        break;
      case 4:
      case 5:
      case 6:
      case 7:
        Offset = R_ICLK_PCR_SRC_MUXSEL_26;
        break;
      case 8:
      case 9:
      case 10:
      case 11:
        Offset = R_ICLK_PCR_SRC_MUXSEL_34;
        break;
      case 12:
      case 13:
      case 14:
      case 15:
        Offset = R_ICLK_PCR_SRC_MUXSEL_42;
        break;
      case 16:
      case 17:
        Offset = R_ICLK_PCR_SRC_MUXSEL_50;
        break;
      }
   ClkMuxRegister = PchPcrRead32 (PID_ICLK, Offset);
   ClkReq = (ClkMuxRegister >> ((ClkSrc % S_ICLK_PCR_SRC_MUXSEL_FIELDS_PER_REG) * 8)) & B_ICLK_PCR_SRC_MUXSEL_BITS_PER_FIELD;
   DEBUG ((DEBUG_INFO, "GetClkReqForClkSrc %d->%d, register %08x\n", ClkSrc, ClkReq, ClkMuxRegister));
   return (UINT8)ClkReq;
  }
}

/**
  Determines whether L0s is supported on current stepping.

  @return TRUE if L0s is supported, FALSE otherwise
**/
BOOLEAN
PchIsPcieL0sSupported (
  VOID
  )
{
  return TRUE;
}

/**
  Some early PCH steppings require Native ASPM to be disabled due to hardware issues:
   - RxL0s exit causes recovery
   - Disabling PCIe L0s capability disables L1
  Use this function to determine affected steppings.

  @return TRUE if Native ASPM is supported, FALSE otherwise
**/
BOOLEAN
PchIsPcieNativeAspmSupported (
  VOID
  )
{
  return PchIsPcieL0sSupported ();
}