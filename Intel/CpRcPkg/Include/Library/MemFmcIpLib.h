/** @file
  MemFmcIpLib.h

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

#ifndef  _MEM_FMC_IP_LIB_H_
#define  _MEM_FMC_IP_LIB_H_

#include "SysHost.h"
#include <Uefi.h>
#include <ReferenceCodeDataTypes.h>
#include <Library/BaseLib.h>

#define MAX_MEDIA_LOGS      3
#define MAX_THERMAL_LOGS    7
#define FNV_MEDIA_LOG_REQ   0
#define FNV_THERMAL_LOG_REQ 1

// FMC cache info
#define  ENTRY_VALID      BIT0
#define  ENTRY_USED       BIT1

// Performance Knob ids and values as per FIS 1.14 document
#define PERF_KNOB1_ID                 0x0
#define PERF_KNOB2_ID                 0x1
#define PERF_KNOB5_ID                 0x4
#define PERF_KNOB6_ID                 0x5

#define PERF_KNOB1_VALUE_2LMPM        0x5
#define PERF_KNOB2_VALUE_2LMPM        0x4
#define PERF_KNOB1_VALUE_PMONLY       0xF
#define PERF_KNOB2_VALUE_PMONLY       0x6
#define PERF_KNOB5_VALUE_DEFAULT      0xA
#define PERF_KNOB6_VALUE_DEFAULT      0x7FF

// Performance Knob id and value as per FIS 2.0 document
#define PERF_KNOB7_ID                 0x6
#define PERF_KNOB7_VALUE              0x0
#define PERF_KNOB7_VALUE_DEFAULT      0x1

//
// Default time for Set System time FIS command
// (Monday, January 1, 1990 12:00:00 AM)
//
#define SET_SYS_TIME_DEFAULT_YEAR    1990
#define SET_SYS_TIME_DEFAULT_MONTH   1
#define SET_SYS_TIME_DEFAULT_DAY     1
#define SET_SYS_TIME_DEFAULT_HOUR    0
#define SET_SYS_TIME_DEFAULT_MINUTE  0
#define SET_SYS_TIME_DEFAULT_SECOND  0

typedef enum FMC_STEPPING {
  FmcStepA0 = 0x00,
  FmcStepA1 = 0x01,
  FmcStepA2 = 0x02,
  FmcStepA3 = 0x03,
  FmcStepB0 = 0x10,
  FmcStepB1 = 0x11,
  FmcStepB2 = 0x12,
  FmcStepB3 = 0x13,
  FmcStepC0 = 0x20,
  FmcStepC1 = 0x21,
  FmcStepC2 = 0x22,
  FmcStepC3 = 0x23,
  FmcStepD0 = 0x30,
  FmcStepD1 = 0x31,
  FmcStepD2 = 0x32,
  FmcStepD3 = 0x33,
  FmcStepMax
} FmcStepping;

//
// Max number of DDRT2 clock movement
//
#define MAX_CLOCK_MOVEMENT_NUM  6

//
// FMC IP interface
//

/**

  GetSetTxVrefFnv

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

  @retval status

**/
MRC_STATUS
GetSetTxVrefFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  );

MRC_STATUS
CacheTxVrefFnv (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank
  );

MRC_STATUS
InitDdrioInterfaceLateFmc (
  PSYSHOST  Host,
  UINT8     Socket
  );

/**
  Retrieves the manufacturer's revision of the FMC

  @param[in] Host             Pointer to sysHost
  @param[in] Socket           Socket Id
  @param[in] Channel          Socket channel number
  @param[in] Dimm             DIMM slot number

  @retval FMC_STEPPING        Stepping of the FMC
**/
FmcStepping
EFIAPI
GetFmcStepping (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel,
  IN UINT8    Dimm
  );
  
MRC_STATUS
SetTxPiOnFmc (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT32    TxPiOn
  );

/**
  Resets the DDRT DDRIO

  @param[in]  Host              Pointer to sysHost
  @param[in]  Socket            Socket Id
  @param[in]  ChannelInSocket   Channel number (0-based)
  @param[in]  Dimm              Zero based DIMM number

  @retval N/A
**/
VOID
EFIAPI
BiosAepIoResetChip (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     ChannelInSocket,
  IN  UINT8     Dimm
  );

/**

  Checks if the Nvm is POR or not

  @param[in] NvmStepping  - Nvm Stepping
  @param[in] FmcModRev    - Nvm Module Revision and Type
  @param[in] NvmCapacity  - Nvm Capacity

  @retval FALSE The Nvm is not Supported
  @retval TRUE  The Nvm is Supported

**/
BOOLEAN
DdrtPorCheck (
  IN UINT8                        NvmStepping,
  IN SPD_AEP_MOD_REVISION_STRUCT  FmcModRev,
  IN UINT16                       NvmCapacity
  );

VOID
DisableScrambling (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  );

VOID
InvalidateDdrtPrefetchCache (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  );

/**
  Programs CCC Settings for CWV

  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH3CCC_STRUCT
  picode1 BCLK_A
  picode2 BCOM_A[0], BCS_A

  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT
  picode1 BRST_A, BCOM_A[2:1]

  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_STRUCT
  picode1 BCLK_B
  picode2 BCOM_B[0], BCS_B

  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH1CCC_STRUCT
  picode1 BRST_B, BCOM_B[2:1]

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket Id
  @param[in] Ch        - Ch number
  @param[in] SubchMask - SubchMask number
  @param[in] Dimm      - Dimm number (zero-based)
  @param[in] Data      - Data to write

  @retval N/A
**/
VOID
WriteCwvBufferDelay(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     SubchMask,
  IN UINT8     Dimm,
  IN UINT16    Data
  );

/**
  Programs Bclk Latency for CPS

  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT
  picode1 BCLK_A

  DDRCCC_CR_DDRCRCCCPICODING0_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_STRUCT
  picode1 BCLK_B

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket Id
  @param[in] Ch         - Ch number
  @param[in] Subch      - Subch
  @param[in] Dimm       - Dimm number (zero-based)
  @param[in] InputDelay - Desired Delay

  @retval N/A
**/
VOID
ProgramDdrt2BclkDelay(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Subch,
  IN UINT8     Dimm,
  IN INT16     InputDelay
  );

VOID
ProgramDdrtGnt2Erid (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  );

/**
  Return DDRT CAS latencies (read, write, and adders) for a target DIMM.

  @param[in]  Socket                The socket containing the target DIMM.
  @param[in]  Channel               The channel containing the target DIMM.
  @param[in]  Dimm                  The target DIMM to get CAS latencies for.
  @param[out] CASLatency            An optional parameter to return the CAS (read) latency for the target DIMM.
  @param[out] CASLatencyAdder       An optional parameter to return the CAS (read) latency adder for the target DIMM.
  @param[out] CASWriteLatency       An optional parameter to return the CAS write latency for the target DIMM.
  @param[out] CASWriteLatencyAdder  An optional parameter to return the CAS write latency adder for the target DIMM.

  @retval   EFI_SUCCESS       CAS latencies were successfully determined for the target DIMM.
  @retval   EFI_UNSUPPORTED   CAS latencies could not be determined for the target DIMM's FMC type.
**/
EFI_STATUS
EFIAPI
GetDdrtCasLatencies (
  IN      UINT8    Socket,
  IN      UINT8    Channel,
  IN      UINT8    Dimm,
      OUT UINT8    *CASLatency,           OPTIONAL
      OUT UINT8    *CASLatencyAdder,      OPTIONAL
      OUT UINT8    *CASWriteLatency,      OPTIONAL
      OUT UINT8    *CASWriteLatencyAdder  OPTIONAL
  );

VOID
ProgramtCLtCWLAdd (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  );

VOID
SwitchToNormalModeFmc1 (
  PSYSHOST  Host,
  UINT8     Socket
  );

VOID
ReadBcwDimmFmc (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     bcwFunc,
  UINT8     bcwByte,
  UINT8     bcwNibble,
  UINT8     MprData[MAX_STROBE]
  );

/**

  Disable FNV

  @param Host                 - Pointer to sysHost, the system Host (root) structure
  @param socket               - Socket Id
  @param ch                   - Channel number (0-based)
  @param chipErlyCmdCKStruct  - Pointer to Early CMD CK structure

  @retval None

**/

VOID
DisableChipFNV (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct
  );


/**

Clears DqSwizzle Register in the AEP DIMM

@param Host - Pointer to sysHost, the system Host(root) structure
@param socket - CPU Socket Node number(Socket ID)

@retval None

**/
VOID
ClearDqSwizzleAep (
  IN  PSYSHOST    Host,                             // Pointer to sysHost, the system Host (root) structure
  IN  UINT8       Socket                            // CPU Socket Node number (Socket ID)
  );

/**

  Write control word data

  @param Host             - Pointer to sysHost, the system Host (root) structure
  @param socket           - CPU Socket Node number (Socket ID)
  @param ch               - Channel number (0-based)
  @param dimm             - Current dimm
  @param controlWordData  - Data to be written
  @param controlWordAddr  - Control word address (RDIMM_RC0A or RDIMM_RC0E)

  @retval None
**/

VOID
WriteRCFnv (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    controlWordData,
  UINT8    controlWordAddr
  );

/**

  Enables/Disables Training Mode for FNV


  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket number
  @param mode    - Training step

  @retval SUCCESS

**/

UINT32
SetAepTrainingMode (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    mode
  );

/**

  Routine Description: Clears Prefetch Cache miss for NVMCTLR

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

  @retval SUCCESS
**/
UINT32
ClrPcheMiss (
  PSYSHOST Host,
  UINT8    socket
  );

/**

  Routine Description: Enables/Disables Training Mode for NVMCTLR, SMBUS version

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param mode    - training mode

  @retval SUCCESS
**/
UINT32
SetAepTrainingModeSMB (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    mode
  );

/**
Puts the BGF into reset. Prevents the Core from decoding a misaligned command.

@param[in]  Socket                The socket containing the target DIMM.
@param[in]  Channel               The channel containing the target DIMM.
@param[in]  Dimm                  The target DIMM
@param[in]  Value                 Value to put BFG in reset or not

@retval   None
**/

VOID
BgfReset(
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm,
  IN  UINT8     Value
);

/**

  Routine Description: Enables/Disables Training Mode for NVMCTLR, SMBUS version

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number
  @param[in] GetFlag - True: Get, False: Restore
  @param[in/out] DaTrainingEnOrg - DCPMM training mode register setting
  @param[in/out] DTrainingEnOrg  - DCPMM training mode register setting

  @retval SUCCESS
**/
UINT32
GetRestoreAepTrainingModeSMB (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  BOOLEAN     GetFlag,
  IN OUT  UINT32  *DaTrainingEnOrg,
  IN OUT  UINT32  *DTrainingEnOrg,
  IN OUT  UINT32  *HsioTrainingEnOrg
);

/**

  This resets the NVMCTLR DDRTIO FIFO Pointers

  @param Host    - Pointer to sysHost
  @param socket    - Socket number

  @retval N/A

**/
VOID
IODdrtReset (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    dimm
  );

VOID
IODdrtResetAll (
  PSYSHOST Host,
  UINT8 socket
  );

/**
  Resets the FNV DDRTIO FIFO Pointers

  @param[in]  Host    Pointer to sysHost
  @param[in]  Socket  Socket number
  @param[in]  Channel Channel number
  @param[in]  Dimm    Dimm number

  @retval N/A
**/
VOID
EFIAPI
IODdrtResetPerCh (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Channel,
  IN  UINT8    Dimm
  );

/**

  This sets the CTL loopback mode in the buffer

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number
  @param[in] Ch        - Channel number
  @param[in] Dimm      - Dimm number
  @param[in] TrainFlag - TRUE:  Set Rc0c
                         False: Clear Rc0c

  @retval N/A

**/
VOID
ChipSetCTLLoopbackFmc (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    Dimm,
  IN  BOOLEAN  TrainFlag
);

/**

  Put CS0_n into loopback mode in the buffer

  @param Host     - Pointer to sysHost
  @param socket   - Socket number

  @retval N/A

**/
VOID
ChipCS0ToLoopback (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
);

/**

  This functions enables TxOn in datacontrol0

  @param Host     - Pointer to sysHost
  @param socket   - Socket number

  @retval - None

**/
VOID
ToggleTxOnFnv (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    value
  );

/**

  This functions enables InternalClocks On in datacontrol0

  @param Host     - Pointer to sysHost
  @param socket   - Socket number

  @retval - None

**/
VOID
ToggleInternalClocksOnFnv (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    value
  );

VOID
RPQDrain (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank
  );

VOID
FifoTrainReset (
  PSYSHOST Host,
  UINT8    socket
  );

/**

  Enable/Disable BCOM Mode for DDRT

  @param[in] Host     - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket   - Socket Id
  @param[in] Value    - Enable/Disable

  @retval None

**/
VOID
ToggleBcomFlopEn (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Value
  );

/**

  Enable/Disable BCOM Mode for DDRT on specified Channel

  @param[in] Host     - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number (0-based)
  @param[in] Value    - Enable/Disable

  @retval None

**/
VOID
ToggleBcomFlopEnPerCh (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Value
  );

/**

  Enable/Disable DDRT Dq BCOM Ctrl

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - current socket
  @param value   - Enable/Disable

  @retval None

**/

VOID
ToggleBcom (PSYSHOST Host,
  UINT8    socket,
  UINT8    value
  );

/**

  Program DA Pattern

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket Id
  @param ch       - Channel number (0-based)
  @param dimm     - Current dimm
  @param pattern  - Pattern to program

  @retval None

 **/

VOID
ProgramDAFnvMprPattern (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    pattern
  );

VOID
ProgramDFnvMprPattern (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    pattern
  );

/**

  Set Rank DA

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket Id
  @param ch       - Channel number (0-based)
  @param dimm     - Current dimm
  @param mprMode  - Either 0 for Disable or MR3_MPR to Enable

  @retval None

 **/

VOID
SetRankDAMprFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    mprMode
  );

/**

  Set Rank D

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket Id
  @param ch       - Channel number (0-based)
  @param dimm     - Current dimm
  @param mprMode  - Either 0 for Disable or MR3_MPR to Enable

  @retval None

 **/

VOID
SetRankDMprFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    mprMode
  );

VOID
DdrtBufferReset (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm
  );

VOID
SetFmcTempCrit (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT32   TempCrit
  );

UINT32
GetFmcCoreFreq (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm
  );

VOID
ConfigNvmdimmtRidFeedback (
  IN PSYSHOST HOST,
  IN BOOLEAN  RidFeedbackDis
  );
//
// Frequency values returned by GetFmcCoreFreq
//
#define FMC_CORE_FREQ_533MHZ    0x08
#define FMC_CORE_FREQ_667MHZ    0x0A
#define FMC_CORE_FREQ_800MHZ    0x0C

//
// MemFmcMailbox.c
//

#define COMMIT_ID                40
#define NUM_PAYLOAD_REG          32
#define MAX_HOST_TRANSACTIONS    64

//
// FIS 3.0-Pre2 Current Boot Status Register Definition (CBSR)
// FIS 3.0-Pre2 Previous Boot Status Register Definition (PBSR)
// Previous Boot Status Register(PBSR) is a saved copy of the CBSR from the previous boot.
//
typedef union {
  struct {
    UINT32 MajorCheckpoint:8;             ///< [07:00] Major Checkpoint
    UINT32 MinorCheckpoint:8;             ///< [15:08] Minor Checkpoint
    UINT32 MediaReady:1;                  ///< [16] MR (0:Not Ready, 1:Ready)
    UINT32 PcrAccessLocked:1;             ///< [17] PCR (0:Unlocked, 1:Locked)
    UINT32 MailboxInterfaceReady:1;       ///< [18] MBR (0:Not Ready, 1:Ready)
    UINT32 WatchdogTimerStatus:1;         ///< [19] WTS (0:No Change, 1:WDT NMI Generated)
    UINT32 MediaDisabled:1;               ///< [20] MD (0:User Data is accessible, 1:User Data is not accessible)
    UINT32 HostInterfaceStatus:1;         ///< [21] HIS (0:Host Interface is inactive, 1:Host Interface is active)
    UINT32 RebootRequired:1;              ///< [22] RR (0:No reset is needed, 1:The DIMM'S internal state requires a platform power cycle)
    UINT32 LinkFailureOnPreviousBoot:1;   ///< [23] LFOPB (0:No Error, 1:Fatal Link Error
    UINT32 Reserved:8;                    ///< [31:24] Reserved
  } Bits;
  UINT32 Data;
} CURRENT_BOOT_STATUS_REGISTER, PREVIOUS_BOOT_STATUS_REGISTER;

typedef union {
  struct {
    UINT32 Reserved:32;                    ///< [63:32] Reserved
  } Bits;
  UINT32 Data;
} CURRENT_BOOT_STATUS_HIGH_REGISTER, PREVIOUS_BOOT_STATUS_HIGH_REGISTER;

// FIS 3.0-Pre2 Legacy Boot Status Register Definition (LBSR)
typedef union {
  struct {
    UINT32 MajorCheckpoint:8;             ///< [07:00] Major Checkpoint
    UINT32 MinorCheckpoint:8;             ///< [15:08] Minor Checkpoint
    UINT32 MediaReady:2;                  ///< [17:16] MR (0:Not Ready, 1:Ready, 2:Error, 3:Rsvd)
    UINT32 DdrtIoInitComplete:1;          ///< [18] DT (0:Not Ready, 1:Ready)
    UINT32 PcrAccessLocked:1;             ///< [19] PCR (0:Unlocked, 1:Locked)
    UINT32 MailboxInterfaceReady:1;       ///< [20] MBR (0:Not Ready, 1:Ready)
    UINT32 WatchdogTimerStatus:1;         ///< [21] WTS (0:No Change, 1:WDT NMI Generated)
    UINT32 FirstRefreshCompletedFlag:1;   ///< [22] FRCF (0:No Change, 1:First Refresh Cycle Completed)
    UINT32 CreditReady:1;                 ///< [23] CR (0:WDB Not Flushed, 1:WDB Flushed)
    UINT32 MediaDisabled:1;               ///< [24] MD (0:Media Normal, 1:Media Disabled)
    UINT32 SvnDowngradeOptInEnable:1;     ///< [25] SVNDE (0:Not Enabled, 1:Enabled)
    UINT32 SvnDowngradeOptInWasEnabled:1; ///< [26] SVNCOIS (0:Never Enabled, 1:Has Been Enabled)
    UINT32 DramReadyAit:2;                ///< [28:27] DR (0:Not Trained/Not Loaded, 1:Trained/Not Loaded, 2:Error, 3:Trained/Loaded)
    UINT32 RebootRequired:1;              ///< [29] RR (0:No reset is needed, internal state requires a platform power cycle)
    UINT32 LinkFailureOnPreviousBoot:1;   ///< [30] LFOPB (0:No Error, 1:Fatal Link Error
    UINT32 SvnOptInWindowClosed:1;        ///< [31] SVNWC (0:The SVN Opt-In Window is open, 1:The SVN Opt-In Window is closed)
  } Bits;
  UINT32 Data;
} BOOT_STATUS_REGISTER;

// FIS 1.5 Boot Status High Register Definition
typedef union {
  struct {
    UINT32 Assertion:1;                   ///< [32] Assertion (1:FW Hit Assert - debug only)
    UINT32 MediaInterfaceStalled:1;       ///< [33] MI_Stalled (1:Media Interface Stalled - debug only)
    UINT32 DdrtIoInitTrainingStatus:2;    ///< [35:34] Ddrt IO Init Training Status (0: Not Complete, 1: Complete, 2:Failure, 3:S3 Complete)
    UINT32 Reserved:28;                   ///< [63:36] Reserved
  } Bits;
  UINT32 Data;
} BOOT_STATUS_HIGH_REGISTER;

// FIS 3.0-Pre2 Feature Status Register Definition
typedef union {
  struct {
    UINT64 BootZeroMem: 1;                    ///< [0] BZM (0: Disabled, 1: Enabled)
    UINT64 SvnDowngradeCapOptInStatus: 1;     ///< [1] SVNCOIS (0: Never Enabled, 1: SVN Downgrade Capability opted in)
    UINT64 SvnOptInWindowClosed: 1;           ///< [2] SVNWC (0: Open, 1: Closed)
    UINT64 SvnDowngradeEnable: 1;             ///< [3] SVNDE (0: Disabled, 1: Enabled)
    UINT64 SecureErasePolicy: 1;              ///< [4] SEP (0: Never set, 1: Secure Erase Policy opted in)
    UINT64 MasterPassphraseEnabled: 1;        ///< [5] MPE (0: Disabled, 1: Enabled)
    UINT64 S3PmLockedOptIn: 1;                ///< [6] SPLO (0: Disabled, 1: Enabled)
    UINT64 Reserved: 57;                      ///< [63:7] Reserved
  } Bits;
  UINT64_STRUCT Data;
} FEATURE_STATUS_REGISTER;

// time is in bcd format
typedef struct fnvSystemTime{
  UINT8  second;
  UINT8  minute;
  UINT8  hour;
  UINT8  day;
  UINT8  month;
  UINT16 year;
}*PFNVSYSTEMTIME;

#pragma pack(1)
typedef struct {
  UINT8   LogLevel    : 1;    // 0 for LOW and 1 for HIGH level log
  UINT8   LogType     : 1;    // 0 for Media log, 1 for thermal
  UINT8   LogInfo     : 1;    // 0 for retrieving data and 1 for retrieving info
  UINT8   LogPayload  : 1;    // 0 for small payload, 1 for large payload
  UINT8   Rsvd        : 4;
  UINT16  SequenceNumber;     // sequence number from which to get logs, 0 for oldest available
  UINT16  RequestCount;       // how many log entries to retrieve
  UINT8   Reserved[123];
} GET_ERROR_LOG_PARAMS;

typedef struct {
  UINT64_STRUCT SysTimeStamp;        // Unix Epoch time of the log entry
  UINT64_STRUCT Dpa;                 // Specifies the DPA address of the error
  UINT64_STRUCT Pda;                 // Specifies the PDA address of the failure
  UINT8         Range;               // Specifies the length in address space of the error
  UINT8         ErrorType;           // Indicates the kind of error that was logged.
  UINT8         ErrorFlags;          // Indicates the error flags for this entry
  UINT8         TransactionType;     // Indicates what transaction caused the error
  UINT16        SequenceNumber;      // Incremented for each log entry added to the selected log
  UINT16        Rsvd;
} MEDIA_LOG_ENTRY;

#define DDRT_ERRTYPE_UNCORRECTABLE          0
#define DDRT_ERRTYPE_DPA_MISMATCH           1
#define DDRT_ERRTYPE_AIT_ERROR              2
#define DDRT_ERRTYPE_DATA_PATH_ERROR        3
#define DDRT_ERRTYPE_ILLEGAL_ACCESS         4
#define DDRT_ERRTYPE_SPARE_BLOCK_ALARM_TRIP 5
#define DDRT_ERRTYPE_SMART_HEALTH_CHANGED   6
#define DDRT_ERRTYPE_PERSISTENT_WRITE_ECC   7

typedef struct {
  UINT64_STRUCT SysTimestamp;         // Unix Epoch time of the log entry
  UINT32        Temperature   : 15;   // Host reported temperature (0.0625 C resolution)
  UINT32        Sign          : 1;    // Temperature sign, 0 - positive, 1 - negative
  UINT32        Reported      : 3;    // Temperature being reported
  UINT32        TempType      : 2;    // This denotes which device the temperature is for
  UINT32        Rsvd1         : 11;
  UINT16        SequenceNumber;       // Incremented for each log entry added to the selected log
  UINT16        Rsvd2;
} THERMAL_LOG_ENTRY;

#define DDRT_TEMPREPORT_USERTRIP 0
#define DDRT_TEMPREPORT_LOW      1
#define DDRT_TEMPREPORT_HIGH     2
#define DDRT_TEMPREPORT_CRITICAL 4

typedef struct {
  UINT16          ReturnCount;
  MEDIA_LOG_ENTRY MediaLog[MAX_MEDIA_LOGS];
} MEDIA_LOG_RETURN_DATA;

typedef struct {
  UINT16            ReturnCount;
  THERMAL_LOG_ENTRY ThermalLog[MAX_THERMAL_LOGS];
} THERMAL_LOG_RETURN_DATA;

typedef struct {
  UINT16        MaxLogEntries;          // Total number of log entries FW has abitily to log before overflow
  UINT16        CurrentSequenceNumber;  // Last assigned Sequence Number
  UINT16        OldestSequenceNumber;   // Oldest Sequence Number currently stored
  UINT64_STRUCT OldestEntryTimestamp;   // Unix Epoch time of the oldest log entry
  UINT64_STRUCT NewestEntryTimestamp;   // Unix Epoch time of the newest log entry
} LOG_INFO_RETURN_DATA;

typedef union {
  MEDIA_LOG_RETURN_DATA   MediaLogData;
  THERMAL_LOG_RETURN_DATA ThermalLogData;
  LOG_INFO_RETURN_DATA    LogInfoData;
  UINT32                  SmallPayload[NUM_PAYLOAD_REG];
} ERROR_LOG_RETURN_DATA;

typedef union {
  UINT32   DWord;
  struct {
    UINT32 LatchSystemShutdownStateEn: 1,
           Reserved                  :31;
  } Bits;
} SET_LATCH_SYSTEM_SHUTDOWN_STATE;

typedef union {
  UINT32   DWord;
  struct {
    UINT32 ExtendedAdrEn  : 1,
           Reserved       :31;
  } Bits;
} SET_EXTENDED_ADR_STATE;
#pragma pack()

typedef enum {
  LSSS_DISABLED,
  LSSS_ENABLED,
  LSSS_MAX
} LATCH_SYSTEM_SHUTDOWN_STATE;

typedef enum {
  EXTENDED_ADR_DISABLED,
  EXTENDED_ADR_ENABLED,
  EXTENDED_ADR_MAX
} EXTENDED_ADR_STATE;

typedef struct dimmPartition {
  UINT32           volatileCap;     // Volatile Capacity (2LM)
  UINT64_STRUCT    volatileStart;   // Volatile Start Address
  UINT32           persistentCap;   // Persistent Capcity (PM)
  UINT64_STRUCT    persistentStart; // Persistent Start Address
  UINT32           twoLMRegion;     // DPA start address of 2LM Region
  UINT32           PMRegion;        // DPA start address of PM Region
  UINT32           rc;              // Raw Capacity
  UINT32           pending2LM;      // Pending Volatile taking effect on next boot
  UINT32           pendingPM;       // Pending Persistent taking effect on next boot
  UINT64_STRUCT    nonceSecurity;   // BIOS Security Nonce
  UINT32           partitionSize;   // Size of Partition in Bytes
  UINT32           totalPlatform;   // Total Size of Platform Config Area
} DIMM_PARTITION_DATA;

typedef struct {
  UINT16  PeakPowerBudget;    ///< Power budget in mW used for instantaneous power (10000 - 20000 mW)
  UINT16  AveragePowerBudget; ///< Power budget in mW used for averaged power (10000 - 18000 mW)
} FIS1X_PWR_MGMT;

typedef struct {
  UINT16  AveragePowerLimit;        // Power limit in mW used for averaged power ( Valid range starts from 10000mW).
  UINT8   AveragePowerTimeConstant; // Value used as a base time window for power usage measurements(in mSec).
  UINT8   TurboModeState;           // Returns if the Turbo Mode is currently enabled or not.
  UINT16  TurboPowerLimit;          // Power limit in mW used for limiting the Turbo Mode power consumption (Valid range starts from 15000mW).
  UINT16  MaxTurboPower;            // If Turbo Mode is supported by the Module, then this field will return the maximum Turbo Power value (in mW).
  UINT16  MaxAveragePower;          // Returns the maximum average power limit supported by the Module (in mW).
} FIS2_0_PWR_MGMT;

typedef struct {
  UINT16  AveragePowerLimit;           // Power limit in mW used for averaged power ( Valid range starts from 10000mW).
  UINT32  MbbAveragePowerTimeConstant; // Determines time constant for maintaining average power limit specified in Average Power Limit field.
  UINT8   MbbFeature;                  // Allows enabling and disabling the feature.
  UINT16  MbbMaxPowerLimit;            // Sets the maximum amount, in units of mW, above average power limit ( Valid range starts from 15000mW).
  UINT16  MaxAveragePower;             // Returns the maximum average power limit supported by the Module (in mW).
  UINT16  MaxMbbPower;                 // If Memory Bandwidth Boost feature is supported by the Module, then this field will return the maximum value (in mW).
  UINT32  MaxMbbAveragePowerTimeConstant; // Returns the maximum supported value of the Memory Bandwidth Boost Average Power Time Constant in milliseconds.
  UINT32  MbbAveragePowerTimeConstantStep; // Returns the increments in milliseconds allowed by the firmware when setting the Memory Bandwidth Boost Average Power Time Constant
  UINT32  MaxAveragePowerReportingTimeConstant; // Returns the maximum supported value of the Reporting Average Power Time Constant in milliseconds
  UINT32  AveragePowerReportingTimeConstantStep; // Returns the increments in milliseconds allowed by the firmware when setting the Average Power Reporting Time Constant
} FIS2_1_OR_LATER_PWR_MGMT;

typedef union {
  FIS1X_PWR_MGMT Fis1x;
  FIS2_0_PWR_MGMT Fis200;
  FIS2_1_OR_LATER_PWR_MGMT Fis201;
} POWER_MANAGEMENT_POLICY;

typedef struct {
  UINT16              CTST;       // Controller Temperature Shutdown Threshold
  UINT16              MTST;       // Optane Media Temperature Shutdown Threshold
  UINT16              MTSTT;      // Optane Media Throttling Start Threshold
  UINT16              MTSPT;      // Optane Media Throttling Stop Threshold
  UINT16              CTSTT;      // Controller Throttling Start Threshold
  UINT16              CTSPT;      // Controller Throttling Stop Threshold
} OPTANE_DEVICE_CHAR;

typedef struct dimmInformation {
  UINT16              vendorID;                           // Vendor Identification
  UINT16              deviceID;                           // Device Identification
  UINT16              revisionID;                         // Revision Identification
  UINT16              ifc;                                // Interface Format Code
  struct firmwareRev  fwr;                                // Firmware Revision
  UINT8               fswr;                               // Featured SW Required Mask
  UINT16              nbw;                                // Number of Block Windows
  UINT32              ombcr;                              // Offset of Block Mode Control Region
  UINT32              rc;                                 // Raw Capacity Usabled size (volatile + persistent)
  UINT8               mf[NGN_MAX_MANUFACTURER_STRLEN];    // Manufacturer
  UINT8               sn[NGN_MAX_SERIALNUMBER_STRLEN];    // Serial Number
  UINT8               Pn[NGN_MAX_PARTNUMBER_STRLEN];      // Part Number
  UINT8               dimmSku;                            // Dimm SKU info Bit 0:MemorymodeEnabled 1: StoragemodeEnabled 2:App-DirectmodeEnabled 3:DieSparingCapable 4:SoftProgrammableSKU 5:MemorymodeEncryptionEnabled 6:AppDirectmodeEncryptionEnabled 7:StoragemodeEncrytionEnabled
  UINT8               Uid[NGN_MAX_UID_STRLEN];            // Unique ID
  UINT16              ifce;                               // Interface Format Code Extra
  UINT16              api;                                // API Version
  UINT8               AitDramPoll;                        // Enable/Disable Polling
} DIMM_INFO_STRUCT, *DIMMINFORMATION;

typedef struct fnvInformation {
  struct firmwareRev  fwr;                                // Firmware Revision
  UINT8               fwrType;                            // Firmware Type
  UINT8               sfwStatus;
  struct firmwareRev  sfwr;
  UINT8               sfwrType;
  UINT8               Lfus;
  UINT8               commitId[COMMIT_ID];
}*FNVINFORMATION;

struct platformConfiguration {
  UINT32              currentSize;
  UINT32              totalSize;
};

typedef struct fnvCISBuffer {
  struct dimmInformation        dimmInfo[MAX_SOCKET][MAX_CH][MAX_DIMM];
  DIMM_PARTITION_DATA           dimmPart[MAX_SOCKET][MAX_CH][MAX_DIMM];
  struct platformConfiguration  platformConfig[MAX_SOCKET][MAX_CH][MAX_DIMM];
  UINT32                        platformData[NUM_PAYLOAD_REG];
  UINT32                        platformSize;
  UINT64_STRUCT                 securityNonce;
  UINT16                        hostAlerts[MAX_HOST_TRANSACTIONS];
  struct fnvSystemTime          time;
} FNV_CIS_BUFFER, *FNVCISBUFFER;

typedef struct {
  UINT8   PerfKnobId;    // performance knob identifier
  UINT16  PerfKnobValue; // performance knob value based on FIS 1.14
} PERFORMANCE_KNOB;

UINT32
SetFnvSystemTime (PSYSHOST     Host,
  UINT8        socket,
  UINT8        ch,
  UINT8        dimm,
  UINT8        *mbStatus
  );

/**
Initializes Dcpmm Power Management Policy

This routine issues GetPowerManagementPolicy and SetPowerManagementPolicy command.

@param[in]  Host                  - Pointer to System Host (root) structure
@param[in]  Socket                - Socket Number
@param[in]  Ch                    - DDR Channel ID
@param[in]  Dimm                  - Dimm number of the channel
@param[in]  FisVersion            - FisVersion of the firmware interface
**/
VOID
InitDcpmmPowerManagementPolicy (
  IN  SYSHOST                  *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm,
  IN  UINT16                   FisVersion
  );

UINT32
SetPlatformConfigData (
  IN  SYSHOST      *Host,
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  IN  UINT8         Dimm,
  IN  UINT8         PartitionId,
  IN  UINT32        Offset,
  IN  FNVCISBUFFER  FnvBuffer,
  OUT UINT8        *MailboxStatus
  );

UINT32
GetPlatformConfigData (
  IN  SYSHOST      *Host,
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  IN  UINT8         Dimm,
  IN  UINT8         PartitionId,
  IN  UINT8         RetrieveOption,
  IN  UINT32        Offset,
  OUT FNVCISBUFFER  FnvBuffer,
  OUT UINT8        *MailboxStatus
  );

/**
  Sets Latch System Shutdown State

  @param[in]  Host     - Pointer to System Host (root) structure
  @param[in]  Socket   - Socket Number
  @param[in]  Ch       - DDR Channel ID
  @param[in]  Dimm     - Dimm number of the channel
  @parma[in]  State    - Latch System Shutdown State (enable/disable)
  @param[out] MbStatus - Pointer to 8 bit status code from status register
**/
UINT32
SetLatchSystemShutdownState (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     State,
  OUT  UINT8    *MbStatus
  );

/**
  Sets Extended ADR State

  @param[in]  Host     - Pointer to System Host (root) structure
  @param[in]  Socket   - Socket Number
  @param[in]  Ch       - DDR Channel ID
  @param[in]  Dimm     - Dimm number of the channel
  @parma[in]  State    - Extended ADR State (enable/disable)
  @param[out] MbStatus - Pointer to 8 bit status code from status register
**/
UINT32
SetExtendedAdrState (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT8     State,
  OUT  UINT8    *MbStatus
  );

UINT32
GetSecurityState (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         ch,
  UINT8         dimm,
  UINT32        *security,
  UINT32        *OptStatus,
  UINT8         *mbStatus
  );

UINT32
SecureEraseUnit (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         ch,
  UINT8         dimm,
  UINT8         *passphrase,
  UINT8         *mbStatus
  );

UINT32
SetDimmPartitionInfo (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         ch,
  UINT8         dimm,
  UINT32        twoLM,
  UINT32        PM,
  FNVCISBUFFER fnvBuffer,
  UINT8        *mbStatus
  );

UINT32
GetDimmPartitionInfo (
  PSYSHOST     Host,
  UINT8        socket,
  UINT8        ch,
  UINT8        dimm,
  FNVCISBUFFER fnvBuffer,
  UINT8        *mbStatus
  );

/**
  *Updated 06/12/2020*
  *CrowPass FIS 3.0 Build 3.0.01616*

  Routine Description:
  SaveDdrtTrainingFailureCode will send the BIOS training failure code to the DCMPMM DIMM

  @param[in]  WarningCode           - Major warning code.
  @param[in]  MinorWarningCode      - Minor warning code.
  @param[in]  Socket                - Socket Number
  @param[in]  Channel               - DDR Channel ID
  @param[in]  Dimm                  - Dimm number of the channel

  @retval 0 SUCCESS
  @retval 1 FAILURE
**/
UINT32
SaveDdrtTrainingFailureCode (
  IN UINT8        WarningCode,
  IN UINT8        MinorWarningCode,
  IN UINT8        Socket,
  IN UINT8        Channel,
  IN UINT8        Dimm
);

/**
Issues Identify command to DIMM and returns info provided.

@param[in]  Host          - Pointer to the system Host (root) structure
@param[in]  Socket        - Socket Number
@param[in]  Ch            - DDR Channel ID
@param[in]  Dimm          - DIMM number
@param[in]  AitDramFlag   - AIT Dram Flag to check if BIOS support for FIS 1.11 is enabled/disabled
@param[out] DimmInfo      - Pointer to DIMM information structure
@param[out] MailboxStatus - Pointer to 8 bit status code from status register

@retval 0 SUCCESS
@retval 1 FAILURE
**/
UINT32
IdentifyDimm (
  IN  SYSHOST          *Host,
  IN  UINT8             Socket,
  IN  UINT8             Ch,
  IN  UINT8             Dimm,
  IN  BOOLEAN           AitDramFlag,
  OUT DIMM_INFO_STRUCT *DimmInfo,
  OUT UINT8            *MailboxStatus
  );

UINT32
GetFnvInfo (
  PSYSHOST            Host,
  UINT8               socket,
  UINT8               ch,
  UINT8               dimm,
  FNVINFORMATION      FnvInformation,
  UINT8               *mbStatus
  );

/**
  This function disables the extended ADR

  @param[in] Host   - Pointer to sysHost, the system host (root) structure
  @param[in] Socket - Socket Id

  @retval None
**/
VOID
DisableExtendedAdr (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**
  This function disables the LSS Latch on warm boot flow

  @param[in] Host   - Pointer to sysHost, the system host (root) structure
  @param[in] Socket - Socket Id

  @retval None
**/
VOID
DisableLastSystemShutdownLatch (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**
  FMC CSR unlock sequence for a channel

  @param[in] Socket      - Processor socket ID
  @param[in] Ch          - Current channel

  @retval EFI_UNSUPPORTED if the feature is not supported
  @retval EFI_SUCCESS otherwise

**/
EFI_STATUS
EFIAPI
FmcCsrUnlockCh (
  IN  UINT8    Socket,
  IN  UINT8    Ch
  );

UINT32
FnvPollingBootStatusRegister (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   bitmask
  );

UINT32
FnvDdrtIoInit (
  PSYSHOST      Host,
  UINT8         socket,
  UINT8         payload
  );

UINT32
FnvMailboxInterfaceStart (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT32   inputPayload[NUM_PAYLOAD_REG],
  UINT32   opcode
  );

UINT32
FnvMailboxInterfaceFinish (
  PSYSHOST     Host,
  UINT8        socket,
  UINT8        ch,
  UINT8        dimm,
  UINT32       outputPayload[NUM_PAYLOAD_REG],
  UINT32       opcode,
  UINT8        *mbStatus
  );

UINT32
SetDieSparingPolicy (
  PSYSHOST       Host,
  UINT8          socket,
  UINT8          ch,
  UINT8          dimm,
  UINT8          Enable,
  UINT8          *mbStatus
  );

UINT32
GetDieSparingPolicy (
  PSYSHOST       Host,
  UINT8          socket,
  UINT8          ch,
  UINT8          dimm,
  UINT8          *Enable,
  UINT8          *supported,
  UINT8          *mbStatus
  );

VOID
ReadBootStatusRegisters (
  IN  SYSHOST                   *Host,
  IN  UINT8                      Socket,
  IN  UINT8                      Ch,
  IN  UINT8                      Dimm,
  OUT BOOT_STATUS_REGISTER      *BootStatusRegister,
  OUT BOOT_STATUS_HIGH_REGISTER *BootStatusHighRegister
  );

/**
  This function reads the FW Feature Status Register.

  @param[in]  Host                  Pointer to the sysHost structure.
  @param[in]  Socket                Socket index.
  @param[in]  Ch                    Channel index on socket.
  @param[in]  Dimm                  Dimm index on channel.
  @param[out] FeatureStatusReg      Pointer to the value of Feature Status Register.

  @retval EFI_SUCCESS               Read the Feature Status Reister successfully.
  @retval EFI_INVALID_PARAMETER     Some of the input parameters are invalid.
**/
EFI_STATUS
EFIAPI
ReadFeatureStatusRegister (
  IN  SYSHOST                 *Host,
  IN  UINT8                   Socket,
  IN  UINT8                   Ch,
  IN  UINT8                   Dimm,
  OUT FEATURE_STATUS_REGISTER *FeatureStatusReg
  );

/**
Clears FW logs & resets registers to default state.

@param[in]  Host     - Pointer to System Host (root) structure
@param[in]  Socket   - Socket Number
@param[in]  Ch       - DDR Channel ID
@param[in]  Dimm     - Dimm number of the channel
@param[out] MbStatus - Pointer to 8 bit status code from status register
**/
UINT32
FactoryResetClear (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  UINT8       Dimm,
  IN  UINT8      *MbStatus
  );

VOID
ReportPrevBootDcpmError (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  );
//
// MemFmcRegIndex.c
//
typedef enum {
  MB_REG_COMMAND = 0,
  MB_REG_NONCE0,
  MB_REG_NONCE1,
  MB_REG_STATUS,
} MB_REGS;

/**

Routine Description: GetRegIndex will return the index of the NVMDIMM
Mailbox Register that are #defined in the NVMCTLR Register header.

Arguments.

  @param RegSPA      - Register that is in MBSpa.
  @retval Index of the register if found, else -1.

**/
INT16
GetRegIndex (
  UINT64    Reg
  );

//
// MemFmcCmdControl.c

/**
  Muli-use function to either get or set control delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param dimm     - DIMM number
  @param mode     - GSM_READ_CSR - Read the data from hardware and not cache
                  -  GSM_READ_ONLY - Do not write
                  -  GSM_WRITE_OFFSET - Write offset and not value
                  -  GSM_FORCE_WRITE - Force the write
  @param value    - Value to program or offset
  @param *minVal  - Current minimum control delay
  @param *maxVal  - Current maximum control delay

  @retval Pi delay value

**/

MRC_STATUS
GetSetCtlDelayFnv (
  PSYSHOST    Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    mode,
  INT16    *value,
  UINT16   *minDelay,
  UINT16   *maxDelay
  );

/**
  Muli-use function to either get or set command delays based on the provided group number

  @param Host     - Pointer to sysHost, the system Host (root) structure
  @param socket   - Socket number
  @param ch       - Channel number
  @param dimm     - DIMM number
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
GetSetCmdGroupDelayFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value,
  UINT16    *minVal,
  UINT16    *maxVal
  );

/**
  Muli-use function to either get or set command delays based on the provided group number

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
GetSetCmdDelayFnv (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  MRC_GT   group,
  UINT8    mode,
  INT16    *value
  );

MRC_STATUS
GetSetCmdVrefFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     mode,
  INT16     *value
  );

MRC_STATUS
GetSetCmdVrefFnvBackside (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     mode,
  INT16     *value
  );

/**
  Muli-use function to either get or set clock delays based on the provided group number

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
GetSetClkDelayFnv (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    mode,
  INT16    *value
  );

/**
  Display FNV/EKV Media/Thermal Error Log Information & Entries

  @param Host           Pointer to the system host (root) structure
  @param Socket         Socket Number
  @param Ch             DDR Channel ID
  @param Dimm           DIMM Number
  @param LogType        Log type
                        0: Media Log
                        1: Thermal Log
  @param LogInfo        Log info
                        0: Retrieve Log Data
                        1: Retrieve Log Info (SequenceNumber and RequestCount are ignored)
  @param ReturnData     Pointer to the structure to hold the results

**/
VOID
FnvDisplayErrorLog (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Ch,
  UINT8                 Dimm,
  UINT8                 LogType,
  UINT8                 LogInfo,
  ERROR_LOG_RETURN_DATA *ReturnData
  );

/**
  Retrieves Error Log from FNV/EKV FW

  @param Host           Pointer to the system host (root) structure
  @param Socket         Socket Number
  @param Ch             DDR Channel ID
  @param Dimm           DIMM Number
  @param LogType        Log type
                        0: Media Log
                        1: Thermal Log
  @param LogLevel       Log level
                        0: Low Priority Log
                        1: High Priority Log
  @param LogInfo        Log info
                        0: Retrieve Log Data
                        1: Retrieve Log Info (SequenceNumber and RequestCount are ignored)
  @param SequenceNumber Sequence number to start at
  @param RequestCount   Number of log entries to request from FW
  @param ReturnData     Pointer to the structure to hold the results
  @param MailboxStatus  Pointer to 8 bit status code from status register

  @retval 0 SUCCESS
  @retval 1 FAILURE

**/
UINT32
FnvGetErrorLog (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Ch,
  UINT8                 Dimm,
  UINT8                 LogType,
  UINT8                 LogLevel,
  UINT8                 LogInfo,
  UINT16                SequenceNumber,
  UINT16                RequestCount,
  ERROR_LOG_RETURN_DATA *ReturnData,
  UINT8                 *MailboxStatus
  );

/**
  Retrieves then displays Error Log information from FNV/EKV over SMBUS mailbox

  @param Host           Pointer to the system host (root) structure
  @param Socket         Socket Number
  @param Ch             DDR Channel ID
  @param Dimm           DIMM Number
  @param LogType        Log type
                        0: Media Log
                        1: Thermal Log
  @param LogLevel       Log level
                        0: Low Priority Log
                        1: High Priority Log

  @retval 0 SUCCESS
  @retval 1 FAILURE

**/
UINT32
FnvGetSmbusErrorLog (
  PSYSHOST              Host,
  UINT8                 Socket,
  UINT8                 Ch,
  UINT8                 Dimm,
  UINT8                 LogType,
  UINT8                 LogLevel
);

/**
  This function gets the FFW digest data from the specified DCPMM DIMM.

  @param[in] Host                         Pointer to the sysHost structure.
  @param[in] SocketId                     Socket index.
  @param[in] ChId                         Channel index.
  @param[in] DimmId                       DIMM index.
  @param[in] DigestLen                    Digest data length in units of UINT32.
  @param[out] FfwDigest                   Pointer to the FFW digest data.
  @param[out] FfwDigest1                  Pointer to the FFW digest1 data.

  @retval MRC_STATUS_SUCCESS              This function is executed successfully.
  @retval MRC_STATUS_NOT_FOUND            DCPMM DIMM is not found on the specified DIMM slot.
  @retval MRC_STATUS_INVALID_PARAMETER    Some of the input parameters are invalid.

**/
MRC_STATUS
GetFfwDigest (
  IN  PSYSHOST Host,
  IN  UINT8    SocketId,
  IN  UINT8    ChId,
  IN  UINT8    DimmId,
  IN  UINT8    DigestLen,
  OUT UINT32   *FfwDigest,
  OUT UINT32   *FfwDigest1
  );

/**

  This routine caches the FNV DDRTIO CSRs required by training steps

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Ch                                - Channel number (0-based)

  @retval N/A

**/
VOID
UpdateFnvDdrioCache10nm (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  );

VOID
EarlyFnvConfigAccess (
  PSYSHOST                  Host,
  UINT8                     socket
  );

VOID
LateFnvConfigAccess (
  PSYSHOST                  Host,
  UINT8                     socket
  );

UINT32
ReadFnvCfg (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT32    Reg
  );

UINT32
WriteFnvCfg (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Dimm,
  IN  UINT32    Reg,
  IN  UINT32    Data
  );

/**
Routine Description: Collect necessary SPD data for CR DXE/SMM driver

@param[in] Host         - Pointer to the system host (root) structure
@param[in] Socket       - Socket Number
@param[in] Channel      - DDR Channel ID
@param[in] Dimm         - DIMM Number
**/
VOID
GatherDdrtSPDData (
  IN  SYSHOST                 *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm
  );

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
  );

/**

Checks DQ swizzling and programs registers to account for it

@param Host    - Pointer to sysHost, the system Host (root) structure struct
@param socket  - Socket Id

@retval None

**/
VOID
CheckAndProgramDqSwizzleAep (
  PSYSHOST Host,
  UINT8 socket
  );

/**

Enable / disable 32 B2B GNTs without NODATA. Needed only for DDRT

@param Host                              - Pointer to sysHost
@param Socket                            - Socket Id
@param Value                             - Enable / Disable

@retval N/A

**/
VOID
Grant32ContEn (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Value
  );

/**

  BIOS shall drive tristate on RespA and RespB

  @param[in]  Socket    - Socket

  @retval status

  **/
VOID
EFIAPI
DdrtDriveRespARespBHigh (
  IN  UINT8     Socket
  );

/**

  Routine Description: EmulationPoll is an assembly polling loop for reading CSRs
  EmulationPoll allows you to poll multiple bits

  @param Host    - Pointer to sysHost, the system Host (root) structure struct
  @param socket  - Socket Id
  @param BoxInst - Box Instance, 0 based
  @param Offset  - Register offset; values come from the auto generated header file
  @param mask    - Bit mask of bits we are polling
  @param logic   - Polling for 0 or 1

  @retval None

**/
VOID
EmulationPoll (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 BoxInst,
  UINT32 Offset,
  UINT32 mask,
  UINT8 logic
  );

VOID
Resync (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    clk
  );

VOID
RelockPll (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    clk
  );

/**
  DDRT Clock Movement

  @param[in]      Socket      - Socket number
  @param[in]      Ch          - Memory channel number
  @param[in]      OrgClk      - Original clk number with step size of clkpi/128
  @param[in]      TargetClk   - Target clk number with step size of clkpi/128
  @param[in, out] NewClk      - Pointer to New clk number with step size of clkpi/128

  @retval TRUE  - Allowed
          FALSE - Not allowed
*/
BOOLEAN
EFIAPI
IsDdrtClockMovementAllowed (
  IN     UINT8  Socket,
  IN     UINT8  Ch,
  IN     UINT8  OrgClk,
  IN     UINT8  TargetClk,
  IN OUT UINT8  *NewClk
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
GetCmdGroupLimitsFmc (
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     Dimm,
  IN    MRC_LT    Level,
  IN    MRC_GT    Group,
  OUT   UINT16    *MinLimit,
  OUT   UINT16    *MaxLimit
  );

/**

  Returns a pointer to the DIMM_PARTITION_DATA structure within an FNVCIS buffer

  @param FnvBuffer  Pointer to FNVCIS structure

  @retval Address of the DIMM partition structure array in FnvBuffer

**/
DIMM_PARTITION_DATA (*GetFnvCisBufferDimmPartition (
  FNVCISBUFFER FnvBuffer
  ))[MAX_SOCKET][MAX_CH][MAX_DIMM];

/**

  Determine if disabled Nvmdimm got recovered by DFX format from previous boot

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)

  @retval TRUE  - NVMDIMM is recovered
          FALSE - NVMDIMM is not recovered

**/
BOOLEAN
DetectNvmdimmDisableChange (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  );

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
  );

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
  );

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
  );

/** Get the type of FMC on a given channel

  @param[in]  Socket  The socket containing the DDRT DIMM
  @param[in]  Channel The channel containing the DDRT DIMM

  @retval The type of FMC (e.g. FMC_EKV_TYPE, FMC_BWV_TYPE, etc.)

**/
UINT8
EFIAPI
GetFmcType (
  IN  UINT8 Socket,
  IN  UINT8 Channel
  );

/**
 Remap MRA to CWV address

@param[in]      Host        - Pointer to sysHost
@param[in]      Socket      - Socket Id
@param[in]      Ch          - Ch number
@param[in]      Dimm        - Dimm number (zero-based)
@param[in out]  Address     - Address for the MRS write
@param[in out]  Cw          - CW flag

@retval none

**/
VOID
RemapMraAddress(
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN OUT UINT16 *Address,
  IN OUT UINT8  *Cw
);

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
);

/**

Enable/Disable NVMDIMM C2

@param[in] Host    - Pointer to sysHost, the system host (root) structure struct
@param[in] Socket  - current socket
@param[in] Value   - TRUE: Disable, FALSE: Enable

@retval None

**/
VOID
ConfigNvmDimmC2 (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN BOOLEAN Value
  );

/** DDRT 2.0: For CWV A0, BIOS must issue BRST to the databuffer when it asserts DRST low.

  @param[in] Host           Pointer to the system host (root) structure
  @param[in] Socket         The socket number where the DIMM resides
  @param[in] Channel        The channel number where the DIMM resides on the provided socket

  @retval None

**/
VOID
DdrtAssertBrst (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Channel
  );

/**
  Displays the contents of the FW Boot Status Registers.

  @param[in] Host   - Pointer to System Host (root) structure
  @param[in] Socket - Socket Number
  @param[in] Ch     - DDR Channel ID
  @param[in] Dimm   - Dimm number of the channel

  @retval VOID      - None
**/
VOID
ShowBsr (
  IN SYSHOST *Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm
  );

/**
  Sets performance knob value based on knob and CR modes to FNV/EKV FW

  This routine issues the Set Features / Performance Knob command and
  sets the data from the provided buffer.

  @param[in]  Host                  - Pointer to System Host (root) structure
  @param[in]  Socket                - Socket Number
  @param[in]  Ch                    - DDR Channel ID
  @param[in]  Dimm                  - Dimm number of the channel
  @param[in]  PerformanceKnob       - Pointer to structure containing DIMM performance knob information
  @param[out] MailboxStatus         - Pointer to 8 bit status code from status register

  @retval 0                         - SUCCESS
  @retval 1                         - FAILURE
**/
UINT32
SetPerformanceKnob (
  IN  SYSHOST                 *Host,
  IN  UINT8                    Socket,
  IN  UINT8                    Ch,
  IN  UINT8                    Dimm,
  IN  PERFORMANCE_KNOB        *PerformanceKnob,
  OUT UINT8                   *MailboxStatus
  );

/**
  Check if training has failed on previous boot

  @param[in]  Socket                - Socket Number

  @retval                           - MRC_STATUS_SUCCESS
**/
MRC_STATUS
EFIAPI
CheckDdrtIoTrainingStatus (
  IN  UINT8                    Socket
  );

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
ProgramMr0Ddrt2 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  );

/**
Enables Write Leveling Mode for DDRT2 ranks

@param[in] Host              - Pointer to sysHost
@param[in] Socket            - Socket Id
@param[in] Ch                - Ch number
@param[in] Dimm              - Dimm number (zero-based)
@param[in] Rank              - Rank number (zero-based)
@param[in] Mode              - Write Leveling Mode

@retval N/A

**/
VOID
ProgramWlMr2Ddrt2(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank,
  IN UINT8     Mode
);

/**
  CWV Tx Vref build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  SysBootMode       Boot Mode.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
CacheTxVrefCwvTarget (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN BootMode  SysBootMode
  );

/**

  DDRT2 Receive Enable Training

  @param[in]  Host      - Pointer to sysHost
  @param[in]  Socket    - Socket
  @param[in]  Ch        - Channel numer within the socket
  @param[in]  SubChMask - Mask of subchannels within the channel
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Rank      - Rank number of the DIMM
  @param[in]  Enable    - TRUE to enable Receive Enable training mode

  @retval status

  **/
VOID
EFIAPI
SetFMCReceiveEnableModeDdrt2 (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubChMask,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  BOOLEAN   Enable
  );

/**
Program DDRT2 Dram TxDqDelay

@param[in] Host -  Pointer to sysHost
@param[in] Dimm - Dimm number(zero - based)
@param[in] Value - value for ReadDqsOffset

@retval N / A
**/
VOID
ProgramDramTxDqDelayDdrt2 (
  IN   PSYSHOST  Host,
  IN   UINT8     Dimm,
  IN   UINT8     Value
  );

/*
Enables write leveling in CWV IO

@param[in] Host - Pointer to sysHost
@param[in] Socket - Socket Id
@param[in] Ch - Ch number
@param[in] Dimm - Dimm number(zero - based)
@param[in] Value - value for cwvwriteleven

@retval N / A
*/
VOID
ProgramCwvWriteLevel(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Value
);

/**
General function for Get/Set CS Vref for CWV

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Ch number
  @param[in] Dimm        - Dimm number (zero-based)
  @param[in] SubCh       - Sub channel number
  @param[in] Signal      - Signal to train (zero-based)
  @param[in] Mode        - GSM_READ_CSR - Read the data from hardware and not cache
                         - GSM_FORCE_WRITE - Force the write
  @param[in out] *CsVrefValue - Vref setting % of VDD (zero-based)

  @retval MRC_STATUS_SUCCESS if entry is returned
  @retval MRC_STATUS_FAILURE if the entry is not supported
  @retval MRC_STATUS_UNSUPPORTED if input not supported mode
  @retval MRC_STATUS_SIGNAL_NOT_SUPPORTED if signal is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
GetSetDcsVrefDdrt2 (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      Dimm,
  IN UINT8      SubCh,
  IN UINT8      Signal,
  IN UINT8      Mode,
  IN OUT UINT8  *CsVrefValue
);

/**
General function for Get/Set CA Vref for FMC

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Ch number
  @param[in] Dimm        - Dimm number (zero-based)
  @param[in] SubCh       - Sub channel number
  @param[in] Signal      - Signal to train (zero-based)
  @param[in] Mode        - GSM_READ_CSR - Read the data from hardware and not cache
                         - GSM_FORCE_WRITE - Force the write
  @param[in out] *CaVrefValue - Vref setting % of VDD (zero-based)

  @retval MRC_STATUS_SUCCESS if entry is returned
  @retval MRC_STATUS_FAILURE if the entry is not supported
  @retval MRC_STATUS_UNSUPPORTED if input not supported mode
  @retval MRC_STATUS_SIGNAL_NOT_SUPPORTED if signal is not supported
  @retval MRC_FUNCTION_NOT_IMPLEMENTED if the function is not implemented

**/
MRC_STATUS
GetSetDcaVrefDdrt2 (
  IN PSYSHOST     Host,
  IN UINT8        Socket,
  IN UINT8        Ch,
  IN UINT8        Dimm,
  IN UINT8        SubCh,
  IN GSM_CSN      Signal,
  IN UINT8        Mode,
  IN OUT UINT8    *CaVrefValue
);

/**
Programs rw01_parity_control.blk_cmd_dq_buffer to block/unblock BCOM commands during DCS or DCA

@param[in] Host        - Pointer to sysHost
@param[in] Socket      - Socket Id
@param[in] Enable      - Enable bit

@retval N/A

**/

VOID
EnableDisableBcom(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN BOOLEAN   Enable
);

/**
  Checks to see if the DDRT Round Trip value timing should be used

  @param[in]  Socket                - Socket Number
  @param[in]  Channel               - DDR Channel ID

  @retval  TRUE                     - DDRT Round Trip value should be used
  @retval  FALSE                    - Default Round Trip value should be used
**/
BOOLEAN
EFIAPI
DdrtRoundTripIsNeeded (
  IN  UINT8     Socket,
  IN  UINT8     Channel
);


/**
  Determine if clock movement should be avoided to prevent FMC from hanging.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)

  @retval TRUE  - clock movement should be avoided
  @retval FALSE - clock movement is allowed.

**/
BOOLEAN
FmcAvoidClockMovementInSocket (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
);

/**
  Determine if clock movement should be avoided to prevent FMC from hanging.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket

  @retval TRUE  - clock movement should be avoided
  @retval FALSE - clock movement is allowed.

**/
BOOLEAN
FmcAvoidClockMovementInChannel (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
);

/**
  Determine if clock movement should be avoided to prevent FMC from hanging.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket
  @param[in]  Dimm        - Dimm number within the Channel

  @retval TRUE  - clock movement should be avoided
  @retval FALSE - clock movement is allowed.

**/
BOOLEAN
FmcAvoidClockMovementInDimm (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm
);

/**

Programs ddrt_hsio_func_defeature0.clear_ddrt_err to 1 then 0 to de-assert ERR# and Req# after DCS training

@param[in] Host        - Pointer to sysHost
@param[in] Socket      - Socket Id

@retval N/A

**/
VOID
DeassertDdrtErrAndReq (
  IN PSYSHOST  Host,
  IN UINT8     Socket
);

/**
  Enable Resync FSM on all DDRT DIMMs present on the given socket

  @param Socket   Socket number

  @retval         None
**/
VOID
FnvEnableResyncFsm (
  UINT8    Socket
  );

/**
  Determine if the given socket and channel is populated with a DIMM that may require frequency limitations

  @param[in]    Host    Pointer to host structure
  @param[in]    Socket  Socket number to check
  @param[in]    Channel Channel number to check

  @retval   FALSE   No DCPMM DIMMs found on the given socket & channel that require frequency limitations
  @retval   TRUE    DCPMM DIMMs found on the given socket & channel that may require frequency limitations
**/
BOOLEAN
EFIAPI
FmcEnforceDcpmmFreqLimit (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  );

/**
  Collects DDRT Freq from B2P Mailbox, and compare if setup current Freq limit same as DIMM init freq

  @param[in] Host      - Point to sysHost
  @param[in] Socket    - CPU socket ID

  @retval TRUE  - Dcpmem freq is changed
          FALSE - Dcpmem freq is not changed

**/
BOOLEAN
FmcCheckDdrtFreqChanged (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket
  );

/**
  Program direct snoop settings to DQ buffer

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Ch number
  @param[in] SubChMask   - Mask of subchannel numbers to be updated
  @param[in] Dimm        - Dimm number (zero-based)
  @param[in] Rank        - Rank number (zero-based)
  @param[in] Address     - Address for the MRS write
  @param[in] Cw          - Data to write

  @retval none
**/
VOID
EFIAPI
ProgramSnoopSettingsToDb (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubChMask,
  IN UINT8      Dimm,
  IN UINT8      Rank,
  IN UINT8      Address,
  IN UINT8      Data
  );

/**
  Get internal write leveling WL_ADJ_end value.

  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket
  @param[in]  Dimm        - Dimm number within the Channel

  @retval   - write leveling WL_ADJ_end result

**/
UINT16
GetDdrt2WLAdjEndValue (
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm
  );

/*
  Programs Dfe Bias Gain in the CWV IO

  @param[in] Host - Pointer to sysHost
  @param[in] Socket - Socket Id
  @param[in] Ch - Ch number
  @param[in] Dimm - Dimm number(zero - based)
  @param[in] Value - Bias gain

  @retval N / A
*/

VOID
ProgramCwvRxBiasGain(
IN PSYSHOST Host,
IN UINT8    Socket,
IN UINT8    Ch,
IN UINT8    Dimm,
IN UINT8    Value
);

/*
  Programs DFE DCA LFSR Seed for Training Accelerator

  @param[in] Host - Pointer to sysHost
  @param[in] Socket - Socket Id
  @param[in] Ch - Ch number
  @param[in] Dimm - Dimm number(zero - based)
  @param[in] Signal - DCA Signal

  @retval N / A
*/
VOID
ProgramCaLfsrSeed(
IN PSYSHOST   Host,
IN UINT8      Socket,
IN UINT8      Ch,
IN UINT8      Dimm,
IN GSM_CSN    Signal
);

#endif   // _MEM_FMC_IP_LIB_H_
