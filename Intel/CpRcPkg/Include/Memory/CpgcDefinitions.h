/** @file
  CpgcDefinitions.h

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

#ifndef _CPGC_DEFINITIONS_H_
#define _CPGC_DEFINITIONS_H_

//
// Definition used with SetupIOTest CPGCAddress parameter
//

typedef struct _TcpgcAddress {
  UINT32 Start[4];                   ///<  (4, uint32)    Rank, Bank, Row, Col
  UINT32 Stop[4];                    ///<  (4, uint32)    Rank, Bank, Row, Col
  UINT8  Order[4];                   ///<  [4, uint8)     Rank, Bank, Row, Col
  UINT32 IncRate[4];                 ///<  (4, unit32)    Rank, Bank, Row, Col
  UINT16 IncVal[4];                  ///<  (4, unit16)    Rank, Bank, Row, Col
} TCPGCAddress, *PTCPGCAddress;

//
// Definition used with SetupIOTest WDBPattern parameter
//

typedef struct TwdbPattern {
  UINT16 IncRate;                    ///< How quickly the WDB walks through cachelines (uint16)
  UINT32 Start;                      ///< Starting pointer in WDB
  UINT32 Stop;                       ///< Stopping pointer in WDB
  UINT8  DQPat;                      ///< [0:BasicVA, 1:SegmentWDB, 2:CADB, 3:TurnAround, 4:LMNVa,
                                     ///<  5:TurnAroundWR, 6:TurnAroundODT, 7:DqAllSamePat, 8:DqAAAAAAAV,
                                     ///<  9: DqAAAVAAAV, 10:DqAVAVAVAV]
  UINT8  DQRotate;                   ///< DQ rotation every "DQRotate" cacheline
} TWDBPattern, *PTWDBPattern;

//
// CPGC Subsequence types     (defined by CPGC_SUBSEQx_CTL_A_MCDDC_CTL_STRUCT.subsequence_type)
//

#define BRd                                 0
#define BWr                                 1
#define BRdWr                               2
#define BWrRd                               3
#define ORd                                 4
#define OWr                                 5

//
// CPGC Subsequence transaction sizes    (defined by CPGC_SUBSEQ0_CTL_B_MCDDC_CTL_STRUCT.request_data_size)
//

#define BURST_SIZE_32B  0
#define BURST_SIZE_64B  1

//
// CPGC Mux Control
//

#define LMNMode                             0
#define BTBUFFER                            1
#define LFSRMode                            2
#define GALOIS_LFSR_MODE                    3

#define RD_DQ_DQS_PATTERN_DDR5  0x1C711C71
#define RD_DQ_DQS_PREDFE_DDR5   0xA5A5A5A5
#define DPAT_UNISEQ_AA55        0xAA55AA55
#define WR_DQ_DQS_PATTERN_DDR5  0x8E388E38
#define DRAM_DCA_PATTERN_DDR5   0xAAAAAAAA
#define RD_DFE_PATTERN_DDR5     0x3C5A3C5A

//
// Margin params
//

/*
 1D Margin Types:
  RcvEn:  Shifts just RcvEn.  Only side effect is it may eat into read dq-dqs for first bit of burst
  RdT:    Shifts read DQS timing, changing where DQ is sampled
  WrT:    Shifts write DQ timing, margining DQ-DQS timing
  WrDqsT: Shifts write DQS timing, margining both DQ-DQS and DQS-CLK timing
  RdV:    Shifts read Vref voltage for DQ only
  WrV:    Shifts write Vref voltage for DQ only
  WrLevel: Shifts write DQ and DQS timing, margining only DQS-CLK timing
  WrTBit:  Shifts write DQ per bit timing.
  RdTBit:  Shifts read DQ per bit timing.
  RdVBit:  Shifts read DQ per bit voltage.

 2D Margin Types (Voltage, Time)
  RdFan2:  Margins both RdV and RdT at { (off, -2/3*off), (off, 2/3*off) }
  WrFan2:  Margins both WrV and WrT at { (off, -2/3*off), (off, 2/3*off) }
  RdFan3:  argins both RdV and RdT at { (off, -2/3*off),  (5/4*off, 0), (off, 2/3*off)  }
  WrFan3:  Margins both WrV and WrT at { (off, -2/3*off), (5/4*off, 0), (off, 2/3*off)  }
*/

#define RcvEna                              0
#define RdT                                 1
#define WrT                                 2
#define WrDqsT                              3
#define RdV                                 4
#define WrV                                 5
#define WrLevel                             6

#define WrTBit                              8
#define RdTBit                              9
#define RdVBit                              10

#define RdFan2                              12
#define WrFan2                              13
#define RdFan3                              14
#define WrFan3                              15

#define DDR_CPGC_DQ_PATTERN         0
#define DDR_CPGC_ADV_CMD_PATTERN    1
#define DDR_CPGC_CMD_G2_PATTERN     2
#define DDR_CPGC_CMD_G3MPR_PATTERN  3
#define DDR_CPGC_MEM_TEST_PATTERN   4
#define DDR_CPGC_PATTERN_MASK       (BIT0 | BIT1 | BIT2 | BIT3)
#define DDR_CPGC_MEM_TEST_READ      BIT8
#define DDR_CPGC_MEM_TEST_WRITE     BIT9
#define DDR_CPGC_MEM_TEST_INVERTPAT BIT10
#define DDR_CPGC_MEM_TEST_ALL0      BIT11
#define DDR_CPGC_MEM_TEST_A5        BIT12
#define DDR_CPGC_PPR_TEST           BIT13

//
// CPGC configuration training defines
//

#define CPGC_UPDATE_BY_CL        0 // 0 means that the four Address Update Rate fields
                                   // (Rank, Bank, Row, and Column) are updated based on the # of read and
                                   // write cacheline transactions.

#define CPGC_UPDATE_BY_LOOP_CNT  1 // means that the four Address Update Rate fields
                                   // (Rank, Bank, Row, and Column) are updated by based on the Loopcount
                                   // (# of sequences perfromed).

#define CPGC_BASE_READ_SUBSEQ          0 // 0000: Base read
#define CPGC_BASE_WRITE_SUBSEQ         1 // 0001: Base Write
#define CPGC_BASE_READ_WRITE_SUBSEQ    2 // 0002: Base read followed by write
#define CPGC_BASE_WRITE_READ_SUBSEQ    3// 0003: Base Write followed by read
#define CPGC_BASE_WR_SUBSEQ            CPGC_BASE_WR_SUBSEQ_CHIP
#define CPGC_BASE_RW_SUBSEQ            CPGC_BASE_RW_SUBSEQ_CHIP
#define CPGC_BASE_OFFSET_READ_SUBSEQ   4 // 0100: Offset Read
#define CPGC_BASE_OFFSET_WRITE_SUBSEQ  5 // 0101: Offset Write
#define CPGC_SUBSEQ_WAIT_DEFAULT       4 // subseq wait to allow credits to drain
#define CPGC_SUBSEQ_WAIT_DDRT          240
#define CPGC_SUBSEQ_WAIT_DDRT2         512
#define CPGC_SUBSEQ_WAIT_BRS_WL        40 //Maintain the spacing between non back-to-back WRs

#define CPGC_NO_SUBSEQ1                0xFF  // do not use subseq1

//
// WDB control
//

#define CPGC_WDB_MUX_CTL_LMN                0           // LMN counter
#define CPGC_WDB_MUX_CTL_PATBUF             1           // pattern buffer
#define CPGC_WDB_MUX_CTL_LFSR24             2           // LFSR24
#define CPGC_WDB_MUX_CTL_LFSR24_PATBUF      3           // LFSR + Pattern Buffer

#define CPGC0_LFSR0_INIT_SEED_HSX  0x00FFFF00
#define CPGC0_LFSR1_INIT_SEED_HSX  0x0000FFFF
#define CPGC0_LFSR2_INIT_SEED_HSX  0x00000000
#define CPGC1_LFSR0_INIT_SEED_HSX  0x00555000
#define CPGC1_LFSR1_INIT_SEED_HSX  0x00000555
#define CPGC1_LFSR2_INIT_SEED_HSX  0x00000000
#define CPGC_LFSR0_INIT_SEED_HSX(scheduler) ((scheduler == 0) ? CPGC0_LFSR0_INIT_SEED_HSX:CPGC1_LFSR0_INIT_SEED_HSX)
#define CPGC_LFSR1_INIT_SEED_HSX(scheduler) ((scheduler == 0) ? CPGC0_LFSR1_INIT_SEED_HSX:CPGC1_LFSR1_INIT_SEED_HSX)
#define CPGC_LFSR2_INIT_SEED_HSX(scheduler) ((scheduler == 0) ? CPGC0_LFSR2_INIT_SEED_HSX:CPGC1_LFSR2_INIT_SEED_HSX)

//
// MAX threshold to consider cpgc errors as transient
//

#define CPGC_ERR_THRESHOLD 1

//
// Data Pattern Configuration
//

typedef union {
  struct {
    UINT32   seqMode0: 2;
    UINT32   seqMode1: 2;
    UINT32   seqMode2: 2;
    UINT32   eccDis: 1;
    UINT32   eccReplaceByteControl: 1;
    UINT32   eccDataSourceSel: 1;
    UINT32   saveLfsrSeedRate: 5;
    UINT32   reloadLfsrSeedRate: 8;
    UINT32   rsvd: 10;
  } Bits;
  UINT32  Data;
} SequencerMode;


#define MT_MAX_SUBSEQ       2
#define MT_PATTERN_DEPTH    16

#define MT_CPGC_WRITE       0
#define MT_CPGC_READ_WRITE  1
#define MT_CPGC_READ        2

#define MT_ADDR_DIR_UP      0
#define MT_ADDR_DIR_DN      1

//
// CPCG subsequencer type setup
//
typedef struct {
  UINT8 SubSequenceStart;
  UINT8 SubSequenceEnd;
  UINT8 SubSequenceType[2];  // Update to two subsequencer used by the CPGC_GLOBAL_TRAINING_SETUP
} CPGC_SUBSEQ_SETUP;

#endif // #ifndef _CPGC_DEFINITIONS_H_
