/** @file
  This file includes functions for multi-sockets support.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2020 Intel Corporation. <BR>

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
#include <RcRegs.h>
#include <KtiSetupDefinitions.h>
#include <Upi/KtiSi.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <Library/EightSocketLib.h>
#include <KtiMisc.h>
#include <SiliconSetting.h>
#include <Chip/Include/EvAutoRecipe.h>
#include <UncoreCommonIncludes.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/IoAccessLib.h>
#include <Library/DebugLib.h>
#include <ScratchpadList.h>
#include <Library/CsrSbPortIdLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemRcLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/ChaLib.h>
#include <Guid/UboxIpInterface.h>
#include <Library/PcuIpLib.h>
#include <Guid/KtiIpInterface.h>
#include <Guid/FpgaIpInterface.h>
#include <Library/KtiApi.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Guid/MrcPlatformHooksGuid.h>
#include <Library/EnhancedWarningLogLib.h>
#include "MultiSocketInternal.h"

//
// No interface defined for KtiRasSupport functions, investigate.
//

extern
VOID
CsrWriteCheckPoint (
  VOID
  );

/**
  Private routine to update the KTI topology scratchpad based on the current topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

**/
KTI_STATUS
UpdateKtiTopologyInScratchpad (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8  Index, Port;
  UINT64 KtiTopSaved, SrTop64, OriginalData;

  if ((KtiInternalGlobal->CurrentReset == POST_RESET_POWERGOOD) || (KtiInternalGlobal->ForceColdResetFlow == TRUE)) {
    //
    // Update the Ubox sticky scratch register 02 with the LEP info. This info is used to verify
    // if the system KTI topology changed across boots and take remedial action. Note that in case
    // of reduced topology scenario, scratch registers are updated with the reduced topology, not
    // the current topology. The current reset flow has been running under the assumption of the
    // reduced topology. (For example Credits are programmed for reduced topology scenario)
    // [07:00]   - CPU list; this list reflects the CPUs after the topology is degraded, if needed
    // Start from BIT8, each KtiPortCnt bits will be used for one CPU
    // Each bit for a CPU indicates whether the Link is valid or not.
    // If KtiPortCnt = 3:
    // [10:08]   - CPU0 Link 0, 1, 2 Valid
    // [13:11]   - CPU1 Link 0, 1, 2 Valid
    // [16:14]   - CPU2 Link 0, 1, 2 Valid
    // [19:17]   - CPU3 Link 0, 1, 2 Valid
    // [22:20]   - CPU4 Link 0, 1, 2 Valid
    // [25:23]   - CPU5 Link 0, 1, 2 Valid
    // [28:26]   - CPU6 Link 0, 1, 2 Valid
    // [31:29]   - CPU7 Link 0, 1, 2 Valid
    // If KtiPortCnt = 6:
    // [13:08]   - CPU0 Link 0, 1, 2, 3, 4, 5 Valid
    // [19:14]   - CPU1 Link 0, 1, 2, 3, 4, 5 Valid
    // [25:20]   - CPU2 Link 0, 1, 2, 3, 4, 5 Valid
    // [31:26]   - CPU3 Link 0, 1, 2, 3, 4, 5 Valid
    // [37:32]   - CPU4 Link 0, 1, 2, 3, 4, 5 Valid
    // [43:38]   - CPU5 Link 0, 1, 2, 3, 4, 5 Valid
    // [49:44]   - CPU6 Link 0, 1, 2, 3, 4, 5 Valid
    // [55:50]   - CPU7 Link 0, 1, 2, 3, 4, 5 Valid
    //
    SrTop64 = 0;
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if (SocketData->Cpu[Index].Valid == TRUE) {
        for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
          if (SocketData->Cpu[Index].LinkData[Port].Valid == TRUE) {
            SrTop64 = SrTop64 | LShiftU64 (1, (Index * (KtiInternalGlobal->KtiPortCnt) + Port));
          }
        }
      }
    }
    SrTop64 = LShiftU64 (SrTop64, 8) | (KtiInternalGlobal->CpuList);

    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n    Setting Ubox Sticky to save KTI topology to 0x%016X \n", SrTop64));

    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
        //
        //Save the Scrachpad register bits that are not used for topology bitmap
        //
        UBoxGetKtiTopologyInScratchpad (Index, 0, &OriginalData);
        OriginalData &= (UINT64) (~TOPOLOGY_BITMAP_64BITS_AND_MASK);
        //
        //Program the Scrachpad register with new topology bitmap
        //
        UBoxSetKtiTopologyInScratchpad (Index, 0, (SrTop64 | OriginalData));
        UBoxGetKtiTopologyInScratchpad (Index, 0, &KtiTopSaved);
      }
    }
  }
  return KTI_SUCCESS;
}

/**
  Prime the KTIRC data structure based on fpga input option and system topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
PrimeHostStructureFpga (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8      Socket, Port, PeerSocId, PeerPort, Stack;
  UINT8      FpgaLinkSpeed;
  BOOLEAN    FpgaPresent;
  BOOLEAN    FpgaOpSpeedDiscovered;

  FpgaPresent = FALSE;
  FpgaOpSpeedDiscovered = FALSE;

  //
  // Update structures for Fpga
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Port = 0; Port < (KtiInternalGlobal->KtiPortCnt); Port++) {
      if ((SocketData->Cpu[Socket].LinkData[Port].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_FPGA)) {

        PeerSocId = (UINT8) SocketData->Cpu[Socket].LinkData[Port].PeerSocId;
        PeerPort = (UINT8) SocketData->Cpu[Socket].LinkData[Port].PeerPort;
        if ((PeerPort >= (KtiInternalGlobal->KtiPortCnt)) || (PeerSocId >= MAX_SOCKET)) {
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n PeerPort:%u >= KtiPortCnt:%u or PeerSocId:%u >= MAX_SOCKET:%u",
            PeerPort, (KtiInternalGlobal->KtiPortCnt), PeerSocId, MAX_SOCKET));
          KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
          return KTI_FAILURE;
        }

        KTIVAR->OutKtiFpgaEnable[PeerSocId] = 1;

        FpgaLinkSpeed = FpgaIpGetOpSpeed (Socket, Port);
        KTI_ASSERT (((FpgaLinkSpeed == SPEED_REC_112GT) || (FpgaLinkSpeed == SPEED_REC_104GT) || (FpgaLinkSpeed == SPEED_REC_96GT)),
                    ERR_FULL_SPEED_TRANSITION, MINOR_ERR_LINK_SPEED_UNSUPPORTED); // FPGA not reporting valid speed
        if (FpgaOpSpeedDiscovered) {
          //
          //For now, require all FPGAs in the system to operate at the same UPI link frequency
          //Assert that the newly discovered FPGA OpSpeed matches the previously discovered FPGA OpSpeed
          //
          KTI_ASSERT (FpgaLinkSpeed == KTIVAR->OutKtiLinkSpeed, ERR_FULL_SPEED_TRANSITION, MINOR_ERR_LINK_SPEED_UNSUPPORTED);
        }
        KTIVAR->OutKtiLinkSpeed = FpgaLinkSpeed;
        FpgaOpSpeedDiscovered = TRUE;

        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  Fpga detected, disable L1, L0p, Failover on FPGA link"));
        KTISETUP->DfxCpuCfg[Socket].Link[Port].DfxL1Enable = KTI_DISABLE;
        KTISETUP->DfxCpuCfg[Socket].Link[Port].DfxL0pEnable = KTI_DISABLE;
        KTISETUP->DfxCpuCfg[Socket].Phy[Port].DfxKtiFailoverEn = KTI_DISABLE;
        KTISETUP->DfxCpuCfg[PeerSocId].Link[PeerPort].DfxL1Enable = KTI_DISABLE;
        KTISETUP->DfxCpuCfg[PeerSocId].Link[PeerPort].DfxL0pEnable = KTI_DISABLE;
        KTISETUP->DfxCpuCfg[PeerSocId].Phy[PeerPort].DfxKtiFailoverEn = KTI_DISABLE;

        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  Fpga detected, disable CRC on FPGA link"));
        KTISETUP->DfxCpuCfg[Socket].Link[Port].DfxCrcMode = CRC_MODE_16BIT;
        KTISETUP->DfxCpuCfg[PeerSocId].Link[PeerPort].DfxCrcMode = CRC_MODE_16BIT;

        KtiInternalGlobal->ProgramPhyLink = TRUE;

        //
        // Fpga only decodes a single bus over Kti
        //
        for (Stack = 0; Stack < MAX_LOGIC_IIO_STACK; Stack ++) {
          KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].Personality = TYPE_NONE;
        }
        //
        //Assign 1 UBOX-TYPE stack for a WHR UPI port, and also 1 UBOX-TYPE stack for FPGA
        //
        KTIVAR->CpuInfo[Socket].CpuRes.StackRes[IIO_CSTACK].Personality  = TYPE_UBOX; //Bus for WHR Port0
        KTIVAR->CpuInfo[Socket].CpuRes.StackRes[IIO_PSTACK0].Personality = TYPE_UBOX; //Bus for FPGA

        SetStackPresent (Socket, IIO_CSTACK);
        SetStackPresent (Socket, IIO_PSTACK0);
        KTIVAR->CpuInfo[Socket].CpuRes.TotEnabledStacks = 0;

        //
        //Indicate whether FPGA is a home agent or not
        //
        if (FpgaIpIsNodeHomeAgent (Socket)) {
          KTIVAR->OutFpgaHomeAgent[Socket] = 1;
        } else {
          KTIVAR->OutFpgaCacheAgent[Socket] = 1;
        }

        FpgaPresent = TRUE;
      }
    }

    if (FpgaPresent) {
      //
      // Directory must be enabled for FPGA snoopfilter
      //
      KTIVAR->OutSysDirectoryModeEn = KTI_ENABLE;
      KtiInternalGlobal->SnoopFilter = KTI_ENABLE;
      KTIVAR->OutHitMeEn = KTI_DISABLE;
      KtiInternalGlobal->SnoopFanoutEn = KTI_DISABLE;
      KtiInternalGlobal->SysOsbEn = KTI_DISABLE;  // Disable until told otherwise by design
      KTIVAR->OutIoDcMode = KTI_DISABLE;          // Disable IODC in all configs with FPGA present
      KtiInternalGlobal->D2kEn = KTI_DISABLE;
      KTIVAR->OutD2kEn = KTI_DISABLE;

      if (KTIVAR->SysConfig > SYS_CONFIG_1S) {
        KtiInternalGlobal->IsRouteThrough = TRUE; //FPGA TODO - revisit for Y-topology
      }
      //
      // Force ChaID interleaving
      //
      if (KTIVAR->SysConfig == SYS_CONFIG_2S2L) {
        if (KTIVAR->OutSncEn != KTI_DISABLE) {
          KtiInternalGlobal->DualLinksInterleavingMode = DFX_DUAL_LINK_INTLV_MODE_DISABLE_D2C;
          KtiInternalGlobal->D2cEn = 0;
        } else {
          KtiInternalGlobal->DualLinksInterleavingMode = DFX_DUAL_LINK_INTLV_MODE_CHA_INTLV;
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  TSC Sync for all sockets
  1. For TSC alignment, BIOS forces assertion of TSC_SYNC ( MAILBOX_BIOS_CMD_TSC_SYNC ) to SBSP.
  2. This triggers TSC capture (need to delay for 20us after completing this on all sockets).
  3. BIOS deasserts TSC_SYNC via same processor command as for assertion (except with deassert state).
  4. BIOS calculates delta between Largest TSC and all sockets ( or BSP and new sockets' TSCs for on-line). Delta =(BSP TSC - AP TSC).
  5. BIOS writes TSC_HOT_PLUG_OFFSET register in sockets or new skt (one time update only).

  @param SocketData        - add argument and description to function comment
  @UINT8 PendingSocId      - The pending sock is on-lined
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval Nothing

**/
VOID
Tsc_Sync (
  IN KTI_SOCKET_DATA          *SocketData,
  IN UINT8                    PendingSocId,
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
  )
{
  UINT8          Socket, SbspSocket, NoDelta;
  UINT64_STRUCT  TscDelta = { 0, 0 };
  UINT64_STRUCT  TscSnapShot[MAX_SOCKET];
  UINT64_STRUCT  OrgTscSnapShot[MAX_SOCKET];
  UINT64_STRUCT  LargestSnapshot = { 0, 0 };

  if (CheckEmulationType (SIMICS_FLAG)) {
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "Skip the Tsc_Sync in Simics!!!\n"));
    return;
  }
  //
  // 1S doesn't need the TSC sync
  //
  if (KTIVAR->SysConfig == SYS_CONFIG_1S) {
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "Skip the Tsc_Sync in 1S!!!\n"));
    return;
  }


  if ( (KtiInternalGlobal->TSCSyncEn != KTI_ENABLE) && (KTIVAR->RasInProgress != TRUE)) {
    return;
  }

  SbspSocket = KtiInternalGlobal->SbspSoc;
  NoDelta = 0xFF;
  ZeroMem ((VOID *) TscSnapShot, sizeof (TscSnapShot));
  ZeroMem ((VOID *) OrgTscSnapShot, sizeof (OrgTscSnapShot));

  //
  // Read all TSC Snapshot before sending the TSC_SYNC assert command
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_CPU)) {
      PcuGetUncTsc (Socket, &OrgTscSnapShot[Socket].lo, &OrgTscSnapShot[Socket].hi);
    }
  }

  //
  // Step1: Send the TSC_SYNC assert to Pcode in SBSP
  //
  PcuSynchronizeTsc (SbspSocket, 1);

  //
  // Step2: Need to delay for ~20uS after completing this on all sockets.
  //
  KtiFixedDelay (20);

  //
  // Step3: Send TSC_SYNC de-assert to Pcode in SBSP
  //
  PcuSynchronizeTsc (SbspSocket, 0);

  if (!KTIVAR->RasInProgress) {
    //
    // Do the TSC_SYNC in boot time
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_CPU)) {
        PcuGetUncTsc (Socket, &TscSnapShot[Socket].lo, &TscSnapShot[Socket].hi);
      }
    }

    //
    // Check if all sockets could capture the new snapshot or not.
    //
    if ((TscSnapShot[SbspSocket].lo != OrgTscSnapShot[SbspSocket].lo) || (TscSnapShot[SbspSocket].hi != OrgTscSnapShot[SbspSocket].hi)){
      for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
        if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_CPU)) {
          if ((TscSnapShot[Socket].lo == OrgTscSnapShot[Socket].lo) && (TscSnapShot[Socket].hi == OrgTscSnapShot[Socket].hi)){
            //
            // Socket cannot capture new TSC snapshot, show the alert message that TSC SYNC is not supported in this system
            //
            KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n\nWARNING: This system may not support the TSC_SYNC for all sockets. Please disable the TSC_SYNC!!!\n\n"));
            KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_TSC_SYNC_UNSUPPORTED);
            return;
          }
        }
      }
    } else {
      //
      // SBSP doesn't read the new snapshot, de-Assert all TSC_SYNC to see the sbsp could capture the new snapshot or not
      //
      for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
        if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_CPU)) {
          PcuSynchronizeTsc (Socket, 0);
          KtiFixedDelay (20);
        }
      }

      //
      // Re-send the TSC_SYNC command to capture the TSC snapshot again
      //
      PcuSynchronizeTsc (SbspSocket, 1);
      KtiFixedDelay (20);
      PcuSynchronizeTsc (SbspSocket, 0);

      //
      // De-Assert the TSC_SYNC could get the new snapshot, read all snapshot again.
      //
      for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
        if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_CPU)) {
          PcuGetUncTsc (Socket, &TscSnapShot[Socket].lo, &TscSnapShot[Socket].hi);
        }
      }

      if ((TscSnapShot[SbspSocket].lo != OrgTscSnapShot[SbspSocket].lo) || (TscSnapShot[SbspSocket].hi != OrgTscSnapShot[SbspSocket].hi)){
        for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
          if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_CPU)) {
            if ((TscSnapShot[Socket].lo == OrgTscSnapShot[Socket].lo) && (TscSnapShot[Socket].hi == OrgTscSnapShot[Socket].hi)){
              //
              // Socket cannot capture new TSC snapshot, show the alert message that TSC SYNC is not supported in this system
              //
              KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n\nWARNING: This system may not support the TSC_SYNC for all sockets. Please disable the TSC_SYNC!!!\n\n"));
              KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_TSC_SYNC_UNSUPPORTED);
              return;
            }
          }
        }
      } else {
        //
        // Even de-assert all sockets the sbsp still cannot capture the TSC snapshot, return without doing the TSC sync
        //
        KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n\nWARNING: TSC SYNC command cannot capture new snapshot. Abort the TSC SYNC\n\n"));
        KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_TSC_SYNC_UNSUPPORTED);
        return;
      }
    }
    //
    // Find the largest TSC snapshot
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_CPU)) {
        if ( IsXGreaterThanYUINT64(TscSnapShot[Socket], LargestSnapshot)){
          LargestSnapshot.hi = TscSnapShot[Socket].hi;
          LargestSnapshot.lo = TscSnapShot[Socket].lo;
          NoDelta = 0;
        } else if ( !IsXEqualYUINT64 (TscSnapShot[Socket], LargestSnapshot)){
          NoDelta = 0;
        }
      }
    }

    if (NoDelta == 0xFF) {
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n All TSC Snapshot are the same, no need to do the sync!!!\n"));
      return;
    }


    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_CPU)) {
        if (IsXEqualYUINT64 (TscSnapShot[Socket], LargestSnapshot)) {
          continue;  //Skip the largest snapshot socket
        }

        //
        // Step4: Calculates delta between largest TSC and socket TSC
        //
        TscDelta = SubUINT64 (LargestSnapshot, TscSnapShot[Socket]);

        //
        // Step 5: writes TSC_HOT_PLUG_OFFSET register in remote skt (1-time update only)
        //
        PcuSetTscOffset (Socket, TscDelta.lo, TscDelta.hi);
      }
    }
  } else {
    //
    //Enforce range allowed by size of TscSnapshot array
    //
    if (PendingSocId >= MAX_SOCKET) {
      KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n\nWARNING: Cannot sync TSC for onlining socket %d. Max Socket = %d!\n\n", PendingSocId, MAX_SOCKET));
      KtiLogWarning (PendingSocId, SOCKET_TYPE_CPU, 0xFF, WARN_TSC_SYNC_UNSUPPORTED);
      return;
    }
    //
    // Do the TSC SYNC in on-lined.
    // Read the snapshot of sbsp and pendingSoc
    //
    PcuGetUncTsc (SbspSocket,   &TscSnapShot[SbspSocket].lo,   &TscSnapShot[SbspSocket].hi);
    PcuGetUncTsc (PendingSocId, &TscSnapShot[PendingSocId].lo, &TscSnapShot[PendingSocId].hi);

    //
    // Step4: Calculates delta between BSP and socket TSC
    //
    TscDelta = SubUINT64 (TscSnapShot[SbspSocket], TscSnapShot[PendingSocId]);

    //
    // Step 5: writes TSC_HOT_PLUG_OFFSET register in new skt (1-time update only)
    //
    PcuSetTscOffset (PendingSocId, TscDelta.lo, TscDelta.hi);
  }


  return;
}

/**
  Parse the Link Exchange Parameter (LEP) for a given Fpga.
  Currently we don't examine the LEP register, we assume it and
  update data strcuture for consistency

  @param SocketData        - add argument and description to function comment
  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocId             - Fpga Socket Number, 0 based

  @retval 0 - found
  @retval 1 - not found

**/
KTI_STATUS
ParseFpgaLep (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  )
{
  UINT8                      PeerSocId;
  UINT8                      PeerPort;
  UINT8                      Port;
  BOOLEAN                    FoundPeer;

  Port = 0;//For now, assume FPGA will only have Port0
  FoundPeer = FALSE;

  //
  // Fake the LEP of the Fpga
  // Peer-socket LinkData MUST be present in SocketData structure
  // For now, assume ONLY one CPU/Port connects to this FPGA node, and it connects to Port 0 on FPGA side
  //
  for (PeerSocId = 0; PeerSocId < MAX_SOCKET; PeerSocId++) {
    for (PeerPort = 0; PeerPort < KtiInternalGlobal->KtiPortCnt; PeerPort++) {
      if (SocketData->Cpu[PeerSocId].LinkData[PeerPort].PeerSocId == SocId) {
        FoundPeer = TRUE;
        break;
      }
    }
    if (FoundPeer) {
      break;
    }
  }
  // Invalidate this link if either the current port or its peer port were set to "Disabled" via Setup
  if ((PeerSocId >= MAX_SOCKET) ||
      (KTISETUP->PhyLinkPerPortSetting[SocId].Link[Port].KtiPortDisable == TRUE) ||
      (KTISETUP->PhyLinkPerPortSetting[PeerSocId].Link[XEON_FPGA_PORT].KtiPortDisable == TRUE)) {
    SocketData->Cpu[SocId].LinkData[Port].Valid = FALSE;
  } else {
    //
    // Mark the Link as valid for the socket
    //
    SocketData->Cpu[SocId].LinkData[Port].Valid = TRUE;
    SocketData->Cpu[SocId].LinkData[Port].PeerPort = PeerPort;
    SocketData->Cpu[SocId].LinkData[Port].PeerSocId = PeerSocId;
    SocketData->Cpu[SocId].LinkData[Port].PeerSocType = SOCKET_TYPE_CPU;

    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n   PGA%u Link Exchange\n", SocId));
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "   LEP%u(%u,CPU%u)", Port, PeerPort, PeerSocId));
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n"));

  return KTI_SUCCESS;
}

/**
  Returns immediate Parent of Child socket in Topology Tree.

  @param TopologyTree - Topology Tree
  @param Child        - Child Socket
  @param Parent       - Parent Socket

  @retval 0 - Successful, parent found
  @retval 1 - Failure

**/
KTI_STATUS
GetParentSocket (
  TOPOLOGY_TREE_NODE      *TopologyTree,
  TOPOLOGY_TREE_NODE      *Child,
  TOPOLOGY_TREE_NODE      *Parent
  )
{

  KTI_ASSERT ((TopologyTree != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_2C);
  KTI_ASSERT ((Parent != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_2D);
  KTI_ASSERT ((Child != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_2E);

  Parent->RawNode = TopologyTree[Child->Node.ParentIndex].RawNode;
  if (Parent->Node.Valid != TRUE) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\n Parent Node not found."));
    KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_2F);
  }

  return KTI_SUCCESS;

}

/**
  Check for matching number of CHAs, MCP Links, M3KTIs and matching SKUs
  between SBSP and Socket

  @param KtiInternalGlobal - pointer to the KTI RC internal global structure
  @param SbspSktId         - SBSP's socket ID
  @param SocketId          - Socket ID to compare SBSP to

  @retval 0     - check pass
  @retval other - check failed

**/
BOOLEAN
CheckThisSocketInfoWithSbsp (
 KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
 UINT8                      SbspSktId,
 UINT8                      SocketId
 )
{
  BOOLEAN  FeatureMismatch = FALSE;
  BOOLEAN  McpPresent = FALSE;

  //
  // Check for MCP
  //
  if ((KTIVAR->CpuInfo[SocketId].CpuType == CPU_TYPE_F) || (KTIVAR->CpuInfo[SbspSktId].CpuType == CPU_TYPE_F) ||
      (KTIVAR->CpuInfo[SocketId].CpuType == CPU_TYPE_P) || (KTIVAR->CpuInfo[SbspSktId].CpuType == CPU_TYPE_P)) {
    McpPresent = TRUE;
  }

  if ((KtiInternalGlobal->TotCha[SocketId] != KtiInternalGlobal->TotCha[SbspSktId])) {
    KtiCheckPoint ((UINT8) (1 << SocketId), 0, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_CBO_COUNT_MISMATCH);
    FeatureMismatch = TRUE;
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Cbo Count/List mismatch between SBSP (CPU%u) and CPU%u", SbspSktId, SocketId));
  }

  //
  // Ignore m3kti mismatch if MCP present.  SKX allows MCP Skus to be mixed w/ non-MCP
  //
  if (McpPresent == FALSE) {
    if (KTIVAR->CpuInfo[SocketId].TotM3Kti != KTIVAR->CpuInfo[SbspSktId].TotM3Kti) {
      KtiCheckPoint ((UINT8) (1 << SocketId), 0, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_M3KTI_COUNT_MISMATCH);
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n M3KTI Count mismatch between SBSP (CPU%u) and CPU%u", SbspSktId, SocketId));
      if (KTISETUP->DfxParm.DfxM3KtiCountMismatchEn == KTI_ENABLE) {
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n User has changed Setup to skip the M3KTI count mismatch check"));
      } else {
        FeatureMismatch = TRUE;
      }
    }
  }

  //
  // Verify socket ways value match
  //
  if (KtiInternalGlobal->SocketWays[SocketId] != KtiInternalGlobal->SocketWays[SbspSktId]) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Processor Socket Ways mismatch between SBSP (CPU%u) and CPU%u:  Socket Ways %u vs %u",
                       SbspSktId, SocketId, KtiInternalGlobal->SocketWays[SbspSktId], KtiInternalGlobal->SocketWays[SocketId]));
    if (KTISETUP->KtiSkuMismatchCheck) {
      FeatureMismatch = TRUE;
    } else {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n WARNING: Ignoring previous Socket Ways mismatch"));
    }
  }

  return FeatureMismatch;
 }

/**
  Returns TRUE if the CPU is connected to Legacy CPU.

  @param SocketData        - socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocId             - CPU Socket Number, 0 based

  @retval TRUE  - CPU is connected to Legacy CPU
  @retval FALSE - CPU is not connected to Legacy CPU

**/
BOOLEAN
CheckCpuConnectedToLegCpu (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  )
{
  UINT8 Index;

  //
  // Check the peer of each link to see if the given CPU is directly connected to Legacy CPU
  //
  for (Index = 0; Index < KtiInternalGlobal->KtiPortCnt; Index++) {
    if (SocketData->Cpu[SocId].LinkData[Index].Valid == TRUE &&
        SocketData->Cpu[SocId].LinkData[Index].PeerSocType == SOCKET_TYPE_CPU &&
        SocketData->Cpu[SocId].LinkData[Index].PeerSocId == KtiInternalGlobal->SbspSoc) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Returns port number of the Parent socket that connects it to Child socket.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuData           - Contains the LEP of Parent & Child nodes
  @param Parent            - Socket at one end of the link
  @param Child             - Socket at the other end of the link

  @retval Port of the Parent socket that connects it to the child socket
  @retval 0xFF - Child is not an immediate neighbor of Parent

**/
UINT8
GetChildLocalPortFromLEP (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData,
  TOPOLOGY_TREE_NODE         *Parent,
  TOPOLOGY_TREE_NODE         *Child
  )
{
  UINT8         KtiLink;
  KTI_LINK_DATA TempLep;

  KTI_ASSERT ((CpuData != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_26);
  KTI_ASSERT ((Parent != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_27);
  KTI_ASSERT ((Child != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_28);

  if (Parent->Node.SocType == SOCKET_TYPE_CPU) {
    for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {
      TempLep = CpuData[Parent->Node.SocId].LinkData[KtiLink];
      if (TempLep.Valid != TRUE || TempLep.DualLink == TRUE || TempLep.TwoSkt3Link == TRUE) { // Skip the Invalid/Duplicate Links
        continue;
      }

      if (TempLep.PeerSocId == Child->Node.SocId && TempLep.PeerSocType == Child->Node.SocType) {
        return KtiLink;
      }
    }
  }

  return KTI_INVALID_PORT;
}

/**
  Searches the Topology tree to see if the node under search is already found in the given SBSP min path tree.

  @param Tree       - Topology Tree
  @param SearchNode -  Node to search in the topology tree

  @retval TRUE   - Node already exists in tree
  @retval FALSE  - Node doesn't exist in tree

**/
BOOLEAN
NodeFoundInMinPathTree (
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *SearchNode
  )
{
  UINT8 Index;

  KTI_ASSERT ((Tree != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_37);
  KTI_ASSERT ((SearchNode != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_38);

  for (Index = 0; Index < MAX_TREE_NODES; Index++) {
    //
    // If there are no more valid nodes, then the node in question is not in the tree
    //
    if (Tree[Index].Node.Valid == FALSE) {
      return FALSE;
    }
    //
    // If socket ID & Type matches we found the node
    //
    if (Tree[Index].Node.SocId == SearchNode->Node.SocId && Tree[Index].Node.SocType == SearchNode->Node.SocType) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Adds the current node to the tree

  @param Tree  - system KTI fabric node tree
  @param Node  - current node tree

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
AddNodeToTopologyTree (
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *Node
  )
{
  UINT8 Index;

  KTI_ASSERT ((Tree != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_39);
  KTI_ASSERT ((Node != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_3A);

  for (Index = 0; Index < MAX_TREE_NODES; Index++) {
    //
    // Place the tree in first available empty slot in the array
    //
    if (Tree[Index].Node.Valid == FALSE) {
      Tree[Index] = *Node;
      return KTI_SUCCESS;
    }
  }

  return (KTI_STATUS) KTI_FAILURE;
}

/**
  Routine to set minimum required route to allow config accesses coming from one of SBSP's core
  to be routed to the agent that claims the region. It also constructs the SBSP minimum tree
  and collects information such as CPU sockets populated and their LEP info to facilitate
  the topology discovery process to be done without any config access. The route set by this
  routine may not be the optimum path. Note that, the route set by this routine allows only the
  SBSP be able to access the config space of other CPU sockets in the system; not from
  one PBSP to other PBSPs.

  It also sets up the route to the Legacy CPU so that >1 hop away CPUs can be released from reset
  and brought under BIOS control.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
SetupSbspPathToAllSockets (
  OUT KTI_SOCKET_DATA          *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal
  )
{
  KTI_STATUS                         Status;
  TOPOLOGY_TREE_NODE                 GrandParent;
  TOPOLOGY_TREE_NODE                 Parent;
  TOPOLOGY_TREE_NODE                 Child;
  UINT8                              Index;
  UINT8                              Index1;
  UINT32                             Ctr;
  UINT8                              KtiLink;
  UINT8                              Port;
  UINT32                             WaitTime;
  BOOLEAN                            CheckInFlag;
  BOOLEAN                            FeatureMismatch;
  UINT8                              sbspSktId;
  UINT8                              LegacyPchTargetForAp;
  BOOLEAN                            bCheckInBitsSet;
  UINT32                             LlcBist;

  sbspSktId = KtiInternalGlobal->SbspSoc;
  KtiCheckPoint (sbspSktId, SOCKET_TYPE_CPU, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_ADD_SOCKET_TO_MIN_PATH_TREE);

  //
  //  Add the Sbsp to minimum path tree
  //
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n Constructing SBSP minimum path Topology Tree"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n --------------------------------------------"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n Adding SBSP (CPU%u) to the tree", sbspSktId));

  Parent.RawNode = 0;
  Parent.Node.Valid = TRUE;
  Parent.Node.SocId = sbspSktId;
  Parent.Node.SocType = SOCKET_TYPE_CPU;
  Parent.Node.ParentPort = 0xF;
  Parent.Node.Hop = 0;
  Parent.Node.ParentIndex = 0x7F;

  if ((Status = AddNodeToTopologyTree (SocketData->SbspMinPathTree, &Parent)) != KTI_SUCCESS) {
    KtiCheckPoint ((UINT8)(1 << Parent.Node.SocId), SOCKET_TYPE_CPU, 0, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INTERNAL_DATA_STRUCTURE);
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Adding CPU%u to Minimum Path Tree failed - Data Structure Overflow.", Parent.Node.SocId));
    KTI_ASSERT (FALSE, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INTERNAL_DATA_STRUCTURE);
  }
  //
  // Add the Sbsp to the socketdata
  //
  SocketData->Cpu[sbspSktId].Valid = TRUE;
  SocketData->Cpu[sbspSktId].SocId = sbspSktId;
  SocketData->Cpu[sbspSktId].SocType = SOCKET_TYPE_CPU;

  //
  // Discover the neighbors of Sbsp and setup path to neighbor's M2PCIe/Ubox. Repeat it until no
  // new sockets are found. Also, setup the boot path if a CPU is >1 hop away from Legacy CPU.
  //
  for (Index = 0; SocketData->SbspMinPathTree[Index].Node.Valid == TRUE; Index++) {

    Parent = SocketData->SbspMinPathTree[Index];
    if (Parent.Node.SocType == SOCKET_TYPE_CPU) {

      if (Parent.Node.SocId != sbspSktId) {
        //
        // Make sure the pCode has completed initializing the socket before making any CSR access in that socket.
        // Limit delay to 5s.
        //
        CheckInFlag = FALSE;
        WaitTime = 5000;
        for (Ctr = 0; Ctr < WaitTime; Ctr++) {
          UBoxGetLlcBistResultsInScratchpad ((UINT8)Parent.Node.SocId, 0, &LlcBist);
          if (LlcBist == LLC_BIST_COMPLETE) { // Bits [15:8] should be 0x2 (LLC BIST complete) before the socket can be accessed.
            CheckInFlag = TRUE;
            break;
          }

          KtiFixedDelay (1000); // 1000 microsecs
        }

        if (CheckInFlag == FALSE) {
          // Halt the System
          KtiCheckPoint ((UINT8)(1 << Parent.Node.SocId), (UINT8)Parent.Node.SocType, 0, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE);
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\npCode has not completed initilization of AP Socket %u. System Halted!!!", Parent.Node.SocId));
          KTI_ASSERT (FALSE, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE);
        }

        //
        // Make sure each discovered socket has Cha Count/List, M3KTI. Socket Sku Type that matches that of SBSP's.
        //
        UpdateCpuInfoInKtiInternal (KtiInternalGlobal, (UINT8)Parent.Node.SocId);
        GetLegacyPchTargetSktId ((UINT8)Parent.Node.SocId, &LegacyPchTargetForAp);
        FeatureMismatch = CheckThisSocketInfoWithSbsp (KtiInternalGlobal, sbspSktId, (UINT8)Parent.Node.SocId);

        if (sbspSktId != LegacyPchTargetForAp) {
          KtiCheckPoint ((UINT8)(1 << Parent.Node.SocId), (UINT8)Parent.Node.SocType, 0, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_LEGACY_PCH_MISMATCH);
          FeatureMismatch = TRUE;
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Legacy PCH target NID mismatch between SBSP (CPU%u) and CPU%u", sbspSktId, Parent.Node.SocId));
        }

        if (FeatureMismatch == TRUE) {
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n System Configuration not supported!!! "));
          KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }
      }
      //
      // Parse the LEP of the discovered socket.
      //
      KtiCheckPoint ((UINT8)(1 << Parent.Node.SocId), (UINT8)Parent.Node.SocType, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_COLLECT_LEP);
      ParseCpuLep (SocketData, KtiInternalGlobal, (UINT8)Parent.Node.SocId);
    } else if (Parent.Node.SocType == SOCKET_TYPE_FPGA) {
      //
      // Parse the LEP of the discovered FPGA.
      //
      KtiCheckPoint ((UINT8)(1 << Parent.Node.SocId), (UINT8)Parent.Node.SocType, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_COLLECT_LEP);
      if (IsSkxFamilyCpu (KtiInternalGlobal->CpuType) || KtiInternalGlobal->CpuType == CPU_ICXSP) {
        ParseFpgaLep (SocketData, KtiInternalGlobal, (UINT8)Parent.Node.SocId);
      }
    } else {
      KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_6D);
    }

    //
    // Make sure the parent has checked-in. NonSticky Scratch Register 02 in SBSP, is used by each AP sockets to indicate
    // that they have successfully checked-in. If it hasn't checked in yet, handle error.
    //
    if ((Parent.Node.SocId != sbspSktId) && (Parent.Node.SocType == SOCKET_TYPE_CPU)) { // SBSP has checked in, otherwise we won't be here
      KtiCheckPoint ((UINT8)(1 << Parent.Node.SocId), (UINT8)Parent.Node.SocType, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_CHK_PBSP_CHKIN);

      //
      // The AP that is directly connected to Legacy CPU must have checked-in.
      //
      if (CheckCpuConnectedToLegCpu (SocketData, KtiInternalGlobal, (UINT8)Parent.Node.SocId) &&
          !SocketRequireBootGo (SocketData, KtiInternalGlobal, (UINT8)Parent.Node.SocId)) {
        CheckInFlag = FALSE;
        WaitTime = KTISETUP->WaitTimeForPSBP;
        for (Ctr = 0; Ctr < WaitTime; Ctr++) {
          UBoxCheckInBitsIsSetInScratchpad ((UINT8)Parent.Node.SocId, 0, &bCheckInBitsSet);
          if (bCheckInBitsSet) { // Read CheckInBit to determine if AP has checked-in
            CheckInFlag = TRUE;
            break;
          }
          KtiFixedDelay (1000); // 1000 microsecs
        }
        if (!CheckInFlag) {
          KtiCheckPoint ((UINT8)(1 << Parent.Node.SocId), (UINT8)Parent.Node.SocType, 0, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE);
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\n AP Socket %u is discovered but hasn't come out of reset. System Halted!!!", Parent.Node.SocId));
          KTI_ASSERT (FALSE, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE);
        }
      } else {
        //
        // Setup the boot path for the parent which is not directly connected to Legacy CPU.
        //
        KtiCheckPoint ((UINT8)(1 << Parent.Node.SocId), (UINT8)Parent.Node.SocType, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_SET_PBSP_BOOT_PATH);
        GetParentSocket (SocketData->SbspMinPathTree, &Parent, &GrandParent);

        Port = GetChildLocalPortFromLEP (KtiInternalGlobal, SocketData->Cpu, &GrandParent, &Parent);
        if (Port == 0xFF) {
          KtiCheckPoint ((UINT8)(1 << Parent.Node.SocId), (UINT8)Parent.Node.SocType, 0, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INVALID_PORT_CONNECTION);
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid Port connection between CPU%u and CPU%u.", GrandParent.Node.SocId, Parent.Node.SocId));
          KTI_ASSERT (FALSE, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INVALID_PORT_CONNECTION);
        }

        if ((Parent.Node.SocType == SOCKET_TYPE_CPU) && (Port < (KtiInternalGlobal->KtiPortCnt)) && (Port < MAX_FW_KTI_PORTS)) {
          SetupRemoteCpuBootPath (SocketData, KtiInternalGlobal, &GrandParent, &Parent, Port);
        }
      }
    }


    Child.RawNode = 0;
    Child.Node.Valid = TRUE;
    Child.Node.Hop = Parent.Node.Hop + 1; // One level down from parent
    Child.Node.ParentIndex = Index; // Remember who is the immediate parent of this child
    for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {
      if (SocketData->Cpu[Parent.Node.SocId].LinkData[KtiLink].Valid == TRUE) {
        //
        // Get the Child Socket Id, Type and port that connects it to the parent
        //
        Child.Node.SocId = SocketData->Cpu[Parent.Node.SocId].LinkData[KtiLink].PeerSocId;
        Child.Node.SocType = SocketData->Cpu[Parent.Node.SocId].LinkData[KtiLink].PeerSocType;
        Child.Node.ParentPort = SocketData->Cpu[Parent.Node.SocId].LinkData[KtiLink].PeerPort;

        //
        // Make sure that this socket is not already found in the tree.
        //
        if (NodeFoundInMinPathTree (SocketData->SbspMinPathTree, &Child) == TRUE) {
          continue;
        }

        //
        // When setting up Child Node that is at least 2 hops away from the Legacy CPU, if the
        // ports used for route through traffic on the Parent Node does not allow route through
        // traffic, we will skip setting up this path.
        // The same Child Node will be added using a different Parent Node in subsequent discovery.
        // This is added because SPR has limitation where UPI Port 3 cannot be used for route through traffic.
        //
        if (Child.Node.Hop > 1) {
          if (SocketData->Cpu[Parent.Node.SocId].LinkData[KtiLink].DisallowRouteThru == TRUE) {
            //
            // Outgoing port on Parent Node does not support Route Through traffic
            //
            continue;
          }

          if ((Parent.Node.ParentPort < KtiInternalGlobal->KtiPortCnt) &&
              (SocketData->Cpu[Parent.Node.SocId].LinkData[Parent.Node.ParentPort].DisallowRouteThru == TRUE)) {
            //
            // Incoming port on Parent Node does not support Route Through traffic
            //
            continue;
          }
        }

        //
        // Add the node to the tree.
        //
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n Adding %a%u to the tree", (Child.Node.SocType == SOCKET_TYPE_FPGA) ? "PGA" : "CPU", Child.Node.SocId));

        KtiCheckPoint ((UINT8)(1 << Child.Node.SocId), (UINT8)Child.Node.SocType, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_ADD_SOCKET_TO_MIN_PATH_TREE);
        if ((Status = AddNodeToTopologyTree (SocketData->SbspMinPathTree, &Child)) != KTI_SUCCESS) {
          KtiCheckPoint ((UINT8)(1 << Child.Node.SocId), (UINT8)Child.Node.SocType, 0, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INTERNAL_DATA_STRUCTURE);
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Adding %a%u to Minimum Path Tree failed - Data Structure Overflow.", (Child.Node.SocType == SOCKET_TYPE_FPGA) ? "PGA" : "CPU", Child.Node.SocId));
          KTI_ASSERT (FALSE, ERR_MINIMUM_PATH_SETUP, MINOR_ERR_INTERNAL_DATA_STRUCTURE);
        }
        //
        // Add the node to the socketdata
        //
        if ((Child.Node.SocType == SOCKET_TYPE_CPU) || (Child.Node.SocType == SOCKET_TYPE_FPGA)) {
          SocketData->Cpu[Child.Node.SocId].Valid = TRUE;
          SocketData->Cpu[Child.Node.SocId].SocId = (UINT8)Child.Node.SocId;
          SocketData->Cpu[Child.Node.SocId].SocType = (UINT8)Child.Node.SocType;
        } else {
          KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }

        if ((Child.Node.SocType == SOCKET_TYPE_CPU) || (SocketFpgasInitialized (KtiInternalGlobal))) {
          //
          // Setup path from Sbsp to the new found node so that the Sbsp can access the new found node's CSRs.
          //
          KtiCheckPoint ((UINT8)(1 << Child.Node.SocId), (UINT8)Child.Node.SocType, 0, STS_SETUP_MINIMUM_PATH, MINOR_STS_SET_SBSP_CONFIG_PATH);
          SetupSbspConfigAccessPath (SocketData, KtiInternalGlobal, &Parent, &Child, KtiLink);
          //
          // If Child Socket is FPGA, update Child node SocType the SocketData tree structures (Child FPGA Socket type is initialized by SetupSbspConfigAccessPath)
          //
          if (Child.Node.SocType == SOCKET_TYPE_FPGA) {
            SocketData->Cpu[Child.Node.SocId].SocType = SOCKET_TYPE_FPGA;
            SocketData->Cpu[Parent.Node.SocId].LinkData[KtiLink].PeerSocType = SOCKET_TYPE_FPGA;
            //
            // Search through MinPathTree to update the Child Socket Type
            //
            for (Index1 = 0; Index1 < MAX_TREE_NODES; Index1++) {
              if (SocketData->SbspMinPathTree[Index1].Node.SocId == Child.Node.SocId) {
                SocketData->SbspMinPathTree[Index1].Node.SocType = SOCKET_TYPE_FPGA;
                break;
              }
            }
          }
        } else {
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n"));
        }
      }
    } // for(KtiLink...)
  } // for(Index...)

  //
  // Print the Minimum Path Tree
  //
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nSBSP Minimum Path Tree"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n----------------------"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nIndex  Socket  ParentPort  Hop  ParentIndex"));
  for (Index = 0; SocketData->SbspMinPathTree[Index].Node.Valid != FALSE; Index++) {
    if (SocketData->SbspMinPathTree[Index].Node.SocType == SOCKET_TYPE_CPU) {
      if (SocketData->SbspMinPathTree[Index].Node.SocId == sbspSktId) {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n %02u     CPU%u    --         %1u     --", Index,
          SocketData->SbspMinPathTree[Index].Node.SocId, 0));
      } else {
        KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n %02u     CPU%u    %02u         %1u     %02u", Index,
          SocketData->SbspMinPathTree[Index].Node.SocId, SocketData->SbspMinPathTree[Index].Node.ParentPort,
          SocketData->SbspMinPathTree[Index].Node.Hop, SocketData->SbspMinPathTree[Index].Node.ParentIndex));
      }
    }
  }

  //
  // Slow Mode WA start
  //
#if defined (IA32)
#if defined (PPO_FLAG)
  if (IsSkxFamilyCpu (KtiInternalGlobal->CpuType)) {
    UpiSlowModeWaSkx(SocketData, KtiInternalGlobal);
  }
#endif // PPO_FLAG
#endif //defined (IA32)

  return KTI_SUCCESS;
}

/**
  Routine to check fpga links and degrade if needed
  Fpga only allowed in 1/2S

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
PreProcessFpgaLinks (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8     Socket;
  UINT32    FpgaList = 0;

  //
  // Update the global structure with the number of and list of fpgas populated
  //

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((SocketData->Cpu[Socket].Valid == TRUE) && (SocketData->Cpu[Socket].SocType == SOCKET_TYPE_FPGA)) {
       FpgaList |= (1 << Socket);
    }
  }

  if ((KTIVAR->SysConfig >= SYS_CONFIG_4S) && (FpgaList != 0)) {
    //
    // Fpga not allowed in 4s or above, reduce to 1S
    //
    DegradeTo1S (SocketData, KtiInternalGlobal);
    KTIVAR->SysConfig = SYS_CONFIG_1S;
  }

  if (SocketFpgasInitialized (KtiInternalGlobal)) {
    KtiInternalGlobal->FpgaList = FpgaList;
  }
  if (FpgaList) {
    KtiInternalGlobal->ProgramCreditFpga = 1;
  }

  return KTI_SUCCESS;
}

/**
  Degrade the system to 1S topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval KTI_SUCCESS

**/
KTI_STATUS
DegradeTo1S (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8   Idx;

  for (Idx = 0; Idx < MAX_SOCKET; Idx++) {
    if (Idx != KtiInternalGlobal->SbspSoc) {
      // Mark all sockets except socket legacy socket as invalid.
      SocketData->Cpu[Idx].Valid = 0;
      SocketData->Cpu[Idx].SocId = 0;
    }
    ZeroMem ((UINT8 *) SocketData->Cpu[Idx].LinkData, sizeof (SocketData->Cpu[Idx].LinkData));
    KtiInternalGlobal->CpuList = 0x00000001;
  }

  return KTI_SUCCESS;
}

/**
  Routine to interpret the system topology in the format used by Enhanced Warning Log
  I.E. 64 bit bit mask:
  Each socket has KtiPortCnt bits
  [(8*KtiPortCnt - 1):7*KtiPortCnt] - link bit mask for socket 7
  [(7*KtiPortCnt - 1):6*KtiPortCnt] - link bit mask for socket 6
   ....
  [(2*KtiPortCnt - 1):  KtiPortCnt] - link bit mask for socket 1
  [(KtiPortCnt - 1)  :           0] - link bit mask for socket 0

  @param SocketData        - Info such as CPUs populated and their LEP.
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS - on successful completion

**/
UINT64
GetTopologyBitmap (
  KTI_SOCKET_DATA          *SocketData,
  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal
) {

  UINT8  Index;
  UINT8  Port;
  UINT64 TopologyBitmap = 0;

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
        if (SocketData->Cpu[Index].LinkData[Port].Valid == TRUE) {
          TopologyBitmap = TopologyBitmap | LShiftU64 (1, (Index * (KtiInternalGlobal->KtiPortCnt) + Port));
        }
      }
    }
  }
  return TopologyBitmap;
}

/**
  Routine to log topology change in EWL output structure.
  These warnings will only be added to the Enhanced Warning Log, if it is defined at build time

  @param TopologyBefore - Bit mask, where bit x corresponds to CPU (CPU:0...3). Set to 0xFF if the Warning is system wide.
  @param TopologyAfter  - 0: CPU socket. Set to 0xFF if the Warning is system wide.
  @param Reason         - Bit mask, where bit x corresponds to CPU KTI port 0/1. Set to 0xFF if the Warning is system wide, 0 if Link Not Applicable.

  @retval None.

**/VOID
KtiLogTopologyChange (
  UINT64                  TopologyBefore,
  UINT64                  TopologyAfter,
  TOPOLOGY_DEGRADE_REASON Reason
)
{
  EWL_ENTRY_TYPE21 type21;

  type21.Header.Size     = sizeof (EWL_ENTRY_TYPE21);
  type21.Header.Type     = EwlType21;
  type21.Header.Severity = EwlSeverityWarning;

  type21.DegradedFrom = TopologyBefore;
  type21.NewTopology  = TopologyAfter;
  type21.Reason       = Reason;

  //
  // Output warning details to the Serial Log
  //
  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n\n**** KTI Topology Change Logged ****"));
  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\nTopology before degrade: 0x%016x", type21.DegradedFrom));
  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\nTopology after degrade:  0x%016x", type21.NewTopology));
  KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\nDegrade reason code   :  0x%x\n",   type21.Reason));


  //
  //log the warning in the Enhanced Warning Log structure
  //

  EwlLogEntry (&type21.Header);

}

/**
  Chooses config precedence topology vs feature

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
PreProcessFeatureVsTopology (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8 topology = KTIVAR->SysConfig;
  UINT64 PreDegradeTopo;
  UINT64 PostDegradeTopo;
  PreDegradeTopo = GetTopologyBitmap (SocketData, KtiInternalGlobal);
  //
  // By default we will choose topology as precedence for defining configuration, if setup option is set then choose feature as precedence
  // If topology precedence is set, PrimeHostStructureIsoc() will disable isoc in unsupported topologies (i.e. 2S1L)
  //
  if (KTISETUP->DegradePrecedence == FEATURE_PRECEDENCE) {
    //Isoc will only be enabled in 1S, and 2S2L
    //if 2S1L topology with FEATURE_PRECEDENCE set, reduce to 1S
    if (KTISETUP->isocEn == 1 &&
        topology == SYS_CONFIG_2S1L){
      KTIVAR->SysConfig = SYS_CONFIG_1S;
      DegradeTo1S (SocketData, KtiInternalGlobal);
      PostDegradeTopo = GetTopologyBitmap (SocketData, KtiInternalGlobal);
      KtiLogTopologyChange (PreDegradeTopo, PostDegradeTopo, FeatureVsTopology);
    }
  }
  return KTI_SUCCESS;
}

/**
  Sanity checks the LEP data.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuData           - socket specific data structure

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
SanityCheckLep (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData
  )
{
  UINT8   Idx, Port;
  UINT8   PeerSocId, PeerPort;

  //
  // Do some sanity checking of the link exchange data such as link Valid,
  // Peer Socket ID and Peer Port are consistent at both the ends of a given link.
  //

  for (Idx = 0; Idx < MAX_SOCKET; Idx++) {
    if (CpuData[Idx].Valid == TRUE) {
      for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
        if (CpuData[Idx].LinkData[Port].Valid == TRUE) {
          PeerSocId = (UINT8) CpuData[Idx].LinkData[Port].PeerSocId;
          PeerPort  = (UINT8) CpuData[Idx].LinkData[Port].PeerPort;

          if ((PeerPort >= (KtiInternalGlobal->KtiPortCnt)) || (PeerSocId >= MAX_SOCKET)) {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n PeerPort:%u >= KtiPortCnt:%u or PeerSocId:%u >= MAX_SOCKET:%u",
              PeerPort, (KtiInternalGlobal->KtiPortCnt), PeerSocId, MAX_SOCKET));
            KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
            return KTI_FAILURE;
          }

          if (CpuData[PeerSocId].Valid != TRUE ||
              CpuData[PeerSocId].LinkData[PeerPort].Valid != TRUE ||
              CpuData[PeerSocId].LinkData[PeerPort].PeerSocId != Idx ||
              CpuData[PeerSocId].LinkData[PeerPort].PeerPort != Port) {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n CPU%u Port%u has incosistent LEP with Peer socket. Topology Not Supported", Idx, Port));
            KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_1B);
          }
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Dumps Link Exchange parameters to output device

  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuData           - socket specific data structure

  @retval VOID

**/
VOID
LepDump (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData
  )
{
  UINT8         Index;

  UINT8         Index1;
  KTI_LINK_DATA *LinkData;

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n\nLink Exchange Parameter"));
  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n-----------------------"));

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (CpuData[Index].Valid == TRUE) {
      LinkData = CpuData[Index].LinkData;
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n%a%u ", (CpuData[Index].SocType == SOCKET_TYPE_FPGA)  ? "PGA":"CPU", Index));
      for (Index1 = 0; Index1 < KtiInternalGlobal->KtiPortCnt; Index1++) {
        if (LinkData[Index1].Valid == TRUE) {
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, ": LEP%u(%u:%a%u) ", Index1, LinkData[Index1].PeerPort, (LinkData[Index1].PeerSocType == SOCKET_TYPE_FPGA) ? "PGA":"CPU", LinkData[Index1].PeerSocId));
        }
      }
    }
  }

  return ;
}

/**
  This routine checks if the given non-2S topology has more than link connecting any 2 given sockets. If
  found, it retains only one and marks the other link(s) as invalid.This routine should not be called if the
  given system is 2S.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param SocketData        - Socket specific data structure

  @retval 0 - Successful

**/
KTI_STATUS
RemoveDualLink (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_SOCKET_DATA            *SocketData
  )
{
  UINT8   Idx1, Idx2, Idx3;
  UINT32  PeerSocId;
  UINT32  PeerPort;

  for (Idx1 = 0; Idx1 < MAX_SOCKET; Idx1++) {
    if (SocketData->Cpu[Idx1].Valid == TRUE) {
      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
        if (SocketData->Cpu[Idx1].LinkData[Idx2].Valid == TRUE) {
          for (Idx3 = 0; Idx3 < KtiInternalGlobal->KtiPortCnt; Idx3++) {
            if ((Idx3 != Idx2) && (SocketData->Cpu[Idx1].LinkData[Idx3].Valid == TRUE)) {
              //
              // Check if this is a dual link and mark it as invalid
              //
              if ((SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocId == SocketData->Cpu[Idx1].LinkData[Idx3].PeerSocId) &&
                  (SocketData->Cpu[Idx1].LinkData[Idx2].PeerSocType == SocketData->Cpu[Idx1].LinkData[Idx3].PeerSocType)) {
                PeerSocId    = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx3].PeerSocId;
                PeerPort     = (UINT8) SocketData->Cpu[Idx1].LinkData[Idx3].PeerPort;

                if ((PeerPort >= (KtiInternalGlobal->KtiPortCnt)) || (PeerSocId >= MAX_SOCKET)) {
                  KtiDebugPrintFatal ((KTI_DEBUG_ERROR, " PeerPort:%u >= KtiPortCnt:%u or PeerSocId:%u >= MAX_SOCKET:%u\n",
                    PeerPort, (KtiInternalGlobal->KtiPortCnt), PeerSocId, MAX_SOCKET));
                  KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
                  return KTI_FAILURE;
                }

                KtiLogWarning (Idx1, SOCKET_TYPE_CPU, Idx3, WARN_INVALID_LINK);
                KtiDebugPrintWarn ((KTI_DEBUG_WARN, "  CPU%u has more than one link (%u, %u) connecting to CPU%u (%u, %u).   Disabling link CPU%uP%u <-> CPU%uP%u.\n",
                                Idx1, Idx2, Idx3,                                                    //Soc # and local dual-link ports
                                PeerSocId, SocketData->Cpu[Idx1].LinkData[Idx2].PeerPort, PeerPort,  //Peer Soc # and Peer dual-link ports
                                Idx1, Idx3, PeerSocId, PeerPort));                                   //Link to be disabled:  Soc # and port (both ends)
                SocketData->Cpu[Idx1].LinkData[Idx3].Valid = 0;
                SocketData->Cpu[PeerSocId].LinkData[PeerPort].Valid = 0;
              }
            }
          }
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  This routine checks if the given topology has more than one link connecting any 2 given sockets.
  If found, it marks the second link as duallink on both connecting sockets. This routine also
  updates the field "DualLinkIndex" on the found links so that the rest of topology routines can
  search for the duallink using "DualLinkIndex" field.

  @param[in]     KtiInternalGlobal   KTIRC internal variables.
  @param[in,out] SocketData          Socket specific data structure

  @retval KTI_SUCCESS

**/
KTI_STATUS
ProcessDualLink (
  IN     KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN OUT KTI_SOCKET_DATA            *SocketData
  )
{
  UINT8    SocId;
  UINT8    Idx1;
  UINT8    Idx2;
  UINT32   PeerSocId;
  UINT32   PeerPort;

  for (SocId = 0; SocId < MAX_SOCKET; SocId++) {
    if (SocketData->Cpu[SocId].Valid == 0) {
      continue;
    }

    for (Idx1 = 0; Idx1 < KtiInternalGlobal->KtiPortCnt; Idx1++) {
      if (SocketData->Cpu[SocId].LinkData[Idx1].Valid == 0) {
        continue;
      }

      for (Idx2 = 0; Idx2 < KtiInternalGlobal->KtiPortCnt; Idx2++) {
        if ((Idx2 != Idx1) && (SocketData->Cpu[SocId].LinkData[Idx2].Valid == TRUE)) {
          //
          // Check if this is a dual link
          //
          if ((SocketData->Cpu[SocId].LinkData[Idx1].PeerSocId == SocketData->Cpu[SocId].LinkData[Idx2].PeerSocId) &&
              (SocketData->Cpu[SocId].LinkData[Idx1].PeerSocType == SocketData->Cpu[SocId].LinkData[Idx2].PeerSocType)) {
            PeerSocId    = (UINT8) SocketData->Cpu[SocId].LinkData[Idx2].PeerSocId;
            PeerPort     = (UINT8) SocketData->Cpu[SocId].LinkData[Idx2].PeerPort;

            if ((PeerPort >= (KtiInternalGlobal->KtiPortCnt)) || (PeerSocId >= MAX_SOCKET)) {
              KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n PeerPort:%u >= KtiPortCnt:%u or PeerSocId:%u >= MAX_SOCKET:%u",
                PeerPort, (KtiInternalGlobal->KtiPortCnt), PeerSocId, MAX_SOCKET));
              KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
              return KTI_FAILURE;
            }

            //
            // Skip if the link has already identified as dual link
            //
            if (SocketData->Cpu[SocId].LinkData[Idx2].DualLinkIndex != 0) {
              continue;
            }

            if (SocketData->Cpu[SocId].LinkData[Idx2].DualLink == TRUE ||
                SocketData->Cpu[SocId].LinkData[Idx1].DualLink == TRUE) {
              continue;
            }

            //
            // Mark the second matching link as "DualLink" and update DualLinkIndex field
            // on both matching links.
            //
            SocketData->Cpu[SocId].LinkData[Idx2].DualLink = TRUE;
            SocketData->Cpu[SocId].LinkData[Idx2].DualLinkIndex = Idx1;
            SocketData->Cpu[SocId].LinkData[Idx1].DualLinkIndex = Idx2;

            //
            // Repeat the same on Peer Socket.
            //
            SocketData->Cpu[PeerSocId].LinkData[PeerPort].DualLink = TRUE;
            SocketData->Cpu[PeerSocId].LinkData[PeerPort].DualLinkIndex = SocketData->Cpu[SocId].LinkData[Idx1].PeerPort;
            PeerPort     = (UINT8) SocketData->Cpu[SocId].LinkData[Idx1].PeerPort;
            if (PeerPort >= (KtiInternalGlobal->KtiPortCnt)) {
              KTI_ASSERT ((PeerPort < (KtiInternalGlobal->KtiPortCnt)), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_KTI_PORT_ID);
              return KTI_FAILURE;
            }
            SocketData->Cpu[PeerSocId].LinkData[PeerPort].DualLinkIndex = SocketData->Cpu[SocId].LinkData[Idx2].PeerPort;

            KtiDebugPrintInfo0 ((
              KTI_DEBUG_INFO0,
              "\n  CPU%u Link%u's duallink is Link%u; Peer CPU%u Link%u's duallink is Link%u",
              SocId, Idx1, SocketData->Cpu[SocId].LinkData[Idx1].DualLinkIndex,
              PeerSocId, PeerPort, SocketData->Cpu[PeerSocId].LinkData[PeerPort].DualLinkIndex
              ));

            break;
          }
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  This routine checks if the number of links connecting 2 given sockets match the requested
  topology including 2S2L, 2S3L, 2S4L.
  If found, for 2S2L, it marks the second link as DualLink on both connecting sockets.
            for 2S3L, it marks the second link and third link as TwoSkt3Link on both connecting sockets.
            for 2S4L, it marks the 2nd/3rd/4th link as TwoSkt4Link on both connecting sockets.
  If the routine fails to setup 2S2L, 2S3L or 2S4L, the routine will configure the topology as 2S1L.

  @param[in]     KtiInternalGlobal   KTIRC internal variables.
  @param[in,out] SocketData          Socket specific data structure
  @param[in]     RootSocId           Root socket ID.
  @param[in]     PeerSocId           Peer socket ID.
  @param[in]     NumOfLinksToCheck   Number of active UPI links to check, value should be 2, 3 or 4

  @retval KTI_SUCCESS  The number of links connecting the given Root and Peer sockets match the requested
                       topology. If requested topology is not successful, the topology is setup as 2S1L.

**/
KTI_STATUS
ProcessTwoSocketMultipleLinks (
  IN     KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN OUT KTI_SOCKET_DATA            *SocketData,
  IN     UINT8                      RootSocId,
  IN     UINT8                      PeerSocId,
  IN     UINT8                      NumOfLinksToCheck
  )
{
  UINT8      Index;
  UINT8      PeerPort;
  UINT8      Port0;
  UINT8      Port1;
  UINT8      Port2;
  UINT8      Port3;
  UINT8      NumOfLinksFound;

  Port0 = Port1 = Port2 = Port3 = 0xFF;
  NumOfLinksFound = 0;

  KTI_ASSERT (((NumOfLinksToCheck == 3) || (NumOfLinksToCheck == 4) || (NumOfLinksToCheck == 2)), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_8A);

  for (Index = 0; Index < KtiInternalGlobal->KtiPortCnt; Index++) {
    if (SocketData->Cpu[RootSocId].LinkData[Index].Valid == 0) {
      continue;
    }

    if (SocketData->Cpu[RootSocId].LinkData[Index].PeerSocId != PeerSocId) {
      continue;
    }

    PeerPort = (UINT8) SocketData->Cpu[RootSocId].LinkData[Index].PeerPort;

    if (NumOfLinksFound == NumOfLinksToCheck) {
      //
      // We've already found sufficient number of active links required.
      // Remove the extra active links
      //
      SocketData->Cpu[RootSocId].LinkData[Index].Valid = 0;
      SocketData->Cpu[PeerSocId].LinkData[PeerPort].Valid = 0;
      continue;
    }

    if (Port0 == 0xFF) {
      Port0 = Index;
      NumOfLinksFound++;

      if ((NumOfLinksToCheck == 2) && (SocketData->Cpu[RootSocId].LinkData[Index].PeerPort != Index)) {
        KTIVAR->TwistedKtiLinks = TRUE;
      }
    } else if ((Port1 == 0xFF) || (Port2 == 0xFF) || (Port3 == 0xFF)) {
      if (NumOfLinksToCheck == 2) {
        SocketData->Cpu[RootSocId].LinkData[Index].DualLink = TRUE;
        SocketData->Cpu[PeerSocId].LinkData[PeerPort].DualLink = TRUE;
      } else if (NumOfLinksToCheck == 3) {
        SocketData->Cpu[RootSocId].LinkData[Index].TwoSkt3Link = TRUE;
        SocketData->Cpu[PeerSocId].LinkData[PeerPort].TwoSkt3Link = TRUE;
      } else {
        SocketData->Cpu[RootSocId].LinkData[Index].TwoSkt4Link = TRUE;
        SocketData->Cpu[PeerSocId].LinkData[PeerPort].TwoSkt4Link = TRUE;
      }
      NumOfLinksFound++;
      if (Port1 == 0xFF) {
        Port1 = Index;
      } else if (Port2 == 0xFF) {
        Port2 = Index;
      } else {
        Port3 = Index;
      }
    }
  }

  if (NumOfLinksFound != NumOfLinksToCheck) {
    KtiDebugPrintInfo0 ((
      KTI_DEBUG_INFO0,
      "\n2S%dL error! Found %d active links between S%d and S%d!\n",
      NumOfLinksToCheck,
      NumOfLinksFound,
      RootSocId,
      PeerSocId
      ));
    RemoveDualLink (KtiInternalGlobal, SocketData);
    KTIVAR->SysConfig = SYS_CONFIG_2S1L;
    return KTI_SUCCESS;
  }

  switch (NumOfLinksToCheck) {
  case 2:
    KTIVAR->SysConfig = SYS_CONFIG_2S2L;
    if (Is16EntryUpiRoutingTableSupported ()) {
      KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeB2;
    } else {
      KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeB0;
    }
    break;

  case 3:
    KTIVAR->SysConfig = SYS_CONFIG_2S3L;
    if (Is16EntryUpiRoutingTableSupported ()) {
      KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeC1;
    } else {
      KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeC0;
    }
    break;

  case 4:
    KTIVAR->SysConfig = SYS_CONFIG_2S4L;
    //
    // Check whether it's 2S4L twisted mode
    // S0U0 <-> S1U1 S0U1 <-> S1U0 S0U2 <-> S1U2 S0U3 <-> S1U3
    //
    if (SocketData->Cpu[RootSocId].LinkData[0].PeerPort != 0) {
      KTIVAR->TwistedKtiLinks = TRUE;
    }
    KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeD0;
    break;

  default:
    break;
  }

  return KTI_SUCCESS;
}

/**
  This routine checks if the given topology has 2 links connecting 2 given sockets.
  This routine expects at least two active links from UPI Port 0, Port 1 and Port 2.
  SKX/CLX requires both Port 0 and Port 1 to be active for 2S2L topology.

  @param[in]     KtiInternalGlobal   KTIRC internal variables.
  @param[in,out] SocketData          Socket specific data structure

  @retval KTI_SUCCESS  Found 2 links connecting given Root and Peer sockets.

**/
KTI_STATUS
ProcessTwoSocket2Links (
  IN     KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN OUT KTI_SOCKET_DATA            *SocketData
  )
{
  UINT8      Socket;

  KTIVAR->SysConfig = SYS_CONFIG_2S2L;

  if ((SocketData->Cpu[0].LinkData[1].PeerPort != 0) && (SocketData->Cpu[0].LinkData[2].PeerPort != 0) ||
      (SocketData->Cpu[1].LinkData[1].PeerPort != 0) && (SocketData->Cpu[1].LinkData[2].PeerPort != 0)) { // There is no Port 0
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n WARN: There is no UPI PORT 0 in Socket\n"));
  }

  if (IsSkxFamilyCpu (KtiInternalGlobal->CpuType)) {
    if ((KtiInternalGlobal->KtiPortCnt == 3) &&
        (((SocketData->Cpu[0].LinkData[2].Valid == TRUE) && (SocketData->Cpu[0].LinkData[2].PeerSocType == SOCKET_TYPE_CPU)) ||
         ((SocketData->Cpu[1].LinkData[2].Valid == TRUE) && (SocketData->Cpu[1].LinkData[2].PeerSocType == SOCKET_TYPE_CPU)))) {
      //
      // Remove the dual links if any KTI links 2 is one of two active KTI links between CPU sockets.
      // KTI links 2 is not allowed for 2S2L.
      // KTI link 2 is allowed to connect to FPGA.
      //
      RemoveDualLink (KtiInternalGlobal, SocketData);
      KTIVAR->SysConfig = SYS_CONFIG_2S1L;
    }
  }

  if (KTIVAR->SysConfig == SYS_CONFIG_2S2L) {
    //
    // Mark link as DualLink
    //
    for (Socket = 0; Socket < 2; Socket++) {
      if (SocketData->Cpu[Socket].LinkData[1].Valid == TRUE) {
        SocketData->Cpu[Socket].LinkData[1].DualLink = TRUE;
        SocketData->Cpu[Socket].LinkData[0].DualLinkIndex = 1;
      } else if (SocketData->Cpu[Socket].LinkData[2].Valid == TRUE) {
        SocketData->Cpu[Socket].LinkData[2].DualLink = TRUE;       // Mark link 2 as DualLink
        SocketData->Cpu[Socket].LinkData[0].DualLinkIndex = 2;
      }
    }

    if (SocketData->Cpu[0].LinkData[0].PeerPort != 0) {
      KTIVAR->TwistedKtiLinks = TRUE;
    }

    if (Is16EntryUpiRoutingTableSupported ()) {
      KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeB2;
    } else {
      KtiInternalGlobal->UpiRoutingMode = UpiRoutingModeB0;
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to check if the system has supported topology. Following terminology is used in
  describing the system topology:
  1S    - Only socket 0 is populated
  2S1L -Socket 0 & 1 are populated; there is only one link between them. Link number doesn't matter.
  2S2L -Socket 0 & 1 are populated; there are two links between them. (Link 0 & 1 only)
  4S -Total number of populated sockets is <= 4, populated sockets have ID < 4, and it is not a 1S/2S system.
  8S -All other configurations.

Note:
  When degraded topology is detected (i.e one or more links failed), BIOS will try to reduce it to one of the
  supported topology by forcing certain links and/or sockets out. Link/Socket removal is done by marking the
  appropriate data structures; nothing specifically done to the h/w that will disable a link or socket. That will
  take effect only after next reset. We track the number of and list of sockets populated before and after
  topology reduction is applied. While KTIRC uses only the reduced topology information, other modules
  such as the code that issues Warm reset needs to be aware of what is the list of sockets populated
  currently. So we update the Host numCpus/socketPresentBitMap structure with the current number of and
  list of sockets populated, not based on the reduced topology.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
PreProcessKtiLinks (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8   Idx, ActivelinkOnCpu0, ActivelinkOnCpu1, Port;
  UINT8   TempTotCpu;
  UINT32  TempCpuList;
#if KTI_DEBUG_LEVEL
  UINT8   Topology[SYS_CONFIG_MAX][7] = {{"1S"}, {"2S1L"}, {"2S2L"}, {"2S3L"}, {"2S4L"}, {"4S"}, {"8S"}};
#endif // KTI_DEBUG_LEVEL
  UINT32  BoardTypeBitmask;
  UINT64  OriginalTopologyBitmask = GetTopologyBitmap (SocketData, KtiInternalGlobal);
  UINT64  FinalTopologyBitmask;
  BOOLEAN IsCpx;
  BOOLEAN IsSpr;

  KtiCheckPoint (0xFF, 0xFF, 0xFF, STS_TOPOLOGY_DISCOVERY, MINOR_STS_CHK_TOPOLOGY);

  // Sanity check the LEP info
  SanityCheckLep (KtiInternalGlobal, SocketData->Cpu);
  LepDump (KtiInternalGlobal, SocketData->Cpu);

  IsCpx = FALSE;
  IsSpr = FALSE;
  if (KtiInternalGlobal->CpuType == CPU_CPX) {
    IsCpx = TRUE;
  } else if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    IsSpr = TRUE;
  }

  //
  // Identify the number of and list of sockets populated.
  //
  TempTotCpu  = 0;
  TempCpuList = 0;

  for (Idx = 0; Idx < MAX_SOCKET; Idx++) {
    if ((SocketData->Cpu[Idx].Valid == TRUE) && (SocketData->Cpu[Idx].SocType == SOCKET_TYPE_CPU)) {
      TempTotCpu++;
      TempCpuList = TempCpuList | (1 << Idx);
    }
  }
  KtiInternalGlobal->OrigTotCpu = TempTotCpu;
  KtiInternalGlobal->OrigCpuList = TempCpuList;

  KTIVAR->SysConfig = SYS_CONFIG_MAX;
  if (TempTotCpu == 1) { // 1S system
    KTIVAR->SysConfig = SYS_CONFIG_1S;

    if (IsCpx && (GetDieCount (KtiInternalGlobal->SbspSoc) == TWO_DIE_IN_SOCKET)) {  // CPX MCP
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\nFatal Error: CPX MCP system does not support 1S topology!!\n"));
      KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
    }
  } else if ((TempTotCpu == 2) && (TempCpuList == 0x3)) { // 2S system
    ActivelinkOnCpu0 = ActivelinkOnCpu1 = 0;
    //check how many links active
    for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
      if ((SocketData->Cpu[0].LinkData[Port].Valid == TRUE) && (SocketData->Cpu[0].LinkData[Port].PeerSocType == SOCKET_TYPE_CPU)) {
         ActivelinkOnCpu0 += 1;
      }
      if ((SocketData->Cpu[1].LinkData[Port].Valid == TRUE) && (SocketData->Cpu[1].LinkData[Port].PeerSocType == SOCKET_TYPE_CPU)) {
         ActivelinkOnCpu1 += 1;
      }
    }

    //
    // 2S3L is special case. A system will be declared as 2S only when it has sockets 0 & 1 populated, 3 KTI links are connecting
    // the 2 sockets and the 2 sockets are connected by KTI links 0 & 1, 2.
    //
    if ((ActivelinkOnCpu0 == 3 && ActivelinkOnCpu1 == 3) ||
        (IsCpx && (ActivelinkOnCpu0 > 3 && ActivelinkOnCpu1 > 3))) {
      ProcessTwoSocketMultipleLinks (KtiInternalGlobal, SocketData, 0, 1, 3);
    } else if (ActivelinkOnCpu0 == 2 && ActivelinkOnCpu1 == 2) {
      if (IsCpx) {
        //
        // CPX allows any UPI port number to be setup as 2S2L
        //
        ProcessTwoSocketMultipleLinks (KtiInternalGlobal, SocketData, 0, 1, 2);
      } else {
        ProcessTwoSocket2Links (KtiInternalGlobal, SocketData);
      }
    } else if (ActivelinkOnCpu0 == 4 && ActivelinkOnCpu1 == 4) {
      ProcessTwoSocketMultipleLinks (KtiInternalGlobal, SocketData, 0, 1, 4);
    } else {
      if (IsCpx && (GetDieCount (KtiInternalGlobal->SbspSoc) == TWO_DIE_IN_SOCKET)) {  // CPX MCP
        KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n\nFatal Error: CPX MCP system does not support 2S1L topology!!\n"));
        KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
      }
      //
      // Ensure that only one link connection exist between the 2 sockets it will be treated as 2S1L
      //
      RemoveDualLink (KtiInternalGlobal, SocketData);
      KTIVAR->SysConfig = SYS_CONFIG_2S1L;
    }
  } else if ((TempTotCpu <= 4) && ((TempCpuList & 0xF0) == 0)) { // 4S system
    //
    // Check and remove the dual links if there is any (except CPX and SPR).
    // Also check if it has supported topology otherwise force supported topology.
    //
    if (IsCpx || IsSpr) {
      ProcessDualLink (KtiInternalGlobal, SocketData);
    } else {
      RemoveDualLink (KtiInternalGlobal, SocketData);
    }
    CheckForDegraded4S (SocketData, KtiInternalGlobal);
  } else if (TempTotCpu <= MAX_SOCKET) { // 8S system
    //
    // Check and remove the dual links (except CPX and SPR) if there is any.
    // Also check if it has supported topology otherwise force supported topology.
    //
    if (IsCpx || IsSpr) {
      KTIVAR->SysConfig = SYS_CONFIG_8S;       //CHECK_CPX
      ProcessDualLink (KtiInternalGlobal, SocketData);
    } else {
      RemoveDualLink (KtiInternalGlobal, SocketData);
    }
    CheckForDegraded8S (SocketData, KtiInternalGlobal);
  } else {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n  Unsupported topology"));
    KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
  }

  FinalTopologyBitmask = GetTopologyBitmap (SocketData, KtiInternalGlobal);
  if (OriginalTopologyBitmask != FinalTopologyBitmask) {
    KtiLogTopologyChange (OriginalTopologyBitmask, FinalTopologyBitmask, InvalidTopology);
  }

  //
  // If hot plug enabled, will program credits assuming it as 4S or 8S system;
  // so that the Credits for already populated sockets need not be reprogrammed during hot add/remove event.
  //
  if (KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE){
    KTIVAR->SysConfig = SYS_CONFIG_4S;      // force to 4S for future hot plug
    TempTotCpu =  MAX_SOCKET;
    if ((KTISETUP->KtiCpuSktHotPlugTopology == 1) && (TempTotCpu == 8)) {
      KTIVAR->SysConfig = SYS_CONFIG_8S;   // force to 8S for future hot plug
    }
  }

  if (KTIVAR->SysConfig == SYS_CONFIG_8S){
    KtiInternalGlobal->IsRouteThrough = TRUE;
  }

  //
  // Sanity check the LEP info to make sure the degraded topology handling is done properly
  //
  SanityCheckLep (KtiInternalGlobal, SocketData->Cpu);

  //
  // Check for feature Vs Topology Precedence
  //
  PreProcessFeatureVsTopology (SocketData, KtiInternalGlobal);

  PreProcessFpgaLinks (SocketData, KtiInternalGlobal);

  //
  // Update the global structure with the number of and list of sockets populated
  //
  for (Idx = 0; Idx < MAX_SOCKET; Idx++) {
    if ((SocketData->Cpu[Idx].Valid == TRUE) && (SocketData->Cpu[Idx].SocType == SOCKET_TYPE_CPU)) {
      KtiInternalGlobal->TotCpu++;
      KtiInternalGlobal->CpuList = KtiInternalGlobal->CpuList | (1 << Idx);
    }
  }
  if (KtiInternalGlobal->CpuList != KtiInternalGlobal->OrigCpuList) {
    LepDump (KtiInternalGlobal, SocketData->Cpu);
  } else {
    KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n  Already Reduced to Supported Topology"));
  }

  //
  // Check to make sure the system configuration is valid
  //
  KTI_ASSERT ((KTIVAR->SysConfig < SYS_CONFIG_MAX), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);

  KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\n  System will be treated %a Configuration", Topology[KTIVAR->SysConfig]));

  // Copy the LEP Info and Topology Tree Info into Host structure
  for (Idx = 0; Idx < MAX_SOCKET; Idx++) {
    if (SocketData->Cpu[Idx].Valid == TRUE) {
      KTIVAR->CpuInfo[Idx].Valid  = SocketData->Cpu[Idx].Valid;
      KTIVAR->CpuInfo[Idx].SocId  = SocketData->Cpu[Idx].SocId;
      KTIVAR->CpuInfo[Idx].SocType  = SocketData->Cpu[Idx].SocType;
      CopyMem ((UINT8 *) KTIVAR->CpuInfo[Idx].LepInfo, (UINT8 *) SocketData->Cpu[Idx].LinkData, sizeof (KTIVAR->CpuInfo[Idx].LepInfo));
    }
  }

  //
  // Check if all valid sockets are supported by the board
  //
  BoardTypeBitmask = KTISETUP->BoardTypeBitmask;
  KTIVAR->OutBoardVsCpuConflict = 0;
  for (Idx = 0; Idx < MAX_SOCKET; Idx++) {
    if ((SocketData->Cpu[Idx].Valid == TRUE) && (SocketData->Cpu[Idx].SocType == SOCKET_TYPE_CPU)) {
      if ((BoardTypeBitmask & ((1 << KTIVAR->CpuInfo[Idx].CpuType) << (4 * Idx))) == 0) {
        KTIVAR->OutBoardVsCpuConflict |= (1 << Idx);
      }
    }
  }
  if (KTIVAR->OutBoardVsCpuConflict != 0) {
    KtiDebugPrintInfo1 ((KTI_DEBUG_WARN, "\n WARNING:  A board VS CPU conflict detected. Hiding MCP devices."));
    HideMcpDevices (KtiInternalGlobal, SocketData);
  }

  KTIVAR->Is4SRing = KtiInternalGlobal->Is4SRing;

  return KTI_SUCCESS;
}

/**
  Routine to sync up the PBSPs and update the necessary scratch registers. As part of that,
  the sockets that are not connected to legacy are issued "BOOT_GO" command to bring them
  under BIOS control.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param RstType           - Boot path type.  powergood or warm reset flow

**/
KTI_STATUS
SyncUpPbspForReset (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      RstType
  )
{
  UINT8                              Index, Idx2;
  UINT8                              SteppingMismatch = FALSE;
  UINT32                             GblNidMap0, GblNidMap1, GidNidMap;
  UINT32                             Ctr, WaitTime;
  UINT32                             SbspLtTarget;
  UINT32                             SbspLtTargetEnabled;
  UINT32                             ApLtTarget;
  UINT32                             ApLtTargetEnabled;
  UINT8                              CheckInFlag[MAX_SOCKET], AllCheckInFlag;
  UINT8                              LowestStepping;
  UINT8                              SbspStepping;
  UINT32                             IntList, StopStartList, MiscList, IntPrioList, BcastList;
  BOOLEAN                            bCheckInBitsSet;
  UINT8                              SteppingInScratchPad;
  UINT8                              SocketLocalNid;

  //
  //Update the SBSP Ubox sticky scratch register 02 with the LEP info, which can include cases
  //where links have failed to train or there's been a topology change.
  //
  UpdateKtiTopologyInScratchpad (SocketData, KtiInternalGlobal);

  //
  // Set the "BOOT_GO" flag in the remote socket to bring it under BIOS control.
  //
  SetMem ((UINT8 *) &CheckInFlag, sizeof (CheckInFlag), FALSE);
  if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    SocketLocalNid = SOCKET_LOCAL_5BITS_NID_MSB;
  } else {
    SocketLocalNid = SOCKET_LOCAL_4BITS_NID_MSB;
  }

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((Index == KtiInternalGlobal->SbspSoc) || (SocketData->Cpu[Index].Valid != TRUE)) {
      CheckInFlag[Index] = TRUE;
      continue;
    } else if ((!CheckCpuConnectedToLegCpu (SocketData, KtiInternalGlobal, Index) || SocketRequireBootGo (SocketData, KtiInternalGlobal, Index)) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
      //
      // BIOS has to initialize the following fields of SAD_TARGET_CHABC_SAD1 register for the remote socket.
      // For the legacy socket pCode will set them for this register.  For sockets directly connected to legacy socket,
      // pCode will only program flash_target and lt_target.   BIOS must program legacy_pch_target
      //
      ChaSadTargetCfg (Index, 0, KtiInternalGlobal->SbspSoc, KtiInternalGlobal->SbspSoc, CHA_DEFAULT_CFG, CHA_DEFAULT_CFG, CHA_DEFAULT_CFG);

      //
      // BIOS has to initialize the LT_CONTROL register for the remote socket.
      // For the legacy socket and the sockets directly connected to it, the pCode will set this register.
      // So we just replicate the register setting from SBSP to the remote socket.
      //
      ChaGetLtTargetAndStatus (KtiInternalGlobal->SbspSoc, 0, &SbspLtTarget, &SbspLtTargetEnabled);
      if (SbspLtTargetEnabled == 0x1) {
        if (SbspLtTarget & SocketLocalNid) {
          ApLtTarget = KtiInternalGlobal->SbspSoc;
        } else {
          ApLtTarget = SbspLtTarget;
        }
        ApLtTargetEnabled = SbspLtTargetEnabled;
        ChaSetLtTargetAndStatus (Index, 0, ApLtTarget, ApLtTargetEnabled);
      }

      //
      // In LT enabled system, releasing a non-directly connected socket may cause a LT_SHUTDOWN,
      // which causes a surprise warm reset.
      // Set bit29 of SP7 to indicate we are releasing the remote socket, so that if LT_SHUTDOWN happens.
      // BIOS can detect this and handle it.
      //
      for (Idx2 = 0; Idx2 < MAX_SOCKET; Idx2++) {
        if ((SocketData->Cpu[Idx2].Valid == TRUE) && (SocketData->Cpu[Idx2].SocType == SOCKET_TYPE_CPU)) {
          UBoxSetRemoteSocketReleasedFlagInScratchpad (Idx2, 0, 1);
        }
      }

      //
      // Issue BOOT_GO command
      //
      KtiCheckPoint ((UINT8)(1 << Index), SOCKET_TYPE_CPU, 0, STS_PBSP_SYNC_UP, MINOR_STS_ISSUE_BOOT_GO);
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Issuing BOOT_GO command to CPU%u.", SocketData->Cpu[Index].SocId));
      UBoxIssueBootGoCommand (Index, 0);
    } else {                         // skip the socket directly connected to SBSP
      CheckInFlag[Index] = TRUE;
    }
  }

  //
  // Wait for the remote CPU(s) to check-in.
  //
  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n    Verifying if the remote socket(s) checked-in. \n"));
  WaitTime = KTISETUP->WaitTimeForPSBP;

  // Loop until either all CPUs checked-in or the wait time expires
  Idx2=0;
  for (Ctr = 0; Ctr < WaitTime; Ctr++) {
    AllCheckInFlag = TRUE; // Assume all CPUs checked-in
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if (CheckInFlag[Index] == FALSE) { // If this CPU has not checked-in yet
        UBoxCheckInBitsIsSetInScratchpad (Index, 0, &bCheckInBitsSet);
        if (bCheckInBitsSet) {  // Read CheckInBit to determine if AP has checked-in
          CheckInFlag[Index] = TRUE;
        } else {
          AllCheckInFlag = FALSE; // Indicate that at least one CPU hasn't checked-in
          Idx2=Index;
        }
      }
    }
    if (AllCheckInFlag == FALSE) {
      KtiFixedDelay (1000); // 1000 microsecs
    } else {
      break;
    }
  }

  //
  // Handle the failure
  //
  if (Ctr == WaitTime) { // If the remote socket(s) hasn't checked-in, assert; it is a fatal condition
    KtiCheckPoint ((UINT8)(1 << Index), SOCKET_TYPE_CPU, 0xFF, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE);
    KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n\n  Remote Socket %u is discovered but hasn't come out of reset. System Halted!!!", Idx2));
    KTI_ASSERT (FALSE, ERR_BOOT_MODE, MINOR_ERR_PBSP_CHKIN_FAILURE);
  }

  //
  // In LT enabled system, releasing a non-directly connected socket may cause a LT_SHUTDOWN, which causes a surprise warm reset.
  // If we made it here, that is not the case, so clear the bit
  //
  for (Idx2 = 0; Idx2 < MAX_SOCKET; Idx2++) {
    if ((SocketData->Cpu[Idx2].Valid == TRUE) && (SocketData->Cpu[Idx2].SocType == SOCKET_TYPE_CPU)) {
      UBoxSetRemoteSocketReleasedFlagInScratchpad (Idx2, 0, 0);
    }
  }


  if ((RstType == POST_RESET_POWERGOOD) || (KtiInternalGlobal->ForceColdResetFlow == TRUE) || (GetSysResetRequired () == POST_RESET_POWERGOOD)) {
    //
    // Update the number/list of CPUs populated
    //
    GblNidMap0 = 0;         // for socket 0-3
    GblNidMap1 = 0;         // for socket 4-7
    GidNidMap = 0;
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
          if ( Index < 4) {
              GblNidMap0 = GblNidMap0 | (1 << (28 + Index)) | (Index << (4 * Index));
          } else {
              GblNidMap1 = GblNidMap1 | (1 << (28 + (Index - 4))) | (Index << (4 * (Index - 4)));
          }
          GidNidMap = GidNidMap | (Index << (3 * Index));
      }
    }

    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
        // Local Node id
        UBoxConfigureCpuLocalId (Index, 0, Index, 0x7);
        // Group Id and Node Id mapping
        UBoxConfigureGidNidMapping (Index, 0, GidNidMap);

        // CpuEnable
        UBoxConfigureCpuEnabled (Index, 0, KtiInternalGlobal->CpuList);

        // Bcastlist
        IntList = StopStartList = MiscList = IntPrioList = KtiInternalGlobal->CpuList;
        UBoxConfigureCpuBroadcastList (Index, 0, IntList, StopStartList, MiscList, IntPrioList, &BcastList);

        // CpuNodeID
        PcuSetGblNid0to3Map (Index, GblNidMap0);
        if (GblNidMap1 != 0) {
          PcuSetGblNid4to7Map (Index, GblNidMap1);
        }
      }
    }

    //
    // At this point we are in cold-reset flow; now all CPUs can access all other socket's CSRs and their
    // path to FWH agent is set. Request for reset.
    //
    SetNewResetRequired (POST_RESET_WARM);
    CopyMem ((UINT8 *) KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].TopologyInfo,
             (UINT8 *) SocketData->SbspMinPathTree, sizeof (KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].TopologyInfo));
  }

  //
  // Read stepping from scratch pad register of each PBSP and store them into sysHost struct
  //
  SbspStepping = KtiInternalGlobal->PackageBspStepping[KtiInternalGlobal->SbspSoc];
  LowestStepping = 0xFF;
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
      //
      // CSR bit[30:27] = CPU Stepping
      //
      UBoxGetCpuSteppingInScratchpad (Index, 0, &SteppingInScratchPad);
      KtiInternalGlobal->PackageBspStepping[Index] = SteppingInScratchPad;
      KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "packageBspStepping[%x]=%x\n", Index, KtiInternalGlobal->PackageBspStepping[Index]));

      //
      // Update common.stepping to the lowest stepping level
      //
      if (KtiInternalGlobal->PackageBspStepping[Index] < LowestStepping) {
        LowestStepping = KtiInternalGlobal->PackageBspStepping[Index];
      }

      //
      // Log a warning if there's a stepping mismatch
      //
      if (KtiInternalGlobal->PackageBspStepping[Index] != SbspStepping) {
        KtiLogWarning (0xFF, 0xFF, 0xFF, WARN_CPU_STEPPING_MISMATCH);
        KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n WARNING:  CPU stepping mismatch detected.\n"));
        SteppingMismatch = TRUE;
      }

      //
      // Get every CPU's ChopType
      //
      KtiInternalGlobal->ChopType[Index] = PcuGetPhysicalChop (Index);
      KTIVAR->CpuInfo[Index].ChopType = KtiInternalGlobal->ChopType[Index];
    }
  }

  //
  // If lower stepping value found, CpuAndRevision will update to use it
  //
  NormalizeCpuAndRevision (LowestStepping);

  if (SteppingMismatch == TRUE) {
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\n  Socket | Stepping \n"));
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
        KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "  %d      | 0x%x\n", Index, KtiInternalGlobal->PackageBspStepping[Index]));
      }
    }
  }

  return KTI_SUCCESS;
}



/**
  Program the TOR Threshold loctorem_thresh_norm and loctorem_thresh_empty for 4S ring or 8S

  @param KtiInternalGlobal - KTIRC internal variables.

  @retval VOID

**/
VOID
Init_TOR_Threshold_Loctorem (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
)
{
  UINT8              LoctoremNormTable[3][5] = {{LOCTOREM_THRESH_NORM_DIS, LOCTOREM_THRESH_NORM_AUTO, LOCTOREM_THRESH_NORM_LOW, LOCTOREM_THRESH_NORM_MEDIUM, LOCTOREM_THRESH_NORM_HIGH},
                                                 {LOCTOREM_THRESH_NORM_DIS, LOCTOREM_THRESH_NORM_4SRING_AUTO, LOCTOREM_THRESH_NORM_LOW, LOCTOREM_THRESH_NORM_MEDIUM, LOCTOREM_THRESH_NORM_HIGH},
                                                 {LOCTOREM_THRESH_NORM_DIS, LOCTOREM_THRESH_NORM_8S_AUTO, LOCTOREM_THRESH_NORM_LOW, LOCTOREM_THRESH_NORM_MEDIUM, LOCTOREM_THRESH_NORM_HIGH}};
  UINT8              LoctoremEmptyTable[3][5] = {{LOCTOREM_THRESH_EMPTY_DIS, LOCTOREM_THRESH_EMPTY_AUTO, LOCTOREM_THRESH_EMPTY_LOW, LOCTOREM_THRESH_EMPTY_MEDIUM, LOCTOREM_THRESH_EMPTY_HIGH},
                                                  {LOCTOREM_THRESH_EMPTY_DIS, LOCTOREM_THRESH_EMPTY_4SRING_AUTO, LOCTOREM_THRESH_EMPTY_LOW, LOCTOREM_THRESH_EMPTY_MEDIUM, LOCTOREM_THRESH_EMPTY_HIGH},
                                                  {LOCTOREM_THRESH_EMPTY_DIS, LOCTOREM_THRESH_EMPTY_8S_AUTO, LOCTOREM_THRESH_EMPTY_LOW, LOCTOREM_THRESH_EMPTY_MEDIUM, LOCTOREM_THRESH_EMPTY_HIGH}};
  UINT8               ConfigIndex = 0;

  if ((KTIVAR->SysConfig == SYS_CONFIG_4S) && KTIVAR->Is4SRing){
    ConfigIndex=1;
  } else if ((KTIVAR->SysConfig == SYS_CONFIG_8S)){
    ConfigIndex=2;
  } else {
    ConfigIndex=0;
  }

  if (KtiInternalGlobal->CpuType == CPU_ICXSP || IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    KTIVAR->OutTorThresLoctoremNorm = LoctoremNormTable[ConfigIndex][KTISETUP->TorThresLoctoremNorm];
    KTIVAR->OutTorThresLoctoremEmpty = LoctoremEmptyTable[ConfigIndex][KTISETUP->TorThresLoctoremEmpty];
  }
}

/**
  Returns boolean if FPGA active for a given CPU socket.
  Whitley FPGA topologies are always "active", WHR will respond to CFG accesses in cold-reset flow

  FPGA TODO: Delete this function and any flow control statements based on it

  @param KtiInternalGlobal    - KTIRC internal variables.

  @retval TRUE    - FPGA can respond to CFG accesses
  @retval FALSE   - "Blue Bitstream" not already "downloaded"

**/
BOOLEAN
SocketFpgasInitialized (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return TRUE;
}

/**
  Checks if more than 1 link exists between parent & child from their LEP.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuData           - Contains the LEP of Parent & Child nodes
  @param Parent            - Socket at one end of the link
  @param Child             - Socket at the other end of the link

  @retval TRUE   - More than 1 link exists
  @retval FALSE  - 1 or 0 link exists

**/
BOOLEAN
MoreThanOneLinkExists (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData,
  TOPOLOGY_TREE_NODE         *Parent,
  TOPOLOGY_TREE_NODE         *Child
  )
{
  UINT8         KtiLink;
  KTI_LINK_DATA TempLep;
  UINT8         Count;
  UINT8         LinkValid;

  KTI_ASSERT ((CpuData != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_1D);
  KTI_ASSERT ((Parent != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_1E);
  KTI_ASSERT ((Child != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_1F);

  Count = 0;

  if (Parent->Node.SocType == SOCKET_TYPE_CPU) {
    for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {
      TempLep = CpuData[Parent->Node.SocId].LinkData[KtiLink];

      LinkValid = (UINT8) TempLep.Valid;

      if (((UINT8) TempLep.DualLink == 1) && !KtiInternalGlobal->DualLinksEn) {
        LinkValid = 0;
      }

      if (LinkValid == 1 && TempLep.PeerSocId == Child->Node.SocId && TempLep.PeerSocType == Child->Node.SocType) {
        Count++;
      }
    }
  }

  if (Count > 1) {
    return TRUE;
  }

  return FALSE;
}

/**
  Returns the Kti port numbers of the dual or triple or 4 link of Node1 socket.
  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuData           - Contains the LEP of Node1 & Node2 nodes
  @param Node1             - Socket at one end of the link
  @param Node2             - Socket at the other end of the link
  @param Ports             - An array to include ports of the 2/3/4 link
  @param MaxLinks          - Max Links in array Ports

  @retval Ports            - Ports number of links


**/
KTI_STATUS
GetMultiLinkFromLEP (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData,
  TOPOLOGY_TREE_NODE         *Node1,
  TOPOLOGY_TREE_NODE         *Node2,
  UINT8                      *Ports,
  UINT8                      MaxLinks
  )
{
  UINT8         KtiLink;
  UINT8         PortIdx;
  KTI_LINK_DATA TempLep;

  KTI_ASSERT ((CpuData != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_20);
  KTI_ASSERT ((Node1 != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_21);
  KTI_ASSERT ((Node2 != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_22);

  for (PortIdx = 0; PortIdx < MaxLinks; PortIdx++) {
    Ports[PortIdx] = KTI_INVALID_PORT;
  }
  //
  // Only CPU sockets can have a dual-link
  //
  if (Node1->Node.SocType == SOCKET_TYPE_CPU) {
    for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {
      TempLep = CpuData[Node1->Node.SocId].LinkData[KtiLink];

      if (TempLep.Valid == 1 && TempLep.PeerSocId == Node2->Node.SocId && TempLep.PeerSocType == Node2->Node.SocType) {
        for (PortIdx = 0; PortIdx < MaxLinks; PortIdx++) {
          if (Ports[PortIdx] == KTI_INVALID_PORT) {
            Ports[PortIdx] = KtiLink;
            break;
          }
        }
        if (PortIdx == MaxLinks) {
          //
          // parent has more than MaxLinks links to child
          //
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n CPU %u has more than %u links to CPU %u. Topology not supported", Node1->Node.SocId, MaxLinks, Node2->Node.SocId));
          KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_TOPOLOGY);
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Translate generic port number to Si port

  @param GenPort - GenPort number to be translated

  @retval port number

**/
UINT8
TranslateToSiPort (
  UINT8                   GenPort
  )
{

  // Currently for all supported CPUs, Si port numbering and generic port numbering is same
  return GenPort;
}

/**
  Dump topology info onto output device

  @param[in] KtiInternalGlobal   KTIRC internal variables.
  @param[in] Cpu                 socket specific data structure
  @param[in] CpuTree             Cpu topology tree structure pointer
  @param[in] AlternateCpuTree    Alternate route for Cpu topology tree structure pointer
  @param[in] SiPort              TRUE to display UPI port number with actual Silicon Port ID;
                                 FALSE otherwise.

  @retval None

**/
VOID
TopologyInfoDump (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN KTI_CPU_SOCKET_DATA        *Cpu,
  IN TOPOLOGY_TREE_NODE         (*CpuTree)[MAX_TREE_NODES],
  IN TOPOLOGY_TREE_NODE         (*AlternateCpuTree)[MAX_TREE_NODES],
  IN BOOLEAN                    SiPort
  )
{
#if (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
  UINT8               Index, Index1, Index2;
  UINT8               Index3;
  UINT8               Port;
  UINT8               RootVn;
  UINT8               TempStr[MAX_SOCKET * 2];
  TOPOLOGY_TREE_NODE  *TopologyTree;
  TOPOLOGY_TREE_NODE  Parent, Child;

  Port = 0;

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((Cpu[Index].Valid == TRUE) && (Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
      TopologyTree = CpuTree[Index];
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n\nCPU%u Topology Tree", Index));
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n-------------------"));
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\nIndex  Socket  ParentSocket  ParentPort  ParentIndex  Hop  XOR"));
      for (Index1 = 0; TopologyTree[Index1].Node.Valid != FALSE; Index1++) {
        if (Index1 == 0) {
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n %02u     CPU%u      %a           %a          %a        %1u    %a",
                          Index1, TopologyTree[Index1].Node.SocId, " -- ", "--", "--", TopologyTree[Index1].Node.Hop, "--"));
        } else {
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n %02u     %a%u      CPU%u           %02u          %02u        %1u%6a",
                        Index1,
                        (TopologyTree[Index1].Node.SocType == SOCKET_TYPE_CPU) ? "CPU" : "PGA",
                        TopologyTree[Index1].Node.SocId,
                        TopologyTree[TopologyTree[Index1].Node.ParentIndex].Node.SocId,
                        TopologyTree[Index1].Node.ParentPort,
                        TopologyTree[Index1].Node.ParentIndex,
                        TopologyTree[Index1].Node.Hop,
                        "0"));
        }

        if (KtiInternalGlobal->CpuType == CPU_CPX && AlternateCpuTree[Index][Index1].Node.Valid == TRUE) {
          KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n %02u     %a%u      CPU%u           %02u          %02u        %1u%6a",
            Index1,
            (AlternateCpuTree[Index][Index1].Node.SocType == SOCKET_TYPE_FPGA) ? "PGA" : "CPU",
            AlternateCpuTree[Index][Index1].Node.SocId,
            TopologyTree[AlternateCpuTree[Index][Index1].Node.ParentIndex].Node.SocId,
            AlternateCpuTree[Index][Index1].Node.ParentPort,
            AlternateCpuTree[Index][Index1].Node.ParentIndex,
            AlternateCpuTree[Index][Index1].Node.Hop,
            "1"
            ));
        }
      }
    }
  }

  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if ((Cpu[Index].Valid == TRUE) &&  (Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
      KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n"));
      TopologyTree = CpuTree[Index];
      for (Index1 = 1; TopologyTree[Index1].Node.Valid != FALSE; Index1++) {
        for (Index3 = 0; Index3 < 2; Index3++) {
          RootVn = Cpu[TopologyTree[0].Node.SocId].Vn;
          if (Index3 == 0) {
            Child = TopologyTree[Index1];
          } else {
            Child = AlternateCpuTree[Index][Index1];
            if ((UINT8) Child.Node.Valid == FALSE) {
              continue;
            }
          }

          SetMem ((VOID *) TempStr, sizeof (TempStr), '$');
          Index2 = 0;
          while (Child.Node.Hop != 0) {
            if (Index2 >= (MAX_SOCKET * 2)) {
              KtiDebugPrintFatal ((KTI_DEBUG_ERROR,
                            "\nERR_SW_CHECK: 0x%X!!!!  Invalid value. Index2: 0x%X is greater or equal to (MAX_SOCKET * 2): 0x%X",
                            MINOR_ERR_SW_CHECK_51,
                            Index2,
                            (MAX_SOCKET * 2)
                            ));
              KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_51);
            }

            if (Child.Node.ParentIndex >= MAX_SOCKET) {
              KTI_ASSERT ((Child.Node.ParentIndex < MAX_SOCKET), ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_INVALID_CPU_SOCKET_ID);
              return;
            }
            Parent = TopologyTree[Child.Node.ParentIndex];
            Port = GetChildLocalPortFromLEP (KtiInternalGlobal, Cpu, &Parent, &Child);
            TempStr[Index2++] = (UINT8)((Child.Node.SocId  << 4) | Child.Node.ParentPort);
            if (SiPort == TRUE) {
              Port = TranslateToSiPort (Port);
              TempStr[Index2++] = (UINT8)(Parent.Node.SocId << 4) | Port;
            } else {
              TempStr[Index2++] = (UINT8)(Parent.Node.SocId << 4) | Port;
            }
            Child = Parent;
          }

          if (Index2 > (MAX_SOCKET * 2)) {
            KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_1C);
          } else {
            KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\n"));

            if (IsUpiPortWithOppositeRootVn (KtiInternalGlobal, Cpu, (UINT8) Parent.Node.SocId, Port)) {
              RootVn = (RootVn == VN0) ? VN1 : VN0;
            }

            for (Index2 = Index2 - 1; Index2 != 0; Index2--) {
              KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1,
                            "\"S%u P%u VN%u %a\" -> ",
                            (TempStr[Index2] >> 4),
                            (TempStr[Index2] & 0xF),
                            RootVn,
                            (Index2 % 2) ? "TX" : "RX"
                            ));
            }
            KtiDebugPrintInfo1 ((KTI_DEBUG_INFO1, "\"S%u P%u VN%u RX\";", (TempStr[Index2] >> 4), (TempStr[Index2] & 0xF), RootVn));
          } // checking Index2 value
        }
      }
    }
  }
#endif // (KTI_DEBUG_LEVEL & KTI_DEBUG_INFO1)
  return;
}

/**
  interrogatest discovered topology for errors

  @param SocketData        - socket specific data structure
  @param KtiInternalGlobal - KTIRC internal variables.


  @retval 0 - Successful
  @retval 1 - Failures

**/
KTI_STATUS
SanityCheckDiscoveredTopology (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  BOOLEAN             ErrFlag;
  UINT8               Index;
  UINT8               Index1;
  UINT32              TempCpuList;
  TOPOLOGY_TREE_NODE  *TopologyTree;

  ErrFlag = FALSE;

  //
  // Topology Tree sanity check. Check the topology tree of each socket to see if it has all the other sockets in its tree.
  //
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    TempCpuList = 0;
    if ((SocketData->Cpu[Index].Valid == TRUE) && (SocketData->Cpu[Index].SocType == SOCKET_TYPE_CPU)) {
      TopologyTree  = SocketData->CpuTree[Index];
      TempCpuList   = KtiInternalGlobal->CpuList;
      for (Index1 = 0; TopologyTree[Index1].Node.Valid != FALSE; Index1++) {
        if (TopologyTree[Index1].Node.SocType == SOCKET_TYPE_CPU) {
          if (TempCpuList) {
            TempCpuList &= (~(1 << TopologyTree[Index1].Node.SocId));
          } else {
            KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n CPU%u not found in CPU%u Topology Tree!!!", Index1, Index));
            ErrFlag = TRUE;
            break;
          }
        } else if (TopologyTree[Index1].Node.SocType == SOCKET_TYPE_FPGA) {
        } else {
          KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Invalid Socket Type found in CPU%u Topology Tree!!!", Index));
          ErrFlag = TRUE;
        }
      }
    }

    if (TempCpuList) {
      KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n One or more CPUs not found in CPU%u Topology Tree!!!", Index));
      ErrFlag = TRUE;
    }
  }

  if (ErrFlag == TRUE) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\n Topology Discovery Failed!!!\n"));
    if (KTIVAR->RasInProgress == TRUE) {
      return KTI_FAILURE;
    } else {
      KTI_ASSERT (FALSE, ERR_TOPOLOGY_DISCOVERY, MINOR_ERR_SANITY_CHECK);
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to check if the topology changed after warm reset. Any such topology change requires
  KTIRC cold reset flow to be forced on the system since the uncore mesh programming is no longer correct for the
  changed topology. Note that when such topology change occurs it could be only due to some links not training; it can not
  be due to some links that failed to train before but trained successfully this time around. The reason is a link
  that failt to train is disabled by clearing c_init_begin. So such links can successfully train only after Cold Reset.

  @param SocketData        - Info such as CPUs populated and their LEP.
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval KTI_SUCCESS - on successful completion

**/
KTI_STATUS
CheckForTopologyChange (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  UINT8           Index, Port;
  UINT64          Data64;
  UINT64          KtiPortBitmap;
  UINT8           ScratchpadBits;
  UINT64          Scratchpad2Topology;
  UINT64          NewTopologyBitmap;

  // Collect the previous topology/LEP from SR02
  UBoxGetKtiTopologyInScratchpad (KtiInternalGlobal->SbspSoc, 0, &Data64);

  KtiPortBitmap = RShiftU64 (Data64, 8);

  ScratchpadBits = 8;
  Scratchpad2Topology = 0;
  for (Index = 0; Index < MAX_SOCKET; Index++){
    for (Port = 0; Port < (KtiInternalGlobal->KtiPortCnt); Port++){
      if (1 << ScratchpadBits & Data64){
        Scratchpad2Topology |= LShiftU64 (1, (Index * (KtiInternalGlobal->KtiPortCnt) + Port));
      }
      ScratchpadBits++;
    }
  }

  // Check if the current LEP matches previous boot LEP
  for (Index = 0; Index < MAX_SOCKET; Index++) {
    if (SocketData->Cpu[Index].Valid == TRUE) {
      for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
        if (SocketData->Cpu[Index].LinkData[Port].Valid != ((RShiftU64 (KtiPortBitmap, ((Index * (KtiInternalGlobal->KtiPortCnt)) + Port)) & 0x1))) {
          NewTopologyBitmap = GetTopologyBitmap (SocketData, KtiInternalGlobal);
          KtiLogTopologyChange (Scratchpad2Topology, NewTopologyBitmap, LinkFail);
          KtiLogWarning (Index, SOCKET_TYPE_CPU, Port, WARN_LINK_FAILURE);
          KtiDebugPrintWarn ((KTI_DEBUG_WARN, "\n    CPU%u Link%u failed to train. Topology changed across reset!!!", Index, Port));
          KtiInternalGlobal->ForceColdResetFlow = TRUE; // Indicate that the Cold Reset flow needs to be redone
          if (KTISETUP->DfxParm.DfxHaltLinkFailReset == KTI_ENABLE) {
            KTI_ASSERT (FALSE, ERR_UNSUPPORTED_TOPOLOGY, MINOR_ERR_TOPOLOGY_CHANGE);
          }
        }
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Find a KTI node within overall KTI fabric

  @param[in] KtiInternalGlobal      KTIRC internal variables.
  @param[in,out] Tree               KTI fabric tree structure.
                                    If the node is found in alternate routes table, this
                                    function will replace the original node with the one
                                    in Alternate routes table.
  @param[in] AlternateTree          Alternate route for KTI fabric.
  @param[in,out] Parent             Parent tree node
  @param[in] SearchNode             Node to search for.
  @param[out] FoundInAlternateTree  TRUE if the node is found in Alternate routes table.
                                    FALSE otherwise.

  @retval TRUE   Node found in topology tree (either main or alternate tree)
  @retval FALSE  Mode isn't found in any topology tree.

**/
BOOLEAN
NodeFoundInTopologyTree (
  IN     KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN OUT TOPOLOGY_TREE_NODE         *Tree,
  IN     TOPOLOGY_TREE_NODE         *AlternateTree,
  IN OUT TOPOLOGY_TREE_NODE         *Parent,
  IN     TOPOLOGY_TREE_NODE         *SearchNode,
  OUT    BOOLEAN                    *FoundInAlternateTree
  )
{
  UINT8                Index;

  KTI_ASSERT ((Tree != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_35);
  KTI_ASSERT ((SearchNode != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_36);

  *FoundInAlternateTree = FALSE;
  for (Index = 0; Index < MAX_TREE_NODES; Index++) {
    //
    // If there are no more valid nodes, then the node in question is not in the tree
    //
    if (Tree[Index].Node.Valid == FALSE) {
      return FALSE;
    }

    //
    // If socket ID and Type matches then we found the node in the tree
    //
    if (Tree[Index].Node.SocId == SearchNode->Node.SocId &&
        Tree[Index].Node.SocType == SearchNode->Node.SocType &&
        Tree[Index].Node.Hop == SearchNode->Node.Hop &&
        Tree[Index].Node.ParentPort == SearchNode->Node.ParentPort
        ) {
      *FoundInAlternateTree = FALSE;
      return TRUE;
    }

    if (KtiInternalGlobal->CpuType == CPU_CPX) {
      //
      // Try finding the node in the alternate route
      //
      if (AlternateTree[Index].Node.Valid == 0) {
        continue;
      }

      if (AlternateTree[Index].Node.SocId == SearchNode->Node.SocId &&
          AlternateTree[Index].Node.SocType == SearchNode->Node.SocType &&
          AlternateTree[Index].Node.Hop == SearchNode->Node.Hop &&
          AlternateTree[Index].Node.ParentPort == SearchNode->Node.ParentPort
          ) {
        //
        // Update the node with the one found in Alternate routes table
        //
        Tree[Index].RawNode = AlternateTree[Index].RawNode;

        *FoundInAlternateTree = TRUE;
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
  Adds the current node to the tree

  @param Tree  - system KTI fabric node tree
  @param Node  - current node tree

  @retval 0 - Successful
  @retval 1 - Failure

**/
KTI_STATUS
AddNodeToRingTree (
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *Node
  )
{
  UINT8 Index;

  KTI_ASSERT ((Tree != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_3B);
  KTI_ASSERT ((Node != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_3C);

  for (Index = 0; Index < MAX_RING_TREE_NODES; Index++) {
    //
    // Place the tree in first available empty slot in the array
    //
    if (Tree[Index].Node.Valid == FALSE) {
      Tree[Index] = *Node;
      return KTI_SUCCESS;
    }
  }

  return KTI_FAILURE;
}

/**
  Searches the Topology tree to see if the node under search is already found in the
  tree at lower Hop levels. In effect, it checks if there is a shorter path exists
  from the root node to this node.

  @param Tree       - Topology Tree
  @param SearchNode - Node to search in the topology tree

  @retval TRUE   - Shorter path exists
  @retval FALSE  - Shorter path doesn't exist

**/
BOOLEAN
ShorterPathExists (
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *SearchNode
  )
{
  UINT8 Index;

  KTI_ASSERT ((Tree != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_3F);
  KTI_ASSERT ((SearchNode != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_40);

  for (Index = 0; Index < MAX_TREE_NODES; Index++) {
    //
    // If there are no more valid nodes, then the node in question is not in the tree
    //
    if (Tree[Index].Node.Valid == FALSE) {
      return FALSE;
    }
    //
    // If socket ID & Type matches and the hop level of the matching socket is less than the hop level of the node under search,
    // we found a shorter path
    //
    if (Tree[Index].Node.SocId == SearchNode->Node.SocId &&
        Tree[Index].Node.SocType == SearchNode->Node.SocType &&
        Tree[Index].Node.Hop < SearchNode->Node.Hop
        ) {
      return TRUE;
    }

  }

  return FALSE;
}

/**
  Compares to Ring arrays.

  @param Ring1 - first ring to compare
  @param Ring2 - second ring to compare

  @retval TRUE  - Rings are same
  @retval FALSE - Rings are different

**/
BOOLEAN
CompareRing (
  UINT8                *Ring1,
  UINT8                *Ring2
  )
{
  UINT8 Idx1, Idx2;
  UINT8 Match = 0;

  KTI_ASSERT ((Ring1 != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_41);
  KTI_ASSERT ((Ring2 != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_42);

  for (Idx1 = 0; Idx1 < 4; Idx1++) {
    for (Idx2 = 0; Idx2 < 4; Idx2++) {
      if (Ring1[Idx1] == Ring2[Idx2]) {
        Match = Match | (1 << Idx1);
        break;
      }
    }
  }

  if (Match == 0xF) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  Read IAR register.

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param UnitId    - Unit ID (lane 0-19=0-19, fwdc=20, reserved=21-62, PQD=62, Broadcast Data Lane=63)
                     Broadcast Data Lane UnitId should NOT be used for reads.
  @param RegOffset - Register offset; values come from the auto generated header file
  @param data      - Data to write

  @retval register value read

**/
UINT8
KtiReadIar (
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT8                 UnitId,
  UINT32                RegOffset
  )
{
  UINT32  WaitTime, Ctr;

  //Wait for "write_en" bit to be cleared before reading KTI IAR register
  Ctr      = 0;
  WaitTime = 5000;
  if (!CheckEmulationType (SIMICS_FLAG)) {
    for (Ctr = 0; Ctr < WaitTime; Ctr++) {
      if (IsIarWriteEnableClear (SocId, BoxInst)) {
        break;
      }
      //
      // Delay 10 microsecs
      //
      KtiFixedDelay (10);
    }
  }
  // Halt the system if timeout
  if (Ctr == WaitTime) {
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nERR_IAR_READ for Socket %d, BoxInst %d, Offset 0x%x!!!! \nCheck PQ_CSR_AFEIAR_KTI_CIOPHYDFX_STRUCT.write_en is TIMEOUT\n", SocId, BoxInst, RegOffset));
    KTI_ASSERT (FALSE, ERR_FULL_SPEED_TRANSITION, MINOR_ERR_S0_TIMEOUT_IAR | SocId);
  }

  //Once write-en bit is cleared, free to write hw_override_en bit and then read the IAR
  return GetIarData (SocId, BoxInst, UnitId, RegOffset);;
}

/**
  Write to IAR register.

  @param SocId     - CPU Socket Node number (Socket ID)
  @param BoxInst   - Box Instance, 0 based
  @param UnitId    - Unit ID (lane 0-19=0-19, fwdc=20, reserved=21-62, PQD=62, Broadcast Data Lane=63)
  @param RegOffset - Register offset; values come from the auto generated header file
  @param data      - Data to write

**/
VOID
KtiWriteIar (
  UINT8                 SocId,
  UINT8                 BoxInst,
  UINT8                 UnitId,
  UINT32                RegOffset,
  UINT8                 Data
  )
{
  UINT32                             WaitTime, Ctr;
  Ctr      = 0;
  WaitTime = 5000;

  if (!CheckEmulationType (SIMICS_FLAG)) {
    for (Ctr = 0; Ctr < WaitTime; Ctr++) {
      if (IsIarWriteEnableClear (SocId, BoxInst)) {
        break;
      }
      //
      // Delay 10 microsecs
      //
      KtiFixedDelay (10);
    }
  }

  //
  // Halt the system once timeout
  //
  if (Ctr == WaitTime){
    KtiDebugPrintFatal ((KTI_DEBUG_ERROR, "\nERR_IAR_WRITE for Socket %d, BoxInst %d, Offset 0x%x, Data 0x%x, Ctr %d!!!! \nCheck PQ_CSR_AFEIAR_KTI_CIOPHYDFX_STRUCT.write_en is TIMEOUT\n", SocId, BoxInst, RegOffset, Data, Ctr));
    KTI_ASSERT (FALSE, ERR_FULL_SPEED_TRANSITION, MINOR_ERR_S0_TIMEOUT_IAR | SocId);
  }

  WriteIarData (SocId, BoxInst, UnitId, RegOffset, Data);
}

#ifdef RAS_SUPPORT
/**
  Get physical stack number for the logical bus number passed in.
  Loops through all stacks.

  @param Soc     - Socket ID the logical buses are checked on
  @param BusAddr - logical bus number

  @retval Bus - physical bus number

**/
UINT32
GetPhysicalBusNumber (
  UINT8    Soc,
  UINT32   BusAddr
  )
{
  UINT32 Bus;

  for (Bus = 0; Bus < MAX_LOGIC_IIO_STACK; Bus++) {
    if (BusAddr == GetStackBus (Soc, (UINT8) Bus)) {
      return Bus;
    }
    //
    // For UBOX1 Bus
    //
    if ((Bus == UBOX_STACK) && (BusAddr == (UINT32) GetStackBus (Soc, (UINT8) Bus) + 1)) {
      return Bus;
    }

  }

  return 0xFFFF;
}
#endif // RAS_SUPPORT

/**

  This routine assigns IO resources for Pbsp CPUs/stacks that are populated. It consults the appropriate inputs given to KTIRC and
  applies the following set of rules:
    1. Resource will be allocated only for the CPUs that are currently populated
     1a. If hot pluggable system, will preallocate resources for all sockets
     1b. On EX Hotpluggable systems, assume same number stacks per socket as sbsp

  @param SocketData        - CPU related info
  @param KtiInternalGlobal - KTIRC internal variables.
  @param IoAlloc           - IoAllocation value for each sokcet, stack
  @param IoBase            - IoBase for resource allocation

  @retval KTI_SUCCESS      - on successful completion

**/
KTI_STATUS
CalculateIoResourcesForPbsp (
  IN KTI_SOCKET_DATA            *SocketData,
  OUT KTI_HOST_INTERNAL_GLOBAL  *KtiInternalGlobal,
  IN UINT8                      IoAlloc[MAX_SOCKET][MAX_LOGIC_IIO_STACK],
  IN UINT32                     IoBase
  )
{
  UINT8          Stack;
  UINT8          Socket;
  UINT32         TmpIoSize;
  UINT8          TargetInSkt;

  //
  // Assign to non-sbsps
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket ++) {
    TargetInSkt = 0;
    if (Socket == KtiInternalGlobal->SbspSoc) {
      continue;
    }
    if (SocketData->Cpu[Socket].Valid == TRUE || KtiInternalGlobal->KtiCpuSktHotPlugEn == TRUE ) {
      KTIVAR->CpuInfo[Socket].CpuRes.IoBase  = (UINT16)IoBase;
      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        if (IoAlloc[Socket][Stack] == 0 || TargetInSkt >= SI_MAX_IO_IOAPIC_TARGET_IN_KTITAD) {
          KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoBase = 0xFFFF;
          KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoLimit = 0;
        } else {
          KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoBase = (UINT16)IoBase;
          TmpIoSize = (IoAlloc[Socket][Stack] * SAD_LEG_IO_GRANTY) - 1;
          KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoLimit = (UINT16)(IoBase + TmpIoSize);
          KTIVAR->CpuInfo[Socket].CpuRes.IoLimit = (UINT16)(IoBase + TmpIoSize);
          IoBase = KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoLimit + 1;
          TargetInSkt++;
        }
      }
      if (KTIVAR->CpuInfo[Socket].CpuRes.IoBase >= KTIVAR->CpuInfo[Socket].CpuRes.IoLimit) {
        // Indicate the resources are not valid by forcing Base > Limit
        KTIVAR->CpuInfo[Socket].CpuRes.IoBase = 0xFFFF;
        KTIVAR->CpuInfo[Socket].CpuRes.IoLimit = 0;
      }
    } else {
      // Indicate the resources are not valid by forcing Base > Limit
      KTIVAR->CpuInfo[Socket].CpuRes.IoBase = 0xFFFF;
      KTIVAR->CpuInfo[Socket].CpuRes.IoLimit = 0;
      for (Stack=0; Stack < MAX_LOGIC_IIO_STACK; Stack++) {
        KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoBase = 0xFFFF;
        KTIVAR->CpuInfo[Socket].CpuRes.StackRes[Stack].IoLimit = 0;
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Returns boolean if FPGA active for a given CPU socket.

  @param SocId             - Soc ID of the CPU who's enabled LLC slice info is requested, 0...3
  @param KtiInternalGlobal - KTIRC internal variables.

  @retval TRUE  - BBS was downloaded for this socket
  @retval FALSE - BBS was not downloaded for this socket

**/
BOOLEAN
WasSocketFpgaInitialized (
  UINT8                      SocId,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  BOOLEAN                 rc = FALSE;
  //
  // If -P sku and attempting to enable FPGA and bitstream initialized
  // Only look at the spad[5] on sbsp
  //
  if ((SocketFpgasInitialized (KtiInternalGlobal)) && \
      (FpgaIpIsNodeFpga (SocId)) && \
      (KtiInternalGlobal->FpgaEnable[SocId])) {
    rc = TRUE;
  }

  return rc;
}

/**
  Returns port number of the Child socket that connects it to the Parent socket.

  @param KtiInternalGlobal - KTIRC internal variables.
  @param CpuData           - Contains the LEP of Parent & Child nodes
  @param Parent            - Socket at one end of the link
  @param Child             - Socket at the other end of the link

  @retval Port of the Child socket that connects it to the Parent socket
  @retval 0xFF - Child is not an immediate neighbor of Parent

**/
UINT8
GetChildPeerPortFromLEP (
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  KTI_CPU_SOCKET_DATA        *CpuData,
  TOPOLOGY_TREE_NODE         *Parent,
  TOPOLOGY_TREE_NODE         *Child
  )
{
  UINT8         KtiLink;
  KTI_LINK_DATA TempLep;

  KTI_ASSERT ((CpuData != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_29);
  KTI_ASSERT ((Parent != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_2A);
  KTI_ASSERT ((Child != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_2B);

  if (Parent->Node.SocType == SOCKET_TYPE_CPU) {
    for (KtiLink = 0; KtiLink < KtiInternalGlobal->KtiPortCnt; KtiLink++) {
      TempLep = CpuData[Parent->Node.SocId].LinkData[KtiLink];
      if (TempLep.Valid != TRUE || TempLep.DualLink == TRUE || TempLep.TwoSkt3Link == TRUE) { // Skip the Invalid/Duplicate Links/3 Links
        continue;
      }

      if (TempLep.PeerSocId == Child->Node.SocId && TempLep.PeerSocType == Child->Node.SocType) {
        return (UINT8) TempLep.PeerPort;
      }
    }
  }

  return 0xFF;
}

/**
  finds the child's parent in tree

  @param CpuData - socket specific data structure
  @param Tree    - system KTI fabric node tree
  @param Child   - current node tree

  @retval ParentIndex - Success
  @retval 0xFF - Failure

**/
UINT8
GetParentIndex (
  KTI_CPU_SOCKET_DATA    *CpuData,
  TOPOLOGY_TREE_NODE     *Tree,
  TOPOLOGY_TREE_NODE     *Child
  )
{
  UINT8         Index;
  KTI_LINK_DATA Lep;

  KTI_ASSERT ((Tree != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_30);
  KTI_ASSERT ((Child != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_31);
  KTI_ASSERT ((Child->Node.Valid == TRUE), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_32);

  if (Child->Node.SocType == SOCKET_TYPE_CPU) {
    Lep = CpuData[Child->Node.SocId].LinkData[Child->Node.ParentPort];

    for (Index = 0; Index < MAX_TREE_NODES; Index++) {
      if (Tree[Index].Node.Valid == TRUE &&
          Tree[Index].Node.SocId == Lep.PeerSocId &&
          Tree[Index].Node.SocType == Lep.PeerSocType
          ) {
        return Index;
      }
    }
  } else {
    KTI_ASSERT (FALSE, ERR_SW_CHECK, MINOR_ERR_SW_CHECK_33);
  }

  return 0xFF;
}


/**
  counts hops to child from current socket

  @param Tree    - system KTI fabric node tree
  @param SocId   - processor socket id
  @param SocType - device type in socket

  @retval 0 - Successful
  @retval 1 - Failure

**/
UINT8
GetChildHopLength (
  TOPOLOGY_TREE_NODE   *Tree,
  UINT8                SocId,
  UINT8                SocType
  )
{
  UINT8 Index;

  KTI_ASSERT  ((Tree != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_34);

  for (Index = 0; Index < MAX_TREE_NODES; Index++) {
    if (Tree[Index].Node.Valid == TRUE && Tree[Index].Node.SocId == SocId && Tree[Index].Node.SocType == SocType) {
      return (UINT8) Tree[Index].Node.Hop;
    }
  }

  return 0xFF;
}

/**
  Counts hops in path

  @param Tree  - system KTI fabric node tree
  @param Node  - current node tree

  @retval 0 - Successful
  @retval 1 - Failure

**/
UINT8
CountImmediateChildren (
  TOPOLOGY_TREE_NODE      *Tree,
  TOPOLOGY_TREE_NODE      *Parent
  )
{
  UINT8 Idx;
  UINT8 ParentIdx;
  UINT8 Children;

  KTI_ASSERT ((Tree != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_3D);
  KTI_ASSERT ((Parent != NULL), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_3E);

  ParentIdx = 0xFF;
  Children  = 0;

  for (Idx = 0; Idx < MAX_TREE_NODES; Idx++) {
    if (Tree[Idx].Node.Valid == FALSE) {
      return Children;
    }

    if (Tree[Idx].Node.SocId == Parent->Node.SocId &&
        Tree[Idx].Node.SocType == Parent->Node.SocType &&
        Tree[Idx].Node.Hop == Parent->Node.Hop &&
        Tree[Idx].Node.ParentPort == Parent->Node.ParentPort
        ) {
      ParentIdx = Idx;
      break;
    }
  }

  if (ParentIdx != 0xFF) {
    for (Idx = 0; Idx < MAX_TREE_NODES; Idx++) {
      if (Tree[Idx].Node.Valid == FALSE) {
        return Children;
      }

      if (Tree[Idx].Node.ParentIndex == ParentIdx) {
        Children++;
      }
    }
  }

  return Children;
}

/**
  Returns neighbor and/or remote socket of the given socket.

  @param SocketData        - pointer socket specific data
  @param KtiInternalGlobal - KTIRC internal variables.
  @param Soc               - current processor socket ID
  @param NeighborSoc1      - KTI port 1 neighbor socket ID
  @param NeighborSoc2      - KTI port 2 neighbor socket ID
  @param RemoteSoc         - Remote socket ID

  @retval VOID

**/
VOID
GetNeighborSocs (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      Soc,
  UINT8                      *NeighborSoc1,
  UINT8                      *NeighborSoc2,
  UINT8                      *RemoteSoc
  )
{
  UINT8 Ctr;
  UINT32 TempList;

  *NeighborSoc1 = *NeighborSoc2 = *RemoteSoc = 0xFF;

  if (KTIVAR->SysConfig == SYS_CONFIG_1S) {
    return;
  } else if (KTIVAR->SysConfig == SYS_CONFIG_2S1L || KTIVAR->SysConfig == SYS_CONFIG_2S2L) {
    (Soc == KtiInternalGlobal->SbspSoc) ? (*NeighborSoc1 = 1) : (*NeighborSoc1 = KtiInternalGlobal->SbspSoc);
    return;
  }

  //
  // Is it a 4S system with only 2 CPUs populated?
  //
  if (KtiInternalGlobal->TotCpu == 2) {
    if (SocketData->Cpu[Soc].LinkData[0].Valid == TRUE) {
      *NeighborSoc1 = (UINT8) SocketData->Cpu[Soc].LinkData[0].PeerSocId;
    } else {
      *NeighborSoc1 = (UINT8) SocketData->Cpu[Soc].LinkData[1].PeerSocId;
    }
    //
    // Even though the other 2 sockets are missing, identify them for credit programming simplicity.
    //
    for (Ctr = 0; Ctr < MAX_SOCKET; Ctr++) {
      if ((KtiInternalGlobal->CpuList & (1 << Ctr)) == 0) {
        *NeighborSoc2 = Ctr;
        break;
      }
    }

    for (Ctr++; Ctr < MAX_SOCKET; Ctr++) {
      if ((KtiInternalGlobal->CpuList & (1 << Ctr)) == 0) {
        *RemoteSoc = Ctr;
        break;
      }
    }

    return;
  } else {
    //
    // Must be a 3S or  4S system
    //
    if (SocketData->Cpu[Soc].LinkData[0].Valid == TRUE && SocketData->Cpu[Soc].LinkData[1].Valid == TRUE) {
      *NeighborSoc1 = (UINT8) SocketData->Cpu[Soc].LinkData[0].PeerSocId;
      *NeighborSoc2 = (UINT8) SocketData->Cpu[Soc].LinkData[1].PeerSocId;
      TempList      = (KtiInternalGlobal->CpuList) & ((1 << Soc) | (1 << *NeighborSoc1) | (1 << *NeighborSoc2));
      for (Ctr = 0; Ctr < KtiInternalGlobal->TotCpu; Ctr++) {
        if ((TempList & (1 << Ctr)) == 0) {
          //
          // Socket that is missing in the list must be the Remote Socket
          //
          *RemoteSoc = Ctr;
          break;
        }
      }
    } else {
      if (SocketData->Cpu[Soc].LinkData[0].Valid == TRUE) {
        //
        // This must be serially connected topology with the given socket at the begining or at the end
        //
        *NeighborSoc1 = (UINT8) SocketData->Cpu[Soc].LinkData[0].PeerSocId;
      } else {
        *NeighborSoc1 = (UINT8) SocketData->Cpu[Soc].LinkData[1].PeerSocId;
      }

      if (SocketData->Cpu[*NeighborSoc1].LinkData[0].PeerSocId == Soc) {
        //
        // NeighborSoc1 must have both the links valid
        //
        *NeighborSoc2 = (UINT8) SocketData->Cpu[*NeighborSoc1].LinkData[1].PeerSocId;
      } else {
        *NeighborSoc2 = (UINT8) SocketData->Cpu[*NeighborSoc1].LinkData[0].PeerSocId;
      }

      if (KtiInternalGlobal->TotCpu == 3) {
        TempList = (KtiInternalGlobal->CpuList) & ((1 << Soc) | (1 << *NeighborSoc1) | (1 << *NeighborSoc2));
        for (Ctr = 0; Ctr < KtiInternalGlobal->TotCpu; Ctr++) {
          if ((TempList & (1 << Ctr)) == 0) {
            //
            // Socket that is missing in the list must be the Remote Socket
            //
            *RemoteSoc = Ctr;
            break;
          }
        }
      } else {
        if (SocketData->Cpu[*NeighborSoc2].LinkData[0].PeerSocId == *NeighborSoc1) {
          //
          // NeighborSoc2 must have both the links valid
          //
          *RemoteSoc = (UINT8) SocketData->Cpu[*NeighborSoc2].LinkData[1].PeerSocId;
        } else {
          *RemoteSoc = (UINT8) SocketData->Cpu[*NeighborSoc2].LinkData[0].PeerSocId;
        }
      }
    }
  }
  return;
}

/**
  Routine to program D2C register

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket Id

  @retval None

**/
VOID
ProgramD2C (
  IN KTI_SOCKET_DATA          *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    SocId
  )
{
  UINT8                        Port;
  //
  // KTIAgent
  //
  for (Port = 0; Port < KtiInternalGlobal->KtiPortCnt; Port++) {
    if (SocketData->Cpu[SocId].LinkData[Port].Valid == TRUE) {
      ProgramD2CRegister (SocId, Port, KtiInternalGlobal->D2cEn);
    }
  }
}

/**
  Routine to program the final IOPORT SAD for the given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuIoEntriesKti (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  UINT32  IoBaseTmp;
  UINT16  IoLimit;
  UINT32  IoLocalLimits[2] = {0,0};
  UINT32  IoLocalTgtLst = 0;
  UINT8   TargetIndex = 0, LinkIndex, Stack;
  UINT8   TargetBitsWidth;
  UINT8   CpuType;

  LinkIndex = 0;
  CpuType     = KtiInternalGlobal->CpuType;

  if (IsSprFamilyCpu (CpuType)) {
    TargetBitsWidth = 4;
  } else {
    TargetBitsWidth = 3;
  }

  //
  // Setup starting base of legacy IO for the socket
  //
  IoLocalLimits[0] = (UINT32) (KTIVAR->CpuInfo[SocId].CpuRes.IoBase >> 10) & 0x3f;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    //
    // Program a target and limit for each enabled stack
    //
    if (StackPresent (SocId, Stack)) {
      IoBaseTmp = KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Stack].IoBase;
      if (IoBaseTmp != 0xFFFF) {
        IoLocalTgtLst |= Stack << (TargetIndex * TargetBitsWidth);
        IoLimit = (KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Stack].IoLimit >> 10) & 0x3f;
        KTI_ASSERT ((((TargetIndex / 3) < (sizeof (IoLocalLimits) / sizeof (IoLocalLimits[0])))), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_84);
        if (TargetIndex < 3) {
          IoLocalLimits[TargetIndex / 3] |= (IoLimit << ((TargetIndex + 1) * 8)) ;
        } else {
          IoLocalLimits[TargetIndex / 3] |= (IoLimit << ((TargetIndex - 3) * 8));
        }
        TargetIndex++;
      }
    }
  }

  for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
    if (SocketData->Cpu[SocId].LinkData[LinkIndex].Valid == TRUE) {
      ProgramKtiIoTad (SocId, LinkIndex, IoLocalTgtLst, IoLocalLimits);
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to program the final IoApic SAD for given socket

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramCpuIoApicEntriesKti (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  UINT32  IoApicBaseTmp;
  UINT32  IoApicLimit;
  UINT32  IoApicLocalLimits[2] = {0,0};
  UINT32  IoApicLocalTgtLst = 0;
  UINT8   TargetIndex = 0, Stack;
  UINT8   LinkIndex;
  UINT8   TargetBitsWidth;

  if (IsSprFamilyCpu (KtiInternalGlobal->CpuType)) {
    TargetBitsWidth = 4;
  } else {
    TargetBitsWidth = 3;
  }
  //
  // Setup starting base of legacy IO for the socket
  //
  IoApicLocalLimits[0] = (UINT32) (KTIVAR->CpuInfo[SocId].CpuRes.IoApicBase >> 15) & 0x1f;

  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    //
    // Program a target and limit for each enabled stack
    //
    if (StackPresent (SocId, Stack)) {
      IoApicBaseTmp = KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Stack].IoApicBase;
      if (IoApicBaseTmp != 0xFFFFFFFF) {
        IoApicLocalTgtLst |= Stack << (TargetIndex * TargetBitsWidth);
        IoApicLimit = (KTIVAR->CpuInfo[SocId].CpuRes.StackRes[Stack].IoApicLimit >> 15) & 0x1f;

        KTI_ASSERT ((((TargetIndex / 3) < (sizeof (IoApicLocalLimits) / sizeof (IoApicLocalLimits[0])))), ERR_SW_CHECK, MINOR_ERR_SW_CHECK_85);
        if (TargetIndex < 3) {
          IoApicLocalLimits[TargetIndex / 3] |= (IoApicLimit << ((TargetIndex + 1) * 8)) ;
        } else {
          IoApicLocalLimits[TargetIndex / 3] |= (IoApicLimit << ((TargetIndex - 3) * 8));
        }
        TargetIndex++;
      }
    }
  }

  for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
    if (SocketData->Cpu[SocId].LinkData[LinkIndex].Valid == TRUE) {
      ProgramKtiIoApicTad (SocId, LinkIndex, IoApicLocalTgtLst, IoApicLocalLimits);
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to program MISC SAD register in KTI Agent

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
KTI_STATUS
ProgramKtiMiscSadEntries (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocId
  )
{
  UINT8                            LinkIndex;
  UINT32                           StackBitmap;

  StackBitmap = GetStackPresentBitMap (SocId);

  for (LinkIndex = 0; LinkIndex < KtiInternalGlobal->KtiPortCnt; LinkIndex++) {
    if (SocketData->Cpu[SocId].LinkData[LinkIndex].Valid == TRUE) {
      //
      // Program the StackPresentBitmap to the Kti port's tad iio_stackid_enable
      //
      ProgramKtiIioStackIdEnable (SocId, LinkIndex, StackBitmap);
      if (SocId == KTIVAR->OutLegacyVgaSoc) {
        ProgramKtiVgaSadTargets (SocId, LinkIndex, 1, KTIVAR->OutLegacyVgaStack);
      } else {
        ProgramKtiVgaSadTargets (SocId, LinkIndex, 0, 0);
      }
    }
  }

  return KTI_SUCCESS;
}

/**
  Routine to collect and clear M3KTI errors

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data
  @param SocId             - Socket to program

  @retval 0     - Successful
  @retval Other - failure

**/
VOID
CollectAndClearM3KtiErrors (
  IN KTI_SOCKET_DATA          *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    Soc
  )
{
  UINT8  Ctr;
  UINT8  CpuType;
  UINT8  M3KtiCtr, ProgrammedM3Kti;

  CpuType     = KtiInternalGlobal->CpuType;
  ProgrammedM3Kti = M3KtiCtr = 0xff;

  for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
    if ((KtiInternalGlobal->KtiPortPresentBitmap[Soc] & (1 << Ctr)) != 0) {
      if (IsIcxFamilyCpu (CpuType) || IsSprFamilyCpu (CpuType)) {
        ClearM3KtiErrors (Soc, Ctr, &(KTIVAR->CpuInfo[Soc].CpuErrLog.M3Kti));
      } else {
        //
        // SKX + CPX, KTI port 0 & 1 share 1 M3KTI0, KTI port 2 owns M3KTI1
        //
        M3KtiCtr = PORT_TO_M3KTI (Ctr);
        if (ProgrammedM3Kti != M3KtiCtr) {
          ClearM3KtiErrors (Soc, M3KtiCtr, &(KTIVAR->CpuInfo[Soc].CpuErrLog.M3Kti));
          ProgrammedM3Kti = M3KtiCtr;
        }
      }
    }
  }
}

/**
   Returns TRUE if the CPU was not already released to fetch code

   @param SocketData        - Pointer to socket specific data
   @param KtiInternalGlobal - KTIRC internal variables.
   @param SocId             - CPU Socket Number, 0 based

   @retval TRUE  - CPU requires boot_go
   @retval FALSE - CPU does not need boot_go

**/
BOOLEAN
SocketRequireBootGo (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  UINT8                      SocId
  )
{
  BOOLEAN   RequireBootGo = FALSE;

  if (SocketData->Cpu[SocId].SocType == SOCKET_TYPE_CPU) {
    UBoxCheckSocketRequireBootGo (SocId, &RequireBootGo);
  }

  return RequireBootGo;
}
