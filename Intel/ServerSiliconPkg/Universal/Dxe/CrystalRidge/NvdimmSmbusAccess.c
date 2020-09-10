/** @file NvdimmSmbusAccess.c
  Implementation of NVDIMM SMBus interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#include "CrystalRidge.h"
#include "NvdimmSmbusAccess.h"
#include <Library/ProcSmbIpLib.h>
#include <FnvAccessCommon.h>
#include <CpuPciAccess.h>
#include <MemFmcRegs.h>
#include <Library/IoAccessLib.h>
#include <Memory/I3cDefinitions.h>

#ifdef DDR5_SUPPORT
#define DPA_MAPPED_0  0x00
#define DA_UNIT_0     0x01
#define D_UNIT_0      0x02
#define DS_UNIT_0     0x03
#define A_UNIT_0      0x04
#define P_UNIT_0      0x05
#define P_UNIT_1      0x06
#define DFX_MISC_0    0x07
#define SEC_0         0x08
#define PW_UNIT_0     0x09
#define DDRPHY_0      0x0A
#define DDRPHY_1      0x0b
#define DDRPHY_2      0x0c
#define DDRPHY_3      0x0d
#endif

#ifdef DDR5_SUPPORT
//
// Command or data length of DWORD read requests.
//
#define SMB_READ_DWORD    4  // 1 byte PortID + 1 byte Attribute + 2 bytes Address or 4 bytes read data.
//
// Command + data length of DWORD/WORD/BYTE write requests.
//
#define SMB_WRITE_DWORD   8  // 1 byte PortID + 1 byte Attribute + 2 bytes Address + 4 bytes write data.
#define SMB_WRITE_WORD    6  // 1 byte PortID + 1 byte Attribute + 2 bytes Address + 2 bytes write data.
#define SMB_WRITE_BYTE    5  // 1 byte PortID + 1 byte Attribute + 2 bytes Address + 1 bytes write data.
#endif

#ifdef DDR5_SUPPORT
#define SMB_REG_ADDR(Device,Function,Offset) \
  (((Offset) & 0xffff) | (((Function) & 0x07) << 29) | (((Device) & 0x1F) << 24))
#else
#define SMB_REG_ADDR(Device, Function, Offset) \
  (((Offset) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1) << 15))
#endif
#define SMB_REG_READ_ADDR_MASK 0xFFFFFFFC

#define NVDIMM_SMBUS_ACCESS_SWITCH_TO_SMM_SW_SMI  0xFE
#define NVDIMM_SMBUS_ACCESS_MS_TO_US(DelayMs)     ((DelayMs) * 1000)

#define SMB_BSR_H_SHIFT 32

NVDIMM_SMBUS_ACCESS_DATA mSmbusAccessData[MAX_SOCKET][MAX_IMC];

/**

  Verifies if given opcode is valid and returns
  index in the opcode lookup table

  @param[in]  Opcode      - NVMCTLR command (opcode)
  @param[out] OpcodeIndex - NVMCTLR command (opcode) index in the command lookup table

  @return Status              - Status of the operation

**/
EFI_STATUS
NvdimmSmbusAccessValidateOpcode (
  IN  UINT32 Opcode,
  OUT UINT8  *OpcodeIndex
  )
{
  if (NULL == OpcodeIndex) {
    return EFI_INVALID_PARAMETER;
  }

  for (*OpcodeIndex = 0; *OpcodeIndex < FNV_OPCODE_ARRAY_SIZE; (*OpcodeIndex)++){
    if (Opcode == FnvPayloadLookupTable [*OpcodeIndex][OPCODE_INDEX]) {
      break;
    }
  }

  if ((*OpcodeIndex < FNV_OPCODE_ARRAY_SIZE) && (FnvPayloadLookupTable[*OpcodeIndex][OPCODE_INDEX] != NULL_COMMAND)) {
    return EFI_SUCCESS;
  }

  *OpcodeIndex = 0;
  return EFI_NOT_FOUND;
}

/**

  Calculates execution time

  @param[in] StartTime  - NVDIMM SMBus access start time
  @param[in] EndTime    - NVDIMM SMBus access end time

  @return Execution time

**/
UINT64
NvdimmSmbusAccessExecutionTime (
  IN UINT64 StartTime,
  IN UINT64 EndTime
  )
{
  UINT64                              CounterStart = 0;
  UINT64                              CounterEnd = 0;
  INT64                               WaitTime;
  INT64                               CounterCycle;

  GetPerformanceCounterProperties (&CounterStart, &CounterEnd);

  CounterCycle = CounterEnd - CounterStart;
  if (CounterCycle < 0) {
    CounterCycle = -CounterCycle;
  }
  CounterCycle++;

  WaitTime = EndTime - StartTime;
  if (CounterStart > CounterEnd) {
    WaitTime = -WaitTime;
  }

  if (WaitTime < 0) {
    WaitTime += CounterCycle;
  }

  return (GetTimeInNanoSecond (WaitTime) / 1000);
}

/**

  Calculates FW Mailbox SMBus register address

  @param[in]  RegOffset   - SMBus register
  @param[in]  Reg         - SMBus register address

  @return Status      - Status of the operation

**/
EFI_STATUS
NvdimmSmbusAccessGetMailboxRegisterAddress (
  IN  CSR_OFFSET  RegOffset,
  OUT UINT32      *Reg
  )
{
  if (NULL == Reg) {
    return EFI_INVALID_PARAMETER;
  }

#ifdef DDR5_SUPPORT
  if ((RegOffset.Bits.boxtype != 0x1B) || (RegOffset.Bits.funcblk > 0x0E)) {
    return EFI_INVALID_PARAMETER;
  }

  switch(RegOffset.Bits.funcblk) {
    case DPA_MAPPED_0:
      //
      // B0:D0:F6
      //
      *Reg = SMB_REG_ADDR (0, 6, RegOffset.Bits.offset);
      break;

    case DA_UNIT_0:
      //
      // B0:D1:F6
      //
      *Reg = SMB_REG_ADDR (1, 6, RegOffset.Bits.offset);
      break;

    case D_UNIT_0:
      //
      // B0:D4:F6
      //
      *Reg = SMB_REG_ADDR (4, 6, RegOffset.Bits.offset);
      break;

    case DS_UNIT_0:
      //
      // B0:D5:F6
      //
      *Reg = SMB_REG_ADDR (5, 6, RegOffset.Bits.offset);
      break;

    case A_UNIT_0:
      //
      // B0:D6:F6
      //
      *Reg = SMB_REG_ADDR (6, 6, RegOffset.Bits.offset);
      break;

    case P_UNIT_0:
      //
      // B0:D16:F6
      //
      *Reg = SMB_REG_ADDR (16, 6, RegOffset.Bits.offset);
      break;

    case P_UNIT_1:
      //
      // B0:D18:F6
      //
      *Reg = SMB_REG_ADDR (18, 6, RegOffset.Bits.offset);
      break;

    case DFX_MISC_0:
      //
      // B0:D24:F6
      //
      *Reg = SMB_REG_ADDR (24, 6, RegOffset.Bits.offset);
      break;

    case SEC_0:
      //
      // B0:D17:F4
      //
      *Reg = SMB_REG_ADDR (17, 4, RegOffset.Bits.offset);
      break;

    case PW_UNIT_0:
      //
      // B0:D17:F6
      //
      *Reg = SMB_REG_ADDR (17, 6, RegOffset.Bits.offset);
      break;

    case DDRPHY_0:
      //
      // B8:D0:F7
      //
      *Reg = SMB_REG_ADDR (0, 7, RegOffset.Bits.offset);
      break;

    case DDRPHY_1:
      //
      // B8:D1:F7, sub opcode = 6
      //
      *Reg = (SMB_REG_ADDR (1, 7, RegOffset.Bits.offset) | ((6 & 0x7) << 20));
      break;

    case DDRPHY_2:
      //
      // B8:D2:F7
      //
      *Reg = SMB_REG_ADDR (2, 7, RegOffset.Bits.offset);
      break;

    case DDRPHY_3:
      //
      // B8:D3:F7
      //
      *Reg = SMB_REG_ADDR (3, 7, RegOffset.Bits.offset);
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }
#else
  *Reg = SMB_REG_ADDR ((RegOffset.Bits.funcblk >> 3), ((UINT8)RegOffset.Bits.funcblk & 7), RegOffset.Bits.offset);
#endif
  return EFI_SUCCESS;
}

#ifdef ICX_HOST
/**

  Process NVMCTLR read requests, sequence of commands required to process read requests

  @param[in]  Socket  - Socket Number
  @param[in]  SmbReg  - SMBus register contents
  @param[in]  SmbDev  - Pointer to SMBus device data structure
  @param[out] Data    - Pointer to return data

  @return Status      - Status of the operation

**/
EFI_STATUS
NvdimmSmbusAccessReadMailboxRegisterWorker (
  IN  UINT8             Socket,
  IN  UINT32            SmbReg,
  IN  SMB_DEVICE_STRUCT SmbDev,
  OUT volatile UINT32   *Data
  )
{
  EFI_STATUS      Status;
  volatile UINT8  *PByte;
  UINT8           DevFunc = 0;
  UINT8           ZeroData = 0;
  UINT8           Data8 = 0;

  if (NULL == Data) {
    return EFI_INVALID_PARAMETER;
  }
  PByte = (volatile UINT8*)Data;

  //
  // Write command code 0x90 with zero as data
  // ([7]Begin=1, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  Status = WriteProcSmb (SmbDev, 0x80, &ZeroData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Write command code 0x10 with DevFunc as data
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  DevFunc = (UINT8)((SmbReg >> 12) & 0xF);
  Status = WriteProcSmb (SmbDev, 0x00, &DevFunc);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Write command code 0x10 with Reg as data for Reg[15:8]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  Data8 = (UINT8)((SmbReg >> 8) & 0xF);
  Status = WriteProcSmb (SmbDev, 0x00, &Data8);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Write command code 0x50 with Reg as data for Reg[7:0]
  // ([7]Begin=0, [6]End=1, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  Data8 = (UINT8)(SmbReg & 0xFF);
  Status = WriteProcSmb (SmbDev, 0x40, &Data8);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Read command code 0x90
  // ([7]Begin=1, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  Status = ReadProcSmb (SmbDev, 0x80, &ZeroData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Read command code 0x10 with Data[31:24]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  Status = ReadProcSmb (SmbDev, 0x00, (PByte + 3));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Read command code 0x10 with Data[23:16]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  Status = ReadProcSmb (SmbDev, 0x00, (PByte + 2));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Read command code 0x10 with Data[15:8]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  Status = ReadProcSmb (SmbDev, 0x00, (PByte + 1));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Read command code 0x50 with Data[7:0]
  // ([7]Begin=0, [6]End=1, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  Status = ReadProcSmb (SmbDev, 0x40, PByte);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}
#else
/**

  Process NVMCTLR read requests, sequence of commands required to process read requests.

  @param[in]  Socket   Socket index.
  @param[in]  SmbReg   SMBus register address offset in NVMCTLR cfg space.
  @param[in]  SmbDev   SMBus device address.
  @param[out] Data     Pointer to the return data.

  @retval Status       Status of the operation.

**/
EFI_STATUS
NvdimmSmbusAccessReadMailboxRegisterWorker (
  IN  UINT8                 Socket,
  IN  UINT32                SmbReg,
  IN  SMB_DEVICE_STRUCT     SmbDev,
  OUT volatile UINT32       *Data
  )
{
  volatile UINT8  *Data8;
  UINT8           Mode = 1;
  EFI_STATUS      Status;
  UINT8           CmdDataLen;
  UINT8           CmdData[SMB_READ_DWORD];

  if ((Socket >= MAX_SOCKET) || (Data == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Data8 = (volatile UINT8 *) Data;
  ZeroMem (CmdData, sizeof (CmdData));

  CmdDataLen = SMB_READ_DWORD;

  CmdData[0] = (UINT8) ((SmbReg >> 24) & 0xFF);                  // SB PortId[7:0] = 32 * Function[2:0] + Device[4:0]
  CmdData[1] = (UINT8) (((SmbReg >> 16) & 0xFF) | (Mode << 3));  // SB Opcode | Mode = 1 (read request)
  CmdData[2] = (UINT8) (SmbReg & 0xFF);                          // Address[7:0]
  CmdData[3] = (UINT8) ((SmbReg >> 8) & 0xFF);                   // Address[15:8]

  //
  // Send command then read
  //
  Status = SmbSendCccCommand (Socket, SmbDev, NO_CCC, AttrNormalWriteReStartRead, (UINT8 *) &CmdData, &CmdDataLen);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Analyse the return data
  //
  *Data8       = CmdData[0];   // Byte 0
  *(Data8 + 1) = CmdData[1];   // Byte 1
  *(Data8 + 2) = CmdData[2];   // Byte 2
  *(Data8 + 3) = CmdData[3];   // Byte 3

  return EFI_SUCCESS;
}
#endif

/**

  This functions reads a FW mailbox register via SMBus

  @param[in]  Socket      - Socket Number
  @param[in]  Ch          - DDR Channel ID
  @param[in]  Dimm        - DIMM number
  @param[in]  Reg         - NVMCTLR register Address
  @param[out] Data        - Data from NVMCTLR register

  @return Status          - Status of the operation

**/
EFI_STATUS
NvdimmSmbusAccessReadMailboxRegister (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT32    Reg,
  OUT UINT32    *Data
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  CSR_OFFSET        RegOffset;
  SMB_DEVICE_STRUCT SmbDev;
  UINT32            SmbReg;
  UINT32            TimeoutCounter;
  volatile UINT32   LocalData = 0;
  UINT8             Index = 0;
  UINT8             RegSize = 0;

  if (NULL == Data) {
    return EFI_INVALID_PARAMETER;
  }

  RegOffset.Data = Reg;
  RegSize = (UINT8)RegOffset.Bits.size;
  Index = (UINT8)(RegOffset.Bits.offset & 3) << 3;

  Status = NvdimmSmbusAccessGetMailboxRegisterAddress (RegOffset, &SmbReg);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }
  SmbReg &=SMB_REG_READ_ADDR_MASK;

  SmbDev.compId = SPD;
  SmbDev.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  SmbDev.address.deviceType = DTI_FNV;

  Status = GetSmbAddress (Socket, Ch, Dimm, &SmbDev);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  TimeoutCounter = COMP_TIMEOUT / COMMON_DELAY_UNIT;

  while (TimeoutCounter) {
    Status = NvdimmSmbusAccessReadMailboxRegisterWorker (Socket, SmbReg, SmbDev, &LocalData);
    if (!EFI_ERROR (Status)) {
      if (RegSize == 4) {
        *Data = (LocalData >> Index);
      } else if (RegSize == 2) {
        *Data = ((LocalData >> Index) & 0x0FFFF);
      } else {
        *Data = ((LocalData >> Index) & 0x0FF);
      }
      break;
    }

    MicroSecondDelay (COMMON_DELAY_UNIT);
    TimeoutCounter--;
  }

  return (TimeoutCounter) ? EFI_SUCCESS : EFI_TIMEOUT;
}

#ifdef ICX_HOST
/**

  Process NVMCTLR write requests, sequence of commands required to process write requests

  @param[in] Socket     Socket number
  @param[in] SmbReg     smbus register contents
  @param[in] SmbDev     Pointer to smbDevice
  @param[in] Data       Pointer to the data to be written
  @param[in] RegSize    Size of the data to be written
  @param[in] CmdCode    CmdData code

  @retval Status        Status of the operation.

**/
EFI_STATUS
NvdimmSmbusAccessWriteMailboxRegisterWorker (
  IN UINT8             Socket,
  IN UINT32            SmbReg,
  IN SMB_DEVICE_STRUCT SmbDev,
  IN UINT32            *Data,
  IN UINT8             RegSize,
  IN UINT8             CmdCode
  )
{
  EFI_STATUS  Status;
  UINT8       *PByte = (UINT8 *)Data;
  UINT8       DevFunc = 0;
  UINT8       ZeroData = 0;
  UINT8       Data8 = 0;

  //
  // Write command code 0x9C with zeroes as data
  // ([7]Begin=1, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  Status = WriteProcSmb (SmbDev, (0x80 | CmdCode), &ZeroData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Write command code 0x1C with DevFunc as data
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  DevFunc = (UINT8)((SmbReg >> 12) & 0xF);
  Status = WriteProcSmb (SmbDev, (0x00 | CmdCode), &DevFunc);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Write command code 0x1C with Reg as data for Reg[15:8]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  Data8 = (UINT8)((SmbReg >> 8) & 0xF);
  Status = WriteProcSmb (SmbDev, (0x00 | CmdCode), &Data8);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Write command code 0x1C with Reg as data for Reg[7:0]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  Data8 = (UINT8)(SmbReg & 0xFF);
  Status = WriteProcSmb (SmbDev, (0x00 | CmdCode), &Data8);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Read command code 0x1C with Data[31:24]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  if (RegSize == 4) {
    Status = WriteProcSmb (SmbDev, (0x00 | CmdCode), (PByte + 3));
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //
  // Read command code 0x1C with Data[23:16]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  if (RegSize == 4) {
    Status = WriteProcSmb (SmbDev, (0x00 | CmdCode), (PByte + 2));
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //
  // Read command code 0x1C with Data[15:8]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  if ((RegSize == 4) || (RegSize == 2)) {
    Status = WriteProcSmb (SmbDev, (0x00 | CmdCode), (PByte + 1));
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //
  // Read command code 0x5C with Data[7:0]
  // ([7]Begin=0, [6]End=1, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  Status = WriteProcSmb (SmbDev, (0x40 | CmdCode), PByte);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}
#else
/**

  Process NVMCTLR write requests, sequence of commands required to process write requests

  @param[in] Socket     Socket index.
  @param[in] SmbReg     SMBus register address offset in NVMCTLR cfg space.
  @param[in] SmbDev     SMBus device address.
  @param[in] Data       Pointer to the data to be written.
  @param[in] RegSize    Size of the data to be written.
  @param[in] CmdCode    CmdData code (Unused for CWV).

  @retval Status        Status of the operation.

**/
EFI_STATUS
NvdimmSmbusAccessWriteMailboxRegisterWorker (
  IN UINT8              Socket,
  IN UINT32             SmbReg,
  IN SMB_DEVICE_STRUCT  SmbDev,
  IN UINT32             *Data,
  IN UINT8              RegSize,
  IN UINT8              CmdCode
  )
{
  EFI_STATUS  Status;
  UINT8       *Data8;
  UINT8       Mode = 0;
  UINT8       CmdDataLen;
  UINT8       CmdData[SMB_WRITE_DWORD];

  if ((Socket >= MAX_SOCKET) || (Data == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Data8 = (UINT8 *) Data;
  ZeroMem (CmdData, sizeof (CmdData));

  CmdData[0] = (UINT8) ((SmbReg >> 24) & 0xFF);                   // SB PortId[7:0] = 32 * Function[2:0] + Device[4:0]
  CmdData[1] = (UINT8) (((SmbReg >> 16) & 0xFF) | (Mode << 3));   // SB Opcode | Mode = 0 (write request)
  CmdData[2] = (UINT8) (SmbReg & 0xFF);                           // Address[7:0]
  CmdData[3] = (UINT8) ((SmbReg >> 8) & 0xFF);                    // Address[15:8]

  switch (RegSize) {
  case sizeof (UINT32):
    CmdDataLen = SMB_WRITE_DWORD;
    CmdData[4] = *Data8;           // Byte 0
    CmdData[5] = *(Data8 + 1);     // Byte 1
    CmdData[6] = *(Data8 + 2);     // Byte 2
    CmdData[7] = *(Data8 + 3);     // Byte 3
    break;

  case sizeof (UINT16):
    CmdDataLen = SMB_WRITE_WORD;
    CmdData[4] = *Data8;           // Byte 0
    CmdData[5] = *(Data8 + 1);     // Byte 1
    break;

  case sizeof (UINT8):
    CmdDataLen = SMB_WRITE_BYTE;
    CmdData[4] = *Data8;           // Byte 0
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }

  //
  // Send command and write data
  //
  Status = SmbSendCccCommand (Socket, SmbDev, NO_CCC, AttrNormalWrite, (UINT8 *) &CmdData, &CmdDataLen);

  return Status;
}
#endif

/**

  This function writes 32 bits of data into a FW mailbox register via SMBus

  @param[in] Socket      - Socket Number
  @param[in] Ch          - DDR Channel ID
  @param[in] Dimm        - DIMM number
  @param[in] Reg         - NVMCTLR register Address
  @param[in] Data        - Data to be written to NVMCTLR register

  @return Status         - Status of the operation

**/
EFI_STATUS
NvdimmSmbusAccessWriteMailboxRegister (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT32    Reg,
  IN  UINT32    Data
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  CSR_OFFSET        RegOffset;
  UINT8             RegSize = 0;
  UINT32            SmbReg;
  SMB_DEVICE_STRUCT SmbDev;
  UINT8             CmdCode;
  UINT32            TimeoutCounter;
  UINT32            LocalData = Data;

  RegOffset.Data = Reg;
  RegSize = (UINT8)RegOffset.Bits.size;

  Status = NvdimmSmbusAccessGetMailboxRegisterAddress (RegOffset, &SmbReg);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (RegSize == 4) {
    CmdCode = 0x0C;
  } else if (RegSize == 2) {
    CmdCode = 0x08;
  } else {
    CmdCode = 0x04;
  }

  SmbDev.compId = SPD;
  SmbDev.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  SmbDev.address.deviceType = DTI_FNV;

  Status = GetSmbAddress (Socket, Ch, Dimm, &SmbDev);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  TimeoutCounter = (COMP_TIMEOUT / COMMON_DELAY_UNIT);

  while (TimeoutCounter) {
    Status = NvdimmSmbusAccessWriteMailboxRegisterWorker (Socket, SmbReg, SmbDev, &LocalData, RegSize, CmdCode);
    if (!EFI_ERROR (Status)) {
      break;
    }

    MicroSecondDelay (COMMON_DELAY_UNIT);
    TimeoutCounter--;
  }

  return (TimeoutCounter) ? EFI_SUCCESS : EFI_TIMEOUT;
}

#ifdef ICX_HOST
/**
  Reads the FW Boot Status Registers.

  @param[in]  Socket          - Socket Number
  @param[in]  Ch              - DDR Channel id
  @param[in]  Dimm            - DIMM Number
  @param[out] Bsr             - Pointer to value of Boot Status Register

  @return Status              - Status of the operation
**/
EFI_STATUS
NvdimmSmbusAccessReadBootStatusRegisters (
  IN  UINT8   Socket,
  IN  UINT8   Ch,
  IN  UINT8   Dimm,
  OUT UINT64  *Bsr
  )
{
  EFI_STATUS  Status;
  UINT32      BsrL;
  UINT32      BsrH;
  UINT8       Imc;

  if (NULL == Bsr) {
    return EFI_INVALID_PARAMETER;
  }

  Imc = ConvertSktChToImc (Ch);
  if (mInSmm) {
    Status = NvdimmSmbusAccessAcquire (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access acquiring failed (status %r)\n", Socket, Ch, Dimm, Status));
      return EFI_DEVICE_ERROR;
    }
#ifdef DDR5_SUPPORT
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, D_FW_STATUS_CWV_D_UNIT_0_REG, &BsrL);
#else
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, D_FW_STATUS_FNV_D_UNIT_0_REG, &BsrL);
#endif
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }

#ifdef DDR5_SUPPORT
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, D_FW_STATUS_H_CWV_D_UNIT_0_REG, &BsrH);
#else
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, D_FW_STATUS_H_FNV_D_UNIT_0_REG, &BsrH);
#endif
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }

    *Bsr |= (((UINT64)BsrH << SMB_BSR_H_SHIFT) | ((UINT64)BsrL));

    Status = NvdimmSmbusAccessRelease (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("FIS") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      return EFI_DEVICE_ERROR;
    }
  } else {
    //
    // Update type/parameters/data in mNvdimmSmbusSmmInterfaceProtocol to access in SMM
    //
    if (mNvdimmSmbusSmmInterface == NULL) {
      //
      // NVDIMM SMBus DXE <-> SMM interface should already allocated at CR DXE driver entry
      //
      ASSERT (FALSE);
      return EFI_LOAD_ERROR;
    }

    ZeroMem (&mNvdimmSmbusSmmInterface->RequestParams, sizeof (mNvdimmSmbusSmmInterface->RequestParams));
    ZeroMem (&mNvdimmSmbusSmmInterface->RequestData, sizeof (mNvdimmSmbusSmmInterface->RequestData));

    mNvdimmSmbusSmmInterface->RequestParams.GetBsr.Socket = Socket;
    mNvdimmSmbusSmmInterface->RequestParams.GetBsr.Ch = Ch;
    mNvdimmSmbusSmmInterface->RequestParams.GetBsr.Dimm = Dimm;
    mNvdimmSmbusSmmInterface->RequestData.GetBsr.BootStatusRegister = Bsr;
    mNvdimmSmbusSmmInterface->RequestType = GET_BSR;
    //
    // Generating SW SMI
    //
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "S%d.C%d.D%d: Generating SW SMI for Boot Status Register retrieval\n", Socket, Ch, Dimm));
    IoWrite8 (0xB2, (UINT8)NVDIMM_SMBUS_ACCESS_SWITCH_TO_SMM_SW_SMI);

    //
    // SMI handler executed, retrieve operation status
    //
    if (EFI_ERROR (mNvdimmSmbusSmmInterface->RequestStatus)) {
      return EFI_DEVICE_ERROR;
    }
  }
  return EFI_SUCCESS;
}

/**

  NvdimmSmbusAccessMailboxInterface is a generic read/write to the BIOS/SMM mailbox

  @param[in]  Socket          - Socket Number
  @param[in]  Ch              - DDR Channel id
  @param[in]  Dimm            - DIMM Number
  @param[in]  InputPayload    - Input Payload Register
  @param[out] OutputPayload   - Output Payload Register
  @param[in]  Opcode          - NVMCTLR command (opcode)
  @param[in]  Timeout         - Timeout for executing command
  @param[out] *MbFisStatus    - Status of FIS command

  @return Status              - Status of the operation

**/
EFI_STATUS
NvdimmSmbusAccessMailboxInterface (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT32    InputPayload[NUM_PAYLOAD_REG],
  OUT UINT32    OutputPayload[NUM_PAYLOAD_REG],
  IN  UINT32    Opcode,
  IN  UINT64    Timeout,
  OUT UINT8     *MbFisStatus
  )
{
  EFI_STATUS                            Status;
  EFI_STATUS                            MbStatus;
#ifdef DDR5_SUPPORT
  MB_SMBUS_CMD_CWV_DS_UNIT_0_STRUCT     SmbusCommandRegister;
  MB_SMBUS_STATUS_CWV_DS_UNIT_0_STRUCT  SmbusStatusRegister;
#else
  MB_SMBUS_CMD_FNV_D_UNIT_0_STRUCT      SmbusCommandRegister;
  MB_SMBUS_STATUS_FNV_D_UNIT_0_STRUCT   SmbusStatusRegister;
#endif
  UINT64                                StartTime;
  UINT64                                EndTime;
  UINT64                                TimeoutCounter;
  UINT8                                 OpcodeIndex = 0;
  UINT8                                 i;
  UINT8                                 Imc;
  CHAR8                                 *MboxErrStrPtr = "Mailbox Failure: N/A";

  //
  // If users not pass any timeout use defaults
  //
  if (Timeout == 0) {
    Timeout = COMP_TIMEOUT;
  }

  //
  // Validate NVDIMM commend opcode
  //
  Status = NvdimmSmbusAccessValidateOpcode (Opcode, &OpcodeIndex);
  if (EFI_ERROR (Status)){
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "S%d.C%d.D%d: Invalid request opcode (0x%04X)\n", Socket, Ch, Dimm, Opcode));
    return EFI_INVALID_PARAMETER;
  }

  Imc = ConvertSktChToImc (Ch);
  if (mInSmm) {
    Status = NvdimmSmbusAccessAcquire (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access acquiring failed (status %r)\n", Socket, Ch, Dimm, Status));
      return EFI_DEVICE_ERROR;
    }

    //
    // Initialize for doorbell status retrieval timeout
    //
    TimeoutCounter = (DOORBELL_TIMEOUT / COMMON_DELAY_UNIT);

    //
    // Read command register to check doorbell status
    //
  #ifdef DDR5_SUPPORT
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, &SmbusCommandRegister.Data);
  #else
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, &SmbusCommandRegister.Data);
  #endif
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }

    //
    // Check/Wait for doorbell idle status
    //
    while (SmbusCommandRegister.Bits.doorbell) {
      if (!TimeoutCounter) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: Previous request 0x%04X on SMBus mailbox didn't complete\n",
          Socket, Ch, Dimm, SmbusCommandRegister.Bits.opcode));
        //
        // SMBus mailbox interface filed - try to release SMBus access
        //
        Status = NvdimmSmbusAccessRelease (Socket, Imc);
        if (EFI_ERROR (Status)) {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
        }
        return EFI_DEVICE_ERROR;
      }

      MicroSecondDelay (COMMON_DELAY_UNIT);
      TimeoutCounter--;

  #ifdef DDR5_SUPPORT
      Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, &SmbusCommandRegister.Data);
  #else
      Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, &SmbusCommandRegister.Data);
  #endif
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
        //
        // SMBus mailbox interface filed - try to release SMBus access
        //
        Status = NvdimmSmbusAccessRelease (Socket, Imc);
        if (EFI_ERROR (Status)) {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
        }
        return EFI_DEVICE_ERROR;
      }
    }

    //
    // Send Opcode to Mailbox
    //
    SmbusCommandRegister.Bits.opcode = Opcode;
  #ifdef DDR5_SUPPORT
    Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, SmbusCommandRegister.Data);
  #else
    Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, SmbusCommandRegister.Data);
  #endif
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }

    //
    // Fill Input Payload Registers
    //
    for (i = 0; i < FnvPayloadLookupTable [OpcodeIndex][INPUT_PAYLOAD_INDEX]; i++){
  #ifdef DDR5_SUPPORT
      Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, (MB_SMBUS_INPUT_PAYLOAD_0_CWV_DS_UNIT_0_REG + (i * 4)), InputPayload[i]);
  #else
      Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, (MB_SMBUS_INPUT_PAYLOAD_0_FNV_D_UNIT_0_REG + (i * 4)), InputPayload[i]);
  #endif
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
        //
        // SMBus mailbox interface filed - try to release SMBus access
        //
        Status = NvdimmSmbusAccessRelease (Socket, Imc);
        if (EFI_ERROR (Status)) {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
        }
        return EFI_DEVICE_ERROR;
      }
    }

    //
    // Set doorbell to busy status
    //
    SmbusCommandRegister.Bits.doorbell = BUSY;
  #ifdef DDR5_SUPPORT
    Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, SmbusCommandRegister.Data);
  #else
    Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, SmbusCommandRegister.Data);
  #endif
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }

    //
    // Start Performance counter
    //
    StartTime = GetPerformanceCounter ();

    //
    // Initialize for doorbell status retrieval timeout
    //
    TimeoutCounter = (Timeout / COMMON_DELAY_UNIT);

    //
    // Read command register to check doorbell status
    //
  #ifdef DDR5_SUPPORT
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, &SmbusCommandRegister.Data);
  #else
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, &SmbusCommandRegister.Data);
  #endif
    if (EFI_ERROR (Status)) {
     CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
     //
     // SMBus mailbox interface filed - try to release SMBus access
     //
     Status = NvdimmSmbusAccessRelease (Socket, Imc);
     if (EFI_ERROR (Status)) {
       CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
     }
     return EFI_DEVICE_ERROR;
    }

    //
    // Check/Wait for doorbell idle status
    //
    while (SmbusCommandRegister.Bits.doorbell) {
      if (!TimeoutCounter) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: Request 0x%04X on SMBus mailbox failed after %lldus (timeout %lldus)\n",
          Socket, Ch, Dimm, Opcode, NvdimmSmbusAccessExecutionTime (StartTime, GetPerformanceCounter ()), Timeout));
        //
        // SMBus mailbox interface filed - try to release SMBus access
        //
        Status = NvdimmSmbusAccessRelease (Socket, Imc);
        if (EFI_ERROR (Status)) {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
        }
        return EFI_DEVICE_ERROR;
      }

      MicroSecondDelay (COMMON_DELAY_UNIT);
      TimeoutCounter--;

  #ifdef DDR5_SUPPORT
      Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, &SmbusCommandRegister.Data);
  #else
      Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, &SmbusCommandRegister.Data);
  #endif
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
        //
        // SMBus mailbox interface filed - try to release SMBus access
        //
        Status = NvdimmSmbusAccessRelease (Socket, Imc);
        if (EFI_ERROR (Status)) {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
        }
        return EFI_DEVICE_ERROR;
      }
    }

    //
    // Stop Performance counter
    //
    EndTime = GetPerformanceCounter ();

    //
    // Read completion and status
    //
  #ifdef DDR5_SUPPORT
      Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_STATUS_CWV_DS_UNIT_0_REG, &SmbusStatusRegister.Data);
  #else
      Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_STATUS_FNV_D_UNIT_0_REG, &SmbusStatusRegister.Data);
  #endif
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }

    //
    // Assign MB status
    //
    if (NULL != MbFisStatus) {
      *MbFisStatus = (UINT8)SmbusStatusRegister.Bits.stat;
    }

    //
    // Check completion bit
    //
    if (!SmbusStatusRegister.Bits.comp) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: Request 0x%04X on SMBus mailbox - completion bit not set\n", Socket, Ch, Dimm, Opcode));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }

    //
    // Check for MB Errors and convert to status accordingly
    //
    MbStatus = MailboxStatusDecode ((UINT8)SmbusStatusRegister.Bits.stat, (UINT16)Opcode, InputPayload, &MboxErrStrPtr);
    if (EFI_ERROR (MbStatus)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: Request 0x%04X on SMBus mailbox failed - status %r (%a)\n",
        Socket, Ch, Dimm, Opcode, MbStatus, MboxErrStrPtr));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
        return EFI_DEVICE_ERROR;
      }
      //
      // Return status from MB Decoder
      //
      return MbStatus;
    }

    //
    // Read Output Payload Registers
    //
    for (i = 0; i < NUM_PAYLOAD_REG; i++){
  #ifdef DDR5_SUPPORT
      Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, (MB_SMBUS_OUTPUT_PAYLOAD_0_CWV_DS_UNIT_0_REG + (i * 4)), &OutputPayload[i]);
  #else
      Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, (MB_SMBUS_OUTPUT_PAYLOAD_0_FNV_D_UNIT_0_REG + (i * 4)), &OutputPayload[i]);
  #endif
      if (EFI_ERROR (Status)) {
        //
        // SMBus mailbox interface filed - try to release SMBus access
        //
        Status = NvdimmSmbusAccessRelease (Socket, Imc);
        if (EFI_ERROR (Status)) {
          CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
        }
        return EFI_DEVICE_ERROR;
      }
    }

    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "S%d.C%d.D%d: Request 0x%04X on SMBus mailbox completed in %lldus (timeout %lldus)\n",
      Socket, Ch, Dimm, Opcode, NvdimmSmbusAccessExecutionTime (StartTime, EndTime), Timeout));

    Status = NvdimmSmbusAccessRelease (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("FIS") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      return EFI_DEVICE_ERROR;
    }
  } else {
    //
    // Update type/parameters/data in mNvdimmSmbusSmmInterfaceProtocol to access in SMM
    //
    if (mNvdimmSmbusSmmInterface == NULL) {
      //
      // NVDIMM SMBus DXE <-> SMM interface should already allocated at CR DXE driver entry
      //
      ASSERT (FALSE);
      return EFI_LOAD_ERROR;
    }

    ZeroMem (&mNvdimmSmbusSmmInterface->RequestParams, sizeof (mNvdimmSmbusSmmInterface->RequestParams));
    ZeroMem (&mNvdimmSmbusSmmInterface->RequestData, sizeof (mNvdimmSmbusSmmInterface->RequestData));

    mNvdimmSmbusSmmInterface->RequestParams.FisRequest.Socket = Socket;
    mNvdimmSmbusSmmInterface->RequestParams.FisRequest.Ch = Ch;
    mNvdimmSmbusSmmInterface->RequestParams.FisRequest.Dimm = Dimm;
    mNvdimmSmbusSmmInterface->RequestType = FIS_REQUEST;
    CopyMem (mNvdimmSmbusSmmInterface->RequestData.FisRequest.InputPayload, InputPayload, (NUM_PAYLOAD_REG * sizeof (UINT32)));
    ZeroMem (mNvdimmSmbusSmmInterface->RequestData.FisRequest.OutputPayload, sizeof (mNvdimmSmbusSmmInterface->RequestData.FisRequest.OutputPayload));
    mNvdimmSmbusSmmInterface->RequestData.FisRequest.Opcode = Opcode;
    mNvdimmSmbusSmmInterface->RequestData.FisRequest.Timeout = Timeout;
    mNvdimmSmbusSmmInterface->RequestData.FisRequest.MbStatus = FIS_STS_UNDEFINED;
    //
    // Generating SW SMI
    //
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "S%d.C%d.D%d: Generating SW SMI for request 0x%04X on SMBus mailbox \n", Socket, Ch, Dimm, Opcode));
    IoWrite8 (0xB2, (UINT8)NVDIMM_SMBUS_ACCESS_SWITCH_TO_SMM_SW_SMI);

    //
    // Assign returned FIS status code
    //
    if (NULL != MbFisStatus) {
      *MbFisStatus = (UINT8)mNvdimmSmbusSmmInterface->RequestData.FisRequest.MbStatus;
    }

    //
    // SMI handler executed, retrieve operation status and return error if needed
    //
    if (EFI_ERROR (mNvdimmSmbusSmmInterface->RequestStatus)) {
      return mNvdimmSmbusSmmInterface->RequestStatus;
    }

    CopyMem (OutputPayload, mNvdimmSmbusSmmInterface->RequestData.FisRequest.OutputPayload, (NUM_PAYLOAD_REG * sizeof (UINT32)));
  }

  return EFI_SUCCESS;
}
#else
/**
  Reads the FW Boot Status Registers.

  @param[in]  Socket          - Socket Number
  @param[in]  Ch              - DDR Channel id
  @param[in]  Dimm            - DIMM Number
  @param[out] Bsr             - Pointer to value of Boot Status Register

  @return Status              - Status of the operation
**/
EFI_STATUS
NvdimmSmbusAccessReadBootStatusRegisters (
  IN  UINT8   Socket,
  IN  UINT8   Ch,
  IN  UINT8   Dimm,
  OUT UINT64  *Bsr
  )
{
  EFI_STATUS  Status;
  UINT32      BsrL;
  UINT32      BsrH;
  UINT8       Imc;

  if (NULL == Bsr) {
    return EFI_INVALID_PARAMETER;
  }

  Imc = ConvertSktChToImc (Ch);
  Status = NvdimmSmbusAccessAcquire (Socket, Imc);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access acquiring failed (status %r)\n", Socket, Ch, Dimm, Status));
    return EFI_DEVICE_ERROR;
  }
#ifdef DDR5_SUPPORT
  Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, D_FW_STATUS_CWV_D_UNIT_0_REG, &BsrL);
#else
  Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, D_FW_STATUS_FNV_D_UNIT_0_REG, &BsrL);
#endif
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
    //
    // SMBus mailbox interface filed - try to release SMBus access
    //
    Status = NvdimmSmbusAccessRelease (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
    }
    return EFI_DEVICE_ERROR;
  }

#ifdef DDR5_SUPPORT
  Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, D_FW_STATUS_H_CWV_D_UNIT_0_REG, &BsrH);
#else
  Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, D_FW_STATUS_H_FNV_D_UNIT_0_REG, &BsrH);
#endif
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
    //
    // SMBus mailbox interface filed - try to release SMBus access
    //
    Status = NvdimmSmbusAccessRelease (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
    }
    return EFI_DEVICE_ERROR;
  }

  *Bsr |= (((UINT64)BsrH << SMB_BSR_H_SHIFT) | ((UINT64)BsrL));

  Status = NvdimmSmbusAccessRelease (Socket, Imc);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("FIS") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**

  NvdimmSmbusAccessMailboxInterface is a generic read/write to the BIOS/SMM mailbox

  @param[in]  Socket          - Socket Number
  @param[in]  Ch              - DDR Channel id
  @param[in]  Dimm            - DIMM Number
  @param[in]  InputPayload    - Input Payload Register
  @param[out] OutputPayload   - Output Payload Register
  @param[in]  Opcode          - NVMCTLR command (opcode)
  @param[in]  Timeout         - Timeout for executing command
  @param[out] *MbFisStatus       - Status of FIS command

  @return Status              - Status of the operation

**/
EFI_STATUS
NvdimmSmbusAccessMailboxInterface (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT32    InputPayload[NUM_PAYLOAD_REG],
  OUT UINT32    OutputPayload[NUM_PAYLOAD_REG],
  IN  UINT32    Opcode,
  IN  UINT64    Timeout,
  OUT UINT8     *MbFisStatus
  )
{
  EFI_STATUS                            Status;
  EFI_STATUS                            MbStatus;
#ifdef DDR5_SUPPORT
  MB_SMBUS_CMD_CWV_DS_UNIT_0_STRUCT     SmbusCommandRegister;
  MB_SMBUS_STATUS_CWV_DS_UNIT_0_STRUCT  SmbusStatusRegister;
#else
  MB_SMBUS_CMD_FNV_D_UNIT_0_STRUCT      SmbusCommandRegister;
  MB_SMBUS_STATUS_FNV_D_UNIT_0_STRUCT   SmbusStatusRegister;
#endif
  UINT64                                StartTime;
  UINT64                                EndTime;
  UINT64                                TimeoutCounter;
  UINT8                                 OpcodeIndex = 0;
  UINT8                                 i;
  UINT8                                 Imc;
  CHAR8                                 *MboxErrStrPtr = "Mailbox Failure: N/A";

  //
  // If users not pass any timeout use defaults
  //
  if (Timeout == 0) {
    Timeout = COMP_TIMEOUT;
  }

  //
  // Validate NVDIMM commend opcode
  //
  Status = NvdimmSmbusAccessValidateOpcode (Opcode, &OpcodeIndex);
  if (EFI_ERROR (Status)){
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "S%d.C%d.D%d: Invalid request opcode (0x%04X)\n", Socket, Ch, Dimm, Opcode));
    return EFI_INVALID_PARAMETER;
  }

  Imc = ConvertSktChToImc (Ch);
  Status = NvdimmSmbusAccessAcquire (Socket, Imc);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access acquiring failed (status %r)\n", Socket, Ch, Dimm, Status));
    return EFI_DEVICE_ERROR;
  }

  //
  // Initialize for doorbell status retrieval timeout
  //
  TimeoutCounter = (DOORBELL_TIMEOUT / COMMON_DELAY_UNIT);

  //
  // Read command register to check doorbell status
  //
#ifdef DDR5_SUPPORT
  Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, &SmbusCommandRegister.Data);
#else
  Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, &SmbusCommandRegister.Data);
#endif
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
    //
    // SMBus mailbox interface filed - try to release SMBus access
    //
    Status = NvdimmSmbusAccessRelease (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
    }
    return EFI_DEVICE_ERROR;
  }

  //
  // Check/Wait for doorbell idle status
  //
  while (SmbusCommandRegister.Bits.doorbell) {
    if (!TimeoutCounter) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: Previous request 0x%04X on SMBus mailbox didn't complete\n",
        Socket, Ch, Dimm, SmbusCommandRegister.Bits.opcode));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }

    MicroSecondDelay (COMMON_DELAY_UNIT);
    TimeoutCounter--;

#ifdef DDR5_SUPPORT
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, &SmbusCommandRegister.Data);
#else
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, &SmbusCommandRegister.Data);
#endif
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }
  }

  //
  // Send Opcode to Mailbox
  //
  SmbusCommandRegister.Bits.opcode = Opcode;
#ifdef DDR5_SUPPORT
  Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, SmbusCommandRegister.Data);
#else
  Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, SmbusCommandRegister.Data);
#endif
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
    //
    // SMBus mailbox interface filed - try to release SMBus access
    //
    Status = NvdimmSmbusAccessRelease (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
    }
    return EFI_DEVICE_ERROR;
  }

  //
  // Fill Input Payload Registers
  //
  for (i = 0; i < FnvPayloadLookupTable [OpcodeIndex][INPUT_PAYLOAD_INDEX]; i++){
#ifdef DDR5_SUPPORT
    Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, (MB_SMBUS_INPUT_PAYLOAD_0_CWV_DS_UNIT_0_REG + (i * 4)), InputPayload[i]);
#else
    Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, (MB_SMBUS_INPUT_PAYLOAD_0_FNV_D_UNIT_0_REG + (i * 4)), InputPayload[i]);
#endif
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }
  }

  //
  // Set doorbell to busy status
  //
  SmbusCommandRegister.Bits.doorbell = BUSY;
#ifdef DDR5_SUPPORT
  Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, SmbusCommandRegister.Data);
#else
  Status = NvdimmSmbusAccessWriteMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, SmbusCommandRegister.Data);
#endif
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
    //
    // SMBus mailbox interface filed - try to release SMBus access
    //
    Status = NvdimmSmbusAccessRelease (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
    }
    return EFI_DEVICE_ERROR;
  }

  //
  // Start Performance counter
  //
  StartTime = GetPerformanceCounter ();

  //
  // Initialize for doorbell status retrieval timeout
  //
  TimeoutCounter = (Timeout / COMMON_DELAY_UNIT);

  //
  // Read command register to check doorbell status
  //
#ifdef DDR5_SUPPORT
  Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, &SmbusCommandRegister.Data);
#else
  Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, &SmbusCommandRegister.Data);
#endif
  if (EFI_ERROR (Status)) {
   CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
   //
   // SMBus mailbox interface filed - try to release SMBus access
   //
   Status = NvdimmSmbusAccessRelease (Socket, Imc);
   if (EFI_ERROR (Status)) {
     CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
   }
   return EFI_DEVICE_ERROR;
  }

  //
  // Check/Wait for doorbell idle status
  //
  while (SmbusCommandRegister.Bits.doorbell) {
    if (!TimeoutCounter) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: Request 0x%04X on SMBus mailbox failed after %lldus (timeout %lldus)\n",
        Socket, Ch, Dimm, Opcode, NvdimmSmbusAccessExecutionTime (StartTime, GetPerformanceCounter ()), Timeout));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }

    MicroSecondDelay (COMMON_DELAY_UNIT);
    TimeoutCounter--;

#ifdef DDR5_SUPPORT
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_CWV_DS_UNIT_0_REG, &SmbusCommandRegister.Data);
#else
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_CMD_FNV_D_UNIT_0_REG, &SmbusCommandRegister.Data);
#endif
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }
  }

  //
  // Stop Performance counter
  //
  EndTime = GetPerformanceCounter ();

  //
  // Read completion and status
  //
#ifdef DDR5_SUPPORT
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_STATUS_CWV_DS_UNIT_0_REG, &SmbusStatusRegister.Data);
#else
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, MB_SMBUS_STATUS_FNV_D_UNIT_0_REG, &SmbusStatusRegister.Data);
#endif
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: NVDIMM communication failed (status %r)\n", Socket, Ch, Dimm, Status));
    //
    // SMBus mailbox interface filed - try to release SMBus access
    //
    Status = NvdimmSmbusAccessRelease (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
    }
    return EFI_DEVICE_ERROR;
  }

  //
  // Assign MB status
  //
  if (NULL != MbFisStatus) {
    *MbFisStatus = (UINT8)SmbusStatusRegister.Bits.stat;
  }

  //
  // Check completion bit
  //
  if (!SmbusStatusRegister.Bits.comp) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: Request 0x%04X on SMBus mailbox - completion bit not set\n", Socket, Ch, Dimm, Opcode));
    //
    // SMBus mailbox interface filed - try to release SMBus access
    //
    Status = NvdimmSmbusAccessRelease (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
    }
    return EFI_DEVICE_ERROR;
  }

  //
  // Check for MB Errors and convert to status accordingly
  //
  MbStatus = MailboxStatusDecode ((UINT8)SmbusStatusRegister.Bits.stat, (UINT16)Opcode, InputPayload, &MboxErrStrPtr);
  if (EFI_ERROR (MbStatus)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: Request 0x%04X on SMBus mailbox failed - status %r (%a)\n",
      Socket, Ch, Dimm, Opcode, MbStatus, MboxErrStrPtr));
    //
    // SMBus mailbox interface filed - try to release SMBus access
    //
    Status = NvdimmSmbusAccessRelease (Socket, Imc);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      return EFI_DEVICE_ERROR;
    }
    //
    // Return status from MB Decoder
    //
    return MbStatus;
  }

  //
  // Read Output Payload Registers
  //
  for (i = 0; i < FnvPayloadLookupTable [OpcodeIndex][OUTPUT_PAYLOAD_INDEX]; i++){
#ifdef DDR5_SUPPORT
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, (MB_SMBUS_OUTPUT_PAYLOAD_0_CWV_DS_UNIT_0_REG + (i * 4)), &OutputPayload[i]);
#else
    Status = NvdimmSmbusAccessReadMailboxRegister (Socket, Ch, Dimm, (MB_SMBUS_OUTPUT_PAYLOAD_0_FNV_D_UNIT_0_REG + (i * 4)), &OutputPayload[i]);
#endif
    if (EFI_ERROR (Status)) {
      //
      // SMBus mailbox interface filed - try to release SMBus access
      //
      Status = NvdimmSmbusAccessRelease (Socket, Imc);
      if (EFI_ERROR (Status)) {
        CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("SMB") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
      }
      return EFI_DEVICE_ERROR;
    }
  }

  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "S%d.C%d.D%d: Request 0x%04X on SMBus mailbox completed in %lldus (timeout %lldus)\n",
    Socket, Ch, Dimm, Opcode, NvdimmSmbusAccessExecutionTime (StartTime, EndTime), Timeout));

  Status = NvdimmSmbusAccessRelease (Socket, Imc);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR("FIS") "S%d.C%d.D%d: SMBus access releasing failed (status %r)\n", Socket, Ch, Dimm, Status));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}
#endif

/**
  TSOD polling state retrieving.

  @param[in]     Socket          - Socket index at system level
  @param[in]     Imc             - Imc index at socket level
  @param[in]     PollingState    - TSOD polling state (0x00-disable, 0x01-Enable)

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NvdimmSmbuslGetPollingState (
  IN     UINT8  Socket,
  IN     UINT8  Imc,
  OUT    UINT8  *PollState
  )
{
  UINT8   MaxImc;

  MaxImc = GetMaxImc ();

  if (NULL == PollState) {
    return EFI_INVALID_PARAMETER;
  }

  if (Socket >= MAX_SOCKET || Imc >= MaxImc) {
    return EFI_INVALID_PARAMETER;
  }

  if (!mSystemMemoryMap->Socket[Socket].imcEnabled[Imc]) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "Socket (%d) iMC (%d) - iMC not enabled\n", Socket, Imc));
    return EFI_INVALID_PARAMETER;
  }

  *PollState = (UINT8) GetTsodPollEnSmb (Socket, GetProcSmbInstanceFromMcId (Imc));
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "Socket (%d) iMC (%d) - TSOD Polling %a\n", Socket, Imc, ((*PollState) ? "enabled" : "disabled")));
  return EFI_SUCCESS;

}

/**
  TSOD Polling settings

  @param[in]     Socket          - Socket index at system level
  @param[in]     Imc             - Imc index at socket level
  @param[in]     PollState      - Enable TSOD polling.(0x00-disable, 0x01-Enable)
  @param[out]    SaveState      - Preserve polling state if provided.

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NvdimmSmbusSetPollingState (
  IN     UINT8           Socket,
  IN     UINT8           Imc,
  IN     UINT8           PollState,
  OUT    UINT8  * CONST  SaveState
  )
{
  UINT8  MaxImc;
  UINT32 CurrentPollState;

  MaxImc = GetMaxImc ();

  if (Socket >= MAX_SOCKET || Imc >= MaxImc) {
    return EFI_INVALID_PARAMETER;
  }

  if (!mSystemMemoryMap->Socket[Socket].imcEnabled[Imc]) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "Socket (%d) iMC (%d) - iMC not enabled\n", Socket, Imc));
    return EFI_INVALID_PARAMETER;
  }

  if (mInSmm) {
    CurrentPollState = GetTsodPollEnSmb (Socket, GetProcSmbInstanceFromMcId (Imc));

    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "Socket (%d) iMC (%d) - TSOD Polling state change from '%a' to '%a'\n",
        Socket, Imc, (CurrentPollState ? "enabled" : "disabled"), (PollState ? "enabled" : "disabled")));

    if (SaveState != NULL) {
      *SaveState = (UINT8)CurrentPollState;
    }

    SetTsodPollEnSmb (Socket, GetProcSmbInstanceFromMcId (Imc), PollState);
  } else {
    //
    // Update type/parameters/data in mNvdimmSmbusSmmInterfaceProtocol to access in SMM
    //
    if (mNvdimmSmbusSmmInterface == NULL) {
      //
      // NVDIMM SMBus DXE <-> SMM interface should already allocated at CR DXE driver entry
      //
      ASSERT (FALSE);
      return EFI_LOAD_ERROR;
    }

    mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Socket = Socket;
    mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Imc = Imc;
    mNvdimmSmbusSmmInterface->RequestType = SET_TSOD_POLLING_STATE;
    mNvdimmSmbusSmmInterface->RequestData.SetTsodPollState.PollState = PollState;

    //
    // Generating SW SMI for TSOD register value change
    //
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "Socket (%d) iMC (%d) - Generating SW SMI for TSOD polling state change\n", Socket, Imc));
    IoWrite8 (0xB2, (UINT8)NVDIMM_SMBUS_ACCESS_SWITCH_TO_SMM_SW_SMI);

    //
    // SMI handler executed, retrieve operation status
    //
    if (EFI_ERROR (mNvdimmSmbusSmmInterface->RequestStatus)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "Socket (%d) iMC (%d) - TSOD polling state change failed with status %r\n", Socket, Imc, mNvdimmSmbusSmmInterface->RequestStatus));
      return EFI_DEVICE_ERROR;
    }
  }

  return EFI_SUCCESS;
}

/**
  @brief Return maximum amount of time allowed to use the SMBus

  This function returns maximum amount of time allowed for UEFI MGMT SW to use the SMBus
  and can be overwritten by OEM hook. Note, that zero is special value and means no limit.

  @param[out] TimeMs           Maximum amount of time in ms allowed to use the SMBus.

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_INVALID_PARAMETER TimeMs is NULL.
**/
EFI_STATUS
NvdimmSmbusGetMaxAccessTime (
  OUT UINT32 *TimeMs
  )
{
  EFI_STATUS Status;

  if (NULL == TimeMs) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check Platform Policy for TSOD polling state
  //
  if (mSystemMemoryMap->MemSetup.thermalThrottlingOptions & CLTT_EN) {
    *TimeMs = mSystemMemoryMap->MemSetup.NvdimmSmbusMaxAccessTime;
  } else {
    //
    // TSOD polling is disabled, no time restrictions when accessing the SMBus
    //
    *TimeMs = 0;
  }

  //
  // Call OEM-hook to update/modify the time value
  //
  Status = OemSmbusGetMaxAccessTime (TimeMs);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "OEM SMBus max allowed time retrieval failed with status %r\n", Status));
    return Status;
  }

  if (*TimeMs == 0) {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "SMBus max allowed access time: no time restrictions when accessing the SMBus\n"));
  } else {
    CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "SMBus max allowed access time: %d ms\n", *TimeMs));
  }

  return EFI_SUCCESS;

}

/**
  @brief Provides interface to acquire SMBus access

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control acquisition or TSOD state change failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
EFI_STATUS
NvdimmSmbusAccessAcquire (
  IN UINT8 Socket,
  IN UINT8 Imc
  )
{
  EFI_STATUS Status;
  UINT8      MaxImc;

  MaxImc = GetMaxImc ();

  if (Socket >= MAX_SOCKET || Imc >= MaxImc) {
    return EFI_INVALID_PARAMETER;
  }

  if (!mSmbusAccessData[Socket][Imc].AccessAcquired) {
    mSmbusAccessData[Socket][Imc].AccessAcquired = TRUE;
  } else {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "SMBus access in progress (socket: %d, iMC %d)\n", Socket, Imc));
    return EFI_ALREADY_STARTED;
  }

  //
  // Call OEM-hook for SMBus control acquisition
  //
  Status = OemAcquireSmbusCtrl (Socket, Imc, &mSmbusAccessData[Socket][Imc].OemTsodPollingState);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "OEM SMBus control acquisition failed (socket: %d, iMC %d) with status %r\n", Socket, Imc, Status));
    mSmbusAccessData[Socket][Imc].AccessAcquired = FALSE;
    mSmbusAccessData[Socket][Imc].TsodPollingState = NVDIMM_SMBUS_TSOD_POLLING_UNKNOWN;
    return EFI_NOT_READY;
  }

  if (mSmbusAccessData[Socket][Imc].OemTsodPollingState == NVDIMM_SMBUS_TSOD_POLLING_UNKNOWN) {
    Status = NvdimmSmbuslGetPollingState (Socket, Imc, &mSmbusAccessData[Socket][Imc].TsodPollingState);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") " SMBus access not acquired (socket: %d, iMC %d) - retrieving TSOD state failed\n", Socket, Imc));
    }
  } else {
    //
    // Do not need to get the TSOD Polling State just use the OEM TSOD Polling state
    //
    mSmbusAccessData[Socket][Imc].TsodPollingState = mSmbusAccessData[Socket][Imc].OemTsodPollingState;
  }

  //
  // Disable TSOD polling (if enabled and no errors so far) and save initial state
  //
  if (!EFI_ERROR (Status) && mSmbusAccessData[Socket][Imc].TsodPollingState == NVDIMM_SMBUS_TSOD_POLLING_ENABLED) {
    Status = NvdimmSmbusSetPollingState (Socket, Imc, NVDIMM_SMBUS_TSOD_POLLING_DISABLED, NULL);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "SMBus access not acquired (socket: %d, iMC %d) - TSOD state change failed\n", Socket, Imc));
    }
  }

  //
  // Check for error (we need to release OEM SMBus control in case of non-successful SMBus access acquisition)
  //
  if (EFI_ERROR (Status)) {
    Status = OemReleaseSmbusCtrl (Socket, Imc, mSmbusAccessData[Socket][Imc].OemTsodPollingState);
    if (EFI_ERROR (Status)) {
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "OEM SMBus control release failed (socket: %d, iMC %d) with status %r\n", Socket, Imc, Status));
    }
    mSmbusAccessData[Socket][Imc].AccessAcquired = FALSE;
    mSmbusAccessData[Socket][Imc].TsodPollingState = NVDIMM_SMBUS_TSOD_POLLING_UNKNOWN;
    return EFI_DEVICE_ERROR;
  }

  //
  // SMBus access acquisition completed successfully
  //
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "SMBus access acquired (socket: %d, iMC %d)\n", Socket, Imc));
  return EFI_SUCCESS;
}

/**
  @brief Provides interface to release SMBus access

  @param[in]  Socket            Socket index at system level
  @param[in]  Imc               iMC index at socket level

  @retval EFI_SUCCESS           Command completed successfully.
  @retval EFI_NOT_READY         TSOD control acquisition or TSOD state change failed.
  @retval EFI_INVALID_PARAMETER Invalid Socket or Imc value.
**/
EFI_STATUS
NvdimmSmbusAccessRelease (
  IN UINT8 Socket,
  IN UINT8 Imc
  )
{
  EFI_STATUS Status;
  UINT8      MaxImc;

  MaxImc = GetMaxImc ();

  if (Socket >= MAX_SOCKET || Imc >= MaxImc) {
    return EFI_INVALID_PARAMETER;
  }

  if (!mSmbusAccessData[Socket][Imc].AccessAcquired) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "SMBus access not acquired - nothing to release (socket: %d, iMC %d)\n", Socket, Imc));
    return EFI_NOT_STARTED;
  }

  //
  // Restore the initial TSOD state if needed
  //
  if (mSmbusAccessData[Socket][Imc].TsodPollingState == NVDIMM_SMBUS_TSOD_POLLING_ENABLED) {
    Status = NvdimmSmbusSetPollingState (Socket, Imc, NVDIMM_SMBUS_TSOD_POLLING_ENABLED, NULL);
    if (EFI_ERROR (Status)) {
      mSmbusAccessData[Socket][Imc].AccessAcquired = FALSE;
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "SMBus access released with errors (socket: %d, iMC %d)\n", Socket, Imc));
      return EFI_DEVICE_ERROR;
    }
    MicroSecondDelay (NVDIMM_SMBUS_ACCESS_MS_TO_US (mSystemMemoryMap->MemSetup.NvdimmSmbusReleaseDelay));
  }

  //
  // Call OEM-hook for SMBus control release
  //
  Status = OemReleaseSmbusCtrl (Socket, Imc, mSmbusAccessData[Socket][Imc].OemTsodPollingState);
  if (EFI_ERROR (Status)) {
    mSmbusAccessData[Socket][Imc].AccessAcquired = FALSE;
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "SMBus access released with errors (socket: %d, iMC %d)\n", Socket, Imc));
    return EFI_NOT_READY;
  }

  mSmbusAccessData[Socket][Imc].AccessAcquired = FALSE;
  CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "SMBus access released (socket: %d, iMC %d)\n", Socket, Imc));
  return EFI_SUCCESS;
}

/**
  @brief SMM handler to handle NVDIMM SMBus access related actions.

  @param[in]  DispatchHandle       - EFI handler
  @param[in]  DispatchContext      - context from the caller
  @param[in]  CommBuffer           - buffer from caller - optional
  @param[out] CommBufferSize       - size of the buffer - optional

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NvdimmSmbusAccessSmmActionsHandler (
   IN       EFI_HANDLE                   DispatchHandle,
   IN CONST EFI_SMM_SW_REGISTER_CONTEXT  *DispatchContext,
   IN OUT   VOID                         *CommBuffer,     OPTIONAL
   IN OUT   UINTN                        *CommBufferSize  OPTIONAL
   )
{
  switch (mNvdimmSmbusSmmInterface->RequestType) {
    case SET_TSOD_POLLING_STATE:
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "SMM TSOD change polling state (socket %d, iMC %d)\n",
          mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Socket, mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Imc));

      mNvdimmSmbusSmmInterface->RequestStatus = NvdimmSmbusSetPollingState (
        mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Socket,
        mNvdimmSmbusSmmInterface->RequestParams.SetTsodPollState.Imc,
        mNvdimmSmbusSmmInterface->RequestData.SetTsodPollState.PollState,
        NULL);
      break;

#ifdef ICX_HOST
    case FIS_REQUEST:
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "S%d.C%d.D%d: Process request 0x%04X on SMBus mailbox \n",
        mNvdimmSmbusSmmInterface->RequestParams.FisRequest.Socket,
        mNvdimmSmbusSmmInterface->RequestParams.FisRequest.Ch,
        mNvdimmSmbusSmmInterface->RequestParams.FisRequest.Dimm,
        mNvdimmSmbusSmmInterface->RequestData.FisRequest.Opcode));

      mNvdimmSmbusSmmInterface->RequestStatus = NvdimmSmbusAccessMailboxInterface (
        mNvdimmSmbusSmmInterface->RequestParams.FisRequest.Socket,
        mNvdimmSmbusSmmInterface->RequestParams.FisRequest.Ch,
        mNvdimmSmbusSmmInterface->RequestParams.FisRequest.Dimm,
        mNvdimmSmbusSmmInterface->RequestData.FisRequest.InputPayload,
        mNvdimmSmbusSmmInterface->RequestData.FisRequest.OutputPayload,
        mNvdimmSmbusSmmInterface->RequestData.FisRequest.Opcode,
        mNvdimmSmbusSmmInterface->RequestData.FisRequest.Timeout,
        &(mNvdimmSmbusSmmInterface->RequestData.FisRequest.MbStatus));
      break;

    case GET_BSR:
      CRDEBUG ((DEBUG_INFO, CR_INFO_STR ("SMB") "S%d.C%d.D%d: Get Boot Status Register \n",
        mNvdimmSmbusSmmInterface->RequestParams.GetBsr.Socket,
        mNvdimmSmbusSmmInterface->RequestParams.GetBsr.Ch,
        mNvdimmSmbusSmmInterface->RequestParams.GetBsr.Dimm));

      mNvdimmSmbusSmmInterface->RequestStatus = NvdimmSmbusAccessReadBootStatusRegisters (
        mNvdimmSmbusSmmInterface->RequestParams.GetBsr.Socket,
        mNvdimmSmbusSmmInterface->RequestParams.GetBsr.Ch,
        mNvdimmSmbusSmmInterface->RequestParams.GetBsr.Dimm,
        mNvdimmSmbusSmmInterface->RequestData.GetBsr.BootStatusRegister);
      break;
#endif

    default:
      CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "Unknown SMM request type (type %d)\n", mNvdimmSmbusSmmInterface->RequestType));
      mNvdimmSmbusSmmInterface->RequestStatus = EFI_INVALID_PARAMETER;
      break;
  }

  return EFI_SUCCESS;
}

/**
  @brief Initializes NVDIMM SMBus access
  (installs SMM handler for NVDIMM SMBus related actions)

  @param None

  @return Standard EFI_STATUS is returned.
**/
EFI_STATUS
NvdimmSmbusAccessSmmInit (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_SMM_SW_DISPATCH2_PROTOCOL *SwDispatch = 0;
  EFI_SMM_SW_REGISTER_CONTEXT   SwContext;
  EFI_HANDLE                    SwHandle;

  Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID**)&SwDispatch);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "Can't locate SW Dispatch protocol (status %r)\n", Status));
    return EFI_ABORTED;
  }

  SwContext.SwSmiInputValue = NVDIMM_SMBUS_ACCESS_SWITCH_TO_SMM_SW_SMI;
  Status = SwDispatch->Register (SwDispatch, (EFI_SMM_HANDLER_ENTRY_POINT2)NvdimmSmbusAccessSmmActionsHandler, &SwContext, &SwHandle);
  if (EFI_ERROR (Status)) {
    CRDEBUG ((DEBUG_ERROR, CR_ERROR_STR ("SMB") "SMM handler registration failed (status %r)\n", Status));
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}
