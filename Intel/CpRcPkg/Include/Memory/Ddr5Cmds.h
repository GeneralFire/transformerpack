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

#ifndef  _ddr5cmds_h
#define  _ddr5cmds_h
//
//MPC Command Definition
//
#define MPC(OP)                (0x0F | ( OP << 5))
#define MPC1(OP)               (0x0F | ( (OP & 0x3) << 5))
#define MPC2(OP)               (( OP & 0xFC) >> 2)
//
//MPC Opcodes
//
#define Exit_CS_Training_Mode                       0x0  //0000 0000b
#define Enter_CS_Training_Mode                      0x01 //0000 0001b
#define DLL_RESET                                   0x02 //0000 0010b
#define Enter_CA_Training_Mode                      0x03 //0000 0011b
#define ZQCAL_LATCH                                 0x04 //0000 0100b
#define ZQCAL_START                                 0x05 //0000 0101b
#define Stop_DQS_Osc                                0x06 //0000 0110b
#define Start_DQS_Osc                               0x07 //0000 0111b
#define SET_2N_CMD_TIMING                           0x08 //0000 1000b
#define SET_1N_CMD_TIMING                           0x09 //0000 1001b
#define EXIT_PDA_ENUMERATE_PROGRAM_MODE             0x0A //0000 1010b
#define ENTER_PDA_ENUMERATE_PROGRAM_MODE            0x0B //0000 1011b
#define Manual_ECS_Operation                        0x0C //0000 1100b
#define APPLY_VREFCA_RTT_CA_CS_CK                   0x1F //0001 1111b
#define GROUP_A_RTT_CK                              0x20 //0010 0xxxb, xxx map to MR32[2:0] ODT Setting
#define GROUP_B_RTT_CK                              0x28 //0010 1xxxb, xxx map to MR32[2:0] ODT Setting
#define GROUP_A_RTT_CS                              0x30 //0011 0000b, xxx map to MR32[5:3] ODT Setting
#define GROUP_B_RTT_CS                              0x38 //0011 1xxxb, xxx map to MR32[5:3] ODT Setting
#define GROUP_A_RTT_CA                              0x40 //0100 0xxxb, xxx map to MR33[2:0] ODT Setting
#define GROUP_B_RTT_CA                              0x48 //0100 1xxxb, xxx map to MR33[2:0] ODT Setting
#define DQS_RTT_PARK                                0x50 //0101 0xxxb, xxx map to MR33[5:3] ODT Setting
#define RTT_PARK                                    0x58 //0101 1xxxb, xxx map to MR34[2:0] ODT Setting
#define PDA_ENUMERATE_ID                            0x60 //0110 xxxxb, xxxx = PDA Enumerate ID
#define PDA_SELECT_ID                               0x70 //0111 xxxxb, xxxx = PDA Select ID = xxxx
#define PDA_SELECT_ALL                              0x7F //PDA Select ID = 1111 (Enable all DRAMs to execute all commands)
#define Configure_tDLLK_tCCD_L                      0x80 //1000 xxxxb, xxxx map to MR13[3:0] tCCD_L / tCCD_L_WR / tDLLK


#define VREFCA1(OP)               (0x03 | ( (OP & 0x3) << 5))
#define VREFCA2(OP)               (( OP & 0x7C) >> 2)

#define VREFCS1(OP)               (0x03 | ( (OP & 0x3) << 5))
#define VREFCS2(OP)               ((( OP & 0x7C) | (0x80)) >> 2)

#ifdef USE_LATEST_DRAM_SPEC
  //
  //VrefCA Command Definition in two pass mode
  //
  #define VrefCA(OP)                (0x03 | ( OP << 5))

  //
  //VrefCA Command Definition in one pass mode
  //
  #define VrefCA2(OP)                ((OP & 0x78) >> 3)
#else
  //
  //VrefCA Command Definition in two pass mode
  //
  #define VrefCA(OP)                (0x03 | ( (OP & 0x7f) << 5))

  //
  //VrefCA Command Definition in one pass mode
  //
  #define VrefCA2(OP)                ( (OP & 0x7c) >> 2)
#endif

//
//VrefCA Command Definition in one pass mode
//
#define VrefCA1(OP)                (0x03 | ( (OP & 0x07) << 5))

//
//VrefCS Command Definition in two pass mode
//
#define VrefCS(OP)                (0x03 | ( (OP | 0x80) << 5))

//
//VrefCS Command Definition in one pass mode
//
#define VrefCS1(OP)                (0x03 | ( (OP & 0x07) << 5))
#define VrefCS2(OP)                (((OP & 0x78) | 0X80) >> 3)

//
//Vref CA/CS Mode
//
#define CA_MODE 0
#define CS_MODE 1



//
//MRR Command Defintion
//
#define MRRL(VALUE, ADD)                   ((VALUE & 0x2000) | (0x15 | (ADD << 5)))
#define MRRH(VALUE, CW)                    ((VALUE & 0x3BFC) | (CW << 10))
//
//MRW Command Defintion with non-RCD DIMM (UDIMM, SODIMM)
//
#define MRWL(VALUE, ADD)                   ((VALUE & 0x2000) | (0x05 | (ADD << 5)))
#define MRWH(VALUE, OP, CW)                ((VALUE & 0x3B00) | (OP | (CW << 10)))

//
//MRW Command Defintion with RCD DIMM (RDIMM, LRDIMM)
//
#define MRWLL(VALUE, ADD)                  (MRWL(VALUE, ADD) & 0x7F)            //Cmd first UI low 7 bits
#define MRWLH(VALUE, ADD)                  ((MRWL(VALUE, ADD) >> 7) & 0x7F)     //Cmd first UI high 7 bits
#define MRWHL(VALUE, OP, CW)               (MRWH(VALUE, OP, CW) & 0x7F)         //Cmd second UI low 7 bits
#define MRWHH(VALUE, OP, CW)               ((MRWH(VALUE, OP, CW) >> 7) & 0x7F)  //Cmd second UI high 7 bits
#define MRW1(ADD)                   (0x05 | ( (ADD&0x03) << 5))
#define MRW2(ADD)                   (((ADD&0xFC) >> 2))
#define MRW3(OP)                    ((OP&0x7F))
#define MRW4(OP, CW)                ((((OP&0x80) >> 7) | ( CW << 3)))

//
//MRW Command Defintion in 2N mode
//
#define MRWL2N0(VALUE, ADD)                   ((0x05 | ( (ADD & 0x03) << 5))) | (((0x05 | ( (ADD & 0x03) << 5))) << 7)
#define MRWL2N1(VALUE, ADD)                   ((ADD & 0xFC) >> 2) | (((ADD & 0xFC) >> 2) << 7)
#define MRWH2N0(VALUE, OP, CW)                (OP & 0x7F) | ((OP & 0x7F) << 7)
#define MRWH2N1(VALUE, OP, CW)                (((OP & 0x80) >> 3) | ( CW << 3)) | ((((OP & 0x80) >> 3) | ( CW << 3)) << 7)

//
//MRW Command Defintion in 1N mode for DDR5 RDIMM.
//
#define MRWL1N0(ADD)                   ((0x05 | ( (ADD & 0x03) << 5))) | (((0x05 | ( (ADD & 0x03) << 5))) << 7)
#define MRWL1N1(ADD)                   ((ADD & 0xFC) >> 2) | (((ADD & 0xFC) >> 2) << 7)
#define MRWH1N0(OP, CW)                (OP & 0x7F) | ((OP & 0x7F) << 7)
#define MRWH1N1(OP, CW)                (((OP & 0x80) >> 7) | ( CW << 3)) | ((((OP & 0x80) >> 7) | ( CW << 3)) << 7)

//
// NOP Command
//
#define NOP_Command                         0x1F //NOP Command

//
// Precharge Command
//
#define PREabL          (0x0B | BIT6)                             // First half of Precharge All Command
#define PREabH          (BIT0 | BIT1 | BIT2)                      // Second half of Precharge all Command
#define PREsbL(BK)      (0x0B | ((BK & BIT0) << 6))               // First half of Precharge Same Bank Command
#define PREsbH(BK)      (BIT1 | BIT2 | BIT3 | ((BK >> 1) & BIT0)) // Second half of Precharge Same Bank Command
#define PREpbL(BK, BG)  (0x1B | ((BK & BIT0) << 6))               // First half of Precharge Command
#define PREpbH(BK, BG)  (((BK >> 1) & BIT0) | ((BG & 0x7) << 1))  // Second half of Precharge Command

#define TIMING_SINGLE_CS                    0 // Single CS Assertion Timing Mode
#define TIMING_MULTI_CS                     1 // Multi CS Assertion Timing Mode
#define TIMING_4N                           4 // 4N Timing Mode
#endif   // _ddr5regs_h
