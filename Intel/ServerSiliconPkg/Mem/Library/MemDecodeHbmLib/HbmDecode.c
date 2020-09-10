/** @file
  Interface source file for the HBM library class.

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

#include <Base.h>
#include <Uefi.h>
#include <Library/KtiApi.h>
#include <SysHost.h>
#include <Library/SysHostPointerLib.h>
#include <MaxSocket.h>
#include <KtiSetupDefinitions.h>
#include <Library/MemoryServicesLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RcDebugLib.h>
#include <Library/MemRcLib.h>
#include <Library/HbmCoreLib.h>
#include <Library/HbmMemLib.h>
#include <Library/ChaLib.h>
#include <MemDecodeCommonIncludes.h>
#include <MemHostChipCommon.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemDecodeLib.h>
#include "HbmDecode.h"
#include <Guid/SocketCommonRcVariable.h>


//
// SPR HBM stacks are numbered as follows:
//
//   0 | 2
//  ---+---
//   1 | 3
//
// Rules:
// 1. To simplify the FM target decision for 2LM, the left side CHAs must route
//    to a different stack than the right side CHAs, even if that requires
//    routing asymmetrically (i.e., left CHAs to stack 3, or right CHAs to
//    stack 1).
// 2. The routing logic cannot support three stacks. If three are enabled, only
//    two can be mapped. They are chosen to be symmetrical per rule 1.
//
CONST UINT8 mStackRoutingByEnableMaskSpr[][MAX_CLUSTERS] = {
// CHA Cluster   HBM IO Stack
// 0  1  2  3      3 2 1 0
  {0, 0, 0, 0}, // 0 0 0 0 - no stacks enabled, invalid
  {0, 0, 0, 0}, // 0 0 0 1
  {1, 1, 1, 1}, // 0 0 1 0
  {0, 0, 1, 1}, // 0 0 1 1 - asymmetrical
  {2, 2, 2, 2}, // 0 1 0 0
  {0, 0, 2, 2}, // 0 1 0 1
  {1, 1, 2, 2}, // 0 1 1 0
  {0, 0, 2, 2}, // 0 1 1 1 - three stacks enabled, only two can be mapped
  {3, 3, 3, 3}, // 1 0 0 0
  {0, 0, 3, 3}, // 1 0 0 1
  {1, 1, 3, 3}, // 1 0 1 0
  {0, 0, 3, 3}, // 1 0 1 1 - three stacks enabled, only two can be mapped
  {2, 2, 3, 3}, // 1 1 0 0 - asymmetrical
  {0, 0, 3, 3}, // 1 1 0 1 - three stacks enabled, only two can be mapped
  {1, 1, 2, 2}, // 1 1 1 0 - three stacks enabled, only two can be mapped
  {0, 1, 2, 3}  // 1 1 1 1 - POR
};

UINT8 mChannelInterBitmap[] = {0x3, 0x3, 0x3, 0x3};

//
// Declare a constant matrix describing target lists based on which HBM IO
// stack is selected. There are 4 MCs in each cluster, with 2/2/2/2
// channels.
//
CONST UINT8 mRouteTableTargetList[][MAX_HBM_IO_CH] = {
  {HBM_IO_EDC_START_TGT,   HBM_IO_EDC_START_TGT,   HBM_IO_EDC_START_TGT+1, HBM_IO_EDC_START_TGT+1,
   HBM_IO_EDC_START_TGT+2, HBM_IO_EDC_START_TGT+2, HBM_IO_EDC_START_TGT+3, HBM_IO_EDC_START_TGT+3},
  {HBM_IO_EDC_START_TGT,   HBM_IO_EDC_START_TGT,   HBM_IO_EDC_START_TGT+1, HBM_IO_EDC_START_TGT+1,
   HBM_IO_EDC_START_TGT+2, HBM_IO_EDC_START_TGT+2, HBM_IO_EDC_START_TGT+3, HBM_IO_EDC_START_TGT+3},
  {HBM_IO_EDC_START_TGT+3, HBM_IO_EDC_START_TGT+3, HBM_IO_EDC_START_TGT+2, HBM_IO_EDC_START_TGT+2,
   HBM_IO_EDC_START_TGT+1, HBM_IO_EDC_START_TGT+1, HBM_IO_EDC_START_TGT,   HBM_IO_EDC_START_TGT},
  {HBM_IO_EDC_START_TGT+3, HBM_IO_EDC_START_TGT+3, HBM_IO_EDC_START_TGT+2, HBM_IO_EDC_START_TGT+2,
   HBM_IO_EDC_START_TGT+1, HBM_IO_EDC_START_TGT+1, HBM_IO_EDC_START_TGT,   HBM_IO_EDC_START_TGT}
};

//
// Declare a constant channel list for HBM. There are four MCs in each cluster.
//
// This should never need to change, because all 8 channels will always be equal
// and fully interleaved.
//
CONST UINT8 mRouteTableChannelList[MAX_HBM_IO_CH] = {0, 1, 0, 1, 0, 1, 0, 1};

/**
  Intitialize HBM channel and MC bitmaps

  @param[in,out] ChBitmap   Array of channel bitmaps.
  @param[in,out] McBitMap   Memory controller bitmap.

**/
VOID
EFIAPI
LoadHbm2lmInterleaveData (
  IN OUT UINT8 *ChBitmap,
  IN OUT UINT8 *McBitMap
  )
{
  UINT8 MaxImcPerIo;
  UINT32 Index;

  CopyMem (ChBitmap, mChannelInterBitmap, sizeof (mChannelInterBitmap));
  MaxImcPerIo = GetMaxImcPerIoHbm ();
  *McBitMap = 0;

  for (Index = 0; Index < MaxImcPerIo; ++Index) {
    *McBitMap |= BIT0 << Index;
  }

}

/**
  Allocate HBM private data structure

  @param[in,out] MemMapData   The private data structure for memory map.

**/
VOID
EFIAPI
InitHbmMemoryMap (
  IN OUT MEMORY_MAP_DATA *MemMapData
  )
{
  if (!IsHbmSku()) {
    MemMapData->HbmMemoryMapData = NULL;
    return;
  }

  MemMapData->HbmMemoryMapData = RcAllocatePool (sizeof(HBM_MEMORY_MAP_CONFIG) );
  if (MemMapData->HbmMemoryMapData == NULL) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "HBM: Out of resources\n");
    ASSERT (FALSE);
  }

}

/**
  Get HBM Socket information

  @param[in] MemMapData   The private data structure for memory map.
  @param[in] Socket       Current Socket (0-Based)

  @return                 Pointer to HBM private data.
**/
HBM_MEMORY_MAP_SOCKET_CONFIG *
GetHbmSocketData (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Socket
  )
{
  return &((HBM_MEMORY_MAP_CONFIG *)(MemMapData->HbmMemoryMapData))->HbmSocketData[Socket];
}


/**
  Test if HBM is enabled.

  Test if the HBM stacks are enabled.

  @param[in] MemMapData   The private data structure for memory map.

  @retval  TRUE   HBM is enabled.
  @retval  FALSE  HBM is not enabled.
**/
BOOLEAN
EFIAPI
IsHbmEnabled (
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  if (!IsHbmSku() || (MemMapData->HbmMemoryMapData == NULL)) {
    return FALSE;
  }

  return (GetHbmSocketData(MemMapData, 0)->StackCount ? TRUE : FALSE);
}

/**
  Retrieve HBM stack count.

  Return the number of enabled HBM stacks on socket.

  @param[in] MemMapData   The private data structure for memory map.
  @param[in] Socket       Current Socket (0-Based)

  @return                 Number of enabled HBM stacks.

**/
UINT8
EFIAPI
GetHbmStackCount (
  IN MEMORY_MAP_DATA *MemMapData,
  IN  UINT8          Socket
  )
{
  if (MemMapData->HbmMemoryMapData == NULL) {
    return 0;
  }

  return GetHbmSocketData(MemMapData, Socket)->StackCount;
}



/**
  Check if we have HBM in quadrant mode.

  @param[in] MemMapData   The private data structure for memory map.
  @param[in] Socket       Current Socket (0-Based)

  @retval    True  HBM is in quadrant mode
  @retval    False HBM is not in quadrant mode

**/
BOOLEAN
IsHbmInQuadrantMode (
  IN MEMORY_MAP_DATA *MemMapData,
  IN  UINT8          Socket
  )
{
  if (GetHbmStackCount (MemMapData, Socket) == MAX_HBM_IO) {
    if (!IfFeatureEnabled (TypeSnc)) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Populate HBM size fields.

  Populate the HBM memory map private data structure with the size of available
  HBM in the system, by reading the output data from HBM IO training.

  @param[in] MemMapData   The private data structure for memory map.
  @param[in] Socket       Current Socket (0-Based)

  @return                 Total amount of HBM in the Socket expressed in
                          units of 64MB.
**/
UINT32
EFIAPI
PopulateHbmSizeFields (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Socket
  )
{
  EFI_STATUS Status;
  UINT8  StackValidMap;
  UINT32 HbmSize64;
  UINT32 HbmSize64Total;
  UINT8  Stack;
  UINT8  StackCount;
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;

  if (MemMapData->HbmMemoryMapData == NULL) {
    return 0;
  }

  HbmSocket = GetHbmSocketData(MemMapData, Socket);

  Status = GetHbmStackInfo (Socket, &HbmSocket->HbmStackInfo);
  if (EFI_ERROR (Status)) {
    return 0;
  }

  StackValidMap = HbmSocket->HbmStackInfo.HbmStackValidBitMask;
  HbmSize64 = HbmSocket->HbmStackInfo.MemSizePerStack;
  HbmSize64Total = 0;
  StackCount = 0;

  for (Stack = 0; Stack < MAX_HBM_IO; ++Stack) {
    if (StackValidMap & (BIT0 << Stack)) {
      HbmSize64Total += HbmSize64;
      ++StackCount;
    }
  }

  HbmSocket->StackCount = StackCount;

  if (StackCount == 0) {
    return 0;
  }

  if ((StackCount != MIN_STACK_COUNT) && (StackCount != MAX_HBM_IO)) {
    //
    // Invalid number of stacks
    //
    ZeroMem (HbmSocket, sizeof (HBM_MEMORY_MAP_SOCKET_CONFIG));
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "HBM: Stack count not supported: %d\n", StackCount);
    return 0;
  }

  if (StackCount != MAX_HBM_IO) {
    //
    // non conforming config. disable UMA based clustering
    //
    MemMapData->UmaBasedClustersPerSocket = UMABASEDCLUSTERING_DISABLED;
  }

  HbmSocket->SizePerStack = HbmSize64;
  HbmSocket->ChannelCapacity = HbmSize64 / GetNumChannelPerIoHbm ();

  if (IfFeatureEnabled (TypeSnc)) {
    if (GetNumOfClusterPerSystem () != KTI_SNC4) {
      ZeroMem (HbmSocket, sizeof (HBM_MEMORY_MAP_SOCKET_CONFIG));
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "HBM: SNC enabled but not SNC-4\n");
      return 0;
    }
    HbmSocket->StackRouting = mStackRoutingByEnableMaskSpr[ALL_STACKS_SPR];
  } else {
    HbmSocket->StackRouting = mStackRoutingByEnableMaskSpr[StackValidMap];
  }

  return HbmSize64Total;
}

/**
  Get the HBM Cache Size in units of 64MB

  Assumes we are in Hbm cache mode

  @param[in] MemMapData The private data structure for memory map.
  @param[in] Socket       Socket number


  @return The HBM Cache Size in units of 64MB

**/
UINT32
EFIAPI
GetHbmCacheSize (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8            Socket
  )
{
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;

  if (MemMapData->HbmMemoryMapData == NULL) {
    return 0;
  }

  HbmSocket = GetHbmSocketData(MemMapData, Socket);

  return HbmSocket->ChannelCapacity;
}

/**
  Check if the Socket is using HBM memory.

  @param[in] MemMapData   The private data structure for memory map.
  @param[in] Socket       Current Socket (0-Based)

  @retval    TRUE    HBM is enabled in Socket.
  @retval    FALSE   HBM is disabled in Socket.
**/
BOOLEAN
EFIAPI
IsHbmEnabledInSkt (
  IN MEMORY_MAP_DATA *MemMapData,
  IN  UINT8          Socket
  )
{
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;

  if (Socket >= MAX_SOCKET) {
    return FALSE;
  }

  if (MemMapData->HbmMemoryMapData == NULL) {
    return FALSE;
  }

  HbmSocket = GetHbmSocketData(MemMapData, Socket);

  if ((HbmSocket->StackCount != 0) && (HbmSocket->SizePerStack != 0)){
    return TRUE;
  }

  return FALSE;
}

/**
  Get HBM Flat Interleave List


  @return  Flat interleave list for HBM
**/
UINT32
EFIAPI
GetHbmFlatInterleaveList (
  VOID
  )
{
  return HBM_FLAT_INTERLEAVE_LIST;
}

/**
  Get HBM route tables.

  Get route tables based on the enabled HBM IO stacks for a cluster.

  @param[in]   MemMapData  Pointer to the memory map private data structure.
  @param[in]   Socket      Current Socket (0-Based)
  @param[in]   Cluster     Cluster in the Socket.
  @param[out]  HalfBitmap  Pointer to a bitmap of the halves supported.
  @param[out]  H0ChanRt0   Pointer to the H0 channel route table Value.
  @param[out]  H0TargRt0   Pointer to the H0 target route table Value.
  @param[out]  H1ChanRt0   Pointer to the H1 channel route table Value.
  @param[out]  H1TargRt0   Pointer to the H1 target route table Value.
**/
VOID
GetHbmRouteTables (
  IN MEMORY_MAP_DATA *MemMapData,
  IN        UINT8    Socket,
  IN        UINT8    Cluster,
      OUT   UINT8    *HalfBitmap,
      OUT   UINT32   *InputH0ChanRt0,
      OUT   UINT32   *InputH0TargRt0,
      OUT   UINT32   *InputH1ChanRt0,
      OUT   UINT32   *InputH1TargRt0
  )
{
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;
  CONST UINT8                   *H0TargArray;
  UINT8                         Stack;
  UINT32                        Index;

  if (InputH0ChanRt0 == NULL || InputH0TargRt0 == NULL ||
      InputH1ChanRt0 == NULL || InputH1TargRt0 == NULL) {
    return;
  }

  *HalfBitmap = 0;

  *InputH0ChanRt0 = 0;
  *InputH0TargRt0 = 0;
  *InputH1ChanRt0 = 0;
  *InputH1TargRt0 = 0;

  if (!GetHbmStackCount(MemMapData, Socket)) {
    return;
  }

  HbmSocket = GetHbmSocketData (MemMapData, Socket);
  Stack = HbmSocket->StackRouting[Cluster];
  H0TargArray = mRouteTableTargetList[Stack];

  //
  // Get H0 channels for route table.
  //
  for (Index = 0; Index < MAX_RT_WAYS; ++Index) {
    *InputH0ChanRt0 |= mRouteTableChannelList[Index] << (Index * RT_CHAN_WIDTH);
  }

  //
  // Get H0 targets for route table.
  //
  for (Index = 0; Index < MAX_RT_WAYS; ++Index) {
    *InputH0TargRt0 |= H0TargArray[Index] << (Index * RT_TARG_WIDTH);
  }

  *HalfBitmap = (BIT0 << RtHalf0);

}


/** Write Route table data for Hbm.

  Write Route Table 0 data to the CHAs in the cluster

  @param[in]  MemMapData - Pointer to the memory map private data structure.
  @param[in]  Socket     - Socket Id
  @param[in]  Cluster    - Cluster ID in socket, only 0 if socket is not clustered.
  @param[in]  TotCluster - Total number of clusters enabled in a socket.

  @retval None

**/
VOID
EFIAPI
WriteHbmRouteTables (
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            Socket,
  IN UINT8            Cluster,
  IN UINT8            TotCluster
  )
{
  UINT32 Rt0[MaxRouteTableDestination][MaxRtHalves];
  UINT8  HalfBitmap;

  if (!IsHbmEnabledInSkt (MemMapData, Socket)) {
    return;
  }

  //
  // Get HBM route tables for channel and target IDs, per cluster
  //
  GetHbmRouteTables (
    MemMapData,
    Socket,
    Cluster,
    &HalfBitmap,
    &Rt0[RouteTableChannel][RtHalf0],
    &Rt0[RouteTableTarget][RtHalf0],
    &Rt0[RouteTableChannel][RtHalf1],
    &Rt0[RouteTableTarget][RtHalf1]
    );

  PrintRouteTable (Rt0[RouteTableChannel], Rt0[RouteTableTarget], "0", Cluster);

  WriteRouteTables (RouteTable0, Rt0, Socket, Cluster, TotCluster, HalfBitmap);

  return;
}

/**

  Get highest address of the HBM memory range being cached in the remote far memory.

  @param[in] MemMapData    Pointer to the memory map private data structure.
  @param[in] Socket        Socket Id
  @param[in] IoStack       Index of the HBM IO stack.
  @param[in] IoClusterId   Cluster Id for the stack for multicast access.

  @retval Highest address of the memory range being cached within the MC in XTile.
          0 if 2LM is not Xtile flow enabled.
**/
UINT32
GetHbmTadLimitForXTileNm (
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT8           Socket,
  IN  UINT8           IoStack,
  IN  UINT8           IoClusterId
  )
{
  SYSHOST         *Host;
  UINT8           SadIndex;
  SAD_TABLE       *SadEntry;
  MEM_VAR_STRUCT  *Mem;
  UINT8           BeginSadIndex;
  UINT8           EndSadIndex;
  UINT8           ClusterId;
  BOOLEAN         CacheRangeLowAssigned;
  UINT32          NmCacheLow;
  UINT32          NmCacheHigh;

  Host = GetSysHostPointer ();
  NmCacheLow = 0;
  NmCacheHigh = 0;
  Mem = &Host->var.mem;
  CacheRangeLowAssigned = FALSE;

  if (MemMapData->NumberofChaDramClusters == 1) {
    ClusterId = 0;
  } else {
    ClusterId = IoClusterId;
  }

  //
  // Get the highest limit of 2LM SAD mapped to the FM MC.
  //
  BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
  EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);

  //
  // Scan through the SADs to find the lowest address of the memory range that is being cached.
  //
  for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
    SadEntry = &Mem->socket[Socket].SAD[SadIndex];
    if (SadEntry->Enable == 0) {
      break;
    }

    if (SadEntry->local == 0) {
      continue;
    }

    //
    // Skip SADs that are not of MemType HBM 2LM
    //
    if (!IsMemType2lmHbm (SadEntry->type)) {
      continue;
    }

    //
    // Skip if the Stack is not part of the SAD
    //
    if (SadEntry->NmImcInterBitmap && (BIT0 << IoStack) == 0) {
      continue;
    }

    if (!CacheRangeLowAssigned) {
      NmCacheLow = Mem->socket[Socket].SAD[SadIndex].Base;
      CacheRangeLowAssigned = TRUE;
    }

    if (SadEntry->Limit > NmCacheHigh) {
      NmCacheHigh = SadTadEncodeLimit (SadEntry->Limit);
    }
  }

  return NmCacheHigh;
}


/** Write Mesh to Memory CSRs for Hbm.

  This function writes mesh to memory CSR registers

  @param[in]  MemMapData - Pointer to the memory map private data structure.
  @param[in]  Socket     - Socket Id

**/
VOID
EFIAPI
WriteHbmMesh2MemCsrs (
  IN MEMORY_MAP_DATA  *MemMapData,
  IN UINT8            Socket
  )
{
  HBM_MEMORY_MAP_SOCKET_CONFIG *HbmSocket;
  HBM_MEMORY_MAP_MC_DATA       *HbmMcData;
  HBM_M2M_MISC_DATA            HbmM2mMiscData;
  HBM_M2M_TAD_DATA             HbmM2mTadData;
  TAD_TABLE                    *TadEntry;
  UINT8                        TadIndex;
  SAD_TABLE                    *SadEntry;
  UINT8                        IoStack;
  SYSHOST                      *Host;

  if (MemMapData->HbmMemoryMapData == NULL) {
    return;
  }

  HbmSocket = GetHbmSocketData(MemMapData, Socket);

  if ((HbmSocket->StackCount == 0) || (HbmSocket->SizePerStack == 0)){
    return;
  }

  RcDebugPrint (SDBG_MAX, "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  RcDebugPrint (SDBG_MAX, "\t\tWrite HBM MESH2MEM CSRs for Socket: %d\n", Socket);
  RcDebugPrint (SDBG_MAX, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

  Host = GetSysHostPointer ();
  for (IoStack = 0; IoStack < MAX_HBM_IO; IoStack++) {
    if (((BIT0 << IoStack) & HbmSocket->HbmStackInfo.HbmStackValidBitMask) == 0) {
      continue;
    }
    HbmMcData = &HbmSocket->McHbm[IoStack];
    RcDebugPrint (SDBG_MAX, "HBM Stack: %d\n", IoStack);

    //
    // Program miscellaneous Mesh2Mem register(s).
    //
    ZeroMem (&HbmM2mMiscData, sizeof (HbmM2mMiscData));
    if (Host->var.mem.CacheMemType == CACHE_TYPE_HBM_CACHE_DDR) {
      HbmM2mMiscData.NmCaching = TRUE;
    }
    RcDebugPrint (SDBG_MAX, "\tMesh2Mem misc: nmcaching: %x", HbmM2mMiscData.NmCaching);
    if (HbmM2mMiscData.NmCaching) {
      RcDebugPrint (SDBG_MAX, " wrcmpfromnm: 1\n");
    } else {
      RcDebugPrint (SDBG_MAX, "\n");
    }
    WriteHbmM2mMiscData (Socket, IoStack, &HbmM2mMiscData);

    //
    // Program Mesh2mem TAD registers
    //
    for (TadIndex = 0; TadIndex < MAX_TAD_RULES_10NM; TadIndex++) {
      TadEntry = &HbmMcData->TadTable[TadIndex];
      if (TadEntry->Enable == 0) {
        break;
      }
      SadEntry = &Host->var.mem.socket[Socket].SAD[TadEntry->SADId];
      if (!IsMemTypeHbm (SadEntry->type)) {
        continue;
      }
      ZeroMem (&HbmM2mTadData, sizeof (HbmM2mTadData));
      HbmM2mTadData.TadId = TadIndex;
      HbmM2mTadData.DdrTadId = TadIndex;
      if (TadEntry->Limit > 0) {
        HbmM2mTadData.AddressLimit = SadTadEncodeLimit (TadEntry->Limit);
      } else {
        HbmM2mTadData.AddressLimit = GetHbmTadLimitForXTileNm (MemMapData, Socket, IoStack, SadEntry->Cluster);
      }
      HbmM2mTadData.NmCacheableValid = (BOOLEAN) (SadEntry->type != MemType1lmHbm);

      RcDebugPrint (SDBG_MAX, "\tMesh2Mem TAD: tadid:0x%x ddrtadid:0x%x nmcacheablevld:0x%x addresslimit[51:26]:0x%x\n",
        HbmM2mTadData.TadId,
        HbmM2mTadData.DdrTadId,
        HbmM2mTadData.NmCacheableValid,
        HbmM2mTadData.AddressLimit
        );
      WriteHbmM2mTadData (Socket, IoStack, &HbmM2mTadData);
    }
  } // IoStack
}


/**
  Create a SAD rule for HBM as flat memory.

  Create a SAD rule for a region of memory containing only HBM as flat memory.
  If SNC is enabled by policy, then this function will create a SAD
  rule for the cluster specified by Cluster. Otherwise, Cluster is ignored.

  @param[in] MemMapData   Pointer to the memory map private data structure.
  @param[in] Socket       Current Socket (0-Based)
  @param[in] SadIndex     Index of the SAD rule to create.
  @param[in] Cluster      If SNC is enabled, cluster for which to create a
                                             rule.

  @retval  EFI_SUCCESS            An HBM SAD rule was successfully created.
  @retval  EFI_INVALID_PARAMETER  An HBM SAD rule was not created.
**/
EFI_STATUS
EFIAPI
CreateHbmFlatSadRule (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Socket,
  IN UINT8           SadIndex,
  IN UINT8           Cluster
  )
{
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;
  UINT32                      PrevLimit;
  UINT32                      Limit;
  UINT8                       TargetWays;
  UINT8                       TargetBitmap;
  EFI_STATUS                  Status;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "HBM: CreateHbmFlatSadRule\n"
    );

  if (Socket >= MAX_SOCKET || SadIndex >= ((Cluster * SAD_RULES_10NM) + SAD_RULES_10NM)) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "HBM: CreateHbmFlatSadRule = Error: SadIndex = 0x%X\n",
      SadIndex
      );

    return EFI_INVALID_PARAMETER;
  }

  if (MemMapData->HbmMemoryMapData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HbmSocket = GetHbmSocketData (MemMapData, Socket);

  if (IfFeatureEnabled (TypeSnc)) {
    //
    // HBM will not be enabled if GetNumOfClusterPerSystem value is not supported
    // no need to recheck it here
    //
    if (Cluster >= GetNumOfClusterPerSystem ()) {
      return EFI_INVALID_PARAMETER;
    }

    //
    // If there is no stack for this cluster, we're done.
    //
    if ((HbmSocket->HbmStackInfo.HbmStackValidBitMask & (BIT0 << HbmSocket->StackRouting[Cluster])) == 0) {
      return EFI_SUCCESS;
    }
  }

  if (HbmSocket->SizePerStack == 0) {
    return EFI_UNSUPPORTED;
  }

  PrevLimit = GetSadBase10nmHl (MemMapData, Socket, SadIndex) ;

  if (IfFeatureEnabled (TypeSnc)) {
    TargetWays = ONE_WAY;
    TargetBitmap = BIT0 << HbmSocket->StackRouting[Cluster];
  } else {
    TargetWays = HbmSocket->StackCount;
    TargetBitmap = HbmSocket->HbmStackInfo.HbmStackValidBitMask;
  }

  Limit = PrevLimit + (TargetWays * HbmSocket->SizePerStack);

  Status = AddSadEntries10nmHl (
             MemMapData,
             Socket,
             MemType1lmHbm,
             TargetWays,
             MAX_HBM_IO_CH,
             TargetBitmap,
             0,
             mChannelInterBitmap,
             NULL,
             SadIndex,
             &Limit,
             Cluster
             );

  return Status;
}

/**

  This routine returns the logical ID for a given Physical MC and Physical Channel ID in a HBM Io Stack.

  @param[in] McIndex  Physical Stack Index to use for Logical Channel ID lookup.
  @param[in] McIndex  Physical MC Index to use for Logical Channel ID lookup.
  @param[in] ChIndex  Physical Channel Index within the McIndex to use for the
                      logical Channel ID lookup.

  @retval UINT8 Channel logical ID corresponding the physical MC and Channel ID.

**/
UINT8
GetHbmLogicalChannelId (
  IN UINT8           Stack,
  IN UINT8           McIndex,
  IN UINT8           ChIndex
  )
{
  UINT8 ChLid;

  McIndex += HBM_IO_EDC_START_TGT;

  for (ChLid = 0; ChLid < MAX_RT_WAYS; ChLid++) {
    if (mRouteTableTargetList[Stack][ChLid] == McIndex &&
      mRouteTableChannelList[ChLid] == ChIndex) {
      break;
    }
  }

  return ChLid;
}

/**

  This routine returns the encoded value for chan_cap.
  The value returned will be in a range indicating power of 2 increments
  from 256 MB base.

  @param[in] ChSize  HBM channel size to encode.

  @retval    Encoded value.

**/
UINT32
GetHbmNmCacheSize (
  IN UINT32           ChSize
  )
{
  return Log2x32 (ChSize >> CONVERT_64MB_TO_256MB_GRAN) + HBM_CHAN_CAP_RANGE_BASE;
}

/**

  This routine returns the logical ID for a HBM stack. The logical ID is always 0 where SNC-4 enabled.

  @param[in] MemMapData  - Pointer to the memory map private data structure
  @param[in] Sckt        - Socket Id
  @param[in] IoStack     - Index of the HBM IO stack.

  @retval  The target LID for HBM IO stack.

**/
UINT8
GetHbmLogicalTargetId (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN UINT8           IoStack
  )
{
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;
  UINT8                       Index;
  UINT8                       TargetLid;

  TargetLid = 0;
  HbmSocket = GetHbmSocketData (MemMapData, Sckt);

  if (IfFeatureEnabled (TypeSnc)) {
    TargetLid = 0;
  } else {
    for (Index = 0; Index < IoStack; Index++) {
      if ((HbmSocket->HbmStackInfo.HbmStackValidBitMask & (BIT0 << Index)) != 0) {
        TargetLid++;
      }
    }
  }

  return TargetLid;
}

/**

  Program HBM 2LM decoder TAD rule registers.

  @param[in] MemMapData  - Pointer to the memory map private data structure
  @param[in] Sckt        - Socket number
  @param[in] IoStack     - Index of the HBM IO stack.

  @retval None

**/
VOID
WriteHbm2lmDecoderRegisters (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN UINT8           IoStack
  )
{
  SYSHOST                                       *Host;
  HBM_MEMORY_MAP_SOCKET_CONFIG                  *HbmSocket;
  SOCKET_INFO_STRUCT                            *Socket;
  UINT8                                         TadIndex;
  SAD_TABLE                                     *SadEntry;
  UINT8                                         MaxImcHbm;
  UINT8                                         McIndex;
  UINT8                                         NumChPerMcHbm;
  UINT8                                         ChIndex;
  UINT8                                         ChannelInterleaveBitMap;
  BOOLEAN                                       TwoLmRegionProcessed;
  NM_2LM_DATA                                   NmData;
  FM_2LM_DATA                                   FmData;
  UINT8                                         TadTargetGranularity;
  UINT8                                         SadIndex;
  UINT8                                         ClusterId;
  UINT8                                         BeginSadIndex;
  UINT8                                         EndSadIndex;
  SYS_SETUP                                     *Setup;

  Host = GetSysHostPointer ();
  HbmSocket = GetHbmSocketData (MemMapData, Sckt);
  Socket = &Host->var.mem.socket[Sckt];
  MaxImcHbm = GetMaxImcPerIoHbm ();
  NumChPerMcHbm = GetNumChannelPerMcHbm ();
  Setup = GetSysSetupPointer ();
  TwoLmRegionProcessed = FALSE;

  ChannelInterleaveBitMap = 0;
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
      // Skip if SAD type is not 2LM.
      // Skip if IO stack is not interleaved as NM for this SAD.
      //
      if (!IsMemType2lmHbm (SadEntry->type) ||
          ((SadEntry->NmImcInterBitmap & (1 << IoStack)) == 0)) {
        continue;
      }

      GetTadRuleTargetGranularity (MemMapData, Sckt, SadIndex, &TadTargetGranularity);

      //
      // 2LMCNTL registers are Per Channel per MC. So we process one instance of 2LM interleave.
      //
      if (!TwoLmRegionProcessed) {
        ChannelInterleaveBitMap |= SadEntry->channelInterBitmap[IoStack];

        ZeroMem (&NmData, sizeof (NmData));

        MemMapData->MemMapSocket[Sckt].NmIntlvValues[IoStack].NmTargetWays = SadEntry->ways;

        NmData.NmWaysGran.EncTgtWays = TadEncodeSkWays (MemMapData->MemMapSocket[Sckt].NmIntlvValues[IoStack].NmTargetWays);

        NmData.NmTgtLid = GetHbmLogicalTargetId (MemMapData, Sckt, IoStack);

        MemMapData->MemMapSocket[Sckt].NmIntlvValues[IoStack].NmChannelWays  = MAX_HBM_IO_CH;
        NmData.NmWaysGran.EncChWays = ImcTadEncodeChWays (MemMapData->MemMapSocket[Sckt].NmIntlvValues[IoStack].NmChannelWays);
        NmData.NmWaysGran.EncTgtGran = EncodeTadTargetInterleaveGranularity (TadTargetGranularity);
        NmData.NmWaysGran.EncChGran  = EncodeChannelInterleaveGranularity (SadEntry->granularity);

        for (McIndex = 0; McIndex < MaxImcHbm; McIndex++) {
          ChannelInterleaveBitMap = SadEntry->channelInterBitmap[McIndex];
          if (ChannelInterleaveBitMap == 0) {
            continue;
          }

          RcDebugPrint (SDBG_MAX,
            "\n       Write 2LM NM Decoder CSR's for HBM Stack %d MC %d\n",
            IoStack,
            McIndex
            );

          for (ChIndex = 0; ChIndex < NumChPerMcHbm; ChIndex++) {
            if ((ChannelInterleaveBitMap & (BIT0 << ChIndex)) == 0) {
              continue;
            }

            //
            // Calculate logical ID corresponding to Physical ID
            //
            NmData.NmChLid = GetHbmLogicalChannelId (IoStack, McIndex, ChIndex);

            NmData.NmChSize = GetHbmNmCacheSize (HbmSocket->ChannelCapacity);

            NmData.NmCacheBase = SadEntry->Base;

            if (NmData.NmCacheBase != 0) {
              //
              // Whenever the NmCachingOffsetEn is enabled, force_lat must be set.
              //
              McSetForceLatency (
                MemTechHbm,
                Sckt,
                GET_HBM_MCID_SOCKET(IoStack, McIndex),
                ChIndex
                );
            }

            McSet2lmNm (
              MemTechHbm,
              Sckt,
              GET_HBM_MCID_SOCKET(IoStack, McIndex),
              ChIndex,
              &NmData
              );
          } // ChIndex
        } // McIndex
        TwoLmRegionProcessed = TRUE;
      }

      //
      // d. Program MC DRAM_RULE_CFG[7:0]
      //
      ZeroMem (&FmData, sizeof (FmData));

      FmData.TadLimit              = SadTadEncodeLimit (SadEntry->Limit);
      FmData.FmWaysGran.EncChWays  = ImcTadEncodeChWays (ONE_WAY);
      FmData.FmWaysGran.EncTgtWays = TadEncodeSkWays (SadEntry->ways);
      FmData.FmWaysGran.EncChGran  = EncodeChannelInterleaveGranularity (SadEntry->granularity);
      FmData.FmWaysGran.EncTgtGran = EncodeTadTargetInterleaveGranularity (TadTargetGranularity);
      FmData.FmTarget              = IoStack;
      FmData.FmChIntBitmap         = SadEntry->FMchannelInterBitmap[IoStack];

      for (McIndex = 0; McIndex < MaxImcHbm; McIndex++) {
        ChannelInterleaveBitMap = SadEntry->channelInterBitmap[McIndex];
        if (ChannelInterleaveBitMap == 0) {
          continue;
        }
        RcDebugPrint (SDBG_MAX,
          "\n       Write 2LM FM Decoder CSR's for HBM Stack %d MC %d TAD %d\n",
          IoStack,
          McIndex,
          TadIndex
          );
        for (ChIndex = 0; ChIndex < NumChPerMcHbm; ChIndex++) {
          if ((ChannelInterleaveBitMap & (BIT0 << ChIndex)) == 0) {
            continue;
          }

         McSet2lmFm (
           MemTechHbm,
           Sckt,
           GET_HBM_MCID_SOCKET(IoStack, McIndex),
           ChIndex,
           TadIndex,
           &FmData
           );
        } // ChIndex
      } // McIndex
      TadIndex++;
    }  // SAD loop
  } // ClusterId
}


/**

  Program 1LM decoder (TADCHNILVOFFSET) TAD rule registers.

  @param[in] MemMapData  - Pointer to the memory map private data structure
  @param[in] Sckt        - Socket number
  @param[in] IoStack     - Index of the HBM IO stack.

  @retval None

**/
VOID
WriteHbm1lmDecoderRegisters (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN UINT8           IoStack
  )
{
  SYSHOST                                       *Host;
  HBM_MEMORY_MAP_SOCKET_CONFIG                  *HbmSocket;
  SOCKET_INFO_STRUCT                            *Socket;
  UINT8                                         TadIndex;
  TAD_TABLE                                     *TadEntry;
  SAD_TABLE                                     *SadEntry;
  TAD_1LM_DATA                                  TadData;
  UINT8                                         ChannelInterleaveBitMap;
  UINT8                                         MaxImcHbm;
  UINT8                                         McIndex;
  UINT8                                         NumChPerMcHbm;
  UINT8                                         ChIndex;
  UINT8                                         SadIndex;
  UINT8                                         ClusterId;
  UINT8                                         BeginSadIndex;
  UINT8                                         EndSadIndex;
  BOOLEAN                                       NmCachingCfgUpdated;

  Host = GetSysHostPointer ();
  HbmSocket = GetHbmSocketData (MemMapData, Sckt);
  MaxImcHbm = GetMaxImcPerIoHbm ();
  NumChPerMcHbm = GetNumChannelPerMcHbm ();
  NmCachingCfgUpdated = FALSE;

  Socket = &Host->var.mem.socket[Sckt];

  for (TadIndex = 0; TadIndex < MAX_TAD_RULES_10NM; TadIndex++) {
    TadEntry = &HbmSocket->McHbm[IoStack].TadTable[TadIndex];
    ZeroMem (&TadData, sizeof (TadData));

    if (TadEntry->Enable == 0) {
      break;
    }

    SadEntry = &Socket->SAD[TadEntry->SADId];
    if (!IsMemTypeHbm (SadEntry->type)) {
      continue;
    }

    TadData.TadOffset = HbmSocket->McHbm[IoStack].TadOffset[TadIndex];

    TadData.TadWaysGran.EncChWays = ImcTadEncodeChWays (MAX_HBM_IO_CH);
    TadData.TadWaysGran.EncTgtWays = TadEncodeSkWays (SadEntry->ways);
    TadData.TadWaysGran.EncChGran = EncodeChannelInterleaveGranularity (SadEntry->granularity);
    TadData.TadWaysGran.EncTgtGran = EncodeTadTargetInterleaveGranularity (TadEntry->TargetGran);

    for (McIndex = 0; McIndex < MaxImcHbm; McIndex++) {
      ChannelInterleaveBitMap = SadEntry->channelInterBitmap[McIndex];
      if (ChannelInterleaveBitMap == 0) {
        continue;
      }

      RcDebugPrint (SDBG_MAX,
        "\n       Write 1LM Decoder CSR's for HBM Stack %d MC %d\n",
        IoStack,
        McIndex
        );

      for (ChIndex = 0; ChIndex < NumChPerMcHbm; ChIndex++) {
        if ((ChannelInterleaveBitMap & (BIT0 << ChIndex)) == 0) {
          continue;
        }
        McSet1lmTad (
          MemTechHbm,
          Sckt,
          GET_HBM_MCID_SOCKET(IoStack, McIndex),
          ChIndex,
          TadIndex,
          &TadData
          );
      } // ChIndex
    } // McIndex
  } // Tad Loop

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
      // Skip SADs that are not of MemType HBM 2LM
      //
      if (!IsMemType2lmHbm (SadEntry->type)) {
        continue;
      }

      //
      // Skip if SAD type is 2LM and IO stack is not interleaved as NM for this SAD.
      //
      if (SadEntry->NmImcInterBitmap && (BIT0 << IoStack) == 0) {
        continue;
      }

      if (!NmCachingCfgUpdated) {
        for (McIndex = 0; McIndex < MaxImcHbm; McIndex++) {
          if (SadEntry->channelInterBitmap[McIndex] == 0) {
            continue;
          }

          RcDebugPrint (SDBG_MAX,
            "\n       Write MCNMCACHINGCFG for HBM Stack %d MC %d\n",
            IoStack,
            McIndex
            );

          //
          // HBM 2LM so set
          //
          McSetNmCachingCfg (
            MemTechHbm,
            Sckt,
            GET_HBM_MCID_SOCKET(IoStack, McIndex),
            TRUE
            );
        } // McIndex
        NmCachingCfgUpdated = TRUE;
      }
    }  // SAD loop
  } // ClusterId

  return;
}

/**
  This routine programs 2LM NM TAD for patrol and sparing address generation.

  @param[in]  Host        Pointer to the sysHost structure.
  @param[in]  MemMapData  Pointer to memory map private data structure.
  @param[in]  SocketId    Socket number.
  @param[in]  IoStack     Memory controller index.

**/
VOID
WriteHbmNmCacheTadRegister (
  IN SYSHOST         *Host,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           SocketId,
  IN UINT8           IoStack
  )
{
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;
  TAD_TABLE                     *TadEntry;
  UINT8                         TadIndex;
  UINT8                         SadIndex;
  UINT8                         McIndex;
  UINT8                         MaxImcHbm;

  if (!IsMemTypePresentMcFunction (SocketId, IoStack, IsMemType2lmHbm)) {
    return;
  }

  HbmSocket = GetHbmSocketData (MemMapData, SocketId);
  MaxImcHbm = GetMaxImcPerIoHbm ();

  for (TadIndex = 0; TadIndex < MAX_TAD_RULES_10NM; TadIndex++) {
    TadEntry = &HbmSocket->McHbm[IoStack].TadTable[TadIndex];
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
      for (McIndex = 0; McIndex < MaxImcHbm; McIndex++) {
          RcDebugPrint (SDBG_MAX,
            "\n        Write Hbm Nm Caching Tad CSR's for HBM Stack %d MC %d\n",
            IoStack,
            McIndex
            );

          McSetNmCachingTad (MemTechHbm, SocketId, GET_HBM_MCID_SOCKET(IoStack, McIndex), TadIndex);
        }

      break;
    }
  }
}

/**
  This routine programs following near memory dram rule registers:
  MC_DEC_CRNODE_CH{#CH}_MC{#MC}_CR_NM_DRAM_RULE_[7:0]
  NM_DRAM_RULE is programmed only for SA based patrol and is not programmed for CA based patrol.

  @param[in]  MemMapData  Pointer to memory map private data structure.
  @param[in]  Socket      Socket number.
  @param[in]  IoStack     Index of the HBM IO stack.

  @retval VOID
**/
VOID
WriteHbmNmDramRuleRegisters (
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             Socket,
  IN UINT8             IoStack
  )
{
  SAD_TABLE                   *SadEntry;
  UINT8                       SadIndex;
  UINT8                       ClusterId;
  UINT8                       BeginSadIndex;
  UINT8                       EndSadIndex;
  SYSHOST                     *Host;
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;
  UINT8                       NmDramIndex;
  PS_RULE_DATA                RuleData;
  UINT8                       MaxImcHbm;
  UINT8                       McIndex;
  UINT8                       NumChPerIoHbm;
  UINT8                       NumChPerMcHbm;
  UINT8                       ChIndex;
  UINT8                       ChannelInterleaveBitMap;
  UINT8                       ChLid;

  NmDramIndex = 0;
  Host = GetSysHostPointer ();
  MaxImcHbm = GetMaxImcPerIoHbm ();
  NumChPerMcHbm = GetNumChannelPerMcHbm ();
  NumChPerIoHbm = GetNumChannelPerIoHbm ();

  HbmSocket = GetHbmSocketData (MemMapData, Socket);

  for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
    BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
    EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);

    for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
      SadEntry = &Host->var.mem.socket[Socket].SAD[SadIndex];
      if (SadEntry->Enable == 0) {
        break;
      }

      if (SadEntry->type != MemType1lmHbm) {
        continue;
      }

      if (!(SadEntry->imcInterBitmap & (BIT0 << IoStack))) {
        //
        // this SAD doesn't pertain to this IoStack
        //
        continue;
      }

      ZeroMem (&RuleData, sizeof (RuleData));

      RuleData.PsLimit = SadTadEncodeLimit (SadEntry->Limit);

      RuleData.WaysGran.EncChWays = ImcTadEncodeChWays (MAX_HBM_IO_CH);
      RuleData.WaysGran.EncChGran  = EncodeChannelInterleaveGranularity (SadEntry->granularity);
      RuleData.WaysGran.EncTgtGran = EncodeTadTargetInterleaveGranularity (SadEntry->tgtGranularity);
      RuleData.WaysGran.EncTgtWays = TadEncodeSkWays (SadEntry->ways);

      for (McIndex = 0; McIndex < MaxImcHbm; McIndex++) {
        ChannelInterleaveBitMap = SadEntry->channelInterBitmap[McIndex];
        if (ChannelInterleaveBitMap == 0) {
          continue;
        }

        RcDebugPrint (SDBG_MAX,
          "\n        Write Patrol and Sparing NM DRAM RULE CSR's for HBM Stack %d MC %d\n",
          IoStack,
          McIndex
          );

        //
        // Init. channel ID transalation table to invalid data
        //
        for (ChLid = 0; ChLid < NumChPerIoHbm; ChLid++) {
          RuleData.EncChL2pid |= 0x7 << (ChLid * 3);
        }
        for (ChIndex = 0; ChIndex < NumChPerMcHbm; ChIndex++) {
          if ((ChannelInterleaveBitMap & (BIT0 << ChIndex)) == 0) {
            continue;
          }

          ChLid = GetHbmLogicalChannelId (IoStack, McIndex, ChIndex);
          if (ChLid == MAX_RT_WAYS) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
             "HBM: Invalid Channel logical ID found for IoStack %d\n", IoStack);
            ASSERT (FALSE);
          }

          //
          // Clear only bits corresponding to this ChLid
          //
          RuleData.EncChL2pid &= ~(0x7 << (ChLid * 3));
          RuleData.EncChL2pid |= ChIndex << (ChLid * 3);
        }
        for (ChIndex = 0; ChIndex < NumChPerMcHbm; ChIndex++) {
          if ((ChannelInterleaveBitMap & (BIT0 << ChIndex)) == 0) {
            continue;
          }

          McSetPatrolSparingRule (
            MemTechHbm,
            Socket,
            GET_HBM_MCID_SOCKET(IoStack, McIndex),
            ChIndex,
            NmDramIndex,
            &RuleData
            );
        }
        RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          " HBM MC%d NM_DRAM_RULE[%d]:\n"
          "  rule_enable: %d  limit: 0x%x  nm_chn_ways: %d  nm_chn_gran: %d  "
          "nm_target_ways: %d  nm_gran_eq: %d  nm_chn_l2pid: 0x%08x\n",
          McIndex,
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

        for (ChIndex = 0; ChIndex < MAX_RT_WAYS; ChIndex++) {
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          " %d ",
          (RuleData.EncChL2pid >> (ChIndex * 3)) & 0x7
          );
        }
        RcDebugPrint (SDBG_MAX, "\n");
      }
      ++NmDramIndex;
    }
  }
}

/**
  Get HBM patrol sparing limit by parsing all TAD tables correspond to the SAD index.
  Zero is returned if no such TAD table is found.

  @param[in]  MemMapData  Pointer to memory map private data structure.
  @param[in]  SocketId    Socket number.
  @param[in]  IoStack     Index of the HBM IO stack.
  @param[in]  SadIndex    Sad table index.

  @return The patrol sparing limit or 0 if no TAD is found.

**/
UINT32
GetHbmPsLimit (
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             Socket,
  IN UINT8             IoStack,
  IN UINT8             SadIndex
  )
{
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;
  TAD_TABLE                   *TadEntry;
  UINT32                      PsLimit;
  UINT8                       TadIndex;

  HbmSocket = GetHbmSocketData (MemMapData, Socket);

  PsLimit = 0;
  for (TadIndex = 0; TadIndex < MAX_TAD_RULES_10NM; TadIndex++) {
    TadEntry = &HbmSocket->McHbm[IoStack].TadTable[TadIndex];
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
  This routine programs the follwoing patrol sparing range registers:
  MC_DEC_CRNODE_CH{#CH}_MC{#MC}_CR_TADBASE_[7:0]
  and MC_DEC_CRNODE_CH{#CH}_MC{#MC}_CR_TADWAYNESS_[7:0]

  @param[in]  MemMapData  Pointer to memory map private data structure.
  @param[in]  SocketId    Socket number.
  @param[in]  IoStack     Index of the HBM IO stack.

**/
VOID
WriteHbmPatrolSparingRangeRegisters (
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             Socket,
  IN UINT8             IoStack
  )
{
  UINT32                      PsLimit;
  UINT8                       SadIndex;
  UINT8                       ClusterId;
  UINT8                       BeginSadIndex;
  UINT8                       EndSadIndex;
  SYSHOST                     *Host;
  SAD_TABLE                   *SadEntry;
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;
  PS_RANGE_DATA               RangeData;
  UINT8                       ChannelInterleaveBitMap;
  UINT8                       MaxImcHbm;
  UINT8                       McIndex;
  UINT8                       NumChPerMcHbm;
  UINT8                       ChIndex;
  UINT8                       PsRangeRegIndex;
  BOOLEAN                     TwoLmRegionProcessed;

  Host = GetSysHostPointer ();
  PsRangeRegIndex = 0;

  HbmSocket = GetHbmSocketData (MemMapData, Socket);
  MaxImcHbm = GetMaxImcPerIoHbm ();
  NumChPerMcHbm = GetNumChannelPerMcHbm ();
  TwoLmRegionProcessed = FALSE;

  for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
    BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
    EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);

    for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
      SadEntry = &Host->var.mem.socket[Socket].SAD[SadIndex];
      if (SadEntry->Enable == 0) {
        break;
      }

      if (SadEntry->type == MemType1lmHbm) {
        if (((SadEntry->imcInterBitmap & (BIT0 << IoStack)) == 0)) {
          continue;
        }
        PsLimit = GetHbmPsLimit (MemMapData, Socket, IoStack, SadIndex);
        if (PsLimit == 0) {
          continue;
        }
        RangeData.PsBase = SadEntry->Base;
      } else if (IsMemType2lmHbm (SadEntry->type)) {
        if (TwoLmRegionProcessed || (SadEntry->NmImcInterBitmap & (BIT0 << IoStack)) == 0) {
          continue;
        }
        GetNmPsRange (Host, MemMapData, Socket, IoStack, SadIndex, &RangeData.PsBase, &PsLimit);
        TwoLmRegionProcessed = TRUE;
      } else {
        continue;
      }

      RangeData.PsLimit = SadTadEncodeLimit (PsLimit);

      //
      // The Logical Target ID depends on how the CHA interleaves the various MCs,
      // and it can be different for each dram memory rule (SAD Rule).
      //
      RangeData.TgtLid = GetHbmLogicalTargetId (MemMapData, Socket, IoStack);
      ChannelInterleaveBitMap = SadEntry->channelInterBitmap[0];
      for (McIndex = 0; McIndex < MaxImcHbm; McIndex++) {
        ChannelInterleaveBitMap = SadEntry->channelInterBitmap[McIndex];
        if (ChannelInterleaveBitMap == 0) {
          continue;
        }
        RcDebugPrint (SDBG_MAX,
          "\n        Write Patrol and Sparing TAD range CSR's for HBM Stack %d MC %d\n",
          IoStack,
          McIndex
          );
        for (ChIndex = 0; ChIndex < NumChPerMcHbm; ChIndex++) {
          if ((ChannelInterleaveBitMap & (BIT0 << ChIndex)) == 0) {
            continue;
          }

          RangeData.ChLid = GetHbmLogicalChannelId (IoStack, McIndex, ChIndex);
          if (RangeData.ChLid == MAX_RT_WAYS) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
             "HBM: Invalid Channel logical ID found for IoStack %d\n", IoStack);
            ASSERT (FALSE);
          }

          RangeData.PsAttr = 0;

          McSetPatrolSparingRange (
            MemTechHbm,
            Socket,
            GET_HBM_MCID_SOCKET(IoStack, McIndex),
            ChIndex,
            PsRangeRegIndex,
            &RangeData
            );
        } // ChIndex
      } // McIndex
      PsRangeRegIndex++;
    } // SadIndex
  } // ClusterId
}

/**

  Program HBM patrol and sparing registers (TADBASE, TADWAYNESS, MCNMCACHINGCFG and
  NM_DRAM_RULE).

  @param[in] MemMapData      - Pointer to the memory map private data structure
  @param[in] Sckt            - Socket number
  @param[in] IoStack         - Index of the HBM IO stack.

  @retval VOID

**/
VOID
WriteHbmPatrolSparingRegisters (
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           Sckt,
  IN UINT8           IoStack
  )
{
  SYSHOST                     *Host;

  Host = GetSysHostPointer ();

  WriteHbmPatrolSparingRangeRegisters (MemMapData, Sckt, IoStack);

  WriteHbmNmDramRuleRegisters (MemMapData, Sckt, IoStack);

  WriteHbmNmCacheTadRegister (Host, MemMapData, Sckt, IoStack);

  return;
}

/**
  Write TAD CSRs for HBM.

  For each HBM IO stack, write the TAD CSRs to its MCs.

  @param[in] Socket     Current Socket (0-Based)
  @param[in] MemMapData Pointer to the memory map private data structure

**/
VOID
WriteHbmTadCsrs (
  IN UINT8            Socket,
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;
  UINT8                       IoStack;
  UINT8                       MaxIoHbm;

  MaxIoHbm = GetMaxIoHbm ();

  if (MemMapData->HbmMemoryMapData == NULL) {
    return;
  }

  HbmSocket = GetHbmSocketData (MemMapData, Socket);

  if ((HbmSocket->StackCount == 0) || (HbmSocket->SizePerStack == 0)){
    return;
  }

  for (IoStack = 0; IoStack < MaxIoHbm; IoStack++) {
    if ((HbmSocket->HbmStackInfo.HbmStackValidBitMask & (1 << IoStack)) == 0) {
      continue;
    }

    //
    // 1. Program registers for patrol and sparing
    //
    WriteHbmPatrolSparingRegisters (
      MemMapData,
      Socket,
      IoStack
      );

    //
    // 2. Program 1LM decoder registers
    //
    WriteHbm1lmDecoderRegisters (
      MemMapData,
      Socket,
      IoStack
      );

    //
    // 3. Program 2LM decoder registers
    //
    WriteHbm2lmDecoderRegisters (
      MemMapData,
      Socket,
      IoStack
      );
  } // IoStack

  return;
}

/**

  This function writes M2M and MC CSRs to enable or disable the early completion.

  @param[in] SocketId          Socket index.

**/
VOID
HbmWriteEarlyCmpCsrs (
  IN UINT8   SocketId
  )
{
  UINT8      HbmIoId;
  UINT8      HbmMaxIo;
  UINT8      HbmMcId;
  UINT8      HbmMaxImcPerIo;
  UINT8      HbmChId;
  UINT8      HbmMaxChPerMc;
  UINT8      HbmChIdPerSocket;
  UINT8      EarlyCmpEn;
  SYS_SETUP  *Setup;

  EarlyCmpEn = 0x0;
  Setup = GetSysSetupPointer ();
  HbmMaxIo = GetMaxIoHbm ();
  HbmMaxImcPerIo = GetMaxImcPerIoHbm ();
  HbmMaxChPerMc = GetNumChannelPerMcHbm ();

  //
  // Early completion is only supported on 1S system.
  //
  if (CheckSysConfig (SYS_CONFIG_1S) && (Setup->mem.dfxMemSetup.DfxHbmEarlyCmpEn == HBM_EARLY_CMP_ENABLED)) {
    EarlyCmpEn = 0x1;
  }

  for (HbmIoId = 0; HbmIoId < HbmMaxIo; HbmIoId++) {
    if (!IsHbmMemSsEnabled (SocketId, HbmIoId)) {
      continue;
    }

    for (HbmMcId = 0; HbmMcId < HbmMaxImcPerIo; HbmMcId++) {
      //
      // Enable or disable early completion in Mesh2Mem.
      //
      GetSetM2mAkEarlyCompletionsEnabled (SocketId, HBM_M2M_INSTANCE_ID (HbmIoId, HbmMcId), GSM_FORCE_WRITE, &EarlyCmpEn);

      //
      // SysFeatures0.AkEarlyCmpEn relies on D2C to occur so DisableDir4AppDirect and AllowDirUpdToBlock
      // need to be set to 0x0 and 0x1 respectively to prevent D2C from getting squashed.
      //
      if (EarlyCmpEn != 0x0) {
        SetM2mDisableDir4AppDirect (SocketId, HBM_M2M_INSTANCE_ID (HbmIoId, HbmMcId), 0x0);
        SetM2mAllowDirUpdToBlock (SocketId, HBM_M2M_INSTANCE_ID (HbmIoId, HbmMcId), 0x1);
      }

      for (HbmChId = 0; HbmChId < HbmMaxChPerMc; HbmChId++) {
        HbmChIdPerSocket = HbmChId + (HbmMcId * HbmMaxChPerMc) + (HbmIoId * HbmMaxChPerMc * HbmMaxImcPerIo);
        if (!IsHbmChannelEnabled (SocketId, HbmChIdPerSocket)) {
          continue;
        }
        //
        // Enable or disable early completion in MC.
        //
        McSetEnEarlyCmp (MemTechHbm, SocketId, HbmChIdPerSocket, EarlyCmpEn);

        //
        // MC must be set consistently with Mesh2Mem for DisableDir4AppDirect and AllowDirUpdToBlock.
        //
        if (EarlyCmpEn != 0x0) {
          McDisableDirForAppDirect (MemTechHbm, SocketId, HbmChIdPerSocket, 0x0);
          McSetAllowDirUpdToBlock (MemTechHbm, SocketId, HbmChIdPerSocket, 0x1);
        }
      }
    }
  }
}

/**
  Find the next HBM TAD index.

  Search an HBM IO stack MC instance for the next unused TAD index.

  This is a private helper function, and should not be called externally.

  @param[in] Socket     Current Socket (0-Based)
  @param[in] MemMapData Pointer to the memory map private data structure.
  @param[in] IoStack    Index of the HBM IO stack.

  @return     Index of the next available TAD entry.
**/
UINT8
FindHbmTadIndex (
  IN UINT8           Socket,
  IN MEMORY_MAP_DATA *MemMapData,
  IN UINT8           IoStack
  )
{
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;
  HBM_MEMORY_MAP_MC_DATA      *McHbm;
  UINT8                       TadIndex;

  HbmSocket = GetHbmSocketData (MemMapData, Socket);

  //
  // Find the first not-yet-enabled TAD for this stack.
  //
  McHbm = &HbmSocket->McHbm[IoStack];
  for (TadIndex = 0; TadIndex < MAX_TAD_RULES_10NM; TadIndex++) {
    if (McHbm->TadTable[TadIndex].Enable == 0) {
      break;
    }
  }

  return TadIndex;
}

/**
  Initialize the McHbm structure.

  Zero out the structure and initialize fields

  @param[in] HbmSocket   Pointer to the HBM socket data structure.

**/
VOID
InitializeMcHbm (
  IN HBM_MEMORY_MAP_SOCKET_CONFIG *HbmSocket
  )
{
  UINT8 Stack;

  ZeroMem (HbmSocket->McHbm, (sizeof (HBM_MEMORY_MAP_MC_DATA) * MAX_HBM_IO));
  for (Stack = 0; Stack < MAX_HBM_IO; ++Stack) {
    HbmSocket->McHbm[Stack].RemainingChannelCapacity = HbmSocket->ChannelCapacity;
  }
}

/**
  Create TAD rules for HBM.

  Iterate through the existing SAD rules. For each one that includes HBM,
  create TAD rules in the HBM MCs.

  @param[in] MemMapData   Pointer to the memory map private data structure.

  @retval  EFI_SUCCESS             Any HBM TAD rules were successfully created.
  @retval  EFI_INVALID_PARAMETER   An invalid parameter was passed in.
  @retval  EFI_OUT_OF_RESOURCES    Max TAD rules was exceeded.
**/
EFI_STATUS
EFIAPI
CreateHbmTadRules (
  IN MEMORY_MAP_DATA *MemMapData
  )
{
  EFI_STATUS                  Status;
  SYSHOST                     *Host;
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;
  UINT32                      ChannelMemUsed;
  UINT8                       TargetWays;
  UINT8                       ChannelWays;
  UINT8                       ClusterId;
  UINT8                       BeginSadIndex;
  UINT8                       EndSadIndex;
  UINT8                       SadIndex;
  SAD_TABLE                   *SadEntry;
  TAD_TABLE                   *TadEntry;
  UINT32                      TadLimit;
  UINT8                       Socket;
  UINT8                       TadIndex;
  UINT8                       MaxIoHbm;
  UINT8                       StartIoStack;
  UINT8                       EndIoStack;
  UINT8                       IoStack;
  UINT16                      Tolm;
  UINT8                       IoStackBitmap2LmProcessed;

  Host = GetSysHostPointer ();
  Tolm = GetTolmAddress ();
  Status = EFI_SUCCESS;
  MaxIoHbm = GetMaxIoHbm ();

  if (MemMapData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (MemMapData->HbmMemoryMapData == NULL) {
    return EFI_SUCCESS;
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "HBM: CreateHbmTadRules\n"
    );

  ChannelWays = GetNumChannelPerIoHbm ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    if (CheckSocketEnabled (Socket) == 0) {
      continue;
    }

    HbmSocket = GetHbmSocketData (MemMapData, Socket);

    if ((HbmSocket->StackCount == 0) || (HbmSocket->SizePerStack == 0)){
      return Status;
    }

    IoStackBitmap2LmProcessed = 0;

    InitializeMcHbm (HbmSocket);

    //
    // For SNC, interleaving is within the cluster.
    //
    if (IfFeatureEnabled (TypeSnc)) {
      TargetWays = ONE_WAY;
    } else { // For Quad, the target ways is total number of stacks
      TargetWays = HbmSocket->StackCount;
    }

    for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
      BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
      EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
      if (Host->var.mem.socket[Socket].SAD[BeginSadIndex].Enable == 0) {
        break;
      }
      for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
        SadEntry = &Host->var.mem.socket[Socket].SAD[SadIndex];
        if (SadEntry->Enable == 0) {
          break;
        }

        if (SadEntry->local == 0) {
          continue;
        }

        //
        // Skip entries without HBM.
        //
        if (!IsMemTypeHbm (SadEntry->type)) {
          continue;
        }

        if (IfFeatureEnabled (TypeSnc)) {
          StartIoStack = HbmSocket->StackRouting[SadEntry->Cluster];
          EndIoStack = StartIoStack + 1;
          if ((HbmSocket->HbmStackInfo.HbmStackValidBitMask & (1 << StartIoStack)) == 0) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "HBM: CreateHbmTadRules. Invalid stack found in Cluster = %d SadIndex = %d\n",
              ClusterId,
              SadIndex
              );
            return EFI_INVALID_PARAMETER;
          }
        } else { // For Quad, the target ways is total number of clusters
          StartIoStack = 0;
          EndIoStack = MaxIoHbm;
        }

        //
        // Create 1lM HBM TAD or 2LM NM HBM TAD
        //
        if (SadEntry->type == MemType1lmHbm) {
          TadLimit = SadEntry->Limit;
          if (SadEntry->Base < Tolm && SadEntry->Limit >= Tolm) {
            TadLimit -= GetLowGap ();
          }
        } else {
          TadLimit = 0;
        }

        for (IoStack = StartIoStack; IoStack < EndIoStack; IoStack++) {
          if ((HbmSocket->HbmStackInfo.HbmStackValidBitMask & (1 << IoStack)) == 0) {
            continue;
          }
          if ((IoStackBitmap2LmProcessed & (1 << IoStack)) != 0) {
              continue;  // The 2LM region has been processed for this stack.
            }
          TadIndex = FindHbmTadIndex (Socket, MemMapData, IoStack);
          if (TadIndex == MAX_TAD_RULES_10NM) {
            RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "HBM: CreateHbmTadRules. TAD rules exeeded\n");
            return EFI_OUT_OF_RESOURCES;
          }

          TadEntry = &HbmSocket->McHbm[IoStack].TadTable[TadIndex];

          TadEntry->Enable = 1;
          TadEntry->SADId = SadIndex;
          TadEntry->Limit = TadLimit;
          TadEntry->socketWays = TargetWays;
          Status = GetTadRuleTargetGranularity (
            MemMapData,
            Socket,
            SadIndex,
            &TadEntry->TargetGran
          );

          if (Status != SUCCESS) {
            return EFI_NOT_FOUND;
          } else {
            Status = EFI_SUCCESS;
          }
          TadEntry->ChGran = SadEntry->granularity;

          //
          // CA TAD offset is required only for 1LM HBM
          //
          if (SadEntry->type == MemType1lmHbm) {
            //
            // first calculate what will remain of each channel after this TAD rule
            // then subtract that from the channel's capacity to calculate how much of the channel is used up
            //
            HbmSocket->McHbm[IoStack].RemainingChannelCapacity -= (TadLimit - SadEntry->Base) / (TargetWays * ChannelWays);
            ChannelMemUsed = HbmSocket->ChannelCapacity - HbmSocket->McHbm[IoStack].RemainingChannelCapacity;

            HbmSocket->McHbm[IoStack].TadOffset[TadIndex] = CalculateTadOffset (
              FALSE,
              TadEntry->socketWays,
              ChannelWays,
              ChannelMemUsed,
              TadLimit
            );
          } else {
            IoStackBitmap2LmProcessed |= 1 << IoStack;
          }

        } // IoStack
      } // SadIndex
    } // Cluster
  } // Socket

  return Status;
}

/**
  Display TAD rule tables for HBM.

  Iterate through the existing TAD rules for each HBM IO stack and display
  their data.

  @param[in] Socket       Current Socket (0-Based)
  @param[in] MemMapData   Pointer to the memory map private data structure.

**/
VOID
DisplayHbmTadTables (
  IN  UINT8           Socket,
  IN MEMORY_MAP_DATA  *MemMapData
  )
{
  HBM_MEMORY_MAP_SOCKET_CONFIG  *HbmSocket;
  HBM_MEMORY_MAP_MC_DATA      *McHbm;
  UINT8                       IoStack;
  UINT8                       TadIndex;
  UINT8                       MaxIoHbm;

  if (Socket >= MAX_SOCKET) {
    return;
  }

  if (MemMapData->HbmMemoryMapData == NULL) {
    return;
  }

  HbmSocket = GetHbmSocketData (MemMapData, Socket);

  if ((HbmSocket->StackCount == 0) || (HbmSocket->SizePerStack == 0)){
    return;
  }

  MaxIoHbm = GetMaxIoHbm ();

  //
  // Loop for each HBM IO stack.
  //
  for (IoStack = 0; IoStack < MaxIoHbm; IoStack++) {
    if ((HbmSocket->HbmStackInfo.HbmStackValidBitMask & (1 << IoStack)) == 0) {
      continue;
    }

    McHbm = &HbmSocket->McHbm[IoStack];

    //
    // If the first TAD is disabled, then all of them are. Skip printing for this stack.
    //
    if (McHbm->TadTable[0].Enable == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\n\n*******TAD Table for Socket %d Stack %d*******\n"
      "Enable  SadID  Limit    SktWays  TargetGran  ChGran\n",
      Socket,
      IoStack
      );

    for (TadIndex = 0; TadIndex < MAX_TAD_RULES_10NM; TadIndex++) {
      if (McHbm->TadTable[TadIndex].Enable == 0) {
        break;
      }

      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "%6d  %5d  0x%05x  %7d  %10d  %6d\n",
        McHbm->TadTable[TadIndex].Enable,
        McHbm->TadTable[TadIndex].SADId,
        McHbm->TadTable[TadIndex].Limit,
        McHbm->TadTable[TadIndex].socketWays,
        McHbm->TadTable[TadIndex].TargetGran,
        McHbm->TadTable[TadIndex].ChGran
        );
    }
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\n</TADTable>\n");
  }
}
