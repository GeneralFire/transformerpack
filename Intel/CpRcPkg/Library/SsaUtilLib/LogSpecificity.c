/** @file
  LogSpecificity.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2018 Intel Corporation. <BR>

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

#include "SsaUtilInternal.h"

/**
  This function is used to log the given specificity value.

  @param[in]Specificity  -  Margin parameter specificity

  @retval  N/A.
**/
VOID
LogSpecificity (
  IN MARGIN_PARAM_SPECIFICITY Specificity
  )
{
  BOOLEAN SomethingLogged = FALSE;

  if (Specificity & PlatformSpecific) {
    RcDebugPrint (SDBG_ERROR, "Platform");
    SomethingLogged = TRUE;
  }

  if (Specificity & SocketSpecific) {
    if (SomethingLogged) {
      RcDebugPrint (SDBG_ERROR, "|");
    }
    RcDebugPrint (SDBG_ERROR, "Socket");
    SomethingLogged = TRUE;
  }

  if (Specificity & ControllerSpecific) {
    if (SomethingLogged) {
      RcDebugPrint (SDBG_ERROR, "|");
    }
    RcDebugPrint (SDBG_ERROR, "Controller");
    SomethingLogged = TRUE;
  }

  if (Specificity & ChannelSpecific) {
    if (SomethingLogged) {
      RcDebugPrint (SDBG_ERROR, "|");
    }
    RcDebugPrint (SDBG_ERROR, "Channel");
    SomethingLogged = TRUE;
  }

  if (Specificity & RankSpecific) {
    if (SomethingLogged) {
      RcDebugPrint (SDBG_ERROR, "|");
    }
    RcDebugPrint (SDBG_ERROR, "Rank");
    SomethingLogged = TRUE;
  }

  if (Specificity & StrobeSpecific) {
    if (SomethingLogged) {
      RcDebugPrint (SDBG_ERROR, "|");
    }
    RcDebugPrint (SDBG_ERROR, "Strobe");
    SomethingLogged = TRUE;
  }

  if (Specificity & LaneSpecific) {
    if (SomethingLogged) {
      RcDebugPrint (SDBG_ERROR, "|");
    }
    RcDebugPrint (SDBG_ERROR, "Lane");
    SomethingLogged = TRUE;
  }

  if (!SomethingLogged) {
    RcDebugPrint (SDBG_ERROR, "None");
  }
} // end function LogSpecificity


// end file LogSpecificity.c
