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

#include "Ddr5Core.h"
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/Ddr5MrRegs.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Memory/I3cDefinitions.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/PerformanceTrackerLib.h>
#include <Library/SiliconWorkaroundPMemLib.h>
#include <Library/OdtTableLib.h>
#include "MemDramAccess.h"
#include <Library/MemoryCoreLib.h>

extern GSM_CSN SignalsToSweepBacksideDDR5[NUM_CA_SIGNALS_DDR5];
extern GSM_CSN CsToSweepBackSide[NUM_CS];
#ifdef USE_LATEST_DRAM_SPEC
#define MIN_VREFDQ_PERCENT_DDR5       350 // Min VREF DQ percentage multiplied by 10
#else
#define MIN_VREFDQ_PERCENT_DDR5       450 // Min VREF DQ percentage multiplied by 10
#endif // USE_LATEST_DRAM_SPEC
#define MAX_VREFDQ_PERCENT_DDR5       975 // Max VREF DQ percentage multiplied by 10
#define VREFDQ_INCREMENT_DDR5         5   // VrefDQ increment value (.5)  multiplied by 10

#define BCOM_MIN_VAL          0
#define BCOM_MAX_VAL          0x7F
#define QCA_OUTPUT_DELAY_EN   BIT7

#define QCA_MIN_VAL           0
#define QCA_MAX_VAL           0x7F
#define QCA_OUTPUT_DELAY_EN   BIT7

#define MIN_ODTLON_OFFSET       -4
#define MAX_ODTLON_OFFSET       2
#define MIN_ODTLOFF_OFFSET      -2
#define MAX_ODTLOFF_OFFSET      4

#define DDR5_DRAM_READ_PREAMBLE_1TCK      0
#define DDR5_DRAM_READ_PREAMBLE_2TCK_0010 1
#define DDR5_DRAM_READ_PREAMBLE_2TCK_1110 2
#define DDR5_DRAM_READ_PREAMBLE_3TCK      3
#define DDR5_DRAM_READ_PREAMBLE_4TCK      4

#define DDR5_DRAM_WRITE_PREAMBLE_2TCK     1
#define DDR5_DRAM_WRITE_PREAMBLE_3TCK     2
#define DDR5_DRAM_WRITE_PREAMBLE_4TCK     3

#define DDR5_DRAM_READ_POSTAMBLE_0P5TCK   0
#define DDR5_DRAM_READ_POSTAMBLE_1P5TCK   1

#define DDR5_DRAM_WRITE_POSTAMBLE_0P5TCK  0
#define DDR5_DRAM_WRITE_POSTAMBLE_1P5TCK  1

#define IBT_TBD_OHMS        0xFFFF  // placeholder for an impedance value
#define IBT_DEFAULT_ENCODED 0       // the default encoded value
#define MAX_RCD_DCS_VREF_RANGE            105  // 0x69
#define MAX_RCD_CA_DFE_VREF_RANGE         200  // 0xC8
#define MAX_RCD_CA_VREF_RANGE             105  // 0x69
#define MAX_QCA_RANGE                     127  // defined in the RW1B/RW1C
#define MAX_QCS_RANGE                     127  // defined in the RW17/RW18/RW19/RW1A
#define MAX_BCOM_RANGE                    127  // defined in the RCD RW1D
#define MAX_DB_DFE_VREF_RANGE             200  // 0xC8 defined in the PG6RWE2
#define MAX_BCOM_VREF_RANGE               125  // defined in DB PG[2]RWFA
#define MAX_DRAM_CS_CA_VREF_RANGE         125  // defined in the MR11/MR12

#if FixedPcdGetBool(PcdMcrSupport)
/**

Get the operating speed value for the RW05 in MCR.

@param[in] Socket          - Socket number

@retval RCW speed value

**/
UINT8
GetSpeedRcwValueMcr (
  IN UINT8    Socket
  );

/**
InitDramInterface

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (0-based)
  @param[in] Dimm       - DIMM number (0-based)
  @param[in] SubChannelMask - Sub channel mask
  @param[in] Rank       - Rank number (0-based)

@retval N/A

**/
VOID
InitDramInterfaceMcr (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  );

/**

Init Db and Dram Interface for MCR

@param[in] Socket          - Socket Id
@param[in] Ch              - Channel number (0-based)
@param[in] Dimm            - DIMM number (0-based)
@param[in] SubChannelMas   - Sub Channel Mask
@param[in] Rank            - Rank number (0-based)

@retval N/A

**/
VOID
InitDbDramInterfaceMcr (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  );

/**
  Enter RCD CA pass through broadcast mode.

  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/
VOID
EFIAPI
EnterRcdCaBcastPassThroughMode (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**
  Exit RCD CA pass through mode.

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/
VOID
ExitRcdCaPassThroughModeDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );
#endif // FixedPcdGetBool(PcdMcrSupport)

/**

  Get the DRAM preamble and post amble MR8 value.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number.

  @retval Preamble value

**/

UINT8
GetPreamblePostambleMr8Value (
  PSYSHOST  Host,
  UINT8     Socket
  );

/**

  Returns the RTT ODT to be programmed to the DRAM.

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number
  @param[in] Dimm         - DIMM number
  @param[in] Rank         - Rank number
  @param[out] RttWriteVal - Pointer to store RTT Write value
  @param[out] RttParkVal  - Pointer to store RTT Park value
  @param[out] RttNomRdVal - Pointer to store RTT Nom Read value
  @param[out] RttNomWrVal - Pointer to store RTT Nom Write value
  @param[out] DqsRttParkVal - Pointer to store Dqs RTT Park value

  @retval N/A

**/
VOID
GetRttValues (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  OUT UINT8     *RttWriteVal,
  OUT UINT8     *RttParkVal,
  OUT UINT16    *RttNomRdVal,
  OUT UINT16    *RttNomWrVal,
  OUT UINT16    *DqsRttParkVal
  );

// -------------------------------------
// Code section
// -------------------------------------

/**

  Return the time required for the parity recovery for DDR5 in DCLKs

  @param[in]    Socket        Current socket under test (0-based)
  @param[in]    RatioIndex    Current ratio index (frequency)

  @return       return the parity recovery in DCLKs

**/
UINT32
EFIAPI
GetParityRecoveryDdr5 (
  IN        UINT8           Socket,
  IN        UINT8           RatioIndex
)
{
  UINT32  tCk;              // in 0.1ps units
  UINT32  tPW = 1200000;    // in 0.1ps units
  UINT32  tParityPulseWidthInDclks;
  //
  //  tPW is always 120ns (or 1,200,000 in 0.1 ps)
  //  tParityPulseWidthInDclks = ROUNDDOWN (tPW / tCK)
  //
  tCk = GettCK (GetSysHostPointer (), Socket);
  tParityPulseWidthInDclks = tPW / tCk;

  return tParityPulseWidthInDclks;
}
/**

Check if RCD CW is write only.

@param  Address - RCD CW Address.

@retval TRUE    - This RCD CW is write only.
@retval FALSE   - This RCD CW is not write only.

**/
BOOLEAN
IsWriteOnlyControlWord (
  UINT16 Address
  )
{
  UINT16 WriteOnlyCWs [] = {RDIMM_RW04};
  UINT16 Index;

  for (Index = 0; Index < ARRAY_SIZE (WriteOnlyCWs); Index ++) {
    if (Address == WriteOnlyCWs [Index]) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Determine if MRW should be sent over SMBUS for SendMrw function

  @param Host     - Pointer to SysHost
  @param Socket   - Socket Id
  @param Ch       - Channel number (0-based)
  @param Dimm     - DIMM number (0-based)
  @param Address  - Address for the MRS write
  @param Cw       - CW flag

  @retval TRUE  - Use SMBUS to send MRW
  @retval FALSE - Do not use SMBUS for MRW

**/
BOOLEAN
UseSmbusMrwForSendMrw (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT16    Address,
  IN  UINT8     Cw
  )
{
  BOOLEAN                    EnableSmbusMrw;
  DDR5_LRDIMM_TRAINING_DATA  *TrainingData;
  MRC_STATUS                 Status;

  //
  // Determine if MRW should be sent inband or over SMBUS
  //
  EnableSmbusMrw = UseSmbusMrw (Host, Socket);

  if (Cw == CW_DRAM && FeaturePcdGet (PcdCteBuild) == TRUE) {
    EnableSmbusMrw = FALSE; // for CTE, force inband if not CW
  }

  if (IsDcpmmPresentDimm (Socket, Ch, Dimm)) {
    if (IsPmemSiliconWorkaroundEnabled (Host, Socket, Ch, Dimm, "S1508018006") && (Host->nvram.mem.socket[Socket].ddrFreq > DDR_3600)) {
      // As a part of HSD2208175879 fix - BGF is put into reset. Due to this the logic of writing MRW to CWV CSR will be disabled.
      // Therefore, BIOS cannot send MRW to CWV CSR (i.e. when CW = 0)
      EnableSmbusMrw = TRUE;
    }

    // CWV WA for MRW MR0 - BIOS shall use sideband access to MR0. MR0 does not have direct MRA address.
    if ((Address == DDR5_MODE_REGISTER_0_REG) && (Cw == CW_DRAM)) {
      EnableSmbusMrw = TRUE;
    }
  }

  // If EnableSmbusMrw is TRUE, check to see if the target is a data buffer address (CW set to 1, Bit 7 of address
  // is 1).  If so, check if BCOM training is done.  When BCOM training is complete, and the taget is the data buffer,
  // do not use SMBUS.
  if ((EnableSmbusMrw == TRUE) && (Cw == CW_RCD_DB) && (Address & BIT7)) {
    Status = GetDdr5LrdimmTrainingData (&TrainingData);
    if (Status == MRC_STATUS_SUCCESS) {
      if (TrainingData->BcomTrainingDone == TRUE) {
        EnableSmbusMrw = FALSE;
      }
    }
  }

  return EnableSmbusMrw;
}

/**

Send MRW Command.

@param Host     - Pointer to sysHost
@param Socket   - Socket Id
@param Ch       - Channel number (0-based)
@param Dimm     - DIMM number (0-based)
@param Rank     - Rank number (0-based)
@param Address  - Address for the MRS write
@param Cw       - CW flag
@param Data     - Data to write

@retval N/A

**/
UINT32
SendMrwDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT16   Address,
  IN UINT8    Cw,
  IN UINT8    Data
  )
{
  UINT32                     Data32;
  BOOLEAN                    EnableSmbusMrw;
  UINT8                      SubCh;
  UINT8                      SmbData8;
  struct dimmNvram           (*DimmNvList)[MAX_DIMM];
  UINT8                      Page;

  SmbData8 = 0;

  RcDebugPrintWithDevice (SDBG_RCWRITETAG, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "MRW command with Address 0x%04X, CW 0x%04X, Data0x%04X,  SubChMask %d \n", Address, Cw, Data, SubChMask);

  CountTrackingData (PTC_MRS_WRITE, 1);//DDR5_TODO.
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RemapMraAddress(Host, Socket, Ch, Dimm, &Address, &Cw);

  if (Host->var.mem.socket[Socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  //
  // Determine if MRW should be sent inband or over SMBUS
  //
  EnableSmbusMrw = UseSmbusMrwForSendMrw (Host, Socket, Ch, Dimm, Address, Cw);

  if (EnableSmbusMrw == FALSE) {
    WriteCadbMrCmd (Host, Socket, Ch, Dimm, SubChMask, Rank, Address, Data, Cw, MRW_CMD);
  } else {
    if (Cw == CW_RCD_DB || ((*DimmNvList)[Dimm].DcpmmPresent == 1)) {
      Data32 = Data;
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        //
        // Because page number is not passed as an input parameter, need to find the cached page number.
        // Caller shall set up the page number before call this function.
        //
        Page = RDIMM_CW_PAGE0;
        if (Address >= MAX_RCD_DIRECT_CONTROL_WORDS) {
          ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_CW_PAGE_REG, &Page);
        }
        WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SubChMask, BYTE_WRITE, Page, Address & 0xFF, &Data32);
      } else {
        WriteCwvSideband (Host, Socket, Ch, Dimm, Address, Data);
      }
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch,  Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Warning: SendMrw target can not be DRAM when the DRAM CA is not trained\n");
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_115);
    }
  }

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & SubChMask) == 0) {
      continue;
    }
    //
    // Handle RCD MRW
    //
    if ((Cw == CW_RCD_DB) && ((Address & BIT7) == 0)) {
      //
      // Get Page select
      //

      //
      // This isn't useful in RcSim
      //
      if (IsBrsPresent (Host, Socket)) {
        //
        // Debug code
        //
        ReadRcdSmbByteDdr5 (Host, Socket, Ch, Dimm, SubCh, RDIMM_CW_PAGE0, Address & 0xFF, &SmbData8);
        if ((Data != SmbData8) && (!IsWriteOnlyControlWord(Address))) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, 0, NO_STROBE, NO_BIT,
            "WARNING! SendMrw failed. Expected value 0x%02x, ReadSmb value 0x%02x\n", Data, SmbData8);
        }
      } // BrsPresent
    }
  }

  //
  // tMRD: Mode Register Set command delay
  //
  FixedDelayMicroSecond (1);

  return SUCCESS;
} // SendMrwDdr5

/**

Send MRW Command.

@param Host     - Pointer to sysHost
@param Socket   - Socket Id
@param Ch       - Channel number (0-based)
@param Dimm     - DIMM number (0-based)
@param Rank     - Rank number (0-based)
@param Address  - Address for the MRS write
@param Cw       - CW flag
@param Data     - Data to write

@retval N/A

**/
UINT32
SendMrw (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT16   Address,
  IN UINT8    Cw,
  IN UINT8    Data
  )
{
#if FixedPcdGetBool(PcdMcrSupport)
  return SendMrwMcr (Socket, Ch, SubChMask, PS_CH_UNUSED, Dimm, Rank, Address, Cw, Data);
#else
  return SendMrwDdr5 (Host, Socket, Ch, SubChMask, Dimm, Rank, Address, Cw, Data);
#endif // FixedPcdGetBool(PcdMcrSupport)
}

/**

Send MRW Command by PBA.

@param Host      - Pointer to sysHost
@param Socket    - Socket Id
@param Ch        - Channel number (0-based)
@param SubChMask - Sub Channel Mask
@param Dimm      - DIMM number (0-based)
@param Rank      - Rank number (0-based)
@param Buffer    - Data Buffer device number (0-based)
@param Address   - Address for the MRW write
@param Data      - Data to write

@retval NA

**/
VOID
EFIAPI
SendMrwPba (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Buffer,
  IN UINT8    Address,
  IN UINT8    Data
  )
{

  UINT32      PrevDebugLevel;
  UINT8       MaxChDdr;
  UINT8       MaxRank;
  UINT8       SubChMSVx8;

  MaxChDdr    = GetMaxChDdr ();
  MaxRank     = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
  SubChMSVx8  = GetSubChMaxStrobeValid (Host) / 2;

  RcDebugPrintWithDevice (SDBG_RCWRITETAG, Socket, Ch, Dimm, NO_SUBCH, Rank, Buffer, NO_BIT,
    "MRW command with Address 0x%04X, Data0x%04X,  SubChMask %d  Buffer %d by PBA\n", Address, Data, SubChMask, Buffer);

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM)   ||
      (Rank   >= MaxRank)    ||
      (Buffer >= SubChMSVx8))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, Buffer, NO_BIT,
      "WARNING - SendMrwPba called with an invalid parameter\n");
    return;
  }

  //
  // Save the debug message level and enable error messages only
  //
  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }

  SendMrw (Host, Socket, Ch, SubChMask, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, CW_RCD_DB, Buffer);
  SendMrw (Host, Socket, Ch, SubChMask, Dimm, Rank, Address, CW_RCD_DB, Data);
  SendMrw (Host, Socket, Ch, SubChMask, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, CW_RCD_DB, ALL_DATABUFFERS);

  //
  // Restore the debug message level.
  //
  SetDebugLevel (PrevDebugLevel);
  return;

}

/**

  Send NOP Command.

  @param Host     - Pointer to sysHost
  @param Socket   - Socket Id
  @param Ch       - Channel number (0-based)
  @param Dimm     - DIMM number (0-based)
  @param Rank     - Rank number (0-based)
  @param Address  - Address for the MRS write
  @param Cw       - CW flag
  @param Data     - Data to write

  @retval N/A

**/
UINT32
SendNop (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Cw,
  IN BOOLEAN  PassThroughMode,
  IN BOOLEAN  ExitCATM
  )
{

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "NOP command: CW 0x%04X, PassThroughMode: %x, ExitCATM: %x, SubChMask: %d\n", Cw, PassThroughMode, ExitCATM, SubChMask);

  CountTrackingData (PTC_MRS_WRITE, 1); //DDR5_TODO.

  if (Host->var.mem.socket[Socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  WriteCadbNopCmd (Host, Socket, Ch, Dimm, SubChMask, Rank, Cw, NOP_CMD, PassThroughMode, ExitCATM);
  FixedDelayMicroSecond (1);
  return SUCCESS;
} // SendNop

/**

  Determine if the given DQ bit number is valid for DDR5

  @param[in]  Host    Pointer to sysHost
  @param[in]  Bit     DQ bit number to check

  @retVal   TRUE      DQ bit number is not valid
  @retVal   FALSE     DQ bit number is valid

**/
BOOLEAN
IsBitNotValidSubChDdr5 (
  IN      PSYSHOST        Host,
  IN      UINT8           Bit
  )
{
  UINT8 MaxBit;

  if (!Host->nvram.mem.eccEn) {
    MaxBit = (GetSubChMaxStrobeValid (Host) - 2) * BITS_PER_NIBBLE;
  } else {
    if (IsBrsPresent (Host, Host->var.mem.currentSocket)) {
      MaxBit = 40;
    } else {
      MaxBit = (UINT8)GetMaxBitsValid (Host) / 2;
    }
  }
  if (Bit >= MaxBit) {
    return TRUE;
  }
  return FALSE;
} // IsBitNotValidSubChDdr5

/**

Send MRW Command by PDA.

@param Host      - Pointer to sysHost
@param Socket    - Socket Id
@param Ch        - Channel number (0-based)
@param SubChMask - Sub Channel Mask
@param Dimm      - DIMM number (0-based)
@param Rank      - Rank number (0-based)
@param Dram      - Dram device number (0-based)
@param Address   - Address for the MRW write
@param Data      - Data to write

@retval NA

**/
VOID
EFIAPI
SendMrwPda (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT8    Address,
  IN UINT8    Data
  )
{

  UINT32      PrevDebugLevel;
  UINT8       MaxChDdr;
  UINT8       MaxRank;
  UINT8       SubChMSVx4;

  MaxChDdr    = GetMaxChDdr ();
  MaxRank     = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
  SubChMSVx4  = GetSubChMaxStrobeValid (Host);

  RcDebugPrintWithDevice (SDBG_RCWRITETAG, Socket, Ch, Dimm, NO_SUBCH, Rank, Dram, NO_BIT,
    "MRW command with Address 0x%04X, Data0x%04X,  SubChMask %d  Dram %d by PDA\n", Address, Data, SubChMask, Dram);

  if ((Socket >= MAX_SOCKET) ||
      (Ch     >= MaxChDdr)   ||
      (Dimm   >= MAX_DIMM)   ||
      (Dram   >= SubChMSVx4) ||
      (Rank   >= MaxRank))
  {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Ch, Dimm, NO_SUBCH, Rank, Dram, NO_BIT,
      "WARNING - SendMrwPda called with an invalid parameter\n");
    return;
  }

  //
  // Save the debug message level and enable error messages only
  //
  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, PDA_SELECT_ID | Dram, TIMING_1N, TRUE);
  SendMrw (Host, Socket, Ch, SubChMask, Dimm, Rank, Address, CW_DRAM, Data);
  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, PDA_SELECT_ALL, TIMING_1N, TRUE);

  //
  // Restore the debug message level.
  //
  SetDebugLevel (PrevDebugLevel);
  return;

} // WriteMrwPda

/**
  Determine if MRR should be sent over SMBUS

  @param[in]  Socket  - Socket number

  @retval TRUE  - Use SMBUS to send MRR
  @retval FALSE - Do not use SMBUS for MRR

**/
BOOLEAN
UseSmbusMrr (
  IN  UINT8       Socket
  )
{
  BOOLEAN     SmbMrrEnable;

  SmbMrrEnable = TRUE;

  if (IsCoarseRdDqDqsTrainingDone (Socket)) {
    SmbMrrEnable = FALSE;
  }

  return SmbMrrEnable;
}

/**

  Read data buffer mode register

  @param[in]  Socket          - Socket Id
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  SubChMask       - Sub-channel bit mask
  @param[in]  Dimm            - DIMM number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Address         - Mode register Address
  @param[out] Data            - Data array to store the returned result

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

  **/
MRC_STATUS
ReadDbMr (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        SubChMask,
  IN     UINT8        Dimm,
  IN     UINT8        Rank,
  IN     UINT8        Address,
    OUT  UINT8        (*Data)[SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  PSYSHOST   Host;
  MRC_STATUS Status = MRC_STATUS_FAILURE;

  Host = GetSysHostPointer();

  RcDebugPrintWithDevice (SDBG_RCWRITETAG, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "Read Db Mrr command with Address 0x%04X\n", Address);

  if (Host->var.mem.socket[Socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  if (!UseSmbusMrr (Socket)) {

    WriteCadbMrCmd (Host, Socket, Ch, Dimm, SubChMask, Rank, Address, 0, CW_RCD_DB, MRR_CMD);

    Status = ReadCpgcMrrData (Socket, Ch, SubChMask, Dimm, Data);
  }

  return Status;
}

/**

  Read dram mode register

  @param[in]  Socket          - Socket Id
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  SubChMask       - Sub-channel bit mask
  @param[in]  Dimm            - DIMM number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Address         - Mode register Address
  @param[out] Data            - Data array to store the returned result for all dram devices

  @retval MRC_STATUS_SUCCESS if read is successful
  @retval MRC_STATUS_FAILURE otherwise

  **/
MRC_STATUS
ReadDramMr (
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        SubChMask,
  IN     UINT8        Dimm,
  IN     UINT8        Rank,
  IN     UINT8        Address,
    OUT  UINT8        (*Data)[SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  PSYSHOST   Host;
  MRC_STATUS Status = MRC_STATUS_FAILURE;

  Host = GetSysHostPointer();

  RcDebugPrintWithDevice (SDBG_RCWRITETAG, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "Read Db Mrr command with Address 0x%04X\n", Address);

  if (Host->var.mem.socket[Socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  if (!UseSmbusMrr (Socket)) {

    WriteCadbMrCmd (Host, Socket, Ch, Dimm, SubChMask, Rank, Address, 0, CW_DRAM, MRR_CMD);

    Status = ReadCpgcMrrData (Socket, Ch, SubChMask, Dimm, Data);
  }

  return Status;
}

/**
Save the timing mode value.

@param[in] Host       - Pointer to sysHost
@param[in] Socket     - Socket Id
@param[in] Ch         - Channel number (0-based)
@param[in] Dimm       - DIMM number (0-based)
@param[in] Rank       - Rank number (0-based)
@param[in] Opcode     - Opcode for MPC command

@retval N/A

**/
VOID
SaveTimingModeValue (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT16   Opcode
  )
{
#ifdef USE_LATEST_DRAM_SPEC
  DDR5_MODE_REGISTER_2_STRUCT Mr2;
#else
  DDR5_MODE_REGISTER_3_STRUCT Mr3;
#endif //USE_LATEST_DRAM_SPEC

  if (Opcode == SET_1N_CMD_TIMING) {
#ifdef USE_LATEST_DRAM_SPEC
    ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, &Mr2.Data);
    Mr2.Bits.two_n_mode = 1;
    WriteDramModeRegCachePerRank (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, Mr2.Data);
#else
    ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_3_REG, &Mr3.Data);
    Mr3.Bits.two_n_mode = 1;
    WriteDramModeRegCachePerRank (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_3_REG, Mr3.Data);
#endif
  }
  return;
} // SaveTimingModeValue

/**
Writes Multi-Purpose Command

@param Host       - Pointer to sysHost
@param Socket     - Socket Id
@param Ch         - Channel number (0-based)
@param Dimm       - DIMM number (0-based)
@param Rank       - Rank number (0-based)
@param Opcode     - Opcode for MPC command
@param Timingmode - support MPC_1N, MPC_2N, MPC_4N.

@retval N/A

**/
VOID
WriteMPC (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  )
{
  DDR5_RCD_GLOBAL_FEATURES_STRUCT    OrgGlobalFeatures;
  DDR5_RCD_GLOBAL_FEATURES_STRUCT    GlobalFeatures;
  struct dimmNvram                   (*DimmNvList) [MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "MPC command with Opcode 0x%04X SubChannelMask 0x%x\n", Opcode, SubChannelMask);

  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    //
    // If not in CA Pass Thru mode then switch it
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures.Data);
    OrgGlobalFeatures.Data = GlobalFeatures.Data;
    if(OrgGlobalFeatures.Bits.ca_pass_through_mode_enable == 0){
      PassThroughMode = 1;
      GlobalFeatures.Bits.ca_pass_through_mode_enable = 1;
      GlobalFeatures.Bits.ca_pass_through_mode_rank_selection = Rank;
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures.Data);
    }

    if (Opcode == SET_1N_CMD_TIMING) {
      //
      // Save the timing mode value.
      //
      SaveTimingModeValue (Host, Socket, Ch, Dimm, Rank, Opcode);
    }
    WriteMpcDdr5 (Host, Socket, Ch, Dimm, SubChannelMask, Rank, Opcode, Timingmode, PassThroughMode);

    //
    // Restore GlobalFeatures if switched
    // Note: Rank needs to xor 1 since RW00[3] decide what Rank RCW will process MRW command When Pass Thru Mode is enabled
    //
    if(OrgGlobalFeatures.Bits.ca_pass_through_mode_enable == 0){
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank ^ 1, DDR5_RCD_GLOBAL_FEATURES_REG, OrgGlobalFeatures.Data);
    }
  } else { // UDIMM
    if (Opcode == SET_1N_CMD_TIMING) {
      //
      // Save the timing mode value.
      //
      SaveTimingModeValue (Host, Socket, Ch, Dimm, Rank, Opcode);
    }
    WriteMpcDdr5 (Host, Socket, Ch, Dimm, SubChannelMask, Rank, Opcode, Timingmode, PassThroughMode);
  }

  return;
} // WriteMPC

/**
  JEDEC DRAM Reset Initialization steps 6 and 7 (Te thorugh Tj)

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (0-based)
  @param[in] Dimm       - DIMM number (0-based)
  @param[in] Rank       - Rank number (0-based)
  @param[in] Opcode     - Opcode for MPC command
  @param[in] Timingmode - support MPC_1N, MPC_2N, MPC_4N.

@retval N/A

**/
VOID
DramResetInitialization (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  )
{
  UINT8                                               ControlWord = 0;
  struct dimmNvram                                    (*DimmNvList)[MAX_DIMM];

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "InitDramInterface starts\n");
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // UDIMM: This routines can be called starting at JEDEC DRAM reset seqeunce time point Te, or the end of step 5
  // RDIMM: This routine can be called starting at JEDEC RCD reset sequence time point Tk
  //
  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    //
    // For RCD applications, QRST_n has already been released as part of RCD initialization (RcdReleaseQrst)
    // RCD drives QCS_n low until it is instructed to release the signal with a NOP command
    // tINIT3 is the amount of time required from QRST_n being released until QCS_n is released
    // The delay below makes no assumptions about how much time has passed since QRST_n was released
    //
    // tINIT3 = 4ms
    //
    FixedDelayMicroSecond (4000);
    //
    // NOP CMD to RCD to release QCS - RCD initialization time point Tk to Tl
    //
    SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 0, 1, FALSE, FALSE);
    SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 1, 1, FALSE, FALSE);
  } else {
    //
    // tINIT4 = 2us
    //
    FixedDelayMicroSecond (2);
    //
    // Clear CS_n override - allow CS_n to be driven with commands from the host
    // This step was already performed for RCD initialzation but still needed for UDIMM
    //
    OverrideCke (Socket, Ch, 0, 0);
  }

  //
  // For RDIMM, assuming RWs have been initialized/resetored and RW01 is configured to forward all commands to DRAM
  //
  // Step 6.
  //    Upon Te, NOP commands must be issued for a minimum of tINIT5 (3nCK) to conclude exit of initialization
  //    process and start tXPR timer at (Tf). The system must wait at least tXPR before issuing any legal configuration
  //    commands (Tg). During configuration, only MRR, MRW, MPC and VREFCA commands are legal.
  //
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, Rank, 0, TRUE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, Rank, 0, TRUE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, Rank, 0, TRUE, FALSE);
  //
  // tXPR (= tXS = tRFC1 < 1 us)
  //
  FixedDelayMicroSecond (1);

  //
  // Step 7.
  //    Between (Tg to Tj), the following initial configuration modes must be completed prior to other training modes:
  //    MPC for setting MR13 (tCCD/tDLLK) must be issued before the MPC command to reset the DLL.
  //    MPC to execute DLL RESET must be issued before ZQCal Start
  //    MPC to execute ZQCal Start and ZQCal Latch must be issued before any other training modes such as CS Training
  //
  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    //
    // Enter CA passthrough mode for this rank
    //
    EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);
    //
    //tMRD_L2 (32tCK)
    //
    FixedDelayMicroSecond (1);
  } // RDIMM
  if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3200) {
    ControlWord = DataRate_2001_3200;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3600) {
    ControlWord = DataRate_3201_3600;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4000) {
    ControlWord = DataRate_3601_4000;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4400) {
    ControlWord = DataRate_4001_4400;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4800) {
    ControlWord = DataRate_4401_4800;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_5200) {
    ControlWord = DataRate_4801_5200;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_5600) {
    ControlWord = DataRate_5201_5600;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_6000) {
    ControlWord = DataRate_5601_6000;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_6400) {
    ControlWord = DataRate_6001_6400;
  }
  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, Configure_tDLLK_tCCD_L | ControlWord, TIMING_4N, TRUE);
  //
  // tMRD (< 1 us)
  //
  FixedDelayMicroSecond (1);
  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, DLL_RESET, TIMING_4N, TRUE);
  //
  // tMRD (< 1 us)
  //
  FixedDelayMicroSecond (1);
  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ZQCAL_START, TIMING_4N, TRUE);
  //
  // tZQCAL (= 1 us)
  //
  FixedDelayMicroSecond (2);
  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ZQCAL_LATCH, TIMING_4N, TRUE);
  //
  // tZQLAT (< 1 us)
  //
  FixedDelayMicroSecond (1);

  if (Host->nvram.mem.dimmTypePresent == RDIMM) {

    ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);
    //
    //tMRD_L2 (32tCK)
    //
    FixedDelayMicroSecond (1);
  } // RDIMM

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "DramResetInitialization ends\n");

  return;
} // DramResetInitialization


/**
  InitDramInterface

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (0-based)
  @param[in] Dimm       - DIMM number (0-based)
  @param[in] Rank       - Rank number (0-based)
  @param[in] Opcode     - Opcode for MPC command
  @param[in] Timingmode - support MPC_1N, MPC_2N, MPC_4N.

@retval N/A

**/
VOID
InitDramInterfaceDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  )
{
  UINT8                                               ControlWord = 0;
  struct dimmNvram                                    (*DimmNvList)[MAX_DIMM];
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT         ParityAndAlert;
  DDR5_RCD_COMMAND_LATENCY_ADDER_CONFIGURATION_STRUCT CommandLatencyAdderConfig;
  DDR5_DATA_BUFFER_RW93_STRUCT                        DataBufferRw93;
  DDR5_DATA_BUFFER_RW82_STRUCT                        DataBufferRw82;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "InitDramInterface starts\n");
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Enable DRAM interface to forward all commands.
  //
  ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
#else
  ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
#endif
  RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);
  //
  //add some delay
  //
  FixedDelayMicroSecond (1);
  //
  // Send NOP Command to Rank 0 and Rank 1 for initilization. Assume two ranks during the init flow.
  // After init done, will based on the actual Rank number to do the training.
  //
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 0, 1, FALSE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 1, 1, FALSE, FALSE);
  FixedDelayMicroSecond (1); // delay at least tXPR (< 1 us)

  //
  // Issue MR13 to initial tCCD_L / tDLLK before DLL_RESET MPC command is sent
  //
  if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3200) {
    ControlWord = DataRate_2001_3200;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3600) {
    ControlWord = DataRate_3201_3600;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4000) {
    ControlWord = DataRate_3601_4000;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4400) {
    ControlWord = DataRate_4001_4400;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4800) {
    ControlWord = DataRate_4401_4800;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_5200) {
    ControlWord = DataRate_4801_5200;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_5600) {
    ControlWord = DataRate_5201_5600;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_6000) {
    ControlWord = DataRate_5601_6000;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_6400) {
    ControlWord = DataRate_6001_6400;
  }
  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, Configure_tDLLK_tCCD_L | ControlWord, TIMING_4N, TRUE);
  FixedDelayMicroSecond (1);
  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, DLL_RESET, TIMING_4N, TRUE);
  FixedDelayMicroSecond (1); // delay at least tMRD (< 1 us)
  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ZQCAL_START, TIMING_4N, TRUE);
  FixedDelayMicroSecond (2); // delay at least tZQCAL (== 1 us), FixedDelayMicroSecond is roughly 1 microsec
  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ZQCAL_LATCH, TIMING_4N, TRUE);
  FixedDelayMicroSecond (1); // delay at least tZQLAT (< 1 us)

  //
  // Set command latency to 1tck.
  //
  CommandLatencyAdderConfig.Data = 0;
  CommandLatencyAdderConfig.Bits.latency_adder_nladd_to_all_dram_commands = 1;
  RcdControlWordWrite (Socket, Ch, SubChannelMask, Dimm, Rank, DDR5_RCD_COMMAND_LATENCY_ADDER_CONFIGURATION_REG, CommandLatencyAdderConfig.Data);

  EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

  //
  // Three NOPs to take DRAM out of SR. Send NOP Command to Rank 0 and Rank 1 for initilization.
  // Assume two ranks during the init flow. After init done, will based on the actual Rank number to do the training.
  //
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 0, 0, TRUE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 0, 0, TRUE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 0, 0, TRUE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 1, 0, TRUE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 1, 0, TRUE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 1, 0, TRUE, FALSE);

  ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

  if (IsLrdimmPresent (Socket, Ch, Dimm) == 1) {
    //
    // Block commands to DRAM, forward commands to DB
    //
    ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);
    //
    // Select all data buffers
    //
    DataBufferRw93.Data = 0;
    DataBufferRw93.Bits.pba_select_id = PBA_SELECT_ALL;
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, DataBufferRw93.Data);
    //
    // Configure DB in DQ pass-through mode for reads
    //
    DataBufferRw82.Data = 0;
    DataBufferRw82.Bits.dq_pass_through_mode = 1;
    DataBufferRw82.Bits.dq_pass_through_mode_direction_select = 1;
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW82_REG, DataBufferRw82.Data);

    //
    // Block commands to DB, forward commands to DRAM
    //
    ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);
  }

  FixedDelayMicroSecond (10);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "InitDramInterface ends\n");

  return;
} // InitDramInterfaceDdr5

/**
InitDramInterface

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (0-based)
  @param[in] Dimm       - DIMM number (0-based)
  @param[in] Rank       - Rank number (0-based)
  @param[in] Opcode     - Opcode for MPC command
  @param[in] Timingmode - support MPC_1N, MPC_2N, MPC_4N.

@retval N/A

**/
VOID
InitDramInterface (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  )
{
#if FixedPcdGetBool(PcdMcrSupport)
  InitDramInterfaceMcr (Socket, Ch, Dimm, SubChannelMask, Rank);
#else
  InitDramInterfaceDdr5 (Host, Socket, Ch, Dimm, SubChannelMask, Rank);
#endif // FixedPcdGetBool(PcdMcrSupport)
}

/**
InitDbInterface

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket Id
  @param[in] Ch              - Channel number (0-based)
  @param[in] Dimm            - DIMM number (0-based)
  @param[in] SubChannelMask  - Sub channel mask
  @param[in] Rank            - Rank number (0-based)

@retval N/A

**/
VOID
InitDbInterface (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  )
{
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT         ParityAndAlert;
  DDR5_DATA_BUFFER_PG2_RWF0_STRUCT                    VrefMdq;
  DDR5_DATA_BUFFER_RW84_STRUCT                        DataBufferRw84;
  DDR5_DATA_BUFFER_RW85_STRUCT                        DataBufferRw85;
  DDR5_DATA_BUFFER_RW86_STRUCT                        DataBufferRw86;
  DDR5_DATA_BUFFER_RW87_STRUCT                        DataBufferRw87;
  DDR5_DATA_BUFFER_RW88_STRUCT                        DataBufferRw88;
  DDR5_DATA_BUFFER_RW8A_STRUCT                        DataBufferRw8a;
  DDR5_DATA_BUFFER_RW8B_STRUCT                        DataBufferRw8b;
  DDR5_DATA_BUFFER_RW8C_STRUCT                        DataBufferRw8c;
  DDR5_DATA_BUFFER_PG2_RWE0_STRUCT                    DataBufferPg2E0;
  DDR5_DATA_BUFFER_PG2_RWFA_STRUCT                    Bvref;
  UINT16                                              LrdimmFrequency = 0;
  UINT16                                              FineGranValue = 0;
  UINT8                                               Strobe;
  UINT8                                               MSVx4 = GetMaxStrobeValid (Host);
  UINT8                                               DramOrDbHostRon;
  struct channelNvram                                 (*ChannelNvList) [MAX_CH];
  struct dimmNvram                                    (*DimmNvList)[MAX_DIMM];
  struct ddrRank                                      (*RankList)[MAX_RANK_DIMM];
  struct rankDevice                                   (*RankStruct)[MAX_RANK_DIMM];

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "InitDbInterface starts\n");

  ChannelNvList = GetChannelNvList (Host, Socket);
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  RankStruct  = GetRankStruct (Host, Socket, Ch, Dimm);

  //
  // Enable DRAM interface to forward all commands to DB whil blocking commands to DRAM.
  //
  ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
  RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

  //set DB ODT
  if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4800) {
    // Set LRDIMM Operating Speed (RW84)
    DataBufferRw84.Data = 0;
    DataBufferRw84.Bits.operating_speed = LRDIMM_RW84_DDR5_3200;
    if (Host->nvram.mem.socket[Socket].ddrFreq > DDR_3200)  {
      DataBufferRw84.Bits.operating_speed = LRDIMM_RW84_DDR5_3600;
    }
    if (Host->nvram.mem.socket[Socket].ddrFreq > DDR_3600)  {
      DataBufferRw84.Bits.operating_speed = LRDIMM_RW84_DDR5_4000;
    }
    if (Host->nvram.mem.socket[Socket].ddrFreq > DDR_4000)  {
      DataBufferRw84.Bits.operating_speed = LRDIMM_RW84_DDR5_4400;
    }
    if (Host->nvram.mem.socket[Socket].ddrFreq > DDR_4400)  {
      DataBufferRw84.Bits.operating_speed = LRDIMM_RW84_DDR5_4800;
    }
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW84_REG, DataBufferRw84.Data);

    // Set LRDIMM Fine Granularity Operating Speed (RW85)
    LrdimmFrequency = Host->nvram.mem.socket[Socket].ddrFreqMHz * 2;
    FineGranValue = LrdimmFrequency % LRDIMM_RW85_MAX_GRAN;  // Put frequency in the range 0-400
    // If frequency is a multiple of 400, leave the granularity setting as 0
    if (FineGranValue != 0) {
      FineGranValue = LRDIMM_RW85_MAX_GRAN - FineGranValue;  // Find how far from the max of 400 the value is
      FineGranValue = FineGranValue / LRDIMM_RW85_GRAN_INC;  // Divide by 20 (incremental value) for register value
    }
    DataBufferRw85.Data = 0;
    DataBufferRw85.Bits.fine_granularity_operating_speed_in_terms_of = (UINT8) FineGranValue;
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW85_REG, DataBufferRw85.Data);

    // Set DB DQS rtt park
    DataBufferRw86.Data = 0;
    DataBufferRw86.Bits.dqs_rtt_park = (UINT8)(*RankList)[Rank].DqsRttPrk;
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW86_REG, DataBufferRw86.Data);

    // Set DB DQ rtt park and write
    DataBufferRw87.Data = 0;
    DataBufferRw87.Bits.dq_rtt_park = (UINT8)(*RankList)[Rank].RttPrk;
    DataBufferRw87.Bits.dq_rtt_wr = (UINT8)(*RankList)[Rank].RttWr;
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW87_REG, DataBufferRw87.Data);

    // Set Host Output Driver Impedance control
    DataBufferRw88.Data = 0;
    DataBufferRw88.Bits.dq_rtt_nom_rd = (UINT8) (*RankList)[Rank].RttNomRd;
    DataBufferRw88.Bits.dq_rtt_nom_wr = (UINT8) (*RankList)[Rank].RttNomWr;
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW88_REG, DataBufferRw88.Data);

    // Set Host Output Driver Impedance control
    DataBufferRw8a.Data = 0;
    GetDramOrDbHostRon (Socket, Ch, Dimm, &DramOrDbHostRon);
    DataBufferRw8a.Bits.host_interface_dq_dqs_output_driver_pull_up = (UINT8) ConvertOdtValueDdr5 (DbHostRonPullUp, DramOrDbHostRon, ENCODE_RTT);
    DataBufferRw8a.Bits.host_interface_dq_dqs_output_driver_pull_down = (UINT8) ConvertOdtValueDdr5 (DbHostRonPullDown, DramOrDbHostRon, ENCODE_RTT);
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW8A_REG, DataBufferRw8a.Data);

    // Set MDQ Output Driver Impedance control
    DataBufferRw8b.Data = 0;
    DataBufferRw8b.Bits.dram_interface_mdq_mdqs_output_driver_pull_up = DDR5_MDQ_RON_34_BITS;
    DataBufferRw8b.Bits.dram_interface_mdq_mdqs_output_driver_pull_down = DDR5_MDQ_RON_34_BITS;
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW8B_REG, DataBufferRw8b.Data);

    DataBufferRw8c.Data = 0;
    DataBufferRw8c.Bits.mdq_rtt_park = DDR5_RTT_60;
    DataBufferRw8c.Bits.mdqs_rtt_park = DDR5_RTT_60;
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW8C_REG, DataBufferRw8c.Data);

    //Set VrefMDQ to 76% for DR and 68% for SR. For CPS 68%
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGE2);
    VrefMdq.Data = 0;
    Bvref.Data = 0;
    DimmNvList = GetDimmNvList(Host, Socket, Ch);
    if ((*DimmNvList)[Dimm].numRanks == 2) {
      VrefMdq.Bits.Vref_MDQ = MDQ_VREF_INIT_VALUE_DR;
    } else {
      VrefMdq.Bits.Vref_MDQ = MDQ_VREF_INIT_VALUE_SR;
    }

    //
    // Save backside RxVref safe value.
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      (*RankList)[Rank].lrbufRxVref[Strobe] = VrefMdq.Data;
      (*RankStruct)[Rank].lrbufRxVrefCache[Strobe] = VrefMdq.Data;
    }

    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWF0_REG, VrefMdq.Data);
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWF1_REG, VrefMdq.Data);

    // Set VREF DQ to value from ODT table
    DataBufferPg2E0.Data = 0;
    DataBufferPg2E0.Bits.vref_dq = (UINT8) (*ChannelNvList)[Ch].txVrefSafe[Rank];
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWE0_REG, DataBufferPg2E0.Data);
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWE1_REG, DataBufferPg2E0.Data);
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWE2_REG, DataBufferPg2E0.Data);
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWE3_REG, DataBufferPg2E0.Data);
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWE4_REG, DataBufferPg2E0.Data);
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWE5_REG, DataBufferPg2E0.Data);
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWE6_REG, DataBufferPg2E0.Data);
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWE7_REG, DataBufferPg2E0.Data);

    Bvref.Bits.B_Vref = B_VREF_INIT_VALUE;
    if ((*DimmNvList)[Dimm].DcpmmPresent == 1 && (*DimmNvList)[Dimm].fmcType == FMC_CWV_TYPE) {
      Bvref.Bits.B_Vref = B_VREF_CPS_INIT_VALUE;
    }
    DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG2_RWFA_REG, Bvref.Data);
  }
  //
  // Add some delay
  //
  FixedDelayMicroSecond (1);
  return;
} // InitDbInterface

/**
Writes Vref CS Command wihout updating the NVRAM cache

@param[in]  Host             - Pointer to sysHost
@param[in]  Socket           - Socket Id
@param[in]  Ch               - Channel number (0-based)
@param[in]  Dimm             - DIMM number (0-based)
@param[in]  Subchannelmask   - Sub channel mask
@param[in]  Rank             - Rank number (0-based)
@param[in]  Dram             - DRAM number in the subchannel
@param[in]  Opcode           - Opcode for Vref CA command
@param[in]  Timingmode       - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
@param[in]  PassThroughMode  - PassThrough mode or not

@retval N/A

**/
VOID
WriteVrefCsNoCacheUpdate (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Subchannelmask,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  )
{
  DDR5_RCD_GLOBAL_FEATURES_STRUCT    OrgGlobalFeatures;
  DDR5_RCD_GLOBAL_FEATURES_STRUCT    GlobalFeatures;
  struct dimmNvram                   (*DimmNvList) [MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "Vref CS command with Opcode 0x%04X\n", Opcode);

  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    //
    // If not in CA Pass Thru mode then switch it
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures.Data);
    OrgGlobalFeatures.Data = GlobalFeatures.Data;
    if (OrgGlobalFeatures.Bits.ca_pass_through_mode_enable == 0) {
      PassThroughMode = 1;
      GlobalFeatures.Bits.ca_pass_through_mode_enable = 1;
      GlobalFeatures.Bits.ca_pass_through_mode_rank_selection = Rank;
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures.Data);
    }

    WriteVrefCsDdr5 (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode);

    //
    // Restore GlobalFeatures if switched
    // Note: Rank needs to xor 1 since RW00[3] decide what Rank RCW will process MRW command When Pass Thru Mode is enabled
    //
    if (OrgGlobalFeatures.Bits.ca_pass_through_mode_enable == 0) {
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank ^ 1, DDR5_RCD_GLOBAL_FEATURES_REG, OrgGlobalFeatures.Data);
    }
  } else { // UDIMM
    WriteVrefCsDdr5 (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode);
  }

  return;
} // WriteVrefCsNoCacheUpdate

/**
Writes Vref CS Command and update NVRAM cache

@param[in]  Host             - Pointer to sysHost
@param[in]  Socket           - Socket Id
@param[in]  Ch               - Channel number (0-based)
@param[in]  Dimm             - DIMM number (0-based)
@param[in]  Subchannelmask   - Sub channel mask
@param[in]  Rank             - Rank number (0-based)
@param[in]  Dram             - DRAM number in the subchannel
@param[in]  Opcode           - Opcode for Vref CA command
@param[in]  Timingmode       - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
@param[in]  PassThroughMode  - PassThrough mode or not

@retval N/A

**/
VOID
WriteVrefCs (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Subchannelmask,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  )
{
  WriteVrefCsNoCacheUpdate (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Dram, Opcode, Timingmode, PassThroughMode);

  SaveDramVrefCs (Socket, Ch, Subchannelmask, Dimm, Rank, Dram, Opcode);

  return;
} // WriteVrefCs

/**
  Writes Vref CA Command without updating the NVRAM cache

  @param[in]  Host             - Pointer to sysHost
  @param[in]  Socket           - Socket Id
  @param[in]  Ch               - Channel number (0-based)
  @param[in]  Dimm             - DIMM number (0-based)
  @param[in]  Subchannelmask   - Sub channel mask
  @param[in]  Rank             - Rank number (0-based)
  @param[in]  Dram             - DRAM number in the subchannel
  @param[in]  Opcode           - Opcode for Vref CA command
  @param[in]  Timingmode       - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
  @param[in]  PassThroughMode  - PassThrough mode or not

  @retval N/A

**/
VOID
WriteVrefCaNoCacheUpdate (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Subchannelmask,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  )
{
  DDR5_RCD_GLOBAL_FEATURES_STRUCT    OrgGlobalFeatures;
  DDR5_RCD_GLOBAL_FEATURES_STRUCT    GlobalFeatures;
  struct dimmNvram                   (*DimmNvList) [MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "Vref CA command with Opcode 0x%04X\n", Opcode);

  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    //
    // If not in CA Pass Thru mode then switch it
    //
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures.Data);
    OrgGlobalFeatures.Data = GlobalFeatures.Data;
    if (OrgGlobalFeatures.Bits.ca_pass_through_mode_enable == 0) {
      PassThroughMode = 1;
      GlobalFeatures.Bits.ca_pass_through_mode_enable = 1;
      GlobalFeatures.Bits.ca_pass_through_mode_rank_selection = Rank;
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures.Data);
    }

    WriteVrefCaDdr5 (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode);

    //
    // Restore GlobalFeatures if switched
    // Note: Rank needs to xor 1 since RW00[3] decide what Rank RCW will process MRW command When Pass Thru Mode is enabled
    //
    if (OrgGlobalFeatures.Bits.ca_pass_through_mode_enable == 0) {
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank ^ 1, DDR5_RCD_GLOBAL_FEATURES_REG, OrgGlobalFeatures.Data);
    }
  } else { // UDIMM
    WriteVrefCaDdr5 (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode);
  }

  return;
} // WriteVrefCa

/**
Writes Vref CA Command

@param[in]  Host             - Pointer to sysHost
@param[in]  Socket           - Socket Id
@param[in]  Ch               - Channel number (0-based)
@param[in]  Dimm             - DIMM number (0-based)
@param[in]  Subchannelmask   - Sub channel mask
@param[in]  Rank             - Rank number (0-based)
@param[in]  Dram             - DRAM number in the subchannel
@param[in]  Opcode           - Opcode for Vref CA command
@param[in]  Timingmode       - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
@param[in]  PassThroughMode  - PassThrough mode or not

@retval N/A

**/
VOID
WriteVrefCa (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Subchannelmask,
  IN UINT8    Rank,
  IN UINT8    Dram,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  )
{
  WriteVrefCaNoCacheUpdate (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Dram, Opcode, Timingmode, PassThroughMode);

  SaveDramVrefCa (Socket, Ch, Subchannelmask, Dimm, Rank, Dram, Opcode);

  return;
} // WriteVrefCa

/**
  Determine if the given DQ nibble number is valid for ECC mode on current Socket for DDR5.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Nibble        - DQ nibble number to check

  @retVal   1     DQ nibble number is not valid
  @retVal   0     DQ nibble number is valid
**/
BOOLEAN
IsNibbleNotValidDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Nibble
  )
{
  UINT8 MaxSubChNibble;

  if (!Host->nvram.mem.eccEn) {
    MaxSubChNibble = GetSubChMaxStrobeValid (Host) - 2;
  } else {
    MaxSubChNibble = GetSubChMaxStrobeValid (Host);
  }
  if (Nibble >= MaxSubChNibble) {
    return 1;
  }
  return 0;
} // IsNibbleNotValidDdr5

/**
  Determine if the given Strobe number is valid for ECC mode on current Socket for DDR5.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Strobe        - Strobe number to check

  @retVal   1     Strobe number is not valid
  @retVal   0     Strobe number is valid
**/
BOOLEAN
IsStrobeNotValidDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Strobe
  )
{
  UINT8  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  if ((Strobe >= SubChMSVx4) ||
      ((!Host->nvram.mem.eccEn) && ((Strobe == (SubChMSVx4 /2 - 1)) || (Strobe == (SubChMSVx4 - 1))))) {
    return 1;
  }
  return 0;
}

/**
  Determine if the given Strobe number is valid for ECC mode on current Socket for DDR5 full channel.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Strobe        - Strobe number to check

  @retVal   1     Strobe number is not valid
  @retVal   0     Strobe number is valid
**/
BOOLEAN
EFIAPI
IsStrobeNotValidChDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Strobe
  )
{
  UINT8  ChMSVx4 = MAX_STROBE_DDR5; //SPR_TODO. GetMaxStrobeValid(Host);

  if ((Strobe >= ChMSVx4) ||
      ((!Host->nvram.mem.eccEn) &&
       ((Strobe == (ChMSVx4 /4 - 1)) || (Strobe == (ChMSVx4/2 - 1)) ||
        (Strobe == ((ChMSVx4 /4 - 1) + ChMSVx4/2)) || ((Strobe == (ChMSVx4/2 - 1) + ChMSVx4/2))))) {
    return 1;
  }
  return 0;
}

/**
  Determine if the given Dimm and Strobe number is valid for ECC mode on current Socket for DDR5.

  @param[in]  DimmNvramStructPtr  Pointer to dimmNvram structure to check
  @param[in]  Strobe              Strobe number to check

  @retVal   TRUE    Strobe number is not valid
  @retVal   FALSE   Strobe number is valid
**/
BOOLEAN
EFIAPI
IsDimmStrobeNotValidDdr5 (
  IN struct dimmNvram *DimmNvramStructPtr,
  IN UINT8            Strobe
  )
{
  PSYSHOST  Host;
  UINT8     SubChMSVx4;
  UINT8     SubChMSVx8;

  Host = GetSysHostPointer ();
  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  SubChMSVx8 = SubChMSVx4 / 2;

  //
  // DDR5_TODO. Check the sub channel enabled or disabled.
  //
  if ((*DimmNvramStructPtr).x4Present) {
    if (Strobe >= SubChMSVx4) {
      return TRUE;
    } else if ((!Host->nvram.mem.eccEn) && ((Strobe == (SubChMSVx8 - 1)) || (Strobe == (SubChMSVx4 - 1)))) {
      return TRUE;
    }
  } else {
    if (Strobe >= SubChMSVx8) {
      return TRUE;
    } else if ((!Host->nvram.mem.eccEn) && (Strobe == (SubChMSVx8 - 1))) {
      return TRUE;
    }
  }

  return FALSE;
} // IsDimmStrobeNotValidDdr5


  /**
  Determines a bit mask of valid Strobe numbers for ECC mode on current Socket for DDR5.

  @param[in]      Host          - Pointer to sysHost

  @retVal   UINT32    Bit mask of valid Strobe numbers
  **/
UINT32
GetSubChStrobeMaskDdr5 (
  IN PSYSHOST  Host
  )
{
  UINT32 StrobeMask = 0;
  UINT8 Strobe;
  UINT8 SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    StrobeMask |= (1 << Strobe);
  }
  return StrobeMask;
} // GetSubChStrobeMaskDdr5

/**

Writes Multi-Purpose Command for DDR5

@param Host                 - Pointer to sysHost, the system Host (root) structure
@param Socket               - Socket Id
@param Ch                   - Channel number (0-based)
@param Dimm                 - DIMM number (0-based)
@param Subchannelmask       - sub channel mask
@param Rank                 - Rank number (0-based)
@param Opcode               - Opcode for the MPC command
@param Timingmode           - support MPC_1N, MPC_2N, MPC_4N.

@retval None

**/
VOID
WriteMpcDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  )
{

  if (Host->var.mem.socket[Socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  WriteCadbMpcCmd (Host, Socket, Ch, Dimm, SubChannelMask, Rank, Opcode, Timingmode, PassThroughMode);

  //
  // tMRD: Mode Register Set command delay
  //
  FixedDelayMicroSecond (1);
  return;
} // WriteMpcDdr5

/**

Writes Vref CA Command for DDR5

@param[in] Host             - Pointer to sysHost
@param[in] Socket           - Socket Id
@param[in] Ch               - Channel number (0-based)
@param[in] Dimm             - DIMM number (0-based)
@param[in] Subchannelmask   - Sub channel mask
@param[in] Rank             - Rank number (0-based)
@param[in] Opcode           - Vref CA command opcode
@param[in] Timingmode       - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
@param[in] PassThroughMode  - PassThrough mode or not

@retval None

**/
VOID
WriteVrefCaDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Subchannelmask,
  IN UINT8    Rank,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  )
{

  if (Host->var.mem.socket[Socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }
  // DDR5_TODO with Subchannelmask.
  WriteCadbVrefCaCmd (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode);

  //
  // tMRD: Mode Register Set command delay
  //
  FixedDelayMicroSecond (1);
  return;
} // WriteVrefCaDdr5

/**

Writes Vref CS Command for DDR5

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket Id
  @param[in] Ch               - Channel number (0-based)
  @param[in] Dimm             - DIMM number (0-based)
  @param[in] Subchannelmask   - Sub channel mask
  @param[in] Rank             - Rank number (0-based)
  @param[in] Opcode           - Vref CA command opcode
  @param[in] Timingmode       - Supports TIMING_SINGLE_CS, TIMING_MULTI_CS
  @param[in] PassThroughMode  - PassThrough mode or not

  @retval None

**/
VOID
WriteVrefCsDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Subchannelmask,
  IN UINT8    Rank,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
)
{

  if (Host->var.mem.socket[Socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  WriteCadbVrefCsCmd (Host, Socket, Ch, Dimm, Subchannelmask, Rank, Opcode, Timingmode, PassThroughMode);

  FixedDelayMicroSecond (1);
  return;
} // WriteVrefCsDdr5

/**

Get the operating speed value for the RW05.

@param[in] Host            - Pointer to sysHost
@param[in] Socket          - Socket number

@retval RCW speed value

**/
UINT8
GetSpeedRcwValueDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8 SpeedRcwValue = 0;

  if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_2800) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH,  NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Warning: The DDR frequency %d is not supported\n", Host->nvram.mem.socket[Socket].ddrFreq);
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3200) {
    SpeedRcwValue = 0;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3600) {
    SpeedRcwValue = 1;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4000) {
    SpeedRcwValue = 2;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4400) {
    SpeedRcwValue = 3;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4800) {
    SpeedRcwValue = 4;
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH,  NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Warning: The DDR frequency %d is not supported\n", Host->nvram.mem.socket[Socket].ddrFreq);
  }
  return SpeedRcwValue;
} // GetSpeedRcwValueDdr5

/**

Get the operating speed value for the RW05.

@param[in] Host            - Pointer to sysHost
@param[in] Socket          - Socket number

@retval RCW speed value

**/
UINT8
GetSpeedRcwValue (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
#if FixedPcdGetBool(PcdMcrSupport)
  return GetSpeedRcwValueMcr (Socket);
#else
  return GetSpeedRcwValueDdr5 (Host, Socket);
#endif // FixedPcdGetBool(PcdMcrSupport)
} // GetSpeedRcwValue

//
// New for DDR5
//

/**
  Determine if a DDR memory subsystem is present in the given Socket

  @param[in]      Host          Pointer to sysHost structure
  @param[in]      Socket        Processor Socket

  @retval 1 DDR memory subsystem is presetn
  @retval 0 DDR memory subsystem is not present
**/
BOOLEAN
IsDdr5Present (
  IN PSYSHOST  Host,
  IN UINT32    Socket
  )
{

  //
  // The compiler is not able to determine the value of
  // Host->var.mem.socket[SocketLocal].MemSsList at
  // compile time, which means that DDR4 code get pulled
  // into DDR5 builds which increases the size of the binary.
  // So rather than check MemSslist, just look at DDR5_SUPPORT.
  //

#ifdef DDR5_SUPPORT

  return TRUE;

#else // #ifdef DDR5_SUPPORT

  return FALSE;

#endif // #ifdef DDR5_SUPPORT


} // IsDdr5Present


/**
  Release QRST for DRAM subchannel A and B after RCD reset

  @param(in)  Host        - Pointer to sysHost
  @aparm(in)  Socket      - Socket number
  @aparm(in)  Channel     - Channel number within the socket
  @aparm(in)  Dimm        - DIMM number within the channel
  @aparm(in)  Rank        - Rank number within the DIMM

  retval N/A

**/
VOID
RcdReleaseQrst (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      Dimm,
  IN UINT8      Rank
  )
{
  UINT32                            SmbData;
  DDR5_RCD_COMMAND_SPACE_STRUCT     CommandSpace;

  if (Host->nvram.mem.dimmTypePresent != RDIMM) {
    return;
  }

  //
  // Release QRST. As it's a command space control word, no need to store the cache value.
  //
  CommandSpace.Data = 0;
  CommandSpace.Bits.command = RCD_COMMAND_CLEAR_CH_A_DRAM_RESET;
  SmbData = CommandSpace.Data;
  WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_COMMAND_SPACE_REG, &SmbData);
  //
  // tMRC
  //
  TrainingDelay (Host, 64, 1);

  CommandSpace.Data = 0;
  CommandSpace.Bits.command = RCD_COMMAND_CLEAR_CH_B_DRAM_RESET;
  SmbData = CommandSpace.Data;
  WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_COMMAND_SPACE_REG, &SmbData);
  //
  // tMRC
  //
  TrainingDelay (Host, 64, 1);
}

/**
  Configure RCD Control Words as required by the JEDEC RCD reset sequence during power-up

  @param(in)  Host        - Pointer to sysHost
  @aparm(in)  Socket      - Socket number
  @aparm(in)  Channel     - Channel number within the socket
  @aparm(in)  Dimm        - DIMM number within the channel
  @aparm(in)  Rank        - Rank number within the DIMM

  retval N/A

**/
VOID
RcdCwPreTrainingInit (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      Dimm,
  IN UINT8      Rank
  )
{
  UINT8                                 RankInChannel;
  UINT8                                 OriginalTimingMode;
  UINT32                                SmbData = 0;
  DDR5_RCD_GLOBAL_FEATURES_STRUCT       GlobalFeatures;
  DDR5_RCD_FINE_GRANULARITY_DIMM_OPERATING_SPEED_STRUCT FineGranularityDimmOperatingSpeed;
  DDR5_RCD_IBT_GLOBAL_STRUCT            IbtGlobal;
#ifdef USE_LATEST_RCD_SPEC
  DDR5_RCD_DIMM_OPERATING_SPEED_STRUCT  DimmOperatingSpeed;
#endif //USE_LATEST_DRAM_SPEC
  struct channelNvram                   (*ChannelNvList) [MAX_CH];
#if FixedPcdGetBool(PcdMcrSupport)
  DDR5_MCR_RCD_PG70_RW60_STRUCT         McrRcdPg70Rw60;
#endif // FixedPcdGetBool(PcdMcrSupport)

  if (Host->nvram.mem.dimmTypePresent != RDIMM) {
    return;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  OriginalTimingMode = (*ChannelNvList)[Channel].timingMode;
  if ((FeaturePcdGet (PcdCteBuild) == TRUE) || (UbiosGenerationOrHsleEnabled ()) && (IsMcrPresentChannel (Socket, Channel) == FALSE)) {
    if (OriginalTimingMode == TIMING_1N) {
      //
      //Change MC to 2N mode
      //
      SetTimingMode (Host, Socket, Channel, TIMING_2N);
      (*ChannelNvList) [Channel].timingMode = TIMING_2N;
    }
  }

#ifdef USE_LATEST_RCD_SPEC
  GlobalFeatures.Data = 0;
#if FixedPcdGetBool(PcdMcrSupport)
  if (IsMcrPresentChannel (Socket, Channel)) {
    GlobalFeatures.Bits.command_address_rate = 1; // Enable DDR mode for MCR
  }
#endif // FixedPcdGetBool(PcdMcrSupport)
  RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures.Data);

#if FixedPcdGetBool(PcdMcrSupport)
  if (IsMcrPresentChannel (Socket, Channel)) {
    McrRcdPg70Rw60.Data = 0;
    if (IsX4Dimm (Socket, Channel, Dimm)) {
      McrRcdPg70Rw60.Bits.memory_data_width = MCR_x4_DRAM;
    } else {
      McrRcdPg70Rw60.Bits.memory_data_width = MCR_x8_DRAM;
    }
    McrRcdPg70Rw60.Bits.scrambling_enable = 0;
    McrRcdPg70Rw60.Bits.operating_mode    = MCR_DDR5_MODE;
    SendMrw (Host, Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_MCR_RCD_PG70_RW60_REG, CW_RCD_DB, McrRcdPg70Rw60.Data);
  }
#endif // FixedPcdGetBool(PcdMcrSupport)

  //
  // RW05
  //
  if (Host->var.mem.socket[Socket].hostRefreshStatus == 0) {
    DimmOperatingSpeed.Data = 0;
    DimmOperatingSpeed.Bits.operating_speed = GetSpeedRcwValue (Host, Socket) & 0x0F;
    RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_DIMM_OPERATING_SPEED_REG, DimmOperatingSpeed.Data);
  }
  //
  // tSTABx
  //
  TrainingDelay (Host, 600, 2);
#endif //USE_LATEST_DRAM_SPEC
  //
  // RW06
  //
  FineGranularityDimmOperatingSpeed.Data = 0;
  RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank,
  DDR5_RCD_FINE_GRANULARITY_DIMM_OPERATING_SPEED_REG, FineGranularityDimmOperatingSpeed.Data);
  //
  // tSTABx
  //
  TrainingDelay (Host, 600, 2);

  RankInChannel = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);
  //
  // RW10
  //
  IbtGlobal.Data = 0;
  IbtGlobal.Bits.dca_6_0_b_a_and_dpar_b_a_input_bus_termination = (*ChannelNvList)[Channel].IbtCaRegValue[RankInChannel];
  IbtGlobal.Bits.dcs_1_0_b_a_n_input_bus_termination = (*ChannelNvList)[Channel].IbtCsRegValue[RankInChannel];
  IbtGlobal.Bits.dck_input_bus_termination = (*ChannelNvList)[Channel].IbtClkRegValue[RankInChannel];
  RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_IBT_GLOBAL_REG, IbtGlobal.Data);
  //
  // Delay at least tMRD_L (16) clocks (much less than 1us)
  //
  FixedDelayMicroSecond (1); // 1us

  //
  // Resetore RCD DDR, DRAM 1N mode if needed
  //
  if (OriginalTimingMode == TIMING_1N) {
    ReadRcdCwCache (Socket, Channel, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures.Data);
    GlobalFeatures.Bits.command_address_rate = 1;
    SmbData = GlobalFeatures.Data;
    if ((FeaturePcdGet (PcdCteBuild) == TRUE) || (UbiosGenerationOrHsleEnabled ())) {
      RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures.Data);
      //
      // Change back MC to 1N mode
      //
      SetTimingMode (Host, Socket, Channel, TIMING_1N);
      (*ChannelNvList) [Channel].timingMode = TIMING_1N;
    }
    WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0,
      DDR5_RCD_GLOBAL_FEATURES_REG, &SmbData);
    //
    // tSTABx: wait for the PLL to be ready
    //
    TrainingDelay (Host, 600, 2);
  }

  //
  // Clear CS_n override - allow CS_n to be driven with commands from the host
  //
  OverrideCke (Socket, Channel, 0, 0);

}

/**
  Configure RCD Control Words over sideband as required by the JEDEC RCD reset sequence during power-up

  @param(in)  Host        - Pointer to sysHost
  @aparm(in)  Socket      - Socket number
  @aparm(in)  Channel     - Channel number within the socket
  @aparm(in)  Dimm        - DIMM number within the channel
  @aparm(in)  Rank        - Rank number within the DIMM

  retval N/A

**/
VOID
JedecRcdCwInitialization (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Channel,
  IN UINT8      Dimm,
  IN UINT8      Rank
  )
{
  UINT8                                                 RankInChannel;
  UINT32                                                SmbData;
  DDR5_RCD_GLOBAL_FEATURES_STRUCT                       GlobalFeatures;
  DDR5_RCD_FINE_GRANULARITY_DIMM_OPERATING_SPEED_STRUCT FineGranularityDimmOperatingSpeed;
  DDR5_RCD_IBT_GLOBAL_STRUCT                            IbtGlobal;
  DDR5_RCD_DIMM_OPERATING_SPEED_STRUCT                  DimmOperatingSpeed;
  struct channelNvram                                   (*ChannelNvList) [MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Operating speed
  //
  if (Host->var.mem.socket[Socket].hostRefreshStatus == 0) {
    DimmOperatingSpeed.Data = 0;
    DimmOperatingSpeed.Bits.operating_speed = GetSpeedRcwValue (Host, Socket) & 0x0F;
    SmbData = DimmOperatingSpeed.Data;
    WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_DIMM_OPERATING_SPEED_REG, &SmbData);
  }
  //
  // tSTABx
  //
  TrainingDelay (Host, 600, 2);
  //
  // Fine granularity speed
  //
  FineGranularityDimmOperatingSpeed.Data = 0;
  SmbData = FineGranularityDimmOperatingSpeed.Data;
  WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_FINE_GRANULARITY_DIMM_OPERATING_SPEED_REG, &SmbData);
  //
  // tSTABx
  //
  TrainingDelay (Host, 600, 2);

  RankInChannel = GetLogicalRank (Host, Socket, Channel, Dimm, Rank);
  //
  // Input bus termination
  //
  IbtGlobal.Data = 0;
  IbtGlobal.Bits.dca_6_0_b_a_and_dpar_b_a_input_bus_termination = (*ChannelNvList)[Channel].IbtCaRegValue[RankInChannel];
  IbtGlobal.Bits.dcs_1_0_b_a_n_input_bus_termination = (*ChannelNvList)[Channel].IbtCsRegValue[RankInChannel];
  IbtGlobal.Bits.dck_input_bus_termination = (*ChannelNvList)[Channel].IbtClkRegValue[RankInChannel];
  SmbData = IbtGlobal.Data;
  WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_IBT_GLOBAL_REG, &SmbData);
  //
  // Delay at least tMRD_L (16) clocks (much less than 1us)
  //
  FixedDelayMicroSecond (1);

  //
  // Command rate (RCD DDR/SDR, DRAM 1N/2N)
  //
  ReadRcdCwCache (Socket, Channel, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures.Data);
  if ((*ChannelNvList)[Channel].timingMode == TIMING_1N) {
    GlobalFeatures.Bits.command_address_rate = 1;
  } else {
    GlobalFeatures.Bits.command_address_rate = 0;
  }
  SmbData = GlobalFeatures.Data;
  WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, SUB_CH_A_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &SmbData);
  //
  // tSTABx: wait for the PLL to be ready
  //
  TrainingDelay (Host, 600, 2);

}

/**

  Starts the JEDEC initialization sequence

  @param Host    - Pointer to sysHost
  @param Socket  - Socket to initizlize

  @retval SUCCESS

**/
UINT32
JedecInitPreTraining (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT8                                 Dimm;
  UINT8                                 Rank;
  UINT8                                 Ch;
  UINT8                                 RankIndex;
  struct channelNvram                   (*ChannelNvList) [MAX_CH];
  struct dimmNvram                      (*DimmNvList) [MAX_DIMM];
  UINT8                                 MaxChDdr;
  DDR5_MODE_REGISTER_8_STRUCT           Mr8;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // When VDD is not powered on, the RCD does not respond to any commands or events.
  // Should switch to I3C mode again before accesing RCD
  //
  I3cSwitchToI3cMode (Socket);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (IsChannelEnabled (Socket, Ch) == FALSE) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }

        if ((*DimmNvList) [Dimm].DcpmmPresent == 1) {
          continue;
        }

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        RankIndex = GetRankIndex (Socket, Ch, Dimm, Rank);

        //
        // Configure RCD Control Words as required by the JEDEC RCD reset sequence during power-up
        //
        RcdCwPreTrainingInit (Host, Socket, Ch, Dimm, Rank);
        //
        // Release QRST and wait tMRC
        //
        RcdReleaseQrst (Host, Socket, Ch, Dimm, Rank);
        //
        // tSTABx
        //
        TrainingDelay (Host, 600, 2);

        if (FeaturePcdGet (PcdCteBuild) == TRUE && IsMcrPresentChannel (Socket, Ch) == FALSE) {
          //
          // Init DRAM interface for CTE.
          //
          InitDramInterface (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank);
        }
        if ((FeaturePcdGet (PcdCteBuild) == TRUE) || (UbiosGenerationOrHsleEnabled ())) {
          //
          // CTE WA and SOC emulation 1N mode enabling.
          //
          if ((*ChannelNvList) [Ch].timingMode == TIMING_1N && IsMcrPresentChannel (Socket, Ch) == FALSE) {
            EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);
            //
            // Change DRAM to 1N.
            //
            WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, SET_1N_CMD_TIMING, TIMING_4N, TRUE);
            //
            // Wait for the PLL to be ready (This delay can be tuned or removed based on the post silicon data).
            //
            TrainingDelay (Host, 600, 2);

            ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

            //
            // Notify ULT to change to 1N mode.
            //
            CteSetOptions ("cmd_stretch", 1 << Ch, TWO_SUBCH_MASK, 0, 1);
          }

          if ((UbiosGenerationOrHsleEnabled () || (FeaturePcdGet (PcdCteBuild) == TRUE)) && IsMcrPresentChannel (Socket, Ch) == FALSE) {
            //
            // Program premable value in MR8 for SOC emulation and CTE.
            //
            Mr8.Data = GetPreamblePostambleMr8Value (Host, Socket);
            EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

            DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_8_REG, Mr8.Data);
            TrainingDelay (Host, 64, 1);
            ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

          }
        }
      } //Ch
    } //Rank loop
  }//Dimm loop

  //
  // tSTABx
  //
  TrainingDelay (Host, 100, 2);
  return SUCCESS;
} // JedecInitPreTraining

/**
This function will setup CKEMask with Selected Chip Selects. This is used for reset and self-refresh exit

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket Id

@retVal None
**/

VOID
EFIAPI
SetCSMask (
  IN  PSYSHOST Host,
  IN  UINT8    Socket
  )
{
  UINT8 Ch;
  UINT8 SubChannel;
  UINT8 Dimm;
  UINT8 Rank;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  UINT8 MaxChDdr;
#if FixedPcdGetBool(PcdMcrSupport)
  UINT8 HostRank;
  UINT8 PsCh = 0;
#endif // FixedPcdGetBool(PcdMcrSupport)

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    (*ChannelNvList) [Ch].ckeMask = 0;
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }

      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
#if FixedPcdGetBool(PcdMcrSupport)
          for (PsCh = 0; PsCh < MAX_PS; PsCh++) {
            HostRank = GetHostRankId (Socket, Ch, Dimm, Rank, PsCh);
            (*ChannelNvList)[Ch].ckeMask |= 1 << ((SubChannel * 4) + (Dimm * 2) + HostRank);
          }
#else
          (*ChannelNvList) [Ch].ckeMask |= 1 << ((SubChannel * 4) + (Dimm * 2) + Rank);
#endif    // FixedPcdGetBool(PcdMcrSupport)
        } // subchannel loop

      } // Rank loop
    } // Dimm loop
  } // Ch loop
} // SetCSMask

/**

  Choose QLBD/QLBS As The Feedback

  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  Dimm            - Dimm number
  @param[in]  Rank            - Rank number

  @retval: None
**/
VOID
EFIAPI
ChooseQLBAsFeedback (
  IN      UINT8    Socket,
  IN      UINT8    Channel,
  IN      UINT8    Dimm,
  IN      UINT8    Rank
  )
{
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT ParityAlertReg;

  if (IsDcpmmPresentDimm (Socket, Channel, Dimm) == FALSE) {
    //
    // Choose QLBD/QLBS as the feedback.
    //
    ReadRcdCwCache (Socket, Channel, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &ParityAlertReg.Data);
    ParityAlertReg.Bits.host_interface_training_feedback = 1;
#ifdef USE_LATEST_RCD_SPEC
    ParityAlertReg.Bits.dram_interface_forward_all_cmds = 0;
#else // #ifdef USE_LATEST_RCD_SPEC
    ParityAlertReg.Bits.dram_interface_block_all_cmds = 0;
#endif // #ifdef USE_LATEST_RCD_SPEC

    RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAlertReg.Data);
  }

}
/**

  Enables parity checking on the DDR5 RCD

  @param[in]  Socket:  Processor Socket to check
  @param[in]  Ch:      Channel number

  @retval: None
**/
VOID
EFIAPI
EnableParityCheckingDdr5 (
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  PSYSHOST                                    Host;
  UINT8                                       Dimm;
  UINT8                                       Rank;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT ParityAndAlert;
  struct dimmNvram                            (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  Rank = 0;
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if (IsPmemSiliconWorkaroundEnabled (Host, Socket, Ch, Dimm, "S14011274250") && (Host->nvram.mem.socket[Socket].ddrFreq > DDR_3600)) {
      continue;
    }
    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &ParityAndAlert.Data);

    if (IsBrsPresent (Host, Socket)) {
      //
      // Disable the parity check for BRS
      //
      ParityAndAlert.Bits.parity_checking = 0;
#ifdef USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
      if (IsLrdimmPresent (Socket, Ch, Dimm)) {
        ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
      }
#else // USE_LATEST_RCD_SPEC
      ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
      if (IsLrdimmPresent (Socket, Ch, Dimm)) {
        ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
      }
#endif // USE_LATEST_RCD_SPEC
      RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);
      return;
    }

    ParityAndAlert.Bits.parity_checking = 1;
    ParityAndAlert.Bits.alert_n_assertion = 1;
    ParityAndAlert.Bits.alert_n_re_enable = 1;
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);
  } // Dimm
}

/**

  Disable parity checking on the DDR5 RCD

  @param[in]  Socket:  Processor Socket to check
  @param[in]  Ch:      Channel number

  @retval: None
**/
VOID
EFIAPI
DisableParityCheckingDdr5 (
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  PSYSHOST                                     Host;
  UINT8                                        Dimm;
  UINT8                                        Rank;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT  ParityAndAlert;
  struct dimmNvram                             (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  Rank = 0;
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &ParityAndAlert.Data);
    ParityAndAlert.Bits.parity_checking = 0;
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);
  } // Dimm
}

/**

  Check if RCD loopback mode should be used

  @param Host    - Pointer to sysHost

  @retval TURE/FALSE

**/
BOOLEAN
CheckRCDLoopback (
  PSYSHOST Host
  )
{
  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    return TRUE;
  } else {
    return FALSE;
  }
} // CheckRCDLoopback

/*

  This function Gets or Sets the QCA backside signals

  @param[in]      Host    Pointer to SysHost
  @param[in]      Socket  Current socket under test (0-based)
  @param[in]      Channel Current channel under test (0-based)
  @param[in]      Dimm    Current dimm under test (0-based)
  @param[in]      Rank    Current rank under test (0-based)
  @param[in]      Level   DDR level. LrbufLevel is the only allowed parameter
  @param[in]      Group   Group to be accessed (allowed: CmdAll, Cmd0All, Cmd1All)
  @param[in]      Mode    Mode of operation:
                          read (GSM_READ_ONLY <| GSM_READ_CSR>),
                          write (GSM_WRITE_OFFSET | GSM_FORCE_WRITE),
                          update cache (GSM_UPDATE_CACHE)
  @param[in,out]  Value   Pointer to memory location that contains the value to be written
                          or the location where the read value will be placed
  @param[in,out]  MinVal  Pointer to a memory location that keep track of the minimum value
  @param[in,out]  MaxVal  Pointer to a memory location that keep track of the maximum value

  @retval   EFI_INVALID_PARAMETER     if Level is not supported
  @retval   EFI_INVALID_PARAMETER     if Socket or Channel or Dimm are not within the allowed limits
  @retval   EFI_INVALID_PARAMETER     if any pointer to Value or MinValue or MaxValue is NULL
  @retval   EFI_UNSUPPORTED           for not supported groups
  @retval   EFI_UNSUPPORTED           if Max or Min thresholds are not met

*/
EFI_STATUS
EFIAPI
GetSetCaBacksideDdr5 (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value,
  IN OUT  UINT16    *MinVal,
  IN OUT  UINT16    *MaxVal
  )
{
  UINT8   SubChannel;
  UINT8   SubChannelBegin;
  UINT8   SubChannelEnd;
  INT16   CurVal;
  GSM_CSN Signal;
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  if (Level != LrbufLevel ||
      Socket >= MAX_SOCKET || Channel >= MAX_CH || Dimm >= MAX_DIMM ||
      Value == NULL || MinVal == NULL || MaxVal == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Group != CmdAll && Group != Cmd0All && Group != Cmd1All) {
    return EFI_UNSUPPORTED;
  }

  if (Group == CmdAll) {
    SubChannelBegin = 0;
    SubChannelEnd   = 2;
  } else if (Group == Cmd0All) {
    SubChannelBegin = 0;
    SubChannelEnd   = 1;
  } else {
    SubChannelBegin = 1;
    SubChannelEnd   = 2;
  }

  for (SubChannel = SubChannelBegin; SubChannel < SubChannelEnd; SubChannel++) {
    Signal = SignalsToSweepBacksideDDR5[ GetBacksideCaSignalIndex (Channel, SubChannel, Dimm, Rank, 0) ];
    CurVal = GetUpdateCachedQcaDelay (Host, Socket, Channel, Dimm, Rank, Signal, FALSE, NULL);
    CurVal &= ~QCA_OUTPUT_DELAY_EN; //< Remove Output delay feature enable

    if (Mode & GSM_READ_ONLY) {

      *Value = CurVal;

    } else {

      if (Mode & GSM_WRITE_OFFSET) {
        CurVal = CurVal + *Value;
      } else {
        CurVal = *Value;
      }

      if (CurVal < QCA_MIN_VAL) {
        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
          "%a Out of range!! CurVal = %d, MinLimit = %d\n",
          GetSignalStr (Signal), CurVal, QCA_MIN_VAL
          );
        CurVal = QCA_MIN_VAL;
        Status = EFI_UNSUPPORTED;
      }
      if (CurVal > QCA_MAX_VAL) {
        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
          "%a Out of range!! CurVal = %d, MaxLimit = %d\n",
          GetSignalStr (Signal), CurVal, QCA_MAX_VAL
          );
        CurVal = QCA_MAX_VAL;
        Status = EFI_UNSUPPORTED;
      }

      ProgramQcaDelay (Host, Socket, Channel, Dimm, SubChannel, Rank, Signal, GSM_FORCE_WRITE, (UINT8) CurVal, TRUE);
      GetUpdateCachedQcaDelay (Host, Socket, Channel, Dimm, Rank, Signal, TRUE, (UINT8 *)&CurVal);
      UpdateMinMaxInt (CurVal, MinVal, MaxVal);
    } // Mode == GSM_READ_ONLY
  } // SubChannel loop ...

  return Status;
}

/*

  This function Gets or Sets the QCS backside signals based on Dimm/Rank

  @param[in]      Host    Pointer to SysHost
  @param[in]      Socket  Current socket under test (0-based)
  @param[in]      Channel Current channel under test (0-based)
  @param[in]      Dimm    Current dimm under test (0-based)
  @param[in]      Rank    Current rank under test (0-based)
  @param[in]      Level   DDR level. LrbufLevel is the only allowed parameter
  @param[in]      Group   Group to be accessed (allowed: CtlAll, Ctl0Grp, Ctl1Grp)
  @param[in]      Mode    Mode of operation:
                          read (GSM_READ_ONLY <| GSM_READ_CSR>),
                          write (GSM_WRITE_OFFSET | GSM_FORCE_WRITE),
                          update cache (GSM_UPDATE_CACHE)
  @param[in,out]  Value   Pointer to memory location that contains the value to be written
                          or the location where the read value will be placed
  @param[in,out]  MinVal  Pointer to a memory location that keep track of the minimum value
  @param[in,out]  MaxVal  Pointer to a memory location that keep track of the maximum value

  @retval   EFI_INVALID_PARAMETER     if Level is not supported
  @retval   EFI_INVALID_PARAMETER     if Socket or Channel or Dimm are not within the allowed limits
  @retval   EFI_INVALID_PARAMETER     if any pointer to Value or MinValue or MaxValue is NULL
  @retval   EFI_UNSUPPORTED           for not supported groups
  @retval   EFI_UNSUPPORTED           if Max or Min thresholds are not met

*/
EFI_STATUS
EFIAPI
GetSetCsBacksideDdr5 (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value,
  IN OUT  UINT16    *MinVal,
  IN OUT  UINT16    *MaxVal
  )
{
  UINT8   SubChannel;
  UINT8   SubChannelBegin;
  UINT8   SubChannelEnd;
  GSM_CSN Signal;
  EFI_STATUS  Status;
  UINT8       BacksideCopy;

  Status = EFI_SUCCESS;

  if (Level != LrbufLevel ||
      Socket >= MAX_SOCKET || Channel >= MAX_CH || Dimm >= MAX_DIMM ||
      Value == NULL || MinVal == NULL || MaxVal == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Group != CtlAll && Group != Ctl0Grp && Group != Ctl1Grp) {
    return EFI_UNSUPPORTED;
  }

  if (Group == CtlAll) {
    SubChannelBegin = 0;
    SubChannelEnd   = 2;
  } else if (Group == Ctl0Grp) {
    SubChannelBegin = 0;
    SubChannelEnd   = 1;
  } else {
    SubChannelBegin = 1;
    SubChannelEnd   = 2;
  }

  for (SubChannel = SubChannelBegin; SubChannel < SubChannelEnd; SubChannel++) {

    Signal = CsToSweepBackSide[ GetBacksideCsSignalIndex (Channel, SubChannel, Dimm, Rank) ];
    for (BacksideCopy = 0; BacksideCopy < GetMaxBacksideCopyNum (Socket, Channel, Dimm); BacksideCopy++) {
      Status = GetSetQcsDelay (Host, Socket, Channel, Dimm, Rank, Signal, BacksideCopy, Mode, Value, MinVal, MaxVal);
    }
  } // SubChannel loop ...

  return Status;
}

/*

  This function Gets or Sets the QCA backside signals

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number
  @param[in] Signal           - Enumerated signal name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

*/
MRC_STATUS
EFIAPI
GetSetQcaDelay (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     SubChannel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      GSM_CSN   Signal,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  INT16                             CurVal = 0;
  INT16                             MinLimit;
  INT16                             MaxLimit;
  INT16                             Delay;
  UINT8                             Address = DDR5_RCD_QACA_OUTPUT_DELAY_REG;
  MRC_STATUS                        Status = MRC_STATUS_SUCCESS;
  DDR5_RCD_QACA_OUTPUT_DELAY_STRUCT QxcaOutputDelay;

  if (((Signal >= Q0ACA0) && (Signal <= Q0ACA13)) || ((Signal >= Q1ACA0) && (Signal <= Q1ACA13))) {

    Address = DDR5_RCD_QACA_OUTPUT_DELAY_REG;

  } else if (((Signal >= Q0BCA0) && (Signal <= Q0BCA13)) || ((Signal >= Q1BCA0) && (Signal <= Q1BCA13))){

    Address = DDR5_RCD_QBCA_OUTPUT_DELAY_REG;

  } else {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_198);
    return MRC_STATUS_FAILURE;
  }

  GetDimmParamGroupLimits (LrbufLevel, QcaDelay, &MinLimit, &MaxLimit, &Delay);

  ReadRcdCwCache (Socket, Channel, SubChannel, Dimm, RDIMM_CW_PAGE0, Address, &QxcaOutputDelay.Data);

  CurVal = QxcaOutputDelay.Bits.output_delay_control_for_qaca_output_signals;

  if (QxcaOutputDelay.Bits.full_cycle_delay == 1) {
    CurVal += 64;
  }

  if (Mode & GSM_READ_ONLY) {

    *Value = CurVal;

  } else {

    if (Mode & GSM_WRITE_OFFSET) {
      CurVal = CurVal + *Value;
    } else {
      CurVal = *Value;
    }

    if (CurVal < MinLimit) {
      RcDebugPrintWithDevice (
        SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
        "%a Out of range!! CurVal = %d, MinLimit = %d\n",
        GetSignalStr (Signal), CurVal, MinLimit
      );
      CurVal = MinLimit;
      Status = MRC_STATUS_FAILURE;
    }
    if (CurVal > MaxLimit) {
      RcDebugPrintWithDevice (
        SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
        "%a Out of range!! CurVal = %d, MaxLimit = %d\n",
        GetSignalStr (Signal), CurVal, MaxLimit
      );
      CurVal = MaxLimit;
      Status = MRC_STATUS_FAILURE;
    }

    ProgramQcaDelay (Host, Socket, Channel, Dimm, SubChannel, Rank, Signal, GSM_FORCE_WRITE, (UINT8)CurVal, TRUE);
  } // Mode == GSM_READ_ONLY

  return Status;
} // GetSetQcaDelay

/*

  This function Gets or Sets the DRAM CA/CS Vref which are the backside QCA/QCS Vref.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number
  @param[in] Strobe           - Strobe number inside the channel. Range from 0 to 19, or ALL_STROBES
  @param[in] Group            - Margin Group. Valid value DramCaVref or DramCsVref 
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

*/
MRC_STATUS
EFIAPI
GetSetDramCaCsVref (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      MRC_GT    Group,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  UINT8               SubChannel;
  UINT16              CurVal = 0;
  INT16               MinLimit;
  INT16               MaxLimit;
  INT16               Delay;
  UINT8               StrobeStart;
  UINT8               StrobeEnd;
  UINT8               MaxStrobeCh;
  UINT8               MaxStrobeSubCh;
  UINT8               MSVx4;
  UINT8               MSVx4SubCh;
  UINT8               MSVx8SubCh;
  UINT8               SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MRC_STATUS          Status = MRC_STATUS_SUCCESS;
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();

  MSVx4 = GetMaxStrobeValid (Host);

  if (Group != DramCaVref && Group != DramCsVref) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
      "Invalid margin group %a:!\n", GetGroupStr (Group));
    return MRC_STATUS_GROUP_NOT_SUPPORTED;
  }

  if (Strobe >= MSVx4 && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
      "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((Strobe < MSVx4 || Strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  MaxStrobeCh = MSVx4;
  MSVx4SubCh = GetSubChMaxStrobeValid (Host);
  MSVx8SubCh = MSVx4SubCh / 2;

  if (IsX4Dimm (Socket, Channel, Dimm)) {
    MaxStrobeSubCh = MSVx4SubCh;
  } else {
    MaxStrobeSubCh = MSVx8SubCh;
  }

  GetDimmParamGroupLimits (LrbufLevel, Group, &MinLimit, &MaxLimit, &Delay);

  if ((Setup->mem.optionsExt & PDA_EN) &&
    ((Host->nvram.mem.socket[Socket].TrainingStepDone.TxVref == 1))) {

    if (Strobe == ALL_STROBES) {
      StrobeStart = 0;
      StrobeEnd = MaxStrobeCh;
    } else {
      StrobeStart = Strobe;
      StrobeEnd = Strobe + 1;
    }

    for (Strobe = StrobeStart; Strobe < StrobeEnd; Strobe++) {
      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }

      if (!VALID_LOGICAL_STROBE (Strobe, MaxStrobeCh, MaxStrobeSubCh)) {
        //
        // For x8 devices, valid logical strobe numbers in the channel are 0-4 and 10-14
        // Skip the other strobes
        //
        continue;
      }

      if (Strobe < MSVx4SubCh) {
        SubChannel = 0;
      } else {
        SubChannel = 1;
      }

      if (Group == DramCaVref) {
        GetSavedDramVrefCa (Socket, Channel, 1 << SubChannel, Dimm, Rank, Strobe % SubChMSVx4, &CurVal);
      } else {
        GetSavedDramVrefCs (Socket, Channel, 1 << SubChannel, Dimm, Rank, Strobe % SubChMSVx4, &CurVal);
      }

      if (Mode & GSM_READ_ONLY) {

        *Value = (INT16)CurVal;

      } else {

        if (Mode & GSM_WRITE_OFFSET) {
          CurVal = CurVal + *Value;
        } else {
          CurVal = *Value;
        }

        if (CurVal < MinLimit) {
          RcDebugPrintWithDevice (
            SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
            "%a Out of range!! CurVal = %d, MinLimit = %d\n",
            GetGroupStr (Group), CurVal, MinLimit
          );
          CurVal = MinLimit;
          Status = MRC_STATUS_FAILURE;
        }
        if (CurVal > MaxLimit) {
          RcDebugPrintWithDevice (
            SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, Strobe, NO_BIT,
            "%a Out of range!! CurVal = %d, MaxLimit = %d\n",
            GetGroupStr (Group), CurVal, MaxLimit
          );
          CurVal = MaxLimit;
          Status = MRC_STATUS_FAILURE;
        }

        WriteMPC (Host, Socket, Channel, Dimm, 1 << SubChannel, Rank, PDA_SELECT_ID | (Strobe % SubChMSVx4), TIMING_4N, TRUE);

        if (Group == DramCaVref) {
          WriteVrefCa (Host, Socket, Channel, Dimm, 1 << SubChannel, Rank, Strobe % SubChMSVx4, CurVal, TIMING_MULTI_CS, TRUE);
        } else {
          WriteVrefCs (Host, Socket, Channel, Dimm, 1 << SubChannel, Rank, Strobe % SubChMSVx4, CurVal, TIMING_MULTI_CS, TRUE);
        }

        WriteMPC (Host, Socket, Channel, Dimm, 1 << SubChannel, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);

      } // Mode == GSM_READ_ONLY
    } // Strobe
  } else {
    //
    // Non PDA mode
    //

    if (Group == DramCaVref) {
      GetSavedDramVrefCa (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, 0, &CurVal);
    } else {
      GetSavedDramVrefCa (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, 0, &CurVal);
    }

    if (Mode & GSM_READ_ONLY) {

      *Value = (INT16)CurVal;

    } else {

      if (Mode & GSM_WRITE_OFFSET) {
        CurVal = CurVal + *Value;
      } else {
        CurVal = *Value;
      }

      if (CurVal < MinLimit) {
        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "%a Out of range!! CurVal = %d, MinLimit = %d\n",
          GetGroupStr (Group), CurVal, MinLimit
        );
        CurVal = MinLimit;
        Status = MRC_STATUS_FAILURE;
      }
      if (CurVal > MaxLimit) {
        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
          "%a Out of range!! CurVal = %d, MaxLimit = %d\n",
          GetGroupStr (Group), CurVal, MaxLimit
        );
        CurVal = MaxLimit;
        Status = MRC_STATUS_FAILURE;
      }

      WriteMPC (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank, PDA_SELECT_ID | PDA_SELECT_ALL, TIMING_4N, TRUE);

      if (Group == DramCaVref) {
        WriteVrefCa (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank, ALL_STROBES, CurVal, TIMING_MULTI_CS, TRUE);
      } else {
        WriteVrefCs (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank, ALL_STROBES, CurVal, TIMING_MULTI_CS, TRUE);
      }

      WriteMPC (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);

    } // Mode == GSM_READ_ONLY

  }

  return Status;
} // GetSetDramCaVref
/**

  Get/Set DDR5 QCSx Delay of all copies (A and B)
  QCSx is the CSx Signal from the RCD to the DRAM
  Programs QCS delay depending on the Raw Card detected
  This function shall program A, both A and B copies if applicable based on the Dimm type.

  @param[in]      Host         - Pointer to SysHost
  @param[in]      Socket       - Socket number
  @param[in]      Channel      - Channel number
  @param[in]      SubChannel   - SubChannel number
  @param[in]      Dimm         - DIMM number within the channel
  @param[in]      Rank         - DIMM Rank number
  @param[in]      Mode         - Access Mode
  @param[in,out]  Delay        - Pointer to Delay value
  @param[in,out]  MinVal       - Pointer to a memory location that keep track of the minimum value of all copies
  @param[in,out]  MaxVal       - Pointer to a memory location that keep track of the maximum value of all copies
                                 The MinVal and MaxVal can be used by the MemSweep lib to keep track the delta of
                                 the initial trained values of both copies. It helps the MemSweep lib to adjust the
                                 sweep range.

  @retval   - MRC_STATUS_SUCCESS if command completed successfully
  @retval   - !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
EFIAPI
GetSetQcsAllCopiesDelay (
  IN      PSYSHOST        Host,
  IN      UINT8           Socket,
  IN      UINT8           Channel,
  IN      UINT8           SubChannel,
  IN      UINT8           Dimm,
  IN      UINT8           Rank,
  IN      UINT8           Mode,
  IN OUT  INT16           *Delay,
  IN OUT  INT16           *MinVal,  OPTIONAL
  IN OUT  INT16           *MaxVal   OPTIONAL
  )
{
  GSM_CSN         Signal;
  UINT8           BacksideCopyNum;
  INT16           MinValue;
  INT16           MaxValue;
  INT16           Value;
  UINT8           CopyIndex;
  MRC_STATUS      Status = MRC_STATUS_SUCCESS;

  Signal = GetBacksideCsSignal (Dimm, Rank, SubChannel);
  BacksideCopyNum = GetMaxBacksideCopyNum (Socket, Channel, Dimm);

  //
  // Return the mininum value of all copies when read
  //
  if (Mode & GSM_READ_ONLY) {
    MinValue = MAX_INT16;
    MaxValue = MIN_INT16;

    for (CopyIndex = 0; CopyIndex < BacksideCopyNum; CopyIndex++) {

      GetSetQcsDelay (Host, Socket, Channel, Dimm, Rank, Signal, CopyIndex, Mode, &Value, NULL, NULL);

      if (Value < MinValue) {
        MinValue = Value;
      }

      if (Value > MaxValue) {
        MaxValue = Value;
      }
    }

    *Delay = MinValue;

    if (MinVal != NULL) {
      *MinVal = MinValue;
    }

    if (MaxVal != NULL) {
      *MaxVal = MaxValue;
    }

  } else {

    for (CopyIndex = 0; CopyIndex < BacksideCopyNum; CopyIndex++) {

      if (GetSetQcsDelay (Host, Socket, Channel, Dimm, Rank, Signal, CopyIndex, Mode, Delay, NULL, NULL) != MRC_STATUS_SUCCESS) {
        Status = MRC_STATUS_FAILURE;
      }
    }
  } // GSM_READ_ONLY

  return Status;
} // GetSetQcsAllCopiesDelay

/**

  Programs CAS Latency for DDR devices

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Ch number

  @retval N/A

**/
VOID
ProgramCasLDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT8 Dimm;
  UINT8 Rank;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];

  if ((UbiosGenerationOrHsleEnabled() == TRUE) || (Host->var.mem.socket[Socket].TrainingStatus.Bits.QcaUcaTrainingDone == 1)) {
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList) [Dimm].dimmPresent == 0) {
        continue;
      }
      for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        if ((*DimmNvList) [Dimm].DcpmmPresent == 0) {
          ProgramMr0Ddr5 (Host, Socket, Ch, Dimm, Rank);
        }
#ifdef DDRT_SUPPORT
        else if ((*DimmNvList) [Dimm].DcpmmPresent == 1 && (*DimmNvList) [Dimm].fmcType == FMC_CWV_TYPE) {
          ProgramMr0Ddrt2 (Host, Socket, Ch, Dimm, Rank);
        }
#endif
      }
    }
  }
} // ProgramClDdr5

/**

  Determine if Backside CMD Margin is enabled for DDR5

  @param  VOID

  @retval TRUE  - Backside CMD Margin is enabled
  @retval FALSE - Backside CMD Margin is not enabled

**/
BOOLEAN
IsDdr5BacksideCmdMarginEnabled (
  VOID
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if ((Setup->mem.enableBacksideRMT) && (Setup->mem.enableBacksideCMDRMT)) {
    return TRUE;
  }
  return FALSE;
}  // IsDdr5BacksideCmdMarginEnabled

/**
  Programs CAS Latency for DDR5 ranks

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Ch number
  @param[in] Dimm    - Dimm number (zero-based)
  @param[in] Rank    - Rank number (zero-based)

  @retval N/A

**/
VOID
ProgramMr0Ddr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  struct channelNvram (*ChannelNvList)[MAX_CH];
  DDR5_MODE_REGISTER_0_STRUCT Mr0;

  ChannelNvList = GetChannelNvList (Host, Socket);
  //
  // Program the CL
  //
  Mr0.Data = 0;
  ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_0_REG, &Mr0.Data);
  Mr0.Bits.cas_latency_rl = DDR5_MR0_CAS_LATENCY_RL ((*ChannelNvList)[Ch].common.nCL);

  if ((!IsBrsPresent (Host, Socket)) && (FeaturePcdGet (PcdCteBuild) == TRUE)) {
    Mr0.Bits.cas_latency_rl = DDR5_MR0_CAS_LATENCY_RL_22;
  }

  DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_0_REG, Mr0.Data);
}

/**
Enables Write Leveling Mode for DDR5 ranks

  @param[in] Host              - Pointer to sysHost
  @param[in] Socket            - Socket Id
  @param[in] Ch                - Ch number
  @param[in] Dimm              - Dimm number (zero-based)
  @param[in] Rank              - Rank number (zero-based)
  @param[in] Mode              - Write Leveling Mode
  @retval N/A

**/
VOID
ConfigureDramForWriteLeveling (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Mode
)
{
  DDR5_MODE_REGISTER_2_STRUCT Mr2;

  ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, &Mr2.Data);

  switch (Mode) {
    case EXTERNAL_WRITE_LEVELING_MODE:
      //
      // Program External Write Leveling
      //
      Mr2.Bits.write_leveling = 1;
      break;

    case INTERNAL_WRITE_LEVELING_MODE:
      //
      // Program Internal Write Leveling
      //
      Mr2.Bits.internal_write_timing = 1;
      break;

    case DISABLE_WRITE_LEVELING_MODE:
      //
      // Clear External Write Leveling
      // Leave Internal Write Leveling enabled for power savings
      //
      Mr2.Bits.write_leveling = 0;
      break;

    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_100);
      return;
  }

  DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, Mr2.Data);
}

/*
  Convert a given Tx DQ VREF percentage value to a linear Tx DQ VREF value
  Where:
  Percentage(%)     Linear
      45              0
      45.5            1
      50              2
      50.5            3
      ...
      96.5            103
      97              104
      97.5            105

  @param[in]      TxVrefPercentage      Tx VREF percentage (45% - 97.5%) represented with a fixed point of 1 decimal (multiplied by 10).
                                        45.0% is represented as 450
                                        45.5% is represented as 455

  @return         Linear value from percentage

*/
INT16
ConvertPercentageToLinear (
  IN        UINT16     TxVrefPercentage
  )
{
  INT16   Linear = 0;

  TxVrefPercentage = (TxVrefPercentage - MIN_VREFDQ_PERCENT_DDR5) / VREFDQ_INCREMENT_DDR5;

  Linear = TxVrefPercentage;
  if (Linear < 0) {
    Linear = 0;
  } else if (Linear > DDR5_MAX_VREF_RANGE) {
    Linear = (UINT16) DDR5_MAX_VREF_RANGE;
  }

  return Linear;
}

/**

  This function returns the value of Ddr5 tXP

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel number

  @retval UINT32 Value of Ddr5 tXP

**/
UINT32
GettXPDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  UINT32  tXPLocal;

  //
  // DDR5 tXP should be max(7.5ns, 8nCK)
  //
  tXPLocal = MAX (PicosecondToClocks (Socket, tXP_PS), tXP_MIN_DCLK);

  //
  // Must be at least 3 clocks
  //
  if (tXPLocal < 3) {
    tXPLocal = 3;
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tXP, &tXPLocal);

  return tXPLocal;
} // GettXPDdr5


/**

  This function returns the value of tSRX2SRX.  The minimum number of valid
  input clock cycles between SRX command to release QCS and SRX to take
  DRAM out of Self Refresh (DDR5 JEDEC RCD Specification)

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tSRX2SRX in clocks

**/
UINT32
EFIAPI
GettCKSRX2SRX (
  IN UINT8    Socket
  )
{
  UINT32 TCkSrx2Srx;

  //
  // DDR5 tCKSRKX should be max(10ns, 16nCK)
  //
  TCkSrx2Srx = MAX (NanosecondToClocks (Socket, tCKSRKX2SRX_MIN_NS), tCKSRKX2SRX_MIN_DCLK);

  return TCkSrx2Srx;
} // GettCKSRX2SRX

/**

  This function returns the value of tCKACT.
  Maximum time from DCS_n transition HIGH to DCK start
  (DDR5 JEDEC RCD Specification)

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCKACT in clocks

**/
UINT32
EFIAPI
GettCKACT (
  IN UINT8    Socket
  )
{
  UINT32 TCkAct;

  //
  // DDR5 TCkAct should be 16nCK
  //
  TCkAct = tCKACT_MAX_DCLK;

  return TCkAct;
} // GettCKACT

/**

  This function returns the value of tCSSR.
  Minimum DCS[1:0]_n LOW time (after SRE) when entering DCK stop
  (DDR5 JEDEC RCD Specification)

  @param[in] Socket  - Socket Id

  @retval UINT32 Value of tCSSR in clocks

**/
UINT32
EFIAPI
GettCSSR (
  IN UINT8    Socket
  )

{
  UINT32 Tcssr;

  //
  // DDR5 tCSSR should be max(15ns, 24nCK)
  //
  Tcssr = MAX (NanosecondToClocks (Socket, tCSSR_MIN_NS), tCSSR_MIN_DCLK);

  return Tcssr;
} // GettCSSR

/*
  Convert a given Tx DQ VREF physical value to percentage and then finally to a linear Tx DQ VREF value
  Where:
  Physical    Percentage(%)   Linear
    0x7D          35            0
    0x7C          35.5          1
    0x7B          36            2
    0x7A          36.5          3
    ...
    0x02          96.5          123
    0x01          97            124
    0x00          97.5          125

  @param[in]      Physical      Tx VREF percentage (0x00 - 0x7D)

  @return         Linear value from physical

*/
INT16
EFIAPI
ConvertPhysicalToLinear (
  IN        INT16   Physical
  )
{
  INT16   Percentage;

  if (Physical < 0) {
    Physical = 0;
  }
  if (Physical > DDR5_MAX_VREF_RANGE) {
    Physical = DDR5_MAX_VREF_RANGE;
  }

  Percentage = MAX_VREFDQ_PERCENT_DDR5 - (Physical * VREFDQ_INCREMENT_DDR5);

  return ConvertPercentageToLinear (Percentage);

}

/*
  Convert a given Tx DQ VREF linear value to physical Tx DQ VREF value
  Where:
  Linear        Physical
    0             0x7D
    1             0x7C
    2             0x7B
    3             0x7A
    ...           ...
    123           0x02
    124           0x01
    125           0x00

  @param[in]      Linear      Value between (0-125) that represents an specific VREF DQ percentage

  @return         Physical value from linear

*/
INT16
EFIAPI
ConvertLinearToPhysical (
  IN        INT16   Linear
  )
{
  if (Linear < 0) {
    Linear = 0;
  }
  if (Linear > DDR5_MAX_VREF_RANGE) {
    Linear = DDR5_MAX_VREF_RANGE;
  }

  return (DDR5_MAX_VREF_RANGE - Linear);
}

/**
  Apply WA1 (Internal Timing) for Rambus, if needed

  @param[in]  Host      - Pointer to SysHost
  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number in the socket
  @param[in]  Dimm      - Dimm number in the channel
  @param[in]  Rank      - Rank number in the DIMM

  @retval N/A
**/
VOID
RambusRevC1Wa1 (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN UINT8      Rank
  )
{
  UINT32            SmbData;
  struct dimmNvram  (*DimmNvList) [MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  if (((*DimmNvList)[Dimm].SPDRegVen == MFGID_RAMBUS && Rank == 0) && ((*DimmNvList)[Dimm].SPDRegRev == RAMBUS_SPD_REGREV_C1)) {

    RcDebugPrintWithDevice(
      SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Applying WA1 (Internal Timing) for Rambus\n");

    SmbData = 0xA0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x60, &SmbData);
    SmbData = 0x90;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x60, &SmbData);

    SmbData = 0xB8;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x61, &SmbData);
    SmbData = 0xC0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x61, &SmbData);

    SmbData = 0xB2;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x62, &SmbData);
    SmbData = 0xC0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x62, &SmbData);

    SmbData = 0xC0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x63, &SmbData);
    SmbData = 0xB8;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x63, &SmbData);

    SmbData = 0xA0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x64, &SmbData);
    SmbData = 0xA0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x64, &SmbData);

    SmbData = 0xA0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x65, &SmbData);
    SmbData = 0xB0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x65, &SmbData);

    SmbData = 0xB8;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x68, &SmbData);
    SmbData = 0xB0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x68, &SmbData);

    SmbData = 0xB0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x69, &SmbData);
    SmbData = 0xA8;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x69, &SmbData);

    SmbData = 0xB0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x6A, &SmbData);
    SmbData = 0xB0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x6A, &SmbData);

    SmbData = 0xB0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x6B, &SmbData);
    SmbData = 0xB0;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x6B, &SmbData);

    if (IsLrdimmPresent(Socket, Ch, Dimm)) {
      SmbData = 0x8A;
      WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x66, &SmbData);
      SmbData = 0x09;
      WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x66, &SmbData);

      SmbData = 0x10;
      WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA6, 0x67, &SmbData);
      WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA6, 0x67, &SmbData);
    }

    RcDebugPrintWithDevice(
      SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Applying WA3 (Enable I2C HID) for Rambus\n");

    SmbData = 0x0F;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0xA3, 0x6B, &SmbData);
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0xA3, 0x6B, &SmbData);

    RcDebugPrintWithDevice(
      SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Applying WA2  for Rambus\n");
    RcDebugPrintWithDevice(
      SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "RW1B=RW1C=0X80\n");
    SmbData = 0x80;
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0, RDIMM_RW1B, &SmbData);
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0, RDIMM_RW1B, &SmbData);
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_A_MASK, BYTE_WRITE, 0, RDIMM_RW1C, &SmbData);
    WriteRcdSmbDdr5(Host, Socket, Ch, Dimm, SUB_CH_B_MASK, BYTE_WRITE, 0, RDIMM_RW1C, &SmbData);
  }
}

/**
  Perform DRAM ODT RTT initialization using MPC

  @param[in]  Host      - Pointer to SysHost
  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number in the socket
  @param[in]  Dimm      - Dimm number in the channel
  @param[in]  SubChMask - Mask of subchannels
  @param[in]  Rank      - Rank number in the DIMM

  @retval N/A
**/
VOID
DramMpcOdtRttInit (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN UINT8      SubChMask,
  IN UINT8      Rank
  )
{
  UINT8            RttWriteVal, RttParkVal;
  UINT16           RttNomRdVal, RttNomWrVal, DqsRttParkVal;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  struct ddrRank   (*RankList)[MAX_RANK_DIMM];
  UINT16           QcaOdtValue = 0;
  UINT16           QckOdtValue = 0;
  UINT16           QcsOdtValue = 0;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);

  EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

  // Group A ODT settings may be different per DRAM, but we cannot program individual DRAMs
  // until after PDA enumerate.  Therefore programming a default value here for Group A
  // but will update these values after PDA enumeration.

  // MPC command with Opcode 0x0020, Group A Rtt_CK OP[2:0] = MR32[2:0], default to 0

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, GROUP_A_RTT_CK, TIMING_4N, TRUE);

  // MPC command with Opcode 0x0030, Group A Rtt_CS OP[2:0] = MR32[5:3], default to 0

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, GROUP_A_RTT_CS, TIMING_4N, TRUE);

  // MPC command with Opcode 0x0040, Group A Rtt_CA OP[2:0] = MR33[2:0], default to 0

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, GROUP_A_RTT_CA, TIMING_4N, TRUE);

  // Group B ODT settings are just in 1 position that corresponds to a strobe value of 0
  // Because all Group B strapped DRAMs in a DIMM are the same, we can just program the
  // whole rank with the group B value

  GetDramQckValueOdt (Socket, Ch, Dimm, 0, &QckOdtValue);

  // MPC command with Opcode 0x002F SubChannelMask 0x3 // Group B Rtt_CK OP[2:0] = MR32[2:0]

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, GROUP_B_RTT_CK | QckOdtValue, TIMING_4N, TRUE);

  GetDramQcsValueOdt (Socket, Ch, Dimm, 0, &QcsOdtValue);

  // MPC command with Opcode 0x003F SubChannelMask 0x3 // Group B Rtt_CS OP[2:0] = MR32[5:3]

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, GROUP_B_RTT_CS | QcsOdtValue, TIMING_4N, TRUE);

  GetDramQcaValueOdt (Socket, Ch, Dimm, 0, &QcaOdtValue);

  // MPC command with Opcode 0x004C SubChannelMask 0x3 // Group B Rtt_CA OP[2:0] = MR33[2:0]

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, GROUP_B_RTT_CA | QcaOdtValue, TIMING_4N, TRUE);

  GetRttValues (Host, Socket, Ch, Dimm, Rank, &RttWriteVal, &RttParkVal, &RttNomRdVal, &RttNomWrVal, &DqsRttParkVal);
  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, DQS_RTT_PARK | (DqsRttParkVal & 0x7), TIMING_4N, TRUE);
  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, RTT_PARK | (RttParkVal & 0x7), TIMING_4N, TRUE);
  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);

  ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

}


/**
  Perform DRAM ODT RTT initialization using MPC for values that
  are per DRAM, so must be called after PDA Enumerate

  @param[in]  Host      - Pointer to SysHost
  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number in the socket
  @param[in]  Dimm      - Dimm number in the channel
  @param[in]  SubChMask - Mask of subchannels
  @param[in]  Rank      - Rank number in the DIMM

  @retval N/A
**/
VOID
DramPerStrobeMpcOdtRttInit (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN UINT8      SubChMask,
  IN UINT8      Rank
  )
{
  struct ddrRank   (*RankList)[MAX_RANK_DIMM];
  UINT16           QcaOdtValue = 0;
  UINT16           QckOdtValue = 0;
  UINT16           QcsOdtValue = 0;
  UINT8            Strobe = 0;
  UINT8            SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8            SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  UINT8            MaxStrobe = GetMaxStrobeValid (Host);
  UINT8            SubChStrobe = 0;
  UINT8            SubChannel = 0;
  UINT8            GroupXRttCk;
  UINT8            GroupXRttCs;
  UINT8            GroupXRttCa;

  RankList = GetRankNvList (Host, Socket, Ch, Dimm);

  EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

  //Set Per Strobe backside ODT settings

  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }

    SubChStrobe = Strobe % SubChMSVx4;
    if (!(IsX4Dimm (Socket, Ch, Dimm))) {
      if (SubChStrobe >= SubChMSVx8) {
        continue;
      }
    }
    SubChannel = Strobe / SubChMSVx4;

    if (((1 << SubChannel) & (SubChMask)) == 0) {
      continue;
    }

    WriteMPC (Host, Socket, Ch, Dimm, 1 << SubChannel, Rank, PDA_SELECT_ID | SubChStrobe, TIMING_4N, TRUE);

    if (GetPositionFromStrobe (Strobe) == STRAP_POSITION) {
      GroupXRttCk = GROUP_B_RTT_CK;
      GroupXRttCs = GROUP_B_RTT_CS;
      GroupXRttCa = GROUP_B_RTT_CA;
    } else {
      GroupXRttCk = GROUP_A_RTT_CK;
      GroupXRttCs = GROUP_A_RTT_CS;
      GroupXRttCa = GROUP_A_RTT_CA;
    }

    GetDramQckValueOdt (Socket, Ch, Dimm, Strobe, &QckOdtValue);

    // MPC command with Opcode 0x0020, Group A Rtt_CK OP[2:0] = MR32[2:0]

    WriteMPC (Host, Socket, Ch, Dimm, 1 << SubChannel, Rank, GroupXRttCk | QckOdtValue, TIMING_4N, TRUE);

    GetDramQcsValueOdt (Socket, Ch, Dimm, Strobe, &QcsOdtValue);

    // MPC command with Opcode 0x0030, Group A Rtt_CS OP[2:0] = MR32[5:3]

    WriteMPC (Host, Socket, Ch, Dimm, 1 << SubChannel, Rank, GroupXRttCs | QcsOdtValue, TIMING_4N, TRUE);

    GetDramQcaValueOdt (Socket, Ch, Dimm, Strobe, &QcaOdtValue);

    // MPC command with Opcode 0x0040, Group A Rtt_CA OP[2:0] = MR33[2:0]

    WriteMPC (Host, Socket, Ch, Dimm, 1 << SubChannel, Rank, GroupXRttCa | QcaOdtValue, TIMING_4N, TRUE);

  } //Strobe

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, PDA_SELECT_ID | PDA_SELECT_ALL, TIMING_4N, TRUE);

  // MPC command with Opcode 0x001F SubChannelMask 0x3 // Apply settings for CA Vref and Rtt_Ca,Cs,Ck

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);

  ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

}

/**
  Perform DRAM MPC initialization

  @param[in]  Host      - Pointer to SysHost
  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number in the socket
  @param[in]  Dimm      - Dimm number in the channel
  @param[in]  SubChMask - Mask of subchannels
  @param[in]  Rank      - Rank number in the DIMM

  @retval N/A
**/
VOID
BacksideMpcInit (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN UINT8      SubChMask,
  IN UINT8      Rank
  )
{

  DramMpcOdtRttInit (Host, Socket, Ch, Dimm, SubChMask, Rank);

  EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, ZQCAL_START, TIMING_4N, TRUE);
  FixedDelayMicroSecond (2); // delay at least tZQCAL (== 1 us)

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, ZQCAL_LATCH, TIMING_4N, TRUE);
  FixedDelayMicroSecond (1); // delay at least tZQLAT (< 1 us)

  WriteMPC (Host, Socket, Ch, Dimm, SubChMask, Rank, DLL_RESET, TIMING_4N, TRUE);
  FixedDelayMicroSecond (1);

  ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

}

/**

  Start backside dram init.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to initialize
  @param[in] ChannelMask      - Bitmask of channels to init (1 in a bit position = init that channel number)

  @retval SUCCESS

**/
UINT32
BacksideDramInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask
  )
{
  UINT8   Dimm = 0;
  UINT8   Rank = 0;
  UINT8   Ch = 0;
  UINT8   NumRanks;
  INT8    QcaVrefValue;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "BacksideDramInit starts\n");

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {

      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {

        if ((IsChannelEnabled (Socket, Ch) == FALSE) || (((1 << Ch) & ChannelMask) == 0)) {
          continue;
        }

        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }

        if (IsDcpmmPresentDimm (Socket, Ch, Dimm)) {
          continue;
        }

        NumRanks = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);

        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= NumRanks)) {
          continue;
        }

        /*
        N0: BacksideDramInit starts
        N0.C00.D0.R0: MRW command: Address 0x00, CW 1, Data 0x04, SubChMask 1
        N0.C00.D0.R0: Vref CA command with Opcode QCA_VREF_INIT_VALUE
        BacksideMpcInit ()
        N0.C00.D0.R1: MRW command: Address 0x00, CW 1, Data 0x00, SubChMask 1
        N0: BacksideDramInit ends
        */

        EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

        GetQcaVrefValueOdt (Socket, Ch, Dimm, &QcaVrefValue);
        WriteVrefCa (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, ALL_STROBES, QcaVrefValue, TIMING_MULTI_CS, TRUE);

        BacksideMpcInit (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank);

        ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

        RambusRevC1Wa1 (Host, Socket, Ch, Dimm, Rank);
      } //Ch
    } //Rank loop
  }//Dimm loop
  //
  // tSTABx
  //
  TrainingDelay (Host, 100, 2);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "BacksideDramInit ends\n");

  return SUCCESS;
} // BacksideDramInit

/**

  Get the DRAM preamble and post amble MR8 value.

  @param Host    - Pointer to sysHost
  @param Socket  - Socket number.

  @retval Preamble value

**/
UINT8
GetPreamblePostambleMr8Value (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  DDR5_MODE_REGISTER_8_STRUCT Mr8;
  UINT8 PreambleLen  = 0;
  UINT8 PostambleLen = 0;
  Mr8.Data = 0;

  if (IsBrsPresent (Host, Socket)) {
    Mr8.Bits.read_preamble_settings = DDR5_DRAM_READ_PREAMBLE_1TCK;
    Mr8.Bits.write_preamble_settings = DDR5_DRAM_WRITE_PREAMBLE_3TCK;
    return Mr8.Data;
  }
  PreambleLen = GetPreambleState (Socket, READ_PREAMBLE);
  switch (PreambleLen) {
    case PREAMBLE_1TCLK:
      Mr8.Bits.read_preamble_settings = DDR5_DRAM_READ_PREAMBLE_1TCK;
      break;
    case PREAMBLE_2TCLK:
      //
      // No DDR4 style in SPR.
      //
      Mr8.Bits.read_preamble_settings = DDR5_DRAM_READ_PREAMBLE_2TCK_0010;
      break;
    case PREAMBLE_3TCLK:
      Mr8.Bits.read_preamble_settings = DDR5_DRAM_READ_PREAMBLE_3TCK;
      break;
    case PREAMBLE_4TCLK:
      Mr8.Bits.read_preamble_settings = DDR5_DRAM_READ_PREAMBLE_4TCK;
      break;
    default:
      Mr8.Bits.read_preamble_settings = DDR5_DRAM_READ_PREAMBLE_2TCK_0010;
      break;
  }

  PreambleLen = GetPreambleState (Socket, WRITE_PREAMBLE);
  switch (PreambleLen) {
    case PREAMBLE_2TCLK:
      Mr8.Bits.write_preamble_settings = DDR5_DRAM_WRITE_PREAMBLE_2TCK;
      break;
    case PREAMBLE_3TCLK:
      Mr8.Bits.write_preamble_settings = DDR5_DRAM_WRITE_PREAMBLE_3TCK;
      break;
    case PREAMBLE_4TCLK:
      Mr8.Bits.write_preamble_settings = DDR5_DRAM_WRITE_PREAMBLE_4TCK;
      break;
    default:
      Mr8.Bits.write_preamble_settings = DDR5_DRAM_WRITE_PREAMBLE_2TCK;
      break;
  }

  PostambleLen = GetPostambleState (Socket, READ_POSTAMBLE);
  switch (PostambleLen) {
    case POSTAMBLE_0P5TCLK:
      Mr8.Bits.read_postamble_settings = DDR5_DRAM_READ_POSTAMBLE_0P5TCK;
      break;
    case POSTAMBLE_1P5TCLK:
      Mr8.Bits.read_postamble_settings = DDR5_DRAM_READ_POSTAMBLE_1P5TCK;
      break;
    default:
      Mr8.Bits.read_postamble_settings = DDR5_DRAM_READ_POSTAMBLE_0P5TCK;
      break;
  }

  PostambleLen = GetPostambleState (Socket, WRITE_POSTAMBLE);
  switch (PostambleLen) {
    case POSTAMBLE_0P5TCLK:
      Mr8.Bits.write_postamble_settings = DDR5_DRAM_WRITE_POSTAMBLE_0P5TCK;
      break;
    case POSTAMBLE_1P5TCLK:
      Mr8.Bits.write_postamble_settings = DDR5_DRAM_WRITE_POSTAMBLE_1P5TCK;
      break;
    default:
      Mr8.Bits.write_postamble_settings = DDR5_DRAM_WRITE_POSTAMBLE_0P5TCK;
      break;
  }

  return Mr8.Data;
} // GetPreamblePostambleMr8Value

//
// Table used to convert tRTP MR6 field encoding
// Index in the table corresponds to the encoding
//
UINT8 Ddr5TrtpMr6Encoding[] = {
  12,  //0000b
  14,  //0001b
  15,  //0010b
  17,  //0011b
  18,  //0100b
  20,  //0101b
  21,  //0110b
  23,  //0111b
  24   //1000b
};

//Per DDR5 JEDEC Spec, tRTP is currently defined as 7.5ns across
//all speed bins.  7.5ns is 7500ps
#define TRTP_IN_PICOSECONDS 7500

/**

  Get tRTP Jedec encoded value as defined in DDR5, MR6 register definition

  @param[in] Socket  - Socket to retrieve value from

  @retval Encoded tRTP value

**/
UINT8
GetDdr5EncodedTRTP (
  IN  UINT8  Socket
  )
{
  UINT16              RtpInClocks;
  UINT8               Index;
  UINT8               EncodedTrtp = DDR5_MR6_TRTP_24NCK; //largest supported value, default if not found in table

  RtpInClocks =  PicosecondToClocks (Socket, TRTP_IN_PICOSECONDS);

  for (Index = 0; Index < ARRAY_SIZE (Ddr5TrtpMr6Encoding); Index++) {
    if (Ddr5TrtpMr6Encoding[Index] >= RtpInClocks) {
      // The Index in the table corresponds to the encoded value (i.e. 0 is 12nCK, 1 is 14nCK, ...)
      EncodedTrtp = Index;
      break;
    }
  }

  return EncodedTrtp;
}

//
// Table used to convert tWR MR6 field encoding (Write Recovery Time)
// Index in the table corresponds to the encoding
//
UINT8 Ddr5TwrMr6Encoding[] = {
  48,  //0000b
  54,  //0001b
  60,  //0010b
  66,  //0011b
  72,  //0100b
  78,  //0101b
  84,  //0110b
  90,  //0111b
  96   //1000b
};
/**

  Get tWR Jedec encoded value as defined in DDR5, MR6 register definition

  @param[in] Socket  - Socket to retrieve value from
  @param[in] Ch      - Channel to retrieve value from

  @retval Encoded tWR value

**/
UINT8
GetDdr5EncodedTWR (
  IN  UINT8  Socket,
  IN  UINT8  Ch
  )
{
  PSYSHOST            Host;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               Index;
  UINT8               EncodedTwr = DDR5_MR6_WRITE_RECOVERY_TIME_96NCK; //largest supported value, default if not found in table

  Host = (PSYSHOST) GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Index = 0; Index < ARRAY_SIZE (Ddr5TrtpMr6Encoding); Index++) {
    if (Ddr5TwrMr6Encoding[Index] >= (*ChannelNvList)[Ch].common.nWR) {
      // The Index in the table corresponds to the encoded value (i.e. 0 is 48nCK, 1 is 54nCK, ...)
      EncodedTwr = Index;
      break;
    }
  }

  return EncodedTwr;
}

/**

  Returns the RTT ODT to be programmed to the DRAM.

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number
  @param[in] Dimm         - DIMM number
  @param[in] Rank         - Rank number
  @param[out] RttWriteVal - Pointer to store RTT Write value
  @param[out] RttParkVal  - Pointer to store RTT Park value
  @param[out] RttNomRdVal - Pointer to store RTT Nom Read value
  @param[out] RttNomWrVal - Pointer to store RTT Nom Write value
  @param[out] DqsRttParkVal - Pointer to store Dqs RTT Park value

  @retval N/A

**/
VOID
GetRttValues (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  OUT UINT8     *RttWriteVal,
  OUT UINT8     *RttParkVal,
  OUT UINT16    *RttNomRdVal,
  OUT UINT16    *RttNomWrVal,
  OUT UINT16    *DqsRttParkVal
  )
{
  struct ddrRank                (*RankList) [MAX_RANK_DIMM];
  struct dimmNvram              (*DimmNvList) [MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  // If no LRDIMM present, pull values from frontside ODT table
  if (IsLrdimmPresent (Socket, Ch, Dimm) == FALSE) {
    RankList = GetRankNvList (Host, Socket, Ch, Dimm);

    *RttWriteVal = (UINT8) (*RankList)[Rank].RttWr;
    *RttParkVal = (UINT8) (*RankList)[Rank].RttPrk;
    *RttNomRdVal = (*RankList)[Rank].RttNomRd;
    *RttNomWrVal = (*RankList)[Rank].RttNomWr;
    *DqsRttParkVal = (*RankList)[Rank].DqsRttPrk;

  } else {
    if ((*DimmNvList)[Dimm].numRanks != 1) {
      *RttWriteVal = DDR5_RTT_240;
      *RttParkVal  = DDR5_RTT_60;
      *RttNomRdVal = DDR5_RTT_OFF;
      *RttNomWrVal = DDR5_RTT_OFF;
      *DqsRttParkVal = DDR5_RTT_60;
    } else {
      *RttWriteVal = DDR5_RTT_60;
      *RttParkVal  = DDR5_RTT_NA;
      *RttNomRdVal = DDR5_RTT_NA;
      *RttNomWrVal = DDR5_RTT_NA;
      *DqsRttParkVal = DDR5_RTT_60;
    }
  }
}

/**

  Start backside dram init after QCA.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket to initialize
  @param[in] ChannelMask      - Bitmask of channels to init (1 in a bit position = init that channel number)

  @retval SUCCESS

**/
UINT32
BacksideDramInitAfterQca (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT32    ChannelMask
  )
{
  UINT8                         Dimm = 0;
  UINT8                         Rank = 0;
  UINT8                         Ch = 0;
  struct channelNvram           (*ChannelNvList)[MAX_CH];
  struct dimmNvram              (*DimmNvList)[MAX_DIMM];
  struct ddrRank                (*RankList)[MAX_RANK_DIMM];
  struct rankDevice             (*RankStruct)[MAX_RANK_DIMM];
  UINT8                         Ddr5RttLookup[] = {0, 240, 120, 80, 60, 48, 40, 34};
#ifndef SPR_PO_FLAG
#ifdef USE_LATEST_DRAM_SPEC
  DDR5_MODE_REGISTER_2_STRUCT   Mr2;
#else // #ifdef USE_LATEST_DRAM_SPEC
  DDR5_MODE_REGISTER_3_STRUCT   Mr3;
#endif // #ifdef USE_LATEST_DRAM_SPEC
#endif //
  DDR5_MODE_REGISTER_5_STRUCT   Mr5;
  DDR5_MODE_REGISTER_6_STRUCT   Mr6;
  DDR5_MODE_REGISTER_8_STRUCT   Mr8;
  DDR5_MODE_REGISTER_9_STRUCT   Mr9;
  DDR5_MODE_REGISTER_10_STRUCT  Mr10;
  DDR5_MODE_REGISTER_34_STRUCT  Mr34;
  DDR5_MODE_REGISTER_35_STRUCT  Mr35;
  DDR5_MODE_REGISTER_37_STRUCT  Mr37;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT  RcdParityAlert;
  UINT8                         RttWriteVal;
  UINT8                         RttParkVal;
  UINT8                         DramOrDbHostRon;
  UINT16                        RttNomRdVal;
  UINT16                        RttNomWrVal;
  UINT16                        DqsRttParkVal;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "BacksideDramInitAfterQca starts\n");

  //
  // Initialize structures to write during the loop.
  //

  Mr8.Data = GetPreamblePostambleMr8Value (Host, Socket);

  Mr9.Data = 0;

  Mr37.Data = 0;  // make sure all RFU bits are 0
  Mr37.Bits.odtlon_wr_offset = INITIAL_ODTLON_WR_OFFSET;
  Mr37.Bits.odtloff_wr_offset = INITIAL_ODTLOFF_WR_OFFSET;

  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      for (Ch = 0; Ch < MAX_CH; Ch += GetChInc (Host)) {
        if (((*ChannelNvList)[Ch].enabled == 0) || (((1 << Ch) & ChannelMask) == 0)) {
          continue;
        }

        Mr5.Data = 0;
        if (IsX4Dimm (Socket, Ch, Dimm)) {
          Mr5.Bits.tdqs_enable = 0;
        } else {
          Mr5.Bits.tdqs_enable = 1;
        }

        if (IsLrdimmPresent (Socket, Ch, Dimm)) {
          Mr5.Bits.pull_up_output_driver_impedance = DDR5_DRAM_RON_34_BITS;
          Mr5.Bits.pull_down_output_driver_impedance = DDR5_DRAM_RON_34_BITS;
        } else {
          GetDramOrDbHostRon (Socket, Ch, Dimm, &DramOrDbHostRon);
          Mr5.Bits.pull_up_output_driver_impedance = (UINT8) ConvertOdtValueDdr5 (DramRonPullUp, DramOrDbHostRon, ENCODE_RTT);
          Mr5.Bits.pull_down_output_driver_impedance = (UINT8) ConvertOdtValueDdr5 (DramRonPullDown, DramOrDbHostRon, ENCODE_RTT);
        }

        Mr6.Data = 0;
        Mr6.Bits.write_recovery_time = GetDdr5EncodedTWR (Socket, Ch);
        Mr6.Bits.trtp = GetDdr5EncodedTRTP (Socket);

        RankList = GetRankNvList (Host, Socket, Ch, Dimm); // To gain access to the DRAM RTT values. DDR5_TODO: must update SPR code to use DDR5 ODT

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }
        if ((*DimmNvList)[Dimm].DcpmmPresent == 1) {
          continue;
        }
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
          continue;
        }

        /*
        N0: BacksideDramInitAfterQca starts
        N0.C00.D0.R0: MRW command: Address 0x00, CW 0, Data 0x0C, SubChMask 3   // BL16 with CL=28
        N0.C00.D0.R0: MRW command: Address 0x03, CW 0, Data 0x44, SubChMask 3   // 2N mode with DLL enable and keep multiple cycle MPC
        N0.C00.D0.R0: MRW command: Address 0x05, CW 0, Data 0x42, SubChMask 3   // Pull-up drive strength 40ohms, pull-down drive strength 40ohms
        N0.C00.D0.R0: MRW command: Address 0x06, CW 0, Data 0x03, SubChMask 3   // tWR = 66nCK, tRTP = 12nCK
        N0.C00.D0.R0: MRW command: Address 0x08, CW 0, Data 0x08, SubChMask 3   // Read preamble = 1tCK, Write preamble = 3tCK, postambles = 0.5 tCK
        N0.C00.D0.R0: MRW command: Address 0x09, CW 0, Data 0x00, SubChMask 3   // Vref DQ Mon and Vref CA Mon as normal mode.

        N0.C00.D0.R0: MRW command: Address 0x0A, CW 0, Data 0x34, SubChMask 3   // VrefDQ = 71.5% from XLS; Adding 10 to this value based on TxDqDqs empirical results
        N0.C00.D0.R0: MRW command: Address 0x0C, CW 0, Data 0x00, SubChMask 3   // tCCD_L as 8
        N0.C00.D0.R0: MRW command: Address 0x22, CW 0, Data 0x24, SubChMask 3   // Rtt_park = 60, Rtt_wr = 60
        N0.C00.D0.R0: MRW command: Address 0x23, CW 0, Data 0x00, SubChMask 3   // Rtt_nom_WR = OFF, Rtt_nom_RD = OFF
        N0: BacksideDramInitAfterQca ends
        */

        if (IsLrdimmPresent (Socket, Ch, Dimm) == 1){
          RcdParityAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
          RcdParityAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // USE_LATEST_RCD_SPEC
          RcdParityAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // USE_LATEST_RCD_SPEC
          // Block commands to DRAM, forward commands to DB
          RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, RcdParityAlert.Data);
          // Select all data buffers
          DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
          // Exit DQ pass-through mode
          DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW82_REG, 0);
          // Forward commands to DRAM, forward commands to DB
#ifdef USE_LATEST_RCD_SPEC
          RcdParityAlert.Bits.dram_interface_forward_all_cmds = 1;
          RcdParityAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // USE_LATEST_RCD_SPEC
          RcdParityAlert.Bits.dram_interface_block_all_cmds = 1;
          RcdParityAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // USE_LATEST_RCD_SPEC
          RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, RcdParityAlert.Data);
        }
        //
        // DDR5_TODO: These MRW should not be sent before DRAM QCA is done!
        // Move these DRAM settings after QCA training step.
        //
        RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

        //
        // Samsung W/A to fix Receive Enable Training fail
        //
        if((*DimmNvList)[Dimm].SPDDramMfgId == MFGID_SAMSUNG){
          WriteMPC (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, DLL_RESET, TIMING_4N, TRUE);
          FixedDelayMicroSecond (10); // delay at least tDLLk (< 10 us)
        }

        ProgramMr0Ddr5 (Host, Socket, Ch, Dimm, Rank);
#ifndef SPR_PO_FLAG
        //
        // Keep MPC as multiple cycles firstly.
        //
#ifdef USE_LATEST_DRAM_SPEC
        ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, &Mr2.Data);
        Mr2.Bits.cs_assertion_duration_mpc = 1;
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, Mr2.Data);
        TrainingDelay (Host, 64, 1);
#else // #ifdef USE_LATEST_DRAM_SPEC
        ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_3_REG, &Mr3.Data);
        Mr3.Bits.cs_assertion_duration_mpc = 1;
        Mr3.Bits.dll_enable = 1;
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_3_REG, Mr3.Data);
        TrainingDelay (Host, 64, 1);
#endif // #ifdef USE_LATEST_DRAM_SPEC
#endif //SPR_PO_FLAG

        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_5_REG, Mr5.Data);
        TrainingDelay (Host, 64, 1);

        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_6_REG, Mr6.Data);
        TrainingDelay (Host, 64, 1);

        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_8_REG, Mr8.Data);
        TrainingDelay (Host, 64, 1);

        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_9_REG, Mr9.Data);
        TrainingDelay (Host, 64, 1);

        Mr10.Data = 0;

        if (IsLrdimmPresent (Socket, Ch, Dimm) == 1) {
          //
          // For Lrdimm, the backside (aka Dram) Vref value is stored in the "lrbufTxvref". while the frontside/(aka. host side)
          // value is stored in "(*ChannelNvList)[Ch].txVrefSafe[Rank].
          //
          // ""lrbufTxvref"" is per strobe and strobe 0 value is used here because all strobes were initialized with the same value in
          // the InitDdrioInterfaceLate( ).
          //
          Mr10.Bits.vrefdq_cal_value = (UINT8)(*RankList)[Rank].lrbufTxVref[0];
        } else {
          Mr10.Bits.vrefdq_cal_value = (UINT8) (*ChannelNvList)[Ch].txVrefSafe[Rank];
        }
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "Will set [Dram Vref %% (txVrefSafe): 0x%x]\n", Mr10.Bits.vrefdq_cal_value);
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_10_REG, Mr10.Data);
        TrainingDelay (Host, 64, 1);

        TrainingDelay (Host, 64, 1);

        // 1) VrefCA(MR11) and VrefCS(MR12) are not programmed from here because there are specific VrefCA and VrefCS commands
        // 2) VrefCS is not part of the older 0.7x spec, which is why you can see a SendMrw() call against MR12 above
        // 3) The operation on MR10 above illustrates how to get ODT data from ChannelNvList
        //    As another example, one could get say... the converted Vref CA value as (UINT8)((*ChannelNvList)[Ch].ConvertedDramCaVrefValue[Rank])

        // Also:
        // 1) The operation on MR34 below illustrates how to get ODT values from a RankList
        // 2) MRs 32 and 33 are not programmed from here, because they can't receive MRWs.
        //    They are instead programmed using MPC commands, from BacksideDramInit()
        GetRttValues (Host, Socket, Ch, Dimm, Rank, &RttWriteVal, &RttParkVal, &RttNomRdVal, &RttNomWrVal, &DqsRttParkVal);
        // MR34
        Mr34.Data = 0;
        Mr34.Bits.rtt_wr = RttWriteVal;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Will set (stored) RttWr  of (%03d) ohms for [CH: %d] [DIMM: %d] [Rank: %d]\n", Ddr5RttLookup[Mr34.Bits.rtt_wr] , Ch, Dimm, Rank);
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_34_REG, Mr34.Data);
        TrainingDelay (Host, 64, 1);

        //
        // Set RTT_NOM_RD and RTT_NOM_WR as ODT table provided.
        //
        Mr35.Data = 0;
        Mr35.Bits.rtt_nom_rd = (UINT8) RttNomRdVal;
        Mr35.Bits.rtt_nom_wr = (UINT8) RttNomWrVal;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Will set (stored) RttNomRd of (%03d) ohms for [CH: %d] [DIMM: %d] [Rank: %d]\n", Ddr5RttLookup[RttNomRdVal], Ch, Dimm, Rank);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Will set (stored) RttNomWr of (%03d) ohms for [CH: %d] [DIMM: %d] [Rank: %d]\n", Ddr5RttLookup[RttNomWrVal], Ch, Dimm, Rank);
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_35_REG, Mr35.Data);
        TrainingDelay (Host, 64, 1);

        //
        // MR37 value was set up before the loop
        //
        DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_37_REG, Mr37.Data);

        if (FeaturePcdGet (PcdCteBuild) == FALSE) {
          WriteCadbPrechargeCmdDdr5 (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank, 0, 0, PREab_CMD);
        }

        DramPerStrobeMpcOdtRttInit (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank);

      } //Ch
    } //Rank loop
  }//Dimm loop
   //
   // tSTABx
   //
  TrainingDelay (Host, 100, 2);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "BacksideDramInitAfterQca ends\n");

  return SUCCESS;
} // BacksideDramInitAfterQca

// RW4A Default value of 0.7 Vdd per HSD 1409025990

#define RCD_ERROR_IN_VREF_CONTROL_DEFAULT 2

/**

  Program Backside ODT

  @param Host        - Pointer to sysHost
  @param Socket      - Socket number

  @retval SUCCESS

  **/
UINT32
ProgramBacksideOdt(
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT32                                                          RcwValue = 0;
  UINT8                                                           Channel;
  UINT8                                                           Dimm;
  CHANNEL_NVRAM_STRUCT                                            (*ChannelNvList)[MAX_CH];
  struct dimmNvram                                                (*DimmNvList)[MAX_DIMM];
  UINT8                                                           TableValue;
  DDR5_RCD_QCK_SIGNALS_DRIVER_CHARACTERISTICS_STRUCT              RcdQckSignalsDriverCharacteristics;
  DDR5_RCD_QXCA_AND_QXCS_N_SIGNALS_DRIVER_CHARACTERISTICS_STRUCT  RcdQcaQcsSignalsDriverCharacteristics;
#ifdef USE_LATEST_RCD_SPEC
  DDR5_RCD_ERROR_IN_VREF_STRUCT                                   RcdErrorInVref;
#endif

  ChannelNvList = GetChannelNvList(Host, Socket);

  for (Channel = 0; Channel < MAX_CH; Channel += GetChInc (Host)) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList(Host, Socket, Channel);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      // Look up value for RDIMM_RW0A in the backside ODT table

      GetRcdQckRonValueOdt (Socket, Channel, Dimm, &TableValue);

      // Set all drive strengths to the same CK value

      RcdQckSignalsDriverCharacteristics.Bits.qack_t_qack_c = TableValue;
      RcdQckSignalsDriverCharacteristics.Bits.qbck_t_qbck_c = TableValue;
      RcdQckSignalsDriverCharacteristics.Bits.qcck_t_qcck_c = TableValue;
      RcdQckSignalsDriverCharacteristics.Bits.qdck_t_qdck_c = TableValue;

      RcwValue = RcdQckSignalsDriverCharacteristics.Data;
      WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_QCK_SIGNALS_DRIVER_CHARACTERISTICS_REG, &(RcwValue));

      RcdQcaQcsSignalsDriverCharacteristics.Data = 0;

      // Look up value for RDIMM_RW0C [1:0]

      GetRcdQcaRonValueOdt (Socket, Channel, Dimm, &TableValue);

      RcdQcaQcsSignalsDriverCharacteristics.Bits.address_command_address_for_both_a_b_copies = TableValue;

      // Look up value for RDIMM_RW0C [5:4]

      GetRcdQcsRonValueOdt (Socket, Channel, Dimm, &TableValue);

      RcdQcaQcsSignalsDriverCharacteristics.Bits.qxcs0_n_qxcs1_n_outputs = TableValue;

      RcwValue = RcdQcaQcsSignalsDriverCharacteristics.Data;
      WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_QXCA_AND_QXCS_N_SIGNALS_DRIVER_CHARACTERISTICS_REG, &(RcwValue));

      //
      // Program RDIMM_RW4A to 0.7VDD (value = 2)
      //
#ifdef USE_LATEST_RCD_SPEC
      RcdErrorInVref.Data = 0;
      RcdErrorInVref.Bits.error_in_n_vref_control = RCD_ERROR_IN_VREF_CONTROL_DEFAULT;
      RcwValue = RcdErrorInVref.Data;
      WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, DDR5_RCD_ERROR_IN_VREF_REG, &(RcwValue));
#else
      RcwValue = 0x2;
      WriteRcdSmbDdr5 (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, BYTE_WRITE, RDIMM_CW_PAGE0, RDIMM_RW4A, &(RcwValue));
#endif
    } //Dimm
  } //Channel
  return SUCCESS;
}

/**

Init Db and Dram Interface

@param[in] Socket          - Socket Id
@param[in] Ch              - Channel number (0-based)
@param[in] Dimm            - DIMM number (0-based)
@param[in] SubChannelMas   - Sub Channel Mask
@param[in] Rank            - Rank number (0-based)

@retval N/A

**/
VOID
EFIAPI
InitDbDramInterfaceDdr5 (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  )
{
  PSYSHOST Host;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT         ParityAndAlert;
  DDR5_RCD_COMMAND_LATENCY_ADDER_CONFIGURATION_STRUCT CommandLatencyAdder;

  Host = GetSysHostPointer ();

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "Init Db and DramInterface starts\n");
  //
  // Enable DRAM interface to forward all commands.
  //
  ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
  ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
  ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
  RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

  //
  //add some delay
  //
  FixedDelayMicroSecond (1);
  //
  // Send NOP Command to Rank 0 and Rank 1 for initilization. Assume two ranks during the init flow. After init done, will based on the actual Rank number to do the training.
  //
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 0, 0, TRUE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 1, 0, TRUE, FALSE);
  //
  // Set command latency to 1tck.
  //
  CommandLatencyAdder.Data = 0;
  CommandLatencyAdder.Bits.latency_adder_nladd_to_all_dram_commands = 1;
  RcdControlWordWrite (Socket, Ch, SubChannelMask, Dimm, Rank, DDR5_RCD_COMMAND_LATENCY_ADDER_CONFIGURATION_REG, CommandLatencyAdder.Data);

  EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

  //
  // Three NOPs to take DRAM out of SR. Send NOP Command to Rank 0 and Rank 1
  // for initilization. Assume two ranks during the init flow. After init done,
  // will based on the actual Rank number to do the training.
  //
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 0, 0, FALSE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 0, 0, FALSE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 0, 0, FALSE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 1, 0, FALSE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 1, 0, FALSE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 1, 0, FALSE, FALSE);

  ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

  return;
} // InitDbDramInterfaceDdr5

/**

Init Db and Dram Interface

@param[in] Socket          - Socket Id
@param[in] Ch              - Channel number (0-based)
@param[in] Dimm            - DIMM number (0-based)
@param[in] SubChannelMas   - Sub Channel Mask
@param[in] Rank            - Rank number (0-based)

@retval N/A

**/
VOID
EFIAPI
InitDbDramInterface (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  )
{
#if FixedPcdGetBool(PcdMcrSupport)
  InitDbDramInterfaceMcr (Socket, Ch, Dimm, SubChannelMask, Rank);
#else
  InitDbDramInterfaceDdr5 (Socket, Ch, Dimm, SubChannelMask, Rank);
#endif // FixedPcdGetBool(PcdMcrSupport)
}

/**
  Program DQS Interval Control (MR45).

  @param[in]  Host          Pointer to sysHost
  @param[in]  Socket        Socket number
  @param[in]  Channel       Channel number on socket
  @param[in]  TimerRunTime  DQS interval timer run time (see MR45 spec)
**/
VOID
EFIAPI
ProgramDqsIntervalControl (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    TimerRunTime
  )
{
  DDR5_MODE_REGISTER_45_STRUCT  Mr45;
  struct dimmNvram              (*DimmNvList)[MAX_DIMM];
  UINT8                         Dimm;
  UINT8                         NumRanks;
  UINT8                         Rank;

  //
  // Return if SendMrw is not ready to target DRAM.
  //
  if (Host->var.mem.socket[Socket].TrainingStatus.Bits.QcaUcaTrainingDone == 0 &&
      UbiosGenerationOrHsleEnabled () == FALSE) {
    return;
  }

  //
  // The same value of MR45 should be used for every rank.
  //
  Mr45.Data = 0;
  Mr45.Bits.dqs_interval_timer_run_time = TimerRunTime;

  //
  // Loop through ranks on DIMMs, and set MR45 for each one.
  //
  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    NumRanks = MIN (Host->var.mem.socket[Socket].maxRankDimm, (*DimmNvList)[Dimm].numRanks);
    for (Rank = 0; Rank < NumRanks; Rank++) {
      if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      DramModeRegWrite (Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_45_REG, Mr45.Data);
    }
  }
}

/**
  Determine if MRW should be sent over SMBUS

  @param  Host    - Pointer to SysHost

  @retval TRUE  - Use SMBUS to send MRW
  @retval FALSE - Do not use SMBUS for MRW

**/
BOOLEAN
UseSmbusMrw (
  IN  PSYSHOST    Host,
  IN  UINT8        Socket
  )
{
  SYS_SETUP   *Setup;
  BOOLEAN     SmbMrwEnable;

  Setup = GetSysSetupPointer ();

  //
  // 1 - Assume SMBUS is needed for MRW commands
  //
  SmbMrwEnable = TRUE;

  if (Setup != NULL) {
    //
    // 2 - Follow the policy if it is available
    //
    SmbMrwEnable = Setup->mem.UseSmbusForMrwEarly;
  }

  //
  // 3 - Ignore the policy for Hybrid System Level Emulation
  //
  if (UbiosGenerationOrHsleEnabled ()) {
    SmbMrwEnable = FALSE;
  }

  //
  // 4 - Ignore the policy for Simics for case where DRAM CA training is skipped
  //
  if (IsSiliconWorkaroundEnabled ("22010473374")) {
    SmbMrwEnable = FALSE;
  }

  //
  // 5 - Once DRAM CA training is compelte, always perform MRW commands inband
  //
  if (Host->var.mem.socket[Socket].TrainingStatus.Bits.QcaUcaTrainingDone == 1) {
    SmbMrwEnable = FALSE;
  }

  return SmbMrwEnable;
}

/**
  Enter RCD CA pass through mode.

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/
VOID
EnterRcdCaPassThroughModeDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  DDR5_RCD_GLOBAL_FEATURES_STRUCT GlobalFeatures;

  ReadRcdCwCache (Socket, Channel, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures.Data);
  GlobalFeatures.Bits.ca_pass_through_mode_enable = 1;
  GlobalFeatures.Bits.ca_pass_through_mode_rank_selection = Rank;
  //
  // Send MRW using this CS (rank).
  //
  RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures.Data);
}

/**
  Enter RCD CA pass through mode.

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/
VOID
EnterRcdCaPassThroughMode (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
#if FixedPcdGetBool(PcdMcrSupport)
  EnterRcdCaBcastPassThroughMode (Socket, Channel, Dimm, Rank);
#else
  EnterRcdCaPassThroughModeDdr5 (Host, Socket, Channel, Dimm, Rank);
#endif // FixedPcdGetBool(PcdMcrSupport)

}

/**
  Exit RCD CA pass through mode.

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/
VOID
ExitRcdCaPassThroughModeDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  DDR5_RCD_GLOBAL_FEATURES_STRUCT GlobalFeatures;

  ReadRcdCwCache (Socket, Channel, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_GLOBAL_FEATURES_REG, &GlobalFeatures.Data);
  GlobalFeatures.Bits.ca_pass_through_mode_enable = 0;
  GlobalFeatures.Bits.ca_pass_through_mode_rank_selection = 0;
  //
  // Send MRW using the opposite CS (rank).
  //
  RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank ^ 1, DDR5_RCD_GLOBAL_FEATURES_REG, GlobalFeatures.Data);
} // ExitRcdCaPassThroughMode

/**
  Exit RCD CA pass through mode.

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/
VOID
ExitRcdCaPassThroughMode (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
#if FixedPcdGetBool(PcdMcrSupport)
  ExitRcdCaBcastPassThroughMode (Socket, Channel, Dimm, Rank);
#else
  ExitRcdCaPassThroughModeDdr5 (Host, Socket, Channel, Dimm, Rank);
#endif // FixedPcdGetBool(PcdMcrSupport)
}

/**

  This function returns the value of tREFI

  @param[in] Host         - Point to sysHost
  @param[in] Socket       - Socket Id
  @param[in] Ch           - Channel number
  @param[in] RefreshMode  - Refresh Mode
                              RefabNormal
                              RefabFineGrain
                              RefsbFineGrain

  @retval UINT32 Value of tREFI

**/
UINT32
GettREFIDdr5 (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN REFRESH_MODE   RefreshMode
)
{
  UINT32              tREFILocal;
  UINT8               MaxNumBanks;
  UINT8               NumBanks;
  UINT8               Dimm;
  struct channelNvram(*channelNvList)[MAX_CH];
  struct dimmNvram   (*DimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Calculate tREFI for DDR5
  //
  if ((*channelNvList)[Ch].twoXRefreshSetPerCh >= 20) {

    // Assume temp_lo = 0, so divide requested rate by 2
    tREFILocal = (UINT16)((3900 * 20) / (*channelNvList)[Ch].twoXRefreshSetPerCh * 10000 / GettCK (Host, Socket));

  } else {

    // Otherwise use 1x formula
    tREFILocal = (UINT16)((3900 * 10) / (*channelNvList)[Ch].twoXRefreshSetPerCh * 10000 / GettCK (Host, Socket));
  }

  switch (RefreshMode) {
    case RefabNormal:
      //
      // tREFI1 = tREFI
      // no change needed
      //
      break;
    case RefabFineGrain:
      //
      // tREFI2 = tREFI/2
      //
      tREFILocal = tREFILocal / 2;
      break;
    case RefsbFineGrain:
      //
      // tREFsb = tREFI/(2*n) us
      // n is the number of banks in a bank group (eg. 8G: n=2; 16G: n=4)
      //
      // The minimum number of DDR5 banks in a bank group is 2
      //
      MaxNumBanks = 2;
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
          continue;
        }
        //Get Max Banks to calculate trefi for fsbFinegrain. Gets the worst case of refresh timing within a channel
        NumBanks = 1 << (*DimmNvList)[Dimm].numBankAddressBits;
        if (NumBanks > MaxNumBanks) {
          MaxNumBanks = NumBanks;
        }
      }
      tREFILocal = tREFILocal / (2 * MaxNumBanks);
      break;
    default:
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Refresh mode set incorrectly\n");
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_180);
      break;
  }

  CheckBoundsChip (MemTechDdr, Socket, Ch, tREFI, &tREFILocal);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Refresh rate = %d\n", tREFILocal);

  return tREFILocal;
} // GettREFIDdr5

/**

  Enables Write CRC in the DRAM

  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number within the socket
  @param[in] Dimm     - DIMM number within the channel
  @param[in] Rank     - Rank number within the DIMM

  @retval N/A

**/
VOID
EFIAPI
Ddr5DramEnableWrCrc (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     Rank
  )
{
  PSYSHOST                        Host;
  DDR5_MODE_REGISTER_50_STRUCT    Mr50;

  Host = GetSysHostPointer ();

  Mr50.Data = 0;
  ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_50_REG, &Mr50.Data);
  Mr50.Bits.write_crc_enable_lower_nibble = 1;
  Mr50.Bits.write_crc_enable_upper_nibble = 1;

  DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_50_REG, Mr50.Data);
}

/**

  Prints all DRAM, Register and Buffer CSR data from all DIMMs on the socket.
  Note: this function assumes that the read training has been successfully
  completed in order to read DRAM and Buffer data.

  @param[in] Socket  - Socket Id

  @retval N/A

**/
VOID
EFIAPI
DumpDimmStateDdr5 (
  IN  UINT8     Socket
  )
{
  DumpDramModeRegisters (Socket);
  DumpRcdControlWords (Socket);
}

/**
  Encode an IBT impedance to the corresponding register value.

  The value returned is meant to be written to one of the fields of the RCD's
  IBT Global Control Word (RW10).

  @param[in]  ImpedanceInOhms       Impedance value to be encoded.
  @param[out] EncodedValue          Encoded IBT impedance value per RCD spec.

  @retval     MRC_STATUS_SUCCESS    Encoded successfully.
  @retval     MRC_STATUS_NOT_FOUND  Could not find encoding, default returned.
**/
MRC_STATUS
EFIAPI
EncodeIbtValueToRegister (
  IN  UINT16  ImpedanceInOhms,
  OUT UINT8   *EncodedValue
  )
{
  CONST UINT16  Ddr5IbtLookup[] = {
    60,           // 00=60,
    IBT_TBD_OHMS, // 01=TBD,
    IBT_TBD_OHMS, // 10=TBD,
    //
    // Note: OFF is not intended for normal operation and will not be returned:
    // 11=OFF
    //
  };

  UINT8         NumberOfEntries;
  UINT8         Index;

  NumberOfEntries = ARRAY_SIZE (Ddr5IbtLookup);
  *EncodedValue = IBT_DEFAULT_ENCODED;

  for (Index = 0; Index < NumberOfEntries; Index++) {
    if (Ddr5IbtLookup[Index] == ImpedanceInOhms) {
      *EncodedValue = Index;
      return MRC_STATUS_SUCCESS;
    }
  }

  return MRC_STATUS_NOT_FOUND;
}

/**
  Save and disable DCPMM training mode for FMC
  This function saves the DCPMM training context for restore by RestoreDcpmmTrainingContext

  @param[in]  Host                  - Pointer to SysHost
  @param[in]  Socket                - Socket number
  @param[in]  ChBitmask             - Bitmask of channels to initialize
  @param[out] TrainingContext       - DCPMM training context array pointer

  @retval N/A
**/
VOID
SaveDcpmmTrainingContext (
  IN     PSYSHOST             Host,
  IN     UINT8                Socket,
  IN     UINT32               ChBitMask,
     OUT DCPMM_TRAIN_CONTEXT  TrainingContext[MAX_CH]
  )
{
  UINT8                 Ch;
  UINT8                 MaxChDdr;
  BOOLEAN               BpsIsFound;

  if (IsDcpmmPresentSocket (Socket) == FALSE) {
    //
    // Nothing to do
    //
    return;
  }

  MaxChDdr = GetMaxChDdr ();

  BpsIsFound = FALSE;
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((((1 << Ch) & ChBitMask) == 0) ||
        (IsChannelEnabled (Socket, Ch) == FALSE) ||
        (IsDcpmmPresentChannel (Socket, Ch) == FALSE)) {
      continue;
    }
    TrainingContext[Ch].DaTrainingEn = 0;
    TrainingContext[Ch].DTrainingEn = 0;
    TrainingContext[Ch].HsioTrainingEn = 0;

    if (GetFmcType (Socket, Ch) == FMC_CWV_TYPE) {
      GetRestoreAepTrainingModeSMB (Host, Socket, Ch, TRUE,
        &(TrainingContext[Ch].DaTrainingEn), &(TrainingContext[Ch].DTrainingEn), &(TrainingContext[Ch].HsioTrainingEn));
      BpsIsFound = TRUE;
    }
  } // Ch
  //
  // Disale DCPMM training step for FMC
  //
  if (BpsIsFound) {
    SetAepTrainingModeSMB (Host, Socket, DISABLE_TRAINING_STEP);
  }
}

/**
  Resetore DCPMM training mode for FMC from context buffer

  @param[in]  Host                  - Pointer to SysHost
  @param[in]  Socket                - Socket number
  @param[in]  ChBitmask             - Bitmask of channels to initialize
  @param[in]  TrainingContext       - DCPMM training context array pointer

  @retval N/A
**/
VOID
RestoreDcpmmTrainingContext (
  IN  PSYSHOST                Host,
  IN  UINT8                   Socket,
  IN  UINT32                  ChBitMask,
  IN  DCPMM_TRAIN_CONTEXT     TrainingContext[MAX_CH]
  )
{
  UINT8                 Ch;
  UINT8                 MaxChDdr;

  if (IsDcpmmPresentSocket (Socket) == FALSE) {
    //
    // Nothing to do
    //
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  //
  // Restore DCPMM Training mode setting for BWV
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((((1 << Ch) & ChBitMask) == 0) ||
        (IsChannelEnabled (Socket, Ch) == FALSE) ||
        (IsDcpmmPresentChannel (Socket, Ch) == FALSE)) {
      continue;
    }

    if (GetFmcType (Socket, Ch) == FMC_CWV_TYPE) {
      GetRestoreAepTrainingModeSMB (Host, Socket, Ch, FALSE,
        &(TrainingContext[Ch].DaTrainingEn), &(TrainingContext[Ch].DTrainingEn), &(TrainingContext[Ch].HsioTrainingEn));
    }
  } // Ch

}

/**
  If soft PPR has already been applied, we need to re-run
  it every time JedecInitSequence is executed

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] ChBitMask  - Bit Mask of Channels to operate on

**/
VOID
PostPackageRepairJedecInit (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    ChBitMask
  )
{
  // PPR_SUPPORT
  //
  // If soft PPR has already been applied, we need to re-run
  // it every time JedecInitSequence is executed
  //

  Host->var.mem.notRunningFromCT = 1; //Set when not calling from Call Table
  Host->var.mem.chBitMask = ChBitMask;

  if (Host->var.mem.softPprDone[Socket]) {
    PostPackageRepairMain (Host);
  }

  Host->var.mem.notRunningFromCT = 0; //Clear value

  // End PPR_SUPPORT
}

/**
  Set the CMD timing mode in the DRAM

  @param[in] Host             - Pointer to SysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number (socket-based)
  @param[in] SubChannelMask   - Mask of subchannels to configure
  @param[in] Dimm             - DIMM number within the channel
  @param[in] Rank             - Rank number within the DIMM
  @param[in] TimingMode       - CMD timing mode to set

  @retval   N/A

**/
VOID
SetDramCmdTiming (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     SubChannelMask,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  UINT8     TimingMode
  )
{
  EnterRcdCaPassThroughMode (Host, Socket, Channel, Dimm, Rank);

   if (TimingMode == TIMING_1N) {
     WriteMPC (Host, Socket, Channel, Dimm, SubChannelMask, Rank, SET_1N_CMD_TIMING, TIMING_4N, TRUE);
   } else {
     //
     // The only other option is 2N
     //
     WriteMPC (Host, Socket, Channel, Dimm, SubChannelMask, Rank, SET_2N_CMD_TIMING, TIMING_4N, TRUE);
   }

  ExitRcdCaPassThroughMode (Host, Socket, Channel, Dimm, Rank);
}

/**
  Restore per-device CA VREF

  @param[in]  Host       - Pointer to SysHost
  @param[in]  Socket     - Socket number
  @param[in]  Channel    - Channel number (socket-based)

  @retval   N/A

**/
VOID
RestoreVrefCa (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  UINT8               SubCh;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Dram;
  UINT8               NumRanks;
  UINT16              CaVref;
  UINT8               SubChMSV;

  SubChMSV    = GetSubChMaxStrobeValid (Host);
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if ((IsDimmPresent (Socket, Channel, Dimm) == FALSE) || (IsDcpmmPresentDimm (Socket, Channel, Dimm) == TRUE)) {
        continue;
      }
      if (CheckS3Jedec (Host, Socket, Channel, Dimm) == 0) {
        continue;
      }

      if (IsX4Dimm (Socket, Channel, Dimm) == FALSE) {
        SubChMSV = SubChMSV / 2;
      }

      NumRanks = GetNumberOfRanksOnDimm (Socket, Channel, Dimm);
      for (Rank = 0; Rank < NumRanks; Rank++) {
        if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        for (Dram = 0; Dram < SubChMSV; Dram++) {
          if (GetSavedDramVrefCa (Socket, Channel, 1 << SubCh, Dimm, Rank, Dram, &CaVref) == MRC_STATUS_SUCCESS) {
            WriteVrefCaNoCacheUpdate (Host, Socket, Channel, Dimm, 1 << SubCh, Rank, Dram, CaVref, TIMING_MULTI_CS, TRUE);
          }
        } // Dram
        WriteMPC (Host, Socket, Channel, Dimm, 1 << SubCh, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);
      } // Rank
    } // Dimm
  } // SubCh
}

/**
  Restore per-rank CS VREF

  @param[in] Host             - Pointer to SysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number (socket-based)
  @param[in] SubChMask        - Mask of subchannels to configure
  @param[in] Dimm             - DIMM number within the channel
  @param[in] Rank             - Rank number within the DIMM
  @param[in] TimingMode       - CMD timing mode to set

  @retval   N/A

**/
VOID
RestoreCsVref (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     SubChMask,
  IN  UINT8     Dimm,
  IN  UINT8     Rank
  )
{
  UINT8         SubCh;
  UINT16        CsVref;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & SubChMask) == 0) {
      continue;
    }
    //
    // Set DRAM CS Vref Value.--DIMM sub channel A and B at the same time.
    // Use the cached value from DRAM 0 - currently all DRAMs have the same value
    //
    if (GetSavedDramVrefCs (Socket, Channel, 1 << SubCh, Dimm, Rank, 0, &CsVref) == MRC_STATUS_SUCCESS) {
      WriteVrefCsNoCacheUpdate (Host, Socket, Channel, Dimm, 1 << SubCh, Rank, ALL_STROBES, CsVref, TIMING_MULTI_CS, TRUE);
    }
    WriteMPC (Host, Socket, Channel, Dimm, 1 << SubCh, Rank, APPLY_VREFCA_RTT_CA_CS_CK, TIMING_4N, TRUE);
  } // SubCh
}

/**
  Initialize DRAM as part of JEDEC Init sequence

  @param[in]  Host       - Pointer to SysHost
  @param[in]  Socket     - Socket number
  @param[in]  Channel    - Channel number (socket-based)

  @retval   MRC_STATUS_SUCCESS if registers are restored
  @retval   MRC_STATUS_FAILURE otherwise
  @retval   MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
DoDramInitChannel (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               NumRanks;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  if (Host->var.mem.socket[Socket].TrainingStatus.Bits.QcaUcaTrainingDone == 0) {
    //
    // DRAM CA interface not yet trained
    //
    return MRC_STATUS_FAILURE;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

    if ((IsDimmPresent (Socket, Channel, Dimm) == FALSE)     ||
        (IsDcpmmPresentDimm (Socket, Channel, Dimm) == TRUE) ||
        (CheckS3Jedec (Host, Socket, Channel, Dimm) == 0))
    {
      continue;
    }

    NumRanks = GetNumberOfRanksOnDimm (Socket, Channel, Dimm);
    for (Rank = 0; Rank < NumRanks; Rank++) {
      if (CheckRank (Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      //
      // Take DRAMs out of reset
      // JEDEC DRAM Reset Initialization steps 6 and 7 (Te thorugh Tj)
      //
      DramResetInitialization (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank);

      //
      // Set run-time CMD timing mode in the DRAMs
      //
      SetDramCmdTiming (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, (*ChannelNvList) [Channel].timingMode);

      //
      // Restore per-rank DRAM CS VREF from NVRAM
      //
      RestoreCsVref (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank);

      //
      // Set per-rank DRAM CA VREF to initial fixed value
      // Per-device values will be restored below after PDA enumerate ID executes
      //
      WriteVrefCaNoCacheUpdate (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank, ALL_STROBES, QCA_VREF_INIT_VALUE, TIMING_MULTI_CS, TRUE);

      //
      // Initialize DRAM ODT
      //
      DramMpcOdtRttInit (Host, Socket, Channel, Dimm, TWO_SUBCH_MASK, Rank);

      RambusRevC1Wa1 (Host, Socket, Channel, Dimm, Rank);

    } // Rank
  } // Dimm
  //
  // tSTABx
  //
  TrainingDelay (Host, 100, 2);

  //
  // Programs CAS Latency for DDR devices
  //
  ProgramCasLDdr5 (Host, Socket, Channel);

  //
  // Set the PDA enuemrate ID.
  //
  SetPdaEnumerateIdDdr5 (Host, Socket, 1 << Channel);

  //
  // Restore per-device DRAM CA VREF from NVRAM
  //
  RestoreVrefCa (Host, Socket, Channel);

  //
  // Initialize critical mode registers
  //
  BacksideDramInitAfterQca (Host, Socket, 1 << Channel);

  //
  // Restore mode registers from cache
  //
  RestoreDramModeRegisters (Host, Socket, Channel);

  return MRC_STATUS_SUCCESS;
}

/**
  Perform steps necessary to take the RCD out of reset
  Jedec Device Initialization steps Tc to Te

  @param[in]  Host       - Pointer to SysHost
  @param[in]  Socket     - Socket number
  @param[in]  Channel    - Channel number (socket-based)

  @retval   N/A

**/
VOID
JedecInitRcdFromReset (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    ChannelBitMask
  )
{
  UINT8       Channel;
  UINT8       Dimm;
  UINT8       Rank;
  UINT8       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((IsChannelEnabled (Socket, Channel) == FALSE) || ((1 << Channel) & (ChannelBitMask == 0))) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }
      Rank = 0;
      //
      // The control words initialized in this function are sticky across resets
      // but not across power cycles
      //
      JedecRcdCwInitialization (Host, Socket, Channel, Dimm, Rank);
      //
      // Take DRAMs out of reset
      //
      RcdReleaseQrst (Host, Socket, Channel, Dimm, Rank);

    } // Dimm
    //
    // tSTAB02 = 25us
    //
    FixedDelayMicroSecond (25);
    //
    // Clear CS_n override - allow CS_n to be driven with commands from the host
    //
    OverrideCke (Socket, Channel, 0, 0);
  } // Channel
}

/**
  Initialize RDIMM RCD as part of JEDEC Init sequence

  This routine covers JEDEC reset initialization steps for RCD:
     - Time points Td to Te
    - RCD time points Ta through Tc
    - Data Buffer time points Ta through Tc


  @param[in]  Host       - Pointer to SysHost
  @param[in]  Socket     - Socket number
  @param[in]  Channel    - Channel number (socket-based)

  @retval   MRC_STATUS_SUCCESS if registers are restored
  @retval   MRC_STATUS_FAILURE otherwise
  @retval   MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
DoRcdInitChannel (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  UINT8               Dimm;
  UINT8               SubChannel;

  if (Host->nvram.mem.dimmTypePresent == RDIMM) {

    //
    // Take the RCDs out of reset
    //
    JedecInitRcdFromReset (Host, Socket, 1 << Channel);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }
      if (CheckRank (Host, Socket, Channel, Dimm, 0, DONT_CHECK_MAPOUT)) {
        continue;
      }
      if (CheckS3Jedec (Host, Socket, Channel, Dimm) == 0) {
        continue;
      }
      if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
        continue;
      }

      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

        RestoreRcdControlWordsFromCache (Socket, Channel, SubChannel, Dimm);

      } // SubChannel
    } // Dimm
  } // RDIMM

  return MRC_STATUS_SUCCESS;
}

/**
  Initialize LRDIMM buffer as part of JEDEC Init sequence

  @param[in]  Host       - Pointer to SysHost
  @param[in]  Socket     - Socket number
  @param[in]  Channel    - Channel number (socket-based)

  @retval   MRC_STATUS_SUCCESS if registers are restored
  @retval   MRC_STATUS_FAILURE otherwise
  @retval   MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
DoLrBufferInitChannel (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  UINT8               Dimm;

  PbaEnumerateIdDdr5 (Host);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
      continue;
    }

    if (IsLrdimmPresent (Socket, Channel, Dimm)) {
      if (CheckS3Jedec (Host, Socket, Channel, Dimm) == 0) {
        continue;
      }
      RestoreLrbufferControlWordsFromCache (Socket, Channel, Dimm);
    }
  } // Dimm

  return MRC_FUNCTION_NOT_IMPLEMENTED;
}

/**

  Restore TxVref to a known good value

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number within the socket

  @retval N/A

**/
VOID
RestoreTxVref (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch
  )
{
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               NumRanks;

  if (Host->nvram.mem.socket[Socket].TrainingStepDone.TxVref == 1) {

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }
      if (CheckS3Jedec (Host, Socket, Ch, Dimm) == 0) {
        continue;
      }

      NumRanks = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
      for (Rank = 0; Rank < NumRanks; Rank++) {
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        if ((Rank > 0) & (IsLrdimmPresent (Socket, Ch, Dimm))) {
          continue;
        } // only do Rank = 0 for lrdimm

        RestoreOffset (Host, Socket, Ch, Dimm, Rank, 0, DdrLevel, TxVref);
      } // Rank loop
    } // Dimm loop
  }
}

/**

  Executes the JEDEC initialization sequence for the given channel

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number within the socket

  @retval MRC_STATUS_SUCCESS if sequence is successful
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if function is not implemented
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
JedecInitDdrChannelDdr5 (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch
  )
{
  struct channelNvram (*ChannelNvList)[MAX_CH];
  BootMode            SysBootMode;

  SysBootMode   = GetSysBootMode ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  //
  // Initialize RDIMM RCD and restore control words
  // JEDEC RCD Reset initialization time points Td through Tl
  // Also DRAM reset initialization through time Td (release QCS_n)
  //
  DoRcdInitChannel (Host, Socket, Ch);

  //
  // FMC CSR unlock sequence
  //
  FmcCsrUnlockCh (Socket, Ch);

  //
  // LRDIMM buffer init and restore control words
  //
  DoLrBufferInitChannel (Host, Socket, Ch);

  //
  // Initialize DRAMs and restore mode registers
  // JEDEC DRAM reset initialization time points Te through Tz
  //
  DoDramInitChannel (Host, Socket, Ch);

  //
  // Issue ZQCAL. DDRT Cosim/Cte may execute in 1n causing
  // parity issue. ICX_TODO clean CABD ZQCal for 2 cycle.
  //
  if (FeaturePcdGet (PcdCosimBuild) == FALSE) {
    DoZQ (Host, Socket, Ch, ZQ_LONG);
  }

  //
  // Restore front-side TxVref to known good value
  //
  RestoreTxVref (Host, Socket, Ch);

  return MRC_STATUS_SUCCESS;
} // JedecInitDdrChannelDdr5

/**

  Executes the JEDEC initialization sequence for each channel on the provided socket number

  @param Host      - Pointer to sysHost
  @param Socket    - Socket Id
  @param ChBitMask - Bit Mask of Channels Present on the Socket

  @retval MRC_STATUS_SUCCESS if sequence is successful
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if function is not implemented
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
JedecInitDdrAllDdr5 (
  PSYSHOST      Host,
  UINT8         Socket,
  UINT32        ChBitMask
  )
{
  UINT8               Ch;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MAX, "JEDEC Init\n");

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((IsChannelEnabled (Socket, Ch) == FALSE) ||
        ((ChBitMask & (1 << Ch)) == 0))
    {
      continue;
    }

    JedecInitDdrChannelDdr5 (Host, Socket, Ch);

  } // Ch

  return MRC_STATUS_SUCCESS;
} // JedecInitDdrAllDdr5

/**

  Starts the JEDEC initialization sequence for DDR5

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket number
  @param[in] ChBitMask  - Bit Mask of Channels to operate on

  @retval MRC_STATUS_SUCCESS if Jedec Init sequences is successful or is skipped
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if function is not implemented
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
JedecInitSequenceDdr5 (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    ChBitMask
  )
{
  UINT8                 MaxChDdr;
  DCPMM_TRAIN_CONTEXT   DcpmmTrainingContext[MAX_CH];

  if (((GetSysBootMode () != NormalBoot) || (Host->var.mem.subBootMode == WarmBootFast))) {
    RcDebugPrint (SDBG_MAX, "JEDEC Init Sequence - Skipped\n");
    return MRC_STATUS_SUCCESS;
  }

  MaxChDdr = GetMaxChDdr ();

  //
  // Save Original DCPMM training mode for FMC
  //
  SaveDcpmmTrainingContext (Host, Socket, ChBitMask, DcpmmTrainingContext);
  //
  // Issue reset to the channels
  // This routine covers the first few JEDEC reset initialization steps:
  //   - UDIMM time points Ta through Td
  //   - RDIMM/LRDIMM time points Ta to Tc
  //       - DoRcdInitChannel will cover time points through Td
  //
  ResetAllDdrChannelsPreTraining (Host, Socket, ChBitMask);

  //
  // Initiate JEDEC init
  //
  JedecInitDdrAllDdr5 (Host, Socket, ChBitMask);

  //
  // DIMM reset will reset rows that were previously repaired with soft post-package repair
  // Rerun SPPR in case there are any bad rows
  //
  PostPackageRepairJedecInit (Host, Socket, ChBitMask);

  //
  // Restore DCPMM Training mode setting for FMC
  //
  RestoreDcpmmTrainingContext (Host, Socket, ChBitMask, DcpmmTrainingContext);

  RcDebugPrint (SDBG_MAX, "JEDEC Init Sequence - Done\n");

  return MRC_STATUS_SUCCESS;
} // JedecInitSequence

/**

  Program all DRAM in a channel with the refresh mode

  @param[in] Socket       - Socket Id
  @param[in] Ch           - Channel number
  @param[in] RefreshMode  - Refresh Mode
                              RefabNormal
                              RefabFineGrain
                              RefsbFineGrain

  @retval MRC_STATUS_SUCCSS if refresh mode is programmed
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
EFIAPI
SetDramRefreshMode (
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  REFRESH_MODE    RefreshMode
  )
{
  UINT8                         Rank;
  UINT8                         Dimm;
  UINT8                         NumRanks;
  DDR5_MODE_REGISTER_4_STRUCT   Mr4;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    NumRanks = GetNumberOfRanksOnDimm (Socket, Ch, Dimm);
    for (Rank = 0; Rank < NumRanks; Rank++) {
      if (IsRankValid (Socket, Ch, Dimm, Rank, FALSE) == FALSE) {
        continue;
      }
      ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_4_REG, &Mr4.Data);
      //
      // 0B: Normal Refresh Mode (tRFC1)
      // 1B: Fine Granularity Refresh Mode (tRFC2)
      //
      if (RefreshMode == RefabNormal) {
        Mr4.Bits.refresh_trfc_mode = 0;
      } else {
        Mr4.Bits.refresh_trfc_mode = 1;
      }
      DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_4_REG, Mr4.Data);
    } // Rank
  } // Dimm
  return MRC_STATUS_SUCCESS;
}

/**

  Return the DDR5 Refresh Mode policy setting

  @retval Refresh Mode - RefabNormal
                       - RefabFineGrain
                       - RefsbFineGrain

**/
REFRESH_MODE
EFIAPI
GetRefreshModePolicy (
  VOID
  )
{
  SYS_SETUP       *Setup;
  REFRESH_MODE    RefreshMode;

  Setup  = GetSysSetupPointer ();

  switch (Setup->mem.dfxMemSetup.DfxDdr5RefreshMode) {
    case DDR5_REFRESH_MODE_AB_NORMAL:
      RefreshMode = RefabNormal;
      break;
    case DDR5_REFRESH_MODE_AB_FINE:
      RefreshMode = RefabFineGrain;
      break;
    case DDR5_REFRESH_MODE_SB_FINE:
      RefreshMode = RefsbFineGrain;
      break;
    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_222);
      RefreshMode = RefabNormal;
      break;
  }
  return RefreshMode;
}

/**

  Get set RCD DCS vref based on the provided signal number

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number (0-based)
  @param[in] Dimm             - DIMM number
  @param[in] Signal           - Enumerated signal name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out] Value      - Value to set or get

  @retval Status
**/
MRC_STATUS
EFIAPI
GetSetRcdDcsVref (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Channel,
  IN     UINT8    SubChannel,
  IN     UINT8    Dimm,
  IN     GSM_CSN  Signal,
  IN     UINT8    Mode,
  IN OUT INT16   *Value
  )
{
  UINT16            MinLimit;
  UINT16            MaxLimit;
  UINT16            Delay;
  UINT8             CwRegAddress;
  UINT8             DcsVref;
  INT16             DcsVrefNew;
  UINT8             CurrentPageNumber;

  switch (Signal) {
  case CS0_A_N:
  case CS0_B_N:
  case CS2_A_N:
  case CS2_B_N:
    CwRegAddress = RDIMM_RW48;
    break;
  case CS1_A_N:
  case CS1_B_N:
  case CS3_A_N:
  case CS3_B_N:
    CwRegAddress = RDIMM_RW49;
    break;
  default:
    return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
    break;
  }

  //
  // get register data
  //
  if (Mode & GSM_READ_CSR) {
    if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
      GetSetDcsVrefDdrt2 (Host, Socket, Channel, Dimm, SubChannel, Signal, GSM_READ_CSR, &DcsVref);
    } else {
      ReadRcdSmbByteDdr5 (Host, Socket, Channel, Dimm, SubChannel, RDIMM_CW_PAGE0, CwRegAddress, &DcsVref);
    }
  } else {
    ReadRcdCwCache (Socket, Channel, SubChannel, Dimm, RDIMM_CW_PAGE0, CwRegAddress, &DcsVref);
  }

  if (Mode & GSM_READ_ONLY) {
    *Value = DcsVref;
  } else {
    if (Mode & GSM_WRITE_OFFSET) {
      DcsVrefNew = DcsVref + *Value;
    } else {
      DcsVrefNew = *Value;
    }
    //
    // check DcsVref Value
    //
    GetDimmParamGroupLimits (DdrLevel, RcdDcsVref, &MinLimit, &MaxLimit, &Delay);
    if ((DcsVrefNew >= MinLimit) && (DcsVrefNew <= MaxLimit)) {
      ReadRcdCwCache (Socket, Channel, SubChannel, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_CW_PAGE_REG, &CurrentPageNumber);
      if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
        GetSetDcsVrefDdrt2 (Host, Socket, Channel, Dimm, SubChannel, Signal, GSM_FORCE_WRITE, (UINT8 *) &DcsVrefNew);
        WriteRcdCwCache (Socket, Channel, 1 << SubChannel, Dimm, RDIMM_CW_PAGE0, CwRegAddress, (UINT8) DcsVrefNew);
      } else {
        if (CurrentPageNumber != RDIMM_CW_PAGE0) {
          RcdControlWordWrite (Socket, Channel, 1 << SubChannel, Dimm, 0, DDR5_RCD_CW_PAGE_REG, RDIMM_CW_PAGE0);
        }
        RcdControlWordWrite (Socket, Channel, 1 << SubChannel, Dimm, 0, CwRegAddress, (UINT8)DcsVrefNew);
      }
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                     "Rcd DcsVref Out of range!! DcsVrefNew = 0x%x, MinLimit = 0x%x, MaxLimit = 0x%x\n", DcsVrefNew, MinLimit, MaxLimit);
      return MRC_STATUS_FAILURE;
    }
  }

  return MRC_STATUS_SUCCESS;
}

/**

  Get set RCD command DFE vref based on the provided signal number

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] SubChannel       - SubChannel number
  @param[in] Signal           - Enumerated signal name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              -  GSM_READ_ONLY - Do not write
                              -  GSM_WRITE_OFFSET - Write offset and not value
                              -  GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

  @retval Status
**/
MRC_STATUS
GetSetRcdDcaDfeVrefInternal (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Channel,
  IN    UINT8     Dimm,
  IN    UINT8     SubChannel,
  IN    GSM_CSN   Signal,
  IN    UINT8     Mode,
  IN OUT INT16    *Value
  )
{
#ifdef USE_LATEST_RCD_SPEC
  UINT8             CwRegAddress;
  INT16             CurVal;
  INT16             Temp;
  UINT8             Bit;
  UINT8             DfeVref;
  UINT8             RangeSelect;
  UINT8             DfeVrefNew;
  UINT8             RangeSelectNew;
  DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_STRUCT Ddr5RcdDparDcaDfeTrainingMode;

  switch (Signal) {
    case A0A:
      CwRegAddress = RDIMM_RW62;
      Bit = 0;
      break;
    case A1A:
      CwRegAddress = RDIMM_RW66;
      Bit = 1;
      break;
    case A2A:
      CwRegAddress = RDIMM_RW6A;
      Bit = 2;
      break;
    case A3A:
      CwRegAddress = RDIMM_RW6E;
      Bit = 3;
      break;
    case A4A:
      CwRegAddress = RDIMM_RW72;
      Bit = 4;
      break;
    case A5A:
      CwRegAddress = RDIMM_RW76;
      Bit = 5;
      break;
    case A6A:
      CwRegAddress = RDIMM_RW7A;
      Bit = 6;
      break;
    case PARA:
      CwRegAddress = RDIMM_RW7E;
      Bit = 7;
      break;

    default:
      return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
      break;
  }

  ReadRcdCwCache (Socket, Channel, SubChannel, Dimm, RDIMM_CW_PAGE0, RDIMM_RW3F, &RangeSelect);
  ReadRcdCwCache (Socket, Channel, SubChannel, Dimm, RDIMM_CW_PAGE2, CwRegAddress, &DfeVref);

  CurVal = DfeVref;

  // Check the sign bit. A value of 1 means negative offset
  if ((RangeSelect >> Bit) & BIT0) {
    CurVal = -1 * CurVal;
  }

  //
  // Normalize to range of 0 to 2 * MAX_RCD_CA_DFE_VREF_RANGE, so the value is always positive.
  //
  CurVal += MAX_RCD_CA_DFE_VREF_RANGE;

  if (Mode & GSM_READ_ONLY) {
      *Value = CurVal;
    } else {
      // Adjust the current value by offset
      if (Mode & GSM_WRITE_OFFSET) {
        // Get the new offset (note: can be negative)
        Temp = CurVal + *Value;
      } else {
        // FORCE_WRITE
        Temp = *Value;
      }

      // Check range
      if ((Temp >= 0) && (Temp <= 2 * MAX_RCD_CA_DFE_VREF_RANGE)) {
        RangeSelectNew = RangeSelect & (UINT8)~(BIT0 << Bit);
        if (Temp < MAX_RCD_CA_DFE_VREF_RANGE) {
          // Negative
          RangeSelectNew |= (BIT0 << Bit);
          DfeVrefNew = (UINT8)(MAX_RCD_CA_DFE_VREF_RANGE - Temp);
        } else {
          DfeVrefNew = (UINT8)(Temp - MAX_RCD_CA_DFE_VREF_RANGE);
        }
      } else {
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_74);
        return MRC_STATUS_FAILURE;
      }

      //
      // Write the new value (range and value)
      //
      RcdControlWordWrite (Socket, Channel, (1 << SubChannel), Dimm, 0, RDIMM_RW3F, RangeSelectNew);

      RcdControlWordWrite (Socket, Channel, (1 << SubChannel), Dimm, 0, DDR5_RCD_CW_PAGE_REG, RDIMM_CW_PAGE2);
      RcdControlWordWrite (Socket, Channel, (1 << SubChannel), Dimm, 0, CwRegAddress, DfeVrefNew);

      //
      // Select the corresponding signal pin.
      //
      ReadRcdCwCache (Socket, Channel, SubChannel, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_REG, &Ddr5RcdDparDcaDfeTrainingMode.Data);
      if (Ddr5RcdDparDcaDfeTrainingMode.Bits.dcn_dfe_pin_selection != Bit) {
        Ddr5RcdDparDcaDfeTrainingMode.Bits.dcn_dfe_pin_selection = Bit;
        RcdControlWordWrite (Socket, Channel, (1 << SubChannel), Dimm, 0, DDR5_RCD_DPAR_AND_DCA_6_0_DFE_TRAINING_MODE_REG, Ddr5RcdDparDcaDfeTrainingMode.Data);
      }
    }
#endif
  return MRC_STATUS_SUCCESS;
}

/**
  Multi-use function to either get or set RCD command DFE vref based on the provided siganl number

  @param[in] Host        - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] SubChannel  - SubChannel number
  @param[in] Dimm        - DIMM number within a DDR channel (0-based, farthest slot == 0)
  @param[in] Signal      - Enumerated signal name
  @param[in] Mode        - GSM_READ_CSR - Read the data from hardware and not cache
                         -  GSM_READ_ONLY - Do not write
                         -  GSM_WRITE_OFFSET - Write offset and not value
                         -  GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value - Value to program

  @retval Status

**/
MRC_STATUS
EFIAPI
GetSetDcaDfeVref (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Channel,
  IN     UINT8    SubChannel,
  IN     UINT8    Dimm,
  IN     GSM_CSN  Signal,
  IN     UINT8    Mode,
  IN OUT INT16    *Value
  )
{
  if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
    // Placeholder! To add DDRT2 version  DCA DfeVref GetSet function.
    return MRC_FUNCTION_NOT_IMPLEMENTED;
  } else {
    return GetSetRcdDcaDfeVrefInternal (Host, Socket, Channel, Dimm, SubChannel, Signal, Mode, Value);
  }

}


/**

  Get the limits for the group or level requested. The adjustment of the group is done
  by the RCD, DB buffer or DRAM.

  @param Level     - Level to program (DDR, VMSE, Buffer)
  @param Group     - Parameter to program
  @param MinLimit  - Minimum delay value allowed
  @param MaxLimit  - Maximum absolute value allowed
  @param Delay     - Time in usec required for new setting to take effect

  @retval MRC_STATUS

**/
MRC_STATUS
GetDimmParamGroupLimits (
  IN  MRC_LT    Level,
  IN  MRC_GT    Group,
  OUT UINT16    *MinLimit,
  OUT UINT16    *MaxLimit,
  OUT UINT16    *Delay
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;

  if (Level == DdrLevel) {
    switch (Group) {
    case RcdDcsVref:
      *MinLimit = 0;
      *MaxLimit = MAX_RCD_DCS_VREF_RANGE;
      *Delay = 0;
      break;
    case DcaDfeVref:
      *MinLimit = 0;
      *MaxLimit = 2 * MAX_RCD_CA_DFE_VREF_RANGE;
      *Delay = 0;
      break;
    case RcdDcaVref:
      *MinLimit = 0;
      *MaxLimit = MAX_RCD_CA_VREF_RANGE;
      *Delay = 0;
      break;

   case DbDfeVrefDq0:
   case DbDfeVrefDq1:
   case DbDfeVrefDq2:
   case DbDfeVrefDq3:
   case DbDfeVrefDq4:
   case DbDfeVrefDq5:
   case DbDfeVrefDq6:
   case DbDfeVrefDq7:
      *MinLimit = 0;
      *MaxLimit = 2 * MAX_DB_DFE_VREF_RANGE;
      *Delay = 0;
      break;
    default:
      Status = MRC_STATUS_GROUP_NOT_SUPPORTED;
      break;
    } // switch group
#ifdef LRDIMM_SUPPORT
  } else if (Level == LrbufLevel) {
    switch (Group) {
    case QcaDelay:
      *MinLimit = 0;
      *MaxLimit = MAX_QCA_RANGE;
      *Delay = 0;
      break;
    case QcsDelay:
      *MinLimit = 0;
      *MaxLimit = MAX_QCS_RANGE;
      *Delay = 0;
      break;
    case BcomDelay:
      *MinLimit = 0;
      *MaxLimit = MAX_BCOM_RANGE;
      *Delay = 0;
      break;
    case BcomVref:
      *MinLimit = 0;
      *MaxLimit = MAX_BCOM_VREF_RANGE;
      *Delay = 0;
      break;
    case DramCaVref:
    case DramCsVref:
      *MinLimit = 0;
      *MaxLimit = MAX_DRAM_CS_CA_VREF_RANGE;
      *Delay = 0;
    default:
      Status = MRC_STATUS_GROUP_NOT_SUPPORTED;
      break;
    } // switch group
#endif
  } else {
    Status = MRC_STATUS_LEVEL_NOT_SUPPORTED;
  }

  return Status;
} // GetDimmGroupLimits

/**

  Checks to see of supplied group is a member of the "Dimm" parameter group which is adjuested
  by the RCD, DB or DRAM.

  @param group - Group item to check

  @retval TRUE  - Is a member of DIMM Group
  @retval FALSE - Is not a member of DIMM Group

**/
BOOLEAN
CheckDimmParamGroupDdr5 (
  MRC_GT  Group
  )
{
  switch (Group) {
  case RcdDcsVref:
  case DcaDfeVref:
  case RcdDcaVref:
  case QcaDelay:
  case QcsDelay:
  case BcomDelay:
  case BcomVref:
  case DbDfeVrefDq0:
  case DbDfeVrefDq1:
  case DbDfeVrefDq2:
  case DbDfeVrefDq3:
  case DbDfeVrefDq4:
  case DbDfeVrefDq5:
  case DbDfeVrefDq6:
  case DbDfeVrefDq7:
  case DramCaVref:
  case DramCsVref:
    return TRUE;
  default:
    return FALSE;
  }
}

/**

  Get set RCD DCA vref based on the provided signal number

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Signal           - Enumerated signal name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out] Value       - Value to set or get

  @retval MRC_STATUS_SUCCESS if cache entry is returned
  @retval MRC_STATUS_FAILURE if the cache entry is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented
**/
MRC_STATUS
EFIAPI
GetSetRcdDcaVref (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Channel,
  IN     UINT8     SubChannel,
  IN     UINT8     Dimm,
  IN     GSM_CSN   Signal,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  )
{
  UINT8              CwRegAddress;
  UINT8              ValueToUpdateCache = 0;
  UINT8              CaVref8;
  UINT16             CurrentCaVref;
  UINT8              NewCaVref;
  UINT8              TempCaVref;
  UINT16             MinLimit;
  UINT16             MaxLimit;
  UINT16             Delay;

  switch (Signal) {
  case A0A:
    CwRegAddress = RDIMM_RW40;
    break;
  case A1A:
    CwRegAddress = RDIMM_RW41;
    break;
  case A2A:
    CwRegAddress = RDIMM_RW42;
    break;
  case A3A:
    CwRegAddress = RDIMM_RW43;
    break;
  case A4A:
    CwRegAddress = RDIMM_RW44;
    break;
  case A5A:
    CwRegAddress = RDIMM_RW45;
    break;
  case A6A:
    CwRegAddress = RDIMM_RW46;
    break;
  case PARA:
    CwRegAddress = RDIMM_RW47;
    break;
  default:
    return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
    break;
  }

  GetDimmParamGroupLimits (DdrLevel, RcdDcaVref, &MinLimit, &MaxLimit, &Delay);

  if (Mode & GSM_READ_CSR) {
    if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
      GetSetDcaVrefDdrt2 (Host, Socket, Channel, Dimm, SubChannel, Signal, GSM_READ_CSR, &CaVref8);
    } else {
      ReadRcdSmbByteDdr5 (Host, Socket, Channel, Dimm, SubChannel, RDIMM_CW_PAGE0, CwRegAddress, &CaVref8);
    }
  } else {
    ReadRcdCwCache (Socket, Channel, SubChannel, Dimm, RDIMM_CW_PAGE0, CwRegAddress, &CaVref8);
  }
  CurrentCaVref = CaVref8;

  if (Mode & GSM_READ_ONLY) {
    *Value = CurrentCaVref;
    ValueToUpdateCache = (UINT8)*Value;
    if (Mode & GSM_UPDATE_CACHE) {
      WriteRcdCwCache (Socket, Channel, (1 << SubChannel), Dimm, RDIMM_CW_PAGE0, CwRegAddress, ValueToUpdateCache);
    }
  } else {
    if (Mode & GSM_WRITE_OFFSET) {
      TempCaVref = (UINT8) (CurrentCaVref + *Value);
    } else {
      TempCaVref = (UINT8) *Value;
    }

    if (TempCaVref >= MinLimit) {
      NewCaVref = TempCaVref;
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "Rcd DcaVref Out of range!! TempCaVref = 0x%x, MinLimit = 0x%x\n", TempCaVref, MinLimit);
      NewCaVref = (UINT8) MinLimit;
    }

    if (TempCaVref > MaxLimit) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "Rcd DcaVref Out of range!! TempCaVref = 0x%x, MaxLimit = 0x%x\n", TempCaVref, MaxLimit);
      NewCaVref = (UINT8) MaxLimit;
    }
    if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
      GetSetDcaVrefDdrt2 (Host, Socket, Channel, Dimm, SubChannel, Signal, GSM_FORCE_WRITE, &NewCaVref);
      WriteRcdCwCache (Socket, Channel, 1 << SubChannel, Dimm, RDIMM_CW_PAGE0, CwRegAddress, NewCaVref);
    } else {
      RcdControlWordWrite (Socket, Channel, (1 << SubChannel), Dimm, 0, CwRegAddress, NewCaVref);
    }
  }

  return MRC_STATUS_SUCCESS;
}

/**

  Set burst chop

  @param[in] Socket          - Socket Id
  @param[in] Ch              - Channel number (0-based)
  @param[in] Dimm            - DIMM number (0-based)
  @param[in] Rank            - Rank number (0-based)

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
SetBurstChopDdr5 (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  DDR5_MODE_REGISTER_0_STRUCT    Ddr5Mr0;

  ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_0_REG, &Ddr5Mr0.Data);
  Ddr5Mr0.Bits.burst_length |= BIT0;
  DramModeRegWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_MODE_REGISTER_0_REG, Ddr5Mr0.Data);

  ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_B, Dimm, Rank, DDR5_MODE_REGISTER_0_REG, &Ddr5Mr0.Data);
  Ddr5Mr0.Bits.burst_length |= BIT0;
  DramModeRegWrite (Socket, Ch, SUB_CH_B_MASK, Dimm, Rank, DDR5_MODE_REGISTER_0_REG, Ddr5Mr0.Data);

  return MRC_STATUS_SUCCESS;
}

/**

  Sets the lower bits [2:0] of OdtlWriteRegValue to the register encoding described in the DDR5
  Spec based on the OdtlOnWrOffset value.

  @param[in]  Socket             - Socket number
  @param[in]  Channel            - Channel number
  @param[in]  Strobe             - Strobe number
  @param[in]  OdtlOnWrOffset     - tODTLon_WR value to be programmed
  @param[out] OdtlWriteRegValue  - Pointer which stores register value to be programmed

  @retval N/A

**/
VOID
GetOdtlOnRegValue (
  IN  UINT8                         Socket,
  IN  UINT8                         Channel,
  IN  UINT8                         Strobe,
  IN  INT16                         OdtlOnWrOffset,
  OUT DDR5_MODE_REGISTER_37_STRUCT  *OdtlMr37
  )
{
  UINT8  Baseline = 1;
  UINT8  DistanceValue;

  // Make sure input value is not too high or too low
  if (OdtlOnWrOffset < MIN_ODTLON_OFFSET) {
    // Print warning and adjust value
    RcDebugPrint (SDBG_MINMAX, "ODTLon_WR_Offset -- Register Overflow! Strobe: %d, MinValue = %d, Value = %d, adjusted to MinVal.\n",
                    Strobe, MIN_ODTLON_OFFSET, OdtlOnWrOffset);
    OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    OdtlOnWrOffset = MIN_ODTLON_OFFSET;
  } else if (OdtlOnWrOffset > MAX_ODTLON_OFFSET) {
    // Print warning and adjust value
    RcDebugPrint (SDBG_MINMAX, "ODTLon_WR_Offset -- Register Overflow! Strobe: %d, MaxValue = %d, Value = %d, adjusted to MaxVal.\n",
                    Strobe, MAX_ODTLON_OFFSET, OdtlOnWrOffset);
    OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    OdtlOnWrOffset = MAX_ODTLON_OFFSET;
  }

  // A value of -4 (minimum possible value) equals a register value of 1. -3 equals a register value of 2.
  // -2 equals 3 and so on until +2 equals a register value of 7.  So to find the register value, we'll find
  // the distance the input value is from -4 and add it to the baseline.

  DistanceValue = (UINT8) (OdtlOnWrOffset - MIN_ODTLON_OFFSET);
  OdtlMr37->Bits.odtlon_wr_offset = Baseline + DistanceValue;
}

/**

  Sets the upper bits [5:3] of OdtlWriteRegValue to the register encoding described in the DDR5
  Spec based on the OdtlOffWrOffset value.

  @param[in]  Socket             - Socket number
  @param[in]  Channel            - Channel number
  @param[in]  Strobe             - Strobe number
  @param[in]  OdtlOnWrOffset     - tODTLoff_WR value to be programmed
  @param[out] OdtlWriteRegValue  - Pointer which stores register value to be programmed

  @retval N/A

**/
VOID
GetOdtlOffRegValue (
  IN  UINT8                         Socket,
  IN  UINT8                         Channel,
  IN  UINT8                         Strobe,
  IN  INT16                         OdtlOffWrOffset,
  OUT DDR5_MODE_REGISTER_37_STRUCT  *OdtlMr37
  )
{
  UINT8  Baseline = 1;
  UINT8  DistanceValue;

  // Make sure input value is not too high or too low
  if (OdtlOffWrOffset < MIN_ODTLOFF_OFFSET) {
    // Print warning and adjust value
    RcDebugPrint (SDBG_MINMAX, "ODTLoff_WR_Offset -- Register Overflow! Strobe: %d, MinValue = %d, Value = %d, adjusted to MinVal.\n",
                    Strobe, MIN_ODTLOFF_OFFSET, OdtlOffWrOffset);
    OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    OdtlOffWrOffset = MIN_ODTLOFF_OFFSET;
  } else if (OdtlOffWrOffset > MAX_ODTLOFF_OFFSET) {
    // Print warning and adjust value
    RcDebugPrint (SDBG_MINMAX, "ODTLoff_WR_Offset -- Register Overflow! Strobe: %d, MaxValue = %d, Value = %d, adjusted to MaxVal.\n",
                    Strobe, MAX_ODTLOFF_OFFSET, OdtlOffWrOffset);
    OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    OdtlOffWrOffset = MAX_ODTLOFF_OFFSET;
  }

  // A value of 4 (maximum possible value) equals a register value of 1. 3 equals a register value of 2.
  // 2 equals 3 and so on until -2 equals a register value of 7.  So to find the register value, we'll find
  // the distance the input value is from 4 and add it to the baseline.

  DistanceValue = (UINT8) (MAX_ODTLOFF_OFFSET - OdtlOffWrOffset);
  OdtlMr37->Bits.odtloff_wr_offset = Baseline + DistanceValue;
}

/**

  Programs the ODT Latency On Write offset control word in the data buffer.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] SubCh           - SubChannel number
  @param[in] Dimm            - DIMM number
  @param[in] Rank            - Rank number on DIMM
  @param[in] Strobe          - Strobe number
  @param[in] OdtlOnWrOffset  - ODT Latency value to be programmed

  @retval N/A

**/
VOID
ProgramDbOdtlOn (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     SubCh,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Strobe,
  IN INT16     OdtlOnWrOffset
  )
{
  UINT8                             SubChMSVx4;
  UINT8                             MaxStrobePairs;
  UINT8                             DataBuffer;
  UINT8                             Baseline = 1;
  UINT8                             DistanceValue;
  UINT8                             OdtlOnReg;
  UINT8                             OdtlOnRegValue;
  DDR5_DATA_BUFFER_PGB_RWE0_STRUCT  OdtlOn;

  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxStrobePairs = SubChMSVx4 / 2;
  // Select data buffer based on strobe (Strobe 0,5 - DB0, Strobe 1,6 - DB1...)
  DataBuffer = Strobe % 5;

  // Each rank's values are programmed independently.
  if (Rank == 0) {
    OdtlOnReg = DDR5_DATA_BUFFER_PGB_RWE0_REG;
  } else {
    OdtlOnReg = DDR5_DATA_BUFFER_PGB_RWE1_REG;
  }

  ReadDbCwCache (Socket, Channel, SubCh, Dimm, DataBuffer, LRDIMM_DB_PAGEB, OdtlOnReg, &OdtlOn.Data);

  // Make sure input value is not too high or too low
  if (OdtlOnWrOffset < MIN_ODTLON_OFFSET) {
    // Print warning and adjust value
    RcDebugPrint (SDBG_MINMAX, "ODTLon_WR_Offset -- Register Overflow! Strobe: %d, MinValue = %d, Value = %d, adjusted to MinVal.\n",
                    Strobe, MIN_ODTLON_OFFSET, OdtlOnWrOffset);
    OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    OdtlOnWrOffset = MIN_ODTLON_OFFSET;
  } else if (OdtlOnWrOffset > MAX_ODTLON_OFFSET) {
    // Print warning and adjust value
    RcDebugPrint (SDBG_MINMAX, "ODTLon_WR_Offset -- Register Overflow! Strobe: %d, MaxValue = %d, Value = %d, adjusted to MaxVal.\n",
                    Strobe, MAX_ODTLON_OFFSET, OdtlOnWrOffset);
    OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    OdtlOnWrOffset = MAX_ODTLON_OFFSET;
  }

  // A value of -4 (minimum possible value) equals a register value of 1. -3 equals a register value of 2.
  // -2 equals 3 and so on until +2 equals a register value of 7.  So to find the register value, we'll find
  // the distance the input value is from -4 and add it to the baseline.

  DistanceValue = (UINT8) (OdtlOnWrOffset - MIN_ODTLON_OFFSET);
  OdtlOnRegValue = Baseline + DistanceValue;

  // Select data buffer to program
  DbControlWordWrite (Host, Socket, Channel, (1 << SubCh), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, DataBuffer);
  // Select page B for ODT latency programming
  DbControlWordWrite (Host, Socket, Channel, (1 << SubCh), Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGEB);

  // First 5 strobes represent lower nibble, last 5 upper nibble
  if (Strobe < MaxStrobePairs) {
    OdtlOn.Bits.odtlon_wr_n0_r1_offset_the_offset = OdtlOnRegValue;
  } else {
    OdtlOn.Bits.odtlon_wr_n1_r1_offset_the_offset = OdtlOnRegValue;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubCh, Rank, Strobe, NO_BIT,
        "OdtlOnWrOffset:%3d  BCW PG[B]%x: 0x%02x\n", OdtlOnWrOffset, OdtlOnReg, OdtlOn.Data);
  DbControlWordWrite (Host, Socket, Channel, (1 << SubCh), Dimm, Rank, OdtlOnReg, OdtlOn.Data);

  // Select all data buffers now that programming is done
  DbControlWordWrite (Host, Socket, Channel, (1 << SubCh), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
}

/**

  Programs the ODT Latency Off Write offset control word in the data buffer.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubCh            - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number on DIMM
  @param[in] Strobe           - Strobe number
  @param[in] OdtlOffWrOffset  - ODT Latency value to be programmed

  @retval N/A

**/
VOID
ProgramDbOdtlOff (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     SubCh,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Strobe,
  IN INT16     OdtlOffWrOffset
  )
{
  UINT8                             SubChMSVx4;
  UINT8                             MaxStrobePairs;
  UINT8                             DataBuffer;
  UINT8                             Baseline = 1;
  UINT8                             DistanceValue;
  UINT8                             OdtlOffReg;
  UINT8                             OdtlOffRegValue;
  DDR5_DATA_BUFFER_PGB_RWE2_STRUCT  OdtlOff;

  SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  MaxStrobePairs = SubChMSVx4 / 2;
  // Select data buffer based on strobe (Strobe 0,5 - DB0, Strobe 1,6 - DB1...)
  DataBuffer = Strobe % 5;

  // Each rank's values are programmed independently.
  if (Rank == 0) {
    OdtlOffReg = DDR5_DATA_BUFFER_PGB_RWE2_REG;
  } else {
    OdtlOffReg = DDR5_DATA_BUFFER_PGB_RWE3_REG;
  }

  ReadDbCwCache (Socket, Channel, SubCh, Dimm, DataBuffer, LRDIMM_DB_PAGEB, OdtlOffReg, &OdtlOff.Data);

  // Make sure input value is not too high or too low
  if (OdtlOffWrOffset < MIN_ODTLOFF_OFFSET) {
    // Print warning and adjust value
    RcDebugPrint (SDBG_MINMAX, "ODTLoff_WR_Offset -- Register Overflow! Strobe: %d, MinValue = %d, Value = %d, adjusted to MinVal.\n",
                    Strobe, MIN_ODTLOFF_OFFSET, OdtlOffWrOffset);
    OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    OdtlOffWrOffset = MIN_ODTLOFF_OFFSET;
  } else if (OdtlOffWrOffset > MAX_ODTLOFF_OFFSET) {
    // Print warning and adjust value
    RcDebugPrint (SDBG_MINMAX, "ODTLoff_WR_Offset -- Register Overflow! Strobe: %d, MaxValue = %d, Value = %d, adjusted to MaxVal.\n",
                    Strobe, MAX_ODTLOFF_OFFSET, OdtlOffWrOffset);
    OutputWarning (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW, Socket, Channel, NO_DIMM, NO_RANK);
    OdtlOffWrOffset = MAX_ODTLOFF_OFFSET;
  }

  // A value of 4 (maximum possible value) equals a register value of 1. 3 equals a register value of 2.
  // 2 equals 3 and so on until -2 equals a register value of 7.  So to find the register value, we'll find
  // the distance the input value is from 4 and add it to the baseline.

  DistanceValue = (UINT8) (MAX_ODTLOFF_OFFSET - OdtlOffWrOffset);
  OdtlOffRegValue = Baseline + DistanceValue;

  // Select data buffer to program
  DbControlWordWrite (Host, Socket, Channel, (1 << SubCh), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, DataBuffer);
  // Select page B for ODT latency programming
  DbControlWordWrite (Host, Socket, Channel, (1 << SubCh), Dimm, Rank, RDIMM_RWDF, LRDIMM_DB_PAGEB);

  // First 5 strobes represent lower nibble, last 5 upper nibble
  if (Strobe < MaxStrobePairs) {
    OdtlOff.Bits.odtloff_wr_n0_r1_offset_the_offset = OdtlOffRegValue;
  } else {
    OdtlOff.Bits.odtloff_wr_n1_r1_offset_the_offset = OdtlOffRegValue;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubCh, Rank, Strobe, NO_BIT,
    "OdtlOffWrOffset:%3d  BCW PG[B]%x: 0x%02x\n", OdtlOffWrOffset, OdtlOffReg, OdtlOff.Data);
  DbControlWordWrite (Host, Socket, Channel, (1 << SubCh), Dimm, Rank, OdtlOffReg, OdtlOff.Data);

  // Select all data buffers now that programming is done
  DbControlWordWrite (Host, Socket, Channel, (1 << SubCh), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, ALL_DATABUFFERS);
}

/**

  Calculates and programs ODTLon_WR_Offset and ODTLoff_WR_Offset values.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number on DIMM

  @retval N/A

**/
VOID
CalculateAndProgramDramOdtlValues (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     SubChannel,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8                         Strobe;
  UINT8                         PerChStrobe;
  INT16                         DqDelay[MAX_BITS_IN_BYTE];
  INT16                         MinTxDqsDq;
  INT16                         MaxTxDqsDq;
  INT16                         OdtlOnWrOffset;
  INT16                         OdtlOffWrOffset;
  DDR5_MODE_REGISTER_37_STRUCT  OdtlMr37;
  UINT8                         MaxStrobe;
  INT16                         TxDqsDly = 0;
  INT16                         TxDqDly[MAX_BITS_IN_BYTE];
  UINT8                         SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                         SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;


  if (IsX4Dimm (Socket, Channel, Dimm)) {
    MaxStrobe = SubChMSVx4;
  } else {
    MaxStrobe = SubChMSVx8;
  }

  OdtlMr37.Data = 0;

  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    PerChStrobe = Strobe + SubChannel * MaxStrobe;
    OdtlOnWrOffset = 0;
    OdtlOffWrOffset = 0;
    // a. Tx_DQS2DQ_DQ0: txdq_dly0 - txdqs_dly
    // b. Tx_DQS2DQ_DQ1: txdq_dly1 - txdqs_dly
    // c. Tx_DQS2DQ_DQ2: txdq_dly2 - txdqs_dly
    // d. Tx_DQS2DQ_DQ3: txdq_dly3 - txdqs_dly

    GetSetDataGroup (Host, Socket, Channel, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, &TxDqsDly);
    GetSetDataGroup (Host, Socket, Channel, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, &TxDqDly[0]);
    GetSetDataGroup (Host, Socket, Channel, SubChannel, Dimm, Rank, Strobe, 1, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, &TxDqDly[1]);
    GetSetDataGroup (Host, Socket, Channel, SubChannel, Dimm, Rank, Strobe, 2, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, &TxDqDly[2]);
    GetSetDataGroup (Host, Socket, Channel, SubChannel, Dimm, Rank, Strobe, 3, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, &TxDqDly[3]);

    DqDelay[0] = TxDqDly[0] - TxDqsDly;
    DqDelay[1] = TxDqDly[1] - TxDqsDly;
    DqDelay[2] = TxDqDly[2] - TxDqsDly;
    DqDelay[3] = TxDqDly[3] - TxDqsDly;

    // Min_Tx_DQS2DQ = MIN (Tx_DQS2DQ_DQ0, Tx_DQS2DQ_DQ1, Tx_DQS2DQ_DQ2, Tx_DQS2DQ_DQ3)
    // Max_Tx_DQS2DQ = MAX (Tx_DQS2DQ_DQ0, Tx_DQS2DQ_DQ1, Tx_DQS2DQ_DQ2, Tx_DQS2DQ_DQ3)

    MinTxDqsDq = MIN (DqDelay[0], DqDelay[1]);
    MinTxDqsDq = MIN (MinTxDqsDq, DqDelay[2]);
    MinTxDqsDq = MIN (MinTxDqsDq, DqDelay[3]);
    MaxTxDqsDq = MAX (DqDelay[0], DqDelay[1]);
    MaxTxDqsDq = MAX (MaxTxDqsDq, DqDelay[2]);
    MaxTxDqsDq = MAX (MaxTxDqsDq, DqDelay[3]);

    if (IsX4Dimm (Socket, Channel, Dimm)) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, Strobe, NO_BIT,
        "TxDqsDly:%3d  TxDqDly0:%3d  TxDqDly1:%03d  TxDqDly2:%3d  TxDqDly3:%3d  MinTxDqsDq:%3d  MaxTxDqsDq:%3d \n",
         TxDqsDly, TxDqDly[0], TxDqDly[1], TxDqDly[2], TxDqDly[3], MinTxDqsDq, MaxTxDqsDq);
    } else {

      //x8 DIMM, so need to consider the whole byte
      GetSetDataGroup (Host, Socket, Channel, SubChannel, Dimm, Rank, Strobe + SubChMSVx8, 0, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, &TxDqDly[4]);
      GetSetDataGroup (Host, Socket, Channel, SubChannel, Dimm, Rank, Strobe + SubChMSVx8, 1, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, &TxDqDly[5]);
      GetSetDataGroup (Host, Socket, Channel, SubChannel, Dimm, Rank, Strobe + SubChMSVx8, 2, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, &TxDqDly[6]);
      GetSetDataGroup (Host, Socket, Channel, SubChannel, Dimm, Rank, Strobe + SubChMSVx8, 3, DdrLevel, TxDqBitDelay, GSM_READ_ONLY, &TxDqDly[7]);

      DqDelay[4] = TxDqDly[4] - TxDqsDly;
      DqDelay[5] = TxDqDly[5] - TxDqsDly;
      DqDelay[6] = TxDqDly[6] - TxDqsDly;
      DqDelay[7] = TxDqDly[7] - TxDqsDly;

      // Min_Tx_DQS2DQ = MIN (Tx_DQS2DQ_DQ0, ... DQ7)
      // Max_Tx_DQS2DQ = MAX (Tx_DQS2DQ_DQ0, ... DQ7)

      MinTxDqsDq = MIN (MinTxDqsDq, DqDelay[4]);
      MinTxDqsDq = MIN (MinTxDqsDq, DqDelay[5]);
      MinTxDqsDq = MIN (MinTxDqsDq, DqDelay[6]);
      MinTxDqsDq = MIN (MinTxDqsDq, DqDelay[7]);
      MaxTxDqsDq = MAX (MaxTxDqsDq, DqDelay[4]);
      MaxTxDqsDq = MAX (MaxTxDqsDq, DqDelay[5]);
      MaxTxDqsDq = MAX (MaxTxDqsDq, DqDelay[6]);
      MaxTxDqsDq = MAX (MaxTxDqsDq, DqDelay[7]);

      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, Strobe, NO_BIT,
        "TxDqsDly:%3d  TxDqDly0:%3d  TxDqDly1:%03d  TxDqDly2:%3d  TxDqDly3:%3d  TxDqDly4:%3d  TxDqDly5:%03d  TxDqDly6:%3d  TxDqDly7:%3d \n",
         TxDqsDly, TxDqDly[0], TxDqDly[1], TxDqDly[2], TxDqDly[3], TxDqDly[4], TxDqDly[5], TxDqDly[6], TxDqDly[7]);
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, Strobe, NO_BIT,
        "MinTxDqsDq:%3d  MaxTxDqsDq:%3d \n",
         MinTxDqsDq, MaxTxDqsDq);
    }
    // 2. Perform the following calculations to obtain the ODTLon_WR_offset:
    //  ODTLon_WR_offset _ticks = tDQSoffset_max * 128 +
    //                            tADC_max * 128 +
    //                            Min_Tx_DQS2DQ +
    //                            Tx_DQS2DQ_Drift_Guardband
    // Where:
    //  tDQSoffset_max = -0.5 (or the max delta across all strobes between external WL and internal WL)
    //  tADC_max = -0.8
    //  Tx_DQS2DQ_Drift_Guardband = -10 ticks Note: Guardband can be tuned based on Post-Silicon Characterization.
    // ODTLon_WR_offset = RoundUP(ODTLon_WR_offset / 128)

    // Factors are multiplied by 10 to better account for rounding
    OdtlOnWrOffset =  ((128 * ODTON_TDQSOFFSET_MAX) + (128 * TADC_MAX) + (MinTxDqsDq * 10) + ODTON_DRIFT_GUARD);

    // Round up if value is not a factor of 128 x 10 and number is positive.
    if ((OdtlOnWrOffset % 1280 != 0) && (OdtlOnWrOffset > 0)) {
      OdtlOnWrOffset += 1280;
    }
    //negative numbers must be rounded down
    if ((OdtlOnWrOffset % 1280 != 0) && (OdtlOnWrOffset < 0)) {
      OdtlOnWrOffset -= 1280;
    }

    // Final rounded value
    OdtlOnWrOffset = OdtlOnWrOffset / 1280;

    // If there is a data buffer present, program the ODT Latency On in the data buffer, not the DRAM
    if (IsLrdimmPresent (Socket, Channel, Dimm) || IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
      ProgramDbOdtlOn (Host, Socket, Channel, SubChannel, Dimm, Rank, Strobe, OdtlOnWrOffset);
    } else {
      // Get register programming (OdtlOn and OdtlOff are programmed in the same register for RDIMM)
      GetOdtlOnRegValue (Socket, Channel, PerChStrobe, OdtlOnWrOffset, &OdtlMr37);
    }

    // 3. Perform the following calculations to obtain the ODTLoff_WR_offset:
    //    ODTLoff_WR_offset _ticks = tDQSoffset_max * 128 +
    //                               Max_Tx_DQS2DQ +
    //                               Tx_DQS2DQ_Drift_Guardband
    // Where:
    //  tDQSoffset_max = +0.5 (or the max delta across all strobes between external WL and internal WL)
    //  Tx_DQS2DQ_Drift_Guardband = +20 ticks Note: Guardband can be tuned based on Post-Silicon Characterization.
    // ODTLoff_WR_offset = RoundUP(ODTLoff_WR_offset / 128)
    OdtlOffWrOffset = ((128 * ODTOFF_TDQSOFFSET_MAX) + (MaxTxDqsDq * 10) + ODTOFF_DRIFT_GUARD);

    // Round up if value is not a factor of 128 x 10 and number is positive.
    if ((OdtlOffWrOffset % 1280 != 0) && (OdtlOffWrOffset > 0)) {
      OdtlOffWrOffset += 1280;
    }
    // Negative numbers will be rounded down
    if ((OdtlOffWrOffset % 1280 != 0) && (OdtlOffWrOffset < 0)) {
      OdtlOffWrOffset -= 1280;
    }

    // Final rounded value
    OdtlOffWrOffset = OdtlOffWrOffset / 1280;

    // If there is a data buffer present, program the ODT Latency Off in the data buffer, not the DRAM
    if (IsLrdimmPresent (Socket, Channel, Dimm) || IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
      ProgramDbOdtlOff (Host, Socket, Channel, SubChannel, Dimm, Rank, Strobe, OdtlOffWrOffset);
    } else {
      GetOdtlOffRegValue (Socket, Channel, PerChStrobe, OdtlOffWrOffset, &OdtlMr37);

      RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, Strobe, NO_BIT,
        "OdtlOnWrOffset:%3d OdtlOffWrOffset:%3d  MR37:0x%02x \n",
         OdtlOnWrOffset, OdtlOffWrOffset, OdtlMr37.Data);

      DramModeRegWritePda (Socket, Channel, (1 << SubChannel), Dimm, Rank, Strobe, DDR5_MODE_REGISTER_37_REG, OdtlMr37.Data);
    }

  }

}


/**
  Configure the data buffer DQ pass-through mode for read.

  @param[in] Socket     - Socket Id
  @param[in] Channel    - Channel number (0-based)
  @param[in] Dimm       - DIMM number (0-based)
  @param[in] Rank       - Rank number (0-based)
  @param[in] Enable     - TRUE: Enter DB DQ pass-through mode for read.
                          FALSE: Exit DB DQ pass-through mode.

  @retval N/A

**/
VOID
SetDbDqPassThroughModeForRead (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN BOOLEAN  Enable
  )
{
  PSYSHOST                                            Host;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT         ParityAndAlert;
  DDR5_DATA_BUFFER_RW93_STRUCT                        DataBufferRw93;
  DDR5_DATA_BUFFER_RW82_STRUCT                        DataBufferRw82;

  Host = GetSysHostPointer ();

  if (IsLrdimmPresent (Socket, Channel, Dimm) != 1) {
    return;
  }

  //
  // Block commands to DRAM, forward commands to DB
  //
  ReadRcdCwCache (Socket, Channel, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, &ParityAndAlert.Data);

#ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.dram_interface_forward_all_cmds = 0;
  ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.dram_interface_block_all_cmds = 0;
  ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
  RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

  //
  // Select all data buffers
  //
  DataBufferRw93.Data = 0;
  DataBufferRw93.Bits.pba_select_id = PBA_SELECT_ALL;
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, DataBufferRw93.Data);

  //
  // Enable/Disable DB in DQ pass-through mode for reads
  //
  DataBufferRw82.Data = 0;
  if (Enable) {
    DataBufferRw82.Bits.dq_pass_through_mode = 1;
    DataBufferRw82.Bits.dq_pass_through_mode_direction_select = 1;
  }
  DbControlWordWrite (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW82_REG, DataBufferRw82.Data);

  //
  // Block commands to DB, forward commands to DRAM
  //
#ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
  ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 0;
#else // #ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
  ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 0;
#endif // #ifdef USE_LATEST_RCD_SPEC
  RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);
}

/*

  This function Gets or Sets the BCOM signals

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number
  @param[in] Signal           - Enumerated signal name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

*/
MRC_STATUS
EFIAPI
GetSetBcomDelay (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     SubChannel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  INT16       CurVal = 0;
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  GSM_CSN     Signal;
  UINT16      MinLimit;
  UINT16      MaxLimit;
  UINT16      Delay;
  DDR5_RCD_BCS_N_AND_BCOM_2_0_OUTPUT_DELAY_STRUCT BcomOutputDelay;

  GetDimmParamGroupLimits (LrbufLevel, BcomDelay, &MinLimit, &MaxLimit, &Delay);

  if (SubChannel == SUB_CH_A) {
    Signal = BCOM_A;
  } else {
    Signal = BCOM_B;
  }

  ReadRcdCwCache (Socket, Channel, SubChannel, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_BCS_N_AND_BCOM_2_0_OUTPUT_DELAY_REG, &BcomOutputDelay.Data);

  CurVal = BcomOutputDelay.Bits.output_delay_control_for_bcs_n_and_bcom_2_0_output_signals_1_2;

#ifdef USE_LATEST_RCD_SPEC
  if (BcomOutputDelay.Bits.full_cycle_delay == 1) {
    CurVal += 64;
  }
#else
  if (BcomOutputDelay.Bits.full_cycle_delay_1 == 1) {
    CurVal += 64;
  }
#endif

  if (Mode & GSM_READ_ONLY) {

    *Value = CurVal;

  } else {

    if (Mode & GSM_WRITE_OFFSET) {
      CurVal = CurVal + *Value;
    } else {
      CurVal = *Value;
    }

    if (CurVal < MinLimit) {
      RcDebugPrintWithDevice (
        SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
        "%a Out of range!! CurVal = %d, MinLimit = %d\n",
        GetSignalStr (Signal), CurVal, MinLimit
      );
      CurVal = BCOM_MIN_VAL;
      Status = MRC_STATUS_FAILURE;
    }
    if (CurVal > MaxLimit) {
      RcDebugPrintWithDevice (
        SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
        "%a Out of range!! CurVal = %d, MaxLimit = %d\n",
        GetSignalStr (Signal), CurVal, MaxLimit
      );
      CurVal = BCOM_MAX_VAL;
      Status = MRC_STATUS_FAILURE;
    }

    ProgramBcomDelay (Host, Socket, Channel, Dimm, (1 << SubChannel), Rank, GSM_FORCE_WRITE | GSM_UPDATE_CACHE, (UINT8)CurVal, TRUE);
  } // Mode == GSM_READ_ONLY

  return Status;
}

/**

  Assert/Deassert DB BRST_n.

  @param[in] Socket         - Socket number
  @param[in] ChannelBitMask - Bitmask of channels to reset
  @param[in] IsAssert       - Assert\Deassert BRST_n for DB

  @retval  MRC_STATUS_SUCCESS

**/
MRC_STATUS
SetDbBrstN (
  IN UINT8     Socket,
  IN UINT32    ChannelBitMask,
  IN BOOLEAN   IsAssert
  )
{
  PSYSHOST                      Host;
  UINT8                         Channel;
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         MaxChDdr;

  DDR5_RCD_COMMAND_SPACE_STRUCT CommandSpaceA;
  DDR5_RCD_COMMAND_SPACE_STRUCT CommandSpaceB;

  Host               = GetSysHostPointer ();
  MaxChDdr           = GetMaxChDdr ();
  Rank               = 0;
  CommandSpaceA.Data = 0;
  CommandSpaceB.Data = 0;
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }
    if ((ChannelBitMask & (1 << Channel)) == 0) {
      continue;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsLrdimmPresent (Socket, Channel, Dimm) == 0) {
        continue;
      }
      //
      // Skip for DDRT
      //
      if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
        continue;
      }
      if (IsAssert) {
        CommandSpaceA.Bits.command = RCD_COMMAND_CH_A_DB_RESET; // Set BRST_A low
        CommandSpaceB.Bits.command = RCD_COMMAND_CH_B_DB_RESET; // Set BRST_B low
      } else {
        CommandSpaceA.Bits.command = RCD_COMMAND_CLEAR_CH_A_DB_RESET; // Set BRST_A high
        CommandSpaceB.Bits.command = RCD_COMMAND_CLEAR_CH_B_DB_RESET; // Set BRST_B high
      }
      RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_COMMAND_SPACE_REG, CommandSpaceA.Data);
      FixedDelayMicroSecond (2);
      RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_COMMAND_SPACE_REG, CommandSpaceB.Data);
    }
 }
  return MRC_STATUS_SUCCESS;
} // SetDbBrstN

/**

  Take DB in or out of reset.

  @param[in] Socket         - Socket number
  @param[in] ChannelBitMask - Bitmask of channels to reset
  @param[in] IsAssert       - Assert/Deassert BRST_n for DB

  @retval  MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
SetDbReset (
  IN UINT8     Socket,
  IN UINT32    ChannelBitMask,
  IN BOOLEAN   IsAssert
  )
{
#ifdef LRDIMM_SUPPORT
  PSYSHOST                      Host;
  UINT8                         Channel;
  UINT8                         SubChannel;
  UINT8                         Dimm;
  UINT8                         Rank;
  UINT8                         MaxChDdr;
  DDR5_RCD_OUTPUT_ADDRESS_AND_CONTROL_ENABLE_STRUCT  OutputEnable;

  Host      = GetSysHostPointer ();
  MaxChDdr  = GetMaxChDdr ();
  Rank      = 0;

  if (IsAssert == FALSE) {
    SetDbBrstN (Socket, ChannelBitMask, DB_BRST_DEASSERT); // Deassert DB BRST_n (Set HIGH)
    FixedDelayMicroSecond (2); // Delay tStrap_Hold (32 tCK)
  }

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }
    if ((ChannelBitMask & (1 << Channel)) == 0) {
      continue;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsLrdimmPresent (Socket, Channel, Dimm) == 0) {
        continue;
      }
      // Skip for DDRT
      if (IsDcpmmPresentDimm (Socket, Channel, Dimm)) {
        continue;
      }
      if (IsAssert) {
        //
        // Set Bit[3] as 0 to enable BCS_n , BCOM[2:0] and BRST_n outputs for LRDIMM
        //
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ReadRcdSmbByteDdr5 (Host, Socket, Channel, Dimm, SubChannel, 0, DDR5_RCD_OUTPUT_ADDRESS_AND_CONTROL_ENABLE_REG, &OutputEnable.Data);
          if (OutputEnable.Bits.bcs_n_bcom_2_0_brst_n_outputs != 0) {
            OutputEnable.Bits.bcs_n_bcom_2_0_brst_n_outputs = 0;
            RcdControlWordWrite (Socket, Channel, (1 << SubChannel), Dimm, Rank, DDR5_RCD_OUTPUT_ADDRESS_AND_CONTROL_ENABLE_REG, OutputEnable.Data);
          }
        } // SubChannel loop
        FixedDelayMicroSecond (2);
        SetBcomMode (Socket, ChannelBitMask, Dimm, Rank, BcomResetMode); // Set BCOM[2:0] and BCS_n as HIGH before assert BRST_n
      } else {
        SetBcomMode (Socket, ChannelBitMask, Dimm, Rank, BcomNormalMode); // Set BCOM[2:0] and BCS_n as LOW after deassert BRST_n
      }
    } // Dimm loop
  } // Channel loop

  if (IsAssert) {
    FixedDelayMicroSecond (2); // Delay tStrap_Setup (32 tCK)
    SetDbBrstN (Socket, ChannelBitMask, DB_BRST_ASSERT); // Assert DB BRST_n (set LOW)
  }
#endif // LRDIMM_SUPPORT

  return MRC_STATUS_SUCCESS;
} // SetDbReset

/*
  Increase BCOM Vref encoded value in DB PG[2]RWFA by 2, which
  deceases BCOM Vref by 1%.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number

  @retval    None
*/
VOID
IncraseBcomVrefBy2 (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     SubChannel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank
  )
{
  DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_STRUCT DbInterfaceTrainingModes;
  DDR5_DATA_BUFFER_PG2_RWFA_STRUCT                        BVref;
  UINT8                                                   Strobe;

  //
  // Use RCD static control value to decrease BCOM Vref by 1%.
  //
  DbInterfaceTrainingModes.Data = 0;
  DbInterfaceTrainingModes.Bits.static_control_value_for_bcom2 = 1;
  DbInterfaceTrainingModes.Bits.static_control_value_for_bcom1 = 1;
  DbInterfaceTrainingModes.Bits.static_control_value_for_bcom0 = 0;
  DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 1;
  RcdControlWordWrite (
    Socket,
    Channel,
    SUB_CH_A_MASK,
    Dimm,
    Rank,
    DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG,
    DbInterfaceTrainingModes.Data
    );
  ToggleBReset (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank);

  //
  // Manually update cache of DB PG[2]RWFA (BCOM Vref).
  // 1% BCOM vref decrease equals to BCOM_VREF_STEP_SIZE_ENCODED_VALUE increase in register.
  //
  ReadDbCwCache (Socket, Channel, SubChannel, Dimm, 0, LRDIMM_DB_PAGE2, DDR5_DATA_BUFFER_PG2_RWFA_REG, &BVref.Data);
  BVref.Bits.B_Vref = BVref.Bits.B_Vref + BCOM_VREF_STEP_SIZE_ENCODED_VALUE;
  for (Strobe = 0; Strobe < GetMaxStrobeWithinSubCh(Host, Socket, Channel, Dimm, BcomVref, LrbufLevel); Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }

    WriteDbCwCache (Socket, Channel, SubChannel, Dimm, Strobe, LRDIMM_DB_PAGE2, DDR5_DATA_BUFFER_PG2_RWFA_REG, BVref.Data);
  } // Strobe
}

/*
  Decrease BCOM Vref encoded value in DB PG[2]RWFA by 2, which
  increases BCOM Vref by 1%. This function also update cache of
  PG[2]RWFA.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number

  @retval    None
*/
VOID
DecreaseBcomVrefBy2 (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     SubChannel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank
  )
{
  DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_STRUCT DbInterfaceTrainingModes;
  DDR5_DATA_BUFFER_PG2_RWFA_STRUCT                        BVref;
  UINT8                                                   Strobe;

  //
  // Use RCD static control value to increase BCOM Vref by 1%.
  //
  DbInterfaceTrainingModes.Data = 0;
  DbInterfaceTrainingModes.Bits.static_control_value_for_bcom2 = 1;
  DbInterfaceTrainingModes.Bits.static_control_value_for_bcom1 = 0;
  DbInterfaceTrainingModes.Bits.static_control_value_for_bcom0 = 1;
  DbInterfaceTrainingModes.Bits.static_bcom_control_enable = 1;
  RcdControlWordWrite (
    Socket,
    Channel,
    SUB_CH_A_MASK,
    Dimm,
    Rank,
    DDR5_RCD_DRAM_DB_INTERFACE_TRAINING_MODES_GLOBAL_REG,
    DbInterfaceTrainingModes.Data
    );
  ToggleBReset (Host, Socket, Channel, (1 << SubChannel), Dimm, Rank);

  //
  // Manually update cache of DB PG[2]RWFA (BCOM Vref). 1% BCOM vref increase equals to BCOM_VREF_STEP_SIZE_ENCODED_VALUE decrease in register.
  //
  ReadDbCwCache (Socket, Channel, SubChannel, Dimm, 0, LRDIMM_DB_PAGE2, DDR5_DATA_BUFFER_PG2_RWFA_REG, &BVref.Data);
  BVref.Bits.B_Vref = BVref.Bits.B_Vref - BCOM_VREF_STEP_SIZE_ENCODED_VALUE;
  for (Strobe = 0; Strobe < GetMaxStrobeWithinSubCh(Host, Socket, Channel, Dimm, BcomVref, LrbufLevel); Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }

    WriteDbCwCache (Socket, Channel, SubChannel, Dimm, Strobe, LRDIMM_DB_PAGE2, DDR5_DATA_BUFFER_PG2_RWFA_REG, BVref.Data);
  } // Strobe
}


/*

  This function Gets or Sets the BCOM signals.
  It assumes all DB in the same DIMM use the same BCOM Vref.
  This function also update cache of PG[2]RWFA.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] SubChannel       - SubChannel number
  @param[in] Dimm             - DIMM number
  @param[in] Rank             - Rank number
  @param[in] Strobe           - DB index
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program into DB PG[2]RWFA.
                                When Mode is GSM_WRITE_OFFSET, the offset step size is 2,
                                which is 1% Vdd.

*/
MRC_STATUS
EFIAPI
GetSetBcomVref (
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     SubChannel,
  IN      UINT8     Dimm,
  IN      UINT8     Rank,
  IN      UINT8     Strobe,
  IN      UINT8     Mode,
  IN OUT  INT16     *Value
  )
{
  INT16       CurVal = 0;
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  GSM_CSN     Signal;
  UINT16      MinLimit;
  UINT16      MaxLimit;
  UINT16      Delay;
  UINT8       Step;
  UINT8       Index;
  BOOLEAN     ToIncrease;
  DDR5_DATA_BUFFER_PG2_RWFA_STRUCT   BVref;
  PSYSHOST    Host;

  Host = GetSysHostPointer ();

  if (Strobe != ALL_STROBES) {
    //
    // This function assumes all DB in the same rank use the same BCOM Vref.
    //
    return MRC_STATUS_FAILURE;
  }

  GetDimmParamGroupLimits (LrbufLevel, BcomVref, &MinLimit, &MaxLimit, &Delay);

  if (SubChannel == SUB_CH_A) {
    Signal = BCOM_A;
  } else {
    Signal = BCOM_B;
  }

  ToIncrease = TRUE;

  //
  // Reading from DB CW (GSM_READ_CSR mode) is not ready yet. We have to use cached value.
  // And all storbes in a rank use the same BCOM Vref value.
  //
  ReadDbCwCache (Socket, Channel, SubChannel, Dimm, 0, LRDIMM_DB_PAGE2, DDR5_DATA_BUFFER_PG2_RWFA_REG, &BVref.Data);

  CurVal = BVref.Bits.B_Vref;

  if (Mode & GSM_READ_ONLY) {

    *Value = CurVal;

  } else {

    if (Mode & GSM_WRITE_OFFSET) {

      //
      // Calculate steps number of decrease or increase
      //
      Step = ((UINT8)ABS(*Value)) / BCOM_VREF_STEP_SIZE_ENCODED_VALUE;
      if (*Value < 0) {
        ToIncrease = FALSE;
      }

      CurVal = CurVal + *Value;
    } else {

      //
      // Calculate step number of decrease or increase
      //
      Step = ((UINT8)(ABS (*Value - CurVal))) / BCOM_VREF_STEP_SIZE_ENCODED_VALUE;
      if ((*Value - CurVal) < 0) {
        ToIncrease = FALSE;
      }

      CurVal = *Value;
    }

    if (CurVal < MinLimit) {
      RcDebugPrintWithDevice (
        SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
        "%a Out of range!! CurVal = %d, MinLimit = %d\n",
        GetSignalStr (Signal), CurVal, MinLimit
      );
      CurVal = MinLimit;
      Status = MRC_STATUS_FAILURE;
    }
    if (CurVal > MaxLimit) {
      RcDebugPrintWithDevice (
        SDBG_MAX, Socket, Channel, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
        "%a Out of range!! CurVal = %d, MaxLimit = %d\n",
        GetSignalStr (Signal), CurVal, MaxLimit
      );
      CurVal = MaxLimit;
      Status = MRC_STATUS_FAILURE;
    }

    //
    // Use BCOM static value to change BCOM Vref. This will affect DB PG[2]RWFA.
    //
    for (Index = 0; Index < Step; Index ++) {
      if (ToIncrease == TRUE) {
        IncraseBcomVrefBy2 (Host, Socket, Channel, SubChannel, Dimm, Rank);
      } else {
        DecreaseBcomVrefBy2(Host, Socket, Channel, SubChannel, Dimm, Rank);
      }
    }
  } // Mode == GSM_READ_ONLY

  return Status;
}

/**

  Get set Data Buffer DFE vref based on the provided group number of a give DQ pin

  NOTE: In order to set the DFE_Vref, the DFE feature (RWA0) and DFE training mode (RWA1) should
        be enabled.
        They are enabled in the RMT pre-sweep function.

  @param[in] Host             - Pointer to sysHost
  @param[in] Socket           - Socket number
  @param[in] Channel          - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] Strobe           - Strobe number inside the channel.
                                Caller shall pass the valid strobes: ALL_STROBES, 0-4 and 10 -14
  @param[in] Group            - Enumerated group name
  @param[in] Mode             - GSM_READ_CSR - Read the data from hardware and not cache
                              - GSM_READ_ONLY - Do not write
                              - GSM_WRITE_OFFSET - Write offset and not value
                              - GSM_FORCE_WRITE - Force the write
  @param[in, out ] Value      - Value to program

  @retval Status
**/
MRC_STATUS
GetSetDbDfeVrefDqn (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Channel,
  IN    UINT8     Dimm,
  IN    UINT8     Strobe,
  IN    MRC_GT    Group,
  IN    UINT8     Mode,
  IN OUT INT16    *Value
  )
{
  UINT8                         SubChannel;
  UINT8                         MaxSubChStrobe;
  UINT8                         StrobeStart;
  UINT8                         StrobeEnd;
  UINT8                         Buffer;
  UINT8                         CwRegAddress;
  INT16                         CurVal;
  INT16                         Temp;
  UINT8                         Bit;
  UINT8                         DfeVref;
  UINT8                         RangeSelect;
  UINT16                        MinLimit;
  UINT16                        MaxLimit;
  UINT16                        Delay;
  UINT8                         MSVx4;
  UINT8                         MSVx8;
  UINT8                         MSVx8SubCh;
  BOOLEAN                       SameFinalValue = FALSE;  // Same setting for all data buffers
  INT16                         FirstValue;
  DDR5_DATA_BUFFER_RWA2_STRUCT  DbRwA2;

  GetDimmParamGroupLimits (DdrLevel, Group, &MinLimit, &MaxLimit, &Delay);

  MSVx4          = GetMaxStrobeValid (Host);
  MSVx8          = MSVx4 / 2;
  MSVx8SubCh     = GetSubChMaxStrobeValid (Host) / 2;
  MaxSubChStrobe = GetSubChMaxStrobeValid (Host);

  if (Strobe >= MSVx4 && Strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
      "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((Strobe < MSVx4 || Strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  switch (Group) {
  case DbDfeVrefDq0:
    CwRegAddress = LRDIMM_RWE2;
    Bit = 0;
    break;
  case DbDfeVrefDq1:
    CwRegAddress = LRDIMM_RWE6;
    Bit = 1;
    break;
  case DbDfeVrefDq2:
    CwRegAddress = LRDIMM_RWEA;
    Bit = 2;
    break;
  case DbDfeVrefDq3:
    CwRegAddress = LRDIMM_RWEE;
    Bit = 3;
    break;
  case DbDfeVrefDq4:
    CwRegAddress = LRDIMM_RWF2;
    Bit = 4;
    break;
  case DbDfeVrefDq5:
    CwRegAddress = LRDIMM_RWF6;
    Bit = 5;
    break;
  case DbDfeVrefDq6:
    CwRegAddress = LRDIMM_RWFA;
    Bit = 6;
    break;
  case DbDfeVrefDq7:
    CwRegAddress = LRDIMM_RWFE;
    Bit = 7;
    break;

  default:
    return MRC_STATUS_SIGNAL_NOT_SUPPORTED;
    break;
  }

  if (Strobe == ALL_STROBES) {
    StrobeStart = 0;
    StrobeEnd = MSVx4;
  } else {
    StrobeStart = Strobe;
    StrobeEnd = Strobe + 1;
  }

  //
  // For the ALL_STROBES case, check if the final setting are the same for all data buffers,
  // if they are the same, we can optimize the regsiter access by using broadcast.
  //
  if ((Strobe == ALL_STROBES) && ((Mode & GSM_READ_ONLY) == 0)) {

    SameFinalValue = TRUE;
    FirstValue = MinLimit;

    for (Strobe = StrobeStart; Strobe < StrobeEnd; Strobe++) {

      if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
        continue;
      }

      //
      // Data buffer is x8 device, skip the strobes on the upper nibble
      //
      if ((Strobe % MaxSubChStrobe) >= MSVx8SubCh) {
        continue;
      }

      if (Strobe < MaxSubChStrobe) {
        SubChannel = 0;
      } else {
        SubChannel = 1;
      }

      Buffer = Strobe % (MaxSubChStrobe / 2);

      ReadDbCwCache (Socket, Channel, SubChannel, Dimm, Buffer, LRDIMM_DB_PAGE6, CwRegAddress, &DfeVref);

      //
      // Sign bit is in the next register which has address of CwRegAddress + 1
      // LRDIMM_RWE3, LRDIMM_RWE7, LRDIMM_RWEB, LRDIMM_RWEF,
      // LRDIMM_RWF3, LRDIMM_RWF7, LRDIMM_RWFB, LRDIMM_RWFF
      //
      ReadDbCwCache (Socket, Channel, SubChannel, Dimm, Buffer, LRDIMM_DB_PAGE6, CwRegAddress + 1, &RangeSelect);

      CurVal = DfeVref;

      // Check the sign bit. A value of 1 means negative offset
      if (RangeSelect & BIT0) {
        CurVal = -1 * CurVal;
      }

      //
      // Normalize to range of 0 to 2 * MAX_DB_DFE_VREF_RANGE or half of the MaxLimit so the value is always positive.
      //
      CurVal += MaxLimit / 2;

      // Adjust the current value by offset
      if (Mode & GSM_WRITE_OFFSET) {
        // Get the new offset (note: can be negative)
        Temp = CurVal + *Value;
      } else {
        // FORCE_WRITE
        Temp = *Value;
      }

      if (Strobe == 0) {
        FirstValue = Temp;
      } else {
        if (FirstValue != Temp) {
          SameFinalValue = FALSE;
          break;
        }
      }
    } // Strobe
  }

  for (Strobe = StrobeStart; Strobe < StrobeEnd; Strobe++) {

    if (IsStrobeNotValidChDdr5 (Host, Strobe)) {
      continue;
    }

    //
    // Data buffer is x8 device, skip the strobes on the upper nibble
    //
    if ((Strobe % MaxSubChStrobe) >= MSVx8SubCh) {
      continue;
    }

    //
    // If broadcast will be used, skip the Strobe != 0 and 10
    //
    if (SameFinalValue && (Strobe != 0) && (Strobe != MaxSubChStrobe)) {
      continue;
    }

    if (Strobe < MaxSubChStrobe) {
      SubChannel = 0;
    } else {
      SubChannel = 1;
    }

    Buffer = Strobe % (MaxSubChStrobe / 2);

    ReadDbCwCache (Socket, Channel, SubChannel, Dimm, Buffer, LRDIMM_DB_PAGE6, CwRegAddress, &DfeVref);

    //
    // Sign bit is in the next register which has address of CwRegAddress + 1
    // LRDIMM_RWE3, LRDIMM_RWE7, LRDIMM_RWEB, LRDIMM_RWEF,
    // LRDIMM_RWF3, LRDIMM_RWF7, LRDIMM_RWFB, LRDIMM_RWFF
    //
    ReadDbCwCache (Socket, Channel, SubChannel, Dimm, Buffer, LRDIMM_DB_PAGE6, CwRegAddress + 1, &RangeSelect);

    CurVal = DfeVref;

    // Check the sign bit. A value of 1 means negative offset
    if (RangeSelect & BIT0) {
      CurVal = -1 * CurVal;
    }

    //
    // Normalize to range of 0 to 2 * MAX_DB_DFE_VREF_RANGE or half of the MaxLimit so the value is always positive.
    //
    CurVal += MaxLimit / 2;

    if (Mode & GSM_READ_ONLY) {
      *Value = CurVal;
    } else {
      // Adjust the current value by offset
      if (Mode & GSM_WRITE_OFFSET) {
        // Get the new offset (note: can be negative)
        Temp = CurVal + *Value;
      } else {
        // FORCE_WRITE
        Temp = *Value;
      }

      // Check range
      if ((Temp >= MinLimit) && (Temp <= MaxLimit)) {
        RangeSelect = 0;
        if (Temp < MaxLimit / 2) {
          // Negative
          RangeSelect |= BIT0;
          DfeVref = (UINT8)(MaxLimit / 2 - Temp);
        } else {
          DfeVref = (UINT8)(Temp - MaxLimit / 2);
        }
      } else {
        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Channel, Dimm, SubChannel, NO_RANK, Strobe, NO_BIT,
          "DB DFE Vref is Out of Range!! Value = 0x%x, MaxLimit = 0x%x\n",
          Temp,
          MaxLimit
        );
        return MRC_STATUS_FAILURE;
      }

      //
      // Read the current setting of the DQ pin selection.
      //
      ReadDbCwCache (Socket, Channel, SubChannel, Dimm, Buffer, LRDIMM_DB_PAGE0, DDR5_DATA_BUFFER_RWA2_REG, &DbRwA2.Data);

      if (SameFinalValue) {
        Buffer = ALL_DATABUFFERS;
      }

      //
      // Write the new value (range and value), the new value will be cached by the DbControlWordWrite()
      //
      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, 0, DDR5_DATA_BUFFER_RW93_REG, Buffer);

      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, 0, RDIMM_RWDF, LRDIMM_DB_PAGE6);

      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, 0, CwRegAddress, DfeVref);
      DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, 0, CwRegAddress + 1, RangeSelect);

      //
      // Select the corresponding DQ pin.
      //
      if (DbRwA2.Bits.dqn_dfe_pin_selection != (1 << Bit)) {
        DbRwA2.Bits.dqn_dfe_pin_selection = 1 << Bit;
        DbControlWordWrite (Host, Socket, Channel, 1 << SubChannel, Dimm, 0, DDR5_DATA_BUFFER_RWA2_REG, DbRwA2.Data);
      }
    }
  } // Strobe

  return MRC_STATUS_SUCCESS;
}

#if FixedPcdGetBool(PcdMcrSupport)
/**
  Get Host Rank ID (DCS0/1 CID2) mapping for MCR DIMM

  @param[in]      Socket        Processor Socket
  @param[in]      Ch            Channel
  @param[in]      Dimm          DIMM ID
  @param[in]      Rank          Rank ID within DIMM
  @param[in]      PsCh          Pseudo-channel

  @retval Host Rank ID
**/
UINT8
EFIAPI
GetHostRankId (
    IN UINT8    Socket,
    IN UINT8    Ch,
    IN UINT8    Dimm,
    IN UINT8    Rank,
    IN UINT8    PsCh
  )
{
  UINT8    HostRank = 0;

  if (IsMcrPresentChannel (Socket, Ch)) {
    //
    // For x4, DCS0 maps to PS0, DCS1 maps to PS1
    // Only rank0 supported for x4 MCR
    //
    if (IsX4Dimm (Socket, Ch, Dimm)) {
      HostRank = PsCh;
    } else {
      //
      // MCR_TODO: Implement MCR to Host Rank mapping for x8 MCR
      //
    }
  } else {
    HostRank = Rank;
  }
  return HostRank;
}

/**

Writes Multi-Purpose Command for DDR5 / MCR

@param[in] Socket               - Socket Id
@param[in] Ch                   - Channel number (0-based)
@param[in] Dimm                 - DIMM number (0-based)
@param[in] Subchannelmask       - sub channel mask
@param[in] PsChannelMask        - Pseudo Channel Mask
@param[in] Rank                 - Rank number (0-based)
@param[in] Opcode               - Opcode for the MPC command
@param[in] Timingmode           - support MPC_1N, MPC_2N, MPC_4N.

@retval None

**/
VOID
EFIAPI
WriteMpcMcr (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    PsChannelMask,
  IN UINT8    Rank,
  IN UINT16   Opcode,
  IN UINT8    Timingmode,
  IN BOOLEAN  PassThroughMode
  )
{
  PSYSHOST Host;
  UINT8     LocalPsChannelMask;

  Host = GetSysHostPointer ();

  if (PsChannelMask == PS_CH_UNUSED) {
    LocalPsChannelMask = PS_CH0_MASK;
  } else {
    LocalPsChannelMask = PsChannelMask;
    //
    //  If Passthrough Mode and x4 DIMM present, restrict MPC to only PS0.
    //  It will be broadcasted to PS1.
    //
    if (IsMcrPresentChannel (Socket, Ch) && PassThroughMode == TRUE && IsX4Dimm (Socket, Ch, Dimm)) {
      LocalPsChannelMask &= ~PS_CH1_MASK;

      if (PsChannelMask & PS_CH1_MASK) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "MPC in PassThroughMode restricted to PS0\n");
      }

      if (LocalPsChannelMask == 0) {
        return;
      }
    }
  }
  if (Host->var.mem.socket[Socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  WriteCadbMcrMpcCmd (Socket, Ch, Dimm, SubChannelMask, LocalPsChannelMask, Rank, Opcode, Timingmode, PassThroughMode);

  FixedDelayMicroSecond (1);
  return;
} // WriteMpcMcr

/**
  Enter RCD CA pass through broadcast mode.

  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/
VOID
EFIAPI
EnterRcdCaBcastPassThroughMode (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  PSYSHOST Host;
  DDR5_MCR_RCD_PG70_RW64_STRUCT McrRcdPg70Rw64;

  Host = GetSysHostPointer ();

  //
  // Enable Broadcast for CA pass through
  //
  if (IsMcrPresentChannel (Socket, Channel)) {
    ReadRcdCwCache (Socket, Channel, Dimm, SUB_CH_A, RCW_PAGE_ID (DDR5_MCR_RCD_PG70_RW64_REG), RCW_REG_NUM (DDR5_MCR_RCD_PG70_RW64_REG), &McrRcdPg70Rw64.Data);
    if (IsX4Dimm (Socket, Channel, Dimm)) {
      McrRcdPg70Rw64.Bits.ca_pass_through_mode_broadcast = 1;
    } else {
      McrRcdPg70Rw64.Bits.ca_pass_through_mode_broadcast = 0;
    }
    RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_CW_PAGE_REG, RCW_PAGE_ID (DDR5_MCR_RCD_PG70_RW64_REG));
    RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, RCW_REG_NUM (DDR5_MCR_RCD_PG70_RW64_REG), McrRcdPg70Rw64.Data);
  }
  EnterRcdCaPassThroughModeDdr5 (Host, Socket, Channel, Dimm, Rank);
} // EnterRcdCaBcastPassThroughMode

/**
  Exit RCD CA pass through broadcast mode.

  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/
VOID
EFIAPI
ExitRcdCaBcastPassThroughMode (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  PSYSHOST Host;
  DDR5_MCR_RCD_PG70_RW64_STRUCT McrRcdPg70Rw64;

  Host = GetSysHostPointer ();

  //
  // Disable Broadcast for CA pass through
  //
  ExitRcdCaPassThroughModeDdr5 (Host, Socket, Channel, Dimm, Rank);

  if (IsMcrPresentChannel (Socket, Channel)) {
    ReadRcdCwCache (Socket, Channel, Dimm, SUB_CH_A, RCW_PAGE_ID (DDR5_MCR_RCD_PG70_RW64_REG), RCW_REG_NUM (DDR5_MCR_RCD_PG70_RW64_REG), &McrRcdPg70Rw64.Data);
    McrRcdPg70Rw64.Bits.ca_pass_through_mode_broadcast = 0;
    RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_CW_PAGE_REG, RCW_PAGE_ID (DDR5_MCR_RCD_PG70_RW64_REG));
    RcdControlWordWrite (Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, RCW_REG_NUM (DDR5_MCR_RCD_PG70_RW64_REG), McrRcdPg70Rw64.Data);
  }
} // ExitRcdCaBcastPassThroughMode

/**

  Send NOP Command to MCR / DDR5

  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number (0-based)
  @param[in] SubChMask - Sub Channel Mask
  @param[in] PsChMask - Pseudo CH Mask
  @param[in] Dimm     - DIMM number (0-based)
  @param[in] Rank     - Rank number (0-based)
  @param[in] Address  - Address for the MRS write
  @param[in] Cw       - CW flag
  @param[in] PassThroughMode - Pass Through Mode
  @param[in] ExitCATM - Exit CATM
  @param[in] NoOfCsClks - Assert CS atleast for this number of DRAM CLKS

  @retval N/A

**/
UINT32
EFIAPI
SendNopMcr (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    PsChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    Cw,
  IN BOOLEAN  PassThroughMode,
  IN BOOLEAN  ExitCATM,
  IN UINT8    NoOfCsClks
  )
{
  PSYSHOST  Host;
  UINT8     LocalPsChMask;

  Host = GetSysHostPointer();

  if (PsChMask == PS_CH_UNUSED) {
    LocalPsChMask = PS_CH0_MASK;
  } else {
    LocalPsChMask = PsChMask;
  }

  RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "NOP command: CW 0x%04X, PassThroughMode: %x, ExitCATM: %x, SubChMask: %d\n", Cw, PassThroughMode, ExitCATM, SubChMask);

  CountTrackingData(PTC_MRS_WRITE, 1);

  if (Host->var.mem.socket[Socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  WriteCadbMcrNopCmd (Socket, Ch, Dimm, SubChMask, LocalPsChMask, Rank, Cw, NOP_CMD, PassThroughMode, ExitCATM, NoOfCsClks);
  FixedDelayMicroSecond (1);
  return SUCCESS;
} // SendNopMcr

/**

Send MRW Command to MCR / DDR5

@param[in] Socket   - Socket Id
@param[in] Ch       - Channel number (0-based)
@param[in] SubChMask- Sub Channel mask
@param[in] PsChMask - Pseudo CH Mask
@param[in] Dimm     - DIMM number (0-based)
@param[in] Rank     - Rank number (0-based)
@param[in] Address  - Address for the MRS write
@param[in] Cw       - CW flag
@param[in] Data     - Data to write

@retval N/A

**/
UINT32
SendMrwMcr (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChMask,
  IN UINT8    PsChMask,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT16   Address,
  IN UINT8    Cw,
  IN UINT8    Data
  )
{
  PSYSHOST                   Host;
  UINT32                     Data32;
  BOOLEAN                    EnableSmbusMrw;
  UINT8                      SmbData8;
  struct dimmNvram           (*DimmNvList)[MAX_DIMM];
  DDR5_LRDIMM_TRAINING_DATA  *TrainingData;
  MRC_STATUS                 Status;
  UINT8                       CurrentRcwPage;
  UINT8                       PageReg;

  SmbData8 = 0;
  Host = GetSysHostPointer ();

  RcDebugPrintWithDevice (SDBG_RCWRITETAG, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "MRW command with Address 0x%04X, CW 0x%04X, Data0x%04X,  SubChMask %d PsChMask %d\n", Address, Cw, Data, SubChMask, PsChMask);

  CountTrackingData (PTC_MRS_WRITE, 1);//DDR5_TODO.
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RemapMraAddress(Host, Socket, Ch, Dimm, &Address, &Cw);

  if (Host->var.mem.socket[Socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  //
  // Determine if MRW should be sent inband or over SMBUS
  //
  EnableSmbusMrw = UseSmbusMrw (Host, Socket);
  if (Cw == CW_DRAM && FeaturePcdGet (PcdCteBuild) == TRUE) {
    EnableSmbusMrw = FALSE; // for CTE, force inband if not CW
  }

  // If EnableSmbusMrw is TRUE, check to see if the target is a data buffer address (CW set to 1, Bit 7 of address
  // is 1).  If so, check if BCOM training is done.  When BCOM training is complete, and the taget is the data buffer,
  // do not use SMBUS.
  if ((EnableSmbusMrw == TRUE) && (Cw == CW_RCD_DB) && (Address & BIT7)) {
    Status = GetDdr5LrdimmTrainingData (&TrainingData);
    if (Status == MRC_STATUS_SUCCESS) {
      if (TrainingData->BcomTrainingDone == TRUE) {
        EnableSmbusMrw = FALSE;
      }
    }
  }

  if (EnableSmbusMrw == FALSE) {
    CurrentRcwPage = 0;
    if (Cw == CW_RCD_DB) {
      //
      // If we are accessing paged register, select page if required
      // Assume: SUBCH A and SUBCH B always have the same page-id selected
      //
      if ((Address & BIT7) != 0) {
        //
        // DB register
        //
        PageReg = DDR5_DATA_BUFFER_CW_PAGE_REG;
      } else {
        PageReg = DDR5_RCD_CW_PAGE_REG;
      }
      ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, PageReg, &CurrentRcwPage);
      if (CurrentRcwPage != RCW_PAGE_ID (Address) && (RCW_PAGE_ID (Address) != 0 ||
        (RCW_PAGE_ID (Address) == 0 && RCW_REG_NUM (Address) >= MAX_RCD_DIRECT_CONTROL_WORDS))) {
        WriteRcdCwCache (Socket, Ch, SubChMask, Dimm, RDIMM_CW_PAGE0, PageReg, RCW_PAGE_ID (Address));
        WriteCadbMcrMrCmd (Socket, Ch, Dimm, SubChMask, PsChMask, Rank, PageReg, RCW_PAGE_ID (Address), Cw, MRW_CMD);
      }
    }
    WriteCadbMcrMrCmd (Socket, Ch, Dimm, SubChMask, PsChMask, Rank, RCW_REG_NUM (Address), Data, Cw, MRW_CMD);
  } else {
    if (Cw == CW_RCD_DB || ((*DimmNvList)[Dimm].DcpmmPresent == 1)) {
      Data32 = Data;
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        WriteRcdSmbDdr5 (Host, Socket, Ch, Dimm, SubChMask, BYTE_WRITE, RCW_PAGE_ID (Address), RCW_REG_NUM (Address), &Data32);
      } else {
        WriteCwvSideband (Host, Socket, Ch, Dimm, Address, Data);
      }
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch,  Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Warning: SendMrw target can not be DRAM when the DRAM CA is not trained\n");
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_115);
    }
  }

  //
  // tMRD: Mode Register Set command delay
  //
  FixedDelayMicroSecond (1);

  return SUCCESS;
} // SendMrwMcr

  /**

  Send MRR Command to MCR / DDR5

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Channel number (0-based)
  @param[in] SubChMask   - Sub-channel mask
  @param[in] PsChMask    - Pseudo CH Mask
  @param[in] Dimm        - DIMM number (0-based)
  @param[in] Rank        - Rank number (0-based)
  @param[in] Address     - Mode register Address
  @param[in] Cw          - CW flag
  @param[in] Data        - Data to store the return result.

  @retval N/A

  **/
VOID
SendMrrMcr (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Ch,
  IN UINT8        SubChMask,
  IN UINT8        PsChMask,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN UINT16       Address,
  IN UINT8        Cw,
  IN OUT UINT8   *Data
  )
{
  UINT8   LocalPsChMask;

  if (PsChMask == PS_CH_UNUSED) {
    LocalPsChMask = PS_CH0_MASK;
  } else {
    LocalPsChMask = PsChMask;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "MRR command with Address 0x%04X, CW 0x%04X\n", Address, Cw);
  CountTrackingData (PTC_MRS_WRITE, 1);

  if (Host->var.mem.socket[Socket].hostRefreshStatus == 1) {
    FixedDelayMicroSecond (10);
  }

  WriteCadbMcrMrCmd (Socket, Ch, Dimm, SubChMask, LocalPsChMask, Rank, Address, 0, Cw, MRR_CMD);
  FixedDelayMicroSecond (1);

  return;
} // SendMrrMcr

/**
  InitDramInterfaceMcr - Does MCR Initialiation if MCR is present
  else, does DDR5 initialization.

  @param[in] Socket     - Socket Id
  @param[in] Ch         - Channel number (0-based)
  @param[in] Dimm       - DIMM number (0-based)
  @param[in] SubChannelMask - Sub channel mask
  @param[in] Rank       - Rank number (0-based)

@retval N/A

**/
VOID
InitDramInterfaceMcr (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  )
{
  PSYSHOST                                            Host;
  UINT8                                               ControlWord = 0;
  struct dimmNvram                                    (*DimmNvList)[MAX_DIMM];
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT         ParityAndAlert;
  DDR5_RCD_COMMAND_LATENCY_ADDER_CONFIGURATION_STRUCT CommandLatencyAdderConfig;
  DDR5_DATA_BUFFER_RW93_STRUCT                        DataBufferRw93;
  DDR5_DATA_BUFFER_RW82_STRUCT                        DataBufferRw82;
  DDR5_DATA_BUFFER_PG70_RWE4_STRUCT                   DataBufferRwE4;
  DDR5_DATA_BUFFER_PG70_RWE5_STRUCT                   DataBufferRwE5;

  Host = GetSysHostPointer();

  //
  // Skip this for Front side training / pretraining init
  //
  if (GetCurrentTestType (Socket) == EarlyRcdCsLoopbackTest || GetCurrentTestType (Socket) == EarlyRcdCaLoopbackTest) {
    return;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "InitDramInterfaceMcr starts\n");
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Enable DRAM interface to forward all commands.
  //
  ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
#else
  ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
#endif
  RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);
  //
  //add some delay
  //
  FixedDelayMicroSecond (1);
  //
  // Send NOP Command to Rank 0 and Rank 1 for initilization. Assume two ranks during the init flow.
  // After init done, will based on the actual Rank number to do the training.
  //
  if (!IsMcrPresentChannel (Socket, Ch)) {
    SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 0, 1, FALSE, FALSE);
    SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 1, 1, FALSE, FALSE);
    EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);
    SendNopMcr (Socket, Ch, SubChannelMask, PS_CH_UNUSED, Dimm, 0, 1, TRUE, FALSE, 3);  // Send NOP with at least 3 CLK CS assertion
  } else {
    McrSetPhaseCmd (Socket, Ch, Dimm, SubChannelMask, 2); // Send NOP with CS stretched to 2 CLKs
    EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);
    McrSetPhaseCmd (Socket, Ch, Dimm, SubChannelMask, 4); // Send NOP with CS stretched to 4 CLKs on each PS.
  }

  FixedDelayMicroSecond (1); // delay at least tXPR (< 1 us)

  //
  // Issue MR13 to initial tCCD_L / tDLLK before DLL_RESET MPC command is sent
  //
  if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3200) {
    ControlWord = DataRate_2001_3200;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3600) {
    ControlWord = DataRate_3201_3600;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4000) {
    ControlWord = DataRate_3601_4000;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4400) {
    ControlWord = DataRate_4001_4400;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4800) {
    ControlWord = DataRate_4401_4800;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_5200) {
    ControlWord = DataRate_4801_5200;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_5600) {
    ControlWord = DataRate_5201_5600;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_6000) {
    ControlWord = DataRate_5601_6000;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_6400) {
    ControlWord = DataRate_6001_6400;
  }
  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, Configure_tDLLK_tCCD_L | ControlWord, TIMING_4N, TRUE);

  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, DLL_RESET, TIMING_4N, TRUE);
  FixedDelayMicroSecond (1); // delay at least tMRD (< 1 us)
  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ZQCAL_START, TIMING_4N, TRUE);
  FixedDelayMicroSecond (2); // delay at least tZQCAL (== 1 us), fixeddelay is roughly 1 microsec
  CteDelayDclk (1700);       // Additional delay for CTE to avoid ZQCAL timing violation
  WriteMPC (Host, Socket, Ch, Dimm, SubChannelMask, Rank, ZQCAL_LATCH, TIMING_4N, TRUE);
  FixedDelayMicroSecond (1); // delay at least tZQLAT (< 1 us)

  if (IsMcrPresentChannel (Socket, Ch)) {
    //
    // Change DRAM to 1N.
    //
    WriteMPC (Host, Socket, Ch, Dimm, GetMaxValidSubChannelMask (), Rank, SET_1N_CMD_TIMING, TIMING_4N, TRUE);
  }
  ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);
  //
  // Set command latency to 1tck.
  //
  CommandLatencyAdderConfig.Data = 0;
  CommandLatencyAdderConfig.Bits.latency_adder_nladd_to_all_dram_commands = 1;
  RcdControlWordWrite (Socket, Ch, SubChannelMask, Dimm, Rank, DDR5_RCD_COMMAND_LATENCY_ADDER_CONFIGURATION_REG, CommandLatencyAdderConfig.Data);
  if (IsLrdimmPresent (Socket, Ch, Dimm) == 1) {
    //
    // Block commands to DRAM, forward commands to DB
    //
    ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);
    //
    // Select all data buffers
    //
    DataBufferRw93.Data = 0;
    DataBufferRw93.Bits.pba_select_id = PBA_SELECT_ALL;
    SendMrw (Host, Socket, Ch, GetMaxValidSubChannelMask (), Dimm, Rank, DDR5_DATA_BUFFER_RW93_REG, CW_RCD_DB, DataBufferRw93.Data);
    //
    // Configure DB in DQ pass-through mode for reads
    //
    DataBufferRw82.Data = 0;
    DataBufferRw82.Bits.dq_pass_through_mode = 1;
    DataBufferRw82.Bits.dq_pass_through_mode_direction_select = 1;
    SendMrw (Host, Socket, Ch, GetMaxValidSubChannelMask (), Dimm, Rank, DDR5_DATA_BUFFER_RW82_REG, CW_RCD_DB, DataBufferRw82.Data);
    if (IsMcrPresentChannel (Socket, Ch)) {
      DataBufferRwE5.Data = 0xCC; //0-PS0, 1-PS1 0xCC results in : DQ0, 1, 4, 5 : PS0, DQ2, 3, 6, 7 : PS1
      SendMrw (Host, Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG70_RWE5_REG, CW_RCD_DB, DataBufferRwE5.Data);

      DataBufferRwE4.Data                                = 0;
      DataBufferRwE4.Bits.dq_static_mux_mode_for_dq_pass = 1;
      SendMrw (Host, Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG70_RWE4_REG, CW_RCD_DB, DataBufferRwE4.Data);
    }

    //
    // Block commands to DB, forward commands to DRAM
    //
    ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
    ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
    RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "InitDramInterfacMcr ends\n");

  return;
} // InitDramInterfaceMcr

/**

Init Db and Dram Interface for MCR

@param[in] Socket          - Socket Id
@param[in] Ch              - Channel number (0-based)
@param[in] Dimm            - DIMM number (0-based)
@param[in] SubChannelMas   - Sub Channel Mask
@param[in] Rank            - Rank number (0-based)

@retval N/A

**/
VOID
EFIAPI
InitDbDramInterfaceMcr (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    SubChannelMask,
  IN UINT8    Rank
  )
{
  PSYSHOST Host;
  DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_STRUCT         ParityAndAlert;
  DDR5_RCD_COMMAND_LATENCY_ADDER_CONFIGURATION_STRUCT CommandLatencyAdder;

  Host = GetSysHostPointer ();

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
    "Init Db and DramInterface starts\n");
  //
  // Enable DRAM interface to forward all commands.
  //
  ParityAndAlert.Data = 0;
#ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.dram_interface_forward_all_cmds = 1;
  ParityAndAlert.Bits.data_buffer_interface_forward_all_cmds = 1;
#else // #ifdef USE_LATEST_RCD_SPEC
  ParityAndAlert.Bits.dram_interface_block_all_cmds = 1;
  ParityAndAlert.Bits.data_buffer_interface_block_all_cmds = 1;
#endif // #ifdef USE_LATEST_RCD_SPEC
  RcdControlWordWrite (Socket, Ch, SUB_CH_A_MASK, Dimm, Rank, DDR5_RCD_PARITY_AND_ALERT_REG_GLOBAL_REG, ParityAndAlert.Data);

  //
  //add some delay
  //
  FixedDelayMicroSecond (1);
  //
  // Send NOP Command to Rank 0 and Rank 1 for initilization. Assume two ranks during the init flow. After init done, will based on the actual Rank number to do the training.
  //
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 0, 0, TRUE, FALSE);
  SendNop (Host, Socket, Ch, SubChannelMask, Dimm, 1, 0, TRUE, FALSE);
  //
  // Set command latency to 1tck.
  //
  CommandLatencyAdder.Data = 0;
  CommandLatencyAdder.Bits.latency_adder_nladd_to_all_dram_commands = 1;
  RcdControlWordWrite (Socket, Ch, SubChannelMask, Dimm, Rank, DDR5_RCD_COMMAND_LATENCY_ADDER_CONFIGURATION_REG, CommandLatencyAdder.Data);

  EnterRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

  //
  // Three NOPs to take DRAM out of SR. Send NOP Command to Rank 0 and Rank 1
  // for initilization. Assume two ranks during the init flow. After init done,
  // will based on the actual Rank number to do the training.
  //
  SendNopMcr (Socket, Ch, SubChannelMask, PS_CH0_MASK, Dimm, 0, 0, FALSE, FALSE, 1);
  SendNopMcr (Socket, Ch, SubChannelMask, PS_CH0_MASK, Dimm, 0, 0, FALSE, FALSE, 1);
  SendNopMcr (Socket, Ch, SubChannelMask, PS_CH0_MASK, Dimm, 0, 0, FALSE, FALSE, 1);
  SendNopMcr (Socket, Ch, SubChannelMask, PS_CH0_MASK, Dimm, 1, 0, FALSE, FALSE, 1);
  SendNopMcr (Socket, Ch, SubChannelMask, PS_CH0_MASK, Dimm, 1, 0, FALSE, FALSE, 1);
  SendNopMcr (Socket, Ch, SubChannelMask, PS_CH0_MASK, Dimm, 1, 0, FALSE, FALSE, 1);

  ExitRcdCaPassThroughMode (Host, Socket, Ch, Dimm, Rank);

  return;
} // InitDbDramInterface


/**

Get the operating speed value for the RW05 in MCR.

@param[in] Socket          - Socket number

@retval RCW speed value

**/
UINT8
GetSpeedRcwValueMcr (
  IN UINT8    Socket
  )
{
  PSYSHOST  Host;
  UINT8     SpeedRcwValue = 0;

  Host = GetSysHostPointer();

  if (IsMcrPresentChannel (Socket, MAX_CH)) {
    /*
      MCR Host side Data rate is 2x of DRAM Data rate

      x x   x   x   0   0   0   0       DDR5-3200 (2800 MT/s < f < 3200 MT/s)
      x x   x   x   0   0   0   1       DDR5-3600 (3200 MT/s < f < 3600 MT/s)
      x x   x   x   0   0   1   0       DDR5-4000 (3600 MT/s < f < 4000 MT/s)
      x x   x   x   0   0   1   1       DDR5-4400 (4000 MT/s < f < 4400 MT/s)
      x x   x   x   0   1   0   0       DDR5-4800 (4400 MT/s < f < 4800 MT/s)
      x x   x   x   0   1   0   1       DDR5-5200 (4800 MT/s < f < 5200 MT/s)
      x x   x   x   0   1   1   0       DDR5-5600 (5200 MT/s < f < 5600 MT/s)
      x x   x   x   0   1   1   1       DDR5-6000 (5600 MT/s < f < 6000 MT/s)
      x x   x   x   1   0   0   0       DDR5-6400 (6000 MT/s < f < 6400 MT/s)
      x x   x   x   1   0   0   1       DDR5-6800 (6400 MT/s < f < 6800 MT/s)
      x x   x   x   1   0   1   0       DDR5-7200 (6800 MT/s < f < 7200 MT/s)
      x x   x   x   1   0   1   1       DDR5-7600 (7200 MT/s < f < 7600 MT/s)
      x x   x   1   0   0   0   0       DDR5-8000 (7600 MT/s < f < 8000 MT/s)
      x x   x   1   0   0   0   1       DDR5-8400 (8000 MT/s < f < 8400 MT/s)
      x x   x   1   0   0   1   0       DDR5-8800 (8400 MT/s < f < 8800 MT/s)

      Supported Data rates are 6400 <= MCR <= 8800, DRAM Data rates are 3200 <= DRAM <= 4400
    */
    if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3200 && Host->nvram.mem.socket[Socket].ddrFreq > DDR_4400) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Warning: The DDR frequency %d is not supported for MCR\n", Host->nvram.mem.socket[Socket].ddrFreq);
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3200) {
      SpeedRcwValue = 8; // MCR 6400
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3400) {
      SpeedRcwValue = 9; // MCR 6800
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3600) {
      SpeedRcwValue = 10; // MCR 7200
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3800) {
      SpeedRcwValue = 11; // MCR 7600
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4000) {
      SpeedRcwValue = 16; // MCR 8000
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4200) {
      SpeedRcwValue = 17; // MCR 8400
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4400) {
      SpeedRcwValue = 18; // MCR 8800
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Warning: The DDR frequency %d is not supported for MCR\n", Host->nvram.mem.socket[Socket].ddrFreq);
    }
  } else {
    if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_2800 && Host->nvram.mem.socket[Socket].ddrFreq > DDR_6400) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH,  NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Warning: The DDR frequency %d is not supported\n", Host->nvram.mem.socket[Socket].ddrFreq);
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3200) {
      SpeedRcwValue = 0;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3600) {
      SpeedRcwValue = 1;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4000) {
      SpeedRcwValue = 2;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4400) {
      SpeedRcwValue = 3;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4800) {
      SpeedRcwValue = 4;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_5200) {
      SpeedRcwValue = 5;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_5600) {
      SpeedRcwValue = 6;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_6000) {
      SpeedRcwValue = 7;
    } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_6400) {
      SpeedRcwValue = 8;
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Warning: The DDR frequency %d is not supported\n", Host->nvram.mem.socket[Socket].ddrFreq);
    }
  } // if/else IsMcrPresentChannel

  return SpeedRcwValue;
} // GetSpeedRcwValueMcr

/**
  Enter DB DQ pass through mode.

  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number
  @param[in]  DqMask    DQ -> PS Mapping
  @param[in]  Rank      Pass Thru direction - Read / Write

  @retval N/A
**/

VOID
EFIAPI
EnterDbDqPassThroughMode (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT8    DqMask,
  IN UINT8    PassThruDirection
  )
{
  PSYSHOST                                            Host;
  DDR5_DATA_BUFFER_RW82_STRUCT                        DataBufferRw82;
  DDR5_DATA_BUFFER_PG70_RWE4_STRUCT                   DataBufferRwE4;
  DDR5_DATA_BUFFER_PG70_RWE5_STRUCT                   DataBufferRwE5;

  Host = GetSysHostPointer();

  //
  // Configure DB in DQ pass-through mode for reads
  //
  DataBufferRw82.Data                                       = 0;
  DataBufferRw82.Bits.dq_pass_through_mode                  = 1;
  DataBufferRw82.Bits.dq_pass_through_mode_direction_select = PassThruDirection;
  SendMrw (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW82_REG, 1, DataBufferRw82.Data);

  if (IsMcrPresentChannel (Socket, Channel)) {
    DataBufferRwE5.Data = DqMask; //value of 0 at bit position maps that DQ to PS0, 1 maps it to PS1
    SendMrw (Host, Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG70_RWE5_REG, 1, DataBufferRwE5.Data);

    DataBufferRwE4.Data                                = 0;
    DataBufferRwE4.Bits.dq_static_mux_mode_for_dq_pass = 1;
    SendMrw (Host, Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG70_RWE4_REG, 1, DataBufferRwE4.Data);
  }
}

/**
  Exit DB DQ pass through mode.

  @param[in]  Socket    Socket ID
  @param[in]  Channel   Channel number
  @param[in]  Dimm      DIMM number
  @param[in]  Rank      Rank number

  @retval N/A
**/

VOID
EFIAPI
ExitDbDqPassThroughMode (
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  PSYSHOST                                            Host;
  DDR5_DATA_BUFFER_RW82_STRUCT                        DataBufferRw82;
  DDR5_DATA_BUFFER_PG70_RWE4_STRUCT                   DataBufferRwE4;
  DDR5_DATA_BUFFER_PG70_RWE5_STRUCT                   DataBufferRwE5;

  Host = GetSysHostPointer();

  if (IsMcrPresentChannel (Socket, Channel)) {
    DataBufferRwE5.Data = 0;  // Map all DQs to PS0
    SendMrw (Host, Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG70_RWE5_REG, 1, DataBufferRwE5.Data);

    DataBufferRwE4.Data = 0;
    SendMrw (Host, Socket, Channel, SUB_CH_A_MASK, Dimm, Rank, DDR5_DATA_BUFFER_PG70_RWE4_REG, 1, DataBufferRwE4.Data);
  }

  //
  // Configure DB in DQ pass-through mode for reads
  //
  DataBufferRw82.Data = 0;
  SendMrw (Host, Socket, Channel, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW82_REG, 1, DataBufferRw82.Data);
}
#endif // FixedPcdGetBool(PcdMcrSupport)
