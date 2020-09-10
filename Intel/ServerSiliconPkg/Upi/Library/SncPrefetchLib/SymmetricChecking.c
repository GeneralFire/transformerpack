/** @file
  Checking Memory Symmetric.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Library/OemProcMemInitLib.h>
#include <Uefi.h>
#include <Library/PeiServicesLib.h>

#include <KtiMisc.h>
#include <KtiSetupDefinitions.h>
#include <Upi/KtiSi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemRcLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/IoAccessLib.h>
#include <ScratchpadList.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/KtiSimLib.h>
#include <Library/IoLib.h>
#include <RcRegs.h>
#include <Library/ChaLib.h>
#include <Guid/UboxIpInterface.h>
#include <Library/Ms2IdiLib.h>
#include <Library/OobIpLib.h>
#include <Library/PcuIpLib.h>
#include <Library/M2IalIpLib.h>
#include <Guid/KtiIpInterface.h>
#include <Guid/IioIpInterface.h>
#include <Guid/MsmIpInterface.h>
#include <Library/CsrAccessLib.h>
#include <Library/PmcSetStrapMessageLib.h>
#include <Library/MemTypeLib.h>
#include <Library/CxlLib.h>
#include <Guid/SocketCommonRcVariable.h>
#include <Library/SncPrefetchLib.h>
#include "SncPrefetchLibInternal.h"


/**
  This routine scans through all populated channels within the MC, and
  returns TRUE if all channels are uniform/symmetric. Else, it returns FALSE.
  Caller provides memory controller ID and the maximum number of channels
  supported by the said memory controller.

  @param[in]  Socket            Socket Node ID
  @param[in]  McId              Memory Controller ID to scan
  @param[in]  MaxChPerMc        Maximum number of channels supported by this memory controller
  @param[out] CommonChMemSize   Optional parameter. If it is not NULL, it will return the channel
                                memory size if all populated channels on this MC is symmetric.
  @param[out] TotalMemSizeOnMc  Optional parameter. If it is not NULL, it will return total memory
                                size on this MC if all populated channels on this MC is symmetric.

  @retval TRUE    All populated channels within MC are symmetric.
  @retval FALSE   Populated channels within MC are not symmetric.

**/
BOOLEAN
ChannelsWithinMcSymmetric (
  IN  UINT8                Socket,
  IN  UINT8                McId,
  IN  UINT8                MaxChPerMc,
  OUT UINT32               *CommonChMemSize,  OPTIONAL
  OUT UINT32               *TotalMemSizeOnMc  OPTIONAL
  )
{
  BOOLEAN               IsFirstMemSizeUpdated;
  UINT32                FirstMemSize;
  UINT32                ChMemSize;
  UINT8                 Ch;
  UINT8                 ChId;
  UINT32                TotalMemSize;

  ChMemSize = 0;
  TotalMemSize = 0;
  IsFirstMemSizeUpdated = FALSE;

  for (Ch = 0; Ch < MaxChPerMc; Ch++) {
    ChId = Ch + (McId * MaxChPerMc);

    ChMemSize = GetChannelMemSize (Socket, ChId);
    KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "S%d Channel[%d]-Size=0x%x\n", Socket, ChId, ChMemSize));

    if (ChMemSize == 0) {
      continue;
    }

    TotalMemSize += ChMemSize;

    if (!IsFirstMemSizeUpdated) {
      IsFirstMemSizeUpdated = TRUE;
      FirstMemSize = ChMemSize;
      continue;
    }

    if (FirstMemSize != ChMemSize) {
      KtiDebugPrintInfo0 ((
        KTI_DEBUG_INFO0,
        "\nS%d: MC%d channels not symmetric\n",
        Socket,
        McId
        ));
      return FALSE;
    }
  }

  if (CommonChMemSize != NULL) {
    *CommonChMemSize = ChMemSize;
  }

  if (TotalMemSizeOnMc != NULL) {
    *TotalMemSizeOnMc = TotalMemSize;
  }

  return TRUE;
}

/**
  This routine scans through all populated channels within the cluster, and
  returns TRUE if all channels are uniform/symmetric. Else, it returns FALSE.
  Caller provides first and last memory controller IDs belong to the targeted
  cluster, and maximum number of channels supported by the memory controller.

  @param[in] Socket      Socket Node ID
  @param[in] FirstMcId   First Memory Controller ID of the targeted cluster to scan
  @param[in] LastMcId    Last Memory Controller ID of the targeted cluster to scan
  @param[in] MaxChPerMc  Maximum number of channels supported by the memory controller

  @retval TRUE    All populated channels within the targeted cluster are symmetric.
  @retval FALSE   Populated channels within the targeted cluster are not symmetric.

**/
BOOLEAN
ChannelsWithinClusterSymmetric (
  IN UINT8       Socket,
  IN UINT8       FirstMcId,
  IN UINT8       LastMcId,
  IN UINT8       MaxChPerMc
  )
{
  BOOLEAN               IsFirstMemSizeUpdated;
  UINT32                FirstMemSize;
  UINT32                CommonChMemSize;
  UINT8                 McId;
  UINT32                TotalMemSizePerMc[MAX_IMC] = {0};
  BOOLEAN               ChSymmetric;
  UINT8                 MaxImc;

  ChSymmetric = FALSE;
  IsFirstMemSizeUpdated = FALSE;
  CommonChMemSize = 0;
  MaxImc = GetMaxImc();
  //
  // Scan all populated channels on all MCs belong to the targeted cluster.
  // All populated channels should have the same memory size for uniform memory population.
  //
  for (McId = FirstMcId; (McId <= LastMcId) && (McId < MaxImc); McId++) {
    if (!CheckSocketMcEnabled (Socket, McId)) {
      continue;
    }

    TotalMemSizePerMc[McId] = 0;
    ChSymmetric = ChannelsWithinMcSymmetric(Socket, McId, MaxChPerMc, &CommonChMemSize, &TotalMemSizePerMc[McId]);

    if (!ChSymmetric) {
      return FALSE;
    }

    if (!IsFirstMemSizeUpdated) {
      FirstMemSize = CommonChMemSize;
      IsFirstMemSizeUpdated = TRUE;
      continue;
    }

    if (FirstMemSize != CommonChMemSize) {
      return FALSE;
    }
  }

  //
  // Verify all memory controllers populated with memory are uniform by checking
  // their total memory size.
  //
  IsFirstMemSizeUpdated = FALSE;
  for (McId = FirstMcId; (McId <= LastMcId) && (McId < MaxImc); McId++) {
    if (TotalMemSizePerMc[McId] == 0) {
      continue;
    }

    if (!IsFirstMemSizeUpdated) {
      FirstMemSize = TotalMemSizePerMc[McId];
      IsFirstMemSizeUpdated = TRUE;
      continue;
    }

    if (TotalMemSizePerMc[McId] != FirstMemSize) {
      KtiDebugPrintInfo0 ((
        KTI_DEBUG_INFO0,
        "S%d Last scanned Mc%d - total memory size mismatched found!\n",
        Socket,
        McId
        ));
      return FALSE;
    }
  }
  return TRUE;
}

/**
  Check if an MC is a part of a 3-way channel interleave across 2 MCs

  @param[in] Socket             Socket Node ID
  @param[in] FirstMcId          First Memory Controller ID in Interleave check
  @param[in] LastMcId           Last Memory Memory Controller ID to check
  @param[in] MemInfo            Memory Information structure

  @retval TRUE    exactly 2 memory controllers share a 3-way interleave in a cluster on one socket.
  @retval FALSE   3-way interleave across 2 memory controllers was not found.

**/
BOOLEAN
Is3wayInterleaveAcross2Mcs (
  IN UINT8                      Socket,
  IN UINT8                      FirstMcId,
  IN UINT8                      LastMcId,
  IN MEM_INFO                   *MemInfo
  )
{
  UINT8 SadCluster;
  UINT8 McBitMap;
  UINT8 MaxImc;
  UINT8 McId;

  //
  // build McBitMap to find which MC are interleaved, based on function parameters
  //
  MaxImc = GetMaxImc ();
  McBitMap = 0;
  for (McId = FirstMcId; (McId <= LastMcId) && (McId < MaxImc); McId++) {
    if (!CheckSocketMcEnabled (Socket, McId)) {
      continue;
    }
    McBitMap |= (BIT0 << McId);
  }

  //
  // Find the right cluster in MemInfo structure, verify exactly 2 MCs, and verify 3-way interleave.
  //
  for (SadCluster = 0; SadCluster < MemInfo->SncInfo[Socket].NumOfCluster; SadCluster++) {
    if ((MemInfo->SncInfo[Socket].McBitmapPerCluster[SadCluster] & McBitMap) != 0) {
      if ((MemInfo->SncInfo[Socket].McBitmapPerCluster[SadCluster] & ~McBitMap) != 0) {
        return FALSE;
      }
      if (MemInfo->Ways[Socket][SadCluster] == 3) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
  Determine the channel configuration meets the symmetric requirement or not
  1-Cluster: Need all enabled MCs and their populated channels symmetric
  2-Cluster (SNC2): Need MCs in the same cluster symmetric. Channels within
                    MC also need to be symmetric.
  4-cluster (SNC4): Populated channels within the MC should be symmetric

  @param[in] KtiInternalGlobal  KTIRC Internal Global data
  @param[in] Socket             Socket Node ID
  @param[in] MemInfo            Memory Information structure

  @retval KTI_FAILURE  Doesn't meet prefetch symmetric requirement
          KTI_SUCCESS  Meet prefetch symmetric requirement, or
                       prefetch was set to disable during SAD entries scanning
                       and therefore symmetric check can be skipped.

**/
KTI_STATUS
CheckPrefetchSymmetric (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      Socket,
  IN MEM_INFO                   *MemInfo
  )
{
  UINT8    MaxImc;
  UINT8    MaxChPerMc;
  UINT8    Index;
  BOOLEAN  DisablePrefetch;

  MaxImc   = GetMaxImc ();
  MaxChPerMc = GetNumChannelPerMc ();

  KtiDebugPrintInfo0 ((KTI_DEBUG_INFO0, "CheckPrefetchSymmetric\n"));

  DisablePrefetch = FALSE;
  if (MemInfo->SncInfo[Socket].NumOfCluster == 1) {
    if (MemInfo->SncInfo[Socket].NumOfMcEnabled > 1) {
      //
      // All MCs in SNC1 should have symmetric channel population
      //
      if (!ChannelsWithinClusterSymmetric (Socket, 0, (MaxImc - 1), MaxChPerMc)) {
        DisablePrefetch = TRUE;
        //
        // 3 channel interleave across 2 MCs is not symmetric, but prefetch is supported
        //
        if (MaxChPerMc < 3) {
          if (Is3wayInterleaveAcross2Mcs (Socket, 0, (MaxImc - 1), MemInfo)) {
            DisablePrefetch = FALSE;
          }
        }
      }
    } else {
      for (Index = 0; Index < MaxImc; Index++) {
        if (((MemInfo->SncInfo[Socket].McBitmapPerCluster[0] >> Index) & 0x1) == 0) {
          continue;
        }
        if (!ChannelsWithinMcSymmetric (Socket, Index, MaxChPerMc, NULL, NULL)) {
          DisablePrefetch = TRUE;
        }
        break;
      }
    }
  } else if (MemInfo->SncInfo[Socket].NumOfCluster == 2) {
    if ((MemInfo->SncInfo[Socket].NumOfMcPerCluster[0] == 1) && (MemInfo->SncInfo[Socket].NumOfMcPerCluster[1] == 1)) {
      //
      // Populated channels within MC should be symmetric
      //
      for (Index = 0; Index < MaxImc; Index++) {
        if (!CheckSocketMcEnabled (Socket, Index)) {
          continue;
        }
        if (!ChannelsWithinMcSymmetric (Socket, Index, MaxChPerMc, NULL, NULL)) {
          DisablePrefetch = TRUE;
          break;
        }
      }
    } else {
      //
      // 2-Cluster: left side MCs(Mc0 & MC1) should be symmetric and right side MCs(Mc2 & MC3) should be symmetric
      //
      if (!(ChannelsWithinClusterSymmetric (Socket, 0, 1, MaxChPerMc) && ChannelsWithinClusterSymmetric (Socket, 2, 3, MaxChPerMc))) {
        DisablePrefetch = TRUE;
        //
        // 3 channel interleave across 2 MCs is not symmetric, but prefetch is supported
        //
        if (MaxChPerMc < 3) {
          if (Is3wayInterleaveAcross2Mcs (Socket, 0, 1, MemInfo) && Is3wayInterleaveAcross2Mcs (Socket, 2, 3, MemInfo)) {
            DisablePrefetch = FALSE;
          }
        }
      }
    }
  } else {
    //
    // 4-Cluster: should check all populated channels within MC are symmetric
    //
    for (Index = 0; Index < MaxImc; Index++) {
      if (!CheckSocketMcEnabled (Socket, Index)) {
        continue;
      }
      if (!ChannelsWithinMcSymmetric (Socket, Index, MaxChPerMc, NULL, NULL)) {
        DisablePrefetch = TRUE;
        break;
      }
    }
  }

  if (DisablePrefetch) {
    KtiDebugPrintInfo0 ((
      KTI_DEBUG_INFO0,
      "\nS%d %d-Cluster does not have all channels symmetric to enable the XPT/UPI Prefetch, therefore disable XPT/UPI prefetch\n",
      Socket,
      MemInfo->SncInfo[Socket].NumOfCluster
      ));
    return KTI_FAILURE;
  } else {
    return KTI_SUCCESS;
  }
}