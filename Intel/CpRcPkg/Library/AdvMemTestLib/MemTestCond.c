/** @file

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

#include "MemTestFunc.h"
#include <Library/AdvMemTestLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/MemRcLib.h>

#define   AMT_TC_TWR_NO_CHANGE  0xFF

//
// Code section
//

/**

  Set Advanced Memory Test condition

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type
  @param Operation           - AMT_SET_TC or AMT_RESTORE_TC to set or restore the test condition
  @param *PrevAmtTestCond    - The previous test condition before AMT_SET_TC

  @retval status             - SUCCESS/FAILURE

**/
UINT32
SetAdvMemTestCondition (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   ddrChEnabled,
  UINT8    TestType,
  UINT8    Operation,
  AMT_TEST_CONDITION *PrevAmtTestCond
)
{
  UINT8  ch;
  UINT8  imc;
  UINT32 status = SUCCESS;
  UINT16 temp;
#ifndef DDR5_SUPPORT
  UINT8  dimm;
  UINT8  rank;
  UINT16 Mr0Mask;
  UINT16 Mr0Value;
  struct rankDevice (*rankStruct)[MAX_RANK_DIMM];
#endif // !DDR5_SUPPORT
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  AMT_TEST_CONDITION AmtTestCond;
  AMT_TEST_CONDITION CurAmtTestCond;
  UINT8 ApplyChanges = 0;
  UINT32 PrevDebugLevel;
  BOOLEAN  PrevOpenPageMode;
  UINT32   PrevScrambleEnConfig;
  UINT8    MaxImc;


  SYS_SETUP *Setup;
  UINT8     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();
  MaxImc = GetMaxImc ();

  RcDebugPrint (SDBG_MAX,
    "\nSetAdvMemTestCondition Starts\n");

  if (Host->nvram.mem.dramType != SPD_TYPE_DDR4) {
    return status;
  }

  SetMem ((UINT8 *)&AmtTestCond, sizeof (AmtTestCond), 0);

  //
  // Initialize Page Mode and Scrambler config based on TestType
  //
  switch (TestType) {
    case ADV_MT_TYPE_TWR:
    case ADV_MT_TYPE_MATS8_TC1:
      AmtTestCond.OpenPageMode = TRUE;                                  // Open Page mode
      AmtTestCond.ScrambleEnConfig = 1;                                 // DDR scrambler enabled
      break;
    case ADV_MT_TYPE_MATS8_TC3:
      AmtTestCond.OpenPageMode = FALSE;                                 // Closed Page mode
      AmtTestCond.ScrambleEnConfig = 1;                                 // DDR scrambler enabled
      break;
    default:
      AmtTestCond.OpenPageMode = TRUE;                                  // Open Page mode
      AmtTestCond.ScrambleEnConfig = 0;                                 // DDR scrambler disabled
      break;
  }

  //
  // Evaluate setup option for Test Conditions
  //
  if ((Setup->mem.AdvMemTestCondition == ADV_MEM_TEST_COND_MANUAL) && (TestType != ADV_MT_TYPE_PPR)) {

    ApplyChanges = 1;
    AmtTestCond.VddLevel = Setup->mem.AdvMemTestCondVdd;                  // mV
    AmtTestCond.TwrValue = Setup->mem.AdvMemTestCondTwr;                  // tCK
    AmtTestCond.Mr0TwrValue = Setup->mem.AdvMemTestCondTwr;               // tCK
    AmtTestCond.TrefiValue = Setup->mem.AdvMemTestCondTrefi;              // nsec

  } else if ((Setup->mem.AdvMemTestCondition == ADV_MEM_TEST_COND_AUTO) || (TestType == ADV_MT_TYPE_PPR)) {

    //
    // Initialize Test Condition based on TestType
    //
    ApplyChanges = 1;
    switch (TestType) {
      case ADV_MT_TYPE_TWR:
        AmtTestCond.VddLevel = 1200;                                      // 1.2V nominal
        AmtTestCond.TwrValue = 10;                                        // Host tWR = 10 cycles
        AmtTestCond.Mr0TwrValue = 10;                                     // MR0 tWR = 10 cycles
        AmtTestCond.TrefiValue = 7800;                                    // 7.8 usec
        break;
      case ADV_MT_TYPE_DATA_RET:
        AmtTestCond.VddLevel = 1200;                                      // 1.2V nominal
        AmtTestCond.TwrValue = AMT_TC_TWR_NO_CHANGE;                      // No change to Host tWR
        AmtTestCond.Mr0TwrValue = AMT_TC_TWR_NO_CHANGE;                   // No change to MR0 tWR
        AmtTestCond.TrefiValue = 15600;                                   // 15.6 usec
        break;
      case ADV_MT_TYPE_MATS8_TC1:
      case ADV_MT_TYPE_MATS8_TC2:
      case ADV_MT_TYPE_MATS8_TC3:
        AmtTestCond.VddLevel = 1200;                                      // 1.2V nominal
        AmtTestCond.TwrValue = AMT_TC_TWR_NO_CHANGE;                      // No change to Host tWR
        AmtTestCond.Mr0TwrValue = AMT_TC_TWR_NO_CHANGE;                   // No change to MR0 tWR
        AmtTestCond.TrefiValue = 7800;                                    // 7.8 usec
        break;
      case ADV_MT_TYPE_PPR:
        AmtTestCond.VddLevel = 1200;                                      // 1.2V nominal
        AmtTestCond.TwrValue = AMT_TC_TWR_NO_CHANGE;                      // No change to Host tWR
        AmtTestCond.Mr0TwrValue = AMT_TC_TWR_NO_CHANGE;                   // No change to MR0 tWR
        AmtTestCond.TrefiValue = 0;                                       // Disable refresh during PPR
        break;
      default:
        AmtTestCond.VddLevel = 1200;                                      // 1.2V nominal
        AmtTestCond.TwrValue = AMT_TC_TWR_NO_CHANGE;                      // No change to Host tWR
        AmtTestCond.Mr0TwrValue = AMT_TC_TWR_NO_CHANGE;                   // No change to MR0 tWR
        AmtTestCond.TrefiValue = 7800;                                    // 7.8 usec
        ApplyChanges = 0;
        break;
    }
  }

  //
  // Restore test conditions
  //
  if (Operation == AMT_RESTORE_TC) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Restore Test Condition\n");
    CopyMem ((UINT8 *)&AmtTestCond, (UINT8 *)PrevAmtTestCond, sizeof (AMT_TEST_CONDITION));
  }

  //
  // Save current MR0 test condition before JEDEC init
  //
  else {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Set Test Condition\n");
    //
    // Convert tREFI input units from nsec to tCK units, maintaining precision and preventing overflow
    //
    temp = AmtTestCond.TrefiValue;
    temp = (UINT16)((temp * 10000) / GettCK (Host, socket));
    AmtTestCond.TrefiValue = temp;

#ifndef DDR5_SUPPORT
    for (ch = 0; ch < MaxChDdr; ch++) {
      if (ddrChEnabled & (1 << ch)) {
        dimmNvList = GetDimmNvList (Host, socket, ch);
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }
          if ((*dimmNvList)[dimm].DcpmmPresent) {
            continue;
          }
          rankStruct = GetRankStruct (Host, socket, ch, dimm);
          for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
            temp = (*rankStruct)[rank].MR0;
            DecodeMr0TrtpTwrDdr4 (TRUE, temp, &CurAmtTestCond.Mr0TwrValue);
          } // rank
        } // dimm
      } // ddrChEnabled
    } //ch
#endif // !DDR5_SUPPORT
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (ddrChEnabled & (1 << ch)) {
      //
      // Enable page close timer for short period to guarantee that all pages are closed
      //
      if (GetPageTableTimerState (Host, socket, ch) == PageTimerDisable) {
        SetPageTableTimer (Host, socket, ch, PageTimerEnable);
        FixedDelayMicroSecond (1);
        SetPageTableTimer (Host, socket, ch, PageTimerDisable);
      }
      //
      // Set scrambler disabled
      //
      GetSetDdrScramblerEn (Host, socket, ch, 0, &CurAmtTestCond.ScrambleEnConfig);
    }
  }
  //
  // Set Open Page mode enabled
  //
  for (imc = 0; imc < MaxImc; imc++) {
    if (Host->var.mem.socket[socket].imcEnabled[imc]) {
      BiosSetPageOpenConfigChip (Host, socket, imc, TRUE, &CurAmtTestCond.OpenPageMode);
    }
  }

  //
  // Apply the test condition changes
  //
  if (ApplyChanges) {

    //
    // Disable refresh commands
    //
    SetRefresh (Host, socket, (UINT8)-1);
    FixedDelayMicroSecond (10);

    //
    // Adjust Vdd
    //
    if (AmtTestCond.VddLevel && (AmtTestCond.VddLevel >= ADV_MEM_TEST_COND_VDD_MIN) && (AmtTestCond.VddLevel <= ADV_MEM_TEST_COND_VDD_MAX)) {

      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Set VDD = %d mV\n", AmtTestCond.VddLevel);

      PrevDebugLevel = GetDebugLevel();
      SetDebugLevel (0);
      //
      // Margin Vdd level
      //
      if (Setup->mem.inputMemTime.vdd) {
        CurAmtTestCond.VddLevel = Setup->mem.inputMemTime.vdd;
      } else {
        CurAmtTestCond.VddLevel = 1200; // 1200 mV
      }
      //
      // Margin Vdd - TODO: clean up SetVdd interface to not use Setup policy directly
      //
      Setup->mem.inputMemTime.vdd = AmtTestCond.VddLevel;
      SetVdd (Host, socket);
      //
      // Jedec Init after changing Vdd
      //
      JedecInitSequence (Host, socket, CH_BITMASK);
      SetDebugLevel (PrevDebugLevel);
    }

    for (ch = 0; ch < MaxChDdr; ch++) {
      if (ddrChEnabled & (1 << ch)) {

        //
        // Set tREFI
        //
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Set tREFI = 0x%x tCK\n", AmtTestCond.TrefiValue);

        GetSetTrefi (Host, socket, ch, AmtTestCond.TrefiValue, &CurAmtTestCond.TrefiValue);

        //
        // Set host tWR (only used in open page mode)
        //
        if (AmtTestCond.TwrValue == AMT_TC_TWR_NO_CHANGE) {
          AmtTestCond.TwrValue = (UINT8) GettWR (Host, socket, ch);
        }
        temp = AmtTestCond.TwrValue;
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Set Host tWR = %d tCK\n", temp);
        SetChTwr (Host, socket, ch, (UINT8) temp, &CurAmtTestCond.TwrValue);

        //
        // Loop for each DIMM
        //
        dimmNvList = GetDimmNvList (Host, socket, ch);
#ifndef DDR5_SUPPORT
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }
          if ((*dimmNvList)[dimm].DcpmmPresent) {
            continue;
          }

          rankStruct = GetRankStruct (Host, socket, ch, dimm);
          for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
            //
            // Set Write Recovery time
            //
            temp = (*rankStruct)[rank].MR0;
            if (AmtTestCond.Mr0TwrValue == AMT_TC_TWR_NO_CHANGE) {
              DecodeMr0TrtpTwrDdr4 (TRUE, temp, &AmtTestCond.Mr0TwrValue);
            }
            EncodeMr0TrtpTwrDdr4 (AmtTestCond.Mr0TwrValue, TRUE, &Mr0Mask, &Mr0Value);
            temp &= ~Mr0Mask;
            temp |= Mr0Value;

            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
              "Set MR0 tWR = %d tCK, MR0 = 0x%04x\n", AmtTestCond.Mr0TwrValue, temp);
            WriteMRS (Host, socket, ch, dimm, rank, temp & ~(MR0_DLL), BANK0);
            (*rankStruct)[rank].MR0 = temp;

          } // rank
        } // dimm
#endif // !DDR5_SUPPORT
      } // ddrChEnabled
    } //ch

    //
    // Enable refresh commands if tREFI > 0
    //
    if (AmtTestCond.TrefiValue) {
      SetRefresh (Host, socket, 0);
    }
  } // ApplyChanges

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (ddrChEnabled & (1 << ch)) {
      //
      // Enable page close timer for short period to guarantee that all pages are closed
      //
      if (GetPageTableTimerState (Host, socket, ch) == PageTimerDisable) {
        SetPageTableTimer (Host, socket, ch, PageTimerEnable);
        FixedDelayMicroSecond (1);
        SetPageTableTimer (Host, socket, ch, PageTimerDisable);
      }
      //
      // Set scrambler config
      //
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Set DDR Scrambler = %d\n", AmtTestCond.ScrambleEnConfig);
      GetSetDdrScramblerEn (Host, socket, ch, AmtTestCond.ScrambleEnConfig, &PrevScrambleEnConfig);
    }
  }
  //
  // Set Open or Closed Page mode
  //
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Set Open Page Mode = %d\n", AmtTestCond.OpenPageMode);
  for (imc = 0; imc < MaxImc; imc++) {
    if (Host->var.mem.socket[socket].imcEnabled[imc]) {
      BiosSetPageOpenConfigChip (Host, socket, imc, AmtTestCond.OpenPageMode, &PrevOpenPageMode);
    }
  }
  //
  // Return previous test conditions on set operation
  //
  if (Operation == AMT_SET_TC) {
    CopyMem ((UINT8 *)PrevAmtTestCond, (UINT8 *)&CurAmtTestCond, sizeof (AMT_TEST_CONDITION));
  }

  RcDebugPrint (SDBG_MAX, "SetAdvMemTestCondition Ends\n");

  return status;
} //SetAdvMemTestCondition


/**

  Pause condition for Advanced Memory Test

  @param Host                - Pointer to sysHost
  @param socket              - Socket Id
  @param ddrChEnabled        - List of channels to be tested
  @param testType            - Memory test type

  @retval status             - SUCCESS/FAILURE

**/
UINT32
AdvMemTestPauseCondition (
  PSYSHOST Host,
  UINT8    Socket,
  UINT32   DdrChEnabled,
  UINT8    TestType
)
{
  UINT8  ApplyChanges = 0;
  UINT32 DelayTimeUs;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  //
  // Evaluate setup option for test conditions
  //
  if (Setup->mem.AdvMemTestCondition == ADV_MEM_TEST_COND_MANUAL) {

    ApplyChanges = 1;
    DelayTimeUs = Setup->mem.AdvMemTestCondPause;

  } else if (Setup->mem.AdvMemTestCondition == ADV_MEM_TEST_COND_AUTO) {

    ApplyChanges = 1;
    switch (TestType) {
    case ADV_MT_TYPE_DATA_RET:
      DelayTimeUs = 128000;         // Delay 128 msec without refresh
      break;
    case ADV_MT_TYPE_MATS8_TC1:
    case ADV_MT_TYPE_MATS8_TC2:
    case ADV_MT_TYPE_MATS8_TC3:
      DelayTimeUs = 0;              // Do not delay
      break;
    default:
      ApplyChanges = 0;
      break;
    }
  } // Setup->mem.AdvMemTestCondition

  if (ApplyChanges && (DelayTimeUs > 0)) {
    //
    // Ensure all pages closed and rows are refreshed
    // 128ms covers worst case of 0.5x refresh rate
    //
    FixedDelayMicroSecond (128000);

    //
    // Disable refresh commands
    //
    SetRefresh (Host, Socket, (UINT8)-1);

    //
    // Pause for given test delay
    //
    FixedDelayMicroSecond (DelayTimeUs);

    //
    // Enable refresh commands
    //
    SetRefresh (Host, Socket, 0);

    //
    // Ensure all rows are refreshed
    // 128ms covers worst case of 0.5x refresh rate
    //
    FixedDelayMicroSecond (128000);

    RcDebugPrint (SDBG_MAX,
      "AdvMemTestPauseCondition = %d ms\n", DelayTimeUs / 1000);
  }

  return SUCCESS;
} // AdvMemTestPauseCondition

