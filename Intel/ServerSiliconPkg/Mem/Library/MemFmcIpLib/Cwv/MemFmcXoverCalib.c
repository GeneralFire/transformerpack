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
#include "Include/MemFmcRegs.h"
#include "Chip/Include/FnvAccessCommon.h"
#include <Library/MemFmcIpLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CteNetLib.h>
#include <Library/CheckpointLib.h>
#include <Library/ProcMemInitChipLib.h>
#include "Include/MemFmcIpLibInternal.h"
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/MemoryCheckpointCodes.h>

#define   X_OVER_STEP_SIZE  1
#define   X_OVER_PI_START   0
#define   X_OVER_MAX_SAMPLE 64
#define   X_OVER_OFFSET     58
#define XOVER_CALIB_SAMPLE_COUNT      50

#define   MAX_CMDPI       7

#define   MAX_CLKPI       8
#define   CMDN_INDEX      0
#define   CMDS_INDEX      1
#define   CTL_INDEX       2
#define   CKE_INDEX       3
#define   MAX_CMD_FUB     4
#define   CMDPIREF0_INDEX (MAX_CMDPI - 1)


/**

Peform crossover calibration for FNV

@param Host  - Pointer to sysHost

@retval Status

**/
UINT32
CrossoverCalibFnv(
  PSYSHOST  Host
)
{
  UINT8                                 socket;
  UINT8                                 ch;
  UINT8                                 dimm;
  UINT16                                piDelay;
  UINT8                                 sampleCounter;
  UINT8                                 previousStatusCmdS[MAX_CH][MAX_CMDPI];
  UINT16                                offsetCmdS[MAX_CH][MAX_CMDPI];
  UINT8                                 cmdLoop;
  UINT8                                 cmdsCounter[MAX_CMDPI];
  UINT8                                 cmdsResult[MAX_CH][MAX_CMDPI];
  struct channelNvram                   (*channelNvList)[MAX_CH];
  struct dimmNvram                      (*dimmNvList)[MAX_DIMM];
//CWV_TODO  DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_STRUCT    ddrCRCmdControls;
//CWV_TODO  DDRCRCMDTRAINING_SA_FNV_DDRIO_DAT7_CH_STRUCT    ddrCRCmdSTraining;
//CWV_TODO  DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_STRUCT   ddrCRCmdControls3;
//CWV_TODO  DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_STRUCT   ddrCRCmdControls2;
  MSC_FNVIO_DDRTDLL_CSR_CWV_DFX_MISC_0_STRUCT     fnvIoDdrtDll;
//CWV_TODO  DDRCRCTLRANKSUSED_SA_FNV_DDRIO_DAT7_CH_STRUCT   ddrCRCtlRanksUsed;
  UINT8     i;
  UINT16    minOffset;
  UINT16    maxOffset;
  UINT16    midOffset;
  UINT8     count1cmds[MAX_CH][MAX_CMDPI];
  UINT8     MaxChDdr;
//CWV_TODO  UINT32    TempData;

  if (!IsMemFlowEnabled (CrossoverCalibration)) {
    return SUCCESS;
  }

#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif //DEBUG_CODE_BLOCK
  socket = Host->var.mem.currentSocket;

  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\nCrossoverCalibFnv Starts\n");

  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr = GetMaxChDdr ();

  if ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {

    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_CROSSOVER_CALIBRATION, socket);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].ddrtEnabled == 0) {
        continue;
      }
      dimmNvList = GetDimmNvList(Host, socket, ch);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        if (!(*dimmNvList)[dimm].DcpmmPresent) {
          continue;
        }
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Enable xovercal\n");

        fnvIoDdrtDll.Data = ReadFnvCfg(Host, socket, ch, dimm, MSC_FNVIO_DDRTDLL_CSR_CWV_DFX_MISC_0_REG);
        fnvIoDdrtDll.Bits.ddrtdllpowerdown4qnnnh_val = 0;
        WriteFnvCfg(Host, socket, ch, dimm, MSC_FNVIO_DDRTDLL_CSR_CWV_DFX_MISC_0_REG, fnvIoDdrtDll.Data);

/**
CWV_TODO
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2.Data, 0, FNV_IO_READ, 0xF);
        ddrCRCmdControls2.Bits.fnvddrcrdllpibypassen = 0;
        ddrCRCmdControls2.Bits.fnvcmdcrxoverbypassen = 0;
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2.Data, 0, FNV_IO_WRITE, 0xF);

        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls.Data, 0, FNV_IO_READ, 0xF);
        ddrCRCmdControls.Bits.txon = 0;
        ddrCRCmdControls.Bits.xovercal = 1;
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls.Data, 0, FNV_IO_WRITE, 0xF);
        IODdrtResetPerCh (Host, socket, ch, dimm);
**/
      }
    }

    RcDebugPrint (SDBG_MAX, "\nCMDS[0:5]:\n");
    RcDebugPrint (SDBG_MAX,
      "PI ");

    for (ch = 0; ch < MaxChDdr; ch++) {
      RcDebugPrint (SDBG_MAX, "       Ch%d", ch);
    } // ch loop
    RcDebugPrint (SDBG_MAX, "\n");

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].ddrtEnabled == 0) {
        continue;
      }
      for (i = 0; i < MAX_CMDPI; i++) {
        count1cmds[ch][i] = 0;
        offsetCmdS[ch][i] = 0;
        previousStatusCmdS[ch][i] = 0;
      }
    }

    for (piDelay = X_OVER_PI_START; piDelay < X_OVER_MAX_SAMPLE; piDelay += X_OVER_STEP_SIZE) {
      RcDebugPrint (SDBG_MAX,
        "%2d     ", piDelay);

      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].ddrtEnabled == 0) {
          RcDebugPrint (SDBG_MAX,
            "          ");
          continue;
        }

        dimmNvList = GetDimmNvList(Host, socket, ch);
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }

          if (!(*dimmNvList)[dimm].DcpmmPresent) {
            continue;
          }

          for (i = 0; i < MAX_CMDPI; i++) {
            cmdsCounter[i] = 0;
          }

//CWV_TODO          GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls3.Data, 0, FNV_IO_READ, 0xF);
//CWV_TODO          ddrCRCmdControls3.Bits.cmdrefpiclkdelay = piDelay;
//CWV_TODO          GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls3.Data, 0, FNV_IO_WRITE, 0xF);
          IODdrtResetPerCh (Host, socket, ch, dimm);

          // HSD4165676: Multiple sample during crossover calibration
          for (sampleCounter = 0; sampleCounter < XOVER_CALIB_SAMPLE_COUNT; sampleCounter++) {
            FixedDelayMicroSecond (1); // might want to fine tune this delay with real silicon

//CWV_TODO            GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDTRAINING_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdSTraining.Data, 0, FNV_IO_READ, 0xF);
            IODdrtResetPerCh(Host, socket, ch, dimm);


            for (cmdLoop = 0; cmdLoop < MAX_CMDPI; cmdLoop++) {
//CWV_TODO              if ((ddrCRCmdSTraining.Bits.xoverphasedet >> cmdLoop) & 1) {
//CWV_TODO                cmdsCounter[cmdLoop]++;
//CWV_TODO              }
            }
          } // sampleCounter

          for (cmdLoop = 0; cmdLoop < MAX_CMDPI; cmdLoop++) {
            if ((cmdsCounter[cmdLoop] * 10) >= ((XOVER_CALIB_SAMPLE_COUNT * 10) / 2)) {
              cmdsResult[ch][cmdLoop] = 1;
            } else {
              cmdsResult[ch][cmdLoop] = 0;
            }
          }

          for (cmdLoop = 0; cmdLoop < MAX_CMDPI; cmdLoop++) {
            if (cmdsResult[ch][cmdLoop]) {
              count1cmds[ch][cmdLoop]++;
              RcDebugPrint (SDBG_MAX,
                "1");
            } else {
              RcDebugPrint (SDBG_MAX,
                "0");
            }
            if ((previousStatusCmdS[ch][cmdLoop] != 0) & (cmdsResult[ch][cmdLoop] == 0)) {
              if (piDelay > offsetCmdS[ch][cmdLoop]) {
                offsetCmdS[ch][cmdLoop] = piDelay;
              }

            }
            previousStatusCmdS[ch][cmdLoop] = cmdsResult[ch][cmdLoop];
          }
        } // dimm
        RcDebugPrint (SDBG_MAX,
          "    ");
      } // ch
      RcDebugPrint (SDBG_MAX,
        "\n");
    } // piDelay

    RcDebugPrint (SDBG_MAX,
      "\nCOUNT NON-ZERO:\n");

    //Print Number of 1s from all phase detectors
    RcDebugPrint (SDBG_MAX,
      "  CMDS [0][1][2][3][4][5]\n");

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].ddrtEnabled == 0) {
        continue;
      }
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "");

      for (i = 0; i < MAX_CMDPI; i++) RcDebugPrint (SDBG_MAX, "%d ", count1cmds[ch][i]);
      RcDebugPrint (SDBG_MAX,
        "\n");
    }

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].ddrtEnabled == 0) {
        RcDebugPrint (SDBG_MAX,
          "      ");
        continue;
      }
      dimmNvList = GetDimmNvList(Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        if (!(*dimmNvList)[dimm].DcpmmPresent) {
          continue;
        }

/**
CWV_TODO
        //
        // Disable xover training mode
        //
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls.Data, 0, FNV_IO_READ, 0xF);
        ddrCRCmdControls.Bits.xovercal = 0;
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls.Data, 0, FNV_IO_WRITE, 0xF);
**/

        IODdrtResetPerCh (Host, socket, ch, dimm);
        //
        // Determine best CMD offsets
        //
        minOffset = 0xFF;
        maxOffset = 0;

        for (cmdLoop = 0; cmdLoop < MAX_CMDPI; cmdLoop++) {

          // Get min/max values for each CMD fub
          if (offsetCmdS[ch][cmdLoop] < minOffset) {
            minOffset = offsetCmdS[ch][cmdLoop];
          }
          if (offsetCmdS[ch][cmdLoop] > maxOffset) {
            maxOffset = offsetCmdS[ch][cmdLoop];
          }
        }
        if (maxOffset - minOffset > X_OVER_MAX_SAMPLE / 2) {
          midOffset = ((minOffset + maxOffset + X_OVER_MAX_SAMPLE) / 2) % X_OVER_MAX_SAMPLE;
        } else {
          midOffset = (minOffset + maxOffset) / 2;
        }

/**
//CWV_TODO
        //
        // Program offsets
        //
        piDelay = midOffset % X_OVER_MAX_SAMPLE;
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls3.Data, 0, FNV_IO_READ, 0xF);
        ddrCRCmdControls3.Bits.cmdrefpiclkdelay = piDelay;
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls3.Data, 0, FNV_IO_WRITE, 0xF);

        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDTRAINING_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdSTraining.Data, 0, FNV_IO_READ, 0xF);
        ddrCRCmdSTraining.Bits.trainingoffset = piDelay;
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDTRAINING_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdSTraining.Data, 0, FNV_IO_WRITE, 0xF);

        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCTLRANKSUSED_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCtlRanksUsed.Data, 0, FNV_IO_READ, 0xF);
        ddrCRCtlRanksUsed.Bits.ranken = 0x3f;
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCTLRANKSUSED_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCtlRanksUsed.Data, 0, FNV_IO_WRITE, 0xF);
**/

        IODdrtResetPerCh (Host, socket, ch, dimm);

      } // dimm loop
    } // ch loop

  } else if (Host->var.mem.subBootMode == ColdBootFast) {

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].ddrtEnabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList(Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        if (!(*dimmNvList)[dimm].DcpmmPresent) {
          continue;
        }
        fnvIoDdrtDll.Data = ReadFnvCfg(Host, socket, ch, dimm, MSC_FNVIO_DDRTDLL_CSR_CWV_DFX_MISC_0_REG);
        fnvIoDdrtDll.Bits.ddrtdllpowerdown4qnnnh_val = 0;
        WriteFnvCfg(Host, socket, ch, dimm, MSC_FNVIO_DDRTDLL_CSR_CWV_DFX_MISC_0_REG, fnvIoDdrtDll.Data);

/**
CWV_TODO
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2.Data, 0, FNV_IO_READ, 0xF);
        ddrCRCmdControls2.Bits.fnvddrcrdllpibypassen = 0;
        ddrCRCmdControls2.Bits.fnvcmdcrxoverbypassen = 0;
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2.Data, 0, FNV_IO_WRITE, 0xF);

        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls.Data, 0, FNV_IO_READ, 0xF);
        ddrCRCmdControls.Bits.txon = 0;
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls.Data, 0, FNV_IO_WRITE, 0xF);

        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDTRAINING_SA_FNV_DDRIO_DAT7_CH_REG, &TempData, 0, FNV_IO_WRITE, 0xF);

        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_REG, &TempData, 0, FNV_IO_WRITE, 0xF);

        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCTLRANKSUSED_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCtlRanksUsed.Data, 0, FNV_IO_READ, 0xF);
        ddrCRCtlRanksUsed.Bits.ranken = 0x3f;
        GetSetFnvIO(Host, socket, ch, dimm, DDRCRCTLRANKSUSED_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCtlRanksUsed.Data, 0, FNV_IO_WRITE, 0xF);
**/

        IODdrtResetPerCh (Host, socket, ch, dimm);
      }
    }
  }
#ifdef DEBUG_CODE_BLOCK
  DisplayXoverResultsFnv(Host, socket);
  ReleasePrintControl ();
#endif //DEBUG_CODE_BLOCK
  return SUCCESS;
} // CrossoverCalibFnv
