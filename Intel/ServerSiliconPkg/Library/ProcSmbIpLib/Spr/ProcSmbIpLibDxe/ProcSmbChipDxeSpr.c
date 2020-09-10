/** @file
  The DXE implementation of the ProcSmbIpLib for Spr.

  This implementation provides processor I3C services for use during DXE.
  Most functions are not needed during DXE phase, so function stubs are
  used.  Reset and Read/Write functions for Spr are implemented for use.
  Read/Write functions call into the common file (shared with SMM) for the
  low-level interactions.

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

#include <SysRegs.h>
#include <Ppi/MemoryPolicyPpi.h>
#include <Include/ProcSmbRegs.h>
#include <UncoreCommonIncludes.h>
#include <ProcSmbChipCommonSpr.h>
#include <Library/TimerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/DebugLib.h>
#include <Library/SemaphoreLib.h>
#include <Library/RcDebugLib.h>

/**
  Initialize I3C controller for given socket

  @param[in] Socket  - processor socket ID

  @retval EFI_SUCCESS      I3C initalized successfully
  @retval EFI_UNSUPPORTED  Function not supported
**/
EFI_STATUS
EFIAPI
InitProcSmb (
  IN UINT8  Socket
  )
{
  return EFI_UNSUPPORTED;
} //InitProcSmb

/**
  Reset I3C controller with given Socket and Instance

  @param[in] Socket      - processor socket ID
  @param[in] SmbInstance - I3C Instance number

  @retval EFI_SUCCESS      I3C reset successfully
  @retval EFI_UNSUPPORTED  Function not supported
**/
EFI_STATUS
EFIAPI
ResetProcSmb (
  IN UINT8   Socket,
  IN UINT16  SmbInstance
  )
{
  EFI_STATUS                           Status;
  UINT8                                Instance;
  UINT32                               Timeout;
  UINT32                               I3cComDevCtrlReg;
  UINT32                               I3cDevCtrlReg;
  UINT32                               I3cResetReg;
  UINT32                               I3cQueueCtrlReg;
  UINT32                               I3cStatusEnReg;
  UINT32                               I3cDataCtrlReg;
  COMMON_DEVICE_CTRL_SB_I3C0_STRUCT    I3cComDevCtrl;
  DEVICE_CONTROL_SB_I3C0_STRUCT        I3cDevCtrl;
  DATA_BUFFER_THLD_CTRL_SB_I3C0_STRUCT I3cDataThld;
  QUEUE_THLD_CTRL_SB_I3C0_STRUCT       I3cQueueThld;
  RESET_CTRL_SB_I3C0_STRUCT            ResetCtrl;

  Status = EFI_SUCCESS;
  Instance = (UINT8) SmbInstance;

  if (Instance == 0) {
    I3cComDevCtrlReg  = COMMON_DEVICE_CTRL_SB_I3C0_REG;
    I3cDevCtrlReg     = DEVICE_CONTROL_SB_I3C0_REG;
    I3cResetReg       = RESET_CTRL_SB_I3C0_REG;
    I3cQueueCtrlReg   = QUEUE_THLD_CTRL_SB_I3C0_REG;
    I3cStatusEnReg    = INTR_STATUS_ENABLE_SB_I3C0_REG;
    I3cDataCtrlReg    = DATA_BUFFER_THLD_CTRL_SB_I3C0_REG;
  } else {
    I3cComDevCtrlReg  = COMMON_DEVICE_CTRL_SB_I3C1_REG;
    I3cDevCtrlReg     = DEVICE_CONTROL_SB_I3C1_REG;
    I3cResetReg       = RESET_CTRL_SB_I3C1_REG;
    I3cQueueCtrlReg   = QUEUE_THLD_CTRL_SB_I3C1_REG;
    I3cStatusEnReg    = INTR_STATUS_ENABLE_SB_I3C1_REG;
    I3cDataCtrlReg    = DATA_BUFFER_THLD_CTRL_SB_I3C1_REG;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Reset I3C controller Instance %d\n", Instance);

  //
  //Disable I3C
  //
  I3cDevCtrl.Data = UsraCsrRead (Socket, Instance, I3cDevCtrlReg);
  I3cDevCtrl.Bits.enable = I3C_DISABLE;
  UsraCsrWrite (Socket, Instance, I3cDevCtrlReg, I3cDevCtrl.Data);

  //reset the sts and fifo
  ResetCtrl.Data = UsraCsrRead (Socket, Instance, I3cResetReg);
  ResetCtrl.Bits.soft_rst = I3C_ENABLE;
  ResetCtrl.Bits.cmd_queue_rst = I3C_ENABLE;
  ResetCtrl.Bits.resp_queue_rst = I3C_ENABLE;
  ResetCtrl.Bits.tx_fifo_rst = I3C_ENABLE;
  ResetCtrl.Bits.rx_fifo_rst = I3C_ENABLE;
  ResetCtrl.Bits.ibi_queue_rst = I3C_ENABLE;
  UsraCsrWrite (Socket, Instance, I3cResetReg, ResetCtrl.Data);

  //
  // Wait for reset completion.
  //
  Timeout = I3C_TIMEOUT;

  do {
    ResetCtrl.Data = UsraCsrRead (Socket, Instance, I3cResetReg);

    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_TIMEOUT;
      break;
    }

  } while (ResetCtrl.Bits.soft_rst == 1 || ResetCtrl.Bits.cmd_queue_rst == 1);

  I3cQueueThld.Data = UsraCsrRead (Socket, Instance, I3cQueueCtrlReg);
  //set resp buf thld to 0  [15:8]
  I3cQueueThld.Bits.resp_buf_thld = 0;
  //set cmd empty thld buf to 1  [7:0]
  I3cQueueThld.Bits.cmd_empty_buf_thld = 1;
  UsraCsrWrite (Socket, Instance, I3cQueueCtrlReg, I3cQueueThld.Data);

  I3cDataThld.Data = UsraCsrRead (Socket, Instance, I3cDataCtrlReg);
  //
  // set resp_buf_thld as 0 to identify every transaction is ready for reading/writing
  // set RX/TX Start/buf thld as n to support n+1 byte Read/Write
  // The supported values for RX_START_THLD/TX_START_THLD/RX_BUF_THLD/TX_BUF_THLD are:
  //   000 - 1;  001 - 4;  010 - 8;   011 - 16
  //   100 - 32; 101 - 64; 110 - 128; 111 - 256
  //
  //set RX Start thld to 0  [26:24]
  I3cDataThld.Bits.rx_start_thld = 0;
  //set RX buf thld to 0  [10:8]
  I3cDataThld.Bits.rx_buf_thld = 0;
  //set TX Start THLD to 0  [18:16]
  I3cDataThld.Bits.tx_start_thld = 0;
  //set tX buf thld to 0  [2:0]
  I3cDataThld.Bits.tx_buf_thld = 0;
  UsraCsrWrite (Socket, Instance, I3cDataCtrlReg, I3cDataThld.Data);

  //Enable I3C
  I3cDevCtrl.Bits.enable = I3C_ENABLE;
  UsraCsrWrite (Socket, Instance, I3cDevCtrlReg, I3cDevCtrl.Data);

  //
  // Enable all the states for I3cStatusEnReg
  //
  UsraCsrWrite (Socket, Instance, I3cStatusEnReg, I3C_INTR_STATUS_ENABLE_ALL);

  //
  // Set Device NACK Retry Count
  //
  I3cComDevCtrl.Data = UsraCsrRead (Socket, Instance, I3cComDevCtrlReg);
  I3cComDevCtrl.Bits.dev_nack_retry_cnt = MAX_DEV_NACK_RETRY_CNT;
  UsraCsrWrite (Socket, Instance, I3cComDevCtrlReg, I3cComDevCtrl.Data);

  return Status;
} //ResetProcSmb

/**
  Adjust the I3C frequency to the specified value.

  @param[in] Socket       - Processor socket ID
  @param[in] SmbInstance  - SMBUS Instance number
  @param[in] ClkFrequency - Clock Frequency to be programmed

  @retval EFI_SUCCESS           - Clock Period programmed successfully
  @retval EFI_INVALID_PARAMETER - Invalid Socket
  @retval EFI_UNSUPPORTED       - Function not supported
**/
EFI_STATUS
EFIAPI
AdjustProcSmbFreq (
  IN UINT8                Socket,
  IN UINT8                SmbInstance,
  IN SMB_CLOCK_FREQUENCY  ClkFrequency
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Read the processor I3C device at a given device and offset

  @param[in] Dev         - I3C Device address
  @param[in] ByteOffset  - byte offset within Device
  @param[out] Data       - pointer to store value

  @retval EFI_SUCCESS       Read was successful
  @retval EFI_DEVICE_ERROR  Read failed
**/
EFI_STATUS
EFIAPI
ReadProcSmb (
  IN  SMB_DEVICE_STRUCT  Dev,
  IN  UINT8              ByteOffset,
  OUT volatile UINT8     *Data
  )
{
  EFI_STATUS  Status;
  BOOLEAN     SemaphoreAcquired;
  UINT32      QueueNumber;
  UINT8       SmbSocket;

  SmbSocket = (UINT8) Dev.address.SmbSocket;
  // Acquire semaphore to read I3C
  SemaphoreAcquired = AcquireSocketSemaphore (SmbSocket, SystemSemaphore0, ADD_TO_QUEUE, &QueueNumber);

  if (!SemaphoreAcquired) {
    while (!IsSocketSemaphoreOwned (SmbSocket, SystemSemaphore0, QueueNumber));
  }

  Status = SmbReadCommon (SmbSocket, Dev, ByteOffset, Data);

  if (EFI_ERROR (Status)) {
    ResetProcSmb (SmbSocket, Dev.address.SmbInstance);
    Status = SmbReadCommon (SmbSocket, Dev, ByteOffset, Data);
  }

  ReleaseSocketSemaphore (SmbSocket, SystemSemaphore0);

  return Status;
}

/**
  Write the processor I3C dev/offset with specified data

  @param[in] Dev         - I3C device address
  @param[in] ByteOffset  - byte offset within device
  @param[in] Data        - pointer to data to write

  @retval EFI_SUCCESS      - Write was successful
  @retval EFI_DEVICE_ERROR - Write failed
**/
EFI_STATUS
EFIAPI
WriteProcSmb (
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              ByteOffset,
  IN UINT8              *Data
  )
{
  EFI_STATUS  Status;
  BOOLEAN     SemaphoreAcquired;
  UINT32      QueueNumber;
  UINT8       SmbSocket;

  SmbSocket = (UINT8) Dev.address.SmbSocket;

  SemaphoreAcquired = AcquireSocketSemaphore (SmbSocket, SystemSemaphore0, ADD_TO_QUEUE, &QueueNumber);

  if (!SemaphoreAcquired) {
    while (!IsSocketSemaphoreOwned (SmbSocket, SystemSemaphore0, QueueNumber));
  }

  Status = SmbWriteCommon (SmbSocket, Dev, ByteOffset, Data);

  if (EFI_ERROR (Status)) {
    ResetProcSmb (SmbSocket, Dev.address.SmbInstance);
    Status = SmbWriteCommon (SmbSocket, Dev, ByteOffset, Data);
  }

  // Release Semaphore
  ReleaseSocketSemaphore (SmbSocket, SystemSemaphore0);

  return Status;
}

/**
  Get slave device type - i2cni3c

  @param[in] Socket     - Socket number
  @param[in] Instance   - I3C Instance number

  @rval SlaveType   - slave device type

**/
UINT8
GetSlaveType (
  IN UINT8  Socket,
  IN UINT8  Instance
  )
{
  return GetSlaveTypeCommon (Socket, Instance);
}

/**
  Returns the I3C strap address of the DIMM slot

  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number
  @param[in]  Dimm    - Dimm number
  @param[out] Dev     - Pointer to smbDevice

  @retval EFI_SUCCESS  Successfully retrieved I3C address data
**/
EFI_STATUS
EFIAPI
GetSmbAddress (
  IN UINT8               Socket,
  IN UINT8               Ch,
  IN UINT8               Dimm,
  OUT SMB_DEVICE_STRUCT  *Dev
  )
{
  EFI_STATUS  Status;

  Status = GetSmbAddressCommon (Socket, Ch, Dimm, Dev);

  return Status;
}

/**
  Calculates the Channel and Dimm number based on the I3C address information.

  @param[in]  SpdDev - ptr to smbDevice which contains Instance and strap address data
  @param[out] Chan   - Channel number based on I3C address
  @param[out] Dimm   - Dimm number based on I3C address

  @retval EFI_SUCCESS        Successfully calculated channel and dimm number
  @retval EFI_UNSUPPORTED    Function not supported
**/
EFI_STATUS
EFIAPI
GetChDimmFromSmbAddress (
  IN SMB_DEVICE_STRUCT  *Dev,
  OUT UINT8             *Chan,
  OUT UINT8             *Dimm
  )
{
  // Not needed for DXE
  return EFI_UNSUPPORTED;
}

/**
  This routine gets the Tsod Polling enable bit value

  @param[in] Socket   - Socket Id
  @param[in] SmbInstance  - SMBus Instance number

  @retval TsodPollEn  value of Tsod poll enable bit
**/
UINT32
GetTsodPollEnSmb (
  IN UINT8   Socket,
  IN UINT16  SmbInstance
  )
{
  // THIS NEEDS CHANGED
  return 0;
}

/**
  This routine sets periodic_poll_cmd_en to the given value

  @param[in] Socket        - Socket Id
  @param[in] SmbInstance   - SMBus Instance number
  @param[in] TsodPollEn    - TsodPollEn value to set

  @retval  VOID
**/
VOID
SetTsodPollEnSmb (
  IN UINT8   Socket,
  IN UINT8   SmbInstance,
  IN UINT32  TsodPollEn
  )
{
  return;
}

/**
  This routine sets smb_tsod_poll_rate to the given value

  @param[in] Socket        - Socket Id
  @param[in] SmbInstance   - SMBus Instance number
  @param[in] TsodPollRate  - TsodPollRate value to set

  @retval VOID
**/
VOID
SetTsodPollRateSmb (
  IN UINT8   Socket,
  IN UINT8   SmbInstance,
  IN UINT32  TsodPollRate
  )
{
  return;
}

/**
  This routine updates the Tsod population based on DIMM types

  @param[in] Socket   Socket ID
  @param[in] McId     Memory controller ID

  @retval  VOID
**/
VOID
UpdateTsodPopulation (
  IN UINT8  Socket,
  IN UINT8  McId
  )
{
  return;
}

/**
  This routine compares RegOffset to SMB_CMD registers

  @param[in] RegOffset  - Register address

  @retval 1  for match
  @retval 0  for no match
**/
UINT32
IsRegCmdSmb (
  IN UINT32  RegOffset
  )
{
  return 0;
}

/**
  This routine checks if the offset belongs to SMB

  @param[in] RegOffset  - Register address

  @retval 1  for pass
  @retval 0  for no pass
**/
UINT32
IsRegFilterPassSmb (
  IN UINT32  RegOffset
  )
{
  return 0;
}

/**
  This routine gets processor I3C data register

  @param[in] Socket       - Socket Id
  @param[in] SmbInstance  - Processor I3C Instance
  @param[in] RegSize      - Pointer to size of register

  @retval  register address
**/
UINT32
GetProcSmbDataReg (
  IN UINT8   Socket,
  IN UINT16  SmbInstance,
  IN UINT8   *RegSize
  )
{
  return 0;
}

/**
  This routine sets tsod I3C slave address for reading

  @param[in] Socket       - Socket Id
  @param[in] SmbInstance  - Processor I3C Instance
  @param[in] TsodSa       - Tsod Slave address

  @retval  VOID
**/
VOID
DoProcSmbTsodRead (
  IN UINT8   SktId,
  IN UINT8   SmbInstance,
  IN UINT32  TsodSa
  )
{
  return;
}

/**
  This routine gets I3C status register

  @param[in] SmbInstance  - Processor I3C Instance

  @retval register address
**/
UINT32
GetProcSmbStatReg (
  IN UINT8  SmbInstance
  )
{
  return 0;
}

/**
  This routine gets smbus command register

  @param[in] SmbInstance  - Processor Smbus Instance

  @retval register address
**/
UINT32
GetProcSmbCmdReg (
  IN UINT8  SmbInstance
  )
{
  return 0;
}

/**
  This routine gets smbus error bit

  @param[in] SktId    - Socket number
  @param[in] SmbInstance  - Processor Smbus Instance

  @retval  Smbus error bit value
**/
UINT32
GetProcSmbSbe (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  return 0;
}

/**
  This routine gets smbus clock override bit

  @param[in] SktId        - Socket number
  @param[in] SmbInstance  - Processor Smbus Instance

  @retval Smbus clock override bit value
**/
UINT32
GetProcSmbCkovrd (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  return 0;
}

/**
  This routine sets smbus soft reset bit

  @param[in] SktId        - Socket number
  @param[in] SmbInstance  - Processor Smbus Instance
  @param[in] Softrst      - Smbus soft reset bit value

  @retval  VOID
**/
VOID
SetProcSmbSoftrst (
  IN UINT8   SktId,
  IN UINT8   SmbInstance,
  IN UINT32  Softrst
  )
{
  return;
}

/**
  This routine gets information on TSOD slave address, Tsod polling bit
  and Smbus recover enable bit info

  @param[in] SktId        - Socket number
  @param[in] SmbInstance  - Processor Smbus Instance
  @param[out] TsodSa      - Tsod Slave Address
  @param[out] TsodPollEn  - Tsode Polling enable bit info
  @param[out] SbeEn       - SMBus error recovery enable bit

  @retval  VOID
**/
VOID
GetProcSmbTsodSaTsodPollEnSbeEn (
  IN UINT8    SktId,
  IN UINT8    SmbInstance,
  OUT UINT32  *TsodSa,
  OUT UINT32  *TsodPollEn,
  OUT UINT32  *SbeEn
  )
{
  return;
}

/**
  This routine sets information on Tsod polling bit
  and Smbus recover enable bit info

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Processor Smbus Instance
  @param[in] TsodPollEn  - Tsode Polling enable bit info
  @param[in] SbeEn       - SMBus error recovery enable bit

  @retval  VOID
**/
VOID
SetProcSmbTsodPollEnSbeEn (
  IN UINT8   SktId,
  IN UINT8   SmbInstance,
  IN UINT32  TsodPollEn,
  IN UINT32  SbeEn
  )
{
  return;
}

/**
  This routine get smi enable on SBE bit.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Processor Smbus Instance

  @retval Smbus SmiEn bit value
**/
UINT32
GetProcSmbSmiEn (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  return 0;
}

/**
  This routine sets smi enable on SBE bit. Also sets\resets SBE (smbus error recovery)
  enable bit.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Processor Smbus Instance
  @param[in] SmiEn       - Smi enable on SBE error
  @param[in] SbeEn       - SBE Enable

  @retval  VOID
**/
VOID
SetProcSmbSmiEnSbeEn (
  IN UINT8   SktId,
  IN UINT8   SmbInstance,
  IN UINT32  SmiEn,
  IN UINT32  SbeEn
  )
{
  return;
}

/**
  Prepares the Smbus Recovery Operation by disabling TSOD polling
  and Smbus recovery enable.

  @param[in] SktId       - Socket Id
  @param[in] SmbInstance - Instance

  @retval  VOID
**/
VOID
DisableTsodAndSmbSbeEn (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  return;
}

/**
  Triggers the Smbus Recovery operation.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Instance

  @retval VOID
**/
VOID
TriggerSmbRecovery (
  IN UINT8   SktId,
  IN UINT8   SmbInstance
  )
{
  return;
}

/**
  Performs the Smbus Recovery operation.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Instance

  @retval VOID
**/
VOID
DoProcSmbRecovery (
  IN UINT8   SktId,
  IN UINT8   SmbInstance
  )
{
  return;
}

/**
  Returns Smbus Instance for given McId, by mapping Instance to
  McId

  @param[in] McId  - Memory Controller Id, 0 based

  @retval UINT8  Instance Number
**/
UINT8
EFIAPI
GetProcSmbInstanceFromMcId (
  IN UINT8  McId
  )
{
  return GetProcSmbInstanceFromMcIdCommon (McId);
}

/**
  I3cSwitchToI3cMode.

  @param[in] Socket        - Socket Id

  @rval EFI_SUCCESS  Successfully Send CCC commands
**/
EFI_STATUS
EFIAPI
I3cSwitchToI3cMode (
  IN  UINT8    Socket
  )
{
  return EFI_UNSUPPORTED; // PEI only
}

/**
  I3C Send CCC Commands.

  Directed CCC Write/Read Transfer
  Data Flow Representation of Directed CCC Transfer with IC_HAS_DAT=0
  1. Enqueue the Immediate/Regular Transfer command with the required field settings.
  2. The DWC_mipi_i3c controller will generate the Directed CCC transfer on the line to the device with the
   address of SLV_ADDR field and maintains the timing based on the MODE field in the command and issues
   data payload (if present) retrieved from the command.
  3. The DWC_mipi_i3c controller terminates the Directed CCC transfer either when BYTE_CNT/DATA_LENGTH
   expires or upon receiving NACK for Broadcast address or for dynamic address.
  4. The DWC_mipi_i3c Controller generates the response for the enqueued Directed CCC transfer command in the
   Response Queue with the Error status (if any) and the Remaining Data Length of the transfer if the write transfer
   terminated before the BYTE_CNT/DATA_LENGTH expires or the received data length in case of read transfer.

  Broadcast CCC Write Transfer
  Data Flow Representation of Broadcast CCC Transfer with IC_HAS_DAT=0
  1. Enqueue the Immediate Transfer/Regular Transfer command with the required field.
  2. The DWC_mipi_i3c controller will generate the Broadcast CCC transfer on the line and issues data payload (if
     present) retrieved from the command.
  3. The DWC_mipi_i3c controller terminates the Broadcast CCC transfer either on BYTE_CNT/DATA_LENGTH
     expires or upon receiving NACK for Broadcast address.
  4. The DWC_mipi_i3c Controller generates the response of the enqueued Broadcast CCC transfer command in the Response
     Queue with the Error status (if any) and the Remaining Data Length of the transfer if the transfer terminated before the
     BYTE_CNT/DATA_LENGTH expires.

  @param[in]  Socket      - Socket Id
  @param[in]  Dev         - Device address on I3C
  @param[in]  CccCommand  - CCC Command
  @param[in]  Attribute   - Attribute for Read/Write CCC/None-CCC
  @param[out] Data        - Pointer to Data to store the value
  @param[out] DataLength  - Pointer to the length, in bytes, of Data

  @retval EFI_SUCCESS           - Successfully Send CCC commands
  @retval EFI_DEVICE_ERROR      - Send CCC failed
  @retval EFI_UNSUPPORTED       - Function not supported
  @retval EFI_INVALID_PARAMETER - Invalid Parameter
**/
EFI_STATUS
EFIAPI
SmbSendCccCommandWorker (
  IN     UINT8             Socket,
  IN     SMB_DEVICE_STRUCT Dev,
  IN     UINT8             CccCommand,
  IN     SMB_ATTR_FLAG     Attribute,
  IN OUT UINT8             *Data,
  IN OUT UINT8             *DataLength
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
  UINT8                                   Div4Quotient;
  UINT8                                   Div4Remainder;
  UINT8                                   QuoIndex;
  UINT8                                   RemIndex;
  UINT8                                   RemLen;
  UINT8                                   TransactionID;
  UINT8                                   Instance;
  UINT8                                   ReadOrWrite;
  UINT8                                   CmdPresent;
  UINT8                                   Toc;
  UINT8                                   Roc;
  COMMAND_QUEUE_PORT_SB_I3C0_STRUCT       CmdPort;
  REGULAR_DATA_TRANSFER_COMMAND_LOW_WITHOUT_DAT  RegularCommandLow;
  REGULAR_DATA_TRANSFER_COMMAND_HIGH_WITHOUT_DAT RegularCommandHigh;
  // Same address with RX_DATA_PORT and TX_DATA_PORT, single bi-direction design
  volatile DATA_PORT_SB_I3C0_STRUCT       DataPort;
  INTR_STATUS_SB_I3C0_STRUCT              I3cStatus;
  UINT32                                  I3cDataCtrlReg;
  DATA_BUFFER_THLD_CTRL_SB_I3C0_STRUCT    I3cDataThld;
  UINT32                                  I3cDevCtrlReg;
  DEVICE_CONTROL_SB_I3C0_STRUCT           I3cDevCtrl;

  Status = EFI_SUCCESS;
  TransactionID = TidCcc;
  Instance = (UINT8) Dev.address.SmbInstance;

  if (Instance == 0) {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C0_REG;
    I3cResetReg = RESET_CTRL_SB_I3C0_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C0_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C0_REG;
    I3cDataReg = DATA_PORT_SB_I3C0_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C0_REG;
    I3cDataCtrlReg = DATA_BUFFER_THLD_CTRL_SB_I3C0_REG;
    I3cDevCtrlReg = DEVICE_CONTROL_SB_I3C0_REG;
  } else {
    I3cCmdReg = COMMAND_QUEUE_PORT_SB_I3C1_REG;
    I3cResetReg = RESET_CTRL_SB_I3C1_REG;
    I3cQueueCtrlReg = QUEUE_THLD_CTRL_SB_I3C1_REG;
    I3cStatusReg = INTR_STATUS_SB_I3C1_REG;
    I3cDataReg = DATA_PORT_SB_I3C1_REG;  // Use DATA_PORT for reading and writing
    I3cRespReg = RESPONSE_QUEUE_PORT_SB_I3C1_REG;
    I3cDataCtrlReg = DATA_BUFFER_THLD_CTRL_SB_I3C1_REG;
    I3cDevCtrlReg = DEVICE_CONTROL_SB_I3C1_REG;
  }

  //
  // determine the Read no Write field identifies direction of the transfer
  //
  switch (Attribute) {
  case AttrCccRead:
    ReadOrWrite = RNW_READ;
    CmdPresent = CP_CCC_HDR;
    Roc = ROC_REQUIRED;
    Toc = TOC_STOP;
    break;

  case AttrCccWrite:
    ReadOrWrite = RNW_WRITE;
    CmdPresent = CP_CCC_HDR;
    Roc = ROC_REQUIRED;
    Toc = TOC_STOP;
    break;

  case AttrNormalRead:
    ReadOrWrite = RNW_READ;
    CmdPresent = CP_TRANFSER;
    Roc = ROC_REQUIRED;
    Toc = TOC_STOP;
    break;

  case AttrNormalWrite:
    ReadOrWrite = RNW_WRITE;
    CmdPresent = CP_TRANFSER;
    Roc = ROC_REQUIRED;
    Toc = TOC_STOP;
    break;

  case AttrNormalWriteReStartRead:
    ReadOrWrite = RNW_WRITE;
    CmdPresent = CP_TRANFSER;
    Roc = ROC_NOT_REQUIRED;
    Toc = TOC_RESTART;
    break;

  default:
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "ERROR: I3C unsupported Attribute %d\n", Attribute);
    return EFI_INVALID_PARAMETER;
    break;
  }

  //
  // Set tx_start_thld = 2 and iba_include = 0 before RCD read
  // do not send write command until whole write payload is written to tx_data_port
  //
  if ((Attribute == AttrNormalWriteReStartRead) || (Attribute == AttrNormalWrite)) {
    I3cDataThld.Data = UsraCsrRead (Socket, Instance, I3cDataCtrlReg);
    //set TX Start THLD to 2  [18:16] 010 - 8
    I3cDataThld.Bits.tx_start_thld = 2;
    UsraCsrWrite (Socket, Instance, I3cDataCtrlReg, I3cDataThld.Data);

    I3cDevCtrl.Data = UsraCsrRead (Socket, Instance, I3cDevCtrlReg);
    I3cDevCtrl.Bits.iba_include = 0;
    UsraCsrWrite (Socket, Instance, I3cDevCtrlReg, I3cDevCtrl.Data);
  }

  //
  // Form read/write command
  //
  RegularCommandLow.Data = 0x0;
  RegularCommandLow.Bits.com_attr = I3C_COM_ATTR_XFER;  // I3C_COM_ATTR_XFER  0x0 Regular Transfer
  RegularCommandLow.Bits.tid = TransactionID; // Transaction ID field is used as identification tag for the command.
  RegularCommandLow.Bits.i2cni3c = GetSlaveType (Socket, Instance); // 0x0: I3C device 0x1: I2C device
  if (CmdPresent == CP_CCC_HDR) {
    RegularCommandLow.Bits.cmd = CccCommand;
  }
  RegularCommandLow.Bits.cp = CmdPresent;

  // Broadcast CCC do not care slave_address field
  if (CccCommand >= MAX_CCC_CODE_BROADCAST) {  // Dirct CCC
    RegularCommandLow.Bits.slave_address = (UINT32) ((Dev.address.deviceType << 3) | Dev.address.strapAddress);
  }

  // 0x0: I3C SDR0/ I2C Fm: Note: I3C SDR0 represents standard SDR Speed (0 to fSCL Max)
  RegularCommandLow.Bits.mode_speed = GetModeSpeedCommon (Socket, Instance);  // All CCC transfers will be initiated with SDR0 Speed
  RegularCommandLow.Bits.rnw = ReadOrWrite; // 0x1: READ: Read transfer
  RegularCommandLow.Bits.roc = Roc; // Response Status is required/not required
  RegularCommandLow.Bits.toc = Toc; // Repeated Start (Sr) / STOP

  RegularCommandHigh.Data = 0x0;
  RegularCommandHigh.Bits.data_length = *DataLength;

  CmdPort.Data = 0x0;
  CmdPort.Bits.command = RegularCommandLow.Data;

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
  // wait for there is space in the command queue
  CmdPort.Bits.command = RegularCommandHigh.Data;
  Status = WaitForHostNotBusyTargetCommon (Socket, Instance, I3cStatusReg);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);

  if (Attribute == AttrNormalWriteReStartRead) {

    RegularCommandLow.Bits.roc = ROC_REQUIRED;
    RegularCommandLow.Bits.toc = TOC_STOP;
    RegularCommandLow.Bits.rnw = RNW_READ;

    CmdPort.Data = 0x0;
    CmdPort.Bits.command = RegularCommandLow.Data;

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
    // wait for there is space in the command queue
    CmdPort.Bits.command = RegularCommandHigh.Data;
    Status = WaitForHostNotBusyTargetCommon (Socket, Instance, I3cStatusReg);

    if (EFI_ERROR (Status)) {
      return Status;
    }

    UsraCsrWrite (Socket, Instance, I3cCmdReg, CmdPort.Bits.command);
  }

  //
  // data to write.
  // if com_attr is Immediate transfer command, populate data in Immediate command; else push data to transmit fifo.
  // program the data port when Data Length is not 0 and (BroadCast CCC or Direct CCC with Read attribute)
  //
  if ((*DataLength > 0) && (ReadOrWrite == RNW_WRITE)) {

    // There are 4 bytes per one Data Port
    Div4Quotient = (*DataLength / DATA_LEN_I3C_PORT);
    Div4Remainder = (*DataLength % DATA_LEN_I3C_PORT);
    if (Div4Remainder != 0) {
      Div4Quotient += 1;
    }

    for (QuoIndex = 0; QuoIndex < Div4Quotient; QuoIndex++) {

      // All 4 bytes are valid if it is not the last Data Port to read
      if ((Div4Remainder != 0) && (QuoIndex == (Div4Quotient - 1))) {
        RemLen = Div4Remainder;
      } else {
        RemLen = DATA_LEN_I3C_PORT;
      }

      DataPort.Data = 0;
      for (RemIndex = 0; RemIndex < RemLen; RemIndex++) {
        DataPort.Bits.data_port |= ((UINT32) *(Data + (DATA_LEN_I3C_PORT * QuoIndex + RemIndex)) << (8 * RemIndex));
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
    }
  }

  if (Attribute == AttrNormalWriteReStartRead) {
    // Write then Read
    ReadOrWrite = RNW_READ;
  }

  //
  // Transfer Complete or not INTR_STATUS[4]
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

  // No Read BroadCast CCC
  if ((*DataLength > 0) && (ReadOrWrite == RNW_READ)) {

    //
    // For Read, Wait for the read data
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

      Div4Quotient = (*DataLength / DATA_LEN_I3C_PORT);
      Div4Remainder = (*DataLength % DATA_LEN_I3C_PORT);
      if (Div4Remainder != 0) {
        Div4Quotient += 1;
      }

      for (QuoIndex = 0; QuoIndex < Div4Quotient; QuoIndex++) {
        if ((Div4Remainder != 0) && (QuoIndex == (Div4Quotient - 1))) {
          RemLen = Div4Remainder;
        } else {
          RemLen = DATA_LEN_I3C_PORT;
        }

        DataPort.Data = UsraCsrRead (Socket, Instance, I3cDataReg);

        for (RemIndex = 0; RemIndex < RemLen; RemIndex++) {
          *(Data + (DATA_LEN_I3C_PORT * QuoIndex + RemIndex)) = (UINT8) (DataPort.Bits.data_port >> (8 * RemIndex));
        }
      }

    } else {
      Status = EFI_DEVICE_ERROR;
    }
  }

  //
  // Check response status
  //
  Timeout = I3C_TIMEOUT;

  do {
    I3cResp.Data = UsraCsrRead (Socket, Instance, I3cRespReg);

    if (GetEmulation () & SIMICS_FLAG) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_TIMEOUT;
      break;
    }
  } while ((I3cResp.Bits.tid != TransactionID) || I3cResp.Bits.err_status);

  //
  // Restore tx_start_thld = 0 and iba_include = 1 after RCD read
  //
  if ((Attribute == AttrNormalWriteReStartRead) || (Attribute == AttrNormalWrite)) {
    //set TX Start THLD to 0 [18:16] 000 - 1
    I3cDataThld.Bits.tx_start_thld = 0;
    UsraCsrWrite (Socket, Instance, I3cDataCtrlReg, I3cDataThld.Data);

    I3cDevCtrl.Bits.iba_include = 1;
    UsraCsrWrite (Socket, Instance, I3cDevCtrlReg, I3cDevCtrl.Data);
  }

  return Status;
}

/**
  I3C Send CCC Commands.

  @param[in]  Socket      - Socket Id
  @param[in]  Dev         - Device address on I3C
  @param[in]  CccCommand  - CCC Command
  @param[in]  Attribute   - Attribute for Read/Write CCC/None-CCC
  @param[out] Data        - Pointer to Data to store the value
  @param[out] DataLength  - Pointer to the length, in bytes, of Data

  @retval EFI_SUCCESS           - Successfully Send CCC commands
  @retval EFI_DEVICE_ERROR      - Send CCC failed
  @retval EFI_UNSUPPORTED       - Function not supported
  @retval EFI_INVALID_PARAMETER - Invalid Parameter
**/
EFI_STATUS
EFIAPI
SmbSendCccCommand (
  IN     UINT8             Socket,
  IN     SMB_DEVICE_STRUCT Dev,
  IN     UINT8             CccCommand,
  IN     SMB_ATTR_FLAG     Attribute,
  IN OUT UINT8             *Data,
  IN OUT UINT8             *DataLength
  )
{
  EFI_STATUS  Status;

  Status = SmbSendCccCommandWorker (Socket, Dev, CccCommand, Attribute, Data, DataLength);

  // Retry if error detected
  if (EFI_ERROR (Status)) {
    ResetProcSmb (Socket, Dev.address.SmbInstance);
    Status = SmbSendCccCommandWorker (Socket, Dev, CccCommand, Attribute, Data, DataLength);
  }

  return Status;
}

/**
  Set output mode to show I3C CSR traces if enabled by user

  @param[in] PrintFlag                - Flag to print the trace log

  @retval Value of the current output mode
**/
UINT8
EnableI3cAsmOutput (
  IN  BOOLEAN  PrintFlag
  )
{
  return 0; // PEI only
}

/**
  Restore uBIOS output mode

  @param[in] OriginalUbiosOutputMode  - Output mode value to restore
  @param[in] PrintFlag                - Flag to print the trace log

  @retval N/A
**/
VOID
RestoreI3cAsmOutput (
  IN  UINT8    OriginalUbiosOutputMode,
  IN  BOOLEAN  PrintFlag
  )
{
  return; // PEI only
}
