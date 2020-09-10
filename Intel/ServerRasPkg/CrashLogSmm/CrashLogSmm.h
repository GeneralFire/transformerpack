/** @file
  Header file for CrashLog support.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#ifndef _CRASH_LOG_SMM_H_
#define _CRASH_LOG_SMM_H_

//
// Data definitions & structures
//

#pragma pack(push, 1)

//
// CRASHLOG_REASON
//
typedef union {
  struct {
    UINT32  CWReset    : 1;  // Cold/Warm reset
    UINT32  GReset     : 1;  // Global reset
    UINT32  Error      : 1;  // Error
    UINT32  MInit      : 1;  // Manually Initiated
    UINT32  Reserved   : 28;
  } Bits;
  UINT32  Uint32;
} CRASHLOG_REASON;

//
// CRASHLOG FIELD
//
typedef struct {
  UINT32            CrashLogVersion;
  CRASHLOG_REASON   CrashLogReason;
} CRASHLOG_FIELD;

#pragma pack(pop)

#endif  // #ifndef _CRASH_LOG_SMM_H_
