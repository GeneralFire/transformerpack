/** @file
  The SMM implementation of the ProcSmbIpLib for 10nm.

  This implementation provides processor SMbus services for use during SMM.
  A number of Get/Set functions are used by RAS (ProcSmbRecovery.c) in order
  to facilitate SMBus recovery.  Read/Write functions for 10nm are also
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
#include "../Include/ProcSmbRegs.h"
#include <UncoreCommonIncludes.h>
#include <ProcSmbChipCommon10nm.h>
#include <Library/DebugLib.h>
#include <Library/SemaphoreLib.h>
#include <Include/ProcSmbStruct.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Cpu/CpuIds.h>
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

  // On error, do not attempt to reset SMBus in SMM.  Allow RAS flow to handle recovery.  Simply release the
  // Semaphore and return the status.
  ReleaseSocketSemaphore (SmbSocket, SystemSemaphore0);

  return Status;
}

/**
  Write the processor SMBUS dev/offset with specified data

  @param[in] Dev         - SMBUS device address
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

  // On error, do not attempt to reset SMBus in SMM.  Allow RAS flow to handle recovery.  Simply release the
  // Semaphore and return the status.
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
  UINT32                   SmbCmdReg;
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;


  if (SmbInstance == 0) {
    SmbCmdReg = SMB_CMD_SMB_SPD0_REG;
  } else {
    SmbCmdReg = SMB_CMD_SMB_SPD1_REG;
  }

  SmbCmd.Data = UsraCsrRead (Socket, PCU_INSTANCE_0, SmbCmdReg);
  SmbCmd.Bits.smb_tsod_poll_en = TsodPollEn;
  SmbCmd.Bits.smb_dti = DTI_TSOD;
  UsraCsrWrite (Socket, PCU_INSTANCE_0, SmbCmdReg, SmbCmd.Data);
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
  // Not implementation needed in Smm
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
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;
  UINT32                   SmbCmdReg;

  SmbCmdReg = GetProcSmbCmdReg (SmbInstance);
  SmbCmd.Data = UsraCsrRead (SktId, 0, SmbCmdReg);
  SmbCmd.Bits.smb_dti = 0x03;  // '0011' specifies TSOD
  UsraCsrWrite (SktId, 0, SmbCmdReg, SmbCmd.Data);

  SmbCmd.Data = UsraCsrRead (SktId, 0, SmbCmdReg);
  SmbCmd.Bits.smb_ba = 0x05;
  SmbCmd.Bits.smb_sa = TsodSa;
  SmbCmd.Bits.smb_wrt = 0; //smb_wrt_cmd and smb_wrt_pntr has been combined in SKX
  SmbCmd.Bits.smb_cmd_trigger = 1;
  UsraCsrWrite (SktId, 0, SmbCmdReg, SmbCmd.Data);
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
  UINT32  SmbStatReg;

  if (SmbInstance > 0) {
    SmbStatReg = SMB_STATUS_SMB_SPD1_REG;
  } else {
    SmbStatReg = SMB_STATUS_SMB_SPD0_REG;
  }

  return SmbStatReg;
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
  UINT32  SmbCmdReg;

  if (SmbInstance > 0) {
    SmbCmdReg = SMB_CMD_SMB_SPD1_REG;
  } else {
    SmbCmdReg = SMB_CMD_SMB_SPD0_REG;
  }

  return SmbCmdReg;
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
  SMB_STATUS_SMB_SPD0_STRUCT  SmbStat;
  UINT32                      SmbStatReg;

  SmbStatReg = GetProcSmbStatReg (SmbInstance);
  SmbStat.Data = UsraCsrRead (SktId, 0, SmbStatReg);

  return SmbStat.Bits.smb_sbe;
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
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;
  UINT32                   SmbCmdReg;

  SmbCmdReg = GetProcSmbCmdReg (SmbInstance);
  SmbCmd.Data = UsraCsrRead (SktId, 0, SmbCmdReg);

  return SmbCmd.Bits.smb_ckovrd;
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
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;
  UINT32                   SmbCmdReg;

  //
  // Deactivate the Soft Reset
  //
  SmbCmdReg = GetProcSmbCmdReg (SmbInstance);
  SmbCmd.Data = UsraCsrRead (SktId, 0, SmbCmdReg);
  SmbCmd.Bits.smb_soft_rst = Softrst;
  UsraCsrWrite (SktId, 0, SmbCmdReg, SmbCmd.Data);
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
  SMB_CMD_SMB_SPD0_STRUCT     SmbCmd;
  UINT32                      SmbCmdReg;
  SMB_STATUS_SMB_SPD0_STRUCT  SmbStat;
  UINT32                      SmbStatReg;

  SmbCmdReg = GetProcSmbCmdReg (SmbInstance);
  SmbCmd.Data = UsraCsrRead (SktId, 0, SmbCmdReg);
  SmbStatReg = GetProcSmbStatReg (SmbInstance);
  SmbStat.Data = UsraCsrRead (SktId, 0, SmbStatReg);

  *TsodSa = SmbStat.Bits.tsod_sa;
  *TsodPollEn = SmbCmd.Bits.smb_tsod_poll_en;
  *SbeEn = SmbCmd.Bits.smb_sbe_en;
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
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;
  UINT32                   SmbCmdReg;

  SmbCmdReg = GetProcSmbCmdReg (SmbInstance);
  SmbCmd.Data = UsraCsrRead (SktId, 0, SmbCmdReg);
  SmbCmd.Bits.smb_tsod_poll_en = TsodPollEn;
  SmbCmd.Bits.smb_sbe_en = SbeEn;
  UsraCsrWrite (SktId, 0, SmbCmdReg, SmbCmd.Data);
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
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;
  UINT32                   SmbCmdReg;

  SmbCmdReg = GetProcSmbCmdReg (SmbInstance);
  SmbCmd.Data = UsraCsrRead (SktId, 0, SmbCmdReg);

  return SmbCmd.Bits.smb_sbe_smi_en;
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
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;
  UINT32                   SmbCmdReg;

  SmbCmdReg = GetProcSmbCmdReg (SmbInstance);
  SmbCmd.Data = UsraCsrRead (SktId, 0, SmbCmdReg);

  //
  // This bit will make the system generate SMI when there is an error on SMBUS
  //
  SmbCmd.Bits.smb_sbe_smi_en = SmiEn;

  //
  // This bit will enable the error recovery and stops TSOD pollilng
  //(other wise, the error gets cleared in the next SMBus transaction)
  //
  SmbCmd.Bits.smb_sbe_en = SbeEn;

  UsraCsrWrite (SktId, 0, SmbCmdReg, SmbCmd.Data);
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
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;
  UINT32                   SmbCmdReg;

  SmbCmdReg = GetProcSmbCmdReg (SmbInstance);
  SmbCmd.Data = UsraCsrRead (SktId, 0, SmbCmdReg);

  //
  // Disable TSOD polling and Smbus recovery enable, so that error recovery can be performed.
  //
  SmbCmd.Bits.smb_tsod_poll_en = 0;
  SmbCmd.Bits.smb_sbe_en = 0;
  UsraCsrWrite (SktId, 0, SmbCmdReg, SmbCmd.Data);
}

/**
  Triggers the Smbus Recovery operation.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Instance

  @retval VOID
**/
VOID
TriggerSmbRecovery (
  IN UINT8  SktId,
  IN UINT8  SmbInstance
  )
{
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;
  UINT32                   SmbCmdReg;

  SmbCmdReg = GetProcSmbCmdReg (SmbInstance);
  SmbCmd.Data = UsraCsrRead (SktId, 0, SmbCmdReg);

  //
  // Activate the Smbus recovery process by setting smb_soft_rst to 1 and smb_ckovrd to 0
  // When the recovery is complete, h/w will set smb_ckovrd to '1'
  // and it is BIOS responsibility to deactive smb_soft_rst (which is done is later steps)
  //
  SmbCmd.Bits.smb_ckovrd = 0;
  SmbCmd.Bits.smb_soft_rst = 1;
  UsraCsrWrite (SktId, 0, SmbCmdReg, SmbCmd.Data);
}

/**
  Performs the Smbus Recovery operation.

  @param[in] SktId       - Socket number
  @param[in] SmbInstance - Instance

  @retval VOID
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
  ICXR_RAS: SPD_BITMAP.IP_BITMAP for SPD Smbus defaults to 0xC rather than 0x00 resulting in errors
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
  SPD_BITMAP_SMB_SPD0_STRUCT  SmbSpdBitmap;

  SmbSpdBitmap.Data = UsraCsrRead (SktId, 0, SPD_BITMAP_SMB_SPD0_REG);
  SmbSpdBitmap.Bits.ip_bitmap = 0;
  UsraCsrWrite (SktId, 0, SPD_BITMAP_SMB_SPD0_REG, SmbSpdBitmap.Data);

  if (!(IsCpuAndRevision(CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL))) {
    SmbSpdBitmap.Data = UsraCsrRead (SktId, 0, SPD_BITMAP_SMB_SPD1_REG);
    SmbSpdBitmap.Bits.ip_bitmap = 0;
    UsraCsrWrite (SktId, 0, SPD_BITMAP_SMB_SPD1_REG, SmbSpdBitmap.Data);
  }
}
