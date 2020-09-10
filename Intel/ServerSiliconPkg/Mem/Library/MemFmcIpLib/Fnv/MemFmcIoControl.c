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
#include <SysHostChip.h>
#include <Chip/Include/CpuPciAccess.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include "Chip/Include/FnvAccessCommon.h"
#include <Library/MemFmcIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemRcLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/IoAccessLib.h>
#include "Include/MemFmcRegs.h"
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/PerformanceTrackerLib.h>
#include "Include/MemFmcIpLibInternal.h"

//
// Local Prototypes
//

/**

Initial backside (BCOM) DLL for margin test

@param Host  - Pointer to sysHost

@retval none

**/
VOID
EnableBcomDll (
  PSYSHOST  Host
)
{
  UINT8 Socket;
  UINT8 Ch;
  UINT8 Dimm;
  UINT8 MaxChDdr;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_STRUCT   ddrCRCmdControls3;
  DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_STRUCT   ddrCRCmdControls2;  // aka. DDRCRCMDCONTROLS2_3_FNV_DDRIO_DAT7_CH_STRUCT
  MSC_FNVIO_DDRTDLL_CSR_FNV_DFX_MISC_0_STRUCT     fnvIoDdrtDll;
  DDRCRCMDCONTROLS2_0_FNV_DDRIO_DAT7_CH_STRUCT    ddrCRCmdControls2_0;
  DDRCRCMDCONTROLS2_1_FNV_DDRIO_DAT7_CH_STRUCT    ddrCRCmdControls2_1;
  DDRCRCMDCONTROLS2_2_FNV_DDRIO_DAT7_CH_STRUCT    ddrCRCmdControls2_2;
  MSC_FNVIO_INIT_CONTROL_1_FNV_DFX_MISC_0_STRUCT  fnvIoInitCtl;

  Socket = Host->var.mem.currentSocket;

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "\nInitialize FNV backside DLL.\n");

  channelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*channelNvList)[Ch].ddrtEnabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*dimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      if (!(*dimmNvList)[Dimm].DcpmmPresent) {
        continue;
      }

      fnvIoDdrtDll.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, MSC_FNVIO_DDRTDLL_CSR_FNV_DFX_MISC_0_REG);
      fnvIoDdrtDll.Bits.ddrtdllpowerdown01qnnnh_val = 0;
      fnvIoDdrtDll.Bits.ddrtdllpowerdown23qnnnh_val = 0;
      fnvIoDdrtDll.Bits.ddrtdllpowerdown4qnnnh_val = 0;
      fnvIoDdrtDll.Bits.en = 0;
      WriteFnvCfg (Host, Socket, Ch, Dimm, MSC_FNVIO_DDRTDLL_CSR_FNV_DFX_MISC_0_REG, fnvIoDdrtDll.Data);

      GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2.Data, 0, FNV_IO_READ, 0xF);
      ddrCRCmdControls2.Bits.fnvddrcrdllpibypassen = 0;
      GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCONTROLS2_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2.Data, 0, FNV_IO_WRITE, 0xF);

      // Not sure if they are necessary to set below 2_0, 2_1 and 2_2 registers.
      GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCONTROLS2_0_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2_0.Data, 0, FNV_IO_READ, 0xF);
      ddrCRCmdControls2_0.Bits.fnvddrcrdllpibypassen = 0;
      GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCONTROLS2_0_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2_0.Data, 0, FNV_IO_WRITE, 0xF);

      GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCONTROLS2_1_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2_1.Data, 0, FNV_IO_READ, 0xF);
      ddrCRCmdControls2_1.Bits.fnvddrcrdllpibypassen = 0;
      GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCONTROLS2_1_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2_1.Data, 0, FNV_IO_WRITE, 0xF);

      GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCONTROLS2_2_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2_2.Data, 0, FNV_IO_READ, 0xF);
      ddrCRCmdControls2_2.Bits.fnvddrcrdllpibypassen = 0;
      GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCONTROLS2_2_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls2_2.Data, 0, FNV_IO_WRITE, 0xF);

      GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls3.Data, 0, FNV_IO_READ, 0xF);
      ddrCRCmdControls3.Bits.cmdbypass = 1;
      ddrCRCmdControls3.Bits.drvencrovrd = 0;
      GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCONTROLS3_SA_FNV_DDRIO_DAT7_CH_REG, &ddrCRCmdControls3.Data, 0, FNV_IO_WRITE, 0xF);

      fnvIoInitCtl.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, MSC_FNVIO_INIT_CONTROL_1_FNV_DFX_MISC_0_REG);
      WriteFnvCfg (Host, Socket, Ch, Dimm, MSC_FNVIO_INIT_CONTROL_1_FNV_DFX_MISC_0_REG, fnvIoInitCtl.Data);

      IODdrtResetPerCh (Host, Socket, Ch, Dimm);
    }
  }
}

/**

  GetSetTxVrefFnv

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)
  @param strobe  - Strobe number (0-based)
  @param bit     - Bit number
  @param level   - CSM_LT - Level to program (DDR, VMSE, Buffer)
  @param group   - CSM_GT - Parameter to program
  @param mode    - 0 - do not read from cache, 1 - read from cache, 2 - use cache to program register (used for fast boot or S3)
  @param value   - Data to program

  @retval status

**/
MRC_STATUS
GetSetTxVrefFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     rank,
  UINT8     strobe,
  UINT8     bit,
  MRC_LT    level,
  MRC_GT    group,
  UINT8     mode,
  INT16     *value
  )
{
  UINT8                             maxStrobe;
  UINT16                            curVal = 0;
  UINT16                            newVref;
  INT16                             tmp;
  UINT16                            maxLimit = 127;
  UINT16                            minLimit = 0;
  UINT16                            usDelay = 1;
  UINT8                             mask = 0;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  RXVREFCTRLN0_0_FNV_DDRIO_COMP_STRUCT  txVrefCtrl;
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  CountTrackingData (PTC_VREF_MOVE, 1);

  rankList = GetRankNvList(Host, socket, ch, dimm);
  rankStruct  = GetRankStruct(Host, socket, ch, dimm);
  txVrefCtrl.Data = 0;

  if (bit == 0xFF) {
    mask = 0xF;
  } else {
    mask = 1 << bit;
  }
  //
  // Get minimum and maximum value
  //
  GetDataGroupLimits(Host, level, group, &minLimit, &maxLimit, &usDelay);
  UpdateDdrtGroupLimits(Host, socket, ch, dimm, level, group, &minLimit, &maxLimit);

  if (strobe >= MAX_STROBE && strobe != ALL_STROBES) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, strobe, NO_BIT,
                   "The strobe value is out of range!\n");
    RC_FATAL_ERROR ((strobe < MAX_STROBE || strobe == ALL_STROBES), ERR_ARRAY_OUT_OF_BOUNDS,  RC_FATAL_ERROR_MINOR_CODE_0);
    return MRC_STATUS_STROBE_NOT_PRESENT;
  }

  if (strobe == ALL_STROBES) {
    strobe    = 0;
    maxStrobe = MSVx4;
  } else {
    maxStrobe = strobe + 1;
  }

  for (; strobe < maxStrobe; strobe++) {

    if (mode & GSM_READ_CSR) {
      {
        //
        // Read from PCI config space
        //
        // GetSetFnvIO
        GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, RXVREFCTRLN0_0_FNV_DDRIO_COMP_REG), &txVrefCtrl.Data ,0, FNV_IO_READ, mask);
      }
    } else {
      //
      // Read from cache
      //
      {
        txVrefCtrl.Data = (*rankStruct)[rank].lrbufTxVrefCache[strobe];
      }
    }

      if (bit == ALL_BITS) {
        curVal = (UINT16)txVrefCtrl.Bits.vrefperbit0;
      } else {
        switch (bit) {
        case 0:
          curVal = (UINT16)txVrefCtrl.Bits.vrefperbit0;
          break;

        case 1:
          curVal = (UINT16)txVrefCtrl.Bits.vrefperbit1;
          break;

        case 2:
          curVal = (UINT16)txVrefCtrl.Bits.vrefperbit2;
          break;

        case 3:
          curVal = (UINT16)txVrefCtrl.Bits.vrefperbit3;
          break;
          default:
            //Automated add of default case. Please review.
            break;
        } // switch (bit)
      }

    //
    // Check if read only
    //
    if (mode & GSM_READ_ONLY) {
      //
      // Save the value read into the output parameter
      //
      *value = curVal;
    } else {
      //
      // Write
      //
      // Adjust the current CMD delay value by offset
      if (mode & GSM_WRITE_OFFSET) {
        //
        // Add the offset to the current value
        //
        tmp = curVal + *value;

        //
        // Make sure we do not exeed the limits
        //
        if (tmp > minLimit) {
          newVref = tmp;
        } else {
          newVref = minLimit;
        }
        // Ensure we do not exceed maximums
        if (newVref > maxLimit) {
          newVref = maxLimit;
        }
      } else {
        //
        // Write absolute value
        //
        newVref = *value;
      }

      if (bit == ALL_BITS) {
        txVrefCtrl.Bits.vrefperbit0 = newVref;
        txVrefCtrl.Bits.vrefperbit1 = newVref;
        txVrefCtrl.Bits.vrefperbit2 = newVref;
        txVrefCtrl.Bits.vrefperbit3 = newVref;
      } else {
        switch (bit) {
        case 0:
          txVrefCtrl.Bits.vrefperbit0 = newVref;
          break;

        case 1:
          txVrefCtrl.Bits.vrefperbit1 = newVref;
          break;

        case 2:
          txVrefCtrl.Bits.vrefperbit2 = newVref;
          break;

        case 3:
          txVrefCtrl.Bits.vrefperbit3 = newVref;
          break;
          default:
            //Automated add of default case. Please review.
            break;
        } // switch (bit)
      }


      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
      if ((txVrefCtrl.Data != (*rankList)[rank].lrbufTxVref[strobe]) || (mode & GSM_FORCE_WRITE)) {
        GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, RXVREFCTRLN0_0_FNV_DDRIO_COMP_REG), &txVrefCtrl.Data ,0, FNV_IO_WRITE, mask);
        IODdrtResetPerCh (Host, socket, ch, dimm);
        //
        // Wait for the new value to settle
        //
        (*rankList)[rank].lrbufTxVref[strobe] = txVrefCtrl.Data;

        FixedDelayMicroSecond (usDelay);
      }
    } // if read only

    //
    // Save to cache
    //
    if (mode & GSM_UPDATE_CACHE) {
       (*rankStruct)[rank].lrbufTxVrefCache[strobe] = (*rankList)[rank].lrbufTxVref[strobe];
    }

  } // strobe loop

  return MRC_STATUS_SUCCESS;
} // GetSetTxVrefFnv


/**

  CacheTxVrefFnv

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)

  @retval status

**/
MRC_STATUS
CacheTxVrefFnv (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     Dimm,
  UINT8     Rank
  )
{
  UINT8  Strobe;
  RXVREFCTRLN0_0_FNV_DDRIO_COMP_STRUCT        txVrefCtrl;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  rankList = GetRankNvList(Host, Socket, Ch, Dimm);
  rankStruct  = GetRankStruct(Host, Socket, Ch, Dimm);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
    GetSetFnvIO (Host, Socket, Ch, Dimm, UpdateIoRegisterChDdrt (Host, Strobe, RXVREFCTRLN0_0_FNV_DDRIO_COMP_REG), &txVrefCtrl.Data, 0, FNV_IO_READ, 0xF);
    (*rankList)[Rank].lrbufTxVref[Strobe] = txVrefCtrl.Data;
    (*rankStruct)[Rank].lrbufTxVrefCache[Strobe] = (*rankList)[Rank].lrbufTxVref[Strobe];
  } // strobe
  return MRC_STATUS_SUCCESS;
}




/**

  InitDdrioInterfaceLateFmc

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)
  @param dimm    - DIMM number (0-based)
  @param rank    - Rank number (0-based)

  @retval status

**/
MRC_STATUS
InitDdrioInterfaceLateFmc (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  UINT8                                       ch;
  UINT8                                       dimm;
  UINT16                                      minVal = 0xFF;
  UINT16                                      maxVal = 0x0;
  INT16                                       piDelayfnv = 64;
  struct channelNvram                         (*channelNvList)[MAX_CH];
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];
  SYS_SETUP                                   *Setup;
  UINT8                                       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList(Host, Socket);

  if (Setup->mem.enableNgnBcomMargining) {
    if (Host->nvram.mem.DcpmmPresent) {
      CrossoverCalibFnv(Host);
      //
      // Enable DLL for BCOM margin test
      //
      EnableBcomDll (Host);
    }
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      dimmNvList = GetDimmNvList(Host, Socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }
        if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
          continue;
        }

        GetSetClkDelayCore(Host, Socket, ch, dimm, LrbufLevel, 0, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelayfnv);
        GetSetCtlGroupDelayCore(Host, Socket, ch, dimm, LrbufLevel, CtlAll, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelayfnv, &minVal, &maxVal);
        GetSetCmdGroupDelayCore(Host, Socket, ch, dimm, LrbufLevel, CmdAll, GSM_FORCE_WRITE | GSM_WRITE_OFFSET | GSM_UPDATE_CACHE, &piDelayfnv, &minVal, &maxVal);
      } // dimm
    } // ch
  } // IsBacksideCmdMarginEnabled

  return MRC_STATUS_SUCCESS;
} //InitDdrioInterfaceLateFmc


/**

  SetTxPiOnFmc

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param ch      - Channel number (0-based)

  @retval status

**/
MRC_STATUS
SetTxPiOnFmc (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT32    TxPiOn
  )
{
  UINT8                                       dimm;
  UINT8                                       strobe;
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];
  DATACONTROL0N0_0_FNV_DDRIO_COMP_STRUCT      dataControl0fnv;
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  //
  //IO power optimization requires DDRT training update. This could go on FW rather than BIOS.
  //
  dimmNvList = GetDimmNvList(Host, Socket, Ch);
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
      continue;
    }

    for (strobe = 0; strobe < MSVx4; strobe++) {
      GetSetFnvIO(Host, Socket, Ch, dimm, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG, &dataControl0fnv.Data, 0, FNV_IO_READ, 0xF);
      dataControl0fnv.Bits.txpion = TxPiOn;
      dataControl0fnv.Bits.rxpion = TxPiOn;
      GetSetFnvIO (Host, Socket, Ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG), &dataControl0fnv.Data, 0, FNV_IO_WRITE, 0xF);
    } // strobe loop
    IODdrtResetPerCh (Host, Socket, Ch, dimm);
  }
  return MRC_STATUS_SUCCESS;
}

/**
  Resets the DDRT DDRIO

  @param[in]  Host              Pointer to sysHost
  @param[in]  Socket            Socket Id
  @param[in]  ChannelInSocket   Channel number (0-based)
  @param[in]  Dimm              Zero based DIMM number

  @retval N/A
**/
VOID
EFIAPI
BiosAepIoResetChip (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     ChannelInSocket,
  IN  UINT8     Dimm
  )
{
  MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_STRUCT   FnvioCtl;

  FnvioCtl.Data = ReadFnvCfg (Host, Socket, ChannelInSocket, Dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG);

  FnvioCtl.Bits.mc2ddrttrainreset = 1;
  WriteFnvCfg (Host, Socket, ChannelInSocket, Dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, FnvioCtl.Data);

  FnvioCtl.Bits.mc2ddrttrainreset = 0;
  WriteFnvCfg (Host, Socket, ChannelInSocket, Dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, FnvioCtl.Data);
}

/**

  This routine caches the FNV DDRTIO CSRs required by training steps

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param Ch                                - Channel number (0-based)

  @retval N/A

**/
VOID
UpdateFnvDdrioCache10nm (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  UINT8                                dimm;
  UINT8                                rank = 0;
  INT16                                zeroOffset = 0;
  struct dimmNvram                     (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(Host, socket, ch);
  for(dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
      continue;
    }
    GetSetCmdVref(Host, socket, ch, dimm, DdrLevel, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &zeroOffset);
    GetSetDataGroup(Host, socket, ch, NO_SUBCH, dimm, rank, ALL_STROBES, ALL_BITS, LrbufLevel, TxVref, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &zeroOffset);
  }
}

/**

Enable / disable 32 B2B GNTs without NODATA. Needed only for DDRT

@param Host                              - Pointer to sysHost
@param Socket                            - Socket Id
@param Value                             - Enable / Disable

@retval N/A

**/
VOID
Grant32ContEn (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Value
)
{
  UINT8                                             ch;
  UINT8                                             dimm;
  UINT8                                             FmcType;
  struct channelNvram                               (*channelNvList)[MAX_CH];
  struct dimmNvram                                  (*dimmNvList)[MAX_DIMM];
  DA_DDRT_FUNC_DEFEATURE1_FNV_DA_UNIT_0_STRUCT      ddrtFunctionalDefeature;
  UINT8                                             MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);
  if (Value != 0) {
    Value = 1;
  }
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Only apply to BWV architecture
    //
    FmcType = GetFmcType (Socket, ch);
    if (FmcType == FMC_BWV_TYPE) {
      dimmNvList = GetDimmNvList (Host, Socket, ch);
      //
      // Enable / Disable 32 B2B GNTs without NODATA
      //
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if (((*dimmNvList)[dimm].dimmPresent) == 0) {
          continue;
        }
        if (((*dimmNvList)[dimm].DcpmmPresent) == 0) {
          continue;
        }

        ddrtFunctionalDefeature.Data = 0;
        ddrtFunctionalDefeature.Bits.gnt_32_cont_en = Value;
        GetSetProtectedDcpmmRegister (Host, Socket, ch, dimm, GSM_FORCE_WRITE, DA_DDRT_FUNC_DEFEATURE1_FNV_DA_UNIT_0_REG,
          PROT_REG_MASK_GNT_32_CONT_EN, &ddrtFunctionalDefeature.Data);

      } // dimm loop
    }
  } // ch loop
}


const UINT32 ddrioOffsetTableDdrt[MAX_STROBE] = { 0x200, 0x100, 0xc00, 0xb00, 0x700, 0x400, 0x300, 0x000, 0x800,
0x300, 0x000, 0xd00, 0xa00, 0x600, 0x500, 0x200, 0x100, 0x900 };


/**

This function returns the register address with propper swizzle.
DDRT IO swizzle will not change from SKX. Hence DDRT IO
registers have to use this function

@param strobe            - Strobe number (0-based)
@param RegisterGroupBase - Address of the the register group

@retval IO Address

**/
UINT32
UpdateIoRegisterChDdrt (
  PSYSHOST    Host,
  UINT8       strobe,
  UINT32      regOffset
  )
{
  CSR_OFFSET  csrReg;
  UINT32      realOffset;

  csrReg.Data = regOffset;

  //check to make sure that strobe is within the range of strobes (used as array index in function)
  MemCheckIndex (strobe, MAX_STROBE);

  // Convert pseudo offset to real offset
  realOffset = MemPciCfgOffset (0, 0, regOffset);

  csrReg.Data = realOffset;

  //
  // SKX change. Bytes 0 and 1 are in the DDRIO_EXT function
  //
  if ((strobe == 0) || (strobe == 1) || (strobe == 9) || (strobe == 10)) {
    csrReg.Bits.funcblk = csrReg.Bits.funcblk + 1;
  }
  //
  // Add in offset for data group
  //
  csrReg.Bits.offset = csrReg.Bits.offset + ddrioOffsetTableDdrt[strobe];

  return csrReg.Data;
} // UpdateIoRegisterChDdrt

/**

  BIOS shall drive tristate on RespA and RespB

  @param[in]  Socket    - Socket

  @retval status

  **/
VOID
EFIAPI
DdrtDriveRespARespBHigh (
  IN  UINT8     Socket
  )
{
  // NULL
} // DdrtDriveRespARespBHigh