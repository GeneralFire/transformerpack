/** @file

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

#include <RcRegs.h>
#include <Library/DebugLib.h>
#include <Guid/MsmIpInterface.h>
#include <Library/UsraCsrLib.h>

/**
  Init Bar 0 for SPK:
  If the BAR0 has already been initialized, re-use the BAR0, so no need to reassign temp bus.
  If BAR0 not enabled, assign one bar to it, then after configure SNC register,

  @param SocketId                     - KtiInternalGlobal
  @param SocketId                     - Socket ID
  @param StatusCommandDefaultValuePtr - pointer to the power on default value of PMon PCICFG status and command register
  @param SpkBar0                      - value of SPK BAR0
  @param MmioBar0Initialized          - Indicate whether MMIOB BAR0 has been initialized or not

  @retval                             - EFI_UNSUPPORTED: the function not implemented
                                        EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                                        EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
SpkBarInit (
  IN UINT32                    TempBar,
  IN UINT8                     SocketId,
  OUT BOOLEAN                  *MmioBar0Initialized,
  OUT UINT32                   *StatusCommandDefaultValuePtr,
  OUT UINT32                   *SpkBar0
  )
{
  PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_STRUCT   DfdMsmDfxCfgCommand;
  PCI_CFG_BAR0_DFD_MSM_DFX_STRUCT             DfdMsmDfxBar;
  UINT32                                      Data32;

  DfdMsmDfxCfgCommand.Data = UsraCsrRead (SocketId, 0, PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_REG);
  *StatusCommandDefaultValuePtr = DfdMsmDfxCfgCommand.Data;
  DEBUG ((DEBUG_INFO, "SPK Bar0 initial configure status: 0x%8X \n", DfdMsmDfxCfgCommand.Data));

  DfdMsmDfxBar.Data = UsraCsrRead (SocketId, 0, PCI_CFG_BAR0_DFD_MSM_DFX_REG);
  DEBUG ((DEBUG_INFO, "SPK Bar0 initial BAR0: 0x%8X \n", DfdMsmDfxBar.Data));

  if ((DfdMsmDfxCfgCommand.Bits.memory_space_ena == 1) && (DfdMsmDfxBar.Bits.base_addr != 0)) {
    //
    // Mem enabled in command register and bar0 is not 0
    //
    *MmioBar0Initialized = TRUE;
    *SpkBar0 = (DfdMsmDfxBar.Data & 0xFFFFFFF0);
    DEBUG ((DEBUG_INFO, "SPK Bar0 BAR0 has been initialized! \n"));
    return EFI_SUCCESS;
  }

  DfdMsmDfxCfgCommand.Bits.memory_space_ena = 0;
  DfdMsmDfxCfgCommand.Bits.bus_master_ena = 0;
  UsraCsrWrite (SocketId, 0, PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_REG, DfdMsmDfxCfgCommand.Data);

  UsraCsrWrite (SocketId, 0, PCI_CFG_BAR0_DFD_MSM_DFX_REG, 0xFFFFFFFF);
  Data32 = UsraCsrRead (SocketId, 0, PCI_CFG_BAR0_DFD_MSM_DFX_REG);
  Data32 = Data32 & 0xFFFFFFF0;
  DEBUG ((DEBUG_INFO, "SPK Bar0 required size: 0x%8X \n", (~Data32) + 1));

  Data32 = TempBar;
  DfdMsmDfxBar.Data = Data32 & 0xFFFFFFF0;
  UsraCsrWrite (SocketId, 0, PCI_CFG_BAR0_DFD_MSM_DFX_REG, DfdMsmDfxBar.Data);

  DfdMsmDfxCfgCommand.Data = UsraCsrRead (SocketId, 0, PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_REG);
  DfdMsmDfxCfgCommand.Bits.memory_space_ena = 1;
  DfdMsmDfxCfgCommand.Bits.bus_master_ena = 1;
  UsraCsrWrite (SocketId, 0, PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_REG, DfdMsmDfxCfgCommand.Data);
  *SpkBar0 = DfdMsmDfxBar.Data;
  *MmioBar0Initialized = FALSE;
   DEBUG ((DEBUG_INFO, "SPK Bar0 BAR0 initialization done! \n"));

  return EFI_SUCCESS;
}

/**
  Clear PMon temp Bar 0 and restore the PMon PCICFG status and command register value

  @param SocketId                  - Socket ID
  @param StatusCommandDefaultValue - the power on default value of PMon PCICFG status and command register

  @retval                          - EFI_UNSUPPORTED: the function not implemented
                                     EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                                     EFI_SUCCESS: the function is excuted successfully
**/
EFI_STATUS
EFIAPI
ClearSpkBar (
  IN UINT8    SocketId,
  IN UINT32   StatusCommandDefaultValue
  )
{
  PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_STRUCT   DfdMsmDfxCfgCommand;
  PCI_CFG_BAR0_DFD_MSM_DFX_STRUCT             DfdMsmDfxBar;

  DfdMsmDfxCfgCommand.Data = UsraCsrRead (SocketId, 0, PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_REG);
  DfdMsmDfxCfgCommand.Bits.memory_space_ena = 0;
  DfdMsmDfxCfgCommand.Bits.bus_master_ena = 0;
  UsraCsrWrite (SocketId, 0, PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_REG, DfdMsmDfxCfgCommand.Data);

  DfdMsmDfxBar.Data = UsraCsrRead (SocketId, 0, PCI_CFG_BAR0_DFD_MSM_DFX_REG);
  DfdMsmDfxBar.Bits.base_addr = 0;
  UsraCsrWrite (SocketId, 0, PCI_CFG_BAR0_DFD_MSM_DFX_REG, DfdMsmDfxBar.Data);

  DfdMsmDfxCfgCommand.Data = StatusCommandDefaultValue;
  UsraCsrWrite (SocketId, 0, PCI_CFG_STATUS_COMMAND_DFD_MSM_DFX_REG, DfdMsmDfxCfgCommand.Data);

  return EFI_SUCCESS;
}