/** @file
  The PEI implementation of the ProcSmbIpLib for 10nm.

  This implementation provides processor SMbus services for use during PEI.
  Functions are implemented for use with 10nm.  Reset, Initialize, Read, and
  Write functions are implemented as well as a variety of other misc
  functions which provide data from and/or interact with the processor
  SMBus controller.  SMBus initialization should be called before attempting
  to interact with the controller.

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

#include "../Include/ProcSmbRegs.h"
#include "../Include/ProcSmbEmulation.h"
#include <UncoreCommonIncludes.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/IoAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SemaphoreLib.h>
#include <ProcSmbChipCommon10nm.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/ProcSmbIpTargetLib.h>
#include <Library/HardwareSemaphoreTargetLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/ProcMemInitTargetLib.h>
#include <Library/MemoryCoreLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Memory/ProcSmbChipCommon.h>
#include <CpuPciAccess.h>
#include <Library/MemRcLib.h>
#include <Library/PerformanceTrackerLib.h>

/**
  Adjust the SMBUS frequency if DDRT is detected in the system.

  @param[in] Socket       - Processor socket ID
  @param[in] SmbInstance   - SMBUS Instance number
  @param[in] ClkFrequency - Pointer to the requested SMB frequency value

  @retval NA
**/
VOID
AdjustFreqForDdrt (
  IN UINT8                    Socket,
  IN UINT8                    SmbInstance,
  IN OUT SMB_CLOCK_FREQUENCY  *ClkFrequency
  )
{
  UINT8                       Ch;
  UINT8                       Dimm;
  PSYSHOST                    Host;
  SMB_DEVICE_STRUCT           SmbDev;
  CHANNEL_NVRAM_STRUCT (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT    (*DimmNvList)[MAX_DIMM];
  UINT8                MaxChDdr;

  // If requested frequency is less than or equal to 400Khz, no need to adjust
  if (*ClkFrequency <= SmbClk400K) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();

  ChannelNvList = GetChannelNvList (Host, Socket);

  // Loop through channels and DIMMs searching for DDRT on this Instance
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    // For each DIMM
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < (*ChannelNvList)[Ch].maxDimm; Dimm++) {
      // Retrieve Instance for current DIMM
      GetSmbAddress (Socket, Ch, Dimm, &SmbDev);
      if (SmbDev.address.SmbInstance != SmbInstance) {
        continue;  // Don't care about this Instance, so continue
      }
      // If DDRT DIMM present, adjust frequency to 400 Khz and return
      if ((*DimmNvList)[Dimm].DcpmmPresent != 0) {
        *ClkFrequency = SmbClk400K;
        return;
      }
    }
  }
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
  PSYSHOST                    Host;
  UINT32                      SmbPeriodReg;
  SMB_PERIOD_SMB_SPD0_STRUCT  SmbPeriod;

  Host = GetSysHostPointer ();

  // Validate incoming parameters
  if (Socket >= MAX_SOCKET) {
    return EFI_INVALID_PARAMETER;
  }

  // Check for invalid Instance
  if (SmbInstance >= MAX_SMB_INSTANCE) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "Invalid SMBUS SmbInstance Parameter\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_84);
    return EFI_INVALID_PARAMETER;
  }

  AdjustFreqForDdrt (Socket, SmbInstance, &ClkFrequency);

  if (SmbInstance == 0) {
    SmbPeriodReg = SMB_PERIOD_SMB_SPD0_REG;
  } else {
    SmbPeriodReg = SMB_PERIOD_SMB_SPD1_REG;
  }

  SmbPeriod.Data = UsraCsrRead (Socket, 0, SmbPeriodReg);

  switch (ClkFrequency) {
  case SmbClk100K:
    if ((SmbPeriod.Bits.smb_clk_prd == SMB_100K_CLK_PERIOD) && (SmbPeriod.Bits.smb_clk_offset == SMB_100K_CLK_OFFSET)) {
      return EFI_SUCCESS;
    }

    SmbPeriod.Bits.smb_clk_prd = SMB_100K_CLK_PERIOD;
    SmbPeriod.Bits.smb_clk_offset = SMB_100K_CLK_OFFSET;
    break;

  case SmbClk400K:
    if ((SmbPeriod.Bits.smb_clk_prd == SMB_400K_CLK_PERIOD) && (SmbPeriod.Bits.smb_clk_offset == SMB_400K_CLK_OFFSET)) {
      return EFI_SUCCESS;
    }

    SmbPeriod.Bits.smb_clk_prd = SMB_400K_CLK_PERIOD;
    SmbPeriod.Bits.smb_clk_offset = SMB_400K_CLK_OFFSET;
    break;

  case SmbClk700K:
    if ((SmbPeriod.Bits.smb_clk_prd == SMB_700K_CLK_PERIOD) && (SmbPeriod.Bits.smb_clk_offset == SMB_700K_CLK_OFFSET)) {
      return EFI_SUCCESS;
    }

    SmbPeriod.Bits.smb_clk_prd = SMB_700K_CLK_PERIOD;
    SmbPeriod.Bits.smb_clk_offset = SMB_700K_CLK_OFFSET;
    break;

  case SmbClk1M:
    if ((SmbPeriod.Bits.smb_clk_prd == SMB_1M_CLK_PERIOD) && (SmbPeriod.Bits.smb_clk_offset == SMB_1M_CLK_OFFSET)) {
      return EFI_SUCCESS;
    }

    SmbPeriod.Bits.smb_clk_prd = SMB_1M_CLK_PERIOD;
    SmbPeriod.Bits.smb_clk_offset = SMB_1M_CLK_OFFSET;
    break;

  default:
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Invalid SMBUS ClkFrequency Parameter: %x\n", ClkFrequency);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_85);
    return EFI_INVALID_PARAMETER;
    break;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Bus Instance = %d SMB Clock Period = %d (in 10ns units)\n", SmbInstance, SmbPeriod.Bits.smb_clk_prd);

  UsraCsrWrite (Socket, 0, SmbPeriodReg, SmbPeriod.Data);

  return EFI_SUCCESS;
}

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
  UINT32                      SmbCmdReg;
  UINT32                      SmbStatReg;
  UINT8                       SmbInstance;
  SMB_STATUS_SMB_SPD0_STRUCT  SmbStat;
  SMB_CMD_SMB_SPD1_STRUCT     SmbCmd;

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DIMM_DETECT, CHECKPOINT_MINOR_INIT_SMB, Socket));

  for (SmbInstance = 0; SmbInstance < MAX_SMB_INSTANCE; SmbInstance++) {

    // Initialize SMBus speed to 400Khz for DIMM detection.  The speed will be adjusted after
    // DIMM detection is complete.
    AdjustProcSmbFreq (Socket, SmbInstance, SmbClk400K);

    if (SmbInstance == 0) {
      SmbCmdReg = SMB_CMD_SMB_SPD0_REG;
      SmbStatReg = SMB_STATUS_SMB_SPD0_REG;
    } else {
      SmbCmdReg = SMB_CMD_SMB_SPD1_REG;
      SmbStatReg = SMB_STATUS_SMB_SPD1_REG;
    }

    //
    // Check if the first controller is busy
    //
    SmbStat.Data = UsraCsrRead (Socket, 0, SmbStatReg);
    if (SmbStat.Bits.smb_busy) {
      SmbCmd.Data = UsraCsrRead (Socket, 0, SmbCmdReg);

      //
      // Override the clock
      //
      SmbCmd.Bits.smb_ckovrd = 0;
      SmbCmd.Bits.smb_soft_rst = 1;
      UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);
      //
      // Wait 35ms
      //
      FixedDelayMicroSecond (35000);
      SmbCmd.Bits.smb_ckovrd   = 1;
      SmbCmd.Bits.smb_soft_rst = 0;
      UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);
    }

    //
    // Make Sure TSOD Polling is disabled
    //
    SmbCmd.Data = UsraCsrRead (Socket, 0, SmbCmdReg);
    SmbCmd.Bits.smb_tsod_poll_en = 0;
    UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);
  }
  return EFI_SUCCESS;
} //InitProcSmb

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
  FixedDelayMicroSecond (35000);
  SmbCmd.Bits.smb_ckovrd   = 1;
  SmbCmd.Bits.smb_soft_rst = 0;
  UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);
  return EFI_SUCCESS;
} //ResetProcSmb

/**
  Read SMBUS register at a given device and offset

  @param[in]  Socket      - Processor socket ID
  @param[in]  Dev         - Device address on SMBUS
  @param[in]  ByteOffset  - Offset in Device
  @param[out] Data        - Pointer to store value

  @retval EFI_SUCCESS       Read was successful
  @retval EFI_DEVICE_ERROR  Read failed
  @retval EFI_UNSUPPORTED   Function not supported
**/
EFI_STATUS
EFIAPI
ReadProcSmbWorker (
  IN  UINT8              Socket,
  IN  SMB_DEVICE_STRUCT  Dev,
  IN  UINT8              ByteOffset,
  OUT volatile UINT8     *Data
  )
{
  UINT32                            SmbCmdReg;
  UINT32                            SmbStatReg;
  UINT32                            SmbDataReg;
  UINT32                            SmbSavedCmdData;
  SMB_STATUS_SMB_SPD0_STRUCT        SmbStat;
  volatile SMB_DATA_SMB_SPD0_STRUCT SmbData;
  SMB_CMD_SMB_SPD0_STRUCT           SmbCmd;
  EFI_STATUS                        Status;
  PSYSHOST                          Host;
  BOOLEAN                           ShouldBreak = FALSE;

  Host = GetSysHostPointer ();

  Status = EFI_SUCCESS;

  CountTrackingData (PTC_SMBUS_READ, 1);

  if (Dev.address.SmbInstance == 0) {
    SmbCmdReg = SMB_CMD_SMB_SPD0_REG;
    SmbStatReg = SMB_STATUS_SMB_SPD0_REG;
    SmbDataReg = SMB_DATA_SMB_SPD0_REG;
  } else {
    SmbCmdReg = SMB_CMD_SMB_SPD1_REG;
    SmbStatReg = SMB_STATUS_SMB_SPD1_REG;
    SmbDataReg = SMB_DATA_SMB_SPD1_REG;
  }

  //
  // Ensure TSOD polling is disabled before continuing
  //
  DisableTsod (Dev.address.SmbInstance, Socket, &SmbSavedCmdData);

  //
  // Form read command
  //
  SmbCmd.Data = 0;
  SmbCmd.Bits.smb_ckovrd = 1;
  SmbCmd.Bits.smb_ba = (UINT32) ByteOffset;
  SmbCmd.Bits.smb_dti = Dev.address.deviceType;
  SmbCmd.Bits.smb_sa = (UINT32) Dev.address.strapAddress;
  SmbCmd.Bits.smb_wrt = 0;
  SmbCmd.Bits.smb_pntr_sel = 0;
  if (Dev.compId == MTS) {
    SmbCmd.Bits.smb_word_access = 1;
  } else {
    SmbCmd.Bits.smb_word_access = 0;
  }
  SmbCmd.Bits.smb_cmd_trigger = 1;

  WaitForHostNotBusyTarget (Socket, 0, SmbStatReg);
  PollSmbusStatusRegister (Socket, 0, SmbStatReg, SMB_BUSY_BIT_MASK, SMB_BUSY_POLLING_LOGIC);

  //
  // Send command
  //
  UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);

  WaitForHostNotBusyTarget (Socket, 0, SmbStatReg);
  PollSmbusStatusRegister (Socket, 0, SmbStatReg, SMB_BUSY_BIT_MASK, SMB_BUSY_POLLING_LOGIC);

  SmbStat.Data = UsraCsrRead (Socket, 0, SmbStatReg);

  //
  // Wait for the data
  //
  while (!SmbStat.Bits.smb_rdo && !SmbStat.Bits.smb_sbe) {
    SmbStat.Data = UsraCsrRead (Socket, 0, SmbStatReg);
    CountTrackingData (PTC_POLLING_COUNT, 1);
    BreakForSmbDataTarget (&ShouldBreak);
    if (ShouldBreak) {
      break;
    }
  }

  PollSmbusStatusRegister (Socket, 0, SmbStatReg, SMB_BUSY_BIT_MASK, SMB_BUSY_POLLING_LOGIC);

  //
  // If read data is valid
  //
  if (SmbStat.Bits.smb_rdo && !SmbStat.Bits.smb_sbe) {
    SmbData.Data = UsraCsrRead (Socket, 0, SmbDataReg);
    if (Dev.compId == MTS) {
      *Data = (UINT8) SmbData.Bits.smb_rdata;  //lsb
      *(Data + 1) = (UINT8) (SmbData.Bits.smb_rdata >> 8); //msb
    } else {
      *Data = (UINT8) SmbData.Bits.smb_rdata;
    }
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  RestoreTsod (Dev.address.SmbInstance, Socket, &SmbSavedCmdData);

  return Status;
} //ReadProcSmb

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
  UINT8       SmbSocket;

  Status = EFI_SUCCESS;

  SmbSocket = (UINT8) Dev.address.SmbSocket;
  // Acquire SMBUS Semaphore
  AcquireSmbusSemaphoreControl (SmbSocket);

  Status = ReadProcSmbWorker (SmbSocket, Dev, ByteOffset, Data);
  if (EFI_ERROR (Status) && IsSmbSimulated ()) {
    Status = EFI_SUCCESS; // If in a simulated environment, do not report SMBus failures
    *Data = 0;            // make sure data is determinant
  }
  // Retry if error detected
  if (EFI_ERROR (Status)) {
    ResetProcSmb (SmbSocket, Dev.address.SmbInstance);
    Status = ReadProcSmbWorker (SmbSocket, Dev, ByteOffset, Data);
  }

  // Release Semaphore
  ReleaseSmbusSemaphoreControl (SmbSocket);

  return Status;
}

/**
  Write SMBUS dev/offset with specified data

  @param[in] Socket      - Processor socket ID
  @param[in] Dev         - SMBUS device address
  @param[in] ByteOffset  - byte offset within device
  @param[in] Data        - pointer to data to write

  @retval EFI_SUCCESS       Write was successful
  @retval EFI_DEVICE_ERROR  Write failed
  @retval EFI_UNSUPPORTED   Function not supported
**/
EFI_STATUS
EFIAPI
WriteProcSmbWorker (
  IN UINT8              Socket,
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              ByteOffset,
  IN UINT8              *Data
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINT32                      SmbCmdReg;
  UINT32                      SmbStatReg;
  UINT32                      SmbDataReg;
  UINT32                      SmbSavedCmdData;
  SMB_CMD_SMB_SPD0_STRUCT     SmbCmd;
  SMB_DATA_SMB_SPD0_STRUCT    SmbData;
  PSYSHOST                    Host;

  Host = GetSysHostPointer ();

  if (IsHvmModeEnable ()) {
    return Status;
  }

  CountTrackingData (PTC_SMBUS_WRITE, 1);

  if (Dev.address.SmbInstance == 0) {
    SmbCmdReg = SMB_CMD_SMB_SPD0_REG;
    SmbStatReg = SMB_STATUS_SMB_SPD0_REG;
    SmbDataReg = SMB_DATA_SMB_SPD0_REG;
  } else {
    SmbCmdReg = SMB_CMD_SMB_SPD1_REG;
    SmbStatReg = SMB_STATUS_SMB_SPD1_REG;
    SmbDataReg = SMB_DATA_SMB_SPD1_REG;
  }
  //
  // Ensure TSOD polling is disabled before continuing
  //
  DisableTsod (Dev.address.SmbInstance, Socket, &SmbSavedCmdData);

  //
  // Form write command
  //
  SmbCmd.Data = 0;
  SmbCmd.Bits.smb_ckovrd = 1;
  SmbCmd.Bits.smb_dis_wrt = 0;
  SmbCmd.Bits.smb_dti = Dev.address.deviceType;
  SmbCmd.Bits.smb_ba = (UINT32) ByteOffset;
  SmbCmd.Bits.smb_sa = (UINT32) Dev.address.strapAddress;
  SmbCmd.Bits.smb_wrt = 1;
  if (Dev.compId == MTS) {
    SmbCmd.Bits.smb_word_access = 1;
  } else {
    SmbCmd.Bits.smb_word_access = 0;
  }
  SmbCmd.Bits.smb_pntr_sel = 0;
  SmbCmd.Bits.smb_cmd_trigger = 1;

  //
  // data to write
  //
  SmbData.Data = 0;
  if (Dev.compId == MTS) {
    SmbData.Bits.smb_wdata = (UINT32) *Data + ((UINT32) *(Data + 1) << 8);
  } else {
    SmbData.Bits.smb_wdata = (UINT32) *Data;
  }

  WaitForHostNotBusyTarget (Socket, 0, SmbStatReg);
  PollSmbusStatusRegister (Socket, 0, SmbStatReg, SMB_BUSY_BIT_MASK, SMB_BUSY_POLLING_LOGIC);

  //
  // Send command
  //
  UsraCsrWrite (Socket, 0, SmbDataReg, SmbData.Data);
  UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);

  Status = WaitForWriteToCompleteTarget (Socket, 0, SmbStatReg);
  PollSmbusStatusRegister (Socket, 0, SmbStatReg, SMB_BUSY_BIT_MASK, SMB_BUSY_POLLING_LOGIC);

  RestoreTsod (Dev.address.SmbInstance, Socket, &SmbSavedCmdData);

  return Status;
} //WriteProcSmb

/**
  Write the processor SMBUS dev/offset with specified data

  @param[in] Dev         - SMBUS device address
  @param[in] ByteOffset  - byte offset within device
  @param[in] Data        - pointer to data to write

  @retval EFI_SUCCESS      - Read was successful
  @retval EFI_DEVICE_ERROR - Read failed
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
  UINT8       SmbSocket;

  Status = EFI_SUCCESS;

  SmbSocket = (UINT8) Dev.address.SmbSocket;
  // Acquire SMBUS Semaphore
  AcquireSmbusSemaphoreControl (SmbSocket);

  Status = WriteProcSmbWorker (SmbSocket, Dev, ByteOffset, Data);
  if (IsSmbSimulated ()) {
    Status = EFI_SUCCESS;
  }
  // Retry if error detected
  if (EFI_ERROR (Status)) {
    ResetProcSmb (SmbSocket, Dev.address.SmbInstance);
    Status = WriteProcSmbWorker (SmbSocket, Dev, ByteOffset, Data);
  }

  // Release Semaphore
  ReleaseSmbusSemaphoreControl (SmbSocket);

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
  UINT8                 NumChPerMc;
  UINT8                 MaxImc;
  NumChPerMc       =    GetNumChannelPerMc ();
  MaxImc           =    GetMaxImc ();

  if (IsBrsPresent (NULL, 0)) {
    //
    // As BRS board uses an special mapping in the board,
    // then it is required to unmapped it to match the Dimm population in the INI File
    // This would translate to:
    // BusSeg   StrapAddress    INI File Mc-Ch-Dimm
    // 0        0               0-0-0
    // 0        1               0-0-1
    // 0        2               1-0-0
    // 0        3               1-0-1
    // 1        4               2-0-0
    // 1        5               2-0-1
    // 1        6               3-0-0
    // 1        7               3-0-1
    //
    *Chan = (UINT8) ((Dev->address.strapAddress / 2) * NumChPerMc);
  } else {
    *Chan = (UINT8) ((Dev->address.SmbInstance * (NumChPerMc * MaxImc / 2)) + (Dev->address.strapAddress / MAX_DIMM));
  }
  *Dimm = Dev->address.strapAddress % MAX_DIMM;

  return EFI_SUCCESS;
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
  PSYSHOST                 Host;

  Host = GetSysHostPointer ();

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
  PSYSHOST                 Host;

  Host = GetSysHostPointer ();

  if (SmbInstance == 0) {
    SmbCmdReg = SMB_CMD_SMB_SPD0_REG;
  } else {
    SmbCmdReg = SMB_CMD_SMB_SPD1_REG;
  }

  SmbCmd.Data = UsraCsrRead (Socket, PCU_INSTANCE_0, SmbCmdReg);
  SmbCmd.Bits.smb_tsod_poll_en = TsodPollEn;
  SmbCmd.Bits.smb_dti = DTI_TSOD;
  UsraCsrWrite (Socket, PCU_INSTANCE_0, SmbCmdReg, SmbCmd.Data);
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
  UINT32                              SmbCmdReg;
  SMB_TSOD_POLL_RATE_SMB_SPD0_STRUCT  SmbTsodPollRate;

  if (SmbInstance == 0) {
    SmbCmdReg = SMB_TSOD_POLL_RATE_SMB_SPD0_REG;
  } else {
    SmbCmdReg = SMB_TSOD_POLL_RATE_SMB_SPD1_REG;
  }

  SmbTsodPollRate.Data = MemReadPciCfgEp (Socket, PCU_INSTANCE_0, SmbCmdReg);
  SmbTsodPollRate.Bits.smb_tsod_poll_rate = TsodPollRate;
  MemWritePciCfgEp (Socket, PCU_INSTANCE_0, SmbCmdReg, SmbTsodPollRate.Data);
}

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
  )
{
  PSYSHOST                          Host;
  UINT8                             McChannel;
  UINT8                             SocketChannel;
  UINT8                             Dimm;
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  struct dimmNvram                  (*DimmNvList)[MAX_DIMM];
  SMB_TSOD_CONFIG_SMB_SPD0_STRUCT   SmbCntl;
  UINT8                             EnablePollingMask = 0;
  UINT8                             UpdateDimmMask = 0;
  SMB_DEVICE_STRUCT                 SmbDev;
  EFI_STATUS                        Status;
  UINT32                            Address;
  UINT8                             NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (McChannel = 0; McChannel < NumChPerMc; McChannel++) {

    SocketChannel = McChannel + (McId * NumChPerMc);
    DimmNvList = GetDimmNvList (Host, Socket, SocketChannel);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      Status = GetSmbAddress (Socket, SocketChannel, Dimm, &SmbDev);
      if (EFI_ERROR (Status)) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, SocketChannel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Error retrieving DIMM SMBus address while updating TSOD population\n");
        continue;
      }

      UpdateDimmMask |= (1 << SmbDev.address.strapAddress);

      if (((*DimmNvList)[Dimm].dimmPresent == 0)) {
        continue;
      }

      //
      // We want to enable TSOD polling for all DIMMs on enabled channels,
      // and only DDRT DIMMs on disabled channels
      //
      if (((*ChannelNvList)[SocketChannel].enabled == 0) && ((*DimmNvList)[Dimm].DcpmmPresent == 0)) {
        continue;
      }

      EnablePollingMask |= (1 << SmbDev.address.strapAddress);
    } // Dimm loop
  } // McChannel loop

  switch (GetProcSmbInstanceFromMcId (McId)) {
  case 0:
    Address = SMB_TSOD_CONFIG_SMB_SPD0_REG;
    break;
  case 1:
    Address = SMB_TSOD_CONFIG_SMB_SPD1_REG;
    break;
  default:
    RcDebugPrint (SDBG_ERROR, "Error retrieving McId SMBus Instance for McId 0x%x while updating TSOD population\n");
    return;
  }

  SmbCntl.Data = UsraCsrRead (Socket, PCU_INSTANCE_0, Address);
  SmbCntl.Bits.group0_dimm_present &= ~UpdateDimmMask;
  SmbCntl.Bits.group0_dimm_present |= EnablePollingMask;
  UsraCsrWrite (Socket, PCU_INSTANCE_0, Address, SmbCntl.Data);

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
  if ((RegOffset == SMB_CMD_SMB_SPD0_REG) || (RegOffset == SMB_CMD_SMB_SPD1_REG)) {
    return 1;
  } else {
    return 0;
  }
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
  if ((GetUbiosOutputMode () == ASM_OUTPUT_ENABLE_ON_SMBUS) ||
    ((GetUbiosOutputMode () == ASM_OUTPUT_ENABLE)
    && !((RegOffset == SMB_CMD_SMB_SPD0_REG) ||
    (RegOffset == SMB_CMD_SMB_SPD1_REG) ||
    (RegOffset == SMB_STATUS_SMB_SPD0_REG) ||
    (RegOffset == SMB_STATUS_SMB_SPD1_REG) ||
    (RegOffset == SMB_DATA_SMB_SPD0_REG) ||
    (RegOffset == SMB_DATA_SMB_SPD1_REG))
  )){
    return 1;
  } else {
    return 0;
  }
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
  UINT8     *RegAddr = 0;
  PSYSHOST  Host;

  Host = GetSysHostPointer ();

  if (SmbInstance == 0) {
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (Socket, 0, SMB_DATA_SMB_SPD0_REG);
  } else {
    RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (Socket, 0, SMB_DATA_SMB_SPD1_REG);
  }

  return (UINT32) (UINTN) RegAddr;
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
  @param[in] MessagChannMasteringEnBit   - Value 1 enables sending slot status messages to IIO and error messages to UBOX.
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
  VPPCTL_SMB_VPP_STRUCT    VppControl;
  VppControl.Data = UsraCsrRead (Socket, 0, VPPCTL_SMB_VPP_REG);
  VppControl.Bits.vpp_reset_mode = ResetModeBit;
  VppControl.Bits.vpp_error_severity = ErrorSeverityBit;
  VppControl.Bits.vpp_mc_enable = MessagChannMasteringEnBit;
  UsraCsrWrite (Socket, 0, VPPCTL_SMB_VPP_REG, VppControl.Data);
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
  // Unused in PEI
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
  // Unused in PEI
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
  // Unused in PEI
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
  // Unused in PEI
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
  // Unused in PEI
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
  // Unused in PEI
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
  // Unused in PEI
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
  // Unused in PEI
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
  // Unused in PEI
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
  // Unused in PEI
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
  // Unused in PEI
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
  // Unused in PEI
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
