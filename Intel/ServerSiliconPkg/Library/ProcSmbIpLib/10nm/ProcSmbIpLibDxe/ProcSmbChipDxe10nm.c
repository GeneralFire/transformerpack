/** @file
  The DXE implementation of the ProcSmbIpLib for 10nm.

  This implementation provides processor SMbus services for use during DXE.
  Most functions are not needed during DXE phase, so function stubs are
  used.  Reset and Read/Write functions for 10nm are implemented for use.
  Read/Write functions call into the common file (shared with SMM) for the
  low-level interactions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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
#include "../Include/ProcSmbRegs.h"
#include <UncoreCommonIncludes.h>
#include <ProcSmbChipCommon10nm.h>
#include <Library/TimerLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/DebugLib.h>
#include <Library/SemaphoreLib.h>
#include <Library/ProcSmbIpLib.h>
#include <CpuPciAccess.h>


/**
  Initialize SMBUS controller for given socket

  @param[in] Socket  - processor socket ID

  @retval EFI_SUCCESS      Smbus initalized successfully
  @retval EFI_UNSUPPORTED  Function not supported
**/
EFI_STATUS
EFIAPI
InitProcSmb (
  IN UINT8  Socket
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Reset SMBUS controller with given Socket and Instance

  @param[in] Socket      - processor socket ID
  @param[in] SmbInstance - Smbus Instance number

  @retval EFI_SUCCESS      Smbus reset successfully
  @retval EFI_UNSUPPORTED  Function not supported
**/
EFI_STATUS
EFIAPI
ResetProcSmb (
  IN UINT8   Socket,
  IN UINT16  SmbInstance
  )
{
  UINT32                   SmbCmdReg;
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;

  //
  // Force a soft reset and retry transaction
  //
  if (SmbInstance == 0) {
    SmbCmdReg = SMB_CMD_SMB_SPD0_REG;
  } else {
    SmbCmdReg = SMB_CMD_SMB_SPD1_REG;
  }
  SmbCmd.Data = UsraCsrRead (Socket, 0, SmbCmdReg);

  //
  // If slave device is hung, software can write this bit to 1 and
  // the SMB_CKOVRD to 0 (for more than 35ms) to force hung the SMB
  // slaves to time-out and put it in idle state without using power
  // good reset or warm reset.
  //

  //
  // Override the clock
  //
  SmbCmd.Bits.smb_ckovrd = 0;
  SmbCmd.Bits.smb_soft_rst = 1;
  UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);
  //
  // Wait 35ms
  //
  MicroSecondDelay (35);
  SmbCmd.Bits.smb_ckovrd   = 1;
  SmbCmd.Bits.smb_soft_rst = 0;
  UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);
  return EFI_SUCCESS;
}

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
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Read the processor SMBUS device at a given device and offset

  @param[in] Dev         - SMBUS Device address
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
  // Acquire semaphore to read SMBus
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
  Write the processor SMBUS dev/offset with specified data.

  @param[in] Dev         - SMBUS device address
  @param[in] ByteOffset  - byte offset within device
  @param[in] Data        - pointer to data to write

  @retval EFI_SUCCESS       Read was successful
  @retval EFI_DEVICE_ERROR  Read failed
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
  )
{
  EFI_STATUS  Status;

  Status = GetSmbAddressCommon (Socket, Ch, Dimm, Dev);

  return Status;
}

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
  UINT32                   SmbCmdReg;
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;


  if (SmbInstance == 0) {
    SmbCmdReg = SMB_CMD_SMB_SPD0_REG;
  } else {
    SmbCmdReg = SMB_CMD_SMB_SPD1_REG;
  }
  SmbCmd.Data = UsraCsrRead (Socket, PCU_INSTANCE_0, SmbCmdReg);

  return SmbCmd.Bits.smb_tsod_poll_en;
}

/**
  This routine sets smb_tsod_poll_en to the given value

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
  //Should only ever be called in SMM
  ASSERT (FALSE);
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
  )
{
  // Not implementation needed in Dxe
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
  return;
}

/**
  This routine gets smbus status register

  @param[in] SmbInstance  - Processor Smbus Instance

  @retval  register address
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

