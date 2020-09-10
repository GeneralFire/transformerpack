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

#include <Library/KtiApi.h>
#include <Library/RasRcDataLib.h>
#include <Library/SysHostPointerLib.h>
#include <MemDecodeRas.h>
#include <MemDecode.h>
#include <Library/MemRcLib.h>
#include <Include/MemMapHost.h>

/**
  Check if a IMC is capable for mirroring by checking if DIMMs populated in all of its
  channels is good for mirroring mode.

  @param[in] Host             Pointer to sysHost.
  @param[in] MemMapData       Pointer to the memory map private data structure.
  @param[in] Socket           Socket number.

  @retval RAS_IS_CAPABLE
  @retval RAS_NOT_CAPABLE

**/
UINT8
CheckMirrorPopulation (
  IN PSYSHOST          Host,
  IN MEMORY_MAP_DATA   *MemMapData,
  IN UINT8             Socket
  )
{
  MEMORY_MAP_HOST *MemMapHost;
  UINT8           Capable = RAS_NOT_CAPABLE;
  UINT8           ChWay;
  UINT8           ChInterBitmap = 0;
  UINT8           SocketImcPresent;
  UINT8           SadIndex;
  UINT8           BeginSadIndex;
  UINT8           EndSadIndex;
  UINT8           ClusterId;
  UINT8           Imc;
  UINT8           MaxImc;
  SAD_TABLE       *SadTablePtr;

  MaxImc = GetMaxImc ();
  MemMapHost = GetMemMapHostPointer ();
  SadTablePtr = Host->var.mem.socket[Socket].SAD;

  for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
    BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
    EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
    if (SadTablePtr[BeginSadIndex].Enable == 0) {
      break;
    }
    for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
      if (!SadTablePtr[SadIndex].Enable) {
        break;
      }

      // Determine which IMC channel bitmap to use
      Imc = GetMcIndexFromBitmap (SadTablePtr[SadIndex].imcInterBitmap);

      if (SadTablePtr[SadIndex].type == MemType1lmDdr) {
        ChWay = SadTablePtr[SadIndex].NmChWays;
      } else {
        continue; //Do not check the SADs that are not 1LM for mirror eligibility
      }

      switch (ChWay) {
      case ONE_WAY: // channel 0; 1 chway
        Capable = RAS_NOT_CAPABLE;
        return Capable;
        break;
      case THREE_WAY: // channel 2, channel 1, channel 0; 3 chway
        for (Imc = 0; Imc < MaxImc; Imc++) {
          SocketImcPresent = MemMapHost->SocketInfo[Socket].SadIntList[SadIndex][(Socket * MaxImc) + Imc];

          if (SocketImcPresent) {
            if ((CheckMirrorPopulation2 (Host, Socket, Imc, 0, 1) == RAS_IS_CAPABLE) &&
              (CheckMirrorPopulation2 (Host, Socket, Imc, 1, 2) == RAS_IS_CAPABLE)) {
              Capable = RAS_IS_CAPABLE;
            }
          }
        } // Imc loop
        break;
      case TWO_WAY:
        for (Imc = 0; Imc < MaxImc; Imc++) {
          SocketImcPresent = MemMapHost->SocketInfo[Socket].SadIntList[SadIndex][(Socket * MaxImc) + Imc];

          if (SocketImcPresent == 0) {
            continue;
          }
          if (SadTablePtr[SadIndex].type == MemType1lmDdr) {
            ChInterBitmap = SadTablePtr[SadIndex].channelInterBitmap[Imc];
          }
          switch (ChInterBitmap) {
          case BITMAP_CH0_CH1: // channel 1, channel 0; 2 chway
            Capable = CheckMirrorPopulation2 (Host, Socket, Imc, 0, 1);
            break;

          case BITMAP_CH0_CH2: // channel 2, channel 0; 2 chway
            Capable = CheckMirrorPopulation2 (Host, Socket, Imc, 0, 2);
            break;

          case BITMAP_CH1_CH2: // channel 2, channel 1; 2 chway
            Capable = CheckMirrorPopulation2 (Host, Socket, Imc, 1, 2);
            break;

          default:
            Capable = RAS_NOT_CAPABLE;
            break;
          }
          //Break out of Imc loop even if one Imc fails capability check
          if (Capable == RAS_NOT_CAPABLE)
            break;
        } // Imc loop
        break;
      case 0: // empty IMC
        Capable = RAS_SKIP_CHANNEL;
        break;
      }

      //Break out of SAD loop even if one SAD fails capability check
      if (Capable == RAS_NOT_CAPABLE) {
        break;
      }
    } // SadIndex
  } // ClusterId

  return Capable;
}

/**
  Adjust memory address map on all the other sockets when mirroring is enabled.

  Given a socket and a SAD adjustment that was made on it to support mirroring,
  this function adjusts the SAD and remote SAD rules on other sockets, as well
  as the remote SAD rules for the given socket, to match the changes.

  @param[in, out] Host        Pointer to sysHost to be updated.
  @param[in, out] MemMapData  Pointer to memory map private data structure
  @param[in]      SocketsDone Bitmap of sockets on which memory was adjusted.
  @param[in]      Limit       Limit of the SAD that was adjusted.
  @param[in]      MirrorSize  Size of the mirror adjustment.
**/
VOID
AdjustMemAddrMapRemote (
  IN OUT  PSYSHOST        Host,
  IN OUT  MEMORY_MAP_DATA *MemMapData,
  IN      UINT32          SocketsDone,
  IN      UINT32          Limit,
  IN      UINT32          MirrorSize
  )
{
  UINT8           SocketIndex;
  UINT8           SadIndex;
  BOOLEAN         PrevSadAdjusted;
  SAD_TABLE       *SadTablePtr;
  RSAD_TABLE      *RemoteSadTablePtr;
  UINT32          NewBase;
  UINT8           BeginSadIndex;
  UINT8           EndSadIndex;
  UINT8           ClusterId;

  PrevSadAdjusted = FALSE;

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "<AdjustMemAddrMapRemote>\n"
    );

  //
  // For all the sockets in the system
  //
  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    if (Host->nvram.mem.socket[SocketIndex].enabled == 0) {
      continue;
    }

    SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;
    RemoteSadTablePtr = MemMapData->MemMapSocket[SocketIndex].RSad;

    //
    // Adjust remote SAD rules for every socket, including the one on which
    // mirroring was done.
    //
    for (SadIndex = 0; SadIndex < MemMapData->MaxRemoteSadRules; SadIndex++) {
      if (!RemoteSadTablePtr[SadIndex].Enable) {
        break;
      }

      if (RemoteSadTablePtr[SadIndex].Limit >= Limit) {
        if (RemoteSadTablePtr[SadIndex].Limit == SAD_4GB) {
          RcDebugPrintWithDevice (SDBG_MAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "RSAD: %d  Old Limit: 0x%04x  (NOT ADJUSTING FOR 4GB)\n",
            SadIndex,
            RemoteSadTablePtr[SadIndex].Limit
            );
        } else {
          RcDebugPrintWithDevice (SDBG_MAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "RSAD: %d  Old Limit: 0x%04x  New Limit: 0x%04x\n",
            SadIndex,
            RemoteSadTablePtr[SadIndex].Limit,
            RemoteSadTablePtr[SadIndex].Limit - MirrorSize
            );

          //
          // Only adjust the base address of RSAD rules above the limit
          //
          if (RemoteSadTablePtr[SadIndex].Base >= Limit) {
            //
            // RSADs above the limit need to shift down to account
            // for the updated RSAD limits of the rules lower in
            // the address space.
            //
            NewBase = RemoteSadTablePtr[SadIndex].Base - MirrorSize;
          } else {
            //
            // RSADs below the limit do not need to shift down since
            // the limits of the rules lower in the address space
            // are not changed.
            //
            NewBase = RemoteSadTablePtr[SadIndex].Base;
          }
          RcDebugPrintWithDevice (SDBG_MAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "RSAD: %d  Old Base: 0x%04x  New Base: 0x%04x\n",
            SadIndex,
            RemoteSadTablePtr[SadIndex].Base,
            NewBase
            );
          RemoteSadTablePtr[SadIndex].Limit -= MirrorSize;
          RemoteSadTablePtr[SadIndex].Base   = NewBase;
        }
      }
    }

    //
    // Skip the rest for the socket on which the mirroring was already done.
    //
    if ((SocketsDone & (BIT0 << SocketIndex)) != 0) {
      continue;
    }

    //
    // Adjust general purpose SAD rules for other sockets.
    //
    for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
      BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
      EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);

      if (SadTablePtr[BeginSadIndex].Enable == 0) {
        continue;
      }

      for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
        if (!SadTablePtr[SadIndex].Enable) {
          break;
        }

        //
        // Size of NXM SAD cannot change, it can only move.
        //
        if (SadTablePtr[SadIndex].type == MemTypeNxm) {
          if (!PrevSadAdjusted) {
            continue;
          }
        }

        if (SadTablePtr[SadIndex].Limit >= Limit) {
          if (SadTablePtr[SadIndex].Limit == SAD_4GB) {
            RcDebugPrintWithDevice (SDBG_MAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              " SAD: %d  Old Limit: 0x%04x  (NOT ADJUSTING FOR 4GB)\n",
              SadIndex,
              SadTablePtr[SadIndex].Limit
              );
            PrevSadAdjusted = FALSE;
          } else {
            RcDebugPrintWithDevice (SDBG_MAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              " SAD: %d  Old Limit: 0x%04x  New Limit: 0x%04x\n",
              SadIndex,
              SadTablePtr[SadIndex].Limit,
              SadTablePtr[SadIndex].Limit - MirrorSize
              );

            //
            // Only adjust the base address of SAD rules above the limit
            // Do not adjust base for SAD rule following 4GB SAD since 4GB limit should not change.
            //
            if ((SadTablePtr[SadIndex].Base >= Limit) && (SadTablePtr[SadIndex].Base != SAD_4GB)) {
              //
              // RSADs above the limit need to shift down to account
              // for the updated RSAD limits of the rules lower in
              // the address space.
              //
              NewBase = SadTablePtr[SadIndex].Base - MirrorSize;
            } else {
              //
              // RSADs below the limit do not need to shift down since
              // the limits of the rules lower in the address space
              // are not changed.
              //
              NewBase = SadTablePtr[SadIndex].Base;
            }
            RcDebugPrintWithDevice (SDBG_MAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              " SAD: %d  Old Base: 0x%04x  New Base: 0x%04x\n",
              SadIndex,
              SadTablePtr[SadIndex].Base,
              NewBase
              );
            SadTablePtr[SadIndex].Limit -= MirrorSize;
            SadTablePtr[SadIndex].Base   = NewBase;
            PrevSadAdjusted = TRUE;
          }
        }
      } // SadIndex
    } // ClusterId
  } // SocketIndex

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "</AdjustMemAddrMapRemote>\n"
    );
} // AdjustMemAddrMapRemote

/**
  Shift SADs for insert.

  This is a helper for InsertUpdateSad which copies SADs from a given index up
  by one index, adjusting each base and limit by the partial mirror size.

  @param[in]  Host         - Pointer to sysHost
  @param[in]  MemMapData   - Pointer to memory map private data struture
  @param[in]  MemMapHost   - Pointer to memory map host structure
  @param[in]  SocketIndex  - Socket number
  @param[in]  SadIndex     - SAD index
  @param[in]  PMirrorSize  - Partial mirror size to adjust each SAD by
**/
VOID
ShiftSadsForInsert (
  IN  SYSHOST         *Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  MEMORY_MAP_HOST *MemMapHost,
  IN  UINT8           SocketIndex,
  IN  UINT8           SadIndex,
  IN  UINT32          PMirrorSize
  )
{
  SAD_TABLE       *SadTablePtr;
  INTN            SadShiftIndex;
  UINT8           Package;

  SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;

  //
  // Locate last enabled SAD and move them by one position to insert a SAD
  //
  for (SadShiftIndex = MemMapData->MaxSadRules - 2; SadShiftIndex >= SadIndex; SadShiftIndex--) {
    if (SadTablePtr[SadShiftIndex].Enable == 0) {
      continue;
    }

    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "\nCopying SAD %d to SAD %d\n", SadShiftIndex, SadShiftIndex + 1);
    SadTablePtr[SadShiftIndex + 1] = SadTablePtr[SadShiftIndex];
    SadTablePtr[SadShiftIndex + 1].Limit -= PMirrorSize;
    SadTablePtr[SadShiftIndex + 1].Base -= PMirrorSize;

    //
    // Adjust SAD Interleave list
    //
    for (Package = 0; Package < MC_MAX_NODE; Package++) {
      MemMapHost->SocketInfo[SocketIndex].SadIntList[SadShiftIndex + 1][Package] =
        MemMapHost->SocketInfo[SocketIndex].SadIntList[SadShiftIndex][Package];
    }
  }
}

/**

  This routine inserts new SAD for partial mirroring.
  The insertion happens at location SadIndex+1

  @param Host         - Pointer to sysHost
  @param MemMapData   - Pointer to memory map private data struture
  @param SocketIndex  - Socket number
  @param SadIndex     - IMC number

**/
UINT32
InsertUpdateSad (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData,
  UINT8           SocketIndex,
  UINT8           SadIndex,
  UINT32          PMirrorSize,
  UINT32          MemHoleSize
  )
{
  MEMORY_MAP_HOST *MemMapHost;
  SAD_TABLE       *SadTablePtr;
  UINT32          Base;
  UINT32          Limit;
  UINT32          Status = SUCCESS;
  UINT32          SocketsDone;
  UINT8           RemoteSocketIndex;

  MemMapHost = GetMemMapHostPointer ();
  SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;

  //
  // Cannot insert if we have reached last DRAM rule
  //
  if (SadTablePtr[MemMapData->MaxSadRules - 1].Enable == 1) {
    return ERROR_RESOURCE_CALCULATION_FAILURE;
  }

  Base = SadTablePtr[SadIndex].Base;
  Limit = SadTablePtr[SadIndex].Limit;

  //
  // *********************************************************************************
  // *              SAD TABLE MODIFICATION FOR ADDRESS RANGE MIRRORING               *
  //      ---------------------
  //
  //                                            ---------------------
  //
  //                                                  SAD 3
  //             SAD 2                           New Base, New Limit
  //      ---------------------                  Same size as Old SAD 2
  //                                            ---------------------
  //                                                  SAD 2
  //                              ----------->  New Base, New Limit reduced
  //                                            size from Old SAD 1
  //                                            ---------------------
  //             SAD 1                          INSERT MIRROR SAD 1 (borrowed memory from SAD 2 for mirror)
  //      ---------------------                 ---------------------
  //                                                  SAD 0
  //             SAD 0                          Same Base, Same Limit
  //      ---------------------                 ---------------------
  // *********************************************************************************
  //
  ShiftSadsForInsert (Host, MemMapData, MemMapHost, SocketIndex, SadIndex, PMirrorSize);

  //
  // After inserting SAD in desired location, set the limit of Inserted SAD and
  // base of SAD from which memory is borrowed
  //
  SadTablePtr[SadIndex].Limit    = (SadTablePtr[SadIndex].Base) + PMirrorSize;
  SadTablePtr[SadIndex + 1].Base = (SadTablePtr[SadIndex].Base) + PMirrorSize;

  SadTablePtr[SadIndex].mirrored = 1;

  SocketsDone = (BIT0 << SocketIndex);

  for (RemoteSocketIndex = 0; RemoteSocketIndex < MAX_SOCKET; RemoteSocketIndex++) {
    if (Host->nvram.mem.socket[SocketIndex].enabled == 0) {
      continue;
    }

    if (RemoteSocketIndex == SocketIndex) {
      continue;
    }

    SadTablePtr = Host->var.mem.socket[RemoteSocketIndex].SAD;
    if (SadTablePtr[SadIndex].Enable == 0) {
      continue;
    }

    //
    // If this socket is interleaved in the same SAD, adjust it the same way.
    //
    if (SadTablePtr[SadIndex].Base == Base &&
        SadTablePtr[SadIndex].Limit == Limit) {
      ShiftSadsForInsert (Host, MemMapData, MemMapHost, RemoteSocketIndex, SadIndex, PMirrorSize);
      SadTablePtr[SadIndex].Limit    = (SadTablePtr[SadIndex].Base) + PMirrorSize;
      SadTablePtr[SadIndex + 1].Base = (SadTablePtr[SadIndex].Base) + PMirrorSize;
      SadTablePtr[SadIndex].mirrored = 1;
      SocketsDone |= (BIT0 << RemoteSocketIndex);
    }
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "\nAdjustMemAddrMap for SAD %d Limit\n", SadIndex);
  AdjustMemAddrMapRemote (Host, MemMapData, SocketsDone, Limit, PMirrorSize);

  return Status;
} //InsertUpdateSad

/**

  Adjust SAD limits for partial mirroring

  Routine figures out the adjustment required in SAD tables.

  @param Host             - Pointer to sysHost
  @param MemMapData       - Pointer to memory map private data structure.
  @param SocketIndex      - Socket number
  @param SadIndex         - Sad number above which limits have to be adjusted
  @param MaxSadIndex      - Max Sad number from SadIndex to which limits have to be adjusted
  @pmirrorsize            - Partial mirror size used to adjust the sad limits.s

  @retval SUCCESS

**/
UINT32
UpdateSad (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData,
  UINT8           SocketIndex,
  UINT8           SadIndex,
  UINT8           MaxSadIndex,
  UINT32          MirrorSize
  )
{
  INTN        SadUpdateIndex;
  UINT32      Limit;
  UINT32      Status = SUCCESS;
  SAD_TABLE   *SadTablePtr;
  BOOLEAN     PrevSadAdjusted = FALSE;
  UINT32      Base;
  UINT8       RemoteSocketIndex;
  UINT8       BeginSadIndex;
  UINT8       EndSadIndex;
  UINT8       ClusterId;

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "<UpdateSad>\n");
  SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;

  Limit = SadTablePtr[SadIndex].Limit;
  Base = SadTablePtr[SadIndex].Base;

  SadTablePtr[SadIndex].mirrored = 1;
  ClusterId = SadIndex / MemMapData->MaxSadRules;
  SadUpdateIndex = SadIndex;

  for (ClusterId; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
    BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
    EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
    if (SadUpdateIndex != SadIndex) {
      SadUpdateIndex = BeginSadIndex;
    }
    if (SadTablePtr[SadUpdateIndex].Enable == 0) {
      continue;
    }

    for (SadUpdateIndex; SadUpdateIndex < EndSadIndex; SadUpdateIndex++) {
      if (SadTablePtr[SadUpdateIndex].Enable == 0) {
        break;
      }
      //
      // Size of NXM SAD Cannot change it can only move.
      //
      if (SadTablePtr[SadUpdateIndex].Attr == SAD_NXM_ATTR) {
        if (!PrevSadAdjusted) {
          continue;
        }
      }

      if (SadTablePtr[SadUpdateIndex].Limit == SAD_4GB) {
        PrevSadAdjusted = FALSE;
        continue;
      }

      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "Before UpdateSad: Socket: %x,Reducing sad %d Base = %d Limit =%d by %d\n", SocketIndex, SadUpdateIndex, SadTablePtr[SadUpdateIndex].Base, SadTablePtr[SadUpdateIndex].Limit, MirrorSize );
      SadTablePtr[SadUpdateIndex].Limit -= MirrorSize;
      if (PrevSadAdjusted) {
        SadTablePtr[SadUpdateIndex].Base -= MirrorSize;
      }
      PrevSadAdjusted = TRUE;
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "After UpdateSad: Socket: %x,Reducing sad %d Base = %d Limit =%d\n", SocketIndex, SadUpdateIndex, SadTablePtr[SadUpdateIndex].Base, SadTablePtr[SadUpdateIndex].Limit);
    } // SadUpdateIndex loop
  } // ClusterId

  for (RemoteSocketIndex = 0; RemoteSocketIndex < MAX_SOCKET; RemoteSocketIndex++) {
    if (Host->nvram.mem.socket[SocketIndex].enabled == 0) {
      continue;
    }

    if (RemoteSocketIndex == SocketIndex) {
      continue;
    }

    SadTablePtr = Host->var.mem.socket[RemoteSocketIndex].SAD;
    if (SadTablePtr[SadIndex].Enable == 0) {
      continue;
    }

    //
    // If this socket is interleaved in as part of the SAD it should also be marked as mirrored
    //
    if (SadTablePtr[SadIndex].Base == Base &&
        SadTablePtr[SadIndex].Limit == Limit) {
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "\t Marking SadIndex:%d in Socket:%d as mirrored\n",SadIndex,RemoteSocketIndex);
      SadTablePtr[SadIndex].mirrored = 1;
    }
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "AdjustMemAddrMap Remote for SAD %d Limit\n", SadIndex);
  AdjustMemAddrMapRemote (Host, MemMapData, (BIT0 << SocketIndex), Limit, MirrorSize);

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "</UpdateSad>\n");
  return Status;
}

/**
Gets memory below 4GB

@param[in]   Host   Pointer to sysHost

@retval Tolm

**/
UINT32
GetMemBlocksBelow4GB (
  PSYSHOST  Host
  )
{
  UINT32     Tolm;
  UINT8      SocketIndex;
  UINT8      SadIndex;
  SAD_TABLE  *SadTablePtr;
  UINT32     TadLimit;
  UINT8      TadLimitAdjustment;
  UINT8      ChannelWays;
  UINT8      ImcWays;
  UINT8      TotalWays;

  Tolm = GetTolmAddress ();
  SadIndex = SAD0INDEX;
  TadLimit = 0;

  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    if (Host->nvram.mem.socket[SocketIndex].enabled == 0) {
      continue;
    }

    SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;
    ChannelWays = GetChannelWays (&SadTablePtr[SadIndex]);
    ImcWays = SadTablePtr[SadIndex].ways;

    TadLimitAdjustment = 0;
    TadLimit = SadTablePtr[SadIndex].Limit - GetLowGap ();

    if (SadTablePtr[SadIndex].Limit == SAD_4GB) { // Find the 4GB sad and its corresponding TAD to get correct value of Tolm
      if (ChannelWays == CH_3WAY) {
        TotalWays = ImcWays * ChannelWays;
        if (Tolm % TotalWays) {
          TadLimitAdjustment = TotalWays - (Tolm % TotalWays);
        }
      }
    } // if SAD_4GB

    // Adjust TAD limit as needed for memory hole
    TadLimit += TadLimitAdjustment;
    break;
  } // Socket loop
  return TadLimit;
} // GetMemBlocksBelow4GB


/**

Adjust memory address map for mirroring SAD0

Routine figures out the adjustment required in SAD, SAD2TAD, TAD tables
when we enable mirroring the 4GB SAD0. This is different on account of
having the memory hole in SAD0

@param Host       - Pointer to sysHost
@param MemMapData - Pointer to memory map private data structure

@retval SUCCESS

**/
VOID
AdjustMemAddrMapForSad0Mirror (
  PSYSHOST        Host,
  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8      SocketIndex;
  UINT32     PMirrorSize;
  UINT8      Status;
  SAD_TABLE  *SadTablePtr;


  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {

    SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;
    if (SadTablePtr[0].Enable == 0) {
      continue;
    } // skip empty sockets

    if (SadTablePtr[0].Limit == SAD_4GB) {
      Status = CheckSadForMirroring (Host, SadTablePtr, SAD0INDEX);
      if (Status == SAD_NOT_ELIGIBLE_MIRROR) {
        continue;
      }

      PMirrorSize = GetMemBlocksBelow4GB (Host);
      UpdateSad (Host, MemMapData, SocketIndex, SAD0INDEX, MAX_SAD_RULES, PMirrorSize);
    }
  } // Socket Loop
  return;
} //AdjustMemAddrMapForSad0Mirror

/**

Adjust memory address map for UEFI

@param [in] Host      Pointer to sysHost
@param [in] MemMapDat Pointer to memory map private data structure

@retval SUCCESS

**/
UINT32
AdjustMemAddrMapUEFI (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8       SocketIndex;
  UINT8       SadIndex;
  UINT8       BeginSadIndex;
  UINT8       EndSadIndex;
  UINT8       ClusterId;
  UINT32      MemHoleSize;
  UINT32      PMirrorSize;
  UINT32      CurrentSadSize;
  UINT32      TotalMemoryAvailable;
  UINT32      BlocksBelow4G = 0;
  UINT8       SadCheckReturnStatus;
  SAD_TABLE   *SadTablePtr;
  UINT32      Status = SUCCESS;
  BOOLEAN     PmirrorCreated = FALSE;
  UINT8       ch;
  UINT8       ChannelWays;
  UINT8       ImcWays;
  UINT8       TotalSadWays;
  SYS_SETUP   *Setup;
  EFI_STATUS  EfiStatus;
  UINT32      Numerator;
  UINT32      Denominator;
  UINT8       MaxChDdr;

  Setup = GetSysSetupPointer ();

  MemHoleSize = (FOUR_GB_MEM - GetTolmAddress ());

  //The percentage value was entered from OS instead of setup option
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "UEFI Variable for address range mirroring found \n");

  EfiStatus = GetMirrorRatio (
    Host->var.mem.socket[0].SAD[0].type,
    &Numerator,
    &Denominator
    );
  //
  // Fail if ratio is greater than 50%.
  //
  if (EFI_ERROR (EfiStatus) || (Numerator * 2) > Denominator) {
    return MIRROR_STATUS_INVALID_REQUEST;
  }

  MaxChDdr = GetMaxChDdr ();
  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    TotalMemoryAvailable = 0;
    //several nvram structures that have info on dimms in each channel DDR4 or NGN
    // use structure like struct ddrChannel to calculate total memory memhost.h  also make sure to use the volatile size and not persistent size

    if (Host->nvram.mem.socket[SocketIndex].enabled) {
      for (ch = 0; ch < MaxChDdr; ch++) {
        if (Host->nvram.mem.socket[SocketIndex].channelList[ch].enabled) {
          TotalMemoryAvailable += Host->var.mem.socket[SocketIndex].channelList[ch].memSize;
          RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                         "Socket 0x%x ch:0x%x memSize:0x%x volSize: 0x%x\n",
              SocketIndex,
              ch,
              Host->var.mem.socket[SocketIndex].channelList[ch].memSize,
              Host->var.mem.socket[SocketIndex].channelList[ch].volSize);
        }
      }
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "Socket %x - TotalMemoryAvailable : 0x%x\n", SocketIndex, TotalMemoryAvailable);

      SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;
      if (SadTablePtr[0].Limit == SAD_4GB && SadTablePtr[0].Enable == 1 && SadTablePtr[0].local == 1) {
        BlocksBelow4G = GetMemBlocksBelow4GB(Host);
        RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "Socket %x -BlocksBelow4G : 0x%x\n", SocketIndex, BlocksBelow4G);
        TotalMemoryAvailable -= BlocksBelow4G;//do not count the amount of memory below 4GB
        if (IsLowMemoryMirrorEnabled (Host->var.mem.socket[0].SAD[0].type)) {
          TotalMemoryAvailable -= BlocksBelow4G; // Take twice the size of Tolm because it need that amount to mirror.
        }
        RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "Socket %x - TotalMemoryAvailable after subtraction of Tolm : 0x%x\n", SocketIndex, TotalMemoryAvailable);
      }

      // Find the partial mirror size from percentage given
      PMirrorSize = (TotalMemoryAvailable * Numerator) / Denominator;
      PMirrorSize += (((TotalMemoryAvailable * Numerator) % Denominator) != 0); // Extra block to account for remainder
      if (PMirrorSize > (TotalMemoryAvailable / 2)) {
        PMirrorSize = (TotalMemoryAvailable / 2); // make sure it doesn't exceed 50% due to the ceiling.
      }

      SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;
      for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
        BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
        EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
        if (SadTablePtr[BeginSadIndex].Enable == 0) {
          break;
        }
        for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {

          //4GB SAD has memory hole and is mirrored separately
          if (SadTablePtr[SadIndex].Limit == SAD_4GB) {
            continue;
          } else {
            CurrentSadSize = SadTablePtr[SadIndex].Limit - SadTablePtr[SadIndex].Base;

            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "SAD %d\n", SadIndex);
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Requested partial mirror size 0x%x \n", PMirrorSize);
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Current SAD size 0x%x\n", CurrentSadSize);

            SadCheckReturnStatus = CheckSadForMirroring (Host, SadTablePtr, SadIndex);
            if (SadCheckReturnStatus == SAD_NOT_ENABLED) {
              RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "SAD_NOT_ENABLED\n");
              break;
            } else if (SadCheckReturnStatus == SAD_NOT_ELIGIBLE_MIRROR) {
              RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "SAD_NOT_ELIGIBLE_MIRROR\n");
              continue;
            }

            //Following calculation of TotalSadWays is required to round off the partial mirror size
            //to a value that is exactly divisible by number of channels interleaved in that SAD
            ChannelWays = 0;
            ImcWays = SadTablePtr[SadIndex].ways;

            switch (SadTablePtr[SadIndex].type) {
            case MemType1lmDdr:
              ChannelWays = SadTablePtr[SadIndex].NmChWays;
              break;
            default:
              break;
            }

            TotalSadWays = ImcWays * ChannelWays;

            if (CurrentSadSize == (PMirrorSize * 2)) {
              RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "SAD %d Size == (PMirrorSize * 2)\n", SadIndex);
              Status = UpdateSad (Host, MemMapData, SocketIndex, SadIndex, EndSadIndex, PMirrorSize);
              PMirrorSize = 0;
            } else if (CurrentSadSize > (PMirrorSize * 2)) {
              RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "SAD %d Size > (PMirrorSize * 2) \n", SadIndex);
              if (SadIndex < (MemMapData->MaxSadRules - 1)) {

                //make size of Partial mirror a multiple of the interleave ways to divide memory equally between the channels
                if (TotalSadWays > 1) {
                  //Limit of SAD should be GIGA BIT Aligned for partial mirorring
                  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "SNC GB Alignment required!\n");
                  PMirrorSize = PMirrorSize & GB_BOUNDARY_ALIGN;
                  if (!(TotalSadWays % 3)) {
                    //The Size of the SAD should always be a multiple of number of channels interleaved.
                    while (PMirrorSize % TotalSadWays) {
                      PMirrorSize = PMirrorSize - 0x10;
                      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "PMirrorSize:%d \n", PMirrorSize);

                      if (PMirrorSize < 0x10) { // Cannot create a mirror region that is GB aligned
                        RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "No Mirror!! Cannot create mirror region that is GB Aligned.\n");
                        PMirrorSize = 0;
                        break;
                      }
                    }
                  }
                }

                Status = InsertUpdateSad (Host, MemMapData, SocketIndex, SadIndex, PMirrorSize, MemHoleSize);
                PMirrorSize = 0;
              }
              //break;
            } else if (CurrentSadSize < (PMirrorSize * 2)) {
              if(SadIndex < (MemMapData->MaxSadRules - 1)) {
                RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "SAD %d Size < (PMirrorSize*2) \n", SadIndex);
                PMirrorSize -= (CurrentSadSize / 2); //only part of requested pmirror is done
                Status = UpdateSad (Host, MemMapData, SocketIndex, SadIndex, EndSadIndex, (CurrentSadSize / 2));
              }
            }
          }

          if (PMirrorSize == 0) {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Mirror done on socket %d\n", SocketIndex);
            PmirrorCreated = TRUE;
            break;
          }
        } // SAD loop
      } // ClusterId
    } // SocketEnabled?
  } //Socket loop

  return Status;
}

/**
  Calculate if enough memory is available to create
  the mirror region completely.

  @param[in] Host             Pointer to sysHost.
  @param[in] MemMapData       Pointer to the memory map private data structure.
  @param[in] MirrorSize       Mirror size to verify.

  @retval SUCCESS

**/
BOOLEAN
CalculateMirrorMem (
  IN  PSYSHOST          Host,
  IN  MEMORY_MAP_DATA   *MemMapData,
  IN  UINT32            MirrorSize
  )
{
  UINT8     SocketIndex;
  UINT8     SadIndex;
  UINT8     BeginSadIndex;
  UINT8     EndSadIndex;
  UINT8     ClusterId;
  UINT32    MemoryAvailable;
  BOOLEAN   MemAvailable = FALSE;
  SAD_TABLE *SadTablePtr;

  MemoryAvailable = 0;
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\n MirrorSize :%d\n", MirrorSize);
  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    if (Host->nvram.mem.socket[SocketIndex].enabled == 0) {
      continue;
    }

    SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;
    for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
      BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
      EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
      if (SadTablePtr[BeginSadIndex].Enable == 0) {
        break;
      }
      for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
        if (SadTablePtr[SadIndex].Enable == 0) {
          continue;
        }
        if (SadTablePtr[SadIndex].Limit == SAD_4GB) {
          continue; //4gb SAD is mirrored separately
        }

        if (CheckSadForMirroring (Host, SadTablePtr, SadIndex) == SAD_ELIGIBLE_MIRROR) {
          if (SadIndex == 0) {
            MemoryAvailable += (SadTablePtr[SadIndex].Limit);
          } else {
            MemoryAvailable += (SadTablePtr[SadIndex].Limit - SadTablePtr[SadIndex].Base);
          }
        }
        RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "\n Socket:%d Sad :%d Mem Avl so far:%d\n", SocketIndex, SadIndex, MemoryAvailable);
        if (MemoryAvailable >= (MirrorSize * 2)) {
          MemAvailable = TRUE;
          break;
        }
      } // SadIndex
    } // ClusterId
    if (MemAvailable) {
      break;
    }
  } // SocketIndex
  if (MemAvailable) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\n Enough Memory available for Mirroring \n");
    return TRUE;
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "\n Not Enough memory for Mirroring \n");
    return FALSE;
  }
}

/**

Adjust memory address map for partial mirroring

Routine figures out the adjustment required in SAD, SAD2TAD, TAD tables for various partial mirror regions and sizes.

@param Host       - Pointer to sysHost
@param MemMapData - Pointer to memory map private data structure

@retval SUCCESS

**/
UINT32
AdjustMemAddrMapForPMirror (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData
  )
{
  UINT8       SocketIndex;
  UINT8       SadIndex;
  UINT8       BeginSadIndex;
  UINT8       EndSadIndex;
  UINT8       ClusterId;
  UINT32      count;
  UINT32      MemHoleSize;
  UINT32      PMirrorCount;
  UINT32      PMirrorSizes[MAX_PARTIAL_MIRROR];
  UINT32      PMirrorSize;
  UINT32      CurrentSadSize;
  UINT8       SadCheckReturnStatus;
  SAD_TABLE   *SadTablePtr;
  UINT32      Status = SUCCESS;
  UINT8       ChannelWays;
  UINT8       ImcWays;
  UINT8       TotalSadWays;
  BOOLEAN     PmirrorCreated = FALSE;
  SYS_SETUP   *Setup;
  EFI_STATUS  EfiStatus;

  Setup = GetSysSetupPointer ();

  //
  // Calculate the size of PCIe MMIO hole just below 4GB
  //
  MemHoleSize = (FOUR_GB_MEM - GetTolmAddress ());

  // Cannot create mirror regions if rank sparing is enabled
  if (Host->nvram.mem.RASmode & RK_SPARE) {
    Status = MIRROR_STATUS_UNSUPPORTED_CONFIG; //Unsopported_mirror_config
    return Status;
  }
  //
  //Partial mirror SAD0 is enabled - mirror the region below 4gb
  //
  if (IsLowMemoryMirrorEnabled (Host->var.mem.socket[0].SAD[0].type)) {
    AdjustMemAddrMapForSad0Mirror (Host, MemMapData);
  }

  //The percentage value was entered from OS instead of setup option
  if (IsMirrorByRatioEnabled (Host->var.mem.socket[0].SAD[0].type)) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "UEFI Variable for address range mirroring found \n");

    Status = AdjustMemAddrMapUEFI (Host, MemMapData);
    return Status;

  } else if (IsMirrorBySizeEnabled (Host->var.mem.socket[0].SAD[0].type)) {
    //if address range mirror is enabled in setup
    PMirrorCount = MAX_PARTIAL_MIRROR;
    EfiStatus = GetMirrorSizes (
      Host->var.mem.socket[0].SAD[0].type,
      &PMirrorCount,
      PMirrorSizes
      );
    if (EFI_ERROR (EfiStatus)) {
      PMirrorSizes[0] = 0;
    }
    //Loop for number of regions that can be created
    for (count = 0; count < PMirrorCount; count++) {
      if (PMirrorSizes[count] == 0) {
      break;
      }
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "Partial mirror size : %d\n", PMirrorSizes[count]);
      PMirrorSize = 0;
      for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
        SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;
        if (SadTablePtr[0].Enable == 0) {
          break;
        } // skip empty sockets

        RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "\nAt Socket %d\n", SocketIndex);

        for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
          BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
          EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
          if (SadTablePtr[BeginSadIndex].Enable == 0) {
            break;
          }
          for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "At Sad Index : %d\n", SadIndex);

            //Non Existant Memory cannot be considered for Mirroring related adjustments
            if (SadTablePtr[SadIndex].Attr == SAD_NXM_ATTR) {
              continue;
            }

            //Following calculation of TotalSadWays is required to round off the partial mirror size
            //to a value that is exactly divisible by number of channels interleaved in that SAD
            ChannelWays = 0;
            ImcWays = SadTablePtr[SadIndex].ways;

            switch (SadTablePtr[SadIndex].type) {
            case MemType1lmDdr:
              ChannelWays = SadTablePtr[SadIndex].NmChWays;
              break;
            default:
              //Automated add of default case. Please review.
              break;
            }

            TotalSadWays = ImcWays*ChannelWays;

            //Initialize PMirrorSize
            if (PMirrorSize == 0) {
              PMirrorSize = PMirrorSizes[count];
            }

            //
            // skip adjustment for SAD with 2G-4G memory hole.
            //4 << 4 is actually (4 << 30) >> 26. i.e., 4GB >> 26.
            //
            if (SadTablePtr[SadIndex].Limit == SAD_4GB) {
              RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "AdjustMemAddrMapForPMirror: Ignore 4GB Sad %d\n", SadIndex);
              continue;
            }

            SadCheckReturnStatus = CheckSadForMirroring (Host, SadTablePtr, SadIndex);
            if (SadCheckReturnStatus == SAD_NOT_ENABLED) {
              break;
            } else if (SadCheckReturnStatus == SAD_NOT_ELIGIBLE_MIRROR) {
              continue;
            }

            CurrentSadSize = SadTablePtr[SadIndex].Limit - SadTablePtr[SadIndex].Base;

            RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "Current SAD size %d\n", CurrentSadSize);

            if (CurrentSadSize == (PMirrorSize * 2)) {
              RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "SAD %d Size == (PMirrorSize*2)\n", SadIndex);
              Status = UpdateSad (Host, MemMapData, SocketIndex, SadIndex, EndSadIndex, PMirrorSize);

              PMirrorSize = 0;
              break;
            } else if (CurrentSadSize > (PMirrorSize * 2)) {
              RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "SAD %d Size > (PMirrorSize*2) \n", SadIndex);
              if (SadIndex < (MemMapData->MaxSadRules - 1)) {

                //make size of Partial mirror a multiple of the interleave ways to divide memory equally between the channels
                if (TotalSadWays > 1) {
                  PMirrorSize = PMirrorSize & GB_BOUNDARY_ALIGN;
                  if (!(TotalSadWays % 3)) { // SAD is 3way or 6way then also ensure the pmirror size is muliple of chways
                    //The Size of the SAD should always be a multiple of number of channels interleaved.
                    while (PMirrorSize % TotalSadWays) {
                      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "PMirrorSize:%d, TotalSadWays:%d\n", PMirrorSize, TotalSadWays);
                      PMirrorSize = PMirrorSize - 0x10;
                      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                        "PMirrorSize:%d \n", PMirrorSize);

                      if (PMirrorSize < 0x10) { // Cannot create a mirror region that is GB aligned
                        PMirrorSize = 0;
                        break;
                      }
                    }
                  }
                }

                Status = InsertUpdateSad (Host, MemMapData, SocketIndex, SadIndex, PMirrorSize, MemHoleSize);

                PMirrorSize = 0;
              }
              break;
            } else if (CurrentSadSize < (PMirrorSize * 2)) {
              if (SadIndex < (MemMapData->MaxSadRules - 1)) {
                RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "SAD %d Size < (PMirrorSize*2) \n", SadIndex);

                if (CalculateMirrorMem (Host, MemMapData, PMirrorSize)) { //Check if enough memory is available to create mirror
                  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "Enough memory for mirror\n");
                  PMirrorSize -= (CurrentSadSize / 2); //only part of requested pmirror is done
                  if (CurrentSadSize % 2) {
                    Status = UpdateSad (Host, MemMapData, SocketIndex, SadIndex, EndSadIndex, ((CurrentSadSize / 2) + 1));
                  } else {
                    Status = UpdateSad (Host, MemMapData, SocketIndex, SadIndex, EndSadIndex, (CurrentSadSize / 2));
                  }

                  continue;
                } else {
                  Status = SUCCESS; //Identify the right way to handle this in memory mapping code
                }
              }
              break;
            }
          } // for (SadIndex)
        } // ClusterId
        //
        /* Break from the socket loop if requested partial mirror
        was created in current socket. If not continue to next socket*/
        //
        if (PMirrorSize == 0) {
          PmirrorCreated = TRUE;
          Status = SUCCESS;
          break;
        }
      } // for (SocketIndex)
    } // Number of partial mirror to be created
  }//Setup enabled address range mirror
  if(Status != EFI_SUCCESS) {
    Status = MIRROR_STATUS_UNSUPPORTED_CONFIG; //UNSUPPORTED_CONFIG
  }
  return Status;
}

/**

The function alligns the sad on GB boundary when SNC is enabled

@param Host       - Pointer to sysHost
@param MemMapData - Pointer to memory map private data structure

@retval None

**/
VOID
DoSncGBAlignmentForMirror (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  OUT UINT8           SizeAdjustmentForSnc[MAX_SOCKET][MAX_DRAM_CLUSTERS * MAX_SAD_RULES]
  )
{
  UINT8       SocketIndex;
  UINT8       SadIndex;
  UINT8       BeginSadIndex;
  UINT8       EndSadIndex;
  UINT8       ClusterId;
  SAD_TABLE   *SadTablePtr;
  UINT32      GbAlignSize;
  UINT32      NewSadLimit;
  UINT64      PrevHigherSadLimit = 0;
  UINT8       ChannelWays;
  UINT8       ImcWays;
  UINT8       TotalSadWays;
  SYS_SETUP   *Setup;

  Setup = GetSysSetupPointer ();

  if (!SncGbAlignRequired ()) { //determined that the SAD limites have to be GB aligned
    return;
  }

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "<DoSncGBAlignmentForMirror>\n");

  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    if (Host->nvram.mem.socket[SocketIndex].enabled == 0) {
      continue;
    }

    SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;

    for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
      BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
      EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);

      if (SadTablePtr[BeginSadIndex].Enable == 0) {
        break;
      }

      for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {

        //Break if Sad is not enabled
        if (SadTablePtr[SadIndex].Enable == 0) {
          break;
        }
        //Skip this SAD if the SAD is not local
        if (SadTablePtr[SadIndex].local == 0) {
          continue;
        }
        //Cannot Mirror Non Existant Memory
        if (SadTablePtr[SadIndex].Attr == SAD_NXM_ATTR) {
          continue;
        }
        // might be needed incase if mirroring is supported with diff memory socket sizes.
        if (((Setup->mem.options & NUMA_AWARE) == 00) && (SadTablePtr[SadIndex].Limit <= PrevHigherSadLimit)) {
          continue;
        }

        //Following calculation of TotalSadWays is required to round off the SAD size
        //to a value that is exactly divisible by number of channels interleaved in that SAD
        ChannelWays = 0;
        ImcWays = SadTablePtr[SadIndex].ways;

        switch (SadTablePtr[SadIndex].type) {
        case MemType1lmDdr:
          ChannelWays = SadTablePtr[SadIndex].NmChWays;
          break;
        default:
          break;
        }

        TotalSadWays = ImcWays * ChannelWays;

        switch (SadTablePtr[SadIndex].type) {
        case MemType1lmDdr:
          if (SadTablePtr[SadIndex].Limit == SAD_4GB) {
            continue;
          } // SAD_4GB
          NewSadLimit = SadTablePtr[SadIndex].Limit & GB_BOUNDARY_ALIGN;
          SizeAdjustmentForSnc[SocketIndex][SadIndex] = (UINT8)(SadTablePtr[SadIndex].Limit & ~GB_BOUNDARY_ALIGN);
          if (!(TotalSadWays % 3)) { //3chway or 6chway interleave
            if (SadIndex > 0) {
              while ((NewSadLimit - SadTablePtr[SadIndex - 1].Limit) % TotalSadWays) {
                // Each channel did not have an equal contribution, so try next GB boundary
                NewSadLimit = NewSadLimit - 0x10;
                SizeAdjustmentForSnc[SocketIndex][SadIndex] += 0x10;
              }
            }
          }
          GbAlignSize = SadTablePtr[SadIndex].Limit - NewSadLimit;
          UpdateSad (Host, MemMapData, SocketIndex, SadIndex, EndSadIndex, GbAlignSize);
          PrevHigherSadLimit = SadTablePtr[SadIndex].Limit;
          break;
        default:
          /*Mirroring from Setup can be requested only for 1LM memory
          PMEM/PMEM$ mirroring is dictated by platform config data in NGN DIMMs*/
          break;
        } // switch
      } // for (SadIndex)
    } // for (ClusterId)
  } // for (SocketIndex)
}

/**

  Adjust memory address map for mirroring. Routine figures out the adjustment
  required in SAD, SAD2TAD, TAD tables when mirroring or ARM is enabled

  @param Host      - Pointer to sysHost
  @param MemMapDat - Pointer to memory map private data structure

  @retval SUCCESS

**/
UINT32
AdjustMemAddrMapForFullMirror (
  IN  PSYSHOST        Host,
  IN  MEMORY_MAP_DATA *MemMapData,
  IN  UINT32          MemHoleSize,
  IN  UINT8           SizeAdjustmentForSnc[MAX_SOCKET][MAX_DRAM_CLUSTERS * MAX_SAD_RULES]
  )
{

  UINT8      SocketIndex;
  UINT8      SadIndex;
  UINT8      BeginSadIndex;
  UINT8      EndSadIndex;
  UINT8      ClusterId;
  UINT32     Status = SUCCESS;
  SAD_TABLE  *SadTablePtr;
  UINT32     MirrorSize;

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "<AdjustMemAddrMapForFullMirror>\n");
  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    if (Host->nvram.mem.socket[SocketIndex].enabled == 0) {
      continue;
    }

    SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;

    for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
      BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
      EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);

      if (SadTablePtr[BeginSadIndex].Enable == 0) {
        continue;
      }

      for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {

        //Break if Sad is not enabled
        if (SadTablePtr[SadIndex].Enable == 0) {
          break;
        }
        //Skip this SAD if the SAD is not local
        if (SadTablePtr[SadIndex].local == 0) {
          continue;
        }
        //Cannot Mirror Non Existant Memory
        if (SadTablePtr[SadIndex].Attr == SAD_NXM_ATTR) {
          continue;
        }

        if (SadTablePtr[SadIndex].mirrored == 1) {
          continue;
        }

        switch (SadTablePtr[SadIndex].type) {
        case MemType1lmDdr:
          //Special case when the 4GB SAD is not halved but we borrow memory from SAD above
          if (SadTablePtr[SadIndex].Limit == SAD_4GB) {
            MirrorSize = SAD_4GB - MemHoleSize;
            UpdateSad (Host, MemMapData, SocketIndex, SadIndex, EndSadIndex, MirrorSize);
            continue;
          } // SAD_4GB

          MirrorSize = (SadTablePtr[SadIndex].Limit - SadTablePtr[SadIndex].Base) / 2;
          UpdateSad (Host, MemMapData, SocketIndex, SadIndex, EndSadIndex, MirrorSize);
          break;
        default:
          break;
        }

      } // for (SadIndex)
    } // for (ClusterId)
  } // for (SocketIndex)
  DoSncGBAlignmentForMirror (Host, MemMapData, SizeAdjustmentForSnc);

  return Status;
}

/**

  Adjust memory address map for mirroring. Routine figures out the adjustment
  required in SAD, SAD2TAD, TAD tables when mirroring or ARM is enabled

  @param Host       - Pointer to sysHost
  @param MemMapData - Pointer to memory map private data structure

  @retval SUCCESS

**/
UINT32
AdjustMemAddrMapForMirror (
    PSYSHOST        Host,
    MEMORY_MAP_DATA *MemMapData,
    UINT8           SizeAdjustmentForSnc[MAX_SOCKET][MAX_DRAM_CLUSTERS * MAX_SAD_RULES]
  )
{
  UINT8           SocketIndex;
  UINT32          MemHoleSize;
  SAD_TABLE       *SADTablePtr;
  UINT32          Status = SUCCESS;
  SYS_SETUP       *Setup;
  RAS_RC_CONFIG   *RasRcConfig;
  MEM_TYPE        MemType;
  EFI_STATUS      EfiStatus;
  UINT32          Numerator;
  UINT32          Denominator;
  BOOLEAN         Full;

  Setup = GetSysSetupPointer ();

  // Calculate the size of PCIe MMIO hole just below 4GB
  MemHoleSize = (FOUR_GB_MEM - (UINT16)GetMemBlocksBelow4GB (Host));

  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "\nAdjustMemAddrMapForMirror: In the function\n");

  //Verify the socket populated has memory population for mirroring. The memory population
  //requirements are the same for both full and address based mirror

  for (SocketIndex = 0; SocketIndex < MAX_SOCKET; SocketIndex++) {
    if ((Host->nvram.mem.socket[SocketIndex].enabled == 0) || (Host->nvram.mem.socket[SocketIndex].maxDimmPop == 0)) {
      continue;
    }
    if (CheckMirrorPopulation (Host, MemMapData, SocketIndex) == RAS_NOT_CAPABLE) {
      RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "\nMirroring population not met\n");
      Host->nvram.mem.RASmode &= ~(CH_ALL_MIRROR);
      RasRcConfig = GetRasRcConfig ();
      ASSERT (RasRcConfig != NULL);
      if (RasRcConfig != NULL) {
        RasRcConfig->SupportedRasModes &= ~(CH_ALL_MIRROR);
      }
      Setup->mem.partialmirrorsts = MIRROR_STATUS_MIRROR_INCAPABLE;
      return Status;
    }

    if (Host->var.mem.volMemMode == VOL_MEM_MODE_2LM) {
      //
      //can turn off regardless of cache type
      //
      Host->nvram.mem.RASmode &= ~(CH_ALL_MIRROR);
      Setup->mem.partialmirrorsts = MIRROR_STATUS_MIRROR_INCAPABLE;
      return Status;
    }

    SADTablePtr = Host->var.mem.socket[SocketIndex].SAD;
    if (SADTablePtr[0].Enable == 0) {
      continue;
    } // skip empty sockets
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "\nAdjustMemAddrMapForMirror - Socket: %d \n", SocketIndex);
  } // For Socket Index

  MemType = Host->var.mem.socket[0].SAD[0].type;
  if (IsMirrorByRatioEnabled (MemType) ||
      IsMirrorBySizeEnabled (MemType)) {
    Full = FALSE;
    if (IsMirrorByRatioEnabled (MemType)) {
      EfiStatus = GetMirrorRatio (
        MemType,
        &Numerator,
        &Denominator
        );

      //
      // A ratio of 1/2 is full mirroring.
      //
      if (!EFI_ERROR (EfiStatus) && (Numerator * 2) == Denominator) {
        Full = TRUE;
      }
    }

    if (Full) {
      RcDebugPrintWithDevice (
        SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\tAdjustMemAddrMapForMirror: Full Mirror enabled\n"
        );
      Status = AdjustMemAddrMapForFullMirror (
                 Host,
                 MemMapData,
                 MemHoleSize,
                 SizeAdjustmentForSnc
                 );
    } else {
      RcDebugPrintWithDevice (
        SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "\tAdjustMemAddrMapForMirror: Address Range Mirror enabled\n"
        );
      Status = AdjustMemAddrMapForPMirror (Host, MemMapData);
    }

    Setup->mem.partialmirrorsts = (UINT8) Status;
  }

  return Status;
} // AdjustMemAddrMapForMirror

/**
  After mirroring, this call will adjust the memory size fields to accurately reflect the
  value of the reduced memory in system.

  @param[in,out] Host                  Pointer to sysHost.
  @param[in]     MemMapData            Pointer to the memory map private data structure.
  @param[in]     SizeAdjustmentForSnc  Memory size need to adjust for SNC.

  @retval SUCCESS

**/
UINT32
AdjustMemorySizeFieldsforMirror (
  IN OUT PSYSHOST          Host,
  IN     MEMORY_MAP_DATA   *MemMapData,
  IN     UINT8             SizeAdjustmentForSnc[MAX_SOCKET][MAX_DRAM_CLUSTERS * MAX_SAD_RULES]
  )
{
  UINT8     Imc;
  UINT8     SocketIndex;
  UINT8     SadIndex;
  UINT8     BeginSadIndex;
  UINT8     EndSadIndex;
  UINT8     ClusterId;
  UINT8     MaxImc;
  UINT32    Status;
  UINT16    CurrentSadSize;
  UINT16    CurrentSadInterleaves;
  UINT16    sadInterleaveSizePerMc;
  BOOLEAN   AdjustMemForSnc = FALSE;

  MEM_VAR_STRUCT *mem;
  SAD_TABLE      *SadTablePtr;

  Status = SUCCESS;
  mem    = &Host->var.mem;
  MaxImc = GetMaxImc ();

  for (SocketIndex = 0; SocketIndex<MAX_SOCKET; SocketIndex++) {
    RcDebugPrintWithDevice(SDBG_MAX, SocketIndex, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n<AdjustMemorySizeFieldsforMirror> \n");
    /*Verify the socket has memory populated and enabled*/
    if ((Host->nvram.mem.socket[SocketIndex].enabled == 0) || (Host->nvram.mem.socket[SocketIndex].maxDimmPop == 0)) {
      continue;
    }

    SadTablePtr = Host->var.mem.socket[SocketIndex].SAD;
    for (ClusterId = 0; ClusterId < MemMapData->NumberofChaDramClusters; ClusterId++) {
      BeginSadIndex = (ClusterId * MemMapData->MaxSadRules);
      EndSadIndex = (BeginSadIndex + MemMapData->MaxSadRules);
      if (SadTablePtr[BeginSadIndex].Enable == 0) {
        break;
      }
      for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
        if (SadTablePtr[SadIndex].Enable == 0) {
          break;
        }

        if (!SadTablePtr[SadIndex].local) {
          continue;
        }

        if ((SadTablePtr[SadIndex].type != MemType1lmDdr) && (SadTablePtr[SadIndex].type != MemType2lmDdrCacheMemoryMode)) {
          continue;
        }

        if (!SadTablePtr[SadIndex].mirrored) {
          continue;
        }

        CurrentSadSize = 0;
        /*Find the size of current sad*/
        if (SadTablePtr[SadIndex].Base == 0 && SadTablePtr[SadIndex].Limit == SAD_4GB) {
          CurrentSadSize = (UINT16)GetMemBlocksBelow4GB (Host);
        } else {
          CurrentSadSize = (UINT16)SadTablePtr[SadIndex].Limit - (UINT16)SadTablePtr[SadIndex].Base;
        }

        if (SizeAdjustmentForSnc[SocketIndex][SadIndex]) {
          CurrentSadSize += SizeAdjustmentForSnc[SocketIndex][SadIndex];
          AdjustMemForSnc = TRUE;
        }

        /*Initialize number of Imcs interleaved in the SAD*/
        CurrentSadInterleaves = (UINT16)SadTablePtr[SadIndex].ways;
        sadInterleaveSizePerMc = CurrentSadSize / CurrentSadInterleaves;
        for (Imc = 0; Imc < MaxImc; Imc++) {
          if (!(SadTablePtr[SadIndex].imcInterBitmap & (1 << Imc))) {
            continue;
          }

          /*for each mirrored SAD, reduce the memory contributed by each Imc to that SAD from memSize structure of the Imc*/
          if (SadTablePtr[SadIndex].type == MemType1lmDdr) {
            mem->socket[SocketIndex].imc[Imc].memSize -= sadInterleaveSizePerMc;
            mem->socket[SocketIndex].memSize -= sadInterleaveSizePerMc; // Reduce total physical memory size per socket
            if (SizeAdjustmentForSnc[SocketIndex][SadIndex]) {
              // Need to adjust for SNC
              mem->socket[SocketIndex].imc[Imc].memSize &= GB_BOUNDARY_ALIGN;
              mem->socket[SocketIndex].memSize &= GB_BOUNDARY_ALIGN;
            }
          }

          mem->memSize -= sadInterleaveSizePerMc; // Also reduce total physical memory size
        }
      } // SadIndex
    } // ClusterId
  } // For Socket Index

  if (AdjustMemForSnc) {
    // Need to adjust for SNC
    mem->memSize &= GB_BOUNDARY_ALIGN;
  }

  return Status;
}
