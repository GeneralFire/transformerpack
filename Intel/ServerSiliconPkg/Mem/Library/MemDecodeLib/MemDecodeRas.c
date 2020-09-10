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

#include <Library/MemRcLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryCoreLib.h>
#include <MemDecodeRas.h>

  /**

  Check if two channels in a IMC is capable for mirroring by checking if DIMMs
  populated in all of those channels are good for mirroring mode

  @param Host  - Pointer to sysHost
  @param socket  - Socket number
  @param imc   - IMC number

  @retval RAS_IS_CAPABLE
  @retval RAS_NOT_CAPABLE

  **/
UINT8
CheckMirrorPopulation2 (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Imc,
  UINT8    Ch0,        // at Imc level
  UINT8    Ch1         // at Imc level
  )
{
  UINT8   Capable;
  UINT8   Dimm;
  UINT8   Rank;
  UINT8   MaxChPerMc;

  SOCKET_NVRAM            *socketNv;
  DDR_RANK_STRUCT         (*rankList0)[MAX_RANK_DIMM];
  DDR_RANK_STRUCT         (*rankList1)[MAX_RANK_DIMM];
  DIMM_NVRAM_STRUCT       (*dimmNvList0)[MAX_DIMM];
  DIMM_NVRAM_STRUCT       (*dimmNvList1)[MAX_DIMM];
  CHANNEL_NVRAM_STRUCT    (*channelNvList)[MAX_CH];
  DIMM_DEVICE_INFO_STRUCT (*chdimmList0)[MAX_DIMM];
  DIMM_DEVICE_INFO_STRUCT (*chdimmList1)[MAX_DIMM];

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "Imc = %x, Imc Level: Ch0 = %x, Ch1 = %x\n", Imc, Ch0, Ch1
    );
  // convert chx to socket level
  MaxChPerMc = GetNumChannelPerMc ();
  if (Imc > 0) {
    Ch0 += Imc * MaxChPerMc;
    Ch1 += Imc * MaxChPerMc;
  }
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "MaxChPerMc = %x, Socket Level: Ch0 = %x, Ch1 = %x\n", MaxChPerMc, Ch0, Ch1
    );

  Capable = RAS_IS_CAPABLE;

  socketNv = &Host->nvram.mem.socket[Socket];
  channelNvList = GetChannelNvList(Host, Socket);

  //
  // If both channel Ch0 and Ch1 not enabled then skip channel pair for determining system RAS capability
  //
  if ((*channelNvList)[Ch0].enabled == 0 && (*channelNvList)[Ch1].enabled == 0) {
    Capable = RAS_SKIP_CHANNEL;
  } else if ((*channelNvList)[Ch0].enabled != (*channelNvList)[Ch1].enabled) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "RAS_NOT_CAPABLE: [Ch0].enabled = %x, [Ch1].enabled = %x\n", (*channelNvList)[Ch0].enabled, (*channelNvList)[Ch1].enabled
      );
    Capable = RAS_NOT_CAPABLE;
  } else {
    dimmNvList0 = GetDimmNvList(Host, Socket, Ch0);
    dimmNvList1 = GetDimmNvList(Host, Socket, Ch1);
    chdimmList0 = &Host->var.mem.socket[Socket].channelList[Ch0].dimmList;
    chdimmList1 = &Host->var.mem.socket[Socket].channelList[Ch1].dimmList;

    //
    // Compare each pair of DIMMs to ensure not present or match
    //
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      rankList0 = &socketNv->channelList[Ch0].dimmList[Dimm].rankList;
      rankList1 = &socketNv->channelList[Ch1].dimmList[Dimm].rankList;

      //
      // Ensure DIMMs in both channels both present or both not present
      //
      if (((*dimmNvList0)[Dimm].dimmPresent ^ (*dimmNvList1)[Dimm].dimmPresent) != 0) {
#ifdef DDRT_SUPPORT
        if ((*dimmNvList0)[Dimm].DcpmmPresent || (*dimmNvList1)[Dimm].DcpmmPresent) {
          continue;
        }
#endif // DDRT_SUPPORT
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "RAS_NOT_CAPABLE: DimmPresent %x ^ %x != 0\n", (*dimmNvList0)[Dimm].dimmPresent, (*dimmNvList1)[Dimm].dimmPresent
          );
        Capable = RAS_NOT_CAPABLE;
        break;
      }

      //
      // Exactly same 2LM size means NM channel & FM channel of each address will be same (validation scope reduction)
      //
#ifdef DDRT_SUPPORT
      if ((*dimmNvList0)[Dimm].DcpmmPresent) {
        if ((*dimmNvList0)[Dimm].volCap != (*dimmNvList1)[Dimm].volCap) {
          Capable = RAS_NOT_CAPABLE;
          break;
        }
      } else
#endif // DDRT_SUPPORT
      {
        if ((*chdimmList0)[Dimm].memSize != (*chdimmList1)[Dimm].memSize) {
          RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "RAS_NOT_CAPABLE: Memsize %x != %x\n", (*chdimmList0)[Dimm].memSize, (*chdimmList1)[Dimm].memSize
            );
          Capable = RAS_NOT_CAPABLE;
          break;
        }
        for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {
          if (((*rankList0)[Rank].enabled ^ (*rankList1)[Rank].enabled) != 0) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "RAS_NOT_CAPABLE: Rank.Enabled %x ^ %x != 0\n", (*rankList0)[Rank].enabled, (*rankList1)[Rank].enabled
              );
            Capable = RAS_NOT_CAPABLE;
            break;
          }

          //
          // Ensure ranks in both channels both mapped out or not mapped out
          //
          if (((*dimmNvList0)[Dimm].mapOut[Rank] ^ (*dimmNvList1)[Dimm].mapOut[Rank]) != 0) {
            RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "RAS_NOT_CAPABLE: Rank.mapout %x ^ %x != 0\n", (*rankList0)[Rank].enabled, (*rankList1)[Rank].enabled
              );
            Capable = RAS_NOT_CAPABLE;
            break;
          }
        } // Rank loop
      }
    } // for (dimm)
  } // if ()

  RcDebugPrintWithDevice(SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\nCheckMirrorPopulation (Socket = %d, Imc = %d) - ", Socket, Imc);
  RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    (Capable == RAS_IS_CAPABLE) ? "Yes\n" : "No\n");

  return Capable;
}

/**

Routine figures out if a SAD can be mirrored

@param Host      - Pointer to sysHost
@param SAD       - Pointer to the Sad Table
@param SadIndex  - Index into the Sad Table

@retval 0 Sad not local or not enabled
@retval 1 Sad can not be mirrored
@retval 2 Sad can be mirrored
**/
UINT8
CheckSadForMirroring (
  PSYSHOST  Host,
  SAD_TABLE *SadTablePtr,
  UINT8     SadIndex
  )
{
  UINT8    ChannelWays;

  if (SadTablePtr[SadIndex].Enable == 0) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "SAD Not Enabled: %d\n", SadIndex);
    return SAD_NOT_ENABLED; // end of SAD table
  }

  if (SadTablePtr[SadIndex].local != 1) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "SAD Not Local: %d\n", SadIndex);
    return SAD_NOT_ELIGIBLE_MIRROR; // Use only local SAD for creating partial mirror regions.
  }

  if (SadTablePtr[SadIndex].Attr == SAD_NXM_ATTR) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Cannot mirror Non Existant Mem %d\n", SadIndex);
    return SAD_NOT_ELIGIBLE_MIRROR;
  }

  //Find If channel interleave bitmap is power of 2 -- No interleaving and single channel way
  ChannelWays = SadTablePtr[SadIndex].NmChWays;
  if (ChannelWays == ONE_WAY) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "SAD %d is single chway\n", SadIndex);
    return SAD_NOT_ELIGIBLE_MIRROR;
  } //Cannot mirror 1 chway in SKX

  if (SadTablePtr[SadIndex].type != MemType1lmDdr) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "SAD %d is not 1LM\n", SadIndex);
    return SAD_NOT_ELIGIBLE_MIRROR;
  } //skip SAD if, PMEM/PMEM$

  if (SadTablePtr[SadIndex].mirrored == 1) {
    RcDebugPrintWithDevice (SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "SAD: %d is already mirrored\n", SadIndex);
    return SAD_NOT_ELIGIBLE_MIRROR; // Skip SAD if its already mirrored
  }
  return SAD_ELIGIBLE_MIRROR;
}
