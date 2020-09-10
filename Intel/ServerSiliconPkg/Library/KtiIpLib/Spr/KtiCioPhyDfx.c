/** @file
  KTI *KTI_CIOPHYDFX_REG related registers access special for SPR.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

//
// Include files
//
#include <Uefi.h>
#include <Base.h>
#include "Include/KtiRegs.h"
#include <Library/UsraCsrLib.h>
#include <Guid/KtiIpInterface.h>
#include <Library/UpimdfIpAccessLib.h>
#include <Library/KtiApi.h>

/**
  Function definition from ClearOrSetKtiDfxLckForFullSpeedTransition ()

  @param Socket - Socket ID
  @param Port   - Port ID
  @param Value  - 1 (set) or 0 (clear) the KTI DFX LCK bits

  @retval None
**/
VOID
EFIAPI
SetKtiDfxLck (
  UINT8                 Socket,
  UINT8                 Port,
  UINT16                Value
  )
{
  if (IsUpiPortUpimdf (Socket, Port)) {
    //
    // SPR-UCC: UPIMDF does not have this register.
    //
    return;
  }

  DFX_LCK_CTL_KTI_CIOPHYDFX_STRUCT     DfxLckCtlCfg;
  DfxLckCtlCfg.Data = UpimdfIpReadAccess32 (Socket, Port, 0, DFX_LCK_CTL_KTI_CIOPHYDFX_REG);
  DfxLckCtlCfg.Bits.reutenglck = Value;
  DfxLckCtlCfg.Bits.privdbg_phy_l0_sync_lck = Value;
  UpimdfIpWriteAccess32 (Socket, Port, UPIMDF_INSTANCE_BROADCAST, DFX_LCK_CTL_KTI_CIOPHYDFX_REG, DfxLckCtlCfg.Data);
}

/**
  Check to see if LBC lbc_start bit is clear
  The lbc_start bit must be clear before reading/writing the KTI LBC register

  @param Socket    - CPU Socket Node number (Socket ID)
  @param Port      - KTI Port number on socket

  @retval TRUE  - lbc_start bit is clear
  @retval FALSE - lbc_start bit is set
**/
BOOLEAN
EFIAPI
IsLbcStartClear (
  UINT8                 Socket,
  UINT8                 Port,
  UINT8                 LbcType
  )
{
  LBC_PER_IOU_CONTROL_KTI_CIOPHYDFX_STRUCT LbcControl;

  //
  // Make sure LBC_PER_IOU_CONTROL.lbc_start == 0x0
  //
  LbcControl.Data = 0;
  LbcControl.Bits.lbc_start = 1;
  LbcControl.Data = UpimdfIpReadAccess32 (Socket, Port, 0, LBC_PER_IOU_CONTROL_KTI_CIOPHYDFX_REG);
  if (LbcControl.Bits.lbc_start == 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Write to LBC register.

  Note, depending on the value of the parameter LbcType, this function
  will write into LBC_PER_IOU_CONTROL (AFE type) or into KTIMISC_LBC_CONTROL
  (COM type). This function assumes that the bitfield structure of both
  registers is the same and just varies the write destination based on the
  LbcType.

  @param Socket    - CPU Socket Node number (Socket ID)
  @param Port      - KTI Port Index, 0 based
  @param LbcType   - LBC_AFE or LBC_COM - Note: unused in 10nm
  @param LaneMask  - 20-bit wide lane mask
  @param LoadSel   - 7-bit wide load select (address within LBC)
  @param Data      - 14-bit data value to write

**/
VOID
EFIAPI
WriteLbcData (
  UINT8                 Socket,
  UINT8                 Port,
  UINT8                 LbcType,
  UINT32                LaneMask,
  UINT32                LoadSel,
  UINT32                Data
  )
{
  LBC_PER_IOU_DATA_KTI_CIOPHYDFX_STRUCT    LbcData;
  LBC_PER_IOU_CONTROL_KTI_CIOPHYDFX_STRUCT LbcControl;

  //
  // Fill LBC_PER_IOU_DATA.lbc_data and write
  //
  LbcData.Data = 0;
  LbcData.Bits.lbc_data = Data & 0x3fff;
  UpimdfIpWriteAccess32 (Socket, Port, UPIMDF_INSTANCE_BROADCAST, LBC_PER_IOU_DATA_KTI_CIOPHYDFX_REG, LbcData.Data);

  //
  // Fill LBC_PER_IOU_CONTROL.lbc_start = 1, lbc_req = 2, lbc_ld_sel, lbc_ln_sel and write
  //
  LbcControl.Bits.lbc_start = 1;
  LbcControl.Bits.lbc_req = 2;
  LbcControl.Bits.lbc_ld_sel = LoadSel & 0x7f;
  UpimdfIpWriteAccess32 (Socket, Port, UPIMDF_INSTANCE_BROADCAST, LBC_PER_IOU_CONTROL_KTI_CIOPHYDFX_REG, LbcControl.Data);
}

/**
Write safe value into the Rx PI Duty cycle corrector CSR
Null function, the RX DCC CSRs are not available in SPR

@param SocId     - CPU Socket Node number (Socket ID)
@param BoxInst   - Box Instance, 0 based

@retval None
**/
VOID
EFIAPI
OverrideRxDccExtVal (
  UINT8 Socket,
  UINT8 Port
  )
{
  //
  // Null function, the RX DCC CSRs are not available in SPR
  //
  return;
}

/**
Apply the RxDCC Clock override for this Socket/Port
Null function, the RX DCC CSRs are not available in SPR

@param SocId     - CPU Socket Node number (Socket ID)
@param BoxInst   - Box Instance, 0 based

@retval None
**/
VOID
EFIAPI
ApplyRxDccClockOverride (
  UINT8 Socket,
  UINT8 Port
  )
{
  //
  // Null function, the RX DCC CSRs are not available in SPR
  //
  return;
}