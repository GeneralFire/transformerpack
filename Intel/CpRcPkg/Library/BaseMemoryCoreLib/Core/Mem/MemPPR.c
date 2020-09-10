/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2019 Intel Corporation. <BR>

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
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/IoAccessLib.h>
#include <Library/CheckpointLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>
#include <Memory/JedecDefinitions.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/PprCmosLib.h>

/**

  Run write/read test on a row

  @param Host     - Pointer to sysHost
  @param socket   - socket id
  @param chBitMask- target CHs on which PPR seq. should be executed
  @param pprAddr  - DRAM Address that need to be repaired
  @param bwSerr   - CPGC Error status

  @retval status - SUCCESS / failure

**/
UINT32
RowTestPPR (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitMask,
  PPR_ADDR  pprAddr[MAX_CH],
  UINT32    DramMask[MAX_CH],
  UINT32    bwSerr[MAX_CH][3]
  )
{
  UINT32  status = SUCCESS;
  UINT8   ch;
  UINT16  testPattern = 0;
  UINT8   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  if (GetSkipPprRowTestFlag ()) {
    return SUCCESS;
  }
  //
  // Test pattern all 1s
  //
  testPattern = DDR_CPGC_MEM_TEST_ALL0 | DDR_CPGC_MEM_TEST_INVERTPAT;
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }

    CpgcPprTestGlobalSetup (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, pprAddr[ch].subRank,
      DramMask[ch], pprAddr[ch].bank, pprAddr[ch].row, testPattern | DDR_CPGC_MEM_TEST_WRITE);
  }

  CpgcAdvTrainingExecTest (Host, socket, chBitMask, testPattern);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, pprAddr[ch].dimm, NO_SUBCH, pprAddr[ch].rank, NO_STROBE, NO_BIT,
                                 "RowTestPPR - Row: 0x%x Write all 1s done\n", pprAddr[ch].row);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }

    CpgcPprTestGlobalSetup (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, pprAddr[ch].subRank,
      DramMask[ch], pprAddr[ch].bank, pprAddr[ch].row, testPattern | DDR_CPGC_MEM_TEST_READ);
  }

  CpgcAdvTrainingExecTest (Host, socket, chBitMask, testPattern | DDR_CPGC_MEM_TEST_READ);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, pprAddr[ch].dimm, NO_SUBCH, pprAddr[ch].rank, NO_STROBE, NO_BIT,
        "RowTestPPR - Row: 0x%x Read all 1s done\n", pprAddr[ch].row);
  }

  status = CollectTestResults (Host, socket, chBitMask, bwSerr);

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "RowTestPPR - Row Read Test status :%x\n", status);

  if (status) {
    return status;
  }

  //
  // Test pattern all 0s
  //
  testPattern = DDR_CPGC_MEM_TEST_ALL0;
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }
    CpgcPprTestGlobalSetup (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, pprAddr[ch].subRank,
      DramMask[ch], pprAddr[ch].bank, pprAddr[ch].row, testPattern | DDR_CPGC_MEM_TEST_WRITE);
  }
  CpgcAdvTrainingExecTest (Host, socket, chBitMask, testPattern | DDR_CPGC_MEM_TEST_WRITE);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }
     RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, pprAddr[ch].dimm, NO_SUBCH, pprAddr[ch].rank, NO_STROBE, NO_BIT,
                               "RowTestPPR - Row: 0x%x Write all 0s done\n", pprAddr[ch].row);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }
    CpgcPprTestGlobalSetup (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, pprAddr[ch].subRank,
      DramMask[ch], pprAddr[ch].bank, pprAddr[ch].row, testPattern | DDR_CPGC_MEM_TEST_READ);
  }

  CpgcAdvTrainingExecTest (Host, socket, chBitMask, testPattern | DDR_CPGC_MEM_TEST_READ);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }
     RcDebugPrintWithDevice (SDBG_MAX, socket, ch, pprAddr[ch].dimm, NO_SUBCH, pprAddr[ch].rank, NO_STROBE, NO_BIT,
         "RowTestPPR - Row: 0x%x Read all 0s done\n", pprAddr[ch].row);
  } // ch loop

  status = CollectTestResults (Host, socket, chBitMask, bwSerr);

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "RowTestPPR - Row Read Test status :%x\n", status);

  return status;
}

/**

  Get fail device from CPGC error status

  @param Host     - Pointer to sysHost
  @param socket   - socket id
  @param ch       - DDR CH ID
  @param dimm     - DIMM ID
  @param bwSerr   - CPGC Error status

  @retval dramMask - bit map of DRAMs that failed (BIT 0 - DRAM0, BIT1 - DRAM1...and so on)

**/
UINT32
GetFailDevice (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT32    bwSerr[3]
  )
{
  UINT8   dram;
  UINT8   i;
  UINT32  dramMask;
  struct  channelNvram (*channelNvList)[MAX_CH];
  struct  dimmNvram  (*dimmNvList)[MAX_DIMM];
  UINT8   BuddyByte;
  UINT8   MSVx4 = GetMaxStrobeValid (Host);

  RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
             "GetFailDevice - bwSerr[0]:0x%08x, bwSerr[1]:0x%08x, bwSerr[2]:0x%08x\n", bwSerr[0], bwSerr[1], bwSerr[2]);
  dimmNvList = GetDimmNvList (Host, socket, ch);
  channelNvList = GetChannelNvList (Host, socket);

  dram = DRAM_UNKNOWN;
  dramMask = 0;
  for (i = 0; i < 4; i++) {
    if (((bwSerr[0] >> (i * 8)) & 0xFF) != 0) {
      dram = 0;
      if ((*dimmNvList)[dimm].x4Present) {
        if (((bwSerr[0] >> (i * 8)) & 0x0F) != 0) {
          dram = i*2;
          if (dram != DRAM_UNKNOWN) {
            dramMask |= 1 << dram;
          }
        }
        if (((bwSerr[0] >> (i * 8)) & 0xF0) != 0) {
          dram = i*2 + 1;
          if (dram != DRAM_UNKNOWN) {
            dramMask |= 1 << dram;
          }
        }
      } else {
        dram = i;
        if (dram != DRAM_UNKNOWN) {
          dramMask |= 1 << dram;
        }
      }
    }

    if (((bwSerr[1] >> (i * 8)) & 0xFF) != 0) {
      dram = 4;
      if ((*dimmNvList)[dimm].x4Present) {
        if (((bwSerr[1] >> (i * 8)) & 0x0F) != 0) {
          dram = i*2 + 8;
          if (dram != DRAM_UNKNOWN) {
            dramMask |= 1 << dram;
          }
        }
        if (((bwSerr[1] >> (i * 8)) & 0xF0) != 0) {
          dram = i*2 + 8 + 1;
          if (dram != DRAM_UNKNOWN) {
            dramMask |= 1 << dram;
          }
        }
      } else {
        dram = i + 4;
        if (dram != DRAM_UNKNOWN) {
          dramMask |= 1 << dram;
        }
      }
    }
  }

  if ((bwSerr[2] & 0xFF) != 0) {
    dram = 8;
    if ((*dimmNvList)[dimm].x4Present) {
      if ((bwSerr[2] & 0x0F) != 0) {
        dram = 16;
        dramMask |= 1 << dram;
      }
      if ((bwSerr[2] & 0xF0) != 0) {
        dram = 17;
        dramMask |= 1 << dram;
      }
    } else {
      dram = 8;
      dramMask |= 1 << dram;
    }
  }

  //
  // Handle x16 buddy byte
  //
  if ((*channelNvList)[ch].features & X16_PRESENT) {
    for (dram = 0; dram < MSVx4 / 2; dram++) {
      if (dramMask & (1 << dram)) {
        BuddyByte = (*dimmNvList)[dimm].LogicalX16Mapping.PairLogical[dram];
        if (BuddyByte != 0xFF) {
          dramMask |= (1 << BuddyByte);
        }
      }
    }
  }
  return dramMask;
}

/**

  Execute PPR sequence on the specified address

  @param Host     - Pointer to sysHost
  @param socket   - socket id
  @param chBitMask- target CHs on which PPR seq. should be executed
  @param pprAddr  - DRAM Address that need to be repaired
  @param pprType  - PPR type - hard / soft PPR
  @param pprStatus - Status of PPR operation

  @retval SUCCESS

**/
UINT32
ExecutePPR (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitMask,
  PPR_ADDR  pprAddr[MAX_CH],
  PPR_TYPE  pprType,
  UINT32    pprStatus[MAX_CH]
  )
{
  UINT32  status = SUCCESS;
#ifndef DDR5_SUPPORT
  UINT8   ch = 0;
  UINT8 controlWordData = 0;
  UINT32  tPGM = 1;
  UINT16  mrsData;
  struct  channelNvram (*channelNvList)[MAX_CH];
  struct  dimmNvram  (*dimmNvList)[MAX_DIMM];
  struct  rankDevice (*rankStruct)[MAX_RANK_DIMM];
  PAGE_TIMER_STATE   PageTableTimerState[MAX_CH];
  UINT32  bwSerr[MAX_CH][3];
  UINT32  failDevMask;
  UINT8   MaxChDdr;
  UINT32  DramMask[MAX_CH];
  UINT8   Dimm;
  UINT8   Nibble;
  UINT8   BuddyByte;
  UINT8   MSVx4 = GetMaxStrobeValid (Host);

  MaxChDdr = GetMaxChDdr ();
  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_PRE_TEST, 0);
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "ExecutePPR - START\n");

  //
  // Convert nibbleMask input to legacy dramMask format in local variable
  //
  channelNvList = GetChannelNvList (Host, socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    DramMask[ch] = 0;

    Dimm = pprAddr[ch].dimm;
    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (Nibble = 0; Nibble < MSVx4; Nibble++) {

      if (pprAddr[ch].nibbleMask & (1 << Nibble)) {
        if ((*dimmNvList)[Dimm].x4Present) {
          DramMask[ch] |= (1 << Nibble);
        } else {
          DramMask[ch] |= (1 << (Nibble / 2));
        }
        if ((*channelNvList)[ch].features & X16_PRESENT) {
          BuddyByte = (*dimmNvList)[Dimm].LogicalX16Mapping.PairLogical[Nibble / 2];
          if (BuddyByte != 0xFF) {
            DramMask[ch] |= (1 << BuddyByte);
          }
        }
      }
    }
  }

  //
  // Test row to find device that failed (required to handle UC errors)
  //
  if (RowTestPPR (Host, socket, chBitMask, pprAddr, DramMask, bwSerr)) {
    //
    // Row test failed, figure out which CH / device failed.
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((~chBitMask) & (1 << ch)) {
        continue;
      }
      failDevMask = GetFailDevice (Host, socket, ch, pprAddr[ch].dimm, bwSerr[ch]);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, pprAddr[ch].dimm, NO_SUBCH, pprAddr[ch].rank, NO_STROBE, NO_BIT,
          "ExecutePPR - Pre-PPR Row test failed - dramMask = 0x%x\n", failDevMask);

      //
      // if pprAddr[ch].dramMask is 0, it is possible UC error and runtime handler could not find the fail devices;
      // initialize with failDevMask value
      //
      if (DramMask[ch] == 0) {
        DramMask[ch] = failDevMask;
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, pprAddr[ch].dimm, NO_SUBCH, pprAddr[ch].rank, NO_STROBE, NO_BIT,
            "ExecutePPR - Input dramMask is 0, initializing with detected fail mask = 0x%x\n", DramMask[ch]);
      }
    } // ch loop
  } // RowTestPPR

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }
    pprStatus[ch] = PPR_STS_SUCCESS;
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, pprAddr[ch].dimm, NO_SUBCH, pprAddr[ch].rank, NO_STROBE, NO_BIT,
      "ExecutePPR - Row: 0x%x, Bank: %d, CID: %d, DramMask: 0x%x\n", pprAddr[ch].row, pprAddr[ch].bank, pprAddr[ch].subRank, DramMask[ch]);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }
    //
    // Enable page close timer for short period to guarantee that all pages are closed
    //
    if (GetPageTableTimerState (Host, socket, ch) == PageTimerDisable) {
      SetPageTableTimer (Host, socket, ch, PageTimerEnable);
      FixedDelayMicroSecond (1);
      SetPageTableTimer (Host, socket, ch, PageTimerDisable);
    }
    //
    // Pre-charge setup
    //
    CpgcAdvCmdParity (Host, socket, ch, GetPhyRank (pprAddr[ch].dimm, pprAddr[ch].rank), pprAddr[ch].subRank, PRECHARGE, NormalCadbSetup);
  } // ch loop

  //
  // Precharge all Banks
  //
  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_PREA_BEFORE, 0);
  ExecuteCmdSignalTest (Host, socket, chBitMask, NO_SUBCH);
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "ExecutePPR - Pre-charge before PPR\n");

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }

    //
    // Disable page timers
    //
    PageTableTimerState[ch] = GetPageTableTimerState (Host, socket, ch);
    if (PageTableTimerState[ch] == PageTimerEnable) {
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "ExecutePPR - Disable Page Timer\n");
      SetPageTableTimer (Host, socket, ch, PageTimerDisable);
    }


    dimmNvList = GetDimmNvList (Host, socket, ch);
    rankStruct = GetRankStruct (Host, socket, ch, pprAddr[ch].dimm);

    if (pprType == PprTypeHard) {
      tPGM = 2000000;    // 2000 ms for hard PPR
      mrsData = (*rankStruct)[pprAddr[ch].rank].MR4 | BIT13;
    } else {
      tPGM = 1;         // 1us for soft PPR (actually tWR is enough, but we can do delays only in uS resolution)
      mrsData = (*rankStruct)[pprAddr[ch].rank].MR4 | BIT5;
    }

    //
    // Workaround: Disable DB snooping by writing to F0RCBx
    //
    if (IsLrdimmPresent (socket, ch, pprAddr[ch].dimm)) {
      OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_DIS_DB_SNOOP, ch);
      //Obtain the previously programmed value and change it
      controlWordData = (*dimmNvList)[pprAddr[ch].dimm].rcxCache[RDIMM_RCBx >> 4];
      controlWordData |= BIT3;
      WriteRC (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, controlWordData, RDIMM_RCBx);
    }

    //
    // MR command to enable PPR Mode
    //
    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_EN_PPR_MODE, ch);
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                         "ExecutePPR - MR4 write to enable PPR: 0x%0x\n", mrsData);
    WriteMRS (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, mrsData, BANK4);

    //
    // Issue Guard Key
    //
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                         "ExecutePPR - Issue 4 MR0 guard key sequence\n");
    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_EN_GUARD_KEY0, ch);
    WriteMRS (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, PPR_GUARD_KEY0, BANK0);
    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_EN_GUARD_KEY1, ch);
    WriteMRS (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, PPR_GUARD_KEY1, BANK0);
    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_EN_GUARD_KEY2, ch);
    WriteMRS (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, PPR_GUARD_KEY2, BANK0);
    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_EN_GUARD_KEY3, ch);
    WriteMRS (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, PPR_GUARD_KEY3, BANK0);


    //
    // Setup to issue WR Command with PPR DQ pattern
    //
    CpgcSetupPprPattern (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, pprAddr[ch].subRank, DramMask[ch], pprAddr[ch].bank, pprAddr[ch].row);
  } // ch loop

  //
  // Run PPR sequence
  //
  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_WR_PATTERN, 0);
  CpgcAdvTrainingExecTest (Host, socket, chBitMask, 0);

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "ExecutePPR - WR with PPR pattern issued\n");
  //
  // Wait for tPGM - 2000 mS for hard PPR and tWR for soft PPR
  //
  FixedDelayMicroSecond (tPGM);

  //
  // Issue pre-charge
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }
    //
    // Enable page close timer for short period to guarantee that all pages are closed
    //
    if (GetPageTableTimerState (Host, socket, ch) == PageTimerDisable) {
      SetPageTableTimer (Host, socket, ch, PageTimerEnable);
      FixedDelayMicroSecond (1);
      SetPageTableTimer (Host, socket, ch, PageTimerDisable);
    }
    //
    // Pre-charge setup
    //
    CpgcAdvCmdParity (Host, socket, ch, GetPhyRank (pprAddr[ch].dimm, pprAddr[ch].rank), pprAddr[ch].subRank, PRECHARGE, NormalCadbSetup);
  } // ch loop

  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_PREA_AFTER, 0);
  ExecuteCmdSignalTest (Host, socket, chBitMask, NO_SUBCH);
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "ExecutePPR - Pre-charge after PPR\n");
  //
  // Wait for at least tPGM_exit - 15 nS
  //
  FixedDelayMicroSecond (1);  // 1 uS

  //
  // MR command to disable PPR Mode
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }

    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_DIS_PPR_MODE, ch);
    rankStruct = GetRankStruct (Host, socket, ch, pprAddr[ch].dimm);
    WriteMRS (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, (*rankStruct)[pprAddr[ch].rank].MR4, BANK4);
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, pprAddr[ch].dimm, NO_SUBCH, pprAddr[ch].rank, NO_STROBE, NO_BIT,
                            "ExecutePPR - Disable PPR - MRS Command issued: %x\n", (*rankStruct)[pprAddr[ch].rank].MR4);
  } // ch loop

  //
  // Wait for at least tPGMPST - 50uS
  //
  FixedDelayMicroSecond (50);

  //
  //Workaround: Enable DB snooping by writing to F0RCBx
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    if (IsLrdimmPresent (socket, ch, pprAddr[ch].dimm)) {
      OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_EN_DB_SNOOP, ch);
      //Obtain the previously programmed value and change it
      controlWordData = (*dimmNvList)[pprAddr[ch].dimm].rcxCache[RDIMM_RCBx >> 4];
      controlWordData &= ~BIT3;
      WriteRC (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, controlWordData, RDIMM_RCBx);
    }
  }

  //
  // POST PPR sequence
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((~chBitMask) & (1 << ch)) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    rankStruct = GetRankStruct (Host, socket, ch, pprAddr[ch].dimm);

    //
    // Since Guard key was enabled, restore MR0. It could have been overwritten if PPR sequence had error
    //
    OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_SEQ_RESTORE_MR0, ch);
    WriteMRS (Host, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, (*rankStruct)[pprAddr[ch].rank].MR0, BANK0);
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                               "ExecutePPR - Restore MR0 post PPR\n");

    //
    // Re-enable page close timer for short period to guarantee that host scheduler closes pages to match DRAM idle state
    // Otherwise if page close timer is disabled and scheduler state is tracking an open page from previous PPR WR command,
    // then subsequent steps could miss the ACT command.
    //
    SetPageTableTimer (Host, socket, ch, PageTimerEnable);
    FixedDelayMicroSecond (1);
    if (PageTableTimerState[ch] == PageTimerEnable) {
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                           "ExecutePPR - Re-enable Page Timer\n");
    } else {
      SetPageTableTimer (Host, socket, ch, PageTimerDisable);
    }
  } // ch loop


  //
  // Test row after repair
  //
  OutputCheckpoint (CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_PPR_POST_TEST, 0);

  if (RowTestPPR (Host, socket, chBitMask, pprAddr, DramMask, bwSerr)) {
    //
    // Row test failed, figure out which device failed.
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((~chBitMask) & (1 << ch)) {
        continue;
      }
      failDevMask = GetFailDevice (Host, socket, ch, pprAddr[ch].dimm, bwSerr[ch]);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, pprAddr[ch].dimm, NO_SUBCH, pprAddr[ch].rank, NO_STROBE, NO_BIT,
          "ExecutePPR - Post-PPR Row test (RowTestPPR) failed - dramMask = 0x%x\n", failDevMask);
      if (failDevMask) {
        pprStatus[ch] = PPR_STS_FAILED;
      }
    } // ch loop
  }

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "ExecutePPR - END\n");
#endif // !DDR5_SUPPORT
  return status;

} // ExecutePPR

/**

  Get next address from list for PPR operation

  @param Host     - Pointer to sysHost
  @param socket   - socket id
  @param ch       - DDR CH
  @param currEntry - current index in the list
  @param pprAddr  - buffer to hold DRAM Address that need to be repaired
  @param chMask   - buffer to hold the ch bitmap (0 - no PPR for this CH, 1 - run PPR on this CH)

  @retval nextEntry - index of next entry in the list

**/
UINT8
GetNextPprAddrForSktCh (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     currEntry,
  UINT32    NumEntries,
  PPR_ADDR_MRC_SETUP *PprAddrSetup,
  UINT32    *chMask,
  PPR_ADDR  *pprAddr
  )
{
  UINT8   nextEntry = 0xFF;
  UINT8   mcId;
  UINT8   mcCh;

  if ((*chMask) & (1 << ch)) {
    mcId = GetMCID (Host, socket, ch);
    mcCh = GetMCCh (mcId, ch);
    for (; currEntry < NumEntries; currEntry++) {
      if (((PprAddrSetup[currEntry].pprAddrStatus & PPR_STS_ADDR_VALID)) && (mcId == PprAddrSetup[currEntry].mc) &&
          (socket == PprAddrSetup[currEntry].socket) && (mcCh == PprAddrSetup[currEntry].ch)) {
        nextEntry = currEntry + 1;
        *pprAddr = PprAddrSetup[currEntry].pprAddr;
        break;
      }
    } // for ppr entry
  }
  if (nextEntry == 0xFF) {
    (*chMask) &= ~(1 << ch);
  }

  return nextEntry;
}

/**

  Main routine that runs Post Pckage Repair for all valid entries in error address list

  @param Host     - Pointer to sysHost

  @retval status - SUCCESS / failure

**/
UINT32
PostPackageRepairMain (
  PSYSHOST           Host
  )
{
  UINT8         socket;
  UINT32        chBitMask = 0;
  UINT32        status = SUCCESS;
  UINT32        i;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  socket = Host->var.mem.currentSocket;

  //Host->var.mem.notRunningFromCT; the Default will be 0, set it to 1 when calling with chBitMask
  if (Host->var.mem.notRunningFromCT == 0) {
    chBitMask = CH_BITMASK;
    //
    // Copy setup option list to variable list that is used during JEDEC init sequence
    // The variable list has additional entries to accomodate Advance Memtest additions
    //
    if (Host->var.mem.NumValidPprEntries == 0) {
      for (i = 0; i < MAX_PPR_ADDR_ENTRIES; i++) {
        if (Setup->mem.pprAddrSetup[i].pprAddrStatus == PPR_STS_ADDR_VALID) {
          Host->var.mem.pprAddrSetup[Host->var.mem.NumValidPprEntries++] = Setup->mem.pprAddrSetup[i];
        }
      }
    }
  } else {
    chBitMask = Host->var.mem.chBitMask; //Use the 'chBitMask' value set in JedecInitSequence (For a SSA or non SSA I/P)
  }

  if(!IsPprDataConsumed (socket) || (Setup->mem.pprType == PPR_TYPE_SOFT)) {
    RcDebugPrint (SDBG_MAX, "Calling PostPackageRepair\n");
    status = PostPackageRepair (Host, socket, chBitMask, Setup->mem.pprType,
      &Host->var.mem.softPprDone[socket], Host->var.mem.NumValidPprEntries,
      (PPR_ADDR_MRC_SETUP *) &Host->var.mem.pprAddrSetup,
      (UINT8 *) &Host->var.mem.pprStatus);
    RcDebugPrint (SDBG_MAX, "Setting PPR Data Consumed\n");
    SetPprDataConsumed (socket);
  }
  return status;

} //PostPackageRepairMain

/**

  Routine that runs Post Package Repair for all valid entries in error address list

  @param[in]          Host            Pointer to SYSHOST
  @param[in]          Socket          Socket number
  @param[in]          ChBitMask       Bit mask of enabled channels
  @param[in]          PprTypeIn       Hard or Soft PPR type
  @param[in, out]     *SoftPprDone    Global status of Soft PPR repair done
  @param[in]          NumEntries      Number of entries in PprAddrSetup and PprStatusOut arrays
  @param[in]          PprAddrSetup    Array of DRAM addresses to repair
  @param[out]         *PprStatusOut   Array of repair status for each address in PprAddrSetup array

  @retval status   - SUCCESS / FAILURE

**/
UINT32
PostPackageRepair (
  IN      PSYSHOST Host,
  IN      UINT8 socket,
  IN      UINT32 chBitMask,
  IN      UINT8 PprTypeIn,
  IN OUT  UINT8 *SoftPprDone,
  IN      UINT32 NumEntries,
  IN      PPR_ADDR_MRC_SETUP *PprAddrSetup,
  OUT     UINT8 *PprStatusOut
  )
{
  UINT32    status = SUCCESS;
  PPR_ADDR  pprAddr[MAX_CH];
  UINT32    chMask;
  UINT32    tempChMask;
  PPR_TYPE  pprType;
  UINT8     dimm;
  UINT8     ch;
  UINT8     pprEntry;
  UINT8     pprCurrEntry[MAX_CH];
  UINT32    pprStatus[MAX_CH];
  struct channelNvram   (*channelNvList)[MAX_CH];
  struct dimmNvram      (*dimmNvList)[MAX_DIMM];
  UINT8                 MaxChDdr;


  if ((PprTypeIn == PPR_DISABLED) || (Host->nvram.mem.dramType != SPD_TYPE_DDR4) ||
      (GetSysBootMode () == S3Resume) || (Host->var.mem.subBootMode == WarmBootFast)) {
    return status;
  }

  if (PprTypeIn == PPR_TYPE_HARD) {
    pprType = PprTypeHard;
  } else {
    pprType = PprTypeSoft;
  }

  //
  // Display PPR capabilities
  //
  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  //Check if this channel has been masked off as each BSSA Call is per MC
  ZeroMem (pprCurrEntry, sizeof (pprCurrEntry));

  if (*SoftPprDone == 0) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((chBitMask & (1 << ch)) == 0) {
        continue;
      }
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "SPD Other Opt features [byte 9]: 0x%02X\n", (*dimmNvList)[dimm].SPDOtherOptFeatures);

        if (((*dimmNvList)[dimm].SPDOtherOptFeatures & SPD_PPR_MASK) == SPD_PPR_HARD_1R) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Hard PPR Supported: Yes\n");
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Hard PPR Supported: No\n");
        }

        if ((*dimmNvList)[dimm].SPDOtherOptFeatures & SPD_SOFT_PPR) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Soft PPR Supported: Yes\n");
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Soft PPR Supported: No\n");
        }
      }
    }
  }

  chMask = (1 << MAX_CH) - 1;
  for (pprEntry = 0; pprEntry < NumEntries; pprEntry++) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((chBitMask & (1 << ch)) == 0) {
        chMask &= ~(1 << ch);
        continue;
      }
      if ((*channelNvList)[ch].enabled == 0) {
        chMask &= ~(1 << ch);
        continue;
      }
      if ((~chMask) & (1 << ch)) {
        continue;
      }

      pprStatus[ch] = 0;
      pprCurrEntry[ch] = GetNextPprAddrForSktCh (Host, socket, ch, pprCurrEntry[ch], NumEntries, PprAddrSetup, &chMask, &pprAddr[ch]);
      if (chMask & (1 << ch)) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, pprAddr[ch].dimm, NO_SUBCH, pprAddr[ch].rank, NO_STROBE, NO_BIT,
            "PostPackageRepair - Row: 0x%x, Bank: %d, CID: %d, NibbleMask: 0x%x\n", pprAddr[ch].row, pprAddr[ch].bank, pprAddr[ch].subRank, pprAddr[ch].nibbleMask);
      }
    } // ch loop

    if (chMask == 0) {
      break;
    }

    //
    // Check if specified DIMMs support PPR type requested and adjust chBitMask
    //
    tempChMask = chMask;
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((chBitMask & (1 << ch)) == 0) {
        continue;
      } //Exit if CH not in mask
      if ((~tempChMask) & (1 << ch)) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);
      if (((((*dimmNvList)[pprAddr[ch].dimm].SPDOtherOptFeatures & SPD_PPR_MASK) != SPD_PPR_HARD_1R) && (pprType == PprTypeHard)) ||
          ((!((*dimmNvList)[pprAddr[ch].dimm].SPDOtherOptFeatures & SPD_SOFT_PPR)) && (pprType == PprTypeSoft))) {
        tempChMask &= ~(1 << ch);
        RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, pprAddr[ch].dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "PostPackageRepair - DIMM doesn't support requested PPR type; skipping it!\n");
      }
    } // ch loop

    if (tempChMask == 0) {
      continue;
    }

    //
    // Execute the PPR flow on current entry
    //
    ExecutePPR (Host, socket, tempChMask, pprAddr, pprType, pprStatus);

    //
    // Update PPR status
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((chBitMask & (1 << ch)) == 0) {
        continue;
      }
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      if ((~chMask) & (1 << ch)) {
        continue;
      }
      if (pprStatus[ch]) {
        PprStatusOut[pprCurrEntry[ch]-1] = PPR_STS_FAILED;
        EwlOutputType2 (WARN_PPR_FAILED, 0, socket, ch, pprAddr[ch].dimm, pprAddr[ch].rank, EwlSeverityWarning, NO_STROBE, NO_BIT, MrcGtDelim, DdrLevel, 0xFF);
      } else {
        PprStatusOut[pprCurrEntry[ch]-1] = PPR_STS_SUCCESS;

        //
        // If this is soft PPR sequence, update flag
        //
        if (pprType == PprTypeSoft) {
          *SoftPprDone = 1;
        }
      }
    }
  } // for PPR loop

  //
  // Clean-up after PPR
  //
  CpgcPprCleanup (Host, socket, chBitMask);

  return status;
}

/**
  Set SkipPprRowTest Flag to given Value

  @param[in]  FlagValue - Value to Set(TRUE or FALSE)

  @retval N/A
**/
VOID
SetSkipPprRowTestFlag (
  IN BOOLEAN FlagValue
  )
{
  UINT8  Socket = 0;
  PSYSHOST Host = GetSysHostPointer ();

  Socket = Host->var.mem.currentSocket;
  Host->var.mem.SkipPprRowTest[Socket] = FlagValue;
  RcDebugPrint (SDBG_INFO, "Set SkipPprRowTest = %a\n", FlagValue? "TRUE":"FALSE");
}

/**
  Function gets current SkipPprRowTest flag value

  @retval BOOLEAN - Current value of the SkipPprRowTest Flag

**/
BOOLEAN
GetSkipPprRowTestFlag (
  VOID
  )
{
  UINT8  Socket = 0;
  PSYSHOST Host = GetSysHostPointer ();

  Socket = Host->var.mem.currentSocket;

  RcDebugPrint (SDBG_INFO, "Get SkipPprRowTest = %a\n", Host->var.mem.SkipPprRowTest[Socket]? "TRUE":"FALSE");

  return Host->var.mem.SkipPprRowTest[Socket];
}
