/** @file
  Implementation of the Bios to Pcode Mailbox library for 10nm wave 3.

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

#include <Library/MemoryCoreLib.h>
#include <RcRegs.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include "Chip10nm/Include/MemHostChip10nm.h"


STATIC CHAR8 PCU_MBOX_ERROR_MESSAGE[] = "PCU: API - Command->0x%02X, Error Code -> 0x%02X\n";


/**
  Set Memory Controller Frequency

  @param[in]   Socket         0 based Socket number
  @param[in]   RefClk         Reference Clock type
  @param[in]   QclkRatio      Qclk Ratio
  @param[in]   MemType        Memory Type (HBM, DDR4, DDR5)
  @param[in]   MemSsEnabled   Memory Subsystem enable mask
  @param[in]   RampBitEnable  Ramp Bit Enable, if its last call to set MC

  @retval EFI_SUCCESS     Function execution successful
  @retval EFI_UNSUPPORTED Frequency given is unsupported
  @retval !EFI_SUCCESS    Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetMCFreq (
  IN UINT8 Socket,
  IN UINT8 RefClk,
  IN UINT8 QclkRatio,
  IN PCU_MEM_TYPE MemType,
  IN UINT8 MemSsEnabled,
  IN BOOLEAN RampBitEnable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32    MailboxData = 0;
  UINT32    PcodeMailboxStatus = 0;

  if (MemType == PCU_MEM_HBM) {
    MailboxData |= MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MEMORY_TYPE_HBM << MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MEMORY_TYPE_LSB;
  }
  else if (MemType == PCU_MEM_DDR4) {
    MailboxData |= MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MEMORY_TYPE_DDR4 << MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MEMORY_TYPE_LSB;
  }
  else if (MemType == PCU_MEM_DDR5) {
    MailboxData |= MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MEMORY_TYPE_DDR5 << MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MEMORY_TYPE_LSB;
  }
  else if (MemType == PCU_MEM_NONE) {
    MailboxData |= MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MEMORY_TYPE_NONE << MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MEMORY_TYPE_LSB;
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  if (!EFI_ERROR(Status)) {
    MailboxData |= MemSsEnabled << (MAILBOX_BIOS_CMD_READ_MC_FREQ_DATA_MC_INSTANCE_LSB);
    //
    // Sent QCLK ratio.
    //
    MailboxData |= ((RefClk << MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MC_REF_CLK_TYPE_LSB) | QclkRatio);
    if (RampBitEnable) {
      MailboxData |= BIT31;
    }
    PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                           Socket,
                           MAILBOX_BIOS_CMD_ZERO,
                           MAILBOX_BIOS_CMD_SET_MC_FREQ,
                           &MailboxData,
                           0,
                           0
                           );
    if (PcodeMailboxStatus == MAILBOX_BIOS_CC_UNSUPPORTED_MC_FREQ) {
      RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_SET_MC_FREQ, PcodeMailboxStatus);
      Status = EFI_UNSUPPORTED;
    } else if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
      RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_SET_MC_FREQ, PcodeMailboxStatus);
      Status = EFI_DEVICE_ERROR;
    }
  }

  return Status;
}

/**
  Disable MC timeout W / A

  @param [in]   Socket  0 based Socket number

  @return      EFI_STATUS as reflected by ReadModifyWritePcuMailbox
**/
EFI_STATUS
EFIAPI
  PcuMcTimeoutWaDisable (
  IN  UINT8    Socket
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Send command to PUNIT when MC is operating in 2N mode.

  This function MUST only be called when 2N mode operation is required

  @param[in]    Socket      Current socket under test
  @param[in]    MemType     Memory Type (DDR4, HBM, DDR5)
  @param[in]    McBitMap    Memory Controller supported (bitwise)
  @param[in]    ChBitMap    Memory Channels supported (bitwise)

  @retval       EFI_DEVICE_ERROR  Command did not executed properly
  @retval       EFI_SUCCESS       Command was executed properly

**/
EFI_STATUS
EFIAPI
PcuEnable2nMode (
  IN        UINT8           Socket,
  IN        PCU_MEM_TYPE    MemType,
  IN        UINT8           McBitMap,
  IN        UINT8           ChBitMap
  )
{

  return EFI_UNSUPPORTED;
}

/**
  Set MS2IOSF HW_INIT for the given Mesh ID

  @param[in]    Socket      0 based Socket number
  @param[in]    MeshId      Mesh ID of the IIO stack to trigger HW_INIT

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetMs2iosfHwInit (
  IN UINT8 Socket,
  IN UINT8 MeshId
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Read MS2IOSF HW_INIT for the given Mesh ID

  @param[in]    Socket                    0 based Socket number
  @param[in]    MeshId                    Mesh ID of the IIO stack to trigger HW_INIT
  @param[OUT]   TcMiscUcrMailBoxData      Data from TC_MISC_UCR_SB_M2PCIE_REG

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuGetMs2iosfHwInit (
  IN UINT8 Socket,
  IN UINT8 MeshId,
  OUT UINT32 *TcMiscUcrMailBoxData
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Set PCIe DFX registers
  @param[in]    Socket                    Socket number
  @param[in]    CrIndex                   Index of register type. Currently, supported three PCIe DFX Gen 3 registers
                                          {pxpprtgen3eqpriv = 0x80, gen3privtxeq = 0x81, tx_ph3_cursor = 0x82}
  @param[in]    CrInst                    Ip Instance supported. Currently only supported value {0}, representing PCIe Gen 3 for SNR.
  @param[in]    CrPortIdx                 PCIe Port Index to access. Currently supported four values {0, 1, 2, 3} corresponding to
                                          PCIe port A, port B, port C, portD, respectively.
  @param[in]    CrProxyMailBoxData        Data to PCIe DFX register
  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetPcieDfx (
  IN UINT8   Socket,
  IN UINT8   CrIndex,
  IN UINT8   CrInst,
  IN UINT8   CrPortIdx,
  IN UINT32  CrProxyMailBoxData
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Read PCIe DFX registers
  @param[in]    Socket                    Socket number
  @param[in]    CrIndex                   Index of register type. Currently, supported three PCIe DFX Gen 3 registers
                                          {pxpprtgen3eqpriv = 0x80, gen3privtxeq = 0x81, tx_ph3_cursor = 0x82}
  @param[in]    CrInst                    Ip Instance supported. Currently only supported value {0}, representing PCIe Gen 3 for SNR.
  @param[in]    CrPortIdx                 PCIe Port Index to access. Currently supported four values {0, 1, 2, 3} corresponding to
                                          PCIe port A, port B, port C, portD, respectively.

  @param[OUT]   CrProxyMailBoxData        Data from PCIe DFX reg

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuGetPcieDfx (
  IN UINT8 Socket,
  IN UINT8 CrIndex,
  IN UINT8 CrInst,
  IN UINT8 CrPortIdx,
  OUT UINT32 *CrProxyMailBoxData
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Set the UPI (f.k.a. KTI, f.k.a. QPI) frequency for the next reset.

  @param[in]    Socket                 0 based Socket number
  @param[in]    UpiPortId              UPI Port ID
  @param[in]    UpiSpeedRatio          UPI Ratio (the UPI fast clock frequency in 100MHz units)

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetUpiFrequency (
  IN UINT8  Socket,
  IN UINT8  UpiPortId,
  IN UINT32 UpiSpeedRatio
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 PcuMailboxCommand;
  UINT32 PcuMailboxData;

  //
  // Set the Ratio encoding and mode first for the link
  // UpiSpeedRatio = 0: Slow mode; otherwise, swith to fast mode
  //
  if (UpiSpeedRatio != 0) {
    PcuMailboxCommand = MAILBOX_BIOS_CMD_SET_UPI_FREQ |
                        (UpiPortId << MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_LINK_INSTANCE_ID_LSB) |
                        (FAST_MODE_OF_SET_UPI_FREQ << MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_MODE_LSB);
  } else {
    PcuMailboxCommand = MAILBOX_BIOS_CMD_SET_UPI_FREQ |
                        (UpiPortId << MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_LINK_INSTANCE_ID_LSB) |
                        (SLOW_MODE_OF_SET_UPI_FREQ << MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_MODE_LSB);
  }
  PcuMailboxData = UpiSpeedRatio;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         PcuMailboxCommand,
                         &PcuMailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_SET_UPI_FREQ, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Set the trigger bit to trigger the GV FLow for UPI (f.k.a. KTI, f.k.a. QPI) frequency switch.
  Set once for each socket

  @param[in]    Socket                 0 based Socket number
  @param[in]    UpiPortId              UPI Port ID
  @param[in]    UpiSpeedRatio          UPI Ratio (the UPI fast clock frequency in 100MHz units)

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetTriggerBit (
  IN UINT8  Socket,
  IN UINT8  UpiPortId,
  IN UINT32 UpiSpeedRatio
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 PcuMailboxCommand;
  UINT32 PcuMailboxData;

  //
  // Set the trigger bit to trigger GV
  // UpiSpeedRatio = 0: Slow mode; otherwise, swith to fast mode
  //
  if (UpiSpeedRatio != 0) {
    PcuMailboxCommand = MAILBOX_BIOS_CMD_SET_UPI_FREQ |
                        (UpiPortId << MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_LINK_INSTANCE_ID_LSB) |
                        (FAST_MODE_OF_SET_UPI_FREQ << MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_MODE_LSB) |
                        (TRIGGER_BIT_OF_SET_UPI_FREQ << MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_TRIGGER_LSB);
  } else {
    PcuMailboxCommand = MAILBOX_BIOS_CMD_SET_UPI_FREQ |
                        (UpiPortId << MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_LINK_INSTANCE_ID_LSB) |
                        (SLOW_MODE_OF_SET_UPI_FREQ << MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_MODE_LSB) |
                        (TRIGGER_BIT_OF_SET_UPI_FREQ << MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_TRIGGER_LSB);
  }
  PcuMailboxData = UpiSpeedRatio;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         PcuMailboxCommand,
                         &PcuMailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_SET_UPI_FREQ, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Extract the link number from Mailbox Interface register

  @param[in]    Socket                 0 based Socket number
  @param[in]    Value                  Mailbox Interface register value
  @param[out]   UpiPort                UPI Port Number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuExtractLinkNum (
  IN UINT8  Socket,
  IN UINT32 Value,
  OUT UINT8 *UpiPort
  )
{
  *UpiPort = (Value >> MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_LINK_INSTANCE_ID_LSB) & MAILBOX_BIOS_CMD_SET_UPI_FREQ_INTERFACE_LINK_INSTANCE_ID_RANGE;

  return EFI_SUCCESS;
}

/**

  To write into the respective SMBus2 control registers given by the parameters:
    0: SMB2_CMD register
    1: SMB2_CMD_EXT register
    2: SMB2_DATA register

  Error codes:
  MAILBOX_BIOS_RETURN_CODE_NONE
  MAILBOX_BIOS_ERROR_CODE_INVALID_COMMAND

  Semaphore Arbitration
  An "access semaphore" is maintained by CPU to arbitrate between multiple competing requestors (PECI, BIOS, CPU itself).
  BIOS must request the semaphore (bit 17, see below) prior to performing any CR reads or writes.  When BIOS requests semaphore,
  CPU will return either of the following return codes:
    MAILBOX_BIOS_RETURN_CODE_NONE: semaphore is granted
    MAILBOX_BIOS_ERROR_CODE_INVALID_COMMAND: semaphore is not granted
  Once granted, BIOS will "own" the semaphore until explicitly released using bit 18.
  Semaphore request/release cannot be combined with a CR access; if bit 17 or 18 is set, the other bits will be ignored.
  If BIOS does not 'own' the semaphore, all CR access requests will return INVALID_COMMAND.
  CPU will filter addresses to prevent access to certain address ranges.  If an attempt to read or write one of those addresses,
  INVALID_COMMAND will be returned.

  @param[in]     Socket                 Zero based socket number.
  @param[in]     SmbusCommand           SMBUS command.
  @param[in]     SmbusCommandExt        SMBUS command extension.
  @param[in]     SmbusData              SMBUS data.

  @retval        EFI_SUCCESS            PCU executed command with no error.
  @retval        EFI_DEVICE_ERROR       PCU failed to execute command.
  @retval        EFI_INVALID_PARAMETER  Parameter error.

**/
EFI_STATUS
EFIAPI
PcuSmbAccessWrite (
  IN  UINT8   Socket,
  IN  UINT32  SmbusCommand,
  IN  UINT32  SmbusCommandExt,
  IN  UINT16  SmbusData
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Set PI5 IP Disable for the given Stack ID

  @param[in]    Socket      0 based Socket number
  @param[in]    StackId     IIO stack to trigger PI5 IP Disable
  @param[in]    DisConf     PI5 IP Disable config

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_DEVICE_ERROR  Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetPi5IpDisable (
  IN UINT8  Socket,
  IN UINT8  StackId,
  IN UINT32 DisConf
  )
{
  UINT32     PcodeMailboxCommand;
  UINT32     PcodeMailboxStatus = 0;
  UINT32     PcodeMailboxData = 0;
  EFI_STATUS Status = EFI_SUCCESS;

  PcodeMailboxCommand = MAILBOX_BIOS_CMD_PI5_D2_BRINGUP;
  PcodeMailboxCommand |= (StackId << MAILBOX_BIOS_CMD_PI5_D2_BRINGUP_INTERFACE_STACK_ID_LSB) &
                        MAILBOX_BIOS_CMD_PI5_D2_BRINGUP_INTERFACE_STACK_ID_MASK;
  PcodeMailboxCommand |= (DisConf << MAILBOX_BIOS_CMD_PI5_D2_BRINGUP_INTERFACE_IP_DISABLE_CONFIG_LSB) &
                        MAILBOX_BIOS_CMD_PI5_D2_BRINGUP_INTERFACE_IP_DISABLE_CONFIG_MASK;
  PcodeMailboxCommand |= MAILBOX_BIOS_CMD_PI5_D2_BRINGUP_INTERFACE_RAMP_MASK;

  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         PcodeMailboxCommand,
                         &PcodeMailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_PI5_D2_BRINGUP, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Inform pCode if the result of Protocol negotiation during Link Training is CXL for the given Stack ID.

  @param[in]    Socket        0 based Socket number
  @param[in]    StackId       IIO stack to trigger PI5 IP Disable
  @param[in]    ConfComplete  Config Complete.

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_DEVICE_ERROR  Error on command execution
**/
EFI_STATUS
EFIAPI
PcuNotifyCxlInst (
  IN UINT8    Socket,
  IN UINT8    StackId,
  IN BOOLEAN  ConfComplete
  )
{
  UINT32     PcodeMailboxCommand;
  UINT32     PcodeMailboxStatus = 0;
  UINT32     PcodeMailboxData = 0;
  EFI_STATUS Status = EFI_SUCCESS;

  PcodeMailboxCommand = 0xC9;
  PcodeMailboxCommand |= (StackId << 8) & 0xF00;
  PcodeMailboxCommand |= (1 << 12) & 0xF000;
  if (ConfComplete) {
    PcodeMailboxCommand |= (1 << 28);
  }

  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         PcodeMailboxCommand,
                         &PcodeMailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, 0xC9, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Program Minimum OPS value for DRAM PBM\RAPL

  @param[in]    Socket                  0 based Socket number
  @param[in]    MinOpsValue             Minimum operation value

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramMinOpsValue (
  IN UINT8 Socket,
  IN UINT32 MinOpsValue
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Report voltage regulator on Dimm to PCU

  @param[in]    Socket                 0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuReportVrod (
  IN UINT8 Socket
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 PcuData = 0;

  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                          Socket,
                          MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG,
                          MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG,
                          &PcuData,
                          ~(UINT32)MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_VROD_MASK,
                          (1 << MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_VROD_LSB)
                          );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Inform pCode the fADR config and core/mesh ratio.

  @param[in]  Socket        0 based Socket number
  @param[in]  fAdrEn        fADR status
  @param[in]  CoreRatio     Core Ratio value
  @param[in]  MeshRatio     Mesh Ratio Value

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetEadrRatio (
  IN UINT8    Socket,
  IN BOOLEAN  fAdrEn,
  IN UINT8    CoreRatio,
  IN UINT8    MeshRatio
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailBoxStatus;
  UINT32 PcuMailboxData;

  PcuMailboxData = (MeshRatio << MAILBOX_BIOS_CMD_ENHANCED_ADR_DATA_MESH_RATIO_LSB)|CoreRatio;
  PcuMailboxData |= (fAdrEn << MAILBOX_BIOS_CMD_ENHANCED_ADR_DATA_FADR_EN_LSB);
  PcodeMailBoxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MAILBOX_BIOS_CMD_ENHANCED_ADR,
                         &PcuMailboxData,
                         0,
                         0
                         );
  if (PcodeMailBoxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_ENHANCED_ADR, PcodeMailBoxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  PCU writes the CR_PROXY supported register.

  This function is a general workaround for writing registers implemented in wrong
  SAI policy group which prevents direct BIOS access. Pcode will write the register
  on behalf of BIOS.

  @param[in] ScktId               Socket index.
  @param[in] InstId               CR instance index on socket.
  @param[in] ChId                 CR channel index on CR instance.
  @param[in] RegId                Register index in the CR whitelist.
  @param[in] PcuMailboxData       Data to be written through PCU mailbox.

  @retval EFI_SUCCESS             Function execution successful.
  @retval !EFI_SUCCESS            Error on command execution.

**/
EFI_STATUS
EFIAPI
PcuWriteCrProxyReg (
  IN UINT8      ScktId,
  IN UINT8      InstId,
  IN UINT8      ChId,
  IN UINT8      RegId,
  IN UINT32     PcuMailboxData
  )
{
  UINT32        PcuMailboxCommand;
  UINT32        PcuMailboxStatus;

  PcuMailboxCommand = MAILBOX_BIOS_CR_WRITE_CMD (InstId, ChId, RegId);
  PcuMailboxStatus = ReadModifyWritePcuMailbox (ScktId, MAILBOX_BIOS_CMD_ZERO, PcuMailboxCommand, &PcuMailboxData, 0, 0);

  if (PcuMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_CR_PROXY_WRITE, PcuMailboxStatus);
    return EFI_ACCESS_DENIED;
  }

  return EFI_SUCCESS;
}

/**
  PCU reads the CR_PROXY supported register.

  This function is a general workaround for reading registers implemented in wrong
  SAI policy group which prevents direct BIOS access. Pcode will read the register
  on behalf of BIOS.

  @param[in]  ScktId              Socket index.
  @param[in]  InstId              CR instance index on socket.
  @param[in]  ChId                CR channel index on CR instance.
  @param[in]  RegId               Register index in the CR whitelist.
  @param[out] RegValue            Register value retured by PCU mailbox.

  @retval EFI_SUCCESS             Function execution successful.
  @retval !EFI_SUCCESS            Error on command execution.

**/
EFI_STATUS
EFIAPI
PcuReadCrProxyReg (
  IN  UINT8      ScktId,
  IN  UINT8      InstId,
  IN  UINT8      ChId,
  IN  UINT8      RegId,
  OUT UINT32     *RegValue
  )
{
  UINT32        PcuMailboxCommand;
  UINT32        PcuMailboxStatus;

  PcuMailboxCommand = MAILBOX_BIOS_CR_READ_CMD (InstId, ChId, RegId);
  PcuMailboxStatus = ReadModifyWritePcuMailbox (ScktId, PcuMailboxCommand, MAILBOX_BIOS_CMD_ZERO, RegValue, 0, 0);

  if (PcuMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_CR_PROXY_WRITE, PcuMailboxStatus);
    return EFI_ACCESS_DENIED;
  }

  return EFI_SUCCESS;
}