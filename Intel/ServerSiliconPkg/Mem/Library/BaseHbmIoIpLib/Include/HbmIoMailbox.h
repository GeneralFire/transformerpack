/** @file
  Header file for Gfsp HBMIO Training for SPR HBMs

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018-2019 Intel Corporation.

  This software and the related documents are Intel copyrighted materials, and
  your use of them is governed by the express license under which they were provided
  to you ("License"). Unless the License provides otherwise, you may not use, modify,
  copy, publish, distribute, disclose or transmit this software or the related documents
  without Intel's prior written permission.

  This software and the related documents are provided as is, with no express or implied
  warranties, other than those that are expressly stated in the License.

  @par Specification
**/

#ifndef _HBM_IO_MAILBOX_H_
#define _HBM_IO_MAILBOX_H_

#define BIT_SET 1
#define BIT_CLR 0

#define HBMIO_MBOX_CTL_DATA0_OFFSET 0x7824
#define HBMIO_MBOX_CTL_DATA1_OFFSET 0x7828

//Define Timeouts for each HBMIO Mailbox function
#define HBMIO_MAILBOX_TIMEOUT   100000 //100ms
#define HBMIO_POLLING_INTERVAL  10     //poll every 10us

//
// Define HBMIO running mode
//
typedef enum {
  HBMIO_RUNNING_MODE_EMULATION_TEST     = 0,  // opcode order: 0x0->0x1->0x2->0x3->  (skip all training: 0x0B->0xC->0xD->0xE->0xF)->0x04->0x5
  HBMIO_RUNNING_MODE_FULL_PROD_TRAINING,      // opcode order: 0x0->0x1->0x2->0x3->  0x0B->0xC->0xD->0xE->0xF                     ->0x04->0x5
  HBMIO_RUNNING_MODE_PROD_TRAINING,           // opcode order: 0x14     only use one opcode
  HBMIO_RUNNING_MODE_FULL_HVM_SORT,           // opcode order: 0x0->0x1->0x2->0x3->  0x10->0xF                                    ->0x04->0x5
  HBMIO_RUNNING_MODE_FULL_HVM_CLASS,          // opcode order: 0x0->0x1->0x2->0x3->  0x0B->0xC->0xD->0xE->0xF                     ->0x04->0x5
  HBMIO_RUNNING_MODE_MAX
} HBMIO_RUNNING_MODE;

//
// Define return codes
//
typedef enum {
  HBM_MBX_NO_ERROR = 0,
  HBM_MBX_INV_OP = 1,
  HBM_MBX_FW_CHK_FAIL = 2,
  HBM_MBX_TRAINING_FAIL = 3,
  HBM_MBX_WATCHDOG_TMOUT = 4,
  HBM_MBX_BAD_FW = 7,
  HBM_MBX_POLLING_TMOUT = 5  //Internal error code to reflect a polling timeout
} HBMIO_RETURN_STATUS;

typedef enum  {
  HbmTrainTxPiStart,     //  0
  HbmTrainTxPiStep,      //  1
  HbmTrainTxPiRange,     //  2
  HbmTrainRxPiStart,     //  3
  HbmTrainRxPiStep,      //  4
  HbmTrainRxPiRange,     //  5
  HbmTrainDeskewStart,   //  6
  HbmTrainDeskewStep,    //  7
  HbmTrainDeskewRange,   //  8
  HbmTrainVrefStart,     //  9
  HbmTrainVrefStep,      // 10
  HbmTrainVrefRange,     // 11
  HbmTrainWriteLatency,  // 12
  HbmTrainReadLatency,   // 13
  HbmTrainParityLatency, // 14
  HbmTrainMinEyewidth,   // 15
  HbmTrainMax            // invalid max
}  HBM_TRAIN_PARAM;

//
// Mailbox menu parameter is 16-bit wide
// Interpretation depends on mailbox command
//
typedef union {
  struct {
    UINT32 SkipAllInternal   :  1; //1 Will Skip All Other Mailbox Functions
    UINT32 Rsvd              : 15;
  } Bits;
  UINT32 Data;
} MBOX_PARAM_INITIALIZE;

typedef union {
  struct {
    UINT32 Enable :  1;
    UINT32 Rsvd   : 15;
  } Bits;
  UINT32 Data;
}  MBOX_PARAM_DLL_CTRL;

typedef union {
  struct {
    UINT32 DeAssert          :  1;
    UINT32 Bypass500UsDelay  :  1;
    UINT32 Rsvd              : 14;
  } Bits;
  UINT32 Data;
} MBOX_PARAM_RESET_WRST_N_CTRL;

typedef union {
  struct {
    UINT32 ChannelEnable  :  8;
    UINT32 Rsvd           :  8;
  } Bits;
  UINT32 Data;
} MBOX_PARAM_HBM_PHY_CONFIG;

typedef union {
  struct {
    UINT32 MsbSel         :  1;
    UINT32 Rsvd           : 15;
  } Bits;
  UINT32 Data;

} MBOX_PARAM_IEEE1500_READ_DEVICE_ID;

typedef union {
  struct {
    UINT32 DataIndex  :  8; // 0-255 Data Index
    UINT32 Rsvd       :  7;
    UINT32 IsData     :  1; // 0 = Command, 1 = Data
  } Bits;
  UINT32 Data;
} MBOX_PARAM_IEEE1500_GENERIC;

typedef union {
  struct {
    UINT32 IsMisr  :  1; // 0 = Lfsr, 1 = Misr
    UINT32 Rsvd    : 15;
  } Bits;
  UINT32 Data;
} MBOX_PARAM_AWROD_LFSR_TRAINING;

typedef union {
  struct {
    UINT32 IsMisr  :  1; // 0 = Lfsr, 1 = Misr
    UINT32 Rsvd    : 15;
  } Bits;
  UINT32 Data;
} MBOX_PARAM_DWORD_LFSR_WRITE_TRAINING;

typedef union {
  struct {
    UINT32 DwordPatternCfg   :  4;
    UINT32 Rsvd0             :  3;
    UINT32 IsRdqs            :  1; // 0 = Wdqs, 1 = Rdqs
    UINT32 BurstIdleCfg      :  8;
  } Bits;
  UINT32 Data;
} MBOX_PARAM_ACIOLB;

typedef union {
  struct {
    HBM_TRAIN_PARAM      ParamName :  5;
    UINT32               ParamVal  :  7;
    UINT32               Rsvd      :  4;
  } Bits;
  UINT32 Data;
} MBOX_PARAM_PICODE_SWEEP_SETUP;

#endif
