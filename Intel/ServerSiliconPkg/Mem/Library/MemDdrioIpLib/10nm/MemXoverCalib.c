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
#include <Memory/CpgcDefinitions.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemDdrIoIpTargetLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include "Include/MemCmdControl.h"
#include "Include/MemIoControl.h"
#include <Include/MemXoverCalib.h>
#include <Library/CheckpointLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/PerformanceTrackerLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include "Include/MemDdrioPrivate.h"

//
// Workaround offset table per DQ signal group as per S1409677997
//                Strobe     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17
UINT8 RecEnXoverOffset[] = { 0,  0,  0,  0, 58, 58, 58, 58,  0, 58, 58, 58, 58,  0,  0,  0,  0, 58 };
UINT8 TxDqsXoverOffset[] = { 0,  0,  0,  0, 52, 52, 52, 52,  0, 52, 52, 52, 52,  0,  0,  0,  0, 52 };
UINT8 TxDqXoverOffset[]  = {53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 };

#if (MEM_IP_VER >= MEM_IP_17ww37e)

/**

  Enable or Disable crossover calibration.

  @param Host   - Pointer to sysHost, the system Host (root) structure struct
  @param Socket - Current CPU socket
  @param Ch     - Current channel
  @param State  - Enable = 1, Disable = 0

  @retval SUCCESS

**/
EFI_STATUS
EnableDisableXoverCalRegs (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    State
  )
{
  UINT8                                           Strobe;
  DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_STRUCT      ddrCRCmdControls;
  DDRCRCLKCONTROLS_MCIO_DDRIOEXT_STRUCT           ddrCRClkControls;
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT              dataControl0;
  UINT8                                           MSVx4 = GetMaxStrobeValid (Host);
  struct channelNvram(*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);
  if (IsBrsPresent (Host, Socket)) {
    MSVx4 = MAX_STROBE;
  }
  ddrCRCmdControls.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
  ddrCRCmdControls.Bits.xovercal = State;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

  ddrCRCmdControls.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CKE_MCIO_DDRIOEXT_REG);
  ddrCRCmdControls.Bits.xovercal = State;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

  ddrCRCmdControls.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG);
  ddrCRCmdControls.Bits.xovercal = State;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

  ddrCRCmdControls.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CTL_MCIO_DDRIOEXT_REG);
  ddrCRCmdControls.Bits.xovercal = State;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

  ddrCRClkControls.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
  ddrCRClkControls.Bits.xovercal = State;
  MemWritePciCfgEp (Socket, Ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, ddrCRClkControls.Data | (*ChannelNvList)[Ch].ddrCRClkControls);

  dataControl0.Data = 0;
  dataControl0.Bits.xovercal = State;
  for (Strobe = 0;Strobe < MSVx4;Strobe++) {
    WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data);
  } // Strobe loop

  return (EFI_SUCCESS);
} // EnableDisableXoverCalRegs

/**

  Initialize registers used during crossover calibration.

  @param Host                   - Pointer to sysHost, the system Host (root) structure struct
  @param Ch                     - Current channel
  @param COUNT1_STRUCT          - Structure containing Count1 variables
  @param OFFSET_STRUCT          - Structure containing offset variables
  @param PREVIOUS_STATUS_STRUCT - Structure containing previous status variables

  @retval SUCCESS

**/
EFI_STATUS
InitializeLocalsPerCh (
  IN  PSYSHOST                Host,
  IN  UINT8                   Ch,
  OUT COUNT1_STRUCT           *Count1,
  OUT OFFSET_STRUCT           *OffsetSt,
  OUT PREVIOUS_STATUS_STRUCT  *PreviousStatus,
  OUT RESULTS_STRUCT          *Results
  )
{
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  if (IsBrsPresent (Host, Host->var.mem.currentSocket)) {
    MSVx4 = MAX_STROBE;
  }
  Count1->CmdN = 0;
  Count1->CmdS = 0;
  Count1->Ctl  = 0;
  Count1->Cke  = 0;
  Count1->Clk  = 0;
  ZeroMem (Count1->Strobe, MSVx4);

  OffsetSt->CmdN[Ch] = 0;
  OffsetSt->CmdS[Ch] = 0;
  OffsetSt->Ctl[Ch] = 0;
  OffsetSt->Cke[Ch] = 0;
  OffsetSt->Clk[Ch] = 0;
  ZeroMem (OffsetSt->Offset[Ch], MSVx4 * sizeof(UINT16));

  PreviousStatus->CmdN[Ch] = 0;
  PreviousStatus->CmdS[Ch] = 0;
  PreviousStatus->Ctl[Ch] = 0;
  PreviousStatus->Cke[Ch] = 0;
  PreviousStatus->Clk[Ch] = 0;
  ZeroMem (PreviousStatus->Data[Ch], MSVx4);


  return (EFI_SUCCESS);
} // InitializeLocalsPerCh

/**

  Set piDelay value to test crossover calibration.

  @param Host       - Pointer to sysHost, the system Host (root) structure struct
  @param Socket     - Current CPU socket
  @param Ch         - Current channel
  @param PiDelay    - PiDelay

  @retval SUCCESS

**/
EFI_STATUS
SetCmdRefPiClkDelay (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT16   PiDelay
  )
{
  UINT8                                           Strobe;
  DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_STRUCT     DdrCrCmdControls3;
  DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_STRUCT          DdrCrClkRanksUsed;
  DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_STRUCT     DdrCrCmdNPiCoding4;
  DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_STRUCT     DdrCrCmdSPiCoding4;
  DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_STRUCT      DdrCrCtlPiCoding4;
  DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_STRUCT      DdrCrCkePiCoding4;
  DATACONTROL4N0_0_MCIO_DDRIO_STRUCT              DataControl4;
  UINT8                                           MSVx4 = GetMaxStrobeValid (Host);
  struct channelNvram(*ChannelNvList)[MAX_CH];
  if (IsBrsPresent (Host, Socket)) {
    MSVx4 = MAX_STROBE;
  }
  ChannelNvList = GetChannelNvList (Host, Socket);

  DdrCrCmdControls3.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG);
  DdrCrCmdControls3.Bits.cmdrefpiclkdelay = PiDelay;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG, DdrCrCmdControls3.Data);

  DdrCrCmdControls3.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG);
  DdrCrCmdControls3.Bits.cmdrefpiclkdelay = PiDelay;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG, DdrCrCmdControls3.Data);

  DdrCrCmdControls3.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG);
  DdrCrCmdControls3.Bits.cmdrefpiclkdelay = PiDelay;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG, DdrCrCmdControls3.Data);

  DdrCrCmdControls3.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG);
  DdrCrCmdControls3.Bits.cmdrefpiclkdelay = PiDelay;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG, DdrCrCmdControls3.Data);

  DdrCrCmdNPiCoding4.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_REG);
  DdrCrCmdNPiCoding4.Bits.cmdrefpi0picode = PiDelay;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_REG, DdrCrCmdNPiCoding4.Data);

  DdrCrCmdSPiCoding4.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_REG);
  DdrCrCmdSPiCoding4.Bits.cmdrefpi0picode = PiDelay;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_REG, DdrCrCmdSPiCoding4.Data);

  DdrCrCtlPiCoding4.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_REG);
  DdrCrCtlPiCoding4.Bits.cmdrefpi0picode = PiDelay;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_REG, DdrCrCtlPiCoding4.Data);

  DdrCrCkePiCoding4.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_REG);
  DdrCrCkePiCoding4.Bits.cmdrefpi0picode = PiDelay;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_REG, DdrCrCkePiCoding4.Data);

  DdrCrClkRanksUsed.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
  DdrCrClkRanksUsed.Bits.refpiclk = PiDelay;
  MemWritePciCfgEp (Socket, Ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, DdrCrClkRanksUsed.Data);

  for (Strobe = 0;Strobe < MSVx4;Strobe++) {
    DataControl4.Data = (*ChannelNvList)[Ch].dataControl4[Strobe];
    DataControl4.Bits.refpiclkdelay = PiDelay;
    WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, DataControl4.Data);
  } // Strobe loop

  return (EFI_SUCCESS);
} // SetCmdRefPiClkDelay

/**

  Get multiple samples based on the current piDelay value used to test crossover calibration.

  @param Host             - Pointer to sysHost, the system Host (root) structure struct
  @param Socket           - Current CPU socket
  @param Ch               - Current channel
  @param PiDelay          - PiDelay
  @param Results          - Pointer to the struct containing the results of crossover calibration
  @param PiRef0Results    - Pointer to the struct containing the results of crossover calibration

  @retval SUCCESS

**/
EFI_STATUS
GetCrossoverCalibrationResults (
  IN  PSYSHOST             Host,
  IN  UINT8                Socket,
  IN  UINT8                Ch,
  IN  UINT16               PiDelay,
  OUT RESULTS_STRUCT       *Results,
  OUT RESULTS_STRUCT       *PiRef0Results
  )
{
  UINT8                                           Strobe;
  UINT32                                          FeedBack;
  UINT8                                           SampleCounter;
  DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_STRUCT      DdrCrCmdNTraining;
  DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_STRUCT      DdrCrCmdSTraining;
  DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_STRUCT       DdrCrCtlTraining;
  DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_STRUCT       DdrCrCkeTraining;
  DDRCRCLKTRAINING_MCIO_DDRIOEXT_STRUCT           DdrCrClkTraining;
  SAMPLE_COUNT_STRUCT                             SampleCnt;
  SAMPLE_COUNT_STRUCT                             PiRef0SampleCnt;
  UINT8                                           MSVx4 = GetMaxStrobeValid (Host);

  if (IsBrsPresent (Host, Socket)) {
    MSVx4 = MAX_STROBE;
  }
  SampleCnt.CmdN = 0;
  SampleCnt.CmdS = 0;
  SampleCnt.Cke  = 0;
  SampleCnt.Ctl  = 0;
  SampleCnt.Clk  = 0;
  ZeroMem (SampleCnt.Data, MSVx4);
  PiRef0SampleCnt.CmdN = 0;
  PiRef0SampleCnt.CmdS = 0;
  PiRef0SampleCnt.Cke  = 0;
  PiRef0SampleCnt.Ctl  = 0;
  PiRef0SampleCnt.Clk  = 0;
  ZeroMem (PiRef0SampleCnt.Data, MSVx4);

  for (SampleCounter = 0;SampleCounter < XOVER_CALIB_SAMPLE_COUNT;SampleCounter++) {
    CteDelayQclk (64);

    DdrCrCmdNTraining.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG);
    DdrCrCmdSTraining.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG);
    DdrCrCtlTraining.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG);
    DdrCrCkeTraining.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG);

    RcSimCmdXoverFeedbackTarget (
      PiDelay,
      (VOID *)&DdrCrCmdNTraining,
      (VOID *)&DdrCrCmdSTraining,
      (VOID *)&DdrCrCtlTraining,
      (VOID *)&DdrCrCkeTraining
      );

    if (DdrCrCmdNTraining.Bits.xoverphasedet == 1) {
      SampleCnt.CmdN++;
    }
    if (DdrCrCmdSTraining.Bits.xoverphasedet == 1) {
      SampleCnt.CmdS++;
    }
    if (DdrCrCtlTraining.Bits.xoverphasedet == 1) {
      SampleCnt.Ctl++;
    }
    if (DdrCrCkeTraining.Bits.xoverphasedet == 1) {
      SampleCnt.Cke++;
    }
    // Handle piref0xoverphasedet (bit 28)
    if (DdrCrCmdNTraining.Bits.piref0xoverphasedet == 1) {
      PiRef0SampleCnt.CmdN++;
    }
    if (DdrCrCmdSTraining.Bits.piref0xoverphasedet == 1) {
      PiRef0SampleCnt.CmdS++;
    }
    if (DdrCrCtlTraining.Bits.piref0xoverphasedet == 1) {
      PiRef0SampleCnt.Ctl++;
    }
    if (DdrCrCkeTraining.Bits.piref0xoverphasedet == 1) {
      PiRef0SampleCnt.Cke++;
    }
    DdrCrClkTraining.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG);
    GetClkXoverPhaseDetFeedbackTarget (PiDelay, (VOID *)&DdrCrClkTraining);

    if (DdrCrClkTraining.Bits.clkxoverphasedet) {
      PiRef0SampleCnt.Clk++;
    }

    for (Strobe = 0;Strobe < MSVx4;Strobe++) {
      FeedBack = ReadDdrioDataRegCh (Host, Socket, Ch, Strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG);

      RcSimDataXoverFeedbackTarget (PiDelay, Strobe, &FeedBack);

      if (FeedBack & 1) {
        PiRef0SampleCnt.Data[Strobe]++;
      }
    } // Strobe loop
  } // for (SampleCounter = 0; SampleCounter < XOVER_CALIB_SAMPLE_COUNT; SampleCounter++)
    //
    // Check to see if each counter is at least half of the total sample count
    //
  if ((SampleCnt.CmdN * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
    Results->CmdN = 1;
  } else {
    Results->CmdN = 0;
  }
  if ((SampleCnt.CmdS * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
    Results->CmdS = 1;
  } else {
    Results->CmdS = 0;
  }
  if ((SampleCnt.Cke * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
    Results->Cke = 1;
  } else {
    Results->Cke = 0;
  }
  if ((SampleCnt.Ctl * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
    Results->Ctl = 1;
  } else {
    Results->Ctl = 0;
  }

  if ((PiRef0SampleCnt.CmdN * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
    PiRef0Results->CmdN = 1;
  } else {
    PiRef0Results->CmdN = 0;
  }
  if ((PiRef0SampleCnt.CmdS * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
    PiRef0Results->CmdS = 1;
  } else {
    PiRef0Results->CmdS = 0;
  }
  if ((PiRef0SampleCnt.Cke * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
    PiRef0Results->Cke = 1;
  } else {
    PiRef0Results->Cke = 0;
  }
  if ((PiRef0SampleCnt.Ctl * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
    PiRef0Results->Ctl = 1;
  } else {
    PiRef0Results->Ctl = 0;
  }

  if ((PiRef0SampleCnt.Clk * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
    PiRef0Results->Clk = 1;
  } else {
    PiRef0Results->Clk = 0;
  }

  for (Strobe = 0;Strobe < MSVx4;Strobe++) {
    if ((PiRef0SampleCnt.Data[Strobe] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
      PiRef0Results->Data[Strobe] = 1;
    } else {
      PiRef0Results->Data[Strobe] = 0;
    }
  }
  return (EFI_SUCCESS);
} // GetCrossoverCalibrationResults

/**

  Display the results from crossover calibration for this PiDelay.

  @param Host                 - Pointer to sysHost, the system Host (root) structure struct
  @param Ch                   - Current channel
  @param PiDelay              - PiDelay
  @param Results              - Pointer to the struct containing the results of crossover calibration
  @param PiRef0Results        - Pointer to the struct containing the results of crossover calibration
  @param Offset               - Pointer to the struct containing the offsets for crossover calibration
  @param PiRef0OffsetSt       - Pointer to the struct containing the offsets for crossover calibration
  @param Count1               - Pointer to the struct containing the Count of positive results
  @param PiRef0Count1         - Pointer to the struct containing the Count of positive results
  @param PreviousStatus       - Pointer to the struct containing the previous offset for crossover calibration
  @param PiRef0PreviousStatus - Pointer to the struct containing the previous offset for crossover calibration

  @retval SUCCESS

**/
EFI_STATUS
DisplayCrossoverCalibrationResults (
  IN  PSYSHOST                 Host,
  IN  UINT8                    Ch,
  IN  UINT16                   PiDelay,
  IN  RESULTS_STRUCT           *Results,
  IN  RESULTS_STRUCT           *PiRef0Results,
  IN  OFFSET_STRUCT            *OffsetSt,
  IN  OFFSET_STRUCT            *PiRef0OffsetSt,
  OUT COUNT1_STRUCT            *Count1,
  OUT COUNT1_STRUCT            *PiRef0Count1,
  OUT PREVIOUS_STATUS_STRUCT   *PreviousStatus,
  OUT PREVIOUS_STATUS_STRUCT   *PiRef0PreviousStatus
  )
{
  UINT8                                           Strobe;
  UINT8                                           MSVx4 = GetMaxStrobeValid (Host);

  if (IsBrsPresent (Host, Host->var.mem.currentSocket)) {
    MSVx4 = MAX_STROBE;
  }
  RcDebugPrint (SDBG_MAX," ");
  if (Results->CmdN == 1) {
    Count1->CmdN++;
    RcDebugPrint (SDBG_MAX,
                    "1");
  } else {
    RcDebugPrint (SDBG_MAX,
                    "0");
  }
  if ((PreviousStatus->CmdN[Ch] == 0) & (Results->CmdN != 0)) {
    if (PiDelay > OffsetSt->CmdN[Ch]) {
      OffsetSt->CmdN[Ch] = PiDelay;
    }
  }
  PreviousStatus->CmdN[Ch] = Results->CmdN;
  RcDebugPrint (SDBG_MAX,"        ");

  if (PiRef0Results->CmdN == 1) {
    PiRef0Count1->CmdN++;
    RcDebugPrint (SDBG_MAX,
                    "1");
  } else {
    RcDebugPrint (SDBG_MAX,
                    "0");
  }
  if ((PiRef0PreviousStatus->CmdN[Ch] != 0) & (PiRef0Results->CmdN == 0)) {
    if (PiDelay > PiRef0OffsetSt->CmdN[Ch]) {
      PiRef0OffsetSt->CmdN[Ch] = PiDelay;
    }
  }
  PiRef0PreviousStatus->CmdN[Ch] = PiRef0Results->CmdN;
  RcDebugPrint (SDBG_MAX,"      ");

  if (Results->CmdS == 1) {
    Count1->CmdS++;
    RcDebugPrint (SDBG_MAX,
                    "1");
  } else {
    RcDebugPrint (SDBG_MAX,
                    "0");
  }
  if ((PreviousStatus->CmdS[Ch] == 0) & (Results->CmdS != 0)) {
    if (PiDelay > OffsetSt->CmdS[Ch]) {
      OffsetSt->CmdS[Ch] = PiDelay;
    }
  }
  PreviousStatus->CmdS[Ch] = Results->CmdS;
  RcDebugPrint (SDBG_MAX,"        ");

  if (PiRef0Results->CmdS == 1) {
    PiRef0Count1->CmdS++;
    RcDebugPrint (SDBG_MAX,
                    "1");
  } else {
    RcDebugPrint (SDBG_MAX,
                    "0");
  }

  if ((PiRef0PreviousStatus->CmdS[Ch] != 0) & (PiRef0Results->CmdS == 0)) {
    if (PiDelay > PiRef0OffsetSt->CmdS[Ch]) {
      PiRef0OffsetSt->CmdS[Ch] = PiDelay;
    }
  }
  PiRef0PreviousStatus->CmdS[Ch] = PiRef0Results->CmdS;
  RcDebugPrint (SDBG_MAX,"      ");

  if (Results->Ctl == 1) {
    Count1->Ctl++;
    RcDebugPrint (SDBG_MAX,
                    "1");
  } else {
    RcDebugPrint (SDBG_MAX,
                    "0");
  }
  if ((PreviousStatus->Ctl[Ch] == 0) & (Results->Ctl != 0)) {
    if (PiDelay > OffsetSt->Ctl[Ch]) {
      OffsetSt->Ctl[Ch] = PiDelay;
    }
  }
  PreviousStatus->Ctl[Ch] = Results->Ctl;
  RcDebugPrint (SDBG_MAX,"      ");

  if (PiRef0Results->Ctl == 1) {
    PiRef0Count1->Ctl++;
    RcDebugPrint (SDBG_MAX,
                    "1");
  } else {
    RcDebugPrint (SDBG_MAX,
                    "0");
  }
  if ((PiRef0PreviousStatus->Ctl[Ch] != 0) & (PiRef0Results->Ctl == 0)) {
    if (PiDelay > PiRef0OffsetSt->Ctl[Ch]) {
      PiRef0OffsetSt->Ctl[Ch] = PiDelay;
    }
  }
  PiRef0PreviousStatus->Ctl[Ch] = PiRef0Results->Ctl;
  RcDebugPrint (SDBG_MAX,"      ");

  if (Results->Cke == 1) {
    Count1->Cke++;
    RcDebugPrint (SDBG_MAX,
                    "1");
  } else {
    RcDebugPrint (SDBG_MAX,
                    "0");
  }
  if ((PreviousStatus->Cke[Ch] == 0) & (Results->Cke != 0)) {
    if (PiDelay > OffsetSt->Cke[Ch]) {
      OffsetSt->Cke[Ch] = PiDelay;
    }
  }
  PreviousStatus->Cke[Ch] = Results->Cke;
  RcDebugPrint (SDBG_MAX,"      ");

  if (PiRef0Results->Cke == 1) {
    PiRef0Count1->Cke++;
    RcDebugPrint (SDBG_MAX,
                    "1");
  } else {
    RcDebugPrint (SDBG_MAX,
                    "0");
  }
  if ((PiRef0PreviousStatus->Cke[Ch] != 0) & (PiRef0Results->Cke == 0)) {
    if (PiDelay > PiRef0OffsetSt->Cke[Ch]) {
      PiRef0OffsetSt->Cke[Ch] = PiDelay;
    }
  }
  PiRef0PreviousStatus->Cke[Ch] = PiRef0Results->Cke;
  RcDebugPrint (SDBG_MAX,"      ");

  if (PiRef0Results->Clk == 1) {
    PiRef0Count1->Clk++;
    RcDebugPrint (SDBG_MAX,
                    "1");
  } else {
    RcDebugPrint (SDBG_MAX,
                    "0");
  }
  if ((PiRef0PreviousStatus->Clk[Ch] != 0) & (PiRef0Results->Clk == 0)) {
    if (PiDelay > PiRef0OffsetSt->Clk[Ch]) {
      PiRef0OffsetSt->Clk[Ch] = PiDelay;
    }
  }
  PiRef0PreviousStatus->Clk[Ch] = PiRef0Results->Clk;
  RcDebugPrint (SDBG_MAX,"   ");

  for (Strobe = 0;Strobe < MSVx4;Strobe++) {

    if (PiRef0Results->Data[Strobe]) {
      PiRef0Count1->Strobe[Strobe]++;
      RcDebugPrint (SDBG_MAX,
                      "1  ");
    } else {
      RcDebugPrint (SDBG_MAX,
                      "0  ");
    }

    // Check if previous was one and current is 0
    if (PiRef0PreviousStatus->Data[Ch][Strobe] && (PiRef0Results->Data[Strobe] == 0)) {
      if (PiDelay > PiRef0OffsetSt->Offset[Ch][Strobe]) {
        PiRef0OffsetSt->Offset[Ch][Strobe] = PiDelay;
      }
    }

    PiRef0PreviousStatus->Data[Ch][Strobe] = PiRef0Results->Data[Strobe];
  }

  RcDebugPrint (SDBG_MAX,
                  "\n");

  return (EFI_SUCCESS);
} // DisplayCrossoverCalibrationResults


/**
  Function to implement Silicon Woraround S1409677997
  Adds offset to incoming training offset.
  This offset should be applied to
  DataOffsetTrain.Bits.rcvenoffset
  DataOffsetTrain.Bits.txdqoffset
  DataOffsetTrain.Bits.txdqsoffset

  @param[in]       Group - Signal group
  @param[in]       Strobe - Strobe number
  @param[in, out] *OffsetValue - Pointer to Training Offset value

  @retval N/A
**/
VOID
SiliconWorkaroundS1409677997 (
  IN MRC_GT      Group,
  IN UINT8       Strobe,
  IN OUT UINT16 *OffsetValue
)
{
  if (IsSiliconWorkaroundEnabled ("S1409677997")) {
    UINT8 TrainOffset;
    
    switch (Group)
    {
    case RecEnDelay:
      TrainOffset = RecEnXoverOffset[Strobe];
      break;
    case TxDqsDelay:
      TrainOffset = TxDqsXoverOffset[Strobe];
      break;
    case TxDqDelay:
      TrainOffset = TxDqXoverOffset[Strobe];
      break;
    default:
      TrainOffset = 0;
    }

    *OffsetValue += TrainOffset;
  }
}
/**

  Program the offsets found during crossover calibration.

  @param Host             - Pointer to sysHost, the system Host (root) structure struct
  @param Socket           - Current CPU socket
  @param Ch               - Current channel
  @param OffsetSt         - Pointer to the struct containing the offsets found during crossover calibration
  @param PiRef0OffsetSt   - Pointer to the struct containing the offsets found during crossover calibration

  @retval SUCCESS

**/
EFI_STATUS
ProgramOffsets (
  IN  PSYSHOST             Host,
  IN  UINT8                Socket,
  IN  UINT8                Ch,
  IN  OFFSET_STRUCT        *OffsetSt,
  IN  OFFSET_STRUCT        *PiRef0OffsetSt
  )
{
  UINT8                                           Strobe;
  UINT8                                           RegIndex;
  UINT8                                           MSVx4;
  UINT16                                          PiDelay;
  UINT16                                          PiRef0PiDelay;
  UINT16                                          RecEnOffsetValue = 0;
  UINT16                                          TxDqsOffsetValue = 0;
  UINT16                                          TxDqOffsetValue  = 0;
  XOVER_OFFSETS                                   *XoverOffsets;
  struct channelNvram                             (*ChannelNvList)[MAX_CH];
  DATAOFFSETTRAINN0_0_MCIO_DDRIO_STRUCT           DataOffsetTrain;
  DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_STRUCT      DdrCrCmdNTraining;
  DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_STRUCT      DdrCrCmdSTraining;
  DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_STRUCT      DdrCRCmdSPiCoding;
  DDRCRCMDPICODING_CTL_MCIO_DDRIOEXT_STRUCT       DdrCRCtlPiCoding;
  DDRCRCMDPICODING_CKE_MCIO_DDRIOEXT_STRUCT       DdrCRCkePiCoding;
  DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_STRUCT       DdrCrCtlTraining;
  DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_STRUCT       DdrCrCkeTraining;
  DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_STRUCT     DdrCrCmdControls3;
  DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_STRUCT          DdrCrClkRanksUsed;
  DDRCRCLKTRAINING_MCIO_DDRIOEXT_STRUCT           DdrCrClkTraining;
  DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_STRUCT      DdrCrCmdNPiCoding;
  DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_STRUCT     DdrCrCmdNPiCoding4;
  DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_STRUCT     DdrCrCmdSPiCoding4;
  DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_STRUCT      DdrCrCtlPiCoding4;
  DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_STRUCT      DdrCrCkePiCoding4;
  DATACONTROL4N0_0_MCIO_DDRIO_STRUCT              DataControl4;

  DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_STRUCT      DdrPiCodingCmdS;
  DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_STRUCT      DdrPiCodingCmdN;
  DDRCRCMDPICODING_CTL_MCIO_DDRIOEXT_STRUCT       DdrPiCodingCtl;
  DDRCRCMDPICODING_CKE_MCIO_DDRIOEXT_STRUCT       DdrPiCodingCke;

  MSVx4 = GetMaxStrobeValid (Host);
  if (IsBrsPresent (Host, Socket)) {
    MSVx4 = MAX_STROBE;
  }
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (EFI_SUCCESS != GetXoverOffsets (Socket, Ch, &XoverOffsets)) {
    RcDebugPrint (SDBG_MAX,
                  "!! ProgramOffsets - Failed to find offset table !!\n");
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_143);
    return EFI_NOT_FOUND;
  }

  //
  // Program offsets
  //
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  " CMDN PiRef0CMDN CMDS PiRef0CMDS CTL PiRef0CTL CKE PiRef0CKE CLK  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17\n");

  PiDelay = OffsetSt->CmdN[Ch];
  PiRef0PiDelay =  PiRef0OffsetSt->CmdN[Ch];
  RcDebugPrint (SDBG_MAX,
                  "offsets:  %2d", PiDelay);
  RcDebugPrint (SDBG_MAX,
                  "      %2d", PiRef0PiDelay);
  DdrCrCmdControls3.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG);
  DdrCrCmdControls3.Bits.cmdrefpiclkdelay = (PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG, DdrCrCmdControls3.Data);
  (*ChannelNvList)[Ch].ddrCRCmdControls3CmdN = DdrCrCmdControls3.Data;

  DdrCrCmdNTraining.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG);
  DdrCrCmdNTraining.Bits.trainingoffset = (PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, DdrCrCmdNTraining.Data);
  (*ChannelNvList)[Ch].ddrCRCmdTrainingCmdN = DdrCrCmdNTraining.Data;

  DdrCrCmdNPiCoding.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_REG);
  DdrCrCmdNPiCoding.Bits.ddrcmdctlsyncmsb = 2;
  // 2 MSB of PiDelay
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_REG, DdrCrCmdNPiCoding.Data);
  GetCmdGroupAddress10nm (Host, 0, SIDE_A, &RegIndex);
  (*ChannelNvList)[Ch].cmdCsr[RegIndex] = DdrCrCmdNPiCoding.Data;

  DdrCrCmdNPiCoding4.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_REG);
  DdrCrCmdNPiCoding4.Bits.cmdrefpi0picode = (PiRef0PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_REG, DdrCrCmdNPiCoding4.Data);
  GetCmdGroupAddress10nm (Host, 9, SIDE_A, &RegIndex);
  (*ChannelNvList)[Ch].cmdCsr[RegIndex] = DdrCrCmdNPiCoding4.Data;

  PiDelay = OffsetSt->CmdS[Ch];
  PiRef0PiDelay = PiRef0OffsetSt->CmdS[Ch];
  RcDebugPrint (SDBG_MAX,
                  "      %2d", PiDelay);
  RcDebugPrint (SDBG_MAX,
                  "      %2d,", PiRef0PiDelay);
  DdrCrCmdControls3.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG);
  DdrCrCmdControls3.Bits.cmdrefpiclkdelay = (PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG, DdrCrCmdControls3.Data);
  (*ChannelNvList)[Ch].ddrCRCmdControls3CmdS = DdrCrCmdControls3.Data;

  DdrCrCmdSTraining.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG);
  DdrCrCmdSTraining.Bits.trainingoffset = (PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, DdrCrCmdSTraining.Data);
  (*ChannelNvList)[Ch].ddrCRCmdTrainingCmdS = DdrCrCmdSTraining.Data;

  DdrCRCmdSPiCoding.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_REG);
  DdrCRCmdSPiCoding.Bits.ddrcmdctlsyncmsb = 2;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_REG, DdrCRCmdSPiCoding.Data);
  GetCmdGroupAddress10nm (Host, CC_SOUTH, SIDE_A, &RegIndex);
  (*ChannelNvList)[Ch].cmdCsr[RegIndex] = DdrCRCmdSPiCoding.Data;

  DdrCrCmdSPiCoding4.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_REG);
  DdrCrCmdSPiCoding4.Bits.cmdrefpi0picode = (PiRef0PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_REG, DdrCrCmdSPiCoding4.Data);
  GetCmdGroupAddress10nm (Host, 9 + CC_SOUTH, SIDE_A, &RegIndex);
  (*ChannelNvList)[Ch].cmdCsr[RegIndex] = DdrCrCmdSPiCoding4.Data;

  PiDelay = OffsetSt->Ctl[Ch];
  PiRef0PiDelay = PiRef0OffsetSt->Ctl[Ch];
  RcDebugPrint (SDBG_MAX,
                  "    %2d", PiDelay);
  RcDebugPrint (SDBG_MAX,
                 "     %2d,", PiRef0PiDelay);
  DdrCrCmdControls3.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG);
  DdrCrCmdControls3.Bits.cmdrefpiclkdelay = (PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG, DdrCrCmdControls3.Data);
  (*ChannelNvList)[Ch].ddrCRCmdControls3Ctl = DdrCrCmdControls3.Data;

  DdrCrCtlTraining.Data = (UINT16)MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG);
  DdrCrCtlTraining.Bits.trainingoffset = (PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG, DdrCrCtlTraining.Data);
  (*ChannelNvList)[Ch].ddrCRCtlTraining = DdrCrCtlTraining.Data;

  DdrCRCtlPiCoding.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING_CTL_MCIO_DDRIOEXT_REG);
  DdrCRCtlPiCoding.Bits.ddrcmdctlsyncmsb = 2;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING_CTL_MCIO_DDRIOEXT_REG, DdrCRCtlPiCoding.Data);
  GetCmdGroupAddress10nm (Host, CC_SOUTH, SIDE_B, &RegIndex);
  (*ChannelNvList)[Ch].cmdCsr[RegIndex] = DdrCRCtlPiCoding.Data;

  DdrCrCtlPiCoding4.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_REG);
  DdrCrCtlPiCoding4.Bits.cmdrefpi0picode = (PiRef0PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_REG, DdrCrCtlPiCoding4.Data);
  GetCmdGroupAddress10nm (Host, 9 + CC_SOUTH, SIDE_B, &RegIndex);
  (*ChannelNvList)[Ch].cmdCsr[RegIndex] = DdrCrCtlPiCoding4.Data;

  PiDelay = OffsetSt->Cke[Ch];
  PiRef0PiDelay = PiRef0OffsetSt->Cke[Ch];
  RcDebugPrint (SDBG_MAX,
                  "    %2d", PiDelay);
  RcDebugPrint (SDBG_MAX,
                  "     %2d", PiRef0PiDelay);
  DdrCrCmdControls3.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG);
  DdrCrCmdControls3.Bits.cmdrefpiclkdelay = (PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG, DdrCrCmdControls3.Data);
  (*ChannelNvList)[Ch].ddrCRCmdControls3Cke = DdrCrCmdControls3.Data;

  DdrCrCkeTraining.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG);
  DdrCrCkeTraining.Bits.trainingoffset = (PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG, DdrCrCkeTraining.Data);
  (*ChannelNvList)[Ch].ddrCRCkeTraining = DdrCrCkeTraining.Data;

  DdrCRCkePiCoding.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING_CKE_MCIO_DDRIOEXT_REG);
  DdrCRCkePiCoding.Bits.ddrcmdctlsyncmsb = 2;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING_CKE_MCIO_DDRIOEXT_REG, DdrCRCkePiCoding.Data);
  GetCmdGroupAddress10nm(Host, 0, SIDE_B, &RegIndex);
  (*ChannelNvList)[Ch].cmdCsr[RegIndex] = DdrCRCkePiCoding.Data;

  DdrCrCkePiCoding4.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_REG);
  DdrCrCkePiCoding4.Bits.cmdrefpi0picode = (PiRef0PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_REG, DdrCrCkePiCoding4.Data);
  GetCmdGroupAddress10nm (Host, 9, SIDE_B, &RegIndex);
  (*ChannelNvList)[Ch].cmdCsr[RegIndex] = DdrCrCkePiCoding4.Data;

  DdrPiCodingCmdS.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_REG);
  DdrPiCodingCmdS.Bits.ddrcmdctlsyncmsb = 2;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_REG, DdrPiCodingCmdS.Data);

  DdrPiCodingCmdN.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_REG);
  DdrPiCodingCmdN.Bits.ddrcmdctlsyncmsb = 2;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_REG, DdrPiCodingCmdN.Data);

  DdrPiCodingCtl.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING_CTL_MCIO_DDRIOEXT_REG);
  DdrPiCodingCtl.Bits.ddrcmdctlsyncmsb = 2;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING_CTL_MCIO_DDRIOEXT_REG, DdrPiCodingCtl.Data);

  DdrPiCodingCke.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCMDPICODING_CKE_MCIO_DDRIOEXT_REG);
  DdrPiCodingCke.Bits.ddrcmdctlsyncmsb = 2;
  MemWritePciCfgEp (Socket, Ch, DDRCRCMDPICODING_CKE_MCIO_DDRIOEXT_REG, DdrPiCodingCke.Data);

  PiDelay = PiRef0OffsetSt->Clk[Ch];
  RcDebugPrint (SDBG_MAX,
                  "     %2d ", PiDelay);
  DdrCrClkRanksUsed.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
  DdrCrClkRanksUsed.Bits.refpiclk = (PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, DdrCrClkRanksUsed.Data);
  (*ChannelNvList)[Ch].ddrCRClkRanksUsed = DdrCrClkRanksUsed.Data;

  DdrCrClkTraining.Data = MemReadPciCfgEp (Socket, Ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG);
  DdrCrClkTraining.Bits.trainingoffset = (PiDelay) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, DdrCrClkTraining.Data);
  (*ChannelNvList)[Ch].ddrCRClkTraining = DdrCrClkTraining.Data;

  // SKX change for upper nibble
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {

    PiDelay = PiRef0OffsetSt->Offset[Ch][Strobe];
    RcDebugPrint (SDBG_MAX,
                    " %2d", PiDelay);
    DataControl4.Data = (*ChannelNvList)[Ch].dataControl4[Strobe];
    DataControl4.Bits.refpiclkdelay = (PiDelay + XoverOffsets->DataOffset) % X_OVER_MAX_SAMPLE;
    WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, DataControl4.Data);
    (*ChannelNvList)[Ch].dataControl4[Strobe] = DataControl4.Data;

    DataOffsetTrain.Data = 0;

    RecEnOffsetValue = PiDelay + XoverOffsets->DataOffset;
    TxDqOffsetValue = PiDelay + XoverOffsets->DataOffset;
    TxDqsOffsetValue = PiDelay + XoverOffsets->DataOffset;

    SiliconWorkaroundS1409677997 (RecEnDelay, Strobe, &RecEnOffsetValue);
    SiliconWorkaroundS1409677997 (TxDqDelay, Strobe, &TxDqOffsetValue);
    SiliconWorkaroundS1409677997 (TxDqsDelay, Strobe, &TxDqsOffsetValue);

    DataOffsetTrain.Bits.rcvenoffset = RecEnOffsetValue % X_OVER_MAX_SAMPLE;
    DataOffsetTrain.Bits.txdqoffset  = TxDqOffsetValue % X_OVER_MAX_SAMPLE;
    DataOffsetTrain.Bits.txdqsoffset = TxDqsOffsetValue % X_OVER_MAX_SAMPLE;
    WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATAOFFSETTRAINN0_0_MCIO_DDRIO_REG, DataOffsetTrain.Data);
    (*ChannelNvList)[Ch].dataOffsetTrain[Strobe] = DataOffsetTrain.Data;

  } // Strobe loop

  RcDebugPrint (SDBG_MAX, "\n");

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  " Additional Xover PiDelay offset for ICX: CMD = %d, CMD PIREF0 = %d, CLK = %d, DATA = %d\n",
                  XoverOffsets->CmdOffset, XoverOffsets->CmdPi0Offset, XoverOffsets->ClkOffset, XoverOffsets->DataOffset);
  return (EFI_SUCCESS);
} //ProgramOffsets

/**

  Peform crossover calibration

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
XoverCalib (
  PSYSHOST  Host
  )
{
  UINT8                                           Ch;
  UINT8                                           Socket;
  UINT8                                           Strobe;
  UINT16                                          PiDelay;
  UINT8                                           MSVx4 = GetMaxStrobeValid (Host);
  struct channelNvram (*ChannelNvList) [MAX_CH];
  COUNT1_STRUCT                                   Count1;
  COUNT1_STRUCT                                   PiRef0Count1;
  OFFSET_STRUCT                                   OffsetSt;
  OFFSET_STRUCT                                   PiRef0OffsetSt;
  PREVIOUS_STATUS_STRUCT                          PreviousStatus;
  PREVIOUS_STATUS_STRUCT                          PiRef0PreviousStatus;
  RESULTS_STRUCT                                  Results;
  RESULTS_STRUCT                                  PiRef0Results;
  UINT8                                           MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  ChannelNvList = GetChannelNvList (Host, Socket);

  if (IsBrsPresent (Host, Socket)) {
    MSVx4 = MAX_STROBE;
  }

  if ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "Enable xovercal\n");
    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_CROSSOVER_CALIBRATION, Socket);
    for (Ch = 0;Ch < MaxChDdr;Ch++) {
      if ((*ChannelNvList) [Ch].enabled == 0) {
        continue;
      }
      EnableDisableXoverCalRegs (Host, Socket, Ch, 1);
      InitializeLocalsPerCh (Host, Ch, &Count1, &OffsetSt, &PreviousStatus, &Results);
      InitializeLocalsPerCh (Host, Ch, &PiRef0Count1, &PiRef0OffsetSt, &PiRef0PreviousStatus, &PiRef0Results);

      RcDebugPrint (SDBG_MAX, "\n");
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "piDelay CMDN PiRef0CMDN CMDS PiRef0CMDS CTL PiRef0CTL CKE PiRef0CKE CLK");
      for (Strobe = 0;Strobe < MSVx4;Strobe++) {
        RcDebugPrint (SDBG_MAX,
                        "%3d", Strobe);
      }
      RcDebugPrint (SDBG_MAX,
                      "\n");

      for (PiDelay = X_OVER_PI_START;PiDelay < X_OVER_MAX_SAMPLE;PiDelay += X_OVER_STEP_SIZE) {
        RcDebugPrint (SDBG_MAX,
                        "         %2d     ", PiDelay);
        SetCmdRefPiClkDelay (Host, Socket, Ch, PiDelay);
        GetCrossoverCalibrationResults (Host, Socket, Ch, PiDelay, &Results, &PiRef0Results);
        DisplayCrossoverCalibrationResults (Host, Ch, PiDelay, &Results, &PiRef0Results, &OffsetSt, &PiRef0OffsetSt, &Count1, &PiRef0Count1, &PreviousStatus, &PiRef0PreviousStatus);
      } // PiDelay loop

      //Print Number of 1s from all phase detectors
      RcDebugPrint (SDBG_MAX,
                      "\n                CMDN    PiRef0CMDN    CMDS    PiRef0CMDS    CTL    PiRef0CTL    CKE    PiRef0CKE    CLK     0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17\n");
      RcDebugPrint (SDBG_MAX, "COUNT NON-ZERO: ");
      RcDebugPrint (SDBG_MAX, "%3d", Count1.CmdN);
      RcDebugPrint (SDBG_MAX, "        ");
      RcDebugPrint (SDBG_MAX, "%3d", PiRef0Count1.CmdN);
      RcDebugPrint (SDBG_MAX, "        ");
      RcDebugPrint (SDBG_MAX, "%3d", Count1.CmdS);
      RcDebugPrint (SDBG_MAX, "        ");
      RcDebugPrint (SDBG_MAX, "%3d", PiRef0Count1.CmdS);
      RcDebugPrint (SDBG_MAX, "       ");
      RcDebugPrint (SDBG_MAX, "%3d", Count1.Ctl);
      RcDebugPrint (SDBG_MAX, "       ");
      RcDebugPrint (SDBG_MAX, "%3d", PiRef0Count1.Ctl);
      RcDebugPrint (SDBG_MAX, "       ");
      RcDebugPrint (SDBG_MAX, "%3d", Count1.Cke);
      RcDebugPrint (SDBG_MAX, "       ");
      RcDebugPrint (SDBG_MAX, "%3d", PiRef0Count1.Cke);
      RcDebugPrint (SDBG_MAX, "       ");
      RcDebugPrint (SDBG_MAX, "%3d", PiRef0Count1.Clk);
      RcDebugPrint (SDBG_MAX, "    ");
      for (Strobe = 0;Strobe < MSVx4;Strobe++) {
        RcDebugPrint (SDBG_MAX,
                        "%3d", PiRef0Count1.Strobe[Strobe]);
      }
      RcDebugPrint (SDBG_MAX,
                      "\n");

      EnableDisableXoverCalRegs (Host, Socket, Ch, 0);
      ProgramOffsets (Host, Socket, Ch, &OffsetSt, &PiRef0OffsetSt);

    } // Ch loop


  } else if (Host->var.mem.subBootMode == ColdBootFast) { // if ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot))

    for (Ch = 0;Ch < MaxChDdr;Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      MemWritePciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, (*ChannelNvList)[Ch].ddrCRCmdTrainingCmdN);
      MemWritePciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, (*ChannelNvList)[Ch].ddrCRCmdTrainingCmdS);
      MemWritePciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG, (*ChannelNvList)[Ch].ddrCRCtlTraining);
      MemWritePciCfgEp (Socket, Ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG, (*ChannelNvList)[Ch].ddrCRCkeTraining);
      MemWritePciCfgEp (Socket, Ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, (*ChannelNvList)[Ch].ddrCRClkTraining);

      MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG, (*ChannelNvList)[Ch].ddrCRCmdControls3CmdN);
      MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG, (*ChannelNvList)[Ch].ddrCRCmdControls3CmdS);
      MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG, (*ChannelNvList)[Ch].ddrCRCmdControls3Ctl);
      MemWritePciCfgEp (Socket, Ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG, (*ChannelNvList)[Ch].ddrCRCmdControls3Cke);
      MemWritePciCfgEp (Socket, Ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, (*ChannelNvList)[Ch].ddrCRClkRanksUsed);

      // SKX change for upper nibble
      for (Strobe = 0;Strobe < MSVx4;Strobe++) {

        WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATAOFFSETTRAINN0_0_MCIO_DDRIO_REG, (*ChannelNvList)[Ch].dataOffsetTrain[Strobe]);
        WriteDdrioDataRegCh (Host, Socket, Ch, Strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, (*ChannelNvList)[Ch].dataControl4[Strobe]);
      } // Strobe loop
    } // Ch loop
  } // if ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot))

#ifdef DEBUG_CODE_BLOCK
  DisplayXoverResults (Host, Socket);
  ReleasePrintControl ();
#endif //DEBUG_CODE_BLOCK
  return (SUCCESS);
} // XoverCalib
#endif // #if (MEM_IP_VER >= MEM_IP_17ww37e)

#if (MEM_IP_VER < MEM_IP_17ww37e)
/**

  Peform crossover calibration.  This version of the function uses the CRIF prior to 17ww37e.

  @param Host  - Pointer to sysHost

  @retval Status

**/
UINT32
XoverCalibPre17ww37e (
            PSYSHOST  Host
           )
{
  UINT8                                 socket;
  UINT8                                 ch;
  UINT8                                 strobe;
  UINT16                                piDelay;
  UINT32                                feedBack;
  UINT8                                 sampleCounter;
  UINT8                                 previousStatusData[MAX_CH][MAX_STROBE];
  UINT8                                 previousStatusCmdN[MAX_CH][MAX_CMDPI];
  UINT8                                 previousStatusCmdS[MAX_CH][MAX_CMDPI];
  UINT8                                 previousStatusCtl[MAX_CH][MAX_CMDPI];
  UINT8                                 previousStatusCke[MAX_CH][MAX_CMDPI];
  UINT8                                 previousStatusClk[MAX_CH][MAX_CLKPI];
  UINT16                                offset[MAX_CH][MAX_STROBE];
  UINT16                                offsetCmdN[MAX_CH][MAX_CMDPI];
  UINT16                                offsetCmdS[MAX_CH][MAX_CMDPI];
  UINT16                                offsetCtl[MAX_CH][MAX_CMDPI];
  UINT16                                offsetCke[MAX_CH][MAX_CMDPI];
  UINT16                                offsetClk[MAX_CH][MAX_CLKPI];
  UINT8                                 cmdLoop;
  UINT8                                 cmdnCounter[MAX_CMDPI];
  UINT8                                 cmdsCounter[MAX_CMDPI];
  UINT8                                 ckeCounter[MAX_CMDPI];
  UINT8                                 ctlCounter[MAX_CMDPI];
  UINT8                                 clkCounter[MAX_CLKPI];
  UINT8                                 cmdnResult[MAX_CMDPI];
  UINT8                                 cmdsResult[MAX_CMDPI];
  UINT8                                 ckeResult[MAX_CMDPI];
  UINT8                                 ctlResult[MAX_CMDPI];
  UINT8                                 clkResult[MAX_CLKPI];
  UINT8                                 dataCntr[MAX_STROBE];
  UINT8                                 dataResult[MAX_STROBE];
  XOVER_OFFSETS                         *XoverOffsets;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  DATACONTROL0N0_0_MCIO_DDRIO_STRUCT              dataControl0;
  DATAOFFSETTRAINN0_0_MCIO_DDRIO_STRUCT           dataOffsetTrain;
  DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_STRUCT      ddrCRCmdControls;
  DDRCRCLKCONTROLS_MCIO_DDRIOEXT_STRUCT           ddrCRClkControls;
  DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_STRUCT      ddrCRCmdNTraining;
  DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_STRUCT      ddrCRCmdSTraining;
  DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_STRUCT       ddrCRCtlTraining;
  DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_STRUCT       ddrCRCkeTraining;
  DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_STRUCT     ddrCRCmdControls3;
  DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_STRUCT          ddrCRClkRanksUsed;
  DATACONTROL4N0_0_MCIO_DDRIO_STRUCT              dataControl4;
  DDRCRCLKTRAINING_MCIO_DDRIOEXT_STRUCT           ddrCRClkTraining;
  // ICX_TODO
  DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_STRUCT      ddrCRCmdNPiCoding;
  DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_STRUCT      ddrCRCmdSPiCoding;
  DDRCRCMDPICODING_CTL_MCIO_DDRIOEXT_STRUCT       ddrCRCtlPiCoding;
  DDRCRCMDPICODING_CKE_MCIO_DDRIOEXT_STRUCT       ddrCRCkePiCoding;
  DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_STRUCT     ddrCRCmdNPiCoding4;
  DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_STRUCT     ddrCRCmdSPiCoding4;
  DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_STRUCT      ddrCRCtlPiCoding4;
  DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_STRUCT      ddrCRCkePiCoding4;

  UINT8     i;
  UINT16    minOffset[MAX_CMD_FUB];
  UINT16    maxOffset[MAX_CMD_FUB];
  UINT16    midOffset[MAX_CMD_FUB];
  UINT16    minClkOffset;
  UINT16    maxClkOffset;
  UINT16    midClkOffset;
  UINT8     regIndex;
  UINT8     count1cmdn[MAX_CMDPI];
  UINT8     count1cmds[MAX_CMDPI];
  UINT8     count1ctl[MAX_CMDPI];
  UINT8     count1cke[MAX_CMDPI];
  UINT8     count1clk[MAX_CLKPI];
  UINT8     count1strobe[MAX_STROBE];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;

  channelNvList = GetChannelNvList(Host, socket);

  dataControl0.Data = 0;
#ifdef DDR5_SUPPORT
  if (IsBrsPresent(Host, socket)) {
    MSVx4 = MAX_STROBE;
  }
#endif //DDR5_SUPPORT
  if ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {

    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Enable xovercal\n");
    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_CROSSOVER_CALIBRATION, socket);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      // List of SKX registers containing xovercal
      //2_22_0
      //DDRDATA0N0CH0_CR_DATACONTROL0N0_0_REG
      //DDRDATA0N1CH0_CR_DATACONTROL0N1_0_REG
      //DDRDATA1N0CH0_CR_DATACONTROL0N0_1_REG
      //DDRDATA1N1CH0_CR_DATACONTROL0N1_1_REG
      //DDRDATA2N0CH0_CR_DATACONTROL0N0_2_REG
      //DDRDATA2N1CH0_CR_DATACONTROL0N1_2_REG
      //DDRDATA3N0CH0_CR_DATACONTROL0N0_3_REG
      //DDRDATA3N1CH0_CR_DATACONTROL0N1_3_REG
      //DDRDATA8N0CH0_CR_DATACONTROL0N0_8_REG
      //DDRDATA8N1CH0_CR_DATACONTROL0N1_8_REG
      //DDRDATA4N0CH0_CR_DATACONTROL0N0_4_REG
      //DDRDATA4N1CH0_CR_DATACONTROL0N1_4_REG
      //DDRDATA5N0CH0_CR_DATACONTROL0N0_5_REG
      //DDRDATA5N1CH0_CR_DATACONTROL0N1_5_REG
      //2_22_1
      //DDRDATA6N0CH0_CR_DATACONTROL0N0_6_REG
      //DDRDATA6N1CH0_CR_DATACONTROL0N1_6_REG
      //DDRDATA7N0CH0_CR_DATACONTROL0N0_7_REG
      //DDRDATA7N1CH0_CR_DATACONTROL0N1_7_REG
      //DDRCMDNCH0_CR_DDRCRCMDCONTROLS_CMDN_REG
      //DDRCKECH0_CR_DDRCRCMDCONTROLS_CKE_REG
      //DDRCLKCH0_CR_DDRCRCLKCONTROLS_REG
      //DDRCMDSCH0_CR_DDRCRCMDCONTROLS_CMDS_REG
      //DDRCTLCH0_CR_DDRCRCMDCONTROLS_CTL_REG

      ddrCRCmdControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls.Bits.xovercal = 1;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

      ddrCRCmdControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS_CKE_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls.Bits.xovercal = 1;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

      ddrCRCmdControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls.Bits.xovercal = 1;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

      ddrCRCmdControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS_CTL_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls.Bits.xovercal = 1;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

      ddrCRClkControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
      ddrCRClkControls.Bits.xovercal = 1;
      MemWritePciCfgEp (socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, ddrCRClkControls.Data | (*channelNvList)[ch].ddrCRClkControls);

      dataControl0.Bits.xovercal = 1;
      for (strobe = 0; strobe < MSVx4; strobe++) {
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data);
      } // strobe loop


      for (i = 0; i < MAX_CMDPI; i++) {
        count1cmdn[i] = 0;
        count1cmds[i] = 0;
        count1ctl[i] = 0;
        count1cke[i] = 0;

        offsetCmdN[ch][i] = 0;
        offsetCmdS[ch][i] = 0;
        offsetCtl[ch][i] = 0;
        offsetCke[ch][i] = 0;

        previousStatusCmdN[ch][i] = 0;
        previousStatusCmdS[ch][i] = 0;
        previousStatusCtl[ch][i] = 0;
        previousStatusCke[ch][i] = 0;
      }
      // Special case for SKX phase detector not implemented
      //trainingDoneCtl[ch][0] = 1;
      //trainingDoneCke[ch][0] = 1;

      for (i = 0; i < MAX_CLKPI; i++) {
        offsetClk[ch][i] = 0;
        previousStatusClk[ch][i] = 0;
        count1clk[i] = 0;
      }

      RcDebugPrint (SDBG_MAX, "\n");
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "piDelay CMDN[0:5] CMDS[0:5] CTL[0:5]  CKE[0:5]  CLK[0:7]  ");

      ZeroMem (previousStatusData[ch], MSVx4);
      ZeroMem (offset[ch], MSVx4 * sizeof(UINT16));
      ZeroMem (count1strobe, MSVx4);
      for (strobe = 0; strobe < MSVx4; strobe++) {
        RcDebugPrint (SDBG_MAX,
                       "%3d", strobe);
      }
      RcDebugPrint (SDBG_MAX,
                       "\n");


      //SKX training algorithm: remains the same as HSX x-over training, except:
      //  BIOS will sweep RefPiClkDelay (instead of DdrTxDqPiDelayQnnnL)
      //  Resulting offset is programmed into RcvEnOffset, TxDqOffset, TxDqsOffset AND RefPiClkDelay (last one is new)

      // List of SKX registers containing RefPiClkDelay
      //2_22_0
      //DDRDATA0N0CH0_CR_DATACONTROL4N0_0_REG
      //DDRDATA0N1CH0_CR_DATACONTROL4N1_0_REG
      //DDRDATA1N0CH0_CR_DATACONTROL4N0_1_REG
      //DDRDATA1N1CH0_CR_DATACONTROL4N1_1_REG
      //DDRDATA2N0CH0_CR_DATACONTROL4N0_2_REG
      //DDRDATA2N1CH0_CR_DATACONTROL4N1_2_REG
      //DDRDATA3N0CH0_CR_DATACONTROL4N0_3_REG
      //DDRDATA3N1CH0_CR_DATACONTROL4N1_3_REG
      //DDRDATA8N0CH0_CR_DATACONTROL4N0_8_REG
      //DDRDATA8N1CH0_CR_DATACONTROL4N1_8_REG
      //DDRDATA4N0CH0_CR_DATACONTROL4N0_4_REG
      //DDRDATA4N1CH0_CR_DATACONTROL4N1_4_REG
      //DDRDATA5N0CH0_CR_DATACONTROL4N0_5_REG
      //DDRDATA5N1CH0_CR_DATACONTROL4N1_5_REG
      //2_22_1
      //DDRDATA6N0CH0_CR_DATACONTROL4N0_6_REG
      //DDRDATA6N1CH0_CR_DATACONTROL4N1_6_REG
      //DDRDATA7N0CH0_CR_DATACONTROL4N0_7_REG
      //DDRDATA7N1CH0_CR_DATACONTROL4N1_7_REG
      //DDRCMDNCH0_CR_DDRCRCMDCONTROLS3_CMDN_REG - cmdrefpiclkdelay
      //DDRCKECH0_CR_DDRCRCMDCONTROLS3_CKE_REG - cmdrefpiclkdelay
      //DDRCLKCH0_CR_DDRCRCLKRANKSUSED_REG - refpiclk
      //DDRCMDSCH0_CR_DDRCRCMDCONTROLS3_CMDS_REG - cmdrefpiclkdelay
      //DDRCTLCH0_CR_DDRCRCMDCONTROLS3_CTL_REG - cmdrefpiclkdelay


      for (piDelay = X_OVER_PI_START; piDelay < X_OVER_MAX_SAMPLE; piDelay += X_OVER_STEP_SIZE) {

        RcDebugPrint (SDBG_MAX,
                       "         %2d     ", piDelay);

        for (i = 0; i < MAX_CMDPI; i++) {
          cmdnCounter[i] = 0;
          cmdsCounter[i] = 0;
          ckeCounter[i] = 0;
          ctlCounter[i] = 0;
        }
        for (i = 0; i < MAX_CLKPI; i++) {
          clkCounter[i] = 0;
        }
        ZeroMem (dataCntr, MSVx4);

        ddrCRCmdControls3.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG);
        ddrCRCmdControls3.Bits.cmdrefpiclkdelay = piDelay;
        MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls3.Data);

        ddrCRCmdControls3.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG);
        ddrCRCmdControls3.Bits.cmdrefpiclkdelay = piDelay;
        MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG, ddrCRCmdControls3.Data);

        ddrCRCmdControls3.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG);
        ddrCRCmdControls3.Bits.cmdrefpiclkdelay = piDelay;
        MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls3.Data);

        ddrCRCmdControls3.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG);
        ddrCRCmdControls3.Bits.cmdrefpiclkdelay = piDelay;
        MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG, ddrCRCmdControls3.Data);

        // ICX_TODO
        // Program 10nm CmdRefPi0Picode
        ddrCRCmdNPiCoding4.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_REG);
        //ddrCRCmdNPiCoding4.Bits.cmdrefpi0picode = piDelay;
        ddrCRCmdNPiCoding4.Data &= ~(0x3F << 24);
        ddrCRCmdNPiCoding4.Data = ddrCRCmdNPiCoding4.Data | (piDelay & 0x3F) << 24;
        MemWritePciCfgEp (socket, ch, DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdNPiCoding4.Data);

        ddrCRCmdSPiCoding4.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_REG);
        //ddrCRCmdSPiCoding4.Bits.cmdrefpi0picode = piDelay;
        ddrCRCmdSPiCoding4.Data &= ~(0x3F << 24);
        ddrCRCmdSPiCoding4.Data = ddrCRCmdSPiCoding4.Data | (piDelay & 0x3F) << 24;
        MemWritePciCfgEp (socket, ch, DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdSPiCoding4.Data);

        ddrCRCtlPiCoding4.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_REG);
        //ddrCRCtlPiCoding4.Bits.cmdrefpi0picode = piDelay;
        ddrCRCtlPiCoding4.Data &= ~(0x3F << 24);
        ddrCRCtlPiCoding4.Data = ddrCRCtlPiCoding4.Data | (piDelay & 0x3F) << 24;
        MemWritePciCfgEp (socket, ch, DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlPiCoding4.Data);

        ddrCRCkePiCoding4.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_REG);
        //ddrCRCkePiCoding4.Bits.cmdrefpi0picode = piDelay;
        ddrCRCkePiCoding4.Data &= ~(0x3F << 24);
        ddrCRCkePiCoding4.Data = ddrCRCkePiCoding4.Data | (piDelay & 0x3F) << 24;
        MemWritePciCfgEp (socket, ch, DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_REG, ddrCRCkePiCoding4.Data);

        ddrCRClkRanksUsed.Data = MemReadPciCfgEp (socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
        ddrCRClkRanksUsed.Bits.refpiclk = piDelay;
        MemWritePciCfgEp (socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksUsed.Data);

        for (strobe = 0; strobe < MSVx4; strobe++) {
          dataControl4.Data = (*channelNvList)[ch].dataControl4[strobe];
          dataControl4.Bits.refpiclkdelay = piDelay;
          WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, dataControl4.Data);
        } // strobe loop

        // HSD4165676: Multiple sample during crossover calibration
        for (sampleCounter = 0; sampleCounter < XOVER_CALIB_SAMPLE_COUNT; sampleCounter++) {

          //
          // Delay only if this is a CTE build, otherwise do nothing.
          //

          CteDelayQclk (64);

          //
          // Delay if this is a hardware build, otherwise do nothing.
          //

          FixedDelayMicroSecond (1); // might want to fine tune this delay with real silicon

          //list of SKX regs containing xoverphasedet
          //2_22_1
          //DDRCMDNCH0_CR_DDRCRCMDTRAINING_CMDN_REG
          //DDRCKECH0_CR_DDRCRCMDTRAINING_CKE_REG
          //DDRCLKCH0_CR_DDRCRCLKTRAINING_REG - pclkxoverphasedet
          //DDRCLKCH0_CR_DDRCRCLKTRAINING_REG - nclkxoverphasedet
          //DDRCMDSCH0_CR_DDRCRCMDTRAINING_CMDS_REG
          //DDRCTLCH0_CR_DDRCRCMDTRAINING_CTL_REG

          ddrCRCmdNTraining.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG);
          ddrCRCmdSTraining.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG);
          ddrCRCtlTraining.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG);
          ddrCRCkeTraining.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG);

          RcSimCmdXoverFeedbackTarget (
            piDelay,
            (VOID *)&ddrCRCmdNTraining,
            (VOID *)&ddrCRCmdSTraining,
            (VOID *)&ddrCRCtlTraining,
            (VOID *)&ddrCRCkeTraining
            );

          for (cmdLoop = 0; cmdLoop < MAX_CMDPI; cmdLoop++) {
            if (cmdLoop < CMDPIREF0_INDEX) {
              if ((ddrCRCmdNTraining.Bits.xoverphasedet >> cmdLoop) & 1) {
                cmdnCounter[cmdLoop]++;
              }
              if ((ddrCRCmdSTraining.Bits.xoverphasedet >> cmdLoop) & 1) {
                cmdsCounter[cmdLoop]++;
              }
              if ((ddrCRCtlTraining.Bits.xoverphasedet >> cmdLoop) & 1) {
                ctlCounter[cmdLoop]++;
              }
              if ((ddrCRCkeTraining.Bits.xoverphasedet >> cmdLoop) & 1) {
                ckeCounter[cmdLoop]++;
              }
            }
            else {
              // ICX_TODO
              // Handle piref0xoverphasedet (bit 28)
              /*
              if ((ddrCRCmdNTraining.Bits.piref0xoverphasedet) & 1) {
                cmdnCounter[cmdLoop]++;
              }
              if ((ddrCRCmdSTraining.Bits.piref0xoverphasedet) & 1) {
                cmdsCounter[cmdLoop]++;
              }
              if ((ddrCRCtlTraining.Bits.piref0xoverphasedet) & 1) {
                ctlCounter[cmdLoop]++;
              }
              if ((ddrCRCkeTraining.Bits.piref0xoverphasedet) & 1) {
                ckeCounter[cmdLoop]++;
              }
              */
              if (ddrCRCmdNTraining.Data & BIT28) {
                cmdnCounter[cmdLoop]++;
              }
              if (ddrCRCmdSTraining.Data & BIT28) {
                cmdsCounter[cmdLoop]++;
              }
              if (ddrCRCtlTraining.Data & BIT28) {
                ctlCounter[cmdLoop]++;
              }
              if (ddrCRCkeTraining.Data & BIT28) {
                ckeCounter[cmdLoop]++;
              }
            }
          }
          ddrCRClkTraining.Data = MemReadPciCfgEp (socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG);

          GetPclkNclkClkXoverFeedbackTarget (piDelay, (VOID *)&ddrCRClkTraining);

          for (cmdLoop = 0; cmdLoop < MAX_CLKPI; cmdLoop++) {
            if (cmdLoop < (MAX_CLKPI / 2)) {
              if ((ddrCRClkTraining.Bits.pclkxoverphasedet >> cmdLoop) & 1) {
                clkCounter[cmdLoop]++;
              }
            } else {
              if ((ddrCRClkTraining.Bits.nclkxoverphasedet >> (cmdLoop % (MAX_CLKPI / 2))) & 1) {
                clkCounter[cmdLoop]++;
              }
            }
          }

          //DDRDATA0N0CH0_CR_DATATRAINFEEDBACKN0_0_REG
          //DDRDATA0N1CH0_CR_DATATRAINFEEDBACKN1_0_REG
          //DDRDATA1N0CH0_CR_DATATRAINFEEDBACKN0_1_REG
          //DDRDATA1N1CH0_CR_DATATRAINFEEDBACKN1_1_REG
          //DDRDATA2N0CH0_CR_DATATRAINFEEDBACKN0_2_REG
          //DDRDATA2N1CH0_CR_DATATRAINFEEDBACKN1_2_REG
          //DDRDATA3N0CH0_CR_DATATRAINFEEDBACKN0_3_REG
          //DDRDATA3N1CH0_CR_DATATRAINFEEDBACKN1_3_REG
          //DDRDATA8N0CH0_CR_DATATRAINFEEDBACKN0_8_REG
          //DDRDATA8N1CH0_CR_DATATRAINFEEDBACKN1_8_REG
          //DDRDATA4N0CH0_CR_DATATRAINFEEDBACKN0_4_REG
          //DDRDATA4N1CH0_CR_DATATRAINFEEDBACKN1_4_REG
          //DDRDATA5N0CH0_CR_DATATRAINFEEDBACKN0_5_REG
          //DDRDATA5N1CH0_CR_DATATRAINFEEDBACKN1_5_REG

          /* datatrainfeedback - Bits[17:0], RW_L, default = 1'b0
             Data Training Feedback Storage

                     DataTrainFeedback Description:
                      Training Step    Bits    Nibble    Description
                     --------------    ----    -------
             --------------------------------------------------------------------------------------------------------------------------------------------------------
              - SaOffset Calibration: during SA offset calibration, RX outputs are stored in bits 3:0.
              - Xover Calibration: during x-over calibration, X-over phase detector output is stored in bit 0
              - Read Leveling: during Rcven/Read leveling training, 9-bit counter output is stored in bits 8:0
              - Write Leveling: during Write leveling training, 9-bit counter output is stored in bits 8:0
          */

          for (strobe = 0; strobe < MSVx4; strobe++) {
            feedBack = ReadDdrioDataRegCh (Host, socket, ch, strobe, DATATRAINFEEDBACKN0_0_MCIO_DDRIO_REG);


            RcSimDataXoverFeedbackTarget (piDelay, strobe, &feedBack);

            if (feedBack & 1) {
              dataCntr[strobe]++;
            }
          } // strobe loop
        }

        for (cmdLoop = 0; cmdLoop < MAX_CMDPI; cmdLoop++) {
          if ((cmdnCounter[cmdLoop] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
            cmdnResult[cmdLoop] = 1;
          } else {
            cmdnResult[cmdLoop] = 0;
          }
          if ((cmdsCounter[cmdLoop] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
            cmdsResult[cmdLoop] = 1;
          } else {
            cmdsResult[cmdLoop]= 0;
          }
          if ((ckeCounter[cmdLoop] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
            ckeResult[cmdLoop] = 1;
          } else {
            ckeResult[cmdLoop] = 0;
          }
          if ((ctlCounter[cmdLoop] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
            ctlResult[cmdLoop] = 1;
          } else {
            ctlResult[cmdLoop] = 0;
          }
        }

        for (cmdLoop = 0; cmdLoop < MAX_CLKPI; cmdLoop++) {
          if ((clkCounter[cmdLoop] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
            clkResult[cmdLoop] = 1;
          } else {
            clkResult[cmdLoop] = 0;
          }
        }

        for (strobe = 0; strobe < MSVx4; strobe++) {
          if ((dataCntr[strobe] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
            dataResult[strobe] = 1;
          } else {
            dataResult[strobe] = 0;
          }
        }

        for (cmdLoop = 0; cmdLoop < MAX_CMDPI; cmdLoop++) {
            if (cmdnResult[cmdLoop]) {
              count1cmdn[cmdLoop]++;
              RcDebugPrint (SDBG_MAX,
                             "1");
            } else {
              RcDebugPrint (SDBG_MAX,
                             "0");
            }
            if ((previousStatusCmdN[ch][cmdLoop] != 0) & (cmdnResult[cmdLoop] == 0)) {
              if (piDelay > offsetCmdN[ch][cmdLoop]) {
                offsetCmdN[ch][cmdLoop] = piDelay;
              }

            }
              previousStatusCmdN[ch][cmdLoop] = cmdnResult[cmdLoop];

        }
        RcDebugPrint (SDBG_MAX,"    ");

        for (cmdLoop = 0; cmdLoop < MAX_CMDPI; cmdLoop++) {
            if (cmdsResult[cmdLoop]) {
              count1cmds[cmdLoop]++;
              RcDebugPrint (SDBG_MAX,
                             "1");
            } else {
              RcDebugPrint (SDBG_MAX,
                             "0");
            }
            if ((previousStatusCmdS[ch][cmdLoop] != 0) & (cmdsResult[cmdLoop] == 0)) {
              if (piDelay > offsetCmdS[ch][cmdLoop]) {
                offsetCmdS[ch][cmdLoop] = piDelay;
              }

            }
              previousStatusCmdS[ch][cmdLoop] = cmdsResult[cmdLoop];

        }
        RcDebugPrint (SDBG_MAX,"    ");

        for (cmdLoop = 0; cmdLoop < MAX_CMDPI; cmdLoop++) {
            if (ctlResult[cmdLoop]) {
              if(cmdLoop >= 1) {
                count1ctl[cmdLoop]++;
              }
              RcDebugPrint (SDBG_MAX,
                             "1");
            } else {
              RcDebugPrint (SDBG_MAX,
                             "0");
            }
            if ((previousStatusCtl[ch][cmdLoop] != 0) & (ctlResult[cmdLoop] == 0)) {
              if (piDelay > offsetCtl[ch][cmdLoop]) {
                offsetCtl[ch][cmdLoop] = piDelay;
              }
            }
              previousStatusCtl[ch][cmdLoop] = ctlResult[cmdLoop];

        }
        RcDebugPrint (SDBG_MAX,"    ");

        for (cmdLoop = 0; cmdLoop < MAX_CMDPI; cmdLoop++) {
            if (ckeResult[cmdLoop]) {
              if (cmdLoop >= 1) {
                count1cke[cmdLoop]++;
              }
              RcDebugPrint (SDBG_MAX,
                             "1");
            } else {
              RcDebugPrint (SDBG_MAX,
                             "0");
            }
            if ((previousStatusCke[ch][cmdLoop] != 0) & (ckeResult[cmdLoop] == 0)) {
              if (piDelay > offsetCke[ch][cmdLoop]) {
                offsetCke[ch][cmdLoop] = piDelay;
              }

            }
              previousStatusCke[ch][cmdLoop] = ckeResult[cmdLoop];
            }
        RcDebugPrint (SDBG_MAX,"    ");

        for (cmdLoop = 0; cmdLoop < MAX_CLKPI; cmdLoop++) {
            if (clkResult[cmdLoop]) {
              count1clk[cmdLoop]++;
              RcDebugPrint (SDBG_MAX,
                             "1");
            } else {
              RcDebugPrint (SDBG_MAX,
                             "0");
            }
            if ((previousStatusClk[ch][cmdLoop] != 0) & (clkResult[cmdLoop] == 0)) {
              if (piDelay > offsetClk[ch][cmdLoop]) {
                offsetClk[ch][cmdLoop] = piDelay;
              }
          }
              previousStatusClk[ch][cmdLoop] = clkResult[cmdLoop];
        }
        RcDebugPrint (SDBG_MAX,"    ");

        for (strobe = 0; strobe < MSVx4; strobe++) {

            if (dataResult[strobe]) {
              count1strobe[strobe]++;
              RcDebugPrint (SDBG_MAX,
                             "1  ");
            } else {
              RcDebugPrint (SDBG_MAX,
                             "0  ");
            }

            // Check if previous was one and current is 0
            if (previousStatusData[ch][strobe] && (dataResult[strobe] == 0)) {
              if (piDelay > offset[ch][strobe]) {
                offset[ch][strobe] = piDelay;
              }
            }
              previousStatusData[ch][strobe] = dataResult[strobe];
        }

            RcDebugPrint (SDBG_MAX,
                       "\n");
      } // piDelay loop

      //Print Number of 1s from all phase detectors
      RcDebugPrint (SDBG_MAX,
      "\n           CMDN [0][1][2][3][4][5] CMDS[0][1][2][3][4][5] CTL[1][2][3][4][5] CKE[1][2][3][4][5] CLK[0][1][2][3][4][5][6][7]     0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17\n");
      RcDebugPrint (SDBG_MAX, "COUNT NON-ZERO: ");
      for (i = 0; i < MAX_CMDPI; i++) RcDebugPrint (SDBG_MAX, "%3d", count1cmdn[i]);
      RcDebugPrint (SDBG_MAX, "     ");
      for (i = 0; i < MAX_CMDPI; i++) RcDebugPrint (SDBG_MAX, "%3d", count1cmds[i]);
      RcDebugPrint (SDBG_MAX, "    ");
      for (i = 1; i < MAX_CMDPI; i++) RcDebugPrint (SDBG_MAX, "%3d", count1ctl[i]);
      RcDebugPrint (SDBG_MAX, "    ");
      for (i = 1; i < MAX_CMDPI; i++) RcDebugPrint (SDBG_MAX, "%3d", count1cke[i]);
      RcDebugPrint (SDBG_MAX, "    ");
      for (i = 0; i < MAX_CLKPI; i++) RcDebugPrint (SDBG_MAX, "%3d", count1clk[i]);
      RcDebugPrint (SDBG_MAX, "   ");
      for (strobe = 0; strobe < MSVx4; strobe++) {
          RcDebugPrint (SDBG_MAX,
              "%3d", count1strobe[strobe]);
        }
        RcDebugPrint (SDBG_MAX,
                       "\n");

      //
      // Disable xover training mode
      //
      ddrCRCmdControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls.Bits.xovercal = 0;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

      ddrCRCmdControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS_CKE_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls.Bits.xovercal = 0;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS_CKE_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

      ddrCRCmdControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls.Bits.xovercal = 0;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

      ddrCRCmdControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS_CTL_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls.Bits.xovercal = 0;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS_CTL_MCIO_DDRIOEXT_REG, ddrCRCmdControls.Data);

      ddrCRClkControls.Data = MemReadPciCfgEp (socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG);
      ddrCRClkControls.Bits.xovercal = 0;
      MemWritePciCfgEp (socket, ch, DDRCRCLKCONTROLS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkControls);

      dataControl0.Bits.xovercal = 0;
      for (strobe = 0; strobe < MSVx4; strobe++) {
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL0N0_0_MCIO_DDRIO_REG, dataControl0.Data);
      } // strobe loop

      // List of SKX registers containing RefPiClkDelay
      //2_22_0
      //DDRDATA0N0CH0_CR_DATACONTROL4N0_0_REG
      //DDRDATA0N1CH0_CR_DATACONTROL4N1_0_REG
      //DDRDATA1N0CH0_CR_DATACONTROL4N0_1_REG
      //DDRDATA1N1CH0_CR_DATACONTROL4N1_1_REG
      //DDRDATA2N0CH0_CR_DATACONTROL4N0_2_REG
      //DDRDATA2N1CH0_CR_DATACONTROL4N1_2_REG
      //DDRDATA3N0CH0_CR_DATACONTROL4N0_3_REG
      //DDRDATA3N1CH0_CR_DATACONTROL4N1_3_REG
      //DDRDATA8N0CH0_CR_DATACONTROL4N0_8_REG
      //DDRDATA8N1CH0_CR_DATACONTROL4N1_8_REG
      //DDRDATA4N0CH0_CR_DATACONTROL4N0_4_REG
      //DDRDATA4N1CH0_CR_DATACONTROL4N1_4_REG
      //DDRDATA5N0CH0_CR_DATACONTROL4N0_5_REG
      //DDRDATA5N1CH0_CR_DATACONTROL4N1_5_REG
      //2_22_1
      //DDRDATA6N0CH0_CR_DATACONTROL4N0_6_REG
      //DDRDATA6N1CH0_CR_DATACONTROL4N1_6_REG
      //DDRDATA7N0CH0_CR_DATACONTROL4N0_7_REG
      //DDRDATA7N1CH0_CR_DATACONTROL4N1_7_REG
      //DDRCMDNCH0_CR_DDRCRCMDCONTROLS3_CMDN_REG - cmdrefpiclkdelay
      //DDRCKECH0_CR_DDRCRCMDCONTROLS3_CKE_REG - cmdrefpiclkdelay
      //DDRCLKCH0_CR_DDRCRCLKRANKSUSED_REG - refpiclk
      //DDRCMDSCH0_CR_DDRCRCMDCONTROLS3_CMDS_REG - cmdrefpiclkdelay
      //DDRCTLCH0_CR_DDRCRCMDCONTROLS3_CTL_REG - cmdrefpiclkdelay

      //
      // Determine best CMD offsets
      //
      for (i = 0; i < MAX_CMD_FUB; i++) {
        minOffset[i] = 0xFF;
        maxOffset[i] = 0;
      }

      for (cmdLoop = 0; cmdLoop < MAX_CMDPI; cmdLoop++) {
        // ICX_TODO
        // Skip PiRef0Xover PD
        if (cmdLoop == CMDPIREF0_INDEX) {
          continue;
        }

        // Get min/max values for each CMD fub
        if (offsetCmdN[ch][cmdLoop] < minOffset[CMDN_INDEX]) {
          minOffset[CMDN_INDEX] = offsetCmdN[ch][cmdLoop];
        }
        if (offsetCmdN[ch][cmdLoop] > maxOffset[CMDN_INDEX]) {
          maxOffset[CMDN_INDEX] = offsetCmdN[ch][cmdLoop];
        }

        if (offsetCmdS[ch][cmdLoop] < minOffset[CMDS_INDEX]) {
          minOffset[CMDS_INDEX] = offsetCmdS[ch][cmdLoop];
        }
        if (offsetCmdS[ch][cmdLoop] > maxOffset[CMDS_INDEX]) {
          maxOffset[CMDS_INDEX] = offsetCmdS[ch][cmdLoop];
        }

        // Special case for SKX phase detector not implemented
        if (cmdLoop == 0) {
          continue;
        }
        if (offsetCtl[ch][cmdLoop] < minOffset[CTL_INDEX]) {
          minOffset[CTL_INDEX] = offsetCtl[ch][cmdLoop];
        }
        if (offsetCtl[ch][cmdLoop] > maxOffset[CTL_INDEX]) {
          maxOffset[CTL_INDEX] = offsetCtl[ch][cmdLoop];
        }

        if (offsetCke[ch][cmdLoop] < minOffset[CKE_INDEX]) {
          minOffset[CKE_INDEX] = offsetCke[ch][cmdLoop];
        }
        if (offsetCke[ch][cmdLoop] > maxOffset[CKE_INDEX]) {
          maxOffset[CKE_INDEX] = offsetCke[ch][cmdLoop];
        }
      }
      for (i = 0; i < MAX_CMD_FUB; i++) {
        if (maxOffset[i] - minOffset[i] > X_OVER_MAX_SAMPLE/2){
            midOffset[i] =((minOffset[i] + maxOffset[i] + X_OVER_MAX_SAMPLE) / 2)%X_OVER_MAX_SAMPLE;
        } else {
        midOffset[i] = (minOffset[i] + maxOffset[i]) / 2;
        }
      }

      minClkOffset = 0xFF;
      maxClkOffset = 0;
      for (cmdLoop = 0; cmdLoop < MAX_CLKPI; cmdLoop++) {

        // Get min/max values for each CMD fub
        if (offsetClk[ch][cmdLoop] < minClkOffset) {
          minClkOffset = offsetClk[ch][cmdLoop];
        }
        if (offsetClk[ch][cmdLoop] > maxClkOffset) {
          maxClkOffset = offsetClk[ch][cmdLoop];
        }
      }
      if (maxClkOffset - minClkOffset > X_OVER_MAX_SAMPLE/2){
        midClkOffset = ((minClkOffset + maxClkOffset + X_OVER_MAX_SAMPLE) / 2)%X_OVER_MAX_SAMPLE;
      } else {
        midClkOffset = (minClkOffset + maxClkOffset) / 2;
      }

      //
      // Program offsets
      //
      if (EFI_SUCCESS != GetXoverOffsets (socket, ch, &XoverOffsets)) {
        RcDebugPrint (SDBG_MAX,
                      "!! ProgramOffsets - Failed to find offset table !!\n");
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_142);
        return FAILURE;
      }
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        " CMDN REF0 CMDS REF0 CTL REF0 CKE REF0 CLK  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17\n");

      piDelay = midOffset[CMDN_INDEX];
      RcDebugPrint (SDBG_MAX,
                     "offsets: %2d", piDelay);
      ddrCRCmdControls3.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls3.Bits.cmdrefpiclkdelay = (piDelay + XoverOffsets->CmdOffset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdControls3.Data);
      (*channelNvList)[ch].ddrCRCmdControls3CmdN = ddrCRCmdControls3.Data;

      ddrCRCmdNTraining.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCmdNTraining.Bits.trainingoffset = (piDelay + XoverOffsets->CmdOffset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdNTraining.Data);
      (*channelNvList)[ch].ddrCRCmdTrainingCmdN = ddrCRCmdNTraining.Data;

      ddrCRCmdNPiCoding.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_REG);
      // 2 MSB of piDelay

      MemWritePciCfgEp (socket, ch, DDRCRCMDPICODING_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdNPiCoding.Data);
      GetCmdGroupAddress10nm(Host, 0, SIDE_A, &regIndex);
      (*channelNvList)[ch].cmdCsr[regIndex] = ddrCRCmdNPiCoding.Data;

      // ICX_TODO
      // For PiRef0 (cmdLoop = CMDPIREF0_INDEX) use offSet directly
      piDelay = offsetCmdN[ch][CMDPIREF0_INDEX];
      RcDebugPrint (SDBG_MAX,
                     "   %2d", piDelay);
      ddrCRCmdNPiCoding4.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_REG);
      ddrCRCmdNPiCoding4.Bits.cmdrefpi0picode = (piDelay + XoverOffsets->CmdPi0Offset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDPICODING4_CMDN_MCIO_DDRIOEXT_REG, ddrCRCmdNPiCoding4.Data);
      GetCmdGroupAddress10nm(Host, 9, SIDE_A, &regIndex);
      (*channelNvList)[ch].cmdCsr[regIndex] = ddrCRCmdNPiCoding4.Data;

      piDelay = midOffset[CMDS_INDEX];
      RcDebugPrint (SDBG_MAX,
                     "   %2d", piDelay);
      ddrCRCmdControls3.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls3.Bits.cmdrefpiclkdelay = (piDelay + XoverOffsets->CmdOffset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdControls3.Data);
      (*channelNvList)[ch].ddrCRCmdControls3CmdS = ddrCRCmdControls3.Data;

      ddrCRCmdSTraining.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG);
      ddrCRCmdSTraining.Bits.trainingoffset = (piDelay + XoverOffsets->CmdOffset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdSTraining.Data);
      (*channelNvList)[ch].ddrCRCmdTrainingCmdS = ddrCRCmdSTraining.Data;

      ddrCRCmdSPiCoding.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING_CMDS_MCIO_DDRIOEXT_REG);
      (*channelNvList)[ch].cmdCsr[regIndex] = ddrCRCmdSPiCoding.Data;

      // ICX_TODO
      // For PiRef0 (cmdLoop = CMDPIREF0_INDEX) use offSet directly
      piDelay = offsetCmdS[ch][CMDPIREF0_INDEX];
      RcDebugPrint (SDBG_MAX,
        "   %2d", piDelay);
      ddrCRCmdSPiCoding4.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_REG);
      ddrCRCmdSPiCoding4.Bits.cmdrefpi0picode = (piDelay + XoverOffsets->CmdPi0Offset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDPICODING4_CMDS_MCIO_DDRIOEXT_REG, ddrCRCmdSPiCoding4.Data);
      GetCmdGroupAddress10nm(Host, 9 + CC_SOUTH, SIDE_A, &regIndex);
      (*channelNvList)[ch].cmdCsr[regIndex] = ddrCRCmdSPiCoding4.Data;

      piDelay = midOffset[CTL_INDEX];
      RcDebugPrint (SDBG_MAX,
                     "  %2d", piDelay);
      ddrCRCmdControls3.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls3.Bits.cmdrefpiclkdelay = (piDelay + XoverOffsets->CmdOffset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG, ddrCRCmdControls3.Data);
      (*channelNvList)[ch].ddrCRCmdControls3Ctl = ddrCRCmdControls3.Data;

      ddrCRCtlTraining.Data = (UINT16)MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG);
      ddrCRCtlTraining.Bits.trainingoffset = (piDelay + XoverOffsets->CmdOffset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlTraining.Data);
      (*channelNvList)[ch].ddrCRCtlTraining = ddrCRCtlTraining.Data;

      ddrCRCtlPiCoding.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING_CTL_MCIO_DDRIOEXT_REG);

      MemWritePciCfgEp (socket, ch, DDRCRCMDPICODING_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlPiCoding.Data);
      GetCmdGroupAddress10nm(Host, CC_SOUTH, SIDE_B, &regIndex);
      (*channelNvList)[ch].cmdCsr[regIndex] = ddrCRCtlPiCoding.Data;

      // ICX_TODO
      // For PiRef0 (cmdLoop = CMDPIREF0_INDEX) use offSet directly
      piDelay = offsetCtl[ch][CMDPIREF0_INDEX];
      RcDebugPrint (SDBG_MAX,
        "   %2d", piDelay);
      ddrCRCtlPiCoding4.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_REG);
      ddrCRCtlPiCoding4.Bits.cmdrefpi0picode = (piDelay + XoverOffsets->CmdPi0Offset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDPICODING4_CTL_MCIO_DDRIOEXT_REG, ddrCRCtlPiCoding4.Data);
      GetCmdGroupAddress10nm(Host, 9 + CC_SOUTH, SIDE_B, &regIndex);
      (*channelNvList)[ch].cmdCsr[regIndex] = ddrCRCtlPiCoding4.Data;

      piDelay = midOffset[CKE_INDEX];
      RcDebugPrint (SDBG_MAX,
                     "  %2d", piDelay);
      ddrCRCmdControls3.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG);
      ddrCRCmdControls3.Bits.cmdrefpiclkdelay = (piDelay + XoverOffsets->CmdOffset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG, ddrCRCmdControls3.Data);
      (*channelNvList)[ch].ddrCRCmdControls3Cke = ddrCRCmdControls3.Data;

      ddrCRCkeTraining.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG);
      ddrCRCkeTraining.Bits.trainingoffset = (piDelay + XoverOffsets->CmdOffset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG, ddrCRCkeTraining.Data);
      (*channelNvList)[ch].ddrCRCkeTraining = ddrCRCkeTraining.Data;

      ddrCRCkePiCoding.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING_CKE_MCIO_DDRIOEXT_REG);

      MemWritePciCfgEp (socket, ch, DDRCRCMDPICODING_CKE_MCIO_DDRIOEXT_REG, ddrCRCkePiCoding.Data);
      GetCmdGroupAddress10nm(Host, 0, SIDE_B, &regIndex);
      (*channelNvList)[ch].cmdCsr[regIndex] = ddrCRCkePiCoding.Data;

      // ICX_TODO
      // For PiRef0 (cmdLoop = CMDPIREF0_INDEX) use offSet directly
      piDelay = offsetCke[ch][CMDPIREF0_INDEX];
      RcDebugPrint (SDBG_MAX,
        "   %2d", piDelay);
      ddrCRCkePiCoding4.Data = MemReadPciCfgEp (socket, ch, DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_REG);
      ddrCRCkePiCoding4.Bits.cmdrefpi0picode = (piDelay + XoverOffsets->CmdPi0Offset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCMDPICODING4_CKE_MCIO_DDRIOEXT_REG, ddrCRCkePiCoding4.Data);
      GetCmdGroupAddress10nm(Host, 9, SIDE_B, &regIndex);
      (*channelNvList)[ch].cmdCsr[regIndex] = ddrCRCkePiCoding4.Data;

      piDelay = midClkOffset;
      RcDebugPrint (SDBG_MAX,
                     "  %2d ", piDelay);
      ddrCRClkRanksUsed.Data = MemReadPciCfgEp (socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG);
      ddrCRClkRanksUsed.Bits.refpiclk = (piDelay + XoverOffsets->ClkOffset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, ddrCRClkRanksUsed.Data);
      (*channelNvList)[ch].ddrCRClkRanksUsed = ddrCRClkRanksUsed.Data;

      ddrCRClkTraining.Data = MemReadPciCfgEp (socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG);
      ddrCRClkTraining.Bits.trainingoffset = (piDelay + XoverOffsets->ClkOffset) % X_OVER_MAX_SAMPLE;
      MemWritePciCfgEp (socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, ddrCRClkTraining.Data);
      (*channelNvList)[ch].ddrCRClkTraining = ddrCRClkTraining.Data;

      // SKX change for upper nibble
      for (strobe = 0; strobe < MSVx4; strobe++) {

        piDelay = offset[ch][strobe];
        RcDebugPrint (SDBG_MAX,
                       " %2d", piDelay);
        dataControl4.Data = (*channelNvList)[ch].dataControl4[strobe];
        dataControl4.Bits.refpiclkdelay = (piDelay + XoverOffsets->DataOffset) % X_OVER_MAX_SAMPLE;
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, dataControl4.Data);
        (*channelNvList)[ch].dataControl4[strobe] = dataControl4.Data;

        dataOffsetTrain.Data = 0;
        dataOffsetTrain.Bits.rcvenoffset = (piDelay + XoverOffsets->DataOffset) % X_OVER_MAX_SAMPLE;
        dataOffsetTrain.Bits.txdqoffset = (piDelay + XoverOffsets->DataOffset) % X_OVER_MAX_SAMPLE;
        dataOffsetTrain.Bits.txdqsoffset = (piDelay + XoverOffsets->DataOffset) % X_OVER_MAX_SAMPLE;
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATAOFFSETTRAINN0_0_MCIO_DDRIO_REG, dataOffsetTrain.Data);
        (*channelNvList)[ch].dataOffsetTrain[strobe] = dataOffsetTrain.Data;

      } // strobe loop
      RcDebugPrint (SDBG_MAX, "\n");

      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        " Additional Xover PiDelay offset for ICX: CMD = %d, CMD PIREF0 = %d, CLK = %d, DATA = %d\n",
        XoverOffsets->CmdOffset, XoverOffsets->CmdPi0Offset, XoverOffsets->ClkOffset, XoverOffsets->DataOffset);
    } // ch loop


  } else if ((Host->var.mem.subBootMode == WarmBootFast) || (Host->var.mem.subBootMode == ColdBootFast)) {

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDN_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdTrainingCmdN);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CMDS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdTrainingCmdS);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CTL_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCtlTraining);
      MemWritePciCfgEp (socket, ch, DDRCRCMDTRAINING_CKE_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCkeTraining);
      MemWritePciCfgEp (socket, ch, DDRCRCLKTRAINING_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkTraining);

      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDN_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3CmdN);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CMDS_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3CmdS);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CTL_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3Ctl);
      MemWritePciCfgEp (socket, ch, DDRCRCMDCONTROLS3_CKE_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRCmdControls3Cke);
      MemWritePciCfgEp (socket, ch, DDRCRCLKRANKSUSED_MCIO_DDRIOEXT_REG, (*channelNvList)[ch].ddrCRClkRanksUsed);

      // SKX change for upper nibble
      for (strobe = 0; strobe < MSVx4; strobe++) {

        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATAOFFSETTRAINN0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataOffsetTrain[strobe]);
        WriteDdrioDataRegCh (Host, socket, ch, strobe, DATACONTROL4N0_0_MCIO_DDRIO_REG, (*channelNvList)[ch].dataControl4[strobe]);
      } // strobe loop
    }
  }

#ifdef DEBUG_CODE_BLOCK
  DisplayXoverResults (Host, socket);
  ReleasePrintControl ();
#endif //DEBUG_CODE_BLOCK
  return SUCCESS;
} // XoverCalib10
#endif // #if (MEM_IP_VER < MEM_IP_17ww37e)

/**

  Determine which version of crossover calibration to call based on CRIF version

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS

**/
UINT32
CrossoverCalib10nm (
  PSYSHOST  Host
  )
{
  UINT32    Status;
  PT_ZONE   MemPhaseSave;

  if (!IsMemFlowEnabled (CrossoverCalibration)) {
    return (SUCCESS);
  }
#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif // DEBUG_CODE_BLOCK
  GetMemPhase (&MemPhaseSave);
  SetMemPhaseCP (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_CROSSOVER_CALIBRATION);
#if (MEM_IP_VER >= MEM_IP_17ww37e)
  Status = XoverCalib (Host);
#else // (MEM_IP_VER >= MEM_IP_17ww37e)
  Status = XoverCalibPre17ww37e (Host);
#endif // (MEM_IP_VER >= MEM_IP_17ww37e)
  SetMemPhase (MemPhaseSave);
  return (Status);
}

#ifdef DEBUG_CODE_BLOCK
/*++

  Displays the training results

  @param[in] Host:         Pointer to sysHost
  @param[in] Socket:       Processor socket

  @retval N/A

--*/
VOID
DisplayXoverResults (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  UINT8               Ch;
  UINT8               Strobe;
  struct channelNvram (*ChannelNvList) [MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  RcDebugPrint (SDBG_MAX,
    "\nSTART_DATA_XOVER\n");

  RcDebugPrint (SDBG_MAX,
    "        CMDN CMDS  CTL  CKE  CLK    0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17\n");

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList) [Ch].enabled == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, " %3d", (*ChannelNvList) [Ch].ddrCRCmdTrainingCmdN & 0x3f);
    RcDebugPrint (SDBG_MAX, "  %3d", (*ChannelNvList) [Ch].ddrCRCmdTrainingCmdS & 0x3f);
    RcDebugPrint (SDBG_MAX, "  %3d", (*ChannelNvList) [Ch].ddrCRCtlTraining & 0x3f);
    RcDebugPrint (SDBG_MAX, "  %3d", (*ChannelNvList) [Ch].ddrCRCkeTraining & 0x3f);
    RcDebugPrint (SDBG_MAX, "  %3d", (*ChannelNvList) [Ch].ddrCRClkTraining & 0x3f);

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      RcDebugPrint (SDBG_MAX, "  %3d", GetCachedRcvenXoverOffsetDdrio (Host, Socket, Ch, Strobe));
    }
    RcDebugPrint (SDBG_MAX,
      "\n");
  }
} // DisplayXoverResults
#endif //DEBUG_CODE_BLOCK

