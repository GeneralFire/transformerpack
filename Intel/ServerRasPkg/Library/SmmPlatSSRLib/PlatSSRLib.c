/** @file
  Implementation of memory SSR library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include <Protocol/SmmBase2.h>
#include <Library/SmmPeriodicTimerLib.h>
#include <Library/TimerLib.h>

#include <Library/RasGlobalDataLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/RasDebugLib.h>

#include <Library/PlatSSRLib.h>
#include <Library/WorkaroundLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/RasSiliconLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/CrystalRidgeLib.h>
#include "PlatSSRLibPrivate.h"
#include <Library/PcuMailBoxRasLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/S3MemDataLib.h>
#include <Library/SiliconWorkaroundLib.h>

SSR_STRUCTURE *mSSRData;

#define ADDDC_REGION_MCDDC_LIST          {ADDDC_REGION0_MCDDC_DP_REG, ADDDC_REGION1_MCDDC_DP_REG, \
                                              ADDDC_REGION2_MCDDC_DP_REG, ADDDC_REGION3_MCDDC_DP_REG}
#define ADDDC_RAS_QUIESCE_UNQUIESCE_WORKAROUND   1 //0b0001
#define PLUSONE_RAS_QUIESCE_UNQUIESCE_WORKAROUND 2 //0b0010

/**
  This implementation is to get current imc running mode.

  @retval     TRUE -- 2LM; FALSE  --  1LM.

**/
BOOLEAN
EFIAPI
Is2LMMode (
  VOID
  )
{
  BOOLEAN                                     Flag = FALSE;
  UINT8                                       NodeId;
  UINT8                                       Socket;
  UINT8                                       Mc;

  for (NodeId=0; NodeId< MC_MAX_NODE; NodeId++) {
    Socket          = NODE_TO_SKT(NodeId);
    Mc              = NODE_TO_MC(NodeId);

    if (IsNodePresent (SKTMC_TO_NODE(Socket, Mc))) {
      if (GetM2mModeNmCaching (Socket, Mc)) {
        Flag = TRUE;
        break;
      }
    }
  }

  return Flag;
}

/**
  Check if current speed step is enabled or not.

  @retval   TRUE -- speed step enabled, FALSE -- speed step disabled.

**/
BOOLEAN
EFIAPI
IsGvEnabled (
  VOID
  )
{
  MSR_IA32_MISC_ENABLE_REGISTER MiscEnableReg;

  MiscEnableReg.Uint64 = AsmReadMsr64 (MSR_IA32_MISC_ENABLE);

  return (BOOLEAN) (MiscEnableReg.Bits.EIST);
}

/**
  This implementations is to intialize SSR data.

  @param    pSSRData   pointer of SSR data in SMM.
  @retval   none.

**/
VOID
EFIAPI
InitializeSSRData (
  IN  SSR_STRUCTURE * pSSRData
  )
{
  MEM_TOPOLOGY  *MemTopology;

  MemTopology = GetMemTopology ();

  if (MemTopology->ExRasModesEnabled & PTRLSCRB_EN) {
    pSSRData->PatrolScrubFlag = TRUE;
  } else {
    pSSRData->PatrolScrubFlag = FALSE;
  }

}

/**
  This implementation is to get SSR data, in case first time invoked, it also initialize SSR data.

  @retval    pointer of SSR data.

**/
SSR_STRUCTURE *
EFIAPI
GetSSRData (
  VOID
  )
{
  return mSSRData;
}

EFI_STATUS
M2MQuiescePCodeCallback (
  IN  EFI_HANDLE     Handle,
  IN  CONST VOID     *mPeriodicTimer,
  IN  OUT VOID       *CommBuffer  OPTIONAL,
  IN  OUT UINTN      *CommBufferSize OPTIONAL
  )
{
  UINT32                                     MailboxData;
  UINT32                                     MailboxSts;
  UINT8                                      Socket;
  UINT8                                      Mc;

  SSR_STRUCTURE                              *SSRData;

  SSRData = GetSSRData ();

  RAS_DEBUG((LEVEL_FUNC_FLOW, "M2MQuiescePCodeCallback \n"));

  //
  //Look for the Socket and Mc for which the Quiesce was in progress
  //
  for (Socket=0; Socket < MAX_SOCKET; Socket++) {
    if (SSRData->AdddcPcodeSts[Socket] == QuiesceInProgress) {

      MailboxData = SSRData->AdddcPcodeCmd[Socket];
      Mc = (MailboxData >> 8 ) & 1;

      MailboxSts = SendVlsSparing4Callback (Socket, MailboxData);

      SSRData->QuiescePcodeCounter[Socket]++;

      if ((MailboxSts == 0) || (SSRData->QuiescePcodeCounter[Socket] == 100)) {
        SSRData->AdddcPcodeSts[Socket] = QuiesceAvailable;
        EndSmmPeriodicTimer ();
      }
    }
  }
  return EFI_SUCCESS;
}

/**

  Pcode mailbox call that enables M2M quiesce. The spare enable bit is set in Pcode after Quiesce

  @param Socket    - Socket ID
  @param Mc        - Node ID

  @retval EFI_SUCCESS if spare copy is started and the operation successful

**/
EFI_STATUS
M2MQuiescePCodeInterface (
  IN UINT8  Socket,
  IN UINT32 MailBoxCommand,
  IN UINT32 MailboxData
  )
{
  UINT32                                   MailboxSts = 0;
  EFI_STATUS                               Status = EFI_SUCCESS;
  SSR_STRUCTURE                            *SSRData;

  SSRData = GetSSRData ();

  if (SSRData->AdddcPcodeSts[Socket] == QuiesceAvailable) {

    MailboxSts = Bios2PcodeMailBoxWrite (Socket, MailBoxCommand, MailboxData);

    if (MailboxSts == 0) {
      RAS_DEBUG((LEVEL_FUNC_FLOW,"Quiesce Successful\n"));
      return Status;
    } else {

      //
      //Attempt to read the quiesce status twice before initiating periodic SMI for reading quiesce status
      //
      MailboxSts = Bios2PcodeMailBoxWrite (Socket, MailBoxCommand, MailboxData);

      RAS_DEBUG((LEVEL_FUNC_FLOW,"M2M Quiesce going to register periodic SMI\n"));
      if (( MailboxSts !=0) && (SSRData->AdddcPcodeSts[Socket] == QuiesceAvailable)) {
        SSRData->QuiescePcodeCounter[Socket] = 0;
        SSRData->AdddcPcodeCmd[Socket] = MailboxData;
        //
        //Initiate periodic SMI to trigger here
        //
        StartSmmPeriodicTimer ();
        SSRData->AdddcPcodeSts[Socket] = QuiesceInProgress;
      }
    }
  } else {
    Status = EFI_OUT_OF_RESOURCES;
    RAS_DEBUG((LEVEL_FUNC_FLOW, "M2MQuiescePCodeInterface - Socket%d - Failed\n", Socket));
  }

  return Status;
}

/**

  At the end of 2LM sparing (rank/adddc/+1) we have to clear Work around bits
  nomcstoponddr4err,Link_Cfg_read and this function is used for that purpose

  @param Mc - Memory controller ID
  @retval EFI_SUCCESS if sparing engine is released
  @retval EFI_UNSUPPORTED any other error

**/
VOID
EFIAPI
ClearSparing2LmWa(
    IN UINT8 NodeId,
    IN UINT8 Ch
 )
{
  UINT8   Socket;
  UINT8   SktCh;
  UINT8   Mc;
  UINT8   Sparing2LmWa;
  UINT32  MailboxData = 0;
  UINT32  MailboxCmd = 0;
  UINT32  TCcdWr;

  BOOLEAN IsDcpmmPresent;

if (!IsSiliconWorkaroundEnabled ("S5385535")) {
    return;
  }

  Socket = NODE_TO_SKT(NodeId);
  Mc     = NODE_TO_MC(NodeId);
  SktCh  = NODECHA_TO_SKTCHA(NodeId, Ch);

  Sparing2LmWa = 0x3;

  //
  // Call to clear the work around bits in 2LM mode
  //
  IsDcpmmPresent = IsSystemWithDcpmm ();
  if (IsDcpmmPresent) {
    TCcdWr = GetVlsSparingTccdWr (Socket, SktCh);
    MailboxData = GetVlsSparingData (NodeId, Ch, FALSE);
    MailboxData = MailboxData | (Sparing2LmWa << 14);
    MailboxCmd  = GetRasQuiesceUnquiesceCmd (NodeId, Ch);
    M2MQuiescePCodeInterface(Socket, MailboxCmd, MailboxData);
  }
}

VOID
EFIAPI
InitSparingControlReg (
  VOID
  )
{
  UINT8                                       Socket;
  UINT8                                       Mc;
  UINT8                                       ChOnMc;
  UINT8                                       ChOnSkt;
  MEM_TOPOLOGY        *MemTopology;

  MemTopology = GetMemTopology ();

  //
  // Skipping it in Partial Mirroring case, as it is handled during boot time
  //
  if ((MemTopology->RasModesEnabled) & PARTIAL_MIRROR_1LM) {
    return;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

    for (Mc=0; Mc < MAX_IMC; Mc ++) {
      if (IsNodePresent (SKTMC_TO_NODE(Socket, Mc)) == FALSE) {
        continue;
      }

      for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc ++) {
        ChOnSkt = NODECHA_TO_SKTCHA (Mc, ChOnMc);
        SetSparingCtrlTadRule (Socket, Mc, ChOnSkt, Is2LMMode ());
      }
    }
  }
}


EFI_STATUS
EFIAPI
InitializeSSR (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
{
 BOOLEAN        NewAllocation;

 mSSRData = GetRasGlobalData (L"SSRData", sizeof(SSR_STRUCTURE), &NewAllocation);

 if (NewAllocation) {
   InitializeSSRData (mSSRData);
 }

 SetSmmPeriodicTimer (15000, 15000, M2MQuiescePCodeCallback);

 InitSparingControlReg ();

 return EFI_SUCCESS;
}


/**
  Disables pkg C states.

  @param Node ID
  @retval None

**/
VOID
EFIAPI
DisablePkgC6 (
  IN  UINT8   NodeId
  )
{
  UINT8                           Socket = NODE_TO_SKT (NodeId);
  EFI_STATUS                      Status;
  SSR_STRUCTURE                  *SSRData;

  SSRData = GetSSRData ();

  //
  // If PkgC state is already disabled in the socket, do nothing
  //
  if (SSRData->PkgC6Signal[Socket] == 0) {

    //
    // RAS start disables the Pkg C states
    //
    Status = Bios2PcodeMailBoxWrite (Socket, MAILBOX_BIOS_CMD_RAS_START, 0);
    if (EFI_ERROR(Status)) {
      RAS_DEBUG((LEVEL_BASIC_FLOW,"[Error], diable Pkg C6 status %r \n", Status));
    }
  }

  //
  // update PkgC semaphore
  //
  SSRData->PkgC6Signal[Socket]++;
}


/**
  Enable pkg C states.

  @param Node ID
  @retval None

**/
VOID
EFIAPI
EnablePkgC6 (
  IN  UINT8   NodeId
  )
{
  EFI_STATUS                      Status;
  UINT8                           Socket = NODE_TO_SKT (NodeId);
  SSR_STRUCTURE                  *SSRData;

  SSRData = GetSSRData ();

  //
  // Reset PkgC states for this NODE
  //
  SSRData->PkgC6Signal[Socket]--;

  //
  // Do not enable PkgC states even if 1 MC behind socket has disabled status
  //
  if (SSRData->PkgC6Signal[Socket] == 0) {

    //
    // RAS Exit Re-enables the Pkg C states
    //
    Status = Bios2PcodeMailBoxWrite (Socket, MAILBOX_BIOS_CMD_RAS_EXIT, 0);
    if (EFI_ERROR(Status)) {
      RAS_DEBUG((LEVEL_BASIC_FLOW,"[Error], enable Pkg C6 status %r \n",Status));
    }
  }
}

/**
  This implementation is to scrub specic memory rank to prepare for rank sparing.
  After scrub complete, it will trigger SMI, or if any correctable or uncorrectable error happens,
  it will stop and trigger SMI.

  Caller should ensure restore patrol scrub, and check if patrol scrub is enabled by MRC.

  @param   NodeId     Mc index in whole system
  @param   ChOnMc     Channal index on memory controller
  @param   SparePhysicalRank   rank index to be scrub. Logical rank index is the same as physical rank index on SKX.

**/
VOID
EFIAPI
PatrolScrubSpecificAddress (
  IN  UINT8     NodeId,
  IN  UINT8     ChOnMc,
  IN  UINT8     SparePhysicalRank
  )
{
  UINT8                           Socket = NODE_TO_SKT (NodeId);
  UINT8                           Mc = NODE_TO_MC (NodeId);
  UINT8                           ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnMc);
  EFI_STATUS                      Status;
  UINT32                          RegAddr;
  UINT32                          RegValue;

  RAS_DEBUG ((
    LEVEL_BASIC_FLOW,
    "[SSR] Patrol scrub specific rank. Node:%d  ChOnMc:%d  Rank:%d\n",
    NodeId,
    ChOnMc,
    SparePhysicalRank
    ));

  PatrolScrubInfoSaveOrRestore (NodeId, PATROL_SAVE);
  SetPatrolErrLogDisable (Socket, Mc, ChOnSkt, TRUE);

  //
  // SI WA to down grade patrol error during spare rank scrub.
  //
  if (IsSiliconWorkaroundEnabled ("S2206549949")) {
    SetPatrolErrorDowngrade (Socket, ChOnSkt, TRUE, &RegAddr, &RegValue);
  }
  //
  // Update AMAP to disable all other ranks except the specific rank.
  //
  SetAmapForLegacyPatrol (Socket, ChOnSkt, SparePhysicalRank);

  SwitchPatrolScrubMode (Socket, Mc, ChOnSkt, TRUE);
  SetPatrolScrubLegacyAddress (Socket, Mc, ChOnSkt, SparePhysicalRank, 0);
  SetScrubEnable (Socket, Mc, ChOnSkt, 1);

  if (IsCpuAndRevision (CPU_ICXSP, REV_R0) || IsCpuAndRevision (CPU_SNR , REV_A0)) {
    SetScrubCtlForPatrol (Socket, Mc, ChOnSkt, 0xff, 0x0, 0x0, 0x0, 0xff, 0xff); //Skt, Mc, ChOnSkt, startscrub, stop_on_err, ptl_cmpl, stop_on_cmpl , issueonce, stop_on_rank
  } else {
    SetScrubCtlForPatrol (Socket, Mc, ChOnSkt, 0xff, 0x0, 0x0, 0x0, 0xff, 1); //Skt, Mc, ChOnSkt, startscrub, stop_on_err, ptl_cmpl, stop_on_cmpl , issueonce, stop_on_rank
  }

  //
  // Send patrol inteval value to PCU.
  //
  Status = SetScrubIntervalMailbox (Socket, Mc, POISON_SCRUB_INTERVAL);
  RAS_DEBUG ((LEVEL_BASIC_FLOW, "[patrol] switch to legacy mode status %r\n", Status));
}


/**
  This implementation is to restore patrol scrub engine after scrub complete.

  Caller should ensure  if patrol scrub is enabled by MRC.

  @param   NodeId     Mc index in whole system
  @param   ChOnMc     Channal index on memory controller
  @param   SparePhysicalRank   rank index to be scrub. Logical rank index is the same as physical rank index on SKX.

**/
VOID
EFIAPI
PatrolScrubGoBack (
  IN  UINT8     NodeId,
  IN  UINT8     ChOnMc,
  IN  UINT8     FailedRank,
  IN  UINT8     ScrubbedRank
  )
{
  UINT8                           Socket = NODE_TO_SKT (NodeId);
  UINT8                           Mc = NODE_TO_MC (NodeId);
  UINT8                           ChOnSkt = NODECHA_TO_SKTCHA (NodeId, ChOnMc);
  SSR_STRUCTURE                  *SSRData;

  SSRData = GetSSRData ();

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[SSR] Patrol scrub restore back. Node:%d ChOnMc:%d scrubbed Rank:%d  Failed Rank:%d\n",NodeId,ChOnMc,ScrubbedRank,FailedRank));
  //
  // Stop ptrol scrub engine.
  // Stop patrol engine first, then set patrol engine register.
  //
  ClearScrubInterval (Socket, Mc);
  if (IsCpuAndRevision (CPU_ICXSP, REV_R0) || IsCpuAndRevision (CPU_SNR , REV_A0)) {
    SetScrubCtlForPatrol (Socket, Mc, ChOnSkt, 0xff, 0x0, 0x0, 0x0, 0xff, 0xff); //Skt, Mc, ChOnSkt, startscrub, stop_on_err, ptl_cmpl, stop_on_cmpl , issueonce, stop_on_rank
  } else {
    SetScrubCtlForPatrol (Socket, Mc, ChOnSkt, 0xff, 0x0, 0x0, 0x0, 0xff, 0); //Skt, Mc, ChOnSkt, startscrub, stop_on_err, ptl_cmpl, stop_on_cmpl , issueonce, stop_on_rank
  }
  SetScrubEnable (Socket, Mc, ChOnSkt, 0);
  SetPatrolErrLogDisable (Socket, Mc, ChOnSkt, FALSE);
  SwitchPatrolScrubMode (Socket, Mc, ChOnSkt, FALSE);

  //
  // set AMAP
  //
  SetAmapFromLegacyPatrol (Socket, ChOnSkt, SSRData->PatrolScrubStatus[NodeId][ChOnMc].ChAmapReg, ScrubbedRank, FailedRank);

  ClearSparing2LmWa (NodeId, ChOnMc);
  PatrolScrubInfoSaveOrRestore (NodeId, PATROL_RESTORE);
}

BOOLEAN
EFIAPI
IsPatrolEnabled (
  VOID
  )
{
  SSR_STRUCTURE                              *SSRData;
  SSRData = GetSSRData ();

  return SSRData->PatrolScrubFlag;
}

/**
  Find and update current scrubbing sparing (SSR) state of each McChan to be used to determine virtual SSR
  state of MC

  @param   Node   node index in whole system
  @retval  None
**/
VOID
EFIAPI
UpdateMcChanSsrState (
  IN UINT8         Node
  )
{
  UINT8             Socket;
  UINT8             ChOnSkt;
  UINT8             Mc;
  UINT8             ChOnMc;
  UINT32            PtlComplete;
  UINT32            PtlStopped;

  SSR_STRUCTURE     *SsrData;

  SPARING_PATROL_STATUS_MC_MAIN_STRUCT      SsrStatusReg;

  SsrData = GetSSRData ();
  Socket = NODE_TO_SKT(Node);
  Mc = NODE_TO_MC(Node);
  //
  // Only One channel in MC can have an active scrub/spare in progress. Exit on finding first ch
  //
  for (ChOnMc = 0; ChOnMc< MAX_MC_CH; ChOnMc++) {
    ChOnSkt = NODECHA_TO_SKTCHA (Node, ChOnMc);
    if (IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) {
      SsrData->PatrolScrubStatus[Node][ChOnMc].McchanSsrState = SSRStopMode;
      continue;
    }

    GetScrubCtlForPatrol (Socket, Mc, ChOnSkt, &PtlComplete, &PtlStopped, NULL); // Skt,Mc,ChOnSkt,PtlCmpl,PtlStopped,ScrubIssued
    SsrStatusReg.Data = GetSparingPatrolStatus (Socket, Mc, ChOnSkt);
    //
    //check patrol scrub side.
    //
    if (GetScrubEnable (Socket, Mc, ChOnSkt)) {
      if (IsPatrolScrubSaMode (Socket, Mc, ChOnSkt)) {
        SsrData->PatrolScrubStatus[Node][ChOnMc].McchanSsrState = PatrolSPAMode;
        RAS_DEBUG((LEVEL_BASIC_FLOW, "UpdateMcChanSsrState ch:%d:PatrolSPAMode\n",ChOnSkt));
      } else {
        //
        //legacy mode.
        //
        if (PtlComplete && (SsrStatusReg.Bits.patrol_complete)) {
          SsrData->PatrolScrubStatus[Node][ChOnMc].McchanSsrState = PatrolLegacyCPLMode;
        }
        if (PtlStopped) {
          SsrData->PatrolScrubStatus[Node][ChOnMc].McchanSsrState = PatrolLegacyStoppedMode;
        }
        SsrData->PatrolScrubStatus[Node][ChOnMc].McchanSsrState = PatrolLegacyScrubbingMode;
      }
    } else {
      if (SsrStatusReg.Bits.copy_in_progress == 1 &&  IsSparingEnable (Socket, Mc, ChOnSkt)) {
        SsrData->PatrolScrubStatus[Node][ChOnMc].McchanSsrState = SpareCopyingMode;
        RAS_DEBUG((LEVEL_BASIC_FLOW, "UpdateMcChanSsrState  ch:%d:SpareCopyingMode\n",ChOnSkt));
      } else if ((SsrStatusReg.Bits.copy_in_progress == 0) && (SsrStatusReg.Bits.copy_complete == 1) && (!IsSparingEnable (Socket, Mc, ChOnSkt))) {
        SsrData->PatrolScrubStatus[Node][ChOnMc].McchanSsrState = SpareCopyCPLMode;
        ClearSpareCopyStatus (Socket, Mc, ChOnSkt);
        RAS_DEBUG((LEVEL_BASIC_FLOW, "UpdateMcChanSsrState  ch:%d:SpareCopyCPLMode\n",ChOnSkt));
      } else {
        SsrData->PatrolScrubStatus[Node][ChOnMc].McchanSsrState = SSRStopMode;
        RAS_DEBUG((LEVEL_BASIC_FLOW, "PS Disabled :UpdateMcChanSsrState ch:%d:SSRStopMode\n",ChOnSkt));
      }
    }
  }
}

/**
  Get SSR state of specific MC. Caller need to check if the node exist.
  This routine will return on finding first channel with scrubbing/sparing status in progress

  @param   Node   node index in whole system
  @retval  SSR state of specific node.

**/
SSR_STATE
EFIAPI
GetMcSsrState (
  IN UINT8         Node
  )
{
  UINT8           ChOnMc;
  SSR_STRUCTURE   *SsrData;

  SSR_STATE     McSsrState = SSRStopMode;

  SsrData = GetSSRData ();
  UpdateMcChanSsrState (Node);

  for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc++) {
    if ((SsrData->PatrolScrubStatus[Node][ChOnMc].McchanSsrState > McSsrState) && (SsrData->PatrolScrubStatus[Node][ChOnMc].McchanSsrState < MaxSsrState)) {
      McSsrState = SsrData->PatrolScrubStatus[Node][ChOnMc].McchanSsrState;
    }
  }
  switch (McSsrState) {
    case SSRStopMode:
      RAS_DEBUG((LEVEL_BASIC_FLOW, "McSsrState Node:%d :SSRStopMode\n", Node));
      break;
    case PatrolSPAMode:
      RAS_DEBUG((LEVEL_BASIC_FLOW, "McSsrState Node:%d :PatrolSPAMode\n", Node));
      break;
    case PatrolLegacyCPLMode:
      RAS_DEBUG((LEVEL_BASIC_FLOW, "McSsrState Node:%d :PatrolLegacyCPLMode\n", Node));
      break;
    case PatrolLegacyScrubbingMode:
      RAS_DEBUG((LEVEL_BASIC_FLOW, "McSsrState Node:%d :PatrolLegacyScrubbingMode\n", Node));
      break;
    case PatrolLegacyStoppedMode:
      RAS_DEBUG((LEVEL_BASIC_FLOW, "McSsrState Node:%d :PatrolLegacyStoppedMode\n", Node));
      break;
    case SpareCopyCPLMode:
      RAS_DEBUG((LEVEL_BASIC_FLOW, "McSsrState Node:%d :SpareCopyCPLMode\n", Node));
      break;
    case SpareCopyingMode:
      RAS_DEBUG((LEVEL_BASIC_FLOW, "McSsrState Node:%d :SpareCopyingMode\n", Node));
      break;
    default:
      RAS_DEBUG((LEVEL_BASIC_FLOW, "McSsrState Node:%d :INVALID\n", Node));
  }

  return McSsrState;
}

/**
  Save and restore patrol scrub register.

  @param  Node   node index in whole system
  @param  Flag   PATROL_SAVE  -- save; PATROL_RESTORE -- restore. GV_C6Flag for Gv and C6 enable or not.

  @retval none.

**/
VOID
EFIAPI
PatrolScrubInfoSaveOrRestore (
  UINT8   Node,
  UINT8   Flag
  )
{
  SSR_STRUCTURE                      *SSRData;
  UINT8                               Socket;
  UINT8                               Mc;
  UINT8                               ChOnMc;
  UINT8                               ChOnSkt;
  MEM_TOPOLOGY                        *MemTopology;

  if (!(((Flag & (~GV_C6FLAG)) == PATROL_SAVE) ||
     ((Flag & (~GV_C6FLAG)) == PATROL_RESTORE))) {
    return;
  }

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[SSR] SSR state Save! Flag:%x\n",Flag));

  Socket = NODE_TO_SKT(Node);
  Mc = NODE_TO_MC(Node);
  SSRData = GetSSRData ();

  MemTopology = GetMemTopology ();

  if ((Flag & (~GV_C6FLAG)) == PATROL_SAVE) {
    if ((Flag & GV_C6FLAG) != 0) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "[SSR] disable C6 & GV\n"));
      DisablePkgC6 (Node);
    }

    if (IsPatrolEnabled ()) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "[SSR] stop and save patrol scrub info\n"));

      ClearScrubInterval (Socket, Mc);

      for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc++) {
        ChOnSkt = NODECHA_TO_SKTCHA (Node, ChOnMc);
        if ((IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) || (IsFullMirrorChFailed (Node, ChOnMc))) {
          continue;
        }

        SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubCtlReg.Data = GetScrubCtl (Socket, Mc, ChOnSkt);

        //
        //assume patrol scrub enabled.
        //
        if (GetScrubEnable (Socket, Mc, ChOnSkt)) {
          SetScrubEnable (Socket, Mc, ChOnSkt, 0);
        }

        SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubAddrLoReg.Data = ReadCpuCsr (Socket, ChOnSkt, SCRUBADDRESSLO_MC_MAIN_REG);
        SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubAddrHiReg.Data = ReadCpuCsr (Socket, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);
        SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubAddrLo2Reg.Data = ReadCpuCsr (Socket, ChOnSkt, SCRUBADDRESS2LO_MC_MAIN_REG);
        SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubAddrHi2Reg.Data = ReadCpuCsr (Socket, ChOnSkt, SCRUBADDRESS2HI_MC_MAIN_REG);
        SSRData->PatrolScrubStatus[Node][ChOnMc].ChAmapReg =  ReadCpuCsr (Socket, ChOnSkt, AMAP_MC_MAIN_REG);
        SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubInterval = MemTopology->Socket[Socket].PatrolScrubInterval[Mc];
      }
    }
  } else {
    if (IsPatrolEnabled ()) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "[SSR] restore and restart patrol scrub info\n"));

      for (ChOnMc = 0; ChOnMc < MAX_MC_CH; ChOnMc++) {
        ChOnSkt = NODECHA_TO_SKTCHA (Node, ChOnMc);
        if ((IsChannelPresentPerSocket (Socket, ChOnSkt) == FALSE) || (IsFullMirrorChFailed (Node, ChOnMc))) {
          continue;
        }

        WriteCpuCsr (Socket, ChOnSkt, SCRUBADDRESSLO_MC_MAIN_REG, SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubAddrLoReg.Data);
        WriteCpuCsr (Socket, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG, SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubAddrHiReg.Data);

        if (IsSiliconWorkaroundEnabled ("S2209304250")) {
          WriteCRProxyRegister (Socket, ChOnSkt, MAILBOX_BIOS_CMD_CR_PROXY_SCRUBADDRESS2LO_INDEX, (UINT32) SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubAddrLo2Reg.Data);
          WriteCRProxyRegister (Socket, ChOnSkt, MAILBOX_BIOS_CMD_CR_PROXY_SCRUBADDRESS2HI_INDEX, (UINT32) SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubAddrHi2Reg.Data);
        } else {
          WriteCpuCsr (Socket, ChOnSkt, SCRUBADDRESS2LO_MC_MAIN_REG, (UINT32) SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubAddrLo2Reg.Data);
          WriteCpuCsr (Socket, ChOnSkt, SCRUBADDRESS2HI_MC_MAIN_REG, (UINT32) SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubAddrHi2Reg.Data);
        }

        SetScrubCtl (Socket, Mc, ChOnSkt, SSRData->PatrolScrubStatus[Node][ChOnMc].ScrubCtlReg.Data);
      }

      RestoreScrubInterval (Socket, Mc);
    }

    if ((Flag & GV_C6FLAG) != 0) {
      EnablePkgC6 (Node);
      RAS_DEBUG((LEVEL_BASIC_FLOW, "[SSR] enable C6\n"));
    }
  }
}

/**
  spare copy for rank sparing.

  @param   NodeId    Node in whole system
  @param   Ch        channel index on memory controller
  @param   SrcRank   source rank
  @param   DstRank   Destination rank

**/
VOID
RankSparingCopy (
  IN UINT8 NodeId,
  IN UINT8 Ch,
  IN UINT8 SrcRank,
  IN UINT8 DstRank
  )
{
  UINT8   Socket;
  UINT8   Mc;
  UINT8   SktCh;
  UINT8   SpareRankDiff;

  Socket          = NODE_TO_SKT(NodeId);
  Mc              = NODE_TO_MC(NodeId);
  SktCh           = NODECHA_TO_SKTCHA(NodeId, Ch);
  RAS_DEBUG((LEVEL_BASIC_FLOW, "[SSR] rank sparing start!\n"));

  SetM2MEgreco (Socket, Mc, 0x04);
  Wa_5330286 (NodeId, Ch);
  Wa_4930005 (Socket, SktCh, SrcRank);

  if (IsPatrolEnabled ()) {
    //
    // make sure scrub ctrl register is not set to scrub on failed rank after sparing.
    //
    if (IsLegacyScrubbingRank (Socket, Mc, SktCh, Ch, SrcRank)) {
      SetPatrolScrubLegacyAddress (Socket, Mc, SktCh, DstRank, 0);
    } else {
      RAS_DEBUG((LEVEL_REG, "[SSR]Spare ctrl Src Logical rank[%x] is not same as Scrubbing channel phy. before sparing.\n", SrcRank ));
    }
  }

  //
  // Set the source rank
  //
  SetSparingCtlSrc (Socket, Mc, SktCh, SrcRank, 0xff, 0xff, 0xff, 0xff);

  //
  // Set the destination rank
  //
  SetSparingCtlDest (Socket, Mc, SktCh, DstRank);

  //
  // Set spare rank different value to 1 if the failed rank and spare rank have different rank bit 0. This is new requirement for ICX.
  //
  if ((SrcRank & BIT0) == (DstRank & BIT0)) {
    SpareRankDiff = 0;
  } else {
    SpareRankDiff = 1;
  }
  SetSpareRankDiff (Socket, SktCh, SpareRankDiff);

  //
  // Set the spare mode in the Ch as rank_sparing
  //

  SetSparingCtrl (Socket, Mc, SktCh, TRUE, FALSE, FALSE, 1, TRUE, 0xff, 0xff, 0xff, 0xff);

  RAS_DEBUG((LEVEL_BASIC_FLOW, "[SSR] rank sparing end!\n"));
}

/**
  Wrapper for applying silicon workarounds for RAS_QUIESCE_UNQUIESCE command

 @param Mc -- Node index
 @param ChOnMc  -- channel on memory controller
 @param   SparingMode  sparing mode

 @return VLS sparing mailbox command
**/
UINT32
EFIAPI
GetRasQuiesceUnquiesceCmdWrapper (
  IN  UINT8           NodeId,
  IN  UINT8           ChOnMc,
  IN  UINT8           FailedRank,
  IN  SPARING_MODE    SparingMode
  )
{
  UINT8 Mc;
  UINT32  MailboxCmd = 0;

  Mc = NODE_TO_MC(NodeId);
  MailboxCmd = GetRasQuiesceUnquiesceCmd(NodeId, ChOnMc);
  //
  // Workaround Bits [23:20] = 0001 for ADDC 0010 for +1 sparing
  //
  if (SparingMode == SddcPlusOne) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t GetRasQuiesceUnquiesceCmdWrapper Sparing mode - +1\n"));
    if (IsSiliconWorkaroundEnabled ("S2207860950")) {
      return ((PLUSONE_RAS_QUIESCE_UNQUIESCE_WORKAROUND << 20) | (FailedRank << 24) | MailboxCmd);
    }
    return ((PLUSONE_RAS_QUIESCE_UNQUIESCE_WORKAROUND << 20) | MailboxCmd);
  } else {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t GetRasQuiesceUnquiesceCmdWrapper Sparing mode - adddc\n"));
    return ((ADDDC_RAS_QUIESCE_UNQUIESCE_WORKAROUND << 20) | MailboxCmd);
  }
}


/**
  spare copy for VLS(ADDDC, +1)

  @param   SparingRegionPtr ADDDC region parameters.
  @param   SparingMode  sparing mode
**/
EFI_STATUS
EFIAPI
VlsSparingCopy (
  IN SPARING_REGION_STRUCT  *SparingRegionPtr,
  IN SPARING_MODE           SparingMode
  )
{
  UINT8         Socket;
  UINT8         Mc;
  UINT8         SktCh;
  UINT8         NodeId;
  UINT8         Ch;
  UINT32        AdddcRegionoffset[] = ADDDC_REGION_MCDDC_LIST;
  UINT8         RegionNum;
  UINT32        MailboxData = 0;
  UINT32        MailboxCmd = 0;
  UINT8         RegionSize = 0;
  UINT8         Reverse = 0;
  UINT8         Mirror = 0;
  UINT8         PartialMirror = 0;
  UINT8         FailedBank;
  UINT8         NonFailedBank;
  EFI_STATUS    Status;
  MEM_TOPOLOGY  *MemTopology;

  ADDDC_REGION0_MCDDC_DP_STRUCT               AdddcRegion;

  MemTopology = GetMemTopology ();

  Status = EFI_UNSUPPORTED;

  if (SparingRegionPtr == NULL) {
    return Status;
  }

  NodeId           = SparingRegionPtr->NodeId;
  Ch               = SparingRegionPtr->ChOnNode;
  RegionNum        = SparingRegionPtr->RegionNum;

  Socket           = NODE_TO_SKT(NodeId);
  Mc               = NODE_TO_MC(NodeId);
  SktCh            = NODECHA_TO_SKTCHA(NodeId, Ch);

  if (SparingMode == SddcPlusOne) {
  } else {
    if (SparingMode == BankAdddcSparingMR) {
      RAS_DEBUG((LEVEL_BASIC_FLOW,"VlsSparingCopy: Bank AdddcSparingMR\n"));
    } else {
      RAS_DEBUG((LEVEL_BASIC_FLOW,"VlsSparingCopy: Rank AdddcSparingMR\n"));
    }

    RegionSize = ADDDC_REGION_SIZE_BANK;
    if (SparingMode == RankAdddcSparingMR) {
      RegionSize = ADDDC_REGION_SIZE_RANK;
    }

    if (SparingRegionPtr->ReverseRegion) {
      Reverse = 1;
    } else {
      Reverse = 0;
    }

    if (MemTopology->RasModesEnabled & FULL_MIRROR_1LM) {
      Mirror = 1;
    }
    if(MemTopology->RasModesEnabled &  PARTIAL_MIRROR_1LM) {
      Mirror = 1;
      PartialMirror = 1;
    }

    SetSparingCtrl (Socket, Mc, SktCh, FALSE, TRUE, FALSE, RegionSize, FALSE, 0, Reverse, Mirror, PartialMirror);

    if (SparingMode == BankAdddcSparingMR) {
      SetSparingCtlSrc (Socket, Mc, SktCh, SparingRegionPtr->FailedRank, SparingRegionPtr->FailedSubRank, SparingRegionPtr->FailedBank, 0xff, 0xff);
    }else {
      SetSparingCtlSrc (Socket, Mc, SktCh, SparingRegionPtr->FailedRank, 0xff, 0xff, 0xff, 0xff);
    }

    SetSparingCtlDest (Socket, Mc, SktCh, SparingRegionPtr->FailedRank);

    if (SparingMode == BankAdddcSparingMR) {
      FailedBank = SparingRegionPtr->FailedBank;
      NonFailedBank = SparingRegionPtr->NonFailedBank;
    } else {
      FailedBank = 0xff;
      NonFailedBank = 0xff;
    }

    SetAdddcRegion (Socket, SktCh, RegionNum, SparingRegionPtr->FailedRank, SparingRegionPtr->FailedSubRank, SparingRegionPtr->NonFailedRank,
                            SparingRegionPtr->NonFailedSubRank, RegionSize, FailedBank, NonFailedBank);
    SetSchedulerAdddcRegion (Socket, SktCh, RegionNum, SparingRegionPtr->FailedRank, SparingRegionPtr->FailedSubRank, SparingRegionPtr->NonFailedRank,
                            SparingRegionPtr->NonFailedSubRank, RegionSize, FailedBank, NonFailedBank);


    if(RegionNum >= (sizeof(AdddcRegionoffset) / sizeof(AdddcRegionoffset[0]))) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "\n Unsupported Region number access\n"));
      return EFI_UNSUPPORTED;
    } else {
      AdddcRegion.Data = (UINT8) ReadCpuCsr (Socket, SktCh, AdddcRegionoffset[RegionNum]);
      AdddcRegion.Bits.faildevice          = SparingRegionPtr->FailedDevice;
      WriteCpuCsr (Socket, SktCh, AdddcRegionoffset[RegionNum], AdddcRegion.Data);
    }

    Wa_5372419 (Socket, SktCh, SparingRegionPtr->FailedRank);
    Wa_5372419 (Socket, SktCh, SparingRegionPtr->NonFailedRank);
  }

  SetM2MEgreco (Socket, Mc, 0x04);
  Wa_5330286 (NodeId, Ch);
  AdddcStandardRasWa (Socket, SktCh);

  if (IsSiliconWorkaroundEnabled ("S1909288826")) {
    MailboxCmd  = GetRasQuiesceUnquiesceCmdWrapper (NodeId, Ch, SparingRegionPtr->FailedRank, SparingMode);
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "\t VlsSparingCopy: Silicon WA Active. Mailbox = %d\n", MailboxCmd));
  } else {
    MailboxCmd  = GetRasQuiesceUnquiesceCmd (NodeId, Ch);
  }

  MailboxData = GetVlsSparingData (NodeId, Ch, FALSE);
  Status =   M2MQuiescePCodeInterface(Socket, MailboxCmd, MailboxData);

  return Status;
}
