/** @file
  Basic reference code data types Header File

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _REFERENCE_CODE_DATA_TYPES_H_
#define _REFERENCE_CODE_DATA_TYPES_H_

//
// All the common UEFI data types such as UINT8
// come from Base.h
//

#include <Base.h>

//
// Ref code uses UINT64_STRUCT rather than
// UINT64 for historical reasons. Should
// be replaced at some point.
//

typedef struct _UINT64_STRUCT {
  UINT32  lo;
  UINT32  hi;
} UINT64_STRUCT, *PUINT64_STRUCT;

//
// This also could be replaced by UINT64
// in the future.
//

typedef union _UINT64_DATA {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} UINT64_DATA;

typedef struct _UINT128 {
  UINT32  one;
  UINT32  two;
  UINT32  three;
  UINT32  four;
} UINT128;

#endif // #ifndef _REFERENCE_CODE_DATA_TYPES_H_
