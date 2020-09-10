/** @file
  Implementation of memory correctable error routine library.

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
#include <IioUniversalData.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/PlatSSRLib.h>
#include <Library/RasDebugLib.h>
#include <Library/PlatMemErrorDataLib.h>
#include <Library/ErrorReportLib.h>
#include <Library/BaseMemoryLib.h>
#include <SiliconSetting.h>
#include <SiliconSettingRAS.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/RasSiliconLib.h>
#include "ImcRoutingPrivate.h"
#include <Library/SiliconWorkaroundLib.h>
#include <Library/KtiApi.h>
#include <Library/RasRegisterClearLib.h>

BOOLEAN
EFIAPI
DummyHandler (
  IN UINT8        NodeId,
  IN EVENT_TYPE   EventType
  )
{
  return FALSE;
}

/**
  Enables logging of QPI errors.

  @param  VOID

  @retval EFI_SUCCESS if the call succeeded.

**/
VOID
EFIAPI
EnableElogKTI (
  UINT8 Socket
  )
{
  UINT8 LinkIndex;
  MEM_TOPOLOGY                                *MemTopology;

  MemTopology = GetMemTopology ();

  //
  // This code currently only enables/disables support for Clock Data Failover.
  // All other errors are enabled (unmasked) by default, currently.  Need to add setup question support.
  //
  Ms2IosfIngErrConfig (Socket);

  for (LinkIndex = 0; LinkIndex < (GetKtiPortCnt()); LinkIndex++) {
    if (!MemTopology->Socket[Socket].UPIPortValid[LinkIndex]) {
      continue;
    }

    EnableDisableKtiElog (Socket, LinkIndex, FALSE);
  }
}

VOID
EFIAPI
EnableUboxError (
  UINT8 Socket
  )
{
  EnableUboxErrCtrl (Socket);

  EnableUboxSmiSrc (Socket);
}

/**
  Configure Retry read register for deciding which will log patrol and which one will log
  ECC errors

  @param[in]      Socket

  @retval none
**/
VOID
EFIAPI
ConfigSystemRetryRegister (
  IN  UINT8   Socket
  )
{
  UINT8                                   ChOnSkt;

  RAS_DEBUG ((LEVEL_BASIC_FLOW,"[imc] ConfigSystemRetryRegister start!\n"));
  for (ChOnSkt = 0; ChOnSkt < IMC_MAX_CH; ChOnSkt ++) {
    if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
      continue;
    }
    //
    //Enable sparing/patrol scrub errors in the RETRY_RD_ERROR_LOG CSR
    //
    ConfigRetryRegister (Socket, ChOnSkt);
  }
}

VOID
EFIAPI
ImcCorrectableErrorEnable (
  IN  UINT8   Socket,
  IN  UINT8   SpareIntSelect
  )
{
  UINT8                                   Mc;
  UINT8                                   ChOnSkt;
  UINT8                                   Rank;

  RAS_DEBUG ((LEVEL_BASIC_FLOW,"[imc] ImcCorrectableErrorEnable start!\n"));
  for (ChOnSkt = 0; ChOnSkt < IMC_MAX_CH; ChOnSkt ++) {
    if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
      continue;
    }
    Mc = ChOnSkt/MAX_MC_CH;

    for (Rank = 0; Rank < (MAX_RANK_CH * SUB_CH); Rank++) {
      RAS_DEBUG ((LEVEL_REG,"[imc] clear socket 0x%x channel 0x%x  Rank 0x%x status.\n", Socket, ChOnSkt, Rank));
      ClearRankErrors (Socket, ChOnSkt, Rank);
    }

    if (SpareIntSelect != 0) {
      if (SpareIntSelect == 1) {
        SetSmiSpareCtl (Socket, Mc, ChOnSkt, 1, 0xff, 0xff); //Skt, Imc, SktCh,intrpt_sel_smi,intrpt_sel_cmci,intrpt_sel_pin
      }
      else if (SpareIntSelect == 2) {
        SetSmiSpareCtl (Socket, Mc, ChOnSkt, 0xff, 0xff, 1);
      }
      else if (SpareIntSelect == 4) {
        SetSmiSpareCtl (Socket, Mc, ChOnSkt, 0xff, 1, 0xff);
      }
      RAS_DEBUG ((LEVEL_REG,"[imc] SMI pin selection:0x%x\n", SpareIntSelect));
    }
  }

  EnableUboxError (Socket);
  EnableElogKTI (Socket);
}

/**
  Get new and pending error bitmaps.

  Update the bitmaps for each node in the socket that has new and/or pending
  errors.

  Assumption: iMC error on UBOX register.

  @param[in]      Socket
  @param[in, out] NewErrorBitmap      Bitmap of nodes with new errors.
  @param[in, out] PendingErrorBitmap  Bitmap of nodes with pending errors.

  @retval TRUE    New or pending errors were found.
  @retval FALSE   Neither new nor pending errors were found.
**/
BOOLEAN
EFIAPI
GetNewAndPendingError (
  IN      UINT8   Socket,
  IN OUT  UINT32  *NewErrorBitmap,
  IN OUT  UINT32  *PendingErrorBitmap
  )
{
  CORRERRORSTATUS_MCDDC_DP_STRUCT CorrErrorStatusReg;
  UINT8                           ChOnSkt;
  UINT8                           Mc;
  UINT8                           Node;
  BOOLEAN                         FlagImcCorrError = FALSE;

  for (ChOnSkt = 0; ChOnSkt < IMC_MAX_CH; ChOnSkt++) {
    if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
      continue;
    }

    Mc = ChOnSkt / MAX_MC_CH;
    Node = SKTMC_TO_NODE (Socket, Mc);

    CorrErrorStatusReg.Data = ReadCpuCsr (Socket, ChOnSkt, CORRERRORSTATUS_MCDDC_DP_REG);
    RAS_DEBUG ((
      LEVEL_REG,
      "[imc] Socket:0x%x Ch:0x%x overflow status:0x%x\n",
      Socket,
      ChOnSkt,
      CorrErrorStatusReg.Bits.err_overflow_stat
      ));

    if (CorrErrorStatusReg.Bits.err_overflow_stat) {
      *NewErrorBitmap |= (1 << Node);
      FlagImcCorrError = TRUE;
    }
  }

  for (Mc = 0; Mc < MAX_IMC; Mc++) {
    Node = SKTMC_TO_NODE (Socket, Mc);

    if (GetNodeProgress (Node)) {
      *PendingErrorBitmap |= (1 << Node);
      FlagImcCorrError = TRUE;
    }
  }
  RAS_DEBUG ((
    LEVEL_REG,
    "[imc]Socket:0x%x  New event:0x%x  pending event:0x%x  Flag:0x%x\n",
    Socket,
    *NewErrorBitmap,
    *PendingErrorBitmap,
    FlagImcCorrError
    ));

  return FlagImcCorrError;
}

VOID
EFIAPI
ClearMcCorrectableError (
  IN   UINT8   Node
  )
{
  UINT8                           Socket;
  UINT8                           ChOnMc;
  UINT8                           ChOnSkt;
  UINT8                           Rank;

  Socket = NODE_TO_SKT (Node);

  for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc ++) {
    ChOnSkt = NODECHA_TO_SKTCHA (Node, ChOnMc);

    if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
      continue;
    }

    for (Rank = 0; Rank < MAX_RANKS; Rank++) {
      ClearRankErrors (Socket, ChOnSkt, Rank);
    }
  }
}

VOID
EFIAPI
ReportMemoryError (
  IN    UINT8     Node,
  IN    UINT8     MemCeFloodPolicy,
  IN    BOOLEAN   CloakingEn
  )
{
  UINT8                           Socket;
  UINT8                           ChOnMc;
  UINT8                           ChOnSkt;
  FAILED_DIMM_INFO                ErrorInfo;
  DIMM_ERROR_INFO                 ErrorDataInfo;
  EFI_PLATFORM_MEMORY_ERROR_DATA  ErrorDataForReport;
  EFI_STATUS                      Status;

  ZeroMem (&ErrorDataInfo, sizeof (DIMM_ERROR_INFO));

  Socket = NODE_TO_SKT (Node);

  for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc ++) {
    ChOnSkt = NODECHA_TO_SKTCHA (Node, ChOnMc);

    if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
      continue;
    }
    Status = GetChannelErrorInfo (Node, ChOnMc, (UINT64)-1, &ErrorInfo);
    if (!EFI_ERROR (Status)) {
      ErrorDataInfo.Socket = Socket;
      ErrorDataInfo.SktCh = ChOnSkt;
      ErrorDataInfo.Rank = ErrorInfo.FailedRank;
      ErrorDataInfo.SubRank = ErrorInfo.FailedSubRank;
      ErrorDataInfo.Dimm = ErrorInfo.FailedDimm;
      ErrorDataInfo.Dev = ErrorInfo.FailedDev;
      ErrorDataInfo.Bank = ErrorInfo.FailedBG << 2 | ErrorInfo.FailedBank;
      ErrorDataInfo.Row = ErrorInfo.FailedRow;
      ErrorDataInfo.Col = ErrorInfo.FailedCol;
      FillCorrectablMemoryErrorData (MEM_ECC_ERROR, &ErrorDataInfo, &ErrorDataForReport);
      if (CloakingEn == FALSE) {
        ReportCorrectedMemErrorLog (&ErrorDataForReport);
      }
      //
      //disable CE in 24 time frame. To check if can move to listener.
      //
      CorrectedErrorSMIThrottling (Node, ChOnMc, ErrorInfo.FailedRank, MemCeFloodPolicy);
      RecordPositionToClearRetryRegister(Socket, ChOnSkt);
    }
  }
}

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
VOID
ClearSkippedCorrectableError (
  )
{
    UINT8                Node;
    
    for (Node = 0; Node < MC_MAX_NODE; Node++) {
        ClearMcCorrectableError (Node);
    }
}
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support

VOID
EFIAPI
CorrectableMemoryErrorHandler (
  IN       UINT8    Socket,
  IN       UINT32  *NewErrorBitmap,
  IN       UINT32  *PendingErrorBitmap,
  IN       UINT8    MemCeFloodPolicy
  )
{
  UINT8                RasEventHndlrEntry;
  UINT8                Mc;
  UINT8                Node;
  BOOLEAN              IsEventHandled;
  SSR_STATE            SSRState;
  IMC_RAS_STRUCTURE   *ImcRas;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[imc] memory error handler start!\n"));

  ImcRas = GetImcRasData ();

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
  {
  	  BOOLEAN 		SkipEventHandlers = FALSE;
  	  UINT8         ErrorEvent = SKIP_CE_HANDLING;

      // Oem Hook To decide the Error action based on the Error data
      OemReadyToTakeAction(ErrorEvent, NULL, &SkipEventHandlers);
      if (SkipEventHandlers == TRUE) {
          DEBUG((DEBUG_INFO,"OEM request..Skipping all corrected memory error handlers!\n"));
          ClearSkippedCorrectableError();
          return;
      }
  }
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support
  
  for (Mc = 0; Mc < MAX_IMC; Mc++) {
    if (IsNodePresent (SKTMC_TO_NODE (Socket, Mc)) == FALSE) {
      continue;
    }

    Node = SKTMC_TO_NODE (Socket, Mc);
    SSRState = GetMcSsrState (Node);

    if (((*PendingErrorBitmap & (1 << Node)) != 0) && (SSRState == SpareCopyCPLMode)) {
      for (RasEventHndlrEntry = 0; ImcRas->RasEventHndlrTable[RasEventHndlrEntry] != END_OF_TABLE; RasEventHndlrEntry++) {
        IsEventHandled = ImcRas->RasEventHndlrTable[RasEventHndlrEntry] (Node, EVENTS_IN_PROGRESS);

        if (IsEventHandled ) {
          break;      // Event handled on this node. Check other nodes
        }
      }
    }

    if ((*NewErrorBitmap & (1 << Node)) != 0) {
      if (ImcRas->TriggerSWErrThEn) {
        if (!UpdateEnhancedDimmErrRecord (Node, ImcRas->SpareSwErrTh, ImcRas->TimeWindow)) {
          ClearMcCorrectableError (Node);
          continue;
        }
      }
      if ((SSRState == PatrolSPAMode) || (SSRState == SSRStopMode)) {
        for (RasEventHndlrEntry = 0; ImcRas->RasEventHndlrTable[RasEventHndlrEntry] != END_OF_TABLE; RasEventHndlrEntry++) {
          RAS_DEBUG ((LEVEL_BASIC_FLOW,"RasEventHndlrEntry index: 0x%x\n",RasEventHndlrEntry));
          IsEventHandled = ImcRas->RasEventHndlrTable[RasEventHndlrEntry] (Node, NEW_EVENTS);
          if (IsEventHandled ) {
            break;
          }
        }
      }

// APTIOV_SERVER_OVERRIDE_RC_START: RAS OEM Hooks Support
#if AMI_RAS_ENHANCEMENTS_AND_HOOKS_SUPPORT
      if (IsEventHandled) {
          ReportMemoryError (Node, MemCeFloodPolicy , ImcRas->CloakingEn);
      }
#else
      ReportMemoryError (Node, MemCeFloodPolicy, ImcRas->CloakingEn);
#endif
// APTIOV_SERVER_OVERRIDE_RC_END: RAS OEM Hooks Support
      ClearMcCorrectableError (Node);
    } // for(rasEventHndlrEntry)
  }
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[imc] memory error handler end!\n"));
}

/**
  This implementation is to get subrank value on certain Channel from retry register.

  @param[in]     Node    Memory controller index info per whole system
  @param[in]     ChOnNode  channel index on memory controller

  @retval     if SUCCESS, means DIMM error found; otherwise no DIMM error found.

**/
UINT8
EFIAPI
GetSubRank (
  IN      UINT8    Node,
  IN      UINT8    ChOnSkt
  )
{
  UINT8    Socket;
  UINT8    SubRank = 0;

  Socket = NODE_TO_SKT (Node);
  GetRetryReadInfo (Socket, ChOnSkt, (UINT64)-1,NULL, NULL, NULL, &SubRank, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

  return SubRank;
}



