/** @file
  API for the memory Pkg Delay library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <SysHostChip.h>
#include <Library/MemoryCoreLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/MemCmdControlTablesLib.h>

#include <Library/BaseLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/BaseMemoryLib.h>

#include "Include/MemCmdControl.h"
#include <Memory/MemCmdCtlClkCommon.h>
#include "Include/MemDdrioRegs.h"
#include <UncoreCommonIncludes.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/CheckpointLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/MemRcLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/PkgDelayLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/MemRoutingLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/RcMemAllocLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>


STATIC
MRC_STATUS
SignalToPiGroupBrs (
  PSYSHOST              Host,
  GSM_CSN               sig,
  struct signalIOGroup  *signalGroup,
  UINT8                 *Channel
  );

STATIC
VOID
EvaluateCaBacksideMargins (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     ckEnabled[MAX_CLK_BS_SUBCH_DDR5],
  UINT8     ctlIndex[MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdLeft[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdRight[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  MRC_GT    group[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  GSM_CSN   cmdSignal[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdOffset[COPY_NUM]
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
  )
{
  UINT8               ai;
  INT16               baseDelay;
  UINT8               mode = GSM_FORCE_WRITE + GSM_UPDATE_CACHE; // Use the cached value and don't read
  UINT8               picoPerPi;
  INT16               delay;
  TYPE_CHOPTYPE_ENUM  CpuChopType;
  INT16               maxDelay;
  INT16               minDelay;
  UINT8               numClkEntries;
  UINT8               numCmdEntries;
  UINT8               numCtlEntries;
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT8               ch;
  UINT8               clk;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  INT16               aepClk[MAX_CLK];
  UINT8               *CtlDelay;
  UINT8               *CmdDelay;
  UINT16              *ClkDelay;
  IoGroupStruct       *CtlTableStruct;
  IoGroupStruct       *CmdTableStruct;
  IoGroupClkStruct    *ClkTableStruct;
  UINT8               MaxChDdr;

  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PACKAGE_DELAY_CCC, Socket);

  if (UbiosGenerationOrHsleEnabled ()) {
    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_POST_PACKAGE_DELAY_CCC, Socket);
    return;
  }

  channelNvList = GetChannelNvList (Host, Socket);
  picoPerPi = GetPicoSecondsPerPiTickForFreq (Host->nvram.mem.ratioIndex);
  MaxChDdr  = GetMaxChDdr ();

#ifdef DEBUG_CODE_BLOCK
  AcquirePrintControl ();
#endif //DEBUG_CODE_BLOCK

  CpuChopType = GetChopType (GetCurrentSocketId ());

  GetCmdDelay (&numCmdEntries, &CmdDelay, &CmdTableStruct);
  GetCtlDelay (&numCtlEntries, &CtlDelay, &CtlTableStruct);
  GetClkDelay (&numClkEntries, &ClkDelay, &ClkTableStruct);


  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "SetStartingCCCBrs10nm => CpuSku=%2d, CtlEntries=%3d\n", CpuChopType, numCtlEntries);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "SetStartingCCCBrs10nm => CpuSku=%2d, CmdEntries=%3d\n", CpuChopType, numCmdEntries);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "SetStartingCCCBrs10nm => CpuSku=%2d, ClkEntries=%3d\n", CpuChopType, numClkEntries);

  for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {

    // TODO: DDR5_TODO. Commented out until the BRS channel mapping is available.
    //if ((*channelNvList)[ch].enabled == 0) {
    //  continue;
    //}

    // Calculate max and min delays and use them to determine a shared base delay.

    // Initialize with extreme values in the opposite direction. (Allowable range is 0 - 255 PI ticks.)
    maxDelay = 0;
    minDelay = MAX_CMD_DELAY;

    for (ai = 0; ai < numCtlEntries; ai++) {
      if (CtlTableStruct[ai].ch != ch || CtlDelay[ai] == 0xFF) {
        continue;
      }
      delay = CtlDelay[ai] / picoPerPi;
      if (delay > maxDelay) {
        maxDelay = delay;
      }
      if (delay < minDelay) {
        minDelay = delay;
      }
    }

    for (ai = 0; ai < numCmdEntries; ai++) {
      if (CmdTableStruct[ai].ch != ch || CmdDelay[ai] == 0xFF) {
        continue;
      }
      delay = CmdDelay[ai] / picoPerPi + 64;  // Additional 64 PI tick delay is due to 3N timing.
      if (delay > maxDelay) {
        maxDelay = delay;
      }
      if (delay < minDelay) {
        minDelay = delay;
      }
    }

    baseDelay = (MAX_CMD_DELAY + 1 - (maxDelay + minDelay)) / 2;

    RcDebugPrintWithDevice (SDBG_MAX, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "SetStartingCCCBrs10nm => CpuSku=%2d, Max Delay=%3d\n", CpuChopType, maxDelay);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "SetStartingCCCBrs10nm => CpuSku=%2d, Min Delay=%3d\n", CpuChopType, minDelay);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "SetStartingCCCBrs10nm => CpuSku=%2d, Base Delay=%3d\n", CpuChopType, baseDelay);

    // Apply base delay (and other modifiers) to control, command, and clock.

    for (ai = 0; ai < numCtlEntries; ai++) {
      if (CtlTableStruct[ai].ch != ch || CtlDelay[ai] == 0xFF) {
        continue;
      }
      delay = CtlDelay[ai] / picoPerPi + baseDelay;
      if (delay > 191) {
        delay = 191;
      } else if (delay < 64) {
        delay = 64;
      }
      GetSetCmdDelay (Host, Socket, CtlTableStruct[ai].ch, CtlTableStruct[ai].iog, CtlTableStruct[ai].side, mode, &delay);
    }

    for (ai = 0; ai < numCmdEntries; ai++) {
      if (CmdTableStruct[ai].ch != ch || CmdDelay[ai] == 0xFF) {
        continue;
      }
      delay = CmdDelay[ai] / picoPerPi + baseDelay + 64;  // Additional 64 PI tick delay is due to 3N timing.
      if (delay > 191) {
        delay = 191;
      }  // Delay cannot be less than 64 due to 3N timing adjustment, so "else if (delay < 64)" is omitted.
      GetSetCmdDelay (Host, Socket, CmdTableStruct[ai].ch, CmdTableStruct[ai].iog, CmdTableStruct[ai].side, mode, &delay);
    }

    dimmNvList = GetDimmNvList (Host, Socket, ch);
    for (clk = 0; clk < MAX_CLK; clk++) {
      // 1:1 association between clock and DIMM assumed, up to a point.
      if (clk < MAX_DIMM && (*dimmNvList)[clk].DcpmmPresent) {
        aepClk[clk] = 64;
      } else {
        aepClk[clk] = 0;
      }
    }

    for (ai = 0; ai < numClkEntries; ai++) {
      // Maximum of piPsFrqTable is 4, clk delay range is 128 (MAX_CK_DELAY). 4*128 - 1 = 511 (0x1FF)
      if ((ClkTableStruct[ai].ch != ch) || (ClkDelay[ai] >= (4*MAX_CK_DELAY - 1))) {
        continue;
      }
      delay = ClkDelay[ai] / picoPerPi + baseDelay + aepClk[ClkTableStruct[ai].clk];
      GetSetClkDelay (Host, Socket, ClkTableStruct[ai].ch, ClkTableStruct[ai].clk, mode, &delay);

    }

  }  // ch loop

#ifdef DEBUG_CODE_BLOCK
  DisplayCCCResults (Host, Socket);
  ReleasePrintControl ();
#endif //DEBUG_CODE_BLOCK

  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_POST_PACKAGE_DELAY_CCC, Socket);

} // SetStartingCCCBrs10nm

#ifdef DDR5_SUPPORT
  /**

  Evaluates the CMD margins and determines the appropriate offset

  @param Host                 - Pointer to Host structure
  @param socket               - Processor socket within the system (0-based)

  @retval MRC_STATUS

  **/
STATIC VOID
EvaluateCMDMarginsBrs (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     ckEnabled[MAX_CLK],
  UINT8     ctlIndex[MAX_CLK],
  INT16     (*cmdLeft)[MAX_CLK],
  INT16     (*cmdRight)[MAX_CLK],
  MRC_GT    (*group)[MAX_CLK],
  GSM_CSN   (*cmdSignal)[MAX_CLK],
  INT16     *cmdOffset
  )
{
  UINT8            dimm;
  UINT8            rank;
  UINT8            clk;
  UINT8            cmdPiGroup;
  UINT8            MaxCmdCtlGroup;
  struct ioGroup   *CmdCtlGroupSignals;
  INT16            minCmdOffset;
  INT16            maxCmdOffset;
  INT16            (*cmdOffsetClk)[MAX_CLK];
  INT16            maxLeftOffset;
  UINT16           ctlMinVal;
  UINT16           ctlMaxVal;
  UINT8            SubChannel;
  INT16            minRightOffset;
  INT16            ckOffset[MAX_CLK];
  UINT8            earlyCmdClkDisable = 0;
  UINT8            parityWorkaround = 0;
  struct ddrRank   (*rankList) [MAX_RANK_DIMM];
  struct dimmNvram (*dimmNvList) [MAX_DIMM];
  UINT8            maxCmdGroupAll;
  UINT8            CmdSetupOffset;
  struct ioGroup   *platformCmdGroupAll;
  MRC_STATUS       status;
  INT16            TempValue;
  UINT8            RelativeDdr4Channel = 0;
  UINT8            Channel = 0;
  SYS_SETUP        *Setup;

  Setup = GetSysSetupPointer ();

  GetCmdCtlGroupAllTable (&CmdCtlGroupSignals, &MaxCmdCtlGroup);
  cmdOffsetClk = (INT16(*)[MAX_CLK]) RcAllocatePool (MaxCmdCtlGroup * 2 * sizeof (*cmdOffsetClk));
  if (cmdOffsetClk == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }

  status = GetCmdCtlGroupList (Host, CmdAll, &maxCmdGroupAll, &platformCmdGroupAll);
  CmdSetupOffset = Setup->mem.cmdSetupPercentOffset;

  //
  // Determine if early CMD/CLK training has been disabled
  //
  dimmNvList = GetDimmNvList (Host, socket, ch);

  //
  // Determine if early CMD/CLK training has been disabled
  //
  earlyCmdClkDisable = ((!IsMemFlowEnabled (EarlyCmdClkTraining)) || parityWorkaround);

  RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate offset per PI group per DIMM \n");
  for (clk = 0; clk < MAX_CLK; clk++) {
    if (ckEnabled[clk] == 0) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      //
      // Initialize these values so the maximum CMD delay can be determined
      //
      minCmdOffset = 255;
      maxCmdOffset = -255;

      //
      // Determine the offset for each CMD pi group and the max offset
      //
      for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {

        cmdPiGroup = cmdPiGroup + MaxCmdCtlGroup * SubChannel;
        //
        // Make sure the eye width is large enough
        //
#ifdef DDR5_SUPPORT
        if (IsBrsPresent (Host, socket)) {
          if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
            if ((cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]) < MinimumMargin (Host, CmdAll)) {
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "Eye width is too small: %d\n",
                              cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]);
              OutputWarning (WARN_CMD_CLK_TRAINING, 0, socket, ch, NO_DIMM, NO_RANK);
              DisableChannelSw (Host, socket, ch);
            }
          }
        } else {
#endif
          if ((cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]) < MinimumMargin (Host, CmdAll)) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Eye width is too small: %d\n",
                            cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]);
            OutputWarning (WARN_CMD_CLK_TRAINING, 0, socket, ch, NO_DIMM, NO_RANK);
            DisableChannelSw (Host, socket, ch);

          }
#ifdef DDR5_SUPPORT
        }
#endif
        if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
          cmdOffsetClk[cmdPiGroup][clk] = (cmdLeft[cmdPiGroup][clk] + cmdRight[cmdPiGroup][clk]) / 2;

          UpdateMinMaxInt (cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);

          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                              "CMD Pi Group %2d clk %d cmdOffset %d\n",
                              cmdPiGroup - MaxCmdCtlGroup * SubChannel, clk, cmdOffsetClk[cmdPiGroup][clk]);
        }
        cmdPiGroup = cmdPiGroup - MaxCmdCtlGroup * SubChannel;
      } // cmdPiGroup loop
    } //subchannel loop
  } // clk offset

  //
  // Calculate the final offset for each CMD pi group
  //
  RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate Final offset per PI group per CH \n");
  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
      // Track minCgOffset, maxCgOffset across all CK groups
      minCmdOffset = 255;
      maxCmdOffset = -255;
      cmdPiGroup = cmdPiGroup + MaxCmdCtlGroup * SubChannel;
      for (clk = 0; clk < MAX_CLK; clk++) {
        if (ckEnabled[clk]) {
          if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
            //if(moveClk) {
            //  UpdateMinMaxInt(cmdPiShift[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);
            //} else {
            UpdateMinMaxInt (cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);
          }
        }
      } // clk loop

      // Save common CMD offset
      cmdOffset[cmdPiGroup] = (maxCmdOffset + minCmdOffset) / 2;
      //
      // Setup/Hold parameter in late CMD CLK (early CMD CLK disabled OR early CMD CLK executed already)
      //
      if (earlyCmdClkDisable || (Host->var.mem.earlyCmdClkExecuted == 1)) {
        cmdOffset[cmdPiGroup] = maxCmdOffset - (((PERCENT - CmdSetupOffset)* (maxCmdOffset - minCmdOffset)) / PERCENT);
      }

      //
      // Write the CMD pi Group
      //
      TempValue = cmdOffset[cmdPiGroup];
      cmdPiGroup = cmdPiGroup - MaxCmdCtlGroup * SubChannel;//DDR5_TODO. channel mapping.
      if ((maxCmdOffset != -255) && (minCmdOffset != 255)) {
        //BRS_TODO. Get channel mapping
        if (SubChannel == 0) {
          RelativeDdr4Channel = 1;
        } else {
          RelativeDdr4Channel = 0;
        }
        Channel = RelativeDdr4Channel + ch;

        GetSetCmdDelay (Host, socket, Channel, platformCmdGroupAll[cmdPiGroup].num, platformCmdGroupAll[cmdPiGroup].side,
          GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &TempValue);//DDR5_TODO. ch mapping.

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                            "CMD Pi Group %2d: maxOffset = %d, minOffset = %d, cmdOffset = %d\n",
                            cmdPiGroup, maxCmdOffset, minCmdOffset, TempValue);
      }
    } // cmdPiGroup loop
  }//sub channel loop

  //
  // Determine the clock offset
  //
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate Clk offset \n");
  for (clk = 0; clk < MAX_CLK; clk++) {
    if (ckEnabled[clk] == 0) {
      continue;
    }
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
        cmdPiGroup = cmdPiGroup + MaxCmdCtlGroup * SubChannel;
        if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
          cmdLeft[cmdPiGroup][clk] = cmdLeft[cmdPiGroup][clk] - cmdOffset[cmdPiGroup];
          cmdRight[cmdPiGroup][clk] = cmdRight[cmdPiGroup][clk] - cmdOffset[cmdPiGroup];

          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, SubChannel, NO_RANK, NO_STROBE, NO_BIT,
                              "<--CMD Pi Group %2d clk %d: cmdLeft %d - cmdRight %d\n",
                              cmdPiGroup - MaxCmdCtlGroup * SubChannel, clk, cmdLeft[cmdPiGroup][clk], cmdRight[cmdPiGroup][clk]);
        }
        cmdPiGroup = cmdPiGroup - MaxCmdCtlGroup * SubChannel;
      } // clk offset
    } // cmdPiGroup loop
  }//subchannel loop

  for (clk = 0; clk < MAX_CLK; clk++) {
    if (ckEnabled[clk] == 0) {
      continue;
    }

    minRightOffset = 255;
    maxLeftOffset = -255;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      for (cmdPiGroup = 0; cmdPiGroup < maxCmdGroupAll; cmdPiGroup++) {
        cmdPiGroup = cmdPiGroup + MaxCmdCtlGroup * SubChannel;
        if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
          UpdateMinMaxInt (cmdLeft[cmdPiGroup][clk], &minCmdOffset, &maxLeftOffset);
          UpdateMinMaxInt (cmdRight[cmdPiGroup][clk], &minRightOffset, &maxCmdOffset);
        }
        cmdPiGroup = cmdPiGroup - MaxCmdCtlGroup * SubChannel;
      } // cmdPiGroup loop
    }//subchannel loop

    ckOffset[clk] = - (minRightOffset + maxLeftOffset) / 2;

    //
    // Write the CTL delay
    //
    ctlMinVal = 255;
    ctlMaxVal = 0;
    for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
      //BRS_TODO. Get channel mapping
      if (SubChannel == 0) {
        RelativeDdr4Channel = 1;
      } else {
        RelativeDdr4Channel = 0;
      }
      Channel = RelativeDdr4Channel + ch;

      GetSetCtlGroupDelay (Host, socket, Channel, ctlIndex[clk] + CtlGrp0, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &ckOffset[clk], &ctlMinVal, &ctlMaxVal);
    }
    //
    // Write the CLK delay
    //
    //BRS_TODO. the CLK is from which DDR4 channel? update the channel mapping.
    GetSetClkDelay (Host, socket, ch, clk, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &ckOffset[clk]);
    //
    // Only move IO Delays if in late CMD CLK (early CMD CLK disabled OR early CMD CLK executed already)
    //
    if (earlyCmdClkDisable || (Host->var.mem.earlyCmdClkExecuted == 1)) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        rankList = GetRankNvList (Host, socket, ch, dimm);
        for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if (clk == (*rankList) [rank].ckIndex) {
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&ckOffset[clk]);
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&ckOffset[clk]);
            GetSetDataGroup (Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&ckOffset[clk]);
          }
        } // rank loop
      } // dimm loop
    }

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<----clk %d ckOffset %d: -(maxLeftOffset:%d + minRightOffset:%d) / 2\n",
                    clk, ckOffset[clk], maxLeftOffset, minRightOffset);

  } // clk loop

  RcFreePool (cmdOffsetClk);
} // EvaluateCMDMarginsBrs

/**

Evaluates the CMD margins and determines the appropriate offset

@param Host                 - Pointer to Host structure
@param socket               - Processor socket within the system (0-based)

@retval MRC_STATUS

**/
STATIC VOID
EvaluateCaBacksideMargins (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     ckEnabled[MAX_CLK_BS_SUBCH_DDR5],
  UINT8     ctlIndex[MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdLeft[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdRight[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  MRC_GT    group[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  GSM_CSN   cmdSignal[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5],
  INT16     cmdOffset[COPY_NUM]
  )
{
  UINT8                 dimm;
  UINT8                 rank;
  UINT8 clk;
  UINT8 cmdPiGroup;
  INT16                 minCmdOffset;
  INT16                 maxCmdOffset;
  INT16                 cmdOffsetClk[COPY_NUM][MAX_CLK_BS_SUBCH_DDR5];
  INT16                 maxLeftOffset;
  UINT16                ctlMinVal;
  UINT16                ctlMaxVal;
  INT16                 minRightOffset;
  INT16                 ckOffset[MAX_CLK];
  UINT8                 earlyCmdClkDisable = 0;
  UINT8                 parityWorkaround = 0;
  struct ddrRank (*rankList) [MAX_RANK_DIMM];
  struct dimmNvram (*dimmNvList) [MAX_DIMM];

  //
  // Determine if early CMD/CLK training has been disabled
  //
  dimmNvList = GetDimmNvList (Host, socket, ch);

  //
  // Determine if early CMD/CLK training has been disabled
  //
  earlyCmdClkDisable = ((!IsMemFlowEnabled (EarlyCmdClkTraining)) || parityWorkaround);//DDR5_TODO.

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate offset per PI group per sub channel per DIMM \n");

  for (clk = 0; clk < MAX_CLK_BS_SUBCH_DDR5; clk++) {
    if (ckEnabled[clk] == 0) {
      continue;
    }
    //
    // Initialize these values so the maximum CMD delay can be determined
    //
    minCmdOffset = 128;
    maxCmdOffset = -128;

    //
    // Determine the offset for each CMD pi group and the max offset
    //
    for (cmdPiGroup = 0; cmdPiGroup < COPY_NUM; cmdPiGroup++) {

      //
      // Make sure the eye width is large enough
      //
      if ((cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]) < 20) { //DDR5_TODO.
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "Eye width is too small: %d\n",
                        cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]);
        EwlOutputType3 (WARN_FPT_CORRECTABLE_ERROR, WARN_CMD_PI_GROUP_SMALL_EYE, socket, ch, NO_DIMM, NO_RANK, EwlSeverityWarning, group[cmdPiGroup][clk], DdrLevel, cmdSignal[cmdPiGroup][clk], (UINT8) (cmdRight[cmdPiGroup][clk] - cmdLeft[cmdPiGroup][clk]));
        DisableChannelSw (Host, socket, ch);
      }
      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
        cmdOffsetClk[cmdPiGroup][clk] = (cmdLeft[cmdPiGroup][clk] + cmdRight[cmdPiGroup][clk]) / 2;

        UpdateMinMaxInt (cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "Backside CA PI Group %2d clk %d cmdOffset %d\n",
                        cmdPiGroup, clk, cmdOffsetClk[cmdPiGroup][clk]);
      }
    } // cmdPiGroup loop

  } // clk offset

  //
  // Calculate the final offset for each CMD pi group
  //
  RcDebugPrint (SDBG_MAX, "\n");
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate Final offset per PI group per CH \n");

  for (cmdPiGroup = 0; cmdPiGroup < COPY_NUM; cmdPiGroup++) {
    minCmdOffset = 255;
    maxCmdOffset = -255;

    for (clk = 0; clk < MAX_CLK_BS_SUBCH_DDR5; clk++) {
      if (ckEnabled[clk]) {
        if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
          UpdateMinMaxInt (cmdOffsetClk[cmdPiGroup][clk], &minCmdOffset, &maxCmdOffset);
        }
      }
    } // clk loop

    // Save common CMD offset
    cmdOffset[cmdPiGroup] = (maxCmdOffset + minCmdOffset) / 2;
    //
    // Write the CMD pi Group
    //
    if ((maxCmdOffset != -255) && (minCmdOffset != 255)) {
      // GetSetCmdDelay(Host, socket, ch, platformCmdCtlGroupAll[cmdPiGroup].num, platformCmdCtlGroupAll[cmdPiGroup].side,
      //   GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &cmdOffset[cmdPiGroup]);
      //DDR5_TODO. Use the MRW to write the delay.
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "CMD Pi Group %2d: maxOffset = %d, minOffset = %d, cmdOffset = %d\n",
                      cmdPiGroup, maxCmdOffset, minCmdOffset, cmdOffset[cmdPiGroup]);
    }
  } // cmdPiGroup loop
  //
  // Determine the clock offset
  //
  RcDebugPrint (SDBG_MAX, "\n");
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Calculate Clk offset \n");
  for (cmdPiGroup = 0; cmdPiGroup < COPY_NUM; cmdPiGroup++) {
    for (clk = 0; clk < MAX_CLK_BS_SUBCH_DDR5; clk++) {
      if (ckEnabled[clk] == 0) {
        continue;
      }
      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
        cmdLeft[cmdPiGroup][clk] = cmdLeft[cmdPiGroup][clk] - cmdOffset[cmdPiGroup];
        cmdRight[cmdPiGroup][clk] = cmdRight[cmdPiGroup][clk] - cmdOffset[cmdPiGroup];

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "<--CMD Pi Group %2d clk %d: cmdLeft %d - cmdRight %d\n",
                        cmdPiGroup, clk, cmdLeft[cmdPiGroup][clk], cmdRight[cmdPiGroup][clk]);
      }
    } // clk offset
  } // cmdPiGroup loop

  for (clk = 0; clk < MAX_CLK_BS_SUBCH_DDR5; clk++) {
    if (ckEnabled[clk] == 0) {
      continue;
    }

    minRightOffset = 255;
    maxLeftOffset = -255;

    for (cmdPiGroup = 0; cmdPiGroup < COPY_NUM; cmdPiGroup++) {
      if ((cmdLeft[cmdPiGroup][clk] != -UNMARGINED_CMD_EDGE) && (cmdRight[cmdPiGroup][clk] != UNMARGINED_CMD_EDGE)) {
        UpdateMinMaxInt (cmdLeft[cmdPiGroup][clk], &minCmdOffset, &maxLeftOffset);
        UpdateMinMaxInt (cmdRight[cmdPiGroup][clk], &minRightOffset, &maxCmdOffset);
      }
    } // cmdPiGroup loop

    ckOffset[clk] = - (minRightOffset + maxLeftOffset) / 2;

    //
    // Write the CTL delay--shift the CS Delay--DDR5_TODO.
    //
    ctlMinVal = 255;
    ctlMaxVal = 0;
    //
    // DDR5_TODO. Based on CLK--A/B CS mapping to shift the QCS delay.
    //
    GetSetCtlGroupDelay (Host, socket, ch, ctlIndex[clk] + CtlGrp0, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &ckOffset[clk], &ctlMinVal, &ctlMaxVal);
    //
    // Write the CLK delay
    //
    GetSetClkDelay (Host, socket, ch, clk, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE, &ckOffset[clk]);//DDR5_TODO.
    //
    // Only move IO Delays if in late CMD CLK (early CMD CLK disabled OR early CMD CLK executed already)
    //
    //DDR5_TODO. To update the below code.
    if (earlyCmdClkDisable || (Host->var.mem.earlyCmdClkExecuted == 1)) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        rankList = GetRankNvList (Host, socket, ch, dimm);
        for (rank = 0; rank < (*dimmNvList) [dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          if (clk == (*rankList) [rank].ckIndex) {
            /*
            GetSetDataGroup(Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, RecEnDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&ckOffset[clk]);
            GetSetDataGroup(Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&ckOffset[clk]);
            GetSetDataGroup(Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, 0, DdrLevel, TxDqsDelay, GSM_WRITE_OFFSET + GSM_UPDATE_CACHE,
              (INT16 *)&ckOffset[clk]);
            */
          }
        } // rank loop
      } // dimm loop
    }

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<----clk %d ckOffset %d: -(maxLeftOffset:%d + minRightOffset:%d) / 2\n",
                    clk, ckOffset[clk], maxLeftOffset, minRightOffset);

  } // clk loop
} // EvaluateCaBacksideMargins

/**

Given an array of command delays relative to current clock and control delays,
this function will combine shared settings in the DDRIO design and normalize the
lowest command, clock or control value to their minimum limits. This is for BRS.

@param[in] Host                 - Pointer to Host structure
@param[in] Socket               - Processor socket within the system (0-based)
@param[in] Ch                   - DDR channel number within the processor socket (0-based)
@param[in] Mode                 - Bit-field of different modes
@param[in] ListType             - Selects type of each list
@param[in] ListSize             - Number of entries in each list
@param[in] Value                - Array of pointers to listType
@param[in] ValueSize            - List data size

@retval MRC_STATUS

**/
MRC_STATUS
SetCombinedCmdGroupDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Mode,
  IN UINT8     ListType,
  IN UINT16    ListSize,
  IN VOID      *Value,
  IN UINT32    ValueSize
  )
{
  UINT8                 Dimm;
  UINT8                 Rank;
  UINT8                 Clk;
  UINT8                 ListIndex;
  UINT8                 CmdPiGroup;
  UINT8                 CkEnabled[MAX_CLK];
  UINT8                 CtlIndex[MAX_CLK];
  UINT8                 *Allocation;
  INT16                 (*CmdLeft)[MAX_CLK];
  INT16                 (*CmdRight)[MAX_CLK];
  MRC_GT                (*CmdGroup)[MAX_CLK];
  GSM_CSN               (*CmdSignal)[MAX_CLK];
  INT16                 *CmdOffset;
  UINT8                 Channel = 0;
  UINT8                 SubChannel;
  UINT8                 MaxCmdCtlGroupAll;
  struct ioGroup        *PlatformCmdCtlGroupAll;
  MRC_STATUS            Status;
  GSM_CSEDGE_CA         (*RcdsignalListEdgeRank) [SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  GSM_CSEDGE_CA         (*DramsignalListEdgeRank) [SUB_CH][MAX_RANK_CH][DRAM_CA_SIGNALS_TO_SWEEP];
  struct signalIOGroup  SignalGroup;
  struct ddrRank        (*RankList) [MAX_RANK_DIMM];
  struct dimmNvram      (*DimmNvList) [MAX_DIMM];

  Status = MRC_STATUS_SUCCESS;

  Status = GetCmdCtlGroupList (Host, CmdCtlAll, &MaxCmdCtlGroupAll, &PlatformCmdCtlGroupAll);

  Allocation = RcAllocatePool (MaxCmdCtlGroupAll * 2 * (sizeof (*CmdLeft) + sizeof (*CmdRight) + sizeof (*CmdGroup) + sizeof (*CmdSignal) +
                               sizeof (*CmdOffset)));
  if (Allocation == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return MRC_STATUS_FAILURE;
  }

  CmdLeft = (INT16(*)[MAX_CLK])((UINTN)Allocation);
  CmdRight = (INT16(*)[MAX_CLK])((UINTN)CmdLeft + (MaxCmdCtlGroupAll * 2 * sizeof (*CmdLeft)));
  CmdGroup = (MRC_GT(*)[MAX_CLK])((UINTN)CmdRight + (MaxCmdCtlGroupAll * 2 * sizeof (*CmdRight)));
  CmdSignal = (GSM_CSN(*)[MAX_CLK])((UINTN)CmdGroup + (MaxCmdCtlGroupAll * 2 * sizeof (*CmdGroup)));
  CmdOffset = (INT16*)((UINTN)CmdSignal + (MaxCmdCtlGroupAll * 2 * sizeof (*CmdSignal)));;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (Clk = 0; Clk < MAX_CLK; Clk++) { //DDR5 8 clks
    CkEnabled[Clk] = 0;
    CtlIndex[Clk] = 0;
  } // clk loop

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    RankList = GetRankNvList (Host, Socket, Ch, Dimm);
    for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {

      // If Rank enabled
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      Clk = (*RankList) [Rank].ckIndex;
      CtlIndex[Clk] = (*RankList) [Rank].ctlIndex;
      CkEnabled[Clk] = 1;
    } // Rank loop
  } // Dimm loop

  //
  // Initialize the edges for each CMD pi group
  //
  for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupAll * 2; CmdPiGroup++) {
    // Init CK Composite edges to 255
    for (Clk = 0; Clk < MAX_CLK; Clk++) {
      CmdLeft[CmdPiGroup][Clk] = -UNMARGINED_CMD_EDGE;
      CmdRight[CmdPiGroup][Clk] = UNMARGINED_CMD_EDGE;
      //CmdOffsetClk[CmdPiGroup][Clk] = 0;
    } // Clk loop
    CmdOffset[CmdPiGroup] = 0;
  } // CmdPiGroup loop

  if (ListType == CMDCTL_SIGNAL_DELAY) {
    //
    // Combine edges if necessary
    //
    if (Mode == GSM_COMBINE_EDGES) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "Calculate CA Composite edges per PI Group per Dimm\n");
      if (CheckRCDLoopback (Host) && ValueSize == sizeof (*RcdsignalListEdgeRank)) {
        RcdsignalListEdgeRank = Value;
      } else if (ValueSize == sizeof (*DramsignalListEdgeRank)){
        DramsignalListEdgeRank = Value;
      } else {
        ASSERT (FALSE);
      }
      for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          RankList = GetRankNvList (Host, Socket, Ch, Dimm);

          for (Rank = 0; Rank < (*DimmNvList) [Dimm].numRanks; Rank++) {
            if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            //skip ranks >0 if rdimm
            if ((Host->nvram.mem.dimmTypePresent != UDIMM) && (Rank > 0)) {
              continue;
            }

            //
            // Loop through the list to create the composite left and right edges for each CMD group
            //
            for (ListIndex = 0; ListIndex < ListSize; ListIndex++) {

              Clk = (*RankList) [Rank].ckIndex;
              //
              // Get the silicon pi group this signal belongs to
              //
              if (CheckRCDLoopback (Host)) {
                Status = SignalToPiGroupBrs (Host, (*RcdsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].signal, &SignalGroup, &Channel);
              } else {
                Status = SignalToPiGroupBrs (Host, (*DramsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].signal, &SignalGroup, &Channel);
              }
              //
              // Get the index into the CMD silicon pi group
              //
              for (CmdPiGroup = 0; CmdPiGroup < MaxCmdCtlGroupAll; CmdPiGroup++) {
                if ((SignalGroup.group.num == PlatformCmdCtlGroupAll[CmdPiGroup].num) &&
                    (SignalGroup.group.side == PlatformCmdCtlGroupAll[CmdPiGroup].side)) {
                  //
                  // We found the CMD pi group so break out of the loop
                  //
                  break;
                }
              } // CmdPiGroup loop
              CmdPiGroup = CmdPiGroup + MaxCmdCtlGroupAll * SubChannel;
              RC_FATAL_ERROR (CmdPiGroup != MaxCmdCtlGroupAll, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_44);

              //
              // Update the composite left and right edges for the current CMD pi group relative to the clock
              //
              if (CheckRCDLoopback (Host)) {
                UpdateEdges ((*RcdsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].le, (*RcdsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].re,
                  &CmdLeft[CmdPiGroup][Clk], &CmdRight[CmdPiGroup][Clk]);
                CmdGroup[CmdPiGroup][Clk] = MrcGtDelim;
                CmdSignal[CmdPiGroup][Clk] = (*RcdsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].signal;
                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                                    "%8a: CA PI Group %2d, Group Num %2d, Side %2d,  Clk %d: le = %3d re = %3d, Left = %3d Right = %3d\n",
                                    GetSignalStrDdr5 ((*RcdsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].signal), CmdPiGroup - MaxCmdCtlGroupAll * SubChannel, SignalGroup.group.num, SignalGroup.group.side, Clk,
                                    (*RcdsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].le, (*RcdsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].re,
                                    CmdLeft[CmdPiGroup][Clk], CmdRight[CmdPiGroup][Clk]);
              } else {
                UpdateEdges ((*DramsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].le, (*DramsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].re,
                  &CmdLeft[CmdPiGroup][Clk], &CmdRight[CmdPiGroup][Clk]);
                CmdGroup[CmdPiGroup][Clk] = MrcGtDelim;
                CmdSignal[CmdPiGroup][Clk] = (*DramsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].signal;
                RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, SubChannel, Rank, NO_STROBE, NO_BIT,
                                    "%8a: CA PI Group %2d, Group Num %2d, Side %2d,  Clk %d: le = %3d re = %3d, Left = %3d Right = %3d\n",
                                    GetSignalStrDdr5 ((*DramsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].signal), CmdPiGroup, SignalGroup.group.num, SignalGroup.group.side, Clk,
                                    (*DramsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].le, (*DramsignalListEdgeRank) [SubChannel][ (*RankList) [Rank].rankIndex][ListIndex].re,
                                    CmdLeft[CmdPiGroup][Clk], CmdRight[CmdPiGroup][Clk]);
              }
            } // ListIndex loop
            RcDebugPrint (SDBG_MAX, "\n");
          } // Rank loop
        }//Dimm loop
      } // SubChannel loop
      //
      // Evaluate CMD margins and offset CMD, CTL, and Clk appropriately
      //
      EvaluateCMDMarginsBrs (Host, Socket, Ch, CkEnabled, CtlIndex, CmdLeft, CmdRight, CmdGroup, CmdSignal, CmdOffset);
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "ERROR: This SetCombinedCmd Mode not supported!\n");
    }
  } else {
    Status = MRC_STATUS_GROUP_NOT_SUPPORTED;
  }

  RcFreePool (Allocation);

  return Status;
} // SetCombinedCmdGroupDdr5

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
  )
{
  MRC_STATUS    Status;
  GSM_CSEDGE_CA (*RcdsignalListEdgeCh) [MAX_CH][SUB_CH][MAX_RANK_CH][RCD_CA_SIGNALS_TO_SWEEP];
  GSM_CSEDGE_CA (*DramsignalListEdgeCh) [MAX_CH][SUB_CH][MAX_RANK_CH][DRAM_CA_SIGNALS_TO_SWEEP];
  GSM_CSEDGE_CA (*RcdBacksidesignalListEdgeCh) [MAX_CH][SUB_CH][MAX_RANK_CH][COPY_NUM][BACKSIDE_CA_SIGNALS_TO_SWEEP];
  UINT8         MaxChDdr;

  if (GetCurrentTestType (Socket) == EarlyCaBacksideLoopbackTest && ValueSize == sizeof (*RcdBacksidesignalListEdgeCh)) {
    RcdBacksidesignalListEdgeCh = Value;
  } else if (CheckRCDLoopback (Host) && ValueSize == sizeof (*RcdsignalListEdgeCh)) {
    RcdsignalListEdgeCh = Value;
  } else if (ValueSize == sizeof (*DramsignalListEdgeCh)) {
    DramsignalListEdgeCh = Value;
  } else {
    ASSERT (FALSE);
  }

  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch += GetChInc (Host)) {
    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }
    //
    // Evaluate results and write them
    //
#ifdef DEBUG_CODE_BLOCK
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Summary:\n");
#endif //DEBUG_CODE_BLOCK
    if (GetCurrentTestType (Socket) == EarlyCaBacksideLoopbackTest) {
      //DDR5_TODO.
      Status = SetCombinedCaClkBacksideDdr5 (Host, Socket, Ch, SubChannel, Level, Mode, ListType, ListSize, &(*RcdBacksidesignalListEdgeCh) [Ch]);
    } else if (CheckRCDLoopback (Host)) {
      Status = SetCombinedCmdGroupDdr5 (Host, Socket, Ch, Mode, ListType, ListSize, (*RcdsignalListEdgeCh) [Ch], sizeof ((*RcdsignalListEdgeCh) [Ch]));
    } else {
      Status = SetCombinedCmdGroupDdr5 (Host, Socket, Ch, Mode, ListType, ListSize, (*DramsignalListEdgeCh) [Ch], sizeof ((*DramsignalListEdgeCh) [Ch]));
    }
  } // Ch loop

  return Status;
} //SetCombinedCaGroupDdr5

/**
  Gets the silicon pi group from the signal name in Brs

  @param Host:     Pointer to SYSHOST
  @param sig:      Enumerated signal name

  @retval MRC_STATUS

**/
STATIC MRC_STATUS
SignalToPiGroupBrs (
  PSYSHOST              Host,
  GSM_CSN               sig,
  struct signalIOGroup  *signalGroup,
  UINT8                 *Channel
  )
{
  UINT8                sigLoop;
  UINT8                maxSignals;
  UINT8                MaxBrsSignals;
  GSM_CSN              SignalLocal;
  struct signalIOGroup *signals;
  struct signalMapping *signalsBrs;
  MRC_STATUS           status;
  //
  // Mapping the DDR5 signal to DDR4 signal.
  //
  GetDdrSignalsMappingChip (&signalsBrs, &MaxBrsSignals);

  for (sigLoop = 0; sigLoop < MaxBrsSignals; sigLoop++) {
    if (signalsBrs[sigLoop].signalDdr5.Signal == sig) { //DDR5_TODO. Add ch check when BRS channel/pin mapping is finalized.
      SignalLocal = signalsBrs[sigLoop].signalDdr4.Signal;
      *Channel = signalsBrs[sigLoop].signalDdr4.Channel;
      break;
    }
  } // sigLoop loop

  if (sigLoop == MaxBrsSignals) {
    RcDebugPrint (SDBG_MAX, "Unable to find signal %a\n", GetSignalStr (sig));
    status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
    return status;
  }

  //
  // Get the signal IO group.
  //
  GetDdrSignalsChip (&signals, &maxSignals);

  for (sigLoop = 0; sigLoop < maxSignals; sigLoop++) {
    if (signals[sigLoop].sig == SignalLocal) {
      *signalGroup = signals[sigLoop];
      break;
    }
  } // sigLoop loop

  if (sigLoop == maxSignals) {
    RcDebugPrint (SDBG_MAX, "Unable to find signal %a\n", GetSignalStr (sig));
    status = MRC_STATUS_SIGNAL_NOT_SUPPORTED;
  } else {
    status = MRC_STATUS_SUCCESS;
  }

  return status;
} // SignalToPiGroupBrs


/**
  Multi-use function to either get or set signal delays based on the provided group number

  @param Host:     Pointer to SYSHOST
  @param socket:   Socket number
  @param ch:       Channel number
  @param level:    IO level to access
  @param sig:      Enumerated signal name
  @param mode:     GSM_READ_CSR - Read the data from hardware and not cache
                   GSM_READ_ONLY - Do not write
                   GSM_WRITE_OFFSET - Write offset and not value
                   GSM_FORCE_WRITE - Force the write
  @param value:    Absolute value or offset selected by GSM_WRITE_OFFSET

  @retval MRC_STATUS

**/
MRC_STATUS
GetSetSignalDdr5 (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  MRC_LT    level,
  GSM_CSN   sig,
  UINT8     mode,
  INT16     *value
  )
{
  MRC_STATUS            status;
  UINT8                 RelativeDdr4Channel;
  UINT8                 Channel;
  struct signalIOGroup  signalGroup;

  status = MRC_STATUS_SUCCESS;
  //
  // Get the silicon pi group this signal belongs to
  //
  if (IsBrsPresent (Host, socket)) {
    status = SignalToPiGroupBrs (Host, sig, &signalGroup, &RelativeDdr4Channel);

    if (status == MRC_STATUS_SUCCESS) {
      Channel = RelativeDdr4Channel + ch;
      status = GetSetCmdDelay (Host, socket, Channel, signalGroup.group.num, signalGroup.group.side, mode, value);
    }
  } else {
    status = SignalToPiGroup (Host, sig, &signalGroup);

    if (status == MRC_STATUS_SUCCESS) {
      status = GetSetCmdDelay (Host, socket, ch, signalGroup.group.num, signalGroup.group.side, mode, value);
    }
  }
  return status;
} // GetSetSignalDdr5

#endif //DDR5_SUPPORT
