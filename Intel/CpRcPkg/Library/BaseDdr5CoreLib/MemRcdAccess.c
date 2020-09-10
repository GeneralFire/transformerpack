/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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
#include "Ddr5Core.h"
#include <Memory/Ddr5Cmds.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CteNetLib.h>
#include <MemHost.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/I3cDefinitions.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/Ddr5CoreLib.h>

#define QCK_MIN_VAL           0
#define QCK_MAX_VAL           0x3F
#define QCK_OUTPUT_DELAY_EN   BIT7

#define QCS_MIN_VAL           0
#define QCS_MAX_VAL           0x7F
#define QCS_OUTPUT_DELAY_EN   BIT7


QCX_SIGNAL_RAW_CARD_MAP SRx8[MAX_RANK_DIMM] =
{
  {RDIMM_RW17,        RDIMM_RW_INVALID,   1}, ///< Rank 0 accesses QACS0
  {RDIMM_RW_INVALID,  RDIMM_RW_INVALID,   0}, ///< Rank 1 does not exist
};

QCX_SIGNAL_RAW_CARD_MAP SRx4[MAX_RANK_DIMM] =
{
  {RDIMM_RW17,        RDIMM_RW19,         2}, ///< Rank 0 accesses QACS0 & QBCS0
  {RDIMM_RW_INVALID,  RDIMM_RW_INVALID,   0}, ///< Rank 1 does not exist
};

QCX_SIGNAL_RAW_CARD_MAP DRx8[MAX_RANK_DIMM] =
{
  {RDIMM_RW17,        RDIMM_RW_INVALID,   1}, ///< Rank 0 accesses QACS0
  {RDIMM_RW18,        RDIMM_RW_INVALID,   1}, ///< Rank 1 accesses QACS1
};

QCX_SIGNAL_RAW_CARD_MAP DRx4[MAX_RANK_DIMM] =
{
  {RDIMM_RW17,        RDIMM_RW19,         2}, ///< Rank 0 accesses QACS0 & QBCS0
  {RDIMM_RW18,        RDIMM_RW1A,         2}, ///< Rank 1 accesses QACS1 & QBCS1
};

QCX_SIGNAL_RAW_CARD_MAP *RawCards [MAX_RAW_CARDS] = {
  SRx8,
  SRx4,
  DRx8,
  DRx4
};

/**

  Send MRW Command to RCD and update the RCD CW cache

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (socket-based)
  @param[in] SubChMask  - Mask of subchannel numbers to be updated
  @param[in] Dimm       - DIMM number within the channel
  @param[in] Rank       - Rank number within the DIMM
  @param[in] Address    - Address of the mode register
  @param[in] Data       - Data to write

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
RcdControlWordWrite (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Address,
  IN UINT8    Data
  )
{
  PSYSHOST          Host;
  UINT32            Status;
  UINT8             MaxChDdr;
  UINT8             SubCh;
  UINT8             Page;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  MaxChDdr    = GetMaxChDdr ();

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "WARNING - RcdControlWordWrite called with an invalid parameter\n");
    return MRC_STATUS_FAILURE;
  }

  Host        = GetSysHostPointer ();
  DimmNvList  = GetDimmNvList (Host, Socket, Ch);
  Status      = MRC_STATUS_FAILURE;

  if (SendMrw (Host, Socket, Ch, SubChMask, Dimm, Rank, Address, CW_RCD_DB, Data) == SUCCESS) {

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if ((SubChMask & (1 << SubCh)) == 0) {
        continue;
      }
      Page = RDIMM_CW_PAGE0;
      if (Address >= MAX_RCD_DIRECT_CONTROL_WORDS) {
        ReadRcdCwCache (Socket, Ch, SubCh, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_CW_PAGE_REG, &Page);
      }
      WriteRcdCwCache (Socket, Ch, 1 << SubCh, Dimm, Page, Address, Data);
    }
    Status = MRC_STATUS_SUCCESS;
  }

  return Status;
}

/**

  Read a DWORD from DDR5 RCD over SMBUS
  Note the protocol only allows accesses to addresses on
  DWORD boundaries. The RCD will mask off the lower 2 address
  bits.
  To read a specific byte, use ReadRcdSmbByteDdr5.

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket Id
  @param[in]  Ch           - Channel number (0-based)
  @param[in]  Dimm         - DIMM number (0-based)
  @param[in]  SubCh        - SubChannel number (0-based)
  @param[in]  Rank         - Rank number (0-based)
  @param[in]  CwRegPage    - RCD control word page number
  @param[in]  CwRegAddress - RCD control word number
  @param[out] CwData       - Pointer to the data buffer

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
ReadRcdSmbDdr5 (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       Dimm,
  IN      UINT8       SubCh,
  IN      UINT8       CwRegPage,
  IN      UINT8       CwRegAddress,
     OUT  UINT32      *CwData
  )
{
  UINT8                   Reserved = 0;
  UINT8                   PageNum;
  UINT8                   RegNum;
  UINT32                  CwDataLocal = 0xFFFFFFFF;
  UINT32                  Status = FAILURE;
  SMB_DEVICE_STRUCT       Dev;
  UINT8                   RcdStatus;
  RCD_SMBUS_ADDRESS_DDR5  RcdSmbAddr;
  UINT8                   CmdDataLen;
  UINT8                   CmdDataList[RCD_READ_DWORD];

  PageNum = CwRegPage;
  RegNum = CwRegAddress;

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    RcdSmbAddr.Data = 0;
    RcdSmbAddr.Bits.Channel = SubCh;
    RcdSmbAddr.Bits.PageNum = CwRegPage;
    RcdSmbAddr.Bits.RegNum = CwRegAddress;
    CteReadRcdCfgSmb (Ch, Dimm, RcdSmbAddr.Data, sizeof (UINT32), CwData);
    return SUCCESS;
  }

  Dev.compId = SPD;
  Dev.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  Dev.address.deviceType = DTI_LRDIMM;

  if (EFI_ERROR (GetSmbAddress (Socket, Ch, Dimm, &Dev))) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
    return FAILURE;
  }

  // Use Byte mode when I3C bus is in I2C mode
  if (GetSlaveType (Socket, (UINT8) Dev.address.SmbInstance) == I2C_DEVICE) {

    if ((WriteProcSmb (Dev, 0x80, &Reserved) == EFI_SUCCESS) &&
        (WriteProcSmb (Dev, 0x00, &SubCh) == EFI_SUCCESS) &&
        (WriteProcSmb (Dev, 0x00, &PageNum) == EFI_SUCCESS) &&
        (WriteProcSmb (Dev, 0x40, &RegNum) == EFI_SUCCESS) &&
        (ReadProcSmb (Dev, 0x80, &RcdStatus) == EFI_SUCCESS) &&
        (ReadProcSmb (Dev, 0x00, (UINT8 *) &CwDataLocal + 3) == EFI_SUCCESS) &&
        (ReadProcSmb (Dev, 0x00, (UINT8 *) &CwDataLocal + 2) == EFI_SUCCESS) &&
        (ReadProcSmb (Dev, 0x00, (UINT8 *) &CwDataLocal + 1) == EFI_SUCCESS) &&
        (ReadProcSmb (Dev, 0x40, (UINT8 *) &CwDataLocal) == EFI_SUCCESS)) {
      Status = SUCCESS;
    }
  } else {

    //
    // Use Block mode when I3C bus is in I3C mode
    //
    CmdDataLen = RCD_READ_DWORD;
    CmdDataList[0] = RCD_SB_CMD_READ_DWORD;  // Double Word Read
    CmdDataList[1] = RCD_READ_DWORD - RCD_CMD_DATA_LEN_OFFSET; // Command length
    CmdDataList[2] = Reserved;
    CmdDataList[3] = SubCh;
    CmdDataList[4] = PageNum;
    CmdDataList[5] = RegNum;

    // Write then Read
    if (SmbSendCccCommand (Socket, Dev, NO_CCC, AttrNormalWriteReStartRead, (UINT8 *) &CmdDataList, &CmdDataLen) == EFI_SUCCESS) {

      // analysis the data
      CwDataLocal = 0;
      if (CmdDataList[0] != (RCD_READ_DWORD - 1)) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                                "ReadRcdSmb the first Read Data should be %d, not %d \n", (RCD_READ_DWORD - 1), CmdDataList[0]);
      }

      RcdStatus = CmdDataList[1];
      CwDataLocal |= (CmdDataList[2] << 24);
      CwDataLocal |= (CmdDataList[3] << 16);
      CwDataLocal |= (CmdDataList[4] << 8);
      CwDataLocal |= CmdDataList[5];
      Status = SUCCESS;
    }
  }

  WriteRcdCwCache (Socket, Ch, (1 << SubCh), Dimm, RDIMM_CW_PAGE0, DDR5_RCD_CW_PAGE_REG, PageNum);


  *CwData = CwDataLocal;

  return Status;
}

/**

  Read a BYTE from DDR5 RCD over SMBUS

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket Id
  @param[in]  Ch           - Channel number (0-based)
  @param[in]  Dimm         - DIMM number (0-based)
  @param[in]  SubCh        - SubChannel number (0-based)
  @param[in]  Rank         - Rank number (0-based)
  @param[in]  CwRegPage    - RCD control word page number
  @param[in]  CwRegAddress - RCD control word number
  @param[out] CwData       - Pointer to the data buffer

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
ReadRcdSmbByteDdr5 (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       Dimm,
  IN      UINT8       SubCh,
  IN      UINT8       CwRegPage,
  IN      UINT8       CwRegAddress,
     OUT  UINT8       *CwData
  )
{
  UINT8   ByteOffset;
  UINT32  CwDataDword;
  UINT32  Status;

  Status = ReadRcdSmbDdr5 (Host, Socket, Ch, Dimm, SubCh, CwRegPage, CwRegAddress & ~0x03, &CwDataDword);

  if (Status == SUCCESS) {
    //
    // The protocol only supports reads at DWORD bounaries. Need to obtain the
    // byte for the register specified by the caller.
    //
    ByteOffset = CwRegAddress & 0x03;
    *CwData = *((UINT8 *)&CwDataDword + ByteOffset);
  }

  return Status;
}

/**

  Write a BYTE to the DDR5 RCD over SMBUS

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket Id
  @param[in]  Ch           - Channel number (0-based)
  @param[in]  Dimm         - DIMM number (0-based)
  @param[in]  SubChMask    - SubChannel mask
  @param[in]  Size         - Size in the number of bytes
  @param[in]  CwRegPage    - RCD control word page number
  @param[in]  CwRegAddress - RCD control word number
  @param[in]  CwData       - Data to write

  @retval SUCCESS
  @retval FAILURE

**/
UINT32
WriteRcdSmbDdr5 (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       Dimm,
  IN      UINT8       SubChMask,
  IN      UINT8       Size,
  IN      UINT8       CwRegPage,
  IN      UINT8       CwRegAddress,
  IN      UINT32      *CwData
  )
{
  UINT32                  Status = FAILURE;
  UINT8                   CommandLen;
  UINT8                   CommandList[RCD_WRITE_DWORD_CMD];
  UINT8                   Reserved = 0;
  UINT8                   PageNum;
  UINT8                   RegNum;
  UINT8                   SubCh;
  EFI_STATUS              EfiStatus;
  RCD_SMBUS_ADDRESS_DDR5  RcdSmbAddr;
  SMB_DEVICE_STRUCT       Dev;
  struct dimmNvram (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  RcDebugPrintWithDevice (SDBG_TRACE, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "SMB RCD Write with Address 0x%04X, Page 0x%04X, Data 0x%04X, SubChMask %d \n", CwRegAddress, CwRegPage, *CwData, SubChMask);
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & SubChMask) == 0) {
      continue;
    }

    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      RcdSmbAddr.Data = 0;
      RcdSmbAddr.Bits.Channel = SubCh;
      RcdSmbAddr.Bits.PageNum = CwRegPage;
      RcdSmbAddr.Bits.RegNum = CwRegAddress;
      EfiStatus = CteWriteRcdCfgSmb (Ch, Dimm, RcdSmbAddr.Data, sizeof (UINT8), *CwData);
      if (EFI_ERROR (EfiStatus)) {
        Status = FAILURE;
        RcDebugPrint (SDBG_DEFAULT, "WriteRcdSmbDdr5: Failed CteWriteRcdCfgSmb with return code 0x%x.\n", EfiStatus);
      } else {
        Status = SUCCESS;
      }
      return Status;
    }

    Dev.compId = SPD;
    Dev.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
    Dev.address.deviceType = DTI_LRDIMM;

    if (EFI_ERROR (GetSmbAddress (Socket, Ch, Dimm, &Dev))) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
      return FAILURE;
    }

    PageNum = CwRegPage;
    RegNum = CwRegAddress;

    if (Size > MAX_SIZE) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_173);
    }

    switch (Size) {
    case sizeof (UINT32):

      // Use Byte mode when it is in I2C mode
      if (GetSlaveType (Socket, (UINT8) Dev.address.SmbInstance) == I2C_DEVICE) {

        if ((WriteProcSmb (Dev, 0x8C, &Reserved) == EFI_SUCCESS) &&
            (WriteProcSmb (Dev, 0x0C, &SubCh) == EFI_SUCCESS) &&
            (WriteProcSmb (Dev, 0x0C, &PageNum) == EFI_SUCCESS) &&
            (WriteProcSmb (Dev, 0x0C, &RegNum) == EFI_SUCCESS) &&
            (WriteProcSmb (Dev, 0x0C, (UINT8 *) CwData + 3) == EFI_SUCCESS) &&
            (WriteProcSmb (Dev, 0x0C, (UINT8 *) CwData + 2) == EFI_SUCCESS) &&
            (WriteProcSmb (Dev, 0x0C, (UINT8 *) CwData + 1) == EFI_SUCCESS) &&
            (WriteProcSmb (Dev, 0x4C, (UINT8 *) CwData) == EFI_SUCCESS)) {
          Status = SUCCESS;
        }
      } else {

        //
        // Use Block mode when I3C bus is in I3C mode
        //
        CommandLen = RCD_WRITE_DWORD_CMD;
        CommandList[0] = RCD_SB_CMD_WRITE_DWORD;  // Double Word Write
        CommandList[1] = RCD_WRITE_DWORD_CMD - RCD_CMD_DATA_LEN_OFFSET; // Command length
        CommandList[2] = Reserved;
        CommandList[3] = SubCh;
        CommandList[4] = PageNum;
        CommandList[5] = RegNum;
        CommandList[6] = *((UINT8 *) CwData + 3);
        CommandList[7] = *((UINT8 *) CwData + 2);
        CommandList[8] = *((UINT8 *) CwData + 1);
        CommandList[9] = *(UINT8 *) CwData;

        if (SmbSendCccCommand (Socket, Dev, NO_CCC, AttrNormalWrite, (UINT8 *) &CommandList, &CommandLen) == EFI_SUCCESS) {
          Status = SUCCESS;
        }
      }

      if (Status == SUCCESS) {
        WriteRcdCwCache (Socket, Ch, (1 << SubCh), Dimm, RDIMM_CW_PAGE0, DDR5_RCD_CW_PAGE_REG, PageNum);
        WriteRcdCwCache (Socket, Ch, (1 << SubCh), Dimm, PageNum, RegNum + 3, *((UINT8 *)CwData + 3));
        WriteRcdCwCache (Socket, Ch, (1 << SubCh), Dimm, PageNum, RegNum + 2, *((UINT8 *)CwData + 2));
        WriteRcdCwCache (Socket, Ch, (1 << SubCh), Dimm, PageNum, RegNum + 1, *((UINT8 *)CwData + 1));
        WriteRcdCwCache (Socket, Ch, (1 << SubCh), Dimm, PageNum, RegNum, *(UINT8 *)CwData);
        RcDebugPrintWithDevice (SDBG_TRACE, Socket, Ch, Dimm, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                                "WriteRcdSmbDdr5: RcwPage 0x%0x, RcwAddress 0x%02x, Size %d, Data 0x%0x\n", PageNum, RegNum, Size, * (UINT8 *)CwData
                               );
      }

      break;

    case sizeof (UINT8):

      if (GetSlaveType (Socket, (UINT8) Dev.address.SmbInstance) == I2C_DEVICE) {

        if ((WriteProcSmb (Dev, 0x84, &Reserved) == EFI_SUCCESS) &&
            (WriteProcSmb (Dev, 0x04, &SubCh) == EFI_SUCCESS) &&
            (WriteProcSmb (Dev, 0x04, &PageNum) == EFI_SUCCESS) &&
            (WriteProcSmb (Dev, 0x04, &RegNum) == EFI_SUCCESS) &&
            (WriteProcSmb (Dev, 0x44, (UINT8 *) CwData) == EFI_SUCCESS)) {
          Status = SUCCESS;
        }
      } else {

        //
        // Use Block mode when I3C bus is in I3C mode
        //
        CommandLen = RCD_WRITE_BYTE_CMD;
        CommandList[0] = RCD_SB_CMD_WRITE_BYTE;  // Byte Write
        CommandList[1] = RCD_WRITE_BYTE_CMD - RCD_CMD_DATA_LEN_OFFSET; // Command length
        CommandList[2] = Reserved;
        CommandList[3] = SubCh;
        CommandList[4] = PageNum;
        CommandList[5] = RegNum;
        CommandList[6] =  *(UINT8 *) CwData;

        if (SmbSendCccCommand (Socket, Dev, NO_CCC, AttrNormalWrite, (UINT8 *) &CommandList, &CommandLen) == EFI_SUCCESS) {
          Status = SUCCESS;
        }
      }

      if (Status == SUCCESS) {
        WriteRcdCwCache (Socket, Ch, (1 << SubCh), Dimm, RDIMM_CW_PAGE0, DDR5_RCD_CW_PAGE_REG, PageNum);
        WriteRcdCwCache (Socket, Ch, (1 << SubCh), Dimm, PageNum, RegNum, *(UINT8 *)CwData);
        RcDebugPrintWithDevice (SDBG_TRACE, Socket, Ch, Dimm, SubCh, NO_RANK, NO_STROBE, NO_BIT,
                                "WriteRcdSmbDdr5: RcwPage 0x%0x, RcwAddress 0x%02x, Size %d, Data 0x%0x\n", PageNum, RegNum, Size, * (UINT8 *)CwData);
      }

      break;

    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_76);

    } //switch size
  } // SubCh

  return Status;
}

/**

  Program Signal CA vref internal function.

  @param Host        - Pointer to sysHost
  @param Socket      - Socket number
  @param Ch          - channel number
  @param Dimm        - DIMM number
  @param Subch       - sub channel number
  @param Signal      - Signal
  @param CaVref      - CA Vref Value
  @retval N/A

**/
UINT32
ProgramSignalCaVrefInternal (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Subch,
  GSM_CSN   Signal,
  UINT8     CaVref
  )
{
  UINT8             RwIndex = 0;
  UINT8             SubChMask = 1 << Subch;
  UINT32            SmbData;
  struct dimmNvram  (*DimmNvList) [MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  switch (Signal) {
    case A0A:
    case A0B:
      RwIndex = RDIMM_RW40;
      break;
    case A1A:
    case A1B:
      RwIndex = RDIMM_RW41;
      break;
    case A2A:
    case A2B:
      RwIndex = RDIMM_RW42;
      break;
    case A3A:
    case A3B:
      RwIndex = RDIMM_RW43;
      break;
    case A4A:
    case A4B:
      RwIndex = RDIMM_RW44;
      break;
    case A5A:
    case A5B:
      RwIndex = RDIMM_RW45;
      break;
    case A6A:
    case A6B:
      RwIndex = RDIMM_RW46;
      break;
    case PAR:
    case PARA:
    case PARB:
      RwIndex = RDIMM_RW47;
      break;
    case CS0_A_N:
    case CS0_B_N:
    case CS2_A_N:
    case CS2_B_N:
      RwIndex = RDIMM_RW48;
      break;
    case CS1_A_N:
    case CS1_B_N:
    case CS3_A_N:
    case CS3_B_N:
      RwIndex = RDIMM_RW49;
      break;
    default:
      return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
      break;
  }
  SmbData = CaVref;
  WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SubChMask, BYTE_WRITE, RDIMM_CW_PAGE0, RwIndex, &SmbData);

  return SUCCESS;
}

/**
  Program signal CA Vref.

  This function calls the correct function based on whether a DCPMM DIMM is
  present or not. For RcSim, it also calls a function to prime the feedback
  logic.

  @param[in]  Host        - Pointer to sysHost
  @param[in]  Socket      - Socket number
  @param[in]  Channel     - channel number
  @param[in]  Dimm        - DIMM number
  @param[in]  SubChannel  - sub-channel number
  @param[in]  Signal      - Signal
  @param[in]  CaVrefValue - CA Vref Value
**/
VOID
ProgramSignalCaVref (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     SubChannel,
  IN GSM_CSN   Signal,
  IN UINT8     CaVrefValue
  )
{
  UINT8        CaVref = CaVrefValue;

  if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
    GetSetDcaVrefDdrt2 (Host, Socket, Channel, Dimm, SubChannel, Signal, GSM_FORCE_WRITE, &CaVref);
  } else {
    ProgramSignalCaVrefInternal (Host, Socket, Channel, Dimm, SubChannel, Signal, CaVrefValue);
  }

}

/**

  Program QCK Delay.

  @param Rank        - Rank number
  @param Signal      - Signal
  @param Delay       - Delay

  @retval N/A

**/
UINT32
ProgramQckDelay(
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  GSM_CSN   Signal,
  UINT8     Mode,
  INT16     Delay
  )
{
  UINT32  Status = MRC_STATUS_SUCCESS;
  UINT8   SubChannel = 0;
  UINT8   SubChMask;
  UINT8   Rank = 0;
  UINT8   CwRegPage = 0;
  UINT8   CwRegAddress;
  UINT8   CwRegByte;
  UINT8   CwRegByteCached;
  UINT32  CwRegDword;
  UINT8   Size = BYTE_WRITE;
  INT16   MinLimit;
  INT16   MaxLimit;
  INT16   CurDelay;
  INT16   NewDelay;

  if (Signal < Q1ACK) {
    SubChannel = 0;
  } else {
    SubChannel = 1;
  }
  SubChMask = 1 << SubChannel;

  switch (Signal) {
  case Q0ACK:
    CwRegAddress = RDIMM_RW12;
    SubChannel = 0;
    break;
  case Q1ACK:
    CwRegAddress = RDIMM_RW12;
    SubChannel = 1;
    break;
  case Q0BCK:
    CwRegAddress = RDIMM_RW13;
    SubChannel = 0;
    break;
  case Q1BCK:
    CwRegAddress = RDIMM_RW13;
    SubChannel = 1;
    break;
  case Q0CCK:
    CwRegAddress = RDIMM_RW14;
    SubChannel = 0;
    break;
  case Q1CCK:
    CwRegAddress = RDIMM_RW14;
    SubChannel = 1;
    break;
  case Q0DCK:
    CwRegAddress = RDIMM_RW15;
    SubChannel = 0;
    break;
  case Q1DCK:
    CwRegAddress = RDIMM_RW15;
    SubChannel = 1;
    break;
  default:
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
    return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
    break;
  }

  MaxLimit = QCK_MAX_VAL;
  MinLimit = QCK_MIN_VAL;

  ReadRcdCwCache (Socket, Ch, SubChannel, Dimm, RDIMM_CW_PAGE0, CwRegAddress, &CwRegByteCached);

  if (Mode & GSM_READ_CSR) {
    //
    // Read from register
    //
    Status |= ReadRcdSmbByteDdr5 (Host, Socket, Ch, Dimm, SubChannel, CwRegPage, CwRegAddress, &CwRegByte);
  } else {
    //
    // Read from cache
    //
    CwRegByte = CwRegByteCached;
  }

  //
  // Mask off "Output Delay Feature Enable" field to obtain just the delay
  //
  CurDelay = CwRegByte & ~(QCK_OUTPUT_DELAY_EN);
  if (Mode & GSM_READ_ONLY) {

    //*Delay = CurDelay;

  } else { // GSM_READ_ONLY

    if (Mode & GSM_WRITE_OFFSET) {
      //
      // Program delay offset
      //
      NewDelay = CurDelay + Delay;

    } else { // GSM_WRITE_OFFSET
      //
      // Program delay value
      //
      NewDelay = Delay;
    }

    //
    // Handle min and max values
    //
    if (NewDelay < MinLimit) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
        "%a Out of range!! NewDelay = %d, MinLimit = %d\n", GetSignalStr (Signal), NewDelay, MinLimit);
      NewDelay = MinLimit;
      Status |= EFI_UNSUPPORTED;
    }
    if (NewDelay > MaxLimit) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
        "%a Out of range!! NewDelay = %d, MaxLimit = %d\n", GetSignalStr (Signal), NewDelay, MaxLimit);
      NewDelay = MaxLimit;
      Status |= EFI_UNSUPPORTED;
    }

    CwRegByte = (UINT8)NewDelay | QCK_OUTPUT_DELAY_EN;

    if ((CwRegByteCached != CwRegByte) || (Mode & GSM_FORCE_WRITE)) {
      CwRegDword = CwRegByte;
      Status |= WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SubChMask, Size, CwRegPage, CwRegAddress, &CwRegDword);
    }
  } // GSM_READ_ONLY

  //
  // When feature is enabled the delay settings in RW15[6:0] require a time of tODU for the delay to become stable on the outputs
  //
  //DDR5_TODO. To confirm--need to add a delay here so that it becomes stable?
  return (UINT32)Status;
}

/**

  Get/Set DDR5 QCSx Delay
  QCSx is the CSx Signal from the RCD to the DRAM
  Programs QCS delay depending on the Raw Card detected

  @param[in]      Host         - Pointer to SysHost
  @param[in]      Socket       - Socket number
  @param[in]      Ch           - Channel number
  @param[in]      Dimm         - DIMM number within the channel
  @param[in]      Rank         - DIMM Rank number
  @param[in]      Signal       - Signal name, valid signals are: Q0CS0_N, Q0CS1_N, Q0CS2_N, Q0CS3_N, Q1CS0_N, Q1CS1_N, Q1CS2_N, Q1CS3_N
  @param[in]      BacksideCopy - QCS A copy or B copy
  @param[in]      Mode         - Access Mode
  @param[in,out]  Delay        - Pointer to Delay value
  @param[in,out]  MinVal       - Pointer to a memory location that keep track of the minimum value
  @param[in,out]  MaxVal       - Pointer to a memory location that keep track of the maximum value

  @retval   - MRC_STATUS_SUCCESS if command completed successfully
  @retval   - !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
GetSetQcsDelay ( //to update backside a and b copy
  IN      PSYSHOST        Host,
  IN      UINT8           Socket,
  IN      UINT8           Ch,
  IN      UINT8           Dimm,
  IN      UINT8           Rank,
  IN      GSM_CSN         Signal,
  IN      UINT8           BacksideCopy,
  IN      UINT8           Mode,
  IN OUT  INT16           *Delay,
  IN OUT  INT16           *MinVal,  OPTIONAL
  IN OUT  INT16           *MaxVal   OPTIONAL
  )
{
  UINT32              Status = MRC_STATUS_SUCCESS;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  UINT8               SubChannel = 0;
  UINT8               SubChMask;
  UINT8               RcdRegisterAddress;
  UINT8               CwRegPage = 0;
  UINT8               CwRegByte;
  UINT8               CwRegByteCached;
  UINT32              CwRegDword;
  UINT8               Size = BYTE_WRITE;
  INT16               MinLimit;
  INT16               MaxLimit;
  INT16               CurDelay;
  INT16               NewDelay;
  RAW_CARD_SELECTION      RawCardSelection;
  QCX_SIGNAL_RAW_CARD_MAP *SelectedRawCard;
  UINT8               (*Data)[SUB_CH][MAX_STROBE_DDR5 / 2];


  if (Signal < QCS0_A_n || Signal > QCS3_B_n) {
    return EFI_UNSUPPORTED;
  }

  if (Signal < QCS0_B_n) {
    SubChannel = 0;
  } else {
    SubChannel = 1;
  }
  SubChMask = 1 << SubChannel;

  RawCardSelection.Data = 0;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  if ((*DimmNvList) [Dimm].numRanks >= 2) {
    RawCardSelection.Bits.DRSR = 1;
  }
  if ((*DimmNvList) [Dimm].x4Present) {
    RawCardSelection.Bits.X4X8 = 1;
  }

  SelectedRawCard = RawCards[RawCardSelection.Data];

  MaxLimit = QCS_MAX_VAL;
  MinLimit = QCS_MIN_VAL;

    //
    // The cache index may not always be the same as the register address, but it currently is the same.
    //
    RcdRegisterAddress = SelectedRawCard[Rank].RcdRegisters[BacksideCopy];
    ReadRcdCwCache (Socket, Ch, SubChannel, Dimm, RDIMM_CW_PAGE0, RcdRegisterAddress, &CwRegByteCached);

    if (Mode & GSM_READ_CSR) {
      //
      // Read from register
      //
      Status |= ReadRcdSmbByteDdr5 (Host, Socket, Ch, Dimm, SubChannel, CwRegPage, RcdRegisterAddress, &CwRegByte);

      if (FeaturePcdGet (PcdCteBuild) == TRUE) {
        Data = RcAllocatePool (sizeof (*Data));
        if (Data == NULL) {
          return MRC_STATUS_FAILURE;
        }
        ZeroMem ((UINT8 *) (Data), sizeof ((*Data)));
        ReadDbMr (Socket, Ch, SubChMask, Dimm, Rank, RcdRegisterAddress, Data);
        CwRegByte = (*Data)[SubChannel][LOGICAL_STROBE_0];
        RcFreePool (Data);
      }

    } else {
      //
      // Read from cache
      //
      CwRegByte = CwRegByteCached;
    }

    //
    // Mask off "Output Delay Feature Enable" field to obtain just the delay
    //
    CurDelay = CwRegByte & ~(QCS_OUTPUT_DELAY_EN);
    if (Mode & GSM_READ_ONLY) {

      *Delay = CurDelay;

    } else { // GSM_READ_ONLY

      if (Mode & GSM_WRITE_OFFSET) {
        //
        // Program delay offset
        //
        NewDelay = CurDelay + *Delay;

      } else { // GSM_WRITE_OFFSET
        //
        // Program delay value
        //
        NewDelay = *Delay;
      }

      //
      // Handle min and max values
      //
      if (NewDelay < MinLimit) {
        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
          "%a Out of range!! NewDelay = %d, MinLimit = %d\n", GetSignalStr (Signal), NewDelay, MinLimit
          );
        NewDelay = MinLimit;
        Status |= EFI_UNSUPPORTED;
      }
      if (NewDelay > MaxLimit) {
        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
          "%a Out of range!! NewDelay = %d, MaxLimit = %d\n", GetSignalStr (Signal), NewDelay, MaxLimit
          );
        NewDelay = MaxLimit;
        Status |= EFI_UNSUPPORTED;
      }

      //
      // Write it back if the current data does not equal the cache value or if in force write Mode
      //
      if (MinVal != NULL && MaxVal != NULL) {
        UpdateMinMaxInt (NewDelay, MinVal, MaxVal);
      }
      //
      // Set "Output Delay Feature Enable" field when writing the delay
      //
      CwRegByte = (UINT8)NewDelay | QCS_OUTPUT_DELAY_EN;

      if ((CwRegByteCached != CwRegByte) || (Mode & GSM_FORCE_WRITE)) {
        CwRegDword = CwRegByte;
        Status |= WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SubChMask, Size, CwRegPage, RcdRegisterAddress, &CwRegDword);
      }
    } // GSM_READ_ONLY

    //
    // Update cache
    //
    if (Mode & GSM_UPDATE_CACHE) {
      WriteRcdCwCache (Socket, Ch, SubChMask, Dimm, RDIMM_CW_PAGE0, RcdRegisterAddress, CwRegByte);
    }

  return Status;
}

/**
  Perform DDR5 Rambus D0 errata

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number

  @retval  TRUE  - Errata is applied
           FALSE - Errata is not applied
**/
BOOLEAN
EFIAPI
Ddr5RambusD0Errata (
  IN  PSYSHOST Host,
  IN  UINT8    Socket
  )
{
  BOOLEAN                  WorkaroundApplied;
  UINT8                    Channel;
  UINT8                    Dimm;
  UINT8                    MaxChDdr;
  UINT32                   SmbData;
  UINT8                    ChannelIncrement;
  struct dimmNvram         (*DimmNvList)[MAX_DIMM];
  struct channelNvram      (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr = GetMaxChDdr ();
  ChannelIncrement = GetChInc (Host);

  WorkaroundApplied = FALSE;
  for (Channel = 0; Channel < MaxChDdr; Channel += ChannelIncrement) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      DimmNvList = GetDimmNvList(Host, Socket, Channel);
      if (((*DimmNvList)[Dimm].SPDRegVen == MFGID_RAMBUS) && ((*DimmNvList)[Dimm].SPDRegRev == RAMBUS_SPD_REGREV_D0)) {

        RcDebugPrintWithDevice ( SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Rambus DRST WA for D0\n");

        SmbData = 0x04;
        WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGEFB, RDIMM_RW68, &SmbData);

        //100 microsec delay
        FixedDelayMicroSecond (100);

        ReadRcdSmbDdr5 (Host, Socket, Channel, Dimm, SUB_CH_A, RDIMM_CW_PAGEFB, RDIMM_RW68, &SmbData);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "SmbData = 0x%x\n", SmbData);

        SmbData = 0x02;
        WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, RDIMM_RW31, &SmbData);
        SmbData = 0x50;
        WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE9A, RDIMM_RW6D, &SmbData);
        SmbData = 0x60;
        WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGEFE, RDIMM_RW78, &SmbData);
        SmbData = 0x44;
        WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE9B, RDIMM_RW60, &SmbData);

        //200 microsec delay
        FixedDelayMicroSecond (200);

        SmbData = 0x0;
        WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, RDIMM_RW31, &SmbData);
        SmbData = 0x0;
        WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, RDIMM_RW31, &SmbData);
        SmbData = 0x60;
        WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE9A, RDIMM_RW6D, &SmbData);
        SmbData = 0x0;
        WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGEFE, RDIMM_RW78, &SmbData);
        SmbData = 0x40;
        WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE9B, RDIMM_RW60, &SmbData);
        WorkaroundApplied = TRUE;
      } //Rambus D0
    }//Dimm
  }//Channel
  return WorkaroundApplied;
}

/**
  Display RCD control words by reading directly using sideband access

  @param[in]  Socket      - Socket number

  @retval   N/A
**/
VOID
EFIAPI
DumpRcdControlWords (
  IN  UINT8     Socket
  )
{
#ifdef DEBUG_CODE_BLOCK
  PSYSHOST                    Host;
  UINT32                      Status;
  UINT8                       Ch;
  UINT8                       SubCh;
  UINT8                       Dimm;
  UINT8                       MaxChDdr;
  UINT8                       Page;
  UINT8                       PageCount;
  UINT8                       PageLength;
  UINT8                       PagedAddressStart;
  UINT8                       PagedAddressMax;
  UINT8                       Address;
  UINT8                       *DataByte;
  UINT32                      DataDword;
  UINT16                      DirectAddressMax;
  UINT8                       PrevChannelModeOperation;
  UINT32                      DbgLevel;

  Host = GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  //
  // Every print in this function should use the same level.
  //
  DbgLevel = SDBG_MAX;

  //
  // If this function can't print, don't bother to read anything, but
  // return early instead.
  //
  if (!CheckDebugPrintLevel (DbgLevel)) {
    return;
  }

  PrevChannelModeOperation = ModeDdr5SubChannel;
  if (IsBrsPresent (Host, Socket)) {
    PrevChannelModeOperation = Host->ChannelModeOperation;
    Host->ChannelModeOperation= ModeDdr5FullChannel;
  }

  RcDebugPrint (DbgLevel, "\nDDR5 RCD CW Data\n");

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      Page = 0;

        DirectAddressMax = MAX_RCD_DIRECT_CONTROL_WORDS;
        for (Address = 0; Address < DirectAddressMax; Address += sizeof (DataDword)) {
          if ((Address % 16) == 0) {
            RcDebugPrint (DbgLevel, "\n");
            RcDebugPrintWithDevice (DbgLevel, Socket, Ch, Dimm, SubCh, NO_RANK, NO_STROBE, NO_BIT, "      RW%02x: ", Address);
          }
          Status = ReadRcdSmbDdr5 (Host, Socket, Ch, Dimm, SubCh, Page, Address, &DataDword);

          if (Status == SUCCESS) {
            DataByte = (UINT8 *)&DataDword;
            RcDebugPrint (DbgLevel, " 0x%02x 0x%02x 0x%02x 0x%02x", DataByte[0], DataByte[1], DataByte[2], DataByte[3]);
          } else {
            RcDebugPrint (DbgLevel, "   -    -    -    - ");
          }
        } // Address

        PageCount = MAX_RCD_PAGES;
        PageLength = MAX_RCD_CONTROL_WORDS_PER_PAGE;
        PagedAddressStart = MAX_RCD_DIRECT_CONTROL_WORDS;
        PagedAddressMax = PageLength + MAX_RCD_DIRECT_CONTROL_WORDS;

        for (Page = 0; Page < PageCount; Page++) {

          for (Address = PagedAddressStart; Address < PagedAddressMax; Address += sizeof (DataDword)) {
            if ((Address % 16) == 0) {
              RcDebugPrint (DbgLevel, "\n");
              RcDebugPrintWithDevice (DbgLevel, Socket, Ch, Dimm, SubCh, NO_RANK, NO_STROBE, NO_BIT, "PG[%02x]RW%02x: ", Page, Address);
            }
            Status = ReadRcdSmbDdr5 (Host, Socket, Ch, Dimm, SubCh, Page, Address, &DataDword);

            if (Status == SUCCESS) {
              DataByte = (UINT8 *)&DataDword;
              RcDebugPrint (DbgLevel, " 0x%02x 0x%02x 0x%02x 0x%02x", DataByte[0], DataByte[1], DataByte[2], DataByte[3]);
            } else {
              RcDebugPrint (DbgLevel, "   -    -    -    - ");
            }
          } // Address
        } // Page
        RcDebugPrint (DbgLevel, "\n");
      } // SubCh

    } // Dimm
  } // Ch

  if (IsBrsPresent (Host, Socket)) {
    Host->ChannelModeOperation= PrevChannelModeOperation;
  }
#endif // DEBUG_CODE_BLOCK
}

