/** @file
  EvDebugPrint.h

  Debug Print function for EV library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef _EV_DEBUG_PRINT_H_
#define _EV_DEBUG_PRINT_H_

//
//  EV Debug message type
//
typedef enum {
  BSSA_API_DEBUG_MSGS = 0,
  VERBOSE_LOG_MSGS    = 1,
  VERBOSE_ERROR_MSGS  = 2,
  FUNCTION_DEBUG_MSGS = 3,
  ENBL_MARGIN_TEST_MSG_MAX
} EvMsgType;

/**
  Function to check if the Print message is enabled by PcdEvDebugMsg

  @param[in] MsgType - Message Type to check

  @retval   TRUE  -  Message Enabled
            FALSE -  Message Disabled
**/
BOOLEAN
EFIAPI
IsCheckMsgTypeEnabled (
  IN EvMsgType MsgType
  );

/**
  Function to print debug message for EV library.

  @param[in] PrintLevel   -  Print Level defined by PcdRcDebugAllowedLevelsMask
  @param[in] MsgType      -  Message Type defined by EvMsgType
                  BSSA_API_DEBUG_MSGS = 0
                  VERBOSE_LOG_MSGS    = 1
                  VERBOSE_ERROR_MSGS  = 2
                  FUNCTION_DEBUG_MSGS = 3
  @param[in] FormatString -  A Null-terminated format string.

  @retval N/A
**/
VOID
EFIAPI
EvDebugPrint (
  IN UINT32               PrintLevel,
  IN EvMsgType            MsgType,
  IN UINT8                *FormatString,
  ...
  );

#endif // #ifndef _EV_DEBUG_PRINT_H_
