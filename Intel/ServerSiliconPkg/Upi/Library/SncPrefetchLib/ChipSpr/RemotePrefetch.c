/** @file
  File includes routines to configure remote XPT prefetch

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

#include <RcRegs.h>
#include <Library/MemoryCoreLib.h>
#include <Library/UncoreLib.h>
#include <Library/MultiSocketLib.h>
#include <KtiMisc.h>
#include <Chip/Include/EvAutoRecipe.h>
#include <UncoreCommonIncludes.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/MemoryMapData.h>
#include <Library/UpiHostDataLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/SimRegsLookupLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/KtiApi.h>
#include <KtiSetupDefinitions.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/ChaLib.h>
#include <Guid/UboxIpInterface.h>
#include <Library/Ms2IdiLib.h>
#include <Library/PcuIpLib.h>
#include <Guid/KtiIpInterface.h>
#include <Guid/FpgaIpInterface.h>
#include <Library/CsrAccessLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/PipeSyncLib.h>
#include <Library/SncPrefetchLib.h>
#include "SncPrefetchLibInternal.h"


/**
  This routine is to disable xpt remote prefetch feature

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param Socket            - Socket Id

  @retval None

**/
VOID
DisableRemoteXpt (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Socket
  )
{
  UINT8      Ctr;

  Ms2IdiSetXptRmtPref (Socket, 0x1);

  for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
    if (KTIVAR->CpuInfo[Socket].LepInfo[Ctr].Valid == TRUE) {
      Ms2IdiClearXptUpiDecTab (Socket, Ctr);
    }
  }

  return;
}

/**
  This routine is to get remote socket's memory limit

  @param  Socket           - Socket Id
  @param  MemInfo          - Memory Information structure.
  @param  Base             - Remote Socket's base
  @param  Limit            - Remote Socket's limit

  @retval None

**/
VOID
GetRemoteSocketMemorySize (
  IN UINT8                  Socket,
  IN MEM_INFO               *MemInfo,
  OUT UINT32                *Base,
  OUT UINT32                *Limit
  )
{
  UINT8                     Cluster;
  UINT32                    ClusterLimit;

  *Limit = 0;
  //To search the memory limit of remote socket
  for (Cluster = 0; Cluster <= MAX_CLUSTERS; Cluster++) {
    ClusterLimit = MemInfo->ClusterMemBase[Socket][Cluster];
    if (ClusterLimit > *Limit) {
      *Limit = ClusterLimit;
    }
  } // Cluster loop

  *Base = MemInfo->ClusterMemBase[Socket][0];
}

/**
  This routine is to get remote socket's memory base/limit
  It's only used for UPI Affinity - SNC2/SNC4.

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param MemInfo           - Memory Information structure.
  @param Socket            - Socket Id
  @param LocalClusterId    - Local Cluster Id
  @param Base              - Remote Socket's base
  @param Limit             - Remote Socket's limit

  @retval None

**/
VOID
GetRmtSktMemInfoForUpiAffinity (
  IN  KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN  MEM_INFO                 *MemInfo,
  IN  UINT8                    Socket,
  IN  UINT8                    LocalClusterId,
  OUT UINT32                   *Base,
  OUT UINT32                   *Limit
  )
{
  UINT8           Snc4TwistedIndex[] = {2, 1, 0, 3};
  UINT8           Snc2TwistedIndex[] = {1, 0};
  UINT8           RemoteClusterId = 0;

  if (KTIVAR->TwistedKtiLinks) {
    if (KTIVAR->OutSncEn == 4) {
      RemoteClusterId = Snc4TwistedIndex[LocalClusterId];
    } else {
      RemoteClusterId = Snc2TwistedIndex[LocalClusterId];
    }
  } else {
    RemoteClusterId = LocalClusterId;
  }

  *Base  = MemInfo->ClusterRngInLocal[Socket][RemoteClusterId].MemBase >> 4;
  *Limit = MemInfo->ClusterRngInLocal[Socket][RemoteClusterId].MemLimit >> 4;
}

/**
  This routine is to enable xpt remote prefetch feature

  @param KtiInternalGlobal - KTIRC Internal Global data
  @param  Socket           - Socket Id
  @param  MemInfo          - Memory Information structure.

  @retval None

**/
VOID
ProgramRemoteXpt (
  IN KTI_HOST_INTERNAL_GLOBAL *KtiInternalGlobal,
  IN UINT8                    Socket,
  IN MEM_INFO                 *MemInfo
  )
{
  UINT8   RemoteSktId;
  UINT8   Ctr;
  UINT8   OutPort[4];
  UINT8   UpiId;
  UINT8   CurCount;
  UINT8   XptMiniSadModeSelect;
  UINT32  Base;
  UINT32  Limit;
  UINT8   SncId2UpiId_Snc4Upi4 [] = {0, 2, 1, 3};
  UINT8   SncId2UpiId_Snc4Upi2 [] = {0, 0, 1, 1};
  UINT8   SncId2UpiId_Snc2Upi4 [] = {0, 1};
  UINT8   SncId2UpiId_Snc2Upi2 [] = {0, 1};

  if (KTIVAR->SysConfig == SYS_CONFIG_1S) {
    return;
  }

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "\nSocket %d Remote XPT Enabled\n", Socket));

  // Enable xpt remote prefetch
  Ms2IdiSetXptRmtPref (Socket, 0x0);

  //
  // Program minsad mode and take care of 2S 2link or 2S 3links cases
  //
  XptMiniSadModeSelect = MemInfo->MiniSadInfo.Xptminisadmodeselect;

  //Initialize variables
  for (Ctr = 0; Ctr < sizeof (OutPort) / sizeof (OutPort[0]); Ctr++) {
    OutPort[Ctr] = UPI_PORT_INVALID;
  }

  if (KTIVAR->SysConfig == SYS_CONFIG_2S2L ||
      KTIVAR->SysConfig == SYS_CONFIG_2S3L ||
      KTIVAR->SysConfig == SYS_CONFIG_2S4L) { //2S 2link OR 2S 3link or 2S4L
    RemoteSktId = (UINT8)KTIVAR->CpuInfo[Socket].LepInfo[0].PeerSocId; //use 0 because 2S2L or 2L3S topology always has port 0
    if (((KTIVAR->OutSncEn == 2) || (KTIVAR->OutSncEn == 4)) &&
         KTIVAR->UpiAffinityEn) {
      for (Ctr = 0; Ctr < KTIVAR->OutSncEn; Ctr++) {
        GetRmtSktMemInfoForUpiAffinity (
          KtiInternalGlobal,
          MemInfo,
          RemoteSktId,
          Ctr,
          &Base,
          &Limit
        );
        //SNC cluster -> UPI link
        if (KTIVAR->SysConfig == SYS_CONFIG_2S4L) {
          if (KTIVAR->OutSncEn == 4) {
            //SNC4 4UPI
            UpiId = SncId2UpiId_Snc4Upi4[Ctr];
          } else {
            //SNC2 4UPI
            UpiId = SncId2UpiId_Snc2Upi4[Ctr];
          }
        } else {
          if (KTIVAR->OutSncEn == 4) {
            //SNC4 2UPI
            UpiId = SncId2UpiId_Snc4Upi2[Ctr];
          } else {
            //SNC2 2UPI
            UpiId = SncId2UpiId_Snc2Upi2[Ctr];
          }
        }
        Ms2IdiProgramXptUpiDecodeTable (Socket, Ctr, RemoteSktId, UpiId, Base, Limit);
      }
    } else {
      GetRemoteSocketMemorySize (RemoteSktId, MemInfo, &Base, &Limit);   //Get remote socket's memory limit
      OutPort[0] = (UINT8)KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].LepInfo[0].PeerPort; //get remote socket's port # from sbsp port 0

      if (KTIVAR->SysConfig == SYS_CONFIG_2S2L) { //2S2L
        if ((UINT8)KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].LepInfo[1].PeerPort != 0xFF) { //get remote socket's port # from sbsp port 1 or 2
          OutPort[1] = (UINT8)KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].LepInfo[1].PeerPort;
        } else {
          OutPort[1] = (UINT8)KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].LepInfo[2].PeerPort;
        }
      } else if (KTIVAR->SysConfig == SYS_CONFIG_2S3L) { // 2S3L
        OutPort[1] = (UINT8)KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].LepInfo[1].PeerPort; //get remote socket's port # from sbsp port 1
        OutPort[2] = (UINT8)KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].LepInfo[2].PeerPort; //get remote socket's port # from sbsp port 2
      } else if (KTIVAR->SysConfig == SYS_CONFIG_2S4L) { // 2S4L
        OutPort[1] = (UINT8)KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].LepInfo[1].PeerPort; //get remote socket's port # from sbsp port 1
        OutPort[2] = (UINT8)KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].LepInfo[2].PeerPort; //get remote socket's port # from sbsp port 2
        OutPort[3] = (UINT8)KTIVAR->CpuInfo[KtiInternalGlobal->SbspSoc].LepInfo[3].PeerPort; //get remote socket's port # from sbsp port 3
      }

      //
      // Program remote socket memory base and limit, remote skt id, upi_id for each UPI link
      //

      // Scan all Kti link if valid, and find the memory range of remote socket
      CurCount = 0;
      for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
        if ((KTIVAR->CpuInfo[Socket].LepInfo[Ctr].Valid == TRUE) && (KTIVAR->CpuInfo[Socket].LepInfo[Ctr].PeerSocType == SOCKET_TYPE_CPU)) {
          if (CurCount == 0) {
            if (Socket == KtiInternalGlobal->SbspSoc) {
              UpiId = Ctr;
            } else {
              UpiId = OutPort[0]; // get which port connected to sbsp port 0
            }

            Ms2IdiProgramXptUpiDecodeTable (Socket, 0, RemoteSktId, UpiId, Base, Limit); //program socket decode table 0 with remote socket/limit/UpiId
          } else if (CurCount == 1) {
            if (Socket == KtiInternalGlobal->SbspSoc) {
              UpiId = Ctr;
            } else {
              UpiId = OutPort[1]; // get which port connected to sbsp port 1
            }
            //only this field upi link need to be updated
            //just use table_1 for upi_id
            Ms2IdiSetDecUpiId (Socket, UpiId, 1);
          } else if (CurCount == 2) {
            if (Socket == KtiInternalGlobal->SbspSoc) {
              UpiId = Ctr;
            } else {
              UpiId = OutPort[2];                  // get which port connected to sbsp port 2
            }
            //only this field upi link need to be updated
            //just use table_2 for upi_id
            Ms2IdiSetDecUpiId (Socket, UpiId, 2);
          } else if (CurCount == 3) {
            if (Socket == KtiInternalGlobal->SbspSoc) {
              UpiId = Ctr;
            } else {
              UpiId = OutPort[3];                  // get which port connected to sbsp port 3
            }
            //only this field upi link need to be updated
            //just use table_3 for upi_id
            Ms2IdiSetDecUpiId (Socket, UpiId, 3);
          }
          CurCount++;
        }
      }
    }
  } else { // other than 2S2L OR 2S3L OR 2S4L
    //
    // Program remote socket memory base and limit, remote skt id, upi_id for each UPI link
    //

    // Scan all Kti link if valid, and find the memory range of remote socket
    CurCount = 0;
    for (Ctr = 0; Ctr < KtiInternalGlobal->KtiPortCnt; Ctr++) {
      if (KTIVAR->CpuInfo[Socket].LepInfo[Ctr].Valid == TRUE) {
        RemoteSktId = (UINT8) KTIVAR->CpuInfo[Socket].LepInfo[Ctr].PeerSocId;
        GetRemoteSocketMemorySize (RemoteSktId, MemInfo, &Base, &Limit);                  //Get remote socket's memory limit
        Ms2IdiProgramXptUpiDecodeTable (Socket, CurCount, RemoteSktId, Ctr, Base, Limit); //program socket decode table "CurCount" with remote socket/limit/UpiId
        CurCount++;
      }
    }
  }

  Ms2IdiXptRmtPrefCfg (Socket, KTIVAR->SysConfig, XptMiniSadModeSelect);

  return;
}