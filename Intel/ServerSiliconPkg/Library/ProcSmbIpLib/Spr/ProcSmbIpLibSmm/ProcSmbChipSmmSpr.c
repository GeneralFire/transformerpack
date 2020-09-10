/** @file
  The SMM implementation of the ProcSmbIpLib for Spr.

  This implementation provides processor I3C services for use during SMM.
  A number of Get/Set functions are used by RAS (ProcSmbRecovery.c) in order
  to facilitate I3C recovery.  Read/Write functions for Spr are also
  implemented for use.  Read/Write functions call into the common file
  (shared with DXE) for the low-level interactions.

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
#include <Library/DebugLib.h>
#include <Library/SemaphoreLib.h>
#include <Include/ProcSmbStruct.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/UsraCsrLib.h>
#include <Cpu/CpuIds.h>

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
  return EFI_SUCCESS;
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
  @retval EFI_NOT_READY     Unable to obtain semaphore
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
  // Attempt to acquire semaphore - if unsuccessful, do not wait in SMM
  SemaphoreAcquired = AcquireSocketSemaphore (SmbSocket, SystemSemaphore0, DO_NOT_ADD_TO_QUEUE, &QueueNumber);

  if (!SemaphoreAcquired) {
    return EFI_NOT_READY;
  }

  Status = SmbReadCommon (SmbSocket, Dev, ByteOffset, Data);

  // On error, do not attempt to reset I3C in SMM.  Allow RAS flow to handle recovery.  Simply release the
  // Semaphore and return the status.
  ReleaseSocketSemaphore (SmbSocket, SystemSemaphore0);

  return Status;
}

/**
  Write the processor I3C dev/offset with specified data

  @param[in] Dev         - I3C device address
  @param[in] ByteOffset  - byte offset within device
  @param[in] Data        - pointer to data to write

  @retval EFI_SUCCESS      - Write was successful
  @retval EFI_NOT_READY    - Unable to obtain semaphore
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
  // Attempt to acquire semaphore - if unsuccessful, do not wait in SMM
  SemaphoreAcquired = AcquireSocketSemaphore (SmbSocket, SystemSemaphore0, DO_NOT_ADD_TO_QUEUE, &QueueNumber);

  if (!SemaphoreAcquired) {
    return EFI_NOT_READY;
  }

  Status = SmbWriteCommon (SmbSocket, Dev, ByteOffset, Data);

  // On error, do not attempt to reset I3C in SMM.  Allow RAS flow to handle recovery.  Simply release the
  // Semaphore and return the status.
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

  @param[in]  SpdDev - ptr to smbDevice which contains bus Instance and strap address data
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
  // Not needed for SMM
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
  This routine compares RegOffset to SMB_CMD registers for a match

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
  This routine gets processor smbus data register

  @param[in] Socket       - Socket Id
  @param[in] SmbInstance  - Processor Smbus Instance
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
  This routine sets tsod smbus slave address for reading

  @param[in] Socket       - Socket Id
  @param[in] SmbInstance  - Processor Smbus Instance
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
  return;  // UNSUPPORTED
}

/**
  This routine gets I3C status register

  @param[in] SmbInstance  - Processor Smbus Instance

  @retval register address
**/
UINT32
GetProcSmbStatReg (
  IN UINT8  SmbInstance
  )
{
  UINT32  I3cStatusReg;

  if (SmbInstance > 0) {
    I3cStatusReg = INTR_STATUS_SB_I3C1_REG;
  } else {
    I3cStatusReg = INTR_STATUS_SB_I3C0_REG;
  }

  return I3cStatusReg;
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
  UINT32  TsodCmdReg;

  TsodCmdReg = 0x0;  // UNSUPPORTED

  return TsodCmdReg;
}

/**
  This routine gets I3C error bit

  @param[in] SktId    - Socket number
  @param[in] SmbInstance  - Processor Smbus Instance

  @retval  I3C error bit value
**/
UINT32
GetProcSmbSbe (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  return 0x0;  // UNSUPPORTED
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
  return 0x0;  // UNSUPPORTED
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
  return;  // UNSUPPORTED
}

/**
  This routine gets information on TSOD slave address, Tsod polling bit
  and I3C recover enable bit info

  @param[in] SktId        - Socket number
  @param[in] SmbInstance  - Processor Smbus Instance
  @param[out] TsodSa      - Tsod Slave Address
  @param[out] TsodPollEn  - Tsode Polling enable bit info
  @param[out] SbeEn       - I3C error recovery enable bit

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
  // UNSUPPORTED
  *TsodSa = 0x0;
  *TsodPollEn = 0x0;
  *SbeEn = 0x0;
}

/**
  This routine sets information on Tsod polling bit
  and I3C recover enable bit info

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Processor Smbus Instance
  @param[in] TsodPollEn  - Tsode Polling enable bit info
  @param[in] SbeEn       - I3C error recovery enable bit

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
  return;  // UNSUPPORTED
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
  return 0;  // UNSUPPORTED
}

/**
  This routine sets smi enable on SBE bit. Also sets\resets SBE (I3C error recovery)
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
  return;  // UNSUPPORTED
}

/**
  Prepares the I3C Recovery Operation by disabling TSOD polling
  and I3C recovery enable.

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
  return;  // UNSUPPORTED
}

/**
  Triggers the I3C Recovery operation.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Instance

  @retval  VOID
**/
VOID
TriggerSmbRecovery (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  return;  // UNSUPPORTED
}

/**
  Performs the I3C Recovery operation.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Instance

  @retval  VOID
**/
VOID
DoProcSmbRecovery (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  DisableTsodAndSmbSbeEn (SktId, SmbInstance);
  TriggerSmbRecovery (SktId, SmbInstance);
}

/**
  Returns I3C Instance for given McId, by mapping Instance to
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
  ICXR_RAS: SPD_BITMAP.IP_BITMAP for SPD I3C defaults to 0xC rather than 0x00 resulting in errors
  logged in global error logic rather than local error logic of Global IEH

  @param [IN]  SktId    The socket number

  @retval VOID
 **/
VOID
EFIAPI
ClearSmbIpBitmap (
  IN  UINT8   SktId
  )
{
  return;  // UNSUPPORTED
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
  return EFI_UNSUPPORTED; // PEI only
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

