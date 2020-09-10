/** @file
  Provide cached access for CPGC 2 registers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

//
// Include files
//
#include <Library/MemoryCoreLib.h>
#include <Library/MemCpgcIpLib.h>
#include "Include/MemCpgcRegs.h"
#include <Library/SysHostPointerLib.h>
#include "Include/MemCpgcRegCache.h"
#include "Include/CpgcChip.h"
#include <Library/MemRcLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/MemoryServicesLib.h>

//
// Register list is used to load default register value
//
UINT32       RegListCpgc[] = { CPGC2_BASE_REPEATS_MCCPGC_MAIN_REG,
                               CPGC2_BASE_COL_REPEATS_MCCPGC_MAIN_REG,
                               CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG,
                               CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG,
                               CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG,
                               CPGC2_REGION_LOW_N0_MCCPGC_MAIN_REG,
                               CPGC2_REGION_LOW_N1_MCCPGC_MAIN_REG,
                               CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG,
                               CPGC2_ADDRESS_CONTROL_MCCPGC_MAIN_REG,
                               CPGC2_DATA_CONTROL_MCCPGC_MAIN_REG,
                               CPGC2_ADDRESS_INSTRUCTION_0_MCCPGC_MAIN_REG,
                               CPGC2_ADDRESS_INSTRUCTION_1_MCCPGC_MAIN_REG,
                               CPGC2_ADDRESS_INSTRUCTION_2_MCCPGC_MAIN_REG,
                               CPGC2_ADDRESS_INSTRUCTION_3_MCCPGC_MAIN_REG,
                               CPGC_DPAT_CFG_MCCPGC_MAIN_REG,
                               CPGC_SEQ_CFG_B_MCCPGC_MAIN_REG
                             };

UINT32       RegListCadb[] = { CADB_CFG_MCCADB_MAIN_REG,
                               CADB_AO_MRSCFG_MCCADB_MAIN_REG,
                               CADB_MRSCFG_N0_MCCADB_MAIN_REG,
                               CADB_MRSCFG_N1_MCCADB_MAIN_REG,
                               CADB_DLY_MCCADB_MAIN_REG,
                               CADB_SELCFG_MCCADB_MAIN_REG,
                               CADB_DSEL_UNISEQ_LMN_MCCADB_MAIN_REG
                             };

/**
  Check whether or not CPGC register caching is enabled.

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval     TRUE                CPGC register caching is enabled
              FALSE               CPGC register caching is disabled

**/
BOOLEAN
IsCpgcCacheEnabled (
  VOID
  )
{

  CPGC2_HOST_DATA   *CpgcHost;

  CpgcHost = GetCpgcHostPointer ();

  if (CpgcHost == NULL) {
    InitCpgcHostData ();
    CpgcHost = GetCpgcHostPointer ();
  }

  return CpgcHost->CpgcIn.IsCacheEnable;
}

/**
  Setup CPGC 2 for Standard Write/Read Loopback test pattern

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence
  @param[in]  IsAddrUp            Set address traverse direction. TRUE - up direction (incremental), FALSE - down direction (decremental)
  @param[in]  IsDataInvt          Set data inversion. TRUE - data inversion, FALSE - data no inversion

  @retval N/A

**/
VOID
SetCpgcCmdPatRd (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait,
  IN BOOLEAN        IsAddrUp,
  IN BOOLEAN        IsDataInvt
  )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl0;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction0;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction0;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;

  //
  // 1 data instruction
  //
  DataInstruction0.Data = 0;
  DataInstruction0.Bits.last = 1;
  DataInstruction0.Bits.background_mode = 1; // Chunk inversion based on data_background
  DataInstruction0.Bits.data_background = 0; // No chunk inversion
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction0.Data);

  //
  // 1 algorithm instructions
  // (read)
  //
  AlgorithmInstruction0.Data = 0;
  AlgorithmInstruction0.Bits.command_start_pointer = 0; // Point to read command instruction
  if (IsAddrUp) {
    AlgorithmInstruction0.Bits.inverse_direction = 0; // address up, incremental
  } else {
    AlgorithmInstruction0.Bits.inverse_direction = 1; // address down, decremental
  }
  if (IsDataInvt) {
    AlgorithmInstruction0.Bits.invert_data = 1; // data inversion
  } else {
    AlgorithmInstruction0.Bits.invert_data = 0; // data no inversion
  }
  AlgorithmInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction0.Data);

  //
  // set wait time between each algorithms
  //
  AlgorithmInstructionCtrl0.Data = 0;
  AlgorithmInstructionCtrl0.Bits.wait_event_start = 0; // Do not wait for an event for algo to start
  AlgorithmInstructionCtrl0.Bits.wait_count_start = 1; // Wait for timer before starting (introduces bubble between bursts)
  AlgorithmInstructionCtrl0.Bits.wait_count_clear = 1; // Reset the wait timer at the start of this algo instruction
  AlgorithmInstructionCtrl0.Bits.be_train_err_en = 0; // Don't do byte enable checking
  AlgorithmInstructionCtrl0.Bits.select_on = 1; // Enable CADB to replace CMD with CADB
  AlgorithmInstructionCtrl0.Bits.deselect_on = 0; // Do not enable CADB to drive deselect traffic for this algo
  AlgorithmInstructionCtrl0.Bits.base_range_row = 0; // Use base_repeats and not base_col_repeats
  AlgorithmInstructionCtrl0.Bits.fasty_init = 0; // Do not override address_direction setting
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);

  AlgorithmWaitEventControl.Data = 0;
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0

  //
  // Disable the event wait timer.
  //
  AlgorithmWaitEventControl.Bits.timer = 0x4;
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = 3; // Native CPGC clock
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait;//64
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);

  //
  // 1 command instructions
  // (read)
  //
  CommandInstruction0.Data = 0;
  CommandInstruction0.Bits.writecmd = 0; // read
  CommandInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction0.Data);
}

/**
  Setup CPGC 2 for read/write test pattern. It's currently only used by memory test.
  If Is2ndCmdDataInvt = 1
                         IsGlobalAddrUp = 0     IsGlobalAddrUp = 1
  IsGlobalDataInvt = 0      v(rD, wI)            ^(rD, wI)
  IsGlobalDataInvt = 1      v(rI, wD)            ^(rI, wD)

  If Is2ndCmdDataInvt = 0
                         IsGlobalAddrUp = 0     IsGlobalAddrUp = 1
  IsGlobalDataInvt = 0      v(rD, wD)            ^(rD, wD)
  IsGlobalDataInvt = 1      v(rI, wI)            ^(rI, wI)

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence
  @param[in]  IsGlobalAddrUp      Set address traverse direction. TRUE - up direction (incremental), FALSE - down direction (decremental)
  @param[in]  IsGlobalDataInvt    Set data inversion. TRUE - data inversion, FALSE - data no inversion
  @param[in]  Is2ndCmdDataInvt    Set the second command instruction's data inversion. TRUE - data inversion, FALSE - data no inversion

  @retval N/A

**/
VOID
SetCpgcCmdPatRdWr (
    IN PSYSHOST       Host,
    IN UINT8          Socket,
    IN UINT8          Ch,
    IN UINT8          SubCh,
    IN UINT16         SubSeqWait,
    IN BOOLEAN        IsGlobalAddrUp,
    IN BOOLEAN        IsGlobalDataInvt,
    IN BOOLEAN        Is2ndCmdDataInvt
    )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;
  UINT32                                                 Data32;

  //
  // 1 algorithm instructions
  // (rD, wI)
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG));
  AlgorithmInstruction.Data = Data32 & 0xFF;
  AlgorithmInstruction.Bits.command_start_pointer = 0; // Point to the first command instruction - read command instruction
  AlgorithmInstruction.Bits.last = 1;
  //
  // Global setting which will apply all commands instructions within this algorithm, i.e. (rD, wI)
  // If Is2ndCmdDataInvt = 1
  //                        IsGlobalAddrUp = 0     IsGlobalAddrUp = 1
  // IsGlobalDataInvt = 0      v(rD, wI)            ^(rD, wI)
  // IsGlobalDataInvt = 1      v(rI, wD)            ^(rI, wD)
  //
  // If Is2ndCmdDataInvt = 0
  //                        IsGlobalAddrUp = 0     IsGlobalAddrUp = 1
  // IsGlobalDataInvt = 0      v(rD, wD)            ^(rD, wD)
  // IsGlobalDataInvt = 1      v(rI, wI)            ^(rI, wI)
  //
  if (IsGlobalAddrUp) {
    AlgorithmInstruction.Bits.inverse_direction = 0; // address up, incremental
  } else {
    AlgorithmInstruction.Bits.inverse_direction = 1; // address down, decremental
  }
  if (IsGlobalDataInvt) {
    AlgorithmInstruction.Bits.invert_data = 1; // data inversion
  } else {
    AlgorithmInstruction.Bits.invert_data = 0; // data no inversion
  }
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction.Data);

  //
  // set wait time between each algorithms
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG));
  AlgorithmInstructionCtrl.Data = Data32 & 0xFF;
  AlgorithmInstructionCtrl.Bits.wait_event_start = 0; // Do not wait for an event for algo to start
  AlgorithmInstructionCtrl.Bits.wait_count_start = 0; // Wait for timer before starting (introduces bubble between bursts)
  AlgorithmInstructionCtrl.Bits.wait_count_clear = 1;//1, Reset the wait timer at the start of this algo instruction
  AlgorithmInstructionCtrl.Bits.be_train_err_en = 0; // Don't do byte enable checking
  AlgorithmInstructionCtrl.Bits.select_on = 0; // Disable CADB to replace CMD with CADB
  AlgorithmInstructionCtrl.Bits.deselect_on = 0; // Do not enable CADB to drive deselect traffic for this algo
  AlgorithmInstructionCtrl.Bits.base_range_row = 0;//0, Use base_repeats and not base_col_repeats. Control base repeats. 1 controls the base cl repeats for nop and 1 nop.
  AlgorithmInstructionCtrl.Bits.fasty_init = 0; // Do not override address_direction setting
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl.Data);

  //
  // 2 command instructions
  // read
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG));
  CommandInstruction.Data = Data32 & 0xFF;
  CommandInstruction.Bits.writecmd = 0; // read
  CommandInstruction.Bits.alternate_data = 0;
  CommandInstruction.Bits.invert_data = 0; // rD, no data inversion in this command instruction
  CommandInstruction.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction.Data);

  //
  // write
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG));
  CommandInstruction.Data = Data32 & 0xFF;
  CommandInstruction.Bits.writecmd = 1; // write
  CommandInstruction.Bits.last = 1;
  CommandInstruction.Bits.alternate_data = 0;
  if (Is2ndCmdDataInvt) {
    CommandInstruction.Bits.invert_data = 1; // wI, data inversion in this command instruction
  } else {
    CommandInstruction.Bits.invert_data = 0; // wD, no data inversion in this command instruction
  }
  CommandInstruction.Bits.last = 1; // algorithm end
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG), CommandInstruction.Data);

  AlgorithmWaitEventControl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG));
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0
  AlgorithmWaitEventControl.Bits.timer = 0x4; // 2^timer
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = NATIVE_CPGC_CLOCK; // Native CPGC clock
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait; // value is 64
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);

  //
  // Do nothing here.
  //
  DataInstruction.Data = 0;
  DataInstruction.Bits.last = 1;
  DataInstruction.Bits.background_mode = 1; // Chunk inversion based on data_background
  DataInstruction.Bits.data_background = 0; // No chunk inversion
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction.Data);
}

/**
  Setup CPGC 2 for DQDQS test pattern

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatRdDqs (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait
  )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl0;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction0;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction0;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;
  CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_STRUCT       AlgorithmInstrcution1;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_STRUCT  AlgorithmInstrcutionCtrl1;
  CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_STRUCT        CommandInstruction23;
  UINT32                                                 Data32;

  //
  // algorithm instructions
  // (read)
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG));
  AlgorithmInstruction0.Data = Data32 & 0xFF;
  AlgorithmInstruction0.Bits.command_start_pointer = 0; // Point to read command instruction
  AlgorithmInstruction0.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction0.Data);
  //
  // set wait time between each algorithms
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG));
  AlgorithmInstructionCtrl0.Data = Data32 & 0xFF;
  AlgorithmInstructionCtrl0.Bits.wait_event_start = 0; // Do not wait for an event for algo to start
  AlgorithmInstructionCtrl0.Bits.wait_count_start = 0; // Wait for timer before starting (introduces bubble between bursts)
  AlgorithmInstructionCtrl0.Bits.wait_count_clear = 1;//1; // Reset the wait timer at the start of this algo instruction
  AlgorithmInstructionCtrl0.Bits.be_train_err_en = 0; // Don't do byte enable checking
  AlgorithmInstructionCtrl0.Bits.select_on = 1; // Enable CADB to replace CMD with CADB
  AlgorithmInstructionCtrl0.Bits.deselect_on = 0; // Do not enable CADB to drive deselect traffic for this algo
  AlgorithmInstructionCtrl0.Bits.base_range_row = 0;//1;//0; // Use base_repeats and not base_col_repeats. Control base repeats. 1 controls the base cl repeats for nop and 1 nop.
  AlgorithmInstructionCtrl0.Bits.fasty_init = 0; // Do not override address_direction setting
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);

  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_REG));
  AlgorithmInstrcution1.Data = Data32 & 0xFF;
  AlgorithmInstrcution1.Bits.command_start_pointer = 23; // Point to read command instruction
  AlgorithmInstrcution1.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_REG), AlgorithmInstrcution1.Data);
  //
  // set wait time between each algorithms
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_REG));
  AlgorithmInstrcutionCtrl1.Data = Data32 & 0xFF;
  AlgorithmInstrcutionCtrl1.Bits.wait_event_start = 0; // Do not wait for an event for algo to start
  AlgorithmInstrcutionCtrl1.Bits.wait_count_start = 1; // Wait for timer before starting (introduces bubble between bursts)
  AlgorithmInstrcutionCtrl1.Bits.wait_count_clear = 0;//1; // Reset the wait timer at the start of this algo instruction
  AlgorithmInstrcutionCtrl1.Bits.be_train_err_en = 0; // Don't do byte enable checking
  AlgorithmInstrcutionCtrl1.Bits.select_on = 1; // Enable CADB to replace CMD with CADB
  AlgorithmInstrcutionCtrl1.Bits.deselect_on = 0; // Do not enable CADB to drive deselect traffic for this algo
  AlgorithmInstrcutionCtrl1.Bits.base_range_row = 1;//0, Use base_repeats and not base_col_repeats
  AlgorithmInstrcutionCtrl1.Bits.fasty_init = 0; // Do not override address_direction setting
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_REG), AlgorithmInstrcutionCtrl1.Data);
  //
  // 1 command instructions
  // (read)
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG));
  CommandInstruction0.Data = Data32 & 0xFF;
  CommandInstruction0.Bits.writecmd = 0; // read
  CommandInstruction0.Bits.last = 1;
  CommandInstruction0.Bits.alternate_data = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction0.Data);

  //
  // command 23
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_REG));
  CommandInstruction23.Data = Data32 & 0xFF;
  CommandInstruction23.Bits.last = 1;
  CommandInstruction23.Bits.alternate_data = 1;
  CommandInstruction23.Bits.offset = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_REG), CommandInstruction23.Data);


  AlgorithmWaitEventControl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG));
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0
  //
  // Disable the event wait timer.
  //
  AlgorithmWaitEventControl.Bits.timer = 0x4; // 2^timer
  //
  // Native CPGC Clock
  //
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = NATIVE_CPGC_CLOCK;
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait; // the value is 64;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);

  //
  // Do nothing here.
  //
  DataInstruction0.Data = 0;
  DataInstruction0.Bits.last = 1;
  DataInstruction0.Bits.background_mode = 1; // Chunk inversion based on data_background
  DataInstruction0.Bits.data_background = 0; // No chunk inversion
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction0.Data);
}

/**
  Setup CPGC 2 for MRR command by using alternate data with all zeros

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatRdMrr (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait
  )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl0;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction0;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction0;

  //
  // 1 data instruction
  //
  DataInstruction0.Data = 0;
  DataInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction0.Data);

  //
  // 1 algorithm instructions
  // (read)
  //
  AlgorithmInstruction0.Data = 0;
  AlgorithmInstruction0.Bits.command_start_pointer = 0; // Point to read command instruction
  AlgorithmInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction0.Data);

  //
  // set wait time between each algorithms
  //
  AlgorithmInstructionCtrl0.Data = 0;
  AlgorithmInstructionCtrl0.Bits.select_on = 1; // Enable CADB to replace CMD with CADB
  AlgorithmInstructionCtrl0.Bits.deselect_on = 1; // Enable CADB to drive deselect traffic for this algo
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);


  //
  // 1 command instructions
  // (read)
  //
  CommandInstruction0.Data = 0;
  CommandInstruction0.Bits.writecmd = 0; // read
  CommandInstruction0.Bits.alternate_data = 1; // Use the alternate zero data source instead of the UniSequencers and Pattern_Select
  CommandInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction0.Data);
}

/**
  Setup CPGC 2 for write and read test pattern

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatWrRd (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait
  )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl0;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction0;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction0;
  CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_STRUCT         CommandInstruction1;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;
  CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_STRUCT       AlgorithmInstrcution1;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_STRUCT  AlgorithmInstrcutionCtrl1;
  CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_STRUCT        CommandInstruction23;
  UINT32                                                 Data32;
  //
  // 1 algorithm instructions
  // (read)
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG));
  AlgorithmInstruction0.Data = Data32 & 0xFF;
  AlgorithmInstruction0.Bits.command_start_pointer = 0; // Point to read command instruction
  AlgorithmInstruction0.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction0.Data);

  //
  // set wait time between each algorithms
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG));
  AlgorithmInstructionCtrl0.Data = Data32 & 0xFF;
  AlgorithmInstructionCtrl0.Bits.wait_event_start = 0; // Do not wait for an event for algo to start
  AlgorithmInstructionCtrl0.Bits.wait_count_start = 0; // Wait for timer before starting (introduces bubble between bursts)
  AlgorithmInstructionCtrl0.Bits.wait_count_clear = 1;//1, Reset the wait timer at the start of this algo instruction
  AlgorithmInstructionCtrl0.Bits.be_train_err_en = 0; // Don't do byte enable checking
  AlgorithmInstructionCtrl0.Bits.select_on = 1; // Enable CADB to replace CMD with CADB
  AlgorithmInstructionCtrl0.Bits.deselect_on = 0; // Do not enable CADB to drive deselect traffic for this algo
  AlgorithmInstructionCtrl0.Bits.base_range_row = 0;//0, Use base_repeats and not base_col_repeats. Control base repeats. 1 controls the base cl repeats for nop and 1 nop.
  AlgorithmInstructionCtrl0.Bits.fasty_init = 0; // Do not override address_direction setting
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);

  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_REG));
  AlgorithmInstrcution1.Data = Data32 & 0xFF;
  AlgorithmInstrcution1.Bits.command_start_pointer = 23; // Point to read command instruction
  AlgorithmInstrcution1.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_REG), AlgorithmInstrcution1.Data);

  //
  // set wait time between each algorithms
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_REG));
  AlgorithmInstrcutionCtrl1.Data = Data32 & 0xFF;
  AlgorithmInstrcutionCtrl1.Bits.wait_event_start = 0; // Do not wait for an event for algo to start
  AlgorithmInstrcutionCtrl1.Bits.wait_count_start = 1; // Wait for timer before starting (introduces bubble between bursts)
  AlgorithmInstrcutionCtrl1.Bits.wait_count_clear = 0;//1, Reset the wait timer at the start of this algo instruction
  AlgorithmInstrcutionCtrl1.Bits.be_train_err_en = 0; // Don't do byte enable checking
  AlgorithmInstrcutionCtrl1.Bits.select_on = 1; // Enable CADB to replace CMD with CADB
  AlgorithmInstrcutionCtrl1.Bits.deselect_on = 0; // Do not enable CADB to drive deselect traffic for this algo
  AlgorithmInstrcutionCtrl1.Bits.base_range_row = 1;//0,  Use base_repeats and not base_col_repeats
  AlgorithmInstrcutionCtrl1.Bits.fasty_init = 0; // Do not override address_direction setting
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_REG), AlgorithmInstrcutionCtrl1.Data);
  //
  // 1 command instructions
  // (read)
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG));
  CommandInstruction0.Data = Data32 & 0xFF;
  CommandInstruction0.Bits.writecmd = 1; // write
  CommandInstruction0.Bits.last = 0;
  CommandInstruction0.Bits.alternate_data = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction0.Data);

  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG));
  CommandInstruction1.Data = Data32 & 0xFF;
  CommandInstruction1.Bits.writecmd = 0; // read
  CommandInstruction1.Bits.last = 1;
  CommandInstruction1.Bits.alternate_data = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG), CommandInstruction1.Data);

  //
  // command 23
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_REG));
  CommandInstruction23.Data = Data32 & 0xFF;
  CommandInstruction23.Bits.last = 1;
  CommandInstruction23.Bits.alternate_data = 1;
  CommandInstruction23.Bits.offset = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_REG), CommandInstruction23.Data);


  AlgorithmWaitEventControl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG));
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0
  AlgorithmWaitEventControl.Bits.timer = 0x4; // 2^timer
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = NATIVE_CPGC_CLOCK; // Native CPGC clock
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait; // value is 64
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);

  //
  // Do nothing here.
  //
  DataInstruction0.Data = 0;
  DataInstruction0.Bits.last = 1;
  DataInstruction0.Bits.background_mode = 1; // Chunk inversion based on data_background
  DataInstruction0.Bits.data_background = 0; // No chunk inversion
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction0.Data);
}

/**
  Setup CPGC 2 for DDRT2 write and read test pattern

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatWrRdDdrt2 (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait
  )
{
  UINT32                                                 Data32;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_STRUCT  AlgorithmInstrcutionCtrl1;

  SetCpgcCmdPatWrRd (Host, Socket, Ch, SubCh, SubSeqWait); // DDRT2 SubSeqWait = 512
  //
  // Reset the wait timer at the start of this algo instruction
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_REG));
  AlgorithmInstrcutionCtrl1.Data = Data32 & 0xFF;
  AlgorithmInstrcutionCtrl1.Bits.wait_count_clear = 1; // 1, Reset the wait timer at the start of this algo instruction
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_REG), AlgorithmInstrcutionCtrl1.Data);
}

/**
  Setup CPGC2 for advance dq write and read test pattern which this type of traffic:
  WR.....WR Idle RD.....RD Idle
  The number of consecutive WR or RD is controlled by the base_repeats (aka. NumCacheline).
  The Idle is the wait time that is similar to the "subseq wait" of CPGC1.

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatBack2BackWrRd (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Ch,
  IN UINT8            SubCh,
  IN UINT16           SubSeqWait
  )
{
  UINT32                                                 Data32;
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;

  //
  // Data instruction
  //
  DataInstruction.Data = 0;
  DataInstruction.Bits.last = 1;
  DataInstruction.Bits.background_mode = 1; // Chunk inversion based on data_background
  DataInstruction.Bits.data_background = 0; // No chunk inversion
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction.Data);

  //
  // 4 algorithm instructions
  // 0 - WR
  // 1 - Wait
  // 2 - RD
  // 3 - Wait
  //

  AlgorithmInstructionCtrl.Data = 0;
  AlgorithmInstructionCtrl.Bits.wait_event_start = 0; // Do not wait for an event for algo to start
  AlgorithmInstructionCtrl.Bits.wait_count_start = 0; // Wait for timer before starting (introduces bubble between bursts)
  AlgorithmInstructionCtrl.Bits.wait_count_clear = 1; // Reset the wait timer at the start of this algo instruction
  AlgorithmInstructionCtrl.Bits.be_train_err_en = 0;  // Don't do byte enable checking
  AlgorithmInstructionCtrl.Bits.select_on = 0;        // Enable CADB to replace CMD with CADB
  AlgorithmInstructionCtrl.Bits.deselect_on = 0;      // Do not enable CADB to drive deselect traffic for this algo
  AlgorithmInstructionCtrl.Bits.base_range_row = 0;   // Use base_repeats and not base_col_repeats. Control base repeats. 1 controls the base cl repeats for nop and 1 nop.
  AlgorithmInstructionCtrl.Bits.fasty_init = 0;       // Do not override address_direction setting

  //
  // Program the algrithm instruction 0 which performs the "Write"
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG));
  AlgorithmInstruction.Data = Data32 & 0xFF;
  AlgorithmInstruction.Bits.command_start_pointer = 0; // Point to write command instruction
  AlgorithmInstruction.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl.Data);

  //
  // Program the algrithm instruction 1 which performs the "Wait"
  //
  AlgorithmInstructionCtrl.Bits.wait_count_start = 1;
  AlgorithmInstructionCtrl.Bits.wait_count_clear = 0;
  AlgorithmInstructionCtrl.Bits.base_range_row = 1;

  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh,  CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_REG));
  AlgorithmInstruction.Data = Data32 & 0xFF;
  AlgorithmInstruction.Bits.command_start_pointer = 23; // Point to NOP command
  AlgorithmInstruction.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_REG), AlgorithmInstruction.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl.Data);

  //
  // Program the algrithm instruction 2 which perform the "Read"
  //
  AlgorithmInstructionCtrl.Bits.wait_count_start = 0;
  AlgorithmInstructionCtrl.Bits.wait_count_clear = 1;
  AlgorithmInstructionCtrl.Bits.base_range_row = 0;

  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_2_MCCPGC_MAIN_REG));
  AlgorithmInstruction.Data = Data32 & 0xFF;
  AlgorithmInstruction.Bits.command_start_pointer = 1; // Point to read command instruction
  AlgorithmInstruction.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_2_MCCPGC_MAIN_REG), AlgorithmInstruction.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_2_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl.Data);

  //
  // Program the algrithm instruction 3 which performs the "Wait"
  //
  AlgorithmInstructionCtrl.Bits.wait_count_start = 1;
  AlgorithmInstructionCtrl.Bits.wait_count_clear = 0;
  AlgorithmInstructionCtrl.Bits.base_range_row = 1;

  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_3_MCCPGC_MAIN_REG));
  AlgorithmInstruction.Data = Data32 & 0xFF;
  AlgorithmInstruction.Bits.command_start_pointer = 23; // Point to NOP command
  AlgorithmInstruction.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_3_MCCPGC_MAIN_REG), AlgorithmInstruction.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_3_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl.Data);

  //
  // 3 command instructions
  //

  //
  // Write
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG));
  CommandInstruction.Data = Data32 & 0xFF;
  CommandInstruction.Bits.writecmd = 1; // write
  CommandInstruction.Bits.last = 1;
  CommandInstruction.Bits.alternate_data = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction.Data);

  //
  // Read
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG));
  CommandInstruction.Data = Data32 & 0xFF;
  CommandInstruction.Bits.writecmd = 0; // read
  CommandInstruction.Bits.last = 1;
  CommandInstruction.Bits.alternate_data = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG), CommandInstruction.Data);

  //
  // (dummy for wait oepration)
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_REG));
  CommandInstruction.Data = Data32 & 0xFF;
  CommandInstruction.Bits.last = 1;
  CommandInstruction.Bits.alternate_data = 1;
  CommandInstruction.Bits.offset = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_REG), CommandInstruction.Data);

  //
  // wait settings
  //
  AlgorithmWaitEventControl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG));
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0
  AlgorithmWaitEventControl.Bits.timer = 0x4; // 2^timer
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = NATIVE_CPGC_CLOCK; // Native CPGC clock
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);
}

/**
  Setup CPGC2 to generate the pattern WR.....WR Idle RD.....RD Idle......
  The number of consecutive WR or RD is controlled by the base_repeats (aka. NumCacheline).
  The Idle is the wait time that is similar to the "subseq wait" of CPGC1.

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatBack2BackWrWrRdRd (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Ch,
  IN UINT8            SubCh,
  IN UINT16           SubSeqWait
  )
{
  UINT32                                                 Data32;
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;

  //
  // Data instruction
  //
  DataInstruction.Data = 0;
  DataInstruction.Bits.last = 1;
  DataInstruction.Bits.background_mode = 0;// 1; // Chunk inversion based on data_background
  DataInstruction.Bits.data_background = 0; // No chunk inversion
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction.Data);

  //
  // 4 algorithm instructions
  // 0 - WR
  // 1 - Wait
  // 2 - RD
  // 3 - Wait
  //

  AlgorithmInstructionCtrl.Data = 0;
  AlgorithmInstructionCtrl.Bits.wait_event_start = 0; // Do not wait for an event for algo to start
  AlgorithmInstructionCtrl.Bits.wait_count_start = 0; // Wait for timer before starting (introduces bubble between bursts)
  AlgorithmInstructionCtrl.Bits.wait_count_clear = 1; // Reset the wait timer at the start of this algo instruction
  AlgorithmInstructionCtrl.Bits.be_train_err_en = 0;  // Don't do byte enable checking
  AlgorithmInstructionCtrl.Bits.select_on = 0;        // Enable CADB to replace CMD with CADB
  AlgorithmInstructionCtrl.Bits.deselect_on = 0;      // Do not enable CADB to drive deselect traffic for this algo
  AlgorithmInstructionCtrl.Bits.base_range_row = 0;   // Use base_repeats and not base_col_repeats. Control base repeats. 1 controls the base cl repeats for nop and 1 nop.
  AlgorithmInstructionCtrl.Bits.fasty_init = 0;       // Do not override address_direction setting

  //
  // Program the algrithm instruction 0 which performs the "Write"
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG));
  AlgorithmInstruction.Data = Data32 & 0xFF;
  AlgorithmInstruction.Bits.command_start_pointer = 0; // Point to write command instruction
  AlgorithmInstruction.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl.Data);

  //
  // Program the algrithm instruction 1 which performs the "Wait"
  //
  AlgorithmInstructionCtrl.Bits.wait_count_start = 1;
  AlgorithmInstructionCtrl.Bits.wait_count_clear = 0;
  AlgorithmInstructionCtrl.Bits.base_range_row = 1;

  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh,  CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_REG));
  AlgorithmInstruction.Data = Data32 & 0xFF;
  AlgorithmInstruction.Bits.command_start_pointer = 23; // Point to NOP command
  AlgorithmInstruction.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_REG), AlgorithmInstruction.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl.Data);

  //
  // Program the algrithm instruction 2 which perform the "Read"
  //
  AlgorithmInstructionCtrl.Bits.wait_count_start = 0;
  AlgorithmInstructionCtrl.Bits.wait_count_clear = 1;
  AlgorithmInstructionCtrl.Bits.base_range_row = 0;

  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_2_MCCPGC_MAIN_REG));
  AlgorithmInstruction.Data = Data32 & 0xFF;
  AlgorithmInstruction.Bits.command_start_pointer = 1; // Point to read command instruction
  AlgorithmInstruction.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_2_MCCPGC_MAIN_REG), AlgorithmInstruction.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_2_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl.Data);

  //
  // Program the algrithm instruction 3 which performs the "Wait"
  //
  AlgorithmInstructionCtrl.Bits.wait_count_start = 1;
  AlgorithmInstructionCtrl.Bits.wait_count_clear = 0;
  AlgorithmInstructionCtrl.Bits.base_range_row = 1;

  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_3_MCCPGC_MAIN_REG));
  AlgorithmInstruction.Data = Data32 & 0xFF;
  AlgorithmInstruction.Bits.command_start_pointer = 23; // Point to NOP command
  AlgorithmInstruction.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_3_MCCPGC_MAIN_REG), AlgorithmInstruction.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_3_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl.Data);

  //
  // 3 command instructions
  //

  //
  // Write
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG));
  CommandInstruction.Data = Data32 & 0xFF;
  CommandInstruction.Bits.writecmd = 1; // write
  CommandInstruction.Bits.last = 1;
  CommandInstruction.Bits.alternate_data = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction.Data);

  //
  // Read
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG));
  CommandInstruction.Data = Data32 & 0xFF;
  CommandInstruction.Bits.writecmd = 0; // read
  CommandInstruction.Bits.last = 1;
  CommandInstruction.Bits.alternate_data = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG), CommandInstruction.Data);

  //
  // (dummy for wait oepration)
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_REG));
  CommandInstruction.Data = Data32 & 0xFF;
  CommandInstruction.Bits.last = 1;
  CommandInstruction.Bits.alternate_data = 1;
  CommandInstruction.Bits.offset = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_REG), CommandInstruction.Data);

  //
  // wait settings
  //
  AlgorithmWaitEventControl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG));
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0
  AlgorithmWaitEventControl.Bits.timer = 0x4; // 2^timer
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = NATIVE_CPGC_CLOCK; // Native CPGC clock
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);
}

/**
  Setup CPGC 2 for advance dq write and read test pattern

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatWrRdAdvDq (
  IN PSYSHOST         Host,
  IN UINT8            Socket,
  IN UINT8            Ch,
  IN UINT8            SubCh,
  IN UINT16           SubSeqWait
  )
{
  UINT32 Data32;
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;

  UINT32 CpgcAlgoInstRegArray[CPGC_MAX_NUMBER_ALGORITHM_INSTRUCTION] =
                                      {CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_2_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_3_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_4_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_5_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_6_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_7_MCCPGC_MAIN_REG};

  UINT32 CpgcAlgoInstCtrlRegArray[CPGC_MAX_NUMBER_ALGORITHM_INSTRUCTION] =
                                      {CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_CTRL_2_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_CTRL_3_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_CTRL_4_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_CTRL_5_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_CTRL_6_MCCPGC_MAIN_REG,
                                       CPGC2_ALGORITHM_INSTRUCTION_CTRL_7_MCCPGC_MAIN_REG};

  UINT8 AlgoForWrIndex[CPGC_MAX_ADV_DQ_WR_ALGO]     = {0,2};
  UINT8 AlgoForRdIndex[CPGC_MAX_ADV_DQ_RD_ALGO]     = {4,6};
  UINT8 AlgoForWaitIndex[CPGC_MAX_ADV_DQ_WAIT_ALGO] = {1,3,5,7};
  UINT8 Index;

  //
  // 1 data instruction
  //
  DataInstruction.Data = 0;
  DataInstruction.Bits.last = 1;
  DataInstruction.Bits.background_mode = 1; // Chunk inversion based on data_background
  DataInstruction.Bits.data_background = 0; // No chunk inversion
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction.Data);

  //
  // 8 algorithm instructions
  // (32WR-WAIT-32WR-WAIT-32RD-WAIT-32RD-WAIT)
  //
  AlgorithmInstructionCtrl.Data = 0;
  AlgorithmInstructionCtrl.Bits.wait_event_start = 0; // Do not wait for an event for algo to start
  AlgorithmInstructionCtrl.Bits.wait_count_start = 0; // Wait for timer before starting (introduces bubble between bursts)
  AlgorithmInstructionCtrl.Bits.wait_count_clear = 1;// Reset the wait timer at the start of this algo instruction
  AlgorithmInstructionCtrl.Bits.be_train_err_en = 0; // Don't do byte enable checking
  AlgorithmInstructionCtrl.Bits.select_on = 0; // Enable CADB to replace CMD with CADB
  AlgorithmInstructionCtrl.Bits.deselect_on = 0; // Do not enable CADB to drive deselect traffic for this algo
  AlgorithmInstructionCtrl.Bits.base_range_row = 0;// Use base_repeats and not base_col_repeats. Control base repeats. 1 controls the base cl repeats for nop and 1 nop.
  AlgorithmInstructionCtrl.Bits.fasty_init = 0; // Do not override address_direction setting

  //
  // 2 writes
  //
  for (Index = 0; Index < CPGC_MAX_ADV_DQ_WR_ALGO; Index++) {
    Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CpgcAlgoInstRegArray[AlgoForWrIndex[Index]]));
    AlgorithmInstruction.Data = Data32 & 0xFF;
    AlgorithmInstruction.Bits.command_start_pointer = 0; // Point to write command instruction
    AlgorithmInstruction.Bits.last = 0;
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CpgcAlgoInstRegArray[AlgoForWrIndex[Index]]), AlgorithmInstruction.Data);
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CpgcAlgoInstCtrlRegArray[AlgoForWrIndex[Index]]), AlgorithmInstructionCtrl.Data);
  }

  //
  // 2 reads
  //
  for (Index = 0; Index < CPGC_MAX_ADV_DQ_RD_ALGO; Index++) {
    Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CpgcAlgoInstRegArray[AlgoForRdIndex[Index]]));
    AlgorithmInstruction.Data = Data32 & 0xFF;
    AlgorithmInstruction.Bits.command_start_pointer = 1; // Point to read command instruction
    AlgorithmInstruction.Bits.last = 0;
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CpgcAlgoInstRegArray[AlgoForRdIndex[Index]]), AlgorithmInstruction.Data);
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CpgcAlgoInstCtrlRegArray[AlgoForRdIndex[Index]]), AlgorithmInstructionCtrl.Data);
  }

  //
  // 4 waits
  //
  AlgorithmInstructionCtrl.Data = 0;
  AlgorithmInstructionCtrl.Bits.wait_event_start = 0; // Do not wait for an event for algo to start
  AlgorithmInstructionCtrl.Bits.wait_count_start = 1; // Wait for timer before starting (introduces bubble between bursts)
  AlgorithmInstructionCtrl.Bits.wait_count_clear = 0;// Reset the wait timer at the start of this algo instruction
  AlgorithmInstructionCtrl.Bits.be_train_err_en = 0; // Don't do byte enable checking
  AlgorithmInstructionCtrl.Bits.select_on = 0; // Enable CADB to replace CMD with CADB
  AlgorithmInstructionCtrl.Bits.deselect_on = 0; // Do not enable CADB to drive deselect traffic for this algo
  AlgorithmInstructionCtrl.Bits.base_range_row = 1;// Use base_col_repeats and not base_repeats for a rapid short test
  AlgorithmInstructionCtrl.Bits.fasty_init = 0; // Do not override address_direction setting

  for (Index = 0; Index < CPGC_MAX_ADV_DQ_WAIT_ALGO; Index++) {
    Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CpgcAlgoInstRegArray[AlgoForWaitIndex[Index]]));
    AlgorithmInstruction.Data = Data32 & 0xFF;
    AlgorithmInstruction.Bits.command_start_pointer = 23; // Point to read command instruction
    if (Index == (CPGC_MAX_ADV_DQ_WAIT_ALGO - 1)) {
      AlgorithmInstruction.Bits.last = 1; // the last algorithm is a wait according to (32WR-WAIT-32WR-WAIT-32RD-WAIT-32RD-WAIT)
    } else {
      AlgorithmInstruction.Bits.last = 0;
    }
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CpgcAlgoInstRegArray[AlgoForWaitIndex[Index]]), AlgorithmInstruction.Data);
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CpgcAlgoInstCtrlRegArray[AlgoForWaitIndex[Index]]), AlgorithmInstructionCtrl.Data);
  }

  //
  // 3 command instructions
  // (write)
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG));
  CommandInstruction.Data = Data32 & 0xFF;
  CommandInstruction.Bits.writecmd = 1; // write
  CommandInstruction.Bits.last = 1;
  CommandInstruction.Bits.alternate_data = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction.Data);

  //
  // (read)
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG));
  CommandInstruction.Data = Data32 & 0xFF;
  CommandInstruction.Bits.writecmd = 0; // read
  CommandInstruction.Bits.last = 1;
  CommandInstruction.Bits.alternate_data = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG), CommandInstruction.Data);

  //
  // (dummy for wait oepration)
  //
  Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_REG));
  CommandInstruction.Data = Data32 & 0xFF;
  CommandInstruction.Bits.last = 1;
  CommandInstruction.Bits.alternate_data = 1;
  CommandInstruction.Bits.offset = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_23_MCCPGC_MAIN_REG), CommandInstruction.Data);

  //
  // wait settings
  //
  AlgorithmWaitEventControl.Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG));
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0
  AlgorithmWaitEventControl.Bits.timer = 0x4; // 2^timer
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = NATIVE_CPGC_CLOCK; // Native CPGC clock
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);
}

/**
  Setup CPGC 2 for ReadWriteRead test pattern

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatRdWrRd (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait
  )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl0;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_STRUCT       AlgorithmInstruction1;
  CPGC2_ALGORITHM_INSTRUCTION_2_MCCPGC_MAIN_STRUCT       AlgorithmInstruction2;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction0;
  CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_STRUCT         CommandInstruction1;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;

  //
  // 1 data instruction
  //
  DataInstruction0.Data = 0;
  DataInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction0.Data);

  //
  // 3 algorithm instructions
  // (read), (write), (read)
  //
  AlgorithmInstruction0.Data = 0;
  AlgorithmInstruction0.Bits.command_start_pointer = 1; // point to read command instruction
  AlgorithmInstruction0.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction0.Data);

  AlgorithmInstruction1.Data = 0;
  AlgorithmInstruction1.Bits.command_start_pointer = 0; // point to write command instruction
  AlgorithmInstruction1.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_REG), AlgorithmInstruction1.Data);

  AlgorithmInstruction2.Data = 0;
  AlgorithmInstruction2.Bits.command_start_pointer = 1; // point to read command instruction
  AlgorithmInstruction2.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_2_MCCPGC_MAIN_REG), AlgorithmInstruction2.Data);

  //
  // set wait time between each algorithms
  //
  AlgorithmInstructionCtrl0.Data = 0;
  AlgorithmInstructionCtrl0.Bits.wait_count_clear = 1;
  AlgorithmInstructionCtrl0.Bits.wait_count_start = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_2_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);

  AlgorithmWaitEventControl.Data = 0;
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0
  AlgorithmWaitEventControl.Bits.timer = 0xf; // Disable event wait timer
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = 3; // Native CPGC clock
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);

  //
  // 2 command instructions
  // (write), (read)
  //
  CommandInstruction0.Data = 0;
  CommandInstruction0.Bits.writecmd = 1; // Write
  CommandInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction0.Data);
  CommandInstruction1.Data = 0;
  CommandInstruction1.Bits.writecmd = 0; // Read
  CommandInstruction1.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG), CommandInstruction1.Data);
}

/**
  Setup CPGC 2 for ODT Turnaround test pattern

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatODTTA (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait
  )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl0;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_STRUCT       AlgorithmInstruction1;
  CPGC2_ALGORITHM_INSTRUCTION_2_MCCPGC_MAIN_STRUCT       AlgorithmInstruction2;
  CPGC2_ALGORITHM_INSTRUCTION_3_MCCPGC_MAIN_STRUCT       AlgorithmInstruction3;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction0;
  CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_STRUCT         CommandInstruction1;
  CPGC2_COMMAND_INSTRUCTION_2_MCCPGC_MAIN_STRUCT         CommandInstruction2;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;

  //
  // 1 data instruction
  //
  DataInstruction0.Data = 0;
  DataInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction0.Data);

  //
  // 4 algorithm instructions
  // (write), (read), (write, read), (write)
  //
  AlgorithmInstruction0.Data = 0;
  AlgorithmInstruction0.Bits.command_start_pointer = 0; // point to write command instruction
  AlgorithmInstruction0.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction0.Data);

  AlgorithmInstruction1.Data = 0;
  AlgorithmInstruction1.Bits.command_start_pointer = 2; // point to read command instruction
  AlgorithmInstruction1.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_1_MCCPGC_MAIN_REG), AlgorithmInstruction1.Data);

  AlgorithmInstruction2.Data = 0;
  AlgorithmInstruction2.Bits.command_start_pointer = 1; // point to write&read command instruction
  AlgorithmInstruction2.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_2_MCCPGC_MAIN_REG), AlgorithmInstruction2.Data);

  AlgorithmInstruction3.Data = 0;
  AlgorithmInstruction3.Bits.command_start_pointer = 0; // point to write command instruction
  AlgorithmInstruction3.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_3_MCCPGC_MAIN_REG), AlgorithmInstruction3.Data);

  //
  // set wait time between each algorithms
  //
  AlgorithmInstructionCtrl0.Data = 0;
  AlgorithmInstructionCtrl0.Bits.wait_count_clear = 1;
  AlgorithmInstructionCtrl0.Bits.wait_count_start = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_1_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_2_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_3_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);

  AlgorithmWaitEventControl.Data = 0;
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0
  AlgorithmWaitEventControl.Bits.timer = 0xf; // Disable event wait timer
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = 3; // Native CPGC clock
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);

  //
  // 3 command instructions
  // (write), (write, read)
  //
  CommandInstruction0.Data = 0;
  CommandInstruction0.Bits.writecmd = 1; // Write
  CommandInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction0.Data);
  CommandInstruction1.Data = 0;
  CommandInstruction1.Bits.writecmd = 1; // Write
  CommandInstruction1.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG), CommandInstruction1.Data);
  CommandInstruction2.Data = 0;
  CommandInstruction2.Bits.writecmd = 0; // Read
  CommandInstruction2.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_2_MCCPGC_MAIN_REG), CommandInstruction2.Data);
}

/**
  Setup CPGC 2 for Write Only test pattern

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence
  @param[in]  IsAddrUp            Set address traverse direction. TRUE - up direction (incremental), FALSE - down direction (decremental)
  @param[in]  IsDataInvt          Set data inversion. TRUE - data inversion, FALSE - data no inversion

  @retval N/A

**/
VOID
SetCpgcCmdPatWr (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait,
  IN BOOLEAN        IsAddrUp,
  IN BOOLEAN        IsDataInvt
  )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl0;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction0;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction0;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;

  //
  // 1 data instruction
  //
  DataInstruction0.Data = 0;
  DataInstruction0.Bits.last = 1;
  DataInstruction0.Bits.background_mode = 1; // Chunk inversion based on data_background
  DataInstruction0.Bits.data_background = 0; // No chunk inversion
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction0.Data);

  //
  // 1 algorithm instructions
  // (write)
  //
  AlgorithmInstruction0.Data = 0;
  AlgorithmInstruction0.Bits.command_start_pointer = 0; // point to write command instruction
  AlgorithmInstruction0.Bits.last = 1;
  if (IsAddrUp) {
    AlgorithmInstruction0.Bits.inverse_direction = 0; // address up, incremental
  } else {
    AlgorithmInstruction0.Bits.inverse_direction = 1; // address down, decremental
  }
  if (IsDataInvt) {
    AlgorithmInstruction0.Bits.invert_data = 1; // data inversion
  } else {
    AlgorithmInstruction0.Bits.invert_data = 0; // data no inversion
  }
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction0.Data);

  //
  // set wait time between each algorithms
  //
  AlgorithmInstructionCtrl0.Data = 0;
  AlgorithmInstructionCtrl0.Bits.wait_count_clear = 1;
  AlgorithmInstructionCtrl0.Bits.wait_count_start = 1; // Algo instruction is stalled until the Wait_Time_Current is equal to the Wait_Time.
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);

  AlgorithmWaitEventControl.Data = 0;
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0
  AlgorithmWaitEventControl.Bits.timer = CPGC_EVENT_WAIT_TIMER_DISABLE; // Disable event wait timer
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = NATIVE_CPGC_CLOCK; // Native CPGC clock
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);

  //
  // 1 command instructions
  // (write)
  //
  CommandInstruction0.Data = 0;
  CommandInstruction0.Bits.writecmd = 1; // Write
  CommandInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction0.Data);
}

/**
  Setup CPGC 2 for [n WR]-[n RD] Command Sequence.

  @param[in]  Host                   Pointer to SysHost
  @param[in]  Socket                 Socket number
  @param[in]  Ch                     Channel number
  @param[in]  Subch                  Sub channel number
  @param[in]  SubSeqWait             Number of Dclks to stall at the end of a sub-sequence
  @param[in]  ConsecutiveReadNumber  Number of consecutive read commands
  @param[in]  ConsecutiveWriteNumber Number of consecutive write commands

  @retval N/A

**/
VOID
SetCpgcCmdPatTurnaroundAll (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait,
  IN UINT16         ConsecutiveReadNumber,
  IN UINT16         ConsecutiveWriteNumber
  )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl0;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction0;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;
  UINT16                                                 Index;
  UINT32                                                 RegisterAddr;

  //
  // Create Command instructions and Algorithm instruction:
  // [(<WR x ConsecutiveWriteNumber>-<RD x ConsecutiveReadNumber>) x Base Repeates] x Block Repeates
  //

  //
  // 1 data instruction
  //
  DataInstruction0.Data = 0;
  DataInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction0.Data);

  //
  // Create algrotim instruction list which has only 1 algorithm instructions.
  //

  AlgorithmInstruction.Data = 0;
  AlgorithmInstruction.Bits.command_start_pointer = 0; // Point to write command instruction list
  AlgorithmInstruction.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction.Data);

  //
  // Set wait time between each algorithm.
  //
  AlgorithmInstructionCtrl0.Data = 0;
  AlgorithmInstructionCtrl0.Bits.wait_count_clear = 1;
  AlgorithmInstructionCtrl0.Bits.wait_count_start = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);

  AlgorithmWaitEventControl.Data = 0;
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0
  AlgorithmWaitEventControl.Bits.timer = 0xf; // Disable event wait timer
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = 3; // Native CPGC clock
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);

  //
  // ConsecutiveWriteNumber Write commands.
  //
  for (Index = 0; Index < ConsecutiveWriteNumber; Index++) {
    CommandInstruction0.Data = 0;
    CommandInstruction0.Bits.writecmd = 1; // Write
    CommandInstruction0.Bits.last = 0;
    RegisterAddr = CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG + Index;
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, RegisterAddr), CommandInstruction0.Data);
  }

  //
  // (ConsecutiveReadNumber - 1) Read commands.
  //
  for (Index = 0; Index < (ConsecutiveReadNumber - 1); Index++) {
    CommandInstruction0.Data = 0;
    CommandInstruction0.Bits.writecmd = 0; // read
    CommandInstruction0.Bits.last = 0;
    RegisterAddr = CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG + ConsecutiveWriteNumber + Index;
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, RegisterAddr), CommandInstruction0.Data);
  }

  //
  // The last Read command.
  //
  CommandInstruction0.Data = 0;
  CommandInstruction0.Bits.writecmd = 0; // read
  CommandInstruction0.Bits.last = 1;
  RegisterAddr = CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG + ConsecutiveWriteNumber + (ConsecutiveReadNumber - 1);
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, RegisterAddr), CommandInstruction0.Data);
}

/**
  Setup CPGC 2 for ReadWrite Turnarounds test pattern

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatRdWrTA (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait
  )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl0;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction0;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction0;
  CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_STRUCT         CommandInstruction1;
  CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_STRUCT  AlgorithmWaitEventControl;

  //
  // 1 data instruction
  //
  DataInstruction0.Data = 0;
  DataInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction0.Data);

  //
  // 1 algorithm instructions
  // (write, read)
  //
  AlgorithmInstruction0.Data = 0;
  AlgorithmInstruction0.Bits.command_start_pointer = 0; // point to write&read command instruction
  AlgorithmInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction0.Data);

  //
  // set wait time between each algorithms
  //
  AlgorithmInstructionCtrl0.Data = 0;
  AlgorithmInstructionCtrl0.Bits.wait_count_clear = 1;
  AlgorithmInstructionCtrl0.Bits.wait_count_start = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);

  AlgorithmWaitEventControl.Data = 0;
  AlgorithmWaitEventControl.Bits.select_event = 0; // Don't care for wait_event_start = 0
  AlgorithmWaitEventControl.Bits.timer = 0xf; // Disable event wait timer
  AlgorithmWaitEventControl.Bits.wait_clock_frequency = 3; // Native CPGC clock
  AlgorithmWaitEventControl.Bits.wait_time = SubSeqWait;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), AlgorithmWaitEventControl.Data);

  //
  // 3 command instructions
  // (write, read)
  //
  CommandInstruction0.Data = 0;
  CommandInstruction0.Bits.writecmd = 1; // Write
  CommandInstruction0.Bits.last = 0;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction0.Data);
  CommandInstruction1.Data = 0;
  CommandInstruction1.Bits.writecmd = 0; // read
  CommandInstruction1.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_1_MCCPGC_MAIN_REG), CommandInstruction1.Data);
}

/**
  Setup CPGC 2 for dummy write pattern

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatDummyWr (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait
  )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction0;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl0;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction0;

  //
  // 1 data instruction
  //
  DataInstruction0.Data = 0;
  DataInstruction0.Bits.last = 1;
  DataInstruction0.Bits.data_background = 1; // chunk inversion based on data_background
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction0.Data);

  //
  // 1 algorithm instructions
  // (write)
  //
  AlgorithmInstruction0.Data = 0;
  AlgorithmInstruction0.Bits.command_start_pointer = 0; // point to write command instruction
  AlgorithmInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction0.Data);

  //
  // set wait time between each algorithms
  //
  AlgorithmInstructionCtrl0.Data = 0;
  AlgorithmInstructionCtrl0.Bits.select_on = 1; // Enable CADB to replace CMDs with CADB
  AlgorithmInstructionCtrl0.Bits.deselect_on = 1; // Enable CADB to drive deselect traffic for this algo
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);

  //
  // wait timer not used
  //
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_WAIT_EVENT_CONTROL_MCCPGC_MAIN_REG), 0);

  //
  // 1 command instructions
  // (write)
  //
  CommandInstruction0.Data = 0;
  CommandInstruction0.Bits.writecmd = 1; // Write
  CommandInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction0.Data);
}

/**
  Setup CPGC 2 for PDA Enumerate ID pattern

  @param[in]  Host                Pointer to SysHost
  @param[in]  Socket              Socket number
  @param[in]  Ch                  Channel number
  @param[in]  Subch               Sub channel number
  @param[in]  SubSeqWait          Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPatPdaEnum (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT16         SubSeqWait
  )
{
  CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_STRUCT            DataInstruction0;
  CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_STRUCT         CommandInstruction0;
  CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_STRUCT  AlgorithmInstructionCtrl0;
  CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_STRUCT       AlgorithmInstruction0;

  CommandInstruction0.Data = 0;

  //
  // Program cpgc2_command_instruction
  //
  CommandInstruction0.Bits.writecmd = 1;
  CommandInstruction0.Bits.last = 1;
  CommandInstruction0.Bits.alternate_data = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_COMMAND_INSTRUCTION_0_MCCPGC_MAIN_REG), CommandInstruction0.Data);

  //
  // Program cpgc2_algorithm_instruction
  //
  AlgorithmInstruction0.Data = 0;
  AlgorithmInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_0_MCCPGC_MAIN_REG), AlgorithmInstruction0.Data);

  //
  // Program cpgc2_algorithm_instruction_ctrl
  //
  AlgorithmInstructionCtrl0.Data = 0;
  AlgorithmInstructionCtrl0.Bits.select_on = 1;
  AlgorithmInstructionCtrl0.Bits.deselect_on = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_ALGORITHM_INSTRUCTION_CTRL_0_MCCPGC_MAIN_REG), AlgorithmInstructionCtrl0.Data);

  //
  // Program cpgc2_data_instruction
  //
  DataInstruction0.Data = 0;
  DataInstruction0.Bits.last = 1;
  MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, CPGC2_DATA_INSTRUCTION_0_MCCPGC_MAIN_REG), DataInstruction0.Data);
}

/**
  Query the pointer of cached register list

  @param[in] Socket             - Socket number
  @param[in] Ch                 - Channel number
  @param[in] Subch              - Sub channel number
  @param[in] RegType            - Register type: CPGC2_REGISTER_TYPE, CADB2_REGISTER_TYPE

  @retval Pointer of cached register list
          NULL if Regtype is not supported for Cpgc cache

**/
VOID *
GetCpgcCacheList(
  IN UINT8  Socket,
  IN UINT8  Ch,
  IN UINT8  SubCh,
  IN UINT8  RegType
  )
{
  CPGC2_HOST_DATA  *CpgcHost;
  CPGC2_HOST_CACHE *CpgcCachePtr;

  CpgcHost = GetCpgcHostPointer ();
  if (CpgcHost == NULL) {
    InitCpgcHostData ();
    CpgcHost = GetCpgcHostPointer ();
  }

  CpgcCachePtr = (CPGC2_HOST_CACHE *)CpgcHost->CpgcCachePtr;

  if (CpgcCachePtr == NULL) {
    RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
      "Error: Cpgc cache pointer is not available.\n");
    RC_FATAL_ERROR(FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_201);
    return NULL;
  }

  if (CpgcCachePtr->Signature[Socket] != CPGC_CACHE_SIGNATURE) {
    InitCpgcCacheReg(Socket);
  }

  if (RegType == CPGC2_REGISTER_TYPE) {
    return (VOID *)&CpgcCachePtr->CpgcCache[Socket][Ch][SubCh];
  }
  else if (RegType == CADB2_REGISTER_TYPE) {
    return (VOID *)&CpgcCachePtr->CadbCache[Socket][Ch][SubCh];
  }
  else {
    RcDebugPrintWithDevice(SDBG_MINMAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
      "Error: RegType [%d] is not supported for CPGC cache.\n", RegType);
    RC_FATAL_ERROR(FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_201);
    return NULL;
  }
}

/**
  Query the pointer of CADB cached data

  @param[in] Socket        - Socket number
  @param[in] Ch            - Channel number
  @param[in] Subch         - Sub channel number
  @param[in] RegAddr       - CADB register address

  @retval Pointer of CADB cached data
          NULL if RegAddr is not supported for CADB cache

**/
UINT32 *
GetCadbCacheDataPointer (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT32   RegAddr
  )
{
  CADB2_REG_CACHE   *CadbCache;

  CadbCache = (CADB2_REG_CACHE *) GetCpgcCacheList (Socket, Ch, SubCh, CADB2_REGISTER_TYPE);

  switch (RegAddr) {
    case CADB_CFG_MCCADB_MAIN_REG:
      return &CadbCache->Cfg;
    case CADB_AO_MRSCFG_MCCADB_MAIN_REG:
      return &CadbCache->AoMrsCfg;
    case CADB_MRSCFG_N0_MCCADB_MAIN_REG:
      return &CadbCache->MrsCfgN0;
    case CADB_MRSCFG_N1_MCCADB_MAIN_REG:
      return &CadbCache->MrsCfgN1;
    case CADB_DLY_MCCADB_MAIN_REG:
      return &CadbCache->Dly;
    case CADB_SELCFG_MCCADB_MAIN_REG:
      return &CadbCache->SelCfg;
    case CADB_DSEL_UNISEQ_LMN_MCCADB_MAIN_REG:
      return &CadbCache->DselUniseqLmn;
    default:
      return NULL;
  }
}

/**
  Query the pointer of CPGC cached data

  @param[in] Socket        - Socket number
  @param[in] Ch            - Channel number
  @param[in] Subch         - Sub channel number
  @param[in] RegAddr       - CPGC register address:

  @retval Pointer of CPGC cached data
          NULL if Regtype is not supported for CPGC cache

**/
UINT32 *
GetCpgcCacheDataPointer (
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    SubCh,
  IN UINT32   RegAddr
  )
{
  CPGC2_REG_CACHE   *CpgcCache;

  CpgcCache = (CPGC2_REG_CACHE *) GetCpgcCacheList (Socket, Ch, SubCh, CPGC2_REGISTER_TYPE);

  switch (RegAddr) {
    case CPGC2_BASE_REPEATS_MCCPGC_MAIN_REG:
      return &CpgcCache->BaseRepeats;
    case CPGC2_BASE_COL_REPEATS_MCCPGC_MAIN_REG:
      return &CpgcCache->BaseColRepeats;
    case CPGC2_BLOCK_REPEATS_MCCPGC_MAIN_REG:
      return &CpgcCache->BlockRepeats;
    case CPGC2_BASE_ADDRESS_CONTROL_MCCPGC_MAIN_REG:
      return &CpgcCache->BaseAddrCtl;
    case CPGC2_ADDRESS_CONTROL_MCCPGC_MAIN_REG:
      return &CpgcCache->AddrCtl;
    case CPGC2_ADDRESS_SIZE_N0_MCCPGC_MAIN_REG:
      return &CpgcCache->AddrSizeN0;
    case CPGC2_ADDRESS_SIZE_N1_MCCPGC_MAIN_REG:
      return &CpgcCache->AddrSizeN1;
    case CPGC2_REGION_LOW_N0_MCCPGC_MAIN_REG:
      return &CpgcCache->RegionLowN0;
    case CPGC2_REGION_LOW_N1_MCCPGC_MAIN_REG:
      return &CpgcCache->RegionLowN1;
    case CPGC2_DATA_CONTROL_MCCPGC_MAIN_REG:
      return &CpgcCache->DataCtl;
    case CPGC2_ADDRESS_INSTRUCTION_0_MCCPGC_MAIN_REG:
      return &CpgcCache->AddrInst[0];
    case CPGC2_ADDRESS_INSTRUCTION_1_MCCPGC_MAIN_REG:
      return &CpgcCache->AddrInst[1];
    case CPGC2_ADDRESS_INSTRUCTION_2_MCCPGC_MAIN_REG:
      return &CpgcCache->AddrInst[2];
    case CPGC2_ADDRESS_INSTRUCTION_3_MCCPGC_MAIN_REG:
       return &CpgcCache->AddrInst[3];
    case CPGC_DPAT_CFG_MCCPGC_MAIN_REG:
       return &CpgcCache->DpatCfg;
    case CPGC_SEQ_CFG_B_MCCPGC_MAIN_REG:
       return &CpgcCache->SeqCfgB;
    default:
      return NULL;
  }
}

/**
  CPGC register cache initialization

  @param[in]  Socket            - Socket number

  @retval N/A

**/
VOID
InitCpgcCacheReg (
  IN UINT8 Socket
  )
{
  UINT8                                   Ch, SubCh;
  PSYSHOST                                Host;
  struct channelNvram                     (*ChannelNvList)[MAX_CH];
  CPGC2_HOST_DATA                         *CpgcHost;
  CPGC2_HOST_CACHE                        *CpgcCachePtr;
  UINT32                                  *CachedData;
  UINT32                                  Index;

  Host = GetSysHostPointer ();

  CpgcHost = GetCpgcHostPointer ();
  if (CpgcHost == NULL) {
    InitCpgcHostData ();
    CpgcHost = GetCpgcHostPointer ();
  }

  CpgcCachePtr = (CPGC2_HOST_CACHE *) CpgcHost->CpgcCachePtr;

  if (CpgcCachePtr->Signature[Socket] == CPGC_CACHE_SIGNATURE) {
    RcDebugPrintWithDevice (SDBG_CPGC, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "InitCpgcCacheReg: Cpgc cache is initialized prevously.\n");
    return;
  } else {
    CpgcCachePtr->Signature[Socket] = CPGC_CACHE_SIGNATURE;
  }

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < GetMaxChDdr (); Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    for (SubCh = 0; SubCh < CpgcGetValidSubCh (); SubCh++) {
      //
      // Load CADB registers which are listed in RegListCadb table
      //
      for (Index = 0; Index < sizeof(RegListCadb) / sizeof(UINT32); Index++) {
        //
        // Get CADB cache data pointer
        //
        CachedData = GetCadbCacheDataPointer (Socket, Ch, SubCh, RegListCadb[Index]);
        //
        // Initialize CADB cache data with default CADB register value
        //
        *CachedData = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegListCadb[Index]));
      }

      //
      // Load CPGC registers which are listed in RegListCpgc table
      //
      for (Index = 0; Index < sizeof(RegListCpgc) / sizeof(UINT32); Index++) {
        //
        // Get CPGC cache data pointer
        //
        CachedData = GetCpgcCacheDataPointer (Socket, Ch, SubCh, RegListCpgc[Index]);
        //
        // Initialize CPGC cache data with default CPGC register value
        //
        *CachedData = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegListCpgc[Index]));
      }
    }
  }
}

/**
  Setup CPGC command pattern for test

  @param[in]  Host               - Pointer to SysHost
  @param[in]  Socket             - Socket number
  @param[in]  Ch                 - Channel number
  @param[in]  Subch              - Sub channel number
  @param[in]  CmdPat             - Command pattern for test
                                 - PatWrRd
                                 - PatWr
                                 - PatRdWrTA
                                 - PatRd
                                 - PatWrRdTA
                                 - PatODTTA
                                 - PatRdWrRd
                                 - PatWrRdAdvDq
                                 - PatRdWr
                                 - PatDummyWr
                                 - PatPdaEnum
  @param[in]  *AddressCarry      - Structure for address carry
  @param[in]  SubSeqWait         - Number of Dclks to stall at the end of a sub-sequence

  @retval N/A

**/
VOID
SetCpgcCmdPat (
  IN PSYSHOST       Host,
  IN UINT8          Socket,
  IN UINT8          Ch,
  IN UINT8          SubCh,
  IN UINT8          CmdPat,
  IN TAddressCarry  *AddressCarry,
  IN UINT16         SubSeqWait
  )
{
  CPGC2_REG_CACHE                                *CpgcCache;
  BOOLEAN                                        DiffFound = FALSE;
  CPGC2_ADDRESS_INSTRUCTION_0_MCCPGC_MAIN_STRUCT AddressInstruction;
  CPGC2_ADDRESS_CONTROL_MCCPGC_MAIN_STRUCT       AddressControl;
  UINT32                                         Data32;
  TAddressCarry                                  AddressCarryLocal;

  if (IsCpgcCacheEnabled()) {

    CpgcCache = (CPGC2_REG_CACHE *) GetCpgcCacheList (Socket, Ch, SubCh, CPGC2_REGISTER_TYPE);

    //
    // Check register cache
    //
    if ((CpgcCache->CmdPattern == CmdPat) &&
        (CpgcCache->SubSeqWait == SubSeqWait)) {
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "SetCpgcCmdPat: Command pattern %d is cached.\n", CmdPat);
      DiffFound = FALSE;
    } else {
      //
      // Update cache
      //
      CpgcCache->CmdPattern = CmdPat;
      CpgcCache->SubSeqWait = SubSeqWait;
      DiffFound = TRUE;
    }
  }

  if ((!IsCpgcCacheEnabled()) || (DiffFound)) {
    switch (CmdPat) {
      case PatWrRd:
        SetCpgcCmdPatWrRd (Host, Socket, Ch, SubCh, SubSeqWait);
        break;
      case PatWrRdDdrt:
        SetCpgcCmdPatWrRdDdrt2 (Host, Socket, Ch, SubCh, SubSeqWait);
        break;
      case PatWr:
      case PatWrUp:
        SetCpgcCmdPatWr (Host, Socket, Ch, SubCh, SubSeqWait, TRUE, FALSE);
        break;
      case PatWrDown:
        SetCpgcCmdPatWr (Host, Socket, Ch, SubCh, SubSeqWait, FALSE, FALSE);
        break;
      case PatWrUpInvt:
        SetCpgcCmdPatWr (Host, Socket, Ch, SubCh, SubSeqWait, TRUE, TRUE);
        break;
      case PatWrDownInvt:
        SetCpgcCmdPatWr (Host, Socket, Ch, SubCh, SubSeqWait, FALSE, TRUE);
        break;
      case PatRdWrTA:
        SetCpgcCmdPatRdWrTA (Host, Socket, Ch, SubCh, SubSeqWait);
        break;
      case PatTurnaroundSgSr:
        SetCpgcCmdPatTurnaroundAll (Host, Socket, Ch, SubCh, SubSeqWait, 4, 4);
        break;
      case PatTurnaroundDrDd:
        SetCpgcCmdPatTurnaroundAll (Host, Socket, Ch, SubCh, SubSeqWait, 8, 8);
        break;
      case PatRd:
      case PatRdUp:
        if (GetCurrentTestType (Socket) == RdDqTest) {
          SetCpgcCmdPatRdDqs (Host, Socket, Ch, SubCh, SubSeqWait);
        } else {
          SetCpgcCmdPatRd (Host, Socket, Ch, SubCh, SubSeqWait, TRUE, FALSE);
        }
        break;
      case PatRdMrr:
        SetCpgcCmdPatRdMrr (Host, Socket, Ch, SubCh, SubSeqWait);
        break;
      case PatRdDown:
        SetCpgcCmdPatRd (Host, Socket, Ch, SubCh, SubSeqWait, FALSE, FALSE);
        break;
      case PatRdUpInvt:
        SetCpgcCmdPatRd (Host, Socket, Ch, SubCh, SubSeqWait, TRUE, TRUE);
        break;
      case PatRdDownInvt:
        SetCpgcCmdPatRd (Host, Socket, Ch, SubCh, SubSeqWait, FALSE, TRUE);
        break;
      case PatWrRdTA:
        //SetCpgcCmdPatWrRdTA (Host, Socket, Ch, SubCh, AddressCarry, SubSeqWait); // SPR_TODO
        break;
      case PatODTTA:
        SetCpgcCmdPatODTTA (Host, Socket, Ch, SubCh, SubSeqWait);
      case PatWrRdFlyb:
        //SetCpgcCmdPatWrRdFlyb (Host, Socket, Ch, SubCh, AddressCarry, SubSeqWait); // SPR_TODO
        break;
      case PatRdWrRd:
        SetCpgcCmdPatRdWrRd (Host, Socket, Ch, SubCh, SubSeqWait);
        break;
      case PatWrRdAdvDq:
        SetCpgcCmdPatWrRdAdvDq (Host, Socket, Ch, SubCh, SubSeqWait);
        break;
      case PatBack2BackWrRd:
        SetCpgcCmdPatBack2BackWrRd (Host, Socket, Ch, SubCh, SubSeqWait);
        break;
      case PatBack2BackWrWrRdRd:
        SetCpgcCmdPatBack2BackWrWrRdRd (Host, Socket, Ch, SubCh, SubSeqWait);
        break;
      case PatRdWrUp:
        SetCpgcCmdPatRdWr (Host, Socket, Ch, SubCh, SubSeqWait, TRUE, FALSE, FALSE);
        break;
      case PatRdWrDown:
        SetCpgcCmdPatRdWr (Host, Socket, Ch, SubCh, SubSeqWait, FALSE, FALSE, FALSE);
        break;
      case PatRdWrUpInvt:
        SetCpgcCmdPatRdWr (Host, Socket, Ch, SubCh, SubSeqWait, TRUE, TRUE, FALSE);
        break;
      case PatRdWrDownInvt:
        SetCpgcCmdPatRdWr (Host, Socket, Ch, SubCh, SubSeqWait, FALSE, TRUE, FALSE);
        break;
      case PatRdWrUpAlt:
        SetCpgcCmdPatRdWr (Host, Socket, Ch, SubCh, SubSeqWait, TRUE, FALSE, TRUE);
        break;
      case PatRdWrDownAlt:
        SetCpgcCmdPatRdWr (Host, Socket, Ch, SubCh, SubSeqWait, FALSE, FALSE, TRUE);
        break;
      case PatRdWrUpInvtAlt:
        SetCpgcCmdPatRdWr (Host, Socket, Ch, SubCh, SubSeqWait, TRUE, TRUE, TRUE);
        break;
      case PatRdWrDownInvtAlt:
        SetCpgcCmdPatRdWr (Host, Socket, Ch, SubCh, SubSeqWait, FALSE, TRUE, TRUE);
        break;
      case PatDummyWr:
        SetCpgcCmdPatDummyWr (Host, Socket, Ch, SubCh, SubSeqWait);
        break;
      case PatPdaEnum:
        SetCpgcCmdPatPdaEnum (Host, Socket, Ch, SubCh, SubSeqWait);
        break;
      default:
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Error: Unknown command pattern %d in SetCpgcCmdPat\n", CmdPat);
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_201);
      break;
    }
  }

  //
  // Update address info for test
  //
  AddressCarryLocal.AddressOrder = AddressCarry->AddressOrder;
  AddressCarryLocal.AddressDirection = AddressCarry->AddressDirection;

  if (((CmdPat == PatRd) && (GetCurrentTestType (Socket) == RdDqTest)) ||
     (CmdPat == PatWrRd)) {
    AddressCarryLocal.AddressOrder = AddressOrderBankRankColRow;
  }

  AddressInstruction.Data = 0;
  AddressInstruction.Bits.last = 1;
  AddressInstruction.Bits.address_decode_enable = 0;
  AddressInstruction.Bits.address_direction = AddressCarryLocal.AddressDirection;
  AddressInstruction.Bits.address_order = AddressCarryLocal.AddressOrder;
  Data32 = AddressInstruction.Data;
  GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_INSTRUCTION_0_MCCPGC_MAIN_REG, &Data32);

  AddressControl.Data = 0;
  AddressControl.Bits.address_decode_rotate_repeats = 0; // 1 repeat at address level
  AddressControl.Bits.fasty_address_direction = AddressCarryLocal.AddressDirection;
  AddressControl.Bits.fasty_address_order = AddressCarryLocal.AddressOrder;
  GetSetCpgcRegs (Host, Socket, Ch, SubCh, CPGC_REG_WRITE, CPGC2_ADDRESS_CONTROL_MCCPGC_MAIN_REG, &AddressControl.Data);
}

/**

  Set the CPGC Unisequencer pattern buffer

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Ch        - Channel number
  @param[in] SubCh     - Sub-channel number
  @param[in] Patbuf    - Pointer of Pattern Buffer

  @retval N/A

**/
VOID
SetCpgcDpatUniseqPat (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubCh,
  IN UINT32     *Patbuf
  )
{
  CPGC2_REG_CACHE   *CpgcCache;
  UINT8             Index;
  BOOLEAN           DiffFound = FALSE;
  UINT32            RegList[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {CPGC_DPAT_UNISEQ_0_MCCPGC_MAIN_REG,
                                                            CPGC_DPAT_UNISEQ_1_MCCPGC_MAIN_REG,
                                                            CPGC_DPAT_UNISEQ_2_MCCPGC_MAIN_REG};

  if (IsCpgcCacheEnabled()) {

    CpgcCache = (CPGC2_REG_CACHE *) GetCpgcCacheList (Socket, Ch, SubCh, CPGC2_REGISTER_TYPE);

    //
    // Check register cache
    //
    for (Index = 0; Index < CPGC_MAX_NUMBER_DPAT_UNISEQ; Index++) {
      if ((CpgcCache->UniseqPat[Index] != Patbuf[Index]) && (Patbuf[Index] != CPGC_DONT_CARE)) {
        //
        // There is something different, need to program registers.
        //
        CpgcCache->UniseqPat[Index] = Patbuf[Index];
        DiffFound = TRUE;
      }
    }

    if (!DiffFound) {
      //
      // No difference found, return.
      //
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
         "SetCpgcDpatUniseqPat: CPGC_DPAT_UNISEQ_0/1/2 are cached = 0x%08x, 0x%08x, 0x%08x\n",
          CpgcCache->UniseqPat[0],
          CpgcCache->UniseqPat[1],
          CpgcCache->UniseqPat[2]);
      return;
    }
  }

  //
  // Update register and cache
  //
  for (Index = 0; Index < CPGC_MAX_NUMBER_DPAT_UNISEQ; Index++) {
    if (Patbuf[Index] != CPGC_DONT_CARE) {
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, RegList[Index]), Patbuf[Index]);
    }
  }
}

/**

  Set the CPGC Unisequencer pattern mux

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Ch        - Channel number
  @param[in] SubCh     - Sub-channel number
  @param[in] MuxCtl    - Pointer of mux array
  @param[in] LfsrSize  - Pointer of LFSR size array

  @retval N/A

**/
VOID
SetCpgcDpatUniseqCfg (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubCh,
  IN UINT32     *MuxCtl,
  IN UINT32     *LfsrSize
  )
{
  CPGC_DPAT_USQ_CFG_0_MCCPGC_MAIN_STRUCT  DpatUsqCfg;
  UINT8                                   Index;
  CPGC2_REG_CACHE                         *CpgcCache;
  BOOLEAN                                 DiffFound = FALSE;
  UINT32                                  RegList[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {CPGC_DPAT_USQ_CFG_0_MCCPGC_MAIN_REG,
                                                                                  CPGC_DPAT_USQ_CFG_1_MCCPGC_MAIN_REG,
                                                                                  CPGC_DPAT_USQ_CFG_2_MCCPGC_MAIN_REG};

  if (IsCpgcCacheEnabled()) {

    CpgcCache = (CPGC2_REG_CACHE *) GetCpgcCacheList (Socket, Ch, SubCh, CPGC2_REGISTER_TYPE);

    //
    // Check register cache
    //
    for (Index = 0; Index < CPGC_MAX_NUMBER_DPAT_UNISEQ; Index++) {
      if (((CpgcCache->UniseqMux[Index] != MuxCtl[Index]) && (MuxCtl[Index] != CPGC_DONT_CARE)) ||
          ((CpgcCache->UniseqLfsrSize[Index] != LfsrSize[Index]) && (LfsrSize[Index] != CPGC_DONT_CARE ))) {
        //
        // There is something different, need to program registers.
        //
        CpgcCache->UniseqLfsrSize[Index] = LfsrSize[Index];
        CpgcCache->UniseqMux[Index] = MuxCtl[Index];
        DiffFound = TRUE;
      }
    }

    if (!DiffFound) {
      //
      // No difference found, return.
      //
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "SetCpgcDpatUniseqCfg: CPGC_DPAT_USQ_CFG_0/1/2 are cached, Mux = %d %d %d LfsrSize = %d %d %d\n",
          CpgcCache->UniseqMux[0],
          CpgcCache->UniseqMux[1],
          CpgcCache->UniseqMux[2],
          CpgcCache->UniseqLfsrSize[0],
          CpgcCache->UniseqLfsrSize[1],
          CpgcCache->UniseqLfsrSize[2]);
      return;
    }
  }

  //
  // write register
  //
  for (Index = 0; Index < CPGC_MAX_NUMBER_DPAT_UNISEQ; Index++) {
    if ((MuxCtl[Index] == CPGC_DONT_CARE) && (LfsrSize[Index] == CPGC_DONT_CARE)) {
      continue;
    } else {
      DpatUsqCfg.Data = (UINT8) (0xFF & MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, RegList[Index])));
      if (LfsrSize[Index] != CPGC_DONT_CARE) {
        DpatUsqCfg.Bits.uniseq_lfsr_polynomial_size = (UINT8) LfsrSize[Index];
      }
      if (MuxCtl[Index] != CPGC_DONT_CARE) {
        DpatUsqCfg.Bits.uniseq_mode = (UINT8) MuxCtl[Index];
      }
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, RegList[Index]), DpatUsqCfg.Data);
    }
  }
}

/**

  Set the CPGC Unisequencer galois polynomial

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket
  @param[in] Ch         - Channel number
  @param[in] SubCh      - Sub-channel number
  @param[in] Poly       - Pointer of polynomial

  @retval N/A

**/
VOID
SetCpgcDpatUniseqPoly (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       SubCh,
  IN UINT32      *Poly
  )
{
  UINT8             Index;
  CPGC2_REG_CACHE   *CpgcCache;
  BOOLEAN           DiffFound = FALSE;
  UINT32            RegList[CPGC_MAX_NUMBER_DPAT_UNISEQ] = {CPGC_DPAT_UNISEQ_POLY_0_MCCPGC_MAIN_REG,
                                                            CPGC_DPAT_UNISEQ_POLY_1_MCCPGC_MAIN_REG,
                                                            CPGC_DPAT_UNISEQ_POLY_2_MCCPGC_MAIN_REG};
  if (IsCpgcCacheEnabled()) {

    CpgcCache = (CPGC2_REG_CACHE *) GetCpgcCacheList (Socket, Ch, SubCh, CPGC2_REGISTER_TYPE);

    //
    // Check register cache
   //
    for (Index = 0; Index < CPGC_MAX_NUMBER_DPAT_UNISEQ; Index++) {
      if ((CpgcCache->UniseqPoly[Index] != Poly[Index]) && (Poly[Index] != CPGC_DONT_CARE)) {
        //
        // There is something different, need to program registers.
        //
        CpgcCache->UniseqPoly[Index] = Poly[Index];
        DiffFound = TRUE;
      }
    }

    if (!DiffFound) {
      //
      // No difference found, return.
      //
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "SetCpgcDpatUniseqPoly: CPGC_DPAT_UNISEQ_POLY_0/1/2 are cached = 0x%08x, 0x%08x, 0x%08x\n",
          CpgcCache->UniseqPoly[0],
          CpgcCache->UniseqPoly[1],
          CpgcCache->UniseqPoly[2]);
      return;
    }
  }

  //
  // write register
  //
  for (Index = 0; Index < CPGC_MAX_NUMBER_DPAT_UNISEQ; Index++) {
    if (Poly[Index] != CPGC_DONT_CARE) {
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, RegList[Index]), Poly[Index]);
    }
  }
}

/**

  Set the CADB deselect unisequencer galois polynomial

  @param[in] Host       - Pointer to sysHost
  @param[in] Socket     - Socket
  @param[in] Ch         - Channel number
  @param[in] SubCh      - Sub-channel number
  @param[in] Poly       - Pointer of polynomial

  @retval N/A

**/
VOID
SetCadbDselUniseqPoly (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       SubCh,
  IN UINT32      *Poly
  )
{
  UINT8             Index;
  CADB2_REG_CACHE   *CadbCache;
  BOOLEAN           DiffFound = FALSE;
  UINT32            RegList[CADB_MUX_MAX] = {CADB_DSEL_UNISEQ_POLY_0_MCCADB_MAIN_REG,
                                             CADB_DSEL_UNISEQ_POLY_1_MCCADB_MAIN_REG,
                                             CADB_DSEL_UNISEQ_POLY_2_MCCADB_MAIN_REG,
                                             CADB_DSEL_UNISEQ_POLY_3_MCCADB_MAIN_REG};

  if (IsCpgcCacheEnabled()) {

    CadbCache = (CADB2_REG_CACHE *) GetCpgcCacheList (Socket, Ch, SubCh, CADB2_REGISTER_TYPE);

    //
    // Check register cache
    //
    for (Index = 0; Index < CADB_MUX_MAX; Index++) {
      if ((CadbCache->UniseqPoly[Index] != Poly[Index]) && (Poly[Index] != CPGC_DONT_CARE)) {
        //
        // There is something different, need to program registers.
        //
        CadbCache->UniseqPoly[Index] = Poly[Index];
        DiffFound = TRUE;
      }
    }

    if (!DiffFound) {
      //
      // No difference found, return.
      //
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "SetCadbDselUniseqPoly: CADB_DSEL_UNISEQ_POLY_0/1/2/3 are cached = 0x%08x, 0x%08x, 0x%08x, 0x%08x\n",
          CadbCache->UniseqPoly[0],
          CadbCache->UniseqPoly[1],
          CadbCache->UniseqPoly[2],
          CadbCache->UniseqPoly[3]);
      return;
    }
  }

  //
  // write register
  //
  for (Index = 0; Index < CADB_MUX_MAX; Index++) {
    if (Poly[Index] != CPGC_DONT_CARE) {
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegList[Index]), Poly[Index]);
    }
  }
}

/**

  Set the CADB deselect unisequencer pattern buffer

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Ch        - Channel number
  @param[in] SubCh     - Sub-channel number
  @param[in] Patbuf    - Pointer of pattern buffer

  @retval N/A

**/
VOID
SetCadbDselUniseqPat (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubCh,
  IN UINT32     *Patbuf
  )
{
  UINT8             Index;
  CADB2_REG_CACHE   *CadbCache;
  BOOLEAN           DiffFound = FALSE;
  UINT32            RegList[CADB_MUX_MAX] = {CADB_DSEL_UNISEQ_PBUF_0_MCCADB_MAIN_REG,
                                             CADB_DSEL_UNISEQ_PBUF_1_MCCADB_MAIN_REG,
                                             CADB_DSEL_UNISEQ_PBUF_2_MCCADB_MAIN_REG,
                                             CADB_DSEL_UNISEQ_PBUF_3_MCCADB_MAIN_REG};

  if (IsCpgcCacheEnabled()) {

    CadbCache = (CADB2_REG_CACHE *) GetCpgcCacheList (Socket, Ch, SubCh, CADB2_REGISTER_TYPE);

    //
    // Check register cache
    //
    for (Index = 0; Index < CADB_MUX_MAX; Index++) {
      if ((CadbCache->UniseqPat[Index] != Patbuf[Index]) && (Patbuf[Index] != CPGC_DONT_CARE)) {
        //
        // There is something different, need to program registers.
        //
        CadbCache->UniseqPat[Index] = Patbuf[Index];
        DiffFound = TRUE;
      }
    }

    if (!DiffFound) {
      //
      // No difference found, return.
      //
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "SettCadbDselUniseqPat: CADB_DSEL_UNISEQ_PBUF_0/1/2/3 are cached = 0x%08x, 0x%08x, 0x%08x, 0x%08x\n",
          CadbCache->UniseqPat[0],
          CadbCache->UniseqPat[1],
          CadbCache->UniseqPat[2],
          CadbCache->UniseqPat[3]);
      return;
    }
  }

  //
  // write register
  //
  for (Index = 0; Index < CADB_MUX_MAX; Index++) {
    if (Patbuf[Index] != CPGC_DONT_CARE) {
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegList[Index]), Patbuf[Index]);
    }
  }
}

/**

  Set the CADB deselect unisequencer pattern mux and polynomial size for LFSR mux mode

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Ch        - Channel number
  @param[in] SubCh     - Sub-channel number
  @param[in] MuxCtl    - Pointer of mux array
  @param[in] LfsrSize  - Pointer of LFSR size array

  @retval N/A

**/
VOID
SetCadbDselUniseqCfg (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubCh,
  IN UINT32     *MuxCtl,
  IN UINT32     *LfsrSize
  )
{
  CADB_DSEL_UNISEQ_CFG_0_MCCADB_MAIN_STRUCT  CadbDselUniseqCfg;
  UINT32                                     Data32;
  UINT8                                      Index;
  CADB2_REG_CACHE                            *CadbCache;
  BOOLEAN                                    DiffFound = FALSE;
  UINT32                                     RegList[CADB_MUX_MAX] = {CADB_DSEL_UNISEQ_CFG_0_MCCADB_MAIN_REG,
                                                                      CADB_DSEL_UNISEQ_CFG_1_MCCADB_MAIN_REG,
                                                                      CADB_DSEL_UNISEQ_CFG_2_MCCADB_MAIN_REG,
                                                                      CADB_DSEL_UNISEQ_CFG_3_MCCADB_MAIN_REG};

  if (IsCpgcCacheEnabled()) {

    CadbCache = (CADB2_REG_CACHE *) GetCpgcCacheList (Socket, Ch, SubCh, CADB2_REGISTER_TYPE);

    //
    // Check register cache
    //
    for (Index = 0; Index < CADB_MUX_MAX; Index++) {
      if (((CadbCache->UniseqMux[Index] != MuxCtl[Index]) && (MuxCtl[Index] != CPGC_DONT_CARE)) ||
          ((CadbCache->UniseqLfsrSize[Index] != LfsrSize[Index]) && (LfsrSize[Index] != CPGC_DONT_CARE))) {
        //
        // There is something different, need to program registers.
        //
        CadbCache->UniseqLfsrSize[Index] = LfsrSize[Index];
        CadbCache->UniseqMux[Index] = MuxCtl[Index];
        DiffFound = TRUE;
      }
    }

    if (!DiffFound) {
      //
      // No difference found, return.
      //
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "SetCadbDpatUniseqCfg: CADB_DSEL_UNISEQ_CFG_0/1/2/3 are cached, Mux = %d %d %d %d LfsrSize = %d %d %d %d\n",
          CadbCache->UniseqMux[0],
          CadbCache->UniseqMux[1],
          CadbCache->UniseqMux[2],
          CadbCache->UniseqMux[3],
          CadbCache->UniseqLfsrSize[0],
          CadbCache->UniseqLfsrSize[1],
          CadbCache->UniseqLfsrSize[2],
          CadbCache->UniseqLfsrSize[3]);
      return;
    }
  }

  //
  // write register
  //
  for (Index = 0; Index < CADB_MUX_MAX; Index++) {
    if ((MuxCtl[Index] == CPGC_DONT_CARE) && (LfsrSize[Index] == CPGC_DONT_CARE)) {
      continue;
    } else {
      Data32 = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegList[Index]));
      CadbDselUniseqCfg.Data = (Data32 & 0xFF);
      if (LfsrSize[Index] != CPGC_DONT_CARE) {
        CadbDselUniseqCfg.Bits.uniseq_lfsr_polynomial_size = (UINT8) LfsrSize[Index];
      }
      if (MuxCtl[Index] != CPGC_DONT_CARE) {
        CadbDselUniseqCfg.Bits.uniseq_mode = (UINT8) MuxCtl[Index];
      }
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegList[Index]), CadbDselUniseqCfg.Data);
    }
  }
}

/**

  Set CADB buffer

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket
  @param[in] Ch        - Channel number
  @param[in] SubCh     - Sub-channel number
  @param[in] Index     - CADB buffer index
  @param[in] BufLow    - Low 32 bits CADB buffer
  @param[in] BufHigh   - High 32 bits CADB buffer

  @retval N/A

**/
VOID
SetCadbBuf (
  IN PSYSHOST      Host,
  IN UINT8         Socket,
  IN UINT8         Ch,
  IN UINT8         SubCh,
  IN UINT8         Index,
  IN UINT32        BufLow,
  IN UINT32        BufHigh
  )
{
  CADB2_REG_CACHE  *CadbCache;
  UINT32           RegList[CADB2_LINES * 2] = { CADB_BUF_0_N0_MCCADB_MAIN_REG, CADB_BUF_0_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_1_N0_MCCADB_MAIN_REG, CADB_BUF_1_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_2_N0_MCCADB_MAIN_REG, CADB_BUF_2_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_3_N0_MCCADB_MAIN_REG, CADB_BUF_3_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_4_N0_MCCADB_MAIN_REG, CADB_BUF_4_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_5_N0_MCCADB_MAIN_REG, CADB_BUF_5_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_6_N0_MCCADB_MAIN_REG, CADB_BUF_6_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_7_N0_MCCADB_MAIN_REG, CADB_BUF_7_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_8_N0_MCCADB_MAIN_REG, CADB_BUF_8_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_9_N0_MCCADB_MAIN_REG, CADB_BUF_9_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_10_N0_MCCADB_MAIN_REG, CADB_BUF_10_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_11_N0_MCCADB_MAIN_REG, CADB_BUF_11_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_12_N0_MCCADB_MAIN_REG, CADB_BUF_12_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_13_N0_MCCADB_MAIN_REG, CADB_BUF_13_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_14_N0_MCCADB_MAIN_REG, CADB_BUF_14_N1_MCCADB_MAIN_REG,
                                            CADB_BUF_15_N0_MCCADB_MAIN_REG, CADB_BUF_15_N1_MCCADB_MAIN_REG
                                          };
  if (Index >= CADB2_LINES) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
        "Error: Cadb buffer index %d is overflow in SetCadbBuf\n", Index);
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_201);
    return;
  }

  if (IsCpgcCacheEnabled()) {

    CadbCache = (CADB2_REG_CACHE *) GetCpgcCacheList (Socket, Ch, SubCh, CADB2_REGISTER_TYPE);

    if (CadbCache->BufLow[Index] != BufLow) {
      CadbCache->BufLow[Index] = BufLow;
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegList[Index * 2]), BufLow);
    } else {
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "SetCadbBuf: CacheCadbBufLow is cached = 0x%08x\n",
          CadbCache->BufLow[Index]);
    }

    if (CadbCache->BufHi[Index] != BufHigh) {
      CadbCache->BufHi[Index] = BufHigh;
      MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegList[Index * 2 + 1]), BufHigh);
    } else {
      RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
          "SetCadbBuf: CacheCadbBufHi is cached = 0x%08x\n",
          CadbCache->BufHi[Index]);
    }
  } else {
    //
    // write register
    //
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegList[Index * 2]), BufLow);
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegList[Index * 2 + 1]), BufHigh);
  }
}

/**

  Get/Set CADB registers

  @param[in]     Host      - Pointer to sysHost
  @param[in]     Socket    - Socket
  @param[in]     Ch        - Channel number
  @param[in]     SubCh     - Sub-channel number
  @param[in]     RdWrFlag  - Register read/write flag: CPGC_REG_READ, CPGC_REG_WRITE
  @param[in]     RegAddr   - CADB register address
  @param[in out] Data      - Pointer to the data for reading or writing the register

  @retval N/A

**/
VOID
GetSetCadbRegs (
  IN     PSYSHOST     Host,
  IN     UINT8        Socket,
  IN     UINT8        Ch,
  IN     UINT8        SubCh,
  IN     UINT8        RdWrFlag,
  IN     UINT32       RegAddr,
  IN OUT UINT32       *Data
  )
{
  UINT32            *CachedData;

  if (IsCpgcCacheEnabled()) {

    CachedData = GetCadbCacheDataPointer (Socket, Ch, SubCh, RegAddr);

    if (CachedData != NULL) {
      if (RdWrFlag == CPGC_REG_READ) {
        *Data = *CachedData;
        return;
      } else {
        if (*CachedData != *Data) {
          *CachedData = *Data;
        } else {
          RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
            "CsrWrite: Cadb register is cached [0x%08x] = 0x%08x\n", SUBCH_OFFSET_CADB (SubCh, RegAddr), *CachedData);
          return;
        }
      }
    }
  }

  if (RdWrFlag == CPGC_REG_READ) { // read
    *Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegAddr));
  } else { // write
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CADB (SubCh, RegAddr), *Data);
  }
}

/**

  Get/Set CPGC registers

  @param[in]     Host      - Pointer to sysHost
  @param[in]     Socket    - Socket
  @param[in]     Ch        - Channel number
  @param[in]     SubCh     - Sub-channel number
  @param[in]     RdWrFlag  - Register read/write flag: CPGC_REG_READ, CPGC_REG_WRITE
  @param[in]     RegAddr   - CPGC register address
  @param[in out] Data      - Pointer to the data for reading or writing the register

  @retval N/A

**/
VOID
GetSetCpgcRegs (
  IN     PSYSHOST  Host,
  IN     UINT8     Socket,
  IN     UINT8     Ch,
  IN     UINT8     SubCh,
  IN     UINT8     RdWrFlag,
  IN     UINT32    RegAddr,
  IN OUT UINT32    *Data
  )
{
  UINT32            *CachedData;

  if (IsCpgcCacheEnabled()) {

    CachedData = GetCpgcCacheDataPointer (Socket, Ch, SubCh, RegAddr);
    if (CachedData != NULL) {
      if (RdWrFlag == CPGC_REG_READ) {
        *Data = *CachedData;
        return;
      } else {
        if (*CachedData != *Data) {
          *CachedData = *Data;
        } else {
          RcDebugPrintWithDevice (SDBG_CPGC, Socket, Ch, NO_DIMM, SubCh, NO_RANK, NO_STROBE, NO_BIT,
            "CsrWrite: Cpgc register is cache [0x%08x] = 0x%08x\n", SUBCH_OFFSET_CPGC (SubCh, RegAddr), *CachedData);
          return;
        }
      }
    }
  }

  if (RdWrFlag == CPGC_REG_READ) { // read
    *Data = MemReadPciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, RegAddr));
  } else { // write
    MemWritePciCfgEp (Socket, Ch, SUBCH_OFFSET_CPGC (SubCh, RegAddr), *Data);
  }
}