/** @file
  Implementation of mem topology library.

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

#include <Library/HobLib.h>
#include <Library/MemMapDataLib.h>
#include <Library/RasSiliconLib.h>
#include <Library/RasRcDataLib.h>
#include <Library/RasDebugLib.h>
#include <Library/SystemInfoLib.h>
#include <IioUniversalData.h>
#include <SmbiosString.h>
#include <Library/KtiApi.h>

MEM_TOPOLOGY  *mMemTopology = NULL;


EFI_STATUS
EFIAPI
ShowMemoryTopology (
  IN MEM_TOPOLOGY  *MemTopology
  )
{
  UINT8              i, j, k, m, n, r;
  UINT8              MpLink;

  RAS_DEBUG((LEVEL_BASIC_FLOW, "\nShow Memory Topology - Start\n\n"));
  if (MemTopology == NULL) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "Show Memory Topology - End\n\n"));
    return EFI_INVALID_PARAMETER;
  }

  RAS_DEBUG((LEVEL_BASIC_FLOW, "RasModesEnabled = %x\n", MemTopology->RasModesEnabled));
  RAS_DEBUG((LEVEL_BASIC_FLOW, "RasModesSupported = %x  ExRasModesEnabled= %x\n",MemTopology->RasModesSupported, MemTopology->ExRasModesEnabled));
  RAS_DEBUG((LEVEL_BASIC_FLOW, "\n"));

  for (i = 0; i < MAX_SOCKET; i++) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "  Socket[%x].Valid = %x\n", i, MemTopology->Socket[i].Valid));

    if (MemTopology->Socket[i].Valid == 1) {
      for (MpLink = 0; MpLink < (GetKtiPortCnt()); MpLink++) {
        RAS_DEBUG((LEVEL_BASIC_FLOW, "    Socket[%x].UPIPortValid[%x] = %x\n", i, MpLink, MemTopology->Socket[i].UPIPortValid[MpLink]));
      }

      for (j = 0; j < MAX_IMC; j++) {
        RAS_DEBUG((LEVEL_BASIC_FLOW, "    Socket[%x].imcEnabled[%x] = %x\n", i, j, MemTopology->Socket[i].imcEnabled[j]));
        RAS_DEBUG((LEVEL_BASIC_FLOW, "    Socket[%x].imcEnabled[%x] SmbiosType16Handle:%x \n", i, j, MemTopology->Socket[i].SmbiosType16Handle[j]));
      }

      for (k = 0; k < IMC_MAX_CH; k++) {
        RAS_DEBUG((LEVEL_BASIC_FLOW, "    Socket[%x].ChannelInfo[%x].Enabled = %x\n", i, k, MemTopology->Socket[i].ChannelInfo[k].Enabled));
        RAS_DEBUG((LEVEL_BASIC_FLOW, "      Socket[%x].ChannelInfo[%x].Features = %x\n", i, k, MemTopology->Socket[i].ChannelInfo[k].Features));
        RAS_DEBUG((LEVEL_BASIC_FLOW, "      Socket[%x].ChannelInfo[%x].MaxDimm = %x\n", i, k, MemTopology->Socket[i].ChannelInfo[k].MaxDimm));
        RAS_DEBUG((LEVEL_BASIC_FLOW, "      Socket[%x].ChannelInfo[%x].NumRanks = %x\n", i, k, MemTopology->Socket[i].ChannelInfo[k].NumRanks));
        RAS_DEBUG((LEVEL_BASIC_FLOW, "      Socket[%x].ChannelInfo[%x].chFailed = %x\n", i, k, MemTopology->Socket[i].ChannelInfo[k].chFailed));
        RAS_DEBUG((LEVEL_BASIC_FLOW, "      Socket[%x].ChannelInfo[%x].ngnChFailed = %x\n", i, k, MemTopology->Socket[i].ChannelInfo[k].ngnChFailed));

          for (m = 0; m < MAX_SPARE_RANK; m++){
            RAS_DEBUG((LEVEL_BASIC_FLOW, "        Socket[%x].ChannelInfo[%x].SpareLogicalRank[%x] = %x\n", i, k, m, MemTopology->Socket[i].ChannelInfo[k].SpareLogicalRank[m]));
            RAS_DEBUG((LEVEL_BASIC_FLOW, "        Socket[%x].ChannelInfo[%x].SparePhysicalRank[%x] = %x\n", i, k, m, MemTopology->Socket[i].ChannelInfo[k].SparePhysicalRank[m]));
            RAS_DEBUG((LEVEL_BASIC_FLOW, "        Socket[%x].ChannelInfo[%x].SpareRankSize[%x] = %x\n", i, k, m, MemTopology->Socket[i].ChannelInfo[k].SpareRankSize[m]));
          }

          for(n = 0; n < MAX_DIMM; n++) {
            RAS_DEBUG((LEVEL_BASIC_FLOW, "        Socket[%x].ChannelInfo[%x].DimmInfo[%x].Present = %x\n", i, k, n, MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].Present));
            RAS_DEBUG((LEVEL_BASIC_FLOW, "        Socket[%x].ChannelInfo[%x].DimmInfo[%x].Enabled = %x\n", i, k, n, MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].Enabled));
            RAS_DEBUG((LEVEL_BASIC_FLOW, "        Socket[%x].ChannelInfo[%x].DimmInfo[%x].X4Present = %x\n", i, k, n, MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].X4Present));
            RAS_DEBUG((LEVEL_BASIC_FLOW, "        Socket[%x].ChannelInfo[%x].DimmInfo[%x].DimmSize = %x\n", i, k, n, MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].DimmSize));
            RAS_DEBUG((LEVEL_BASIC_FLOW, "        Socket[%x].ChannelInfo[%x].DimmInfo[%x].SmbiosType17Handle = %x\n", i, k, n, MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].SmbiosType17Handle));
            if (MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].Present) {
              for (r = 0; r < (MAX_RANK_DIMM * SUB_CH); r++) {
                RAS_DEBUG((LEVEL_BASIC_FLOW, "          Socket[%x].ChannelInfo[%x].DimmInfo[%x].rankList[%x].enabled = %x\n", i, k, n, r, MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].rankList[r].enabled));
                RAS_DEBUG((LEVEL_BASIC_FLOW, "          Socket[%x].ChannelInfo[%x].DimmInfo[%x].rankList[%x].logicalRank = %x\n", i, k, n, r, MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].rankList[r].logicalRank));
                RAS_DEBUG((LEVEL_BASIC_FLOW, "          Socket[%x].ChannelInfo[%x].DimmInfo[%x].rankList[%x].rankSize = %x\n", i, k, n, r, MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].rankList[r].rankSize));
              }
            }
          }
      }
    }
    RAS_DEBUG((LEVEL_BASIC_FLOW, "\n"));
  }

  RAS_DEBUG((LEVEL_BASIC_FLOW, "Show Memory Topology - End\n\n"));

  return EFI_SUCCESS;
}

/**
  Initialize mem topology.

  @param  OUT MEM_TOPOLOGY

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
InitializeMemTopology (
  OUT MEM_TOPOLOGY  *MemTopology
  )
{
  struct SystemMemoryMapHob  *SystemMemoryMap = NULL;
  EFI_GUID                    UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  IIO_UDS                    *UdsHobPtr;
  EFI_HOB_GUID_TYPE          *GuidHob;
  UINT8                       i, j, k, m, n, r;
  UINT8                       MpLink;
  RAS_RC_CONFIG               *RasRcConfig;
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;

  RAS_DEBUG ((LEVEL_BASIC_FLOW, "  InitializeMemTopology start"));
  SystemMemoryMap = GetSystemMemoryMapData ();
  if (SystemMemoryMap == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "  [Error] Get memory map data failed!"));
    return EFI_NOT_FOUND;
  }

  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  RAS_ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "  [Error] Get IIO data failed!"));
    return EFI_NOT_FOUND;
  }
  UdsHobPtr = GET_GUID_HOB_DATA (GuidHob);

  RasRcConfig = GetRasRcConfig ();
  RAS_ASSERT (RasRcConfig != NULL);
  if (RasRcConfig == NULL) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, "  [Error] Get RAS RC config failed!"));
    return EFI_NOT_FOUND;
  }
  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();

  MemTopology->SystemRasType = SystemMemoryMap->SystemRasType;
  MemTopology->RasModesEnabled = SystemMemoryMap->RasModesEnabled;
  MemTopology->ExRasModesEnabled = SystemMemoryMap->ExRasModesEnabled;
  MemTopology->RasModesSupported = RasRcConfig->SupportedRasModes;
  MemTopology->VolMemMode = SystemMemoryMap->volMemMode;
  MemTopology->CacheMemType = SystemMemoryMap->CacheMemType;
  MemTopology->EkvPresent = SystemMemoryMap->EkvPresent;
  MemTopology->BwvPresent = SystemMemoryMap->BwvPresent;
  MemTopology->DieCount = GetDieCount (0);

  SetMemTopoSmbios (MemTopology);

  for(i = 0; i < MAX_SOCKET; i++) {
    if (( CpuCsrAccessVarPtr->socketPresentBitMap & (UINT32) 1 << i) == 0) {
      continue;
    }
    MemTopology->Socket[i].Valid = 1;
    MemTopology->Socket[i].LLCMap = GetLlcSliceBitMap (i);
    MemTopology->Socket[i].LlcIaCoreEnBitMap = GetLlcIaCoreEnBitMap (i);
    MemTopology->Socket[i].TotalCha = GetTotChaCount (i);

    for (MpLink = 0; MpLink < (GetKtiPortCnt()); MpLink++) {
      MemTopology->Socket[i].UPIPortValid[MpLink] = UdsHobPtr->PlatformData.CpuQpiInfo[i].PeerInfo[MpLink].Valid;
    }

    for (j = 0; j < MAX_IMC; j++) {
      MemTopology->Socket[i].imcEnabled[j] = SystemMemoryMap->Socket[i].imcEnabled[j];
      MemTopology->Socket[i].PatrolScrubInterval[j] = RasRcConfig->PatrolScrubInterval[i][j];
    }

    for (k = 0; k < IMC_MAX_CH; k++) {
      MemTopology->Socket[i].ChannelInfo[k].Enabled = SystemMemoryMap->Socket[i].ChannelInfo[k].Enabled;
      MemTopology->Socket[i].ChannelInfo[k].Features = SystemMemoryMap->Socket[i].ChannelInfo[k].Features;
      MemTopology->Socket[i].ChannelInfo[k].MaxDimm = SystemMemoryMap->Socket[i].ChannelInfo[k].MaxDimm;
      MemTopology->Socket[i].ChannelInfo[k].NumRanks = SystemMemoryMap->Socket[i].ChannelInfo[k].NumRanks;
      MemTopology->Socket[i].ChannelInfo[k].chFailed = SystemMemoryMap->Socket[i].ChannelInfo[k].chFailed;
      MemTopology->Socket[i].ChannelInfo[k].ngnChFailed = SystemMemoryMap->Socket[i].ChannelInfo[k].ngnChFailed;
      MemTopology->DdrCacheSize[i][k] = SystemMemoryMap->DdrCacheSize[i][k];

      if (SystemMemoryMap->Socket[i].ChannelInfo[k].Enabled) {
        for (m = 0; m < MAX_SPARE_RANK; m++) {
          MemTopology->Socket[i].ChannelInfo[k].SpareLogicalRank[m] = SystemMemoryMap->Socket[i].ChannelInfo[k].SpareLogicalRank[m];
          MemTopology->Socket[i].ChannelInfo[k].SparePhysicalRank[m] = SystemMemoryMap->Socket[i].ChannelInfo[k].SparePhysicalRank[m];
          MemTopology->Socket[i].ChannelInfo[k].SpareRankSize[m] = SystemMemoryMap->Socket[i].ChannelInfo[k].SpareRankSize[m];
        }

        for (n = 0; n < MAX_DIMM; n++) {
          MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].Present = SystemMemoryMap->Socket[i].ChannelInfo[k].DimmInfo[n].Present;
          MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].AepDimmPresent = SystemMemoryMap->Socket[i].ChannelInfo[k].DimmInfo[n].DcpmmPresent;
          MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].Enabled = SystemMemoryMap->Socket[i].ChannelInfo[k].DimmInfo[n].Enabled;
          MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].X4Present = SystemMemoryMap->Socket[i].ChannelInfo[k].DimmInfo[n].X4Present;
          MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].nonVolCap = SystemMemoryMap->Socket[i].ChannelInfo[k].DimmInfo[n].nonVolCap;
          MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].DimmSize = SystemMemoryMap->Socket[i].ChannelInfo[k].DimmInfo[n].DimmSize;

          if (SystemMemoryMap->Socket[i].ChannelInfo[k].DimmInfo[n].Present) {
            for (r = 0; r < (MAX_RANK_DIMM * SUB_CH); r++) {
              MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].rankList[r].enabled = SystemMemoryMap->Socket[i].ChannelInfo[k].DimmInfo[n].rankList[r/SUB_CH].enabled;
              MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].rankList[r].logicalRank = SystemMemoryMap->Socket[i].ChannelInfo[k].DimmInfo[n].rankList[r/SUB_CH].logicalRank;
              MemTopology->Socket[i].ChannelInfo[k].DimmInfo[n].rankList[r].rankSize = SystemMemoryMap->Socket[i].ChannelInfo[k].DimmInfo[n].rankList[r/SUB_CH].rankSize;
            }
          }
        }
      }
    }
  }

  //
  // Get Partial Mirror Info
  //
  MemTopology->MirrorCurrentType.MirrorVersion = SystemMemoryMap->RasMeminfo.MirrorCurrentType.MirrorVersion;
  MemTopology->MirrorCurrentType.MirrorMemoryBelow4GB = SystemMemoryMap->RasMeminfo.MirrorCurrentType.MirrorMemoryBelow4GB;
  MemTopology->MirrorCurrentType.MirroredAmountAbove4GB = SystemMemoryMap->RasMeminfo.MirrorCurrentType.MirroredAmountAbove4GB;
  MemTopology->MirrorCurrentType.MirrorStatus = SystemMemoryMap->RasMeminfo.MirrorCurrentType.MirrorStatus;

  MemTopology->MirrorRequestType.MirrorVersion = SystemMemoryMap->RasMeminfo.MirrorRequestType.MirrorVersion;
  MemTopology->MirrorRequestType.MirrorMemoryBelow4GB = SystemMemoryMap->RasMeminfo.MirrorRequestType.MirrorMemoryBelow4GB;
  MemTopology->MirrorRequestType.MirroredAmountAbove4GB = SystemMemoryMap->RasMeminfo.MirrorRequestType.MirroredAmountAbove4GB;
  MemTopology->MirrorRequestType.MirrorStatus = SystemMemoryMap->RasMeminfo.MirrorRequestType.MirrorStatus;

  //
  // Show Memory Topology
  //
  ShowMemoryTopology (MemTopology);

  return EFI_SUCCESS;
}


/**
  Get memory topology data, in case first one invoke this routine, it will initialize topology data.

  @retval  point of memory topology info

**/
MEM_TOPOLOGY  *
EFIAPI
GetMemTopology (
  VOID
  )
{
  if (mMemTopology->MemTopologyInitFlag == FALSE) {
    InitializeMemTopology (mMemTopology);
    mMemTopology->MemTopologyInitFlag = TRUE;
  }
  return mMemTopology;
}


/**
  Get the number of DDR4 DIMMs in the channel

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number

  @retval - The number of DIMMs

**/
UINT8
EFIAPI
GetNumOfDimms (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode
  )
{
  UINT8             Socket;
  UINT8             ChOnSkt;
  MEM_TOPOLOGY      *MemTopology;
  UINT8             Ddr4DimmPerCh;
  UINT8             Index;

  RAS_ASSERT (NodeId < MC_MAX_NODE);
  RAS_ASSERT (ChOnNode < MAX_MC_CH);

  Socket = NODE_TO_SKT (NodeId);
  ChOnSkt= NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    return FALSE;
  }

  Ddr4DimmPerCh = MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].MaxDimm;
  for (Index = 0; Index < MAX_DIMM; Index ++) {
    if (MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].DimmInfo[Index].AepDimmPresent) {
      Ddr4DimmPerCh --;
    }
  }

  return Ddr4DimmPerCh;
}


/**
  Check whether the socket is present

  @param  Skt             - socket ID

  @retval TRUE          - Socket exist
          FALSE         - Socket not exist

**/
BOOLEAN
EFIAPI
IsSocketPresent (
  IN  UINT8  Skt
  )
{
  MEM_TOPOLOGY      *MemTopology;

  RAS_ASSERT (Skt < MAX_SOCKET);

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    return FALSE;
  }

  if (MemTopology->Socket[Skt].Valid) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  Check whether the node is present

  @param  NodeId          - Memory controller ID

  @retval Status          - TRUE/FALSE

**/
BOOLEAN
EFIAPI
IsNodePresent (
  IN  UINT8         NodeId
  )
{
  UINT8             Socket;
  UINT8             Mc;
  MEM_TOPOLOGY      *MemTopology;

  RAS_ASSERT (NodeId < MC_MAX_NODE);

  Socket = NODE_TO_SKT (NodeId);
  Mc     = NODE_TO_MC (NodeId);

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    return FALSE;
  }

  if (MemTopology->Socket[Socket].imcEnabled[Mc]) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  Check whether the channel is present per socket

  @param  Socket          - Socket ID
  @param  ChOnSkt         - Channel number per socket

  @retval Status          - TRUE/FALSE

**/
BOOLEAN
EFIAPI
IsChannelPresentPerSocket (
  IN  UINT8         Socket,
  IN  UINT8         ChOnSkt
  )
{
  MEM_TOPOLOGY      *MemTopology;

  RAS_ASSERT (ChOnSkt < IMC_MAX_CH);

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    return FALSE;
  }

  if (!IsSocketPresent (Socket)) {
    return FALSE;
  }

  if (MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].Enabled) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  Check whether the channel is present

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number

  @retval Status          - TRUE/FALSE

**/
BOOLEAN
IsChannelPresent (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode
  )
{
  UINT8             Socket;
  UINT8             ChOnSkt;

  RAS_ASSERT (NodeId < MC_MAX_NODE);
  RAS_ASSERT (ChOnNode < MAX_MC_CH);

  Socket = NODE_TO_SKT (NodeId);
  ChOnSkt= NODECHA_TO_SKTCHA (NodeId, ChOnNode);

  return (IsChannelPresentPerSocket (Socket, ChOnSkt));
}


/**
  Get the number of DIMMs in a channel

  @param  NodeId          - Memory controller ID
  @param  ChOnNode        - Channel number
  @param  Rank            - Rank Number

  @retval Status          - TRUE/FALSE

**/
BOOLEAN
EFIAPI
IsRankEnabled (
  IN  UINT8         NodeId,
  IN  UINT8         ChOnNode,
  IN  UINT8         Rank
  )
{
  UINT8             Socket;
  UINT8             ChOnSkt;
  MEM_TOPOLOGY      *MemTopology;
  UINT8             Dimm;
  UINT8             RankOnDimm;
  BOOLEAN           Ret;

  RAS_ASSERT (NodeId < MC_MAX_NODE);
  RAS_ASSERT (ChOnNode < MAX_MC_CH);
  RAS_ASSERT (Rank < (MAX_RANK_CH * SUB_CH));

  Socket = NODE_TO_SKT (NodeId);
  ChOnSkt= NODECHA_TO_SKTCHA (NodeId, ChOnNode);
  MemTopology = GetMemTopology ();

  Dimm       = Rank / (MAX_RANK_DIMM * SUB_CH);
  RankOnDimm = Rank % (MAX_RANK_DIMM * SUB_CH);

  if (MemTopology == NULL) {
    Ret = FALSE;
  } else if (!IsNodePresent (NodeId)) {
    RAS_DEBUG((LEVEL_BASIC_FLOW, "\t Node 0x%x is not present!\n", NodeId));
    Ret = FALSE;
  } else {
    Ret = MemTopology->Socket[Socket].ChannelInfo[ChOnSkt].DimmInfo[Dimm].rankList[RankOnDimm].enabled;
    if (!Ret) {
      RAS_DEBUG((LEVEL_BASIC_FLOW, "\tS 0x%x, CH 0x%x, Dimm 0x%x, RankOnDimm 0x%x is not enabled!\n",
            Socket, ChOnNode, Dimm, RankOnDimm));
    }
  }

  return Ret;
}

/**
  check if current imc is under 1LM mode or not

  @param  none
  @retval TRUE -- 1LM; FALSE -- Not 1LM.
**/
BOOLEAN
EFIAPI
IsImc1LmMode (
  VOID
  )
{
  MEM_TOPOLOGY      *MemTopology;

  MemTopology = GetMemTopology ();
  if (MemTopology == NULL) {
    return FALSE;
  }

  return (MemTopology->VolMemMode == VOL_MEM_MODE_1LM);
}
