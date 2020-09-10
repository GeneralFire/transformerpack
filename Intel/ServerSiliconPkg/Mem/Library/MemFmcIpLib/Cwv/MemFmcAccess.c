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
#include <Memory/Ddrt2SpdRegisters.h>
#include <Library/MemFmcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Chip/Include/FnvAccessCommon.h>
#include "Include/MemFmcAccess.h"
#include "Include/MemFmcRegs.h"
#include "Include/MemFmcJedec.h"
#include <UncoreCommonIncludes.h>
#include <SysRegs.h>
#include <Library/ProcSmbIpLib.h>
#include <Memory/I3cDefinitions.h>
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
#include <Library/MemDdrioIpLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SiliconWorkaroundLib.h>

// Command/Data length to send Read Dword commad to CWV FMC
// Command length 4       // 1 byte Dest-SB-PortID-within-CWV (PortID)
                          // 1 byte MemReg CMD[3:0] READ=1 RSVD RSVD RSVD (Attribute)
                          // 2 bytes for Address
// Data length    4       // 4 bytes for read data
#define CWV_READ_DWORD      4

// Command length to send Write Dword/Byte commad to RCD
#define CWV_WRITE_DWORD_CMD  8     // 1 byte PortID + 1 byte Attribute + 2 bytes for Address + 4 bytes for write data
#define CWV_WRITE_WORD_CMD   6     // 1 byte PortID + 1 byte Attribute + 2 bytes for Address + 2 bytes for write data
#define CWV_WRITE_BYTE_CMD   5     // 1 byte PortID + 1 byte Attribute + 2 bytes for Address + 1 byte for write data


#define MAX_MRA_SIDEBAND 23

struct CwvMrwSideBand CwvMrwSideBandMap[MAX_MRA_SIDEBAND] = {
  //Cwv Address CSR Address
  {0x14, RW00_GLOBAL_FEATURES_CWV_DDRIO_HSIO_REG},               // 1. RW00
  {0x15, RW01_PARITY_CONTROL_CWV_DDRIO_HSIO_REG},                // 2. RW01
  {0x16, RW02_HOST_INTERFACE_TRAINING_MODE_CWV_DDRIO_HSIO_REG},  // 3. RW02
  {0x17, RW03_DB_INTF_TRAINING_MODE_CWV_DDRIO_HSIO_REG},         // 4. RW03
  {0x18, RW04_CMD_SPACE_CTRL_CWV_DDRIO_HSIO_REG},                // 5. RW04
  {0x00, DDRT_HSIO_TRAINING_MR0_CWV_DDRIO_HSIO_REG},             // 6. MR00
  {0x19, DDRT_HSIO_TRAINING_MR2_CWV_DDRIO_HSIO_REG},             // 7. MR02
  {0x1A, MR25_RD_TRN_MODE_CWV_DDRIO_HSIO_REG},                   // 8. MR25
  {0x1B, MR26_RD_PAT_D0_LFSR0_CWV_DDRIO_HSIO_REG},               // 9. MR26
  {0x1C, MR27_RD_PAT_D1_LFSR1_CWV_DDRIO_HSIO_REG},               // 10. MR27
  {0x1D, MR28_RD_PAT_INV_LOWER_CWV_DDRIO_HSIO_REG},              // 11. MR28
  {0x1E, MR30_RD_LFSR_MAP_CWV_DDRIO_HSIO_REG},                   // 12. MR30
  {0x1F, MR8_WR_RD_PRE_POSTAMBLE_CWV_DDRIO_HSIO_REG},            // 13. MR08
  {0x77, MR40_RD_DQS_OFFSET_CWV_DDRIO_HSIO_REG},                 // 14. MR40
  {0x76, DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_REG},              // 15. HSIO Training
// DCA DFE
  {0x31, RW31_DFE_CONFIG_CWV_DDRIO_HSIO_REG},                    // 16. RW31
  {0x32, RW32_DFETM_CTL_CWV_DDRIO_HSIO_REG},                     // 17. RW32
  {0x34, RW34_LFSR_DFETM_CWV_DDRIO_HSIO_REG},                    // 18. RW34
  {0x36, RW36_DFETM_INNER_LOOP_START_CWV_DDRIO_HSIO_REG},        // 19. RW36
  {0x37, RW37_DFETM_OUTER_LOOP_START_CWV_DDRIO_HSIO_REG},        // 20. RW37
  {0x3A, RW3A_DFETM_LOOP_STEP_SIZE_CWV_DDRIO_HSIO_REG},          // 21. RW3A
  {0x3B, RW3B_DFETM_INNER_LOOP_INCR_CWV_DDRIO_HSIO_REG},         // 22. RW3B
  {0x3C, RW3C_DFETM_OUTER_LOOP_INCR_CWV_DDRIO_HSIO_REG},          // 23. RW3C
};


//
// CPS Dimm Capacity info
//
const   UINT32 cpsDensity[16]             = {32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576}; // MBytes
const   UINT32 cpsdataWidth[4]            = {8, 16, 32, 64};
const   UINT32 cpsSizeGB[MAX_TECH_DDRT]   = {8, 16, 32, 64, 128, 256, 512, 1024}; // NVMDIMM size in GB

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
    return cpsSizeGB[TechIndex];
  } else {
    return 0;
  }
}

//-----------------------------------------------------------------------------------------------
/*
NVMCTLR address mapping

Function              AHB Slave Name  Base Address  End Address
DPA Mapped Registers  AHB_Slave_1     0xC0000000    0xC0FFFFFF
DA Unit CSRs          AHB_Slave_2     0xC1000000    0xC3FFFFFF
Dunit CSRs            AHB_Slave_3     0xC4000000    0xC4FFFFFF
DS Unit CSRs          AHB_Slave_4     0xC5000000    0xC5FFFFFF


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

#define CWV_SMB_REG_ADDR(Device, Function, Offset, SbOpCode) \
  (((Offset) & 0xffff) | (((Function) & 0x07) << 29) | (((Device) & 0x1F) << 24) | (((SbOpCode) & 0x7) << 20))

#define CWV_SIM_REG_ADDR(Device,Function,Offset) \
  (((Offset) & 0xffff) | (((Function) & 0x07) << 12) | (((Device) & 0x1F) << 15))

// Note: Mask bits[5:3] to ensure Quad word access (the first UI of the NVMDIMM data burst)
// CSR write size less than Quad word will need to use CSR read-modify-write unless caller handles caching
#define CWV_DPA_REG_ADDR(Device,Offset) \
  ((((Offset) & 0xfff) >> 3) | (((Device) & 0x1F) << 9))

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
GetCwvCfgAddress (
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
  UINT8 SbOpCode;

  RegOffset.Data = Offset;

  BoxType = (UINT8)RegOffset.Bits.boxtype;
  FuncBlk = (UINT8)RegOffset.Bits.funcblk;
  SbOpCode = 0;

  // Assert NVMCTLR BoxType == 5
  RC_FATAL_ERROR (BoxType == 0x1B, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_31);

  //
  // Identify the NVMCTLR Device/Function number for the access
  //
  if (FuncBlk < 0xe) {

//    Dev = FuncBlk >> 3;
//    Fun = FuncBlk & 7;

// pseudo function block definition
// 19ww15a-CWV-CSR_CORE\RIT\CsrWorkbook.cwv19ww15a.xlsx
// 19ww15a-CWV-DDRPHY\RIT\CsrWorkbook.cwv_ddrphy_19ww15a.xlsx
// +------------+---------------+------------------------+
// |Bunc Block  | pseudo FuncBlk|   BDF                  |
// +------------+---------------+------------------------+
// |DPA_MAPPED_0|   0x0         |   B0:D0:F6             |
// |DA_UNIT_0   |   0x1         |   B0:D1:F6             |
// |D_UNIT_0    |   0x2         |   B0:D4:F6             |
// |DS_UNIT_0   |   0x3         |   B0:D5:F6             |
// |A_UNIT_0    |   0x4         |   B0:D6:F6             |
// |P_UNIT_0    |   0x5         |   B0:D16:F6            |
// |P_UNIT_1    |   0x6         |   B0:D18:F6            |
// |DFX_MISC_0  |   0x7         |   B0:D24:F6            |
// |SEC_0       |   0x8         |   B0:D17:F4            |
// |PW_UNIT_0   |   0x9         |   B0:D17:F6            |
// |DDRPHY_0    |   0xa         |   B8:D0:F7             |
// |DDRPHY_1    |   0xb         |   B8:D1:F7             |
// |DDRPHY_2    |   0xc         |   B8:D2:F7             |
// |DDRPHY_3    |   0xd         |   B8:D3:F7             |
// +------------+---------------+------------------------+
  switch(FuncBlk) {
    case 0x0:
      Dev = 0;
      Fun = 6;
      break;
    case 0x1:
      Dev = 1;
      Fun = 6;
      break;
    case 0x2:
      Dev = 4;
      Fun = 6;
      break;
    case 0x3:
      Dev = 5;
      Fun = 6;
      break;
    case 0x4:
      Dev = 6;
      Fun = 6;
      break;
    case 0x5:
      Dev = 16;
      Fun = 6;
      break;
    case 0x6:
      Dev = 18;
      Fun = 6;
      break;
    case 0x7:
      Dev = 24;
      Fun = 6;
      break;
    case 0x8:
      Dev = 17;
      Fun = 4;
      break;
    case 0x9:
      Dev = 17;
      Fun = 6;
      break;
    case 0xa:
      Dev = 0;
      Fun = 7;
      break;
    case 0xb:
      Dev = 1;
      Fun = 7;
      SbOpCode = 6;
      break;
    case 0xc:
      Dev = 2;
      Fun = 7;
      break;
    case 0xd:
      Dev = 3;
      Fun = 7;
      break;
    default:
      //Assert on error
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_32);
      break;
  }

    //
    // Form address
    //
    switch (fnvInterface) {
    case FMC_SMB:
      RegAddr = CWV_SMB_REG_ADDR(Dev, Fun, RegOffset.Bits.offset, SbOpCode);
      break;
    case FMC_DPA:
      RegAddr = CWV_DPA_REG_ADDR(Dev, RegOffset.Bits.offset);
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

} // GetCwvCfgAddress

/**

  Read NVMCTLR SMBUS dev/offset worker

  @param Socket      - Processor socket ID
  @param SmbReg      - byte offset for NVMCTLR
  @param Dev         - SMBUS device address
  @param LocalData   - Pointer to data to read
  @param RegSize     - data size

  @retval EFI_SUCCESS  - success
  @retval !EFI_SUCCESS - failure

**/
EFI_STATUS
ReadCwvCfgSmbWorkerFunction (
  UINT8             Socket,
  UINT32            SmbReg,
  SMB_DEVICE_STRUCT Dev,
  volatile UINT32   *LocalData,
  UINT8             RegSize
  )
{
  volatile UINT8  *pByte = (volatile UINT8 *)LocalData;
  UINT8           Read = 1;
  EFI_STATUS      Status;
  UINT8           CmdDataLen;
  UINT8           CmdDataList[CWV_READ_DWORD];
  UINT8           *RegAddr = 0;

  Status = EFI_DEVICE_ERROR;
  ZeroMem (CmdDataList, sizeof (CmdDataList));
  RegAddr = (UINT8 *) (UINTN) GetProcSmbDataReg (Socket, Dev.address.SmbInstance, &RegSize);

  //
  // Use Block mode
  //
  CmdDataLen = CWV_READ_DWORD;
  // CWV SB portid[7:0]  = 32 * Function[2:0] + Device[4:0]
  CmdDataList[0] = (UINT8) ((SmbReg >> 24) & 0xFF);
  // Write command with 2nd byte packet with SBOPCODE | Read = 1 for read
  CmdDataList[1] = (UINT8) (((SmbReg >> 16) & 0xFF) | (Read << 3));
  // Write command with Reg as data for Reg[7:0]
  CmdDataList[2] = (UINT8) (SmbReg & 0xFF);
  // Write command with Rsvd as data for Reg[15:8]
  CmdDataList[3] = (UINT8) ((SmbReg >> 8) & 0xFF);

  // Write then Read
  if (SmbSendCccCommand (Socket, Dev, NO_CCC, AttrNormalWriteReStartRead, (UINT8 *) &CmdDataList, &CmdDataLen) == EFI_SUCCESS) {

    // analysis the data
    *pByte       = CmdDataList[0]; // Byte 0
    *(pByte + 1) = CmdDataList[1]; // Byte 1
    *(pByte + 2) = CmdDataList[2]; // Byte 2
    *(pByte + 3) = CmdDataList[3]; // Byte 3

    if (GetUbiosOutputMode () == ASM_OUTPUT_ENABLE_ON_SMBUS) {
      RcDebugPrint (SDBG_DEFAULT, "mov ecx, DWORD PTR ds:[0%08xh]\n", RegAddr);
    }

    Status = EFI_SUCCESS;
  }

  return Status;

} // ReadCwvCfgSmbWorkerFunction

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

UINT32
AhbToSimAddress(
                UINT32 smbReg
)
{
  UINT32 Device;
  UINT32 Function;
  UINT32 Offset;
  UINT32 SimAddress;
  UINT32 Bus;

  Device = (smbReg & (BIT24 | BIT25 | BIT26 | BIT27 | BIT28)) >> 24;
  Function = (smbReg & (BIT29 | BIT30 | BIT31)) >> 29;
  Offset = (smbReg & 0xFFFF);
  SimAddress = CWV_SIM_REG_ADDR(Device, Function, Offset);
  //
  // DDRIO registers are Function = 7. Bus is 8.
  //
  if (Function == 7) {
    Bus = 8 << 20;
    SimAddress = Bus | SimAddress;
  }

  return SimAddress;
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
  UINT64 StartTime = 0;
  UINT64 CurrentTime = 0;
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
  smbReg = GetCwvCfgAddress (Host, FMC_SMB, reg);
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
    // apply a timeout
    //
    StartTime = GetPerformanceCounter ();
    do {
      localData = 0;
      Status = ReadCwvCfgSmbWorkerFunction (socket, smbReg, dev, &localData, regSize);
      CurrentTime = GetPerformanceCounter ();
      if (TimeDiff (StartTime, CurrentTime, TDIFF_UNIT_MS) > 500) {
        OutputWarning (WARN_SMBUS_FAULT, WARN_SMBUS_READ, socket, ch, dimm, NO_RANK);
        DisableDIMM (Host, socket, ch, dimm);
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

/**

  Write NVMCTLR SMBUS dev/offset worker

  @param Socket      - Processor socket ID
  @param SmbReg      - byte offset for NVMCTLR
  @param Dev         - SMBUS device address
  @param LocalData   - Pointer to data to read
  @param RegSize     - data size
  @param CmdCode     - command code

  @retval EFI_SUCCESS  - success
  @retval !EFI_SUCCESS - failure

**/
EFI_STATUS
WriteCwvCfgSmbWorkerFunction (
  UINT8             Socket,
  UINT32            SmbReg,
  SMB_DEVICE_STRUCT Dev,
  UINT32            *LocalData,
  UINT8             RegSize,
  UINT8             CmdCode
  )
{
  EFI_STATUS  Status = EFI_DEVICE_ERROR;
  UINT8       *pByte = (UINT8 *) LocalData;
  UINT8       Write = 0;
  UINT8       CommandLen;
  UINT8       CommandList[CWV_WRITE_DWORD_CMD];

  if (RegSize > MAX_SIZE) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_173);
  }

  ZeroMem (CommandList, sizeof (CommandList));

  //
  // Use Block mode
  //

  // CWV SB portid[7:0]  = 32 * Function[2:0] + Device[4:0]
  CommandList[0] = (UINT8) ((SmbReg >> 24) & 0xFF);
  // Write command with 2nd byte packet with SBOPCODE | Read = 0 for write
  CommandList[1] = (UINT8) (((SmbReg >> 16) & 0xFF) | (Write << 3));
  // Write command with Reg as data for Reg[7:0]
  CommandList[2] = (UINT8) (SmbReg & 0xFF);
  // Write command with Rsvd as data for Reg[15:8]
  CommandList[3] = (UINT8) ((SmbReg >> 8) & 0xFF);

  switch (RegSize) {
  case sizeof (UINT32):
    CommandLen = CWV_WRITE_DWORD_CMD;
    CommandList[4] = *pByte;       // Byte 0
    CommandList[5] = *(pByte + 1); // Byte 1
    CommandList[6] = *(pByte + 2); // Byte 2
    CommandList[7] = *(pByte + 3); // Byte 3
    break;

  case sizeof (UINT16):
    CommandLen = CWV_WRITE_WORD_CMD;
    CommandList[4] = *pByte;       // Byte 0
    CommandList[5] = *(pByte + 1); // Byte 1
    break;

  case sizeof (UINT8):
    CommandLen = CWV_WRITE_BYTE_CMD;
    CommandList[4] = *pByte;       // Byte 0
    break;

  default:
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_76);
    return Status;
  }

  if (SmbSendCccCommand (Socket, Dev, NO_CCC, AttrNormalWrite, (UINT8 *) &CommandList, &CommandLen) == EFI_SUCCESS) {
    Status = EFI_SUCCESS;
  }

  return Status;

}  // WriteCwvCfgSmbWorkerFunction

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
  UINT64 StartTime = 0;
  UINT64 CurrentTime = 0;
  UINT32 localData = data;
  SMB_DEVICE_STRUCT dev;
  UINT8 cmdCode;
  CSR_OFFSET RegOffset;
  UINT8 OriginalUbiosOutputMode = ASM_OUTPUT_ENABLE; // This default is needed for compilation but shouldn't matter.
  SYS_SETUP *Setup;
  Setup = GetSysSetupPointer ();

#ifdef MRC_TRACE
  RcDebugPrintWithDevice (SDBG_TRACE, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "WriteFnvCfgSmb Starts\n");
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
  smbReg = GetCwvCfgAddress (Host, FMC_SMB, reg);

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

    // Apply a timeout for SMBUS writes. Infinite loop if you always receive NACK from EKV
    // Unique error code so that we can identify a timeout occurred.
    StartTime = GetPerformanceCounter ();
    
    do {
      Status = WriteCwvCfgSmbWorkerFunction (socket, smbReg, dev, &localData, regSize, cmdCode);
      CurrentTime = GetPerformanceCounter ();
      if (TimeDiff (StartTime, CurrentTime, TDIFF_UNIT_MS) > 500) {
        OutputWarning (WARN_SMBUS_FAULT, WARN_SMBUS_WRITE, socket, ch, dimm, NO_RANK);
        DisableDIMM (Host, socket, ch, dimm);
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
  ZeroMem ((UINT8 *) LaneErrorStatus, sizeof (LaneErrorStatus));

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

  ZeroMem ((UINT8 *) LaneErrorStatus, sizeof (LaneErrorStatus));

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
                 "ReadFnvCfgDpa Starts\n");

  RegOffset.Data = reg;
  regSize = RegOffset.Bits.size;
  regIndex = (RegOffset.Bits.offset & 0x7) * 8;
  dpa = GetCwvCfgAddress (Host, FMC_DPA, reg);

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
  dpa = GetCwvCfgAddress (Host, FMC_DPA, reg);

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
  UINT8                              ch;
  struct channelNvram               (*channelNvList)[MAX_CH];
  UINT8                             MaxChDdr;

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
  MaxChDdr = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Update NVMCTLR access mode
    //
    SetFmcAccessMode(socket, ch, SMBUS);
  }

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
  UINT8                                        Ch;
  UINT8                                        Dimm;
  BUS_TYPE                                     OriginalFmcAccessMode;
  DDRT_HSIO_WR_SCR_CTRL_CWV_DDRIO_HSIO_STRUCT  TxSeedSel;
  DDRT_HSIO_RD_SCR_CTRL_CWV_DDRIO_HSIO_STRUCT  RxSeed;
  BootMode                                     SysBootMode;
  SYS_SETUP                                    *Setup;
  UINT8                                        MaxChDdr;

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

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (socket, Ch, Dimm) == FALSE) {
        continue;
      }
      if (IsDcpmmPresentDimm (socket, Ch, Dimm) == FALSE) {
        continue;
      }

      if (Setup->mem.options & SCRAMBLE_EN_DDRT) {
        //
        // Set the FMC access mode
        //
        OriginalFmcAccessMode = GetFmcAccessMode (socket, Ch);
        SetFmcAccessMode (socket, Ch, SMBUS);

        // Program Scrambling
        TxSeedSel.Data = 0;
        RxSeed.Data = 0;

        //Initialize seeds
        TxSeedSel.Bits.lfsr_seed_ordersel = Host->nvram.mem.scrambleSeed;
        RxSeed.Bits.lfsr_seed = 0xFFFF;
        WriteFnvCfgSmb (Host, socket, Ch, Dimm, DDRT_HSIO_WR_SCR_CTRL_CWV_DDRIO_HSIO_REG, TxSeedSel.Data);
        WriteFnvCfgSmb (Host, socket, Ch, Dimm, DDRT_HSIO_RD_SCR_CTRL_CWV_DDRIO_HSIO_REG, RxSeed.Data);

        // Ensure 0->1 transition to load seeds
        RxSeed.Bits.lfsr_load = 1;
        WriteFnvCfgSmb (Host, socket, Ch, Dimm, DDRT_HSIO_RD_SCR_CTRL_CWV_DDRIO_HSIO_REG, RxSeed.Data);

        TxSeedSel.Bits.lfsr_en = 1;
        RxSeed.Bits.lfsr_en = 1;
        WriteFnvCfgSmb (Host, socket, Ch, Dimm, DDRT_HSIO_WR_SCR_CTRL_CWV_DDRIO_HSIO_REG, TxSeedSel.Data);
        WriteFnvCfgSmb (Host, socket, Ch, Dimm, DDRT_HSIO_RD_SCR_CTRL_CWV_DDRIO_HSIO_REG, RxSeed.Data);

        //
        // Restore the FMC access mode
        //
        SetFmcAccessMode (socket, Ch, OriginalFmcAccessMode);

        SetScrambleConfigMc (Host, socket, Ch, SCRAMBLE_CONFIG_LATE_FNV_CONFIG);

      }
    } //Dimm loop

    if (UbiosGenerationOrHsleEnabled () &&
        (Setup->common.ddrtXactor == 0))
    {
      // Update NVMCTLR access mode. This is done a first time to reduce the SMBUS accesses in FnvDdrtIoInit (below).
      SetFmcAccessMode (socket, Ch, CPGC);
    }

  } //Ch loop

  if (SysBootMode == NormalBoot && (
        Host->var.mem.subBootMode == ColdBoot ||
        Host->var.mem.subBootMode == ColdBootFast))
  {
    FnvDdrtIoInit (Host, socket, DDRT_TRAINING_COMPLETE);
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (socket, Ch) == FALSE) {
      continue;
    }
    if (UbiosGenerationOrHsleEnabled () &&
        (Setup->common.ddrtXactor == 0))
    {
      // Update NVMCTLR access mode. This is done a second time to revert SMBUS accesses that came about in
      // FnvDdrtIoInit (above).
      SetFmcAccessMode (socket, Ch, CPGC);
    }

    //
    // Shutoff clock for Channel which is disabled
    //
    if ((IsDcpmmPresentChannel (socket, Ch)) && (IsChannelEnabled (socket, Ch) == FALSE)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Shut off clock for disabled channel %d/n", Ch);
      SetRankEnDdrio (Host, socket, Ch, 0);
    }
  } // for (Ch)

} // LateFnvConfigAccess


/**

Routine Description: ReadFnvCfg will read a NVMCTLR register via SMBus or CPGC interface.
This will return the data read from a NVMCTLR register because ReadFnvCfgSmb and ReadFnvCfgDpa require different size of data types
If no interface is supported, then enter RC_FATAL_ERROR

  @param Host        - Pointer to the system Host (root) structure
  @param socket      - Socket Number
  @param ch          - DDR Channel ID
  @param dimm        - DIMM number
  @param mode        - Type of interface SMBus, or CPGC
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
                                "Invalid CWV Access mode.  Using SMBUS\n");
        EwlOutputType1 (WARN_FNV_ACCESS, WARN_INVALID_FNV_ACCESS_MODE, Socket, Ch, Dimm, NO_RANK);
      case SMBUS:
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
  @param mode        - Type of interface SMBUS, or CPGC
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
                              "Invalid CWV Access mode.  Using SMBUS\n");
      EwlOutputType1 (WARN_FNV_ACCESS, WARN_INVALID_FNV_ACCESS_MODE, Socket, Ch, Dimm, NO_RANK);
    case SMBUS:
      WriteFnvCfgSmb (Host, Socket, Ch, Dimm, Reg, Data);
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
  D_FW_STATUS_CWV_D_UNIT_0_STRUCT fwStatusReg;

  fwStatusReg.Data = ReadFnvCfg(Host, socket, ch, dimm, D_FW_STATUS_CWV_D_UNIT_0_REG);
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

  Calls to gather CPS important SPD Data(list as follows) in case of Channel/Dimm map out before GatherSPDData be executed

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
  UINT8               ngnCapacity;      // NVMDIMM capacity
  UINT8               ngnBusWidth;      // NVMDIMM bus width
  UINT8               ngnDeviceType;    // NVMDIMM device type
  UINT8               ngnModuleOrg;     // NVMDIMM module organization
  UINT8               ti;
  UINT32              size;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct channelNvram (*ChannelNvList)[MAX_CH];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Get NGN Capacity
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_MEMORY_DEVICE_DENSITY_REG, &SPDReg);
  ngnCapacity = SPDReg;
  (*DimmNvList)[Dimm].NvmDevDensity = SPDReg;

  //
  // Get NGN Number of ranks and device width
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_CPS_MODULE_ORGANIZATION_REG, &SPDReg);
  ngnModuleOrg = SPDReg;

  //
  // Get NGN Device Type
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_NVM_PACKAGE_TYPE_AND_LOADING_REG, &SPDReg);
  ngnDeviceType = SPDReg;
  (*DimmNvList)[Dimm].NvmDevType = SPDReg;

  //
  // Get NGN Bus Width
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_CPS_MODULE_NVM_MEMORY_BUS_WIDTH_REG, &SPDReg);
  ngnBusWidth = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_NUMBER_OF_WRITE_CREDITS_CWV_MEMORY_CONTROLLER_CAN_HANDLE_REG, &SPDReg);
  (*ChannelNvList)[Ch].FmcWrCreditLimit = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_NUMBER_OF_READ_CREDITS_CWV_MEMORY_CONTROLLER_CAN_HANDLE_REG, &SPDReg);
  (*ChannelNvList)[Ch].FmcRdCreditLimit = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_SUBSYSTEM_CONTROLLER_PRODUCT_IDENTIFIER_LSB_REG, &SPDReg);
  (*DimmNvList)[Dimm].fmcType = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_NVM_DEVICE_AND_STEPPING_REG, &SPDReg);
  (*DimmNvList)[Dimm].NvmStepping = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_REVISION_CODE_REG, &SPDReg);
  (*DimmNvList)[Dimm].FmcModRev.Data = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_NON_VOLATILE_MEMORY_SUBSYSTEM_CONTROLLER_REVISION_REG, &SPDReg);
  (*DimmNvList)[Dimm].fmcRev = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_THERMAL_SENSOR_ACCURACY_REG, &SPDReg);
  (*DimmNvList)[Dimm].dimmTs = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_SYSTEM_DATA_CHANNEL_BYTE_CONNECTION_ORDERING_1_REG, &SPDReg);
  (*DimmNvList)[Dimm].lrbufswizzle = SPDReg;
  SpdReadByte (Socket, Ch, Dimm, SPD_SYSTEM_DATA_CHANNEL_BYTE_CONNECTION_ORDERING_REG, &SPDReg);
  (*DimmNvList)[Dimm].lrbufswizzle |= SPDReg << 8;

  if (FeaturePcdGet (PcdDdrt2BufferlessBuild) == TRUE) {
    //
    // Allow to configure DDRT2 as bufferless
    //
    (*DimmNvList)[Dimm].lrDimmPresent = 0;
    (*ChannelNvList)[Ch].lrDimmPresent = 0;
    Host->nvram.mem.socket[Socket].lrDimmPresent = 0;
  }

  //
  // Bits 3-0: Individual NVM Die Density
  // Bits 2-0: Effective NVM Channel Data Width, bit2 is reserved, only used bit 1-0
  //
  size = CalculateDdrtDimmSize (cpsDensity[ngnCapacity & 0xF], cpsdataWidth[ngnBusWidth & (BIT0 | BIT1)], ngnDeviceType, ngnModuleOrg);

  for (ti = 0; ti < MAX_TECH_DDRT; ti++) {
    if (cpsSizeGB[ti] == size) {
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
  (*DimmNvList)[Dimm].aepTechIndex = ti;
  (*DimmNvList)[Dimm].AitDramPoll  = 0;   // AIT Dram polling disable

  if ((*DimmNvList)[Dimm].fmcType == FMC_CWV_TYPE) {
    //
    // CWV A0 now only supports x4
    //
    (*DimmNvList)[Dimm].SPDModuleOrg = 0;
    (*DimmNvList)[Dimm].actSPDModuleOrg = 0;

#ifdef DDR5_SUPPORT
    GetCommonDDR5DIMMConfig (Host, Socket, Ch, Dimm);
#endif
  }

  //Get VendorID, DeviceID, RID, SubSystemVendorID, SubSystemDeviceID, and SubSystem RID from SPD
  SpdReadByte (Socket, Ch, Dimm, SPD_CPS_VENDORID_BYTE0, &SPDReg);
  (*DimmNvList)[Dimm].VendorID  = (UINT16) SPDReg;
  SpdReadByte (Socket, Ch, Dimm, SPD_CPS_VENDORID_BYTE1, &SPDReg);
  (*DimmNvList)[Dimm].VendorID |= (UINT16) (SPDReg << 8);

  SpdReadByte (Socket, Ch, Dimm, SPD_CPS_MODULE_PRODUCT_IDENTIFIER_LSB_REG, &SPDReg);
  (*DimmNvList)[Dimm].DeviceID = (UINT16) SPDReg;
  SpdReadByte (Socket, Ch, Dimm, SPD_CPS_MODULE_PRODUCT_IDENTIFIER_MSB_REG, &SPDReg);
  (*DimmNvList)[Dimm].DeviceID |= (UINT16) (SPDReg << 8);

  SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_REVISION_CODE_REG, &SPDReg);
  (*DimmNvList)[Dimm].RevisionID = SPDReg;

  SpdReadByte (Socket, Ch, Dimm, SPD_MEMORY_SUBSYSTEM_CONTROLLER_MANUFACTURERS_ID_CODE_0_REG, &SPDReg);
  (*DimmNvList)[Dimm].SubsystemVendorID = (UINT16) SPDReg;
  SpdReadByte (Socket, Ch, Dimm, SPD_MEMORY_SUBSYSTEM_CONTROLLER_MANUFACTURERS_ID_CODE_1_REG, &SPDReg);
  (*DimmNvList)[Dimm].SubsystemVendorID |= (UINT16) (SPDReg << 8);

  SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_SUBSYSTEM_CONTROLLER_PRODUCT_IDENTIFIER_LSB_REG, &SPDReg);
  (*DimmNvList)[Dimm].SubsystemDeviceID = (UINT16) SPDReg;
  SpdReadByte (Socket, Ch, Dimm, SPD_MODULE_SUBSYSTEM_CONTROLLER_PRODUCT_IDENTIFIER_MSB_REG, &SPDReg);
  (*DimmNvList)[Dimm].SubsystemDeviceID |= (UINT16) (SPDReg << 8);

  SpdReadByte (Socket, Ch, Dimm, SPD_NON_VOLATILE_MEMORY_SUBSYSTEM_CONTROLLER_REVISION_REG, &SPDReg);
  (*DimmNvList)[Dimm].SubsystemRevisionID = SPDReg;

  //
  // 32 Data lanes + 8 ECC lanes
  //
  (*DimmNvList)[Dimm].SPDMemBusWidth = 0x32;
} //GatherDdrtSPDData

/**

  Checks DQ swizzling and programs registers to account for it

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param entry   - TRUE when entering GetMargins, FALSE when exiting.

**/
VOID
CheckAndProgramDqSwizzleAep (
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
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      dimmNvList = GetDimmNvList (Host, socket, ch);
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
/**
//CWV_TODO
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
**/
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
GetFMCCacheIndex (
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
/**
//CWV_TODO
  UINT32            FmcCacheWeightedCSR[MAX_FMC_CACHE] = {MSC_HOLE_1_CWV_DFX_MISC_0_REG, D_FUNC_DEFEATURE2_CWV_D_UNIT_0_REG,
                                       D_DDRT_TRAINING_OTHER_EN_CWV_D_UNIT_0_REG, DA_CXFIFO_TRAININGRESET_CWV_DA_UNIT_1_REG, 0, 0};
**/
  struct FmcCacheSt (*FmcCache)[MAX_FMC_CACHE];

  for (InclutionIndex = 0; InclutionIndex < MAX_FMC_CACHE; InclutionIndex++) {
/**
//CWV_TODO
    if (Reg == FmcCacheWeightedCSR[InclutionIndex]) {
        FmccacheIndex = 1;
      break;
    }
**/
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

  if (Mode == EMRS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Invalid FMC Access Mode type  - Modle = %d CPS DIMM not supported!\n", Mode);
    RC_FATAL_ERROR (FALSE, ERR_INVALID_REG_ACCESS, ERR_INVALID_ACCESS_METHOD);
    return;
  }
  Host = (PSYSHOST)GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  (*ChannelNvList)[Channel].fnvAccessMode = Mode;
}


/**
Convert MRW to Sideband

@param[in]  Host        - Pointer to sysHost
@param[in]  Socket      - Socket Id
@param[in]  Ch          - Ch number
@param[in]  Dimm        - Dimm number (zero-based)
@param[in]  Address     - Address for the MRS write
@param[in]  Data        - Data for MRW
**/

VOID
WriteCwvSideband (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT16   Address,
  IN UINT32   Data
)
{
  UINT8 MraIndex = 0;

  for (MraIndex = 0; MraIndex < MAX_MRA_SIDEBAND; MraIndex++) {
    if ((CwvMrwSideBandMap[MraIndex].CrowValleyMraAddress == Address)) {
      WriteFnvCfg(Host, Socket, Ch, Dimm, CwvMrwSideBandMap[MraIndex].CrowValleyCsr, Data);
      break;
    }
  }
}
