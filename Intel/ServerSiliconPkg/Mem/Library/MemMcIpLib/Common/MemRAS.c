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

#include <SysHostChip.h>
#include "Chip10nm/Include/RcRegs10nm.h"
#include <Mem/Library/MemMcIpLib/Include/MemMcRegs.h>
#include <Library/MemMcIpLib.h>
#include <Library/KtiApi.h>
#include "Include/MemMcRegs.h"
#include "Include/MemMcChip.h"
#include "Include/MemMcIpLibInternal.h"
#include <UncoreCommonIncludes.h>
#include <Library/SystemInfoLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemRcLib.h>
#include <Library/MemSmeeIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemDecodeLib.h>
#include <Library/DebugLib.h>
#include <Library/RasRcDataLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/ProcMemInitTargetLib.h>
#include <Cpu/CpuIds.h>
#include <Library/MemTypeLib.h>
#include <Library/RasRcMirrorLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/PipeSyncLib.h>
#include <Register/Msr.h>
#include <Cpu/CpuCoreRegs.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Include/MemMapHost.h>

#define TAD0INDEX                                 0
#define MIN_TAD_GRAN                              2 //64MB units
#define PCUDATA_CLOCK_UNITS                       64
#define TOTAL_TIME_MEM_SCRUB                      95367
#define DAY_FACTOR                                10
#define DCLK_DAY_FACTOR                           100
#define DCLK_TIME_UNIT                            8
#define STATIC_VLS                                0
#define ADAPTIVE_VLS                              1
#define TAD_MIRRORED                              1
#define TAD_NON_MIRRORED                          2
#define ECC_MODE_REG_LIST      {ECC_MODE_RANK0_MC_MAIN_REG, ECC_MODE_RANK1_MC_MAIN_REG, ECC_MODE_RANK2_MC_MAIN_REG, ECC_MODE_RANK3_MC_MAIN_REG, ECC_MODE_RANK4_MC_MAIN_REG, ECC_MODE_RANK5_MC_MAIN_REG, ECC_MODE_RANK6_MC_MAIN_REG, ECC_MODE_RANK7_MC_MAIN_REG}
#define RRD_RCB_CLOCK_GATE_DISABLE                BIT29
#define RT_RCB_CLOCK_GATE_DISABLE                 BIT31
#define SCRUB_INTERVAL_CHANNELS_PER_MC            3 // includes fuse-disabled channels

typedef struct {
  UINT8  dimm;
  UINT8  rank;
}SparedRank;

//
// Structure that associates various representations of DDR speed and their corresponding
// DDRT link fail values based on design inputs
//
typedef struct {
  UINT16  LinkErrFlowTime;  // Holds the tick amount (specified in DCLKs) for the error observation window
  UINT8   Window;           // Number of clock cycles for observing
  } DdrtFreqLinkFail;

//
// The table map different DDR frequency and their corresponding
// DDRT link fail values based on design inputs in ascending order
//
                                      //    Freq(MT/s) Tick Time  # cycles
DdrtFreqLinkFail  mDdrFreqLinkFail[] = { {/*DDR_800*/     186,       38},
                                         {/*DDR_1000*/    232,       38},
                                         {/*DDR_1066*/    248,       38},
                                         {/*DDR_1200*/    279,       39},
                                         {/*DDR_1333*/    310,       39},
                                         {/*DDR_1400*/    325,       39},
                                         {/*DDR_1600*/    372,       39},
                                         {/*DDR_1800*/    419,       39},
                                         {/*DDR_1866*/    434,       39},
                                         {/*DDR_2000*/    465,       39},
                                         {/*DDR_2133*/    496,       39},
                                         {/*DDR_2200*/    512,       40},
                                         {/*DDR_2400*/    558,       40},
                                         {/*DDR_2600*/    605,       40},
                                         {/*DDR_2666*/    620,       40},
                                         {/*DDR_2800*/    651,       40},
                                         {/*DDR_2933*/    682,       40},
                                         {/*DDR_3000*/    698,       40},
                                         {/*DDR_3200*/    745,       40},
                                         {/*DDR_3400*/    791,       40},
                                         {/*DDR_3466*/    806,       40},
                                         {/*DDR_3600*/    838,       40},
                                         {/*DDR_3733*/    869,       40},
                                         {/*DDR_3800*/    884,       40},
                                         {/*DDR_4000*/    931,       40},
                                         {/*DDR_4200*/    977,       40},
                                         {/*DDR_4266*/    993,       40},
                                         {/*DDR_4400*/   1024,       41},
                                         {/*DDR_4800*/   1117,       41},
                                         {/*DDR_5200*/   1210,       41},
                                         {/*DDR_5600*/   1303,       41},
                                         {/*DDR_6000*/   1396,       41},
                                         {/*DDR_6400*/   1490,       41},
                                         {/*DDR_8400*/   1955,       41} };

//
// Local Prototypes
//
STATIC UINT8
CheckSparingPopulation (
  PSYSHOST Host,
  UINT8    socket
  );

BOOLEAN
EFIAPI
IsErrorRecoveryCap (
  VOID
  );


/**

  Find out the spare ranks per channel, and sets the rank size in rankList

  @param Host        - Pointer to sysHost
  @param socket        - Socket number
  @param ch          - Channel number (0-based)
  @retval N/A

**/
STATIC VOID
SetRankSizeForSpare(
                   PSYSHOST Host,
                   UINT8    socket,
                   UINT8    ch
                   )
{
  UINT8               spareRankNum;
  UINT8               rank;
  UINT16              size;
  UINT8               dimm;
  UINT8               k;
  UINT8               j;
  SparedRank          spareLogicalRank[MAX_SPARE_RANK];
  UINT8               sparedimm;
  UINT8               sparerank;
  UINT8               chRankEnabled = 0;
  UINT16              sparesize;
  struct ddrRank      *rankNvList0;
  struct dimmNvram    *dimmNvList0;
  struct channelNvram *channelNvList;
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();

  channelNvList = &(*GetChannelNvList(Host, socket))[ch];

  //
  // Find how many ranks need to be spared (must be <= 50% of available ranks)
  //
  for (dimm = 0; (dimm < channelNvList->maxDimm && dimm < MAX_DIMM); dimm++) {
    dimmNvList0 = &(*GetDimmNvList(Host, socket, ch))[dimm];
    if (dimmNvList0->dimmPresent == 0) {
      continue;
    }
    if (dimmNvList0->DcpmmPresent == 1) {
      continue;
    }

    for (rank = 0; (rank < dimmNvList0->numRanks && rank < MAX_RANK_DIMM); rank++) {
      rankNvList0 = &channelNvList->dimmList[dimm].rankList[rank];
      if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].mapOut[rank] == 0) {
        chRankEnabled++;
      }
    }
  }

  //
  // Max 2 spare ranks per channel
  //
  spareRankNum = chRankEnabled / 2;

  if(Setup->mem.spareRanks < spareRankNum) {
    spareRankNum = Setup->mem.spareRanks;
  }

  RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "SpareRankNum:%d, SetupSpareNum: %d, ch Ranks: %d, chRankEnabled: %d\n",
                spareRankNum, Setup->mem.spareRanks, channelNvList->numRanks, chRankEnabled);

  for (j = 0; j < MAX_SPARE_RANK; j++) {
    spareLogicalRank[j].dimm = 0xff;
    spareLogicalRank[j].rank = 0xff;
  }

  //
  // search all DIMM and ranks to find spare ranks.
  //
  for (k = 0; k < spareRankNum; k++) {

    sparedimm     = 0xff;
    sparerank     = 0xff;
    sparesize     = 0;

    for (dimm = 0; (dimm < channelNvList->maxDimm && dimm < MAX_DIMM); dimm++) {

      dimmNvList0 = &(*GetDimmNvList(Host, socket, ch))[dimm];

      if (dimmNvList0->dimmPresent == 0) {
        continue;
      }

      //
      // MC Rank Sparing is for DDR4 ranks only, skip NVM DIMM/ranks
      //
      if (dimmNvList0->DcpmmPresent == 1) {
        continue;
      }

      //
      // Correct rank size for RAS mode
      // Loop for each rank
      //
      for (rank = 0; (rank < dimmNvList0->numRanks && rank < MAX_RANK_DIMM); rank++) {
        rankNvList0 = &channelNvList->dimmList[dimm].rankList[rank];

        //
        // Skip ranks that are mapped out
        //
        if (CheckRank(Host, socket, ch, dimm, rank, CHECK_MAPOUT)) {
          continue;
        }

        //
        // If this rank has been selected, go ahead.
        //
        for (j = 0; j < MAX_SPARE_RANK; j++) {
          if ((spareLogicalRank[j].dimm == dimm) && (spareLogicalRank[j].rank == rank)) {
            break;
          }
        }

        //
        // This rank has been logged, skip it.
        //
        if (j < MAX_SPARE_RANK) {
          continue;
        }

        size = rankNvList0->rankSize;

        if (size > sparesize) {
          sparesize = size;
          sparedimm = dimm;
          sparerank = rank;
        }
      } // rank loop
    } // dimm loop

    if (sparedimm != 0xff) {
      spareLogicalRank[k].dimm = sparedimm;
      spareLogicalRank[k].rank = sparerank;
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     " spared dimm and rank:0x%x  0x%x \n", sparedimm, sparerank);
    }
  } //spare rank loop


  for (k = 0; k < (MAX_SPARE_RANK); k++) {
    //
    // Spare is rank on largest dimm. If more than one largest choose rank 0 on the minimum largest dimm
    //
    if (spareLogicalRank[k].dimm == 0xff) {
      channelNvList->spareDimm[k]  = 0xff;
      channelNvList->spareRankSize[k] = 0;
      channelNvList->spareLogicalRank[k] = 0xff;
      channelNvList->sparePhysicalRank[k] = 0xff;
      continue;
    }
    sparedimm                 = spareLogicalRank[k].dimm;
    sparerank                 = spareLogicalRank[k].rank;
    rankNvList0               = &channelNvList->dimmList[sparedimm].rankList[sparerank];
    channelNvList->spareDimm[k]  = sparedimm;
    channelNvList->spareRank[k]  = sparerank;
    channelNvList->spareRankSize[k] = rankNvList0->rankSize;
    channelNvList->spareLogicalRank[k] = rankNvList0->logicalRank;
    channelNvList->sparePhysicalRank[k] = rankNvList0->phyRank;
    rankNvList0->rankSize     = 0;
    rankNvList0->remSize      = 0;


    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Spare DIMM %d, Spare logical rank %d, Spare phy rank id %d, ch spare logical rank:0x%x \n",
                   sparedimm, rankNvList0->logicalRank, rankNvList0->phyRank,
                   channelNvList->spareLogicalRank[k]
                  );
  } // spare rank loop

}


/**

  Sets the rank size in rankList

  @param Host        - Pointer to sysHost
  @param socket        - Socket number
  @param ch          - Channel number (0-based)
  @param sizeFactor  - Size factor based on the RAS mode
  @param RASMode     - Current RAS mode

  @retval N/A

**/
STATIC VOID
SetRankSize (
            PSYSHOST Host,
            UINT8    socket,
            UINT8    ch,
            UINT8    sizeFactor,
            UINT8    RASMode
            )
{

  UINT8               i;
  UINT16              size;
  UINT8               dimm;

  struct ddrRank      *rankNvList0;
  struct dimmNvram    *dimmNvList0;
  struct channelNvram *channelNvList;


  channelNvList = &(*GetChannelNvList(Host, socket))[ch];

  //
  // skip if channel doesn't exist
  //
  if (!channelNvList->enabled) {
    return ;
  }

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    dimmNvList0 = &(*GetDimmNvList(Host, socket, ch))[dimm];

    if (dimmNvList0->dimmPresent == 0) {
      continue;
    }

    //
    // Correct rank size for RAS mode
    // Loop for each rank
    //
    for (i = 0; i < dimmNvList0->numDramRanks; i++) {
      rankNvList0 = &channelNvList->dimmList[dimm].rankList[i];

      //
      // Skip ranks that are mapped out
      //
      if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].mapOut[i] == 1) {
        continue;
      }

      //
      // Determine rank size
      //
      size = rankNvList0->rankSize;

      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, i, NO_STROBE, NO_BIT,
                     "size %d\nTechIndex 0x%x, size 0x%x\n", size, dimmNvList0->techIndex, rankSizeDDR4[dimmNvList0->techIndex]);
      //
      // Correct for RAS mode
      //
      rankNvList0->rankSize = sizeFactor * size;
      rankNvList0->remSize  = sizeFactor * size;
    } // rank loop
  } // dimm loop
}
/**

  Verifes the the rank cfg for SVLS setup..

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval Success if cfg is supported
          UnSupported if any issue with rank setup.

**/
UINT32
CheckSvlsCfgSupport (
  IN  PSYSHOST Host,
  IN  UINT8    socket
  )
{
  UINT8 dimm = 00;
  UINT8 rank = 00;
  UINT8 ch = 00;
  UINT8 X4PresentFlag = 00;
  UINT32  Status = SUCCESS;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (Host->nvram.mem.socket[socket].channelList[ch].enabled == 0) {
      continue; // Channel not enabled, skip it
    }

    //
    // no svls support for x4 rank.
    //
    if ((Host->nvram.mem.socket[socket].channelList[ch].features & X4_PRESENT)== X4_PRESENT) {
      X4PresentFlag = 1;
    }

    if ((Host->nvram.mem.socket[socket].channelList[ch].dimmList[0].dimmPresent == 1) &&
        (Host->nvram.mem.socket[socket].channelList[ch].dimmList[1].dimmPresent == 1)) {

#ifdef DDRT_SUPPORT
      if ((Host->nvram.mem.socket[socket].channelList[ch].dimmList[0].DcpmmPresent) || (Host->nvram.mem.socket[socket].channelList[ch].dimmList[1].DcpmmPresent)) {
        //both dimms are present and one of them is AEP  - cannot enable VLS here
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                           "CheckSvlsCfgSupport: Both PMem and DDR4 dimm on same channel:%d No VLS\n",ch);
        Status = FAILURE;
        break;
      }
#endif // DDRT_SUPPORT

      //
      //both dimms populated, if ranks are not equal or rank map out return
      //
      dimm = 00;
      if ((Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].numRanks) !=
        (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm + 1].numRanks)) {
        return FAILURE;
      }

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        //
        // if rank is map out return
        //
        if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList[rank].enabled) {
          if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].mapOut[rank]) {
            return FAILURE;
          }
        }
        if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm+1].rankList[rank].enabled) {
          if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm+1].mapOut[rank]) {
            return FAILURE;
          }
        }
      } // rank loop
    } else {

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        // single dimm case check rank is not map out and minimum 2 ranks exist.

        if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].dimmPresent == 0) {
          continue;
        }

#ifdef DDRT_SUPPORT
        if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].DcpmmPresent == 1) {
          if(X4PresentFlag) {
            X4PresentFlag = 0;
          }
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "CheckSvlsCfgSupport: PMem dimm in this channel:%d skipping the channel\n",ch);
          break; //This is a single dimm case if one of the Slot is aep then ignore that sVLS enabled on ch with ddr4dimm
        }
#endif // DDRT_SUPPORT

        if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].numRanks <= 1) {
          return FAILURE;
        }
        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].rankList[rank].enabled) {
            if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[dimm].mapOut[rank]) {
              return FAILURE;   // return if rank is map out
            }
          }
        } // rank loop
      } // dimm loop
    }

    if (X4PresentFlag) {
      return FAILURE;
    }
  } // ch loop
  return Status;
}

/**

  @param Host        - Pointer to sysHost
  @param socket        - Socket number
  @param VLS_MODE     - Which VLS Mode are we checking the populatin for SVLS or Adaptive VLS

  @retval CAPABLE     - Population meets requirements or not

**/
UINT8
checkVLSpopulation (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    VlsMode
  )
{
  UINT8  ch;
  struct channelNvram *ChannelNvList;
  UINT8   capable = RAS_IS_CAPABLE;
  UINT8   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  if (VlsMode == STATIC_VLS) { //Static VLS only x8 dimms in system
    if ((CheckSvlsCfgSupport (Host, Socket)) != EFI_SUCCESS) {
      RcDebugPrint (SDBG_MAX,
                     "\t sVLS is not support supported\n");
      capable = RAS_NOT_CAPABLE;
    }
  } else if (VlsMode == ADAPTIVE_VLS) { // Adaptive VLS  only x4 dimms in the system
    for (ch = 0; ch < MaxChDdr; ch++) {
      ChannelNvList = &Host->nvram.mem.socket[Socket].channelList[ch];

      if (ChannelNvList->enabled == 0) {
        continue;
      }

      if ((ChannelNvList->features & X8_PRESENT)==X8_PRESENT) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "\t No SDDC/ADDDC because of X8 Dimms \n");
        capable = RAS_NOT_CAPABLE;
      }
    }
  }

  return capable;
}

/**
  Validate RAS policy.

  Check the requested RAS input policy for errors, and correct them. Compute
  the supported RAS modes based on the CPU and memory population, and commit
  the set of modes that are both requested and supported as the current active
  mode setting.

  @param[in, out] Host      A pointer to the SysHost structure.
**/
VOID
ValidateRasPolicy (
  IN OUT SYSHOST *Host
  )
{
  CONST RAS_RC_POLICY     *Policy;
  RAS_RC_CONFIG           *Config;
  UINT8                   SetupRasModes;
  UINT16                  SetupRasModesEx;
  UINT8                   SupportedRasModes;
  UINT16                  SupportedRasModesEx;
  UINT8                   MissingRasModes;
  UINT16                  MissingRasModesEx;
  UINT8                   Socket;
  CAPID1_PCU_FUN3_STRUCT  PcuCapid1;
  CAPID3_PCU_FUN3_STRUCT  PcuCapid3;
  CAPID5_PCU_FUN3_STRUCT  PcuCapid5;
  SYS_SETUP               *Setup;

  Setup = GetSysSetupPointer ();
  ASSERT (Setup != NULL);
  if (Setup == NULL) {
    return;
  }
  if (IsHvmModeEnable ()) {
    return;
  }

  Config = GetRasRcConfig ();
  ASSERT (Config != NULL);
  if (Config == NULL) {
    return;
  }

  //
  // Capture the input policy and correct it to conform to restrictions.
  //
  Policy = GetRasRcPolicy ();
  ASSERT (Policy != NULL);
  if (Policy == NULL) {
    SetupRasModes = 0;
    SetupRasModesEx = 0;
  } else {
    SetupRasModes = Policy->RasModes;
    SetupRasModesEx = Policy->RasModesEx;
  }

  RcDebugPrint (SDBG_MAX, "RAS modes requested from input policy (setup):\n");
  RcDebugPrint (SDBG_MAX, "  Modes:    0x%02x\n", SetupRasModes);
  RcDebugPrint (SDBG_MAX, "  Modes EX: 0x%02x\n", SetupRasModesEx);

  //
  // Sparing and mirror are mutually exclusive: should not both be set in setup
  // If both set, turn off mirroring.
  //
  if ((SetupRasModes & CH_MS) == CH_MS) {
    RcDebugPrint (SDBG_MAX, "Sparing is enabled: Disable Full Mirroring\n");
    SetupRasModes &= ~(FULL_MIRROR_1LM | FULL_MIRROR_2LM);
  }

  RcDebugPrint (SDBG_MAX, "RAS modes requested after policy correction:\n");
  RcDebugPrint (SDBG_MAX, "  Modes:    0x%02x\n", SetupRasModes);
  RcDebugPrint (SDBG_MAX, "  Modes EX: 0x%02x\n", SetupRasModesEx);

  //
  // Assume the maximum supported capability and then reduce as appropriate.
  //
  SupportedRasModes = (
    FULL_MIRROR_1LM |
    FULL_MIRROR_2LM |
    STAT_VIRT_LOCKSTEP |
    RK_SPARE |
    PARTIAL_MIRROR_1LM |
    PARTIAL_MIRROR_2LM
    );
  SupportedRasModesEx = (
    PTRLSCRB_EN |
    DMNDSCRB_EN |
    SDDC_EN |
    ADDDC_EN |
    PCLS_EN |
    ADDDC_ERR_INJ_EN |
    PTRLSCRB_EOP_EN
    );

  //
  // Check for correctly configured DIMMs on each socket.
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Skip disabled sockets and sockets with no DIMMs.
    //
    if (Host->nvram.mem.socket[Socket].enabled == 0) {
      continue;
    }
    if ((Host->nvram.mem.socket[Socket].maxDimmPop == 0) && (!IsHbmSku ())) {
      continue;
    }

    RcDebugPrint (SDBG_MAX, "Checking Socket %d...\n", Socket);

    //
    // Read socket SKU capabilities.
    //
    PcuCapid1.Data = UsraCsrRead (Socket, 0, CAPID1_PCU_FUN3_REG);
    PcuCapid3.Data = UsraCsrRead (Socket, 0, CAPID3_PCU_FUN3_REG);
    PcuCapid5.Data = UsraCsrRead (Socket, 0, CAPID5_PCU_FUN3_REG);

    RcDebugPrint (SDBG_MAX, "CAPID1_PCU_FUN3 = 0x%08x\n", PcuCapid1.Data);
    RcDebugPrint (SDBG_MAX, "CAPID3_PCU_FUN3 = 0x%08x\n", PcuCapid3.Data);
    RcDebugPrint (SDBG_MAX, "  .disable_sparing            = %d\n", PcuCapid3.Bits.disable_sparing);
    RcDebugPrint (SDBG_MAX, "  .disable_patrol_scrub       = %d\n", PcuCapid3.Bits.disable_patrol_scrub);
    RcDebugPrint (SDBG_MAX, "CAPID5_PCU_FUN3 = 0x%08x\n", PcuCapid5.Data);
    RcDebugPrint (SDBG_MAX, "  .cap_mirror_ddr4_en         = %d\n", PcuCapid5.Bits.cap_mirror_ddr4_en);
    RcDebugPrint (SDBG_MAX, "  .cap_mirror_ddrt_en         = %d\n", PcuCapid5.Bits.cap_mirror_ddrt_en);
    RcDebugPrint (SDBG_MAX, "  .addr_based_mem_mirror      = %d\n", PcuCapid5.Bits.addr_based_mem_mirror);

    //
    // Verify socket full mirroring capabilities.
    //
    if (PcuCapid5.Bits.cap_mirror_ddr4_en == 0) {
      SupportedRasModes &= ~FULL_MIRROR_1LM;
    }
    if (PcuCapid5.Bits.cap_mirror_ddrt_en == 0) {
      SupportedRasModes &= ~FULL_MIRROR_2LM;
    }

    //
    // Verify socket address based mirroring capabilities.
    //
    if (PcuCapid5.Bits.addr_based_mem_mirror == 1) {
      if (PcuCapid5.Bits.cap_mirror_ddr4_en == 0) {
        SupportedRasModes &= ~PARTIAL_MIRROR_1LM;
        //
        // If the platform has AEP dimm, FULL_MIRROR_1LM is technically Address based mirror for DDR4 address range
        // and so addr_based_mem_mirror capability is needed to support this.
        //
        if (Host->nvram.mem.DcpmmPresent) {
          SupportedRasModes &= ~FULL_MIRROR_1LM;
        }
      }
      if (PcuCapid5.Bits.cap_mirror_ddrt_en == 0) {
        SupportedRasModes &= ~PARTIAL_MIRROR_2LM;
      }
    } else {
      SupportedRasModes &= ~(PARTIAL_MIRROR_1LM | PARTIAL_MIRROR_2LM);
      //
      // If the platform has AEP dimm, FULL_MIRROR_1LM is technically Address based mirror for DDR4 address range
      // and so addr_based_mem_mirror capability is needed to support this.
      //
      if (Host->nvram.mem.DcpmmPresent) {
        SupportedRasModes &= ~FULL_MIRROR_1LM;
      }
    }

    //
    // Verify socket rank sparing capability. If it is capable, also check
    // the DIMM population.
    //
    if ((PcuCapid3.Bits.disable_sparing == 1)) {
      SupportedRasModes &= ~RK_SPARE;
    } else if (CheckSparingPopulation (Host, Socket) == RAS_NOT_CAPABLE) {
      RcDebugPrint (SDBG_MAX, "DIMM population: Rank Sparing is unsupported!\n");
      SupportedRasModes &= ~RK_SPARE;
    }

    //
    // Verify socket patrol scrub capability.
    //
    if ((PcuCapid3.Bits.disable_patrol_scrub == 1) || (IsSiliconWorkaroundEnabled ("S22010267887"))) {
      SupportedRasModesEx &= ~(PTRLSCRB_EN | PTRLSCRB_EOP_EN);
    }

    //
    // Check the required population for Static Virtual Lockstep on the socket.
    //
    if (checkVLSpopulation (Host, Socket, STATIC_VLS) == RAS_NOT_CAPABLE) {
      RcDebugPrint (SDBG_MAX, "DIMM population: sVLS is unsupported!\n");
      SupportedRasModes &= ~STAT_VIRT_LOCKSTEP;
    }

    //
    // Disable SDDC plus one if in Independent mode with X8 DIMMs.
    //
    if (checkVLSpopulation (Host, Socket, ADAPTIVE_VLS) == RAS_NOT_CAPABLE) {
      RcDebugPrint (SDBG_MAX, "DIMM population: ADDDC is unsupported!\n");
      SupportedRasModesEx &= ~ADDDC_EN;
      SupportedRasModesEx &= ~ADDDC_ERR_INJ_EN;
      if ((SupportedRasModes & STAT_VIRT_LOCKSTEP) == 0) {
        RcDebugPrint (SDBG_MAX, "DIMM population: SDDC+1 is unsupported!\n");
        SupportedRasModesEx &= ~SDDC_EN;
      }
    }

    //
    // If Mirroring is enabled, we disable ADDDC
    //
    if ((SetupRasModes & SupportedRasModes) & CH_ALL_MIRROR) {
      RcDebugPrint (SDBG_MAX, "Warning! Cross Product not supported: Mirror enabled, Disabling Adddc and +1\n");
      SupportedRasModesEx &= ~ADDDC_EN;
      SupportedRasModesEx &= ~SDDC_EN;
      SupportedRasModesEx &= ~ADDDC_ERR_INJ_EN;
    }

    RcDebugPrint (SDBG_MAX, "Socket %d done\n", Socket);
  } // Socket

  //
  // Disable patrol scrub and demand scrub if ECC is not supported.
  //
  if (Host->nvram.mem.eccEn == 0) {
    RcDebugPrint (SDBG_MAX, "No ECC: Patrol Scrub is unsupported!\n");
    RcDebugPrint (SDBG_MAX, "No ECC: Demand Scrub is unsupported!\n");
    SupportedRasModesEx &= ~(PTRLSCRB_EN | DMNDSCRB_EN | PTRLSCRB_EOP_EN);
  }

  //
  // s4929717: CRB BIOS and Unified Stack with SuperSKU impacts Isoc feature
  //
  if (IfFeatureEnabled (TypeIsoc)) {
    RcDebugPrint (SDBG_MAX, "Isoc enabled: Mirroring is unsupported!\n");
    RcDebugPrint (SDBG_MAX, "Isoc enabled: Rank Sparing is unsupported!\n");
    SupportedRasModes &= ~(FULL_MIRROR_1LM | FULL_MIRROR_2LM | RK_SPARE | PARTIAL_MIRROR_1LM | PARTIAL_MIRROR_2LM);
  }

  //
  // Done computing supported modes.
  //
  RcDebugPrint (SDBG_MAX, "RAS modes supported by CPU/memory population:\n");
  RcDebugPrint (SDBG_MAX, "  Modes:    0x%02x\n", SupportedRasModes);
  RcDebugPrint (SDBG_MAX, "  Modes EX: 0x%02x\n", SupportedRasModesEx);

  Config->SupportedRasModes = SupportedRasModes;
  Config->SupportedRasModesEx = SupportedRasModesEx;

  //
  // Commit the final RAS modes.
  //
  Host->nvram.mem.RASmode = SetupRasModes & SupportedRasModes;
  Host->nvram.mem.RASmodeEx = SetupRasModesEx & SupportedRasModesEx;

  RcDebugPrint (SDBG_MAX, "Final RAS modes (requested & supported):\n");
  RcDebugPrint (SDBG_MAX, "  Modes:    0x%02x\n", Host->nvram.mem.RASmode);
  RcDebugPrint (SDBG_MAX, "  Modes EX: 0x%02x\n", Host->nvram.mem.RASmodeEx);

  //
  // Determine which modes were enabled per policy, but not supported, and
  // output warnings.
  //
  MissingRasModes = SetupRasModes & ~SupportedRasModes;
  MissingRasModesEx = SetupRasModesEx & ~SupportedRasModesEx;

  if ((MissingRasModes & FULL_MIRROR_1LM) != 0 || (MissingRasModes & FULL_MIRROR_2LM) != 0) {
    OutputWarning (WARN_MIRROR_DISABLE, WARN_MIRROR_DISABLE_MINOR_RAS_DISABLED, 0xFF, 0xFF, 0xFF, 0xFF);
  }

  if ((MissingRasModes & PARTIAL_MIRROR_1LM) != 0 || (MissingRasModes & PARTIAL_MIRROR_2LM) != 0) {
    OutputWarning (WARN_PMIRROR_DISABLE, WARN_PMIRROR_DISABLE_MINOR_RAS_DISABLED, 0xFF, 0xFF, 0xFF, 0xFF);
  }

  if ((MissingRasModes & RK_SPARE) != 0) {
    OutputWarning (WARN_SPARE_DISABLE, WARN_SPARE_DISABLE_MINOR_RK_SPARE, 0xFF, 0xFF, 0xFF, 0xFF);
  }

  if ((MissingRasModes & STAT_VIRT_LOCKSTEP) != 0) {
    OutputWarning (WARN_LOCKSTEP_DISABLE, WARN_LOCKSTEP_DISABLE_MINOR_RAS_MODE, 0xFF, 0xFF, 0xFF, 0xFF);
  }

  if ((MissingRasModesEx & PTRLSCRB_EN) != 0) {
    OutputWarning (WARN_PTRLSCRB_DISABLE, WARN_PTRLSCRB_MINOR_DISABLE, 0xFF, 0xFF, 0xFF, 0xFF);
  }

  if ((MissingRasModesEx & ADDDC_EN) != 0) {
    OutputWarning (WARN_ADDDC_DISABLE, WARN_ADDDC_MINOR_DISABLE, 0xFF, 0xFF, 0xFF, 0xFF);
  }

  if ((MissingRasModesEx & SDDC_EN) != 0) {
    OutputWarning (WARN_SDDC_DISABLE, WARN_SDDC_MINOR_DISABLE, 0xFF, 0xFF, 0xFF, 0xFF);
  }
} // ValidateRasPolicy

/**

  Initialize rank structures.  This is based on the requested
  RAS mode and what RAS modes the configuration supports. This
  routine also evalues the requested RAS modes to ensure they
  are supported by the system configuration.

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
RASCheckDimmRanks (
                  PSYSHOST Host
                  )
{
  UINT8 socket;
  UINT8 ch;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ValidateRasPolicy (Host);

  //
  // Set RANK size for selected mode, includes size accounting for the spare rank size
  //

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    if (IsSocketDimmPopulated (Host, socket) != TRUE) {
      continue;
    }

    //
    // Set rank size in each ddr channel (MaxChDdr will cover all IMCs)
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      SetRankSize (Host, socket, ch, 1, Host->nvram.mem.RASmode);
    }
  } // socket loop

  return SUCCESS;
}

UINT32
CheckRASSupportAfterMemInit (
  PSYSHOST Host
  )
/*++

Routine Description:

  Initialize rank structures.  This is based on the requested
  RAS mode and what RAS modes the configuration supports. This
  routine also evalues the requested RAS modes to ensure they
  are supported by the system configuration.

Arguments:

  Host  - Pointer to sysHost

Returns:

  SUCCESS

--*/
{

  UINT8                     dimm;
  UINT8                     rank;
  UINT8                     ch;
  UINT8                     socket;
  UINT32                    dimmAmap;
  AMAP_MC_MAIN_STRUCT       imcAMAPReg;
  DIMMMTR_0_MC_MAIN_STRUCT  dimmMtr;
  struct channelNvram       (*channelNvList)[MAX_CH];
  struct dimmNvram          (*dimmNvList)[MAX_DIMM];
  UINT8                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;

  channelNvList = GetChannelNvList(Host, socket);

  //
  // Update dimmmtr.rank_disable setting on mapout results after memtest
  //
  CheckRankPopLocal (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList(Host, socket, ch);
    dimmAmap = 0;

    //
    // If more than 1+ rank is mapped out on a channel use AMAP dimmX_pat_rank_disable to disable scrubbing
    //
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      dimmMtr.Data = MemReadPciCfgEp (socket, ch, GetDimmMtrRegOffset (dimm));

      for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {

        // Disable Patrol Scrub when ranks are mapped out and rank_disable is 0

        if ((*dimmNvList)[dimm].mapOut[rank] && ((dimmMtr.Bits.rank_disable & (1 << rank)) == 0)) {

          dimmAmap |= (1 << (dimm * 8 + rank));

          // Update AMAP register
          imcAMAPReg.Data = MemReadPciCfgEp (socket, ch, AMAP_MC_MAIN_REG);
          imcAMAPReg.Bits.dimm0_pat_rank_disable |= (UINT8)(dimmAmap & 0xff);
          imcAMAPReg.Bits.dimm1_pat_rank_disable |= (UINT8)((dimmAmap >> 8) & 0xff);
          MemWritePciCfgEp (socket, ch, AMAP_MC_MAIN_REG,imcAMAPReg.Data);
        }

#ifdef DDR5_SUPPORT
        //
        // BRS maps each ICX channel to DDR5 subchannel, so divide rank size by 2
        //
        if (IsBrsPresent (Host, socket)) {
          (*dimmNvList)[dimm].rankList[rank].rankSize >>= 1;
        }
#endif

      }// rank
    }// dimm
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              "DimmAmap: 0x%x\n", dimmAmap);
  } // ch

  //
  // Check for spare support after training
  //
  if (Host->nvram.mem.RASmode & RK_SPARE) {
    //
    // Adjust for spare ranksize and set spare ranks
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      SetRankSizeForSpare(Host, socket, ch);
    }
  }

  return SUCCESS;
}

/**

  SetVLSModePerChannel Configure on the fly burst mode for Static Virtual Lockstep mode

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
VOID
SetVLSModePerChannel(
  PSYSHOST Host,
  UINT8    socket
)
{
  UINT8                                   ch;
  UINT8                                   roundTripDiff;
  UINT8                                   MinRoundTrip;
  UINT8                                   MaxRoundTrip;
  struct channelNvram                     (*channelNvList)[MAX_CH];
  BootMode                                SysBootMode;
  UINT8                                   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  SysBootMode = GetSysBootMode ();

  if (((Host->nvram.mem.RASmode & STAT_VIRT_LOCKSTEP) == STAT_VIRT_LOCKSTEP)) {

    RcDebugPrint (SDBG_MAX,
                   "\n<SetVLSModePerChannel(socket = %d)>\n", socket);
    channelNvList = GetChannelNvList(Host, socket);

    if (SysBootMode == NormalBoot) {
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          RcDebugPrint (SDBG_MAX,
                         "\nch=%d Channel not enabled, skipping it \n", ch);
          continue;            // Channel not enabled, does not disable sVLS though
        }

        //
        // The copy of wait4bothhalves in mcchan_amap should be set to match the one in defeatures0 for ICX
        //
        if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL)  || IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
          SetM2mDefeatures0Wait4BothHalves (socket, ch/MAX_MC_CH, TRUE);
        }
        McSetWaitForBothHalves (socket, ch/MAX_MC_CH, ch%MAX_MC_CH);

        MinRoundTrip = 0;
        MaxRoundTrip = 0;
        //
        // GetMinMaxRoundTrip will not modify MinRoundTrip or MaxRoundTrip if there are no DDR4 DIMMs in the channel
        //
        GetMinMaxRoundTrip (Host, socket, ch, &MinRoundTrip, &MaxRoundTrip);
        roundTripDiff = MaxRoundTrip - MinRoundTrip;

      } // ch loop
    } // NormalBoot
    RcDebugPrint (SDBG_MAX,
                   "\n</SetVLSModePerChannel>\n");
  // Is sVLS enabled
  }

  channelNvList = GetChannelNvList(Host, socket);
  if (SysBootMode == NormalBoot) {
    RcDebugPrint (SDBG_MAX,
                   "\n Program drainrttimer\n");
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          RcDebugPrint (SDBG_MAX,
                         "\nch=%d Channel not enabled \n", ch);
          continue;            // Channel not enabled
        }

        SetM2mBiosQuiesceRtTimer (socket, (ch/MAX_MC_CH), 0x78);
      }
  }

  if( (Host->nvram.mem.RASmodeEx & ADDDC_EN) == ADDDC_EN) {
    RcDebugPrint (SDBG_MAX,
                   "\t ADDDC enabled\n", socket);
    channelNvList = GetChannelNvList(Host, socket);

    if (SysBootMode == NormalBoot) {
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          RcDebugPrint (SDBG_MAX,
                         "\nch=%d Channel not enabled, skipping it \n", ch);
           continue;            // Channel not enabled, does not disable sVLS though
        }

        MinRoundTrip = 0;
        MaxRoundTrip = 0;
        //
        // GetMinMaxRoundTrip will not modify MinRoundTrip or MaxRoundTrip if there are no DDR4 DIMMs in the channel
        //
        GetMinMaxRoundTrip (Host, socket, ch, &MinRoundTrip, &MaxRoundTrip);
        roundTripDiff = MaxRoundTrip - MinRoundTrip;
      } // ch loop
    } // if (Host->var.common.bootMode == NormalBoot)
    RcDebugPrint (SDBG_MAX,
                   "\n</ADDDC enabled>\n");
  } // if LS enabled in setup
  return;
} //SetVLSModePerChannel

/**

  Configure Lockstep mode

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
VOID
SetVLSRegionPerChannel (
  PSYSHOST Host,
  UINT8    socket
)
{
  UINT8                      Ch;
  struct channelNvram        (*channelNvList)[MAX_CH];
  ADDDC_REGION0_CONTROL_MC_MAIN_STRUCT  RegCtrl;
#if (MEM_IP_VER == MEM_IP_TODO)
  MCMTR_MC_MAIN_STRUCT       McMtrMain;
#endif // (MEM_IP_VER == MEM_IP_TODO)
  UINT8                      RegionNum;
  UINT8                      RankOffset = 00;
  UINT8                      RankLoop = 00;
  UINT8                      MaxChDdr;
  UINT32                     AddcRegionRegs[]  = {ADDDC_REGION0_CONTROL_MC_MAIN_REG,
                                                  ADDDC_REGION1_CONTROL_MC_MAIN_REG,
                                                  ADDDC_REGION2_CONTROL_MC_MAIN_REG,
                                                  ADDDC_REGION3_CONTROL_MC_MAIN_REG};
  if ((Host->nvram.mem.RASmode & STAT_VIRT_LOCKSTEP) != STAT_VIRT_LOCKSTEP) {
    return;
  }

  //
  // Configure Lockstep mode
  //
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "\n<SetVLSRegionPerChannel>\n");

  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "\n<SetVLSRegionPerChannel(Enable:%d)>\n", Host->nvram.mem.socket[socket].channelList[Ch].enabled);
    // sVLS is enabled on system-wide level.
    // Ignore disabled channels though
    if ((*channelNvList)[Ch].enabled == 0) {
      continue;            // Channel not enabled, skip it
    }

    if ((Host->nvram.mem.socket[socket].channelList[Ch].dimmList[0].dimmPresent == 1) &&
        (Host->nvram.mem.socket[socket].channelList[Ch].dimmList[1].dimmPresent == 1) ) {
      RankOffset = 04;
      RankLoop = (*channelNvList)[Ch].dimmList[0].numRanks;
    } else {
      RankOffset = (*channelNvList)[Ch].dimmList[0].numRanks / 2;
      RankLoop = (*channelNvList)[Ch].dimmList[0].numRanks / 2;
    }

    RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "<SetVLSRegionPerChannel>\n");

#if (MEM_IP_VER == MEM_IP_TODO)
    McMtrMain.Data = MemReadPciCfgEP (socket, Ch, MCMTR_MC_MAIN_REG);
    McMtrMain.Bits.adddc_mode = 1;
    MemWritePciCfgEp (socket, Ch, MCMTR_MC_MAIN_REG, McMtrMain.Data);
    RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "<MCMTR_MC_MAIN.adddc_mode = %d>\n",McMtrMain.Bits.adddc_mode);
#endif // (MEM_IP_VER == MEM_IP_TODO)
    for(RegionNum=0;RegionNum<(*channelNvList)[Ch].dimmList[0].numRanks;RegionNum++) {

      RegCtrl.Data = MemReadPciCfgEp (socket, Ch, AddcRegionRegs[RegionNum]);
      RegCtrl.Bits.region_enable = 1;
      RegCtrl.Bits.region_size = ADDDC_REGION_SIZE_RANK;
      RegCtrl.Bits.failed_cs = RegionNum;
      RegCtrl.Bits.failed_c = 0;
      RegCtrl.Bits.failed_ba = 0;
      RegCtrl.Bits.failed_bg = 0;
      RegCtrl.Bits.nonfailed_cs = RegionNum + 0x4;
      RegCtrl.Bits.nonfailed_c = 0;
      RegCtrl.Bits.nonfailed_ba = 0;
      RegCtrl.Bits.nonfailed_bg = 0;
      RegCtrl.Bits.copy_in_progress = 0;
      MemWritePciCfgEp (socket, Ch, AddcRegionRegs[RegionNum], RegCtrl.Data);

      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "\n <ADDDC_REGION%d_CONTROL.region_enable = %d>\n",RegionNum, RegCtrl.Bits.region_enable);
      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<ADDDC_REGION%d_CONTROL.failed_cs = %d>\n",RegionNum, RegCtrl.Bits.failed_cs);
      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<ADDDC_REGION%d_CONTROL.failed_c = %d>\n",RegionNum, RegCtrl.Bits.failed_c);
      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<ADDDC_REGION%d_CONTROL.failed_ba = %d>\n",RegionNum, RegCtrl.Bits.failed_ba);
      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<ADDDC_REGION%d_CONTROL.failed_bg = %d>\n",RegionNum, RegCtrl.Bits.failed_bg);
      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<ADDDC_REGION%d_CONTROL.nonfailed_cs = %d>\n",RegionNum, RegCtrl.Bits.nonfailed_cs);
      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<ADDDC_REGION%d_CONTROL.nonfailed_c = %d>\n",RegionNum, RegCtrl.Bits.nonfailed_c);
      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<ADDDC_REGION%d_CONTROL.nonfailed_ba = %d>\n",RegionNum, RegCtrl.Bits.nonfailed_ba);
      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<ADDDC_REGION%d_CONTROL.nonfailed_bg = %d>\n",RegionNum, RegCtrl.Bits.nonfailed_bg);
      RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "<ADDDC_REGION%d_CONTROL.copy_in_progress = %d>\n",RegionNum, RegCtrl.Bits.copy_in_progress);
    }

    RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "</SetVLSRegionPerChannel Ch>\n");
  } // mcId loop

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Lockstep enabled\n");
  RcDebugPrintWithDevice (SDBG_MAX, socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "</SetVLSRegionPerChannel>\n");
  return;
} //SetVLSRegionPerChannel


/**

  Configure MC Link Fail feature

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/

STATIC VOID
SetMCLinkFail (
    PSYSHOST Host,
    UINT8    socket
    )
{
  UINT8  Ch;
  UINT8  Imc;
  UINT8  SktCh;
  LINK_LINK_FAIL_MCDDC_DP_STRUCT                LinkLinkFailMcddc;
  LINK_MCA_CTL_MCDDC_DP_STRUCT                  LinkMcaCtlMcddc;
  LINK_RETRY_TIMER_MCDDC_DP_STRUCT              LinkRetryTimerMcddc;
  LINK_RETRY_ERR_LIMITS_MCDDC_DP_STRUCT         LinkRetryErrLimits;
  DDRT_RETRY_LINK_FAIL_MC_2LM_STRUCT            DdrtRetryLinkFail;
  SYS_SETUP                                     *Setup;
  UINT8                                         MaxImc;


  Setup = GetSysSetupPointer ();
  MaxImc = GetMaxImc ();

  //
  // Enable Link Fail/Retry/Retry Error Limits for each IMC
  //
  for (Imc = 0; Imc < MaxImc; Imc++) {

    // SKip if IMC not enabled
    if (Host->nvram.mem.socket[socket].imc[Imc].enabled == 0) {
      continue;
    }

    // Check all channels for current IMC
    for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
      // Looping over all the imcs calculate channel number for socket
      SktCh = NODECH_TO_SKTCH(Imc, Ch);

      // Skip if any channel is not populated
      if (Host->nvram.mem.socket[socket].channelList[SktCh].enabled == 0) {
        continue;
      }

      RcDebugPrint (SDBG_MAX,
                     "\n ===========Socket[%d] - SktCh[%d]===========\n", socket, SktCh);

      LinkMcaCtlMcddc.Data = MemReadPciCfgEp (socket, SktCh, LINK_MCA_CTL_MCDDC_DP_REG);
      LinkMcaCtlMcddc.Bits.err0_en   = 1;
      LinkMcaCtlMcddc.Bits.err0_log  = 1;
      MemWritePciCfgEp (socket, SktCh, LINK_MCA_CTL_MCDDC_DP_REG, LinkMcaCtlMcddc.Data);
      RcDebugPrint (SDBG_MAX,
                     "\nReading back LinkMcaCtlMcddc data\nerr0_en:%d\nerr0_log:%d\nerr0_smi:%d\n",
                         LinkMcaCtlMcddc.Bits.err0_en,
                         LinkMcaCtlMcddc.Bits.err0_log,
                         LinkMcaCtlMcddc.Bits.err0_smi);
      //
      // Update Link Fail cfg
      //
      LinkLinkFailMcddc.Data = MemReadPciCfgEp (socket, SktCh, LINK_LINK_FAIL_MCDDC_DP_REG);
      LinkLinkFailMcddc.Bits.fail_threshold = 1;
      MemWritePciCfgEp (socket, SktCh, LINK_LINK_FAIL_MCDDC_DP_REG, LinkLinkFailMcddc.Data);
      RcDebugPrint (SDBG_MAX,
                     "\n Reading back LinkLinkFailMcddc.Bits.fail_threshold: %d\n", LinkLinkFailMcddc.Bits.fail_threshold);

      //
      // Update Link Retry Timer cfg
      //
      LinkRetryTimerMcddc.Data = MemReadPciCfgEp (socket, SktCh, LINK_RETRY_TIMER_MCDDC_DP_REG);
      LinkRetryTimerMcddc.Bits.link_fail_observation_time = 31; // value provided by HW Design
      LinkRetryTimerMcddc.Bits.sb_err_observation_time    = 31; // value provided by HW Design
      LinkRetryTimerMcddc.Bits.link_err_flow_time_unit    = mDdrFreqLinkFail[Host->nvram.mem.socket[socket].ddrFreq].LinkErrFlowTime; // value based on table
      MemWritePciCfgEp (socket, SktCh, LINK_RETRY_TIMER_MCDDC_DP_REG, LinkRetryTimerMcddc.Data);
      RcDebugPrint (SDBG_MAX,
                     "\n Reading back LinkRetryTimerMcddc Data \nlink_fail_observation_time: %d\nsb_err_observation_time: %d\nlink_err_flow_time_unit: %d\nDDR Freq: %d",
                          LinkRetryTimerMcddc.Bits.link_fail_observation_time,
                          LinkRetryTimerMcddc.Bits.sb_err_observation_time,
                          LinkRetryTimerMcddc.Bits.link_err_flow_time_unit,
                          Host->nvram.mem.socket[socket].ddrFreq);

      //
      // Update Link Retry Error Limits cfg
      //
      LinkRetryErrLimits.Data = MemReadPciCfgEp (socket, SktCh, LINK_RETRY_ERR_LIMITS_MCDDC_DP_REG);
      LinkRetryErrLimits.Bits.sb_err_limit = 5;
      if ((Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN)) {
        LinkRetryErrLimits.Bits.sb_err_enable = 1;
      }
      MemWritePciCfgEp (socket, SktCh, LINK_RETRY_ERR_LIMITS_MCDDC_DP_REG, LinkRetryErrLimits.Data);
      RcDebugPrint (SDBG_MAX,
                     "\n Reading back LinkRetryErrLimits Data\nsb_err_limit: %d\nsb_err_enable: %d\n",
                          LinkRetryErrLimits.Bits.sb_err_limit,
                          LinkRetryErrLimits.Bits.sb_err_enable);
      //
      // Update Retry link fail
      //
      DdrtRetryLinkFail.Data = MemReadPciCfgEp (socket, SktCh, DDRT_RETRY_LINK_FAIL_MC_2LM_REG);
      if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
        DdrtRetryLinkFail.Bits.res_threshold = 0;  // Disable the link fail condition for ICX
      } else {
        DdrtRetryLinkFail.Bits.res_threshold = 5;
      }
      DdrtRetryLinkFail.Bits.window = mDdrFreqLinkFail[Host->nvram.mem.socket[socket].ddrFreq].Window; // value based on table
      MemWritePciCfgEp (socket, SktCh, DDRT_RETRY_LINK_FAIL_MC_2LM_REG, DdrtRetryLinkFail.Data);
    } // Ch
  } // Imc
}

/**

  Configure Mirror mode

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC VOID
ConfigMirrorMode (
    PSYSHOST Host,
    UINT8    socket
    )
{
  UINT8   SadIndex;
  UINT8   TadIndex;
  UINT8   Imc;
  UINT8   ch;
  UINT8   SktCh;
  UINT8   Ddr4ChnlFailed;
  UINT8   DdrtChnlFailed;

  RASENABLES_MC_MAIN_STRUCT         RasEnables;
  TADBASE_0_MC_MAIN_STRUCT          mcTADBase;
  AMAP_MC_MAIN_STRUCT               AmapMain;
  MCNMCACHINGCFG_MC_MAIN_STRUCT     McNmCachingCfg;
  BOOLEAN                           Pcommitforcebroadcast;
  BOOLEAN                           Immediatefailoveractionena;
  BOOLEAN                           MirrorDdr4;
  BOOLEAN                           MirrorDdrt;
  BOOLEAN                           SkipBadChnlForPwr;
  UINT8                             Ddr4Chnl0Sec;
  UINT8                             Ddr4Chnl1Sec;
  UINT8                             Ddr4Chnl2Sec;
  UINT8                             DdrtChnl0Sec;
  UINT8                             DdrtChnl1Sec;
  UINT8                             DdrtChnl2Sec;
  UINT8                             MaxImc;
  UINT8                             NumChPerMc;
  MEMORY_MAP_HOST                   *MemMapHost;
  UINT8                             ClusterId;
  UINT8                             BeginSadIndex;
  UINT8                             EndSadIndex;

  UINT32 TADBase[TAD_RULES] =
    { TADBASE_0_MC_MAIN_REG, TADBASE_1_MC_MAIN_REG, TADBASE_2_MC_MAIN_REG, TADBASE_3_MC_MAIN_REG,
      TADBASE_4_MC_MAIN_REG, TADBASE_5_MC_MAIN_REG, TADBASE_6_MC_MAIN_REG, TADBASE_7_MC_MAIN_REG } ;

  struct SADTable   *SAD;
  struct TADTable   *TAD;
  SYS_SETUP         *Setup;
  UINT8             MaxChDdr;

  MaxChDdr   = GetMaxChDdr ();
  Setup      = GetSysSetupPointer ();
  MaxImc     = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();

  Pcommitforcebroadcast      = FALSE;
  Immediatefailoveractionena = FALSE;
  MirrorDdr4                 = FALSE;
  MirrorDdrt                 = FALSE;
  SkipBadChnlForPwr          = FALSE;
  //
  // In 10nm generation, M2M is physically still a 3 channel design and needs programming for three channels for mirror
  // Channel 0 and 1 will be mirrored on each other and Channel 2 will have secondary channel as same as it is unpopulated
  //
  Ddr4Chnl0Sec               = 0;
  Ddr4Chnl1Sec               = 1;
  Ddr4Chnl2Sec               = 2;
  //
  // Todo: This will be cleaned up while removing all support for mirroring in 2LM memory mode
  //
  DdrtChnl0Sec               = 0;
  DdrtChnl1Sec               = 0;
  DdrtChnl2Sec               = 0;

  // Check if full mirroring (1LM ^ 2LM) is supported by hardware or enabled in BIOS setup

  RcDebugPrint (SDBG_MAX,
                 "\n<ConfigMirrorMode(socket = %d)>\n", socket);

  MemMapHost = GetMemMapHostPointer ();
  for (ClusterId = 0; ClusterId < MemMapHost->NumberofChaDramClusters; ClusterId++) {
    BeginSadIndex = ClusterId * MAX_SAD_RULES;
    EndSadIndex = BeginSadIndex + MAX_SAD_RULES;
    if (Host->var.mem.socket[socket].SAD[BeginSadIndex].Enable == 0) {
      break;
    }
    for (SadIndex = BeginSadIndex; SadIndex < EndSadIndex; SadIndex++) {
      SAD = &Host->var.mem.socket[socket].SAD[SadIndex];
      if(SAD->Enable == 0) {
        break;
      }
      if (!SAD->local) {
        continue;
      }

      //skip SAD that is not mirrored if its partial mirroring
      if(!SAD->mirrored) {
        RcDebugPrint (SDBG_MAX,
                      "\n ConfigMirrorMode:SAD not mirrored %d\n", SadIndex);
        continue;
      }

      //
      // Program Mirror Enables for each IMC
      //
      for (Imc = 0; Imc < MaxImc; Imc++) {

      // SKip if IMC not enabled
        if (Host->nvram.mem.socket[socket].imc[Imc].enabled == 0) {
          continue;
        }

        // Skip IMC that is not part of the SAD
        if ((SAD->imcInterBitmap & (1 << Imc)) == 0) {
          continue;
        }

        for(ch = 0; ch< MAX_MC_CH; ch++) {
          //Looping over all the imcs calculate channel number for socket
          SktCh = NODECH_TO_SKTCH(Imc,ch);
          //skip if any channel is not populated
          if (Host->nvram.mem.socket[socket].channelList[SktCh].enabled == 0) {
            continue;
          }

          for(TadIndex=0;TadIndex<TAD_RULES;TadIndex++){
            TAD = &Host->var.mem.socket[socket].imc[Imc].TAD[TadIndex];
            if(TAD->SADId==SadIndex) {
              if(TAD->Enable)
              {
                mcTADBase.Data = MemReadPciCfgEp (socket, SktCh, TADBase[TadIndex]);
                mcTADBase.Bits.mirror_en = 1;
                mcTADBase.Bits.ign_ptrl_uc = 1;
                MemWritePciCfgEp (socket, SktCh, TADBase[TadIndex], mcTADBase.Data);
                RcDebugPrint (SDBG_MAX,
                              "mcTADBase[%d].Bits.mirror_en:%d \n", TadIndex, mcTADBase.Bits.mirror_en);

                if (IsSiliconWorkaroundEnabled ("S1409866097")) {
                  if (mcTADBase.Bits.mirror_en == 1) {
                    DisableOpenPageMode (MemTechDdr, socket, SktCh);
                  }
                }
              }
            }
          }

          AmapMain.Data = MemReadPciCfgEp (socket, SktCh, AMAP_MC_MAIN_REG);
          AmapMain.Bits.force_lat = 1;
          MemWritePciCfgEp (socket, SktCh, AMAP_MC_MAIN_REG, AmapMain.Data);

          AmapMain.Data = MemReadPciCfgEp (socket, SktCh, AMAP_MC_MAIN_REG);
          RcDebugPrint (SDBG_MAX,
                          "\n Reading back AmapMain.Bits.force_lat %d\n", AmapMain.Bits.force_lat);
        }

        if (SAD->type == MemType1lmDdr) {
          RcDebugPrint (SDBG_MAX,
                        "\n ConfigMirrorMode:mirrorddr4 sad %d\n", SadIndex);

          switch (SAD->channelInterBitmap[Imc]) {
            case BITMAP_CH0_CH1:
              Ddr4Chnl0Sec = 1;
              Ddr4Chnl1Sec = 0;
              break;
            case BITMAP_CH1_CH2:
              Ddr4Chnl1Sec = 2;
              Ddr4Chnl2Sec = 1;
              break;
            case BITMAP_CH0_CH2:
              Ddr4Chnl0Sec = 2;
              Ddr4Chnl2Sec = 0;
              break;
            case BITMAP_CH0_CH1_CH2:
              Ddr4Chnl0Sec = 1;
              Ddr4Chnl1Sec = 2;
              Ddr4Chnl2Sec = 0;
              break;
            default:
              // control should never come here
              ;
          } // switch

          //
          // enable DDR4 mirror mode
          //
          MirrorDdr4 = TRUE;

          //
          // Program iMC RAS Enables to enable Mirror mode
          //
          for(ch = 0; ch< MAX_MC_CH; ch++)
          {
            SktCh = NODECH_TO_SKTCH(Imc,ch);
            if (Host->nvram.mem.socket[socket].channelList[SktCh].enabled == 0) {
              continue;
            }
            RasEnables.Data = MemReadPciCfgEp (socket, SktCh, RASENABLES_MC_MAIN_REG);
            RasEnables.Bits.ch0_mirror = Ddr4Chnl0Sec;
            RasEnables.Bits.ch1_mirror = Ddr4Chnl1Sec;
            RasEnables.Bits.ch2_mirror = Ddr4Chnl2Sec;
            RasEnables.Bits.mirroren = 1;
            MemWritePciCfgEp (socket, SktCh, RASENABLES_MC_MAIN_REG, RasEnables.Data);
          }

        } else if ((SAD->type == MemType2lmDdrCacheMemoryMode) || (SAD->type == MemType1lmAppDirect) || (SAD->type == MemType1lmAppDirectReserved)) {

          RcDebugPrint (SDBG_MAX,
                        "\n ConfigMirrorMode:mirrorddrt sad %d\n", SadIndex);
          for(ch = 0; ch< MAX_MC_CH; ch++)
          {
            SktCh = NODECH_TO_SKTCH(Imc,ch);
            if (Host->nvram.mem.socket[socket].channelList[SktCh].enabled == 0) {
              continue;
            }
            RasEnables.Data = MemReadPciCfgEp (socket, SktCh, RASENABLES_MC_MAIN_REG);
            RasEnables.Bits.ch0_mirror = Ddr4Chnl0Sec;
            RasEnables.Bits.ch1_mirror = Ddr4Chnl1Sec;
            RasEnables.Bits.ch2_mirror = Ddr4Chnl2Sec;
            RasEnables.Bits.mirroren = 1;
            MemWritePciCfgEp (socket, SktCh, RASENABLES_MC_MAIN_REG, RasEnables.Data);
          }
          //
          // enable NGN mirror mode
          //
          MirrorDdrt = TRUE;

          RcDebugPrint (SDBG_MAX,
                        "\n ClearMirrorMode:mirrordd4 sad %d\n", SadIndex);

          MirrorDdr4 = FALSE;

          // Program iMC RAS Enables to enable Mirror mode

          for(ch = 0; ch < MAX_MC_CH; ch++) {
            SktCh = NODECH_TO_SKTCH(Imc,ch);

            //skip if any channel is not populated
            if (Host->nvram.mem.socket[socket].channelList[SktCh].enabled == 0) {
              continue;
            }
            //
            // Program mcmirrormode per channel.
            //
            McNmCachingCfg.Data = MemReadPciCfgEp (socket, SktCh, MCNMCACHINGCFG_MC_MAIN_REG);
            McNmCachingCfg.Bits.mcmirrormode = 1;
            MemWritePciCfgEp (socket, SktCh, MCNMCACHINGCFG_MC_MAIN_REG, McNmCachingCfg.Data);

            AmapMain.Data = MemReadPciCfgEp (socket, SktCh, AMAP_MC_MAIN_REG);
            AmapMain.Bits.force_lat = 0;
            MemWritePciCfgEp (socket, SktCh, AMAP_MC_MAIN_REG, AmapMain.Data);

  #if (MEM_IP_VER == MEM_IP_TODO)
            Ddrt2lmMisc.Data = MemReadPciCfgEp (socket, SktCh, DDRT_MISC_CTL_MC_2LM_REG);
            Ddrt2lmMisc.Bits.ddrt_dec_force_lat = 1;
            MemWritePciCfgEp (socket, SktCh, DDRT_MISC_CTL_MC_2LM_REG, Ddrt2lmMisc.Data);
  #endif // (MEM_IP_VER == MEM_IP_TODO)
          }

        } // if (SAD->type)

        //
        //exras_config.corr2corroverwritedis should be 0 when mirroring is enabled
        //
        SetM2mExRasConfigCorr2CorroverwriteDis (socket, Imc, 0);
        SetM2mDefeatures0MirrorMode (socket, Imc, 1, 0, 1, 0); //Wait4BothHalves, EgrBypassDis, IngBypassDis, DemandScrubWrDis

        SkipBadChnlForPwr = GetM2mDefeatures0BadChRetry (socket, Imc);

        Immediatefailoveractionena = FALSE;

        SetM2mSysFeatures0ForMirror (
          socket,
          Imc,
          Pcommitforcebroadcast,
          Immediatefailoveractionena
          );

        SetM2mErrCountCtrl ( socket, Imc, TRUE, TRUE, TRUE, MIRRSCRUBRD_ERR_TYPE, MIRRSCRUBRD_ERR_TYPE);

        SetM2mMirrorChs(
          socket, Imc,
          (Ddr4Chnl0Sec | (Ddr4Chnl1Sec << CH1_SHIFT_COUNT) | (Ddr4Chnl2Sec << CH2_SHIFT_COUNT)),
          (DdrtChnl0Sec | (DdrtChnl1Sec << CH1_SHIFT_COUNT) | (DdrtChnl2Sec << CH2_SHIFT_COUNT))
          );

        SetM2mModeMirrorType (socket, Imc, MirrorDdr4, MirrorDdrt);

        RcDebugPrint (SDBG_MAX,
                      "M2M[%d].MODE.mirrorddr4 = %d\n", Imc, MirrorDdr4);
        RcDebugPrint (SDBG_MAX,
                      "M2M[%d].MODE.mirrorddrt = %d\n", Imc, MirrorDdrt);
        RcDebugPrint (SDBG_MAX,
                      "M2M[%d].ERR_CNTR_CTL_M2MEM_MAIN_REG =0x%x\n", Imc, GetM2mErrCountCtrl (socket, Imc));

        //
        // On S3 RESUME, set M2M fail over status from NVRAM
        //
        if (GetSysBootMode () == S3Resume || Host->var.mem.subBootMode == WarmBootFast) {
          Ddr4ChnlFailed = DdrtChnlFailed = 0;

          // Find ddr4 and ngn channels disabled due to UC (MFO)
          for (ch = 0; ch < MaxChDdr; ch++) {
            if (Host->nvram.mem.socket[socket].channelList[ch].enabled == 0) {
              continue;
            }

            if (Host->nvram.mem.socket[socket].channelList[ch].chFailed == 1) {
              Ddr4ChnlFailed |= (1 << (ch % NumChPerMc));
            }
            if (Host->nvram.mem.socket[socket].channelList[ch].ngnChFailed == 1) {
              DdrtChnlFailed |= (1 << (ch % NumChPerMc));
            }

          } // for (ch)

          SetM2mMirrorFailOver (socket, Imc, Ddr4ChnlFailed, DdrtChnlFailed);
        } // if (S3resume)

      } // Imc loop
    } // for (SadIndex)
  } // for (ClusterId = 0; ClusterId < MemMapHost->NumberofChaDramClusters; ClusterId++) {

  RcDebugPrint (SDBG_MAX,
                 "</ConfigMirrorMode>\n");
  return;
}

/**

  Configure Spare error threshold

  @param Host                    - Pointer to sysHost
  @param socket                    - Socket number
  @param ch                      - Channel number (0-based)
  @param imcCORRERRTHRSHLD01Reg  -
  @param ErrThreshold            -

  @retval N/A

**/
STATIC VOID
SetErrorThreshold (
                  PSYSHOST Host,
                  UINT8    socket,
                  UINT8    ch,
                  UINT32   ErrThreshold
                  )
{
  UINT8                               rank;
  UINT32          imcCORRERRTHRSHLD01Regs[] = {CORRERRTHRSHLD_0_MCDDC_DP_REG,
                                               CORRERRTHRSHLD_1_MCDDC_DP_REG,
                                               CORRERRTHRSHLD_2_MCDDC_DP_REG,
                                               CORRERRTHRSHLD_3_MCDDC_DP_REG};

  CORRERRTHRSHLD_0_MCDDC_DP_STRUCT  imcCORRERRTHRSHLD01;

  if (Host->nvram.mem.socket[socket].channelList[ch].enabled == 1) {
    //
    // Set Error Threshold
    //
    for (rank = 0; rank < (MAX_RANK_CH * SUB_CH); rank += 2) {
      if (rank >= 8) {
        break;
      }
      imcCORRERRTHRSHLD01.Data              = MemReadPciCfgEp (socket, ch, imcCORRERRTHRSHLD01Regs[rank/2]);
      //
      // first rank
      //
      imcCORRERRTHRSHLD01.Bits.cor_err_th_0 = ErrThreshold;
      //
      // second rank
      //
      imcCORRERRTHRSHLD01.Bits.cor_err_th_1 = ErrThreshold;
      MemWritePciCfgEp (socket, ch, imcCORRERRTHRSHLD01Regs[rank/2], imcCORRERRTHRSHLD01.Data);
    }
  }

  return;
}

/**

  5370684: CLONE SKX Sighting: [SKX B0 PO] [ADDDC x Partial Mirroring] System sees CECC's when doing device sparing
  If Mirroring is enabled then all channels in the systems should have atleast 2 Ranks. Else adddc is not supported

  @param Host  - Pointer to sysHost

  @retval  SUCCESS - if the system supports ADDDC and Mirroring

**/
STATIC UINT32
CheckAdddcMirrorSupport(
  PSYSHOST Host
 )
{
  UINT32 Status = SUCCESS;
  UINT8  Skt, Ch;
  UINT8  MaxChDdr;

 if ( ((Host->nvram.mem.RASmodeEx & ADDDC_EN) != ADDDC_EN) || ((Host->nvram.mem.RASmodeEx & SDDC_EN) != SDDC_EN) ) {
   return FAILURE;
 }

  MaxChDdr = GetMaxChDdr ();
  if (Host->nvram.mem.RASmode & CH_ALL_MIRROR) {
    for (Skt = 0; Skt < MAX_SOCKET; Skt++) {
      for (Ch = 0; Ch < MaxChDdr; Ch++) {
        if (Host->nvram.mem.socket[Skt].channelList[Ch].enabled) {
          if (Host->nvram.mem.socket[Skt].channelList[Ch].numRanks < 2) {
            RcDebugPrint (SDBG_MAX,
                           "\n System doesn't satisfy the ADDDC X Mirroring support \n");
            Host->nvram.mem.RASmodeEx &= ~(ADDDC_EN);
            Host->nvram.mem.RASmodeEx &= ~(SDDC_EN);
            return FAILURE;
          }
        }
      }
    }
  }

  return Status;
}

/**

  Configure ADDDC X Mirror mode

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/


VOID
SetADDDCMirrorMode (
    PSYSHOST Host,
    UINT8    socket
    )
{
  UINT8                                   Imc;
  UINT8                                   Ch;
  UINT8                                   SktCh;
  UINT8                                   TadIndex;
  UINT8                                   MirroredTadCount;
  UINT8                                   MinMirTadRule;
  UINT8                                   MaxMirTadRule;
  UINT8                                   MinNoMirTadRule;
  UINT8                                   MaxNoMirTadRule;
  UINT8                                   TadConfig[TAD_RULES];
  UINT8                                   Index1;
  UINT8                                   Index2;
  UINT8                                   MaxImc;

  SPARING_CONTROL_MC_MAIN_STRUCT               ImcSparingCtrl;
  AMAP_MC_MAIN_STRUCT                          ImcAmapMain;
  TADBASE_0_MC_MAIN_STRUCT                     ImcTadBase;
  TADWAYNESS_0_MC_MAIN_STRUCT                  ImcTadWayness;
  SPARING_CONTROL_TAD_MC_MAIN_STRUCT           ImcSparingControlTad;

  UINT32 TadBase[TAD_RULES] = {
    TADBASE_0_MC_MAIN_REG, TADBASE_1_MC_MAIN_REG, TADBASE_2_MC_MAIN_REG, TADBASE_3_MC_MAIN_REG,
    TADBASE_4_MC_MAIN_REG, TADBASE_5_MC_MAIN_REG, TADBASE_6_MC_MAIN_REG, TADBASE_7_MC_MAIN_REG
   };

    UINT32 TadWayness[TAD_RULES] = {
    TADWAYNESS_0_MC_MAIN_REG, TADWAYNESS_1_MC_MAIN_REG, TADWAYNESS_2_MC_MAIN_REG, TADWAYNESS_3_MC_MAIN_REG,
    TADWAYNESS_4_MC_MAIN_REG, TADWAYNESS_5_MC_MAIN_REG, TADWAYNESS_6_MC_MAIN_REG, TADWAYNESS_7_MC_MAIN_REG
  };

  MaxImc = GetMaxImc ();

  if (CheckAdddcMirrorSupport (Host) != SUCCESS) {
    return;
  }

  if (Host->nvram.mem.RASmode & FULL_MIRROR_1LM) {

    RcDebugPrint (SDBG_MAX,
      "Enabling ADDDC in Full Mirror mode\n");

    for (Imc = 0; Imc < MaxImc; Imc++) {
      if (Host->nvram.mem.socket[socket].imc[Imc].enabled == 0) {
        continue;
      }

      for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
        SktCh = NODECH_TO_SKTCH (Imc,Ch);
        if (Host->nvram.mem.socket[socket].channelList[SktCh].enabled == 0) {
          continue;
        }

        //
        // Programming Sparing_Control register
        //
        ImcSparingCtrl.Data = MemReadPciCfgEp (socket, SktCh, SPARING_CONTROL_MC_MAIN_REG);
        ImcSparingCtrl.Bits.mirr_adddc_en = 1;
        MemWritePciCfgEp (socket, SktCh, SPARING_CONTROL_MC_MAIN_REG, ImcSparingCtrl.Data);
        RcDebugPrint (SDBG_MAX,
          "ImcSparingCtrl.Data after is 0x%x\n", ImcSparingCtrl.Data);

        //
        // Programming AMAP register
        //
        ImcAmapMain.Data = MemReadPciCfgEp (socket, SktCh, AMAP_MC_MAIN_REG);
        ImcAmapMain.Bits.mirr_adddc_en = 1;
        MemWritePciCfgEp (socket, SktCh, AMAP_MC_MAIN_REG, ImcAmapMain.Data);
        RcDebugPrint (SDBG_MAX,
          "ImcAmapMain.Data after is 0x%x\n", ImcAmapMain.Data);

      }// Ch loop
    } // Imc for loop
  } // Full Mirroring loop

  if (Host->nvram.mem.RASmode & PARTIAL_MIRROR_1LM) { // remove the condition check for 2lm

      RcDebugPrint (SDBG_MAX,
                     " Enabling ADDDC in Partial Mirror mode\n");

      for (Imc = 0; Imc < MaxImc; Imc++) {
      if (Host->nvram.mem.socket[socket].imc[Imc].enabled == 0) {
        continue;
      }

      for (Ch = 0; Ch < MAX_MC_CH; Ch++) {
        SktCh = NODECH_TO_SKTCH (Imc,Ch);
        if (Host->nvram.mem.socket[socket].channelList[SktCh].enabled == 0) {
          continue;
        }

        // TODO: TAD Min Max values calculation may change if SPARING_CONTROL_TAD_MC_MAIN_REG
        // changed scope to channel level instead of Imc level

        //
        // Initialize the following variabls for each Imc channel.
        //
        MirroredTadCount = 0;
        MinMirTadRule    = 0xF;
        MinNoMirTadRule  = 0xF;
        MaxMirTadRule    = 0;
        MaxNoMirTadRule  = 0;

        for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex++) {
          TadConfig[TadIndex] = 0;
        }

        //
        // Parse all TADS for each Imc, find out which TAD is mirrored and which is not mirrored
        // Also get the minTadRule, MaxTadRule in Mirrored and NonMirrored case
        //

        for (TadIndex=0; TadIndex < TAD_RULES; TadIndex++) {

          ImcTadWayness.Data = MemReadPciCfgEp (socket, SktCh, TadWayness[TadIndex]);

          //
          // Tad is not enabled, if tad_limit is 0, Break out, since the next TADS will be not enabled as well
          //
          if (ImcTadWayness.Bits.tad_limit == 0) {
            break;
          }

          //
          // Read TadBase Register to check if the TAD is mirrored or not
          //
          ImcTadBase.Data = MemReadPciCfgEp (socket, SktCh, TadBase[TadIndex]);
          if (ImcTadBase.Bits.mirror_en == 1) {
            if (MinMirTadRule == 0xF) {
              MinMirTadRule=  TadIndex;
            }
            TadConfig[TadIndex] = TAD_MIRRORED;
            MirroredTadCount++;
            MaxMirTadRule = TadIndex;
          } else {
            if (MinNoMirTadRule == 0xF){
              MinNoMirTadRule=  TadIndex;
            }
            TadConfig[TadIndex] = TAD_NON_MIRRORED;
            MaxNoMirTadRule = TadIndex;
          }
        }

        for (TadIndex=0; TadIndex < TAD_RULES; TadIndex++) {
          RcDebugPrint (SDBG_MAX,
            "Tad 0x%x,  TadConfig[TadIndex] = 0x%x\n", TadIndex, TadConfig[TadIndex]);
        }

        //
        // MirroredTadCount > 0 says that Partial mirroring is enabled in the system,  Program the registers.
        //
        if (MirroredTadCount > 0) {

          //
          // Programming Sparing_Contorl register
          //
          ImcSparingCtrl.Data = MemReadPciCfgEp (socket, SktCh, SPARING_CONTROL_MC_MAIN_REG);
          ImcSparingCtrl.Bits.mirr_adddc_en = 1;
          ImcSparingCtrl.Bits.partial_mirr_en = 1;
          ImcSparingCtrl.Bits.minimum_tad_rule = MinMirTadRule;
          ImcSparingCtrl.Bits.maximum_tad_rule = MaxMirTadRule;
          ImcSparingCtrl.Bits.minimum_tad_rule_nonmirr = MinNoMirTadRule;
          ImcSparingCtrl.Bits.maximum_tad_rule_nonmirr = MaxNoMirTadRule;
          MemWritePciCfgEp (socket, SktCh, SPARING_CONTROL_MC_MAIN_REG, ImcSparingCtrl.Data);
          RcDebugPrint (SDBG_MAX,
            "ImcSparingCtrl.Data after is 0x%x\n", ImcSparingCtrl.Data);

          //
          // Programming AMAP register
          //
          ImcAmapMain.Data = MemReadPciCfgEp (socket, SktCh, AMAP_MC_MAIN_REG);
          ImcAmapMain.Bits.mirr_adddc_en = 1;
          MemWritePciCfgEp (socket, SktCh, AMAP_MC_MAIN_REG, ImcAmapMain.Data);
          RcDebugPrint (SDBG_MAX,
            "ImcAmapMain.Data after is 0x%x\n", ImcAmapMain.Data);

          //
          // Programming Sparing_Control_TAD register
          //
          ImcSparingControlTad.Data = MemReadPciCfgEp (socket, SktCh, SPARING_CONTROL_TAD_MC_MAIN_REG);

          for (Index1 = 0; Index1 < TAD_RULES; Index1++) {
            if (TadConfig[Index1] == 0) {
              break;
            }

            for (Index2 = Index1; Index2 < TAD_RULES; Index2++) {
              if (TadConfig[Index2] == 0) {
                break;
              }

              if (TadConfig[Index1] == TadConfig[Index2]) {
                switch (Index1) {
                  case 0:
                    ImcSparingControlTad.Bits.nxt_tad_0 = Index2;
                    break;
                  case 1:
                    ImcSparingControlTad.Bits.nxt_tad_1 = Index2;
                    break;
                  case 2:
                    ImcSparingControlTad.Bits.nxt_tad_2 = Index2;
                    break;
                  case 3:
                    ImcSparingControlTad.Bits.nxt_tad_3 = Index2;
                    break;
                  case 4:
                    ImcSparingControlTad.Bits.nxt_tad_4 = Index2;
                    break;
                  case 5:
                    ImcSparingControlTad.Bits.nxt_tad_5 = Index2;
                    break;
                  case 6:
                    ImcSparingControlTad.Bits.nxt_tad_6 = Index2;
                    break;
                  default:
                    //Automated add of default case. Please review.
                    break;
                }

                if (Index1 != Index2) {
                  break;
                }
              }
            }
          }

          MemWritePciCfgEp (socket, SktCh, SPARING_CONTROL_TAD_MC_MAIN_REG, ImcSparingControlTad.Data);
          RcDebugPrint (SDBG_MAX,
            "ImcSparingControlTad.Data after is 0x%x\n", ImcSparingControlTad.Data);
        }
      }
    }
  }
}

/**

  Configure Sparing on S3 resume
  Note: S3 / Sparing is not POR and may not be fully validated

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC VOID
S3SpareResume (
              PSYSHOST Host,
              UINT8    socket
              )
{
  return;
}


/**

  Configure Spare mode

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC VOID
SetSpareMode (
             PSYSHOST Host,
             UINT8    socket
             )
{
  UINT8   ch;
  UINT8   rankIndex;
  UINT8   physicalRank;
  UINT8   logicalRank;
  UINT32  DimmAmap;
  UINT8   Dimm = 0;
  UINT8   RankPerDimm;
  UINT8   mcId;
  SMISPARECTL_MC_MAIN_STRUCT                SmiSpareCtl;
  SPAREINTERVAL_MC_MAIN_STRUCT              imcSPAREINTERVAL;
#if (MEM_IP_VER == MEM_IP_TODO)
  SPARECTL_MC_MAIN_STRUCT                   imcSpareCtlReg;
  SPARING_MCDDC_CTL_STRUCT                  imcSparingReg;
#endif // (MEM_IP_VER == MEM_IP_TODO)
  struct channelNvram                       *channelNvList;
  AMAP_MC_MAIN_STRUCT                       AMAPReg;
  SCRATCHPAD2_MC_MAIN_STRUCT                ChScratchPad2;
  SYS_SETUP                                 *Setup;
  UINT8                                     MaxChDdr;
  UINT8                                     MaxImc;


  Setup = GetSysSetupPointer ();
  MaxImc = GetMaxImc ();

  //
  // Configure Spare mode
  //
  if ((Host->nvram.mem.RASmode & RK_SPARE) != RK_SPARE) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrint (SDBG_MAX,
                 "\n<SetSpareMode(socket = %d)>\n", socket);

  //
  // Program iMC for sparing
  //
  for (mcId = 0; mcId < MaxImc; mcId++) {
    if (Host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
      continue;
    }
    for (ch = 0; ch < MAX_MC_CH; ch++) {
      channelNvList = &(*GetChannelNvList(Host, socket))[ch];
      if (channelNvList->enabled == 0) {
        continue;
      }
      SmiSpareCtl.Data                 = MemReadPciCfgEp (socket, ch, SMISPARECTL_MC_MAIN_REG);
      SmiSpareCtl.Bits.intrpt_sel_cmci = 0;
      SmiSpareCtl.Bits.intrpt_sel_smi  = 0;
      SmiSpareCtl.Bits.intrpt_sel_pin  = 0;
      MemWritePciCfgEp (socket, ch,  SMISPARECTL_MC_MAIN_REG, SmiSpareCtl.Data);
    }
  }

#if (MEM_IP_VER == MEM_IP_TODO)
  for (ch = 0; ch < MaxChDdr; ch++) {
    imcSparingReg.Data = MemReadPciCfgEp (socket, ch, SPARING_MCDDC_CTL_REG);
    imcSparingReg.Bits.sparecrdts = 0x0F;
    imcSparingReg.Bits.wrfifohwm = 0x0; // HSD 4929575
    MemWritePciCfgEp (socket, ch, SPARING_MCDDC_CTL_REG, imcSparingReg.Data);
    imcSparingReg.Data = MemReadPciCfgEp (socket, ch, SPARING_MCDDC_CTL_REG);
  } // ch loop

  imcSpareCtlReg.Data = MemReadPciCfgMain (socket, SPARECTL_MC_MAIN_REG);
  imcSpareCtlReg.Bits.hafifowm = 0x02;
  imcSpareCtlReg.Bits.diswpqwm = 0x01;
  MemWritePciCfgMain (socket, SPARECTL_MC_MAIN_REG, imcSpareCtlReg.Data);
#endif // (MEM_IP_VER == MEM_IP_TODO)
  //
  // Set spare interval - to recommended value (from imc design)
  //
  imcSPAREINTERVAL.Data           = MemReadPciCfgMain (socket, SPAREINTERVAL_MC_MAIN_REG);
  imcSPAREINTERVAL.Bits.normopdur = Setup->mem.normOppIntvl;
  MemWritePciCfgMain (socket, SPAREINTERVAL_MC_MAIN_REG, imcSPAREINTERVAL.Data);

  if (GetSysBootMode () == S3Resume) {
    S3SpareResume (Host, socket);
  }

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Sparing configured\n");

  if ((Host->nvram.mem.RASmodeEx & PTRLSCRB_EN) ) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      channelNvList = &(*GetChannelNvList(Host, socket))[ch];
      if (channelNvList->enabled == 0) {
        continue;
      }

      // exclude spare ranks from patrol scrub
      DimmAmap = 0;
      for (rankIndex = 0; rankIndex <  MAX_SPARE_RANK; rankIndex ++) {
        physicalRank = channelNvList->sparePhysicalRank[rankIndex];
        if (physicalRank == 0xff) {
          continue;
        }
        Dimm = physicalRank / 4;
        RankPerDimm  = physicalRank % 4;
        DimmAmap |= (1 << (Dimm * 8 + RankPerDimm));
      } // rankIndex loop

      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "DimmAmap: 0x%x\n", DimmAmap);

      // update the AMAP
      AMAPReg.Data = MemReadPciCfgEp (socket, ch, AMAP_MC_MAIN_REG);
      AMAPReg.Bits.dimm0_pat_rank_disable |= (UINT8)(DimmAmap & 0xff);
      AMAPReg.Bits.dimm1_pat_rank_disable |= (UINT8)((DimmAmap >> 8) & 0xff);
      MemWritePciCfgEp (socket, ch, AMAP_MC_MAIN_REG,AMAPReg.Data);
      RcDebugPrint (SDBG_MAX,
                     "\n<ch=%d, AMAP.Data=0x%x>\n", ch, AMAPReg.Data);
    }
  }

  //
  // update scratch pad register
  // SCRATCHPAD2 in each DDR channel
  // bits 0-7     one-hot encoding of channel's spare ranks, each bit position represents a logical rank id
  // bits 8       sparing enabled
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    channelNvList = &(*GetChannelNvList(Host, socket))[ch];
    if (channelNvList->enabled == 0) {
      continue;
    }
      ChScratchPad2.Data = MemReadPciCfgEp (socket, ch, SCRATCHPAD2_MC_MAIN_REG);
      ChScratchPad2.Data &= 0xFFFFFF00; // clear LSB
      for (rankIndex = 0; rankIndex < MAX_SPARE_RANK; rankIndex ++) {
        logicalRank = channelNvList->spareLogicalRank[rankIndex];
        if (logicalRank == 0xff) {
          continue;
        }
        ChScratchPad2.Data |= (1 << logicalRank);
      } // rankIndex loop
      // if spare ranks available, set enable flag
      if (ChScratchPad2.Data & 0xFF) {
        ChScratchPad2.Data |= (1 << MAX_RANK_CH);
      }
      MemWritePciCfgEp (socket, ch, SCRATCHPAD2_MC_MAIN_REG, ChScratchPad2.Data);
      RcDebugPrint (SDBG_MAX,
                     "\n<ch=%d, SCRATCHPAD2.Data=0x%x>\n", ch, ChScratchPad2.Data);
    } // ch loop

  RcDebugPrint (SDBG_MAX,
                 "\n</SetSpareMode>\n");
  return ;
} // SetSpareMode

/**

  Program corrected error thresholds for all channels in socket

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC VOID
SetErrorThresholdNode (
                      PSYSHOST Host,
                      UINT8    socket
                      )
{
  UINT8                 ch;
  UINT32                ErrThreshold;
  SYS_SETUP             *Setup;
  UINT8                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  //
  // Set Error Threshold
  //
  ErrThreshold = Setup->mem.spareErrTh;

  if (ErrThreshold != 0) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      SetErrorThreshold (Host, socket, ch, ErrThreshold);
    }
  }
  return;
} //SetErrorThresholdNode

STATIC const UINT8 timingTable[MAX_SUP_FREQ] = {20, 17, 15, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 2, 1, 1};

/**

  Configure Patrol Scrubbing

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC VOID
SetPatrolScrub (
    PSYSHOST Host,
    UINT8    socket
)
{
  UINT8                             ChOnSkt;
  UINT8                             ChIndex;
  UINT8                             Dimm;
  UINT8                             mcId;
  UINT8                             TadIndex;
  UINT8                             SadIndex;
  UINT8                             MaxTadIndex;
  UINT8                             ScrubChMask;
  UINT32                            EnabledChNum;
  UINT32                            memSize;
  UINT32                            ScrubInterval;
  UINT32                            MissedThreshold;
  UINT32                            AdvancedThreshold;
  UINT32                            Pc6Duration = 128000000; //128 ms or 128000000 ns
  UINT32                            IdleDuration = 122400; //122.4 us or 122400000 ns
  UINT32                            ScrubFactor;
  SCRUBCTL_MC_MAIN_STRUCT           imcSCRUBCTL;
  SCRUBADDRESSHI_MC_MAIN_STRUCT     scrubAddrHi;
  SYSFEATURES0_MC_2LM_STRUCT        SysFeatures0Mc2lm;
  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT   dpCkhnBit;
  BOOLEAN                           FoundDdrDimm;
  UINT8                             MaxImc;
  MCMAIN_CHKN_BITS_MC_MAIN_STRUCT   McMainChknReg;

  struct channelNvram               (*channelNvList)[MAX_CH];
  struct ddrChannel                 (*channelList)[MAX_CH];
  struct dimmNvram                  (*DimmNvList)[MAX_DIMM];
  struct SADTable                   *SadEntry;
  BootMode                          SysBootMode;
  SYS_SETUP                         *Setup;
  RAS_RC_CONFIG                     *Config;
  UINT8                             MaxChDdr;
  BOOLEAN                           DisableScrubOnMc = FALSE;
  UINT64_STRUCT                     MsrData;
  UINT8                             SocketId;
  TAD_TABLE                         *TadEntry;
  UINT32                            VolIntlvSizePerCh;

  Setup = GetSysSetupPointer ();
  SysBootMode = GetSysBootMode ();
  MaxImc = GetMaxImc ();

  Config = GetRasRcConfig ();
  ASSERT (Config != NULL);
  if (Config == NULL) {
    return;
  }

  RcDebugPrint (SDBG_MINMAX,
                "SetPatrolScrub execution on Skt %x\n", socket);
  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr      = GetMaxChDdr ();

  //
  //In Mirroring mode ,poison bit should be set before enabling Patrol Scrup
  //
  if ((Host->nvram.mem.RASmode & CH_ALL_MIRROR) && IsErrorRecoveryCap ()) {
    RcDebugPrint (SDBG_MAX,
                       "Mirror mode detected; Enabling Poison \n");
    for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {
      if (SocketPresent (SocketId)) {
        MsrData = ReadMsrPipeHL (SocketId, MSR_MCG_CONTAIN);
        MsrData.lo = (MsrData.lo | BIT0);
        WriteMsrPipeHL (SocketId, MSR_MCG_CONTAIN, MsrData);
      }
    }
  }
  //
  // Enable patrol scrubs
  //
  if ((Host->nvram.mem.RASmodeEx & PTRLSCRB_EN) && (Host->var.mem.socket[socket].memSize != 0)) {

    for (mcId = 0; mcId < MaxImc; mcId++) {
      DisableScrubOnMc = FALSE;
      if (Host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
        continue;
      }
      EnabledChNum = 0;
      //
      // Equation to get a scrub to every line in 24 hours is..
      // (86400/(MEMORY CAPACITY/64))/CYCLE TIME of DCLK
      // Seconds it takes to scrub all of memory (86400 seconds in a day)
      //
      if ((Setup->mem.patrolScrubDuration != 0) && (Setup->mem.patrolScrubDuration <= PATROL_SCRUB_DURATION_MAX)) {
        ScrubFactor = 3600 * Setup->mem.patrolScrubDuration;
      } else {
        ScrubFactor = 86400;
      }

      MaxTadIndex = 0;
      for (TadIndex = 0; TadIndex < TAD_RULES; TadIndex ++) {
        TadEntry = &Host->var.mem.socket[socket].imc[mcId].TAD[TadIndex];
        if (TadEntry->Enable == 0) {
            break;
        }

        SadIndex = TadEntry->SADId;
        SadEntry = &Host->var.mem.socket[socket].SAD[SadIndex];

        //
        // Process all DDR TAD's. 2LM NM TAD is populated before 1LM FM TAD.
        //
        if ((SadEntry->type == MemType1lmDdr) || (SadEntry->type == MemType2lmDdrCacheMemoryMode && TadEntry->Limit == 0)) {
          MaxTadIndex++;
        }
      }

      for (ChIndex = 0; ChIndex < MAX_MC_CH; ChIndex++) {
        ChOnSkt = NODECH_TO_SKTCH (mcId, ChIndex);
        if ((*channelNvList)[ChOnSkt].enabled == 0) {
          continue;
        }
        //
        // Program System Physical Address (or) Reverse Address mode based on BIOS setup option
        //
        scrubAddrHi.Data = MemReadPciCfgEp (socket, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG);
        scrubAddrHi.Bits.ptl_sa_mode = Setup->mem.patrolScrubAddrMode;
        scrubAddrHi.Bits.minimum_tad_rule = 0;
        if (MaxTadIndex > 1) {
          scrubAddrHi.Bits.maximum_tad_rule = MaxTadIndex - 1;
        } else {
          scrubAddrHi.Bits.maximum_tad_rule = 0;
        }
        MemWritePciCfgEp (socket, ChOnSkt, SCRUBADDRESSHI_MC_MAIN_REG, scrubAddrHi.Data);
      }

      channelList = &Host->var.mem.socket[socket].channelList;
      memSize = 0;
      ScrubChMask = 0;
      for (ChIndex = 0; ChIndex < MAX_MC_CH; ChIndex++) {
        ChOnSkt = NODECH_TO_SKTCH (mcId, ChIndex);
        if ((*channelNvList)[ChOnSkt].enabled == 0) {
          continue;
        }
        EnabledChNum++;
        if ((*channelList)[ChOnSkt].mcId == mcId) {
          VolIntlvSizePerCh = GetChannelVolMemInterleaveSize (socket, ChOnSkt);
          if (VolIntlvSizePerCh > memSize) {
            memSize = VolIntlvSizePerCh;
          }
        }
      } // ch loop

      if (memSize) {
        //
        // Calculate the interval value to be programmed for Patrol Scrub.
        //
        // Scrub interval equation:
        // ScrubInterval = (ScrubFactor:86400s / memCachelineSize) * chnlsPerMc * 120)
        // ScrubInterval = (ScrubFactor * 1000000000) / (memSize * 2^20 (1M) * chnlsPerMc * 120)
        // ScrubInterval = (ScrubFactor / memSize) *  (1000000000 / 2^20 * chnlsPerMc * 120)
        // ScrubInterval = (ScrubFactor / memSize) * (7.947 / chnlsPerMc);
        //
        // Note that "chnlsPerMc" includes fuse-disabled channels.
        //
        // SPR patrol scrub interval calculation is based off the largest channel populated in an MC,
        // then multiply that rate by the number of enabled channels in the MC (1 or 2,).
        //
        if (!IsCpuAndRevision  (CPU_SPRSP, REV_ALL)) {
          EnabledChNum = SCRUB_INTERVAL_CHANNELS_PER_MC;
        }
        ScrubInterval = ((ScrubFactor / memSize) * 7947) / (1000 * EnabledChNum);

        RcDebugPrintWithDevice (SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "ScrubFactor: %d, MemSize: %x, Scrub Interval:%x\n", ScrubFactor, memSize, ScrubInterval);

        //
        // PCU interval has 32 bit width.
        //
        if (ScrubInterval > 0xFFFFFF){
          ScrubInterval = 0xFFFFFF;
        }

        //
        // If memSize is too big, the value will round to 0, under such case, set to 1.
        //
        if (ScrubInterval == 0){
          ScrubInterval = 1;
        }
      } else {
        ScrubInterval = 0;
      }

      if (ScrubInterval != 0) {
        //
        //  PC6 duration = 128ms, & 16b for missthresh
        //  _MissedThresh = (_PC6Duration/(ScrubInterval * 120 ns)) + 1
        //
        //
        MissedThreshold = (UINT32)(Pc6Duration / (ScrubInterval * 120)) + 1;
        if (MissedThreshold > 0xFFFF) {
          MissedThreshold = 0xFFFF;
        }
        PcuSetMissedThreshold (socket, mcId, MissedThreshold);

        //
        // IdleDuration = (idle detect time)* Guard Band = 6.12us * 20 = 122.4 us
        //  _AdvancedThreshold = (_IdleDuration/(_ScrubInterval)) + 1
        //
        AdvancedThreshold = (UINT32)(IdleDuration / (ScrubInterval * 120)) + 1;
        if (AdvancedThreshold > 0xFFFF) {
          AdvancedThreshold = 0xFFFF;
        }
        if (IsSiliconWorkaroundEnabled ("S1706993599")) {
          //Do NOT program Advanced threshold if WA is enabled
        } else {
          PcuSetAdvancedThreshold (socket, mcId, AdvancedThreshold);
        }
      }
      //
      // Enable scrub here only for non S3 resume. For S3 resume the setting is restored by S3BootScript
      //
      if (SysBootMode != S3Resume) {
        for (ChIndex = 0; ChIndex < MAX_MC_CH; ChIndex++) {
          ChOnSkt = NODECH_TO_SKTCH(mcId, ChIndex);
          if ((*channelNvList)[ChOnSkt].enabled == 0) {
            continue;
          }

          DimmNvList = GetDimmNvList (Host, socket, ChOnSkt);
          FoundDdrDimm = FALSE;
          for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
            if ((*DimmNvList)[Dimm].dimmPresent == 0) {
              continue;
            }
            if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
              FoundDdrDimm = TRUE;
              break;
            }
          }
          if (!FoundDdrDimm) {
            continue;
          }

          if (IsSiliconWorkaroundEnabled ("S1909312404")) {
            //
            // Disable patrol scrub when TME is enabled.
            //
            if (IsTmeEnabled (socket)) {
                DisableScrubOnMc = TRUE;
            }
          }
          if (IsSiliconWorkaroundEnabled ("S1909312388")) {
            //
            // Disable patrol scrub on any channel with a DDRT DIMM that supports both 2LM and app-direct (mixed mode).
            //
            for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
              if ((*DimmNvList)[Dimm].dimmPresent == 0) {
                continue;
              }
              if (((*DimmNvList)[Dimm].DcpmmPresent == 1) &&
                (Host->var.mem.volMemMode == VOL_MEM_MODE_2LM) &&
                (Host->var.mem.CacheMemType == CACHE_TYPE_DDR_CACHE_PMEM) &&
                ((*DimmNvList)[Dimm].volCap > 0) && ((*DimmNvList)[Dimm].nonVolCap > 0)) {
                DisableScrubOnMc = TRUE;
                break;
              }
            }
          }
          if (DisableScrubOnMc) {
            break;
          }

          if (!((Host->nvram.mem.RASmodeEx & PTRLSCRB_EOP_EN) == PTRLSCRB_EOP_EN)) {
            //
            //Set scrub_en here, only if scrub enable at end of post is not selected
            //
            imcSCRUBCTL.Data = MemReadPciCfgEp (socket, ChIndex + (mcId * MAX_MC_CH), SCRUBCTL_MC_MAIN_REG);
            imcSCRUBCTL.Bits.scrub_en = 1;
            MemWritePciCfgEp (socket, ChIndex + (mcId * MAX_MC_CH), SCRUBCTL_MC_MAIN_REG, imcSCRUBCTL.Data);
          }
        } // ch loop

        if (DisableScrubOnMc) {
          ScrubInterval = 0;
          for (ChIndex = 0; ChIndex < MAX_MC_CH; ChIndex++) {
            ChOnSkt = NODECH_TO_SKTCH(mcId, ChIndex);
            if ((*channelNvList)[ChOnSkt].enabled == 0) {
              continue;
            }

            imcSCRUBCTL.Data = MemReadPciCfgEp (socket, ChIndex + (mcId * MAX_MC_CH), SCRUBCTL_MC_MAIN_REG);
            if (imcSCRUBCTL.Bits.scrub_en) {
              imcSCRUBCTL.Bits.scrub_en = 0;
              MemWritePciCfgEp (socket, ChIndex + (mcId * MAX_MC_CH), SCRUBCTL_MC_MAIN_REG, imcSCRUBCTL.Data);
            }
          } // ch loop
        }

        //
        // Save patrol scrub interval for RAS runtime code.
        //
        Config->PatrolScrubInterval[socket][mcId] = ScrubInterval;

        PcuSetScrubInterval (socket, mcId, ScrubInterval);

        if (ScrubInterval) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "Patrol scrub enabled and started, interval value:%x\n", ScrubInterval);
        }
      }
    } // mcId loop
  } // if patrol scrubbing enabled

  //
  // Enable Patrol and Demand Scrub based on setup knobs
  //
  for (ChOnSkt = 0; ChOnSkt < ChOnSkt; ChOnSkt++) {
    if ((*channelNvList)[ChOnSkt].enabled == 0) {
      continue;
    }

    SysFeatures0Mc2lm.Data  = MemReadPciCfgEp (socket, ChOnSkt, SYSFEATURES0_MC_2LM_REG);

    if (Host->nvram.mem.RASmodeEx & DMNDSCRB_EN) {
      SysFeatures0Mc2lm.Bits.srubwr = 0;        //Set 0 to Enable Demand Scrub feature
    } else {
      SysFeatures0Mc2lm.Bits.srubwr = 1;        //Set 1 to Disable Demand Scrub feature
    }

    MemWritePciCfgEp (socket, ChOnSkt, SYSFEATURES0_MC_2LM_REG, SysFeatures0Mc2lm.Data);
  }

  //
  // Enable/Disable Demand Scrubs
  // Note: Hardware default is demand scrub enabled
  //
  if (Host->nvram.mem.RASmodeEx & DMNDSCRB_EN) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Demand scrub enabled\n");
  } else {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Demand scrub disabled\n");
  }


  //
  // Convert uncorrectable patrol scrub errors to correctable if mirror is enabled
  //
  if ((  (Host->nvram.mem.RASmode & FULL_MIRROR_1LM) == FULL_MIRROR_1LM ||
         (Host->nvram.mem.RASmode & FULL_MIRROR_2LM) == FULL_MIRROR_2LM ) &&
      (Host->nvram.mem.RASmodeEx & PTRLSCRB_EN)  ) {
      for (ChOnSkt = 0; ChOnSkt < MaxChDdr; ChOnSkt++) {
        if ((*channelNvList)[ChOnSkt].enabled == 0) {
          continue;
        }

        dpCkhnBit.Data = MemReadPciCfgEp (socket, ChOnSkt, MC0_DP_CHKN_BIT_MCDDC_DP_REG);
        dpCkhnBit.Bits.ign_ptrl_uc = 1;
        MemWritePciCfgEp (socket, ChOnSkt, MC0_DP_CHKN_BIT_MCDDC_DP_REG, dpCkhnBit.Data);
      } // ch loop
  } // if mirroring and patrol scrubbing enabled


  //
  // Scrub non-mirrored region only once if partial mirror is enabled
  //
  if (( (Host->nvram.mem.RASmode & PARTIAL_MIRROR_1LM) == PARTIAL_MIRROR_1LM ||
        (Host->nvram.mem.RASmode & PARTIAL_MIRROR_2LM) == PARTIAL_MIRROR_2LM ) &&
       (Host->nvram.mem.RASmodeEx & PTRLSCRB_EN)) {
        for (ChOnSkt = 0; ChOnSkt < MaxChDdr; ChOnSkt++) {
          if ((*channelNvList)[ChOnSkt].enabled == 0) {
           continue;
          }
        McMainChknReg.Data = MemReadPciCfgEp (socket, ChOnSkt, MCMAIN_CHKN_BITS_MC_MAIN_REG);
        McMainChknReg.Bits.pat_skip_non_mirr = 1;
        MemWritePciCfgEp (socket, ChOnSkt, MCMAIN_CHKN_BITS_MC_MAIN_REG, McMainChknReg.Data);
      }// ch loop
  }// if partial mirroring and patrol scrubbing enabled

} //SetPatrolScrub

/**

  Configure Leaky Bucket

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC VOID
SetLeakyBucketConfig (
  PSYSHOST Host,
  UINT8    socket
)
{
  LEAKY_BUCKET_CFG_MC_MAIN_STRUCT     imcLEAKY_BUCKET_CFG;
  SYS_SETUP                           *Setup;
  UINT64                              ddrFreq;
  UINT64                              seconds;
  UINT64                              counterThreshold;
  UINT64                              mssb;

  Setup = GetSysSetupPointer ();

  imcLEAKY_BUCKET_CFG.Data = MemReadPciCfgMain (socket, LEAKY_BUCKET_CFG_MC_MAIN_REG);

  if (Setup->mem.leakyBktTimeWindow) {
    //
    // Program leaky bucket rate through time window interface
    //
    RcDebugPrint (SDBG_MAX,
                  "Time window based memory leaky bucket\n");
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
         "Time window parameters: Day = %d, Minute = %d\n", Setup->mem.leakyBktHour, Setup->mem.leakyBktMinute);

    ddrFreq = MultU64x64 (GetDdrFreqInMhz (socket), 1000000);
    seconds = MultU64x32 (Setup->mem.leakyBktHour, 3600) + MultU64x32 (Setup->mem.leakyBktMinute, 60);
    counterThreshold = RShiftU64 (MultU64x64 (ddrFreq, seconds), 11);
    mssb = HighBitSet64 (counterThreshold);
    imcLEAKY_BUCKET_CFG.Bits.leaky_bkt_cfg_hi = (UINT32) mssb;
    imcLEAKY_BUCKET_CFG.Bits.leaky_bkt_cfg_lo = (UINT32) HighBitSet64 (counterThreshold & (~LShiftU64 (BIT0, (UINTN) mssb)));
  } else {
    //
    // Program leaky bucket rate
    // Set to a rate of about 1 leaky bucket pulse per 10 days at a dclk rate of 1GHz
    //
    RcDebugPrint (SDBG_MAX,
                  "Legacy leaky bucket\n");
    imcLEAKY_BUCKET_CFG.Bits.leaky_bkt_cfg_hi = Setup->mem.leakyBktHi;
    imcLEAKY_BUCKET_CFG.Bits.leaky_bkt_cfg_lo = Setup->mem.leakyBktLo;
  }
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "leaky_bkt_cfg_hi = %x\n", imcLEAKY_BUCKET_CFG.Bits.leaky_bkt_cfg_hi);
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "leaky_bkt_cfg_lo = %x\n", imcLEAKY_BUCKET_CFG.Bits.leaky_bkt_cfg_lo);
  MemWritePciCfgMain (socket, LEAKY_BUCKET_CFG_MC_MAIN_REG, imcLEAKY_BUCKET_CFG.Data);


  return;
}

/**
  Disable MC data parity checking.

  This function disables MC data parity checking as needed, such as due to silicon work-arounds.

  @param[in]  Host      Pointer to the SYSHOST structure.
  @param[in]  Socket    Socket number.
**/
VOID
DisableMcDataParityCheckAsNeeded (
  IN PSYSHOST Host,
  IN UINT8    Socket
)
{
  UINT8                                Ch;
  UINT8                                MaxChDdr;
  UINT32                               DisableDataParityChk;
  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT      DpChkn;
  SYS_SETUP                            *Setup;

  Setup = GetSysSetupPointer ();
  MaxChDdr = GetMaxChDdr ();

  DisableDataParityChk = 0; // Default is to enable the data parity checking

  //
  // When setup value is "AUTO", the default value of the "dis_ha_par_chk" depends on the
  // silicon WA and configurations.
  //
  if (Setup->mem.dfxMemSetup.DfxMcDataParityCheck == MC_DATA_PARITY_CHECK_AUTO) {
    if (IsSiliconWorkaroundEnabled ("S2208918713") && (CheckSysConfig (SYS_CONFIG_1S) == FALSE)) {
      DisableDataParityChk = 1;
    }
  } else if (Setup->mem.dfxMemSetup.DfxMcDataParityCheck == MC_DATA_PARITY_CHECK_DISABLE){
    DisableDataParityChk = 1;
  }

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    DpChkn.Data = MemReadPciCfgEp (Socket, Ch, MC0_DP_CHKN_BIT_MCDDC_DP_REG);
    DpChkn.Bits.dis_ha_par_chk = DisableDataParityChk;
    MemWritePciCfgEp (Socket, Ch, MC0_DP_CHKN_BIT_MCDDC_DP_REG, DpChkn.Data);

    RcDebugPrintWithDevice ( SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Disable MC data parity checking = %d\n", DpChkn.Bits.dis_ha_par_chk );
  }
}

/**
  Disable clock gating.

  This function disables clock gates as needed, such as due to silicon work-arounds.

  @param[in]  Host      Pointer to the SYSHOST structure.
  @param[in]  Socket    Socket number.
**/
VOID
DisableClockGatingAsNeeded (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  LINK_CFG_READ_1_MCDDC_DP_STRUCT LinkCfgRead;
  UINT8                           Channel;
  UINT8                           MaxChDdr;

  if (!IsSiliconWorkaroundEnabled ("S1706854367") &&
      !IsSiliconWorkaroundEnabled ("S1408782269") &&
      !IsSiliconWorkaroundEnabled ("S14011218130"))
  {
    return;
  }

  ASSERT (Host != NULL);
  ASSERT (Socket < MAX_SOCKET);
  if (Host == NULL || Socket >= MAX_SOCKET) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (Host->nvram.mem.socket[Socket].channelList[Channel].enabled == 0) {
      continue;
    }

    LinkCfgRead.Data = MemReadPciCfgEp (Socket, Channel, LINK_CFG_READ_1_MCDDC_DP_REG);
    if (IsSiliconWorkaroundEnabled ("S1706854367") ||
        IsSiliconWorkaroundEnabled ("S14011218130")) {
      // Disable RRD RCB clock gating
      LinkCfgRead.Bits.read_data |= RRD_RCB_CLOCK_GATE_DISABLE;
    }
    if (IsSiliconWorkaroundEnabled ("S1408782269")) {
      // Disable RT RCB clock gating
      LinkCfgRead.Bits.read_data |= RT_RCB_CLOCK_GATE_DISABLE;
    }
    RcDebugPrintWithDevice (
      SDBG_MAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "LINK_CFG_READ_1 = 0x%08x\n",
      LinkCfgRead.Data
      );
    MemWritePciCfgEp (Socket, Channel, LINK_CFG_READ_1_MCDDC_DP_REG, LinkCfgRead.Data);
  }
}

/**
  Disable ADDDC column correction.

  This function disables ADDDC column correction, such as due to silicon work-arounds.

  @param[in]  Host      Pointer to the SYSHOST structure.
  @param[in]  Socket    Socket number.
**/
VOID
DisableAdddcColumnCorrectionAsNeeded (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  LINK_CFG_READ_1_MCDDC_DP_STRUCT LinkCfgRead;
  UINT8                           Channel;
  UINT8                           MaxChDdr;

  if (!(IsSiliconWorkaroundEnabled ("S1409873802") || IsSiliconWorkaroundEnabled ("S1409873823"))) {
    return;
  }

  ASSERT (Host != NULL);
  ASSERT (Socket < MAX_SOCKET);
  if (Host == NULL || Socket >= MAX_SOCKET) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (Host->nvram.mem.socket[Socket].channelList[Channel].enabled == 0) {
      continue;
    }
    LinkCfgRead.Data = MemReadPciCfgEp (Socket, Channel, LINK_CFG_READ_1_MCDDC_DP_REG);
    LinkCfgRead.Bits.read_data |= BIT27;
    MemWritePciCfgEp (Socket, Channel, LINK_CFG_READ_1_MCDDC_DP_REG, LinkCfgRead.Data);
  }
}

/**
  set Link Cfg Read1 ReadData Bit18.

  This function Enables Link Cfg Read1 ReadData Bit18, such as due to silicon work-arounds.

  @param[in]  Host      Pointer to the SYSHOST structure.
  @param[in]  Socket    Socket number.
**/
VOID
SetLinkCfgRead1ReadDataBit18AsNeeded (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  LINK_CFG_READ_1_MCDDC_DP_STRUCT LinkCfgRead;
  UINT8                           Channel;
  UINT8                           MaxChDdr;

  if (!(IsSiliconWorkaroundEnabled ("S14010892610"))) {
    return;
  }

  ASSERT (Host != NULL);
  ASSERT (Socket < MAX_SOCKET);
  if (Host == NULL || Socket >= MAX_SOCKET) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (Host->nvram.mem.socket[Socket].channelList[Channel].enabled == 0) {
      continue;
    }
    LinkCfgRead.Data = MemReadPciCfgEp (Socket, Channel, LINK_CFG_READ_1_MCDDC_DP_REG);
    LinkCfgRead.Bits.read_data |= BIT18;
    MemWritePciCfgEp (Socket, Channel, LINK_CFG_READ_1_MCDDC_DP_REG, LinkCfgRead.Data);
  }
}

/**
  Set mc_wp_crnode_registers.link_cfg_read_1.read_data[11] to 1.

  This function Set mc_wp_crnode_registers.link_cfg_read_1.read_data[11] to 1 as needed, such as due to silicon work-arounds.

  @param[in]  Host      Pointer to the SYSHOST structure.
  @param[in]  Socket    Socket number.
**/
VOID
SetLinkCfgRead1ReaddataBit11AsNeeded (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  LINK_CFG_READ_1_MCDDC_DP_STRUCT LinkCfgRead;
  UINT8                           Channel;
  UINT8                           MaxChDdr;

  if (!(IsSiliconWorkaroundEnabled ("S14011971908"))) {
    return;
  }

  ASSERT (Host != NULL);
  ASSERT (Socket < MAX_SOCKET);
  if (Host == NULL || Socket >= MAX_SOCKET) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (Host->nvram.mem.socket[Socket].channelList[Channel].enabled == 0) {
      continue;
    }
    LinkCfgRead.Data = MemReadPciCfgEp (Socket, Channel, LINK_CFG_READ_1_MCDDC_DP_REG);
    LinkCfgRead.Bits.read_data |= BIT11;
    MemWritePciCfgEp (Socket, Channel, LINK_CFG_READ_1_MCDDC_DP_REG, LinkCfgRead.Data);
  }
}

/**
  Set mc0_dp_chkn_bit2.mask_adddc_trans_specrd_only to 1 on A0. Silicon default value change only happen on B0.
  BIOS WA: Change the default value of mc_wp_crnode_registers.mc0_dp_chkn_bit2.mask_adddc_trans_specrd_only  to 1 from 0 .
           This register bit is used to mask error logging on transient errors for spec reads to a ADDDC  region .

  @retval   none
 **/
VOID
Wa_14011241978 (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  MC0_DP_CHKN_BIT2_MCDDC_DP_STRUCT        MC0DpChknBit2;
  UINT8                                   Channel;
  UINT8                                   MaxChDdr;

  if (!IsSiliconWorkaroundEnabled ("S14011241978")) {
    return;
  }

  ASSERT (Host != NULL);
  ASSERT (Socket < MAX_SOCKET);
  if (Host == NULL || Socket >= MAX_SOCKET) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (Host->nvram.mem.socket[Socket].channelList[Channel].enabled == 0) {
      continue;
    }
    MC0DpChknBit2.Data = MemReadPciCfgEp (Socket, Channel, MC0_DP_CHKN_BIT2_MCDDC_DP_REG);
    MC0DpChknBit2.Data |= (UINT32)BIT4;//mask_adddc_trans_specrd_only for SPR
    MemWritePciCfgEp (Socket, Channel, MC0_DP_CHKN_BIT2_MCDDC_DP_REG, MC0DpChknBit2.Data);
  }
}

/**
  Disable Address Parity Checking in MC_.msg for silicon workaround 22010732966
  @retval   none
 **/
VOID
DisableApppErrorLogAsNeeded (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT MC0DpChknBit;;
  UINT8                           Channel;
  UINT8                           MaxChDdr;

  if (!IsSiliconWorkaroundEnabled ("S22010732966")) {
    return;
  }

  ASSERT (Host != NULL);
  ASSERT (Socket < MAX_SOCKET);
  if (Host == NULL || Socket >= MAX_SOCKET) {
    return;
  }

  if (GetSocketPresentBitMap () == 1) {
    //
    // only apply this WA when multiple socket populated
    //
    return;
  }

  if (KTI_HOST_OUT_PTR->CxlPresentBitmap[Socket] == 0) {
    //
    // only apply this WA when CXL Device installed
    //
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (Host->nvram.mem.socket[Socket].channelList[Channel].enabled == 0) {
      continue;
    }
    MC0DpChknBit.Data = MemReadPciCfgEp (Socket, Channel, MC0_DP_CHKN_BIT_MCDDC_DP_REG);
    MC0DpChknBit.Bits.dis_appp_err_log = 1;
    MemWritePciCfgEp (Socket, Channel, MC0_DP_CHKN_BIT_MCDDC_DP_REG, MC0DpChknBit.Data);
  }
}

/**
  Disable WDB parity error.
  This function disables WDB parity error, such as due to silicon work-arounds.

  @param[in]  Host      Pointer to the SYSHOST structure.
  @param[in]  Socket    Socket number.
**/
VOID
DisableWdbParityCheckAsNeeded (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT MC0DpChknBit;;
  UINT8                           Channel;
  UINT8                           MaxChDdr;

  if (!IsSiliconWorkaroundEnabled ("S1409873816")) {
    return;
  }

  ASSERT (Host != NULL);
  ASSERT (Socket < MAX_SOCKET);
  if (Host == NULL || Socket >= MAX_SOCKET) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (Host->nvram.mem.socket[Socket].channelList[Channel].enabled == 0) {
      continue;
    }
    MC0DpChknBit.Data = MemReadPciCfgEp (Socket, Channel, MC0_DP_CHKN_BIT_MCDDC_DP_REG);
    MC0DpChknBit.Bits.dis_wdb_par_chk = 1;
    MemWritePciCfgEp (Socket, Channel, MC0_DP_CHKN_BIT_MCDDC_DP_REG, MC0DpChknBit.Data);
  }
}

/**
  Disable Performance Based Preemption
  This function disables performance based preemption, such as due to silicon work-arounds.

  @param[in]  Host      Pointer to the SYSHOST structure.
  @param[in]  Socket    Socket number.
**/
VOID
DisablePerformanceBasedPreemptionAsNeeded (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  SCHEDULER_PREEMPTION_MCDDC_CTL_STRUCT       SchedulerPreemption;
  UINT8                           Channel;
  UINT8                           MaxChDdr;

  if (!IsSiliconWorkaroundEnabled ("S14010042217")) {
    return;
  }

  ASSERT (Host != NULL);
  ASSERT (Socket < MAX_SOCKET);
  if (Host == NULL || Socket >= MAX_SOCKET) {
    return;
  }

  if (IsTmeEnabled (Socket)) {
    MaxChDdr = GetMaxChDdr ();
    for (Channel = 0; Channel < MaxChDdr; Channel++) {
      if (Host->nvram.mem.socket[Socket].channelList[Channel].enabled == 0) {
        continue;
      }
      SchedulerPreemption.Data = MemReadPciCfgEp (Socket, Channel, SCHEDULER_PREEMPTION_MCDDC_CTL_REG);
      SchedulerPreemption.Bits.enable_critical_preemption = 1;
      SchedulerPreemption.Bits.enable_wmm_read_preemption = 1;
      SchedulerPreemption.Bits.enable_rmm_write_preemption = 1;
      SchedulerPreemption.Bits.enable_omm_miss_preemption = 1;
      SchedulerPreemption.Bits.enable_omm_hit_promotion = 1;
      SchedulerPreemption.Bits.enable_omm_hit_preemption = 1;
      SchedulerPreemption.Bits.enable_refresh_preemption = 1;
      MemWritePciCfgEp (Socket, Channel, SCHEDULER_PREEMPTION_MCDDC_CTL_REG, SchedulerPreemption.Data);
    }
  }
}

/**
  Enable address skip in sparing_control_thresh.

  @param Host - Pointer to sysHost

  @retval SUCCESS

**/
STATIC VOID
EnableSparingControlAddressSkip (
    PSYSHOST Host,
    UINT8    socket
)
{
  UINT8                             ch;
  UINT8                             MaxChDdr;
  SYS_SETUP                         *Setup;
  struct channelNvram (*channelNvList)[MAX_CH];
  SPARING_CONTROL_THRESH_MC_MAIN_STRUCT SparingCtlThr;

  Setup = GetSysSetupPointer ();
  MaxChDdr = GetMaxChDdr ();

  channelNvList = GetChannelNvList(Host, socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    SparingCtlThr.Data = UsraCsrRead (socket, ch, SPARING_CONTROL_THRESH_MC_MAIN_REG);
    SparingCtlThr.Bits.addr_skip_en = 1;  //enable addr_skip_en
    UsraCsrWrite (socket, ch, SPARING_CONTROL_THRESH_MC_MAIN_REG, SparingCtlThr.Data);
    RcDebugPrint (SDBG_MAX,"SPARING_CONTROL_THRESH addr_skip_en enabled.\n");
  }
}

/**
  Enable Pmon1 Clock running for link fail trigger.

  @param[in]  Host      Pointer to the SYSHOST structure.
  @param[in]  Socket    Socket number.

**/
VOID
EnablePmon1ForLinkFailAsNeeded (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  LINK_ERR_FSM_TRIGGER_CTL_MCDDC_DP_STRUCT  LinkErrFsmTriggerCtl;

#ifdef SPR_HOST
  FINAL_TRIG_CTL_MC_MAIN_STRUCT             FinalTrigCtl;
#else
  FINAL_TRIG_CTL_MC_2LM_STRUCT              FinalTrigCtl;
#endif

  DDR4_GLBRSP_CTL_MCDDC_DP_STRUCT           Ddr4GlbrspCtl;
  MCGLBRSPCNTL_MC_MAIN_STRUCT               McgLbrSpCntl;

  UINT8                                     Channel;
  UINT8                                     MaxChDdr;

  if (!IsSiliconWorkaroundEnabled ("S14010044321")) {
    return;
  }

  ASSERT (Host != NULL);
  ASSERT (Socket < MAX_SOCKET);

  if (Host == NULL || Socket >= MAX_SOCKET) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (Host->nvram.mem.socket[Socket].channelList[Channel].enabled == 0) {
      continue;
    }

    LinkErrFsmTriggerCtl.Data = MemReadPciCfgEp (Socket, Channel, LINK_ERR_FSM_TRIGGER_CTL_MCDDC_DP_REG);
    LinkErrFsmTriggerCtl.Bits.trigger_sb_uncorr_lmt = 0x1;
    MemWritePciCfgEp (Socket, Channel, LINK_ERR_FSM_TRIGGER_CTL_MCDDC_DP_REG, LinkErrFsmTriggerCtl.Data);

    LinkErrFsmTriggerCtl.Data = MemReadPciCfgEp (Socket, Channel, LINK_ERR_FSM_TRIGGER_CTL_MCDDC_DP_REG);
    LinkErrFsmTriggerCtl.Bits.ctl_enable_trigger = 0x1;
    MemWritePciCfgEp (Socket, Channel, LINK_ERR_FSM_TRIGGER_CTL_MCDDC_DP_REG, LinkErrFsmTriggerCtl.Data);

#ifdef SPR_HOST
    FinalTrigCtl.Data = MemReadPciCfgEp (Socket, Channel, FINAL_TRIG_CTL_MC_MAIN_REG);
    FinalTrigCtl.Bits.trig_select0 = 0x2;
    MemWritePciCfgEp (Socket, Channel, FINAL_TRIG_CTL_MC_MAIN_REG, FinalTrigCtl.Data);
#else
    FinalTrigCtl.Data = MemReadPciCfgEp (Socket, Channel, FINAL_TRIG_CTL_MC_2LM_REG);
    FinalTrigCtl.Bits.trig_select0 = 0x2;
    MemWritePciCfgEp (Socket, Channel, FINAL_TRIG_CTL_MC_2LM_REG, FinalTrigCtl.Data);
#endif

    Ddr4GlbrspCtl.Data = MemReadPciCfgEp (Socket, Channel, DDR4_GLBRSP_CTL_MCDDC_DP_REG);
    Ddr4GlbrspCtl.Bits.trigger_retry_fsm = 0x1;
    MemWritePciCfgEp (Socket, Channel, DDR4_GLBRSP_CTL_MCDDC_DP_REG, Ddr4GlbrspCtl.Data);

    McgLbrSpCntl.Data = MemReadPciCfgEp (Socket, Channel, MCGLBRSPCNTL_MC_MAIN_REG);
    McgLbrSpCntl.Bits.glbrsp0starttrigsel = 0x4;
    MemWritePciCfgEp (Socket, Channel, MCGLBRSPCNTL_MC_MAIN_REG, McgLbrSpCntl.Data);

    McgLbrSpCntl.Data = MemReadPciCfgEp (Socket, Channel, MCGLBRSPCNTL_MC_MAIN_REG);
    McgLbrSpCntl.Bits.glbrsp0stopsel = 0x7;
    MemWritePciCfgEp (Socket, Channel, MCGLBRSPCNTL_MC_MAIN_REG, McgLbrSpCntl.Data);

  }
}

/**

  SetM2mDefeatures1UcOnNmCacheRdUcDataAsNeeded

  @param[in]  Host      Pointer to the SYSHOST structure.
  @param[in]  Socket    Socket number.

  @retval N/A

**/
VOID
EFIAPI
SetM2mDefeatures1UcOnNmCacheRdUcDataAsNeeded (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8                           McId;
  UINT8                           MaxImc;

  if(IsSiliconWorkaroundEnabled ("S14011166558")) {
    MaxImc = GetMaxImc ();
    for (McId = 0; McId < MaxImc; McId++) {
      if (!CheckSocketMcEnabled (Socket, McId)) {
        continue;
      }
      SetM2mDefeatures1UcOnNmCacheRdUcData(Socket, McId);
    }
  }
}

/**

  SetMcmainChknBitsDisLclCkGateMcmainAsNeeded

  @param[in]  Host      Pointer to the SYSHOST structure.
  @param[in]  Socket    Socket number.

  @retval N/A

**/
VOID
EFIAPI
SetMcmainChknBitsDisLclCkGateMcmainAsNeeded (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8                             ChOnSkt;
  struct channelNvram               (*channelNvList)[MAX_CH];
  MCMAIN_CHKN_BITS_MC_MAIN_STRUCT   McMainChknReg;
  UINT8                             MaxChDdr;

  if (!(IsSiliconWorkaroundEnabled ("S22010555690"))) {
    return;
  }

  ASSERT (Host != NULL);
  ASSERT (Socket < MAX_SOCKET);
  if (Host == NULL || Socket >= MAX_SOCKET) {
    return;
  }

  MaxChDdr = GetMaxChDdr ();
  //
  // Dsiable clock gating in 2LM or 1LM/2LM hybrid mode
  //
  channelNvList = GetChannelNvList(Host, Socket);
  if (Host->var.mem.volMemMode == VOL_MEM_MODE_2LM || Host->var.mem.volMemMode == VOL_MEM_MODE_MIX_1LM2LM) {
    for (ChOnSkt = 0; ChOnSkt < MaxChDdr; ChOnSkt++) {
      if ((*channelNvList)[ChOnSkt].enabled == 0) {
        continue;
      }

      McMainChknReg.Data = MemReadPciCfgEp (Socket, ChOnSkt, MCMAIN_CHKN_BITS_MC_MAIN_REG);
      McMainChknReg.Bits.dis_lcl_ck_gate_mcmain = 1;
      MemWritePciCfgEp (Socket, ChOnSkt, MCMAIN_CHKN_BITS_MC_MAIN_REG, McMainChknReg.Data);
    } // ch loop
  } // if volMemMode
}

/**
  Enable RAS modes
  NOTE: Default settings are unchanged if feature not explicitly enabled by BIOS

  @param Host - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
SetRASConfig (
             PSYSHOST Host
             )
{
  UINT8                               socket;
  UINT8                               mcId;
  UINT8                               ch;
  UINT8                               i;
  UINT8                               MaxImc;
  UINT8                               Dimm;
  MCSCHED_CHKN_BIT2_MCDDC_CTL_STRUCT  McschedChknBit2;
  BOOLEAN                             AppDirect;

  struct channelNvram (*channelNvList)[MAX_CH];
  UINT32  CorrErrCntRegs[] = {CORRERRCNT_0_MCDDC_DP_REG,
                              CORRERRCNT_1_MCDDC_DP_REG,
                              CORRERRCNT_2_MCDDC_DP_REG,
                              CORRERRCNT_3_MCDDC_DP_REG};
  UINT32  CorrErrThrshldRegs[] = {CORRERRTHRSHLD_0_MCDDC_DP_REG,
                                  CORRERRTHRSHLD_1_MCDDC_DP_REG,
                                  CORRERRTHRSHLD_2_MCDDC_DP_REG,
                                  CORRERRTHRSHLD_3_MCDDC_DP_REG};
  UINT8   MaxChDdr;
  UINT32  Command;
  UINT32  UboxSmiCtrl;
  SYS_SETUP    *Setup;

  Setup = GetSysSetupPointer ();

  MaxChDdr = GetMaxChDdr ();
  MaxImc = GetMaxImc ();

  RcDebugPrint (SDBG_MAX,
                 "Set RAS Config\n");

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    if (IsSocketDimmPopulated (Host, socket) != TRUE) {
      continue;
    }

    //
    // Set Nsddc and Esddc before Patrol Scrub and any other traffic
    //
    if (IsCpuAndRevisionOrLater (CPU_ICXSP, REV_C0)) {
      if (Setup->mem.NsddcEn == 1) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Enable nSddc mode\n");
        EnableNsddcMode (Host, socket);
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "New Sddc is disabled\n");
      }
    }
    if (IsCpuAndRevisionOrLater (CPU_ICXSP, REV_D0) || IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      if ((Setup->mem.EsddcEn == 1) && (Setup->mem.ExtendedADDDCEn == 1) && IsSiliconWorkaroundEnabled ("S1707090346")) {
        Setup->mem.EsddcEn = 0;
        RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Changing Enhanded Sddc to disabled due to workaround\n");
      }
      if (Setup->mem.EsddcEn == 1) { //Enable
        if (Host->nvram.mem.eccEn != 0) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "Enable Enhanced Sddc\n");
          EnableEnhancedSddc (Host, socket);
        }
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Enhanced Sddc is disabled\n");
      }
    }

    //
    // ICX and beyond, Set Link Fail either in independent or mirrored mode
    //
    SetMCLinkFail (Host, socket);

    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_RAS_CONFIG, CHECKPOINT_MINOR_MIRROR_MODE, socket));
    ConfigMirrorMode (Host, socket);
    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_RAS_CONFIG, CHECKPOINT_MINOR_SPARE_MODE, socket));
    SetSpareMode (Host, socket);
    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_RAS_CONFIG, CHECKPOINT_MINOR_ERR_THRESH, socket));
    SetErrorThresholdNode (Host, socket);
    OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_RAS_CONFIG, CHECKPOINT_MINOR_ADDDC_MIRROR, socket));
    SetADDDCMirrorMode (Host, socket);

    if ((Host->nvram.mem.RASmode & CH_ML) == CH_INDEPENDENT) {

      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "Independent ch mode enabled\n");
    }

    SetLinkCfgRead1ReadDataBit18AsNeeded (Host, socket);
    SetLinkCfgRead1ReaddataBit11AsNeeded (Host, socket);

    SetPatrolScrub (Host, socket);
    EnableSparingControlAddressSkip(Host, socket);

    //
    // Set leaky bucket config to non-zero, per architect request
    //
    SetLeakyBucketConfig (Host, socket);

    //
    // Disable MC data parity checking for certain configurations due to silicon WA
    //
    DisableMcDataParityCheckAsNeeded (Host, socket);

    DisableClockGatingAsNeeded (Host, socket);

    DisableAdddcColumnCorrectionAsNeeded (Host, socket);

    Wa_14011241978 (Host, socket);

    DisableWdbParityCheckAsNeeded (Host, socket);

    DisableApppErrorLogAsNeeded (Host, socket);

    DisablePerformanceBasedPreemptionAsNeeded (Host, socket);

    EnablePmon1ForLinkFailAsNeeded (Host, socket);

    SetM2mDefeatures1UcOnNmCacheRdUcDataAsNeeded (Host, socket);

    SetMcmainChknBitsDisLclCkGateMcmainAsNeeded (Host, socket);

    channelNvList = GetChannelNvList(Host, socket);
    if ((GetSysBootMode () == S3Resume) && (Host->var.mem.subBootMode != AdrResume)){
      //
      // during S3 resume, restore corrected error counters/thresholds/signalmode
      //
      for (ch = 0; ch < MaxChDdr; ch++) {

        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }
        for (i = 0; i < (MAX_RANK_CH * SUB_CH)/2; i++) {
          MemWritePciCfgEp (socket, ch, CorrErrCntRegs[i], Host->nvram.mem.socket[socket].channelList[ch].rankErrCountInfo[i]);
          MemWritePciCfgEp (socket, ch, CorrErrThrshldRegs[i], Host->nvram.mem.socket[socket].channelList[ch].rankErrThresholdInfo[i]);
        } // for (i)

        MemWritePciCfgEp (socket, ch, LINK_RETRY_ERR_LIMITS_MCDDC_DP_REG, Host->nvram.mem.socket[socket].channelList[ch].LinkRetryErrLimits);
        MemWritePciCfgEp (socket, ch, LINK_LINK_FAIL_MCDDC_DP_REG, Host->nvram.mem.socket[socket].channelList[ch].LinkLinkFail);
        MemWritePciCfgEp (socket, ch, SMISPARECTL_MC_MAIN_REG, Host->nvram.mem.socket[socket].channelList[ch].SmiSpareCtlMcMainExt);
      } // for (ch)

      UboxSmiCtrl = Host->nvram.mem.socket[socket].smiCtrlUboxMisc;
      if (IsSiliconWorkaroundEnabled ("S1409629810")) {
        Command = MAILBOX_BIOS_CR_WRITE_CMD (0, 0, MAILBOX_BIOS_CMD_CR_PROXY_UBOX_SMICTRL_CFG_INDEX);
        ReadModifyWritePcuMailbox (socket, MAILBOX_BIOS_CMD_ZERO, Command, &UboxSmiCtrl, 0, 0);
      } else {
        UsraCsrWrite (socket, 0, UBOX_SMICTRL_UBOX_MISC_REG, UboxSmiCtrl);
      }

      for (mcId = 0; mcId < MaxImc; mcId++) {
        // Skip inactive IMC
        if (Host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
          continue;
        }

        MemWritePciCfgMC_AllCh (socket, mcId, EMCALTCTL_MC_MAIN_REG, Host->nvram.mem.socket[socket].imc[mcId].EmcaLtCtlMcMainExt);
        SetM2mExRasConfig (socket, mcId, Host->nvram.mem.socket[socket].imc[mcId].ExRasConfigHaCfg);
      } // mcId
    } //s3 resume
  } // socket loop

  if (Host->nvram.mem.eccEn != 0) {
    RcDebugPrint (SDBG_MAX,
                   "ECC is enabled\n");
  }

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (Host->nvram.mem.socket[socket].DcpmmPresent) {
      for (ch = 0; (ch < MaxChDdr); ch++) {
        AppDirect = FALSE;
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
          if (Host->nvram.mem.socket[socket].channelList[ch].dimmList[Dimm].nonVolCap != 0) {
            AppDirect = TRUE;
            break;
          }
        }
        if (AppDirect == TRUE) {
          McschedChknBit2.Data = MemReadPciCfgEp (socket, ch, MCSCHED_CHKN_BIT2_MCDDC_CTL_REG);
          McschedChknBit2.Bits.drop_writes_on_viral = 1;
          MemWritePciCfgEp (socket, ch, MCSCHED_CHKN_BIT2_MCDDC_CTL_REG, McschedChknBit2.Data);
        }
      }
    }
  }

  RcDebugPrint (SDBG_MINMAX,
          "SetRASConfig End\n");

  return SUCCESS;
}

/**

  Check if a socket's ddr channel population is valid for lockstep

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval RAS_IS_CAPABLE or RAS_NOT_CAPABLE

**/
UINT8
CheckLockstepPopulation (
    PSYSHOST Host,
    UINT8    socket
)
{
  UINT8                 ch;
  UINT8                 dimm;
  UINT8                 capable;
  UINT8                 notOnlyX4Dimms;

  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT8                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  capable = RAS_IS_CAPABLE;

  // Are there any DIMMs other than x4?
  notOnlyX4Dimms = 0;
  channelNvList = GetChannelNvList(Host, socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      if (!(*dimmNvList)[dimm].x4Present) {
        notOnlyX4Dimms++;
      } //if !x4Present
    } //for dimm
    RcDebugPrint (SDBG_MAX,
                   "\nCheckLockstepPopulation (Socket = %d, ch = %d) - ", socket, ch);
    RcDebugPrint (SDBG_MAX,
                   (capable == RAS_IS_CAPABLE)? "Yes\n" : "No\n" );
  } //for ch

  return capable;
}

/**

  Check if channel supports sparing by checking if occupied channel has a spare rank

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

  @retval RAS_IS_CAPABLE or RAS_NOT_CAPABLE

**/
STATIC UINT8
CheckSparingPopulation (
  PSYSHOST Host,
  UINT8    socket
)
{
  UINT8                 channel;
  UINT8                 dimm;
  UINT8                 rank;
  UINT8                 capable;
  UINT8                 numranks;
  UINT8                 numdimms;
  struct socketNvram    *socketNv;
  struct dimmNvram (*dimmNvList0)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT8                 MaxChDdr;

  MaxChDdr  = GetMaxChDdr ();
  socketNv  = &Host->nvram.mem.socket[socket];
  channelNvList = GetChannelNvList(Host, socket);
  capable = RAS_IS_CAPABLE;

  for (channel=0; channel < MaxChDdr; channel++) {
    if ((*channelNvList)[channel].enabled == 0) {
      continue;
    }
    dimmNvList0 = GetDimmNvList(Host, socket, channel);
    numranks    = 0;
    numdimms    = 0;

    // Count number of ranks in channel
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if (((*dimmNvList0)[dimm].dimmPresent) == 0) {
        continue;
      }

      // MC Rank Sparing is only for DDR4 Ranks, Skip NVM DIMMs
      if (((*dimmNvList0)[dimm].DcpmmPresent) == 1) {
        continue;
      }

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (CheckRank(Host, socket, channel, dimm, rank, CHECK_MAPOUT)) {
          continue;
        }
        numranks++;
      } // rank loop
      if (numranks) {
        numdimms++;
      }
    } // dimm loop

    // Rank sparing possible only if there are two or more DDR4 ranks
    if ((numdimms == 1) && (numranks < 2)) {
      capable = RAS_NOT_CAPABLE;
      break;
    }

    RcDebugPrint (SDBG_MAX,
                   "\nCheckSparingPopulation (Socket = %d, ch = %d) - ", socket, channel);
    RcDebugPrint (SDBG_MAX,
                   (capable == RAS_IS_CAPABLE)? "Yes\n" : "No\n" );

    if (capable == RAS_IS_CAPABLE) {
      break;
    }

  } // for (channel)

  return capable;
}

/**

  Returns the paired rank

  @param Host           - Pointer to sysHost
  @param socket         - Socket number
  @param ch             - Pointer to sysHost
  @param dimm           - Pointer to sysHost
  @param rank           - Pointer to sysHost
  @param pairRankNumber - Pointer to sysHost
  @param pairChannel    - Pointer to sysHost
  @param pairDimm       - Pointer to sysHost
  @param pairRank       - Pointer to sysHost

  @retval VOID

**/
VOID
GetPairedRank (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     *pairRankNumber,
  UINT8     *pairChannel,
  UINT8     *pairDimm,
  UINT8     *pairRank
  )
{
  UINT8                             NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();

  RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
    "GetPairedRank: NumChPerMc = %x\n", NumChPerMc
    );
  if (ChipMirrorEn(Host)){
    *pairRank = rank;
    *pairDimm = dimm;
    switch (NumChPerMc) {
      case 2:
        //
        // 2 Ch/Mc case:
        // 0:1
        // 2:3
        // 4:5
        // 6:7
        //
        if (*pairRankNumber == 1) {
          *pairChannel = ch ^ 1;
        } else {
          *pairRankNumber = 0;
        }
        break;
      default:
        //
        // 3 Ch/Mc case:
        // 0:1:2
        // 3:4:5
        //
        if (*pairRankNumber == 1) {
          if (ch == 0) {
            *pairChannel = 1;
          } else if (ch == 1) {
            *pairChannel = 0;
          } else if (ch == 2) {
            *pairChannel = 0;
          } else if (ch == 3) {
            *pairChannel = 4;
          } else if (ch == 4) {
            *pairChannel = 3;
          } else if (ch == 5) {
            *pairChannel = 3;
          }
        } else if (*pairRankNumber == 2) {
          if (ch == 0) {
            *pairChannel = 2;
          } else if (ch == 1) {
            *pairChannel = 2;
          } else if (ch == 2) {
            *pairChannel = 1;
          } else if (ch == 3) {
            *pairChannel = 5;
          } else if (ch == 4) {
            *pairChannel = 5;
          } else if (ch == 5) {
            *pairChannel = 4;
          }
        } else {
          *pairRankNumber = 0;
        }
        break;
    }
  } else if (ChipLockstepEn(Host)) {

    if (*pairRankNumber == 1) {
      *pairChannel = ch;
      *pairRank = rank;
      if (dimm == 0) {
        *pairDimm = 1;
      } else if (dimm == 1) {
        *pairDimm = 0;
      }
    } else {
      *pairRankNumber = 0;
    }
  }
  RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
    "GetPairedRank: pairRankNumber = %x, pairChannel = %x\n", *pairRankNumber, *pairChannel
    );
} // GetPairedRank

/**
  Check if CPU support Software Error Recovery
  @param  VOID
  @retval TRUE  - CPU supports Software Error Recovery
          FALSE - CPU does not support Software Error Recovery
**/
BOOLEAN
EFIAPI
IsErrorRecoveryCap (
  VOID
  )
{
  MSR_IA32_MCG_CAP_REGISTER McgCapReg;

  McgCapReg.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_CAP);
  return (BOOLEAN) (McgCapReg.Bits.MCG_SER_P);
}
