/** @file
  Memory crossover calibration.

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
#include <Library/CheckpointLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/Ddr5CoreLib.h>
#include "MemCmdControl.h"
#include "Include/MemIoControl.h"
#include <Include/MemXoverCalib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/PerformanceTrackerLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include "Include/MemDdrioPrivate.h"
#include <Library/SysHostPointerLib.h>

#define XOVER_PIREF_SETUP_ADJ  0x3F
#define MAX_ESTIMATED_FSM_TIME_3200  2400 //ns
#define MAX_ESTIMATED_FSM_TIME_4800  1600 //ns

#define CMD_CTL_FUB_NUM         4
#define CLK_FUB_NUM             2


/**

  Print the header with signal and strobe counts.

  @param IN Socket     - Current CPU socket
  @param IN Ch         - Current channel
  @param IN SignalsStr - The signals to print
  @param IN StrobesMax - The max number of strobe

**/
VOID
PrintSignalWithStrobe (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    StrobeMax,
  IN CHAR8 *  SignalsStr
  )
{
  UINT8                                           Strobe;

  RcDebugPrint (SDBG_MAX,
    "\n");

  RcDebugPrint (SDBG_MAX,  SignalsStr);

  for (Strobe = 0;Strobe < StrobeMax;Strobe++) {
    RcDebugPrint (SDBG_MAX,
      " %02d", Strobe);
  }
  RcDebugPrint (SDBG_MAX, "\n");
}

/*++

  Displays the Xover training results

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
  UINT8                                       Ch;
  UINT8                                       Strobe;
  UINT8                                       SubCh;
  UINT8                                       MSVx4;
  DDRIO_CHANNEL_NVDATA                        (*DdrioChannelNvList) [MAX_CH];
  DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset00[SUB_CH];
  DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset01[SUB_CH];
  DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset02[SUB_CH];
  DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset03[SUB_CH];
  DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT DdrclkPirefOffset00;
  DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT DdrclkPirefOffset01;
  DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_STRUCT     DataOffsetTrain[MAX_STROBE];
  UINT8                                       MaxChDdr;
  UINT32                                      DbgLevel;

  //
  // Every print in this function should use the same level.
  //
  DbgLevel = SDBG_MAX;

  //
  // If this function can't print, don't bother to read anything, but
  // return early instead.
  //
  if (!CheckDebugPrintLevel (DbgLevel)) {
    return;
  }

  MSVx4 = GetMaxStrobeValid (Host);
  MaxChDdr = GetMaxChDdr ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    //
    // Read all CSRs up-front so CSR tracing (if enabled) does not interrupt
    // the results as they are printed below.
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DdrccPirefOffset00[SubCh].Data = (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset00;
      DdrccPirefOffset01[SubCh].Data = (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset01;
      DdrccPirefOffset02[SubCh].Data = (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset02;
      DdrccPirefOffset03[SubCh].Data = (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset03;
    }
    DdrclkPirefOffset00.Data = (*DdrioChannelNvList) [Ch].DdrclkPirefOffset00;
    DdrclkPirefOffset01.Data = (*DdrioChannelNvList) [Ch].DdrclkPirefOffset01;
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      DataOffsetTrain[Strobe].Data = (*DdrioChannelNvList) [Ch].DdrdN0PirefOffset0[Strobe];
    }
  }

  //
  // Print the results.
  //
  RcDebugPrint (DbgLevel, "\nSTART_DATA_CCC_XOVER_PIREF_OFFSET\n");
  RcDebugPrint (SDBG_MAX, "SIGNAL:       CC0   CC1   CC2   CC3   CK0   CK1\n");
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (SubCh == 0) {
        RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT, "    %d     %d     %d     %d     %d     %d\n",
          DdrccPirefOffset00[SubCh].Bits.cmd_piref_offset,
          DdrccPirefOffset01[SubCh].Bits.cmd_piref_offset,
          DdrccPirefOffset02[SubCh].Bits.cmd_piref_offset,
          DdrccPirefOffset03[SubCh].Bits.cmd_piref_offset,
          DdrclkPirefOffset00.Bits.clk_piref_offset,
          DdrclkPirefOffset01.Bits.clk_piref_offset);
      } else {
        RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT, "    %d     %d     %d     %d            \n",
          DdrccPirefOffset00[SubCh].Bits.cmd_piref_offset,
          DdrccPirefOffset01[SubCh].Bits.cmd_piref_offset,
          DdrccPirefOffset02[SubCh].Bits.cmd_piref_offset,
          DdrccPirefOffset03[SubCh].Bits.cmd_piref_offset);
      }
    }
  }
  RcDebugPrint (DbgLevel, "STOP_DATA_CCC_XOVER_PIREF_OFFSET\n");

  RcDebugPrint (DbgLevel, "\nSTART_DATA_CCC_XOVER_PIO_CODE\n");
  RcDebugPrint (SDBG_MAX, "SIGNAL:       CC0   CC1   CC2   CC3   CK0   CK1\n");
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      if (SubCh == 0) {
        RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT, "    %d     %d     %d     %d     %d     %d\n",
          DdrccPirefOffset00[SubCh].Bits.piref_train_pio_code,
          DdrccPirefOffset01[SubCh].Bits.piref_train_pio_code,
          DdrccPirefOffset02[SubCh].Bits.piref_train_pio_code,
          DdrccPirefOffset03[SubCh].Bits.piref_train_pio_code,
          DdrclkPirefOffset00.Bits.piref_train_pio_code,
          DdrclkPirefOffset01.Bits.piref_train_pio_code);
      } else {
        RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT, "    %d     %d     %d     %d            \n",
          DdrccPirefOffset00[SubCh].Bits.piref_train_pio_code,
          DdrccPirefOffset01[SubCh].Bits.piref_train_pio_code,
          DdrccPirefOffset02[SubCh].Bits.piref_train_pio_code,
          DdrccPirefOffset03[SubCh].Bits.piref_train_pio_code);
      }
    }
  }
  RcDebugPrint (DbgLevel, "STOP_DATA_CCC_XOVER_PIO_CODE\n");

  RcDebugPrint (DbgLevel, "\nSTART_DATA_DATA_XOVER_RCVEN_OFFSET\n");
  RcDebugPrint (SDBG_MAX, "NIBBLE:     ");
  for (Strobe = 0; Strobe < MSVx4/2; Strobe++) {
    RcDebugPrint (SDBG_MAX, "  %2d ", Strobe);
  }
  RcDebugPrint (SDBG_MAX, "\n");
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SUB_CH_A, NO_RANK, NO_STROBE, NO_BIT, "");
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (Strobe == (MSVx4 / 2)) {
        RcDebugPrint (SDBG_MEM_TRAIN, "\n");
        RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SUB_CH_B, NO_RANK, NO_STROBE, NO_BIT, "");
      }
      RcDebugPrint (SDBG_MAX, "  %2d ", DataOffsetTrain[Strobe].Bits.rcven_piref_offset);
    }
    RcDebugPrint (SDBG_MAX, "\n");
  } //ch loop
  RcDebugPrint (DbgLevel, "STOP_DATA_DATA_XOVER_RCVEN_OFFSET\n");

  RcDebugPrint (DbgLevel, "\nSTART_DATA_DATA_XOVER_TXDQS_OFFSET\n");
  RcDebugPrint (SDBG_MAX, "NIBBLE:     ");
  for (Strobe = 0; Strobe < MSVx4/2; Strobe++) {
    RcDebugPrint (SDBG_MAX, "  %2d ", Strobe);
  }
  RcDebugPrint (SDBG_MAX, "\n");
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SUB_CH_A, NO_RANK, NO_STROBE, NO_BIT, "");
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (Strobe == (MSVx4 / 2)) {
        RcDebugPrint (SDBG_MEM_TRAIN, "\n");
        RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SUB_CH_B, NO_RANK, NO_STROBE, NO_BIT, "");
      }
      RcDebugPrint (SDBG_MAX, "  %2d ", DataOffsetTrain[Strobe].Bits.txdqs_piref_offset);
    }
    RcDebugPrint (SDBG_MAX, "\n");
  } //ch loop
  RcDebugPrint (DbgLevel, "STOP_DATA_DATA_XOVER_TXDQS_OFFSET\n");

  RcDebugPrint (DbgLevel, "\nSTART_DATA_DATA_XOVER_TXDQ_OFFSET\n");
  RcDebugPrint (SDBG_MAX, "NIBBLE:     ");
  for (Strobe = 0; Strobe < MSVx4/2; Strobe++) {
    RcDebugPrint (SDBG_MAX, "  %2d ", Strobe);
  }
  RcDebugPrint (SDBG_MAX, "\n");
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SUB_CH_A, NO_RANK, NO_STROBE, NO_BIT, "");
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (Strobe == (MSVx4 / 2)) {
        RcDebugPrint (SDBG_MEM_TRAIN, "\n");
        RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SUB_CH_B, NO_RANK, NO_STROBE, NO_BIT, "");
      }
      RcDebugPrint (SDBG_MAX, "  %2d ", DataOffsetTrain[Strobe].Bits.txdq_piref_offset);
    }
    RcDebugPrint (SDBG_MAX, "\n");
  } //ch loop
  RcDebugPrint (DbgLevel, "STOP_DATA_DATA_XOVER_TXDQ_OFFSET\n");

  RcDebugPrint (DbgLevel, "\nSTART_DATA_DATA_XOVER_PI_CODE\n");
  RcDebugPrint (SDBG_MAX, "NIBBLE:     ");
  for (Strobe = 0; Strobe < MSVx4/2; Strobe++) {
    RcDebugPrint (SDBG_MAX, "  %2d ", Strobe);
  }
  RcDebugPrint (SDBG_MAX, "\n");
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SUB_CH_A, NO_RANK, NO_STROBE, NO_BIT, "");
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      if (Strobe == (MSVx4 / 2)) {
        RcDebugPrint (SDBG_MEM_TRAIN, "\n");
        RcDebugPrintWithDevice (DbgLevel, Socket, Ch, NO_DIMM, SUB_CH_B, NO_RANK, NO_STROBE, NO_BIT, "");
      }
      RcDebugPrint (SDBG_MAX, "  %2d ", DataOffsetTrain[Strobe].Bits.piref_train_pi_code);
    }
    RcDebugPrint (SDBG_MAX, "\n");
  } //ch loop
  RcDebugPrint (DbgLevel, "STOP_DATA_DATA_XOVER_PI_CODE\n");
} // DisplayXoverResults

/**

  Enable or Disable crossover calibration.

  @param[in] Host   - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket - Current CPU socket
  @param[in] Ch     - Current channel
  @param[in] State  - Enable = 1, Disable = 0

  @retval EFI_SUCCESS

**/
EFI_STATUS
EnableDisableXoverCalRegs (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    State
  )
{
  UINT8                                   Strobe;
  DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT   DdrccTrainCtl1Ext;
  DDRCLK_TRAIN_CTL1_MCIO_DDRIOEXT_STRUCT  DdrclkTrainCtl1Ext;
  DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_STRUCT    DdrdN0TrainCtl1;
  DDRIO_CHANNEL_NVDATA                    (*DdrioChannelNvList) [MAX_CH];
  UINT8                                   SubCh;
  UINT8                                   MSVx4;

  MSVx4 = GetMaxStrobeValid (Host);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  //
  // Program xover_cal for CMD/CTL
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    DdrccTrainCtl1Ext.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG);
    DdrccTrainCtl1Ext.Bits.xover_cal = State;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_CTL1_MCIO_DDRIOEXT_REG, DdrccTrainCtl1Ext.Data);
  } //subch loop

  //
  // Program xover_cal for CLK
  //
  DdrclkTrainCtl1Ext.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_TRAIN_CTL1_MCIO_DDRIOEXT_REG);
  DdrclkTrainCtl1Ext.Bits.xover_cal = State;
  MemWritePciCfgEp (Socket, Ch, DDRCLK_TRAIN_CTL1_MCIO_DDRIOEXT_REG, DdrclkTrainCtl1Ext.Data);

  //
  // Program xover_cal for Data
  //
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    DdrdN0TrainCtl1.Data = (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe];
    DdrdN0TrainCtl1.Bits.xover_cal = State;
    (*DdrioChannelNvList)[Ch].DdrdN0TrainCtl1[Strobe] = DdrdN0TrainCtl1.Data;
    MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL1_MCIO_DDRIO_REG), DdrdN0TrainCtl1.Data);
  } // Strobe loop

  if (State == XOVER_CAL_DISABLE) {
    //
    // Delay to allow propigation in CTE. From design, it should be no need to add additional delay in real silicon.
    //
    CteDelayTime (3200);
  }
  return EFI_SUCCESS;
} // EnableDisableXoverCalRegs


/**

Select which Xover phase detect output will be used for calibration.

@param Host   - Pointer to sysHost, the system Host (root) structure struct
@param Socket - Current CPU socket

@retval EFI_SUCCESS

**/
EFI_STATUS
SelectXoverPhaseDetectOutput (
  IN  PSYSHOST Host,
  IN  UINT8    Socket
  )
{
  UINT8                                   Ch = 0;
  UINT8                                   Strobe;
  UINT8                                   SubCh = 0;
  DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_STRUCT    DdrdTrainCtl2;
  DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_STRUCT   DdrccTrainCtl2Ext;
  DDRCLK_TRAIN_CTL2_MCIO_DDRIOEXT_STRUCT  DdrclkTrainCtl2Ext;
  UINT8                                   MSVx4 = GetMaxStrobeValid (Host);
  UINT8                                   MaxChDdr;
  MaxChDdr = GetMaxChDdr ();
  //
  // The value for the cal_mux_sel may be revisited during post silicon.
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Select xover phase detect output for Data.
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      DdrdTrainCtl2.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG));
      DdrdTrainCtl2.Bits.xover_cal_mux_sel = 0;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_CTL2_MCIO_DDRIO_REG), DdrdTrainCtl2.Data);
    }
    //
    //Select xover phase detect output for CMD/CTL.
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DdrccTrainCtl2Ext.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG);
      DdrccTrainCtl2Ext.Bits.dll0_top_xover_cal_mux_sel = 0;
      DdrccTrainCtl2Ext.Bits.dll0_bot_xover_cal_mux_sel = 0;
      DdrccTrainCtl2Ext.Bits.dll1_top_xover_cal_mux_sel = 0;
      DdrccTrainCtl2Ext.Bits.dll1_bot_xover_cal_mux_sel = 0;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_CTL2_MCIO_DDRIOEXT_REG, DdrccTrainCtl2Ext.Data);
    } //subch loop
    //
    //Select xover phase detect output for CLK.
    //
    DdrclkTrainCtl2Ext.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_TRAIN_CTL2_MCIO_DDRIOEXT_REG);
    DdrclkTrainCtl2Ext.Bits.dll_top_xover_cal_mux_sel = 0;
    DdrclkTrainCtl2Ext.Bits.dll_bot_xover_cal_mux_sel = 0;
    MemWritePciCfgEp (Socket, Ch, DDRCLK_TRAIN_CTL2_MCIO_DDRIOEXT_REG, DdrclkTrainCtl2Ext.Data);
  } // Ch loop
  return EFI_SUCCESS;
} // SelectXoverPhaseDetectOutput

  /**

  Program the PIREF code.

  @param[in] Host      - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket    - Current CPU socket
  @param[in] Value     - Value for program
  @param[in] *OffsetSt - offset
  @retval EFI_SUCCESS

  **/
EFI_STATUS
ProgramPirefCode (
  IN  PSYSHOST       Host,
  IN  UINT8          Socket,
  IN  UINT8          Ch,
  IN  OFFSET_STRUCT  *OffsetSt
  )
{
  UINT8                                       Strobe;
  UINT8                                       SubCh = 0;
  DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_STRUCT     DdrdN0PirefOffset0;
  DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset00;
  DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset01;
  DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset02;
  DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset03;
  DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT  DdrclkPirefOffset00;
  DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT  DdrclkPirefOffset01;
  DDRIO_CHANNEL_NVDATA            (*DdrioChannelNvList) [MAX_CH];

  UINT8                                        MSVx4 = GetMaxStrobeValid (Host);

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  //
  // Program piref_train_pio_code  for Data.
  //
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    DdrdN0PirefOffset0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG));
    DdrdN0PirefOffset0.Bits.piref_train_pi_code = (OffsetSt->Offset[Ch][Strobe]) % X_OVER_MAX_SAMPLE;
    (*DdrioChannelNvList) [Ch].DdrdN0PirefOffset0[Strobe] = DdrdN0PirefOffset0.Data;
    MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG), DdrdN0PirefOffset0.Data);
  }
  //
  // Program PIREF offset 00 piref_train_pio_code for CMD/CTL.
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    DdrccPirefOffset00.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset00.Bits.piref_train_pio_code = (OffsetSt->CmdCtl[Ch][SubCh][0]) % X_OVER_MAX_SAMPLE;
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset00 = DdrccPirefOffset00.Data;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, DdrccPirefOffset00.Data);
    //
    // Program PIREF offset 01 piref_train_pio_code for CMD/CTL.
    //
    DdrccPirefOffset01.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset01.Bits.piref_train_pio_code = (OffsetSt->CmdCtl[Ch][SubCh][1]) % X_OVER_MAX_SAMPLE;
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset01 = DdrccPirefOffset01.Data;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, DdrccPirefOffset01.Data);
    //
    // Program PIREF offset 02 piref_train_pio_code for CMD/CTL.
    //
    DdrccPirefOffset02.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset02.Bits.piref_train_pio_code = (OffsetSt->CmdCtl[Ch][SubCh][2]) % X_OVER_MAX_SAMPLE;
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset02 = DdrccPirefOffset02.Data;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG, DdrccPirefOffset02.Data);
    //
    // Program PIREF offset 03 piref_train_pio_code for CMD/CTL.
    //
    DdrccPirefOffset03.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset03.Bits.piref_train_pio_code = (OffsetSt->CmdCtl[Ch][SubCh][3]) % X_OVER_MAX_SAMPLE;
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset03 = DdrccPirefOffset03.Data;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG, DdrccPirefOffset03.Data);
  }
  //
  // Program PIREF offset 00 piref_train_pio_code for CLK.
  //
  DdrclkPirefOffset00.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG);
  DdrclkPirefOffset00.Bits.piref_train_pio_code = (OffsetSt->Clk[Ch][0]) % X_OVER_MAX_SAMPLE;
  (*DdrioChannelNvList) [Ch].DdrclkPirefOffset00 = DdrclkPirefOffset00.Data;
  MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, DdrclkPirefOffset00.Data);
  //
  // Program PIREF offset 01 piref_train_pio_code for CLK.
  //
  DdrclkPirefOffset01.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG);
  DdrclkPirefOffset01.Bits.piref_train_pio_code = (OffsetSt->Clk[Ch][1]) % X_OVER_MAX_SAMPLE;
  (*DdrioChannelNvList) [Ch].DdrclkPirefOffset01 = DdrclkPirefOffset01.Data;
  MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, DdrclkPirefOffset01.Data);
  return EFI_SUCCESS;
} // ProgramPirefCode

  /**

  Program the PIREF Setup Adjust.

  @param[in] Host      - Pointer to sysHost, the system Host (root) structure struct
  @param[in] Socket    - Current CPU socket
  @param[in] Ch        - Ch number
  @param[in] Value     - Value for program

  @retval EFI_SUCCESS

  **/
EFI_STATUS
ProgramPirefSetupAdj (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    Value
  )
{
  UINT8                                       Strobe;
  UINT8                                       SubCh = 0;
  DDRD_N0_PIREF_OFFSET1_MCIO_DDRIO_STRUCT     DdrdPirefOffset1;
  DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset00;
  DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset01;
  DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset02;
  DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_STRUCT  DdrccPirefOffset03;

  DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT  DdrclkPirefOffset00;
  DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT  DdrclkPirefOffset01;

  DDRIO_CHANNEL_NVDATA            (*DdrioChannelNvList) [MAX_CH];

  UINT8                                        MSVx4 = GetMaxStrobeValid (Host);
  UINT8                                        MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  //
  // Program piref_setup_adj  for Data.
  //
  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    DdrdPirefOffset1.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET1_MCIO_DDRIO_REG));
    DdrdPirefOffset1.Bits.piref_setup_adj = Value;
    (*DdrioChannelNvList) [Ch].DdrdN0PirefOffset1[Strobe] = DdrdPirefOffset1.Data;
    MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET1_MCIO_DDRIO_REG), DdrdPirefOffset1.Data);
  }
  //
  // Program PIREF offset 00 piref_setup_adj for CMD/CTL.
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    DdrccPirefOffset00.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset00.Bits.piref_setup_adj = Value;
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset00 = DdrccPirefOffset00.Data;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, DdrccPirefOffset00.Data);
    //
    // Program PIREF offset 01 piref_setup_adj for CMD/CTL.
    //
    DdrccPirefOffset01.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset01.Bits.piref_setup_adj = Value;
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset01 = DdrccPirefOffset01.Data;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, DdrccPirefOffset01.Data);
    //
    // Program PIREF offset 02 piref_setup_adj for CMD/CTL.
    //
    DdrccPirefOffset02.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset02.Bits.piref_setup_adj = Value;
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset02 = DdrccPirefOffset02.Data;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG, DdrccPirefOffset02.Data);
    //
    // Program PIREF offset 03 piref_setup_adj for CMD/CTL.
    //
    DdrccPirefOffset03.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset03.Bits.piref_setup_adj = Value;
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset03 = DdrccPirefOffset03.Data;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG, DdrccPirefOffset03.Data);
  }
  //
  // Program PIREF offset 00 piref_setup_adj for CLK.
  //
  DdrclkPirefOffset00.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG);
  DdrclkPirefOffset00.Bits.piref_setup_adj = Value;
  MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, DdrclkPirefOffset00.Data);
  //
  // Program PIREF offset 01 piref_setup_adj for CLK.
  //
  DdrclkPirefOffset01.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG);
  DdrclkPirefOffset01.Bits.piref_setup_adj = Value;
  MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, DdrclkPirefOffset01.Data);

  return EFI_SUCCESS;
} // ProgramPirefSetupAdj

  /**

  Configure and Start the HW FSM.

  @param Host   - Pointer to sysHost, the system Host (root) structure struct
  @param Socket - Current CPU socket

  @retval EFI_SUCCESS

  **/
EFI_STATUS
ConfigureStartHwFsm (
  IN  PSYSHOST Host,
  IN  UINT8    Socket
  )
{
  UINT8                                      Ch = 0;
  UINT8                                      Strobe;
  UINT8                                      SubCh = 0;
  DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_STRUCT    DdrdHwTrainCtl0;
  DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_STRUCT   DdrccHwTrainCtl0;
  DDRCLK_HW_TRAIN_CTL0_MCIO_DDRIOEXT_STRUCT  DdrclkHwTrainCtl0;
  UINT8                                      MSVx4 = GetMaxStrobeValid (Host);
  UINT8                                      MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Configure and Start HW FSM for Data.
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      DdrdHwTrainCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG));
      DdrdHwTrainCtl0.Bits.dfe_summer_offset_stage_en = 0;
      DdrdHwTrainCtl0.Bits.sampler_offset_stage_en = 0;
      DdrdHwTrainCtl0.Bits.xover_stage_en = 0x1;
      //
      // 16 samples
      //
      DdrdHwTrainCtl0.Bits.sample_cnt = 0;
      DdrdHwTrainCtl0.Bits.sample_thresh = 0x8;
      DdrdHwTrainCtl0.Bits.settling_time_x2 = 0x0;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG), DdrdHwTrainCtl0.Data);
    }
    //
    // Configure and Start HW FSM for CMD/CTL.
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DdrccHwTrainCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG);
      DdrccHwTrainCtl0.Bits.dfe_summer_offset_stage_en = 0;
      DdrccHwTrainCtl0.Bits.xover_stage_en = 0x1;
      //
      // 16 samples
      //
      DdrccHwTrainCtl0.Bits.sample_cnt = 0;
      DdrccHwTrainCtl0.Bits.sample_thresh = 0x8;
      DdrccHwTrainCtl0.Bits.settling_time_x2 = 0x0;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG, DdrccHwTrainCtl0.Data);
    } //subch loop
    //
    // Configure and Start HW FSM for CLK.
    //
    DdrclkHwTrainCtl0.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG);
    DdrclkHwTrainCtl0.Bits.dfe_summer_offset_stage_en = 0;
    DdrclkHwTrainCtl0.Bits.xover_stage_en = 0x1;
    //
    // 16 samples
    //
    DdrclkHwTrainCtl0.Bits.sample_cnt = 0;
    DdrclkHwTrainCtl0.Bits.sample_thresh = 0x8;
    DdrclkHwTrainCtl0.Bits.settling_time_x2 = 0;
    MemWritePciCfgEp (Socket, Ch, DDRCLK_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG, DdrclkHwTrainCtl0.Data);
  } // Ch loop

  TrainingDelay (Host, 10, 1);
  //
  // Start the HW FSM
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    //
    // Configure and Start HW FSM for Data.
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      DdrdHwTrainCtl0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG));
      DdrdHwTrainCtl0.Bits.hw_cal_en = 0x1;
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG), DdrdHwTrainCtl0.Data);
    }
    //
    // Configure and Start HW FSM for CMD/CTL.
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DdrccHwTrainCtl0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG);
      DdrccHwTrainCtl0.Bits.hw_cal_en = 0xf;
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG, DdrccHwTrainCtl0.Data);
    } //subch loop
    //
    // Configure and Start HW FSM for CLK.
    //
    DdrclkHwTrainCtl0.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG);
    DdrclkHwTrainCtl0.Bits.hw_cal_en = 0x3;
    MemWritePciCfgEp (Socket, Ch, DDRCLK_HW_TRAIN_CTL0_MCIO_DDRIOEXT_REG, DdrclkHwTrainCtl0.Data);
  } // Ch loop
  return EFI_SUCCESS;
} // ConfigureStartHwFsm

  /**

  Get the Max polling time.

  @param Host   - Pointer to sysHost, the system Host (root) structure struct
  @param Socket - Current CPU socket

  @retval UINT64 : The max polling time.

  **/
UINT64
GetMaxPollingTime (
  IN  PSYSHOST Host,
  IN  UINT8    Socket
  )
{
  //
  // Xover time out
  // 3200: 2,400ns
  // 4800: 1,600ns
  //
  UINT64 MaxEstimatedFsmTime;

  if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_3200) {
    //
    // MAX_ESTIMATED_FSM_TIME_3200/10 is guardband.
    //
    MaxEstimatedFsmTime = MAX_ESTIMATED_FSM_TIME_3200 + MAX_ESTIMATED_FSM_TIME_3200/10;
  } else if (Host->nvram.mem.socket[Socket].ddrFreq <= DDR_4800) {
    //
    // MAX_ESTIMATED_FSM_TIME_4800/10 is guardband.
    //
    MaxEstimatedFsmTime = MAX_ESTIMATED_FSM_TIME_4800 + MAX_ESTIMATED_FSM_TIME_4800/10;
  } else {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_223);
  }
  return MaxEstimatedFsmTime;

} // GetMaxPollingTime

  /**

  Check Xover Fsm Error Status.

  @param[in]  Host   - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket - Current CPU socket

  @retval EFI_SUCCESS       Status was successful
  @retval EFI_DEVICE_ERROR  An error was reported by HW

  **/
EFI_STATUS
CheckXoverFsmErrorStatus (
  IN  PSYSHOST Host,
  IN  UINT8    Socket
  )
{
  UINT8                                     Ch = 0;
  UINT8                                     Strobe;
  DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_STRUCT   DdrdHwTrainCtl0[MAX_STROBE];
  DDRCC_HWCAL_STS0_0_MCIO_DDRIOEXT_STRUCT   DdrccHwcalSts00;
  DDRCC_HWCAL_STS0_1_MCIO_DDRIOEXT_STRUCT   DdrccHwcalSts01;
  DDRCC_HWCAL_STS0_2_MCIO_DDRIOEXT_STRUCT   DdrccHwcalSts02;
  DDRCC_HWCAL_STS0_3_MCIO_DDRIOEXT_STRUCT   DdrccHwcalSts03;
  DDRCLK_HWCAL_STS0_0_MCIO_DDRIOEXT_STRUCT  DdrclkHwcalSts00;
  DDRCLK_HWCAL_STS0_1_MCIO_DDRIOEXT_STRUCT  DdrclkHwcalSts01;
  UINT8                                     MSVx4;
  DDRIO_CHANNEL_NVDATA                      (*DdrioChannelNvList)[MAX_CH];
  UINT8                                     MaxChDdr;
  UINT8                                     SubCh;
  BOOLEAN                                   ErrorsOccurred;
  BOOLEAN                                   DataErrorsOccurred;
  UINT32                                    ReadWrite;

  MSVx4 = GetMaxStrobeValid (Host);
  MaxChDdr = GetMaxChDdr ();
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  ErrorsOccurred = FALSE;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DataErrorsOccurred = FALSE;

    //
    // Read the HW FSM Error Status for Data.
    //
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      DdrdHwTrainCtl0[Strobe].Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG));
      if (DdrdHwTrainCtl0[Strobe].Bits.xover_err != 0) {
        DataErrorsOccurred = TRUE;
        //
        // Write it back to clear the xover_err
        //
        MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_HW_TRAIN_CTL0_MCIO_DDRIO_REG), DdrdHwTrainCtl0[Strobe].Data);
      }
    }

    if (DataErrorsOccurred) {
      ErrorsOccurred = TRUE;
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "HW FSM Error Status for DATA\n");
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        RcDebugPrint (SDBG_MAX, "%-2d ", Strobe);
      }
      RcDebugPrint (SDBG_MAX, "\n");
      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        RcDebugPrint (SDBG_MAX, "%-2d ", DdrdHwTrainCtl0[Strobe].Bits.xover_err);
      }
      RcDebugPrint (SDBG_MAX, "\n");
    }

    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      //
      // Read the HW FSM Error Status 0 for for CMD/CTL.
      //
      DdrccHwcalSts00.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HWCAL_STS0_0_MCIO_DDRIOEXT_REG);
      if (DdrccHwcalSts00.Bits.xover_err != 0) {
        ErrorsOccurred = TRUE;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "HW FSM Error Status 0 for CMD/CTL  %d\n", DdrccHwcalSts00.Bits.xover_err);
        //
        // Write it back to clear the xover_err
        //
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HWCAL_STS0_0_MCIO_DDRIOEXT_REG, DdrccHwcalSts00.Data);
      }
      //
      // Read the HW FSM Error Status 1 for for CMD/CTL.
      //
      DdrccHwcalSts01.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HWCAL_STS0_1_MCIO_DDRIOEXT_REG);
      if (DdrccHwcalSts01.Bits.xover_err != 0) {
        ErrorsOccurred = TRUE;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "HW FSM Error Status 1 for CMD/CTL  %d\n", DdrccHwcalSts01.Bits.xover_err);
        //
        // Write it back to clear the xover_err
        //
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HWCAL_STS0_1_MCIO_DDRIOEXT_REG, DdrccHwcalSts01.Data);
      }
      //
      // Read the HW FSM Error Status 2 for for CMD/CTL.
      //
      DdrccHwcalSts02.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HWCAL_STS0_2_MCIO_DDRIOEXT_REG);
      if (DdrccHwcalSts02.Bits.xover_err != 0) {
        ErrorsOccurred = TRUE;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "HW FSM Error Status 2 for CMD/CTL  %d\n", DdrccHwcalSts02.Bits.xover_err);
        //
        // Write it back to clear the xover_err
        //
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HWCAL_STS0_2_MCIO_DDRIOEXT_REG, DdrccHwcalSts02.Data);
      }
      //
      // Read the HW FSM Error Status 3 for for CMD/CTL.
      //
      DdrccHwcalSts03.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HWCAL_STS0_3_MCIO_DDRIOEXT_REG);
      if (DdrccHwcalSts03.Bits.xover_err != 0) {
        ErrorsOccurred = TRUE;
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "HW FSM Error Status 3 for CMD/CTL  %d\n", DdrccHwcalSts03.Bits.xover_err);
        //
        // Write it back to clear the xover_err
        //
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_HWCAL_STS0_3_MCIO_DDRIOEXT_REG, DdrccHwcalSts03.Data);
      }
    }
    //
    // Read the HW FSM Error Status 0 for for CLK.
    //
    DdrclkHwcalSts00.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_HWCAL_STS0_0_MCIO_DDRIOEXT_REG);
    if (DdrclkHwcalSts00.Bits.xover_err != 0) {
      ErrorsOccurred = TRUE;
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "HW FSM Error Status 0 for CLK          %d\n", DdrclkHwcalSts00.Bits.xover_err);
      //
      // Write it back to clear the xover_err
      //
      MemWritePciCfgEp (Socket, Ch, DDRCLK_HWCAL_STS0_0_MCIO_DDRIOEXT_REG, DdrclkHwcalSts00.Data);
    }

    //
    // Read the HW FSM Error Status 1 for for CLK.
    //
    DdrclkHwcalSts01.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_HWCAL_STS0_1_MCIO_DDRIOEXT_REG);
    if (DdrclkHwcalSts01.Bits.xover_err != 0) {
      ErrorsOccurred = TRUE;
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "HW FSM Error Status 1 for CLK          %d\n", DdrclkHwcalSts01.Bits.xover_err);
      //
      // Write it back to clear the xover_err
      //
      MemWritePciCfgEp (Socket, Ch, DDRCLK_HWCAL_STS0_1_MCIO_DDRIOEXT_REG, DdrclkHwcalSts01.Data);
    }


    //
    // Read and cache results
    //
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      ReadWrite = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG);
      (*DdrioChannelNvList)[Ch].SubChannel[SubCh].DdrccPirefOffset00 = ReadWrite;

      ReadWrite = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG);
      (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset01 = ReadWrite;
      ReadWrite = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG);
      (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset02 = ReadWrite;
      ReadWrite = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG);
      (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset03 = ReadWrite;
      if (FeaturePcdGet (PcdCteBuild) == TRUE) {
        //
        // Write back registers for CTE checker
        //
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset00);
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset01);
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset02);
        WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset03);
      }
    } // SubCh

    ReadWrite = MemReadPciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG);
    (*DdrioChannelNvList) [Ch].DdrclkPirefOffset00 = ReadWrite;

    ReadWrite = MemReadPciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG);
    (*DdrioChannelNvList) [Ch].DdrclkPirefOffset01 = ReadWrite;

    if (FeaturePcdGet (PcdCteBuild) == TRUE) {
      //
      // Write back registers for CTE checker
      //
      MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].DdrclkPirefOffset00);
      MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].DdrclkPirefOffset01);
    }
    for (Strobe = 0; Strobe < MSVx4; Strobe++) {

      ReadWrite = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG));
      (*DdrioChannelNvList) [Ch].DdrdN0PirefOffset0[Strobe] = ReadWrite;
      if (FeaturePcdGet (PcdCteBuild) == TRUE) {
        //
        // Write back registers for CTE checker
        //
        MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG), ReadWrite);
      }
    } // Strobe loop
  } // Ch loop
  //
  // Report any errors to the caller.
  //
  if (ErrorsOccurred) {
    RcDebugPrintWithDevice (
      SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Xover HW FSM error(s) occurred\n"
      );
    return EFI_DEVICE_ERROR;
  } else {
    return EFI_SUCCESS;
  }
} // CheckXoverFsmErrorStatus

/**

  Initialize registers used during crossover calibration.

  @param Host                   - Pointer to sysHost, the system Host (root) structure struct
  @param Ch                     - Current channel
  @param COUNT1_STRUCT          - Structure containing Count1 variables
  @param OFFSET_STRUCT          - Structure containing offset variables
  @param PREVIOUS_STATUS_STRUCT - Structure containing previous status variables

  @retval EFI_SUCCESS

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
  ZeroMem (Count1, sizeof (*Count1));
  ZeroMem (OffsetSt, sizeof (*OffsetSt));
  ZeroMem (PreviousStatus, sizeof (*PreviousStatus));
  ZeroMem (Results, sizeof (*Results));

  return EFI_SUCCESS;
} // InitializeLocalsPerCh

/**

  Set piDelay value to test crossover calibration.

  @param Host       - Pointer to sysHost, the system Host (root) structure struct
  @param Socket     - Current CPU socket
  @param Ch         - Current channel
  @param PiDelay    - PiDelay

  @retval EFI_SUCCESS

**/
EFI_STATUS
SetPiRefDelay (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT16   PiDelay
  )
{
  UINT8                                           Strobe;
  UINT8                                           SubCh = 0;
  DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_STRUCT         DdrdN0PirefOffset0;
  DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT      DdrccPirefOffset00;
  DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT      DdrccPirefOffset01;
  DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_STRUCT      DdrccPirefOffset02;
  DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_STRUCT      DdrccPirefOffset03;
  DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT     DdrclkPirefOffset00;
  DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT     DdrclkPirefOffset01;

  UINT8                                           MSVx4 = GetMaxStrobeValid (Host);
  DDRIO_CHANNEL_NVDATA                            (*DdrioChannelNvList) [MAX_CH];

  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    DdrccPirefOffset00.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset00.Bits.piref_train_pio_code = PiDelay;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, DdrccPirefOffset00.Data);

    DdrccPirefOffset01.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset01.Bits.piref_train_pio_code = PiDelay;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, DdrccPirefOffset01.Data);

    DdrccPirefOffset02.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset02.Bits.piref_train_pio_code = PiDelay;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG, DdrccPirefOffset02.Data);

    DdrccPirefOffset03.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset03.Bits.piref_train_pio_code = PiDelay;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG, DdrccPirefOffset03.Data);
  }
  //
  // Program CLK PI.
  //
  DdrclkPirefOffset00.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG);
  DdrclkPirefOffset00.Bits.piref_train_pio_code = PiDelay;
  MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, DdrclkPirefOffset00.Data);

  DdrclkPirefOffset01.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG);
  DdrclkPirefOffset01.Bits.piref_train_pio_code = PiDelay;
  MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, DdrclkPirefOffset01.Data);

  for (Strobe = 0;Strobe < MSVx4; Strobe++) {
    DdrdN0PirefOffset0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG));
    DdrdN0PirefOffset0.Bits.piref_train_pi_code = PiDelay;
    MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG), DdrdN0PirefOffset0.Data);
  } // Strobe loop
  return EFI_SUCCESS;
} // SetPiRefDelay

/**

  Get multiple samples based on the current piDelay value used to test crossover calibration.

  @param Host       - Pointer to sysHost, the system Host (root) structure struct
  @param Socket     - Current CPU socket
  @param Ch         - Current channel
  @param PiDelay    - PiDelay
  @param Results    - Pointer to the struct containing the results of crossover calibration

  @retval NA

**/
VOID
GetCrossoverCalibrationResults (
  IN  PSYSHOST             Host,
  IN  UINT8                Socket,
  IN  UINT8                Ch,
  IN  UINT16               PiDelay,
  OUT RESULTS_STRUCT       *Results
  )
{
  UINT8                                           Strobe;
  UINT8                                           SampleCounter;
  DDRCLK_TRAIN_RESULT0_MCIO_DDRIOEXT_STRUCT       DdrclkTrainResult0;
  DDRCC_TRAIN_RESULT0_MCIO_DDRIOEXT_STRUCT        DdrccTrainResult0;
  DDRD_N0_TRAIN_RESULT0_MCIO_DDRIO_STRUCT         DdrdN0TrainResult0;
  SAMPLE_COUNT_STRUCT                             SampleCnt;
  RESULTS_STRUCT                                  *ResultsLocal;
  UINT8                                           SubCh = 0;
  UINT8                                           Index = 0;
  UINT8                                           MSVx4 = GetMaxStrobeValid (Host);


  ResultsLocal = (RESULTS_STRUCT *) Results;
  ZeroMem (&SampleCnt, sizeof (SampleCnt));

  //
  // XOVER_CALIB_SAMPLE_COUNT: can be tuned.
  //
  for (SampleCounter = 0;SampleCounter < XOVER_CALIB_SAMPLE_COUNT; SampleCounter++) {
    CteDelayQclk (64);
    //
    // Confirmed with design and EV that the CSR access intenral delay is enough here in real silicon, no need
    // to add additional delay.
    // Read feedback register (each time the register is read,
    // it represents a new software sample, there should be enough time between reads so that we are not reading the register many times
    // before it gets updat-ed with a new physical sample)
    //
    DdrclkTrainResult0.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_TRAIN_RESULT0_MCIO_DDRIOEXT_REG);


    RcSimCmdXoverFeedbackTarget (
      0,
      PiDelay,
      &DdrclkTrainResult0,
      &DdrccTrainResult0
      );

    if (((DdrclkTrainResult0.Bits.xover_cal_pd_result) & BIT0) == 0) {
      SampleCnt.Clk[Ch][0]++;
    }
    if (((DdrclkTrainResult0.Bits.xover_cal_pd_result) & BIT1) == 0) {
      SampleCnt.Clk[Ch][1]++;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      DdrccTrainResult0.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_TRAIN_RESULT0_MCIO_DDRIOEXT_REG);

      RcSimCmdXoverFeedbackTarget (
        SubCh,
        PiDelay,
        &DdrclkTrainResult0,
        &DdrccTrainResult0
        );
      //
      // BIT0 : ddrcc_piref_offset0
      // BIT1 : ddrcc_piref_offset1
      // BIT2 : ddrcc_piref_offset2
      // BIT3 : ddrcc_piref_offset3
      //
      if (((DdrccTrainResult0.Bits.xover_cal_pd_result) & BIT0) == 0) {
        SampleCnt.CmdCtl[Ch][SubCh][0]++;
      }
      if (((DdrccTrainResult0.Bits.xover_cal_pd_result) & BIT1) == 0) {
        SampleCnt.CmdCtl[Ch][SubCh][1]++;
      }
      if (((DdrccTrainResult0.Bits.xover_cal_pd_result) & BIT2) == 0) {
        SampleCnt.CmdCtl[Ch][SubCh][2]++;
      }
      if (((DdrccTrainResult0.Bits.xover_cal_pd_result) & BIT3) == 0) {
        SampleCnt.CmdCtl[Ch][SubCh][3]++;
      }
    } //subch loop

    for (Strobe = 0;Strobe < MSVx4; Strobe++) {
      DdrdN0TrainResult0.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_TRAIN_RESULT0_MCIO_DDRIO_REG));

      RcSimDataXoverFeedbackTarget (PiDelay, Strobe, &DdrdN0TrainResult0.Data);

      if (DdrdN0TrainResult0.Bits.xover_cal_pd_result == 0) {
        SampleCnt.Data[Ch][Strobe]++;
      }
    } // Strobe loop
  }
  //
  // Check to see if each counter is at least half of the total sample count
  //
  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    for (Index = 0; Index < CMD_CTL_FUB_NUM; Index++) {
      if ((SampleCnt.CmdCtl[Ch][SubCh][Index] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
        ResultsLocal->CmdCtl[Ch][SubCh][Index] = 0;
      } else {
        ResultsLocal->CmdCtl[Ch][SubCh][Index] = 1;
      }
    }
  } //subch loop

  if ((SampleCnt.Clk[Ch][0] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
    ResultsLocal->Clk[Ch][0] = 0;
  } else {
    ResultsLocal->Clk[Ch][0] = 1;
  }
  if ((SampleCnt.Clk[Ch][1] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
    ResultsLocal->Clk[Ch][1] = 0;
  } else {
    ResultsLocal->Clk[Ch][1] = 1;
  }

  for (Strobe = 0;Strobe < MSVx4;Strobe++) {
    if ((SampleCnt.Data[Ch][Strobe] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
      ResultsLocal->Data[Ch][Strobe] = 0;
    } else {
      ResultsLocal->Data[Ch][Strobe] = 1;
    }
  }
  return;
} // GetCrossoverCalibrationResults

/**

  Display the results from crossover calibration for this PiDelay.

  @param Host           - Pointer to sysHost, the system Host (root) structure struct
  @param Ch             - Current channel
  @param PiDelay        - PiDelay
  @param Results        - Pointer to the struct containing the results of crossover calibration
  @param Offset         - Pointer to the struct containing the offsets for crossover calibration
  @param Count1         - Pointer to the struct containing the Count of positive results
  @param PreviousStatus - Pointer to the struct containing the Count of positive results

  @retval EFI_SUCCESS

**/
EFI_STATUS
DisplayCrossoverCalibrationResults (
  IN  PSYSHOST                 Host,
  IN  UINT8                    Ch,
  IN  UINT16                   PiDelay,
  IN  RESULTS_STRUCT           *Results,
  IN  OFFSET_STRUCT            *OffsetSt,
  OUT COUNT1_STRUCT            *Count1,
  OUT PREVIOUS_STATUS_STRUCT   *PreviousStatus
  )
{
  UINT8                                           Strobe;
  UINT8                                           SubCh = 0;
  UINT8                                           Index = 0;
  UINT8                                           MSVx4 = GetMaxStrobeValid (Host);
  RESULTS_STRUCT*                                 ResultsLocal;
  CHAR8                                           Str00[] = "  0";
  CHAR8                                           Str01[] = "  1";

  ResultsLocal = (RESULTS_STRUCT* ) Results;

  for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
    //
    // CMD/CTRL
    //
    for (Index = 0; Index < CMD_CTL_FUB_NUM; Index ++) {
      if (ResultsLocal->CmdCtl[Ch][SubCh][Index] == 1) {
        Count1->CmdCtl[Ch][SubCh][Index]++;
        RcDebugPrint (SDBG_MAX, Str01);
      } else {
        RcDebugPrint (SDBG_MAX, Str00);
      }

      if ((PreviousStatus->CmdCtl[Ch][SubCh][Index] == 0) && (ResultsLocal->CmdCtl[Ch][SubCh][Index] == 1)) {
        if (PiDelay > OffsetSt->CmdCtl[Ch][SubCh][Index]) {
          OffsetSt->CmdCtl[Ch][SubCh][Index] = PiDelay;
        }
      }

      PreviousStatus->CmdCtl[Ch][SubCh][Index] = ResultsLocal->CmdCtl[Ch][SubCh][Index];
    }

    RcDebugPrint (SDBG_MAX,"       ");
  } //subch loop
  //
  // CLK
  //
  for (Index = 0; Index < CLK_FUB_NUM; Index ++) {
    if (ResultsLocal->Clk[Ch][Index] == 1) {
      Count1->Clk[Ch][Index]++;
      RcDebugPrint (SDBG_MAX, Str01);
    } else {
      RcDebugPrint (SDBG_MAX, Str00);
    }
    if ((PreviousStatus->Clk[Ch][Index] == 0) && (ResultsLocal->Clk[Ch][Index] == 1)) {
      if (PiDelay > OffsetSt->Clk[Ch][Index]) {
        OffsetSt->Clk[Ch][Index] = PiDelay;
      }
    }
    PreviousStatus->Clk[Ch][Index] = ResultsLocal->Clk[Ch][Index];
  }

  RcDebugPrint (SDBG_MAX,"    ");

  for (Strobe = 0;Strobe < MSVx4;Strobe++) {

    if (ResultsLocal->Data[Ch][Strobe]) {
      Count1->Strobe[Ch][Strobe]++;
      RcDebugPrint (SDBG_MAX, Str01);
    } else {
      RcDebugPrint (SDBG_MAX, Str00);

    }
    //
    // Check if previous was 0 and current is 1
    //
    if ((PreviousStatus->Data[Ch][Strobe] == 0) && (ResultsLocal->Data[Ch][Strobe] == 1)) {
      if (PiDelay > OffsetSt->Offset[Ch][Strobe]) {
        OffsetSt->Offset[Ch][Strobe] = PiDelay;
      }
    }
    PreviousStatus->Data[Ch][Strobe] = ResultsLocal->Data[Ch][Strobe];
  }

  RcDebugPrint (SDBG_MAX, "\n");

  return EFI_SUCCESS;

} // DisplayCrossoverCalibrationResults

/**

  Program the offsets found during crossover calibration.

  @param Host       - Pointer to sysHost, the system Host (root) structure struct
  @param Socket     - Current CPU socket
  @param Ch         - Current channel
  @param OffsetSt   - Pointer to the struct containing the offsets found during crossover calibration

  @retval EFI_SUCCESS

**/
EFI_STATUS
ProgramOffsets (
  IN  PSYSHOST             Host,
  IN  UINT8                Socket,
  IN  UINT8                Ch,
  IN  OFFSET_STRUCT        *OffsetSt
  )
{
  UINT8                                           Index = 0;
  UINT8                                           Strobe;
  UINT8                                           MSVx4;
  UINT16                                          PiDelay;
  UINT8                                           SubCh = 0;
  DDRIO_CHANNEL_NVDATA                            (*DdrioChannelNvList) [MAX_CH];
  DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT      DdrccPirefOffset00;
  DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT      DdrccPirefOffset01;
  DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_STRUCT      DdrccPirefOffset02;
  DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_STRUCT      DdrccPirefOffset03;
  DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_STRUCT     DdrclkPirefOffset00;
  DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_STRUCT     DdrclkPirefOffset01;
  DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_STRUCT         DataOffsetTrain;
  CHAR8                                           XoverStr[] = "        CMDCTL0[0:3]    CMDCTL1[0:3]    CLK[0:1]  ";
  SYS_SETUP                                       *Setup;

  Setup  = GetSysSetupPointer ();
  MSVx4 = GetMaxStrobeValid (Host);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  //
  // Program offsets
  //
  if (Setup->mem.dfxMemSetup.DfxSwXoverOption == DFX_XOVER_SW_SERIAL) {
    PrintSignalWithStrobe (Socket, Ch, MSVx4, XoverStr);
  }
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "offsets: ");

  for (SubCh = 0 ; SubCh < SUB_CH; SubCh++) {
    if (SubCh == 1) {
      RcDebugPrint (SDBG_MAX,
        "     ");
    }
    for (Index = 0; Index < 4; Index ++) {
      RcDebugPrint (SDBG_MAX,
        "%2d ", OffsetSt->CmdCtl[Ch][SubCh][Index]);
    }
  }

  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    DdrccPirefOffset00.Data = ReadDdrioCcRegister (Host, Socket, Ch,  SubCh, DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset00.Bits.cmd_piref_offset = (OffsetSt->CmdCtl[Ch][SubCh][0]) % X_OVER_MAX_SAMPLE;
    WriteDdrioCcRegister (Host, Socket, Ch,  SubCh, DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, DdrccPirefOffset00.Data);
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset00 = DdrccPirefOffset00.Data;

    DdrccPirefOffset01.Data = ReadDdrioCcRegister (Host, Socket, Ch,  SubCh, DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset01.Bits.cmd_piref_offset = (OffsetSt->CmdCtl[Ch][SubCh][1]) % X_OVER_MAX_SAMPLE;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, DdrccPirefOffset01.Data);
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset01 = DdrccPirefOffset01.Data;

    DdrccPirefOffset02.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset02.Bits.cmd_piref_offset = (OffsetSt->CmdCtl[Ch][SubCh][2]) % X_OVER_MAX_SAMPLE;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG, DdrccPirefOffset02.Data);
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset02 = DdrccPirefOffset02.Data;

    DdrccPirefOffset03.Data = ReadDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG);
    DdrccPirefOffset03.Bits.cmd_piref_offset = (OffsetSt->CmdCtl[Ch][SubCh][3]) % X_OVER_MAX_SAMPLE;
    WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG, DdrccPirefOffset03.Data);
    (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset03 = DdrccPirefOffset03.Data;
  }
  RcDebugPrint (SDBG_MAX, "     ");
  RcDebugPrint (SDBG_MAX,
    "%2d ", OffsetSt->Clk[Ch][0]);
  RcDebugPrint (SDBG_MAX,
    "%2d", OffsetSt->Clk[Ch][1]);

  DdrclkPirefOffset00.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG);
  DdrclkPirefOffset00.Bits.clk_piref_offset = (OffsetSt->Clk[Ch][0]) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, DdrclkPirefOffset00.Data);
  (*DdrioChannelNvList) [Ch].DdrclkPirefOffset00 = DdrclkPirefOffset00.Data;

  DdrclkPirefOffset01.Data = MemReadPciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG);
  DdrclkPirefOffset01.Bits.clk_piref_offset = (OffsetSt->Clk[Ch][1]) % X_OVER_MAX_SAMPLE;
  MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, DdrclkPirefOffset01.Data);
  (*DdrioChannelNvList) [Ch].DdrclkPirefOffset01 = DdrclkPirefOffset01.Data;
  RcDebugPrint (SDBG_MAX,
    "   ");

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {

    PiDelay = OffsetSt->Offset[Ch][Strobe];
    RcDebugPrint (SDBG_MAX,
      "%2d ", PiDelay);
    DataOffsetTrain.Data = MemReadPciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG));
    DataOffsetTrain.Bits.rcven_piref_offset = (PiDelay) % X_OVER_MAX_SAMPLE;
    DataOffsetTrain.Bits.txdq_piref_offset = (PiDelay) % X_OVER_MAX_SAMPLE;
    DataOffsetTrain.Bits.txdqs_piref_offset = (PiDelay) % X_OVER_MAX_SAMPLE;
    MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG), DataOffsetTrain.Data);
    (*DdrioChannelNvList) [Ch].DdrdN0PirefOffset0[Strobe] = DataOffsetTrain.Data;

  } // Strobe loop

  RcDebugPrint (SDBG_MAX, "\n");

  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Additional Xover PiDelay offset for SPR: CMD = %d, CMD PIREF0 = %d, CLK = %d, DATA = %d\n",
    0,  0,  0,  0);
  return EFI_SUCCESS;
} //ProgramOffsets

/**

  Display Xover Calibration Count Results.

  @param Count1  - Pointer to COUNT1_STRUCT

  @retval Status

**/
VOID
DisplayXoverCountResults (
  IN  UINT8                                       Socket,
  IN  UINT8                                       Ch,
  IN  COUNT1_STRUCT                               *Count1
  )
{
  SYS_SETUP                                       *Setup;

  Setup  = GetSysSetupPointer ();
  if (Setup->mem.dfxMemSetup.DfxSwXoverOption == DFX_XOVER_SW_SERIAL) {
    RcDebugPrint (SDBG_MAX, "\n                CMDCTL0[0] [1] [2] [3]  CMDCTL1[0] [1] [2] [3] CLK[0] [1]    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19\n");
    RcDebugPrint (SDBG_MAX, "COUNT NON-ZERO:       ");
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "COUNT NON-ZERO:");
  }

  RcDebugPrint (SDBG_MAX, " %3d", Count1->CmdCtl[Ch][0][0]);
  RcDebugPrint (SDBG_MAX, " %3d", Count1->CmdCtl[Ch][0][1]);
  RcDebugPrint (SDBG_MAX, " %3d", Count1->CmdCtl[Ch][0][2]);
  RcDebugPrint (SDBG_MAX, " %3d", Count1->CmdCtl[Ch][0][3]);

  RcDebugPrint (SDBG_MAX, "         %3d", Count1->CmdCtl[Ch][1][0]);
  RcDebugPrint (SDBG_MAX, " %3d", Count1->CmdCtl[Ch][1][1]);
  RcDebugPrint (SDBG_MAX, " %3d", Count1->CmdCtl[Ch][1][2]);
  RcDebugPrint (SDBG_MAX, " %3d", Count1->CmdCtl[Ch][1][3]);

  RcDebugPrint (SDBG_MAX, "   ");
  RcDebugPrint (SDBG_MAX, "%3d", Count1->Clk[Ch][0]);
  RcDebugPrint (SDBG_MAX, " ");
  RcDebugPrint (SDBG_MAX, "%3d", Count1->Clk[Ch][1]);
  RcDebugPrint (SDBG_MAX, "    ");

  return;
} // DisplayXoverCountResults

/**

  Restore Xover CSRs for warm boot or fast cold boot.

  @param Host  - Pointer to sysHost

  @retval EFI_SUCCESS

**/
EFI_STATUS
RestoreXover (
  PSYSHOST  Host
  )
{
  UINT8                           Socket;
  UINT8                           Ch;
  UINT8                           SubCh;
  UINT8                           Strobe;
  UINT8                           MSVx4;
  UINT8                           MaxChDdr;
  DDRIO_CHANNEL_NVDATA            (*DdrioChannelNvList) [MAX_CH];

  MaxChDdr = GetMaxChDdr ();
  Socket = GetCurrentSocketId ();
  MSVx4 = GetMaxStrobeValid (Host);
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset00);
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset01);
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_2_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset02);
      WriteDdrioCcRegister (Host, Socket, Ch, SubCh, DDRCC_PIREF_OFFSET0_3_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].SubChannel[SubCh].DdrccPirefOffset03);
    } // SubCh

    for (Strobe = 0; Strobe < MSVx4; Strobe++) {
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET0_MCIO_DDRIO_REG), (*DdrioChannelNvList) [Ch].DdrdN0PirefOffset0[Strobe]);
      MemWritePciCfgEp (Socket, Ch, UpdateIoRegisterCh10nm (Host, Ch, Strobe, DDRD_N0_PIREF_OFFSET1_MCIO_DDRIO_REG), (*DdrioChannelNvList) [Ch].DdrdN0PirefOffset1[Strobe]);
    } // Strobe

    MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_0_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].DdrclkPirefOffset00);
    MemWritePciCfgEp (Socket, Ch, DDRCLK_PIREF_OFFSET0_1_MCIO_DDRIOEXT_REG, (*DdrioChannelNvList) [Ch].DdrclkPirefOffset01);

  } // Ch loop

  return EFI_SUCCESS;
} // RestoreXover

  /**

  Peform crossover calibration by BIOS for normal boot or cold boot.

  @param Host  - Pointer to sysHost

  @retval Status

  **/
EFI_STATUS
XoverCalibration (
  PSYSHOST  Host
  )
{
  UINT8                                           Ch;
  UINT8                                           Socket;
  UINT8                                           Strobe;
  UINT16                                          PiDelay;
  UINT8                                           MSVx4 = GetMaxStrobeValid (Host);
  DDRIO_CHANNEL_NVDATA                            (*DdrioChannelNvList) [MAX_CH];
  COUNT1_STRUCT                                   Count1;
  OFFSET_STRUCT                                   OffsetSt;
  PREVIOUS_STATUS_STRUCT                          PreviousStatus;
  RESULTS_STRUCT                                  Results;
  CHAR8                                           XoverStr[] = "                CMDCTL0[0:3]        CMDCTL1[0:3]       CLK[0:1]  ";
  UINT8                                           MaxChDdr;
  SYS_SETUP                                       *Setup;

  Setup  = GetSysSetupPointer ();
  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;
  DdrioChannelNvList = GetDdrioChannelNvList (Host, Socket);

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Enable xovercal\n");
  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_CROSSOVER_CALIBRATION, Socket);

  //
  //Step 1: Select which Xover phase detect output will be used for calibration.
  //
  SelectXoverPhaseDetectOutput (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    //
    // Step 2: Program the PIREF setup adjust to 0 during training.
    //
    ProgramPirefSetupAdj (Host, Socket, Ch, 0);

    //
    // Step 3: Enable the Xover Calibration mode. Check whether we still need to do it serially in SPR. From the design, it should be OK to do it in parallel.
    // but also see the post silicon result.
    //
    EnableDisableXoverCalRegs (Host, Socket, Ch, XOVER_CAL_ENABLE);
    InitializeLocalsPerCh (Host, Ch, &Count1, &OffsetSt, &PreviousStatus, &Results);
  }

  if (Setup->mem.dfxMemSetup.DfxSwXoverOption == DFX_XOVER_SW_SERIAL) {
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (!IsChannelEnabled (Socket, Ch)) {
        continue;
      }
      PrintSignalWithStrobe (Socket, Ch, MSVx4, XoverStr);
      //
      // Step 4. Training starts.
      //
      for (PiDelay = X_OVER_PI_START; PiDelay < X_OVER_MAX_SAMPLE; PiDelay += X_OVER_STEP_SIZE) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          " %03d    ", PiDelay);
        //
        // Program PIREF
        //
        SetPiRefDelay (Host, Socket, Ch, PiDelay);
        //
        // Read the Calibration Results
        //
        GetCrossoverCalibrationResults (Host, Socket, Ch, PiDelay, &Results);
        DisplayCrossoverCalibrationResults (Host, Ch, PiDelay, &Results, &OffsetSt, &Count1, &PreviousStatus);
      } // PiDelay loop

      DisplayXoverCountResults (Socket, Ch, &Count1);

      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        RcDebugPrint (SDBG_MAX,
          "%3d", Count1.Strobe[Ch][Strobe]);
      }
      RcDebugPrint (SDBG_MAX,
        "\n");

      EnableDisableXoverCalRegs (Host, Socket, Ch, XOVER_CAL_DISABLE);
      //
      // Program final results.
      //
      ProgramOffsets (Host, Socket, Ch, &OffsetSt);
    }
  } else {
    PrintSignalWithStrobe (Socket, Ch, MSVx4, XoverStr);

    //
    // Step 4. Training starts.
    //
    for (PiDelay = X_OVER_PI_START; PiDelay < X_OVER_MAX_SAMPLE; PiDelay += X_OVER_STEP_SIZE) {
      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if (!IsChannelEnabled (Socket, Ch)) {
          continue;
        }
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          " %03d    ", PiDelay);
        //
        // Program PIREF
        //
        SetPiRefDelay (Host, Socket, Ch, PiDelay);
        //
        // Read the Calibration Results
        //
        GetCrossoverCalibrationResults (Host, Socket, Ch, PiDelay, &Results);
        DisplayCrossoverCalibrationResults (Host, Ch, PiDelay, &Results, &OffsetSt, &Count1, &PreviousStatus);
      } // Ch loop
    } // PiDelay loop

    RcDebugPrint (SDBG_MAX, "\n                 CMDCTL0[0] [1] [2] [3]  CMDCTL1[0] [1] [2] [3] CLK[0] [1]    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19\n");
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (!IsChannelEnabled (Socket, Ch)) {
        continue;
      }
      DisplayXoverCountResults (Socket, Ch, &Count1);

      for (Strobe = 0; Strobe < MSVx4; Strobe++) {
        RcDebugPrint (SDBG_MAX,
          "%3d", Count1.Strobe[Ch][Strobe]);
      }
      RcDebugPrint (SDBG_MAX, "\n");
    }

    RcDebugPrint (SDBG_MAX,
      "\n");

    PrintSignalWithStrobe (Socket, Ch, MSVx4, XoverStr);
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (!IsChannelEnabled (Socket, Ch)) {
        continue;
      }

      EnableDisableXoverCalRegs (Host, Socket, Ch, XOVER_CAL_DISABLE);
      //
      // Program final results.
      //
      ProgramOffsets (Host, Socket, Ch, &OffsetSt);
    }
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    //
    // Program the PIREF setup adjust to its final values.
    // Confirmed with design and EV that the setup adjust final value may be tuned in post silicon.
    //
    ProgramPirefSetupAdj (Host, Socket, Ch, XOVER_PIREF_SETUP_ADJ);
    //
    // Program piref_train_pio_code for each xover with trained value. so that we have
    // absolute 0 delay between piref clk and piclko in xover when cmd_dly = 0.
    // Otherwise, when starts sweeping cmd_dly from 0, the piclko is
    // misaligned with piref_clk, so sweeping cmd_dly from 0 to 'hff leads to
    // outrange wrap around sweeping. data goes into xover will not be
    // delayed properly (it choose wrap-arounded nearest piclko posedge and
    // corrupts the data)
    //
    ProgramPirefCode (Host, Socket, Ch, &OffsetSt);
  } // Ch loop
  return EFI_SUCCESS;
} // XoverCalibration

/**

  Peform Crossover HW FSM Calibration during normal boot.

  @param Host  - Pointer to sysHost

  @retval EFI_SUCCESS       HW FSM was successful
  @retval EFI_TIMEOUT       Timed out while polling for completion
  @retval EFI_DEVICE_ERROR  An error was reported by HW

**/
EFI_STATUS
XoverCalibHwFsmNormalBoot (
  IN  PSYSHOST  Host
  )
{
  UINT8       Socket;
  EFI_STATUS  Status;
  UINT8       Ch = 0;
  UINT8       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Socket = Host->var.mem.currentSocket;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Run xovercal HW FSM during normal boot\n");
  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_CROSSOVER_CALIBRATION, Socket);
  //
  //Step 1: Select which Xover phase detect output will be used for calibration.
  //
  Status = SelectXoverPhaseDetectOutput (Host, Socket);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Step 2: Program the PIREF setup adjust to its final values
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    Status = ProgramPirefSetupAdj (Host, Socket, Ch, XOVER_PIREF_SETUP_ADJ);
  }
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Step 3: Configure and Start the HW FSM
  //
  Status = ConfigureStartHwFsm (Host, Socket);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Step 4: Poll the HW FSM to complete.
  //
  Status = PollHwFsmComplete (Host, Socket, CH_BITMASK, GetMaxPollingTime (Host, Socket));
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Step 5: Check for the error status.
  //
  Status = CheckXoverFsmErrorStatus (Host, Socket);
  return Status;
} // XoverCalibHwFsmNormalBoot

/**

  Determine which version of crossover calibration to call based on CRIF version

  @param Host - Pointer to sysHost, the system Host (root) structure struct

  @retval SUCCESS   Calibration was successful or skipped
  @retval FAILURE   An error occurred
**/
UINT32
CrossoverCalib10nm (
  PSYSHOST  Host
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  PT_ZONE     MemPhaseSave;

  if (!IsMemFlowEnabled (CrossoverCalibration)) {
    return SUCCESS;
  }

  GetMemPhase (&MemPhaseSave);
  SetMemPhaseCP (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_CROSSOVER_CALIBRATION);

#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif // DEBUG_CODE_BLOCK

  if ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {
    if (IsMemFlowEnabled (CrossoverHwFsmCalibration)) {
      Status = XoverCalibHwFsmNormalBoot (Host);
      if (EFI_ERROR (Status)) {
        //
        // Run SW Xover training for comparison before fatal error.
        //
        XoverCalibration (Host);
        if (Status == EFI_TIMEOUT) {
          FatalError (ERR_DDRIO_HWFSM, ERR_XOVER_HWFSM_TIMEOUT);
        } else {
          FatalError (ERR_DDRIO_HWFSM, ERR_XOVER_HWFSM_FAILURE);
        }
      }
    } else {
      Status = XoverCalibration (Host);
    }
  } else if ((Host->var.mem.subBootMode == WarmBootFast) || (Host->var.mem.subBootMode == ColdBootFast)) {
    Status = RestoreXover (Host);
  }

#ifdef DEBUG_CODE_BLOCK
  DisplayXoverResults (Host, Host->var.mem.currentSocket);
  ReleasePrintControl ();
#endif //DEBUG_CODE_BLOCK

  SetMemPhase (MemPhaseSave);

  if (EFI_ERROR (Status)) {
    return FAILURE;
  } else {
    return SUCCESS;
  }
}
