/** @file
  BaseMemoryCoreInternal.h

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

#ifndef _BASE_MEMORY_CORE_INTERNAL_H_
#define _BASE_MEMORY_CORE_INTERNAL_H_

#include <ReferenceCodeDataTypes.h>
#include <ReferenceCodeFatalErrors.h>
#include <Memory/MemoryCheckpointCodes.h>

#define PUSH_EXIT_FRAME(host) \
  {                           \
    UINT32                    OldJumpBuffer;                   \
    UINTN                     JumpFlagStatus;                  \
    BASE_LIBRARY_JUMP_BUFFER  JumpBuffer;                      \
    OldJumpBuffer = host->var.common.JumpBuffer;               \
    host->var.common.JumpBuffer = (UINT32)(UINTN) &JumpBuffer; \
    JumpFlagStatus = SetJump (&JumpBuffer);                    \
    if (JumpFlagStatus == 0) {                                 \


#define POP_EXIT_FRAME(host, status)                           \
    }                                                          \
    status = (UINT32) JumpFlagStatus;                          \
    host->var.common.JumpBuffer = OldJumpBuffer;               \
  }

#pragma pack(push, 1)
//
// Associate QCLK ratio to DDR speed strings
//
typedef struct {
  UINT8     QclkRatio;
  CHAR8     *SpeedString;
} QCLK_RATIO_TO_SPEED;

//
// Associate DDR speed strings with encoded frequency and TCK min values
//
typedef struct {
  UINT8     DdrFreq; // Encoded DDR frequency (DDR_2400, DDR_2666, etc)
  UINT16    TckMin;  // tCKmin in pico seconds
  CHAR8     *SpeedString;
} DDR_SPEED_STRINGS;

//
// Associate raw card numbers to a string
//
typedef struct {
  UINT8     RawCardNumber;
  CHAR8     *String;
} RAW_CARD_STRINGS;

#pragma pack(pop)

/**

  Returns a bit mask of DIMMs present.

  @param Host      - Pointer to sysHost, the system Host (root) structure
  @param socket    - Processor socket to check
  @param dimmMask  - Pointer to results

  @retval None

**/
VOID
GetDimmPresent (
  PSYSHOST Host,                // Pointer to the system Host (root) structure
  UINT8     socket,             // Processor socket to check
  UINT8     *dimmMask[MAX_CH]   // Pointer to results
  );

/**

  Returns a bit mask of logical ranks present.

  @param Host      - Pointer to sysHost, the system Host (root) structure
  @param socket    - Processor socket to check
  @param rankMask  - Pointer to results

  @retval None

**/
VOID
GetRankPresent (
  PSYSHOST Host,                  // Pointer to the system Host (root) structure
  UINT8     socket,               // Processor socket to check
  UINT8     *rankMask[MAX_CH]     // Pointer to results
  );

/**

  Initialize DDR4 voltage

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket number

  @retval None

**/
VOID
InitDDRVoltageDDR4 (
  PSYSHOST  Host,         // Pointer to the system Host (root) structure
  UINT8     socket        // socket number
  );

#ifdef LRDIMM_SUPPORT
/**

  VRef Centering for LRDIMM (DDR4)

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - Processor socket to check
  @param group            - Parameter to be margined
  @param level            - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param perRankCentering - 0 = false, 1 = true (if false, PDA training is done)

  @retval SUCCESS

**/
UINT32
LRDIMMAdvCentering (
  PSYSHOST  Host,            // Pointer to Host structure (outside scope of this spec)
  UINT8     socket,          // Processor socket to check
  MRC_GT    group,           // Parameter to be margined
  MRC_LT    level,           // DDR or LRDIMM Host side, LRDIMM Backside, etc...
  UINT8     perRankCentering // 0 = false, 1 = true (if false, PDA training is done)
  );
#endif // LRDIMM_SUPPORT

/**

  Per Bit Deskew for the requested group

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param group   - Parameter to center \n
                     RxDqsDelay   0: Center Rx DqDqs \n
                     TxDqDelay    1: Center Tx DqDqs

  @retval None

**/
VOID
PerBitDeskew (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  MRC_GT      group                             // Parameter to center
                                                //   RxDqsDelay   0: Center Rx DqDqs
                                                //   TxDqDelay    1: Center Tx DqDqs
);


/**

  Set the DDR clock frequency

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id

  @retval SUCCESS - Clock is set properly
  @retval FAILURE - A reset is required to set the clock

**/
UINT32
SetDDRFreq (
  PSYSHOST Host,      // Pointer to the system Host (root) structure
  UINT8    socket     // Socket Id
  );

/**

  @param Host      - Pointer to sysHost, the system Host (root) structure
  @param socket    - CPU Socket Node number (Socket ID)
  @param ch        - Channel number (0-based)
  @param dimm      - Current dimm
  @param rank      - Rank number (0-based)
  @param scope     - Margin granularity
  @param ranksDone - Array of bits showing if rans are done. \n
                       If the bit is set the rank is done

  @retval None

**/
VOID
EvalResSetRanksDone (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Rank number (0-based)
  UINT8       scope,                            // Margin granularity
  UINT32      *ranksDone                        // Array of bits showing if rans are done.
                                                //   If the bit is set the rank is done
  );

/**

  Function to offset the final training results before entering the OS that would facilitate customer debug.

  @param Host            - Pointer to sysHost, the system Host (root) structure
  @param socket          - Processor socket to check
  @param ch              - Current Channel
  @param dimm            - Dimm number (0-based)
  @param rank            - Rank number (0-based)
  @param offset          - Pointer to offset
  @param level           - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group           - Test group

  @retval None

**/
VOID
TrainingResultOffsetFunction (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  INT16     offset,
  MRC_LT    level,
  MRC_GT    group
  );

/**

  Display Right Edge and Left Edge margin results

  @param Host          - Pointer to sysHost, the system Host (root) structure
  @param socket        - current socket
  @param current_rank  - Rank number (0-based)
  @param resultsBit    - Per bit margin results
  @param group         - Command group

  @retval None

**/
VOID
DisplayREandLEMargins (
  PSYSHOST          Host,         // Pointer to sysHost, the system Host (root) structure
  UINT8             socket,       // current socket
  UINT8               ch,
  UINT8               dimm,
  UINT8               rank,
  struct bitMarginCh  *resultsBit,
  MRC_GT            group         // Command group
  );

/**

  Finds command margins for the given signal

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id
  @param dimm    - DIMM number
  @param rank    - rank number
  @param signal  - Signal
  @param le      - Left edge
  @param re      - Right edge

  @retval None

**/
VOID
GetCmdMarginsSweep (
  PSYSHOST Host,        // Pointer to the system Host (root) structure
  UINT8    socket,      // Socket Id
  UINT8     dimm,       // DIMM number
  UINT8     rank,       // rank number
  GSM_CSN   signal,     // Signal
  INT16     *le,        // Left edge
  INT16     *re         // Right edge
  );

/**

  Finds command margins for the given signal

  @param[in]  Host         Pointer to sysHost, the system host (root) structure
  @param[in]  Socket       Socket Id
  @param[in]  Dimm         DIMM number
  @param[in]  Rank         Rank number
  @param[in]  Signal       Signal
  @param[out] LeftEdge     Left edge
  @param[out] RightEdge    Right edge

  @retval None

**/
VOID
GetCmdMarginsSweepBackside (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  GSM_CSN   Signal,
  OUT INT16     *LeftEdge,
  OUT INT16     *RightEdge
  );

/**

  his function gets data from slave processors

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
GetData (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  This function gets data from slave processors

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
GetNvramData (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );



/**

  Get the status variable from each socket

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval 0 - no reset
  @retval 2 - Power Good
  @retval 3 - Warm reset

**/
UINT32
GetStatus (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  This function sends data to slave processors

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
SendNvramData (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

This function sends data to slave processors

@param Host - Pointer to sysHost, the system Host (root) structure

@retval SUCCESS

**/
UINT32
SendVarData(
  PSYSHOST                  Host
  );

/**

This function gets data from master

@param Host - Pointer to sysHost, the system Host (root) structure

@retval SUCCESS

**/
UINT32
GetVarData(
  PSYSHOST                  Host
  );
/**

  This function sends data to slave processors


  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
SendSlaveData (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**
  This function sends bootmode to slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
SendSocketbootmode (
  PSYSHOST  Host
  );

/**
  This function gets bootmode to slave processors

  @param Host:         Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
GetSocketbootmode (
  PSYSHOST  Host
  );

/**
This function sends bootmode to slave processors

@param Host:         Pointer to sysHost

@retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
Sendbootmode(
  PSYSHOST  Host
  );

/**
This function sends bootmode to slave processors

@param Host:         Pointer to sysHost

@retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
Getbootmode(
  PSYSHOST  Host
  );

/**

  Send mem.var, mem.nvram common and mem.nvram data from each socket to master to sync up

  @param Host - Pointer to sysHost, the system Host (root) structure

**/
UINT32
SendSlaveSsaData (
    PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
    );

/**

  Get mem.var, mem.nvram common and mem.nvram data from each slave socket to sync up

  @param Host - Pointer to sysHost, the system Host (root) structure

**/
UINT32
GetSlaveSsaData (
    PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
    );

/**

  Send a variable to each socket to sync up

  @param Host - Pointer to sysHost, the system Host (root) structure

**/
UINT32
SendSsaData (
    PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
    );

/**

  Get a variable from master socket to sync up

  @param Host - Pointer to sysHost, the system Host (root) structure

**/
UINT32
GetSsaData (
  PSYSHOST  Host  // Pointer to Host structure (outside scope of this spec)
  );

/**

  Check if any processor has been changed across boots

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval TRUE - Processor has been changed
  @retval FALSE - Processor has not been changed

**/
UINT32
ProcPpinChangedCheck (
  PSYSHOST  Host,    // Pointer to the system Host (root) structure
  UINT8     socket
  );

/**
This function accesses the IO group setting as directed by the mode parameter. The access
is based on the MemSs, Ch, Dimm, Rank, Strobe and Bit parameters.

  @param Host:        Pointer to SYSHOST
  @param Socket:      Socket number
  @param MemSs:       MemSS number within the socket (0-based)
  @param Channel:     Channel number within the MemSS (0-based)
  @param Dimm:        DIMM number within a channel (0-based, farthest slot == 0)
  @param Rank:        Logical rank number within a DIMM (0-based)
  @param Strobe:      Dqs data group within the rank
  @param Bit:         Bit index within the data group
  @param FreqIndex:   Index to designate which operating frequency, for multi-freq support
  @param Level:       IO level to access
  @param Group:       IO group to access
  @param Mode:        Bit-field of different modes
  @param *Value:      Pointer to value or offset based on mode

@retval MRC_STATUS
**/
MRC_STATUS
GetSetIoGroup (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     MemSs,
  IN     UINT8     Channel,
  IN     UINT8     Dimm,
  IN     UINT8     Rank,
  IN     UINT8     Strobe,
  IN     UINT8     Bit,
  IN     UINT8     FreqIndex,
  IN     MRC_LT    Level,
  IN     MRC_GT    Group,
  IN     UINT8     Mode,
  IN OUT INT16     *Value
  );

/**
  Function to shift backside Cmd, Ctl, Clk by +/- 31 PI ticks

  @param[in] Host       Pointer to SYSHOST
  @param[in] Socket     Socket number
  @param[in] Direction  +/- direction to move timings

  @retval SUCCESS
**/
UINT32
EarlyBacksideShift (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  INT8      Direction
  );

/**

  This function implements a generic 1-D parameter optimization

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Group         - Array of signal groups to margin
  @param[in]      NumGroups     - Number of signal groups to margin
  @param[in]      Param         - Pointer to list of parameter structures
  @param[in]      NumParams     - Number of parameters in Param

  @retVal MRC_STATUS_SUCCESS

**/
MRC_STATUS
TrainDDROptParam (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      MRC_GT            Group[MAX_GROUPS],
  IN      UINT8             NumGroups,
  IN      POWER_TRAIN_PARAM *Param,
  IN      UINT8             NumParams
  );

/**

  Find the optimal trade off between margins and power

  @param[in] Host            - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket          - Socket Id
  @param[in] Channel         - Channel Id
  @param[in] Dimm            - Dimm Id
  @param[in] Rank            - Rank Id
  @param[in] Strobe          - Strobe Id
  @param[in] Bit          - Bit Id
  @param[in] AverageMargins  - Average margins
  @param[in] NumSettings     - Number of settings
  @param[in] NumGroups       - Number of groups
  @param[in] ParamType       - Parameter type (PerNibblePerRankPerCh, PerBytePerRankPerCh, etc)

  @retval Index of the optimal setting

**/
INT16
FindOptimalTradeOff (
  IN    PSYSHOST            Host,
  IN    UINT8               Socket,
  IN    UINT8               Channel,
  IN    UINT8               Dimm,
  IN    UINT8               Rank,
  IN    UINT8               Strobe,
  IN    UINT8               Bit,
  IN    PER_GROUP_SETTINGS  *AverageMargins,
  IN    UINT8               NumSettings,
  IN    UINT8               NumGroups,
  IN    MPT_PT              ParamType
  );

/**

  Inphi A1 WA

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - Current dimm

  @retval None

**/
VOID
InphiPIWA (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  );

/**
  Prints a heading for per-bit results displays.

  @param [in]     Host        - Pointer to SysHost
  @param [in]     Socket      - Socket number
  @param [in]     Bit         - Specific bit number being displayed
  @param [in]     MaxBits     - Maximum number of bits to display

  @retval N/A
**/
VOID
PrintPerBitHeading (
  IN     PSYSHOST      Host,
  IN     UINT8         Socket,
  IN     UINT8         Bit,
  IN     UINT8         MaxBits
  );


/**

  Displays information about the DIMMs present

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param dimm    - Current dimm

  @retval None

**/
VOID
CheckMappedOut (
  PSYSHOST  Host,
  UINT8 socket,
  UINT8 dimm
  );

/**

  Enables or Disables the MPR pattern in all DIMMs/Channels/Ranks

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param mprMode - Either 0 for Disable or MR3_MPR to Enable

  @retval None

**/
VOID
SetAllRankMPR (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 mprMode
  );

/**

  Enables or Disables the MPR pattern in all DIMMs/Channels/Ranks

  @param Host     - Pointer to sysHost, the system Host (root) structure struct
  @param socket   - Socket Id
  @param ch       - Channel number (0-based)
  @param dimm     - Current dimm
  @param rank     - Rank number (0-based)
  @param mprMode  - Either 0 for Disable or MR3_MPR to Enable
  @param pattern  - Pattern to use for testing

  @retval None

**/
VOID
SetRankMPRPattern (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  UINT16 mprMode,
  UINT16 pattern
  );

/**

  Enables or Disables the MPR pattern in all DIMMs/Channels/Ranks

  @param Host      - Pointer to sysHost, the system Host (root) structure struct
  @param socket    - Socket Id
  @param ch        - Channel number (0-based)
  @param dimm      - Current dimm
  @param rank      - Rank number (0-based)
  @param mprMode   - Either 0 for Disable or MR3_MPR to Enable
  @param pattern   - Pattern to use for testing

  @retval None

**/
VOID
SetRankMPRPatternDA (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  UINT16 mprMode,
  UINT16 pattern
  );

/**

 For each data module, a 128 bit vector of zeros and ones holding the tests results is
 evaluated which is a concatenation of the two reads from the feedback CRs.
 BIOS locates the middle of the longest consecutive stream of ones (when the vector is evaluated as cyclic,
 this location in the vector indicates the PI setting and logic delay for that data module).

  @param Host              - Pointer to sysHost, the system Host (root) structure struct
  @param socket            - Current Socket
  @param ch                - Channel number
  @param dimm              - Dimm number on ch
  @param rank              - Rank on dimm
  @param strobe            - Strobe number
  @param risingEdge        - Pointer to save the rising edge
  @param centerPoint       - Pointer to save the center point
  @param fallingEdge       - Pointer to save the falling edge
  @param pulseWidth        - Pointer to save the pulse width
  @param step              - The PI Step to work on:
                               FPT_PI_READ_TYPE FPT_PI_WRITE_TYPE
                               FPT_PI_LRDIMM_READ_TYPE
                               FPT_PI_LRDIMM_WRITE_TYPE
                               FPT_PI_LRDIMM_RD_MRD_TYPE
                               FPT_PI_LRDIMM_WR_MRD_TYPE

  @retval None

**/
VOID
EvaluatePiSettingResults (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  UINT8 strobe,
  UINT16 risingEdge[MAX_CH][MAX_STROBE],
  UINT16 centerPoint[MAX_CH][MAX_STROBE],
  UINT16 fallingEdge[MAX_CH][MAX_STROBE],
  UINT16 pulseWidth[MAX_CH][MAX_STROBE],
  UINT8 step
  );

/**

  This function print the sample array

  @param Host      - Pointer to sysHost, the system Host (root) structure struct
  @param socket    - Socket number
  @param ch        - Channel number
  @param dimm      - DIMM number
  @param ReadMode  - 5 = LRDIMM Write MWD Delay
                     4 = LRDIMM Read DQ DQS
                     3 = LRDIMM Read Recieve Enable Phase
                     2 = LRDIMM Write DWL
                     1 = Read DqDqs
                     0 = Write DqDqs

  @retval None

**/
VOID
PrintSampleArray (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 ReadMode
  );

/**

  Prints the edges found for LRDIMM

  @param Host          - Pointer to sysHost, the system Host (root) structure struct
  @param socket        - Current Socket
  @param ch            - Channel number
  @param pulseWidth    - Pointer to save the pulse width
  @param fallingEdge   - Pointer to save the falling edge
  @param risingEdge    - Pointer to save the rising edge
  @param centerPoint   - Pointer to save the center point
  @param maxStrobe     - Maximum number of strobes

  @retval None

**/
VOID
DisplayLrdimmMrdEdges (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT16 pulseWidth[MAX_STROBE],
  UINT16 fallingEdge[MAX_STROBE],
  UINT16 risingEdge[MAX_STROBE],
  UINT16 centerPoint[MAX_STROBE],
  UINT8 maxStrobe
  );

/**

  Changes Rtt Nom to Rtt Wr

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket number
  @param dimm    - DIMM number
  @param rank    - Rank number

  @retval None

**/
VOID
OverrideRttNom (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 dimm,
  UINT8 rank
  );

/**

  Restore Rtt Nom

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket number
  @param dimm    - DIMM number
  @param rank    - Rank number

  @retval None

**/
VOID
RestoreRttNom (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 dimm,
  UINT8 rank
  );

/**

  Displays information about the device being refered to

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket number

  @retval None

**/
VOID
DisplayDDRFreq (
  PSYSHOST Host,
  UINT8 socket
  );

/**

  Displays the bitwise error status

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - Current dimm
  @param rank    - Rank number (0-based)
  @param bwSerr  - BitWise error status
  @param margin  - Margin

  @retval None

**/
VOID
DisplayBwSerr (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  UINT32 bwSerr[3],
  INT16 margin
  );

/**

  Displays the LRDIMM backside training results

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param group   - MRC_GT group

  @retval None

**/
VOID
DisplayDdr4LrdimmTrainingResults (
  PSYSHOST Host,
  UINT8 socket,
  MRC_GT group
  );

/**

  Calculates margins at multiple Vref points

  @param Host         - Pointer to sysHost, the system Host (root) structure struct
  @param socket       - Processor socket to check
  @param level        - DDR or LRDIMM Host side, LRDIMM Backside, etc...
  @param group        - Parameter to be margined
                         RxDqsDelay    : Margin Rx DqDqs
                         TxDqDelay     : Margin Tx DqDqs
                         RxVref         : Margin Rx Vref
                         TxVref         : Margin Tx Vref
                         RecEnDelay    : Margin Receive Enable
                         WrLvlDelay    : Margin Write Level
                         PcodeMailboxStatus = 0;
                         CmdGrp0        : Margin CMD group 0
                         CmdGrp1        : Margin CMD group 1
                         CmdGrp2        : Margin CMD group 2
                         CmdAll         : Margin all CMD groups
  @param mode         - Test mode to use
                         MODE_XTALK            BIT0: Enable crosstalk (placeholder)
                         MODE_VIC_AGG          BIT1: Enable victim/aggressor
                         MODE_START_ZERO       BIT2: Start at margin 0
                         MODE_POS_ONLY         BIT3: Margin only the positive side
                         MODE_NEG_ONLY         BIT4: Margin only the negative side
                         MODE_DATA_MPR         BIT5: Enable MPR Data pattern
                         MODE_DATA_FIXED       BIT6: Enable Fixed Data pattern
                         MODE_DATA_LFSR        BIT7: Enable Data LFSR
                         MODE_ADDR_LFSR        BIT8: Enable Address LFSR
                         MODE_ADDR_CMD0_SETUP  BIT9
                         MODE_CHECK_PARITY     BIT10: Enable parity checking
                         MODE_DESELECT         BIT11: Enable deselect patterns
                         MODE_VA_DESELECT      BIT12: Enable Victim - Aggressor deselect patterns
  @param scope        - Margin granularity
                         SCOPE_SOCKET      0: Margin per processor socket
                         SCOPE_CH        1: Margin per channel
                         SCOPE_DIMM      2: Margin per DIMM
                         SCOPE_RANK      3: Margin per rank
                         SCOPE_STROBE    4: Margin per strobe group
                         SCOPE_BIT       5: Margin per bit
  @param filter        - Mask of bits to exclude from testing
  @param results       - Pointer to the structure to store the margin results. The structure type varies based on scope
  @param patternLength - RankMarginTest Pattern length
  @param chMask        - Bit Mask of channels to not be used
  @param rankMask      - Bit Mask of ranks to not be used
  @param stepSize      - Step size
  @param numPoints     - Number of points to test

  @retval None

**/
VOID
GetMultiVref (
  PSYSHOST Host,
  UINT8 socket,
  MRC_LT level,
  MRC_GT group,
  UINT16 mode,
  UINT8 scope,
  struct bitMask *filter,
  VOID *results,
  UINT32 patternLength,
  UINT8 chMask,
  UINT8 rankMask,
  UINT8 stepSize,
  UINT8 numPoints
  );

/**

  Get the Serial Number of the requested module

  @param Host       - Pointer to sysHost, the system Host (root) structure
  @param socket     - Socket ID
  @param ch         - Channel on socket
  @param dimm       - DIMM on channel
  @param SPDReg     - Pointer to the SPD Module Serial Number Table

  @retval None

**/
VOID
GetModuleSerialNumber (
  IN     PSYSHOST  Host,
  IN     UINT8     socket,
  IN     UINT8     ch,
  IN     UINT8     dimm,
  IN OUT UINT8     SPDModSN[SPD_MODULE_SERIAL]
  );

/*

  Determines if the Dimm is a DDR4 Jedec specification defined NVDIMM-N type


  @param[in]                Socket              Socket under test (Based-0)
  @param[in]                Channel             Channel under test (Based-0)
  @param[in]                Dimm                Dimm under test (Based-0)

  @retval                   TRUE                Device is an NVDIMM-N type
  @retval                   FALSE               Any other Dimm type

*/
BOOLEAN
EFIAPI
DetectJedecDdr4NvdimmN (
  IN  UINT8             Socket,
  IN  UINT8             Channel,
  IN  UINT8             Dimm
  );

#define DISABLE_MPR_OVERRIDE  0
#define ENABLE_MPR_OVERRIDE   1

/**

  Enable or disable the MPR buffer override setting for every DDR4 buffered DIMM in the socket

  @param[in] Host           - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket         - Socket number
  @param[in] MprOverride    - Enable MPR override mode if ENABLE_MPR_OVERRIDE
                            - Disable MPR override mode if DISABLE_MPR_OVERRIDE

  @retval None

**/
VOID
SetBufferMprOverrideMode (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    BOOLEAN   MprOverride
  );

#define DISABLE_PREAMBLE_TR_MODE  0
#define ENABLE_PREAMBLE_TR_MODE   1

/**

  Enable or disable LRDIMM Preamble TR Mode

  @param[in] Host             - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket           - CPU Socket Node number (Socket ID)
  @param[in] Ch               - Channel number (0-based)
  @param[in] Dimm             - Current dimm
  @param[in] PreambleTrMode   - Enable MPR override mode if ENABLE_PREAMBLE_TR_MODE
                              - Disable MPR override mode if DISABLE_PREAMBLE_TR_MODE

  @retval None

**/
VOID
SetLrdimmPreambleTrMode (
  IN    PSYSHOST    Host,
  IN    UINT8       Socket,
  IN    UINT8       Ch,
  IN    UINT8       Dimm,
  IN    UINT8       PreambleTrMode
  );

/**

  Prints all DIMM DQ Buffer CSR data from a specific socket.
  Note: This function can be called before read training has been successfully completed.
  Read training will be peformed while in before reading the buffer registers.

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket Id

  @retval N/A

**/
VOID
DumpStateBcwEarlyDdr4 (
  PSYSHOST  Host,
  UINT8     Socket
  );

/**

  Prints all DQ Buffer CSR data from a specific DIMM.
  Note: this function assumes that the read training has been successfully
  completed in order to read DRAM and Buffer data.

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number within the socket
  @param[in] Dimm     - DIMM number within the channel

  @retval N/A

**/
VOID
DumpStateBcwDdr4 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  );

/*
  Get the Cached Rx Eq value from RankList Structure

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket Id (0-based)
  @param[in] Ch       - Channel number (0-based)
  @param[in] Dimm     - DIMM number (0-based)
  @param[in] Rank     - Rank number (0-based)
  @param[in] Strobe:  - Dqs data group within the rank

  @retval - Cached DQ RX EQ value
*/
UINT32
GetCachedRxEq (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Rank,
  IN UINT8    Dimm,
  IN UINT8    Strobe
  );

/*
  Update the cached Rx Eq value in RankList Structure

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket Id (0-based)
  @param[in] Ch           - Channel number (0-based)
  @param[in] Dimm         - DIMM number (0-based)
  @param[in] Rank         - Rank number (0-based)
  @param[in] Strobe:      - Dqs data group within the rank
  @param[in] RxEqSetting  - Value to be updated in cache.

  @retval - N/A
*/
VOID
SetCachedRxEq (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Rank,
  IN UINT8    Dimm,
  IN UINT8    Strobe,
  IN INT16    RxEqSetting
  );

/**
  Populates a POWER_TRAIN_PARAM structure based on a MRC_GT signal name

  @param[in]  Host        Pointer to SysHost
  @param[in]  Socket      Socket number
  @param[in]  SignalName  Signal group name
  @param[out] Param       Pointer to the power training parameter structure to populate

  @retval     MRC_STATUS_SUCCESS                The Param structure was successfully populated
  @retval     MRC_STATUS_FAILURE                Unknown error occurred
  @retval     MRC_STATUS_SIGNAL_NOT_SUPPORTED   Invalid parameter for SignalName provided
**/
MRC_STATUS
GetPowerParamFromSignalName (
  IN PSYSHOST             Host,
  IN UINT8                Socket,
  IN MRC_GT               SignalName,
  OUT POWER_TRAIN_PARAM   *Param
  );

/**
  Function gets current SkipPprRowTest flag value


  @retval BOOLEAN - Current value of the SkipPprRowTest Flag

**/
BOOLEAN
GetSkipPprRowTestFlag (
  VOID
  );

/**
  Set SkipPprRowTest Flag to given Value

  @param[in]  FlagValue - Value to Set(TRUE or FALSE)

  @retval N/A
**/
VOID
SetSkipPprRowTestFlag (
  IN BOOLEAN FlagValue
  );

/**

  Enables parity checking on the DDR4 RCD/DRAM

  @param[in]  Socket:  Processor socket to check
  @param[in]  Ch:      Channel number

  @retval: None
**/
VOID
EFIAPI
EnableParityCheckingDdr4 (
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**

  Disables parity checking on the DDR4 DIMM

  @param[in]  Socket:  Processor socket to check
  @param[in]  Ch:      Channel number

**/
VOID
DisableParityCheckingDdr4 (
  UINT8     Socket,
  UINT8     Ch
  );

/**
  Indicate whether handling for an ADR event is needed.

  @param[in]  Setup   A pointer to the setup structure

  @retval FALSE   No ADR event needs to be handled.
  @retval TRUE    An ADR event needs to be handled.
**/
BOOLEAN
IsAdrHandlingNeeded (
  IN  SYS_SETUP *Setup
  );

#endif // #ifndef _BASE_MEMORY_CORE_INTERNAL_H_
