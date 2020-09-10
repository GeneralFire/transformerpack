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
#include <Memory/CpgcDefinitions.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/SysHostPointerLib.h>
#include <Memory/JedecDefinitions.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Memory/MemoryCheckpointCodes.h>

#define  KEEP_FAILURE(_test, _current) (_test == FAILURE ? FAILURE : _current)
#define  MinLrMrecCycleSetting -2
#define  MaxLrMrecCycleSetting 2

#ifdef  LRDIMM_SUPPORT

// Program Desired Resulst here
// Rising Edge RE
UINT16 BCW2x[MAX_STROBE/2] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
UINT16 BCW3x[MAX_STROBE/2] = {2, 2, 2, 2, 2, 2, 2, 2, 2};
UINT16 BCWAx[MAX_STROBE/2] = {72, 72, 72, 72, 72, 72, 72, 72, 72};
UINT16 BCWBx[MAX_STROBE/2] = {72, 72, 72, 72, 72, 72, 72, 72, 72};
INT16 BCCX[MAX_STROBE] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
INT16 BCDX[MAX_STROBE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
UINT16 BC4X5X[MAX_STROBE] = {63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63};
UINT16 BC8X9X[MAX_STROBE] = {63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63};

//
// Local function prototypes
//
STATIC
UINT32
MDQReceiveEnablePhaseTraining (
  PSYSHOST  Host,
  UINT8     socket
  );

STATIC
UINT32
DramInterfaceWlTraining (
  PSYSHOST  Host,
  UINT8     socket
  );

STATIC
UINT32
MDQCoarseWlTraining (
  PSYSHOST  Host,
  UINT8     socket
  );

STATIC
UINT32
MDQWriteDelayTraining (
  PSYSHOST  Host,
  UINT8     socket
  );

STATIC
UINT32
MDQReceiveEnableCycleTraining (
  PSYSHOST  Host,
  UINT8     socket
  );

STATIC
UINT32
MDQReadDelayTraining (
  PSYSHOST  Host,
  UINT8     socket
  );

STATIC
UINT32
Ddr4LrdimmBacksideRxOneRankTimingMode (
  PSYSHOST  Host,
  UINT8     socket
  );

STATIC
UINT32
Ddr4LrdimmBacksideDisableOneRankTimingMode (
  PSYSHOST  Host,
  UINT8     socket
  );

STATIC
UINT32
Ddr4LrdimmBacksideTxOneRankTimingMode (
  PSYSHOST  Host,
  UINT8     socket
  );

UINT32
SetLrdimmMrdResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT16    centerPoint[MAX_STROBE]
  );

UINT32
SetLrdimmMwdResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT16    centerPoint[MAX_STROBE]
  );

VOID
GetLrdimmTrainResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT16    piDelay,
  UINT8     step
  );

VOID
GetLrdimmMrecResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  INT8      cycleSetting
  );

/**

  Checks the previous cycle training check and if passed, takes the most current one

  @param[in]  Host                     - Pointer to sysHost
  @param[in]  Socket                   - Socket number
  @param[in]  Ch                       - Ch number
  @param[in]  Strobe                   - Strobe number
  @param[in]  lrMrecTrainingResults    - Pointer to LrdimmMrecResults
  @param[in]  CycleSetting             - Nibble cycle setting number

  @retval N/A

**/
VOID
AdjustLrMrecTrainingResults (
  IN PSYSHOST    Host,
  IN UINT8       Socket,
  IN UINT8       Ch,
  IN UINT8       Strobe,
  IN struct      lrMrecTrainingResults lrMrecTrainRes[MAX_CH][MAX_STROBE],
  IN INT8        CycleSetting
  );

VOID
GetLrdimmCwlResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  INT8      cycleDelay
  );

VOID
GetLrdimmMrdMwdResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT8     delaySetting,
  UINT8     step
  );

VOID
SetupLrdimmReadTest (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitmask,
  UINT8     NumCL,
  UINT8     loopCount
  );

VOID
SetupLrdimmReadWriteTest (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitmask,
  UINT8     NumCL,
  UINT8     loopCount
  );

STATIC
VOID
LrdimmCycleTrainingInit (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    rank,
  UINT8    step
  );

VOID
LrdimmExitRankTraining (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    rank
  );

STATIC
VOID
LrdimmMrdInit (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    rank
  );

STATIC
VOID
LrdimmMwdInit (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    rank
  );

STATIC
UINT32
RestoreDimmRankPresence (
  PSYSHOST  host,
  UINT8     socket,
  UINT8     dimm
  );

VOID
PrintMrecResults (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  UINT8 dimm,
  UINT8 ReadMode
  );

/**

  Initialize LRDIMM MPR Buffer with provided pattern

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Ch      - channel number
  @param[in] Dimm    - dimm number
  @param[in] Pattern - pointer to an 8 byte array containing pattern values to be written.

  @retval N/A

**/
VOID
SetBufferMprPattern (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    Dimm,
  IN  UINT8    Pattern[MAX_BUFFER_MPR]
  );


//
// code starts here
//
VOID
SkipDdr4LrdimmBacksideRxDelay (
  PSYSHOST Host,
  UINT8 socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT32                            chBitmask;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "COSIM_WA: SkipDdr4LrdimmBacksideRxDelay\n");

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {

      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        SetLrdimmMrdResults (Host, socket, ch, dimm, rank, BC4X5X);
      } // ch
    } // rank
  } // dimm
}

VOID
SkipDdr4LrdimmBacksideTxDelay (
  PSYSHOST Host,
  UINT8 socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT32                            chBitmask;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "COSIM_WA: SkipDdr4LrdimmBacksideTxDelay\n");

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {

      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        SetLrdimmMwdResults (Host, socket, ch, dimm, rank, BC8X9X);
      } // ch
    } // rank
  } // dimm
}

VOID
SkipDdr4LrdimmBacksideRxCycle (
  PSYSHOST Host,
  UINT8 socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             strobe;
  UINT32                            chBitmask;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  UINT8                             *controlWordDataPtr;
  UINT8                             codedNibbleCycleSetting;
  UINT8                             controlWordAddr;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "COSIM_WA: SkipDdr4LrdimmBacksideRxCycle\n");

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {

      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (strobe = 0; strobe < MSVx4; strobe++) {
          if (BCCX[strobe] < 0) {
            codedNibbleCycleSetting = 4 + (UINT8) ABS (BCCX[strobe]);
          } else {
            codedNibbleCycleSetting = (UINT8) BCCX[strobe];
          }
          (*rankList)[rank].lrBuf_FxBCCxEx[strobe % MSVx8] &= (0xF0 >> (4 * (strobe / MSVx8)));
          (*rankList)[rank].lrBuf_FxBCCxEx[strobe % MSVx8] |= (codedNibbleCycleSetting << (4 * (strobe / MSVx8)));
        } // strobe

        controlWordDataPtr = &((*rankList)[rank].lrBuf_FxBCCxEx[0]);
        controlWordAddr = LRDIMM_BCCx + ((rank >> 1) * 0x20);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, (rank & BIT0) , controlWordAddr, ALL_DATABUFFERS);
      } // ch
    } // rank
  } // dimm
}

VOID
SkipDdr4LrdimmBacksideTxCoarse (
  PSYSHOST Host,
  UINT8 socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             strobe;
  UINT32                            chBitmask;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  UINT8                             *controlWordDataPtr;
  UINT8                             codedNibbleCycleSetting;
  UINT8                             controlWordAddr;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "COSIM_WA: SkipDdr4LrdimmBacksideTxCoarse\n");

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {

      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (strobe = 0; strobe < MSVx4; strobe++) {
          if (BCDX[strobe] < 0) {
            codedNibbleCycleSetting = 4 + (UINT8) ABS (BCDX[strobe]);
          } else {
            codedNibbleCycleSetting = (UINT8) BCDX[strobe];
          }
          (*rankList)[rank].lrBuf_FxBCDxFx[strobe % MSVx8] &= (0xF0 >> (4 * (strobe / MSVx8)));
          (*rankList)[rank].lrBuf_FxBCDxFx[strobe % MSVx8] |= (codedNibbleCycleSetting << (4 * (strobe / MSVx8)));
        } // strobe

        controlWordDataPtr = &((*rankList)[rank].lrBuf_FxBCDxFx[0]);
        controlWordAddr = LRDIMM_BCDx + ((rank >> 1) * 0x20);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, (rank & BIT0) , controlWordAddr, ALL_DATABUFFERS);
      } // ch
    } // rank
  } // dimm
}

VOID
SkipDdr4LrdimmBacksideRxPhase (
  PSYSHOST Host,
  UINT8 socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             strobe;
  UINT32                            chBitmask;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             *controlWordDataPtr;
  UINT8                             controlWordFunc;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "COSIM_WA: SkipDdr4LrdimmBacksideRxPhase\n");

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {

      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        rankStruct = GetRankStruct (Host, socket, ch, dimm);
        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (strobe = 0; strobe < MSVx4/2; strobe++) {
          if (GetPreambleState(socket, READ_PREAMBLE) == PREAMBLE_1TCLK) {
            (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe] = (UINT8) (((BCW2x[strobe]/2) + 32) % 64);
            (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe + MSVx8] = (UINT8) (((BCW3x[strobe]/2) + 32) % 64);
            (*rankList)[rank].lrBuf_FxBC2x3x[strobe] = (UINT8) (((BCW2x[strobe]/2) + 32) % 64);
            (*rankList)[rank].lrBuf_FxBC2x3x[strobe + MSVx8] = (UINT8) (((BCW3x[strobe]/2) + 32) % 64);
          } else {
            (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe] = (UINT8) (((BCW2x[strobe]/2)) % 64);
            (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe + MSVx8] = (UINT8) (((BCW3x[strobe]/2)) % 64);
            (*rankList)[rank].lrBuf_FxBC2x3x[strobe] = (UINT8) (((BCW2x[strobe]/2)) % 64);
            (*rankList)[rank].lrBuf_FxBC2x3x[strobe + MSVx8] = (UINT8) (((BCW3x[strobe]/2)) % 64);
          }
        } //strobe

        controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBC2x3x[0];
        controlWordFunc = rank;
        WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BC2x, ALL_DATABUFFERS);
        controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBC2x3x[MSVx8];
        controlWordFunc = rank;
        WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BC3x, ALL_DATABUFFERS);
      } // ch
    } // rank
  } // dimm
} // SkipDdr4LrdimmBacksideRxPhase

VOID
SkipDdr4LrdimmBacksideTxFine (
  PSYSHOST Host,
  UINT8 socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             strobe;
  UINT32                            chBitmask;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             *controlWordDataPtr;
  UINT8                             controlWordFunc;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "COSIM_WA: SkipDdr4LrdimmBacksideTxFine\n");

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {

      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        rankStruct = GetRankStruct (Host, socket, ch, dimm);
        rankList = GetRankNvList (Host, socket, ch, dimm);

        for (strobe = 0; strobe < MSVx4/2; strobe++) {
          (*rankStruct)[rank].cachedLrBuf_FxBCAxBx[strobe] = (UINT8) ((BCWAx[strobe]/2) % 64);
          (*rankStruct)[rank].cachedLrBuf_FxBCAxBx[strobe + MSVx8] = (UINT8) ((BCWBx[strobe]/2) % 64);
          (*rankList)[rank].lrBuf_FxBCAxBx[strobe] = (UINT8) ((BCWAx[strobe]/2) % 64);
          (*rankList)[rank].lrBuf_FxBCAxBx[strobe + MSVx8] = (UINT8) ((BCWBx[strobe]/2) % 64);

        } //strobe

        controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBCAxBx[0];
        controlWordFunc = rank;
        WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BCAx, ALL_DATABUFFERS);
        controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBCAxBx[MSVx8];
        controlWordFunc = rank;
        WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BCBx, ALL_DATABUFFERS);
      } // ch
    } // rank
  } // dimm
} // SkipDdr4LrdimmBacksideTxFine
/**

  Perform DDR4 LRDIMM Backside Read Training (RX)

  @param Host  - Pointer to sysHost

  @retval status

**/
UINT32
Ddr4LrdimmBacksideRxPhase (
  PSYSHOST  Host
  )
{
  UINT8                             socket;
  UINT32                            status = SUCCESS;
  struct channelNvram               (*channelNvList)[MAX_CH];
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;
  channelNvList = GetChannelNvList (Host, socket);
  //
  // Reasons to just return with SUCCESS...
  //
  if ((!IsMemFlowEnabled (LrdimmBacksideRxTraining))      ||   // Training step is disabled
      (Host->nvram.mem.socket[socket].enabled == 0)       ||   // Socket is disable
      (Host->nvram.mem.socket[socket].maxDimmPop == 0)    ||   // No DIMM present
      (Host->nvram.mem.socket[socket].lrDimmPresent == 0) ||
      (Host->nvram.mem.dramType != SPD_TYPE_DDR4))  // No LRDIMM present
  {

    if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
      SkipDdr4LrdimmBacksideRxPhase (Host, socket);
    }
    return SUCCESS;
  }

  //
  // HSX B0 workaround, BIOS sighting #4168616
  //
  SetDisable2cycBypass (Host, socket, 0);

  //
  //MDQ Receive Enable Phase Training (MREP)
  //
  status = KEEP_FAILURE (MDQReceiveEnablePhaseTraining (Host, socket), status);

  //
  // HSX B0 workaround, BIOS sighting #4168616
  //
  SetDisable2cycBypass (Host, socket, 1);

  // Set function control nibble back to 0
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      if ((*channelNvList)[ch].dimmList[dimm].rcLrFunc != 0) {
        WriteBC (Host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
        (*channelNvList)[ch].dimmList[dimm].rcLrFunc = 0;
      }
    } // ch
  } //dimm

  return status;
} //Ddr4LrdimmBacksideRx


/**

  Perform DDR4 LRDIMM Backside Read Training (RX)

  @param Host  - Pointer to sysHost

  @retval status

**/
UINT32
Ddr4LrdimmBacksideRxCycle (
  PSYSHOST  Host
  )
{
  UINT8                             socket;
  UINT32                            status = SUCCESS;
  struct channelNvram               (*channelNvList)[MAX_CH];
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;
  channelNvList = GetChannelNvList (Host, socket);

  //
  // Reasons to just return with SUCCESS...
  //
  if ((!IsMemFlowEnabled (LrdimmBacksideRxTraining))      ||   // Training step is disabled
      (Host->nvram.mem.socket[socket].enabled == 0)       ||   // No Socket present
      (Host->nvram.mem.socket[socket].maxDimmPop == 0)    ||   // No DIMM present
      (Host->nvram.mem.socket[socket].lrDimmPresent == 0) ||  // No LRDIMM present
      (Host->nvram.mem.dramType != SPD_TYPE_DDR4))
  {

    if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
      SkipDdr4LrdimmBacksideRxCycle (Host, socket);
    }
    return SUCCESS;
  }

  //
  //MDQ Receive Enable Cycle Training (MREC)
  //
  status = KEEP_FAILURE (MDQReceiveEnableCycleTraining (Host, socket), status);

  // Set function control nibble back to 0
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      if ((*channelNvList)[ch].dimmList[dimm].rcLrFunc != 0) {
        WriteBC (Host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
        (*channelNvList)[ch].dimmList[dimm].rcLrFunc = 0;
      }
    } // ch
  } //dimm

  return status;
} //Ddr4LrdimmBacksideRxCycle


/**

  Perform DDR4 LRDIMM Backside Read Training (RX)

  @param Host  - Pointer to sysHost

  @retval status

**/
UINT32
Ddr4LrdimmBacksideRxDelay (
  PSYSHOST  Host
  )
{
  UINT8                             socket;
  UINT32                            status = SUCCESS;
  struct channelNvram               (*channelNvList)[MAX_CH];
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;
  channelNvList = GetChannelNvList (Host, socket);

  //
  // Reasons to just return with SUCCESS...
  //
  if ((!IsMemFlowEnabled (LrdimmBacksideRxTraining))      ||   // Training step is disabled
      (Host->nvram.mem.socket[socket].enabled == 0)       ||   // No Socket present
      (Host->nvram.mem.socket[socket].maxDimmPop == 0)    ||   // No DIMM present
      (Host->nvram.mem.socket[socket].lrDimmPresent == 0) || // No LRDIMM present
      (Host->nvram.mem.dramType != SPD_TYPE_DDR4))
  {

    if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
      SkipDdr4LrdimmBacksideRxDelay (Host, socket);
    }
    return SUCCESS;
  }

  //
  // MDQ Read Delay Training (MRD)
  //
  status = KEEP_FAILURE (MDQReadDelayTraining (Host, socket), status);

  status = KEEP_FAILURE (Ddr4LrdimmBacksideRxOneRankTimingMode (Host, socket), status);

  // Set function control nibble back to 0
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      if ((*channelNvList)[ch].dimmList[dimm].rcLrFunc != 0) {
        WriteBC (Host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
        (*channelNvList)[ch].dimmList[dimm].rcLrFunc = 0;
      }
    } // ch
  } //dimm

  return status;
} //Ddr4LrdimmBacksideRxDelay


/**

  Perform LRDIMM Backside One Rank Timing Mode Read calculations
  and enable the support.

  @param Host   - Pointer to sysHost
  @param socket - socket number

  @retval status

**/
UINT32
Ddr4LrdimmBacksideRxOneRankTimingMode (
  PSYSHOST  Host,
  UINT8     socket
  )
{
#ifndef DDR5_SUPPORT
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             backsideRank;
  UINT8                             backsideRankMax;
  UINT32                            chBitmask;
  UINT8                             strobe;
  INT16                             rxCycle, averageRxCycle;
  INT16                             rxPhase, averageRxPhase;
  INT16                             rxDelay;
  INT16                             averageRecieveEnable, averageReadDelay;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct DimmDeviceTraining         (*ChdimmListTraining)[MAX_DIMM];
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             *controlWordDataPtr;
  UINT8                             controlWordData;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  SYS_SETUP *Setup;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList (Host, socket);
  //
  // Create a bit mask of channels that have an LRDIMM with at least 1 rank present for dimm 0, rank 0
  //
  chBitmask = GetChBitmaskLrdimm (socket, 0, 0);
  for (ch = 0; ch < MaxChDdr; ch++) {
    //
    // Initialize the chOneRankTimingModeEnable variable
    //
    (*channelNvList)[ch].chOneRankTimingModeEnable = ONE_RANK_TIMING_MODE_DISABLED;
    if (!((1 << ch) & chBitmask)) {
      continue;
    }
    (*channelNvList)[ch].chOneRankTimingModeEnable = Setup->mem.oneRankTimingModeEn;
  } //ch
  //
  // For each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Create a bit mask of channels that have at least 1 rank present for this dimm and rank 0
    //
    chBitmask = GetChBitmaskLrdimm (socket, dimm, 0);
    //
    // For each channel
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if (!((1 << ch) & chBitmask)) {
        continue;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);

      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "lrbufGen = %u\n", (*dimmNvList)[dimm].lrbufGen);

      if ((*dimmNvList)[dimm].lrbufGen == 0) {  //One Rank Timing Moded only supported on newer RCD revision, check the next dimm
        (*channelNvList)[ch].chOneRankTimingModeEnable = ONE_RANK_TIMING_MODE_DISABLED;
      }
      // Montage Gen2 LRBUF W/A
      if (((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE)&& ((*dimmNvList)[dimm].lrbufGen == 0x01) && ((*dimmNvList)[dimm].lrbufRid == LRBUF_MONTAGE_RID_GEN2_A0)) {
        (*channelNvList)[ch].chOneRankTimingModeEnable = ONE_RANK_TIMING_MODE_DISABLED;
      }
    } //ch
  } //dimm

#ifdef DEBUG_CODE_BLOCK
  for (ch = 0; ch < MaxChDdr; ch++) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "One Rank Timing Mode(1 = enabled): %d \n", (*channelNvList)[ch].chOneRankTimingModeEnable);

  } //ch
#endif //DEBUG_CODE_BLOCK

  // For each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Create a bit mask of channels that have at least 1 rank present for this dimm and rank 0
    //
    chBitmask = GetChBitmaskLrdimm (socket, dimm, 0);
    //
    // For each channel
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if (!((1 << ch) & chBitmask)) {
        continue;
      }
      if ((*channelNvList)[ch].chOneRankTimingModeEnable == ONE_RANK_TIMING_MODE_DISABLED) {
        continue;
      }

      ChdimmListTraining = GetChDimmListTraining (ch);
      dimmNvList = GetDimmNvList (Host, socket, ch);
      rankList = GetRankNvList (Host, socket, ch, dimm);
      //
      // Save off the original rank 0 values first
      //
      if ((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) {
        backsideRankMax = (*dimmNvList)[dimm].numDramRanks - 1;
      } else {
        backsideRankMax = 0;
      }
      for (backsideRank = 0; backsideRank <= backsideRankMax; backsideRank++) {
        for (strobe = 0; strobe < MSVx4; strobe++) {
          (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBC2x3x[backsideRank][strobe] = (*rankList)[backsideRank].lrBuf_FxBC2x3x[strobe];
          (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBC4x5x[backsideRank][strobe] = (*rankList)[backsideRank].lrBuf_FxBC4x5x[strobe];
          if (strobe < MSVx8) {
            (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBCCxEx[backsideRank][strobe] = (*rankList)[backsideRank].lrBuf_FxBCCxEx[strobe];
          }
        } //strobe
      } // backsideRank loop

      //
      // For each strobe
      //
      for (strobe = 0; strobe < MSVx4; strobe++) {
        //
        // find the average backside Recieve Enable (RX phase + cycle) and Read Delay.
        //
        averageRecieveEnable = 0;
        averageReadDelay = 0;
        //
        // For each rank
        //
        for (backsideRank = 0; backsideRank < (*dimmNvList)[dimm].numDramRanks; backsideRank++) {
          rxPhase = (INT16) ((*rankList)[backsideRank].lrBuf_FxBC2x3x[strobe]);
          rxCycle = (INT16) ((((*rankList)[backsideRank].lrBuf_FxBCCxEx[strobe % MSVx8]) >> (4 * (strobe / MSVx8))) & 0x0F);
          if (rxCycle > 2) {
            rxCycle = -1 * (rxCycle & 0x3);
          }
          rxCycle += 2; // add 2 just to avoid negative values
          averageRecieveEnable += ((rxCycle * 64) + rxPhase);
          rxDelay = (INT16) ((*rankList)[backsideRank].lrBuf_FxBC4x5x[strobe]);
          if (rxDelay > 15) {
            rxDelay = -1 * (rxDelay - 16);
          }
          averageReadDelay += rxDelay;
        } //backsideRank
        averageRecieveEnable = averageRecieveEnable / (*dimmNvList)[dimm].numDramRanks;
        averageRxCycle = (averageRecieveEnable / 64) - 2; // subtract 2 to remove the previous adjustment for negative numbers
        averageRxPhase = averageRecieveEnable % 64;
        averageReadDelay = averageReadDelay / (*dimmNvList)[dimm].numDramRanks;

        (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC2x3x[strobe] = (UINT8) averageRxPhase;
        if (averageRxCycle < 0) {
          averageRxCycle = ABS (averageRxCycle) + 4;
        }
        (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCCxEx[strobe % MSVx8] &= (UINT8) (0xF0 >> (4 * (strobe / MSVx8)));
        (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCCxEx[strobe % MSVx8] |= (UINT8) (averageRxCycle << (4 * (strobe / MSVx8)));

        if (averageReadDelay < 0) {
          averageReadDelay = ABS (averageReadDelay) + 16;
        }
        (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC4x5x[strobe] = (UINT8) averageReadDelay;
      } //strobe
      //
      // Store One Rank Timing mode values into rank 0
      //
      if ((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) {
        backsideRankMax = (*dimmNvList)[dimm].numDramRanks - 1;
      } else {
        backsideRankMax = 0;
      }
      rankStruct = GetRankStruct (Host, socket, ch, dimm);
      for (backsideRank = 0; backsideRank <= backsideRankMax; backsideRank++) {
        for (strobe = 0; strobe < MSVx4; strobe++) {
          (*rankStruct)[backsideRank].cachedLrBuf_FxBC2x3x[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC2x3x[strobe];
          (*rankStruct)[backsideRank].cachedLrBuf_FxBC4x5x[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC4x5x[strobe];
          if (strobe < MSVx8) {
            (*rankStruct)[backsideRank].cachedLrBuf_FxBCCxEx[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCCxEx[strobe];
          }
          (*rankList)[backsideRank].lrBuf_FxBC2x3x[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC2x3x[strobe];
          (*rankList)[backsideRank].lrBuf_FxBC4x5x[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC4x5x[strobe];
          if (strobe < MSVx8) {
            (*rankList)[backsideRank].lrBuf_FxBCCxEx[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCCxEx[strobe];
          }
        } // strobe loop

        //
        // program new rank 0 values and enable One Rank Timing Mode
        //
        //
        // Write F[0]BC2x/F[0]BC3x
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC2x3x[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC2x, ALL_DATABUFFERS);

        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC2x3x[MSVx8]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC3x, ALL_DATABUFFERS);
        //
        // Write F[0]BC4x/F[0]BC5x
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC4x5x[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC4x, ALL_DATABUFFERS);

        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC4x5x[MSVx8]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC5x, ALL_DATABUFFERS);
        //
        // Write F0 BCCxEx
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCCxEx[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BCCx, ALL_DATABUFFERS);
      } // backsideRank loop
      //
      // Write to F0 BC1x, enabling One Rank Timing Mode
      //
      (*dimmNvList)[dimm].lrBuf_BC1x = (*dimmNvList)[dimm].lrBuf_BC1x | LRDIMM_ONE_RANK_TIMING_MODE;
      controlWordData = (*dimmNvList)[dimm].lrBuf_BC1x;
      WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC1x);

    } //ch
  } //dimm

  return SUCCESS;
#else // !DDR5_SUPPORT
  return FAILURE;
#endif // !DDR5_SUPPORT
} //Ddr4LrdimmBacksideRxOneRankTimingMode


/**

  Disable LRDIMM Backside One Rank Timing Mode

  @param Host   - Pointer to sysHost
  @param socket - socket number

  @retval status

**/
UINT32
Ddr4LrdimmBacksideDisableOneRankTimingMode (
  PSYSHOST  Host,
  UINT8     socket
  )
{
#ifndef DDR5_SUPPORT
  UINT8                             ch;
  UINT8                             dimm;
  UINT32                            chBitmask;
  UINT8                             strobe;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct DimmDeviceTraining         (*ChdimmListTraining)[MAX_DIMM];
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             *controlWordDataPtr;
  UINT8                             controlWordData;
  UINT8                             backsideRank;
  UINT8                             backsideRankMax;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  //
  // For each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Create a bit mask of channels that have at least 1 rank present for this dimm and rank 0
    //
    chBitmask = GetChBitmaskLrdimm (socket, dimm, 0);
    //
    // For each channel
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if (!((1 << ch) & chBitmask)) {
        continue;
      }
      if ((*channelNvList)[ch].chOneRankTimingModeEnable == ONE_RANK_TIMING_MODE_DISABLED) {
        continue;
      }

      ChdimmListTraining = GetChDimmListTraining (ch);
      dimmNvList = GetDimmNvList (Host, socket, ch);
      rankList = GetRankNvList (Host, socket, ch, dimm);
      //
      // restore original rank 0
      //
      if ((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) {
        backsideRankMax = (*dimmNvList)[dimm].numDramRanks - 1;
      } else {
        backsideRankMax = 0;
      }
      rankStruct = GetRankStruct (Host, socket, ch, dimm);
      for (backsideRank = 0; backsideRank <= backsideRankMax; backsideRank++) {
        for (strobe = 0; strobe < MSVx4; strobe++) {
          (*rankStruct)[backsideRank].cachedLrBuf_FxBC2x3x[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBC2x3x[backsideRank][strobe];
          (*rankStruct)[backsideRank].cachedLrBuf_FxBC4x5x[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBC4x5x[backsideRank][strobe];
          (*rankStruct)[backsideRank].cachedLrBuf_FxBC8x9x[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBC8x9x[backsideRank][strobe];
          (*rankStruct)[backsideRank].cachedLrBuf_FxBCAxBx[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBCAxBx[backsideRank][strobe];
          if (strobe < MSVx8) {
            (*rankStruct)[backsideRank].cachedLrBuf_FxBCCxEx[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBCCxEx[backsideRank][strobe];
            (*rankStruct)[backsideRank].cachedLrBuf_FxBCDxFx[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBCDxFx[backsideRank][strobe];
          }
          (*rankList)[backsideRank].lrBuf_FxBC2x3x[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBC2x3x[backsideRank][strobe];
          (*rankList)[backsideRank].lrBuf_FxBC4x5x[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBC4x5x[backsideRank][strobe];
          (*rankList)[backsideRank].lrBuf_FxBC8x9x[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBC8x9x[backsideRank][strobe];
          (*rankList)[backsideRank].lrBuf_FxBCAxBx[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBCAxBx[backsideRank][strobe];
          if (strobe < MSVx8) {
            (*rankList)[backsideRank].lrBuf_FxBCCxEx[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBCCxEx[backsideRank][strobe];
            (*rankList)[backsideRank].lrBuf_FxBCDxFx[strobe] = (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBCDxFx[backsideRank][strobe];
          }
        } // strobe loop

        //
        // program rank 0 values and disable One Rank Timing Mode
        //
        //
        // Write F[0]BC2x/F[0]BC3x
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC2x3x[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC2x, ALL_DATABUFFERS);

        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC2x3x[MSVx8]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC3x, ALL_DATABUFFERS);
        //
        // Write F[0]BC4x/F[0]BC5x
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC4x5x[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC4x, ALL_DATABUFFERS);

        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC4x5x[MSVx8]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC5x, ALL_DATABUFFERS);
        //
        // Write F0 BCCxEx
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCCxEx[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BCCx, ALL_DATABUFFERS);
        //
        // Write F[0]BC8x/F[0]BC9x
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC8x9x[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC8x, ALL_DATABUFFERS);

        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC8x9x[MSVx8]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC9x, ALL_DATABUFFERS);
        //
        // Write F[0]BCAx/F[0]BCBx
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCAxBx[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BCAx, ALL_DATABUFFERS);

        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCAxBx[MSVx8]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BCBx, ALL_DATABUFFERS);
        //
        // F0BCDx for backside rank 0
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCDxFx[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BCDx, ALL_DATABUFFERS);
      } // backsideRank loop
      //
      // Write to F0 BC1x, disabling One Rank Timing Mode
      //
      (*dimmNvList)[dimm].lrBuf_BC1x = (*dimmNvList)[dimm].lrBuf_BC1x & ~LRDIMM_ONE_RANK_TIMING_MODE;
      controlWordData = (*dimmNvList)[dimm].lrBuf_BC1x;
      WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC1x);
    } //ch
  } //dimm

  return SUCCESS;
#else // !DDR5_SUPPORT
  return FAILURE;
#endif // !DDR5_SUPPORT
} //Ddr4LrdimmBacksideDisableOneRankTimingMode


/**

  Perform LRDIMM Backside One Rank Timing Mode Write
  calculations and enable the support.

  @param Host   - Pointer to sysHost
  @param socket - socket number

  @retval status

**/
UINT32
Ddr4LrdimmBacksideTxOneRankTimingMode (
  PSYSHOST  Host,
  UINT8     socket
  )
{
#ifndef DDR5_SUPPORT
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             backsideRank;
  UINT8                             backsideRankMax;
  UINT32                            chBitmask;
  UINT8                             strobe;
  INT16                             txCycle, averageTxCycle;
  INT16                             txPhase, averageTxPhase;
  INT16                             txDelay;
  INT16                             averageWriteLeveling, averageWriteDelay;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct DimmDeviceTraining         (*ChdimmListTraining)[MAX_DIMM];
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             *controlWordDataPtr;
  UINT8                             controlWordData;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  //
  // For each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Create a bit mask of channels that have at least 1 rank present for this dimm and rank 0
    //
    chBitmask = GetChBitmaskLrdimm (socket, dimm, 0);
    //
    // For each channel
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if (!((1 << ch) & chBitmask)) {
        continue;
      }
      if ((*channelNvList)[ch].chOneRankTimingModeEnable == ONE_RANK_TIMING_MODE_DISABLED) {
        continue;
      }

      ChdimmListTraining = GetChDimmListTraining (ch);
      dimmNvList = GetDimmNvList (Host, socket, ch);
      rankList = GetRankNvList (Host, socket, ch, dimm);
      //
      // Save off the original Tx rank 0 values first
      //
      if ((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) {
        backsideRankMax = (*dimmNvList)[dimm].numDramRanks - 1;
      } else {
        backsideRankMax = 0;
      }
      for (backsideRank = 0; backsideRank <= backsideRankMax; backsideRank++) {
        for (strobe = 0; strobe < MSVx4; strobe++) {
          (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBC8x9x[backsideRank][strobe] = (*rankList)[backsideRank].lrBuf_FxBC8x9x[strobe];
          (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBCAxBx[backsideRank][strobe] = (*rankList)[backsideRank].lrBuf_FxBCAxBx[strobe];
          if (strobe < MSVx8) {
            (*ChdimmListTraining)[dimm].originalRank0Lrbuf_FxBCDxFx[backsideRank][strobe] = (*rankList)[backsideRank].lrBuf_FxBCDxFx[strobe];
          }
        } // strobe loop
      } // backsideRank loop

      //
      // For each strobe
      //
      for (strobe = 0; strobe < MSVx4; strobe++) {
        //
        // find the average backside Dram Write Leveling (RX phase + cycle) and Write Delay.
        //
        averageWriteLeveling = 0;
        averageWriteDelay = 0;
        //
        // For each rank
        //
        for (backsideRank = 0; backsideRank < (*dimmNvList)[dimm].numDramRanks; backsideRank++) {
          txPhase = (INT16) ((*rankList)[backsideRank].lrBuf_FxBCAxBx[strobe]);
          txCycle = (INT16) ((((*rankList)[backsideRank].lrBuf_FxBCDxFx[strobe % MSVx8]) >> (4 * (strobe / MSVx8))) & 0x0F);
          if (txCycle > 2) {
            txCycle = -1 * (txCycle & 0x3);
          }
          txCycle += 2; // add 2 just to avoid negative values
          averageWriteLeveling += ((txCycle * 64) + txPhase);
          txDelay = (INT16) ((*rankList)[backsideRank].lrBuf_FxBC8x9x[strobe]);
          if (txDelay > 15) {
            txDelay = -1 * (txDelay - 16);
          }
          averageWriteDelay += txDelay;
        } //backsideRank
        averageWriteLeveling = averageWriteLeveling / (*dimmNvList)[dimm].numDramRanks;
        averageTxCycle = (averageWriteLeveling / 64) - 2; // subtract 2 to remove the previous adjustment for negative numbers
        averageTxPhase = averageWriteLeveling % 64;
        averageWriteDelay = averageWriteDelay / (*dimmNvList)[dimm].numDramRanks;

        (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCAxBx[strobe] = (UINT8) averageTxPhase;
        if (averageTxCycle < 0) {
          averageTxCycle = ABS (averageTxCycle) + 4;
        }
        (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCDxFx[strobe % MSVx8] &= (UINT8) (0xF0 >> (4 * (strobe / MSVx8)));
        (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCDxFx[strobe % MSVx8] |= (UINT8) (averageTxCycle << (4 * (strobe / MSVx8)));

        if (averageWriteDelay < 0) {
          averageWriteDelay = ABS (averageWriteDelay) + 16;
        }
        (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC8x9x[strobe] = (UINT8) averageWriteDelay;

      } //strobe
      //
      // Store One Rank Timing mode values into rank 0
      //
      if ((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) {
        backsideRankMax = (*dimmNvList)[dimm].numDramRanks - 1;
      } else {
        backsideRankMax = 0;
      }
      rankStruct = GetRankStruct (Host, socket, ch, dimm);
      for (backsideRank = 0; backsideRank <= backsideRankMax; backsideRank++) {
        for (strobe = 0; strobe < MSVx4; strobe++) {
          (*rankStruct)[backsideRank].cachedLrBuf_FxBC2x3x[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC2x3x[strobe];
          (*rankStruct)[backsideRank].cachedLrBuf_FxBC4x5x[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC4x5x[strobe];
          (*rankStruct)[backsideRank].cachedLrBuf_FxBC8x9x[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC8x9x[strobe];
          (*rankStruct)[backsideRank].cachedLrBuf_FxBCAxBx[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCAxBx[strobe];
          if (strobe < MSVx8) {
            (*rankStruct)[backsideRank].cachedLrBuf_FxBCCxEx[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCCxEx[strobe];
            (*rankStruct)[backsideRank].cachedLrBuf_FxBCDxFx[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCDxFx[strobe];
          }
          (*rankList)[backsideRank].lrBuf_FxBC2x3x[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC2x3x[strobe];
          (*rankList)[backsideRank].lrBuf_FxBC4x5x[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC4x5x[strobe];
          (*rankList)[backsideRank].lrBuf_FxBC8x9x[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBC8x9x[strobe];
          (*rankList)[backsideRank].lrBuf_FxBCAxBx[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCAxBx[strobe];
          if (strobe < MSVx8) {
            (*rankList)[backsideRank].lrBuf_FxBCCxEx[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCCxEx[strobe];
            (*rankList)[backsideRank].lrBuf_FxBCDxFx[strobe] = (*ChdimmListTraining)[dimm].oneRankTimingModeLrbuf_FxBCDxFx[strobe];
          }
        } // strobe loop

        //
        // program new rank 0 values and enable One Rank Timing Mode
        //
        //
        // Write F[0]BC2x/F[0]BC3x
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC2x3x[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC2x, ALL_DATABUFFERS);

        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC2x3x[MSVx8]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC3x, ALL_DATABUFFERS);
        //
        // Write F[0]BC4x/F[0]BC5x
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC4x5x[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC4x, ALL_DATABUFFERS);

        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC4x5x[MSVx8]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC5x, ALL_DATABUFFERS);
        //
        // Write F0 BCCx
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCCxEx[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BCCx, ALL_DATABUFFERS);
        //
        // Write F[0]BC8x/F[0]BC9x
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC8x9x[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC8x, ALL_DATABUFFERS);

        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBC8x9x[MSVx8]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BC9x, ALL_DATABUFFERS);
        //
        // Write F[0]BCAx/F[0]BCBx
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCAxBx[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BCAx, ALL_DATABUFFERS);

        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCAxBx[MSVx8]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BCBx, ALL_DATABUFFERS);
        //
        // Write F0 BCDx
        //
        controlWordDataPtr = &((*rankList)[backsideRank].lrBuf_FxBCDxFx[0]);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, backsideRank, LRDIMM_BCDx, ALL_DATABUFFERS);
      } // backsideRank loop

      //
      // Write to F0 BC1x, enabling One Rank Timing Mode
      //
      (*dimmNvList)[dimm].lrBuf_BC1x = (*dimmNvList)[dimm].lrBuf_BC1x | LRDIMM_ONE_RANK_TIMING_MODE;
      controlWordData = (*dimmNvList)[dimm].lrBuf_BC1x;
      WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC1x);

    } //ch
  } //dimm

  return SUCCESS;
#else // !DDR5_SUPPORT
  return FAILURE;
#endif // !DDR5_SUPPORT
} //Ddr4LrdimmBacksideTxOneRankTimingMode

/**

  Perform LRDIMM Backside Training (TX)

  @param Host  - Pointer to sysHost

  @retval status

**/
UINT32
Ddr4LrdimmBacksideTxFineWL (
  PSYSHOST  Host
  )
{
  UINT8                             socket;
  UINT32                            status = SUCCESS;
  struct channelNvram               (*channelNvList)[MAX_CH];
  UINT8                             ch;
  UINT8                             dimm;
  SYS_SETUP                         *Setup;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  socket = Host->var.mem.currentSocket;

  channelNvList = GetChannelNvList (Host, socket);

  //
  // Reasons to just return with SUCCESS...
  //
  if ((!IsMemFlowEnabled (LrdimmBacksideTxTraining))      ||   // Training step is disabled
      (Host->nvram.mem.socket[socket].enabled == 0)       ||   // No Socket present
      (Host->nvram.mem.socket[socket].maxDimmPop == 0)    ||   // No DIMM present
      (Host->nvram.mem.socket[socket].lrDimmPresent == 0) ||
      (Host->nvram.mem.dramType != SPD_TYPE_DDR4))  // No LRDIMM present
  {

    if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
      SkipDdr4LrdimmBacksideTxFine (Host, socket);
    }

    return SUCCESS;
  }

  //
  // If One Rank Timing mode is enabled, disable it and restore multi-rank mode for the purposes of backside training
  //
  if (Setup->mem.oneRankTimingModeEn) {
    status = KEEP_FAILURE (Ddr4LrdimmBacksideDisableOneRankTimingMode (Host, socket), status);
  }

  //
  // DRAM Interface WL (DWL)
  //
  status = KEEP_FAILURE (DramInterfaceWlTraining (Host, socket), status);

  //
  // Set function control nibble back to 0
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      if ((*channelNvList)[ch].dimmList[dimm].rcLrFunc != 0) {
        WriteBC (Host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
        (*channelNvList)[ch].dimmList[dimm].rcLrFunc = 0;
      }
    } // ch
  } //dimm

  return status;
} //Ddr4LrdimmBacksideTxFineWL

/**

  Perform LRDIMM Backside Training (TX)

  @param Host  - Pointer to sysHost

  @retval status

**/
UINT32
Ddr4LrdimmBacksideTxCoarseWL (
  PSYSHOST  Host
  )
{
  UINT8                             socket;
  UINT32                            status = SUCCESS;
  UINT8                             ch;
  UINT8                             dimm;
  struct channelNvram               (*channelNvList)[MAX_CH];
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;

  //
  // Reasons to just return with SUCCESS...
  //
  if ((!IsMemFlowEnabled (LrdimmBacksideTxTraining))      ||   // Training step is disabled
      (Host->nvram.mem.socket[socket].enabled == 0)       ||   // No Socket present
      (Host->nvram.mem.socket[socket].maxDimmPop == 0)    ||   // No DIMM present
      (Host->nvram.mem.socket[socket].lrDimmPresent == 0) ||
      (Host->nvram.mem.dramType != SPD_TYPE_DDR4))  // No LRDIMM present
  {

    if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
      SkipDdr4LrdimmBacksideTxCoarse (Host, socket);
    }
    return SUCCESS;
  }

  //
  // MDQS Coarse WL step (CWL)
  //
  status = KEEP_FAILURE (MDQCoarseWlTraining (Host, socket), status);

  channelNvList = GetChannelNvList (Host, socket);
  // Set function control nibble back to 0
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      if ((*channelNvList)[ch].dimmList[dimm].rcLrFunc != 0) {
        WriteBC (Host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
        (*channelNvList)[ch].dimmList[dimm].rcLrFunc = 0;
      }
    } // ch
  } //dimm
  return status;
} //Ddr4LrdimmBacksideTxCoarseWL

/**

  Perform LRDIMM Backside Training (TX)

  @param Host  - Pointer to sysHost

  @retval status

**/
UINT32
Ddr4LrdimmBacksideTxDelay (
  PSYSHOST  Host
  )
{
  UINT8                             socket;
  UINT32                            status = SUCCESS;
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  struct channelNvram               (*channelNvList)[MAX_CH];
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  socket = Host->var.mem.currentSocket;
  //
  // Reasons to just return with SUCCESS...
  //
  if ((!IsMemFlowEnabled (LrdimmBacksideTxTraining))      ||   // Training step is disabled
      (Host->nvram.mem.socket[socket].enabled == 0)       ||   // No Socket present
      (Host->nvram.mem.socket[socket].maxDimmPop == 0)    ||   // No DIMM present
      (Host->nvram.mem.socket[socket].lrDimmPresent == 0) ||
      (Host->nvram.mem.dramType != SPD_TYPE_DDR4))  // No LRDIMM present
  {

    if (FeaturePcdGet (PcdCosimBuild) == TRUE) {
      SkipDdr4LrdimmBacksideTxDelay (Host, socket);
    }
    return SUCCESS;
  }

  //
  // MDQS-MDQ Write Delay Training step (MWD)
  //
  status = KEEP_FAILURE (MDQWriteDelayTraining (Host, socket), status);

  status = KEEP_FAILURE (Ddr4LrdimmBacksideTxOneRankTimingMode (Host, socket), status);

  channelNvList = GetChannelNvList (Host, socket);
  // Set function control nibble back to 0
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }

        // for (strobe = 0; strobe < MSVx4; strobe++) {
        // //
        // //Update the "cached" register values from the nvram "actual" register values
        // //
        // (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe] = (*channelNvList)[ch].dimmList[dimm].rankList[rank].lrBuf_FxBC2x3x[strobe];
        // (*rankStruct)[rank].cachedLrBuf_FxBC4x5x[strobe] = (*channelNvList)[ch].dimmList[dimm].rankList[rank].lrBuf_FxBC4x5x[strobe];
        // (*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] = (*channelNvList)[ch].dimmList[dimm].rankList[rank].lrBuf_FxBC8x9x[strobe];
        // } // strobe
        if ((*channelNvList)[ch].dimmList[dimm].rcLrFunc != 0) {
          WriteBC (Host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
          (*channelNvList)[ch].dimmList[dimm].rcLrFunc = 0;
        }
      } // ch
    } //rank
  } //dimm

  return status;
} //Ddr4LrdimmBacksideTxDelay


/**

  Set F[x]BC4x and F[x]BC5x to the respective settings that are in the center of the passing region.

  @param Host              - Pointer to sysHost
  @param ch                - Channel number
  @param dimm              - Dimm number
  @param rank              - Rank number
  @param strobe            - Strobe number

  @retval status

**/
UINT32
SetLrdimmMrdResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT16    centerPoint[MAX_STROBE]
  )
{
  UINT32                            status = SUCCESS;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             mdqsReadDelay = 0;
  UINT8                             strobe;
  UINT8                             *controlWordDataPtr;
  UINT8                             controlWordFunc;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  rankList = GetRankNvList (Host, socket, ch, dimm);
  rankStruct = GetRankStruct (Host, socket, ch, dimm);

  for (strobe = 0; strobe < MSVx4; strobe++) {
    if ((centerPoint[strobe]/4) < 15) {
      mdqsReadDelay = (UINT8) (((centerPoint[strobe]/4) ^ 31) & 0x1F);
    } else {
      mdqsReadDelay = (UINT8) (((centerPoint[strobe]/4) - 15) & 0x1F);
    }
    //
    // store the data to write for later use
    //
    (*rankList)[rank].lrBuf_FxBC4x5x[strobe] = mdqsReadDelay;
    (*rankStruct)[rank].cachedLrBuf_FxBC4x5x[strobe] = (*rankList)[rank].lrBuf_FxBC4x5x[strobe];
  } //strobe
  controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBC4x5x[0];
  controlWordFunc = rank;
  WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BC4x, ALL_DATABUFFERS);
  controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBC4x5x[MSVx8];
  controlWordFunc = rank;
  WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BC5x, ALL_DATABUFFERS);

  return status;
} //SetLrdimmMrdResults


/**

  Set F[x]BC8x and F[x]BC9x to the respective settings that are in the center of the passing region.

  @param Host              - Pointer to sysHost
  @param ch                - Channel number
  @param dimm              - Dimm number
  @param rank              - Rank number
  @param strobe            - Strobe number

  @retval status

**/
UINT32
SetLrdimmMwdResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT16    centerPoint[MAX_STROBE]
  )
{
  UINT32                            status = SUCCESS;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             mdqsWriteDelay = 0;
  UINT8                             strobe;
  UINT8                             *controlWordDataPtr;
  UINT8                             controlWordFunc;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  rankList = GetRankNvList (Host, socket, ch, dimm);
  rankStruct = GetRankStruct (Host, socket, ch, dimm);

  for (strobe = 0; strobe < MSVx4; strobe++) {
    if ((centerPoint[strobe]/4) < 15) {
      mdqsWriteDelay = (UINT8) (((centerPoint[strobe]/4) ^ 31) & 0x1F);
    } else {
      mdqsWriteDelay = (UINT8) (((centerPoint[strobe]/4) - 15) & 0x1F);
    }
    //
    // store the data to write for later use
    //
    (*rankList)[rank].lrBuf_FxBC8x9x[strobe] = mdqsWriteDelay;
    (*rankStruct)[rank].cachedLrBuf_FxBC8x9x[strobe] = (*rankList)[rank].lrBuf_FxBC8x9x[strobe];
  } //strobe
  controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBC8x9x[0];
  controlWordFunc = rank;
  WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BC8x, ALL_DATABUFFERS);
  controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBC8x9x[MSVx8];
  controlWordFunc = rank;
  WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BC9x, ALL_DATABUFFERS);

  return status;
} //SetLrdimmMwdResults


/**

  Get's the results from the current test

  @param Host      - Pointer to sysHost
  @param socket    - socket number
  @param dimm      - DIMM number
  @param rank      - rank number of the DIMM
  @param piDelay   - CLK adjustment setting for this test
  @param step      - which training step is this called from, using same value as training mode for convenience

  @retval N/A

**/
VOID
GetLrdimmTrainResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT16    piDelay,
  UINT8     step
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT8               swizzleStrobe;
  UINT8               dWord;
  UINT32              lrdimmFeedBack;
  struct TrainingResults (*lrTrainRes)[MAX_CH][MAX_STROBE];
    UINT8               DumArr[7] = {0, 0, 0, 0, 0, 0, 0};
  UINT32              chBitmask;
  UINT32              databit;
  UINT32              stepSizeBits = (1 << LRDIMM_BACKSIDE_STEP_SIZE) - 1;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);

  if (step == LRDIMM_MREP_TRAINING_MODE) {
    lrTrainRes = &Host->var.mem.lrTrainRes;
  } else {
    lrTrainRes = &Host->var.mem.lrDwlTrainRes;
  }

  dWord = (UINT8) ((piDelay - LRDIMM_BACKSIDE_PI_START) / 32);

  if (step == LRDIMM_MREP_TRAINING_MODE) {
    //
    // ii.  Execute the CPGC MPR RD sequence
    //
    // Send a burst of 7 read commands back to back (4 DCLK apart) using MC_SPECIAL_COMMAND CRs
    //
    RunIOTest (Host, socket, chBitmask, BasicVA, DumArr, 1, 0);
    //
    // iii. As the CPGC test is still running, after a TBD wait time, read the results of the DQ sampling in the DDRIO.
    FixedDelayMicroSecond (1);
  }
  //
  // enter sense amp training mode here to latch results
  //
  SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_SENSE_AMP);

  if (FeaturePcdGet (PcdCosimBuild) == TRUE) {

    //
    // Delay if this is a CTE build, otherwise do nothing.
    //

    CteDelayDclk (100);
  } else {

    FixedDelayMicroSecond (1);
  }

  //
  // Exit sense amp training mode
  //
  SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);

  if (step == LRDIMM_MREP_TRAINING_MODE) {
    //
    //iv. Stop the CPGC test in prep for the next phase setting.
    //
    CpgcGlobalTestStop (Host, socket, chBitmask, NO_SUBCH);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (!((1 << ch) & chBitmask)) {
      continue;
    }

    for (strobe = 0; strobe < MSVx4; strobe++) {
      swizzleStrobe = CheckBacksideSwizzle (Host, socket, ch, dimm, strobe);
      lrdimmFeedBack = LrDimmReadTrainFeedback (Host, socket, ch, swizzleStrobe, 0xFF);

      //
      // Add full results to a larger results structure (populate odd ticks same as the even, so I can reuse same results routines
      // as Host receive enable)
      //

      if (lrdimmFeedBack) { // mapping a clock cycle so any bit of 4 passing will be logged as a 1
        databit = (stepSizeBits << (piDelay % 32)); // 3 is used to fill 2 locations in the results array
        (*lrTrainRes)[ch][strobe].results[dWord] |= databit;
      }

    } // strobe loop
  } //ch loop
} // GetLrdimmTrainingResults


//M88DDR4 RCD/DB Sighting Report, sighting 1302101
// montage M88DDR4DB01-A1 mrec workaround for DB A1
//
VOID
ReadBCWAsynByDq (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    rank,
  UINT8    controlWordData[MAX_STROBE/2],
  UINT8    controlWordFunc,
  UINT8    controlWordAddr
  )
/**

    Read BC Work Around By DQ

    @param Host                          - Pointer to SysHost
    @param socket                        - Processor Socket
    @param ch                            - Channel on socket
    @param dimm                          - DIMM on channel
    @param rank                          - Rank on DIMM
    @param controlWordData[MAX_STROBE/2] - Control Word Data
    @param controlWordFunc               - Control Word Function
    @param controlWordAddr               - Control Word Address

    @retval N/A

**/
{
  UINT8   bit;
  UINT8   strobe;
  UINT32  lrDimmFeedback;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  ZeroMem (controlWordData, MSVx8);

  lrDimmFeedback = 0;
  MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);

  WriteLrbuf (Host, socket, ch, dimm, 0, 0x37, LRDIMM_F7, LRDIMM_BC5x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 2, LRDIMM_F7, LRDIMM_BC6x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xb7, LRDIMM_F7, LRDIMM_BC5x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 2, LRDIMM_F7, LRDIMM_BC6x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);

  WriteLrbuf (Host, socket, ch, dimm, 0, controlWordAddr, LRDIMM_F7, LRDIMM_BC5x);
  MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);

  for (bit = 0; bit < 8; bit++) {
    if (controlWordFunc > 7) {
      WriteLrbuf (Host, socket, ch, dimm, 0, 1<<4|bit<<1|1<<0, LRDIMM_F6, LRDIMM_BC6x);
    } else {
      WriteLrbuf (Host, socket, ch, dimm, 0, controlWordFunc<<5|bit<<1|1<<0, LRDIMM_F6, LRDIMM_BC6x);
    }

    FixedDelayMicroSecond (10);

    // this should still work for SKX. We take bit0 feedback of the lower strobes and shove it into the other 7 bit positions
    for (strobe = 0; strobe < (MSVx4/2); strobe++) {
      lrDimmFeedback = LrDimmReadTrainFeedback (Host, socket, ch, strobe, bit);
      controlWordData[strobe] = controlWordData[strobe]| (UINT8) lrDimmFeedback;
    }
  }

  MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0, LRDIMM_F6, LRDIMM_BC6x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0x37, LRDIMM_F7, LRDIMM_BC5x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0, LRDIMM_F7, LRDIMM_BC6x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xb7, LRDIMM_F7, LRDIMM_BC5x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0, LRDIMM_F7, LRDIMM_BC6x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
  MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);
}

VOID
MRECWorkaround2Intel (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  INT8      cycleSetting
  )
{
  UINT8                         strobe;
  struct lrMrecTrainingResults  (*lrMrecTrainRes)[MAX_CH][MAX_STROBE];
  struct rankDevice             (*rankStruct)[MAX_RANK_DIMM];
  UINT8                         controlWordData [MAX_STROBE/2];
  UINT8                         TempMprdata[8][MAX_STROBE/2];
  UINT8                         Mprdata[MAX_STROBE];
  UINT8                         trainingmode;
  UINT8                         i,j;
  UINT8                         offset, function;
  UINT8                         F6BC4X[MAX_STROBE/2];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  lrMrecTrainRes = &Host->var.mem.lrMrecTrainRes;
  rankStruct = GetRankStruct (Host, socket, ch, dimm);

  trainingmode = (*rankStruct)[rank].CurrentLrdimmTrainingMode;
  //exit MRD mode
  WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_NORMAL_MODE, LRDIMM_F0, LRDIMM_BC0C);
  (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_NORMAL_MODE;

  for (i = 0; i < 8; i++) {
    if (i > 3) {
      function = 6;
      offset = (i - 4) << 4;
    } else {
      function = 5;
      offset = i << 4;
    }
    ReadBCWAsynByDq (Host,socket,ch,dimm, 0, TempMprdata[i], function, offset);
  }

  for (i = 0; i < MSVx4; i++) {
    Mprdata[i] = 0;
    for (j = 0; j < 8; j++) {
      if (i < MSVx8) {
        Mprdata[i] = Mprdata[i] | ((TempMprdata[j][i] & 0x01) << j);
      } else {
        Mprdata[i] = Mprdata[i] | (((TempMprdata[j][i-MSVx8]>>4) & 0x01) << j);
      }
    }
  }

  MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xac, LRDIMM_F7, LRDIMM_BC5x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0x68, LRDIMM_F7, LRDIMM_BC6x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
  MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);

  ReadBCWAsynByDq (Host, socket, ch, dimm, 0, F6BC4X, LRDIMM_F6, LRDIMM_BC4x);

  for (strobe = 0; strobe < (MSVx4/2); strobe++) {
    F6BC4X[strobe] = F6BC4X[strobe] & 0x1f;
  }

  WriteLrbufPBA (Host, socket, ch, dimm, rank, F6BC4X, LRDIMM_F6, LRDIMM_BC4x, ALL_DATABUFFERS);

  ReadBCWAsynByDq (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F6, LRDIMM_BC5x);

  for (strobe = 0; strobe < (MSVx4/2); strobe++) {
    if (controlWordData[strobe] == Mprdata[strobe]) {
      (*lrMrecTrainRes)[ch][strobe].results = (*lrMrecTrainRes)[ch][strobe].results | (1 << (cycleSetting + 2));
    }
  }

  for (strobe = 0; strobe < (MSVx4/2); strobe++) {
    F6BC4X[strobe] = (F6BC4X[strobe] & 0x1f) | 0x80;
  }

  WriteLrbufPBA (Host, socket, ch, dimm, rank, F6BC4X, LRDIMM_F6, LRDIMM_BC4x, ALL_DATABUFFERS);

  ReadBCWAsynByDq (Host,socket,ch,dimm, 0, controlWordData, LRDIMM_F6, LRDIMM_BC5x);

  for (strobe = 0; strobe < (MSVx4/2); strobe++) {
    if (controlWordData[strobe] == Mprdata[strobe + MSVx8]) {
      (*lrMrecTrainRes)[ch][strobe + MSVx8].results = (*lrMrecTrainRes)[ch][strobe + MSVx8].results | (1 << (cycleSetting + 2));
    }
  }

  MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xac, LRDIMM_F7, LRDIMM_BC5x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0x08, LRDIMM_F7, LRDIMM_BC6x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
  MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);

  if (trainingmode != LRDIMM_NORMAL_MODE) {
    WriteLrbuf (Host, socket, ch, dimm, 0, trainingmode, LRDIMM_F0, LRDIMM_BC0C);
    (*rankStruct)[rank].CurrentLrdimmTrainingMode = trainingmode;
  }
}


/**

  Get's the results from the current test

  @param Host      - Pointer to sysHost
  @param dimm      - DIMM number
  @param rank      - rank number of the DIMM

  @retval N/A

**/
VOID
GetLrdimmMrecResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  INT8      cycleSetting
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT8               swizzleStrobe = 0;
  UINT32              lrdimmFeedBack;

  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  struct lrMrecTrainingResults  (*lrMrecTrainRes)[MAX_CH][MAX_STROBE];
    UINT8               DumArr[7] = {1,1,1,1,1,1,1};
  UINT32              chBitmask;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
  lrMrecTrainRes = &Host->var.mem.lrMrecTrainRes;

  //
  // enter sense amp training mode
  //
  SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_SENSE_AMP);
  //
  //i.  Execute the CPGC MPR RD sequence
  //
  RunIOTest (Host, socket, chBitmask, BasicVA, DumArr, 1, 0);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (!((1 << ch) & chBitmask)) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    //
    //M88DDR4 RCD/DB Sighting Report, sighting 1302101
    // montage M88DDR4DB01-A1 mrec workaround for DB A1
    //
    if (((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE &&
         (*dimmNvList)[dimm].lrbufRid == LRBUF_MONTAGE_RID_A1) && cycleSetting > -2) {
      MRECWorkaround2Intel (Host, socket, ch, dimm, rank, cycleSetting);
    } else
    {
      for (strobe = 0; strobe < MSVx4; strobe++) {
        //
        // Skip if this is an ECC strobe when ECC is disabled
        //
        if (IsStrobeNotValid (Host, strobe)) {
          continue;
        }

        if (((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE && (*dimmNvList)[dimm].lrbufRid == LRBUF_MONTAGE_RID_A1) && cycleSetting == -2) {
          lrdimmFeedBack = 0;
        } else {
          swizzleStrobe = CheckBacksideSwizzle (Host, socket, ch, dimm, strobe);
          lrdimmFeedBack = LrDimmReadTrainFeedback (Host, socket, ch, swizzleStrobe, 0xFF);
        }
        if (lrdimmFeedBack) {
          //For 2TCLK preamable, check to see if previous cycle test was also passing, if so, select current one
          if (GetPreambleState (socket, READ_PREAMBLE) == PREAMBLE_2TCLK) {
            AdjustLrMrecTrainingResults (Host, socket, ch, strobe, *lrMrecTrainRes, cycleSetting);
          } else {
            (*lrMrecTrainRes)[ch][strobe].results = (*lrMrecTrainRes)[ch][strobe].results | (1 << (cycleSetting + 2));
          }
        }
      } // strobe loop
    }
  } // ch loop
  //
  // Exit sense amp training mode
  //
  SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);
} // GetLrdimmMrecResults

/**

  Checks the previous cycle training check and if passed, takes the most current one. This code assumes that
  we are in 2TCLK Mode and that 2 passing cycles is acceptable.
  This code may update the global variable var.mem.secondPass[Ch][Strobe]

  @param[in] Host                     - Pointer to sysHost
  @param[in] Socket                   - Socket number
  @param[in] Ch                       - Ch number
  @param[in] Strobe                   - Strobe number
  @param[in] lrMrecTrainingResults    - Pointer to LrdimmMrecResults
  @param[in] CycleSetting             - Nibble cycle setting number

  @retval N/A

**/
VOID
AdjustLrMrecTrainingResults (
  IN PSYSHOST  Host,
  IN UINT8 Socket,
  IN UINT8 Ch,
  IN UINT8 Strobe,
  IN struct lrMrecTrainingResults  lrMrecTrainRes[MAX_CH][MAX_STROBE],
  IN INT8 CycleSetting
  )
{
  // cycleSetting should fall within the range of -2 and 2
  if ((CycleSetting < MinLrMrecCycleSetting) || (CycleSetting > MaxLrMrecCycleSetting)) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, Strobe, NO_BIT,
                 "CycleSetting out of range\n");
    ASSERT (FALSE);
    return;
  }

  if (Host->var.mem.secondPass[Ch][Strobe] == FALSE) { // We may pass twice in 2tck mode due to long Rx preamble. Pick latest valid delay.
    //
    // Test results encoding:
    // BIT0 -> CycleSetting = -2
    // BIT1 -> CycleSetting = -1
    // BIT2 -> CycleSetting =  0
    // BIT3 -> CycleSetting =  1
    // BIT4 -> CycleSetting =  2
    //
    if (CycleSetting == MinLrMrecCycleSetting) { // Don't need analyze previous cycle test if on the first cycle test
      Host->var.mem.secondPass[Ch][Strobe] = TRUE;
      lrMrecTrainRes[Ch][Strobe].results = 1 << (CycleSetting + 2);
      return;
    } else {
      // Check previous cycle test to see if passed, if so, select current one
      if (lrMrecTrainRes[Ch][Strobe].results == (1 << (CycleSetting + 1))) {
        Host->var.mem.secondPass[Ch][Strobe] = TRUE;
        lrMrecTrainRes[Ch][Strobe].results = 1 << (CycleSetting + 2);
        return;

      }
    }
  }
  lrMrecTrainRes[Ch][Strobe].results = lrMrecTrainRes[Ch][Strobe].results | (1 << (CycleSetting + 2));
}


/**

  Get's the results from the current test

  @param Host      - Pointer to sysHost
  @param dimm      - DIMM number
  @param rank      - rank number of the DIMM

  @retval N/A

**/
VOID
GetLrdimmCwlResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  INT8      cycleDelay
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT8               swizzleStrobe;
  UINT32              lrdimmFeedBack;
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct channelNvram (*channelNvList)[MAX_CH];
  struct lrMrecTrainingResults  (*lrCwlTrainRes)[MAX_CH][MAX_STROBE];
  INT8                resultBitPosition;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  lrCwlTrainRes = &Host->var.mem.lrCwlTrainRes;
  channelNvList = GetChannelNvList (Host, socket);
  resultBitPosition = cycleDelay + 2; //(-2, -1, 0, 1, 2 maps to bit 0, 1, 2, 3, 4)

  //
  // enter sense amp training mode here to latch results
  //
  SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_SENSE_AMP);
  FixedDelayMicroSecond (1);

  //
  // Delay only if this is a CTE build, otherwise do nothing.
  //

  CteDelayQclk (40);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    for (strobe = 0; strobe < MSVx4; strobe++) {
      swizzleStrobe = CheckBacksideSwizzle (Host, socket, ch, dimm, strobe);
      if (((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE && (*dimmNvList)[dimm].lrbufRid == LRBUF_MONTAGE_RID_A1) && cycleDelay == -2) {
        lrdimmFeedBack = 0;
      } else {
        lrdimmFeedBack = LrDimmReadTrainFeedback (Host, socket, ch, swizzleStrobe, 0xFF);
      }
      if (lrdimmFeedBack == 0xF) {
        (*lrCwlTrainRes)[ch][strobe].results = (*lrCwlTrainRes)[ch][strobe].results | (1 << resultBitPosition);
      }
    } // strobe loop
  } // ch loop
  //
  // Exit sense amp training mode
  //
  SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);
} // GetLrdimmCwlResults


/**

  Get's the results from the current test

  @param Host              - Pointer to sysHost
  @param dimm              - DIMM number
  @param rank              - rank number of the DIMM
  @param delaySetting      - Encoded Delay MDQS setting (per F[3:0]BC4x register definition in JEDEC Spec)
  @param step              - which training step is this called from, using same value as training mode for convenience

  @retval N/A

**/
VOID
GetLrdimmMrdMwdResults (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm,
  UINT8     rank,
  UINT8     delaySetting,
  UINT8     step
  )
{
  UINT8               ch;
  UINT8               strobe;
  UINT8               swizzleStrobe;
  UINT8               dWord;
  UINT32              lrdimmFeedBack;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct TrainingResults  (*lrTrainRes)[MAX_CH][MAX_STROBE];
  UINT8               resultsCodedDelay;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  if (step == LRDIMM_MRD_TRAINING_MODE) {
    lrTrainRes = &Host->var.mem.lrMrdTrainRes;
  } else {
    lrTrainRes = &Host->var.mem.lrMwdTrainRes;
  }
  channelNvList = GetChannelNvList (Host, socket);

  //
  // The encoded read delay is such that values 0 to 15d map to settings 0 to + 15,
  // and values 16 to 31 map to values 0 to -15 respectively.  However, to find
  // the widest passing region, it is more convient to map the results such that
  // values 0 to 15 maps to settings -15 to 0 and values 16 to 31 maps to values
  // 1 to 16.
  if (delaySetting < 16) {
    resultsCodedDelay = delaySetting + 15;
  } else {
    resultsCodedDelay = delaySetting ^ 31;
  }
  //
  // Putting the results per nibble in a 128bit structure, so every 4 bits = 1 entry
  // and 32 / 4 = 8, thus divide by 8 to get the proper dword for this entry
  //
  dWord = (UINT8) (resultsCodedDelay / 8);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    // Continue to the next rank if this one is disabled
    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    for (strobe = 0; strobe < MSVx4; strobe++) {
      swizzleStrobe = CheckBacksideSwizzle (Host, socket, ch, dimm, strobe);
      lrdimmFeedBack = LrDimmReadTrainFeedback (Host, socket, ch, swizzleStrobe, 0xFF);
      if (lrdimmFeedBack == 0xF) {
        (*lrTrainRes)[ch][strobe].results[dWord] = (*lrTrainRes)[ch][strobe].results[dWord] | (0xF << ((resultsCodedDelay % 8) * 4));
      }
    } // strobe loop
  } // ch loop
} // GetLrdimmMrdMwdResults


/**

  Initialize CPGC for LRDIMM Recieve Training


  @param Host   - Pointer to sysHost
  @param socket - socket number
  @param chBitmask - mask of channels to be used in this CPGC test

**/
VOID
SetupLrdimmReadTest (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitmask,
  UINT8     NumCL,
  UINT8     loopCount
  )
{

    TCPGCAddress        CPGCAddress = {{0, 0, 0, 0},   // Start
    {0, 4, 0, 0x1F}, // Stop
    {0, 0, 0, 0},    // Order
    {0, 0, 0, 0},    // IncRate
    {0, 4, 0, 1}};   // IncValue
  // IncRate, Start, Stop, DQPat
    TWDBPattern         WDBPattern = {1,       0,   0, BasicVA}; // Should not matter what this is. We do not compare data.


  // CmdPat = PatRd, NumCL = input, LC = input, CPGCAddress, SOE = 0,
  // WDBPattern, EnCADB = 0, EnCKE = 0, SubSeqWait = 0
  SetupIOTest (Host, socket, chBitmask, PatRd, NumCL, loopCount, &CPGCAddress, 0, &WDBPattern, 0, 0, 0);

} //SetupLrdimmReadTest

/**

  Initialize CPGC for LRDIMM Write Training


  @param Host   - Pointer to sysHost
  @param socket - socket number
  @param chBitmask - mask of channels to be used in this CPGC test

**/
VOID
SetupLrdimmReadWriteTest (
  PSYSHOST  Host,
  UINT8     socket,
  UINT32    chBitmask,
  UINT8     NumCL,
  UINT8     loopCount
  )
{
  UINT8     subseqwait = 0;
  UINT8     ch;
  struct    channelNvram          (*channelNvList)[MAX_CH];

  //
  // Setup CPGC Pattern
  //
    TCPGCAddress CPGCAddress = {{0, 0, 0, 0},    // Start
    {0, 7, 0, 0x1F}, // Stop
    {0, 0, 0, 0},    // Order
    {0, 0, 0, 0},    // IncRate
    {0, 0, 0, 1}};   // IncValue
  //                       IncRate Start, Stop, DQPat
  TWDBPattern WDBPattern = {0,  0,  1,  BasicVA};
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if ((*channelNvList)[ch].ddrtEnabled) {
      subseqwait = 100;
      break;
    }
  }
  // CmdPat = PatRd, NumCL = input, LC = input, CPGCAddress, SOE = 0,
  // WDBPattern, EnCADB = 0, EnCKE = 0, SubSeqWait = 0/100
  SetupIOTest (Host, socket, chBitmask, PatWrRd, NumCL, loopCount, &CPGCAddress, NSOE, &WDBPattern, 0, 0, subseqwait);
} //SetupLrdimmReadWriteTest

/**

  Initialize LRDIMM MDQ Receive Enable Cycle Training Parameters

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC
VOID
LrdimmCycleTrainingInit (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    rank,
  UINT8    step
  )
{
  struct rankDevice  (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             ch;
  UINT8                             pattern[8] = {0, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0xFF};
  struct channelNvram               (*channelNvList)[MAX_CH];
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    rankStruct = GetRankStruct (Host, socket, ch, dimm);
    if (!((1 << ch) & chBitmask)) {
      continue;
    }
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }

    //
    //Set the rank within the BC08 register.
    //
    WriteLrbuf (Host, socket, ch, dimm, 0, rank, LRDIMM_F0, LRDIMM_BC08);

    WriteLrbuf (Host, socket, ch, dimm, 0, (0xF & ~(1 << rank)), LRDIMM_F0, LRDIMM_BC07);
    //
    // 5. Set BC0C to enable cycle training mode.
    //    Write to BC0C
    //
    WriteLrbuf (Host, socket, ch, dimm, 0, step, LRDIMM_F0, LRDIMM_BC0C);
    (*rankStruct)[rank].CurrentLrdimmTrainingMode = step;
    //
    // 2. Set the Buffer MPR pattern to a non-periodic time varying pattern
    //   (i.e. 0x2B, or 00101011)
    //
    SetBufferMprPattern (Host, socket, ch, dimm, pattern);
  } //ch
} // LrdimmCycleTrainingInit


/**

  Initialize LRDIMM MPR Buffer with provided pattern

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Ch      - channel number
  @param[in] Dimm    - dimm number
  @param[in] Pattern - pointer to an 8 byte array containing pattern values to be written.

  @retval N/A

**/
VOID
SetBufferMprPattern (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    Dimm,
  IN  UINT8    Pattern[MAX_BUFFER_MPR]
  )
{
  UINT8                             Index;

  for (Index = 0; Index < 4; Index++) {
    WriteLrbuf (Host, Socket, Ch, Dimm, 0, Pattern[Index], LRDIMM_F5, (LRDIMM_BC0x + (Index << 4)));
  }
  for (Index = 4; Index < 8; Index++) {
    WriteLrbuf (Host, Socket, Ch, Dimm, 0, Pattern[Index], LRDIMM_F6, (LRDIMM_BC0x + ((Index - 4) << 4)));
  }
}

/**

  Initialize LRDIMM MPR Buffer with provided pattern for all DIMMs in the socket

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket number
  @param[in] Pattern  - Pointer to an 8 byte array containing pattern values to be written.

  @retval N/A

**/
VOID
SetBufferMprPatternSocket (
  IN    PSYSHOST Host,
  IN    UINT8    Socket,
  IN    UINT8    Pattern[MAX_BUFFER_MPR]
  )
{
  UINT8               Ch;
  UINT8               Dimm;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if (((*DimmNvList)[Dimm].dimmPresent == 0) || (IsLrdimmPresent (Socket, Ch, Dimm) == 0)) {
        continue;
      }
      SetBufferMprPattern (Host, Socket, Ch, Dimm, Pattern);
    } // Dimm
  } // Ch
}


/**

  Restore LRDIMM parameters changed during training steps

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param dimm    - dimm number (0-based)
  @param rank    - rank number (0-based)

  @retval N/A

**/
VOID
LrdimmExitRankTraining (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    rank
  )
{
#ifndef DDR5_SUPPORT
  UINT8                             ch;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             controlWordData;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  SetTrainingMode (Host, socket, dimm, rank, CLEAR_MODE);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (!((1 << ch) & chBitmask)) {
      continue;
    }
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    rankStruct = GetRankStruct (Host, socket, ch, dimm);

    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }
    //
    //Send MRS commands to place DRAM's out of MPR mode
    //
    SetRankMPR (Host, socket, ch, dimm, rank, 0);
    if ((*dimmNvList)[dimm].DcpmmPresent == 1) {
      SetRankDAMprFnv (Host, socket, ch, dimm, 0);
      SetRankDMprFnv (Host, socket, ch, dimm, 0);
    }
    (*rankStruct)[rank].CurrentDramMode = 0;
    //
    //Clear training mode.
    //   Write 0 to BC0C
    //
    WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_NORMAL_MODE, LRDIMM_F0, LRDIMM_BC0C);
    (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_NORMAL_MODE;
    //
    //Disable: read preamble training mode, MPR override, and PBA/PDA mode in the buffer: F0BC1x = 00000000b
    //
    (*dimmNvList)[dimm].lrBuf_BC1x = (*dimmNvList)[dimm].lrBuf_BC1x & LRDIMM_ONE_RANK_TIMING_MODE;
    controlWordData = (*dimmNvList)[dimm].lrBuf_BC1x;
    WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, LRDIMM_F0, LRDIMM_BC1x);

  } //ch
#endif // !DDR5_SUPPORT
} // LrdimmExitRankTraining


/**

  Initialize LRDIMM MDQ Receive Enable Cycle Training Parameters

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC
VOID
LrdimmMrdInit (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    rank
  )
{
  UINT8                             ch;
  UINT8                             index;
  UINT8                             pattern[8] = {0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (!((1 << ch) & chBitmask)) {
      continue;
    }
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    rankStruct = GetRankStruct (Host, socket, ch, dimm);

    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }
    //
    //enable only this rank with BC07
    //
    WriteLrbuf (Host, socket, ch, dimm, 0, (0xF & ~(1 << rank)), LRDIMM_F0, LRDIMM_BC07);
    //
    // 6. Set BC0C to enable MRD training mode.
    //    Write 6 to BC0C
    //
    WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_MRD_TRAINING_MODE, LRDIMM_F0, LRDIMM_BC0C);
    (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_MRD_TRAINING_MODE;
    //
    //3. Set the comparison pattern in the Buffer registers (F5BC0x through
    //   F5BC3x and F6BC0x through F6BC3x) to the default (i.e. 10101010)
    //
    for (index = 0; index < 4; index++) {
      WriteLrbuf (Host, socket, ch, dimm, 0, pattern[index], LRDIMM_F5, (LRDIMM_BC0x + (index << 4)));
    }
    for (index = 4; index < 8; index++) {
      WriteLrbuf (Host, socket, ch, dimm, 0, pattern[index], LRDIMM_F6, (LRDIMM_BC0x + ((index - 4) << 4)));
    }
    //
    //2.  Set the DRAM in MPR mode, with expected pattern programmed in the DRAM's
    //    (i.e. the default 0101 pattern).
    SetRankMPRPattern (Host, socket, ch, dimm, rank, MR3_MPR, 0x55);
    (*rankStruct)[rank].CurrentDramMode = MR3_MPR;
    (*rankStruct)[rank].CurrentMpr0Pattern = 0x55;
  } //ch
} // LrdimmMrdInit


/**

  Initialize LRDIMM MDQ-MDQS Write Delay Training Parameters

  @param Host  - Pointer to sysHost
  @param socket  - Socket number

  @retval N/A

**/
STATIC
VOID
LrdimmMwdInit (
  PSYSHOST Host,
  UINT8    socket,
  UINT32   chBitmask,
  UINT8    dimm,
  UINT8    rank
  )
{
  UINT8                             ch;
  UINT8                             index;
  UINT8                             pattern[8] = {0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF};
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (!((1 << ch) & chBitmask)) {
      continue;
    }
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    rankStruct = GetRankStruct (Host, socket, ch, dimm);

    if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
      continue;
    }
    //
    //enable only this rank with BC07
    //
    WriteLrbuf (Host, socket, ch, dimm, 0, (0xF & ~(1 << rank)), LRDIMM_F0, LRDIMM_BC07);
    //
    // 2. Setup the Buffer Training Config CW to drive per-bit feedback on
    //    the DQ lanes.
    WriteLrbuf (Host, socket, ch, dimm, 0, 1, LRDIMM_F6, LRDIMM_BC4x);
    //
    // 5. Set BC0C to enable MWD training mode.
    //    Write 7 to BC0C
    //
    WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_MWD_TRAINING_MODE, LRDIMM_F0, LRDIMM_BC0C);
    (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_MWD_TRAINING_MODE;
    //
    //3. Set the comparison pattern in the Buffer registers (F5BC0x through
    //   F5BC3x and F6BC0x through F6BC3x) to the default (i.e. 10101010)
    //
    for (index = 0; index < 4; index++) {
      WriteLrbuf (Host, socket, ch, dimm, 0, pattern[index], LRDIMM_F5, (LRDIMM_BC0x + (index << 4)));
    }
    for (index = 4; index < 8; index++) {
      WriteLrbuf (Host, socket, ch, dimm, 0, pattern[index], LRDIMM_F6, (LRDIMM_BC0x + ((index - 4) << 4)));
    }
  } //ch
} // LrdimmMwdInit


/**

  Perform LRDIMM MDQ Receive Enable Phase Training (MREP)

  @param Host   - Pointer to sysHost
  @param socket - socket number

  @retval status

**/
UINT32
MDQReceiveEnablePhaseTraining (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT32                            chBitmask;
  UINT8                             logRank = 0;
  UINT8                             logSubRank = 0;
  UINT32                            status = SUCCESS;
  UINT8                             strobe;
  UINT8                             PairedStrobe;
  UINT8                             SwizzleStrobe;
  UINT8                             dWord;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  struct TrainingResults            (*lrTrainRes)[MAX_CH][MAX_STROBE];
  UINT8                             nibblePhaseSetting;
  UINT16                            risingEdge[MAX_CH][MAX_STROBE];
  UINT16                            centerPoint[MAX_CH][MAX_STROBE];
  UINT16                            fallingEdge[MAX_CH][MAX_STROBE];
  UINT16                            pulseWidth[MAX_CH][MAX_STROBE];
  UINT8                             *controlWordDataPtr;
  UINT8                             controlWordFunc;
  UINT8                             faultyPartsStatus;
  UINT8                             FinalOffset;
  ACT_TO_ACT_SAVE_STRUCT            ActToActSave[MAX_CH];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  lrTrainRes = &Host->var.mem.lrTrainRes;

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "DDR4 LRDIMM MDQ RecEn Phase Training\n");
  ReceiveEnableInitAllRanks (Host, socket); //lrdimm rcven doesn't have roundtrip latency set.

  for (ch = 0; ch < MaxChDdr; ch++) {
    ActToActSave[ch].TRrdSOrg = 0;
    ActToActSave[ch].TRrdLOrg = 0;
    ActToActSave[ch].TRcdRdOrg = 0;
    ActToActSave[ch].TRrsrOrg = 0;
    ActToActSave[ch].TRrSgOrg = 0;
    SetActToActPeriod (Host, socket, ch, ActToActSave);
  }
  SetAepTrainingMode (Host,socket, CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING);
  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Train each rank
    //
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING, (UINT16) ((socket << 8) | (dimm << 4) |rank)));
      //
      // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
      //
      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (chBitmask == 0) {
        continue;
      }

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);
        if ((*channelNvList)[ch].encodedCSMode) {
          logSubRank = (rank >> 1);
        } else {
          logSubRank = 0;
        }
        //
        // a) Adjust the CPGC test for the target rank
        //
        SelectCPGCRanks (Host, socket, ch, 1 << logRank, 0, logSubRank);
        //
        // Clear training results variable for this channel
        //
        for (strobe = 0; strobe < MSVx4; strobe++) {
          for (dWord = 0; dWord < 4; dWord++) {
            (*lrTrainRes)[ch][strobe].results[dWord] = 0;
          } // dWord loop
        } // strobe loop
      } // ch loop

      //
      // 1. Setup CPGC to execute "infinite" back to back sequence of MPR RD's.
      // This needs to be an "infinite sequence" since the phase is being compared.
      // CPGC enables this with a cacheline_num setting of 0 in the subsequence.
      // The CPGC test can be stopped after the DQ feedback has been read.
      // This is why we can't use this mode for cycle training to find the preamble.
      //
      // input parameters for this function are 8 for Number of cache lines and 0 for loop count
      //
      SetupLrdimmReadTest (Host, socket, chBitmask, MREP_CACHELINES, MREP_LOOPCOUNT);
      //
      //3. Setup HSX DDRIO in senseamp training mode, for sampling of DQ lanes.
      //   NOTE: using rank0 VOC for all LRDIMM training
      //   NOTE: will enable and disable sense amp training mode just before and after test to get MC to latch results
      //
      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, socket, ch);
        rankStruct = GetRankStruct (Host, socket, ch, dimm);
        SetMprModeFilterActPre (Host, socket, ch, !(*dimmNvList)[dimm].DcpmmPresent);
        //
        //enable only this rank with BC07
        //
        WriteLrbuf (Host, socket, ch, dimm, 0, (0xF & ~(1 << rank)), LRDIMM_F0, LRDIMM_BC07);
        //
        //4. Set BC0C to enable MREP training mode.
        //   Write 1 to BC0C
        //
        WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_MREP_TRAINING_MODE, LRDIMM_F0, LRDIMM_BC0C);
        (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_MREP_TRAINING_MODE;
        if ((*dimmNvList)[dimm].DcpmmPresent == 1) {
          SetRankMPRPatternDA (Host, socket, ch, dimm, rank, MR3_MPR, 0x55);
        } else {
          SetRankMPRPattern (Host, socket, ch, dimm, rank, MR3_MPR, 0x55);
        }
        (*rankStruct)[rank].CurrentDramMode = MR3_MPR;
        (*rankStruct)[rank].CurrentMpr0Pattern = 0x55;
      } // ch loop

      //
      //5.  For each back-side rank [x]:
      //
      // Find the DQ rising edge
      //
      //
      // b) For nibble phase settings of 0 to 64, do the following:
      //
      //RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
      //              "LRDIMM MDQ RecEn Phase Training - Pi Scanning:\n");

      for (nibblePhaseSetting = LRDIMM_BACKSIDE_PI_START; nibblePhaseSetting < LRDIMM_BACKSIDE_PI_RANGE; nibblePhaseSetting += LRDIMM_BACKSIDE_STEP_SIZE) {
        for (ch = 0; ch < MaxChDdr; ch++) {
          if (!((1 << ch) & chBitmask)) {
            continue;
          }

          //
          //i.  Set F[x]BC2x and F[x]BC3x to phase setting in all buffers
          //    x = backside rank 0:3
          WriteLrbuf (Host, socket, ch, dimm, 0, (nibblePhaseSetting >> 1), rank, LRDIMM_BC2x);
          WriteLrbuf (Host, socket, ch, dimm, 0, (nibblePhaseSetting >> 1), rank, LRDIMM_BC3x);
        } // ch loop

        //
        // For NVM dimms change infinite read to infinite GNT
        //
        for (ch = 0; ch < MaxChDdr; ch++) {
          if (!((1 << ch) & chBitmask)) {
            continue;
          }
          dimmNvList = GetDimmNvList (Host, socket, ch);
          if ((*dimmNvList)[dimm].DcpmmPresent) {
            CpgcAdvCmdParity (Host, socket, ch, GetPhyRank (dimm, rank), 0, GNT, NormalCadbSetup);
          }
        }
        //
        // read results from training results registers
        //
        GetLrdimmTrainResults (Host, socket, dimm, rank, nibblePhaseSetting, LRDIMM_MREP_TRAINING_MODE);
      } //nibblePhaseSetting loop
      LrdimmExitRankTraining (Host, socket, chBitmask, dimm, rank);

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, socket, ch);

        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          Host->var.mem.piSettingStopFlag[ch] = 0;
          continue;
        }

        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          Host->var.mem.piSettingStopFlag[ch] = 0;
          continue;
        }

        Host->var.mem.piSettingStopFlag[ch] = GetStrobeMask (Host);

      } // ch loop

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        rankList = GetRankNvList (Host, socket, ch, dimm);
        dimmNvList = GetDimmNvList (Host, socket, ch);

        rankStruct = GetRankStruct (Host, socket, ch, dimm);

#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();
          RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM MDQ RcvEn Phase Training\nS%d, Ch%d, DIMM%d, Rank%d\n", socket, ch, dimm, rank);
          PrintSampleArray (Host, socket, ch, dimm, PRINT_ARRAY_TYPE_LRDIMM_RCV_EN_PHASE);
          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK
        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          EvaluatePiSettingResults (Host, socket, ch, dimm, rank, strobe, risingEdge, centerPoint, fallingEdge, pulseWidth,
            FPT_PI_LRDIMM_READ_TYPE);

          if (Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] & (1 << strobe)) {
            //
            // Add this error to the warning log for both correctable and uncorrectable errors.
            //
            faultyPartsStatus = EvaluateFaultyParts (Host, socket, ch);

            switch (faultyPartsStatus) {
            case FPT_NO_ERROR:
              break;
            case FPT_CORRECTABLE_ERROR:
              // if correctable  log the warning -> OutputWarning
              // careful for cases when warning gets promoted to error !!!
              EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_RCVEN_PHASE_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, RecEnDelay, LrbufLevel, (UINT8) pulseWidth[ch][strobe]);
              break;
            default:
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                             "LRDIMM training failure!!!\n");
              //
              // Add this error to the warning log for both correctable and uncorrectable errors.
              //
              Host->var.mem.piSettingStopFlag[ch] |= (1 << strobe);
              DisableChannelSw (Host, socket, ch);
              EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_RCVEN_PHASE_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, RecEnDelay, LrbufLevel, (UINT8) pulseWidth[ch][strobe]);
              break;
            }
          }
        } // strobe loop
#ifdef DEBUG_CODE_BLOCK
        DisplayEdges (Host, socket, ch, pulseWidth[ch], fallingEdge[ch], risingEdge[ch], centerPoint[ch], MSVx4);
#endif // DEBUG_CODE_BLOCK

        //
        //c)  Set F[x]BC2x and F[x]BC3x to the respective settings that resulted in the
        //    first DQ feedback value transitioning from 0 to 1, first rising edge.
        //    An additional offset must be applied to center in the preamble.
        //    The offset should be 32/64 tCK for HSX (1 tCK preamble setting in the DRAM)
        //
        if (GetPreambleState (socket, READ_PREAMBLE) > PREAMBLE_1TCLK) {
          FinalOffset = 64;
        } else {
          FinalOffset = 32;
        }
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT, "Rx Skew Adjust\n");
        for (strobe = 0; strobe < MSVx4; strobe++) {
          PairedStrobe = (strobe < 9) ? (strobe + 9) : (strobe - 9);
          //
          // store the data to write for later use
          //
          //save off the "true" value in the cached variable

          //Adding a code offset of 5 to trained MREP setting per HSD 5372218
          if (((*dimmNvList)[dimm].SPDRegVen == MFGID_IDT) && ((*dimmNvList)[dimm].lrbufRid == LRBUF_IDT_DB_RID_GPB0)) {
            if ((*dimmNvList)[dimm].DcpmmPresent) {
              //
              // De-swizzle NVMDIMM backside strobe for retrive FPT information
              //
              SwizzleStrobe = CheckBacksideSwizzle (Host, socket, ch, dimm, strobe);
              if ((*rankList)[rank].faultyParts[SwizzleStrobe]) {
                //
                // Using setting of Paired strobe to replace faulty strobe
                //
                (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe] = (UINT8) (((risingEdge[ch][PairedStrobe]/2) + FinalOffset + 5) % 64);
              } else {
                (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe] = (UINT8) (((risingEdge[ch][strobe]/2) + FinalOffset + 5) % 64);
              }
            } else {
              if ((*rankList)[rank].faultyParts[strobe]) {
                //
                // Using setting of Paired strobe to replace faulty strobe
                //
                (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe] = (UINT8) (((risingEdge[ch][PairedStrobe]/2) + FinalOffset + 5) % 64);
              } else {
                (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe] = (UINT8) (((risingEdge[ch][strobe]/2) + FinalOffset + 5) % 64);
              }
            }
          } else {
            if ((*dimmNvList)[dimm].DcpmmPresent) {
              //
              // De-swizzle NVMDIMM backside strobe for retrive FPT information
              //
              SwizzleStrobe = CheckBacksideSwizzle (Host, socket, ch, dimm, strobe);
              if ((*rankList)[rank].faultyParts[SwizzleStrobe]) {
                //
                // Using setting of Paired strobe to replace faulty strobe
                //
                (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe] = (UINT8) (((risingEdge[ch][PairedStrobe]/2) + FinalOffset) % 64);
              } else {
                (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe] = (UINT8) (((risingEdge[ch][strobe]/2) + FinalOffset) % 64);
              }
            } else {
              if ((*rankList)[rank].faultyParts[strobe]) {
                //
                // Using setting of Paired strobe to replace faulty strobe
                //
                (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe] = (UINT8) (((risingEdge[ch][PairedStrobe]/2) + FinalOffset) % 64);
              } else {
                (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe] = (UINT8) (((risingEdge[ch][strobe]/2) + FinalOffset) % 64);
              }
            }
          }
          (*rankList)[rank].lrBuf_FxBC2x3x[strobe] = (*rankStruct)[rank].cachedLrBuf_FxBC2x3x[strobe];

        }//strobe
        controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBC2x3x[0];
        controlWordFunc = rank;
        WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BC2x, ALL_DATABUFFERS);
        controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBC2x3x[MSVx8];
        controlWordFunc = rank;
        WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BC3x, ALL_DATABUFFERS);

        SetMprModeFilterActPre (Host, socket, ch, 0);
      } // ch loop
      DetectLowMargin (Host, NO_CH, dimm, rank, pulseWidth, RecEnDelay, LM_LRDIMM);
    } //rank
  } //dimm

  for (ch = 0; ch < MaxChDdr; ch++) {
    RestoreActToActPeriod (socket, ch, ActToActSave);
  }

  SetAepTrainingMode (Host,socket, DISABLE_TRAINING_STEP);
  FifoTrainReset (Host, socket);
  return status;
} //MDQReceiveEnablePhaseTraining


/**

  Perform LRDIMM MDQ Receive Enable Cycle Training (MREC)

  The following option is a change relative to the Host-side training algorithm
  for RCVEN, in that there is no search for the preamble on the strobe. Instead,
  a functional read (MRD training mode) needs to be tested to determine when the
  cycle timing is correct. This means there is some dependency on the DQ-DQS
  timing to ensure that the data is correctly captured, even though the DQ-DQS
  timing has not yet been trained (similar scenario to TX WL Coarse).

  @param Host   - Pointer to sysHost
  @param socket - socket number

  @retval status

**/
UINT32
MDQReceiveEnableCycleTraining (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT32                            chBitmask;
  UINT8                             logRank = 0;
  UINT8                             logSubRank = 0;
  UINT8                             Strobe;
  UINT32                            status = SUCCESS;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  struct lrMrecTrainingResults      (*lrMrecTrainRes)[MAX_CH][MAX_STROBE];
  INT8                              nibbleCycleSetting;
  UINT8                             codedNibbleCycleSetting = 0;
#ifdef DEBUG_CODE_BLOCK
  UINT8                             maxStrobe;
  INT8                              cycleSetting;
#endif // DEBUG_CODE_BLOCK
  UINT8                             controlWordAddr;
  UINT8                             controlWordData;
  UINT8                             *controlWordDataPtr = NULL;
  INT8                              mdqsReadDelay = 0;
  UINT8                             faultyPartsStatus;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  lrMrecTrainRes = &Host->var.mem.lrMrecTrainRes;

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "DDR4 LRDIMM MDQ RecEn Cycle Training\n");
  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Train each rank
    //
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RX_BACKSIDE_CYCLE_TRAINING, (UINT16) ((socket << 8) | (dimm << 4) |rank)));

      faultyPartsStatus = FPT_NO_ERROR;
      //
      // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
      //
      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (chBitmask == 0) {
        continue;
      }

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        //M88DDR4 RCD/DB Sighting Report, sighting 1302101
        // montage M88DDR4DB01-A1 mrec workaround for DB A1
        dimmNvList = GetDimmNvList (Host, socket, ch);
        if ((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE &&
            (*dimmNvList)[dimm].lrbufRid == LRBUF_MONTAGE_RID_A1) {
          MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xb0, LRDIMM_F7, LRDIMM_BC5x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0x0e, LRDIMM_F7, LRDIMM_BC6x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);

          WriteLrbuf (Host, socket, ch, dimm, 0, 0xb1, LRDIMM_F7, LRDIMM_BC5x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0x08, LRDIMM_F7, LRDIMM_BC6x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xb0, LRDIMM_F7, LRDIMM_BC5x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0x2e, LRDIMM_F7, LRDIMM_BC6x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);

          WriteLrbuf (Host, socket, ch, dimm, 0, 0xb0, LRDIMM_F7, LRDIMM_BC5x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0x0e, LRDIMM_F7, LRDIMM_BC6x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xb1, LRDIMM_F7, LRDIMM_BC5x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0x09, LRDIMM_F7, LRDIMM_BC6x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);

          WriteLrbuf (Host, socket, ch, dimm, 0, 0xb0, LRDIMM_F7, LRDIMM_BC5x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0x2e, LRDIMM_F7, LRDIMM_BC6x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);

          WriteLrbuf (Host, socket, ch, dimm, 0, 0xb0, LRDIMM_F7, LRDIMM_BC5x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0x0e, LRDIMM_F7, LRDIMM_BC6x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
          MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);
        }

        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);
        if ((*channelNvList)[ch].encodedCSMode) {
          logSubRank = (rank >> 1);
        } else {
          logSubRank = 0;
        }
        //
        //6.  For each back-side rank [x]:
        //
        // a) Adjust the CPGC test for the target rank
        //
        SelectCPGCRanks (Host, socket, ch, 1 << logRank, 0, logSubRank);
        //
        // Clear training results variable for this channel
        //
        // As long as struct lrMrecTrainingResults includes only UINT8 the compiler will optimise the loop to memset
        ZeroMem ((*lrMrecTrainRes)[ch], MSVx4);
      } // ch loop

      //
      // Early LRDIMM MDQ Recieve Enable Cycle training step init
      //
      LrdimmCycleTrainingInit (Host, socket, chBitmask, dimm, rank, LRDIMM_MRD_TRAINING_MODE);
      //
      // 1. Setup CPGC sequence with a single cacheline RD. RCVEN in buffer will
      //    start, and the duration will be the preamble + BL/2.
      //
      //    parameters are 1 for Number of cache lines and 1 for loop count
      //
      SetupLrdimmReadTest (Host, socket, chBitmask, 1, 1);

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, socket, ch);
        rankStruct = GetRankStruct (Host, socket, ch, dimm);
        SetMprModeFilterActPre (Host, socket, ch, !(*dimmNvList)[dimm].DcpmmPresent);
        //
        // setup LRDIMM feedback as per bit (F6BC4X, perbit feedback)
        WriteLrbuf (Host, socket, ch, dimm, 0, 1, LRDIMM_F6, LRDIMM_BC4x);
        //
        //3.  Set the DRAM in MPR mode, and write the same pattern to the DRAM MPR registers
        //    (as is set in the buffer MPR register)
        //
        SetRankMPRPattern (Host, socket, ch, dimm, rank, MR3_MPR, 0x2B);
        SetRankMPRPatternDA (Host, socket, ch, dimm, rank, MR3_MPR, 0x2B);
        (*rankStruct)[rank].CurrentDramMode = MR3_MPR;
        (*rankStruct)[rank].CurrentMpr0Pattern = 0x2B;
        //}
      } // ch

      ToggleTxOnFnv (Host, socket, chBitmask, dimm, 1);
      //
      //6.  For each back-side rank [x]:
      //
      // b) For nibble cycle settings of -1 to 2, do the following:
      //
      //RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
      //              "LRDIMM MDQ RecEn Cycle Training \n");

      for (nibbleCycleSetting = -2; nibbleCycleSetting <= 2; nibbleCycleSetting += 1) {

        //Spec 0.92 compliant changes
        if (nibbleCycleSetting < 0) {
          codedNibbleCycleSetting = 4 + (UINT8) ABS (nibbleCycleSetting);
        } else {
          codedNibbleCycleSetting = (UINT8) nibbleCycleSetting;
        }
        controlWordData = (codedNibbleCycleSetting | (codedNibbleCycleSetting << 4));
        // F0BCCx for backside rank 0, F1BCCx for backside rank 1, F0BCEx for backside rank 2, and F1BCEx for backside rank 3
        controlWordAddr = LRDIMM_BCCx + ((rank >> 1) * 0x20);

        for (ch = 0; ch < MaxChDdr; ch++) {
          if (!((1 << ch) & chBitmask)) {
            continue;
          }

          dimmNvList = GetDimmNvList (Host, socket, ch);

          //Skip WriteLrbuf if montage, A1 and cycle setting = -2
          if ((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE && (*dimmNvList)[dimm].lrbufRid == LRBUF_MONTAGE_RID_A1 && nibbleCycleSetting == -2) {

            continue;
          }
          WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, (rank & BIT0) , controlWordAddr);
        } //ch
        SetupLrdimmReadTest (Host, socket, chBitmask, 1, 1);
        //
        //ii. For several settings of MDQS read delay (F[x]BC4x and F[x]BC5x)
        //    (i.e. default, +/- 4/64 tCK, +/- 8/64 tCK)
        //
        for (mdqsReadDelay = -8; mdqsReadDelay <= 8; mdqsReadDelay += 4) {
          if (mdqsReadDelay < 0) {
            controlWordData = (UINT8) (ABS (mdqsReadDelay) | BIT4);
          } else {
            controlWordData = (UINT8) mdqsReadDelay;
          }
          for (ch = 0; ch < MaxChDdr; ch++) {
            if (!((1 << ch) & chBitmask)) {
              continue;
            }

            WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, rank, LRDIMM_BC4x);
            WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, rank, LRDIMM_BC5x);

            WriteLrbuf (Host, socket, ch, dimm, 0, (0xF & ~(1 << rank)), LRDIMM_F0, LRDIMM_BC07);
            //
          }
          //ii. Check the DQ feedback from the Buffer comparator, if there's
          //    a pass in the nibble (for any MDQS read delay setting), that
          //    nibble has the correct cycle setting.
          //
          GetLrdimmMrecResults (Host, socket, dimm, rank, nibbleCycleSetting);
        } //read delay
      } // nibble cycle

      LrdimmExitRankTraining (Host, socket, chBitmask, dimm, rank);
      ToggleTxOnFnv (Host, socket, chBitmask, dimm, 0);
      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, socket, ch);
        rankList = GetRankNvList (Host, socket, ch, dimm);
        rankStruct = GetRankStruct (Host, socket, ch, dimm);
        //
        //c.  Set F[x]BC2x and F[x]BC3x to the respective settings that
        //    resulted in at least one passing test per nibble:
        //
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          //
          // Skip if this is an ECC Strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          //
          //Find best coded nibble cycle setting for this phase.
          //The encoded test results assume test run 0 is for -8, test run 1 is for -4, ...,test run 4 is for + 8
          //
          switch ((*lrMrecTrainRes)[ch][Strobe].results) {
          case BIT0:
            nibbleCycleSetting = -2;
            break;

          case BIT1:
            nibbleCycleSetting = -1;
            break;

          case BIT2:
            nibbleCycleSetting = 0;
            break;

          case BIT3:
            nibbleCycleSetting = 1;
            break;

          case BIT4:
            nibbleCycleSetting = 2;
            break;

          case 0:
          default:
#ifdef DDRT_SUPPORT
            if ((*dimmNvList)[dimm].DcpmmPresent) {
              //
              // De-swizzle NVMDIMM backside strobe for FPT tracking
              //
              UINT8 SwizzleStrobe = CheckBacksideSwizzle (Host, socket, ch, dimm, Strobe);
              //
              // Failure, mark this part as faulty
              //
              (*rankList)[rank].faultyParts[SwizzleStrobe] |= FPT_LRDIMM_TRAINING_FAILED;

              if ((*lrMrecTrainRes)[ch][Strobe].results) {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_SUBCH, dimm, rank, SwizzleStrobe, NO_BIT,
                  "PMem MDQS RecEn Cycle Training - FOUND MULTIPLE PASSING READ CYCLES - bit map = %d\n", (*lrMrecTrainRes)[ch][Strobe].results);
              } else {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_SUBCH, dimm, rank, SwizzleStrobe, NO_BIT,
                  "PMem MDQS RecEn Cycle Training - FAILED PASSING READ CYCLE\n");
              }
              nibbleCycleSetting = 1;
            }
#endif // DDRT_SUPPORT
            if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
              //
              // Failure, mark this part as faulty
              //
              (*rankList)[rank].faultyParts[Strobe] |= FPT_LRDIMM_TRAINING_FAILED;

              if ((*lrMrecTrainRes)[ch][Strobe].results) {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_SUBCH, dimm, rank, Strobe, NO_BIT,
                  "LRDIMM MDQS RecEn Cycle Training - FOUND MULTIPLE PASSING READ CYCLES - bit map = %d\n", (*lrMrecTrainRes)[ch][Strobe].results);
              } else {
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_SUBCH, dimm, rank, Strobe, NO_BIT,
                  "LRDIMM MDQS RecEn Cycle Training - FAILED PASSING READ CYCLE\n");
              }
              nibbleCycleSetting = -1; //DEFAULT
            }
            break;
          }

          //spec 0.92 compliant code here
          if (nibbleCycleSetting < 0) {
            codedNibbleCycleSetting = 4 + (UINT8) ABS (nibbleCycleSetting);
          } else {
            codedNibbleCycleSetting = (UINT8) nibbleCycleSetting;
          }
          (*rankList)[rank].lrBuf_FxBCCxEx[Strobe % MSVx8] &= (0xF0 >> (4 * (Strobe / MSVx8)));
          (*rankList)[rank].lrBuf_FxBCCxEx[Strobe % MSVx8] |= (codedNibbleCycleSetting << (4 * (Strobe / MSVx8)));
          (*rankStruct)[rank].cachedLrBuf_FxBCCxEx[Strobe % MSVx8] = (*rankList)[rank].lrBuf_FxBCCxEx[Strobe % MSVx8];
        } // Strobe

        // 0.92 spec compliant code
        controlWordDataPtr = &((*rankList)[rank].lrBuf_FxBCCxEx[0]);
        controlWordAddr = LRDIMM_BCCx + ((rank >> 1) * 0x20);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, (rank & BIT0) , controlWordAddr, ALL_DATABUFFERS);
      } // ch loop

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);
        dimmNvList = GetDimmNvList (Host, socket, ch);

        //M88DDR4 RCD/DB Sighting Report, sighting 1302101
        // montage M88DDR4DB01-A1 mrec workaround for DB A1
        if ((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE &&
            (*dimmNvList)[dimm].lrbufRid == LRBUF_MONTAGE_RID_A1) {
          MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0x0f, LRDIMM_F7, LRDIMM_BC5x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0x0c, LRDIMM_F7, LRDIMM_BC6x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
          MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);

          FixedDelayMicroSecond (10);

          MRSCmdStacking (Host, socket, ch, dimm, LOAD_MRS);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0x0f, LRDIMM_F7, LRDIMM_BC5x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0x04, LRDIMM_F7, LRDIMM_BC6x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcd, LRDIMM_F7, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, 0xcc, LRDIMM_F7, LRDIMM_BC4x);
          MRSCmdStacking (Host, socket, ch, dimm, EXECUTE_MRS);
        }

#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();
          RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM RecEn Cycle\nS%d, Ch%d, DIMM%d, Rank%d\n", socket, ch, dimm, rank);
          PrintMrecResults (Host, socket, ch, dimm, 0);
          ReleasePrintControl ();
        }
        // Print rising edges
        if ((GetDebugLevel () & SDBG_MAX)) {
          if (!(*dimmNvList)[dimm].x4Present) {
            maxStrobe = MSVx8;
          } else {
            maxStrobe = MSVx4;
          }

          AcquirePrintControl ();

          RcDebugPrintLine (SDBG_MAX, 76, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

          RcDebugPrint (SDBG_DEFAULT, "RE: ");
          for (Strobe = 0; Strobe < maxStrobe; Strobe++) {
            //if (IsStrobeNotValid(Host, Strobe)) continue;
            // Just print out the final cycle settings
            if (IsStrobeNotValid (Host, Strobe)) {
              continue;
            }
            if ((*lrMrecTrainRes)[ch][Strobe].results & BIT0) {
              cycleSetting = -2;
            } else if ((*lrMrecTrainRes)[ch][Strobe].results & BIT1) {
              cycleSetting = -1;
            } else if ((*lrMrecTrainRes)[ch][Strobe].results & BIT2) {
              cycleSetting = 0;
            } else if ((*lrMrecTrainRes)[ch][Strobe].results & BIT3) {
              cycleSetting = 1;
            } else if ((*lrMrecTrainRes)[ch][Strobe].results & BIT4) {
              cycleSetting = 2;
            } else {
              //
              // Do not log the warning if this is an ECC Strobe when ECC is disabled
              //
              if ((IsStrobeNotValid (Host, Strobe)) == 0) {
                EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_RD_RCVEN, socket, ch, dimm, rank, EwlSeverityWarning, Strobe, (*lrMrecTrainRes)[ch][Strobe].results, RecEnDelayCycle,LrbufLevel, 0xFF);
                cycleSetting = -1;
              } else {
                cycleSetting = 0;
              }
            }
            RcDebugPrint (SDBG_DEFAULT, " %3d", cycleSetting);
          } // Strobe loop
          RcDebugPrint (SDBG_DEFAULT, "\n");

          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK

        faultyPartsStatus = EvaluateFaultyParts (Host, socket, ch);
        if (faultyPartsStatus == FPT_CORRECTABLE_ERROR) {
          EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_RCVEN_CYCLE_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, NO_STROBE, NO_BIT, RecEnDelayCycle, LrbufLevel, 0xFF);
        } else if (faultyPartsStatus == FPT_NO_CORRECTABLE_ERROR) {
          EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_RCVEN_CYCLE_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, NO_STROBE, NO_BIT, RecEnDelayCycle, LrbufLevel, 0xFF);
          // Disable the channel with faulty ranks
          DisableChannelSw (Host, socket, ch);
        }
        SetMprModeFilterActPre (Host, socket, ch, 0);
      } // ch loop
    } //rank

    status = KEEP_FAILURE (RestoreDimmRankPresence (Host, socket, dimm), status);

  } //dimm
  FifoTrainReset (Host, socket);
  return status;
} //MDQReceiveEnableCycleTraining


/**

  Perform LRDIMM MDQ Read Delay Training (MRD)

  This training step leverages the comparison capability of the buffer
  MRD Training Mode to measure the margins and determine the best
  Rx strobe (MDQS) delay timing relative to the MDQ signals.

  @param Host   - Pointer to sysHost
  @param socket - socket number

  @retval status

**/
UINT32
MDQReadDelayTraining (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             dWord;
  UINT32                            chBitmask;
  UINT8                             logRank = 0;
  UINT8                             logSubRank = 0;
  UINT32                            status = SUCCESS;
  UINT8                             strobe;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  struct TrainingResults            (*lrMrdTrainRes)[MAX_CH][MAX_STROBE];
  UINT8                             DumArr[7] = {1,1,1,1,1,1,1};
  UINT8                             controlWordData;
  INT8                              mdqsReadDelay = 0;
  UINT16                            risingEdge[MAX_CH][MAX_STROBE];
  UINT16                            centerPoint[MAX_CH][MAX_STROBE];
  UINT16                            fallingEdge[MAX_CH][MAX_STROBE];
  UINT16                            pulseWidth[MAX_CH][MAX_STROBE];
  UINT8                             faultyPartsStatus;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  lrMrdTrainRes = &Host->var.mem.lrMrdTrainRes;

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "DDR4 LRDIMM MDQ Read Delay Training\n");
  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Train each rank
    //
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      //
      // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
      //
      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      if (chBitmask == 0) {
        continue;
      }

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, socket, ch);

        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);
        if ((*channelNvList)[ch].encodedCSMode) {
          logSubRank = (rank >> 1);
        } else {
          logSubRank = 0;
        }

        SetMprModeFilterActPre (Host, socket, ch, !(*dimmNvList)[dimm].DcpmmPresent);
        //
        // a. Adjust the CPGC test for the target rank
        //
        SelectCPGCRanks (Host, socket, ch, 1 << logRank, 0, logSubRank);
        //
        // Clear training results variable for this channel
        //
        for (strobe = 0; strobe < MSVx4; strobe++) {
          for (dWord = 0; dWord < 4; dWord++) {
            (*lrMrdTrainRes)[ch][strobe].results[dWord] = 0;
          }
        } // strobe loop
      } // ch loop

      //
      // Early LRDIMM MDQ Read Delay training step init
      //
      LrdimmMrdInit (Host, socket, chBitmask, dimm, rank);

      //
      // 1. Setup CPGC sequence for a single cacheline MPR read.
      //
      SetupLrdimmReadTest (Host, socket, chBitmask, 1, 1);

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        rankStruct = GetRankStruct (Host, socket, ch, dimm);
        //
        //4. Setup the Buffer Training Config CW to drive per-bit feedback on
        //   the DQ lanes.
        WriteLrbuf (Host, socket, ch, dimm, 0, 1, LRDIMM_F6, LRDIMM_BC4x);
        //
        // 6. Set BC0C to enable MRD training mode.
        //    Write 6 to BC0C
        //
        WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_MRD_TRAINING_MODE, LRDIMM_F0, LRDIMM_BC0C);
        (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_MRD_TRAINING_MODE;
      } // ch

      // Force the buffer outputs to rail values when using a 2-tclk read preamble, avoiding floating voltage levels
      // that appear to be edges, and in turn preventing false results.
      if (GetPreambleState (socket, READ_PREAMBLE) == PREAMBLE_2TCLK) {
        ToggleTxOnFnv (Host, socket, chBitmask, dimm, 1);
      }

      //
      //6.  For each back-side rank [x]:
      //
      //RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
      //              "LRDIMM MDQ Read Delay Training \n");
      //
      // b. Sweep F[x]BC4x and F[x]BC5x from -15/64 to + 15/64 in 1/64
      //    increments), for each setting do the following:
      //
      for (mdqsReadDelay = LRDIMM_BACKSIDE_READ_DELAY_START; mdqsReadDelay <= LRDIMM_BACKSIDE_READ_DELAY_END; mdqsReadDelay += LRDIMM_BACKSIDE_READ_DELAY_STEP_SIZE) {
        if (mdqsReadDelay < 0) {
          controlWordData = (UINT8) (ABS (mdqsReadDelay) | BIT4);
        } else {
          controlWordData = (UINT8) mdqsReadDelay;
        }
        for (ch = 0; ch < MaxChDdr; ch++) {
          if (!((1 << ch) & chBitmask)) {
            continue;
          }

          WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, rank, LRDIMM_BC4x);
          WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, rank, LRDIMM_BC5x);
        } //ch
        //
        //i.  Execute the CPGC MPR RD sequence
        //
        RunIOTest (Host, socket, chBitmask, BasicVA, DumArr, 1, 0);
        //
        // 5. Setup HSX DDRIO in senseamp training mode, for sampling of DQ lanes.
        //
        SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_SENSE_AMP);

        //
        // Delay only if this is a CTE build, otherwise do nothing.
        //

        CteDelayQclk (40);

        //
        //ii. After the CPGC test completes, read the results of the DQ sampling
        //    in the DDRIO, and extract the per nibble composite eye (AND the results).
        //
        GetLrdimmMrdMwdResults (Host, socket, dimm, rank, controlWordData, LRDIMM_MRD_TRAINING_MODE);

        SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);

      } // end read delay sweep
      //
      //c.  Set F[x]BC4x and F[x]BC5x to the respective settings that are in the center of the passing region.
      //
      LrdimmExitRankTraining (Host, socket, chBitmask, dimm, rank);

      // Stop forcing the buffer outputs to rail values when using a 2-tclk read preamble.
      if (GetPreambleState (socket, READ_PREAMBLE) == PREAMBLE_2TCLK) {
        ToggleTxOnFnv (Host, socket, chBitmask, dimm, 0);
      }

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          Host->var.mem.piSettingStopFlag[ch] = 0;
          continue;
        }

        Host->var.mem.piSettingStopFlag[ch] = GetStrobeMask (Host);
      } // ch loop

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        //
        //c.  Set F[x]BC4x and F[x]BC5x to the respective settings that are in the center of the passing region.
        //
#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();
          RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM Rd MDQS Delay\nS%d, Ch%d, DIMM%d, Rank%d\n", socket, ch, dimm, rank);
          PrintSampleArray (Host, socket, ch, dimm, PRINT_ARRAY_TYPE_LRDIMM_MDQ_RD_DELAY);
          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK
        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          //TODO: qualify duty cycle pass / fail
          EvaluatePiSettingResults (Host, socket, ch, dimm, rank, strobe, risingEdge, centerPoint, fallingEdge, pulseWidth,
            FPT_PI_LRDIMM_RD_MRD_TYPE);
          if (Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] & (1 << strobe)) {
            //
            // Add this error to the warning log for both correctable and uncorrectable errors.
            //
            faultyPartsStatus = EvaluateFaultyParts (Host, socket, ch);

            switch (faultyPartsStatus) {
            case FPT_NO_ERROR:
              break;
            case FPT_CORRECTABLE_ERROR:
              // if correctable  log the warning -> OutputWarning
              // careful for cases when warning gets promoted to error !!!
              EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_READ_DELAY_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, MrcGtDelim, LrbufLevel, (UINT8) pulseWidth[ch][strobe]);
              break;
            default:
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                             "LRDIMM training failure!!!\n");
              //
              // Add this error to the warning log for both correctable and uncorrectable errors.
              //
              Host->var.mem.piSettingStopFlag[ch] |= (1 << strobe);
              DisableChannelSw (Host, socket, ch);
              EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_READ_DELAY_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, MrcGtDelim, LrbufLevel, (UINT8) pulseWidth[ch][strobe]);
              break;
            }
          }
        } // strobe loop
#ifdef DEBUG_CODE_BLOCK
        DisplayLrdimmMrdEdges (Host, socket, ch, pulseWidth[ch], fallingEdge[ch], risingEdge[ch], centerPoint[ch], MSVx4);
#endif // DEBUG_CODE_BLOCK
        status = KEEP_FAILURE (SetLrdimmMrdResults (Host, socket, ch, dimm, rank, centerPoint[ch]), status);

        SetMprModeFilterActPre (Host, socket, ch, 0);
      } // ch loop
      DetectLowMargin (Host, NO_CH, dimm, rank, pulseWidth, RxDqsDelay, LM_LRDIMM);
    } //rank

    status = KEEP_FAILURE (RestoreDimmRankPresence (Host, socket, dimm), status);

  } //dimm
  FifoTrainReset (Host, socket);
  return status;
} //MDQReadDelayTraining


/**

  Perform LRDIMM DRAM Interface WL (DWL):

  This training step leverages the buffer DWL Training Mode to determine the
  best Tx strobe (MDQS) strobe alignment to the DRAM clock.

  @param Host   - Pointer to sysHost
  @param socket - socket number

  @retval status

**/
UINT32
DramInterfaceWlTraining (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT32                            chBitmask;
  UINT32                            status = SUCCESS;
  UINT8                             strobe;
  UINT8                             PairedStrobe;
  UINT8                             dWord;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  struct TrainingResults            (*lrDwlTrainRes)[MAX_CH][MAX_STROBE];
  UINT16                            risingEdge[MAX_CH][MAX_STROBE];
  UINT16                            centerPoint[MAX_CH][MAX_STROBE];
  UINT16                            fallingEdge[MAX_CH][MAX_STROBE];
  UINT16                            pulseWidth[MAX_CH][MAX_STROBE];
  UINT8                             *controlWordDataPtr;
  UINT8                             controlWordFunc;
  UINT8                             pi;
  UINT8                             faultyPartsStatus;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  lrDwlTrainRes = &Host->var.mem.lrDwlTrainRes;

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "DDR4 LRDIMM DRAM WL Training\n");
  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Train each rank
    //
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_TX_BACKSIDE_FINE_WL_TRAINING, (UINT16) ((socket << 8) | (dimm << 4) |rank)));
      //
      // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
      //
      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (chBitmask == 0) {
        continue;
      }

      for (ch = 0; ch < MaxChDdr; ch++) {

        //
        // Clear training results variable for this channel
        //
        for (strobe = 0; strobe < MSVx4; strobe++) {
          for (dWord = 0; dWord < 4; dWord++) {
            (*lrDwlTrainRes)[ch][strobe].results[dWord] = 0;
          } // dWord loop
        } // strobe loop
      } // ch loop
      //

      SetupLrdimmReadTest (Host, socket, chBitmask, 1, 1);

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        rankStruct = GetRankStruct (Host, socket, ch, dimm);

        //
        // setup LRDIMM feedback as per bit (F6BC4X, perbit feedback)
        //
        WriteLrbuf (Host, socket, ch, dimm, 0, 1, LRDIMM_F6, LRDIMM_BC4x);
        //
        //2. Set the buffer into DWL training mode in the BC0C register.
        //   Once this is done, the buffer will send continuous stream of strobes
        //   to the DRAM, with the timing associated with the selected rank's WL
        //   control words. The DRAM will send DQ feedback indicating the CK sample
        //   in the DRAM. This DQ feedback will be propogated to the Host.
        //
        //   Write 4 to BC0C
        //
        WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_DWL_TRAINING_MODE, LRDIMM_F0, LRDIMM_BC0C);
        (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_DWL_TRAINING_MODE;
      } //ch
      //
      //3.  For each back-side rank [x]:
      //
      // a. Set the target DRAMs in WL mode, with adjustments to RTT_PARK
      //    (i.e. copy RTT_WR to RTT_PARK, need to make sure Host is overriding
      //    the ODT signals in the same way as for Host interface WL) as needed.
      //    Leave all other rank DRAM's in normal mode.
      //
      OverrideRttNom (Host, socket, dimm, rank);
      ToggleInternalClocksOnFnv (Host, socket, chBitmask, dimm, 1);
      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        //
        //b. Set the rank within the BC08 and BC07 registers
        //
        WriteLrbuf (Host, socket, ch, dimm, 0, rank, LRDIMM_F0, LRDIMM_BC08);
        WriteLrbuf (Host, socket, ch, dimm, 0, (0xF & ~(1 << rank)), LRDIMM_F0, LRDIMM_BC07);
      } // ch

      //RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
      //              "LRDIMM DRAM Interface WL - Pi Scanning:\n");
      //
      //c. Sweep F[x]BCAx and F[x]BCBx Phase Control in 1/64 increments),
      //   for each setting do the following:
      //
      for (pi = LRDIMM_BACKSIDE_PI_START; pi < LRDIMM_BACKSIDE_PI_RANGE; pi += LRDIMM_BACKSIDE_STEP_SIZE) {
        for (ch = 0; ch < MaxChDdr; ch++) {
          if (!((1 << ch) & chBitmask)) {
            continue;
          }
          // set all strobes to the pi setting
          WriteLrbuf (Host, socket, ch, dimm, 0, pi >> 1, rank, LRDIMM_BCAx);
          WriteLrbuf (Host, socket, ch, dimm, 0, pi >> 1, rank, LRDIMM_BCBx);

          //
          // Inphi A1 WA
          //
          dimmNvList = GetDimmNvList (Host, socket, ch);
          if (((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) &&
              ((*dimmNvList)[dimm].lrbufRid == LRBUF_INPHI_RID_A1)) {
            InphiPIWA (Host, socket, ch, dimm);
          }

          WriteLrbuf (Host, socket, ch, dimm, 0, (0xF & ~(1 << rank)), LRDIMM_F0, LRDIMM_BC07);
        } //ch
        //
        //a. Capture the DQ feedback values in the Host
        //
        GetLrdimmTrainResults (Host, socket, dimm, rank, pi, LRDIMM_DWL_TRAINING_MODE);
      }
      //
      // fix ODT
      //
      ToggleInternalClocksOnFnv (Host, socket, chBitmask, dimm, 0);
      RestoreRttNom (Host, socket, dimm, rank);
      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        rankStruct = GetRankStruct (Host, socket, ch, dimm);
        //
        //set DRAMS back to normal mode
        //   Write 0 to BC0C
        //
        WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_NORMAL_MODE, LRDIMM_F0, LRDIMM_BC0C);
        (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_NORMAL_MODE;
      } // ch loop

      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          Host->var.mem.piSettingStopFlag[ch] = 0;
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);

        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          Host->var.mem.piSettingStopFlag[ch] = 0;
          continue;
        }

        Host->var.mem.piSettingStopFlag[ch] = GetStrobeMask (Host);

      } // ch loop

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);
        dimmNvList = GetDimmNvList (Host, socket, ch);

        // Continue to the next rank if this one is disabled
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
          continue;
        }
        rankStruct = GetRankStruct (Host, socket, ch, dimm);

#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();
          RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM DRAM WL Phase\nS%d, Ch%d, DIMM%d, Rank%d\n", socket, ch, dimm, rank);
          PrintSampleArray (Host, socket, ch, dimm, PRINT_ARRAY_TYPE_LRDIMM_WL_PHASE);
          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK
        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }
          //
          //d. Set F[x]BCAx and F[x]BCBx Phase Control to the respective
          //   settings that are aligned to the rising edge of the CK.
          //
          EvaluatePiSettingResults (Host, socket, ch, dimm, rank, strobe, risingEdge, centerPoint, fallingEdge, pulseWidth,
            FPT_PI_LRDIMM_WRITE_TYPE);

          if (Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] & (1 << strobe)) {
            //
            // Add this error to the warning log for both correctable and uncorrectable errors.
            //
            faultyPartsStatus = EvaluateFaultyParts (Host, socket, ch);

            switch (faultyPartsStatus) {
            case FPT_NO_ERROR:
              break;
            case FPT_CORRECTABLE_ERROR:
              // if correctable  log the warning -> OutputWarning
              // careful for cases when warning gets promoted to error !!!
              EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_WL_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, MrcGtDelim, LrbufLevel, (UINT8) pulseWidth[ch][strobe]);
              break;
            default:
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                             "LRDIMM training failure!!!\n");
              //
              // Add this error to the warning log for both correctable and uncorrectable errors.
              //
              Host->var.mem.piSettingStopFlag[ch] |= (1 << strobe);
              DisableChannelSw (Host, socket, ch);
              EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_WL_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, MrcGtDelim, LrbufLevel, (UINT8) pulseWidth[ch][strobe]);
              break;
            }
          }
        } // strobe loop
#ifdef DEBUG_CODE_BLOCK
        DisplayEdges (Host, socket, ch, pulseWidth[ch], fallingEdge[ch], risingEdge[ch], centerPoint[ch], MSVx4);
#endif // DEBUG_CODE_BLOCK

        for (strobe = 0; strobe < MSVx4; strobe++) {
          PairedStrobe = (strobe < 9) ? (strobe + 9) : (strobe - 9);
#ifdef DDRT_SUPPORT
          if ((*dimmNvList)[dimm].DcpmmPresent) {
            //
            // De-swizzle NVMDIMM backside strobe for retrive FPT information
            //
            UINT8 SwizzleStrobe = CheckBacksideSwizzle (Host, socket, ch, dimm, strobe);
            if ((*rankList)[rank].faultyParts[SwizzleStrobe]) {
              //
              // Using setting of paired strobe to replace faulty strobe
              //
              (*rankList)[rank].lrBuf_FxBCAxBx[strobe] = (((risingEdge[ch][PairedStrobe]) % 128) >> 1);
            } else {
              //
              // store the data to write for later use
              //
              (*rankList)[rank].lrBuf_FxBCAxBx[strobe] = (((risingEdge[ch][strobe]) % 128) >> 1);
            }
          }
#endif // DDRT_SUPPORT
          if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
            if ((*rankList)[rank].faultyParts[strobe]) {
              //
              // Using setting of paired strobe to replace faulty strobe
              //
              (*rankList)[rank].lrBuf_FxBCAxBx[strobe] = (((risingEdge[ch][PairedStrobe]) % 128) >> 1);
            } else {
              //
              // store the data to write for later use
              //
              (*rankList)[rank].lrBuf_FxBCAxBx[strobe] = (((risingEdge[ch][strobe]) % 128) >> 1);
            }
          }
          (*rankStruct)[rank].cachedLrBuf_FxBCAxBx[strobe] = (*rankList)[rank].lrBuf_FxBCAxBx[strobe];
        } // strobe
        //
        //d. Set F[x]BCAx and F[x]BCBx Phase Control to the respective settings
        //   that are aligned to the rising edge of the CK.
        //
        controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBCAxBx[0];
        controlWordFunc = rank;
        WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BCAx, ALL_DATABUFFERS);
        controlWordDataPtr = &(*rankList)[rank].lrBuf_FxBCAxBx[MSVx8];
        controlWordFunc = rank;
        WriteLrbufPBA (Host, socket, ch, dimm, rank, controlWordDataPtr, controlWordFunc, LRDIMM_BCBx, ALL_DATABUFFERS);
        //
        // Inphi A1 WA
        //
        if (((*dimmNvList)[dimm].SPDRegVen == MFGID_INPHI) &&
            ((*dimmNvList)[dimm].lrbufRid == LRBUF_INPHI_RID_A1)) {
          InphiPIWA (Host, socket, ch, dimm);
        }
        WriteLrbuf (Host, socket, ch, dimm, 0, (0xF & ~(1 << rank)), LRDIMM_F0, LRDIMM_BC07);
      } // ch loop
      DetectLowMargin (Host, NO_CH, dimm, rank, pulseWidth, TxDqsDelay, LM_LRDIMM);
    } //rank

    status = KEEP_FAILURE (RestoreDimmRankPresence (Host, socket, dimm), status);

  } //dimm
  FifoTrainReset (Host, socket);
  return status;
} //DramInterfaceWlTraining


/**

  Perform LRDIMM DRAM Interface WL (DWL):

  This training step leverages the comparison capability of the buffer MWD
  Training Mode to determine the best Tx strobe (MDQS) cycle alignment for
  the associated CWL setting and DIMM routing.

  @param Host   - Pointer to sysHost
  @param socket - socket number

  @retval status

**/
UINT32
MDQCoarseWlTraining (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT32                            chBitmask;
  UINT8                             logRank = 0;
  UINT8                             logSubRank = 0;
  UINT32                            status = SUCCESS;
  UINT8                             Strobe;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  struct lrMrecTrainingResults      (*lrCwlTrainRes)[MAX_CH][MAX_STROBE];
#ifdef DEBUG_CODE_BLOCK
  UINT8                             maxStrobe;
#endif // DEBUG_CODE_BLOCK
  UINT8                             controlWordData;
  UINT8                             controlWordAddr;
  UINT8                             *controlWordDataPtr;
  INT8                              cycleDelay;
  UINT8                             codedDelayCycleSetting = 0;
  UINT8                             DumArr[7] = {1,1,1,1,1,1,1};
  UINT8                             pattern[12] = {0, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0xFF, 0, 0xFF, 0xFF,0};
  UINT8                             faultyPartsStatus;
  INT8                              MdqsWriteDelay = 0;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  lrCwlTrainRes = &Host->var.mem.lrCwlTrainRes;

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "DDR4 LRDIMM DRAM Coarse WL Training\n");
  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Train each rank
    //
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_TX_BACKSIDE_COARSE_WL_TRAINING, (UINT16) ((socket << 8) | (dimm << 4) |rank)));

      faultyPartsStatus = FPT_NO_ERROR;
      //
      // Create a bit mask of channels that have at least 1 rank present for this dimm and rank
      //
      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (chBitmask == 0) {
        continue;
      }

      ZeroMem ((UINT8 *) lrCwlTrainRes, sizeof (*lrCwlTrainRes));
      //
      //4. Setup CPGC to execute single cacheline write/read loopback test
      //
      SetupLrdimmReadWriteTest (Host, socket, chBitmask, 1, 1);

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }
        //
        //2. Program the Buffer Training Configuration Control Word
        //   for per-bit feedback (F6BC4x).
        //
        WriteLrbuf (Host, socket, ch, dimm, 0, 1, LRDIMM_F6, LRDIMM_BC4x);
        //
        //Set the rank within the BC08 register.
        //
        WriteLrbuf (Host, socket, ch, dimm, 0, rank, LRDIMM_F0, LRDIMM_BC08);
        WriteLrbuf (Host, socket, ch, dimm, 0, (0xF & ~(1 << rank)), LRDIMM_F0, LRDIMM_BC07);
        //
        //7. For each back-side rank [x]:
        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);
        if ((*channelNvList)[ch].encodedCSMode) {
          logSubRank = (rank >> 1);
        } else {
          logSubRank = 0;
        }
        //
        // a. Adjust the CPGC test for the target rank.
        //
        SelectCPGCRanks (Host, socket, ch, 1 << logRank, 0, logSubRank);
      } // ch
      //RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
      //              "LRDIMM MDQS Coarse WL - Cycle Scanning:\n");
      //
      // b. Sweep F[x]BCAx and F[x]BCBx cycle delays from 2 down to -1.
      //    For each setting do the following:
      //
      for (cycleDelay = 2; cycleDelay >= -2; cycleDelay--) {
        //Spec 0.92 compliant code here
        if (cycleDelay < 0) {
          codedDelayCycleSetting = 4 + (UINT8) ABS (cycleDelay);
        } else {
          codedDelayCycleSetting = (UINT8) cycleDelay;
        }
        controlWordData = (codedDelayCycleSetting | (codedDelayCycleSetting << 4));

        for (ch = 0; ch < MaxChDdr; ch++) {
          if (!((1 << ch) & chBitmask)) {
            continue;
          }

          dimmNvList = GetDimmNvList (Host, socket, ch);
          rankList = GetRankNvList (Host, socket, ch, dimm);
          rankStruct = GetRankStruct (Host, socket, ch, dimm);

          //Skip WriteLrbuf if montage, A1 and cycle setting = -2
          if ((*dimmNvList)[dimm].SPDRegVen == MFGID_MONTAGE && (*dimmNvList)[dimm].lrbufRid == LRBUF_MONTAGE_RID_A1 && cycleDelay == -2) {

            continue;
          }
          // F0BCDx for backside rank 0, F1BDCx for backside rank 1, F0BCFx for backside rank 2, and F1BCFx for backside rank 3
          controlWordAddr = LRDIMM_BCDx + ((rank >> 1) * 0x20);
          WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, (rank & BIT0) , controlWordAddr);
          //
          // change the buffer pattern for each cycle
          //
          SetBufferMprPattern (Host, socket, ch, dimm, &pattern[cycleDelay + 2]);
          //
          //5. Set the buffer into MWD training mode in the BC0C register.
          //   Write 7 to BC0C
          //
          WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_MWD_TRAINING_MODE, LRDIMM_F0, LRDIMM_BC0C);
          (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_MWD_TRAINING_MODE;
        } //ch

        // Conduct MdqsWriteDelay sweep for better alignment at higher frequencies.
        // DDRT FIFO has stale data, thus reset FIFO for each sweep.
        for (MdqsWriteDelay = -8; MdqsWriteDelay <= 8; MdqsWriteDelay++) {
          IODdrtReset (Host, socket, dimm);
          if (MdqsWriteDelay < 0) {
            controlWordData = (UINT8) (ABS (MdqsWriteDelay) | BIT4);
          } else {
            controlWordData = (UINT8) MdqsWriteDelay;
          }
          for (ch = 0; ch < MaxChDdr; ch++) {
            if (!((1 << ch) & chBitmask)) {
              continue;
            }

            WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, rank, LRDIMM_BC8x);
            WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, rank, LRDIMM_BC9x);

            WriteLrbuf (Host, socket, ch, dimm, 0, (0xF & ~(1 << rank)), LRDIMM_F0, LRDIMM_BC07);
          }
          //
          // Per nibble, determine the passing cycle setting
          // (and store away in the results structure for later).
          //
          RunIOTest (Host, socket, chBitmask, BasicVA, DumArr, 1, 0);
          // Check the DQ feedback from the Buffer comparator, if there's
          // a pass in the nibble (for any MDQS read delay setting), that
          // nibble has the correct cycle setting.
          //
          GetLrdimmCwlResults (Host, socket, dimm, rank, cycleDelay);
        } //Write delay

        for (ch = 0; ch < MaxChDdr; ch++) {
          if (!((1 << ch) & chBitmask)) {
            continue;
          }

          rankStruct = GetRankStruct (Host, socket, ch, dimm);

          //
          //set DRAMS back to normal mode
          //   Write 0 to BC0C
          //
          WriteLrbuf (Host, socket, ch, dimm, 0, LRDIMM_NORMAL_MODE, LRDIMM_F0, LRDIMM_BC0C);
          (*rankStruct)[rank].CurrentLrdimmTrainingMode = LRDIMM_NORMAL_MODE;
          //
          // dummy MRS write
          //
          WriteMRS (Host, socket, ch, dimm, rank,                       0, BANK3);   // MR3 = 0
        } // ch loop

        IODdrtReset (Host, socket, dimm);
        FifoTrainReset (Host, socket);

      } // cycleDelay loop

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        rankList = GetRankNvList (Host, socket, ch, dimm);
        rankStruct = GetRankStruct (Host, socket, ch, dimm);
        dimmNvList = GetDimmNvList (Host, socket, ch);

#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();
          RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM MDQS Coarse WL Cycle\nS%d, Ch%d, DIMM%d, Rank%d\n", socket, ch, dimm, rank);
          PrintMrecResults (Host, socket, ch, dimm, 1);
          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK

        // Gather per Strobe results
        for (Strobe = 0; Strobe < MSVx4; Strobe++) {
          if (IsStrobeNotValid (Host, Strobe)) {
            continue;
          }
          //Spec v0.92 compliant code here
          if ((*lrCwlTrainRes)[ch][Strobe].results & BIT0) {
            codedDelayCycleSetting = 6 << (4 * (Strobe / MSVx8));
          } else if ((*lrCwlTrainRes)[ch][Strobe].results & BIT1) {
            codedDelayCycleSetting = 5 << (4 * (Strobe / MSVx8));
          } else if ((*lrCwlTrainRes)[ch][Strobe].results & BIT2) {
            codedDelayCycleSetting = 0 << (4 * (Strobe / MSVx8));
          } else if ((*lrCwlTrainRes)[ch][Strobe].results & BIT3) {
            codedDelayCycleSetting = 1 << (4 * (Strobe / MSVx8));
          } else if ((*lrCwlTrainRes)[ch][Strobe].results & BIT4) {
            codedDelayCycleSetting = 2 << (4 * (Strobe / MSVx8));
          } else {
#ifdef DDRT_SUPPORT
            if ((*dimmNvList)[dimm].DcpmmPresent) {
              //
              // De-swizzle NVMDIMM strobe for FPT tracking
              //
              UINT8 SwizzleStrobe = CheckBacksideSwizzle (Host, socket, ch, dimm, Strobe);
              //
              // Failure, mark this part as faulty
              //
              (*rankList)[rank].faultyParts[SwizzleStrobe] |= FPT_LRDIMM_TRAINING_FAILED;
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_SUBCH, dimm, rank, SwizzleStrobe, NO_BIT,
                  "PMem MDQS Coarse WL - FAILED WRITE CYCLE\n");
            }
#endif // DDRT_SUPPORT
            if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
              //
              // Failure, mark this part as faulty
              //
              (*rankList)[rank].faultyParts[Strobe] |= FPT_LRDIMM_TRAINING_FAILED;
                RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_SUBCH, dimm, rank, Strobe, NO_BIT,
                  "LRDIMM MDQS Coarse WL - FAILED WRITE CYCLE\n");
            }
            codedDelayCycleSetting = 0;
          }
          (*rankList)[rank].lrBuf_FxBCDxFx[Strobe % MSVx8] &= 0xF0 >> (4 * (Strobe / MSVx8));
          (*rankList)[rank].lrBuf_FxBCDxFx[Strobe % MSVx8] |= codedDelayCycleSetting;
          (*rankStruct)[rank].cachedLrBuf_FxBCDxFx[Strobe % MSVx8] = (*rankList)[rank].lrBuf_FxBCDxFx[Strobe % MSVx8];
        } //Strobe

#ifdef DEBUG_CODE_BLOCK
        // Print rising edges
        if ((GetDebugLevel () & SDBG_MAX)) {
          if (!(*dimmNvList)[dimm].x4Present) {
            maxStrobe = MSVx8;
          } else {
            maxStrobe = MSVx4;
          }

          AcquirePrintControl ();

          RcDebugPrintLine (SDBG_MAX, 76, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);

          RcDebugPrint (SDBG_DEFAULT, "RE: ");
          for (Strobe = 0; Strobe < maxStrobe; Strobe++) {
            if (IsStrobeNotValid (Host, Strobe)) {
              continue;
            }
            //0.92 compliant results
            if ((((*rankList)[rank].lrBuf_FxBCDxFx[Strobe % MSVx8] >> (4 * (Strobe / MSVx8))) & 0xF) > 4) {
              RcDebugPrint (SDBG_DEFAULT, " %3d", 4-(((*rankList)[rank].lrBuf_FxBCDxFx[Strobe % MSVx8] >> (4 * (Strobe / MSVx8))) & 0xF));
            } else {
              RcDebugPrint (SDBG_DEFAULT, " %3d", (((*rankList)[rank].lrBuf_FxBCDxFx[Strobe % MSVx8] >> (4 * (Strobe / MSVx8))) & 0xF));
            }
          } // Strobe loop
          RcDebugPrint (SDBG_DEFAULT, "\n");

          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK

        // Make sure CWL is 0
        controlWordAddr = LRDIMM_BCDx + ((rank >> 1) * 0x20);
        controlWordData = 0;
        WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, (rank & BIT0), controlWordAddr);

        // F0BCDx for backside rank 0, F1BCDx for backside rank 1, F0BCFx for backside rank 2, and F1BCFx for backside rank 3
        controlWordDataPtr = &((*rankList)[rank].lrBuf_FxBCDxFx[0]);
        controlWordAddr = LRDIMM_BCDx + ((rank >> 1) * 0x20);
        WriteLrbufPBA (Host, socket, ch, dimm, 0, controlWordDataPtr, (rank & BIT0) , controlWordAddr, ALL_DATABUFFERS);

        faultyPartsStatus = EvaluateFaultyParts (Host, socket, ch);
        if (faultyPartsStatus == FPT_CORRECTABLE_ERROR) {
          EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_COARSE_WL_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, Strobe, NO_BIT, MrcGtDelim, LrbufLevel, 0xFF);
        } else if (faultyPartsStatus == FPT_NO_CORRECTABLE_ERROR) {
          EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_COARSE_WL_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, Strobe, NO_BIT, MrcGtDelim, LrbufLevel, 0xFF);
          DisableRank (Host, socket, ch, dimm, rank);
          //
          // if we are in encoded mode, disable ranks that share this rank (TODO: fix up for 3DS)
          //
          if ((*channelNvList)[ch].encodedCSMode) {
            DisableRank (Host, socket, ch, dimm, ((rank + 2) & 0x3));
          }
        }
      } // ch loop
    } //rank

    status = KEEP_FAILURE (RestoreDimmRankPresence (Host, socket, dimm), status);

  } //dimm
  FifoTrainReset (Host, socket);
  return status;
} //MDQCoarseWlTraining

/**

  Perform LRDIMM MDQ MDQS Write Delay Training (MWD)

  This training step leverages the comparison capability of the buffer
  MWD Training Mode to determine the best Tx data (MDQ) phase alignment
  to the strobes (MDQS).

  @param Host   - Pointer to sysHost
  @param socket - socket number

  @retval status

**/
UINT32
MDQWriteDelayTraining (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                             ch;
  UINT8                             dimm;
  UINT8                             rank;
  UINT8                             dWord;
  UINT32                            chBitmask;
  UINT8                             logRank = 0;
  UINT8                             logSubRank = 0;
  UINT32                            status = SUCCESS;
  UINT8                             strobe;
  struct channelNvram               (*channelNvList)[MAX_CH];
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  struct TrainingResults            (*lrMwdTrainRes)[MAX_CH][MAX_STROBE];
  UINT8                             DumArr[7] = {1,1,1,1,1,1,1};
  UINT8                             controlWordData;
  INT8                              mdqsWriteDelay = 0;
  UINT16                            risingEdge[MAX_CH][MAX_STROBE];
  UINT16                            centerPoint[MAX_CH][MAX_STROBE];
  UINT16                            fallingEdge[MAX_CH][MAX_STROBE];
  UINT16                            pulseWidth[MAX_CH][MAX_STROBE];
  UINT8                             faultyPartsStatus;
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, socket);
  lrMwdTrainRes = &Host->var.mem.lrMwdTrainRes;

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "DDR4 LRDIMM MDQ Write Delay Training\n");
  //
  // Train each DIMM
  //
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    //
    // Train each rank
    //
    for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
      //
      // Create a bit mask of channels that have atleast 1 rank present for this dimm and rank
      //
      chBitmask = GetChBitmaskLrdimm (socket, dimm, rank);
      //
      // Go to the next rank if this rank is not present on any channels
      //
      if (chBitmask == 0) {
        continue;
      }

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        //
        // Get the logical rank #
        //
        logRank = GetLogicalRank (Host, socket, ch, dimm, rank);
        if ((*channelNvList)[ch].encodedCSMode) {
          logSubRank = (rank >> 1);
        } else {
          logSubRank = 0;
        }
        //
        // a. Adjust the CPGC test for the target rank
        //
        SelectCPGCRanks (Host, socket, ch, 1 << logRank, 0, logSubRank);
        //
        // Clear training results variable for this channel
        //
        for (strobe = 0; strobe < MSVx4; strobe++) {
          for (dWord = 0; dWord < 4; dWord++) {
            (*lrMwdTrainRes)[ch][strobe].results[dWord] = 0;
          }
        } // strobe loop
      } // ch loop

      //
      // Early LRDIMM MDQ-MDQS Write Delay training step init
      //
      LrdimmMwdInit (Host, socket, chBitmask, dimm, rank);

      //
      //4. Setup CPGC to execute single cacheline write/read loopback test
      //
      SetupLrdimmReadWriteTest (Host, socket, chBitmask, 1, 1);

      //
      //6.  For each back-side rank [x]:
      //
      // b. Sweep F[x]BC8x and F[x]BC9x from -15/64 to + 15/64 in 1/64
      //    increments), for each setting do the following:
      //
      for (mdqsWriteDelay = LRDIMM_BACKSIDE_WRITE_DELAY_START; mdqsWriteDelay <= LRDIMM_BACKSIDE_WRITE_DELAY_END; mdqsWriteDelay += LRDIMM_BACKSIDE_WRITE_DELAY_STEP_SIZE) {
        if (mdqsWriteDelay < 0) {
          controlWordData = (UINT8) (ABS (mdqsWriteDelay) | BIT4);
        } else {
          controlWordData = (UINT8) mdqsWriteDelay;
        }
        for (ch = 0; ch < MaxChDdr; ch++) {
          if (!((1 << ch) & chBitmask)) {
            continue;
          }
          dimmNvList = GetDimmNvList (Host, socket, ch);

          WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, rank, LRDIMM_BC8x);
          WriteLrbuf (Host, socket, ch, dimm, 0, controlWordData, rank, LRDIMM_BC9x);
        } //ch
        //
        //i.  Run the CPGC write/read loopback test
        //
        RunIOTest (Host, socket, chBitmask, BasicVA, DumArr, 1, 0);
        //
        // 3. Setup HSX DDRIO in senseamp training mode, for sampling of DQ lanes.
        //
        SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_SENSE_AMP);

        //
        // Delay only if this is a CTE build, otherwise do nothing.
        //

        CteDelayQclk (40);

        //
        //ii. After the CPGC test completes, read the results of the DQ sampling
        //    in the DDRIO, and extract the per nibble composite eye (AND the results).
        //
        GetLrdimmMrdMwdResults (Host, socket, dimm, rank, controlWordData, LRDIMM_MWD_TRAINING_MODE);

        SetTrainingMode (Host, socket, dimm, rank, CHECKPOINT_MINOR_DISABLE_SENSE_AMP);

      } // end read delay sweep
      //
      //c.  Set F[x]BC4x and F[x]BC5x to the respective settings that are in the center of the passing region.
      //
      LrdimmExitRankTraining (Host, socket, chBitmask, dimm, rank);

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          Host->var.mem.piSettingStopFlag[ch] = 0;
          continue;
        }

        Host->var.mem.piSettingStopFlag[ch] = GetStrobeMask (Host);
      } // ch loop

      for (ch = 0; ch < MaxChDdr; ch++) {
        if (!((1 << ch) & chBitmask)) {
          continue;
        }

        dimmNvList = GetDimmNvList (Host, socket, ch);

        //
        //c.  Set F[x]BC8x and F[x]BC9x to the respective settings that are in the center of the passing region.
        //
#ifdef DEBUG_CODE_BLOCK
        if (GetDebugLevel () & SDBG_MAX) {
          AcquirePrintControl ();
          RcDebugPrint (SDBG_DEFAULT, "\nSummary: LRDIMM MDQ Wr Delay\nS%d, Ch%d, DIMM%d, Rank%d\n", socket, ch, dimm, rank);
          PrintSampleArray (Host, socket, ch, dimm, PRINT_ARRAY_TYPE_LRDIMM_MDQ_WR_DELAY);
          ReleasePrintControl ();
        }
#endif // DEBUG_CODE_BLOCK
        for (strobe = 0; strobe < MSVx4; strobe++) {
          //
          // Skip if this is an ECC strobe when ECC is disabled
          //
          if (IsStrobeNotValid (Host, strobe)) {
            continue;
          }

          //TODO: qualify duty cycle pass / fail
          EvaluatePiSettingResults (Host, socket, ch, dimm, rank, strobe, risingEdge, centerPoint, fallingEdge, pulseWidth,
            FPT_PI_LRDIMM_WR_MRD_TYPE);

          if (Host->nvram.mem.socket[socket].FaultyPartsFlag[ch] & (1 << strobe)) {
            //
            // Add this error to the warning log for both correctable and uncorrectable errors.
            //
            faultyPartsStatus = EvaluateFaultyParts (Host, socket, ch);

            switch (faultyPartsStatus) {
            case FPT_NO_ERROR:
              break;
            case FPT_CORRECTABLE_ERROR:
              // if correctable  log the warning -> OutputWarning
              // careful for cases when warning gets promoted to error !!!
              EwlOutputType2 (WARN_FPT_CORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_WRITE_DELAY_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, MrcGtDelim, LrbufLevel, (UINT8) pulseWidth[ch][strobe]);
              break;
            default:
              RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                             "LRDIMM training failure!!!\n");
              //
              // Add this error to the warning log for both correctable and uncorrectable errors.
              //
              Host->var.mem.piSettingStopFlag[ch] |= (1 << strobe);
              DisableChannelSw (Host, socket, ch);
              EwlOutputType2 (WARN_FPT_UNCORRECTABLE_ERROR, WARN_FPT_MINOR_LRDIMM_WRITE_DELAY_TRAINING, socket, ch, dimm, rank, EwlSeverityWarning, strobe, NO_BIT, MrcGtDelim, LrbufLevel, (UINT8) pulseWidth[ch][strobe]);
              break;
            }
          }
        } // strobe loop
#ifdef DEBUG_CODE_BLOCK
        DisplayLrdimmMrdEdges (Host, socket, ch, pulseWidth[ch], fallingEdge[ch], risingEdge[ch], centerPoint[ch], MSVx4);
#endif // DEBUG_CODE_BLOCK
        status = KEEP_FAILURE (SetLrdimmMwdResults (Host, socket, ch, dimm, rank, centerPoint[ch]), status);
      } // ch loop
      DetectLowMargin (Host, NO_CH, dimm, rank, pulseWidth, TxDqDelay, LM_LRDIMM);
    } //rank

    status = KEEP_FAILURE (RestoreDimmRankPresence (Host, socket, dimm), status);

  } //dimm

  FifoTrainReset (Host, socket);
  return status;
} //MDQWriteDelayTraining


/**

  Restore Data Buffer Rank Presence Settings:

  Restore the rank presence settings for data buffers associated with a
  particular DIMM.

  @param Host   - Pointer to sysHost
  @param socket - socket number
  @param dimm   - DIMM number

  @retval status

**/
UINT32
RestoreDimmRankPresence (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     dimm
  )
{
  UINT8                             ch;
  UINT32                            chBitmask;
  UINT32                            chStatus;
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  UINT8                             numDramRanks;
  UINT8                             rankBitMask;
  UINT32                            status = SUCCESS;
  UINT8                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {
    chBitmask = GetChBitmaskLrdimm (socket, dimm, 0);
    if (!((1 << ch) & chBitmask)) {
      continue;
    }

    dimmNvList = GetDimmNvList (Host, socket, ch);
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }

    chStatus = SUCCESS;
    numDramRanks = (*dimmNvList)[dimm].numDramRanks;
    rankBitMask = 0;
    switch (numDramRanks) {
    case 1:
      rankBitMask = 0xE;
      break;
    case 2:
      rankBitMask = 0xC;
      break;
    case 4:
      rankBitMask = 0x0;
      break;
    default:
      chStatus = FAILURE;
      status = FAILURE;
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "ERROR: Invalid number of NV DIMM ranks: %u. Keeping current settings.\n", numDramRanks);
      break;
    }
    if (chStatus == SUCCESS) {
      WriteLrbuf (Host, socket, ch, dimm, 0, rankBitMask, LRDIMM_F0, LRDIMM_BC07);
    }
  } //ch

  return status;
} //RestoreDimmRankPresence


#ifdef DEBUG_CODE_BLOCK
/**

  This function print the sample array

  @param Host      - Pointer to sysHost
  @param socket      - Socket number
  @param ch        - Channel number
  @param dimm      - DIMM number
  @param ReadMode  - 5 = LRDIMM Write MWD Delay
                     4 = LRDIMM Read DQ DQS
                     3 = LRDIMM Read Recieve Enable Phase
                     2 = LRDIMM Write DWL
                     1 = Read DqDqs
                     0 = Write DqDqs

  @retval N/A

**/
VOID
PrintMrecResults (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    ReadMode
  )
{
  UINT8   Strobe;
  UINT8   maxStrobe;
  INT8    CycleSetting;
  struct lrMrecTrainingResults (*lrMrecTrainRes)[MAX_CH][MAX_STROBE];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid (Host);
  UINT8 MSVx8 = GetMaxStrobeValid (Host) / 2;

  lrMrecTrainRes = &Host->var.mem.lrMrecTrainRes;
  if (ReadMode == 1) {
    lrMrecTrainRes = &Host->var.mem.lrCwlTrainRes;
  }

  dimmNvList = GetDimmNvList (Host, socket, ch);

  // BRS_TODO
  if ((ReadMode == 0) && !(*dimmNvList)[dimm].x4Present) {
    RcDebugPrintLine (SDBG_DEFAULT, 39, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7   8\n");
    maxStrobe = MSVx8;
  } else {
    RcDebugPrintLine (SDBG_DEFAULT, 85, SINGLE_LINE, NOCRLF_FLAG, CRLF_FLAG);
    RcDebugPrint (SDBG_DEFAULT, "       0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17\n");
    maxStrobe = MSVx4;
  }

  for (CycleSetting = -2; CycleSetting < 3; CycleSetting++) {
    RcDebugPrint (SDBG_DEFAULT, "%2d  ", CycleSetting);
    for (Strobe = 0; Strobe < maxStrobe; Strobe++) {
      if (IsStrobeNotValid (Host, Strobe)) {
        continue;
      }
      RcDebugPrint (SDBG_DEFAULT, "   ");
      if (((1 << (CycleSetting + 2)) & (*lrMrecTrainRes)[ch][Strobe].results) != 0) {
        RcDebugPrint (SDBG_DEFAULT, "1");
      } else {
        RcDebugPrint (SDBG_DEFAULT, "0");
      }
    } // Strobe loop

    RcDebugPrint (SDBG_DEFAULT, "\n");
  } // CycleSetting loop
} // PrintMrecResults

#endif // DEBUG_CODE_BLOCK

VOID
InphiPIWA (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm
  )
{
  FixedDelayMicroSecond (1);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0x03, LRDIMM_F7, LRDIMM_BC4x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0x03, LRDIMM_F0, LRDIMM_BC8x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0x00, LRDIMM_F0, LRDIMM_BC8x);
  WriteLrbuf (Host, socket, ch, dimm, 0, 0x00, LRDIMM_F7, LRDIMM_BC4x);
  FixedDelayMicroSecond (1);
} // InphiPIWA

#endif // LRDIMM_SUPPORT

/**

  Enable or disable the MPR buffer override setting for every DDR4 buffered DIMM in the socket

  @param[in] Host           - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket         - Socket number
  @param[in] MprOverride    - Enable MPR override mode if ENABLE_MPR_OVERRIDE
                            - Disable MPR override mode if DISABLE_MPR_OVERRIDE

  @retval None

**/
VOID
SetBufferMprOverrideMode (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    BOOLEAN   MprOverride
  )
{
#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               ControlWordData;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      DimmNvList = &Host->nvram.mem.socket[Socket].channelList[Ch].dimmList;
      //
      // Skip if no DIMM present
      //
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if (IsLrdimmPresent (Socket, Ch, Dimm) && (Host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
        if (MprOverride == ENABLE_MPR_OVERRIDE) {
          // enable MPR override mode in the buffer
          //
          (*DimmNvList)[Dimm].lrBuf_BC1x = (*DimmNvList)[Dimm].lrBuf_BC1x | LRDIMM_MPR_OVERRIDE_MODE;
        } else if (MprOverride == DISABLE_MPR_OVERRIDE) {
          //
          // disable MPR override mode in the buffer
          //
          (*DimmNvList)[Dimm].lrBuf_BC1x = (*DimmNvList)[Dimm].lrBuf_BC1x & ~LRDIMM_MPR_OVERRIDE_MODE;
        }
        ControlWordData = (*DimmNvList)[Dimm].lrBuf_BC1x;
        WriteLrbuf (Host, Socket, Ch, Dimm, 0, ControlWordData, LRDIMM_F0, LRDIMM_BC1x);
      } // LrDimm
    } // Dimm
  } // Ch
#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
}

/**

  Enable or disable LRDIMM Preamble TR Mode

  @param[in] Host             - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket           - CPU Socket Node number (Socket ID)
  @param[in] Ch               - Channel number (0-based)
  @param[in] Dimm             - Current dimm
  @param[in] PreambleTrMode   - Enable MPR override mode if ENABLE_PREAMBLE_TR_MODE
                              - Disable MPR override mode if DISABLE_PREAMBLE_TR_MODE

  @retval None

**/
VOID
SetLrdimmPreambleTrMode (
  IN    PSYSHOST    Host,
  IN    UINT8       Socket,
  IN    UINT8       Ch,
  IN    UINT8       Dimm,
  IN    UINT8       PreambleTrMode
  )
{
#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
  UINT8               ControlWordData;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];

  DimmNvList = &Host->nvram.mem.socket[Socket].channelList[Ch].dimmList;

  if (IsLrdimmPresent (Socket, Ch, Dimm) && (Host->nvram.mem.dramType == SPD_TYPE_DDR4)) {
    if (PreambleTrMode == ENABLE_PREAMBLE_TR_MODE) {
      //F0BC1x = 00010000b
      // enable read preamble training mode in the buffer
      (*DimmNvList)[Dimm].lrBuf_BC1x = (*DimmNvList)[Dimm].lrBuf_BC1x | LRDIMM_RD_PREAMBLE_TRAINING_MODE;
    } else if (PreambleTrMode == DISABLE_PREAMBLE_TR_MODE) {
      (*DimmNvList)[Dimm].lrBuf_BC1x = (*DimmNvList)[Dimm].lrBuf_BC1x & ~LRDIMM_RD_PREAMBLE_TRAINING_MODE;
    }
    ControlWordData = (*DimmNvList)[Dimm].lrBuf_BC1x;
    WriteLrbuf(Host, Socket, Ch, Dimm, 0, ControlWordData, LRDIMM_F0, LRDIMM_BC1x);

    if (PreambleTrMode == DISABLE_PREAMBLE_TR_MODE) {
      // IDT WA for GP B0
      if (((*DimmNvList)[Dimm].SPDRegVen == MFGID_IDT) && ((*DimmNvList)[Dimm].lrbufRid == LRBUF_IDT_DB_RID_GPB0)) {
        WriteLrbuf(Host, Socket, Ch, Dimm, 0, LRDIMM_NORMAL_MODE, LRDIMM_F0, LRDIMM_BC0C);
      }
      // END IDT WA for GP B0
    }
  }
#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
}

/**

  Prints all DIMM DQ Buffer CSR data from a specific socket.
  Note: This function is intended to be called before early RX training has been executed.
  The buffer will be placed in MPR override mode and frontside ReceiveEnable and ReadDqDqsPerBit
  training will be peformed before reading the buffer registers.

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket Id

  @retval N/A

**/
VOID
DumpStateBcwEarlyDdr4 (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
#if defined(DEBUG_CODE_BLOCK) && defined(LRDIMM_SUPPORT)
  UINT8               Ch;
  UINT8               Dimm;
  UINT8               MprPattern[MAX_BUFFER_MPR] = {0xA5, 0x33, 0x0F, 0x00, 0x00, 0x00, 0x00 ,0x00};
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DumpStateBcwEarlyDdr4 - Start\n");
  if (Host->nvram.mem.socket[Socket].lrDimmPresent == 0) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "No LRDIMM in the socket\nDumpStateBcwEarlyDdr4 - End\n");
    return;
  }

  SetBufferMprOverrideMode (Host, Socket, ENABLE_MPR_OVERRIDE);
  ReceiveEnable (Host);
  //
  // MPR0 is programmed with the same clock pattern expected by ReadDqDQsPerBit.
  // The remaining MPR registers are programmed with JEDEC default values
  //
  SetBufferMprPatternSocket (Host, Socket, MprPattern);
  ReadDqDqsPerBit (Host);

  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if (Host->nvram.mem.dimmTypePresent == RDIMM) {
        DumpStateBcwDdr4 (Host, Socket, Ch, Dimm);
      }
    } // Dimm
  } // Ch
  SetBufferMprOverrideMode (Host, Socket, DISABLE_MPR_OVERRIDE);
  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DumpStateBcwEarlyDdr4 - End\n");
#endif // defined(DEBUG_CODE_BLOCK) && defined(LRDIMM_SUPPORT)
}

/**

  Prints all DQ Buffer CSR data from a specific DIMM.
  Note: this function assumes that the read training has been successfully
  completed in order to read DRAM and Buffer data.

  @param[in] Host     - Pointer to sysHost
  @param[in] Socket   - Socket Id
  @param[in] Ch       - Channel number within the socket
  @param[in] Dimm     - DIMM number within the channel

  @retval N/A

**/
VOID
DumpStateBcwDdr4 (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm
  )
{
#if defined(DEBUG_CODE_BLOCK) && defined(LRDIMM_SUPPORT)
  UINT8               Strobe;
  UINT8               BcwFunc = 0;
  UINT8               BcwByte;
  UINT8               BcwNibble;
  UINT32              PrevDebugLevel;
  UINT8               MprData[MAX_STROBE];
  UINT8               MSVx8 = GetMaxStrobeValid (Host) / 2;

  //debug only!!!
  //disableAllMsg(Host);
  //WriteLrbuf(Host, Socket, Ch, Dimm, 0, 0x1, LRDIMM_F0, LRDIMM_BC00);
  //WriteLrbuf(Host, Socket, Ch, Dimm, 0, 0x2, LRDIMM_F0, LRDIMM_BC04);
  //WriteLrbuf(Host, Socket, Ch, Dimm, 0, 0x5, LRDIMM_F0, LRDIMM_BC05);
  //restoreMsg(Host);

  if (IsLrdimmPresent (Socket, Ch, Dimm)) {
    PrevDebugLevel = GetDebugLevel ();

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "DDR4 Buffer CW data:\n");
    AcquirePrintControl ();
    RcDebugPrint (SDBG_DEFAULT, "DQ Index:         0     8     16    24    32    40    48    56    64\n");

    for (BcwFunc = 0; BcwFunc < 8; BcwFunc++) {
      for (BcwByte = 0; BcwByte < 16; BcwByte++) {

        if (BcwFunc == 0 && BcwByte == 0) {

          // per nibble
          for (BcwNibble = 0; BcwNibble < 16; BcwNibble += 2) {

            // Note: IDT buffer always returns nibble[1:0] regardless of the nibble address or auto-increment setting
            if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
              SetDebugLevel (SDBG_ERROR);
            }
            ReadBcwDimmDdr4 (Host, Socket, Ch, Dimm, BcwFunc, BcwByte, BcwNibble, MprData);
            SetDebugLevel (PrevDebugLevel);
            RcDebugPrint (SDBG_DEFAULT, "F%d, Nibble 0x%02x: ", BcwFunc, BcwNibble);

            for (Strobe = 0; Strobe < MSVx8; Strobe++) {
              RcDebugPrint (SDBG_DEFAULT, " 0x%02x ", MprData[Strobe]);
            }
            RcDebugPrint (SDBG_DEFAULT, "\n");

          } // BcwNibble
        } else {

          // per byte
          if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
            SetDebugLevel (SDBG_ERROR);
          }
          ReadBcwDimmDdr4 (Host, Socket, Ch, Dimm, BcwFunc, BcwByte, 0, MprData);
          SetDebugLevel (PrevDebugLevel);
          RcDebugPrint (SDBG_DEFAULT, "F%d, Byte   0x%02x: ", BcwFunc, BcwByte);

          for (Strobe = 0; Strobe < MSVx8; Strobe++) {
            RcDebugPrint (SDBG_DEFAULT, " 0x%02x ", MprData[Strobe]);
          }
          RcDebugPrint (SDBG_DEFAULT, "\n");

        } //if BcwByte == 0
      } //for BcwByte
    } //for BcwFunc
    RcDebugPrint (SDBG_DEFAULT, "\n");
    ReleasePrintControl ();

  }//LRDIMM
#endif // defined(DEBUG_CODE_BLOCK) && defined(LRDIMM_SUPPORT)
}

