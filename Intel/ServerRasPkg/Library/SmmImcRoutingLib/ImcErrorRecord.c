/** @file
  Implementation of memory correctable error record table library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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

#include "ImcRoutingPrivate.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RasDebugLib.h>

ENHANCED_ERROR_RECORD_DATA  mEnhancedMemErrRecList[MAX_SOCKET][MAX_CH][MAX_RANK_CH * SUB_CH][MAX_BANK_RANK];

UINT16
EFIAPI
CheckTimeWindow (
  IN  UINT16         Threshold,
  IN  UINT16         HeadOfTsc,
  IN  UINT16         TailOfTsc,
  IN  OUT  UINT16   *Counter,
  IN  UINT32         TimeWindow,
  IN  UINT32        *TscList
)
{
  UINT16  Index;

  for (Index = HeadOfTsc; Index % Threshold != TailOfTsc; Index++){
    if ((TscList[TailOfTsc] - TscList[Index]) <= TimeWindow) {
      break;
    }
    *Counter = *Counter - 1;
  }
  return Index;
}

BOOLEAN
EFIAPI
UpdateEnhancedDimmErrRecord (
  IN    UINT8     Node,
  IN    UINT16    Threshold,
  IN    UINT16    TimeWindow
  )
{
  UINT8                           Socket;
  UINT8                           ChOnMc;
  UINT8                           ChOnSkt;
  UINT16                          IndexOfVaildRec;
  UINT16                          NextTsc;
  FAILED_DIMM_INFO                ErrorInfo;
  EFI_STATUS                      Status;
  ENHANCED_ERROR_RECORD_DATA     *ErrorRecord;
  BOOLEAN                         BankLevelOverflow = FALSE;
  UINT32                          CurrTimeStamp;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Updated] Policy bank level threshold: %d, time window: %d.\n", Threshold, TimeWindow));

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[Updated] UpdateEnhancedDimmErrRecord started.\n"));

  Socket = NODE_TO_SKT (Node);

  for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc ++) {
    ChOnSkt = NODECHA_TO_SKTCHA (Node, ChOnMc);

    if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
      continue;
    }

    Status = GetChannelErrorInfo (Node, ChOnMc, FALSE, &ErrorInfo);
    if (!EFI_ERROR (Status)) {
      ErrorRecord = &mEnhancedMemErrRecList[Socket][ChOnSkt][ErrorInfo.FailedRank][ErrorInfo.FailedBank];
      CurrTimeStamp = EfiSmmGetTime ();
      NextTsc = (ErrorRecord->Counter + ErrorRecord->HeadOfTsc) % Threshold;
      ErrorRecord->Tsc[NextTsc] = CurrTimeStamp;
      ErrorRecord->Counter++;
      if (ErrorRecord->Counter == Threshold){
        IndexOfVaildRec = CheckTimeWindow (Threshold, ErrorRecord->HeadOfTsc, NextTsc, &ErrorRecord->Counter, TimeWindow, ErrorRecord->Tsc);
        if (IndexOfVaildRec == ErrorRecord->HeadOfTsc){
          ErrorRecord->Counter = 0;
          ErrorRecord->HeadOfTsc = 0;
          BankLevelOverflow = TRUE;
        } else {
          ErrorRecord->HeadOfTsc = IndexOfVaildRec;
        }
      }
    }
  }
  RAS_DEBUG ((LEVEL_BASIC_FLOW,"[Updated] UpdateEnhancedDimmErrRecord ended.\n"));
  return BankLevelOverflow;
}

/**
  Initialize Ehanced DIMM Error Record list.

**/
VOID
EFIAPI
InitEnhancedDimmErrRecList (
  IN UINT16    Threshold
  )
{
  UINT8                           Socket;
  UINT8                           ChOnSkt;
  UINT8                           Rank;
  UINT8                           Bank;

  ZeroMem (&mEnhancedMemErrRecList, sizeof (ENHANCED_ERROR_RECORD_DATA));
  for(Socket = 0; Socket < MAX_SOCKET; Socket ++){
    for(ChOnSkt = 0; ChOnSkt < MAX_CH; ChOnSkt ++){
      for(Rank = 0; Rank < MAX_RANK_CH * SUB_CH; Rank ++){
        for(Bank =0; Bank < MAX_BANK_RANK; Bank ++){
          mEnhancedMemErrRecList[Socket][ChOnSkt][Rank][Bank].Tsc = (UINT32 *) AllocateRuntimeZeroPool (Threshold * sizeof (UINT32));
          ASSERT (mEnhancedMemErrRecList[Socket][ChOnSkt][Rank][Bank].Tsc != NULL);
        }
      }
    }
  }
}
