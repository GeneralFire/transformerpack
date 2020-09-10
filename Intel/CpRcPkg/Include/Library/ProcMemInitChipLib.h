/** @file
  ProcMemInitChipLib.h

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

#ifndef  __PROC_MEM_INIT_CHIP_H__
#define  __PROC_MEM_INIT_CHIP_H__

#include <Library/MemoryCoreLib.h>
#include <Library/PipeSyncLib.h>
#include <ReferenceCodeDataTypes.h>
#include "SysHost.h"
#include <UsraAccessType.h>

#include "MemHostChipCommon.h"
#include "MrcCommonTypes.h"
#include <Memory/CpgcDefinitions.h>

#include <MemDecodeCommonIncludes.h>
#include <Library/Ddr5CoreLib.h>

VOID
EarlyUncoreInit (
  PSYSHOST Host
  );

VOID
EarlyCpuInit (
  PSYSHOST Host
  );

VOID
LateUncoreInit (
  PSYSHOST Host
  );

VOID
FixupPamAttributes (
  PSYSHOST Host
  );

VOID
InitializeDefaultData (
  PSYSHOST Host
  );

VOID
InitializePlatformData (
  PSYSHOST Host
  );

VOID
CheckAndHandleResetRequests (
  PSYSHOST Host
  );

//
// Maximum serial debug message for the FatalErrSetMaxSDBGSelect function
//
#define MAX_SDBG_FATAL_ERR_CHECK   0
#define MAX_SDBG_FATAL_ERR_SET     1
#define MAX_SDBG_FATAL_ERR_CLEAR   2

BOOLEAN
FatalErrSetMaxSDBGSelect (
  PSYSHOST Host,
  UINT8 MaxSDBGFunction
  );

MRC_STATUS
GetMemSubsystemMap (
  IN      PSYSHOST      Host,                   // Pointer to Host structure (outside scope of this spec)
  IN      UINT32        Socket,                 // Processor socket within the system (0-based)
  IN OUT  UINT32        *NumMemSs,              // Returns number of memory subsystems in the socket
  IN OUT  MRC_MSM       *MemSsList              // Returns ordered list of memory subsystems
  );

VOID
DisplayTcwlAdj (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  MRC_GT group
  );

VOID
DisplayXoverResultsFnv (
  PSYSHOST Host,
  UINT8 socket
  );

VOID
DisplayResultsChip (
  PSYSHOST Host,
  UINT8 socket
  );

/*++
  Displays the comp register contents

  @param[in] Socket:       Processor socket

  @retval N/A

--*/
VOID
DisplayCompRegResults10nm (
  IN UINT8     Socket
  );

VOID
SetRestoreTimings (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch
  );

/**
  Multi-use function to either get or set control delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param ioGroup  - Group number
  @param side     - Side number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param value    - Value to program or offset

  @retval Pi delay value

**/
MRC_STATUS
GetSetCtlDelay (
  PSYSHOST  Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8     socket,                           // Processor socket within the system (0-based)
  UINT8     ch,                               // DDR channel number within the processor socket (0-based)
  UINT8     ioGroup,                          // Group number
  UINT8     side,                             // Side number
  UINT8     mode,                             // GSM_READ_CSR - Read the data from hardware and not cache
                                              // GSM_READ_ONLY - Do not write
                                              // GSM_WRITE_OFFSET - Write offset and not value
                                              // GSM_FORCE_WRITE - Force the write
  INT16     *value                            // Value to program or offset
  );

/**

  Quick centering of Vref

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket number
  @param group   - Parameter to center
                   RxDqsDelay   0: Center Rx DqDqs
                   TxDqDelay    1: Center Tx DqDqs

  @retval SUCCESS

**/
UINT8
CmdVrefQuick (
  PSYSHOST  Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8     socket,                           // Processor socket within the system (0-based)
  MRC_GT    group                             // Data group to access
  );

/**

  Detect SPD Type DIMM configuration

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket ID
  @param ch      - Channel on socket
  @param dimm    - DIMM on channel
  @param keyByte - Key byte

  @retval SUCCESS

**/
UINT32
DetectSpdTypeDIMMConfig (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch,                               // Channel number (0-based)
  UINT8       dimm,                             // Current dimm
  UINT8       keyByte                           // Key byte holds the DIMM type information
  );

/**

    Print Memory Setup Options

    @param Host   - Pointer to sysHost, the system Host (root) structure
    @param socket - Current socket

    @retval None

**/
VOID
PrintMemSetupOptionsChip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Processor socket ID
  );

/**

  Wait for the Mailbox interface to indicate it is ready

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - Processor socket ID

  @retval None
**/
VOID
WaitForMailboxReady (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Socket Id
  );

/**

  Notify all sockets if NVMDIMM is populated on any socket, and check for surprise clock stop

  @param None

  @retval None
**/
VOID
DetectSurpriseClockStop (
  VOID
  );

/*++

  Attempt Cold Boot Fast

  @param Host   - Pointer to sysHost, the system Host (root) structure

  @retval TRUE
  @retval FALSE

--*/
UINT32
AttemptColdBootFastChip (
  PSYSHOST    Host                             // Pointer to sysHost, the system Host (root) structure
  );

/*++

  Detect IMC Enabled Change

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
UINT32
DetectIMCEnabledChangeChip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Socket Id
  );

/*++

  Detect MCA Errors

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - socket id
  @param logData - pointer to 32-bit lag data

  @retval TRUE
  @retval FALSE

--*/
UINT32
DetectMCAErrorsChip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT32      *logData                          // Pointer to log data
  );

/*++

  Detect if the processor has been changed

  @param Host  - Pointer to sysHost, the system Host (root) structure

  @retval TRUE @retval FALSE

--*/
UINT32
DetectProcessorRemovedChip (
  PSYSHOST    Host                              // Pointer to sysHost, the system Host (root) structure
  );

/*++

  Check if channel disabled in HW

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket id
  @param ch     - ch id

  @retval TRUE
  @retval FALSE

--*/
UINT32
GetChnDisableChip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // Socket Id
  UINT8       ch                                // Channel number (0-based)
  );

/*++

  Check if 2 DPC is disabled in HW

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
UINT32
GetDisable2dpcChip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Socket Id
  );

/*++

  Check if 3 DPC is disabled in HW

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
UINT32
GetDisable3dpcChip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Socket Id
  );

/**

  Determines if quad-rank DIMMs are enabled or disabled by the hardware

  If MEM_IP_VER is MEM_IP17ww51f or greater, then Host->var.common.procCom[socket].capid3
  must be initialized with the contents of the capid3 register before calling this function.

  @param[in]  Host    Pointer to the system Host (root) structure
  @param[in]  Socket  Socket ID

  @retval TRUE  Support for quad-rank DIMMs is disabled in the hardware
  @retval FALSE Support for quad-rank DIMMs is enabled in the hardware

**/
BOOLEAN
GetDisableQRDimmChip (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket
  );

/*++

  Gets Processor PPINs

  @param Host  - Pointer to sysHost, the system Host (root) structure

  @retval (UINT64_STRUCT)procPpin

--*/
UINT64_STRUCT
GetPpinChip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Socket Id
  );

/*++

  Initialize DDR Frequench

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket id

  @retval None

--*/
VOID
InitDdrFreqChip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Socket Id
  );

/*++

  Initialize internal data structures

  @param Host   - Pointer to sysHost, the system Host (root) structure

  @retval TRUE
  @retval FALSE

--*/
UINT32
SavedSetupDataChangedChip (
  PSYSHOST    Host                              // Pointer to sysHost, the system Host (root) structure
  );

/*++

  Initialize internal data structures

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket id

  @retval TRUE
  @retval FALSE

--*/
UINT32
SavedSetupDataChangedIMCChip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Socket Id
  );

/*++

  Set IMC Enabled

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket id

  @retval None

--*/
VOID
SetIMCEnabledChip (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket                            // Socket Id
  );

/*++

  Set Setup Options

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket id

  @retval None

--*/
VOID
SetSetupOptionsChip (
  PSYSHOST    Host                              // Pointer to sysHost, the system Host (root) structure
  );

/**

  Gets the type of parameter

  @param Host  - Pointer to sysHost, the system Host (root) structure
  @param Param - Parameter input to convert to parameter type

  @retval (UINT8) Parameter type

**/
UINT8
GetChipParamType (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       Param                             // Parameter input to convert to parameter type
  );

/**

  Check the DIMM Type

  @param Host - Pointer to sysHost, the system Host (root) structure

  @retval SUCCESS - DDR4
  @retval FAILURE - Anything except DDR4

**/
UINT32
CheckDimmType (
  PSYSHOST    Host                              // Pointer to sysHost, the system Host (root) structure
  );

/**

 Determine if we train a given rank for a given group

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket
  @param ch      - Channel
  @param dimm    - Dimm
  @param rank    - Rank
  @param level   - DdrLevel or LrbufLevel
  @param group   - Tx/RxVref, Tx/RxDq/Dqs, etc

  @retval 1 - skip
  @retval 0 - train

**/
UINT8
CheckSkipRankTrain (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank,
  MRC_LT level,
  MRC_GT group
  );

/**

    Checks if the processor is BDX EP

    @param Host - Pointer to the system Host (root) structure

    @retval TRUE if BDX EP SKU is detected
    @retval FALSE if not BDX EP SKU detected.

**/
UINT8
IsBDXEP (
  PSYSHOST    Host                              // Pointer to sysHost, the system Host (root) structure
  );

/**

    Checks if the processor is DE

    @param Host - Pointer to the system Host (root) structure

    @retval TRUE if DE SKU is detected
    @retval FALSE if not DE SKU detected.

**/
UINT8
IsDESku (
  PSYSHOST    Host                              // Pointer to sysHost, the system Host (root) structure
  );

/**

  Set gnt2erid register

  @param Host   - Pointer to sysHost, the system Host (root) structure struct
  @param socket - Socket Id
  @param ch     - Channel number (0-based)
  @param dimm   - Current dimm
  @param data   - Data to set

  @retval None

**/
VOID
SetSxpGnt2Erid (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT32 data
  );

VOID
GetWpqRidtoRt (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  );

/**

  Find Index Resource Data

  @param strobe - Strobe number (0-based)

  @retval Index

**/
UINT8
FindIndexResData (
  UINT8 strobe
  );

/**

  Checks to see if the TxDqDqs is too close to the zero encoding
  Adjusts TxDqDqs if this is the case for both directions

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - current socket
  @param[in] Direction  - DENORMALIZE (-1) or RENORMALIZE (1)
  @param[in] ExtraRange - Range to pull in

  @retval SUCCESS - Function executed successfully.

**/
UINT32
EFIAPI
WLPushOut (
  IN PSYSHOST  Host,
  IN UINT8 socket,
  IN INT8 Direction,
  IN UINT8 ExtraRange
  );

VOID
MultiThreadS3ResumeScratchPadRestore (
  UINT8                 socket,
  UINT32                *Buffer,
  UINT32                DwordCount,
  UINT32                *DstBuffer
  );

UINT32
GetDispatchPipeCsr (
  UINT8                   socket
  );

VOID
WriteDispatchPipeCsr (
  UINT8                 socket,
  UINT32                data
  );

/**

  Abstract all knowledge of scratchpad registers from the core

  @param socket     - Socket number
  @param checkpoint - The major and minor codes combined in a UINT16 value

  @retval UINT32 - returns the value of the data that was written to the scratchpad register.

**/
UINT32
SetPostCode (
  UINT8                   socket,
  UINT16                  checkpoint,
  UINT16                  data
  );

/**

  Abstract all knowledge of scratchpad registers from the core

  @param socket  - Socket number

  @retval UINT32 - returns the scratchpad value for the breakpoint.

**/
UINT32
GetBreakpointCsr (
  UINT8                   socket
  );

/**

  Set the break point scratch pad

  @param[in] Socket       - Socket number
  @param[in] CheckPoint   - Check point to break on: 0xMMmm, where MM = Major Code, mm = Minor Code

  @retval N/A

**/
VOID
SetBreakpointCsr (
  IN    UINT8     Socket,
  IN    UINT32    CheckPoint
  );

/**

  Abstract all knowledge of scratchpad registers from the core

  @param socket  - Socket number

  @retval UINT32 - returns the scratchpad value for the SSA Agent Opcode.

**/
UINT32
GetSsaAgentData (
  UINT8                   socket
  );

/**

  Abstract all knowledge of scratchpad registers from the core

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

  @retval VOID

**/
VOID
ClearSsaAgentData (
  UINT8                   socket
  );

//
// Definitions for use with GetSetValFunctions Mode parameter
//

#define GET_REQ       0
#define RESTORE_REQ   1

UINT32
GetSetValFunctions (
  UINT8                   socket,
  UINT8                   mode,
  UINT32                  *valFuncs
  );

/**
  Return pointer to SimRegs table and table length

  @param[in    ] Host            - Pointer to sysHost
  @param[in    ] Socket          - Socket number
  @param[in    ] LookupType      - SIM_REGS_TYPE value specifying the type of table requested
  @param[   out] Limit           - Number of table entries
  @param[   out] TablePtr        - Pointer to pointer to the SimRegs table
  @param[   out] NumRegInstances - Will be updated with the number of instances of each register in the table

  @retval  SUCCESS if the table is found
          !SUCCESS if table not found or some other failure occurs
**/
UINT32
GetSimRegsPtr (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    LookupType,
     OUT UINT32   *Limit,
     OUT UINT32   **TablePtr,
     OUT UINT32   *NumRegInstances
  );

UINT32
AcquireReleaseAPSystemSemaphore (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     LocAPSemaphoreNum,
  UINT8     OpCode
  );

#define MAX_PERFORMANCE_REG_CHAR   47
#define MAX_PERFORMANCE_FIELD_CHAR 33
#define MAX_PERFORMANCE_IP_CHAR    11

typedef struct {
  UINT32 Address;
  CHAR8* RegisterName;
} REGISTER_LIST;

typedef struct {
  UINT8  Index;
  UINT8* FieldName;
  UINT8  Offset;
  UINT8  Bits;
} FIELD_LIST;

/**
  Prints the fields of the registers specified in RegIndex

  @param Host           - Pointer to sysHost structure
  @param Socket         - Socket value
  @param Value          - Value of the register
  @param RegList        - Array of registers
  @param RegIndex       - Register index to be displayed
  @param FieldList      - Array of fields for the IP specified
  @param FieldArraySize - Size of FieldList
  @param Ip             - String of the IP
  @param Instance       - Instance of the register

  @retval N/A
**/
VOID
PrintPerformanceRegisters (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT32        Value,
  IN REGISTER_LIST RegList[],
  IN UINT8         RegIndex,
  IN FIELD_LIST    FieldList[],
  IN UINT8         FieldArraySize,
  IN UINT8*        Ip,
  IN UINT8         Instance
  );

/**
  Print performance settings

  @param Host  - Pointer to sysHost

  @retval N/A
**/
UINT32
PrintPerformanceSettings (
  IN PSYSHOST Host
  );

//
// Routines to access scratch pad register
//
UINT32
readScratchPad_CMD (
  VOID
  );

VOID
writeScratchPad_CMD (
  UINT32          val
  );


UINT32
readScratchPad_DATA0 (
  VOID
  );

VOID
writeScratchPad_DATA0 (
  UINT32           val
  );

UINT32
readScratchPad_DATA1 (
  VOID
  );

VOID
writeScratchPad_DATA1 (
  UINT32          val
  );

VOID
writeScratchPad_currentCheckPoint (
  UINT32          val
  );

VOID
SsaStichedModeRMTChip (
  SSA_BIOS_SERVICES_PPI *SsaBiosServicesPpi
  );

VOID
SsaBiosLoadStatusChip (
  UINT32 agentReg
  );

BOOLEAN
SSAIsRegRev2DimmPresent (
  PSYSHOST Host
  );


/**

Receive enable train feedback result handler.

@param Host     - Pointer to sysHost, the system Host (root) structure
@param socket   - CPU Socket Node number (Socket ID)
@param ch       - Channel number (0-based)
@param ch       - sub Channel number (0-based)
@param strobe   - Strobe (0-based)
@param csrReg   - CSR Register
@param piDelay  - Pi Delay

@retval None

**/
VOID
RecEnTrainFeedbackDdr5 (
  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  UINT8       socket,                           // CPU Socket Node number (Socket ID)
  UINT8       ch,                               // Channel number (0-based)
  UINT8       subch,                            // sub Channel number (0-based)
  UINT8       strobe,                           // Strobe (0-based)
  UINT32      csrReg,                           // CSR Register
  UINT16      piDelay,                          // Pi Delay
  struct   TrainingResultsDdr5 (*trainResInput)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

VOID
DisplayTcwlAdjDdr5 (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 subch,
  MRC_GT group
  );

/**

Chip specific code path to collect and store the results from the current test for DDR5 External WL fine Sweep.

@param Host        - Pointer to sysHost, the system Host (root) structure
@param socket      - Socket Id
@param dimm        - Current dimm
@param rank        - Rank number (0-based)
@param offset      - Pi Delay
@param CwlAdj      - CWL_ADJ Input
@param TxDqsDelay  - TxDqsDelay Input

@retval None

**/
VOID
GetResultsWLFineSweep (
  PSYSHOST                   Host,
  UINT8                      socket,
  UINT8                      dimm,
  UINT8                      rank,
  INT16                      offset,
  INT16                      CwlAdj [MAX_CH][SUB_CH][MAX_RANK_CH],
  INT16                      TxDqsDelay [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  struct TrainingResultsDdr5 trainRes [MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**
Pattern used on Write Fly by clean up for this project for DDR5.

@retval PatWrRdFlyb
PatWrRd
**/
UINT8
GetChipPatternExternalWriteLevelingDdr5 (
  VOID
  );

/*++
Get the maximum number of data groups based on DIMM and group type

@param Host  - Pointer to sysHost
@param socket  - Socket
@param ch    - Channel
@param dimm  - DIMM
@param group - Group type
@param level - Level type

@retval Max strobe

--*/
UINT8
GetMaxStrobeScDdr5 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  MRC_GT    group,
  MRC_LT    level
  );


/**

Chip specific setup to initialize loop indices when performing a pidelay step for DDR5

@param Host          - Pointer to sysHost, the system Host (root) structure
@param socket        - CPU Socket Node number (Socket ID)
@param piDelayStart  - Pointer to the loop start index
@param piDelayEnd    - Pointer to the loop end index

@retval None

**/
VOID
GetTxPiSamplepiDelayStartandEndDdr5 (
  PSYSHOST  Host,                               // Pointer to sysHost, the system Host (root) structure
  UINT8     socket,                             // CPU Socket Node number (Socket ID)
  INT16     *piDelayStart,                      // Pointer to the loop start index
  UINT16    *piDelayEnd                         // Pointer to the loop end index
  );

/*++

Routine Description:

Get's the results from the current test for DDR5 WL.

Arguments:

Host      - Pointer to sysHost
dimm      - DIMM number
rank      - rank number of the DIMM

Returns:

N/A

--*/
VOID
GetResultsWLDdr5 (
  PSYSHOST                   Host,
  UINT8                      socket,
  UINT8                      dimm,
  UINT8                      rank,
  INT16                      piDelay [MAX_CH][SUB_CH],
  INT16                      TempCwlAdj,
  UINT32                     StrobePass [MAX_CH][SUB_CH],
  INT16                      CwlAdj [MAX_CH][SUB_CH][MAX_RANK_CH],
  INT16                      CwlAdjRecord [2][MAX_CH][SUB_CH],
  UINT8                      LockCwlAdj [MAX_CH][SUB_CH],
  INT16                      TxDqsDelay [MAX_CH][SUB_CH][MAX_RANK_CH][MAX_STROBE_DDR5 / 2],
  struct TrainingResultsDdr5 trainRes [MAX_CH][SUB_CH][MAX_STROBE_DDR5/2]
  );

/**
Chip specific code path to get the starting value requested


@param step     - Current PI step
@param piStart  - PI start value

@retval (UINT16)Starting value

**/
UINT16
StartBackOverDdr5 (
  UINT8  step,
  UINT16 piStart
  );

/**

  This sets the CTL loopback mode in the buffer

  @param[in] Host       - Pointer to sysHost
  @param[in] socket     - Socket number
  @param[in] ch         - Channel number
  @param[in] dimm       - Dimm number
  @param[in] ControlWordData  - Data to be written

**/
VOID
ChipSetCsLoopbackDdr5(
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    ControlWordData
  );

/**

Check if RCD loopback mode should be used

@param Host - Pointer to sysHost, the system Host (root) structure struct

@retval TRUE - CTL loopback mode should be used
@retval TRUE - CTL loopback mode should not be used

**/
BOOLEAN
CheckRCDLoopback (
  PSYSHOST Host
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
GetTrainingResultIndexDDR5 (
  UINT8  step,
  UINT16 piSetting,
  UINT16 piStart
  );

/**

Chip specific code path to updateChipMCDelayDdr5

@param Host                - Pointer to sysHost, the system Host (root) structure
@param socket              - processor socket ID
@param ch                  - Processor socket Channel number
@param dimm                - dimm number
@param SubChannel          - SubChannel number
@param channelNvList       - Pointer to the Channel nvram
@param CRAddDelay          - Add delays
@param WLChipCleanUpStruct - Pointer to CHIP_WL_CLEANUP_STRUCT

@retval None

**/
VOID
UpdateChipMCDelayDdr5 (
  PSYSHOST                Host,
  UINT8                   socket,
  UINT8                   channel,
  UINT8                   Dimm,
  UINT8                   SubChannel,
  struct channelNvram (*channelNvList)[MAX_CH],
  INT16                   CRAddDelay,
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  );

/**

Dq Train Feedback Calculation

@param Host             - Pointer to sysHost, the system Host (root) structure
@param socket           - CPU Socket Node number (Socket ID)
@param ch               - Channel number
@param csrReg           - DQ train feedback register value
@param LogicResult      - Logic result for DQ train feedback register value

@retval None

**/
VOID
DqTrainFeedbackCalculation (
  IN      PSYSHOST Host,
  IN      UINT8    socket,
  IN      UINT8    ch,
  IN      UINT32   csrReg[MAX_STROBE],
  IN OUT  UINT32   LogicResult [MAX_STROBE]
  );

/**

Turn on command add operation enable

@param Host                - Pointer to sysHost, the system Host (root) structure
@param socket              - Socket number
@param ch                  - DDR channel (0-based)

@retval None

**/
VOID
TrnOnChipCMDADDOpEnDdr510nm (
  PSYSHOST  Host,                               // Pointer to sysHost, the system Host (root) structure
  UINT8     socket,                             // Processor socket ID
  UINT8     ch                                  // DDR channel (0-based)
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
CsClkSetupPiSettingsParams (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    step,
  UINT16   *piSetting,
  UINT16   *piStart,
  UINT16   *NumberOfCycle,
  MRC_TT   CurrentTestType,
  struct ClkTrainingResults (**trainRes)[MAX_CH][SUB_CH][MAX_STROBE_DDR5/2]
  );


/**

Setup PI settings parameters

@param Host             - Pointer to sysHost, the system Host (root) structure
@param socket           - CPU Socket Node number (Socket ID)
@param Ch               - Channel number
@param step             - The PI step to work on
@param piSetting        - The PI setting returned
@param piStart          - Test starting PI
@param NumberOfCycle    - Number of cycles
@param trainRes         - Training results

@retval None

**/
VOID
CaClkSetupPiSettingsParams (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    Ch,
  UINT8    step,
  UINT16   *piSetting,
  UINT16   *piStart,
  UINT16   *NumberOfCycle,
  MRC_TT   CurrentTestType,
  struct   ClkTrainingResults (**trainRes)[MAX_CH][SUB_CH][MAX_STROBE_DDR5 / 2]
  );

/**
  Prime CA Margins Sweep Error Result Feedback.

  @param[in]  Socket            - CPU Socket Node number (Socket ID)
  @param[in]  Channel           - Channel number
  @param[in]  Dimm              - DIMM number
  @param[in]  SubChannel        - Sub-channel number
  @param[in]  Signal            - Enumerated signal value
  @param[in]  CaVrefValue       - CA Vref value
**/
VOID
EFIAPI
PrimeCaMarginsSweepErrorResFeedback (
  IN  UINT8   Socket,
  IN  UINT8   Channel,
  IN  UINT8   Dimm,
  IN  UINT8   SubChannel,
  IN  GSM_CSN Signal,
  IN  INT8    CaVrefValue
  );

/**
  Get CA Margins Sweep Error Result Feedback when using DCA complex pattern.

  @param[in]  Socket            - CPU Socket Node number (Socket ID)
  @param[in]  Channel           - Channel number
  @param[in]  Dimm              - DIMM number
  @param[in]  SubChannel        - Sub-channel number
  @param[in]  Rank              - Rank number
  @param[in]  Signal            - Signal
  @param[in]  PiIndex           - PI index
  @param[in]  CurError          - Current error
  @param[in]  RetrainLoopCount  - Retrain loop count

  @retval error result of margin sweep
**/
UINT8
EFIAPI
GetCaMarginsSweepXtalkErrorResFeedback (
  IN  UINT8   Socket,
  IN  UINT8   Channel,
  IN  UINT8   Dimm,
  IN  UINT8   SubChannel,
  IN  UINT8   Rank,
  IN  GSM_CSN Signal,
  IN  INT16   PiIndex,
  IN  UINT8   CurError,
  IN  UINT8   RetrainLoopCount
  );

/**
  Get CA Margins Sweep Error Result Feedback.

  @param[in]  Socket            - CPU Socket Node number (Socket ID)
  @param[in]  Channel           - Channel number
  @param[in]  Dimm              - DIMM number
  @param[in]  SubChannel        - Sub-channel number
  @param[in]  Rank              - Rank number
  @param[in]  Signal            - Signal
  @param[in]  PiIndex           - PI index
  @param[in]  CurError          - Current error
  @param[in]  RetrainLoopCount  - Retrain loop count
  @param[in]  PatternLoop        - Pattern Loop

  @retval error result of margin sweep
**/
UINT8
EFIAPI
GetCaMarginsSweepErrorResFeedback (
  IN  UINT8   Socket,
  IN  UINT8   Channel,
  IN  UINT8   Dimm,
  IN  UINT8   SubChannel,
  IN  UINT8   Rank,
  IN  GSM_CSN Signal,
  IN  INT16   PiIndex,
  IN  UINT8   CurError,
  IN  UINT8   RetrainLoopCount,
  IN  UINT8   PatternLoop
  );

/**
  Get DCA DFE Margins Sweep Error Result Feedback.

  @param[in]  Socket            - CPU Socket Node number (Socket ID)
  @param[in]  Channel           - Channel number
  @param[in]  Dimm              - DIMM number
  @param[in]  SubChannel        - Sub-channel number
  @param[in]  Rank              - Rank number
  @param[in]  Signal            - Signal
  @param[in]  PiIndex           - PI index
  @param[in]  CurError          - Current error
  @param[in]  RetrainLoopCount  - Retrain loop count
  @param[in]  PatternLoop       - Pattern loop
  @param[in]  Loopnumber        - Loop number
  @param[in]  TapIndex          - Tap index

  @retval error result of margin sweep
**/
UINT8
EFIAPI
GetCaMarginsSweepErrorResFeedbackDcaDfe (
  IN  UINT8   Socket,
  IN  UINT8   Channel,
  IN  UINT8   Dimm,
  IN  UINT8   SubChannel,
  IN  UINT8   Rank,
  IN  GSM_CSN Signal,
  IN  INT16   PiIndex,
  IN  UINT8   CurError,
  IN  UINT8   RetrainLoopCount,
  IN  UINT8   PatternLoop,
  IN  UINT8   Loopnumber,
  IN UINT8    TapIndex
  );

VOID
CheckSecrets10nm (
  PSYSHOST Host
  );

VOID
EFIAPI
SetLegacyVariables (
  IN        UINT8           MaxMemSs,
  IN        UINT8           MaxChannel,
  IN        UINT8           MaxChannelPerSs
  );

BOOLEAN
EFIAPI
DoesCpuSupportNearMemory (
  VOID
  );

/**

  Install the Mrc Hooks Chip Layer Services PPI interface

  @param Host     - Pointer to sysHost, the system Host (root) structure

  @retval EFI_STATUS

**/
EFI_STATUS
InstallMrcHooksChipServicesPpi (
  PSYSHOST Host
  );

/**
  Initialize CPU ID related fields in sysHost.
  If the processor detected is supported by this driver, initialize
  the function pointer table and return TRUE.

  @param  Host    - Pointer to sysHost structure

  @retval   TRUE  - the detected processor is supported
            FALSE - the detected processor is not supported
**/

BOOLEAN
ProcessorSupported (
  PSYSHOST    Host
  );

/**
Common DDR training entry

@param[in] Host     - Pointer to sysHost structure

@retval SUCCESS:  Memory initialization completed successfully.
@retval FAILURE:  A reset is required
**/
UINT32
DdrTraining (
  IN PSYSHOST Host
  );

/**
Unlock access to the Memory Controller and related sub-systems
if required by the processor.

@param[in]      Host          - Pointer to sysHost

@retVal SUCCESS
**/
UINT32
UnlockMemoryAfterRamp (
  IN PSYSHOST  Host
  );

/**

This routine gets things ready for DDR training

@param Host  - Pointer to sysHost

@retval SUCCESS

**/
UINT32
PreTrainingInit (
  IN PSYSHOST Host
  );

/**

  Performs post-training hardware initialization

  @param[in] Host   Pointer to sysHost

  @retval SUCCESS   All operations completed successfully

**/
UINT32
PostTrainingInit (
  IN  PSYSHOST  Host
  );

/**
  Disables channels in HW that are supported in IP (MC, DDRIO, etc) but are unsupported
  by the platform

  @param[in]  Socket        Socket number to disable unused channels for

  @retval     EFI_SUCCESS   Unsupported channels successfully disabled
**/
EFI_STATUS
EFIAPI
DisableUnsupportedChannelsHw (
  IN  UINT8   Socket
  );

/**

Execute SSA RMT BultIn\Loader method

@param Host  - Pointer to sysHost

@retval SUCCESS

**/
UINT32
ExecuteSsaRmt (
  IN PSYSHOST Host
  );

/*++

  Wrapper to call Memory margin test

  @param Host  - Pointer to sysHost

  @retval SUCCESS

--*/
UINT32
MemBootHealthTestWrapper (
  PSYSHOST Host
  );

/**
This function implements a MRC reset loop using scratchpad registers

@param Host:         Pointer to sysHost

@retval SUCCESS:  Memory initialization completed successfully.
**/
UINT32
DDRResetLoop (
  IN PSYSHOST Host
  );

/**

Wrapper code to gather device specific SPDData

@param Host - Pointer to sysHost, the system Host (root) structure struct

@retval SUCCESS

**/
UINT32
GatherSPDData (
  IN PSYSHOST Host
  );

/*++

Routine Description:

  Initialize the Cpu info by calling into CpuTypeLib.

  @param[out] Host          A pointer to host.

  @retval EFI_SUCCESS

--*/

EFI_STATUS
EFIAPI
InitCpuFamilyTypeStepping (
  OUT PSYSHOST Host
  );

/**

  Hook to override debug levels

  @param[in] Host   - Pointer to syshost structure
  @param[in] Socket - Socket number

**/

VOID
OverrideDebugLevels (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**

  Perform post memory init functions, install memory, etc.

  @param[in] Host   - Pointer to syshost structure

  @retval EFI_SUCCESS

**/

EFI_STATUS
EFIAPI
PostMemoryInit (
  IN PSYSHOST Host
  );

/**

  Puts the Host structure variables for POST

  @param[in] Host - pointer to sysHost structure on stack

  @retval VOID

**/

VOID
EFIAPI
PublishDataForPost (
  IN PSYSHOST  Host
  );

#endif  //__PROC_MEM_INIT_CHIP_H__
