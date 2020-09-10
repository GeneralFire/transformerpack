/** @file
  Interface for RAS debug library.

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

#ifndef __RAS_DEBUG_LIB_H__
#define __RAS_DEBUG_LIB_H__

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>


#define LEVEL_BASIC_FLOW    BIT26  //basic process flow.
#define LEVEL_FUNC_FLOW     BIT27  //function level flow
#define LEVEL_REG           BIT28  //register level or status
#define MCE_PRINT           BIT29  //for MCE procedure usage
#define MP_PRINT            BIT30  //for AP procedure usage.

#define RAS_INFO_STR(module)  "[RAS] (" module ") "
#define RAS_WARN_STR(module)  "[RAS] (" module ") WARNING: "
#define RAS_ERROR_STR(module) "[RAS] (" module ") ERROR: "


/**
  A service function provided to RT driver. Prints a string

  @param  ComponentId         Specifies the component callint this routine.
  @param  Level               Specifies the severity of the message being sent.
  @param  String              Specifies a pointer to the format string to print.

  @retval (VOID)

**/
typedef
UINT64
(EFIAPI *CORE_PRINT_FUNC) (
  IN UINT64 CompId,
  IN UINT64 Level,
  IN CHAR8 *String,
  ...
  );


/**
  This procedue is to using OS print routine for RAS code debugging. This is dfx function.
  For example, BIOS serial port print can't show in windbg program. Only windows self-build print routine could show on windbg through serial port.
  Hence we need to use this routine to pass OS self-build print to make BIOS code print on windbg program.

  @param [in]  OsPrint -- windows print function address

  @retval none.

**/
VOID
EFIAPI
SaveOsPrint (
  IN  CORE_PRINT_FUNC   OsPrint
  );


/**
  Prints a debug message to the debug output device if the specified error level is enabled.

  If any bit in ErrorLevel is also set in DebugPrintErrorLevelLib function
  GetDebugPrintErrorLevel (), then print the message specified by Format and the
  associated variable argument list to the debug output device.

  If MP_PRINT flag in ErrorLevel is set, this function API needs to consider MP safety
  for each output.

  If Format is NULL, then ASSERT().

  @param  ErrorLevel  The error level of the debug message.
  @param  Format      The format string for the debug message to print.
  @param  ...         The variable argument list whose contents are accessed
                      based on the format string specified by Format.

**/
VOID
EFIAPI
RasDebugPrint (
  IN  UINTN        ErrorLevel,
  IN  CHAR8       *Format,
  ...
  );
//APTIOV_SERVER_OVERRIDE_RC_START : Added to avoid printing of Ras Debug messages in release mode after a reset
#ifdef EFI_DEBUG
//APTIOV_SERVER_OVERRIDE_RC_END : Added to avoid printing of Ras Debug messages in release mode after a reset
#define RAS_DEBUG(Expression) RasDebugPrint Expression
//APTIOV_SERVER_OVERRIDE_RC_START : Added to avoid printing of Ras Debug messages in release mode after a reset
#else
#define RAS_DEBUG(Expression) 
#endif 
//APTIOV_SERVER_OVERRIDE_RC_END : Added to avoid printing of Ras Debug messages in release mode after a reset

/**
  Macro that print error location, then halt CPU  if an expression evaluates to FALSE.
  RE-invent this macro is because knockworks will give an error on ASSERT.

  @param  Expression  Boolean expression.
**/
#define RAS_ASSERT(Expression) \
  do { \
    if (DebugAssertEnabled ()) { \
      if (!(Expression)) { \
        RAS_DEBUG ((LEVEL_FUNC_FLOW, "RAS ASSERT ERROR found -- %a line %d!\n", __FILE__, __LINE__)); \
        CpuDeadLoop (); \
      } \
    } \
  } while (FALSE)

#endif
