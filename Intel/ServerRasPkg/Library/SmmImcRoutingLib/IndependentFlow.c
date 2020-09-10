/** @file
  Implementation of SDDC library.
 
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
#include <Library/RasDebugLib.h>
#include <SiliconSetting.h>
#include <Library/PlatMemTopologyLib.h>
#include "ImcRoutingPrivate.h"

/**
  Check if SDDC Plus One is enabled and perform that

  @param NodeId    - Memory controller ID
  @param EventType - NEW_EVENTS or EVENTS_IN_PROGRESS
  @param *IsEventHandled - ptr to buffer to hold event handled status

  @retval status - EFI_SUCCESS if no failures
  @retval non-zero error code for failures

  @retval IsEventHandled - TRUE -  event was handled
  @retval FALSE - event not handled

**/
BOOLEAN
EFIAPI
CheckAndHandleIndependentCE (
  IN UINT8      NodeId,
  IN EVENT_TYPE EventType
  )
{
  UINT8                             ChOnMc;
  UINT8                             ChOnSkt;
  UINT8                             Socket;
  UINT8                             Rank = 0;

  RAS_DEBUG((LEVEL_BASIC_FLOW,"[PERF] start \n"));
  if (EventType == EVENTS_IN_PROGRESS) {
    //
    // clear unhandled correct error events cannot be 'IN PROGRESS'
    // indicate that event as not handled always
    //
    return TRUE;
  }

  Socket = NODE_TO_SKT(NodeId);
  for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc++) {
    if (IsChannelPresent (NodeId, ChOnMc) == FALSE) {
      continue;
    }

    ChOnSkt = NODECHA_TO_SKTCHA(NodeId,ChOnMc);
    for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank ++) {
// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
      {
	    EFI_STATUS  		Status;
        BOOLEAN     		SkipEventHandlers = FALSE;
        UINT8       		ErrorEvent = DEF_HANDLER;
		FAILED_DIMM_INFO    IndependentCeFailedDimmInfo;

        Status = GetChannelErrorInfo (NodeId, ChOnMc, FALSE, &IndependentCeFailedDimmInfo);
        if (!EFI_ERROR(Status)) {
            OemReadyToTakeAction(ErrorEvent, &IndependentCeFailedDimmInfo, &SkipEventHandlers); // Oem Hook To decide the Error action based on the Error data
            if (SkipEventHandlers == TRUE) {
                DEBUG((DEBUG_INFO,"OEM request..Skipping Independent mode memory error event!\n"));
                return FALSE;
            }
        }
      }
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support
      ClearRankErrors (Socket, ChOnSkt, Rank);
    }
  }
  RAS_DEBUG((LEVEL_BASIC_FLOW,"[PERF] end \n"));
  return TRUE;
}

/**

  Initialize independent mode CE handler.
  This routine should be called only once during boot

  @param None
  @retval None
  **/
VOID
EFIAPI
InitializeIndependentMode (
  VOID
  )
{
  IMC_RAS_STRUCTURE   *ImcRas;
  ImcRas = GetImcRasData ();

  ImcRas->RasEventHndlrTable[INDEPENDENT_INDEX] = CheckAndHandleIndependentCE;
}


