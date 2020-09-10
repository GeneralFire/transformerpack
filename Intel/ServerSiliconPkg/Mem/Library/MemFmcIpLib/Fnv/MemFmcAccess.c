/** @file
  FalconValley Access mechanisms and mailbox commands.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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
#include <Memory/CpgcDefinitions.h>
#include <Library/MemFmcIpLib.h>
#include <Library/ProcMemInitChipLib.h>

#include <Chip/Include/CpuPciAccess.h>
#include <Chip/Include/FnvAccessCommon.h>
#include "Include/MemFmcAccess.h"
#include "Include/MemFmcRegs.h"

#include <UncoreCommonIncludes.h>
#include <SysRegs.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KtiApi.h>
#include <Library/CteNetLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/CsrAccessLib.h>
#include "Include/MemFmcIpLibInternal.h"
#include <Library/SpdAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/MemDdrioIpLib.h>


const   UINT32 aepDensity[16]  =  {32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576}; // MBytes
const   UINT32 aepdataWidth[4] = {8,16,32,64};
const   UINT32 aepSizeGB[MAX_TECH_DDRT] = {8,16,32,64,128,256,512,1024}; // NVMDIMM size in GB

/**
  Returns the capacity of the DCPMM DIMM at the input Socket, Channel, and DIMM
  index in units of GB. Returns a size of zero if an error is encountered.

  @param[in] Socket:       Socket index
  @param[in] Channel:      Channel index within the socket
  @param[in] Dimm:         DIMM slot index within the channel

  @return The capacity of the DCPMM DIMM in units of GB

**/
UINT32
GetDcPmmDimmSize (
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm
  )
{
  UINT8              TechIndex;
  PSYSHOST           Host;
  DIMM_NVRAM_STRUCT  (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  TechIndex = (*DimmNvList)[Dimm].aepTechIndex;

  if (TechIndex < MAX_TECH_DDRT) {
    return aepSizeGB[TechIndex];
  } else {
    return 0;
  }
}

//-----------------------------------------------------------------------------------------------
/*
NVMCTLR address mapping

Function              AHB Slave Name  Base Address  End Address
DPA Mapped Registers  AHB_Slave_1     0xC0000000    0xC3FFFFFF
DA Unit CSRs          AHB_Slave_2     0xC4000000    0xC7FFFFFF
Dunit CSRs            AHB_Slave_3     0xC8000000    0xCBFFFFFF
Aunit CSRs            AHB_Slave_4     0xCC000000    0xCFFFFFFF
Punit CSR             AHB_Slave_5     0xD0000000    0xD3FFFFFF
Security CSRs         AHB_Slave_6     0xD4000000    0xD7FFFFFF
DFx/Misc CSRs         AHB_Slave_7     0xD8000000    0xDBFFFFFF
Reserved              AHB_Slave_8     0xDC000000    0xDFFFFFFF


AHB Addr Bit        31:29   28:26         25:22                     21:13           12:0
NVMCTLR CSR AHB Address 110b    Slave Number  Requestor ID              000000000b      12:0

AHB Addr Bit        31:29   28:26         25:22                     21:13           12    11:0
SMBus/JTAG (JCONFL) 110b    Dev Fun[2:1]  Req ID = SMBus or JTAG    000000000b      F[0]  11:0

AHB Addr Bit        31:29   28:26         25:22                     21:13           12:0
Extended MRS        110b    15:13         Req ID = NVMDIMM             000000000b      12:0
NOTE: Extended MRS Access is disabled by FW after NVMDIMM Initialization

AHB Addr Bit        31:29   28:26         25:22                     21:13           12:3    2:0
NVMDIMM DPA            110b    18:16         Req ID = NVMDIMM             000000000b      15:6    000b
NOTES: NVMDIMM DPA Starting Address A[42:0] = 000_0000_0000
Access to complete NVMCTLR Registers space is only enabled for debug
NVMCTLR Registers Occupy 512KByte NVMDIMM DPA Space + reserved (total 1MB)
NVMCTLR Reads/Writes Eight Bytes of NVMCTLR CSR AHB space per 64B NVMDIMM Access
(Data [63:0])

AHB Addr Bit        31:26                 25:22                     21:11           10:3    2:0
NVMDIMM Mailbox        110000b               Req ID = NVMDIMM             0100000000000b  13:6    000b
NOTES: NVMDIMM DPA Starting Address A[42:0] = 000_0010_0000
Mailbox Registers Occupy 12KByte NVMDIMM DPA Space + reserved (total 1MB)
NVMCTLR Reads/Writes Eight Bytes of NVMCTLR CSR AHB space per 64B NVMDIMM Access
(Data [63:0])
*/

#define FNV_SMB_REG_ADDR(Device,Function,Offset) \
  (((Offset) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1) << 15))

// Note: Mask bits[5:3] to ensure Quad word access (the first UI of the NVMDIMM data burst)
// CSR write size less than Quad word will need to use CSR read-modify-write unless caller handles caching
#define FNV_DPA_REG_ADDR(Device,Function,Offset) \
  ((((Offset) & 0xff8) << 3) | (((Function) & 0x07) << 15) | (((Device) & 0x1) << 18))

//-----------------------------------------------------------------------------------------------
/**

  Computes address of NVMCTLR configuration space

  @param Host      - Pointer to the system Host (root) structure
  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param Offset    - Register offset; values come from the auto generated header file
  @param Size      - Register size in bytes (may be updated if pseudo-offset)

  @retval Address

**/
STATIC UINT32
GetFnvCfgAddress (
                 PSYSHOST Host,
                 UINT8    fnvInterface,
                 UINT32   Offset
                 )
{
  UINT32 Dev;
  UINT32 Fun;
  UINT32 RegAddr = 0;
  UINT8 BoxType;
  UINT8 FuncBlk;
  CSR_OFFSET RegOffset;

  RegOffset.Data = Offset;

  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;

  // Assert NVMCTLR BoxType == 5
  RC_FATAL_ERROR (BoxType == 0x1B, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_31);

  //
  // Identify the NVMCTLR Device/Function number for the access
  //
  if (FuncBlk < 0xe) {

    Dev = FuncBlk >> 3;
    Fun = FuncBlk & 7;
    //
    // Form address
    //
    switch (fnvInterface) {
    case FMC_SMB:
    case FMC_EMRS:
      RegAddr = FNV_SMB_REG_ADDR(Dev, Fun, RegOffset.Bits.offset);
      break;
    case FMC_DPA:
      RegAddr = FNV_DPA_REG_ADDR(Dev, Fun, RegOffset.Bits.offset);
      break;
    default:
      //Assert on error
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_32);
      break;
    }
  } else {

    //Assert on error
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_32);
  }

  return RegAddr;

} // GetFnvCfgAddress

EFI_STATUS
ReadFnvCfgSmbWorkerFunction(
  PSYSHOST          Host,
  UINT8             socket,
  UINT32            smbReg,
  SMB_DEVICE_STRUCT dev,
  volatile UINT32   *localData,
  UINT8             regSize
  )
{
  volatile UINT8  *pByte = (volatile UINT8 *)localData;
  UINT8  devFunc = 0;
  UINT8  rsvd = 0;
  UINT8  data8 = 0;
  EFI_STATUS Status;


  UINT8 *RegAddr = 0;
  RegAddr = (UINT8 *) (UINTN) GetProcSmbDataReg (socket, dev.address.SmbInstance, &regSize);

  //
  // Write command code 0x90 with Rsvd as data
  // ([7]Begin=1, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  if ((Status = WriteProcSmb (dev, 0x80, &rsvd)) != EFI_SUCCESS) {
    return Status;
  }

  //
  // Write command code 0x10 with DevFunc as data
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  devFunc = (UINT8)((smbReg >> 12) & 0xF);
  if ((Status = WriteProcSmb (dev, 0x00, &devFunc)) != EFI_SUCCESS) {
    return Status;
  }

  //
  // Write command code 0x10 with Rsvd as data for Reg[15:8]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  data8 = (UINT8)((smbReg >> 8) & 0xF);
  if ((Status = WriteProcSmb (dev, 0x00, &data8)) != EFI_SUCCESS) {
    return Status;
  }

  //
  // Write command code 0x50 with Reg as data for Reg[7:0]
  // ([7]Begin=0, [6]End=1, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  data8 = (UINT8)(smbReg & 0xFF);
  if ((Status = WriteProcSmb (dev, 0x40, &data8)) != EFI_SUCCESS) {
    return Status;
  }

  //
  // Read command code 0x90 with Rsvd holding status data
  // ([7]Begin=1, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  if ((Status = ReadProcSmb (dev, 0x80, &rsvd)) != EFI_SUCCESS) {
    return Status;
  }

  //
  // Read command code 0x10 with Data[31:24]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  if ((Status = ReadProcSmb (dev, 0x00, pByte + 3)) != EFI_SUCCESS) {
    return Status;
  }

  if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE_ON_SMBUS) {
    RcDebugPrint (SDBG_DEFAULT, "mov ecx, 0\n");
    ReadByteAndShift;                     // Note: this macro uses RegAddr variable
  }
  //
  // Read command code 0x10 with Data[23:16]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  if ((Status = ReadProcSmb (dev, 0x00, pByte + 2)) != EFI_SUCCESS) {
    return Status;
  }

  if (UbiosGenerationEnabled () && (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE_ON_SMBUS)) {
    ReadByteAndShift;
  }

  //
  // Read command code 0x10 with Data[15:8]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  if ((Status = ReadProcSmb (dev, 0x00, pByte + 1)) != EFI_SUCCESS) {
    return Status;
  }

  if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE_ON_SMBUS) {
    ReadByteAndShift;
  }

  //
  // Read command code 0x50 with Data[7:0]
  // ([7]Begin=0, [6]End=1, [4]PEC_en=1, [3:2]=00-DWord RD, [1:0]=Byte)
  //
  if ((Status = ReadProcSmb (dev, 0x40, pByte)) != EFI_SUCCESS) {
    return Status;
  }

  if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE_ON_SMBUS) {
    ReadByteAndShift;
  }

  return EFI_SUCCESS;
} //ReadFnvCfgSmbWorkerFunction

VOID
DdrtCosimDelay (
  IN UINT8 Delay
)
{
  if ((FeaturePcdGet (PcdCosimBuild) == TRUE) && (CteIsFnvFeedbackPresent ())) {

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //

    CteDelayDclk (Delay);
  }
}

//-----------------------------------------------------------------------------------------------
/**

  Read NVMCTLR SMBUS dev/offset

  @param Host        - Pointer to the system Host (root) structure
  @param socket        - Processor socket ID
  @param ch          - DDR CHannel ID
  @param dev         - SMBUS device address
  @param reg         - byte offset for NVMCTLR
  @param data        - pointer to data to be read

  @retval 0 - success
  @retval 1 - failure

**/
EFI_STATUS
ReadFnvCfgSmb(
             PSYSHOST Host,
             UINT8  socket,
             UINT8  ch,
             UINT8  dimm,
             UINT32 reg,
             UINT32 *data
             )

{
  UINT8 index = 0;
  UINT8 regSize = 0;
  UINT32 smbReg;
  EFI_STATUS Status = EFI_SUCCESS;
  volatile UINT32 localData = 0;
  UINT64  StartCount = 0;
  SMB_DEVICE_STRUCT dev;
  CSR_OFFSET RegOffset;

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "ReadFnvCfgSmb Starts\n");
#endif //MRC_TRACE

  DdrtCosimDelay (50);

  RegOffset.Data = reg;

  regSize = (UINT8)RegOffset.Bits.size;
  index = (UINT8)(RegOffset.Bits.offset & 3) << 3;
  smbReg = GetFnvCfgAddress (Host, FMC_SMB, reg);
  smbReg &= 0xFFFFFFFC;             // Always Do Dword access
  *data = 0xFFFFFFFF;

  if (FeaturePcdGet (PcdCteBuild) == FALSE) {
    //
    // Initialize smbDevice structure
    //
    dev.compId = SPD;
    dev.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
    dev.address.deviceType = DTI_FNV;

    if (EFI_ERROR (GetSmbAddress (socket, ch, dimm, &dev))) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
      return EFI_DEVICE_ERROR;
    }

    //
    // Apply a timeout for SMBUS reads
    //
    StartCount = GetCount ();
    do {
      localData = 0;
      Status = ReadFnvCfgSmbWorkerFunction (Host, socket, smbReg, dev, &localData, regSize);
      if ((IsSimics () == FALSE) && (GetDelay (StartCount) > FMC_CONFIG_SMBUS_TIMEOUT_MICROSECONDS)) {
        OutputWarning (WARN_SMBUS_FAULT, WARN_SMBUS_READ, socket, ch, dimm, NO_RANK);
        DisableChannelSw (Host, socket, ch);
        return EFI_DEVICE_ERROR;
      }
    } while (Status != EFI_SUCCESS);
  }


  localData >>= index;
  if (regSize == 4) {
    *data = localData;
  }
  else if (regSize == 2)
    *data = localData & 0x0FFFF;
  else
    *data = localData & 0x0FF;

  return Status;

}

EFI_STATUS
WriteFnvCfgSmbWorkerFunction (
  PSYSHOST          Host,
  UINT8             socket,
  UINT32            smbReg,
  SMB_DEVICE_STRUCT dev,
  UINT32            *localData,
  UINT8             regSize,
  UINT8             cmdCode
  )
{
  EFI_STATUS Status;
  UINT8  *pByte = (UINT8 *)localData;
  UINT8 devFunc = 0;
  UINT8 rsvd = 0;
  UINT8 data8 = 0;
  //
  // Write command code 0x9C with Rsvd as data
  // ([7]Begin=1, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  if ((Status = WriteProcSmb (dev, 0x80 | cmdCode, &rsvd)) != EFI_SUCCESS) {
    return Status;
  }

  //
  // Write command code 0x1C with DevFunc as data
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  devFunc = (UINT8)((smbReg >> 12) & 0xF);
  if ((Status = WriteProcSmb (dev, 0x00 | cmdCode, &devFunc)) != EFI_SUCCESS) {
    return Status;
  }

  //
  // Write command code 0x1C with Rsvd as data for Reg[15:8]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  data8 = (UINT8)((smbReg >> 8) & 0xF);
  if ((Status = WriteProcSmb (dev, 0x00 | cmdCode, &data8)) != EFI_SUCCESS) {
    return Status;
  }

  //
  // Write command code 0x1C with Reg as data for Reg[7:0]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  data8 = (UINT8)(smbReg & 0xFF);
  if ((Status = WriteProcSmb (dev, 0x00 | cmdCode, &data8)) != EFI_SUCCESS) {
    return Status;
  }

  //
  // Read command code 0x1C with Data[31:24]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  if (regSize == 4) {
    if ((Status = WriteProcSmb (dev, 0x00  | cmdCode, pByte + 3)) != EFI_SUCCESS) {
      return Status;
    }
  }

  //
  // Read command code 0x1C with Data[23:16]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  if (regSize == 4) {
    if ((Status = WriteProcSmb (dev, 0x00 | cmdCode, pByte + 2)) != EFI_SUCCESS) {
      return Status;
    }
  }

  //
  // Read command code 0x1C with Data[15:8]
  // ([7]Begin=0, [6]End=0, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  if ((regSize == 4) || (regSize == 2)) {
    if ((Status = WriteProcSmb (dev, 0x00 | cmdCode, pByte + 1)) != EFI_SUCCESS) {
      return Status;
    }
  }

  //
  // Read command code 0x5C with Data[7:0]
  // ([7]Begin=0, [6]End=1, [4]PEC_en=1, [3:2]=11-DWord WR, [1:0]=Byte)
  //
  if ((Status = WriteProcSmb (dev, 0x40 | cmdCode, pByte)) != EFI_SUCCESS) {
    return Status;
  }

  return EFI_SUCCESS;
} // WriteFnvCfgSmbWorkerFunction


//-----------------------------------------------------------------------------------------------
/**

  Write NVMCTLR SMBUS dev/offset with specified data

  @param Host        - Pointer to the system Host (root) structure
  @param socket        - Processor socket ID
  @param ch          - DDR Channel ID
  @param reg         - byte offset for NVMCTLR
  @param data        - pointer to data to be read

  @retval 0 - success
  @retval 1 - failure

**/
EFI_STATUS
WriteFnvCfgSmb (
               PSYSHOST Host,
               UINT8  socket,
               UINT8  ch,
               UINT8  dimm,
               UINT32 reg,
               UINT32 data
               )

{
  UINT8 regSize = 0;
  UINT32 smbReg;
  EFI_STATUS Status = EFI_SUCCESS;
  UINT64  StartCount = 0;
  UINT32 localData = data;
  SMB_DEVICE_STRUCT dev;
  UINT8 cmdCode;
  CSR_OFFSET RegOffset;
  UINT8 OriginalUbiosOutputMode = ASM_OUTPUT_ENABLE; // This default is needed for compilation but shouldn't matter.
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "WritePmemCfgSmb Starts\n");
#endif //MRC_TRACE

  if (UbiosGenerationEnabled () &&
      (Setup->common.ddrtXactor == 0))
  {
    OriginalUbiosOutputMode = GetUbiosOutputMode ();
    SetUbiosOutputMode (ASM_OUTPUT_ENABLE_ON_SMBUS);
  }

  DdrtCosimDelay (50);

  RegOffset.Data = reg;
  regSize = (UINT8)RegOffset.Bits.size;
  smbReg = GetFnvCfgAddress (Host, FMC_SMB, reg);

  if (FeaturePcdGet (PcdCteBuild) == FALSE) {
    if (regSize == 4) {
      cmdCode = 0x0C;
    } // ([7]Begin=0, [6]End=0, [4]PEC_en=0, [3:2]=11-DWord WR, [1:0]=Byte)
    else if (regSize == 2)
      cmdCode = 0x08; // ([7]Begin=0, [6]End=0, [4]PEC_en=0, [3:2]=10-Word WR, [1:0]=Byte)
    else
      cmdCode = 0x04; // ([7]Begin=0, [6]End=0, [4]PEC_en=0, [3:2]=01-BYTE WR, [1:0]=Byte)
    //
    // Initialize smbDevice structure
    //
    dev.compId = SPD;
    dev.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
    dev.address.deviceType = DTI_FNV;

    if (EFI_ERROR (GetSmbAddress (socket, ch, dimm, &dev))) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
      return EFI_DEVICE_ERROR;
    }

    //
    // Apply a timeout for SMBUS writes
    //
    StartCount = GetCount ();
    do {
      Status = WriteFnvCfgSmbWorkerFunction (Host, socket, smbReg, dev, &localData, regSize, cmdCode);
      if ((IsSimics () == FALSE) && (GetDelay (StartCount) > FMC_CONFIG_SMBUS_TIMEOUT_MICROSECONDS)) {
        OutputWarning (WARN_SMBUS_FAULT, WARN_SMBUS_WRITE, socket, ch, dimm, NO_RANK);
        DisableChannelSw (Host, socket, ch);
        return EFI_DEVICE_ERROR;
      }
    } while (Status != EFI_SUCCESS);
  }

  if (UbiosGenerationEnabled () &&
      (Setup->common.ddrtXactor == 0))
  {
    SetUbiosOutputMode (OriginalUbiosOutputMode);
  }

  return Status;
}

//-----------------------------------------------------------------------------------------------
/**

Arguments:

  Host        - Pointer to the system Host (root) structure
  ch          - DDR Channel ID

  @retval 0 - success
  @retval 1 - failure

**/
UINT32
WriteFnvCfgEmrs (
                PSYSHOST Host,
                UINT8  socket,
                UINT8  ch,
                UINT8  dimm,
                UINT32 reg,
                UINT32 data
                )

{
  UINT8                   regSize = 0;
  UINT32                  smbReg;
  UINT32                  status = SUCCESS;
  UINT32                  localData = data;
  UINT8                   *dataPtr = (UINT8 *)&localData;
  CSR_OFFSET              RegOffset;
  UINT8                   i;
  UINT8                   timingModeOrg;
  struct channelNvram     (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(Host, socket);

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "WriteFnvCfgEmrs Starts\n");
#endif //MRC_TRACE
  //
  // Set 3N timing and save original setting
  //
  timingModeOrg = GetCurrentTimingMode(Host, socket, ch);
  SetTimingMode (Host, socket, ch, (*channelNvList)[ch].trainTimingMode);

  RegOffset.Data = reg;
  regSize = (UINT8)RegOffset.Bits.size;
  smbReg = GetFnvCfgAddress (Host, FMC_SMB, reg);

  // Use little-endian ordering
  // Assume natural size alignment
  for (i = 0; i < regSize; i++) {

    // MRS command to bank 8 with address[11:0] = NVMCTLR CSR address [15:4]
    if (i == 0) {
      WriteMrsDdr4(Host, socket, ch, dimm, 0, ((smbReg + i) >> 4) & 0xFFF, 8, A_SIDE);
    }

    // MRS command to bank 9 with address[11:8] = NVMCTLR CSR address [3:0], NVMCTLR CSR data [7:0]
    WriteMrsDdr4(Host, socket, ch, dimm, 0, (((smbReg + i) & 0xF) << 8) | (*(dataPtr + i) & 0xFF), 9, A_SIDE);
  }
  //
  // Restore original CMD timing
  //
  SetTimingMode (Host, socket, ch, timingModeOrg);


  return status;
}

//-----------------------------------------------------------------------------------------------
/**

Arguments:

  Host        - Pointer to the system Host (root) structure
  ch          - DDR Channel ID

  @retval 0 - success
  @retval 1 - failure

**/
STATIC UINT32
DpaWrite64 (
            PSYSHOST Host,
            UINT8  socket,
            UINT8  ch,
            UINT8  dimm,
            UINT32 dpa,
            UINT64_STRUCT *data
            )

{
  UINT32 status = SUCCESS;
  UINT8  oldValue = 0;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  // Disable assembly for NVMDIMM x'tor
  if (UbiosGenerationEnabled ()) {
    if (Setup->common.ddrtXactor == 1) {
      oldValue = GetUbiosOutputMode ();
      SetUbiosOutputMode (ASM_OUTPUT_DISABLE);
    }
    SetUbiosFnvAccessValue (FNV_ACCESS_ENABLE);
  }

  // Setup CPGC sequence
  CpgcSetup64(Host, socket, ch, dimm, dpa, WRITE_ACCESS, 1, data);

  // IO reset
  IO_Reset(Host, socket);

  // Start test
  CpgcGlobalTestStart (Host, socket, (1 << ch), NO_SUBCH);

  // Poll test completion
  CpgcPollGlobalTestDone (Host, socket, (1 << ch), NO_SUBCH);


  // Re-enable assembly for NVMDIMM x'tor
  if (UbiosGenerationEnabled ()) {
    if (Setup->common.ddrtXactor == 1) {
      SetUbiosOutputMode (oldValue);
    }
    SetUbiosFnvAccessValue (FNV_ACCESS_DISABLE);
  }

  return status;
}

//-----------------------------------------------------------------------------------------------
/**

Arguments:

  Host        - Pointer to the system Host (root) structure
  ch          - DDR Channel ID

  @retval 0 - success
  @retval 1 - failure

**/
STATIC UINT32
DpaRead64 (
           PSYSHOST Host,
           UINT8  socket,
           UINT8  ch,
           UINT8  dimm,
           UINT32 dpa,
           UINT64_STRUCT *data
           )

{
  UINT8  LaneErrorStatus[MAX_BITS / 8];
  UINT32 status = SUCCESS;
  UINT64_STRUCT localData;
  UINT8  oldValue = 0;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  // Disable the assembly for NVMDIMM x'tor
  if (UbiosGenerationEnabled ()) {
    if (Setup->common.ddrtXactor == 1) {
      oldValue = GetUbiosOutputMode ();
      SetUbiosOutputMode (ASM_OUTPUT_DISABLE);
    }
    SetUbiosFnvAccessValue (FNV_ACCESS_ENABLE);
  }

  localData.lo = 0;
  localData.hi = 0;

  // Setup CPGC sequence
  CpgcSetup64(Host, socket, ch, dimm, dpa, READ_ACCESS, 1, &localData);

  // IO reset
  IO_Reset(Host, socket);

  // Start test
  CpgcGlobalTestStart (Host, socket, (1 << ch), NO_SUBCH);

  // Poll test completion
  CpgcPollGlobalTestDone (Host, socket, (1 << ch), NO_SUBCH);

  // Collect test results
  BiosServerGetLaneErrorStatusChip (Host, socket, ch, BIT0, LaneErrorStatus);
  data->lo = *(UINT32 *)LaneErrorStatus;
  data->hi = *(UINT32 *)(LaneErrorStatus + 4);

    // Re-enable assembly for NVMDIMM x'tor
  if (UbiosGenerationEnabled ()) {
    if (Setup->common.ddrtXactor == 1) {
      SetUbiosOutputMode (oldValue);
    }
    SetUbiosFnvAccessValue (FNV_ACCESS_DISABLE);
  }

  return status;
}

//-----------------------------------------------------------------------------------------------
/**

Arguments:

  Host        - Pointer to the system Host (root) structure
  ch          - DDR Channel ID

**/
VOID
FnvBcwRead (
           PSYSHOST Host,
           UINT8  socket,
           UINT8  ch,
           UINT8  dimm,
           UINT32 dpa,
           UINT8 data[MAX_STROBE/2]
           )
{
  UINT8 ui, byte;
  UINT8 uiBitMask;
  UINT8 LaneErrorStatus[MAX_BITS / 8];
  UINT64_STRUCT localData;

  //Clear WDB
  SetPatternBuffers (Host, socket, MEMINIT, 0);

  // Initialize results to 0
  for (byte = 0; byte < 9; byte++) {
    data[byte] = 0;
  }

  // For each UI of the burst
  for (ui = 0; ui < 8; ui++) {

    localData.lo = 0;
    localData.hi = 0;

    // UI0 = uiBitMask 0x01 and early
    // UI1 = uiBitMask 0x01 and late
    // UI2 = uiBitMask 0x02 and early
    // UI3 = uiBitMask 0x02 and late
    // UI4 = uiBitMask 0x04 and early
    // UI5 = uiBitMask 0x04 and late
    // UI6 = uiBitMask 0x08 and early
    // UI7 = uiBitMask 0x08 and late
    uiBitMask = (BIT0 << (ui >> 1));

    // Setup CPGC sequence
    CpgcSetup64(Host, socket, ch, dimm, dpa, READ_ACCESS, uiBitMask, &localData);

    // IO reset
    IO_Reset(Host, socket);

    // Start test
    CpgcGlobalTestStart (Host, socket, (1 << ch), NO_SUBCH);

    // Poll test completion
    CpgcPollGlobalTestDone (Host, socket, (1 << ch), NO_SUBCH);

    // Collect test results
    BiosServerGetLaneErrorStatusChip (Host, socket, ch, BIT0 << ui, LaneErrorStatus);

    // Update the results
    for (byte = 0; byte < 9; byte++) {
      data[byte] |= ((LaneErrorStatus[byte] & BIT0) << (7 - ui));
    }
  }
} // FnvBcwRead

//-----------------------------------------------------------------------------------------------
/**

Arguments:

  Host        - Pointer to the system Host (root) structure
  ch          - DDR Channel ID

  @retval 0 - success
  @retval 1 - failure

**/
UINT32
ReadFnvCfgDpa (
              PSYSHOST Host,
              UINT8  socket,
              UINT8  ch,
              UINT8  dimm,
              UINT32 reg,
              UINT64_STRUCT *data
              )

{
  UINT32 regSize = 0;
  CSR_OFFSET RegOffset;
  UINT64_STRUCT dataMask;
  UINT64_STRUCT localData;
  UINT32 dpa;
  UINT8 regIndex;
  UINT64_STRUCT temp;
  UINT32 status = SUCCESS;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "ReadPmemCfgDpa Starts\n");

  RegOffset.Data = reg;
  regSize = RegOffset.Bits.size;
  regIndex = (RegOffset.Bits.offset & 0x7) * 8;
  dpa = GetFnvCfgAddress (Host, FMC_DPA, reg);

  // If size < 8, Read64, mask size, or data, Write64
  if (regSize) {

    //dataMask = ((1ULL << (8*regSize)) - 1) << regIndex;
    temp.lo = 1;
    temp.hi = 0;
    dataMask = LShiftUINT64(SubUINT64(LShiftUINT64(temp, 8 * regSize), temp), regIndex);

    DpaRead64(Host, socket, ch, dimm, dpa, &localData);

    //localData = (localData & dataMask) >> regIndex;
    temp.lo = localData.lo & dataMask.lo;
    temp.hi = localData.hi & dataMask.hi;
    localData = RShiftUINT64(temp, regIndex);

    if (regSize == 4) {
      *(UINT32 *)data = localData.lo;
    } else if (regSize == 2) {
      *(UINT16 *)data = (UINT16)localData.lo;
    } else {
      *(UINT8 *)data = (UINT8)localData.lo;
    }
  } else {
    DpaRead64(Host, socket, ch, dimm, dpa, data);
  }

  return status;
}

//-----------------------------------------------------------------------------------------------
UINT32 WriteFnvCfgDpa (
                      PSYSHOST Host,
                      UINT8  socket,
                      UINT8  ch,
                      UINT8  dimm,
                      UINT32 reg,
                      UINT64_STRUCT data
                      )
/**

Arguments:

  Host        - Pointer to the system Host (root) structure
  ch          - DDR Channel ID

  @retval 0 - success
  @retval 1 - failure

**/

{
  UINT32 regSize = 0;
  UINT32 regIndex = 0;
  CSR_OFFSET RegOffset;
  UINT64_STRUCT dataMask;
  UINT64_STRUCT localData;
  UINT32 dpa;
  UINT32 status = SUCCESS;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "WriteFnvCfgDpa Starts\n");

  RegOffset.Data = reg;
  regSize = RegOffset.Bits.size;
  regIndex = RegOffset.Bits.offset & 0x7;
  dpa = GetFnvCfgAddress (Host, FMC_DPA, reg);

  // If size < 8, Read64, mask size, or data, Write64
  if (regSize) {
    dataMask.lo = 0xffffffff;
    dataMask.hi = 0xffffffff;
    // getting mask for the value being written
    dataMask = LShiftUINT64(dataMask, (8 * regSize));
    dataMask.lo = ~dataMask.lo;
    dataMask.hi = ~dataMask.hi;

    data.lo &= dataMask.lo; // masking LO of the value
    data.hi &= dataMask.hi; // masking HI of the value
    data = LShiftUINT64(data, (8 * regIndex)); // shifting the value to the offset of the CSR

    // getting mask for the value being read
    dataMask = LShiftUINT64(dataMask, (8 * regIndex));
    dataMask.lo = ~dataMask.lo;
    dataMask.hi = ~dataMask.hi;

    DpaRead64(Host, socket, ch, dimm, dpa, &localData);
    localData.lo = (localData.lo & dataMask.lo) | data.lo;
    localData.hi = (localData.hi & dataMask.hi) | data.hi;

    DpaWrite64(Host, socket, ch, dimm, dpa, &localData);
  } else {
    DpaWrite64(Host, socket, ch, dimm, dpa, &data);
  }

  return status;
}

//#define RC_SIM_TEST  0
//-----------------------------------------------------------------------------------------------
/**

    NVMCTLR hook for NVMDIMM
    Enable NVMDIMM training mode, Test SMBus / DPA / CPGC interfaceS

    @param Host   - Pointer to the system Host (root) structure
    @param socket - Processor socket ID

    @retval None

**/
VOID
EarlyFnvConfigAccess (
                     PSYSHOST                  Host,
                     UINT8                     socket
                     )
{
  UINT8 ch;
  UINT8 dimm;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                    (*dimmNvList)[MAX_DIMM];
  UINT8                               MaxChDdr;

  if (Host->nvram.mem.socket[socket].DcpmmPresent == 0) {
    return;
  }
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "EarlyFnvConfigAccess Starts\n");
  //
  // Assume that enough time has elapsed for NVMCTLR power-good
  // so that we can safely use SMBus interface
  //
  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr      = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Update NVMCTLR access mode
    //
    SetFmcAccessMode (socket, ch, SMBUS);

    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }

    } //dimm loop
  } //ch loop

  return;
}

/**

    Falcon Valley hook after DDR training
    This routine disables NVMDIMM trainingmode, setup scramble seed

    @param Host   - Pointer to the system Host (root) structure
    @param socket - Processor socket ID

    @retval None

**/
VOID
LateFnvConfigAccess (
                     PSYSHOST                  Host,
                     UINT8                     socket
                     )
{
  UINT8 ch;
  UINT8 dimm;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  struct dimmNvram                    (*dimmNvList)[MAX_DIMM];
  DA_WR_SCRAMBLE_SEED_SEL_FNV_DA_UNIT_0_STRUCT  txSeedSel;
  DA_WR_SCR_LFSR_L_FNV_DA_UNIT_0_STRUCT         txSeedLo;
  DA_RD_SCR_LFSR_FNV_DA_UNIT_0_STRUCT           rxSeed;
  BootMode                                      SysBootMode;
  SYS_SETUP                                     *Setup;
  UINT8                                         MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  SysBootMode = GetSysBootMode ();
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "LateFnvConfigAccess Starts\n");
  //
  // Disable NVMDIMM training mode
  //
  SetAepTrainingModeSMB (Host,socket, DISABLE_TRAINING_MODE);

  //
  // setting gnt_32_cont_en = 0 to disabled for normal mode
  //
  Grant32ContEn (Host, socket, 0);

  channelNvList = GetChannelNvList(Host, socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }

      if (Setup->mem.options & SCRAMBLE_EN_DDRT) {

        // Program Scrambling
        txSeedSel.Data = 0;
        txSeedLo.Data = 0;
        rxSeed.Data = 0;

        //Initialize seeds
        txSeedSel.Bits.ordersel = Host->nvram.mem.scrambleSeed;
        rxSeed.Bits.seed = 0xFFFF;

        WriteFnvCfg (Host, socket, ch, dimm, DA_WR_SCRAMBLE_SEED_SEL_FNV_DA_UNIT_0_REG, txSeedSel.Data);
        WriteFnvCfg (Host, socket, ch, dimm, DA_WR_SCR_LFSR_L_FNV_DA_UNIT_0_REG, txSeedLo.Data);
        WriteFnvCfg (Host, socket, ch, dimm, DA_RD_SCR_LFSR_FNV_DA_UNIT_0_REG, rxSeed.Data);

        // Ensure 0->1 transition to load seeds
        txSeedLo.Bits.lfsr_load = 1;
        rxSeed.Bits.lfsr_load = 1;

        WriteFnvCfg (Host, socket, ch, dimm, DA_WR_SCR_LFSR_L_FNV_DA_UNIT_0_REG, txSeedLo.Data);
        WriteFnvCfg (Host, socket, ch, dimm, DA_RD_SCR_LFSR_FNV_DA_UNIT_0_REG, rxSeed.Data);

        txSeedLo.Bits.en = 1;
        rxSeed.Bits.en = 1;

        WriteFnvCfg (Host, socket, ch, dimm, DA_WR_SCR_LFSR_L_FNV_DA_UNIT_0_REG, txSeedLo.Data);
        WriteFnvCfg (Host, socket, ch, dimm, DA_RD_SCR_LFSR_FNV_DA_UNIT_0_REG, rxSeed.Data);

        SetScrambleConfigMc (Host, socket, ch, SCRAMBLE_CONFIG_LATE_FNV_CONFIG);

      }
    } //dimm loop

    if (UbiosGenerationOrHsleEnabled () &&
        (Setup->common.ddrtXactor == 0))
    {
      // Update NVMCTLR access mode. This is done a first time to reduce the SMBUS accesses in FnvDdrtIoInit (below).
      SetFmcAccessMode (socket, ch, CPGC);
    }

  } //ch loop

  if (SysBootMode == NormalBoot && (
        Host->var.mem.subBootMode == ColdBoot ||
        Host->var.mem.subBootMode == ColdBootFast))
  {
    FnvDdrtIoInit (Host, socket, DDRT_TRAINING_COMPLETE);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if (UbiosGenerationOrHsleEnabled () &&
        (Setup->common.ddrtXactor == 0))
    {
      // Update NVMCTLR access mode. This is done a second time to revert SMBUS accesses that came about in
      // FnvDdrtIoInit (above).
      SetFmcAccessMode (socket, ch, CPGC);
    }

    //
    // Shutoff clock for Channel which is disabled
    //
    if ((IsDcpmmPresentChannel (socket, ch)) && (IsChannelEnabled (socket, ch) == FALSE)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Shut off clock for disabled channel %d/n", ch);
      SetRankEnDdrio (Host, socket, ch, 0);
    }
  } // for (ch)

} // LateFnvConfigAccess


/**

Routine Description: ReadFnvCfg will read a NVMCTLR register via SMBus or CPGC interface.
This will return the data read from a NVMCTLR register because ReadFnvCfgSmb and ReadFnvCfgDpa require different size of data types
If no interface is supported, then enter RC_FATAL_ERROR

  @param Host        - Pointer to the system Host (root) structure
  @param socket      - Socket Number
  @param ch          - DDR Channel ID
  @param dimm        - DIMM number
  @param mode        - Type of interface SMBus, EMRS, or CPGC
  @param reg         - NVMCTLR register Address
@retval Data from NVMCTLR register

**/
UINT32
ReadFnvCfg (
              IN  PSYSHOST  Host,
              IN  UINT8     Socket,
              IN  UINT8     Ch,
              IN  UINT8     Dimm,
              IN  UINT32    Reg
)
{
  UINT8                  CacheIndex;
  UINT64_STRUCT          LocalData = {0,0};
  BUS_TYPE               Mode;
  struct channelNvram    (*ChannelNvList)[MAX_CH];
  struct FmcCacheSt      (*FmcCache)[MAX_FMC_CACHE];

  FmcCache = &Host->var.mem.socket[Socket].channelList[Ch].dimmList[Dimm].FmcCache;

  ChannelNvList = GetChannelNvList (Host, Socket);
  Mode = GetFmcAccessMode (Socket, Ch);

  if ((FeaturePcdGet (PcdCosimBuild) == TRUE) && (Mode == CPGC)) {
    Mode = SMBUS;
  }

  CacheIndex = GetFMCCacheIndex (Host, Socket, Ch, Dimm, Reg);

  if ((CacheIndex < MAX_FMC_CACHE) && ((*FmcCache)[CacheIndex].Status & ENTRY_VALID) && (Host->nvram.mem.FmcCacheDone == 0)) {
    LocalData.lo = (*FmcCache)[CacheIndex].Data;
    Host->var.mem.FmcCachedReads++;
  } else {
    switch (Mode) {
      default:
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "Invalid FNV Access mode.  Using SMBUS\n");
        EwlOutputType1 (WARN_FNV_ACCESS, WARN_INVALID_FNV_ACCESS_MODE, Socket, Ch, Dimm, NO_RANK);
      case SMBUS:
      case EMRS:
        ReadFnvCfgSmb (Host, Socket, Ch, Dimm, Reg, &LocalData.lo);
        break;
      case CPGC:
        ReadFnvCfgDpa (Host, Socket, Ch, Dimm, Reg, &LocalData);
        break;
    }

    if (CacheIndex < MAX_FMC_CACHE) {
      (*FmcCache)[CacheIndex].Data = LocalData.lo;
      (*FmcCache)[CacheIndex].Status |= ENTRY_VALID;
    }
  }
  return LocalData.lo;
}


/**

Routine Description:WriteFnvCfg will write 32 bits of data into a NVMCTLR register.
If no interface is supported, then enter RC_FATAL_ERROR

  @param Host        - Pointer to the system Host (root) structure
  @param socket      - Socket Number
  @param ch          - DDR Channel ID
  @param dimm        - DIMM number
  @param mode        - Type of interface SMBUS, EMRS, or CPGC
  @param reg         - NVMCTLR register Address
  @param data        - Data to be written to NVMCTLR register
  @retval 0 - success

**/
UINT32
WriteFnvCfg (
              IN  PSYSHOST  Host,
              IN  UINT8     Socket,
              IN  UINT8     Ch,
              IN  UINT8     Dimm,
              IN  UINT32    Reg,
              IN  UINT32    Data
)
{
  UINT8                  CacheIndex;
  UINT64_STRUCT          LocalData;
  BUS_TYPE               Mode;
  struct channelNvram    (*ChannelNvList)[MAX_CH];
  struct FmcCacheSt      (*FmcCache)[MAX_FMC_CACHE];

  FmcCache = &Host->var.mem.socket[Socket].channelList[Ch].dimmList[Dimm].FmcCache;

  ChannelNvList = GetChannelNvList (Host, Socket);

  CacheIndex = GetFMCCacheIndex (Host, Socket, Ch, Dimm, Reg);

  if (CacheIndex < MAX_FMC_CACHE) {
    (*FmcCache)[CacheIndex].Data = Data;
    (*FmcCache)[CacheIndex].Status |= ENTRY_VALID;
  }

  Mode = GetFmcAccessMode (Socket, Ch);

  if ((FeaturePcdGet (PcdCosimBuild) == TRUE) && (Mode == CPGC)) {
    Mode = SMBUS;
  }

  LocalData.lo = Data;
  switch (Mode) {
    default:
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "Invalid FNV Access mode.  Using SMBUS\n");
      EwlOutputType1 (WARN_FNV_ACCESS, WARN_INVALID_FNV_ACCESS_MODE, Socket, Ch, Dimm, NO_RANK);
    case SMBUS:
      WriteFnvCfgSmb (Host, Socket, Ch, Dimm, Reg, Data);
      break;
    case EMRS:
      WriteFnvCfgEmrs (Host, Socket, Ch, Dimm, Reg, Data);
      break;
    case CPGC:
      WriteFnvCfgDpa (Host, Socket, Ch, Dimm, Reg, LocalData);
      break;
  }
  return SUCCESS;
}


VOID EmulationReadFnvCfgSmbus (
                          PSYSHOST Host,
                          UINT8    socket,
                          UINT8    ch,
                          UINT8    dimm,
                          UINT32   reg,
                          UINT32   mask,
                          UINT8    logic
)
{
  UINT32     LabelNumber;
  UINT32     data;
  BOOLEAN    IsPostResetPowerGood;

  if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
    LabelPrint (LabelNumber);
    SetUbiosLoopbackLabelNumber (LabelNumber + 1);
    ReadFnvCfgSmb (Host, socket, ch, dimm, reg, &data);
    IsPostResetPowerGood = CheckCurrentReset (POST_RESET_POWERGOOD);
    if (logic == 0) {
      RcDebugPrint (SDBG_DEFAULT, "\nnot ecx\n");
    }
    RcDebugPrint (SDBG_DEFAULT, "and ecx, %8xh\n", mask);
    RcDebugPrint (SDBG_DEFAULT, "cmp ecx, 0%08xh\n", mask);
    if (IsPostResetPowerGood) {
      RcDebugPrint (SDBG_DEFAULT, "jnz LABEL_c_mbox%d\n", LabelNumber);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "jnz LABEL_w_mbox%d\n", LabelNumber);
    }
  }
}


VOID EmulationReadFnvCfg (
                          PSYSHOST Host,
                          UINT8    socket,
                          UINT8    ch,
                          UINT8    dimm,
                          UINT32   reg,
                          UINT32   mask,
                          UINT8    logic
)
{
  UINT8     *RegAddr;
  UINT32    LabelNumber;

  RegAddr = GetCpgcErrData0sAddr (Host, socket, ch);

  if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
    LabelPrint (LabelNumber);
    SetUbiosLoopbackLabelNumber (LabelNumber + 1);
    ReadFnvCfg (Host, socket, ch, dimm, reg);
    RcDebugPrint (SDBG_DEFAULT, "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr);
    if (logic) {
      MaskPrint (LabelNumber);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "\nnot eax\n");
      MaskPrint (LabelNumber);
    }
  }
}

/**

  Routine Description: EmulationPoll is an assembly polling loop for reading CSRs
  EmulationPoll allows you to poll multiple bits

  @param Host        - Pointer to the system Host (root) structure
  @param socket      -
  @param BoxInst     -
  @param byteOffset  -
  @param bit         - bit we are polling
  @param logic       - polling for 0 or 1

  @retval 0 - success
  @retval 1 - failure

**/


VOID
EmulationPoll (PSYSHOST Host,
               UINT8    socket,
               UINT8    BoxInst,
               UINT32   Offset,
               UINT32   mask,
               UINT8    logic
               )
{
  CSR_OFFSET RegOffset;
  UINT8      *RegAddr;
  UINT8      size;
  UINT32     LabelNumber;

  RegOffset.Data = Offset;
  size = (UINT8) RegOffset.Bits.size;
  RegAddr = (UINT8 *) UsraGetCsrRegisterAddress (socket, BoxInst, Offset);
  if (GetUbiosLoopbackLabelNumber (&LabelNumber) == EFI_SUCCESS) {
    LabelPrint (LabelNumber);
    SetUbiosLoopbackLabelNumber (LabelNumber + 1);
    RcDebugPrint (SDBG_DEFAULT, "mov eax, DWORD PTR ds:[0%8xh]\n", RegAddr);
    if (logic) {
      MaskPrint (LabelNumber);
    } else {
      RcDebugPrint (SDBG_DEFAULT, "not eax\n");
      MaskPrint (LabelNumber);
    }
  }
}

/**

Routine Description: Polls on the fmc for read or write
                     transaction complet.

**/
MRC_STATUS
FmcPollForResponse (
    PSYSHOST Host,
    UINT8    Socket,
    UINT8    Ch,
    UINT8    Dimm
    )
{
  D_MC_STAT_FNV_D_UNIT_0_STRUCT   WindowStatus;
  UINT64                          StartCount;
  UINT32                          Timeout;

  //
  // Set Timeout
  //
  if (IsSimics ()) {
    Timeout = FMC_RESPONSE_TIMEOUT_MICROSECONDS_SIMICS;
  } else {
    Timeout = FMC_RESPONSE_TIMEOUT_MICROSECONDS_HARDWARE;
  }
  StartCount = GetCount ();

  do {
    WindowStatus.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, D_MC_STAT_FNV_D_UNIT_0_REG);

    if ((FeaturePcdGet (PcdCosimBuild) == FALSE) && (GetDelay (StartCount) >= Timeout)) {
      // Log Warning
      return MRC_STATUS_FAILURE;
    }

    if ((FeaturePcdGet (PcdCteBuild) == TRUE) && (FeaturePcdGet (PcdCosimBuild) == FALSE)) {
      WindowStatus.Bits.mc_resp = 1;
    }

    if (UbiosGenerationOrHsleEnabled ()) {
      WindowStatus.Bits.mc_resp = 1;
    }
  } while (!WindowStatus.Bits.mc_resp); // Poll for Read Data Valid Bit

  if (WindowStatus.Bits.mc_rdata_src != 0) {
    // Log Warning if you read from DA register
  }
  return MRC_STATUS_SUCCESS;
}
/**

Routine Description: This is a generic Write to IO

  1. Write Data into Window Register MC_WDATA for Write only
  2. Write Bus, Device, and Function into Account Info
     Bits   [11]   - IA
     Bits   [10:8] - Bus - (Contrary to what the documentation may say, this is 2 for both NGN and NVMDIMM IO)
     Bits   [7:3]  - Device - Found in CSR description document (CSPEC/XML).
     Bits   [2:0]  - Function - Found in CSR description document (CSPEC/XML).
  3. Configure control Register
  4. Wait for completion for reads and nonposted writes
  @param Host        - Pointer to the system Host (root) structure
  @retval Data or Status depending on Read or Write

**/
UINT32
GetSetFnvIO (
              PSYSHOST Host,
              UINT8    socket,
              UINT8    ch,
              UINT8    dimm,
              UINT32   reg,
              UINT32   *data,
              UINT8    IA,
              UINT8    mode,
              UINT8    byteMask // 0xF
)
{
  CSR_OFFSET                          RegOffset;
  UINT8                               func;
  UINT8                               device;
  D_MC_ACC_INFO_FNV_D_UNIT_0_STRUCT   windowAccountInfo;
  D_MC_CTRL_FNV_D_UNIT_0_STRUCT       windowControl;
  MRC_STATUS                          Status;
  SYS_SETUP                           *Setup;

  Setup = GetSysSetupPointer ();

  Status = MRC_STATUS_SUCCESS;

  if (UbiosGenerationOrHsleEnabled ()) {
    //
    //For uBIOS, enable FMC/FNV access only for co-emulation and skip for emulation
    //
    if (Setup->common.ddrtXactor == 1) {
      return MRC_STATUS_SUCCESS;
    }
  }

  //
  // 1. Write Data into Window Register MC_WDATA for Write only
  //
  if (mode == FNV_IO_WRITE) {
    WriteFnvCfg (Host, socket, ch, dimm, D_MC_WDATA_FNV_D_UNIT_0_REG, *data);
  }

  //
  // 2. Write IA, Bus, Device, and Function into Account Info
  //
  RegOffset.Data = reg;
  windowAccountInfo.Data = 0;
  device = 22;
  switch(RegOffset.Bits.funcblk) {
    case 0x0E:
      func = 0;
      break;
    case 0x0F:
      func = 1;
      break;
   // case 0x10:
    default:
      func = 6;
      break;
  }
  windowAccountInfo.Bits.mc_acc_info = func| (device << 3) | (2 << 8) | (IA << 11); // Added place holder
  WriteFnvCfg (Host, socket, ch, dimm, D_MC_ACC_INFO_FNV_D_UNIT_0_REG, windowAccountInfo.Data);

  //
  // 3. Configure Control CSR
  //
  windowControl.Data = 0;
  windowControl.Bits.mc_opcode = mode; // Read = 0 Write = 1
  windowControl.Bits.mc_port = 0x71;   // Port ID for IO CSR is 0x71
  if (mode == FNV_IO_WRITE) {
    windowControl.Bits.mc_posted = 1;  // Write posted set to 0 for now. Add lookup Table Later
  } else {
    windowControl.Bits.mc_posted = 0;  // Reads are always nonposted
  }
  windowControl.Bits.mc_addr = (RegOffset.Bits.offset >> 2) | (func << 10); // Address bits <11:2>
  windowControl.Bits.mc_be = byteMask; // Byte Enable Mask Field
  windowControl.Bits.mc_start = 0x01;  // Kick off the command
  WriteFnvCfg (Host, socket, ch, dimm, D_MC_CTRL_FNV_D_UNIT_0_REG, windowControl.Data);

  //
  // 4. Wait for completion for reads and nonposted writes
  //
  if (windowControl.Bits.mc_posted == 0) {
    if (mode == FNV_IO_READ) {
      Status = FmcPollForResponse (Host, socket, ch, dimm);
      if (Status != MRC_STATUS_SUCCESS) {
        return Status;
      }
      *data = ReadFnvCfg (Host, socket, ch, dimm, D_MC_RDATA_FNV_D_UNIT_0_REG);

      return SUCCESS;
    } else {
      Status = FmcPollForResponse (Host, socket, ch, dimm);
      if (Status != MRC_STATUS_SUCCESS) {
        return Status;
      }
    }
  } // nonposted

  if (mode == FNV_IO_WRITE) {

  }
  return MRC_STATUS_SUCCESS;
} // GetSetFnvIO

/**

  Routine Description: This function reads the Major and Minor Checkpoint of NGN training
  Bits  7 - 0 represent the Major Checkpoint
  Bits 15 - 8 represent the Minor Checkpoint
  @param Host        - Pointer to the system Host (root) structure
  @retval Data or Status depending on Read or Write
**/
UINT32
GetFnvTrainingStatus (
                      PSYSHOST Host,
                      UINT8    socket,
                      UINT8    ch,
                      UINT8    dimm
)
{
  D_FW_STATUS_FNV_D_UNIT_0_STRUCT fwStatusReg;

  fwStatusReg.Data = ReadFnvCfg(Host, socket, ch, dimm, D_FW_STATUS_FNV_D_UNIT_0_REG);
  return (fwStatusReg.Data & (MAJOR_CHECKPOINT | MINOR_CHECKPOINT));
} // GetFnvTrainingStatus

/**
  Check if training has failed on previous boot

  @param[in]  Socket                - Socket Number

  @retval                           - MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
CheckDdrtIoTrainingStatus (
  IN  UINT8                    Socket
)
{
  UINT8                     Ch;
  UINT8                     Dimm;
  BOOT_STATUS_REGISTER      Bsr;
  BOOT_STATUS_HIGH_REGISTER BsrHi;
  CHANNEL_NVRAM_STRUCT      (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT         (*DimmNvList)[MAX_DIMM];
  PSYSHOST                  Host;
  UINT8                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Bsr.Data = 0;
  BsrHi.Data = 0;
  Host = (PSYSHOST) GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < (*ChannelNvList)[Ch].maxDimm; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }

      if (Host->var.mem.socket[Socket].channelList[Ch].FisVersion[Dimm] >= FIS_114_VERSION) {
        ReadBootStatusRegisters (Host, Socket, Ch, Dimm, &Bsr, &BsrHi);

        if (BsrHi.Bits.DdrtIoInitTrainingStatus == 2) {
          RcDebugPrint (SDBG_MINMAX, "Training did not complete on previous boot. Mapping out channel\n");
          EwlOutputType1 (WARN_FNV_BSR, WARN_DR_READY_ERROR, Socket, Ch, NO_DIMM, NO_RANK);
          DisableChannelSw (Host, Socket, Ch);
        }
      } else {
        RcDebugPrint (SDBG_MINMAX, "Please upgrade FW to 1.14 FIS or greater\n");
      }
    }
  }
  return MRC_STATUS_SUCCESS;
}

/**
  Routine Description: This function returns a random 32 bit number
  @param Host    - pointer to System Host (root) structure
  @param EaxReg  - data
  1. Check if processor supports RDRAND
  2. Call rdrand
  3. Check Carry Flag to see if rdrand failed
  @retval status
**/
UINT32
RdRand32(
       PSYSHOST Host,
       UINT32*  EaxReg
)
{
#ifdef IA32
#ifdef __GNUC__
  asm (
            "movl    %0, %%esi\n\t"
    "repeat:\n\t"
            ".byte   $0x0f\n\t"
            ".byte   $0xc7\n\t"
            ".byte   $0xf0\n\t"
            "jnc     repeat\n\t"
            "movl    %%eax, (%%esi)\n\t"
            : : "r"(EaxReg) : "%esi"
  );
#else
  _asm {
            mov     esi, EaxReg
    repeat:
            _emit   0x0f
            _emit   0xc7
            _emit   0xf0
            jnc     repeat
            mov     dword ptr [esi], eax
  }
#endif
#endif
  return SUCCESS;
}


/**

Calls to gather AEP important SPD Data(list as follows) in case of Channel/Dimm map out before GatherSPDData be executed
- SPD_VENDORID_BYTE0 320
- SPD_VENDORID_BYTE1 321
- SPD_DEVICEID_BYTE0 192
- SPD_DEVICEID_BYTE1 193
- SPD_RID 349
- SPD_SUBSYSTEM_VENDORID_BYTE0 194
- SPD_SUBSYSTEM_VENDORID_BYTE1 195
- SPD_SUBSYSTEM_DEVICEID_BYTE0 196
- SPD_SUBSYSTEM_DEVICEID_BYTE1 197
- SPD_SUBSYSTEM_REVISIONID 198
- SPD_MM_LOC_DDR4       322   // Module Manufacturing Location
- SPD_MM_DATE_YR_DDR4   323   // Module Manufacturing Date Year
- SPD_MM_DATE_WK_DDR4   324   // Module Manufacturing Date Week
- SPD_MODULE_SN_DDR4    325   // Module Serial Number 325-328

@param host   - Point to sysHost
@param Socket - Point to Socket
@param Ch     - Point to Channel
@param Dimm   - Point to Dimm

**/
VOID
GatherDdrtSPDData (
  IN  SYSHOST                 *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm
)
{
  UINT8               SPDReg;
  UINT8  ngnCapacity;      // NVMDIMM capacity
  UINT8  ngnBusWidth;      // NVMDIMM bus width
  UINT8  ngnDeviceType;    // NVMDIMM device type
  UINT8  ngnModuleOrg;     // NVMDIMM module organization
  UINT8  ti;
  UINT32 size;
  UINT8               i;
  BOOLEAN  IsOlderSpdRevsion = FALSE;
  struct dimmNvram(*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];

  dimmNvList = GetDimmNvList(Host, Socket, Ch);
  channelNvList = GetChannelNvList (Host, Socket);

  //
  // Get NGN Capacity
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_DEV_DENSITY, &SPDReg);
  ngnCapacity = SPDReg;
  (*dimmNvList)[Dimm].NvmDevDensity = SPDReg;

  //
  // Get NGN Number of ranks and device width
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_MOD_ORGANIZATION, &SPDReg);
  ngnModuleOrg = SPDReg;

  //
  // Get NGN Device Type
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_DEV_TYPE, &SPDReg);
  ngnDeviceType = SPDReg;
  (*dimmNvList)[Dimm].NvmDevType = SPDReg;

  //
  // Get NGN Bus Width
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_MOD_BUS_WIDTH, &SPDReg);
  ngnBusWidth = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_WRITE_CREDIT, &SPDReg);
  (*channelNvList)[Ch].FmcWrCreditLimit = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_READ_CREDIT, &SPDReg);
  (*channelNvList)[Ch].FmcRdCreditLimit = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_LSB_FMC_TYPE, &SPDReg);
  (*dimmNvList)[Dimm].fmcType = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_NVM_STEPPING, &SPDReg);
  (*dimmNvList)[Dimm].NvmStepping = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_MOD_REVISION, &SPDReg);
  (*dimmNvList)[Dimm].FmcModRev.Data = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_FMC_REV, &SPDReg);
  (*dimmNvList)[Dimm].fmcRev = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_BACKSIDE_SWIZZLE_0, &SPDReg);
  (*dimmNvList)[Dimm].lrbufswizzle = SPDReg;
  SpdReadByte (Socket, Ch, Dimm, SPD_AEP_BACKSIDE_SWIZZLE_1, &SPDReg);
  (*dimmNvList)[Dimm].lrbufswizzle |= SPDReg << 8;

  //
  // BPS rev 0.1 is equivalent to AEP 1.2, need drop support the older SPD rev.
  //
  switch ((*dimmNvList)[Dimm].fmcType) {
    case FMC_FNV_TYPE:
    case FMC_EKV_TYPE:
      if ((*dimmNvList)[Dimm].SPDSpecRev < SPD_DCPMM_REV_1P2) {
        IsOlderSpdRevsion = TRUE;
      }
      break;
    case FMC_BWV_TYPE:
      if ((*dimmNvList)[Dimm].SPDSpecRev < SPD_DCPMM_REV_0P1) {
        IsOlderSpdRevsion = TRUE;
      }
      break;
    default:
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "SPD ERROR - Not identifed Pmem Module!\n");
      RC_FATAL_ERROR (FALSE, ERR_DIMM_COMPAT, ERR_MISMATCH_DIMM_TYPE);
      break;
  }
  if ((IsOlderSpdRevsion == TRUE) && !(IsSiliconWorkaroundEnabled ("S1507797143"))) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "SPD ERROR - Older SPD Revision detected, PMem Module not supported!\n");
    DisableChannelSw (Host, Socket, Ch);
    OutputWarning (ERR_DIMM_COMPAT, ERR_NVMDIMM_NOT_SUPPORTED, Socket, Ch, NO_DIMM, NO_RANK);
  }

  //
  // Bits 3-0: Individual NVM Die Density
  // Bits 2-0: Effective NVM Channel Data Width, bit2 is reserved, only used bit 1-0
  //
  size = CalculateDdrtDimmSize (aepDensity[ngnCapacity & 0xF], aepdataWidth[ngnBusWidth & (BIT0 | BIT1)], ngnDeviceType, ngnModuleOrg);

  for (ti = 0; ti < MAX_TECH_DDRT; ti++) {
    if (aepSizeGB[ti] == size) {
      break;
    }
  }

  if (ti >= MAX_TECH_DDRT) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "SPD ERROR - PMem DIMM not supported!\n");
    DisableChannelSw (Host, Socket, Ch);
    OutputWarning (ERR_DIMM_COMPAT, ERR_NVMDIMM_NOT_SUPPORTED, Socket, Ch, NO_DIMM, NO_RANK);
  }

  //
  // Save NVM size
  // Note: this is only a temporary workaround for Emulation and CTE execution
  // because NVMCTLR FW command mailbox is not available.
  //
  (*dimmNvList)[Dimm].aepTechIndex = ti;
  (*dimmNvList)[Dimm].AitDramPoll  = 0;   // AIT Dram polling disable

  if (((*dimmNvList)[Dimm].fmcType == FMC_EKV_TYPE) || ((*dimmNvList)[Dimm].fmcType == FMC_BWV_TYPE)) {
    //
    // BWV and EKV are always x4
    //
    (*dimmNvList)[Dimm].SPDModuleOrg = 0;
    (*dimmNvList)[Dimm].actSPDModuleOrg = 0;
    GetCommonDDR4DIMMConfig (Host, Socket, Ch, Dimm);
  } else {
    (*dimmNvList)[Dimm].SPDModuleOrg = 0;
    (*dimmNvList)[Dimm].actSPDModuleOrg = 0;
  }

  //Get VendorID, DeviceID, RID, SubSystemVendorID, SubSystemDeviceID, and SubSystem RID from SPD
  SpdReadByte (Socket, Ch, Dimm, SPD_VENDORID_BYTE1, &SPDReg);
  (*dimmNvList)[Dimm].VendorID = SPDReg;
  (*dimmNvList)[Dimm].VendorID = (*dimmNvList)[Dimm].VendorID << 8;
  SpdReadByte (Socket, Ch, Dimm, SPD_VENDORID_BYTE0, &SPDReg);
  (*dimmNvList)[Dimm].VendorID = (*dimmNvList)[Dimm].VendorID | SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_DEVICEID_BYTE1, &SPDReg);
  (*dimmNvList)[Dimm].DeviceID = SPDReg;
  (*dimmNvList)[Dimm].DeviceID = (*dimmNvList)[Dimm].DeviceID << 8;
  SpdReadByte (Socket, Ch, Dimm, SPD_DEVICEID_BYTE0, &SPDReg);
  (*dimmNvList)[Dimm].DeviceID = (*dimmNvList)[Dimm].DeviceID | SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_RID, &SPDReg);
  (*dimmNvList)[Dimm].RevisionID = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_SUBSYSTEM_VENDORID_BYTE1, &SPDReg);
  (*dimmNvList)[Dimm].SubsystemVendorID = SPDReg;
  (*dimmNvList)[Dimm].SubsystemVendorID = (*dimmNvList)[Dimm].SubsystemVendorID << 8;
  SpdReadByte (Socket, Ch, Dimm, SPD_SUBSYSTEM_VENDORID_BYTE0, &SPDReg);
  (*dimmNvList)[Dimm].SubsystemVendorID = (*dimmNvList)[Dimm].SubsystemVendorID | SPDReg;
  SpdReadByte (Socket, Ch, Dimm, SPD_SUBSYSTEM_DEVICEID_BYTE1, &SPDReg);
  (*dimmNvList)[Dimm].SubsystemDeviceID = SPDReg;
  (*dimmNvList)[Dimm].SubsystemDeviceID = (*dimmNvList)[Dimm].SubsystemDeviceID << 8;
  SpdReadByte (Socket, Ch, Dimm, SPD_SUBSYSTEM_DEVICEID_BYTE0, &SPDReg);
  (*dimmNvList)[Dimm].SubsystemDeviceID = (*dimmNvList)[Dimm].SubsystemDeviceID | SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_SUBSYSTEM_REVISIONID, &SPDReg);
  (*dimmNvList)[Dimm].SubsystemRevisionID = SPDReg;

  //
  // Get module mfg id from SPD 320-321.
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_MMID_MSB_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDMMfgId = (UINT16)(SPDReg << 8);
  SpdReadByte (Socket, Ch, Dimm, SPD_MMID_LSB_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDMMfgId |= (UINT16)(SPDReg &~BIT7);

  //
  // Get module module mfg location from SPD 322
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_MM_LOC_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDMMfLoc = SPDReg;

  //
  // Get module module date code from SPD 323-324.
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_MM_DATE_YR_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDModDate = (UINT16)(SPDReg);
  SpdReadByte (Socket, Ch, Dimm, SPD_MM_DATE_WK_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDModDate |= (UINT16)(SPDReg << 8);

  //
  // Get module Serial Number from SPD 325-328
  //
  for (i = 0; i < SPD_MODULE_SERIAL; i++) {
    SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_SN_DDR4 + i, &SPDReg);
    (*dimmNvList)[Dimm].SPDModSN[i] = SPDReg;
  }

  //
  // 64 Data lanes + 8 ECC lanes
  //
  (*dimmNvList)[Dimm].SPDMemBusWidth = 0x0B;
} //GatherDdrtSPDData


/**

Checks DQ swizzling and programs registers to account for it

@param Host    - Pointer to sysHost
@param socket  - Socket number
@param ch      - Channel number
@param entry   - TRUE when entering GetMargins, FALSE when exiting.

**/
VOID
CheckAndProgramDqSwizzleAep(
PSYSHOST  Host,
UINT8     socket
)
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT32                            rxDqSwz[5];
  UINT32                            txDqSwz[5];
  UINT8                             i;
  UINT8                             j;
  struct channelNvram(*channelNvList)[MAX_CH];
  struct dimmNvram(*dimmNvList)[MAX_DIMM];
  UINT8                             MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      dimmNvList = GetDimmNvList(Host, socket, ch);
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }
      // Program results

      for (i = 0; i < 5; i++) {
        txDqSwz[i] = 0;
        rxDqSwz[i] = 0;
      }

      for (i = 0; i < 72; i++) {
        txDqSwz[i / 16] |= (((*dimmNvList)[dimm].dqSwz[i / 2] >> ((i % 2) * 4)) & 3) << ((i % 16) * 2);
      }

      for (j = 0; j < 5; j++) {
        for (i = 0; i < 16; i++) {
          rxDqSwz[j] |= (i % 4) << ((((txDqSwz[j] >> (i * 2)) & (BIT0 | BIT1)) * 2) + ((i / 4) * 8));
        } // i loop
      } // j loop
      txDqSwz[4] &= 0x0000FFFF;
      rxDqSwz[4] &= 0x0000FFFF;

      if (!(UbiosGenerationOrHsleEnabled ())) {
        // Program Swizzle NVMCTLR Registers
        if (IsMemFlowEnabled (DqSwizzlingDiscovery)) {
          WriteFnvCfg(Host, socket, ch, dimm, (DA_DDRT_TX_DQ_SWZ0_FNV_DA_UNIT_0_REG), txDqSwz[0]);
          WriteFnvCfg(Host, socket, ch, dimm, (DA_DDRT_RX_DQ_SWZ0_FNV_DA_UNIT_0_REG), rxDqSwz[0]);
          WriteFnvCfg(Host, socket, ch, dimm, (DA_DDRT_TX_DQ_SWZ1_FNV_DA_UNIT_0_REG), txDqSwz[1]);
          WriteFnvCfg(Host, socket, ch, dimm, (DA_DDRT_RX_DQ_SWZ1_FNV_DA_UNIT_0_REG), rxDqSwz[1]);
          WriteFnvCfg(Host, socket, ch, dimm, (DA_DDRT_TX_DQ_SWZ2_FNV_DA_UNIT_0_REG), txDqSwz[2]);
          WriteFnvCfg(Host, socket, ch, dimm, (DA_DDRT_RX_DQ_SWZ2_FNV_DA_UNIT_0_REG), rxDqSwz[2]);
          WriteFnvCfg(Host, socket, ch, dimm, (DA_DDRT_TX_DQ_SWZ3_FNV_DA_UNIT_0_REG), txDqSwz[3]);
          WriteFnvCfg(Host, socket, ch, dimm, (DA_DDRT_RX_DQ_SWZ3_FNV_DA_UNIT_0_REG), rxDqSwz[3]);
          WriteFnvCfg(Host, socket, ch, dimm, (DA_DDRT_TX_DQ_SWZ4_FNV_DA_UNIT_0_REG), txDqSwz[4]);
          WriteFnvCfg(Host, socket, ch, dimm, (DA_DDRT_RX_DQ_SWZ4_FNV_DA_UNIT_0_REG), rxDqSwz[4]);
        }
      }
    } // dimm loop
  } // ch loop
} // CheckAndProgramDqSwizzleAep

/**
  Routine Description: This function returns an valid Index ID 0-5 to decide Top 6 frequently accessed registers need to be cached
  @param Host      - pointer to System Host (root) structure
  @param Socket    - Socket ID
  @param Ch        - Channel Number
  @param Dimm      - Dimm Number
  @param Register  - Register

  @retval status
**/
UINT8
GetFMCCacheIndex(
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT32    Reg
  )
{
  UINT8             Status = 0xFF;
  UINT8             FmccacheIndex = 0;
  UINT8             CacheIndex;
  UINT8             InclutionIndex;
  UINT32            FmcCacheWeightedCSR[MAX_FMC_CACHE] = {D_DDRT_TRAINING_OTHER_EN_FNV_D_UNIT_0_REG,
                                                          DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_REG};

  struct FmcCacheSt (*FmcCache)[MAX_FMC_CACHE];

  for (InclutionIndex = 0; InclutionIndex < MAX_FMC_CACHE; InclutionIndex++) {
    if (Reg == FmcCacheWeightedCSR[InclutionIndex]) {
        FmccacheIndex = 1;
      break;
    }
  }

  if (FmccacheIndex == 0) {
    return Status;
  }

  FmcCache = &Host->var.mem.socket[Socket].channelList[Ch].dimmList[Dimm].FmcCache;

  for (CacheIndex = 0; CacheIndex < MAX_FMC_CACHE; CacheIndex++) {
    if ((*FmcCache)[CacheIndex].Status & ENTRY_USED) {
      if ((*FmcCache)[CacheIndex].Reg == Reg) {
        Status = CacheIndex;
        break;
      }
    } else {
      (*FmcCache)[CacheIndex].Status |= ENTRY_USED;
      (*FmcCache)[CacheIndex].Reg = Reg;
      Status = CacheIndex;

      if (Host->var.mem.FmcMaxCached < CacheIndex) {
        Host->var.mem.FmcMaxCached = CacheIndex;
      }
      break;
    }
  } //CacheIndex loop

  return Status;
}

/** Return the access mode for a given channel's far memory controller (FMC).

  @param  Socket  The socket where the channel of interest is located
  @param  Channel The channel of interest

  @retval The access mode for the far memory controller

**/
BUS_TYPE
EFIAPI
GetFmcAccessMode (
  IN  UINT8 Socket,
  IN  UINT8 Channel
  )
{
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];
  PSYSHOST              Host;

  Host = (PSYSHOST)GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  return (*ChannelNvList)[Channel].fnvAccessMode;
}

/** Set the access mode for a given channel's far memory controller (FMC).

  @param  Socket  The socket where the channel of interest is located
  @param  Channel The channel of interest
  @param  Mode    The access mode for the far memory controller

**/
VOID
EFIAPI
SetFmcAccessMode (
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  BUS_TYPE  Mode
  )
{
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];
  PSYSHOST              Host;

  Host = (PSYSHOST)GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  (*ChannelNvList)[Channel].fnvAccessMode = Mode;
}
