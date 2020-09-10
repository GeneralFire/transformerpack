/** @file
  Interface of memory function library.

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
#ifndef __MEM_FUNC_LIB__
#define __MEM_FUNC_LIB__

#define   DEVICE_NUMBER8             8
#define   DEVICE_NUMBER16            16
#define   MAX_ADDDC_REGION           4
#ifdef DDR5_SUPPORT
#define   ADDDC_SPARE_DEVICE         9
#define   ADDDC_DEVICE_SWITCH_NUMBER 10
#else
#define   ADDDC_SPARE_DEVICE         17
#define   ADDDC_DEVICE_SWITCH_NUMBER 18
#endif
#define   RETRY_ECC_MODE_SDDC_2LM 0
#define   RETRY_ECC_MODE_SDDC_1LM 1
#define   RETRY_ECC_MODE_SDDC_PLUSONE_2LM 2
#define   RETRY_ECC_MODE_SDDC_PLUSONE_1LM 3
#define   RETRY_ECC_MODE_ADDDC_2LM 4
#define   RETRY_ECC_MODE_ADDDC_1LM 5
#define   RETRY_ECC_MODE_ADDDC_PLUSONE_2LM 6
#define   RETRY_ECC_MODE_ADDDC_PLUSONE_1LM 7
#define   RETRY_ECC_MODE_DDRT 8
#define   RETRY_ECC_MODE_X8_SDDC 9

typedef enum {
  ErrInIndReg              = 0,
  ErrInAdddcRankFailureReg = 1,
  ErrInAdddcRankBuddyReg   = 2,
  ErrInAdddcBankFailureReg = 3,
  ErrInAdddcBankBuddyReg   = 4,
  ErrInSVLSFailureReg      = 5,
  ErrInSVLSBuddyReg        = 6,
  MaxErrType
} FAILURE_REGION_TYPE;

#pragma  pack(1)
typedef struct {
  UINT8   RegionType;
  UINT8   Node;
  UINT8   NodeCh;
  UINT8   FailedRank;
  UINT8   FailedSubRank;
  UINT8   FailedDimm;
  UINT8   FailedDev;
  UINT8   FailedBank;
  UINT8   FailedBG;
  UINT32  FailedRow;
  UINT32  FailedCol;
  UINT8   PeerRank; //added for ADDDC case.
  UINT8   Transfer; //burst index in cacheline
  UINT8   Persistent; //HBM BIST
  UINT8   SingleBit;
  UINT8   Mode;
  UINT16  InterLocSyn;
} FAILED_DIMM_INFO;
#pragma  pack()


EFI_STATUS
EFIAPI
GetChannelErrorInfo (
  IN      UINT8               Node,
  IN      UINT8               ChOnNode,
  IN      UINT64              SystemAddress,
  IN OUT  FAILED_DIMM_INFO   *pFailedDimmInfo
);

#endif
