/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2019 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include "KtiRas.h"
#include "KtiRasMain.h"
#include "KtiRasSupport.h"
#include <Library/BaseMemoryLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/SystemInfoLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemRcLib.h>
#include <Library/TimerLib.h>
#include <RcRegs.h>
#include <ScratchpadList.h>
#include <Library/MemMcIpLib.h>
#include <Library/ChaLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Guid/UboxIpInterface.h>
#include <SiliconSetting.h>

extern  EFI_HP_IOX_ACCESS_PROTOCOL *mHpIOXAccess;

/**

  Do some checking to make sure the parameters passed to the driver APIs are valid.

  @param EventType  - RAS_MPLINK_EVNT_ONLINE or RAS_MPLINK_EVNT_OFFLINE
  @param SocketType - should always be RAS_MPLINK_SOCKET_TYPE_CPU
  @param SocketId   - Socket ID to be on/offlined

  @retval RAS_MPLINK_SUCCESS
  @retval RAS_MPLINK_FAILURE_INVALID_EVTTYPE

**/
RAS_MPLINK_STATUS
ValidateParameters (
  IN UINT8 EventType,
  IN INT32 SocketType,
  IN INT32 SocketId
  )
{

  if (EventType != RAS_MPLINK_EVNT_ONLINE && EventType != RAS_MPLINK_EVNT_OFFLINE) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Invalid Event Type - Must be online or offline event type \n"));
    return RAS_MPLINK_FAILURE_INVALID_EVTTYPE;
  }

  if (SocketType == RAS_MPLINK_SOCKET_TYPE_CPU) {
    if (SocketId >= MAX_SOCKET){
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Invalid CPU Socket Id. \n"));
      return RAS_MPLINK_FAILURE_INVALID_EVTTYPE;
    }
  } else {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Invalid Socket Type - Must be CPU or IOH socket. \n"));
    return RAS_MPLINK_FAILURE_INVALID_EVTTYPE;
  }

  return RAS_MPLINK_SUCCESS;
} // ValidateParameters

/**

  Issue a Power On PLD command to apply power to the new socket.

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
PowerUpOnlinedSocket (
 VOID
  )
{
  EFI_STATUS  SubStatus;

  SubStatus = mHpIOXAccess->IssuePldCmdThroIox (ISSUE_POWER_ON, (UINT8)mPendingSocId, FALSE);

  return RAS_MPLINK_SUCCESS;
} // PowerUpOnlinedSocket

/**

  Issue an Online PLD command to release reset on the new socket.

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
ReleaseResetOnlinedSocket (
 VOID
  )
{
  EFI_STATUS  SubStatus;

  SubStatus = mHpIOXAccess->IssuePldCmdThroIox (ISSUE_ONLINE, (UINT8)mPendingSocId, FALSE);

  return RAS_MPLINK_SUCCESS;
} // ReleaseResetOnlinedSocket

/**

  Issue an Offline PLD command.

  @param SocketId      - Socket ID to offline
  @param RasSocketData - pointer to the KTI RAS Socket Data struct

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
PowerDownOnlinedSocket (
  IN INT32            SocketId,
  IN KTI_SOCKET_DATA  *RasSocketData
  )
{
  EFI_STATUS  SubStatus;

  SubStatus = mHpIOXAccess->IssuePldCmdThroIox (ISSUE_OFFLINE, (UINT8)mPendingSocId, FALSE);
  //
  // Delay a little bit
  //
  KtiFixedDelay (5000);
  ClearCinitbeginOnInvalidLinks (RasSocketData);

  return RAS_MPLINK_SUCCESS;
}

/**

   Clear the RUN_BUSY flag by performing a ReadCpuPciCfgRas() poll with
   a timeout of 5 Milli Seconds.

  @param socket        - CPU Socket Node number (Socket ID)
  @param BoxInst       - Box Instance, 0 based
  @param RegOffset     - Register offset

  @retval If error return the value of the last read. If success return 0;

**/
UINT32
ClearRunBusyFlagRas (
  IN  UINT8               socket,
  IN  UINT8               BoxInst,
  IN  UINT32              RegOffset
  )
{
  UINT32 StartCount     = 500;
  UINT32 MailBoxStatus  = 0;

  MailBoxStatus = ReadCpuPciCfgRas (socket, BoxInst, RegOffset); //returns back data

  //After 5ms if run_busy is still 1, then SW cannot write to the CSRs; error needs to be O/P(last read is O/P)
  //Poll with a max timeout of 5ms
  while (MailBoxStatus & BIT31) {
    MicroSecondDelay (10);

    MailBoxStatus = ReadCpuPciCfgRas (socket, BoxInst, RegOffset);
    StartCount--;
    if (StartCount == 0) {
      break;
    }
  }

  if (MailBoxStatus & BIT31) {
    return MailBoxStatus;
  }

  return 0;
}

/**

    Send BIOS to pCode mailbox command

    @param Cpu     - Socket ID on which command is to be sent
    @param Command - Encoded command
    @param Data    - Data attached to command

    @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
SendMailBoxCmdToPcodeRas (
  IN  UINT8               Cpu,
  IN  UINT32              Command,
  IN  UINT32              Data
  )
{
  UINT32              MailBoxStatus = 0;

  //
  // Wait until pCode Mailbox is available (i.e. run_busy flag is clear)
  //
  MailBoxStatus = ClearRunBusyFlagRas (Cpu, 0, BIOS_MAILBOX_INTERFACE);
  if (MailBoxStatus & BIT31) {
    DEBUG ((EFI_D_ERROR, "\n  **ERROR! Timeout reached when trying to clear RUN_BUSY flag in PCU. \n"));
    ASSERT (FALSE);
    return MailBoxStatus;
  }

  //
  // Write data to the BIOS to Pcode Mailbox data register (now that run_busy != 1)
  //
  WriteCpuPciCfgRas (Cpu, 0, BIOS_MAILBOX_DATA, (UINT32) Data);

  //
  // Write pcode mailbox command code (now that run_busy != 1)
  //
  WriteCpuPciCfgRas (Cpu, 0, BIOS_MAILBOX_INTERFACE, (UINT32) (Command | BIT31));

  MailBoxStatus = ClearRunBusyFlagRas (Cpu, 0, BIOS_MAILBOX_INTERFACE);
  if (MailBoxStatus & BIT31) {
    DEBUG ((EFI_D_ERROR, "\n  **ERROR! Timeout reached when trying to clear RUN_BUSY flag in PCU. \n"));
    ASSERT (FALSE);
    return MailBoxStatus;
  }

  //
  // Read Mailbox data (updated/read by Pcode)
  //
  MailBoxStatus = ReadCpuPciCfgRas (Cpu, 0, BIOS_MAILBOX_INTERFACE);

  return KTI_SUCCESS;
}

/**

  Fill the mKtiInternal with latest info, and save some original settings:
    1. Use InitCpuTimer
    2. Initialize like at cold boot phase:
        KTI_HOST_OUT_PTR->socketPresentBitMap = FLD_MASK(MAX_SOCKET, 0);
        KTI_HOST_OUT_PTR->SocketFirstBus[x]
        KTI_HOST_OUT_PTR->StackBus[x][y]
        KTI_HOST_OUT_PTR->SocketLastBus[x]
        KTI_HOST_OUT_PTR->RasInProgress
        KTI_HOST_OUT_PTR->RasEvtType
        KTI_HOST_OUT_PTR->RasSocId
        mKtiInternalGlobal->SbspSoc
    3. Call UpdateCpuInfoInKtiInternal for SBSP
    4. Clear mRasSocketDataOrig, and copy over from KtiHostOutPtr:
        mRasSocketDataOrig->Cpu[x].Valid
        mRasSocketDataOrig->Cpu[x].SocId
        mRasSocketDataOrig->Cpu[x].LinkData

  @retval RAS_MPLINK_SUCCESS on successful completion

**/
RAS_MPLINK_STATUS
PrepareHostDataStruct (
   VOID
  )
{
  UINT8   Idx1;
  KTI_HOST_OUT *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  DEBUG((EFI_D_ERROR, "\n  Preparing the Host structure for O*L event. "));
  DEBUG((EFI_D_ERROR, "\n  If Hot Plug enabled, assuming MAX_SOCKET for socketPresentBitMap"));

  //
  // Re-initialize cpuFreq in RAS because it has got the incorrect uninitialized
  // value from boot time before mpcpu initialized for update.
  //
  DEBUG ((EFI_D_ERROR, "\n    Before update: "));
  DEBUG ((EFI_D_ERROR, "\n    CpuFreq = %lx", GetCpuFreq ()));
  InitCpuTimer ();
  DEBUG ((EFI_D_ERROR, "\n    After update: "));
  DEBUG ((EFI_D_ERROR, "\n    CpuFreq = %lx", GetCpuFreq ()));

  //
  // Make hot plug just like the cold reset boot flow, assuming MAX_SOCKET.
  // Fill the Host common structure with the Uncore & IIO bus numbers
  // so that the common CSR access routines become usable for all sockets once they get configured.
  // Later, warm reset topology discover will system wide re-program all routing on all sockets over
  // again, overriding this assumption.
  //
  KtiHostOutPtr->numCpus = MAX_SOCKET;
  KtiHostOutPtr->SocketPresentBitMap = FLD_MASK(MAX_SOCKET, 0);

  KtiHostOutPtr->RasInProgress = (UINT8) mInProgress;
  KtiHostOutPtr->RasEvtType = (UINT8) mPendingEvtType;
  KtiHostOutPtr->RasSocId = (UINT8) mPendingSocId;

  mKtiInternalGlobal->SbspSoc = mSbspId;
  mKtiInternalGlobal->MmcfgBase = KTI_HOST_OUT_PTR->mmCfgBase;
  mKtiInternalGlobal->KtiSetup = (UINT32) (UINTN)KTI_HOST_IN_PTR;
  mKtiInternalGlobal->KtiVar = (UINT32) (UINTN) KTI_HOST_OUT_PTR;
  mKtiInternalGlobal->KtiNvram = (UINT32) (UINTN) KTI_HOST_NVRAM_PTR;

  //
  // Initialize CpuType/CpuSubType/CpuStepping in KtiInternalGlobal
  //
  KtiGetCpuTypeInfo (mKtiInternalGlobal);

  UpdateCpuInfoInKtiInternal (mKtiInternalGlobal, mSbspId);

  //
  // Initialize the SocketData to remember what is the original LEP before the O*L event occured
  //
  ZeroMem ((VOID *)mRasSocketDataOrig, sizeof (KTI_SOCKET_DATA));
  for (Idx1 = 0; Idx1 < MAX_SOCKET; ++Idx1) {
    if (KtiHostOutPtr->CpuInfo[Idx1].Valid == TRUE) {
      mRasSocketDataOrig->Cpu[Idx1].Valid = TRUE;
      mRasSocketDataOrig->Cpu[Idx1].SocId = Idx1;
      CopyMem ((VOID *)(mRasSocketDataOrig->Cpu[Idx1].LinkData), (VOID *)(KtiHostOutPtr->CpuInfo[Idx1].LepInfo), sizeof (mRasSocketDataOrig->Cpu[Idx1].LinkData));
    }
  }

  return RAS_MPLINK_SUCCESS;
} // PrepareHostDataStruct

/**

  Print out some KTI RAS variables to serial log.

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
DataDumpRasMpLink (
   VOID
  )
{
  KTI_HOST_OUT *KtiHostOutPtr;
  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  DataDumpRasMpLink:    "));
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    platformType  %u ", mPlatformType));
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    numCpus  %u ", KtiHostOutPtr->numCpus));
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    socketPresentBitMap  0x%08X ", KtiHostOutPtr->SocketPresentBitMap));

    return RAS_MPLINK_SUCCESS;
} // DataDumpRasMpLink

/**

  Set up SBSP's MMCFG SAD so that the incoming socket's config space can be accessed from SBSP.

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
OpenSbspConfigSpaceForHotAdd (
   VOID
  )
{
  UINT8                           Idx, NumBuses,TargetsPerSocket;
  UINT32                          MmcfgGranularity;
  UINT32                          MmCfgSize;
  UINT32                          MmcfgValidTarget;

  KTI_HOST_OUT                    *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  MmCfgSize = KTI_HOST_IN_PTR->mmCfgSize;
  //
  // We pre-program the target list during boot, and will only need to update
  // MMCFG_RULE0.valid_targets in this function.
  //
  ChaGetMmcfgRuleValidTarget ((UINT8)mSbspId, 0, &MmcfgValidTarget);

  if (MmCfgSize <= 0x10000000) {
    MmcfgGranularity = SAD_BUS_GRANTY;
  } else {
    MmcfgGranularity = (MmCfgSize / 1024 / 1024 / SAD_BUS_TARGETS);
  }

  Idx = 0;
  NumBuses = (UINT8)(((((KtiHostOutPtr->CpuInfo[mPendingSocId].CpuRes.BusLimit - KtiHostOutPtr->CpuInfo[mPendingSocId].CpuRes.BusBase + 1) / MmcfgGranularity) - 1)) + 1);
  TargetsPerSocket = NumBuses;
  while (NumBuses) {
    MmcfgValidTarget |= 1 << (Idx + TargetsPerSocket * mPendingSocId);
    NumBuses--;
    Idx++;
  }

  ChaSetMmcfgRuleValidTarget ((UINT8)mSbspId, 0, MmcfgValidTarget);

  //
  // Plan is to move the StackBus programming to boot flow as well,
  // but looks like currently boot flow leaves the StackBus values at 0xff
  //
  KtiHostOutPtr->SocketFirstBus[mPendingSocId] = 0x20 * TargetsPerSocket * ((UINT8) mPendingSocId);
  for (Idx = 0; Idx < MAX_IIO_STACK; ++Idx) {
    KtiHostOutPtr->StackBus[mPendingSocId][Idx] = KtiHostOutPtr->SocketFirstBus[mPendingSocId] + Idx;
  }
  KtiHostOutPtr->SocketLastBus[mPendingSocId] = KtiHostOutPtr->SocketFirstBus[mPendingSocId] + ((0x20 * TargetsPerSocket) - 1);
  KtiHostOutPtr->Socket10nmUboxBus0[mPendingSocId] = KtiHostOutPtr->SocketLastBus[mPendingSocId] - BUS_PER_UBOX_10nm + 1;
  KtiHostOutPtr->StackBus[mPendingSocId][UBOX_STACK] = KtiHostOutPtr->Socket10nmUboxBus0[mPendingSocId];

  return RAS_MPLINK_SUCCESS;
} // OpenSbspConfigSpaceForHotAdd

/**

  The offline socket's config space needs to be removed from SBSP's MMCFG SAD.
  It will be copied to all active PBSP later in RasMpLinkMileStone10.

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
AdjustSbspConfigSpaceForHotRemoval (
   VOID
  )
{
  UINT8                           Idx, NumBuses,TargetsPerSocket;
  UINT32                          MmcfgGranularity;
  UINT32                          MmCfgSize;
  KTI_HOST_OUT                    *KtiHostOutPtr;
  UINT32                          MmcfgValidTarget;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  MmCfgSize = KTI_HOST_IN_PTR->mmCfgSize;

  //
  // We will only need to update MMCFG_RULE0.valid_targets in this function
  // No need to change the StackBus programming here
  //
  ChaGetMmcfgRuleValidTarget ((UINT8)mSbspId, 0, &MmcfgValidTarget);
  if (MmCfgSize <= 0x10000000) {
    MmcfgGranularity = SAD_BUS_GRANTY;
  } else {
    MmcfgGranularity = (MmCfgSize / 1024 / 1024 / SAD_BUS_TARGETS);
  }

  Idx = 0;
  NumBuses = (UINT8)(((((KtiHostOutPtr->CpuInfo[mPendingSocId].CpuRes.BusLimit - KtiHostOutPtr->CpuInfo[mPendingSocId].CpuRes.BusBase + 1) / MmcfgGranularity) - 1)) + 1);
  TargetsPerSocket = NumBuses;
  while (NumBuses) {
    MmcfgValidTarget &= ~(1 << (Idx + TargetsPerSocket * mPendingSocId));
    NumBuses--;
    Idx++;
  }

  ChaSetMmcfgRuleValidTarget ((UINT8)mSbspId, 0, MmcfgValidTarget);

  return RAS_MPLINK_SUCCESS;
} // AdjustSbspConfigSpaceForHotRemoval

/**

  Collect LEP on all pre-existing sockets, and check if any of the links
  are connected to the socket being onlined. If yes, report the decoded LEP
  and update mRasSocketData->Cpu[mPendingSocId].LinkData[x]. If no ports are
  connected to new socket it means the links failed to train in slow mode.

  @retval RAS_MPLINK_SUCCESS - if at least one of the pre-existing sockets
                               connects to the pending socket
  @retval RAS_MPLINK_FAILURE_SOC_UNREACHABLE - if no links report connection
                                               to the new socket

**/
RAS_MPLINK_STATUS
CollectLepForOnlineEvent (
   VOID
  )
{
  UINT8   Index;
  UINT8   KtiLink, LocalKtiLink;
  UINT8   PeerSocId;
  BOOLEAN SocValid= FALSE;
  KTI_HOST_OUT *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Collecting LEP for ONLINE event."));
  RasInitSocketData (mRasSocketData);

  //
  // Since LEP is affected by the O*L event, recollect the LEP of the sockets that exist
  // already. Note that we can not reach the incoming socket itself since the route is not
  // yet set for it. But we don't need to reach the incoming socket to know its LEP. We can
  // deduce its LEP from its immediate neighbors' LEP.
  //

  //
  // Update the mRasSocketData structure (Equivalent to identifying what sockets are out there already)
  //
  for (Index = 0; Index < MAX_SOCKET; ++Index) {
    if (KtiHostOutPtr->CpuInfo[Index].Valid == TRUE) {
      mRasSocketData->Cpu[Index].Valid = TRUE;
      mRasSocketData->Cpu[Index].SocId = Index;
    }
  }

  //
  // We don't know who are the immediate neighbors of the incoming socket. So recollect the LEP
  // of all the existing sockets.
  //
  for (Index = 0; Index < MAX_SOCKET; ++Index) {
    if (mRasSocketData->Cpu[Index].Valid == TRUE) {
      ParseCpuLep (mRasSocketData, mKtiInternalGlobal, Index);
    }
  }

  //
  // Deduce the LEP of the Onlined socket from its neighbor's LEP
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Onlined Socket CPU%u Link Exchange", mPendingSocId));
  for (Index = 0; Index < MAX_SOCKET; ++Index) {
    if (mRasSocketData->Cpu[Index].Valid == TRUE) {
      for (KtiLink = 0; KtiLink < KtiHostOutPtr->KtiPortCnt; ++KtiLink) {
        if (mRasSocketData->Cpu[Index].LinkData[KtiLink].Valid == TRUE &&
            mRasSocketData->Cpu[Index].LinkData[KtiLink].PeerSocId == (UINT32)mPendingSocId &&
            mRasSocketData->Cpu[Index].LinkData[KtiLink].PeerSocType == (UINT32)mPendingSocType) {
          SocValid = TRUE;
          LocalKtiLink = (UINT8)mRasSocketData->Cpu[Index].LinkData[KtiLink].PeerPort;
          KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " : LEP%u(%u,CPU%u)", LocalKtiLink, KtiLink, Index));
          mRasSocketData->Cpu[mPendingSocId].LinkData[LocalKtiLink].Valid = TRUE;
          mRasSocketData->Cpu[mPendingSocId].LinkData[LocalKtiLink].PeerSocId = Index;
          mRasSocketData->Cpu[mPendingSocId].LinkData[LocalKtiLink].PeerSocType = SOCKET_TYPE_CPU;
          mRasSocketData->Cpu[mPendingSocId].LinkData[LocalKtiLink].PeerPort = KtiLink;
        }
      } // for (KtiLink...)
    }
  } // for (Index...)

  //
  // Make sure the socket being Onlined is reachable
  //
  if (SocValid != TRUE) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  Onlined socket is unreachable. At least one link to the socket must reach Normal Opeartion.\n  Operation Aborted"));
    return RAS_MPLINK_FAILURE_SOC_UNREACHABLE;
  }

  //
  // Mark the socket as valid
  //
  mRasSocketData->Cpu[mPendingSocId].Valid = TRUE;
  mRasSocketData->Cpu[mPendingSocId].SocId = (UINT8)mPendingSocId;

  //
  // The UPI RAS state machine is written to handle only one online event at a time. So even if the system
  // onlined more than one socket, this step will ensure that LEP of the links connected to socket other than
  // the one being handled (i.e the socket id/type passed to UPI RAS driver) is marked as "Invalid".
  //
  for (Index = 0; Index < MAX_SOCKET; ++Index) {
    if (mRasSocketData->Cpu[Index].Valid == TRUE) {
      for (KtiLink = 0; KtiLink < KtiHostOutPtr->KtiPortCnt; ++KtiLink) {
        if (mRasSocketData->Cpu[Index].LinkData[KtiLink].Valid == TRUE) {
          PeerSocId = (UINT8)mRasSocketData->Cpu[Index].LinkData[KtiLink].PeerSocId;
          if (mRasSocketData->Cpu[PeerSocId].Valid != TRUE) {
            KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, " Clearing LEP of CPU%u Link%u connected to offlined CPU %u", Index, KtiLink, PeerSocId));
            ZeroMem ((VOID *)&mRasSocketData->Cpu[Index].LinkData[KtiLink], sizeof (mRasSocketData->Cpu[Index].LinkData[KtiLink]));
          }
        }
      } // for (KtiLink...)
    }
  } // for (Index...)

  return RAS_MPLINK_SUCCESS;
} // CollectLepForOnlineEvent

/**

  Mark the socket being offlined as invalid and also update LEP info to mark the ports
  connected to the socket being offlined as invalid.

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
CollectLepForOfflineEvent (
   VOID
  )
{
  UINT8             Index;
  UINT8             KtiLink;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Adjusting the LEP for OFFLINE event. "));
  ZeroMem ((VOID *)mRasSocketData, sizeof (KTI_SOCKET_DATA));

  //
  // Update the mRasSocketData structure (Equivalent to identifying what sockets are out there already)
  // and exclude the socket that is being offlined (i.e it is marked as invalid)
  //
  for (Index = 0; Index < MAX_SOCKET; ++Index) {
    if (KTI_HOST_OUT_PTR->CpuInfo[Index].Valid != TRUE || Index == mPendingSocId) {
      continue;
    }
    mRasSocketData->Cpu[Index].Valid = TRUE;
    mRasSocketData->Cpu[Index].SocId = Index;
    CopyMem ((VOID *)mRasSocketData->Cpu[Index].LinkData, (VOID *)KTI_HOST_OUT_PTR->CpuInfo[Index].LepInfo, sizeof (mRasSocketData->Cpu[Index].LinkData));
  }

  //
  // Mark the immediate neighbors' links connecting to the offlined socket as Invalid
  //
  for (Index = 0; Index < MAX_SOCKET; ++Index) {
    if (mRasSocketData->Cpu[Index].Valid == TRUE) {
      for (KtiLink = 0; KtiLink < KTI_HOST_OUT_PTR->KtiPortCnt; ++KtiLink) {
        if (mRasSocketData->Cpu[Index].LinkData[KtiLink].Valid == TRUE &&
            mRasSocketData->Cpu[Index].LinkData[KtiLink].PeerSocId == (UINT32)mPendingSocId &&
            mRasSocketData->Cpu[Index].LinkData[KtiLink].PeerSocType == (UINT32)mPendingSocType) {
            ZeroMem ((VOID *)&mRasSocketData->Cpu[Index].LinkData[KtiLink], sizeof (mRasSocketData->Cpu[Index].LinkData[KtiLink]));
        }
      } // for(KtiLink...)
    }
  } // for(Index...)
  return RAS_MPLINK_SUCCESS;
} // CollectLepForOfflineEvent

/**

  Copy LEP and Topology Tree info to KtiHostOutPtr from mRasSocketData.

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
CopyTopologyInfoToHostParm (
  )
{
  UINT8                      Index;
  KTI_HOST_OUT               *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  for (Index = 0; Index < MAX_SOCKET; ++Index) {
    if (mRasSocketData->Cpu[Index].Valid == TRUE) {
      KtiHostOutPtr->CpuInfo[Index].Valid = mRasSocketData->Cpu[Index].Valid;
      KtiHostOutPtr->CpuInfo[Index].SocId = mRasSocketData->Cpu[Index].SocId;

      // Copy LEP info
      CopyMem (
         (VOID *) KtiHostOutPtr->CpuInfo[Index].LepInfo,
         (VOID *) mRasSocketData->Cpu[Index].LinkData,
          sizeof (KtiHostOutPtr->CpuInfo[Index].LepInfo)
         );

      // Copy Topology tree info
      CopyMem (
        (VOID *)KtiHostOutPtr->CpuInfo[Index].TopologyInfo,
        (VOID *)mRasSocketData->CpuTree[Index],
        sizeof (KtiHostOutPtr->CpuInfo[Index].TopologyInfo)
        );
    }
  }
  return RAS_MPLINK_SUCCESS;
} // CopyTopologyInfoToHostParm

/**

  Set up the Config Access Path and Boot Path (if applicable) for the online event. To minimize the
  number of RTA entries touched, the onlined socket is appended to the topology tree of the
  SBSP for setting up Config Access Path. For Boot Path, the Onlined CPU socket uses one of its
  immediate neighbor's route to LIOH.

  @retval RAS_MPLINK_SUCCESS - on successful completion
  @retval RAS_MPLINK_FAILURE_SOC_UNREACHABLE - if no sockets show a link connected to the new socket
  @retval RAS_MPLINK_FAILURE_GENERAL

**/
RAS_MPLINK_STATUS
SetConfigAndBootPath (
   VOID
  )
{
  TOPOLOGY_TREE_NODE         Parent, Child, Temp;
  UINT8                      Index, KtiLink = 0;
  KTI_HOST_OUT *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Setting config path for online event"));

  //
  // Use SBSP's optimum route tree as the minimum path tree
  //
  CopyMem ((VOID *)mRasSocketData->SbspMinPathTree, (VOID *)KtiHostOutPtr->CpuInfo[mSbspId].TopologyInfo, sizeof (KtiHostOutPtr->CpuInfo[mSbspId].TopologyInfo));

  //
  // Append the Hot Added socket to SBSP's minimum path tree
  //
  Parent.RawNode = 0;
  Index = 0;
  do {
    Temp.RawNode = mRasSocketData->SbspMinPathTree[Index].RawNode;
    if (Temp.Node.Valid == TRUE && Temp.Node.SocType == SOCKET_TYPE_CPU) {
      for(KtiLink = 0; KtiLink < KtiHostOutPtr->KtiPortCnt; ++KtiLink) {
        //
        // Check if this node has a link connected to the Hot Added socket
        //
        if (mRasSocketData->Cpu[Temp.Node.SocId].LinkData[KtiLink].Valid == TRUE &&
            mRasSocketData->Cpu[Temp.Node.SocId].LinkData[KtiLink].PeerSocId == (UINT32)mPendingSocId &&
            mRasSocketData->Cpu[Temp.Node.SocId].LinkData[KtiLink].PeerSocType== (UINT32)mPendingSocType) {
          Parent.RawNode = Temp.RawNode;
          break;
        }
      }
    }
    if (Parent.RawNode != 0) {
      break;
    }
    ++Index;
  } while (Index < MAX_TREE_NODES);

  if (Parent.Node.Valid != TRUE) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n    Onlined socket is unreachable. Appending the Hot Added socket to SBSP Min Path Tree failed.\n    Operation Aborted!!!"));
    return RAS_MPLINK_FAILURE_SOC_UNREACHABLE;
  }

  Child.RawNode = 0;
  Child.Node.Valid = TRUE;
  Child.Node.SocId = mPendingSocId;
  Child.Node.SocType = mPendingSocType;
  //
  // One level down from parent:
  //
  Child.Node.Hop = Parent.Node.Hop + 1;
  //
  // Remember who is the immediate parent of this child:
  //
  Child.Node.ParentIndex = Index;
  Child.Node.ParentPort = mRasSocketData->Cpu[Parent.Node.SocId].LinkData[KtiLink].PeerPort;

  //
  // Add the node to the tree
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Adding Socket%u to SBSP Min Path tree", Child.Node.SocId));
  if (AddNodeToTopologyTree (mRasSocketData->SbspMinPathTree, &Child) != KTI_SUCCESS) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n    Adding CPU%u to SBSP Min Path tree failed - Data Structure Overflow.",  Child.Node.SocId));
    return RAS_MPLINK_FAILURE_GENERAL;
  }

  SetupSbspConfigAccessPath (mRasSocketData, mKtiInternalGlobal, &Parent, &Child, KtiLink);
  CopyMem ((VOID *)mRasSocketData->SbspMinPathTree,
    (VOID *)KtiHostOutPtr->CpuInfo[Parent.Node.SocId].TopologyInfo,
    sizeof (KtiHostOutPtr->CpuInfo[Parent.Node.SocId].TopologyInfo));

  //
  // Update onlining socket internal data
  //
  UpdateCpuInfoInKtiInternal (mKtiInternalGlobal, (UINT8)mPendingSocId);
  SetupRemoteCpuBootPath (mRasSocketData, mKtiInternalGlobal, &Parent, &Child, KtiLink);
  return RAS_MPLINK_SUCCESS;
} // SetConfigAndBootPath

/**

  Make sure the new socket's SEC code checked into Nonstickypad2[0].

  @retval RAS_MPLINK_SUCCESS - if new socket successfully checked in
  @retval RAS_MPLINK_FAILURE_CHKIN - on failure to check-in

**/
RAS_MPLINK_STATUS
VerifyCpuCheckIn (
   VOID
  )
{
  BOOLEAN  bCheckInBitsSet;

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Verifying if the onlined CPU checked in."));
  UBoxCheckInBitsIsSetInScratchpad ((UINT8)mPendingSocId, 0, &bCheckInBitsSet);
  if (!bCheckInBitsSet) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n    Onlined socket failed to check in."));
    return RAS_MPLINK_FAILURE_CHKIN;
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Onlined socket successfully checked in."));
  return RAS_MPLINK_SUCCESS;
} // VerifyCpuCheckIn

/**

  Instruct the Onlined CPU to tear down NEM and halt.

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
HaltPbsp (
   VOID
  )
{
  WriteCpuPciCfgRas ((UINT8)mPendingSocId, 0, SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR, (UINT32)-1);
  return RAS_MPLINK_SUCCESS;
} // HaltPbsp

/**

  Issue Warm Reset through a platform PLD command.

  @param TimeSliced - TRUE/FALSE indicating if we're in time-sliced mode

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
IssueWarmResetOfOnlinedCpu (
  IN UINT8          TimeSliced
  )
{
  mHpIOXAccess->IssuePldCmdThroIox (ISSUE_WARM_RESET, (UINT8)mPendingSocId, TimeSliced);
  return RAS_MPLINK_SUCCESS;
} // IssueWarmResetOfOnlinedCpu

/**

  Routine to sync up the onlined socket, update the necessary scratch registers and issue "BOOT_GO"
  command to bring it under BIOS control.

  @param SocketData        - info such as CPUs populated and their LEP
  @param KtiInternalGlobal - structure with KTIRC global variables
  @param RstType           - reset type - post power good cycle or post warm reset

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
SyncUpPbspForRas (
  IN  KTI_SOCKET_DATA          *SocketData,
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN  UINT8                    RstType
  )
{
  UINT32                               SbspLtTarget;
  UINT32                               SbspLtTargetEnabled;
  UINT32                               ApLtTarget;
  UINT32                               ApLtTargetEnabled;

  if (RstType == POST_RESET_POWERGOOD) {
    UBoxUpdateSbspSocIdInScratchpad ((UINT8)mPendingSocId, 0, (UINT8) (KtiInternalGlobal->SbspSoc & 0x0F));
    UpdateCpuList (FALSE);
  }

  //
  // BIOS has to initialize the following fields of SAD_TARGET_CHABC_SAD1 register for the new socket.
  //
  ChaSadTargetCfg ((UINT8)mPendingSocId, 0, KtiInternalGlobal->SbspSoc, KtiInternalGlobal->SbspSoc, CHA_DEFAULT_CFG, CHA_DEFAULT_CFG, CHA_DEFAULT_CFG);

  //
  // BIOS has to initialize the LT_CONTROL register for the new socket.
  // So we just replicate the register setting from SBSP to the new socket.
  //
  ChaGetLtTargetAndStatus (KtiInternalGlobal->SbspSoc, 0, &SbspLtTarget, &SbspLtTargetEnabled);
  if (SbspLtTargetEnabled & 0x1) { // LT_Target_enable is set?
    ApLtTargetEnabled = SbspLtTargetEnabled;
    ApLtTarget        = SbspLtTarget;
    ChaSetLtTargetAndStatus ((UINT8)mPendingSocId, 0, ApLtTarget, ApLtTargetEnabled);
  }

  //
  // In LT enabled system, releasing the new socket may cause a LT_SHUTDOWN, which causes a surprise warm reset.
  // Set bit29 of SP7 to indicate we are releasing the onlined socket, so that if LT_SHUTDOWN happens,
  // BIOS can detect this and handle it.
  //
  UBoxSetRemoteSocketReleasedFlagInScratchpad ((UINT8)mPendingSocId, 0, 1);

  //
  // Local Node ID in UBOX
  //
  UBoxConfigureCpuNodeId ((UINT8)mPendingSocId, 0, mPendingSocId, 0xFFFFFFFF, 0xFFFFFFFF);

  //
  // Issue BOOT_GO command
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Issuing BOOT_GO command to CPU%u. ", mPendingSocId));
  UBoxIssueBootGoCommand ((UINT8)mPendingSocId, 0);

  return RAS_MPLINK_SUCCESS;
} // SyncUpPbspForRas

RAS_MPLINK_STATUS
UpdateCpuList (
   IN BOOLEAN AllCpus
  )
{
  UINT8                           Index;
  UINT32                          MmcfgRule0;

  //
  // Update CPU List in ScratchPad register
  //
  UBoxUpdateCpuListInScratchpad ((UINT8)mPendingSocId, AllCpus, mRasSocketData);

  if (AllCpus == TRUE) {
    //
    // Update it on all CPUs populated
    //
    ChaGetMmcfgTargetListAndRules ((UINT8)mSbspId, 0, NULL, &MmcfgRule0, NULL);
    for (Index = 0; Index < MAX_SOCKET; ++Index) {
      if (mRasSocketData->Cpu[Index].Valid == TRUE){
        //
        // Update MMCFG_RULE0.valid_targets on all sockets but SBSP
        //
        if (Index != (UINT8)mSbspId) {
          ChaSetMmcfgRule (Index, 0, MmcfgRule0);
        }
      }
    }
  }

  return RAS_MPLINK_SUCCESS;
}

/**

  Program the new socket's DRAM decoder registers to add the pre-existing
  sockets' DRAM to its memory map, thus allowing threads on new socket to
  reach the pre-existing sockets' memory.

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
SyncUpDramSad (
  VOID
  )
{
  UINT8                                McId, Stack;
  UINT32 Data32;


  //
  // Replicate the SBSP's DRAM decoder setting into the onlined CPU socket
  // Replicate the DRAM decoders starting with entry 0.
  //
    // Read SBSP's DRAM decoder entries

  ChaRasReplicateDramSetting ((UINT8)mSbspId, 0, (UINT8)mPendingSocId);

  for (Stack=0; Stack < MAX_IIO_STACK; Stack ++) {
    if (KTI_HOST_OUT_PTR->stackPresentBitmap[mSbspId] & (BIT0 << Stack)) {
      Data32 = ReadCpuPciCfgRas ((UINT8)mSbspId, Stack, TOLM_IIO_VTD_REG);
      WriteCpuPciCfgRas ((UINT8)mPendingSocId, Stack, TOLM_IIO_VTD_REG, Data32);

      Data32 = ReadCpuPciCfgRas ((UINT8)mSbspId, Stack, TOHM_N0_IIO_VTD_REG);
      WriteCpuPciCfgRas ((UINT8)mPendingSocId, Stack, TOHM_N0_IIO_VTD_REG, Data32);
      Data32 = ReadCpuPciCfgRas ((UINT8)mSbspId, Stack, TOHM_N1_IIO_VTD_REG);
      WriteCpuPciCfgRas ((UINT8)mPendingSocId, Stack, TOHM_N1_IIO_VTD_REG, Data32);

      //
      // Program ME SEG Base/Limit in IIO
      //
      Data32 = ReadCpuPciCfgRas ((UINT8)mSbspId, Stack, IMR0BASE_N0_IIO_VTD_REG);
      WriteCpuPciCfgRas ((UINT8)mPendingSocId, Stack, IMR0BASE_N0_IIO_VTD_REG, Data32);
      Data32 = ReadCpuPciCfgRas ((UINT8)mSbspId, Stack, IMR0BASE_N1_IIO_VTD_REG);
      WriteCpuPciCfgRas ((UINT8)mPendingSocId, Stack, IMR0BASE_N1_IIO_VTD_REG, Data32);
      Data32 = ReadCpuPciCfgRas ((UINT8)mSbspId, Stack, IMR0MASK_N0_IIO_VTD_REG);
      WriteCpuPciCfgRas ((UINT8)mPendingSocId, Stack, IMR0MASK_N0_IIO_VTD_REG, Data32);
      Data32 = ReadCpuPciCfgRas ((UINT8)mSbspId, Stack, IMR0MASK_N1_IIO_VTD_REG);
      WriteCpuPciCfgRas ((UINT8)mPendingSocId, Stack, IMR0MASK_N1_IIO_VTD_REG, Data32);

      //
      // Program TSEG Base/Limit in IIO
      //
      Data32 = ReadCpuPciCfgRas ((UINT8)mSbspId, Stack, TSEG_N0_IIO_VTD_REG);
      WriteCpuPciCfgRas ((UINT8)mPendingSocId, Stack, TSEG_N0_IIO_VTD_REG, Data32);
      Data32 = ReadCpuPciCfgRas ((UINT8)mSbspId, Stack, TSEG_N1_IIO_VTD_REG);
      WriteCpuPciCfgRas ((UINT8)mPendingSocId, Stack, TSEG_N1_IIO_VTD_REG, Data32);
    }
  }

  for (McId = 0; McId < KTI_HOST_OUT_PTR->CpuInfo[mSbspId].TotM2m; McId++) {
    MemSyncPerMcCsrs ((UINT8)mSbspId, (UINT8)mPendingSocId, McId);
  }

  //
  // Program ME SEG Base/Limit in CHA
  //
  ChaRasReplicateMeSeg ((UINT8)mSbspId, 0, (UINT8)mPendingSocId);

  return RAS_MPLINK_SUCCESS;
}

/**

  Include the Onlined socket in System Interrupt Domain

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
AdjustIntDomainForHotAdd (
   VOID
  )
{
  UINT8           Idx1, Misclist=0, CpuList=0, IpiList=0;
  UINT32          Data32 = 0;
  UINT32          BcastList = 0;

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    Adjusting the System Interrupt Domain to comprehend the Onlined CPU socket."));

  for (Idx1 = 0; Idx1 < MAX_SOCKET; ++Idx1) {
    if (mRasSocketData->Cpu[Idx1].Valid == TRUE){
      CpuList |= (1 << Idx1);
      Misclist |= (1<< Idx1);
    }
  }

  //
  // For all the existing sockets, IPI broadcast has to exclude the online socket during milestone12.
  // For the online socket, make it to have only itself for start up of its ap threads incl. smm relocation.
  // Later, all will be set to be included all from subsequent calls by cpu ras code.
  //
  IpiList = CpuList & (~(1 << mPendingSocId));
  for (Idx1 = 0; Idx1 < MAX_SOCKET; ++Idx1) {
    if (mRasSocketData->Cpu[Idx1].Valid == TRUE && Idx1 != mPendingSocId ){
      UBoxConfigureCpuBroadcastList (Idx1, 0, IpiList, CpuList, Misclist, CpuList, &Data32);
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    Cpu%u BCASTLIST_UBOX_CFG_REG = %x", Idx1, Data32 ));
    }
  }

  UBoxConfigureCpuBroadcastList ((UINT8)mPendingSocId, 0, (1 << mPendingSocId), CpuList, Misclist, CpuList, &BcastList);

  return RAS_MPLINK_SUCCESS;
} // AdjustIntDomainForHotAdd

/**

  In order to prepare the links for Slow mode training, on all invalid links:
    1. Set c_init_begin
    2. Clear fake_l1
    3. Issue phy reset via cp_reset

  @param SocketData - SocketData struct with information about the system

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
PrepareForSlowSpeedTraining (
  IN KTI_SOCKET_DATA        *SocketData
)
{
  UINT8                           Socket, Port;
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT KtiReutPhCtr1;
  KTIPHYPWRCTRL_KTI_REUT_STRUCT   KtiPhyPwrCtrl;
  KTI_HOST_OUT                    *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (KtiHostOutPtr->CpuInfo[Socket].Valid == TRUE) {
      for (Port = 0; Port < GetKtiPortCnt(); ++Port) {
        if (KtiHostOutPtr->CpuInfo[Socket].LepInfo[Port].Valid == FALSE) {
          //
          // 1. Set c_init_begin
          //
          KtiReutPhCtr1.Data = ReadCpuPciCfgRas (Socket, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
          KtiReutPhCtr1.Bits.c_init_begin = 1;
          WriteCpuPciCfgRas (Socket, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);

          //
          // 2. Clear fake_l1
          //
          KtiPhyPwrCtrl.Data = ReadCpuPciCfgRas (Socket, Port, KTIPHYPWRCTRL_KTI_REUT_REG);
          KtiPhyPwrCtrl.Bits.fake_l1 = 0;
          WriteCpuPciCfgRas (Socket, Port, KTIPHYPWRCTRL_KTI_REUT_REG, KtiPhyPwrCtrl.Data);

          //
          // 3. Issue phy reset via cp_reset
          //
          KtiReutPhCtr1.Data = ReadCpuPciCfgRas (Socket, Port, KTIREUT_PH_CTR1_KTI_REUT_REG);
          KtiReutPhCtr1.Bits.cp_reset = 1;
          WriteCpuPciCfgRas (Socket, Port, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);
        }
      }
    }
  }

  return RAS_MPLINK_SUCCESS;
} // PrepareForSlowSpeedTraining

/**

  Clear KtiReutPhCtr1.c_init_begin on all invalid KTI ports.

  @param SocketData - SocketData struct with information about the system

  @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
ClearCinitbeginOnInvalidLinks (
  IN KTI_SOCKET_DATA        *SocketData
  )
{
  UINT8                           KtiLink, Socket;
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT KtiReutPhCtr1;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (SocketData->Cpu[Socket].Valid == TRUE) {
      for (KtiLink = 0; KtiLink < GetKtiPortCnt(); ++KtiLink) {
        if (SocketData->Cpu[Socket].LinkData[KtiLink].Valid != TRUE) {
          KtiReutPhCtr1.Data = ReadCpuPciCfgRas (Socket, KtiLink, KTIREUT_PH_CTR1_KTI_REUT_REG);
          KtiReutPhCtr1.Bits.c_init_begin = 0;
          WriteCpuPciCfgRas (Socket, KtiLink, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);
        }
      }
    }
  }
  return RAS_MPLINK_SUCCESS;
} // ClearCinitbeginOnInvalidLinks

KTI_STATUS
KtiFullSpeedTransitionForHotAdd (
  VOID
  )
{
  UINT8         LinkIndex;
  UINT32        LinkSpeed;
  KTI_LINK_DATA *pPeerInfo;

  //
  // Make sure per-link setup options don't conflict between the peer sockets
  //
  NormalizeLinkOptions (mRasSocketData, mKtiInternalGlobal);

  ClearOrSetKtiDfxLckForFullSpeedTransition (mKtiInternalGlobal, 0);
  ProgramLateActionForHotAdd (1);
  ProgramPhyLayerForHotAdd ();
  ProgramLinkLayerForHotAdd ();
  ProgramLateActionForHotAdd (0);
  ClearOrSetKtiDfxLckForFullSpeedTransition (mKtiInternalGlobal, 1);

  //
  // KTI Link Speed is selected during boot flow in SelectSupportedKtiLinkSpeed
  // and is stored in KTI_HOST_OUT_PTR->OutKtiLinkSpeed and
  // setup->kti.PhyLinkPerPortSetting[Index].Phy[LinkIndex].KtiLinkSpeed
  //
  switch (KTI_HOST_OUT_PTR->OutKtiLinkSpeed) {
    case (SPEED_REC_96GT):
      LinkSpeed = 12;
      break;
    case (SPEED_REC_104GT):
      LinkSpeed = 13;
      break;
    case (SPEED_REC_112GT):
      LinkSpeed = 14;
      break;
    default:
      LinkSpeed = 12;
      break;
  }

  //
  // Send the pcode mailbox command and Prepare pre-existing sockets' KTI ports for training to op speed
  //
  for (LinkIndex = 0; LinkIndex < KTI_HOST_OUT_PTR->KtiPortCnt; LinkIndex++) {
    if (mRasSocketData->Cpu[mPendingSocId].LinkData[LinkIndex].Valid == TRUE) {
      PcuSetUpiFrequency ((UINT8)mPendingSocId, LinkIndex, LinkSpeed);
      pPeerInfo = &(mRasSocketData->Cpu[mPendingSocId].LinkData[LinkIndex]);
      ReceivingKtiPortsFullSpeedTransition (mKtiInternalGlobal, (UINT8)pPeerInfo->PeerSocId, (UINT8)pPeerInfo->PeerPort);
    }
  }

  return RAS_MPLINK_SUCCESS;
} // KtiFullSpeedTransitionForHotAdd

/**

 Sequence of setting bits and issuing phy/link layer resets that prepares
 the pre-existing sockets' KTI ports for op speed training.

 @param KtiInternalGlobal - KTIRC internal variables.
 @param Socket            - Socket ID
 @param LinkIndex         - Link number

 @retval RAS_MPLINK_SUCCESS - on successful completion

**/
KTI_STATUS
ReceivingKtiPortsFullSpeedTransition (
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN  UINT8                    Socket,
  IN  UINT8                    LinkIndex
  )
{
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT  KtiReutPhCtr1;
  KTIREUT_PH_CIS_KTI_REUT_STRUCT   KtiReutPhCis;
  KTILCL_KTI_LLPMON_STRUCT         KtiLcl;
  KTIREUT_PH_CSS_KTI_REUT_STRUCT   KtiReutPhCss;


  //
  // 1.  BIOS must clear KTIREUT.PH_CIS.sp_rx_calib on the Host socket.
  //
  KtiReutPhCis.Data = ReadCpuPciCfgRas (Socket, LinkIndex, KTIREUT_PH_CIS_KTI_REUT_REG);
  KtiReutPhCis.Bits.sp_rx_calib = 0;
  WriteCpuPciCfgRas (Socket, LinkIndex, KTIREUT_PH_CIS_KTI_REUT_REG, KtiReutPhCis.Data);

  //
  // 2.  BIOS clears cinitbegin on Host socket.
  //    This breaks the communication w/ new socket.
  //
  KtiReutPhCtr1.Data = ReadCpuPciCfgRas (Socket, LinkIndex, KTIREUT_PH_CTR1_KTI_REUT_REG);
  KtiReutPhCtr1.Bits.c_init_begin = 0;
  WriteCpuPciCfgRas (Socket, LinkIndex, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);

  //
  // 3.  BIOS issues phy layer reset via cpreset on the Host socket.
  //
  KtiReutPhCtr1.Data = ReadCpuPciCfgRas (Socket, LinkIndex, KTIREUT_PH_CTR1_KTI_REUT_REG);
  KtiReutPhCtr1.Bits.cp_reset = 1;
  WriteCpuPciCfgRas (Socket, LinkIndex, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);

  //
  // 4. BIOS verifies that the Host socket has entered reset by verifying that
  //    PH_CSS.s_tx_state is in ResetC state (encoding 0b00010 according to Table 12-26
  //    of the KTI Spec).
  //
    do {
      KtiReutPhCss.Data = ReadCpuPciCfgRas (Socket, LinkIndex, KTIREUT_PH_CSS_KTI_REUT_REG);
    } while (KtiReutPhCss.Bits.s_tx_state != 0x2);

  //
  // 4929779: Cloned From SKX Si Bug Eco: KTI Link Layer not sending LL_INIT after LL Reset
  // BIOS hot-plug flow can program the clock gate disable prior to LL reset, then clear it
  // after hot-plug is done.
  //


  //
  // 5. BIOS issues link layer reset on the Host socket by setting KTILCL.LinkLayerReset to 1.
  //
  KtiLcl.Data = ReadCpuPciCfgRas (Socket, LinkIndex, KTILCL_KTI_LLPMON_REG);
  KtiLcl.Bits.link_layer_reset = 1;
  WriteCpuPciCfgRas (Socket, LinkIndex, KTILCL_KTI_LLPMON_REG, KtiLcl.Data);

  //
  // 6. Poll for hardware to clear this register back to 0.
  //
  do {
      KtiLcl.Data = ReadCpuPciCfgRas (Socket, LinkIndex, KTILCL_KTI_LLPMON_REG);
  } while (KtiLcl.Bits.link_layer_reset != 0);

  return RAS_MPLINK_SUCCESS;
} // ReceivingKtiPortsFullSpeedTransition

/**

 Set cinitbegin on Host sockets to initiate op speed training.

 @param KtiInternalGlobal - KTIRC internal variables.

 @retval RAS_MPLINK_SUCCESS - on successful completion

**/
KTI_STATUS
ReceivingKtiPortsSetCinitbegin (
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  )
{
  UINT8                           LinkIndex;
  KTI_LINK_DATA                   *pPeerInfo;
  KTIREUT_PH_CTR1_KTI_REUT_STRUCT KtiReutPhCtr1;

  for (LinkIndex = 0; LinkIndex < KTI_HOST_OUT_PTR->KtiPortCnt; LinkIndex++) {
    if (mRasSocketData->Cpu[mPendingSocId].LinkData[LinkIndex].Valid == TRUE) {
      pPeerInfo = &(mRasSocketData->Cpu[mPendingSocId].LinkData[LinkIndex]);
      KtiReutPhCtr1.Data = ReadCpuPciCfgRas ((UINT8)pPeerInfo->PeerSocId, (UINT8)pPeerInfo->PeerPort, KTIREUT_PH_CTR1_KTI_REUT_REG);
      KtiReutPhCtr1.Bits.c_init_begin = 1;
      WriteCpuPciCfgRas ((UINT8)pPeerInfo->PeerSocId, (UINT8)pPeerInfo->PeerPort, KTIREUT_PH_CTR1_KTI_REUT_REG, KtiReutPhCtr1.Data);
    }
  }

  return RAS_MPLINK_SUCCESS;
}

/**

 Re-enable Link Layer Clock Gating on Host sockets.

 @retval RAS_MPLINK_SUCCESS - on successful completion

**/
KTI_STATUS
ReceivingKtiPortsReenableLinkLayerClockGating (
  )
{


  return RAS_MPLINK_SUCCESS;
}

//
// Memory related RAS routines, need review later
//
//KTI_STATUS
//MemSpeedModeTransitionForHotAdd (
//  VOID
//  )
//{
//  UINT32      Status;
//  UINT32      mailboxData;
//  UINT32      mcBiosReq;
//  UINT8       req_data;
//  UINT8       req_type;
//  UINT8       maxFreq;
//
//  if(mRasHost->var.mem.socket[mSbspId].ddrFreqCheckFlag == 0) {   //Ckeck if it is the first time
//    Status = SendMailBoxCmdToPcodeRas (mSbspId, MAILBOX_BIOS_CMD_READ_MC_FREQ, 0x0); //get DCLK from SBSP socket
//    if (Status == 0) {  // success
//      mailboxData = ReadCpuPciCfgRas (mSbspId, 0, BIOS_MAILBOX_DATA);
//      req_data = (UINT8) mailboxData & 0x3F;
//      req_type = (UINT8) ((mailboxData & 0x40) >> 6);
//      maxFreq   = (UINT8) ((mailboxData & 0x1F80) >> 7);
//      mRasHost->var.mem.socket[mSbspId].ddrFreqCheckFlag = 1;
//      mRasHost->var.mem.socket[mSbspId].ddrClkData = req_data;
//      mRasHost->var.mem.socket[mSbspId].ddrClkType = req_type;
//      mRasHost->var.mem.socket[mSbspId].maxFreq    = maxFreq;
//    } else {
//      return Status;
//    }
//  } else {
//    req_data = mRasHost->var.mem.socket[mSbspId].ddrClkData;
//    req_type = mRasHost->var.mem.socket[mSbspId].ddrClkType;
//    maxFreq  = mRasHost->var.mem.socket[mSbspId].maxFreq;
//  }
//
//  mcBiosReq = ((req_type << 0x6) | req_data);
//  SendMailBoxCmdToPcodeRas ((UINT8)mPendingSocId, MAILBOX_BIOS_CMD_SET_MC_FREQ, mcBiosReq);  //set DCLK ratio to incoming socket
//
//  return RAS_MPLINK_SUCCESS;
//}

KTI_STATUS
ProgramLateActionForHotAdd (
  IN UINT8 Value
  )
{
  UINT8                         Idx2;
  KTI_LINK_DATA                 *pPeerInfo;

  for (Idx2 = 0; Idx2 < KTI_HOST_OUT_PTR->KtiPortCnt; Idx2++) {
    ProgramLateActionOnCpuLink ((UINT8)mPendingSocId, Idx2, Value);
    pPeerInfo = &(mRasSocketData->Cpu[mPendingSocId].LinkData[Idx2]);
    ProgramLateActionOnCpuLink ((UINT8)pPeerInfo->PeerSocId, (UINT8)pPeerInfo->PeerPort, Value );
  }
  return RAS_MPLINK_SUCCESS;
}


/**

   Programs the values from the EPARAMS entry for specified link

   @param Socket - SocId we are looking up
   @param Link - Link# on socket
   @param *LinkSpeedParameter - ptr to eparam data

   @retval RAS_MPLINK_SUCCESS - on successful completion
**/
RAS_MPLINK_STATUS
ConfigSocketLinkEparamsForHotAdd (
  IN UINT8          Socket,
  IN UINT8          Link,
  IN VOID           *LinkSpeedParameter
  )
{
  return RAS_MPLINK_SUCCESS;
}

/**

   Locates the platform EPARAMS based on socket/link

   @param Socket - SocId we are looking up
   @param Link - Link# on socket
   @param Freq - Link frequency
   *LinkSpeedParameter - return Ptr

   @retval Ptr to EPARAM table
   @retval RAS_MPLINK_SUCCESS if entry found
   @retval other value if no entry found

**/
RAS_MPLINK_STATUS
KtiGetEParamsForHotAdd (
  IN  UINT8                    SocketID,
  IN  UINT8                    Link,
  IN  UINT8                    Freq,
  OUT VOID                     *LinkSpeedParameter
  )
{
  ALL_LANES_EPARAM_LINK_INFO       *AllLanesPtr = NULL;
  PER_LANE_EPARAM_LINK_INFO        *PerLanePtr = NULL;
  PER_LANE_EPARAM_LINK_INFO        TempPerLaneData;
  UINT32                           AllLanesLinkCount = 0;
  UINT32                           PerLaneLinkCount = 0;
  BOOLEAN EparamInfoFound = FALSE;

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  OemKtiGetEParamsForHotAdd++, SocketID: 0x%X, Port: 0x%X, Freq: 0x%X \n", SocketID, Link, Freq));

  if (Freq == 0){
    Freq = (1 << Freq);
  }

  if (mKtiEparamLinkInfo.CountOfAllLanesEparamLink > 0) {
    AllLanesPtr = &mKtiEparamLinkInfo.AllLanesEparamLinkInfo[0];
    AllLanesLinkCount = mKtiEparamLinkInfo.CountOfAllLanesEparamLink;


    //
    // Find per bundle Table first
    //
    while (AllLanesLinkCount) {

      if ((AllLanesPtr->SocketID == SocketID) && (AllLanesPtr->Freq & Freq) && (AllLanesPtr->Link & (1 << Link))) {
        //
        //Convert to per lane data structure format from all lanes data structure
        //
        TempPerLaneData.SocketID = AllLanesPtr->SocketID;
        TempPerLaneData.AllLanesUseSameTxeq = ALL_LANES_TXEQ_ENABLED;
        TempPerLaneData.Freq = AllLanesPtr->Freq;
        TempPerLaneData.Link = AllLanesPtr->Link;
        TempPerLaneData.TXEQL[0] = AllLanesPtr->AllLanesTXEQ;
        TempPerLaneData.CTLEPEAK[0] = AllLanesPtr->CTLEPEAK;
        CopyMem ((UINT8*)LinkSpeedParameter, (UINT8*) &TempPerLaneData, sizeof (PER_LANE_EPARAM_LINK_INFO));
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  Full Speed EPARAM entry located for Socket %u Link %u", SocketID, Link));
        EparamInfoFound = TRUE;
        break;
      }

      AllLanesPtr++;
      AllLanesLinkCount --;
    }

    if (EparamInfoFound == TRUE){
      return RAS_MPLINK_SUCCESS;
    }
  }


  if (mKtiEparamLinkInfo.CountOfPerLaneEparamLink > 0) {
    PerLanePtr = &mKtiEparamLinkInfo.PerLaneEparamLinkInfo[0];
    PerLaneLinkCount = mKtiEparamLinkInfo.CountOfPerLaneEparamLink;

    //
    // Find per lane Table if we cannot find in per bundle table
    //
    while (PerLaneLinkCount) {
      if ((PerLanePtr->SocketID == SocketID) && (PerLanePtr->Freq & (1 << Freq)) && (PerLanePtr->Link & (1 << Link))) {
        CopyMem ((UINT8*)LinkSpeedParameter, (UINT8*)PerLanePtr, sizeof (PER_LANE_EPARAM_LINK_INFO));
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n Per Lane Full Speed EPARAM entry located for Socket %u Link %u", SocketID, Link));
        EparamInfoFound = TRUE;
        break;
      }

      PerLanePtr++;
      PerLaneLinkCount --;
    }

    if (EparamInfoFound == TRUE){
      return RAS_MPLINK_SUCCESS;
    }
  }

  return RAS_MPLINK_SUCCESS;
}

/**

   Programs Phy layer Electrical Parameters

   @param KtiInternalGlobal - KTIRC internal variables.
   @param SocketData        - Socket specific data structure
   @param Index             - socket Index
   @param LinkIndex         - qpi port number

**/
RAS_MPLINK_STATUS
ProgramEparamsForHotAdd (
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN  UINT8                    Index,
  IN  UINT8                    LinkIndex,
  IN  UINT8                    Group
  )
{
  UINT8              LinkSpeedParameter[128];
  UINT8              LinkSpeed = 0;
  RAS_MPLINK_STATUS Status;

  if (Group == KTI_GROUP) {
    LinkSpeed = (UINT8)KTI_HOST_OUT_PTR->OutPerLinkSpeed[Index][LinkIndex];
  }

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    ProgramEparamsForHotAdd++ LinkSpeed: %u", LinkSpeed));

  //
  // Get Eparamtable. the function is returning the correct EPARAM based on
  // Socket, Link, Freq, Platform
  //
  Status = KtiGetEParamsForHotAdd (Index, LinkIndex, LinkSpeed, &LinkSpeedParameter);

  if (Status != RAS_MPLINK_SUCCESS) {
    //
    // No entry found at all!!
    //
    KtiCheckPoint ((UINT8)(1 << Index), SOCKET_TYPE_CPU, (UINT8)(1 << LinkIndex), ERR_FULL_SPEED_TRANSITION, MINOR_ERR_KTI_ELEC_PARAM_NOT_FOUND);
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n***** KTI Electrical Parameters for CPU%u  Link %u is not found**** \n", Index, LinkIndex));
    KTI_ASSERT (FALSE, ERR_FULL_SPEED_TRANSITION, MINOR_ERR_KTI_ELEC_PARAM_NOT_FOUND);
  }

  ConfigSocketLinkEparamsForHotAdd (Index, LinkIndex, &LinkSpeedParameter);

  return RAS_MPLINK_SUCCESS;
}

RAS_MPLINK_STATUS
ProgramPhyLayerForHotAdd (
  VOID
  )
{
  UINT8         Idx2;
  KTI_LINK_DATA *pPeerInfo;

  //
  // Program the PHY layer settings for RAS CPU's valid links
  //
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    Programming Phy layer for Online event. "));

  for (Idx2 = 0; Idx2 < KTI_HOST_OUT_PTR->KtiPortCnt; Idx2++) {
    if (mRasSocketData->Cpu[mPendingSocId].LinkData[Idx2].Valid == TRUE){
      pPeerInfo = &(mRasSocketData->Cpu[mPendingSocId].LinkData[Idx2]);
      //
      // Program Electrical Parameters (board-layout-dependent)
      //
      ProgramEparamsForHotAdd (mKtiInternalGlobal, (UINT8)mPendingSocId, Idx2, KTI_GROUP);
      ProgramEparamsForHotAdd (mKtiInternalGlobal, (UINT8)pPeerInfo->PeerSocId, (UINT8)pPeerInfo->PeerPort, KTI_GROUP);
      //
      // Program CTR1 and LDC registers
      //
      ProgramPhyLayerControlRegisters (mKtiInternalGlobal, mRasSocketData, (UINT8)mPendingSocId, Idx2, KTI_GROUP);
      ProgramPhyLayerControlRegisters (mKtiInternalGlobal, mRasSocketData, (UINT8)pPeerInfo->PeerSocId, (UINT8)pPeerInfo->PeerPort, KTI_GROUP);
    }
  }

  //
  // Clear cinitbegin on invalid ports
  //
  ClearCinitbeginOnInvalidLinks (mRasSocketData);
  return RAS_MPLINK_SUCCESS;
} // ProgramPhyLayerForHotAdd

/**

 Program Link Layer registers for the new links

 @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
ProgramLinkLayerForHotAdd (
  VOID
  )
{
  UINT8         LinkIndex, Socket, M3Kti;
  KTI_LINK_DATA *pPeerInfo;
  UINT32        VnaCredit;
  KTI_HOST_OUT *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    Programming Link layer for Online event. "));

  if (KtiHostOutPtr->OutVn1En == TRUE) {
    VnaCredit = KTI_LL_VNA_WITH_VN1_SKX;
  } else {
    VnaCredit = KTI_LL_VNA_NORMAL_SKX;
  }

  for (LinkIndex = 0; LinkIndex < KtiHostOutPtr->KtiPortCnt; LinkIndex++) {
    if (mRasSocketData->Cpu[mPendingSocId].LinkData[LinkIndex].Valid == TRUE) {
      ProgramLinkLayerControlRegisters (mKtiInternalGlobal, (UINT8) mPendingSocId, LinkIndex, KTI_GROUP);
      pPeerInfo = &(mRasSocketData->Cpu[mPendingSocId].LinkData[LinkIndex]);
      ProgramLinkLayerControlRegisters (mKtiInternalGlobal, (UINT8) pPeerInfo->PeerSocId, (UINT8) pPeerInfo->PeerPort, KTI_GROUP);
    }
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((KtiHostOutPtr->SocketPresentBitMap & (BIT0 << Socket)) != 0) {
      for (M3Kti = 0; M3Kti < KtiHostOutPtr->CpuInfo[Socket].TotM3Kti; M3Kti++) {
        ProgramLinkLayerOnM3Kti (mKtiInternalGlobal, Socket, M3Kti, VnaCredit);
      }
    }
  }

 return RAS_MPLINK_SUCCESS;
}

/**

  Make sure none of the links of the hot added socket fail to train after the
  warm reset.

  @retval RAS_MPLINK_SUCCESS         - links successfully trained
  @retval RAS_MPLINK_FAILURE_GENERAL - one of the links failed to train

**/
KTI_STATUS
VerifyLinkStatusAfterWarmReset (
  VOID
  )
{
  UINT8                      Port;
  KTILS_KTI_LLPMON_STRUCT    LsSts;
  KTI_HOST_OUT *KtiHostOutPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  //
  // The route tables are setup only during the first warm reset of the hot added CPU. During subsequent reset
  // the route is not setup. So
  //
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n    Verifying if all links of the hot added CPU reached Normal Operation after warm reset: "));

  for (Port = 0; Port < KtiHostOutPtr->KtiPortCnt; Port++) {
    if (mRasSocketData->Cpu[mPendingSocId].LinkData[Port].Valid == TRUE) {
      LsSts.Data = ReadCpuPciCfgRas ((UINT8)mPendingSocId, Port, KTILS_KTI_LLPMON_REG);
      if (LsSts.Bits.init_state != LINK_SM_INIT_DONE) {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "Failure"));
        return RAS_MPLINK_FAILURE_GENERAL;
      }
    }
  }

 KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "Success"));
 return RAS_MPLINK_SUCCESS;
}

/**

 Update the system CpuCsrAccessVar in KTI RAS, need to review when verify in real platform

 @retval RAS_MPLINK_SUCCESS - on successful completion

**/
RAS_MPLINK_STATUS
KtiRasUpdateSysCpuCsrAccessVar (
  VOID
  )
{
  UINT8 Socket;
  KTI_HOST_OUT *KtiHostOutPtr;
  CPU_CSR_ACCESS_VAR *CsrAccessVarPtr;

  KtiHostOutPtr = KTI_HOST_OUT_PTR;
  CsrAccessVarPtr = GetSysCpuCsrAccessVar ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    CsrAccessVarPtr->stackPresentBitmap[Socket] = KtiHostOutPtr->stackPresentBitmap[Socket];

    CopyMem (
      CsrAccessVarPtr->StackBus[Socket],
      KtiHostOutPtr->StackBus[Socket],
      sizeof (CsrAccessVarPtr->StackBus[0])
      );

    CsrAccessVarPtr->SocketFirstBus[Socket] = KtiHostOutPtr->SocketFirstBus[Socket];
    CsrAccessVarPtr->SocketLastBus[Socket]  = KtiHostOutPtr->SocketLastBus[Socket];
    CsrAccessVarPtr->segmentSocket[Socket]  = KtiHostOutPtr->segmentSocket[Socket];

    CopyMem (
      CsrAccessVarPtr->MmioBar[Socket],
      KtiHostOutPtr->MmioBar[Socket],
      sizeof (CsrAccessVarPtr->MmioBar[0])
      );
    CsrAccessVarPtr->Socket10nmUboxBus0[Socket] = KtiHostOutPtr->Socket10nmUboxBus0[Socket];
  }

  return RAS_MPLINK_SUCCESS;
}
