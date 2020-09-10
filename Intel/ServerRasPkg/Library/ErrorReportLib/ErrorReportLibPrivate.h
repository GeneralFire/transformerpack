/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef _ERROR_REPORT_LIB_PRIVATE_H_
#define _ERROR_REPORT_LIB_PRIVATE_H_

//
// Includes
//
#include <Guid/ErrorReportLibListener.h>
#include <PCIeErrorTypes.h>

typedef struct {
  UINT32  RegEax;
  UINT32  RegEbx;
  UINT32  RegEcx;
  UINT32  RegEdx;
} EFI_CPUID_REGISTER;

#define MEM_ERROR_RECORD_BUFFER_NUM 64
//
// ECMA Memory Error Record List
//
typedef struct {
  UINT8                  ErrorNum;
  EMCA_MEM_ERROR_RECORD  MemErrorRecordBuffer[MEM_ERROR_RECORD_BUFFER_NUM];
} EMCA_MEM_ERROR_RECORD_LIST;

#define FIRMWARE_ERROR_RECORD_REVISION  2 // version of the firmware error record

#endif  //_ERROR_REPORT_LIB_PRIVATE_H_

