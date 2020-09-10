/** @file
  MemMcIpLib.h

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

#ifndef  _MEM_MC_IP_LIB_H_
#define  _MEM_MC_IP_LIB_H_

#include "SysHost.h"
#include <ReferenceCodeDataTypes.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <MemCommon.h>
#include <Protocol/CpuCsrAccess.h>
#include <MemDecodeCommonIncludes.h>
#include <Memory/CpgcDefinitions.h>
#include <Library/Ddr5CoreLib.h>

//
// in-band SMI
//
#define DDRT_SIGNAL_SMI_FLAG                BIT0

//
// ERROR0# pin signaling
//
#define DDRT_SIGNAL_ERROR0_FLAG                  BIT1

//
// Low Priority event
//
#define DDRT_SIGNAL_LOW_PRIORITY_EVENT      0

//
// High Priority event
//
#define DDRT_SIGNAL_HIGH_PRIORITY_EVENT     1

//
// Number of priority levels
//
#define DDRT_SIGNAL_PRIORITY_EVENT_NUM      2

//
// MC IP interface
//
#define DEFEATURES1_M2MEM_EGRECO_IGNORE_DIR_STATE   BIT4

//
// Optional dimm parameter value
//
#define NO_DIMM_OPTION                    0

//
// Read operation define
//
#define READ_THROTTLE_VALUES TRUE
//
// Write Operation define
//
#define WRITE_THROTTLE_VALUES FALSE

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Log : 24;        // FNV event log
    UINT32 Overflow : 1;    // FNV event overflow
    UINT32 ErrorId  : 3;    // WA FNV event error id
    UINT32 Reserved : 4;
  } Bits;
  UINT32 Data;

} DDRT_EVENT_DATA;

///
/// Memory thermal feature type
///
typedef enum {
  ConfigureMemHotFeatureType = 0,          ///< Memhot Feature type
  ConfigureMemTripFeatureType,             ///< Memtrip feature type
  ConfigurePcuBiosSpare2RegType,           ///< Bios spare register
  ConfigureThermFeatureMaxType             ///< Enum limit to check valid value
} CONFIGURE_MEM_THERMAL_FEATURE_TYPE;

//
// ODT Assertion selectors
//
typedef enum {
  SameRank = 0,
  DiffRank = 1,
  DiffDimm = 2,
  } MRC_ODTASSERTION_SELECTION;

///
/// Memory thermal management register field enum type
///
typedef enum {
  EnumBwLimitThrottleEnable = 0,               ///< Bandwith limit throttle enable.
  EnumOlttEnable,                              ///< Enable OLTT temperature tracking
  EnumThrottleExternal,                        ///< Max number of throttled transactions to be issued during BWLIMITTF due to externally
                                               ///< asserted MEMHOT#.
  EnumThrottlingDimmTempOffsetValue,           ///< Temperature Offset value in degree celsius.
  EnumHalfxRefresh,                            ///< Enable Halfx refresh when temperature is above temp_halfxref
  EnumTwoxRefresh,                             ///< Enable 2X refresh when memory temperature is above TEMP_TWOXREF
  EnumFourxRefresh,                            ///< Enable 4x refresh when temperature is above temp_fourxref
  EnumHalfxRefreshValue,                       ///< Program Halfx refresh Value
  EnumTwoxRefreshValue,                        ///< Program 2X refresh Value
  EnumFourxRefreshValue,                       ///< Program 4x refresh Value
  EnumBandWidthThrottleWindowSize,             ///< BW Throttle Window Size in DCLK/8 (a value of 1 results in 8 DCLKs)
  EnumMemThrotAllowIsoch,                      ///< Throttle allow isochannel
  EnumTempThersholdLow,                        ///< Temperature thershold low value type
  EnumTempThersholdMid,                        ///< Temperature thershold medium value type
  EnumTempThersholdHigh,                       ///< Temperature thershold High value type
  EnumTempThrottleMid,                         ///< Temperature throttle mid value type
  EnumTempThrottleHigh,                        ///< Temperature throttle high value type
  EnumTempThrottleCritical,                    ///< Temperature throttle critical value type
  EnumTempThrottleHysteresis,                 ///< Temperature throttle hysteresis type
  EnumTempMemTrip,                             ///< Temperature memtrip
  EnumTempStatusCurrentDimmTemp,               ///< Current temperature of dimm in celcius
  EnumThrottleValueMax                         ///< Enum limit to check valid value
} THROTTLE_VALUE_TYPE;

///
/// Memory thermal management register field structure to pass values
///
typedef struct {
  BOOLEAN BwLimitThrottleEnable;           ///< Bandwith limit throttle enable.
  BOOLEAN OlttEnable;                      ///< Enable OLTT temperature tracking
  UINT8 ThrottleExternal;                  ///< Max number of throttled transactions to be issued during BWLIMITTF due to externally
                                           ///< asserted MEMHOT#.

  UINT8 ThrottlingDimmTempOffsetValue;     ///< Temperature Offset value in degree celsius.
  BOOLEAN HalfxRefresh;                    ///< Enable Halfx refresh when memory temperature is above TEMP_HALFXREF
  BOOLEAN TwoxRefresh;                     ///< Enable 2X refresh when memory temperature is above TEMP_TWOXREF
  BOOLEAN FourxRefresh;                    ///< Enable 4x refersh when memory temperature is above TEMP_FOURXREF
  UINT8 HalfxRefreshValue;                 ///< Program TEMP_HALFXREF Value (Value in Celcius)
  UINT8 TwoxRefreshValue;                  ///< Program TEMP_TWOXREF Value (Value in Celcius)
  UINT8 FourxRefreshValue;                 ///< Program TEMP_FOURXREF Value (Value in Celcius)
  UINT32 BandWidthThrottleWindowSize;      ///< BW Throttle Window Size in DCLK/8 (a value of 1 results in 8 DCLKs)
  BOOLEAN MemThrotAllowIsoch;              ///< Throttle allow isochannel
  UINT8 TempThersholdLow;                  ///< Temperature thershold low value in degree celsius
  UINT8 TempThersholdMid;                  ///< Temperature thershold medium value in degree celsius
  UINT8 TempThersholdHigh;                 ///< Temperature thershold High value in degree celsius
  UINT8 TempThrottleMid;                   ///< Temperature throttle mid value
                                           ///< Max number of throttled transactions (ACT, READ, WRITE) to be issued during BWLIMITTF.

  UINT8 TempThrottleHigh;                  ///< Temperature throttle high value
                                           ///< Max number of throttled transactions (ACT, READ, WRITE) to be issued during BWLIMITTF.

  UINT8 TempThrottleCritical;              ///< Temperature throttle critical value in degree celsius
                                           ///< Max number of throttled transactions (ACT, READ, WRITE) to be issued during BWLIMITTF.

  UINT8 TempThrottleHysteresis;            ///< Positive going Threshold Hysteresis Value from Dimm throttle register

  UINT8 TempMemTrip;                       ///< When memory temperature is above
                                           ///< or at this value the MEMTRIP pin
                                           ///< should assert and the platform
                                           ///< should shut down

  UINT8 TempStatusCurrentDimmTemp;         ///< Current temperature of dimm in celcius

} THROTTLE_VALUE;

//
// 2LM - cross channel system Mode
//
typedef enum {
  DdrIn2lmCrossCh = 0,  // Check if current channel is DDR5/NM and another channel is DDRT/FM
  DdrtIn2lmCrossCh = 1, // Check if current channel is DDRT/FM and another channel is DDR5/NM
  Max2lmCrossCh = 2
} MODE_2LM_CROSS_CH;

/**

  Check if the DDR5/NM or DDRT/FM channel is in 2LM - cross channel system (DDRT2/FM on one channel and DDR5/NM on another channel)

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket
  @param[in] Channel   - Current Channel
  @param[in] Mode      - Check DDR5/NM or DDRT/FM channel

  @retval TRUE   - DDR5/NM or DDRT/FM channel is in 2LM - cross channel system
  @retval FALSE  - DDR5/NM or DDRT/FM channel is not in 2LM - cross channel system

**/
BOOLEAN
IsChIn2lmCrossChannelSystem (
  IN PSYSHOST          Host,
  IN UINT8             Socket,
  IN UINT8             Channel,
  IN MODE_2LM_CROSS_CH Mode
  );

/**
  Set/Restore Long Read Preamble

  @param[in] Host         - Point to sysHost
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number
  @param[in] Set          - if TRUE: Set Long Read Preamble
                            else:    Restore original Read Preamble state

  @retval N/A
**/
VOID
SetLongReadPreamble (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      BOOLEAN  Set
  );

/**

  Initialize the DDR4 and DDRT scrambler config in the MC

  @param[in]  Host          - Point to sysHost
  @param[in]  Socket        - Socket number
  @param[in]  Ch            - Channel number
  @param[in]  Context       - Indicates what action to take

  @retval n/a

**/
VOID
SetScrambleConfigMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Context
  );

//
// Context definitions for SetScrambleConfigMc
//
#define SCRAMBLE_CONFIG_LATE_FNV_CONFIG   0
#define SCRAMBLE_CONFIG_EARLY_INIT        1

/**

  Enable DCLK in the MC

  @param MemTechType    - Mem technology type
  @param Socket         - Socket number

  @retval n/a

**/
VOID
EFIAPI
SetDclkEnableMc (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          Socket
  );

/**
  Build target function reset the Pmon unit ctrl counters.

  @param[in] MemTechType  Mem technology type.
  @param[in] Socket       Socket index
  @param[in] Ch           Ch index

  @retval EFI_SUCCESS     Completed successfully.
  @retval !EFI_SUCCESS    Error.
**/
EFI_STATUS
EFIAPI
ResetPmonUnitCtrlCounters (
  IN MEM_TECH_TYPE   MemTechType,
  IN UINT8           Socket,
  IN UINT8           Ch
  );

/**

  Disable Rcomp in the MC

  @param Host           - Point to sysHost
  @param Socket         - Socket number
  @param Mcid           - Memory controller

  @retval n/a

**/
VOID
SetRcompDisableMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Mcid
  );


/**

  Clear DDRT MC Req counter in the MC

  @param Host           - Point to sysHost
  @param Socket         - Socket number
  @param Ch             - Channel number
  @param Dimm           - Current dimm

  @retval n/a

**/
VOID
ClearDdrtReqCounterMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm
  );


/**

  Set timer for Idle Cmd to CKE low

  @param Host           - Point to sysHost
  @param Socket         - Socket number
  @param Ch             - Channel number

  @retval n/a

**/
VOID
SetIdleCmdToCkeLowMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/*
  Get the maximum number of data groups based on DIMM and group type

  @param[in]        Host      Pointer to sysHost
  @param[in]        socket    Socket
  @param[in]        ch        Channel
  @param[in]        dimm      DIMM
  @param[in]        group     Group type
  @param[in]        level     Level type

  @retval Max strobe

*/
UINT8
GetMaxStrobeWithinSubCh (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN MRC_GT    Group,
  IN MRC_LT    Level
  );

/**
  Check LPMODE entry latency value to search for constraints and fix them if present

  @param [in] Host   - Point to sysHost
  @param [in] Socket - Socket number
  @param [in] Ch     - Channel number

  @retval n/a
**/
VOID
CheckLpmodeEntryLatencyConstraints (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  );

/**
  Check DDRT CKE setting value to search for constraints and fix them if present

  @param [in] Host   - Point to sysHost
  @param [in] Socket - Socket number
  @param [in] Ch     - Channel number

  @retval n/a
**/
VOID
CheckTDdrtCkeConstraints (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  );

/**

  This resets the MC IO module

  @param Host    - Pointer to sysHost
  @param socket    - Socket number

  @retval N/A

**/
VOID
IO_Reset (
  PSYSHOST Host,
  UINT8    socket
  );


/**

  Set Open page mode in the MC

  @param Host           - Point to sysHost
  @param Socket         - Socket number
  @param Mcid           - Memory controller
  @param PageMode       - Closed or Open page mode

  @retval n/a

**/
VOID
SetPageModeMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Mcid,
  IN UINT8     PageMode
  );


/**

  Issue a comp cycle

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
VOID
DoComp (
  PSYSHOST Host,
  UINT8    socket
  );

/**

  This function applies a new PXC setup values(enables or disables the PXC training).

  @param[in]  Host           - Pointer to sysHost
  @param[in]  Socket         - Socket
  @param[in]  Ch             - Channel
  @param[in]  PxcEnDisable   - Enable/disable PXC

  @retval none

**/
VOID
PxcSetup (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     PxcEnDisable
  );


/**
  Calculate any additional delay for the DDRT CWL add value due to PXC enablement.

  @param[in]  Socket  The socket to test for PXC enablement

  @retval Any additional delay due to PXC enablement.
**/
UINT8
EFIAPI
CalculateDdrtCwlAddPxcAdder (
  IN  UINT8 Socket
  );

/**
  This function returns the first DDR channel found in the configuration

  @param[in    ] Socket - Socket number
  @param[   out] Ch     - Pointer to store the first DDR channel

  @retval TRUE if DDR channel was found
**/
BOOLEAN
FindFirstChannel (
  IN  UINT8 Socket,
  OUT UINT8 *Ch
  );

/*
  This function return the first DIMM found in the specified memory channel

  @param[in    ] Socket - Socket number
  @param[in    ] Ch     - Channel number
  @param[   out] Dimm   - Pointer to store the number of the first DIMM

  @retval TRUE if DIMM was found
**/
BOOLEAN
FindFirstDimmInChannel (
  IN     UINT8 Socket,
  IN     UINT8 Ch,
     OUT UINT8 *Dimm
  );

/**

  Verifies current population does not validate POR restrictions

  @param Host  - Pointer to sysHost

  @retval N/A

**/
UINT32
CheckPORCompat (
  PSYSHOST Host
  );

/**

  This function applies the given CKE override

  @param Host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel
  @param ckeMask    - CKE settings to apply

@retval none

**/
VOID
SetCkeOverride (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     ckeMask
  );

/**
  Checks if CKE signal needs to be driven on or not via CADB

  @param[in]  Host    Pointer to host structure
  @param[in]  Socket  Target socket
  @param[in]  Channel Target channel

  @retval TRUE if CKE signal needs to be driven on
          FALSE if CKE signal needs to be driven off
**/
BOOLEAN
ShouldDriveCkeOn (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**
  Initializes CMI Credit registers

  @param[in] Host     - Pointer to sysHost structure

  @retVal   1 CMI Registers initialized
            0 CMI Registers not initialized
**/
UINT32
EFIAPI
InitCmiCreditProgramming (
  IN      PSYSHOST Host
  );

/**
  Print memory performance settings

  @param Host   - Pointer to sysHost
  @param Socket - Socket value

  @retval N/A
**/
VOID
PrintMemPerformanceSettings (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**

  Set Directory Enable mode in the MC

  @param Socket         - Socket number
  @param Mcid           - Memory controller
  @param DirEn          - Directory Enable

  @retval n/a

**/
VOID
SetDirectoryEnableMc (
  IN UINT8     Socket,
  IN UINT8     Mcid,
  IN UINT32    DirEn
  );

/**

  This function gets the Target Socket and Channel interleave values

  @param Host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel
  @param *TargetIntlv
  @param *ChannelIntlv

@retval none

**/
VOID
GetNmCachingIntlvValues (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     *TargetIntlv,
  UINT8     *ChannelIntlv
  );

/**

  Set dis_isoch_rd and defeature_3 bits in MCDECS_CHICKEN_BITS

  @param Socket         - Socket number
  @param Ch             - Channel
  @param DisIsocRd      - TRUE if IOSC enable; FALSE otherwise
  @param Def3           - TRUE if Directory Mode feature is enabled; FALSE otherwise

  @retval n/a

**/
VOID
SetDisIsocRdDef3Mc (
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     DisIsocRd,
  IN UINT8     Def3
  );

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost

  @retval N/A

**/
UINT32
InitMem (
  PSYSHOST Host
  );

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
UINT32
EarlyConfig (
  PSYSHOST  Host
  );

/**

  Programs timing parameters

  @param[in] Socket      - Socket number

  @retval EFI_SUCCESS     Success
          !EFI_SUCCESS    Failed

**/
EFI_STATUS
EFIAPI
ProgramTimingsHbm (
  UINT8    Socket
  );

/**

  Program scheduler registers after training

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS     Success
          !EFI_SUCCESS    Failed

**/
EFI_STATUS
EFIAPI
SchedulerConfigLateHbm (
  IN    UINT8    Socket
  );

/**
  Sets new tREFI value (refresh interval) and returns previous value

  @param[in]  Host            Pointer to sysHost
  @param[in]  Socket          Socket number
  @param[in]  Ch              Channel number
  @param[in]  Trefi           New tREFI value
  @param[out] *PrevTrefi      Pointer to return previous tREFI value

  @reval      N/A
**/
VOID
GetSetTrefi (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT16   Trefi,
  OUT UINT16   *PrevTrefi
);

/**
  Enables all channels in memory controller registers

  @param[in]  Socket          Socket number

  @reval      EFI_SUCCESS     Channels successfully enabled
  @retval     !EFI_SUCCESS    An error occurred during channel enabling
**/
EFI_STATUS
EFIAPI
EnableAllChannelsMc (
  IN  UINT8 Socket
  );

/**
  Disables channels in MC hardware that are unsupported by the platform

  @param[in]  Socket    Socket number to disable unused channels for

  @retval     EFI_SUCCESS   Unsupported channels successfully disabled
  @retval     !EFI_SUCCESS  An error occurred during the disabling of unsupported channels
**/
EFI_STATUS
EFIAPI
DisableUnsupportedChannelsMc (
  IN  UINT8   Socket
  );

/**
  Disables unused channels in memory controller registers

  Note:

     There is a hardware requirement for channel0 to remain active if all channels on
     a memory controller are going to be disabled. If a channel disable mask is passed in that
     would cause all channels to be disabled, channel0 will be automatically enabled.

  @param[in]  Socket          Socket number
  @param[in]  McId            Memory controller ID
  @param[in]  ChDisableMask   Bitmask indicating which channels to disable.
                                Leave enabled = 0, disable = 1
                                Bit 0 = MC<McId> CH 0, Bit 1 = MC<McId> CH 1
                                Channel numbers are relative to the memory controller

  @retval     EFI_SUCCESS     Channels successfully disabled
  @retval     !EFI_SUCCESS    An error occurred during the channel disabling
**/
EFI_STATUS
EFIAPI
DisableChannelsMc (
  IN  UINT8     Socket,
  IN  UINT8     McId,
  IN  UINT32    ChDisableMask
  );

/**

  Initialize channel specific memory controller registers after training and before normal mode

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
LateConfig (
  PSYSHOST Host
  );

/**

  Initialize SVL and Scrmabling settings

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
SetupSVLandScrambling (
  PSYSHOST Host
  );

/**

  Switches to normal mode

  @param[in] Host    - Pointer to sysHost

  @retval SUCCESS    - Always returns SUCCESS status

**/
UINT32
SwitchToNormalMode (
  IN  PSYSHOST  Host
  );

/**

  Set CPL2 MRC - PCODE Handshake at end of MRC

  @param[in] Host    - Pointer to sysHost

  @retval SUCCESS - Always returns SUCCESS status.

**/

UINT32
EFIAPI
SetCpl2 (
  IN PSYSHOST  Host
  );

/**

  Wrapper function to enable 2N mode

  @param[in]    Host      SysHost structure pointer

  @retval       SUCCESS   Function executed completely (it does not necessarily
                          mean the mailbox command was sent properly)

**/
UINT32
Enable2nMode (
  IN        PSYSHOST          Host
  );

/**

    Initialize ADR

    @param Host - Pointer to SysHost

    @retval SUCCESS

**/
UINT32
InitADR (
  PSYSHOST  Host
  );

/**

  Programming required late in the memory initialization process

  @param Host  - Pointer to sysHost

  @retval N/A

**/
UINT32
MemLate (
  PSYSHOST Host
  );

/**

  Update MCDDRTCFG slot present

  @param Host  - Pointer to sysHost

  @retval VOID

**/
VOID
SetMcDdrtCfg (
  PSYSHOST  Host
  );

/**

  Programs the WrWr_s for ddrt2 ranks

  @param Host    - Pointer to sysHost
  @param Socket  - Processor to initialize
  @param Value   - Wr Wr S delay
  @retval N/A

**/

VOID
ProgramDdrt2WrWrS (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Value
  );

/**

  This routine gets the CSRs required by BiosServerGet3dsRankPerCs

  @param Host                       - Pointer to sysHost
  @param Socket                     - Socket Id
  @param ChannelInSocket            - Channel number (0-based)
  @param Dimm                       - Zero based Dimm number (0-based)
  @param RankCount                  - Pointer to where the rank count per chip select signal will be stored

  @retval N/A

**/
VOID
BiosServerGet3dsRankPerCsChip (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 ChannelInSocket,
  UINT8                 Dimm,
  UINT8                 *RankCount
  );

/**

  Set the CPGC transaction target is DDRT or DRAM scheduler.

  @param[in]                Socket              Socket Id
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                CpgcForDdrt         BOOLEAN TRUE: target is DDRT; FALSE: target is DRAM

  @retval N/A

**/
VOID
SetCpgcTrainTarget (
  IN UINT8           Socket,
  IN UINT8           Channel,
  IN BOOLEAN         CpgcForDdrt
  );

/**

  Enables/Disables the CPGC Mode

  @param[in]                Socket              Socket Id
  @param[in]                MemSs               Current Memory SubSystem (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Enable              TRUE: Enables CPGC Mode.FALSE: Disables CPGC Mode

**/
VOID
SetCpgcTrainMode (
  IN        UINT8           Socket,
  IN        UINT8           MemSs,
  IN        UINT8           Channel,
  IN        BOOLEAN         Enable
  );

/**

  Get the CPGC transaction target is DDRT or DRAM scheduler.

  @param[in]                Socket              Socket Id
  @param[in]                Channel             Current Channel (0-Based)

  @retval BOOLEAN       - TRUE: DDRT, FALSE: DRAM

**/
BOOLEAN
GetCpgcTrainTarget (
  IN UINT8           Socket,
  IN UINT8           Channel
  );

/**

  Return TRUE or FALSE if in DDRT2 Mode

  @param Host           - Pointer to sysHost
  @param Socket         - Socket Id
  @param Ch             - channel Id

  @retval TRUE/FALSE

**/
BOOLEAN
GetDdrt2Mode (
  PSYSHOST        Host,
  UINT8           Socket,
  UINT8           Ch
  );

/**

  Set for DDRT mode.

  @param Socket         - Socket Id
  @param Ch             - channel Id
  @param ForceDdrt      - BOOLEAN TRUE: Force to DDRT mode; FALSE: Force DDR mode


  @retval N/A

**/
VOID
SetDdrtForceMode (
  UINT8           Socket,
  UINT8           Ch,
  BOOLEAN         ForceDdrt
  );

/**

  clear for mode in the misc reg.

  @param Socket         - Socket Id
  @param Ch             - channel Id

  @retval N/A

**/
VOID
ClearDdrtForceMode (
  UINT8           Socket,
  UINT8           Ch
  );

/**

  Memory controller initialization required after training

  @param[in] MemTechType  - Memory technology type
  @param[in] Socket       - Socket number

  @retval N/A

**/
VOID
EFIAPI
PostTrainingInitMc (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          Socket
  );

/**
  Program WR data delay for a channel

  @param[in] Host     - Point to sysHost
  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number
  @param[in] SetType  - Specifies technologies - DDR4, DDRT or ALL
  @param[in] CwlAdj   - Signed value to set

  @retval N/A
**/
VOID
SetCwlAdj (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SetType,
  IN  INT16     CwlAdj
  );

//
// Defines for SetCwlAdj() SetType parameter
//
#define CWLADJ_DDR5 3
#define CWLADJ_DDRT 2
#define CWLADJ_DDR4 1
#define CWLADJ_ALL  0

/**
  Cache MC roundtrip registers

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number

  @retval - N/A
**/
VOID
CacheRoundTripMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**
  Program the IO Latency in the MC

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number
  @param[in] Value  - Value to program

  @retval N/A
**/

VOID
SetIoLatCompMc (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    ch,
  IN  UINT32   Value
  );

/**

  Set the IO latency register value in the memory controller

  @param[in] Socket          - Socket numer
  @param[in] Ch              - Channel number (0-based)
  @param[in] Rank            - Rank number (0-based)
  @param[in] IoLatencyValue  - New IO Latency value

  @retval EFI_SUCCESS if value found
          EFI_UNSUPPORTED if the function is not supported
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
SetIoLatencyMc (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    IoLatencyValue
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
GetIoLatencyMc (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *IoLatencyValue
  );

/**

  Set the round trip register value in the memory controller

  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number (0-based)
  @param[in] Rank            - Rank number (0-based)
  @param[in] RoundTripValue  - New Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_UNSUPPORTED if the function is not supported
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
SetRoundTripMc (
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
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetRoundTripMc (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *RoundTripValue
  );

SSA_STATUS
BiosGetMarginParamSpecificityChip (
  PSYSHOST Host,
  MRC_LT IoLevel,
  MRC_GT MarginGroup,
  MARGIN_PARAM_SPECIFICITY *Specificity
  );

BOOLEAN
SSAIsDdrtDimmPresent (
  PSYSHOST Host
  );

VOID
BiosGetMemTempChip (
  PSYSHOST Host,
  UINT8 Socket,
  UINT8 ChannelInSocket,
  UINT8 Dimm,
  INT32 *Temperature
  );

VOID
BiosIoResetChip (
  PSYSHOST Host,
  UINT8 Socket,
  UINT8 Controller,
  UINT8 resetIo
  );

VOID
BiosSetPageOpenConfigChip (
  PSYSHOST Host,
  UINT8 Socket,
  UINT8 Controller,
  BOOLEAN Enable,
  BOOLEAN *PrevEnable
  );

VOID
BiosInitCpgcChip (
  PSYSHOST Host,
  UINT8 Socket,
  UINT8 Controller,
  SETUP_CLEANUP SetupCleanup
  );

VOID
ddrtS3Resume (
  PSYSHOST Host,
  UINT8 socket
  );


/**

  Chip specific code path to set the VDD

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - Socket Id

  @retval None

**/
VOID
SetVdd (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Socket Id
  );

/**

  Reset the DDR channels for the given socket number (or per MC for BSSA)

  Host      - Pointer to sysHost, the system Host (root) structure
  socket    - Socket Id
  chBitMask - Bitmask of channels to reset

  @retval None

**/
VOID
ResetAllDdrChannels (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT32      chBitMask                         // Bitmask of channels to reset
  );

/**

  Reset the DDR channels for the given socket number (or per MC for BSSA)

  Host      - Pointer to sysHost, the system Host (root) structure
  socket    - Socket Id
  chBitMask - Bitmask of channels to reset

  @retval None

**/
VOID
ResetAllDdrChannelsPreTraining (
  PSYSHOST    Host,
  UINT8       socket,
  UINT32      chBitMask
  );

/**

  Enables CMD/ADDR Parity

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Processor socket

  @retval None

**/
VOID
EnableCAParityRuntime (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Processor socket ID
  );

/**

  Enable Pmon Clock running for DRAM RAPL

  @param[in] Socket       - Socket number

  @retval N/A

**/
VOID
EnablePmonClock (
  UINT8      Socket
  );

/**

  Set the bit indicating memory initialization is complete

  @param Host  - Pointer to sysHost, the system Host (root) structure

  @retval None

**/
VOID
SetMemDone (
  PSYSHOST    Host
  );

/**

  Returns the physical rank number

  @param dimm    - DIMM number
  @param rank    - Logical rank number

  @retval N/A

**/

UINT8
GetPhyRank (
  UINT8 dimm,
  UINT8 rank
  );

/**

  Returns the memory controller ID

  @param mcId - Memory controller ID
  @param ch   - Channel

  @retval MC ID

**/
UINT8
GetMCCh (
  UINT8 mcId,
  UINT8 ch
  );

/**

    Check to see if Rank for each socket is populated

    @param Host - Pointer to sysHost, the system Host (root) structure

    @retval None

**/
VOID
CheckRankPop (
  PSYSHOST Host
  );

/**

    Check to see if Rank for specified socket is populated

    @param Host   - Pointer to SysHost
    @param socket - Current socket

    @retval N/A

**/
VOID
CheckRankPopLocal (
  PSYSHOST  Host,
  UINT8     socket
  );

/**

    Get Maximum DDR Frequency supported by the processor

    @param Host   - Pointer to sysHost, the system Host (root) structure
    @param socket - Current Socket

    @retval maxCpuDdrFreq

**/
UINT8
GetMaxCpuDdrFreq (
  PSYSHOST   Host,                              // Pointer to sysHost, the system Host (root) structure
  UINT8      socket                             // Processor socket ID
  );


/**
  Programs tstab for DDRT2

  @param[in]  Host    - Pointer to sysHost structure
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number
  @param[in]  value   - tstab value;

@retval void
**/
VOID
SprTstabDdrt2 (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch,
  UINT32                    value
  );

/**
  Programs Tcpded, Tcssr, and Tcpded2srx for DDRT2

  @param[in]  Host    - Pointer to sysHost structure
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval void
**/
VOID
SprTimingsDdrt2 (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch
  );

/**
Programs DDRT2 MC settings

  @param[in]  Host    - Pointer to sysHost structure
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval void
**/
VOID
LateDdrt2Config (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Ch
  );

/**
  Programs settings for DDRT2

  @param[in]  Host    - Pointer to sysHost structure
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @retval void
**/
VOID
EarlyDdrt2Config (
  PSYSHOST                  Host,
  UINT8                     socket,
  UINT8                     Ch
  );

/**
  Get the QCLK Ratio at CPU Startup

  @param[in] Socket   - Current Socket
  @param[out] BaseClockType  - Base frequency for the Qclk Multiplier  <BR>
                         0h     MC base clock of 133 Mhz <BR>
                         1h     MC base clock 100 Mhz <BR>
                         All other values are reserved.
  @param[out] QclkMultiplier  - Product of this value with the bus clock results in the Qclk frequency              <BR>
                        Examples of some encodings of this field with a 133 MHz multiplier for QCLK:                <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq    <BR>
                        000110b    6d         3 * 133.33          400.00 MHz       6  * 133.33 MHz      800.00 MHz  <BR>
                        001000b    8d         4 * 133.33          533.33 MHz       8  * 133.33 MHz      1066.67 MHz <BR>
                        001010b    10d        5 * 133.33          666.67 MHz       10 * 133.33 MHz      1333.33 MHz <BR>
                        001100b    12d        6 * 133.33          800.00 MHz       12 * 133.33 MHz      1600.00 MHz <BR>
                        001110b    14d        7 * 133.33          933.33 MHz       14 * 133.33 MHz      1866.67 MHz <BR>
                        010000b    16d        8 * 133.33          1066.67 MHz      16 * 133.33 MHz      2133.33 MHz <BR>

                        Examples of some encodings of this field with a 100 MHz multiplier for QCLK:                <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq    <BR>
                        100000b    32d        16 * 100            1600MHz          32 * 100 MHz         3200.00 MHz <BR>
                        100100b    36d        18 * 100            1800MHz          36 * 100 MHz         3600.00 MHz <BR>
                        101000b    40d        20 * 100            2000MHz          40 * 100 MHz         4000.00 MHz <BR>
                        101100b    44d        22 * 100            2200MHz          44 * 100 MHz         4400.00 MHz <BR>
                        110000b    48d        24 * 100            2400MHz          48 * 100 MHz         4800.00 MHz <BR>

  @retval EFI_SUCCESS   Settings were retrieved successfully.
  @retval !EFI_SUCCESS  Settings were not retrieved successfully.
**/
EFI_STATUS
EFIAPI
GetStartupQclkRatio (
  IN  UINT8       Socket,
  OUT UINT8       *BaseClockType,
  OUT UINT8       *QclkMultiplier
  );


/**
  Get the QCLK Ratio.

  @param Host       - Pointer to sysHost, the system Host (root) structure
  @param socket     - Current Socket
  @param RefClkType - Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_type <BR>
                        0h     MC frequency request for 133 Mhz              <BR>
                        1h     MC frequency request for 100 Mhz              <BR>
                        All other values are reserved.
  @param QClkRatio  - Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_data <BR>
                        The data for the request.                            <BR>
                        The encoding of this field is the 133 MHz or 100MHz multiplier for QCLK: <BR>

                        Examples of some encodings of this field with a 133 MHz multiplier for QCLK:               <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq   <BR>
                        000110b    6d         3 * 133.33          400.00 MHz       6  * 133.33 MHz      800.00 MHz  <BR>
                        001000b    8d         4 * 133.33          533.33 MHz       8  * 133.33 MHz      1066.67 MHz <BR>
                        001010b    10d        5 * 133.33          666.67 MHz       10 * 133.33 MHz      1333.33 MHz <BR>
                        001100b    12d        6 * 133.33          800.00 MHz       12 * 133.33 MHz      1600.00 MHz <BR>
                        001110b    14d        7 * 133.33          933.33 MHz       14 * 133.33 MHz      1866.67 MHz <BR>
                        010000b    16d        8 * 133.33          1066.67 MHz      16 * 133.33 MHz      2133.33 MHz <BR>

                        Examples of some encodings of this field with a 100 MHz multiplier for QCLK:               <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq   <BR>
                        100000b    32d        16 * 100            1600MHz          32 * 100 MHz         3200.00 MHz <BR>
                        100100b    36d        18 * 100            1800MHz          36 * 100 MHz         3600.00 MHz <BR>
                        101000b    40d        20 * 100            2000MHz          40 * 100 MHz         4000.00 MHz <BR>
                        101100b    44d        22 * 100            2200MHz          44 * 100 MHz         4400.00 MHz <BR>
                        110000b    48d        24 * 100            2400MHz          48 * 100 MHz         4800.00 MHz <BR>

    @retval None
**/
VOID
EFIAPI
GetQclkRatio (
  IN  PSYSHOST    Host,
  IN  UINT8       socket,
  OUT UINT8       *RefClkType,
  OUT UINT8       *QClkRatio
  );

/**
  Set the DDR QCLK Ratio.

  @param Host       - Pointer to sysHost, the system Host (root) structure
  @param Socket     - Current Socket
  @param RefClkType - Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_type <BR>
                        0h     MC frequency request for 133 Mhz <BR>
                        1h     MC frequency request for 100 Mhz <BR>
                        All other values are reserved.
  @param QClkRatio  - Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_data <BR>
                        The data for the request. <BR>
                        The encoding of this field is the 133 MHz or 100MHz multiplier for  QCLK: <BR>

                        Examples of some encodings of this field with a 133 MHz multiplier for QCLK:                <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq    <BR>
                        000110b    6d         3 * 133.33          400.00 MHz       6  * 133.33 MHz      800.00 MHz  <BR>
                        001000b    8d         4 * 133.33          533.33 MHz       8  * 133.33 MHz      1066.67 MHz <BR>
                        001010b    10d        5 * 133.33          666.67 MHz       10 * 133.33 MHz      1333.33 MHz <BR>
                        001100b    12d        6 * 133.33          800.00 MHz       12 * 133.33 MHz      1600.00 MHz <BR>
                        001110b    14d        7 * 133.33          933.33 MHz       14 * 133.33 MHz      1866.67 MHz <BR>
                        010000b    16d        8 * 133.33          1066.67 MHz      16 * 133.33 MHz      2133.33 MHz <BR>

                        Examples of some encodings of this field with a 100 MHz multiplier for QCLK:                <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq    <BR>
                        100000b    32d        16 * 100            1600MHz          32 * 100 MHz         3200.00 MHz <BR>
                        100100b    36d        18 * 100            1800MHz          36 * 100 MHz         3600.00 MHz <BR>
                        101000b    40d        20 * 100            2000MHz          40 * 100 MHz         4000.00 MHz <BR>
                        101100b    44d        22 * 100            2200MHz          44 * 100 MHz         4400.00 MHz <BR>
                        110000b    48d        24 * 100            2400MHz          48 * 100 MHz         4800.00 MHz <BR>
                        ...
    @param FrequencyChangeNeeded - Indicate if frequency change is required.

    @retval None
**/
VOID
SetQclkRatio (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     RefClkType,
  IN UINT8     QClkRatio,
  IN BOOLEAN   FrequencyChangeNeeded
  );

/** Perform any DDR frequency settings necessary for cases where a socket has no DIMMs.

  @param[in]  Socket  A socket without any DIMMs populated.

  @retval EFI_SUCCESS   Settings were performed successfully.
  @retval !EFI_SUCCESS  Settings were not performed successfully.

**/
EFI_STATUS
EFIAPI
SetDDRFreqNoDimms (
  IN  UINT8 Socket
  );

/**

  Stores initial sequence numbers of AEP error logs (per DIMM) to var.mem structure

  @param Host   Pointer to host structure
  @param Socket Socket index

**/
VOID
SaveAepLogInfo (
  PSYSHOST Host,
  UINT8    Socket
  );

/**

  Retrieves then displays Error Log information over SMBUS mailbox per DCPMEM DIMM populated

  @param Host   Pointer to host structure
  @param Socket Socket index

**/
VOID
DcpMemErrorLogDisplay (
  PSYSHOST Host,
  UINT8    Socket
  );

/**

    Determine if rank is disabled

    @param Host   - Pointer to sysHost, the system Host (root) structure
    @param socket - Current socket
    @param ch     - Current channel
    @param dimm   - Current dimm
    @param rank   - Current rank

    @retval 1 - rank is disabled
    @retval 0 - rank is enabled

**/
UINT32
IsRankDisabled (
  PSYSHOST  Host,                               // Pointer to sysHost, the system Host (root) structure
  UINT8     socket,                             // Socket Id
  UINT8     ch,                                 // Channel number (0-based)
  UINT8     dimm,                               // Current dimm
  UINT16    rank                                // Current rank
  );

/**

    Set the DIMM is populated bit

    @param Host   - Pointer to sysHost, the system Host (root) structure
    @param socket - Current socket
    @param ch     - Current channel
    @param dimm   - Current dimm

    @retval None

**/
VOID
SetDimmPop (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm                              // Current dimm
  );

/**

    display processor abbreviation, stepping and socket

    @param[in] Host   - Pointer to sysHost, the system Host (root) structure
    @param[in] Socket - Processor socket ID

    @retval None

**/
VOID
DimmInfoDisplayProc (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket
  );

/**

    Display DIMM Info

    @param Host   - Pointer to sysHost, the system Host (root) structure

    @retval None

**/
VOID
DimmInfoDisplayProcFeatures (
  PSYSHOST    Host
  );

/**

    Set tWR for this channel

    @param Host   - Pointer to sysHost, the system Host (root) structure
    @param socket - Current socket
    @param ch     - Current channel
    @param nWR    - nWR value
    @param *PrevWR    - Previous WR value

    @retval None

**/
VOID
SetChTwr (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Processor socket within the system (0-based)
  UINT8       ch,                               // DDR channel number within the processor socket (0-based)
  UINT8       nWR,                              // Write Recovery value in DCLK units
  UINT8       *PrevWR                           // Previous Write Recovery value
  );

/**

  Get the current timing mode

  @param Host            - Pointer to sysHost, the system Host (root) structure
  @param socket          - Socket Id
  @param ch              - Channel number (0-based)

  @retval (UINT8)Current timing mode

**/
UINT8
GetCurrentTimingMode (
  PSYSHOST    Host,
  UINT8       socket,
  UINT8       ch
  );

/**
  Set the command timing

  @param[in] Host       - Pointer to SysHost
  @param[in] Socket     - Socket number
  @param[in] Ch         - Channel number (socket-based)
  @param[in] CmdTiming  - Command timing (TIMING_1N, TIMING_2N, TIMING_3N)

  @retval EFI_SUCCESS if timing is successfully set
          !EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
SetTimingMode (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     CmdTiming
  );

/**

    Set Encoded CS Mode

    @param Host     - Pointer to sysHost, the system Host (root) structure
    @param socket   - Socket Id
    @param ch       - Channel number (0-based)

    @retval None

**/
VOID
SetEncodedCsMode (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch                                // Channel number (0-based)
  );

/**

  Set the configuration before RMT

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id

  @retval None

**/
VOID
SetConfigBeforeRmt (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Socket Id
  );


/**

  For certain DCLK to UCLK settings we need to add bubbles

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket to switch

  @retval None

**/
VOID
ProgramBgfTable (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Socket Id
  );

/**

  This function returns the CH corrected offset for each used CPGC register, and corrects for HA0/1 if needed

  @param socket              - Socket number
  @param ch                  - Channel Number
  @param regOffset           - Base register offset

  @retval New register Offset based on CH number

**/
UINT32
GetCpgcRegOffset (
  UINT8     socket,
  UINT8     ch,
  UINT32    regOffset
  );

UINT8
GetCmdMarginsSweepErrorResFeedback (
  GSM_CSN   signal,
  UINT8     dimm,
  INT16     piIndex,
  UINT8     CurError
  );

/**

  Provides RcSim feedback for RCD QxCA Training

  @param[in]  Signal        Signal being swept
  @param[in]  Dimm          Current DIMM
  @param[in]  PiIndex       Index to be used in feedback
  @param[in]  CurError      Current Pi Delay feedback error

  @retval 0   PiDelay passed
  @retval 1   PiDelay Failed

**/
UINT8
GetCmdMarginsBacksideSweepErrorResFeedback (
  IN  GSM_CSN   Signal,
  IN  UINT8     Dimm,
  IN  INT16     PiIndex,
  IN  UINT8     CurError
  );

/**

  Chip specific code path to get the data stored in the requested sticky scratchpad

  @param socket      - Socket Id
  @param scratchpad  - Scratch pad to read

  @retval (UINT32) Scratchpad data

**/
UINT32
GetStickyScratchpad (
  UINT8       socket,                           // Socket Id
  UINT8       scratchpad                        // Scratch pad to read
  );

/**

  Chip specific code path to set the data into the requested sticky scratchpad

  @param socket      - Socket Id
  @param scratchpad  - Scratch pad to read
  @param data        - Data to be written

  @retval None

**/
VOID
SetStickyScratchpad (
  UINT8       socket,                           // Socket Id
  UINT8       scratchpad,                       // Scratch pad to read
  UINT32      data                              // Data to be written
  );

/**

  This routine gets the existing state of the sticky scratchpad value for
  the MultiThread bit into the address provided by the caller and it clears
  or sets the bit based on the caller's input.

  @param Host        - Pointer to sysHost, the system Host (root) structure
  @param socket      - Socket Id
  @param Data        - Data to be written

  @retval MRC_STATUS_SUCCESS or MRC_STATUS_FAILURE

**/
MRC_STATUS
SetMultiThreadScratchpadBit (
  PSYSHOST    Host,
  UINT8       Socket,
  UINT8       Data
  );

  /**

  Chip specific code path to get the data from the sticky scratchpad that contains the WarmBoot bit.

  @param Host        - Pointer to sysHost, the system Host (root) structure
  @param socket      - Socket Id

  @retval The value found in the Scratchpad bit for WarmBoot.

**/
UINT8
GetScratchpadWarmBoot (
  PSYSHOST    Host,
  UINT8       Socket
  );

/**

  This routine gets the existing state of the sticky scratchpad value for
  the WarmBoot bit into the address provided by the caller and it clears the
  WarmBoot bit.

  @param Host        - Pointer to sysHost, the system Host (root) structure
  @param socket      - Socket Id

  @retval None

**/
VOID
ClearScratchpadWarmBoot (
  PSYSHOST    Host,
  UINT8       Socket
  );

/**

  This routine gets the existing state of the sticky scratchpad value for
  the BIST/FRB event bit into the address provided by the caller and it clears
  or sets the bit based on the caller's input.

  @param Host        - Pointer to sysHost, the system Host (root) structure
  @param socket      - Socket Id
  @param Data        - Data to be written

  @retval MRC_STATUS_SUCCESS or MRC_STATUS_FAILURE

**/
MRC_STATUS
CheckFrb (
  PSYSHOST    Host,
  UINT8       Socket,
  UINT8       Data,
  UINT8       *BitState
  );

/**

  Read from a non-sticky scratch pad

  @param[in] Socket      - Socket Id
  @param[in] Scratchpad  - Scratchpad to write

  @retval Data read from the requested scratch pad

**/
UINT32
EFIAPI
GetNonStickyScratchpad (
  IN UINT8  Socket,
  IN UINT8  Scratchpad
  );

/**

  Write to a non-sticky scratch pad

  @param[in] Socket      - Socket Id
  @param[in] Scratchpad  - Scratchpad to write
  @param[in] Data        - Data to be written

  @retval None

**/
VOID
EFIAPI
SetNonStickyScratchpad (
  IN UINT8   Socket,
  IN UINT8   Scratchpad,
  IN UINT32  Data
  );

/**

  Set the ActToAct Period

  @param Host         - Pointer to sysHost, the system Host (root) structure
  @param socket       - CPU Socket Node number (Socket ID)
  @param ch           - Channel number (0-based)
  @param ActToActSave - Array or turnaround values to
                        save/restore

  @retval None

**/
VOID
SetActToActPeriod (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  IN ACT_TO_ACT_SAVE_STRUCT ActToActSave[MAX_CH] // Turnaround values to save/restore
  );

/**

  Restore the ActToAct Period

  @param[in] Socket       - CPU Socket Node number (Socket ID)
  @param[in] Ch           - Channel number (0-based)
  @param[in] ActToActSave - Array or turnaround values to
                        save/restore

  @retval None

**/
VOID
RestoreActToActPeriod (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN ACT_TO_ACT_SAVE_STRUCT ActToActSave[MAX_CH]
  );

/**

  Read IoLatencyIoComp setting

  @param[in]  Host          - Pointer to sysHost
  @param[in]  Socket        - Processor to initialize
  @param[in]  Ch            - Channel number
  @param[out] IoLatIoComp   - Pointer to caller's storage
  @param[in]  Mode          - Bit field of access modes

  @retval N/A

**/
VOID
EFIAPI
GetIoLatencyIoCompMc (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  OUT   UINT32    *IoLatIoComp,
  IN    UINT8     Mode
  );

/**

  set IO Latency Compensation for normal operation

  @param Host         - Pointer to sysHost, the system Host (root) structure
  @param socket       - Socket Id
  @param ch           - Channel number (0-based)
  @param SubCh        - Sub Channel number (0-based)
  @param rtError      - Error status on current channel
  @param ActToActSave - Array or turnaround values to
                        save/restore

  @retval None

**/
VOID
SetIOLatencyComp (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       SubCh,                            // Sub channel number
  UINT8       rtError,                          // Error status on current channel
  IN ACT_TO_ACT_SAVE_STRUCT ActToActSave[MAX_CH] // Turnaround values to save/restore
  );

/**

  Chip specific call to Decrement IO Latency if necessary

  @param Host           - Pointer to sysHost, the system Host (root) structure
  @param socket         - Socket Id
  @param ch             - Channel number (0-based)
  @param dimm           - Current dimm
  @param rank           - Current rank
  @param IoLatency      - Pointer to IoLatency value to decrement
  @param centerPoint    - Center point

  @retval None
 */
VOID
DecIoLatency (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Current rank
  UINT8       *IoLatency,                       // Pointer to IoLatency value to decrement
  UINT16      centerPoint[MAX_CH][MAX_STROBE]   // Center point
  );

/**

  Setup PI settings parameters

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - CPU Socket Node number (Socket ID)
  @param step             - The PI step to work on
  @param piSetting        - The PI setting returned
  @param piStart          - Test starting PI
  @param NumberOfCycle    - Number of cycles
  @param trainRes         - Training results

  @retval None

**/
VOID
SetupPiSettingsParams (
  PSYSHOST  Host,                                           // Pointer to sysHost, the system Host (root) structure
  UINT8     socket,                                         // CPU Socket Node number (Socket ID)
  UINT8     step,                                           // The PI step to work on
  UINT16    *piSetting,                                     // The PI setting returned
  UINT16    *piStart,                                       // Test starting PI
  UINT16    *NumberOfCycle,                                 // Number of cycles
  struct  TrainingResults  (**trainRes)[MAX_CH][MAX_STROBE] // Training results
  );

/**

  Chip specific code path to get the correct index into the training results

  @param step      - Pointer to sysHost, the system Host (root) structure
  @param piSetting - CPU Socket Node number (Socket ID)
  @param piStart   - Channel number (0-based)
                     Strobe (0-based)
  @retval (UINT8)index value

**/
UINT8
GetTrainingResultIndex (
  UINT8       step,                             // Current training step
  UINT16      piSetting,                        // Pi Setting
  UINT16      piStart                           // Pi Start
  );

/**

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - CPU Socket Node number (Socket ID)
  @param ch       - Channel number (0-based)
  @param strobe   - Strobe (0-based)
  @param csrReg   - CSR Register
  @param piDelay  - Pi Delay

  @retval None

**/
VOID
RecEnTrainFeedback (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       strobe,                           // Strobe (0-based)
  UINT32      csrReg,                           // CSR Register
  UINT16      piDelay                           // Pi Delay
  );

/**
  Chip specific code path to get the starting value requested


  @param step     - Current PI step
  @param piStart  - PI start value

  @retval (UINT16)Starting value

**/
UINT16
StartBackOver (
  UINT8       step,                             // Current PI step
  UINT16      piStart                           // PI start value
  );

/**

  Set the initial round trip value

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket number
  @param ch      - Channel number
  @param dimm    - DIMM number
  @param rank    - Rank number

  @retval None

**/
VOID
SetInitRoundTrip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Current rank
  );

/**

  Chip specific code path to disable 2 cycle bypass

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param data    - Data for disable 2cyc bypass

  @retval None

**/
VOID
SetDisable2cycBypass (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       data                              // Data for disable 2cyc bypass
  );

/**

  Program ODT Timing

  @param Host        - Pointer to sysHost, the system Host (root) structure
  @param socket      - Socket number
  @param ch          - Channel number (0-based)

  @retval None

**/
VOID
SetOdtTiming (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch                                // Channel number (0-based)
  );

/**

  Set ODT Matrix for DDR3

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number
  @param rank    - Rank

  @retval None
**/
VOID
SetDdr3OdtMatrix (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Processor socket number
  UINT8       ch,                               // Channel number
  UINT8       dimm,                             // DIMM number
  UINT8       rank                              // Rank
  );

/**
  Write the Read and Write ODT Activates to the Matrix registers

  @param [IN] Socket         - Socket number
  @param [IN] Ch             - Channel number (0-based)
  @param [IN] Dimm           - DIMM number
  @param [IN] RdOdtTableData - Read ODT table data
  @param [IN] WrOdtTableData - Write ODT table data

  @retval None
**/
VOID
SetOdtActMatrixRegs (
  IN UINT8  Socket,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT32 RdOdtTableData,
  IN UINT32 WrOdtTableData
  );

/** Get or set DDRT Grant-to-ERID value.

  Note that the "combine edges" mode is not applicable, so the CSM_COMBINE_EDGES mode flag will be ignored.

  @param[in]      Host    Pointer to host structure
  @param[in]      Socket  Target socket
  @param[in]      Channel Target channel
  @param[in]      Dimm    Target DIMM
  @param[in]      Mode    Get/Set mode flags
  @param[in,out]  Data    Grant-to-ERID value in Dclks. The current value is always returned, while an input value is
                          used in accordance with Mode flags.

  @retval MRC_STATUS_GSM_LIMIT  Value to set exceeded range (max or min) and was limited
  @retval MRC_STATUS_SUCCESS    Values read and/or written without issue

**/
MRC_STATUS
EFIAPI
GetSetSxpGnt2Erid (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel,
  IN      UINT8     Dimm,
  IN      UINT8     Mode,
  IN OUT  INT16     *Data
  );

/**

  Collect ERID test results

  @param Host             - Pointer to sysHost, the system Host (root) structure struct
  @param socket           - Socket Id
  @param ch               - Channel number (0-based)
  @param dimm             - Current dimm
  @param offset           - Offset to stored result
  @param index            - Index of the result to get
  @param eridOffset       - Offset into the ERID results
  @param gnt2eridPassed   - Status of the requested result. 1 = PASS, 0 = FAIL

  @retval None

**/
VOID
CollectTestResultsErid (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT16 offset,
  UINT8 index,
  UINT16 eridOffset,
  UINT8 *gnt2eridPassed
  );

/**

  Chip specific setup to initialize loop indices when performing a pidelay step

  @param Host          - Pointer to sysHost, the system Host (root) structure
  @param socket        - CPU Socket Node number (Socket ID)
  @param piDelayStart  - Pointer to the loop start index
  @param piDelayEnd    - Pointer to the loop end index

  @retval None

**/
VOID
GetChipTiPiSamplepiDelayStartandEnd (
  PSYSHOST  Host,                               // Pointer to sysHost, the system Host (root) structure
  UINT8     socket,                             // CPU Socket Node number (Socket ID)
  INT16     *piDelayStart,                      // Pointer to the loop start index
  UINT16    *piDelayEnd                         // Pointer to the loop end index
  );

/**

  Chip specific code path to determine if DDR4 LRDIM Host side training should be enabled. Disables legacy Host side training if it has been enabled

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param ch      - Channel number (0-based)
  @param dimm    - Current dimm
  @param rank    - Current rank

  @retval TRUE if the chip should disable DDR4 LRDIM Host side training
  @retval FALSE if the chip should not disable DDR4 LRDIM Host side training

**/
BOOLEAN
ShouldChipEnableDDR4LRDIMMHostSideTraining (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Rank number (0-based)
  );

/**

  Chip specific code path to support each platforms individual set of RTT_NOM values

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - processor socket ID
  @param ch      - Processor socket Channel number
  @param dimm    - Processor socket channel dimm number
  @param rank    - Processor socket channel dimm rank number

  @retval None

**/
VOID
SetChipRTTPark (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Current rank
  );

/**

  Chip specific code path to Disable any legacy LRDRIM WriteLeveling and return false, or return true if should disable DDR4 write leveling.

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param ch      - Channel number (0-based)
  @param dimm    - Current dimm
  @param rank    - Current rank

  @retval TRUE if the chip should disable DDR4 write leveling
  @retval FALSE if the chip should not disable DDR4 write leveling

**/
BOOLEAN
ShouldChipDisableDDR4LRDIMMWriteLeveling (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Current rank
  );

/**

  Chip specific code path. Platforms supporting DDR3 require seperate code path for DDR4 that to restore RTT_PARK. DDR3 does not need to do this.

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)
  @param dimm    - Current dimm
  @param rank    - Rank number (0-based)

  @retval None

**/
VOID
DoChipCompatibleRTT_PARKRestore (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       rank                              // Rank number (0-based)
  );

/**

  Chip specific code path to collect and store the results from the current test

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id
  @param dimm    - Current dimm
  @param rank    - Rank number (0-based)
  @param piDelay - Pi Delay

  @retval None

**/
VOID
GetResultsWL (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       dimm,                             // Current dimm
  UINT8       rank,                             // Rank number (0-based)
  UINT16      piDelay                           // Pi Delay
  );

/**

  Update the Global Offsets

  @param Host            - Pointer to sysHost, the system Host (root) structure
  @param ch              - processor socket ID
  @param TargetOffset    - Target offset for byte lanes
  @param CRAddDelay      - Return value from UpdateGlobalOffsets()
  @param GlobalByteOff   - Return value from UpdateGlobalOffsets()

  @retval None

**/
VOID
UpdateGlobalOffsets (
  PSYSHOST    Host,
  UINT8       ch,
  INT16       TargetOffset,
  INT16       *CRAddDelay,
  INT16       *GlobalByteOff
  );

/**

  Chip specific code path to initialize "GlobalByteOff"

  @param Host                 - Pointer to sysHost, the system Host (root) structure
  @param socket               - processor socket ID
  @param ch                   - Processor socket Channel number
  @param offsetValue          - Offset Value
  @param CRAddDelay           - Return value from UpdateGlobalOffsets()
  @param GlobalByteOff        - Return value from UpdateGlobalOffsets()

  @retval None
**/
VOID
SetChipDQDQSTiming (
  PSYSHOST                Host,                 // Pointer to sysHost, the system Host (root) structure
  UINT8                   socket,               // Socket Id
  UINT8                   ch,                   // Channel number (0-based)
  INT16                   offsetValue,          // Offset Value
  INT16*                  CRAddDelay,           // Return value from UpdateGlobalOffsets()
  INT16*                  GlobalByteOff,
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct   // Pointer to WL Cleanup structure
  );


/**

  Chip specific code path to write Chip Wr Addr delays

  @param Host                - Pointer to sysHost, the system Host (root) structure
  @param socket              - Socket Id
  @param ch                  - Channel number (0-based)
  @param offsetValue         - Offset for byte lanes
  @param CRAddDelay          - Add delays
  @param WLChipCleanUpStruct - Pointer to CHIP_WL_CLEANUP_STRUCT
  @retval None

**/
VOID
WriteChipWrADDDelays (
  PSYSHOST                Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8                   socket,                           // Socket Id
  UINT8                   ch,                               // Channel number (0-based)
  IN UINT8                SetType,                          // 0 = everything, 1 = DDR4, 2 = DDRT
  INT16                   offsetValue,                      // Offset for byte lanes
  INT16                   *CRAddDelay,                      // Add delays
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  );

/**

  Chip specific code path to getChipTimingData

  @param Host                 - Pointer to sysHost, the system Host (root) structure
  @param socket               - processor socket ID
  @param ch                   - Processor socket Channel number
  @param WLChipCleanUpStruct - Pointer to CHIP_WL_CLEANUP_STRUCT

  @retval None

**/
VOID
GetChipTimingData (
  PSYSHOST                Host,                 // Pointer to sysHost, the system Host (root) structure
  UINT8                   socket,               // Socket Id
  UINT8                   ch,                   // Channel number (0-based)
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct   // Pointer to CHIP_WL_CLEANUP_STRUCT
  );

/**

  Chip specific code path to updateChipMCDelay

  @param Host                - Pointer to sysHost, the system Host (root) structure
  @param socket              - processor socket ID
  @param ch                  - Processor socket Channel number
  @param channelNvList       - Pointer to the Channel nvram
  @param CRAddDelay          - Add delays
  @param WLChipCleanUpStruct - Pointer to CHIP_WL_CLEANUP_STRUCT

  @retval None

**/
VOID
UpdateChipMCDelay (
  PSYSHOST                Host,                     // Pointer to sysHost, the system Host (root) structure
  UINT8                   socket,                   // Socket Id
  UINT8                   ch,                       // Channel number (0-based)
  IN UINT8                SetType,                  // 0 = everything, 1 = DDR4, 2 = DDRT
  struct channelNvram     (*channelNvList)[MAX_CH], // Host channel list in NVRam
  INT16                   *CRAddDelay,              // Add delays
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct       // Write Leveling clean up structure pointer
  );

/**

  Exectues command/clock training

  @param Host      - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS

**/
UINT32
ChipLateCmdClk (
  PSYSHOST    Host                              // Pointer to sysHost, the system Host (root) structure
  );

/**

  Chip specific code path to enable check parity mode

  @param Host - Pointer to sysHost, the system Host (root) structure
  @param mode - mode is either MODE_VA_DESELECT or MODE_DESELECT

  @retval None

**/
VOID
ChipEnableCheckParity (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT16      *mode                             // mode is either MODE_VA_DESELECT or MODE_DESELECT
  );

/**

  Turn on command add operation enable

  @param Host                - Pointer to sysHost, the system Host (root) structure
  @param socket              - Socket number
  @param ch                  - DDR channel (0-based)
  @param chipErlyCmdCKStruct - Pointer to Early CMD CK structure

  @retval None

**/
VOID
TrnOnChipCMDADDOpEn (
  PSYSHOST  Host,                               // Pointer to sysHost, the system Host (root) structure
  UINT8     socket,                             // Processor socket ID
  UINT8     ch,                                 // DDR channel (0-based)
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct  // Pointer to Early CMD CK structure
  );

/**

  Enable FNV

  @param Host                 - Pointer to sysHost, the system Host (root) structure
  @param socket               - Socket Id
  @param ch                   - Channel number (0-based)
  @param chipErlyCmdCKStruct  - Pointer to Early CMD CK structure

  @retval None

**/
VOID
EnableChipFNV (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct  // Pointer to Early CMD CK structure
  );

/**

  Turn off command add operation enable

  @param Host                - Pointer to sysHost, the system Host (root) structure
  @param socket              - Socket number
  @param ch                  - DDR channel (0-based)
  @param chipErlyCmdCKStruct - Pointer to Early CMD CK structure

  @retval None

**/
VOID
TrnOffChipCMDADDOpEn (
  PSYSHOST  Host,                               // Pointer to sysHost, the system Host (root) structure
  UINT8     socket,                             // Processor socket ID
  UINT8     ch,                                 // DDR channel (0-based)
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct  // Pointer to Early CMD CK structure
  );

/**

  Initialize the requested structure

  @param chipGetCmdMgnSwpStruct  - Pointer to struct to initialize

  @retval None

**/
VOID
GetCmdMarginsSweepInit (
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct // Pointer to struct to initialize
  );

/**

  Update the CMD Margin Masks

  @param Host                   - Pointer to sysHost, the system Host (root) structure
  @param socket                 - Socket Id
  @param ch                     - Channel number (0-based)
  @param piIndex                - PI index
  @param offset                 - Max PI index
  @param errorResult            - Error results
  @param chipGetCmdMgnSwpStruct - Pointer to structure

  @retval None
**/
VOID
UpdateCmdMarginMasks (
  PSYSHOST    Host,                                   // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                                 // Socket Id
  UINT8       ch,                                     // Channel number (0-based)
  INT16       piIndex,                                // PI index
  INT16       offset,                                 // Max PI index
  UINT8 errorResult[MAX_CH][PAR_TEST_RANGE],          // Error results
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct // Pointer to structure
  );

/**

  Chip specific code path to check if the channel is done

  @param Host                    - Pointer to sysHost, the system Host (root) structure
  @param chipGetCmdMgnSwpStruct  - Mask data to check

  @retval 0 the channel is done
  @retval 1 the channel is not done

**/
UINT8
CheckChDoneMask (
  PSYSHOST    Host,                                   // Pointer to sysHost, the system Host (root) structure
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct // Mask data to check
  );

/**

  Chip specific code path to determine if errors should be cleared if a parity error occurred

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param status - Status of existing parity error

  @retval TRUE
  @retval FALSE

**/
BOOLEAN
ShouldChipClearCMDMGNParityError (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT32      status                            // Status of existing parity error
  );

/**

  Collects the results of the previous parity based test

  @param Host      - Pointer to sysHost, the system Host (root) structure
  @param socket    - CPU Socket Node number (Socket ID)
  @param chToCheck - Channel number (0-based)
  @param dimm      - Current dimm
  @param bwSerr    - Bit wise status structure

  @retval 0 if no failures were detected
  @retval Non-zero if any failure was detected

**/
UINT32
ChipCollectParityResults (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT32      chToCheck,                        // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT32      bwSerr[MAX_CH][3]                 // Bit wise status structure
  );

/**

  Clears the results of the previous parity based test

  @param Host       - Pointer to sysHost, the system Host (root) structure
  @param socket     - CPU Socket Node number (Socket ID)
  @param chToCheck  - Channel number (0-based)
  @param dimm       - Current dimm

  @retval None
**/
VOID
ChipClearParityResults (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT32      chToCheck,                        // Channel number (0-based)
  UINT8       dimm                              // Current dimm
  );

/**

  Set 3N timing and save original setting

  @param Host                            - Pointer to sysHost, the system Host (root) structure
  @param socket                          - Socket Id
  @param ch                              - Channel number (0-based)
  @param chipEnableParityCheckingStruct  - Pointer to parity checking

  @retval None

**/
VOID
ChipSet3NTimingandSaveOrigSetting (
  PSYSHOST    Host,                                           // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                                         // Socket Id
  UINT8       ch,                                             // Channel number (0-based)
  PCHIP_PARITY_CHECKING_STRUCT chipEnableParityCheckingStruct // Pointer to parity checking
  );

/**
  Determine if the given channel number is valid on current socket.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Channel       - Channel number

  @retval   TRUE      Channel number is valid
  @retval   FALSE     Channel number is not valid
**/
BOOLEAN
EFIAPI
IsChannelValid (
  IN      PSYSHOST        Host,
  IN      UINT8           Channel
  );

/**
  Determine if the given MC number is valid on current socket.

  @param[in]      Host          - Pointer to sysHost
  @param[in]      McId          - MC number

  @retval   TRUE      MC number is valid
  @retval   FALSE     MC number is not valid
**/
BOOLEAN
EFIAPI
IsMcValid (
  IN      PSYSHOST        Host,
  IN      UINT8           McId
  );

/**
  Update the IMC number based on the CAP ID value

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number

  @retVal         SUCCESS
**/
EFI_STATUS
EFIAPI
UpdateImcNumber (
  IN      PSYSHOST        Host,
  IN      UINT8           Socket
  );

/**

  Restore original CMD timing

  @param Host                           - Pointer to sysHost, the system Host (root) structure
  @param socket                         - Socket Id
  @param ch                             - Channel number (0-based)
  @param chipEnableParityCheckingStruct - Pointer to parity checking

  @retval None

**/
VOID
ChipRestoreOrigCMDtiming (
  PSYSHOST    Host,                                            // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                                          // Socket Id
  UINT8       ch,                                              // Channel number (0-based)
  PCHIP_PARITY_CHECKING_STRUCT chipEnableParityCheckingStruct  // Pointer to parity checking
  );

/**

  Chip specific code to determine if parity is supported for the DIMM

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval TRUE
  @retval FALSE

**/
BOOLEAN
DoesChipSupportParityChecking (
  IN PSYSHOST    Host                              // Pointer to sysHost, the system Host (root) structure
  );

/**

  Clears the results of the previous parity based test

  @param Host       - Pointer to sysHost, the system Host (root) structure
  @param socket     - CPU Socket Node number (Socket ID)
  @param chToCheck  - Channel number (0-based)
  @param rank       - Rank number (0-based)
  @param dimm       - Current dimm

  @retval None

**/
VOID
ChipClearParityResultsAlt (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT32      chToCheck,                        // Channel number (0-based)
  UINT8       rank,                             // Rank number (0-based)
  UINT8       dimm                              // Current dimm
  );

/**

  Clears the results of the previous parity based test during RCD QxCA Training

  @param[in]  Host        Pointer to sysHost, the system host (root) structure
  @param[in]  Socket      CPU Socket Node number (Socket ID)
  @param[in]  ChToCheck   Channel number (0-based)
  @param[in]  Rank        Current rank
  @param[in]  Dimm        Current dimm

  @retval None

**/
VOID
ChipClearBacksideParityResults (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT32    ChToCheck,
  IN  UINT8     Rank,
  IN  UINT8     Dimm
  );

/**

  Chip specific path to disable parity checking per channel

  @param Host                            - Pointer to sysHost, the system Host (root) structure
  @param socket                          - Socket Id
  @param ch                              - Channel number (0-based)
  @param chipDisableParityCheckingStruct - Pointer to parity checking

  @retval None

**/
VOID
ChipSpecificParityCheckDisablePerCh (
  PSYSHOST    Host,                                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                                           // Socket Id
  UINT8       ch,                                               // Channel number (0-based)
  PCHIP_PARITY_CHECKING_STRUCT chipDisableParityCheckingStruct  // Pointer to parity checking
  );

/**

  Chip specific code path to enable specific error conditions

  @param Host                            - Pointer to sysHost, the system Host (root) structure
  @param socket                          - Socket Id
  @param ch                              - Channel number (0-based)
  @param chipDisableParityCheckingStruct - Pointer to parity checking

  @retval None

**/
VOID
ChipSpecificErrorEnable (
  PSYSHOST    Host,                                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                                           // Socket Id
  UINT8       ch,                                               // Channel number (0-based)
  PCHIP_PARITY_CHECKING_STRUCT chipDisableParityCheckingStruct  // Pointer to parity checking
  );

/**

  Chip specific code path to determine if chip should set normal CMD timing

  @param Host                         - Pointer to sysHost, the system Host (root) structure
  @param socket                       - Socket Id
  @param ch                           - Channel number (0-based)
  @param chipSetNormalCMDTimingStruct - Normal Command Timing structure

  @retval TRUE chip should set normal CMD timing
  @retval FALSE chip should not set normal CMD timing

**/
BOOLEAN
ShouldChipSetNormalCMDTiming (
  PSYSHOST    Host,                                                // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                                              // Socket Id
  UINT8       ch,                                                  // Channel number (0-based)
  PCHIP_SET_NORMAL_CMD_TIMING_STRUCT chipSetNormalCMDTimingStruct  // Normal Command Timing structure
  );

VOID
ChipCtlClkFeedBack (
  PSYSHOST Host,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  UINT8 sign,
  INT16 *ctlMargins
  );



/**
  Set the IO Latency offset required for early CTL/CLK training using the
  Receive Enable method.

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number
  @param[in]  LogRank         - Logical rank number

  @retval   EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
StepChipCtlCKIOLatency (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   LogRank
  );

/**
  Restore the IO Latency offset after early CTL/CLK training using the
  Receive Enable method.

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number
  @param[in]  LogRank         - Logical rank number

  @retval   EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
RestoreChipCtlCKIOLatency (
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   LogRank
  );

/**
  Gets the Receive Enable PI delay offset required for Early
  CTL/CLK training using the Receive Enable method.

  @param[in]    Socket          - Socket number
  @param[out]   RecEnOffset     - Pointer updated to point to the offset to apply
                                  to the Receive Enable PI delay

  @retval   EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
EarlyCtlClkRecEnOffset (
  IN    UINT8   Socket,
  OUT   INT16   *RecEnOffset
  );

/**

  Handles any register interaction on GetMargins entry/exit.

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket number
  @param ch      - Channel number
  @param entry   - TRUE when entering GetMargins, FALSE when exiting.

  @retval SUCCESS

**/
UINT32
GetMarginsHook (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  BOOLEAN     entry                             // TRUE when entering GetMargins, FALSE when exiting.
  );

/**

  Checks if DQ swizzling is supported

  @param Host      - Pointer to sysHost, the system Host (root) structure
  @param socket    - CPU Socket Node number (Socket ID)

  @retval 0 Swizzle not supported
  @retval 1 Swizzle supported

**/
UINT8
CheckDqSwizzleSupport (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // CPU Socket Node number (Socket ID)
  );

/**

  Check if CTL loopback mode should be used

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval TRUE - CTL loopback mode should be used
  @retval TRUE - CTL loopback mode should not be used

**/
BOOLEAN
CheckCTLLoopback (
  PSYSHOST Host
  );

/**

  This gets the parity error status during CTL loopback training

  @param Host   - Pointer to sysHost, the system Host (root) structure struct
  @param socket - Socket Id
  @param ch     - Channel number (0-based)
  @param dimm   - Current dimm

  @retval Alert Feedback Sticky Bit

**/
UINT8
ChipGetParityErrorStatus (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  );

/**

  This sets the CTL loopback mode in the buffer

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure struct
  @param[in] socket  - Socket Id
  @param[in] ch      - Channel number (0-based)
  @param[in] dimm    - Current dimm
  @param[in] rank    - Current rank

  @retval None

**/
VOID
ChipSetCTLLoopback (
  IN  PSYSHOST Host,
  IN  UINT8 socket,
  IN  UINT8 ch,
  IN  UINT8 dimm,
  IN  UINT8 rank
);

/**

  Initialize Receive Enable Parameters the same for all ranks

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket number

  @retval None

**/
VOID
ReceiveEnableInitAllRanks (
  PSYSHOST Host,                                // Pointer to sysHost, the system Host (root) structure
  UINT8    socket                               // Socket Id
  );

/**
  Prepare all ranks for Receive Enable training

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number

  @retval N/A
**/
VOID
ReceiveEnableInitAllRanksDdr5 (
  IN    PSYSHOST    Host,
  IN    UINT8       Socket
  );

/**
  DDR5 Pre Training Init Task.


  @param[in] Host     - Pointer to sysHost

  @retval N/A
**/
UINT32
Ddr5PreTrainingInit (
  IN PSYSHOST Host
  );

/**

  Start Early CMD/CLK tests

  @param Host       - Pointer to sysHost, the system Host (root) structure
  @param socket     - Socket Id
  @param chBitmask  - Channels to execute test on

  @retval None

**/
VOID
EarlyCmdSignalTest (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT32      chBitmask                         // Channels to execute test on
  );

/**

  Checks if Rx Vref tuning is required

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval TRUE - Enable RxVref tuning
  @retval FALSE - Do not enable RxVref tuning

**/
BOOLEAN
ShouldChipEnableRxVrefTuning (
  PSYSHOST Host
  );

///
/// Definitions for use with GetPageTableTimerState() and SetPageTableTimer()
///
typedef enum {
  PageTimerEnable,    ///< Page table timer enabled
  PageTimerDisable,   ///< Page table timer disabled
  PageTimerStateMax   ///< Number of elements in this enum
} PAGE_TIMER_STATE;

/**

  Get page table timer status

  @param[in] Host     Pointer to sysHost
  @param[in] Socket   Socket number
  @param[in] Channel  Channel number

  @retval PageTimerEnable   Page timer is enabled
  @retval PageTimerDisable  Page timer is disabled

**/
PAGE_TIMER_STATE
EFIAPI
GetPageTableTimerState (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Channel
  );

/**

  Enable/Disable page table timer

  @param[in]   Host     Pointer to sysHost
  @param[in]   Socket   Socket number
  @param[in]   Channel  Channel number
  @param[in]   State    PageTimerDisable to disable page timer
                        PageTimerEnable to enable page timer

  @retval None

**/
VOID
EFIAPI
SetPageTableTimer (
  IN  PSYSHOST          Host,
  IN  UINT8             Socket,
  IN  UINT8             Channel,
  IN  PAGE_TIMER_STATE  State
  );

/**

  Initialize the settings for a power training parameter

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to a specific power training parameter structure

  @retval MRC_STATUS

**/
MRC_STATUS
GetPowerParameterSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  );

/**

  Check socket if MemRasFlag is set and inComingSktId is not set

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket number

  @return 1 if conditions are met
  @return 0 if conditions are not met
**/
UINT8
CheckSktId (
  PSYSHOST Host,                     // Pointer to sysHost, the system Host (root) structure
  UINT8    socket                    // Socket Id
  );

/**

  Check if socket is BSP and if MemRasFlag is not set

  @param Host    - Pointer to sysHost, the system Host (root) structure

  @return 1 if conditions are met
  @return 0 if conditions are not met
**/
UINT8
CheckMemRas (
  PSYSHOST    Host                   // Pointer to sysHost, the system Host (root) structure
  );

/**
  Function copies CPUID information to syshost from cpuid instruction for given socket

  @param[in] Socket     Socket number

  @retval None
**/
VOID
EFIAPI
LoadCpuId (
  IN UINT8    Socket
  );

/**
  Init CAPID's that are used to track nested call levels for semaphores

  @param[in]  SocketId       Socket index.

  @retval None
**/
VOID
EFIAPI
LoadCapId (
  IN  UINT8    SocketId
  );

/**

  Get current DDR scrambler mode and set new scrambler mode

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Ch number
  @param scrambleConfigEn    - DDR scrambler enable state
  @param scrambleConfigEnOrg    Original DDR scrambler config

  @retval None

**/
VOID
GetSetDdrScramblerEn (
  PSYSHOST    Host,
  UINT8       Socket,
  UINT8       Ch,
  UINT32      ScrambleEnConfig,
  UINT32      *PrevScrambleEnConfig
);

/**

  Restore Scrambler after AdvancedMemTest ends

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket Id
  @param ch                 - Channel number (0-based)
  @param scrambleConfigOrg  - DDR scrambler config returned by PreAdvancedMemTest

  @retval None

**/
VOID
PostAdvancedMemTestRestoreScrambler (
  PSYSHOST    Host,                            // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                          // Socket Id
  UINT8       ch,                              // Channel number (0-based)
  UINT32      scrambleConfigOrg                // DDR scrambler config returned by PreAdvancedMemTest
  );

/**

  Determine if CMD VREF Training is complete

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - Socket number

  @retval TRUE - CMD VREF training is complete
  @retval FALSE - CMD VREF training is not complete

**/
UINT8
isCmdVrefTrainingDone (
  PSYSHOST Host,
  UINT8 socket
  );

/**

  Apply settings before AdvancedMemTest starts

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id
  @param ch      - Channel number (0-based)

  @retval (UINT32) scrambleConfigOrg

**/
UINT32
PreAdvancedMemTest (
  PSYSHOST    Host,                            // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                          // Socket Id
  UINT8       ch                               // Channel number (0-based)
  );

/**

  Apply settings after AdvancedMemTest ends

  @param Host               - Pointer to sysHost, the system Host (root) structure
  @param socket             - Socket Id
  @param ch                 - Channel number (0-based)
  @param scrambleConfigOrg  - DDR scrambler config returned by PreAdvancedMemTest

  @retval None

**/
VOID
PostAdvancedMemTest (
  PSYSHOST    Host,                            // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                          // Socket Id
  UINT8       ch,                              // Channel number (0-based)
  UINT32      scrambleConfigOrg                // DDR scrambler config returned by PreAdvancedMemTest
  );

/**

  This API programs 2X refresh functionality for current Socket
  pointed by SysHost Structure. Based on setup option the function
  can enable\disable 2x refresh programming.

  Needs to be called after advanced memory test.

  @retval N/A

**/
VOID
EFIAPI
ProgramTemperatureRefresh (
  VOID
  );

/**
  Check for conditions that require DDR frequency to be overridden, apply
  overrides if conditions dictate it

  @param[in]      Host      Pointer to sysHost
  @param[in]      Socket    Socket number
  @param[in,out]  DdrFreq   DDR frequency

  @retval N/A
**/
VOID
EFIAPI
ChipOverrideFreq (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN OUT  UINT8    *DdrFreq
  );

/**

  Routine Description: Checks if lockstep is enabled

  @param Host    - Pointer to sysHost

  @retval N/A
**/

UINT8
ChipLockstepEn (
  PSYSHOST Host
  );

/**

  Routine Description: Checks if mirroring is enabled

  @param Host    - Pointer to sysHost

  @retval N/A
**/

UINT8
ChipMirrorEn (
  PSYSHOST Host
  );

/**

  This function parses through the KTI var structure
  to detect if MCP is valid. Future Reference code will use mcpPresent

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

  @retval None

**/

VOID
CheckMCPPresent (
  PSYSHOST Host,
  UINT8    socket
  );

/**

  Get the maximum number of data groups based on DIMM and group type

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - Socket
  @param ch     - Channel
  @param dimm   - DIMM
  @param group  - Group type
  @param level  - Level type

  @retval Max strobe

**/
UINT8
GetMaxStrobe (
  PSYSHOST  Host,       // Pointer to Host structure (outside scope of this spec)
  UINT8     socket,     // Socket
  UINT8     ch,         // Channel
  UINT8     dimm,       // DIMM
  MRC_GT    group,      // Group type
  MRC_LT    level       // Level type
  );

/**
  Enable/Disable 3DS mode

  @pararm[in]   Host          Pointer to SysHost
  @pararm[in]   Socket        Socket number
  @pararm[in]   Ch            Channel number
  @pararm[in]   Enable3DsMode Flag to enable or disable 3DS Mode
                                0 == disable
                                !0 == enable

  @retval       N/A
**/
VOID
Toggle3DSMode (
  PSYSHOST Host,                             // Pointer to the system Host (root) structure
  UINT8    socket,                           // Socket Id
  UINT8    ch,                               // Channel number (0-based)
  UINT8    value                             // value to be programmed
  );

VOID
UpdateDdrtGroupLimits (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  MRC_LT   level,                            // IO level to access
  MRC_GT   group,                            // Data group to access
  UINT16   *minLimit,                        // Minimum delay value allowed
  UINT16   *maxLimit                         // Maximum absolute value allowed
  );

/**

  Offsets CCC IO delays

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket number

  @retval VOID

**/
VOID
OffsetCCC (
  PSYSHOST  Host,
  UINT8     socket,
  INT16     offset,
  UINT8     jedecinit
  );

/**

  This routine initializes the memory size fields in the structures
  for DIMMs, Channels, and Nodes.  The sizes are calculated from the sizes in
  the rank structures.

  @param Host   - Pointer to sysHost
  @param Socket - Socket

  @retval VOID

**/

VOID
DisplayFnvInfo (
  IN    PSYSHOST    Host,
  IN    UINT8       Socket
  );

/**
  Check if custom refresh rate selected

  @retval FAILURE -  Custom refresh rate not selected
  @retval SUCCESS -  Custom refresh rate selected
**/
UINT32
EFIAPI
IsCustomRefreshRate (
  VOID
  );

/**
Retrieves then AEP FW information over SMBUS mailbox per AEP DIMM populated and
Enable/Disble the AIT Dram polling

@param Host   Pointer to host structure
@param Socket Socket index

**/
VOID
CheckAepFwAndEnableDramPoll (
  PSYSHOST  Host,
  UINT8     Socket
  );

/**
  Populate the MEM_CHIP_POLICY structure in SysHost

  @param  Host  - Pointer to SysHost structure

  @retval MRC_STATUS_SUCCESS
**/

MRC_STATUS
PopulateMemChipPolicy (
  PSYSHOST Host
  );

UINT8
CheckS3Jedec (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  );

/**
  IncludeDeselects value for this project

  @retval TRUE
          FALSE
**/
UINT8
GetChipIncludeDeselectsWriteLevelingCleanUp (
  VOID
  );

UINT8
CheckBacksideSwizzle (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    strobe
  );

/**
  Returns desired Training Step Status

  @param socket       - Socket Id
  @param TrainingStep - Training Step Id

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
EFIAPI
GetTrainingStepEnableChip (
  IN UINT8    Socket,
  IN UINT8    TrainingStep
  );

/**
  Returns desired status for the Rx DFE feature

  @param Host          - Pointer to sysHost
  @param Socket        - Socket Id

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
GetRxDfeEnableChip (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**
  Returns desired status for the TX Rise Fall Slew Rate feature

  @param Host          - Pointer to sysHost
  @param Socket        - Socket Id

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
GetTxRiseFallSlewRateEnableChip (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**
  Convert DDR frequency in MT/s to QCLK ratio

  @param[in]    Host        - Pointer to sysHost structure
  @param[in]    Socket      - Socket number
  @param[in]    DdrFreq     - The DDR frequency to convert
  @param[out]   QclkRatio   - A pointer to the QCLK ratio for the refclk and frequency

  @retval SUCCESS if frequency encoding found
          FAILURE otherwise
**/
UINT32
EFIAPI
DdrFreqToQclkRatio (
  IN        PSYSHOST  Host,
  IN        UINT8     Socket,
  IN        UINT32    DdrFreq,
  OUT       UINT8     *QclkRatio
  );

/**

  Convert QCLK ratio to DDR frequency in MT/s

  @param[in]    Host        - Pointer to sysHost structure
  @param[in]    Socket      - Socket number
  @param[in]    QclkRatio   - The DCLK ratio for the refclk and frequency
  @param[out]   DdrFreqMts  - A pointer to the DDR frequency in MT/s to convert

  @retval MRC_STATUS_SUCCESS if frequency encoding found
          MRC_STATUS_FAILURE otherwise
**/
MRC_STATUS
EFIAPI
QclkRatioToDdrFreq (
  IN        PSYSHOST  Host,
  IN        UINT8     Socket,
  IN        UINT8     QclkRatio,
  OUT       UINT32    *DdrFreqMts
  );

/**
  Convert QCLK ratio to MRC DDR Frequency index

  @param[in]    Host        - Pointer to sysHost
  @param[in]    Socket      - Socket number
  @param[in]    QclkRatio   - QCLK ratio
  @param[out]   FreqIndex   - MRC index for the DDR frequency

  @retval SUCCESS if conversion supported
          FAILURE otherwise
**/
UINT32
EFIAPI
QclkToDdrFreqIndex (
  IN        PSYSHOST  Host,
  IN        UINT8     Socket,
  IN        UINT8     QclkRatio,
  OUT       UINT8     *FreqIndex
  );

/**  This function execute the write leveling Cleanup data evaluation for DDR4 and DDRT.

  @param Host                - Pointer to sysHost
  @param ByteOff             - Byte Offset
  @param ByteSum             - Byte Sum
  @param ByteCount           - Byte Count
  @param Done                - Done indication from centering flow
  @param CRAddDelay          - Additive Delay
  @param WLChipCleanUpStruct - WL Delays


  @retval NA
**/

VOID
WriteLevelingCleanUpDataEval (
  IN PSYSHOST                Host,
  IN INT16                   ByteOff[MAX_CH][MAX_RANK_CH][MAX_STROBE],
  IN INT16                   ByteSum[2][MAX_CH],
  IN UINT16                  ByteCount[2][MAX_CH],
  IN UINT8                   Done,
  IN INT16                   CRAddDelay[MAX_CH],
  IN CHIP_WL_CLEANUP_STRUCT  WLChipCleanUpStruct
  );

/**

  Evaluate if DDR4/DDRT when DdrTech match the DIMM

  @param DdrTech             - Type of flow to follow for DDR Technology 0 DDR4 1 DDRT
  @param AepDimmPresent      - DDRT Dimm type

  @retval BOOLEAN

**/

BOOLEAN
DoesDdrTechMatchThisDimm (
  UINT8      DdrTech,
  UINT8      AepDimmPresent
  );

/**

  Initialize rank structures.  This is based on the requested
  RAS mode and what RAS modes the configuration supports. This
  routine also evalues the requested RAS modes to ensure they
  are supported by the system configuration.

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
RASCheckDimmRanks (
  PSYSHOST Host
  );

/**

  Initialize rank structures.  This is based on the requested
  RAS mode and what RAS modes the configuration supports. This
  routine also evalues the requested RAS modes to ensure they
  are supported by the system configuration.

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
CheckRASSupportAfterMemInit (
  PSYSHOST Host
  );

/**

  Enable RAS modes
  NOTE: Default settings are unchanged if feature not explicitly enabled by BIOS

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
SetRASConfig (
  PSYSHOST Host
  );

/**

  Returns the paired rank

  @param Host           - Pointer to sysHost
  @param socket         - Socket number
  @param ch             - Pointer to sysHost
  @param dimm           - Pointer to sysHost
  @param rank           - Pointer to sysHost
  @param pairRankNumber - Pointer to sysHost
  @param pairChannel    - Pointer to sysHost
  @param pairDimm       - Pointer to sysHost
  @param pairRank       - Pointer to sysHost

  @retval VOID

**/

VOID
GetPairedRank (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  UINT8 *pairRankNumber,
  UINT8 *pairChannel,
  UINT8 *pairDimm,
  UINT8 *pairRank
  );

/**

  Restores other timing parameters

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket number
  @param ch      - Channel number

  @retval None

**/
VOID
RestoreTimingsChip (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch
  );

/**

  Programs the MRS shadow register

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - CPU Socket Node number (Socket ID)
  @param ch      - Channel number (0-based)
  @param dimm    - Dimm number (0-based)
  @param data    - MRS Data to set
  @param bank    - Bank number

  @retval None
**/
VOID
SetMRSShadow (
  IN      PSYSHOST      Host,                                // Pointer to sysHost, the system Host (root) structure
  IN      UINT8         Socket,                              // CPU Socket Node number (Socket ID)
  IN      UINT8         Ch,                                  // Channel number (0-based)
  IN      UINT8         Dimm,                                // Dimm number (0-based)
  IN      UINT32        Data,                                // MRS Data to program
  IN      UINT8         Bank                                 // Bank number
  );

/**

Indicates whether 2x Refresh needs to be disabled for the entire channel

  @param[in] Host     - Pointer to SysHost
  @param[in] Socket   - Socket number
  @param[in] Ch       - Channel number

  @retval Status      - TRUE /FALSE to disable 2x Refresh for channel

**/
BOOLEAN
EFIAPI
Disable2xRefreshCh (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  );

/**

  Get the CWL adjustment

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - CPU Socket Node number (Socket ID)
  @param[in] Ch      - Channel number (0-based)
  @param[in] Dimm    - Dimm number (0-based)
  @param[out] CwlAdj - Ptr to the adjustment

  @retval None

**/

VOID
GetCwlAdj (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  OUT INT16   *CwlAdj
  );

/**
  Switch to DDRT2 mode for Tx training

  @param[in]  Host    Pointer to SysHost

  @retval Success
**/
UINT32
EFIAPI
SwitchToDdrt2Mode (
  IN  PSYSHOST  Host
  );

/**

  Get the DDR5 CWL adjustment

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - CPU Socket Node number (Socket ID)
  @param[in] Ch      - Channel number (0-based)
  @param[in] SubCh   - Channel number (0-based)
  @param[in] Dimm    - Dimm number (0-based)
  @param[out] CwlAdj - Ptr to the adjustment

  @retval None

**/

VOID
GetCwlAdjDdr5 (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       subch,
  IN UINT8    Dimm,
  INT16       *cwlAdj
  );

/**

  Chip specific code path to initialize the effective bus delay

  @param Host                - Pointer to sysHost, the system Host (root) structure
  @param socket              - CPU Socket Node number (Socket ID)
  @param ch                  - Channel number (0-based)
  @param CwlAdj              - Pointer to CwlAdj
  @param IOComp              - Pointer to IOComp
  @param IntDelta            - Pointer to IntDelta
  @param longPreambleEnable  - Pointer to the INT32 preamble enable

  @retval None

**/
VOID
EffectiveBusDelayInit (
  PSYSHOST    Host,                                   // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                                 // CPU Socket Node number (Socket ID)
  UINT8       ch,                                     // Channel number (0-based)
  IN UINT8    Dimm,                                   // DIMM number (Don't care for SKX)
  INT16       *CwlAdj,                                // Pointer to CwlAdj
  UINT32      *IOComp,                                // Pointer to IOComp
  UINT16      *IntDelta,                              // Pointer to IntDelta
  UINT32      *longPreambleEnable                     // Pointer to the long preamble enable
  );

/**

  Calculate the final gap

  @param gap      - Pointer to the final gap value
  @param maxGap   - Maximum gap
  @param gapDimm  - Gap calculated based on current DIMM

  @retval None

**/
VOID
CalcFinalGap (
  UINT16      *gap,                             // Pointer to the final gap value
  UINT16      maxGap,                           // Maximum gap
  UINT16      gapDimm                           // Gap calculated based on current DIMM
  );

/**

  Get ODT Timing

  @param Host          - Pointer to sysHost, the system Host (root) structure
  @param socket        - Socket number
  @param ch            - Channel number (0-based)
  @param rdOdtTiming   - Pointer to Read ODT Timings
  @param wrOdtTiming   - Pointer to Write ODT Timings

  @retval None

**/
VOID
GetOdtTiming (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  ODT_TIMING  *odtTiming                        // Pointer to ODT Timings structure
  );

/**

  This function ensures we do not overflow a timing parameter.
  If *Value is outside of the limits for the processor, a warning will be
  logged and *Value will be updated with the Max value allowed.

  @param[in]      MemTechType  - Memory Technology Type
  @param[in]      Socket       - CPU Socket number
  @param[in]      Ch           - Channel number
  @param[in]      Timing       - Timing parameter
  @param[in,out]  *Value       - Pointer to the value to check

  @retval EFI_SUCCESS           - Value is in bounds
          EFI_INVALID_PARAMETER - Value is out of bounds, *Value will be updated
                                  with the maximum supported value
          EFI_NOT_FOUND         - Timing parameter not supported

**/
EFI_STATUS
EFIAPI
CheckBoundsChip (
  IN      MEM_TECH_TYPE     MemTechType,
  IN      UINT8             Socket,
  IN      UINT8             Ch,
  IN      MEM_TIMING_TYPE   Timing,
  IN OUT  UINT32            *Value
  );

/**
  Get or Set specified single type of turnaround timing for specified channel.

  If Mode is GSM_READ_CSR on input, read turnaround related CSRs,
  adjust the data to be non SOC specific, and return
  If the Mode is GSM_FORCE_WRITE, adjust the data in a turnaround structure
  as needed for a given SOC, and write the CSRs
  Other input is not supported.
  Other Mode input settings are not supported by this function

  @param[in]       MemTechType         - Memory Technology Type
  @param[in]       Socket              - Socket number
  @param[in]       Ch                  - Channel number
  @param[in]       Mode                - Get/Set mode flags
  @param[in]       TurnaroundType      - Turn around type.
  @param[in, out]  TurnaroundValue     - Pointer to Turnaround setting

  @retval MRC_STATUS_SUCCESS   Turnaround CSRs read/write successful
  @retval MRC_STATUS_SUCCESS   Turnaround CSRs read/write fail
**/
EFI_STATUS
GetSetSingleTurnaroundSetting (
  IN     MEM_TECH_TYPE    MemTechType,
  IN     UINT8            Socket,
  IN     UINT8            Ch,
  IN     UINT8            Mode,
  IN     UINT8            TurnaroundType,
  IN OUT UINT32           *TurnaroundValue
  );

/**
  GetSetTurnaroundSettings
  If Mode is GSM_READ_ONLY on input, read turnaround related CSRs,
  adjust the data to be non SOC specific, and return
  If the Mode is GSM_FORCE_WRITE, adjust the data in a turnaround structure
  as needed for a given SOC, and write the CSRs
  Other Mode input settings are not supported by this function

  @param[in]       MemTechType         - Memory Technology Type
  @param[in]       Socket              - Socket number
  @param[in]       Ch                  - Channel number
  @param[in]       Mode                - Get/Set mode flags
  @param[in, out]  TurnaroundSettings  - Pointer to Turnaround settings

  @retval EFI_SUCCESS   Turnaround CSRs read/write successful
**/
EFI_STATUS
GetSetTurnaroundSettings (
  IN     MEM_TECH_TYPE  MemTechType,
  IN     UINT8          Socket,
  IN     UINT8          Ch,
  IN     UINT8          Mode,
  IN OUT UINT32         TurnaroundSettings[]
  );

/**

  GetSetDcpmmTurnaroundSettings
  If Mode is GSM_READ_ONLY on input, read DCPMM turnaround related CSRs,
  adjust the data to be non SOC specific, and return
  If the Mode is GSM_FORCE_WRITE, adjust the data in a turnaround structure
  as needed for a given SOC, and write the CSRs
  Other input is not supported.
  Other Mode input settings are not supported by this function

  @param[in]       Socket                        - Socket number
  @param[in]       Ch                            - Channel number
  @param[in]       Mode                          - Get/Set mode flags
  @param[in, out]  DcpmmTurnaroundSettingsTable  - Pointer to Turnaround settings

  @retval EFI_SUCCESS   Turnaround CSRs read/write successful

**/
EFI_STATUS
GetSetDcpmmTurnaroundSettings (
  IN     UINT8   Socket,
  IN     UINT8   Ch,
  IN     UINT8   Mode,
  IN OUT UINT32  DcpmmTurnaroundSettingsTable[]
  );

/**
  Program chip-specific refresh WaterMark timing register.

  @param[in]  Socket      Socket number
  @param[in]  Channel     Channel number

  @retval N/A

**/
VOID
ProgramRefreshTimingsWaterMark (
  IN UINT8    Socket,
  IN UINT8    Channel
  );

/**
  Find if there is a LRDIMM present on the platform

  @retval TRUE/FALSE     TRUE: LRDIMM is present
**/

BOOLEAN
SSAIsLrdimmPresent (
  PSYSHOST Host
  );

/**

  Restore MC timing parameters for fast boot or resume paths

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Processor to initialize
  @param[in]  Ch      - Channel number

  @retval N/A

**/
VOID
SetRestoreTimingsMc (
  IN    PSYSHOST  Host,
  IN    UINT8     socket,
  IN    UINT8     ch
  );
/**

  Returns the memory controller ID

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket Id
  @param ch      - Channel number

  @retval MCID

**/
UINT8
GetMCID (
  PSYSHOST   Host,                              // Pointer to sysHost, the system Host (root) structure
  UINT8      socket,                            // CPU Socket Node number (Socket ID)
  UINT8      ch                                 // Channel number
  );

/**
   Synchronize Mcmtr register of two sockets.

  @param SbspId      -- SBSP index
  @param PendingSoc  -- Socket to synchronize index
  @param McId        -- Memory Controller id

  @retval none

**/
VOID
EFIAPI
MemSyncPerMcCsrs (
  IN  UINT8     SbspId,
  IN  UINT8     PendingSocId,
  IN  UINT8     McId
  );

/**

  Enable/Disable Error injection bits on each channel

  @param[in] MemTechType  - Memory Technology Type
  @param Socket           - Socket number
  @param McId             - Memory controller id
  @param ErrInj           - Value to enable/disable error injection

  @retval N/A

**/
VOID
SetErrInjValPerMc (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          Socket,
  IN UINT8          McId,
  IN UINT8          ErrInj
  );

/**

  Program DRAM RAPL Values such as TDP, Min power and Max Power

  @param[in] Socket        - Socket Number, 0 based
  @param[in] DramTpd       - The Spec power allowed for DRAM. The TDP setting
                             is typical (not guaranteed). The units for this
                             value are defined in Watt.
  @param[in] DramMinPwr    - The minimal power setting allowed for DRAM.
                             Lower values will be clamped to this value. The
                             minimum setting is typical (not guaranteed). The
                             units for this value are defined in Watt.
  @param[in] DramMaxPwr    - The maximal power setting allowed for DRAM.
                             Higher values will be clamped to this value. The
                             maximum setting is typical (not guaranteed). The
                             units for this value are defined in Watt.

  @retval N/A

**/
VOID
EFIAPI
ProgramDramRaplValues (
  IN UINT8  Socket,
  IN UINT16 DramTdp,
  IN UINT16 DramMinPwr,
  IN UINT16 DramMaxPwr
  );

/**

  Checks if close page mapping is enabled

  @param[in] socket   - Socket number, 0 based
  @param[in] Channel  - Channel number, 0 based

  @retval  TRUE   if close page mapping
  @retval  FALSE  if open page mapping

**/
BOOLEAN
EFIAPI
IsMcMtrClosePageMapping (
  IN UINT8 Socket,
  IN UINT8 Channel
  );


/**

  Configures Input sense assertion period

  @param[in]  Socket             - Socket Number, 0 based

  @retval N/A

**/
VOID
EFIAPI
ConfigureMemHotSenseAssertPeriod (
  IN UINT8 Socket
  );

/**

  Disables Throttling for safely initializing Memory with no
  interference from MPM

  @param[in] Socket                 - Socket Id, 0 based
  @param[in] Channel                - Channel Id, 0 based

  @retval N/A

**/
VOID
EFIAPI
DisableThrottling (
  IN UINT8 Socket,
  IN UINT8 Channel
  );

/** Configure CKE and ODT contributions for RAPL.

  @param[in]  Socket  The target socket number
  @param[in]  Channel The target channel number

  @retval N/A

**/
VOID
EFIAPI
ConfigureCkeAndOdtContributions (
  IN UINT8 Socket,
  IN UINT8 Channel
  );

/**

  Configures power per command for given DIMM

  @param[in] Socket            - Socket number, 0 based
  @param[in] Channel           - Channel Number, 0 based
  @param[in] Dimm              - Dimm Number, 0 based
  @param[in] WTPmCmdPwrData    - DimmWTRowEntryStruct structure that passes the values
                                 Parameter is an entry from DIMM Weight Table, 0 -3 => PwrcasrDimmVal, 4-7 =>PwrcaswDimmVal

  @retval    N/A

**/
VOID
EFIAPI
ConfigurePwrPerCmd (
  IN UINT8  Socket,
  IN UINT8  Channel,
  IN UINT8  Dimm,
  IN UINT32 WTPmCmdPwrData
  );

/** Configure min OPS.

  @param[in]  Socket  The target socket number

  @retval N/A

**/
VOID
EFIAPI
ConfigureMinOps (
  IN UINT8 Socket
  );

/**

  Configure thermal management feature

  @param[in]  Socket                 - Socket Number, 0 based
  @param[in]  CfgMemThermFeatureType - Feature type to change
                                       Takes value from enum CONFIGURE_MEM_THERMAL_FEATURE_TYPE.

  @retval   EFI_SUCCESS       Feature Configured Properly
  @retval   EFI_UNSUPPORTED   Feature not supported

**/
EFI_STATUS
EFIAPI
ConfigureMemoryThermalFeature (
  IN UINT8 Socket,
  IN CONFIGURE_MEM_THERMAL_FEATURE_TYPE CfgMemThermFeatureType
  );

/**

  Gets\Sets various memory thermal throttle values

  @param[in] MemTechType            - Memory technology type.
  @param[in] Socket                 - Socket number, 0 based
  @param[in] Channel                - Channel Number, 0 based
  @param[in] Dimm                   - Dimm Number, 0 based
  @param[in] OperationMode          - FALSE = Operation Write, TRUE = Operation Read
  @param[in] ThrottleValueType      - Throttle value type to configure.
                                    - For more information on this parameter refer enum THROTTLE_VALUE_TYPE
  @param[in out] ThrottleValue      - Structure to hold value to write or read.

  @retval   EFI_SUCCESS              Feature Configured Properly
  @retval   EFI_UNSUPPORTED          Feature not supported

**/
EFI_STATUS
EFIAPI
GetSetThrottleValues (
  IN MEM_TECH_TYPE       MemTechType,
  IN UINT8               Socket,
  IN UINT8               Channel,
  IN UINT8               Dimm,
  IN BOOLEAN             OperationMode,
  IN THROTTLE_VALUE_TYPE ThrottleValueType,
  IN OUT THROTTLE_VALUE  *ThrottleValue
  );

/**

  Gets\Sets current memory temperature of given dimm

  @param[in] Socket                 - Socket Id, 0 based
  @param[in] Channel                - Channel Id, 0 based
  @param[in] Dimm                   - Dimm Id, 0 based
  @param[in] ValueType              - Value type to program. Check enum THROTTLE_VALUE_TYPE for more info.
  @param[in out] Value              - Value to Get\set

  @retval EFI_UNSUPPORTED             Value not supported
  @retval EFI_SUCCESS                 Value Get\set successfully.

**/
EFI_STATUS
EFIAPI
GetSetMemTempStatusRegValues (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN BOOLEAN OperationMode,
  IN THROTTLE_VALUE_TYPE ValueType,
  IN OUT UINT8 *Value
  );


UINT8
CheckBacksideSwizzle (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    strobe
  );

/** Returns whether or not a memory controller is active.

  @param[in]  ChannelMask       Bit mask indicating channel activity (active = 1, inactive = 0)
  @param[in]  MemoryController  The memory controller to check against

  @retval FALSE The memory controller is not active
  @retval TRUE  The memory controller is active

**/
BOOLEAN
EFIAPI
MemoryControllerActive (
  IN UINT32  ChannelMask,
  IN UINT8   MemoryController
  );

/**
  Enable WPQ Flush on all channels on the input MC

  @param [in] Socket    - socket index
  @param [in] Mc        - MC on socket

  @retval             - None
 */
VOID
EnableMcWpqFlush (
  IN UINT8   Socket,
  IN UINT8   Mc
  );

typedef struct {
  UINT8  EncTgtWays;  // Encoded value for target ways
  UINT8  EncTgtGran;  // Encoded value for target granularity
  UINT8  EncChWays;   // Encoded value for channel ways
  UINT8  EncChGran;   // Encoded value for channel granularity
} ENC_WAYS_GRAN;

typedef struct {
  UINT8     EncInterleaveMode; // Encoded value for DRAM rule interleave mode
  UINT8     EncMod3AsMod2;     // Encoded value to indicate if mod3 or mod2 is used
  UINT8     EncMod3;           // Encoded value to determine usage of mod3/mod2 in target idx calculation
  UINT8     EncMod3Mode;       // Encoded value to define range of bits used for mod3/mod2 calculation
} ENC_MODE_DATA;

typedef struct {
  INT32         TadOffset;   // Offset for TAD rule (in 64MB, signed)
  ENC_WAYS_GRAN TadWaysGran; // TAD ways and granularity
  BOOLEAN       FarMemory;   // Indicates far memory (14nm)
  UINT8         ChIdxOffset; // Reverse addr translation chan index offset (14nm)
} TAD_1LM_DATA;

#define FEATURE_DIR_WR_DIS       BIT0  // Disable directory writes
#define FEATURE_FORCE_DIR_INV    BIT1  // Force directory state to invalid
#define FEATURE_DIS_PCOMMIT_PERF BIT2  // Disable PCOMMIT performance optimizations

#define PS_ATTR_MIRROR    BIT0  // Enables mirroring
#define PS_ATTR_FAILOVER  BIT1  // Indicates mirror failover
#define PS_ATTR_DOWNGRADE BIT2  // Downgrades patrol scrub to correctable

typedef struct {
  UINT32        PsBase;     // Base for patrol/sparing region (64MB granularity)
  UINT32        PsLimit;    // Limit for patrol/sparing region (64MB granularity)
  UINT32        PsAttr;     // Bitfield for patrol/sparing attributes defined above
  UINT8         TgtLid;     // Logical target ID
  UINT8         ChLid;      // Logical channel ID
  UINT8         McPosition; // Ordinal position of MC in the interleave (14nm)
  ENC_WAYS_GRAN WaysGran;   // Encoded ways and granularity (14nm)
} PS_RANGE_DATA;

typedef struct {
  UINT32        PsLimit;      // limit for patrol/sparing region (64mb granularity)
  UINT32        EncChL2pid;   // Bit array of active channel IDs (10nm)
  ENC_WAYS_GRAN WaysGran;     // encoded ways and granularity (10nm)
  BOOLEAN       NmRuleEnable; // Rule enable for near memory Dram rule (14nm)
  ENC_MODE_DATA NmMode;       // Near memory mode data (14nm)
} PS_RULE_DATA;

typedef struct {
  UINT32        NmCacheBase;   // Base address of near memory cache (64MB granularity)
  UINT32        NmChSize;      // Near memory channel size (64MB granularity)
  UINT8         NmTgtLid;      // Near memory logical target ID
  UINT8         NmChLid;       // Near memory logical channel ID
  BOOLEAN       EnableTwoWay;  // Enable or disable biased 2-way near memory cache (10nm wave 3 only)
  ENC_WAYS_GRAN NmWaysGran;    // Near memory ways and granularity
} NM_2LM_DATA;

typedef struct {
  UINT32        TadLimit;      // Limit for 2LM region (64MB granularity)
  UINT8         FmChIntBitmap; // Far memory channel interleave bitmap (10nm)
  UINT8         FmTarget;      // Far memory target ID (10nm)
  ENC_WAYS_GRAN FmWaysGran;    // Far memory ways and granularity (10nm)
  BOOLEAN       FmRuleEnable;  // Rule enable for far memory Dram rule (14nm)
  ENC_MODE_DATA FmMode;        // Far memory mode data (14nm)
} FM_2LM_DATA;

typedef struct {
  BOOLEAN RirValid;                // Indicates if rank interleave range is valid
  UINT32  RirLimit;                // Rank interleave limit (64MB granularity)
  UINT32  RirOffset[MAX_RIR_WAYS]; // Rank interleave offset (64MB granularity)
  UINT8   RirTgtId[MAX_RIR_WAYS];  // Target ID for rank interleave
  UINT8   EncRirWays;              // Encoded value for rank interleave wayness
} ENC_RIR_LIMIT_WAYS;

typedef struct {
  ENC_RIR_LIMIT_WAYS NmRir[MAX_RIR];      // near memory rank interleave data
  ENC_RIR_LIMIT_WAYS FmRir[MAX_RIR_DDRT]; // far memory rank interleave data
} RIR_DATA;

/**
  Programs MC 1LM registers for a specified TAD rule.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.
  @param [in] TadIndex               The index of the TAD entry.
  @param [in] TadData                Information required to define a TAD rule region.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSet1lmTad (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          ChIndex,
  IN UINT8          TadIndex,
  IN TAD_1LM_DATA   *TadData
  );


/**
  Programs MC force_lat bit for a channel.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetForceLatency (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          ChIndex
  );

/**

  Programs MC wait4bothhalves bit for a channel.

  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/

EFI_STATUS
EFIAPI
McSetWaitForBothHalves (
  IN UINT8  SocketId,
  IN UINT8  McIndex,
  IN UINT8  ChIndex
  );

/**
  Programs MC scratchpad register bits to support workarounds.

  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.
  @param [in] AndMask                Bit field to AND with scratch register bits.
  @param [in] OrMask                 Bit field to OR with scratch register bits.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetScratchBits (
  IN UINT8   SocketId,
  IN UINT8   McIndex,
  IN UINT8   ChIndex,
  IN UINT32  AndMask,
  IN UINT32  OrMask
  );

/**
  Programs MC feature registers.

  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.
  @param [in] FeatureBitmap          Bitmap of the features to enable on the memory controller.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetFeatures (
  IN UINT8   SocketId,
  IN UINT8   McIndex,
  IN UINT8   ChIndex,
  IN UINT32  FeatureBitmap
  );

/**
  Programs MC near memory cache configuration to enabled memory modes for M2Mem traffic.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] IsNmTile               Indicates whether MC tile is populated with 2LM NM.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetNmCachingCfg (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN BOOLEAN        IsNmTile
  );

/**
  Programs 2LM TAD bits in MC near memory cache configuration.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] TadIndex2lm            Index of the TAD entry for 2LM.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetNmCachingTad (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          TadIndex2lm
  );

/**
  Programs MC patrol and sparing registers for a specified TAD rule.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.
  @param [in] OffsetIndex            The index of this TAD rule.
  @param [in] RangeData              Information required to define a patrol/sparing region.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetPatrolSparingRange (
  IN MEM_TECH_TYPE   MemTechType,
  IN UINT8           SocketId,
  IN UINT8           McIndex,
  IN UINT8           ChIndex,
  IN UINT8           OffsetIndex,
  IN PS_RANGE_DATA   *RangeData
  );

/**
  Programs MC patrol and sparing registers for a specified TAD rule.

  @param [in] MemTechType            Memory technology type.
  @param [in] SocketId               Socket number ID (0 should always be the SBSP).
  @param [in] McIndex                Index of the memory controller on the socket.
  @param [in] ChIndex                Index of the channel on the memory controller.
  @param [in] RegIndex               Register Index for TAD rule(10nm), SAD rule(14nm).
  @param [in] RuleData               Information required to define a patrol/sparing region.

  @retval EFI_SUCCESS                The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetPatrolSparingRule (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          ChIndex,
  IN UINT8          RegIndex,
  IN PS_RULE_DATA   *RuleData
  );

/**
  Programs MC 2LM registers for a specified TAD rule.

  @param[in] MemTechType         Memory technology type.
  @param[in] SocketId            Socket number ID (0 should always be the SBSP).
  @param[in] McIndex             Index of the memory controller on the socket.
  @param[in] ChIndex             Index of the channel on the memory controller.
  @param[in] NmData              Information required to define a 2LM near memory region.

  @retval EFI_SUCCESS            The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER  The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSet2lmNm (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          ChIndex,
  IN NM_2LM_DATA    *NmData
  );

/**
  Programs MC 2LM registers for far memory.

  @param [in] MemTechType        Memory technology type.
  @param [in] SocketId           Socket number ID(0 should always be the SBSP).
  @param [in] McIndex            Index of the memory controller on the socket.
  @param [in] ChIndex            Index of the channel on the memory controller.
  @param [in] RegIndex           Register Index for MC DRAM Rule(10nm), SAD rule(14nm).
  @param [in] FmData             Information required to define a 2LM far memory region.

  @retval EFI_SUCCESS            The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER  The function failed due to some illegal input.
  @retval EFI_OUT_OF_RESOURCES   The available registers are exausted.

**/
EFI_STATUS
EFIAPI
McSet2lmFm (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          McIndex,
  IN UINT8          ChIndex,
  IN UINT8          RegIndex,
  IN FM_2LM_DATA    *FmData
  );


/**
  Programs MC RIR registers.

  @param [in] MemTechType        Memory technology type.
  @param [in] SocketId           Socket number ID (0 should always be the SBSP).
  @param [in] McIndex            Index of the memory controller on the socket.
  @param [in] ChIndex            Index of the channel on the memory controller.
  @param [in] DdrtPresent        Indicates if DDRT DIMM is present on the channel.
  @param [in] RirData            Information required to program the rank interleave registers for the channel.

  @retval EFI_SUCCESS            The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER  The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetRir (
  IN MEM_TECH_TYPE    MemTechType,
  IN UINT8            SocketId,
  IN UINT8            McIndex,
  IN UINT8            ChIndex,
  IN BOOLEAN          DdrtPresent,
  IN RIR_DATA         *RirData
  );

/**

  Programs MC Interleave registers for 14nm near/far memory.

  @param [in] SocketId           Socket number ID(0 should always be the SBSP).
  @param [in] McIndex            Index of the memory controller on the socket.
  @param [in] SadIndex           The index of this SAD rule.
  @param [in] FarMemory          Indicates far memory.
  @param [in] EncInterleaveData  Encoded Information indicating InterleaveList for near/far memory.

  @retval EFI_SUCCESS            The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER  The function failed due to some illegal input.

**/

EFI_STATUS
EFIAPI
McSetInterleaveList (
  IN UINT8         SocketId,
  IN UINT8         McIndex,
  IN UINT8         SadIndex,
  IN BOOLEAN       FarMemory,
  IN UINT32        EncInterleaveData
  );

typedef struct {
  UINT8 NmChannelId[MAX_CH];
  UINT8 FmChannelId[MAX_CH];
} ROUTE_TABLE_INFO;

/**

  Writes route table data to registers

  @param [in] SocketId           Socket number ID(0 should always be the SBSP).
  @param [in] McIndex            Index of the memory controller on the socket.
  @param [in] DdrtPresent        Indicates if DDRT DIMM is present on the channel.
  @param [in] RouteTableData     Information required to program the Route table CSR's.

  @retval EFI_SUCCESS            The function is executed successfully without any error
  @retval EFI_INVALID_PARAMETER  The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
McSetRouteTableCsr (
  IN UINT8              SocketId,
  IN UINT8              McIndex,
  IN BOOLEAN            DdrtPresent,
  IN ROUTE_TABLE_INFO   *RouteTableData
  );

/**

  Sets SBMCACTL_MC_2LM.nmfillwrcamerrdis = 1 on the input channel.

  @param [in] SocketId           Socket number ID(0 should always be the SBSP).
  @param [in] McIndex            Index of the memory controller on the socket.
  @param [in] ChIndex            Index of the channel on the memory controller.

  @retval none

**/
VOID
EFIAPI
McDisableNmFillWrCamErrViralTriggering (
  IN UINT8 SocketId,
  IN UINT8 McIndex,
  IN UINT8 ChIndex
  );

/**
  @brief Programs Error Signalling for McDdrt Errors
  Note that DDRT_FNV_INTR is used to select error singaling not DDRT_FNV0/1_EVENT.

  @param[in] Socket      - Socket index at system level
  @param[in] Ch          - Channel index at socket level
  @param[in] Dimm        - DIMM slot index within DDR channel
  @param[in] Priority    - 0:Low Priotity, 1:High Priority.
                           There are two buckets of errors. Each bucket can be
                           programmed to a particular signal scheme.
  @param[in] Signal      - BIT0 - SMI, BIT1 - ERROR0#, BIT2 - MSMI#
                           (all mutually exclusive)

  @retval EFI_SUCCESS           - signal programmed successfully
  @retval EFI_INVALID_PARAMETER - given DIMM is not NGN DIMM
**/
EFI_STATUS
EFIAPI
McDdrtErrorLogProgramSignal (
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 Priority,
  IN UINT8 Signal
  );

/**
  @brief Detects the NGN DIMM error and returns 24-bit log.

  @param[in] Socket         - Socket index at system level
  @param[in] Ch             - Channel index at socket level
  @param[in] Dimm           - DIMM slot index within DDR channel
  @param[in] Priority       - 0 for low priority event, 1 for high
  @param[in] ClearOnRead    - when set, clears the error when a NGN DIMM
                              is found and reported
  @param[out] *EventData    - DDRT_EVENT_DATA outcome

  @retval EFI_SUCCESS             - A NGN DIMM with error was found, pLog updated
  @retval EFI_INVALID_PARAMETER   - Given DIMM is not NGN DIMM or Log is NULL
  @retval EFI_NOT_FOUND           - No error found on NGN DIMM, Log = 0
**/
EFI_STATUS
EFIAPI
McDdrtErrorLogDetectError (
  IN  UINT8   Socket,
  IN  UINT8   Ch,
  IN  UINT8   Dimm,
  IN  UINT8   Priority,
  IN  BOOLEAN ClearOnRead,
  OUT DDRT_EVENT_DATA  *EventData
  );

  /**

  This function applies the given CKE override to the bits specified by ckeMask

  @param[in] Host      - Pointer to sysHost
  @param[in] socket    - Socket
  @param[in] ch        - Channel
  @param[in] ckeMask   - CKE settings to apply
  @param[in] assertCke - Flag to ASSERT/DEASERT CKE

  @retval none

**/
VOID
SetCkeMask (
  IN      PSYSHOST  Host,
  IN      UINT8     socket,
  IN      UINT8     ch,
  IN      UINT8     ckeMask,
  IN      UINT8     assertCke
  );

///
// CKE override mask
//
#define CKE_OVERRIDE_ALL_RANK 0xFF
/**

  Overrides CKE value

  @param[in]  Socket             - Current Socket (0-Based)
  @param[in]  Channel            - Current Channel (0-Based)
  @param[in]  Mask               - Mask to get overwritten (Bitwise)
  @param[in]  Value              - Specify value for CKE (Bitwise)

  @retval  EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
OverrideCke (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          Mask,
  IN        UINT32          Value
  );

/**
  Checks if CmdTxEq training needs to be skipped due to configuration

  Checks what processor is running and memory configuration and DDR frequency to evaluate if
  CmdTxEq needs to be skipped

  @retval TRUE if the training needs to be skipped
          FALSE if the training needs to be executed
**/
BOOLEAN
ShouldChipSkipCmdTxEq (
  VOID
  );

/**

  Disable all the channel based and dimm based DRAM throttling

  @param[in] Socket   - Socket index

  @retval None

**/
VOID
DisableDramThrottling (
  IN UINT8   Socket
  );


/**
  Overrides the global command timing mode variable setting if required

  Checks if there is any silicon specific requirement to override the
  command timing mode (i.e. a silicon workaround) and overrides the
  timing mode variable if required.

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Channel   - Channel

  @retval N/A
**/
VOID
EFIAPI
SiliconSpecificOverrideCommandTimingMode (
  IN      PSYSHOST  Host,
  IN      UINT8     Socket,
  IN      UINT8     Channel
  );

/**

  Programs turnaround times. Unused in 10nm.

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket number

  @retval SUCCESS

**/
UINT32
TurnAroundTimings (
  IN PSYSHOST Host,
  IN UINT8 Socket
  );

/**

  Compares turnaround times with HAS table of expected values
  reports via scratchpad if worse than expected.

  Unused in 10nm.

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param Socket  - Socket number

  @retval SUCCESS

**/
UINT32
CheckTurnAroundTimings (
  IN PSYSHOST Host,
  IN UINT8 Socket
  );

/**

  Get the DDRT write latency (tCWL/tWL).

  This makes any positive and negative adjustments to the raw value before returning it.

  @param[in]  Socket  Target socket
  @param[in]  Channel Target channel

  @retval The DDRT write latency (tCWL/tWL)

**/
UINT8
GetDdrtWriteLatency (
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**

  Returns the DCPMM data path delay value.

  @param[in] Socket         - Socket number
  @param[in] Channel        - Channel number
  @param[in] DdrtToDdr4DataPathDelay  - DDRT data path delay
  @param[in] Ddr4ToDdrtDataPathDelay - DDRT data path delay 2

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
GetDcpmmDataPathDelay (
  IN UINT8   Socket,
  IN UINT8   Channel,
  IN UINT32  *DdrtToDdr4DataPathDelay,
  IN UINT32  *Ddr4ToDdrtDataPathDelay
  );

/**

  Read Chip Wr ADD Delays

  Unused in 10nm.

  @param[in] Host                 - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket               - Socket number
  @param[in] Channel              - Channel number
  @param[in] CRAddDelay           - Delay
  @param[in] WLChipCleanUpStruct  - Cleanup struct

  @retval SUCCESS

**/

VOID
EFIAPI
ReadChipWrADDDelays (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN INT16 *CRAddDelay,
  IN PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  );

/**

  Set MCA DDRT Thermal Event

  @param[in] Socket                  - Socket number
  @param[in] Channel                 - Channel number
  @param[in] Value                   - TRUE: Disable
                                       FALSE: Enable

  @retval n/a

**/
VOID
EFIAPI
SetMcaDdrtThermalEvent (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN BOOLEAN   Value
);

/**

  Disable DDRT Thermal Event in defeature register

  @param[in] Socket                  - Socket number
  @param[in] Channel                 - Channel number
  @param[in] InNormalMode            - True: In normal mode
                                       False: Not in normal mode

  @retval n/a

**/
VOID
EFIAPI
SetDefeatureThermalEvent (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN BOOLEAN   InNormalMode
  );

/**

  Mc Init for DisableDirForAppDirect

  @param[in] MemTechType            Memory technology type.
  @param[in] Socket                 Socket index.
  @param[in] Ch                     Channel index within socket.
  @param[in] DisableDir4AppDirect   Value to program disabledir4appdirect field.

  @retval N/A

**/
VOID
EFIAPI
McDisableDirForAppDirect (
  IN  MEM_TECH_TYPE      MemTechType,
  IN  UINT8              Socket,
  IN  UINT8              Ch,
  IN  UINT8              DisableDir4AppDirect
  );

/**

  This function sets the AllowDirUpdToBlock field of SbdFeatures0 register.

  @param[in] MemTechType            Memory technology type.
  @param[in] SocketId               Socket index.
  @param[in] ChId                   Channel index within socket.
  @param[in] AllowDirUpdToBlock     Data to be written to AllowDirUpdToBlock field.

**/
VOID
EFIAPI
McSetAllowDirUpdToBlock (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          ChId,
  IN UINT8          AllowDirUpdToBlock
  );

/**

  This function sets the EnEarlyCmp field of SysFeatures0 register.

  @param[in] MemTechType            Memory technology type.
  @param[in] SocketId               Socket index.
  @param[in] ChId                   Channel index within socket.
  @param[in] EnEarlyCmp             Data to be written to EnEarlyCmp field.

**/
VOID
EFIAPI
McSetEnEarlyCmp (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          ChId,
  IN UINT8          EnEarlyCmp
  );

/*++

  This routine determines that the various indexes associated with each rank

  @param[in] Host  - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch    - Channel number (0-based)
  @param[in] Dimm  - DIMM number (0-based)
  @param[in] Rank  - Rank number (0-based)

  @retval N/A

--*/
VOID
EFIAPI
SetRankIndexes (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Rank
);

/**

  Set MCA CTL for Unexpected Pkg CMI Idle event

  @param[in] Socket                  - Socket number
  @param[in] Channel                 - Channel number
  @param[in] Value                   - 1: Disable
                                       0: Enable

  @retval n/a

**/
VOID
EFIAPI
SetMcaCmiIdleEvent (
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Value
  );

/**

  Enable or disable CADB

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Enable      - TRUE - enable CADB; FALSE- disable CADB

  @retval None

**/

VOID
EFIAPI
SetCadbModeEn (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT32    ChBitmask,
  IN    BOOLEAN   Enable
  );

/**
  Determine if the current platform has any DCPMM(Intel Optane DC Persistent Memory Module) is populated

  @retval   TRUE if any DCPMM is populated
  @retval   FALSE otherwise

**/
BOOLEAN
IsDcpmmPresentOnPlatform (
  VOID
  );

/**
  Limit the round trip values as needed for a given socket.

  @param[in]  Socket  The target socket

  @retval   EFI_SUCCESS   Round trip values were successfully limited, or they did not need limiting
  @retval   !EFI_SUCCESS  Round trip values needed to be limited, but limiting was not successful
**/
EFI_STATUS
EFIAPI
LimitRoundTripAsNeeded (
  IN  UINT8 Socket
  );

/**
  Checks to see if the DDRT Round Trip value timing should be used.

  @param[in]  Socket                - Socket Number
  @param[in]  Channel               - DDR Channel ID

  @retval  TRUE                     - DDRT Round Trip value should be used
  @retval  FALSE                    - Default Round Trip value should be used
**/
BOOLEAN
EFIAPI
DdrtRoundTripIsNeededMc (
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**
  This function programs DRAM_RULE_CFG*.fm_target to 0.

  @param[in] ScktId           Socket index.
  @param[in] McId             Memory controller index on socket.
  @param[in] ChId             Channel index on memory controller.
  @param[in] RegId            Register Index of MC DRAM Rule.

  @retval N/A
**/
VOID
EFIAPI
McClearFmTarget (
  IN UINT8   SocketId,
  IN UINT8   McId,
  IN UINT8   ChId,
  IN UINT8   RegId
  );

/**
  This function enables metadata ECC correction.

  @param[in] ScktId           Socket index.
  @param[in] McId             Memory controller index on socket.
  @param[in] ChId             Channel index on memory controller.

  @retval N/A
**/
VOID
EFIAPI
McSetMetaEccCorrBit (
  IN UINT8   ScktId,
  IN UINT8   McId,
  IN UINT8   ChId
  );

/**
  This function enables VC preemption by setting scheduler_enables_bs.enable_vc_preemption.

  @param[in] ScktId           Socket index.
  @param[in] McId             Memory controller index on socket.
  @param[in] ChId             Channel index on memory controller.
  @param[in] EnVcPreemption   Flag indicates if VC preemption needs to be enabled.

  @retval N/A
**/
VOID
EFIAPI
McSetVcPreemptionBit (
  IN UINT8   ScktId,
  IN UINT8   McId,
  IN UINT8   ChId,
  IN BOOLEAN EnVcPreemption
  );

/**
  This function disables the ADR actions per channel.

  @param[in] ScktId            Socket index.
  @param[in] McId              Memory controller index on socket.
  @param[in] AdrDisableBitMap  Bitmap indicates if ADR needs to be disabled on the channels of the specified MC.
                               Disable = 1 and Enable = 0. One bit corresponds to one channel.

  @retval N/A
**/
VOID
EFIAPI
McSetAdrDisableBits (
  IN UINT8   ScktId,
  IN UINT8   McId,
  IN UINT8   AdrDisableBitMap
  );

/**
  This function checks if the fuse of Persistent Memory with Crystal Ridge is
  disabled on system.

  @param VOID

  @retval TRUE                 The fuse of persistent memory is disabled.
  @retval FALSE                The fuse of persistent memory is enabled.
**/
BOOLEAN
EFIAPI
McCheckCrPmemFuseDisabled (
  VOID
  );

/**

  Setup PI settings parameters

  @param[in]   Socket           - CPU Socket Node number (Socket ID)
  @param[in]   Step             - The PI step to work on
  @param[out]  PiSetting        - Pointer of the PI setting returned
  @param[out]  PiStart          - Pointer of test starting PI
  @param[out]  NumberOfCycle    - Pointer for number of cycles

  @retval None

**/

VOID
EFIAPI
SetupPiSettingsParamsDdr5 (
  IN  UINT8    Socket,
  IN  UINT8    Step,
  OUT UINT16   *PiSetting,
  OUT UINT16   *PiStart,
  OUT UINT16   *NumberOfCycle
  );

/**

  Returns a ODT stretch

  @param[in]  Socket           - Socket Number
  @param[in]  Channel          - Channel Number
  @param[in]  Selection        - Selection for SameRank, DiffRank, DiffDimm

  @retval  TRUE  - ODT is asserted
           FALSE - ODT is not asserted

**/
BOOLEAN
CheckOdtAsserted (
  IN UINT8                       Socket,
  IN UINT8                       Channel,
  IN MRC_ODTASSERTION_SELECTION  Selection
);

/**
  This function gets the transfer speed of DDR/HBM from fuse shadow register.

  @param[in] MemTechType       Memory technology type.
  @param[in] SocketId          Socket index.
  @param[in] ChId              Channel index on socket.

  @retval  The value of transfer speed.
**/
UINT8
McGetTransferSpeed (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          ChId
  );

///
///  Read to Read Turnaround Timing
///
typedef enum {
  TRrdrType = BIT0,           ///< Back to back READ to READ from different RANK separation parameter
  TRrddType = BIT1,           ///< Back to back READ to READ from different DIMM separation parameter.
  TRrMaxtype = 0xFF           ///< End of Enum value
} READ_TO_READ_TURNAROUND_TYPE;

///
///  Read to Read Turnaround Timing
///
typedef struct {
  UINT8 TRrdr;           ///< Back to back READ to READ from different RANK separation parameter
  UINT8 TRrdd;           ///< Back to back READ to READ from different DIMM separation parameter.
} READ_TO_READ_TURNAROUND_STRUCT;

/**
  Get original read to read timing value based on the enum parameter passed


  @param[in]  Socket           - Socket Number
  @param[in]  Ch          -      Channel Number
  @param[in]  TimingType       - Entry from READ_TO_READ_TURN_AROUND_TYPE
  @param[in]  *Value           - Value from register

  @retval EFI_SUCCESS - Successfully set the value
  @retval EFI_UNSUPPORTED - TimingType not supported

**/
EFI_STATUS
EFIAPI
GetReadToReadTimingOrg (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT32   TimingType,
  IN READ_TO_READ_TURNAROUND_STRUCT *ReadtoReadTiming
  );


/**
  Get read to read timing value based on the enum parameter passed


  @param[in]  Socket           - Socket Number
  @param[in]  Ch          -      Channel Number
  @param[in]  TimingType       - Entry from READ_TO_READ_TURN_AROUND_TYPE
  @param[in]  *Value           - Value from register

  @retval EFI_SUCCESS - Successfully set the value
  @retval EFI_UNSUPPORTED - TimingType not supported

**/
EFI_STATUS
EFIAPI
GetReadToReadTiming (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT32   TimingType,
  IN READ_TO_READ_TURNAROUND_STRUCT *ReadtoReadTiming
  );

/**
  Set read to read timing value based on the enum parameter passed

  @param[in]  Socket           - Socket Number
  @param[in]  Ch               - Channel Number
  @param[in]  TimingType       - Entry from READ_TO_READ_TURN_AROUND_TYPE
  @param[in]  Value            - Value to program

  @retval EFI_SUCCESS - Successfully set the value
  @retval EFI_UNSUPPORTED - TimingType not supported
**/
EFI_STATUS
EFIAPI
SetReadToReadTiming (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT32   TimingType,
  IN READ_TO_READ_TURNAROUND_STRUCT  *ReadtoReadTiming
  );

/**

  Set HBM MC Mode

  @param[in]                Socket             Current Socket (0-Based)
  @param[in]                HbmCh              Current HBM Ch (0-Based)

**/
VOID
EFIAPI
HbmSetMcMode (
  IN        UINT8           Socket,
  IN        UINT8           HbmCh
  );

/**

  Initialize HBM MC page policy

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Ch             Current Ch (0-Based)

**/
VOID
EFIAPI
HbmInitPagePolicy (
  IN        UINT8           Socket,
  IN        UINT8           Ch
  );

/**

  Initialize HBM MC page policy

  @param[in]                Socket         Current Socket (0-Based)
  @param[in]                Ch             Current Ch (0-Based)
  @param[in]                EnableDbi      BOOLEAN, to enable DBI or disable it

**/
VOID
EFIAPI
HbmInitDbi (
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        BOOLEAN         EnableDbi
  );

/**
  This function programs HBM MC registers required after training and before normal mode.

  @param[in] SocketId                     Socket index.
  @param[in] ChId                         Channel index within socket.

  @retval MRC_STATUS_SUCCESS              This function is executed successfully.
  @retval MRC_STATUS_UNSUPPORTED          Register programming is not supported.
  @retval MRC_STATUS_INVALID_PARAMETER    Some of the input parameters are invalid.
**/
MRC_STATUS
EFIAPI
HbmMcLateConfig (
  IN UINT8     SocketId,
  IN UINT8     ChId
  );

/**

  Program HBM MC into normal mode

  @param[in] Socket                 Socket index (0-Based).
  @param[in] Ch                     Channel index within Socket (0-Based).

  @retval EFI_SUCCESS               HBM MC were programmed into normal mode.
  @retval EFI_UNSUPPORTED           Register programming for normal mode is unsupported.
  @retval EFI_INVALID_PARAMETER     Some of input parameters are invalid.

**/
EFI_STATUS
EFIAPI
HbmMcSetNormalMode (
  IN UINT8    Socket,
  IN UINT8    Ch
  );

/**

  Program HBM DIMM MTR register

  @param[in]   Socket    Current Socket (0-Based)

  @retval  EFI_SUCCESS             This function completed execution with no warnings/errors
  @retval  EFI_INVALID_PARAMETER   Invalid density detected on one of HBM IO modules.

**/
EFI_STATUS
EFIAPI
HbmDimmMtrConfig (
  IN        UINT8           Socket
  );


/**

  Configures registers for Memory hot feature

  @param[in] Socket  - Socket Id, 0 based

  @retval N/A

**/
VOID
HbmConfigureMemHotFeature (
  IN UINT8 Socket
  );

/**
  This function toggles the reset_io register field.

  @param[in] SocketId             Socket index.

  @retval MRC_STATUS_SUCCESS             This function is executed successfully.
  @retval MRC_STATUS_UNSUPPORTED         The toggling of reset_io is not supported.
  @retval MRC_STATUS_INVALID_PARAMETER   Some of the input parameters are invalid.
**/
MRC_STATUS
EFIAPI
HbmMcSetResetIo (
  IN UINT8   SocketId
  );

/**

  Mc Cmd Type Override Enable And Encoding Program

  @param[in]  Socket                   Current Socket (0-Based)
  @param[in]  Ch                       Current Ch (0-Based)
  @param[in]  CmdTypeOverrideEnable    Command Type Override Enable Value
  @param[in]  CmdTypeEncoding          Command Type Encoding Value

  @retval  NA

**/
VOID
EFIAPI
McCmdTypeOverrideEnableAndEncoding(
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 CmdTypeOverrideEnable,
  IN UINT8 CmdTypeEncoding
  );

/**

  Apply settings before MemInit starts

  @param Socket  - Socket number

  @retval EFI_UNSUPPORTED - Function is not supported
  @retval EFI_SUCCESS     - Otherwise

**/
EFI_STATUS
EFIAPI
PreMemInit (
  IN  UINT8       Socket
  );

/**

  Apply settings before Mem Test

  @param[in]      Socket         - Socket number
  @param[in, out] MemTestConfig  - Address of a pointer to MemTestConfig structure

  @retval  MRC_FUNCTION_NOT_IMPLEMENTED     - Function not implemented
           MRC_STATUS_FAILURE               - Pre-mem test execution failure
           MRC_STATUS_SUCCESS               - Otherwise

**/
MRC_STATUS
EFIAPI
PreMemTest (
  IN     UINT8            Socket,
  IN OUT MEM_TEST_CONFIG  **MemTestConfig
  );

/**

  Apply settings after Mem Test

  @param[in] Socket         - Socket number
  @param[in] MemTestConfig  - Address of a pointer to MemTestConfig structure

  @retval  MRC_FUNCTION_NOT_IMPLEMENTED     - Function not implemented
           MRC_STATUS_FAILURE               - Post-mem test execution failure
           MRC_STATUS_SUCCESS               - Otherwise

**/
MRC_STATUS
EFIAPI
PostMemTest (
  IN  UINT8            Socket,
  IN  MEM_TEST_CONFIG  **MemTestConfig
  );

/**

  Chip specific code path to write Chip Wr Addr delays for DDR5

  @param[in] Host                - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket              - Socket Id
  @param[in] Ch                  - Channel number (0-based)
  @param[in] SubCh               - Sub Channel number (0-based)
  @param[in] SetType             - Set type. 0 = everything
  @param[in] OffsetValue         - Offset for byte lanes
  @param[in] CRAddDelay          - Add delays

  @retval None

**/
VOID
EFIAPI
WriteChipWrADDDelaysDdr5 (
  IN  PSYSHOST                Host,
  IN  UINT8                   Socket,
  IN  UINT8                   Ch,
  IN  UINT8                   Subch,
  IN  UINT8                   SetType,
  IN  INT16                   OffsetValue,
  IN  INT16                   CRAddDelay,
  IN  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  );

/**
  Return tRFC and nRFC based on refresh mode policy
  Apply MC-specific overrides

  @param[in]  Socket      - Socket to train
  @param[in]  Ch          - DDR channel to setup
  @param[in]  Dimm        - Dimm number the rank is on
  @param[in]  RefreshMode - Refresh mode policy setting
  @param[out] tRFC        - tRFC returned in units of ns
  @param[out] nRFC        - nRFC returned in units of tCLK

  @retval MRC_STATUS_SUCCESS if tRFC and nRFC are valid
  @retval MRC_STATUS_FAILURE otherwise

**/
MRC_STATUS
EFIAPI
GetTrfcMcOverride (
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  IN  UINT8         Dimm,
  IN  REFRESH_MODE  RefreshMode,
  OUT UINT16        *tRFC,
  OUT UINT16        *nRFC
);

/**

  Get CMI Setup program choice.

  @retval   CMI_AUTO_OPTION - Program values from Excel sheet
            CMI_REGISTER_DEFAULTS Do not program any cmi credit, use register defaults.

**/
UINT8
GetCmiSetupProgramValue (
  VOID
);

/**

  Program Mem Mc CMI registers for Hbm

  @param[in] ProgramableValue[MAX_HBM_CH]    - Array containing type of value to program per channel (From 0 to Max Channel in Socket)
  @param[in] Socket                          - Socket number

  @retVal   1 CMI Registers initialized
            0 CMI Registers not initialized
**/
UINT32
EFIAPI
ProgramMemMcCmiRegistersHbm (
  IN UINT8 ProgramableValue[],
  IN UINT8 Socket
  );

/**

  SetMcCmiConfigDoneChHbm.

  @param Socket       - Socket ID
  @param HbmCh        - Hbm Channel ID

  @retval N/A

**/
VOID
EFIAPI
SetMcCmiConfigDoneChHbm (
  IN UINT8    Socket,
  IN UINT8    HbmCh
  );

/**
  Build Target function to wait for Cmi misc channels to complete.

  @param[in]   Socket                 Socket Number.
  @param[in]   HbmCh                  HbmCh

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/
EFI_STATUS
EFIAPI
WaitForMcMiscCmiCreditConfigCompleteTargetHbm (
  IN UINT8 Socket,
  IN UINT8 HbmCh
  );

/**
  Build Target function to wait for Mc Cmi Req Cpcfg to complete.

  @param[in]   Socket           Socket Number.
  @param[in]   HbmCh            Hbm Channel Id.

  @retval EFI_SUCCESS           Completed successfully.
  @retval !EFI_SUCCESS          Error.

**/
EFI_STATUS
EFIAPI
WaitForMcCmiReqCpcfgCompleteTargetHbm (
  IN UINT8 Socket,
  IN UINT8 HbmCh
  );

/**
  This function sets the wcilfoptdis bit field of MCNMCACHINGCFG_MC_MAIN_STRUCT register.

  @param[in] SocketId         Socket index.
  @param[in] ChId             Channel index within socket.
  @param[in] WcilfOptDis      Flag indicates if Wcilf optimization needs to be disabled.

**/
VOID
EFIAPI
McSetNmCachingCfgWcilfOptDis (
  IN UINT8    SocketId,
  IN UINT8    ChId,
  IN BOOLEAN  WcilfOptDis
  );

/**
  Initialize or cleanup MC settings for DRAM Duty Cycle Adjuster training

  @param[in] Socket             - Socket to initialize
  @param[in] ChannelMask        - One-hot encoding to mask a channel (0: Configure channel, 1: Skip channel)
  @param[in] DoInitNotRestore   - TRUE to perform initialization needed for DRAM DCA training
                                  FALSE to restore MC to original settings

  @retval MRC_FUNCTION_NOT_IMPLEMENTED if function not implemented
  @retval MRC_STATUS_SUCCESS otherwise
**/
MRC_STATUS
EFIAPI
DramDcaMemoryControllerConfiguration (
  IN    UINT8     Socket,
  IN    UINT32    ChannelMask,
  IN    BOOLEAN   DoInitNotRestore
  );

/**

  Cache and Clear RDB entries for LRDIMM and DDRT

  @param[in]  Socket                   Current Socket (0-Based)

  @retval  NA

**/
VOID
EFIAPI
CacheAndClearRidEntries (
  IN  UINT8    Socket
  );

/**

  Restore RDB entries for LRDIMM and DDRT

  @param[in]  Socket                   Current Socket (0-Based)

  @retval  NA

**/
VOID
EFIAPI
RestoreRidEntries (
  IN  UINT8    Socket
  );

#if FixedPcdGetBool(PcdMcrSupport)
/**

  Enable or disable CADB and update MCR mode

  @param[in] Socket      - Socket number
  @param[in] ChBitmask   - Channel bit mask
  @param[in] Enable      - TRUE - enable CADB and MCR mode; FALSE- disable CADB and MCR Mode

  @retval None

**/
VOID
EFIAPI
SetCadbAndMcrMode (
  IN    UINT8     Socket,
  IN    UINT32    ChBitmask,
  IN    BOOLEAN   Enable
  );

/**

  Enable / Disable MCR mode

  @param[in] Socket         - Socket number
  @param[in] DdrChEnMap     - Bit mask of channels to use
  @param[in] SubChEnMap     - Bit mask of sub-channels to use
  @param[in] EnableMcrMode  - TRUE - Enable MCR Mode, FALSE- Disable MCR Mode

  @retval None

**/
VOID
EFIAPI
SetCadbMcrMode (
  IN  UINT8     Socket,
  IN  UINT32    DdrChEnMap,
  IN  UINT8     SubChEnMap,
  IN  BOOLEAN   EnableMcrMode
  );
#endif // FixedPcdGetBool(PcdMcrSupport)
#endif // #ifndef  _MEM_MC_IP_LIB_H_
