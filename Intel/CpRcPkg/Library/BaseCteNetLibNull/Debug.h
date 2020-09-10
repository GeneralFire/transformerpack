/** @file
  Debug header file for BaseCteNetLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2016 Intel Corporation. <BR>

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

#ifndef _DEBUG_H_
#define _DEBUG_H_

//
// Instrumentation for error paths.
//

#undef DPRINTF_ERROR

#if defined DEBUG_ERROR_OUTPUT && DEBUG_ERROR_OUTPUT == 1
#define DPRINTF_ERROR(...) DEBUG((DEBUG_ON, "ERROR. " __FUNCTION__ ": " __VA_ARGS__))
#else
#define DPRINTF_ERROR(...)
#endif

//
// Instrumentation for initialization path.
//

#undef DPRINTF_INIT
#undef DEBUG_INIT
#undef ASSERT_INIT

#if defined D_FUNC_NAME && D_FUNC_NAME == 1
#define DPRINTF_INIT(...)   DEBUG((DEBUG_ON, __FUNCTION__ ": " __VA_ARGS__))
#define DEBUG_INIT          DEBUG_CODE
#define ASSERT_INIT         ASSERT
#else
#define DPRINTF_INIT(...)
#define DEBUG_INIT
#define ASSERT_INIT
#endif

#if defined D_TRACE_ENTER && D_TRACE_ENTER == 1
#define TRACE_ENTER()   DEBUG((DEBUG_ON, __FUNCTION__ ": Enter\n"))
#else
#define TRACE_ENTER()
#endif

#if defined D_TRACE_EXIT && D_TRACE_EXIT == 1
#define TRACE_EXIT()    DEBUG((DEBUG_ON, __FUNCTION__ ": Exit\n"))
#else
#define TRACE_EXIT()
#endif

//
// If x does not equal EFI_SUCCESS, this macro
// will print a debug message and assert if this
// is a debug build. If x does not equal EFI_SUCCESS
// and this is not a debug build, this macro will
// return EFI_STATUS x.
//

#define EFI_ERROR_RETURN_OR_ASSERT(x,...)       \
  if (EFI_ERROR(x)) {                           \
    DPRINTF_ERROR(__VA_ARGS__);                 \
    ASSERT_EFI_ERROR(x);                        \
    return (x);                                 \
  }

//
// If x equals NULL, this macro will print a debug
// message and assert on EFI_STATUS y if this
// is a debug build. If x equals NULL and this is
// not a debug build, this macro will return EFI_STATUS y.
//

#define CHECK_NULL_RETURN_OR_ASSERT(x, y, ...)  \
  if (x == NULL) {                              \
    DPRINTF_ERROR(__VA_ARGS__);                 \
    ASSERT_EFI_ERROR(y);                        \
    return (y);                                 \
  }

#endif // _DEBUG_H_

