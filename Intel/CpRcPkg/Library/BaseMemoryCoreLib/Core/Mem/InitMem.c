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

#include <Library/MemoryCoreLib.h>
#include <Library/PipeSyncLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/CheckpointLib.h>
#include <Library/TimerLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/MemRoutingLib.h>
#include <Library/SpdDecodeLib.h>

// -------------------------------------
// Code section
// -------------------------------------

UINT32
CheckVdd (
  PSYSHOST                  Host
  )
/*++

  This function checks if Vdd needs to be set

  @param Host       - Pointer to sysHost

  @retval SUCCESS:  Memory initialization completed successfully.

--*/
{
  UINT8   socket = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    //
    // Continue if this socket is not present
    //
    if ((Host->nvram.mem.socket[socket].enabled == 0)) {
      continue;
    }

    {
      SetVdd (Host, socket);
    }
  } // socket loop

  return SUCCESS;
} // CheckVdd
#ifdef DDR5_SUPPORT
/**
  Disables the specified DIMM

  @param[in] Host         - Pointer to sysHost
  @param[in] Socket       - Socket
  @param[in] Ch           - Channel
  @param[in] Dimm         - DIMM to disable
  @param[in] Subch        - subch to disable
  @retval N/A

**/
VOID
DisableDIMMDdr5 (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Subch
  )
{
  UINT8                       Rank;
  RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "FPT: DisableDIMM()\n");

  //
  // Disable DIMM so no maintenance operations go to it
  //
  SetDimmPop (Host, Socket, Ch, Dimm);//DDR5_TODO.1507356595 update the register for spr.

  //
  // Disable each rank on this DIMM
  //
  for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, Socket, Ch, Dimm, Rank, CHECK_MAPOUT)) {
      continue;
    }

    DisableRank (Host, Socket, Ch, Dimm, Rank);
  } // rank loop
} // DisableDIMMDdr5


/*++

Returns a bitmask of channels that have the current lrdimm and rank enabled

@param[in] Host  - Pointer to sysHost
@param[in] dimm  - DIMM to check
@param[in] rank  - rank to check

@retval chBitmask - Bitmask of channels present for the current rank

--*/
UINT32
GetChBitmaskLrdimmBrs (
  IN PSYSHOST  Host,
  IN UINT8     socket,
  IN UINT8     dimm,
  IN UINT8     rank
  )
{
  UINT8               ch;
  UINT32              chBitmask = 0;
  UINT8               MaxChDdr;
  UINT8               ChannelLocal = 0;

  MaxChDdr = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    if (IsLrdimmPresent (socket, ch, dimm) == 0) {
      continue;
    }
    GetChannelMapping (ch, SUB_CH_A, &ChannelLocal);
    chBitmask |= (1 << ChannelLocal);
    GetChannelMapping (ch, SUB_CH_B, &ChannelLocal);
    chBitmask |= (1 << ChannelLocal);
  } // ch loop

  return  chBitmask;
} // GetChBitmaskLrdimmBrs

#endif
#ifdef DEBUG_CODE_BLOCK
VOID
CheckMappedOut (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm
  )
/*++

  Displays information about the DIMMs present

  @param Host  - Pointer to sysHost

  @retval N/A

--*/
{
  UINT8 ch;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {

    RcDebugPrintEmptyBlock (SDBG_DEFAULT, 20);
  } // ch loop
} // CheckMappedOut
#endif //DEBUG_CODE_BLOCK

/**

  Check if a DIMM is a dual-rank DIMM.

  @param Socket - Socket number
  @param Ch     - Channel number.
  @param Dimm   - DIMM number.

  @retval TRUE  - Current DIMM is a dual-rank DIMM.
  @retval FALSE - Current DIMM is not a dual-rank DIMM.
**/

BOOLEAN
IsDualRankOnDimm (
  IN       UINT8              Socket,
  IN       UINT8              Ch,
  IN       UINT8              Dimm
  )
{
  PSYSHOST  Host;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];

  Host = GetSysHostPointer ();
  if (IsChannelEnabled (Socket, Ch) == FALSE) {
      return FALSE;
  }

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  if ((*DimmNvList) [Dimm].numRanks == DUAL_RANK) {
    return TRUE;
  }

  return FALSE;
}

/*++

  Returns a bitmask of channels in which the current dimm# are dual-rank DIMMs.

  @param Host  - Pointer to sysHost
  @param dimm  - DIMM to check

  @retval chBitmask - Bitmask of channels present for the current rank

**/
UINT32
GetChBitmaskDualRank (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Dimm
  )
{
  UINT8               Ch;
  UINT32              ChBitmask = 0;
  UINT8               MaxChDdr;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    if (IsDimmPresent(Socket, Ch, Dimm) == FALSE) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    if (IsDualRankOnDimm(Socket, Ch, Dimm) == FALSE) {
      continue;
    }

    ChBitmask |= (1 << Ch);
  } // ch loop

  return  ChBitmask;
} // GetChBitmask

UINT32
GetChBitmask (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank
  )
/*++

  Returns a bitmask of channels that have the current dimm and rank enabled

  @param Host  - Pointer to sysHost
  @param dimm  - DIMM to check
  @param rank  - rank to check

  @retval chBitmask - Bitmask of channels present for the current rank

--*/
{
  UINT8               ch;
  UINT32              chBitmask = 0;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    chBitmask |= (1 << ch);
  } // ch loop

  return  chBitmask;
} // GetChBitmask


/*++

  Returns a bitmask of channels that have the current lrdimm and rank enabled

  @param[in] Socket  - Socket to check
  @param[in] Dimm    - DIMM to check
  @param[in] Rank    - Rank to check

  @retval chBitmask - Bitmask of channels present for the current rank

--*/
UINT32
EFIAPI
GetChBitmaskLrdimm (
  IN UINT8     Socket,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  UINT8               Ch;
  UINT32              ChBitmask = 0;
  UINT8               MaxChDdr;
  PSYSHOST            Host;

  Host = GetSysHostPointer ();
#ifdef DDR5_SUPPORT
  if (IsBrsPresent (Host, Socket)) {
    return GetChBitmaskLrdimmBrs (Host, Socket, Dimm, Rank);
  }
#endif //DDR5_SUPPORT
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    if (IsLrdimmPresent (Socket, Ch, Dimm) == 0) {
      continue;
    }

    ChBitmask |= (1 << Ch);
  } // Ch loop

  return  ChBitmask;
} // GetChBitmaskLrdimm

UINT32
GetChBitmaskAll (
  PSYSHOST  Host,
  UINT8     socket
  )
/*++

  Returns a bitmask of channels that are enabled

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval chBitmask - Bitmask of enabled channels

--*/
{
  UINT8               ch;
  UINT32              chBitmask = 0;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    chBitmask |= (1 << ch);
  } // ch loop

  return  chBitmask;
} // GetChBitmaskAll


BOOLEAN
CheckMemPresentStatus (
  PSYSHOST Host
  )
/*++

  Checks to make sure there is still memory available

  @param Host  - Pointer to sysHost

  @retval N/A

--*/
{
  UINT8               socket;
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               memPresent;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  memPresent = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    if (IsSocketDimmPopulated (Host, socket) != TRUE) {
      continue;
    }

    for (ch = 0; ch < MaxChDdr; ch++) {

      if (IsChannelEnabled (socket, ch) == FALSE) {
        continue;
      }

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        if (IsDimmPresent (socket, ch, dimm) == FALSE) {
          continue;
        }

        for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "Memory Found!\n");
          memPresent = 1 << socket;
          break;
        } // rank loop
      } // dimm loop
    } // ch loop

    //
    // Disable this socket if doesn't have any enabled memory
    //
    if ((memPresent & (1 << socket)) == 0) {
      Host->nvram.mem.socket[socket].maxDimmPop = 0;
    }
  } // socket loop

  //
  // Check for no memory error
  //
  if (memPresent == 0) {
    return FALSE;
  } else {
    return TRUE;
  }
} // CheckMemPresentStatus



/**

  Disables the specified channel in software only (Host structure).
  At the end of MRC, the Host structure will be checked for channels that have
  been disabled, and write the appropriate registers to disable them in hardware.

  @param[in] Host     Pointer to sysHost, the system Host (root) structure
  @param[in] Socket   socket number
  @param[in] Ch       channel number

  @retval None

**/
VOID
DisableChannelSw (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  UINT8               Dimm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  (*ChannelNvList)[Ch].enabled = 0;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    //
    // Skip if no DIMM present
    //
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    DisableDIMM (Host, Socket, Ch, Dimm);
  } // Dimm loop

} // DisableChannelSw

VOID
DisableDIMM (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  )
/*++
  Disables the specified DIMM

  @param Host         - Pointer to sysHost
  @param socket       - Socket
  @param ch           - Channel
  @param dimm         - DIMM to disable

  @retval N/A

--*/
{
  UINT8                       rank;
  RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "FPT: DisableDIMM()\n");

  // If NVMDimm then set NvmDimmDisable = 1
  if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].DcpmmPresent == 1) {
    Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].NvmDimmDisable = 1;
  }

  //
  // Disable each rank on this DIMM
  //
  for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, socket, ch, dimm, rank, CHECK_MAPOUT)) {
      continue;
    }

    DisableRank (Host, socket, ch, dimm, rank);
  } // rank loop
} // DisableDIMM

/**

  Maps out the specified DIMM

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel Id
  @param[in] Dimm    - DIMM Id
  @param[in] Reason  - Rank map out reason

  @retval None

**/
VOID
MapOutDimm (
  IN PSYSHOST                 Host,
  IN UINT8                    Socket,
  IN UINT8                    Ch,
  IN UINT8                    Dimm,
  IN DIMM_RANK_MAP_OUT_REASON Reason
  )
{
  UINT8 Rank;

  if ((Host == NULL) || (Socket >= MAX_SOCKET) || (Ch >= MAX_CH) || (Dimm >= MAX_DIMM) || (Reason >= DIMM_RANK_MAP_OUT_MAX)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FPT: MapOutDimm() - invalid parameters\n");
    return;
  }

  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FPT: MapOutDimm() - reason 0x%x\n", Reason);

  //
  // Map out each rank on this DIMM
  //
  for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
    //
    // Map out rank
    //
    MapOutRank (Host, Socket, Ch, Dimm, Rank, Reason);
  }
}


VOID
DisableRank (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank
  )
/*++
  Disables the specified rank

  @param Host        - Pointer to sysHost
  @param socket      - Socket
  @param ch          - Channel
  @param dimm        - DIMM
  @param rank        - Rank to disable

  @retval N/A

--*/
{
  UINT8                       pairChannel;
  UINT8                       pairDimm;
  UINT8                       pairRank;
  UINT8                       pairRankNumber;
  struct ddrRank              (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram            (*dimmNvList)[MAX_DIMM];
  struct channelNvram         (*channelNvList)[MAX_CH];

  dimmNvList = GetDimmNvList (Host, socket, ch);
  rankList = GetRankNvList (Host, socket, ch, dimm);
  channelNvList = GetChannelNvList (Host, socket);

  //
  // Make sure the rank has not already been disabled.  This function can get called recurvsively in Mirror/Lockstep
  //

  if ((*rankList)[rank].enabled != 0) {
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                            "FPT: DisableRank()\n\n");

    //
    // Disable rank in Host structure
    //
    (*rankList)[rank].enabled = 0;
    (*dimmNvList)[dimm].mapOut[rank] = 1;
    (*dimmNvList)[dimm].MapOutReason[rank] = DIMM_RANK_MAP_OUT_RANK_DISABLED;

    EwlOutputType1(WARN_USER_DIMM_DISABLE, WARN_USER_DIMM_DISABLE_RANK_DISABLED, socket, ch, dimm, rank);

    //
    // Rank is logically disabled at this point using rank mapout
    //
    // Notes about the mapout flow:
    //
    // - Before switching from IOSAV mode to Normal mode:
    // CheckRASSupportAfterMemInit evaluates rank mapout settings
    // If all ranks present on a channel are mapped out, then set SCRUBMASK all_ranks field
    // If only some ranks present on a channel are mapped out, then set AMAP dimmX_pat_rank_disable field

    //
    // - After Memtest and init are complete:
    // CheckRASSupportAfterMemInit initializes the patrol scrub settings

    //
    //If in Lockstep or Mirror, disable the paired Rank
    //
    if (ChipLockstepEn (Host) || ChipMirrorEn (Host)) {
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
                              "System configured for Lockstep or Mirroring.  Disable paired rank(s)\n");

      pairRankNumber = 1;

      //
      // Disable all paired ranks
      //
      while (pairRankNumber) {

        GetPairedRank (Host, socket, ch, dimm, rank, &pairRankNumber, &pairChannel, &pairDimm, &pairRank);

        //
        // Break if no more pairs
        //
        if (pairRankNumber == 0) {
          break;
        }

        dimmNvList = GetDimmNvList (Host, socket, pairChannel);
        rankList = GetRankNvList (Host, socket, pairChannel, pairDimm);

        RcDebugPrintWithDevice (SDBG_MINMAX, socket, pairChannel, pairDimm, NO_SUBCH, pairRank, NO_STROBE, NO_BIT,
                                "FPT: DisableRank() paired rank\n\n");

        //
        // Disable rank in Host structure
        //
        (*rankList)[pairRank].enabled = 0;
        (*dimmNvList)[pairDimm].mapOut[pairRank] = 1;
        (*dimmNvList)[pairDimm].MapOutReason[pairRank] = DIMM_RANK_MAP_OUT_RANK_DISABLED;

        EwlOutputType1(WARN_USER_DIMM_DISABLE, WARN_USER_DIMM_DISABLE_RANK_DISABLED, socket, ch, dimm, rank);

        //
        // Disable pair channel if mirror is enabled
        //
        if (ChipMirrorEn (Host)) {
          channelNvList = GetChannelNvList (Host, socket);
          (*channelNvList)[pairChannel].enabled = 0;
        }

        //
        // Check for another pair
        //
        pairRankNumber++;
      }
    } //end if lockstep
  }
} // DisableRank

/**

  Maps out the specified rank

  @param[in] Host    - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Channel Id
  @param[in] Dimm    - DIMM Id
  @param[in] Rank    - Rank to map out
  @param[in] Reason  - Rank map out reason

  @retval None

**/
VOID
MapOutRank (
  IN PSYSHOST                 Host,
  IN UINT8                    Socket,
  IN UINT8                    Ch,
  IN UINT8                    Dimm,
  IN UINT8                    Rank,
  IN DIMM_RANK_MAP_OUT_REASON Reason
  )
{
  UINT8                 PairChannel;
  UINT8                 PairDimm;
  UINT8                 PairRank;
  UINT8                 PairRankNumber = 1;
  DIMM_NVRAM_STRUCT     (*DimmNvList)[MAX_DIMM];

  if ((Host == NULL) || (Socket >= MAX_SOCKET) || (Ch >= MAX_CH) || (Dimm >= MAX_DIMM) || (Rank >= MAX_RANK_DIMM) || (Reason >= DIMM_RANK_MAP_OUT_MAX)) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "FPT: MapOutRank() - invalid parameters\n");
    return;
  }

  //
  // Get DIMM list for channel
  //
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  //
  // Rank already mapped out
  //
  if (CheckRank (Host, Socket, Ch, Dimm, Rank, CHECK_MAPOUT)) {
    return;
  }

  RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "FPT: MapOutRank() - reason 0x%x\n\n", Reason);

  //
  // Map out rank
  //
  (*DimmNvList)[Dimm].mapOut[Rank] = 1;
  (*DimmNvList)[Dimm].MapOutReason[Rank] = Reason;
  EwlOutputType1 (WARN_USER_DIMM_DISABLE, WARN_USER_DIMM_DISABLE_MAPPED_OUT, Socket, Ch, Dimm, Rank);

  //
  // If Lockstep, map out the paired rank
  //
  if (ChipLockstepEn (Host)) {

    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, Rank, NO_STROBE, NO_BIT, "System configured for Lockstep or Mirroring. Map out paired rank(s)\n");

    //
    // Disable all paired ranks
    //
    while (PairRankNumber) {
      //
      // Get paired rank
      //
      GetPairedRank (Host, Socket, Ch, Dimm, Rank, &PairRankNumber, &PairChannel, &PairDimm, &PairRank);

      //
      // Break if no more pairs
      //
      if (PairRankNumber == 0) {
        break;
      }

      //
      // Get DIMM list for paired channel
      //
      DimmNvList = GetDimmNvList (Host, Socket, PairChannel);

      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, PairChannel, PairDimm, NO_SUBCH, PairRank, NO_STROBE, NO_BIT, "FPT: MapOutRank() paired rank\n\n");

      //
      // Map out paired rank
      //
      (*DimmNvList)[PairDimm].mapOut[PairRank] = 1;
      (*DimmNvList)[PairDimm].MapOutReason[PairRank] = Reason;
      EwlOutputType1 (WARN_USER_DIMM_DISABLE, WARN_USER_DIMM_DISABLE_MAPPED_OUT, Socket, Ch, Dimm, Rank);

      //
      // Check for another pair
      //
      PairRankNumber++;
    }
  }

}

VOID
GetDimmPresent (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     *dimmMask[MAX_CH]
  )
/*++
  Returns a bit mask of DIMMs present.

  @param Host:         Pointer to sysHost
  @param socket:         Processor socket to check
  @param dimmMask:     Pointer to results

  @retval N/A
--*/
{
  UINT8  ch;
  UINT8  dimm;
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  // Update channels
  for (ch = 0; ch < MaxChDdr; ch++) {

    // Initialize to 0 for this channel
    *dimmMask[ch] = 0;

    //
    // Go to next channel if this is disabled
    //

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      //
      // Check next DIMM if this one is not present
      //

      if (IsDimmPresent (socket, ch, dimm) == FALSE) {
        continue;
      }

      // Indicate this dimm is present
      *dimmMask[ch] |= 1 << dimm;

    } // dimm loop
  } // ch loop
} // GetDimmPresent

UINT8
IsLrdimmPresent (
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm
  )
/**
  Detect if current DIMM is LRDIMM

  @param Socket  - Socket Id
  @param Ch      - Channel number (0-based)
  @param Dimm    - Current dimm

  @retval 1 LRDIMM is present
  @retval 0 LRDIMM is not present
**/
{
  PSYSHOST Host;

  Host = (PSYSHOST) GetSysHostPointer ();

  return (Host->nvram.mem.socket[Socket].channelList[Ch].dimmList[Dimm].lrDimmPresent);
} // IsLrdimmPresent

/**

  Detect if any LRDIMM is present on a given socket

  @param[in] Socket  - Socket Id

  @retval 1 LRDIMM is present
  @retval 0 LRDIMM is not present

**/
UINT8
EFIAPI
IsLrdimmPresentOnSocket (
  IN UINT8 Socket
  )
{
  PSYSHOST            Host;
  UINT8               Ch;
  UINT8               Dimm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  UINT8               LrdimmFound = 0;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Host = (PSYSHOST) GetSysHostPointer ();

  ChannelNvList = GetChannelNvList (Host, Socket);

  // Update channels
  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    // Go to next channel if this is disabled
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      LrdimmFound = IsLrdimmPresent (Socket, Ch, Dimm);
      if (LrdimmFound != 0) {
        break;
      }
    } // dimm loop
    if (LrdimmFound != 0) {
      break;
    }
  } // ch loop
  return LrdimmFound;
}

/**
  Determine whether the socket contains any DIMMs that are not LDRIMMs (RDIMMs, UDIMMs, etc.).

  @param[in] Socket   The socket to search for non-LRDIMMs

  @retval FALSE   All DIMMs on the socket are LRDIMMs
  @retval TRUE    The socket contains at least one DIMM that is not an LRDIMM
**/
BOOLEAN
EFIAPI
IsNonLrdimmPresentOnSocket (
  IN  UINT8 Socket
  )
{
  UINT8   Channel = 0;
  UINT8   Dimm = 0;
  BOOLEAN Found = FALSE;
  UINT8   MaxChDdr = 0;

  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Channel, Dimm) == FALSE) {
        continue;
      }

      if (IsLrdimmPresent (Socket, Channel, Dimm) == FALSE) {
        Found = TRUE;
      }

      if (Found) {
        break;
      }
    }

    if (Found) {
      break;
    }
  }

  return Found;
}

/*
  Determines if there is any ECC Dimm populated in the channel

  @param[in] Socket - Socket under test (Based-0)
  @param[in] Ch     - Channel under test (Based-0)

  @retval TRUE  When a ECC device is detected in the channel
  @retval FALSE Any other Dimm or not populated
*/
BOOLEAN
EFIAPI
IsThereAnyEccDimmInChannel (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
  UINT8             Dimm;
  PSYSHOST          Host;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  Host = (PSYSHOST) GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    // Check if bus width extension is present to detect ECC
    if (((*DimmNvList)[Dimm].SPDMemBusWidth & SPD_BUS_WIDTH_EXT) != 0) {
      return TRUE;
    }
  }

  return FALSE;
}

/*
  Determines if there is any x16 Dimm populated in the channel

  @param[in] Socket - Socket under test (Based-0)
  @param[in] Ch     - Channel under test (Based-0)

  @retval TRUE  When a x16 device is detected in the channel
  @retval FALSE Any other Dimm or not populated
*/
BOOLEAN
EFIAPI
IsThereAnyDimmX16InChannel (
  IN  UINT8   Socket,
  IN  UINT8   Ch
  )
{
  UINT8             Dimm;
  PSYSHOST          Host;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];

  Host = (PSYSHOST) GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    if ((*DimmNvList)[Dimm].dramIOWidth == SPD_DRAM_IO_WIDTH_X16) {
      return TRUE;
    }
  }

  return FALSE;
}


VOID
GetRankPresent (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     *rankMask[MAX_CH]
  )
/*++
  Returns a bit mask of logical ranks present.

  @param Host:         Pointer to sysHost
  @param socket:         Processor socket to check
  @param rankMask:     Pointer to results

  @retval N/A
--*/
{
  UINT8  ch;
  UINT8  dimm;
  UINT8  rank;
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  UINT8  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  // Update channels
  for (ch = 0; ch < MaxChDdr; ch++) {

    // Initialize to 0 for this channel
    *rankMask[ch] = 0;

    //
    // Go to next channel if this is disabled
    //

    if (IsChannelEnabled (socket, ch) == FALSE) {
      continue;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      //
      // Check next DIMM if this one is not present
      //

      if (IsDimmPresent (socket, ch, dimm) == FALSE) {
        continue;
      }

      rankList = GetRankNvList (Host, socket, ch, dimm);

      // Loop for each rank
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {

        // Continue to the next rank if this one is disabled
        if (CheckRank (Host, socket, ch, dimm, rank, CHECK_MAPOUT)) {
          continue;
        }

        // Indicate this rank is present
        *rankMask[ch] |= 1 << (*rankList)[rank].logicalRank;
      } // rank loop
    } // dimm loop
  } // ch loop
} // GetRankPresent

VOID
FastestCommon (
  UINT8 *time1,
  UINT8 *time2,
  UINT8 *time3
  )
/*++

  Returns the fastest time of the times provided

  @param time1 - 1st time to check
  @param time2 - 2nd time to check
  @param time3 - 3rd time to check (ignored if 0)

  @retval N/A

--*/
{
  UINT8 newTime;

  if (*time1 > *time2) {
    newTime = *time1;
  } else {
    newTime = *time2;
  }

  if ((time3 != 0) && (*time3 > newTime)) {
    newTime = *time3;

  }

  *time1 = newTime;
  *time2 = newTime;
  if (time3 != 0) {
    *time3 = newTime;
  }
} // FastestCommon

VOID
FastestCommon16 (
  UINT16 *time1,
  UINT16 *time2,
  UINT16 *time3
  )
/*++

  Returns the fastest time of the times provided

  @param time1 - 1st time to check
  @param time2 - 2nd time to check
  @param time3 - 3rd time to check (ignored if 0)

  @retval N/A

--*/
{
  UINT16  newTime;

  if (*time1 > *time2) {
    newTime = *time1;
  } else {
    newTime = *time2;
  }

  if ((time3 != 0) && (*time3 > newTime)) {
    newTime = *time3;

  }

  *time1 = newTime;
  *time2 = newTime;
  if (time3 != 0) {
    *time3 = newTime;
  }
} // FastestCommon16



UINT32
BaseLimitOffset (
  UINT32 value,
  UINT32 base,
  UINT32 limit,
  UINT32 offset
  )
/*++

  Ensure we do not program a value larger than is supported by the field

  @param value   - Value to be programmed
  @param base    - Smallest value that can be programmed
  @param limit   - Largest value that can be programmed
  @param offset  - Offset in the register to be programmed

  @retval regValue  - Value to be programmed

--*/
{
  UINT32  retValue;

  if (value > base) {
    retValue = value - base;
  } else {
    retValue = 0;

  }

  if (retValue > limit) {
    retValue = limit;
  }

  return retValue << offset;
} // BaseLimitOffset


VOID
FixedQClkDelay (
  PSYSHOST Host,
  UINT32   QClk
  )
/*++

  Delay time in QCLK units
    Note: Max delay is 1.2 seconds

  @param Host  - Pointer to sysHost
  @param QClk  - Number of QClks to delay

  @retval N/A

--*/
{
  UINT32  delay;

  delay = ((Host->var.mem.QCLKPeriod/1000000) * QClk) + 1;
  FixedDelayMicroSecond (delay);
} // FixedQClkDelay

UINT8
CheckRankAbsent (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank
  )
/*++

  Checks if a rank is absent

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param ch      - Channel number
  @param dimm    - DIMM number
  @param rank    - Rank number

  @retval 1 if rank is disabled or absent
  @retval 0 if enabled or present

--*/
{
  struct ddrRank  (*rankNvList)[MAX_RANK_DIMM];
  rankNvList = GetRankNvList (Host, socket, ch, dimm);

  if ((*rankNvList)[rank].enabled == 0) {
    return 1;
  } else {
    return 0;
  }

} // CheckRankAbsentDisabled

UINT8
CheckRankAbsentDisabled (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank
  )
/*++

  Checks if a rank is absent or disabled

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param ch      - Channel number
  @param dimm    - DIMM number
  @param rank    - Rank number

  @retval 1 if rank is disabled or absent
  @retval 0 if enabled

--*/
{
  struct ddrRank  (*rankNvList)[MAX_RANK_DIMM];
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  rankNvList = GetRankNvList (Host, socket, ch, dimm);
  dimmNvList = GetDimmNvList (Host, socket, ch);

  if ((*rankNvList)[rank].enabled == 0) {
    return 1;
  }
  if ((*dimmNvList)[dimm].mapOut[rank]) {
    return 1;
  }

  return 0;

} // CheckRankAbsentDisabled


/**

  Determines if this rank is valid

  @param Host   - Pointer to sysHost, the system Host (root) structure
  @param socket - socket number
  @param ch     - channel number
  @param dimm   - dimm number
  @param rank   - rank number
  @param flags  - CHECK_MAPOUT

  @retval SUCCESS - Rank is valid
  @retval FAILURE - Rank is not valid

**/
UINT8
CheckRank (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank,
  UINT8    flags
  )
{
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  struct ddrRank    (*rankList)[MAX_RANK_DIMM];

  dimmNvList = GetDimmNvList (Host, socket, ch);
  rankList = GetRankNvList (Host, socket, ch, dimm);

  //
  // always return false for rank1 on ddrt2 dimms as CS1 was repurposed for S3 wake event, NOT function as a normal CS pin.
  //
  if (((*dimmNvList)[dimm].fmcType == FMC_CWV_TYPE) && (rank == 1)) {
    return FAILURE;
  }

  if (((*rankList)[rank].enabled == 0) && (GetCurrentTestType (socket) != EarlyRcdCsLoopbackTest)) {
    return FAILURE;
  }
  if ((flags & CHECK_MAPOUT) && (Host->var.mem.checkMappedOutRanks == 0)) {
    if ((*dimmNvList)[dimm].mapOut[rank]) {
      return FAILURE;
    }
  }

  return SUCCESS;
} // CheckRank


/**

  Calculate package rank size (in 64MB unit) for asymmetric or symmeteric DIMM gived the target package rank number.

  @param[in] Socket            - Socket number
  @param[in] Ch                - Channel number
  @param[in] Dimm              - Dimm number
  @param[in] Rank              - Package rank number
  @param[out] RankSize         - Pointer to package rank size in 64MB unit

  @retval MRC_STATUS_SUCCESS

**/

MRC_STATUS
CalculateDimmPackageRankSize (
  IN     UINT8   Socket,
  IN     UINT8   Ch,
  IN     UINT8   Dimm,
  IN     UINT8   Rank,
     OUT UINT16  *RankSize
  )
{
  PSYSHOST            Host;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               TechIndex;

  Host          = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  DimmNvList    = GetDimmNvList (Host, Socket, Ch);

  if (IsDdr5Present (Host, Socket)) {
    CalculateDimmPackageRankSizeDdr5 (Socket, Ch, Dimm, Rank, RankSize);
    return MRC_STATUS_SUCCESS;
  }
  if ((*DimmNvList)[Dimm].mapOut[Rank]) {
    *RankSize = 0;
  } else {
    if ((*DimmNvList)[Dimm].DcpmmPresent) {
      TechIndex = (*DimmNvList)[Dimm].aepTechIndex;
      *RankSize  = rankSizeDdrt[TechIndex];
    } else {
      TechIndex = (*DimmNvList)[Dimm].techIndex;
      *RankSize = rankSizeDDR4[TechIndex];
      //
      // Handle asymmetric Dimm
      //
      if (((*DimmNvList)[Dimm].SPDSecondaryDeviceType & SPD_HALF_SIZE_SECOND_RANK) && (Rank % 2)) {
        *RankSize = (*RankSize) / 2;
      } else if (((*DimmNvList)[Dimm].SPDSecondaryDeviceType & SPD_QUARTER_SIZE_SECOND_RANK) && (Rank % 2)) {
        *RankSize = (*RankSize) / 4;
      }
    }
  }

  if (((*DimmNvList)[Dimm].dieCount) && ((*ChannelNvList)[Ch].encodedCSMode == 2)) {
    (*RankSize) *= (*DimmNvList)[Dimm].dieCount;
  }

  return MRC_STATUS_SUCCESS;
} // CalculateDimmPackageRankSize

UINT32
CheckDimmRanks (
  PSYSHOST Host
  )
/*++

  Initialize rank structures.  This is based on the requested
  RAS mode and what RAS modes the configuration supports. This
  routine also evalues the requested RAS modes to ensure they
  are supported by the system configuration.

  @param Host  - Pointer to sysHost

  @retval SUCCESS

--*/
{
  UINT8   socket;
  UINT8   ch;
  UINT8   dimm;
  UINT16  size;
  UINT8   i;
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  SYS_SETUP *Setup;
  UINT8   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  //
  // Check if any ranks have been disabled
  //
  CheckRankPop (Host);

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    if (IsSocketDimmPopulated (Host, socket) != TRUE) {
      continue;
    }

    channelNvList = GetChannelNvList (Host, socket);

    //
    // Look at each channel
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if (!IsChannelValid (Host, ch)) {
        continue;
      }
      dimmNvList = GetDimmNvList (Host, socket, ch);

      //
      // Look at each DIMM
      //
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {

        if (IsDimmPresent (socket, ch, dimm) == FALSE) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);

        if (IsLrdimmPresent (socket, ch, dimm)) {
          i = (*dimmNvList)[dimm].numDramRanks;
        } else {
          i = (*dimmNvList)[dimm].numRanks;
        }

        //
        // Loop for each rank
        //
        for (; i != 0; i--) {

          // Skip ranks that are disabled
          if (IsRankDisabled (Host, socket, ch, dimm, i - 1)) {
            continue;
          }
          //
          // Determine rank size
          //
          CalculateDimmPackageRankSize (socket, ch, dimm, i - 1, &size);
          SetRankIndexes (Host, socket, ch, dimm, i - 1);

          //
          // Enable this rank (0-based)
          //
          (*rankList)[i - 1].enabled = 1;

          (*rankList)[i - 1].rankSize = size;
          (*rankList)[i - 1].remSize = size;

          if (PcdGetBool (PcdNvDimmEn)) {
            if ((((*dimmNvList)[dimm].NvDimmNPresent && Setup->mem.ADRDataSaveMode == ADR_NVDIMM) || Setup->mem.socket[socket].ddrCh[ch].batterybacked) && !(Host->nvram.mem.DcpmmPresent)) {
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, i - 1, NO_STROBE, NO_BIT,
                                      "RankSize");
              (*rankList)[i - 1].NVrankSize = size;
              (*rankList)[i - 1].remSize = 0;
            }
          }
        } // rank loop
      } // dimm loop

    } // ch loop
  } // socket loop

  RASCheckDimmRanks (Host);

  return SUCCESS;
} // CheckDimmRanks

/**
  Map out ranks based on Policy rankmask.

  @param[in]  Host              A pointer to host data.

  @retval EFI_SUCCESS           Success.
  @retval !EFI_SUCCESS          Failure.

**/

EFI_STATUS
EFIAPI
CheckRankMapout (
  IN PSYSHOST Host
  )
{
  UINT8 Rank;
  UINT8 Dimm;
  UINT8 DimmRank;
  UINT8 Socket;
  UINT8 Channel;
  SYS_SETUP *Setup;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  //
  // Update rank mapout data based on rankMask option
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    for (Channel = 0; Channel < MaxChDdr; Channel++) {

      if (Host->nvram.mem.socket[Socket].channelList[Channel].maxDimm > MAX_DIMM) {
        return EFI_INVALID_PARAMETER;
      }

      for (Rank = 0; Rank < MAX_RANK_CH; Rank++) {

        DimmRank = Rank % MAX_RANK_DIMM;
        Dimm = Rank / MAX_RANK_DIMM;

        //
        // If rank is not enabled in rankMask, then  flag the rank as
        // mapped out
        //
        if (!(Setup->mem.socket[Socket].ddrCh[Channel].rankmask & (1 << Rank))) {
          Setup->mem.socket[Socket].ddrCh[Channel].dimmList[Dimm].mapOut[DimmRank] = 1;
        } else {
          Setup->mem.socket[Socket].ddrCh[Channel].dimmList[Dimm].mapOut[DimmRank] = 0;
        }
      } // rank
    } // ch
  } // socket

  return EFI_SUCCESS;

} // CheckRankMapout


//
// Host struct abstraction functions
//
struct imcNvram
(*GetImcNvList (
  PSYSHOST Host,
  UINT8    socket
  ))[MAX_IMC]
{
  return (&Host->nvram.mem.socket[socket].imc);
}

struct channelNvram
(*GetChannelNvList (
  PSYSHOST  Host,
  UINT8     socket
  ))[MAX_CH]
{
  return (&Host->nvram.mem.socket[socket].channelList);
}

struct dimmNvram
(*GetDimmNvList (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch
  ))[MAX_DIMM]
{
  return (&Host->nvram.mem.socket[socket].channelList[ch].dimmList);
}

struct ddrRank
(*GetRankNvList (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  ))[MAX_RANK_DIMM]
{
  return (&Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList);
}

struct rankDevice
(*GetRankStruct (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  ))[MAX_RANK_DIMM]
{
  return (&Host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct);
}

/**

  Get the Mem Nvram pointer from Host

  @param None

**/
MEM_NVRAM_STRUCT *
GetMemNvramPointer (
  VOID
  )
{
  PSYSHOST Host;
  Host = GetSysHostPointer ();
  return &Host->nvram.mem;
}

/**

  Get the Mem Var pointer from Host

  @param None

**/
MEM_VAR_STRUCT *
GetMemVarPointer (
  VOID
  )
{
  PSYSHOST Host;
  Host = GetSysHostPointer ();
  return &Host->var.mem;
}

/**

    Get DDR training channel list

    @param -  N/A

    @retval - Pointer to channel list array

**/
struct DdrChannelTraining
(*GetChannelListTraining (
  VOID)
  )[MAX_CH]
{
  PSYSHOST Host;

  Host = (PSYSHOST) GetSysHostPointer ();

  return (&Host->var.mem.TrainingVar.channelList);
}

/**

    Get local DDR training variables structure

    @param - N/A

    @retval - Pointer to training variables structure

**/
struct TrainingVariable*
MemTrainingVar (
  VOID
  )
{
  PSYSHOST Host;

  Host = (PSYSHOST) GetSysHostPointer ();

  return (&Host->var.mem.TrainingVar);
}

/**
  Retrieve the cached MRS value for a given strobe

  @param[in] Socket
  @param[in] Ch
  @param[in] Dimm
  @param[in] Rank
  @param[in] Strobe
  @param[in] Bank

  @retval UINT16 Cached value for given setting
**/
UINT16
EFIAPI
GetPerStrobeMrsValue (
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        UINT8           Dimm,
  IN        UINT8           Rank,
  IN        UINT8           Strobe,
  IN        UINT8           Bank
  )
{
  UINT16              PerStrobeValue = 0;
#ifndef DDR5_SUPPORT
  SYSHOST             *Host;
  struct rankDevice   (*RankStruct)[MAX_RANK_DIMM];
  UINT32              Mr1RxEqSettingForInputStrobe = 0;

  Host = GetSysHostPointer ();
  RankStruct = GetRankStruct (Host, Socket, Ch, Dimm);

  switch (Bank) {
  case BANK0:
    PerStrobeValue = (*RankStruct)[Rank].MR0;
    break;
  case BANK1:
    PerStrobeValue = (*RankStruct)[Rank].MR1;
    if (Host->nvram.mem.socket[Socket].TrainingStepDone.DramRxEq == 1) {
      //
      // Merge the "common" MR1 base setting with the trained RxEq Setting stored in the packed data structure
      // the stucture contains 9 elements per structure so the first 9 strobes are in the first structure
      // and only 2 structures are currently accomodated so force strobe 0 if the input strobe is out of range
      //
      if (Strobe >= 18) {
        Strobe = 0;
      }
      Mr1RxEqSettingForInputStrobe = GetCachedRxEq (Host, Socket, Ch, Rank, Dimm, Strobe);
      //
      // Clear out the Rx Eq bits from the common setting
      //
      PerStrobeValue &= ~MR1_RX_EQ;
      //
      // Set the Rx Eq bits from the Mr1RxEqSettings struct in the Host
      //
      PerStrobeValue |= DistributeSparse16BitField ((BIT13 | BIT6 | BIT5), (UINT16) Mr1RxEqSettingForInputStrobe);
    }
    break;
  case BANK2:
    PerStrobeValue = (*RankStruct)[Rank].MR2;
    break;
  case BANK3:
    PerStrobeValue = (*RankStruct)[Rank].MR3;
    break;
  case BANK4:
    PerStrobeValue = (*RankStruct)[Rank].MR4;
    break;
  case BANK5:
    PerStrobeValue = (*RankStruct)[Rank].MR5;
    break;
  case BANK6:
    PerStrobeValue = (*RankStruct)[Rank].MR6[Strobe];
    break;
  default:
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_77);
  }

#endif // !DDR5_SUPPORT
  return PerStrobeValue;
}

/** Return the number of sockets in the system.

  @retval The number of sockets in the system.

**/
UINT8
EFIAPI
GetNumSockets (
  VOID
  )
{
  PSYSHOST  Host;
  UINT8     NumSockets = 0;
  UINT8     Socket;

  Host = GetSysHostPointer ();
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (Host->nvram.mem.socket[Socket].enabled == 1) {
      NumSockets++;
    }
  }

  return NumSockets;
}
