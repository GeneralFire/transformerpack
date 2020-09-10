/** @file
  The Proc SMB Chip Common file provides common functions for the PEI, DXE, and
  SMM phases of the implmentation of ProcSmbIpLib for Spr.

  This common file provides functions which are universal between PEI, DXE, and
  SMM.  The implementations for disabling/restoring TSOD and are common between
  all phases.  The read/write I3C routines are common between DXE and SMM.
  PEI has its own implementation of these functions.  The common code is
  implemented here to mitigate replicated code.

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

#include "Include/ProcSmbRegs.h"
#include <RcRegs.h>
#include <SysRegs.h>
#include <Ppi/MemoryPolicyPpi.h>
#include <Library/UsraCsrLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemMapDataLib.h>
#include <Cpu/CpuIds.h>
#include <UncoreCommonIncludes.h>
#include <Library/SiliconWorkaroundLib.h>

/**
  Set I3C bus operation mode

  @param[in] Socket     - Socket number
  @param[in] Instance   - I3C Instance number
  @param[in] BusMode    - I3C bus operation mode

  @rval EFI_SUCCESS     - Successfully set I3C bus operation mode
  @rval EFI_NOT_READY   - Cannot retrieve SPD Access data

**/
EFI_STATUS
EFIAPI
SetI3cBusModeCommon (
  IN  UINT8  Socket,
  IN  UINT8  Instance,
  IN  UINT8  BusMode
  )
{
  SYSTEM_MEMORY_MAP_HOB *SystemMemoryMap;

  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    return EFI_NOT_FOUND;
  }

  SystemMemoryMap->SmbMode[Socket][Instance] = BusMode;

  return EFI_SUCCESS;
}

/**
  Get slave device type - i2cni3c

  @param[in] Socket     - Socket number
  @param[in] Instance   - I3C Instance number

  @rval SlaveType   - slave device type

**/
UINT8
GetSlaveTypeCommon (
  IN UINT8   Socket,
  IN UINT8   Instance
  )
{
  SYSTEM_MEMORY_MAP_HOB *SystemMemoryMap;

  SystemMemoryMap = GetSystemMemoryMapData ();

  //
  // Treat the slave as I3C device only when the DramType is DDR5 and switched to I3C mode
  //
  if ((SystemMemoryMap != NULL) && (SystemMemoryMap->SmbMode[Socket][Instance] == I3C_MODE) && (SystemMemoryMap->DramType == SPD_TYPE_DDR5)) {
    return I3C_DEVICE;
  } else {
    return I2C_DEVICE;
  }
}

/**
  Get I3C mode and speed - mode_speed

  @param[in] Socket     - Socket number
  @param[in] Instance   - I3C Instance number

  @rval ModeSpeed   - I3C mode and speed

**/
I3C_BUS_MODE
GetModeSpeedCommon (
  IN UINT8  Socket,
  IN UINT8  Instance
  )
{
  SYSTEM_MEMORY_MAP_HOB *SystemMemoryMap;
  I3C_BUS_MODE          ModeSpeed;

  SystemMemoryMap = GetSystemMemoryMapData ();

  if (SystemMemoryMap == NULL) {
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    return I3cSdr0I2cFm;
  }

  //
  // I3C SDR3 and SDR4 are not supported as there is no corresponding I2C Speed.
  // I2C UDR  and UDR3 are reserved in I2C mode
  //
  switch (SystemMemoryMap->MemSetup.SpdSmbSpeed) {
  case SmbClk1M:   // 1 Mhz in I2C mode; 10Mhz in I3C mode
    ModeSpeed = I3cSdr0I2cFm;
    break;
  case SmbClk700K: // 700 Khz in I2C mode; 8Mhz in I3C mode
    ModeSpeed = I3cSdr1I2cFmPlus;
  case SmbClk400K: // 400 Khz in I2C mode; 6Mhz in I3C mode
  case SmbClk100K: // 100 Khz in I2C mode; 4Mhz in I3C mode
    ModeSpeed = I3cSdr2I2cUd1;
    break;
  default:
    ModeSpeed = I3cSdr0I2cFm; // Default
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Invalid SMBUS ClkFrequency Parameter: %x\n", SystemMemoryMap->MemSetup.SpdSmbSpeed);
    ASSERT_EFI_ERROR (EFI_UNSUPPORTED);
    break;
  }

  return ModeSpeed;
}

/**
  I3C has queues for transactions, and do not need to wait for Host not busy.
  But need Wait for comand queue ready based on target build.
  Command Queue Ready Status field reflects interrupt status
  that is populated upon number of entries in command queue
  is smaller or equal to threshold specified by
  CMD_EMPTY_BUF_THLD in QUEUE_THLD_CTRL
  register. This field is automatically cleared once number of
  entries is greater than the threshold value.

  @param[in] Socket       - Socket number
  @param[in] Instance     - I3C Instance number
  @param[in] I3cStatusReg - Status register to check

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Failure.
**/
EFI_STATUS
EFIAPI
WaitForHostNotBusyTargetCommon (
  IN UINT8   Socket,
  IN UINT8   Instance,
  IN UINT32  I3cStatusReg
  )
{
  UINT32                      Timeout;
  INTR_STATUS_SB_I3C0_STRUCT  I3cStatus;
  EFI_STATUS                  Status;

  Timeout = I3C_TIMEOUT;
  Status  = EFI_DEVICE_ERROR;

  //
  // Wait for there is space in the command queue
  //
  do {
    I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

    if (I3cStatus.Bits.cmd_queue_ready_stat == 1) {
      Status = EFI_SUCCESS;
      break;
    }

    if (GetEmulation () & SIMICS_FLAG) {
      Status = EFI_SUCCESS;
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

    // Wait for timeout
  } while (Timeout > 0);

  return Status;
}

/**
  Reads from I3C at a given device and offset.

  @param[in] Socket      - Processor socket ID
  @param[in] Dev         - I3C device object
  @param[in] ByteOffset  - Byte offset within device
  @param[out] Data       - Pointer to buffer to store read value

  @retval EFI_SUCCESS       Read completed successfully
  @retval EFI_UNSUPPORTED   Function not supported
  @retval EFI_DEVICE_ERROR  Error on read
**/
EFI_STATUS
SmbReadCommon (
  IN  UINT8              Socket,
  IN  SMB_DEVICE_STRUCT  Dev,
  IN  UINT8              ByteOffset,
  OUT volatile UINT8     *Data
  )
{
  EFI_STATUS                              Status;
  UINT32                                  I3cStatusReg;
  UINT32                                  I3cCmdReg;
  UINT32                                  I3cResetReg;
  UINT32                                  I3cQueueCtrlReg;
  UINT32                                  I3cDataReg;
  UINT32                                  I3cRespReg;
  UINT32                                  Timeout;
  RESPONSE_QUEUE_PORT_SB_I3C0_STRUCT      I3cResp;
  UINT8                                   TransactionID;
  UINT8                                   Instance;
  UINT8                                   I2cOrI3c;
  UINT8                                   SubOffsetLen;
  UINT32                                  SubOffset;
  COMMAND_QUEUE_PORT_SB_I3C0_STRUCT       CmdPort;
  COMBO_TRANSFER_COMMAND_LOW_WITHOUT_DAT  ComboCommandLow;
  COMBO_TRANSFER_COMMAND_HIGH_WITHOUT_DAT ComboCommandHigh;
  // Same address with RX_DATA_PORT and TX_DATA_PORT, single bi-direction design
  volatile DATA_PORT_SB_I3C0_STRUCT       DataPort;
  INTR_STATUS_SB_I3C0_STRUCT              I3cStatus;

  Status = EFI_SUCCESS;
  TransactionID = TidRead;
  Instance = (UINT8) Dev.address.SmbInstance;

  if (Instance == 0) {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C0_REG;
    I3cResetReg = RESET_CTRL_SB_I3C0_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C0_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C0_REG;
    I3cDataReg = DATA_PORT_SB_I3C0_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C0_REG;
  } else {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C1_REG;
    I3cResetReg = RESET_CTRL_SB_I3C1_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C1_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C1_REG;
    I3cDataReg = DATA_PORT_SB_I3C1_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C1_REG;
  }

  if (GetSlaveTypeCommon (Socket, Instance) == I3C_DEVICE) {
    I2cOrI3c = I3C_DEVICE;
  } else {
    I2cOrI3c = I2C_DEVICE;
  }

  // Use 2 Byte addressing only when accessing the SPR DDR5 SPD data in I3C mode
  if ((I2cOrI3c == I3C_DEVICE) && (Dev.address.deviceType == DTI_EEPROM)) {
    SubOffsetLen = SUBOFFSET_16_BIT;
    SubOffset = (Dev.SpdPage | (ByteOffset << 8));  // The low byte should follow the definition of SPD_DDR5_ADDRESS_SECOND_BYTE_STRUCT
  } else {
    SubOffsetLen = SUBOFFSET_8_BIT;
    SubOffset = ByteOffset;
  }

  //
  // Form read command
  //
  ComboCommandLow.Data = 0x0;
  ComboCommandLow.Bits.com_attr = I3C_COM_ATTR_WWR_COMBO_XFER;  // 0x3: WWR_COMBO_XFER: Write+Write/Read Combo Transfer;
  ComboCommandLow.Bits.tid = TransactionID; // Transaction ID field is used as identification tag for the command.
  ComboCommandLow.Bits.i2cni3c = I2cOrI3c; // 0x0: I3C device 0x1: I2C device
  ComboCommandLow.Bits.slave_address = (UINT32) ((Dev.address.deviceType << I3C_STRAP_ADDRESS_OFFSET) | Dev.address.strapAddress);
  //Indicates 8-or 16-bit sub-offset Values:0x0: 8_BIT_SUBOFFSET: 8 bit suboffset; 0x1: suboffset_16_bit:16 bit suboffset
  ComboCommandLow.Bits.suboffset_16_bit = SubOffsetLen;
  // 0x0: I3C SDR0/ I2C Fm: Note: I3C SDR0 represents standard SDR Speed (0 to fSCL Max)
  ComboCommandLow.Bits.mode_speed = GetModeSpeedCommon (Socket, Instance);
  ComboCommandLow.Bits.rnw = RNW_READ; // 0x1: READ: Read transfer
  ComboCommandLow.Bits.roc = ROC_REQUIRED; // 0x1: REQUIRED: Response Status is required
  ComboCommandLow.Bits.toc = TOC_STOP; // 0x1: STOP: Stop (P) is issued at end of the transfer

  ComboCommandHigh.Data = 0x0;
  ComboCommandHigh.Bits.offset_suboffset = SubOffset;

  if (Dev.compId == MTS) {
    ComboCommandHigh.Bits.data_length = 2;  // 2 byte
  } else {
    ComboCommandHigh.Bits.data_length = 1;  // 1 byte
  }

  CmdPort.Data = 0x0;
  CmdPort.Bits.command = ComboCommandLow.Data;

  //
  // Send command can check the rx_thld_stat to see if there is space in rx_fifo
  //
  // Low 32 bits
  Status = WaitForHostNotBusyTargetCommon (Socket, Instance, I3cStatusReg);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

  // High 32 bits
  // wait for there is space in the command queue
  CmdPort.Bits.command = ComboCommandHigh.Data;
  Status = WaitForHostNotBusyTargetCommon (Socket, Instance, I3cStatusReg);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

  //
  // Check Transfer Complete
  //
  Timeout = I3C_TIMEOUT;

  do {
    I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_TIMEOUT;
      break;
    }

  } while ((I3cStatus.Bits.resp_ready_stat == 0) && (I3cStatus.Bits.transfer_err_stat == 0));

  //
  // Wait for the read data
  //
  Timeout = I3C_TIMEOUT;

  do {
    I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_TIMEOUT;
      break;
    }

  } while ((I3cStatus.Bits.rx_thld_stat == 0) && (I3cStatus.Bits.transfer_err_stat == 0));

  //
  // If read data is valid
  //
  if ((I3cStatus.Bits.rx_thld_stat == 1) && (I3cStatus.Bits.transfer_err_stat == 0)) {
    DataPort.Data = UsraCsrRead (Socket, Instance, I3cDataReg);

    if (Dev.compId == MTS) {
      *Data = (UINT8) DataPort.Bits.data_port;  //lsb
      *(Data + 1) = (UINT8) (DataPort.Bits.data_port >> 8); //msb
    } else {
      *Data = (UINT8) DataPort.Bits.data_port;
    }

  } else {
    Status = EFI_DEVICE_ERROR;
  }

  //
  // Check response status
  //
  I3cResp.Data = UsraCsrRead (Socket, Instance, I3cRespReg);

  if ((I3cResp.Bits.tid != TransactionID) || I3cResp.Bits.err_status == 1) {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
} //SmbReadCommon

/**
  Performs I3C write operations at a given device and offset.

  @param[in] Socket      - Processor socket ID
  @param[in] Dev         - I3C device object
  @param[in] ByteOffset  - Byte offset within device
  @param[in] Data        - Pointer to data to write

  @retval EFI_SUCCESS       - Write completed successfully
  @retval EFI_UNSUPPORTED   - Function not supported
  @retval EFI_DEVICE_ERROR  - Write failure
**/
EFI_STATUS
SmbWriteCommon (
  IN UINT8              Socket,
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              ByteOffset,
  IN UINT8              *Data
  )
{
  EFI_STATUS                              Status = EFI_SUCCESS;
  UINT32                                  I3cStatusReg;
  UINT32                                  I3cCmdReg;
  UINT32                                  I3cResetReg;
  UINT32                                  I3cQueueCtrlReg;
  UINT32                                  I3cDataReg;
  UINT32                                  I3cRespReg;
  UINT32                                  Timeout;
  RESPONSE_QUEUE_PORT_SB_I3C0_STRUCT      I3cResp;
  UINT8                                   TransactionID;
  UINT8                                   Instance;
  UINT8                                   I2cOrI3c;
  COMMAND_QUEUE_PORT_SB_I3C0_STRUCT       CmdPort;
  REGULAR_DATA_TRANSFER_COMMAND_LOW_WITHOUT_DAT  RegularCommandLow;
  REGULAR_DATA_TRANSFER_COMMAND_HIGH_WITHOUT_DAT RegularCommandHigh;
  DATA_PORT_SB_I3C0_STRUCT                DataPort;
  INTR_STATUS_SB_I3C0_STRUCT              I3cStatus;

  TransactionID = TidWrite;
  Instance = (UINT8) Dev.address.SmbInstance;

  if (Instance == 0) {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C0_REG;
    I3cResetReg = RESET_CTRL_SB_I3C0_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C0_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C0_REG;
    I3cDataReg = DATA_PORT_SB_I3C0_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C0_REG;
  } else {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C1_REG;
    I3cResetReg = RESET_CTRL_SB_I3C1_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C1_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C1_REG;
    I3cDataReg = DATA_PORT_SB_I3C1_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C1_REG;
  }

  if (GetSlaveTypeCommon (Socket, Instance) == I3C_DEVICE) {
    I2cOrI3c = I3C_DEVICE;
  } else {
    I2cOrI3c = I2C_DEVICE;
  }

  //
  // Form write command
  //
  RegularCommandLow.Data = 0x0;
  RegularCommandLow.Bits.com_attr = I3C_COM_ATTR_XFER;  // I3C_COM_ATTR_XFER             0x0  // Regular Transfer
  RegularCommandLow.Bits.tid = TransactionID; // Transaction ID field is used as identification tag for the command.
  RegularCommandLow.Bits.i2cni3c = I2cOrI3c; // 0x0: I3C device 0x1: I2C device
  //RegularCommandLow.Bits.cmd // CMD field is not valid
  RegularCommandLow.Bits.cp = CP_TRANFSER; // 0x0: TRANFSER: Describes SDR transfer. CMD field is not valid.
  RegularCommandLow.Bits.slave_address = (UINT32) ((Dev.address.deviceType << I3C_STRAP_ADDRESS_OFFSET) | Dev.address.strapAddress);
  // 0x0: I3C SDR0/ I2C Fm: Note: I3C SDR0 represents standard SDR Speed (0 to fSCL Max)
  RegularCommandLow.Bits.mode_speed = GetModeSpeedCommon (Socket, Instance);
  RegularCommandLow.Bits.rnw = RNW_WRITE; // 0x0: WRITE: Write transfer
  RegularCommandLow.Bits.roc = ROC_REQUIRED; // 0x1: REQUIRED: Response Status is required
  RegularCommandLow.Bits.toc = TOC_STOP; // 0x1: STOP: Stop (P) is issued at end of the transfer

  RegularCommandHigh.Data = 0x0;

  CmdPort.Data = 0x0;
  CmdPort.Bits.command = RegularCommandLow.Data;

  //
  // data to write
  //
  DataPort.Data = 0;

  if (Dev.compId == MTS) {
    RegularCommandHigh.Bits.data_length = ONE_BYTE_ADDRESSING + 2;  // 1 Byte addressing + 2 bytes data
    DataPort.Bits.data_port = ((UINT32) ByteOffset) | ((UINT32) *Data << 8) | ((UINT32) *(Data + 1) << 16);
  } else {
    // Use 2 Byte addressing only when accessing the SPR DDR5 SPD data in I3C mode
    if ((I2cOrI3c == I3C_DEVICE) && (Dev.address.deviceType == DTI_EEPROM)) {
      RegularCommandHigh.Bits.data_length = TWO_BYTE_ADDRESSING + 1;  // 2 Byte addressing + 1 byte data
      DataPort.Bits.data_port = ((UINT32) ByteOffset) | ((UINT32) Dev.SpdPage << 8) | ((UINT32) *Data << 16);
    } else {
      RegularCommandHigh.Bits.data_length = ONE_BYTE_ADDRESSING + 1;  // 1 Byte addressing + 1 byte data
      DataPort.Bits.data_port = ((UINT32) ByteOffset) | ((UINT32) *Data << 8);
    }
  }

  //
  // check if there is space in the tx_fifo
  //
  Timeout = I3C_TIMEOUT;

  do {
    I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_TIMEOUT;
      break;
    }

  } while ((I3cStatus.Bits.tx_thld_stat == 0) && (I3cStatus.Bits.transfer_err_stat == 0));

  //
  // Write the Data
  //
  UsraCsrWrite (Socket, Instance, I3cDataReg, DataPort.Data);

  //
  // Send command
  //
  // Low 32 bits
  Status = WaitForHostNotBusyTargetCommon (Socket, Instance, I3cStatusReg);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

  // High 32 bits
  CmdPort.Bits.command = RegularCommandHigh.Data;
  Status = WaitForHostNotBusyTargetCommon (Socket, Instance, I3cStatusReg);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

  //
  // Check Transfer Complete
  //
  Timeout = I3C_TIMEOUT;

  do {
    I3cStatus.Data = UsraCsrRead (Socket, Instance, I3cStatusReg);

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_TIMEOUT;
      break;
    }

  } while ((I3cStatus.Bits.resp_ready_stat == 0) && (I3cStatus.Bits.transfer_err_stat == 0));

  // If I3C Status indicates transfer error
  if ((I3cStatus.Bits.resp_ready_stat == 0) || (I3cStatus.Bits.transfer_err_stat == 1)) {
    Status = EFI_DEVICE_ERROR;
  }

  //
  // Check response status
  //
  I3cResp.Data = UsraCsrRead (Socket, Instance, I3cRespReg);

  if ((I3cResp.Bits.tid != TransactionID) || I3cResp.Bits.err_status == 1) {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
} //SmbWriteCommon

/**
  Returns I3C Instance for given McId, by mapping Instance to
  McId

  @param[in] McId  - Memory Controller Id, 0 based

  @retval UINT8  Instance Number
**/
UINT8
GetProcSmbInstanceFromMcIdCommon (
  IN UINT8  McId
  )
{
  UINT8   MaxImc = GetMaxImc ();

  //Map McId to I3C Instance
  if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    return (McId / MaxImc);
  } else {
    return ((McId * 2) / MaxImc);
  }
}

/**
  Returns the I3C strap address of the DIMM slot.

  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number
  @param[in]  Dimm    - Dimm number
  @param[out] Dev     - Pointer to smbDevice

  @retval EFI_SUCCESS  Successfully retrieved I3C address data
**/
EFI_STATUS
GetSmbAddressCommon (
  IN UINT8               Socket,
  IN UINT8               Ch,
  IN UINT8               Dimm,
  OUT SMB_DEVICE_STRUCT  *Dev
  )
{
  UINT8                  NumChPerMc;
  UINT8                  MaxImc;
  NumChPerMc        =    GetNumChannelPerMc ();
  MaxImc            =    GetMaxImc ();
  //
  // SPD addressing for 1 IMC, 2 Ch per IMC
  // SPD seg 0, addr 0-1 to MC0, Ch0
  // SPD seg 0, addr 2-3 to MC0, Ch1
  //
  // SPD addressing for 2 IMC, 3 Ch per IMC
  // SPD seg 0, addr 0-1 to MC0, Ch0
  // SPD seg 0, addr 2-3 to MC0, Ch1
  // SPD seg 0, addr 4-5 to MC0, Ch2
  // SPD seg 1, addr 0-1 to MC1, Ch3
  // SPD seg 1, addr 2-3 to MC1, Ch4
  // SPD seg 1, addr 4-5 to MC1, Ch5
  //
  // SPD addressing for 4 IMC, 2 Ch per IMC
  // SPD seg 0, addr 0-1 to MC0, Ch0
  // SPD seg 0, addr 2-3 to MC0, Ch1
  // SPD seg 0, addr 4-5 to MC1, Ch2
  // SPD seg 0, addr 6-7 to MC1, Ch3
  // SPD seg 1, addr 0-1 to MC2, Ch4
  // SPD seg 1, addr 2-3 to MC2, Ch5
  // SPD seg 1, addr 4-5 to MC3, Ch6
  // SPD seg 1, addr 6-7 to MC3, Ch7
  //
  Dev->mcId = Ch / MAX_MC_CH;

  // For 10nm each socket has its own SMBUS controller
  Dev->address.SmbSocket = Socket;

  //Calculate I3C Instance from Memory Controller Id
  Dev->address.SmbInstance = GetProcSmbInstanceFromMcIdCommon (Dev->mcId);

  if (MaxImc == 1) {
    Dev->address.strapAddress = Dimm + ((Ch % (NumChPerMc * MaxImc)) * MAX_DIMM);
  } else {
    //
    // 2 and 4 IMC
    //
    Dev->address.strapAddress = Dimm + ((Ch % (NumChPerMc * MaxImc / 2)) * MAX_DIMM);
  }

  return EFI_SUCCESS;
}
