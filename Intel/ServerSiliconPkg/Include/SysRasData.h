/** @file
  Interface of Ras Last Boot Error.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#include <ReferenceCodeDataTypes.h>

//
// System previous boot error structure
//
#define MC_BANK_STATUS_REG    1
#define MC_BANK_ADDRESS_REG   2
#define MC_BANK_MISC_REG      3

#define MSR_LOG_VALID   BIT31
#define MSR_LOG_UC      BIT29
#define MSR_LOG_EN      BIT28

#define MAX_PREV_BOOT_ERR_ENTRIES   15

struct McBankTableEntry {
  UINT8   McBankNum;
  UINT16  McBankAddress;
};

struct McBankStruct {
  UINT8           SocketId;
  UINT8           McBankNum;
  UINT64_STRUCT   McStatus;
  UINT64_STRUCT   McAddress;
  UINT64_STRUCT   McMisc;
};

struct PrevBootErrInfo {
  UINT32 ValidEntries;
  struct McBankStruct McRegs[MAX_PREV_BOOT_ERR_ENTRIES];
};

//
// Ras Data structure
//
typedef struct SysRasData {
  struct PrevBootErrInfo PrevBootErr;
}
SYSRASDATA, *PSYSRASDATA;
