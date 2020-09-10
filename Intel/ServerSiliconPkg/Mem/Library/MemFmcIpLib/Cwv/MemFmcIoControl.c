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
#include <Library/EmulationConfigurationLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/IoAccessLib.h>
#include "Include/MemFmcRegs.h"
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/PerformanceTrackerLib.h>
#include "Include/MemFmcIpLibInternal.h"
#include "Include/MemFmcJedec.h"
#include "Include/Memory/Ddr5MrRegs.h"

#define NUM_CWV_DCA_DFE_GAIN_BIAS            7

//
// Local Prototypes
//

const UINT32 ddrPhyDdrDataOffset[] = {
  0x1600, //Strobe 0 SubChannel A
  0x1200, //Strobe 1 SubChannel A
  0xE00,  //Strobe 2 SubChannel A
  0x1000, //Strobe 3 SubChannel A
  0xC00,  //Strobe 4 SubChannel A
  0xA00,  //Strobe 0 SubChannel B
  0x400,  //Strobe 1 SubChannel B
  0x600,  //Strobe 2 SubChannel B
  0x200,  //Strobe 3 SubChannel B
  0x800   //Strobe 4 SubChannel B
};


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
//CWV_TODO  RXVREFCTRLN0_0_FNV_DDRIO_COMP_STRUCT  txVrefCtrl;
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  CountTrackingData (PTC_VREF_MOVE, 1);

  rankList = GetRankNvList(Host, socket, ch, dimm);
  rankStruct  = GetRankStruct(Host, socket, ch, dimm);
//CWV_TODO  txVrefCtrl.Data = 0;

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
//CWV_TODO        GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, RXVREFCTRLN0_0_FNV_DDRIO_COMP_REG), &txVrefCtrl.Data ,0, FNV_IO_READ, mask);
      }
    } else {
      //
      // Read from cache
      //
      {
//CWV_TODO        txVrefCtrl.Data = (*rankStruct)[rank].lrbufTxVrefCache[strobe];
      }
    }

/**
//CWV_TODO
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
**/

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

/**
//CWV_TODO
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
**/

      //
      // Write it back if the current data does not equal the cache value or if in force write mode
      //
//CWV_TODO      if ((txVrefCtrl.Data != (*rankList)[rank].lrbufTxVref[strobe]) || (mode & GSM_FORCE_WRITE)) {
//CWV_TODO        GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, RXVREFCTRLN0_0_FNV_DDRIO_COMP_REG), &txVrefCtrl.Data ,0, FNV_IO_WRITE, mask);
//CWV_TODO        IODdrtResetPerCh (Host, socket, ch, dimm);
        //
        // Wait for the new value to settle
        //
//CWV_TODO        (*rankList)[rank].lrbufTxVref[strobe] = txVrefCtrl.Data;

//CWV_TODO        FixedDelayMicroSecond (usDelay);
//CWV_TODO      }
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
//CWV_TODO  RXVREFCTRLN0_0_FNV_DDRIO_COMP_STRUCT        txVrefCtrl;
  struct ddrRank                    (*rankList)[MAX_RANK_DIMM];
  struct rankDevice                 (*rankStruct)[MAX_RANK_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  rankList = GetRankNvList(Host, Socket, Ch, Dimm);
  rankStruct  = GetRankStruct(Host, Socket, Ch, Dimm);

  for (Strobe = 0; Strobe < MSVx4; Strobe++) {
//CWV_TODO    GetSetFnvIO (Host, Socket, Ch, Dimm, UpdateIoRegisterChDdrt (Host, Strobe, RXVREFCTRLN0_0_FNV_DDRIO_COMP_REG), &txVrefCtrl.Data, 0, FNV_IO_READ, 0xF);
//CWV_TODO    (*rankList)[Rank].lrbufTxVref[Strobe] = txVrefCtrl.Data;
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
//CWV_TODO  DATACONTROL0N0_0_FNV_DDRIO_COMP_STRUCT      dataControl0fnv;
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
//CWV_TODO      GetSetFnvIO(Host, Socket, Ch, dimm, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG, &dataControl0fnv.Data, 0, FNV_IO_READ, 0xF);
//CWV_TODO      dataControl0fnv.Bits.txpion = TxPiOn;
//CWV_TODO      dataControl0fnv.Bits.rxpion = TxPiOn;
//CWV_TODO      GetSetFnvIO (Host, Socket, Ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG), &dataControl0fnv.Data, 0, FNV_IO_WRITE, 0xF);
    } // strobe loop
    IODdrtResetPerCh (Host, Socket, Ch, dimm);
  }
  return MRC_STATUS_SUCCESS;
}

/**

  This routine sets the CSRs required by BiosAepIoResetChip

  @param Host                              - Pointer to sysHost
  @param Socket                            - Socket Id
  @param ChannelInSocket                   - Channel number (0-based)
  @param Dimm                              - Zero based DIMM number.

  @retval N/A

**/
VOID
BiosAepIoResetChip (
  PSYSHOST                              Host,
  UINT8                                 Socket,
  UINT8                                 ChannelInSocket,
  UINT8                                 Dimm
  )
{
  struct dimmNvram                                 (*dimmNvList)[MAX_DIMM];
//CWV_TODO  MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_STRUCT   fnvioCtl;

  dimmNvList = GetDimmNvList (Host, Socket, ChannelInSocket);

  //Set FNV MC Reset IO Bits
//CWV_TODO  fnvioCtl.Data = (*dimmNvList)[Dimm].fnvioControl;
//CWV_TODO  fnvioCtl.Bits.mc2ddrttrainreset = 1;
//CWV_TODO  WriteFnvCfg(Host, Socket, ChannelInSocket, Dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, fnvioCtl.Data);
//CWV_TODO  fnvioCtl.Bits.mc2ddrttrainreset = 0;
//CWV_TODO  WriteFnvCfg(Host, Socket, ChannelInSocket, Dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, fnvioCtl.Data);
//CWV_TODO  (*dimmNvList)[Dimm].fnvioControl = fnvioCtl.Data;
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
//CWV_TODO  DA_DDRT_FUNC_DEFEATURE1_FNV_DA_UNIT_0_STRUCT      ddrtFunctionalDefeature;
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

//CWV_TODO        ddrtFunctionalDefeature.Data = ReadFnvCfg (Host, Socket, ch, dimm, DA_DDRT_FUNC_DEFEATURE1_FNV_DA_UNIT_0_REG);
//CWV_TODO        ddrtFunctionalDefeature.Bits.gnt_32_cont_en = Value;
//CWV_TODO        WriteFnvCfg (Host, Socket, ch, dimm, DA_DDRT_FUNC_DEFEATURE1_FNV_DA_UNIT_0_REG, ddrtFunctionalDefeature.Data);
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
  PSYSHOST      Host;
  UINT8         Ch;
  UINT8         Dimm;
  UINT8         MaxChDdr;
  DDRT_HSIO_CCC_CFG8_CWV_DDRIO_HSIO_STRUCT CccCfg8;
  DDRT_HSIO_CCC_CFG5_CWV_DDRIO_HSIO_STRUCT CccCfg5;

  Host = GetSysHostPointer ();
  MaxChDdr    = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }
      if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == FALSE) {
        continue;
      }

      CccCfg8.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, DDRT_HSIO_CCC_CFG8_CWV_DDRIO_HSIO_REG);
      CccCfg5.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, DDRT_HSIO_CCC_CFG5_CWV_DDRIO_HSIO_REG);

      CccCfg8.Bits.txtype_ccc2_buf0 = 0;  // To drive REQ# High
      CccCfg5.Bits.txtype_ccc0_buf0 = 0;  // To drive ERR# High

      WriteFnvCfg (Host, Socket, Ch, Dimm, DDRT_HSIO_CCC_CFG8_CWV_DDRIO_HSIO_REG, CccCfg8.Data);
      WriteFnvCfg (Host, Socket, Ch, Dimm, DDRT_HSIO_CCC_CFG5_CWV_DDRIO_HSIO_REG, CccCfg5.Data);
    } // Dimm
  } // Ch
} // DdrtDriveRespARespBHigh

/**
  Cwv Tx Vref build target function.

  @param[in]  Host              Ptr to SysHost.
  @param[in]  Socket            Socket.
  @param[in]  SysBootMode       Boot Mode.

  @retval EFI_SUCCESS           Handled successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
CacheTxVrefCwvTarget (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN BootMode  SysBootMode
  )
{
  UINT8 Channel;
  UINT8 Dimm;
  UINT8 Rank;
  struct channelNvram (*ChannelNvList)[MAX_CH];
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  INT16 ZeroOffset = 0;
  UINT8 MaxChDdr;

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    return EFI_SUCCESS;
  }

  ChannelNvList = GetChannelNvList(Host, Host->var.mem.currentSocket);
  MaxChDdr      = GetMaxChDdr ();

  if (((SysBootMode == NormalBoot) && ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast)))) {

    if (!(UbiosGenerationOrHsleEnabled())) {

      for (Channel = 0; Channel < MaxChDdr; Channel++) {

        if ((*ChannelNvList)[Channel].enabled == 0) {
          continue;
        }

        DimmNvList = GetDimmNvList(Host, Socket, Channel);
        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

          if ((*DimmNvList)[Dimm].dimmPresent == 0) {
            continue;
          }

          if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
            continue;
          }

          GetSetCmdVref(Host, Socket, Channel, Dimm, DdrLevel, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &ZeroOffset);
          GetSetCmdVref(Host, Socket, Channel, Dimm, LrbufLevel, GSM_READ_CSR | GSM_READ_ONLY | GSM_UPDATE_CACHE, &ZeroOffset);

          for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
            if (CheckRank(Host, Socket, Channel, Dimm, Rank, DONT_CHECK_MAPOUT)) {
              continue;
          }

            CacheTxVrefFnv(Host, Socket, Channel, Dimm, Rank);
          } // rank
        } // Dimm
      } // Channel
    }
  }
  return EFI_SUCCESS;

} // CacheTxVrefCwvTarget

/**

  DDRT2 Receive Enable Training

  @param[in]  Host      - Pointer to sysHost
  @param[in]  Socket    - Socket
  @param[in]  Ch        - Channel numer within the socket
  @param[in]  SubChMask - Mask of subchannels within the channel
  @param[in]  Dimm      - DIMM number within the channel
  @param[in]  Rank      - Rank number of the DIMM
  @param[in]  Enable    - TRUE to enable Receive Enable training mode

  @retval status

  **/
VOID
EFIAPI
SetFMCReceiveEnableModeDdrt2 (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     SubChMask,
  IN  UINT8     Dimm,
  IN  UINT8     Rank,
  IN  BOOLEAN   Enable
  )
{

  UINT8     strobe;
  UINT8     maxStrobe = 5;
  UINT8     SubCh;
  UINT16    PreambleState = 0;
  UINT32    wrPreambleSetting;
  UINT32    dqsPulseMask;
  UINT32    parkMode;

  MR8_WR_RD_PRE_POSTAMBLE_CWV_DDRIO_HSIO_STRUCT       DdrtMr8Hsio;

  DDRDATA_CR_DDRCRDATACONTROL3_0_0_0_MCHBAR_CWV_DDRIO_DAT_STRUCT           DdrDataControl3;
  DDRDATA_CR_DDRCRCMDBUSTRAIN_0_0_0_MCHBAR_CWV_DDRIO_DAT_STRUCT            DdrCrCmdBusTrain;
  DDRDATA_CR_DDRCRDATACONTROL0_0_0_0_MCHBAR_CWV_DDRIO_DAT_STRUCT           DdrCrDataControl0;

  DdrtMr8Hsio.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, MR8_WR_RD_PRE_POSTAMBLE_CWV_DDRIO_HSIO_REG);

  //
  // Enable Read Preamble Training Mode in FMC.
  //
  if (Enable) {
    DdrtMr8Hsio.Bits.read_preamble = READ_PREAMBLE_1TCK;
    wrPreambleSetting = 0;
    dqsPulseMask = 0;
    parkMode = PARK_DIFF_LOW;
  }
  else {
    parkMode = PARK_OFF;
    PreambleState = GetPreambleState(Socket, READ_PREAMBLE);
    switch (PreambleState) {
      case PREAMBLE_1TCLK:
        DdrtMr8Hsio.Bits.read_preamble = READ_PREAMBLE_1TCK;
        wrPreambleSetting = 0;
        dqsPulseMask = 0;
        break;
      case PREAMBLE_2TCLK:
        DdrtMr8Hsio.Bits.read_preamble = READ_PREAMBLE_2TCK;
        wrPreambleSetting = 1;
        dqsPulseMask = 1;
        break;
      case PREAMBLE_3TCLK:
        DdrtMr8Hsio.Bits.read_preamble = READ_PREAMBLE_3TCK;
        wrPreambleSetting = 2;
        dqsPulseMask = 2;
        break;
      case PREAMBLE_4TCLK:
        DdrtMr8Hsio.Bits.read_preamble = READ_PREAMBLE_4TCK;
        wrPreambleSetting = 3;
        dqsPulseMask = 2;
        break;
      default:
        DdrtMr8Hsio.Bits.read_preamble = READ_PREAMBLE_1TCK;
        wrPreambleSetting = 0;
        dqsPulseMask = 0;
        break;
    }

  }


  for (SubCh = 0; SubCh < SUB_CH; SubCh++) {
    if (((1 << SubCh) & SubChMask) == 0) {
      continue;
    }
    DramModeRegWrite (Socket, Ch, 1 << SubCh, Dimm, Rank, DDR5_MODE_REGISTER_8_REG, (UINT8)DdrtMr8Hsio.Data);

    for (strobe = 0; strobe < maxStrobe; strobe++) {

      DdrDataControl3.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRDATA_CR_DDRCRDATACONTROL3_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG + ddrPhyDdrDataOffset[strobe + (SubCh * maxStrobe)]);
      DdrDataControl3.Bits.wrpreamble = wrPreambleSetting;
      WriteFnvCfg(Host, Socket, Ch, Dimm, DDRDATA_CR_DDRCRDATACONTROL3_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG + ddrPhyDdrDataOffset[strobe + (SubCh * maxStrobe)], DdrDataControl3.Data);

      DdrCrCmdBusTrain.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRDATA_CR_DDRCRCMDBUSTRAIN_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG + ddrPhyDdrDataOffset[strobe + (SubCh * maxStrobe)]);
      DdrCrCmdBusTrain.Bits.ddrcrmaskcntpulsenumstart = dqsPulseMask;
      WriteFnvCfg(Host, Socket, Ch, Dimm, DDRDATA_CR_DDRCRCMDBUSTRAIN_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG + ddrPhyDdrDataOffset[strobe + (SubCh * maxStrobe)], DdrCrCmdBusTrain.Data);

      DdrCrDataControl0.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRDATA_CR_DDRCRDATACONTROL0_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG + ddrPhyDdrDataOffset[strobe + (SubCh * maxStrobe)]);
      DdrCrDataControl0.Bits.endqsodtparkmode = parkMode;
      WriteFnvCfg(Host, Socket, Ch, Dimm, DDRDATA_CR_DDRCRDATACONTROL0_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG + ddrPhyDdrDataOffset[strobe + (SubCh * maxStrobe)], DdrCrDataControl0.Data);

    }
  } // SubCh

} // SetFMCReceiveEnableModeDdrt2

/**
Program DDRT2 Dram TxDqDelay

@param[in] Host -  Pointer to sysHost
@param[in] Dimm - Dimm number(zero - based)
@param[in] Value - value for ReadDqsOffset

@retval N / A
**/
VOID
ProgramDramTxDqDelayDdrt2 (
  IN   PSYSHOST  Host,
  IN   UINT8     Dimm,
  IN   UINT8     Value
  )
{
  UINT8               Socket;
  UINT8               Ch;
  UINT8               MaxChDdr;
  UINT16              MmrcValue;
  UINT8               CsrTableIndex;
  STATIC UINT32       TxControl0Rank0x[] = { DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG,
                                             DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH1_REG,
                                             DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT1_CH0_REG,
                                             DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT1_CH1_REG,
                                             DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT2_CH0_REG,
                                             DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT2_CH1_REG,
                                             DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT3_CH0_REG,
                                             DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT3_CH1_REG,
                                             DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT4_CH0_REG,
                                             DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT4_CH1_REG,
                                             DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT5_CH0_REG,
                                             DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT5_CH1_REG
                                           };

  DDRDATA_CR_TXCONTROL0RANK0_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_STRUCT         DdrDataTxControl0Rank0;

  Socket = Host->var.mem.currentSocket;
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
    "ProgramDramTxDqDelayDdrt2 start\n");

  MaxChDdr = GetMaxChDdr ();
  MmrcValue = MMRC_DEFAULT_VALUES;
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }

    if (IsDcpmmPresentDimm (Socket, Ch, Dimm)) {
      for (CsrTableIndex = 0; CsrTableIndex < (sizeof (TxControl0Rank0x) / sizeof (UINT32)); CsrTableIndex++) {
        DdrDataTxControl0Rank0.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, TxControl0Rank0x[CsrTableIndex]);
        DdrDataTxControl0Rank0.Bits.txdqdelay = (UINT32) (MmrcValue + (Value * 128));
        WriteFnvCfg(Host, Socket, Ch, Dimm, TxControl0Rank0x[CsrTableIndex], DdrDataTxControl0Rank0.Data);
      }
    }
  }

  return;
}

/*
Enables write leveling in CWV IO

@param[in] Host - Pointer to sysHost
@param[in] Socket - Socket Id
@param[in] Ch - Ch number
@param[in] Dimm - Dimm number(zero - based)
@param[in] Value - value for cwvwriteleven

@retval N / A
*/
VOID
ProgramCwvWriteLevel(
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Value
)
{
  UINT8     SubChannel;
  UINT8     Strobe;
  UINT8     MaxStrobe = 5;
  DDRDATA_CR_DDRCRDATACONTROLBYPASS_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_STRUCT Datacontrolbypass;

  for (SubChannel = 0; SubChannel < SUB_CH; SubChannel++) {
    for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
      Datacontrolbypass.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRDATA_CR_DDRCRDATACONTROLBYPASS_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG + ddrPhyDdrDataOffset[Strobe + (SubChannel * MaxStrobe)]);
      Datacontrolbypass.Bits.cwvwriteleven = Value;
      WriteFnvCfg(Host, Socket, Ch, Dimm, DDRDATA_CR_DDRCRDATACONTROLBYPASS_0_0_0_MCHBAR_CWV_DDRIO_DAT0_CH0_REG + ddrPhyDdrDataOffset[Strobe + (SubChannel * MaxStrobe)], Datacontrolbypass.Data);
    }
  }
}

/*
  Programs Dfe Bias Gain in the CWV IO

  @param[in] Host - Pointer to sysHost
  @param[in] Socket - Socket Id
  @param[in] Ch - Ch number
  @param[in] Dimm - Dimm number(zero - based)
  @param[in] Value - Bias gain

  @retval N / A
*/

VOID
ProgramCwvRxBiasGain(
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch,
  IN UINT8    Dimm,
  IN UINT8    Value
)
{
  UINT8 DfeGainBias[NUM_CWV_DCA_DFE_GAIN_BIAS] = {2, 6, 0xA, 0xE, 0x12, 0x16, 0x1A};

  if (Value >= NUM_CWV_DCA_DFE_GAIN_BIAS) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "The Input Value(%d) is out of range(%d)!\n", Value, NUM_CWV_DCA_DFE_GAIN_BIAS);
    return;
  }

  DDRCCC_CR_DDRCRCCCRXBIASANA_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_STRUCT SubCh0Bias;
  DDRCCC_CR_DDRCRCCCRXBIASANA_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_STRUCT SubCh1Bias;

  SubCh0Bias.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCRXBIASANA_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG);
  SubCh1Bias.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCRXBIASANA_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG);

  SubCh0Bias.Bits.rxgainctlbiasadj = DfeGainBias[Value];
  SubCh1Bias.Bits.rxgainctlbiasadj = DfeGainBias[Value];

  WriteFnvCfg (Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCRXBIASANA_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH2CCC_REG, SubCh0Bias.Data);
  WriteFnvCfg (Host, Socket, Ch, Dimm, DDRCCC_CR_DDRCRCCCRXBIASANA_0_0_0_MCHBAR_CWV_DDRIO_CMD_CH0CCC_REG, SubCh1Bias.Data);
}