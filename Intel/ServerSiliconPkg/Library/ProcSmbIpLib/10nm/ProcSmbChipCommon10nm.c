/** @file
  The Proc SMB Chip Common file provides common functions for the PEI, DXE, and
  SMM phases of the implmentation of ProcSmbIpLib for 10nm.

  This common file provides functions which are universal between PEI, DXE, and
  SMM.  The implementations for disabling/restoring TSOD and are common between
  all phases.  The read/write SMBus routines are common between DXE and SMM.
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
#include <Library/SysHostPointerLib.h>
#include <SysHost.h>
#include <Cpu/CpuIds.h>
#include <UncoreCommonIncludes.h>
#include <Guid/PlatformInfo.h>
#include <Library/HobLib.h>
#include "Common/SpdPlatformData.h"
#include <Library/MemRcLib.h>

#define SMB_TIMEOUT       100000   // 100 ms

/**
  Get slave device type - i2cni3c

  @param[in] Socket     - Socket number
  @param[in] Instance   - smbus Instance number

  @rval SlaveType   - slave device type

**/
UINT8
GetSlaveType (
  IN UINT8  Socket,
  IN UINT8  Instance
  )
{
  return I2C_DEVICE;
}

/**
  I3cSwitchToI3cMode.

  @param[in] Socket        - Socket Id

  @rval EFI_UNSUPPORTED  Function not supported
**/
EFI_STATUS
EFIAPI
I3cSwitchToI3cMode (
  IN  UINT8    Socket
  )
{
  return EFI_UNSUPPORTED;
}

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
  )
{
  return EFI_UNSUPPORTED;
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
  return 0; // SPR only
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
  return; // SPR only
}

/**
  Disable TSOD polling and store if TSOD was enabled/disabled.

  @param[in] SmbInstance           - SMBUS Instance
  @param[in] Socket                - Socket ID
  @param[in, out] SmbSavedCmdData  - Pointer to address to store Cmd data

  @retval VOID
**/
VOID
DisableTsod (
  IN UINT16      SmbInstance,
  IN UINT8       Socket,
  IN OUT UINT32  *SmbSavedCmdData
  )
{
  UINT32                   SmbCmdReg;
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;

  // Select appropriate command register based on Instance
  if (SmbInstance == 0) {
    SmbCmdReg   = SMB_CMD_SMB_SPD0_REG;
  } else {
    SmbCmdReg   = SMB_CMD_SMB_SPD1_REG;
  }

  // Save register state and disable TSOD polling
  *SmbSavedCmdData = UsraCsrRead (Socket, 0, SmbCmdReg);
  SmbCmd.Data = *SmbSavedCmdData;
  if (SmbCmd.Bits.smb_tsod_poll_en == 1) {
    SmbCmd.Bits.smb_tsod_poll_en = 0;
    UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);
  }
}

/**
  Restore TSOD polling based on previously saved setting.

  @param[in] SmbInstance      - SMBUS Instance
  @param[in] Socket           - Socket ID
  @param[in] SmbSavedCmdData  - Pointer to address to store Cmd data

  @retval VOID
**/
VOID
RestoreTsod (
  IN UINT16  SmbInstance,
  IN UINT8   Socket,
  IN UINT32  *SmbSavedCmdData
  )
{
  UINT32                   SmbCmdReg;
  SMB_CMD_SMB_SPD0_STRUCT  SmbCmd;

  if (SmbInstance == 0) {
    SmbCmdReg   = SMB_CMD_SMB_SPD0_REG;
  } else {
    SmbCmdReg   = SMB_CMD_SMB_SPD1_REG;
  }

  // Only restore if TSOD was previously enabled
  SmbCmd.Data = *SmbSavedCmdData;
  if (SmbCmd.Bits.smb_tsod_poll_en == 1) {
    // Restore TSOD setting
    UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);
  }
}

/**
  Reads from SMBUS at a given device and offset.

  @param[in] Socket      - Processor socket ID
  @param[in] Dev         - SMBUS device object
  @param[in] ByteOffset  - Byte offset within device
  @param[out] Data       - Pointer to buffer to store read value

  @retval EFI_SUCCESS       Read completed successfully
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
  UINT32                            SmbCmdReg;
  UINT32                            SmbStatReg;
  UINT32                            SmbDataReg;
  UINT32                            SmbSavedCmdData;
  SMB_STATUS_SMB_SPD0_STRUCT        SmbStat;
  volatile SMB_DATA_SMB_SPD0_STRUCT SmbData;
  SMB_CMD_SMB_SPD0_STRUCT           SmbCmd;
  EFI_STATUS                        Status;
  UINT32                            Timeout;

  SmbSavedCmdData = 0;

  //
  // Ensure TSOD polling is disabled before continuing
  //
  DisableTsod (Dev.address.SmbInstance, Socket, &SmbSavedCmdData);

  Status = EFI_SUCCESS;

  if (Dev.address.SmbInstance == 0) {  // 2 SMBus controllers - select which one
    SmbCmdReg = SMB_CMD_SMB_SPD0_REG;
    SmbStatReg = SMB_STATUS_SMB_SPD0_REG;
    SmbDataReg = SMB_DATA_SMB_SPD0_REG;
  } else {
    SmbCmdReg = SMB_CMD_SMB_SPD1_REG;
    SmbStatReg = SMB_STATUS_SMB_SPD1_REG;
    SmbDataReg = SMB_DATA_SMB_SPD1_REG;
  }

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

  Timeout = SMB_TIMEOUT;

  //
  // Wait for Host not busy
  //
  do {
    SmbStat.Data = UsraCsrRead (Socket, 0, SmbStatReg);
    if (!SmbStat.Bits.smb_busy) {
      break;
    }
    if (GetEmulation() & SIMICS_FLAG) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

  // Wait for timeout
  } while (Timeout > 0);

  //
  // Send command
  //
  UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);

  //
  // Wait for Host not busy
  //
  Timeout = SMB_TIMEOUT;

  do {
    SmbStat.Data = UsraCsrRead (Socket, 0, SmbStatReg);
    if (!SmbStat.Bits.smb_busy) {
      break;
    }
    if (GetEmulation() & SIMICS_FLAG) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;

    // Wait for timeout
  } while (Timeout > 0);

  //
  // Wait for the data
  //

  Timeout = SMB_TIMEOUT;

  while (!SmbStat.Bits.smb_rdo && !SmbStat.Bits.smb_sbe) {
    SmbStat.Data = UsraCsrRead (Socket, 0, SmbStatReg);
    MicroSecondDelay (1);
    Timeout--;
    if (Timeout <= 0){
      break;
    }
  }

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

  // Restore TSOD polling setting
  RestoreTsod (Dev.address.SmbInstance, Socket, &SmbSavedCmdData);

  return Status;
}

/**
  Performs SMBUS write operations at a given device and offset.

  @param[in] Socket      - Processor socket ID
  @param[in] Dev         - SMBUS device object
  @param[in] ByteOffset  - Byte offset within device
  @param[in] Data        - Pointer to data to write

  @retval EFI_SUCCESS       - Write completed successfully
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
  EFI_STATUS                  Status;
  UINT32                      SmbCmdReg;
  UINT32                      SmbStatReg;
  UINT32                      SmbDataReg;
  UINT32                      SmbSavedCmdData;
  SMB_STATUS_SMB_SPD0_STRUCT  SmbStat;
  SMB_CMD_SMB_SPD0_STRUCT     SmbCmd;
  SMB_DATA_SMB_SPD0_STRUCT    SmbData;
  UINT32                      Timeout;

  Status = EFI_SUCCESS;

  // Ensure TSOD polling is disabled before continuing
  DisableTsod (Dev.address.SmbInstance, Socket, &SmbSavedCmdData);

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

  //
  // Wait for Host not busy
  //
  Timeout = SMB_TIMEOUT;
  do {
    SmbStat.Data = UsraCsrRead (Socket, 0, SmbStatReg);
    if (!SmbStat.Bits.smb_busy) {
      break;
    }
    if (GetEmulation() & SIMICS_FLAG) {
      break;
    }

    MicroSecondDelay (1);
    Timeout--;
    // Wait for timeout
  } while (Timeout > 0);

  //
  // Send command
  //
  UsraCsrWrite (Socket, 0, SmbDataReg, SmbData.Data);
  UsraCsrWrite (Socket, 0, SmbCmdReg, SmbCmd.Data);

  //
  // Wait for the write to complete
  //
  Timeout = SMB_TIMEOUT;

  do {
    SmbStat.Data = UsraCsrRead (Socket, 0, SmbStatReg);

    MicroSecondDelay (1);
    Timeout--;

    if (Timeout <= 0) {
      Status = EFI_DEVICE_ERROR;
      break;
    }

  } while (!SmbStat.Bits.smb_wod && !SmbStat.Bits.smb_sbe);

  // If SMB Status indicates write completed with no error, return success
  if ((SmbStat.Bits.smb_wod == 1) && (SmbStat.Bits.smb_sbe == 0)) {
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_DEVICE_ERROR;
  }

  // Restore TSOD polling setting
  RestoreTsod (Dev.address.SmbInstance, Socket, &SmbSavedCmdData);

  return Status;
}

/**
  Returns Smbus Instance for given McId, by mapping Instance to
  McId

  @param[in] McId  - Memory Controller Id, 0 based

  @retval UINT8  Instance Number
**/
UINT8
GetProcSmbInstanceFromMcIdCommon (
  IN UINT8 McId
  )
{
  UINT8   MaxImc = GetMaxImc ();


  //Map McId to Smbus Instance
  if (IsCpuAndRevision(CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    return (McId / MaxImc);
  } else {
    return ((McId * 2) / MaxImc);
  }
}

/**
  Retrieves the interposer type from the PlatformInfo HOB and returns the SMBus strap address
  of the DIMM slot for interposers

  @param[in]  Socket   Socket number
  @param[in]  Ch       Channel number
  @param[in]  Dimm     Dimm number
  @param[out] Dev      Pointer to smbDevice

  @retval EFI_SUCCESS             Address successfully calculated
  @retval EFI_UNSUPPORTED         The given socket, channel, dimm, or Board ID and Revision ID combination is not supported
  @retval EFI_INVALID_PARAMETER   The given socket, channel or dimm number invalid
**/
EFI_STATUS
EFIAPI
GetInterposerSmbStrapAddress (
  IN  UINT8             Socket,
  IN  UINT8             Channel,
  IN  UINT8             Dimm,
  OUT SMB_DEVICE_STRUCT *Dev
  )
{
  INTERPOSER_TYPE               InterposerType = 0;
  UINT32                        RevisionId     = 0;
  EFI_PLATFORM_TYPE             PlatformType   = TypePlatformDefault;

  //
  // Lookup tables to remap memory channel SMBus addresses on
  // PPO Interposers.
  //
  // There are two interposer boards, A and B, and two versions of each,
  // which all have different routing.
  //
  UINT8 InterposerAMap0BusAddressMap[4][2] = {
    {2, 3},
    {0, 1},
    {2, 3},
    {0, 1}
    };

  UINT8 InterposerBMap0BusAddressMap[4][2] = {
    {4, 5},
    {6, 7},
    {4, 5},
    {6, 7}
    };

  GetPlatformData (Socket, &RevisionId, &PlatformType, &InterposerType);


  //
  // Platform has different channels unconnected per InterposerType then other platforms
  // We must correctly disable each channel per board/interposer type
  //

  if (Channel >= MAX_CH) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Use interposer type, platform type, and revision ID to get SMBus strap address from lookup tables
  //
  if (PlatformType == TypeArcherCityRP) {
    if (Channel >= 4) {
      return EFI_INVALID_PARAMETER;
    }
    if (InterposerType == InterposerA) {
      Dev->address.strapAddress = InterposerAMap0BusAddressMap[Channel][Dimm];
    } else {
      Dev->address.strapAddress = InterposerBMap0BusAddressMap[Channel][Dimm];
    }
  } else {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  Returns the SMBus strap address of the DIMM slot.

  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number
  @param[in]  Dimm    - Dimm number
  @param[out] Dev     - Pointer to smbDevice

  @retval EFI_SUCCESS  Successfully retrieved SMBus address data
**/
EFI_STATUS
GetSmbAddressCommon (
  IN UINT8               Socket,
  IN UINT8               Ch,
  IN UINT8               Dimm,
  OUT SMB_DEVICE_STRUCT  *Dev
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  INTERPOSER_TYPE   InterposerType;
  UINT32            RevisionId;
  EFI_PLATFORM_TYPE PlatformType;
  UINT8             NumChPerMc;
  UINT8             MaxImc;

  NumChPerMc   = GetNumChannelPerMc ();
  MaxImc       = GetMaxImc ();
  PlatformType = TypePlatformDefault;

  GetPlatformData (Socket, &RevisionId, &PlatformType, &InterposerType);

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
  // SPD addressing for BRS board
  //  ICX       DDR5  BRS Board
  // Mc,Ch(s)   CH   Silk-screen   SmbInstance StrapAddress
  // MC0,0-1    CH0     CHB         0          0/1
  // MC1,2-3    CH1     CHA         0          2/3
  // MC2,4-5    CH2     CHD         1          4/5
  // MC3,6-7    CH3     CHC         1          6/7
  //
  Dev->mcId = Ch / MAX_MC_CH;

  if (IsBrsPresent (NULL, 0)) {
    Ch = Ch >> 1;
  }

  // For 10nm each socket has its own SMBUS controller
  Dev->address.SmbSocket = Socket;

  //Calculate SMBUS Instance from Memory Controller Id
  Dev->address.SmbInstance = GetProcSmbInstanceFromMcIdCommon (Dev->mcId);

  if (MaxImc == 1) {
    Dev->address.strapAddress = Dimm + ((Ch % (NumChPerMc * MaxImc)) * MAX_DIMM);
  } else {
    //
    // 2 and 4 IMC
    //
    Dev->address.strapAddress = Dimm + ((Ch % (NumChPerMc * MaxImc / 2)) * MAX_DIMM);
  }

  if (IsBrsPresent (NULL, 0)) {
    //
    // BRS = ICX + DDR5_SUPPORT
    // Channel loop should follow the DDR5 channel architecture A->D OR 0->3
    // As BRS uses an ICX derivative, Instance should match ICX based on McId
    //
    if (Ch >= 4) {
      //
      // BRS only support MAX 4 channels.
      // We cannot use the MAX_CH define here, because
      // internally that matches the ICX architecture
      //
      return EFI_INVALID_PARAMETER;
    }
    Dev->mcId                 = Ch;
    Dev->address.SmbInstance   = Ch / 2;
    Dev->address.strapAddress = (Ch * 2) + Dimm;
    switch (PlatformType) {
      case TypeArcherCityRP:
        Status = GetInterposerSmbStrapAddress (Socket, Ch, Dimm, Dev);
        break;
      default:
        break;
    }
  }

  return Status;
}

