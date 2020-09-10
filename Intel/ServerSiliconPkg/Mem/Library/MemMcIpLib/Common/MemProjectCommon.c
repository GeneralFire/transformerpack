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
#include <SysHostChip.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Chip/Include/FnvAccessCommon.h>
#include <UncoreCommonIncludes.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/MemFmcIpLib.h>
#include <SysRegs.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/BaseMemoryLib.h>
#include "Include/MemMcRegs.h"
#include <Library/SystemInfoLib.h>
#include <ScratchpadList.h>
#include <Library/KtiApi.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/Ms2IdiLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpTargetLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include "Include/MemMcIpLibInternal.h"
#include <Library/OdtTableLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/ProcMemInitTargetLib.h>
#include <Library/CteNetLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/MemRoutingLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/SiliconWorkaroundLib.h>
#include <MemProjectSpecific.h>
#include <Library/MemRcLib.h>
#include <Library/MemThermManageLib.h>
#include <Library/MemTurnaroundTimingLib.h>

#define EARLY_CTL_CLK_REC_EN_OFFSET_1TCLK       96
#define EARLY_CTL_CLK_REC_EN_OFFSET_2TCLK       32
#define EARLY_CTL_CLK_IO_LATENCY_OFFSET_1TCLK   2
#define EARLY_CTL_CLK_IO_LATENCY_OFFSET_2TCLK   0
#define RTL_Addition                            4
//
// Timings temporarily used during receive enable training to acheive
// back-to-back reads. These values are programmed directly into the CSRs
// (loop latency has already been taken into account).
//
#define B2B_T_RRD_S_VALUE_DDR4  2
#define B2B_T_RRSR_VALUE_DDR4   2
#define B2B_T_RRSG_VALUE_DDR4   6

#define B2B_T_RRD_S_VALUE_DDR5  6
#define B2B_T_RRD_L_VALUE_DDR5  6
#define B2B_T_RCD_RD_VALUE_DDR5 0x1B
#define B2B_T_RRSR_VALUE_DDR5   6
#define B2B_T_RRSG_VALUE_DDR5   6

#define DDR4_WRITE_LATENCY_MIN  0 // MIN(UINT8)
#define DDRT_WRITE_LATENCY_MIN  0 // MIN(UINT8)
#define DDRT_WRITE_LATENCY_MAX  0xFF // MAX(UINT8)

CONST UINT32 DdrtDimmBasicTimingAddresses[MAX_DIMM] = {
  T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG,
  T_DDRT_DIMM1_BASIC_TIMING_MC_2LM_REG
};

UINT32 rdOdtTableRegs[MAX_DIMM] = {
  RD_ODT_TBL0_MCDDC_CTL_REG,
  RD_ODT_TBL1_MCDDC_CTL_REG
};

UINT32 wrOdtTableRegs[MAX_DIMM] = {
  WR_ODT_TBL0_MCDDC_CTL_REG,
  WR_ODT_TBL1_MCDDC_CTL_REG
};

extern UINT8 DCLKs[MAX_SUP_FREQ];
extern UINT8 BCLKs[MAX_SUP_FREQ];
extern UINT8 BclkFreq[2];

// PRIVATE Prototypes
BOOLEAN DidChipClearAEPDimmParityErrors10nm(PSYSHOST Host, UINT8 socket, UINT8 ch, UINT8 dimm, PCHIP_CLEAR_PARITY_RESULTS_STRUCT chipClearParityResStruct);
VOID PerformChipSpecificNormalCMDTimingSetup10nm(PSYSHOST Host, UINT8 socket, UINT8 ch, PCHIP_SET_NORMAL_CMD_TIMING_STRUCT chipSetNormalCMDTimingStruct);
VOID ReceiveEnableInitChip10nm (PSYSHOST Host, UINT8 socket, UINT8 ch);

/**
  Determine if the current socket has any channel with 2 slots populated

  @param  Host    - Pointer to SysHost

  @retval   TRUE if 2 slots are populated
  @retval   FALSE otherwise

**/
BOOLEAN
Is2DimmPerChannelSocket (
  IN  PSYSHOST            Host
);


/**
  Determine if the current socket has any channel with 2 slots populated and Dcpmm is populated

  @param  Host    - Pointer to SysHost

  @retval   TRUE if 2 slots are populated and one is DCPMM
  @retval   FALSE otherwise

**/
BOOLEAN
Is2DimmDcpmmPerChannelSocket (
  IN  PSYSHOST            Host
);

/**
  Returns desired status for the PXC Training feature

  @param Host          - Pointer to sysHost
  @param Socket        - Socket Id

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
GetPxcEnableChip (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**
  Returns desired status for the DDJC Training feature

  @param Host          - Pointer to sysHost
  @param Socket        - Socket Id

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
GetDdjcEnableChip (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**
  Returns desired status for the DCA DFE Training feature

  @param[in] Host         - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket       - Socket number

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
GetDcaDfeEnableChip (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

/**
  Returns desired status for the LRDIMM DB DFE Training feature

  @param Host          - Pointer to sysHost
  @param Socket        - Socket Id

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
GetDataBufferDFEEnableChip (
  IN PSYSHOST Host,
  IN UINT8    Socket
  );

extern PatCadbProg0 PrechargePattern0[CADB_LINES];
extern PatCadbProg1 DeselectPattern1[CADB_LINES];
STATIC UINT32 NonStickyScratchPads[16] = {BIOSNONSTICKYSCRATCHPAD0_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD1_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD3_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD4_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD5_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD6_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD8_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD9_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD10_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD11_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD12_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG,
                                   BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG};

STATIC UINT32 StickyScratchPads[8] = {BIOSSCRATCHPAD0_UBOX_MISC_REG,
                               BIOSSCRATCHPAD1_UBOX_MISC_REG,
                               BIOSSCRATCHPAD2_UBOX_MISC_REG,
                               BIOSSCRATCHPAD3_UBOX_MISC_REG,
                               BIOSSCRATCHPAD4_UBOX_MISC_REG,
                               BIOSSCRATCHPAD5_UBOX_MISC_REG,
                               BIOSSCRATCHPAD6_UBOX_MISC_REG,
                               BIOSSCRATCHPAD7_UBOX_MISC_REG};

UINT8
GetCmdMarginsSweepErrorResFeedback (
  GSM_CSN Signal,
  UINT8   Dimm,
  INT16   PiIndex,
  UINT8   CurError
  )
{
  UINT8 ErrorResult = 0;

  GetCmdMarginsSweepErrorResFeedbackTarget (Signal, Dimm, PiIndex, CurError, &ErrorResult);

  return ErrorResult;

}

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
  )
{
  UINT8 ErrorResult = 0;

  GetCmdMarginsBacksideSweepErrorResFeedbackTarget (Signal, Dimm, PiIndex, CurError, &ErrorResult);

  return ErrorResult;
}

UINT32
GetStickyScratchpad (
  UINT8     socket,
  UINT8     scratchpad
  )
{
  if (IsHvmModeEnable ()) {
    return 0;
  }
  return UsraCsrRead (socket, 0, StickyScratchPads[scratchpad]);
} // GetStickyScratchpad

VOID
SetStickyScratchpad (
  UINT8     socket,
  UINT8     scratchpad,
  UINT32    data
  )
{
  UsraCsrWrite (socket, 0, StickyScratchPads[scratchpad], data);
} // SetStickyScratchpad

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
  )
{
  BIOS_SCRATCHPAD0_STRUCT   Sp0;

  if (0 < Data || Data > 1) {
    return MRC_STATUS_FAILURE;
  }

  Sp0.Data = GetStickyScratchpad (Socket, 0);
  Sp0.Bits.MultiThread = Data;
  SetStickyScratchpad (Socket, 0, Sp0.Data);
  return MRC_STATUS_SUCCESS;
}

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
  )
{
  BIOS_SCRATCHPAD0_STRUCT   Sp0;

  Sp0.Data = GetStickyScratchpad (Socket, 0);
  return ((UINT8)Sp0.Bits.WarmBoot);
}

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
  )
{
  BIOS_SCRATCHPAD0_STRUCT   Sp0;

  Sp0.Data = GetStickyScratchpad (Socket, 0);
  Sp0.Bits.WarmBoot = 0;
  SetStickyScratchpad (Socket, 0, Sp0.Data);
}

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
  UINT8       socket,
  UINT8       Data,
  UINT8       *BitState
  )
{
  BIOS_SCRATCHPAD7_STRUCT   Sp7;

  if (0 < Data || Data > 1) {
    return MRC_STATUS_FAILURE;
  }

  Sp7.Data = GetStickyScratchpad (socket, 7);
  *BitState = (UINT8)(Sp7.Bits.BistFrbEventLastBoot);
  if (Sp7.Bits.BistFrbEventLastBoot && Data == 0) {
    //
    // disable FRB indication for next boot
    //
    Sp7.Bits.BistFrbEventLastBoot = 0;
    SetStickyScratchpad (socket, 7, Sp7.Data);
  }//endif FRB error
  return MRC_STATUS_SUCCESS;
}

/**

  Read from a non-sticky scratch pad

  @param[in] Socket      -  Socket Id
  @param[in] Scratchpad  -  Scratchpad to write

  @retval Data read from the requested scratch pad

**/
UINT32
GetNonStickyScratchpad (
  IN UINT8  Socket,
  IN UINT8  Scratchpad
  )
{
  if (IsHvmModeEnable ()) {
    return 0;
  }
  return UsraCsrRead (Socket, 0, NonStickyScratchPads[Scratchpad]);
} // GetNonStickyScratchpad

/**

  Write to a non-sticky scratch pad

  @param[in] Socket      - Socket Id
  @param[in] Scratchpad  - Scratchpad to write
  @param[in] Data        - Data to be written

  @retval None

**/
VOID
SetNonStickyScratchpad (
  IN UINT8   Socket,
  IN UINT8   Scratchpad,
  IN UINT32  Data
  )
{
  if (IsHvmModeEnable ()) {
    return;
  }
  UsraCsrWrite (Socket, 0, NonStickyScratchPads[Scratchpad], Data);
} // SetNonStickyScratchpad

/**
  Save original values for t_rrd_s, t_rrsg and t_rrsr to caller's buffer.
  Programm those registers with values guaranteed to generate back-to-back reads.

  @param[in] Host           - Pointer to SysHost
  @param[in] Socket         - Socket number
  @param[in] Ch             - Channel number
  @param[out] ActToActSave  - Pointer to caller's ACT_TO_ACT_SAVE_STRUCT

  @retval   N/A
**/
VOID
SetActToActPeriod (
  IN PSYSHOST                Host,
  IN UINT8                   Socket,
  IN UINT8                   Ch,
  OUT ACT_TO_ACT_SAVE_STRUCT ActToActSave[MAX_CH]
  )
{
  MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_STRUCT  MtTrrdTfaw;
  MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_STRUCT      MtCas2CasSr;
  MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_STRUCT      MtCas2CasSg;
  MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_STRUCT       MtBankTrcd;

  UINT32                                          B2bTRrdSValue;
  UINT32                                          B2bTRrdLValue;
  UINT32                                          B2bTRrsgValue;
  UINT32                                          B2bTRrsrValue;
  UINT32                                          BankTrcd;

  MtTrrdTfaw.Data  = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_REG);
  MtCas2CasSg.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_REG);
  MtCas2CasSr.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_REG);

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    MtBankTrcd.Data  = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_REG);
  } else {
    MtBankTrcd.Data  = 0;
  }
  //
  // This function may be called multiple times for the same channel.
  // Ensure the original value is saved only on the first call.
  //
  if (ActToActSave[Ch].TRrdSOrg == 0) {
    ActToActSave[Ch].TRrdSOrg = MtTrrdTfaw.Bits.t_rrd_s;
  }
  if (ActToActSave[Ch].TRrdLOrg == 0) {
    ActToActSave[Ch].TRrdLOrg = MtTrrdTfaw.Bits.t_rrd_l;
  }
  if (ActToActSave[Ch].TRcdRdOrg == 0) {
    ActToActSave[Ch].TRcdRdOrg = MtBankTrcd.Bits.t_rcd_rd;
  }
  if (ActToActSave[Ch].TRrSgOrg == 0) {
    ActToActSave[Ch].TRrSgOrg = MtCas2CasSg.Bits.t_rrsg;
  }
  if (ActToActSave[Ch].TRrsrOrg == 0) {
    ActToActSave[Ch].TRrsrOrg = MtCas2CasSr.Bits.t_rrsr;
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {

    B2bTRrdLValue = B2B_T_RRD_L_VALUE_DDR5;
    BankTrcd      = B2B_T_RCD_RD_VALUE_DDR5;

  } else {

    B2bTRrdLValue = MtTrrdTfaw.Bits.t_rrd_l;
    BankTrcd      = MtBankTrcd.Bits.t_rcd_rd;

  }

  if (IsDdr5MemSsPresent (Host, Socket)) {
    B2bTRrdSValue = B2B_T_RRD_S_VALUE_DDR5;
    B2bTRrsgValue = B2B_T_RRSG_VALUE_DDR5;
    B2bTRrsrValue = B2B_T_RRSR_VALUE_DDR5;
  } else {
    B2bTRrdSValue = B2B_T_RRD_S_VALUE_DDR4;
    B2bTRrsgValue = B2B_T_RRSG_VALUE_DDR4;
    B2bTRrsrValue = B2B_T_RRSR_VALUE_DDR4;
  }

  if (MtTrrdTfaw.Bits.t_rrd_s != B2bTRrdSValue || MtTrrdTfaw.Bits.t_rrd_l != B2bTRrdLValue) {
    MtTrrdTfaw.Bits.t_rrd_s = B2bTRrdSValue;
    MtTrrdTfaw.Bits.t_rrd_l = B2bTRrdLValue;
    MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_REG, MtTrrdTfaw.Data);
  }
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    if (MtBankTrcd.Bits.t_rcd_rd != BankTrcd) {
      MtBankTrcd.Bits.t_rcd_rd = BankTrcd;
      MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_REG, MtBankTrcd.Data);
    }
  }
  if (MtCas2CasSg.Bits.t_rrsg != B2bTRrsgValue) {
    MtCas2CasSg.Bits.t_rrsg = B2bTRrsgValue;
    MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_REG, MtCas2CasSg.Data);
  }
  if (MtCas2CasSr.Bits.t_rrsr != B2bTRrsrValue) {
    MtCas2CasSr.Bits.t_rrsr = B2bTRrsrValue;
    MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_REG, MtCas2CasSr.Data);
  }
} // SetActToActPeriod

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
  IN UINT8           Socket,
  IN UINT8           Ch,
  IN ACT_TO_ACT_SAVE_STRUCT ActToActSave[MAX_CH]
  )
{
  PSYSHOST                                       Host;
  MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_STRUCT MtTrrdTfaw;
  MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_STRUCT     MtCas2CasSr;
  MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_STRUCT     MtCas2CasSg;
  MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_STRUCT      MtBankTrcd;

  Host = GetSysHostPointer ();

  MtTrrdTfaw.Data  = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_REG);
  MtCas2CasSg.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_REG);
  MtCas2CasSr.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_REG);
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    MtBankTrcd.Data  = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_REG);
  } else {
    MtBankTrcd.Data  = 0;
  }

  if (MtTrrdTfaw.Bits.t_rrd_s != ActToActSave[Ch].TRrdSOrg || MtTrrdTfaw.Bits.t_rrd_l != ActToActSave[Ch].TRrdLOrg) {
    MtTrrdTfaw.Bits.t_rrd_s = ActToActSave[Ch].TRrdSOrg;
    MtTrrdTfaw.Bits.t_rrd_l = ActToActSave[Ch].TRrdLOrg;
    MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_REG, MtTrrdTfaw.Data);
  }
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    if (MtBankTrcd.Bits.t_rcd_rd != ActToActSave[Ch].TRcdRdOrg) {
      MtBankTrcd.Bits.t_rcd_rd = ActToActSave[Ch].TRcdRdOrg;
      MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_REG, MtBankTrcd.Data);
    }
  }
  if (MtCas2CasSg.Bits.t_rrsg != ActToActSave[Ch].TRrSgOrg) {
    MtCas2CasSg.Bits.t_rrsg = ActToActSave[Ch].TRrSgOrg;
    MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SG_MCDDC_CTL_REG, MtCas2CasSg.Data);
  }
  if (MtCas2CasSr.Bits.t_rrsr != ActToActSave[Ch].TRrsrOrg) {
    MtCas2CasSr.Bits.t_rrsr = ActToActSave[Ch].TRrsrOrg;
    MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_SR_MCDDC_CTL_REG, MtCas2CasSr.Data);
  }
} // RestoreActToActPeriod

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
  )
{
  //
  // Stub function to satisfy API
  // IO Latency IO Comp implemented in DDRIO for this processor
  //
  RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_8);

}

VOID
SetIOLatencyComp(
  IN                PSYSHOST  Host,
  IN                UINT8     socket,
  IN                UINT8     ch,
  IN                UINT8     SubCh,
  IN                UINT8     rtError,
  IN ACT_TO_ACT_SAVE_STRUCT ActToActSave[MAX_CH]
)
{
  UINT8           ChannelLocal = 0;
  if (!rtError) {
    if (DdrtRoundTripIsNeededMc (socket, ch)) {
      SetIoLatencyIoComp (Host, socket, ch, SubCh, MRC_ROUND_TRIP_IO_COMPENSATION_DDRT);
    } else {
      SetIoLatencyIoComp (Host, socket, ch, SubCh, GetIoLatCompValueStart ());
    }
  } else {
    DisableChannelSw (Host, socket, ch);
  }
  ChannelLocal = ch;
  if ((IsBrsPresent(Host, socket)) && (SubCh != NO_SUBCH)) {
    GetChannelMapping(ch, SubCh, &ChannelLocal);
  }
  RestoreActToActPeriod(socket, ChannelLocal, ActToActSave);
} // SetIOLatencyComp


VOID
DecIoLatency(
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank,
  UINT8    *IoLatency,
  UINT16   centerPoint[MAX_CH][MAX_STROBE]
)
{
  // Do nothing. In the 10nm IP, there is a constraint that IoLatency must be odd, which means that IoLatency and the
  // strobes must be able to decrement by multiples of 2 QCLKs, or 128 PI ticks.  However, receive enable fine training
  // should not result in all strobes having receive enable delay values greater than 128 PI ticks, so there will never
  // be a condition for IoLatency to be decremented.  Note that previous generations did not have this constraint, so
  // decrementing by 1 QCLK, or 64 PI ticks, was both valid and possible.
} // DecIoLatency

VOID
SetupPiSettingsParams(
                 PSYSHOST Host,
                 UINT8    socket,
                 UINT8    step,
                 UINT16   *piSetting,
                 UINT16   *piStart,
                 UINT16   *NumberOfCycle,
                 struct TrainingResults  (**trainRes)[MAX_CH][MAX_STROBE]
)
{
  if (step == FPT_PI_WRITE_TYPE) {
    *piSetting     = WR_LVL_PI_START;
    *piStart       = WR_LVL_PI_START;
    *NumberOfCycle = WR_LVL_PI_START + WR_LVL_PI_RANGE;
#ifdef LRDIMM_SUPPORT
  } else if (step == FPT_PI_LRDIMM_READ_TYPE) {
    *piSetting     = LRDIMM_BACKSIDE_PI_START;
    *piStart       = LRDIMM_BACKSIDE_PI_START;
    *NumberOfCycle = LRDIMM_BACKSIDE_PI_START + LRDIMM_BACKSIDE_PI_RANGE;
    *trainRes       = &Host->var.mem.lrTrainRes;
  } else if (step == FPT_PI_LRDIMM_WRITE_TYPE) {
    *piSetting     = LRDIMM_BACKSIDE_PI_START;
    *piStart       = LRDIMM_BACKSIDE_PI_START;
    *NumberOfCycle = LRDIMM_BACKSIDE_PI_START + LRDIMM_BACKSIDE_PI_RANGE;
    *trainRes       = &Host->var.mem.lrDwlTrainRes;
  } else if (step == FPT_PI_LRDIMM_RD_MRD_TYPE) {
    *piSetting     = LRDIMM_BACKSIDE_PI_START;
    *piStart       = LRDIMM_BACKSIDE_PI_START;
    *NumberOfCycle = LRDIMM_BACKSIDE_PI_START + LRDIMM_BACKSIDE_PI_RANGE;
    *trainRes       = &Host->var.mem.lrMrdTrainRes;
  } else if (step == FPT_PI_LRDIMM_WR_MRD_TYPE) {
    *piSetting     = LRDIMM_BACKSIDE_PI_START;
    *piStart       = LRDIMM_BACKSIDE_PI_START;
    *NumberOfCycle = LRDIMM_BACKSIDE_PI_START + LRDIMM_BACKSIDE_PI_RANGE;
    *trainRes       = &Host->var.mem.lrMwdTrainRes;
#endif  //LRDIMM_SUPPORT
  } else {
    *piSetting     = REC_EN_PI_START;
    *piStart       = REC_EN_PI_START;
    *NumberOfCycle = REC_EN_PI_START + REC_EN_PI_RANGE;
  }
} // SetupPiSettingsParams

UINT8
GetTrainingResultIndex(
                       UINT8  step,
                       UINT16 piSetting,
                       UINT16 piStart
)
{
  UINT8 dWord;

  if (step == FPT_PI_WRITE_TYPE) {
    dWord = (UINT8)((piSetting - WR_LVL_PI_START) / 32);
#ifdef LRDIMM_SUPPORT
  } else if ((step == FPT_PI_LRDIMM_READ_TYPE) || (step == FPT_PI_LRDIMM_WRITE_TYPE)) {
    dWord = (UINT8)((piSetting - LRDIMM_BACKSIDE_PI_START) / 32);
#endif
  } else {
    dWord = (UINT8)((piSetting - REC_EN_PI_START) / 32);
  }

  return dWord;
} // GetTrainingResultIndex


VOID
RecEnTrainFeedback (
                    PSYSHOST Host,
                    UINT8    socket,
                    UINT8    ch,
                    UINT8    strobe,
                    UINT32   csrReg,
                    UINT16   piDelay
)
{
  UINT8  step;
  UINT8  dWord;
  UINT8  minGood;
  struct TrainingResults  (*trainRes)[MAX_CH][MAX_STROBE];

  trainRes = &Host->var.mem.trainRes;
  minGood  = SetMinGoodInit(Host);
  dWord    = (UINT8)((piDelay - REC_EN_PI_START) / 32);

  if ((csrReg & 0x1FF) >= minGood) {
    for (step = 0; step < REC_EN_STEP_SIZE; step++) {
      (*trainRes)[ch][strobe].results[dWord] = (*trainRes)[ch][strobe].results[dWord] | (1 << ((piDelay + step) % 32));
    } // step loop
  }

} // RecEnTrainFeedback

UINT16
StartBackOver (
                UINT8  step,
                UINT16 piStart
)
{
  UINT16 piSetting;

  if (step == FPT_PI_WRITE_TYPE) {
    piSetting = WR_LVL_PI_START;
  } else {
    piSetting = REC_EN_PI_START;
  }

  return piSetting;
} // StartBackOver

VOID
SetInitRoundTrip(
                 PSYSHOST Host,
                 UINT8    socket,
                 UINT8    ch,
                 UINT8    dimm,
                 UINT8    rank
)
{
#ifdef DDRT_SUPPORT
  UINT8                 Cl;
  UINT8                 ClAdder;
#endif // DDRT_SUPPORT
  UINT8                 rtDelay = 0;
  UINT8                 logRank;
  struct ddrRank        (*rankList)[MAX_RANK_DIMM];
  struct RankCh         (*rankPerCh)[MAX_RANK_CH];
  struct channelNvram   (*channelNvList)[MAX_CH];
  struct dimmNvram      (*dimmNvList)[MAX_DIMM];

  rankList = GetRankNvList(Host, socket, ch, dimm);
  channelNvList = GetChannelNvList(Host, socket);
  rankPerCh = &(*channelNvList)[ch].rankPerCh;
  dimmNvList = GetDimmNvList(Host, socket, ch);

  //
  // Get the logical rank #
  //
  logRank = GetLogicalRank(Host, socket, ch, dimm, rank);

  //
  // Default ROUNDT_LAT = Initial Round Trip Latency Offset +  GetRoundTripStepUnit () * tCL
  //

  if (!(*dimmNvList)[dimm].DcpmmPresent) {
    rtDelay = (UINT8) Host->var.mem.rtDefaultValue + GetRoundTripStepUnit () * ((*channelNvList)[ch].common.nCL);
  }
#ifdef DDRT_SUPPORT
  if ((*dimmNvList)[dimm].DcpmmPresent) {
    GetDdrtCasLatencies (socket, ch, dimm, &Cl, &ClAdder, NULL, NULL);
    if (DdrtRoundTripIsNeededMc (socket, ch)) {
      // Increase round trip delay by step unit to accommodate for the increase of IO_LAT_IO_COMP by step unit.
      rtDelay = (UINT8)(Host->var.mem.rtDefaultValue) + GetRoundTripStepUnit () * (Cl + ClAdder) + GetRoundTripStepUnit ();
    } else {
      rtDelay = (UINT8)(Host->var.mem.rtDefaultValue) + GetRoundTripStepUnit () * (Cl + ClAdder);
    }
  }
#endif // DDRT_SUPPORT

  if (rtDelay >= MEM_CHIP_POLICY_VALUE (Host, mrcRoundTripMax)) {
    rtDelay = MEM_CHIP_POLICY_VALUE (Host, mrcRoundTripMax);

    //
    // Avoid Roundtrip got even number
    //
    if (rtDelay % 2 == 0) {
      rtDelay -= 1;
    }
  }
  RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT, "\n Initial RT = %d\n", rtDelay);

  SetRoundTrip (socket, ch, logRank, rtDelay);

  SetIOLatency (socket, ch, logRank, MRC_IO_LATENCY_DEFAULT_VALUE);
} // SetInitRoundTrip

VOID
SetIoLatComp (
             PSYSHOST Host,
             UINT8    socket,
             UINT8    ch
)
{
  UINT8               SubCh;

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (DdrtRoundTripIsNeededMc (socket, ch)) {
      SetIoLatencyIoComp (Host, socket, ch, SubCh, MRC_ROUND_TRIP_IO_COMP_START_DDRT);
    } else {
      SetIoLatencyIoComp (Host, socket, ch, SubCh, MRC_ROUND_TRIP_IO_COMP_START);
    }
  }
} // SetIoLatComp

VOID
SetDisable2cycBypass (
                       PSYSHOST Host,
                       UINT8    socket,
                       UINT8    data
)
{
  //
  // Not applicable to the 10nm design
  //
} // Disable2cycBypass

//---------------------------------------------------------------
/**

  Program ODT Timing

  @param Host        - Pointer to sysHost
  @param socket        - Socket number
  @param ch          - Channel number (0-based)

  @retval N/A

**/
VOID
SetOdtTiming (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  UINT8  DimmPair;
  UINT8  DimmPairRdimm = 0;
  UINT8  Aep2dpc = 0;
  UINT8  Dimm;
  RD_ODT_TBL2_MCDDC_CTL_STRUCT rdOdtTiming;
  WR_ODT_TBL2_MCDDC_CTL_STRUCT wrOdtTiming;
  struct channelNvram          (*ChannelNvList)[MAX_CH];
  struct dimmNvram             (*DimmNvList)[MAX_DIMM];
  SYS_SETUP                     *Setup;

  Setup = GetSysSetupPointer ();

  ChannelNvList = GetChannelNvList(Host, socket);
  DimmNvList    = GetDimmNvList(Host, socket, ch);

  //
  // Write the Read and Write ODT Activates to the Matrix registers
  //
  rdOdtTiming.Data = MemReadPciCfgEp (socket, ch, RD_ODT_TBL2_MCDDC_CTL_REG);
  wrOdtTiming.Data = MemReadPciCfgEp (socket, ch, WR_ODT_TBL2_MCDDC_CTL_REG);

  rdOdtTiming.Bits.extraleadingodt = ODT_RD_CYCLE_LEADING;
  rdOdtTiming.Bits.extratrailingodt = ODT_RD_CYCLE_TRAILING;
  rdOdtTiming.Bits.read_delay_odt = ODT_RD_DELAY;

  if ((*ChannelNvList)[ch].maxDimm > 1) {
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }

      Aep2dpc = 1;
      DimmPair = (BIT0 ^ Dimm);
      if (!IsLrdimmPresent(socket, ch, DimmPair)) {
        DimmPairRdimm = 1;
      }
    }
  }

  wrOdtTiming.Bits.extra_leading_odt = ODT_WR_CYCLE_LEADING;

  if ((Aep2dpc & DimmPairRdimm) == 0) {
    if (Host->nvram.mem.socket[socket].channelList[ch].lrDimmPresent) {
      wrOdtTiming.Bits.extra_leading_odt = DDR4_LRDIMM_ODT_WR_CYCLE_LEADING;
      wrOdtTiming.Bits.extra_trailing_odt = DDR4_LRDIMM_ODT_WR_CYCLE_TRAILING;
    } else {
      wrOdtTiming.Bits.extra_trailing_odt = ODT_WR_CYCLE_TRAILING;
    }
  } else {
    wrOdtTiming.Bits.extra_trailing_odt = ODT_WR_CYCLE_TRAILING;
  }

  //If Read 2tCK Preamble Add 1 or 2 to ODT pulse width
  if(Host->var.mem.read2tckCL[ch] > 0){
    if ((Host->var.mem.read2tckCL[ch] + rdOdtTiming.Bits.extratrailingodt) < 4 ) {
      rdOdtTiming.Bits.extratrailingodt += Host->var.mem.read2tckCL[ch];
    } else {
      DisableChannelSw (Host, socket, ch);
      OutputWarning (WARN_MEMORY_TRAINING, WARN_ODT_TIMING_OVERFLOW, socket, ch, NO_DIMM, NO_RANK);
    }
  }
  //If Write 2tCK Preamble Add 1 or 2 to ODT pulse width
  if(Host->var.mem.write2tckCWL[ch] > 0){
    if ((Host->var.mem.write2tckCWL[ch] + wrOdtTiming.Bits.extra_trailing_odt) < 4 ){
      wrOdtTiming.Bits.extra_trailing_odt += Host->var.mem.write2tckCWL[ch];
    } else {
      DisableChannelSw (Host, socket, ch);
      OutputWarning (WARN_MEMORY_TRAINING, WARN_ODT_TIMING_OVERFLOW, socket, ch, NO_DIMM, NO_RANK);
    }
  }

  //For WRCRC
  if (Setup->mem.optionsExt & WR_CRC) {
    wrOdtTiming.Data = MemReadPciCfgEp (socket, ch, WR_ODT_TBL2_MCDDC_CTL_REG);
    if (wrOdtTiming.Bits.extra_trailing_odt < 3) {
      wrOdtTiming.Bits.extra_trailing_odt++;
    } else {
      DisableChannelSw (Host, socket, ch);
      OutputWarning (WARN_MEMORY_TRAINING, WARN_ODT_TIMING_OVERFLOW, socket, ch, NO_DIMM, NO_RANK);
    }
    MemWritePciCfgEp (socket, ch, WR_ODT_TBL2_MCDDC_CTL_REG, wrOdtTiming.Data);
  }

  //
  // Write the Read and Write ODT Activates to the Matrix registers
  //
  MemWritePciCfgEp (socket, ch, RD_ODT_TBL2_MCDDC_CTL_REG, rdOdtTiming.Data);
  MemWritePciCfgEp (socket, ch, WR_ODT_TBL2_MCDDC_CTL_REG, wrOdtTiming.Data);
} // SetOdtTiming

VOID
SetDdr3OdtMatrix (
                   PSYSHOST Host,
                   UINT8    socket,
                   UINT8    ch,
                   UINT8    dimm,
                   UINT8    rank
)
{
} // SetDdr3OdtMatrix

/**
  Write the Read and Write ODT Activates to the Matrix registers

  @param [IN] Socket         - Socket number
  @param [IN] Ch             - Channel number (0-based)
  @param [IN] Dimm           - DIMM number
  @param [IN] RdOdtTableData - Read ODT table data
  @param [IN] WrOdtTableData - Write ODT table data

  @retval None
*/
VOID
SetOdtActMatrixRegs (
  IN UINT8  Socket,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT32 RdOdtTableData,
  IN UINT32 WrOdtTableData
  )
{
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  //
  // Write the Read and Write ODT Activates to the Matrix registers
  //
  MemWritePciCfgEp (Socket, Ch, rdOdtTableRegs[Dimm], RdOdtTableData);
  MemWritePciCfgEp (Socket, Ch, wrOdtTableRegs[Dimm], WrOdtTableData);
} // OdtActMatrixRegs

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
  )
{
#ifndef GNRSRF_HOST
  T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_STRUCT DdrtDimmBasicTiming;
  struct dimmNvram                        (*DimmNvList)[MAX_DIMM];
  INT32                                   Gnt2Erid;
  MRC_STATUS                              MrcStatus = MRC_STATUS_SUCCESS;

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  // Explicitly check that the DIMM value is within the bounds of the arrays we use it to access.
  if (Dimm >= MAX_DIMM) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "ERROR: DIMM index (%d) is out of bounds (0 to %d).\n", Dimm, MAX_DIMM - 1);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_154);
  }

  // Read the current value from a) the register or b) the cache.
  if (GsmReadCsr (Mode)) {
    DdrtDimmBasicTiming.Data = MemReadPciCfgEp (Socket, Channel, DdrtDimmBasicTimingAddresses[Dimm]);
  } else {
    DdrtDimmBasicTiming.Data = (*DimmNvList)[Dimm].DdrtDimmBasicTiming;
  }
  Gnt2Erid = (INT32)DdrtDimmBasicTiming.Bits.t_ddrt_gnt2erid;

  // Set a new Grant-to-ERID value (if not read-only).
  if (!GsmReadOnly (Mode)) {

    // Get the new Grant-to-ERID value by a) offsetting the original value or b) directly applying the input value.
    if (GsmWriteOffset (Mode)) {
      Gnt2Erid += *Data;
    } else {
      Gnt2Erid = *Data;
    }

    // Check whether the new setting is out of range, enforcing limits if necessary.
    if ((Gnt2Erid < DDRT_GNT2ERID_MIN) ||
        (Gnt2Erid > DDRT_GNT2ERID_MAX))
    {
      RcDebugPrint (SDBG_MINMAX, "\n");
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "Warning: DDRT Grant-to-ERID (%d) is out of range (%d to %d). ",
                      Gnt2Erid, DDRT_GNT2ERID_MIN, DDRT_GNT2ERID_MAX);

      if (Gnt2Erid < DDRT_GNT2ERID_MIN) {
        Gnt2Erid = DDRT_GNT2ERID_MIN;
      } else { // Gnt2Erid > DDRT_GNT2ERID_MAX
        Gnt2Erid = DDRT_GNT2ERID_MAX;
      }

      RcDebugPrint (SDBG_MINMAX,
                      "Limiting to %d.\n\n", Gnt2Erid);

      MrcStatus = MRC_STATUS_GSM_LIMIT;

    } // if (Gnt2Erid < DDRT_GNT2ERID_MIN || Gnt2Erid > DDRT_GNT2ERID_MAX)

    // Write to the register if a write is being forced or the Grant-to-ERID value has changed.
    if (GsmForceWrite (Mode) ||
        (DdrtDimmBasicTiming.Bits.t_ddrt_gnt2erid != (UINT32)Gnt2Erid))
    {
      DdrtDimmBasicTiming.Bits.t_ddrt_gnt2erid = (UINT32)Gnt2Erid;
      MemWritePciCfgEp (Socket, Channel, DdrtDimmBasicTimingAddresses[Dimm], DdrtDimmBasicTiming.Data);
    }

  } // if (!GsmReadOnly)

  // Update cached timing value.
  if (GsmUpdateCache (Mode)) {
    (*DimmNvList)[Dimm].DdrtDimmBasicTiming = DdrtDimmBasicTiming.Data;
  }

  // Update the return value.
  *Data = (INT16)Gnt2Erid;

  return MrcStatus;

#else // !GNRSRF_HOST

  return MRC_STATUS_UNSUPPORTED;

#endif // !GNRSRF_HOST
} // GetSetSxpGnt2Erid

VOID
GetWpqRidtoRt10nm (
                 PSYSHOST Host,
                 UINT8    socket,
                 UINT8    ch,
                 UINT8    dimm
)
{
#ifdef DDRT_SUPPORT
  T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_STRUCT   ddrtDimm0BasicTiming;
  T_DDRT_DIMM1_BASIC_TIMING_MC_2LM_STRUCT   ddrtDimm1BasicTiming;
  T_DDRT_MISC_DELAY_MC_2LM_STRUCT           ddrtMiscDelay;
  INT16                                     Gnt2Erid;
  // ICX_TODO
  UINT8                                     roundtrip_rt_rank;

  ddrtMiscDelay.Data = MemReadPciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG);
  GetSetSxpGnt2Erid (Host, socket, ch, dimm, GSM_READ_ONLY, &Gnt2Erid);

  switch(dimm) {

    case 0:
      ddrtDimm0BasicTiming.Data = MemReadPciCfgEp (socket, ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG);
      // roundtrip0
      GetRoundTrip (socket, ch, 0, GSM_READ_CSR, &roundtrip_rt_rank);
      ddrtMiscDelay.Bits.wpq_rid_to_rt_ufill = (roundtrip_rt_rank >> 1) - 1  - Gnt2Erid - 5;
      MemWritePciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG, ddrtMiscDelay.Data);
      break;

    case 1:
      ddrtDimm1BasicTiming.Data = MemReadPciCfgEp (socket, ch, T_DDRT_DIMM1_BASIC_TIMING_MC_2LM_REG);
      // roundtrip1
      GetRoundTrip (socket, ch, MAX_RANK_DIMM, GSM_READ_CSR, &roundtrip_rt_rank);
      ddrtMiscDelay.Bits.wpq_rid_to_rt_ufill = (roundtrip_rt_rank >> 1) - 1 - Gnt2Erid - 5;
      MemWritePciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG, ddrtMiscDelay.Data);
      break;

    default:
      break;
  } // switch dimm
#endif // DDRT_SUPPORT
} // GetWpqRidtoRt10nm

VOID
CollectTestResultsErid (
                         PSYSHOST Host,
                         UINT8    socket,
                         UINT8    ch,
                         UINT8    dimm,
                         UINT16   offset,
                         UINT8    index,
                         UINT16   eridOffset,
                         UINT8    *gnt2eridPassed
)
{
#ifdef DDRT_SUPPORT
  // Collect test results
  UINT8 EridCombinedResult = 0;

  SetEridLfsrErrFromFeedbackTarget (Host, socket, ch, index, eridOffset);

  GetEridCombineResult (Host, socket, ch, &EridCombinedResult);

  if (EridCombinedResult) {
    // Mark passing channels
    *gnt2eridPassed = 1;
  } else {
    *gnt2eridPassed = 0;
  }
#endif // DDRT_SUPPORT
} // CollectTestResultsErid

//------------------------------------------------------------------------------------


VOID
GetChipTiPiSamplepiDelayStartandEnd(
  PSYSHOST Host,
  UINT8 socket,
  INT16* piDelayStart,
  UINT16* piDelayEnd
  )
{
  *piDelayEnd = WR_LVL_PI_START + WR_LVL_PI_RANGE;
  *piDelayStart = WR_LVL_PI_START;
}


/**

  Chip specific code path to determine if DDR4 LRDIM Host side training should be enabled. Disables legacy Host side training if it has been enabled

  @param Host  - Pointer to the system Host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param rank - Processor socket channel dimm rank number

  @retval BOOLEAN - true if the chip should disable DDR4 LRDIM Host side training

**/
BOOLEAN
ShouldChipEnableDDR4LRDIMMHostSideTraining(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank
  )
{
  if (IsLrdimmPresent (socket, ch, dimm)){
    return TRUE;
  }
  return FALSE;
}

/**

  Chip specific code path to support each platforms individual set of RTT_NOM values

  @param Host  - Pointer to the system Host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param rank - Processor socket channel dimm rank number

  @retval N/A

**/
VOID
SetChipRTTPark(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank
  )
{
#ifndef DDR5_SUPPORT
  UINT16              MR5;
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];
  struct ddrRank      (*rankList)[MAX_RANK_DIMM];

  rankList = GetRankNvList(Host, socket, ch, dimm);
  rankStruct  = GetRankStruct(Host, socket, ch, dimm);

  if ((Host->nvram.mem.dramType == SPD_TYPE_DDR4) && (*rankList)[rank].RttWr & DDR4_RTT_WR_ODT_MASK) {
    //
    // Get the current settings for MR5
    //
    MR5 = (*rankStruct)[rank].MR5;
    MR5 &= ~DDR4_RTT_PRK_ODT_MASK;
    // Set Rtt Park to the same value as Rtt Wr
    switch ((*rankList)[rank].RttWr >> DDR4_RTT_WR_ODT_SHIFT) {

    case DDR4_RTT_WR_80:
      MR5 |= (DDR4_RTT_NOM_80 << DDR4_RTT_PRK_ODT_SHIFT);
      break;

    case DDR4_RTT_WR_120:
      MR5 |= (DDR4_RTT_NOM_120 << DDR4_RTT_PRK_ODT_SHIFT);
      break;

    case DDR4_RTT_WR_240:
      MR5 |= (DDR4_RTT_NOM_240 << DDR4_RTT_PRK_ODT_SHIFT);
      break;

    case DDR4_RTT_WR_INF:
      MR5 |= (DDR4_RTT_NOM_INF << DDR4_RTT_PRK_ODT_SHIFT);
      break;

    default:
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_63);
      break;
    } // switch
    WriteMRS (Host, socket, ch, dimm, rank, MR5, BANK5);
  } // ddr4
#endif // !DDR5_SUPPORT
}

/**

  Chip specific code path to Disable any legacy LRDRIM WriteLeveling and return false, or return true if should disable DDR4 write leveling.

  @param Host  - Pointer to the system Host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param rank - Processor socket channel dimm rank number

  @retval BOOLEAN - true if the chip should disable DDR4 write leveling

**/
BOOLEAN
ShouldChipDisableDDR4LRDIMMWriteLeveling(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank
  )
{
#ifdef LRDIMM_SUPPORT
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];

  rankStruct  = GetRankStruct(Host, socket, ch, dimm);

  if (IsLrdimmPresent(socket, ch, dimm) &&
      ((*rankStruct)[rank].CurrentLrdimmTrainingMode != LRDIMM_DWL_TRAINING_MODE) &&
      (Host->var.mem.InPbaWaMode == 0)) {
    return TRUE;
  }
#endif // LRDIMM_SUPPORT

  return FALSE;
}

/**

  Chip specific code path.This chip only supports DDR4, so it has a different code path for DDR4 that requires RTT_PARK restored. DDR3 does not need this.

  @param Host  - Pointer to the system Host (root) structure
  @param socket  - processor socket ID
  @param ch  - Processor socket Channel number
  @param dimm  - Processor socket channel dimm number
  @param rank - Processor socket channel dimm rank number

  @retval BOOLEAN - true if the chip should disable DDR4 write leveling

**/
VOID
DoChipCompatibleRTT_PARKRestore(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 rank
  )
{
#ifndef DDR5_SUPPORT
  struct rankDevice   (*rankStruct)[MAX_RANK_DIMM];

  rankStruct  = GetRankStruct(Host, socket, ch, dimm);

  //
  // Restore RTT_PARK
  //
  WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
#endif // !DDR5_SUPPORT
}

/*++

Routine Description:

  Get's the results from the current test

Arguments:

  Host      - Pointer to sysHost
  dimm      - DIMM number
  rank      - rank number of the DIMM

Returns:

  N/A

--*/
VOID
GetResultsWL (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT16    piDelay
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT8               dWord;
  UINT8               step;
  UINT32              csrReg;
  UINT16                  piDelayStart;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct TrainingResults  (*trainRes)[MAX_CH][MAX_STROBE];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8                MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  trainRes      = &Host->var.mem.trainRes;
  channelNvList = GetChannelNvList(Host, socket);

  piDelayStart = WR_LVL_PI_START;

  dWord = (UINT8)((piDelay - piDelayStart) / 32);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList(Host, socket, ch);

    // Continue to the next rank if this one is disabled
    if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
      continue;
    }

    //SKX change for upper strobes in N1 registers
    for (strobe = 0; strobe < MSVx4; strobe++) {

      if (IsDimmStrobeNotValid (&(*dimmNvList)[dimm], strobe)) {
        continue;
      }

      csrReg = ReadTrainFeedback (Host, socket, ch, strobe);

      GetWlFineResultsFeedbackTarget (strobe, dWord, piDelay, &csrReg);

      if ((csrReg & 0x1FF) >= 16) {
        for (step = 0; step < WR_LVL_STEP_SIZE; step++) {
          (*trainRes)[ch][strobe].results[dWord] = (*trainRes)[ch][strobe].results[dWord] | (1 << ((piDelay + step) % 32));
        }
      }

    } // strobe loop
  } // ch loop
} // GetResultsWL

/**

  Update the Global Offsets

  @param Host            - Pointer to sysHost, the system Host (root) structure
  @param ch              - processor socket ID
  @param TargetOffset    - Target offset for byte lanes
  @param CRAddDelay      - Return value
  @param GlobalByteOff   - Return value

  @retval None

**/
VOID
UpdateGlobalOffsets (
  PSYSHOST  Host,
  UINT8     ch,
  INT16     TargetOffset,
  INT16     *CRAddDelay,
  INT16     *GlobalByteOff
  )
{
  // Calculate offsets
  *GlobalByteOff = 0;
  if (TargetOffset > 7) {
    *CRAddDelay = 7;
    *GlobalByteOff = 128 * (TargetOffset - 7);
  } else if (TargetOffset < -3) {
    *CRAddDelay = -3;
    *GlobalByteOff = 128 * (TargetOffset + 3);
  } else {
    *CRAddDelay = TargetOffset;
  }
} // UpdateGlobalOffsets

/**

  Chip specific code path to initialize "GlobalByteOff"

  @param Host  - Pointer to the system Host (root) structure
  @param socket  - processor socket ID
  @param CRAddDelay  -
  @param GlobalByteOff  -

  @retval - VOID

**/

VOID
SetChipDQDQSTiming(
  PSYSHOST                Host,
  UINT8                   socket,
  UINT8                   ch,
  INT16                   offsetValue,
  INT16                   *CRAddDelay,
  INT16                   *GlobalByteOff,
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  )
{
  *GlobalByteOff = 0;
  UpdateGlobalOffsets(Host, ch, offsetValue, CRAddDelay, GlobalByteOff);
}

VOID
WriteChipWrADDDelays(
  PSYSHOST                Host,
  UINT8                   socket,
  UINT8                   ch,
  UINT8                   SetType,
  INT16                   offsetValue,
  INT16*                  UCRAddDelay,
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  )
{
  SetCwlAdj (Host, socket, ch, SetType, UCRAddDelay[ch]);
}

/**

  Chip specific code path to getChipTimingData

  @param Host                - Pointer to the system Host (root) structure
  @param socket              - processor socket ID
  @param ch                  - Processor socket Channel number
  @param WLChipCleanUpStruct - Pointer to CHIP_WL_CLEANUP_STRUCT

  @retval - VOID

**/
VOID
GetChipTimingData(
  PSYSHOST                Host,
  UINT8                   socket,
  UINT8                   ch,
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  )
{
  //
  // API comply
  //
}

VOID
UpdateChipMCDelay(
  PSYSHOST                Host,
  UINT8                   socket,
  UINT8                   ch,
  UINT8                   Dimm,
  struct channelNvram     (*channelNvList)[MAX_CH],
  INT16                   *UCRAddDelay,
  PCHIP_WL_CLEANUP_STRUCT WLChipCleanUpStruct
  )
{
  if ((*channelNvList)[ch].dimmList[Dimm].DcpmmPresent) {
    SetCwlAdj (Host, socket, ch, CWLADJ_DDRT, UCRAddDelay[ch]);
  } else {
    SetCwlAdj (Host, socket, ch, CWLADJ_DDR4, UCRAddDelay[ch]);
  }
}

/**

  Exectues command/clock training

  @param Host      - Pointer to sysHost

  @retval N/A

**/
UINT32
ChipLateCmdClk(
          PSYSHOST  Host
          )
{
  UINT8                               socket;

  if (!IsMemFlowEnabled (LateCmdClkTraining)) {
    //
    // Indicate CMD training is done
    //
    Host->nvram.mem.socket[Host->var.mem.currentSocket].cmdClkTrainingDone = 1;
    return SUCCESS;
  }

  //
  // Return if this socket is disabled
  //
  socket = Host->var.mem.currentSocket;

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  Host->var.mem.checkMappedOutRanks = 0;

  CacheDdrIoTarget (Host, socket);

  // Do not run late command clock if socket is in 3N Timing
  if (Host->nvram.mem.socket[socket].channelList[0].timingMode == TIMING_3N) {
    return SUCCESS;
  }

  //
  // Configure NVMCTLR to enable Early CMD training mode
  //
  SetAepTrainingMode (Host,socket, CHECKPOINT_MINOR_LATE_CMD_CLK);

  //
  // Final Composite Margin & CK Alignment
  //
  PerCMDGroup(Host, Host->var.mem.currentSocket, COMPOSITE_GROUPS);

  //
  // Configure NVMCTLR to disable Early CMD training mode
  //
  SetAepTrainingModeSMB(Host, socket, DISABLE_TRAINING_STEP);

  //
  // Indicate CMD training is done
  //
  Host->nvram.mem.socket[Host->var.mem.currentSocket].cmdClkTrainingDone = 1;

  //
  // Enable PPDF in next JEDEC INIT
  //
  IO_Reset (Host, Host->var.mem.currentSocket);
  JedecInitSequence (Host, Host->var.mem.currentSocket, CH_BITMASK);

  //
  // Avoid clock movement for BPS A0
  //
  if (FmcAvoidClockMovementInSocket (Host, socket) == FALSE) {
    NormalizeCCC(Host, Host->var.mem.currentSocket);
  }

  return SUCCESS;
} // ChipLateCmdClk

VOID
ChipEnableCheckParity(
  PSYSHOST Host,
  UINT16* mode
  )
{
  *mode |= MODE_CHECK_PARITY;
}

VOID
TrnOnChipCMDADDOpEn(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct
  )
{
  SCHEDULER_CMD_DEBUG_MCDDC_CTL_STRUCT SchedulerCmdDebug;

  SchedulerCmdDebug.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG);
  SchedulerCmdDebug.Bits.cmd_ca_oe = 1;
  SchedulerCmdDebug.Bits.cmd_ca_on = 1;
  MemWritePciCfgEp (socket, ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG, SchedulerCmdDebug.Data);
}

VOID
EnableChipFNV(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct
  )
{
  SetAepTrainingMode (Host, socket, CHECKPOINT_MINOR_EARLY_CMD_CLK);
} // EnableChipFNV


VOID
TrnOffChipCMDADDOpEn(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct
  )
{
  SCHEDULER_CMD_DEBUG_MCDDC_CTL_STRUCT SchedulerCmdDebug;

  if (!IsSiliconWorkaroundEnabled ("S1909269691")) {
    SchedulerCmdDebug.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG);
    SchedulerCmdDebug.Bits.cmd_ca_oe = 0;
    SchedulerCmdDebug.Bits.cmd_ca_on = 0;
    MemWritePciCfgEp (socket, ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG, SchedulerCmdDebug.Data);
  }
}

VOID
GetCmdMarginsSweepInit(
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct
  )
{
  UINT8 ch;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  chipGetCmdMgnSwpStruct->chEyemask = 0;
  chipGetCmdMgnSwpStruct->chDonemask = 0;

  for (ch = 0; ch < MaxChDdr; ch++) {
    chipGetCmdMgnSwpStruct->eyeSize[ch] = 0;
  }
}

VOID
UpdateCmdMarginMasks(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  INT16 piIndex,
  INT16 offset,
  UINT8 errorResult[MAX_CH][PAR_TEST_RANGE],
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct
  )
{
  UINT8 step;

  if (errorResult[ch][piIndex]) {
    chipGetCmdMgnSwpStruct->eyeSize[ch] = 0;
  } else {
    // errorResult[ch][piIndex] = 0;
    chipGetCmdMgnSwpStruct->eyeSize[ch] += offset;
  }

  if (chipGetCmdMgnSwpStruct->eyeSize[ch] > 64) {
     chipGetCmdMgnSwpStruct->chEyemask &= ~(1 << ch);
  }
  if (((chipGetCmdMgnSwpStruct->chEyemask && (1 << ch)) == 0) && (errorResult[ch][piIndex] == 1)) {
     chipGetCmdMgnSwpStruct->chDonemask &= ~(1 << ch);
  }

  //
  // Fill in results for skipped Pi delays
  //
  for (step = 0; step < offset; step++) {
    if ((piIndex + step) >= PAR_TEST_RANGE) {
      continue;
    }

    errorResult[ch][piIndex + step] = errorResult[ch][piIndex];
  }
} // UpdateCmdMarginMasks

UINT8
CheckChDoneMask(
  PSYSHOST Host,
  PCHIP_GET_CMD_MGN_SWP_STRUCT chipGetCmdMgnSwpStruct
  )
{
  if(chipGetCmdMgnSwpStruct->chDonemask == 0) {
    return 0;
  } else {
    return 1;
  }
} // CheckChDoneMask

BOOLEAN
ShouldChipClearCMDMGNParityError(
  PSYSHOST Host,
  UINT32 status
  )
{
  if ((status) || ((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM))) {
    return TRUE;
  }
  return FALSE;
}

/**
  Collects the results of the previous parity based test

  @param Host:         Pointer to sysHost
  @param socket:         Processor socket to check

  @retval status:       Non-zero if any failure was detected
**/
UINT32
ChipCollectParityResults (
                     PSYSHOST  Host,
                     UINT8     socket,
                     UINT32    chToCheck,
                     UINT8     dimm,
                     UINT32    bwSerr[MAX_CH][3]
                     )
{
  UINT8                       ch;
  UINT32                      err;
  UINT32                      status = 0;
  struct channelNvram         (*channelNvList)[MAX_CH];
  struct dimmNvram            (*dimmNvList)[MAX_DIMM];
  ALERTSIGNAL_MCDDC_DP_STRUCT alertSignal;
  DDRT_ERROR_MC_2LM_STRUCT    errSignals;
  UINT8                       MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    // No need to check this channel if it's bit isn't set
    if ((chToCheck & (1 << ch))) {

      err = 0;
      dimmNvList = GetDimmNvList(Host, socket, ch);
      if ((*dimmNvList)[dimm].DcpmmPresent) {
        errSignals.Data = MemReadPciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG);
        switch (dimm) {
          case 0:
            err = (UINT8)errSignals.Bits.observed_ddrt_err0_assert;
            break;
          case 1:
            err = (UINT8)errSignals.Bits.observed_ddrt_err1_assert;
            break;
          default:
            //Automated add of default case. Please review.
            break;
        }
      } else {
        alertSignal.Data = (UINT8)MemReadPciCfgEp (socket, ch, ALERTSIGNAL_MCDDC_DP_REG);
        err = alertSignal.Bits.seen;
      }

      status |= err;
      if (err) {
        bwSerr[ch][0] = 0xFFFFFFFF;
        bwSerr[ch][1] = 0xFFFFFFFF;
        if (Host->nvram.mem.eccEn) {
          bwSerr[ch][2] = 0xFF;
        } else {
          bwSerr[ch][2] = 0;
        }
      } // DcpmmPresent
    } // chToCheck
  } // ch loop

  return  status;
} // ChipCollectParityResults

BOOLEAN
DidChipClearAEPDimmParityErrors10nm(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  PCHIP_CLEAR_PARITY_RESULTS_STRUCT chipClearParityResStruct
  )
{
  struct dimmNvram            (*dimmNvList)[MAX_DIMM];
  DDRT_ERROR_MC_2LM_STRUCT    errSignals;

  dimmNvList = GetDimmNvList(Host, socket, ch);

  if ((*dimmNvList)[dimm].DcpmmPresent) {
    errSignals.Data = MemReadPciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG);
    switch (dimm) {
    case 0:
      errSignals.Bits.observed_ddrt_err0_assert = 0;
      break;
    case 1:
      errSignals.Bits.observed_ddrt_err1_assert = 0;
      break;
      default:
        //Automated add of default case. Please review.
        break;
    }
    chipClearParityResStruct->errSignals.Data = errSignals.Data;
    MemWritePciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG, chipClearParityResStruct->errSignals.Data);
    return TRUE;
  } //else {

  return FALSE; //Default to test for if no aepDimm is present.
}

/**
  Clears the results of the previous parity based test

  @param Host:         Pointer to sysHost
  @param socket:         Processor socket to check

  @retval status:       Non-zero if any failure was detected
**/
VOID
ChipClearParityResults (
                   PSYSHOST  Host,
                   UINT8     socket,
                   UINT32    chToCheck,
                   UINT8     dimm
                   )
{
  UINT8                       ch;
  UINT8                       timingModeOrg[MAX_CH];
  struct channelNvram         (*channelNvList)[MAX_CH];
  ALERTSIGNAL_MCDDC_DP_STRUCT alertSignal;
  CHIP_CLEAR_PARITY_RESULTS_STRUCT chipClearParityResStruct;
  UINT8                            MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  ZeroMem (timingModeOrg, sizeof (timingModeOrg));
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Set 3N timing and save original setting
    //
    timingModeOrg[ch] = GetCurrentTimingMode(Host, socket, ch);
    SetTimingMode(Host, socket, ch, (*channelNvList)[ch].trainTimingMode);
  } // ch loop

  IO_Reset (Host, socket);
  JedecInitSequence (Host, socket, CH_BITMASK);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    //
    // Clear parity error status in the memory controller
    //
    if(!DidChipClearAEPDimmParityErrors10nm(Host, socket, ch, dimm, &chipClearParityResStruct)){
      alertSignal.Data = (UINT8)MemReadPciCfgEp (socket, ch, ALERTSIGNAL_MCDDC_DP_REG);
      alertSignal.Bits.seen = 1;
      MemWritePciCfgEp (socket, ch, ALERTSIGNAL_MCDDC_DP_REG, (UINT32)alertSignal.Data);
    }
    //
    // Restore original CMD timing
    //
    SetTimingMode(Host, socket, ch, timingModeOrg[ch]);

  } // ch loop
} // ClearParityResults

BOOLEAN
ShouldChipClearPtyAltWriteMR510nm(
  PSYSHOST Host
  )
{
  if ((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM)) {
    return TRUE;
  }
  else
    return FALSE;
}

VOID
ChipSet3NTimingandSaveOrigSetting(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_PARITY_CHECKING_STRUCT chipEnableParityCheckingStruct
  )
{
  struct channelNvram         (*channelNvList)[MAX_CH];
  channelNvList = GetChannelNvList(Host, socket);

  chipEnableParityCheckingStruct->timingModeOrg = GetCurrentTimingMode (Host, socket, ch);
  SetTimingMode (Host, socket, ch, (*channelNvList)[ch].trainTimingMode);
}

VOID
ChipRestoreOrigCMDtiming(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_PARITY_CHECKING_STRUCT chipEnableParityCheckingStruct
  )
{
  SetTimingMode (Host, socket, ch, chipEnableParityCheckingStruct->timingModeOrg);
}


BOOLEAN
DoesChipSupportParityChecking(
  PSYSHOST Host
  )
{
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5 && (Host->nvram.mem.dimmTypePresent == UDIMM || Host->nvram.mem.dimmTypePresent == SODIMM)) {
    return FALSE; // CA Parity is not supported in DDR5 DRAM. CA Parity is only supported in RCD
  } else {
    return TRUE;  //Parity checking is supported on DDR4 Dimm Types
  }
}

/**
  Clears the results of the previous parity based test

  @param Host:         Pointer to sysHost
  @param socket:       Processor socket to check
  @param rank:         Rank to clear
  @retval status:       Non-zero if any failure was detected
**/
VOID
ChipClearParityResultsAlt (
                      PSYSHOST  Host,
                      UINT8     socket,
                      UINT32    chToCheck,
                      UINT8     rank,
                      UINT8     dimm
                      )
{
#ifndef DDR5_SUPPORT
  UINT8                       ch;
  UINT8                       timingModeOrg[MAX_CH];
  struct channelNvram         (*channelNvList)[MAX_CH];
  struct dimmNvram            (*dimmNvList)[MAX_DIMM];
  struct rankDevice           (*rankStruct)[MAX_RANK_DIMM];
  ALERTSIGNAL_MCDDC_DP_STRUCT alertSignal;
  DDRT_ERROR_MC_2LM_STRUCT    errSignals;
  UINT8                       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  ZeroMem (timingModeOrg, sizeof (timingModeOrg));
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    // No need to check this channel if it's bit isn't set
    if ((chToCheck & (1 << ch))) {
      //
      // Set 3N timing and save original setting
      //
      timingModeOrg[ch] = GetCurrentTimingMode(Host, socket, ch);
      SetTimingMode(Host, socket, ch, (*channelNvList)[ch].trainTimingMode);

      //
      // Skip if no DIMM present
      //
      dimmNvList = GetDimmNvList(Host, socket, ch);
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      rankStruct  = GetRankStruct(Host, socket, ch, dimm);

      //
      // Skip if no rank
      //
      if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      if (Host->nvram.mem.dimmTypePresent == UDIMM) {
        WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
      } else {
        if (rank == 0) { //RDIMM is only RANK0
          WriteRC (Host, socket, ch, dimm, rank, 6, RDIMM_RC06);
          WriteRC (Host, socket, ch, dimm, rank, BIT0, RDIMM_RC0E);
        }
      }

      FixedDelayMicroSecond (1);
    }
  } // ch loop

  IO_Reset (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if (chToCheck & (1 << ch)) {

      //
      // Clear parity error status in the memory controller
      //
      dimmNvList = GetDimmNvList(Host, socket, ch);
      if ((*dimmNvList)[dimm].DcpmmPresent) {
        errSignals.Data = MemReadPciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG);
        switch (dimm) {
          case 0:
            errSignals.Bits.observed_ddrt_err0_assert = 0;
            break;
          case 1:
            errSignals.Bits.observed_ddrt_err1_assert = 0;
            break;
          default:
            //Automated add of default case. Please review.
            break;
        }
        MemWritePciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG, errSignals.Data);

      } else {
        alertSignal.Data = (UINT8)MemReadPciCfgEp (socket, ch, ALERTSIGNAL_MCDDC_DP_REG);
        alertSignal.Bits.seen = 1;
        MemWritePciCfgEp (socket, ch, ALERTSIGNAL_MCDDC_DP_REG, (UINT32)alertSignal.Data);
      }
      //
      // Restore original CMD timing
      //
      SetTimingMode(Host, socket, ch, timingModeOrg[ch]);

    }
  } // ch loop
#endif // !DDR5_SUPPORT
} // ClearParityResultsAlt

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
  )
{
#ifndef DDR5_SUPPORT
  UINT8                       Ch;
  struct channelNvram         (*ChannelNvList)[MAX_CH];
  struct dimmNvram            (*DimmNvList)[MAX_DIMM];
  struct rankDevice           (*RankStruct)[MAX_RANK_DIMM];
  ALERTSIGNAL_MCDDC_DP_STRUCT AlertSignal;
  UINT8                       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    //
    // No need to check this channel if it's bit isn't set
    //
    if ((ChToCheck & (1 << Ch))) {

      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      //
      // Skip if no rank
      //
      RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      WriteMRS (Host, Socket, Ch, Dimm, Rank, (*RankStruct)[Rank].MR5, BANK5);
      FixedDelayMicroSecond (1);
    }
  } // Ch loop

  IO_Reset (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    if (ChToCheck & (1 << Ch)) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT,
        "Clearing alert seen bit\n");
      //
      // Clear parity error status in the memory controller
      //
      DimmNvList = GetDimmNvList (Host, Socket, Ch);
      AlertSignal.Data = (UINT8) MemReadPciCfgEp (Socket, Ch, ALERTSIGNAL_MCDDC_DP_REG);
      AlertSignal.Bits.seen = 1;
      MemWritePciCfgEp (Socket, Ch, ALERTSIGNAL_MCDDC_DP_REG, (UINT32) AlertSignal.Data);
    }
  } // Ch loop
#endif // !DDR5_SUPPORT
} // ChipClearBacksideParityResults

VOID
ChipSpecificParityCheckDisablePerCh (
  PSYSHOST                     Host,
  UINT8                        socket,
  UINT8                        ch,
  PCHIP_PARITY_CHECKING_STRUCT chipDisableParityCheckingStruct
  )
{
  //
  // API comply
  //
}

VOID
PerformChipSpecificNormalCMDTimingSetup10nm(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_SET_NORMAL_CMD_TIMING_STRUCT chipSetNormalCMDTimingStruct
  )
{
  T_DDRT_MISC_DELAY_MC_2LM_STRUCT ddrtMiscDelay;

  ddrtMiscDelay.Data = MemReadPciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG);
#ifndef GNRSRF_HOST
  if (chipSetNormalCMDTimingStruct->timingMode == TIMING_2N) {
    ddrtMiscDelay.Bits.enable_ddrt_2n_timing = 1;
  } else {
    ddrtMiscDelay.Bits.enable_ddrt_2n_timing = 0;
  }
#endif // !GNRSRF_HOST
  chipSetNormalCMDTimingStruct->ddrtMiscDelay.Data = ddrtMiscDelay.Data;
  MemWritePciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG, chipSetNormalCMDTimingStruct->ddrtMiscDelay.Data);
}

BOOLEAN
ShouldChipSetNormalCMDTiming(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_SET_NORMAL_CMD_TIMING_STRUCT chipSetNormalCMDTimingStruct
  )
{
  struct channelNvram         (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(Host, socket);

  chipSetNormalCMDTimingStruct->timingMode = GetCurrentTimingMode(Host, socket, ch);

  if (chipSetNormalCMDTimingStruct->timingMode != (*channelNvList)[ch].timingMode) {
    chipSetNormalCMDTimingStruct->timingMode  = (*channelNvList)[ch].timingMode;
    SetTimingMode(Host, socket, ch, chipSetNormalCMDTimingStruct->timingMode);

    PerformChipSpecificNormalCMDTimingSetup10nm(Host, socket, ch, chipSetNormalCMDTimingStruct);
  }

  return FALSE;
}

VOID
ChipCtlClkFeedBack (
  PSYSHOST  Host,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     sign,
  INT16     *ctlMargins
)
{

  GetChipCtlClkFeedBackTarget (Host, ch, dimm, rank, sign, ctlMargins);

}

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
StepChipCtlCKIOLatency(
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   LogRank
  )
{
  UINT8                                 IoLatency;

  //
  // Nothing to do for 2TCLK read preamble
  //
  if (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_1TCLK) {
    //
    // Add 2 to IO latency
    // Must maintain an odd value so step by 2
    //
    GetIoLatency (Socket, Ch, LogRank, GSM_READ_ONLY, &IoLatency);
    IoLatency += EARLY_CTL_CLK_IO_LATENCY_OFFSET_1TCLK;
    SetIOLatency (Socket, Ch, LogRank, IoLatency);
  }
  return EFI_SUCCESS;
}

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
RestoreChipCtlCKIOLatency(
  IN    UINT8   Socket,
  IN    UINT8   Ch,
  IN    UINT8   LogRank
  )
{
  UINT8                                 IoLatency;
  //
  // Nothing to do for 2TCLK read preamble
  //
  if (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_1TCLK) {
    //
    // Restore IO latency
    // Must match the step value in StepChipCtlCKIOLatency()
    //
    GetIoLatency (Socket, Ch, LogRank, GSM_READ_ONLY, &IoLatency);
    IoLatency -= EARLY_CTL_CLK_IO_LATENCY_OFFSET_1TCLK;
    SetIOLatency (Socket, Ch, LogRank, IoLatency);
  }
  return EFI_SUCCESS;
}

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
  )
{
  if (GetPreambleState (Socket, READ_PREAMBLE) == PREAMBLE_1TCLK) {
    *RecEnOffset = EARLY_CTL_CLK_REC_EN_OFFSET_1TCLK;
  } else {
    *RecEnOffset = EARLY_CTL_CLK_REC_EN_OFFSET_2TCLK;
  }

  return EFI_SUCCESS;
}

/**

  Handles any register interaction on GetMargins entry/exit.

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number
  @param entry   - TRUE when entering GetMargins, FALSE when exiting.

**/
UINT32
GetMarginsHook (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  BOOLEAN   entry
)
{
  SCHEDULER_CMD_DEBUG_MCDDC_CTL_STRUCT  SchedulerCmdDebug;

  if (!IsSiliconWorkaroundEnabled ("S1909269691") || (entry == 1)) {
    SchedulerCmdDebug.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG);
    SchedulerCmdDebug.Bits.cmd_ca_oe = entry;
    SchedulerCmdDebug.Bits.cmd_ca_on = entry;
    MemWritePciCfgEp (socket, ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG, SchedulerCmdDebug.Data);
  }
  return SUCCESS;
}

/**

  Checks if DQ swizzling is supported

  @param Host    - Pointer to sysHost

**/
UINT8
CheckDqSwizzleSupport (
  PSYSHOST  Host,
  UINT8     socket
)
{
  UINT8                         aepdimms = 0;
  UINT8                         ch;
  struct channelNvram           (*channelNvList)[MAX_CH];
  SYS_SETUP                     *Setup;
  UINT8                         MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  // check for NVMDIMM DIMMs present SKX TO: replace this with NVMDIMM per socket variable
  channelNvList = &Host->nvram.mem.socket[socket].channelList;
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if ((*channelNvList)[ch].ddrtEnabled == 1) {
      aepdimms = 1;
    }
  }

  // run DqSwizzleDiscovery if WR_CRC enabled
  if ((Host->nvram.mem.socket[socket].enabled == 0) ||
      (Host->nvram.mem.socket[socket].maxDimmPop == 0) ||
      (!IsMemFlowEnabled (DqSwizzlingDiscovery)) ||
      (Host->nvram.mem.eccEn == 0) ||
      ((!(Setup->mem.optionsExt & WR_CRC)) && (aepdimms == 0)) ||
      (Host->nvram.mem.dramType != SPD_TYPE_DDR4) ||
      (Host->nvram.mem.RASmode & STAT_VIRT_LOCKSTEP)) {

    RcDebugPrint (SDBG_MAX,
                   "Skipping Swizzle Discovery.\n");
    return 0;
  }
  //
  // Since there is an AEP DIMM installed and Swizzle Discover is supported,
  //  we need to clear the DQ Swizzle programming in the DIMM
  //
  ClearDqSwizzleAep (Host, socket);

  return 1;
}

//---------------------------------------------------------------------------------------------------

/**

  Check if CTL loopback mode should be used

  @param Host    - Pointer to sysHost

  @retval N/A

**/
BOOLEAN
CheckCTLLoopback (
             PSYSHOST Host
             )
{
  if (Host->nvram.mem.dimmTypePresent == RDIMM) {
    return TRUE;
  } else {
    return FALSE;
  }
} // CheckCTLLoopback

/**

  This gets the parity error status during CTL loopback training

  @param Host    - Pointer to sysHost
  @param socket    - Socket number

  @retval N/A

**/
UINT8
ChipGetParityErrorStatus (
             PSYSHOST Host,
             UINT8    socket,
             UINT8    ch,
             UINT8    dimm
             )
{
  UINT8                       err = 0;
  struct dimmNvram            (*dimmNvList)[MAX_DIMM];
  ALERTSIGNAL_MCDDC_DP_STRUCT alertSignal;
  DDRT_ERROR_MC_2LM_STRUCT    errSignals;

  dimmNvList = GetDimmNvList(Host, socket, ch);
  if ((*dimmNvList)[dimm].DcpmmPresent) {
    //
    // Clear parity error status in the memory controller
    // 4928893  MRC: For Early CMD CLK, ERR# bits need to be cleared before read
    //
    errSignals.Data = MemReadPciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG);
    switch (dimm) {
      case 0:
        errSignals.Bits.observed_ddrt_err0_assert = 0;
        break;
      case 1:
        errSignals.Bits.observed_ddrt_err1_assert = 0;
        break;
      default:
        //Automated add of default case. Please review.
        break;
    }
    MemWritePciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG, errSignals.Data);

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //

    CteDelayDclk (50);

    //
    // Read Alert Feedback Sticky Bit
    //
    errSignals.Data = MemReadPciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG);
    switch (dimm) {
      case 0:
        err = (UINT8)errSignals.Bits.observed_ddrt_err0_assert;
        break;
      case 1:
        err = (UINT8)errSignals.Bits.observed_ddrt_err1_assert;
        break;
      default:
        //Automated add of default case. Please review.
        break;
    }

  } else {
    //
    // Clear parity error status in the memory controller
    //
    alertSignal.Data = (UINT8)MemReadPciCfgEp (socket, ch, ALERTSIGNAL_MCDDC_DP_REG);
    alertSignal.Bits.seen = 1;
    MemWritePciCfgEp (socket, ch, ALERTSIGNAL_MCDDC_DP_REG, (UINT32)alertSignal.Data);

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //

    CteDelayDclk (50);

    //
    // Read Alert Feedback Sticky Bit
    //
    alertSignal.Data = (UINT8)MemReadPciCfgEp (socket, ch, ALERTSIGNAL_MCDDC_DP_REG);
    err = alertSignal.Bits.seen;
  }


  return err;
} // ChipGetParityErrorStatus

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
  IN  UINT8    socket,
  IN  UINT8    ch,
  IN  UINT8    dimm,
  IN  UINT8    rank
)
{
  UINT32              lrbufData;
  struct dimmNvram            (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(Host, socket, ch);

  if ((*dimmNvList)[dimm].DcpmmPresent) {
    //
    // Only put FMC CTL signal into loopback mode when train FMC CTL sigal
    //
    if (rank == 0) {
      ChipSetCTLLoopbackFmc (Host, socket, ch, dimm, TRUE);
    } else {
      ChipSetCTLLoopbackFmc (Host, socket, ch, dimm, FALSE);
    }
  } else {
    //
    // Put parid FMC CS_0# into loopback mode to avoid CS# got toggle when train DDR4
    //
    if ((dimm == 0) && ((*dimmNvList)[1].DcpmmPresent)) {
      ChipCS0ToLoopback (Host, socket, ch);
    }

    /*
    RC0C: Training Control Word
      000 = Normal operating mode
      001 = Clock-to-CA training mode1
      010 = DCS0_n loopback mode1
      011 = DCS1_n loopback mode1
      100 = DCKE0 loopback mode1
      101 = DCKE1 loopback mode1
      110 = DODT0 loopback mode1
      111 = DODT1 loopback mode1
    */
    lrbufData = (*dimmNvList)[dimm].rcCache[RDIMM_RC0C];

    // Inphi and IDT Gen 2 RCD Clock-to-CA loopback workaround
    if ( ((((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) && ((*dimmNvList)[dimm].SPDRegRev == INPHI_SPD_REGREV_GEN2_C0)) ||
          (((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT) && ((*dimmNvList)[dimm].SPDRegRev == IDT_SPD_REGREV_GEN2_B0)))
          && ((*dimmNvList)[dimm].rcCache[RDIMM_RC0D] & 0x1) && (lrbufData == 0x1)) {

      // configure RC0D = 0 = Dual Rank Mode for when RC0C = 1 = Clock-to-CA training mode
      lrbufData |= (((*dimmNvList)[dimm].rcCache[RDIMM_RC0D] & 0xc) << 4);
    } else {
      lrbufData |= ((*dimmNvList)[dimm].rcCache[RDIMM_RC0D] << 4);
    }

    WriteLrbufSmb (Host, socket, ch, dimm, 1, 0x0e, &lrbufData);
  }
} // ChipSetCTLLoopback

VOID
CheckTwrMargins(
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     tWR,
  UINT8     *DisCh
)
{
} // CheckTwrMargins

VOID
NormalizeLockstepTiming(
  PSYSHOST  Host,
  UINT8     socket
)
{
} // NormalizeLockstepTiming

VOID
ReceiveEnableInitAllRanks (
  PSYSHOST Host,
  UINT8    socket
  )
{
  UINT8                             ch;
  struct channelNvram               (*channelNvList)[MAX_CH];
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    ReceiveEnableInitChip10nm ( Host,  socket,  ch);
  } // ch loop
}

VOID
ReceiveEnableInitChip10nm (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
/*++

  Initialize channel for Recieve Enable training.
  This initialization is needed each time training is performed on a new rank.

  @param Host            - Pointer to sysHost
  @param socket          - Socket number

  @retval  N/A

--*/
{
  UINT8                             dimm;
  UINT8                             rank;
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  dimmNvList = GetDimmNvList(Host, socket, ch);

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
      if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      //
      // Set initial Roundtrip Latency values
      //
      SetInitRoundTrip(Host, socket, ch, dimm, rank);
    } // rank loop
  } // dimm loop
  SetIoLatComp (Host, socket, ch);
} // ReceiveEnableInitChip10nm

VOID
EarlyCmdSignalTest(
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitmask
  )
/*++

Routine Description:

  Start and stop Early CMD/CLK tests

Arguments:

  Host        - Pointer to sysHost
  socket      - Socket number
  chBitmask   - Channels to execute test on

Returns:

  N/A

--*/
{

 ExecuteCmdSignalTest (Host, socket, chBitmask, NO_SUBCH);

} // EarlyCmdSignalTest

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
  )
{
  SCHEDULER_IDLETIME2_MCDDC_CTL_STRUCT  IdleTime2;

  IdleTime2.Data = MemReadPciCfgEp (Socket, Channel, SCHEDULER_IDLETIME2_MCDDC_CTL_REG);

  if (IdleTime2.Bits.opp_pg_en == 0) {
    return PageTimerDisable;
  } else {
    return PageTimerEnable;
  }
}

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
  )
{
  SCHEDULER_IDLETIME2_MCDDC_CTL_STRUCT  IdleTime2;

  IdleTime2.Data = MemReadPciCfgEp (Socket, Channel, SCHEDULER_IDLETIME2_MCDDC_CTL_REG);

  if (State == PageTimerDisable) {
    IdleTime2.Bits.opp_pg_en = 0;
  } else if (State == PageTimerEnable) {
    IdleTime2.Bits.opp_pg_en = 1;
  } else {
    RcDebugPrintWithDevice (SDBG_ERROR, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "During call to SetPageTableTimer(), invalid PAGE_TIMER_STATE requested: 0x%x", State);
    ASSERT (FALSE);
  }

  MemWritePciCfgEp (Socket, Channel, SCHEDULER_IDLETIME2_MCDDC_CTL_REG, IdleTime2.Data);
}

/**

  This gets the CMD Tx Eq settings list

  @param[in]     Host           - Pointer to sysHost
  @param[in]     Socket         - Socket number
  @param[in,out] SettingList    - List of CMD TxEq settings to be tested

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetCmdTxEqSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  UINT8 Loop;
  UINT8   NumSettings;

  NumSettings = MIN (NUM_CMD_TX_EQ_SETTINGS, sizeof (Param->SettingList) / sizeof (Param->SettingList[0]));

  for (Loop = 0; Loop < NumSettings; Loop++) {
    Param->SettingList[Loop] = Loop;
  }

  Param->NumSettings = NumSettings;
  Param->ParamType = PerCh;

  return MRC_STATUS_SUCCESS;
} // GetCmdTxEqSettings

/**

  Gets the Tx Rise/Fall Slew Rate settings list

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetTxRiseFallSlewRateSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{

  Param->ParamType = PerBytePerCh;

  return MRC_STATUS_SUCCESS;
} // GetTxRiseFallSlewRateSettings

/**

  Gets the Tx Fall Slew Rate settings list

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetTxFallSlewRateSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  UINT8   SizeSettings;
  INT16   SettingListLocal[] = {25, 26, 27, 28, 29, 30, 31, 0, 1, 2, 3, 4, 5, 6, 7, 8};

  SizeSettings = MIN (sizeof (SettingListLocal), sizeof (Param->SettingList));
  Param->NumSettings = SizeSettings / sizeof (SettingListLocal[0]);

  CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)SettingListLocal, SizeSettings);

  Param->ParamType = PerBytePerCh;

  return MRC_STATUS_SUCCESS;
} // GetTxFallSlewRateSettings

#define TCO_COARSE_STEP_SIZE    8
/**

  This gets the common DCA and DQ TCO settings list for the coarse sweep

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetTcoCoarseSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  UINT8   Loop;
  UINT8   StepSize;
  UINT8   NumSettings;

  //The Mode, PatternLength, and MarginType parameters are not used in by TCO training
  //so just set normal default values for these fields.
  Param->Mode           = MODE_VIC_AGG;
  Param->PatternLength  = 64;
  Param->MarginType     = GetMargin;

  StepSize = TCO_COARSE_STEP_SIZE;
  NumSettings = MIN (NUM_TCO_COARSE_SETTINGS, sizeof (Param->SettingList) / sizeof (Param->SettingList[0]));

  for (Loop = 0; Loop < NumSettings; Loop++) {
    Param->SettingList[Loop] = (StepSize / 2) + (StepSize * Loop);
  }
  Param->NumSettings = NumSettings;

  return MRC_STATUS_SUCCESS;
}

/**

  This gets the TCO COMP settings list

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetTcoCompSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  UINT8   Loop;
  UINT8   StepSize;
  UINT8   NumSettings;

  StepSize = NUM_TCO_COMP_STEP;
  NumSettings = MIN (NUM_TCO_COMP_SETTINGS, sizeof (Param->SettingList) / sizeof (Param->SettingList[0]));

  for (Loop = 0; Loop < NumSettings; Loop++) {
    if (Loop < (NUM_TCO_COMP_SETTINGS / 2)) {
      // TCO_COMP sweep range 1: 0-31
      Param->SettingList[Loop] = MIN_TCO_COMP_RANGE1 + (StepSize * Loop);
    } else {
      // TCO_COMP sweep range 1: 64-95
      Param->SettingList[Loop] = MIN_TCO_COMP_RANGE2 + (StepSize * (Loop - (NUM_TCO_COMP_SETTINGS / 2)));
    }
  }

  Param->NumSettings = NumSettings;
  Param->ParamType = PerBytePerCh;

  return MRC_STATUS_SUCCESS;
} // GetTcoCompSettings

/**

  This gets the Dram Drive Strength COMP settings list

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetDramDrvStrCompSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  Param->ParamType        = PerRankPerCh;
  return MRC_STATUS_SUCCESS;
} // GetDramDrvStrCompSettings

/**

  Gets DramRxEq settings list

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetDramRxEqSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  //
  // Override default settingList with chip specific values if required
  //
  UINT8       SizeSettings;
  INT16       DramRxEqSettings[] = {0, 1, 2, 3, 4, 5, 6, 7};

  SizeSettings = MIN (sizeof (DramRxEqSettings), sizeof (Param->SettingList));
  Param->NumSettings = SizeSettings / sizeof (DramRxEqSettings[0]);

  CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)DramRxEqSettings, SizeSettings);
  Param->ParamType = PerNibblePerRankPerCh;

  return MRC_STATUS_SUCCESS;
} // GetDramRxEqSettings

/**

  Initialize the imode setting list

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetImodeSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  UINT8   Loop;
  UINT8   NumSettings;
  INT16   SweepRange[NUM_IMODE_SETTINGS_10NM] = {2, 3, 6, 7, 10, 11, 14, 15, 18, 19, 22, 23, 26, 27, 30, 31, 50, 51, 54, 55, 58, 59, 62, 63};

  NumSettings = MIN (sizeof (SweepRange) / sizeof (SweepRange[0]), sizeof (Param->SettingList) / sizeof (Param->SettingList[0]));

  for (Loop = 0; Loop < NumSettings; Loop++) {
    Param->SettingList[Loop] = SweepRange[Loop];
  }

  Param->NumSettings = NumSettings;
  Param->ParamType = PerBytePerCh;

  return MRC_STATUS_SUCCESS;
} // GetImodeSettings

/**

  Gets  CTLE settings list

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetCtleSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT8       Loop;
  INT16       RxEqSettingList[NUM_CTLE_SETTINGS_10NM]  = { 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 4, 4, 5, 5, 4, 4 };
  INT16       CtleCSettingList[NUM_CTLE_SETTINGS_10NM] = { 1, 3, 2, 3, 2, 3, 1, 2, 1, 1, 3, 2, 1, 3, 3, 3 };
  INT16       CtleRSettingList[NUM_CTLE_SETTINGS_10NM] = { 3, 3, 3, 1, 1, 0, 1, 0, 3, 0, 3, 3, 3, 3, 1, 0 };

  Param->NumSettings = MIN (MAX_SETTINGS, NUM_CTLE_SETTINGS_10NM);

  switch (Param->ParamGsm) {
    case RxEq:
      for (Loop = 0; Loop < Param->NumSettings; Loop++) {
        Param->SettingList[Loop] = RxEqSettingList[Loop];
      }
      Param->ParamType = PerBytePerRankPerCh;
      break;
    case RxCtleC:
      for (Loop = 0; Loop < Param->NumSettings; Loop++) {
        Param->SettingList[Loop] = CtleCSettingList[Loop];
      }
      Param->ParamType = PerBytePerCh;
      break;
    case RxCtleR:
      for (Loop = 0; Loop < Param->NumSettings; Loop++) {
        Param->SettingList[Loop] = CtleRSettingList[Loop];
      }
      Param->ParamType = PerBytePerCh;
      break;
    default:
      Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
      break;
  }

  return Status;
} // GetCtleSettings

/**

  This gets MC_RON settings list

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetMcRonSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  //
  // Override default settingList with chip specific values if required
  //
  UINT8   SizeSettings;
  UINT8   NumSettings;
  INT16   McRonSettingList[NUM_MCRON_SETTINGS] = {TX_RON_33, TX_RON_30, TX_RON_28};

  SizeSettings = MIN (sizeof (McRonSettingList), sizeof (Param->SettingList));
  NumSettings = SizeSettings / sizeof (McRonSettingList[0]);
  CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)McRonSettingList, SizeSettings);

  Param->NumSettings = NumSettings;
  Param->ParamType = PerMc;

  return MRC_STATUS_SUCCESS;
} // GetMcRonSettings

/**

  This gets MC ODT settings list

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetMcOdtSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  //
  // Override default settingList with chip specific values if required
  //
  UINT8   SizeSettings;
  UINT8   NumSettings;
  INT16   Settings[] = {ODT_40_OHMS, ODT_45_OHMS, ODT_50_OHMS, ODT_55_OHMS};

  SizeSettings = MIN (sizeof (Settings), sizeof (Param->SettingList));
  NumSettings = SizeSettings / sizeof (Settings[0]);
  CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)Settings, SizeSettings);

  Param->NumSettings = NumSettings;
  Param->ParamType = PerMc;

  return MRC_STATUS_SUCCESS;
} // GetMcOdtSettings

/**
  This gets PXC delta settings list

  @param [in]     Host          - Pointer to sysHost
  @param [in]     Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval - MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetPxcDeltaSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  UINT8   SizeSettings;
  UINT8   NumSettings;
  INT16   Settings[NUM_PXC_SETTINGS] = {0, 1, 2, 3};

  SizeSettings = MIN (sizeof (Settings), sizeof (Param->SettingList));
  NumSettings = SizeSettings / sizeof (Settings[0]);
  CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)Settings, SizeSettings);

  Param->NumSettings = NumSettings;
  Param->ParamType = PerBitPerRankPerCh;

  return MRC_STATUS_SUCCESS;
} // GetPxcDeltaSettings


/**

  Gets the Rx Dq Delay settings list (used in Rx DFE Training)

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetRxDfeSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT8   SizeSettings = 0;
  UINT8   NumSettings = 0;
  INT16   FirstPassSettings[NUM_RX_DFE_PARAMS][NUM_RX_DFE_SETTINGS_PER_PARAM_PASS_1] = {
                                                                                        {0, 1, 2, 3, 4, 5, 6,  0,  1,  2,  3,  4,  5,  6}, // RxDqDelay
                                                                                        {3, 3, 3, 3, 3, 3, 3, 11, 11, 11, 11, 11, 11, 11}, // RxDfeCoefficient
                                                                                       };
  INT16   SecondPassSettings[NUM_RX_DFE_COEFF_SETTINGS_PASS_2] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

  switch (Param->ParamGsm) {
    case RxDqDelay:
      SizeSettings = MIN (sizeof (FirstPassSettings[0]), sizeof (Param->SettingList));
      NumSettings = SizeSettings / sizeof (FirstPassSettings[0][0]);
      CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)FirstPassSettings[0], SizeSettings);
      // DfeCoeff/RxDqDelay should be set PerBytePerRankPerCh in Rx DFE Power Training Step as it has a physical swizzling
      Param->ParamType = PerBytePerRankPerCh;
      break;
    case RxDfe:
      SizeSettings = MIN (sizeof (FirstPassSettings[0]), sizeof (Param->SettingList));
      NumSettings = SizeSettings / sizeof (FirstPassSettings[0][0]);
      CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)FirstPassSettings[1], SizeSettings);
      Param->ParamType = PerBytePerRankPerCh;
      break;
    case RxDfeCoefficient:
      SizeSettings = MIN (sizeof (SecondPassSettings), sizeof (Param->SettingList));
      NumSettings = SizeSettings / sizeof (SecondPassSettings[0]);
      CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)SecondPassSettings, SizeSettings);
      // DfeCoeff/RxDqDelay should be set PerBytePerRankPerCh in Rx DFE Power Training Step as it has a physical swizzling
      Param->ParamType = PerBytePerRankPerCh;
      break;
    default:
      Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
      break;
  }

  Param->NumSettings = NumSettings;


  return Status;
} // GetRxDfeSettings

/**
  This gets DB DFE settings list

  @param [in]     Host          - Pointer to sysHost
  @param [in]     Socket        - Socket number
  @param[in,out]  Param         - Pointer to caller's POWER_TRAIN_PARAM

  @retval - MRC_STATUS_SUCCESS

**/
MRC_STATUS
GetDataBufferDfeSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  MRC_STATUS  Status = MRC_STATUS_SUCCESS;
  UINT8       Index;
  UINT8       SizeSettings = 0;
  UINT8       NumSettings = 0;
  INT16       Tap1Settings[NUM_DB_DFE_TAP1_SETTINGS];
  INT16       Tap2Settings[NUM_DB_DFE_TAP2_SETTINGS];
  INT16       Tap3Settings[NUM_DB_DFE_TAP3_SETTINGS];
  INT16       Tap4Settings[NUM_DB_DFE_TAP4_SETTINGS];

  switch (Param->ParamGsm) {
    case DbDfeTap1:
      // Tap 1 Sweep range: 1~29, stepsize = 2
      for (Index = 0; Index < NUM_DB_DFE_TAP1_SETTINGS; Index++) {
        Tap1Settings[Index] = Index * 2 + 1;
      }
      SizeSettings = MIN (sizeof (Tap1Settings), sizeof (Param->SettingList));
      NumSettings = SizeSettings / sizeof (Tap1Settings[0]);
      CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)Tap1Settings, SizeSettings);
      break;
    case DbDfeTap2:
      // Tap 2 Sweep range: 9~21, stepsize = 2
      for (Index = 0; Index < NUM_DB_DFE_TAP2_SETTINGS; Index++) {
        Tap2Settings[Index] = Index * 2 + 9;
      }
      SizeSettings = MIN (sizeof (Tap2Settings), sizeof (Param->SettingList));
      NumSettings = SizeSettings / sizeof (Tap2Settings[0]);
      CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)Tap2Settings, SizeSettings);
      break;
    case DbDfeTap3:
      // Tap 3 Sweep range: 0~29, stepsize = 1
      for (Index = 0; Index < NUM_DB_DFE_TAP3_SETTINGS; Index++) {
        Tap3Settings[Index] = Index;
      }
      SizeSettings = MIN (sizeof (Tap3Settings), sizeof (Param->SettingList));
      NumSettings = SizeSettings / sizeof (Tap3Settings[0]);
      CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)Tap3Settings, SizeSettings);
      break;
    case DbDfeTap4:
      // Tap 4 Sweep range: 0~29, stepsize = 1
      for (Index = 0; Index < NUM_DB_DFE_TAP4_SETTINGS; Index++) {
        Tap4Settings[Index] = Index;
      }
      SizeSettings = MIN (sizeof (Tap4Settings), sizeof (Param->SettingList));
      NumSettings = SizeSettings / sizeof (Tap4Settings[0]);
      CopyMem ((UINT8 *)Param->SettingList, (UINT8 *)Tap4Settings, SizeSettings);
      break;
    default:
      Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
      break;
  }

  Param->NumSettings = NumSettings;
  Param->ParamType = PerBitPerRankPerCh;

  return Status;
} // GetDataBufferDfeSettings

/**

  Initialize the setting list for a power training parameter

  @param[in]      Host          - Pointer to sysHost
  @param[in]      Socket        - Socket number
  @param[in]      Param         - Pointer to parameter structure

  @retval MRC_STATUS_SIGNAL_NOT_SUPPORTED if ParamType not supported
          MRC_STATUS_SUCCESS otherwise

**/
MRC_STATUS
GetPowerParameterSettings (
  IN     PSYSHOST           Host,
  IN     UINT8              Socket,
  IN OUT POWER_TRAIN_PARAM  *Param
  )
{
  MRC_STATUS                  Status;
  UINT8                       Index;
  POWER_PARAM_SETTING_GETTER  GetterList[] = {{RxOdt,               GetMcOdtSettings},
                                              {TxRon,               GetMcRonSettings},
                                              {TxEq,                GetTxEqSettings},
                                              {TxEqTap2,            GetTxEqSettings},
                                              {CmdTxEq,             GetCmdTxEqSettings},
                                              {TxImode,             GetImodeSettings},
                                              {RxEq,                GetCtleSettings},
                                              {RxCtleC,             GetCtleSettings},
                                              {RxCtleR,             GetCtleSettings},
                                              {TxRiseFallSlewRate,  GetTxRiseFallSlewRateSettings},
                                              {TxFallSlewRate,      GetTxFallSlewRateSettings},
                                              {TxTco,               GetTcoCompSettings},
                                              {DramDrvStr,          GetDramDrvStrCompSettings},
                                              {PxcDelta,            GetPxcDeltaSettings},
                                              {DdjcDelta,           GetDdjcDeltaSettings},
                                              {RxDqDelay,           GetRxDfeSettings},
                                              {RxDfe,               GetRxDfeSettings},
                                              {RxDfeCoefficient,    GetRxDfeSettings},
                                              {DramRxEq,            GetDramRxEqSettings},
                                              {DbDfeTap1,           GetDataBufferDfeSettings},
                                              {DbDfeTap2,           GetDataBufferDfeSettings},
                                              {DbDfeTap3,           GetDataBufferDfeSettings},
                                              {DbDfeTap4,           GetDataBufferDfeSettings},
                                              {LinearCntlDcaTco,    GetTcoCoarseSettings},
                                              {LinearCntlTcoDqDqs,  GetTcoCoarseSettings},
                                             };

  Status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
  for (Index = 0; Index < sizeof (GetterList) / sizeof (GetterList[0]); Index++) {
    if (GetterList[Index].ParamGsm == Param->ParamGsm) {
      Status = GetterList[Index].GetSettings (Host, Socket, Param);
      break;
    }
  }

  return Status;
}

UINT8
CheckSktId (
            PSYSHOST Host,
            UINT8    socket
)
{
  KTI_HOST_OUT *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  if (KtiHostOutPtr->MemRasFlag && (socket != KtiHostOutPtr->inComingSktId)) {
    return 1;
  }
  else return 0;
}

UINT8
CheckMemRas (
             PSYSHOST Host
)
{
  if ((GetSysSocketId () == GetSysSbspSocketIdNv ()) && (KTI_HOST_OUT_PTR->MemRasFlag == 0)) {
    return 1;
  }
  else return 0;
}

/**

  Apply settings after AdvancedMemTest ends

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Ch number
  @param scrambleConfigOrg    Original scrambler config

  @retval None

**/
VOID
PostAdvancedMemTestRestoreScrambler (
  PSYSHOST    Host,
  UINT8       socket,
  UINT8       ch,
  UINT32      scrambleConfigOrg
  )
{
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT  scrambleConfig;
  SYS_SETUP                        *Setup;

  Setup = GetSysSetupPointer ();

  //
  // Enable Scrambling
  //
  if (Setup->mem.options & SCRAMBLE_EN) {

    scrambleConfig.Data = MemReadPciCfgEp (socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG);
    scrambleConfig.Bits.rx_enable = 1;
    scrambleConfig.Bits.tx_enable = 1;
    scrambleConfig.Bits.ch_enable = 1;

    MemWritePciCfgEp (socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, scrambleConfig.Data);
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\nScrambling Enabled\n");
  }
}

UINT8
isCmdVrefTrainingDone (PSYSHOST Host,
                       UINT8    socket
)
{
  return (Host->nvram.mem.socket[socket].cmdVrefTrainingDone);
}

/**
  Programs WWSR turnaround timing considering the WrCRC when switching to normal mode.

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS - Turnaround CSRs read/write successful
**/
EFI_STATUS
EFIAPI
ProgramWwsrTAWithWrCrc (
  IN UINT8 Socket
  )
{
  EFI_STATUS          Status;
  UINT8               Ch;
  UINT32              WrCrcAdder;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT32              TurnaroundSettings[TurnaroundTypeMax];
  PSYSHOST            Host;
  SYS_SETUP           *Setup;
  UINT8               MaxChDdr;

  Status = EFI_SUCCESS;
  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();
  Host = (PSYSHOST) GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  ZeroMem (TurnaroundSettings, sizeof (TurnaroundSettings));

  //
  // Return if the WrCRC is disabled
  //
  if ((Setup->mem.optionsExt & WR_CRC) == 0) {
    return SUCCESS;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    WrCrcAdder = (UINT32)CalculateWrCrcAdder (Ch);
    Status = GetSetTurnaroundSettings (MemTechDdr, Socket, Ch, GSM_READ_CSR, TurnaroundSettings);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    TurnaroundSettings[t_WWSR] += WrCrcAdder;

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "WWSR (DDR4 to DDR4)[%d] when WrCRC = WWSR[%d] + WR_CRC[%d]\n",
                            TurnaroundSettings[t_WWSR], TurnaroundSettings[t_WWSR] - WrCrcAdder, WrCrcAdder);

    Status = GetSetTurnaroundSettings (MemTechDdr, Socket, Ch, GSM_FORCE_WRITE, TurnaroundSettings);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } // Ch loop

  return Status;
}

/**

  Enables Write CRC

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number

  @retval N/A

**/
VOID
EnableWrCrc (
  IN  PSYSHOST Host,
  IN  UINT8    socket
  )
{
  UINT8                                  ch;
  UINT8                                  rank;
  UINT8                                  dimm;
  struct channelNvram                    (*channelNvList)[MAX_CH];
  struct dimmNvram                       (*dimmNvList)[MAX_DIMM];
  struct rankDevice                      (*rankStruct)[MAX_RANK_DIMM];
  UINT8                                  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  //
  // Account for DQ swizzling if necessary
  //
  CheckAndProgramDqSwizzle10nm(Host, socket);

  //
  // Programm WWSR turnaround accounting for WrCRC
  //
  ProgramWwsrTAWithWrCrc (socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ( (*dimmNvList)[dimm].dimmPresent == 0 ) {
        continue;
      }
      rankStruct = GetRankStruct(Host, socket, ch, dimm);
      for (rank = 0;  rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
        if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        // if not S3Resume and not fastwarmboot
        if ((Host->var.mem.subBootMode != WarmBootFast) && (GetSysBootMode () != S3Resume)) {
          DramEnableWrCrc (socket, ch, dimm, rank);
        }
      } // rank
    }  // dimm
    EnableWrCrcMc (Host, socket, ch);
    EnableWrCrcDdrio (Host, socket, ch);

  } // ch loop
}

/**

  Apply settings before AdvancedMemTest starts

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Ch number

  @retval UINT32 scrambleConfigOrg

**/
UINT32
PreAdvancedMemTest (
  PSYSHOST    Host,
  UINT8       socket,
  UINT8       ch
)
{
  UINT32                           scrambleConfigOrg;
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT scrambleConfig;


  // Disable scrambler
  scrambleConfig.Data = scrambleConfigOrg = MemReadPciCfgEp (socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG);
  scrambleConfig.Bits.rx_enable = 0;
  scrambleConfig.Bits.tx_enable = 0;
  scrambleConfig.Bits.ch_enable = 0;
  MemWritePciCfgEp (socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, scrambleConfig.Data);

  if (IsSiliconWorkaroundEnabled ("S2007748898")) {
    RcDebugPrintWithDevice (
      SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Disabling Cpgc-In-Order\n"
      );
    CpgcInorderModeEnable (socket, ch, FALSE);
  } else {
    CpgcInorderModeEnable (socket, ch, TRUE);
  }


  return scrambleConfigOrg;
}

/**

  Apply settings after AdvancedMemTest ends

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Ch number
  @param scrambleConfigOrg    Original scrambler config

  @retval None

**/
VOID
PostAdvancedMemTest (
  PSYSHOST    Host,
  UINT8       socket,
  UINT8       ch,
  UINT32      scrambleConfigOrg
  )
{
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT scrambleConfig;

  // Disable scrambler
  scrambleConfig.Data = scrambleConfigOrg;
  MemWritePciCfgEp (socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, scrambleConfig.Data);

  CpgcInorderModeEnable (socket, ch, FALSE);

}

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
  )
{
  UINT8               ch;
  UINT8               dimm;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8               MaxChDdr;
  UINT8               socket;
  PSYSHOST            Host;
  THROTTLE_VALUE      ThrottleValue;
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();
  Host = GetSysHostPointer();
  socket = Host->var.mem.currentSocket;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      // Enable 2x refresh on temp_lo threshold
      // This code was moved from InitThrottling function due to Pcode issue where the DIMM temperature
      // from TSOD polling is not getting updated until reaching CPL3/4, and the default value of the
      // DIMM temperature register is above temp_lo threshold and interferes with Advanced Memtest condition on tREFI.
      //
      //Program halfX refresh value
      ThrottleValue.HalfxRefresh = FALSE;
      ThrottleValue.HalfxRefreshValue =  Setup->mem.HalfxRefreshValue;
      GetSetThrottleValues (MemTechDdr, socket, ch, dimm, WRITE_THROTTLE_VALUES, EnumHalfxRefreshValue, &ThrottleValue);
      GetSetThrottleValues (MemTechDdr, socket, ch, dimm, WRITE_THROTTLE_VALUES, EnumHalfxRefresh, &ThrottleValue);

      //Program 4X refresh value
      ThrottleValue.FourxRefresh = FALSE;
      ThrottleValue.FourxRefreshValue =  Setup->mem.FourxRefreshValue;
      GetSetThrottleValues (MemTechDdr, socket, ch, dimm, WRITE_THROTTLE_VALUES, EnumFourxRefreshValue, &ThrottleValue);
      GetSetThrottleValues (MemTechDdr, socket, ch, dimm, WRITE_THROTTLE_VALUES, EnumFourxRefresh, &ThrottleValue);

      if ((!Disable2xRefreshCh (Host, socket, ch))) {
        ThrottleValue.TwoxRefresh = TRUE;
        ThrottleValue.TwoxRefreshValue =  Setup->mem.TwoxRefreshValue;
        GetSetThrottleValues (MemTechDdr, socket, ch, dimm, WRITE_THROTTLE_VALUES, EnumTwoxRefreshValue, &ThrottleValue);
        GetSetThrottleValues (MemTechDdr, socket, ch, dimm, WRITE_THROTTLE_VALUES, EnumTwoxRefresh, &ThrottleValue);
      } else {
        // Log one warning per channel
        if ((dimm == 0) && ((Setup->mem.customRefreshRate >= CUST_REFRESH_RATE_MIN) || (Setup->mem.options & ALLOW2XREF_EN))) {
          OutputWarning (WARN_THROT_INSUFFICIENT, WARN_2X_REFRESH_TEMPLO_DISABLED, socket, ch, NO_DIMM, NO_RANK);
        }
      }
    } //dimm
  }//channel

  //
  // HBM program temperature refresh
  //
  HbmProgramTemperatureRefresh ();

}


UINT8
NumRanksToTrain (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 numRanksDefault
  )
{
  return numRanksDefault;
}

/**
  Enfore a frequency limit for DCPMM DIMMs that require it

  @param[in]      Host      Pointer to sysHost
  @param[in]      Socket    Socket number
  @param[in,out]  DdrFreq   DDR frequency

  @retval N/A
**/
VOID
EFIAPI
EnforceDcpmmFreqLimit (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN OUT  UINT8    *DdrFreq
  )
{
  UINT8                Channel;
  BOOLEAN              LimitFreq = FALSE;
  SYS_SETUP            *Setup;
  UINT8                MaxChannelDdr;

  if (DdrFreq == NULL) {
    ASSERT (FALSE);
    return;
  }

  //
  // No frequency enforcement to do if this chip doesn't require it
  //
  if (IsSiliconWorkaroundEnabled ("S1707006795") == FALSE) {
    return;
  }

  //
  // No frequency enforcement to do if socket frequency less than or equal to DDR_2933
  //
  if (DdrFreq[Socket] <= DDR_2933) {
    return;
  }

  //
  // Look for DCPMM DIMMs that may require frequency limiting
  //
  MaxChannelDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChannelDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    if (FmcEnforceDcpmmFreqLimit (Host, Socket, Channel) == TRUE) {
      LimitFreq = TRUE;
      break;
    }
  }

  //
  // No frequency enforcement to do if no DCPMM DIMMs require it
  //
  if (LimitFreq == FALSE) {
    return;
  }

  Setup = GetSysSetupPointer ();
  if (Setup == NULL) {
    ASSERT (FALSE);
    return;
  }

  //
  // Only limit frequency if setup option is DEFAULT
  //
  if (Setup->mem.ddrFreqLimit != DDR_FREQ_AUTO) {
    return;
  }

  //
  // If all previous conditions were met, limit DDR frequency to 2933
  //
  DdrFreq[Socket] = DDR_2933;

  return;
} // EnforceDcpmmFreqLimit

/**
  Check for conditions that require DDR frequency to be overridden, apply
  overrides if conditions dictate it

  @param[in]      Host      Pointer to sysHost
  @param[in]      Socket    Socket number
  @param[in,out]  DdrFreq   DDR frequency

  @retval N/A
**/
VOID
ChipOverrideFreq (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN OUT  UINT8    *DdrFreq
  )
{
  if (DdrFreq == NULL) {
    ASSERT (FALSE);
    return;
  }

  EnforceDcpmmFreqLimit (Host, Socket, DdrFreq);

  return;

} // ChipOverrideFreq

/**

  Routine Description: Checks if lockstep is enabled

  @param Host    - Pointer to sysHost

  @retval N/A
**/
UINT8
ChipLockstepEn (
                 PSYSHOST Host
                 )
{
  if(Host->nvram.mem.RASmode & STAT_VIRT_LOCKSTEP) {
    return 1;
  } else {
    return 0;
  }
} // ChipLockstepEn

/**

  Routine Description: Checks if mirroring is enabled

  @param Host    - Pointer to sysHost

  @retval N/A
**/
UINT8
ChipMirrorEn (
                 PSYSHOST Host
                 )
{
  if(Host->nvram.mem.RASmode & CH_ALL_MIRROR) {
    return 1;
  } else {
    return 0;
  }
} // ChipMirrorEn

/**

  This function parses through the KTI var structure to detect if MCP is valid. Future Reference code will use mcpPresent

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

  @retval None

**/
VOID
CheckMCPPresent (
                 PSYSHOST Host,
                 UINT8    socket
)
{
  UINT8 stack;
  Host->nvram.mem.socket[socket].mcpPresent = 0;
  for (stack = 0; stack < MAX_IIO_STACK; stack++) {
    if (CheckStackPersonality (socket, stack, TYPE_MCP)) {
      Host->nvram.mem.socket[socket].mcpPresent = 1;
    }
  }
}

UINT8
GetMaxStrobe(
            PSYSHOST  Host,
            UINT8     socket,
            UINT8     ch,
            UINT8     dimm,
            MRC_GT    group,
            MRC_LT    level
            )
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
{
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MSVx8 = GetMaxStrobeValid(Host) / 2;

  dimmNvList = &Host->nvram.mem.socket[socket].channelList[ch].dimmList;

#ifdef LRDIMM_SUPPORT
  if ((IsLrdimmPresent (socket, ch, dimm)) && (group == TxVref) && (level == DdrLevel)) {
    return MSVx8;
  }
  if ((IsLrdimmPresent (socket, ch, dimm)) && (group == TxVref) && (level == LrbufLevel)) {
    return MSVx4;
  }
  if ((IsLrdimmPresent (socket, ch, dimm)) && (group == RxVref) && (level == LrbufLevel)) {
    //
    // DDR5 DB has two registers PG2RWF0 and PG2RWF1 that can adjust RxVref of the lower and upper nibbles
    // independently. Thus the backside RxVref acts as x4 strobe.
    //
    // DDR4 DB has one register- F5BC6x that adjusts RxVref for both lower and upper nibbles together.
    // Thus the backside RxVref acts as x8 strobe.
    //
    if (IsDdr5Present (Host, socket)) {
      return MSVx4;
    } else {
      return MSVx8;
    }
  }
#endif
  //
  // Setup variables based on x4 or x8 DIMM or chipset specific
  //
  if ((group == TxEq) || (group == TxImode) || (group == RxEq)  || (group == TxTco) || (group == RxCtleC) || (group == RxCtleR) ||
    (group == TxEqTap2) || ((group == RxVref) && (level == DdrLevel)) || (group == BcomVref) || (group == BcomDelay)) {
     return MSVx8;
  } else if ((*dimmNvList)[dimm].x4Present ||
      (group == RxDqsPDelay) || (group == RxDqsNDelay) || (group == RxDqDqsDelay) ||
      (group == RxDqDelay) || (group == RxDqsDelay) || (group == RecEnDelay) ||
      (group == TxDqDelay) || (group == RxVref) || (group == RxEq) ||
      (group == TxEq) || (group == TxEqTap2) || (group == RxFlyBy) || (group == TxFlyBy) || (group == RxSampler) || (group == TxDqRiseSlewRate) || (group == TxDqFallSlewRate)) {
    return MSVx4;
  } else {
    return MSVx8;
  }
} // GetMaxStrobe

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
GetMaxStrobeWithinSubCh(
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  MRC_GT    group,
  MRC_LT    level
  )
{
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetSubChMaxStrobeValid(Host);
  UINT8 MSVx8 = GetSubChMaxStrobeValid(Host) / 2;

  dimmNvList = &Host->nvram.mem.socket[socket].channelList[ch].dimmList;

#ifdef LRDIMM_SUPPORT
  if ((IsLrdimmPresent (socket, ch, dimm)) && (group == TxVref) && (level == DdrLevel)) {
    return MSVx8;
  }
  if ((IsLrdimmPresent (socket, ch, dimm)) && (group == TxVref) && (level == LrbufLevel)) {
    return MSVx4;
  }
  if ((IsLrdimmPresent (socket, ch, dimm)) && (group == RxVref) && (level == LrbufLevel)) {
    //
    // DDR5 DB has two registers PG2RWF0 and PG2RWF1 that can adjust RxVref of the lower and upper nibbles
    // independently. Thus the backside RxVref acts as x4 strobe.
    //
    // DDR4 DB has one register- F5BC6x that adjusts RxVref for both lower and upper nibbles together.
    // Thus the backside RxVref acts as x8 strobe.
    //
    if (IsDdr5Present (Host, socket)) {
      return MSVx4;
    } else {
      return MSVx8;
    }
  }
#endif
  //
  // Setup variables based on x4 or x8 DIMM or chipset specific
  //
  if ((group == BcomDelay) || (group == TxEq) || (group == TxImode) || (group == RxEq)  || (group == TxTco) || (group == RxCtleC) || (group == RxCtleR) ||
    (group == TxEqTap2) || ((group == RxVref && !IsCpuAndRevision (CPU_SPRSP, REV_ALL)) && (level == DdrLevel)) ||
    ((group >= DbDfeVrefDq0) && (group <= DbDfeVrefDq7)) || (group == BcomVref)) {
     return MSVx8;
  } else if ((*dimmNvList)[dimm].x4Present ||
      (group == RxDqsPDelay) || (group == RxDqsNDelay) || (group == RxDqDqsDelay) ||
      (group == RxDqDelay) || (group == RxDqsDelay) || (group == RecEnDelay) ||
      (group == TxDqDelay) || (group == RxVref) || (group == RxEq) ||
      (group == TxEq) || (group == TxEqTap2) || (group == RxFlyBy) || (group == TxFlyBy) ||
      ((group == RxVref && IsCpuAndRevision (CPU_SPRSP, REV_ALL)) && (level == DdrLevel)) ||
      group == RxSamplerEvenOdd || group == RxSamplerEven || group == RxSamplerOdd || (group == TxDqRiseSlewRate) || (group == TxDqFallSlewRate)) {
    return MSVx4;
  } else {
    return MSVx8;
  }
} // GetMaxStrobeWithinSubCh

/**
  Enable/Disable 3DS mode

  This functionality is not required on 10nm, so this function is empty

  @pararm[in]   Host          Pointer to SysHost
  @pararm[in]   Socket        Socket number
  @pararm[in]   Ch            Channel number
  @pararm[in]   Enable3DsMode Flag to enable or disable 3DS Mode
                                0 == disable
                                !0 == enable

  @retval N/A
**/
VOID
Toggle3DSMode (
  IN     PSYSHOST Host,
  IN     UINT8    Socket,
  IN     UINT8    Ch,
  IN     UINT8    Enable3DsMode
)
{
}

VOID
UpdateDdrtGroupLimits (
                       PSYSHOST Host,
                       UINT8    socket,
                       UINT8    ch,
                       UINT8    dimm,
                       MRC_LT   level,
                       MRC_GT   group,
                       UINT16   *minLimit,
                       UINT16   *maxLimit
)
{
  struct dimmNvram (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(Host, socket, ch);

  if ((*dimmNvList)[dimm].DcpmmPresent == 1 && level == LrbufLevel && group == TxVref) {
    *minLimit = 0;
    *maxLimit = 0x7f;
  }
} //UpdateDdrtGroupLimits

/**

Offsets CMD, CTL, CLK based on specified offset

@param Host      - Pointer to sysHost
@param socket    - Socket Id
@param offset    - Desired offset

@retval N/A

**/
VOID
OffsetCCC(
          PSYSHOST  Host,
          UINT8     socket,
          INT16     offset,
          UINT8     jedecinit
)
{
} // OffsetCCC

/**

This routine confirms the current DCPMM FW version meets minimum requirements
for the platform.

@param[in] Host      - Pointer to sysHost
@param[in] Socket    - Socket number
@param[in] Channel   - Channel number
@param[in] Dimm      - Dimm number
@param[in] DcpmmInfo - DCPMM DIMM information

@retval VOID

**/
VOID
CheckDcpmmFirmwareVersion (
  IN SYSHOST       *Host,
  IN UINT8          Socket,
  IN UINT8          Channel,
  IN UINT8          Dimm,
  IN FNVINFORMATION DcpmmInfo
  )
{
  //
  // Always allow test FW but warn the user
  //
  if ((DcpmmInfo->fwr.minorVersion == 0x99) ||
    (DcpmmInfo->fwr.hotfixVersion == 0x99) ||
    (DcpmmInfo->fwr.buildVersion == 0x9999)) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem running with test FW. Please update to production PMem FW\n");
    return;
  }
}

/**

This routine initializes the memory size fields in the structures
for DIMMs, Channels, and Nodes.  The sizes are calculated from the sizes in
the rank structures.

@param Host  - Pointer to sysHost

@retval VOID

**/
VOID
DisplayFnvInfo(
  IN    PSYSHOST    Host,
  IN    UINT8       Socket
)
{
#ifdef DDRT_SUPPORT
  UINT8  Ch;
  UINT8  Dimm;
  struct channelNvram    (*ChannelNvList)[MAX_CH];
  struct dimmNvram       (*DimmNvList)[MAX_DIMM];
  struct fnvInformation  FnvInfoList;
  UINT8  MbStatus = 0;
  UINT32 Status = SUCCESS;
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  // For each channel
  ChannelNvList = GetChannelNvList(Host, Socket);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    // For each DIMM
    DimmNvList = GetDimmNvList(Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      //Skip if this is not a DDRT dimm
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }

      Status = GetFnvInfo(Host, Socket, Ch, Dimm, &FnvInfoList, &MbStatus);

      if (Status == SUCCESS) {
#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Firmware Revision: %02x.%02x.%02x.%04x Firmware Type: 0x%x\n",
          FnvInfoList.fwr.majorVersion, FnvInfoList.fwr.minorVersion, FnvInfoList.fwr.hotfixVersion, FnvInfoList.fwr.buildVersion, FnvInfoList.fwrType);

        if (!(GetEmulation() & SIMICS_FLAG)) {
          CheckDcpmmFirmwareVersion (Host, Socket, Ch, Dimm, &FnvInfoList);
        }
#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK
      }
    }//Dimm
  }//Ch
#endif // DDRT_SUPPORT
}

/**
  Check if custom refresh rate selected

  @retval FAILURE -  Custom refresh rate not selected
  @retval SUCCESS -  Custom refresh rate selected
**/
UINT32
EFIAPI
IsCustomRefreshRate (
  VOID
  )
{
  UINT32 status = FAILURE;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if ((Setup->mem.customRefreshRate >= CUST_REFRESH_RATE_MIN) && (Setup->mem.customRefreshRate <= CUST_REFRESH_RATE_MAX_10NM)) {
    status = SUCCESS;
  }

  return status;
}

/**

  Stores initial sequence numbers of AEP error logs (per DIMM) to var.mem structure

  @param Host   Pointer to host structure
  @param Socket Socket index

**/
VOID
SaveAepLogInfo(
  PSYSHOST Host,
  UINT8    Socket
  )
{
#ifdef DDRT_SUPPORT
  UINT8                             Ch;
  UINT8                             Dimm;
  struct memNvram                   *NvramMem;
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  struct dimmNvram                  (*DimmNvList)[MAX_DIMM];
  UINT8                             MailboxStatus = 0;
  UINT32                            Status = SUCCESS;
  ERROR_LOG_RETURN_DATA             ReturnData;
  UINT8                             LogTypeIndex;
  UINT8                             LogLevelIndex;
  HOST_DDRT_DIMM_DEVICE_INFO_STRUCT *HostDdrtDimmInfo;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  NvramMem = &Host->nvram.mem;

  ChannelNvList = &NvramMem->socket[Socket].channelList;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = &NvramMem->socket[Socket].channelList[Ch].dimmList;

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }

      HostDdrtDimmInfo = GetHostDdrtDimmInfo (Socket, Ch, Dimm);

      for (LogTypeIndex = 0; LogTypeIndex < NGN_LOG_TYPE_NUM; LogTypeIndex++) {
        for (LogLevelIndex = 0; LogLevelIndex < NGN_LOG_LEVEL_NUM; LogLevelIndex++) {
          Status = FnvGetErrorLog (Host, Socket, Ch, Dimm, LogTypeIndex, LogLevelIndex, NGN_RETRIEVE_LOG_INFO, 0, 0,
                                   &ReturnData, &MailboxStatus);
          if (Status == SUCCESS) {
            HostDdrtDimmInfo->NgnLogSeqNum[LogTypeIndex][LogLevelIndex] = ReturnData.LogInfoData.CurrentSequenceNumber + 1;
            if (HostDdrtDimmInfo->NgnLogSeqNum[LogTypeIndex][LogLevelIndex] == 0) {
              HostDdrtDimmInfo->NgnLogSeqNum[LogTypeIndex][LogLevelIndex] = 1;
            }
            HostDdrtDimmInfo->NgnMaxLogEntries[LogTypeIndex][LogLevelIndex] = ReturnData.LogInfoData.MaxLogEntries;
          }
        }
      }
    }
  }
#endif // DDRT_SUPPORT
}


/**
  Retrieves then displays Error Log information over SMBUS mailbox per DCPMEM DIMM populated

  @param Host   Pointer to host structure
  @param Socket Socket index

**/
VOID
DcpMemErrorLogDisplay (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT8                 Ch;
  UINT8                 Dimm;
  UINT8                 LogTypeIndex;
  UINT8                 LogLevelIndex;
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT     (*DimmNvList)[MAX_DIMM];
  UINT8                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList(Host, Socket);

  // For each Channel
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    // For each DIMM
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      //Skip if this is not a DDRT dimm
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }
      //Retrieve/Display the error log info over SMBUS mailbox
      for (LogTypeIndex = 0; LogTypeIndex < NGN_LOG_TYPE_NUM; LogTypeIndex++) {
        for (LogLevelIndex = 0; LogLevelIndex < NGN_LOG_LEVEL_NUM; LogLevelIndex++) {
            FnvGetSmbusErrorLog (Host, Socket, Ch, Dimm, LogTypeIndex, LogLevelIndex);
        }
      }
    }
  }
}

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
  )
{
#ifdef DDRT_SUPPORT
  UINT8                Ch;
  UINT8                Dimm;
  UINT8                MbStatus = 0;
  UINT32               Status = SUCCESS;
  CHANNEL_NVRAM_STRUCT (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT    (*DimmNvList)[MAX_DIMM];
  DIMM_INFO_STRUCT     DimmInfoList;
  UINT8                MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ZeroMem (&DimmInfoList, sizeof(DimmInfoList));

  // For each Channel
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    // For each DIMM
    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      //Skip if this is not a DDRT dimm
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }

      Status = IdentifyDimm (Host, Socket, Ch, Dimm, AIT_DRAM_FLAG_CHECK_ENABLE, &DimmInfoList, &MbStatus);
      Host->var.mem.socket[Socket].channelList[Ch].FisVersion[Dimm] = DimmInfoList.api;
      if (Status == SUCCESS) {
        if (DimmInfoList.api >= FIS_AIT_DRAM_POLL_ENABLE) {
          (*DimmNvList)[Dimm].AitDramPoll = AIT_DRAM_POLL_ENABLE;
          DimmInfoList.AitDramPoll = AIT_DRAM_POLL_ENABLE;
        } else {
          (*DimmNvList)[Dimm].AitDramPoll = AIT_DRAM_POLL_DISABLE;
          DimmInfoList.AitDramPoll = AIT_DRAM_POLL_DISABLE;
        }
      } else {
        (*DimmNvList)[Dimm].AitDramPoll = AIT_DRAM_POLL_DISABLE;
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMem DIMM failed\n");
        EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Socket, Ch, Dimm, NO_RANK);
        DisableDIMM (Host, Socket, Ch, Dimm);
      }
    }
  }
#endif // DDRT_SUPPORT
}

VOID
ReportDcpmLog (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  )
{
  UINT8                   LogTypeIndex;
  UINT8                   LogLevelIndex;
  UINT16                  OldestSequence;
  UINT16                  CurrentSeq;
  UINT16                  MaxLogEntries;
  ERROR_LOG_RETURN_DATA   ReturnData;
  UINT8                   MailboxStatus;
  UINT32                  Status;
  UINT8                   i;

  RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                         "ReportPmemLog: ReportDcpm logs\n");

  for (LogTypeIndex = 0; LogTypeIndex < NGN_LOG_TYPE_NUM; LogTypeIndex++) {
    for (LogLevelIndex = 0; LogLevelIndex < NGN_LOG_LEVEL_NUM; LogLevelIndex++) {
      Status = FnvGetErrorLog (Host, Socket, Ch, Dimm, LogTypeIndex, LogLevelIndex,
                               NGN_RETRIEVE_LOG_INFO, 0, 0, &ReturnData, &MailboxStatus);
      if (Status != SUCCESS) {
        continue;
      }

      MaxLogEntries = ReturnData.LogInfoData.MaxLogEntries;
      CurrentSeq = ReturnData.LogInfoData.CurrentSequenceNumber;
      OldestSequence = ReturnData.LogInfoData.OldestSequenceNumber;

      while (OldestSequence < CurrentSeq) {
        Status = FnvGetErrorLog (Host, Socket, Ch, Dimm, LogTypeIndex, LogLevelIndex, NGN_RETRIEVE_LOG_DATA,
                                 OldestSequence, MaxLogEntries, &ReturnData, &MailboxStatus);
        if (Status != SUCCESS) {
          break;
        }

        FnvDisplayErrorLog (Host, Socket, Ch, Dimm, LogTypeIndex, NGN_RETRIEVE_LOG_DATA, &ReturnData);

        if (LogTypeIndex == FNV_MEDIA_LOG_REQ) {
          for (i = 0; i < ReturnData.MediaLogData.ReturnCount; i++) {
            EwlOutputType27 (WARN_NVMCTRL_MEDIA_STATUS, WARN_NVMCTRL_MEDIA_INERROR, Socket, Ch, Dimm,
                             (UINT8*)(&ReturnData.MediaLogData.MediaLog[i]));
            RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                   "Creating EWL Type26 for media log = %d\n", (OldestSequence + i));
          }
          OldestSequence += ReturnData.MediaLogData.ReturnCount;
        } else {
          for (i = 0; i < ReturnData.ThermalLogData.ReturnCount; i++) {
            EwlOutputType28 (WARN_NVMCTRL_MEDIA_STATUS, WARN_NVMCTRL_MEDIA_INERROR, Socket, Ch, Dimm,
                             (UINT8*)(&ReturnData.ThermalLogData.ThermalLog[i]));
            RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                   "Creating EWL Type27 for thermal log = %d\n", (OldestSequence + i));
          }
          OldestSequence += ReturnData.ThermalLogData.ReturnCount;
        }
      }
    }
  }
}

VOID
ReportPrevBootDcpmError (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  )
  {
  struct dimmNvram         (*DimmNvList)[MAX_DIMM];
  BOOT_STATUS_REGISTER      Bsr;
  BOOT_STATUS_HIGH_REGISTER BsrHi;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
    return;
  }

  ReadBootStatusRegisters (Host, Socket, Ch, Dimm, &Bsr, &BsrHi);
  if (FNV_PREV_BOOT_ERROR(Bsr)) {
    SetPromoteWarningException(WARN_NVMCTRL_MEDIA_STATUS, WARN_NVMCTRL_MEDIA_INERROR);
    EwlOutputType23 (WARN_NVMCTRL_MEDIA_STATUS, WARN_NVMCTRL_MEDIA_INERROR, Socket, Ch, Dimm, Bsr.Data, BsrHi.Data);
    ReportDcpmLog (Host, Socket, Ch, Dimm);
  }
}

/**
  Populate the MEM_CHIP_POLICY structure in SysHost

  @param  Host  - Pointer to SysHost structure

  @retval MRC_STATUS_SUCCESS
**/
MRC_STATUS
PopulateMemChipPolicy (
  PSYSHOST Host
  )
{

  MEM_CHIP_POLICY_DEF(maxVrefSettings)                       = MAX_VREF_SETTINGS;
  MEM_CHIP_POLICY_DEF(earlyVrefStepSize)                     = EARLY_VREF_STEP_SIZE;
  MEM_CHIP_POLICY_DEF(ctlPiGrp)                              = CTL_PI_GRP;
  MEM_CHIP_POLICY_DEF(cas2DrvenMaxGap)                       = CAS2DRVEN_MAXGAP;
  MEM_CHIP_POLICY_DEF(minIoLatency)                          = MIN_IO_LATENCY;
  MEM_CHIP_POLICY_DEF(mrcRoundTripIoComp)                    = GetIoLatCompValue ();
  MEM_CHIP_POLICY_DEF(mrcRoundTripIoCompStart)               = GetIoLatCompValueStart ();
  MEM_CHIP_POLICY_DEF(mrcRoundTripMax)                       = MRC_ROUND_TRIP_MAX_VALUE;
  MEM_CHIP_POLICY_DEF(SrMemoryDataStorageDispatchPipeCsr)    = SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR;
  MEM_CHIP_POLICY_DEF(SrMemoryDataStorageCommandPipeCsr)     = SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR;
  MEM_CHIP_POLICY_DEF(SrPbspCheckinCsr)                      = SR_PBSP_CHECKIN_CSR;
  MEM_CHIP_POLICY_DEF(SrBiosSerialDebugCsr)                  = SR_BIOS_SERIAL_DEBUG_CSR;
  MEM_CHIP_POLICY_DEF(SrPostCodeCsr)                         = SR_POST_CODE_CSR;
  MEM_CHIP_POLICY_DEF(SrErrorCodeCsr)                        = SR_ERROR_CODE_CSR;
  MEM_CHIP_POLICY_DEF(SrMemoryDataStorageDataPipeCsr)        = SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR;
  MEM_CHIP_POLICY_DEF(SrBdatStructPtrCsr)                    = SR_BDAT_STRUCT_PTR_CSR;
  MEM_CHIP_POLICY_DEF(BiosStickyScratchPad0)                 = BIOSSCRATCHPAD0_UBOX_MISC_REG;
  MEM_CHIP_POLICY_DEF(PerBitMarginDefault)                   = FeaturePcdGet (PerBitMargin);
  MEM_CHIP_POLICY_DEF(RxOdtDefault)                          = FixedPcdGet8 (PcdMrcMcOdtDefault);
  MEM_CHIP_POLICY_DEF(CmdTxEqDefault)                        = CMD_TX_EQ_DEFAULT_10NM;
  MEM_CHIP_POLICY_DEF(RxDfeDefault)                          = FixedPcdGet8 (PcdMrcRxDfeDefault);
  MEM_CHIP_POLICY_DEF(RmtColdFastBootDefault)                = RMT_COLD_FAST_BOOT_DEFAULT_10NM;
  MEM_CHIP_POLICY_DEF(RxVrefTrainingMode)                    = RX_VREF_TRAINING_MODE;
  MEM_CHIP_POLICY_DEF(TxVrefTrainingMode)                    = TX_VREF_TRAINING_MODE;
  MEM_CHIP_POLICY_DEF(MaxPhaseInReadAdjustmentDq)            = MAX_PHASE_IN_READ_ADJUSTMENT_DQ_10NM;
  MEM_CHIP_POLICY_DEF(TxRiseFallSlewRateDefault)             = FixedPcdGet8 (PcdMrcTxRfSlewRateDefault);

  PopulateMemChipPolicyTarget (Host);

  return MRC_STATUS_SUCCESS;
}

/**
  Return 0 if JEDEC init of DIMM can be skipped
  Return 1 if JEDEC init must be performed on the DIMM

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number in the socket
  @param[in] Dimm     - DIMM number in the channel

  @retval 0 - Skip DIMM initialization
  @retval 1 - Initialize the DIMM

**/
UINT8
CheckS3Jedec(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm
  )
{
  struct dimmNvram (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(Host, socket, ch);
  if (GetSysBootMode () == NormalBoot) {
    return 1;
  } else {
    //
    // Only DCPMM DIMMs require initialization in the S3 resume path
    //
    if ((*dimmNvList)[dimm].DcpmmPresent) {
      return 1;
    } else {
      return 0;
    }
  }
}

/**
  IncludeDeselects value for this project

  @retval TRUE
          FALSE
**/
UINT8
GetChipIncludeDeselectsWriteLevelingCleanUp (
  VOID
  )
{
  return TRUE;
}

/**
  Returns desired status for the input training step ID

  @param Socket        - Socket Id
  @param Training Step - Training Step Id

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
EFIAPI
GetTrainingStepEnableChip (
  IN UINT8    Socket,
  IN UINT8    TrainingStep
  )
{
  BOOLEAN  ReturnValue;
  PSYSHOST Host;

  Host = GetSysHostPointer ();

  ReturnValue = FALSE;

  switch (TrainingStep) {
    case CHECKPOINT_MINOR_T_RX_DFE:
      if ((GetRxDfeEnableChip (Host, Socket)) && (IsMemFlowEnabled (RxDfeTraining))) {
        ReturnValue = TRUE;
      }
      break;

    case CHECKPOINT_MINOR_T_TX_RISE_FALL_SLEW_RATE:
      if ((GetTxRiseFallSlewRateEnableChip (Host, Socket)) && (IsMemFlowEnabled (TxRiseFallSlewRateTraining))) {
        ReturnValue = TRUE;
      }
      break;

    case CHECKPOINT_MINOR_DDJC_TRAINING:
      if (GetDdjcEnableChip (Host, Socket)) {
        ReturnValue = TRUE;
      }
      break;

    case CHECKPOINT_MINOR_PXC_TRAINING:
      if (GetPxcEnableChip (Host, Socket)) {
        ReturnValue = TRUE;
      }
      break;

    case CHECKPOINT_MINOR_T_LRDIMM_DB_DFE:
      if (GetDataBufferDFEEnableChip (Host, Socket)) {
        ReturnValue = TRUE;
      }
      break;

    case CHECKPOINT_MINOR_DCA_DFE:
      if (GetDcaDfeEnableChip (Host, Socket)) {
        ReturnValue = TRUE;
      }
      break;

    default:
      ReturnValue = FALSE;
      break;
  }

  return ReturnValue;
} //GetTrainingStepEnableChip

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
  )
{
  BOOLEAN  ReturnValue;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  ReturnValue = FALSE;

  if (LongPreambleAllowed (Socket)) {
    if ((Setup->mem.RxDfeEn == RX_DFE_AUTO) && (Host->nvram.mem.socket[Socket].ddrFreq >= DDR_2933)) {
      ReturnValue = TRUE;
    }
    if (Setup->mem.RxDfeEn == RX_DFE_ENABLE) {
      ReturnValue = TRUE;
    }
  }

  return ReturnValue;
} //GetRxDfeEnableChip

/**
  Returns desired status for the DDJC Training feature

  @param Host          - Pointer to sysHost
  @param Socket        - Socket Id

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
GetDdjcEnableChip (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  BOOLEAN  ReturnValue;
  UINT8    DdrFrequency;

  if (IsSiliconWorkaroundEnabled ("S1408881345")) {
    return FALSE;
  }

  ReturnValue = FALSE;

  if (!EFI_ERROR (GetEncodedDdrFreq (Socket, &DdrFrequency))) {
    if ((DdrFrequency >= DDR_3200)          &&
        IsMemFlowEnabled (TxDdjcTraining)   &&
        Is2DimmPerChannelSocket (Host))
    {
      ReturnValue = TRUE;
    }
  }

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    ReturnValue = FALSE;
  }

  return ReturnValue;
} // GetDdjcEnableChip

/**
  Returns desired status for the PXC Training feature

  @param Host          - Pointer to sysHost
  @param Socket        - Socket Id

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
GetPxcEnableChip (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  BOOLEAN  ReturnValue;
  UINT8    DdrFrequency;

  if (IsSiliconWorkaroundEnabled ("S1408881345")) {
    return FALSE;
  }

  ReturnValue = FALSE;

  if (!EFI_ERROR (GetEncodedDdrFreq (Socket, &DdrFrequency))) {
    if ((DdrFrequency >= DDR_3200)                  &&
        IsMemFlowEnabled (PhaseCrosstalkTraining)   &&
        Is2DimmPerChannelSocket (Host))
    {
      ReturnValue = TRUE;
    }
  }
  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    ReturnValue = FALSE;
  }
  return ReturnValue;
} // GetPxcEnableChip

/**
  Returns desired status for DB DFE Training feature

  @param Host          - Pointer to sysHost
  @param Socket        - Socket Id

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
GetDataBufferDFEEnableChip (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  BOOLEAN    ReturnValue;
  UINT8      DdrFrequency;
  SYS_SETUP  *Setup;

  Setup = GetSysSetupPointer ();
  ReturnValue = FALSE;

  if (!EFI_ERROR (GetEncodedDdrFreq (Socket, &DdrFrequency))) {
    if ((DdrFrequency >= DDR_3200) && (Is2DimmDcpmmPerChannelSocket (Host) || (Setup->mem.DataBufferDfe == DBDFE_ALL_LRDIMM))) {
      ReturnValue = TRUE;
    }
  }
  return ReturnValue;
} // GetDataBufferDFEEnableChip

/**
  Calculate any additional delay for the DDRT CWL add value due to PXC enablement.

  @param[in]  Socket  The socket to test for PXC enablement

  @retval Any additional delay due to PXC enablement.
**/
UINT8
EFIAPI
CalculateDdrtCwlAddPxcAdder (
  IN  UINT8 Socket
  )
{
  PSYSHOST Host;
  Host = GetSysHostPointer ();
  if (GetPxcEnableChip (Host, Socket)) {
    return 1;
  } else {
    return 0;
  }
}

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
  )
{
  BOOLEAN  ReturnValue = FALSE;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (Setup->mem.TxRiseFallSlewRate == TX_RF_SLEW_RATE_ENABLE) {

    ReturnValue = TRUE;

  } else if (Setup->mem.TxRiseFallSlewRate == TX_RF_SLEW_RATE_AUTO) {

    if (Host->nvram.mem.socket[Socket].ddrFreq >= DDR_2933) {
      ReturnValue = TRUE;
    }
  }

  return ReturnValue;
}; // GetTxRiseFallSlewRateEnableChip

/**
  Increases the read/write CL and CWL, respectively

  @param Host   - Pointer to sysHost
  @param socket - Socket Id
  @param ch     - Channel number

  @retval VOID
**/
VOID
IncreaseReadWritePreamble10nm(
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  struct channelNvram (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(Host, socket);

  Host->var.mem.write2tckCWL[ch] = 0;
  Host->var.mem.read2tckCL[ch] = 0;

  if (!IsDdr5Present (Host, socket)) {
    //DDR5 does not need to adjust CL based on preamble state because of the interamble
    //DDR5 does not need to modify the write2tckCWL and read2tckCL host variables as these
    // variables are only used for ODT signal length, which is encoded into the command for
    // DDR5... so the below code only applies to DDR4

    //If Read 2tCK Preamble add 1 to CL
    if (GetPreambleState (socket, READ_PREAMBLE) > PREAMBLE_1TCLK) {
      (*channelNvList)[ch].common.nCL++;
      Host->var.mem.read2tckCL[ch]++;
    }

    //If Write 2tCK Preamble add 1 to CWL
    if (GetPreambleState (socket, WRITE_PREAMBLE)  > PREAMBLE_1TCLK) {
      (*channelNvList)[ch].common.nWL++;
      Host->var.mem.write2tckCWL[ch]++;
    }
  } //DDR4
}

#ifdef LRDIMM_SUPPORT

/**
  This function restore Data Buffer DFE Coefficient Setting

  @param[in] Host     - Pointer to sysHost
  @param[in] Channel  - Channel number

  @retval SUCCESS

**/
MRC_STATUS
EFIAPI
RestoreDBDfeCoef(
  IN  PSYSHOST    Host,
  IN  UINT8       Channel
  )
{
#ifndef DDR5_SUPPORT
  UINT8                             Tap;
  UINT8                             Socket;
  UINT8                             Dimm;
  UINT8                             Bit;
  UINT8                             *ControlWordDataPtr;
  struct dimmDevice                 (*ChdimmList)[MAX_DIMM];
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  struct dimmNvram                  (*DimmNvList)[MAX_DIMM];

  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm ++) {
    if (!IsLrdimmPresent (Socket, Channel, Dimm)) {
      continue;
    }
    //
    // Continus if DB DFE is not enabled
    //
    if (((*DimmNvList)[Dimm].Dfe_F2BCEX & DFE_ENABLE_GLOBAL) != DFE_ENABLE_GLOBAL) {
      continue;
    }

    ChdimmList  = &Host->var.mem.socket[Socket].channelList[Channel].dimmList;
    for (Bit = 0; Bit < MAX_BITS_IN_BYTE; Bit++) {
      for (Tap = 0; Tap < NumofDBDfeTapEnable; Tap ++) {
        //f2bcEx: select dq
        (*DimmNvList)[Dimm].Dfe_F2BCEX &= 0xF8;
        (*DimmNvList)[Dimm].Dfe_F2BCEX |= DFE_ENABLE_GLOBAL | DFE_WRITE_PER_DQ | Bit;
        WriteLrbuf (Host, Socket, Channel, Dimm, 0, (*DimmNvList)[Dimm].Dfe_F2BCEX, LRDIMM_F2, LRDIMM_BCEx);

        //Write tap values to all data buffers
        ControlWordDataPtr = &((*ChdimmList)[Dimm].CachedLrBuf_DFECoef[Bit][Tap][0]);
        if ((((*ChdimmList) [Dimm].CachedLrBuf_DFECoef[Bit][Tap][0]) & DFE_TAP_ENABLE) == DFE_TAP_ENABLE) {
          WriteLrbufPBA (Host, Socket, Channel, Dimm, 0, ControlWordDataPtr, LRDIMM_F3, LRDIMM_BCCx + (0x10 * Tap), ALL_DATABUFFERS);
        }
      } //Tap loop
    } // Bit loop

    (*DimmNvList)[Dimm].Dfe_F2BCEX &= 0xF0;
    WriteLrbuf (Host, Socket, Channel, Dimm, 0, (*DimmNvList)[Dimm].Dfe_F2BCEX, LRDIMM_F2,  LRDIMM_BCEx);
  } //Dimm loop

  return MRC_STATUS_SUCCESS;
#else // !DDR5_SUPPORT
  return MRC_STATUS_FAILURE;
#endif // !DDR5_SUPPORT
} // RestoreDBDfeCoef

#endif // LRDIMM_SUPPORT

#ifdef DDR5_SUPPORT
/**
  Set initial roundtrip value for the specified rank

  @param[in]  Host      - Pointer to SysHost
  @param[in]  Socket    - Socket number
  @param[in]  Ch        - Channel number
  @param[in]  SubCh     - Subchannel number
  @param[in]  Dimm      - DIMM number
  @param[in]  Rank      - Rank number

  @retval N/A
**/
VOID
SetInitRoundTripDdr5 (
  IN    PSYSHOST    Host,
  IN    UINT8       Socket,
  IN    UINT8       Ch,
  IN    UINT8       SubCh,
  IN    UINT8       Dimm,
  IN    UINT8       Rank
)
{
#ifdef DDRT_SUPPORT
  UINT8                 Cl;
  UINT8                 ClAdder;
#endif // DDRT_SUPPORT
  UINT8                 RtDelay;
  UINT8                 LogRank;
  struct channelNvram   (*ChannelNvList)[MAX_CH];
  struct dimmNvram      (*DimmNvList)[MAX_DIMM];

  ChannelNvList = GetChannelNvList(Host, Socket);
  DimmNvList = GetDimmNvList(Host, Socket, Ch);

  //
  // Get the logical rank #
  //
  LogRank = GetLogicalRank(Host, Socket, Ch, Dimm, Rank);

  //
  // Default ROUNDT_LAT = Initial Round Trip Latency Offset +  GetRoundTripStepUnit () * tCL
  //
  if (!(*DimmNvList)[Dimm].DcpmmPresent) {
    if (IsBrsPresent(Host, Socket)) {
      RtDelay = (UINT8) Host->var.mem.rtDefaultValue + GetRoundTripStepUnit () * ((*ChannelNvList)[Ch].common.nCL);
    } else {
      //
      // RTL_Addition is charatertiized by 4800 checkout
      //
      RtDelay = (UINT8) Host->var.mem.rtDefaultValue + GetRoundTripStepUnit () * ((*ChannelNvList)[Ch].common.nCL) + RTL_Addition;
    }
  } else {
    GetDdrtCasLatencies (Socket, Ch, Dimm, &Cl, &ClAdder, NULL, NULL);
    RtDelay = (UINT8)(Host->var.mem.rtDefaultValue) + GetRoundTripStepUnit () * (Cl + ClAdder);
  }

  if (RtDelay >= MEM_CHIP_POLICY_VALUE(Host, mrcRoundTripMax)) {
      RtDelay = MEM_CHIP_POLICY_VALUE(Host, mrcRoundTripMax);

      // Avoid odd roundtrip number
      if (RtDelay % 2 == 1) {
          RtDelay -= 1;
      }
  }
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubCh, Rank, NO_STROBE, NO_BIT, "\n Initial RT = %d\n", RtDelay);

  SetRoundTripDdr5 (Socket, Ch, SubCh, LogRank, RtDelay);

  SetIoLatencyDdr5 (Socket, Ch, SubCh, LogRank, MRC_IO_LATENCY_DEFAULT_VALUE);
} // SetInitRoundTripDdr5


/*++

  Initialize channel for Recieve Enable training.
  This initialization is needed each time training is performed on a new rank.

  @param Host            - Pointer to sysHost
  @param Socket          - Socket number

  @retval  N/A

--*/
VOID
ReceiveEnableInitChipDdr5 (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch
  )
{
  UINT8                             Dimm;
  UINT8                             Rank;
  UINT8                             SubCh;
  struct dimmNvram                  (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList(Host, Socket, Ch);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
      if (CheckRank(Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
        //
        // Set initial Roundtrip Latency values
        //
        SetInitRoundTripDdr5 (Host, Socket, Ch, SubCh, Dimm, Rank);
      }
    } // Rank loop
  } // Dimm loop
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    SetIoLatencyIoComp (Host, Socket, Ch, SubCh, GetIoLatCompValueStart ());
  }
} // ReceiveEnableInitChipDdr5

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
  )
{
  UINT8                             Ch;
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList(Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    ReceiveEnableInitChipDdr5 (Host, Socket, Ch);
  } // Ch loop
}

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
  )
{
  if (Step == FPT_PI_WRITE_TYPE) {
    *PiSetting     = WR_LVL_PI_START;
    *PiStart       = WR_LVL_PI_START;
    *NumberOfCycle = WR_LVL_PI_START + WR_LVL_PI_RANGE;
#ifdef LRDIMM_SUPPORT
  } else if (Step == FPT_PI_LRDIMM_RD_MRD_TYPE) {
    *PiSetting     = LRDIMM_BACKSIDE_PI_START;
    *PiStart       = LRDIMM_BACKSIDE_PI_START;
    *NumberOfCycle = LRDIMM_BACKSIDE_PI_START + LRDIMM_MRD_BACKSIDE_PI_RANGE;
#endif  //LRDIMM_SUPPORT
  } else {
    *PiSetting     = REC_EN_PI_START;
    *PiStart       = REC_EN_PI_START;
    *NumberOfCycle = REC_EN_PI_START + REC_EN_PI_RANGE;
  }
} // SetupPiSettingsParamsDdr5

#endif  // DDR5_SUPPORT

/**
  Convert DDR frequency in MT/s to QCLK ratio
  Find lowest supported max DCLK that supports the caller's input.

  @param[in]    Host        - Pointer to sysHost structure
  @param[in]    Socket      - Socket number
  @param[in]    DdrFreqMts  - The DDR frequency in MT/s to convert
  @param[out]   QclkRatio   - A pointer to the QCLK ratio for the refclk and frequency

  @retval SUCCESS if frequency encoding found
          FAILURE otherwise
**/
UINT32
EFIAPI
DdrFreqToQclkRatio (
  IN        PSYSHOST  Host,
  IN        UINT8     Socket,
  IN        UINT32    DdrFreqMts,
  OUT       UINT8     *QclkRatio
  )
{
  UINT32                              Status = FAILURE;
  UINT8                               Index;
  UINT8                               TableSize;
  UINT32                              DdrFreqRounded;
  DDR_FREQ_ENCODINGS                  *TablePtr;

  GetDdrFreqEncodingTable (Host, Socket, &TablePtr, &TableSize);

  //
  // Round input down to nearest 10
  // This allow the caller to pass in 2666 or 2667, for example, and get the same result
  //
  DdrFreqRounded = DdrFreqMts / 10;
  DdrFreqRounded *= 10;

  for (Index = 0; Index < TableSize; Index++) {
    if (DdrFreqRounded <= TablePtr[Index].DdrFreqMts) {
      *QclkRatio = TablePtr[Index].QclkRatio;
      Status = SUCCESS;
      break;
    }
  }

  return Status;
}

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
  )
{
  MRC_STATUS                          Status = MRC_STATUS_FAILURE;
  UINT8                               Index;
  UINT8                               TableSize;
  DDR_FREQ_ENCODINGS                  *TablePtr;

  GetDdrFreqEncodingTable (Host, Socket, &TablePtr, &TableSize);

  for (Index = 0; Index < TableSize; Index++) {
    if (QclkRatio <= TablePtr[Index].QclkRatio) {
      *DdrFreqMts = TablePtr[Index].DdrFreqMts;
      Status = MRC_STATUS_SUCCESS;
      break;
    }
  }

  return Status;
}

/**
  Convert DCLK ratio to MRC DDR Frequency index

  @param[in]  Host        - Pointer to sysHost
  @param[in]  Socket      - Socket number
  @param[in]  QclkRatio   - QCLK ratio
  @param[out] FreqIndex   - MRC index for the DDR frequency

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
  )
{
  UINT32                              Status = FAILURE;
  UINT8                               TableSize;
  UINT8                               Index;
  DDR_FREQ_ENCODINGS                  *TablePtr;

  GetDdrFreqEncodingTable (Host, Socket, &TablePtr, &TableSize);

  for (Index = 0; Index < TableSize; Index++) {
    if (QclkRatio == TablePtr[Index].QclkRatio) {
      *FreqIndex = TablePtr[Index].FreqIndex;
      Status = SUCCESS;
      break;
    }
  }

  return Status;
}

/**

  This function execute the write leveling Cleanup data evaluation for DDR4 and DDRT.

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
  )
{
  WriteLevelingCleanUpDataEvalTech  (Host, ByteOff, ByteSum, ByteCount, Done, CRAddDelay, WLChipCleanUpStruct, CWLC_DDR4);
  WriteLevelingCleanUpDataEvalTech  (Host, ByteOff, ByteSum, ByteCount, Done, CRAddDelay, WLChipCleanUpStruct, CWLC_DDRT);
} // WriteLevelingCleanUpDataEval

/*++

  Evaluate if DDR4/DDRT when DdrTech match the DIMM

  @param DdrTech             - Type of flow to follow for DDR Technology 0 DDR4 1 DDRT
  @param DcpmmPresent      - DDRT Dimm type
  @retval NA

--*/
BOOLEAN
DoesDdrTechMatchThisDimm (
  IN UINT8      DdrTech,
  IN UINT8      DcpmmPresent
)
{
  if (((DdrTech == CWLC_DDRT) && (DcpmmPresent)) || ((DdrTech == CWLC_DDR4) && (!DcpmmPresent))) {
    return TRUE;
  } else {
    return FALSE;
  }
} // DoesDdrTechMatchThisDimm

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
  )
{
  //
  // Stub function to satisfy the library class API,
  // should not be called in 10nm. Needs investigation
  // as to whether it should be in the library class.
  //

  RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_8);

  return;

} // SetIoLatCompMc

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
  )
{
  INT32                                   DdrtWriteLatency = 0;
#ifdef DDRT_SUPPORT
  PSYSHOST                                Host;
  T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_STRUCT DdrtDimm0BasicTiming;

  Host = (PSYSHOST) GetSysHostPointer ();
  // Note: T_DDRT_DIMM0_BASIC_TIMING is used regardless of which slot has DDRT. T_DDRT_DIMM1_BASIC_TIMING does not have
  // the t_ddrt_twl_adj or t_ddrt_twl_adj_neg bitfields, and its t_ddrt_twl bitfield has the comment: "Not used. DIMM0
  // and DIMM1 must use the same tWL."
  DdrtDimm0BasicTiming.Data = MemReadPciCfgEp (Socket, Channel, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG);
  DdrtWriteLatency = (INT32)(DdrtDimm0BasicTiming.Bits.t_ddrt_twl + DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj -
                             DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj_neg);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "tCWLDDRT (eff)[%d] = tCWLDDRT[%d] + tCWLDDRT_ADJ[%d] - tCWLDDRT_ADJ_NEG[%d]\n",
                          DdrtWriteLatency, DdrtDimm0BasicTiming.Bits.t_ddrt_twl, DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj,
                          DdrtDimm0BasicTiming.Bits.t_ddrt_twl_adj_neg);

  if (DdrtWriteLatency < DDRT_WRITE_LATENCY_MIN) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_101);
    DdrtWriteLatency = DDRT_WRITE_LATENCY_MIN;
  } else if (DdrtWriteLatency > DDRT_WRITE_LATENCY_MAX) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_102);
    DdrtWriteLatency = DDRT_WRITE_LATENCY_MAX;
  }
#endif // DDRT_SUPPORT

  return (UINT8)DdrtWriteLatency;
}

/**
  Determine if the current socket has any channel with 2 slots populated

  @param  Host    - Pointer to SysHost

  @retval   TRUE if 2 slots are populated
  @retval   FALSE otherwise

**/
BOOLEAN
Is2DimmPerChannelSocket (
  IN  PSYSHOST            Host
  )
{
  BOOLEAN             Is2Dpc = FALSE;
  UINT8               Channel;
  UINT8               Socket;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    if ((*ChannelNvList)[Channel].maxDimm > 1) {
      Is2Dpc = TRUE;
      break;
    }
  }
  return Is2Dpc;

}

/**
  Determine if the current socket has any channel with 2 slots populated and Dcpmm is populated

  @param  Host    - Pointer to SysHost

  @retval   TRUE if 2 slots are populated and one is DCPMM
  @retval   FALSE otherwise

**/
BOOLEAN
Is2DimmDcpmmPerChannelSocket (
  IN  PSYSHOST            Host
  )
{
  BOOLEAN                 Is2DpcDcpmm = FALSE;
  UINT8                   Socket;
  UINT8                   Channel;
  struct channelNvram     (*ChannelNvList)[MAX_CH];
  UINT8                   MaxChDdr;

  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    if (((*ChannelNvList)[Channel].maxDimm > 1) && ((*ChannelNvList)[Channel].ddrtEnabled == 1)) {
      Is2DpcDcpmm = TRUE;
      break;
    }
  }
  return Is2DpcDcpmm;
}

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
  )
{
  BOOLEAN             Skip = FALSE;
  BOOLEAN             DdrtPresent = FALSE;
  BOOLEAN             RdimmPresent = FALSE;
  BOOLEAN             IsDdrtAndRdimmPresent = FALSE;
  UINT8               Channel;
  UINT8               Dimm;
  UINT8               Socket;
  PSYSHOST            Host;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (IsSiliconWorkaroundEnabled ("S1909269683")) {
    Skip = TRUE;
  }

  //
  // 1. Detect if there is a BPS DIMM and LRDIMM/RDIMM mix configuration within the same channel
  // 2. If at least 2 DIMMs are populated within a channel, set the 2 DPC flag for enabling the training
  //
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    RdimmPresent = FALSE;
    DdrtPresent = FALSE;
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if (IsLrdimmPresent (Socket, Channel, Dimm) || (Host->nvram.mem.dimmTypePresent == RDIMM)) {
        RdimmPresent = TRUE;
      }

      if ((*DimmNvList)[Dimm].DcpmmPresent) {
        DdrtPresent = TRUE;
      }
    }

    if (RdimmPresent && DdrtPresent) {
      IsDdrtAndRdimmPresent = TRUE;
    }

  }

  //
  // Skip the training if not SNR processor and not DDRT and RDIMM or LRDIMM mix populated and
  // memory frequency is below 3200
  //
  if (!(IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) &&
      (!IsDdrtAndRdimmPresent || (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_2933))) {
    Skip = TRUE;
  }

  //
  // Skip the training if SNR processor and not UDIMM or SODIMM populated and
  // memory frequency is below 2933
  //
  if ((IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) &&
      (!((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM)) ||
       (Host->nvram.mem.socket[Socket].ddrFreq < DDR_2933))) {
    Skip = TRUE;
  }

  //
  // Since the training is only required when the 2 DPC flag is set,
  // disable the training if it is false
  //
  if (!Is2DimmPerChannelSocket (Host)) {
    Skip = TRUE;
  }

  return Skip;
}

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
)
{

  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT ScrambleConfig;

  // Restore scrambler
  ScrambleConfig.Data = MemReadPciCfgEp (Socket, Ch, MCSCRAMBLECONFIG_MCDDC_DP_REG);
  *PrevScrambleEnConfig = ScrambleConfig.Bits.ch_enable;
  ScrambleConfig.Bits.rx_enable = ScrambleEnConfig;
  ScrambleConfig.Bits.tx_enable = ScrambleEnConfig;
  ScrambleConfig.Bits.ch_enable = ScrambleEnConfig;
  MemWritePciCfgEp (Socket, Ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, ScrambleConfig.Data);
}

/**
  Checks if CKE signal needs to be driven on or not via CADB

  @param[in]  Host    Pointer to host structure
  @param[in]  Socket  Target socket
  @param[in]  Channel Target channel

  @retval TRUE if CKE signal needs to be driven
          FALSE if CKE signal doesn't need to be driven
**/
BOOLEAN
ShouldDriveCkeOn (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT      MiscCKECtl;

  MiscCKECtl.Data = MemReadPciCfgEp (Socket, Channel, CPGC_MISCCKECTL_MCDDC_CTL_REG);

  return ((MiscCKECtl.Bits.cke_override == 0) || (MiscCKECtl.Bits.cke_override & MiscCKECtl.Bits.cke_on));
}

/**
  Determine if the current platform has any DCPMM(Intel Optane DC Persistent Memory Module) is populated

  @retval   TRUE if any DCPMM is populated
  @retval   FALSE otherwise

**/
BOOLEAN
IsDcpmmPresentOnPlatform (
  VOID
  )
{
  PSYSHOST                  Host;
  UINT8                     Socket;
  BIOS_SCRATCHPAD7_STRUCT   BiosScratchPad7;

  Host = GetSysHostPointer ();

  for (Socket = 0; Socket < MAX_SOCKET; ++Socket) {
    if ((Host->nvram.mem.socket[Socket].enabled == 0) ||
        (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
      continue;
    }

    BiosScratchPad7.Data = UsraCsrRead (Socket, 0, BIOSSCRATCHPAD7_UBOX_MISC_REG);
    if (BiosScratchPad7.Bits.AepDimmPresent == 1) {
      return TRUE;
    }
  }
  return FALSE;
}

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
  )
{
  if (IsSiliconWorkaroundEnabled ("1707007037")) {
    return DdrtRoundTripIsNeeded (Socket, Channel);
  } else {
    return FALSE;
  }
}

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
)
{
  PSYSHOST                       Host;
  BOOLEAN                        Status = FALSE;
  UINT32                         OdtAsserted_RD_SR = 0;
  UINT32                         OdtAsserted_WR_SR = 0;
  UINT32                         OdtAsserted_RD_DR = 0;
  UINT32                         OdtAsserted_WR_DR = 0;
  UINT32                         OdtAsserted_RD_DD = 0;
  UINT32                         OdtAsserted_WR_DD = 0;
  RD_ODT_TBL0_MCDDC_CTL_STRUCT   RdOdtTable0Data;
  RD_ODT_TBL1_MCDDC_CTL_STRUCT   RdOdtTable1Data;
  WR_ODT_TBL0_MCDDC_CTL_STRUCT   WrOdtTable0Data;
  WR_ODT_TBL1_MCDDC_CTL_STRUCT   WrOdtTable1Data;

  Host = GetSysHostPointer ();
  RdOdtTable0Data.Data = MemReadPciCfgEp (Socket, Channel, RD_ODT_TBL0_MCDDC_CTL_REG);
  RdOdtTable1Data.Data = MemReadPciCfgEp (Socket, Channel, RD_ODT_TBL1_MCDDC_CTL_REG);
  WrOdtTable0Data.Data = MemReadPciCfgEp (Socket, Channel, WR_ODT_TBL0_MCDDC_CTL_REG);
  WrOdtTable1Data.Data = MemReadPciCfgEp (Socket, Channel, WR_ODT_TBL1_MCDDC_CTL_REG);

  OdtAsserted_RD_SR = (RdOdtTable0Data.Bits.rd_odt_rank0 & BIT0) | (RdOdtTable0Data.Bits.rd_odt_rank1 & BIT1) |
                      (RdOdtTable1Data.Bits.rd_odt_rank4 & BIT2) | (RdOdtTable1Data.Bits.rd_odt_rank5 & BIT3);

  OdtAsserted_WR_SR = (WrOdtTable0Data.Bits.wr_odt_rank0 & BIT0) | (WrOdtTable0Data.Bits.wr_odt_rank1 & BIT1) |
                      (WrOdtTable1Data.Bits.wr_odt_rank4 & BIT0) | (WrOdtTable1Data.Bits.wr_odt_rank5 & BIT1);

  OdtAsserted_RD_DR = (RdOdtTable0Data.Bits.rd_odt_rank0 & (BIT0 | BIT1)) | (RdOdtTable0Data.Bits.rd_odt_rank1 & (BIT0 | BIT1)) |
                      (RdOdtTable0Data.Bits.rd_odt_rank2 & (BIT0 | BIT1)) | (RdOdtTable0Data.Bits.rd_odt_rank3 & (BIT0 | BIT1)) |
                      (RdOdtTable1Data.Bits.rd_odt_rank4 & (BIT2 | BIT3)) | (RdOdtTable1Data.Bits.rd_odt_rank5 & (BIT2 | BIT3)) |
                      (RdOdtTable1Data.Bits.rd_odt_rank6 & (BIT2 | BIT3)) | (RdOdtTable1Data.Bits.rd_odt_rank7 & (BIT2 | BIT3));

  OdtAsserted_WR_DR = (WrOdtTable0Data.Bits.wr_odt_rank0 & (BIT0 | BIT1)) | (WrOdtTable0Data.Bits.wr_odt_rank1 & (BIT0 | BIT1)) |
                      (WrOdtTable0Data.Bits.wr_odt_rank2 & (BIT0 | BIT1)) | (WrOdtTable0Data.Bits.wr_odt_rank3 & (BIT0 | BIT1)) |
                      (WrOdtTable1Data.Bits.wr_odt_rank4 & (BIT2 | BIT3)) | (WrOdtTable1Data.Bits.wr_odt_rank5 & (BIT2 | BIT3)) |
                      (WrOdtTable1Data.Bits.wr_odt_rank6 & (BIT2 | BIT3)) | (WrOdtTable1Data.Bits.wr_odt_rank7 & (BIT2 | BIT3));

  OdtAsserted_RD_DD = (RdOdtTable0Data.Bits.rd_odt_rank0 & (BIT0 | BIT1 | BIT2 | BIT3)) | (RdOdtTable0Data.Bits.rd_odt_rank1 & (BIT0 | BIT1 | BIT2 | BIT3)) |
                      (RdOdtTable0Data.Bits.rd_odt_rank2 & (BIT0 | BIT1 | BIT2 | BIT3)) | (RdOdtTable0Data.Bits.rd_odt_rank3 & (BIT0 | BIT1 | BIT2 | BIT3)) |
                      (RdOdtTable1Data.Bits.rd_odt_rank4 & (BIT0 | BIT1 | BIT2 | BIT3)) | (RdOdtTable1Data.Bits.rd_odt_rank5 & (BIT0 | BIT1 | BIT2 | BIT3)) |
                      (RdOdtTable1Data.Bits.rd_odt_rank6 & (BIT0 | BIT1 | BIT2 | BIT3)) | (RdOdtTable1Data.Bits.rd_odt_rank7 & (BIT0 | BIT1 | BIT2 | BIT3));

  OdtAsserted_WR_DD = (WrOdtTable0Data.Bits.wr_odt_rank0 & (BIT0 | BIT1 | BIT2 | BIT3)) | (WrOdtTable0Data.Bits.wr_odt_rank1 & (BIT0 | BIT1 | BIT2 | BIT3)) |
                      (WrOdtTable0Data.Bits.wr_odt_rank2 & (BIT0 | BIT1 | BIT2 | BIT3)) | (WrOdtTable0Data.Bits.wr_odt_rank3 & (BIT0 | BIT1 | BIT2 | BIT3)) |
                      (WrOdtTable1Data.Bits.wr_odt_rank4 & (BIT0 | BIT1 | BIT2 | BIT3)) | (WrOdtTable1Data.Bits.wr_odt_rank5 & (BIT0 | BIT1 | BIT2 | BIT3)) |
                      (WrOdtTable1Data.Bits.wr_odt_rank6 & (BIT0 | BIT1 | BIT2 | BIT3)) | (WrOdtTable1Data.Bits.wr_odt_rank7 & (BIT0 | BIT1 | BIT2 | BIT3));

  //
  // ODT assertion check
  //

  switch (Selection) {
    case SameRank:
      if ((OdtAsserted_RD_SR | OdtAsserted_WR_SR) != 0){
        Status = TRUE;
      }
      break;
    case DiffRank:
      if ((OdtAsserted_RD_DR | OdtAsserted_WR_DR) != 0){
        Status = TRUE;
      }
      break;
    case DiffDimm:
      if ((OdtAsserted_RD_DD | OdtAsserted_WR_DD) != 0){
        Status = TRUE;
      }
      break;
    default:
      Status = FALSE;
      break;
  }

  return Status;
}

/**
  Get read to read timing value based on the enum parameter passed


  @param[in]  Socket           - Socket Number
  @param[in]  Ch               - Ch Number
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
  )
{
  MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_STRUCT TCas2CasDr = {0};
  MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_STRUCT TCas2CasDd = {0};
  EFI_STATUS Status = EFI_SUCCESS;

  if ((TimingType & (UINT8)TRrddType) != 0) {

    TCas2CasDd.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_REG);
    ReadtoReadTiming->TRrdd = (UINT8)TCas2CasDd.Bits.t_rrdd;
  }

  if ((TimingType & TRrdrType) != 0) {

    TCas2CasDr.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_REG);
    ReadtoReadTiming->TRrdr = (UINT8)TCas2CasDr.Bits.t_rrdr;
  }


  return Status;
}

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
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_STRUCT TCas2CasDr = {0};
  MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_STRUCT TCas2CasDd = {0};

  if ((TimingType & TRrddType) != 0) {

    TCas2CasDd.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_REG);
    TCas2CasDd.Bits.t_rrdd = ReadtoReadTiming->TRrdd;
    MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DD_MCDDC_CTL_REG, TCas2CasDd.Data);
  }
  if ((TimingType & TRrdrType) != 0) {
    TCas2CasDr.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_REG);
    TCas2CasDr.Bits.t_rrdr = ReadtoReadTiming->TRrdr;
    MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_CAS2CAS_DR_MCDDC_CTL_REG, TCas2CasDr.Data);
  }

  return Status;
}

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
  )
{

  //
  // No op on 10nm
  //

  return;

} // ReadChipWrADDDelays

/**
  Returns desired status for the DCA DFE Training feature

  @param[in] Host         - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket       - Socket number

  @retval TRUE if enabled, FALSE if disabled

**/
BOOLEAN
GetDcaDfeEnableChip (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{

  if (IsDdr5Present (Host, Socket) &&
      Is2DimmPerChannelSocket (Host) &&
      (!(IsBrsPresent (Host, Socket)))) {
    //
    // Enable DCA DFE for DDR5 2DPC Config, not include BRS.
    //
    return TRUE;
  } else {
    return FALSE;
  }
} // GetDcaDfeEnableChip
