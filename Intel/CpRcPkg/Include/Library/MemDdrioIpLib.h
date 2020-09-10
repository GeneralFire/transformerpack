/** @file
  MemDdrioIpLib.h

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

#ifndef  _MEM_DDRIO_IP_LIB_H_
#define  _MEM_DDRIO_IP_LIB_H_

#include <ReferenceCodeDataTypes.h>
#include "SysHost.h"
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/Ddr5CoreLib.h>

//
// Maximum RxDfe Delay values
//
#define MAX_RXDFE_DELAY_SEL_LIMIT    3
//
// Minimum Rxdfe Delay values
//
#define MIN_RXDFE_DELAY_SEL_LIMIT    0
//
// Number of RxDfe Delay Settings
//
#define NUM_RXDFE_DELAY_SEL_SETTINGS 4
//
// Maximum RxDfe Code value
//
#define MAX_RXDFE_CODE_LIMIT    7
//
// Minimum RxDfe Code value
//
#define MIN_RXDFE_CODE_LIMIT    0
//
// Number of RXDfe Code Settings
//
#define NUM_RXDFE_CODE_SETTINGS 8

//
// Maximum Tx Echo Coef value
//
#define MAX_TX_ECHO_COEF_LIMIT    3
//
// Minimum Tx Echo Coef value
//
#define MIN_TX_ECHO_COEF_LIMIT    0
//
// Number of Tx Echo Coef Settings
//
#define NUM_TX_ECHO_COEF_SETTINGS 2
//
// Maximum Tx Echo Coef value
//
#define MAX_TX_ECHO_UI_SEL_LIMIT    1
//
// Minimum Tx Echo Coef value
//
#define MIN_TX_ECHO_UI_SEL_LIMIT    0
//
// Number of Tx Echo Coef Settings
//
#define NUM_TX_ECHO_UI_SEL_SETTINGS 2
//
// Default\Dont care value for GetDataGroupLimit function
//
#define DEFAULT_DATA_GROUP_DELAY 0
//
// Defines for use with SetDramResetN
// DRAM Assert\Deassert Function
//

#define DRAM_RESET_ASSERT TRUE
#define DRAM_RESET_DEASSERT FALSE

#define RUN_IN_PARALLEL            1 //For DCS/DCA training, it will use the QLBD and QLBS as the training feedback.
#define RUN_IN_SERIAL              0 //For DCS/DCA training, it will use the ALERT_n as the training feedback. There is only one ALERT_n per channel. So, need to train two sub channels serially.
#define TRAINING_MODE_NOT_SUPPORT  0xFF

#define RF_ON_DISABLE             0
#define RF_ON_ENABLE              1

#define WL_TRAIN_COUNT_WINDOW    32 //@4800, 32 cycles is 13.3ns, this is larger than the max tWLO defined by Jedec which is 9.5ns
#define WL_WR_TO_WR_DLY          72

#define RX_PATH_UNGATE_DISABLE    0
#define RX_PATH_UNGATE_ENABLE     1

#define RXVREF_SENSEAMP_SETTING   0x3F

/**

  Move the Clocks from it original point by the number of ticks specified as input parameter

  @param[in]  OffsetDelay   Offset to move the CLK

**/
VOID
EFIAPI
InitializeClkDelay (
  INT16   OffsetDelay
  );

/**
  Multi-use function to either get or set signal delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param level    - IO level to access
  @param sig      - Enumerated signal name
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param value    - Absolute value or offset selected by GSM_WRITE_OFFSET

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetSignal (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  MRC_LT    level,
  GSM_CSN   sig,
  UINT8     mode,
  INT16      *value                           // Value to program
  );

/*
  Returns true if GetSet (..,ALL_STROBES,..) modifies
  all the strobes across all subchannels

  @param[in]      Host              Pointer to SysHost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @retval   TRUE    GetSet modifies all strobes across subchannels when ALL_STROBES is used
  @retval   FALSE   GetSet modifies all strobes within each subchannel when ALL_STROBES is used

*/
BOOLEAN
IsAllStrobesAcrossSubCh (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      MRC_LT      Level,
  IN      MRC_GT      Group
  );

/*
  Returns true if a margin group acts as per strobe control but underline is per be bit control which has
  this behavior: When it is read, only the maximum value of the strobe is returned.
  The MemSweepLib need to adjust the individual bit sweep results by read back the per bit value
  with a per bit version group.

  @param[in]      Host              Pointer to SysHost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @retval   TRUE    Group acts as per strobe control but underline is per be bit control
  @retval   FALSE   Group does not act as per strobe control but underline is per be bit control

*/
BOOLEAN
IsPerBitControlWhileActPerStrobe (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      MRC_LT      Level,
  IN      MRC_GT      Group
  );


/*
  Returns a per bit control margin group that corresponds to the input group.

  NOTE: it should be called after IsPerBitControlWhileActPerStrobe() to get the corresponding group.

  @param[in]      Host              Pointer to SysHost structure
  @param[in]      Socket            Current socket under test (0-based)
  @param[in]      Level             Specifies the MRC_LT level (DdrLevel, LrbufLevel, etc)
  @param[in]      Group             Specifies the MRC_GT group that will be under test

  @retval   Corresponding per be bit control group

*/
MRC_GT
GetPerBitGroup (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      MRC_LT      Level,
  IN      MRC_GT      Group
  );

/*

  Return the number of knobs (signals) inside the specified group

  @param[in]      Host        Pointer to syshost structure
  @param[in]      Group       MRC_GT group under test
  @param[out]     MaxIoGroup  Pointer to return the Max number of IO groups

  @retval         MRC_STATUS_SUCCESS    If function successfully executed
  @retval         MRC_STATUS_GROUP_NOT_SUPPORTED    If group is not supported

*/
MRC_STATUS
GetNumberOfKnobsPerCmdCtlGroups (
  IN     PSYSHOST       Host,
  IN     MRC_GT         Group,
     OUT UINT8          *MaxIoGroup
  );

/*

  Get or Set and specific Knob inside a Cmd or Ctl group delays

  @param[in]      Host          Pointer to syshost
  @param[in]      Socket        Current socket under test (0-based)
  @param[in]      ChannelIndex  Current channel under test (0-based)
  @param[in]      KnobIndex     Current knob under test (0-based)
  @param[in]      Group         Specifies the MRC_GT group that will be under test
  @param[in]      Mode          Mode to access the register
                                GSM_READ_CSR: Read register directly instead of cache value
                                GSM_READ_ONLY: Only read, skip write portion
                                GSM_WRITE_OFFSET: Use the value as an offset to original
                                GSM_UPDATE_CACHE: Update cache
  @param[in,out]  DelayPtr      Pointer to read/write the value from current socket/channel/knob


*/
MRC_STATUS
GetSetCmdCtlIoKnob (
  IN        PSYSHOST          Host,
  IN        UINT8             Socket,
  IN        UINT8             ChannelIndex,
  IN        UINT8             KnobIndex,
  IN        MRC_GT            Group,
  IN        UINT8             Mode,
  IN  OUT   INT16             *DelayPtr
);

/**
  Multi-use function to either get or set control delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param group    - Group number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param value    - Value to program
  @param *minVal  - Current minimum command delay
  @param *maxVal  - Current maximum command delay

  @retval minVal, maxVal and MRC_STATUS

**/
MRC_STATUS
GetSetCtlGroupDelay (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  MRC_GT group,
  UINT8 mode,
  INT16 *value,
  UINT16 *minVal,
  UINT16 *maxVal
  );

/**
  Multi-use function to either get or set command vref

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param dimm     - DIMM number within a DDR channel (0-based, farthest slot == 0)
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param value    - Value to program

  @retval minVal and maxVal

**/
MRC_STATUS
GetSetCmdVref (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  MRC_LT level,
  UINT8 mode,
  INT16 *value
  );

/**
  Multi-use function to either get or set command delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param group    - Group number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param value    - Value to program
  @param *minVal  - Current minimum command delay
  @param *maxVal  - Current maximum command delay

  @retval minVal and maxVal

**/
MRC_STATUS
GetSetCmdGroupDelay (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  MRC_GT group,
  UINT8 mode,
  INT16 *value,
  UINT16 *minVal,
  UINT16 *maxVal
  );

/**

Set the training mode to latch the training result.

@param[in] Host  - Pointer to sysHost
@param[in] Socket  - Socket number
@param[in] Mode  - Training mode

@retval N/A

**/
VOID
SetTrainingModeLatchResult (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Mode
  );

/**
  Multi-use function to either get or set ERID vref

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Value to program or offset

  @retval Pi delay value

**/
MRC_STATUS
GetSetEridVref (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 mode,
  INT16 *value
  );

/**
  Initialize Chip Specific Scomp.

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param SubCh    - Sub channel number

  @retval         - EFI_SUCCESS

**/
MRC_STATUS
EFIAPI
InitializeCcScompChip (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh
  );

/**
  Get or Set Scomp Slew Rate Up or Down Adjuster.

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param SubCh    - Sub channel number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param UpDown   - TRUE  - Set Scomp Up Adj.
                  - FALSE - Set Scomp Down Adj.
  @param value    - Value to program

  @retval Pi delay value

**/
MRC_STATUS
EFIAPI
GetSetCcScompUpDownAdj (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     SubCh,
  IN      UINT8     Mode,
  IN      BOOLEAN   UpDown,
  IN OUT  UINT8     *Value
  );

/**
  Restore CA Slew Rate comp of channel.

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param Channel  - Channel number

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
RestoreCaSlewRate (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**
  Initialize Chip Specific Tx DQ Scomp.

  @param[in] Host     - Pointer to sysHost, the system Host (root) structure
  @param[in] socket   - Socket number
  @param[in] ch       - Channel number
  @param[in] SubCh    - Sub-Channel number
  @param[in] Strobe   - Strobe number

  @retval         - EFI_SUCCESS

**/
MRC_STATUS
EFIAPI
InitializeDataScompChip (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubCh,
  IN  UINT8     Strobe
  );

/**
  Get or Set Tx DQ Scomp Slew Rate Up or Down Adjuster.

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param Socket   - Socket number
  @param Ch       - Channel number
  @param Strobe   - Strobe number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param UpDown   - TRUE  - Set Scomp Up Adj.
                  - FALSE - Set Scomp Down Adj.
  @param value    - Value to program

  @retval Pi delay value

**/
MRC_STATUS
EFIAPI
GetSetDataScompUpDownAdj (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Ch,
  IN      UINT8     Strobe,
  IN      UINT8     Mode,
  IN      BOOLEAN   UpDown,
  IN OUT  UINT8     *Value
  );

/**
  Restore Tx DQ Slew Rate comp of channel.

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param Channel  - Channel number

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
EFIAPI
RestoreDqSlewRate (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**
  Multi-use function to either get or set clock delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param clk      - Clock number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param value    - Value to program

  @retval Pi delay value

**/
MRC_STATUS
GetSetClkDelay (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     clk,
  UINT8     mode,
  INT16      *value
  );

/** Set starting/initial values for clock and control signals.

  Initial values come from analog design.

  @param[in]  Host    Pointer to sysHost
  @param[in]  Socket  Socket number

  @retval N/A

**/
VOID
SetStartingCCC (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**

  Get min/max limits for Command, Control and Clock signals

  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Ch          - Channel number
  @param[in]  Dimm        - DIMM number
  @param[in]  Level       - IO level to access
  @param[in]  Group       - Command, clock or control group to access
  @param[out] *MinLimit   - Minimum delay value allowed
  @param[out] *MaxLimit   - Maximum delay value allowed

  @retval EFI_SUCESS if signal max/mins are returned
          EFI_NOT_FOUND otherwise
**/
EFI_STATUS
GetCmdGroupLimits (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    MRC_LT    Level,
  IN    MRC_GT    Group,
  OUT   UINT16    *MinLimit,
  OUT   UINT16    *MaxLimit
  );

/**

  Given an array of command delays relative to current clock and control delays,
  this function will combine shared settings in the DDRIO design and normalize the
  lowest command, clock or control value to their minimum limits.

  @param Host      - Pointer to sysHost, the system Host (root) structure struct
  @param socket    - Socket
  @param ch        - Channel
  @param listSize  - Number of entries in each list
  @param value     - Array of pointers to listType

  @retval MRC_STATUS_SUCCESS
  @retval MRC_STATUS_SIGNAL_NOT_SUPPORTED

**/
MRC_STATUS
SetCombinedCtlGroup (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT16 listSize,
  VOID *value
  );

/**

  Given an array of command delays relative to current clock and control delays,
  this function will combine shared settings in the DDRIO design and normalize the
  lowest command, clock or control value to their minimum limits.

  @param Host                 - Pointer to Host structure
  @param socket               - Processor socket within the system (0-based)
  @param ch                   - DDR channel number within the processor socket (0-based)
  @param level                - IO level to access
  @param mode                 - Bit-field of different modes
  @param listType             - Selects type of each list
  @param listSize             - Number of entries in each list
  @param *value               - VOID pointer that points to an Array of pointers to listType

  @retval MRC_STATUS

**/
MRC_STATUS
SetCombinedCmdGroup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  MRC_LT    level,
  UINT8     mode,
  UINT8     listType,
  UINT16    listSize,
  VOID      *value
  );

/**

  Given an array of RCD QxCA Training delays relative to clock and control delays,
  this function will combine shared settings in the DDRIO design and normalize the
  lowest command, clock or control value to their minimum limits.

  @param[in] Host         Pointer to host structure
  @param[in] Socket       Processor socket within the system (0-based)
  @param[in] Ch           DDR channel number within the processor socket (0-based)
  @param[in] SignalRange  Total number of signals swept
  @param[in] *Values      Array of RCD QxCA Training delays

  @retval None

**/
VOID
SetCombinedBacksideCmdGroup (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT16      SignalRange,
  IN  GSM_CSEDGE  (*SignalListEdge)[MAX_CH][MAX_RANK_CH][BACKSIDE_SIGNAL_RANGE]
  );

/**

  Initialize the DDRIO interface Wrapper

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/

UINT32
InitInterface (
  PSYSHOST  Host
  );

/**

  Check if DDR5 sub-channel A and B require equal Round Trip Latency.

  @param  VOID

  @retval TRUE   DDR5 Sub-channel A and B require equal Round Trip Latency.
  @retval FALSE  DDR5 Sub-channel A and B do not require equal Round Trip Latency.
**/
BOOLEAN
EFIAPI
IsEqualSubChRtlRequired (
  VOID
  );

/**

  Get the round trip step unit

  @param VOID

  @retval round trip step unit

**/

UINT8
EFIAPI
GetRoundTripStepUnit (
  VOID
  );

/**

  Get the IO latency step unit

  @param VOID

  @retval round trip step unit

**/

UINT8
EFIAPI
GetIoLatStepUnit (
  VOID
  );

/**
  Detect if DDRIO lane reversal is enabled.

  @param[in]  Host    - Pointer to sysHost.
  @param[in]  Socket  - Socket number.
  @param[in]  Channel - Channel number.

  @retval TRUE   DDRIO lane reversal is enabled.
  @retval FALSE  DDRIO lane reversal is disabled.

**/
BOOLEAN
EFIAPI
IsLaneReversalEnabled (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**

  Initialize the DDRIO interface

  @param[in] Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
InitDdrioInterfaceLate (
  IN    PSYSHOST Host
  );

/**

  Programs IO delays

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer, HBM)
  @param group   - CSM_GT - Parameter to program
  @param mode    - Bit-field of different modes
  @param value   - Pointer to delay value or offset based on mode

  @retval MRC_STATUS

**/

MRC_STATUS
GetSetDataGroup (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     SubCh,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  );

/*

  Dumps each value across each bit in the byte (2 Nibbles)

  @param[in]         Socket      Current socket under test (0-based)
  @param[in]         Channel     Current channel under test (0-based)
  @param[in]         Dimm        Current dimm under test (0-based)
  @param[in]         Rank        Current rank under test (0-based)
  @param[in]         Strobe      Current strobe under test (0-based)
  @param[in]         Bit         Current bit under test (0-based) or ALL_BITS
  @param[in]         Group       Parameter to be margined
  @param[in]         Caller      String that identifies the purpose to be calling this function

*/
VOID
DumpRxDelayBitRegs (
  IN        UINT8     Socket,
  IN        UINT8     Channel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_GT    Group,
  IN        CHAR8     *Caller
  );

/*

  Dumps each value across each bit in the byte (2 Nibbles)

  @param[in]         Socket      Current socket under test (0-based)
  @param[in]         Channel     Current channel under test (0-based)
  @param[in]         Dimm        Current dimm under test (0-based)
  @param[in]         Rank        Current rank under test (0-based)
  @param[in]         Strobe      Current strobe under test (0-based)
  @param[in]         Bit         Current bit under test (0-based) or ALL_BITS
  @param[in]         Group       Parameter to be margined
  @param[in]         Caller      String that identifies the purpose to be calling this function

*/
VOID
DumpTxDelayBitRegs (
  IN        UINT8     Socket,
  IN        UINT8     Channel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_GT    Group,
  IN        CHAR8     *Caller
  );

/*

  Dumps each value across each bit in the byte (2 Nibbles)

  @param[in]         Socket      Current socket under test (0-based)
  @param[in]         Channel     Current channel under test (0-based)
  @param[in]         Dimm        Current dimm under test (0-based)
  @param[in]         Rank        Current rank under test (0-based)
  @param[in]         Strobe      Current strobe under test (0-based)
  @param[in]         Bit         Current bit under test (0-based) or ALL_BITS
  @param[in]         Group       Parameter to be margined
  @param[in]         Caller      String that identifies the purpose to be calling this function

*/
VOID
DumpRxVrefBitRegs (
  IN        UINT8     Socket,
  IN        UINT8     Channel,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_GT    Group,
  IN        CHAR8     *Caller
  );

/**

  Get TxDelay

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - Bit-field of different modes
  @param value   - Pointer to delay value or offset based on mode

  @retval IO delay

**/

MRC_STATUS
GetSetTxDelay (
  PSYSHOST    Host,
  UINT8       socket,
  UINT8       ch,
  UINT8       dimm,
  UINT8       rank,
  UINT8       strobe,
  UINT8       bit,
  MRC_LT      level,
  MRC_GT      group,
  UINT8       mode,
  INT16       *value
  );

/**

  Get/Set Data Buffer DFE Coefficient

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Socket number (0-based)
  @param[in] Channel       - Channel number (0-based)
  @param[in] Dimm          - DIMM number (0-based)
  @param[in] Rank          - Rank number (0-based)
  @param[in] Strobe        - Strobe number (0-based)
  @param[in] Bit           - Bit number (0-based)
  @param[in] Level         - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in] Group         - CSM_GT - Parameter to program
  @param[in] Mode          - Bit-field of different modes
  @param[in][out] *Value   - Pointer to Tap value

  @retval SUCCESS

**/
MRC_STATUS
GetSetDataBufferDfeTapPerBit (
  IN     PSYSHOST    Host,
  IN     UINT8       Socket,
  IN     UINT8       Channel,
  IN     UINT8       Dimm,
  IN     UINT8       Rank,
  IN     UINT8       Strobe,
  IN     UINT8       Bit,
  IN     MRC_LT      Level,
  IN     MRC_GT      Group,
  IN     UINT8       Mode,
  IN OUT INT16       *Value
  );

/**

  Get RxDelay

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/

MRC_STATUS
GetSetRxDelay (
  PSYSHOST    Host,
  UINT8       socket,
  UINT8       ch,
  UINT8       dimm,
  UINT8       rank,
  UINT8       strobe,
  UINT8       bit,
  MRC_LT      level,
  MRC_GT      group,
  UINT8       mode,
  INT16       *value
  );

/**

  Get RxDelay

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval IO delay

**/

MRC_STATUS
GetSetRxDelayBit (
  PSYSHOST    Host,
  UINT8       socket,
  UINT8       ch,
  UINT8       dimm,
  UINT8       rank,
  UINT8       strobe,
  UINT8       bit,
  MRC_LT      level,
  MRC_GT      group,
  UINT8       mode,
  INT16       *value
  );

/**

  Set Sense Amp ODT Delays for DDR5

  @param[in]  Socket      Socket number

**/
VOID
SetSenseAmpODTDelaysDdr5 (
  IN      UINT8       Socket
  );

/**

  Determine if it is appropriate to use the multicast offset register

  @param[in]     Strobe  - Strobe number (0-based)
  @param[in]     Bit     - Bit number
  @param[in]     Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]     Group   - CSM_GT - Parameter to program
  @param[in]     Mode    - Bit-field of different modes

  @retval TRUE  - OK to use multicast registers
          FALSE - Cannot use multicast register
**/
BOOLEAN
UseDelayMulticastOffset (
  IN      UINT8     Strobe,
  IN      UINT8     Bit,
  IN      MRC_LT    Level,
  IN      MRC_GT    Group,
  IN      UINT8     Mode
  );

/**

  Get/Set Rx Vref thru multicast/unicast registers

  @param[in]      Host        Pointer to SysHost structure
  @param[in]      Socket      Current socket under test (0-based)
  @param[in]      Ch          Current channel under test (0-based)
  @param[in]      Dimm        Current dimm under test (0-based)
  @param[in]      Rank        Current rank under test (0-based)
  @param[in]      Strobe      Current strobe under test (0-based || ALL_STROBES)
  @param[in]      Bit         Current bit under test (0-based || ALL_BITS)
  @param[in]      Level       Current level (not applicable)
  @param[in]      Group       Current group under test (RxVref)
  @param[in]      Mode        Register/Cache access mode (GSM_READ_CSR, GSM_READ_ONLY, GSM_UPDATE_CACHE, GSM_FORCE_WRITE)
  @param[in,out]  Value       Pointer to data that will be used to retrieve or store

  @retval         MRC_STATUS_SUCCESS  Executed properly thru multicast or unicast registers
  @retval         Any other           Failure to write/read the register(s)

**/
MRC_STATUS
GetSetRxVref (
  IN        PSYSHOST  Host,
  IN        UINT8     Socket,
  IN        UINT8     Ch,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        UINT8     Strobe,
  IN        UINT8     Bit,
  IN        MRC_LT    Level,
  IN        MRC_GT    Group,
  IN        UINT8     Mode,
  IN        INT16     *Value
  );

/**

  Get Set TxRiseFallSlewRate

  @param[in]      Host    - Pointer to sysHost
  @param[in]      Socket  - Socket number
  @param[in]      Ch      - Channel number (0-based)
  @param[in]      Dimm    - DIMM number (0-based)
  @param[in]      Rank    - Rank number (0-based)
  @param[in]      Strobe  - Strobe number (0-based)
  @param[in]      Bit     - Bit number
  @param[in]      Level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param[in]      Group   - CSM_GT - Parameter to program
  @param[in]      Mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param[in][out] Value   - Data to program

  @retval IO delay

**/
MRC_STATUS
GetSetTxRiseFallSlewRate (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Ch,
  IN UINT8        Dimm,
  IN UINT8        Rank,
  IN UINT8        Strobe,
  IN UINT8        Bit,
  IN MRC_LT       Level,
  IN MRC_GT       Group,
  IN UINT8        Mode,
  IN OUT INT16    *Value
  );

/**
  Converts DDRCRCOMPCTL0.DqOdtPupVref value to ODT in ohms (or vice versa)

  @param[in] Value            Value to convert.
  @param[in] Code             Type of operation CODE_ODT or DECODE_ODT.

  @return                     Encoded or decoded ODT value.
**/
UINT16
CodeDeCodeRxOdt (
  IN  UINT16 Value,
  IN  UINT8  Code
  );

/**
  Converts DDRCRCOMPCTL0.DqDrvPupVref value to MC RON in ohms (or vice versa)

  @param[in] Value            Value to convert.
  @param[in] Code             Type of operation CODE_ODT or DECODE_ODT.

  @return                     Encoded or decoded MC RON value.
**/
UINT16
CodeDeCodeTxRon (
  IN  UINT16 Value,
  IN  UINT8  Code
  );

/**

  Converts the Vref setting to be written into MR6

  @param Host - Pointer to sysHost struct
  @param vref - vref value to write

**/
UINT8
GetVrefRange (
  PSYSHOST    Host,
  UINT8       vref
  );

/**

  Converts the Vref setting to be written into MR6

  @param Host    - Pointer to sysHost struct
  @param socket  - processor number

**/

VOID
UpdateSafeTxVref (
  PSYSHOST    Host,
  UINT8       socket,
  MRC_LT      level
  );

MRC_STATUS
GetDataGroupLimits (
  PSYSHOST  Host,                             // Pointer to sysHost, the system Host (root) structure
  MRC_LT    level,                            // IO level to access
  MRC_GT    group,                            // Data group to access
  UINT16    *minLimit,                        // Minimum delay value allowed
  UINT16    *maxLimit,                        // Maximum absolute value allowed
  UINT16    *usdelay                          // Time in usec required for new setting to take effect
  );

UINT32
CacheDDRIO (
  PSYSHOST Host,
  UINT8    socket
  );

VOID
SetTrainingMode (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       rank,                             // Rank number (0-based)
  UINT8       mode                              // Training mode
  );

/**
  Issue a ZQCS or ZQCL command

  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param zqType  - ZQ command type

  @retval N/A

**/
VOID
DoZQ (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       zqType                            // Type of ZQ Calibration: Long or Short
  );

//
// Definitions used with GetOriginalRtlChip
// RtlInitializationMode parameter.
//

#define RTL_INITIALIZE_ONCE   0
#define RTL_RE_INITIALIZE     1

VOID
GetOriginalRtlChip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Rank number (0-based)
  UINT8       RtlInitializationMode,            // RTL Initialization Mode: RTL_INITIALIZE_ONCE or RTL_RE_INITIALIZE
  UINT8       *roundTripLatency                 // Pointer to the round trip latency
  );

/**

  Set the IO latency register value in the register and cache

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0 >= Ch < MAX_CH)
  @param[in]  Rank            - Rank number (0 >= Rank < MAX_RANK_CH)
  @param[in]  IoLatencyValue  - IO Latency value to set

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
SetIOLatency (
  IN    UINT8       Socket,                           // Socket Id
  IN    UINT8       Ch,                               // Channel number (0-based)
  IN    UINT8       Rank,                             // Rank number (0-based)
  IN    UINT8       IoLatencyValue                    // New IO Latency value
  );

/**
  Get the IO latency value for a single rank

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0 >= Ch < MAX_CH)
  @param[in]  Rank            - Rank number (0 >= Rank < MAX_RANK_CH)
  @param[in]  Mode            - Bit field of access modes
  @param[out] IoLatencyValue  - Pointer to caller's IO Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetIoLatency (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *IoLatencyValue
  );

/**

  Set the round trip register value

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  RoundTripValue  - New Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          EFI_OUT_OF_RESOURCES - indicates to the caller that the input RoundTripValue exceeds the max Round Trip Latency.
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
SetRoundTrip (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    RoundTripValue
  );

/**
  Get the round trip value

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Mode            - Bit field of access modes
  @param[out] RoundTripValue  - Pointer to caller's Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
GetRoundTrip (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *RoundTripValue
  );

// ICX_TODO - this function should be removed
VOID
ClearBWErrorStatus (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch
  );

//
// TxEq Data inversion definitions
// for use with ToggleEqPost2Sign
//

#define INVERT_TRUE             0
#define INVERT_FALSE            1

/**

  Toggle EqPost2Sign while TxEq Tunning for tap2

  @param Host    - Pointer to sysHost
  @param Socket  - Socket number
  @param Enable  - Enable / Disable

  @retval N/A

**/
MRC_STATUS
ToggleEqPost2Sign (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Strobe,
  IN UINT8    Enable
  );

/**

  Control flow between 10nm and other legacy products

  @param Host    - Pointer to sysHost
  @param Param   - Parameter power training

  @retval N/A

**/
MRC_STATUS
ExecuteTheNextOptPhaseOnThisProduct (
  IN PSYSHOST          Host,
  IN POWER_TRAIN_PARAM Param
  );

/**

  Set the initial configuration for the TxEq training

  @param Host    - Pointer to sysHost
  @param Param   - Parameter power training

  @retval N/A

**/
MRC_STATUS
InitTxEqTunning (
  IN PSYSHOST          Host,
  IN UINT8             Socket
  );

/**

  Sets aggressor bits for each bit lane

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket
  @param[in]  Ch           - Channel

  @retval SUCCESS

**/
UINT32
PxcProgramAggressors (
  IN     PSYSHOST        Host,
  IN     UINT8           Socket,
  IN     UINT8           Ch
  );

/**
  This function sets capacitive/inductive values.

  @param[in]  Host         - Pointer to sysHost
  @param[in]  Socket       - Socket
  @param[in]  Ch           - Channel

  @retval SUCCESS

**/
UINT32
PxcProgramCapInd (
  IN     PSYSHOST        Host,
  IN     UINT8           Socket,
  IN     UINT8           Ch
  );

/**

  This function performs DDRIO initialization related to Phase Based Cross-talk Cancellation (PXC)

  @param[in]  Host           - Pointer to sysHost
  @param[in]  Socket         - Socket
  @param[in]  Ch             - Channel
  @param[in]  PxcEnDisable   - Enable/disable PXC

  @retval none

**/
VOID
PxcSetupDdrio (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     PxcEnDisable
  );

VOID
PostTrainingInitDdrio (
  PSYSHOST Host
  );

/**

  Set the CAS write latency for DDRT

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number
  @param[in] Cwl          - CAS Write Latency value to set

  @retval N/A

**/
VOID
SetDdrtCwlDdrio (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Ch,
  IN  UINT32                    Cwl
  );

VOID
SetRankEnDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch,
  UINT8                     RankEn
  );

VOID
SetRxOffsetDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch,
  UINT8                     Dimm,
  UINT8                     Rank
  );

/**

  Perform DDRIO functionality necessary to support higher command-to-data completion delay values

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number

  @retval N/A

**/
VOID
ExtendCmdToDataCompletionDdrio (
  IN  PSYSHOST                  Host,
  IN  UINT8                     Socket,
  IN  UINT8                     Ch
  );

/**
  Reads or Writes the Sense Amp or Rx skewed DQSN/P offset parameter

  @param[in]  Host           - Pointer to sysHost
  @param[in]  Socket         - Socket number
  @param[in]  Ch             - Channel number (0-based)
  @param[in]  Dimm           - Dimm number (0-based)
  @param[in]  Rank           - Rank number (0-based)
  @param[in]  Strobe         - Strobe number (0-based)
  @param[in]  Bit            - Bit number (0-based)
  @param[in]  Level          - IO level to access
  @param[in]  Group          - Group number (RxSenseAmpOffset, RxFifoDqsPBitSkew, or RxFifoDqsNBitSkew)
  @param[in]  Mode           - GSM_READ_CSR - Read the data from hardware and not cache
                             - GSM_READ_ONLY - Do not write
                             - GSM_WRITE_OFFSET - Write offset and not value
                             - GSM_FORCE_WRITE - Force the write
  @param[in/out] Value       - pointer to an INT16 value

  @retval MRC_STATUS

**/
MRC_STATUS
EFIAPI
GetSetSenseAmpRxOffsetDdrio (
  IN PSYSHOST               Host,
  IN UINT8                  Socket,
  IN UINT8                  Ch,
  IN UINT8                  Dimm,
  IN UINT8                  Rank,
  IN UINT8                  Strobe,
  IN UINT8                  Bit,
  IN MRC_LT                 Level,
  IN MRC_GT                 Group,
  IN UINT8                  Mode,
  IN OUT INT16              *Value
  );

/**
  Perform any silicon specific initialization required before executing
  Read Early Vref Centering.

  @retval N/A
**/
VOID
EFIAPI
ChipInitBeforeRdEarlyVrefCentering (
  VOID
  );

/**
  Perform any silicon specific initialization required before executing
  Write Early Vref Centering.

  @retval N/A
**/
VOID
ChipInitBeforeWrEarlyVrefCentering (
  VOID
  );

/**
  Perform any silicon specific initialization required before Recieve Enable Training

  @param[in, out] ** ChipDataPtr - A pointer to a pointer can be used to pass data

  @retval EFI_SUCCESS if init done
          !EFI_SUCCESS if other errors were encountered
**/

EFI_STATUS
EFIAPI
SaveAndInitChipRxEnableParams (
  IN OUT VOID                               **ChipDataPtr
  );

/**
  Perform any silicon specific initialization required after Recieve Enable Training

  @param[in, out] ** ChipDataPtr - A pointer to a pointer can be used to pass data

  @retval EFI_SUCCESS if init done
          !EFI_SUCCESS if other errors were encountered
**/
EFI_STATUS
EFIAPI
RestoreChipRxEnableParams (
  IN OUT VOID                               **ChipDataPtr
  );

VOID
SetIodirectionValidDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket
  );


/**

  Set Qlbd and Qlbs Io direction.

  @param[in] Host              - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket            - Socket Id
  @param[in] RxPathUngateValue - Rx Path Ungate Value to program

  @retval  None

**/
VOID
EFIAPI
SetQlbdQlbsIodirection (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT8                     RxPathUngateValue
  );

VOID
SwitchToNormalModeDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket
  );

VOID
SetSelectRefreshDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket
  );

/**

    Get LV Mode

    @param Host   - Pointer to sysHost, the system Host (root) structure
    @param socket - Processor socket

    @retval (UINT8)ddrCRCompCtl0.Bits.lvmode

**/
UINT8
GetLvmode (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Processor socket ID
  );

UINT32
SetDataTxOnDdrio (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT32    TxOn
  );

UINT32
GetCachedDataRxOffsetDdrio (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank,
  UINT8     Strobe,
  UINT8     Bit
  );

/**

  Get the results of test execution

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param ch      - Channel number (0-based)
  @param strobe  - Strobe number (0-based)

  @retval  Feedback data

**/
UINT32
GetDataTrainFeedback (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       strobe                            // Strobe number (0-based)
  );

VOID
SetIoLatencyIoComp (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     SubCh,
  UINT32    ioLatIoComp
  );

/**

  Chip specific code path to get the feedback from the correct register

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param ch      - Channel number (0-based)
  @param strobe  - Strobe (0-based)

  @retval (UINT32)feedback

**/
UINT32
ReadTrainFeedback (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       strobe                            // Strobe (0-based)
  );

/**

  Chip specific code path to execute step for lower nibble

  @param Host           - Pointer to sysHost, the system Host (root) structure
  @param socket         - CPU Socket Node number (Socket ID)
  @param ch             - Channel number (0-based)
  @param strobe         - Strobe (0-based)
  @param csrReg         - CSR Register
  @param strobeFeedback - Pointer to or in the lower nibble

  @retval None

**/
VOID
SetChipGetCtlClkResLowerNibbleStrobeFeedback (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       strobe,                           // Strobe (0-based)
  UINT32      *csrReg,                          // CSR Register
  UINT32      *strobeFeedback                   // Pointer to or in the lower nibble
  );

/**

  Chip specific code path to read training feedback for LRDIMMs

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param ch      - Channel number (0-based)
  @param strobe  - Strobe (0-based)
  @param bit     - Bit

  @retval (UINT32) feedback

**/
UINT32
LrDimmReadTrainFeedback (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       strobe,                           // Strobe (0-based)
  UINT8       bit                               // Bit
  );

/**

  Set Sense Amp ODT Delays

  @param Host        - Pointer to sysHost, the system Host (root) structure
  @param socket      - Socket number

  @retval None

**/
VOID
SetSenseAmpODTDelays (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // CPU Socket Node number (Socket ID)
  );

/**

  Get the DDRT training results

  @param Host   - Pointer to sysHost, the system Host (root) structure struct
  @param socket - Socket Id
  @param ch     - Channel number (0-based)
  @param dimm   - Current dimm

  @retval DDRT training results

**/
UINT8
GetDdrtTrainResult (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  );

/**

  setup to do before TxPiSample() main body. Grantley does not have any special setup to do

  @param dataControl  - Training mode data structure

  @retval None

**/
VOID
TxPiSampleChipInit (
  PCHIP_DDRIO_STRUCT dataControl                // Training mode data structure
  );

/**

  Gets the logical rank number

  @param Host        - Pointer to sysHost, the system Host (root) structure
  @param socket      - Processor socket to check
  @param ch          - channel number
  @param dimm        - DIMM number
  @param rank        - rank number
  @param dataControl - Pointer to data control structure

  @retval None

**/
VOID
GetChipLogicalRank (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Current rank
  PCHIP_DDRIO_STRUCT dataControl                // Pointer to data control structure
  );

/**
Get DDR5 Intf Cmd train build target function.

@param[in]  Host              Ptr to SysHost.
@param[in]  Socket            Socket.
@param[in]  Channel           Channel.
@param[in]  SubCh             Sub Channel.
@param[in]  Strobe            Strobe.
@param[out] Feedback          Feedback.

@retval EFI_SUCCESS           Wrote successfully.
@retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
GetDdr5IntfCmdTrainCtrlFeedbackTarget (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 SubCh,
  IN UINT8 Strobe,
  IN UINT8 Dimm,
  OUT UINT32 *Feedback
  );

/**

  Chip specific setup to set Pi training mode

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - processor socket ID
  @param ch               - processor socket channel number
  @param dataControl      - Training mode data structure

  @retval None

**/
VOID
SetChipTxPiSampleTrainingMode (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  PCHIP_DDRIO_STRUCT dataControl                // Training mode data structure
  );


/**

  Chip specific setup for TxPiSample TXDQS Strobe training, part 1

  @param Host        - Pointer to sysHost, the system Host (root) structure
  @param socket      - processor socket ID
  @param ch          - Processor socket Channel number
  @param dimm        - Processor socket channel dimm number
  @param rank        - Processor socket channel dim rank number
  @param piDelay     - piDelay for nibble
  @param dataControl - training mode data

  @retval None

**/
VOID
SetChipTxPiSampleTrainingTXDQSStrobesI (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Current rank
  INT16       piDelay,                          // piDelay for nibble
  PCHIP_DDRIO_STRUCT dataControl                // Training mode data structure
  );

/**

  Chip specific setup for TxPiSample TXDQS Strobe training, part 2

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - processor socket ID
  @param ch               - Processor socket Channel number
  @param dimm             - Processor socket channel dimm number
  @param dataControl      - Training mode data structure

  @retval None

**/
VOID
SetChipTxPiSampleTrainingTXDQSStrobesII (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  PCHIP_DDRIO_STRUCT dataControl                // Training mode data structure
  );

  /**

  Gets the Tx Eq settings list

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetTxEqSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  );

MRC_STATUS
GetDdjcDeltaSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  );

/*++
  Enables Write CRC in the DDRIO

  @param Host    - Pointer to sysHost

  @retval N/A
--*/
VOID
EnableWrCrcDdrio (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch
  );

#define LONG_PREAMBLE_ENABLE  1
#define LONG_PREAMBLE_DISABLE 0

/**

  Returns the DDRIO Long Preamble Enable setting

  @param[in] Socket      - Socket
  @param[in] Ch          - Ch

  @retval LONG_PREAMBLE_ENABLE if long preamble is enabled in the DDRIO
          LONG_PREAMBLE_DISABLE otherwise

**/
UINT8
EFIAPI
GetLongPreambleEnableDdrio (
  IN  UINT8     Socket,
  IN  UINT8     Ch
  );

/**

  Sets the DDRIO Long Preamble Enable setting

  @param[in] Socket      - Socket
  @param[in] Ch          - Ch
  @param[in] Preamble    - LONG_PREAMBLE_ENABLE or LONG_PREAMBLE_DISABLE

  @retval   N/A
**/
VOID
EFIAPI
SetLongPreambleEnableDdrio (
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Preamble
  );

/**

  Set the round trip register value for DDR5

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Subch           - Sub Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  RoundTripValue  - New Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
SetRoundTripDdr5 (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT8    Rank,
  IN UINT8    RoundTripValue
  );


/**

  Set the round trip register value for DDR5 for all ranks

  @param[in]  Socket          Socket number
  @param[in]  Ch              Channel number (0-based)
  @param[in]  Subch           Sub Channel number (0-based)
  @param[in]  RoundTripValue  New Round Trip Latency value

  @retval MRC_STATUS_SUCCESS  When this function completes execution

**/
MRC_STATUS
SetRoundTripAll (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    RoundTripValue
  );

/**
  Find the maximal round trip latency of all ranks in a channel, adjust each rank's
  round trip latency to this common channel level value.

  @param[in    ]  Socket            Socket number
  @param[in out]  RoundTripLatency  Input value is per-rank RTL, output value is the adjusted per-channel RTL value.
  @param[in out]  IoLatency         Input value is per-rank IOL, output value is the adjusted per-channel IOL value.

  @retval None
*/
VOID
EFIAPI
AdjustChannelRoundTrip (
 IN     UINT8   Socket,
 IN OUT UINT8   (*RoundTripLatency)[MAX_CH][SUB_CH][MAX_RANK_CH],
 IN OUT UINT8   (*IoLatency)[MAX_CH][SUB_CH][MAX_RANK_CH]
 );

/**

  Get the round trip register value for DDR5

  @param[in]  socket          - Socket number
  @param[in]  ch              - Channel number (0-based)
  @param[in]  subch           - Sub Channel number (0-based)
  @param[in]  rank            - Rank number (0-based)
  @param[in]  Mode            - Bit field specifying access mode
  @param[out] RoundTripValue  - Pointer to caller's Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER if channel or rank is out of range
          !EFI_SUCCESS if other errors were encountered

**/
EFI_STATUS
GetRoundTripDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *RoundTripValue
  );

/**

  Set the IO latency register value in the register and cache

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0 >= Ch < MAX_CH)
  @param[in]  Subch           - Sub Channel number (0-based)
  @param[in]  Rank            - Rank number (0 >= Rank < MAX_RANK_CH)
  @param[in]  IoLatencyValue  - IO Latency value to set

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
SetIoLatencyDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    Rank,
  IN    UINT8    IOLatencyValue
  );

/**
  Get the IO latency value for a single rank

  @param[in]  Socket          - Socket numer
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Subch           - Sub Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Mode            - Bit field of access modes
  @param[out] IoLatencyValue  - Pointer to caller's IO Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetIoLatencyDdr5 (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    SubCh,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *IoLatencyValue
  );

/**

  Chip specific code path to get the feedback from the correct register for DDR5

  @param[in] Host         - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket       - CPU Socket Node number (Socket ID)
  @param[in] Channel      - Channel number (0-based)
  @param[in] SubChannel   - Sub Channel number (0-based)
  @param[in] InputStrobe  - Strobe (0-based)

  @retval (UINT32)feedback

**/
UINT32
ReadDataTrainCounterFeedbackDdr5 (
  IN PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  IN UINT8       Socket,                           // CPU Socket Node number (Socket ID)
  IN UINT8       Channel,                          // Channel number (0-based)
  IN UINT8       SubChannel,                       // Sub Channel number (0-based)
  IN UINT8       InputStrobe                       // Strobe (0-based)
  );

/**

  Chip specific code path to get training feedback from the correct register for DDR5

  @param[in] Host         - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket       - CPU Socket Node number (Socket ID)
  @param[in] Channel      - Channel number (0-based)
  @param[in] SubChannel   - Sub Channel number (0-based)
  @param[in] InputStrobe  - Strobe (0-based)

  @retval (UINT32) Feedback - Training feedback

**/
UINT32
ReadDataTrainResultFeedbackDdr5 (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Channel,
  IN UINT8       SubChannel,
  IN UINT8       InputStrobe
  );

/** Set starting/initial values for clock and control signals for BRS.

  Initial values come from analog design.

  @param[in]  Host    Pointer to sysHost
  @param[in]  Socket  Socket number

  @retval N/A

**/
VOID
SetStartingCCCBrs10nm (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/*++

  Routine Description:

  Get's the results from the current test for DDR5 Internal WL Coarse Sweep.

  Arguments:

  @param[in]  Host                         - Pointer to sysHost
  @param[in]  ChBitmask                    - Ch Bit mask for which test should be setup for.
  @param[in]  dimm                         - DIMM number
  @param[in]  rank                         - rank number of the DIMM
  @param[out] StrobePass                   - strobe pass number
  @param[in]  InternalCycleAlignmentValue  - internal cycle delay to be tested
  @param[out] InternalCycleAlignment       - contains the final cycle alignment
  @param[out] trainRes                     - contain training result
  Returns:

N/A

--*/
VOID
GetResultsInternalWLDdr5 (
  IN  PSYSHOST                    Host,
  IN  UINT8                       socket,
  IN  UINT32                      ChBitmask,
  IN  UINT8                       dimm,
  IN  UINT8                       rank,
  OUT UINT32                      StrobePass[MAX_CH][SUB_CH],
  IN  UINT8                       InternalCycleAlignmentValue,
  OUT INT16                       InternalCycleAlignment[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  OUT struct TrainingResultsDdr5  trainRes[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/*++

  Routine Description:

  Get's the results from the current test for DDR5 Internal WL Fine Sweep.

  Arguments:

  Host      - Pointer to sysHost
  dimm      - DIMM number
  rank      - rank number of the DIMM

  Returns:

  N/A

--*/
VOID
GetResultsInternalWLFineSweepDdr5 (
  PSYSHOST                   Host,
  UINT8                      socket,
  UINT8                      dimm,
  UINT8                      rank,
  UINT32                     StrobePass [MAX_CH][SUB_CH],
  INT16                      Offsets,
  INT16                      InternalCycleAlignment [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  INT16                      TxDqsDelay [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  struct TrainingResultsDdr5 trainRes [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**

Given an array of command delays relative to current clock and control delays,
this function will combine shared settings in the DDRIO design and normalize the
lowest command, clock or control value to their minimum limits.

@param[in] Host                 - Pointer to Host structure
@param[in] Socket               - Processor socket within the system (0-based)
@param[in] Ch                   - DDR channel number within the processor socket (0-based)
@param[in] SubChannel           - DDR sub channel number
@param[in] Level                - IO level to access
@param[in] Mode                 - Bit-field of different modes
@param[in] ListType             - Selects type of each list
@param[in] ListSize             - Number of entries in each list
@param[in] Value                - Array of pointers to listType
@param[in] ValueSize            - List data size

@retval MRC_STATUS

**/
MRC_STATUS
SetCombinedCaGroupDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     SubChannel,
  IN MRC_LT    Level,
  IN UINT8     Mode,
  IN UINT8     ListType,
  IN UINT16    ListSize,
  IN VOID      *Value,
  IN UINT32    ValueSize
  );

/**

  Multi-use function to either get or set signal delays based on the provided group number for DDR5

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param level    - IO level to access
  @param sig      - Enumerated signal name
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
  -  GSM_READ_ONLY - Do not write
  -  GSM_WRITE_OFFSET - Write offset and not value
  -  GSM_FORCE_WRITE - Force the write
  @param value    - Absolute value or offset selected by GSM_WRITE_OFFSET

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetSignalDdr5 (
  PSYSHOST  Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8     socket,                           // Processor socket within the system (0-based)
  UINT8     ch,                               // DDR channel number within the processor socket (0-based)
  MRC_LT    level,                            // IO level to access
  GSM_CSN   sig,                              // Enumerated signal name
  UINT8     mode,                             // GSM_READ_CSR - Read the data from hardware and not cache
                                              // GSM_READ_ONLY - Do not write
                                              // GSM_WRITE_OFFSET - Write offset and not value
                                              // GSM_FORCE_WRITE - Force the write
  INT16      *value                           // Value to program
  );

/**

  Chip specific code path to get the feedback from the correct register for DDR5

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param ch      - Channel number (0-based)
  @param strobe  - Strobe (0-based)

  @retval (UINT32)feedback

**/
UINT32
ReadTrainFeedbackDdr5 (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       subch,                            //Sub channel number.
  UINT8       strobe,                            // Strobe (0-based)
  UINT8       Dimm
  );

/**

  Set DDRIO Write Leveling Mode For DDR5.

  @param Host         - Pointer to sysHost
  @param socket       - Processor to initialize
  @param ch           - channel number
  @param dimm         - dimm number
  @param subch        - sub channel number
  @param rank         - rank number
  @param piDelay      - pi delay
  @param dataControl  - data control value
  @param StrobePass   - strobe pass number
  @param UpdateMode   - register program mode

  @retval N/A

**/
VOID
SetDdrioWlModeDdr5 (
  IN PSYSHOST Host,
  IN UINT8 socket,
  IN UINT8 ch,
  IN UINT8 dimm,
  IN UINT8 subch,
  IN UINT8 rank
  );


/**

This routine clears the wl settings and make it ready to be written and read for DDR5.

@param[in] Host         - Pointer to sysHost
@param[in] socket       - Processor to initialize

@retval N/A

**/
VOID
ClearDdrioWlModeDdr5 (
  IN PSYSHOST Host,
  IN UINT8    socket
  );

/**

  This routine gets memory ready to be written and read for DDR5.

  @param[in] Host         - Pointer to sysHost
  @param[in] socket       - Processor to initialize
  @param[in] ch           - Channel number
  @param[in] subch        - Sub channel number
  @param[in] Value        - Value

  @retval N/A

**/
VOID
ProgramDriveDqsLowDdr5 (
  IN PSYSHOST Host,
  IN UINT8    socket,
  IN UINT8    ch,
  IN UINT8    subch,
  IN UINT8    Value
  );

/**

  Set the Drive DQS Low Bit for DDR5.

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Processor to initialize
  @param[in] Ch           - channel number
  @param[in] Subch        - sub channel number
  @param[in] Value        - value to be programmed
  @param[in] Drive        - Drive or not

  @retval N/A

**/
VOID
EFIAPI
SetDriveDqsLowDdr5 (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Subch,
  IN UINT8 Value,
  IN BOOLEAN Drive
  );

/**

  Override the DQ value.

  @param Host         - Pointer to sysHost
  @param Socket       - Processor to initialize
  @param Ch           - channel number
  @param SubChMask    - sub channel mask
  @param Strobe       - strobe number
  @param Override     - override or not
  @param Value        - value

  @retval N/A

**/
VOID
OverrideDqValueDdr5 (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    SubChMask,
  UINT8    Strobe,
  BOOLEAN  Override,
  UINT8    Value
  );

/**

  Set Dq Counting Window of all channels for a given Dimm and Rank

  @param host      - Pointer to sysHost
  @param socket    - Current Socket
  @param dimm      - DIMM number
  @param rank      - rank number of the DIMM

  @retval N/A

**/
VOID
EFIAPI
SetDqCountingWindow (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank
  );

/**

  Set Dq Counting Window for a channel

  @param Socket    - Current Socket
  @param Channel   - Channel number

  @retval N/A

**/
VOID
EFIAPI
SetDqCountingWindowByChannel (
  UINT8     Socket,
  UINT8     Channel
  );

/**
Set Req Counting Window

@param host      - Pointer to sysHost
@param socket    - Current Socket
@param dimm      - DIMM number
@param rank      - rank number of the DIMM

@retval N/A

**/
VOID
EFIAPI
SetReqCountingWindow (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChBitMask,
  UINT8     Dimm
  );

/**

DDRIO Init for Req CLK Training.

@param host      - Pointer to sysHost
@param socket    - Current Socket

@retval N/A

**/
VOID
EFIAPI
ReqClkDdrioInit (
  PSYSHOST   Host,
  UINT8      Socket,
  UINT32     ChBitMask,
  UINT8      Dimm
  );

/**

Programs rx_path_ungate = 1

@param host      - Pointer to sysHost
@param socket    - Current Socket

@retval N/A

**/
VOID
EFIAPI
CleanupReqClk (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     ChBitMask,
  UINT8     Dimm
  );

/**

  Set Counting Window of all channels with the given Dimm and Rank.

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Dimm      - DIMM number
  @param[in] Rank      - rank number of the DIMM
  @param[in] CountWindow  - Count Window Value to program

  @retval N/A

**/
VOID
EFIAPI
SetCountingWindow (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT32    CountWindow
  );

/**

  Set Counting Window per channel

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Current Socket
  @param[in] Channel      - Channel number
  @param[in] CountWindow  - Count Window Value to program

  @retval N/A

**/
VOID
EFIAPI
SetCountingWindowByChannel (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT32    CountWindow
  );

/**

  Set Data Train Count Window of all channel for a given Dimm and Rank

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Dimm      - DIMM number
  @param[in] Rank      - Rank number of the DIMM
  @param[in] Value     - Value to be programmed

  @retval N/A

**/
VOID
EFIAPI
SetDataTrainCountWindow (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Value
  );

/**

  Set Data Train Count Window for a channel

  @param[in] Socket    - Current Socket
  @param[in] Channel   - Channel number
  @param[in] Value     - Value to be programmed

  @retval N/A

**/
VOID
EFIAPI
SetDataTrainCountWindowByChannel (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Value
  );

/**
  Sets the DQ Sample Mask field to mask off any DQ bits not considered
  in the OR computation for the command training counting.

  @param[in] Socket     - Socket number
  @param[in] ChBitmask  - Channel bit mask
  @param[in] MaskLane   - Indicate the lane needs to mask

  @retval N/A
**/
VOID
EFIAPI
SetCounterDqSampleMask (
  IN UINT8   Socket,
  IN UINT32  ChBitmask,
  IN UINT8   MaskLane
  );

/**

  Set Dq Counting Mode for all channels with given Dimm and Rank

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Dimm      - DIMM number
  @param[in] Rank      - rank number of the DIMM

  @retval N/A

**/

VOID
EFIAPI
EnableDdrioTrainingCounter (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**

  Set Dq Counting Mode per channel

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Channel   - Channel number

  @retval N/A

**/

VOID
EFIAPI
EnableDdrioTrainingCounterByChannel (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel
  );
/**

  Enable CS assertion trigger or not for all channels with the given Dimm and Rank.

  @param Host      - Pointer to sysHost
  @param Socket    - Current Socket
  @param DIMM      - Dimm number
  @param Rank      - Rank number
  @param Value     - 1: enable Csa Trigger. 0: disable Csa Trigger.

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
EnableCsaTrigger (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Dimm,
  IN UINT8 Rank,
  IN UINT8 Value
  );

/**

  Enable CS assertion trigger or not per channel.

  @param Host      - Pointer to sysHost
  @param Socket    - Current Socket
  @param Channel   - Channel number
  @param Value     - 1: enable Csa Trigger. 0: disable Csa Trigger.

  @retval EFI_STATUS

**/
VOID
EFIAPI
EnableCsaTriggerByChannel (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Value
  );

//
// Training modes for SetTrainingModeDdr5
//
typedef enum {
  TmReceiveEnable,
  TmReceiveEnableExit,
  TmRxDqDqs,
  TmRxDqDqsCoarseExit,
  TmRxDqDqsExit,
  TmWriteLeveing,
  TmWriteLeveingExit,
  TmTxDqDqs,
  TmTxDqDqsExit,
  TmSenseAmp,
  TmSenseAmpExit,
  TmQcsQcaClk,
  TmQcsQcaClkExit,
  TmBacksideTraining,
  TmBacksideTrainingExit,
  TmPostTraining,
  TmBcomTraining,
  TmBcomTrainingExit,
  TmMre,
  TmMreExit,
  TmMrd,
  TmMrdExit,
  TmDwl,
  TmDwlExit,
  TmMwd,
  TmMwdExit,
  TmModeMax
} TRAINING_MODE;

/**

  Set the DDRIO training mode

  @param[in]     Socket           - Socket number
  @param[in]     Mode             - Training mode to set

  @retval N/A

**/
VOID
EFIAPI
SetTrainingModeDdr5 (
  IN      UINT8           Socket,
  IN      TRAINING_MODE   Mode
  );

/**

  Enable/Disable Write Level training mode in the DDRIO.

  @param[in] Socket  - Socket number
  @param[in] Enable  - TRUE to enable, FALSE to disable

  retval N/A

**/
VOID
EFIAPI
SetWlTrainingMode (
  IN UINT8    Socket,
  IN BOOLEAN  Enable
  );

//
// Buffer to save/restore write latency timings during PDA/PBA enumartion
//
typedef struct {
  UINT32    Timings[SUB_CH];
} WRITE_TIMINGS;

/**
  Program JEDEC timing registers

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Socket number
  @param[in] Ch                   - Channel number
  @param[in] OriginalWriteTimings - Pointer to saved timing values
                                    Will be ignored if NULL

  @retval N/A
**/
VOID
EFIAPI
ProgramJedecTimingsDdrio (
  IN  PSYSHOST        Host,
  IN  UINT8           Socket,
  IN  UINT8           Ch,
  IN  WRITE_TIMINGS   *OriginalWriteTimings   OPTIONAL
  );

VOID
SetCwlAdjDdrio (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     SetType,
  INT16     CwlAdj
  );

VOID
GetCwlDdrDqsAdjDdrio (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SetType,
  OUT INT16   *CwlAdj,
  OUT INT16   *DdrDqsAdj
  );

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
RestoreCwlForSpid (
  PSYSHOST  Host,
  UINT8     Socket
  );


UINT32
GetLongPreambleDdrio (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch
  );

/**

  Program registers to enable or disable TxOn for all strobes in a channel for DDR5

  @param[in]  Host        - Ptr to SYSHOST, system Host (root) structure
  @param[in]  Socket      - Socket number
  @param[in]  Ch          - Channel number
  @param[in]  SubCh       - Sub Channel number
  @param[in]  Strobe      - Strobe number
  @param[in]  TxOn        - TxOn value to program

  @retval None

**/
VOID
SetDataTxOnDdrioDdr5 (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     SubChMask,
  IN   UINT8      Strobe,
  IN    UINT32    TxOn
  );

/**

  This routine gets memory ready to be written and read

  @param[in]  Host          - Pointer to sysHost
  @param[in]  Socket        - Processor to initialize
  @param[in]  Ch            - Channel number
  @param[in]  SubCh         - Sub channel number
  @param[out] IoLatIoComp   - Pointer to caller's storage
  @param[in]  Mode          - Bit field of access modes

  @retval N/A

**/
VOID
GetIoLatencyIoComp (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT32   *IoLatIoComp,
  IN UINT8    Mode
  );

/**

  Program the Fast Boot timings

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param ch      - Channel number (0-based)
  @param dimm    - Current dimm
  @param rank    - Current rank (0-based)
  @param strobe  - Strobe number (0-based)

  @retval None

**/
VOID
WriteFastBootTimings (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Current rank (0-based)
  UINT8       strobe                            // Strobe number (0-based)
  );

/**

  Restore DDRIO timing parameters for fast boot or resume paths

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Processor to initialize
  @param[in]  Ch      - Channel number

  @retval N/A

**/
VOID
SetRestoreTimingsDdrio (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch
  );

/**

  This function applies RfOn setting to given channel.

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param ch      - Channel number (0-based)
  @param rfOn    - rfOn setting to apply

@retval previous value

**/
UINT32
SetRfOn (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT32      rfOn                              // rfOn setting to apply
  );

/**

  This function sets the rank type as DDRT the default is DDR4.
  This function is called only when the dimm is DDRT, this
  configuration will allow the design to select which set of
  timing and delay registers the rank is affected by

  @param  Socket Socket number
  @param  Ch     Channel number
  @param  Dimm   Dimm number

  @retval MRC_STATUS

**/
MRC_STATUS
SetDdrioRankTypeDdrt (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm
  );

/**

This function Asserts or DeAsserts DramReset bit in DDRIO

@param  Socket         Socket number
@param  ChannelBitMask Bitmask of channels to reset
@param  IsAssert       Assert\Deassert for Dram Reset Bit

@retval MRC_STATUS

**/
MRC_STATUS
SetDramResetN (
  IN UINT8     Socket,
  IN UINT32    ChannelBitMask,
  IN BOOLEAN   IsAssert
  );

/**

  Initialize VSSHIORVREFCONTROL

  @param[in] Host  - Pointer to sysHost
  @param[in] socket  - Processor to initialize

  @retval N/A

**/
VOID
InitVssHiControlDdrio (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket
  );

/**

  DDRIO setup for C/A Parity error flow

  @param[in] Host     - Pointer to sysHost
  @param[in] socket   - Processor to initialize
  @param[in] Ch       - Channel number

  @retval N/A

**/
VOID
SetupCapWrCrcErrorFlowDdrio (
  IN      PSYSHOST    Host,
  IN      UINT8       Socket,
  IN      UINT8       Ch
  );

/**

  Indicates if internal RCD VrefCA source is enabled

  @retval TRUE  - Internal RCD VrefCa source used
          FALSE - External RCD VrefCa source used

**/
BOOLEAN
InternalRcdVrefCaEnabled (
  VOID
  );

typedef enum {
  DataCompRCompDrvUp = 0,
  DataCompRCompDrvDn,
  DataCompSComp,
  DataCompTcoComp,
  DataCompVtComp,
  DataCompPanicDrvUp,
  DataCompPanicDrvDn,
  DataCompOdtUp,
  DataCompOdtDn,
  DataCompMax
} DATA_COMP_PARAM;

/**
  Return Data Comp value for specified parameter

  @param[in]  Host            - Pointer to SysHost structure
  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number
  @param[in]  Strobe          - Strobe number
  @param[in]  DataCompParam   - The specific data comp parameter
  @param[in]  DataCompValue   - Pointer to caller's storage for the value to be read

  @retval EFI_SUCCESS if value for parameter is returned
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetDataCompValue (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      UINT8             Ch,
  IN      UINT8             Strobe,
  IN      DATA_COMP_PARAM   DataCompParam,
  OUT     UINT32            *DataCompValue
  );

typedef struct {
  UINT32  DrvUp;
  UINT32  DrvDn;
  UINT32  SComp;
  UINT32  TcoComp;
} CMD_CTL_CLK_COMP;

typedef enum {
  ClkCompGroup = 0,
  CmdNCompGroup,
  CmdSCompGroup,
  CmdCkeCompGroup,
  CmdCtlCompGroup,
  CtlNCompGroup,
  CtlSCompGroup,
  CtlCkeCompGroup,
  CtlCtlCompGroup,
  CmdCtlClkCompGroupMax
} CMD_CTL_CLK_COMP_GROUP;

/**
  Return Comp values for the specified CMD/CTL/CLK group

  @param[in]  Host            - Pointer to SysHost structure
  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number
  @param[in]  CompGroup       - The specific cmd/ctl/clk comp group
  @param[in]  CompValues      - Pointer to caller's CMD_CTL_CLK_COMP structure

  @retval EFI_SUCCESS if value for parameter is returned
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetCmdCtlClkCompValue (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Ch,
  IN      CMD_CTL_CLK_COMP_GROUP  CompGroup,
  OUT     CMD_CTL_CLK_COMP        *CompValues
  );

/**
  Set/Restore Long Read Preamble in DDRIO

  @param[in] Host         - Point to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number
  @param[in] Set          - if TRUE: Set Long Read Preamble
                            else:    Restore original Read Preamble state

  @retval N/A
**/
VOID
SetLongReadPreambleDdrio (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      BOOLEAN  Set
  );

/**

  Return the DDRT DDRIO CLK number value.
  This function is called only when the dimm is DDRT, Current DDRT
  ERID is either uses CLK2 or CLK3,  this configuration will design to return
  the corresponding DIMM using clock number by input Dimm number.

  @param[in]  Host            - Pointer to SysHost structure
  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number
  @param[in]  Dimm            -Dimm number

  @retval DDRIO Clock number

**/
UINT8
GetDdrioClkNumberDdrt (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Ch,
  IN      UINT8                   Dimm
  );

/**
  Disables channels in DDRIO hardware that are unsupported by the platform

  @param[in]  Socket    Socket number to disable unused channels for

  @retval EFI_SUCCESS   Unsupported channels successfully disabled
  @retval !EFI_SUCCESS  An error occurred during the disabling of unsupported channels
**/
EFI_STATUS
EFIAPI
DisableUnsupportedChannelsDdrio (
  IN  UINT8   Socket
  );

/**
  Disables the specified channels in the DDRIO

  @param[in]  Socket        Socket number
  @param[in]  McId          Memory controller ID
  @param[in]  ChDisableMask Bit mask indicating which channels on memory controller McId to disable
                            Bit 0 = McId CH0, Bit 1 = McId CH1, etc

  @retval   EFI_SUCCESS   Operation was successful
  @retval   EFI_INVALID_PARAMETER if input is invalid

**/
EFI_STATUS
EFIAPI
DisableChannelsDdrio (
  IN  UINT8     Socket,
  IN  UINT8     McId,
  IN  UINT32    ChDisableMask
  );


/**

  Get/Set FIFO Point Control bit

  @param [in]       Host        - Pointer to sysHost
  @param [in]       Socket      - Socket number
  @param [in]       Ch          - Channel number
  @param [in]       Mode        - GSM_READ_ONLY - Do not write
                                - GSM_FORCE_WRITE - Force the write
  @param [in,out]   Value       - IN this value is used to set/clear fifoptctl bit
                                - OUT contains the current value of the fifoptctl bit

  @retval EFI_SUCESS

**/
EFI_STATUS
GetSetFifoPointControl (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    Mode,
  IN OUT  UINT8    *Value
  );

/**

  Set the RCD Host Interface Training Feedback selection.

  @param[in]  Host            - Pointer to SysHost structure
  @param[in]  Socket          - Socket number
  @param[in]  Dimm            - Dimm number
  @param[in]  Rank            - Rank number
  @param[in]  PatternLoop     - Pattern loop number

  @retval N/A

**/
VOID
EFIAPI
SetRcdTrainFeedbackSelection (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Dimm,
  IN      UINT8                   Rank,
  IN      UINT8                   PatternLoop
  );

/**

  Set the RCD Host Interface Training Feedback selection per channel

  @param[in]  Host            - Pointer to SysHost structure
  @param[in]  Socket          - Socket number
  @param[in]  Channel         - Channel number
  @param[in]  Dimm            - Dimm number
  @param[in]  Rank            - Rank number
  @param[in]  PatternLoop     - Pattern loop number (This parameter configure the sample_sel to count 0 or 1)

  @retval N/A

**/
VOID
EFIAPI
SetRcdTrainFeedbackSelectionByChannel (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Channel,
  IN      UINT8                   Dimm,
  IN      UINT8                   Rank,
  IN      UINT8                   PatternLoop
  );

/**

DDRIO Init for CS CLK Training.

@param host      - Pointer to sysHost
@param socket    - Current Socket

@retval N/A

**/
VOID
EFIAPI
CsClkDdrioInit (
  PSYSHOST  Host,
  UINT8     Socket
  );

/**

  Configure QCA training feedback sample selection based on training pattern mode

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number (0-based)
  @param[in] Dimm    - DIMM number (0-based)
  @param[in] Rank    - Rank number (0-based)

  @retval N/A

**/
VOID
EFIAPI
ConfigQcaSampleSelection (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 Rank
  );

/**

  DDRIO Init for QCS CLK Training for all channels for a give Dimm.

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Dimm      - Dimm number

  @retval N/A

**/
VOID
EFIAPI
QcsQcaClkDdrioInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Dimm
  );

/**

  DDRIO Init for QCS CLK Training for a channel

  @param[in] Socket    - Current Socket
  @param[in] Channel   - Channel
  @param[in] Dimm      - Dimm number

  @retval N/A

**/
VOID
EFIAPI
QcsQcaClkDdrioInitByChannel (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm
  );

/**

DDRIO Init in Pretraining.

@param host      - Pointer to sysHost
@param socket    - Current Socket

@retval N/A

**/
VOID
EFIAPI
PretrainingDdrioInit (
  PSYSHOST  Host,
  UINT8     Socket
  );

/**
  Configure TX UI (Transmit Unit Interval).

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number

  @retval EFI_SUCCESS
**/
VOID
ConfigureTxUi (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket
  );

/**

  This function enforces a "fence" for posted CSR writes to DDRIO registers in given MEM SS

  @param Host    - Pointer to sysHost
  @param Socket  - Processor to initialize
  @param McId    - Memory controller

  @retval N/A

**/
VOID
FencePostedCsrWritesDdrio (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     McId
  );


/**

  Issue a DDRIO COMP cycle at specific points in training

  @param[in] Socket   - Socket number
  @param[in] Option   - Options to specific specific points in training
                        TRAINING_COMP_MMRC_INIT
                        TRAINING_COMP_EVERY_JEDEC_INIT
                        TRAINING_COMP_EVERY_TRAINING

  @retval MRC_STATUS_SUCCESS   COMP cycle successfully completed
  @retval !MRC_STATUS_SUCCESS  COMP cycle was not successful

**/
MRC_STATUS
EFIAPI
DoCompDdrioPerSetupOption (
  IN  UINT8     Socket,
  IN  UINT8     Option
  );

typedef enum {
  CmdCtlOutputEnableAll,    ///< Enable all CMD CTL pins
  CmdCtlOutputDisableCmd,   ///< Disable All CMD pins
  CmdCtlOutputGet,          ///< Retrieve current CMD/CTL pin output enable state
  CmdCtlOutputRestore,      ///< Restore CMD/CTL pin enable states from input buffer
  CmdCtlOutputMax           ///< Maximum number of elements in this enum
} CMD_CTL_OUT_EN;

//
// CMD/CTL output states sometimes need to be saved and later restored. Use a UINT64 as a buffer for the callers of
// CmdCtlOutputEnable () to allocate and provide.
//
typedef UINT64 CMD_CTL_OUTPUT_BUFFER;

/**
  Program Cmd Ctl Output Enable

  This function is used to enable or disable (tristate) output pins in the DDRIO on the CMD/CTL busses.

  @param[in]              Host    Pointer to sysHost
  @param[in]              Socket  Current socket
  @param[in]              Channel Channel to program registers for
  @param[in]              Command Indicates whether to get, enable, or disable output and on which signals
  @param[in, out] Buffer  Buffer to store current enable settings to restore later

  @retval N/A
**/
VOID
CmdCtlOutputEnable (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Channel,
  IN      CMD_CTL_OUT_EN          Command,
  IN OUT  CMD_CTL_OUTPUT_BUFFER   *Buffer
  );

/**

Training Mode Selection. It supports CS/CA CLK training.

@param[in]  MEMFLOWS       - Training Step
@param[out] TrainingMode   - Training Mode. 1: In parallel. 0: In serial.

@retval MRC_STATUS_SUCCESS  - Select training mode Successfully
        MRC_STATUS_FAILURE  - Fail to set training mode

**/
MRC_STATUS
EFIAPI
TrainingModeSelect (
  IN     MEMFLOWS   Trainingstep,
     OUT UINT8      *TrainingMode
  );

/**

  Gets the maximum latency of DDRIO write plus read per channel. This value is used
  by the MC as a delay between write and read commands that hit a PCLS line.

  @param[in]  Socket         - Socket index at system level
  @param[in]  Ch             - Channel index at socket level
  @param[out] *Delay         - DDRIO write plus read latency in DCLK units

  @retval EFI_SUCCESS        - Delay value was updated
  @reval  EFI_UNSUPPORTED    - Delay value not updated

**/
EFI_STATUS
EFIAPI
GetPclsCaptureDelayDddrio (
  UINT8   Socket,
  UINT8   Ch,
  UINT32  *Delay
  );

/*

  Enforces the maximum gap between bits within the BYTE, and if enabled also enforces the nibbles to be in the same TxDq UI
  Warning: This function must be called during 80b mode or Basic training execution

  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Ch        Current channel under test (0-based)
  @param[in]  SubCh     Current subchannel under test (0-based)
  @param[in]  Dimm      Current dimm under test (0-based)
  @param[in]  Rank      Current rank under test (0-based)
  @param[in]  PiPosition  Pointer with the information of each center position across ALL bits
  @param[in]  ResultsBit  Pointer with the margining information of each bit
  @param[in]  UpdateTxDqsOriginalValue  TRUE: After executing the WA (*ChannelNvList)[ChannelLocal].TxDqsDelay is updated with the new value
                                        FALSE: (*ChannelNvList)[ChannelLocal].TxDqsDelay is not modified

*/
VOID
TxDqWA (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     SubCh,
  UINT8     Dimm,
  UINT8     Rank,
  PerBitPi  *PiPosition,
  IN struct bitMarginCh  *ResultsBit,
  BOOLEAN   UpdateTxDqsOriginalValue
  );

/*

  Enforces the maximum gap between bits within the BYTE, and if enabled also enforces the nibbles to be in the same TxDq UI
  Warning: This function must be called during 40b mode or Adv training execution

  @param[in]  Socket    Current socket under test (0-based)
  @param[in]  Ch        Current channel under test (0-based)
  @param[in]  SubCh     Current subchannel under test (0-based)
  @param[in]  Dimm      Current dimm under test (0-based)
  @param[in]  Rank      Current rank under test (0-based)
  @param[in]  UpdateTxDqsOriginalValue  TRUE: After executing the WA (*ChannelNvList)[ChannelLocal].TxDqsDelay is updated with the new value
                                        FALSE: (*ChannelNvList)[ChannelLocal].TxDqsDelay is not modified

*/
VOID
TxDqWaAdv (
  IN        UINT8     Socket,
  IN        UINT8     Ch,
  IN        UINT8     SubCh,
  IN        UINT8     Dimm,
  IN        UINT8     Rank,
  IN        BOOLEAN   UpdateTxDqsOriginalValue
  );

//
// Hook values used by MemPowerTrainingDdrioHook
//
typedef enum {
  TxSlewRatePreTest,
  TxSlewRatePostTest,
  PowerTrainHookMax
} POWER_TRAIN_HOOK;

/**
  Power Training hook router for MemDdrioIpLib
  General purpose API to handle special cases during power training

  @param[in]  Socket          - Socket number
  @param[in]  PowerTrainHook  - The hook identifier

  @retval  MRC_STATUS_SUCCESS if no failure
  @retval  MRC_STATUS_SIGNAL_NOT_SUPPORTED if hook is not supported
  @retval  !MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
MemPowerTrainingDdrioHook (
  IN  UINT8             Socket,
  IN  POWER_TRAIN_HOOK  PowerTrainHook
  );

/**

  Displays the training results

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure struct
  @param[in] socket - Socket Id

  @retval None

**/
VOID
EFIAPI
DisplayCCCResults (
  IN  PSYSHOST Host,
  IN  UINT8 Socket
  );

/**

  Clean up any chipset bugs related to ReadDqDqsPerBit training

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket - Socket Id

  @retval  MRC_STATUS_SUCCESS if no failure
  @retval  !MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
EFIAPI
ReadDqDqsCleanup (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket
  );

/**

  PXC Training

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure struct

  @retval  MRC_STATUS_SUCCESS if no failure
  @retval  !MRC_STATUS_SUCCESS otherwise

**/

UINT32
PXCTraining (
  IN PSYSHOST  Host
  );

/**
  This function is used to determine if the given margin parameter requires
  incremental stepping.

  @param[in]       MarginGroup        Margin group.

  @retval  TRUE   Incremental stepping is required.
  @retval  FALSE  Incremental stepping is not required.
**/
BOOLEAN
EFIAPI
IsIncrementalSteppingRequired (
  IN MRC_GT MarginGroup
  );

/**
  This function is used to determine whether the given margin group is for
  control signals.

  @param[in]  MarginGroup  Margin parameter group.

  @retval TRUE/FALSE
**/
BOOLEAN
EFIAPI
IsCtlMarginGroup (
  IN MRC_GT MarginGroup
  );

/**
  This function is used to determine whether the given margin group is for
  command/address signals (as opposed to data signals).

  @param[in]  MarginGroup  Margin parameter group.

  @retval TRUE/FALSE
**/
BOOLEAN
EFIAPI
IsCmdMarginGroup (
  IN MRC_GT MarginGroup
  );

/**
  Apply safe but non-optimal Rx data settings

  @param[in] Socket   - Socket number

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
ApplyRxDqsSafeSettings (
  IN  UINT8     Socket
  );

/**

  DDRIO Init for BCOM CLK Training.

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket

  @retval N/A

**/
VOID
EFIAPI
BcomDdrioInit (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**

DDRIO programming for after BCOM CLK Training.

@param[in] Host    - Pointer to sysHost
@param[in] Socket  - Current Socket

@retval N/A

**/
VOID
EFIAPI
PostBcomDdrioProgramming (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**

  DDRIO initialization for Backside Receive Enable (MRE) training.

  @param[in]  Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
MreDdrioInit (
  IN  UINT8  Socket
  );

/**

  DDRIO programming after MRE training has been completed.

  @param[in] Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
PostMreDdrioProgramming (
  IN UINT8  Socket
  );

/**

  DDRIO initialization for Backside Read Delay (MRD) training.

  @param[in]  Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
MrdDdrioInit (
  IN  UINT8  Socket
  );

/**

  DDRIO programming after MRD training has been completed.

  @param[in] Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
PostMrdDdrioProgramming (
  IN UINT8  Socket
  );

/**

  DDRIO initialization for Backside DWL training.

  @param[in]  Socket             - Current socket

  @retval N/A

**/
VOID
EFIAPI
DwlDdrioInit (
  IN UINT8  Socket
  );

/**

  DDRIO programming after DWL training has been completed.

  @param[in] Socket             - Current socket

  @retval N/A

**/
VOID
EFIAPI
PostDwlDdrioProgramming (
  IN UINT8  Socket
  );

/**

  DDRIO initialization for Backside Write Delay (MWD) training.

  @param[in]  Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
MwdDdrioInit (
  IN  UINT8  Socket
  );

/**

  DDRIO programming after MWD training has been completed.

  @param[in] Socket  - Current socket

  @retval N/A

**/
VOID
EFIAPI
PostMwdDdrioProgramming (
  IN UINT8  Socket
  );

/**

  Displays the Xover results.

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor socket

  @retval N/A

**/
VOID
DisplayXoverResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**
  Program JEDEC timing registers to meet the DDR5 PDA/PBA timing requirements.

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Socket number
  @param[in] Channel              - Channel number
  @param[in] OriginalWriteTimings - Pointer to saved timing values

  @retval N / A
**/
VOID
ProgramPdaPbaTimingValues (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Channel,
  IN WRITE_TIMINGS    *OriginalWriteTimings   OPTIONAL
  );

/**
  Initialize Tx Echo Cancellation feature.

  @param[in] Socket   - Socket number

  @retval  N/A
**/
VOID
EFIAPI
InitTxEchoCancellation (
  IN UINT8 Socket
  );

/**
  Initialize Legacy RxDfe feature.

  @param[in] Socket   - Socket number

  @retval  N/A
**/
VOID
EFIAPI
InitLegacyRxDfe (
  IN UINT8 Socket
  );

/**
  Do not allows Req and Err pin to propagate to MC IP.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket

  @retval N/A

**/
VOID
ProgramReqRxPathGate (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**
Allows Req and Err pin to propagate to MC IP.

@param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
@param[in]  Socket      - Processor socket within the system (0-based)
@param[in]  Channel     - Channel within the Socket

@retval N/A

**/
VOID
ProgramReqRxPathUngate (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**

  Initialize the DDRIO NVDATA structure
  Called early in RC, before data is restored from NVRAM storage

  @retval EFI_BUFFER_TOO_SMALL if not enough memory allocated for the buffer
          EFI_UNSUPPORTED if the function is not implemented
          EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
InitDdrioDataStructure (
  VOID
  );

/**

  Ensure TX DQS delays in a byte are within MAX_DQ_GAP ticks of each other

  @param[in] Socket     - Socket number

  @reval    N/A

**/
VOID
TxDqsWA (
  UINT8     Socket
  );

/**

  Configure Rx Path Ungate.

  @param[in] Socket - Socket Id
  @param[in] Value  - Value to program

  @retval  None

**/
VOID
EFIAPI
ConfigureRxPathUngate (
  IN UINT8                     Socket,
  IN UINT8                     Value
  );

/**

  Gets the max DCA retrain loop count.

  @param VOID

  @retval MAX DCA Loop Count

**/
UINT8
EFIAPI
GetMaxDcaRetrainLoopCount (
  VOID
  );

/**

  Check whether the rank needs to be trained or not

  @param[in] Socket - Socket Id
  @param[in] Ch     - Ch Id
  @param[in] Dimm   - Dimm number
  @param[in] Rank   - Rank number
  @param[in] Mode   - Mode: DONT_CHECK_MAPOUT or CHECK_MAPOUT
  @param[in] DimmNvList - DimmNvList pointer

  @retval  TRUE or FALSE

**/
BOOLEAN
EFIAPI
CheckRankNotToTrain (
  IN UINT8                     Socket,
  IN UINT8                     Ch,
  IN UINT8                     Dimm,
  IN UINT8                     Rank,
  IN UINT8                     Mode,
  IN struct                    dimmNvram (*DimmNvList) [MAX_DIMM]
  );

/**

  Program Read Preamble setting.

  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
EFIAPI
ProgramReadPreamble (
  UINT8    Socket
  );

/**

  Restore Write Preamble Setting after write leveling training.

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
EFIAPI
RestoreWritePreambleWriteLeveling (
  PSYSHOST Host,
  UINT8    Socket
  );

/**
  Normalize Roundtrip and IO Latency

  @param Socket  - Processor socket number

  @retval None

**/
VOID
EFIAPI
NormalizeRoundTripAndIoLatency (
  IN UINT8     Socket
  );

/**
  Average Receive Enable delays across ranks

  @param[in] Socket  - Socket number

  @retval None

**/
VOID
EFIAPI
AverageRecEnableDelay (
  IN UINT8     Socket
  );

/**
  Enable Rx Dfe

  @param[in] Socket  - Socket number

  @retval None

**/
VOID
EFIAPI
EnableRxDfe (
  IN UINT8     Socket
  );

/**
  Program Rx Gain Bias

  @param[in] Socket           - Socket number
  @param[in] RxGainBiasValue  - Rx Gain Bias Value to be programmed

  @retval None

**/
VOID
EFIAPI
ProgramRxGainBias (
  IN UINT8     Socket,
  IN UINT8     RxGainBiasValue
  );

/**
  Initializes all instances of VssHiOrVrefControl.vsshiorvrefctl

  The default programming of VssHiOrVrefControl.vsshiorvrefctl is required in all CH0 FUB instances of VsshiOrVrefControl
  because BIT13 gates CaVref controls on other channels. This programming cannot always be done in MMRC because MMRC does
  not program unpopulated channels. This routine programs the default into all instances of this register, some of which
  may be overwritten later as necessary by MMRC or training.

  @param[in]  Host    Pointer to sysHost
  @param[in]  Socket  Current socket
**/
VOID
InitVrefControls (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  );

/**

  Get the IO Latency Comp Start Value

  @retval IO Lat Comp Start Value

**/
UINT8
EFIAPI
GetIoLatCompValueStart (
  VOID
  );

/**

  Get the IO Latency Comp Value

  @retval IO Lat Comp Value

**/
UINT8
EFIAPI
GetIoLatCompValue (
  VOID
  );

/**

DDRIO Init for Tx ReTraining.

@param[in] socket             - Current Socket
@param[in] TxReTrainDuration  - Tx ReTrain Duration Value
@param[in] TxReTrainInit      - Tx ReTrain Init Value

@retval N/A

**/
VOID
EFIAPI
ConfigureDdrioTxRetrain (
  IN UINT8     Socket,
  IN UINT8     TxReTrainDuration,
  IN UINT8     TxReTrainInit
  );

/**

Configure the Tx ReTrain Enable bit in DDRIO.

@param[in] socket    - Socket Number
@param[in] Ch        - Ch number
@param[in] Dimm      - Dimm number
@param[in] Rank      - Rank number
@param[in] Value    -  Value to program

@retval N/A

**/
VOID
EFIAPI
ConfigureDdrioTxRetrainEnable (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Value
  );

/**

Apply the DDRIO Phy Update in Tx ReTraining.

@param[in] Socket    - Current Socket
@param[in] Ch        - Ch Number
@param[in] Dimm      - Dimm Number
@param[in] Rank      - Rank Number

@retval N/A

**/
VOID
EFIAPI
TxReTrainDdrioPhyUpdate (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**
  Displays the Sense Amp register settings

  @param[in] socket:       Processor socket

  @retval N/A
**/
VOID
DisplaySenseAmpTrainingResults (
  IN  UINT8     Socket
  );

/**

  This function implements Sense Amp Offset training.
  SenseAmp offset cancellation

  @param Host    - Pointer to sysHost

  @retval SUCCESS  - Sense Amp Offset training succeded
  @retval !SUCCESS - Sense Amp Offset training failed

**/
UINT32
EFIAPI
SenseAmpOffset (
  PSYSHOST  Host
  );

/**

  Save the dataOffsetTrain value for subsequent boots.

  @param[in]  Host        - Pointer to sysHost

  @retval None

**/
VOID
EFIAPI
DataOffsetTrainSave (
  IN PSYSHOST          Host
  );

/**

  Return Override Read Preamble timing setting

  @param[in] Socket         - Socket number
  @param[in] PreambleType   - READ_PREAMBLE

  @retval PREAMBLE_1TCLK, PREAMBLE_2TCLK, PREAMBLE_3TCLK, PREAMBLE_4TCLK

**/
UINT8
EFIAPI
GetReadPreambleOverride (
  IN      UINT8   Socket,
  IN      UINT8   ReadPreamble
  );

/**

  Equalize CWL value between the two sub channels of one rank.

  @param[in] Host                 - Pointer to sysHost
  @param[in] Socket               - Current Socket
  @param[in] Ch                   - Channel number
  @param[in] Dimm                 - Dimm number
  @param[in] Rank                 - Rank number
  @param[in] MaxStrobe            - Maximum number of strobes
  @param[in] CwlAdj               - Cwl Adjust
  @param[in] WLChipCleanUpStruct  - WL Chip Clean Up Struct
  @param[in] TxDqsDelayValue      - The array is used to store the TxDqs delay value.

  @retval N/A

**/
VOID
EFIAPI
EqualizeCwl (
  IN  PSYSHOST                          Host,
  IN  UINT8                             Socket,
  IN  UINT8                             Ch,
  IN  UINT8                             Dimm,
  IN  UINT8                             Rank,
  IN  UINT8                             MaxStrobe,
  IN  INT16                             CwlAdj[MAX_CH][SUB_CH][MAX_RANK_CH],
  IN  CHIP_WL_CLEANUP_STRUCT            *WLChipCleanUpStruct,
  IN  INT16                             TxDqsDelayValue[MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2]
  );

/**

  Equalize CWL value between the two sub channels for all the DIMMs and Ranks in this channel.

  @param[in] Host                    - Pointer to sysHost
  @param[in] Socket                  - Current Socket
  @param[in] Ch                      - Channel number
  @param[in] WLChipCleanUpStruct     - WL Chip Clean Up Struct
  @param[in] CwlAdjTargetValueInput  - CWL target value

  @retval N/A

**/
VOID
EFIAPI
EqualizeCwlPerCh (
  IN  PSYSHOST                          Host,
  IN  UINT8                             Socket,
  IN  UINT8                             Ch,
  IN  CHIP_WL_CLEANUP_STRUCT            *WLChipCleanUpStruct,
  IN  INT16                             CwlAdjTargetValueInput[SUB_CH]
  );

/**

  Shift CK delays to prepare for Average Receive Enable step2

  @param[in] Socket   - Processor socket number
  @param[in] Ch       - Channel number
  @param[out] CkOffset - Pointer to CK PI offset value to update

  @retval None

**/
VOID
EFIAPI
ReceiveEnableAveragePart2a (
  IN UINT8     Socket,
  IN UINT8     Ch,
  OUT INT16     *CkOffset
);

/**
  Program Rx Analog En Early Value

  @param[in]  Socket    Socket number

  @retval MRC_STATUS_SUCCESS   Register offset was successfully determined and programmed
  @retval !MRC_STATUS_SUCCESS  Register offset was not successfully determined and not programmed

**/
MRC_STATUS
EFIAPI
ProgramRxAnalogEnEarly (
  IN      UINT8   Socket
  );

/**
  GetLinearCntlTcoValue
    TCO settings to monotonically increase duty cycle adjustments are [95 - 65][0 - 31]
    This function takes the current CSR value and returns a value that is normalized
    to 0 and monotonically increasing.

    For example:
    input values  95, 94, 93,..., 65,  0,  1,  2,..., 31
    correspond to
    output values  0,  1,  2,..., 30, 31, 32, 33,..., 63

  @param[in]  CsrSetting - Setting based on the CSR value

  @retval Correponding Linear Control value

**/
INT16
EFIAPI
GetLinearCntlTcoValue (
  IN UINT32  CsrSetting
  );

/**
  GetIpTcoSetting
    TCO settings to monotonically increase duty cycle adjustments are [95 - 65][0 - 31]
    This function undoes what GetLinearCntlTcoValue does.  It takes a normalized
    monotonically increasing value and encodes it based on the IP specific value.

    For example:
    input values    0,  1,  2,..., 30, 31, 32, 33,..., 63
    correspond to
    output values  95, 94, 93,..., 65,  0,  1,  2,..., 31

  @param[in]  LinearValue - Normalized Linear Control value

  @retval Correponding Setting based on the CSR definition

**/
UINT32
EFIAPI
GetIpTcoSetting (
  IN INT16  LinearValue
);

/**

  Get the ANALOG_TUNING Value

  @retval analog_tuning Value

**/
UINT8
EFIAPI
GetAnalogTuningValue (
  IN      PSYSHOST                Host,
  IN      UINT8                   Socket,
  IN      UINT8                   Channel
  );

#endif   // #ifndef  _MEM_DDRIO_IP_LIB_H_
