/** @file
  Header file for the Mem MC IP library implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _MEM_MC_IP_LIB_INTERNAL_H_
#define _MEM_MC_IP_LIB_INTERNAL_H_

#include <ReferenceCodeDataTypes.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <MemCommon.h>
#include <Library/MemMcIpLib.h>
#include "Include/CmiMcRegStruct.h"

// Energy contribution of the DDRT Read
#define DEFAULT_ENERGY_CONTRIBUTION_DDRT_READ     15

// Energy Contribution of the DDRT write
#define DEFAULT_ENERGY_CONTRIBUTION_DDRT_WRITE      15

// Power contribution of ACT command in both OLTT and ET energy counters.
#define DEFAULT_UDIMM_PWR_CONTRIBUTION_ACT_COMMAND  29
#define DEFAULT_LRDIMM_PWR_CONTRIBUTION_ACT_COMMAND 31
#define DEFAULT_RDIMM_PWR_CONTRIBUTION_ACT_COMMAND  11

// Power contribution of CAS RD/RDS4 command in both OLTT and ET energy counters.
#define DEFAULT_PWR_CONTRIBUTION_CAS_RD     15

// Power contribution of CAS WR/WRS4 command in both OLTT and ET energy counters.
#define DEFAULT_PWR_CONTRIBUTION_CAS_WR     15

//  Power contribution of 1x REF or SRE command
#define DEFAULT_UDIMM_PWR_REF_DIMM  133
#define DEFAULT_LRDIMM_PWR_REF_DIMM 106
#define DEFAULT_RDIMM_PWR_REF_DIMM  167

//  Define DDRT2 same rank Four Activate Window
#define DEFAULT_DDRT2_SAME_RANK_tFAW  4

// Add 1 dclk for write CAS to write CAS delay
#define WRITECAS_TO_WRITECAS_DELAY_OFFSET_1_DCLK  1

//
// MC bank scheduler selection
//
#define BANK_SCHEDULER_SELECTION_0    0  //{BG[2:0],BA[1:0]};- DDR4/5 1-8 Ranks  DDR4:BG2==0
#define BANK_SCHEDULER_SELECTION_1    1  //{BG[2:0],BA[1], SR[0]}; - DDR4/5 9-16 Ranks DDR4:BG2==0
#define BANK_SCHEDULER_SELECTION_2    2  //{BG[2:0],SR[1:0]}; -DDR4/5 17-32 Ranks DDR4:BG2==0
#define BANK_SCHEDULER_SELECTION_3    3  //{BG[2:0],R[1],BA[0]};
#define BANK_SCHEDULER_SELECTION_4    4  //{BG[2:0],R[1:0]};
#define BANK_SCHEDULER_SELECTION_5    5  //{BG[2:0],R[2:1]};

//
// Per bank / all bank refresh mode setting
//
#define REFRESH_ALL_BANK_MODE               0
#define REFRESH_PER_BANK_MODE               1

#pragma pack (push, 1)
//
// Electrical turnaround settings
//
typedef struct {
  UINT32  t_rrsg;
  UINT32  t_wwsg;
  UINT32  t_rwsg;
  UINT32  t_wrsg;
  UINT32  t_rrsr;
  UINT32  t_wwsr;
  UINT32  t_rwsr;
  UINT32  t_wrsr;
  UINT32  t_rrdr;
  UINT32  t_wwdr;
  UINT32  t_rwdr;
  UINT32  t_wrdr;
  UINT32  t_rrdd;
  UINT32  t_wwdd;
  UINT32  t_rwdd;
  UINT32  t_wrdd;
  UINT32  t_rrds;
  UINT32  t_wwds;
  UINT32  t_rwds;
  UINT32  t_wrds;
  UINT32  t_rrdlr;
} DDR_TURNAROUND_SETTINGS;

#pragma pack (pop)

//
// Datatypes used only within this implmentation.
//

/**
  This function programs the MC 2LM near memory cache configuration register.

  @param[in] MemTechType  Memory technology type.
  @param[in] ScktId       Socket index.
  @param[in] McId         Memory controller index on the socket.
  @param[in] ChId         Channel index on the memory controller.
  @param[in] NmData       Information required to define a 2LM near memory region.

  @retval N/A
**/
VOID
McSetNm2lmCacheCfg (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          ScktId,
  IN UINT8          McId,
  IN UINT8          ChId,
  IN NM_2LM_DATA    *NmData
  );

/**
  This function programs the MC 2LM near memory caching offset register.

  @param[in] MemTechType  Memory technology type.
  @param[in] ScktId       Socket index.
  @param[in] McId         Memory controller index on the socket.
  @param[in] ChId         Channel index on the memory controller.
  @param[in] NmData       Information required to define a 2LM near memory region.

  @retval N/A
**/
VOID
McSetNmCachingOffset (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          ScktId,
  IN UINT8          McId,
  IN UINT8          ChId,
  IN NM_2LM_DATA    *NmData
  );

//
// Structure used for memory timing min/max checking
//
typedef struct {
  MEM_TIMING_TYPE     Timing;           // This is the timing parameter
  CHAR8               *TimingString;    // Output string describing this task (potentially output to debug serial port).
  MIN_MAX_PAIR        MinMax;           // Min and Max values for the processor
} MEM_TIMING_LIMITS;

//
// Structure that associates various representations of DDR speed
//
typedef struct {
  UINT32  DdrFreqMts;     // DDR transfer rate in MT/s
  UINT8   FreqIndex;      // Index into MRC list of DDR frequencies
  UINT8   QclkRatio;      // QCLK ratio
  UINT8   PcuDmfc;        // Encoding from PCU DMFC register
} DDR_FREQ_ENCODINGS;

//
// Values for MC turnaround idle bypass offsets
//
typedef struct {
  UINT8   t_rrd_s_bypass;
  UINT8   t_rrd_l_bypass;
  UINT8   t_rcd_wr_bypass;
  UINT8   t_rcd_rd_bypass;
  UINT8   t_rc_bypass;
  UINT8   t_ras_bypass;
} IDLE_BYPASS_OFFSETS;

//
// Values for MC JEDEC timing register offsets
//
typedef struct {
  UINT8   t_rrd_s;
  UINT8   t_rrd_l;
  UINT8   t_faw;
  UINT8   t_faw_impre;
} JEDEC_TIMING_OFFSETS;

//
// Structures and definitions for bank and page table scheduler selection
//
typedef struct {
  UINT8 MaxRanks;
  UINT8 EncodedCsModeEnabled;
  UINT8 DramDensityGb;
  UINT8 DimmPerChannel;
  UINT8 BankSchedulerSelection;
  UINT8 PageTableIndexSelection;
} SCHEDULER_SELECTION;

typedef enum {
  CTE_CONFIG_UPDATE_CL  = 0,
  CTE_CONFIG_UPDATE_TCL,
  CTE_CONFIG_UPDATE_TCWL,
  CTE_CONFIG_UPDATE_TIMING_MODE
} CTE_CONFIG;

#define ANY_DENSITY 0

//
// Prototypes for function used ony within this implementation,
// this is not an API header.
//

VOID
GetWpqRidtoRt10nm (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  );

/**

  Enables Write CRC

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number

  @retval None

**/
VOID
EnableWrCrc (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket
  );

/**

  Programs MC to enable Write CRC

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval None

**/
VOID
EnableWrCrcMc (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch
  );

/**

  Configure memory mode DDR5 or DDR4

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number

  @retval None

**/
VOID
ConfigureMemoryMode (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket
  );

/**
  Increases the read/write CL and CWL, respectively

  @param Host   - Pointer to sysHost
  @param socket - Socket Id
  @param ch     - Channel number

  @retval VOID
**/

VOID
IncreaseReadWritePreamble10nm (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch
  );

/**
  Return a pointer to the DDR frequency encoding table

  @param[in]    Host        - Pointer to sysHost structure
  @param[in]    Socket      - Socket number
  @param[out]   TablePtr    - A pointer to the caller's table pointer
  @param[out]   TableSize   - Pointer to the caller's table size parameter

  @retval SUCCESS
**/
UINT32
GetDdrFreqEncodingTable (
  IN        PSYSHOST            Host,
  IN        UINT8               Socket,
  OUT       DDR_FREQ_ENCODINGS  **TablePtr,
  OUT       UINT8               *TableSize
  );

/**

  SetVLSModePerChannel Configure on the fly burst mode for Static Virtual Lockstep mode

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/

VOID
SetVLSModePerChannel (
  PSYSHOST Host,
  UINT8 socket
  );

/**

  Configure Lockstep mode

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/

VOID
SetVLSRegionPerChannel (
  PSYSHOST Host,
  UINT8 socket
  );

/**

  Programs timing parameters

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param socket  - Socket number
  @param ch      - Channel number

  @retval None

**/
VOID
ProgramTimings10nm (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch
  );

/**

  Restores memory controller turnaround timing parameters

  @param Host    - Pointer to sysHost, the system Host (root) structure
  @param Socket  - Socket number
  @param Ch      - Channel number

  @retval None

**/
VOID
RestoreTurnaroundTimings (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  );

/**
  Set Long Preamble for write and read in MC

  @param[in] Host               - Point to sysHost
  @param[in] Socket             - Socket number
  @param[in] Ch                 - Channel number
  @param[in] WritePreambleState - PREAMBLE_2TCLK is long write preamble, else 1TCLK preamble is programmed
  @param[in] ReadPreambleState  - PREAMBLE_2TCLK is long read preamble, else 1TCLK preamble is programmed

  @retval N/A
**/
VOID
SetLongPreambleMc (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch,
  IN      UINT8    WritePreambleState,
  IN      UINT8    ReadPreambleState
  );

/**

  Apply DDRT CMI Workaround
  2S Supercollider causes TOR Time Out - Stuck FMFILLRD on DDRT Channel

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Current Socket

  @retval N/A

**/
VOID
DdrtCmi2SSuperColliderWa (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**
  Increase Dcpmm Read Drain timeout to prevent system hangs

  @param [in]   Socket 0 based number

  @retval       N/a
**/
VOID
MaximizeDcpmmReadDrainTimeout (
  IN UINT8  Socket
  );

/**

  Ddrt Clear Ck Idle Mask

  @param[in] Socket    - Current Socket

  @retval N/A

**/
VOID
DdrtClearCkIdleMask (
  IN UINT8     Socket
  );

/**

  Get CMPL program delay.

  @param[in] Socket    CPU Socket number.

  @retval              CMPL delay.

**/
UINT8
GetCmplDelayDdr (
  IN    UINT8     Socket
  );

/**

  Specify the hold time of DDRT Write data path before it can switch back to DDR4 mode

  @param[in]  Host      Pointer to SysHost
  @param[in]  Socket    The socket containing the timer to initialize
  @param[in]  Channel   The channel containing the timer to initialize
  @param[in]  HoldTime  Time that the Write data path be held

  @retval None

**/
VOID
InitializeDdrtWrPathHoldTime (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     HoldTime
  );

/**
  Specify the delay to switch to DDRT write data path

  @param[in]  Host     Pointer to SysHost
  @param[in]  Socket   The socket containing the timer to initialize
  @param[in]  Channel  The channel containing the timer to initialize
  @param[in]  Delay    Delays to switch to DDRT write data path

  @retval None

**/
VOID
InitializeDdrtWrPathDelay (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Delay
  );

/*
  Program WPQ thershold to avoid deadlock.

  @param[in] Host          SysHost pointer
  @param[in] Socket        0 based socket number
  @param[in] Channel       0 based channel index
  @param[in] Value         Value to program in bit field

  @retval   EFI_SUCCESS             Value programmed successfully
  @retval   EFI_INVALID_PARAMETER   Invalid value passed

*/
EFI_STATUS
ProgramWPQThershold (
  IN PSYSHOST Host,
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT32 Value
  );

/**
  Get minimum vc floor

  @param[in]   Vc0Floor         - Vc0 Floor Value
  @param[in]   Vc1Floor         - Vc1 Floor Value
  @param[in]   Vc3Floor         - Vc3 Floor Value

  @param[out]  *VcFloor          -   Pointer to minimum vc floor

  @retval None
**/
VOID
GetMinimumVcFloor (
  IN  UINT32  Vc0Floor,
  IN  UINT32  Vc1Floor,
  IN  UINT32  Vc3Floor,
  OUT UINT32  *VcFloor
  );

/**

  Get the DIMM_MTR register offset for a particular DIMM.

  This additionally provides bounds checking for the DIMM value, comparing it against the number of members in the
  DimmMtrRegOffset array.

  @param[in]  Dimm  The target DIMM number

  @retval The DIMM_MTR register offset for the target DIMM

**/
UINT32
GetDimmMtrRegOffset(
  IN UINT8 Dimm
  );

/**

  Checks DQ swizzling and programs registers to account for it

  @param[in] Host    - Pointer to sysHost
  @param[in] socket  - Socket number

  @retval None

**/
VOID
CheckAndProgramDqSwizzle10nm (
  IN PSYSHOST  Host,
  IN UINT8     socket
  );

/**

  Enable Data DLL Off feature, which turns off DDRIO data DLL when MemSS is in CKE
  Power down (Auto Power Down or Pre-Charge Power Down) or Opp Self Refresh mode.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Channel - Memory channel number

  @retval N/A

**/
VOID
PmLpModeDataDllOff (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  );

/**
  Program odt_value to HIGH in certain configurations to save power.

  @param[in] Socket     - Socket number
  @param[in] Ch         - Channel number

  @retval    EFI_SUCCESS If odt_value is successfully set.
**/
EFI_STATUS
EFIAPI
SetPowerSaveOdt (
  IN    UINT8     Socket,
  IN    UINT8     Ch
  );

/**
  Internal function to check if OppSR Enable is TRUE for any rank.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Channel - Memory channel number

  @retval  OppSR Auto status.
*/
BOOLEAN
IsOppSrEnabled (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  );

/**
  Internal function to check if CKE Power Down Enable is TRUE for any rank.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Channel - Memory channel number

  @retval  CKE Power Down Enable status.
*/
BOOLEAN
IsCkePowerDownEnabled (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  );

/**

  PMIC Wrapper Function
  Enables the PMIC on the specified DIMM

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number
  @param[in] Dimm   - DIMM number

  @retval SUCCESS;

**/
UINT32
PmicEnableChip (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**
  Return values for MC turnaround idle bypass offsets

  @param[in]  MemTechType         - Memory Technology Type
  @param[out] IdleBypassOffsets   - Pointer to caller's buffer pointer
                                    Updated by this function to pointer to the idle bypass offsets table

  @retval EFI_SUCCESS if values are supported
          EFI_UNSUPPORTED otherwise
**/
EFI_STATUS
GetIdleBypassOffsets (
  IN  MEM_TECH_TYPE         MemTechType,
  OUT IDLE_BYPASS_OFFSETS   **IdleBypassOffsets
  );

/**
  Program JEDEC timing registers

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number

  @retval N/A
**/
VOID
ProgramJedecTimings (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch
  );

/**
  Return pointer to and length of the memory timing limit table

  @param[out] MemTimingLimitsTable       - Pointer to memory timing limit table
  @param[out] TableLength                - Pointer to number of entries in the table

  @retval EFI_SUCCESS                    - Table is found
          EFI_NOT_FOUND                  - Table is not found

**/
EFI_STATUS
GetMemTimingLimitsTable (
  OUT MEM_TIMING_LIMITS  **MemTimingLimitsTable,
  OUT UINT16             *TableLength
  );

/**
  Calculate the max supported round trip delay in Qclks.

  @param[in]  GrantToEridDclk   Grant-to-ERID delay in Dclks

  @retval   The max supported round trip delay in Qclks
**/
UINT32
CalculateMaxRoundTripQclk (
  IN  UINT32  GrantToEridDclk
  );

/**
  Program page policy registers after DDR training

  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number

  @retval N/A
**/
VOID
DdrSchedulerPagePolicyConfigLate (
  IN    UINT8    Socket,
  IN    UINT8    Ch
  );

/**
  Program timing registers related to write major mode

  @param Host        - Pointer to sysHost
  @param socket      - Socket number
  @param ch          - Channel number

  @retval N/A
**/
VOID
InitWriteMajorMode (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  );

/**
  Program starvation registers

  @param Host        - Pointer to sysHost
  @param socket      - Socket number
  @param ch          - Channel number

  @retval N/A
**/
VOID
InitStarvationCounters (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  );

/**
  Program scheduler registers, chip specific

  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number

  @retval N/A
**/
VOID
DdrSchedulerConfigLateChip (
  IN UINT8 Socket,
  IN UINT8 Channel
  );

/**
  Set Mirror mode

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number
  @param[in] Data    - 1: Set mirror mode; 0: Not set mirror mode

  @retval N/A

**/
VOID
SetMirrorMode (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Data
  );

/**
  Program scheduler registers, chip specific

  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number

  @retval N/A
**/
VOID
ConfigureRidVcbeforeTraining (
  IN UINT8 Socket,
  IN UINT8 Channel
  );

/**

  Enable nSddc mode

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
VOID
EnableNsddcMode (
  PSYSHOST Host,
  UINT8    socket
  );

/**

  Configure Enhanced SDDC

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
VOID
EnableEnhancedSddc (
  PSYSHOST Host,
  UINT8    socket
  );

/**

  Configure idle bypass timings

  @param[in] MemTechType  - Memory Technology Type
  @param[in] Socket       - Socket number
  @param[in] Ch           - Channel number

  @retval N/A

**/
VOID
EnableBsIdleBypass (
  IN  MEM_TECH_TYPE  MemTechType,
  IN  UINT8          Socket,
  IN  UINT8          Ch
  );

/**
  Program MemHot Temperature range (Disable, High, Mid, Low)

  @param[in] MemTechType         - Memory technology type
  @param[in] Socket              - Socket Id, 0 based
  @param[in] Channel             - Channel number, 0 based
  @param[in] Dimm                - Dimm number, 0 based
  @param[in] MemHotOutputOnlyOpt - Option to select Memhot temperature range enable
                                 - Available Options are
                                     MEMHOT_OUTPUT_ONLY_DIS          = 0
                                     MEMHOT_OUTPUT_ONLY_HIGH         = 1
                                     MEMHOT_OUTPUT_ONLY_HIGH_MED     = 2
                                     MEMHOT_OUTPUT_ONLY_HIGH_MED_LOW = 3

  retval N/A
**/
VOID
SetMemHotTemperatureRange (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          Socket,
  IN UINT8          Channel,
  IN UINT8          Dimm,
  IN UINT8          MemHotOutputOnlyOpt
  );

/**

  Configures Off pkg Memtrip fields

  @param [in]  ThermalThrottlingOptions      Throttling Options
  @param [in]  Socket                        Socket Number

  @retval N/A

**/
VOID
EFIAPI
ConfigureOffPkgBits (
  IN UINT16 ThermalThrottlingOptions,
  IN UINT8  Socket
  );

/**

  Configures in pkg Memtrip fields

  @param [in]  ThermalThrottlingOptions      Throttling Options
  @param [in]  Socket                        Socket Number

  @retval N/A

**/
VOID
EFIAPI
ConfigureInPkgBits (
  IN UINT16 ThermalThrottlingOptions,
  IN UINT8  Socket
  );

/**

  Get the PM_CMD_PWR register offset for a particular DIMM.

  This additionally provides bounds checking for the DIMM value, comparing it against the number of members in the
  PmCmdPwrRegOffsets array.

  @param[in]  Dimm  The target DIMM number

  @retval The PM_CMD_PWR register offset for the target DIMM

**/
UINT32
GetPmCmdPwrRegOffset (
  IN UINT8 Dimm
  );

/**

  Configure DDRT persistent domain delay
  Executed in late MRC when training is complete and switching the MC to normal mode

  @param[in]  Host    - Pointer to SysHost
  @param[in]  Socket  - Socket number
  @param[in]  Ch      - Channel number

  @return   N/A
**/
VOID
DdrtPersistentDomainDelayLate (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch
  );

/**
  Get the silicon specific scheduler read starve count and wait count value

  @param[out] StarveCount - scheduler read starve count
  @param[out] StarveCount - scheduler read wait count

  @retval   N/A
**/
VOID
GetReadStarveAndWaitCount (
  OUT UINT32*  StarveCount,
  OUT UINT32*  WaitCount
  );

/**
  Get the silicon specific scheduler WMM enter and exit threshold

  @param[out] Enter - enter threshold
  @param[out] Exit  - exit threshold

  @retval   N/A
**/
VOID
GetWmmThreshold (
  OUT UINT32*  EnterThreshold,
  OUT UINT32*  ExitThreshold
  );

/**
  Get the silicon specific scheduler preemption enable setting.

  @param[out] Read  - enable WMM read preemption
  @param[out] Write - enable RMM write preemption
  @param[out] Miss  - enable WMM read preemption
  @param[out] Hit   - enable RMM write preemption

  @retval   N/A
**/
VOID
GetPreemptionEnableSetting (
  OUT UINT32*  Read,
  OUT UINT32*  Write,
  OUT UINT32*  Miss,
  OUT UINT32*  Hit
  );

/**
  Program DDRT RD/WR/GNT to Power Down Timing Constraints.

  @param[in]  Host             Pointer to sysHost.
  @param[in]  Socket           Socket number.
  @param[in]  Ch               Channel number.
  @param[in]  RdPdSeparation   Minimum READ to PD command separation (in DCLK).
                               Ignore when it is 0.
  @param[in]  WrPdSeparation   Minimum WRITE to PD command separation (in DCLK).
                               Ignore when it is 0.
  @param[in]  GntPdSeparation  Minimum GRANT to PD command separation (in DCLK).
                               Ignore when it is 0.

  @return     EFI_SUCCESS      Successfully programmed Power Down timing constraints.
*/
EFI_STATUS
ConfigDdrtPowerDownTiming (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     RdPdSeparation,
  IN  UINT8     WrPdSeparation,
  IN  UINT8     GntPdSeparation
  );

/**
  Enables Wait DDRT Ref Idle for all channels with DDRT DIMMs installed if the SI
  workaround is enabled

  @param[in]  Host    Pointer to SysHost
  @param[in]  Socket  Socket ID

  @retval     None
**/
VOID
EFIAPI
SetWaitDdrtRefIdle (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  );


/**
  Get CKE idle timer.

  Get the appropriate value to write to the cke_idle_timer field of CKE_LLn and
  PKGC_CKE CSRs.

  @param[in]  Host          Pointer to sysHost.
  @param[in]  Socket        Socket number to convert for.
  @param[in]  Nanoseconds   Number of nanoseconds to convert.

  @return     Value to write to cke_idle_timer CSR field(s).
**/
UINT8
GetCkeIdleTimer (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Nanoseconds
  );

/**
  Set post-refresh-enable chicken bits.

  For a given channel, set any chicken bits for the chip which need to happen
  after refresh is enabled.

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket number
**/
VOID
SetPostRefreshEnableChickenBits (
  IN PSYSHOST   Host,
  IN UINT8      Socket
  );

/**
  Checks the support for widths mix withing a channel

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @return TRUE if the mixing is supported,
          FALSE if not
**/
BOOLEAN
IsMixingWidthsSupported (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**
  Set Write CRC chicken bits when Write CRC is disabled.

  @param[in]  Host              Pointer to sysHost.
  @param[in]  Socket            Socket number.
**/
VOID
DisableWriteCrcChickenBits (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**

  Disable Clock Gate in Scheduler

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

  @retval N/A

**/
VOID
DisClkGateSchedMc (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**
  Disable wait ST_REF_DQSOSC_STRT_REQ state during retry

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

  @retval N/A

**/
VOID
DisRefDqsOscWaitDuringRetry (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**

  SPR A0 workaround cas2cad_dd timing restriction

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

  @retval N/A

**/
VOID
TrrddDdrt2ChickenBit (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**

  Config served MRR entry in RDB

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number
  @param[in] Enable  - 1: Enable served MRR entry in RDB;  0: Disable served MRR entry in RDB

  @retval N/A

**/
VOID
ConfigRidMrrEntry (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN BOOLEAN  Enable
  );

/**

  Disable corresponding error in internal_error_dp register when DDRT populated

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number

  @retval N/A

**/
VOID
DisRtUnexpSchedActive (
  IN UINT8    Socket,
  IN UINT8    Ch
  );

/**
  Initializes MC for cpgc training

  @param[in] Socket     - Socket number
  @param[in] Ch         - Channel number

  @retVal   None
**/
VOID
InitMcForCpgcTraining (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**
  Early config on McDp CMI Credit to enable CPGC for memory training

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If Mc doesn't require early config of CMI credit for CPGC
          EFI_SUCCESS            If Mc requires early config of CMI credit  for CPGC
**/
EFI_STATUS
EFIAPI
EarlyConfigMcCmi (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**
  Initializes the MC Req Credits Cfg register

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If Mc doesn't support this
          EFI_SUCCESS            Otherwise
**/
VOID
InitMcCmiReqCpCfg (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**
  Set Req Credit Cfg Done bit to indicate Bios has programmed the desired credit count values

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If Mc doesn't support this
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
SetMcReqCreditCfgDone (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**
  Polling whether or not CPGC hardware has completed request credit exchange

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number

  @retval EFI_UNSUPPORTED        If Mc doesn't support this
          EFI_TIMEOUT            Polling timeout
          EFI_SUCCESS            Otherwise
**/
EFI_STATUS
EFIAPI
PollCpgcReqCreditsInitialized (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**
  Configure the command to data completion delay

  @param[in]  Socket    - Socket number

  @return None
**/
VOID
EarlyConfigCmdToDataCompletionDelay (
  IN    UINT8     Socket
  );

/**
  Configure CTE environment

  @param[in] Host          - Pointer to sysHost
  @param[in] Socket        - Socket number
  @param[in] Ch            - Channel number
  @param[in] ConfigType    - Config type
  @param[in] ConfigData    - Config data

  @return None
**/
VOID
CteConfiguration (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN CTE_CONFIG ConfigType,
  IN UINT32     ConfigData
  );

/**

  Enable Cpgc Cmi Credit for memory training.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

  @retval N/A

**/
VOID
EnableCpgcCmiCredit (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**
  Program Ecc Retry Fail Limit

  @param[in] Socket     - Socket number
  @param[in] Ch         - Channel number

  @retVal   None
**/
VOID
ProgramEccRetryFailLimit (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

/**
  Program the command to data completion delay

  @param[in]  Socket    - Socket number

  @return None
**/
VOID
ProgramCmdToDataCompletionDelay (
  IN    UINT8     Socket
  );

/**
  Configure DQS CAP error wait time.

  @param[in]  Host      Pointer to sysHost
  @param[in]  Socket    Socket number
  @param[in]  Channel   Channel number
**/
VOID
ConfigDqsCapErrorWaitTime (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Channel
  );

/**
  Check if DIMM Capacity is supported or not by CPU SKU.

  @param[in]  Host       Pointer to sysHost.
  @param[in]  Socket     Socket number.
  @param[in]  Ch         Channel number.
  @param[in]  Dimm       Dimm number.

  @retVal   TRUE      DIMM Capacity is supported by CPU SKU
  @retVal   FALSE     DIMM Capacity is NOT supported by CPU SKU
**/
BOOLEAN
CheckIsDramSupportByCpuSku (
  IN PSYSHOST                  Host,
  IN UINT8                     Socket,
  IN UINT8                     Ch,
  IN UINT8                     Dimm
  );

/**
  Set the Rcomp Timer configuration after training is complete.

  @param[in] Host         - Pointer to sysHost.
  @param[in] Socket       - Socket Id
  @param[in] Ch           - Channel number within the socket

  @retval N/A

**/
VOID
RcompTimerConfig (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**
  Program delay from ZQ to anycmd delay

  @param[in] Socket     - Socket number

  @retVal   None
**/
VOID
SetDdrtZqToAnyCmdDelay (
  IN UINT8 Socket
  );

/**


  Late SbMcaCtl Register configuration

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - current socket #

  @return N/A
**/
VOID
SetSbMcaCtl (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  );

/**

  Get CkIndex and CsIndex per Soc

  @param[in]  Socket   - Socket Id
  @param[in]  Ch       - Channel number (0-based)
  @param[in]  Dimm     - DIMM number (0-based)
  @param[in]  Rank     - Rank number (0-based)
  @param[in]  DimmType - Dimm Type
  @param[out] CkIndex  - Pointer to CkIndex
  @param[out] CsIndex  - Pointer to CsIndex

  @retval N/A

**/
VOID
GetClkCsIndexSoc (
  IN     UINT8   Socket,
  IN     UINT8   Ch,
  IN     UINT8   Dimm,
  IN     UINT8   Rank,
  IN     UINT8   DimmType,
  IN OUT UINT8   *CkIndex,
  IN OUT UINT8   *CsIndex
  );

/**
  Disable Open Page Mode.

  @param[in] MemTechType  Memory technology type.
  @param[in] Socket       Socket number.
  @param[in] Ch           Channel number within a socket.

  @retVal   None

**/
VOID
DisableOpenPageMode (
  IN MEM_TECH_TYPE   MemTechType,
  IN UINT8           Socket,
  IN UINT8           Ch
  );

/**
  Set memory controller values for performance, to be performed around the switch to normal mode.

  @param[in]  Host    A pointer to the sys host structure
  @param[in]  Socket  The target socket

  @return N/A
**/
VOID
SwitchToNormalModeMcPerformance (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  );

/**
  Determine whether a timing value is out of bounds, taking appropriate actions if it is.

  If Value is out of bounds, it will be changed to the respective MinValue or MaxValue. Additionally, if ShowWarning is
  TRUE, a message will be printed and a warning will be raised.

  @param[in]      Socket        The socket where the value comes from
  @param[in]      Channel       The channel where the value comes from
  @param[in]      ShowWarning   Whether or not to print a message and raise a warning if Value is out of bounds
  @param[in]      TimingString  The name of the timing parameter
  @param[in]      MinValue      The minimum in-bounds value
  @param[in]      MaxValue      The maximum in-bounds value
  @param[in,out]  Value         The timing value to check

  @return   EFI_INVALID_PARAMETER   Value is out of bounds
            EFI_SUCCESS             Value is not out of bounds
**/
EFI_STATUS
HandleOutOfBoundsTiming (
  IN      UINT8   Socket,
  IN      UINT8   Channel,
  IN      BOOLEAN ShowWarning,
  IN      CHAR8   *TimingString,
  IN      UINT32  MinValue,
  IN      UINT32  MaxValue,
  IN  OUT UINT32  *Value
  );

/**
  Prepare requesters for initializing CMI Credit registers

  @param[in] Socket     - Socket number

  @retVal  MRC_STATUS_SUCCESS if the programming occured
  @retVal  MRC_FUNCTION_NOT_IMPLEMENTED otherwise
**/
MRC_STATUS
PrepareForCmiCreditProgramming (
  IN UINT8    Socket
);

/**
  Save the register default value of the read and write CmiCreditConfig registers

  @param[in]   Host      -   Pointer to SysHost
  @param[in]   Socket    -   Socket number

  @retval  N/A
**/
VOID
SaveCmiCreditConfigDefault (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket
  );

/**

  Save the default value of CmiCreditConfig registers

  @param[in]       Socket                    -  Socket number
  @param[in,out]   CmiCreditConfigContext    -  Pointer to a CMI_CREDIT_CONFIG_DEFAULT buffer

  @retval  N/A

**/
VOID
EFIAPI
SaveCmiCreditConfig (
  IN     UINT8                      Socket,
  IN OUT CMI_CREDIT_CONFIG_DEFAULT  *CmiCreditConfigContext
  );

/**

  Restore the values of CmiCreditConfig registers

  @param[in]   Socket                    -  Socket number
  @param[in]   CmiCreditConfigContext    -  Pointer to a CMI_CREDIT_CONFIG_DEFAULT buffer

  @retval  N/A

**/
VOID
EFIAPI
RestoreCmiCreditConfig (
  IN UINT8                      Socket,
  IN CMI_CREDIT_CONFIG_DEFAULT  *CmiCreditConfigContext
  );

/**
  Set Burst Chop based on ADDDCEn value

  @param Host  - Pointer to sysHost

  @retval N/A
**/
VOID
SetBurstChop (
  PSYSHOST Host
  );

/**

  Generate ASM polling loop for given register offset. Takes mask and logic as input
  for polling.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] BoxInst - Box Instance, 0 based
  @param[in] Offset  - Register offset; values come from the auto generated header file
  @param[in] Mask    - Bit mask of bits we are polling
  @param[in] Logic   - Polling for 0 or 1

  @retval None

**/
VOID
PollRegisterOffsetForEmulation (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    BoxInst,
  IN UINT32   Offset,
  IN UINT32   Mask,
  IN UINT8    Logic
  );

/**

  Configures CA parity control register

  @param[in]    Socket        Current socket under test (0-based)
  @param[in]    Channel       Current channel under test (0-based)
  @param[in]    RatioIndex    Current ratio index (frequency index)

  @retval   MRC_STATUS_SUCCESS  When CA parity control is configured

**/
MRC_STATUS
EFIAPI
ConfigureCaParityControl (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT8           RatioIndex
);

/**
  Program scheduler registers

  @param Host   - Pointer to sysHost
  @param Socket - Socket number
  @param Ch     - Channel number

  @retval N/A
**/
VOID
DdrSchedulerConfig (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch
  );

/**
  Program chip-specific ZQ calibration timing registers.

  @param[in]  Host        Pointer to sysHost
  @param[in]  Socket      Socket number
  @param[in]  Channel     Channel number
**/
VOID
InitZqCalTimingChip (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Channel
  );

/**
  Program chip-specific timings required after training and before Memtest

  @param[in]  Socket      Socket number
  @param[in]  Ch          Channel number

  @retval N/A

**/
VOID
SetMcTimingsLateConfig (
  IN UINT8    Socket,
  IN UINT8    Ch
  );

/**
  Program refresh timing registers

  @param[in] Host                - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket              - Socket Id
  @param[in] Ch                  - Channel number (0-based)

  @retval N/A

**/
VOID
ProgramRefreshTimingsChip (
  IN  PSYSHOST                Host,
  IN  UINT8                   Socket,
  IN  UINT8                   Ch
  );

/**
  Return override value for tREFI, if any

  @param[out] tREFI     - Set to override value if supported

  @retval MRC_STATUS_SUCCESS if tREFI override had been updated and is valid
  @retval MRC_STATUS_FAILURE otherwise - tREFI has not been updated
**/
MRC_STATUS
GetTrefiOverride (
  OUT UINT32      *tREFI
  );

/**
  Initialize the refresh rate based on policy input

  @param[in] Host                - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket              - Socket Id
  @param[in] Ch                  - Channel number (0-based)

  @retval N/A

**/
VOID
InitializeRefreshRate (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch
  );

/**
  Program scheduler blocking rules registers in late step

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval   MRC_STATUS_SUCCESS
**/
MRC_STATUS
DdrSchedulerBlockingRulesConfig (
  IN    UINT8    Socket,
  IN    UINT8    Ch
  );

/**
  Override memory controller count based on the die count.
  The GetMaxImc() always returns MAX_IMC regardless the die count.

  @param[in]     Socket  The target socket
  @param[in out] MaxImc  MC count

  @return N/A
**/
VOID
EFIAPI
UpdateMaxImcByDieCount (
  IN     UINT8      Socket,
  IN OUT UINT8     *MaxImc
  );

/**
  Set write to write timings for turnaround timings registers by offset.

  @param[in]     MemTechType     - Memory Technology Type
  @param[in]     Socket          - Socket number
  @param[in]     Offset          - Write to write timing offset

  @retval MRC_STATUS_SUCCESS            Turnaround CSRs read/write successful
  @retval MRC_STATUS_FAILURE            Turnaround CSRs read/write failed
  @retval MRC_FUNCTION_NOT_IMPLEMENTED  The function has not been implemented
**/
MRC_STATUS
EFIAPI
SetWriteToWriteTurnaroundTimingsByOffset (
  IN     MEM_TECH_TYPE  MemTechType,
  IN     UINT8          Socket,
  IN     UINT8          Offset
  );
/**
  Get the ERF_DDR4_CMD_REG address.

  @param[in] Index  ERF_DDR4_CMD_REG register index.

  @retval N/A
**/
UINT32
GetErfDdr4CmdRegAddress (
  UINT8  Index
  );

/**
  Get the ERF_DDR4_CMD_REG register numbers.

  @param  N/A

  @retval N/A
**/
UINT32
GetErfDdr4CmdRegNum (
  );

/**
  Program 32 to 39 bits of Wpq Pch Entry Enable registers

  @param[in] Socket   -  Zero based Socket number

  retval N/A
**/
VOID
EFIAPI
ProgramWpqPchEntryEnBits32To39 (
  IN UINT8 Socket
  );
#endif  // #ifndef _MEM_MC_IP_LIB_INTERNAL_H_
