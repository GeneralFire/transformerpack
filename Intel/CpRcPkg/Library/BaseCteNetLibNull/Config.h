/** @file
  Configuration Header file for BaseCteNetLib.

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

#ifndef _CONFIG_H_
#define _CONFIG_H_

//
// All configuration items for this driver
// are be placed here.
//

#ifndef D_FUNC_NAME
#define D_FUNC_NAME         0     // Debug output with function name decoration.
#endif

#ifndef DEBUG_ERROR_OUTPUT
#define DEBUG_ERROR_OUTPUT  0     // Debug output on error with function name decoration.
#endif

#ifndef D_TRACE_ENTER
#define D_TRACE_ENTER       0     // Debug output when each function is entered. 
#endif

#ifndef D_TRACE_EXIT
#define D_TRACE_EXIT        0     // Debug output when each function is exited.
#endif

//
// Control of Debug on and off could be managed from a core include or other 
// master config file.
// 
// Turn on all bits of the ErrorLevel parameter of DebugPrint() when debug is turned on
// for a particular DEBUG output control definition.  Turn off all bits when debug is off. 
//
#ifndef DEBUG_ON
#define DEBUG_ON     0xFFFFFFFF
#endif

#ifndef DEBUG_OFF
#define DEBUG_OFF    0
#endif

//
// Defines to use locally in this driver for the ErrorLevel parameter of the DEBUG macro.
//
#ifndef D_FUNC_ARGS
#define D_FUNC_ARGS  DEBUG_OFF    // Debug output of function arguments
#endif

#ifndef D_STRUCT
#define D_STRUCT     DEBUG_ON     // Debug output of data structures
#endif

#ifndef D_PATHS
#define D_PATHS      DEBUG_OFF    // Dump detailed info on the code paths followed
#endif

#ifndef D_INFO
#define D_INFO       DEBUG_ON     // Dump Debug Info
#endif

#ifndef D_ERROR
#define D_ERROR      DEBUG_ON     // Debug output of error information
#endif

#ifndef D_VERBOSE
#define D_VERBOSE    DEBUG_OFF    // Verbose debug output
#endif

#endif // _CONFIG_H_

