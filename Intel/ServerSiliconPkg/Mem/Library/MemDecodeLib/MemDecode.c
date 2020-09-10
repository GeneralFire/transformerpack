/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <MemDecode.h>
#include <MemDecodeRas.h>
#include <KtiSetupDefinitions.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Cpu/CpuCoreRegs.h>
#include <SysHostChipCommon.h>
#include <Library/MemMcIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemRcLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/KtiApi.h>
#include <Library/HobLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemTypeLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemMapDataLib.h>
#include <Include/MemMapHost.h>
#include <Library/RcMemAllocLib.h>
#include <Library/AdvMemTestLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Guid/MrcPlatformHooksGuid.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/NumaAcpiTableLib.h>


// Function definitions

/**

  This routine clears Host structures used for memory mapping so that memory mapping algorithm can be run again afresh.

  @param[in] Host       - Pointer to sysHost
  @param[in] MemMapData - Pointer to the memory map private data structure

  @retval VOID

**/
VOID
ClearMemMapStructs (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  MEMORY_MAP_HOST                    *MemMapHost;
  UINT8                              SadIndex;
  UINT8                              MaxSadIndex;
  UINT8                              Sckt;
  UINT8                              McIndex;
  UINT8                              Index;
  UINT8                              TadIndex;
  UINT8                              MaxImc;
#ifdef DDRT_SUPPORT
  UINT8                              ChIndex;
  UINT8                              DimmIndex;
  DIMM_NVRAM_STRUCT                  (*DimmNvList)[MAX_DIMM];
  DDRT_DIMM_DEVICE_INFO_STRUCT       *DdrtDimmInfo;
#endif // DDRT_SUPPORT
  UINT8                              MaxChPerMc;
  MEM_VAR_STRUCT                     *MemVar;
  SOCKET_INFO_STRUCT                 *Socket;
  IMC_INFO_STRUCT                    *Imc;
  CHANNEL_NVRAM_STRUCT               (*ChannelNvList)[MAX_CH];
  SAD_TABLE                          *SadEntry;
  RSAD_TABLE                         *RemoteSadEntry;
  TAD_TABLE                          *TadEntry;
  UINT8                              MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  MemMapHost = GetMemMapHostPointer ();
  MemVar = &Host->var.mem;
  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();

  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    MemMapData->MemMapSocket[Sckt].AppDirectByOneChannelBitmap = 0;
    ZeroMem (MemMapData->MemMapSocket[Sckt].Ddr1lmX1IntlvCount,
             sizeof (MemMapData->MemMapSocket[Sckt].Ddr1lmX1IntlvCount));

    ZeroMem (MemMapData->MemMapSocket[Sckt].NvMemSadIdxBitMap,
             sizeof (MemMapData->MemMapSocket[Sckt].NvMemSadIdxBitMap));

    Socket = &MemVar->socket[Sckt];
    ChannelNvList = GetChannelNvList (Host, Sckt);

    //
    // Clear all the SAD entries from the internal structs
    //
    MaxSadIndex = (MemMapData->NumberofChaDramClusters * MemMapData->MaxSadRules);
    for (SadIndex = 0; SadIndex < MaxSadIndex; SadIndex++) {
      SadEntry = &Socket->SAD[SadIndex];
      ZeroMem (SadEntry, sizeof (SAD_TABLE));
      //
      // Clear the SAD Interleave list
      //
      for (Index = 0; Index < MC_MAX_NODE; Index++) {
        MemMapHost->SocketInfo[Sckt].SadIntList[SadIndex][Index] = 0;
      }
    }

    //
    // Clear all the remote SAD entries from the internal structs
    //
    for (SadIndex = 0; SadIndex < MemMapData->MaxRemoteSadRules; SadIndex++) {
      RemoteSadEntry = &MemMapData->MemMapSocket[Sckt].RSad[SadIndex];
      ZeroMem (RemoteSadEntry, sizeof (RSAD_TABLE));
    }

    //
    // Clear all the TAD entries from the internal structs
    //
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      Imc = &Socket->imc[McIndex];
      for (TadIndex = 0; TadIndex < MAX_TAD_RULES; TadIndex++) {
        TadEntry = &Socket->imc[McIndex].TAD[TadIndex];
        ZeroMem (TadEntry, sizeof (TAD_TABLE));
        //
        // Parse through each channel in the IMC and clear the TAD offset and Intlist for this TAD
        //
        for (Index = 0; Index < MaxChPerMc; Index++) {
          Imc->TADOffset[TadIndex][Index] = 0;
          Imc->TADChnIndex[TadIndex][Index] = 0;
          Imc->TADintList[TadIndex][Index] = 0;
        }
      } // TadIndex
    } // McIndex

#ifdef DDRT_SUPPORT
    //
    // For each channel
    //
    for (ChIndex = 0; ChIndex < MaxChDdr; ChIndex++) {
      DimmNvList = GetDimmNvList (Host, Sckt, ChIndex);

      // For each DIMM
      for (DimmIndex = 0; DimmIndex < MAX_DIMM; DimmIndex++) {
        if ((*DimmNvList)[DimmIndex].dimmPresent == 0) {
          continue;
        }

        //Skip if this is not a NVMDIMM dimm
        if ((*DimmNvList)[DimmIndex].DcpmmPresent == 0) {
          continue;
        }

        DdrtDimmInfo = GetDdrtDimmInfo (Sckt, ChIndex, DimmIndex);

        //Skip if this dimm is new to the system
        if (DdrtDimmInfo->NewDimm == 1) {
          continue;
        }

        for (Index = 0; Index < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; Index++) {
          //clear processed flag and the sadlimit variable if we have a valid intreleave request present in CFGIN
          if (DdrtDimmInfo->NgnCfgReq.interleave[Index].RecPresent  && DdrtDimmInfo->NgnCfgReq.interleave[Index].Valid) {
              DdrtDimmInfo->NgnCfgReq.interleave[Index].Processed = 0;
              DdrtDimmInfo->NgnCfgReq.interleave[Index].SadLimit = 0;
          }
          //clear processed flag and the sadlimit variable if we have a valid intreleave resord  present in CCUR
          if (DdrtDimmInfo->NgnCfgCur.interleave[Index].RecPresent  && DdrtDimmInfo->NgnCfgCur.interleave[Index].Valid) {
              DdrtDimmInfo->NgnCfgCur.interleave[Index].Processed = 0;
              DdrtDimmInfo->NgnCfgCur.interleave[Index].SadLimit = 0;
          }
        } // Index
      } // DimmIndex
     } // ChIndex
#endif // DDRT_SUPPORT
   }// Sckt

  //
  // Reset configuration status before each run of the interleaving flows.
  //
  MemMapData->IsPorConfig = FALSE;


  //
  // Clear field that holds the SAD adjustment size for GB alignment clipping in Mod3 1LM.
  //
  MemMapData->SadLimitAdjustment = 0;
  MemMapData->TadLimitAdjustment = 0;
  MemMapData->Below4GBSktIndex = 0;
}

/**
  Map out DIMMs and enable only single DDR DIMM on socket.

  @param[in] Host       - Pointer to sysHost
  @param[in] MemMapData - Pointer to MemMapData
  @param[in] Socket     - Socket index for the socket to mapout DIMMs

**/
VOID
MapoutToSingleDdr (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Socket
  )
{
  UINT8                 Channel;
  UINT8                 Dimm;
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT     (*DimmNvList)[MAX_DIMM];
  BOOLEAN               DimmFound;
  UINT8                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  //
  // Reset MemMapState so mem size fields will be repopulated
  //
  MemMapData->MemMapState = MEM_MAP_STATE_RESOURCE_CALCULATION;

  DimmFound = FALSE;

  //
  // Only leave one DDR DIMM on the socket
  //
  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if ((*ChannelNvList)[Channel].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    for (Dimm = 0; Dimm < (*ChannelNvList)[Channel].maxDimm; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if ((DimmFound == FALSE) && ((*DimmNvList)[Dimm].DcpmmPresent == 0)) {
        //
        // Keep this DDR
        //
        DimmFound = TRUE;
      } else {
        //
        // Disable all other DDR and mapout all DCPMMs
        //
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Mapping out DDR\n");
          MapOutDimm (Host, Socket, Channel, Dimm, DIMM_RANK_MAP_OUT_MEM_DECODE);
        } else {
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Mapping out PMem\n");
          Host->var.mem.socket[Socket].channelList[Channel].DdrtDimmList[0].NvdimmUnmapped = 1;
          Host->var.mem.socket[Socket].channelList[Channel].DdrtDimmList[0].NvdimmUnmappedReason = NVDIMM_UNMAPPED_SKU_LIMIT_VIOLATION;
        }
      }
    } // Dimm
  } // Channel
}

/**
  Degrade the memory mapping level and rerun the memory mapping algorithm.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to MemMapData

**/
VOID
DegradeMemoryMapLevel (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8                 Socket;

  //
  // Memory degrading is only possible for PMEM managed by the CR driver
  //
  if (Host->var.mem.dfxMemVars.dfxDimmManagement == DIMM_MGMT_CR_MGMT_DRIVER) {
    MemMapData->MemMapDegradeLevel += 1;

    //
    // If we are here it means, that we have completed initial run of memmap and found that we DONOT have all silicon resources
    // So reset the resourceCalcDone variable and rerun the memory mapping algorithm.
    // Host->var.mem.ResourceCalculationDone = 0;
    //
    MemMapData->MemMapState = MEM_MAP_STATE_RESOURCE_CALCULATION_FAILURE;
  }

  //
  // Clear Memory map structures.
  //
   ClearMemMapStructs (Host, MemMapData);

  //
  // Map out DIMMs on any socket where there is either a SKU Limit violation or insufficient resources in 1LM.
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (Host->nvram.mem.socket[Socket].enabled == 0) {
      continue;
    }
    if (MemMapData->MemMapSkuLimit[Socket].Violation == TRUE) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "Mapping out DIMMs due to SKU Limit violation.\n");
      MapoutToSingleDdr (Host, MemMapData, Socket);
    } else if (Host->var.mem.volMemMode == VOL_MEM_MODE_1LM) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "Mapping out DIMMs due to system already in 1LM only volatile memory mode.\n");
      MapoutToSingleDdr (Host, MemMapData, Socket);
    }
  } // Socket

  //
  // Degrade system to 1LM only when there are insufficient resources to map memory and system is not in 1LM only mode.
  //
  if (Host->var.mem.volMemMode != VOL_MEM_MODE_1LM) {
    Host->var.mem.volMemMode = VOL_MEM_MODE_1LM;
    //
    // Reset MemMapState so mem size fields will be repopulated
    //
    MemMapData->MemMapState = MEM_MAP_STATE_RESOURCE_CALCULATION;
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Insufficient resources to map memory and degrading memory to 1LM.\n");
  }
}

/**
  Displays the SADTable data structure in the Host structure.

  @param[in] Host             Pointer to sysHost.
  @param[in] MemMapData       Pointer to the memory map private data structure.

  @retval N/A
**/
VOID
DisplaySadTable (
  IN PSYSHOST           Host,
  IN MEMORY_MAP_DATA    *MemMapData
  )
{
  MEMORY_MAP_HOST *MemMapHost;
  UINT8           SocketNum;
  UINT8           SadIndex;
  UINT8           ClusterId;
  UINT8           NodeIndex;
  UINT8           McIndex;
  UINT8           MaxImc;
  UINT8           SadSortedIndex[MAX_DRAM_CLUSTERS * MAX_SAD_RULES];
  UINT8           SadCmpIndexPerCluster[MAX_DRAM_CLUSTERS];
  UINT8           MinSadClusterIndex;
  UINT32          MinSadBaseAddr;
  SAD_TABLE       *SadEntry;

  MaxImc = GetMaxImc ();
  MemMapHost = GetMemMapHostPointer ();
  //
  // Print SAD table for each socket
  //
  for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {
    //
    // Skip empty SAD tables
    //
    if (Host->var.mem.socket[SocketNum].SAD[0].Enable == 0) {
      continue;
    }

    RcDebugPrint (SDBG_MAX, "\n\n********SAD table for socket %d*******\n", SocketNum);
    RcDebugPrint (SDBG_MAX, "Type          Base     Limit    Ways  McIntBitmap" );
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      RcDebugPrint (SDBG_MAX, "  ChIntBitmap[MC%d]  FmChIntBitmap[MC%d]", McIndex, McIndex);
    }
    RcDebugPrint (SDBG_MAX, "  Granularity  Tgtgranularity  Attr  Cluster\n");

    //
    // A multi-way merge sort algorithm is used to sort the enabled SAD entries by base address based on
    // the assumption that the enabled SAD entries have been sorted for each cluster.
    //
    SadIndex = 0;
    SetMem (&SadSortedIndex, sizeof (SadSortedIndex), (UINT8) -1);
    ZeroMem (SadCmpIndexPerCluster, sizeof (SadCmpIndexPerCluster));

    while (SadIndex < (MemMapData->NumberofChaDramClusters * MemMapData->MaxSadRules)) {
      MinSadBaseAddr = (UINT32) -1;
      MinSadClusterIndex = (UINT8) -1;
      for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
        if (SadCmpIndexPerCluster[ClusterId] >= MemMapData->MaxSadRules) {
          continue;
        }

        SadEntry = &Host->var.mem.socket[SocketNum].SAD[SadCmpIndexPerCluster[ClusterId] + ClusterId * MemMapData->MaxSadRules];
        if ((SadEntry->Enable != 0) && (SadEntry->Base < MinSadBaseAddr)) {
          MinSadBaseAddr = SadEntry->Base;
          MinSadClusterIndex = ClusterId;
        }
      }

      if ((MinSadBaseAddr == (UINT32) -1) || (MinSadClusterIndex >= MemMapData->NumberofChaDramClusters)) {
        break;  // Break if all the enabled SAD entries are checked.
      }

      SadSortedIndex[SadIndex] = SadCmpIndexPerCluster[MinSadClusterIndex] + MinSadClusterIndex * MemMapData->MaxSadRules;
      SadCmpIndexPerCluster[MinSadClusterIndex]++;
      SadIndex++;
    }

    //
    // Print SAD table information according to the sorted index.
    //
    for (SadIndex = 0; SadIndex < (MemMapData->NumberofChaDramClusters * MemMapData->MaxSadRules); SadIndex++) {
      if (SadSortedIndex[SadIndex] >= MemMapData->NumberofChaDramClusters * MemMapData->MaxSadRules) {
        break;
      }
      SadEntry = &Host->var.mem.socket[SocketNum].SAD[SadSortedIndex[SadIndex]];
      if (SadEntry->Enable == 0) {
        break;
      }
      if (SadEntry->local || (SadEntry->type == MemTypeNxm)) {
        RcDebugPrint (SDBG_MAX, "%-12a  0x%05x  0x%05x  %4d  %11x",
          GetMemTypeString (SadEntry->type),
          SadEntry->Base,
          SadEntry->Limit,
          SadEntry->ways,
          SadEntry->imcInterBitmap
          );
        for (McIndex = 0; McIndex < MaxImc; McIndex++) {
          RcDebugPrint (SDBG_MAX, "  %16x  %18x",
            SadEntry->channelInterBitmap[McIndex],
            SadEntry->FMchannelInterBitmap[McIndex]
            );
        }
        RcDebugPrint (SDBG_MAX, "  %11d  %14d  %4d  %7d\n",
          SadEntry->granularity,
          SadEntry->tgtGranularity,
          SadEntry->Attr,
          SadEntry->Cluster
          );
      } else {
        RcDebugPrint (SDBG_MAX, "REMOTE        0x%05x  0x%05x   N/A          N/A",
          SadEntry->Base,
          SadEntry->Limit
          );

        for (McIndex = 0; McIndex < MaxImc; McIndex++) {
          RcDebugPrint(SDBG_MAX, "  %16a  %18a",
            "N/A",
            "N/A"
            );
        }
        RcDebugPrint (SDBG_MAX, "  %11d  %14d  %4d  %7d\n",
          SadEntry->granularity,
          SadEntry->tgtGranularity,
          SadEntry->Attr,
          SadEntry->Cluster
          );
      }
    }

    RcDebugPrint (SDBG_MAX, "<<SAD Interleave List>>");
    for (SadIndex = 0; SadIndex < (MemMapData->NumberofChaDramClusters * MemMapData->MaxSadRules); SadIndex++) {
      if ((SadSortedIndex[SadIndex] >= MemMapData->NumberofChaDramClusters * MemMapData->MaxSadRules) ||
          (Host->var.mem.socket[SocketNum].SAD[SadSortedIndex[SadIndex]].Enable == 0)) {
        break;
      }
      RcDebugPrint (SDBG_MAX, "\n");
      for (NodeIndex = 0; NodeIndex < MC_MAX_NODE; NodeIndex++) {
        RcDebugPrint (SDBG_MAX, "%d\t",
          MemMapHost->SocketInfo[SocketNum].SadIntList[SadSortedIndex[SadIndex]][NodeIndex]
          );
      }
    }
  } // SocketNum
  RcDebugPrint (SDBG_MAX, "\n</SADTable>\n");
}

/**

Displays the TADTable data structure in the Host structure

@param Host       - Pointer to sysHost
@param MemMapData - Pointer to the memory map private data structure

**/
VOID
DisplayTadTable (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  UINT8             McIndex;
  UINT8             TadIndex;
  UINT8             Sckt;
  UINT8             MaxImc;
  IMC_INFO_STRUCT  *Imc;

  MaxImc = GetMaxImc ();

  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      Imc = &Host->var.mem.socket[Sckt].imc[McIndex];
      if (!Imc->TAD[0].Enable) {
        continue;
      }
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\n\n*******TAD Table for socket %d Mc %d*******\n", Sckt,McIndex);
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Enable  SadID  Limit    SktWays  TargetGran  ChGran  NmTadIndex  FmTadIndex\n" );
      for (TadIndex = 0; TadIndex < MAX_TAD_RULES; TadIndex++) {
        if (!Imc->TAD[TadIndex].Enable) {
          break;
        }
        RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "%6d  %5d  0x%05x  %7d  %10d  %6d  %10d  %10d\n",
          Imc->TAD[TadIndex].Enable, Imc->TAD[TadIndex].SADId, Imc->TAD[TadIndex].Limit, Imc->TAD[TadIndex].socketWays,
          Imc->TAD[TadIndex].TargetGran, Imc->TAD[TadIndex].ChGran, Imc->TAD[TadIndex].NmTadIndex, Imc->TAD[TadIndex].FmTadIndex);
      }
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\n</TADTable>\n");
    }

    DisplayTadTableEx (Sckt, MemMapData);
  }
}

UINT8
FindNmBitMap (
  UINT8 sckt,
  UINT8 mc
  )
  /**

  Description: Find the NM population in the specified MC and fill the bitmap accordingly

  @param socket  - Socket Id
  @param mcIndex - mc id

  @retval Bit map of NM population in the given MC

**/
{
  UINT8 NumChPerMc;
  UINT8 ch, channelInterBitmap = 0;
  NumChPerMc = GetNumChannelPerMc ();

  for (ch = 0 ; ch < NumChPerMc; ch++) {
    if (GetChannelMemSize (sckt, (mc * NumChPerMc) + ch) != 0) {
      channelInterBitmap |= (BIT0<<ch);
    }
  }
  return channelInterBitmap;
}

#ifdef DDRT_SUPPORT
/**

  Check that NVDIMM is enabled

  @param Host           - Pointer to sysHost
  @param Socket         - Socket number
  @param Ch             - Channel number
  @param Dimm           - DIMM slot

  @retval TRUE /FALSE based on NVMDIMM status

**/
BOOLEAN
IsNvDimmEnabled (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Channel,
  UINT8     Dimm
  )
{
  struct ddrRank (*RankList)[MAX_RANK_DIMM];
  struct dimmNvram (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  RankList = GetRankNvList (Host, Socket, Channel, Dimm);

  if (!(*DimmNvList)[Dimm].DcpmmPresent) {
    return FALSE;
  }

  if ((!(*RankList)[0].enabled) || (*DimmNvList)[Dimm].mapOut[0]) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "NVDIMM is disabled!\n");
    return FALSE;
  }

  return TRUE;
}

/**

  Check that NVDIMM is unmapped

  @param Socket         - Socket number
  @param Ch             - Channel number
  @param Dimm           - DIMM slot
  @param Reason         - Reason of NVDIMM unmapped state
                          (valid only when NVDIMM unmapped)

  @retval TRUE /FALSE based on NVMDIMM status

**/
BOOLEAN
IsNvDimmUnmapped (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     *Reason
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  if (NULL == Host) {
    return FALSE;
  }

  //
  // Currently only one DDRT per channel is allowed
  //
  if (NULL != Reason) {
    if (Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmUnmapped) {
      *Reason = Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmUnmappedReason;
    } else {
      *Reason = NVDIMM_UNMAPPED_REASON_MAX;
    }
  }
  return Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmUnmapped;
}

/**

  Check if NVDIMM is remapped for ADx1 configuration

  @param Socket         - Socket number
  @param Ch             - Channel number
  @param Dimm           - DIMM slot

  @retval TRUE /FALSE based on NVMDIMM status

**/
BOOLEAN
IsNvDimmRemapped (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  )
{
  PSYSHOST  Host;

  Host = GetSysHostPointer ();
  if (NULL == Host) {
    return FALSE;
  }

  //
  // Currently only one DDRT per channel is allowed
  //
  return Host->var.mem.socket[Socket].channelList[Ch].DdrtDimmList[0].NvdimmRemapped;
}

/**

  Check that NVDIMM Control Region should be mapped

  @param Socket         - Socket number
  @param Ch             - Channel number
  @param Reason         - Reason of NVDIMM unmapped state
                          (valid only when NVDIMM unmapped)

  @retval TRUE /FALSE based on NVMDIMM status

**/
BOOLEAN
IsNvDimmCtrLRegionToBeMapped (
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  )
{
  PSYSHOST  Host;
  UINT8     Reason;

  Host = GetSysHostPointer ();
  if (NULL == Host) {
    return FALSE;
  }

  if (IsNvDimmUnmapped (Socket, Ch, Dimm, &Reason)) {
    switch (Reason) {
      case NVDIMM_UNMAPPED_POPULATION_NOT_POR:
      case NVDIMM_UNMAPPED_SKU_LIMIT_VIOLATION:
        //
        // Currently for both cases we need to map control region
        // DXE code to update PCD with proper error code to
        // indicate reason of DIMM unmapped status
        //
        return TRUE;

      default:
        //
        // Unknown reason
        //
        return FALSE;
    }
  }

  //
  // DIMM is going to be mapped
  //
  return TRUE;
}

/**
  Get DDRT channel granularity for all CR memory modes

  @retval Channel Granularity for DDRT

**/
UINT8
GetDdrtChannelGranularity (
  VOID
  )
{
  SYSHOST     *Host;

  Host = GetSysHostPointer ();

  if (Host->var.mem.dfxMemVars.DfxDdrtChInterleaveGran == MEM_INT_GRAN_CH_256B_TGT_4KB) {
    return INTERLEAVE_GRANULARITY_256B;
  } else {
    return INTERLEAVE_GRANULARITY_4KB;
  }
}

/**

  Description: Get the first SAD rule of the specified type that the MC is part of.

  @param Host  - Pointer to sysHost
  @param Sckt  - Socket Id
  @param Mc    - IMC Id
  @param Ch    - Chennal id
  @param Dimm  - Dimm num
  @param NodeLimit   - Limit of SAD Rule related to the request

  @retval Partition offset value for the dimm int he Interleave request corresponding to the SAD rule.

**/
UINT32
GetInterReqOffset (
  PSYSHOST Host,
  UINT8    Sckt,
  UINT8    Mc,
  UINT8    Ch,
  UINT8    Dimm,
  UINT32   NodeLimit
  )
{
  UINT8                                     InterleaveIndex;
  UINT8                                     InterleaveChannel;
  UINT8                                     NumOfDimmsInInterleaveSet;
  UINT8                                     ChIndex;
  NGN_DIMM_INTERLEAVE_INFORMATION_PCAT_HOST *InterleavePtr;
  NGN_DIMM_INTERLEAVE_ID_HOST               *InterleaveIdPtr;
  DDRT_DIMM_DEVICE_INFO_STRUCT              *DdrtDimmInfo;


  ChIndex = (Mc * GetNumChannelPerMc ()) + Ch;
  DdrtDimmInfo = GetDdrtDimmInfo (Sckt, Ch, Dimm);
  for (InterleaveIndex = 0; InterleaveIndex < MAX_UNIQUE_NGN_DIMM_INTERLEAVE; InterleaveIndex++) {
    //Skip if there is no interleave record or if the record failed our validation previously
    if ( !DdrtDimmInfo->NgnCfgReq.interleave[InterleaveIndex].RecPresent || !DdrtDimmInfo->NgnCfgReq.interleave[InterleaveIndex].Valid) {
      continue;
    }

    //Skip if this interleave is not processed or nodelimit doesnt match
    if ((!DdrtDimmInfo->NgnCfgReq.interleave[InterleaveIndex].Processed == 1) || (!(DdrtDimmInfo->NgnCfgReq.interleave[InterleaveIndex].SadLimit == NodeLimit))) {
      continue;
    }

    InterleavePtr = &DdrtDimmInfo->NgnCfgReq.interleave[InterleaveIndex];


    NumOfDimmsInInterleaveSet = InterleavePtr->NumOfDimmsInInterleaveSet;
    //scan through the dimm info list to find the matching dimm
    for (InterleaveChannel = 0; InterleaveChannel < NumOfDimmsInInterleaveSet; InterleaveChannel++) {
      //Initialize Interleave id pointer to a dimm set
      InterleaveIdPtr = &DdrtDimmInfo->NgnCfgReq.interleaveId[InterleaveIndex][InterleaveChannel];

      if ((Sckt == InterleaveIdPtr->Socket) && (ChIndex == InterleaveIdPtr->Channel) && (Dimm == InterleaveIdPtr->Dimm)) {
        //If the socket, channel and dimm info matches, return the corresponding offset.
        return InterleaveIdPtr->PartitionOffset;
      }

    } // InterleaveChannel

  } // InterleaveIndex
  //IF we are here, it is an error condition.
  return 0;
}
#endif // DDRT_SUPPORT

/*
  Worker funtion to adjust Dpa offset if system is runnig in 1LM + PMEM mode

  @param[in]   Host        - Pointer to the Host structure.
  @param[in]   DpaOffset   - original DpaOffset
  @param[in]   Socket      - Socket Index
  @param[in]   Ch          - Channel Index

*/
UINT32
ComputeChannelOffset (
  IN PSYSHOST         Host,
  IN UINT32           DpaOffset,
  IN UINT8            Socket,
  IN UINT8            Ch
  )
{
  UINT32 ChMemSize;

  ChMemSize = GetChannelMemSize (Socket, Ch);

  //
  // If the system is running in 1LM + 2LM/PMEM
  // Find if there is another DDR DIMM in this channel that has already been mapped.
  // If so, find the size of the memory that is already mapped from this channel and add it to the offset
  // This will ensure that the channel address decoded using the Tad Offset will be correct.
  //
  switch (Host->var.mem.volMemMode) {
  case VOL_MEM_MODE_1LM:
  case VOL_MEM_MODE_MIX_1LM2LM:
    return DpaOffset + ChMemSize;
  default:
    return DpaOffset;
  }
}

/**

  Find the next SAD entry that needs to be filled for this Socket.

  @param[in] Host             Pointer to sysHost.
  @param[in] MemMapData       Pointer to the memory map private data structure.
  @param[in] Socket           Socket Index.
  @param[in] ClusterId        Cluster Id for the multicast access

  @retval Index of Next available SAD entry.

**/
UINT8
FindSadIndex (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketId,
  IN UINT8            ClusterId
  )
{
  SOCKET_INFO_STRUCT *Socket;
  UINT8              SadIndex;
  UINT8              DramCluster;
  UINT8              BeginSadIndex;
  UINT8              MaxSadIndex;

  if (MemMapData->NumberofChaDramClusters > 1) {
    DramCluster = ClusterId;
  } else {
    DramCluster = 0;
  }

  //
  // Scan through the SAD table and find the next SAD entry with Enable = 0
  //
  Socket = &Host->var.mem.socket[SocketId];
  BeginSadIndex = (DramCluster * MemMapData->MaxSadRules);
  MaxSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
  for (SadIndex = BeginSadIndex; SadIndex < MaxSadIndex; SadIndex++) {
    if (Socket->SAD[SadIndex].Enable == 0) {
      return SadIndex;
    }
  }
  return MaxSadIndex;
}

/** This routine increases Total Memory Allocated amount if SKU Limit violation doesn't occurs.

  @param[in] Host                 - Pointer to sysHost
  @param[in] MemMapData           - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] MemoryToBeAllocated  - Size of memory to be allocated
  @param[in] MemType              - Type of memory
  @param[in] McList               - List of Memory controllers populated

  @retval SUCCESS - Total Memory allocated amount is increased
  @retval FAILURE - Sku limit violation occurs

**/
UINTN
AddTotalMemoryAllocated (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT32          MemoryToBeAllocated,
  IN  UINT16          MemType,
  IN  UINT32          McList[MAX_SOCKET*MAX_IMC]
  )
{
  UINT8   Mc;
  UINT8   Index;
  UINT8   Socket;
  UINT8   MaxImc;
  UINT8   McWays = 0;
  UINT32  MemoryToBeAllocatedPerMc = 0;
  UINT32  MemoryToBeAllocatedPerSoc[MAX_SOCKET] = {0};

  if (MemMapData == NULL) {
    return FAILURE;
  }

  if ((MemType == MemType1lmCtrl) || IsMemTypeHbm (MemType)) {
    return SUCCESS;
  }

  //
  // Calculate number of MC populated across all sockets
  //
  MaxImc = GetMaxImc ();
  for (Index = 0; Index < (MAX_SOCKET * MaxImc); Index++) {
    if (McList[Index] != 0) {
      McWays++;
    }
  }

  //
  // Get total memory allocated per MC
  //
  if (McWays != 0) {
    MemoryToBeAllocatedPerMc = MemoryToBeAllocated / McWays;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }

    //
    // Calculate total memory allocated per Socket
    //
    for (Mc = 0; Mc < MaxImc; Mc++) {
      if (!CheckSocketMcEnabled (Socket, Mc)) {
        continue;
      }
      if (McList[(Socket * MaxImc) + Mc] != 0) {
        MemoryToBeAllocatedPerSoc[Socket] += MemoryToBeAllocatedPerMc;
      }
    } // Mc loop

    if (MemoryToBeAllocatedPerSoc[Socket] != 0) {
      //
      // Check for SKU Limit violation
      //
      if (MemMapData->MemMapSkuLimit[Socket].SkuLimitValid) {
        if ((MemoryToBeAllocatedPerSoc[Socket] + MemMapData->MemMapSkuLimit[Socket].MemoryAllocated) >
            (MemMapData->MemMapSkuLimit[Socket].SkuLimit * 1024)) { //convert SkuLimit from 64GB to 64MB units
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "SKU Limit Violation occurred: 0x%x \n",(MemoryToBeAllocatedPerSoc[Socket] +
            MemMapData->MemMapSkuLimit[Socket].MemoryAllocated));
          MemMapData->MemMapSkuLimit[Socket].Violation = TRUE;
          if (MemMapData->IsNumaEn) {
            //
            // For NUMA, check other sockets
            //
            continue;
          } else {
            //
            // For UMA, exit immediately
            //
            OutputWarning (ERR_SKU_LIMIT, ERR_SKU_LIMIT_VIOLATION, Socket, NO_CH, NO_DIMM, NO_RANK);
            //
            // SKU Limit violation will be handled after SAD rules creation
            //
            return SUCCESS;
          }
        }
      }

      //
      // Update MemoryAllocated
      //
      MemMapData->MemMapSkuLimit[Socket].MemoryAllocated += MemoryToBeAllocatedPerSoc[Socket];
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "MemoryAllocated: 0x%x \n", MemMapData->MemMapSkuLimit[Socket].MemoryAllocated);
    } // MemoryToBeAllocatedPerSoc
  } // Socket loop

  //
  // For NUMA, check if any socket had a SKU Limit violation
  //
  if (MemMapData->IsNumaEn) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (CheckSocketEnabled (Socket) == 0) {
        continue;
      }
      if (MemMapData->MemMapSkuLimit[Socket].Violation == TRUE) {
        OutputWarning (ERR_SKU_LIMIT, ERR_SKU_LIMIT_VIOLATION, Socket, NO_CH, NO_DIMM, NO_RANK);
        //
        // SKU Limit violation will be handled after SAD rules creation
        //
        return SUCCESS;
      }
    } // Socket loop
  } // NUMA

  return SUCCESS;
}

#ifdef DDRT_SUPPORT
BOOLEAN
IsPmemPossible (
  PSYSHOST  Host,
  UINT8     socketBitMap)
/**

  This routine looks for NVM dimms with PMEM partition

  @param Host         - Pointer to sysHost
  @param socketBitMap - BitMap of sockets to process

  @retval SUCCESS

**/
{
  UINT8   ch, sockId;
  UINT8   MaxCh;
  struct  Socket *socket;

  MaxCh = GetMaxImc () * GetNumChannelPerMc ();

  for (sockId = 0; sockId < MAX_SOCKET; sockId++) {
    //Skip the sockets that are not requested. (FOR NUMA)
    if (((BIT0<<sockId) & socketBitMap) == 0) {
      continue;
    }
    socket = &Host->var.mem.socket[sockId];
    for (ch = 0; ch < MaxCh; ch++) {
      if (socket->channelList[ch].perSize) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**

  This routine verifies if one way IMC interleaving and CH interleaving
  should be forced for DFX PMEM request

  @param[in]  Host     - Pointer to sysHosts

  @retval TRUE  - One way interleave should be forced for DFX PMEM
          FALSE - Interleaving should not be modified for DFX PMEM

**/
BOOLEAN
ForceOneWayInterleaveForDfxPmem (
  IN PSYSHOST Host
  )
{
  SYS_SETUP *Setup = NULL;

  if (NULL == Host) {
    return FALSE;
  }

  Setup = GetSysSetupPointer ();
  if (NULL == Setup) {
    return FALSE;
  }

  return !!(Setup->mem.dfxMemSetup.dfxOptions & SHORT_STROKE_NO_INT);
}

/**

  This routine copies the pmem size into the remSize field of each channel in the sockets specified

  @param[in] Host         - Pointer to sysHost
  @param[in] MemMapData   - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] socketBitMap - BitMap of sockets to process

  @retval VOID

**/
VOID
UpdateHostStructForPmem (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketBitMap
  )
{
  UINT8   Ch;
  UINT8   SockId;
  UINT8   MaxCh;

  MaxCh = GetMaxImc () * GetNumChannelPerMc ();

  for (SockId = 0; SockId < MAX_SOCKET; SockId++) {
    // Skip the sockets that are not requested. (FOR NUMA)
    if (((BIT0 << SockId) & SocketBitMap) == 0) {
      continue;
    }
    for (Ch = 0; Ch < MaxCh; Ch++) {
      //
      // Only AD and legacy AD-WB caching shall use the flow for mapping persistent memory.
      // Legacy PMEM caching (AD-WB SOW) does not permit co-existence of AppDirect and AppDirect-WB regions.
      // So the partition size of persistent = persistent cache.
      //
      Host->var.mem.socket[SockId].channelList[Ch].remSize = Host->var.mem.socket[SockId].channelList[Ch].perSize;
    }
  }
}

/** Get persistent memory type

  This is worker function to get persistent memory types based on setup options.

  @param[in]  Host        - Pointer to sysHost
  @param[in]  MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @return Persistent memory type

**/
MEM_TYPE
GetPerMemType (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  MEM_TYPE MemType;

  MemType = MemTypeNone;
  if (Host->var.mem.dfxMemVars.dfxPerMemMode == PER_MEM_MODE)  {
    MemType = MemType1lmAppDirect;
    if (IS_DDR_2LM_OR_MIXED (Host) &&
         (MemMapData->PmemCaching == LegacyPmCaching)) {
      MemType = MemType2lmDdrWbCacheAppDirect;
    }
  }

  return MemType;
}

/**

  Interleave persistent memory on IMC and channel level but not across sockets for NUMA configuration.

  @param[in] Host               - Pointer to sysHost
  @param[in] MemMapData         - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] SocketNum          - Socket number


  @retval status

**/
UINT32
SocketNonInterleavePerMem (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           SocketNum
  )
{
  MEM_TYPE  MemType;
  UINT8     SocketBitMap;
  UINT32    Status = SUCCESS;
  UINT8     ImcInter;
  UINT8     ChInter;
  BOOLEAN   InterModified;

  //
  // Interleave PMEM/PMEM$ only if the dimms are controlled by SV only setup options not the CR Management driver
  //
  if (Host->var.mem.dfxMemVars.dfxDimmManagement != DIMM_MGMT_CR_MGMT_DRIVER) {
    SocketBitMap = BIT0 << SocketNum;
    //
    // Check if PMEM is possible and that PMEM mapping is enabled
    //
    if (IsPmemPossible (Host, SocketBitMap) && Host->var.mem.dfxMemVars.dfxPerMemMode != NON_PER_MEM_MODE) {
      //
      // Get Persistent Memory Type
      //
      MemType = GetPerMemType (Host, MemMapData);
      //
      // Update interleave settings (CH way and iMC way according to selected DFX PMEM interleave type)
      //
      InterModified = ForceOneWayInterleaveForDfxPmem (Host);
      if (InterModified) {
        //
        // Store current values (need to restore after PMEM processing done)
        //
        ImcInter = Host->var.mem.imcInter;
        ChInter = Host->var.mem.chInter;
        //
        // Force One Way Interleave
        //
        Host->var.mem.imcInter = IMC_1WAY;
        Host->var.mem.chInter = CH_1WAY;
      }
      //
      // Check for any SOC-specific restrictions on DCPMM AppDirect interleaving
      //
      CountAdx1DcpmmInterleaveDfxSetupOption (Host, MemMapData);

      //
      // Update the remsize information in the Host struct for this socket before calling SadNuma() to create PMEM SADs
      //
      UpdateHostStructForPmem (Host, MemMapData, SocketBitMap);
      //
      // Call SadNuma() to create SADs for the PMEM in this socket
      //
      Status = SadNuma (Host, MemMapData, SocketNum, MemType, 0);
      //
      // PMEM SADs processed, restore initial interleave setting (regardless of SAD creation status)
      //
      if (InterModified) {
        //
        // Restore initial values
        //
        Host->var.mem.imcInter = ImcInter;
        Host->var.mem.chInter = ChInter;
      }
      //
      // Check PMEM SADs creation status and exit accordingly
      //
      if (Status != SUCCESS) {
        return Status;
      }
    }
  } else {
    //
    // Handle CCUR records
    //
    Status = ApplyCcurRecords (Host, MemMapData, SocketNum);
    if (Status != SUCCESS) {
      return Status;
    }

    if (!MemMapData->MemMapDegradeLevel) {
      //
      // Handle CFGIn interleave requests
      //
      Status = ApplyInterleaveRequests (Host, MemMapData, SocketNum);
    }
  }
  return SUCCESS;
}
#endif // DDRT_SUPPORT

UINT8
RankSort (
  PSYSHOST Host,
  struct rankSort rkSortList[],
  UINT8 sckt,
  UINT8 ch
  )
/**

  Computes a list of ranks sorted by size, largest to smallest.

  @param Host        - Pointer to sysHost
  @param rkSortList  - Rank sort list
  @param sckt      - Socket Id
  @param ch          - Channel number (0-based)

  @retval sortNum - Number of ranks with memory remaining to be allocated
            @retval (sortNum may be 0 indicating all memory allocated)

**/
{
  UINT8 dimm;
  UINT8 rank;
  UINT8 si;
  UINT8 i;
  UINT8 sortNum;
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList (Host, sckt, ch);

  // Sort ranks by size, largest first
  sortNum = 0;
  for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      // Skip if this DIMM is not present
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

#ifdef DDRT_SUPPORT
      // Skip if this DIMM is an NVM DIMM
      if ((*dimmNvList)[dimm].DcpmmPresent == 1) {
        continue;
      }
#endif // DDRT_SUPPORT

      rankList = GetRankNvList (Host, sckt, ch, dimm);

      if (CheckRank (Host, sckt, ch, dimm, rank, CHECK_MAPOUT)) {
        continue;
      }

      // Search for ranks with remaining size
      if ((*rankList)[rank].remSize == 0) {
        continue;
      }

      // Determine sort list position
      for (si = 0; si < sortNum; si++) {
        if (rkSortList[si].rankSize < (*rankList)[rank].remSize) {
          break;
        }
      }
      // Shift smaller ranks towards end of list
      if (si < sortNum) {
        for (i = sortNum; i > si; i--) rkSortList[i] = rkSortList[i - 1];
      }
      // Insert current rank
      rkSortList[si].socketNum = sckt;
      rkSortList[si].chNum = ch;
      rkSortList[si].dimmNum = dimm;
      rkSortList[si].rankNum = rank;
      rkSortList[si].rankID = GetRirRankTarget (Host, sckt, ch, dimm, rank);
      rkSortList[si].rankSize = (*rankList)[rank].remSize;
      sortNum++;
    } // DIMM loop
  } // Rank loop

  return sortNum;
}

/**

  Interleaves memory on the rank level for DDR and NVM memory.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval SUCCESS

**/
UINT32
RankInterleave (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData
  )
{
  RankInterleaveDdr (Host, MemMapData);
#ifdef DDRT_SUPPORT
  RankInterleaveDdrt (Host, MemMapData);
#endif // DDRT_SUPPORT
  return SUCCESS;
}

/**

  Interleaves memory on the rank level. This routine figures out the
  mapping and fills in the structures for the RIR tables for DDR.
  It does not program any registers.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval SUCCESS

**/
UINT32
RankInterleaveDdr (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8              Sckt;
  UINT8              Ch;
  UINT8              Rank;
  UINT8              Si;
  UINT8              IntIndex;
  UINT8              SwapIntIndex;
  UINT8              InterNum;
  UINT8              SortNum;
  INT32              NVPass;
  DDR_RANK_STRUCT    (*RankListTemp)[MAX_RANK_DIMM];
  UINT8              RankTemp;
  UINT8              DimmTemp;
  DIMM_NVRAM_STRUCT  (*DimmNvList)[MAX_DIMM];
  UINT8              DimmNum;
  UINT16             InterSize;
  UINT16             RirLimit;
  UINT8              RirIndex;
  UINT16             RankMemUsed;
  UINT16             PreviousRIRLimit;
  UINT32             ChMemSize;
  SOCKET_NVRAM       (*SocketNvram)[MAX_SOCKET];
  DDR_RANK_STRUCT    (*RankList)[MAX_RANK_DIMM];
  RANK_SORT_STRUCT   RkSortList[MAX_RANK_CH];
  RANK_SORT_STRUCT   RkSortItem;
  DDR_CHANNEL_STRUCT *ChannelList;
  MEMMAPDATA_CHANNEL *MemMapChannel;
  SYS_SETUP          *Setup;
  UINT8              MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  SocketNvram = &Host->nvram.mem.socket;
  // Loop for each socket
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }

    // Loop for each channel
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*SocketNvram)[Sckt].channelList[Ch].enabled == 0) {
        continue;
      }

      ChMemSize = GetChannelMemSize (Sckt, Ch);
      ChannelList = &Host->var.mem.socket[Sckt].channelList[Ch];
      MemMapChannel = &MemMapData->MemMapSocket[Sckt].MemMapChannel[Ch];

      if (ChMemSize == 0) {
        continue;
      }

      // Get desired interleave width
      InterNum = Host->var.mem.rankInter;

      // Start the RirLimit off at 0 for each channel
      RirLimit = 0;
      PreviousRIRLimit = 0;
      DimmNvList = GetDimmNvList (Host, Sckt, Ch);
      NVPass = 0;

      // 5 Rank Level Interleaves can be defined
      for (RirIndex = 0; RirIndex < MAX_RIR; RirIndex++) {

        // Check for exit condition: all channel memory allocated in RIR
        if (RirLimit == ChMemSize) {
          break;
        }

        // Sort ranks by size, largest first
        SortNum = RankSort (Host, RkSortList, Sckt, Ch);
        // Determine largest N ranks, where N:1 is the rank interleave
        if (SortNum) {

          // Compare N to remaining ranks
          while (InterNum > SortNum) {
            InterNum >>= 1;
          }

          // Get size common to N ranks
          InterSize = RkSortList[InterNum - 1].rankSize;

          // Update the size common to N ranks
          if (InterSize > RkSortList[InterNum - 1].rankSize) {
            InterSize = RkSortList[InterNum - 1].rankSize;
          }

          // optimize sortList to eliminate adjacent
          if (InterNum > 2) {
            for (IntIndex = 0; IntIndex < InterNum - 1; IntIndex++) {
              // Check if these ranks are adjacent
              if (RkSortList[IntIndex].dimmNum == RkSortList[IntIndex + 1].dimmNum) {

                // Point to a entry to trade with
                SwapIntIndex = IntIndex + 2;

                // Break if we can't find an entry to trade with
                if (SwapIntIndex >= InterNum) {
                  break;
                }

                // Find an entry that is different
                while (RkSortList[IntIndex].dimmNum == RkSortList[SwapIntIndex].dimmNum) {
                  SwapIntIndex++;
                  // Break if we can't find an entry to trade with
                  if (SwapIntIndex >= InterNum) {
                    break;
                  }
                }

                // Swap entries if this entry is valid
                if (SwapIntIndex >= InterNum) {
                  break;
                }

                // Exchange sortList entries 1 and 2
                RkSortItem               = RkSortList[IntIndex];
                RkSortList[IntIndex]     = RkSortList[SwapIntIndex];
                RkSortList[SwapIntIndex] = RkSortItem;
              }
            }
          }

          // Increment RIR.Limit by InterNum * InterSize
          RirLimit += (InterNum * InterSize);

          // Save RIR Limit / ways
          MemMapChannel->RirLimit[RirIndex] = RirLimit;
          MemMapChannel->RirWay[RirIndex] = InterNum;
          MemMapChannel->RirValid[RirIndex] = 1;

          // Initialize RIR Interleave
          for (IntIndex = 0; IntIndex < InterNum; IntIndex++) {
            Rank = RkSortList[IntIndex].rankNum;

            // Fill in rank ID
            MemMapChannel->RirIntList[RirIndex][IntIndex] = RkSortList[IntIndex].rankID;

            // Compute RIR offset = (channel_address_base / RIRways) - Rank_Address_base
            DimmNum = RkSortList[IntIndex].dimmNum;

            RankList = GetRankNvList (Host, Sckt, Ch, DimmNum);
            if ((PcdGetBool (PcdNvDimmEn)) && (NVPass == 0)) {
              RankMemUsed = (*RankList)[Rank].rankSize - ((*RankList)[Rank].remSize + (*RankList)[Rank].NVrankSize);
            } else {
              RankMemUsed = (*RankList)[Rank].rankSize - (*RankList)[Rank].remSize;
            }
            MemMapChannel->RirOffset[RirIndex][IntIndex] = (PreviousRIRLimit / InterNum) - RankMemUsed;
          }

          // Fill in the rest of the rir registers for this interleave
          // IntIndex index was carried over from previous loop
          for (; IntIndex < MAX_RIR_WAYS; IntIndex++) {
            MemMapChannel->RirIntList[RirIndex][IntIndex] = 0;
            MemMapChannel->RirOffset[RirIndex][IntIndex] = 0;
          }

          // Subtract common size from N ranks
          for (Si = 0; Si < InterNum; Si++) {
            (*SocketNvram)[Sckt].channelList[Ch].dimmList[RkSortList[Si].dimmNum].rankList[RkSortList[Si].rankNum].remSize = (*SocketNvram)[Sckt].channelList[Ch].dimmList[RkSortList[Si].dimmNum].rankList[RkSortList[Si].rankNum].remSize - InterSize;
          }

          // Save current RirLimit to use while creating the next entry
          PreviousRIRLimit = RirLimit;

        } else if (PcdGetBool (PcdNvDimmEn)) {
          if (NVPass != 0) {
            break;
          }
          if (!ChannelList->NVmemSize) {
            break;
          }
          NVPass = 1;
          RirIndex--;
          ChannelList->remSize = ChannelList->NVmemSize;
          if (Setup->mem.interNVDIMMS == 0) {
            InterNum = 1;
          }

          for (DimmTemp = 0; DimmTemp < MAX_DIMM; DimmTemp++) {
            if ((*DimmNvList)[DimmTemp].dimmPresent == 0) {
              continue;
            }

            RankListTemp = &Host->nvram.mem.socket[Sckt].channelList[Ch].dimmList[DimmTemp].rankList;

            for (RankTemp = 0; RankTemp < MAX_RANK_DIMM; RankTemp++) {
              if ((*RankListTemp)[RankTemp].enabled == 0) {
                continue;
              }
              (*RankListTemp)[RankTemp].remSize = (*RankListTemp)[RankTemp].NVrankSize;
            } // RankTemp loop
          } // DimmTemp loop
        } else {
          break;// if SortNum
        }
      } // RirIndex loop

      // Issue Fatal error if all RIR rules used before all memory allocated
      if (RirIndex > MAX_RIR) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "RIR rules exceeds %d\n", MAX_RIR);
        FatalError (ERR_INTERLEAVE_FAILURE, ERR_RIR_RULES_EXCEEDED);
      }

      // Issue warning if all all the memory did not get mapped
      if (RirLimit < ChMemSize) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "RIR rules exceeds %d\n", MAX_RIR);

        ChannelList->memSize = RirLimit;

        OutputWarning (WARN_INTERLEAVE_FAILURE, WARN_RIR_RULES_EXCEEDED, Sckt, Ch, 0xFF, 0xFF);
      }

      // Initialize unused RIR rules for the current channel

      // RirIndex set by the for loop above
      for (; RirIndex < MAX_RIR; RirIndex++) {
        MemMapChannel->RirLimit[RirIndex] = RirLimit;
        MemMapChannel->RirValid[RirIndex] = 0;
        MemMapChannel->RirWay[RirIndex] = 0;

        for (IntIndex = 0; IntIndex < MAX_RIR_WAYS; IntIndex++) {
          MemMapChannel->RirIntList[RirIndex][IntIndex] = 0;
          MemMapChannel->RirOffset[RirIndex][IntIndex] = 0;
        }
      }
    } // Ch loop
  } // socket loop

  return SUCCESS;
}

#ifdef DDRT_SUPPORT
/**

  Interleaves memory on the rank level. This routine figures out the
  mapping and fills in the structures for the RIR tables for DDRT.
  It does not program any registers.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval SUCCESS

**/
UINT32
RankInterleaveDdrt (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8              Sckt;
  UINT8              Ch;
  UINT8              WayIndex;
  UINT32             RirLimit;
  UINT8              RirIndex;
  SOCKET_NVRAM       (*SocketNvram)[MAX_SOCKET];
  DIMM_NVRAM_STRUCT  (*DimmNvList)[MAX_DIMM];
  MEMMAPDATA_CHANNEL *MemMapChannel;
  UINT8              AepPresent;
  UINT8              Dimm;
  UINT16             SadTadGran;
  UINT32             ChMemSize;
  MEMORY_MAP_HOST    *MemMapHost;
  UINT8              MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  SocketNvram = &Host->nvram.mem.socket;
  MemMapHost = GetMemMapHostPointer();

  // Loop for each socket
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }

    // Loop for each channel
    for (Ch = 0; Ch < MaxChDdr; Ch++) {

      // Check if channel is enabled
      if ((*SocketNvram)[Sckt].channelList[Ch].enabled == 0) {
        continue;
      }

      // Find NVM DIMM on this channel - only a single NVM DIMM per channel is supported
      AepPresent = 0;
      DimmNvList = GetDimmNvList (Host, Sckt, Ch);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) continue;
        if ((*DimmNvList)[Dimm].DcpmmPresent)  {
          AepPresent = 1;
          break;
        }
      }

      // Skip this channel if NVM DIMM is not present
      if (AepPresent == 0) {
        continue;
      }

      ChMemSize = GetChannelMemSize (Sckt, Ch);
      MemMapChannel = &MemMapData->MemMapSocket[Sckt].MemMapChannel[Ch];


      // For 1LM and 1LM+2LM mixed mode, the RIR limit needs to include the DDR memory in the channel.  For 2LM, it does not
      if (IS_DDR_2LM_CACHE(Host)) {
        RirLimit = (*DimmNvList)[Dimm].rawCap + DDRT_VOL_DPA_START;
      } else {
        RirLimit = ChMemSize + (*DimmNvList)[Dimm].rawCap + DDRT_VOL_DPA_START;
      }

      // The RirLimit calculated above is in SAD_GRAN units, but it is eventually stored in RIR_GRAN units.  So, we need to round the RirLimit to the next RIR_GRAN boundary or it will be too small
      SadTadGran = GetSadTadGranularity (Host);
      if (RirLimit % (RIR_GRAN / SadTadGran))  {
        RirLimit += (RIR_GRAN / SadTadGran) - (RirLimit % (RIR_GRAN / SadTadGran));
      }

      // Since NVMCTLR DIMMs have a single rank and since there is only one NVMCTLR DIMM per channel, there is only a single entry
      MemMapChannel->RirDdrtLimit[0] = RirLimit;
      MemMapChannel->RirDdrtWay[0] = 1;
      MemMapChannel->RirDdrtValid[0] = 1;
      MemMapChannel->RirDdrtIntList[0][0] = GetRirRankTarget (Host, Sckt, Ch, Dimm, 0);

      // For 1LM and 1LM+2LM mixed mode, update the DDRT offset needs to include the DDR memory in the channel.  For 2LM, it does not
      if (IS_DDR_2LM_CACHE(Host)) {
        MemMapChannel->RirDdrtOffset[0][0] = 0;
      } else {
        MemMapChannel->RirDdrtOffset[0][0] = ChMemSize;
      }

      // Fill in the rest of the rir registers for this interleave
      for (WayIndex = 1 ; WayIndex < MAX_RIR_DDRT_WAYS ; WayIndex++) {
        MemMapChannel->RirDdrtIntList[0][WayIndex] = 0;
        MemMapChannel->RirDdrtOffset[0][WayIndex] = 0;
      }

      // Initialize unused RIR rules for the current channel

      // RirIndex set by the for loop above
      for (RirIndex = 1; RirIndex < MAX_RIR_DDRT ; RirIndex++) {
        MemMapChannel->RirDdrtLimit[RirIndex] = RirLimit;
        MemMapChannel->RirDdrtValid[RirIndex] = 0;
        MemMapChannel->RirDdrtWay[RirIndex] = 0;

        for (WayIndex = 0; WayIndex < MAX_RIR_DDRT_WAYS ; WayIndex++) {
          MemMapChannel->RirDdrtIntList[RirIndex][WayIndex] = 0;
          MemMapChannel->RirDdrtOffset[RirIndex][WayIndex] = 0;
        }
      }
    } // Ch loop
  } // socket loop

  return SUCCESS;
}

/** Parition DDRT DIMM

  This routine checks if partitioning is requested form BIOS knobs and partitions the dimm as per the requested ratio.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] sckt        - Socket number
  @param[in] ch          - Channel number
  @param[in] dimm        - Dimm number

  @retval SUCCESS - DDRT DIMM partitioned successfully.
  @retval FAILURE - DDRT DIMM could not be partitioned.

**/
UINT32
PartitionDdrt (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           sckt,
  IN UINT8           ch,
  IN UINT8           dimm
  )
{
  UINT32 Status;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  UINT8  volRatio = 0, pmemRatio = 0, AdWbRatio = 0, mcIndex = 0;
  UINT32 volSize = 0, nonVolSize = 0;
  UINT64_STRUCT tempData;
  struct fnvCISBuffer fnvBuffer;
  UINT8 MbStatus = 0;
  DIMM_PARTITION_DATA (*dimmPartitionList)[MAX_SOCKET][MAX_CH][MAX_DIMM];
  INT32 Adjustment;

  //
  // Function should only be called if the Reference BIOS Setup is being used to partition and manage the DIMMs
  //
  if (Host->var.mem.dfxMemVars.dfxDimmManagement != DIMM_MGMT_BIOS_SETUP) {
    return SUCCESS;
  }

  dimmNvList = GetDimmNvList (Host, sckt, ch);

  RcDebugPrintWithDevice (SDBG_MAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\tIn PartitionDdrt dimm\n");

  //
  // Get the partition ratio for this dimm from the setup options
  //
  mcIndex = (sckt * GetMaxImc ()) + (ch / GetNumChannelPerMc ());

  RcDebugPrintWithDevice (SDBG_MAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\tmcIndex: %d\n", mcIndex);

  switch (Host->var.mem.dfxMemVars.dfxPartitionRatio[mcIndex]) {
  case RATIO_50_50_0:
    volRatio = 50;
    pmemRatio = 50;
    AdWbRatio = 0;
    break;
  case RATIO_50_0_50:
    volRatio = 50;
    pmemRatio = 0;
    AdWbRatio = 50;
    break;
  case RATIO_0_50_50:
    volRatio = 0;
    pmemRatio = 50;
    AdWbRatio = 50;
    break;
  case RATIO_100_0_0:
    volRatio = 100;
    pmemRatio = 0;
    AdWbRatio = 0;
    break;
  case RATIO_0_100_0:
    volRatio = 0;
    pmemRatio = 100;
    AdWbRatio = 0;
    break;
  case RATIO_0_0_100:
    volRatio = 0;
    pmemRatio = 0;
    AdWbRatio = 100;
    break;
  }

  //
  // Ignore WbCachePerSize in Legacy PMEM caching as it is unique to native AD-WB caching flow introduced in CR 2.0.
  // Legacy PMEM caching is mutually exclusive with PMEM. So PmemRatio is only used.
  //
  if (MemMapData->PmemCaching == LegacyPmCaching) {
    AdWbRatio = 0;
  }

  RcDebugPrintWithDevice (SDBG_MAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\t option chosen: %d\n", Host->var.mem.dfxMemVars.dfxPartitionRatio[mcIndex]);
  RcDebugPrintWithDevice (SDBG_MAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\t %d %d %d\n", volRatio, pmemRatio, AdWbRatio);
  volSize = ((((UINT32) (*dimmNvList)[dimm].rawCap) * volRatio) / 100) << CONVERT_64MB_TO_4KB_GRAN;
  nonVolSize = ((((UINT32) (*dimmNvList)[dimm].rawCap) * (pmemRatio + AdWbRatio)) / 100) << CONVERT_64MB_TO_4KB_GRAN;

  if ((volSize & (MEM_1GB_AT_4KB_GRAN - 1)) != 0) {
    //
    // If there is enough non-volatile memory, and the higher 1GB boundary is
    // closer, round the volatile size up. Otherwise, round it down.
    //
    Adjustment = volSize & (MEM_1GB_AT_4KB_GRAN - 1);
    if (nonVolSize > MEM_1GB_AT_4KB_GRAN &&
        Adjustment >= (MEM_1GB_AT_4KB_GRAN / 2)) {
      Adjustment -= MEM_1GB_AT_4KB_GRAN;
    }

    //
    // Subtract the adjustment (which may be negative) from the volatile size,
    // and add it to the non-volatile size if possible.
    //
    volSize -= Adjustment;
    if (nonVolSize > MEM_1GB_AT_4KB_GRAN ||
        (Adjustment > 0 && nonVolSize != 0)) {
      nonVolSize += Adjustment;
    }
  }

  RcDebugPrintWithDevice (SDBG_MAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\t Requesting partitioning for volCap(4kb gran): %d nonVolCap(4kb gran):%d  \n", volSize, nonVolSize);

  Status = SetDimmPartitionInfo (Host, sckt, ch, dimm, volSize, nonVolSize, &fnvBuffer, &MbStatus);

  if (Status == SUCCESS) {
    dimmPartitionList = GetFnvCisBufferDimmPartition (&fnvBuffer);
    (*dimmNvList)[dimm].volCap = (UINT16)((*dimmPartitionList)[sckt][ch][dimm].volatileCap >> CONVERT_4KB_TO_64MB_GRAN);
    (*dimmNvList)[dimm].nonVolCap = (UINT16)((*dimmPartitionList)[sckt][ch][dimm].persistentCap >> CONVERT_4KB_TO_64MB_GRAN);
    RcDebugPrintWithDevice (SDBG_MAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\t Partitioing returns the values: volCap(4kb): %d nonVolCap(4kb):%d \n", (*dimmPartitionList)[sckt][ch][dimm].volatileCap, (*dimmPartitionList)[sckt][ch][dimm].persistentCap);
    //
    // HSD-169493: Division by zero error when using ratio 100% vol, 0% PMEM, 0% BLK to partition NVMDIMM dimm
    //
    if ((*dimmNvList)[dimm].nonVolCap == 0) {
      (*dimmNvList)[dimm].perCap = 0;
      (*dimmNvList)[dimm].WbCachePerCap = 0;
    } else {
      //
      // HSD-4928795: When partitionDDRTDimm is enabled persistent, blk memory sizes are incorrect
      //
      (*dimmNvList)[dimm].perCap = (UINT16)((((UINT32)(*dimmNvList)[dimm].nonVolCap) * pmemRatio) / (pmemRatio + AdWbRatio));
      (*dimmNvList)[dimm].WbCachePerCap = (UINT16)((((UINT32)(*dimmNvList)[dimm].nonVolCap) * AdWbRatio) / (pmemRatio + AdWbRatio));
    }

    //
    // Start addresses are in byte granularity
    //
    tempData = RShiftUINT64 ((*dimmPartitionList)[sckt][ch][dimm].volatileStart, CONVERT_B_TO_64MB);
    (*dimmNvList)[dimm].volRegionDPA = tempData.lo;
    tempData = RShiftUINT64 ((*dimmPartitionList)[sckt][ch][dimm].persistentStart, CONVERT_B_TO_64MB);
    (*dimmNvList)[dimm].perRegionDPA = tempData.lo;
    RcDebugPrintWithDevice (SDBG_MAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "VolDPA: %d   PerDPA: %d \n", (*dimmNvList)[dimm].volRegionDPA, (*dimmNvList)[dimm].perRegionDPA);
  } else {
    //
    // Update the host structure
    //
    (*dimmNvList)[dimm].nonVolCap = 0;
    (*dimmNvList)[dimm].volCap = 0;
    (*dimmNvList)[dimm].volRegionDPA = 0;
    (*dimmNvList)[dimm].perRegionDPA = 0;
  }

  return Status;
}

VOID AlignVolCaptoGb (PSYSHOST Host)
/**

  This routine aligns the volCap of all the channels in every NVMCTLR dimm in the system to the nearest GB.
  This is to suffice a silicon requirement when SNC is enabled.

  @param Host  - Pointer to sysHost

  @retval VOID

**/
{
  UINT8 sckt;
  UINT8 ch;
  UINT8 dimm;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  SYS_SETUP *Setup;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();


  // For each socket
  for (sckt = 0; sckt < MAX_SOCKET; sckt++) {
    if (CheckSocketEnabled (sckt) == 0) {
      continue;
    }
    channelNvList = GetChannelNvList (Host, sckt);

    // For each channel
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, sckt, ch);
      // For each DIMM
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }        //Skip if this is not a NVMDIMM dimm
        if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
          continue;
        }

        if ((*dimmNvList)[dimm].volCap) {
          //Round off the volcap to the GB.
          (*dimmNvList)[dimm].volCap &= GB_ALIGN_NVM;
        }

        if (Host->var.mem.dfxMemVars.dfxDimmManagement != DIMM_MGMT_CR_MGMT_DRIVER) {
          if ((*dimmNvList)[dimm].perCap) {
            if (!(Setup->mem.dfxMemSetup.dfxOptions & (SHORT_STROKE_INT | SHORT_STROKE_NO_INT))) {
              // round down the perCap to the 1GB
              (*dimmNvList)[dimm].perCap &= GB_ALIGN_NVM;
            }
          }
        } else if ((*dimmNvList)[dimm].nonVolCap) {
          //Round off the nonVolcap to the GB to make size consistent with volCap
          (*dimmNvList)[dimm].nonVolCap &= GB_ALIGN_NVM;
        }
      }//dimm
    }//ch
  }//sckt
  return;
}

/**

  This routine initializes the memory size fields in the structures
  for DIMMs, Channels, and Nodes.  The sizes are calculated from the sizes in
  the rank structures.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @retval VOID

**/
VOID
UpdateDdrtFields (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  MEMORY_MAP_HOST    *MemMapHost;
  UINT8 sckt;
  UINT8 ch;
  UINT8 dimm;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  UINT8 i = 0;
  UINT8 MbStatus = 0;
  struct dimmInformation dimmInfoList;
  struct fnvCISBuffer fnvBuffer;
  UINT64_STRUCT tempData;
  DIMM_PARTITION_DATA  (*dimmPartitionList)[MAX_SOCKET][MAX_CH][MAX_DIMM];
  UINT32 Status = SUCCESS;
  BOOT_STATUS_REGISTER      Bsr;
  BOOT_STATUS_HIGH_REGISTER BsrHi;
  UINT8 MaxChDdr;

  ZeroMem (&dimmInfoList, sizeof (dimmInfoList));
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"Gather PMEM information:\n");

  //
  // WPQ Flush is supported only by CLX Silicon and AEP DIMMs with FW Build versions >= 0x4774.
  // Major revision 0x1/Minor revision 0x0
  //
  MemMapHost = GetMemMapHostPointer ();
  MemMapHost->IsWpqFlushSupported = TRUE;

  MaxChDdr = GetMaxChDdr ();
  // For each socket
  for (sckt = 0; sckt < MAX_SOCKET; sckt++) {
    if (CheckSocketEnabled (sckt) == 0) {
      continue;
    }
    channelNvList = GetChannelNvList (Host, sckt);

    // For each channel
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, sckt, ch);
      // For each DIMM
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        //Skip if this is not a NVMDIMM dimm
        if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
          continue;
        }

        //Skip if DIMM is disabled
        if (!IsNvDimmEnabled (Host, sckt, ch, dimm)) {
          continue;
        }

        if (PcdGetBool (PcdUseNvdimmFwInterface))  {
          RcDebugPrintWithDevice (SDBG_MAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\nIssuing IdentifyDimm\n");
          //Issue identify dimm to all the NVMCTLR dimms present to get the Manufacturer id and serial number
          Status = IdentifyDimm (Host, sckt, ch, dimm, AIT_DRAM_FLAG_CHECK_DISABLE, &dimmInfoList, &MbStatus);
          if (Status == SUCCESS) {
            //Update dimm identification information in the NVRAM struct
            (*dimmNvList)[dimm].firmwareRevision.majorVersion = dimmInfoList.fwr.majorVersion;
            (*dimmNvList)[dimm].firmwareRevision.minorVersion = dimmInfoList.fwr.minorVersion;
            (*dimmNvList)[dimm].firmwareRevision.hotfixVersion = dimmInfoList.fwr.hotfixVersion;
            (*dimmNvList)[dimm].firmwareRevision.buildVersion = dimmInfoList.fwr.buildVersion;

            if (dimmInfoList.fwr.majorVersion == AEP_MAJOR_VERSION_WPQ_ENABLED
                && dimmInfoList.fwr.minorVersion ==  AEP_MINOR_VERSION_WPQ_ENABLED
                && dimmInfoList.fwr.buildVersion < AEP_BUILD_NUMBER_WPQ_ENABLED) {
              MemMapHost->IsWpqFlushSupported = FALSE;
            }

            for (i = 0; i < NGN_MAX_MANUFACTURER_STRLEN; i++) {
              (*dimmNvList)[dimm].manufacturer[i] = dimmInfoList.mf[i];
            }

            for (i = 0; i < NGN_MAX_SERIALNUMBER_STRLEN; i++) {
              (*dimmNvList)[dimm].serialNumber[i] = dimmInfoList.sn[i];
            }
            //
            // Avoid intrinsic _memcpy
            //
            CopyMem (&(*dimmNvList)[dimm].PartNumber, &dimmInfoList.Pn, NGN_MAX_PARTNUMBER_STRLEN);

            for (i = 0; i < NGN_MAX_UID_STRLEN; i++) {
              (*dimmNvList)[dimm].Uniqueid[i] = dimmInfoList.Uid[i];
            }
            (*dimmNvList)[dimm].InterfaceFormatCode = dimmInfoList.ifc;
            (*dimmNvList)[dimm].dimmSku = dimmInfoList.dimmSku;
          } else {
            RcDebugPrintWithDevice (SDBG_MINMAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Communication with the NGN DIMM failed\n");
            //log data format:
            //Byte 0 (Bit 0 - DIMM Slot Number, Bits 1-3 - Channel Number, Bits 4-6 - Socket Number,Bit 7 - Reserved)
            //Byte 1 - FW MB Opcode,Byte 2 - FW MB Sub-Opcode,Byte 3 - FW MB Status Code
            EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, sckt, ch, dimm, NO_RANK);
            DisableDIMM (Host, sckt, ch, dimm);
          }
          (*dimmNvList)[dimm].rawCap = (UINT16) (dimmInfoList.rc >> CONVERT_4KB_TO_64MB_GRAN);
          RcDebugPrintWithDevice (SDBG_MAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "\tRaw Capacity retrieved from Identifydimm : %d\n", (*dimmNvList)[dimm].rawCap);
        }
        //Update raw capacity of the dimm convert from 4kb granularity to 64MB granularity
        if ((*dimmNvList)[dimm].rawCap == 0)  {
          (*dimmNvList)[dimm].rawCap = (UINT16) (dimmInfoList.rc >> CONVERT_4KB_TO_64MB_GRAN);
        }

        //Set the default NVMDIMM DIMM partitioning to be 100%VOL 0%PMEM 0%BLK
        (*dimmNvList)[dimm].volCap = (*dimmNvList)[dimm].rawCap;
        (*dimmNvList)[dimm].nonVolCap = 0;
        (*dimmNvList)[dimm].perCap = 0;
        (*dimmNvList)[dimm].WbCachePerCap = 0;
        (*dimmNvList)[dimm].volRegionDPA = DDRT_VOL_DPA_START;
        (*dimmNvList)[dimm].perRegionDPA = 0;

        //
        // Check Boot Status Register for Media Disabled.  If it's set, then reset capacities stored in NVRAM,
        // since this DIMM can no longer be used until the media is re-enabled and move on to the next DIMM
        //
        ReadBootStatusRegisters (Host, sckt, ch, dimm, &Bsr, &BsrHi);
        if (!FNV_MEDIA_IS_USABLE (Bsr)) {

          (*dimmNvList)[dimm].volCap = 0;

          RcDebugPrintWithDevice (SDBG_MINMAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DIMM Media Disabled!\n");
          EwlOutputType1 (WARN_NVMCTRL_MEDIA_STATUS, WARN_NVMCTRL_MEDIA_INERROR, sckt, ch, dimm, NO_RANK);
          continue;
        } else if (IsNvDimmUnmapped (sckt, ch, dimm, NULL)) {
          (*dimmNvList)[dimm].volCap = 0;

          RcDebugPrintWithDevice (SDBG_MINMAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem Unmapped!\n");
          continue;
        }
        //DIMM partitioning code.
        if (Host->var.mem.dfxMemVars.dfxPartitionDDRTDimm == PARTITION_DDRT_DIMM_EN) {
          RcDebugPrintWithDevice (SDBG_MAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Partitioning PMEM dimms based on setup option.\n");

          PartitionDdrt (Host, MemMapData, sckt, ch, dimm);

        } else {
          if (PcdGetBool (PcdUseNvdimmFwInterface))  {
            //If no partition request is found for this dimm, read the current partition information and update the Host structs.
            Status = GetDimmPartitionInfo (Host, sckt, ch, dimm, &fnvBuffer, &MbStatus);
            if (Status == SUCCESS) {
              dimmPartitionList = GetFnvCisBufferDimmPartition (&fnvBuffer);
              (*dimmNvList)[dimm].volCap = (UINT16) ((*dimmPartitionList)[sckt][ch][dimm].volatileCap >> CONVERT_4KB_TO_64MB_GRAN);
              (*dimmNvList)[dimm].nonVolCap = (UINT16) ((*dimmPartitionList)[sckt][ch][dimm].persistentCap >> CONVERT_4KB_TO_64MB_GRAN);
              (*dimmNvList)[dimm].perCap = (*dimmNvList)[dimm].nonVolCap;
              (*dimmNvList)[dimm].WbCachePerCap = 0;
              //Start addresses are in 64byte granularity
              tempData = RShiftUINT64 ((*dimmPartitionList)[sckt][ch][dimm].volatileStart, CONVERT_B_TO_64MB);
              (*dimmNvList)[dimm].volRegionDPA = tempData.lo;
              tempData = RShiftUINT64 ((*dimmPartitionList)[sckt][ch][dimm].persistentStart, CONVERT_B_TO_64MB);
              (*dimmNvList)[dimm].perRegionDPA = tempData.lo;
            } else {
              RcDebugPrintWithDevice (SDBG_MINMAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Communication with the PMEM DIMM failed\n");
              //log data format:
              //Byte 0 (Bit 0 - DIMM Slot Number, Bits 1-3 - Channel Number, Bits 4-6 - Socket Number,Bit 7 - Reserved)
              //Byte 1 - FW MB Opcode,Byte 2 - FW MB Sub-Opcode,Byte 3 - FW MB Status Code
              EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, sckt, ch, dimm, NO_RANK);
              DisableDIMM (Host, sckt, ch, dimm);
            }
          }
        }
        RcDebugPrintWithDevice (SDBG_MAX, sckt, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "\tRaw: %d  Vol:%d  NonVol:%d  Per:%d  Blk:%d\n",
          (*dimmNvList)[dimm].rawCap, (*dimmNvList)[dimm].volCap,
          (*dimmNvList)[dimm].nonVolCap, (*dimmNvList)[dimm].perCap, (*dimmNvList)[dimm].WbCachePerCap);
      }//dimm
    }//ch
  } //socket

  CheckSkuForSupportedMode (Host);

  //Init variables for blk window size and/or controil region window size
  InitializeDdrtDimmChip (Host);
}

BOOLEAN CheckDdrtParitioningSpdBytesDdrt (PSYSHOST Host, UINT8 *sckt, UINT8 *ch, UINT8 *dimm)
{

  /**

  This routine checks SPD bytes 384,385,386,387 for all NVMDIMM dimms in the system.
  If these bytes match in all NVMDIMM dimms, it returns TRUE, FAIL otherwise.

  @param Host  - Pointer to sysHost
  @param sckt  - Pointer to the current socket index
  @param ch    - Pointer to the current ch index
  @param dimm  - Pointer to the current dimm index

  @retval TRUE/FALSE based on the match of the SPD bytes listed above

**/

  UINT8 MaxImc;
  UINT8 scktNum, chNum,dimmNum, i,tempCounter = 0;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  struct dimmDevice (*chdimmList)[MAX_DIMM];
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  MaxImc = GetMaxImc ();
  for (scktNum = 0; scktNum < MAX_SOCKET; scktNum++) {
    if (CheckSocketEnabled (scktNum) == 0) {
      continue;
    }
    channelNvList = GetChannelNvList (Host, scktNum);
    for (chNum = 0; chNum < MaxChDdr ;chNum++) {
      chdimmList = &Host->var.mem.socket[scktNum].channelList[chNum].dimmList;
      if ((*channelNvList)[chNum].enabled == 0) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, scktNum, chNum);

      //For each DIMM
      for (dimmNum = 0; dimmNum < MAX_DIMM; dimmNum++) {
        if ((*DimmNvList)[dimmNum].dimmPresent == 0) {
          continue;
        }

        if ((*channelNvList)[chNum].dimmList[dimmNum].DcpmmPresent == 1) {
          if (tempCounter == 0) {
            *sckt = scktNum;
            *ch = chNum;
            *dimm = dimmNum;
          }
          tempCounter++;
          for (i = 0; i < (MAX_SOCKET * MaxImc); i++) {
            if (Host->var.mem.socket[scktNum].channelList[chNum].dimmList[dimmNum].SPDPartitionRatio[i] !=
                Host->var.mem.socket[*sckt].channelList[*ch].dimmList[*dimm].SPDPartitionRatio[i] ) {
              return FALSE;
            }
          }
        }
      }
    }
  }

  return TRUE;

}

/**

  This routine initializes the DDRT structures and partitions the DDRT DIMMs

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval None

**/
VOID
EvaluateAndPartitionDdrt (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  //Based on setup option dimmManagement, we decide how we populate size infomation for NGN dimms.
  if (Host->var.mem.dfxMemVars.dfxDimmManagement == DIMM_MGMT_CR_MGMT_DRIVER) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"PMem mgmt Driver is available..\n");

    //Initialize NGN dimm by populating the necessary fields in Host struct
    InitializeNGNDIMM (Host, MemMapData);
    if (MemMapData->MemMapState != MEM_MAP_STATE_RESOURCE_CALCULATION_DONE) {

      EFI_STATUS Status;
      MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

      Status = LocateInterface (
        &gMrcPlatformHooksInterfaceGuid,
        0,
        (VOID **)&MrcPlatformHooksInterface
        );
      if (!EFI_ERROR (Status)) {
        MrcPlatformHooksInterface->PlatformUpdateConfig (Host);
      }

      //Validate the CfgCur and CfgIn tables read from the PCD
      ValidateNGNDimmData (Host, MemMapData);

      if (!EFI_ERROR (Status)) {
        MrcPlatformHooksInterface->PlatformValidateConfig (Host);
      }
    }

    // Deal with any memory population POR violations impacting DCPMMs
    HandleUnmappedDcpmms (Host);

    //Apply Partitioning Request
    HandlePartitionRequests (Host, MemMapData);
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"PMem mgmt Driver is NOT available. Using setup options!!\n");
    //Update NVMDIMM related fields in  DimmNVRAM struct
    UpdateDdrtFields (Host, MemMapData);
  }

  //Adjust volap of each channel to align with nearest GB. Witohout this we will be broken when SNC is enabled.
  //HSD 5330197: SNC + 2LM currently broken
  AlignVolCaptoGb (Host);
}

/**

  Calculate the total reserved DDR Cache Size on the system.

  @param[in] Sckt - The socket index to use for calculating the socket DdrCacheSize

  @return The total reserved DDR Cache Size for the input socket index

**/
UINT32
GetSocketDdrCacheSize (
  IN UINT8 Sckt
  )
{
  SYSHOST *Host;
  UINT8   Ch;
  UINT32  TotalDdrCacheSize;
  UINT8   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = GetSysHostPointer ();
  TotalDdrCacheSize = 0;

  //
  // Only check enabled sockets
  //
  if (CheckSocketEnabled (Sckt) == 0) {
    return 0;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    //
    // Only check enabled channels
    //
    if ((*GetChannelNvList (Host, Sckt))[Ch].enabled == 0) {
      continue;
    }
    TotalDdrCacheSize += Host->var.mem.socket[Sckt].channelList[Ch].DdrCacheSize;
  } // Ch

  return TotalDdrCacheSize;
}

/**

  Get the requested DDR Cache Size in units of 64MB

  The requested DDR Cache Size is always a power of 2 and is calculated using the formula:
  2^(33 + DdrCacheSize). This is then converted to 64MB Granularity by right shifting by 26.

  @param[in] CacheSizeCode  Encoded setup value

  @return The requested DDR Cache Size in units of 64MB

**/
UINT32
DecodeDdrCacheSize (
  IN UINT8 CacheSizeCode
  )
{
  switch (CacheSizeCode) {
    case DDR_ENCODED_CACHE_SIZE_8GB:   return DDR_CACHE_SIZE_8GB;
    case DDR_ENCODED_CACHE_SIZE_16GB:  return DDR_CACHE_SIZE_16GB;
    case DDR_ENCODED_CACHE_SIZE_32GB:  return DDR_CACHE_SIZE_32GB;
    case DDR_ENCODED_CACHE_SIZE_64GB:  return DDR_CACHE_SIZE_64GB;
    case DDR_ENCODED_CACHE_SIZE_128GB: return DDR_CACHE_SIZE_128GB;
    case DDR_ENCODED_CACHE_SIZE_256GB: return DDR_CACHE_SIZE_256GB;

    default: return DDR_CACHE_SIZE_NONE;
  }
}

/**

  Get the requested DDR Cache Size in units of 64MB

  @param VOID

  @return The requested DDR Cache Size in units of 64MB

**/
UINT32
GetRequestedDdrCacheSize (
  VOID
  )
{
  SYS_SETUP *Setup;
  UINT32    RequestedDdrCacheSize;

  Setup = GetSysSetupPointer ();

  if (Setup->mem.DdrCacheSize == DDR_ENCODED_CACHE_SIZE_ALL) {
    ///
    /// if DdrCacheSize == DDR_ENCODED_CACHE_SIZE_ALL, this indicates we are in 2LM mode
    /// this function should not be called in such case
    //
    RcDebugPrint (SDBG_MINMAX, "Invalid - partial cache size is being requested when not in 1LM + 2LM mode\n");
    RequestedDdrCacheSize = DDR_CACHE_SIZE_NONE;
  } else if (Setup->mem.DdrCacheSize > DDR_ENCODED_CACHE_SIZE_256GB) {
    RcDebugPrint (SDBG_MINMAX, "Invalid DdrCacheSize option\n");
    RequestedDdrCacheSize = DDR_CACHE_SIZE_NONE;
  } else {
    RequestedDdrCacheSize = DecodeDdrCacheSize (Setup->mem.DdrCacheSize);
  }
  return RequestedDdrCacheSize;
}

/**

  This routine initializes the DDR cache size fields in the structures
  for DIMMs, Channels, and Nodes.  The sizes are calculated from the sizes in
  the rank structures.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to memory map data structure.

  @retval SUCCESS          System is not in mixed 1LM and 2LM mode.
  @retval SUCCESS          DDR size populated successfully for 1LM and 2LM mode.
  @retval FAILURE          Fails to populate DDR size need to downgrade to 2LM mode.

**/
UINT32
PopulateDdrCacheSizeFields (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8                Sckt;
  UINT8                Mc;
  UINT8                Ch;
  UINT8                MaxImc;
  UINT8                NumChPerMc;
  UINT32               ChannelMemSize;
  UINT32               DdrCacheSize;
  IMC_INFO_STRUCT      *Imc;
  CHANNEL_NVRAM_STRUCT (*ChannelNvList)[MAX_CH];
  DDR_CHANNEL_STRUCT   (*ChannelList)[MAX_CH];
  BOOLEAN              Mixed2lm1lmPossible;
  UINT8                MaxChDdr;

  if (Host->var.mem.volMemMode != VOL_MEM_MODE_MIX_1LM2LM) {
    return SUCCESS;
  }

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();
  Mixed2lm1lmPossible = TRUE;
  MaxChDdr = GetMaxChDdr ();

  for (Sckt = 0; Sckt < MAX_SOCKET && Mixed2lm1lmPossible; Sckt++) { // Loop for each CPU socket
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }

    ChannelNvList = GetChannelNvList (Host, Sckt);
    ChannelList = &Host->var.mem.socket[Sckt].channelList;

    // Initialize Mc structures
    for (Mc = 0; Mc < MaxImc && Mixed2lm1lmPossible; Mc++) {
      Imc = &Host->var.mem.socket[Sckt].imc[Mc];

      if (Host->nvram.mem.socket[Sckt].imc[Mc].enabled == 0) {
        continue;
      }

      //
      // For in-tile 2LM, only IMCs with DDRT configured for 2LM will have DDR participating as cache
      //
      if ((Imc->volSize == 0 && Imc->WbCachePerSize == 0) && !MemMapData->MemMapSocket[Sckt].XTile2lm) {
        continue;
      }

      //
      // MCs without DDR cannot be in a cached memory mode
      //
      if (Imc->memSize == 0) {
        continue;
      }

      // Initialize Imc memory size vairables
      for (Ch = 0; Ch < NumChPerMc; Ch++) {
        ChannelMemSize = GetChannelMemSize (Sckt, Ch + Imc->imcChannelListStartIndex);
        DdrCacheSize = GetRequestedDdrCacheSize ();

        if (ChannelMemSize == 0) {
          continue;
        }

        if (ChannelMemSize <= DdrCacheSize) {
          //
          // If any one channel is smaller than/equal to the requested DdrCacheSize then mixed 1LM + 2LM is not possilble
          //
          Mixed2lm1lmPossible = FALSE;
          break;
        }

        Host->var.mem.socket[Sckt].channelList[Ch + Imc->imcChannelListStartIndex].DdrCacheSize = DdrCacheSize;
      } // Ch
    } // Mc
  } // Sckt

  if (!Mixed2lm1lmPossible) {
    //
    // Downgrade to the regular 2LM configuration
    //
    Host->var.mem.volMemMode = VOL_MEM_MODE_2LM;
    RcDebugPrint (SDBG_MINMAX, "1LM+2LM Not Possible\n");

    //
    // Clean up the DdrCacheSize fields
    //
    for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) { // Loop for each CPU socket
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }
      ChannelList = &Host->var.mem.socket[Sckt].channelList;
      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        (*ChannelList)[Ch].DdrCacheSize = 0;
      } // Ch
    } // Sckt
    return FAILURE;
  }
  return SUCCESS;
}

/**

  This routine fills in the memory topology for the given memory controller

  @param[in]      Host        - Pointer to sysHost
  @param[in]      Socket      - Socket # of the memory controller
  @param[in]      Imc         - Imc # of the memory controller
  @param[in,out]  *ImcDimmPop - Pointer to the Memory topology structure to populate

  @retval none

**/
VOID
GetImcDdrtTopology (
  IN      PSYSHOST          Host,
  IN      UINT8             Socket,
  IN      UINT8             Imc,
  IN OUT  DDRT_IMC_TOPOLOGY *ImcDimmPop
  )
{
  UINT8                   Channel;
  UINT8                   McChannel;
  UINT8                   MaxChPerMc;
  UINT8                   StartingChannel;
  UINT8                   EndChannel;
  UINT8                   Dimm;
  MEM_VAR_STRUCT          *Mem;
  CHANNEL_NVRAM_STRUCT    (*ChannelNvList)[MAX_CH];
  DDR_CHANNEL_STRUCT      (*ChannelList)[MAX_CH];
  DIMM_DEVICE_INFO_STRUCT (*ChDimmList)[MAX_DIMM];
  DIMM_NVRAM_STRUCT       (*DimmNvList)[MAX_DIMM];

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "GetImcDdrtTopology Start IMC %x\n", Imc);
  Mem = &Host->var.mem;

  if (IS_DDR_2LM_CACHE (Host)) {
    // Set 2LM flag
    ImcDimmPop->Modes = MEM_MODE_MM;
  } else {
    ImcDimmPop->Modes = 0;
  }

  ImcDimmPop->NumDdrt = 0;

  MaxChPerMc = GetNumChannelPerMc ();
  ChannelNvList = GetChannelNvList(Host, Socket);
  ChannelList = &Mem->socket[Socket].channelList;

  StartingChannel = MaxChPerMc * Imc;
  EndChannel = MaxChPerMc * (Imc + 1);

  ImcDimmPop->Dpc = 0;  // Each nibble of Dpc will store the # of DIMMs in one channel of the iMC

  for (Channel = StartingChannel; Channel < EndChannel; Channel++) {
    McChannel = Channel % MaxChPerMc;
    if (McChannel != 0) {
      // Shift one nibble to populate the next channel
      ImcDimmPop->Dpc <<= 4;
    }
    if ((*ChannelNvList)[Channel].enabled == 0) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        ImcDimmPop->DimmSlot[McChannel][Dimm].Type = NoDimm;
        ImcDimmPop->DimmSlot[McChannel][Dimm].Capacity = 0;
        ImcDimmPop->DimmSlot[McChannel][Dimm].NonVolCap = 0;
      }
      continue;
    }
    if ((*ChannelList)[Channel].NonVolSize != 0) {
      // Set AD flag
      ImcDimmPop->Modes |= MEM_MODE_AD;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    ChDimmList = &Mem->socket[Socket].channelList[Channel].dimmList;

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        ImcDimmPop->DimmSlot[McChannel][Dimm].Type = NoDimm;
        ImcDimmPop->DimmSlot[McChannel][Dimm].Capacity = 0;
        ImcDimmPop->DimmSlot[McChannel][Dimm].NonVolCap = 0;
        continue;
      }
      // Increment DPC for this channel
      ImcDimmPop->Dpc++;

      if ((*DimmNvList)[Dimm].DcpmmPresent) {
        ImcDimmPop->DimmSlot[McChannel][Dimm].Type = Ddrt;
        ImcDimmPop->NumDdrt++;
        // Convert capacity to GB
        ImcDimmPop->DimmSlot[McChannel][Dimm].Capacity = (*DimmNvList)[Dimm].rawCap >> 4;
        ImcDimmPop->DimmSlot[McChannel][Dimm].NonVolCap = (*DimmNvList)[Dimm].nonVolCap >> 4;
      } else {
        // Convert capacity to GB
        ImcDimmPop->DimmSlot[McChannel][Dimm].Capacity = (*ChDimmList)[Dimm].memSize >> 4;
        ImcDimmPop->DimmSlot[McChannel][Dimm].NonVolCap = 0;

        if ((*DimmNvList)[Dimm].lrDimmPresent) {
          // LRDIMM
          // Check 3DS
          if ((*ChannelNvList)[Channel].encodedCSMode) {
            ImcDimmPop->DimmSlot[McChannel][Dimm].Type = Lrdimm3ds;
          } else {
            ImcDimmPop->DimmSlot[McChannel][Dimm].Type = Lrdimm;
          }
        } else {
          // RDIMM
          // Check 3DS
          if ((*ChannelNvList)[Channel].encodedCSMode) {
            ImcDimmPop->DimmSlot[McChannel][Dimm].Type = Rdimm3ds;
          } else {
            ImcDimmPop->DimmSlot[McChannel][Dimm].Type = Rdimm;
          }
        }
      }
    }
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "GetImcDdrtTopology End\n");
}
#endif // DDRT_SUPPORT

/**

  This routine initializes the memory size fields in the structures
  for DIMMs, Channels, and Nodes.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to memory map private data structure.

  @retval None

**/
VOID
PopulateMemorySizeFields (
  IN  PSYSHOST  Host,
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8                   Sckt;
  UINT8                   Ch;
  UINT8                   i;
  UINT8                   Dimm;
  UINT8                   Mc;
  UINT8                   MaxImc;
  UINT8                   NumChPerMc;
  MEM_NVRAM_STRUCT        *NvramMem;
  MEM_VAR_STRUCT          *Mem;
  CHANNEL_NVRAM_STRUCT    (*ChannelNvList)[MAX_CH];
  DDR_CHANNEL_STRUCT      (*ChannelList)[MAX_CH];
  DDR_RANK_STRUCT         (*rankList)[MAX_RANK_DIMM];
  DIMM_NVRAM_STRUCT       (*DimmNvList)[MAX_DIMM];
  DIMM_DEVICE_INFO_STRUCT (*ChDimmList)[MAX_DIMM];
  IMC_INFO_STRUCT         *Imc;
  struct imcNvram         *ImcNv;
  UINT32                  TotalHbmSize;
  UINT32                  ChMemSize;
  UINT32                  ScktMemSize;
  SYS_SETUP               *Setup;
  UINT8                   MaxChDdr;
  UINT8                   NvDimmCtrRegEnChMap = 0;
  BOOLEAN                 NvDimmCtrRegEn;
  UINT32                  DimmRankEnMap;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  NvramMem = &Host->nvram.mem;
  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();
  Mem = &Host->var.mem;

  Mem->memSize = 0;
  Mem->NVmemSize = 0;

  // Clear all mem sizes
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {

    Mem->socket[Sckt].memSize = 0;
    Mem->socket[Sckt].volSize = 0;
    Mem->socket[Sckt].perSize = 0;
    Mem->socket[Sckt].WbCachePerSize = 0;
    Mem->socket[Sckt].NVmemSize = 0;
    ChannelList = &Mem->socket[Sckt].channelList;
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      (*ChannelList)[Ch].memSize = 0;
      (*ChannelList)[Ch].volSize = 0;
      (*ChannelList)[Ch].NonVolSize = 0;
      (*ChannelList)[Ch].perSize = 0;
      (*ChannelList)[Ch].WbCachePerSize = 0;
      (*ChannelList)[Ch].DdrCacheSize = 0;
      (*ChannelList)[Ch].ddr4RemSize = 0;
      (*ChannelList)[Ch].volRemSize = 0;
      (*ChannelList)[Ch].perRemSize = 0;
      (*ChannelList)[Ch].ctrlRemSize = 0;
      (*ChannelList)[Ch].WbCachePerRemSize = 0;
      (*ChannelList)[Ch].NVmemSize = 0;

      ChDimmList = &Mem->socket[Sckt].channelList[Ch].dimmList;
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        (*ChDimmList)[Dimm].memSize = 0;
        (*ChDimmList)[Dimm].UnmappedMemSize = 0;
        (*ChDimmList)[Dimm].NVmemSize = 0;
      }
    }
  }

  TotalHbmSize = PopulateHbmSizeFieldsEx (MemMapData);

  // For each socket
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) { // Loop for each CPU socket
    if (!CheckSocketEnabled (Sckt)) {
      continue;
    }

    ChannelNvList = GetChannelNvList(Host, Sckt);
    ChannelList = &Mem->socket[Sckt].channelList;

    // For each channel
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) continue;

      ChDimmList = &Mem->socket[Sckt].channelList[Ch].dimmList;
      DimmNvList = GetDimmNvList (Host, Sckt, Ch);

      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }

        rankList = GetRankNvList (Host, Sckt, Ch, Dimm);

        // DIMM size equals the sum of rank sizes in the DIMM
        // Note: Ranksize may be zero if it is the spare rank
        DimmRankEnMap = 0;

        for (i = 0;  i < (*DimmNvList)[Dimm].numDramRanks; i++) {
          //
          // Check if this rank is enabled and not mapped out
          //
          if (!(*DimmNvList)[Dimm].mapOut[i] && (*rankList)[i].enabled) {
            DimmRankEnMap |= (BIT0 << i);
          } else {
            //
            // Check if DDR DIMM rank is enabled
            //
            if (!(*DimmNvList)[Dimm].DcpmmPresent && (*rankList)[i].enabled) {
              //
              // DDR DIMM mapped out (we need DIMM size calculated for SMBIOS Type 17)
              //
              (*ChDimmList)[Dimm].UnmappedMemSize = (*ChDimmList)[Dimm].UnmappedMemSize + (*rankList)[i].rankSize;
            }
            continue;
          }

          if (!(*DimmNvList)[Dimm].DcpmmPresent) {
            (*ChDimmList)[Dimm].memSize = (*ChDimmList)[Dimm].memSize + (*rankList)[i].rankSize;
            if (PcdGetBool (PcdNvDimmEn)) {
              RcDebugPrintWithDevice (0xFFFF, Sckt, Ch, Dimm, NO_SUBCH, i, NO_STROBE, NO_BIT,
                "ranksize = %x, NVranksize = %x (rank %a)\n", (*rankList)[i].rankSize, (*rankList)[i].NVrankSize, (DimmRankEnMap & (BIT0 << i)) ? "enabled" : "disabled");
              (*ChDimmList)[Dimm].NVmemSize = (*ChDimmList)[Dimm].NVmemSize + (*rankList)[i].NVrankSize;
            }
          }
          // Reset remaining size for rank
          if ((PcdGetBool (PcdNvDimmEn)) && ((*rankList)[i].NVrankSize != 0)) {
            (*rankList)[i].remSize = 0; // This is needed to in RankInterleave so that first normal Dimm ranks are interleaved and then NVDIMMs.
          } else {
            // Reset remaining size for rank
            (*rankList)[i].remSize = (*rankList)[i].rankSize;
          }
        }

        //
        // Check if DDR DIMM should be mapped
        //
        if (!(*DimmNvList)[Dimm].DcpmmPresent && !DimmRankEnMap) {
          continue;
        }

        // Channel memory sizes equals the sum of the all DIMM sizes on this channel
        (*ChannelList)[Ch].memSize += (*ChDimmList)[Dimm].memSize;
        if (PcdGetBool (PcdNvDimmEn)) {
          (*ChannelList)[Ch].NVmemSize = (*ChannelList)[Ch].NVmemSize + (*ChDimmList)[Dimm].NVmemSize;
        }
#ifdef DDRT_SUPPORT
        if ((*DimmNvList)[Dimm].DcpmmPresent) {
           //Skip if DIMM is disabled
           if (!IsNvDimmEnabled (Host, Sckt, Ch, Dimm)) {
             continue;
           }
        }

        //
        // If DIMM is enabled, but is going to be unmapped, check if control region should be mapped
        //
        if ((*DimmNvList)[Dimm].DcpmmPresent && IsNvDimmUnmapped (Sckt, Ch, Dimm, NULL)) {
          if (IsNvDimmCtrLRegionToBeMapped (Sckt, Ch, Dimm)) {
            NvDimmCtrRegEnChMap |= (BIT0 << Ch);
          }
        }

        // Fill in appropriate fields based on dfxDimmManagement question
        (*ChannelList)[Ch].volSize += (*DimmNvList)[Dimm].volCap;
        if (Host->var.mem.dfxMemVars.dfxDimmManagement == DIMM_MGMT_BIOS_SETUP) {
          (*ChannelList)[Ch].perSize += (*DimmNvList)[Dimm].perCap;
          (*ChannelList)[Ch].WbCachePerSize += (*DimmNvList)[Dimm].WbCachePerCap;
        } else {
          //
          // This flow in future shall include the WB cache partition request from DDRT DIMM mgmt.
          //
          if ((*DimmNvList)[Dimm].nonVolCap != 0) {
            (*ChannelList)[Ch].perSize += (*DimmNvList)[Dimm].rawCap - (*DimmNvList)[Dimm].volCap; // Needed to ensure space available for PMEM Hole
            (*ChannelList)[Ch].NonVolSize += (*DimmNvList)[Dimm].nonVolCap;
          }
        }
#endif // DDRT_SUPPORT
      } // DIMM

      //Init remsize variables for all the modes.
      ChMemSize = GetChannelMemSize (Sckt, Ch);
      if (PcdGetBool (PcdNvDimmEn)) {
        //      (*ChannelList)[Ch].ddr4RemSize = (*ChannelList)[Ch].memSize - (*ChannelList)[Ch].NVmemSize;
        (*ChannelList)[Ch].ddr4RemSize = ChMemSize;
        (*ChannelList)[Ch].NVmemRemSize = (*ChannelList)[Ch].NVmemSize;
      } else {
        (*ChannelList)[Ch].ddr4RemSize = ChMemSize;
      }
      (*ChannelList)[Ch].volRemSize = (*ChannelList)[Ch].volSize;
      (*ChannelList)[Ch].perRemSize = (*ChannelList)[Ch].perSize;
      (*ChannelList)[Ch].ctrlRemSize = (*ChannelList)[Ch].ctrlSize;
      (*ChannelList)[Ch].WbCachePerRemSize = (*ChannelList)[Ch].WbCachePerSize;

      // The memory size for this socket equals the sum of all Channel sizes on this socket
      Mem->socket[Sckt].memSize += ChMemSize;
      Mem->socket[Sckt].volSize = Mem->socket[Sckt].volSize + (*ChannelList)[Ch].volSize;
      Mem->socket[Sckt].perSize = Mem->socket[Sckt].perSize + (*ChannelList)[Ch].perSize;
      Mem->socket[Sckt].WbCachePerSize = Mem->socket[Sckt].WbCachePerSize + (*ChannelList)[Ch].WbCachePerSize;
      if (PcdGetBool (PcdNvDimmEn)) {
        Mem->socket[Sckt].NVmemSize = Mem->socket[Sckt].NVmemSize + (*ChannelList)[Ch].NVmemSize;
      }
    } // Ch

    // Initialize Mc structures
    for (Mc = 0; Mc < MaxImc; Mc++) {
      ImcNv = &NvramMem->socket[Sckt].imc[Mc];
      Imc = &Mem->socket[Sckt].imc[Mc];

      // Initialize mc nvram structure
      ImcNv->enabled = 0;

      // Initialize mc mem structure
      Imc->imcNum = Mc;
      Imc->memSize = 0;
      Imc->volSize = 0;
      Imc->NonVolSize = 0;
      Imc->perSize = 0;
      Imc->WbCachePerSize = 0;
      Imc->NVmemSize = 0;

      Imc->imcChannelListStartIndex = Mc * NumChPerMc;

      NvDimmCtrRegEn = FALSE;

      // Initialize Imc memory size vairables
      for (Ch = 0; Ch < NumChPerMc; Ch++) {
        Imc->memSize = Imc->memSize + GetChannelMemSize (Sckt, Ch + Imc->imcChannelListStartIndex);
        Imc->volSize = Imc->volSize + Host->var.mem.socket[Sckt].channelList[Ch + Imc->imcChannelListStartIndex].volSize;
        Imc->NonVolSize = Imc->NonVolSize + Host->var.mem.socket[Sckt].channelList[Ch + Imc->imcChannelListStartIndex].NonVolSize;
        Imc->perSize = Imc->perSize + Host->var.mem.socket[Sckt].channelList[Ch + Imc->imcChannelListStartIndex].perSize;
        Imc->WbCachePerSize = Imc->WbCachePerSize + Host->var.mem.socket[Sckt].channelList[Ch + Imc->imcChannelListStartIndex].WbCachePerSize;
        if (PcdGetBool (PcdNvDimmEn)) {
          Imc->NVmemSize = Imc->NVmemSize + Host->var.mem.socket[Sckt].channelList[Ch + Imc->imcChannelListStartIndex].NVmemSize;
        }
        if (!NvDimmCtrRegEn && (NvDimmCtrRegEnChMap & (BIT0 << MEM_IMCCH_TO_SKTCH(Mc, Ch)))){
          NvDimmCtrRegEn = TRUE;
        }
      }// for channel per mc

      //
      // Enable mc if
      // 1. memsize not zero, or
      // 2. memory available in other MCs and aep present in current mc.
      // 3. memory available in other MCs and aep present and mapped out, but still control region mapped (e.g.SKU Limit Violation)
      //
      if ((Imc->memSize != 0) ||
          ((GetSocketMemSize (Sckt) != 0) && (Imc->volSize | Imc->NonVolSize | Imc->perSize | Imc->WbCachePerSize)) ||
          (NvDimmCtrRegEn)) {
        ImcNv->enabled = 1;
      }// if Enable mc

    }// for mc

    if (PcdGetBool (PcdNvDimmEn)) {
      Mem->NVmemSize = Mem->NVmemSize + Mem->socket[Sckt].NVmemSize;
    }

  } // Sckt

#ifdef DDRT_SUPPORT
  do {
    // If volMemMode set to 2LM, check platform compliance and force 1LM if necessary
    if (IS_DDR_2LM_OR_MIXED (Host)) {
      AdjustVolatileMemMode (Host, MemMapData);
    }
  } while (PopulateDdrCacheSizeFields (Host, MemMapData) != SUCCESS);
#endif // DDRT_SUPPORT

  // Fill in memSize based on current volMemMode
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (!CheckSocketEnabled (Sckt)) {
      continue;
    }

#ifdef DDRT_SUPPORT
    if (Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
      //
      // In 2LM + 1LM configurations:
      // total system memory size = PMem capacity (vol + per + wb) + DDR capacity - DDR cache + HBM capacity (if exists).
      //
      ScktMemSize = GetSocketMemSize (Sckt);
      Mem->memSize += Mem->socket[Sckt].volSize + Mem->socket[Sckt].perSize + Mem->socket[Sckt].WbCachePerSize + ScktMemSize - GetSocketDdrCacheSize (Sckt);
      RcDebugPrintWithDevice (
        SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Total mem size = %d; vol size = %d; per size = %d; per wb size = %d; mem size = %d; ddr cache size = %d\n",
        Mem->memSize, Mem->socket[Sckt].volSize,
        Mem->socket[Sckt].perSize,
        Mem->socket[Sckt].WbCachePerSize,
        ScktMemSize,
        GetSocketDdrCacheSize (Sckt)
        );
    } else if (IS_DDR_2LM_CACHE(Host)) {
      //
      // In 2LM DDR$PMem configuration:
      // total system memory size = PMem capacity (vol + per + wb) + HBM capacity (if exists).
      //
      Mem->memSize += Mem->socket[Sckt].volSize + Mem->socket[Sckt].perSize + Mem->socket[Sckt].WbCachePerSize;
      RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Total mem size = %d; vol size = %d; per size = %d; per wb size = %d\n",
        Mem->memSize, Mem->socket[Sckt].volSize,
        Mem->socket[Sckt].perSize,
        Mem->socket[Sckt].WbCachePerSize
        );
    } else
#endif // DDRT_SUPPORT
    {
      //
      // In 2LM HBM$DDR configuration:
      // total system memory size = PMem per capacity + DDR capacity.
      //
      // In 1LM configuration:
      // total system memory size = DDR capacity + PMem per capacity + HBM capacity (if exists).
      //
      ScktMemSize = GetSocketMemSize (Sckt);
      Mem->memSize += ScktMemSize + Mem->socket[Sckt].perSize;
      RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Total mem size = %d; mem size = %d; per size = %d\n", Mem->memSize, ScktMemSize, Mem->socket[Sckt].perSize);
    }

  } // Sckt

  //
  // Count HBM size into total memory size.
  //
  if (!IS_HBM_2LM_CACHE_DDR(Host)) {
    Mem->memSize += TotalHbmSize;
  }

  //
  // Halt if there is no memory
  //
  if (Mem->memSize == 0) {
    RcDebugPrintWithDevice (0xFFFF, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "No Memory to map!\n");
    OutputError (ERR_NO_MEMORY, ERR_NO_MEMORY_MINOR_ALL_CH_DISABLED, 0xFF, 0xFF, 0xFF, 0xFF);
  }

  CheckTwoWayNmCacheSupported ();
} // PopulateMemorySizeFields

/**

  Encode Limit field for all DRAM rules.
  Limit is encoded such that valid address range <= Limit.

  @param Limit - Limit to encode

  @retval Encoded Limit

**/
UINT32
EFIAPI
SadTadEncodeLimit (
  UINT32 Limit
  )
{
  return (Limit - 1);
}

/**

  Encode MC/TAD target interleave granularity

  @param[in] TgtGranularity  Specifies the MC granularity

  @retval Encoded granularity

**/
UINT8
EFIAPI
EncodeTadTargetInterleaveGranularity (
  IN UINT8 TgtGranularity
  )
{
  UINT8 EncodedGranularity;

  switch (TgtGranularity) {
  case INTERLEAVE_GRANULARITY_64B:
    EncodedGranularity = 0;
    break;
  case INTERLEAVE_GRANULARITY_256B:
    EncodedGranularity = 1;
    break;
  case INTERLEAVE_GRANULARITY_4KB:
    EncodedGranularity = 2;
    break;
  case INTERLEAVE_GRANULARITY_2KB:
    EncodedGranularity = 7;
    break;
  default:
    EncodedGranularity = 0;
    break;
  }

  return (EncodedGranularity);
}

/**

  Encode interleave mode granularity

  @param[in] InterleaveMode  DRAM rule target/interleave mode

  @retval Encoded granularity

**/
UINT8
EncodeSadInterleaveModeGranularity (
  IN UINT8 InterleaveMode
  )
{
  UINT8 EncodedGranularity;

  switch (InterleaveMode) {
  case INTERLEAVE_GRANULARITY_64B:
    EncodedGranularity = 0;
    break;
  case INTERLEAVE_GRANULARITY_256B:
    EncodedGranularity = 1;
    break;
  case INTERLEAVE_GRANULARITY_4KB:
    EncodedGranularity = 2;
    break;
  case INTERLEAVE_GRANULARITY_8KB:
    EncodedGranularity = 3;
    break;
  default:
    EncodedGranularity = 0;
    break;
  }

  return (EncodedGranularity);
}

/**

  Description: Return interleave granularity supported by the CPU type.

  @param Host - Pointer to sysHost

  @retval SAD/TAD granularity (64MB is the only granularity currently supported).

**/
UINT16
GetSadTadGranularity (
  PSYSHOST Host
  )
{
  return SAD_GRAN_64MB;
}

/**

  Encode TAD channel "ways" for TAD rule CSR in imc (memory controller)

  @param[in] ways  - ways to encode

  @retval Encoded ways

**/
UINT8
EFIAPI
ImcTadEncodeChWays (
  IN UINT8 ways
  )
{
  switch (ways) {
  case 1:
    ways = 0;
    break;
  case 2:
    ways = 1;
    break;
  case 3:
    ways = 2;
    break;
  case 4:
    ways = 4;
    break;
  case 6:
    ways = 5;
    break;
  case 8:
    ways = 3;
    break;
  default:
    ways = 0;
  }
  return ways;
}

/**

  Encode TAD socket "ways" for TAD rule CSR.

  @param[in] ways  - ways to encode

  @retval Encoded ways

**/
UINT8
EFIAPI
TadEncodeSkWays (
  IN UINT8 ways
  )
{
  switch (ways) {
  case 1:
    ways = 0;
    break;
  case 2:
    ways = 1;
    break;
  case 4:
    ways = 2;
    break;
  case 8:
    ways = 3;
    break;
  case 16:
    ways = 4;
    break;
  default:
    ways = 0;
  }
  return ways;
}

/**

  Find the MC index to use for calculating channel ways.
  imcInterBitmap must be a non-zero value in input.

  @param imcInterBitmap : bitmap of IMCs in the interleave
  @retval IMC number to use for calculating channel ways

**/
UINT8
EFIAPI
GetMcIndexFromBitmap (
  UINT8 imcInterBitmap
  )
{
  return (UINT8) LowBitSet32 ((UINT32) imcInterBitmap);
}

/**

  Find the interleave ways from the bitmap

  @param bitmap : bitmap of the interleave
  @retval number of ways interleaved

**/
UINT8
CalculateWaysfromBitmap (
  UINT8 bitmap
  )
{
  UINT8 i, channelWays;
  UINT8 MaxChPerMc;

  channelWays = 0;
  MaxChPerMc = GetNumChannelPerMc ();

  //Calculate Channel ways from the SAD entry (Channel interleave already calculated during SAD)
  for (i = 0; i < MaxChPerMc; i++) {
    if (bitmap & (BIT0 << i)) {
      channelWays++;
    }
  }
  return channelWays;
}


/**

  Find NM interleave ways.

  @param sckt  - Socket number
  @param mc    - MC index

  @retval NM interleave ways

**/
UINT8
FindNmInterleaveWays (
  UINT8     sckt,
  UINT8     mc
  )
{
  UINT8 counter = 0, ch;
  UINT8 MaxChPerMc;

  MaxChPerMc = GetNumChannelPerMc ();

  //Scan through all the channels in this mc
  for (ch = 0; ch < MaxChPerMc; ch++) {
    if (GetChannelMemSize (sckt, (mc * MaxChPerMc) + ch) != 0) {
      counter++;
    }
  }

  return counter;
}

/**

  Get the number of Channel Ways from the input SAD_TABLE Entry

  @param SadEntry - Pointer to the SAD Table entry to process

  @return The number of channel ways for the input SadEntry

**/
UINT8
EFIAPI
GetChannelWays (
  SAD_TABLE *SadEntry
  )
{
  UINT8 ChannelWays;

  switch (SadEntry->type) {
  case MemType1lmDdr :
    ChannelWays = SadEntry->NmChWays;
    break;
#ifdef DDRT_SUPPORT
  case MemType1lmAppDirect :
  case MemType1lmAppDirectReserved :
  case MemType2lmDdrCacheMemoryMode :
  case MemType2lmDdrWbCacheAppDirect :
  case MemType1lmCtrl :
#endif // DDRT_SUPPORT
  case MemType2lmHbmCacheDdr:
  case MemType2lmHbmCacheMemoryMode:
    ChannelWays = SadEntry->FmChWays;
    break;
  default:
    ChannelWays = 0;
    break;
  }

  return ChannelWays;
}

/**

  Get the ChannelInterBitmap for the input SAD_TABLE Entry

  @param SadEntry - Pointer to the SAD Table entry to process
  @param McIndex  - The McIndex for the requested ChannelInterBitmap

  @return The Channel Interleave Bitmap for the input SadEntry at the requested McIndex

**/
UINT8
GetChannelInterBitmap (
  SAD_TABLE *SadEntry,
  UINT8     McIndex
  )
{
  UINT8 ChInter;

  switch (SadEntry->type) {
  case MemType1lmDdr:
    ChInter = SadEntry->channelInterBitmap[McIndex];
    break;
#ifdef DDRT_SUPPORT
  case MemType1lmAppDirect:
  case MemType1lmAppDirectReserved :
  case MemType2lmDdrCacheMemoryMode:
  case MemType2lmDdrWbCacheAppDirect :
  case MemType1lmCtrl :
#endif // DDRT_SUPPORT
  case MemType2lmHbmCacheDdr:
  case MemType2lmHbmCacheMemoryMode:
    ChInter = SadEntry->FMchannelInterBitmap[McIndex];
    break;
  default:
    ChInter = 0;
    break;
  }

  return ChInter;
}

/**

  Find minimum NM size per channel for this MC to nearest power of 2.

  @param Host  - Pointer to sysHost
  @param Sckt  - Socket number
  @param Mc    - MC index

  @retval Min NM size per channel for this MC

**/
UINT32
FindMinNmChSizePwrOf2 (
  IN PSYSHOST  Host,
  IN UINT8     Sckt,
  IN UINT8     Mc
  )
{
  UINT8               Ch;
  UINT8               NumChPerMc;
  UINT32              ChMemSize;
  UINT32              MinNmSize = MAX_SIZE_INVALID;
  struct channelNvram (*ChannelNvList)[MAX_CH];

  NumChPerMc = GetNumChannelPerMc ();
  ChannelNvList = GetChannelNvList (Host, Sckt);

  //
  // Scan each channel for the smallest NM size
  //
  for (Ch = Mc * NumChPerMc; Ch < ((Mc * NumChPerMc) + NumChPerMc); Ch++) {

    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    ChMemSize = GetChannelMemSize (Sckt, Ch);
    if ((ChMemSize != 0) && (ChMemSize < MinNmSize)) {
      MinNmSize = ChMemSize;
    }
  }

  if (MinNmSize == MAX_SIZE_INVALID) {
    MinNmSize = 0x0;
  }

  //
  // Insure that size returned is a power of 2
  //
  MinNmSize = ((UINT16) (GetPowerOfTwox32 (MinNmSize)));

  return MinNmSize;
}


/** Calculate minimum near memory size per socket.

  Based on the 2LM config mask option, calculate minimum per channel NM size in each MC or
  minimum per MC NM size across all MCs in the socket.

  @param[in] Host   Pointer to sysHost
  @param[in] MemMapData Pointer to the memory map private data structure
  @param[in] Sckt   Socket Id

  @retval None

**/
UINT32
CalculateMinNmSizeForMask (
  IN  PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Sckt
  )
{
  UINT32                    ChMemSize;
  UINT32                    MinNmSizeMc;
  UINT32                    MinNmSizeCh;
  UINT32                    MinNmSizeSkt;
  UINT8                     Ch = 0;
  UINT8                     MaxImc;
  UINT8                     NumChPerMc;
  UINT8                     McIndex = 0;
  UINT8                     NmChCount;

  if (Host->var.mem.CacheMemType == CACHE_TYPE_HBM_CACHE_DDR) {
    MaxImc = GetMaxImcPerIoHbm ();
    NumChPerMc = GetNumChannelPerMcHbm ();
  } else {
    MaxImc = GetMaxImc ();
    NumChPerMc = GetNumChannelPerMc ();
  }

  MinNmSizeSkt = MAX_SIZE_INVALID;

  //
  // Safe option 2LM Configuration Mask
  //
  if (Host->var.mem.dfxMemVars.dfxCfgMask2LM == CFG_MASK_2LM_NORMAL) {
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      //
      // No 2LM or PMEM$ on this memory controller
      //
      if (!IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lm)) {
        continue;
      }
      //
      //  Determine minimal power of 2 NM size for this MC
      //
      MinNmSizeMc = FindMinNmSize (Host, MemMapData, Sckt, McIndex);
      //
      // If there is NM on this memory controller, then adjust the minimum NM socket size if necessary
      //
      if (MinNmSizeMc) {
        if (MinNmSizeMc < MinNmSizeSkt) {
          MinNmSizeSkt = MinNmSizeMc;
        }
      }
    }
  } else {
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      //
      // No 2LM or PMEM$ on this memory controller
      //
      if (!IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lm)) {
        continue;
      }

      MinNmSizeCh = 0;
      MinNmSizeMc = 0;
      NmChCount = 0;

      for (Ch = 0; Ch < NumChPerMc; Ch++) {
        if (Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
          ChMemSize = Host->var.mem.socket[Sckt].channelList[(McIndex * NumChPerMc) + Ch].DdrCacheSize;
        } else {
          ChMemSize = GetNmChannelMemSize (Host, MemMapData, Sckt, (McIndex * NumChPerMc) + Ch);
        }

        //
        // No near memory on this channel
        //
        if (ChMemSize == 0) {
          continue;
        }

        //
        // Increment count of populated NM channels
        //
        NmChCount++;

        //
        // Find minimal NM population on this memory controller channels
        //
        if (MinNmSizeCh == 0) {
          MinNmSizeCh = ChMemSize;
        } else if (ChMemSize < MinNmSizeCh) {
          MinNmSizeCh = ChMemSize;
        }
      }

      //
      // Compute the minimum NM size available for this memory controller
      //
      MinNmSizeMc = MinNmSizeCh * NmChCount;
      //
      // If there is NM on this memory controller, then adjust the minimum NM socket size if necessary
      //
      if (MinNmSizeMc != 0) {
        if (MinNmSizeMc < MinNmSizeSkt) {
          MinNmSizeSkt = MinNmSizeMc;
        }
      }
    }
  }

  if (MinNmSizeSkt == MAX_SIZE_INVALID) {
    return 0;
  }

  return MinNmSizeSkt;
}

UINT8
EFIAPI
RirEncodeRirWays (UINT8 ways)
/**

  Encode RIR "ways" for RIR rule CSR.

  @param ways  - ways to encode

  @retval Encoded ways

**/
{
  switch (ways) {
  case 1:
    ways = 0;
    break;
  case 2:
    ways = 1;
    break;
  case 4:
    ways = 2;
    break;
  case 8:
    ways = 3;
    break;
  default:
    ways = 0;
  }

  return ways;

}

/**
  HSD-4928083: Unused SAD Rules should have limit set

  Adjust the Limit of the unused SAD rules to the limit of the
  last used SAD rule.

  @param[in,out] Host          Pointer to sysHost.
  @param[in]     MemMapData    Pointer to the memory map private data structure.

  @retval N/A

**/
VOID
SetLimitForUnusedSadRules (
  IN OUT PSYSHOST         Host,
  IN     MEMORY_MAP_DATA  *MemMapData
  )
{
  UINT8                SocketNum;
  UINT8                SadIndex;
  UINT8                BeginSadIndex;
  UINT8                EndSadIndex;
  UINT8                ClusterId;
  SOCKET_INFO_STRUCT   *Socket;

  //
  // HSD-4928083:Unused SAD Rules should have limit set
  // Parse through each socket to set unused SAD rules' limit
  // to the limit of the last used SAD rule
  //
  for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {
    if (CheckSocketEnabled (SocketNum) == 0) {
      continue;
    }
    Socket = &Host->var.mem.socket[SocketNum];
    for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
      BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
      EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
      if (Socket->SAD[BeginSadIndex].Enable == 0) {
        break;
      }

      for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
        if (Socket->SAD[SadIndex].Enable == 0) {
          break;
        }
        if ((Socket->SAD[SadIndex].Enable == 0) && (SadIndex > 0)) {
          Socket->SAD[SadIndex].Limit = Socket->SAD[SadIndex - 1].Limit;
        }
      } // SadIndex
    } // ClusterId
  } // SocketNum

  return;
}

VOID
DisplayMemoryMap (PSYSHOST Host, MEMORY_MAP_DATA *MemMapData)
/**

  This routine provides a hook to add routines to display memory map created.

  @param Host  - Pointer to sysHost
  @param MemMapData - Pointer to the memory map private data structure

  @retval N/A

**/
{

  DisplaySadTable (Host, MemMapData);
  DisplayTadTable (Host, MemMapData);
}

UINT32
UpdateMemMapState (PSYSHOST Host, MEMORY_MAP_DATA *MemMapData)
/**

  This routine Updates the memory mapping state and returns proper status.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to memory map private data structure

  @retval status

**/
{

  //if (Host->var.mem.ResourceCalculationDone)
  if (MemMapData->MemMapState == MEM_MAP_STATE_RESOURCE_CALCULATION_DONE) {
    return SUCCESS;
  } else {
    //If we are here it means, that we have completed initial run of memmap and found that we have all silicon resources
    //So set the resourceCalcDone variable and rerun the memory mapping algorithm.
    //Host->var.mem.ResourceCalculationDone = 1;
    MemMapData->MemMapState = MEM_MAP_STATE_RESOURCE_CALCULATION_DONE;

    //Clear Memory map structures.
    ClearMemMapStructs (Host, MemMapData);

    return ERROR_RESOURCE_CALCULATION_COMPLETED;
  }
}

VOID
FinalMemoryMapOemHook (VOID)
/**
  EMPTY Oem hook
  @param VOID

  @retval none
**/
{

}

UINT32
FinalizeMemoryMap (PSYSHOST Host, MEMORY_MAP_DATA *MemMapData)
/**

  This routine provides a hook to perform final adjustments to the internal strructure related to memory map.
  One important function that happens here is, that this is where we set the resourceCalculationFlagDone flag and rerun the memap algorithm.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to memory map private data structure

  @retval status

**/
{
  UINT32 status = SUCCESS;

  FinalMemoryMapOemHook ();
  //HSD-4928083:Unused SAD Rules should have limit set
  SetLimitForUnusedSadRules (Host, MemMapData);


  //Update the memory mapping state. If we are here for the first time,
  //set the variable and return an error, so that the memmap will be rerun.
  status = UpdateMemMapState (Host, MemMapData);
  if (status != SUCCESS) {
    return status;
  }

#ifdef DDRT_SUPPORT
  //If we are in degraded memorymap level, update proper response to the requests from CR driver since they are ignored.
  if (MemMapData->MemMapDegradeLevel) {
    status = UpdateRequestforDegradedMemoryMapping (Host);
    if (status != SUCCESS) {
      return status;
    }
  }

  //Update CCUR structs with proper current status.
  UpdateCfgCurAfterMemmap (Host, MemMapData);

  //Update CFGOUT structs with proper validation status.
  UpdateCfgOutAfterMemmap (Host, MemMapData);
#endif // DDRT_SUPPORT

  return SUCCESS;
}

/**

  This routine Creates RIRs in each channel for the memory to be mapped into the system address space.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to the memory map private data structure

  @retval status

**/
UINT32
CreateRirRules (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT32 Status = SUCCESS;
  // Do rank interleaving
  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DDR_MEMMAP, CHECKPOINT_MINOR_RANK_INTER, 0));
  Status = RankInterleave (Host, MemMapData);
  if (Status != SUCCESS) {
    return Status;
  }
  return SUCCESS;
}

/**

  This routine initializes memory mapping parameters based on setup options and system capability.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval VOID

**/
VOID
InitMemMapParams (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  UINT64_STRUCT          mmiohBase, temp;
  KTI_HOST_IN            *KtiHostInPtr;
  SYS_SETUP              *Setup;

  Setup = GetSysSetupPointer ();

  KtiHostInPtr = KTI_HOST_IN_PTR;

  //Check if memory address map setup options values are valid
  CheckMemAddrMapSetupOptionsValues (Host, MemMapData);

  //Initialize MMIOH base from setup option and store it in 64MB granularity.
  mmiohBase.hi = KtiHostInPtr->mmiohBase;
  mmiohBase.lo = 0;
  temp = RShiftUINT64 (mmiohBase, CONVERT_B_TO_64MB);
  Host->var.mem.mmiohBase = temp.lo;
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "mmiohbasefrom setup: %x MMIOH base = %x (64mb)\n", KtiHostInPtr->mmiohBase, Host->var.mem.mmiohBase);

  //Init TOHM
  // maxAddrMem and highGap are in 256MB units. tohmLimit is 64MB granularity.
  Host->var.common.tohmLimit = (Setup->common.maxAddrMem - KtiHostInPtr->highGap) * 4;

  // Extends the MMIO hole to force memory to high address region
  // Note: Tests must not generate addresses in the hole
  if (Setup->mem.dfxMemSetup.dfxOptions & HIGH_ADDR_EN) {
    if (Setup->mem.dfxMemSetup.dfxStraddleHighAddressBit) {
      Host->var.mem.highMemBase = (BIT0 << (Host->var.mem.dfxMemVars.dfxHighAddrBitStart - CONVERT_B_TO_64MB)) - (FOUR_GB_MEM - CONVERT_B_TO_64MB);
    } else {
      // Adjust for 64MB (26 bits) alignment. Start 64MB below address indicated by bit start
      Host->var.mem.highMemBase = BIT0 << (Host->var.mem.dfxMemVars.dfxHighAddrBitStart - CONVERT_B_TO_64MB);
    }
    if (Host->var.mem.highMemBase >= Host->var.mem.mmiohBase) {
      RcDebugPrint (SDBG_MINMAX, "Warning: High memory base is larger than MMIOH, disable high memory base DFX knob\n");
      Setup->mem.dfxMemSetup.dfxOptions &= ~HIGH_ADDR_EN;
      Host->var.mem.dfxMemVars.dfxHighAddrBitStart = 0;
      Host->var.mem.highMemBase = FOUR_GB_MEM;
    }
  } else {
    Host->var.mem.highMemBase = FOUR_GB_MEM;
  }
  MemMapData->NumberofChaDramClusters = GetNumOfDramClustersPerSystem ();

  MemMapData->PmemCaching = NoPmCaching;

  InitMemMapParamsEx (Host, MemMapData);
}

/**

  This routine checks the values of the Memory address mapping setup options.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval N/A

**/
VOID
CheckMemAddrMapSetupOptionsValues (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8      i,j;
  UINT8      MaxImc;
  UINT8      VolMemMode;
  SYS_SETUP  *Setup;
  UINT8      MaxChDdr;
  UINT8      NumChPerMc;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();

  //Set the Interleave parameters based on the setup options
  if (Setup->mem.chInter == CH_1WAY || Setup->mem.chInter == CH_3WAY) {
    Host->var.mem.chInter = Setup->mem.chInter;
  } else {
    Host->var.mem.chInter = NumChPerMc;
  }

  if (Setup->mem.imcInter == IMC_1WAY) {
    Host->var.mem.imcInter = Setup->mem.imcInter;
  } else {
    Host->var.mem.imcInter = MaxImc;
  }

#ifdef DDRT_SUPPORT
  if (Setup->mem.dfxMemSetup.DfxDdrtChInterleaveGran == MEM_INT_GRAN_CH_4KB_TGT_4KB ||
      Setup->mem.dfxMemSetup.DfxDdrtChInterleaveGran == MEM_INT_GRAN_CH_256B_TGT_4KB) {
    Host->var.mem.dfxMemVars.DfxDdrtChInterleaveGran = Setup->mem.dfxMemSetup.DfxDdrtChInterleaveGran;
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\nWARNING!!! Invalid option value for PMem channel granularity.\n");
    Host->var.mem.dfxMemVars.DfxDdrtChInterleaveGran = MEM_INT_GRAN_DDRT_DEFAULT;
  }

  if (Setup->mem.dfxMemSetup.dfxDimmManagement == DIMM_MGMT_BIOS_SETUP || Setup->mem.dfxMemSetup.dfxDimmManagement == DIMM_MGMT_CR_MGMT_DRIVER) {
    Host->var.mem.dfxMemVars.dfxDimmManagement = Setup->mem.dfxMemSetup.dfxDimmManagement;
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\nWARNING!!! Invalid option value for DIMM Management.\n");
    Host->var.mem.dfxMemVars.dfxDimmManagement = DIMM_MGMT_DEFAULT;
  }

  if (Setup->mem.setSecureEraseAllDIMMs == DISABLE_SECURE_ERASE || Setup->mem.setSecureEraseAllDIMMs == ENABLE_SECURE_ERASE) {
    Host->var.mem.setSecureEraseAllDIMMs = Setup->mem.setSecureEraseAllDIMMs;
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\nWARNING!!! Invalid option value for Erase All DIMMs.\n");
    Host->var.mem.setSecureEraseAllDIMMs = DISABLE_SECURE_ERASE;
  }

  for (i = 0; i < MAX_SOCKET; i++) {
    for (j = 0; j < MaxChDdr; j++) {
      if (Setup->mem.setSecureEraseSktCh[i][j] == DISABLE_SECURE_ERASE || Setup->mem.setSecureEraseSktCh[i][j] == ENABLE_SECURE_ERASE) {
        Host->var.mem.setSecureEraseSktCh[i][j] = Setup->mem.setSecureEraseSktCh[i][j];
      } else {
        RcDebugPrintWithDevice (SDBG_MINMAX, i, NO_CH, j, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "\nWARNING!!! Invalid option value for secure erase of S%d CH%d.\n", i, j);
        Host->var.mem.setSecureEraseSktCh[i][j] = DISABLE_SECURE_ERASE;
      }
    }
  }
#else
  Host->var.mem.dfxMemVars.DfxDdrtChInterleaveGran = MEM_INT_GRAN_DDRT_DEFAULT;
  Host->var.mem.dfxMemVars.dfxDimmManagement = DIMM_MGMT_DEFAULT;
  Host->var.mem.setSecureEraseAllDIMMs = DISABLE_SECURE_ERASE;
  for (i = 0; i < MAX_SOCKET; i++) {
    for (j = 0; j < MAX_CH; j++) {
      Host->var.mem.setSecureEraseSktCh[i][j] = DISABLE_SECURE_ERASE;
    }
  }
#endif // DDRT_SUPPORT

  //
  // Default to user interleaving policy (NUMA / UMA)
  //
  MemMapData->IsNumaEn = IsNumaAware ();

  VolMemMode = Setup->mem.volMemMode;

  if (VolMemMode == VOL_MEM_MODE_1LM ||
      VolMemMode == VOL_MEM_MODE_2LM ) {
    Host->var.mem.volMemMode = VolMemMode;
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\nWARNING!!! Invalid option value for Volatile Memory Mode.\n");
    Host->var.mem.volMemMode = VOL_MEM_MODE_DEFAULT;
  }

#ifdef DDRT_SUPPORT
  Host->var.mem.PmemCaching = Setup->mem.PmemCaching;

  //
  // NM flush required both IIO and processor cache to be flushed first.
  // eADR required only IIO and processor flush, not NM flush.
  //
  if (Setup->mem.EadrSupport == EADR_AUTO) {
    Host->var.mem.EadrSupport = EADR_DISABLED;
  } else {
    Host->var.mem.EadrSupport = Setup->mem.EadrSupport;
  }

  if (Host->var.mem.PmemCaching == PMEM_WBCACHE_ENABLED) {
    Host->var.mem.EadrSupport = EADR_DISABLED;
  }

  if (Host->var.mem.EadrSupport == EADR_ENABLED) {
    Host->var.mem.EadrCacheFlushMode = Setup->mem.EadrCacheFlushMode;
  }

  //If Persistent memory mode is enabled in setup, set the persistent memmode to PMEM or PMEM$ based on volatile memory mode.
  if ((Setup->mem.dfxMemSetup.dfxPerMemMode == PER_MEM_MODE) || (Setup->mem.dfxMemSetup.dfxPerMemMode == NON_PER_MEM_MODE))  {
    Host->var.mem.dfxMemVars.dfxPerMemMode = Setup->mem.dfxMemSetup.dfxPerMemMode;
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\nWARNING!!! Invalid option value for Persistent Memory Mode.\n");
    Host->var.mem.dfxMemVars.dfxPerMemMode = NON_PER_MEM_MODE;
  }

  if (!(HybridSystemLevelEmulationEnabled ())) {
    for (i = 0; i < MAX_SOCKET * MaxImc; i++) {
      if (Setup->mem.dfxMemSetup.dfxPartitionRatio[i] == RATIO_50_50_0 || Setup->mem.dfxMemSetup.dfxPartitionRatio[i] == RATIO_50_0_50 ||
          Setup->mem.dfxMemSetup.dfxPartitionRatio[i] == RATIO_0_50_50 || Setup->mem.dfxMemSetup.dfxPartitionRatio[i] == RATIO_100_0_0 ||
          Setup->mem.dfxMemSetup.dfxPartitionRatio[i] == RATIO_0_100_0 || Setup->mem.dfxMemSetup.dfxPartitionRatio[i] == RATIO_0_0_100) {
        Host->var.mem.dfxMemVars.dfxPartitionRatio[i] = Setup->mem.dfxMemSetup.dfxPartitionRatio[i];
      } else {
        RcDebugPrint (SDBG_MINMAX, "\nWARNING!!! Invalid option value for dfxPartitionRatio[%d], set to default ratio.\n", i);
        Host->var.mem.dfxMemVars.dfxPartitionRatio[i] = RATIO_DEFAULT;
      }
    }
  }

  if (Setup->mem.dfxMemSetup.dfxPartitionDDRTDimm == PARTITION_DDRT_DIMM_EN ||
      Setup->mem.dfxMemSetup.dfxPartitionDDRTDimm == PARTITION_DDRT_DIMM_DIS) {
    Host->var.mem.dfxMemVars.dfxPartitionDDRTDimm = Setup->mem.dfxMemSetup.dfxPartitionDDRTDimm;
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\nWARNING!!! Invalid option value for PMEM DIMM partitioning.\n");
    Host->var.mem.dfxMemVars.dfxPartitionDDRTDimm = PARTITION_DDRT_DIMM_DIS;
  }
#else
  if (!IsHbmSku ()) {
    Host->var.mem.volMemMode = VOL_MEM_MODE_1LM;
  }
  Host->var.mem.PmemCaching = PMEM_WBCACHE_DISABLED;
  Host->var.mem.EadrSupport = EADR_DISABLED;
  Host->var.mem.dfxMemVars.dfxPerMemMode = NON_PER_MEM_MODE;
  Host->var.mem.dfxMemVars.dfxPartitionDDRTDimm = PARTITION_DDRT_DIMM_DIS;
#endif // DDRT_SUPPORT

  if (Setup->mem.dfxMemSetup.dfxCfgMask2LM == CFG_MASK_2LM_NORMAL || Setup->mem.dfxMemSetup.dfxCfgMask2LM == CFG_MASK_2LM_AGGRESSIVE) {
    Host->var.mem.dfxMemVars.dfxCfgMask2LM = Setup->mem.dfxMemSetup.dfxCfgMask2LM;
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\nWARNING!!! Invalid option value for Configuration Mask for 2LM.\n");
    Host->var.mem.dfxMemVars.dfxCfgMask2LM = CFG_MASK_2LM_NORMAL;
  }

#ifdef DDRT_SUPPORT
  //
  // check whether 2GB short stroke is enabled and make appropriate changes
  //
  if (Setup->mem.dfxMemSetup.dfxOptions & (SHORT_STROKE_INT | SHORT_STROKE_NO_INT)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "DFX 2GB Short Stroke Enabled (%a)\n", (Setup->mem.dfxMemSetup.dfxOptions & SHORT_STROKE_INT) ? "Interleaved" :"Not Interleaved");

    // force BIOS setup menu
    Host->var.mem.dfxMemVars.dfxDimmManagement = DIMM_MGMT_BIOS_SETUP;
    // Enable to use BIOS setup menu option to provision CR DIMMs
    Host->var.mem.dfxMemVars.dfxPartitionDDRTDimm = 1; // 0 = Disabled, 1 = Enabled
    // Force 100% PMEM for 2GB short stroke configuration
    SetMem (&Host->var.mem.dfxMemVars.dfxPartitionRatio, sizeof (Host->var.mem.dfxMemVars.dfxPartitionRatio), RATIO_0_100_0);
  }
#endif // DDRT_SUPPORT

  CheckMemAddrMapSetupOptionsValuesEx (Host, MemMapData);

}

/**

  This routine is to get/retrieve SKU limit value per socket in the system from PCU. This
  function will abstract out differences in the Mailbox Command return data to maintain
  consistent handling of Memory SKUing between generations of products.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @retval VOID

**/
VOID
InitializeSkuLimits (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8  Socket;
  UINT32 SkuLimitFromPcu;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    MemMapData->MemMapSkuLimit[Socket].SkuLimit = 0xFFFF;
    MemMapData->MemMapSkuLimit[Socket].MemoryAllocated = 0;
    MemMapData->MemMapSkuLimit[Socket].SkuLimitValid = FALSE;
    MemMapData->MemMapSkuLimit[Socket].Violation = FALSE;

    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }

    if (!EFI_ERROR (PcuGetCpuSkuLimit (Socket, &SkuLimitFromPcu))) {

      if (SkuLimitFromPcu == 0) {
        SkuLimitFromPcu = GetSkuLimitDefault ();
      }

      if (SkuLimitFromPcu == 0) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Error reading SKU Limit FUSE\nSkuLimit read from PCU is 0. Invalid SkuLimit\n");
        FatalError (ERR_PCU, FUSE_ERROR);
        return;
      }

      MemMapData->MemMapSkuLimit[Socket].SkuLimit = SkuLimitFromPcu;
      MemMapData->MemMapSkuLimit[Socket].SkuLimitValid = TRUE;

      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "SkuLimit value in 64GB units: 0x%x \n", MemMapData->MemMapSkuLimit[Socket].SkuLimit);
    } else {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Error reading SkuLimit from PCU \n");
    }
  } //Socket loop

} //InitializeSkuLimits()

/**

  This routine is to check whether there SKU limit violation occurred on any socket after SAD rules creation

  @param[in] Host                - Pointer to sysHost
  @param[in] MemMapData          - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval BOOLEAN                - TRUE (SKU Limit violation) FALSE (No SKU Limit violation)

**/
BOOLEAN
CheckSkuLimitViolation (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  EFI_STATUS                    Status;
  MRC_PLATFORM_HOOKS_INTERFACE  *MrcPlatformHooksInterface = NULL;
  UINT8                         Socket;

  if ((MemMapData == NULL) || (Host == NULL)) {
    return FALSE;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }

    if (MemMapData->MemMapSkuLimit[Socket].Violation) {
      Status = LocateInterface (
        &gMrcPlatformHooksInterfaceGuid,
        0,
        (VOID **)&MrcPlatformHooksInterface
        );

      if (!EFI_ERROR (Status)) {
        MrcPlatformHooksInterface->PlatformHandleSkuLimitViolation (Host);
      }
      return TRUE;
    }
  }
  return FALSE;
} //CheckSkuLimitViolation()

/**

  This routine is to initilize value per socket for SKU limit values

  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval VOID

**/
VOID
ResetSkuLimitsViolation (
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8 Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }

    MemMapData->MemMapSkuLimit[Socket].MemoryAllocated = 0x0;
    MemMapData->MemMapSkuLimit[Socket].Violation = FALSE;
  } // Socket loop
} //ResetSkuLimitsViolation()

#ifdef DDRT_SUPPORT
/**
  This routine sets the system time for NVM dimms populated. If
  secure erase is enabled, sanitize the NVM dimms.

  @param Host  - Pointer to sysHost

  @retval None

**/
VOID
InitNvmDimmSystemCommands (
  IN PSYSHOST Host
  )
{
  UINT8 Socket;
  UINT8 Channel;
  UINT8 Dimm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram (*DimmNvList)[MAX_DIMM];
  UINT32 Security;
  UINT32 OptStatus;
  UINT8 MbStatus = 0;
  UINT8 SecMbStatus = 0;
  UINT8 PassphaseLen;
  UINT8 Passphrase[PASSPHRASE_LENGTH];
  UINT32 Status = SUCCESS;
  UINT32 SecStatus = SUCCESS;
  SYS_SETUP *Setup;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  // For each socket
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }
    ChannelNvList = GetChannelNvList (Host, Socket);
    // For each channel
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if ((*ChannelNvList)[Channel].enabled == 0) {
        continue;
      }
      DimmNvList = GetDimmNvList (Host, Socket, Channel);      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }        //Skip if this is not a NVMDIMM dimm
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }

        //Skip if DIMM is disabled
        if (!IsNvDimmEnabled (Host, Socket, Channel, Dimm)) {
          continue;
        }

        if (PcdGetBool (PcdUseNvdimmFwInterface))  {
          //
          // Set System Time
          //
          RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Set System Time for PMem\n");
          Status = SetFnvSystemTime (Host, Socket, Channel, Dimm, &MbStatus);
          if (Status != SUCCESS) {
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Communication with the PMem DIMM failed\n");
            EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS,
              Socket, Channel, Dimm, NO_RANK);
            DisableDIMM (Host, Socket, Channel, Dimm);
          }

          Security = 0;
          OptStatus = 0;

          Status = GetSecurityState (Host, Socket, Channel, Dimm, &Security, &OptStatus, &MbStatus);

          //
          // Check for Master PassPhrase enabled (secure erase policy should also be opted-in if master passphrase enabled)
          //
          Host->var.mem.SetSecureEraseSktChHob[Socket][Channel] = 0;
          if ((Security & MASTER_PASSPHRASE_ENABLED) && (OptStatus & SECURE_ERASE_POLICY_OPT_IN_STATUS)) {
            Host->var.mem.SetSecureEraseSktChHob[Socket][Channel] = 1;
          }

          //call SecureEraseUnit
          if (Host->var.mem.setSecureEraseAllDIMMs == ENABLE_SECURE_ERASE ||
              Host->var.mem.setSecureEraseSktCh[Socket][Channel] == ENABLE_SECURE_ERASE) {

            if (Status == SUCCESS) {
              if (((Security & SECURITY_ENABLED) != 0 && (Security & SECURITY_LOCKED) != 0) && !(Security & MASTER_PASSPHRASE_ENABLED)) {
                for (PassphaseLen = 0; PassphaseLen < PASSPHRASE_LENGTH; PassphaseLen++) {
                  Passphrase[PassphaseLen] = 0;
                }
                RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Set Secure Erase Unit\n");
                SecStatus = SecureEraseUnit (Host, Socket, Channel, Dimm, Passphrase, &SecMbStatus);
                if (SecStatus != SUCCESS) {
                  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "Communication with the PMEM DIMM failed\n");
                  EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS,
                    Socket, Channel, Dimm, NO_RANK);
                  // Do not disable the dimm if secure erase fails, leaving enabled to handle this by OS tools
                  // if the dimm fails for other reasons
                  // DisableDIMM (Host, Socket, Channel, Dimm);
                }
              }
            } else {
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Communication with the PMem DIMM failed\n");
              EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS,
                Socket, Channel, Dimm, NO_RANK);
              DisableDIMM (Host, Socket, Channel, Dimm);
            }
          }
        }
      }//Dimm
    }//Channel
  } //Socket

  FfwDigestRead (Host);
} // InitNvmDimmSystemCommands()
#endif // DDRT_SUPPORT

/**
  This routine saves ddr clustering mode to MemMapHost to implement our external API.

  @param [in] MemMapData    Pointer to MemMapData (Private DS maintained by Memory decoder)

**/
VOID
SaveDdrClusteringMode (
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  MEMORY_MAP_HOST   *MemMapHost;

  MemMapHost = GetMemMapHostPointer ();

  if (IfFeatureEnabled (TypeSnc)) {
    if (GetNumOfClusterPerSystem () == KTI_SNC4) {
      MemMapHost->DdrClusteringMode = Snc4ClusteringMode;
      RcDebugPrint (SDBG_MINMAX, "DDR clustering mode is SNC4\n");
    } else {
      MemMapHost->DdrClusteringMode = Snc2ClusteringMode;
      RcDebugPrint (SDBG_MINMAX, "DDR clustering mode is SNC2\n");
    }
  } else if (MemMapData->UmaBasedClustersPerSocket == UMABASEDCLUSTERING_QUADRANT) {
    MemMapHost->DdrClusteringMode = QuadClusteringMode;
    RcDebugPrint (SDBG_MINMAX, "DDR clustering mode is Quad\n");
  } else if (MemMapData->UmaBasedClustersPerSocket == UMABASEDCLUSTERING_HEMISPHERE) {
    MemMapHost->DdrClusteringMode = HemiClusteringMode;
    RcDebugPrint (SDBG_MINMAX, "DDR clustering mode is Hemi\n");
  } else {
    MemMapHost->DdrClusteringMode = All2All;
    RcDebugPrint (SDBG_MINMAX, "DDR clustering mode is All2All\n");
  }
}


/**

  This routine is to store/pass MemMapData items(required in later phase) to Host in the system

  @param [IN] Host        - Pointer to sysHost
  @param [IN] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval void

**/
VOID
AddMemMapDatatoHost (
  IN SYSHOST  *Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8                Socket;

  //
  // fill these values in to host, as they will be used later in DXE for populating PCAT-SKTSKU table
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }
    if (MemMapData->MemMapSkuLimit[Socket].SkuLimitValid == TRUE) {
      //
      // units 64GB = 64 * 1024 = in 64MB units
      //
      Host->var.mem.socket[Socket].SktSkuLimit = MemMapData->MemMapSkuLimit[Socket].SkuLimit * GB_TO_MB_CONVERSION;
      Host->var.mem.socket[Socket].SktTotMemMapSPA = MemMapData->MemMapSkuLimit[Socket].MemoryAllocated;   // 64MB units

      //
      // Add Tad adjustment size to SktTotMemMapSPA which gets included in PCAT total size to matche the individual CCur PCD spa size
      //
      if (Socket == MemMapData->Below4GBSktIndex) {
        Host->var.mem.socket[Socket].SktTotMemMapSPA = Host->var.mem.socket[Socket].SktTotMemMapSPA + MemMapData->TadLimitAdjustment;
      }

      if (Host->var.mem.volMemMode == VOL_MEM_MODE_2LM ||
          Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
        //
        // DDR/Memory size excluded from limit when in 2LM mode in 64MB units
        //
        Host->var.mem.socket[Socket].SktMemSize2LM = TotalNmSize (MemMapData, Socket);
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Total NM size:0x%x\n", Host->var.mem.socket[Socket].SktMemSize2LM);
      }
      //
      // used to populated SKT SKU info table only if valid
      //
      Host->var.mem.socket[Socket].SktSkuValid = MemMapData->MemMapSkuLimit[Socket].SkuLimitValid;
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "SktSkuLimit:0x%x; SktTotMemMapSPA:0x%x\n", Host->var.mem.socket[Socket].SktSkuLimit,
        Host->var.mem.socket[Socket].SktTotMemMapSPA);
    }
#ifdef DDRT_SUPPORT
    //
    // Check if there is a change in goal(provisiong) run meminit again
    //
    MemoryInitDdrProvision(Host, Socket);

    //
    // Set the Intel PMem performance knobs as recommended FW Interface Specification(FIS)
    //
    HostAccessiblePerfKnob (Host, Socket);
#endif // DDRT_SUPPORT
  } // Socket loop

  SaveDdrClusteringMode (MemMapData);
}

#ifdef DDRT_SUPPORT
/**

  Description: When there is a change in goal, perform meminit

  @param [IN] Host  - Pointer to sysHost
  @param [IN] Socket - socket id

  @retval VOID

**/
VOID
MemoryInitDdrProvision (
  IN SYSHOST  *Host,
  IN UINT8    Socket
  )
{
  UINT8                         Mc;
  UINT8                         Ch;
  UINT8                         Dimm;
  UINT8                         ImcEnabled;
  BOOLEAN                       GoalChanged;
  UINT8                         MaxImc;
  UINT8                         MaxChPerMc;
  UINT32                        DdrChEnabled = 0;
  MEM_NVRAM_STRUCT              *NvramMem;
  CHANNEL_NVRAM_STRUCT          (*ChannelNvList)[MAX_CH];
  DDRT_DIMM_DEVICE_INFO_STRUCT  *DdrtDimmInfo;
  DIMM_NVRAM_STRUCT             (*DimmNvList)[MAX_DIMM];

  // Create the Channel Mask
  DdrChEnabled = 0;
  NvramMem = &Host->nvram.mem;
  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxImc = GetMaxImc();
  MaxChPerMc = GetNumChannelPerMc ();

  // For each IMC in the socket
  for (Mc = 0; Mc < MaxImc; Mc++) {
    //
    // check if Mc is populated
    //
    GetImcEnabledData (Socket, Mc, &ImcEnabled);
    if (ImcEnabled == 0) {
      continue;
    }

    // init goal changed
    GoalChanged = FALSE;

    for (Ch = 0; Ch < MaxChPerMc; Ch++) {
      if ((*ChannelNvList)[Ch].enabled == 0) {
        continue;
      }

      DimmNvList = &NvramMem->socket[Socket].channelList[Ch].dimmList;

      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        //Skip if this is not a NVMDIMM dimm
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }
        DdrtDimmInfo = GetDdrtDimmInfo (Socket, Ch, Dimm);

        if (DdrtDimmInfo->NgnCfgReqPresent && DdrtDimmInfo->NgnCfgReqValid) {
          GoalChanged = TRUE;
          break;
        }
      } // Dimm
      //
      // found an AEP DIMM with goal changed
      //
      if (GoalChanged == TRUE) {
        break;
      }
    }  // Ch
    //
    // if there is atleast 1 AEP DIMM in a MC with goal changed, meminit all the enabled channels in that MC
    //
    if (GoalChanged == TRUE) {
      for ( Ch = 0; Ch < MaxChPerMc; Ch++ ) {
        if ((*ChannelNvList)[Ch].enabled == 0) {
          continue;
        }

        DdrChEnabled |= 1 << ((MaxChPerMc * Mc) + Ch);
      } // Ch
    }
  } // Mc

  //
  // Write all 0's pattern
  //
  if (MemInitOpt(Host, Socket, DdrChEnabled) == SUCCESS) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Write all 0's pattern was successfull\n");
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Write all 0's pattern was failure\n");
  }
}

/**

  Set the Intel PMem performance knobs as recommended FW Interface Specification(FIS) for all Memory Modes
  @param [IN] Host       - Pointer to sysHost
  @param [IN] Socket     - socket id

  @retval VOID

**/
VOID
HostAccessiblePerfKnob (
  IN SYSHOST          *Host,
  IN UINT8            Socket
  )
{
  UINT8                Mc;
  UINT8                Ch;
  UINT8                Dimm;
  UINT8                KnobCnt;
  UINT8                PerfKnobNum;
  UINT8                MaxImc;
  UINT8                MaxChPerMc;
  UINT8                ImcEnabled;
  UINT8                MailboxStatus;
  UINT32               Status = SUCCESS;
  PERFORMANCE_KNOB     SetPerfKnob[INTEL_PMEM_PERF_KNOBS_CNT];
  CHANNEL_NVRAM_STRUCT (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT    (*DimmNvList)[MAX_DIMM];

  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  ZeroMem (SetPerfKnob, sizeof (PERFORMANCE_KNOB) * INTEL_PMEM_PERF_KNOBS_CNT);
  PerfKnobNum = 0;

  // For each IMC in the socket
  for (Mc = 0; Mc < MaxImc; Mc++) {
    //
    // check if Mc is populated
    //
    GetImcEnabledData (Socket, Mc, &ImcEnabled);
    if (ImcEnabled == 0) {
      continue;
    }

    for (Ch = 0; Ch < MaxChPerMc; Ch++) {
      if ((*ChannelNvList)[Ch + (Mc * MaxChPerMc)].enabled == 0) {
        continue;
      }

      DimmNvList = GetDimmNvList (Host, Socket, Ch + (Mc * MaxChPerMc));
      // For each DIMM
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        //Skip if this is not a NVMDIMM dimm
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
          continue;
        }
        //
        // check for FIS version; only set the perf knob
        //
        if (Host->var.mem.socket[Socket].channelList[Ch + (Mc * MaxChPerMc)].FisVersion[Dimm] >= FIS_200_VERSION) {
          //
          // check Intel DCPMM Gen
          //
          if (((*DimmNvList)[Dimm].fmcType == FMC_BWV_TYPE) || ((*DimmNvList)[Dimm].fmcType == FMC_CWV_TYPE)) {
            SetPerfKnob[0].PerfKnobId = PERF_KNOB1_ID;
            SetPerfKnob[0].PerfKnobValue = PERF_KNOB1_VALUE_2LMPM;
            SetPerfKnob[1].PerfKnobId = PERF_KNOB2_ID;
            SetPerfKnob[1].PerfKnobValue = PERF_KNOB2_VALUE_2LMPM;
          } else if ((*DimmNvList)[Dimm].fmcType == FMC_EKV_TYPE || (*DimmNvList)[Dimm].fmcType == FMC_FNV_TYPE){
            //
            // check for platform level flag(2LM/1LM mode)
            //
            if (IS_DDR_2LM_CACHE(Host)) {
              //
              // mem mode is 2LM/Mixed(2LM+AD)
              // this was done so that ADWB does not execute here.
              //
              if (IsMemTypePresentMcFunction(Socket, Mc, IsMemType2lm) || IsMemTypePresentMcFunction(Socket, Mc, IsMemTypeAppDirect)) {
                SetPerfKnob[0].PerfKnobId = PERF_KNOB1_ID;
                SetPerfKnob[0].PerfKnobValue = PERF_KNOB1_VALUE_2LMPM;
                SetPerfKnob[1].PerfKnobId = PERF_KNOB2_ID;
                SetPerfKnob[1].PerfKnobValue = PERF_KNOB2_VALUE_2LMPM;
              } else if (IsMemTypePresentMcFunction(Socket, Mc, IsMemType2lm)) { // mem mode only
                SetPerfKnob[0].PerfKnobId = PERF_KNOB1_ID;
                SetPerfKnob[0].PerfKnobValue = PERF_KNOB1_VALUE_2LMPM;
                SetPerfKnob[1].PerfKnobId = PERF_KNOB2_ID;
                SetPerfKnob[1].PerfKnobValue = PERF_KNOB2_VALUE_2LMPM;
              }
            } else { // in 1LM (may be in AD)
              //
              // mem mode is AD
              //
              if (IsMemTypePresentMcFunction(Socket, Mc, IsMemTypeAppDirect)) {
                SetPerfKnob[0].PerfKnobId = PERF_KNOB1_ID;
                SetPerfKnob[0].PerfKnobValue = PERF_KNOB1_VALUE_PMONLY;
                SetPerfKnob[1].PerfKnobId = PERF_KNOB2_ID;
                SetPerfKnob[1].PerfKnobValue = PERF_KNOB2_VALUE_PMONLY;
              } else { // neither 2LM nor AD
                SetPerfKnob[0].PerfKnobId = PERF_KNOB1_ID;
                SetPerfKnob[0].PerfKnobValue = PERF_KNOB1_VALUE_2LMPM; // it should this value as per Perf team
                SetPerfKnob[1].PerfKnobId = PERF_KNOB2_ID;
                SetPerfKnob[1].PerfKnobValue = PERF_KNOB2_VALUE_2LMPM; // it should this value as per Perf team
              }
            }
          }
          //
          // Set performance knob 5 & 6 on Intel DCPMM
          //
          SetPerfKnob[2].PerfKnobId = PERF_KNOB5_ID;
          SetPerfKnob[2].PerfKnobValue = PERF_KNOB5_VALUE_DEFAULT;
          SetPerfKnob[3].PerfKnobId = PERF_KNOB6_ID;
          SetPerfKnob[3].PerfKnobValue = PERF_KNOB6_VALUE_DEFAULT;

          //
          // Performance knobs 1, 2, 5, 6 have been added into the performance knob list, count the number here.
          //
          PerfKnobNum = 4;

          //
          // Add SoC-specific performance knobs into the performance list.
          //
          AddPmemPerfKnobs (Host, Socket, Ch + (Mc * MaxChPerMc), Dimm, SetPerfKnob, &PerfKnobNum);

          //
          // Send MB commands to Intel DCPMM
          //
          for (KnobCnt = 0; KnobCnt < PerfKnobNum; KnobCnt++) {
            MailboxStatus = MB_STAT_SUCCESS;

            Status = SetPerformanceKnob (Host, Socket, Ch + (Mc * MaxChPerMc), Dimm, &SetPerfKnob[KnobCnt], &MailboxStatus);
            if (Status == SUCCESS) {
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch + (Mc * MaxChPerMc), Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Setting performance knob %d successfully for memory mode\n", SetPerfKnob[KnobCnt].PerfKnobId);
            }

            //
            // Log error and disable channel if NGN communication failure
            //
            if ((MailboxStatus != MB_STAT_SUCCESS) && (MailboxStatus != MB_STAT_MEDIA_DISABLED)) {   // Allow media disabled case to continue booting
              RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch + (Mc * MaxChPerMc), Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Communication with the PMem DIMM failed when setting performance knob %d!\n", SetPerfKnob[KnobCnt].PerfKnobId);
              //log data format:
              //Byte 0 (Bit 0 - DIMM Slot Number, Bits 1-3 - Channel Number, Bits 4-6 - Socket Number,Bit 7 - Reserved)
              //Byte 1 - FW MB Opcode,Byte 2 - FW MB Sub-Opcode,Byte 3 - FW MB Status Code
              EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, Socket, Ch + (Mc * MaxChPerMc), Dimm, NO_RANK);
              DisableDIMM (Host, Socket, Ch + (Mc * MaxChPerMc), Dimm);

              // Skip and dont send any other perf knobs to this DCPMM
              break;
            }
          }
        }
      } // Dimm
    }  // Ch
  } // Mc
} // HostAccessiblePerfKnob()
#endif // DDRT_SUPPORT

/**

  Calculate the TAD Offset value which will ultimately be used to program the
  MC TADCHNILVOFFSET "tad_offset" and "tad_offset_sign" register fields.
  The calculated TAD Offset value may be a negative 2'complement integer value.

  @param Mirrored       - Indicates if Mirroring is enabled
  @param SocketWays     - Socket Interleave ways for the TAD entry
  @param ChannelWays    - The Channel Interleave ways for the corresponding SAD
  @param ChannelMemUsed - The total channel size of each channel participating
  in the interleave
  @param TadLimit       - Limit of the TAD entry

  @return The TAD Offset

**/
INT32
CalculateTadOffset (
  IN BOOLEAN Mirrored,
  IN UINT8   SocketWays,
  IN UINT8   ChannelWays,
  IN UINT32  ChannelMemUsed,
  IN UINT32  TadLimit
  )
{
  INT32 TadOffset;
  UINT32 TotalMemUsed;

  TotalMemUsed = SocketWays * ChannelWays * ChannelMemUsed;
  if (Mirrored) {
    //
    // Update the TadOffset value
    //
    TadOffset = TadLimit - ((TotalMemUsed) / 2);
  } else{
    //
    //Update the TadOffset value
    //
    TadOffset = TadLimit - TotalMemUsed;
  }
  return TadOffset;
}

/**
  Get SGX memory map PRMRR data after memory map flow.

  This function is invoked by SGX module after memory map flow.
  It abstracts SGX requirement on system memory topology:
  1. If PrmrrSize field in CPUSetup is zero, memory map flow will calculate all supported PRMRR size based on existing
     configuration. SGX module can based on this info and CPU capability to configure the correct PRMRR size after reboot.
  2. If PrmrrSize field in CPUSetup is not zero, memory map flow will allocate and reserve the PRMRR size in system memory map.
     SGX module can depend on this output data structure to program the CPU registers to enable SGX CPU feature.

  Caller is responsible for allocating the output SGX_PRMRR_DATA data structure.

  @retval EFI_INVALID_PARAMETER    SgxMemMapData is NULL.
  @retval EFI_NOT_READY            This function is invoked before memory map flow.
  @retval EFI_NOT_READY            This function is not available for current memory decode SOC library.
  @retval EFI_UNSUPPORTED          Current memory topology is not supported SGX capable configuration.
  @retval EFI_INVALID_PARAMETER    Input PrmrrSize field in CPUSetup structure is not-zero and it is not a power of two.
                                   The supported PRMRR bit map is returned in ValidPrmrrBitMap field in SGX_PRMRR_DATA.
  @retval OUT_OF_RESOURCES         Memory map flow fails to reserve size indicated by PRMRR size, but the supported PRMRR
                                   bit map is returned in ValidPrmrrBitMap field in SGX_PRMRR_DATA.
  @retval EFI_SUCCESS              If the PrmrrSize field in CPUSetup is 0, the supported PRMRR bit map is returned in
                                   ValidPrmrrBitMap field in SGX_PRMRR_DATA.
  @retval EFI_SUCCESS              If the PrmrrSize field in CPUSetup is not 0, memory map flow has successfully reserved
                                   PrmrrSize SGX memory for SGX requirement.

**/
EFI_STATUS
EFIAPI
GetSgxPrmrrData (
  OUT SGX_PRMRR_DATA   *SgxPrmrrData
  )
{
  EFI_HOB_GUID_TYPE    *GuidHob;
  SECURITY_MEMMAP_HOB  *SecurityMemMapHob;

  if (SgxPrmrrData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (SgxPrmrrData, sizeof (*SgxPrmrrData));
  GuidHob = GetFirstGuidHob (&gSecurityMemMapHobGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_READY;
  }

  SecurityMemMapHob = GET_GUID_HOB_DATA (GuidHob);
  if ((SecurityMemMapHob->Status & SGX_STATUS_UNSUPPORTED) != 0) {
    return EFI_UNSUPPORTED;
  }

  SgxPrmrrData->ValidPrmrrBitMap = SecurityMemMapHob->SgxPrmrrData.ValidPrmrrBitMap;
  SgxPrmrrData->PrmrrCountPerPackage = SecurityMemMapHob->SgxPrmrrData.PrmrrCountPerPackage;
  if ((SecurityMemMapHob->Status & SGX_STATUS_INVALID_PARAMETER) != 0) {
    return EFI_INVALID_PARAMETER;
  } else if ((SecurityMemMapHob->Status & SGX_STATUS_OUT_OF_RESOURCES) != 0) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (SgxPrmrrData, &SecurityMemMapHob->SgxPrmrrData, sizeof (*SgxPrmrrData));
  return EFI_SUCCESS;
}


/**
  Get TDX memory map SEAMRR data after memory map flow.

  This function is invoked by TDX module after memory map flow. It abstracts TDX requirement on system memory topology.
  Memory map flow will assign the appropriate TDX SEAMRR region based on current memory topology if SecurityPolicy->EnableTdx
  is enabled.

  Caller is responsible for allocating the output TDX_SEAMRR_DATA data structure.

  @retval EFI_INVALID_PARAMETER    TdxMemMapData is NULL.
  @retval EFI_NOT_READY            This function is invoked before memory map flow.
  @retval OUT_OF_RESOURCES         Memory map flow fails to allocate TDX SEAMRR region.
  @retval EFI_SUCCESS              TDX SEAMRR region is allocated successfully in output TdxMemMapData.

**/
EFI_STATUS
EFIAPI
GetTdxSeamrrData (
  OUT TDX_SEAMRR_DATA   *TdxMemMapData
  )
{
  EFI_HOB_GUID_TYPE     *GuidHob;
  SECURITY_MEMMAP_HOB   *SecurityMemMapHob;

  if (TdxMemMapData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (TdxMemMapData, sizeof (*TdxMemMapData));
  GuidHob = GetFirstGuidHob (&gSecurityMemMapHobGuid);
  if (GuidHob == NULL) {
    return EFI_NOT_READY;
  }

  SecurityMemMapHob = GET_GUID_HOB_DATA (GuidHob);
  if ((SecurityMemMapHob->Status & SGX_STATUS_UNSUPPORTED) != 0) {
    return EFI_UNSUPPORTED;
  }

  if (SecurityMemMapHob->TdxSeamrrData.SeamrrBase == 0) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (TdxMemMapData, &SecurityMemMapHob->TdxSeamrrData, sizeof (*TdxMemMapData));
  return EFI_SUCCESS;
}

/**

  Initialize the MemMapHost global pointer and data structure.

  This routine will allocate memory for the MemMapHost structure on the heap using
  the RcAllocateLib memory allocation routines. The pointer to this memory is then
  stored using the SysHostPointerLib. Once this pointer is stored, it can be fetched
  globally from any module utilizing the SysHostPointerLib.

**/
VOID
InitMemMapHost (
  VOID
  )
{
  MEMORY_MAP_HOST *MemMapHost = NULL;

  MemMapHost = RcAllocatePool (sizeof (MEMORY_MAP_HOST));
  if (MemMapHost == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return;
  }

  SetMemMapHostPointer (MemMapHost);

  //
  // Initialize data structures
  //
  ZeroMem ((VOID *) MemMapHost, sizeof (MEMORY_MAP_HOST));
  RcDebugPrint (SDBG_DEFAULT, "sizeof (MEMORY_MAP_HOST) = %d\n", sizeof (MEMORY_MAP_HOST));

  MemMapHost->NumberofChaDramClusters = GetNumOfDramClustersPerSystem ();
}

/**

  Compute contents for memory map CSRs to allocate physical address space to sockets,
  channels, and dimm ranks. Writes necessary memory map CSRs. Displays memory map
  configuration for debug.

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
EFIAPI
InitMemoryMap (
  PSYSHOST Host
  )
{
  EFI_STATUS      PpiStatus;
  UINT32          Status;
  MEMORY_MAP_DATA *MemMapData;
  UINT32          PrevValueSerialDebugMsgLvl;
  UINT8           SizeAdjustmentForSnc[MAX_SOCKET][MAX_DRAM_CLUSTERS * MAX_SAD_RULES];
  SYS_SETUP       *Setup;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

  Status = SUCCESS;
  Setup = GetSysSetupPointer ();

  if (!IsMemFlowEnabled (InitializeMemoryMap)) {
    return SUCCESS;
  }

  MemMapData = RcAllocatePool (sizeof (MEMORY_MAP_DATA));
  if (MemMapData == NULL) {
    RC_FATAL_ERROR (FALSE, ERR_CAR_LIMIT, ERR_OUT_OF_CAR_RESOURCES);
    return FAILURE;
  }

  PrevValueSerialDebugMsgLvl = GetDebugLevel ();
  if (Setup->common.serialDebugMsgLvl != 0 ) {
    SetDebugLevel (PrevValueSerialDebugMsgLvl | SDBG_INFO | SDBG_MEM_TRAIN);
  }

  //
  // Make sure there is still memory present after running MemTest
  //
  CheckMemPresentStatus (Host);

  //
  // Clear memory mapping private data structure
  //
  ZeroMem ((UINT8 *) MemMapData, sizeof (MEMORY_MAP_DATA));

  //
  // Initialize the MemMapHost data and global pointer.
  //
  InitMemMapHost ();

  //
  // Clear Memory map state.
  //
  MemMapData->MemMapState = MEM_MAP_STATE_RESOURCE_CALCULATION;

  //
  // We need to keep this statement in source, otherwise RC SIM preflight will fail.
  //
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\n***BEGIN MEMORY MAPPING***\n");

  //
  // Get CPU SKU limit from PCU
  //
  InitializeSkuLimits (Host, MemMapData);

  //
  // Initialize paramaters for memory mapping only for MEM_MAP_STATE_RESOURCE_CALCULATION
  //
  InitMemMapParams (Host, MemMapData);

#ifdef DDRT_SUPPORT
  //
  // Initialize the set system time for NVM dimm and secure erase if setup knob is enabled
  //
  InitNvmDimmSystemCommands (Host);
#endif // DDRT_SUPPORT

  do {
    //
    // Clear the SKU limit values, if there is a recomputation due to not successful in 1st pass
    //
    ResetSkuLimitsViolation (MemMapData);

    //
    // Skip populating memory sizes again if we are running because of resource unavailability.
    //
    if (MemMapData->MemMapState != MEM_MAP_STATE_RESOURCE_CALCULATION_FAILURE) {
#ifdef DDRT_SUPPORT
      //
      // Populate DDRT structures and partition DIMMs
      //
      EvaluateAndPartitionDdrt (Host, MemMapData);
#endif // DDRT_SUPPORT

      PopulateMemorySizeFields (Host, MemMapData);

#ifdef DDRT_SUPPORT
      //
      // Check DDRT Topology
      //
      if ((Setup->mem.enforcePOR == ENFORCE_POR_EN) && (Host->nvram.mem.DcpmmPresent == 1)) {
        Status = CheckDdrtPorCompliance (Host, MemMapData);
        if (Status != SUCCESS) {
          continue;
        }
        CheckDdrtPorComplianceAcrossSockets (Host);
      }
      //
      // Check if 2LM request is present and supported
      //
      Check2LmRequestCompliance (Host);
#endif // DDRT_SUPPORT
    }

    if (MemMapData->MemMapState == MEM_MAP_STATE_RESOURCE_CALCULATION) {
      PrintDimmConfig (Host);
    }

    //
    // Create DRAM rules based on memory population
    //
    Status = CreateSadRules (Host, MemMapData);
    if ((Status != SUCCESS) && (Status != ERROR_RESOURCE_CALCULATION_CAP_MET)) {
      //
      // Degrade the memory map level and rerun the memory map
      //
      DegradeMemoryMapLevel (Host, MemMapData);
      continue;
    } else if (CheckSkuLimitViolation (Host, MemMapData)) {
      //
      // SKU Limit Violation found during SAD rules creation - degrade memory map level
      //
      DegradeMemoryMapLevel (Host, MemMapData);
      Status = FAILURE;
      continue;
    }

    ZeroMem (SizeAdjustmentForSnc, sizeof (SizeAdjustmentForSnc));

    //
    // Update DRAM rules to create mirror regions, if enabled
    //
    Status = AdjustMemAddrMapForMirror (Host, MemMapData, SizeAdjustmentForSnc);
    // no specific action needed; continue to map w/o mirror

    //
    // Adjust the DRAM rules for any Silicon Workarounds, if required
    //
    Status = AdjustMemAddrMapForSiliconWorkaround (Host, MemMapData);
    if (Status != SUCCESS) {
      //
      // Degrade the memory map level and rerun the memory map
      //
      DegradeMemoryMapLevel (Host, MemMapData);
      continue;
    }

    //
    // Create TAD rules for each IMC in the system
    //
    Status = CreateTadRules (Host, MemMapData);
    if (Status != SUCCESS)  {
      //
      // Degrade the memory map level and rerun the memory map
      //
      DegradeMemoryMapLevel (Host, MemMapData);
      continue;
    }

    CreateRirRules (Host, MemMapData);

    Status = FinalizeMemoryMap (Host, MemMapData);
  } while (Status != SUCCESS);


  DisplayMemoryMap (Host, MemMapData);

  WriteMemoryMapCsrs (Host, MemMapData);

  PostMemoryMapConfig (Host, MemMapData);

  AdjustMemorySizeFieldsforMirror (Host, MemMapData, SizeAdjustmentForSnc);

  //
  // We need to keep this statement in source, otherwise RC SIM preflight will fail.
  //
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\n***END MEMORY MAPPING***\n");

  if (Setup->common.serialDebugMsgLvl != 0 ) {
    SetDebugLevel (PrevValueSerialDebugMsgLvl);
  }

  PpiStatus = LocateInterface (
    &gMrcPlatformHooksInterfaceGuid,
    0,
    (VOID **)&MrcPlatformHooksInterface
    );
  if (!EFI_ERROR (PpiStatus)) {
    MrcPlatformHooksInterface->PlatformPostAddressMapConfigured (Host);
  }

  //
  // Add MemMapData to host
  //
  AddMemMapDatatoHost (Host, MemMapData);

  Host->nvram.mem.volMemMode = Host->var.mem.volMemMode;
  Host->nvram.mem.CacheMemType = Host->var.mem.CacheMemType;

  if (MemMapData->HbmMemoryMapData != NULL) {
    RcFreePool (MemMapData->HbmMemoryMapData);
  }
  RcFreePool (MemMapData);

  return SUCCESS;
}
