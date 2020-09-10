/** @file

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

#include <MemDecode.h>
#include <Gen2/MemDecodeNode.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/KtiApi.h>
#include <Library/MemRcLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Guid/MemoryMapData.h>
#include <Library/MemTypeLib.h>
#include <Library/RcDebugLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/UsraCsrLib.h>
#include <Library/PsmiConfig.h>
#include <Library/HobLib.h>
#include <Include/MemMapHost.h>
#include <Library/ProcMemInitChipLib.h>
#include <Guid/FpgaIpInterface.h>
#include <Library/MemDecodeHbmLib.h>
#include <Library/SiliconWorkaroundLib.h>

//
// Module variables
//

#ifdef DDRT_SUPPORT
/**

  Check if 1LM memory has multiple x1 interleaves before mapping AD x1.

  @param[in]  MemMapData    Pointer to the memory map private data structure.
  @param[in]  Sckt          Socket instance

  @retval TRUE  1LM memory has >1 x1 interleaves.
  @retval FALSE System is in UMA or 2LM mode.

**/
BOOLEAN
Check1lmX1IntlvForAdX1 (
  IN  MEMORY_MAP_DATA         *MemMapData,
  IN  UINT8                    Sckt
  )
{
  SYSHOST    *Host;
  UINT8      EndCluster;
  UINT8      Cluster;

  Host = GetSysHostPointer ();

  //
  // If 1LM memory interleaving is UMA then x1 interleave is not possible.
  // If system is in 2LM mode then x1 interleave is not possible.
  //
  if (!MemMapData->IsNumaEn || !IS_SYSTEM_1LM(Host)) {
    return FALSE;
  }

  if (IfFeatureEnabled (TypeSnc)) {
    EndCluster = GetNumOfClusterPerSystem ();
  } else {
    EndCluster = 1;
  }

  //
  // Check per each cluster if number of x1 interleave(s) is > 1
  //
  for (Cluster = 0; Cluster < EndCluster; Cluster++) {
    if (MemMapData->MemMapSocket[Sckt].Ddr1lmX1IntlvCount[Cluster] > 1) {
      return TRUE;
    }
  }

  return FALSE;
}
#endif // DDRT_SUPPORT

/**

  Check if the input memory type can support UMA-Based Clustering on the SOC.

  @param[in]  MemMapData    Pointer to the memory map private data structure.

  @retval TRUE  The current memory type is allowed for UMA-Based Clustering.
  @retval FALSE The current memory type is not allowed for UMA-Based Clustering.

**/
BOOLEAN
IsUbcSupportedMemType (
  IN MEM_TYPE MemType
  )
{
  //
  // Check volatile memory types
  //
  if (IsMemTypeVolatile (MemType) || IsMemTypeHbm (MemType)) {
    return TRUE;
  }

  return FALSE;
}

#ifdef DDRT_SUPPORT
/**

  Find data related to memory range cached by NM

  @param[in]  Host             Pointer to sysHost.
  @param[in]  MemMapData       Pointer to the memory map private data structure.
  @param[in]  Sckt             Socket number.
  @param[in]  Mc               MC index.
  @param[in]  McCluster        Cluster Id of MC for multicast access.
  @param[out] CacheRangeHigh   Pointer to store High address of the memory range being cached.
  @param[out] CacheRangeLow    Pointer to store Low address of the memory range being cached.

  @retval EFI_SUCCESS

**/
UINT8
FindCachedRange (
  IN     PSYSHOST         Host,
  IN     MEMORY_MAP_DATA  *MemMapData,
  IN     UINT8            Sckt,
  IN     UINT8            Mc,
  IN     UINT8            McCluster,
     OUT UINT32           *CacheRangeHigh,
     OUT UINT32           *CacheRangeLow
  )
{
  MEMORY_MAP_HOST *MemMapHost;
  UINT8           SadIndex;
  SAD_TABLE       *SadEntry;
  MEM_VAR_STRUCT  *Mem;
  UINT8           BeginSadIndex;
  UINT8           EndSadIndex;
  UINT8           ClusterId;
  BOOLEAN         CacheRangeLowAssigned;

  MemMapHost = GetMemMapHostPointer ();
  *CacheRangeHigh = 0;
  *CacheRangeLow = 0;
  Mem = &Host->var.mem;
  CacheRangeLowAssigned = FALSE;

  //
  // Point Mc to the correct index in the SAD interleave table
  //
  Mc = Mc + (Sckt * GetMaxImc ());

  for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
    if ((MemMapData->NumberofChaDramClusters > 1) && (ClusterId != McCluster)) {
      continue;
    }
    BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
    EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
    if (Host->var.mem.socket[Sckt].SAD[BeginSadIndex].Enable == 0) {
      break;
    }
    //
    // Scan through the SADs to find the lowest address of the memory range that is being cached.
    //
    for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
      SadEntry = &Mem->socket[Sckt].SAD[SadIndex];
      if (SadEntry->Enable == 0) {
        break;
      }
      //
      // Skip remote SADs
      //
      if (SadEntry->local == 0) {
        continue;
      }
      //
      // Skip if the MC is not part of the SAD
      //
      if (MemMapHost->SocketInfo[Sckt].SadIntList[SadIndex][Mc] != 1) {
        continue;
      }
      //
      // Skip SADs that are not of MemType 2LM or PMEM$
      //
      if ((SadEntry->type != MemType2lmDdrCacheMemoryMode) &&
        (SadEntry->type != MemType2lmDdrWbCacheAppDirect)) {
        continue;
      }

      if (!CacheRangeLowAssigned) {
        *CacheRangeLow = CalculateCacheRangeLow (SadIndex, Mem->socket[Sckt].SAD[SadIndex].Base);
        CacheRangeLowAssigned = TRUE;
      }

      if (SadEntry->Limit > *CacheRangeHigh) {
        *CacheRangeHigh = SadTadEncodeLimit (SadEntry->Limit);
      }
    }
  }
  return EFI_SUCCESS;
}
#endif // DDRT_SUPPORT

/**
  Gets printable UMA-Based Clustering type.

  @param[in] UmaBasedClusters   The number of UMA-based clusters

  @retval   -   String with UMA-Based Clustering type

**/
STATIC const CHAR8 *GetUbcTypeString (
  IN UINT8  UmaBasedClusters
  )
{
  switch (UmaBasedClusters) {
  case UMABASEDCLUSTERING_HEMISPHERE:
    return "Hemi   ";
  case UMABASEDCLUSTERING_QUADRANT:
    return "Quad   ";
  default:
    return "All2All";
  }
}

/**

  Get the interleave channel ways, MC ways, channel and MC bitmaps from input SAD instance.

  @param[in]  MemMapData    - Pointer to the memory map private data structure.
  @param[in]  Sckt          - Socket Id
  @param[in]  SadIndex      - SAD Id
  @param[out] McWays        - Pointer to the variable to update with interleave MC Ways.
  @param[out] ChWays        - Pointer to the variable to update with interleave channel Ways.
  @param[out] ChannelBitmap - Pointer to the variable to update wth Channel bitmaps for the interleave.
  @param[out] McBitmap      - Pointer to the variable to update with MC bitmap for the interleave.

**/
VOID
GetInterleaveWays (
  IN MEMORY_MAP_DATA   *MemMapData,
  IN  UINT8            Sckt,
  IN  UINT8            SadIndex,
  OUT UINT8            *McWays,
  OUT UINT8            *ChWays,
  OUT UINT8            ChannelBitmap[MAX_IMC],
  OUT UINT8            *McBitmap
  )
{
  SYSHOST    *Host;
  SAD_TABLE  *SadEntry;

  if (MemMapData == NULL || McWays == NULL || ChWays == NULL || ChannelBitmap == NULL || McBitmap == NULL) {
    return;
  }

  Host = GetSysHostPointer ();
  SadEntry = &Host->var.mem.socket[Sckt].SAD[SadIndex];

  switch (SadEntry->type) {
    case MemType1lmHbm:
    case MemType2lmHbmCacheDdr:
    case MemType1lmDdr:
    case MemType2lmDdrCacheMemoryMode:
    case MemType2lmDdrWbCacheAppDirect:
      *ChWays = SadEntry->NmChWays;
      CopyMem (ChannelBitmap, SadEntry->channelInterBitmap, sizeof (SadEntry->channelInterBitmap));
#ifdef DDRT_SUPPORT
      if (IsMemType2lm (SadEntry->type) && MemMapData->MemMapSocket[Sckt].XTile2lm) {
        *McBitmap = SadEntry->NmImcInterBitmap;
        *McWays = CalculateInterleaveWaysfromBitmap (SadEntry->NmImcInterBitmap);
      } else
#endif // DDRT_SUPPORT
      {
        *McBitmap = SadEntry->imcInterBitmap;
        *McWays = CalculateInterleaveWaysfromBitmap (SadEntry->imcInterBitmap);
      }
      break;
#ifdef DDRT_SUPPORT
    case MemType1lmAppDirect:
      *ChWays = SadEntry->FmChWays;
      *McWays = CalculateInterleaveWaysfromBitmap (SadEntry->imcInterBitmap);
      CopyMem (ChannelBitmap, SadEntry->FMchannelInterBitmap, sizeof (SadEntry->FMchannelInterBitmap));
      *McBitmap = SadEntry->imcInterBitmap;
      break;
#endif // DDRT_SUPPORT
    default:
      *ChWays = SadEntry->NmChWays;
      *McWays = CalculateInterleaveWaysfromBitmap (SadEntry->imcInterBitmap);
      CopyMem (ChannelBitmap, SadEntry->channelInterBitmap, sizeof (SadEntry->channelInterBitmap));
      *McBitmap = SadEntry->imcInterBitmap;
      break;
  }
}

/**

  Sort channels in MC based on remsize value in descending order.

  @param[in]  Host             - Pointer to sysHost
  @param[in]  SocketNum        - Socket Id
  @param[in]  ChIndex          - Index of first channel in the IMC that needs to be sorted
  @param[out] ChannelSortList  - Channels per MC in order of decreasing size.

**/
VOID
ChannelSortMc (
  IN  PSYSHOST            Host,
  IN  UINT8               SocketNum,
  IN  UINT8               ChIndex,
  OUT CHANNEL_SORT_STRUCT ChannelSortList[]
  )
{
  UINT8     Index;
  UINT8     Ch;
  UINT8     SortIndex;
  UINT8     SortNum;
  UINT8     MaxChPerMc;
  UINT32    RemSize;
  UINT8     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  SortNum = 0;

  if (Host == NULL || ChannelSortList == NULL) {
    return;
  }

  MaxChPerMc = GetNumChannelPerMc ();
  if (SocketNum >= MAX_SOCKET || (ChIndex % MaxChPerMc != 0) || ChIndex >= MaxChDdr) {
    return;
  }

  for (Ch = ChIndex; Ch < (ChIndex + MaxChPerMc); Ch++) {

    RemSize = Host->var.mem.socket[SocketNum].channelList[Ch].remSize;
    if (RemSize == 0) {
      continue;
    }

    //
    // Determine sort list position
    //
    for (SortIndex = 0; SortIndex < SortNum; SortIndex++) {
      if (ChannelSortList[ChIndex + SortIndex].chSize < RemSize) {
        break;
      }
    }
    //
    // Shift smaller channels towards end of list
    //
    if (SortIndex < SortNum) {
      for (Index = SortNum; Index > SortIndex; Index--) {
        ChannelSortList[ChIndex + Index] = ChannelSortList[ChIndex + Index - 1];
      }
    }
    //
    // Insert current channel
    //
    ChannelSortList[ChIndex + SortIndex].chNum = Ch - ChIndex;
    ChannelSortList[ChIndex + SortIndex].socketNum = SocketNum;
    ChannelSortList[ChIndex + SortIndex].chSize = RemSize;

    SortNum++;
  } // Channel Loop

  return;
}

/**

  Sort channels in half of the Socket based on remsize value in descending order.

  @param[in]  Host             - Pointer to sysHost
  @param[in]  SocketNum        - Socket Id
  @param[in]  Half             - The half to sort channels in
  @param[in]  InterleaveWays   - Total interleave ways (MC * channel) requested
  @param[out] ChannelSortList  - Channels per socket in order of decreasing size.

**/
VOID
ChannelSortHalf (
  IN  PSYSHOST            Host,
  IN  UINT8               SocketNum,
  IN  ROUTE_TABLE_HALVES  Half,
  IN  UINT8               InterleaveWays,
  OUT CHANNEL_SORT_STRUCT ChannelSortList[MAX_CH]
  )
{
  UINT8               Index;
  UINT8               McIndex;
  UINT8               MaxImc;
  UINT8               ChIndex;
  UINT8               SortIndex;
  UINT8               SortNum;
  UINT8               MaxChPerMc;
  UINT32              RemSize;
  CHANNEL_SORT_STRUCT ChannelSortListInt[MAX_CH];
  UINT8               FirstMcInHalf;

  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  SortNum = 0;
  ZeroMem (ChannelSortListInt, sizeof (ChannelSortListInt));
  FirstMcInHalf = 0;

  if (Host == NULL || ChannelSortList == NULL) {
    return;
  }

  if (SocketNum >= MAX_SOCKET || InterleaveWays > (MAX_MC_HALF * MaxChPerMc)) {
    return;
  }

  //
  // Loop MC based on half
  //
  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if (GetHalfForMc (McIndex) != Half) {
      continue;
    }
    FirstMcInHalf = (UINT8) LowBitSet32 (GetMcBitMaskForHalf (Half));
    for (ChIndex = 0; ChIndex < MaxChPerMc; ChIndex++) {
      RemSize = Host->var.mem.socket[SocketNum].channelList[(McIndex * MaxChPerMc) + ChIndex].remSize;
      if (RemSize == 0) {
        continue;
      }

      //
      // Determine sort list position
      //
      for (SortIndex = 0; SortIndex < SortNum; SortIndex++) {
        if (ChannelSortListInt[SortIndex].chSize < RemSize) {
          break;
        }
      }
      //
      // Shift smaller channels towards end of list
      //
      if (SortIndex < SortNum) {
        for (Index = SortNum; (Index > SortIndex) && (Index > 0); Index--) {
          ChannelSortListInt[Index] = ChannelSortListInt[Index - 1];
        }
      }
      //
      // Insert current channel
      //
      ChannelSortListInt[SortIndex].chNum = ChIndex + (McIndex * MaxChPerMc);
      ChannelSortListInt[SortIndex].socketNum = SocketNum;
      ChannelSortListInt[SortIndex].chSize = RemSize;

      SortNum++;
    } // Channel Loop
  } // Mc loop

  //
  // Before return copy the channel sort list position for the MC's corresponding to the half
  //
  CopyMem (
    &ChannelSortList[FirstMcInHalf * MaxChPerMc],
    ChannelSortListInt,
    sizeof (CHANNEL_SORT_STRUCT) * InterleaveWays
    );

  return;
}

/**

  This routine copies the volSize/DDR size into the remSize field of each channel in the sockets specified based on the volatile memory mode

  @param[in] Host         - Pointer to sysHost
  @param[in] MemMapData   - Pointer to memory map private data structure
  @param[in] SocketBitMap - BitMap of sockets to process
  @param[in] MemType      - Memory type to update for (only used for 10nm, not SKX)

  @retval SUCCESS

**/
VOID
UpdateHostStructForVolMem (
  IN PSYSHOST          Host,
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             SocketBitMap,
  IN MEM_TYPE          MemType
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
      if (MemType == MemType2lmDdrCacheMemoryMode || MemType == MemType2lmHbmCacheMemoryMode) {
        if (MemMapData->MemMapSocket[SockId].XTile2lmSncFmPartition) {
          Host->var.mem.socket[SockId].channelList[Ch].remSize = MemMapData->MemMapSocket[SockId].XTile2lmSncVolSize[Ch];
        } else {
          Host->var.mem.socket[SockId].channelList[Ch].remSize = Host->var.mem.socket[SockId].channelList[Ch].volSize;
        }
      } else if (MemType == MemType2lmDdrWbCacheAppDirect) {
        Host->var.mem.socket[SockId].channelList[Ch].remSize = Host->var.mem.socket[SockId].channelList[Ch].WbCachePerSize;
      } else {
        if (PcdGetBool (PcdNvDimmEn)) {
          Host->var.mem.socket[SockId].channelList[Ch].remSize = GetChannelMemSize (SockId, Ch) - Host->var.mem.socket[SockId].channelList[Ch].NVmemSize;
        } else {
          Host->var.mem.socket[SockId].channelList[Ch].remSize = GetChannelMemSize (SockId, Ch);
        }
        //
        // When Mixed 1LM+2LM is enabled, exclude the reserved DdrCacheSize from the 1LM VolMem interleaving algorithm
        //
        if (Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
          Host->var.mem.socket[SockId].channelList[Ch].remSize -= Host->var.mem.socket[SockId].channelList[Ch].DdrCacheSize;
        }
      }
    }
  } // sockId
}


/**
  Populate HBM size fields.

  Populate the HBM memory map private data structure with the size of available
  HBM in the system, by reading the output data from HBM IO training.

  @param[in] MemMapData  Pointer to the memory map private data structure.

  @return                Total amount of HBM in the system expressed in
                         units of 64MB.
**/
UINT32
PopulateHbmSizeFieldsEx (
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8            Sckt;
  UINT32           TotalHbmSize;
  UINT32           HbmSize;

  if (MemMapData->HbmMemoryMapData == NULL) {
    return 0;
  }

  TotalHbmSize = 0;

  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }

    HbmSize = PopulateHbmSizeFields (MemMapData, Sckt);
    RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Hbm size = %d\n", HbmSize);
    TotalHbmSize += HbmSize;
  }

  RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Total Hbm size = %d\n", TotalHbmSize);

  return TotalHbmSize;
}

/** Get the next volatile memory type to map.

  Get the next volatile memory type to map, based on policy and population.
  The value pointed to by Tracker should be set to zero before the first call
  to get the first volatile memory type to map. If the function's return value
  is zero, then there are no more memory types to map. Otherwise, the return
  value indicates the next volatile memory type to map. The caller should pass
  the returned Tracker value back to this function to get the next volatile
  memory type to map.

  Tracker values other than zero do not have any specified meaning. The caller
  must not set any other value.

  @param[in]      Host        Pointer to the Host structure.
  @param[in]      MemMapData  Pointer to the memory map private data structure.
  @param[in]      Socket      Index of the socket to consider.
  @param[in, out] Tracker     Pointer to tracking data.

  @retval MemType1lmDdr                  Map DDR (1LM) next.
  @retval MemType2lmDdrCacheMemoryMode   Map DDR caching DDRT (2LM) next.
  @retval MemType2lmDdrWbCacheAppDirect  Map DDR write back caching DDRT (2LM) next.
  @retval MemType1lmHbm                  Map flat HBM (1LM) next.
  @retval MemType2lmHbmCacheDdr          Map HBM caching DDR (2LM) next.
  @retval MemType2lmHbmCacheMemoryMode   Map HBM caching DDRT (2LM) next.
  @retval MemTypeNone                    No more volatile memory types to map.
**/
MEM_TYPE
GetNextVolMemTypeToMap (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8             Socket,
  IN UINTN            *Tracker
  )
{
  MEM_TYPE MemType;
  UINTN    Ordinal;

  MemType = MemTypeNone;
  Ordinal = *Tracker;

  if (Ordinal == 0) {
    if (GetSocketMemSize (Socket) != 0) {
      if ((IS_SYSTEM_1LM (Host)) || IS_DDR_MIXED_1LM2LM (Host)) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Map 1LM (DDR)\n"
          );
        MemType = MemType1lmDdr;
      }
    }

    Ordinal++;
  }

  if (MemType == MemTypeNone && Ordinal == 1) {
#ifdef DDRT_SUPPORT
    if (Host->var.mem.socket[Socket].volSize != 0) {
      if (IS_DDR_2LM_OR_MIXED (Host)) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Map 2LM (DDR/DDRT)\n"
          );
        MemType = MemType2lmDdrCacheMemoryMode;
      }
    }
#endif // DDRT_SUPPORT

    Ordinal++;
  }

  //
  // Ordinal 2 is HBM flat mode
  //
  if (MemType == MemTypeNone && Ordinal == 2) {
    if (IsHbmEnabledInSkt (MemMapData, Socket)) {
      if (!IS_HBM_2LM_CACHE_DDR (Host)) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Map 1LM HBM\n"
          );
        MemType = MemType1lmHbm;
      }
    }

    Ordinal++;
  }

  if (MemType == MemTypeNone && Ordinal == 3) {
#ifdef DDRT_SUPPORT
    //
    // AD-WB similar to AD shall co-exist with a volatile memory mode (2LM/1LM + 2LM)
    //
    if ((Host->var.mem.socket[Socket].WbCachePerSize != 0) && (MemMapData->PmemCaching == NativePmCaching)) {
      if (IS_DDR_2LM_OR_MIXED (Host)) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Map AD-WB (DDR/PMEM Persistent)\n"
        );
        MemType = MemType2lmDdrWbCacheAppDirect;
      }
    }
#endif // DDRT_SUPPORT

    Ordinal++;
  }

  //
  // Ordinal 4 is HBM 2LM Cache DDR
  //
  if (MemType == MemTypeNone && Ordinal == 4) {
    if (GetSocketMemSize (Socket) != 0) {
      if (IS_SYSTEM_2LM (Host) && IS_HBM_2LM_CACHE_DDR (Host)) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Map 2LM (HBM/DDR)\n"
          );
        MemType = MemType2lmHbmCacheDdr;
      }
    }

    Ordinal++;
  }

  if (MemType != MemTypeNone && MemType != MemType1lmHbm) {
    //
    // Update each channel remsize in the host struct for this socket before calling SadNuma() to create 1LM/2LM SADs.
    //
    UpdateHostStructForVolMem (
      Host,
      MemMapData,
      (BIT0 << Socket),
      MemType
      );
  }

  if (MemType == 0) {
    *Tracker = 0;             // no more types to map; next call gets first type
  } else {
    *Tracker = Ordinal;
  }

  return MemType;
}

  /** Use mod3 256B decoder and disable 256B XOR mode.

  @param[in, out] MemMapData      Pointer to the memory map data structure.
  @param[in]      SocketIndex     Socket ID.

  @retval    None

**/
VOID
DisableXorModeForMod3 (
  IN OUT  MEMORY_MAP_DATA   *MemMapData,
  IN      UINT8             SocketIndex
  )
{
  MEMORY_MAP_HOST  *MemMapHost;

  MemMapHost = GetMemMapHostPointer ();
  MemMapHost->SocketInfo[SocketIndex].XorMode &= ~CHANNEL_XOR;

  //
  // UBC cannot support mod3 in ICX and SPR due to address aliasing problems. When Hemi/Quad is enabled A[8] cannot
  // participate both in UMA XOR cluster ID selection (SA[8]^SA[11]^SA[17]^SA[25]) and Mod3 operation.
  //
  if (MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) {
    MemMapData->UmaBasedClustersPerSocket = UMABASEDCLUSTERING_DISABLED;
  }
}

/** Calculate interleave wayness from the bitmap.

  Calculate interleave wayness from the input Socket, MC or channel bitmaps.

  @param[in] Bitmap     - Bitmap of the interleave.

  @retval Ways          - Interleave ways

**/
UINT8
CalculateInterleaveWaysfromBitmap (
  IN UINT8 Bitmap
  )
{
  UINT8   Index;
  UINT8   Ways;

  Ways = 0;

  //
  // Calculate ways from the input bitmap
  //
  for (Index = 0; Index <= HighBitSet32 ((UINT32) Bitmap); Index++) {
    if (Bitmap & (BIT0 << Index)) {
      Ways++;
    }
  }
  return Ways;
}

/**

  Calculate number of halves covered by the MC bitmap.

  @param[in] McBitmap     - Mc bit map.

  @retval Halves          - Halves covered by McBitmap, 1..2

**/
UINT8
CalculateNumberOfHalvesFromMcBitmap (
  IN UINT8 McBitmap
  )
{
  UINT8   McIndex;
  UINT8   Halves;
  UINT8   HalfIndex;

  Halves = 0;

  HalfIndex = RtHalf0;
  while (HalfIndex < MaxRtHalves) {
    for (McIndex = 0; McIndex <= HighBitSet32 ((UINT32) McBitmap); McIndex++) {
      if ((McBitmap & (BIT0 << McIndex)) == 0) {
        continue;
      }
      if (GetHalfForMc(McIndex) != HalfIndex) {
        continue;
      }
      Halves ++;
      break;
    }
    HalfIndex ++;
  }

  return Halves;
}

/**
  Print out a formatted representation of the input Route Table data.

  @param[in] ChRt        - Channel Route Table Data
  @param[in] TgtRt       - Target Route Table Data
  @param[in] RtNumString - String to append to Route Table name
  @param[in] Cluster     - The cluster that the route table belongs to

  @retval VOID

**/
VOID
EFIAPI
PrintRouteTable (
  IN UINT32  ChRt[MaxRtHalves],  OPTIONAL
  IN UINT32  TgtRt[MaxRtHalves], OPTIONAL
  IN UINT8   *RtNumString,
  IN UINT8   Cluster
  )
{
  ROUTE_TABLE_HALVES Half;
  BOOLEAN            AdjustNewLine;

  if (RtNumString == NULL) {
    return;
  }

  //
  // Adjust the new line print to keep Ch\Tgt or H0\H1 groups together
  //
  AdjustNewLine = (ChRt == NULL || TgtRt == NULL);

  for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
    if (ChRt != NULL) {
      RcDebugPrint (SDBG_MAX, "  CSR: ");
      if (Cluster != MAX_UINT8) {
        RcDebugPrint (SDBG_MAX, "Cluster %d ", Cluster);
      }
      RcDebugPrint (SDBG_MAX,
        "H%d_CH_ROUTE_TABLE_%a   -  0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
        Half,
        RtNumString,
        BitFieldRead32 (ChRt[Half], 0, 1),
        BitFieldRead32 (ChRt[Half], 2, 3),
        BitFieldRead32 (ChRt[Half], 4, 5),
        BitFieldRead32 (ChRt[Half], 6, 7),
        BitFieldRead32 (ChRt[Half], 8, 9),
        BitFieldRead32 (ChRt[Half], 10, 11),
        BitFieldRead32 (ChRt[Half], 12, 13),
        BitFieldRead32 (ChRt[Half], 14, 15)
      );
    }
    if (TgtRt != NULL) {
      RcDebugPrint (SDBG_MAX, "  CSR: ");
      if (Cluster != MAX_UINT8) {
        RcDebugPrint (SDBG_MAX, "Cluster %d ", Cluster);
      }
      RcDebugPrint (SDBG_MAX,
        "H%d_TGT_ROUTE_TABLE_%a  -  0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
        Half,
        RtNumString,
        BitFieldRead32 (TgtRt[Half], 0, 3),
        BitFieldRead32 (TgtRt[Half], 4, 7),
        BitFieldRead32 (TgtRt[Half], 8, 11),
        BitFieldRead32 (TgtRt[Half], 12, 15),
        BitFieldRead32 (TgtRt[Half], 16, 19),
        BitFieldRead32 (TgtRt[Half], 20, 23),
        BitFieldRead32 (TgtRt[Half], 24, 27),
        BitFieldRead32 (TgtRt[Half], 28, 31)
      );
    }
    if (!AdjustNewLine) {
      RcDebugPrint (SDBG_MAX, "\n");
    }
  }

  if (AdjustNewLine) {
    RcDebugPrint (SDBG_MAX, "\n");
  }
}

/**
  Deliver Route table data to the CHAs

  @param[in]  RtType     - Route table type.
  @param[in]  Rt         - Route table data.
  @param[in]  Socket     - Socket Id.
  @param[in]  Cluster    - Cluster ID in socket, only 0 if socket is not clustered.
  @param[in]  TotCluster - Total number of clusters enabled in a socket.
  @param[in]  HalfBitmap - Map of the half route tables in use.

  @retval None
**/
VOID
EFIAPI
WriteRouteTables (
  IN ROUTE_TABLE_TYPE  RtType,
  IN UINT32            Rt[][MaxRtHalves],
  IN UINT8             Socket,
  IN UINT8             Cluster,
  IN UINT8             TotCluster,
  IN UINT8             HalfBitmap
  )
{
  ROUTE_TABLE_HALVES      RtHalf;
  UINT8                   ChaIndex;
  UINT8                   MaxChaIndexPerCluster;
  UINT8                   TotChaPerCluster;
  UINT64                  RtData;
  ROUTE_TABLE_TYPE_INFO   TypeInfo;

  TotChaPerCluster = GetTotChaCount (Socket) / TotCluster;
  MaxChaIndexPerCluster = (Cluster * TotChaPerCluster) + TotChaPerCluster;

  TypeInfo.RtType = RtType;

  for (ChaIndex = (Cluster * TotChaPerCluster); ChaIndex < MaxChaIndexPerCluster; ChaIndex++) {
    for (RtHalf = 0; RtHalf < MaxRtHalves; RtHalf++) {
      if ((HalfBitmap & (BIT0 << RtHalf)) == 0) {
        break;
      }
      TypeInfo.RtHalf = RtHalf;

      if (RouteTable3 == RtType) {  // 10nm RT2LM
        //
        // only set target for 2LM route table - no channel
        //
        RtData = Rt[ROUTE_TABLE_TARGET_ONLY][RtHalf];
      } else {
        //
        // set channel in higher 32 bits and target in lower 32 bits
        //
        RtData = LShiftU64 (Rt[RouteTableChannel][RtHalf], 32) + Rt[RouteTableTarget][RtHalf];
      }

      ChaSetRouteTable (Socket, ChaIndex, &TypeInfo, RtData);
    }
  }

  return;
}

#ifdef DDRT_SUPPORT
/** Write Route table2 data for DDRT persistent regions.

  Calculate logical to physical channel ID mapping of DDRT persistent memory.
  Target is one of the DDRT memory controllers, e.g. 0 or 1 for silicon
  with two MCs, or 0-3 for silicon with four MCs.

  @param[in]  Host          Pointer to sysHost
  @param[in]  MemMapData    Pointer to the memory map private data structure.
  @param[in]  Socket        Socket Id
  @param[in]  Cluster       Cluster ID in socket, only 0 if socket is not clustered.
  @param[in]  TotCluster    Total number of clusters enabled in a socket.

  @retval None

**/
VOID
WriteDdrtRouteTables (
  IN SYSHOST           *Host,
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             Socket,
  IN UINT8             Cluster,
  IN UINT8             TotCluster
  )
{
  UINT8               RtIndex;
  ROUTE_TABLE_HALVES  RtHalf;
  UINT32              Rt2[MaxRouteTableDestination][MaxRtHalves];
  UINT8               HalfBitmap;
  BOOLEAN             Rt0Used;
  UINT32              Rt0[MaxRouteTableDestination][MaxRtHalves];
  UINT8               SourceCluster; //index into RouteTableData

  if (MemMapData->DdrtRouteTableData.RouteTableUsed[Socket][CLUSTER_0][RtHalf0] == 0) {
    //
    // quick exit path - nothing to do at cluster 0
    //
    return;
  }
  //
  // The logic below is to support memory types in different cluster levels resulting in
  // a system cluster level that is higher than this type's cluster level.
  // If this function is called with a non-zero cluster when SNC is not enabled and DDR/DDRT
  // is not in quadrant mode, it indicates the system cluster level is higher than the DDR/DDRT level
  // In this case, we need to distribute the table information we do have across all quadrants.
  //
  if (MemMapData->DdrtRouteTableData.RouteTableUsed[Socket][Cluster][RtHalf0] == 0) {
    //
    // if SNC enabled or if DDR/DDRT is in Quadrant mode, exit when a cluster is not populated
    //
    if (IfFeatureEnabled (TypeSnc) || (MemMapData->UmaBasedClustersPerSocket == UMABASEDCLUSTERING_QUADRANT)) {
      return;
    }

    //
    // No route tables defined for this cluster and this is not cluster 0.
    // If we made it here, DDRT is in a downgraded cluster mode so calculate
    // cluster index into RouteTableData to use for this Cluster
    //
    SourceCluster = Cluster % (MemMapData->UmaBasedClustersPerSocket == 0 ? 1 : MemMapData->UmaBasedClustersPerSocket);
  } else {
    SourceCluster = Cluster; // route table data exists for cluster so one to one
  }

  //
  // Clear struct fields
  //
  ZeroMem (Rt2, sizeof (UINT32) * MaxRouteTableDestination * MaxRtHalves);
  ZeroMem (Rt0, sizeof (UINT32) * MaxRouteTableDestination * MaxRtHalves);

  HalfBitmap = 0;
  Rt0Used = FALSE;

  for (RtHalf = RtHalf0; RtHalf < MaxRtHalves; RtHalf++) {
    if (MemMapData->DdrtRouteTableData.RouteTableUsed[Socket][SourceCluster][RtHalf] == 0) {
      break;
    }
    HalfBitmap |= BIT0 << RtHalf;
    for (RtIndex = 0; RtIndex < MemMapData->DdrtRouteTableData.MaxRtWays; RtIndex++) {
      if (RtIndex < MemMapData->DdrtRouteTableData.RouteTableUsed[Socket][SourceCluster][RtHalf]) {
        if (MemMapData->MemMapSocket[Socket].UseRT0forAdx1 && (MemMapData->MemMapSocket[Socket].AppDirectByOneCount > AD_NONINTERLEAVE_CH_COUNT6) && RtHalf == RtHalf1 && RtIndex >= 2) {
          Rt0[RouteTableChannel][RtHalf0] |= (MemMapData->DdrtRouteTableData.ChRouteTable[Socket][SourceCluster][RtHalf][RtIndex] << ((RtIndex%2) * RT_CHAN_WIDTH));
          Rt0[RouteTableTarget][RtHalf0] |= (MemMapData->DdrtRouteTableData.TgtRouteTable[Socket][SourceCluster][RtHalf][RtIndex] << ((RtIndex%2) * RT_TARG_WIDTH));
          Rt0Used = TRUE;
        } else {
          Rt2[RouteTableChannel][RtHalf] |= (MemMapData->DdrtRouteTableData.ChRouteTable[Socket][SourceCluster][RtHalf][RtIndex] << (RtIndex * RT_CHAN_WIDTH));
          Rt2[RouteTableTarget][RtHalf] |= (MemMapData->DdrtRouteTableData.TgtRouteTable[Socket][SourceCluster][RtHalf][RtIndex] << (RtIndex * RT_TARG_WIDTH));
        }
      }
    } // RtIndex
  } // RtHalf

  PrintRouteTable (Rt2[RouteTableChannel], Rt2[RouteTableTarget], "2", ((TotCluster > 1) ? Cluster : MAX_UINT8));

  //
  // Write DDRT/LBW route table CSRs
  //
  WriteRouteTables (RouteTable2, Rt2, Socket, Cluster, TotCluster, HalfBitmap);

  if (Rt0Used) {
    PrintRouteTable (Rt0[RouteTableChannel], Rt0[RouteTableTarget], "0", MAX_UINT8);
    WriteRouteTables (RouteTable0, Rt0, Socket, Cluster, TotCluster, HalfBitmap);
  }

  return;
}
#endif // DDRT_SUPPORT

/** Write Route table data for DDR 1LM or 2LM near memory.

  Calculate logical to physical channel ID mapping of DDR 1LM/2LM memory.
  Target is one of the DDR memory controllers, e.g. 0 or 1 for silicon
  with two MCs, or 0-3 for silicon with four MCs.

  This function will automatically place the 2LM near memory target in RT2LM instead
  of RT1 when executed on 10nm wave/wave2 silicon.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - Pointer to the memory map private data structure.
  @param[in]  Socket     - Socket Id
  @param[in]  Cluster    - Cluster ID in socket, only 0 if socket is not clustered.
  @param[in]  TotCluster - Total number of clusters enabled in a socket.

  @retval None

**/
VOID
WriteDdrRouteTables (
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             Socket,
  IN UINT8             Cluster,
  IN UINT8             TotCluster
  )
{
  UINT8                   McIndex;
  UINT8                   RtIndex;
  ROUTE_TABLE_HALVES      RtHalf;
  BOOLEAN                 Rt2lmUsed;
  UINT32                  Rt1[MaxRouteTableDestination][MaxRtHalves];
  UINT32                  Rt2lmTgt[MaxRtHalves];
  UINT8                   HalfBitmap;
  UINT8                   SourceCluster;

  if (MemMapData->DdrRouteTableData.RouteTableUsed[Socket][CLUSTER_0][RtHalf0] == 0) {
    //
    // quick exit path - nothing to do at cluster 0
    //
    return;
  }

  //
  // The logic below is to support memory types in different cluster levels resulting in
  // a system cluster level that is higher than this type's cluster level.
  // If this function is called with a non-zero cluster when SNC is not enabled and DDR/DDRT
  // is not in quadrant mode, it indicates the system cluster level is higher than the DDR/DDRT level
  // In this case, we need to distribute the table information we do have across all quadrants.
  //
  if (MemMapData->DdrRouteTableData.RouteTableUsed[Socket][Cluster][RtHalf0] == 0) {
    if (IfFeatureEnabled (TypeSnc) || (MemMapData->UmaBasedClustersPerSocket == UMABASEDCLUSTERING_QUADRANT)) {
      //
      // if SNC or if DDR/DDRT is in Quadrant mode, exit when a cluster is not populated
      //
      return;
    }
    //
    // No route tables defined for this cluster and this is not cluster 0.
    // If we made it here, DDR is in a downgraded cluster mode so calculate
    // cluster index into RouteTableData to use for this Cluster
    //
    SourceCluster = Cluster % (MemMapData->UmaBasedClustersPerSocket == 0 ? 1 : MemMapData->UmaBasedClustersPerSocket);
  } else {
    SourceCluster = Cluster; // route table data exists for cluster so one to one
  }

  ZeroMem (Rt1, sizeof (UINT32) * MaxRouteTableDestination * MaxRtHalves);
  ZeroMem (Rt2lmTgt,  sizeof (UINT32) * MaxRtHalves);

  Rt2lmUsed = FALSE;
  HalfBitmap = 0;

  //
  // For DDR 1LM, 2LM or 1LM + 2LM volatile memory modes
  //
  for (RtHalf = RtHalf0; RtHalf < MaxRtHalves; RtHalf++) {
    if (MemMapData->DdrRouteTableData.RouteTableUsed[Socket][SourceCluster][RtHalf] == 0) {
      break;
    }
    HalfBitmap |= BIT0 << RtHalf;

    for (RtIndex = 0; RtIndex < MemMapData->DdrRouteTableData.MaxRtWays; RtIndex++) {
      if (RtIndex < MemMapData->DdrRouteTableData.RouteTableUsed[Socket][SourceCluster][RtHalf]) {
        Rt1[RouteTableChannel][RtHalf] |= (MemMapData->DdrRouteTableData.ChRouteTable[Socket][SourceCluster][RtHalf][RtIndex] << (RtIndex * RT_CHAN_WIDTH));
        McIndex = MemMapData->DdrRouteTableData.TgtRouteTable[Socket][SourceCluster][RtHalf][RtIndex];

        //
        // Target MC goes in RT2LM when 2LM memory modes are present
        //
        if (IsMemTypePresentMcFunction (Socket, McIndex, IsMemType2lm) && MemMapData->Is2LmUsingRt2lm) {
          Rt2lmTgt[RtHalf] |= (McIndex << (RtIndex * RT_TARG_WIDTH));
          Rt2lmUsed = TRUE;
        } else {
          Rt1[RouteTableTarget][RtHalf] |= (McIndex << (RtIndex * RT_TARG_WIDTH));
        }
      }
    } // RtIndex
  } // RtHalf

  PrintRouteTable (Rt1[RouteTableChannel], Rt1[RouteTableTarget], "1", Cluster);
  //
  // Write volatile memory route tables CSR
  //
  WriteRouteTables (RouteTable1, Rt1, Socket, Cluster, TotCluster, HalfBitmap);

  if (Rt2lmUsed) {
    PrintRouteTable (NULL, Rt2lmTgt, "2LM", Cluster);
    WriteRouteTables (RouteTable3, &Rt2lmTgt, Socket, Cluster, TotCluster, HalfBitmap); // 10nm RT2LM
  }

  return;
}

/** Fill in route table CRSs for 10nm memory map structure.

  Get route tables for HBM, DDR and DDRT memory and write their CSRs.

  @param[in] Host          - Pointer to sysHost
  @param[in] MemMapData    - Pointer to the memory map private data structure.
  @param[in] Sckt          - Socket Id
**/
VOID
WriteRouteTableCsrs (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            Sckt
  )
{
  UINT8                     Cluster;
  UINT8                     TotCluster;

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\t\t Write Route Table CSRs for Socket: %d\n", Sckt);
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");


  //
  // 10nm wave1/2 SNC : 1LM/2LM, No AD
  // 10nm wave1/2 UBC : 1LM/2LM, No AD
  // 10nm wave3 SNC   : 1LM/2LM, No AD
  // 10nm wave3 UBC   : 1LM/2LM, AD
  //

  TotCluster = RtGetTotalClusterVolMem (MemMapData, Sckt);

  for (Cluster = 0; Cluster < TotCluster; Cluster++) {
    //
    // Populate and write any HBM route tables for channel and target IDs
    //
    WriteHbmRouteTables (
      MemMapData,
      Sckt,
      Cluster,
      TotCluster
      );

    //
    // Populate and write any DDR/MBW route tables for channel and target IDs
    //
    WriteDdrRouteTables (
      MemMapData,
      Sckt,
      Cluster,
      TotCluster
      );
  }

#ifdef DDRT_SUPPORT
  TotCluster = RtGetTotalClusterPMem (MemMapData, Sckt);

  for (Cluster = 0; Cluster < TotCluster; Cluster++) {
    //
    // Populate and write any DDRT/LBW route tables for channel IDs
    //
    WriteDdrtRouteTables (
      Host,
      MemMapData,
      Sckt,
      Cluster,
      TotCluster
      );
  }
#endif // DDRT_SUPPORT

  return;
}

/**
  Encode route table mode.

  Each DRAM rule has a mode which determines how the route table is indexed.
  It is determined by the number of ways of the route table are used.

  @param[in]  RtWays  The number of ways of the route table being used.

  @return             The encoded RT mode value.
**/
UINT8
EncodeRtMode (
  IN UINT8  RtWays
  )
{
  UINT8 RtMode;

  switch (RtWays) {
  default:
  case 1:
    RtMode = 0;   // single target
    break;

  case 2:
    RtMode = 1;   // 2 way
    break;

  case 3:
    RtMode = 2;   // modulo 3
    break;

  case 4:
    RtMode = 3;   // 4 way
    break;

  case 8:
    RtMode = 6;   // 8 way
    break;
  }

  return RtMode;
} // EncodeRtMode

/** Get the Route Table which holds the Target and Channel IDs for the given MemType

  If either output pointers is NULL, then no action is performed.

  @param[in] MemMapData - Pointer to the memory map data structure.
  @param[in]  MemType   - The type of memory
  @param[out] TargetRt  - Pointer to the variable where the Route Table number holding the
                          target IDs for this MemType will be returned
  @param[out] ChannelRt - Pointer to the variable where the Route Table number holding the
                          channel IDs for this MemType will be returned
**/
VOID
GetRouteTableForMemType (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  MEM_TYPE         MemType,
  OUT UINT8           *TargetRt,
  OUT UINT8           *ChannelRt
  )
{
  SYSHOST *Host;

  Host = GetSysHostPointer ();
  if (TargetRt != NULL && ChannelRt != NULL) {
    if (IsMemTypeHbm (MemType)) {
      *TargetRt = RT0;
      *ChannelRt = RT0;
    } else {
      switch (MemType) {
      case MemType1lmDdr:
        *TargetRt = RT1;
        *ChannelRt = RT1;
        break;
#ifdef DDRT_SUPPORT
      case MemType2lmDdrCacheMemoryMode:
      case MemType2lmDdrWbCacheAppDirect:
        if (!MemMapData->Is2LmUsingRt2lm) {
          *TargetRt = RT1;
          *ChannelRt = RT1;
        } else {
          *TargetRt = RT2LM;
          *ChannelRt = RT1;
        }
        break;

      case MemType1lmAppDirect:
        *TargetRt = RT2;
        *ChannelRt = RT2;
        break;
#endif // DDRT_SUPPORT

      default:
        break;
      }
    }
  }
}

/** Program DRAM decode CSRs for memory map from config info in Host structure.

  Read SAD structures filled-in sysHost and program 10nm CHA decoder CSR's.

  @param[in] Host          - Pointer to sysHost
  @param[in] MemMapData    - MemMapData  Pointer to the memory map private data structure.
  @param[in] Sckt          - Socket Id

  @retval None

**/
VOID
WriteSadCsrs (
  IN SYSHOST           *Host,
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             Sckt
  )
{
  UINT8                                    NumberOfChaDramClusters;
  UINT8                                    SadIndex;
  UINT8                                    ClusterId;
  UINT8                                    BeginSadIndex;
  UINT8                                    ClusterStep;
  UINT8                                    DramRegIndex;
  ROUTE_TABLE_HALVES                       Half;
  UINT8                                    ChWays;
  UINT8                                    McWays;
#ifdef DDRT_SUPPORT
  UINT8                                    RtMode;
  UINT8                                    FmRtMode;
#endif // DDRT_SUPPORT
  UINT8                                    TargetRt = 0;
  UINT8                                    ChannelRt = 0;
  UINT8                                    MaxImc;
  MEM_VAR_STRUCT                           *Mem;
  SAD_TABLE                                *SadEntry;
  GLOBAL_INTERLEAVE_DATA                   GlobalInterleaveData;
  SAD_DATA                                 SadData;
  MODE_TYPE                                ModeType;
  MODE_TYPE_INFO                           TypeInfo;
  UINT8                                    RtModes[MaxModeType][MaxRtHalves][MAX_SAD_RULES];
  UINT8                                    RtTypes[MaxRouteType][MAX_SAD_RULES];
  UINT8                                    ChannelBitmap[MAX_IMC];
  UINT8                                    McBitMap;
  BOOLEAN                                  Found;
  UINT8                                    HalfBitmap;
#ifdef DDRT_SUPPORT
  UINT8                                    RouteTablesUsed;
#endif // DDRT_SUPPORT
  MEMORY_MAP_HOST                          *MemMapHost;
  ROUTE_TABLE_DATA                         *RouteTableData;

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\t\t Write CHA SAD CSRs for Socket: %d\n", Sckt);
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

  Mem = &Host->var.mem;
  MaxImc = GetMaxImc ();
  MemMapHost = GetMemMapHostPointer ();
  McWays = 0;
  ChWays = 0;
  MemMapData->MemMapSocket[Sckt].AppDirectByOneCount = 0;

  HalfBitmap = 0;
  RouteTableData = NULL;

  //
  // Get the number of CHA DRAM clusters that the CSR updates can apply to.
  //
  NumberOfChaDramClusters = GetNumOfDramClustersPerSystem ();
  if (NumberOfChaDramClusters > 1) {
    //
    // rules written will depend on current cluster
    // set up step to group of sad rules pertaining to current cluster
    //
    ClusterStep = MemMapData->MaxSadRules;
  } else {
    //
    // one set of rules shared by all clusters
    //
    ClusterStep = 0;
  }

  for (ClusterId = 0; ClusterId < NumberOfChaDramClusters; ClusterId++) {

    BeginSadIndex = ClusterId * ClusterStep;

    //
    // If no interleaving in a cluster then break
    //
    if (Mem->socket[Sckt].SAD[BeginSadIndex].Enable == 0) {
      break;
    }

    //
    // Clear struct fields
    //
    ZeroMem (&GlobalInterleaveData, sizeof (GlobalInterleaveData));
    ZeroMem (RtModes, sizeof (RtModes));
    ZeroMem (RtTypes, sizeof (RtTypes));

    //
    // Before writing multicast SNC DRAM CSR's, unlock ClusterId and lock other Clusters
    //
    ChaUnlockDramCluster (Sckt, ClusterId);

    //
    // 1. Program Global interleave config.
    //

    //
    // Rt0 is fixed for HBM in a HBM enabled SoC.
    // Always enable XOR mode for HBM.
    //
    if (IsHbmEnabledInSkt (MemMapData, Sckt)) {
      GlobalInterleaveData.Rt0InterleaveShift = INTERLEAVE_MODE_256B_XOR;
    }

    //
    // RT1 is fixed for volatile memory (1LM / 2LM / 1LM + 2LM)
    //
    if ((IsMemTypePresentSocketFunction (Sckt, IsMemType1lm)) ||
        (IsMemTypePresentSocketFunction (Sckt, IsMemType2lm))) {
      if ((MemMapHost->SocketInfo[Sckt].XorMode & CHANNEL_XOR) != 0) {
        GlobalInterleaveData.Rt1InterleaveShift = INTERLEAVE_MODE_256B_XOR; // Only DDR has 256B XOR granularity support
      } else {
        GlobalInterleaveData.Rt1InterleaveShift = INTERLEAVE_MODE_256B;
      }
    }

    //
    // RT2 is fixed for DDRT
    //
    if (MemMapData->DdrtIntlvGranularity == INTERLEAVE_GRANULARITY_4KB) {
      GlobalInterleaveData.Rt2InterleaveShift = INTERLEAVE_MODE_4KB;
    } else {
      GlobalInterleaveData.Rt2InterleaveShift = INTERLEAVE_MODE_256B;
    }

    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Program Channel Interleaves\n"
      "    rt0_interleave_mode:  %d\n    rt1_interleave_shift: %d\n    rt2_interleave_shift: %d\n\n",
      GlobalInterleaveData.Rt0InterleaveShift,
      GlobalInterleaveData.Rt1InterleaveShift,
      GlobalInterleaveData.Rt2InterleaveShift
      );

    ChaSetGlobalInterleave (Sckt, &GlobalInterleaveData);

    for (SadIndex = BeginSadIndex; SadIndex < (BeginSadIndex + MemMapData->MaxSadRules); SadIndex++) {
      SadEntry = &Mem->socket[Sckt].SAD[SadIndex];

      //
      // Break if we have reached the end of the SAD table
      //
      if (SadEntry->Enable == 0) {
        break;
      }

      //
      // Before programming CHA DRAM decoder CSR's calculate the DRAM register index w.r.t each DRAM cluster
      //
      DramRegIndex = (SadIndex % MemMapData->MaxSadRules);

      //
      // 2. Program DRAM_RULE_CFG_N
      //
      ZeroMem (&SadData, sizeof (SadData));
      //
      // Update fields based on SAD table entry
      //
      SadData.Enabled = SadEntry->Enable;
      SadData.Attribute = SadEntry->Attr;
      SadData.Limit = SadTadEncodeLimit (SadEntry->Limit);

      SadData.InterleaveMode = EncodeSadInterleaveModeGranularity (SadEntry->tgtGranularity);

      //
      // If memtype is 2LM or WBPMEM specify range is cacheable in NM
      //
      if (IsMemType2lm (SadEntry->type)) {
        SadData.Cacheable = 1;
      }

      if (SadEntry->type == MemType1lmAppDirect && SadEntry->ways == IMC_1WAY && SadEntry->FmChWays == CH_1WAY) {
        MemMapData->MemMapSocket[Sckt].AppDirectByOneCount++;
      }

      //
      // Display enabled SAD rules in the serial log
      //
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Program CSRs for SAD Rule %d ", DramRegIndex);
      if (SadEntry->type != MemTypeNxm) {
        if (SadEntry->local) {
          switch (SadEntry->type) {
          case MemType1lmDdr:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(1LM)");
            break;
          case MemType2lmDdrCacheMemoryMode:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(2LM)");
            break;
          case MemType1lmAppDirect:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(PMEM)");
            break;
          case MemType2lmDdrWbCacheAppDirect:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(WB PMEM)");
            break;
          case MemType1lmHbm:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(HBM_FLAT)");
            break;
          case MemType2lmHbmCacheDdr:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(HBM 2LM DDR)");
            break;
          case MemType2lmHbmCacheMemoryMode:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(HBM 2LM DDRT)");
            break;
          case MemTypeCxlAccVolatileMem:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(CXL ACC VOL MEM)");
            break;
          case MemTypeCxlAccPersistentMem:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(CXL ACC PER MEM)");
            break;
          case MemTypeCxlExpVolatileMem:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(CXL EXP VOL MEM)");
            break;
          case MemTypeCxlExpPersistentMem:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(CXL EXP PER MEM)");
            break;
          default:
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(<ERROR:0x%x>)", SadEntry->type);
            break;
          }
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(Remote)");
        }
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "(NXM)");
      }
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n");
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "    En  Limit    Mode    Attr  NMCacheable  \n");
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "    %d   0x%05x   0x%x    0x%x    %d \n",
        SadData.Enabled,
        SadData.Limit,
        SadData.InterleaveMode,
        SadData.Attribute,
        SadData.Cacheable
      );

      //
      // 3. Program INTERLEAVE_LIST_CFG_N
      //
      if (SadEntry->local && IsMemTypeHbm (SadEntry->type)) {
        SadData.InterleaveList = GetHbmFlatInterleaveList ();
      } else if (IsMemTypeCxl (SadEntry->type)) {
        SadData.InterleaveList = GetCxlMemInterleaveList (MemMapData, SadEntry);
      } else {
        SadData.InterleaveList = SadEncodeInterleaveList (
          Host,
          MemMapData,
          Sckt,
          SadIndex,
          SadEntry->type
        );
      }

      RcDebugPrint (SDBG_MAX, "    Interleave List: 0x%08x\n", SadData.InterleaveList);

      //
      // Check if the interleave list is a single target
      // and print out the decoded single target index.
      //
      if (!IsMemTypeCxl (SadEntry->type) &&
        SadEntry->local == 1 &&
        SadEntry->ways == IMC_1WAY &&
        GetChannelWays (SadEntry) == CH_1WAY) {
        RcDebugPrint (
          SDBG_MAX,
          "    Single Target: H%d Index %d\n",
          ((UINT32) SadData.InterleaveList & BIT0),
          ((UINT32) SadData.InterleaveList & (BIT2 | BIT1)) >> 1
        );
      }

      //
      // Set the indication of whether a DRAM rule pertains to volatile memory or durable memory.
      //
      if (IsMemTypeAppDirect (SadEntry->type) || (SadEntry->type == MemTypeCxlAccPersistentMem) ||
        (SadEntry->type == MemTypeCxlExpPersistentMem) ||
        (PcdGetBool (PcdNvDimmEn) && MemMapData->MemMapSocket[Sckt].NvMemSadIdxBitMap[ClusterId] & (1 << SadIndex))) {
        SadData.DurableMem = TRUE;
      } else {
        SadData.DurableMem = FALSE;
      }

      //
      // Set the indication of whether a DRAM rule pertains to NXM or CXL memory.
      //
      if (SadEntry->type == MemTypeNxm) {
        SadData.Nxm = TRUE;
      } else if (IsMemTypeCxl (SadEntry->type)) {
        if ((SadEntry->type == MemTypeCxlAccVolatileMem) || (SadEntry->type == MemTypeCxlAccPersistentMem)) {
          SadData.CxlAccMem = TRUE;
          SadData.CxlExpMem = FALSE;
        } else {
          SadData.CxlAccMem = FALSE;
          SadData.CxlExpMem = TRUE;
        }
      }

      ChaSetSadData (Sckt, DramRegIndex, &SadData);

      //
      // Skip route table calculation in case of NXM, CXL or FPGA memory types
      //
      if (SadEntry->type == MemTypeNxm || IsMemTypeCxl (SadEntry->type) || SadEntry->type == MemTypeFpga) {
        continue;
      }

      //
      // Program DRAM_H0/1_RT0/1_MODE0/1/2_CFG
      //
      GetRouteTableForMemType (MemMapData, SadEntry->type, &TargetRt, &ChannelRt);
      if (MemMapData->MemMapSocket[Sckt].UseRT0forAdx1 && (MemMapData->MemMapSocket[Sckt].AppDirectByOneCount > AD_NONINTERLEAVE_CH_COUNT6)) {
        TargetRt = RT0;
        ChannelRt = RT0;
      }

      if (!IsMemTypeHbm (SadEntry->type)) {
        RouteTableData = GetRouteTableDataForMemType (MemMapData, Sckt, SadEntry->type);
      }

      switch (ChannelRt) {
        case RT0:
          ModeType = ModeTypeRt0;
          break;
        case RT1:
          ModeType = ModeTypeRt1;
          break;
        case RT2:
          ModeType = ModeTypeRt2;
          break;
        default:
          ModeType = MaxModeType;
          break;
        }

      if ((MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) && IsUbcSupportedMemType (SadEntry->type)) {
        //
        // Only H0 is used when UMA-Based Clustering is enabled
        //
        HalfBitmap = (BIT0 << RtHalf0);
      } else {
        ZeroMem (ChannelBitmap, sizeof (ChannelBitmap));
        GetInterleaveWays (MemMapData, Sckt, SadIndex, &McWays, &ChWays, ChannelBitmap, &McBitMap);

        if (McWays == 0 || ChWays == 0 || McBitMap == 0 || ChannelBitmap == NULL) {
          RcDebugPrint (SDBG_ERROR, "ERROR: Invalid interleave data found for SAD %d\n", SadIndex);
          ASSERT (FALSE);
          return;
        }

        if (!IsMemTypeHbm (SadEntry->type)) {
          //
          // First call FindRouteTable to fetch the HalfBitmap. This will tell us how many route tables
          // are used by the current SAD rule (both H0 and H1 can be used in an 8-way interleave)
          //
          Found = FindRouteTable (
           MemMapData,
           Sckt,
           SadEntry->type,
           SadEntry->Cluster,
           McBitMap,
           ChannelBitmap,
           &HalfBitmap,
           NULL
          );

          //
          // There should be at least one half instance of route table for this DRAM rule
          //
          if (!Found || HalfBitmap == 0 || HighBitSet32 (HalfBitmap) > MaxRtHalves) {
            RcDebugPrint (SDBG_ERROR, "ERROR: Route Table Not Found for SAD %d\n", SadIndex);
            ASSERT (FALSE);
            return;
          }
        }
      }

      if (IsMemTypeHbm (SadEntry->type)) {
        //
        // HBM only uses H0 and supports only 8 way channel interleave
        //
        RtModes[ModeType][RtHalf0][DramRegIndex] = ROUTE_TABLE_MODE_8WAY;
      } else {
        switch (SadEntry->type) {
        case MemType1lmDdr:
          //
          // Since channel and MC granularities are the same, ChWays includes the McWays (0-7)
          // except for mod3 interleave.
          //
          if (SadEntry->NmChWays == CH_3WAY) {
            ChWays = SadEntry->NmChWays;
            for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
              if ((HalfBitmap & (BIT0 << Half)) == 0) {
                continue;
              }
              RtModes[ModeType][Half][DramRegIndex] = EncodeRtMode (ChWays);
            }
          } else {
            McWays = CalculateInterleaveWaysfromBitmap (SadEntry->imcInterBitmap);
            for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
              if ((HalfBitmap & (BIT0 << Half)) == 0) {
                continue;
              }
              if (MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) {
                RtModes[ModeType][Half][DramRegIndex] = EncodeRtMode (SadEntry->NmChWays * (McWays / MemMapData->UmaBasedClustersPerSocket));
              } else {
                RtModes[ModeType][Half][DramRegIndex] = EncodeRtMode (SadEntry->NmChWays * McWays);
              }
            }
          }
          break;

#ifdef DDRT_SUPPORT
        case MemType2lmDdrCacheMemoryMode:
        case MemType2lmDdrWbCacheAppDirect:
          if (!MemMapData->Is2LmUsingRt2lm) {
            //
            // Since channel and MC granularities are the same, ChWays includes the McWays (0-7)
            // except for mod3 interleave.
            //
            GetNearMemoryWays (SadEntry->NmImcInterBitmap, SadEntry->channelInterBitmap, &McWays, &ChWays);
            if (ChWays == CH_3WAY) {
              for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
                if ((HalfBitmap & (BIT0 << Half)) == 0) {
                  continue;
                }
                RtModes[ModeType][Half][DramRegIndex] = EncodeRtMode (ChWays);
              }
            } else {
              for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
                if ((HalfBitmap & (BIT0 << Half)) == 0) {
                  continue;
                }
                if (MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) {
                  RtModes[ModeType][Half][DramRegIndex] = EncodeRtMode (ChWays * (McWays / MemMapData->UmaBasedClustersPerSocket));
                } else {
                  RtModes[ModeType][Half][DramRegIndex] = EncodeRtMode (ChWays * McWays);
                }
              }
            }
          } else {
            //
            // Since channel and MC granularities are different, RTx mode/idx ChWays can be upto max channel ways.
            //
            RtMode = EncodeRtMode (SadEntry->NmChWays);
            for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
              if ((HalfBitmap & (BIT0 << Half)) == 0) {
                continue;
              }
              //
              // For 2LM modes program RT2 indexing mode bits for target, RT2 mode/idx can be upto 4 ways per half
              //
              if (MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) {
                if (RouteTableData == NULL) {
                  RcDebugPrint (SDBG_ERROR, "ERROR: RouteTableData pointer was NULL\n");
                  ASSERT (FALSE);
                  return;
                }

                FmRtMode = EncodeRtMode (RouteTableData->RouteTableUsed[Sckt][SadEntry->Cluster][Half]);
              } else {
                FmRtMode = EncodeRtMode (CalculateInterleaveWaysfromBitmap (SadEntry->imcInterBitmap));
              }
              RtModes[ModeType][Half][DramRegIndex] = RtMode;
              RtModes[ModeTypeRt2][Half][DramRegIndex] = FmRtMode;
            }
          }
          break;

        case MemType1lmAppDirect:
          //
          // The route table ways will always be symmetric when more than one route table is used. Calculate
          // the individual route table ways by dividing the total channel ways by number of route tables used.
          //
          RouteTablesUsed = 0;
          for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
            if ((HalfBitmap & (BIT0 << Half)) == 0) {
              continue;
            }
            RouteTablesUsed++;
          }

          if (RouteTablesUsed == 0) {
            RcDebugPrint (SDBG_ERROR, "ERROR: No route tables for 1lm app direct\n");
            ASSERT (FALSE);
            return;
          }

          McWays = CalculateInterleaveWaysfromBitmap (SadEntry->imcInterBitmap);

          RtMode = EncodeRtMode ((McWays * SadEntry->FmChWays) / RouteTablesUsed);

          //
          // Finally, set the RT MODE for the route tables that are used.
          //
          for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
            if ((HalfBitmap & (BIT0 << Half)) == 0) {
              continue;
            }
            if (RouteTableData == NULL) {
              RcDebugPrint (SDBG_ERROR, "ERROR: RouteTableData pointer was NULL\n");
              ASSERT (FALSE);
              return;
            }

            if ((McWays * SadEntry->FmChWays) >= RouteTableData->RouteTableUsed[Sckt][SadEntry->Cluster][Half]) {
              RtModes[ModeType][Half][DramRegIndex] = EncodeRtMode (RouteTableData->RouteTableUsed[Sckt][SadEntry->Cluster][Half]);
            } else {
              RtModes[ModeType][Half][DramRegIndex] = RtMode;
            }
          }
          break;
#endif // DDRT_SUPPORT

        default:
          break;
        }
      }

      //
      // Calculate DRAM_MC_TARGET_CFG and DRAM_MC_CHANNEL_CFG.
      //
      RtTypes[RouteTypeMcTarget][DramRegIndex] = TargetRt;
      RtTypes[RouteTypeMcChannel][DramRegIndex] = ChannelRt;
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "    TgtRt: %02d  ChnRt: %02d\n\n", TargetRt, ChannelRt);

    } // SadIndex

    //
    // Write CSRs DRAM_MC_TARGET_CFG and DRAM_MC_CHANNEL_CFG
    //
    ChaSetRtType (Sckt, RouteTypeMcTarget, RtTypes[RouteTypeMcTarget]);

    ChaSetRtType (Sckt, RouteTypeMcChannel, RtTypes[RouteTypeMcChannel]);

    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Program Mode config.\n"
    );
    //
    // Write CSRs for DRAM_H0/H1_RT1_MODE0/1_CFG
    //
    for (ModeType = ModeTypeRt0; ModeType < MaxModeType; ModeType++) {
      if (ModeType == ModeTypeRt3) {
        continue; // 10nm does not have RT mode 3
      }
      for (Half = 0; Half < MaxRtHalves; Half++) {
        TypeInfo.RtType = ModeType;
        TypeInfo.RtHalf = Half;
        ChaSetRtMode (Sckt, &TypeInfo, RtModes[ModeType][Half]);
      }
    }

    //
    // Write FZM related CSRs to enumerate memory regions that are fast-zero capable.
    //
    WriteFzmCsrs (Host, MemMapData, Sckt, BeginSadIndex);
  } // ClusterId

  //
  // Write CSR for DRAM_ROUTETHROUGH_RANGE_CFG
  // Need to revisit when HBM is enabled. Unused for silicon with external MCs
  //
  // ChaSetRtType (Sckt, RoutetypeRouteThroughRange, RtTypes[RoutetypeRouteThroughRange]);

  //
  // Ensure DRAM cluster(s) lock after write and before exit.
  //
  ChaLockDramClusters (Sckt);

  return;
}

/**

  Worker function to get the sad limit value.

  @param[in] Host          - Pointer to sysHost
  @param[in] MemmapData    - Pointer to memory map private data structure

  @return SadLimit value

**/
UINT32
GetSadLimit (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8         SocketIndex;
  UINT32        SadLimit;
  UINT32        CurrentSadLimit;
  UINT8         MaxSadRules;
  UINT8         ClusterId;

  SadLimit = 0;
  CurrentSadLimit = 0;

  //
  // If system is NUMA aware, then remote DRAM rules need to be used for limit calculation.
  //
  if (MemMapData->IsNumaEn) {
    for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
      if ((CheckSocketEnabled (SocketIndex) == 0 ||
          (GetSocketMemSize (SocketIndex) == 0 && GetHbmStackCount (MemMapData, SocketIndex) == 0))) {
        continue;
      }
      for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
        MaxSadRules = FindSadIndex (Host, MemMapData, SocketIndex, ClusterId);
        CurrentSadLimit = GetSocketSadLimit (Host, MemMapData, SocketIndex, MaxSadRules, ClusterId);
        if (CurrentSadLimit > SadLimit) {
          SadLimit = CurrentSadLimit;
        }
      }
    }
  } else {
    //
    // Compute SadLimit and TOLM such that  "addr range" <= {SAD,TOLM}Limit (64MB granularity)
    //
    MaxSadRules = FindSadIndex (Host, MemMapData, 0, 0);
    if ((MaxSadRules > 0) && (MaxSadRules < MemMapData->MaxSadRules)) {
      SadLimit = Host->var.mem.socket[0].SAD[MaxSadRules - 1].Limit;
    }
  }

  return SadLimit;
}

#ifdef DDRT_SUPPORT
/** Program block decoder CSRs for memory map from config info in Host structure.

  Read SAD structures filled-in sysHost and program 10nm CHA decoder CSR's.

  @param[in] Socket        - Socket Id

  @retval None

**/
VOID
WriteBlockDecoderCsrs (
  IN UINT8            Socket
  )
{
  MEMORY_MAP_HOST                          *MemMapHost;
  MEMORY_MAP_BLOCK_DECODER_DATA            *BlockDecoder;
  UINT32                                   BlockDecoderLimit;
  UINT8                                    Index;

  MemMapHost = GetMemMapHostPointer ();
  BlockDecoder = &MemMapHost->BlockDecoderData;
  if (BlockDecoder->BlockSocketEnable == 0) {
    return;
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\t\t Write Block Decoder CSRs for Socket: %d\n", Socket);
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

  BlockDecoderLimit = SadTadEncodeLimit (BlockDecoder->BlockDecoderLimit);

  //
  // Write CHA CSRs for block decoders
  //
  ChaSetBlockDecoderBase (Socket, BlockDecoder->BlockDecoderBase, BlockDecoderLimit);

  for (Index = 0; Index < MAX_SI_SOCKET; Index++) {
    ChaSetBlockDecoderEnableCfg (Socket, Index, BlockDecoder->BlockMcChEn[Index]);
  }
}

/**
  Enables block decoder if DDRT DIMM is installed.

  @param[in] Host             Pointer to the host structure.
  @param[in] MemMapData       Pointer to the memory map private data structure.

  @retval SUCCESS             BlockDecoder is successfully enabled.

**/
UINT32
EnableBlockDecoder (
  IN  SYSHOST              *Host,
  IN  MEMORY_MAP_DATA      *MemMapData
  )
{
  UINT8                         Socket;
  UINT8                         Channel;
  UINT8                         Dimm;
  UINT8                         McNum;
  UINT8                         ChNum;
  UINT8                         SocketEnable;
  MEMORY_MAP_BLOCK_DECODER_DATA *BlockDecoder;
  MEMORY_MAP_HOST               *MemMapHost;
  UINT8                         MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  //
  // Determine whether we need to enable block decoder and calculate the enable bit mask
  //
  MemMapHost = GetMemMapHostPointer ();
  BlockDecoder = &MemMapHost->BlockDecoderData;
  SocketEnable = 0;
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }
    for (Channel = 0; Channel < MaxChDdr; Channel++) {

      if (!Host->nvram.mem.socket[Socket].channelList[Channel].enabled) {
        continue;  // Skip disabled channel
      }
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

        if (IsNvDimmEnabled (Host, Socket, Channel, Dimm)) {
          ChNum = MEM_SKTCH_TO_IMCCH (Channel);
          McNum = MEM_SKTCH_TO_IMC (Channel);
          BlockDecoder->BlockMcChEn[Socket][McNum][ChNum] = 1;
          SocketEnable |= 1 << Socket;
        }
      }
    }
  }
  if (SocketEnable == 0) {
    return SUCCESS;
  }

  //
  // Determine whether we have room to map the block decoder below MMIOH
  //
  if (GetSadLimit (Host, MemMapData) > (Host->var.mem.mmiohBase - BLOCK_DECODER_SIZE_64MB)) {
    return FAILURE;
  }

  BlockDecoder->BlockSocketEnable = SocketEnable;
  BlockDecoder->BlockDecoderBase = Host->var.mem.mmiohBase - BLOCK_DECODER_SIZE_64MB;
  BlockDecoder->BlockDecoderLimit = Host->var.mem.mmiohBase;

  return SUCCESS;
}
#endif // DDRT_SUPPORT

/**
  Find the next TAD table entry that needs to be filled for this Socket/IMC.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  SocketNum  - Socket number
  @param[in]  ImcNum     - Memory controller number

  @retval Index of next available TAD table entry (0xFF if table is full)

**/
UINT8
FindTadIndex (
  IN  SYSHOST *Host,
  IN  UINT8    SocketNum,
  IN  UINT8    ImcNum
  )
{
  SOCKET_INFO_STRUCT *Socket;
  IMC_INFO_STRUCT    *Imc;
  UINT8               Index;
  UINT8               TadIndex = 0xFF;

  Socket = &Host->var.mem.socket[SocketNum];
  Imc = &Socket->imc[ImcNum];

  // Only 1 set of TAD offset registers in 10nm but there are fewer total than SKX
  //
  for (Index = 0; Index < MAX_TAD_RULES_10NM; Index++) {
    if (Imc->TAD[Index].Enable == 0) {
      TadIndex = Index;
      break;
    }
  }

  return TadIndex;
}

#ifdef DDRT_SUPPORT
/**
  TAD Interleave for block decoder. It allocates TAD entry for the block decoder

  @param[in] Socket        Current socket id

**/
UINT32
TadInterleaveForBlockDecoder (
  IN UINT8           Socket
  )
{
  SYSHOST                       *Host;
  UINT8                         MaxImc;
  UINT8                         McIndex;
  UINT8                         McChIndex;
  UINT8                         MaxChPerMc;
  UINT8                         ChIndex;
  UINT8                         TadIndex;
  UINT32                        DpaOffset;
  UINT16                        InterSizePerCh;
  UINT32                        ChannelMemUsed;
  UINT8                         ChannelWays;
  MEMORY_MAP_BLOCK_DECODER_DATA *BlockDecoder;
  TAD_TABLE                     *TadEntry;
  IMC_INFO_STRUCT               *Imc;
  DDR_CHANNEL_STRUCT            *Channel;
  BOOLEAN                       BlockDecoderEn;
  MEMORY_MAP_HOST               *MemMapHost;

  Host = GetSysHostPointer ();
  MaxImc = GetMaxImc ();
  MemMapHost = GetMemMapHostPointer ();
  MaxChPerMc = GetNumChannelPerMc ();
  BlockDecoder = &MemMapHost->BlockDecoderData;

  DpaOffset = DDRT_CTRL_DPA_START;
  InterSizePerCh = 2; // 128MB for CSR/MailBox/Ctrl

  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    BlockDecoderEn = FALSE;
    for (McChIndex = 0; McChIndex < MaxChPerMc; McChIndex++) {
      if ((BlockDecoder->BlockMcChEn[Socket][McIndex][McChIndex]) != 0) {
        // Memory controller not participating in Block Decoder per local enable bitmask
        BlockDecoderEn = TRUE;
      }
    }
    if (!BlockDecoderEn) {
      continue;
    }

    //Get the index of next TAD to be filled.
    TadIndex = FindTadIndex (Host, Socket, McIndex);
    // Issue warning if all TAD rules used before all memory allocated
    if (TadIndex == 0xFF) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "TAD rules exeeded\n");
      OutputWarning (WARN_INTERLEAVE_FAILURE, WARN_TAD_RULES_EXCEEDED, Socket, 0xFF, 0xFF, 0xFF);
      return ERROR_RESOURCE_CALCULATION_FAILURE;
    }
    //Create a TAD entry
    TadEntry = &Host->var.mem.socket[Socket].imc[McIndex].TAD[TadIndex];
    TadEntry->Enable = 1;
    TadEntry->SADId = INVALID_SAD;
    TadEntry->Limit = BlockDecoder->BlockDecoderLimit;
    //
    // We need to make sure the channel interleave way is 128 for block decoder.
    // So we program socket way as 16 and channel ways as 8 below to achieve this.
    //
    TadEntry->socketWays = 16;

    Imc = &Host->var.mem.socket[Socket].imc[McIndex];
    for (McChIndex = 0; McChIndex < MaxChPerMc; McChIndex++) {
      if (BlockDecoder->BlockMcChEn[Socket][McIndex][McChIndex] == 0) {
        // Channel not participating in Block Decoder per local enable bitmask
        continue;
      }
      ChIndex = McChIndex + (McIndex * MaxChPerMc);
      Channel = &Host->var.mem.socket[Socket].channelList[ChIndex];
      ChannelMemUsed = (Channel->ctrlSize - Channel->ctrlRemSize) + InterSizePerCh;
      //
      // If the system is running in 1LM + 2LM/PMEM
      // Find if there is another DDR DIMM in this channel that has already been mapped.
      // If so, find the size of the memory that is already mapped from this channel and add it to the offset
      // This will ensure that the channel address decoded using the Tad Offset will be correct.
      //
      ChannelMemUsed = ComputeChannelOffset (Host, ChannelMemUsed, Socket, McChIndex + (McIndex * MaxChPerMc));

      // Update remaining size on channel
      Channel->ctrlRemSize -= InterSizePerCh;

      //Update the TadInterleave list
      Imc->TADintList[TadIndex][McChIndex] = 1;

      //Update the TadOffset value
      ChannelWays = 8; // program channel ways as 8 so that block decoder can reach 128 way channel interleave.
      Imc->TADOffset[TadIndex][McChIndex] = CalculateTadOffset (
                                              FALSE,
                                              Imc->TAD[TadIndex].socketWays,
                                              ChannelWays,
                                              ChannelMemUsed,
                                              Imc->TAD[TadIndex].Limit
                                              );
      Imc->TADChnIndex[TadIndex][McChIndex] = ((BlockDecoder->BlockDecoderBase / Imc->TAD[TadIndex].socketWays) % ChannelWays);
    }
  }

  return SUCCESS;
}
#endif // DDRT_SUPPORT

/** Create remote DRAM rules to target remote memory when that memory is NUMA interleaved

  Add a remote SAD entry in all sockets that are not involved in the SAD interleave.

  @param[in, out] Host       - Pointer to sysHost.
  @param[in, out] MemMapData - Pointer to memory map private data structure.
  @param[in]      SocketNum  - Socket ID.
  @param[in]      MemType    - Type of memory.
  @param[in]      ClusterId  - Cluster Id for the multicast access

  @retval SUCCESS      -  Remote DRAM rule added.
  @retval FAILURE      -  Remote DRAM rules exceed maximum count.

**/
UINT32
AddRemoteSadEntry (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           SocketNum,
  IN  MEM_TYPE        MemType,
  IN  UINT8           ClusterId
  )
{

  UINT8              SadIndex;
  UINT8              Index;
  UINT32             Limit;
  UINT32             Base;
  UINT8              Attr;
  UINT8              Sckt;
  UINT8              RemoteSadIndex;
  UINT8              DramCluster;
  RSAD_TABLE         *RemoteSadEntry;

  //
  // Get index of next available SAD entry.
  //
  SadIndex = FindSadIndex (
               Host,
               MemMapData,
               SocketNum,
               ClusterId
               );

  //
  // If SadIndex 0 is available, then there is no memory to map.
  //
  if (SadIndex == 0) {
    return SUCCESS;
  }

  //
  // Make sure we don't go out of bounds of SAD array
  //
  if (SadIndex >= MAX_SAD_INDEX) {
    return FAILURE;
  }

  //
  // Get SAD type and limit
  //
  Attr = Host->var.mem.socket[SocketNum].SAD[SadIndex - 1].Attr;
  Limit = Host->var.mem.socket[SocketNum].SAD[SadIndex - 1].Limit;

  if (MemMapData->NumberofChaDramClusters > 1) {
    DramCluster = ClusterId;
  } else {
    DramCluster = 0;
  }
  //
  // Traverse SAD entries from the beginning of this socket & cluster ID sad table to get to the first SAD for
  // the memory type that should be mapped remote.
  //
  for (Index = DramCluster * MemMapData->MaxSadRules; Index < SadIndex; Index++) {
    if (Host->var.mem.socket[SocketNum].SAD[Index].type == MemType) {
      break;
    }
  }

  //
  // If we reached next available Sad Index and no matching memory of this type found in SAD table then return
  // We have no remote memory of this type to map on other sockets.
  //
  if (Index == SadIndex) {
    return SUCCESS;
  }
  //
  // Cache to local variable to avoid recomputation in the same function.
  //
  Base = Host->var.mem.socket[SocketNum].SAD[Index].Base;

  //
  // Fill remote SAD struct in all the other sockets
  //
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    //
    // If local socket or remote socket disabled, then skip socket
    //
    if (Sckt == SocketNum ||
        (CheckSocketEnabled (Sckt) == 0)) {
      continue;
    }
    //
    // Get index of next available remote SAD entry.
    //
    RemoteSadIndex = FindRemoteSadIndex (
                       MemMapData,
                       Sckt
                       );
    if (RemoteSadIndex > 0) {
      //
      // Check whether we can merge the remote SAD rule to avoid creating new one
      //
      RemoteSadEntry = &MemMapData->MemMapSocket[Sckt].RSad[RemoteSadIndex - 1];
      if ((RemoteSadEntry->NodeId == SocketNum) && (RemoteSadEntry->Limit >= Base) && (RemoteSadEntry->Attr == Attr)) {
        //
        // Merge remote dram rule according to their latency to save remote dram rule.
        //
        MemMapData->MemMapSocket[Sckt].RSad[RemoteSadIndex - 1].Limit = Limit;
        continue;
      }
    }
    if (RemoteSadIndex >= MemMapData->MaxRemoteSadRules) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "Remote SAD rules exceed %d.\n", MemMapData->MaxRemoteSadRules);
      OutputWarning (
        WARN_INTERLEAVE_FAILURE,
        WARN_SAD_RULES_EXCEEDED,
        SocketNum,
        0xFF,
        0xFF,
        0xFF
        );
      return ERROR_RESOURCE_CALCULATION_FAILURE;
    } else {
      RemoteSadEntry = &MemMapData->MemMapSocket[Sckt].RSad[RemoteSadIndex];
      RemoteSadEntry->Enable = 1;
      RemoteSadEntry->Attr = Attr;
      RemoteSadEntry->Base = Base;
      RemoteSadEntry->NodeId = SocketNum;
      RemoteSadEntry->Limit = Limit;
    }
  }

  return SUCCESS;
}

/** Get the SAD address base of the requested SAD table entry

  Use the local and remote 10nm SAD tables to calculate the requested SAD entry base address.

  @param[in] Host          - Pointer to sysHost.
  @param[in] MemMapData    - Pointer to memory map private data structure.
  @param[in] SocketNum     - Socket id
  @param[in] CurrSadIndex  - Index of SAD table entry within socket

  @retval Base address of the requested SAD table entry

**/
UINT32
GetSadBase (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           SocketNum,
  IN  UINT8           CurrSadIndex
  )
{
  SOCKET_INFO_STRUCT *Socket;
  UINT32             SadBase;
  UINT8              RemoteSadIndex;
  UINT8              BeginSadIndex;
  UINT8              MaxSadIndex;
  UINT8              DramClusterId;
  UINT8              SadIndex;

  Socket = &Host->var.mem.socket[SocketNum];
  SadBase = 0;

  //
  // Scan remote SAD rules for possible SAD base.  This will be the highest RSAD limit
  //
  for (RemoteSadIndex = 0; RemoteSadIndex < MemMapData->MaxRemoteSadRules; RemoteSadIndex++) {
    if (MemMapData->MemMapSocket[SocketNum].RSad[RemoteSadIndex].Enable == 0) {
      break;
    }
    if (SadBase < MemMapData->MemMapSocket[SocketNum].RSad[RemoteSadIndex].Limit) {
      SadBase = MemMapData->MemMapSocket[SocketNum].RSad[RemoteSadIndex].Limit;
    }
  }

  //
  // Sad index 0 shall include last enabled remote SAD limit in its base
  //
  if (CurrSadIndex == 0) {
    return SadBase;
  }

  //
  // SAD base is the limit of last populated SAD in same socket
  //
  for (DramClusterId = 0; DramClusterId < MemMapData->NumberofChaDramClusters; DramClusterId++) {
    BeginSadIndex = (DramClusterId * MemMapData->MaxSadRules);
    MaxSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
    if (Socket->SAD[BeginSadIndex].Enable == 0) {
      break;
    }
    for (SadIndex = BeginSadIndex; SadIndex < MaxSadIndex; SadIndex++) {
      if ((Socket->SAD[SadIndex].Enable == 0) || (SadIndex == CurrSadIndex)) {
        break;
      }
      if (SadBase < Socket->SAD[SadIndex].Limit) {
        SadBase = Socket->SAD[SadIndex].Limit;
      }
    }
  }

  //
  // Use prior SAD limit as SAD base if higher than RSAD limit or clustered SAD limit
  //
  if ((CurrSadIndex > 0) && (SadBase < Socket->SAD[CurrSadIndex - 1].Limit)) {
    SadBase = Socket->SAD[CurrSadIndex - 1].Limit;
  }

  return SadBase;
}

/**

  Get total channel count from an interleave within the target socket.

  This routine assumes the McBitmap, ChannelBitmap are validated
  to represent a valid interleave.

  @param[in]  McBitmap         - Mc bit map of the target socket
  @param[in]  ChannelBitmap    - Channel bit map of the target socket

  @retval                      - Total channel count for the interleave

**/
UINT8
GetTotalChannelCountFromAnInterleaveWithinSocket (
  IN  UINT8               McBitmap,
  IN  UINT8               ChannelBitmap[MAX_IMC]
  )
{
  UINT8     ImcIndex;
  UINT8     MaxImc;
  UINT8     ChannelCount = 0;

  MaxImc = GetMaxImc();

  for (ImcIndex = 0; ImcIndex < MaxImc; ImcIndex++ ) {
    if ((McBitmap & (1 << ImcIndex)) == 0) {
      continue;
    }
    ChannelCount += CalculateInterleaveWaysfromBitmap (ChannelBitmap[ImcIndex]);
  }

  return ChannelCount;
}

/**

  Get total channel count from an interleave.

  This routine assumes the SocketBitmap, McBitmap, ChannelBitmap are validated
  to represent a valid interleave.

  @param[in]  SocketBitmap     - Socket bit map
  @param[in]  McBitmap         - Mc bit map
  @param[in]  ChannelBitmap    - Channel bit map

  @retval                      - Total channel count for the interleave

**/
UINT8
GetTotalChannelCountFromAnInterleave (
  IN  UINT8               SocketBitmap,
  IN  UINT8               McBitmap[MAX_SOCKET],
  IN  UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC]
  )
{
  UINT8     SocketIndex;
  UINT8     ChannelCount = 0;

  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++ ) {
    if ((SocketBitmap & (1 << SocketIndex)) == 0) {
      continue;
    }

    ChannelCount += GetTotalChannelCountFromAnInterleaveWithinSocket(McBitmap[SocketIndex], ChannelBitmap[SocketIndex]);
  }

  return ChannelCount;
}

/** Create new SAD entries which encompass TOLM/TOHM gap got or non-existant memory if DRAM high address >4GB.

  Create a SAD entry with limit upto the total interleave size and its data can be cloned in other SAD
  rules differentiated by limit.
  If MMIOL gap found, modify only the limit of the SAD entry first created to include memory <= TOLM - 4GB.
  If DRAM high start is > 4GB, create a SAD for non-existant memory.
  If MMIOH gap found, create a SAD rule encomapssing it.
  When spllit, for succeeding SAD's adjust their SAD limit to include MMIOL/MMIOH hole and NXM before creating
  the last SAD rule dor this interleave.
  If SAD does not split, check for limit <= MMIOH and adjust the limit when >MMIOH.

  @param[in, out] Host                 - Pointer to sysHost.
  @param[in, out] MemMapData           - Pointer to the memory map private data structure.
  @param[in]      SocketNum            - Socket id.
  @param[in]      MemType              - Type of memory that the interleave request corresponds to.
  @param[in]      SocketWays           - Number of sockets participating in the interleave.
  @param[in]      McWays               - Number of IMC's within socket participating in the interleave.
  @param[in]      ChWays               - Number of channels between IMC's within socket participating in the interleave.
  @param[in]      ImcInterBitmap       - Bit map of IMCs participating in the interleave.
  @param[in]      NmImcInterBitmap     - Bit map of IMCs participating in the interleave as near memory.
  @param[in]      ChannelInterBitmap   - Per IMC Bit map of channels participating in the interleave.
  @param[in]      FmChannelInterBitmap - Per IMC Bit map of channels participating in the interleave.
  @param[in]      SadIndex             - Index of the available SAD table entry to fill data in.
  @param[in, out] InterleaveLimit      - Pointer to total interleave address limit.
  @param[in]      Cluster              - SNC cluster this SAD applies to (ignored if SNC is disabled).
  @param[in]      MirrorEnable         - Enable/Disable mirroring for the interleave's SAD rule

  @retval     SUCCESS               -  Interleave completed.
  @retval     FAILURE               -  Interleave could not be supported for input memory type.

**/
UINT32
AddSadEntries (
  IN OUT SYSHOST         *Host,
  IN OUT MEMORY_MAP_DATA *MemMapData,
  IN     UINT8           SocketNum,
  IN     MEM_TYPE        MemType,
  IN     UINT8           SocketWays,
  IN     UINT8           McWays,
  IN     UINT8           ChWays,
  IN     UINT8           ImcInterBitmap,
  IN     UINT8           NmImcInterBitmap,
  IN     UINT8           ChannelInterBitmap[MAX_IMC],
  IN     UINT8           FmChannelInterBitmap[MAX_IMC],
  IN     UINT8           SadIndex,
  IN OUT UINT32          *InterleaveLimit,
  IN OUT UINT8           Cluster,
  IN     UINT8           MirrorEnable
  )
{
  MEMORY_MAP_HOST                 *MemMapHost;
  SOCKET_INFO_STRUCT              *Socket;
  UINT8                           McIndex;
  UINT8                           Index;
  UINT8                           TotalWays;
#ifdef DDRT_SUPPORT
  UINT8                           NmMcWays;
#endif // DDRT_SUPPORT
  UINT8                           MaxImc;
  SAD_TABLE                       *SadEntry;
  UINT32                          McList[MAX_SOCKET * MAX_IMC];
  UINT32                          MemoryToBeAllocated;
  SYS_SETUP                       *Setup;
  UINT16                          Tolm;
  UINT32                          SadBase;
  UINT8                           InterleaveWaysWithinSocket;

  Tolm = GetTolmAddress ();
  Setup = GetSysSetupPointer ();
  MemMapHost = GetMemMapHostPointer ();

  if (IsMemTypeHbm (MemType)) {
    MaxImc = GetHbmStackCount (MemMapData, SocketNum);
  } else {
    MaxImc = GetMaxImc ();
  }

  if ((MemType == MemType1lmDdr) ||
    (MemType == MemType1lmHbm)) {
    InterleaveWaysWithinSocket = GetTotalChannelCountFromAnInterleaveWithinSocket (ImcInterBitmap, ChannelInterBitmap);
  } else {
    InterleaveWaysWithinSocket = GetTotalChannelCountFromAnInterleaveWithinSocket (ImcInterBitmap, FmChannelInterBitmap);
  }

  Socket = &Host->var.mem.socket[SocketNum];
  SadEntry = &Socket->SAD[SadIndex];
  //
  // Init. Index to the start SAD index for input interleave ways
  // Do not change Index value
  //
  Index = SadIndex;

  ZeroMem (McList, sizeof (McList));
  MemoryToBeAllocated = 0;

  if (IsMemType1lm(MemType)) {
    if (ChannelInterBitmap == NULL) {
      return FAILURE;
    }
    CopyMem (SadEntry->channelInterBitmap, ChannelInterBitmap, sizeof (SadEntry->channelInterBitmap));
  } else if (IsMemType2lm (MemType)) {
    if (ChannelInterBitmap == NULL || FmChannelInterBitmap == NULL) {
      return FAILURE;
    }
    CopyMem (SadEntry->channelInterBitmap, ChannelInterBitmap, sizeof (SadEntry->channelInterBitmap));
    CopyMem (SadEntry->FMchannelInterBitmap, FmChannelInterBitmap, sizeof (SadEntry->FMchannelInterBitmap));
  } else {
    if (FmChannelInterBitmap == NULL) {
      return FAILURE;
    }
    CopyMem (SadEntry->FMchannelInterBitmap, FmChannelInterBitmap, sizeof (SadEntry->FMchannelInterBitmap));
  }

  //
  // Fill out MCs participating from the socket
  //
  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if (ImcInterBitmap & (BIT0 << McIndex)) {
      McList[ (SocketNum * MaxImc) + McIndex] = 1;
    }
  }

  //
  // Create a SAD considering the total interleave size.
  // Calculate SadBase first before a SAD is enabled or assigned limit because
  // SAD base is calculated against the limits of all enabled SAD's.
  //
  SadEntry->Base = GetSadBase (Host, MemMapData, SocketNum, SadIndex);
  SadEntry->Enable = 1;
  SadEntry->local = 1;
  SadEntry->Attr = 0;
  if (ChWays == CH_3WAY) {
    SadEntry->ways = InterleaveWaysWithinSocket / CH_3WAY * SocketWays;
  } else {
    SadEntry->ways = McWays * SocketWays;
  }
  SadEntry->type = MemType;
  SadEntry->imcInterBitmap = ImcInterBitmap;
  SadEntry->Limit = *InterleaveLimit;
  SadEntry->Cluster = Cluster;
  SadEntry->mirrored = MirrorEnable;


  //
  // initialize to common values and override as necessary
  //
  SadEntry->granularity = INTERLEAVE_GRANULARITY_256B;
  SadEntry->tgtGranularity = INTERLEAVE_GRANULARITY_256B;

  if (IsMemTypeHbm (MemType)) {
    //
    // HBM memory types
    //
    SadEntry->NmChWays = ChWays;
    if (IsMemType2lm (MemType)) {
      SadEntry->NmChWays = MAX_HBM_IO_CH;
      SadEntry->FmChWays = ChWays;
      SadEntry->NmImcInterBitmap = ImcInterBitmap;
    }
  } else if (MemType == MemType1lmDdr) {
    //
    // 1LM DDR has the same channel and MC granularity.
    // The target is at the socket level.
    //
    if (SocketWays > ONE_WAY) {
      SadEntry->tgtGranularity = INTERLEAVE_GRANULARITY_4KB;
    }
    SadEntry->NmChWays = ChWays;
    SadEntry->NmImcInterBitmap = ImcInterBitmap;

    //
    // Count the number of 1LM x1 interleaves
    //
    if (InterleaveWaysWithinSocket == ONE_WAY) {
      MemMapData->MemMapSocket[SocketNum].Ddr1lmX1IntlvCount[Cluster]++;
    }
#ifdef DDRT_SUPPORT
  } else if (IsMemTypeDdrt (MemType)) {
    if (MemMapData->DdrtIntlvGranularity == INTERLEAVE_GRANULARITY_256B) {
      SadEntry->FmChWays = ChWays;
      if (SocketWays > ONE_WAY) {
        SadEntry->tgtGranularity = INTERLEAVE_GRANULARITY_4KB;
      }
      if (IsMemType2lm (MemType)) {
        SadEntry->NmImcInterBitmap = NmImcInterBitmap;
        GetNearMemoryWays (NmImcInterBitmap, ChannelInterBitmap, &NmMcWays, &SadEntry->NmChWays);
      }
    } else {
      SadEntry->FmChWays = ChWays;
      if (IsMemType2lm (MemType)) {
        if ((SocketWays > TWO_WAY) && (InterleaveWaysWithinSocket == EIGHT_WAY)) {
          SadEntry->tgtGranularity = INTERLEAVE_GRANULARITY_8KB;
        } else {
          SadEntry->tgtGranularity = INTERLEAVE_GRANULARITY_4KB;
        }
        SadEntry->NmImcInterBitmap = NmImcInterBitmap;
        GetNearMemoryWays (NmImcInterBitmap, ChannelInterBitmap, &NmMcWays, &SadEntry->NmChWays);
      } else {
        SadEntry->granularity = INTERLEAVE_GRANULARITY_4KB;
        if ((SocketWays > ONE_WAY) && (InterleaveWaysWithinSocket == EIGHT_WAY)) {
          SadEntry->tgtGranularity = INTERLEAVE_GRANULARITY_8KB;
        } else {
          SadEntry->tgtGranularity = INTERLEAVE_GRANULARITY_4KB;
        }
      }
    }
#endif // DDRT_SUPPORT
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "INVALID SAD TYPE.\n");
    return FAILURE;
  }

  SadEntry->Attr = SadEncodeDramAttribute (SadEntry->type);
  //
  // Check for MMIOL gap. If found, update only the limit to 4GB for the SAD entry determined by SadIndex.
  // For Dram Hi add a SAD which will have data similar to the input SAD struct except for a limit adjustment.
  //
  if (SadEntry->Limit >= Tolm) {
    if ((SadIndex == 0 && SocketWays > ONE_WAY) || (SadIndex == 0 && SocketNum == 0)) {
      if (!(IsMemType1lm (SadEntry->type) || IsMemType2lm (SadEntry->type))) {
        SadEntry->Enable = 0; // invalidate this SAD
        RcDebugPrintWithDevice (SDBG_MINMAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "INVALID CONFIGURATION. The first SAD rule must be 1LM or 2LM region, not %x region\n", MemType);
        FatalError (ERR_NGN, NGN_PMEM_CONFIG_ERROR);
        return FAILURE;
      }

      if (AddTotalMemoryAllocated (Host, MemMapData, Tolm, MemType, McList) == FAILURE) {
        Socket->SAD[SadIndex].Enable = 0;
        return FAILURE;
      }

      //
      // Encompass mem hole into SAD (DRAM lo - 4GB).
      //
      SadEntry->Limit = FOUR_GB_MEM;
      //
      // Adjust limit with the memhole size.
      //
      *InterleaveLimit += (FOUR_GB_MEM - Tolm);

      //
      // Split SAD if limit > TOLM. One includes DRAM Lo + mem hole and the other starts from DRAM Hi
      //
      if (*InterleaveLimit > SadEntry->Limit) {

        SadIndex++;

        //
        // Add a SAD duplicating data from the existing Sad entry excluding only the limit.
        //
        CopyMem (
          (UINT8 *)&Socket->SAD[SadIndex],
          (UINT8 *) SadEntry,
          sizeof (Socket->SAD[SadIndex])
          );

        //
        // Adjust sad base per previous SAD limit since the interleave range is split into multiple SAD's
        // and expected to be continuous.
        //
        Socket->SAD[SadIndex].Base = Socket->SAD[SadIndex - 1].Limit;

        if (ChWays == CH_3WAY) { // 3 ch way is the only supported non power of 2 channel interleave which wont divide TOLM evenly
          //
          // If TOLM boundary is not evenly divisible when interleaving across three channels of two memory controllers,
          // we need to adjust node limit so that it divides evenly.
          //
          TotalWays = InterleaveWaysWithinSocket * SocketWays;
          if (Tolm % TotalWays) {
            //
            // TOLM boundary is a multiple of 64MB less than a boundary evenly divisible by totalWays, so adjust
            // nodeLimit by that multiple in 64MB units, so that it divides evenly.
            //
            *InterleaveLimit -= (TotalWays - (Tolm % TotalWays));
            //
            // Adjusted limit is a multiple of mem granularity, but not necessarily SAD/TAD granularity, so adjust it.
            //
            if (*InterleaveLimit % (GetSadTadGranularity (Host) / MEM_GRANULARITY)) {
              *InterleaveLimit -= TotalWays;
            }
          }
        }

        //
        // Create SAD for non-existant memory if DRAM Hi address is >4GB
        //
        if (Setup->mem.dfxMemSetup.dfxOptions & HIGH_ADDR_EN) {
          if ((SadIndex > 0) && (SadIndex < SAD_RULES_10NM)) {
            if (Socket->SAD[SadIndex].Base == FOUR_GB_MEM) {
              //
              // Adjust limit for 64MB (26 bits) alignment. Start 64MB below address indicated by bit start.
              //
              Socket->SAD[SadIndex].Enable = 1;
              Socket->SAD[SadIndex].type = MemTypeNxm;
              if (Setup->mem.dfxMemSetup.dfxStraddleHighAddressBit) {
                Socket->SAD[SadIndex].Limit = (BIT0 << (Host->var.mem.dfxMemVars.dfxHighAddrBitStart - CONVERT_B_TO_64MB)) - (FOUR_GB_MEM - CONVERT_B_TO_64MB);
              } else {
                Socket->SAD[SadIndex].Limit = BIT0 << (Host->var.mem.dfxMemVars.dfxHighAddrBitStart - CONVERT_B_TO_64MB);
              }
              Socket->SAD[SadIndex].local = 0;

              SadIndex++;

              //
              // Add a SAD duplicating data from the existing Sad entry excluding only the limit.
              //
              CopyMem (
                (UINT8 *)&Socket->SAD[SadIndex],
                (UINT8 *) SadEntry,
                sizeof (Socket->SAD[SadIndex])
                );

              //
              // Adjust sad base per previous SAD limit since the interleave range is split into multiple SAD's
              // and expected to be continuous.
              //
              Socket->SAD[SadIndex].Base = Socket->SAD[SadIndex - 1].Limit;
            }
          } else {
            RcDebugPrintWithDevice (SDBG_MINMAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "SAD rules exceed %d. Cannot create SAD for NXM.\n", SAD_RULES_10NM);
            OutputWarning (
              WARN_INTERLEAVE_FAILURE,
              WARN_SAD_RULES_EXCEEDED,
              SocketNum,
              0xFF,
              0xFF,
              0xFF
              );
            return ERROR_RESOURCE_CALCULATION_FAILURE;
          }
        } // HIGH_ADDR_EN
      } // *InterleaveLimit > Tolm
    } // SadIndex = 0 or SadIndex - 1 < Tolm
  } // SadEntry->Limit >= Tolm

  SadBase = Socket->SAD[SadIndex].Base;

  //
  // Adjust NXM range. Doing it here should cover cases with HBM flat and HBM SNC where InterleaveLimit == Tolm.
  //
  if (SadIndex > 0 && Socket->SAD[SadIndex- 1].type == MemTypeNxm) {
    *InterleaveLimit += (Socket->SAD[SadIndex - 1].Limit - Socket->SAD[SadIndex - 2].Limit);
  }

  if (*InterleaveLimit > Host->var.mem.mmiohBase) {
    *InterleaveLimit = Host->var.mem.mmiohBase;
    if ((*InterleaveLimit - Socket->SAD[SadIndex].Base) % ChWays != 0) {
      *InterleaveLimit -= (*InterleaveLimit - Socket->SAD[SadIndex].Base) % ChWays;
    }
    if (*InterleaveLimit == Socket->SAD[SadIndex].Base) {
      //
      // Should never get here.  If we get here, return FAILURE
      //
      RcDebugPrintWithDevice (SDBG_MINMAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "WARNING!!! Failure to create Sad%d! Sad%d limit is MMIOHBase...\n", SadIndex, SadIndex - 1);
      return FAILURE;
    }
    RcDebugPrintWithDevice (SDBG_MINMAX, SocketNum, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "WARNING!!! SAD%d limit exceeds MMIOHBase! Sad%d is truncated to MMIOHBase...\n", SadIndex, SadIndex);
  }

  //
  // OutSncGbAlignRequired = 1(enabled) AND 1LM or 2LM.
  //
  if ((SncGbAlignRequired ()) && (IsMemType1lm (MemType) || IsMemType2lm (MemType))) {
    if (ChWays == THREE_WAY) {
      //
      // Save adjusted memory size for use in calculating TAD's for 1LM DDR.
      //
      if ((MemType == MemType1lmDdr) && ((*InterleaveLimit & ~GB_BOUNDARY_ALIGN) != 0) &&
          (Socket->SAD[SadIndex - 1].Limit == FOUR_GB_MEM)) {
        MemMapData->SadLimitAdjustment = (*InterleaveLimit - (*InterleaveLimit & GB_BOUNDARY_ALIGN));
      }

      *InterleaveLimit = (*InterleaveLimit & GB_BOUNDARY_ALIGN); //align to GB boundary

      //
      // Check size of memory mapped by this entry to insure that all six channels contributes equally
      //
      while (((*InterleaveLimit - Socket->SAD[SadIndex].Base) % (InterleaveWaysWithinSocket * SocketWays)) != 0) {
        //
        // Each channel did not have an equal contribution, so try next GB boundary
        //
        *InterleaveLimit = *InterleaveLimit - 0x10;
      }
    } else {
      *InterleaveLimit = (*InterleaveLimit & GB_BOUNDARY_ALIGN); //align to GB boundary
    }
  }

  MemoryToBeAllocated = *InterleaveLimit - Socket->SAD[SadIndex].Base;
  if (AddTotalMemoryAllocated (Host, MemMapData, MemoryToBeAllocated, MemType, McList) == FAILURE) {
    ZeroMem (
      (UINT8 *)&Socket->SAD[SadIndex],
      sizeof (Socket->SAD[SadIndex])
    );
    return FAILURE;
  }

  //
  // Assign new SAD limit adjusted per boundary checks
  //
  if (Socket->SAD[SadIndex].Limit != *InterleaveLimit) {
    Socket->SAD[SadIndex].Limit = *InterleaveLimit;
  }

  //
  // SGX PRMRR / TDX PRMRR range cannot co-operate with mirrored range.
  //
  if (MirrorEnable == 0) {
    HandleSecurityRequest (
      MemMapData,
      SocketNum,
      MemType,
      ImcInterBitmap,
      *InterleaveLimit - Socket->SAD[SadIndex].Base,
      Socket->SAD[SadIndex].Base
      );
  }

  if (MemType != MemType1lmHbm) { // SAD interleave list for HBM is static
    //
    // Update SAD Interleave list for all SAD's created
    //
    for (; Index <= SadIndex; Index++) {
      for (McIndex = 0; McIndex < MaxImc; McIndex++) {
        if (ImcInterBitmap & (BIT0 << McIndex)) {
          MemMapHost->SocketInfo[SocketNum].SadIntList[Index][(SocketNum * MaxImc) + McIndex] = 1;
        }
      }
    }
  }

  return SUCCESS;
}

/** Interleave memory between IMC's within a socket or within an IMC.

  This routine figures out the interleavable memory and its mapping. The structures for SAD, TAD are filled.
  It does not program any CSR's.
  The order of mapping will first be volatile memory of all sockets followed by persistent memory of all sockets.

  @param[in] Host       - Pointer to sysHost.
  @param[in] MemMapData - Pointer to the memory map private data structure

  @retval SUCCESS      -  Memory NUMA interleaved.
  @retval FAILURE      -  Interleave failed.

**/
UINT32
SocketNonInterleave (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{

  UINT8   SocketNum;
  UINT8   Ch;
  UINT8   MaxCh;
  UINT32  Status = SUCCESS;

  MaxCh = GetMaxImc () * GetNumChannelPerMc ();

  //
  // Map volatile and persistent memory
  //
  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DDR_MEMMAP, CHECKPOINT_MINOR_SAD_NONINTER, 0));

  //
  // 1. Loop for every socket present in the system
  // 2. Find the volatile memory mode (1LM/2LM)
  // 3. Create SAD rules for volatile memory in socket index
  // 4. Create SAD rules for CXL memory in socket index
  //
  for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {

    //
    // Skip if socket not enabled
    //
    if (CheckSocketEnabled (SocketNum) == 0) {
      continue;
    }

    Status = SocketNonInterleaveVolMem (
               Host,
               MemMapData,
               SocketNum
               );

    if (Status != SUCCESS) {
      return Status;
    }

    if (IfFeatureEnabled (TypeSnc)) {
      Status = AdjustSadTableForSncMulticast (
                 Host,
                 MemMapData,
                 SocketNum
                 );

      if (Status != SUCCESS) {
        return Status;
      }
    }

    Status = ProcessCxlMemDevices (MemMapData, SocketNum);
    if (Status == FAILURE) {
      return Status;
    }
  }

#ifdef DDRT_SUPPORT
  //
  // 1. Loop for every socket present in the system
  // 2. If PMEM is enabled, fill the Ch level Host struct with PMEM related values
  // 3. Create SAD rules for PMEM in socket index
  //
  for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {

    if (CheckSocketEnabled (SocketNum) == 0) {
      continue;
    }

    //
    // Skip if no DDR or HBM memory present. CR support requires at least one DDR DIMM or HBM memory per socket.
    //
    if (GetSocketMemSize (SocketNum) == 0) {
      continue;
    }

    Status = SocketNonInterleavePerMem (
               Host,
               MemMapData,
               SocketNum
               );

    if (Status != SUCCESS) {
      return Status;
    }

    //
    // Add remote SAD rules if socket non-interleave pernament memory is successful
    //
    if (Host->var.mem.dfxMemVars.dfxDimmManagement != DIMM_MGMT_CR_MGMT_DRIVER) {
      Status = AddRemoteSadEntry (Host, MemMapData, SocketNum, GetPerMemType (Host, MemMapData), 0);
      if (Status != SUCCESS) {
        return Status;
      }
    }
  }
#endif // DDRT_SUPPORT

  //
  // For every socket that NUMA interleaved successfully, reload rem channel size in the syshost structure
  //
  for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {

    if (CheckSocketEnabled (SocketNum) == 0) {
      continue;
    }

    for (Ch = 0; Ch < MaxCh; Ch++)  {
      if (PcdGetBool (PcdNvDimmEn)) {
        Host->var.mem.socket[SocketNum].channelList[Ch].remSize = GetChannelMemSize (SocketNum, Ch) - Host->var.mem.socket[SocketNum].channelList[Ch].NVmemSize;
      } else {
        Host->var.mem.socket[SocketNum].channelList[Ch].remSize = GetChannelMemSize (SocketNum, Ch);
      }
    }
  }

  return Status;
}

/** Clear DDR and DDRT route tables

  Clear the internal route tables so that the second run of memmory mapping flow populated the RT's afresh.

  @param[in] Host       - Pointer to sysHost.
  @param[in] MemMapData - Pointer to the memory map private data structure

**/
VOID
ClearRouteTables (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8         Sckt;

  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {

    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }

    ZeroMem (&(MemMapData->FarDdrRouteTableData), sizeof (MemMapData->FarDdrRouteTableData));
    ZeroMem (&(MemMapData->DdrRouteTableData), sizeof (MemMapData->DdrRouteTableData));
    ZeroMem (&(MemMapData->DdrtRouteTableData), sizeof (MemMapData->DdrtRouteTableData));
  }
}

/** Encode memory type based on the SAD entry type.

  Traverse SAD entries to set the memory type for the DRAM rules according to the following encoding:
       00 - Coherent DRAM
       01 - Memory Mapped CFG
       10 - Low Bandwidth Coherent DRAM
       11 - High Bandwidth Coherent DRAM

  @param[in] SadType  - The type of SAD entry

  @return The corresponding 10nm DRAM attribute.

**/
UINT8
SadEncodeDramAttribute (
  IN UINT16   SadType
  )
{
  switch (SadType) {
  case MemType1lmDdr:
#ifdef DDRT_SUPPORT
  case MemType2lmDdrCacheMemoryMode:
  case MemType2lmDdrWbCacheAppDirect:
#endif // DDRT_SUPPORT
    return SAD_ATTR_DRAM;

#ifdef DDRT_SUPPORT
  case MemType1lmAppDirect:
    return SAD_ATTR_DDRT;
#endif // DDRT_SUPPORT

  case MemType1lmHbm:
  case MemType2lmHbmCacheDdr:
  case MemType2lmHbmCacheMemoryMode:
    return SAD_ATTR_HBM;

  default:
    return SAD_ATTR_DRAM;
  }
}

/**

  Find the next remote SAD entry that needs to be filled for this Socket.

  Traverse remote SAD entries for this socket and return the Index of the next available entry.

  @param[in] MemMapData - Pointer to memory map private data structure.
  @param[in] Sckt       - Socket ID.

  @retval Index of next available Remote SAD entry

**/
UINT8
FindRemoteSadIndex (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt
  )
{
  UINT8              Index;
  UINT8              RemoteSadIndex;

  //
  // Scan through remote SAD table and find the next available SAD entry
  //
  for (Index = 0; Index < MemMapData->MaxRemoteSadRules; Index++) {
    if (MemMapData->MemMapSocket[Sckt].RSad[Index].Enable == 0) {
      RemoteSadIndex = Index;
      return RemoteSadIndex;
    }
  }

  return MemMapData->MaxRemoteSadRules;
}

/** Get the SAD address limit of the last memory mapped range.

  Find the highest limit of local SAD rules and the highest remote mapped limit.
  The greatest of them is the highest mapped address that can be used to calculate
  the interleave limit for mapping next range of memory.

  @param[in] Host       - Pointer to sysHost.
  @param[in] MemMapData - Pointer to memory map private data structure.
  @param[in] SocketNum  - Socket id
  @param[in] SadIndex   - Index of SAD table entry within socket
  @param[in] Cluster    - SNC cluster this SAD applies to (ignored if SNC is disabled).

  @retval Address of the last mapped memory range.

**/
UINT32
GetSocketSadLimit (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           SocketNum,
  IN  UINT8           SadIndex,
  IN  UINT8           ClusterId
  )
{
  UINT32             SadLimit;
  UINT8              RemoteSadIndex;
  UINT32             RemoteSadLimit;
  UINT8              DramCluster;

  SadLimit = 0;
  RemoteSadIndex = 0;
  RemoteSadLimit = 0;

  if (MemMapData->NumberofChaDramClusters > 1) {
    DramCluster = ClusterId;
  } else {
    DramCluster = 0;
  }

  //
  // Get index of next available remote SAD entry.
  //
  RemoteSadIndex = FindRemoteSadIndex (
                     MemMapData,
                     SocketNum
                     );
  if (RemoteSadIndex > 0 && RemoteSadIndex < MemMapData->MaxRemoteSadRules) {
    RemoteSadLimit = MemMapData->MemMapSocket[SocketNum].RSad[RemoteSadIndex - 1].Limit;
  }

  if (SadIndex) {
    if (SadIndex % MemMapData->MaxSadRules == 0) {
      SadLimit = GetSadBase (Host, MemMapData, SocketNum, SadIndex);
    } else {
      SadLimit = Host->var.mem.socket[SocketNum].SAD[SadIndex - 1].Limit;
    }
  }
  return (RemoteSadLimit > SadLimit) ? RemoteSadLimit : SadLimit;
}

/** Program remote DRAM decode CSRs for memory map from config info in Host structure.

  Read remote SAD structures filled-in sysHost and program CHA remote decoder CSR's.

  @param[in] MemMapData    - Pointer to memory map private data structure
  @param[in] Sckt          - Socket Id

  @retval None

**/
VOID
WriteRemoteSadCsrs (
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            Sckt
  )
{
  UINT8                                          RemoteSadIndex;
  RSAD_TABLE                                     *RemoteSadEntry;
  REMOTE_SAD_DATA                                RemoteSadData;

  //
  // Skip if socket has no remote DRAM rules setup.
  //
  if (MemMapData->MemMapSocket[Sckt].RSad[0].Enable == 0) {
    return;
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\t\t Write CHA Remote SAD CSRs for Socket: %d\n", Sckt);
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");


  for (RemoteSadIndex = 0; RemoteSadIndex < MemMapData->MaxRemoteSadRules; RemoteSadIndex++) {
    RemoteSadEntry = &MemMapData->MemMapSocket[Sckt].RSad[RemoteSadIndex];

    //
    // Break if we have reached the end of the SAD table
    //
    if (RemoteSadEntry->Enable == 0) {
      break;
    }

    ZeroMem (&RemoteSadData, sizeof (RemoteSadData));

    RemoteSadData.Enabled = RemoteSadEntry->Enable;
    if (RemoteSadEntry->Attr == SAD_CXL_ATTR) {
      RemoteSadData.CxlMemEnabled = TRUE;
    } else {
      RemoteSadData.Attribute = RemoteSadEntry->Attr;
    }
    RemoteSadData.Base = RemoteSadEntry->Base;

    RemoteSadData.SocketId = RemoteSadEntry->NodeId;
    RemoteSadData.Limit = SadTadEncodeLimit (RemoteSadEntry->Limit);

    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "       En  Base     Limit    NodeId    Attr  \n");
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "       %d   0x%05x  0x%05x    0x%x      0x%x  \n",
      RemoteSadData.Enabled,
      RemoteSadData.Base,
      RemoteSadData.Limit,
      RemoteSadData.SocketId,
      RemoteSadData.Attribute
      );

    //
    // Update Remote DRAM CHA CSR
    //
    ChaSetRemoteSadRule (
      Sckt,
      RemoteSadIndex,
      &RemoteSadData
    );
  }

  return;
}

/** Fill in TOLM/TOHM CSRs for memory map from config info in Host structure.

  Calculate Tolm and Tohm and high memory size. Write CSRs for Tolm and high mem programming.

  @param[in] Host       - Pointer to sysHost
  @param[in] MemMapData - Pointer to memory map private data structure

  @retval None

**/
VOID
WriteTolmTohmCsrs (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{

  UINT8                   Sckt;
  UINT8                   Stack;
  UINT8                   SocketsEn = 0;
  UINT32                  SadLimit;
  UINT32                  TolmLimit;
  UINT32                  TohmLimit;
  SYS_SETUP               *Setup;

  Setup = GetSysSetupPointer ();

  //
  // Program the TOLM/TOHM based on the SAD rule limits
  // Note: IIO tolm/tohm (device 0/5)  -- addr range <= Limit
  //       CBO tolm/tohm (device 12/7) -- addr range <  Limit
  //
  SadLimit = SadTadEncodeLimit (GetSadLimit (Host, MemMapData));

  TolmLimit = ((FOUR_GB_MEM - GetLowGap ()) & ~0x3) - 1;

  Host->var.mem.lowMemBase = 0;
  Host->var.mem.lowMemSize = TolmLimit + 1;

  //
  // highMemBase initialized in InitMemoryMap
  //

  //
  // Calculating the number of sockets on which IOT is enabled
  //
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if ((CheckSocketEnabled (Sckt) == 0) || (Host->nvram.mem.socket[Sckt].maxDimmPop == 0)) {
      continue;
    } else {
      SocketsEn += 1;
    }
  }

  if (SadLimit < FOUR_GB_MEM) {
    //
    // No memory above 4GB
    //
    Host->var.mem.highMemSize = 0;
  } else {
    if (Setup->mem.dfxMemSetup.dfxOptions & HIGH_ADDR_EN) {
      if (Setup->mem.dfxMemSetup.dfxStraddleHighAddressBit) {
        Host->var.mem.highMemSize = (SadLimit - ((BIT0 << (Host->var.mem.dfxMemVars.dfxHighAddrBitStart - CONVERT_B_TO_64MB)) - (FOUR_GB_MEM - CONVERT_B_TO_64MB))+ 1);
      } else {
        Host->var.mem.highMemSize = (SadLimit - (BIT0 << (Host->var.mem.dfxMemVars.dfxHighAddrBitStart - CONVERT_B_TO_64MB)) + 1);
      }
    } else {
      Host->var.mem.highMemSize = (SadLimit - FOUR_GB_MEM + 1); // i.e. For 1 socket IOTMemory*1
    }
  }

  TohmLimit = MAX (FOUR_GB_MEM - 1, SadLimit);

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "lowMemBase: 0x%x\nlowMemSize: 0x%x\n",
    Host->var.mem.lowMemBase,
    Host->var.mem.lowMemSize
    );
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "highMemBase: 0x%x\nhighMemSize: 0x%x\nTOLM: 0x%x\nTOHM: 0x%x\n",
    Host->var.mem.highMemBase,
    Host->var.mem.highMemSize,
    TolmLimit,
    TohmLimit
    );

  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }
    for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
      if (KTI_HOST_OUT_PTR->stackPresentBitmap[Sckt] & (BIT0 << Stack)) {
        IioSetTolmTohm (Sckt, Stack, TolmLimit, TohmLimit);
      }
    } // Stack loop
  } // Socket loop

  return;
}

/** Fill in CHA CRSs for memory map from config info in Host structure.

  Read SAD and TAD structures filled-in sysHost and program Mesh2Mem decoder CSR's.

  @param[in] Host          - Pointer to sysHost
  @param[in] MemMapData    - MemMapData  Pointer to the memory map private data structure.
  @param[in] Sckt          - Socket Id

  @retval None

**/
VOID
WriteChaCsrs (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Sckt
  )
{
  UINT8                              Ch = 0;
  UINT8                              McIndex = 0;
  UINT8                              BitsToShift = 0;
  UINT8                              MinScktIntlv = 0;
  UINT8                              MinChIntlv = 0;
  UINT8                              TargetIntlv = 0;
  UINT8                              ChannelIntlv = 0;
  UINT8                              DisSpecRdValue = 0;
  UINT32                             MinNmSize;
  UINT32                             TwoLmMask;
  UINT8                              MaxImc;
  UINT8                              NumChPerMc;
  BOOLEAN                            FoundSad = FALSE;
  KTI_HOST_IN                       *KtiHostInPtr;
  BOOLEAN                            DirDisable = FALSE;

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\t\t Write CHA CSRs for Socket: %d\n", Sckt);
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

  MaxImc = GetMaxImc ();
  KtiHostInPtr = KTI_HOST_IN_PTR;

  //
  // Is there a DRAM rule on this socket that represents 2LM or WB PMEM memory?
  //
  if (Host->var.mem.CacheMemType == CACHE_TYPE_HBM_CACHE_DDR) {
    FoundSad = TRUE;
  } else {
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if (IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lm)) {
        FoundSad = TRUE;
        break;
      }
    }
  }

  //
  // 2LM DRAM Rule on this Socket
  //
  if (FoundSad) {

    //
    //  1. Program TWO_LM_CONFIG register for this socket
    //
    //
    // Both aggressive and safe/normal algorithms get the same result,
    // because it is always fully populated and interleaved. Use the
    // aggressive algorithm for convenience: find the NM size per target.
    //
    MinNmSize = CalculateMinNmSizeForMask (Host, MemMapData, Sckt);

    //
    // Mask is 24 bits, so remove bits based on the difference between the nmSize (to the nearest power of two) and 256MB.
    //
    BitsToShift = 24 - (Log2x32 (GetPowerOfTwox32 (MinNmSize)) - Log2x32 (MEM_256MB_IN_64MB_UNIT));

    //
    // Set MinScktIntlv to maximum socket way encoding
    //
    MinScktIntlv = TadEncodeSkWays (MAX_SAD_WAYS);
    MinChIntlv = 0;
    NumChPerMc = GetNumChannelPerMc ();
    //
    // Loop through each memory controller and channel
    //
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      //
      // No 2LM or WBPMEM on this memory controller
      //
      if (!IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lm)) {
        continue;
      }

      for (Ch = 0; Ch < NumChPerMc; Ch++) {

        //
        // Check if NM for this channel is populated
        //
        if (GetChannelMemSize (Sckt, (McIndex * NumChPerMc) + Ch) == 0) {
          continue;
        }

        //
        // Get the NM interleave ways in a power of two
        //
        TargetIntlv = (UINT8)Log2x32 ((UINT32)MemMapData->MemMapSocket[Sckt].NmIntlvValues[McIndex].NmTargetWays);
        ChannelIntlv = (UINT8)Log2x32 ((UINT32)MemMapData->MemMapSocket[Sckt].NmIntlvValues[McIndex].NmChannelWays);

        //
        // Pick the smallest socket interleave
        //
        if (TargetIntlv < MinScktIntlv) {
          MinScktIntlv = TargetIntlv;
        }
        //
        // Pick the smallest ch interleave.
        //
        if (ChannelIntlv > MinChIntlv) {
          MinChIntlv = ChannelIntlv;
        }
      } // Ch
    } // McIndex


    //
    // Remove bits based on the MinScktIntlv.  Remove 0 bits for 1-way interleaving, 1 bit for 2-way interleaving, 2 bits for 4-way interleaving, 3 bits for 8-way interleaving.
    // Note that minScktIntlv is encoded as 1-way (00b), 2-way (01b), 4-way (10b), and 8-way (11b), so the number of bits to shift matches the value of the MinScktIntlv
    //
    BitsToShift = BitsToShift - MinScktIntlv;

    //
    // For Safe option only, Remove bits based on the MinChIntlv too. MinChIntlv will have 1 for 2 way NM ch interleave alone. else 0.
    // Remove 1 bits for 2 way NM ch interleave and 0 bits for 1 way or 3 NM ch ways.
    //
    if (Host->var.mem.dfxMemVars.dfxCfgMask2LM == CFG_MASK_2LM_NORMAL) {
      BitsToShift = BitsToShift - MinChIntlv;
    }

    TwoLmMask = 0xFFFFFF >> BitsToShift;

    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "TWO_LM_CONFIG_CHA_PMA_REG\n"
      );
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\tenable: 0x1\tmask: 0x%x\tmask_hi: 0x%x\n",
      TwoLmMask & 0x3FFFF, (TwoLmMask >> 18) & 0x3F
      );
    ChaSet2lmRegisters (Sckt, TwoLmMask);

    if (KtiHostInPtr->DirectoryModeEn == KTI_DISABLE) {
      DirDisable = TRUE;
    }

    ChaDisable2lmOptimization (Sckt, DirDisable);

  } else { // 2LM DRAM Rule not on this Socket
    //
    //  HA_COH.dis_spec_rd should only be set to 1 if non-2LM and HitMe$ is enabled
    //
    if (IfFeatureEnabled (TypeHitMe)) {
      DisSpecRdValue = 1;
    }
  }

  //
  // Update the HA_COH_CHABC_SAD1_REG
  //
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "HA_COH_CHABC_SAD1_REG\n"
    );
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\tdis_spec_rd: 0x%x\n",
    DisSpecRdValue
    );

  ChaDisableEarlyRead (Sckt, DisSpecRdValue, TRUE);
}

/** Program MC features which BIOS need to set differently based on
the system topology/configuration or for performance/power reasons

@param[in] Host          - Pointer to sysHost
@param[in] Sckt          - Socket Id

@retval N/A

**/
VOID
SetSysFeatures0Mc (
  IN  PSYSHOST        Host,
  IN  UINT8           Sckt
  )
{
  CHANNEL_NVRAM_STRUCT                  (*ChannelNvList)[MAX_CH];
  UINT8                                 ChIndex;
  UINT8                                 McIndex;
  UINT32                                FeatureBitMap;
  UINT8                                 MaxImc;
  UINT8                                 NumChPerMc;

  ChannelNvList = GetChannelNvList (Host, Sckt);
  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();
  FeatureBitMap = 0;

  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if (!CheckSocketMcEnabled (Sckt, McIndex)) {
      continue;
    }

    RcDebugPrint (SDBG_MAX, "\n  Programs feature registers for MC %d\n", McIndex);

    for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
      if ((*ChannelNvList)[ChIndex + (McIndex * NumChPerMc)].enabled == 0) {
        continue;
      }

      //
      // If we are in 2lm mode & in single socket config; set FrcDirI to 1.
      //
      if ((CheckSysConfig (SYS_CONFIG_1S)) && (IsMemTypePresentSystemFunction (IsMemType2lm))) {
        FeatureBitMap |= FEATURE_FORCE_DIR_INV;
      }

      //
      // When directory mode is disabled, update the dirwrdisable, MC register to be consistent w/ M2MEM
      //
      if (!IfFeatureEnabled (TypeDirectoryMode)) {
        FeatureBitMap |= FEATURE_DIR_WR_DIS;
      }

      McSetFeatures (
        Sckt,
        McIndex,
        ChIndex,
        FeatureBitMap
        );
    } // Ch loop
  }// McIndex loop

  return;
}

#ifdef DDRT_SUPPORT
/**

  Get highest address of the memory range being cached in the remote far memory.

  @param[in] Host          Pointer to sysHost
  @param[in] MemMapData    MemMapData  Pointer to the memory map private data structure.
  @param[in] Sckt          Socket Id
  @param[in] McIndex       Memory controller number
  @param[in] ClusterId     Cluster Id for the multicast access

  @retval Highest address of the memory range being cached within the MC in XTile.
          0 if 2LM is not Xtile flow enabled.

**/
UINT32
GetTadLimitForXTileNm (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Sckt,
  IN  UINT8           McIndex,
  IN  UINT8           ClusterId
  )
{
  UINT8        FmMcId;
  UINT32       NmCacheLow;
  UINT32       NmCacheHigh;

  if (!MemMapData->MemMapSocket[Sckt].XTile2lm) {
    return 0;
  }

  //
  // Get the FM MC corresponding to the NM MC.
  //
  FmMcId = MemMapData->MemMapSocket[Sckt].XTile2lmFmTarget[McIndex];

  //
  // Get the highest limit of 2LM SAD mapped to the FM MC.
  //
  FindCachedRange (Host, MemMapData, Sckt, FmMcId, ClusterId, &NmCacheHigh, &NmCacheLow);

  return NmCacheHigh;
}
#endif // DDRT_SUPPORT

/**

  Checks whether the memory controller is acting as x-tile caching. The return value of this
  function only takes effect when MC supports 2LM.

  @param[in]  MemMapData     - Pointer to the memory map private data structure.
  @param[in]  SocketId       - The socket index.
  @param[in]  McId           - The MC index.

  @retval TRUE       The MC is acting as x tile 2lm.
  @retval FALSE      The MC is acting as in-tile 2Lm.

**/
BOOLEAN
IsMc2lmXTileCaching (
  IN  MEMORY_MAP_DATA    *MemMapData,
  IN  UINT8              SocketId,
  IN  UINT8              McId
  )
{
  if (MemMapData->MemMapSocket[SocketId].XTile2lm) {
    if (MemMapData->MemMapSocket[SocketId].XTile2lmFmTarget[McId] != McId) {
      return TRUE;
    }
  }

  return FALSE;
}

/** Fill Sad2tad mappinsg and TAD regions types CSRs from config. info in Host structure.

  Read SAD and TAD structures filled-in sysHost and program Mesh2Mem decoder CSR's.

  @param[in] Host          - Pointer to sysHost
  @param[in] MemMapData    - MemMapData  Pointer to the memory map private data structure.
  @param[in] Sckt          - Socket Id

  @retval None

**/
VOID
WriteMesh2MemCsrs (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Sckt
  )
{
  UINT8                 SadIndex;
  UINT8                 BeginSadIndex;
  UINT8                 EndSadIndex;
  UINT8                 ClusterId;
  UINT8                 TadIndex;
  UINT8                 McIndex;
#ifdef DDRT_SUPPORT
  UINT8                 ImcCh;
  UINT8                 Ch;
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];
#endif // DDRT_SUPPORT
  MEM_VAR_STRUCT        *Mem;
  SAD_TABLE             *SadEntry;
  IMC_INFO_STRUCT       *Imc;
  UINT8                 MaxImc;
  UINT8                 MaxChPerMc;
  BOOLEAN               McParticipates = TRUE;
  UINT8                 NextTadId[MAX_IMC];
  UINT32                Limit;
  UINT32                AddressLimitLo;
  UINT32                AddressLimitHi;
  SAD_TAD_CONFIG        SadTadConfig;
  SYS_SETUP             *Setup;
  BOOLEAN               IsMcXTileCaching;
  BOOLEAN               McAsXTileNm;
  BOOLEAN               XTileNmTadCreated[MAX_IMC];

  Setup = GetSysSetupPointer ();

  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();

  Limit = 0;
  AddressLimitLo = 0;
  AddressLimitHi = 0;
  Imc = &Host->var.mem.socket[0].imc[0];

  IsMcXTileCaching = FALSE;

  ZeroMem ((VOID *) (UINTN)&SadTadConfig, sizeof (SAD_TAD_CONFIG));

  Mem = &Host->var.mem;
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"\n");
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\t\tWrite MESH2MEM CSRs for Socket: %d\n", Sckt);
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

  for (McIndex = 0; McIndex < MAX_IMC; McIndex++) {
    NextTadId[McIndex] = 0;
    XTileNmTadCreated[McIndex] = FALSE;
  }

  //
  // Loop through each memory controller
  //
  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if (!CheckSocketMcEnabled (Sckt, McIndex)) {
      continue;
    }

    AddressLimitLo = 0;
    AddressLimitHi = 0;
    ZeroMem ((VOID *) (UINTN)&SadTadConfig, sizeof (SAD_TAD_CONFIG));
    //
    // 1. Program MODE_M2MEM_MAIN_REG
    //
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Memory Controller : %d\n", McIndex);
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "------------------------\n");

    if (IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lm)) {
      IsMcXTileCaching = IsMc2lmXTileCaching (MemMapData, Sckt, McIndex);

      //
      // If Xtile flows, set Defeatures0.WrCmpFromNm.
      //
      if (MemMapData->MemMapSocket[Sckt].XTile2lm || IsMemTypePresentMcEnum (Sckt, McIndex, MemType2lmHbmCacheDdr)) {
        RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "DEFEATURES0_M2MEM_MAIN_REG\n"
          "\twrcmpfromnm: 1\n"
          );
        SetM2mDefeatures0WrCmpFromNm (Sckt, McIndex, TRUE);
      }
    }

    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "MODE_M2MEM_MAIN_REG\n"
      "\tnmcaching:0x%x  pmem:0x%x  blockregion:0x%x in-tile caching: 0x%x\n",
       IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lm),
       IsMemTypePresentMcFunction (Sckt, McIndex, IsMemTypeAppDirect),
       IsMemTypePresentMcEnum (Sckt, McIndex, MemType1lmCtrl),
       ((IsMcXTileCaching) ? 0 : 1)
     );

    SetM2mModeMmap (
      Sckt,
      McIndex,
      IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lm),
      IsMemTypePresentMcFunction (Sckt, McIndex, IsMemTypeAppDirect),
      IsMemTypePresentMcEnum (Sckt, McIndex, MemType1lmCtrl),
      IsMcXTileCaching
      );

#ifdef DDRT_SUPPORT
    //
    // Update X-Tile 2LM field in Mesh2mem IP.
    //
    if (IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lmDdr)) {
      SetM2mFmInSameTile (Sckt, McIndex, IsMcXTileCaching);
    }

    //
    // Update DDRT exist field in Mesh2mem IP.
    //
    if (IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lmDdr) ||
      IsMemTypePresentMcFunction (Sckt, McIndex, IsMemTypeAppDirect) ||
      IsMemTypePresentMcEnum (Sckt, McIndex, MemType1lmCtrl)) {
      SetM2mDdrtExists (Sckt, McIndex, TRUE);
    }

    if (!IsSiliconWorkaroundEnabled("S2205621290")) {
      if (IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lmDdr)) {
        if ((Setup->mem.EliminateDirectoryInFarMemory == ELIMINATE_DIRECTORY_IN_FM_ENABLE) && !CheckSysConfig (SYS_CONFIG_1S)) {
          ProgramM2m2lmDirOpt (Sckt, McIndex, EnableM2m2lmDirOpt);
          ChaEliminateDirectoryInFarMemory (Sckt);
        } else {
          ProgramM2m2lmDirOpt (Sckt, McIndex, DisableM2m2lmDirOpt);
        }
      }

      //
      // Check if Disable Directory bit should programmed. By default, this bit should be set (disabled)
      //
      if (IsMemTypePresentMcFunction (Sckt, McIndex, IsMemTypeAppDirect)) {
        if (Setup->mem.DisableDirForAppDirect == DIRECTORY_FOR_APP_DIRECT_ENABLE) {
          ChaDisableDirForAppDirect (Sckt);
          SetM2mDisableDir4AppDirect (Sckt, McIndex, 1);

          ChannelNvList = GetChannelNvList (Host, Sckt);
          for (ImcCh = 0; ImcCh < MaxChPerMc; ImcCh++) {
            Ch = MEM_IMCCH_TO_SKTCH (McIndex, ImcCh);
            if ((*ChannelNvList)[Ch].enabled == 0) {
              continue;
            }
            McDisableDirForAppDirect (MemTechDdr, Sckt, Ch, 1);
          }
        }
      }
    } else {
      ProgramM2m2lmDirOpt (Sckt, McIndex, DisableM2m2lmDirOpt);
    }

    //
    // 2. Program SYSFEATURES0_M2MEM_MAIN_REG
    //
    if (IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lmDdr) ||
        IsMemTypePresentMcFunction (Sckt, McIndex, IsMemTypeAppDirect) ||
        IsMemTypePresentMcEnum (Sckt, McIndex, MemType1lmCtrl)) {

      SetM2mSysFeatures0PreftchDis (Sckt, McIndex, TRUE);

      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "SYSFEATURES0_M2MEM_MAIN_REG\n"
        );
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\tprefdisable:1\n"
        );
    }
#endif // DDRT_SUPPORT
  } // McIndex

  //
  // 3. Program M2Mem TAD_WR registers for TAD mapping.
  // M2Mem TAD_WR must be programmed in the increasing order of TAD limits.
  //

  //
  // 3.1 Program M2Mem TAD_WR registers for 1LM/2LM volatile memory modes.
  //
  for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
    BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
    EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
    if (Mem->socket[Sckt].SAD[BeginSadIndex].Enable == 0) {
      break;
    }

    for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
      SadEntry = &Mem->socket[Sckt].SAD[SadIndex];

      if (SadEntry->Enable == 0) {
        break;
      }

      if (SadEntry->local == 0) {
        continue;
      }

      if (!IsMemTypeDdr (SadEntry->type) && !IsMemType2lmHbm (SadEntry->type)) {
        continue;
      }

      for (McIndex = 0; McIndex < MaxImc; McIndex++) {
        if (McIndex < MaxImc) {
          McParticipates = ((SadEntry->imcInterBitmap & (BIT0 << McIndex)) != 0);
          Imc = &Host->var.mem.socket[Sckt].imc[McIndex];
        } else {
          McParticipates = FALSE;
        }

        if (!CheckSocketMcEnabled (Sckt, McIndex)) {
          continue;
        }

        //
        // Check whether the MC acts as X-Tile near memory for other MC.
        //
        McAsXTileNm = FALSE;
        if (!McParticipates && ((SadEntry->NmImcInterBitmap & (BIT0 << McIndex)) != 0)) {
          McAsXTileNm = TRUE;
        }

        //
        // Skip if this mc is not a part of the interleave and it does not act as X-tile 2lm near memory.
        //
        if (McParticipates || (McAsXTileNm && !XTileNmTadCreated [McIndex])) {
          if (McIndex < MaxImc) {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"MC: %d\n", McIndex);
          }

          AddressLimitLo = 0;
          AddressLimitHi = 0;

          ZeroMem ((VOID *)(UINTN)&SadTadConfig, sizeof (SAD_TAD_CONFIG));

          //
          // Look up the TAD for DDR MC
          //
          if (McAsXTileNm) {
            TadIndex = 0;
            XTileNmTadCreated[McIndex] = TRUE;
          } else {
            //
            // Match Sad to a Tad for the TADCHNILVOFFSET used for 1LM DDR in MCDDR, "flat mode" (=1LM) for
            // MCHBM. For DDRT, used for AppDirect, Block mode and FM decode in MCDDR since FM is
            // essentially 1LM.
            //
            for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex++) {
              if ((Imc->TAD[TadIndex].SADId == SadIndex) && (Imc->TAD[TadIndex].Limit != 0)) {
                break;
              }
            }
          }

          SadTadConfig.SadTadId = NextTadId[McIndex];
          SadTadConfig.SadTadValid = TRUE;
          NextTadId[McIndex]++;
          //
          // 1LM and 2LM SAD entries have associated DDR TAD entries
          //
          if ((SadEntry->type == MemType1lmDdr) || IsMemType2lmDdr (SadEntry->type) || (SadEntry->type == MemType2lmHbmCacheDdr)) {
            SadTadConfig.Ddr4TadId = TadIndex;
            //
            // DDR4 set for 1LM and 2LM X-tile on NM MC or FM DDR tile in HBM$
            //
            if (SadEntry->type == MemType1lmDdr) {
              SadTadConfig.Ddr4 = TRUE;
            } else {
              if (McAsXTileNm) {
                SadTadConfig.Ddr4 = TRUE;
              }
              if (SadEntry->type == MemType2lmHbmCacheDdr) {
                SadTadConfig.Ddr4 = TRUE;
                //
                // Indicate whether TAD entry represents a near-memory cached region in which the
                // far-memory is non-persistent.
                //
                SadTadConfig.NonPersistentFm = TRUE;
              }
              SadTadConfig.NmCacheableValid = TRUE;
            }
          }

#ifdef DDRT_SUPPORT
          if ((SadEntry->type == MemType2lmDdrCacheMemoryMode) || (SadEntry->type == MemType2lmDdrWbCacheAppDirect)) {
            SadTadConfig.DdrtTadId = TadIndex;
          }

          if (SadEntry->type == MemType2lmDdrWbCacheAppDirect) {
            SadTadConfig.PmemValid = TRUE;
          }
#endif // DDRT_SUPPORT

          SadTadConfig.BlockValid = FALSE;
          SadTadConfig.Mirror = (BOOLEAN)SadEntry->mirrored;

          //
          // Set address limit
          //
          if (McAsXTileNm) {
#ifdef DDRT_SUPPORT
            Limit = GetTadLimitForXTileNm (Host, MemMapData, Sckt, McIndex, SadEntry->Cluster);
#endif // DDRT_SUPPORT
          } else {
            if ((SadIndex == 0) && (SadEntry->Limit == FOUR_GB_MEM) && (SadEntry->type == MemType1lmDdr)) {
              //
              // Workaround for Uncorrectable errors detected when part of the DRAM memory is lost due to MMIO hole
              // and GB alignment of SAD limits.
              //
              Limit = SadTadEncodeLimit (Imc->TAD[TadIndex].Limit + MemMapData->SadLimitAdjustment);
            } else {
              Limit = SadTadEncodeLimit (Imc->TAD[TadIndex].Limit);
            }
          }
          AddressLimitLo = Limit & 0x3F; //Bits[31:26]
          AddressLimitHi = Limit >> 6; //Bits[51:32]
#ifdef DDRT_SUPPORT
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "\ttadid:0x%x tadvld:0x%x pmemvld:0x%x blkvld:0x%x"
            " ddr4tadid:0x%x ddrttadid:0x%x ddr4:0x%x"
            " nmcacheablevld:0x%x mirror:0x%x secondary1st:0x%x"
            " frcnpwr:0x%x spare:0x%x addresslimit[51:26]:0x%x",
            SadTadConfig.SadTadId, SadTadConfig.SadTadValid,
            SadTadConfig.PmemValid, SadTadConfig.BlockValid,
            SadTadConfig.Ddr4TadId, SadTadConfig.DdrtTadId,
            SadTadConfig.Ddr4, SadTadConfig.NmCacheableValid,
            SadTadConfig.Mirror, 0,
            0, 0,
            ((AddressLimitHi << 6) | AddressLimitLo)
            );
#else
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "\ttadid:0x%x tadvld:0x%x pmemvld:0x%x blkvld:0x%x"
            " ddr4tadid:0x%x ddr4:0x%x"
            " nmcacheablevld:0x%x mirror:0x%x secondary1st:0x%x"
            " frcnpwr:0x%x spare:0x%x addresslimit[51:26]:0x%x",
            SadTadConfig.SadTadId, SadTadConfig.SadTadValid,
            SadTadConfig.PmemValid, SadTadConfig.BlockValid,
            SadTadConfig.Ddr4TadId, SadTadConfig.Ddr4,
            SadTadConfig.NmCacheableValid, SadTadConfig.Mirror, 0,
            0, 0,
            ((AddressLimitHi << 6) | AddressLimitLo)
            );
#endif // DDRT_SUPPORT
          if (SadEntry->type == MemType2lmHbmCacheDdr) {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              " nonpersistentfm:0x%x",
              SadTadConfig.NonPersistentFm
              );
          }
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "\n",
            SadTadConfig.NonPersistentFm
            );

          //
          // Set the write-enable bit to 1 after configuring all fields
          //
          SadTadConfig.SadTadWrEn = TRUE;
          SadTadConfig.AddressLimit = ((AddressLimitHi << 6) | AddressLimitLo);
          SetM2mTadConfigs (
            Sckt,
            McIndex,
            &SadTadConfig
            );

        } // (SadEntry->imcInterBitmap) & (BIT0<<McIndex)

        //
        // Program SYSFEATURES0_M2MEM_MAIN_REG based on mirror
        //
        if (GetM2mTadMirror (Sckt, McIndex)) {

          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "SYSFEATURES0_M2MEM_MAIN_REG\n"
          );
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "\tmirror is enabled; prefdisable:1\n"
          );
          SetM2mSysFeatures0PreftchDis (Sckt, McIndex, TRUE);

        }
      } // McIndex
    } // SadIndex
  } // ClusterId

#ifdef DDRT_SUPPORT
  //
  // 3.2 Program M2Mem TAD_WR registers for 1LM AppDirect.
  //
  for (SadIndex = 0; SadIndex < MemMapData->MaxSadRules; SadIndex++) {
    SadEntry = &Mem->socket[Sckt].SAD[SadIndex];

    if (SadEntry->Enable == 0) {
      break;
    }

    if (SadEntry->local == 0) {
      continue;
    }

    if (SadEntry->type != MemType1lmAppDirect) {
      continue;
    }

    for (McIndex = 0; McIndex < MaxImc; McIndex++) {

      if (!CheckSocketMcEnabled (Sckt, McIndex)) {
        continue;
      }

      if ((SadEntry->imcInterBitmap & (BIT0 << McIndex)) == 0) {
        continue;
      }

      Imc = &Host->var.mem.socket[Sckt].imc[McIndex];

      //
      // Match SAD to a TAD for the TADCHNILVOFFSET used for 1LM AppDirect
      //
      for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex++) {
        if ((Imc->TAD[TadIndex].SADId == SadIndex) && (Imc->TAD[TadIndex].Limit != 0)) {
          break;
        }
      }

      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"MC: %d\n", McIndex);

      AddressLimitLo = 0;
      AddressLimitHi = 0;
      ZeroMem ((VOID *)(UINTN)&SadTadConfig, sizeof (SAD_TAD_CONFIG));

      SadTadConfig.SadTadId = NextTadId[McIndex]++;
      SadTadConfig.SadTadValid = TRUE;
      SadTadConfig.DdrtTadId = TadIndex;
      SadTadConfig.PmemValid = TRUE;

      Limit = SadTadEncodeLimit (Imc->TAD[TadIndex].Limit);
      AddressLimitLo = Limit & 0x3F; //Bits[31:26]
      AddressLimitHi = Limit >> 6; //Bits[51:32]

      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\ttadid:0x%x tadvld:0x%x pmemvld:0x%x blkvld:0x%x"
        " ddr4tadid:0x%x ddrttadid:0x%x ddr4:0x%x"
        " nmcacheablevld:0x%x mirror:0x%x secondary1st:0x%x"
        " frcnpwr:0x%x spare:0x%x addresslimit[51:26]:0x%x\n",
        SadTadConfig.SadTadId, SadTadConfig.SadTadValid,
        SadTadConfig.PmemValid, SadTadConfig.BlockValid,
        SadTadConfig.Ddr4TadId, SadTadConfig.DdrtTadId,
        SadTadConfig.Ddr4, SadTadConfig.NmCacheableValid,
        SadTadConfig.Mirror, 0,
        0, 0,
        ((AddressLimitHi << 6) | AddressLimitLo)
        );

      //
      // Set the write-enable bit to 1 after configuring all fields
      //
      SadTadConfig.SadTadWrEn = TRUE;
      SadTadConfig.AddressLimit = ((AddressLimitHi << 6) | AddressLimitLo);
      SetM2mTadConfigs (
        Sckt,
        McIndex,
        &SadTadConfig
        );

    } // McIndex
  } // SadIndex

  //
  // 3.3 Program M2Mem TAD_WR registers for Block decoder.
  //
  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    Imc = &Host->var.mem.socket[Sckt].imc[McIndex];
    for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex++) {
      if (Imc->TAD[TadIndex].SADId != INVALID_SAD) {
        //
        // only consider MC associated with block decoder
        //
        continue;
      }
      ZeroMem ((VOID *) (UINTN) &SadTadConfig, sizeof (SAD_TAD_CONFIG));
      SadTadConfig.SadTadId = NextTadId[McIndex]++;
      SadTadConfig.SadTadValid = TRUE;
      SadTadConfig.DdrtTadId = TadIndex;
      SadTadConfig.BlockValid = TRUE;

      Limit = SadTadEncodeLimit (Imc->TAD[TadIndex].Limit);
      AddressLimitLo = Limit & 0x3F; //Bits[31:26]
      AddressLimitHi = Limit >> 6; //Bits[51:32]

      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,"MC: %d\n", McIndex);
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\ttadid:0x%x tadvld:0x%x pmemvld:0x%x blkvld:0x%x"
        " ddr4tadid:0x%x ddrttadid:0x%x ddr4:0x%x"
        " nmcacheablevld:0x%x mirror:0x%x secondary1st:0x%x"
        " frcnpwr:0x%x spare:0x%x addresslimit[51:26]:0x%x\n",
        SadTadConfig.SadTadId, SadTadConfig.SadTadValid,
        SadTadConfig.PmemValid, SadTadConfig.BlockValid,
        SadTadConfig.DdrtTadId, 0, SadTadConfig.Ddr4,
        SadTadConfig.NmCacheableValid, SadTadConfig.Mirror, 0,
        0, 0,
        ((AddressLimitHi << 6) | AddressLimitLo)
        );
      //
      // Set the write-enable bit to 1 after configuring all fields
      //
      SadTadConfig.SadTadWrEn = TRUE;
      SadTadConfig.AddressLimit = ((AddressLimitHi << 6) | AddressLimitLo);
      SetM2mTadConfigs (
        Sckt,
        McIndex,
        &SadTadConfig
        );
    }
  }
#endif // DDRT_SUPPORT

  SetSysFeatures0Mc (Host, Sckt);

  WriteHbmMesh2MemCsrs (MemMapData, Sckt);

  return;
}

/**
  Returns a pointer to the Route Table Data corresponding to the input memory type. This pointer is only valid as long
  as the MemMapData is in scope. This function is intended to be called as part of the 4MC NUMA Interleaving algorithm.
  When called for configs with less than 2 MCs, then the DDR route table pointer is returned by default.

  Note that the distinction between silicon route tables (i.e. RT1\RT2\RT2LM) is done later in the flow during the
  CSR write phase. The DDR route table is used for 1LM or 2LM volatile memory modes and can populate RT1 in 1LM & x-tile 2LM
  or RT1 and RT2LM for in-tile 2LM. The DDRT table is used to populate RT2.

  @param[in]      MemMapData         Pointer to the memory map data structure.
  @param[in]      SocketId           The index of socket.
  @param[in]      MemType            Type of memory Route Tables to search

  @retval ROUTE_TABLE_DATA* A Pointer to the Route Table Data.
**/
ROUTE_TABLE_DATA*
GetRouteTableDataForMemType (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           SocketId,
  IN  MEM_TYPE        MemType
  )
{
  ROUTE_TABLE_DATA *RouteTableData = NULL;

  switch (MemType) {
  case MemType1lmDdr:
    RouteTableData = &(MemMapData->DdrRouteTableData);
    RouteTableData->MaxRtWays = MAX_RT_WAYS;
    break;
#ifdef DDRT_SUPPORT
  case MemType2lmDdrCacheMemoryMode:
  case MemType2lmDdrWbCacheAppDirect:
    RouteTableData = &(MemMapData->DdrRouteTableData);
    if (!MemMapData->Is2LmUsingRt2lm) {
      RouteTableData->MaxRtWays = MAX_RT_WAYS;
    } else {
      RouteTableData->MaxRtWays = MAX_4K_DDRT_RT_WAYS;
    }
    break;
  case MemType1lmAppDirect:
    RouteTableData = &(MemMapData->DdrtRouteTableData);
    if (MemMapData->DdrtIntlvGranularity == INTERLEAVE_GRANULARITY_256B) {
      RouteTableData->MaxRtWays = MAX_RT_WAYS;
    } else {
      RouteTableData->MaxRtWays = MAX_4K_DDRT_RT_WAYS;
    }
    break;
#endif // DDRT_SUPPORT
  default:
    RouteTableData = &(MemMapData->DdrRouteTableData);
    RouteTableData->MaxRtWays = MAX_RT_WAYS;
    break;
  }

  return RouteTableData;
}

/**
  Find a DDR route table for the given interleave.

  Optionally, return the matching route table (H0/H1) for the route table that
  matched, and the index for single target interleaves (always zero if not
  single-target).

  @param[in]      MemMapData         Pointer to the memory map data structure.
  @param[in]      Socket             Socket index.
  @param[in]      MemType            Type of memory Route Tables to search
  @param[in]      Cluster            Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      McBitmap           Bitmap of MCs in the interleave.
  @param[in]      ChannelBitmap      Array of bitmaps of channels.
  @param[out]     MatchingHalfBitmap Return location for the matching half
  @param[out]     SingleTarget       Index for single target interleaves, or zero.

  @retval TRUE          A matching DDR route table was found.
  @retval FALSE         No matching DDR route table was found.
**/
BOOLEAN
FindRouteTable (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Socket,
  IN  MEM_TYPE        MemType,
  IN  UINT8           Cluster,
  IN  UINT8           McBitmap,
  IN  UINT8           ChannelBitmap[MAX_IMC],
  OUT UINT8           *MatchingHalfBitmap,    OPTIONAL
  OUT UINT8           *SingleTargetIndex      OPTIONAL
  )
{
  BOOLEAN             Found;
  BOOLEAN             SingleTarget;
  ROUTE_TABLE_HALVES  Half;
  UINT8               McBitmapCopy;
  UINT8               ChannelBitmapCopy[MAX_IMC];
  UINT8               McIndex;
  UINT8               ChannelIndex = 0;
  UINT8               RtIndex;
  UINT8               MaxRtIndex;
  UINT8               MatchingHalfBitmapInternal;
  ROUTE_TABLE_DATA    *RouteTableData;
  UINT8               McWays;
  UINT8               ChWays[MAX_IMC];
  UINT8               ChIndex;
  BOOLEAN             RestartSearch;
  UINT8               MaxImc;
  UINT8               MaxChPerMc;

  //
  // function parameter checking
  //
  if (MemMapData == NULL || Socket >= MAX_SOCKET || Cluster >= MAX_CLUSTERS || McBitmap == 0) {
    return FALSE;
  }
  Found = FALSE;
  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  for (McIndex = 0; McIndex < MaxImc; ++McIndex) {
    if (ChannelBitmap[McIndex] != 0) {
      Found = TRUE;
      break;
    }
  }
  if (Found == FALSE) {
    return FALSE;
  }

  Found = FALSE;
  MatchingHalfBitmapInternal = 0;
  RouteTableData = GetRouteTableDataForMemType (MemMapData, Socket, MemType);
  McWays = 0;

  //
  // If only one bit is set in the MC bitmap, and only one bit is set in the
  // corresponding channel bitmap, this is single-target.
  //
  SingleTarget = FALSE;
  if ((McBitmap & (McBitmap - 1)) == 0) {
    McIndex = (UINT8) HighBitSet32 ((UINT32) McBitmap);
    if (McIndex >= MaxImc) {
      return FALSE;
    }
    if ((ChannelBitmap[McIndex] & (ChannelBitmap[McIndex] - 1)) == 0) {
      SingleTarget = TRUE;
    }
  }

  //
  // Copy the bitmaps so they can be modified without losing the originals.
  //
  McBitmapCopy = McBitmap;
  RestartSearch = FALSE;

  for (Half = 0; Half < MAX_RT; Half++) {
    if (RouteTableData->RouteTableUsed[Socket][Cluster][Half] == 0) {
      continue;
    }

    //
    // Reset the bitmaps used to keep track of which MCs and halves
    // have been seen. They need to be reset when a route table
    // missmatch is detected to start a new search.
    //
    if (RestartSearch) {
      RestartSearch = FALSE;
      McBitmapCopy = McBitmap;
      MatchingHalfBitmapInternal = 0;
    }

    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      ChannelBitmapCopy[McIndex] = ChannelBitmap[McIndex];
      ChWays[McIndex] = CalculateInterleaveWaysfromBitmap (ChannelBitmapCopy[McIndex]);
    }

    if (SingleTarget) {
      MaxRtIndex = MAX_SINGLE_TGT_IDX;
    } else {
      MaxRtIndex = RouteTableData->MaxRtWays;
    }

    if (RouteTableData->RouteTableUsed[Socket][Cluster][Half] < MaxRtIndex) {
      MaxRtIndex = RouteTableData->RouteTableUsed[Socket][Cluster][Half];
    }

    for (RtIndex = 0; RtIndex < MaxRtIndex; RtIndex++) {
      if (IsMemType2lm (MemType) && MemMapData->Is2LmUsingRt2lm) {
        McIndex = RouteTableData->TgtRouteTable[Socket][Cluster][Half][RtIndex];
        if ((McBitmapCopy & (BIT0 << McIndex)) == 0) {
          continue;
        }
        //
        // In-tile 4K can be Max channel ways within the Tile.
        //
        for (ChIndex = 0; ChIndex < MaxChPerMc; ChIndex++) {
          ChannelIndex = RouteTableData->ChRouteTable[Socket][Cluster][Half][ChIndex];
          if ((ChannelBitmapCopy[McIndex] & (BIT0 << ChannelIndex)) == 0) {
            RestartSearch = TRUE;
            break;
          }
          //
          // Mark off the channel as already seen.
          //
          ChannelBitmapCopy[McIndex] &= ~(BIT0 << ChannelIndex);
          //
          // Some interleaves require more than one route table, one in H0 and one in H1. Keep track of which Halves are
          // used for this interleave.
          //
          if ((ChannelBitmap[McIndex] & (BIT0 << ChannelIndex)) != 0 && (McBitmap & (BIT0 << McIndex)) != 0) {
            MatchingHalfBitmapInternal |= (BIT0 << Half);
          }
        } // ChIndex
      } else {
          McIndex = RouteTableData->TgtRouteTable[Socket][Cluster][Half][RtIndex];
          ChannelIndex = RouteTableData->ChRouteTable[Socket][Cluster][Half][RtIndex];

          if (!SingleTarget) {
            //
            // If this target is not one of the MCs needed, then this route table is
            // not a match.
            //
            if ((McBitmapCopy & (BIT0 << McIndex)) == 0) {
              RestartSearch = TRUE;
              break;
            }

            //
            // If this channel is not one that is needed by this MC, then this route
            // table is not a match
            //
            if ((ChannelBitmapCopy[McIndex] & (BIT0 << ChannelIndex)) == 0) {
              RestartSearch = TRUE;
              break;
          }
        }

        //
        // Some interleaves require more than one route table, one in H0 and one in H1. Keep track of which Halves are
        // used for this interleave.
        //
        if ((ChannelBitmap[McIndex] & (BIT0 << ChannelIndex)) != 0 && (McBitmap & (BIT0 << McIndex)) != 0) {
          MatchingHalfBitmapInternal |= (BIT0 << Half);
        }
      }

      //
      // Mark off the channel as already seen.
      //
      ChannelBitmapCopy[McIndex] &= ~(BIT0 << ChannelIndex);

      //
      // If all the channels have been seen, mark off the MC.
      //
      if (ChannelBitmapCopy[McIndex] == 0) {
        McBitmapCopy &= ~(BIT0 << McIndex);
      }

      //
      // If all the MCs have been seen, this route table is a match.
      //
      if (McBitmapCopy == 0) {
        Found = TRUE;
        break;
      }
    } // RtIndex

    if (Found) {
      if (MatchingHalfBitmap != NULL) {
        *MatchingHalfBitmap = MatchingHalfBitmapInternal;
      }
      if (SingleTargetIndex != NULL) {
        if (SingleTarget) {
          *SingleTargetIndex = RtIndex;
        } else {
          *SingleTargetIndex = 0;
        }
      }
      break;
    }
  } // Half

  return Found;
} // FindRouteTable

/** Get the number of MCs per half.

  Returns a value of one if max number of memory controller per socket is 1. Assumes that the number of MCs per half is symmetric
  and that the total number of MCs is an even number.

  @param[in] McIndex - The MC index to process

  @retval RtHalf0       Number of MCs enabled per half.
**/
UINT8
GetNumMcPerHalf (
  VOID
  )
{
  UINT8   NumMcPerHalf;
  UINT8   MaxImc;

  MaxImc = GetMaxImc ();

  if (MaxImc == 1) {
    return 1;
  }
  NumMcPerHalf = (MaxImc / MaxRtHalves);
  return NumMcPerHalf;
}

/** Get the half (H0 or H1) corresponding to the MC.

  Returns a value of zero if max number of memory controller per socket is less than 2. Assumes that the number of MCs per half is symmetric
  and that the total number of MCs is an even number.

  @param[in] McIndex - The MC index to process

  @retval RtHalf0       The input MC resides in Socket Half H0
  @retval RtHalf1       The input MC resides in Socket Half H1
**/
ROUTE_TABLE_HALVES
GetHalfForMc (
  IN UINT8 McIndex
  )
{
  UINT8   NumMcPerHalf;
  UINT8   MaxImc;

  MaxImc = GetMaxImc ();

  if (MaxImc == 1) {
    return 0;
  }
  NumMcPerHalf = (MaxImc / 2);
  if (NumMcPerHalf == 1) {
    return McIndex;
  }

  if (IsNorthSouthCluster ()) {
    return McIndex % NumMcPerHalf;
  } else {
    return McIndex / NumMcPerHalf;
  }
}

/** Get a McBitmap bitmask for MC's in a given socket half.

  @param[in] Half - The half to process

  @retval 0x3       The bitmask for MC0 and MC1 in RtHalf0
  @retval 0xC       The bitmask for MC2 and MC3 in RtHalf1
**/
UINT8
GetMcBitMaskForHalf (
  IN ROUTE_TABLE_HALVES  Half
  )
{
  UINT8   McBitMask;
  UINT8   McIndex;
  UINT8   MaxImc;

  MaxImc = GetMaxImc ();

  //
  // Calculate the MC Bit mask for the input Half
  //
  McBitMask = 0;
  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if (GetHalfForMc (McIndex) == Half) {
      McBitMask |= (BIT0 << McIndex);
    }
  }

  return McBitMask;
}

/**
  Get a McBitmap bitmask for MC's in a given cluster. Assumes that the ClustersPerSocket is a value
  of "2" or "4".

  @param[in] Cluster           - The cluster number for the requested bitmask
  @param[in] ClustersPerSocket - The number of clusters per socket

  @return The MC bitmask for MC's in the input cluster. A value of "1" is retuned if the maximum
  IMC is 1.
**/
UINT8
GetMcBitMaskForCluster (
  IN UINT8 Cluster,
  IN UINT8 ClustersPerSocket
)
{
  UINT8   MaxImc;

  MaxImc = GetMaxImc ();

  if (MaxImc == 1) {
    return 1;
  } else if (MaxImc == ClustersPerSocket) {
    return (BIT0 << Cluster);
  } else {
    return GetMcBitMaskForHalf (Cluster);
  }
}

/** Find if the input MC Bitmap describes a cross-half MC population.

  @param[in] McBitmap - The MC Bitmap to process

  @retval TRUE        The input McBitmap describes a cross-half MC population
  @retval FALSE       The input McBitmap does not describe a cross-half MC population
**/
BOOLEAN
IsOneMcOnEachSide (
  IN      UINT8               McBitmap
  )
{
  SYSHOST            *Host;
  BOOLEAN            OneMcEachSide;

  Host = GetSysHostPointer ();
  //
  // If there is at least one MC on each side, then return TRUE
  //
  if ((McBitmap & GetMcBitMaskForHalf (RtHalf0)) != 0 && (McBitmap & GetMcBitMaskForHalf (RtHalf1)) != 0) {
    OneMcEachSide = TRUE;
  } else {
    OneMcEachSide = FALSE;
  }

  return OneMcEachSide;
}

/**
  Assign a route table for the given interleave. This function assigns a single Route Table per half after evaluating
  RT supported interleaving. If RT per half is used then the Mc bitmap and McWays correspond to the MCs per half.

  @param[in]      Host            Pointer to the host structure.
  @param[in, out] MemMapData      Pointer to the memory map data structure.
  @param[in]      Socket          Socket index.
  @param[in]      Cluster         Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      RtHalf          Socket RT Half to fill the RT
  @param[in]      MemType         Type of memory being interleaved.
  @param[in]      McWays          Number of MCs to interleave.
  @param[in]      ChannelWays     Number of channels to interleave per MC.
  @param[in]      McBitmap        Bitmap of all MCs in the interleave.
  @param[in]      ChannelBitmap   Array of bitmaps of channels.
  @param[in]      FmChannelBitmap Array of bitmaps of channels of Far Memory type.
  @param[in]      ChannelSortList List of channels in order of size.
  @param[out]     MatchMcToHalf   Specifies that the MC must be assigned a route table in it's corresponding half.
                                  Can be set to TRUE/FASLE in input. The flow checks if the interleave ways is greater
                                  than the selected RT's maximum supported interleave ways. If yes, then the input is
                                  overwritten to match MC's to the corresponding half.

  @retval SUCCESS       A route table was successfully assigned.
  @retval FAILURE       A route table could not be assigned.
**/
UINT32
AssignRouteTable (
  IN      SYSHOST             *Host,
  IN OUT  MEMORY_MAP_DATA     *MemMapData,
  IN      UINT8               Socket,
  IN      UINT8               Cluster,
  IN      ROUTE_TABLE_HALVES  RtHalf,
  IN      MEM_TYPE            MemType,
  IN      UINT8               McWays,
  IN      UINT8               ChannelWays,
  IN      UINT8               McBitmap,
  IN      UINT8               ChannelBitmap[MAX_IMC],
  IN      UINT8               FmChannelBitmap[MAX_IMC],
  IN      CHANNEL_SORT_STRUCT ChannelSortList[MAX_CH],
  OUT     UINT8               *MatchMcToHalf
  )
{
  UINT8              MaxImc;
  UINT8              MaxChPerMc;
  ROUTE_TABLE_HALVES Half;
  UINT8              *RtTarget;
  UINT8              *RtChannel;
  UINT8              McIndex;
  UINT8              ChannelIndex;
  UINT8              RtIndex;
  BOOLEAN            Found;
  UINT8              McBitmapInternal;
  UINT8              *ChannelBitmapInternal;
  ROUTE_TABLE_DATA   *RouteTableData;
  UINT8              ChLid;
  UINT8              McLid;
  UINT8              RtTargetId;
  UINT8              RtChannelId;
  UINT8              ChannelNum;
  BOOLEAN            ZeroSizeChannel;
  UINT8              SortListId;
  UINT8              MaxChDdr;

  if (Host == NULL || MemMapData == NULL || ChannelSortList == NULL) {
    return FAILURE;
  }

  if (MemType == MemType1lmDdr || IsMemType2lm (MemType)) {
    if (ChannelBitmap == NULL) {
      return FAILURE;
    }
    ChannelBitmapInternal = ChannelBitmap;
  } else {
    if (FmChannelBitmap == NULL) {
      return FAILURE;
    }
    ChannelBitmapInternal = FmChannelBitmap;
  }

  MaxImc = GetMaxImc ();
  if (Socket >= MAX_SOCKET ||  McWays > MaxImc || ChannelWays > CH_3WAY || McBitmap == 0) {
    return FAILURE;
  }

  Half = RtHalf0;
  McBitmapInternal = McBitmap;
  MaxChPerMc = GetNumChannelPerMc ();
  RouteTableData = GetRouteTableDataForMemType (MemMapData, Socket, MemType);
  MaxChDdr = GetMaxChDdr ();

  //
  // Try to find a matching route table that was already created.
  //
  Found = FindRouteTable (
            MemMapData,
            Socket,
            MemType,
            Cluster,
            McBitmap,
            ChannelBitmapInternal,
            NULL,
            NULL
            );
  if (Found) {
    return SUCCESS;
  }

  //
  // By default, we will program RTs from index 0. If single target, we
  // might need to use a different index.
  //
  RtIndex = 0;

  //
  // Default to RT instance only in one half (H0)
  //
  *MatchMcToHalf = FALSE;

  //
  // In-tile 2LM is the only instance where two different RT's are used in different granularities.
  // Channel ways can only be upto MAX_MC_CH and target ways is max. DDRT RT ways.
  // 6-way across 4 MCs is the only POR where the RT logic uses mod3 in each half.
  //
  GetRouteTableForMemType (MemMapData, MemType, &RtTargetId, &RtChannelId);
  if (RtTargetId == RtChannelId) {
    if ((McWays * ChannelWays) >  RouteTableData->MaxRtWays) {
      *MatchMcToHalf =  TRUE;
    } else if (ChannelWays == CH_3WAY && (CalculateNumberOfHalvesFromMcBitmap (McBitmapInternal) > 1)) {
      *MatchMcToHalf = TRUE;
    }
  }

  if (*MatchMcToHalf) {
    //
    // Each RT will hold only half of the MCs for this interleave
    //
    McBitmapInternal = McBitmap & GetMcBitMaskForHalf (RtHalf); // Mask out MCs in the other half
    //
    // For mod3 interleave the McWays can only be max one_way per half.
    // Use MC bitmap per half but the input McWays.
    //
    if (CalculateInterleaveWaysfromBitmap (McBitmapInternal) < McWays) {
      McWays = CalculateInterleaveWaysfromBitmap (McBitmapInternal); // Each RT will hold half of the MCs for this interleave
    }
  }

  //
  // For single target, see if there is an unused route table index in a
  // used route table.
  //
  RtTarget = RouteTableData->TgtRouteTable[Socket][Cluster][Half];
  RtChannel = RouteTableData->ChRouteTable[Socket][Cluster][Half];
  if ((McWays * ChannelWays) == 1) {
    for (Half = 0; Half < MAX_RT; Half++) {
      if (RouteTableData->RouteTableUsed[Socket][Cluster][Half] == 0) {
        continue;
      }
      if (*MatchMcToHalf) {
        McIndex = (UINT8) LowBitSet32 (McBitmapInternal);
        if (Half != GetHalfForMc (McIndex)) {
          //
          // Skip Route Tables that are not in the same half as the input MCs
          //
          continue;
        }
      }

      if (RouteTableData->RouteTableUsed[Socket][Cluster][Half] < MAX_SINGLE_TGT_IDX) {
        RtIndex = RouteTableData->RouteTableUsed[Socket][Cluster][Half];
        RtTarget = RouteTableData->TgtRouteTable[Socket][Cluster][Half];
        RtChannel = RouteTableData->ChRouteTable[Socket][Cluster][Half];
        RouteTableData->RouteTableUsed[Socket][Cluster][Half]++;
        Found = TRUE;
        break;
      }
    }
  }

  //
  // Find an unused route table.
  //
  if (!Found) {
    for (Half = 0; Half < MAX_RT; Half++) {
      if (RouteTableData->RouteTableUsed[Socket][Cluster][Half] != 0) {
        continue;
      }
      if (*MatchMcToHalf) {
        McIndex = (UINT8) LowBitSet32 (McBitmapInternal);
        if (Half != GetHalfForMc (McIndex)) {
          //
          // Skip Route Tables that are not in the same half as the input MCs
          //
          continue;
        }
      }

      RtTarget = RouteTableData->TgtRouteTable[Socket][Cluster][Half];
      RtChannel = RouteTableData->ChRouteTable[Socket][Cluster][Half];
      RouteTableData->RouteTableUsed[Socket][Cluster][Half] = McWays * ChannelWays;
      if (RouteTableData->RouteTableUsed[Socket][Cluster][Half] > RouteTableData->MaxRtWays) {
        RouteTableData->RouteTableUsed[Socket][Cluster][Half] = RouteTableData->MaxRtWays;
      }
      Found = TRUE;
      break;
    }
  }

  //
  // If all route tables are already used, we can't map the interleave.
  //
  if (!Found) {
    return FAILURE;
  }

  //
  // For Wave3, If H1 is used we should disable UMA based clustering
  //
  if ((Half != RtHalf0) && IsUbcSupportedMemType (MemType)) {
    MemMapData->UmaBasedClustersPerSocket = UMABASEDCLUSTERING_DISABLED;
  }

  //
  // Add all the MC channels populated to the route table.
  // For 1LM POR configurations, 2LM and PMEM memory modes symmetric population enforcement is required.
  //
  if ((MemMapData->IsPorConfig) || (MemType == MemType1lmAppDirect)) {
    if (IsMemType2lm (MemType) && MemMapData->Is2LmUsingRt2lm) {
      //
      // For 2LM in-tile, the route tables are populated using the near memory DDR. Since the near memory is required to be
      // the same on all channels, we can assign them to the route table without sorting them.
      //
      for (McIndex = 0, McLid = 0, ChLid = 0; McIndex < MaxImc; McIndex++) {
        if ((McBitmapInternal & (BIT0 << McIndex)) == 0) {
          continue;
        }

        if (McLid > RouteTableData->RouteTableUsed[Socket][Cluster][Half] ||
          ChLid > RouteTableData->RouteTableUsed[Socket][Cluster][Half]) {
          break;
        }

        RtTarget[RtIndex + McLid] = McIndex;
        McLid++;

        //
        // In-tile 4K can be Max channel ways within the Tile.
        //
        for (ChannelIndex = 0; (ChannelIndex < MaxChPerMc) && (ChLid < ChannelWays); ChannelIndex++) {
          if ((ChannelBitmapInternal[McIndex] & (BIT0 << ChannelIndex)) == 0) {
            continue;
          }
          RtChannel[RtIndex + ChLid] = ChannelIndex;
          ChLid++;
        } // ChannelIndex
      } // McIndex
    } else {
      for (ChannelIndex = 0; ChannelIndex < MaxChPerMc; ChannelIndex++) {
        for (McIndex = 0; McIndex < MaxImc; McIndex++) {
          if ((McBitmapInternal & (BIT0 << McIndex)) == 0) {
            continue;
          }

          ZeroSizeChannel = FALSE;
          if (IsMemType2lm (MemType)) {
            //
            // For 2LM (256B FM channel granularity) we can directly use DDR near memory without sorting them.
            //
            ChannelNum = ChannelIndex;
          } else {
            //
            // For 1LM and AppDirect memory, populate the Route Table using the sorted channel list.
            //
            ChannelNum = ChannelSortList[(McIndex * MaxChPerMc) + ChannelIndex].chNum;
            if (ChannelSortList[(McIndex * MaxChPerMc) + ChannelIndex].chSize == 0) {
              ZeroSizeChannel = TRUE;
            }
          }

          if (ZeroSizeChannel || (ChannelBitmapInternal[McIndex] & (BIT0 << ChannelNum)) == 0) {
            continue;
          }

          RtTarget[RtIndex] = McIndex;
          RtChannel[RtIndex] = ChannelNum;
          RtIndex++;
        }
      }
    }
  } else {
    //
    // For Non-POR configurations populate the RT per half
    // The sorted channel capacity is applicable only to 1LM memory modes
    //
    for (SortListId = 0; SortListId < MaxChDdr; SortListId++) {
      if (ChannelSortList[SortListId].chSize == 0) {
        continue;
      }
      McIndex = (ChannelSortList[SortListId].chNum / MaxChPerMc);
      ChannelIndex = (ChannelSortList[SortListId].chNum % MaxChPerMc);
      RtTarget[RtIndex] = McIndex;
      RtChannel[RtIndex] = ChannelIndex;
      RtIndex++;
    }
  }

  //
  // Copy the internal Route Table data to the socket data structure to make it visible to the CR Management Driver.
  //
  if (MemType == MemType1lmAppDirect) {
    for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
      for (RtIndex = 0; RtIndex < MemMapData->DdrtRouteTableData.MaxRtWays; RtIndex++) {
        Host->var.mem.socket[Socket].DdrtChRouteTable[Half][RtIndex] = RouteTableData->ChRouteTable[Socket][Cluster][Half][RtIndex];
        Host->var.mem.socket[Socket].DdrtTgtRouteTable[Half][RtIndex] = RouteTableData->TgtRouteTable[Socket][Cluster][Half][RtIndex];
      }
    }
  }

  return SUCCESS;
} // AssignRouteTable

/**
  Get the cluster number where the input MC resides.
  This routine assumes that the input UmaClustersPerSocket is either "2" or "4"

  @param[in] Mc                   - The MC to look up
  @param[in] UmaClustersPerSocket - The number of UMA-based clusters per socket

  @return The cluster where the input MC resides. Returns a value of "1" if the maximum number
  of MCs is 1.
**/
UINT8
GetUmaBasedClusterForMc (
  IN UINT8 Mc,
  IN UINT8 UmaClustersPerSocket
)
{
  UINT8   MaxImc;

  MaxImc = GetMaxImc ();

  if (MaxImc == 1) {
    return 1;
  } else if (MaxImc == UmaClustersPerSocket) {
    return Mc;
  } else {
    return GetHalfForMc (Mc);
  }
}

/**
  Fill in route table data for DDR as far memory.

  @param[in]      Host            Pointer to the host structure.
  @param[in, out] MemMapData      Pointer to the memory map data structure.
  @param[in]      Socket          Socket index.
  @param[in]      Cluster         Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType         Type of memory being interleaved.
  @param[in]      McWays          Number of MCs to interleave.
  @param[in]      ChannelWays     Number of channels to interleave per MC.
  @param[in]      McBitmap        Bitmap of all MCs in the interleave.
  @param[in]      FmChannelBitmap Array of bitmaps of channels of Far Memory type.

  @retval SUCCESS       A route table was successfully assigned.
  @retval FAILURE       A route table could not be assigned.
**/
UINT32
FarDdrPhysicalToLogical (
  IN      SYSHOST             *Host,
  IN OUT  MEMORY_MAP_DATA     *MemMapData,
  IN      UINT8               Socket,
  IN      UINT8               Cluster,
  IN      MEM_TYPE            MemType,
  IN      UINT8               McWays,
  IN      UINT8               ChannelWays,
  IN      UINT8               McBitmap,
  IN      UINT8               FmChannelBitmap[MAX_IMC]
  )
{
  UINT8               MaxImc;
  UINT8               MaxChPerMc;
  UINT8               *RtTarget;
  UINT8               *RtChannel;
  UINT8               McIndex;
  UINT8               ChannelIndex;
  UINT8               RtIndex;
  BOOLEAN             UmaBasedClustering;
  ROUTE_TABLE_DATA    *DdrFarRtData;
  UINT8               DestinationCluster;

  if (Host == NULL || MemMapData == NULL) {
    return FAILURE;
  }

  if (MemType != MemType2lmHbmCacheDdr) {
      return FAILURE;
    }

  MaxImc = GetMaxImc ();
  if (Socket >= MAX_SOCKET ||  McWays > MaxImc || ChannelWays > CH_3WAY || McBitmap == 0) {
    return FAILURE;
  }

  if ((McWays * ChannelWays) > MAX_RT_WAYS) {
    return FAILURE;
  }

  MaxChPerMc = GetNumChannelPerMc ();

  DdrFarRtData = &(MemMapData->FarDdrRouteTableData);
  DdrFarRtData->MaxRtWays = MAX_RT_WAYS;

  UmaBasedClustering = (MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED);
  DestinationCluster = Cluster;

  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if ((McBitmap & (BIT0 << McIndex)) == 0) {
      continue;
    }
    RtIndex = 0;

    if (UmaBasedClustering) {
      DestinationCluster = GetUmaBasedClusterForMc (McIndex, MemMapData->UmaBasedClustersPerSocket);
    }

    DdrFarRtData->RouteTableUsed[Socket][DestinationCluster][RtHalf0] = McWays * ChannelWays;

    RtTarget = DdrFarRtData->TgtRouteTable[Socket][DestinationCluster][RtHalf0];
    RtChannel = DdrFarRtData->ChRouteTable[Socket][DestinationCluster][RtHalf0];

    for (ChannelIndex = 0; ChannelIndex < MaxChPerMc; ChannelIndex++) {
      if ((FmChannelBitmap[McIndex] & (BIT0 << ChannelIndex)) == 0) {
        continue;
      }

      RtTarget[RtIndex] = McIndex;
      RtChannel[RtIndex] = ChannelIndex;
      RtIndex++;
    }
  }

  return SUCCESS;
}

/**
  Check if interleaving is possible for many (>2) MCs.

  @param[in]      Host              Pointer to the host structure.
  @param[in]      MemMapData        Pointer to the memory map data structure.
  @param[in, out] SocketBitmap      Bitmap of sockets requested/possible for interleave.
  @param[in]      Cluster           Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType           Type of memory being interleaved.
  @param[in]      McWays            Number of MCs to interleave.
  @param[in]      ChannelWays       Number of channels to interleave per MC.
  @param[out]     McBitmap          Bitmap of MCs in the interleave.
  @param[out]     NmMcBitmap        Bitmap of MCs in the ineterleave as near memory.
  @param[out]     ChannelBitmap     Array of bitmaps of channels.
  @param[out]     FmChannelBitmap   Array of bitmaps of channels of Far Memory type.
  @param[out]     ChannelSortList   Channels per MC in order of decreasing size.
  @param[in]      AllowAcrossHalves Specify if the interleave can be checked within or across half.

  @retval TRUE          The given interleave is possible.
  @retval FALSE         The given interleave is impossible.
**/
BOOLEAN
IsXorInterleavePossible (
  IN  SYSHOST             *Host,
  IN  MEMORY_MAP_DATA     *MemMapData,
  IN OUT UINT8            *SocketBitmap,
  IN  UINT8               Cluster,
  IN  MEM_TYPE            MemType,
  IN  UINT8               McWays,
  IN  UINT8               ChannelWays,
  OUT UINT8               McBitmap[MAX_SOCKET],
  OUT UINT8               NmMcBitmap[MAX_SOCKET],
  OUT UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC],
  OUT UINT8               FmChannelBitmap[MAX_SOCKET][MAX_IMC],
  OUT CHANNEL_SORT_STRUCT ChannelSortList[MAX_SOCKET][MAX_CH],
  IN  BOOLEAN             AllowAcrossHalves
  )
{
  BOOLEAN             Possible;
  BOOLEAN             SncEnabled;
  UINT8               NumberOfClusters;
  UINT8               MaxImc;
  UINT8               MaxChPerMc;
  UINT8               McsPerCluster;
  UINT8               McIndex;
  UINT8               SocketIndex;
  UINT8               BeginSktIndex;
  UINT8               EndSktIndex;
  UINT8               ChannelIndex;
  UINT8               NumberOfMcs;
  UINT8               NumberOfChannels;
  UINT8               PrevSocketInterleaved;
  UINT8               SocketWays;
  UINT8               SocketBitmapInternal;
  UINT8               TargetWays;
  UINT8               ChannelBitmapDiscovered[MAX_IMC];
  UINT8               McBitmapInternal[MAX_SOCKET];
  UINT8               NmMcBitmapInternal[MAX_SOCKET];
  UINT8               ChannelBitmapInternal[MAX_SOCKET][MAX_IMC];
  UINT8               NmChannelBitmapInternal[MAX_SOCKET][MAX_IMC];
  UINT8               *ChannelBitmapToCheck;
  UINT8               NmNumberOfChannels[MAX_SOCKET][MAX_IMC];
  UINT8               NumberOfChannelsOnMc[MAX_SOCKET][MAX_IMC];
  CHANNEL_SORT_STRUCT SortList[MAX_SOCKET][MAX_CH];
  ROUTE_TABLE_HALVES  Half;
  ROUTE_TABLE_DATA    *RouteTableData;
  UINT8               FirstMcInHalf;
  UINT8               PreSocketMcBitMap;
  UINT8               FirstMcInInterleave;
  UINT8               InterleaveWays;
  UINT8               SortListId;
  UINT32              SymmetryCheckStatus;

  if (Host == NULL || MemMapData == NULL || SocketBitmap == NULL) {
    return FALSE;
  }

  if (McBitmap == NULL || NmMcBitmap == NULL || ChannelBitmap == NULL || FmChannelBitmap == NULL ||
      ChannelSortList == NULL) {
    return FALSE;
  }

  if (Cluster >= MAX_CLUSTERS) {
    return FALSE;
  }

  MaxImc = GetMaxImc ();
  if (McWays == 0 || McWays > MaxImc) {
    return FALSE;
  }

  if (ChannelWays == 0 || ChannelWays > Host->var.mem.chInter || ChannelWays == CH_3WAY) {
    return FALSE;
  }

  //
  // For NUMA iterate only 1-way intra socket.
  // For UMA iterate through all enabled sockets.
  //
  BeginSktIndex = (UINT8) LowBitSet32 ((UINT32) *SocketBitmap);
  EndSktIndex = (UINT8) HighBitSet32 ((UINT32) *SocketBitmap);

  if (BeginSktIndex >= MAX_SOCKET || EndSktIndex >= MAX_SOCKET) {
    return FALSE;
  }

  if (MemType == MemType1lmAppDirect) {
    //
    // Use the standard interleaving algorithm for AppDirect Memory.
    //
    SncEnabled = FALSE;
  } else {
    SncEnabled = IfFeatureEnabled (TypeSnc);
  }

  if (SncEnabled) {
    NumberOfClusters = GetNumOfClusterPerSystem ();
    if (NumberOfClusters == 0 || NumberOfClusters > MAX_CLUSTERS ||
        Cluster >= NumberOfClusters) {
      return FALSE;
    }

    McsPerCluster = MaxImc / NumberOfClusters;
    if (McsPerCluster == 0 || McWays > McsPerCluster) {
      return FALSE;
    }
  }

  Possible = FALSE;
  NumberOfMcs = 0;
  SocketWays = 0;
  SocketBitmapInternal = 0;
  TargetWays = 0;
  MaxChPerMc = GetNumChannelPerMc ();
  ZeroMem (McBitmapInternal, sizeof (McBitmapInternal));
  ZeroMem (NmMcBitmapInternal, sizeof (NmMcBitmapInternal));
  ZeroMem (ChannelBitmapInternal, sizeof (ChannelBitmapInternal));
  ZeroMem (NumberOfChannelsOnMc, sizeof (NumberOfChannelsOnMc));
  ZeroMem (SortList, sizeof (SortList));
  ZeroMem (NmChannelBitmapInternal, sizeof (NmChannelBitmapInternal));
  ZeroMem (NmNumberOfChannels, sizeof (NmNumberOfChannels));
  ZeroMem (ChannelBitmapDiscovered, sizeof (ChannelBitmapDiscovered));

  PrevSocketInterleaved = BeginSktIndex;

  for (SocketIndex = BeginSktIndex; SocketIndex <= EndSktIndex; SocketIndex++) {
    NumberOfMcs = 0;

    if (CheckSocketEnabled (SocketIndex) == 0 ||
        GetSocketMemSize (SocketIndex) == 0) {
      continue;
    }

    //
    //  Interleave for POR configurations requires channel size symmetry across halves or MC's
    //
    for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
      FirstMcInHalf = (UINT8) LowBitSet32 (GetMcBitMaskForHalf (Half));
      for (McIndex = 0; McIndex < MaxImc; McIndex++) {
        //
        // Loop MC based on half and filtered by cluster in SNC mode
        //
        if ((GetHalfForMc (McIndex) != Half) || (SncEnabled && GetClusterForMc (MemMapData, McIndex) != Cluster)) {
          continue;
        }

        //
        // Clear MC and channel data if one half failed and before trying next half when interleave
        // is restricted within a half
        //
        if (!AllowAcrossHalves && McIndex > 0 && McIndex == FirstMcInHalf) {
          NumberOfMcs = 0;
          ZeroMem (ChannelBitmapInternal[SocketIndex], sizeof (ChannelBitmapInternal[SocketIndex]));
          ZeroMem (NumberOfChannelsOnMc, sizeof (NumberOfChannelsOnMc));
          ZeroMem (SortList[SocketIndex], sizeof (SortList[SocketIndex]));
          McBitmapInternal[SocketIndex] = 0;
        }

        //
        // Sort channels within MC by remaining size.
        //
        ChannelSortMc (
          Host,
          SocketIndex,
          (McIndex * MaxChPerMc),
          SortList[SocketIndex]
          );

        NumberOfChannels = 0;
        ChannelBitmapDiscovered[McIndex] = 0;
        for (ChannelIndex = 0; ChannelIndex < MaxChPerMc; ChannelIndex++) {
          if (SortList[SocketIndex][(McIndex * MaxChPerMc) + ChannelIndex].chSize != 0) {
            //
            // Only mark bits for the number of ways, but count every channel.
            //
            if (NumberOfChannels < ChannelWays) {
              ChannelBitmapDiscovered[McIndex] |= (BIT0 << SortList[SocketIndex][(McIndex * MaxChPerMc) + ChannelIndex].chNum);
            }
            NumberOfChannels++;
          }
        }

        NumberOfChannelsOnMc[SocketIndex][McIndex] = NumberOfChannels;

        //
        // If the number of channels populated in an MC is not >= requested channel ways then check next MC
        //
        if (NumberOfChannels < ChannelWays) {
          continue;
        }

        McBitmapInternal[SocketIndex] |= (BIT0 << McIndex);
        ChannelBitmapInternal[SocketIndex][McIndex] = ChannelBitmapDiscovered[McIndex];
        NumberOfMcs++;

        //
        // If the number of MCs populated with requested channel ways is not >= requested MC ways then continue to
        // next MC till we count Max MCs
        //
        if (NumberOfMcs < McWays) {
          continue;
        }

        Possible = TRUE;
        FirstMcInInterleave = (UINT8) LowBitSet32 (McBitmapInternal[SocketIndex]);

        //
        // Where an interleave request is to allow across half and there is symmetry within half,
        // look for symmetry across half first.
        //
        if (AllowAcrossHalves && !IsOneMcOnEachSide (McBitmapInternal[SocketIndex])) {
          if (McBitmapInternal[SocketIndex] != 0) {
            McBitmapInternal[SocketIndex] &= ~(BIT0 << (UINT8) HighBitSet32 (McBitmapInternal[SocketIndex]));
          }
          ChannelBitmapInternal[SocketIndex][McIndex] = 0;
          Possible = FALSE;
          break;
        }

        if (Possible) {
          Half = MaxRtHalves; // To break half loop
          break;
        }
      } // McIndex
    } // RtHalf

    //
    // If no adequate interleavable MC are detected, return.
    //
    if (!Possible) {
      return FALSE;
    }

    //
    // Check if the candidate interleave sets satisfy wave specific symmetry requirement
    //
    SymmetryCheckStatus = CheckXorInterleaveSymmetryRequirement (Host, MemMapData, *SocketBitmap, Cluster, MemType,
                                               McBitmapInternal, ChannelBitmapInternal,
                                               McWays, ChannelWays);

    if (SymmetryCheckStatus == SUCCESS) {
      //
      // POR interleave would be marked and routed according to POR indexing flow
      //
      Possible = TRUE;
      MemMapData->IsPorConfig = TRUE;
    } else if (SymmetryCheckStatus == RETRY) {
      //
      // If POR symmetric memory configuration failed then below block executes.
      // Asymmetric population can be allowed only Non-POR configurations that restrict interleave within a half.
      // Interleave across halves (> 1MC) requires channel size symmetry in quad or hemi.
      // i.e.
      // POR evaluation failure for across-half candidate will return and retry with smaller interleave width later.
      // POR evaluation failure for within-half candidate will be handled in asymmetric interleave flow
      // Note: we have IMC_1WAY to be fulflled by POR indexing flow.
      //
      Possible = FALSE;
      MemMapData->IsPorConfig = FALSE;

      for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
        FirstMcInHalf = (UINT8) LowBitSet32 (GetMcBitMaskForHalf (Half));
        //
        // Loop MC based on half and filtered by cluster in SNC mode
        //
        if (SncEnabled && (GetClusterForMc (MemMapData, FirstMcInHalf) != Cluster)) {
          continue;
        }

        //
        // Clear MC and channel data if one half failed and before trying next half
        //
        NumberOfMcs = 0;
        ZeroMem (ChannelBitmapInternal[SocketIndex], sizeof (ChannelBitmapInternal[SocketIndex]));
        ZeroMem (NumberOfChannelsOnMc, sizeof (NumberOfChannelsOnMc));
        ZeroMem (SortList[SocketIndex], sizeof (SortList[SocketIndex]));
        McBitmapInternal[SocketIndex] = 0;

        ChannelSortHalf (
          Host,
          SocketIndex,
          Half,
          (McWays * ChannelWays),
          SortList[SocketIndex]
          );

        //
        // First find the MCs and channels that can be interleaved based on the sort list
        //
        for (InterleaveWays = 0; InterleaveWays < (McWays * ChannelWays); InterleaveWays++) {
          SortListId = (FirstMcInHalf * MaxChPerMc) + InterleaveWays;
          if (SortList[SocketIndex][SortListId].chSize != 0) {
            McIndex = (SortList[SocketIndex][SortListId].chNum / MaxChPerMc);
            ChannelIndex = (SortList[SocketIndex][SortListId].chNum % MaxChPerMc);
            ChannelBitmapDiscovered[McIndex] |= (BIT0 << ChannelIndex);
            NumberOfChannelsOnMc[SocketIndex][McIndex]++;
            if (NumberOfChannelsOnMc[SocketIndex][McIndex] == ChannelWays) {
              McBitmapInternal[SocketIndex] |= (BIT0 << McIndex);
              ChannelBitmapInternal[SocketIndex][McIndex] = ChannelBitmapDiscovered[McIndex];
            }
          }
          if (CalculateInterleaveWaysfromBitmap (McBitmapInternal[SocketIndex]) >= McWays) {
            Possible = TRUE;
            MemMapData->IsPorConfig = FALSE;
            Half = MaxRtHalves; // break half loop
            break;
          }
        }
      } // Half
    } else if (SymmetryCheckStatus == FAILURE) {
      //
      // Fail out for current interleave configuration, need to try smaller interleave later.
      //
      Possible = FALSE;
      MemMapData->IsPorConfig = FALSE;
    } else {
      //
      // SymmetryCheckStatus == INVALID_PARAMTER, return FALSE
      //
      return FALSE;
    }

    //
    // If it still looks possible, check the NM population for 2LM configurations. Also populate
    // the NMChannelBitmap.
    //
    if (Possible && IsMemType2lmDdr (MemType)) {
      //
      // Find the number of channels with ddr4 memory populated and their bit map in each iMC
      //
      for (McIndex = 0; McIndex < MaxImc; McIndex++) {
        if (MemMapData->MemMapSocket[SocketIndex].XTile2lm) {
          if (SncEnabled && (GetClusterForMc (MemMapData, McIndex) != Cluster) ||
            !SncEnabled && ((BIT0 << MemMapData->MemMapSocket[SocketIndex].XTile2lmFmTarget[McIndex]) & McBitmapInternal[SocketIndex]) == 0) {
            //
            // In SNC mode, we require all NMC involved should be in the same cluster of FMC and each SNC has only one SAD interleave.
            // In non-SNC mode, we require the NMC whose corresponding FMC should be involved in the interleave. This can address x1
            // interleave issue when we have two SAD entries.
            //
            continue;
          }
        } else {
          if ((McBitmapInternal[SocketIndex] & (BIT0 << McIndex)) == 0) {
            continue;
          }
        }

        NmChannelBitmapInternal[SocketIndex][McIndex] = FindNmBitMap (SocketIndex, McIndex);
        if (NmChannelBitmapInternal[SocketIndex][McIndex] != 0) {
          NmMcBitmapInternal[SocketIndex] |= BIT0 << McIndex;
        }
        for (ChannelIndex = 0; ChannelIndex < MaxChPerMc; ChannelIndex++) {
          if ((NmChannelBitmapInternal[SocketIndex][McIndex] & (BIT0 << ChannelIndex)) != 0) {
            NmNumberOfChannels[SocketIndex][McIndex]++;
          }
        }
        //
        // Each MC with Far Memory must also have Near Memory for In-tile configuration
        //
        if (!MemMapData->MemMapSocket[SocketIndex].XTile2lm && (McBitmapInternal[SocketIndex] & (BIT0 << McIndex)) != 0) {
          if (NmNumberOfChannels[SocketIndex][McIndex] == 0) {
            Possible = FALSE;
          }
        }
      }

      if (Possible && McWays > IMC_1WAY) {
        //
        // Using the lowest MC as reference, check that all MCs for this interleave have the same number
        // of NM Channels populated. This check is only necessary if the interleave involves more than one MC.
        //
        for (McIndex = 0; McIndex < MaxImc; McIndex++) {
          if ((NmMcBitmapInternal[SocketIndex] & (BIT0 << McIndex)) == 0) {
            continue;
          }
          PreSocketMcBitMap = (UINT8) LowBitSet32 ((UINT32) McBitmapInternal[PrevSocketInterleaved]);
          if (PreSocketMcBitMap >= MaxImc || (NmNumberOfChannels[PrevSocketInterleaved][PreSocketMcBitMap] !=
              NmNumberOfChannels[SocketIndex][McIndex])) {
            Possible = FALSE;
          }
        }
      }
#ifdef DDRT_SUPPORT
      //
      // For X-Tile 2LM, we have a special case to support mod3 near memory
      // interleave.
      //
      if (!Possible && MemMapData->MemMapSocket[SocketIndex].XTile2lm &&
        CheckMod3XTile2lmPorConfig (
          McBitmapInternal[SocketIndex],
          ChannelBitmapInternal[SocketIndex],
          NmMcBitmapInternal[SocketIndex],
          NmChannelBitmapInternal[SocketIndex])) {
        Possible = TRUE;
        DisableXorModeForMod3 (MemMapData, SocketIndex);
      }
#endif // DDRT_SUPPORT
    }

    //
    // If it still looks possible, check the route tables.
    //
    if (Possible && !IsMemTypeHbm (MemType)) {

      if (IsMemType2lm (MemType)) {
        ChannelBitmapToCheck = NmChannelBitmapInternal[SocketIndex];
      } else {
        ChannelBitmapToCheck = ChannelBitmapInternal[SocketIndex];
      }

      RouteTableData = GetRouteTableDataForMemType (MemMapData, SocketIndex, MemType);

      if (RouteTableData->RouteTableUsed[SocketIndex][Cluster][RtHalf0] != 0 &&
          RouteTableData->RouteTableUsed[SocketIndex][Cluster][RtHalf1] != 0) {
          Possible = FindRouteTable (
                       MemMapData,
                       SocketIndex,
                       MemType,
                       Cluster,
                       McBitmapInternal[SocketIndex],
                       ChannelBitmapToCheck,
                       NULL,
                       NULL
                       );

        //
        // If this is single target, check if either one of the route tables hasn't used four indexes yet.
        //
        if (!Possible && (McWays * ChannelWays) == 1) {
          if (RouteTableData->RouteTableUsed[SocketIndex][Cluster][RtHalf0] < MAX_SINGLE_TGT_IDX ||
              RouteTableData->RouteTableUsed[SocketIndex][Cluster][RtHalf1] < MAX_SINGLE_TGT_IDX) {
            Possible = TRUE;
          }
        }
      }
    }

    if (Possible) {
      SocketWays++;
      PrevSocketInterleaved = SocketIndex;
      SocketBitmapInternal |= (BIT0 << SocketIndex);
    } else { // clear internal data for this socket if interleave is not possible
      ZeroMem (&McBitmapInternal[SocketIndex], sizeof (McBitmapInternal[SocketIndex]));
      ZeroMem (&ChannelBitmapInternal[SocketIndex], sizeof (ChannelBitmapInternal[SocketIndex]));
      ZeroMem (&SortList[SocketIndex], sizeof (SortList[SocketIndex]));
    }
  } // SocketIndex

  //
  // Check socket interleave ways for UMA
  // If interleave is possible only within a socket and the input bitmap allows to check all enabled sockets
  // then we fail UMA interleaving of memory.
  //
  if (SocketWays == ONE_WAY && CalculateInterleaveWaysfromBitmap (*SocketBitmap) > ONE_WAY) {
    Possible = FALSE;
  }

  //
  // If the interleave is possible, return the MC and channel bitmaps.
  //
  if (Possible) {
    CopyMem (
      McBitmap,
      McBitmapInternal,
      sizeof (McBitmapInternal)
      );
    if (MemType == MemType1lmDdr) {
      CopyMem (
        ChannelBitmap,
        ChannelBitmapInternal,
        sizeof (ChannelBitmapInternal)
        );
    } else {
      CopyMem (
        FmChannelBitmap,
        ChannelBitmapInternal,
        sizeof (ChannelBitmapInternal)
        );
      if (!IsMemTypeHbm (MemType)) {
        CopyMem (
          ChannelBitmap,
          NmChannelBitmapInternal,
          sizeof (NmChannelBitmapInternal)
          );
        CopyMem (
          NmMcBitmap,
          NmMcBitmapInternal,
          sizeof (NmMcBitmapInternal)
          );
      }
    }
    CopyMem (
      ChannelSortList,
      SortList,
      sizeof (SortList)
      );

    *SocketBitmap = SocketBitmapInternal;
  }

  return Possible;
} // IsXorInterleavePossible

/**
  Get the Near Memory Mc Ways and Channel Ways

  @param[in]      NmMcBitmap            The McBitmap for the interleave.
  @param[in]      NmChBitmap            The Near Memory Channel Bitmap for the interleave.
  @param[out]     NmMcWays              Pointer to the variable to update with the Near Memory MC Ways.
  @param[out]     NmChannelWays         Pointer to the variable to update with the Near Memory Channel Ways.
**/
VOID
GetNearMemoryWays (
  UINT8 NmMcBitmap,
  UINT8 NmChBitmap[MAX_IMC],
  UINT8 *NmMcWays,
  UINT8 *NmChannelWays
  )
{
  UINT8   McIndex;
  UINT8   ChIndex;
  UINT8   McWays;
  UINT8   MaxImc;
  UINT8   MaxChPerMc;
  UINT8   TotalChannelWays;

  McWays = 0;
  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();
  TotalChannelWays = 0;

  if (NmMcWays != NULL && NmChannelWays != NULL) {
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if ((NmMcBitmap & (BIT0 << McIndex)) != 0) {
        McWays++;
        for (ChIndex = 0; ChIndex < MaxChPerMc; ChIndex++) {
          if ((NmChBitmap[McIndex] & (BIT0 << ChIndex)) != 0) {
            TotalChannelWays++;
          }
        }
      }
    }
    //
    // Make sure we don't try to divide by zero
    //
    if (McWays == 0 || TotalChannelWays == 0) {
      (*NmMcWays) = 0;
      (*NmChannelWays) = 0;
    } else {
      if ((TotalChannelWays % CH_3WAY) == 0) {
        (*NmChannelWays) = CH_3WAY;
        (*NmMcWays) = ONE_WAY;
      } else {
        (*NmMcWays) = McWays;
        (*NmChannelWays) = (TotalChannelWays / McWays);
      }
    }
  }
}

/** Create remote SAD entry.

  Add a SAD entry in all sockets that are not involved in the SAD interleave.

  @param[in, out] Host       Pointer to sysHost
  @param[in]      MemMapData Pointer to the memory map private data structure.
  @param[in]      McBitmap   Map of all the IMCs that are part of the interleave.
  @param[in]      NodeLimit  Limit value for the SAD entry
  @param[in]      MemType    Type of memory being interleaved.

  @retval  SUCCESS                             -  Remote SAD entry created for every other socket.
  @retval  ERROR_RESOURCE_CALCULATION_FAILURE  -  SAD rules exceed maximum count.

**/
UINT32
AddUmaRemoteSadEntry (
  IN OUT SYSHOST          *Host,
  IN     MEMORY_MAP_DATA  *MemMapData,
  IN     UINT8            McBitmap[MAX_SOCKET],
  IN     UINT32           NodeLimit,
  IN     MEM_TYPE         MemType
  )
{
  MEMORY_MAP_HOST     *MemMapHost;
  SOCKET_INFO_STRUCT  *Socket;
  UINT8               SocketIndex;
  UINT8               SadIndex;
  UINT8               Index;
  UINT8               SocketsInInterleaveList;
  BOOLEAN             SadMerged;
  UINT8               McIndex;
  BOOLEAN             Merge;
  UINT16              SocketFlag;
  UINT16              MapFlag;
  UINT8               MaxImc;

  MaxImc = GetMaxImc ();
  MemMapHost = GetMemMapHostPointer ();

  //
  // Create remote SAD entries for the other sockets
  //
  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    MapFlag = 0;
    SocketFlag = 0;
    SocketsInInterleaveList = 0;
    SadMerged = 0;
    Merge = FALSE;

    if (CheckSocketEnabled (SocketIndex) == 0) {
      continue;
    }

    Socket = &Host->var.mem.socket[SocketIndex];

    //
    // If any MC participating in the interleave, then skip this socket
    //
    if (McBitmap[SocketIndex] != 0) {
      continue;
    }

    //
    // Get index of next available SAD entry.
    //
    SadIndex = FindSadIndex (
                 Host,
                 MemMapData,
                 SocketIndex,
                 0
                 );

    //
    // Should not reach end of SAD rules before allocating all memory, otherwise
    // issue a warning and return.
    //
    if (SadIndex >= SAD_RULES_10NM) {
      RcDebugPrintWithDevice (SDBG_MINMAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "SAD rules exceed %d. Cannot create SAD for Socket %d\n",
        SAD_RULES_10NM,
        SocketIndex
        );
      OutputWarning (
        WARN_INTERLEAVE_FAILURE,
        WARN_SAD_RULES_EXCEEDED,
        SocketIndex,
        0xFF,
        0xFF,
        0xFF
        );
      return ERROR_RESOURCE_CALCULATION_FAILURE;
    }

    Merge = TRUE;
    //
    // Check if the previous entry has the same interleave target list
    //
    for (Index = 0; Index < MC_MAX_NODE; Index += MaxImc) {
      if (SadIndex == 0) {
        break;
      }

      SocketFlag = FALSE;
      //
      // Does previous SAD entry include socket[i]?
      //
      for (McIndex = 0; McIndex < MaxImc; McIndex++) {
        //
        // Does new SAD entry include socket[i]?
        //
        if (MemMapHost->SocketInfo[SocketIndex].SadIntList[SadIndex - 1][McIndex] != 0) {
          SocketFlag = TRUE;
          break;
        }
      }

      //
      // If the previous SAD entry and the new SAD entry do not match, then cannot merge
      //
      if (!(((SocketFlag == 0) && (MapFlag == 0)) || ((SocketFlag != 0) && (MapFlag != 0)))) {
        Merge = FALSE;
        break;
      }
      //
      // Keep track of the number of sockets in the interleave list
      //
      if (SocketFlag) {
        SocketsInInterleaveList++;
      }
    }

    //
    // Merge the entry with the previous SAD entry if the previous entry is also a remote entry
    //
    if (SadIndex > 0) {
      if (Merge && Socket->SAD[SadIndex - 1].local == 0) {
        Socket->SAD[SadIndex - 1].Limit = NodeLimit;
        SadMerged = TRUE;
      }
    }
    if (!SadMerged) {
      Socket->SAD[SadIndex].Enable = 1;
      Socket->SAD[SadIndex].Limit = NodeLimit;
      Socket->SAD[SadIndex].ways = 0;
      for (McIndex = 0; McIndex < MaxImc; McIndex++) {
        Socket->SAD[SadIndex].FMchannelInterBitmap[McIndex] = 0;
        Socket->SAD[SadIndex].channelInterBitmap[McIndex] = 0;
      }
      Socket->SAD[SadIndex].imcInterBitmap = 0;
      Socket->SAD[SadIndex].NmImcInterBitmap = 0;
      Socket->SAD[SadIndex].local = 0;
      Socket->SAD[SadIndex].type = 0;
      Socket->SAD[SadIndex].granularity = 0;
      Socket->SAD[SadIndex].mirrored = 0;
      Socket->SAD[SadIndex].tgtGranularity = 0;

      for (Index = 0; Index < MC_MAX_NODE; Index++) {
        if ((McBitmap[Index / MaxImc] & (BIT0 << (Index % MaxImc))) != 0) {
          MemMapHost->SocketInfo[SocketIndex].SadIntList[SadIndex][Index] = 1;
        }
      }
    }
  }// SocketIndex
  return SUCCESS;
}

/**
  Do interleaving across sockets for many (>2) MCs.

  @param[in, out] Host            Pointer to the host structure.
  @param[in, out] MemMapData      Pointer to the memory map data structure.
  @param[in]      SocketBitmap    Bitmap of sockets to interleave.
  @param[in]      Cluster         Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType         Type of memory being interleaved.
  @param[in]      McWays          Number of MCs to interleave.
  @param[in]      ChannelWays     Number of channels to interleave per MC.
  @param[in]      McBitmap        Bitmap of the MCs to be interleaved.
  @param[in]      ChannelBitmap   Array of Bitmaps of the DDR Channel population within the MCs to be interleaved.
  @param[in]      FmChannelBitmap Array of Bitmaps of the DDRT Channel population within the MCs to be interleaved.
  @param[in]      ChannelSortList Channels per MC in order of decreasing size.
  @param[out]     NodeLimit       Pointer to return the limit of SAD entry created.
  @param[in]      MirrorEnable    Enable/Disable mirroring for the interleave's SAD rule.

  @retval SUCCESS       Memory was successfully interleaved.
  @retval FAILURE       Interleave could not be supported for memory type.
**/
UINT32
ExecuteMod3Interleave (
  IN OUT  SYSHOST             *Host,
  IN OUT  MEMORY_MAP_DATA     *MemMapData,
  IN      UINT8               SocketBitmap,
  IN      UINT8               Cluster,
  IN      MEM_TYPE            MemType,
  IN      UINT8               McWays,
  IN      UINT8               ChannelWays,
  IN      UINT8               McBitmap[MAX_SOCKET],
  IN      UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC],
  IN      UINT8               FmChannelBitmap[MAX_SOCKET][MAX_IMC],
  IN      CHANNEL_SORT_STRUCT ChannelSortList[MAX_SOCKET][MAX_CH],
  OUT     UINT32              *InterleaveLimit,
  IN      UINT8               MirrorEnable
  )
{
  MEMORY_MAP_HOST                 *MemMapHost;
  UINT8                           MaxImc;
  UINT8                           RasMode;
  UINT8                           SocketIndex;
  UINT8                           ChannelIndex;
  UINT8                           McIndex;
  UINT8                           SadIndex;
  UINT8                           MaxSadIndex;
  UINT32                          InterleaveSize;
  UINT8                           SocketWays;
  UINT32                          Status;
  ROUTE_TABLE_HALVES              Half;
  DDR_CHANNEL_STRUCT              *ChannelList;
  SOCKET_INFO_STRUCT              *Socket;
  UINT8                           Index;
  UINT8                           SocketNum;
  UINT8                           NumChPerMc;
  UINT8                           MatchMcToHalf = FALSE;
  UINT32                          SadBase;
  UINT8                           InterleaveWaysWithinSocket;

  MemMapHost = GetMemMapHostPointer ();

  if (Host == NULL || MemMapData == NULL) {
    return FAILURE;
  }

  if (Host->var.mem.chInter != CH_3WAY ||
      ChannelWays != CH_3WAY) {
    return FAILURE;
  }

  if (McBitmap == NULL || ChannelBitmap == NULL || FmChannelBitmap == NULL || ChannelSortList == NULL) {
    return FAILURE;
  }

  if (Cluster >= MAX_CLUSTERS) {
    return FAILURE;
  }

  MaxImc = GetMaxImc ();
  if (McWays == 0 || McWays > MaxImc) {
    return FAILURE;
  }

  //
  // 3-way in a half is supported only in SNC-2 for 2MCs per half
  //
  if(IfFeatureEnabled (TypeSnc) && (Cluster >= KTI_SNC2)) {
    return FAILURE;
  }

  NumChPerMc = GetNumChannelPerMc ();

  //
  // Check request is for 3-way within an MC, 3-way across halves or 6-way across 4 MC's
  // 3-way across 2 MC's is not POR for ICX
  //
  if (McWays < IMC_4WAY) {
    if (!IsCpuCapableThreeChannelInterleaveAcrossMcs () && NumChPerMc != CH_3WAY) {
      return FAILURE;
    }
  }

  //
  // Support for 3 channels interleaving across 2 MCs is only supported for 1LM and is not compatible with mirror mode.
  //
  if (NumChPerMc != CH_3WAY) {
    RasMode = GetRasMode ();
    if ((RasMode & PARTIAL_MIRROR_1LM) != 0 ||
      (RasMode & FULL_MIRROR_1LM) != 0) {
      return FAILURE;
    }
  }

  SocketWays = 0;
  InterleaveSize = 0;
  SocketIndex = 0;
  Socket = &Host->var.mem.socket[SocketIndex];

  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {

    //
    // skip sockets not particicpating in the interleave
    //
    if ((SocketBitmap & (BIT0 << SocketIndex)) == 0 || McBitmap[SocketIndex] == 0) {
      continue;
    }

    //
    // Calculate the minimum interleave size
    //
    InterleaveSize = 0xFFFF;
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if ((McBitmap[SocketIndex] & (BIT0 << McIndex)) == 0) {
        continue;
      }

      //
      // The interleave size is the smallest channel size for any MC.
      //
      for (ChannelIndex = (McIndex * NumChPerMc); (ChannelIndex < ((McIndex * NumChPerMc) + NumChPerMc) &&
        ChannelSortList[SocketIndex][ChannelIndex].chSize != 0); ChannelIndex++) {
        if (ChannelSortList[SocketIndex][ChannelIndex].chSize < InterleaveSize) {
          InterleaveSize = ChannelSortList[SocketIndex][ChannelIndex].chSize;
        }
      }
    } // McIndex

    if (InterleaveSize == 0) {
        return FAILURE;
    }

    SocketWays++;
  } // SocketIndex

  if (SocketWays == 0) {
    return FAILURE;
  }

  if ((MemType == MemType1lmDdr) ||
    (MemType == MemType1lmHbm)) {
    InterleaveWaysWithinSocket = GetTotalChannelCountFromAnInterleave (SocketBitmap,
                        McBitmap,
                        ChannelBitmap) / SocketWays;
  } else {
    InterleaveWaysWithinSocket = GetTotalChannelCountFromAnInterleave (SocketBitmap,
                        McBitmap,
                        FmChannelBitmap) / SocketWays;
  }

  //
  // Parse through each socket to create a SAD
  //
  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {

    if ((SocketBitmap & (BIT0 << SocketIndex)) == 0 || McBitmap[SocketIndex] == 0) {
      continue;
    }

    Socket = &Host->var.mem.socket[SocketIndex];
    ChannelList = Host->var.mem.socket[SocketIndex].channelList;

    //
    // Get index of next available SAD entry.
    //
    SadIndex = FindSadIndex (
                 Host,
                 MemMapData,
                 SocketIndex,
                 Cluster
                 );

    MaxSadIndex = ((((MemMapData->NumberofChaDramClusters > 1) ? Cluster : 0) * MemMapData->MaxSadRules) +
      MemMapData->MaxSadRules);

    //
    // Should not reach end of SAD rules before allocating all memory, otherwise
    // issue a warning and return.
    //
    if (SadIndex >= MaxSadIndex) {
      RcDebugPrintWithDevice (SDBG_MINMAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "SAD rules exceed %d. Cannot create SAD for Socket %d\n",
        MaxSadIndex,
        SocketIndex
        );
      OutputWarning (
        WARN_INTERLEAVE_FAILURE,
        WARN_SAD_RULES_EXCEEDED,
        SocketIndex,
        0xFF,
        0xFF,
        0xFF
        );
      return ERROR_RESOURCE_CALCULATION_FAILURE;
    }

    // Check if memory is already allocated up to MMIOH Base.
    SadBase = GetSadBase (Host, MemMapData, SocketIndex, SadIndex);
    if (SadBase == Host->var.mem.mmiohBase) {
      RcDebugPrintWithDevice (SDBG_MINMAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "WARNING!!! Failure to create Sad%d! Sad base is MMIOHBase...\n", SadIndex);
      return SUCCESS;
    }

    //
    // Assign a route table for this interleave.
    //
    for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
      Status = AssignRouteTable (
                 Host,
                 MemMapData,
                 SocketIndex,
                 Cluster,
                 Half,
                 MemType,
                 ONE_WAY,
                 ChannelWays,
                 McBitmap[SocketIndex],
                 ChannelBitmap[SocketIndex],
                 FmChannelBitmap[SocketIndex],
                 ChannelSortList[SocketIndex],
                 &MatchMcToHalf
                 );
      if (Status == SUCCESS) {
        //
        // MatchMcToHalf can be set to TRUE in return from above call if the interleaving ways is greater than the
        // selected RT's maximum supported interleave ways. If MatchMcToHalf is FALSE then the route table in the other
        // half is not required to be populated.
        //
        if (!MatchMcToHalf) {
          break;
        }
      } else {
        return Status;
      }
    }

    //
    // Adjust InterleaveLimit to include the limit from socket memory last mapped
    //
    *InterleaveLimit = SadBase + InterleaveSize * InterleaveWaysWithinSocket * SocketWays;

    //
    // Add new SAD entries including TOLM/TOHM gap found or DRAM high address NXM.
    //
    Status = AddSadEntries (
               Host,
               MemMapData,
               SocketIndex,
               MemType,
               SocketWays,
               McWays,
               ChannelWays,
               McBitmap[SocketIndex],
               McBitmap[SocketIndex],
               ChannelBitmap[SocketIndex],
               FmChannelBitmap[SocketIndex],
               SadIndex,
               InterleaveLimit,
               Cluster,
               MirrorEnable
               );

    if (Status != SUCCESS) {
      return Status;
    }

    Index = SadIndex;

    //
    // Get index of next available SAD entry.
    //
    SadIndex = FindSadIndex (
                 Host,
                 MemMapData,
                 SocketIndex,
                 Cluster
                 );

    //
    // Update SAD Interleave list for other sockets in the interleave
    //
    for (; Index < SadIndex; Index++) {
      for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {
        //
        // If socket does not participate in interleave or is current socket, continue
        // Interleave list for the current socket is calculated in AddSadEntries().
        //
        if (McBitmap[SocketNum] == 0 || SocketNum == SocketIndex) {
          continue;
        }
        for (McIndex = 0; McIndex < MaxImc; McIndex++) {
          if (McBitmap[SocketNum] & (BIT0 << McIndex)) {
            MemMapHost->SocketInfo[SocketIndex].SadIntList[Index][ (SocketNum * MaxImc) + McIndex] = 1;
          }
        }
      }
    }

    //
    // Subtract interleaved size from the remaining size in corresponding
    // channels.
    //
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if ((McBitmap[SocketIndex] & (BIT0 << McIndex)) == 0) {
        continue;
      }
      for (ChannelIndex = 0; ChannelIndex < NumChPerMc; ChannelIndex++) {
        if (MemType == MemType1lmDdr) {
          if ((ChannelBitmap[SocketIndex][McIndex] & (BIT0 << ChannelIndex)) != 0) {
            ChannelList[(McIndex * NumChPerMc) + ChannelIndex].remSize -= InterleaveSize;
          }
        } else {
          if ((FmChannelBitmap[SocketIndex][McIndex] & (BIT0 << ChannelIndex)) != 0) {
            ChannelList[(McIndex * NumChPerMc) + ChannelIndex].remSize -= InterleaveSize;
          }
        }
      }
    }
    DisableXorModeForMod3 (MemMapData, SocketIndex);

  } // SocketIndex

  return SUCCESS;
}

/**
  Do interleaving across sockets for many (>2) MCs.

  @param[in, out] Host            Pointer to the host structure.
  @param[in, out] MemMapData      Pointer to the memory map data structure.
  @param[in]      SocketBitmap    Bitmap of sockets to interleave.
  @param[in]      Cluster         Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType         Type of memory being interleaved.
  @param[in]      McWays          Number of MCs to interleave.
  @param[in]      ChannelWays     Number of channels to interleave per MC.
  @param[in]      McBitmap        Bitmap of the MCs to be interleaved
  @param[in]      ChannelBitmap   Array of Bitmaps of the DDR Channel population within the MCs to be interleaved
  @param[in]      FmChannelBitmap Array of Bitmaps of the DDRT Channel population within the MCs to be interleaved
  @param[in]      ChannelSortList Channels per MC in order of decreasing size.
  @param[out]     InterleaveLimit Pointer to return the limit of SAD entry created.
  @param[in]      MirrorEnable    Enable/Disable mirroring for the interleave's SAD rule

  @retval SUCCESS       Memory was successfully interleaved.
  @retval FAILURE       Interleave could not be supported for memory type.
**/
UINT32
ExecuteXorInterleave (
  IN OUT  SYSHOST             *Host,
  IN OUT  MEMORY_MAP_DATA     *MemMapData,
  IN      UINT8               SocketBitmap,
  IN      UINT8               Cluster,
  IN      MEM_TYPE            MemType,
  IN      UINT8               McWays,
  IN      UINT8               ChannelWays,
  IN      UINT8               McBitmap[MAX_SOCKET],
  IN      UINT8               NmMcBitmap[MAX_SOCKET],
  IN      UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC],
  IN      UINT8               FmChannelBitmap[MAX_SOCKET][MAX_IMC],
  IN      CHANNEL_SORT_STRUCT ChannelSortList[MAX_SOCKET][MAX_CH],
  OUT     UINT32              *InterleaveLimit,
  IN      UINT8               MirrorEnable
  )
{
  MEMORY_MAP_HOST                 *MemMapHost;
  UINT8                           MaxImc;
  UINT8                           SocketIndex;
  UINT8                           ChannelIndex;
  UINT8                           McIndex;
  UINT8                           SadIndex;
  UINT8                           MaxSadIndex;
  UINT32                          InterleaveSize;
  UINT8                           SocketWays;
  UINT32                          Status;
  ROUTE_TABLE_HALVES              Half;
  DDR_CHANNEL_STRUCT              *ChannelList;
  UINT8                           RtMcWays;
  UINT8                           RtChannelWays;
  UINT8                           RtMcBitMap;
  UINT8                           McBitmapInternal;
  UINT8                           ChBitmap[MAX_IMC];
  UINT8                           FmTarget;
  UINT8                           FmChannelBitmapInternal[MAX_IMC];
  SOCKET_INFO_STRUCT              *Socket;
  UINT8                           Index;
  UINT8                           SocketNum;
  UINT8                           MaxChPerMc;
  UINT8                           MatchMcToHalf;
  UINT32                          SadBase;
  UINT8                           UmaBasedClusters;

  MemMapHost = GetMemMapHostPointer ();

  if (Host == NULL || MemMapData == NULL) {
    return FAILURE;
  }

  if (McBitmap == NULL || NmMcBitmap == NULL || ChannelBitmap == NULL || FmChannelBitmap == NULL ||
      ChannelSortList == NULL) {
    return FAILURE;
  }

  if (Cluster >= MAX_CLUSTERS) {
    return FAILURE;
  }

  MaxImc = GetMaxImc ();
  if (McWays == 0 || McWays > MaxImc) {
    return FAILURE;
  }

  MaxChPerMc = GetNumChannelPerMc ();
  if (ChannelWays == 0 || (ChannelWays > MaxChPerMc) ||
      ChannelWays > Host->var.mem.chInter || ChannelWays == CH_3WAY) {
    return FAILURE;
  }

  if (McWays == 0 || McWays > MaxImc ||
      ChannelWays == 0 || ChannelWays > Host->var.mem.chInter) {
    return FAILURE;
  }

  SocketWays = 0;
  SocketIndex = 0;
  Socket = &Host->var.mem.socket[SocketIndex];

  InterleaveSize = 0xFFFF;
  MatchMcToHalf = FALSE;

  RtMcWays = 0;
  RtMcBitMap = 0;


  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {

    //
    // skip sockets not particicpating in the interleave
    //
    if ((SocketBitmap & (BIT0 << SocketIndex)) == 0 || McBitmap[SocketIndex] == 0) {
      continue;
    }

    ChannelList = Host->var.mem.socket[SocketIndex].channelList;

    //
    // Calculate the minimum interleave size
    //
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if ((McBitmap[SocketIndex] & (BIT0 << McIndex)) == 0) {
        continue;
      }

      //
      // The interleave size is the smallest channel size for any MC.
      //
      if (MemMapData->IsPorConfig) {
        ChannelIndex = (McIndex * MaxChPerMc) + ChannelWays - 1;
        if (ChannelSortList[SocketIndex][ChannelIndex].chSize < InterleaveSize) {
          InterleaveSize = ChannelSortList[SocketIndex][ChannelIndex].chSize;
        }
      } else {
        for (ChannelIndex = 0; (ChannelIndex < MaxChPerMc &&
          ChannelSortList[SocketIndex][(McIndex * MaxChPerMc) + ChannelIndex].chSize != 0); ChannelIndex++) {
          if (ChannelSortList[SocketIndex][(McIndex * MaxChPerMc) + ChannelIndex].chSize < InterleaveSize) {
            InterleaveSize = ChannelSortList[SocketIndex][(McIndex * MaxChPerMc) + ChannelIndex].chSize;
          }
        }
      }
    } // McIndex

    if (InterleaveSize == 0) {
        return FAILURE;
    }

    SocketWays++;
  } // SocketIndex

  if (SocketWays == 0) {
    return FAILURE;
  }

  //
  // Parse through each socket to create a SAD
  //
  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {

    if ((SocketBitmap & (BIT0 << SocketIndex)) == 0 || McBitmap[SocketIndex] == 0) {
      continue;
    }

    Socket = &Host->var.mem.socket[SocketIndex];
    ChannelList = Host->var.mem.socket[SocketIndex].channelList;


    if (IsMemType2lmHbm (MemType)) {
      LoadHbm2lmInterleaveData ((UINT8 *)&ChBitmap, &RtMcBitMap);
      Status = FarDdrPhysicalToLogical (
        Host,
        MemMapData,
        SocketIndex,
        Cluster,
        MemType,
        McWays,
        ChannelWays,
        McBitmap[SocketIndex],
        FmChannelBitmap[SocketIndex]
      );
      if (Status != SUCCESS) {
        return Status;
      }
    } else {
      CopyMem (ChBitmap, ChannelBitmap[SocketIndex], sizeof (ChBitmap));

      if (IsMemType2lm (MemType)) {
        RtMcBitMap = NmMcBitmap[SocketIndex];
        //
        // The route tables are populated using the Near Memory for 2LM
        //
        GetNearMemoryWays (RtMcBitMap, ChannelBitmap[SocketIndex], &RtMcWays, &RtChannelWays);
      } else {
        RtMcWays = McWays;
        RtChannelWays = ChannelWays;
        RtMcBitMap = McBitmap[SocketIndex];
      }

      //
      // Assign a route table for this interleave.
      //
      for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
        Status = AssignRouteTable (
          Host,
          MemMapData,
          SocketIndex,
          Cluster,
          Half,
          MemType,
          RtMcWays,
          RtChannelWays,
          RtMcBitMap,
          ChannelBitmap[SocketIndex],
          FmChannelBitmap[SocketIndex],
          ChannelSortList[SocketIndex],
          &MatchMcToHalf
        );
        if (Status == SUCCESS) {
          //
          // MatchMcToHalf can be set to TRUE in return from above call if the interleaving ways is greater than the
          // selected RT's maximum supported interleave ways. If MatchMcToHalf is FALSE then the route table in the other
          // half is not required to be populated.
          //
          if (!MatchMcToHalf) {
            break;
          }
        } else {
          return Status;
        }
      }
    }

    //
    // Get index of next available SAD entry.
    //
    SadIndex = FindSadIndex (
                  Host,
                  MemMapData,
                  SocketIndex,
                  Cluster
                  );
    MaxSadIndex = ((((MemMapData->NumberofChaDramClusters > 1) ? Cluster : 0) * MemMapData->MaxSadRules) +
                    MemMapData->MaxSadRules);

    //
    // Should not reach end of SAD rules before allocating all memory, otherwise
    // issue a warning and return.
    //
    if (SadIndex >= MaxSadIndex) {
      RcDebugPrintWithDevice (SDBG_MINMAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "SAD rules exceed %d. Cannot create SAD for Socket %d\n",
        MaxSadIndex,
        SocketIndex
      );
      OutputWarning (
        WARN_INTERLEAVE_FAILURE,
        WARN_SAD_RULES_EXCEEDED,
        SocketIndex,
        0xFF,
        0xFF,
        0xFF
      );
      return ERROR_RESOURCE_CALCULATION_FAILURE;
    }

    // Check if memory is already allocated up to MMIOH Base.
    SadBase = GetSadBase (Host, MemMapData, SocketIndex, SadIndex);
    if (SadBase == Host->var.mem.mmiohBase) {
      RcDebugPrintWithDevice (SDBG_MINMAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "WARNING!!! Failure to create Sad%d! Sad base is MMIOHBase...\n", SadIndex);
      return SUCCESS;
    }

    *InterleaveLimit = SadBase + InterleaveSize * ChannelWays * McWays * SocketWays;

    if (IsMemType2lm (MemType) && MemMapData->MemMapSocket[SocketIndex].XTile2lm &&
      MemMapData->MemMapSocket[SocketIndex].XTile2lmSncFmPartition) {
      McBitmapInternal = 0;
      ZeroMem (FmChannelBitmapInternal, sizeof (FmChannelBitmapInternal));
      for (McIndex = 0; McIndex < MaxImc; McIndex++) {
        if ((McBitmap[SocketIndex] & (BIT0 << McIndex)) != 0) {
          //
          // For X-Tile 2LM, we need to create SAD entry on far memory target.
          //
          FmTarget = MemMapData->MemMapSocket[SocketIndex].XTile2lmFmTarget[McIndex];
          McBitmapInternal |= BIT0 << FmTarget;
          FmChannelBitmapInternal[FmTarget] = FmChannelBitmap[SocketIndex][McIndex];
        }
      }
    } else {
      McBitmapInternal = McBitmap[SocketIndex];
      CopyMem (FmChannelBitmapInternal, FmChannelBitmap[SocketIndex], sizeof (FmChannelBitmapInternal));
    }
    //
    // Add new SAD entries including TOLM/TOHM gap found or DRAM high address NXM.
    //
    Status = AddSadEntries (
      Host,
      MemMapData,
      SocketIndex,
      MemType,
      SocketWays,
      McWays,
      ChannelWays,
      McBitmapInternal,
      RtMcBitMap,
      ChBitmap,
      FmChannelBitmapInternal,
      SadIndex,
      InterleaveLimit,
      Cluster,
      MirrorEnable
    );

    if (Status != SUCCESS) {
      return Status;
    }

    Index = SadIndex;

    //
    // Get index of next available SAD entry.
    //
    SadIndex = FindSadIndex (
                  Host,
                  MemMapData,
                  SocketIndex,
                  Cluster
                  );
    //
    // Update SAD Interleave list for other sockets in the interleave
    //
    for (; Index < SadIndex; Index++) {
      for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {
        //
        // If socket does not participate in interleave or is current socket, continue
        // Interleave list for the current socket is calculated in AddSadEntries().
        //
        if (McBitmap[SocketNum] == 0 || SocketNum == SocketIndex) {
          continue;
        }
        for (McIndex = 0; McIndex < MaxImc; McIndex++) {
          if (McBitmap[SocketNum] & (BIT0 << McIndex)) {
            MemMapHost->SocketInfo[SocketIndex].SadIntList[Index][(SocketNum * MaxImc) + McIndex] = 1;
          }
        }
      }
    }


    //
    // Subtract interleaved size from the remaining size in corresponding
    // channels.
    //
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if ((McBitmap[SocketIndex] & (BIT0 << McIndex)) == 0) {
        continue;
      }
      for (ChannelIndex = 0; ChannelIndex < MaxChPerMc; ChannelIndex++) {
        if (MemType == MemType1lmDdr) {
          if ((ChannelBitmap[SocketIndex][McIndex] & (BIT0 << ChannelIndex)) != 0) {
            ChannelList[(McIndex * MaxChPerMc) + ChannelIndex].remSize -= InterleaveSize;
          }
        } else {
          if ((FmChannelBitmap[SocketIndex][McIndex] & (BIT0 << ChannelIndex)) != 0) {
            ChannelList[(McIndex * MaxChPerMc) + ChannelIndex].remSize -= InterleaveSize;
          }
        }
      }
    }

    //
    // Calculate systen UMA based clustering level for XOR mode interleaving
    //
    if (!IsMemType2lmHbm (MemType)) {
      UmaBasedClusters = MemMapData->UmaBasedClustersPerSocket;
      if (!IfFeatureEnabled (TypeSnc) && ( UmaBasedClusters != UMABASEDCLUSTERING_DISABLED) && IsUbcSupportedMemType (MemType) ) {
        if (MemMapData->IsPorConfig && IsOneMcOnEachSide (RtMcBitMap)) {
          if (IsPowerOfTwo (RtMcWays)) {
            if (RtMcWays == UMABASEDCLUSTERING_QUADRANT) {
              MemMapData->UmaBasedClustersPerSocket = UMABASEDCLUSTERING_QUADRANT;
            } else {
              MemMapData->UmaBasedClustersPerSocket = UMABASEDCLUSTERING_HEMISPHERE;
            }
            //
            // Update the number of UMA based clusters for KTI RC consumption if the UMA-Based Clustering is downgraded.
            // Save the number of UMA based clusters for MemMap consumption.
            //
            if (MemMapData->UmaBasedClustersPerSocket > UmaBasedClusters) {
              MemMapData->UmaBasedClustersPerSocket = UmaBasedClusters;
            }
          }
        } else {
          MemMapData->UmaBasedClustersPerSocket = UMABASEDCLUSTERING_DISABLED;
        }
      }
    }
  } // SocketIndex

  return SUCCESS;
}

/** Returns TRUE if the input Socket and McIndex is part of the Socket Interleave.

  @param[in] Socket                - The Socket to check against the SocketInterleave
  @param[in] McIndex               - The index of the MC in the socket to check against the SocketInterleave
  @param[in] ScktInterleave        - An array that describes the MCs in all sockets participating in the interleave

  @retval     TRUE                 -  The Socket and MC is found in the SocketInterleave array
  @retval     FALSE                -  The Socket and MC is not found in the SocketInterleave array

**/
BOOLEAN
IsMcPartOfInterleave (
  IN UINT8   Socket,
  IN UINT8   McIndex,
  IN UINT32  ScktInterleave[MC_MAX_NODE]
  )
{
  if (ScktInterleave[Socket * GetMaxImc () + McIndex] != 0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/** Returns TRUE if the input Socket has at least one MC that is part of the Socket Interleave.

  @param[in] Socket                - The Socket to check against the SocketInterleave
  @param[in] ScktInterleave        - An array that describes the MCs in all sockets participating in the interleave

  @retval     TRUE                 -  The Socket has at least one MC that is found in the SocketInterleave array
  @retval     FALSE                -  The Socket has no MCs that are part of the SocketInterleave array

**/
BOOLEAN
IsSocketPartOfInterleave (
  IN UINT8   Socket,
  IN UINT32  ScktInterleave[MC_MAX_NODE]
  )
{
  UINT8 McIndex;
  UINT8 MaxImc;

  MaxImc = GetMaxImc ();
  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if (IsMcPartOfInterleave (Socket, McIndex, ScktInterleave)) {
      return TRUE;
    }
  }
  return FALSE;
}

#ifdef DDRT_SUPPORT
/** Create SAD and Route Table entries for the input interleave configuration.

  This routine creates a DRAM rule for a specific interleave request and updates the SADTable struct in corresponding sockets. This function
  should be called for platform configurations utilizing 4MCs. This functions assumes that the calling function has already verified that the interleave is possible.

  @param[in, out] Host                 - Pointer to sysHost
  @param[in, out] MemMapData           - Pointer to the memory map private data structure
  @param[in]      ScktInterleave       - structure with list of MCs that are part of this interleave
  @param[in]      SktWays              - Number of IMCs involved in this interleave
  @param[in]      FmChannelBitmapPerMc - Far memory channel Interleave bitmap
  @param[in]      ChannelBitmapPerMc   - Near memory channel Interleave bitmap
  @param[in]      InterSizePerCh       - Interleave size for each channel
  @param[in]      MemType              - PMEM / PMEM$
  @param[in]      MirrorEnable         - Enable/Disable mirroring for this SAD rule
  @param[in]      InterleaveSize       - Interleave granularities for this SAD rule
  @param[in, out] NodeLimit            - Pointer to return the limit of SAD created.

  @retval     SUCCESS              -  The interleave wass successfully applied. A SAD rule was added and Route Table entries were created
  @retval     FAILURE              -  The interleave could not be applied

**/
UINT32
AddSadRule (
  IN OUT PSYSHOST        Host,
  IN OUT MEMORY_MAP_DATA *MemMapData,
  IN     UINT32          ScktInterleave[MC_MAX_NODE],
  IN     UINT8           SktWays,
  IN     UINT8           FmChannelBitmapPerMc,
  IN     UINT8           ChannelBitmapPerMc,
  IN     UINT16          InterSizePerCh,
  IN     MEM_TYPE        MemType,
  IN     UINT8           MirrorEnable,
  IN     UINT32          InterleaveSize,
  IN     UINT32          *NodeLimit
  )
{
  UINT32              Status;
  UINT8               Socket;
  UINT8               McIndex;
  UINT8               Channel;
  UINT8               SortListChannelIndex;
  UINT8               ChannelWays = 0;
  UINT8               Cluster;
  UINT8               McWays;
  UINT8               McBitmap[MAX_SOCKET];
  CHANNEL_SORT_STRUCT ChannelSortList[MAX_SOCKET][MAX_CH];
  UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC];
  UINT8               FmChannelBitmap[MAX_SOCKET][MAX_IMC];
  UINT8               MaxImc;
  UINT8               MaxChPerMc;

  ZeroMem (McBitmap, sizeof (McBitmap));
  ZeroMem ((UINT8 *) ChannelSortList, sizeof (ChannelSortList));
  ZeroMem (ChannelBitmap, sizeof (ChannelBitmap));
  ZeroMem (FmChannelBitmap, sizeof (FmChannelBitmap));
  Cluster = 0;
  MaxImc = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (!IsSocketPartOfInterleave (Socket, ScktInterleave)) {
      continue;
    }

    //
    // Transform the inputs into the McBitmap, McWays, ChWays, and ChannelSortList
    // required for the call to interleaving execution.
    //
    McWays = 0;
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if (!IsMcPartOfInterleave (Socket, McIndex, ScktInterleave)) {
        continue;
      }

      //
      // Count the McWays
      //
      McWays++;

      //
      // Populate an McBitmap
      //
      McBitmap[Socket] |= (BIT0 << McIndex);

      //
      // Populate the channel bitmaps
      //
      FmChannelBitmap[Socket][McIndex] = FmChannelBitmapPerMc;
      ChannelBitmap[Socket][McIndex] = ChannelBitmapPerMc;

      //
      // Create a Channel Sort list
      //
      SortListChannelIndex = 0;
      for (Channel = 0; (Channel < MaxChPerMc) && (SortListChannelIndex < MaxChPerMc); Channel++) {
        if ((FmChannelBitmap[Socket][McIndex] & (BIT0 << Channel)) == 0) {
          continue;
        }
        ChannelSortList[Socket][(McIndex * MaxChPerMc) + SortListChannelIndex].chNum = Channel;
        ChannelSortList[Socket][(McIndex * MaxChPerMc) + SortListChannelIndex].socketNum = Socket;
        ChannelSortList[Socket][(McIndex * MaxChPerMc) + SortListChannelIndex].chSize = InterSizePerCh;
        SortListChannelIndex++;
      } // Channel

      //
      // Count the Channel Ways per MC
      //
      ChannelWays = SortListChannelIndex;
    } // McIndex

    if (ChannelWays == CH_3WAY) {
      Status = ExecuteMod3Interleave (
                 Host,
                 MemMapData,
                 (BIT0 << Socket),
                 Cluster,
                 MemType,
                 McWays,
                 ChannelWays,
                 McBitmap,
                 ChannelBitmap,
                 FmChannelBitmap,
                 ChannelSortList,
                 NodeLimit,
                 MirrorEnable
                 );
      if (Status != SUCCESS) {
        return Status;
      }
    } else {
      Status = ExecuteXorInterleave (
                 Host,
                 MemMapData,
                 (BIT0 << Socket),
                 Cluster,
                 MemType,
                 McWays,
                 ChannelWays,
                 McBitmap,
                 McBitmap,
                 ChannelBitmap,
                 FmChannelBitmap,
                 ChannelSortList,
                 NodeLimit,
                 MirrorEnable
               );

      if (Status != SUCCESS) {
        return Status;
      }
    }

    //
    // Add remote Dram rule after interleaving all non-volatile memory from this socket
    //
    AddRemoteSadEntry (
      Host,
      MemMapData,
      Socket,
      MemType1lmAppDirect,
      0
      );
  } // Socket

  return SUCCESS;
}
#endif // DDRT_SUPPORT

/**
  Check if the input socket has a volatile memory interleave that is symmetric across clusters.
  This routine assumes that the input socket has a single volatile memory interleave.

  @param[in] MemMapData                - Pointer to memory map private data structure.
  @param[in] Sckt                      - The socket to process
  @param[in] UmaBasedClustersPerSocket - The number of UMA-based clusters per socket
  @param[in] SadIndex                  - SAD instance to check the MC symmetry across clusters

  @retval TRUE The input socket has a volatile memory interleave that is symmetric across clusters
  @retval FALSE The input socket volatile memory inteleave is not symmetric across clusters
**/
BOOLEAN
IsMcInterleaveSymmetricAcrossClusters (
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            Sckt,
  IN UINT8            UmaBasedClustersPerSocket,
  IN UINT8            SadIndex
  )
{
  SYSHOST   *Host;
  SAD_TABLE *SadEntry;
  UINT8     McBitmap;
  UINT8     Cluster;
  UINT8     McIndex;
  UINT8     McWays[MAX_CLUSTERS];
  UINT8     ChannelWays[MAX_CLUSTERS];
  UINT8     FmChannelWays[MAX_CLUSTERS];
  BOOLEAN   XTile2lm;

  if (MemMapData == NULL || Sckt >= MAX_SOCKET) {
    return FALSE;
  }

  if (UmaBasedClustersPerSocket == UMABASEDCLUSTERING_DISABLED ||
      UmaBasedClustersPerSocket > MAX_CLUSTERS) {
    return FALSE;
  }

  ZeroMem (&McWays, sizeof (McWays));
  ZeroMem (&ChannelWays, sizeof (ChannelWays));
  ZeroMem (&FmChannelWays, sizeof (FmChannelWays));

  Host = GetSysHostPointer ();

  //
  // SAD Rule 0 should describe the entire socket interleave
  //
  SadEntry = &Host->var.mem.socket[Sckt].SAD[SadIndex];
  if (IsMemType2lm (SadEntry->type) && MemMapData->MemMapSocket[Sckt].XTile2lm) {
    XTile2lm = TRUE;
  } else {
    XTile2lm = FALSE;
  }
  //
  // Count the number of MCs in cluster
  //
  for (Cluster = 0; Cluster < UmaBasedClustersPerSocket; Cluster++) {
    //
    // Mask out MCs in other clusters
    //
    if (XTile2lm) {
      McBitmap = SadEntry->NmImcInterBitmap & GetMcBitMaskForCluster (Cluster, UmaBasedClustersPerSocket);
    } else {
      McBitmap = SadEntry->imcInterBitmap & GetMcBitMaskForCluster (Cluster, UmaBasedClustersPerSocket);
    }
    //
    // Calculate the MC Ways in this cluster
    //
    McWays[Cluster] = CalculateInterleaveWaysfromBitmap (McBitmap);
    //
    // Count the number of Channels in this cluster
    //
    for (McIndex = 0; McIndex < GetMaxImc (); McIndex++) {
      if ((McBitmap & (BIT0 << McIndex)) == 0) {
        continue;
      }
      ChannelWays[Cluster]   += CalculateInterleaveWaysfromBitmap (SadEntry->channelInterBitmap[McIndex]);
      FmChannelWays[Cluster] += CalculateInterleaveWaysfromBitmap (SadEntry->FMchannelInterBitmap[McIndex]);
    }
    //
    // There must be at least one MC present in each cluster
    //
    if (McWays[Cluster] == 0) {
      return FALSE;
    }
  }

  //
  // The Each cluster must have a symmetric MC and Channel configuration
  // XTile2LM is an exception for far memory channel
  //
  for (Cluster = 0; Cluster < (UmaBasedClustersPerSocket - 1); Cluster++) {
    if (McWays[Cluster] != McWays[Cluster + 1] ||
        ChannelWays[Cluster] != ChannelWays[Cluster + 1] ||
        !XTile2lm && FmChannelWays[Cluster] != FmChannelWays[Cluster + 1]) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
  Adjust the input RoutaTableData for UMA-based clustering.

  @param[in]      MemMapData          Pointer to memory map private data struture.
  @param[in]      Socket              Socket ID
  @param[in, out] RouteTableData      Input and Output pointer to the RouteTableData to be adjusted.

**/
VOID
AdjustRouteTableForUmaBasedClustering (
  IN     MEMORY_MAP_DATA  *MemMapData,
  IN     UINT8            Socket,
  IN OUT ROUTE_TABLE_DATA *RouteTableData
  )
{
  ROUTE_TABLE_DATA   RouteTableDataTemp;
  UINT8              DestinationIndex;
  UINT8              McIndex;
  UINT8              ChannelIndex;
  UINT8              DestinationCluster;
  ROUTE_TABLE_HALVES Half;
  UINT8              RtIndex;
  UINT8              MaxChPerMc;

  if (MemMapData == NULL || RouteTableData == NULL || Socket >= MAX_SOCKET) {
    return;
  }

  MaxChPerMc = GetNumChannelPerMc ();

  ZeroMem (&RouteTableDataTemp, sizeof (RouteTableDataTemp));

  //
  // Adjust Route tables for UMA Based Clustering
  // Walk through route table and relocate each MC to it's corresponding cluster
  //
  for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
    for (RtIndex = 0; RtIndex < RouteTableData->RouteTableUsed[Socket][0][Half]; RtIndex++) {
      McIndex = RouteTableData->TgtRouteTable[Socket][0][Half][RtIndex];

      //
      // In-tile 4K can be Max channel ways within the Tile.
      //
      if (MemMapData->Is2LmUsingRt2lm) {
        ChannelIndex = RouteTableData->ChRouteTable[Socket][0][Half][RtIndex % MaxChPerMc];
      } else {
        ChannelIndex = RouteTableData->ChRouteTable[Socket][0][Half][RtIndex];
      }

      DestinationCluster = GetUmaBasedClusterForMc (McIndex, MemMapData->UmaBasedClustersPerSocket);
      DestinationIndex = RouteTableDataTemp.RouteTableUsed[Socket][DestinationCluster][RtHalf0];

      RouteTableDataTemp.TgtRouteTable[Socket][DestinationCluster][RtHalf0][DestinationIndex] = McIndex;
      if (MemMapData->Is2LmUsingRt2lm) {
        RouteTableDataTemp.ChRouteTable[Socket][DestinationCluster][RtHalf0][DestinationIndex % MaxChPerMc] = ChannelIndex;
      } else {
        RouteTableDataTemp.ChRouteTable[Socket][DestinationCluster][RtHalf0][DestinationIndex] = ChannelIndex;
      }
      RouteTableDataTemp.RouteTableUsed[Socket][DestinationCluster][RtHalf0]++;
    } // RtIndex
  } // Half

  //
  // Replace the route table with the new data per cluster
  //
  CopyMem (&RouteTableData->RouteTableUsed[Socket], &RouteTableDataTemp.RouteTableUsed[Socket], sizeof (RouteTableDataTemp.RouteTableUsed[Socket]));
  CopyMem (&RouteTableData->TgtRouteTable[Socket], &RouteTableDataTemp.TgtRouteTable[Socket], sizeof (RouteTableDataTemp.TgtRouteTable[Socket]));
  CopyMem (&RouteTableData->ChRouteTable[Socket], &RouteTableDataTemp.ChRouteTable[Socket], sizeof (RouteTableDataTemp.ChRouteTable[Socket]));
}


/**
  Check if UMA based clustering is enable/disable and apply memory decoder configuration settings.

  If UBC is disabled then Cluster XOR is disabled. If UBC is enabled then CHA route tables are adjusted.

  @param[in, out] MemMapData      Pointer to memory map private data struture

**/
VOID
ConfigureUmaBasedClustering (
  IN OUT MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8             Socket;
  MEMORY_MAP_HOST   *MemMapHost;
  UINT8             SysUmaClusteringLevel;
  UINT8             NewClusterLevel;

  if (MemMapData == NULL) {
    return;
  }

  MemMapHost = GetMemMapHostPointer ();
  SysUmaClusteringLevel = GetKtiOutputUmaClusterSetting ();

  if (IfFeatureEnabled (TypeSnc)) {
    NewClusterLevel = UMABASEDCLUSTERING_DISABLED;
  } else if (IsHbmInQuadrantMode (MemMapData, 0)) {
    NewClusterLevel = UMABASEDCLUSTERING_QUADRANT;
  } else {
    NewClusterLevel = MemMapData->UmaBasedClustersPerSocket;
  }
  //
  // Update the number of UMA based clusters for KTI RC consumption.
  //
  SetUmaBasedClustering (NewClusterLevel);

  //
  // If UBC is disabled then disable cluster XOR setting.
  //
  if (NewClusterLevel == UMABASEDCLUSTERING_DISABLED) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (!CheckSocketEnabled (Socket)) {
        continue;
      }
      MemMapHost->SocketInfo[Socket].XorMode &= ~CLUSTER_XOR;
    }

    if (SysUmaClusteringLevel != UMABASEDCLUSTERING_DISABLED) {
      for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
        if (!CheckSocketEnabled (Socket)) {
          continue;
        }
        if ((MemMapHost->SocketInfo[Socket].XorMode & CHANNEL_XOR) == 0) {
          RcDebugPrint (SDBG_MAX, "System is not in XOR interleave mode and cannot support UMA-Based clustering. Degrading DIMM interleave to %7a \n",
                        GetUbcTypeString (MemMapData->UmaBasedClustersPerSocket));

          return;
        }
      }

      if (IfFeatureEnabled (TypeSnc)) {
        RcDebugPrint (SDBG_MAX, "System is in SNC mode and cannot support UMA-Based clustering. Degrading DIMM interleave to %7a \n",
                      GetUbcTypeString (MemMapData->UmaBasedClustersPerSocket));
      } else {
        RcDebugPrint (SDBG_MAX, "Disabling UMA-Based Clustering. Degrading DIMM interleave to %7a \n", GetUbcTypeString (MemMapData->UmaBasedClustersPerSocket));
      }
    }

    return;
  }

  if (MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) {
    //
    // Single RT is fixed for volatile memory (1LM / 2LM / 1LM + 2LM)
    //
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if ((IsMemTypePresentSocketEnum (Socket, MemType1lmDdr)) ||
          (IsMemTypePresentSocketFunction (Socket, IsMemType2lmDdr))) {
        AdjustRouteTableForUmaBasedClustering (MemMapData, Socket, &MemMapData->DdrRouteTableData);
      }
      if ((IsMemTypePresentSocketEnum (Socket, MemType1lmAppDirect)) && IsUbcSupportedMemType (MemType1lmAppDirect)) {
        AdjustRouteTableForUmaBasedClustering (MemMapData, Socket, &MemMapData->DdrtRouteTableData);
      }
    }
  }

  RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Enabling system level UMA-Based Clustering: %7a\n", GetUbcTypeString (NewClusterLevel));

  return;
} // ConfigureUmaBasedClustering

/**
  Check whether the bit at the given BitOffset is set in the passed in Value.

  @param[in]   Value            The value need to be check.
  @param[in]   BitOffset        The bit offset to check.

  @retval  TRUE             The bit is set.
  @retval  FALSE            The bit is not set.
**/
BOOLEAN
IsBitOffsetSet (
  IN UINT32 Value,
  IN UINT32 BitOffset
  )
{
  return (BOOLEAN) ((Value & (BIT0 << BitOffset)) != 0);
}

/**
  Encode SAD interleave list for MCs within or across halves.

  @param[in]      Host            Pointer to the Host structure.
  @param[in]      MemMapData      Pointer to the memory map data structure.
  @param[in]      Socket          Socket index.
  @param[in]      SadIndex        Index of socket SAD table entry.
  @param[in]      MemType         Type of memory to populate the target list for.

  @return         Encoded interleave list value or value of zero if the SadEntry pointer is NULL
**/
UINT32
SadEncodeInterleaveList (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Socket,
  IN  UINT8           SadIndex,
  IN  MEM_TYPE        MemType
  )
{
  MEMORY_MAP_HOST                    *MemMapHost;
  SAD_TABLE                          *SadEntry;
  UINT32                             InterleaveList;
  BOOLEAN                            Found;
  UINT8                              HalfBitmap;
  UINT8                              TargetListIndex;
  UINT8                              McIndex;
  UINT8                              SingleTargetIndex;
  UINT8                              ChannelBitmap[MAX_IMC];
  UINT8                              SktWays;
  UINT8                              McWays;
  UINT8                              ChWays;
  UINT8                              InterleaveEntry;
  UINT8                              SadIntListEntry;
  ROUTE_TABLE_HALVES                 RtHalf;
  UINT8                              PkgCount;
  UINT8                              SocketIndex;
  UINT8                              ChIndex;
  UINT8                              MaxImc;
  UINT8                              McBitMap;

  MemMapHost = GetMemMapHostPointer ();
  HalfBitmap = 0;
  ZeroMem (ChannelBitmap, sizeof (ChannelBitmap));
  SingleTargetIndex = 0;
  InterleaveList = 0;
  McWays = 0;
  ChWays = 0;
  SktWays = 0;
  PkgCount = 0;

  MaxImc = GetMaxImc ();

  if (MemMapData == NULL || SadIndex >= MAX_SAD_INDEX) {
    return InterleaveList;
  }

  SadEntry = &Host->var.mem.socket[Socket].SAD[SadIndex];

  if (SadEntry == NULL) {
    return InterleaveList;
  }

  if (SadEntry->type == MemTypeNxm) {
    return 0xFFFFFFFF;
  }

  //
  // For remote target list in UMA interleaving fill the interleave list only with remote socket ID
  // SadEntry->local == 0 (Remote) is true only for UMA interleaving where the GP DRAM rules are
  // shared for both local and remote target lists.
  //
  if (SadEntry->local == 0) {
    do {
      for (McIndex = 0; McIndex < MC_MAX_NODE; McIndex++) {
        SadIntListEntry = MemMapHost->SocketInfo[Socket].SadIntList[SadIndex][McIndex];

        if (SadIntListEntry == 0) {
          continue;
        }

        InterleaveEntry = 0;

        //
        // Determine if this Mc is a remote target or local
        //
        if ((McIndex / MaxImc) != Socket) {
          //
          // BIT3 denotes remote or local target
          //
          InterleaveEntry |= (McIndex / MaxImc);   // BIT0, 1 & 2 denote socket number
          InterleaveList |= (InterleaveEntry << (4 * PkgCount));
          PkgCount++;
        }
      }
    } while (PkgCount < MAX_TGT_LIST_ENTRY);

    return InterleaveList;
  }

  GetInterleaveWays (MemMapData, Socket, SadIndex, &McWays, &ChWays, ChannelBitmap, &McBitMap);

  if (ChWays == 0 || McBitMap == 0 || ChannelBitmap == NULL) {
    return InterleaveList;
  }

  if ((MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) && IsUbcSupportedMemType (MemType)) {
    //
    // Only H0 is used when UMA-Based Clustering is enabled
    //
    HalfBitmap = (BIT0 << RtHalf0);
  } else {
    Found = FindRouteTable (
              MemMapData,
              Socket,
              SadEntry->type,
              SadEntry->Cluster,
              McBitMap,
              ChannelBitmap,
              &HalfBitmap,
              &SingleTargetIndex
              );

    if (!Found || HalfBitmap == 0) {
      return InterleaveList;
    }
  }

  //
  // Even in 2LM modes the target list is setup for FM
  //
  McWays = CalculateInterleaveWaysfromBitmap (SadEntry->imcInterBitmap);
  if (McWays == 0) {
    return InterleaveList;
  }

  SktWays = SadEntry->ways / McWays;

  PkgCount = 0;

  do {
    for (TargetListIndex = 0; TargetListIndex < MAX_TGT_LIST_ENTRY * MaxRtHalves; TargetListIndex++) {

      RtHalf = TargetListIndex % MaxRtHalves;
      SocketIndex = TargetListIndex / MaxRtHalves;

      if (MemMapData->MemMapSocket[Socket].UseRT0forAdx1 && (MemMapData->MemMapSocket[Socket].AppDirectByOneCount > AD_NONINTERLEAVE_CH_COUNT6)) {
        //
        // Only H0 is used by RT0 for ADx1
        //
        RtHalf = RtHalf0;
        HalfBitmap = (BIT0 << RtHalf0);
        if (MemMapData->MemMapSocket[Socket].AppDirectByOneCount == AD_NONINTERLEAVE_CH_COUNT7) {
          SingleTargetIndex = 0;
        }

        if (MemMapData->MemMapSocket[Socket].AppDirectByOneCount == AD_NONINTERLEAVE_CH_COUNT8) {
          SingleTargetIndex = 1;
        }
      }

      if (SocketIndex >= MAX_SOCKET) {
        break;
      }

      if (!IsBitOffsetSet (HalfBitmap, RtHalf)) {
        continue;
      }

      SadIntListEntry = 0;

      for (McIndex = 0; McIndex < MaxImc; McIndex++) {
        if ((SadEntry->imcInterBitmap & (BIT0 << McIndex)) == 0) {
          continue;
        }
        SadIntListEntry = (UINT8)MemMapHost->SocketInfo[Socket].SadIntList[SadIndex][(SocketIndex * MaxImc) + McIndex];

        if (SadIntListEntry != 0) {
          break;
        }
      }

      //
      // Skip if no Mc from the socket(1LM)/half is part of this interleave
      //
      if (SadIntListEntry == 0) {
        continue;
      }

      InterleaveEntry = 0;

      //
      // Determine if this MC is a remote socket or local memory controller
      //
      if (SocketIndex != Socket) {
        InterleaveEntry |= SocketIndex;
      } else {
        InterleaveEntry |=  BIT3;
        InterleaveEntry |= RtHalf;
        if (SingleTargetIndex != 0) {
          InterleaveEntry |= (SingleTargetIndex & 0x3) << 1;
        }
      }

      InterleaveList |= (InterleaveEntry << (4 * PkgCount));

      //
      // Handling 4MC-way interleave across memory modes
      //
      if (McWays == IMC_4WAY) {
        if (IsUbcSupportedMemType (MemType) && SktWays > ONE_WAY &&
            MemMapData->UmaBasedClustersPerSocket == UMABASEDCLUSTERING_QUADRANT) {
          PkgCount++;
          InterleaveList |= (InterleaveEntry << (4 * PkgCount));
        } else if (IsMemType2lm (SadEntry->type) && MemMapData->Is2LmUsingRt2lm && SktWays <= TWO_WAY) {
          PkgCount++;
          InterleaveList |= (InterleaveEntry << (4 * PkgCount));
        } else if ((SadEntry->type == MemType1lmAppDirect) && (MemMapData->DdrtIntlvGranularity != INTERLEAVE_GRANULARITY_256B)) {
          //
          // 10nm wave1/2 AD All2All
          //
          if (ChWays == TWO_WAY && SktWays == ONE_WAY) {
            for (ChIndex = 0; ChIndex < ChWays; ChIndex++) {
              PkgCount++;
              InterleaveList |= (InterleaveEntry << (4 * PkgCount));
            }
          }
          PkgCount++;
          InterleaveList |= (InterleaveEntry << (4 * PkgCount));
        }
      }
      //
      // For 4-way (2 MC * 2 Ch)
      //
      if ((McWays == IMC_2WAY && ChWays == TWO_WAY) &&
          (SadEntry->granularity == SadEntry->tgtGranularity)) {
        PkgCount++;
        InterleaveList |= (InterleaveEntry << (4 * PkgCount));
      }

      PkgCount++;
    } // TargetIndex
  } while (PkgCount < MAX_TGT_LIST_ENTRY);

  return InterleaveList;
} // SadEncodeInterleaveList

/** Perform POR compatibility checks for UMA.

  If the interleaving selected is UMA check the platform POR for UMA. When the POR compatibility fails change the
  memory interleaving policy to NUMA.

  @param[in] Host       - Pointers to global Host data structure.
  @param[in] MemMapData - The private data structure for memory map.

  @retval    None

**/
VOID
CheckUmaPorCompatibility (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  SECURITY_POLICY       *SecurityPolicy = NULL;
  UINT8                 Sckt;
  UINT8                 McIndex;
  UINT8                 MaxImc;
  UINT8                 ChIndex;
  UINT8                 NumChPerMc;
  UINT8                 NumSckt;
  UINT8                 NumScktPopulated;
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];
  UINT8                 McBitmap[MAX_SOCKET];
  UINT8                 ChBitmap [MAX_SOCKET][MAX_IMC];
  UINT8                 FirstScktWithMem;
  SOCKET_INFO_STRUCT    *Socket;

  //
  // If the user policy is NUMA interleaving then there is nothing special to do.
  //
  if (MemMapData->IsNumaEn) {
    return;
  }

  //
  // should not require this path when HBM
  // sku is properly detected and IsNumaEn
  // is correctly initialized
  //
  if (IsHbmEnabled (MemMapData)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Cannot support UMA interleave when HBM is enabled. Switching to NUMA memory interleaving\n");
    MemMapData->IsNumaEn = TRUE;
    return;
  }

  if (Host->var.mem.volMemMode != VOL_MEM_MODE_1LM) {
    MemMapData->IsNumaEn = TRUE;
    return;
  }

  MaxImc = GetMaxImc ();

  NumChPerMc = GetNumChannelPerMc ();

  NumSckt = CalculateInterleaveWaysfromBitmap ((UINT8) GetSocketPresentBitMap ());
  ZeroMem (McBitmap, sizeof (McBitmap));
  ZeroMem (ChBitmap, sizeof (ChBitmap));
  FirstScktWithMem = MAX_SOCKET;
  NumScktPopulated = 0;

  //
  // If the system configuration has only 1S populated then there can be no UMA interleaving possibility.
  //
  if (NumSckt == ONE_WAY) {
    MemMapData->IsNumaEn = TRUE;
    return;
  }

  //
  // If UMA interleaving is the user selection, check additonal POR's here
  // 1. When SGX is enabled force interleaving to NUMA
  // 2. Allow only 1LM 2S UMA
  // 3. Ensure that sockets have memory populated uniformly
  //
  SecurityPolicy = GetSecurityPolicy ();
  ASSERT (SecurityPolicy != NULL);
  if ((SecurityPolicy != NULL) && SecurityPolicy->EnableSgx != SECURITY_POLICY_DISABLE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Cannot support UMA interleave when SGX is enabled. Switching to NUMA memory interleaving\n");
    MemMapData->IsNumaEn = TRUE;
    return;
  }

  //
  // Adding check to return if more than 2 sockets populated for UMA.
  //
  if (NumSckt > TWO_WAY) {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Cannot support UMA interleave greater than 2-Socket ways. Switching to NUMA memory interleaving\n");
    MemMapData->IsNumaEn = TRUE;
    return;
  }

  //
  // UMA interleaving necessitate DIMM channel location and channel size symmetry check for interleaves across sockets.
  //
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {

    //
    // Skip if socket disabled or no 1LM volatile memory present
    //
    if ((CheckSocketEnabled (Sckt) == 0) || (GetSocketMemSize (Sckt) == 0)) {
      continue;
    }

    NumScktPopulated++;
    Socket = &Host->var.mem.socket[Sckt];
    ChannelNvList = &Host->nvram.mem.socket[Sckt].channelList;

    //
    // Construct bitmaps for channels and MCs poopulated per socket.
    //
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if (!CheckSocketMcEnabled (Sckt, McIndex)) {
        continue;
      }
      McBitmap[Sckt] |= (BIT0 << McIndex);

      for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
        if (((*ChannelNvList)[ChIndex + (McIndex * NumChPerMc)].enabled == 0) ||
            (GetChannelMemSize (Sckt, (ChIndex + (McIndex * NumChPerMc))) == 0)) {
          continue;
        }
        ChBitmap[Sckt][McIndex] |= (BIT0 << ChIndex);

        if (FirstScktWithMem == MAX_SOCKET) {
          FirstScktWithMem = Sckt;
        }
      } // ChIndex
    } // McIndex

    if (FirstScktWithMem >= MAX_SOCKET) {
      return;
    }

    //
    // Validate MC and channel bitmaps symmetry across populated sockets
    //
    if (McBitmap[Sckt] != McBitmap[FirstScktWithMem]) {
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "***Cannot support memory UMA interleave when populated MCs is non-uniform across sockets."
        " Switching to NUMA memory interleaving. UMA imposes strict memory symmetry across sockets.***\n");
      MemMapData->IsNumaEn = TRUE;
      return;
    }

    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if (ChBitmap[Sckt][McIndex] != ChBitmap[FirstScktWithMem][McIndex]) {
        RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "***Cannot support memory UMA interleave when populated channels in MCs is non-uniform across sockets."
          " Switching to NUMA memory interleaving. UMA imposes strict memory symmetry across sockets.***\n");
        MemMapData->IsNumaEn = TRUE;
        return;
      }
    } // McIndex

    //
    // Validate channel sizes symmetry across populated sockets
    //
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if (!(McBitmap[Sckt] & (1 << McIndex))) {
        continue;
      }
      for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
        if (!(ChBitmap[Sckt][McIndex] & (1 << ChIndex))) {
        continue;
        }
        //
        // Check channel sizes of only channels populated in each MC and socket
        //
        if (GetChannelMemSize (Sckt, ((McIndex * NumChPerMc) + ChIndex)) !=
            GetChannelMemSize (FirstScktWithMem, ((McIndex * NumChPerMc) + ChIndex))) {
          RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "***Cannot support UMA interleave when populated channel sizes of each channel is not uniform across sockets."
            " Switching to NUMA memory interleaving. UMA imposes strict memory symmetry across sockets.***\n");
          MemMapData->IsNumaEn = TRUE;
          return;
        }
      } // ChIndex
    } // McIndex
  } // Sckt

  //
  // Check number of sockets with memory populated
  //
  if (NumScktPopulated != NumSckt) {
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Cannot support UMA interleave unless all sockets have memory. Switching to NUMA memory interleaving\n");
    MemMapData->IsNumaEn = TRUE;
    return;
  }
}

/** Interleave memory across sockets.

  This routine figures out the mapping and fills in the structures for the SAD tables.  It does not program any registers.

  @param[in]  Host       - Pointer to sysHost
  @param[in]  MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in]  MemType    - Type of memory to interleave

  @retval     SUCCESS               -  Interleave completed.
  @retval     FAILURE               -  Interleave could not be supported for input memory type.

**/
UINT32
SadUma (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN MEM_TYPE         MemType
  )
{
  UINT32   Status;
  UINT8    SocketNum;
  UINT8    SocketBitmap;

  Status = SUCCESS;
  SocketBitmap = 0;

  for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {
    if (CheckSocketEnabled (SocketNum) == 0) {
      continue;
    }
    SocketBitmap |= (BIT0 << SocketNum);
  }
  Status = SadInterleave (Host, MemMapData, SocketBitmap, 0, MemType);
  if (Status != SUCCESS) {
    return Status;
  }

  //
  // For each socket interleave (within every socket) until all the remaining memory is mapped.
  //
  for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {
    if (CheckSocketEnabled (SocketNum) == 0) {
      continue;
    }
    Status = SadInterleave (Host, MemMapData, (BIT0 << SocketNum), 0, MemType);
    if (Status != SUCCESS) {
      return Status;
    }

    Status = ProcessCxlMemDevices (MemMapData, SocketNum);
    if (Status != SUCCESS) {
      return Status;
    }
  }

  return Status;
}

/** Interleave memory on the MC and channel level but not across sockets for NUMA configuration.

  This routine figures out the mapping and fills in the structures for the SAD and TAD
  tables.  It does not program any registers.

  @param[in]  Host        - Pointer to sysHost
  @param[in]  MemMapData  - Pointer to the memory map private data structure
  @param[in]  SocketNum   - Socket number
  @param[in]  MemType     - Type of memory being interleaved
  @param[in]  Cluster     - SNC cluster number

  @retval SUCCESS      -  Memory interleaved.
  @retval FAILURE      -  Interleave could not be supported for input memory type.

**/
UINT32
SadNuma (
  IN PSYSHOST         Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketNum,
  IN MEM_TYPE         MemType,
  IN UINT8            Cluster
  )
{
  UINT32  Status = SUCCESS;

  Status = SadInterleave (Host, MemMapData, (BIT0 << SocketNum), Cluster, MemType);

  return Status;
} // SadNuma

/**

  This routine Creates SAD rules for the memory to be mapped into the system address space.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval    Status

**/
UINT32
CreateSadRules (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
#ifdef DDRT_SUPPORT
  UINT8      SocketNum;
#endif // DDRT_SUPPORT
  UINT32     Status = SUCCESS;

  //
  // Device CXL memory devices before SAD interleave.
  //
  DiscoverCxlMemDevices (MemMapData);

  //
  // Before beginning to setup the SAD interleave clear the route tables.
  //
  ClearRouteTables (Host, MemMapData);

  //
  // Before beginning to interleave determine the platform POR's for UMA
  //
  CheckUmaPorCompatibility (Host, MemMapData);

  InitializeSecurityData (MemMapData);

  ReInitializeMemAddrMapInterleavingValues (Host);

  if (!MemMapData->IsNumaEn) {
    //
    // "UMA Interleave"
    // Map the volatile memory present in the system (SAD). (Also, PMEM/PMEM$ if no CR mgmt driver present - SV only)
    //
    Status = SocketInterleave (Host, MemMapData);
    if (Status != SUCCESS) {
      return Status;
    }

#ifdef DDRT_SUPPORT
    //
    // Map the non-volatile memory if CR mgmt driver present
    //
    if (Host->var.mem.dfxMemVars.dfxDimmManagement == DIMM_MGMT_CR_MGMT_DRIVER) {
      for (SocketNum = 0; SocketNum < MAX_SOCKET; SocketNum++) {
        if (CheckSocketEnabled (SocketNum) == 0) {
          continue;
        }

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
          if (Status != SUCCESS) {
            return Status;
          }
        }
      }
    }
#endif // DDRT_SUPPORT
  } else {
    //
    // "NUMA Interleave"
    // Map the volatile AND non-volatile memory present in the system (SAD)
    //
    Status = SocketNonInterleave (Host, MemMapData);
    if (Status != SUCCESS) {
      return Status;
    }
  }

  //
  // Create SAD entries for FPGA present in the system
  //
  Status = CreateNewRegionSadFPGA (Host, MemMapData, MemTypeFpga);
  if (Status != SUCCESS) {
    return Status;
  }

#ifdef DDRT_SUPPORT
  EnableBlockDecoder (Host, MemMapData);
  CheckFadrSupported (Host);
#endif // DDRT_SUPPORT

  ConfigureUmaBasedClustering (MemMapData);

  return SUCCESS;
}

/**

  Interleave volatile (DDR/HBM) memory on IMC and channel level but not across sockets for NUMA configuration.

  @param[in] Host                - Pointer to sysHost
  @param[in] MemMapData          - Pointer to MemMapData (Private DS maintained by Memory decoder)
  @param[in] socketNum           - Socket number

  @retval Status

**/
UINT32
SocketNonInterleaveVolMem (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           socketNum
  )
{
  MEM_TYPE MemType;
  UINT32   Status;
  UINT8    EndCluster;
  UINT8    Cluster;
  UINTN    Tracker;

  Status = SUCCESS;

  if (IfFeatureEnabled (TypeSnc)) {
    EndCluster = GetNumOfClusterPerSystem ();

#ifdef DDRT_SUPPORT
    CalculateXTile2lmSncFarMemoryPartition (Host, MemMapData, socketNum);
#endif // DDRT_SUPPORT
  } else {
    EndCluster = 1;
  }

  MemType = 0;

  for (Cluster = 0; Cluster < EndCluster; Cluster++) {
    Tracker = 0;
    while (TRUE) {
      MemType = GetNextVolMemTypeToMap (
                  Host,
                  MemMapData,
                  socketNum,
                  &Tracker
                  );
      if (MemType == 0) {
        break;
      }

      if (MemType == MemType1lmHbm) {
        Status = (UINT32) CreateHbmFlatSadRule (
                   MemMapData,
                   socketNum,
                   FindSadIndex (Host, MemMapData, socketNum, Cluster),
                   Cluster
                   );
        if (Status != SUCCESS) {
          return Status;
        }
      } else {

        if (PcdGetBool (PcdNvDimmEn)) {
          // Interleave Normal memory dimms first and then NvMem
          Status = SocketNonInterleaveNormalMem (Host, MemMapData, socketNum, MemType, Cluster);
          if (Status != SUCCESS) {
            return Status;
          }

          Status = SocketNonInterleaveNVMem (Host, MemMapData, socketNum, MemType, Cluster);
          if (Status != SUCCESS) {
            return Status;
          }
        } else {
          Status = SadNuma (Host, MemMapData, socketNum, MemType, Cluster);
          if (Status != SUCCESS) {
            return Status;
          }
        }
      }
      // Add remote Dram rule after interleaving each volatile memory type from this socket
      Status = AddRemoteSadEntry (
                 Host,
                 MemMapData,
                 socketNum,
                 MemType,
                 Cluster
                 );
      if (Status != SUCCESS) {
        return Status;
      }
    }
  }


  return Status;
}

/**
  Get the TAD Rule Target Granularity for the input SadIndex

  @param[in]  MemMapData - Pointer to the memory map private data structure.
  @param[in]  Sckt       - The socket index for the input SadIndex.
  @param[in]  SadIndex   - The SadIndex corresponding to the TAD rule.
  @param[out] TargetGran - Pointer to a variable where the Target Granularity will be retuned.

  @retval SUCCESS The Target Granularity was successfully calculated
  @retval FAILURE The operation failed due to an error
**/
UINT32
EFIAPI
GetTadRuleTargetGranularity (
  IN     MEMORY_MAP_DATA    *MemMapData,
  IN     UINT8              Sckt,
  IN     UINT8              SadIndex,
     OUT UINT8              *TargetGran
  )
{
  SYSHOST            *Host;
  SOCKET_INFO_STRUCT *Socket;

  if (TargetGran == NULL) {
    return FAILURE;
  }

  Host = GetSysHostPointer ();
  Socket = &Host->var.mem.socket[Sckt];

  //
  // Target granularity for UBC supported memory type
  //
  if ((MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) && (IsUbcSupportedMemType (Socket->SAD[SadIndex].type))) {
      *TargetGran = INTERLEAVE_GRANULARITY_2KB;
  } else { // All2all/SNC/UBC unsupported memory type
    if ((Socket->SAD[SadIndex].type == MemType1lmDdr) || (Socket->SAD[SadIndex].type == MemType1lmHbm)) {
      //
      // 1LM DDR and HBM have the same channel and MC granularity. The target is at socket level.
      //
      *TargetGran = Socket->SAD[SadIndex].tgtGranularity;
#ifdef DDRT_SUPPORT
    } else if (IsMemTypeDdrt (Socket->SAD[SadIndex].type)) {
      if (MemMapData->DdrtIntlvGranularity == INTERLEAVE_GRANULARITY_256B) {
        *TargetGran = Socket->SAD[SadIndex].tgtGranularity;
      } else {
        *TargetGran = INTERLEAVE_GRANULARITY_4KB;
      }
#endif // DDRT_SUPPORT
    } else {
      RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "INVALID TAD TYPE.\n");
      return FAILURE;
    }
  }

  return SUCCESS;
}

/**
  Get the TAD Rule Channel Granularity for the input SadIndex

  @param[in]  MemMapData - Pointer to the memory map private data structure.
  @param[in]  Sckt       - The socket index for the input SadIndex.
  @param[in]  SadType    - The type of memory corresponding to the TAD rule.
  @param[out] ChGran     - Pointer to a variable where the Channel Granularity will be retuned.

  @retval SUCCESS The Channel Granularity was successfully calculated
  @retval FAILURE The operation failed due to an error
**/
UINT32
GetTadRuleChannelGranularity (
  IN     MEMORY_MAP_DATA    *MemMapData,
  IN     UINT8              Sckt,
  IN     MEM_TYPE           SadType,
  OUT    UINT8              *ChGran
)
{
  if (ChGran == NULL) {
    return FAILURE;
  }

#ifdef DDRT_SUPPORT
  if (IsMemTypeDdrt (SadType)) {
    if ((MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) &&
        (IsUbcSupportedMemType (SadType)) &&
        (MemMapData->DdrtIntlvGranularity != INTERLEAVE_GRANULARITY_256B)) {
      *ChGran = INTERLEAVE_GRANULARITY_2KB;
    } else {
      *ChGran = MemMapData->DdrtIntlvGranularity;
    }
  } else {
#endif // DDRT_SUPPORT
    *ChGran = INTERLEAVE_GRANULARITY_256B;
#ifdef DDRT_SUPPORT
  }
#endif // DDRT_SUPPORT

  return SUCCESS;
}

/**
  Populate channel interleaved memory sizes for 1LM and 2LM NM to program patrol scrub interval.
  Assumes SAD Index is not for HBM 2LM

  @param[in] Host            Pointer to the sysHost structure.
  @param[in] MemMapData      Pointer to memory map private data structure.
  @param[in] Socket          Socket index.
  @param[in] SadIndex        SAD index.
  @param[in] InterSizePerCh  Interleaave size per channel corresponding to the SAD interleave data. Used only for 1LM DDR range.

  @retval SUCCESS            Interleaved channel memory sizes populated successfully.
  @retval FAILURE            SAD input does not correspond to volatile memory interleave.
**/
UINT32
PopulateChInterSizeForPatrolScrub (
  IN PSYSHOST             Host,
  IN MEMORY_MAP_DATA      *MemMapData,
  IN UINT8                Socket,
  IN UINT8                SadIndex,
  IN UINT32               InterSizePerCh
  )
{
  UINT8                         MaxImc;
  UINT8                         NumChPerMc;
  SAD_TABLE                     *SadEntry;
  UINT8                         McIndex;
  UINT8                         ChIndex;
  UINT32                        MinNmSizePerCh;
  MEMORY_MAP_HOST               *MemMapHost;

  if (MemMapData->MemMapState != MEM_MAP_STATE_RESOURCE_CALCULATION_DONE) {
    return SUCCESS;
  }

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();
  SadEntry = &Host->var.mem.socket[Socket].SAD[SadIndex];
  MemMapHost = GetMemMapHostPointer ();
  MinNmSizePerCh = 0;

  if (!IsMemTypeVolatile  (SadEntry->type)) {
    return FAILURE;
  }

  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if ((SadEntry->NmImcInterBitmap & (BIT0 << McIndex)) == 0) {
      continue;
    }

    if (IsMemType2lm (SadEntry->type) && IsMemTypePresentMcFunction (Socket, McIndex, IsMemType2lm)) {
      MinNmSizePerCh = FindMinNmSize (Host, MemMapData, Socket, McIndex);
    }

    for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
      if ((SadEntry->channelInterBitmap[McIndex] & (BIT0 << ChIndex)) == 0) {
        continue;
      }
      //
      // For a 1LM only memory mode, the DDR memory can be split into multiple interleaves. The channel interleave size is cumulative.
      // For a 2LM only memory mode, the NM$ is always fully interleaved. Therefore, there can be only one channel size assigned.
      // For a 1LM + 2LM, the channel size must include DDR 1LM interleave size and cache size.
      //
      if (IsMemType2lm (SadEntry->type)) {
        if (Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
          MemMapHost->SocketInfo[Socket].ChannelInfo[NODECH_TO_SKTCH (McIndex, ChIndex)].VolIntlvSizePerCh += MinNmSizePerCh;
        } else {
          MemMapHost->SocketInfo[Socket].ChannelInfo[NODECH_TO_SKTCH (McIndex, ChIndex)].VolIntlvSizePerCh = MinNmSizePerCh;
        }
      } else {
        MemMapHost->SocketInfo[Socket].ChannelInfo[NODECH_TO_SKTCH (McIndex, ChIndex)].VolIntlvSizePerCh += InterSizePerCh;
      }
    } // ChIndex
  } // McIndex

  return SUCCESS;
}

/**
  Create a NM TAD to decode channel ways and target ways for patrol and sparing.

  This function allocates a TAD entry dedicated to 2LM NM to support patrol/sparing.

  @param[in] Host          Pointer to the sysHost structure.
  @param[in] MemMapData    Pointer to memory map private data structure.
  @param[in] Socket        Socket index.
  @param[in] SadIndex      SAD index.

  @retval SUCCESS                               TAD rule is allocated successfully.
  @retval ERROR_RESOURCE_CALCULATION_FAILURE    All TAD rules used before all memory allocated.
**/
UINT32
CreateNmTadForPatrolSpare (
  IN PSYSHOST             Host,
  IN MEMORY_MAP_DATA      *MemMapData,
  IN UINT8                Socket,
  IN UINT8                McIndex,
  IN UINT8                SadIndex
  )
{
  UINT8                  TadIndex;
  TAD_TABLE              *TadEntry;
  SAD_TABLE              *SadEntry;
  UINT32                 Status;

  SadEntry = &Host->var.mem.socket[Socket].SAD[SadIndex];

  if (!IsMemType2lm (SadEntry->type) || ((SadEntry->NmImcInterBitmap & (1 << McIndex)) == 0)) {
      return FAILURE;
  }

  //
  // Get the index of next TAD to be filled.
  //
  TadIndex = FindTadIndex (Host, Socket, McIndex);

  //
  // Issue warning if all TAD rules used before all memory allocated.
  //
  if (TadIndex == 0xFF) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "TAD rules exceeded\n");
    OutputWarning (WARN_INTERLEAVE_FAILURE, WARN_TAD_RULES_EXCEEDED, Socket, 0xFF, 0xFF, 0xFF);
    return ERROR_RESOURCE_CALCULATION_FAILURE;
  }

  //
  // Create a TAD entry.
  //
  TadEntry = &Host->var.mem.socket[Socket].imc[McIndex].TAD[TadIndex];
  TadEntry->Enable = 1;
  TadEntry->SADId = SadIndex;

  Status = GetTadRuleTargetGranularity (MemMapData, Socket, SadIndex, &TadEntry->TargetGran);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // NM$ can support only 256B channel granularity.
  //
  TadEntry->ChGran = SadEntry->granularity;

  return SUCCESS;
}

/**
  TAD (Channel) Interleave.

  Interleaves memory on the channel level within a socket. This routine figures out the mapping and fills in the
  structures TAD table.  It does not program any registers. Assume all SAD entries are sorted correctly at this point.

  @param[in] MemMapData    Pointer to the memory map private data structure.
  @param[in] Socket        Socket index.
  @param[in] SadIndex      SAD index.
  @param[in] ClusterIndex  Cluster index.

  @retval SUCCESS - Memory interleaved.
  @retval FAILURE - TAD rules exceeded.

**/
UINT32
TadInterleave (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Sckt,
  IN  UINT8           SadIndex,
  IN  UINT8           ClusterId
  )
{
  SYSHOST            *Host;
  UINT8              Index;
  UINT8              McIndex;
  UINT8              ChannelWays;
  UINT8              TadIndex;
  UINT8              ImcWays;
  UINT8              Dimm;
  UINT8              TadLimitAdjustment = 0;
  UINT8              TotalWays;
  UINT8              ChInter = 0;
  UINT8              MaxImc;
  UINT8              RasMode;
  UINT8              MaxChPerMc;
  KTI_HOST_IN        *KtiHostInPtr;
  TAD_TABLE          *TadEntry;
  INT32              (*TadOffset)[MAX_TAD_WAYS];
  UINT8              (*TadChnIndex)[MAX_TAD_WAYS];
  UINT8              (*TadIntList)[MAX_TAD_WAYS];

  UINT32             SadInterleaveSize;
  UINT32             InterSizePerCh = 0;
  UINT32             ChannelMemUsed = 0;
  UINT32             TadLimit;
  UINT32             DpaOffset = 0xFFFFFFFF;
#ifdef DDRT_SUPPORT
  UINT32             InterReqOffset = 0;
  UINT32             WbCachePerRemSize;
#endif // DDRT_SUPPORT
  MEM_VAR_STRUCT     *Mem;
  SOCKET_INFO_STRUCT *Socket;
  DIMM_NVRAM_STRUCT  (*DimmNvList)[MAX_DIMM];
  IMC_INFO_STRUCT    *Imc;
  UINT16             Tolm;
  UINT8              McWays;
  UINT32             Status;

  Host = GetSysHostPointer ();
  Tolm = GetTolmAddress ();
  KtiHostInPtr = KTI_HOST_IN_PTR;
  Mem = &Host->var.mem;
  Socket = &Mem->socket[Sckt];
  Status = SUCCESS;
  TadIndex = 0;
  ChannelWays = 0;
  MaxImc = GetMaxImc ();
  RasMode = GetRasMode ();
  MaxChPerMc = GetNumChannelPerMc ();

  TadEntry = NULL;
  TadOffset = NULL;
  TadChnIndex = NULL;
  TadIntList = NULL;

  ChannelWays = GetChannelWays (&Socket->SAD[SadIndex]);
  if (ChannelWays == 0) {
    return FAILURE;
  }

  ImcWays = Socket->SAD[SadIndex].ways;
  SadInterleaveSize = Socket->SAD[SadIndex].Limit - Socket->SAD[SadIndex].Base;

  //
  // Adjust SadInterleaveSize for Memory hole
  //
  if ((Socket->SAD[SadIndex].Limit > Tolm) && (Socket->SAD[SadIndex].Limit == FOUR_GB_MEM)) {
    if (SadIndex == 0) {

      SadInterleaveSize -= GetLowGap ();
      TadLimitAdjustment = 0;

      if (ChannelWays == 3) {
        //
        // If TOLM boundary is not evenly divisible, then we need to adjust the SadInterleaveSize so that it divides evenly
        //
        TotalWays = ImcWays * ChannelWays;
        if (Tolm % TotalWays) {
          //
          // 5332821: Partial Mirroring with 3Chway population causes Memicals data miscompare
          //
          if ((RasMode & PARTIAL_MIRROR_1LM) || (RasMode & PARTIAL_MIRROR_2LM)) {
            if (ImcWays == 1) {
              TotalWays = TotalWays * 2;
             }
          }

          SadInterleaveSize = SadInterleaveSize + (TotalWays - (Tolm % TotalWays));
          if (Socket->SAD[SadIndex].type == MemType1lmDdr) {
            SadInterleaveSize += MemMapData->SadLimitAdjustment;
          }
          TadLimitAdjustment = TotalWays - (Tolm % TotalWays);

          // Adjusted limit is a multiple of mem granularity, but not necessarily SAD/TAD granularity, so adjust it.
          if (SadInterleaveSize % (GetSadTadGranularity (Host) / MEM_GRANULARITY)) {
              SadInterleaveSize += TotalWays;
              TadLimitAdjustment += TotalWays;
          }
        }
      }
    }
  }

  //
  // Calculate true interleaved sizes for each Channel.
  //
  InterSizePerCh = SadInterleaveSize / (ImcWays * ChannelWays);
  if (Socket->SAD[SadIndex].mirrored) {
    InterSizePerCh = InterSizePerCh * 2;
  }

  //
  // Populate channel interleave sizes for DDR 1LM and 2LM to program Patrol/Scrub interval.
  //
  if (IsMemTypeVolatile (Socket->SAD[SadIndex].type) && !IsMemType2lmHbm (Socket->SAD[SadIndex].type)) {
    Status = PopulateChInterSizeForPatrolScrub (Host, MemMapData, Sckt, SadIndex, InterSizePerCh);
    if (Status != SUCCESS) {
      return Status;
    }
  }
  McWays = CalculateInterleaveWaysfromBitmap (Socket->SAD[SadIndex].imcInterBitmap);
  if (McWays == 0) {
    return FAILURE;
  }

  //
  // Fill the TAD table for each MC in the Socket.
  //
  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if ((Socket->SAD[SadIndex].imcInterBitmap & (BIT0 << McIndex)) == 0) {
      continue;
    }

    Imc = &Mem->socket[Sckt].imc[McIndex];
    //
    // Get the index of next TAD to be filled.
    //
    TadIndex = FindTadIndex (Host, Sckt, McIndex);
    //
    // Issue warning if all TAD rules used before all memory allocated.
    //
    if (TadIndex == 0xFF) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "TAD rules exeeded\n");
      OutputWarning (WARN_INTERLEAVE_FAILURE, WARN_TAD_RULES_EXCEEDED, Sckt, 0xFF, 0xFF, 0xFF);
      return ERROR_RESOURCE_CALCULATION_FAILURE;
    }
    TadEntry = &Imc->TAD[TadIndex];
    TadOffset = &Imc->TADOffset[TadIndex];
    TadChnIndex = &Imc->TADChnIndex[TadIndex];
    TadIntList = &Imc->TADintList[TadIndex];

    //
    // Adjust TAD limit as needed for memory hole.
    //
    TadLimit = Socket->SAD[SadIndex].Limit;
    if ((Socket->SAD[SadIndex].Limit > Tolm) && (Socket->SAD[SadIndex].Limit == FOUR_GB_MEM)) {
      if (SadIndex == 0) {
        TadLimit = Socket->SAD[SadIndex].Limit - GetLowGap () + TadLimitAdjustment;
        MemMapData->TadLimitAdjustment = TadLimitAdjustment;
        MemMapData->Below4GBSktIndex = Sckt;
      }
    }
    //
    // Get the channel bitmaps from this MC.
    //
    ChInter = GetChannelInterBitmap (&Socket->SAD[SadIndex], McIndex);

    if (ChInter == 0) {
      continue;
    }

    //
    // Create a TAD entry.
    //
    TadEntry->Enable = 1;
    TadEntry->SADId = SadIndex;
    TadEntry->Limit = TadLimit;
    if ((Socket->SAD[SadIndex].Limit > Tolm) && (Socket->SAD[SadIndex].Limit == FOUR_GB_MEM)) {
      if ((SadIndex == 0) && (Socket->SAD[SadIndex].type == MemType1lmDdr)) {
        //
        // Workaround for Uncorrectable errors detected when part of the DRAM memory is lost due to MMIO hole
        // and GB alignment of SAD limits.
        //
        TadLimit += MemMapData->SadLimitAdjustment;
      }
    }
    TadEntry->socketWays = Socket->SAD[SadIndex].ways;

    Status = GetTadRuleTargetGranularity (
      MemMapData,
      Sckt,
      SadIndex,
      &TadEntry->TargetGran
      );

    if (Status != SUCCESS) {
      return Status;
    }

    Status = GetTadRuleChannelGranularity (
      MemMapData,
      Sckt,
      Socket->SAD[SadIndex].type,
      &TadEntry->ChGran
      );

    if (Status != SUCCESS) {
      return Status;
    }

    //
    // Update the Offsets for different memory regions for the first SAD of that type.
    //
    for (Index = 0; Index < MaxChPerMc; Index++) {
      //
      // Reset the offset for this channel.
      //
      DpaOffset = 0xFFFFFFFF;
      if (ChInter & (BIT0 << Index)) {
        DimmNvList = GetDimmNvList (Host, Sckt, Index + (McIndex * MaxChPerMc));
        //
        // Parse through the dimms in the channel to find the NVM Dimm and fill its Vol/Per Region DPA.
        //
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          //
          // Skip if DpaOffset is already calculated for this channel.
          //
          if (DpaOffset != 0xFFFFFFFF) {
            continue;
          }
          //
          // Skip if Dimm slot not populated.
          //
          if (((*DimmNvList)[Dimm].dimmPresent == 0)) {
            continue;
          }
          //
          // Update the proper DPA of this Dimm as TAD offset for TAD 0 in that channel based on memory type.
          //
          switch (Socket->SAD[SadIndex].type) {
            case MemType1lmDdr:
            case MemType2lmHbmCacheDdr:
              if (Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
                //
                // For 1LM+2LM mixed mode, we need to exclude DDrCacheSize since it is also mapped to CA space.
                //
                DpaOffset = Host->var.mem.socket[Sckt].channelList[Index].DdrCacheSize;
              } else{
                DpaOffset = 0;
              }
              if (PcdGetBool (PcdNvDimmEn)) {
                if ((MemMapData->MemMapSocket[Sckt].NvMemSadIdxBitMap[ClusterId] >> SadIndex) & 1) {
                  RcDebugPrintWithDevice (SDBG_MAX, Sckt, (Index + (McIndex * MaxChPerMc)), Dimm, NO_SUBCH, NO_RANK, NO_STROBE,
                                          NO_BIT, "SadIndex %d is NVDIMM Sad...\n", SadIndex);
                  //
                  // If this is the first time this NVDIMM SAD is being mapped, make sure ddr4RemSize is reset.
                  // This is required as some of the 3 way interleaving of RDIMMs will not be equally distributed
                  // among channels. So when NVDIMM SAD region is decoded, it could wrongly point to RDIMM on the
                  // channel instead of NVDIMM.Resetting ddr4Remsize to NVmemSize will make sure that TADOffsets
                  // are computed correctly to point to NVDIMMs on the channel.
                  //
                  if (Socket->channelList[Index + (McIndex * MaxChPerMc)].ddr4RemSize >
                      Socket->channelList[Index + (McIndex * MaxChPerMc)].NVmemSize) {
                        Socket->channelList[Index + (McIndex * MaxChPerMc)].ddr4RemSize =
                        Socket->channelList[Index + (McIndex * MaxChPerMc)].NVmemSize;
                        RcDebugPrintWithDevice (SDBG_MAX, Sckt, (Index + (McIndex * MaxChPerMc)), Dimm, NO_SUBCH,
                                                NO_RANK, NO_STROBE, NO_BIT, "Adjusted ddr4RemSize = 0x%x.\n",
                                                Socket->channelList[Index + (McIndex * MaxChPerMc)].ddr4RemSize);
                  }
                }
              }
              ChannelMemUsed = (GetChannelMemSize (Sckt, Index + (McIndex * MaxChPerMc)) - Socket->channelList[Index + (McIndex * MaxChPerMc)].ddr4RemSize) + InterSizePerCh + DpaOffset;

              //
              // Update remaining size on channel.
              //
              Socket->channelList[Index + (McIndex * MaxChPerMc)].ddr4RemSize -= (UINT16)InterSizePerCh;
              break;

#ifdef DDRT_SUPPORT
            case MemType2lmDdrCacheMemoryMode:
            case MemType2lmHbmCacheMemoryMode:
              if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
                break;
              }
              DpaOffset = (*DimmNvList)[Dimm].volRegionDPA;
              //
              // If the system is running in 1LM + 2LM
              // Find if there is another DDR DIMM in this channel that has already been mapped.
              // If so, find the size of the memory that is already mapped from this channel and add it to the offset
              // This will ensure that the channel address decoded using the Tad Offset will be correct.
              //
              DpaOffset = ComputeChannelOffset (Host, DpaOffset, Sckt, Index + (McIndex * MaxChPerMc));
              ChannelMemUsed = (Socket->channelList[Index + (McIndex * MaxChPerMc)].volSize - Socket->channelList[Index + (McIndex * MaxChPerMc)].volRemSize) + InterSizePerCh + DpaOffset;
              //
              // Update remaining size on channel.
              //
              Socket->channelList[Index + (McIndex * MaxChPerMc)].volRemSize -= (UINT16)InterSizePerCh;
              break;

            case MemType1lmAppDirect:
            case MemType1lmAppDirectReserved:
              if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
                break;
              }
              if (InterSizePerCh > Socket->channelList[Index + (McIndex * MaxChPerMc)].perRemSize) {
                //
                // Not enough space, generate error.
                //
                RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "AD: Insufficient space on Dimm\n");
                OutputError (ERR_NGN, NGN_PMEM_CONFIG_ERROR, Sckt, 0xFF, 0xFF, 0xFF);
                return ERROR_RESOURCE_CALCULATION_FAILURE;
              }
              //
              // Get the offset (within Persistant range) specified in the interleave request.
              //
              InterReqOffset = GetInterReqOffset (Host, Sckt, McIndex, Index, Dimm, Socket->SAD[SadIndex].Limit);
              DpaOffset = (*DimmNvList)[Dimm].perRegionDPA + InterReqOffset;
              //
              // If the system is running in 1LM + PMEM
              // Find if there is another DDR DIMM in this channel that has already been mapped.
              // If so, find the size of the memory that is already mapped from this channel and add it to the offset
              // This will ensure that the channel address decoded using the Tad Offset will be correct.
              //
              DpaOffset = ComputeChannelOffset (Host, DpaOffset, Sckt, Index + (McIndex * MaxChPerMc));
              ChannelMemUsed = (Socket->channelList[Index + (McIndex * MaxChPerMc)].perSize - Socket->channelList[Index + (McIndex * MaxChPerMc)].perRemSize) + InterSizePerCh + DpaOffset;
              //
              // Update remaining size on channel.
              //
              Socket->channelList[Index + (McIndex * MaxChPerMc)].perRemSize = Socket->channelList[Index + (McIndex * MaxChPerMc)].perRemSize - (UINT16)InterSizePerCh;
              break;

            case MemType2lmDdrWbCacheAppDirect:
              if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
                break;
              }
              //
              // In legacy PMEM caching the WbCachePerSize = perSize. Native AD-WB caching can use the AD cached PM DIMM partition size.
              //
              WbCachePerRemSize = 0;
              if (MemMapData->PmemCaching == LegacyPmCaching) {
                WbCachePerRemSize =  Socket->channelList[Index + (McIndex * MaxChPerMc)].perRemSize;
              } else {
                WbCachePerRemSize = Socket->channelList[Index + (McIndex * MaxChPerMc)].WbCachePerRemSize;
              }

              if (InterSizePerCh > WbCachePerRemSize) {
                //
                // Not enough space, generate error.
                //
                RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "AD-WB: Insufficient space on Dimm\n");
                OutputError (ERR_NGN, NGN_PMEM_CONFIG_ERROR, Sckt, 0xFF, 0xFF, 0xFF);
                return ERROR_RESOURCE_CALCULATION_FAILURE;
              }
              //
              // Get the offset (within Persistant range) specified in the interleave request.
              //
              InterReqOffset = GetInterReqOffset (Host, Sckt, McIndex, Index, Dimm, Socket->SAD[SadIndex].Limit);
              DpaOffset = (*DimmNvList)[Dimm].perRegionDPA + InterReqOffset;
              //
              // If the system is running in 1LM + AD-WB
              // Find if there is another DDR DIMM in this channel that has already been mapped.
              // If so, find the size of the memory that is already mapped from this channel and add it to the offset.
              // This will ensure that the channel address decoded using the Tad Offset will be correct.
              //
              DpaOffset = ComputeChannelOffset (Host, DpaOffset, Sckt, Index + (McIndex * MaxChPerMc));
              //
              // Update remaining size on channel.
              //
              if (MemMapData->PmemCaching == LegacyPmCaching) {
                Socket->channelList[Index + (McIndex * MaxChPerMc)].perRemSize -= (UINT16)InterSizePerCh;
                ChannelMemUsed = (Socket->channelList[Index + (McIndex * MaxChPerMc)].perSize - WbCachePerRemSize) + InterSizePerCh + DpaOffset;
              } else {
                Socket->channelList[Index + (McIndex * MaxChPerMc)].WbCachePerRemSize -= (UINT16)InterSizePerCh;
                ChannelMemUsed = (Socket->channelList[Index + (McIndex * MaxChPerMc)].WbCachePerSize - WbCachePerRemSize) + InterSizePerCh + DpaOffset;
              }
              break;
#endif // DDRT_SUPPORT
          }
        }
        //
        // Update the TadInterleave list.
        //
        (*TadIntList)[Index] = 1;

        //
        // Update the TadOffset value.
        //
        (*TadOffset)[Index] = CalculateTadOffset (
          Socket->SAD[SadIndex].mirrored,
          TadEntry->socketWays,
          ChannelWays,
          ChannelMemUsed,
          TadLimit
          );

        //
        // Calculate the Ch_idx_offset for each of the channel.
        // Ch_idx_offset = (TAD[N].BASE / TAD[N].TAD_SKT_WAY) % TAD[N].TAD_CH_WAY
        //
        if (SadIndex > 0) {
          (*TadChnIndex)[Index] = (Socket->SAD[SadIndex].Base / TadEntry->socketWays) % ChannelWays;
        } else {
          (*TadChnIndex)[Index] = 0;
        }
      } else {
        (*TadIntList)[Index] = 0;
        (*TadChnIndex)[Index] = 0;
      }
    } // I
  } // McIndex

  //
  // S14010985526: MC Assertion error for a valid address. This workaround requires programming FM TAD rules
  // to all near memory channels to have the offset to avoid MCA.
  //
  if (IsSiliconWorkaroundEnabled ("S14010985526")) {
    if ((TadEntry == NULL) || (TadOffset == NULL) || (TadChnIndex == NULL) || (TadIntList == NULL)) {
      return Status;  // Ensure that the TAD has been created.
    }
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if (((Socket->SAD[SadIndex].imcInterBitmap & (BIT0 << McIndex)) != 0) ||
          ((Socket->SAD[SadIndex].NmImcInterBitmap & (BIT0 << McIndex)) == 0)) {
        continue;
      }

      Imc = &Mem->socket[Sckt].imc[McIndex];
      TadIndex = FindTadIndex (Host, Sckt, McIndex);
      if (TadIndex == 0xFF) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "TAD rules exeeded\n");
        OutputWarning (WARN_INTERLEAVE_FAILURE, WARN_TAD_RULES_EXCEEDED, Sckt, 0xFF, 0xFF, 0xFF);
        return ERROR_RESOURCE_CALCULATION_FAILURE;
      }

      //
      // Duplicate the last created TAD to NM MCs.
      //
      CopyMem (&Imc->TAD[TadIndex], TadEntry, sizeof (Imc->TAD[TadIndex]));
      CopyMem (&Imc->TADOffset[TadIndex], TadOffset, sizeof (Imc->TADOffset[TadIndex]));
      CopyMem (&Imc->TADChnIndex[TadIndex], TadChnIndex, sizeof (Imc->TADChnIndex[TadIndex]));
      CopyMem (&Imc->TADintList[TadIndex], TadIntList, sizeof (Imc->TADintList[TadIndex]));
    }
  }

  return Status;
} // TadInterleave

#ifdef DDRT_SUPPORT
/**

  TAD (Channel) Interleave of 1LM App direct memory.

  Interleave 1LM AD memory on the channel level within a socket. This routine figures out the mapping and fills in the
  structures TAD table. It does not program any registers. Assume all SAD entries are sorted correctly at this point.

  @param[in]   MemMapData  - Pointer to the memory map private data structure.

  @retval SUCCESS - Memory interleaved.
  @retval FAILURE - TAD rules exceeded.

**/
UINT32
TadInterleavePmem (
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  UINT32             Status;
  SYSHOST            *Host;
  SOCKET_INFO_STRUCT *Socket;
  UINT8              Sckt;
  UINT8              SadIndex;
  UINT8              BeginSadIndex;
  UINT8              EndSadIndex;
  UINT8              ClusterId;

  Host = GetSysHostPointer ();
  Status = SUCCESS;

  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {

    Socket = &Host->var.mem.socket[Sckt];

    // Skip socket if not enabled
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }

    for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
      BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
      EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
      if (Socket->SAD[BeginSadIndex].Enable == 0) {
        break;
      }
      for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
        //Break if Sad is not enabled
        if (Socket->SAD[SadIndex].Enable == 0) {
          break;
        }
        //Skip this SAD if the SAD is not local
        if (Socket->SAD[SadIndex].local == 0) {
          continue;
        }

        if (Socket->SAD[SadIndex].type == MemTypeNxm) {
          continue;
        }

        if (Socket->SAD[SadIndex].type != MemType1lmAppDirect) {
          continue;
        }

        Status = TadInterleave (MemMapData, Sckt, SadIndex, 0);
        if (Status != SUCCESS) {
          return Status;
        }
      } // SadIndex
    } // Cluster

    Status = TadInterleaveForBlockDecoder (Sckt);
    if (Status != SUCCESS) {
      return Status;
    }
  } // Sckt

  return Status;
}
#endif // DDRT_SUPPORT


/**

  TAD (Channel) Interleave of volatile 1LM/2LM memory.

  Interleave 1LM/2LM memory on the channel level within a socket. This routine figures out the mapping and fills in the
  structures TAD table. It does not program any registers. Assume all SAD entries are sorted correctly at this point.

  @param[in]   MemMapData  - Pointer to the memory map private data structure.

  @retval SUCCESS - Memory interleaved.
  @retval FAILURE - TAD rules exceeded.

**/
UINT32
TadInterleaveVolMem (
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  UINT32             Status;
  SYSHOST            *Host;
  SOCKET_INFO_STRUCT *Socket;
  UINT8              Sckt;
  UINT8              SadIndex;
  UINT8              BeginSadIndex;
  UINT8              EndSadIndex;
  UINT8              ClusterId;
  UINT32             McBitmap2LmProcessed;
  UINT8              McIndex;
  UINT8              MaxImc;

  Host = GetSysHostPointer ();
  Status = SUCCESS;
  MaxImc = GetMaxImc ();

  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {

    Socket = &Host->var.mem.socket[Sckt];

    // Skip socket if not enabled
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }

    McBitmap2LmProcessed = 0;

    for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
      BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
      EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
      if (Socket->SAD[BeginSadIndex].Enable == 0) {
        break;
      }
      for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
        //Break if Sad is not enabled
        if (Socket->SAD[SadIndex].Enable == 0) {
          break;
        }
        //Skip this SAD if the SAD is not local
        if (Socket->SAD[SadIndex].local == 0) {
          continue;
        }

        if (Socket->SAD[SadIndex].type == MemTypeNxm) {
          continue;
        }

        if (!IsMemTypeDdr (Socket->SAD[SadIndex].type) && !IsMemType2lmHbm (Socket->SAD[SadIndex].type)) {
          continue;
        }

        //
        // For a 2LM TAD, first create the NM TAD before creating the TAD's for FM.
        // Implementation of HBM 2LM TAD for patrol is pending.
        //
        if (IsMemType2lmDdr (Socket->SAD[SadIndex].type)) {
          for (McIndex = 0; McIndex < MaxImc; McIndex++) {
            if ((Socket->SAD[SadIndex].NmImcInterBitmap & (BIT0 << McIndex)) == 0) {
              continue;
            }
            if ((McBitmap2LmProcessed & (1 << McIndex)) != 0) {
              continue;  // The 2LM region has been processed for this MC.
            }
            Status = CreateNmTadForPatrolSpare (Host, MemMapData, Sckt, McIndex, SadIndex);
            if (Status != SUCCESS) {
             return Status;
            }
            McBitmap2LmProcessed |= 1 << McIndex;
          }
        }

        Status = TadInterleave (MemMapData, Sckt, SadIndex, ClusterId);
        if (Status != SUCCESS) {
          return Status;
        }
      } // SadIndex
    } // Cluster
  } // Sckt

  return Status;
}

/**

  This routine Creates TAD rules in each IMC for the memory to be mapped into the system address space.

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to the memory map private data structure

  @retval status

**/
UINT32
CreateTadRules (
  IN  PSYSHOST Host,
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT32  Status;

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DDR_MEMMAP, CHECKPOINT_MINOR_TAD_NONINTER, 0));

  //
  // The order of TAD interleaving matches the order of SAD interleaving.
  // The 1LM and 2LM volatile memory modes is mapped first
  // AD is mapped mext
  // Block decoder is mapped last
  //
  Status = TadInterleaveVolMem (MemMapData);
  if (Status != SUCCESS) {
    return Status;
  }

#ifdef DDRT_SUPPORT
  if (Host->nvram.mem.DcpmmPresent) {
    Status = TadInterleavePmem (MemMapData);
    if (Status != SUCCESS) {
      return Status;
    }
  }
#endif // DDRT_SUPPORT

  Status = (UINT32)CreateHbmTadRules (MemMapData);
  if (EFI_ERROR(Status)) {
    return FAILURE;
  }

  return Status;
}

#ifdef DDRT_SUPPORT
/**

  Program CSRs required to enable WPQ Flush

  @param[in] Host    - Pointer to sysHost
  @param[in] ScktId  - The Socket index for the socket to initialize

  @retval N/A

**/
VOID
WriteWpqFlushCsrs (
  IN PSYSHOST Host,
  IN UINT8    ScktId
  )
{
  UINT8           McId;
  UINT8           ChId;
  UINT8           MaxImc;
  UINT8           ImcEnabled;
  UINT8           NumChPerMc;
  UINT32          BlockDecoderBase;
  UINT64          HintBase;
  UINT64          HintAddress[MAX_MC_CH];
  MEMORY_MAP_HOST *MemMapHost;

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();
  MemMapHost = GetMemMapHostPointer ();
  BlockDecoderBase = MemMapHost->BlockDecoderData.BlockDecoderBase;

  if (BlockDecoderBase == 0 || Host->nvram.mem.socket[ScktId].DcpmmPresent == 0 || !MemMapHost->IsWpqFlushSupported) {
    return;
  }

  //
  // The base of Wpq Flush Hint address for socket 0, mc 0 and ch 0.
  //
  HintBase = LShiftU64 (BlockDecoderBase, CONVERT_64MB_TO_B) + (FLUSH_HINT_ADDRESS_OFFSET & BLOCK_DECODER_LOWER_BITS_MASK) +
             LShiftU64 ((FLUSH_HINT_ADDRESS_OFFSET & (~BLOCK_DECODER_LOWER_BITS_MASK)), BLOCK_DECODER_BITS);

  for (McId = 0; McId < MaxImc; McId++) {
    GetImcEnabledData (ScktId, McId, &ImcEnabled);
    if (ImcEnabled == 0) {
      continue;
    }

    for (ChId = 0; ChId < NumChPerMc; ChId++) {
      if (!AepDimmPresent (ScktId, ChId + McId * NumChPerMc, NULL)) {
        HintAddress[ChId] = 0;
        continue;
      }

      HintAddress[ChId] = HintBase + (ScktId << BLOCK_DECODER_SOCKET_TARGET_SHIFT) +
                          (McId << BLOCK_DECODER_MC_ID_SHIFT) + (ChId << BLOCK_DECODER_CH_ID_SHIFT);

      MemMapHost->SocketInfo[ScktId].ChannelInfo[ChId + McId * NumChPerMc].WpqFlushHintAddress = HintAddress[ChId];
      RcDebugPrint (SDBG_MAX, "Socket%d:Mc%d:Ch%d WpqFlushHintAddress = 0x%llX\n", ScktId, McId, ChId, HintAddress[ChId]);
    }

    //
    // 10nm need to program PAD 0~2 if the corresponding channel has DCPMM dimm installed as 10nm use block decoder and
    // the controller region channel granualirity is fixed 14 bit.
    //
    ProgramM2mPcommit (ScktId, McId, HintAddress);

    //
    // Set DDRT_WPQ_FLUSH_CTL.wpqflush_enable = 1 on all channels on enabled MCs
    //
    EnableMcWpqFlush (ScktId, McId);
  }
}
#endif // DDRT_SUPPORT

/**

  Fill in SAD/TAD/RIR CRSs for memory map from config info in Host structure

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to the memory map private data structure

  @retval N/A

**/
VOID
WriteMemoryMapCsrs (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8           Sckt;

  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }
#ifdef DDRT_SUPPORT
    WriteBlockDecoderCsrs (Sckt);
#endif // DDRT_SUPPORT
    //
    // Checkpoint for SAD covers both remote and local DRAM decoders which must be written sequentially.
    //
    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DDR_MEMMAP, CHECKPOINT_MINOR_WRITE_SAD, Sckt));
    WriteRemoteSadCsrs (MemMapData, Sckt);
    if ((GetSocketMemSize (Sckt) != 0) || (GetHbmStackCount (MemMapData, Sckt) != 0)) {
      WriteSadCsrs (Host, MemMapData, Sckt);
      WriteRouteTableCsrs (Host, MemMapData, Sckt);
      WriteMesh2MemCsrs (Host, MemMapData, Sckt);
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DDR_MEMMAP, CHECKPOINT_MINOR_WRITE_TAD, Sckt));
      WriteTadCsrs (Host, Sckt, MemMapData);
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DDR_MEMMAP, CHECKPOINT_MINOR_WRITE_RIR, Sckt));
      WriteRirCsrs (Host, Sckt, MemMapData);
      WriteChaCsrs (Host, MemMapData, Sckt);
      WriteWorkAroundCsrs (Host, MemMapData, Sckt);
    }
    WriteTolmTohmCsrs (Host, MemMapData);
#ifdef DDRT_SUPPORT
    WriteWpqFlushCsrs(Host, Sckt);
#endif // DDRT_SUPPORT
    WriteCriticalChunkCsrs (Sckt);
  }

  //
  // Write 10nm SoC specific CSRs for memory map
  //
  WriteMemoryMapCsrsEx (Host, MemMapData);

  return;
}

/** Interleaves memory on the channel level across sockets

  This routine figures out the mapping and fills in the structures for the SAD, TAD,
  and SAG tables. It does not program any registers.

  @param[in] Host       - Pointer to sysHost
  @param[in] MemMapData - Pointer to MemMapData (Private DS maintained by Memory decoder)

  @retval    Status

**/
UINT32
SocketInterleave (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  MEM_TYPE  MemType;
  UINT32    Status;
#ifdef DDRT_SUPPORT
  UINT8     ImcInter;
  UINT8     ChInter;
  BOOLEAN   InterModified;
#endif

  //
  // Determine volatile memory mode
  //
  if (Host->var.mem.volMemMode != VOL_MEM_MODE_1LM) {
    return FAILURE;
  }

  Status = SUCCESS;
  MemType = MemType1lmDdr;

  UpdateHostStructForVolMem (Host, MemMapData, SOCKET_BITMAP_ALL, MemType);

  //
  // Map the volatile memory
  //
  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_DDR_MEMMAP, CHECKPOINT_MINOR_SAD_INTERLEAVE, 0));

  if (PcdGetBool (PcdNvDimmEn)) {
    //
    // Interleave Normal memory dimms first and then NvMem
    //
    Status = SocketInterleaveNormalMem (Host, MemMapData, MemType);
    if (Status != SUCCESS) {
      return Status;
    }

    Status = SocketInterleaveNVMem (Host, MemMapData, MemType);
    if (Status != SUCCESS) {
      return Status;
    }

  } else {
    Status = SadUma (Host, MemMapData, MemType);
    if (Status != SUCCESS) {
      return Status;
    }
  }

#ifdef DDRT_SUPPORT
  //
  // Interleave PMEM/PMEM$ only if the dimms are controlled by SV only setup options not the CR Management driver
  //
  if (Host->var.mem.dfxMemVars.dfxDimmManagement != DIMM_MGMT_CR_MGMT_DRIVER) {
    //
    // Check if Pmem is possible and that Pmem mapping is enabled
    //
    if (IsPmemPossible (Host, SOCKET_BITMAP_ALL) && !(Host->var.mem.dfxMemVars.dfxPerMemMode == NON_PER_MEM_MODE)) {
      if (Host->var.mem.dfxMemVars.dfxPerMemMode == PER_MEM_MODE)  {
        MemType = MemType1lmAppDirect;
      }
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
      // Update the remsize information in the Host struct for this socket before calling SadUma() to create PMEM SADs.
      //
      UpdateHostStructForPmem (Host, MemMapData, SOCKET_BITMAP_ALL);
      //
      // Create PMEM SADs
      //
      Status = SadUma (Host, MemMapData, MemType);
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
  }
#endif // DDRT_SUPPORT
  return SUCCESS;
}

/**

  Init memory size variables for blk window size and/or controil region window size

  @param[in] Host     - Pointer to sysHost

  @retval N/A

**/
UINT8
InitializeDdrtDimmChip (
  IN PSYSHOST Host
  )
{
  return SUCCESS;
}

/**

  This routine returns the TAD chn_ways for 1LM and 2LM memory
  for a given "SocketWays" interleave of "Type" memory type

  @param[in] MemMapData  - Pointer to the memory map private data structure.
  @param[in] Type        - The Type of memory to encode for
  @param[in] McBitmap    - Bitmap of MCs in the interleave.
  @param[in] ChWays      - The Number of channel ways per MC

  @retval UINT8 The channel ways values to use for MC interleaving.

**/
UINT8
GetTadChannelWays (
  IN MEMORY_MAP_DATA *MemMapData,
  IN MEM_TYPE        Type,
  IN UINT8           McBitmap,
  IN UINT8           ChWays
  )
{
  UINT8   ChWaysReturnVal;
  UINT8   McIndex;
  UINT8   McWays;
  UINT8   MaxImc;

  McWays = 0;
  MaxImc = GetMaxImc ();

  //
  // The FastDiv3 feature is not supported in 1LM. 2LM in the case of 3 - way interleave always performs "fast div3".
  // NmChnWays has only support for FastDiv3, there is no support for regular division by 3 like in 1LM.
  //
  if (ChWays == CH_3WAY) {
    return (Type != MemType1lmDdr)? ONE_WAY : ChWays;
  }

  if ((Type == MemType1lmDdr) ||
      ((IsMemTypeDdrt (Type) && (MemMapData->DdrtIntlvGranularity == INTERLEAVE_GRANULARITY_256B)))) {
    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if (McBitmap & (BIT0 << McIndex)) {
        McWays++;
      }
    }
    ChWaysReturnVal = McWays * ChWays;

    if (IsUbcSupportedMemType (Type) && MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) {
      //
      // Divide chn_ways by 2 for hemisphere or 4 for quadrant
      //
      if (ChWaysReturnVal >= MemMapData->UmaBasedClustersPerSocket) {
        ChWaysReturnVal = ChWaysReturnVal / MemMapData->UmaBasedClustersPerSocket;
      } else {
        //
        // For X-Tile 2LM, FM channel ways might be less than UBC cluster.
        // in this case, channel ways must always be 1.
        //
        ChWaysReturnVal = ONE_WAY;
      }
    }
  } else {
    ChWaysReturnVal = ChWays;
  }

  return ChWaysReturnVal;
}

/**
  Get cluster for MC.

  @param[in]      MemMapData  Pointer to the memory map private data structure.
  @param[in]      McIndex     MC index to get the hemisphere for.

  @return                     Cluster index for the MC.
**/
UINT8
GetClusterForMc (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           McIndex
  )
{
  UINT8   Cluster;
  BOOLEAN SncEnabled;
  UINT8   NumberOfClusters;
  UINT8   NumberOfMcsPerCluster;

  SncEnabled = IfFeatureEnabled (TypeSnc);
  if (SncEnabled) {
    NumberOfClusters = GetNumOfClusterPerSystem ();
    NumberOfMcsPerCluster = (GetMaxImc () / NumberOfClusters);
    if (NumberOfMcsPerCluster == 1) {
      Cluster = McIndex;
    } else {
      if (IsNorthSouthCluster ()) {
        Cluster = McIndex % NumberOfMcsPerCluster;
      } else {
        Cluster = McIndex / NumberOfMcsPerCluster;
      }
    }
  } else if (MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) {
    Cluster = GetUmaBasedClusterForMc (McIndex, MemMapData->UmaBasedClustersPerSocket);
  } else {
    Cluster = 0;
  }

  return Cluster;
}

/**

  This routine returns the logical ID for a given Physical MC and Physical Channel ID in that MC.

  @param[in] MemMapData  - Pointer to the memory map private data structure
  @param[in] Sckt        - Socket Id
  @param[in] McIndex     - Physical MC Index to use for Logical Channel ID lookup
  @param[in] ChIndex     - Physical Channel Index within the McIndex to use for the
                           logical Channel ID lookup.
  @param[in] MemType     - Type of memory that the interleave request corresponds to.
  @param[in] ChWays      - The number of channel ways for this SadEntry interleave.
  @param[in] McWays      - The number of MC ways for this SadEntry interleave.

  @retval UINT8 Channel logical ID corresponding the physical MC and Channel ID.

**/
UINT8
GetLogicalChannelId (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN UINT8           McIndex,
  IN UINT8           ChIndex,
  IN MEM_TYPE        MemType,
  IN UINT8           ChWays,
  IN UINT8           McWays
  )
{
  UINT8              ChLid = (UINT8)~0;
  ROUTE_TABLE_HALVES Half;
  BOOLEAN            Found;
  UINT8              Cluster;
  UINT8              RtIndex;
  ROUTE_TABLE_DATA   *RouteTableData;

  Cluster = GetClusterForMc (MemMapData, McIndex);
  Found = FALSE;

  if (McWays == ONE_WAY && ChWays == ONE_WAY) {
    //
    // Single targets always index to logical channel 0
    //
    return 0;
  }

  if (MemType == MemType2lmHbmCacheDdr) {
    RouteTableData = &(MemMapData->FarDdrRouteTableData);
  } else {
    RouteTableData = GetRouteTableDataForMemType (MemMapData, Sckt, MemType);
  }

  //
  // Search route tables on both halves.
  //
  for (Half = 0; Half < MAX_RT; Half++) {
    if (IsMemType2lm (MemType) && MemMapData->Is2LmUsingRt2lm) {
      for (RtIndex = 0; RtIndex < RouteTableData->RouteTableUsed[Sckt][Cluster][Half]; RtIndex++) {
        if (RouteTableData->TgtRouteTable[Sckt][Cluster][Half][RtIndex] == McIndex) {
          for (ChLid = 0; ChLid < ChWays; ChLid++) {
            if (RouteTableData->ChRouteTable[Sckt][Cluster][Half][ChLid] == ChIndex) {
              Found = TRUE;
              break;
            }
          }
        }
      } // RtIndex
    } else {
      for (ChLid = 0; ChLid < RouteTableData->RouteTableUsed[Sckt][Cluster][Half]; ChLid++) {
        if (RouteTableData->ChRouteTable[Sckt][Cluster][Half][ChLid] == ChIndex &&
          RouteTableData->TgtRouteTable[Sckt][Cluster][Half][ChLid] == McIndex) {
          Found = TRUE;
          break;
        }
      }
    }
    if (Found) {
      break;
    }
  } // Half
  return ChLid;
}

/**

This routine checks if the interleave is a 6-way across 4 MCs or two halves for volatile.

@param[in]      MemType       Type of memory being interleaved.
@param[in]      ChWays        Number of channels interleaved per target.
@param[in]      McBitmap      Bitmap of MCs in the interleave.


@retval TRUE  Interleave corresponds to a 6-way across both halves.
@retval FALSE Memory is not 1LM or Interleave is not a 6-way.

**/
BOOLEAN
IsVolMod3InterleaveAcrossHalves (
  IN  MEM_TYPE  MemType,
  IN  UINT8     ChWays,
  IN  UINT8     McBitmap
  )
{
  ROUTE_TABLE_HALVES  Half;
  UINT8               NumberOfHalvesInInterleave;

  NumberOfHalvesInInterleave = 0;

  //
  // ICX supports mod3 only for 1LM DDR
  //
  if ((IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)) && (MemType != MemType1lmDdr)) {
    return FALSE;
  }

  if (!IsMemTypeVolatile (MemType) || (ChWays != CH_3WAY)) {
    return FALSE;
  }

  for (Half = 0; Half < MaxRtHalves; Half++) {
    if ((McBitmap & GetMcBitMaskForHalf(Half)) != 0) {
      NumberOfHalvesInInterleave++;
    } // Half loop
  }
  if (NumberOfHalvesInInterleave == MaxRtHalves) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**

  This routine returns the logical ID for a given Physical target (MC / Socket).

  @param[in] MemMapData  - Pointer to the memory map private data structure
  @param[in] Sckt        - Socket Id
  @param[in] McIndex     - Physical MC Index to use for Logical Channel ID lookup
  @param[in] SadEntry    - SadEntry corresponding to the MC interleave being encoded
  @param[in] SadId       - Sad Index corresponding to the MC Tad entry.

  @retval UINT8 The logical target ID to use for MC TAD.
**/
UINT8
GetLogicalTargetId (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN UINT8           McIndex,
  IN SAD_TABLE       *SadEntry,
  IN UINT8           SadId
  )
{
  MEMORY_MAP_HOST *MemMapHost;
  UINT8           Index;
  UINT8           TargetLidSearchIndex;
  UINT8           TargetLid;
  UINT8           MaxImc;
  UINT8           McWays;
  UINT8           SktWays;

  MemMapHost = GetMemMapHostPointer ();
  TargetLid = 0;
  MaxImc = GetMaxImc ();
  McWays = 0;
  SktWays = 0;

  if ((MemMapData->UmaBasedClustersPerSocket == UMABASEDCLUSTERING_DISABLED) ||
      !IsUbcSupportedMemType (SadEntry->type)) {
    //
    // For a 256B MC channel granularity, the target is at socket.
    //
    if (IsMemType1lm (SadEntry->type) ||
        (IsMemTypeDdrt (SadEntry->type) && (MemMapData->DdrtIntlvGranularity == INTERLEAVE_GRANULARITY_256B))) {

      for (Index = 0; Index < Sckt; Index++) {
        for (TargetLidSearchIndex = 0; TargetLidSearchIndex < MaxImc; TargetLidSearchIndex++) {
          if (MemMapHost->SocketInfo[Sckt].SadIntList[SadId][(Index * MaxImc) + TargetLidSearchIndex] != 0) {
            TargetLid++;
            break;
          }
        }
      }
    } else {
      //
      // For a 4K MC channel granularity, the target is at MC.
      //
      for (Index = 0; Index < Sckt * MaxImc; Index++) {
        if (MemMapHost->SocketInfo[Sckt].SadIntList[SadId][Index] == 1) {
          TargetLid++;
        }
      }
      //
      // Index now points to the first MC interleave list entry for the current socket.
      // Continue counting the MC's that are part of the interleave until we reach the
      // MC for McIndex.
      //
      TargetLidSearchIndex = Index;
      while ((TargetLidSearchIndex < Index + McIndex) && (TargetLidSearchIndex < MC_MAX_NODE)) {
        if (MemMapHost->SocketInfo[Sckt].SadIntList[SadId][TargetLidSearchIndex] == 1) {
          TargetLid++;
        }
        TargetLidSearchIndex++;
      }
    }

    //
    // Check for mod3 interleave across halves.
    //
    if (IsVolMod3InterleaveAcrossHalves (SadEntry->type, SadEntry->NmChWays, SadEntry->NmImcInterBitmap)) {
      //
      // Now need to determine the mod3 is a 6-way or 3-way across halves.
      // Target_lid for 6-way should consider # of halves in the interleave.
      //
      if ((SadEntry->NmChWays * SadEntry->ways) > THREE_WAY) {
        return ((TargetLid * MaxRtHalves) + (UINT8)GetHalfForMc (McIndex));
      }
    }
  } else {
    //
    // For a 256B MC channel granularity, the target is at socket.
    //
    for (Index = 0; Index < Sckt; Index++) {
      for (TargetLidSearchIndex = 0; TargetLidSearchIndex < MaxImc; TargetLidSearchIndex++) {
        if (MemMapHost->SocketInfo[Sckt].SadIntList[SadId][(Index * MaxImc) + TargetLidSearchIndex] != 0) {
          TargetLid++;
          break;
        }
      }
    }

    //
    // In the case of Hemi clustering where the target is each Hemi/half and channel granularity is lesser
    // than target granularity.
    //
    if ((MemMapData->UmaBasedClustersPerSocket == UMABASEDCLUSTERING_HEMISPHERE) && (IsMemType1lm (SadEntry->type) ||
        (IsMemTypeDdrt (SadEntry->type) && (MemMapData->DdrtIntlvGranularity == INTERLEAVE_GRANULARITY_256B)))) {
      TargetLid = ((TargetLid * MaxRtHalves) + (UINT8)GetHalfForMc (McIndex));
    } else {
      //
      // In a Quad where target ways is > number of halves or Hemi 4K where target granularity is lesser than channel
      // granularity, the TargetLid is calculated as:
      // A[11]: Hemi, A[12]: MC within Hemi, A[15:13] Socket
      // To achieve clustering performance the decoder jumps hemi before MC within a Hemi when target ways is > number
      // of halves.
      //

      //
      // For UMA interleave target LID must consider # of CPUs interleaved
      //
      McWays = CalculateInterleaveWaysfromBitmap (SadEntry->imcInterBitmap);
      if (McWays == 0) {
        return 0;
      }
      SktWays = SadEntry->ways / McWays;

      if (SktWays > ONE_WAY) {
        TargetLid = (TargetLid << 2);
        TargetLid |= ((McIndex & BIT0) << 1)| (GetHalfForMc (McIndex) & BIT0);
      } else {
        TargetLid = ((McIndex & BIT0) << 1)| (GetHalfForMc (McIndex) & BIT0);
      }
    }
  }
  return TargetLid;
}

/**

  This routine returns the TAD target_ways for a given
  "SocketWays" interleave of "Type" memory type

  @param[in] MemMapData  - Pointer to the memory map private data structure
  @param[in] Sckt        - Socket Id
  @param[in] SadEntry    - SadEntry corresponding to the interleave being encoded

  @retval UINT8 The target ways values to use for MC interleaving.
**/
UINT8
GetTadTargetWays (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN SAD_TABLE       *SadEntry
  )
{
  SYSHOST            *Host;
  UINT8              TargetWays;
  UINT8              McWays;

  McWays = 0;
  Host = GetSysHostPointer ();

  if (IsVolMod3InterleaveAcrossHalves (SadEntry->type, SadEntry->NmChWays, SadEntry->NmImcInterBitmap)) {
    return SadEntry->ways;
  }

  if ((((SadEntry->type == MemType1lmDdr) && (SadEntry->granularity == INTERLEAVE_GRANULARITY_256B))) ||
      ((IsMemTypeDdrt (SadEntry->type) && (MemMapData->DdrtIntlvGranularity == INTERLEAVE_GRANULARITY_256B)))) {

    //
    // For UMA interleave target_ways should consider # of CPUs interleaved
    //
    McWays = CalculateInterleaveWaysfromBitmap (SadEntry->imcInterBitmap);
    if (McWays == 0) {
      return 0;
    }
    TargetWays = SadEntry->ways / McWays;
    if (IsUbcSupportedMemType (SadEntry->type) && MemMapData->UmaBasedClustersPerSocket != UMABASEDCLUSTERING_DISABLED) {
      //
      // Multiply target_ways by 2 for hemisphere or 4 for quadrant
      //
      TargetWays = TargetWays * MemMapData->UmaBasedClustersPerSocket;
    }
  } else {
    TargetWays = SadEntry->ways;
  }

  return TargetWays;
}

/** Encode channel interleave granularity.

@param[in]  Granularity - Internal channel granularity

@retval     -      Encoded channel granularity.

**/
UINT8
EFIAPI
EncodeChannelInterleaveGranularity (
  IN UINT8  Granularity
  )
{
  UINT8 EncodedGranularity = 0;

  switch (Granularity) {
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
    break;
  }

  return (EncodedGranularity);
}

/**

  Get the size of the DDR Memory that will be used as 2LM cache.
  The value returned will be guaranteed to be a power of 2 and in 4GB Granularity.
  Assumes not called when HBM is caching

  @param[in] Sckt        - Socket number
  @param[in] McIndex     - Memory controller
  @param[in] ChIndex     - Channel controller

  @retval DDR Cache Size for the input channel as a power of 2 (4GB Granularity)

**/
UINT32
GetNmCacheSize (
  IN UINT8 Sckt,
  IN UINT8 McIndex,
  IN UINT8 ChIndex
  )
{
  SYSHOST *Host;
  UINT32  NmCacheSize;

  Host = GetSysHostPointer ();

  if (Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
    NmCacheSize = Host->var.mem.socket[Sckt].channelList[(McIndex * GetNumChannelPerMc ()) + ChIndex].DdrCacheSize;
  } else {
    NmCacheSize = GetChannelMemSize (Sckt, (McIndex * GetNumChannelPerMc ()) + ChIndex);
  }

  return Log2x32 (NmCacheSize >> CONVERT_64MB_TO_4GB_GRAN);
}

#ifdef DDRT_SUPPORT
/**


  Program 2LM decoder TAD rule registers.

  @param[in] Host        - Pointer to sysHost
  @param[in] MemMapData  - Pointer to the memory map private data structure
  @param[in] Sckt        - Socket number
  @param[in] McIndex     - Memory controller

  @retval VOID

**/
VOID
Write2lmDecoderRegisters (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN UINT8           McIndex
  )
{
  UINT8                                         ChIndex;
  SOCKET_INFO_STRUCT                            *Socket;
  UINT8                                         TadIndex;
  SAD_TABLE                                     *SadEntry;
  UINT8                                         MaxImc;
  UINT8                                         NumChPerMc;
  UINT8                                         ChannelInterleaveBitmap;
  UINT8                                         MinNmTgtWays;
  BOOLEAN                                       TwoLmRegionProcessed;
  NM_2LM_DATA                                   NmData;
  FM_2LM_DATA                                   FmData;
  UINT32                                        NmCacheLow;
  UINT32                                        NmCacheHigh;
  UINT8                                         TadTargetGranularity;
  UINT8                                         TadChannelGranularity;
  UINT8                                         SadIndex;
  UINT8                                         ClusterId;
  UINT8                                         BeginSadIndex;
  UINT8                                         EndSadIndex;
  SYS_SETUP                                     *Setup;

  RcDebugPrint (SDBG_MAX,
    "\n       Write 2LM Decoder CSR's for MC %d\n",
    McIndex
    );

  Socket = &Host->var.mem.socket[Sckt];
  Setup = GetSysSetupPointer ();
  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();
  TwoLmRegionProcessed = FALSE;

  MinNmTgtWays = MAX_SAD_WAYS;
  ChannelInterleaveBitmap = 0;
  TadIndex = 0;

  for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {

    BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
    EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);

    for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
      SadEntry = &Socket->SAD[SadIndex];
      //
      // Break if we have reached the end of the SAD table
      //
      if (SadEntry->Enable == 0) {
        break;
      }
      //
      // Program 2LMCNTL MC registers only for 2LM NM
      // Skip if SAD type is not 2LM or PMEM reserved (PMEM hole not interleaved across imc's & ch's).
      // Skip if iMC is not interleaved as NM for this SAD.
      //
      if (!IsMemType2lm (SadEntry->type) ||
          (SadEntry->type == MemType1lmAppDirectReserved) ||
          ((SadEntry->NmImcInterBitmap & (1 << McIndex)) == 0)) {
        continue;
      }
      GetTadRuleTargetGranularity (MemMapData, Sckt, SadIndex, &TadTargetGranularity);
      GetTadRuleChannelGranularity (MemMapData, Sckt, SadEntry->type, &TadChannelGranularity);
      //
      // 2LMCNTL registers are Per Channel per MC. So we process one instance of 2LM interleave.
      //
      if (!TwoLmRegionProcessed) {
        ChannelInterleaveBitmap |= SadEntry->channelInterBitmap[McIndex];

        ZeroMem (&NmData, sizeof (NmData));

        MemMapData->MemMapSocket[Sckt].NmIntlvValues[McIndex].NmTargetWays = GetTadTargetWays (MemMapData, Sckt, SadEntry);

        NmData.NmWaysGran.EncTgtWays = TadEncodeSkWays (MemMapData->MemMapSocket[Sckt].NmIntlvValues[McIndex].NmTargetWays);

        NmData.EnableTwoWay = IsTwoWayNmCacheEnabled ();

        NmData.NmTgtLid = GetLogicalTargetId (MemMapData, Sckt, McIndex, SadEntry, SadIndex);

        MemMapData->MemMapSocket[Sckt].NmIntlvValues[McIndex].NmChannelWays  = GetTadChannelWays (
                                                                                 MemMapData,
                                                                                 SadEntry->type,
                                                                                 SadEntry->NmImcInterBitmap,
                                                                                 SadEntry->NmChWays
                                                                                 );
        NmData.NmWaysGran.EncChWays = ImcTadEncodeChWays (MemMapData->MemMapSocket[Sckt].NmIntlvValues[McIndex].NmChannelWays);
        NmData.NmWaysGran.EncTgtGran = EncodeTadTargetInterleaveGranularity (TadTargetGranularity);
        NmData.NmWaysGran.EncChGran  = EncodeChannelInterleaveGranularity (SadEntry->granularity);

        for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
          if ((ChannelInterleaveBitmap & (BIT0 << ChIndex)) == 0) {
            continue;
          }

          //
          // Calculate logical ID corresponding to Physical ID
          //
          NmData.NmChLid = GetLogicalChannelId (
                             MemMapData,
                             Sckt,
                             McIndex,
                             ChIndex,
                             SadEntry->type,
                             SadEntry->NmChWays,
                             CalculateInterleaveWaysfromBitmap (SadEntry->NmImcInterBitmap)
                             );

          NmData.NmChSize = GetNmCacheSize (Sckt, McIndex, ChIndex);

          //
          // Calculate NM low and high base address
          //
          FindCachedRange (Host, MemMapData, Sckt, McIndex, SadEntry->Cluster, &NmCacheHigh, &NmCacheLow);
          NmData.NmCacheBase = NmCacheLow;

          if (NmData.NmCacheBase != 0) {
            //
            // Whenever the offset is enabled, force_lat must be set.
            //
            McSetForceLatency (
              MemTechDdr,
              Sckt,
              McIndex,
              ChIndex
              );
          }

          McSet2lmNm (
            MemTechDdr,
            Sckt,
            McIndex,
            ChIndex,
            &NmData
            );
        } // channel loop
        TwoLmRegionProcessed = TRUE;
      }

      //
      // d. Program MC DRAM_RULE_CFG[7:0]
      //
      ZeroMem (&FmData, sizeof (FmData));

      FmData.TadLimit              = SadTadEncodeLimit (SadEntry->Limit);
      //
      // If NM and FM channels are at the same granularity then the fm_chn_ways & fm_target_ways in MC_DRAM_RULE is max. one way
      //
      if (MemMapData->DdrtIntlvGranularity == INTERLEAVE_GRANULARITY_256B) {
        FmData.FmWaysGran.EncTgtWays = TadEncodeSkWays (ONE_WAY);
        FmData.FmWaysGran.EncChWays  = ImcTadEncodeChWays (ONE_WAY);
      } else {
        FmData.FmWaysGran.EncTgtWays = TadEncodeSkWays (GetTadTargetWays (MemMapData, Sckt, SadEntry));
        FmData.FmWaysGran.EncChWays  = ImcTadEncodeChWays (SadEntry->FmChWays);
      }
      FmData.FmWaysGran.EncChGran  = EncodeChannelInterleaveGranularity (TadChannelGranularity);
      FmData.FmWaysGran.EncTgtGran = EncodeTadTargetInterleaveGranularity (TadTargetGranularity);
      FmData.FmTarget              = GetFmTarget (MemMapData, Sckt, McIndex);
      FmData.FmChIntBitmap         = SadEntry->FMchannelInterBitmap[FmData.FmTarget];

      for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
        if ((ChannelInterleaveBitmap & (BIT0 << ChIndex)) == 0) {
          continue;
        }

        McSet2lmFm (
          MemTechDdr,
          Sckt,
          McIndex,
          ChIndex,
          TadIndex,
          &FmData
          );
      } // Channel loop
      TadIndex++;
    }  // SAD loop
  } // ClusterId
}
#endif // DDRT_SUPPORT

/**

Program 1LM decoder TAD rule registers.

@param[in] Host        - Pointer to sysHost
@param[in] MemMapData  - Pointer to the memory map private data structure
@param[in] Sckt        - Socket number
@param[in] McIndex     - Memory controller

@retval VOID

**/
VOID
Write1lmDecoderRegisters (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN UINT8           McIndex
  )
{
  UINT8                                         ChIndex;
  SOCKET_INFO_STRUCT                            *Socket;
  IMC_INFO_STRUCT                               *Imc;
  UINT8                                         TadIndex;
  TAD_TABLE                                     *TadEntry;
  SAD_TABLE                                     *SadEntry;
  UINT8                                         ChannelInterleaveBitmap;
  UINT8                                         NumChPerMc;
  TAD_1LM_DATA                                  TadData;
  UINT8                                         TargetGran;
  UINT8                                         FmChWays;
  MEMORY_MAP_HOST                               *MemMapHost;
  MEMORY_MAP_BLOCK_DECODER_DATA                 *BlockDecoder;
  BOOLEAN                                       IsNmTile;
  UINT8                                         Index;

  MemMapHost = GetMemMapHostPointer ();
  BlockDecoder = &MemMapHost->BlockDecoderData;

  RcDebugPrint (SDBG_MAX,
    "\n       Write 1LM Decoder CSR's for MC %d\n",
    McIndex
    );

  Socket = &Host->var.mem.socket[Sckt];
  Imc = &Socket->imc[McIndex];
  NumChPerMc = GetNumChannelPerMc ();

  for (TadIndex = 0; TadIndex < MAX_TAD_RULES_10NM; TadIndex++) {
    TadEntry = &Imc->TAD[TadIndex];
    ZeroMem (&TadData, sizeof (TadData));

    if (TadEntry->Enable == 0) {
      break;
    }
    TargetGran = TadEntry->TargetGran;
    if (TadEntry->SADId >= (MemMapData->MaxSadRules * MemMapData->NumberofChaDramClusters)) {
      //
      // BlockDecoder
      //
      SadEntry = NULL;
      ChannelInterleaveBitmap = 0;
      TadData.TadWaysGran.EncTgtWays = TadEncodeSkWays(TadEntry->socketWays);
      TadData.TadWaysGran.EncChWays  = ImcTadEncodeChWays(8);      // 8 channel way for block decoder
      TadData.TadWaysGran.EncTgtGran = INTERLEAVE_GRANULARITY_4KB; // 4K granuarity for block decoder
      TadData.TadWaysGran.EncChGran  = INTERLEAVE_GRANULARITY_4KB; // 4K granularity for channel
      for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
        if (BlockDecoder->BlockMcChEn[Sckt][McIndex][ChIndex] == 1) {
          ChannelInterleaveBitmap |= (BIT0 << ChIndex);
        }
      }
    } else {
      //
      // Check TAD corresponds to 1LM DDR or 2LM DDR NM
      // 2LM DDR NM is a special TAD with limit as 0.
      //
      SadEntry = &Socket->SAD[TadEntry->SADId];
      if ((SadEntry->type == MemType1lmDdr) || (TadEntry->Limit == 0)) {

        ChannelInterleaveBitmap = SadEntry->channelInterBitmap[McIndex];

        //
        // If SA based patrol, the chn_ways and target_ways match the NM_DRAM_RULE of the McChan
        // For CA based patrol, patrol walks through CA so there is not interleave
        //
        if (IsSystemAddressBasedPatrolSparing (Host, Sckt, McIndex, TadEntry->SADId)) {
          TadData.TadWaysGran.EncChWays = ImcTadEncodeChWays (GetTadChannelWays (
                                            MemMapData,
                                            SadEntry->type,
                                            SadEntry->NmImcInterBitmap,
                                            SadEntry->NmChWays
                                            ));
          TadData.TadWaysGran.EncTgtWays = TadEncodeSkWays (GetTadTargetWays (MemMapData, Sckt, SadEntry));
        } else {
          TadData.TadWaysGran.EncChWays = ImcTadEncodeChWays (ONE_WAY);
          TadData.TadWaysGran.EncTgtWays = TadEncodeSkWays (ONE_WAY);
        }
        TadData.TadWaysGran.EncChGran = EncodeChannelInterleaveGranularity (SadEntry->granularity);
      } else {
        ChannelInterleaveBitmap = SadEntry->FMchannelInterBitmap[McIndex];
        if (IsSiliconWorkaroundEnabled ("S14010985526") && (!IsMemType2lmHbm (SadEntry->type))) {
          ChannelInterleaveBitmap |= SadEntry->channelInterBitmap[McIndex];  // S14010985526: Program FM TAD rules to all NM channels.
        }
        FmChWays = GetTadChannelWays (MemMapData, SadEntry->type, SadEntry->imcInterBitmap, SadEntry->FmChWays);
        TadData.TadWaysGran.EncChWays = ImcTadEncodeChWays (FmChWays);
        TadData.TadWaysGran.EncChGran = EncodeChannelInterleaveGranularity (TadEntry->ChGran);
        if (IsMemType2lm (SadEntry->type) && MemMapData->MemMapSocket[Sckt].XTile2lm) {
          //
          // We need to handle some special cases in case of X-Tile 2LM as NM MC bitmap is not the same as FM bitmap
          //
          if (SadEntry->ways > FmChWays) {
            TadData.TadWaysGran.EncTgtWays = TadEncodeSkWays (TWO_WAY);
            if (MemMapData->UmaBasedClustersPerSocket == UMABASEDCLUSTERING_QUADRANT) {
              //
              // Special case that X-Tile 2LM granularity is 4KB in Quad mode when target_ways is 2.
              //
              TargetGran = INTERLEAVE_GRANULARITY_4KB;
            }
          } else {
            TadData.TadWaysGran.EncTgtWays = TadEncodeSkWays (ONE_WAY);
          }
        } else {
          TadData.TadWaysGran.EncTgtWays = TadEncodeSkWays (GetTadTargetWays (MemMapData, Sckt, SadEntry));
        }
      }

      TadData.TadWaysGran.EncTgtGran = EncodeTadTargetInterleaveGranularity (TargetGran);
    }

    for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
      if ((ChannelInterleaveBitmap & (BIT0 << ChIndex)) != 0) {
        TadData.TadOffset = Imc->TADOffset[TadIndex][ChIndex];

        //
        // S14010985526: The same TAD offset needs to be programmed on all interleaved channels for the same TAD rule.
        //
        if ((TadData.TadOffset == 0) && IsSiliconWorkaroundEnabled ("S14010985526")) {
          for (Index = 0; Index < NumChPerMc; Index++) {
            TadData.TadOffset = Imc->TADOffset[TadIndex][Index];
            if (TadData.TadOffset != 0) {
              break;  // Try to find the non-zero TAD offset calculated on FM channel.
            }
          }
        }

        McSet1lmTad (
          MemTechDdr,
          Sckt,
          McIndex,
          ChIndex,
          TadIndex,
          &TadData
          );
      }
    } // ChIndex
  } // Tad Loop

  //
  // Check if MC is NM in-tile or x-tile caching for DDR 2LM
  //
  IsNmTile = FALSE;
  if (IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lmDdr)) {
    if (Imc->memSize != 0) {
      IsNmTile = TRUE;
    }
  }

  McSetNmCachingCfg (
    MemTechDdr,
    Sckt,
    McIndex,
    IsNmTile
    );
}

/**
  Get DDR patrol sparing limit by passing all TAD tables correspond to the SAD index.
  Zero is returned if no such TAD table is found.

  @param[in]  Host        Pointer to the sysHost structure.
  @param[in]  SocketId    Socket number.
  @param[in]  McIndex     Memory controller index.
  @param[in]  SadIndex    Sad table index.

  @return The patrol sapring limit or 0 if not such TAD is found.

**/
UINT32
GetDdrPsLimit (
  IN SYSHOST           *Host,
  IN UINT8             SocketId,
  IN UINT8             McIndex,
  IN UINT8             SadIndex
  )
{
  TAD_TABLE            *TadEntry;
  UINT32               PsLimit;
  UINT8                TadIndex;

  PsLimit = 0;
  for (TadIndex = 0; TadIndex < MAX_TAD_RULES; TadIndex++) {
    TadEntry = &Host->var.mem.socket[SocketId].imc[McIndex].TAD[TadIndex];
    if (TadEntry->Enable == 0) {
      break;
    }
    if (TadEntry->SADId == SadIndex) {
      PsLimit = TadEntry->Limit;
      break;
    }
  }
  return PsLimit;
}

/**
  This routine programs the follwoing patrol sparing range registers: MC_DEC_CRNODE_CH{#CH}_MC{#MC}_CR_TADBASE_[7:0]
  and MC_DEC_CRNODE_CH{#CH}_MC{#MC}_CR_TADWAYNESS_[7:0]

  @param[in]  Host        Pointer to the sysHost structure.
  @param[in]  MemMapData  Pointer to memory map private data structure.
  @param[in]  SocketId    Socket number.
  @param[in]  McIndex     Memory controller index.

**/
VOID
WritePatrolSparingRangeRegisters (
  IN SYSHOST           *Host,
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             SocketId,
  IN UINT8             McIndex
  )
{
  UINT8                SadIndex;
  UINT8                ClusterId;
  UINT8                BeginSadIndex;
  UINT8                EndSadIndex;
  SAD_TABLE            *SadEntry;
  PS_RANGE_DATA        RangeData;
  UINT8                ChLid;
  UINT8                ChannelInterleaveBitmap;
  UINT8                ChIndex;
  UINT8                PsRangeRegIndex;
  BOOLEAN              TwoLmRegionProcessed;
  UINT8                NumChPerMc;
  UINT32               PsLimit;
  MEM_TYPE             SadMemType;

  PsRangeRegIndex = 0;
  NumChPerMc = GetNumChannelPerMc ();
  TwoLmRegionProcessed = FALSE;

  for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
    BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
    EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
    for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
      SadEntry = &Host->var.mem.socket[SocketId].SAD[SadIndex];
      if (SadEntry->Enable == 0) {
        break;
      }
      SadMemType = SadEntry->type;
      if (IsMemType2lmDdr (SadMemType)) {
        if (TwoLmRegionProcessed || (SadEntry->NmImcInterBitmap & (BIT0 << McIndex)) == 0) {
          continue;
        }
        GetNmPsRange (Host, MemMapData, SocketId, McIndex, SadIndex, &RangeData.PsBase, &PsLimit);
        TwoLmRegionProcessed = TRUE;
      } else if (IsMemTypeDdr (SadMemType)) {
        if (((SadEntry->imcInterBitmap & (BIT0 << McIndex)) == 0)) {
          continue;
        }
        PsLimit = GetDdrPsLimit (Host, SocketId, McIndex, SadIndex);
        if (PsLimit == 0) {
          continue;
        }
        if (Host->var.mem.socket[SocketId].SAD[SadIndex].Limit == FOUR_GB_MEM) {
          PsLimit +=  MemMapData->SadLimitAdjustment;
        }
        RangeData.PsBase = SadEntry->Base;
      } else {
        continue;
      }
      RangeData.PsLimit = SadTadEncodeLimit (PsLimit);
      //
      // The Logical Target ID depends on how the CHA interleaves the various MCs,
      // and it can be different for each dram memory rule (SAD Rule).
      //
      RangeData.TgtLid = GetLogicalTargetId (MemMapData, SocketId, McIndex, SadEntry, SadIndex);

      if (SadMemType == MemType2lmHbmCacheDdr) {
        ChannelInterleaveBitmap = SadEntry->FMchannelInterBitmap[McIndex];
      } else {
        ChannelInterleaveBitmap = SadEntry->channelInterBitmap[McIndex];
      }

      for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
        if ((ChannelInterleaveBitmap & (BIT0 << ChIndex)) == 0) {
          continue;
        }
        //
        // Calculate channel logical ID corresponding to Physical ID
        //
        ChLid = GetLogicalChannelId (
                  MemMapData,
                  SocketId,
                  McIndex,
                  ChIndex,
                  SadMemType,
                  SadEntry->NmChWays,
                  CalculateInterleaveWaysfromBitmap (SadEntry->NmImcInterBitmap)
                  );
        RangeData.ChLid = ChLid;

        RangeData.PsAttr = 0;

        McSetPatrolSparingRange (
          MemTechDdr,
          SocketId,
          McIndex,
          ChIndex,
          PsRangeRegIndex,
          &RangeData
          );
      }
      PsRangeRegIndex++;
    } // SadIndex
  } // ClusterId
}

/**
  This routine is worker function to program one of the following near memory dram rule registers:
  MC_DEC_CRNODE_CH{#CH}_MC{#MC}_CR_NM_DRAM_RULE_[7:0] for an input SAD entry

  @param[in]  MemMapData  Pointer to memory map private data structure.
  @param[in]  SocketId    Socket number.
  @param[in]  McIndex     Memory controller index.
  @param[in]  SadEntry    Pointer to input SAD table.
  @param[in]  SadIndex    Index of SAD table in the socket.
  @param[in]  NmDramIndex Near memory dram rule index.

**/
VOID
WriteNmDramRuleRegister (
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             SocketId,
  IN UINT8             McIndex,
  IN SAD_TABLE         *SadEntry,
  IN UINT8             SadIndex,
  IN UINT8             NmDramIndex
  )
{
  UINT8               TargetGran;
  PS_RULE_DATA        RuleData;
  UINT8               ChLid;
  UINT8               ChIndex;
  UINT8               NumChPerMc;
  UINT8               ChannelInterleaveBitmap;
  UINT8               TargetInterleaveBitmap;
  UINT8               ChWays;
  MEM_TYPE            SadMemType;

  NumChPerMc = GetNumChannelPerMc ();
  ZeroMem (&RuleData, sizeof (RuleData));

  RuleData.PsLimit = SadTadEncodeLimit (SadEntry->Limit);

  SadMemType = SadEntry->type;

  if (SadMemType == MemType2lmHbmCacheDdr) {
    TargetInterleaveBitmap = SadEntry->imcInterBitmap;
    ChWays = SadEntry->FmChWays;
    ChannelInterleaveBitmap = SadEntry->FMchannelInterBitmap[McIndex];
  } else {
    TargetInterleaveBitmap = SadEntry->NmImcInterBitmap;
    ChWays = SadEntry->NmChWays;
    ChannelInterleaveBitmap = SadEntry->channelInterBitmap[McIndex];
  }
  RuleData.WaysGran.EncChWays = ImcTadEncodeChWays (GetTadChannelWays (
                                  MemMapData,
                                  SadMemType,
                                  TargetInterleaveBitmap,
                                  ChWays
                                  ));
  GetTadRuleTargetGranularity (MemMapData, SocketId, SadIndex, &TargetGran);
  RuleData.WaysGran.EncChGran  = EncodeChannelInterleaveGranularity (SadEntry->granularity);
  RuleData.WaysGran.EncTgtGran = EncodeTadTargetInterleaveGranularity (TargetGran);

  RuleData.WaysGran.EncTgtWays = TadEncodeSkWays (GetTadTargetWays (MemMapData, SocketId, SadEntry));


  //
  // Init. channel ID transalation table to invalid data
  //
  for (ChLid = 0; ChLid < MAX_RT_WAYS; ChLid++) {
    RuleData.EncChL2pid |= 0x7 << (ChLid * 3);
  }

  for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
    if ((ChannelInterleaveBitmap & (BIT0 << ChIndex)) != 0) {
      //
      // Calculate channel logical ID corresponding to Physical ID
      //
      ChLid = GetLogicalChannelId (
                MemMapData,
                SocketId,
                McIndex,
                ChIndex,
                SadMemType,
                ChWays,
                SadEntry->ways
                );
      //
      // Clear only bits corresponding to this ChLid
      //
      RuleData.EncChL2pid &= ~(0x7 << (ChLid * 3));
      RuleData.EncChL2pid |= ChIndex << (ChLid * 3);
    }
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    " NM_DRAM_RULE[%d]:\n"
    "  rule_enable: %d  limit: 0x%x  nm_chn_ways: %d  nm_chn_gran: %d  "
    "nm_target_ways: %d  nm_gran_eq: %d  nm_chn_l2pid: 0x%08x\n",
    NmDramIndex,
    1,
    RuleData.PsLimit,
    RuleData.WaysGran.EncChWays,
    RuleData.WaysGran.EncChGran,
    RuleData.WaysGran.EncTgtWays,
    ((RuleData.WaysGran.EncTgtWays == 0) || (RuleData.WaysGran.EncChGran != RuleData.WaysGran.EncTgtGran)) ? 0 : 1,
    RuleData.EncChL2pid
    );

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Logical Ch Id:    0  1  2  3  4  5  6  7\n"
    "Physical Ch Id:  "
    );

  for (ChLid = 0; ChLid < MAX_RT_WAYS; ChLid++) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      " %d ",
      (RuleData.EncChL2pid >> (ChLid * 3)) & 0x7
      );
  }

  RcDebugPrint (SDBG_MAX, "\n");

  for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
    if ((ChannelInterleaveBitmap & (BIT0 << ChIndex)) == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, ChIndex, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      " NM_DRAM_RULE[%d]:\n",
      NmDramIndex
      );

    McSetPatrolSparingRule (
      MemTechDdr,
      SocketId,
      McIndex,
      ChIndex,
      NmDramIndex,
      &RuleData
      );
  } // Channel loop

}

/**
  This routine programs following near memory dram rule registers:
  MC_DEC_CRNODE_CH{#CH}_MC{#MC}_CR_NM_DRAM_RULE_[7:0]
  NM_DRAM_RULE is programmed only for SA based patrol and is not programmed for CA based patrol.

  @param[in]  Host        Pointer to the sysHost structure.
  @param[in]  MemMapData  Pointer to memory map private data structure.
  @param[in]  SocketId    Socket number.
  @param[in]  McIndex     Memory controller index.

**/
VOID
WriteNmDramRuleRegisters (
  IN SYSHOST           *Host,
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             SocketId,
  IN UINT8             McIndex
  )
{
  SAD_TABLE           *SadEntry;
  UINT8               SadIndex;
  UINT8               ClusterId;
  UINT8               BeginSadIndex;
  UINT8               EndSadIndex;
  UINT8               NmDramIndex;

  NmDramIndex = 0;
  for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
    BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
    EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
    for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
      SadEntry = &Host->var.mem.socket[SocketId].SAD[SadIndex];
      if (SadEntry->Enable == 0) {
        break;
      }
      if (IsSystemAddressBasedPatrolSparing (Host, SocketId, McIndex, SadIndex)) {
        WriteNmDramRuleRegister (MemMapData, SocketId, McIndex, SadEntry, SadIndex, NmDramIndex++);
      }
    } // SadIndex
  } // ClsuterId
}

/**
  This routine programs 2LM NM TAD for patrol and sparing address generation.

  @param[in]  Host        Pointer to the sysHost structure.
  @param[in]  MemMapData  Pointer to memory map private data structure.
  @param[in]  SocketId    Socket number.
  @param[in]  McIndex     Memory controller index.

**/
VOID
WriteNmCacheTadRegister (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           SocketId,
  IN UINT8           McIndex
  )
{
  TAD_TABLE          *TadEntry;
  UINT8              TadIndex;
  UINT8              SadIndex;

  if (!IsMemTypePresentMcFunction (SocketId, McIndex, IsMemType2lmDdr)) {
    return;
  }

  for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex++) {
    TadEntry = &Host->var.mem.socket[SocketId].imc[McIndex].TAD[TadIndex];
    if (TadEntry->Enable == 0) {
      break;
    }

    SadIndex = TadEntry->SADId;
    if (!IsMemType2lm (Host->var.mem.socket[SocketId].SAD[SadIndex].type)) {
      continue;
    }

    //
    // We found a 2LM memory type TAD here, so lookup TAD is for NM.
    //
    if (TadEntry->Limit == 0) {
      McSetNmCachingTad (MemTechDdr, SocketId, McIndex, TadIndex);
      break;
    }
  }
}

/**

The Patrol scrubbing and sparing logic is only for DDR and HBM, not for DDRT.
Program patrol and sparing registers (TADBASE, TADWAYNESS, MCNMCACHINGCFG and
NM_DRAM_RULE).

@param[in] Host            - Pointer to sysHost
@param[in] MemMapData      - Pointer to the memory map private data structure
@param[in] Sckt            - Socket number
@param[in] McIndex         - Memory controller

@retval VOID

**/
VOID
WritePatrolSparingRegisters (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN UINT8           McIndex
  )
{
   RcDebugPrint (SDBG_MAX,
    "\n       Write Patrol and Sparing CSR's for MC %d\n",
    McIndex
    );

   WritePatrolSparingRangeRegisters (Host, MemMapData, Sckt, McIndex);

   WriteNmDramRuleRegisters (Host, MemMapData, Sckt, McIndex);

   WriteNmCacheTadRegister (Host, MemMapData, Sckt, McIndex);

} // WritePatrolSparingRegisters

  /** Program MC decode CSRs for memory map from config info in Host structure.

  Read TAD structures filled-in sysHost and program 10nm MC decoder CSR's.

  @param[in] Host          - Pointer to sysHost
  @param[in] MemMapData    - MemMapData  Pointer to the memory map private data structure.
  @param[in] Sckt          - Socket Id

  @retval None

  **/
VOID
WriteTadCsrs (
  IN PSYSHOST         Host,
  IN UINT8            Sckt,
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  UINT8                                         McIndex;
  UINT8                                         MaxImc;
  UINT8                                         NumChPerMc;
  UINT8                                         ImcEnabled;


  RcDebugPrint (SDBG_MAX,
    "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n"
    "     Write TAD CSRs for Socket: %d\n"
    "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n",
    Sckt
    );

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();

  for (McIndex = 0; McIndex < MaxImc; McIndex++) {

    //
    // If iMC disabled, continue.
    //
    GetImcEnabledData (Sckt, McIndex, &ImcEnabled);
    if (ImcEnabled == 0) {
      continue;
    }

    //
    // if iMC has no memory, continue
    //
    if (IsMemTypeEmptyMc (Sckt, McIndex)) {
      continue;
    }

    //
    // 1. Program registers for patrol and sparing
    //
    WritePatrolSparingRegisters (
      Host,
      MemMapData,
      Sckt,
      McIndex
      );

    //
    // 2. Program 1LM decoder registers
    //
    Write1lmDecoderRegisters (
      Host,
      MemMapData,
      Sckt,
      McIndex
      );

#ifdef DDRT_SUPPORT
    //
    // 3. Program 2LM decoder registers if this MC participates in 2LM, but not for HBM
    // as cache.
    //
    if (IsMemTypePresentMcFunction (Sckt, McIndex, IsMemType2lm) &&
      !IsMemTypePresentMcFunction (Sckt, McIndex, IsMemTypeHbm)) {
      Write2lmDecoderRegisters (
        Host,
        MemMapData,
        Sckt,
        McIndex
        );
    }

    ForceLatencyIfRequired(Sckt, McIndex, NumChPerMc);
#endif // DDRT_SUPPORT
  } // McIndex

  WriteHbmTadCsrs (Sckt, MemMapData);

  return;
}

/**

  Write the RIR CSRs

  @param Host       - Pointer to sysHost
  @param MemMapData - MemMapData Pointer to the memory map private data structure.
  @param Sckt       - Socket number
  @param Ch         - Channel number (0-based)

  @retval N/A

**/
VOID
WriteRirForChannel (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData,
  UINT8           Sckt,
  UINT8           Ch
  )
{
  UINT8                Rir;
  UINT8                Index;
  MEMMAPDATA_CHANNEL   *MemMapChannel;
  CHANNEL_NVRAM_STRUCT *ChannelNvList;
#ifdef DDRT_SUPPORT
  DIMM_NVRAM_STRUCT    (*DimmNvList)[MAX_DIMM];
  UINT8                Dimm;
#endif // DDRT_SUPPORT
  UINT8                AepPresent = 0;
  UINT8                NumChPerMc;
  RIR_DATA             RirData;

  ChannelNvList = &(*GetChannelNvList (Host, Sckt))[Ch];
  NumChPerMc = GetNumChannelPerMc ();
  ZeroMem (&RirData, sizeof (RirData));


  // Only write CSRs if channel is enabled
  if (ChannelNvList->enabled != 0) {

    RcDebugPrint (SDBG_MAX, "  Channel - %d\n", Ch);

    MemMapChannel = &MemMapData->MemMapSocket[Sckt].MemMapChannel[Ch];

    for (Rir = 0; Rir < MAX_RIR; Rir++) {
      RirData.NmRir[Rir].RirValid   = MemMapChannel->RirValid[Rir];
      RirData.NmRir[Rir].RirLimit   = GetAdjustedRirValue (Host, Sckt, MemMapChannel->RirLimit[Rir]);

      RirData.NmRir[Rir].EncRirWays = RirEncodeRirWays (MemMapChannel->RirWay[Rir]);

      for (Index = 0; Index < MAX_RIR_WAYS; Index++) {
        RirData.NmRir[Rir].RirTgtId[Index]  = MemMapChannel->RirIntList[Rir][Index];
        RirData.NmRir[Rir].RirOffset[Index] = GetAdjustedRirValue (Host, Sckt, MemMapChannel->RirOffset[Rir][Index]);
      } /* rank way loop */
    } /* Rir loop */

#ifdef DDRT_SUPPORT
    // Find NVMCTLR DIMM on this channel - only a single NVMCTLR DIMM per channel is supported
    DimmNvList = GetDimmNvList (Host, Sckt, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) continue;
      if ((*DimmNvList)[Dimm].DcpmmPresent) {
        AepPresent = 1;
        break;
      }
    }

    // Write the FM Rir registers only if an NVMCTLR DIMM exists on this channel
    if (AepPresent == 1) {

      for (Rir = 0; Rir < MAX_RIR_DDRT; Rir++) {
        RirData.FmRir[Rir].RirValid   = MemMapChannel->RirDdrtValid[Rir];
        RirData.FmRir[Rir].RirLimit   = MemMapChannel->RirDdrtLimit[Rir];
        RirData.FmRir[Rir].EncRirWays = RirEncodeRirWays (MemMapChannel->RirDdrtWay[Rir]);

        //Update FM Rir Interleave offset register
        for (Index = 0; Index < MAX_RIR_DDRT_WAYS; Index++) {
          RirData.FmRir[Rir].RirTgtId[Index]  = MemMapChannel->RirDdrtIntList[Rir][Index];
          RirData.FmRir[Rir].RirOffset[Index] = MemMapChannel->RirDdrtOffset[Rir][Index];
        } /* rank way loop */
      } /* Rir loop */
    } /* if NVMCTLR Present */
#endif // DDRT_SUPPORT

    McSetRir (
      MemTechDdr,
      Sckt,
      Ch / NumChPerMc,
      Ch % NumChPerMc,
      AepPresent,
      &RirData
      );
  } /* if channel enabled */
  return;
}

/**

  Description: Fill in RIR CRSs for memory map from config info in host
  structure.

  @param[in] Host       - Pointer to sysHost
  @param[in] Sckt       - Socket Id
  @param[in] MemMapData - MemMapData  Pointer to the memory map private data structure

  @retval N/A

**/
VOID
WriteRirCsrs (
  IN PSYSHOST        Host,
  IN UINT8           Sckt,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8 Ch;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MAX, "\n");
  RcDebugPrint (SDBG_MAX, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrint (SDBG_MAX, "\t\tWrite RIR CSRs for Socket: %d\n", Sckt);
  RcDebugPrint (SDBG_MAX, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

  // RIRs for channels
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    WriteRirForChannel (Host, MemMapData, Sckt, Ch);
  }

  return;
}

/**

  Check if PSMI Hob exist and valid trace sizes are requested.Validate size requested against 1LM memory mapped
  above 4GB and allocate memory buffers.Fill base address and allocation error in HOB.

  @param[in]  Host                   Pointer to sysHost.
  @param[in]  MemMapData             Pointer to the memory map private data structure.

  @retval EFI_SUCCESS                The function is executed successfully without any error.
  @retval EFI_INVALID_PARAMETER      The function failed due to some illegal input.

**/
EFI_STATUS
EFIAPI
MemReservePsmiBuffers (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  EFI_HOB_GUID_TYPE             *GuidHob;
  PSMI_TRACE_INFO_HOB           *HobData;
  UINT8                         Socket;
  UINT8                         TraceIndex;
  UINT64                        RequestedTraceSize;
  UINT64                        BaseAddress;
  EFI_STATUS                    Status;

  RcDebugPrint (SDBG_MAX,"\n Enter MemReservePsmiBuffers\n");

  GuidHob = GetFirstGuidHob (&gPsmiInitDataGuid);
  if (GuidHob == NULL) {
    return EFI_SUCCESS;
  }

  HobData = GET_GUID_HOB_DATA (GuidHob);

  if (HobData->TotalSocketNum == 0 || HobData->TotalSocketNum > MAX_SOCKET) {
    RcDebugPrint (SDBG_MAX, "Invalid number of sockets for PSMI buffer allocations.\n");
    return EFI_INVALID_PARAMETER;
  }

  if (Host->var.mem.volMemMode != VOL_MEM_MODE_1LM &&
      Host->var.mem.volMemMode != VOL_MEM_MODE_MIX_1LM2LM) {
    RcDebugPrint (SDBG_MAX, "Volatile memory mode not in 1LM, cannot allocate PSMI buffers.\n");
    return EFI_INVALID_PARAMETER;
  }

  for (Socket = 0; Socket < HobData->TotalSocketNum; Socket++) {
    for (TraceIndex = 0; TraceIndex < MAX_TRACE_CACHE_TYPE; TraceIndex++) {
      BaseAddress = 0;

      //
      // Skip if trace region size is 0
      //
      if (HobData->SocketBuffer[Socket].Trace[TraceIndex].Size == 0) {
        continue;
      }

      RequestedTraceSize = HobData->SocketBuffer[Socket].Trace[TraceIndex].Size;

      //
      // Check Socket Capacity to support requested Trace Size
      //
      Status = ReserveLocalSocketMemory (MemMapData, Socket, MemType1lmDdr, RequestedTraceSize, &BaseAddress);
      if (EFI_ERROR (Status) || BaseAddress == (UINT64) NULL) {
        RcDebugPrint (SDBG_MAX, "S%d: Trace %d Could not serve request. Status: %r \n", Socket, TraceIndex, Status);
        HobData->SocketBuffer[Socket].Trace[TraceIndex].AllocationError = ERROR_OUT_OF_RESOURCES;
        continue;
      }
      HobData->SocketBuffer[Socket].Trace[TraceIndex].BaseAddress = BaseAddress;
      HobData->SocketBuffer[Socket].Trace[TraceIndex].AllocationError = 0; // No error

      RcDebugPrint (
        SDBG_MAX,
        "\n Buffer value: %llx Size: 0x%llx : %d(MB)\n",
        HobData->SocketBuffer[Socket].Trace[TraceIndex].BaseAddress,
        HobData->SocketBuffer[Socket].Trace[TraceIndex].Size,
        (UINT32) RShiftU64 (HobData->SocketBuffer[Socket].Trace[TraceIndex].Size, CONVERT_B_TO_MB)
        );
    } // Trace loop
  } // Socket loop

  return EFI_SUCCESS;
} // MemReservePsmiBuffers

/**
  Calculate Cached Range Base Address

  Calculate the cached range base address for the input SAD Index

  @param[in] SadIndex    - The SAD index corresponding to the cached range
  @param[in] BaseAddress - The base address for the input SAD index.

  @return The Cached Range Base Address

**/
UINT32
CalculateCacheRangeLow (
  IN UINT8  SadIndex,
  IN UINT32 BaseAddress
  )
{
  return BaseAddress;
}

#ifdef DDRT_SUPPORT
/**
  This function checks if the near memory to far memory (NM:FM) ratio of each socket is compliant for
  2LM configurations. The recommended NM:FM ratio is between 1:4 - 1:16.

  @param[in] Host            Pointer to sysHost.
  @param[in] MemMapData      Pointer to memory map private data structure.
  @param[in] SocketId        Socket index.

  @retval SUCCESS            The NM:FM ratio is compliant for 2LM configurations.
  @retval FAILURE            The NM:FM ratio is not compliant for 2LM configurations.
**/
UINT32
CheckNm2FmRatioCompliance (
  IN SYSHOST          *Host,
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            SocketId
  )
{
  UINT32  NmSize;
  UINT32  FmSize;

  if (!IS_DDR_2LM_OR_MIXED (Host)) {
    return SUCCESS;
  }

  if (IS_DDR_MIXED_1LM2LM (Host)) {
    NmSize = GetSocketDdrCacheSize (SocketId);
  } else {
    NmSize = GetSocketMemSize (SocketId);
  }

  if (NmSize == 0) {
    return FAILURE;
  }

  FmSize = Host->var.mem.socket[SocketId].volSize;
  if (MemMapData->PmemCaching == LegacyPmCaching) {
    FmSize += Host->var.mem.socket[SocketId].perSize;         // FM size of Appdirect-WB part in 10nm wave1 & wave2.
  } else if (MemMapData->PmemCaching == NativePmCaching) {
    FmSize += Host->var.mem.socket[SocketId].WbCachePerSize;  // FM size of Appdirect-WB part in 10nm wave3.
  }

  if (FmSize < NmSize) {
    //
    // If the NM:FM ratio is less than 1:1, UEFI FW needs to:
    //   1. Degrade system to 1LM mode.
    //   2. Reject any Appdirect-WB request.
    //   3. Recalculate memory resources by resetting MemMapState and rerunning memory mapping flow.
    //
    MemMapData->PmemCaching = NoPmCaching;
    Host->var.mem.volMemMode = VOL_MEM_MODE_1LM;
    MemMapData->MemMapState = MEM_MAP_STATE_RESOURCE_CALCULATION;
    RcDebugPrint (SDBG_MAX,
      "Socket %d: Far memory size 0x%x(64MB) is less than near memory size 0x%x(64MB).\n"
      "Insufficient resources to map memory and degrading memory to 1LM!!\n",
      SocketId,
      FmSize,
      NmSize
      );
    if (FmSize) {
      EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_NM_FM_RATIO_VIOLATION, SocketId, NO_CH, NO_DIMM, NO_RANK);
    }
    return FAILURE;
  } else if (((10 * FmSize) < (36 * NmSize)) || (FmSize > (16 * NmSize))) {
    //
    // Due to overhead, volatile capacity will never reach total capacity of the PMem
    // For 1:4 check, use 90% of actual ratio to account for this
    // For 90% adjustment, 1:4 -> 1:3.6
    //
    RcDebugPrint (SDBG_MAX,
      "Socket %d: Near memory to far memory ratio (1:%d.%d) is not in the recommended range (1:4 - 1:16)!\n",
      SocketId,
      FmSize / NmSize,
      (FmSize % NmSize) * 10 / NmSize
      );
    EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_NM_FM_RATIO_VIOLATION, SocketId, NO_CH, NO_DIMM, NO_RANK);
  }

  return SUCCESS;
}

/**

  This routine checks whether 2LM memory mode request can be supported

  @param[in] Host Pointer to sysHost

  @retval None

**/
VOID
Check2LmRequestCompliance (
  IN PSYSHOST Host
  )
{
  return;
}

/**

  This routine checks the current memory topology for POR compliance.

  @param[in] Host            Pointer to sysHost.
  @param[in] MemMapData      Pointer to memory map private data structure.

  @retval status

**/
UINT32
CheckDdrtPorCompliance (
  IN  PSYSHOST          Host,
  IN  MEMORY_MAP_DATA   *MemMapData
  )
{
  UINT8  SocketId;
  UINT32 Status;

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CheckDdrtPorCompliance Start\n");

  //
  // Check the NM:FM ratio compliance for each socket.
  //
  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if ((!CheckSocketEnabled (SocketId)) || (!CheckSocketDcpmmPresent(SocketId))) {
      continue;
    }

    Status = CheckNm2FmRatioCompliance (Host, MemMapData, SocketId);
    if (Status != SUCCESS) {
      return Status;
    }
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CheckDdrtPorCompliance End\n");

  return SUCCESS;
}

/**

  This routine checks the current memory topography for POR compliance across the sockets.
  (currently only DCPMM capacity is verified across the sockets)

  @param[in] Host        - Pointer to sysHost

  @return status

**/
UINT32
CheckDdrtPorComplianceAcrossSockets (
  IN  PSYSHOST  Host
  )
{
  DDRT_IMC_TOPOLOGY CurrentDimmPop;
  UINT8             Socket;
  UINT8             Imc;
  UINT8             Modes;
  UINT8             MaxImc;
  UINT8             S0Modes = 0;
  BOOLEAN           ModeCompliance = TRUE;
  BOOLEAN           DdrtOnS0 = FALSE;
  BOOLEAN           DdrtOnSocket = FALSE;
  MEM_NVRAM_STRUCT  *NvramMem;
  SYS_SETUP         *Setup;

  Setup = GetSysSetupPointer ();

  if (Setup->mem.EnforcePopulationPor == ENFORCE_POPULATION_POR_DIS) {
    RcDebugPrint (SDBG_MAX, "CheckPmemPorComplianceAcrossSockets - functionality disabled\n");
    return SUCCESS;
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CheckPmemPorComplianceAcrossSockets Start\n");

  MaxImc = GetMaxImc ();
  NvramMem = &Host->nvram.mem;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Check only enabled sockets
    //
    if (NvramMem->socket[Socket].enabled == 1) {
      Modes = 0;

      for (Imc = 0; Imc < MaxImc; Imc++) {
        GetImcDdrtTopology (Host, Socket, Imc, &CurrentDimmPop);
        //
        // Get mode for each socket
        //
        if (Socket == 0) {
          S0Modes |= CurrentDimmPop.Modes;
          if (CurrentDimmPop.NumDdrt) {
            DdrtOnS0 = TRUE;
          }
        } else {
          Modes |= CurrentDimmPop.Modes;
          if (CurrentDimmPop.NumDdrt) {
            DdrtOnSocket = TRUE;
          }
        }
      }
      //
      // Check Mode compliance across sockets
      //
      if ((Socket != 0) && (DdrtOnS0 == DdrtOnSocket) && ((S0Modes & MEM_MODE_MM) == 0) && (S0Modes != Modes)) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "PMem Memory Mode mismatch across sockets\n");
        ModeCompliance = FALSE;
      }
    }

    //
    // Break if the test has failed
    //
    if (ModeCompliance == FALSE) {
      break;
    }
  }

  if (ModeCompliance == FALSE) {
    EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_SOCKET_MODE_MISMATCH, NO_SOCKET, NO_CH, NO_DIMM, NO_RANK);
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "CheckPmemPorComplianceAcrossSockets End\n");

  return SUCCESS;
}

/**
  This routine checks if the system configuration has capability to run in 2LM mode. If not, it sets the
  volatile memory mode to 1LM

  @param[in] Host - Pointer to sysHost
  @param[in] MemMapData - Pointer to memory map private data structure.

**/
VOID
AdjustVolatileMemMode (
  IN PSYSHOST        Host,
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  BOOLEAN               AllowVolatileMemMode = TRUE;
  BOOLEAN               AllowCacheModeOnSocket;
  BOOLEAN               FmChNotPwrOfTwo;
  BOOLEAN               AllAppDirect;
  BOOLEAN               VolCapMismatch = FALSE;
  UINT8                 Sckt;
  UINT8                 Mc;
  UINT8                 Ch;
  UINT8                 Dimm;
  UINT8                 FmChCount;
  UINT8                 DisabledMcCount = 0;
  UINT8                 MaxImc;
  UINT8                 NumChPerMc;
  UINT8                 ImcEnabled;
  UINT8                 DimmMappedOutReason;
  UINT16                VolCap;
  UINT32                NonVolSize;
  MEM_VAR_STRUCT        *Mem;
  IMC_INFO_STRUCT       *Imc;
  SYS_SETUP             *Setup;
  CHANNEL_NVRAM_STRUCT  (*ChannelNvList)[MAX_CH];
  DIMM_NVRAM_STRUCT     (*DimmNvList)[MAX_DIMM];

  Setup = GetSysSetupPointer ();

  Mem = &Host->var.mem;
  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();
  FmChNotPwrOfTwo = FALSE;

  //
  // For each socket
  //
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (CheckSocketEnabled (Sckt) == 0) {
      continue;
    }
    AllowCacheModeOnSocket = TRUE;
    AllAppDirect = FALSE;

    //
    // Initialize FM channel count for this socket
    //
    FmChCount = 0;

    //
    // For each IMC in the socket
    //
    for (Mc = 0; Mc < MaxImc; Mc++) {
      Imc = &Mem->socket[Sckt].imc[Mc];
      //
      // Check if iMC is populated and if not populated, increment DisabledMcCount
      //
      GetImcEnabledData (Sckt, Mc, &ImcEnabled);
      if (ImcEnabled == 0) {
        DisabledMcCount++;
        continue;
      }

      //
      // Collect memory topology
      //
      for (Ch = 0; Ch < NumChPerMc; Ch++) {
        if (Host->var.mem.socket[Sckt].channelList[Ch + Mc * NumChPerMc].volSize > 0) {
          FmChCount++;
        }
      }

      //
      // Check if App Direct is on the system
      // If any 100% App Direct on system, don't output the 2LM EWL
      //
      if (Host->var.mem.dfxMemVars.dfxDimmManagement == DIMM_MGMT_BIOS_SETUP) {
        NonVolSize = Imc->perSize;
      } else {
        NonVolSize = Imc->NonVolSize;
      }
      if ((NonVolSize > 0) && ((Imc->volSize == 0) && (Imc->WbCachePerSize == 0))) {
        AllAppDirect = TRUE;
      }

      //
      // Each iMC populated, must have DDR memory and one NVMDIMM dimm with volatile memory,
      // then 2LM is possible on this socket
      // bcz in 2LM mode, X-tile is not supported on SKX/CLX/ICX for DDRT and AD-WB in SPR;
      //
      if ((Imc->memSize == 0) || ((Imc->volSize == 0) && (Imc->WbCachePerSize == 0))) {
        AllowCacheModeOnSocket = FALSE;
      }

      //
      // Perform additional check for native AD-WB
      // Native AD-WB shall exist only with a volatile memory mode (2LM/1LM + 2LM)
      //
      if (AllowCacheModeOnSocket) {
        if ((Host->var.mem.socket[Sckt].WbCachePerSize != 0) &&
          (MemMapData->PmemCaching == NativePmCaching)) {
          //
          // Do not allow native AD-WB if no volatile partition or DDR is 100% cache
          //
          if (((Imc->memSize > 0) && (Host->var.mem.volMemMode != VOL_MEM_MODE_MIX_1LM2LM)) &&
            (Imc->volSize == 0)) {
            AllowCacheModeOnSocket = FALSE;
          }
        }
      }
    } // Mc loop

    if (MaxImc == DisabledMcCount) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "There is NO memory populated in this socket\n");
    } else {
      //
      // For 2LM far memory channels must be power of two
      //
      if (AllowCacheModeOnSocket && (FmChCount != 0) && !IsPowerOfTwo (FmChCount)) {
        //
        // 2LM isn't supported in current configuration
        //
        AllowCacheModeOnSocket = FALSE;
        FmChNotPwrOfTwo = TRUE;
      }

      //
      // Perform extra check for X-Tile 2LM if we cannot establish in-tile 2LM and X-Tilw 2LM is allowed (by default) on this socket.
      //
      if ((!AllowCacheModeOnSocket) && (MemMapData->MemMapSocket[Sckt].XTile2lm)) {
        if (CheckXTile2lmPorConfig (Host, MemMapData, Sckt)) {
          AllowCacheModeOnSocket = TRUE;
        } else {
          MemMapData->MemMapSocket[Sckt].XTile2lm = FALSE;
        }
      } else {
        MemMapData->MemMapSocket[Sckt].XTile2lm = FALSE;
      }

      //
      // For Memory Mode, all DCPMMs should have the same (non-zero) volatile capacity
      //
      if (AllowCacheModeOnSocket) {
        VolCap = 0;
        ChannelNvList = GetChannelNvList(Host, Sckt);
        for (Ch = 0; Ch < MAX_CH; Ch++) {
          if ((*ChannelNvList)[Ch].enabled == 0) {
            continue;
          }
          DimmNvList = GetDimmNvList (Host, Sckt, Ch);

          for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
            if ((*DimmNvList)[Dimm].DcpmmPresent) {
              //
              // Look for present and not mapped out DIMMs (POR enforcement can degrade non-POR population
              // to next available POR population, so we should exclude DIMMs mapped out by POR Population
              // enforcement from the volatile capacity check)
              //
              if (IsNvDimmUnmapped(Sckt, Ch, Dimm, &DimmMappedOutReason) &&
                  (DimmMappedOutReason == NVDIMM_UNMAPPED_POPULATION_NOT_POR)) {
                continue;
              }

              //
              // Skip DIMMs re-mapped, as a part of ADx1 fallback mechanism
              // (they have the volatile memory cleared, to avoid non-POR
              // memory mode / mix mode configurations)
              //
              if (IsNvDimmRemapped (Sckt, Ch, Dimm)) {
                continue;
              }

              if (VolCap == 0) {
                VolCap = (*DimmNvList)[Dimm].volCap;
                if (VolCap == 0) {
                  VolCapMismatch = TRUE;
                  AllowCacheModeOnSocket = FALSE;
                  break;
                }
              } else if (VolCap != (*DimmNvList)[Dimm].volCap) {
                VolCapMismatch = TRUE;
                AllowCacheModeOnSocket = FALSE;
                break;
              }
            }
          }
        }
      }
    }

    //
    // If 2LM isn't supported on this socket, it isn't supported on the platform
    //
    if (!AllowCacheModeOnSocket) {
      if (VolCapMismatch) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "For Memory Mode, all Pmem should have the same (non-zero) Volatile Capacity\n");
        EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_DDRT_PARTITION_MISMATCH, Sckt, NO_CH, NO_DIMM, NO_RANK);
      }
      if (!(IsCpuAndRevision (CPU_SPRSP, REV_ALL)) && (Host->nvram.mem.DcpmmPresent == 1) && (!AllAppDirect) && (!FmChNotPwrOfTwo)) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "For 2LM need at least 1 DDR and 1 Pmem with volatile size > 0 per populated iMC\n");
        EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_2LM_IMC_MEM_MISMATCH, Sckt, NO_CH, NO_DIMM, NO_RANK);
      } else if (FmChNotPwrOfTwo) {
        RcDebugPrintWithDevice (SDBG_MINMAX, Sckt, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "For 2LM need far memory channels number to be power of two (FM Channels %d)\n", FmChCount);
        EwlOutputType1 (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_2LM_FM_CH_NOT_PWR_OF_TWO, Sckt, NO_CH, NO_DIMM, NO_RANK);
      }
      AllowVolatileMemMode = FALSE;
      //
      // exit MC loop, as 2LM cannot be supported on the platform
      //
      break;
    }

    //
    // clear DisabledMcCount
    //
    DisabledMcCount = 0;
  } // Socket loop

  //
  // If this configuration cannot support 2LM memory mode, force memory mode to 1LM
  //
  if (!AllowVolatileMemMode) {
    Host->var.mem.volMemMode = VOL_MEM_MODE_1LM;
    if (Host->nvram.mem.DcpmmPresent) {
      RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "***2LM could not be supported in this memory configuration. So Initializing VolMemMode to 1LM.***\n"
        );
    }

    //
    // Configuration changed to 1LM so RT2LM is not used
    //
    if (MemMapData->Is2LmUsingRt2lm) {
      MemMapData->Is2LmUsingRt2lm = FALSE;
    }
  }
}
#endif // DDRT_SUPPORT

/**
  Get SKU limit default.

  @return SOC specific default SKU limit.

**/
UINT16
EFIAPI
GetSkuLimitDefault (
  VOID
  )
{
  //
  // On 10nm, the Mailbox CMD return data needs to be remapped
  //
  return 0xFFFF;
}

/**

  Called by lib external to MemDecodeLib to create SAD entries.

  @param[in]      MemMapData            Pointer to the memory map private data structure.
  @param[in]      SocketNum             Socket id.
  @param[in]      MemType               Type of memory that the interleave request corresponds to.
  @param[in]      McWays                Number of IMC's within socket participating in the interleave.
  @param[in]      ChWays                Number of channels between IMC's within socket participating in the interleave.
  @param[in]      TgtInterBitmap        Bit map of stacks participating in the interleave.
  @param[in]      NmTgtInterBitmap      Bit map of stacks participating in the interleave as near memory.
  @param[in]      ChannelInterBitmap    Per IMC Bit map of channels participating in the interleave.
  @param[in]      FmChannelInterBitmap  Per IMC Bit map of channels participating in the interleave.
  @param[in]      SadIndex              Index of the available SAD table entry to fill with data.
  @param[in,out]  InterleaveLimit       Total interleave address limit.
  @param[in,out]  Cluster               SNC cluster this SAD applies to (ignored if SNC is disabled).

  @retval     EFI_SUCCESS               Sad entries created
  @retval     EFI_UNSUPPORTED           Sad creation failed

**/
EFI_STATUS
EFIAPI
AddSadEntries10nmHl (
  IN     MEMORY_MAP_DATA *MemMapData,
  IN     UINT8            SocketNum,
  IN     UINT16           MemType,
  IN     UINT8            McWays,
  IN     UINT8            ChWays,
  IN     UINT8            TgtInterBitmap,
  IN     UINT8            NmTgtInterBitmap,
  IN     UINT8            ChannelInterBitmap[MAX_IMC],
  IN     UINT8            FmChannelInterBitmap[MAX_IMC],
  IN     UINT8            SadIndex,
  IN OUT UINT32          *InterleaveLimit,
  IN OUT UINT8            Cluster
  )
{
  UINT32 Status;
  SYSHOST *Host;

  Host = GetSysHostPointer ();
  Status = AddSadEntries (
    Host,
    MemMapData,
    SocketNum,
    MemType,
    ONE_WAY,
    McWays,
    ChWays,
    TgtInterBitmap,
    NmTgtInterBitmap,
    ChannelInterBitmap,
    FmChannelInterBitmap,
    SadIndex,
    InterleaveLimit,
    Cluster,
    FALSE
    );

  if (Status != SUCCESS) {
    return EFI_UNSUPPORTED;
  } else {
    return EFI_SUCCESS;
  }

}

/**

  Called by lib external to MemDecodeLib to get SAD base.

  @param[in]  MemMapData  Pointer to the memory map private data structure.
  @param[in]  SocketNum   Socket id.
  @param[in]  SadIndex    Index of the available SAD table entry to fill data in.

  @return                 The limit of the previous SAD

**/
UINT32
EFIAPI
GetSadBase10nmHl (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8            SocketNum,
  IN  UINT8            SadIndex
  )
{
  SYSHOST *Host;

  Host = GetSysHostPointer ();

  return GetSadBase (Host, MemMapData, SocketNum, SadIndex);
}

/**

  Displays the TAD table data structure in the Host structure.

  @param[in] Socket       Current Socket (0-Based)
  @param[in] MemMapData   Pointer to the memory map private data structure.

  @retval None

**/
VOID
DisplayTadTableEx (
  IN  UINT8           Sckt,
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  DisplayHbmTadTables (Sckt, MemMapData);

  return;
}

#ifdef DDRT_SUPPORT
/**
  read the digests in fw_digest_csr[0-11] and fw_digest1_csr[0-11]

  @param[in] Host             Pointer to the sysHost Structure.

  @retval MRC_STATUS_SUCCESS  This function is executed successfully.

**/
MRC_STATUS
FfwDigestRead (
  IN PSYSHOST        Host
  )
{
  UINT8            SocketId;
  UINT8            ChId;
  UINT8            DimmId;
  UINT8            MaxCh;
  MRC_STATUS       Status;
  MEMORY_MAP_HOST  *MemMapHost;
  UINT8            Index1;
  UINT8            Index2;

  MemMapHost = GetMemMapHostPointer ();
  MaxCh = GetMaxChDdr ();
  Index1 = 0;
  Index2 = 0;

  for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
    if (!CheckSocketEnabled (SocketId)) {
      continue;
    }

    for (ChId = 0; ChId < MaxCh; ChId++) {
      if (!IsChannelEnabled (SocketId, ChId)) {
        continue;
      }

      for (DimmId = 0; DimmId < MAX_DIMM; DimmId++) {
        if (!IsDcpmmPresentDimm (SocketId, ChId, DimmId)) {
          continue;
        }

        Status = GetFfwDigest (Host, SocketId, ChId, DimmId, NGN_FFW_CSR_NUM,
                               MemMapHost->SocketInfo[SocketId].ChannelInfo[ChId].FfwDigestPerDimm[DimmId].Digest[0],
                               MemMapHost->SocketInfo[SocketId].ChannelInfo[ChId].FfwDigestPerDimm[DimmId].Digest[1]);
        if (Status != MRC_STATUS_SUCCESS) {
          return Status;
        }

        //
        // Display the full FFW digest data.
        //
        for (Index1 = 0; Index1 < NGN_FFW_BANK_NUM; Index1++) {
          RcDebugPrintWithDevice (SDBG_MAX, SocketId, ChId, DimmId, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                  "FFW Digest%d =", Index1);
          for (Index2 = 0; Index2 < NGN_FFW_CSR_NUM; Index2++) {
            RcDebugPrint (SDBG_MAX, " %08x", MemMapHost->SocketInfo[SocketId].ChannelInfo[ChId].FfwDigestPerDimm[DimmId].Digest[Index1][Index2]);
          }
          RcDebugPrint (SDBG_MAX, "\n");
        }
      }
    }
  }

  return MRC_STATUS_SUCCESS;
}
#endif // DDRT_SUPPORT
