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
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/SpdAccessLib.h>
#include <Library/SpdDecodeLib.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/Ddr4SpdRegisters.h>

//
// Local Prototypes
//
VOID   GetDimmTimeBaseDDR4 (PSYSHOST Host,UINT8 spdmtb,UINT8 spdftb,INT32 * mtb,INT32 * ftb);

/**
  Store the common DDR4 DIMM configuration in the Host structure

  @param[in,out] Host   - Pointer to sysHost
  @param[in]     Socket - CPU Socket ID
  @param[in]     Ch     - Channel to be accessed
  @param[in]     Dimm   - DIMM to be accessed

  @retval N/A

**/
VOID
GetCommonDDR4DIMMConfig (
  IN OUT PSYSHOST Host,
  IN UINT8        Socket,
  IN UINT8        Ch,
  IN UINT8        Dimm
  )
{
  UINT8  Length = 1;
  UINT8  SPDReg;
  INT32  mediumTimebase;
  INT32  fineTimebase;
  struct dimmDevice (*ChdimmList)[MAX_DIMM];
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  struct channelNvram  (*ChannelNvList)[MAX_CH];

  ChdimmList = GetChDimmList(Host, Socket, Ch);
  DimmNvList = GetDimmNvList(Host, Socket, Ch);
  ChannelNvList = GetChannelNvList(Host, Socket);

  // FTB
  SpdReadByte (Socket, Ch, Dimm, SPD_TB_DDR4, &SPDReg);
  SPDReg = SPDReg & 0x0F; //mask out reserved bits 7-4
  switch (SPDReg & 3) {
  case 0:
    (*DimmNvList)[Dimm].SPDftb = 1;    // 1 picosecond
    break;
    default:
    //Automated add of default case. Please review.
    break;
  }

  // MTB
  switch ((SPDReg >> 2) & 3) {
  case 0:
    (*DimmNvList)[Dimm].SPDmtb = 125;    //125 picoseconds
    break;
    default:
    //Automated add of default case. Please review.
    break;
  }

  //
  // Get Min TCK
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_MIN_TCK_DDR4, &SPDReg);
  (*DimmNvList)[Dimm].minTCK = SPDReg;

  //
  // Get FTB Min TCK
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_FTB_MIN_TCK_DDR4, &SPDReg);
  (*DimmNvList)[Dimm].ftbTCK = SPDReg;

  GetDimmTimeBaseDDR4 (Host, (*DimmNvList)[Dimm].SPDmtb, (*DimmNvList)[Dimm].SPDftb, &mediumTimebase, &fineTimebase);

  if ((*DimmNvList)[Dimm].ftbTCK & 0x80) {
    (*DimmNvList)[Dimm].minTCK = (mediumTimebase * (INT32) (*DimmNvList)[Dimm].minTCK) +
      (fineTimebase * ((INT32) (*DimmNvList)[Dimm].ftbTCK - 256));
  } else {
    (*DimmNvList)[Dimm].minTCK = (mediumTimebase * (INT32) (*DimmNvList)[Dimm].minTCK) +
      (fineTimebase * (INT32) (*DimmNvList)[Dimm].ftbTCK);
  }
  (*ChdimmList)[Dimm].minTCK = (*DimmNvList)[Dimm].minTCK;

  //
  // Get Vdd DIMM Support Info (Only support 1.2v for DDR4)
  //
  SpdReadField (Socket, Ch, Dimm, SPD_INDEX_DRAM_VDD_1_2_V, &SPDReg, &Length);
  if (SPDReg & SPD_VDD_120) {
    Host->nvram.mem.socket[Socket].ddrVoltage = SPD_VDD_120;
  } else {
    (*ChannelNvList)[Ch].v120NotSupported = 1;
    Host->nvram.mem.socket[Socket].ddrVoltage = SPDReg;
  }
} // GetCommonDDR4DIMMConfig

/**

  Converts MTB/FTB to DCLK for DDR4

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param Socket   - Processor socket within the system (0-based)
  @param Ch       - DDR channel number within the processor socket (0-based)
  @param Dimm     - DIMM number within a DDR channel (0-based, farthest slot == 0)
  @param MtbTime  - Time in MTB (Medium Time Base) for DDR4
  @param FtbTime  - Time in FTB (Fine Time Base)   for DDR4

  @retval Time in DCLKs

**/
UINT16
TimeBaseToDCLKDdr4 (
  PSYSHOST  Host,       // Pointer to Host structure (outside scope of this spec)
  UINT8     Socket,     // Processor socket within the system (0-based)
  UINT8     Ch,         // DDR channel number within the processor socket (0-based)
  UINT8     Dimm,       // DIMM number within a DDR channel (0-based, farthest slot == 0)
  UINT32    MtbTime,    // Time in MTB (Medium Time Base)
  UINT16    FtbTime     // Time in FTB (Fine Time Base)
  )
{
  UINT16              Mtb;
  UINT16              Ftb;
  UINT32              TempVal;

  Mtb = 1250;
  Ftb = 10;

  if (FtbTime & BIT7) {
    TempVal = (UINT32) ((MtbTime * Mtb) + ((FtbTime - 256) * Ftb));
  } else {
    TempVal = (UINT32) ((MtbTime * Mtb) + (FtbTime * Ftb));
  }

  //
  // Round up to next cycle subtracting the guardband of 0.01 clocks described in the SPD specification
  //
  TempVal = ((TempVal + GettCK (Host, Socket) - 10) * 100) / GettCK (Host, Socket);

  //
  // 0.01 clocks guardband
  //
  TempVal -= 1;
  TempVal = TempVal / 100;

  return (UINT16) TempVal;
} // TimeBaseToDCLKDdr4

/**

  Detect DDR4 configuration

  @param Host   - Point to sysHost
  @param socket - CPU socket ID
  @param ch     - channel to be accessed
  @param dimm   - DIMM to be accessed

  @retval SUCCESS

**/
UINT32
DetectDDR4DIMMConfig (
  PSYSHOST         Host,
  UINT8            socket,
  UINT8            ch,
  UINT8            dimm
  )
{

#ifndef DDR5_SUPPORT

  UINT8   SPDReg;
  UINT8   DieCount;
  UINT8   DieCountSecondary;
  UINT8   NumColBits;
  UINT8   NumRowBits;
  UINT8   NumBankGroupBits;
  UINT8   NumBankAddressBits;
  UINT8   SdramCapacity;
  UINT8   DramIOWidth;
  BOOLEAN RankMix;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];

  dimmNvList = GetDimmNvList (Host, socket, ch);
  channelNvList = GetChannelNvList (Host, socket);

  GetCommonDDR4DIMMConfig (Host, socket, ch, dimm);

  //
  // Read SPD Bytes containing DDR module info
  //
  SpdReadByte (socket, ch, dimm, SPD_KEY_BYTE2, &SPDReg);
  (*dimmNvList)[dimm].keyByte2 = SPDReg & 0xF;
  (*dimmNvList)[dimm].actKeyByte2 = SPDReg & 0xF;

  if (SPDReg == SPD_ECC_SO_UDIMM_DDR4) {
    // Take SODIMM path, Width will be checked for ECC.
    (*dimmNvList)[dimm].keyByte2 = SPD_SODIMM;
    (*dimmNvList)[dimm].actKeyByte2 = SPD_SODIMM;
  }

  if (SPDReg == SPD_ECC_SO_RDIMM_DDR4) {
    // Take SODIMM path, Width will be checked for ECC.
    (*dimmNvList)[dimm].keyByte2 = SPD_RDIMM;
    (*dimmNvList)[dimm].actKeyByte2 = SPD_RDIMM;
  }

  if ((SPDReg & 0xF) == SPD_MINI_RDIMM) {
    // Take RDIMM path for MiniRDIMM
    (*dimmNvList)[dimm].keyByte2 = SPD_RDIMM;
    (*dimmNvList)[dimm].actKeyByte2 = SPD_RDIMM;
  }

#ifdef LRDIMM_SUPPORT
  if ((SPDReg & 0xF) == SPD_LRDIMM_DDR4) {
    (*dimmNvList)[dimm].lrDimmPresent = 1;
    (*channelNvList)[ch].lrDimmPresent = 1;
    Host->nvram.mem.socket[socket].lrDimmPresent = 1;

    // Take RDIMM path to minimize changes
    (*dimmNvList)[dimm].keyByte2 = SPD_RDIMM;
  }
#endif

  if (PcdGetBool (PcdNvDimmEn)) {
    (*dimmNvList)[dimm].NvDimmNPresent = DetectJedecDdr4NvdimmN (socket, ch, dimm);
  } else {
    (*dimmNvList)[dimm].NvDimmNPresent = FALSE;
  }

  //
  // Get SDRAM Capacity
  //
  SpdGetEncodedDramCapacity (socket, ch, dimm, PrimarySdramNumber, &SdramCapacity);
  (*dimmNvList)[dimm].sdramCapacity = SdramCapacity;
  //
  // Get the number of bank address bits
  //
  SpdGetBankAddressBits (socket, ch, dimm, PrimarySdramNumber, &NumBankAddressBits);
  (*dimmNvList)[dimm].numBankAddressBits = NumBankAddressBits;
  //
  // Get the number of bank group bits
  //
  SpdGetBankGroupBits (socket, ch, dimm, PrimarySdramNumber, &NumBankGroupBits);
  (*dimmNvList)[dimm].numBankGroupBits = NumBankGroupBits;
  //
  // Get number of row bits
  //

  SpdGetNumRowBits (socket, ch, dimm, PrimarySdramNumber, &NumRowBits);
  (*dimmNvList)[dimm].numRowBits = NumRowBits;

  //
  // Get number of column bits
  //
  SpdGetNumColBits (socket, ch, dimm, PrimarySdramNumber, &NumColBits);
  (*dimmNvList)[dimm].numColBits = NumColBits;

  //
  // Get Number of ranks and device width
  //
  SpdReadByte (socket, ch, dimm, SPD_MODULE_ORGANIZATION_REG, &SPDReg);
  SPDReg = SPDReg & 0X7F; //mask out reserved bits 7
  (*dimmNvList)[dimm].SPDModuleOrg = SPDReg;
  (*dimmNvList)[dimm].actSPDModuleOrg = SPDReg;
  //
  // Get the encoded value of dram IO width
  //
  SpdGetEncodedDramIOWidth (socket, ch, dimm, PrimarySdramNumber, &DramIOWidth);
  (*dimmNvList)[dimm].dramIOWidth = DramIOWidth;
  //
  // Get Primary SDRAM Device Type
  //
  SpdReadByte (socket, ch, dimm, SPD_PRIMARY_SDRAM_PACKAGE_TYPE_REG, &SPDReg);
  (*dimmNvList)[dimm].SPDDeviceType = SPDReg & 0xF3; //mask out reserved bits 3-2
  //
  // Get Primary SDRAM die count
  //
  SpdGetDramDieCount (socket, ch, dimm, PrimarySdramNumber, &DieCount);
  (*dimmNvList)[dimm].dieCount = DieCount;

  //
  // Get Raw Card Number
  //
  SpdReadByte (socket, ch, dimm, SPD_REF_RAW_CARD_DDR4, &SPDReg);
  (*dimmNvList)[dimm].SPDRawCard = SPDReg;
  //
  // Get Bus Width
  //
  SpdReadByte (socket, ch, dimm, SPD_MEM_BUS_WID_DDR4, &SPDReg);
  SPDReg = SPDReg & 0x1F; //mask out reserved bits 7-5
  (*dimmNvList)[dimm].SPDMemBusWidth = SPDReg;

  //
  // Get rank mix indicator of DIMM
  //
  SpdGetRankMixIndicator (socket, ch, dimm, &RankMix);
  (*dimmNvList)[dimm].rankMix = RankMix;

  if ((*dimmNvList)[dimm].rankMix) {
    //
    // Get Secondary SDRAM Device Type
    //
    SpdReadByte (socket, ch, dimm, SPD_SECONDARY_SDRAM_PACKAGE_TYPE_REG, &SPDReg);
    (*dimmNvList)[dimm].SPDSecondaryDeviceType = SPDReg;
    //
    // Get Secondary SDRAM die count
    //
    SpdGetDramDieCount (socket, ch, dimm, SecondarySdramNumber, &DieCountSecondary);
    (*dimmNvList)[dimm].dieCountSecondary = DieCountSecondary;

  }

#endif // #ifndef DDR5_SUPPORT

  return SUCCESS;
}


/**

  Collects data from SPD for DDR4 LRDIMM

  @param[in] Host   - Point to sysHost
  @param[in] Socket - CPU socket ID
  @param[in] Ch     - Channel to be accessed
  @param[in] Dimm   - DIMM to be accessed

  @retval SUCCESS

**/
UINT32
GetLrdimmSpdDataDdr4 (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Dimm
  )
{
#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  UINT8  SPDReg;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // DRAM VrefDQ for Package Rank 0
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DRAM_VREFDQ_R0_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDramVrefdqR0 = SPDReg & 0x3F; //mask out reserved bits 6-7
  //
  // DRAM VrefDQ for Package Rank 1
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DRAM_VREFDQ_R1_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDramVrefdqR1 = SPDReg & 0x3F; //mask out reserved bits 6-7
  //
  // DRAM VrefDQ for Package Rank 2
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DRAM_VREFDQ_R2_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDramVrefdqR2 = SPDReg & 0x3F; //mask out reserved bits 6-7
  //
  // DRAM VrefDQ for Package Rank 3
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DRAM_VREFDQ_R3_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDramVrefdqR3 = SPDReg & 0x3F; //mask out reserved bits 6-7
  //
  // LR Data Buffer VrefDQ for DRAM Interface
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DB_VREFDQ_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDbVrefdq = SPDReg + DB_DRAM_VREF_RANGE2_OFFSET;
  //
  // LR Data Buffer MDQ Drive Strength and RTT for data rate <= 1866
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DB_DS_RTT_LE1866_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDbDsRttLe1866 = SPDReg;
  //
  // LR Data Buffer MDQ Drive Strength and RTT for data rate > 1866 and <= 2400
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DB_DS_RTT_GT1866_LE2400_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDbDsRttGt1866Le2400 = SPDReg;
  //
  // LR Data Buffer MDQ Drive Strength and RTT for data rate > 2400 and <= 3200
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DB_DS_RTT_GT2400_LE3200_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDbDsRttGt2400Le3200 = SPDReg;
  //
  // LR Buffer DRAM Drive Strength (for data rates <1866, 1866 < data rate < 2400, and 2400 < data rate < 3200)
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DRAM_DS_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDramDs = SPDReg & 0x3F; //mask out reserved bits 6-7
  //
  // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate <= 1866
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DRAM_ODT_WR_NOM_LE1866_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDramOdtWrNomLe1866 = SPDReg & 0x3F; //mask out reserved bits 6-7
  //
  // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 1866 and <= 2400
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DRAM_ODT_WR_NOM_GT1866_LE2400_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDramOdtWrNomGt1866Le2400 = SPDReg & 0x3F; //mask out reserved bits 6-7
  //
  // LR Buffer DRAM ODT (RTT_WR and RTT_NOM) for data rate > 2400 and <= 3200
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DRAM_ODT_WR_NOM_GT2400_LE3200_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDramOdtWrNomGt2400Le3200 = SPDReg & 0x3F; //mask out reserved bits 6-7
  //
  // LR Buffer DRAM ODT (RTT_PARK) for data rate <= 1866
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DRAM_ODT_PARK_LE1866_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDramOdtParkLe1866 = SPDReg & 0x3F; //mask out reserved bits 6-7
  //
  // LR Buffer DRAM ODT (RTT_PARK) for data rate > 1866 and <= 2400
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DRAM_ODT_PARK_GT1866_LE2400_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDramOdtParkGt1866Le2400 = SPDReg & 0x3F; //mask out reserved bits 6-7
  //
  // LR Buffer DRAM ODT (RTT_PARK) for data rate > 2400 and <= 3200
  //
  SpdReadByte (Socket, Ch, Dimm, SPD_LRBUF_DRAM_ODT_PARK_GT2400_LE3200_DDR4, &SPDReg);
  (*dimmNvList)[Dimm].SPDLrbufDramOdtParkGt2400Le3200 = SPDReg & 0x3F; //mask out reserved bits 6-7

  return SUCCESS;
#else // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  return FAILURE;
#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
}


/**
  Calculate the medium and fine timebases, using integer math.

  @param mtb     - Location to store the calculated medium timebase, in femtoseconds.
                   Example: mtb = 125ps. ((125/1,000) * 1,000,000) = 125,000
  @param ftb     - Location to store the calculated fine timebase, in femtoseconds.
                   Example: ftb = 1ps.  (1 * 1,000) = 1,000

  @retval N/A

**/
VOID
GetDimmTimeBaseDDR4 (
  PSYSHOST  Host,
  UINT8     spdmtb,
  UINT8     spdftb,
  INT32     * mtb,
  INT32     * ftb
  )
{
  if (spdftb > 0) {
    *ftb = spdftb * FREQUENCY_FTB_OFFSET;
  } else {
    *ftb = 0;
  }

  if (spdmtb > 0) {
    *mtb = spdmtb * (FREQUENCY_MTB_OFFSET/1000);
  } else {
    *mtb = 0;
  }
} // GetDimmTimeBaseDDR4

VOID
InitDDRVoltageDDR4 (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  //
  // Stubbed out as this is not necessary for DDR4
  //
}

UINT32
GatherSPDDataDDR4 (
  VOID
  )
/*++

  Collects data from SPD for DDR4

  @param Host  - Point to sysHost

  @retval SUCCESS

--*/
{
  PSYSHOST            Host;
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT8               SPDReg;
  UINT8               ftbVal;
  UINT8               i;
  UINT16              cycles;
  UINT8               pageSize = 0;
  UINT8               ti;
  UINT32              casSup = 0;
  UINT16              tRC;
  UINT16              tRAS;
  UINT16              tRFC;
  UINT8               tFAW;
  UINT8               tWTRL;
  UINT8               tWTR;
  UINT8               tWR;
  UINT8               tRTP;
  UINT8               reservedBits_SPDReg;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
#ifdef DEBUG_CODE_BLOCK
  struct dimmDevice   (*chdimmList)[MAX_DIMM];
#endif  // DEBUG_CODE_BLOCK
  SYS_SETUP *Setup;
  UINT8     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  Host = (PSYSHOST) GetSysHostPointer ();
  socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  //
  // Don't access the SPD if not necessary
  //
  if ((Host->nvram.mem.DataGood != 1) || ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot))) {

    //
    // Only gather SPD info if we don't already have it
    //
    channelNvList = GetChannelNvList (Host, socket);

    // Check strap addresses on each channel moving near to far
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);
#ifdef DEBUG_CODE_BLOCK
      chdimmList = &Host->var.mem.socket[socket].channelList[ch].dimmList;
#endif  // DEBUG_CODE_BLOCK

      //
      // Initialize to all CAS Latencies supported. But DDR4 only use 4 bytes
      //
      (*channelNvList)[ch].common.casSup = MAX_UINT64;

      //Initialize dimmrevtype to 1
      (*channelNvList)[ch].dimmRevType = RCD_REV_2;

      //
      // Initialize to ASR supported. This bit will get cleared if any DIMM on the channel does not support ASR
      //
      (*channelNvList)[ch].features |= ASR_SUPPORT;

      (*channelNvList)[ch].tCCDAdder = 0;
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        //
        // Get SPD Spec revision
        //
        SpdReadByte (socket, ch, dimm, SPD_REVISION, &SPDReg);
        (*dimmNvList)[dimm].SPDSpecRev = SPDReg;

        if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
          //
          // Lookup geometry in DIMM technology table to determine page size

          ti = GetTechIndex (socket, ch, dimm);
          switch (ti) {
          case 0:
          case 3:
          case 6:
          case 9:
            pageSize = PAGE_SIZE_HALFK; // 1/2K Page Size
            break;
          case 1:
          case 4:
          case 7:
          case 10:
            pageSize = PAGE_SIZE_1K;    // 1K Page Size
            break;
          case 2:
          case 5:
          case 8:
          case 11:
            pageSize = PAGE_SIZE_2K;    // 2K Page Size
            break;
            default:
            //Automated add of default case. Please review.
            break;
          }

          tFAW = (UINT8) GettFAW (Host, socket, ch, pageSize);

          if ((*channelNvList)[ch].common.nFAW < tFAW) {
            (*channelNvList)[ch].common.nFAW = tFAW;
          }

          //
          // Use lookup table for tWTR_L
          //
          tWTRL = (UINT8) GettWTR_L (Host, socket, ch);

          if ((*channelNvList)[ch].common.nWTR_L < tWTRL) {
            (*channelNvList)[ch].common.nWTR_L = tWTRL;
          }

          //
          // Use lookup table for tWTR_S
          //
          tWTR = (UINT8) GettWTR_S (Host, socket, ch);

          if ((*channelNvList)[ch].common.nWTR < tWTR) {
            (*channelNvList)[ch].common.nWTR = tWTR;
          }

          //
          // Use lookup table for tWR
          //
          tWR = (UINT8) GettWR (Host, socket, ch);
          if ((*channelNvList)[ch].common.nWR < tWR) {
            (*channelNvList)[ch].common.nWR = tWR;
          }

          //
          // Use lookup table for tRTP
          //
          tRTP = (UINT8) GettRTP (Host, socket, ch);
          if ((*channelNvList)[ch].common.nRTP < tRTP) {
            (*channelNvList)[ch].common.nRTP = tRTP;
          }

          //
          // Get tRRD_S
          //
          SpdReadByte (socket, ch, dimm, SPD_MIN_TRRDS_DDR4, &SPDReg);
          SpdReadByte (socket, ch, dimm, SPD_FTB_TRRDS_DDR4, &ftbVal);

          if ((*channelNvList)[ch].common.tRRD < SPDReg) {
            (*channelNvList)[ch].common.tRRD = SPDReg;

            // Convert to number of cycles
            cycles = TimeBaseToDCLKDdr4 (Host, socket, ch, dimm, (UINT16) SPDReg, ftbVal);
            if ((*channelNvList)[ch].common.nRRD < cycles) {
              (*channelNvList)[ch].common.nRRD = (UINT8) cycles;
            }
          }

          //
          // Get tRRD_L

          SpdReadByte (socket, ch, dimm, SPD_MIN_TRRDL_DDR4, &SPDReg);
          SpdReadByte (socket, ch, dimm, SPD_FTB_TRRDL_DDR4, &ftbVal);

          if ((*channelNvList)[ch].common.tRRD_L < SPDReg) {
            (*channelNvList)[ch].common.tRRD_L = SPDReg;

            // Convert to number of cycles
            cycles = TimeBaseToDCLKDdr4 (Host, socket, ch, dimm, (UINT16) SPDReg, ftbVal);
            if ((*channelNvList)[ch].common.nRRD_L < cycles) {
              (*channelNvList)[ch].common.nRRD_L = (UINT8) cycles;
            }
          }

          //
          // Get tRP
          //
          SpdReadByte (socket, ch, dimm, SPD_MIN_TRP_DDR4, &SPDReg);
          SpdReadByte (socket, ch, dimm, SPD_FTB_TRP_DDR4, &ftbVal);
#ifdef  DEBUG_CODE_BLOCK
          (*chdimmList)[dimm].tRP = SPDReg;
#endif  // DEBUG_CODE_BLOCK
          (*channelNvList)[ch].dimmList[dimm].tRP = SPDReg;
          (*channelNvList)[ch].dimmList[dimm].ftbTRP = ftbVal;

          if ((*channelNvList)[ch].common.tRP < SPDReg) {
            (*channelNvList)[ch].common.tRP = SPDReg;

            // Convert to number of cycles
            cycles = TimeBaseToDCLKDdr4 (Host, socket, ch, dimm, (UINT16) SPDReg, ftbVal);

            if ((*channelNvList)[ch].common.nRP < cycles) {
              (*channelNvList)[ch].common.nRP = (UINT8) cycles;
            }
          }

          //
          // Get upper nibble of tRC and tRAS
          //
          SpdReadByte (socket, ch, dimm, SPD_EXT_TRC_TRAS_DDR4, &SPDReg);
          tRC = (UINT16) (SPDReg & 0xF0) << 4;
          tRAS = (UINT16) (SPDReg & 0x0F) << 8;

          //
          // Get lower byte of tRAS
          //

          SpdReadByte (socket, ch, dimm, SPD_MIN_TRAS_DDR4, &SPDReg);
          tRAS = tRAS | (UINT16) SPDReg;
          if ((*channelNvList)[ch].common.tRAS < tRAS) {
            (*channelNvList)[ch].common.tRAS = tRAS;

            // Convert to number of cycles
            cycles = TimeBaseToDCLKDdr4 (Host, socket, ch, dimm, tRAS, 0);
            if ((*channelNvList)[ch].common.nRAS < cycles) {
              (*channelNvList)[ch].common.nRAS = (UINT8) cycles;
            }
          }

          //
          // Get lower byte of tRC
          //
          SpdReadByte (socket, ch, dimm, SPD_MIN_TRC_DDR4, &SPDReg);
          SpdReadByte (socket, ch, dimm, SPD_FTB_TRC_DDR4, &ftbVal);
          tRC = tRC | (UINT16) SPDReg;
          if ((*channelNvList)[ch].common.tRC < tRC) {
            (*channelNvList)[ch].common.tRC = tRC;

            // Convert to number of cycles
            cycles = TimeBaseToDCLKDdr4 (Host, socket, ch, dimm, tRC, ftbVal);
            if ((*channelNvList)[ch].common.nRC < cycles) {
              (*channelNvList)[ch].common.nRC = (UINT8) cycles;
            }
          }

          //
          // Get tRCD
          //
          SpdReadByte (socket, ch, dimm, SPD_MIN_TRCD_DDR4, &SPDReg);
          SpdReadByte (socket, ch, dimm, SPD_FTB_TRCD_DDR4, &ftbVal);
#ifdef  DEBUG_CODE_BLOCK
          (*chdimmList)[dimm].tRCD = SPDReg;
#endif  // DEBUG_CODE_BLOCK
          (*channelNvList)[ch].dimmList[dimm].tRCD = SPDReg;
          (*channelNvList)[ch].dimmList[dimm].ftbTRCD = ftbVal;

          if ((*channelNvList)[ch].common.tRCD < SPDReg) {
            (*channelNvList)[ch].common.tRCD = SPDReg;

            // Convert to number of cycles
            cycles = TimeBaseToDCLKDdr4 (Host, socket, ch, dimm, (UINT16) SPDReg, ftbVal);

            if ((*channelNvList)[ch].common.nRCD < cycles) {
              (*channelNvList)[ch].common.nRCD = (UINT8) cycles;
            }
          }

          //
          // Get upper byte of tRFC1
          //
          SpdReadByte (socket, ch, dimm, SPD_MIN_TRFC1_LSB_DDR4, &SPDReg);
          tRFC = (UINT16) SPDReg;

          //
          // Get lower byte of tRFC1
          //
          SpdReadByte (socket, ch, dimm, SPD_MIN_TRFC1_MSB_DDR4, &SPDReg);
          tRFC = tRFC | ((UINT16) SPDReg << 8);

          if ((*channelNvList)[ch].common.tRFC < tRFC) {
            (*channelNvList)[ch].common.tRFC = tRFC;

            // Convert to number of cycles
            cycles = TimeBaseToDCLKDdr4 (Host, socket, ch, dimm, tRFC, 0);
            if ((*channelNvList)[ch].common.nRFC < cycles) {
              (*channelNvList)[ch].common.nRFC = cycles;
            }
          }

          //
          // Get tAA
          //
          SpdReadByte (socket, ch, dimm, SPD_MIN_TAA_DDR4, &SPDReg);
          SpdReadByte (socket, ch, dimm, SPD_FTB_TAA_DDR4, &ftbVal);
#ifdef  DEBUG_CODE_BLOCK
          (*chdimmList)[dimm].tCL = SPDReg;
#endif  // DEBUG_CODE_BLOCK
          (*channelNvList)[ch].dimmList[dimm].tCL = SPDReg;
          (*channelNvList)[ch].dimmList[dimm].ftbTAA = ftbVal;

          if ((*channelNvList)[ch].common.tCL < SPDReg) {
            (*channelNvList)[ch].common.tCL = SPDReg;

            // Convert to number of cycles
            cycles = TimeBaseToDCLKDdr4 (Host, socket, ch, dimm, (UINT16) SPDReg, (UINT16) ftbVal);

            if ((*channelNvList)[ch].common.nCL < cycles) {
              (*channelNvList)[ch].common.nCL = (UINT8) cycles;
            }
          }

          //
          // Get byte 4 of casSup
          //
          SpdReadByte (socket, ch, dimm, SPD_CAS_LT_SUP_4_DDR4, &SPDReg);
          SPDReg = SPDReg & 0xBF; //mask out reserved bit 6
          casSup = (UINT32) (SPDReg << 24);
          if (SPDReg & (1<<7)) {
            (*channelNvList)[ch].common.casSupRange = HIGH_CL_RANGE;
          }

          //
          // Get byte 3 of casSup
          //
          SpdReadByte (socket, ch, dimm, SPD_CAS_LT_SUP_3_DDR4, &SPDReg);
          casSup |= (UINT32) (SPDReg << 16);

          //
          // Get byte 2 of casSup
          //
          SpdReadByte (socket, ch, dimm, SPD_CAS_LT_SUP_2_DDR4, &SPDReg);
          casSup |= (UINT32) (SPDReg << 8);

          //
          // Get byte 1 of casSup
          //
          SpdReadByte (socket, ch, dimm, SPD_CAS_LT_SUP_1_DDR4, &SPDReg);
          casSup |= (UINT32) SPDReg;
          //
          // Mask off the unsupported CLs
          //
          (*channelNvList)[ch].common.casSup &= (UINT64) casSup;

          //
          // Get DIMM thermal sensor
          //
          SpdReadByte (socket, ch, dimm, SPD_DIMM_TS_DDR4, &reservedBits_SPDReg);
          (*dimmNvList)[dimm].dimmTs = reservedBits_SPDReg & 0x80; //mask out reserved bits 6-0

          //
          // Get SDRAM optional features
          //
          SpdReadByte (socket, ch, dimm, SPD_OPT_FEAT_DDR4, &SPDReg);
          (*dimmNvList)[dimm].SPDOptionalFeature = SPDReg & 0x3F; //mask out reserved bits 6-7

          //if (((*dimmNvList)[dimm].SPDOptionalFeature & SPD_TRR_IMMUNE) == 0){
          //  rankNvList = GetRankNvList(Host, socket, ch, dimm);
          //  for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          //    if ((*rankNvList)[rank].enabled == 0) continue;
          //    DisableRank(Host, socket, ch, dimm, rank);
          //    OutputWarning (WARN_MEMTEST_DIMM_DISABLE, WARN_FPT_MINOR_MEM_TEST, socket, ch, dimm, rank);
          //  }
          //}

          SpdReadByte (socket, ch, dimm, SPD_OTH_OP_FEAT_DDR4, &SPDReg);
          (*dimmNvList)[dimm].SPDOtherOptFeatures = SPDReg & 0xE0; //mask out reserved bits 4-0

          //Get VendorID, DeviceID, RID, SubSystemVendorID, SubSystemDeviceID, and SubSystem RID from SPD
          SpdReadByte (socket, ch, dimm, SPD_VENDORID_BYTE1, &SPDReg);
          (*dimmNvList)[dimm].VendorID = SPDReg;
          (*dimmNvList)[dimm].VendorID = (*dimmNvList)[dimm].VendorID << 8;
          SpdReadByte (socket, ch, dimm, SPD_VENDORID_BYTE0, &SPDReg);
          (*dimmNvList)[dimm].VendorID = (*dimmNvList)[dimm].VendorID | SPDReg;

          SpdReadByte (socket, ch, dimm, SPD_DEVICEID_BYTE1, &SPDReg);
          (*dimmNvList)[dimm].DeviceID = SPDReg;
          (*dimmNvList)[dimm].DeviceID = (*dimmNvList)[dimm].DeviceID << 8;
          SpdReadByte (socket, ch, dimm, SPD_DEVICEID_BYTE0, &SPDReg);
          (*dimmNvList)[dimm].DeviceID = (*dimmNvList)[dimm].DeviceID | SPDReg;

          SpdReadByte (socket, ch, dimm, SPD_RID, &SPDReg);
          (*dimmNvList)[dimm].RevisionID = SPDReg;

          SpdReadByte (socket, ch, dimm, SPD_SUBSYSTEM_VENDORID_BYTE1, &SPDReg);
          (*dimmNvList)[dimm].SubsystemVendorID = SPDReg;
          (*dimmNvList)[dimm].SubsystemVendorID = (*dimmNvList)[dimm].SubsystemVendorID << 8;
          SpdReadByte (socket, ch, dimm, SPD_SUBSYSTEM_VENDORID_BYTE0, &SPDReg);
          (*dimmNvList)[dimm].SubsystemVendorID = (*dimmNvList)[dimm].SubsystemVendorID | SPDReg;
          SpdReadByte (socket, ch, dimm, SPD_SUBSYSTEM_DEVICEID_BYTE1, &SPDReg);
          (*dimmNvList)[dimm].SubsystemDeviceID = SPDReg;
          (*dimmNvList)[dimm].SubsystemDeviceID = (*dimmNvList)[dimm].SubsystemDeviceID << 8;
          SpdReadByte (socket, ch, dimm, SPD_SUBSYSTEM_DEVICEID_BYTE0, &SPDReg);
          (*dimmNvList)[dimm].SubsystemDeviceID = (*dimmNvList)[dimm].SubsystemDeviceID | SPDReg;

          SpdReadByte (socket, ch, dimm, SPD_SUBSYSTEM_REVISIONID, &SPDReg);
          (*dimmNvList)[dimm].SubsystemRevisionID = SPDReg;

        } else { // NVMDIMM

          SpdReadByte (socket, ch, dimm, SPD_AEP_THERMAL_SENSOR, &SPDReg);
          (*dimmNvList)[dimm].dimmTs = SPDReg;

          if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_1600) {
            SpdReadByte (socket, ch, dimm, SPD_AEP_GNT2ERID_1333_1600, &SPDReg);
            (*dimmNvList)[dimm].DimmGnt2Erid = (SPDReg >> 4);
          } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_1866) {
            SpdReadByte (socket, ch, dimm, SPD_AEP_GNT2ERID_1866_2133, &SPDReg);
            (*dimmNvList)[dimm].DimmGnt2Erid = (SPDReg & 0x0F);
          } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2133) {
            SpdReadByte (socket, ch, dimm, SPD_AEP_GNT2ERID_1866_2133, &SPDReg);
            (*dimmNvList)[dimm].DimmGnt2Erid = (SPDReg >> 4);
          } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2400) {
            SpdReadByte (socket, ch, dimm, SPD_AEP_GNT2ERID_2400_2666, &SPDReg);
            (*dimmNvList)[dimm].DimmGnt2Erid = (SPDReg & 0x0F);
          } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2666) {
            SpdReadByte (socket, ch, dimm, SPD_AEP_GNT2ERID_2400_2666, &SPDReg);
            (*dimmNvList)[dimm].DimmGnt2Erid = (SPDReg >> 4);
          } else {
            SpdReadByte (socket, ch, dimm, SPD_AEP_GNT2ERID_3200, &SPDReg);
            (*dimmNvList)[dimm].DimmGnt2Erid = (SPDReg & 0x0F);
          }

          if (UbiosGenerationOrHsleEnabled ()) {
            //
            // Get PartitionRatios 0-7 from SPD 384-387
            //
            SpdReadByte (socket, ch, dimm, SPD_AEP_PARTITION_RATIO_SKT0, &SPDReg);
            Host->var.mem.socket[socket].channelList[ch].dimmList[dimm].SPDPartitionRatio[0] = SPDReg & 0x0F;
            Host->var.mem.socket[socket].channelList[ch].dimmList[dimm].SPDPartitionRatio[1] = (SPDReg & 0xF0) >> 4;
            SpdReadByte (socket, ch, dimm, SPD_AEP_PARTITION_RATIO_SKT1, &SPDReg);
            Host->var.mem.socket[socket].channelList[ch].dimmList[dimm].SPDPartitionRatio[2] = SPDReg & 0x0F;
            Host->var.mem.socket[socket].channelList[ch].dimmList[dimm].SPDPartitionRatio[3] = (SPDReg & 0xF0) >> 4;
            SpdReadByte (socket, ch, dimm, SPD_AEP_PARTITION_RATIO_SKT2, &SPDReg);
            Host->var.mem.socket[socket].channelList[ch].dimmList[dimm].SPDPartitionRatio[4] = SPDReg & 0x0F;
            Host->var.mem.socket[socket].channelList[ch].dimmList[dimm].SPDPartitionRatio[5] = (SPDReg & 0xF0) >> 4;
            SpdReadByte (socket, ch, dimm, SPD_AEP_PARTITION_RATIO_SKT3, &SPDReg);
            Host->var.mem.socket[socket].channelList[ch].dimmList[dimm].SPDPartitionRatio[6] = SPDReg & 0x0F;
            Host->var.mem.socket[socket].channelList[ch].dimmList[dimm].SPDPartitionRatio[7] = (SPDReg & 0xF0) >> 4;
          }
        } //NVDIMM

        //
        // Get module mfg id from SPD 320-321.
        //
        SpdReadByte (socket, ch, dimm, SPD_MMID_MSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDMMfgId = (UINT16) (SPDReg << 8);
        SpdReadByte (socket, ch, dimm, SPD_MMID_LSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDMMfgId |= (UINT16) (SPDReg &~BIT7);

        //
        // Get module module mfg location from SPD 322
        //
        SpdReadByte (socket, ch, dimm,  SPD_MM_LOC_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDMMfLoc = SPDReg; //1 byte Dimm Location

        //
        // Get module module date code from SPD 323-324.
        //
        SpdReadByte (socket, ch, dimm, SPD_MM_DATE_YR_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDModDate = (UINT16) (SPDReg);
        SpdReadByte (socket, ch, dimm, SPD_MM_DATE_WK_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDModDate |= (UINT16) (SPDReg << 8);

        //
        // Get module Serial Number from SPD 325-328
        //
        for (i = 0; i < SPD_MODULE_SERIAL; i++) {
          SpdReadByte (socket, ch, dimm, SPD_MODULE_SN_DDR4 + i, &SPDReg);
          (*dimmNvList)[dimm].SPDModSN[i] = SPDReg;
        }

        //
        // Get module part number from SPD 329-348.
        //
        for (i = 0; i < SPD_MODULE_PART_DDR4; i++) {
          SpdReadByte (socket, ch, dimm, SPD_MODULE_PN_DDR4 + i, &SPDReg);
          (*dimmNvList)[dimm].SPDModPartDDR4[i] = SPDReg;
        }

#ifdef DEBUG_CODE_BLOCK
        //
        // Get Intel DIMM serial number from SPD 507-511.
        //
        for (i = 0; i < 5; i++) {
          SpdReadByte (socket, ch, dimm, SPD_INTEL_ID_DDR4 + i, &SPDReg);
          (*dimmNvList)[dimm].SPDIntelSN[i] = SPDReg;
        }
#endif //DEBUG_CODE_BLOCK

        //
        // Get DRAM mfg id from SPD 350-351.
        //
        SpdReadByte (socket, ch, dimm, SPD_DRAM_MIDC_MSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDDramMfgId = (UINT16) (SPDReg << 8);
        SpdReadByte (socket, ch, dimm, SPD_DRAM_MIDC_LSB_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDDramMfgId |= (UINT16) (SPDReg &~BIT7);

        //
        // Get DRAM revision id from SPD 352.
        //
        SpdReadByte (socket, ch, dimm, SPD_DRAM_REV_DDR4, &SPDReg);
        (*dimmNvList)[dimm].SPDDramRev = SPDReg;


        //
        // Get SPD Spec revision
        //
        SpdReadByte (socket, ch, dimm, SPD_REVISION, &SPDReg);
        (*dimmNvList)[dimm].SPDSpecRev = SPDReg;

        //
        // RDIMM specific bytes
        //
        if (Host->nvram.mem.dimmTypePresent == RDIMM) {

          //
          // Register Output Drive Strength for Control
          //
          SpdReadByte (socket, ch, dimm, SPD_REG_OD_CTL_DDR4, &SPDReg);
          (*dimmNvList)[dimm].SPDODCtl = SPDReg;

          //
          // Register Output Drive Strength for Clock
          //
          SpdReadByte (socket, ch, dimm, SPD_REG_OD_CK_DDR4, &SPDReg);
          (*dimmNvList)[dimm].SPDODCk = SPDReg & 0x0F; //mask out reserved bits 7-4

          //
          // Get RDIMM/LRDIMM module attributes & check for mirrored rank addresses
          // Initialize dimmRevType to 1
          //
          (*dimmNvList)[dimm].DimmRevType = RCD_REV_2;

          if (!(*dimmNvList)[dimm].DcpmmPresent) {
            SpdReadByte (socket, ch, dimm, SPD_LRDIMM_ATTR_DDR4, &(*dimmNvList)[dimm].dimmAttrib);
            if ((*dimmNvList)[dimm].dimmAttrib & BIT4) {
              (*channelNvList)[ch].dimmRevType &= RCD_REV_2;
              (*dimmNvList)[dimm].DimmRevType &= RCD_REV_2;
            } else {
              (*channelNvList)[ch].dimmRevType &= RCD_REV_1;
              (*dimmNvList)[dimm].DimmRevType &= RCD_REV_1;
            }
            SpdReadByte (socket, ch, dimm, SPD_ADD_MAPPING_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDAddrMapp = SPDReg & 0x01; //mask out reserved bits 7-1
          } else {
            (*dimmNvList)[dimm].SPDAddrMapp = 0;
          }

          // RCD Revision
          (*dimmNvList)[dimm].lrbufGen = ((*dimmNvList)[dimm].dimmAttrib & 0xF0) >>4;

#ifdef  LRDIMM_SUPPORT
          //
          // LRDIMM specific bytes
          //
          if (IsLrdimmPresent (socket, ch, dimm)) {

            //
            // MSB of the register ID
            //
            SpdReadByte (socket, ch, dimm, SPD_LRBUF_VEN_MSB_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen = (UINT16) (SPDReg << 8);

            //
            // LSB of the register ID
            //
            SpdReadByte (socket, ch, dimm, SPD_LRBUF_VEN_LSB_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen |= (UINT16) (SPDReg &~BIT7);

            //
            // Register Revision
            //
            SpdReadByte (socket, ch, dimm, SPD_LRBUF_REV_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegRev = SPDReg;

            //
            // Data Buffer Revision
            //
            SpdReadByte (socket, ch, dimm, SPD_LRBUF_DB_REV_DDR4, &SPDReg);
            (*dimmNvList)[dimm].lrbufRid = SPDReg;

            //
            // Data Buffer Generation
            //
            SpdReadByte (socket, ch, dimm, SPD_LRDIMM_ATTR_DDR4, &(*dimmNvList)[dimm].dimmAttrib);
            (*dimmNvList)[dimm].lrbufGen = ((*dimmNvList)[dimm].dimmAttrib & 0xF0) >> 4;

            if ((*dimmNvList)[dimm].DcpmmPresent == 0) {

              //
              // Check if this LRDIMM is supported
              //
              if (!IsSiliconWorkaroundEnabled ("S5370385")) {
                if (((*dimmNvList)[dimm].SPDSpecRev < 8) && (Setup->mem.enforcePOR == ENFORCE_POR_EN) && ((*dimmNvList)[dimm].DcpmmPresent == 0)) {
                  RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "ERROR: You are using a non-production LRDIMM memory module that is not supported\n");
                  DisableChannelSw (Host, socket, ch);
                  OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_COMPAT_MINOR_NOT_SUPPORTED, socket, ch, dimm, 0);
                }
              }
            }
          } else
#endif
          // SPD bytes for RDIMM module
          {
            //
            // Get RDIMM module attributes
            //
            SpdReadByte (socket, ch, dimm, SPD_RDIMM_ATTR_DDR4, &(*dimmNvList)[dimm].dimmAttrib);

            //
            // MSB of the register ID
            //
            SpdReadByte (socket, ch, dimm, SPD_REG_VEN_MSB_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen = (UINT16) (SPDReg << 8);

            //
            // LSB of the register ID
            //
            SpdReadByte (socket, ch, dimm, SPD_REG_VEN_LSB_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegVen |= (UINT16) (SPDReg &~BIT7);

            //
            // Register Revision
            //
            SpdReadByte (socket, ch, dimm, SPD_REG_REV_DDR4, &SPDReg);
            (*dimmNvList)[dimm].SPDRegRev = SPDReg;
          }
        } else {
          //
          // UDIMM/SODIMM specific bytes
          //
          //
          // Address Mapping from Edge connector to DRAM
          //
          if ((*dimmNvList)[dimm].DcpmmPresent) {
            (*dimmNvList)[dimm].SPDAddrMapp = 0;
          } else {
            SpdReadByte (socket, ch, dimm, SPD_ADDR_MAP_FECTD_DDR4, &SPDReg);
            //
            // Mask out reserved bits 7-1
            //
            (*dimmNvList)[dimm].SPDAddrMapp = SPDReg & BIT0;
          }
        }

#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();

          //
          // Module part number
          //
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                         "DIMM Module Part Number: ");
          for (i = 0; i < SPD_MODULE_PART_DDR4; i++) {
            if ((*dimmNvList)[dimm].SPDModPartDDR4[i] >= 32 && (*dimmNvList)[dimm].SPDModPartDDR4[i] < 127) {
              RcDebugPrint (SDBG_DEFAULT, "%c", (*dimmNvList)[dimm].SPDModPartDDR4[i]);
            } else {
              RcDebugPrint (SDBG_DEFAULT, "(%d)", (*dimmNvList)[dimm].SPDModPartDDR4[i]);
            }
          }

          // New line
          RcDebugPrint (SDBG_DEFAULT, "\n");
          ReleasePrintControl ();
        }
#endif

        //SPD byte 128 check for VLP RDIMM
        if ((IsLrdimmPresent (socket, ch, dimm) == 0) && (Host->nvram.mem.dimmTypePresent == RDIMM)) {
          SpdReadByte (socket, ch, dimm, SPD_MODULE_NH_DDR4, &SPDReg);
          if ((SPDReg&0x1f) <= 0x4) {
            (*dimmNvList)[dimm].VlpRdimmPresent = 1;
          }
        }

#if defined(DEBUG_CODE_BLOCK) && defined(DDRT_SUPPORT)
        if ((*dimmNvList)[dimm].DcpmmPresent) {
          //prints for Buffer Gen, FM Controller, and Stepping to ease debugging
          RcDebugPrint (SDBG_MAX, "Revision Code (Stepping) = 0x%x\n", (*dimmNvList)[dimm].fmcRev);
          RcDebugPrint (SDBG_MAX, "FMC Type: ");
          switch ((*dimmNvList)[dimm].fmcType) {
          case FMC_FNV_TYPE:
            RcDebugPrint (SDBG_MAX, "FNV\n");
            break;
          case FMC_EKV_TYPE:
            RcDebugPrint (SDBG_MAX, "EKV\n");
            break;
          case FMC_BWV_TYPE:
            RcDebugPrint (SDBG_MAX, "BWV\n");
            break;
          default:
            RcDebugPrint (SDBG_MAX, "0x%x\n", (*dimmNvList)[dimm].fmcType);
          }
          RcDebugPrint (SDBG_MAX, "Gen Buffer: Gen %d\n", ((*dimmNvList)[dimm].lrbufGen) + 1);
          RcDebugPrint (SDBG_MAX, "Buffer Vendor: ");
            if (GetDebugLevel () & SDBG_MAX) {
            DisplayManf (Host, (*dimmNvList)[dimm].SPDRegVen);
          }
          RcDebugPrint (SDBG_MAX, "\n");
        }
#endif // DEBUG_CODE_BLOCK && DDRT_SUPPORT
      } // dimm loop
    } // ch loop
  }

  return SUCCESS;
} // GatherSPDDataDDR4
