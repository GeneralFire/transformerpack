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
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Library/ProcMemInitChipLib.h>
#include <SysHostChip.h>
#include <UncoreCommonIncludes.h>
#include "Include/MemMcRegs.h"
#include "Include/MemMcIpLibInternal.h"
#include <Library/SysHostPointerLib.h>
#include <Library/MemRcLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/KtiApi.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpTargetLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemCallTableLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/MemTurnaroundTimingLib.h>
#include <MemCommon.h>
#include <MemProjectSpecific.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SpdDecodeLib.h>
#include <Library/BaseMemoryLib.h>

//
// For DDR5, the panic refresh threshold for Normal Refresh (All Bank only) REFab is 5 and Fine Grain Refresh (All Bank or Same Bank) REFsb is 9.
// Design's recommendation is to set the panic watermark (tcrfp.ref_panic_wm) to max-2.
//
#define REF_PANIC_WM_SPR_NORMAL      3
#define REF_PANIC_WM_SPR_FINE_GRAIN  7

//
// Macros for vlues of tREFSBRD, tREFSBRD_slr and tREFSBRD_dlr defined in DDR5 JEDEC.
// The units are nanosecond.
//
#define T_REFSBRD_NANOSECOND         30
#define T_REFSBRD_DLR_NANOSECOND     15

//
//Macro for rdimmtimingcntl2.t_bank_stagger which should be set to the pre-si tested value of 0xC8
//
#define T_BANK_STAGGER_DCLOCK        0xC8

//
// Number of D clocks to block scheduler before checking returned safe signals.
// Note that the value used must be between 0x0b and 0x18.
// If the scheduler bypass logic is disabled, a value lower than 0x0b may be used for improved performance
//
#define T_REF_BLOCK_DDR5         0xC


//
// Some registers have built-in offsets that must be accounted for when programming target values
// If the target value is x and the register offset is y, then the register should be programed with x - y.
//

//
// Activate to Activate delay, same rank, different bank groups: tRRD_S - 2 dclks.
//
#define TRRD_S_REG_OFFSET             2
//
// Activate to Activate delay, same rank, same bank groups: tRRD_L - 2 dclks.
//
#define TRRD_L_REG_OFFSET             2
//
// Four Activate Window, same rank: DDR5: tFAW - 4 dclks; DDR4: tFAW - 2; HBM: tFAW -2.
//
#define TFAW_REG_DDR4_OFFSET          2
#define TFAW_REG_DDR5_OFFSET          4
#define TFAW_REG_HBM_OFFSET           2

//
// HBM2: Four ImplicitPRE Window: same pseudo-channel: max((tRRD_L+tRP),tFAW) - 2 dclks.
// DDR: Four Activate Window for Idle Bypass, same rank: DDR4: tFAW - 4 dclks; DDR5: tFAW - 6 dclks; (or the t_faw field - 2).
//
#define TFAW_IMPRE_REG_DDR4_OFFSET    4
#define TFAW_IMPRE_REG_DDR5_OFFSET    6
#define TFAW_IMPRE_REG_HBM_OFFSET     2

  //
  // The programming for the idle bypass timings comes directly from the register definitions
  //
  // t_rrd_s_bypass:  Bypass Activate to Activate delay, same rank, different bank groups. DDR4/5: tRRD_S - 4 dclks; HBM2: tRRD_S - 1 dclks.
  //
#define T_RRD_S_BYPASS_OFFSET_DDR   4
#define T_RRD_S_BYPASS_OFFSET_HBM   1
  //
  // t_rrd_l_bypass:  Bypass Activate to Activate delay, same rank, same bank groups. DDR4/5: tRRD_L - 4 dclks; HBM2: tRRD_L - 1 dclks.
  //
#define T_RRD_L_BYPASS_OFFSET_DDR   4
#define T_RRD_L_BYPASS_OFFSET_HBM   1
  //
  // t_rcd_wr_bypass: Bypass Activate to Write CAS delay, same bank: DDR4/5: t_rcd_wr - 5 dclks; HBM2: t_rcd_wr - 2 dclks.
  //
#define T_RCD_WR_BYPASS_OFFSET_DDR  5
#define T_RCD_WR_BYPASS_OFFSET_HBM  2
  //
  // t_rcd_rd_bypass: Bypass Activate to Read CAS delay, same bank: DDR4/5: t_rcd_rd - 5 dclks; HBM2: t_rcd_rd - 2 dclks.
  //
#define T_RCD_RD_BYPASS_OFFSET_DDR  5
#define T_RCD_RD_BYPASS_OFFSET_HBM  2
  //
  // t_rc_bypass:     Bypass Activate to Activate delay, same bank: DDR4/5: t_rc - 5 dclks; HBM2: t_rc - 2 dclks.
  //
#define T_RC_BYPASS_OFFSET_DDR      5
#define T_RC_BYPASS_OFFSET_HBM      2
  //
  // t_ras_bypass:    Bypass Activate to Precharge delay, same bank: DDR4/5: t_ras - 5 dclks; HBM2: t_ras - 0 dclks.
  //
#define T_RAS_BYPASS_OFFSET_DDR     5
#define T_RAS_BYPASS_OFFSET_HBM     0

JEDEC_TIMING_OFFSETS mDdr4JedecTimingOffsets = {TRRD_S_REG_OFFSET,
                                                TRRD_L_REG_OFFSET,
                                                TFAW_REG_DDR4_OFFSET,
                                                TFAW_IMPRE_REG_DDR4_OFFSET
                                               };

JEDEC_TIMING_OFFSETS mDdr5JedecTimingOffsets = {TRRD_S_REG_OFFSET,
                                                TRRD_L_REG_OFFSET,
                                                TFAW_REG_DDR5_OFFSET,
                                                TFAW_IMPRE_REG_DDR5_OFFSET
                                               };

JEDEC_TIMING_OFFSETS mHbmJedecTimingOffsets = {TRRD_S_REG_OFFSET,
                                                TRRD_L_REG_OFFSET,
                                                TFAW_REG_HBM_OFFSET,
                                                TFAW_IMPRE_REG_HBM_OFFSET
                                               };

IDLE_BYPASS_OFFSETS mDdrIdleBypassOffsets = {T_RRD_S_BYPASS_OFFSET_DDR,
                                             T_RRD_L_BYPASS_OFFSET_DDR,
                                             T_RCD_WR_BYPASS_OFFSET_DDR,
                                             T_RCD_RD_BYPASS_OFFSET_DDR,
                                             T_RC_BYPASS_OFFSET_DDR,
                                             T_RAS_BYPASS_OFFSET_DDR};

IDLE_BYPASS_OFFSETS mHbmIdleBypassOffsets = {T_RRD_S_BYPASS_OFFSET_HBM,
                                             T_RRD_L_BYPASS_OFFSET_HBM,
                                             T_RCD_WR_BYPASS_OFFSET_HBM,
                                             T_RCD_RD_BYPASS_OFFSET_HBM,
                                             T_RC_BYPASS_OFFSET_HBM,
                                             T_RAS_BYPASS_OFFSET_HBM};

MEM_TIMING_LIMITS mMemTimingLimits[] = {
// Timing            TimingString                Min     Max
  {tREFI,     CALL_TABLE_STRING ("tREFI"),      {0x0,  0x7FFF }, },
  {tRFC,      CALL_TABLE_STRING ("tRFC"),       {0x0,  0x07FF }, },
  {tRAS,      CALL_TABLE_STRING ("tRAS"),       {0x0,  0x007F }, },
  {tCWL,      CALL_TABLE_STRING ("tCWL"),       {0x0,  0x003F }, },
  {tCL,       CALL_TABLE_STRING ("tCL"),        {0x0,  0x003F }, },
  {tRP,       CALL_TABLE_STRING ("tRP"),        {0x0,  0x007F }, },
  {tRCD,      CALL_TABLE_STRING ("tRCD"),       {0x0,  0x007F }, },
  {tRRD,      CALL_TABLE_STRING ("tRRD"),       {0x2,  0x0021 }, }, // t_rrd_s[5] Requires an offset of -2
  {tRRD_L,    CALL_TABLE_STRING ("tRRD_L"),     {0x2,  0x0021 }, }, // t_rrd_l[5] Requires an offset of -2
  {tWTR,      CALL_TABLE_STRING ("tWTR"),       {0x4,  0x0041 }, }, // Requires an offset of -2
  {tWTR_S,    CALL_TABLE_STRING ("tWTR_S"),     {0x2,  0x0081 }, }, // t_wrsr[6] Requires an offset of -2
  {tWTR_L,    CALL_TABLE_STRING ("tWTR_L"),     {0x2,  0x0081 }, }, // t_wrsg[6] Requires an offset of -2
  {tCCD,      CALL_TABLE_STRING ("tCCD"),       {0x2,  0x0021 }, },
  {tCCD_L,    CALL_TABLE_STRING ("tCCD_L"),     {0x2,  0x0041 }, },
  {tRTP,      CALL_TABLE_STRING ("tRTP"),       {0x0,  0x007F }, },
  {tCKE,      CALL_TABLE_STRING ("tCKE"),       {0x0,  0x000F }, },
  {tFAW,      CALL_TABLE_STRING ("tFAW"),       {0x2,  0x0041 }, }, // Requires an offset of -2
  {tWR,       CALL_TABLE_STRING ("tWR"),        {0x0,  0x007F }, },
  {tPRPDEN,   CALL_TABLE_STRING ("tPRPDEN"),    {0x0,  0x0003 }, },
  {tXP,       CALL_TABLE_STRING ("tXP"),        {0x0,  0x007F }, },
  {tZQCS,     CALL_TABLE_STRING ("tZQCS"),      {0x20, 0x00FF }, }, // tZQCS in DCLK cycles (32 to 255, default is 64)
  {tSTAB,     CALL_TABLE_STRING ("tSTAB"),      {0x1,  0xFFFFF}, }, // Zero value in T_STAB is reserved and it is important to AVOID programming a zero value in the T_STAB
  {tMOD,      CALL_TABLE_STRING ("tMOD"),       {0x0,  0x001F }, },
  {tREFIx9,   CALL_TABLE_STRING ("tREFIx9"),    {0x0,  0x007F }, },
  {tXSDLL,    CALL_TABLE_STRING ("tXSDLL"),     {0x0,  0x0FFF }, }, // t_xsdll[12]
  {tXSOFFSET, CALL_TABLE_STRING ("tXSOFFSET"),  {0x0,  0x001E }, }, // t_xsoffset is shifted right one bit in hardware, so max is max(tXS), * 2
  {tRC,       CALL_TABLE_STRING ("tRC"),        {0x0,  0x00FF }, },
  {tRDA,      CALL_TABLE_STRING ("tRDA"),       {0x0,  0x00FF }, },
  {tWRA,      CALL_TABLE_STRING ("tWRA"),       {0x0,  0x00FF }, },
  {tWRPRE,    CALL_TABLE_STRING ("tWRPRE"),     {0x0,  0x00FF }, },
  {tWRRDA,    CALL_TABLE_STRING ("tWRRDA"),     {0x0,  0x00FF }, },
  {tRDPDEN,   CALL_TABLE_STRING ("tRDPDEN"),    {0x0,  0x007F }, },
  {tWRPDEN,   CALL_TABLE_STRING ("tWRPDEN"),    {0x0,  0x007F }, },
  {tRRSG,     CALL_TABLE_STRING ("tRRSG"),      {0x2,  0x0021 }, }, // t_rrsg[5], internal dclk delay: 2
  {tWWSG,     CALL_TABLE_STRING ("tWWSG"),      {0x2,  0x0041 }, }, // t_wwsg[6], internal dclk delay: 2
  {tRWSG,     CALL_TABLE_STRING ("tRWSG"),      {0x2,  0x0081 }, }, // t_rwsg[7], internal dclk delay: 2
  {tWRSG,     CALL_TABLE_STRING ("tWRSG"),      {0x2,  0x0081 }, }, // t_wrsg[7], internal dclk delay: 2
  {tRRSR,     CALL_TABLE_STRING ("tRRSR"),      {0x2,  0x0021 }, }, // t_rrsr[5], internal dclk delay: 2
  {tWWSR,     CALL_TABLE_STRING ("tWWSR"),      {0x2,  0x0041 }, }, // t_wwsr[6], internal dclk delay: 2
  {tRWSR,     CALL_TABLE_STRING ("tRWSR"),      {0x2,  0x0081 }, }, // t_rwsr[7], internal dclk delay: 2
  {tWRSR,     CALL_TABLE_STRING ("tWRSR"),      {0x2,  0x0081 }, }, // t_wrsr[7], internal dclk delay: 2
  {tRRDR,     CALL_TABLE_STRING ("tRRDR"),      {0x2,  0x0021 }, }, // t_rrdr[5], internal dclk delay: 2
  {tWWDR,     CALL_TABLE_STRING ("tWWDR"),      {0x2,  0x0021 }, }, // t_wwdr[5], internal dclk delay: 2
  {tRWDR,     CALL_TABLE_STRING ("tRWDR"),      {0x2,  0x0041 }, }, // t_rwdr[6], internal dclk delay: 2
  {tWRDR,     CALL_TABLE_STRING ("tWRDR"),      {0x2,  0x0021 }, }, // t_wrdr[5], internal dclk delay: 2
  {tRRDD,     CALL_TABLE_STRING ("tRRDD"),      {0x2,  0x0081 }, }, // t_rrdd[7], internal dclk delay: 2
  {tWWDD,     CALL_TABLE_STRING ("tWWDD"),      {0x2,  0x0081 }, }, // t_wwdd[7], internal dclk delay: 2
  {tRWDD,     CALL_TABLE_STRING ("tRWDD"),      {0x2,  0x0081 }, }, // t_rwdd[7], internal dclk delay: 2
  {tWRDD,     CALL_TABLE_STRING ("tWRDD"),      {0x2,  0x0081 }, }, // t_wrdd[7], internal dclk delay: 2
  {tRRDS,     CALL_TABLE_STRING ("tRRDS"),      {0x2,  0x0021 }, }, // t_rrds[5], internal dclk delay: 2
  {tWWDS,     CALL_TABLE_STRING ("tWWDS"),      {0x2,  0x0041 }, }, // t_wwds[6], internal dclk delay: 2
  {tRWDS,     CALL_TABLE_STRING ("tRWDS"),      {0x2,  0x0081 }, }, // t_rwds[7], internal dclk delay: 2
  {tWRDS,     CALL_TABLE_STRING ("tWRDS"),      {0x2,  0x0081 }, }, // t_wrds[7], internal dclk delay: 2
};
//
// Bank and page table scheduler selection tables
// Taken from 10nm_IMC_Structure_Decode_wave1_wave3_WW45_2018.xlxs
//
                                       // Ranks  EncCsEn  Density   DPC   BSS   PTIS
SCHEDULER_SELECTION mSchedSelectDdr4[] = {{ 1,     0,   ANY_DENSITY, 1,    0,    0   },
                                          { 2,     0,   ANY_DENSITY, 1,    0,    0   },
                                          { 4,     0,   ANY_DENSITY, 1,    0,    0   },
                                          { 4,     1,   ANY_DENSITY, 1,    0,    2   },
                                          { 8,     1,   ANY_DENSITY, 1,    1,    3   },
                                          {16,     1,   ANY_DENSITY, 1,    1,    2   },
                                          { 1,     0,   ANY_DENSITY, 2,    0,    0   },
                                          { 2,     0,   ANY_DENSITY, 2,    0,    1   },
                                          { 4,     0,   ANY_DENSITY, 2,    0,    1   },
                                          { 4,     1,   ANY_DENSITY, 2,    1,    2   },
                                          { 8,     1,   ANY_DENSITY, 2,    2,    1   },
                                          {16,     1,   ANY_DENSITY, 2,    2,    1   }
                                         };

                                       // Ranks  EncCsEn  Density   DPC   BSS   PTIS
SCHEDULER_SELECTION mSchedSelectDdr5[] = {{ 1,     0,       8,       1,    3,    0   },
                                          { 2,     0,       8,       1,    3,    0   },
                                          { 4,     0,       8,       1,    3,    0   },
                                          { 4,     1,       8,       1,    3,    2   },
                                          { 8,     1,       8,       1,    3,    2   },
                                          {16,     1,       8,       1,    1,    3   },
                                          { 1,     0,       8,       2,    0,    0   },
                                          { 2,     0,       8,       2,    3,    1   },
                                          { 4,     0,       8,       2,    3,    1   },
                                          { 4,     1,       8,       2,    3,    1   },
                                          { 8,     1,       8,       2,    1,    3   },
                                          {16,     1,       8,       2,    2,    1   },

                                          { 1,     0,      16,       1,    0,    0   },
                                          { 2,     0,      16,       1,    3,    4   },
                                          { 4,     0,      16,       1,    3,    0   },
                                          { 4,     1,      16,       1,    3,    2   },
                                          { 8,     1,      16,       1,    3,    2   },
                                          {16,     1,      16,       1,    1,    3   },
                                          { 1,     0,      16,       2,    0,    1   },
                                          { 2,     0,      16,       2,    3,    1   },
                                          { 4,     0,      16,       2,    3,    1   },
                                          { 4,     1,      16,       2,    3,    1   },
                                          { 8,     1,      16,       2,    1,    3   },
                                          {16,     1,      16,       2,    2,    1   }
                                         };

//
// Page Policy and Timing parameters
//
#define IDLE_PAGE_RST_VAL     64

//
// Performance settings
//
#define WMM_OPP_TIMER               0xF
#define WMM_OPP_READ_LIMIT          0xF
#define WMM_OPP_TIMER_GRAN          0x2
#define WMM_OPP_READ_LIMIT_GRAN     0x0

#define CAS_STARVE_COUNT            0x8
#define CAS_CRITICAL_COUNT          0x8

#define WRITE_WAIT_COUNT            0x100
#define WRITE_STARVE_COUNT          0x19FE
#define WRITE_STARVE_COUNT_B0       0xFD0

/**
  Get the maximum DRAM density for the channel for DDR DIMMs

  @param[in]  Host                    - Pointer to sysHost
  @param[in]  Socket                  - Socket number
  @param[in]  Ch                      - Channel number

  @retval DRAM density in Gb

**/
UINT16
GetMaxDramDensity (
  IN  PSYSHOST      Host,
  IN  UINT8         Socket,
  IN  UINT8         Ch
  )
{
  UINT8               Dimm;
  UINT16              MaxDramDensityGb;
  UINT16              DramDensityGb;
  struct dimmNvram   (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  MaxDramDensityGb = 0;
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((IsDimmPresent (Socket, Ch, Dimm)) && (!(IsDcpmmPresentDimm (Socket, Ch, Dimm)))) {
      DramDensityGb = EncodedSpdToDramDensity((*DimmNvList)[Dimm].sdramCapacity);
      if (DramDensityGb > MaxDramDensityGb) {
        MaxDramDensityGb = DramDensityGb;
      }
    }
  }

  return MaxDramDensityGb;
}

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
)
{
  PSYSHOST    Host;
  DDR4_CA_CTL_MCDDC_DP_STRUCT DdrCaCtl;

  Host = GetSysHostPointer ();

  DdrCaCtl.Data = MemReadPciCfgEp (Socket, Channel, DDR4_CA_CTL_MCDDC_DP_REG);

  //
  // Program DDR5 tPar Recovery in HCLKs
  //
  DdrCaCtl.Bits.tpar_recov = GetParityRecoveryDdr5 (Socket, RatioIndex) / 2;

  //
  // Program RDIMM/LRDIMM Present on channel
  //
  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    DdrCaCtl.Bits.erf_regpart0 = 1;
  }

  MemWritePciCfgEp (Socket, Channel, DDR4_CA_CTL_MCDDC_DP_REG, DdrCaCtl.Data);

  return MRC_STATUS_SUCCESS;
}

/**
  Get scheduler selection values

  @param[in]  Host                    - Pointer to sysHost
  @param[in]  Socket                  - Socket number
  @param[in]  Ch                      - Channel number
  @param[out] BankSchedulerSelection  - Pointer updated with the BankSchedulerSelection value
                                        for this channel configuration
  @param[out] PageTableIndexSelection - Poinater updated with the PageTableIndexSelection value
                                        for this channel configuration

  @retval SUCCESS if table entry found
          FAILURE otherwise
**/
MRC_STATUS
DdrSchedulerSelectLookup (
  IN  PSYSHOST      Host,
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  OUT UINT8         *BankSchedulerSelection,
  OUT UINT8         *PageTableIndexSelection
  )
{
  MRC_STATUS            Status = MRC_STATUS_FAILURE;
  UINT8                 Index;
  UINT8                 Dimm;
  UINT8                 MaxRanks;
  UINT8                 NumRanks;
  UINT8                 EncodedCsModeEnabled = 0;
  UINT16                DramDensityGb;
  UINT8                 DimmPerChannel = 1;
  UINT8                 TableLength = 0;
  SCHEDULER_SELECTION   *TablePointer = NULL;
  struct channelNvram(*ChannelNvList)[MAX_CH];
  struct dimmNvram   (*DimmNvList)[MAX_DIMM];

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Ignore DCPMM DIMMs for this lookup
  //
  DimmPerChannel = DdrDimmsPerChannel (Socket, Ch);
  if (DimmPerChannel == 0) {
    //
    // Nothing to do
    //
    return MRC_STATUS_FAILURE;
  }

  //
  // Determine if Encoded Chip Select mode is enabled (3DS DIMMs)
  //
  if ((*ChannelNvList)[Ch].encodedCSMode == 2) {
    // 3DS dimm present
    EncodedCsModeEnabled = 1;
  }

  //
  // Find the maximum number of ranks per DIMM in the channel
  //
  MaxRanks = 0;
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((IsDimmPresent (Socket, Ch, Dimm)) && (!(IsDcpmmPresentDimm (Socket, Ch, Dimm)))) {
       NumRanks = (*DimmNvList)[Dimm].numDramRanks;
       if (EncodedCsModeEnabled == 1) {
         NumRanks *= (*DimmNvList)[Dimm].dieCount;
       }
       if (NumRanks > MaxRanks) {
         MaxRanks = NumRanks;
      }
    }
  }

  //
  // Find the maximum DRAM density in Gb for the channel
  //
  DramDensityGb = GetMaxDramDensity (Host, Socket, Ch);

  //
  // Locate the lookup table
  //
  if (IsDdr5MemSsPresent (Host, Socket)) {
    TablePointer = mSchedSelectDdr5;
    TableLength = sizeof (mSchedSelectDdr5) / sizeof (mSchedSelectDdr5[0]);
  } else {
    TablePointer = mSchedSelectDdr4;
    TableLength = sizeof (mSchedSelectDdr4) / sizeof (mSchedSelectDdr4[0]);
  }

  //
  // Find the table entry that matches the channel configuration
  //
  for (Index = 0; Index < TableLength; Index++) {
    if ((TablePointer[Index].MaxRanks == MaxRanks) &&
        (TablePointer[Index].EncodedCsModeEnabled == EncodedCsModeEnabled) &&
        ((TablePointer[Index].DramDensityGb == DramDensityGb) || (TablePointer[Index].DramDensityGb == ANY_DENSITY)) &&
        (TablePointer[Index].DimmPerChannel == DimmPerChannel)) {

      *BankSchedulerSelection  = TablePointer[Index].BankSchedulerSelection;
      *PageTableIndexSelection = TablePointer[Index].PageTableIndexSelection;
      Status = MRC_STATUS_SUCCESS;
      break;
    }
  }
  return Status;
}

/**
  Determine maximum number of banks for the DDR DIMMs installed in the channel
  Return the number of banks encoded for the SCHEDULER_ENABLES_BS register.

  @param[in]  Host                    - Pointer to sysHost
  @param[in]  Socket                  - Socket number
  @param[in]  Ch                      - Channel number

  @retval Register encoded value for number of banks on the DDR DIMM in the channel
          with the maximum number of banks.
          Will be 0 if there are no DDR DIMMs in the channel.

**/
UINT8
NumBanksLookup (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch
  )
{
  UINT8              Dimm;
  UINT8              NumBankBits;
  UINT8              NumBanks;
  UINT8              MaxNumBanks;
  UINT8              NumBanksEncoded = 0;
  struct dimmNvram   (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  MaxNumBanks = 0;
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((IsDimmPresent (Socket, Ch, Dimm)) && (!(IsDcpmmPresentDimm (Socket, Ch, Dimm)))) {
       NumBankBits = (*DimmNvList)[Dimm].numBankGroupBits + (*DimmNvList)[Dimm].numBankAddressBits;
       NumBanks = 1 << NumBankBits;
       if (NumBanks > MaxNumBanks) {
         MaxNumBanks = NumBanks;
       }
    }
  }

  //
  // Register encoding for Number of banks:
  // 0 = 8 banks
  // 1 = 16 banks
  // 2 = 32 banks
  // 3 = reserved (RFU)
  //
  switch (MaxNumBanks) {
    case 8:
      NumBanksEncoded = 0;
      break;
    case 16:
      NumBanksEncoded = 1;
      break;
    case 32:
      NumBanksEncoded = 2;
      break;
    default:
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
         "Unsupported number of banks: %d. Defaulting to 8.\n", MaxNumBanks);
      NumBanksEncoded = 0;
      break;
  }
  return NumBanksEncoded;
}

/**
  Program B0 specific scheduler registers

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval N/A
**/
VOID
DdrSchedulerConfigB0Specific (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch
  )
{
  SCHEDULER_MM_CAS_BLOCK_MCDDC_CTL_STRUCT            SchedulerMmCas;
  SCHEDULER_MMA_PREFERRED_STARVE_MCDDC_CTL_STRUCT    SchedulerMmaPreferred;
  SCHEDULER_MM_MCDDC_CTL_STRUCT                      SchedulerMm;
  SCHEDULER_RMM_PREFERRED_DURATION_MCDDC_CTL_STRUCT  SchedulerRmmPreferred;

  if ((IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) || (IsCpuAndRevision (CPU_GNRSP, REV_ALL))) {
    SchedulerMmCas.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_MM_CAS_BLOCK_MCDDC_CTL_REG);
    SchedulerMmaPreferred.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_MMA_PREFERRED_STARVE_MCDDC_CTL_REG);
    SchedulerMm.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_MM_MCDDC_CTL_REG);
    SchedulerRmmPreferred.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_RMM_PREFERRED_DURATION_MCDDC_CTL_REG);

    SchedulerMmCas.Bits.wpq_major_cas_block_threshold = 0xF;
    SchedulerMmCas.Bits.rpq_major_cas_block_threshold = 0xF;

    SchedulerMmaPreferred.Bits.read_starve_sr_count = 0x100;
    SchedulerMmaPreferred.Bits.write_starve_sr_count = 0xfd0;

    SchedulerMm.Bits.pr_duration_en = 0x1;
    SchedulerMm.Bits.sr_rmm_exit_en = 0x1;
    SchedulerMm.Bits.pr_min_stay = 0x32;
    SchedulerMm.Bits.rmm_exit_threshold = 0x8;
    SchedulerMm.Bits.select_wpq_avail_occupancy = 0x0;
    SchedulerMm.Bits.select_rpq_avail_occupancy = 0x0;

    SchedulerRmmPreferred.Bits.pr_duration_pw = 0x200;
    SchedulerRmmPreferred.Bits.pr_duration_sr = 0x1CE;

    MemWritePciCfgEp (Socket, Ch, SCHEDULER_MM_CAS_BLOCK_MCDDC_CTL_REG, SchedulerMmCas.Data);
    MemWritePciCfgEp (Socket, Ch, SCHEDULER_MMA_PREFERRED_STARVE_MCDDC_CTL_REG, SchedulerMmaPreferred.Data);
    MemWritePciCfgEp (Socket, Ch, SCHEDULER_MM_MCDDC_CTL_REG, SchedulerMm.Data);
    MemWritePciCfgEp (Socket, Ch, SCHEDULER_RMM_PREFERRED_DURATION_MCDDC_CTL_REG, SchedulerRmmPreferred.Data);
  }
}

/**
  Pre-training initialization of scheduler registers

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval N/A
**/
VOID
DdrSchedulerConfig (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch
  )
{
  SCHEDULER_ENABLES_BS_MCDDC_CTL_STRUCT     SchedulerEnablesBs;
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT        SchedulerEnables;
  SCHEDULER_PENDINGQ_MCDDC_CTL_STRUCT       PendingQ;
  SCHEDULER_RID_ENTRY_MCDDC_CTL_STRUCT      RidEntry;
  SCHEDULER_QOS_LOW_MCDDC_CTL_STRUCT        SchedulerQosLow;
  SCHEDULER_QOS_MED_MCDDC_CTL_STRUCT        SchedulerQosMed;
  SCHEDULER_QOS_HIGH_MCDDC_CTL_STRUCT       SchedulerQosHigh;
  UINT8                                     BankSchedulerSelection;
  UINT8                                     PageTableIndexSelection;

  SchedulerEnablesBs.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
  SchedulerEnables.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG);
  PendingQ.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_PENDINGQ_MCDDC_CTL_REG);
  RidEntry.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_RID_ENTRY_MCDDC_CTL_REG);
  SchedulerQosLow.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_QOS_LOW_MCDDC_CTL_REG);
  SchedulerQosMed.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_QOS_MED_MCDDC_CTL_REG);
  SchedulerQosHigh.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_QOS_HIGH_MCDDC_CTL_REG);

  // Scheduler Enables
  SchedulerEnables.Bits.rpq_vc1_llt_en            = 0;
  SchedulerEnablesBs.Bits.link_major_mode_en      = 1;
  SchedulerEnablesBs.Bits.number_of_banks         = NumBanksLookup (Host, Socket, Ch);
  SchedulerEnablesBs.Bits.idle_bypass_en          = 0;
  SchedulerEnables.Bits.idle_bypass_en            = 0;

  SchedulerEnables.Bits.xpq_llt_en                = 1;
  if (!IsDdr5MemSsPresent (Host, Socket)) {
    SchedulerEnables.Bits.pseudo_channel_selection  = 1;
  }

  SchedulerEnables.Bits.bank_scheduler_selection   = 0;
  SchedulerEnables.Bits.page_table_index_selection = 0;
  if (SUCCESS == DdrSchedulerSelectLookup (Host, Socket, Ch, &BankSchedulerSelection, &PageTableIndexSelection)) {
    SchedulerEnables.Bits.bank_scheduler_selection   = BankSchedulerSelection;
    SchedulerEnables.Bits.page_table_index_selection = PageTableIndexSelection;
  }

  // Scheduler Pending Queue
  if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
    PendingQ.Bits.wpq_link_list_threshold   = LINK_LIST_THRESHOLD_B0;
    PendingQ.Bits.rpq_link_list_threshold   = LINK_LIST_THRESHOLD_B0;
  } else {
    PendingQ.Bits.wpq_link_list_threshold   = LINK_LIST_THRESHOLD;
    PendingQ.Bits.rpq_link_list_threshold   = LINK_LIST_THRESHOLD;
  }
  PendingQ.Bits.wpq_empty_llt_priority_en = 0;
  PendingQ.Bits.rpq_empty_llt_priority_en = 0;
  PendingQ.Bits.wpq_priority_en           = 1;
  PendingQ.Bits.rpq_priority_en           = 1;

  // Scheduler RID entry
  RidEntry.Bits.enable_rid_loopback = 0;
  RidEntry.Bits.rid_loopback_entry = 0x13;
  RidEntry.Bits.rid_underfill_entry = 0x17;
  RidEntry.Bits.rid_vc0_entry = 0x14;
  RidEntry.Bits.rid_vc1_entry = 0x15;
  RidEntry.Bits.rid_vc2_entry = 0x15;
  RidEntry.Bits.rid_vc3_entry = 0x16;

  ConfigureRidVcbeforeTraining (Socket, Ch);

  // Scheduler promotion LOW
  SchedulerQosLow.Bits.qos_lowenable = 1;
  SchedulerQosLow.Bits.qos_lowtimer_lim = FixedPcdGet16 (PcdMrcQosLowTimerLim);

  // Scheduler promotion MED
  SchedulerQosMed.Bits.qos_medenable = 1;
  SchedulerQosMed.Bits.qos_medtimer_lim = FixedPcdGet16 (PcdMrcQosMedTimerLim);

  // Scheduler promotion HIGH
  SchedulerQosHigh.Bits.qos_highenable = 1;
  SchedulerQosHigh.Bits.qos_hightimer_lim = FixedPcdGet16 (PcdMrcQosHighTimerLim);

  MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesBs.Data);
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_PENDINGQ_MCDDC_CTL_REG, PendingQ.Data);
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_RID_ENTRY_MCDDC_CTL_REG, RidEntry.Data);
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_QOS_LOW_MCDDC_CTL_REG, SchedulerQosLow.Data);
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_QOS_MED_MCDDC_CTL_REG, SchedulerQosMed.Data);
  MemWritePciCfgEp (Socket, Ch, SCHEDULER_QOS_HIGH_MCDDC_CTL_REG, SchedulerQosHigh.Data);

  //
  // B0 specific registers
  //
  DdrSchedulerConfigB0Specific (Host, Socket, Ch);
}

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
  )
{
  if (MemTechType == MemTechHbm) {
    *IdleBypassOffsets = &mHbmIdleBypassOffsets;
  } else {
    *IdleBypassOffsets = &mDdrIdleBypassOffsets;
  }

  return EFI_SUCCESS;
}

/**
  Return values for MC JEDEC parameter register offsets
  These are the offsets applied to JEDEC parameters before programming the register.

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket number
  @param[in]  MemTechType         - Memory Technology Type
  @param[out] JedecTimingOffsets  - Pointer to caller's buffer pointer
                                    Updated by this function to pointer to the idle bypass offsets table

  @retval EFI_SUCCESS if values are supported
          EFI_UNSUPPORTED otherwise
**/
EFI_STATUS
GetJedecTimingOffsets (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  MEM_TECH_TYPE         MemTechType,
  OUT JEDEC_TIMING_OFFSETS  **JedecTimingOffsets
  )
{

  if (MemTechType == MemTechHbm) {
    *JedecTimingOffsets = &mHbmJedecTimingOffsets;
  } else if (IsDdr5MemSsPresent (Host, Socket)) {
    *JedecTimingOffsets = &mDdr5JedecTimingOffsets;
  } else {
    *JedecTimingOffsets = &mDdr4JedecTimingOffsets;
  }

  return EFI_SUCCESS;
}

/**
  Return a bit mask of enabled ranks for the input socket/channel
  The mask assumes sub channel A ranks are the first 4 bits,
  and sub channel B ranks are the next 4 bits

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number

  @retval N/A
**/
UINT32
GetSubChRankMask (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch
  )
{
  UINT32                              RefMaskValue = 0;
  struct dimmNvram                    (*DimmNvList) [MAX_DIMM];
  UINT8                               Dimm;
  UINT8                               Rank;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      //sub channel A
      RefMaskValue |= ((1 << Rank) << (Dimm * 2));
      //sub channel B
      RefMaskValue |= (((1 << Rank) << (Dimm * 2)) << 4) ;
    } //Rank
  } //Dimm
  return RefMaskValue;
} //GetSubChRankMask

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
  )
{
  MEMORY_TIMINGS_BANK_TCL_TWL_MCDDC_CTL_STRUCT          MtTclTwl;
  MEMORY_TIMINGS_BANK_TRTP_TWR_MCDDC_CTL_STRUCT         MtTrtpTwr;
  MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_STRUCT        MtTrrdTfaw;
  MEMORY_TIMINGS_BANK_TRP_TRC_TRAS_MCDDC_CTL_STRUCT     MtTrpTrcTras;
  MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_STRUCT             MtTrcdRW;
  TCKE_MCDDC_CTL_STRUCT                                 TCke;
  MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_STRUCT MtTrdaTwraTwrpre;
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT                   MtAdj;
  MEMORY_TIMINGS_ADJ2_MCDDC_CTL_STRUCT                  MtAdj2;
  JEDEC_TIMING_OFFSETS                                  *JedecTimingOffsets;
  struct channelNvram                                   (*ChannelNvList)[MAX_CH];
  UINT8                                                 MaxChDdr;
  UINT8                                                 LocalCL;
  UINT8                                                 LocalCWL;
  BootMode                                              SysBootMode;

  MaxChDdr = GetMaxChDdr ();
  if (Ch >= MaxChDdr) {
    // invalid ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_121);
    return;
  }
  SysBootMode = GetSysBootMode ();

  GetJedecTimingOffsets (Host, Socket, MemTechDdr, &JedecTimingOffsets);

  ChannelNvList = GetChannelNvList (Host, Socket);

  MtTclTwl.Data         = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TCL_TWL_MCDDC_CTL_REG);
  MtTrcdRW.Data         = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_REG);
  MtTrpTrcTras.Data     = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRP_TRC_TRAS_MCDDC_CTL_REG);
  TCke.Data             = MemReadPciCfgEp (Socket, Ch, TCKE_MCDDC_CTL_REG);
  MtTrrdTfaw.Data       = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_REG);
  MtTrtpTwr.Data        = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRTP_TWR_MCDDC_CTL_REG);
  MtTrdaTwraTwrpre.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_REG);
  MtAdj.Data            = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
  MtAdj2.Data           = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ2_MCDDC_CTL_REG);

  MtTrcdRW.Bits.t_rcd_rd          = GettRCD (Host, Socket, Ch);
  MtTrcdRW.Bits.t_rcd_wr          = GettRCD (Host, Socket, Ch);

  MtTrrdTfaw.Bits.t_rrd_l         = GettRRD_L (Host, Socket, Ch) - JedecTimingOffsets->t_rrd_l;
  MtTrrdTfaw.Bits.t_rrd_s         = GettRRD (Host, Socket, Ch) - JedecTimingOffsets->t_rrd_s;
  if (IsDcpmmPresentChannel (Socket, Ch) && ((*ChannelNvList)[Ch].maxDimm == 1)) {
    MtTrrdTfaw.Bits.t_faw         = DEFAULT_DDRT2_SAME_RANK_tFAW; // 4 Activate Window
    GetDdrtCasLatencies (Socket, Ch, 0, &LocalCL, NULL, &LocalCWL, NULL);
  } else {
    MtTrrdTfaw.Bits.t_faw         = (*ChannelNvList)[Ch].common.nFAW - JedecTimingOffsets->t_faw;
    LocalCL                       = (UINT8) GettCL (Host, Socket, Ch);
    LocalCWL                      = (UINT8) GettCWL (Host, Socket, Ch);
  }
  MtTclTwl.Bits.t_cl              = LocalCL;
  MtTclTwl.Bits.t_wl              = LocalCWL;

  MtTrtpTwr.Bits.t_rtp            = (*ChannelNvList)[Ch].common.nRTP;

  MtTrpTrcTras.Bits.t_rp          = GettRP (Host, Socket, Ch);
  MtTrpTrcTras.Bits.t_rc          = GettRC (Host, Socket, Ch);
  MtTrpTrcTras.Bits.t_ras         = GettRAS (Host, Socket, Ch);

  TCke.Bits.t_prpden              = GettPRPDEN (Host, Socket, Ch);
  TCke.Bits.t_cke                 = GettCKE (Host, Socket, Ch);
  TCke.Bits.t_xp                  = GettXPDdr5 (Host, Socket, Ch);
  TCke.Bits.t_rdpden              = GettRDPDEN (Host, Socket, Ch);
  TCke.Bits.t_wrpden              = GettWRPDEN (Host, Socket, Ch);

  MtTrdaTwraTwrpre.Bits.t_rda     = GettRDA (Host, Socket, Ch);
  MtTrdaTwraTwrpre.Bits.t_wra     = GettWRA (Host, Socket, Ch);
  MtTrdaTwraTwrpre.Bits.t_wrpre   = GettWRPRE (Host, Socket, Ch);
  MtTrdaTwraTwrpre.Bits.t_wr_rda  = GettWRRDA (Host, Socket, Ch);

  MtAdj.Bits.tcped2srx            = GettCKSRX2SRX (Socket); //Value for this field is the same as srx2srx for this SOC
  MtAdj.Bits.tcssr                = GettCSSR (Socket);

  MtAdj2.Bits.tsrx2srx            = GettCKSRX2SRX (Socket);
  MtAdj2.Bits.tstab               = GettSTAB (Host, Socket, Ch) + GettCKACT (Socket);
  if (SysBootMode == S3Resume) {
    //for S3 resume, the package has lost the information about what ranks are present
    //this programming will restore that information to the package so that it can issue
    //the proper commands for SRX.
    MtAdj2.Bits.force_sr_rank_sel = GetSubChRankMask (Host, Socket, Ch);
  }

  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TCL_TWL_MCDDC_CTL_REG, MtTclTwl.Data);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_REG, MtTrcdRW.Data);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRP_TRC_TRAS_MCDDC_CTL_REG, MtTrpTrcTras.Data);
  (*ChannelNvList)[Ch].tCke = TCke.Data;
  MemWritePciCfgEp (Socket, Ch, TCKE_MCDDC_CTL_REG, TCke.Data);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_REG, MtTrrdTfaw.Data);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRTP_TWR_MCDDC_CTL_REG, MtTrtpTwr.Data);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_REG, MtTrdaTwraTwrpre.Data);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, MtAdj.Data);
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ2_MCDDC_CTL_REG, MtAdj2.Data);

  ProgramJedecTimingsDdrio (Host, Socket, Ch, NULL);
}

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
  )
{
  if (mMemTimingLimits == NULL) {
    return EFI_NOT_FOUND;
  } else {
    *TableLength = sizeof (mMemTimingLimits) / sizeof (mMemTimingLimits[0]);
    *MemTimingLimitsTable = mMemTimingLimits;
    return EFI_SUCCESS;
  }
}

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
  )
{
  PSYSHOST                                    Host;
  SCHEDULER_IDLETIME2_MCDDC_CTL_STRUCT        IdleTime2;

  Host = GetSysHostPointer ();

  IdleTime2.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_IDLETIME2_MCDDC_CTL_REG);

  IdleTime2.Bits.idle_page_rst_val = IDLE_PAGE_RST_VAL;

  MemWritePciCfgEp (Socket, Ch, SCHEDULER_IDLETIME2_MCDDC_CTL_REG, IdleTime2.Data);
}

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
  )
{
  UINT32 EnterThreshold;
  UINT32 ExitThreshold;
  SCHEDULER_WMM_MCDDC_CTL_STRUCT          schedulerWmm;
  SYS_SETUP                                   *Setup;

  Setup = GetSysSetupPointer ();

  schedulerWmm.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_WMM_MCDDC_CTL_REG);

  schedulerWmm.Bits.wmm_read_hit_en              = 1;
  GetWmmThreshold (&EnterThreshold, &ExitThreshold);
  schedulerWmm.Bits.wmm_exit_threshold           = ExitThreshold;
  schedulerWmm.Bits.wmm_enter_threshold          = EnterThreshold;
  schedulerWmm.Bits.wmm_opp_timer                = WMM_OPP_TIMER;
  schedulerWmm.Bits.wmm_opp_read_limit           = WMM_OPP_READ_LIMIT;
  schedulerWmm.Bits.ignore_opp_rd_on_starved_wmm = 1;
  schedulerWmm.Bits.wmm_opp_timer_gran           = WMM_OPP_TIMER_GRAN;
  schedulerWmm.Bits.wmm_opp_read_limit_gran      = WMM_OPP_READ_LIMIT_GRAN;
  schedulerWmm.Bits.wpq_empty_opp_rd_en          = 0;
  schedulerWmm.Bits.wmm_leaky_bucket_en          = 0;

  MemWritePciCfgEp (socket, ch, SCHEDULER_WMM_MCDDC_CTL_REG, schedulerWmm.Data);
}

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
  )
{
  UINT32 ReadStarveCnt;
  UINT32 ReadWaitCnt;
  SCHEDULER_CMD_STARVATION_MCDDC_CTL_STRUCT   schedulerCasStarve;
  SCHEDULER_READ_STARVE_MCDDC_CTL_STRUCT      schedulerReadStarve;
  SCHEDULER_WRITE_STARVE_MCDDC_CTL_STRUCT     schedulerWriteStarve;

  schedulerCasStarve.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_CMD_STARVATION_MCDDC_CTL_REG);
  schedulerReadStarve.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_READ_STARVE_MCDDC_CTL_REG);
  schedulerWriteStarve.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_WRITE_STARVE_MCDDC_CTL_REG);

  schedulerCasStarve.Bits.starved_count = CAS_STARVE_COUNT;
  schedulerCasStarve.Bits.critical_count = CAS_CRITICAL_COUNT;
  schedulerCasStarve.Bits.granularity = 2;
  schedulerCasStarve.Bits.starved_en = 1;

  GetReadStarveAndWaitCount (&ReadStarveCnt, &ReadWaitCnt);
  schedulerReadStarve.Bits.read_wait_count      = ReadWaitCnt;
  schedulerReadStarve.Bits.read_starve_count    = ReadStarveCnt;

  schedulerWriteStarve.Bits.write_wait_count    = WRITE_WAIT_COUNT;
  if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
    schedulerWriteStarve.Bits.write_starve_count  = WRITE_STARVE_COUNT_B0;
  } else {
    schedulerWriteStarve.Bits.write_starve_count  = WRITE_STARVE_COUNT;
  }

  MemWritePciCfgEp (socket, ch, SCHEDULER_CMD_STARVATION_MCDDC_CTL_REG, schedulerCasStarve.Data);
  MemWritePciCfgEp (socket, ch, SCHEDULER_READ_STARVE_MCDDC_CTL_REG, schedulerReadStarve.Data);
  MemWritePciCfgEp (socket, ch, SCHEDULER_WRITE_STARVE_MCDDC_CTL_REG, schedulerWriteStarve.Data);
}

/**

  GetSetDcpmmTurnaroundSettings
  If Mode is GSM_READ_ONLY on input, read DCPMM turnaround related CSRs,
  adjust the data to be non SOC specific, and return
  If the Mode is GSM_FORCE_WRITE, adjust the data in a turnaround structure
  as needed for a given SOC, and write the CSRs
  Other input is not supported.
  Other Mode input settings are not supported by this function

  @param[in]       Socket                        - Socket number
  @param[in]       Channel                       - Channel number
  @param[in]       Mode                          - Get/Set mode flags
  @param[in, out]  DcpmmTurnaroundSettingsTable  - Pointer to DCPMM Turnaround settings

  @retval EFI_SUCCESS   Turnaround CSRs read/write successful

**/
EFI_STATUS
GetSetDcpmmTurnaroundSettings (
  IN     UINT8   Socket,
  IN     UINT8   Channel,
  IN     UINT8   Mode,
  IN OUT UINT32  DcpmmTurnaroundSettingsTable[]
  )
{
  PSYSHOST                                   Host;
  EFI_STATUS                                 ReturnStatus = EFI_SUCCESS;
  T_DDRT_RD_CNSTRNT_MC_2LM_STRUCT            DdrtRdCnstrnt = {0};
  T_DDRT_WR_CNSTRNT_MC_2LM_STRUCT            DdrtWrCnstrnt = {0};
  T_DDRT_GNT2RW_CNSTRNT_MC_2LM_STRUCT        DdrtGnt2RwCnstrnt = {0};
  T_DDRT_RW2GNT_CNSTRNT_MC_2LM_STRUCT        DdrtRw2GntCnstrnt = {0};
  T_DDRT_GNT2GNT_CNSTRNT_MC_2LM_STRUCT       DdrtGnt2GntCnstrnt = {0};


  Host = (PSYSHOST) GetSysHostPointer ();

  if ((GsmReadOnly (Mode)) || (GsmForceWrite (Mode))) {
    ReturnStatus = EFI_SUCCESS;
  } else {
    ASSERT (FALSE);
    ReturnStatus = EFI_INVALID_PARAMETER;
  }
  if (DcpmmTurnaroundSettingsTable == NULL) {
    ASSERT (FALSE);
    ReturnStatus = EFI_INVALID_PARAMETER;
  }
  if (ReturnStatus == EFI_SUCCESS) {
    if (GsmReadOnly (Mode)) {
      DdrtRdCnstrnt.Data = MemReadPciCfgEp (Socket, Channel, T_DDRT_RD_CNSTRNT_MC_2LM_REG);
      DdrtWrCnstrnt.Data = MemReadPciCfgEp (Socket, Channel, T_DDRT_WR_CNSTRNT_MC_2LM_REG);
      DdrtGnt2RwCnstrnt.Data = MemReadPciCfgEp (Socket, Channel, T_DDRT_GNT2RW_CNSTRNT_MC_2LM_REG);
      DdrtRw2GntCnstrnt.Data = MemReadPciCfgEp (Socket, Channel, T_DDRT_RW2GNT_CNSTRNT_MC_2LM_REG);
      DdrtGnt2GntCnstrnt.Data = (UINT16) MemReadPciCfgEp (Socket, Channel, T_DDRT_GNT2GNT_CNSTRNT_MC_2LM_REG);

      DcpmmTurnaroundSettingsTable[t_RDRD_DD]   = DdrtRdCnstrnt.Bits.t_ddrt_rdrd_dd;
      DcpmmTurnaroundSettingsTable[t_WRRD_DD]   = DdrtRdCnstrnt.Bits.t_ddrt_wrrd_dd;
      DcpmmTurnaroundSettingsTable[t_RDRD_S]    = DdrtRdCnstrnt.Bits.t_ddrt_rdrd_s;
      DcpmmTurnaroundSettingsTable[t_WRRD_S]    = DdrtRdCnstrnt.Bits.t_ddrt_wrrd_s;
      DcpmmTurnaroundSettingsTable[t_RDWR_DD]   = DdrtWrCnstrnt.Bits.t_ddrt_rdwr_dd;
      DcpmmTurnaroundSettingsTable[t_RDWR_S]    = DdrtWrCnstrnt.Bits.t_ddrt_rdwr_s;
      DcpmmTurnaroundSettingsTable[t_WRWR_DD]   = DdrtWrCnstrnt.Bits.t_ddrt_wrwr_dd;
      DcpmmTurnaroundSettingsTable[t_WRWR_S]    = DdrtWrCnstrnt.Bits.t_ddrt_wrwr_s;
      DcpmmTurnaroundSettingsTable[t_GNTRD_DD]  = DdrtGnt2RwCnstrnt.Bits.t_ddrt_gntrd_dd;
      DcpmmTurnaroundSettingsTable[t_GNTRD_S]   = DdrtGnt2RwCnstrnt.Bits.t_ddrt_gntrd_s;
      DcpmmTurnaroundSettingsTable[t_GNTWR_DD]  = DdrtGnt2RwCnstrnt.Bits.t_ddrt_gntwr_dd;
      DcpmmTurnaroundSettingsTable[t_GNTWR_S]   = DdrtGnt2RwCnstrnt.Bits.t_ddrt_gntwr_s;
      DcpmmTurnaroundSettingsTable[t_RDGNT_S]   = DdrtRw2GntCnstrnt.Bits.t_ddrt_rdgnt_s;
      DcpmmTurnaroundSettingsTable[t_WRGNT_DD]  = DdrtRw2GntCnstrnt.Bits.t_ddrt_wrgnt_dd;
      DcpmmTurnaroundSettingsTable[t_WRGNT_S]   = DdrtRw2GntCnstrnt.Bits.t_ddrt_wrgnt_s;
      DcpmmTurnaroundSettingsTable[t_GNTGNT_DD] = DdrtGnt2GntCnstrnt.Bits.t_ddrt_gntgnt_dd;
      DcpmmTurnaroundSettingsTable[t_GNTGNT_S]  = DdrtGnt2GntCnstrnt.Bits.t_ddrt_gntgnt_s;
    } else {

      DdrtRdCnstrnt.Bits.t_ddrt_rdrd_dd        = DcpmmTurnaroundSettingsTable[t_RDRD_DD];
      DdrtRdCnstrnt.Bits.t_ddrt_wrrd_dd        = DcpmmTurnaroundSettingsTable[t_WRRD_DD];
      DdrtRdCnstrnt.Bits.t_ddrt_rdrd_s         = DcpmmTurnaroundSettingsTable[t_RDRD_S];
      DdrtRdCnstrnt.Bits.t_ddrt_wrrd_s         = DcpmmTurnaroundSettingsTable[t_WRRD_S];
      DdrtWrCnstrnt.Bits.t_ddrt_rdwr_dd        = DcpmmTurnaroundSettingsTable[t_RDWR_DD];
      DdrtWrCnstrnt.Bits.t_ddrt_rdwr_s         = DcpmmTurnaroundSettingsTable[t_RDWR_S];
      DdrtWrCnstrnt.Bits.t_ddrt_wrwr_dd        = DcpmmTurnaroundSettingsTable[t_WRWR_DD];
      DdrtWrCnstrnt.Bits.t_ddrt_wrwr_s         = DcpmmTurnaroundSettingsTable[t_WRWR_S];
      DdrtGnt2RwCnstrnt.Bits.t_ddrt_gntrd_dd   = DcpmmTurnaroundSettingsTable[t_GNTRD_DD];
      DdrtGnt2RwCnstrnt.Bits.t_ddrt_gntrd_s    = DcpmmTurnaroundSettingsTable[t_GNTRD_S];
      DdrtGnt2RwCnstrnt.Bits.t_ddrt_gntwr_dd   = DcpmmTurnaroundSettingsTable[t_GNTWR_DD];
      DdrtGnt2RwCnstrnt.Bits.t_ddrt_gntwr_s    = DcpmmTurnaroundSettingsTable[t_GNTWR_S];
      DdrtRw2GntCnstrnt.Bits.t_ddrt_rdgnt_s    = DcpmmTurnaroundSettingsTable[t_RDGNT_S];
      DdrtRw2GntCnstrnt.Bits.t_ddrt_wrgnt_dd   = DcpmmTurnaroundSettingsTable[t_WRGNT_DD];
      DdrtRw2GntCnstrnt.Bits.t_ddrt_wrgnt_s    = DcpmmTurnaroundSettingsTable[t_WRGNT_S];
      DdrtGnt2GntCnstrnt.Bits.t_ddrt_gntgnt_dd = (UINT16) DcpmmTurnaroundSettingsTable[t_GNTGNT_DD];
      DdrtGnt2GntCnstrnt.Bits.t_ddrt_gntgnt_s  = (UINT16) DcpmmTurnaroundSettingsTable[t_GNTGNT_S];

      MemWritePciCfgEp (Socket, Channel, T_DDRT_RD_CNSTRNT_MC_2LM_REG, DdrtRdCnstrnt.Data);
      MemWritePciCfgEp (Socket, Channel, T_DDRT_WR_CNSTRNT_MC_2LM_REG, DdrtWrCnstrnt.Data);
      MemWritePciCfgEp (Socket, Channel, T_DDRT_GNT2RW_CNSTRNT_MC_2LM_REG, DdrtGnt2RwCnstrnt.Data);
      MemWritePciCfgEp (Socket, Channel, T_DDRT_RW2GNT_CNSTRNT_MC_2LM_REG, DdrtRw2GntCnstrnt.Data);
      MemWritePciCfgEp (Socket, Channel, T_DDRT_GNT2GNT_CNSTRNT_MC_2LM_REG, DdrtGnt2GntCnstrnt.Data);
    }
  }

  return ReturnStatus;
}

/**
  Program chip-specific ZQ calibration timing registers.

  @param[in]  Host        Pointer to sysHost
  @param[in]  Socket      Socket number
  @param[in]  Channel     Channel number
**/
VOID
InitZqCalTimingChip (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  )
{
  TCMRS_MCDDC_CTL_STRUCT    TcMrs;
  TCSRFTP_MCDDC_CTL_STRUCT  TcsrFtp;

  //
  // Not needed if no DDR5.
  //
  if (!IsDdr5Present (Host, Socket)) {
    return;
  }

  TcMrs.Data = MemReadPciCfgEp (Socket, Channel, TCMRS_MCDDC_CTL_REG);
  TcMrs.Bits.tzqcal_ddr5 = NanosecondToClocks (Socket, DDR5_TZQCAL_NS) / 2; // in hclks
  MemWritePciCfgEp (Socket, Channel, TCMRS_MCDDC_CTL_REG, TcMrs.Data);

  TcsrFtp.Data = MemReadPciCfgEp (Socket, Channel, TCSRFTP_MCDDC_CTL_REG);
  TcsrFtp.Bits.enzqcalerfsrx = 0;
  MemWritePciCfgEp (Socket, Channel, TCSRFTP_MCDDC_CTL_REG, TcsrFtp.Data);
}

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
  )
{
  TCRFP_MCDDC_CTL_STRUCT            Tcrfp;
  PSYSHOST                          Host;
  MCMNT_CHKN_BIT2_MCDDC_CTL_STRUCT  McMntChknBit2;

  Host = GetSysHostPointer ();

  //
  // Set Panic/High WM = 2/1 when 16Gb DIMM is present on channel
  // SPR will disable refresh watermark workaround before get official guidance
  // 1507319265 SPR: enable a refresh watermark workaround ref_panic_wm and ref_hi_wm for 16Gb DIMM depend on PEB forum response
  //
  Tcrfp.Data = MemReadPciCfgEp (Socket, Channel, TCRFP_MCDDC_CTL_REG);
  Tcrfp.Bits.orefni = OREFNI;

  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {

    //
    // set the mcmnt_chkn_bit2.dis_hpref_blk = 0x0 (which eliminates High Priority refresh)
    //
    McMntChknBit2.Data = MemReadPciCfgEp (Socket, Channel, MCMNT_CHKN_BIT2_MCDDC_CTL_REG);
    McMntChknBit2.Bits.dis_hpref_blk = 0;
    MemWritePciCfgEp (Socket, Channel, MCMNT_CHKN_BIT2_MCDDC_CTL_REG, McMntChknBit2.Data);

    //
    // 1. For DDR5, the panic refresh threshold for Normal Refresh (All Bank only) REFab is 5 and Fine Grain Refresh (All Bank or Same Bank) REFsb is 9.
    //    Design's recommendation is to set the panic watermark (tcrfp.ref_panic_wm) to max-2.
    // 2. Leave as default for ref_hi_wm
    //
    if (GetRefreshModePolicy () == RefabNormal) {
      Tcrfp.Bits.ref_panic_wm = REF_PANIC_WM_SPR_NORMAL;
    } else { // For RefabFineGrain and RefsbFineGrain
      Tcrfp.Bits.ref_panic_wm = REF_PANIC_WM_SPR_FINE_GRAIN;
    }

  } else {
    Tcrfp.Bits.ref_hi_wm = REF_HI_WM;
    Tcrfp.Bits.ref_panic_wm = REF_PANIC_WM;
  }

  MemWritePciCfgEp (Socket, Channel, TCRFP_MCDDC_CTL_REG, Tcrfp.Data);
}

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
  )
{
  PSYSHOST                              Host;
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT   MemoryTimingsAdj;

  Host = GetSysHostPointer ();

  MemoryTimingsAdj.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
  MemoryTimingsAdj.Bits.tppd_enable = 1;
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG , MemoryTimingsAdj.Data);
}

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
)
{
  //
  // Based on policy, use tRFC1, tRFC2 or tRFCsb from SPD
  // But SPR MC tRFC register programming requires that tRFCsb = tRFC2
  // So override the policy before reading from SPD
  //
  if (RefreshMode == RefsbFineGrain) {
    RefreshMode = RefabFineGrain;
  }
  return SpdGetTrfc (Socket, Ch, Dimm, RefreshMode, tRFC, nRFC);
}

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
  )
{
  UINT8                                 Dimm;
  UINT32                                tREFI;
  UINT32                                tRFCDclock;
  UINT32                                tRFCHclock;
  UINT16                                tRFCsbNanosecondTemp = 0;
  UINT16                                tRFCsbDclockTemp = 0;
  UINT16                                tRFCsbDclock = 0;
  UINT16                                tREFSBRDDclock = 0;
  UINT16                                tREFSBRDHclock = 0;
  UINT16                                tREFSBRDdlrDclock = 0;
  UINT16                                tREFSBRDdlrHclock = 0;
  UINT32                                tRPDclock;
  REFRESH_MODE                          RefreshMode;
  REFRESH_MODE                          RefreshModeOverride;
  TCRFTP_MCDDC_CTL_STRUCT               Tcrftp;
  TCSTAGGER_REF_MCDDC_CTL_STRUCT        Tcstagger;
  TCZQCAL_MCDDC_CTL_STRUCT              Tczqcal;
  TCMRS_MCDDC_CTL_STRUCT                TcMrs;
  RDIMMTIMINGCNTL2_MCDDC_CTL_STRUCT     RdimmTimingCntl2;
  DIMM_TEMP_EV_OFST_0_MCDDC_CTL_STRUCT  DimmTempEvOfst0;
  struct channelNvram                   (*ChannelNvList)[MAX_CH];
  struct dimmNvram                      (*DimmNvList)[MAX_DIMM];

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  RefreshMode = GetRefreshModePolicy ();

  //
  // Program TCZQCAL_MCDDC_CTL_REG
  //
  Tczqcal.Data = MemReadPciCfgEp (Socket, Ch, TCZQCAL_MCDDC_CTL_REG);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    SpdGetTrfc (Socket, Ch, Dimm, RefsbFineGrain, &tRFCsbNanosecondTemp, &tRFCsbDclockTemp);

    //
    // Update Host with largest tRFC in the channel
    //
    if (tRFCsbDclock < tRFCsbDclockTemp) {
      tRFCsbDclock = tRFCsbDclockTemp;
    }
  }

  Tczqcal.Bits.t_rfcpb     = tRFCsbDclock;

  tREFSBRDDclock = NanosecondToClocks (Socket, T_REFSBRD_NANOSECOND);
  tREFSBRDHclock = (tREFSBRDDclock + (DCLK_PER_HCLK - 1)) / DCLK_PER_HCLK;
  Tczqcal.Bits.t_rrefd     = tREFSBRDHclock; // This value is applicable for both non-3DS device refresh and 3DS device refresh in same logical bank.

  MemWritePciCfgEp (Socket, Ch, TCZQCAL_MCDDC_CTL_REG, Tczqcal.Data);

  //
  // Program TCMRS_MCDDC_CTL_REG
  //
  TcMrs.Data   = MemReadPciCfgEp (Socket, Ch, TCMRS_MCDDC_CTL_REG);

  tREFSBRDdlrDclock = NanosecondToClocks (Socket, T_REFSBRD_DLR_NANOSECOND);
  tREFSBRDdlrHclock = (tREFSBRDdlrDclock + (DCLK_PER_HCLK - 1)) / DCLK_PER_HCLK;
  TcMrs.Bits.trefsbrd_dlr  = tREFSBRDdlrHclock;

  MemWritePciCfgEp (Socket, Ch, TCMRS_MCDDC_CTL_REG, TcMrs.Data);

  //
  // Program TCRFTP_MCDDC_CTL_REG
  //
  if (GetTrefiOverride (&tREFI) != MRC_STATUS_SUCCESS) {

    InitializeRefreshRate (Host, Socket, Ch);
    RefreshModeOverride = RefreshMode;
    if (RefreshMode == RefsbFineGrain) {
      RefreshModeOverride = RefabFineGrain;
    }
    tREFI = GettREFIDdr5 (Host, Socket, Ch, RefreshModeOverride);
  }

  tRFCDclock = GettRFC (Host, Socket, Ch);
  //
  // SPR t_rfc is in HCLK
  // Round up
  //
  tRFCHclock =  (tRFCDclock + (DCLK_PER_HCLK - 1)) / DCLK_PER_HCLK;

  Tcrftp.Data = MemReadPciCfgEp (Socket, Ch, TCRFTP_MCDDC_CTL_REG);
  Tcrftp.Bits.t_refi      = tREFI;
  Tcrftp.Bits.t_rfc       = tRFCHclock;
  Tcrftp.Bits.t_ref_block = T_REF_BLOCK_DDR5;
  MemWritePciCfgEp (Socket, Ch, TCRFTP_MCDDC_CTL_REG, Tcrftp.Data);

  //
  // t_stagger_ref = tcrftp.t_rfc (equivalent to tRFC/2 since tcrftp.t_rfc is in HCLKS)
  //
  // t_slice_stagger = INT(2*(tcrftp.t_rfc/3) - tcrftp.t_ref_block - memory_timings_trp_trc_tras.t_rp - 22)
  //    NOTE 1: t_rfc register is in HCLK units, which is why it is multipled by 2 in the formula. Can use DCLK units and not multiply by 2.
  //    NOTE 2: memory_timings_trp_trc_tras.t_rp isn't programmed yet - use GettRP instead
  //
  tRPDclock = GettRP (Host, Socket, Ch);
  Tcstagger.Data    = MemReadPciCfgEp (Socket, Ch, TCSTAGGER_REF_MCDDC_CTL_REG);
  Tcstagger.Bits.t_stagger_ref = tRFCDclock/2;
  Tcstagger.Bits.t_slice_stagger = (UINT32) ((tRFCDclock + 2)/3 - T_REF_BLOCK_DDR5 - tRPDclock - 22); // The first item is corresponding to "tRFC_dlr = tRFC/3" which is defined in DDR5 JEDEC

  if (RefreshMode == RefsbFineGrain) {
    Tcstagger.Bits.perbank_mode = 1;
  } else {
    Tcstagger.Bits.perbank_mode = 0;
  }
  Tcstagger.Bits.no_bank_overlap = 0;
  Tcstagger.Bits.keep_cke_high_during_trfc = 0;
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "t_stagger_ref = 0x%x\n", Tcstagger.Bits.t_stagger_ref);
  MemWritePciCfgEp (Socket, Ch, TCSTAGGER_REF_MCDDC_CTL_REG, Tcstagger.Data);
  //
  // Program RDIMMTIMINGCNTL2_MCDDC_CTL_REG
  //
  RdimmTimingCntl2.Data = MemReadPciCfgEp (Socket, Ch, RDIMMTIMINGCNTL2_MCDDC_CTL_REG);
  RdimmTimingCntl2.Bits.t_bank_stagger = T_BANK_STAGGER_DCLOCK;
  MemWritePciCfgEp (Socket, Ch, RDIMMTIMINGCNTL2_MCDDC_CTL_REG, RdimmTimingCntl2.Data);

  if (IsSiliconWorkaroundEnabled ("S14010741732")) {
    DimmTempEvOfst0.Data = MemReadPciCfgEp (Socket, Ch, DIMM_TEMP_EV_OFST_0_MCDDC_CTL_REG);
    DimmTempEvOfst0.Bits.ev_2x_pb_ref_en = 0;
    MemWritePciCfgEp (Socket, Ch, DIMM_TEMP_EV_OFST_0_MCDDC_CTL_REG, DimmTempEvOfst0.Data);
  }
}

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
  )
{
  PSYSHOST                                                                    Host;
#ifdef GNRSRF_HOST
  SCHEDULER_BLOCKING_RULES_MCDDC_CTL_STRUCT                                   BlockingRules;
#else // GNRSRF_HOST
  SCHEDULER_BLOCKING_RULES_MCDDC_CTL_SPRB0_SPRHBM_SPRC0_SPRMCC_SPRUCC_STRUCT  BlockingRules;
#endif // GNRSRF_HOST

  Host = GetSysHostPointer ();

  BlockingRules.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_BLOCKING_RULES_MCDDC_CTL_REG);

  //
  // Scheduler Blocking Rules from Memory Performance CSR Settings 04/09/2020
  //
  if (IsCpuAndRevisionOrLater (CPU_SPRSP, REV_B0)) {
    BlockingRules.Bits.rpq_loaded_threshold                                         = 1;
    BlockingRules.Bits.noncrit_maj_write_cas_blck_noncrit_min_read_cas_threshold_en = 1;
    BlockingRules.Bits.noncrit_maj_write_act_blck_noncrit_min_read_act_threshold_en = 1;
    BlockingRules.Bits.noncrit_maj_act_blck_noncrit_min_act_sr_en                   = 1;
    BlockingRules.Bits.crit_min_blck_noncrit_maj_wmm_en                             = 0;
    BlockingRules.Bits.crit_min_blck_noncrit_maj_rmm_en                             = 0;
    BlockingRules.Bits.first_maj_cas_blck_noncrit_min_cas_en                        = 1;
    BlockingRules.Bits.first_maj_cas_blck_noncrit_min_act_en                        = 1;
  } else {
    BlockingRules.Bits.rpq_loaded_threshold                                         = 4;
    BlockingRules.Bits.noncrit_maj_write_cas_blck_noncrit_min_read_cas_threshold_en = 0;
    BlockingRules.Bits.noncrit_maj_write_act_blck_noncrit_min_read_act_threshold_en = 0;
    BlockingRules.Bits.noncrit_maj_act_blck_noncrit_min_act_sr_en                   = 0;
    BlockingRules.Bits.crit_min_blck_noncrit_maj_wmm_en                             = 1;
    BlockingRules.Bits.crit_min_blck_noncrit_maj_rmm_en                             = 1;
  }
  BlockingRules.Bits.wpq_loaded_threshold                                         = 4;
  BlockingRules.Bits.maj_exp_act_blck_noncrit_impre_sp_en                         = 0;
  BlockingRules.Bits.noncrit_maj_read_cas_blck_noncrit_min_write_cas_threshold_en = 1;
  BlockingRules.Bits.noncrit_maj_read_act_blck_noncrit_min_write_act_threshold_en = 1;
  BlockingRules.Bits.crit_act_bclk_noncrit_act_sr_en                              = 1;
  BlockingRules.Bits.top_strv_act_blck_act_sr_en                                  = 1;
  BlockingRules.Bits.crit_maj_blck_crit_min_wmm_en                                = 1;
  BlockingRules.Bits.crit_maj_blck_crit_min_rmm_en                                = 1;
  BlockingRules.Bits.crit_maj_blck_noncrit_min_wmm_en                             = 1;
  BlockingRules.Bits.crit_maj_blck_noncrit_min_rmm_en                             = 1;
  BlockingRules.Bits.top_strv_blck_en                                             = 1;

  MemWritePciCfgEp (Socket, Ch, SCHEDULER_BLOCKING_RULES_MCDDC_CTL_REG, BlockingRules.Data);

  return MRC_STATUS_SUCCESS;
}

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
  )
{
  PSYSHOST            Host;
  UINT8               Ch;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT32              TurnaroundSettingsTable[TurnaroundTypeMax];
  UINT8               MaxChDdr;
  EFI_STATUS          Status;

  Host          = GetSysHostPointer ();
  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  Status        = EFI_SUCCESS;

  ZeroMem (TurnaroundSettingsTable, sizeof (TurnaroundSettingsTable));

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    Status = GetSetTurnaroundSettings (MemTechType, Socket, Ch, GSM_READ_CSR, TurnaroundSettingsTable);
    if (EFI_ERROR (Status)) {
      return MRC_STATUS_FAILURE;
    }
    TurnaroundSettingsTable[t_WWSG] += Offset;
    TurnaroundSettingsTable[t_WWSR] += Offset;
    TurnaroundSettingsTable[t_WWDR] += Offset;
    TurnaroundSettingsTable[t_WWDD] += Offset;
    TurnaroundSettingsTable[t_WWDS] += Offset;
    Status = GetSetTurnaroundSettings (MemTechType, Socket, Ch, GSM_FORCE_WRITE, TurnaroundSettingsTable);
    if (EFI_ERROR (Status)) {
      return MRC_STATUS_FAILURE;
    }
  } // Ch loop

  return MRC_STATUS_SUCCESS;
} // SetWriteToWriteTurnaroundTimingsByOffset
