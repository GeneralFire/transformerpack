/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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


/**
  Check Xor Interleave Symmetry Requirement

  Evaluate DIMM population symmetry to see if satisfies POR requirement or not.
  POR configuration is performance recommended and should be evaluated with priority.
  Non-POR configuration is not performance recommended and would go through speical handling flow.

  The implementation of this routine is wave specific.

  @param[in]      Host                                Pointer to the host structure.
  @param[in]      MemMapData                          Pointer to the memory map data structure.
  @param[in, out] SocketBitmap                        Bitmap of sockets requested/possible for interleave.
  @param[in]      Cluster                             Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType                             Type of memory being interleaved.
  @param[in]      McBitmap[MAX_SOCKET]                Mc bit map of the interleave to be evaluated.
  @param[in]      ChannelBitmap[MAX_SOCKET][MAX_IMC]  Channel bit map of the interleave to be evaluated.
  @param[in]      McWays                              Ways of mc of the interleave to be evaluated.
  @param[in]      ChannelWays                         Ways of channel of the interleave to be evaluated.

  @retval SUCCESS            The DIMM population symmetry is POR.
  @retval RETRY              The DIMM population symmetry is Non-POR, but should re-evaluate the same interleave
                             configuration by asymmetric flow.
  @retval FAILURE            The DIMM population symmetry is non-POR, and should try smaller interleave configurations.
  @retval INVALID_PARAMETER  Parameter check failure
**/
UINT32
CheckXorInterleaveSymmetryRequirement (
  IN  SYSHOST             *Host,
  IN  MEMORY_MAP_DATA     *MemMapData,
  IN  UINT8               SocketBitmap,
  IN  UINT8               Cluster,
  IN  MEM_TYPE            MemType,
  IN  UINT8               McBitmap[MAX_SOCKET],
  IN  UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC],
  IN  UINT8               McWays,
  IN  UINT8               ChannelWays
  )
{

  BOOLEAN              SncEnabled;
  UINT8                MaxImc;
  UINT8                SocketIndex;
  UINT8                IntMcIndex;
  UINT8                ChannelIndex;
  UINT8                MaxChPerMc;
  UINT8                FirstMcInInterleave;
  UINT8                NumberOfChannels;
  UINT8                NumberOfChannelsOnMc[MAX_SOCKET][MAX_IMC];
  CHANNEL_SORT_STRUCT  SortList[MAX_SOCKET][MAX_CH];
  BOOLEAN              SymmetryTestPassed;
  BOOLEAN              AcrossHalf;
  UINT32               Status;

  if ((McBitmap == NULL) || (ChannelBitmap == NULL) || (Host == NULL) || (MemMapData == NULL) || (SocketBitmap == 0)) {
    return INVALID_PARAMETER;
  }

  ZeroMem (NumberOfChannelsOnMc, sizeof (NumberOfChannelsOnMc));
  ZeroMem (SortList, sizeof (SortList));
  MaxChPerMc = GetNumChannelPerMc ();
  MaxImc = GetMaxImc();
  SymmetryTestPassed = TRUE;

  if (MemType == MemType1lmAppDirect) {
    //
    // Use the standard interleaving algorithm for AppDirect Memory.
    //
    SncEnabled = FALSE;
  } else {
    SncEnabled = IfFeatureEnabled (TypeSnc);
  }

  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {

    //
    // POR evaluation is carried out in a per-socket basis.
    // POR requirement needs all sockets in the interleave list to satisfy the symmetry condition
    // If any one fails, the whole check will return FALSE
    //
    if (CheckSocketEnabled (SocketIndex) == 0 ||
        GetSocketMemSize (SocketIndex) == 0) {
      continue;
    }

    if (((SocketBitmap >> SocketIndex) & BIT0) == 0) {
      continue;
    }

    //
    // Prepare: Analyze socket DIMM population
    //
    for (IntMcIndex = 0; IntMcIndex < MaxImc; IntMcIndex++) {

      if (SncEnabled && GetClusterForMc (MemMapData, IntMcIndex) != Cluster) {
        continue;
      }

      //
      // Sort channels within MC by remaining size.
      //
      ChannelSortMc (
        Host,
        SocketIndex,
        (IntMcIndex * MaxChPerMc),
        SortList[SocketIndex]
        );

      NumberOfChannels = 0;
      for (ChannelIndex = 0; ChannelIndex < MaxChPerMc; ChannelIndex++) {
        if (SortList[SocketIndex][(IntMcIndex * MaxChPerMc) + ChannelIndex].chSize != 0) {
          NumberOfChannels ++;
        }
      }

      if (NumberOfChannels > 0) {
        NumberOfChannelsOnMc[SocketIndex][IntMcIndex] = NumberOfChannels;
      }
    }

    FirstMcInInterleave = (UINT8) LowBitSet32 (McBitmap[SocketIndex]);

    //
    // Evaluate symmetry condition based on mc/channels ways
    // 10nm requires MC interleave candidates to have full symmetry among them,
    // which guarantees only one MC rule (note: not CH rules) will be adopted in this MC sets.
    //
    // Checked configurations includes 8-way, 4-way across 4 MCs, 4-way across 2 MCs and 2-way across 2 MCs.
    // If you want to do 4-way (quadrant) interleave across 4 MCs, then there has to be 4-way symmetry.
    // A 2-way across 2 MCs can be performed within half or across half based on the 2 MC symmetry.
    //
    AcrossHalf = IsOneMcOnEachSide (McBitmap[SocketIndex]);

    if (McWays == IMC_4WAY && ChannelWays == CH_2WAY) {
      //
      // For each MC 1-3, make sure that the size of larger channel is equal to the
      // larger channel on MC 0, and the size of smaller to the smaller.
      //
      for (IntMcIndex = 1; IntMcIndex < MaxImc; IntMcIndex++) {
        if ((SortList[SocketIndex][(IntMcIndex * MaxChPerMc) + 0].chSize != SortList[SocketIndex][0].chSize) ||
            (SortList[SocketIndex][(IntMcIndex * MaxChPerMc) + 1].chSize != SortList[SocketIndex][1].chSize)) {
          SymmetryTestPassed = FALSE;
          goto Done;
        }
      }

    } else if (McWays == IMC_4WAY && ChannelWays == CH_1WAY) {
      for (IntMcIndex = 1; IntMcIndex < MaxImc; IntMcIndex++) {
        if (SortList[SocketIndex][(IntMcIndex * MaxChPerMc) + 0].chSize != SortList[SocketIndex][0].chSize) {
          SymmetryTestPassed = FALSE;
          goto Done;
        }
      }

    } else if (McWays == IMC_2WAY && ChannelWays == CH_2WAY) {
      for (IntMcIndex = 0; IntMcIndex < MaxImc; IntMcIndex++) {
        if ((McBitmap[SocketIndex] & (BIT0 << IntMcIndex)) == 0) {
          continue;
        }

        if (SortList[SocketIndex][(IntMcIndex * MaxChPerMc) + 0].chSize != SortList[SocketIndex][(FirstMcInInterleave * MaxChPerMc) + 0].chSize ||
            SortList[SocketIndex][(IntMcIndex * MaxChPerMc) + 1].chSize != SortList[SocketIndex][(FirstMcInInterleave * MaxChPerMc) + 1].chSize) {
          SymmetryTestPassed = FALSE;
          goto Done;
        }
      }

    } else if (McWays == IMC_2WAY && ChannelWays == CH_1WAY) {
      for (IntMcIndex = 0; IntMcIndex < MaxImc; IntMcIndex++) {
        if ((McBitmap[SocketIndex] & (BIT0 << IntMcIndex)) == 0) {
          continue;
        }

        //
        // For each MC, make sure that only one channel is present.
        //
        if ((NumberOfChannelsOnMc[SocketIndex][IntMcIndex] > 1) ||
          SortList[SocketIndex][(IntMcIndex * MaxChPerMc) + 0].chSize != SortList[SocketIndex][(FirstMcInInterleave * MaxChPerMc) + 0].chSize) {
          SymmetryTestPassed = FALSE;
          goto Done;
        }
      }
    }
  }

Done:

  Status = SUCCESS;

  if (!SymmetryTestPassed) {
    if ((McWays > IMC_1WAY) && (!AcrossHalf)) {
      return RETRY;
    } else {
      return FAILURE;
    }
  }

  return Status;
} // CheckXorInterleaveSymmetryRequirement

/**
  Create SAD rules for NUMA/UMA on many-MC silicon.

  This routine interleaves memory on the MC and channel level but not across
  sockets for NUMA configuration and across sockets for UMA configuration. It
  figures out the mapping and fills in the structures for the SAD table,
  but does not program any CSRs.

  @param[in, out] Host            Pointer to the host structure.
  @param[in, out] MemMapData      Pointer to the memory map data structure.
  @param[in]      SocketBitmap    Bitmap of sockets requested for interleave.
  @param[in]      Cluster         Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType         Type of memory being interleaved.

  @retval SUCCESS       Memory was successfully interleaved.
  @retval FAILURE       Interleave could not be supported for memory type.
**/
UINT32
SadInterleave (
  IN OUT  SYSHOST         *Host,
  IN OUT  MEMORY_MAP_DATA *MemMapData,
  IN      UINT8           SocketBitmap,
  IN      UINT8           Cluster,
  IN      MEM_TYPE        MemType
  )
{
  UINT32                Status;
  UINT8                 MaxImc;
  BOOLEAN               Possible;
  ROUTE_TABLE_HALVES    Half;
  BOOLEAN               AllowAcrossHalf;
  UINT8                 Sckt;
  UINT8                 NumChPerMc;
  UINT8                 McIndex;
  UINT8                 ChIndex;
  UINT8                 NumberOfMcs;
  UINT8                 NumberOfChannels;
  UINT8                 McBitmapInternal[MAX_SOCKET];
  CHANNEL_SORT_STRUCT   SortList[MAX_SOCKET][MAX_CH];
  UINT8                 McInHalf0;
  UINT8                 McInHalf1;
  UINT32                ChMemSize;
  UINT8                 CountRemainingCh;
  UINT8                 Index;
  UINT8                 CountNonZeroSizeCh;
  UINT8                 SocketIndex;

  if (Host == NULL || MemMapData == NULL) {
    return FAILURE;
  }

  if (Cluster >= MAX_CLUSTERS) {
    return FAILURE;
  }

  if (MemType == MemTypeNone || MemType == MemTypeMax) {
    return FAILURE;
  }

  AllowAcrossHalf = TRUE;
  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();
  ZeroMem (McBitmapInternal, sizeof (McBitmapInternal));
  ZeroMem (SortList, sizeof (SortList));
  Status = SUCCESS;

  if (MaxImc == IMC_4WAY && Host->var.mem.chInter == CH_3WAY) {
    //
    // 1. If only 6 channels are populated and 6-way possible do a 6-way.
    //
    Status = IsInterleavePossible (
               Host,
               MemMapData,
               SocketBitmap,
               Cluster,
               MemType,
               IMC_4WAY,
               CH_3WAY,
               &Possible,
               TRUE
               );
    //
    // 6-way requires channel sizes symmetry. So if a 6-way is performed there can be no pending interleave.
    //
    if (Possible) {
      return Status;
    }

    //
    // 2. If 6 way not possible, check 3-way in each half (SNC-2).
    //
    Status = IsInterleavePossible (
               Host,
               MemMapData,
               SocketBitmap,
               Cluster,
               MemType,
               IMC_2WAY,
               CH_3WAY,
               &Possible,
               TRUE
               );
    //
    // 3-way requires channel sizes symmetry. So if a 3-way is performed there can be no pending interleave.
    //
    if (Possible) {
      return Status;
    }
  } // IMC 4-ways

  NumberOfMcs = 0;
  SocketIndex = 0;
  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {

    if (((SocketBitmap >> Sckt) & BIT0) == 0 || GetSocketMemSize (Sckt) == 0) {
      continue;
    }

    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      if (!CheckSocketMcEnabled (Sckt, McIndex)) {
        continue;
      }
      //
      // Sort channels within MC by remaining size.
      //
      ChannelSortMc (
        Host,
        Sckt,
        (McIndex * NumChPerMc),
        SortList[Sckt]
        );

      NumberOfChannels = 0;
      for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
        if (SortList[Sckt][(McIndex * NumChPerMc) + ChIndex].chSize != 0) {
          NumberOfChannels++;
        }
      }

      if (NumberOfChannels != 0) {
        McBitmapInternal[Sckt] |= (BIT0 << McIndex);
        NumberOfMcs++;
      }
    } // McIndex

    //
    // For NUMA only one socket will be enabled at a time so we dont want to iterate for Sckt loop
    // For UMA we check only for first Sckt enabled having memory present
    //
    SocketIndex = Sckt;
    break;
  }

  //
  // In SNC we do not interleave across half.
  //
  if (IfFeatureEnabled (TypeSnc) && GetNumOfClusterPerSystem () > 1 && IsMemTypeVolatile (MemType)) {
    AllowAcrossHalf = FALSE;
  }

  //
  // If number of Mc's enabled was one , do not allow across half.
  //
  if (NumberOfMcs == IMC_1WAY) {
    AllowAcrossHalf = FALSE;
  }

  if (MaxImc == IMC_4WAY && Host->var.mem.chInter > CH_1WAY && AllowAcrossHalf) {

    //
    // If number of Mc's enabled was four check for quad symmetry.
    //
    if (NumberOfMcs == IMC_4WAY) {
      //
      // First check for 4*2 symmetry where all Ch0's and Ch1's are of same size.
      //
      for (McIndex = 1; McIndex < MaxImc; McIndex++) {
        if ((SortList[SocketIndex][(McIndex * NumChPerMc) + 0].chSize != SortList[SocketIndex][0].chSize) ||
          (SortList[SocketIndex][(McIndex * NumChPerMc) + 1].chSize != SortList[SocketIndex][1].chSize)) {
          AllowAcrossHalf = FALSE;
          break;
        }
      }

      if (AllowAcrossHalf) {
        //
        // 2. If >1 channels are populated in each MC, we check for a 4-way MC symmetry:
        // Arrange the channels within each MC as higher capacity (Ch0) and lower capacity (Ch1)
        // For a 4-way symmetry, all higher capacity channels should have the
        // same capacity, and all lower capacity channels should have the same capacity.
        // If a 4-way MC symmetry exists, do a 8-way interleave and then a 4-way interleave if necessary.
        //
        Status = IsInterleavePossible (
                   Host,
                   MemMapData,
                   SocketBitmap,
                   Cluster,
                   MemType,
                   IMC_4WAY,
                   CH_2WAY,
                   &Possible,
                   TRUE
                   );
        if (Status != SUCCESS) {
          return Status;
        }

        if (Possible) {
          Status = IsInterleavePossible (
                     Host,
                     MemMapData,
                     SocketBitmap,
                     Cluster,
                     MemType,
                     IMC_4WAY,
                     CH_1WAY,
                     &Possible,
                     TRUE
                     );

          return Status;
        }
      }

      //
      // If 4*2 symmtery was not possible check for 4*1 symmetry where all Ch0's are of same size,
      // remaining channels are of same size and power of two.
      //
      ChMemSize = 0;
      AllowAcrossHalf = TRUE;
      //
      // Check all Ch0's symmetry
      //
      for (McIndex = 0; McIndex < MaxImc; McIndex++) {

        if (ChMemSize == 0) {
          ChMemSize = SortList[SocketIndex][0 + (McIndex * NumChPerMc)].chSize;
          continue;
        }

        if (SortList[SocketIndex][0 + (McIndex * NumChPerMc)].chSize != ChMemSize) {
          AllowAcrossHalf = FALSE;
          break;
        }
      } // Ch0's symmetry

      //
      // Only if Ch0's symmetry is possible look for remaining channels symmetry and power of two.
      //
      if (AllowAcrossHalf) {
        ChMemSize = 0;
        CountRemainingCh = 0;
        CountNonZeroSizeCh = 0;
        for (McIndex = 0; McIndex < MaxImc; McIndex++) {

          if (SortList[SocketIndex][1 + (McIndex * NumChPerMc)].chSize == 0) {
            continue;
          }

          CountNonZeroSizeCh++;

          if (ChMemSize == 0) {
            ChMemSize = SortList[SocketIndex][1 + (McIndex * NumChPerMc)].chSize;
            CountRemainingCh++;
            continue;
          }

          if (SortList[SocketIndex][1 + (McIndex * NumChPerMc)].chSize == ChMemSize) {
            CountRemainingCh++;
          }
        } // remaining channel symmetry

          //
          // If remaining channels are non-power of two do not allow across half, try for 2*2 symmetry.
          //
        if (CountRemainingCh % 2 != 0 || CountNonZeroSizeCh != CountRemainingCh) {
          AllowAcrossHalf = FALSE;
        }
      } // if Ch0's symmetry

      if (AllowAcrossHalf) {
        Status = IsInterleavePossible (
                   Host,
                   MemMapData,
                   SocketBitmap,
                   Cluster,
                   MemType,
                   IMC_4WAY,
                   CH_1WAY,
                   &Possible,
                   TRUE
                   );
        if (Status != SUCCESS) {
          return Status;
        }

        if (Possible) {
          Status = IsInterleavePossible (
                     Host,
                     MemMapData,
                     SocketBitmap,
                     Cluster,
                     MemType,
                     IMC_2WAY,
                     CH_1WAY,
                     &Possible,
                     TRUE
                     );

          return Status;
        }
      }
    } // NumberOfMcs == IMC_4WAY

    //
    // If Number of Mc's enabled was greater than or equal to two, check for 2*2 East-West symmetry.
    // If previous quad symmetry check failed try for 2*2 East-West symmetry.
    //
    if (NumberOfMcs >= IMC_2WAY) {
      //
      // For East-West symmetry iterate through MC rows/columns(Index) since we need to check for 2*2 symmetry across half.
      // SoC supporting east-west hemi, Check for symmetry across MC0 and MC2, then check for symmetry across MC1 and MC3.
      // SoC supporting north-south hemi, Check for symmetry across MC0 and MC1, then check for symmetry across MC2 and MC3.
      //
      AllowAcrossHalf = TRUE;
      for (Index = 0; Index < MaxImc / 2; Index++) {
        if (Index % 2 == 0) {
          //
          // Get Mc's across half for checking 2*2 East-West symmetry.
          // For example ICX-SP get MC0 and MC2 for first iteration.
          //
          McInHalf0 = (UINT8) LowBitSet32 (GetMcBitMaskForHalf (RtHalf0));
          McInHalf1 = (UINT8) LowBitSet32 (GetMcBitMaskForHalf (RtHalf1));
        } else {
          //
          // Get Mc's across half for checking 2*2 East-West symmetry.
          // For example ICX-SP get MC1 and MC3 for second iteration.
          //
          McInHalf0 = (UINT8) HighBitSet32 (GetMcBitMaskForHalf (RtHalf0));
          McInHalf1 = (UINT8)HighBitSet32 (GetMcBitMaskForHalf (RtHalf1));
        }

        //
        // Check Mc's across half where all Ch0's and Ch1's are of same size.
        //
        if (((McBitmapInternal[SocketIndex] & (BIT0 << McInHalf0)) != 0) && ((McBitmapInternal[SocketIndex] & (BIT0 << McInHalf1)) != 0)) {
          if (SortList[SocketIndex][(McInHalf0*NumChPerMc) + 0].chSize != SortList[SocketIndex][(McInHalf1*NumChPerMc) + 0].chSize ||
            SortList[SocketIndex][(McInHalf0*NumChPerMc) + 1].chSize != SortList[SocketIndex][(McInHalf1*NumChPerMc) + 1].chSize) {
            AllowAcrossHalf = FALSE;
            break;
          }
        }
      }

      if (AllowAcrossHalf) {
        //
        // 3. If no 4-way MC symmetry exists, then look for 2-way MC symmetry across half.
        // If 2 way MC symmetry, then do a 4-way and 2-way interleave if necessary.
        //
        for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
          Status = IsInterleavePossible (
                     Host,
                     MemMapData,
                     SocketBitmap,
                     Cluster,
                     MemType,
                     IMC_2WAY,
                     CH_2WAY,
                     &Possible,
                     TRUE
                     );

          if (Status != SUCCESS) {
            return Status;
          }
        } // Half

        for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
          Status = IsInterleavePossible (
                     Host,
                     MemMapData,
                     SocketBitmap,
                     Cluster,
                     MemType,
                     IMC_2WAY,
                     CH_1WAY,
                     &Possible,
                     TRUE
                     );

          if (Status != SUCCESS) {
            return Status;
          }
        } // Half
      }
    } // NumberOfMcs >= IMC_2WAY
  } // MaxImc == IMC_4WAY

  if (MaxImc == IMC_4WAY && Host->var.mem.chInter > CH_1WAY) {
    //
    // 4. If no 2-way MC symmetry across half:
    // Do 4-way using the minimum channel size in each half
    // Do 2-way using remainder of the minimum Ch0 size
    // Do 1-way using remainder of the maximum Ch0 size
    // Do 1-way using remainder of the Ch1 size if it is in the same MC as maximum Ch0 size
    // NOTE: It is possible that we don't use some of the memory that is populated (wasted memory due to uneven population)
    //
    for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
      Status = IsInterleavePossible (
                 Host,
                 MemMapData,
                 SocketBitmap,
                 Cluster,
                 MemType,
                 IMC_2WAY,
                 CH_2WAY,
                 &Possible,
                 FALSE
                 );

      if (Status != SUCCESS) {
        return Status;
      }
    } // Half

    for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
      Status = IsInterleavePossible (
                 Host,
                 MemMapData,
                 SocketBitmap,
                 Cluster,
                 MemType,
                 IMC_2WAY,
                 CH_1WAY,
                 &Possible,
                 FALSE
                 );

      if (Status != SUCCESS) {
        return Status;
      }
    } // Half

    for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
      Status = IsInterleavePossible (
                 Host,
                 MemMapData,
                 SocketBitmap,
                 Cluster,
                 MemType,
                 IMC_1WAY,
                 CH_2WAY,
                 &Possible,
                 FALSE
                 );

      if (Status != SUCCESS) {
        return Status;
      }
    } // Half

    //
    // Left over interleave shall only be 1-way
    //
    while (TRUE) {
      Status = IsInterleavePossible (
                 Host,
                 MemMapData,
                 SocketBitmap,
                 Cluster,
                 MemType,
                 IMC_1WAY,
                 CH_1WAY,
                 &Possible,
                 FALSE
                 );
      if (Status != SUCCESS) {
        return Status;
      }
      if (!Possible) {
        break;
      }
    }

  return Status;
  } // IMC 4-ways

  if (MaxImc >= IMC_2WAY && Host->var.mem.chInter == CH_3WAY) {
    //
    // 1. Wave1 CPU can support a special case of three way interleave across two halves for 1LM non-mirror mode.
    // 2 channels from 1 IMC in one half and 1 one channel from the other IMC in the other half.
    // 3-way is possible only if 3 channels populated symmetrically
    //
    Status = IsInterleavePossible (
               Host,
               MemMapData,
               SocketBitmap,
               Cluster,
               MemType,
               IMC_2WAY,
               CH_3WAY,
               &Possible,
               TRUE
               );

    if (Possible) {
      return Status;
    }
  }

  if (MaxImc >= IMC_2WAY && Host->var.mem.chInter > CH_1WAY && AllowAcrossHalf) {
    //
    // For East-West symmetry iterate through MC rows/columns(Index) since we need to check for 2*2 symmetry across half.
    // SoC supporting east-west hemi, Check for symmetry across MC0 and MC1.
    //
    McInHalf0 = (UINT8) LowBitSet32 (GetMcBitMaskForHalf (RtHalf0));
    McInHalf1 = (UINT8) LowBitSet32 (GetMcBitMaskForHalf (RtHalf1));
    //
    // Check Mc's across half where all Ch0's and Ch1's are of same size.
    //
    if (((McBitmapInternal[SocketIndex] & (BIT0 << McInHalf0)) != 0) && ((McBitmapInternal[SocketIndex] & (BIT0 << McInHalf1)) != 0)) {
      if (SortList[SocketIndex][(McInHalf0*NumChPerMc) + 0].chSize != SortList[SocketIndex][(McInHalf1*NumChPerMc) + 0].chSize ||
        SortList[SocketIndex][(McInHalf0*NumChPerMc) + 1].chSize != SortList[SocketIndex][(McInHalf1*NumChPerMc) + 1].chSize) {
        AllowAcrossHalf = FALSE;
      }
    }

    if (AllowAcrossHalf) {
      //
      // 2. Look for 2-way MC symmetry across half
      //
      Status = IsInterleavePossible (
                 Host,
                 MemMapData,
                 SocketBitmap,
                 Cluster,
                 MemType,
                 IMC_2WAY,
                 CH_2WAY,
                 &Possible,
                 TRUE
                 );

      if (Status != SUCCESS) {
        return Status;
      }

      Status = IsInterleavePossible (
                 Host,
                 MemMapData,
                 SocketBitmap,
                 Cluster,
                 MemType,
                 IMC_2WAY,
                 CH_1WAY,
                 &Possible,
                 TRUE
                 );

      if (Status != SUCCESS) {
        return Status;
      }
    }
  } // MaxImc >= IMC_2WAY

  if (MaxImc >= IMC_2WAY && Host->var.mem.chInter > CH_1WAY) {
    //
    // 3. If no 2-way MC symmetry across half:
    // * Do 2-way using the minimum channel size in each half
    // * Do 1-way using remainder of the minimum Ch0 size
    //
    for (Half = RtHalf0; Half < MaxRtHalves; Half++) {
      Status = IsInterleavePossible (
                 Host,
                 MemMapData,
                 SocketBitmap,
                 Cluster,
                 MemType,
                 IMC_1WAY,
                 CH_2WAY,
                 &Possible,
                 FALSE
                 );

      if (Status != SUCCESS) {
        return Status;
      }
    } // Half

    //
    // Left over interleave shall only be multiple 1-way
    //
    while (TRUE) {
      Status = IsInterleavePossible (
                 Host,
                 MemMapData,
                 SocketBitmap,
                 Cluster,
                 MemType,
                 IMC_1WAY,
                 CH_1WAY,
                 &Possible,
                 FALSE
                 );
      if (Status != SUCCESS) {
        return Status;
      }
      if (!Possible) {
        break;
      }
    }

    return Status;
  } // IMC 2-ways

  //
  // Remaining interleave is retricted only within one half
  //
  if (MaxImc >= IMC_1WAY && Host->var.mem.chInter > CH_1WAY) {
    Status = IsInterleavePossible (
               Host,
               MemMapData,
               SocketBitmap,
               Cluster,
               MemType,
               IMC_1WAY,
               CH_3WAY,
               &Possible,
               FALSE
               );
    if (Status != SUCCESS) {
      return Status;
    }

    Status = IsInterleavePossible (
               Host,
               MemMapData,
               SocketBitmap,
               Cluster,
               MemType,
               IMC_1WAY,
               CH_2WAY,
               &Possible,
               FALSE
               );
    if (Status != SUCCESS) {
      return Status;
    }
  } // IMC 1-way

  //
  // Left over interleave shall only be multiple 1-way
  //
  while (TRUE) {
    Status = IsInterleavePossible (
               Host,
               MemMapData,
               SocketBitmap,
               Cluster,
               MemType,
               IMC_1WAY,
               CH_1WAY,
               &Possible,
               FALSE
               );
    if (Status != SUCCESS) {
      return Status;
    }
    if (!Possible) {
      break;
    }
  }

  return Status;
} // SadInterleave

/** Check CPU capable of supporting 3 channel interleave across two MC's.

  Check the installed CPU is a one supporting channel interleave across MC's.

  @retval TRUE     -  CPU capable of supporting the feature.
  @retval FALSE    -  CPU cannot support the feature.

**/
BOOLEAN
IsCpuCapableThreeChannelInterleaveAcrossMcs (
  VOID
  )
{
  //
  // If 3 channels supported per MC then support for 3 channels across 2 MC's is invalidated
  //
  if (GetNumChannelPerMc () == 3) {
    return FALSE;
  }

  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Check if 3 channel interleaving is possible for many (>2) MCs.

  @param[in]      Host              Pointer to the host structure.
  @param[in]      MemMapData        Pointer to the memory map data structure.
  @param[in, out] SocketBitmap      Bitmap of sockets requested/possible for interleave.
  @param[in]      Cluster           Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType           Type of memory being interleaved.
  @param[in]      McWays            Number of MCs to interleave.
  @param[in]      ChannelWays       Number of channels to interleave per MC.
  @param[out]     McBitmap          Bitmap of MCs in the interleave.
  @param[out]     ChannelBitmap     Array of bitmaps of channels.
  @param[out]     FmChannelBitmap   Array of bitmaps of channels of Far Memory type.
  @param[out]     ChannelSortList   Channels per MC in order of decreasing size.
  @param[in]      AllowAcrossHalves Specify if the interleave can be checked within or across half.

  @retval TRUE          The given interleave is possible.
  @retval FALSE         The given interleave is impossible.
**/
BOOLEAN
IsMod3InterleavePossible (
  IN  SYSHOST             *Host,
  IN  MEMORY_MAP_DATA     *MemMapData,
  IN OUT UINT8            *SocketBitmap,
  IN  UINT8               Cluster,
  IN  MEM_TYPE            MemType,
  IN  UINT8               McWays,
  IN  UINT8               ChannelWays,
  OUT UINT8               McBitmap[MAX_SOCKET],
  OUT UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC],
  OUT UINT8               FmChannelBitmap[MAX_SOCKET][MAX_IMC],
  OUT CHANNEL_SORT_STRUCT ChannelSortList[MAX_SOCKET][MAX_CH],
  IN  BOOLEAN             AllowAcrossHalves
  )
{
  UINT8               MaxImc;
  UINT8               RasMode;
  UINT8               McIndex;
  UINT8               SocketIndex;
  UINT8               BeginSktIndex;
  UINT8               EndSktIndex;
  UINT8               ChIndex;
  UINT8               NumberOfMcs;
  UINT8               NumberOfChannels;
  UINT8               PrevSocketInterleaved;
  UINT8               SocketWays;
  UINT8               SocketBitmapInternal;
  UINT8               TargetWays;
  UINT8               ChannelBitmapDiscovered;
  UINT8               McBitmapInternal[MAX_SOCKET];
  UINT8               ChannelBitmapInternal[MAX_SOCKET][MAX_IMC];
  UINT8               NmChannelBitmapInternal[MAX_SOCKET][MAX_IMC];
  UINT8               NumberOfChannelsOnMc[MAX_SOCKET][MAX_IMC];
  UINT8               NmNumberOfChannels[MAX_SOCKET][MAX_IMC];
  CHANNEL_SORT_STRUCT SortList[MAX_SOCKET][MAX_CH];
  ROUTE_TABLE_HALVES  Half;
  BOOLEAN             Possible;
  UINT32              ChSymmetrySize;
  UINT8               NumChPerMc;
  UINT8               MaxChDdr;
  if (Host == NULL || MemMapData == NULL || SocketBitmap == NULL) {
    return FALSE;
  }

  if (Host->var.mem.chInter != CH_3WAY ||
      ChannelWays != CH_3WAY) {
    return FALSE;
  }

  if (McBitmap == NULL || ChannelBitmap == NULL || FmChannelBitmap == NULL || ChannelSortList == NULL) {
    return FALSE;
  }

  MaxImc = GetMaxImc ();
  if (McWays == 0 || McWays > MaxImc) {
    return FALSE;
  }

  if (Cluster >= MAX_CLUSTERS) {
    return FALSE;
  }

  //
  // 3-way in a half is supported in SNC-2 and 2 MCs per half.
  //
  if (IfFeatureEnabled (TypeSnc)) {
    if ((GetNumOfClusterPerSystem () == KTI_SNC2 && GetNumMcPerHalf () == 1) ||
         GetNumOfClusterPerSystem () == KTI_SNC4) {
      return FALSE;
    }
  }

  NumChPerMc = GetNumChannelPerMc ();

  //
  // For 3 channel SKU's support Mod3 for all memory types populated.
  //
  if (NumChPerMc != CH_3WAY && MemType != MemType1lmDdr) {
    return FALSE;
  }

  //
  // Check request is for 3-way within an MC, 3-way across halves or 6-way across 4 MC's
  // 3-way across 2 MC's is not POR for ICX
  //
  if (McWays < IMC_4WAY) {
    if (!IsCpuCapableThreeChannelInterleaveAcrossMcs () && NumChPerMc != CH_3WAY) {
      return FALSE;
    }
  }

  //
  // Support for 3 channels interleaving across 2 MCs is only supported for 1LM and is not compatible with mirror mode.
  //
  if (NumChPerMc != CH_3WAY) {
    RasMode = GetRasMode ();
    if ((RasMode & PARTIAL_MIRROR_1LM) != 0 ||
        (RasMode & FULL_MIRROR_1LM) != 0) {
      return FALSE;
    }
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

  //
  // Support for Mod3 across 2 MCs in 1LM + 2LM configurations is restricted.
  // In 1LM + 2LM mode, where DDRT granularity is 4KB, 1LM and 2LM use separate route tables,
  // so Mod3 can be allowed for 1LM only.
  // In 1LM + 2LM mode, where DDRT granularity is 256B, 1LM and 2LM share the same route table,
  // so Mod3 can be allowed only if an SoC supports X-tile 2LM.
  //
  for (SocketIndex = BeginSktIndex; SocketIndex <= EndSktIndex; SocketIndex++) {
    if (CheckSocketEnabled (SocketIndex) == 0 || GetSocketMemSize (SocketIndex) == 0) {
      continue;
    }
    if (Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM && (NumChPerMc != CH_3WAY)) {
      if (!MemMapData->Is2LmUsingRt2lm && !MemMapData->MemMapSocket[SocketIndex].XTile2lm) {
        return FALSE;
      }
    }
  }


  Possible = FALSE;
  NumberOfMcs = 0;
  SocketWays = 0;
  SocketBitmapInternal = 0;
  TargetWays = 0;
  ZeroMem (McBitmapInternal, sizeof (McBitmapInternal));
  ZeroMem (ChannelBitmapInternal, sizeof (ChannelBitmapInternal));
  ZeroMem (NmChannelBitmapInternal, sizeof (NmChannelBitmapInternal));
  ZeroMem (NumberOfChannelsOnMc, sizeof (NumberOfChannelsOnMc));
  ZeroMem (NmNumberOfChannels, sizeof (NmNumberOfChannels));
  ZeroMem (SortList, sizeof (SortList));

  PrevSocketInterleaved = BeginSktIndex;
  ChSymmetrySize = 0;
  MaxChDdr = GetMaxChDdr ();

  for (SocketIndex = BeginSktIndex; SocketIndex <= EndSktIndex; SocketIndex++) {
    NumberOfMcs = 0;

    if (CheckSocketEnabled (SocketIndex) == 0 ||
        GetSocketMemSize (SocketIndex) == 0) {
      continue;
    }

    for (McIndex = 0; McIndex < MaxImc; McIndex++) {
      NumberOfChannels = 0;
      ChannelBitmapDiscovered = 0;
      for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
        if (Host->var.mem.socket[SocketIndex].channelList[(McIndex * NumChPerMc) + ChIndex].remSize != 0) {
          ChannelBitmapDiscovered |= (BIT0 << ChIndex);
          NumberOfChannels++;
        }
      }

      NumberOfChannelsOnMc[SocketIndex][McIndex] = NumberOfChannels;
      if (NumberOfChannels != 0) {
        McBitmapInternal[SocketIndex] |= (BIT0 << McIndex);
        ChannelBitmapInternal[SocketIndex][McIndex] = ChannelBitmapDiscovered;
        NumberOfMcs++;
      }

      if (NumberOfMcs >= McWays) {
        Possible = TRUE;
        break;
      }
    }

    //
    // Mod3 should be evenly populated and only the required number of channels
    //
    if (Possible) {
      if (IsCpuCapableThreeChannelInterleaveAcrossMcs () || NumChPerMc == CH_3WAY) {
        NumberOfChannels = 0;
        //
        // Find the total number of channels with memory populated in the socket
        //
        for (McIndex = 0; McIndex < MaxImc; McIndex++) {
          if ((McBitmapInternal[SocketIndex] & (BIT0 << McIndex)) == 0) {
            continue;
          }
          for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
            if (Host->var.mem.socket[SocketIndex].channelList[ChIndex + (McIndex * NumChPerMc) ].remSize != 0) {
              NumberOfChannels++;
            }
          }
        }
        if (NumberOfChannels % ChannelWays != 0) {
          Possible = FALSE;
        }
      } else {
        for (Half = 0; Possible == TRUE && Half < MaxRtHalves; Half++) {
          NumberOfChannels = 0;
          for (McIndex = 0; McIndex < MaxImc; McIndex++) {
            if ((McBitmapInternal[SocketIndex] & (BIT0 << McIndex)) == 0) {
              continue;
            }
            if (GetHalfForMc (McIndex) == Half) {
              NumberOfChannels += NumberOfChannelsOnMc[SocketIndex][McIndex];
            }
          }
          if (NumberOfChannels % ChannelWays != 0) {
            Possible = FALSE;
            break;
          }
        }
      }
    }

    //
    // Mod3 interleaving necessitate channel sizes symmetry check if silicon is not capable of 3 channels per MC.
    //
    if (Possible && NumChPerMc != CH_3WAY) {
      for (ChIndex = 0; (Possible == TRUE && ChIndex < MaxChDdr); ChIndex++) {
        if (Host->var.mem.socket[SocketIndex].channelList[ChIndex].remSize == 0) {
          continue;
        }
        if (ChSymmetrySize == 0) {
          ChSymmetrySize = Host->var.mem.socket[SocketIndex].channelList[ChIndex].remSize;
        }
        if ((Host->var.mem.socket[SocketIndex].channelList[ChIndex].remSize != ChSymmetrySize) ||
            (Host->var.mem.socket[PrevSocketInterleaved].channelList[ChIndex].remSize != ChSymmetrySize)) {
          Possible = FALSE;
        }
      }
    }

    //
    // If interleave still looks possible, check the NM population for 2LM configurations. Also populate
    // the NMChannelBitmap.
    //
    if (Possible && IsMemType2lm (MemType)) {
      //
      // Find the number of channels with ddr4 memory populated and their bit map in each iMC
      //
      for (McIndex = 0; McIndex < MaxImc; McIndex++) {
        if ((McBitmapInternal[SocketIndex] & (BIT0 << McIndex)) == 0) {
          continue;
        }
        NmChannelBitmapInternal[SocketIndex][McIndex] = FindNmBitMap (SocketIndex, McIndex);
        for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
          if ((NmChannelBitmapInternal[SocketIndex][McIndex] & (BIT0 << ChIndex)) != 0) {
            NmNumberOfChannels[SocketIndex][McIndex]++;
          }

          //
          // Check for in-tile. Each MC with Far Memory must also have Near Memory
          //
          if (NmNumberOfChannels[SocketIndex][McIndex] == 0) {
            Possible = FALSE;
          }
        }
      }
    }

    if (Possible) {
      SocketWays++;
      PrevSocketInterleaved = SocketIndex;
      SocketBitmapInternal |= (BIT0 << SocketIndex);

      for (McIndex = 0; McIndex < MaxImc; McIndex++) {
        if ((McBitmapInternal[SocketIndex] & (BIT0 << McIndex)) == 0) {
          continue;
        }
        //
        // Sort channels within MC by remaining size.
        //
        ChannelSortMc (
          Host,
          SocketIndex,
          (McIndex * NumChPerMc),
          SortList[SocketIndex]
        );
      }
    }

  } // SocketIndex

  //
  // If the interleave is possible and SNC-2 then reduce the bitmaps and sort list only to MCs within the cluster
  //
  if (Possible && IfFeatureEnabled (TypeSnc)) {
    for (SocketIndex = BeginSktIndex; SocketIndex <= EndSktIndex; SocketIndex++) {
      if ((SocketBitmapInternal & (BIT0 << SocketIndex)) == 0) {
        continue;
      }
      for (McIndex = 0; McIndex < MaxImc; McIndex++) {
        if (GetClusterForMc (MemMapData, McIndex) == Cluster) {
          continue;
        }
        McBitmapInternal[SocketIndex] &= ~(BIT0 << McIndex);
        ChannelBitmapInternal[SocketIndex][McIndex] = 0;
        NmChannelBitmapInternal[SocketIndex][McIndex] = 0;
        ZeroMem (&SortList[SocketIndex][McIndex * NumChPerMc], (sizeof (CHANNEL_SORT_STRUCT) * NumChPerMc));

      } // McIndex
    } // SocketIndex
  }

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
        ChannelBitmap,
        NmChannelBitmapInternal,
        sizeof (NmChannelBitmapInternal)
        );
      CopyMem (
        FmChannelBitmap,
        ChannelBitmapInternal,
        sizeof (ChannelBitmapInternal)
        );
    }

    CopyMem (
      ChannelSortList,
      SortList,
      sizeof (SortList)
      );

    *SocketBitmap = SocketBitmapInternal;

    MemMapData->IsPorConfig = TRUE;
  }

  return Possible;
} // IsMod3InterleavePossible

/**
  Do all possible interleaving with given ways for many (>2) MCs. If requested interleave is
  across sockets then the SocketBitmap output represents the sockets that can be interleaved.

  @param[in, out] Host                Pointer to the host structure.
  @param[in, out] MemMapData          Pointer to the memory map data structure.
  @param[in, out] SocketBitmap        Bitmap of sockets requested/possible for interleave.
  @param[in]      Cluster             Cluster in socket for SNC, or 0 if not SNC.
  @param[in]      MemType             Type of memory being interleaved.
  @param[in]      McWays              Number of MCs to interleave.
  @param[in]      ChannelWays         Number of channels to interleave per MC.
  @param[out]     Possible            Requested interleave can be done or not possible.
  @param[in]      AllowAcrossHalves   Requested interleave is across halves or within a half.

  @retval SUCCESS       Requested interleave can be done or not possible.
  @retval FAILURE       Interleaving should have been possible, but failed.
**/
UINT32
IsInterleavePossible (
  IN OUT  SYSHOST         *Host,
  IN OUT  MEMORY_MAP_DATA *MemMapData,
  IN      UINT8            SocketBitmap,
  IN      UINT8            Cluster,
  IN      MEM_TYPE         MemType,
  IN      UINT8            McWays,
  IN      UINT8            ChannelWays,
  OUT     BOOLEAN          *Possible,
  IN      BOOLEAN          AllowAcrossHalves
  )
{
  UINT32              Status;
  UINT8               McBitmap[MAX_SOCKET];
  UINT8               NmMcBitmap[MAX_SOCKET];
  UINT8               ChannelBitmap[MAX_SOCKET][MAX_IMC];
  UINT8               FmChannelBitmap[MAX_SOCKET][MAX_IMC];
  CHANNEL_SORT_STRUCT ChannelSortList[MAX_SOCKET][MAX_CH];
  UINT8               Socket;
  UINT32              NodeLimit;

  Status = SUCCESS;   // default to success when not possible
  Socket = 0;

  if (Host == NULL || MemMapData == NULL || Possible == NULL) {
    return FALSE;
  }

  if (Cluster >= MAX_CLUSTERS) {
    return FALSE;
  }

  if (MemType == MemTypeNone || MemType == MemTypeMax) {
    return FAILURE;
  }

  if (McWays == 0 || McWays > GetMaxImc ()) {
    return FALSE;
  }

  if (ChannelWays == 0) {
    return FALSE;
  }

  ZeroMem (McBitmap, sizeof (McBitmap));
  ZeroMem (NmMcBitmap, sizeof (NmMcBitmap));
  ZeroMem (&ChannelBitmap, sizeof (ChannelBitmap));
  ZeroMem (&FmChannelBitmap, sizeof (FmChannelBitmap));

  if (ChannelWays == CH_3WAY) {
      *Possible = IsMod3InterleavePossible (
                    Host,
                    MemMapData,
                    &SocketBitmap,
                    Cluster,
                    MemType,
                    McWays,
                    ChannelWays,
                    McBitmap,
                    ChannelBitmap,
                    FmChannelBitmap,
                    ChannelSortList,
                    AllowAcrossHalves
                   );
    if (!*Possible) {
      return Status;
    }

    Status = ExecuteMod3Interleave (
               Host,
               MemMapData,
               SocketBitmap,
               Cluster,
               MemType,
               McWays,
               ChannelWays,
               McBitmap,
               ChannelBitmap,
               FmChannelBitmap,
               ChannelSortList,
               &NodeLimit,
               MIRROR_EN_DEFAULT
               );
    if (Status != SUCCESS) {
      return Status;
    }

    //
    // Add remote SAD entry here for UMA since the IMC targets are the same until now.
    // Get the Limit value for this node to create remote sad entry in other nodes.
    //
    if (!MemMapData->IsNumaEn) {
      Status = AddUmaRemoteSadEntry (Host, MemMapData, McBitmap, NodeLimit, MemType);
      if (Status != SUCCESS) {
        return Status;
      }
    }
  } else {
    *Possible = IsXorInterleavePossible (
                  Host,
                  MemMapData,
                  &SocketBitmap,
                  Cluster,
                  MemType,
                  McWays,
                  ChannelWays,
                  McBitmap,
                  NmMcBitmap,
                  ChannelBitmap,
                  FmChannelBitmap,
                  ChannelSortList,
                  AllowAcrossHalves
                  );
    if (!*Possible) {
      return Status;
    }

    Status = ExecuteXorInterleave (
               Host,
               MemMapData,
               SocketBitmap,
               Cluster,
               MemType,
               McWays,
               ChannelWays,
               McBitmap,
               NmMcBitmap,
               ChannelBitmap,
               FmChannelBitmap,
               ChannelSortList,
               &NodeLimit,
               MIRROR_EN_DEFAULT
               );
    if (Status != SUCCESS) {
      return Status;
    }
    //
    // Add remote SAD entry here  for UMA since the IMC targets are the same until now.
    // Get the Limit value for this node to create remote sad entry in other nodes.
    //
    if (!MemMapData->IsNumaEn) {
      Status = AddUmaRemoteSadEntry (Host, MemMapData, McBitmap, NodeLimit, MemType);
      if (Status != SUCCESS) {
        return Status;
      }
    }
  }

  return Status;
} // IsInterleavePossible