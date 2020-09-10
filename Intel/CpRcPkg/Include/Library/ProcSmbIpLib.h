/** @file
  ProcSmbIpLib.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef  _PROC_SMB_IP_LIB_
#define  _PROC_SMB_IP_LIB_

#include <Uefi.h>
#include <Memory/ProcSmbChipCommon.h>
#include <Ppi/MemoryPolicyPpi.h>

/**
  Initialize SMBUS controller for given socket

  @param[in] Socket  - processor socket ID

  @retval EFI_SUCCESS  -  Smbus initalized successfully
  @retval EFI_UNSUPPORTED  -  Function not supported
**/
EFI_STATUS
EFIAPI
InitProcSmb (
  IN UINT8    Socket
  );

/**
  Reset SMBUS controller with given Socket and Instance

  @param[in] Socket      - processor socket ID
  @param[in] SmbInstance - Smbus Instance number

  @retval EFI_SUCCESS  -  Smbus reset successfully
  @retval EFI_UNSUPPORTED  -  Function not supported
**/
EFI_STATUS
EFIAPI
ResetProcSmb (
  IN UINT8   Socket,
  IN UINT16  SmbInstance
  );

/**
  Adjust the SMBUS frequency to the specified value.

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
  );

/**
  Read SMBUS register at a given device and offset

  @param[in]  Dev         Device address on SMBUS
  @param[in]  ByteOffset  Offset in Device
  @param[out] Data    Pointer to store value
  @param NOTE: For a PCH block read, the array must be 33bytes in length,
  @param and the first data byte pointed to is the number of bytes read.
  @param This implementation only allows a maximum of 32bytes to be read.

  @retval EFI_SUCCESS   Read was successful
  @retval EFI_DEVICE_ERROR   Read failed
  @retval EFI_UNSUPPORTED  Function not supported
**/
EFI_STATUS
EFIAPI
ReadProcSmb (
  IN  SMB_DEVICE_STRUCT Dev,
  IN  UINT8             ByteOffset,
  OUT volatile UINT8    *Data
  );

/**
  Write SMBUS dev/offset with specified data

  @param[in] dev         - SMBUS device address
  @param[in] byteOffset  - byte offset within device
  @param[in] data        - pointer to data to write
  @param NOTE: For a PCH block write, the first data byte pointed to must be the number of bytes to write.
  @param This implementation only allows a maximum of 32bytes to be written.

  @retval EFI_SUCCESS   Write was successful
  @retval EFI_DEVICE_ERROR   Write failed
  @retval EFI_UNSUPPORTED  Function not supported
**/
EFI_STATUS
EFIAPI
WriteProcSmb (
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              ByteOffset,
  IN UINT8              *Data
  );

/**
  Get slave device type - i2cni3c

  @param[in] Socket       - Socket number
  @param[in] SmbInstance  - SMBus Instance number

  @rval SlaveType   - slave device type

**/
UINT8
GetSlaveType (
  IN UINT8  Socket,
  IN UINT8  Instance
  );

/**
  Returns the SMBus strap address of the DIMM slot

  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number
  @param[in]  Dimm    - Dimm number
  @param[out] Dev     - Pointer to smbDevice

  @retval EFI_SUCCESS  Successfully retrieved SMBus address data
**/
EFI_STATUS
EFIAPI
GetSmbAddress (
  IN UINT8               Socket,
  IN UINT8               Ch,
  IN UINT8               Dimm,
  OUT SMB_DEVICE_STRUCT  *Dev
  );

/**
  Calculates the Channel and Dimm number based on the SMBus address information.

  @param[in]  SpdDev - ptr to smbDevice which contains bus Instance and strap address data
  @param[out] Chan   - Channel number based on SMBus address
  @param[out] Dimm   - Dimm number based on SMBUs address

  @rval EFI_SUCCESS  Successfully calculated channel and dimm number
**/
EFI_STATUS
EFIAPI
GetChDimmFromSmbAddress (
  IN SMB_DEVICE_STRUCT  *Dev,
  OUT UINT8             *Chan,
  OUT UINT8             *Dimm
  );

/**
  This routine gets the Tsod Polling enable bit value

  @param[in] Socket   - Socket Id
  @param[in] SmbInstance  - SMBus Instance number

  @retval TsodPollEn  value of Tsod poll enable bit
**/
UINT32
GetTsodPollEnSmb (
  IN UINT8                 Socket,
  IN UINT16                SmbInstance
  );

/**
  This routine sets Tsod Polling enable bit to the given value

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
  );

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
  );

/**
  This routine updates the Tsod population based on DIMM types

  @param[in] Socket   Socket ID
  @param[in] McId     Memory controller ID

  @retval VOID
**/
VOID
UpdateTsodPopulation (
  IN UINT8  Socket,
  IN UINT8  McId
  );

/**
  This routine compares RegOffset to SMB_CMD registers for a match

  @param[in] RegOffset                  - Register address

  @retval 1 for match
  @retval 0 for no match
**/
UINT32
IsRegCmdSmb (
  IN UINT32                RegOffset
  );

/**
  This routine checks if the offset belongs to SMB

  @param[in] RegOffset                  - Register address

  @retval 1 for pass
  @retval 0 for no pass
**/
UINT32
IsRegFilterPassSmb (
  IN UINT32                RegOffset
  );

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
  );

/**
  This routine set the field values for the VPP Control Smbus Register

  @param[in] Socket                      - Socket Id
  @param[in] ResetModeBit                - Reset the VPP state machine:
                                           0: Power good reset will reset the VPP state machines and warm reset will
                                           cause the VPP state machine to terminate at the next logical VPP stream
                                           boundary and then reset the VPP state machines.
                                           1: Both power good and warm reset will reset the VPP state machines.
  @param[in] ErrorSeverityBit            - Control the severity of VPP error
                                           0: Fatal 1: Non Fatal 2: Correctable
                                           Default is 0.
  @param[in] MessagChannMasteringEnBit   - Value 1 enables sending slot status messages to IIO and error messages UBOX.
                                           Default is 0.
**/
VOID
ProgramVppCtrlSmbReg (
  IN UINT8   Socket,
  IN UINT8   ResetModeBit,
  IN UINT8   ErrorSeverityBit,
  IN UINT8   MessagChannMasteringEnBit
  );

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
  );

/**
  This routine gets smbus status register

  @param[in] SmbInstance  - Processor Smbus Instance

  @retval  register address
**/
UINT32
GetProcSmbStatReg (
  IN UINT8  SmbInstance
  );

/**
  This routine gets smbus command register

  @param[in] SmbInstance  - Processor Smbus Instance

  @retval register address
**/
UINT32
GetProcSmbCmdReg (
  IN UINT8  SmbInstance
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  ICXR_RAS: SPD_BITMAP.IP_BITMAP for SPD Smbus defaults to 0xC rather than 0x00 resulting in errors
  logged in global error logic rather than local error logic of Global IEH

  @param [IN]  Socket    The socket number
  @retval  VOID
 **/
VOID
EFIAPI
ClearSmbIpBitmap (
  IN  UINT8   Socket
  );

/**
  I3cSwitchToI3cMode.

  The SETAASA CCC is only supported when device is in I2C mode. When SETAASA CCC is registered by the SPD5
  Hub, it updates Table 117, "MR18" [5] = '1'. Table 71 shows an example of a single SETAASA CCC.
  SETAASA CCC does not support PEC function as device is in I2C mode and there is no PEC function in I2C mode.

  @param[in] Socket        - Socket Id

  @rval EFI_SUCCESS       Successfully Switch To I3c Mode
  @rval EFI_DEVICE_ERROR  Cannot Switch To I3c Mode
  @rval EFI_UNSUPPORTED   Do not support Switch To I3c Mode
**/
EFI_STATUS
EFIAPI
I3cSwitchToI3cMode (
  IN  UINT8    Socket
  );

/**
  Smbus Send CCC Commands.

  @param[in]  Socket      - Socket Id
  @param[in]  Dev         - Device address
  @param[in]  CccCommand  - CCC Command
  @param[in]  Attribute   - Attribute for Read/Write CCC/None-CCC
  @param[out] Data        - Pointer to Data to store the value
  @param[out] DataLength  - Pointer to the length, in bytes, of Data

  @retval EFI_UNSUPPORTED       - Function not supported
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
  );

/**
  Set output mode to show I3C CSR traces if enabled by user

  @param[in] PrintFlag                - Flag to print the trace log

  @retval Value of the current output mode
**/
UINT8
EnableI3cAsmOutput (
  IN  BOOLEAN  PrintFlag
  );

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
  );

#endif  //_PROC_SMB_IP_LIB_
