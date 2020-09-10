/** @file
  Internal header file for the Compressed Variable library class.

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

#ifndef _COMPRESSED_VARIABLE_LIB_INTERNAL_H_
#define _COMPRESSED_VARIABLE_LIB_INTERNAL_H_

//
// Value to split data at when saving to avoid
// hitting the maximum variable size. Emperically
// determined to work over many generations in the
// reference code, be wary of changing this value to
// anything larger.
//

#define VARIABLE_SPLIT_SIZE 0xF000

//
// Set to 1 to turn on debug output from
// PEI and DXE compressed variable lib instances
//

#define COMPRESSED_VAR_DEBUG_ERROR  0
#define COMPRESSED_VAR_DEBUG_INFO   0

//
// Instrumentation for ERROR path. Strings will
// be omitted unless COMPRESSED_VAR_DEBUG is set to 1
//

#undef COMPRESSED_VAR_ERROR

#if defined COMPRESSED_VAR_DEBUG_ERROR && COMPRESSED_VAR_DEBUG_ERROR == 1

#define COMPRESSED_VAR_ERROR(...)     DEBUG((DEBUG_ERROR, __FUNCTION__ )); DEBUG((DEBUG_ERROR, __VA_ARGS__))

#else

#define COMPRESSED_VAR_ERROR(...)

#endif

//
// Instrumentation for INFO path. Strings will
// be omitted unless COMPRESSED_VAR_DEBUG is set to 1
//

#undef COMPRESSED_VAR_INFO

#if defined COMPRESSED_VAR_DEBUG_INFO && COMPRESSED_VAR_DEBUG_INFO == 1

#define COMPRESSED_VAR_INFO(...)        DEBUG((DEBUG_ERROR, __FUNCTION__ )); DEBUG((DEBUG_ERROR, __VA_ARGS__))

#else

#define COMPRESSED_VAR_INFO(...)

#endif

#endif // #ifndef _COMPRESSED_VARIABLE_LIB_INTERNAL_H_
