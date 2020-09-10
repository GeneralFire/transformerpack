/** @file

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

#include "MemTestFunc.h"
#include <Library/ProcSmbIpLib.h>
#include <Memory/I3cDefinitions.h>
#include <Library/MemRcLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>

//
// Only attempt Hard PPR to the same row address 3 times, then flag the BG as being out of PPR resources
// Soft PPR is limited to 1 repair per BG
//
#define   MAX_HARD_PPR_ROW_REPAIR_ATTEMPT    3
#define   MAX_SOFT_PPR_ROW_REPAIR_ATTEMPT    1


//
// Code section
//

/**

  Updates the Adv memtest error count in existing EWL entry

  @param Host      - Pointer to the system Host (root) structure
  @param socket    - Current socket
  @param channel   - Current channel
  @param dimm      - Current dimm
  @param rank      - Current rank
  @param testType  - Memory test type
  @param errorInfo - Advanced Memtest Error status
  @param warningPtr - Pointer to matching EWL type 4 entry

  @retval            TRUE if EWL entry found; FALSE if not found

**/
BOOLEAN
LookupEwlAdvMemtestEntry (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    channel,
  UINT8    dimm,
  UINT8    rank,
  UINT8    testType,
  CPGC_ERROR_STATUS_MATS errorInfo,
  EWL_ENTRY_TYPE4  **warningPtr
)
{
  UINT32  Index;
  EWL_ENTRY_HEADER *warningHeader;
  EWL_ENTRY_TYPE4  *warning;
  EWL_ENTRY_MEMORY_LOCATION  MemoryLocation;

  MemoryLocation.Socket = socket;
  MemoryLocation.Channel = channel;
  MemoryLocation.Dimm = dimm;
  MemoryLocation.Rank = rank;

  //
  // Search for existing entry in Enhanced Warning Log buffer
  //
  Index = 0;
  while (Index < Host->var.common.ewlPrivateData.status.Header.FreeOffset) {
    //
    // Get next warning from EWL structure
    //
    warningHeader = (EWL_ENTRY_HEADER*) &(Host->var.common.ewlPrivateData.status.Buffer[Index]);
    //
    // If Warning is Advanced Memtest Type (type4)
    //
    if (warningHeader->Type == EwlType4) {
      //
      // Find matching entry in the log
      //
      warning = (EWL_ENTRY_TYPE4 *)warningHeader;
      if ((CompareMem ((UINT8 *) &(warning->AdvMemtestErrorInfo), (UINT8 *)&errorInfo, sizeof (CPGC_ERROR_STATUS_MATS)) == 0) &&
          (CompareMem ((UINT8 *) &(warning->MemoryLocation), (UINT8 *)&MemoryLocation, sizeof (EWL_ENTRY_MEMORY_LOCATION)) == 0) &&
          (warning->MemtestType == testType)) {

        //
        // Return pointer to the Type 4 entry
        //
        *warningPtr = warning;
        return TRUE;
      }
    }
    Index += warningHeader->Size;
  }
  *warningPtr = NULL;
  return FALSE;

} // LookupEwlAdvMemtestEntry

/**

  Logs AdvancedMemtestError into the Host structure

  @param Host      - Pointer to the system Host (root) structure
  @param socket    - Current socket
  @param channel   - Current channel
  @param dimm      - Current dimm
  @param rank      - Current rank
  @param MinorWarningCode - Minor warning code
  @param testType  - Memory test type
  @param errorInfo - Advanced Memtest Error status
  @param Count     - Number of errors observed

  @retval N/A

**/
VOID
LogAdvMemtestError (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    channel,
  UINT8    dimm,
  UINT8    rank,
  UINT8    MinorWarningCode,
  UINT8    testType,
  CPGC_ERROR_STATUS_MATS errorInfo,
  UINT32   Count
)
{
  EWL_ADV_MT_STATUS ewlErrorInfo;
  EWL_ENTRY_TYPE4  *warningPtr;

  //
  // Increment count of Adv memtest error in EWL log if existing entry is found
  //
  if (LookupEwlAdvMemtestEntry (Host, socket, channel, dimm, rank, testType, errorInfo, &warningPtr)) {
    warningPtr->Count++;

  } else {

    //
    // If there is not a warning logged, create new EWL entry with Count
    //
    ewlErrorInfo.Dat0S     = errorInfo.cpgcErrDat0S;
    ewlErrorInfo.Dat1S     = errorInfo.cpgcErrDat1S;
    ewlErrorInfo.Dat2S     = errorInfo.cpgcErrDat2S;
    ewlErrorInfo.Dat3S     = errorInfo.cpgcErrDat3S;
    ewlErrorInfo.EccS      = errorInfo.cpgcErrEccS;
    ewlErrorInfo.Chunk     = errorInfo.cpgcErrChunk;
    ewlErrorInfo.Column    = errorInfo.cpgcErrColumn;
    ewlErrorInfo.ColumnExt = errorInfo.cpgcErrColumnExt;
    ewlErrorInfo.Row       = errorInfo.cpgcErrRow;
    ewlErrorInfo.RowExt    = errorInfo.cpgcErrRowExt;
    ewlErrorInfo.Bank      = errorInfo.cpgcErrBank;
    ewlErrorInfo.Rank      = errorInfo.cpgcErrRank;
    ewlErrorInfo.Subrank   = errorInfo.cpgcSubrank;

    EwlOutputType4 (WARN_FPT_CORRECTABLE_ERROR, MinorWarningCode, socket, channel, dimm, rank, EwlSeverityWarning,
      testType, ewlErrorInfo, Count);
  }
} //LogAdvMemtestError


/**

  Reads the DIMM Temperature Sensor via SMBus and prints the current temperature

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param Socket           - Socket Id
  @param Ch               - Channel
  @param Dimm             - DIMM

  @retval None

**/
VOID
PrintDimmTemperature (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch,
  UINT8                     Dimm
  )
{
  SMB_DEVICE_STRUCT                           Tsod;
  UINT16                                      MstData;
  INT16                                       Temp;
  UINT8                                       DimmTsMask;
  UINT8                                       RegOffset;
  struct dimmNvram                            (*DimmNvList)[MAX_DIMM];
  UINT8                                       TempSensor;
  UINT8                                       TsodDeviceTypeId[MAX_TS_PER_DIMM];

  MstData = 0;
  SetMem ((UINT8 *)&Tsod, sizeof (Tsod), 0);

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  Tsod.compId = MTS;
  Tsod.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  GetSmbAddress (Socket, Ch, Dimm, &Tsod);

  // For DDR4
  RegOffset = MTS_TEMP;
  DimmTsMask = BIT7;
  TsodDeviceTypeId[0] = DTI_TSOD;
  TsodDeviceTypeId[1] = DTI_TSOD;

#ifdef DDR5_SUPPORT
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    RegOffset = TS5_MR49_TEMP;
    DimmTsMask = SPD_DIMM_TS_MASK_DDR5;
    TsodDeviceTypeId[0] = DDR5_TS0;
    TsodDeviceTypeId[1] = DDR5_TS1;  // Support MAX_TS_PER_DIMM is 2
  }
#endif // DDR5_SUPPORT

  if ((*DimmNvList)[Dimm].dimmTs & DimmTsMask) {

    for (TempSensor = 0; TempSensor < MIN (MAX_TS_PER_DIMM, (*DimmNvList)[Dimm].dimmTs); TempSensor++) {

      Tsod.address.deviceType = TsodDeviceTypeId[TempSensor];

      ReadProcSmb (Tsod, RegOffset, (UINT8 *) &MstData);

      //xxx0 0000 0010 11xx + 2.75 °C
      //xxx1 1111 1111 11xx - 0.25 °C

      MstData &= 0x1FFC;
      if (MstData & BIT12) {
        Temp = (INT16) (MstData | 0xE000);  // Sign extend negative
      } else {
        Temp = MstData;
      }
      Temp = Temp / 8;                      // Shift to 0.5 units

      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Dimm Temperature Sensor %d = %d.%d'C\n",
          TempSensor, Temp / 2, (((UINT16) Temp) % 2) * 5);
    }
  }
}

#ifdef  DDR5_SUPPORT

/**

  Checks the results of the preceding advanced memory test on Ddr5

  @param[in]  Host                    - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket                  - Socket Id
  @param[in]  ChBitmask               - List of channels that were tested
  @param[in]  DimmRank                - DIMM containing logicalRank
  @param[in]  Logical2Physical        - Geometery for logical rank within channel
  @param[in]  LogicalRank             - Logical rank
  @param[in]  RowBits                 - Number of row bits supported by current logical rank
  @param[in]  BaseBits                - Number of bank bits in SW loop
  @param[in]  TestType                - Memory test type for logging purposes
  @param[out] TestStatus[MAX_CH]      - Pass/fail status for the test per channel
  @param[in]  Direction               - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval status - SUCCESS/FAILURE

**/
UINT32
CheckTestResultsMATSDdr5 (
  IN     PSYSHOST   Host,
  IN     UINT8      Socket,
  IN     UINT32     ChBitmask,
  IN     UINT8      DimmRank[MAX_RANK_CH][MAX_CH],
  IN     UINT8      Logical2Physical[MAX_RANK_CH][MAX_CH],
  IN     UINT8      LogicalRank,
  IN     UINT8      RowBits[MAX_CH],
  IN     UINT8      BaseBits,
  IN     UINT8      TestType,
     OUT UINT8      TestStatus[MAX_CH],
  IN     UINT8      Direction
  )
{
  UINT32                   Status = SUCCESS;
  UINT32                   ErrorStatusDatSubChA;
  UINT32                   ErrorStatusDatSubChB;
  UINT32                   ErrorStatusEccSubChA;
  UINT32                   ErrorStatusEccSubChB;
  UINT8                    Dimm;
  UINT8                    Ch;
  UINT8                    Rank;
  CPGC_ERROR_STATUS_MATS   CpgcErrorStatus[MAX_CH];
  UINT8                    MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  //
  // Collect test results
  //
  CpgcAdvTrainingErrorStatusMATS (Host, Socket, ChBitmask, CpgcErrorStatus);

  //
  // Inject error for testing purposes
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (ChBitmask & (1 << Ch)) {
      InjectErrorTestResultsMATS (Host, Socket, Ch, LogicalRank, CpgcErrorStatus);
    }
  }

  if ((TestType != ADV_MT_TYPE_LTEST_SCRAM) &&
      (TestType != ADV_MT_TYPE_LINIT_SCRAM)) {

    //
    // Update Row Range Failure tracking
    //
    Status = UpdateRowFailures (Host, Socket, ChBitmask, CpgcErrorStatus, RowBits, BaseBits, TestStatus, Direction);
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    Dimm = DimmRank[LogicalRank][Ch];
    Rank = Logical2Physical[LogicalRank][Ch];

    //
    // Was this channel part of the test group?
    //
    if (ChBitmask & (1 << Ch)) {
      //
      // Evaluate error status
      // Do not log failures on 1st try
      //
      ErrorStatusDatSubChA = 0;
      ErrorStatusDatSubChB = 0;
      ErrorStatusEccSubChA = 0;
      ErrorStatusEccSubChB = 0;

      if (GetAmtRetryState (Host)) {
        ErrorStatusDatSubChA = CpgcErrorStatus[Ch].cpgcErrDat0S | CpgcErrorStatus[Ch].cpgcErrDat2S;
        ErrorStatusDatSubChB = CpgcErrorStatus[Ch].cpgcErrDat1S | CpgcErrorStatus[Ch].cpgcErrDat3S;
        if (Host->nvram.mem.eccEn) {
          ErrorStatusEccSubChA = (CpgcErrorStatus[Ch].cpgcErrEccS & BYTE_MASK) | ((CpgcErrorStatus[Ch].cpgcErrEccS >> BYTE_WIDTH) & BYTE_MASK);
          ErrorStatusEccSubChB = ((CpgcErrorStatus[Ch].cpgcErrEccS >> WORD_WIDTH) & 0xFF) | (((CpgcErrorStatus[Ch].cpgcErrEccS >> (WORD_WIDTH + BYTE_WIDTH))) & 0xFF);
        }
      }

      if (!(GetEmulation () & SIMICS_FLAG)) {

        //
        // Check for any bit errors on any lane on SubCh-A.
        //
        if ((ErrorStatusDatSubChA | ErrorStatusEccSubChA) != 0) {

          if (TestType == ADV_MT_TYPE_LTEST_SCRAM) {
            Status = FAILURE;
          }

#ifdef DEBUG_CODE_BLOCK
          if (GetDebugLevel () & SDBG_MINMAX) {
            AcquirePrintControl ();

            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, SUB_CH_A, Rank, NO_STROBE, NO_BIT,
                            "Failure detected - Memtest Type %d, Checkpoint 0x%08x\n",
                            TestType, Host->var.mem.lastCheckpoint[Socket]);
            RcDebugPrint (SDBG_MINMAX,
                            "Logical Rank = %d, Subrank = %d, Bank = %d, Row = 0x%x, Column = 0x%x, Chunk = 0x%x\n",
                            CpgcErrorStatus[Ch].cpgcErrRank & WORD_MASK, CpgcErrorStatus[Ch].cpgcSubrank & WORD_MASK,
                            CpgcErrorStatus[Ch].cpgcErrBank & WORD_MASK, CpgcErrorStatus[Ch].cpgcErrRow,
                            CpgcErrorStatus[Ch].cpgcErrColumn, CpgcErrorStatus[Ch].cpgcErrChunk & WORD_MASK);
            RcDebugPrint (SDBG_MINMAX,
                            "DQ[39:0] Error on rising strobe:  0x%02x%02x%02x%02x%02x\n",
                            CpgcErrorStatus[Ch].cpgcErrEccS & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat0S >> (WORD_WIDTH + BYTE_WIDTH)) & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat0S >> WORD_WIDTH) & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat0S >> BYTE_WIDTH) & BYTE_MASK,
                            CpgcErrorStatus[Ch].cpgcErrDat0S & BYTE_MASK
                            );
            RcDebugPrint (SDBG_MINMAX,
                            "DQ[39:0] Error on falling strobe: 0x%02x%02x%02x%02x%02x\n",
                            (CpgcErrorStatus[Ch].cpgcErrEccS >> WORD_WIDTH) & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat2S >> (WORD_WIDTH + BYTE_WIDTH)) & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat2S >> WORD_WIDTH) & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat2S >> BYTE_WIDTH) & BYTE_MASK,
                            CpgcErrorStatus[Ch].cpgcErrDat2S & BYTE_MASK
                            );
            //
            // Output DIMM temperature
            //
            PrintDimmTemperature (Host, Socket, Ch, Dimm);

            ReleasePrintControl ();
          } // CheckMsgLevel
#endif
          //
          // Log that the error occurred
          //
          LogAdvMemtestError (Host, Socket, Ch, Dimm, Rank, WARN_FPT_ROW_FAILURE, TestType, CpgcErrorStatus[Ch], 1);

        } // if ErrorStatus on SubCh-A

        //
        // Check for any bit errors on any lane on SubCh-B.
        //
        if ((ErrorStatusDatSubChB | ErrorStatusEccSubChB) != 0) {

          if (TestType == ADV_MT_TYPE_LTEST_SCRAM) {
            Status = FAILURE;
          }

#ifdef DEBUG_CODE_BLOCK
          if (GetDebugLevel () & SDBG_MINMAX) {
            AcquirePrintControl ();

            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, SUB_CH_B, Rank, NO_STROBE, NO_BIT,
                            "Failure detected - Memtest Type %d, Checkpoint 0x%08x\n",
                            TestType, Host->var.mem.lastCheckpoint[Socket]);
            RcDebugPrint (SDBG_MINMAX,
                            "Logical Rank = %d, Subrank = %d, Bank = %d, RowExt = 0x%x, ColumnExt = 0x%x, Chunk = 0x%x\n",
                            (CpgcErrorStatus[Ch].cpgcErrRank >> WORD_WIDTH) & WORD_MASK, (CpgcErrorStatus[Ch].cpgcSubrank >> WORD_WIDTH) & WORD_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrBank >> WORD_WIDTH) & WORD_MASK, CpgcErrorStatus[Ch].cpgcErrRowExt,
                            CpgcErrorStatus[Ch].cpgcErrColumnExt, (CpgcErrorStatus[Ch].cpgcErrChunk >> WORD_WIDTH) & WORD_MASK);
            RcDebugPrint (SDBG_MINMAX,
                            "DQ[39:0] Error on rising strobe:  0x%02x%02x%02x%02x%02x\n",
                            (CpgcErrorStatus[Ch].cpgcErrEccS >> WORD_WIDTH) & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat1S >> (WORD_WIDTH + BYTE_WIDTH)) & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat1S >> WORD_WIDTH) & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat1S >> BYTE_WIDTH) & BYTE_MASK,
                            CpgcErrorStatus[Ch].cpgcErrDat1S & BYTE_MASK
                            );
            RcDebugPrint (SDBG_MINMAX,
                            "DQ[39:0] Error on falling strobe: 0x%02x%02x%02x%02x%02x\n",
                            ((CpgcErrorStatus[Ch].cpgcErrEccS >> WORD_WIDTH) >> WORD_WIDTH) & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat3S >> (WORD_WIDTH + BYTE_WIDTH)) & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat3S >> WORD_WIDTH) & BYTE_MASK,
                            (CpgcErrorStatus[Ch].cpgcErrDat3S >> BYTE_WIDTH) & BYTE_MASK,
                            CpgcErrorStatus[Ch].cpgcErrDat3S & BYTE_MASK
                            );

            PrintDimmTemperature (Host, Socket, Ch, Dimm);

            ReleasePrintControl ();
          } // CheckMsgLevel
#endif
          //
          // Log that the error occurred
          //
          LogAdvMemtestError (Host, Socket, Ch, Dimm, Rank, WARN_FPT_ROW_FAILURE, TestType, CpgcErrorStatus[Ch], 1);
        } // if ErrorStatus on SubCh-B
      } // if Emulation
    } // if (ChBitmask & (1 << Ch))
  } // Ch loop

  return Status;
} // CheckTestResultsMATSDdr5

#endif // #ifdef DDR5_SUPPORT

/**

  Checks the results of the preceding advanced memory test

  @param[in]  Host                 - Pointer to sysHost, the system Host (root) structure
  @param[in]  socket               - Socket Id
  @param[in]  chBitmask            - List of channels that were tested
  @param[in]  dimmRank             - DIMM containing logicalRank
  @param[in]  logical2Physical     - Geometery for logical rank within channel
  @param[in]  logicalRank          - Logical rank
  @param[in]  rowBits              - Number of row bits supported by current logical rank
  @param[in]  baseBits             - Number of bank bits in SW loop
  @param[in]  testType             - Memory test type for logging purposes
  @param[out] TestStatus[MAX_CH]   - Pass/fail status for the test per channel
  @param[in]  Direction            - Sequential address direction MT_ADDR_DIR_UP, MT_ADDR_DIR_DN

  @retval status - SUCCESS/FAILURE

**/
UINT32
CheckTestResultsMATS (
  IN     PSYSHOST                  Host,
  IN     UINT8                     socket,
  IN     UINT32                    chBitmask,
  IN     UINT8                     dimmRank[MAX_RANK_CH][MAX_CH],
  IN     UINT8                     logical2Physical[MAX_RANK_CH][MAX_CH],
  IN     UINT8                     logicalRank,
  IN     UINT8                     rowBits[MAX_CH],
  IN     UINT8                     baseBits,
  IN     UINT8                     testType,
     OUT UINT8                     TestStatus[MAX_CH],
  IN     UINT8                     Direction
  )
{
  UINT32              errorStatus0;
  UINT32              errorStatus1;
  UINT32              errorStatus2;
  UINT8               dimm;
  UINT8               ch;
  UINT8               rank;
  CPGC_ERROR_STATUS_MATS   cpgcErrorStatus[MAX_CH];
  UINT32              status = SUCCESS;
  UINT8               MaxChDdr;

  if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
    return CheckTestResultsMATSDdr5 (Host, socket, chBitmask, dimmRank, logical2Physical, logicalRank, rowBits, baseBits, testType, TestStatus, Direction);
#endif
  }

  MaxChDdr = GetMaxChDdr ();
  //
  // Collect test results
  //
  CpgcAdvTrainingErrorStatusMATS (Host, socket, chBitmask, cpgcErrorStatus);

  //
  // Inject error for testing purposes
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (chBitmask & (1 << ch)) {
      InjectErrorTestResultsMATS (Host, socket, ch, logicalRank, cpgcErrorStatus);
    }
  }

  if ((testType != ADV_MT_TYPE_LTEST_SCRAM) &&
      (testType != ADV_MT_TYPE_LINIT_SCRAM)) {

    //
    // Update Row Range Failure tracking
    //
    status = UpdateRowFailures (Host, socket, chBitmask, cpgcErrorStatus, rowBits, baseBits, TestStatus, Direction);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    dimm = dimmRank[logicalRank][ch];
    rank = logical2Physical[logicalRank][ch];

    //
    // Was this channel part of the test group?
    //
    if (chBitmask & (1 << ch)) {
      //
      // Evaluate error status
      // Do not log failures on 1st try
      //
      errorStatus0 = 0;
      errorStatus1 = 0;
      errorStatus2 = 0;

      if (GetAmtRetryState (Host)) {

        errorStatus0 = cpgcErrorStatus[ch].cpgcErrDat0S | cpgcErrorStatus[ch].cpgcErrDat2S;;
        errorStatus1 = cpgcErrorStatus[ch].cpgcErrDat1S | cpgcErrorStatus[ch].cpgcErrDat3S;
        if (Host->nvram.mem.eccEn) {
          errorStatus2 = (cpgcErrorStatus[ch].cpgcErrEccS & 0xFF) | ((cpgcErrorStatus[ch].cpgcErrEccS >> 8) & 0xFF);
        }
      }

      if (!(GetEmulation () & SIMICS_FLAG)) {

        // Check for any bit errors on any lane. Legacy path.
        if ((errorStatus0 | errorStatus1 | errorStatus2) != 0) {

          if (testType == ADV_MT_TYPE_LTEST_SCRAM) {
            status = FAILURE;
          }

#ifdef DEBUG_CODE_BLOCK
          if (GetDebugLevel () & SDBG_MINMAX) {
            AcquirePrintControl ();

            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "Failure detected - Memtest Type %d, Checkpoint 0x%08x\n",
                            testType, Host->var.mem.lastCheckpoint[socket]);
            RcDebugPrint (SDBG_MINMAX,
                            "Logical Rank = %d, Subrank = %d, Bank = %d, Row = 0x%x, Column = 0x%x, Chunk = 0x%x\n",
                            cpgcErrorStatus[ch].cpgcErrRank, cpgcErrorStatus[ch].cpgcSubrank,
                            cpgcErrorStatus[ch].cpgcErrBank, cpgcErrorStatus[ch].cpgcErrRow,
                            cpgcErrorStatus[ch].cpgcErrColumn, cpgcErrorStatus[ch].cpgcErrChunk);
            RcDebugPrint (SDBG_MINMAX,
                            "DQ[71:0] Error on rising strobe:  0x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
                            (cpgcErrorStatus[ch].cpgcErrEccS >> 0) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat1S >> 24) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat1S >> 16) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat1S >> 8) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat1S >> 0) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat0S >> 24) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat0S >> 16) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat0S >> 8) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat0S >> 0) & 0xFF
                            );
            RcDebugPrint (SDBG_MINMAX,
                            "DQ[71:0] Error on falling strobe: 0x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
                            (cpgcErrorStatus[ch].cpgcErrEccS >> 8) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat3S >> 24) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat3S >> 16) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat3S >> 8) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat3S >> 0) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat2S >> 24) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat2S >> 16) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat2S >> 8) & 0xFF,
                            (cpgcErrorStatus[ch].cpgcErrDat2S >> 0) & 0xFF
                            );

            //
            // Output DIMM temperature
            //
            PrintDimmTemperature (Host, socket, ch, dimm);

            ReleasePrintControl ();
          } // checkMsgLevel
#endif
          // Log that the error occurred
          LogAdvMemtestError (Host, socket, ch, dimm, rank, WARN_FPT_ROW_FAILURE, testType, cpgcErrorStatus[ch], 1);

        } // if errorStatus
      } // if emulation
    } // if (chBitmask & (1 << ch))
  } // ch loop

  return status;
} // CheckTestResultsMATS


/**

  Classifies the DQ errors as correctable or uncorrectable based on current mode of operation

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ch                  - Channel number
  @param dimm                - DIMM number (0-based)
  @param rank                - Rank number (0-based)
  @param errorStatus0        - DQ[31:0] test result
  @param errorStatus1        - DQ[63:32] test result
  @param errorStatus2        - DQ[71:64] test result

  @retval ErrorTypeCorrectable
          ErrorTypeUncorrectable

**/
MEM_TEST_ERROR_TYPE
ClassifyDqErrorStatus (
  PSYSHOST                  Host,
  UINT8                     socket,
  UINT8                     ch,
  UINT8                     dimm,
  UINT8                     rank,
  UINT32                    errorStatus0,
  UINT32                    errorStatus1,
  UINT32                    errorStatus2
  )
{
  UINT8               errorCount = 0;
  UINT8               errorType = ErrorTypeCorrectable;
  UINT8               i;
  UINT8               strobe;
  UINT8               temp;
  UINT8               nibble = 0;
  UINT32              errorStatus[3];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];
  UINT8               MSVx4 = GetMaxStrobeValid(Host);

  dimmNvList = GetDimmNvList (Host, socket, ch);
  rankList = GetRankNvList (Host, socket, ch, dimm);

  errorStatus[0] = errorStatus0;
  errorStatus[1] = errorStatus1;
  errorStatus[2] = errorStatus2;

  // Check for any bit errors on any lane.
  if ((errorStatus[0] | errorStatus[1] | errorStatus[2]) != 0) {
    //
    // Now check to see if the errors found are correctable or uncorrectable.
    //
    // For failures that only fall within a DQ nibble (regardless of x4, x8 or x16 DRAM device width),
    // if there is one DQ lane failure or 2 non-adjacent lane failures, then treat the failure as correctable
    // otherwise treat the failure as uncorrectable.
    //
    if (!Host->nvram.mem.eccEn) {
      errorType = ErrorTypeUncorrectable;

    } else {

      for (i = 0; i < MSVx4; i++) {
        temp = (errorStatus[i / 8] >> ((i % 8) * 4)) & 0xF;
        if (temp) {
          errorCount++;
          nibble = temp;
          strobe = GetStrobeFromNibble (Host, i);
          (*rankList)[rank].faultyParts[strobe] |= FPT_MEM_BIST_FAILED;
        }
      }
      //
      // If we detect errors on more than 1 nibble, then it is uncorrectable.
      //
      if (errorCount > 1) {
        errorType = ErrorTypeUncorrectable;
      } else if (errorCount == 1) {
        //
        // If we detect errors on adjacent DQ lanes, treat it as uncorrectable
        switch (nibble) {
          case 0x3:
          case 0x6:
          case 0x7:
          case 0xb:
          case 0xc:
          case 0xd:
          case 0xe:
          case 0xf:
            errorType = ErrorTypeUncorrectable;
            break;
          default:
            break;
        }
      }
    }
  }
  return errorType;
} //ClassifyDqErrorStatus

#ifdef DDR5_SUPPORT

/**

  Worker to checks the results of the preceding memory test for Ddr5

  @param[in]  Host                            - Pointer to sysHost, the system Host (root) structure
  @param[in]  Socket                          - Socket Id
  @param[in]  Ch                              - Channel Id
  @param[in]  SubCh                           - Sub-channel Id
  @param[in]  Dimm                            - Dimm Id
  @param[in]  LogicalRank                     - Logical rank Id
  @param[in]  LogicalSubRank                  - Logical sub-rank Id
  @param[in]  ErrorStatusDat                  - 32 bits data lane error status
  @param[in]  ErrorStatusEcc                  - 8 bits ecc lane error status
  @param[in]  ErrorStatusBank                 - Bank Id that error happens on
  @param[out] ChBitmaskOut                    - List of channels that were tested and still have rank enabled
  @param[in]  IsAllowCorrectableMemTestError  - Chech whether or not correctable mem test error is allowed
  @param[in]  SecondPass                      - Second Pass check: 0 = 1st Pass, 1 = 2nd Pass

  @retval Status - SUCCESS/FAILURE

**/
UINT32
CheckTestResultsWorkerDdr5 (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch,
  IN      UINT8       SubCh,
  IN      UINT8       Dimm,
  IN      UINT8       LogicalRank,
  IN      UINT8       LogicalSubRank,
  IN      UINT32      ErrorStatusDat,
  IN      UINT32      ErrorStatusEcc,
  IN      UINT8       ErrorStatusBank,
      OUT UINT32      *ChBitmaskOut,
  IN      BOOLEAN     IsAllowCorrectableMemTestError,
  IN      BOOLEAN     SecondPass
  )
{
  UINT32                  ErrorBits[3];
  UINT8                   *DqBytes;
  MEM_TEST_ERROR_TYPE     ErrorType;
  UINT8                   PhyRank;

  PhyRank = LogicalRank % MAX_RANK_DIMM;
  //
  // Clear error counter
  //
  ErrorType = ErrorTypeCorrectable;

  //
  // Read error status
  //
  ErrorBits[0] = ErrorStatusDat;
  ErrorBits[1] = 0; // lane [63:32] is not available
  ErrorBits[2] = ErrorStatusEcc;
  DqBytes = (UINT8 *) ErrorBits;

  if (!(GetEmulation () & SIMICS_FLAG)) {

    //
    // Check for any bit errors on any lane on SubCh-A.
    //
    if ((ErrorStatusDat | ErrorStatusEcc) != 0) {
#ifdef DEBUG_CODE_BLOCK
      if (GetDebugLevel () & SDBG_MINMAX) {
        AcquirePrintControl ();

        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, SubCh, PhyRank, NO_STROBE, NO_BIT, "MemTest Failure!\n");
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, SubCh, PhyRank, NO_STROBE, NO_BIT,"B3:B0 = 0x%x\n", ErrorStatusDat);
        if (Host->nvram.mem.eccEn) {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, SubCh, PhyRank, NO_STROBE, NO_BIT,"ECC = 0x%x\n", ErrorStatusEcc);
        }
        ReleasePrintControl ();
      }
#endif
      //
      // Now check to see if the errors found are correctable or uncorrectable.
      // For failures only fall within x4 aligned lanes, i.e. failed lanes within a single nibble,
      // e.g. lane 15:12 (regardless of running independent mode or lockstep), you can treat it is correctable error.
      // Otherwise, for failures within x8 device aligned lanes, i.e. fail lanes within a double-nibble
      // (e.g. lane 15:8 is but lane 11:4 is not) and running lock-step mode, you can treat the error as correctable error.
      // Otherwise, remaining failures are UC.
      //
      ErrorType = ClassifyDqErrorStatus (Host, Socket, Ch, Dimm, PhyRank, ErrorStatusDat, 0, ErrorStatusEcc);

      //
      // If the error is uncorrectable and this was the 2nd pass through memtest, disable the rank.
      //
      if (ErrorType == ErrorTypeUncorrectable && SecondPass) {
        //
        // Add this error to the warning log for both correctable and uncorrectable errors.
        //
        DisableRank (Host, Socket, Ch, Dimm, PhyRank);
        *ChBitmaskOut &= ~(1 << Ch);
        EwlOutputType5 (WARN_MEMTEST_DIMM_DISABLE, WARN_FPT_MINOR_MEM_TEST, Socket, Ch, Dimm, PhyRank, EwlSeverityWarning, LogicalSubRank, ErrorStatusBank, DqBytes);
      } else if (ErrorType == ErrorTypeUncorrectable && !SecondPass) {
        //
        // Else if the error was uncorrectable and this is the first pass through memtest, run it again on the same rank.
        //
        return FAILURE;
      } else if (ErrorType == ErrorTypeCorrectable) {
        //
        // Add this error to the warning log for correctable errors.
        //
        EwlOutputType5 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_MEM_TEST, Socket, Ch, Dimm, PhyRank, EwlSeverityWarning, LogicalSubRank, ErrorStatusBank, DqBytes);
        if (!IsAllowCorrectableMemTestError) {
          //
          // If correctable mem test error question is disabled, then check the pass
          //
          if (SecondPass) {
            //
            // if this is second pass, remove Rank
            //
            DisableRank (Host, Socket, Ch, Dimm, PhyRank);
          } else {
            //
            // If first pass then return failure, for another try
            //
            return FAILURE;
          }
        }
      }
    } // if Error
  } // if Emulation

  //
  // This means we do not need to rerun the memtest.  Either it passed or the rank was disabled after
  // 2 failing tests in a row.  Returning SUCCESS means that we are done testing this rank.
  //
  return SUCCESS;
} // CheckTestResultsWorkerDdr5

/**

  Checks the results of the preceding memory test

  @param[in] Host             - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket           - Socket Id
  @param[in] ChBitmask        - List of channels that were tested
  @param[in] ChBitmaskOut     - List of channels that were tested and still have rank enabled
  @param[in] DimmRank         - DIMM containing logicalRank
  @param[in] Logical2Physical - Geometery for logical rank within channel
  @param[in] LogicalRank      - Logical rank
  @param[in] LogicalSubRank   - SubRank
  @param[in] SecondPass       - Second Pass check: 0 = 1st Pass, 1 = 2nd Pass

  @retval Status - SUCCESS/FAILURE

**/
UINT32
CheckTestResultsDdr5 (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT32      ChBitmaskIn,
  IN UINT32      *ChBitmaskOut,
  IN UINT8       DimmRank[MAX_RANK_CH][MAX_CH],
  IN UINT8       Logical2Physical[MAX_RANK_CH][MAX_CH],
  IN UINT8       LogicalRank,
  IN UINT8       LogicalSubRank,
  IN BOOLEAN     SecondPass
  )
{
  UINT32                  ErrorStatusDatSubChA;
  UINT32                  ErrorStatusDatSubChB;
  UINT32                  ErrorStatusEccSubChA = 0;
  UINT32                  ErrorStatusEccSubChB = 0;
  UINT8                   Dimm;
  UINT8                   Ch;
  UINT8                   Rank;
  CPGC_ERROR_STATUS_MATS  CpgcErrorStatus[MAX_CH];
  SYS_SETUP               *Setup;
  UINT8                   MaxChDdr;
  BOOLEAN                 IsAllowCorrectableMemTestError;
  UINT32                  Status = SUCCESS;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  IsAllowCorrectableMemTestError = ((Setup->mem.optionsExt & ALLOW_CORRECTABLE_MEM_TEST_ERROR) != 0) ? TRUE : FALSE;

  *ChBitmaskOut = ChBitmaskIn;

  //
  // Collect test results
  //
  CpgcAdvTrainingErrorStatusMATS (Host, Socket, ChBitmaskIn, CpgcErrorStatus);

  //
  // Inject error for testing purposes
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (ChBitmaskIn & (1 << Ch)) {
      InjectErrorTestResultsMATS (Host, Socket, Ch, LogicalRank, CpgcErrorStatus);
      break;
    }
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    Dimm = DimmRank[LogicalRank][Ch];
    Rank = Logical2Physical[LogicalRank][Ch];

    //
    // Was this channel part of the test group?
    //
    if (ChBitmaskIn & (1 << Ch)) {
      //
      // Read error status
      //
      ErrorStatusDatSubChA = CpgcErrorStatus[Ch].cpgcErrDat0S | CpgcErrorStatus[Ch].cpgcErrDat2S;;
      ErrorStatusDatSubChB = CpgcErrorStatus[Ch].cpgcErrDat1S | CpgcErrorStatus[Ch].cpgcErrDat3S;
      if (Host->nvram.mem.eccEn) {
        ErrorStatusEccSubChA = (CpgcErrorStatus[Ch].cpgcErrEccS & BYTE_MASK) | ((CpgcErrorStatus[Ch].cpgcErrEccS >> BYTE_WIDTH) & BYTE_MASK);
        ErrorStatusEccSubChB = ((CpgcErrorStatus[Ch].cpgcErrEccS >> WORD_WIDTH) & 0xFF) | (((CpgcErrorStatus[Ch].cpgcErrEccS >> (WORD_WIDTH + BYTE_WIDTH))) & 0xFF);
      }
      //
      // SubCh-A
      //
      Status = CheckTestResultsWorkerDdr5 (Host, Socket, Ch, SUB_CH_A, Dimm, LogicalRank, LogicalSubRank,
             ErrorStatusDatSubChA, ErrorStatusEccSubChA, (UINT8)( CpgcErrorStatus[Ch].cpgcErrBank & BYTE_MASK),
             ChBitmaskOut, IsAllowCorrectableMemTestError, SecondPass);

      if (Status != SUCCESS) {
        return FAILURE;
      }

      //
      // SubCh-B
      //
      Status = CheckTestResultsWorkerDdr5 (Host, Socket, Ch, SUB_CH_B, Dimm, LogicalRank, LogicalSubRank,
             ErrorStatusDatSubChB, ErrorStatusEccSubChB, (UINT8) ((CpgcErrorStatus[Ch].cpgcErrBank >> WORD_WIDTH) & BYTE_MASK),
             ChBitmaskOut, IsAllowCorrectableMemTestError, SecondPass);

      if (Status != SUCCESS) {
        return FAILURE;
      }

    } // if (ChBitmask & (1 << Ch))
  } // Ch loop

  return Status;
} // CheckTestResultsDdr5

#endif

/**

  Checks the results of the preceding memory test

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - Socket Id
  @param chBitmask        - List of channels that were tested
  @param chBitmaskOut     - List of channels that were tested and still have rank enabled
  @param dimmRank         - DIMM containing logicalRank
  @param logical2Physical - Geometery for logical rank within channel
  @param logicalRank      - Logical rank
  @param logicalSubRank   - SubRank
  @param secondPass       - Second Pass check: 0 = 1st Pass, 1 = 2nd Pass

  @retval status - SUCCESS/FAILURE

**/
UINT32
CheckTestResults (
  PSYSHOST                  Host,
  UINT8                     socket,
  UINT32                    chBitmaskIn,
  UINT32                   *chBitmaskOut,
  UINT8                     dimmRank[MAX_RANK_CH][MAX_CH],
  UINT8                     logical2Physical[MAX_RANK_CH][MAX_CH],
  UINT8                     logicalRank,
  UINT8                     logicalSubRank,
  BOOLEAN                   secondPass
  )
{
  UINT32              errorStatus0;
  UINT32              errorStatus1;
  UINT32              errorStatus2;
  UINT32              errorBits[3];
  UINT8               *dqBytes;
  MEM_TEST_ERROR_TYPE errorType;
  UINT8               dimm;
  UINT8               ch;
  UINT8               rank;
  CPGC_ERROR_STATUS_MATS cpgcErrorStatus[MAX_CH];
  SYS_SETUP *Setup;
  UINT8 MaxChDdr;

  if (IsDdr5Present (Host, socket)) {
#ifdef DDR5_SUPPORT
    CheckTestResultsDdr5 (Host, socket, chBitmaskIn, chBitmaskOut, dimmRank, logical2Physical, logicalRank, logicalSubRank, secondPass);
#endif
  }

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  *chBitmaskOut = chBitmaskIn;

  //
  // Collect test results
  //
  CpgcAdvTrainingErrorStatusMATS (Host, socket, chBitmaskIn, cpgcErrorStatus);

  //
  // Inject error for testing purposes
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (chBitmaskIn & (1 << ch)) {
      InjectErrorTestResultsMATS (Host, socket, ch, logicalRank, cpgcErrorStatus);
      break;
    }
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    dimm = dimmRank[logicalRank][ch];
    rank = logical2Physical[logicalRank][ch];

    //
    // Clear error counter
    //
    errorType = ErrorTypeCorrectable;

    //
    // Was this channel part of the test group?
    //
    if (chBitmaskIn & (1 << ch)) {
      //
      // Read error status
      //
      errorStatus0 = cpgcErrorStatus[ch].cpgcErrDat0S | cpgcErrorStatus[ch].cpgcErrDat2S;
      errorStatus1 = cpgcErrorStatus[ch].cpgcErrDat1S | cpgcErrorStatus[ch].cpgcErrDat3S;
      if (Host->nvram.mem.eccEn) {
        errorStatus2 = (cpgcErrorStatus[ch].cpgcErrEccS & 0xFF) | ((cpgcErrorStatus[ch].cpgcErrEccS >> 8) & 0xFF);
      } else {
        errorStatus2 = 0;
      }
      errorBits[0] = errorStatus0;
      errorBits[1] = errorStatus1;
      errorBits[2] = errorStatus2;
      dqBytes = (UINT8 *) errorBits;

      if (!(GetEmulation () & SIMICS_FLAG)) {

        // Check for any bit errors on any lane.
        if ((errorStatus0 | errorStatus1 | errorStatus2) != 0) {
#ifdef DEBUG_CODE_BLOCK
          if (GetDebugLevel () & SDBG_MINMAX) {
            AcquirePrintControl ();

            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "MemTest Failure!\n");
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "B0:B3 = 0x%x\n", errorStatus0);
            RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "B4:B7 = 0x%x\n", errorStatus1);
            if (Host->nvram.mem.eccEn) {
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                              "ECC = 0x%x\n", errorStatus2);
            }
            ReleasePrintControl ();
          }
#endif
          //
          // Now check to see if the errors found are correctable or uncorrectable.
          // For failures only fall within x4 aligned lanes, i.e. failed lanes within a single nibble,
          // e.g. lane 15:12 (regardless of running independent mode or lockstep), you can treat it is correctable error.
          // Otherwise, for failures within x8 device aligned lanes, i.e. fail lanes within a double-nibble
          // (e.g. lane 15:8 is but lane 11:4 is not) and running lock-step mode, you can treat the error as correctable error.
          // Otherwise, remaining failures are UC.
          //
          errorType = ClassifyDqErrorStatus (Host, socket, ch, dimm, rank, errorStatus0, errorStatus1, errorStatus2);

          //
          // If the error is uncorrectable and this was the 2nd pass through memtest, disable the rank.
          //
          if (errorType == ErrorTypeUncorrectable && secondPass) {
            //
            // Add this error to the warning log for both correctable and uncorrectable errors.
            //
            DisableRank (Host, socket, ch, dimm, rank);
            *chBitmaskOut &= ~(1 << ch);
            EwlOutputType5 (WARN_MEMTEST_DIMM_DISABLE, WARN_FPT_MINOR_MEM_TEST, socket, ch, dimm, rank, EwlSeverityWarning, logicalSubRank, (UINT8)cpgcErrorStatus[ch].cpgcErrBank, dqBytes);
          }
          //
          // Else if the error was uncorrectable and this is the first pass through memtest, run it again on the same rank.
          //
          else if (errorType == ErrorTypeUncorrectable && !secondPass) {

            return FAILURE;
          } else if (errorType == ErrorTypeCorrectable) {

            //
            // Add this error to the warning log for correctable errors.
            //
            EwlOutputType5 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_MEM_TEST, socket, ch, dimm, rank, EwlSeverityWarning, logicalSubRank, (UINT8)cpgcErrorStatus[ch].cpgcErrBank, dqBytes);
            if (!(Setup->mem.optionsExt & ALLOW_CORRECTABLE_MEM_TEST_ERROR)) {
              //
              // If correctable mem test error question is disabled, then check the pass
              //
              if (secondPass) {
                //
                // if this is second pass, remove Rank
                //
                DisableRank (Host, socket, ch, dimm, rank);
              } else {
                //
                // If first pass then return failure, for another try
                //
                return FAILURE;
              }
            }
          }
        } // if error
      } // if emulation
    } // if (chBitmask & (1 << ch))
  } // ch loop

  //
  // This means we do not need to rerun the memtest.  Either it passed or the rank was disabled after
  // 2 failing tests in a row.  Returning SUCCESS means that we are done testing this rank.
  //
  return SUCCESS;
} // CheckTestResults


/**

  Adds new entry to the PPR Address list. If a duplicate entry is found, then DRAM mask field is updated.

  @param NewPprEntry      - New PPR address to add
  @param PprAddrList      - List of PPR addresses to be repaired
  @param MaxEntries       - Max number of entries allowed in PprAddrList
  @param MaxPprValidEntry - Max number of valid entries in the list

  @retval SUCCESS/FAILURE

**/
UINT32
AddPprEntryToList (
  PPR_ADDR_MRC_SETUP *NewPprEntry,
  PPR_ADDR_MRC_SETUP *PprAddrList,
  UINT32              MaxEntries,
  UINT32             *MaxPprValidEntry
)
{
  UINT32              Index;
  PPR_ADDR_MRC_SETUP  Temp1Entry;
  PPR_ADDR_MRC_SETUP  Temp2Entry;
  UINT32              Status = SUCCESS;

  CopyMem ((UINT8 *)&Temp1Entry, (UINT8 *)NewPprEntry, sizeof (PPR_ADDR_MRC_SETUP));
  Temp1Entry.pprAddr.nibbleMask = 0;

  for (Index = 0; Index < MaxEntries; Index++) {

    CopyMem ((UINT8 *)&Temp2Entry, (UINT8 *)&PprAddrList[Index], sizeof (PPR_ADDR_MRC_SETUP));
    Temp2Entry.pprAddr.nibbleMask = 0;

    if (Temp2Entry.pprAddrStatus != PPR_STS_ADDR_VALID) {
      //
      // Entry is not used, so copy new entry
      //
      CopyMem ((UINT8 *)&PprAddrList[Index], (UINT8 *)NewPprEntry, sizeof (PPR_ADDR_MRC_SETUP));
      break;

    } else if (CompareMem ((UINT8 *)&Temp1Entry, (UINT8 *)&Temp2Entry, sizeof(PPR_ADDR_MRC_SETUP)) == 0) {
      //
      // Entry is duplicate, so just update DRAM mask
      //
      PprAddrList[Index].pprAddr.nibbleMask |= NewPprEntry->pprAddr.nibbleMask;
      break;
    }
  }
  //
  // Check for errors
  //
  if (Index == MaxEntries) {
    Status = FAILURE;
  }
  *MaxPprValidEntry = Index + 1;

  return Status;

}  // AddPprEntryToList

/**

  Searches the Row failure list to classify errors as correctable/uncorrectable and
  apply error handling policy like CheckTestResults

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - Socket Id
  @param chBitmask        - List of channels that were tested
  @param dimmRank         - DIMM containing logicalRank
  @param logical2Physical - Geometery for logical rank within channel
  @param logicalRank      - Logical rank
  @param logicalSubRank   - SubRank
  @param baseBits         - Number of bank bits in SW loop
  @param RepairDone       - Set if at least one PPR repair was done, so retest is needed

  @retval  SUCCESS = PPR repairs not needed, or all PPR repairs succeeded and Row Fail list modified
  @retval  FAILURE = PPR repairs failed, Row Fail list not modified

**/
UINT32
DispositionFailRangesWithPprFlow (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChBitmask,
  UINT8     DimmRank[MAX_RANK_CH][MAX_CH],
  UINT8     Logical2Physical[MAX_RANK_CH][MAX_CH],
  UINT8     LogicalRank,
  UINT8     LogicalSubRank,
  UINT8     BaseBits,
  UINT8     *RepairDone
)
{
  ROW_FAIL_RANGE      *FailRangePtr;
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               Rank;
  UINT8               Bank;
  UINT8               BankGroup;
  UINT8               PprBank;
  UINT8               NumBaBits;
  UINT8               Nibble;
  UINT32              NibbleMask;
  UINT32              TempNibbleMask;
  UINT16              FailRow;
  UINT32              FailIndex;
  ROW_ADDR            CurAddr;
  UINT32              Index;
  UINT32              Status;
  UINT32              RetVal = SUCCESS;
  PPR_ADDR_MRC_SETUP  NewPprEntry;
  PPR_ADDR_MRC_SETUP  PprAddrList[MAX_PPR_ADDR_ENTRIES];
  UINT8               PprStatus[MAX_PPR_ADDR_ENTRIES_SPPR];
  UINT8               RepairStatus;
  CPGC_ERROR_STATUS_MATS   cpgcErrorStatus;
  UINT32              DqMask[3];
  UINT32              BgDqMask[2][3];
  UINT32              MaxPprValidEntry = 0;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  EWL_ENTRY_TYPE4     *warningPtr;
  UINT32              MaxPprAddrListSize;
  PPR_ADDR_MRC_SETUP  *PprAddrListPtr = NULL;
  UINT8               MaxChDdr;
  UINT8               NumChPerMc;
  SYS_SETUP          *Setup;
  UINT8               LowNibble;
  UINT8               BuddyByte;
  UINT8               MSVx4 = GetMaxStrobeValid(Host);
  ROW_FAIL_RANGE      TempFail;
  UINT32              DqMaskTemp[3];
  BOOLEAN             ListUpdate;

  Setup = GetSysSetupPointer();
  MaxChDdr = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "DispositionFailRangesWithPprFlow Starts:  ChBitMask=0x%x\n", ChBitmask);

  //
  // Inspect test results for given logicalRank, logicalSubRank, bank
  //
  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    //
    // Was this channel part of the test group?
    //
    if (ChBitmask & (1 << Ch)) {

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      Dimm = DimmRank[LogicalRank][Ch];
      Rank = Logical2Physical[LogicalRank][Ch];

      //
      // For each bank pair
      //
      for (Bank = 0; Bank < (1 << BaseBits); Bank++) {

        CurAddr.Data = 0;
        CurAddr.Bits.bankPair = Bank;
        CurAddr.Bits.logicalSubRank = LogicalSubRank;
        CurAddr.Bits.logicalRank = LogicalRank;
        CurAddr.Bits.valid = 1;
        //
        // Loop through all row failure ranges
        //
        for (FailIndex = 0; FailIndex < GetRowFailMax (Host, Ch); FailIndex++) {

          FailRangePtr = GetRowFailRange (Host, Ch, FailIndex);

          //
          // Check for address match, masking row number
          //
          if (CurrentAddrMatch (CurAddr, FailRangePtr->addr)) {

            Status = FAILURE;
            ZeroMem ((UINT8 *)&BgDqMask, sizeof (BgDqMask));

            for (BankGroup = 0; BankGroup < 2; BankGroup++) {
              //
              // If current BG has a failure
              //
              if (FailRangePtr->addr.Bits.upperBgMask & (1 << BankGroup)) {
                PprBank = (BankGroup << BaseBits) + Bank;

                //
                // Initialize DqMask per BG
                //
                for (Index = 0; Index < 3; Index++) {
                  BgDqMask[BankGroup][Index] = FailRangePtr->mask[Index];
                }

                // TODO - make number of BA bits dynamic for DDR5
                NumBaBits = 2;

                //
                // Clear PPR list and status
                //
                ZeroMem ((UINT8 *)&PprAddrList, sizeof (PprAddrList));
                ZeroMem ((UINT8 *)&PprStatus, sizeof (PprStatus));

                //
                // Construct DRAM Mask
                //
                NibbleMask = 0;

                //
                // Get dq mask over threshold
                // Invert then &= with FailRangePtr->mask
                //
                TempFail = *FailRangePtr;
                GetDqMaskOverThreshold (Host, Socket, Ch, TempFail, MAX_BAD_ROW_LIMIT_PER_BANK, DqMaskTemp);
                for (Index = 0; Index < 3; Index++) {
                  DqMaskTemp[Index] = ~DqMaskTemp[Index] & TempFail.mask[Index];
                }

                for (Nibble = 0; Nibble < MSVx4; Nibble++) {

                  //
                  // Set Nibble mask bits based on failing DQ bits
                  //
                  if ((DqMaskTemp[Nibble / 8] >> ((Nibble % 8) * 4)) & 0xF) {

                    if (GetPprResourceAvailable (Host, Socket, Ch, Dimm, Rank, LogicalSubRank, Nibble, PprBank >> NumBaBits)) {

                      if ((*DimmNvList)[Dimm].x4Present) {
                        NibbleMask |= (1 << Nibble);
                      } else {
                        LowNibble = Nibble & ~1;
                        NibbleMask |= (3 << LowNibble);
                      }
                      if ((*ChannelNvList)[Ch].features & X16_PRESENT) {
                        BuddyByte = (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Nibble / 2];
                        if (BuddyByte != 0xFF) {
                          LowNibble = BuddyByte * 2;
                          NibbleMask |= (3 << LowNibble);
                        }
                      }
                    }
                  }
                }
                //
                // If NibbleMask bits are set, then execute a PPR repair flow
                //
                if (NibbleMask) {

                  //
                  // Handle Soft PPR or Hard PPR types
                  //
                  if (Setup->mem.pprType == PPR_TYPE_SOFT) {
                    //
                    // Append entries to end of global list for sPPR
                    //
                    MaxPprAddrListSize = MAX_PPR_ADDR_ENTRIES_SPPR - Host->var.mem.NumValidPprEntries;
                    PprAddrListPtr = &Host->var.mem.pprAddrSetup[Host->var.mem.NumValidPprEntries];

                  } else {
                    //
                    // Initialize local list for hPPR
                    //
                    MaxPprAddrListSize = MAX_PPR_ADDR_ENTRIES;
                    PprAddrListPtr = PprAddrList;
                  }

                  // check host->var.mem.pprAddrSetup against MAX_PPR_ADDR_ENTRIES_SPPR
                  if (FailRangePtr->size <= MaxPprAddrListSize) {

                    Status = SUCCESS;
                    for (FailRow = 0; (FailRow < FailRangePtr->size) && (Status == SUCCESS); FailRow++) {

                      //
                      // Initialize New PPR entry
                      //
                      NewPprEntry.pprAddrStatus = PPR_STS_ADDR_VALID;
                      NewPprEntry.socket = Socket;
                      NewPprEntry.mc = Ch / NumChPerMc;
                      NewPprEntry.ch = Ch % NumChPerMc;
                      NewPprEntry.pprAddr.dimm = Dimm;
                      NewPprEntry.pprAddr.rank = Rank;
                      NewPprEntry.pprAddr.subRank = LogicalSubRank;
                      NewPprEntry.pprAddr.nibbleMask = NibbleMask;
                      NewPprEntry.pprAddr.bank = PprBank;
                      NewPprEntry.pprAddr.row = FailRangePtr->addr.Bits.row + FailRow;

                      //
                      // Add New PPR entry to the list
                      //
                      Status = AddPprEntryToList (&NewPprEntry, PprAddrListPtr, MaxPprAddrListSize, &MaxPprValidEntry);
                    } // FailRow

                    //
                    // Fixup sPPR list size to include new entries
                    //
                    if (Setup->mem.pprType == PPR_TYPE_SOFT) {
                      Host->var.mem.NumValidPprEntries += MaxPprValidEntry;
                    }

                  } else {

                    Status = FAILURE;
                    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                      "DispositionFailRangesWithPprFlow: buffer size %d too small for %d row failures\n",
                      MaxPprAddrListSize, FailRangePtr->size);

                  } // FailRangePtr->size
                } // if (NibbleMask)

                RepairStatus = 0;
                if (Status == SUCCESS) {

                  //
                  // Do the PPR repairs
                  //
                  Status = PostPackageRepair (Host, Socket, ChBitmask, Setup->mem.pprType, &Host->var.mem.softPprDone[Socket],
                    MaxPprValidEntry, PprAddrListPtr, (UINT8 *)&PprStatus);

                  //
                  // Search PprStatus array, looking for FAILURE or SUCCESS
                  //
                  RepairStatus = 1;
                  for (Index = 0; Index < MaxPprValidEntry; Index++) {
                    if (PprAddrListPtr[Index].pprAddrStatus == PPR_STS_ADDR_VALID) {

                      if (PprStatus[Index] == PPR_STS_FAILED) {
                        //
                        // Return failure
                        //
                        RepairStatus = 0;
                        Status = FAILURE;

                      } else {
                        //
                        // Assume SUCCESS
                        //
                        // Log EWL entry per DRAM device to indicate PPR repair occurred
                        // Convert NibbleMask to cpgcErr format
                        //
                        TempNibbleMask = NibbleMask;
                        for (Nibble = 0; Nibble < MSVx4; Nibble++) {
                          ZeroMem ((UINT8 *)DqMask, sizeof (DqMask));
                          if (TempNibbleMask & (1 << Nibble)) {

                            //
                            // Update DqMask and clear TempNibbleMask so we don't log multiple repairs for the same DRAM device
                            //
                            DimmNvList = GetDimmNvList (Host, Socket, Ch);
                            if ((*DimmNvList)[Dimm].x4Present) {
                              DqMask[Nibble / 8] |= (0xF << ((Nibble % 8) * 4));
                            } else {
                              LowNibble = Nibble & ~1;
                              DqMask[LowNibble / 8] |= (0xFF << ((LowNibble % 8) * 4));
                              TempNibbleMask &= ~(3 << LowNibble);
                            }
                            if ((*ChannelNvList)[Ch].features & X16_PRESENT) {
                              BuddyByte = (*DimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Nibble / 2];
                              if (BuddyByte != 0xFF) {
                                LowNibble = BuddyByte * 2;
                                DqMask[LowNibble / 8] |= (0xFF << ((LowNibble % 8) * 4));
                                TempNibbleMask &= ~(3 << LowNibble);
                              }
                            }
                            cpgcErrorStatus.cpgcErrDat0S = DqMask[0];
                            cpgcErrorStatus.cpgcErrDat1S = DqMask[1];
                            cpgcErrorStatus.cpgcErrDat2S = DqMask[0];
                            cpgcErrorStatus.cpgcErrDat3S = DqMask[1];
                            cpgcErrorStatus.cpgcErrEccS = (DqMask[2] << 8) | DqMask[2];
                            cpgcErrorStatus.cpgcErrChunk = 0;
                            cpgcErrorStatus.cpgcErrColumn = 0;
                            cpgcErrorStatus.cpgcErrRow = PprAddrListPtr[Index].pprAddr.row;
                            cpgcErrorStatus.cpgcErrBank = PprAddrListPtr[Index].pprAddr.bank;
                            //
                            // convert dimm, rank to cpgc logical rank
                            //
                            cpgcErrorStatus.cpgcErrRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
                            cpgcErrorStatus.cpgcSubrank = LogicalSubRank;

                            //
                            // Log EWL entry to indicate PPR repair successful
                            //
                            LogAdvMemtestError (Host, Socket, Ch, Dimm, Rank, WARN_FPT_PPR_ROW_REPAIR, ADV_MT_TYPE_PPR, cpgcErrorStatus, 1);

                            //
                            // If PPR has been attempted on same address too many times, then flag the BG as out of PPR resources
                            //
                            if (LookupEwlAdvMemtestEntry (Host, Socket, Ch, Dimm, Rank, ADV_MT_TYPE_PPR, cpgcErrorStatus, &warningPtr)) {

                              if (((Setup->mem.pprType == PPR_TYPE_HARD) && (warningPtr->Count >= MAX_HARD_PPR_ROW_REPAIR_ATTEMPT)) ||
                                  ((Setup->mem.pprType == PPR_TYPE_SOFT) && (warningPtr->Count >= MAX_SOFT_PPR_ROW_REPAIR_ATTEMPT))) {

                                SetPprResourceAvailable (Host, Socket, Ch, Dimm, Rank, LogicalSubRank, Nibble, PprBank >> NumBaBits, FALSE);
                              }
                            } else {
                              //
                              // If EWL buffer is full, then limit PPR to 1 per BG
                              //
                              SetPprResourceAvailable (Host, Socket, Ch, Dimm, Rank, LogicalSubRank, Nibble, PprBank >> NumBaBits, FALSE);

                              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
                                "DispositionFailRangesWithPprFlow: PPR entry not found in EWL, so limiting repairs to one per BG!\n");

                            } // LookupEwlAdvMemtestEntry

                          } // if TempNibbleMask
                        } // for Nibble
                      } // if PprStatus[Index]
                    } // if PprAddrList[Index].pprAddrStatus
                  } // for Index
                } // if (Status == SUCCESS)

                if (RepairStatus) {
                  *RepairDone = 1;
                }
                if (Status == SUCCESS) {
                  //
                  // Remove the failing DRAM devices from FailRange if PPR flow returns success
                  //
                  for (Nibble = 0; Nibble < MSVx4; Nibble++) {
                    if (NibbleMask & (1 << Nibble)) {
                      BgDqMask[BankGroup][Nibble / 8] &= ~(0xF << ((Nibble % 8) * 4));
                    }
                  }
                  //
                  // Clear BG mask if all DRAMs have been repaired
                  //
                  if ((BgDqMask[BankGroup][0] | BgDqMask[BankGroup][1] | BgDqMask[BankGroup][2]) == 0) {
                    FailRangePtr->addr.Bits.upperBgMask &= ~(1 << BankGroup);
                  }
                } // if (Status == SUCCESS)
              } // upperBgMask
            } // BankGroup

            //
            // Update common Dq mask
            //
            TempFail = *FailRangePtr;
            ListUpdate = FALSE;
            for (Index = 0; Index < 3; Index++) {
              TempFail.mask[Index] = BgDqMask[0][Index] | BgDqMask[1][Index];
              if (TempFail.mask[Index] != FailRangePtr->mask[Index]) {
                FailRangePtr->mask[Index] = TempFail.mask[Index];
                ListUpdate = TRUE;
              }
            }
            if (ListUpdate) {
              //
              // Remove FailRange entry from FailRangeList
              //
              if ((TempFail.mask[0] | TempFail.mask[1] | TempFail.mask[2]) == 0) {
                RemoveRowFailRange (Host, Socket, Ch, FailIndex);
              } else {
                UpdateRowFailList (Host, Socket, Ch, FALSE, TempFail);
              }
              FailIndex = GetRowFailIndex (Host, Ch) - 1;  // subtract 1 to account for FailIndex++ at end of for loop
            }
          } // CurrentAddrMatch
        } // FailIndex
      } // Bank pair
    } // ChBitmask
  } // Ch loop

  return RetVal;

} // DispositionFailRangesWithPprFlow


/**

  Searches the Row failure list to classify errors as correctable/uncorrectable and
  apply error handling policy like CheckTestResults

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - Socket Id
  @param chBitmask        - List of channels that were tested
  @param chBitmaskOut     - List of channels that were tested and still have rank enabled
  @param dimmRank         - DIMM containing logicalRank
  @param logical2Physical - Geometery for logical rank within channel
  @param logicalRank      - Logical rank
  @param logicalSubRank   - SubRank
  @param baseBits         - Number of bank bits in SW loop

  @retval n/a

**/
VOID
DispositionFailRanges (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitmask,
  UINT32    *chBitmaskOut,
  UINT8     dimmRank[MAX_RANK_CH][MAX_CH],
  UINT8     logical2Physical[MAX_RANK_CH][MAX_CH],
  UINT8     logicalRank,
  UINT8     logicalSubRank,
  UINT8     baseBits
  )
{
  ROW_FAIL_RANGE    *FailRangePtr;
  UINT32              errorStatus0;
  UINT32              errorStatus1;
  UINT32              errorStatus2;
  UINT32              errorBits[3];
  UINT8               *dqBytes;
  MEM_TEST_ERROR_TYPE errorType;
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               bank;
  UINT8               FailIndex;
  ROW_ADDR            CurAddr;
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  *chBitmaskOut = chBitmask;
  //
  // Inspect test results for given logicalRank, logicalSubRank, bank
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    dimm = dimmRank[logicalRank][ch];
    rank = logical2Physical[logicalRank][ch];

    //
    // For each Bank address
    //
    for (bank = 0; bank < (1 << baseBits); bank++) {

      CurAddr.Data = 0;
      CurAddr.Bits.bankPair = bank;
      CurAddr.Bits.logicalSubRank = logicalSubRank;
      CurAddr.Bits.logicalRank = logicalRank;
      CurAddr.Bits.valid = 1;
      //
      // Loop through all row failure ranges
      //
      for (FailIndex = 0; FailIndex < GetRowFailMax (Host, ch); FailIndex++) {

        FailRangePtr = GetRowFailRange (Host, ch, FailIndex);

        //
        // Check for address match, masking row number
        //
        if (CurrentAddrMatch (CurAddr, FailRangePtr->addr)) {

          errorStatus0 = FailRangePtr->mask[0];
          errorStatus1 = FailRangePtr->mask[1];
          errorStatus2 = FailRangePtr->mask[2];

          errorBits[0] = errorStatus0;
          errorBits[1] = errorStatus1;
          errorBits[2] = errorStatus2;
          dqBytes = (UINT8 *)errorBits;

          //
          // Clear error counter
          //
          errorType = ErrorTypeCorrectable;

          //
          // Was this channel part of the test group?
          //
          if (chBitmask & (1 << ch)) {
            //
            // Accumulate error status of current bank pair from fail range list
            //

            // Check for any bit errors on any lane.
            if ((errorStatus0 | errorStatus1 | errorStatus2) != 0) {
              //
              // Now check to see if the errors found are correctable or uncorrectable.
              // For failures only fall within x4 aligned lanes, i.e. failed lanes within a single nibble,
              // e.g. lane 15:12 (regardless of running independent mode or lockstep), you can treat it is correctable error.
              // Otherwise, for failures within x8 device aligned lanes, i.e. fail lanes within a double-nibble
              // (e.g. lane 15:8 is but lane 11:4 is not) and running lock-step mode, you can treat the error as correctable error.
              // Otherwise, remaining failures are UC.
              //
              errorType = ClassifyDqErrorStatus (Host, socket, ch, dimm, rank, errorStatus0, errorStatus1, errorStatus2);

              //
              // If the error is uncorrectable and this was the 2nd pass through memtest, disable the rank.
              //
              if (errorType == ErrorTypeUncorrectable) {
                //
                // Add this error to the warning log for both correctable and uncorrectable errors.
                //
                DisableRank (Host, socket, ch, dimm, rank);
                *chBitmaskOut &= ~(1 << ch);
                EwlOutputType5 (WARN_MEMTEST_DIMM_DISABLE, WARN_FPT_MINOR_MEM_TEST, socket, ch, dimm, rank, EwlSeverityWarning, logicalSubRank, bank, dqBytes);

              }
              else if (errorType == ErrorTypeCorrectable) {

                //
                // Don't log correctable errors. Memtest has already logged the failure
                // Handle rank map-out based on MRC input parameter
                //
                if (!(Setup->mem.optionsExt & ALLOW_CORRECTABLE_MEM_TEST_ERROR)) {
                  //
                  // if this is second pass, remove Rank
                  //
                  DisableRank (Host, socket, ch, dimm, rank);
                  *chBitmaskOut &= ~(1 << ch);
                  EwlOutputType5 (WARN_MEMTEST_DIMM_DISABLE, WARN_FPT_MINOR_MEM_TEST, socket, ch, dimm, rank, EwlSeverityWarning, logicalSubRank, bank, dqBytes);
                }
              }
            } // if error
          } // rankChEnMap
        } // CurAddrMatch
      } // FailIndex
    } // bank loop
  } // ch loop
} // DispositionFailRanges

