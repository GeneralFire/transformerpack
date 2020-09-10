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
#include <Library/BaseMemoryLib.h>
#include <Memory/Ddr5MrRegs.h>
#include "Ddr5Core.h"
#include <Memory/Ddr5Cmds.h>
#include "MemHostChipCommonDdr5.h"
#include <Library/MemMcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/OdtTableLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Memory/JedecDefinitions.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Memory/Ddr5DbRegs.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/SpdDecodeLib.h>

#define  DDR5_WL_CWL_ADJ_MAX 7
#define  DDR5_INTERNAL_FINE_SWEEP_RANGE 129

//
// Local function prototypes
//
STATIC VOID
WriteLevelingEndDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

VOID
ProgramExternalWriteLeveling (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank);

VOID
DisableExternalWriteLeveling (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank);

UINT32
ExternalInternalWriteLevelingDdr5 (
  IN PSYSHOST  Host
  );

UINT32
ExternalInternalWriteLevelingDdr5CoarseFine (
  IN PSYSHOST  Host
  );

UINT32
ExternalWriteLevelingDdr5FineSweep (
  IN PSYSHOST  Host
  );

VOID
NomalizeCwlAdjTxDqsDdr5 (
  IN PSYSHOST                Host,
  IN INT16                   (*CwlAdj) [MAX_CH][SUB_CH][MAX_RANK_CH],
  IN INT16                   (*TxDqsDelayValue) [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8                   Done,
  IN CHIP_WL_CLEANUP_STRUCT  WLChipCleanUpStruct
  );

VOID
DisableDramWlDdr5 (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Dimm,
  IN UINT8 Rank
  );

UINT32
InternalWriteLevelingDdr5 (
  IN PSYSHOST  Host
  );

/**

  Enable DRAM / DB External Write Leveling mode

  @param[in] Host  - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Dimm  - DIMM number
  @param[in] Rank  - Rank number

  @retval N/A

**/
VOID
EnableExternalWriteLevelingMode (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Dimm,
  IN UINT8    Rank
  );

/**

This function print the sample array for Internal Write Leveling Coarse/Fine sweep.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] SubChannel      - SubChannel number
  @param[in] Dimm            - DIMM number
  @param[in] Rank            - Rank number
  @param[in] Length          - Sweep range
  @param[in] TrainRes        - Trained result
  @param[in] CurrentTestType - Current training type

  @retval N/A

**/
VOID
PrintSampleArrayInternalCoarseFine (
  IN PSYSHOST                    Host,
  IN UINT8                       Socket,
  IN UINT8                       Channel,
  IN UINT8                       SubChannel,
  IN UINT8                       Dimm,
  IN UINT8                       Rank,
  IN UINT16                      Length,
  IN struct TrainingResultsDdr5  (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN MRC_TT                      CurrentTestType
  );

/**

This function print the sample array for External Write Leveling Coarse sweep.

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] SubChannel  - SubChannel number
  @param[in] Dimm        - DIMM number
  @param[in] Rank        - DIMM number
  @param[in] Length      - Sweep range
  @param[in] TrainRes    - Trained result

  @retval N/A

**/
VOID
PrintSampleArrayExternalCoarse (
  IN PSYSHOST                    Host,
  IN UINT8                       Socket,
  IN UINT8                       Channel,
  IN UINT8                       SubChannel,
  IN UINT8                       Dimm,
  IN UINT8                       Rank,
  IN UINT16                      Length,
  IN struct TrainingResultsDdr5  (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

This function print the sample array for External Write Leveling Fine sweep.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] SubChannel      - SubChannel number
  @param[in] Dimm            - DIMM number
  @param[in] Rank            - Rank number
  @param[in] Length          - Sweep range
  @param[in] TrainRes        - Trained result

  @retval N/A

**/
VOID
PrintSampleArrayExternalFine (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    SubChannel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT16   Length,
  IN struct TrainingResultsDdr5 (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

Perform Write Leveling training for DDR5

@param Host  - Pointer to sysHost

@retval SUCCESS

**/
UINT32
WriteLevelingDdr5 (
  IN PSYSHOST Host
  )
{
  UINT8               Socket;

  if (!IsMemFlowEnabled (WriteLevelingTraining)) {
    return SUCCESS;
  }

  Socket = Host->var.mem.currentSocket;

  //
  // Return if this Socket is disabled
  //

  if (IsSocketDimmPopulated (Host, Socket) != TRUE) {
    return SUCCESS;
  }

  //
  // Start External Write Leveling Training.
  //
  SetCurrentTestType (Socket, WLDdr5);

  DoCompDdrioPerSetupOption (Socket, TRAINING_COMP_EVERY_TRAINING);

  ExternalInternalWriteLevelingDdr5 (Host);

  WriteLevelingEndDdr5 (Host, Socket);
  //
  // Reset the IO
  //
  IO_Reset (Host, Socket);
  FifoTrainReset (Host, Socket);
  SetCurrentTestType (Socket, 0);
  return SUCCESS;
} // WriteLevelingDdr5

/*++
DDR5 external write leveling training.

@param Host    - Pointer to sysHost

@retval SUCCESS

--*/
UINT32
ExternalInternalWriteLevelingDdr5 (
  IN PSYSHOST  Host
  )
{
  UINT8                   Socket;

  Socket = Host->var.mem.currentSocket;
  //
  // External write leveling coarse and fine sweep training.
  //
  ExternalInternalWriteLevelingDdr5CoarseFine (Host);
#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    DisplayResultsDdr5 (Host, Socket, TxDqsDelay);
  }
#endif //DEBUG_CODE_BLOCK

  DoFinalChipSpecificWLCleanUp (Host, Socket);
  Host->nvram.mem.socket[Socket].TrainingStepDone.HostFlyBy = 1;

  return SUCCESS;
} // ExternalInternalWriteLevelingDdr5

/**

For each data module, a 128 bit vector of zeros and ones holding the tests results is
evaluated which is a concatenation of the two reads from the feedback CRs.
BIOS locates the middle of the longest consecutive stream of ones (when the vector is evaluated as cyclic,
this location in the vector indicates the PI setting and logic Delay for that data module).

@param Host              - Pointer to sysHost
@param Socket            - Current Socket
@param Ch                - Channel number
@param Dimm              - Dimm number on Ch
@param Rank              - Rank on Dimm
@param Strobe            - Strobe number
@param RisingEdge        - Pointer to save the rising edge
@param centerPoint       - Pointer to save the center point
@param FallingEdge       - Pointer to save the falling edge
@param PulseWidth        - Pointer to save the pulse width
@param Step              - The PI Step to work on:
PI_CS_CLK_TYPE
@param SubChannel        - sub channel number
@param TrainResinput     - Pointer to ClkTrainingResults


@retval N/A

**/
VOID
EvaluatePiSettingResultsWlFineSweep (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN UINT8      Rank,
  IN UINT8      Strobe,
  IN OUT INT16  RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN OUT INT16  FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8      Step,
  IN UINT8      SubChannel,
  IN MRC_TT     CurrentTestType,
  IN struct TrainingResultsDdr5 (*TrainResinput)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT16                     PiSetting;
  UINT16                     PiStart;
  UINT32                     Register;
  UINT32                     Bit;
  UINT8                      CyclicCase;
  UINT16                     OneCount;
  UINT16                     OneStart;
  UINT16                     NumberOfCycle;
  UINT16                     MaxOneCount;
  UINT8                      DWord;
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  struct TrainingResultsDdr5 (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];

  Register = 0;
  OneStart = 0;

  Bit = BIT0;
  OneCount = 0;
  MaxOneCount = 0;
  CyclicCase = 0;

  TrainRes = TrainResinput;
  RisingEdge[Ch][SubChannel][Strobe] = 0;
  FallingEdge[Ch][SubChannel][Strobe] = 0;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);

  //
  // Start at bit 0
  //
  PiSetting = 0;
  PiStart = 0;
  NumberOfCycle = TX_DQS_DELAY_WL_FINE_SWEEP_RANGE + WR_LVL_FINE_STEP_SIZE_DDR5;

  Host->var.mem.piSettingStopFlag[Ch] = GetSubChStrobeMaskDdr5(Host); //to make sure all the Strobe will be handled.
  if (!(*DimmNvList) [Dimm].x4Present) {
    Host->var.mem.piSettingStopFlag[Ch] &= 0x1F;
  }
  while (Host->var.mem.piSettingStopFlag[Ch] & (1 << Strobe)) {

    DWord = GetTrainingResultIndexDDR5 (Step, PiSetting, PiStart);

    //
    // Get the proper training results
    //
    if ((PiSetting % 32) == 0) {
      Register = (*TrainRes) [Ch][SubChannel][Strobe].results[DWord];
      Bit = BIT0;
    }
    //
    // algo start
    //
    if (Register & Bit) {
      //
      // this index is 1
      //
      if (OneCount == 0) {
        //
        // This first one so save the starting point
        //
        OneStart = PiSetting;
      }

      OneCount++;
    } else {
      //
      // this index is 0
      //
      if (OneCount > 0) {
        //
        // We just moved from a one to a zero so check if that was the largest stream of one's
        //
        if (MaxOneCount < OneCount) {
          //
          // TBD  if i have two max value equals
          //
          MaxOneCount = OneCount;
          RisingEdge[Ch][SubChannel][Strobe] = OneStart;
          FallingEdge[Ch][SubChannel][Strobe] = PiSetting;
        }
        //
        // Start counting again at 0
        //
        OneCount = 0;
      }
    }

    //
    // Go to the next bit
    //
    PiSetting++;
    Bit = Bit << 1;

    //
    // Check if we're at the end of the cycle
    //
    if (PiSetting == NumberOfCycle) {
      if (OneCount > 0) {
        //
        // Cyclic case continue to scan
        // Start back over with bit 0 if the last bit is a 1
        //
        PiSetting = StartBackOverDdr5 (Step, PiStart);
        CyclicCase = 1;
      } else {
        //
        // stop the scan
        // We've checked all bits so save the results
        //
        Host->var.mem.piSettingStopFlag[Ch] &= ~(1 << Strobe);
      }
    }
    //
    // Check if this is a cyclic case and if we are at the end of the one's
    //
    if ((CyclicCase == 1) && (OneCount == 0)) {
      Host->var.mem.piSettingStopFlag[Ch] &= ~(1 << Strobe);
    }
  } // while loop
}

/**

For each data module, a 128 bit vector of zeros and ones holding the tests results is
evaluated which is a concatenation of the two reads from the feedback CRs.
BIOS locates the middle of the longest consecutive stream of ones (when the vector is evaluated as cyclic,
this location in the vector indicates the PI setting and logic Delay for that data module).

@param Host              - Pointer to sysHost
@param Socket            - Current Socket
@param Ch                - Channel number
@param Dimm              - Dimm number on Ch
@param Rank              - Rank on Dimm
@param Strobe            - Strobe number
@param RisingEdge        - Pointer to save the rising edge
@param FallingEdge       - Pointer to save the falling Edge
@param FallingEdge       - Pointer to save the falling edge
@param Step              - The PI Step to work on:
@param SubChannel        - sub channel number
@param CurrentTestType   - Test type.
@param TrainResinput     - Pointer to ClkTrainingResults


@retval N/A

**/
VOID
EvaluatePiSettingResultsInternalWlFineSweep (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN UINT8      Rank,
  IN UINT8      Strobe,
  IN OUT UINT16 RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN OUT UINT16 FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8      Step,
  IN UINT8      SubChannel,
  IN MRC_TT     CurrentTestType,
  IN struct     TrainingResultsDdr5 (*TrainResinput) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT16                     PiSetting;
  UINT16                     PiStart;
  UINT32                     Register;
  UINT32                     Bit;
  UINT8                      CyclicCase;
  UINT16                     OneCount;
  UINT16                     OneStart;
  UINT16                     NumberOfCycle;
  UINT16                     MaxOneCount;
  UINT8                      DWord;
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  struct TrainingResultsDdr5 (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];

  Register = 0;
  OneStart = 0;

  Bit = BIT0;
  OneCount = 0;
  MaxOneCount = 0;
  CyclicCase = 0;

  TrainRes = TrainResinput;
  RisingEdge[Ch][SubChannel][Strobe] = 0;
  FallingEdge[Ch][SubChannel][Strobe] = 0;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);

  //
  // Start at bit 0
  //
  PiSetting = 0;
  PiStart = 0;
  NumberOfCycle = WR_LVL_INTERNAL_FINE_PI_END_DDR5 + WR_LVL_INTERNAL_FINE_PI_EXT_RANGE + WR_LVL_INTERNAL_FINE_PI_ADDITIONAL + 1;

  Host->var.mem.piSettingStopFlag[Ch] = GetSubChStrobeMaskDdr5(Host); //to make sure all the Strobe will be handled.
  if (!(*DimmNvList) [Dimm].x4Present) {
    Host->var.mem.piSettingStopFlag[Ch] &= 0x1F;
  }
  while (Host->var.mem.piSettingStopFlag[Ch] & (1 << Strobe)) {

    DWord = GetTrainingResultIndexDDR5 (CurrentTestType, PiSetting, PiStart);

    //
    // Get the proper training results
    //
    if ((PiSetting % 32) == 0) {
      Register = (*TrainRes) [Ch][SubChannel][Strobe].results[DWord];
      Bit = BIT0;
    }
    //
    // algo start
    //
    if (Register & Bit) {
      //
      // this index is 1
      //
      if (OneCount == 0) {
        //
        // This first one so save the starting point
        //
        OneStart = PiSetting;
      }

      OneCount++;
    } else {
      //
      // this index is 0
      //
      if (OneCount > 0) {
        //
        // We just moved from a one to a zero so check if that was the largest stream of one's
        //
        if (MaxOneCount < OneCount) {
          //
          // TBD  if i have two max value equals
          //
          MaxOneCount = OneCount;
          RisingEdge[Ch][SubChannel][Strobe] = OneStart;
          FallingEdge[Ch][SubChannel][Strobe] = PiSetting;
        }
        //
        // Start counting again at 0
        //
        OneCount = 0;
      }
    }

    //
    // Go to the next bit
    //
    PiSetting++;
    Bit = Bit << 1;

    //
    // Check if we're at the end of the cycle
    //
    if (PiSetting == NumberOfCycle) {
      if (OneCount > 0) {
        //
        // Cyclic case continue to scan
        // Start back over with bit 0 if the last bit is a 1
        //
        PiSetting = StartBackOverDdr5 (Step, PiStart);
        CyclicCase = 1;
      } else {
        //
        // stop the scan
        // We've checked all bits so save the results
        //
        Host->var.mem.piSettingStopFlag[Ch] &= ~(1 << Strobe);
      }
    }
    //
    // Check if this is a cyclic case and if we are at the end of the one's
    //
    if ((CyclicCase == 1) && (OneCount == 0)) {
      Host->var.mem.piSettingStopFlag[Ch] &= ~(1 << Strobe);
    }
  } // while loop
}

#ifdef DEBUG_CODE_BLOCK

/**

Prints the edges found for DDR5 WL fine sweep.

@param Host            - Pointer to sysHost
@param Socket           - Current Socket
@param Ch               - Channel number
@param PulseWidth       - Pointer to save the pulse width
@param FallingEdge      - Pointer to save the falling edge
@param RisingEdge       - Pointer to save the rising edge
@param centerPoint      - Pointer to save the center point
@param MaxStrobe        - Maximum number of strobes
@param CurrentTestType  - Test type.

@retval N/A

**/
VOID
DisplayEdgesWlFineSweep (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     SubChannel,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT16    PulseWidth[MAX_STROBE],
  IN UINT16    FallingEdge[MAX_STROBE],
  IN UINT16    RisingEdge[MAX_STROBE],
  IN UINT8     MaxStrobe,
  IN MRC_TT    CurrentTestType
  )
{
  UINT8 Strobe;
  UINT16  Delay;
  INT16     CwlAdj = 0;
  UINT8   ChannelLocal = 0;

  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }
  ChannelLocal = Ch;
  GetChannelMapping (Ch, SubChannel, &ChannelLocal);
  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_MAX, (MaxStrobe * 4) + 4, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

  // Print rising edges
  if (CurrentTestType == WLDdr5) {
    RcDebugPrint (SDBG_DEFAULT, "RE: ");
    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }
      RcDebugPrint (SDBG_DEFAULT, " %3d", RisingEdge[Strobe]);
    } // Strobe loop
  } else if (CurrentTestType == InternalWlFineSweepTest) {
    // Print falling edges

    RcDebugPrint (SDBG_DEFAULT, "RE:    ");
    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }
      RcDebugPrint (SDBG_DEFAULT, " %3d", RisingEdge[Strobe]);
    } // Strobe loop
    RcDebugPrint (SDBG_DEFAULT, "\n");
    //
    // Print TxDQS Delay Value
    //
    RcDebugPrint (SDBG_DEFAULT, "TxDQS: ");
    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsStrobeNotValidDdr5 (Host, Strobe)) {
        continue;
      }
      GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&Delay);
      RcDebugPrint (SDBG_DEFAULT, " %3d", Delay);
    } // Strobe loop
    RcDebugPrint (SDBG_DEFAULT, "\n");
    //
    // Print CWL_ADJ Value
    //
    GetCwlAdjDdr5 (Host, Socket, Ch, SubChannel, Dimm, &CwlAdj);
    RcDebugPrint (SDBG_DEFAULT, "CWL_ADJ:");
    RcDebugPrint (SDBG_DEFAULT, "%2d", CwlAdj);
  }
  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
} // DisplayEdgesWlFineSweep

/**

This function print the sample array for External Write Leveling Coarse sweep.

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Channel     - Channel number
  @param[in] SubChannel  - SubChannel number
  @param[in] Dimm        - DIMM number
  @param[in] Rank        - DIMM number
  @param[in] Length      - Sweep range
  @param[in] TrainRes    - Trained result

  @retval N/A

**/
VOID
PrintSampleArrayExternalCoarse (
  IN PSYSHOST                    Host,
  IN UINT8                       Socket,
  IN UINT8                       Channel,
  IN UINT8                       SubChannel,
  IN UINT8                       Dimm,
  IN UINT8                       Rank,
  IN UINT16                      Length,
  IN struct TrainingResultsDdr5  (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8              Strobe;
  UINT8              MaxStrobe;
  UINT32             PiSetting;
  UINT32             StrobePass = 0;
  UINT32             StrobeMask = 0;
  UINT32             PiPrint = 0;
  struct dimmNvram   (*DimmNvList) [MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  //
  // Set Strobe bit masks
  //
  StrobeMask = GetSubChStrobeMaskDdr5 (Host);
  if (!(*DimmNvList) [Dimm].x4Present) {
    StrobeMask &= 0x1F;
  }
  if (!(*DimmNvList) [Dimm].x4Present) {
    RcDebugPrintLine (SDBG_DEFAULT, 23, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    if (Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_DEFAULT, "         0   1   2   3   4\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "         0   1   2   3\n");
    }
    MaxStrobe = MAX_STROBE_DDR5 / 4;
  } else {
    RcDebugPrintLine (SDBG_DEFAULT, 48, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    if (Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_DEFAULT, "         0   1   2   3   4   5   6   7   8   9\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "         0   1   2   3   4   5   6   7\n");
    }
    MaxStrobe = MAX_STROBE_DDR5 / 2;
  }

  for (PiSetting = 0; PiSetting < Length; PiSetting++) {
    PiPrint = WR_LVL_PI_START_DDR5 + PiSetting * WR_LVL_COARSE_STEP_SIZE_DDR5;

    if (StrobePass == StrobeMask) {
      break;
    }

    RcDebugPrint (SDBG_DEFAULT, "%d ", PiPrint);
    if (PiPrint < 10) {
      RcDebugPrint (SDBG_DEFAULT, "     ");
    }

    if ((PiPrint < 100) && (PiPrint > 9)) {
      RcDebugPrint (SDBG_DEFAULT, "   ");
    }

    if (PiPrint > 100) {
      RcDebugPrint (SDBG_DEFAULT, "  ");
    }

    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
        continue;
      }

      if (StrobePass & (1 << Strobe)) {
        RcDebugPrint (SDBG_DEFAULT, "   *");
        continue;
      }
      if (((1 << PiSetting) & (*TrainRes) [Channel][SubChannel][Strobe].results[PiSetting / 32]) != 0) {
        RcDebugPrint (SDBG_DEFAULT, "   1");
        StrobePass = StrobePass | (1 << Strobe);
      } else {
        RcDebugPrint (SDBG_DEFAULT, "   0");
      }
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // PiSetting loop

  //
  // Report error when got training failure
  //
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }

    if ((StrobePass & (1 << Strobe)) == 0) {
      RcDebugPrint (SDBG_MINMAX, "Write Leveling - External Coarse training failure!!!\n");
      DisableChannelSw (Host, Socket, Channel);
      EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_WL_EXTERNAL_COARSE, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, WrLvlDelay, DdrLevel, 0xFF);
    }
  }
} // PrintSampleArrayExternalCoarse


/**

This function print the sample array for Internal Write Leveling Coarse/Fine sweep.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] SubChannel      - SubChannel number
  @param[in] Dimm            - DIMM number
  @param[in] Rank            - Rank number
  @param[in] Length          - Sweep range
  @param[in] TrainRes        - Trained result
  @param[in] CurrentTestType - Current training type

  @retval N/A

**/
VOID
PrintSampleArrayInternalCoarseFine (
  IN PSYSHOST                    Host,
  IN UINT8                       Socket,
  IN UINT8                       Channel,
  IN UINT8                       SubChannel,
  IN UINT8                       Dimm,
  IN UINT8                       Rank,
  IN UINT16                      Length,
  IN struct TrainingResultsDdr5  (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2],
  IN MRC_TT                      CurrentTestType
  )
{
  UINT8   Strobe;
  UINT8   MaxStrobe;
  UINT32  PiSetting;
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  UINT32  StrobePass = 0;
  UINT32  StrobePassLog = 0;
  UINT32  StrobeMask = 0;
  UINT32   PiPrint = 0;

  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  //
  // Set Strobe bit masks
  //
  StrobeMask = GetSubChStrobeMaskDdr5 (Host);
  if (!(*DimmNvList) [Dimm].x4Present) {
    StrobeMask &= 0x1F;
  }
  if (!(*DimmNvList) [Dimm].x4Present) {
    RcDebugPrintLine (SDBG_DEFAULT, 23, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    if (Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_DEFAULT, "         0   1   2   3   4\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "         0   1   2   3\n");
    }
    MaxStrobe = MAX_STROBE_DDR5 / 4;
  } else {
    RcDebugPrintLine (SDBG_DEFAULT, 48, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    if (Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_DEFAULT, "         0   1   2   3   4   5   6   7   8   9\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "         0   1   2   3   4   5   6   7\n");
    }
    MaxStrobe = MAX_STROBE_DDR5 / 2;
  }

  for (PiSetting = 0; PiSetting < Length; PiSetting++) {
    PiPrint = PiSetting;
    if (StrobePass == StrobeMask) {
      break;
    }

    RcDebugPrint (SDBG_DEFAULT, "%d ", PiPrint);
    if (PiPrint < 10) {
      RcDebugPrint (SDBG_DEFAULT, "    ");
    }

    if ((PiPrint < 100) && (PiPrint > 9)) {
      RcDebugPrint (SDBG_DEFAULT, "   ");
    }

    if (PiPrint >= 100) {
      RcDebugPrint (SDBG_DEFAULT, "  ");
    }

    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
        continue;
      }

      if (StrobePass & (1 << Strobe)) {
        RcDebugPrint (SDBG_DEFAULT, "   *");
        continue;
      }

      if (((1 << PiSetting) & (*TrainRes) [Channel][SubChannel][Strobe].results[PiSetting / 32]) != 0) {
        RcDebugPrint (SDBG_DEFAULT, "   1");
        if (CurrentTestType != InternalWlFineSweepTest) {
          StrobePass = StrobePass | (1 << Strobe);
        }
        StrobePassLog = StrobePassLog | (1 << Strobe);
      } else {
        RcDebugPrint (SDBG_DEFAULT, "   0");
      }
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // PiSetting loop

  //
  // Report error when got training failure
  //
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }

    if ((StrobePassLog & (1 << Strobe)) == 0) {
      if (CurrentTestType == InternalWlCoarseSweepTest) {
        RcDebugPrint (SDBG_MINMAX, "Write Leveling - Internal Coarse training failure!!!\n");
        DisableChannelSw (Host, Socket, Channel);
        EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_WL_INTERNAL_COARSE, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, WrLvlDelay, DdrLevel, 0xFF);
      } else {
        RcDebugPrint (SDBG_MINMAX, "Write Leveling - Internal Fine training failure!!!\n");
        DisableChannelSw (Host, Socket, Channel);
        EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_WL_INTERNAL_FINE, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, WrLvlDelay, DdrLevel, 0xFF);
      }
    }
  }
} // PrintSampleArrayInternalCoarseFine

/**

This function print the sample array for External Write Leveling Fine sweep.

  @param[in] Host            - Pointer to sysHost
  @param[in] Socket          - Socket number
  @param[in] Channel         - Channel number
  @param[in] SubChannel      - SubChannel number
  @param[in] Dimm            - DIMM number
  @param[in] Rank            - Rank number
  @param[in] Length          - Sweep range
  @param[in] TrainRes        - Trained result

  @retval N/A

**/
VOID
PrintSampleArrayExternalFine (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    SubChannel,
  IN UINT8    Dimm,
  IN UINT8    Rank,
  IN UINT16   Length,
  IN struct TrainingResultsDdr5 (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8             Strobe;
  UINT8             MaxStrobe;
  UINT32            PiSetting;
  UINT32            StrobePass = 0;
  struct dimmNvram  (*DimmNvList) [MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  if (!(*DimmNvList) [Dimm].x4Present) {
    RcDebugPrintLine (SDBG_DEFAULT, 23, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    if (Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3\n");
    }
    MaxStrobe = MAX_STROBE_DDR5 / 4;
  } else {
    RcDebugPrintLine (SDBG_DEFAULT, 48, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    if (Host->nvram.mem.eccEn) {
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7   8   9\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7\n");
    }
    MaxStrobe = MAX_STROBE_DDR5 / 2;
  }

  for (PiSetting = 0; PiSetting < Length; PiSetting++) {
    RcDebugPrint (SDBG_DEFAULT, "%d ", PiSetting);
    if (PiSetting < 10) {
      RcDebugPrint (SDBG_DEFAULT, "  ");
    }

    if ((PiSetting < 100) && (PiSetting > 9)) {
      RcDebugPrint (SDBG_DEFAULT, " ");
    }

    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      if (IsDimmStrobeNotValidDdr5 (&(*DimmNvList) [Dimm], Strobe)) {
        continue;
      }
      if (((1 << PiSetting) & (*TrainRes) [Channel][SubChannel][Strobe].results[PiSetting / 32]) != 0) {
        StrobePass = StrobePass | (1 << Strobe);
        RcDebugPrint (SDBG_DEFAULT, "   1");
      } else {
        RcDebugPrint (SDBG_DEFAULT, "   0");
      }
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // PiSetting loop

  //
  // Report error when got training failure
  //
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }

    if ((StrobePass & (1 << Strobe)) == 0) {
      RcDebugPrint (SDBG_MINMAX, "Write Leveling - External Fine training failure StrobePass = 0x%x!!!\n", StrobePass);
      DisableChannelSw (Host, Socket, Channel);
      EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_WL_EXTERNAL_FINE, Socket, Channel, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, WrLvlDelay, DdrLevel, 0xFF);
    }
  }
} // PrintSampleArrayExternalFine

  /**

  Prints the DDR5 WL Coarse Results

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Current Socket
  @param[in] Ch            - Channel number
  @param[in] Dimm          - Dimm number
  @param[in] Rank          - Rank number
  @param[in] SubChannel    - subchannel number
  @param[in] MaxStrobe     - Maximum number of strobes

  @retval N/A

  **/
VOID
EFIAPI
DisplayWlCoarseResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     SubChannel,
  IN UINT8     MaxStrobe
  )
{
  UINT8   Strobe;
  UINT16  Delay;
  UINT8   ChannelLocal;

  ChannelLocal = Ch;
  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }
  GetChannelMapping (Ch, SubChannel, &ChannelLocal);
  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_MAX, (MaxStrobe * 4) + 8, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  //
  // Print TxDQS Delay Value
  //
  RcDebugPrint (SDBG_DEFAULT, "TxDQS:");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    GetSetDataGroup (Host, Socket, ChannelLocal,  SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&Delay);
    RcDebugPrint (SDBG_DEFAULT, " %3d", Delay);
  } // Strobe loop

  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
} // DisplayWlCoarseResultsDdr5


/**

Prints the DDR5 Internal WL Coarse Results

@param Host          - Pointer to sysHost
@param Socket        - Current Socket
@param Ch            - Channel number
@param Dimm          - Dimm number
@param Rank          - Rank number
@param SubChannel    - subchannel number
@param MaxStrobe     - Maximum number of strobes

@retval N/A

**/
VOID
DisplayInternalWlCoarseResultsDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     SubChannel,
  IN UINT8     MaxStrobe,
  IN INT16     InternalCycleAlignment [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2]
  )
{
  UINT8   Strobe;
  UINT16  Delay;
  INT16   CwlAdj = 0;
  UINT8   ChannelLocal;
  struct ddrRank (*RankList) [MAX_RANK_DIMM];

  ChannelLocal = Ch;
  RankList = GetRankNvList (Host, Socket, Ch, Dimm);
  if (!(GetDebugLevel () & SDBG_MAX)) {
    return;
  }

  GetChannelMapping (Ch, SubChannel, &ChannelLocal);
  AcquirePrintControl ();

  RcDebugPrintLine (SDBG_MAX, (MaxStrobe * 4) + 8, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
  //
  // Print WL internal cycle alingment Value
  //
  RcDebugPrint (SDBG_DEFAULT, "ICyAlm:");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    if (Strobe == 0) {
      RcDebugPrint (SDBG_DEFAULT, "%3d", InternalCycleAlignment [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe]);
    } else {
      RcDebugPrint (SDBG_DEFAULT, " %3d", InternalCycleAlignment [Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe]);
    }
  } // Strobe loop

  RcDebugPrint (SDBG_DEFAULT, "\n");
  //
  // Print TxDQS Delay Value
  //
  RcDebugPrint (SDBG_DEFAULT, "TxDQS:");
  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    GetSetDataGroup (Host, Socket, ChannelLocal,  SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&Delay);
    RcDebugPrint (SDBG_DEFAULT, " %3d", Delay);
  } // Strobe loop

  RcDebugPrint (SDBG_DEFAULT, "\n");
  //
  // Print CWL_ADJ Value
  //
  GetCwlAdjDdr5 (Host, Socket, Ch, SubChannel, Dimm, &CwlAdj);
  RcDebugPrint (SDBG_DEFAULT, "CWL_ADJ:");
  RcDebugPrint (SDBG_DEFAULT, "%2d", CwlAdj);

  RcDebugPrint (SDBG_DEFAULT, "\n\n");

  ReleasePrintControl ();
} // DisplayWlCoarseResultsDdr5

#endif //DEBUG_CODE_BLOCK

/**

  Get Minmal TxDqs Pi

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Ch          - Ch number
  @param[in] SubChannel  - Sub Channel number
  @param[in] Dimm        - Dimm number
  @param[in] Rank        - Rank number

  @retval INT16 Minimal TxDqs Delay

**/
INT16
GetMinTxDqsPi (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubChannel,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  UINT8  Strobe;

  struct dimmNvram        (*DimmNvList) [MAX_DIMM];
  INT16                 PiDelayLocalInt = 0;
  UINT8  ChannelLocal = 0;
  INT16  MinPi = 1024;
  UINT8  MaxStrobe = 0;
  UINT8                 SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                 SubChMSVx8 = SubChMSVx4 / 2;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  if (IsX4Dimm (Socket, Ch, Dimm)) {
    MaxStrobe = SubChMSVx4;
  } else {
    MaxStrobe = SubChMSVx8;
  }

  GetChannelMapping (Ch, SubChannel, &ChannelLocal);

  for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
    //
    // Skip if this is an ECC Strobe when ECC is disabled
    //
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&PiDelayLocalInt);
    if (MinPi > PiDelayLocalInt) {
      MinPi = PiDelayLocalInt;
    }
    if (!(*DimmNvList) [Dimm].x4Present) {
      GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe + SubChMSVx8, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&PiDelayLocalInt);
      if (MinPi > PiDelayLocalInt) {
        MinPi = PiDelayLocalInt;
      }
    }
  }//strobe loop
  return MinPi;
} // GetMinTxDqsPi

/**

  Clean up after Write Leveling training

  @param Host  - Pointer to sysHost
  @param Socket  - Socket number

  @retval N/A

**/
STATIC
VOID
WriteLevelingEndDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8               Dimm;
  UINT8               Rank;

  if (!IsBrsPresent (Host, Socket)) {
    return;
  }
  //
  // Loop for each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Loop for each Rank
    //
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      //
      // Change the Write level mode to 0 in the end of the algorithm
      //
      SetTrainingMode (Host, Socket, Dimm, Rank, WR_LEVELING_CLEAR_MODE);
    } // Rank loop
  } // Dimm loop
} // WriteLevelingEndDdr5

/**

  Enable DRAM / DB External Write Leveling mode

  @param[in] Host  - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Dimm  - DIMM number
  @param[in] Rank  - Rank number

  @retval N/A

**/
VOID
EnableExternalWriteLevelingMode (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  UINT8 Ch;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  struct rankDevice (*RankStruct) [MAX_RANK_DIMM];
  UINT8                         MaxChDdr;
#ifdef LRDIMM_SUPPORT
  DDR5_DATA_BUFFER_RW83_STRUCT  DataBufferRw83;
#endif

  MaxChDdr = GetMaxChDdr ();

  //
  // Enter WL mode and override Rtt_nom
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
    //
    // Skip if no Rank
    //
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

#ifdef LRDIMM_SUPPORT
    if (IsLrdimmPresent (Socket, Ch, Dimm)) {
      DataBufferRw83.Data = 0;
      DataBufferRw83.Bits.training_mode = HOST_WRITE_LEVELING;
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, DataBufferRw83.Data);
    } else
#endif //LRDIMM_SUPPORT
    {
      ProgramExternalWriteLeveling (Host, Socket, Ch, Dimm, Rank);
      //
      // Put a Delay for tWLPEN here. By spec, it should be 15ns. Delay 1us here.
      //
      FixedDelayMicroSecond (1);
    }
  } // Ch loop

  // Wait for tWLDQSEN to be met
  FixedDelayMicroSecond (1);

} // EnableExternalWriteLevelingMode

/**

  This routine programs the TxDqs Delay for all strobes not in StrobeSkipMask

  @param[in]  Host            - Pointer to sysHost
  @param[in]  Socket          - Processor to initialize
  @param[in]  Ch              - Channel number within the socket
  @param[in]  Dimm            - Dimm number within the channel
  @param[in]  SubCh           - Subchannel number
  @param[in]  Rank            - Rank within the DIMM
  @param[in]  StrobeSkipMask  - Mask of strobes to skip
  @param[in]  Mode            - Bit-field of different register access modes
  @param[in]  PiDelayInput    - PiDelay value to write

  @retval N/A

**/
VOID
ProgramTxDqsDelay (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     SubCh,
  IN  UINT8     Rank,
  IN  UINT32    StrobeSkipMask,
  IN  UINT8     Mode,
  IN  INT16     PiDelayInput
  )
{
  UINT8               Strobe;
  INT16               PiDelay;
  UINT8               SubChMSVx4;
  UINT8               ChannelLocal;

  ChannelLocal = Ch;
  GetChannelMapping (Ch, SubCh, &ChannelLocal);

  SubChMSVx4 = GetSubChMaxStrobeValid (Host);

  for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    if (StrobeSkipMask & (1 << Strobe)) {
      continue;
    }
    //
    // Program the delay.
    //
    PiDelay = PiDelayInput;
    GetSetDataGroup (Host, Socket, ChannelLocal, SubCh, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, Mode, &PiDelay);
  } // Strobe loop
}

/**

  This function executes the external DDR5 write leveling coarse and fine sweep training.
  Center TxDQS-CLK timing

  @param Host    - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
ExternalInternalWriteLevelingDdr5CoarseFine (
  IN PSYSHOST Host
  )
{
  UINT8                             Socket;
  UINT8                             Ch;
  UINT8                             Dimm;
  UINT8                             Rank;
  UINT8                             LogRank;
  UINT8                             RankPresent;
  UINT8                             Strobe;
  UINT32                            ChBitmask;
  INT16                             ByteSum[2][MAX_CH]; // Sum of passing offsets for a Ch (DDR4/DDRT)
  INT8                              ByteCount[2][MAX_CH]; // Sum of passing offsets for a Ch (DDR4/DDRT)
  UINT32                            StrobeMask[MAX_CH];
  UINT32                            StrobeFail[MAX_CH]; // Bit mask indicating which Ch/byte fails to pass
  UINT8                             Done = 0;
  UINT8                             IncludeDeselects;
  INT16                             PiDelay;
  INT16                             PiDelayLocal[MAX_CH][SUB_CH];
  UINT16                            PiDelayEnd;
  struct ddrRank                    (*RankList) [MAX_RANK_DIMM];
  struct dimmNvram                  (*DimmNvList) [MAX_DIMM];
  CHIP_WL_CLEANUP_STRUCT            WLChipCleanUpStruct;
  UINT8                             SubChannel = 0;
  UINT8                             LockCwlAdj[MAX_CH][SUB_CH];
  INT16                             CwlAdj[MAX_CH][SUB_CH][MAX_RANK_CH];
  UINT32                            StrobePass[MAX_CH][SUB_CH];
  INT16                             CwlAdjRecord[2][MAX_CH][SUB_CH];
  INT16                             RisingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                             FallingEdge[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                             PulseWidth[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  INT16                             TxDqsDelayValue[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2];
  UINT8                             DoneSum = 0;
  UINT8                             DWord = 0;
  UINT8                             Fail = 0;
  UINT8                             FailChSubCh[MAX_CH][SUB_CH];
  UINT8                             MaxStrobe = 0;
  UINT8                             SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  INT16                             TempCwlAdj = 0;
  INT16                             Offsets = 0;
  INT8                              RankCount[2][MAX_CH][SUB_CH];
  UINT8                             SkipThis[MAX_CH][SUB_CH];
  INT16                             Cwl_Adj_Average[MAX_CH][SUB_CH];
  UINT8                             MaxStrb;
  UINT8                             ChannelLocal = 0;
  INT16                             CwlAdjLocal = 0;
  UINT8                             WritePreamble = 0;
  INT16                             WlAdjStart = 0;
  INT16                             WlAdjEnd = 0;
  INT16                             PiLocal = 0;
  INT8                              CwlAdjTemp = 0;
  UINT8                             WlInternalCycleAlignment = 0;
  UINT8                             MaxStrobeDimm = 0;
  INT16                             PiDelayLocalInt = 0;
  UINT8                             LoopCount = 0;
  UINT32                            NumCl = 0;
  INT16                             InternalCycleAlignment[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2];
  UINT8                             InternalCycleAlignmentChSubCh[MAX_CH][SUB_CH][MAX_RANK_CH];
  UINT8                             SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;
  MRC_RT                            MrcRankList;
  MRC_MST                           MemSsType;
  UINT8                             MaxChDdr;
  INT16                             MinPi = 1024;
  DDR5_MODE_REGISTER_0_STRUCT       Mr0;
  DDR5_MODE_REGISTER_2_STRUCT       Mr2;
#ifdef USE_LATEST_DRAM_SPEC
  DDR5_MODE_REGISTER_3_STRUCT       Mr3;
#endif // #ifdef USE_LATEST_DRAM_SPEC

  MaxChDdr = GetMaxChDdr ();
  struct TrainingResultsDdr5 (*TrainRes) [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2];
  TrainRes = RcAllocatePool (sizeof (*TrainRes));

  WLChipCleanUpStruct.ddrtDimm0BasicTiming.Data = 0;
  WLChipCleanUpStruct.tCOTHP.Data = 0;
  WLChipCleanUpStruct.tCOTHP2.Data = 0;

  ZeroMem ((UINT8 *) TrainRes,                      sizeof (*TrainRes));
  ZeroMem ((UINT8 *) StrobeFail,                    sizeof (StrobeFail));
  ZeroMem ((UINT8 *) CwlAdj,                        sizeof (CwlAdj));
  ZeroMem ((UINT8 *) CwlAdjRecord,                  sizeof (CwlAdjRecord));
  ZeroMem ((UINT8 *) LockCwlAdj,                    sizeof (LockCwlAdj));
  ZeroMem ((UINT8 *) StrobePass,                    sizeof (StrobePass));
  ZeroMem ((UINT8 *) RisingEdge,                    sizeof (RisingEdge));
  ZeroMem ((UINT8 *) FallingEdge,                   sizeof (FallingEdge));
  ZeroMem ((UINT8 *) PulseWidth,                    sizeof (PulseWidth));
  ZeroMem ((UINT8 *) RankCount,                     sizeof (RankCount));
  ZeroMem ((UINT8 *) Cwl_Adj_Average,               sizeof (Cwl_Adj_Average));
  ZeroMem ((UINT8 *) TxDqsDelayValue,               sizeof (*TxDqsDelayValue));
  ZeroMem ((UINT8 *) PiDelayLocal,                  sizeof (PiDelayLocal));
  ZeroMem ((UINT8 *) FailChSubCh,                   sizeof (FailChSubCh));
  ZeroMem ((UINT8 *) InternalCycleAlignment,        sizeof (InternalCycleAlignment));
  ZeroMem ((UINT8 *) InternalCycleAlignmentChSubCh, sizeof (InternalCycleAlignmentChSubCh));
  //
  // Return if this Socket is disabled
  //
  Socket = Host->var.mem.currentSocket;
  MemSsType = GetSysCpuCsrAccessVar ()->MemSsType[Socket];

  //
  // Put the pattern into the WDB
  //
  //DDR5_TODO. To confirm the below code.
  IncludeDeselects = GetChipIncludeDeselectsWriteLevelingCleanUp ();

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    ByteSum[0][Ch] = 0;
    ByteSum[1][Ch] = 0;
    ByteCount[0][Ch] = 0;
    ByteCount[1][Ch] = 0;
    StrobeMask[Ch] = 0;
  } // Ch loop
  //
  // Enter sense amp training mode to all the channels here to latch results.
  //
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_SENSE_AMP);
  //
  // Initialize structures to write during the loop.
  //
  Mr0.Data = 0;
  if (IsBrsPresent (Host, Socket)) {
    Mr0.Bits.cas_latency_rl = DDR5_MR0_CAS_LATENCY_RL_24;
  } else {
    Mr0.Bits.cas_latency_rl = DDR5_MR0_CAS_LATENCY_RL_22;
  }

  SetTrainingModeDdr5 (Socket, TmWriteLeveing);
  //
  // Train each DIMM
  //
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Enable sampler_feedback_en
    //
    QcsQcaClkDdrioInit (Host, Socket, Dimm);
    //
    // Train each Rank
    //
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_WRITE_LEVELING_BASIC_DDR5, (UINT16) ((CHECKPOINT_DATA_WR_FLY_BY_TRAINING << 12) | (Socket << 8) | (Dimm << 4) | Rank)));
      ZeroMem ((UINT8 *) StrobePass, sizeof (StrobePass));
      ZeroMem ((UINT8 *) LockCwlAdj, sizeof (LockCwlAdj));
      //
      // Initialize to no ranks present
      //
      RankPresent = 0;
      MrcRankList.Dimm = Dimm;
      MrcRankList.Rank = Rank;

      if (IsBrsPresent (Host, Socket)) {
        SetTrainingMode (Host, Socket, Dimm, Rank, CLEAR_MODE);
      }

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (!IsChannelEnabled (Socket, Ch)) {
          continue;
        }
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Ch;
          GetChannelMapping (Ch, SubChannel, &ChannelLocal);
          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          if (FeaturePcdGet (PcdCteBuild) == TRUE) {
            //
            // Init DRAM interface only for CTE.
            //
            if (SubChannel == 0) {
              InitDramInterface (Host, Socket, Ch, Dimm, TWO_SUBCH_MASK, Rank);
            }
          }
          //
          // Get the logical Rank #
          //
          LogRank = GetLogicalRank (Host, Socket, Ch, Dimm, Rank);
          //
          // Set Strobe bit masks
          //
          StrobeMask[Ch] = GetSubChStrobeMaskDdr5 (Host);
          if (!(*DimmNvList) [Dimm].x4Present) {
            StrobeMask[Ch] &= 0x1F;
          }
          MaxStrobe = GetMaxStrobeScDdr5 (Host, Socket, ChannelLocal, Dimm, WrLvlDelay, DdrLevel);

          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }
            if ((*RankList) [Rank].faultyParts[Strobe]) {
              StrobeMask[Ch] &= ~(1 << Strobe);
            }
          } // Strobe loop
          //
          // Inidicate this Rank is present on at least one channel
          //
          RankPresent = 1;

          SelectCPGCRanks (Host, Socket, ChannelLocal, 1 << LogRank, 0, 0);

        } //subchannel loop

        if (FeaturePcdGet (PcdCteBuild) == TRUE) {
          //
          // Set the CAS latency (RL)
          //
          DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_0_REG, Mr0.Data);
        }
      } // Ch loop

      //
      // Go to the next Rank if this Rank is not present on any channels
      //
      if (!RankPresent) {
        continue;
      }
      //
      // Enable DDR5 DRAM Write Leveling mode and (no need to change Rtt_nom to Rtt_wr).
      //
#ifdef LRDIMM_SUPPORT
      Host->var.mem.InOvrRttPrkMode = 1;
#endif

      SetDqCountingWindow (Host, Socket, Dimm, Rank);
      ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (!((1 << Ch) & ChBitmask)) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Ch;
          GetChannelMapping (Ch, SubChannel, &ChannelLocal);
          //
          // Clear training results
          //
          for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
            for (DWord = 0; DWord < DDR5_TRAIN_RESULTS_LEN; DWord++) {
              (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] = 0;
            } // DWord loop
          } // Strobe loop

        } //SubChannel loop
      } // Ch loop

      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "DDR5 External Write Leveling Pi Scanning...\n");

      GetTxPiSamplepiDelayStartandEndDdr5 (Host, Socket, &PiDelay, &PiDelayEnd);

      ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);

      //
      // Set the count window--Delay for WL
      //
      SetDataTrainCountWindow (Host, Socket, Dimm, Rank, WL_TRAIN_COUNT_WINDOW);
      //
      // Setup up one time write command. Inside this function, it will setup the test for all the channels.
      //
      if (Host->DdrioUltSupport) {
        LoopCount = 1;
        GetWlFlyByLoopCount (&NumCl);
        CteSetupPattern ("wrlev", (LoopCount) | (NumCl << 4), ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank));

        CteSetOptions ("ww_dly", 7, TWO_SUBCH_MASK, 0, WL_WR_TO_WR_DLY);

      }
      //
      // Enable the DRAM External WL mode
      //
      EnableExternalWriteLevelingMode (Host, Socket, Dimm, Rank);

      //
      // Initialzie the DoneSum as 0.
      //
      DoneSum = 0;
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (!((1 << Ch) & ChBitmask)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        //
        // Set Strobe bit masks
        //
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        StrobeMask[Ch] = GetSubChStrobeMaskDdr5 (Host);
        if (!(*DimmNvList) [Dimm].x4Present) {
          StrobeMask[Ch] &= 0x1F;
        }
        //
        // Set mpr_train_ddr_on bit to disable ACT and PRE.
        //
        SetMprModeFilterActPre (Host, Socket, Ch, !(*DimmNvList) [Dimm].DcpmmPresent);

        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          DoneSum = DoneSum + 1;
          CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] = T_CWL_ADJ_INITIAL_VALUE;
          PiDelayLocal[Ch][SubChannel] = PiDelay;
          //
          // Enable the DDRIO training mode for all the strobes and set the drivedqslow as 1, strobemask as 0xFE.
          //
          SetDdrioWlModeDdr5 (Host, Socket, Ch, Dimm, SubChannel, Rank);
        }
      } //Ch loop
      SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, WLDdr5, gsmCsnDelim, 0, FIRST_ITERATION_LOOP);

      if ((GetPhyRank (Dimm, Rank) != 0) && (IsSiliconWorkaroundEnabled ("S14011080476"))) {
        //
        // Start the test to send the write command to the targeted rank.
        //
        if (Host->DdrioUltSupport) {
          CteStartPattern (ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank));
        } else {
          TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, WLDdr5, 1, START_TEST, NOT_STOP_TEST);
        }
        //
        // Add the Delay.
        //
        TrainingDelay (Host, 2600, 1);

        if (Host->DdrioUltSupport) {
          //
          // Wait the write command to be done for ULT.
          //
          CteWaitPatternDone ();
        }
      }

      //
      // Sweep through the PI/CWL_ADJ until we find a value that passes
      //
      Done = 0;
      Fail = 0;
      ZeroMem ((UINT8 *) SkipThis, sizeof (SkipThis));
      ZeroMem ((UINT8 *) StrobePass, sizeof (StrobePass));
      //
      // Start the Coarse Sweep.
      //
      for (; PiDelay <= PiDelayEnd; ) {
        //
        // Check the exit loop condition in case infinite loop..
        //
        if (DoneSum == Done + Fail) {
          break;
        }

        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (!((1 << Ch) & ChBitmask)) {
            continue;
          }

          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (Rank >= (*DimmNvList) [Dimm].numRanks) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            if (SkipThis[Ch][SubChannel] == 1) {
              continue;
            }
            if (StrobePass[Ch][SubChannel] == StrobeMask[Ch]) {
              Done = Done + 1;
              RankCount[(*DimmNvList) [Dimm].DcpmmPresent][Ch][SubChannel] = RankCount[(*DimmNvList) [Dimm].DcpmmPresent][Ch][SubChannel] + 1;
              CwlAdjRecord[(*DimmNvList) [Dimm].DcpmmPresent][Ch][SubChannel] = CwlAdjRecord[(*DimmNvList) [Dimm].DcpmmPresent][Ch][SubChannel] + CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex];
              SkipThis[Ch][SubChannel] = 1;
              continue;
            }
            if ((CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] >= DDR5_WL_CWL_ADJ_MAX)) {
              if (IsBrsPresent (Host, Socket)) {
                LockCwlAdj[Ch][SubChannel] = 1;
              } else {
                LockCwlAdj[Ch][0] = 1;
                LockCwlAdj[Ch][1] = 1;
              }
            }
            if ((PiDelayLocal[Ch][SubChannel]) > WR_LVL_PI_FULL_RANGE_DDR5) {
              SkipThis[Ch][SubChannel] = 1;
              Fail = Fail + 1;
              FailChSubCh[Ch][SubChannel] = 1;
              //
              // Error handling.
              //
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                "Warning: TxDqs Delay %d is out of range for external wl coarse training\n", PiDelayLocal[Ch][SubChannel]);
              DisableChannelSw (Host, Socket, Ch);
              EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_WL_EXTERNAL_COARSE, Socket, Ch, Dimm, Rank, EwlSeverityWarning, NO_STROBE, NO_BIT, WrLvlDelay, DdrLevel, 0xFF);

            }
            if (((PiDelayLocal[Ch][SubChannel] - WR_LVL_PI_START_DDR5) == (WR_LVL_WRAP_STEP_SIZE_DDR5 + WR_LVL_COARSE_STEP_SIZE_DDR5)) && (LockCwlAdj[Ch][SubChannel] == 0)) {
              CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] = CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] + 1;
              PiDelayLocal[Ch][SubChannel] = PiDelayLocal[Ch][SubChannel] - 128;
            }
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
              "PiDelay = %3d, CWL_ADJ = %2d\n", PiDelayLocal[Ch][SubChannel], CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex]);

            //
            // The register ddrd_train_ctl2.drv_dqs_diff_low must be 0 in order for the TxDqs Delay value to be applied. Therefore, before setting the TxDqs Delay,
            // ddrd_train_ctl2.drv_dqs_diff_low must be set to 0. After setting TxDqs Delay, ddrd_train_ctl2.drv_dqs_diff_low must be set back to 1 to ensure
            // differ-ential drive low during idle.
            //
            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 0);

            //
            // Program TxDQS Delay.
            //
            ProgramTxDqsDelay (Host, Socket, Ch, Dimm, SubChannel, Rank, StrobePass[Ch][SubChannel], GSM_FORCE_WRITE | GSM_UPDATE_CACHE, PiDelayLocal[Ch][SubChannel]);
            //
            // Write Wr ADD Delays
            //
            if (!LockCwlAdj[Ch][SubChannel]) {
              WriteChipWrADDDelaysDdr5 (Host, Socket, Ch, SubChannel, CWLADJ_ALL, 0, CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex], &WLChipCleanUpStruct);
            }

            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 1);
            //
            // Need Delay here to allow train reset to complete, and ensure RfFSM isn't accessed while in InitOp.
            //
            TrainingDelay (Host, 10 * 16, 2);

          }//SubChannel loop.
        } // Ch loop
        IO_Reset(Host, Socket); // Need reset before enabling training mode.
        //
        // Need Delay here to allow train reset to complete, and ensure RfFSM isn't accessed while in InitOp.
        //
        TrainingDelay (Host, 5 * 16, 1);
        //
        // The WRITE command must occur after a Delay of tWLPEN relative to when the MRW enabled Write Leveling Training Mode
        // DDR5_TODO. To check how long the tWLPEN is .It should enough here.
        //
        // Start the test.
        //
        if (Host->DdrioUltSupport) {
          CteStartPattern (ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank));
        } else {
          TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, WLDdr5, 1, START_TEST, NOT_STOP_TEST);
        }

        //
        // Add the Delay to make sure there is no write command overlap between the two PI loop.
        //
        TrainingDelay (Host, 2600, 1);

        //
        // Clear Errors and wait to collect results
        //
        if (Host->DdrioUltSupport) {
          CteWaitPatternDone ();
        }

        GetResultsWLDdr5 (Host, Socket, Dimm, Rank, PiDelayLocal, TempCwlAdj, StrobePass, CwlAdj, CwlAdjRecord, LockCwlAdj, TxDqsDelayValue, *TrainRes);
        //
        // Add pidelay Step size.
        //
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (!((1 << Ch) & ChBitmask)) {
            continue;
          }

          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (Rank >= (*DimmNvList) [Dimm].numRanks) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            if (SkipThis[Ch][SubChannel] == 1) {
              continue;
            }
            PiDelayLocal[Ch][SubChannel] = PiDelayLocal[Ch][SubChannel] + WR_LVL_COARSE_STEP_SIZE_DDR5;
          }//subchannel loop
        }//Ch loop
      } //pidelay loop
      //
      // Display the training results.
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (!IsChannelEnabled (Socket, Ch)) {
          continue;
        }

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        // Continue to the next Rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }

        MaxStrobe = GetMaxStrobeScDdr5 (Host, Socket, Ch, Dimm, WrLvlDelay, DdrLevel);
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
#ifdef DEBUG_CODE_BLOCK
          if (GetDebugLevel () & SDBG_MAX) {
            AcquirePrintControl ();
            RcDebugPrint (SDBG_DEFAULT, "\nSummary: DDR5 External Write Leveling Coarse Pi");
            RcDebugPrint (SDBG_DEFAULT, "\nS%d, Ch%d, DIMM%d, SubCh %d, Rank%d , CWL_ADJ = %d\n", Socket, Ch, Dimm, SubChannel, Rank, CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex]);
            //
            // Display sample results
            //
            PrintSampleArrayExternalCoarse (Host, Socket, Ch, SubChannel, Dimm, Rank, TX_DQS_DELAY_RANGE, TrainRes);
            //
            // Display results
            //
            DisplayWlCoarseResultsDdr5 (Host, Socket, Ch, Dimm, Rank, SubChannel, MaxStrobe);
            ReleasePrintControl ();
          }
#endif // DEBUG_CODE_BLOCK
        } //subchannel loop
        //
        //  Equalize the Cwl between two sub channels.
        //
        EqualizeCwl (Host, Socket, Ch, Dimm, Rank, MaxStrobe, CwlAdj, &WLChipCleanUpStruct, TxDqsDelayValue);
      } //Ch loop

      //
      // External WL Fine Sweep Start.
      //
      ZeroMem ((UINT8 *) TrainRes, sizeof (*TrainRes));
      ZeroMem ((UINT8 *) SkipThis, sizeof (SkipThis));
      ZeroMem ((UINT8 *) StrobePass, sizeof (StrobePass));
      //
      // Shift Back the TxDqs Delay by 64
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (!IsChannelEnabled (Socket, Ch)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        if (IsX4Dimm (Socket, Ch, Dimm)) {
          MaxStrobe = SubChMSVx4;
        } else {
          MaxStrobe = SubChMSVx8;
        }
        // Continue to the next Rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
          ChannelLocal = Ch;
          GetChannelMapping (Ch, SubChannel, &ChannelLocal);
          //
          // Start to evaluate the training results.
          //
          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }
            GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&PiDelay);
            TxDqsDelayValue[Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe] = PiDelay;
          } // Strobe loop
        } //subchannel loop
      } //Ch loop
      //
      // Sweep through the cycle offsets until we find the rising edges.In the first loop, the TxDQS will be shift back by 64.
      //
      Done = 0;
      for (Offsets = WR_LVL_FINE_PI_START_DDR5; Offsets <= WR_LVL_FINE_PI_END_DDR5; Offsets += WR_LVL_FINE_STEP_SIZE_DDR5) {
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (!((1 << Ch) & ChBitmask)) {
            continue;
          }
          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (Rank >= (*DimmNvList) [Dimm].numRanks) {
            continue;
          }
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {

            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 0);
            //
            // Program the offset.
            //
            if (Offsets == WR_LVL_FINE_PI_START_DDR5) {
              PiDelay = WR_LVL_FINE_PI_START_DDR5;
            } else {
              PiDelay = WR_LVL_FINE_STEP_SIZE_DDR5;
            }
            ProgramTxDqsDelay (Host, Socket, Ch, Dimm, SubChannel, Rank, StrobePass[Ch][SubChannel], GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, PiDelay);
            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 1);

            MaxStrb = GetMaxStrobeScDdr5 (Host, Socket, Ch, Dimm, WrLvlDelay, DdrLevel);

            //
            // Need Delay here to allow settling of Tx PiCode changes.
            //
            TrainingDelay (Host, 10 * 16, 2);

          }//SubChannel loop.
        } // Ch loop
        IO_Reset(Host, Socket); // Need reset before enabling training mode.
        //
        // Need Delay here to allow train reset to complete, and ensure RfFSM isn't accessed while in InitOp.
        //
        TrainingDelay (Host, 5 * 16, 1);
        //
        // Start the test.
        //
        SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, WLDdr5, gsmCsnDelim, 0, FIRST_ITERATION_LOOP);
        if (Host->DdrioUltSupport) {
          CteStartPattern (ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank));
        } else {
          TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, WLDdr5, 1, START_TEST, NOT_STOP_TEST);
        }

        TrainingDelay (Host, 5 * 16, 1);

        //
        // Clear Errors and wait to collect results
        //
        if (Host->DdrioUltSupport) {
          CteWaitPatternDone ();
        }

        //
        // Clear Errors and wait to collect results
        //
        GetResultsWLFineSweep (Host, Socket, Dimm, Rank, Offsets, CwlAdj, TxDqsDelayValue, *TrainRes);
      } //pidelay loop
      //
      // Here, the TxDQS Delay is the original value. Display the training log.
      //
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (!IsChannelEnabled (Socket, Ch)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (IsX4Dimm (Socket, Ch, Dimm)) {
          MaxStrobe = SubChMSVx4;
        } else {
          MaxStrobe = SubChMSVx8;
        }
        // Continue to the next Rank if this one is disabled
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
#ifdef DEBUG_CODE_BLOCK
          if (GetDebugLevel () & SDBG_MAX) {
            AcquirePrintControl ();
            RcDebugPrint (SDBG_DEFAULT, "\nSummary: DDR5 External Write Leveling Fine Sweep Pi");
            RcDebugPrint (SDBG_DEFAULT, "\nS%d, Ch%d, DIMM%d, SubCh %d, Rank%d\n", Socket, Ch, Dimm, SubChannel, Rank);
            PrintSampleArrayExternalFine (Host, Socket, Ch, SubChannel, Dimm, Rank, TX_DQS_DELAY_WL_FINE_SWEEP_RANGE, TrainRes);
            ReleasePrintControl ();
          }
#endif // DEBUG_CODE_BLOCK

          ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 0);
          //
          // Start to evaluate the training results.
          //
          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }

            EvaluatePiSettingResultsWlFineSweep (Host, Socket, Ch, Dimm, Rank, Strobe, RisingEdge, FallingEdge,
              PI_WL_FINE_DDR5_TYPE, SubChannel, WlFineSweep, TrainRes);

            //
            // Program the rising edge results.
            //
            ChannelLocal = Ch;
            GetChannelMapping (Ch, SubChannel, &ChannelLocal);

            PiDelay = WR_LVL_FINE_PI_START_DDR5 + TxDqsDelayValue[Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe] + RisingEdge[Ch][SubChannel][Strobe];
            if ((*DimmNvList) [Dimm].fmcType == FMC_CWV_TYPE) {
              PiDelay = PiDelay + GetDdrt2WLAdjEndValue (Socket, ChannelLocal, Dimm);
            }
            GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&PiDelay);
            TxDqsDelayValue[Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe] = PiDelay;
            if (!(*DimmNvList)[Dimm].x4Present) {//DDR5_TODO. This should no need for TxDQS? But it's there in DDR4.
              GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe + SubChMSVx8, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&PiDelay);
            }
          } // Strobe loop
          ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 1);

#ifdef DEBUG_CODE_BLOCK
          // Display results
          DisplayEdgesWlFineSweep (Host, Socket, Ch, SubChannel, Dimm, Rank, PulseWidth[Ch][SubChannel], FallingEdge[Ch][SubChannel], RisingEdge[Ch][SubChannel], MaxStrobe, WLDdr5);
#endif // DEBUG_CODE_BLOCK
        } //subchannel loop
      } //Ch loop
      //DetectLowMargin(Host, NO_CH, Dimm, Rank, PulseWidth, TxDqsDelay, NON_PER_BIT);

#ifdef LRDIMM_SUPPORT
      Host->var.mem.InOvrRttPrkMode = 0;
#endif

      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (!IsChannelEnabled (Socket, Ch)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
          continue;
        }
        if (IsLrdimmPresent (Socket, Ch, Dimm)) {
          //
          // Databuffer does not have internal write leveling
          //
          ChBitmask &= !(1 << Ch);
          continue;
        }

        DisplaySpecificRankResultsDdr5 (Host, Socket, Ch, Dimm, Rank, DISPLAY_FROM_REGISTER, FALSE, &CwlAdj, TxDqsDelay);
      }

      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
        "\nDDR5 Internal Write Leveling Training \n");
      //
      // DDR5 WL Internal Training Start.
      //
      if ((FeaturePcdGet (PcdCteBuild) != TRUE)) {
        //
        // Internal Write Leveling Coarse Start.
        //
        //
        // Figure out the tWL_ADJ_start and tWL_ADJ_end based on the tWPRE.
        //
        WritePreamble = GetPreambleState (Socket, WRITE_PREAMBLE);
        switch (WritePreamble) {
          case PREAMBLE_1TCLK:
            WlAdjStart = WL_ADJ_START_1TCK;
            WlAdjEnd = WL_ADJ_END_1TCK;
            break;
          case PREAMBLE_2TCLK:
            WlAdjStart = WL_ADJ_START_2TCK;
            WlAdjEnd = WL_ADJ_END_2TCK;
            break;
          case PREAMBLE_3TCLK:
            WlAdjStart = WL_ADJ_START_3TCK;
            WlAdjEnd = WL_ADJ_END_3TCK;
            break;
          case PREAMBLE_4TCLK:
            WlAdjStart = WL_ADJ_START_4TCK;
            WlAdjEnd = WL_ADJ_END_4TCK;
            break;
          default:
            break;
        }

        //
        // Program the WL_ADJ_Start.
        //
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (!((1 << Ch) & ChBitmask)) {
            continue;
          }

          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (Rank >= (*DimmNvList) [Dimm].numRanks) {
            continue;
          }

          if (IsX4Dimm (Socket, Ch, Dimm)) {
            MaxStrobe = SubChMSVx4;
          } else {
            MaxStrobe = SubChMSVx8;
          }

          if (IsLrdimmPresent (Socket, Ch, Dimm)) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            //
            // Clear training results
            //
            for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
              for (DWord = 0; DWord < DDR5_TRAIN_RESULTS_LEN; DWord++) {
                (*TrainRes) [Ch][SubChannel][Strobe].results[DWord] = 0;
              } // DWord loop
            } // Strobe loop

            //
            //3.a If the ddr_dqs_adj - (1 tCK) or ddr_dqs_adj - 128 > Minimum value of ddr_dqs_adj
            //    then set ddr_dqs_adj as ddr_dqs_adj -1, and set TxDQS = TxDQS + 128.
            //3.b Set t_cwl_adj to ddr_dqs_adj + Preamble Length.
            //3.c Set TxDqs = TxDqs + tWL_Adj_Start. Report failure if TxDqs < 0 AND t_cwl_adj = -3
            //    - If Preamble = 2tCK -> txdqs_dly = txdqs_dly + (- 0.75*128)
            //    - If Preamble = 3tCK -> txdqs_dly = txdqs_dly + (- 1.25*128)
            //
            if (CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] - 1 >= MIN_CWL_ADJ_DDR5) {
              CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] = CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] - 1;
              PiLocal = 128 + WlAdjStart;
              WriteChipWrADDDelaysDdr5 (Host, Socket, Ch, SubChannel, 0, 0, CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex], &WLChipCleanUpStruct);
            } else {
              PiLocal = WlAdjStart;
            }

            ChannelLocal = Ch;
            GetChannelMapping (Ch, SubChannel, &ChannelLocal);
            //
            // Get the minimal TxDqs delay
            //
            MinPi = GetMinTxDqsPi (Host, Socket, Ch, SubChannel, Dimm, Rank);
            RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubChannel, Rank, NO_STROBE, NO_BIT,
              "Min Pi value in internal wl coarse sweep is 0x%x\n", MinPi);

            //
            // Recursively subtract cwl_adj till reach sufficient range for internal wl coarse training to sweep
            //
            while ((MinPi + PiLocal) < 0) {
              if ((CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] - 1 >= MIN_CWL_ADJ_DDR5)) {
                CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] = CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] - 1;
                PiLocal = PiLocal + 128;
                WriteChipWrADDDelaysDdr5 (Host, Socket, Ch, SubChannel, 0, 0, CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex], &WLChipCleanUpStruct);
              } else {
                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubChannel, Rank, NO_STROBE, NO_BIT,
                  "Warning: CWL_ADJ %x is not sufficient for internal wl coarse training\n", (CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex]));
                DisableChannelSw (Host, Socket, Ch);
                EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_WL_INTERNAL_OUT_OF_CYCLE, Socket, Ch, Dimm, Rank, EwlSeverityWarning, NO_STROBE, NO_BIT, WrLvlDelay, DdrLevel, 0xFF);
                break;
              }
            }

            //
            // Program the TxDQS Delay.
            //
            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 0);
            for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
              //
              // Skip if this is an ECC Strobe when ECC is disabled
              //
              if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                continue;
              }
              GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&PiDelayLocalInt);
              PiDelayLocalInt = PiDelayLocalInt + PiLocal;
              GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&PiDelayLocalInt);
              TxDqsDelayValue[Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe] = PiDelayLocalInt;
              if (!(*DimmNvList) [Dimm].x4Present) {
                GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe + SubChMSVx8, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&PiDelayLocalInt);
              }
            }
            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 1);
          } //SubChannel loop
          //
          //  Equalize the Cwl between two sub channels.
          //
          EqualizeCwl (Host, Socket, Ch, Dimm, Rank, MaxStrobe, CwlAdj, &WLChipCleanUpStruct, TxDqsDelayValue);
        } // Ch loop

        Done = 0;
        Fail = 0;
        ZeroMem ((UINT8 *) SkipThis, sizeof (SkipThis));
        ZeroMem ((UINT8 *) StrobePass, sizeof (StrobePass));
        //
        // Start the Internal WL Coarse Sweep.
        //
        for (WlInternalCycleAlignment = 0; WlInternalCycleAlignment <= MAX_INTERNAL_CYCLE; WlInternalCycleAlignment++) {
          //
          // Check the exit loop condition in case infinite loop..
          //
          if (DoneSum == Done + Fail) {
            break;
          }

          for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
            if (!((1 << Ch) & ChBitmask)) {
              continue;
            }

            DimmNvList = GetDimmNvList (Host, Socket, Ch);
            if (Rank >= (*DimmNvList) [Dimm].numRanks) {
              continue;
            }
            RankList = GetRankNvList (Host, Socket, Ch, Dimm);
            for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
              if (SkipThis[Ch][SubChannel] == 1) {
                continue;
              }
              if (StrobePass[Ch][SubChannel] == StrobeMask[Ch]) {
                Done = Done + 1;
                SkipThis[Ch][SubChannel] = 1;
                InternalCycleAlignmentChSubCh[Ch][SubChannel][ (*RankList) [Rank].rankIndex] = WlInternalCycleAlignment - 1;
                continue;
              }

              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                "Wl Internal Cycle Alignment = %2d\n", WlInternalCycleAlignment);
              //
              // Program the internal cycle alignment.
              //
              //
              // Need Delay here to allow train reset to complete, and ensure RfFSM isn't accessed while in InitOp.
              //
              TrainingDelay (Host, 5 * 16, 1);
              //
              // Need Delay here to allow train reset to complete, and ensure RfFSM isn't accessed while in InitOp.
              //
              TrainingDelay (Host, 5 * 16, 1);
              ReadDramModeRegCachePerRank (Socket, Ch, SUB_CH_A, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, &Mr2.Data);
              Mr2.Bits.write_leveling = 1;
              Mr2.Bits.internal_write_timing = 1;
#ifdef USE_LATEST_DRAM_SPEC
              DramModeRegWrite (Socket, Ch, 1 << SubChannel, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, Mr2.Data);
              Mr3.Data = 0;
              Mr3.Bits.write_leveling_internal_cycle_alignment_lower_byte = WlInternalCycleAlignment & 0x0F;
              //
              // Upper byte only appliable to x16 Dimm
              //
              if (((*DimmNvList)[Dimm].dramIOWidth) == SPD_DRAM_IO_WIDTH_X16) {
                Mr3.Bits.write_leveling_internal_cycle_alignment_upper_byte = WlInternalCycleAlignment & 0x0F;
              }
              DramModeRegWrite (Socket, Ch, 1 << SubChannel, Dimm, Rank, DDR5_MODE_REGISTER_3_REG, Mr3.Data);
#else // #ifdef USE_LATEST_DRAM_SPEC
              Mr2.Bits.wl_internal_cycle_alignment = WlInternalCycleAlignment;
              DramModeRegWrite (Socket, Ch, 1 << SubChannel, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, Mr2.Data);
#endif // #ifdef USE_LATEST_DRAM_SPEC
              TrainingDelay (Host, 2, 1);
            }//SubChannel loop.
          } // Ch loop
          //
          // The WRITE command must occur after a Delay of tWLPEN relative to when the MRW enabled Write Leveling Training Mode
          // DDR5_TODO. To check how long the tWLPEN is .It should enough here.
          //
          // Start the test.
          //
          SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, WLDdr5, gsmCsnDelim, 0, FIRST_ITERATION_LOOP);
          if (Host->DdrioUltSupport) {
            CteStartPattern (ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank));
          } else {
            TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, WLDdr5, 1, START_TEST, NOT_STOP_TEST);
          }
          //
          // Clear Errors and wait to collect results
          //
          TrainingDelay (Host, 5 * 16, 1);
          //
          // Clear Errors and wait to collect results
          //
          GetResultsInternalWLDdr5 (Host, Socket, ChBitmask, Dimm, Rank, StrobePass, WlInternalCycleAlignment, InternalCycleAlignment, *TrainRes);
        } //WlInternalCycleAlignment loop
        //
        // Display the training results and program the result.
        //
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (!IsChannelEnabled (Socket, Ch)) {
            continue;
          }

          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          if (IsX4Dimm (Socket, Ch, Dimm)) {
            MaxStrobeDimm = SubChMSVx4;
          } else {
            MaxStrobeDimm = SubChMSVx8;
          }
          // Continue to the next Rank if this one is disabled
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
            continue;
          }

          if (IsLrdimmPresent (Socket, Ch, Dimm)) {
            continue;
          }
          MaxStrobe = GetMaxStrobeScDdr5 (Host, Socket, Ch, Dimm, WrLvlDelay, DdrLevel);
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            //
            // Program the internal cycle alignment for each devices.
            //
            for (Strobe = 0; Strobe < MaxStrobeDimm; Strobe++) {
              //
              // Skip if this is an ECC Strobe when ECC is disabled
              //
              if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                continue;
              }
              //
              // Program the PDA select ID.
              //
              WriteMPC (Host, Socket, Ch, Dimm, 1 << SubChannel, Rank, PDA_SELECT_ID | Strobe, TIMING_4N, TRUE);//DDR5_todo. SPR_todo. To do two subchannel one time in SPR.
              //
              // Set internal cycle alignment for each dram device.
              //
#ifdef USE_LATEST_DRAM_SPEC
              ReadDramModeRegCachePerRank (Socket, Ch, SubChannel, Dimm, Rank, DDR5_MODE_REGISTER_3_REG, &Mr3.Data);
              Mr3.Bits.write_leveling_internal_cycle_alignment_lower_byte = InternalCycleAlignment[Ch][SubChannel][(*RankList)[Rank].rankIndex][Strobe] & 0x0F;
              //
              // Upper byte only appliable to x16 Dimm
              //
              if (((*DimmNvList)[Dimm].dramIOWidth) == SPD_DRAM_IO_WIDTH_X16) {
                Mr3.Bits.write_leveling_internal_cycle_alignment_upper_byte = InternalCycleAlignment[Ch][SubChannel][(*RankList)[Rank].rankIndex][Strobe] & 0x0F;
              }
              DramModeRegWrite (Socket, Ch, 1 << SubChannel, Dimm, Rank, DDR5_MODE_REGISTER_3_REG, Mr3.Data);
#else // #ifdef USE_LATEST_DRAM_SPEC
              ReadDramModeRegCachePerRank (Socket, Ch, SubChannel, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, &Mr2.Data);
              Mr2.Bits.wl_internal_cycle_alignment = WlInternalCycleAlignment;
              DramModeRegWrite (Socket, Ch, 1 << SubChannel, Dimm, Rank, DDR5_MODE_REGISTER_2_REG, Mr2.Data);
#endif // #ifdef USE_LATEST_DRAM_SPEC
              WriteMPC (Host, Socket, Ch, Dimm, 1 << SubChannel, Rank, PDA_SELECT_ALL, TIMING_4N, TRUE);
            } //Strobe loop
#ifdef DEBUG_CODE_BLOCK
            if (GetDebugLevel () & SDBG_MAX) {
              AcquirePrintControl ();
              RcDebugPrint (SDBG_DEFAULT, "\nSummary: DDR5 Internal Write Leveling Coarse Pi");
              RcDebugPrint (SDBG_DEFAULT, "\nS%d, Ch%d, DIMM%d, SubCh %d, Rank%d , WL_INT_CYC_ALM = %d\n", Socket, Ch, Dimm, SubChannel, Rank, InternalCycleAlignmentChSubCh[Ch][SubChannel][ (*RankList) [Rank].rankIndex]);
              //
              // Display sample results
              //
              PrintSampleArrayInternalCoarseFine (Host, Socket, Ch, SubChannel, Dimm, Rank, MAX_INTERNAL_CYCLE, TrainRes, InternalWlCoarseSweepTest);
              //
              // Display results
              //
              DisplayInternalWlCoarseResultsDdr5 (Host, Socket, Ch, Dimm, Rank, SubChannel, MaxStrobe, InternalCycleAlignment);
              ReleasePrintControl ();
            }
#endif // DEBUG_CODE_BLOCK
          } //subchannel loop
        } //Ch loop
        //
        // Internal WL Fine Sweep Start.
        //
        ZeroMem ((UINT8 *) TrainRes, sizeof (*TrainRes));
        Done = 0;
        Fail = 0;
        ZeroMem ((UINT8 *) SkipThis, sizeof (SkipThis));
        ZeroMem ((UINT8 *) StrobePass, sizeof (StrobePass));
        //
        // Program the WL_ADJ_Start.
        //
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (!((1 << Ch) & ChBitmask)) {
            continue;
          }

          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (Rank >= (*DimmNvList) [Dimm].numRanks) {
            continue;
          }

          if (IsX4Dimm (Socket, Ch, Dimm)) {
            MaxStrobe = SubChMSVx4;
          } else {
            MaxStrobe = SubChMSVx8;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            if (CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] - 1 >= MIN_CWL_ADJ_DDR5) {
              CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] = CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] - 1;
              WriteChipWrADDDelaysDdr5 (Host, Socket, Ch, SubChannel, 0, 0, CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex], &WLChipCleanUpStruct);
              PiLocal = 0 - WR_LVL_INTERNAL_FINE_PI_EXT_RANGE;
            } else {
              PiLocal = 0 - (WR_LVL_INTERNAL_FINE_PI_END_DDR5 + WR_LVL_INTERNAL_FINE_PI_EXT_RANGE);
            }
            ChannelLocal = Ch;
            GetChannelMapping (Ch, SubChannel, &ChannelLocal);
            //
            // Get the minimal TxDqs delay
            //
            MinPi = GetMinTxDqsPi (Host, Socket, Ch, SubChannel, Dimm, Rank);
            RcDebugPrint (SDBG_MEM_TRAIN, "Min Pi value of subch %d in internal wl fine sweep is 0x%x \n",SubChannel, MinPi);

            //
            // Recursively subtract cwl_adj till reach sufficient range for internal wl coarse training to sweep
            //
            while ((MinPi + PiLocal) < 0) {
              if ((CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] - 1 >= MIN_CWL_ADJ_DDR5)) {
                CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] = CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] - 1;
                PiLocal = PiLocal + 128;
                WriteChipWrADDDelaysDdr5 (Host, Socket, Ch, SubChannel, 0, 0, CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex], &WLChipCleanUpStruct);
              } else {
                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubChannel, Rank, NO_STROBE, NO_BIT,
                  "Warning: CWL_ADJ %x is not sufficient for internal wl fine training\n", (CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex]));
                DisableChannelSw (Host, Socket, Ch);
                EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_WL_INTERNAL_OUT_OF_CYCLE, Socket, Ch, Dimm, Rank, EwlSeverityWarning, NO_STROBE, NO_BIT, WrLvlDelay, DdrLevel, 0xFF);
                break;
              }
            }

            //
            // Program the TxDQS Delay.
            //
            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 0);
            for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
              //
              // Skip if this is an ECC Strobe when ECC is disabled
              //
              if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                continue;
              }
              GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY | GSM_READ_CSR, (INT16 *)&PiDelayLocalInt);
              PiDelayLocalInt = PiDelayLocalInt + PiLocal;
              GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&PiDelayLocalInt);
              TxDqsDelayValue[Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe] = PiDelayLocalInt;
              if (!(*DimmNvList) [Dimm].x4Present) {

                GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe + SubChMSVx8, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&PiDelayLocalInt);
              }
            }
            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 1);
          } //SubChannel loop
          //
          //  Equalize the Cwl between two sub channels.
          //
          EqualizeCwl (Host, Socket, Ch, Dimm, Rank, MaxStrobe, CwlAdj, &WLChipCleanUpStruct, TxDqsDelayValue);
        } // Ch loop

        //
        // Start the Internal WL Fine Sweep.
        //
        for (Offsets = WR_LVL_INTERNAL_FINE_PI_START_DDR5; Offsets <= WR_LVL_INTERNAL_FINE_PI_END_DDR5 + WR_LVL_INTERNAL_FINE_PI_EXT_RANGE + WR_LVL_INTERNAL_FINE_PI_ADDITIONAL; Offsets += WR_LVL_INTERNAL_FINE_STEP_SIZE_DDR5) {
          for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
            if (!((1 << Ch) & ChBitmask)) {
              continue;
            }

            DimmNvList = GetDimmNvList (Host, Socket, Ch);
            if (Rank >= (*DimmNvList) [Dimm].numRanks) {
              continue;
            }
            RankList = GetRankNvList (Host, Socket, Ch, Dimm);
            for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
              ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 0);
              //
              // Program the TxDqs Delay.
              //
              if (Offsets == WR_LVL_INTERNAL_FINE_PI_START_DDR5) {
                PiDelay = WR_LVL_INTERNAL_FINE_PI_START_DDR5;
              } else {
                PiDelay = WR_LVL_INTERNAL_FINE_STEP_SIZE_DDR5;
              }
              ProgramTxDqsDelay (Host, Socket, Ch, Dimm, SubChannel, Rank, StrobePass[Ch][SubChannel], GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, PiDelay);
              ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 1);
              TrainingDelay (Host, 10 * 16, 2);
            }//SubChannel loop.
          } // Ch loop
          //
          // Need reset before starting training.
          //
          IO_Reset (Host, Socket);
          //
          // Need Delay here to allow train reset to complete, and ensure RfFSM isn't accessed while in InitOp.
          //
          TrainingDelay (Host, 5 * 16, 1);
          //
          // The WRITE command must occur after a Delay of tWLPEN relative to when the MRW enabled Write Leveling Training Mode
          // DDR5_TODO. To check how long the tWLPEN is .It should enough here.
          //
          // Start the test.
          //
          SetupTest (Host, Socket, MemSsType, ChBitmask, 1, &MrcRankList, DdrLevel, WLDdr5, gsmCsnDelim, 0, FIRST_ITERATION_LOOP);
          if (Host->DdrioUltSupport) {
            CteStartPattern (ChBitmask, TWO_SUBCH_MASK, GetPhyRank (Dimm, Rank));
          } else {
            TestControl (Host, Socket, MemSsType, ChBitmask, NO_SUBCH, WLDdr5, 1, START_TEST, NOT_STOP_TEST);
          }
          //
          // Clear Errors and wait to collect results
          //
          TrainingDelay (Host, 5 * 16, 1);

          //
          // Clear Errors and wait to collect results
          //
          GetResultsInternalWLFineSweepDdr5 (Host, Socket, Dimm, Rank, StrobePass, Offsets, InternalCycleAlignment, TxDqsDelayValue, *TrainRes);
        } //Offsets loop
        //
        // Display the training log.
        //
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (!IsChannelEnabled (Socket, Ch)) {
            continue;
          }
          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (IsX4Dimm (Socket, Ch, Dimm)) {
            MaxStrobe = SubChMSVx4;
          } else {
            MaxStrobe = SubChMSVx8;
          }

          // Continue to the next Rank if this one is disabled
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList) [Dimm].numRanks)) {
            continue;
          }
          if (IsLrdimmPresent (Socket, Ch, Dimm)) {
            continue;
          }
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
#ifdef DEBUG_CODE_BLOCK
            if (GetDebugLevel () & SDBG_MAX) {
              AcquirePrintControl ();
              RcDebugPrint (SDBG_DEFAULT, "\nSummary: DDR5 Internal Write Leveling Fine Sweep Pi");
              RcDebugPrint (SDBG_DEFAULT, "\nS%d, Ch%d, DIMM%d, SubCh %d, Rank%d\n", Socket, Ch, Dimm, SubChannel, Rank);
              PrintSampleArrayInternalCoarseFine (Host, Socket, Ch, SubChannel, Dimm, Rank, WR_LVL_INTERNAL_FINE_PI_END_DDR5 + WR_LVL_INTERNAL_FINE_PI_EXT_RANGE + WR_LVL_INTERNAL_FINE_PI_ADDITIONAL + 1, TrainRes, InternalWlFineSweepTest);
              ReleasePrintControl ();
            }
#endif // DEBUG_CODE_BLOCK
            //
            // Start to evaluate the training results.
            //
            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 0);
            for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
              //
              // Skip if this is an ECC Strobe when ECC is disabled
              //
              if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                continue;
              }

              EvaluatePiSettingResultsInternalWlFineSweep (Host, Socket, Ch, Dimm, Rank, Strobe, RisingEdge, FallingEdge,
                PI_WL_FINE_DDR5_TYPE, SubChannel, InternalWlFineSweepTest, TrainRes);

              //
              // Program the rising edge results.
              //
              ChannelLocal = Ch;
              GetChannelMapping (Ch, SubChannel, &ChannelLocal);
              PiDelay = TxDqsDelayValue[Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe] + RisingEdge[Ch][SubChannel][Strobe];
              GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&PiDelay);
              TxDqsDelayValue[Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe] = PiDelay;
              if (!(*DimmNvList) [Dimm].x4Present) {
                GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe + SubChMSVx8, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&PiDelay);
              }
            }  // Strobe loop
            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 1);
#ifdef DEBUG_CODE_BLOCK
            // Display results
            DisplayEdgesWlFineSweep (Host, Socket, Ch, SubChannel, Dimm, Rank, PulseWidth[Ch][SubChannel], FallingEdge[Ch][SubChannel], RisingEdge[Ch][SubChannel], MaxStrobe, InternalWlFineSweepTest);
#endif // DEBUG_CODE_BLOCK
          } //subchannel loop
        } //Ch loop
        //
        // Program the WL_ADJ_END
        //
        for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
          if (!((1 << Ch) & ChBitmask)) {
            continue;
          }

          if (IsLrdimmPresent (Socket, Ch, Dimm)) {
            continue;
          }

          DimmNvList = GetDimmNvList (Host, Socket, Ch);
          if (Rank >= (*DimmNvList) [Dimm].numRanks) {
            continue;
          }

          if (IsX4Dimm (Socket, Ch, Dimm)) {
            MaxStrobe = SubChMSVx4;
          } else {
            MaxStrobe = SubChMSVx8;
          }

          RankList = GetRankNvList (Host, Socket, Ch, Dimm);
          for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
            //
            // Offset the CWL_ADJ by WL_ADJ_END
            //
            GetCwlAdjDdr5 (Host, Socket, Ch, SubChannel, Dimm, &CwlAdjLocal);
            if (CwlAdjLocal + 1 <= MAX_CWL_ADJ_DDR5) {
              CwlAdjTemp = 1;
              CwlAdjLocal = CwlAdjLocal + CwlAdjTemp;
              CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] = CwlAdj[Ch][SubChannel][ (*RankList) [Rank].rankIndex] + CwlAdjTemp;
              PiLocal = WlAdjEnd - 128;
              WriteChipWrADDDelaysDdr5 (Host, Socket, Ch, SubChannel, 0, 0, CwlAdjLocal, &WLChipCleanUpStruct);
            } else {
              PiLocal = WlAdjEnd;
            }
            ChannelLocal = Ch;
            GetChannelMapping (Ch, SubChannel, &ChannelLocal);
            //
            // Program the TxDQS Delay.
            //
            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 0);
            for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
              //
              // Skip if this is an ECC Strobe when ECC is disabled
              //
              if (IsStrobeNotValidDdr5 (Host, Strobe)) {
                continue;
              }
              //
              // DDR5_TODO. BRS_TODO. Consider the DQS mapping.
              //
              GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, (INT16 *)&PiDelayLocalInt);
              PiDelayLocalInt = PiDelayLocalInt + PiLocal;
              GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&PiDelayLocalInt);
              TxDqsDelayValue[Ch][SubChannel][ (*RankList) [Rank].rankIndex][Strobe] = PiDelayLocalInt;
              if (!(*DimmNvList) [Dimm].x4Present) {
                GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe + SubChMSVx8, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&PiDelayLocalInt);
              }
            } // Strobe loop
            ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 1);
          } //SubChannel loop
          //
          //  Equalize the Cwl between two sub channels.
          //
          EqualizeCwl (Host, Socket, Ch, Dimm, Rank, MaxStrobe, CwlAdj, &WLChipCleanUpStruct, TxDqsDelayValue);
        } // Ch loop
      }

      //
      // Restore Rtt_nom and disable DRAM Write Leveling mode
      //
#ifdef LRDIMM_SUPPORT
      Host->var.mem.InOvrRttPrkMode = 0;
#endif
      //
      // Disable the DRAM write leveling mode and leave the internal write timing enabled.
      //
      DisableDramWlDdr5 (Host, Socket, Dimm, Rank);
      //
      // Clear mpr_train_ddr_on bit to re-enable ACT and PRE.
      //
      ChBitmask = GetChBitmask (Host, Socket, Dimm, Rank);
      for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
        if (!((1 << Ch) & ChBitmask)) {
          continue;
        }
        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if (Rank >= (*DimmNvList) [Dimm].numRanks) {
          continue;
        }
        SetMprModeFilterActPre (Host, Socket, Ch, 0);
      } // Ch loop
      if (IsBrsPresent (Host, Socket)) {
        SetTrainingMode (Host, Socket, Dimm, Rank, WR_LEVELING_CLEAR_MODE);
      }
    } // Rank loop
  } // Dimm loop

  SetTrainingModeDdr5 (Socket, TmWriteLeveingExit);
#ifdef DEBUG_CODE_BLOCK
  //
  // Display all the training reuslts before final calculation.
  //
  RcDebugPrint (SDBG_MAX,
    "Display Training Results:\n");
  DisplayInternalCycleAlingmentDdr5 (Host, Socket, Ch, Dimm, Rank, DISPLAY_FROM_CACHE, InternalCycleAlignment, TxDqsDelay);
  DisplayCwlAdjDdr5 (Host, Socket, DISPLAY_FROM_CACHE, &CwlAdj);
  DisplayTxDqsDdr5 (Host, Socket, DISPLAY_FROM_REGISTER, TxDqsDelay);
#endif // DEBUG_CODE_BLOCK
  //
  // Evaluate the data
  //
  NomalizeCwlAdjTxDqsDdr5 (Host, &CwlAdj, &TxDqsDelayValue, Done, WLChipCleanUpStruct);
  //
  TxDqsWA (Socket);
  // Exit sense amp training mode
  //
  SetTrainingModeLatchResult (Host, Socket, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);
  ClearDdrioWlModeDdr5 (Host, Socket);
  RestoreWritePreambleWriteLeveling (Host, Socket);
  RcFreePool (TrainRes);

  return SUCCESS;
} //ExternalInternalWriteLevelingDdr5CoarseFine


/*++

This function execute the normalization of TxDQS Delay and CWL_ADJ for DDR5 (and DDRT2).

@param Host                  - Pointer to sysHost
@param CwlAdjInput           - CWL_ADJ input
@param TxDqsDelayValueInput  - TxDqs Delay Input
@param Done                  - Done indication from centering flow
@param WLChipCleanUpStruct   - WL Delays
@param DdrTech               - DDR Technology 0 DDR4 1 DDRT

@retval NA

--*/
VOID
NomalizeCwlAdjTxDqsTechDdr5 (
  IN PSYSHOST                Host,
  IN INT16                   (*CwlAdj) [MAX_CH][SUB_CH][MAX_RANK_CH],
  IN INT16                   (*TxDqsDelayValue) [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8                   Done,
  IN CHIP_WL_CLEANUP_STRUCT  WLChipCleanUpStruct,
  IN UINT8                   DdrTech
  )
{
  UINT8 Socket;
  UINT8 Ch;
  UINT8 Dimm;
  UINT8 Rank;
  UINT8 Strobe;
  INT16 Delay = 0;
  INT16 CwlAdjTarget = 0;
  UINT16 PiSumCwlAdjTxDqs[MAX_RANK_CH][MAX_STROBE_DDR5 / 2];
  UINT16 TxDqsDelayResult[MAX_RANK_CH][MAX_STROBE_DDR5 / 2];
  INT16 MinPiSumCwlAdjTxDqs = 0x7FFF;
  UINT8 SubChannel = 0;
  UINT8 ChannelLocal = 0;
  INT16  MinCwlAdj[MAX_CH][SUB_CH];
  UINT8 MaxStrobe;
  UINT32 StrobeFail[MAX_CH];
  UINT8                 SubChMSVx4 = GetSubChMaxStrobeValid (Host);
  UINT8                 SubChMSVx8 = GetSubChMaxStrobeValid (Host) / 2;

  struct channelNvram (*ChannelNvList) [MAX_CH];
  struct ddrRank (*RankList) [MAX_RANK_DIMM];
  struct dimmNvram (*DimmNvList) [MAX_DIMM];
  INT16 CRAddDelay[MAX_CH][SUB_CH];
  INT16 CwlAdjTargetValue[SUB_CH];
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ZeroMem ((UINT8 *) StrobeFail, sizeof (StrobeFail));
  ZeroMem ((UINT8 *) MinCwlAdj,  sizeof (MinCwlAdj));
  ZeroMem ((UINT8 *) CRAddDelay, sizeof (CRAddDelay));
  ZeroMem ((UINT8 *) PiSumCwlAdjTxDqs, sizeof (PiSumCwlAdjTxDqs));
  ZeroMem ((UINT8 *) TxDqsDelayResult, sizeof (TxDqsDelayResult));
  ZeroMem ((UINT8 *) CwlAdjTargetValue, sizeof (CwlAdjTargetValue));
  //
  // Return if this Socket is disabled
  //
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);
#ifdef DEBUG_CODE_BLOCK
  RcDebugPrint (SDBG_MAX,
    "\nStart CWL_ADJ and TxDqs Delay normalization:\n");
  RcDebugPrint (SDBG_MEM_TRAIN,
    "                  ");
  for (Strobe = 0; Strobe < SubChMSVx4; Strobe++) {
    //
    // Skip if this is an ECC Strobe when ECC is disabled
    //
    if (IsStrobeNotValidDdr5 (Host, Strobe)) {
      continue;
    }
    RcDebugPrint (SDBG_MEM_TRAIN,
      "  %2d ", Strobe);
  } // Strobe loop
  RcDebugPrint (SDBG_MEM_TRAIN,
    "\n");
#endif //DEBUG_CODE_BLOCK
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      ChannelLocal = Ch;
      GetChannelMapping (Ch, SubChannel, &ChannelLocal);
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      //
      // Init MinPiSumCwlAdjTxDqs for each sub channel.
      //
      MinPiSumCwlAdjTxDqs = 0x7FFF;
      //
      // Find the minimum sum of CWL_ADJ and TxDQS Delay.
      //
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (!DoesDdrTechMatchThisDimm (DdrTech, (*ChannelNvList) [Ch].dimmList[Dimm].DcpmmPresent)) {
          continue; // Skip DDR4/DDRT when DdrTech does not match DIMMs
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        if (IsX4Dimm (Socket, Ch, Dimm)) {
          MaxStrobe = SubChMSVx4;
        } else {
          MaxStrobe = SubChMSVx8;
        }

        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }
            GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_READ_ONLY, &Delay);
            PiSumCwlAdjTxDqs[ (*RankList) [Rank].rankIndex][Strobe] = (*CwlAdj) [Ch][SubChannel][ (*RankList) [Rank].rankIndex] *128 + Delay;
            if (PiSumCwlAdjTxDqs[ (*RankList) [Rank].rankIndex][Strobe] < MinPiSumCwlAdjTxDqs) {
              MinPiSumCwlAdjTxDqs = PiSumCwlAdjTxDqs[ (*RankList) [Rank].rankIndex][Strobe];
            }
          } //Strobe loop.
        }//Rank loop
      }//Dimm loop
#ifdef DEBUG_CODE_BLOCK
      //
      // Display the sum.
      //
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (!DoesDdrTechMatchThisDimm (DdrTech, (*ChannelNvList) [Ch].dimmList[Dimm].DcpmmPresent)) {
          continue; // Skip DDR4/DDRT when DdrTech does not match DIMMs
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        if (IsX4Dimm (Socket, Ch, Dimm)) {
          MaxStrobe = SubChMSVx4;
        } else {
          MaxStrobe = SubChMSVx8;
        }

        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
            "");
          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }
            RcDebugPrint (SDBG_MEM_TRAIN,
              "%5d", PiSumCwlAdjTxDqs[ (*RankList) [Rank].rankIndex][Strobe]);
          } //Strobe loop.
          RcDebugPrint (SDBG_MEM_TRAIN,
            "\n");
        }//Rank loop
      }//Dimm loop
#endif //DEBUG_CODE_BLOCK
      //
      // Calculate the CWL_ADJ target.
      //

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        if (!DoesDdrTechMatchThisDimm (DdrTech, (*ChannelNvList) [Ch].dimmList[Dimm].DcpmmPresent)) {
          continue; // Skip DDR4/DDRT when DdrTech does not match DIMMs
        }
        //
        // Program CWL_ADJ
        //
        //
        // Calculate the CWL_ADJ target.
        //
        if (MinPiSumCwlAdjTxDqs < TX_DQ_START_OFFSET_DDR5) {
          CwlAdjTarget = -1;
        } else {
          CwlAdjTarget = (MinPiSumCwlAdjTxDqs - TX_DQ_START_OFFSET_DDR5) / 128;
        }

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
          "The minimum CWL_ADJ + TxDQS is %3d, CWL_ADJ target is %3d\n", MinPiSumCwlAdjTxDqs, CwlAdjTarget);
        RcDebugPrint (SDBG_MEM_TRAIN,
          "\n");
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
          "The minimum CWL_ADJ + TxDQS is %3d, CWL_ADJ target is %3d\n", MinPiSumCwlAdjTxDqs, CwlAdjTarget);
        RcDebugPrint (SDBG_MEM_TRAIN,
          "\n");
        UpdateChipMCDelayDdr5 (Host, Socket, Ch, Dimm, SubChannel, ChannelNvList, CwlAdjTarget, &WLChipCleanUpStruct);
        GetCwlAdj (Host, Socket, Ch, Dimm, &CwlAdjTarget);
        CwlAdjTargetValue[SubChannel] = CwlAdjTarget;
      }

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList) [Dimm].dimmPresent == 0) {
          continue;
        }
        //
        // Program CWL_ADJ
        //
        UpdateChipMCDelayDdr5 (Host, Socket, Ch, Dimm, SubChannel, ChannelNvList, CwlAdjTarget, &WLChipCleanUpStruct);
      }

      //
      // Calculate the TxDqs Delay and Program it.
      //
      ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 0);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if (!DoesDdrTechMatchThisDimm (DdrTech, (*ChannelNvList) [Ch].dimmList[Dimm].DcpmmPresent)) {
          continue; // Skip DDR4/DDRT when DdrTech does not match DIMMs
        }
        RankList = GetRankNvList (Host, Socket, Ch, Dimm);
        if (IsX4Dimm (Socket, Ch, Dimm)) {
          MaxStrobe = SubChMSVx4;
        } else {
          MaxStrobe = SubChMSVx8;
        }

        for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
          if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
            //
            // Skip if this is an ECC Strobe when ECC is disabled
            //
            if (IsStrobeNotValidDdr5 (Host, Strobe)) {
              continue;
            }
            TxDqsDelayResult[ (*RankList) [Rank].rankIndex][Strobe] = PiSumCwlAdjTxDqs[ (*RankList) [Rank].rankIndex][Strobe] - CwlAdjTarget * 128;
            //DDR5_TODO. To check and confirm this value.
            if (0) {
            //if (TxDqsDelayResult[ (*RankList) [Rank].rankIndex][Strobe] > 447) {
              //fail
              RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, Strobe, NO_BIT,
                "TxDQS Delay is %3d, more than (or equal to) 447, failed\n", TxDqsDelayResult[ (*RankList) [Rank].rankIndex][Strobe]);
              EwlOutputType2 (WARN_WR_LEVEL, WARN_WR_FLYBY_UNCORR, Socket, Ch, Dimm, Rank, EwlSeverityWarning, Strobe, NO_BIT, WrLvlDelay, DdrLevel, 0xFF);
              //
              // This is a fatal error condition so map out the entire Channel.
              //
              DisableChannelSw (Host, Socket, ChannelLocal);
              break;
            } else {
              GetSetDataGroup (Host, Socket, ChannelLocal, SubChannel, Dimm, Rank, Strobe, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&TxDqsDelayResult[ (*RankList) [Rank].rankIndex][Strobe]);
              if (!(*DimmNvList) [Dimm].x4Present) {
                GetSetDataGroup (Host, Socket, ChannelLocal,SubChannel,  Dimm, Rank, Strobe + SubChMSVx8, 0, DdrLevel, TxDqsDelay, GSM_UPDATE_CACHE | GSM_FORCE_WRITE, (INT16 *)&TxDqsDelayResult[ (*RankList) [Rank].rankIndex][Strobe]);
              }
            }
          } //Strobe loop.
        }//Rank loop
      } //Dimm loop
      ProgramDriveDqsLowDdr5  (Host, Socket, Ch, SubChannel, 1);
    } //subchannel loop
    EqualizeCwlPerCh (Host, Socket, Ch, &WLChipCleanUpStruct, CwlAdjTargetValue);
  } //Ch loop
} //NomalizeCwlAdjTxDqsTechDdr5

/*++

This function execute the normalization of TxDQS Delay and CWL_ADJ for DDR5 (and DDRT2).

@param Host                  - Pointer to sysHost
@param CwlAdjInput           - CWL_ADJ input
@param TxDqsDelayValueInput  - TxDqs Delay Input
@param Done                  - Done indication from centering flow
@param WLChipCleanUpStruct   - WL Delays

@retval NA

--*/
VOID
NomalizeCwlAdjTxDqsDdr5 (
  IN PSYSHOST                Host,
  IN INT16                   (*CwlAdjInput) [MAX_CH][SUB_CH][MAX_RANK_CH],
  IN INT16                   (*TxDqsDelayValueInput) [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  IN UINT8                   Done,
  IN CHIP_WL_CLEANUP_STRUCT  WLChipCleanUpStruct
  )
{
  NomalizeCwlAdjTxDqsTechDdr5 (Host, CwlAdjInput, TxDqsDelayValueInput, Done, WLChipCleanUpStruct, CWLC_DDR5);
}

/**

Add training Delay

@param[in] Host           - Pointer to sysHost
@param[in] CteDelay       - CTE Delay
@param[in] PhysicalDelay  - Delay in real platform

@retval N/A

**/
VOID
EFIAPI
TrainingDelay (
  IN PSYSHOST Host,
  IN UINT32   CteDelay,
  IN UINT32   PhysicalDelay
  )
{
  FixedDelayMicroSecond (PhysicalDelay);
}

/*++

Changes Rtt Nom to Rtt Wr and disable DDR5 DRAM write leveling mode.

@param Host    - Pointer to sysHost
@param Socket  - Socket number
@param Dimm    - DIMM number
@param Rank    - Rank number

@retval N/A

--*/
VOID
DisableDramWlDdr5 (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Dimm,
  IN UINT8    Rank)
{
  UINT8 Ch;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];
  struct rankDevice (*RankStruct) [MAX_RANK_DIMM];
  UINT8                         MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  //
  // Restore Rtt Nom setting and exit WL mode
  //
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

    //
    // Skip if no Rank
    //
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    } {
      DisableExternalWriteLeveling (Host, Socket, Ch, Dimm, Rank);
    }
  } // Ch loop
} // DisableDramWlDdr5

/**

  Enbale DRAM External Write Timing.

  @param [IN] Host    - Pointer to sysHost
  @param [IN] Socket  - Socket number
  @param [IN] Ch      - Channel number
  @param [IN] Dimm    - DIMM number
  @param [IN] Rank    - Rank number

  retval : none
**/
VOID
ProgramExternalWriteLeveling (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  struct dimmNvram (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  if ((*DimmNvList)[Dimm].DcpmmPresent == 0 ) {
    ConfigureDramForWriteLeveling (Host, Socket, Ch, Dimm, Rank, EXTERNAL_WRITE_LEVELING_MODE);
  } else {
    ProgramWlMr2Ddrt2 (Host, Socket, Ch, Dimm, Rank, EXTERNAL_WRITE_LEVELING_MODE);
  }
}

/**

Disable DRAM External Write Timing.

@param [IN] Host    - Pointer to sysHost
@param [IN] Socket  - Socket number
@param [IN] Ch      - Channel number
@param [IN] Dimm    - DIMM number
@param [IN] Rank    - Rank number

retval : none
**/
VOID
DisableExternalWriteLeveling (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank
  )
{
  struct dimmNvram(*DimmNvList)[MAX_DIMM];
#ifdef LRDIMM_SUPPORT
  DDR5_DATA_BUFFER_RW83_STRUCT  DataBufferRw83;
#endif // LRDIMM_SUPPORT

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
#ifdef LRDIMM_SUPPORT
    if (IsLrdimmPresent (Socket, Ch, Dimm) == TRUE) {
      DataBufferRw83.Data = 0;
      DataBufferRw83.Bits.training_mode = 0;
      DbControlWordWrite (Host, Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_DATA_BUFFER_RW83_REG, DataBufferRw83.Data);
    } else
#endif //LRDIMM_SUPPORT
    {
      ConfigureDramForWriteLeveling (Host, Socket, Ch, Dimm, Rank, DISABLE_WRITE_LEVELING_MODE);
    }
}
