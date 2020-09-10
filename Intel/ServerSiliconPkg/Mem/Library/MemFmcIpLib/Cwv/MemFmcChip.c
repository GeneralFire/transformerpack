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
#include <Chip/Include/FnvAccessCommon.h>
#include <Memory/CpgcDefinitions.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Library/MemCpgcIpLib.h>
#include <Library/MemMcIpLib.h>
#include "Include/MemFmcRegs.h"
#include <Library/EmulationConfigurationLib.h>
#include <Library/CteNetLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemDdrioIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/PcdLib.h>
#include <Memory/JedecDefinitions.h>
#include <MemProjectSpecific.h>
#include <Ppi/MemoryPolicyPpi.h>
#include "Include/MemFmcIpLibInternal.h"
#include <Library/MemRcLib.h>
#include <Memory/Ddr5RcwRegs.h>
#include <Library/SiliconWorkaroundPMemLib.h>
#include <Memory/Ddrt2SpdRegisters.h>

#define BWV_READ_CREDIT     0x30
#define EKV_READ_CREDIT     0x28
#define FIS_MAJOR_VERSION_1 0x1
#define FIS_MINOR_VERSION_6 0x6

#define  DQ_SWIZZLE_CLEARING_PATTERN 0xe4e4e4e4

#define DDRT_SUPPORTED_FREQUENCIES 7
#define DDRT_FREQUENCY_MAX_INDEX  (DDRT_SUPPORTED_FREQUENCIES - 1)
//                                                                    3200 3600 4000 4400 4800
// CWV: 1DPC, or 2DPC with RDIMM
UINT8 CwvDdrtCASLatencyRDIMM[DDRT_SUPPORTED_FREQUENCIES]            = { 32, 26,  28,  32,  40,  0,  0};
UINT8 CwvDdrtCASLatencyAdderRDIMM[DDRT_SUPPORTED_FREQUENCIES]       = { 0,   0,   0,   0,   0,  0,  0};
UINT8 CwvDdrtCASWriteLatencyRDIMM[DDRT_SUPPORTED_FREQUENCIES]       = { 30, 24,  26,  30,  38,  0,  0};
UINT8 CwvDdrtCASWriteLatencyAdderRDIMM[DDRT_SUPPORTED_FREQUENCIES]  = { 0,   0,   0,   0,   0,  0,  0};
// CWV: 2DPC with LRDIMM
UINT8 CwvDdrtCASLatencyLRDIMM[DDRT_SUPPORTED_FREQUENCIES]           = { 32, 26,  28,  32,  40,  0,  0};
UINT8 CwvDdrtCASLatencyAdderLRDIMM[DDRT_SUPPORTED_FREQUENCIES]      = { 0,   0,   0,   0,   0,  0,  0};
UINT8 CwvDdrtCASWriteLatencyLRDIMM[DDRT_SUPPORTED_FREQUENCIES]      = { 30, 24,  26,  30,  38,  0,  0};
UINT8 CwvDdrtCASWriteLatencyAdderLRDIMM[DDRT_SUPPORTED_FREQUENCIES] = { 0,   0,   0,   0,   0,  0,  0};
// Emulation
UINT8 CwvDdrtCASLatencyEmu[DDRT_SUPPORTED_FREQUENCIES]              = { 28, 26,  28,  32,  40,  0,  0};
UINT8 CwvDdrtCASWriteLatencyEmu[DDRT_SUPPORTED_FREQUENCIES]         = { 26, 24,  26,  30,  38,  0,  0};
UINT8 CwvDdrtCASLatencyAdderEmu[DDRT_SUPPORTED_FREQUENCIES]         = { 0,   0,   0,   0,   0,  0,  0};
UINT8 CwvDdrtCASWriteLatencyAdderEmu[DDRT_SUPPORTED_FREQUENCIES]    = { 0,   0,   0,   0,   0,  0,  0};

const SPD_AEP_MOD_REVISION_STRUCT MinSpdCwvModRevisions[] = {
  //  .Bits.module_type,   .Bits.module_revision
  { { SPD_CPS_MOD_REV,     0                   } }
};
const UINT8 MinSpdCwvModRevisionsSize = sizeof MinSpdCwvModRevisions / sizeof MinSpdCwvModRevisions[0];

//
// NVMDIMM size
//
//capacity by SPD byte 592 and 594
// 592[3:0] + 594[6:2]
// - 0x120: 128GB
// - 0x140: 256GB
// - 0x144: 512GB
// - 0x14C: 1TB
//
#define CWV_CAPACITYSPD128GB  0x120
#define CWV_CAPACITYSPD256GB  0x140
#define CWV_CAPACITYSPD512GB  0x144
#define CWV_CAPACITYSPD1TB    0x14C

const UINT16 CwvSizeIndex[] = {
  CWV_CAPACITYSPD128GB,
  CWV_CAPACITYSPD256GB,
  CWV_CAPACITYSPD512GB,
  CWV_CAPACITYSPD1TB
};

/** Checks if the NGN/DDRT Firmware Interface Spec version on a given DIMM is less than a particular value.

  @param[in] Host           Pointer to the system host (root) structure
  @param[in] Socket         The socket number where the DIMM resides
  @param[in] Channel        The channel number where the DIMM resides on the provided socket
  @param[in] Dimm           The DIMM number on the provided channel
  @param[in] MajorVersion   The major version of the FIS (from Major.Minor format, e.g. 1 in 1.6)
  @param[in] MinorVersion   The minor version of the FIS (from Major.Minor format, e.g. 6 in 1.6)

  @retval FALSE The DIMM's FIS is greater than or equal to the one determined by the input values
  @retval TRUE  The DIMM's FIS is less than the one determined by the input values

**/
BOOLEAN
CheckFisVersionLessThan (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     MajorVersion,
  IN UINT8     MinorVersion
  );

/**

  Checks if the Nvm is POR or not

  @param[in] NvmStepping  - Nvm Stepping
  @param[in] FmcModRev    - Nvm Module Revision and Type
  @param[in] NvmCapacity  - Nvm Capacity

  @retval FALSE The Nvm is not Supported
  @retval TRUE  The Nvm is Supported

**/
BOOLEAN
DdrtPorCheck (
  IN UINT8                        NvmStepping,
  IN SPD_AEP_MOD_REVISION_STRUCT  FmcModRev,
  IN UINT16                       NvmCapacity
  )
{
  UINT8    Index;
  BOOLEAN  NvmModRevSupport = FALSE;
  BOOLEAN  NvmCapSupport = FALSE;

  //
  // Check Module Revision Code
  //
  for (Index = 0; Index < MinSpdCwvModRevisionsSize; Index++) {
    if (FmcModRev.Bits.module_type == MinSpdCwvModRevisions[Index].Bits.module_type) {
      if (FmcModRev.Bits.module_revision < MinSpdCwvModRevisions[Index].Bits.module_revision) {
        // We've found the target module type, but the revision is too low.
        break;
      } else {
        // We've found the target module type and the revision is okay.
        NvmModRevSupport = TRUE;
        break;
      }
    } else if (Index >= MinSpdCwvModRevisionsSize - 1) {
      // We haven't found the target module type, despite being at the last index (or above--for paranoia's sake), so the type is invalid.
      break;
    }
  }

  //
  // Check Capacity
  //
  for (Index = 0; Index < ARRAY_SIZE (CwvSizeIndex); Index++) {
    if (NvmCapacity == CwvSizeIndex[Index]) {
      NvmCapSupport = TRUE;
      break;
    }
  }

  return (NvmModRevSupport | NvmCapSupport);
}

VOID
DisableScrambling (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{


  UINT8                                        Dimm;
  DDRT_HSIO_WR_SCR_CTRL_CWV_DDRIO_HSIO_STRUCT  TxSeedSel;
  DDRT_HSIO_RD_SCR_CTRL_CWV_DDRIO_HSIO_STRUCT  RxSeed;

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if (IsDimmPresent (socket, ch, Dimm) == FALSE) {
      continue;
    }
    if (IsDcpmmPresentDimm (socket, ch, Dimm) == FALSE) {
      continue;
    }
    TxSeedSel.Data = ReadFnvCfg (Host, socket, ch, Dimm, DDRT_HSIO_WR_SCR_CTRL_CWV_DDRIO_HSIO_REG);
    RxSeed.Data = ReadFnvCfg (Host, socket, ch, Dimm, DDRT_HSIO_RD_SCR_CTRL_CWV_DDRIO_HSIO_REG);

    TxSeedSel.Bits.lfsr_en = 0;
    RxSeed.Bits.lfsr_en    = 0;

    WriteFnvCfg (Host, socket, ch, Dimm, DDRT_HSIO_WR_SCR_CTRL_CWV_DDRIO_HSIO_REG, TxSeedSel.Data);
    WriteFnvCfg (Host, socket, ch, Dimm, DDRT_HSIO_RD_SCR_CTRL_CWV_DDRIO_HSIO_REG, RxSeed.Data);
  }

}

VOID
InvalidateDdrtPrefetchCache (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch
  )
{
  UINT8                                          Dimm;
  struct dimmNvram                               (*DimmNvList)[MAX_DIMM];
  DA_DDRT_TRAINING_OTHER_EN_CWV_DA_UNIT_0_STRUCT DdrtTrainingOtherEn;

  DimmNvList = GetDimmNvList(Host, Socket, Ch);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
      continue;
    }

    if ((*DimmNvList)[Dimm].fmcType == FMC_CWV_TYPE) {
      DdrtTrainingOtherEn.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_OTHER_EN_CWV_DA_UNIT_0_REG);
      DdrtTrainingOtherEn.Bits.sa_pche_inv_allsets = 1;
      DdrtTrainingOtherEn.Bits.sb_pche_inv_allsets = 1;
      WriteFnvCfg (Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_OTHER_EN_CWV_DA_UNIT_0_REG, DdrtTrainingOtherEn.Data);
    }
  }
}

/**
  Return DDRT CAS latencies (read, write, and adders) for a target DIMM.

  @param[in]  Socket                The socket containing the target DIMM.
  @param[in]  Channel               The channel containing the target DIMM.
  @param[in]  Dimm                  The target DIMM to get CAS latencies for.
  @param[out] CASLatency            An optional parameter to return the CAS (read) latency for the target DIMM.
  @param[out] CASLatencyAdder       An optional parameter to return the CAS (read) latency adder for the target DIMM.
  @param[out] CASWriteLatency       An optional parameter to return the CAS write latency for the target DIMM.
  @param[out] CASWriteLatencyAdder  An optional parameter to return the CAS write latency adder for the target DIMM.

  @retval   EFI_SUCCESS       CAS latencies were successfully determined for the target DIMM.
  @retval   EFI_UNSUPPORTED   CAS latencies could not be determined for the target DIMM's FMC type.
**/
EFI_STATUS
EFIAPI
GetDdrtCasLatencies (
  IN      UINT8    Socket,
  IN      UINT8    Channel,
  IN      UINT8    Dimm,
      OUT UINT8    *CASLatency,           OPTIONAL
      OUT UINT8    *CASLatencyAdder,      OPTIONAL
      OUT UINT8    *CASWriteLatency,      OPTIONAL
      OUT UINT8    *CASWriteLatencyAdder  OPTIONAL
  )
{
  UINT8             DdrtFrequency;
  struct dimmNvram  (*DimmNvList)[MAX_DIMM];
  UINT8             FmcType;
  PSYSHOST          Host;
  UINT8             OtherDimm = (BIT0 ^ Dimm);  // Assumes max 2DPC
  UINT8             RecycleBin;
  EFI_STATUS        Status = EFI_SUCCESS;

  Host = (PSYSHOST)GetSysHostPointer ();
  DdrtFrequency = Host->nvram.mem.socket[Socket].ddrtFreq;
  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  FmcType = GetFmcType (Socket, Channel);

  if (DdrtFrequency > DDRT_FREQUENCY_MAX_INDEX) {
    OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_SPEED_NOT_SUP, Socket, Channel, NO_DIMM, NO_RANK);
    DdrtFrequency = DDRT_FREQUENCY_MAX_INDEX;
  }

  // Redirect all unwanted values for optional parameters to a shared, temporary address.
  if (CASLatency == NULL) {
    CASLatency = &RecycleBin;
  }
  if (CASLatencyAdder == NULL) {
    CASLatencyAdder = &RecycleBin;
  }
  if (CASWriteLatency == NULL) {
    CASWriteLatency = &RecycleBin;
  }
  if (CASWriteLatencyAdder == NULL) {
    CASWriteLatencyAdder = &RecycleBin;
  }

  if (IsLrdimmPresent (Socket, Channel, OtherDimm)) {
    switch (FmcType) {
      default:
        OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_NVMDIMM_NOT_SUPPORTED, Socket, Channel, Dimm, NO_RANK);
        Status = EFI_UNSUPPORTED;
        // Fall through to CWV values as a backup if the warning is not promoted to an error.
      case FMC_CWV_TYPE:
        if (UbiosGenerationEnabled ()) {
          *CASLatency = CwvDdrtCASLatencyEmu[DdrtFrequency];
          *CASLatencyAdder = CwvDdrtCASLatencyAdderEmu[DdrtFrequency];
          *CASWriteLatency = CwvDdrtCASWriteLatencyEmu[DdrtFrequency];
          *CASWriteLatencyAdder = CwvDdrtCASWriteLatencyAdderEmu[DdrtFrequency];
        } else {
          *CASLatency = CwvDdrtCASLatencyLRDIMM[DdrtFrequency];
          *CASLatencyAdder = CwvDdrtCASLatencyAdderLRDIMM[DdrtFrequency];
          *CASWriteLatency = CwvDdrtCASWriteLatencyLRDIMM[DdrtFrequency];
          *CASWriteLatencyAdder = CwvDdrtCASWriteLatencyAdderLRDIMM[DdrtFrequency];
        }
        break;
    } // switch (FmcType)
  } else { // 2 DPC with RDIMM or 1 DPC
    switch (FmcType) {
      default:
        OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_NVMDIMM_NOT_SUPPORTED, Socket, Channel, Dimm, NO_RANK);
        Status = EFI_UNSUPPORTED;
        // Fall through to CWV values as a backup if the warning is not promoted to an error.
      case FMC_CWV_TYPE:
        if (UbiosGenerationEnabled ()) {
          *CASLatency = CwvDdrtCASLatencyEmu[DdrtFrequency];
          *CASLatencyAdder = CwvDdrtCASLatencyAdderEmu[DdrtFrequency];
          *CASWriteLatency = CwvDdrtCASWriteLatencyEmu[DdrtFrequency];
          *CASWriteLatencyAdder = CwvDdrtCASWriteLatencyAdderEmu[DdrtFrequency];
        } else {
          *CASLatency = CwvDdrtCASLatencyRDIMM[DdrtFrequency];
          *CASLatencyAdder = CwvDdrtCASLatencyAdderRDIMM[DdrtFrequency];
          *CASWriteLatency = CwvDdrtCASWriteLatencyRDIMM[DdrtFrequency];
          *CASWriteLatencyAdder = CwvDdrtCASWriteLatencyAdderRDIMM[DdrtFrequency];
        }
        break;
    } // switch (FmcType)
  } // if (IsLrdimmPresent (Socket, Channel, OtherDimm)) ... else

  *CASWriteLatencyAdder += CalculateDdrtCwlAddPxcAdder (Socket);

  return Status;
}

/**
  Program the Parity Flop En.

  @param[in]  Socket                The socket containing the target DIMM.
  @param[in]  Channel               The channel containing the target DIMM.

  @retval   none
**/

VOID
ProgramtCLtCWLAdd (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  UINT8                          dimm;
  struct dimmNvram               (*dimmNvList)[MAX_DIMM];
  DDRT_HSIO_DDRIO_CMD_EN_CWV_DDRIO_HSIO_STRUCT  HsioDdrioCmdEn;

  dimmNvList = GetDimmNvList(Host, socket, ch);
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
      continue;
    }

    HsioDdrioCmdEn.Data = ReadFnvCfg(Host, socket, ch, dimm, DDRT_HSIO_DDRIO_CMD_EN_CWV_DDRIO_HSIO_REG);

    if (IsPmemSiliconWorkaroundEnabled(Host, socket, ch, dimm, "S14011274250") && (Host->nvram.mem.socket[socket].ddrFreq > DDR_3600)) {
      HsioDdrioCmdEn.Bits.ddrtio_par_flopen = 1;
    } else {
      HsioDdrioCmdEn.Bits.ddrtio_par_flopen = 0;
    }

    WriteFnvCfg(Host, socket, ch, dimm, DDRT_HSIO_DDRIO_CMD_EN_CWV_DDRIO_HSIO_REG, HsioDdrioCmdEn.Data);

    } //dimm
}

/**
  Puts the BGF into reset. Prevents the Core from decoding a misaligned command.

  @param[in]  Socket                The socket containing the target DIMM.
  @param[in]  Channel               The channel containing the target DIMM.
  @param[in]  Dimm                  The target DIMM
  @param[in]  Value                 Value to put BFG in reset or not

  @retval   None
**/

VOID
BgfReset (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm,
  IN  UINT8     Value
)
{
  DDRT_PLL_INIT_CTRL_CWV_DFX_MISC_0_STRUCT DdrtPllInitCtrl;

  DdrtPllInitCtrl.Data = ReadFnvCfg(Host, Socket, Channel, Dimm, DDRT_PLL_INIT_CTRL_CWV_DFX_MISC_0_REG);
  DdrtPllInitCtrl.Bits.bgf_reset = Value;
  WriteFnvCfg(Host, Socket, Channel, Dimm, DDRT_PLL_INIT_CTRL_CWV_DFX_MISC_0_REG, DdrtPllInitCtrl.Data);

  CteDelayDclk(20);
  FixedDelayMicroSecond (20);
}

VOID
ProgramDdrtGnt2Erid (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  UINT8                                dimm;
  UINT8                                OldValue = 0;
  struct dimmNvram                     (*dimmNvList)[MAX_DIMM];
//CWV_TODO  DA_DDRT_LATENCY_FNV_DA_UNIT_0_STRUCT ddrtLatency;
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();

  if (UbiosGenerationEnabled ()) {
    if (Setup->common.ddrtXactor == 0){
      OldValue = GetUbiosOutputMode ();
      SetUbiosOutputMode (ASM_OUTPUT_ENABLE_ON_SMBUS);
    }
  }

  dimmNvList = GetDimmNvList(Host, socket, ch);
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
      continue;
    }
//CWV_TODO    ddrtLatency.Data = ReadFnvCfg(Host, socket, ch, dimm, DA_DDRT_LATENCY_FNV_DA_UNIT_0_REG);
//CWV_TODO    ddrtLatency.Bits.tcl_gnt_erid = (*dimmNvList)[dimm].DimmGnt2Erid;
//CWV_TODO    WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_LATENCY_FNV_DA_UNIT_0_REG, ddrtLatency.Data);
  }

  if (UbiosGenerationOrHsleEnabled ()) {
    if (Setup->common.ddrtXactor == 0) {
      SetUbiosOutputMode (OldValue);
    }
  }
}


/**
  Performs GetSetFnvIO operations to prepare to switch to normal mode

  @param Host    - Pointer to sysHost
  @param socket  - Socket to switch

**/
VOID
SwitchToNormalModeFmc1GetSetFNV (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  struct channelNvram     (*channelNvList)[MAX_CH];
  UINT8                   ch;
  UINT8                   dimm;
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];
/**
CWV_TODO
  DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_STRUCT  ddrCrDimmCmdControl1; // NVMCTLR CMD
  DDRCRCOMPCTL2_CH_FNV_DDRIO_COMP_STRUCT        ddrCompCtl2Fnv;
  DATACONTROL3N0_0_FNV_DDRIO_COMP_STRUCT        dataControl3CompFnv;      // NVMCTLR data
  DDRCRCLKCONTROLS_CH_FNV_DDRIO_DAT7_CH_STRUCT  DdrCrDimmClkControl1;
  UINT8                   strobe;
  UINT8                   MSVx4 = GetMaxStrobeValid(Host);
**/
  UINT8                  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList (Host, Socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    // NVMDIMM section
    dimmNvList = GetDimmNvList(Host, Socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }
/**
CWV_TODO
      // cmd
      ddrCrDimmCmdControl1.Data = Host->nvram.mem.socket[Socket].channelList[ch].dimmVrefControlFnv1;
      ddrCrDimmCmdControl1.Bits.lvlshft_holden = 0;

      GetSetFnvIO(Host, Socket, ch, dimm, DDRCRCMDCONTROLS_NA_FNV_DDRIO_DAT7_CH_REG, &ddrCrDimmCmdControl1.Data, 0, FNV_IO_WRITE, 0xF);
      GetSetFnvIO(Host, Socket, ch, dimm, DDRCRCMDCONTROLS_NB_FNV_DDRIO_DAT7_CH_REG, &ddrCrDimmCmdControl1.Data, 0, FNV_IO_WRITE, 0xF);
      GetSetFnvIO(Host, Socket, ch, dimm, DDRCRCMDCONTROLS_SB_FNV_DDRIO_DAT7_CH_REG, &ddrCrDimmCmdControl1.Data, 0, FNV_IO_WRITE, 0xF);
      //
      // EKV specific register
      //
      if ((*dimmNvList)[dimm].fmcType == FMC_EKV_TYPE) {
        GetSetFnvIO (Host, Socket, ch, dimm, DDRCRCLKCONTROLS_CH_FNV_DDRIO_DAT7_CH_REG, &DdrCrDimmClkControl1.Data, 0, FNV_IO_READ, 0xF);
        DdrCrDimmClkControl1.Bits.lvlshft_holden = 0;
        GetSetFnvIO (Host, Socket, ch, dimm, DDRCRCLKCONTROLS_CH_FNV_DDRIO_DAT7_CH_REG, &DdrCrDimmClkControl1.Data, 0, FNV_IO_WRITE, 0xF);
      }

      // comp
      GetSetFnvIO (Host, Socket, ch, dimm, DDRCRCOMPCTL2_CH_FNV_DDRIO_COMP_REG, &ddrCompCtl2Fnv.Data, 0, FNV_IO_READ, 0xF);
      ddrCompCtl2Fnv.Bits.lvlshft_holden = 0;
      GetSetFnvIO(Host, Socket, ch, dimm, DDRCRCOMPCTL2_CH_FNV_DDRIO_COMP_REG, &ddrCompCtl2Fnv.Data, 0, FNV_IO_WRITE, 0xF);

      for (strobe = 0; strobe < MSVx4; strobe++) {
        GetSetFnvIO (Host, Socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL3N0_0_FNV_DDRIO_COMP_REG), &dataControl3CompFnv.Data, 0, FNV_IO_READ, 0xF);
        dataControl3CompFnv.Bits.lvlshft_holden = 0;
        GetSetFnvIO (Host, Socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL3N0_0_FNV_DDRIO_COMP_REG), &dataControl3CompFnv.Data, 0, FNV_IO_WRITE, 0xF);
      } // strobe
      IODdrtResetPerCh (Host, Socket, ch, dimm);
**/
    } // dimm
  } // ch

  return;
}

/**
  Writes credit load as part of switching to normal mode

  @param Host    - Pointer to sysHost
  @param socket  - Socket to switch
**/
VOID
SwitchToNormalModeFmc1WriteCredits (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  struct channelNvram     (*channelNvList)[MAX_CH];
  UINT8                   ch;
  UINT8                   dimm;
  struct dimmNvram        (*dimmNvList)[MAX_DIMM];
  DA_ECC_ENABLE_CWV_DA_UNIT_0_STRUCT            daEccEnable;
//CWV_TODO  DN_ECC_ENABLE_FNV_D_UNIT_0_STRUCT             dnEccEnable;
  FNV_DEBUG_LOCK_CWV_DFX_MISC_0_STRUCT          fnvDebugLock;
  DRDP_WCRD_CONFIG_CWV_D_UNIT_0_STRUCT          drdpWcrdConfig;
//CWV_TODO  DA_WRITE_CREDIT_FNV_DA_UNIT_0_STRUCT          daWriteCredit;
  D_READ_CREDIT_CWV_D_UNIT_0_STRUCT             dReadCredit;
  DN_EMASK_CWV_D_UNIT_0_STRUCT                  dnEmask;
//CWV_TODO  DA_EMASK_FNV_DA_UNIT_0_STRUCT                 daEmask;
  D_FUNC_DEFEATURE1_CWV_D_UNIT_0_STRUCT         dFuncDefeature1;
  BOOLEAN                                       NgnDebugLock = TRUE;
  SYS_SETUP                                     *Setup;
  UINT8                                         MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList (Host, Socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList(Host, Socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ( (*dimmNvList)[dimm].dimmPresent == 0 ) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }
      //
      // Enable ECC on NVMCTLR
      // Program Write and Read Credits for NVMCTLR
      //
      daEccEnable.Data     = ReadFnvCfg (Host, Socket, ch, dimm, DA_ECC_ENABLE_CWV_DA_UNIT_0_REG);
//CWV_TODO      dnEccEnable.Data     = ReadFnvCfg (Host, Socket, ch, dimm, DN_ECC_ENABLE_FNV_D_UNIT_0_REG);
      drdpWcrdConfig.Data  = ReadFnvCfg (Host, Socket, ch, dimm, DRDP_WCRD_CONFIG_CWV_D_UNIT_0_REG);
//CWV_TODO      daWriteCredit.Data   = ReadFnvCfg (Host, Socket, ch, dimm, DA_WRITE_CREDIT_FNV_DA_UNIT_0_REG);
      dReadCredit.Data     = ReadFnvCfg (Host, Socket, ch, dimm, D_READ_CREDIT_CWV_D_UNIT_0_REG);

      if (Setup->mem.dfxMemSetup.dfxOptions & NGN_ECC_CORR) {
        daEccEnable.Bits.ecc_corr_en = 1;
      } else{
        daEccEnable.Bits.ecc_corr_en = 0;
      }

      if (Setup->mem.dfxMemSetup.dfxOptions & NGN_ECC_WR_CHK) {
        daEccEnable.Bits.ecc_wr_chk_en = 1;
      } else {
        daEccEnable.Bits.ecc_wr_chk_en = 0;
      }

      if (Setup->mem.dfxMemSetup.dfxOptions & NGN_ECC_EXIT_CORR) {
        daEccEnable.Bits.errflow_a_exitcorr = 1;
      } else {
        daEccEnable.Bits.errflow_a_exitcorr = 0;
      }

//CWV_TODO      if (Setup->mem.optionsNgn & NGN_ECC_RD_CHK) {
//CWV_TODO        daEccEnable.Bits.ecc_rd_chk_en = 1;
//CWV_TODO      } else {
//CWV_TODO        daEccEnable.Bits.ecc_rd_chk_en = 0;
//CWV_TODO      }

      WriteFnvCfg (Host, Socket, ch, dimm, DA_ECC_ENABLE_CWV_DA_UNIT_0_REG, daEccEnable.Data);
//CWV_TODO      WriteFnvCfg (Host, Socket, ch, dimm, DN_ECC_ENABLE_FNV_D_UNIT_0_REG, dnEccEnable.Data);

      //
      // Reset Write Credit
      //
      drdpWcrdConfig.Bits.wcrd_cfg = 0;
      WriteFnvCfg (Host, Socket, ch, dimm, DRDP_WCRD_CONFIG_CWV_D_UNIT_0_REG, drdpWcrdConfig.Data);

      //
      // Trigger Write Credit Load by clearing and then setting D_FUNC_DEAFEATURE1.wcrd_init
      //
      dFuncDefeature1.Data = 0;
      GetSetProtectedDcpmmRegister (Host, Socket, ch, dimm, GSM_FORCE_WRITE, D_FUNC_DEFEATURE1_CWV_D_UNIT_0_REG,
                                    PROT_REG_MASK_WCRD_INIT, &dFuncDefeature1.Data);

      dFuncDefeature1.Bits.wcrd_init = 1;
      GetSetProtectedDcpmmRegister (Host, Socket, ch, dimm, GSM_FORCE_WRITE, D_FUNC_DEFEATURE1_CWV_D_UNIT_0_REG,
                                    PROT_REG_MASK_WCRD_INIT, &dFuncDefeature1.Data);

      //
      // Program Write Credits and Read Credits
      //
      drdpWcrdConfig.Bits.wcrd_cfg = (*channelNvList)[ch].FmcWrCreditLimit;
//CWV_TODO      daWriteCredit.Bits.val = (*channelNvList)[ch].FmcWrCreditLimit;
      dReadCredit.Bits.val = (*channelNvList)[ch].FmcRdCreditLimit;
      WriteFnvCfg (Host, Socket, ch, dimm, DRDP_WCRD_CONFIG_CWV_D_UNIT_0_REG, drdpWcrdConfig.Data);
//CWV_TODO      WriteFnvCfg (Host, Socket, ch, dimm, DA_WRITE_CREDIT_FNV_DA_UNIT_0_REG, daWriteCredit.Data);
      WriteFnvCfg (Host, Socket, ch, dimm, D_READ_CREDIT_CWV_D_UNIT_0_REG, dReadCredit.Data);

      //
      // Unmask Write Credit Check
      //
      if (CheckFisVersionLessThan (Host, Socket, ch, dimm, FIS_MAJOR_VERSION_1, FIS_MINOR_VERSION_6)) {
//CWV_TODO        daEmask.Data = ReadFnvCfg (Host, Socket, ch, dimm, DA_EMASK_FNV_DA_UNIT_0_REG);
        dnEmask.Data = ReadFnvCfg (Host, Socket, ch, dimm, DN_EMASK_CWV_D_UNIT_0_REG);
        dnEmask.Bits.wr_credit = 0;
//CWV_TODO        daEmask.Bits.wr_credit = 0;
        WriteFnvCfg (Host, Socket, ch, dimm, DN_EMASK_CWV_D_UNIT_0_REG, dnEmask.Data);
//CWV_TODO        WriteFnvCfg (Host, Socket, ch, dimm, DA_EMASK_FNV_DA_UNIT_0_REG, daEmask.Data);
      }
    }
  }

  if (!(Setup->mem.optionsNgn & NGN_DEBUG_LOCK)) {
    NgnDebugLock = FALSE;
  }
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, Socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ( (*dimmNvList)[dimm].dimmPresent == 0 ) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }
      if (NgnDebugLock == TRUE) {
        fnvDebugLock.Data = ReadFnvCfg(Host, Socket, ch, dimm, FNV_DEBUG_LOCK_CWV_DFX_MISC_0_REG);
        fnvDebugLock.Bits.dbg_en = 1;     // Write 1 to enable
        WriteFnvCfg (Host, Socket, ch, dimm, FNV_DEBUG_LOCK_CWV_DFX_MISC_0_REG, fnvDebugLock.Data);
        fnvDebugLock.Data = ReadFnvCfg(Host, Socket, ch, dimm, FNV_DEBUG_LOCK_CWV_DFX_MISC_0_REG);
        if (fnvDebugLock.Bits.dwe == 1) { // Check debug enabled by HW
          RcDebugPrint (SDBG_DEFAULT, "\n: NGN unlocked for debug S%d.CH%d.D%d:\n", Socket, ch, dimm);
        }
        fnvDebugLock.Bits.lock = 1;       // // Lock the register
        WriteFnvCfg (Host, Socket, ch, dimm, FNV_DEBUG_LOCK_CWV_DFX_MISC_0_REG, fnvDebugLock.Data);
      }
    }
  }

  return;
}

/**

  Switches to normal mode

  @param Host    - Pointer to sysHost
  @param socket  - Socket to switch

**/
VOID
SwitchToNormalModeFmc1 (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  if ((FeaturePcdGet (PcdCteBuild) == FALSE) && (UbiosGenerationOrHsleEnabled () == FALSE)) {
    SwitchToNormalModeFmc1GetSetFNV (Host, Socket);
  }

  if ((FeaturePcdGet (PcdCteBuild) == FALSE) || (FeaturePcdGet (PcdCosimBuild) == TRUE)) {
    SwitchToNormalModeFmc1WriteCredits (Host, Socket);
  }

  return;
} //SwitchToNormalModeFmc1


/**

  Reads MPR page/location from given rank and updates associated MprData per DRAM device

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param MprData - each byte is updated with UI[0-7] data per DRAM device

**/
VOID
ReadBcwDimmFmc (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT8     bcwFunc,
  UINT8     bcwByte,
  UINT8     bcwNibble,
  UINT8     MprData[MAX_STROBE]
  )
{
/**
CWV_TODO
  DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_STRUCT daDdrtTrainingRc06;
  DA_DDRT_TRAINING_RC4X_FNV_DA_UNIT_0_STRUCT daDdrtTrainingRc4x;
  DA_DDRT_TRAINING_RC6X_FNV_DA_UNIT_0_STRUCT daDdrtTrainingRc6x;
**/
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
//CWV_TODO  DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_STRUCT daDdrtDqBcomCtrl;

  dimmNvList    = GetDimmNvList(Host, socket, ch);
/**
CWV_TODO
    //
    // Enable Far Memory Controller to automatically respond with REQ# on read command
    //
    daDdrtDqBcomCtrl.Data = ReadFnvCfg(Host, socket, ch, dimm, DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_REG);
    if ((*dimmNvList)[dimm].fmcType == FMC_FNV_TYPE) {
      daDdrtDqBcomCtrl.Data |= BIT12;
    } else {
      daDdrtDqBcomCtrl.Bits.bcw_mpr_rd_req_en = 1;
    }
    WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_REG, daDdrtDqBcomCtrl.Data);

    daDdrtTrainingRc4x.Data = ReadFnvCfg(Host, socket, ch, dimm, DA_DDRT_TRAINING_RC4X_FNV_DA_UNIT_0_REG);
    daDdrtTrainingRc4x.Bits.ad_12_8 = BIT4 | bcwByte;
    daDdrtTrainingRc4x.Bits.func = bcwFunc;
    WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_TRAINING_RC4X_FNV_DA_UNIT_0_REG, daDdrtTrainingRc4x.Data);

    daDdrtTrainingRc6x.Data = ReadFnvCfg(Host, socket, ch, dimm, DA_DDRT_TRAINING_RC6X_FNV_DA_UNIT_0_REG);
    daDdrtTrainingRc6x.Bits.ad_7_0 = bcwNibble << 4;
    WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_TRAINING_RC6X_FNV_DA_UNIT_0_REG, daDdrtTrainingRc6x.Data);

    daDdrtTrainingRc06.Data = ReadFnvCfg(Host, socket, ch, dimm, DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_REG);
    daDdrtTrainingRc06.Bits.misc_ctrl = 4;
    WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_REG, daDdrtTrainingRc06.Data);

    //
    // Read the data
    //
    FnvBcwRead (Host, socket, ch, dimm, 0, MprData);

    //
    // Restore FNV settings
    //
    daDdrtTrainingRc06.Bits.misc_ctrl = 0;
    WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_REG, daDdrtTrainingRc06.Data);
    if ((*dimmNvList)[dimm].fmcType == FMC_FNV_TYPE) {
      daDdrtDqBcomCtrl.Data &= ~BIT12;
    } else {
      daDdrtDqBcomCtrl.Bits.bcw_mpr_rd_req_en = 0;
    }
    WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_REG, daDdrtDqBcomCtrl.Data);
**/

} // ReadBcwRankFmc

VOID
DisableChipFNV(
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch,
  PCHIP_ERLY_CMD_CK_STRUCT chipErlyCmdCKStruct
  )
{
  UINT8                               dimm = 0;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  struct dimmNvram                    (*dimmNvList)[MAX_DIMM];
  DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_STRUCT  daTrainingEn;
  D_DDRT_TRAINING_EN_CWV_D_UNIT_0_STRUCT    dTrainingEn;
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  dTrainingEn.Data = 0;
  daTrainingEn.Data = 0;
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }

      WriteFnvCfgSmb(Host, socket, ch, dimm, DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_REG, daTrainingEn.Data);  //CWV NEED CHECK
      WriteFnvCfgSmb(Host, socket, ch, dimm, D_DDRT_TRAINING_EN_CWV_D_UNIT_0_REG, dTrainingEn.Data);
    } // dimm loop
  } // ch loop
}

VOID
WriteRCFnv(
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch,
  UINT8    dimm,
  UINT8    controlWordData,
  UINT8    controlWordAddr
  )
{
  BUS_TYPE  OriginalFmcAccessMode;

  OriginalFmcAccessMode = GetFmcAccessMode (socket, ch);
  SetFmcAccessMode (socket, ch, SMBUS);

  switch (controlWordAddr) {
    case RDIMM_RC0E:
//CWV_TODO      WriteFnvCfg (Host, socket, ch, dimm, DA_DDRT_TRAINING_RC0E_FNV_DA_UNIT_0_REG, controlWordData);
      break;
    default:
      break;
  }

  SetFmcAccessMode (socket, ch, OriginalFmcAccessMode);
}

/**

Clears DqSwizzle Register in the AEP DIMM

@param Host - Pointer to sysHost, the system Host(root) structure
@param socket - CPU Socket Node number(Socket ID)

@retval None

**/
VOID
ClearDqSwizzleAep (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  UINT8                            Ch;
  UINT8                            Dimm;
  UINT32                           RxDqSwz[5];
  UINT32                           TxDqSwz[5];
  UINT8                            i;
  struct channelNvram              (*ChannelNvList)[MAX_CH];
  struct dimmNvram                 (*DimmNvList)[MAX_DIMM];
  UINT8                            MaxChDdr;

  RcDebugPrintWithDevice(SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Clear Dq Swizzle PMEM -- START\n");
  ChannelNvList = GetChannelNvList(Host, Socket);
  MaxChDdr      = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      DimmNvList = GetDimmNvList(Host, Socket, Ch);
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }
      for (i = 0; i < 5; i++) {
        TxDqSwz[i] = DQ_SWIZZLE_CLEARING_PATTERN;
        RxDqSwz[i] = DQ_SWIZZLE_CLEARING_PATTERN;
      }
      TxDqSwz[4] &= 0x0000FFFF;
      RxDqSwz[4] &= 0x0000FFFF;

#ifndef HW_EMULATION
      if (IsMemFlowEnabled (DqSwizzlingDiscovery) == TRUE) {
/**
CWV_TODO
        // Clear swizzle NVMCTLR Registers
        WriteFnvCfg(Host, Socket, Ch, Dimm, (DA_DDRT_TX_DQ_SWZ0_FNV_DA_UNIT_0_REG), TxDqSwz[0]);
        WriteFnvCfg(Host, Socket, Ch, Dimm, (DA_DDRT_RX_DQ_SWZ0_FNV_DA_UNIT_0_REG), RxDqSwz[0]);
        WriteFnvCfg(Host, Socket, Ch, Dimm, (DA_DDRT_TX_DQ_SWZ1_FNV_DA_UNIT_0_REG), TxDqSwz[1]);
        WriteFnvCfg(Host, Socket, Ch, Dimm, (DA_DDRT_RX_DQ_SWZ1_FNV_DA_UNIT_0_REG), RxDqSwz[1]);
        WriteFnvCfg(Host, Socket, Ch, Dimm, (DA_DDRT_TX_DQ_SWZ2_FNV_DA_UNIT_0_REG), TxDqSwz[2]);
        WriteFnvCfg(Host, Socket, Ch, Dimm, (DA_DDRT_RX_DQ_SWZ2_FNV_DA_UNIT_0_REG), RxDqSwz[2]);
        WriteFnvCfg(Host, Socket, Ch, Dimm, (DA_DDRT_TX_DQ_SWZ3_FNV_DA_UNIT_0_REG), TxDqSwz[3]);
        WriteFnvCfg(Host, Socket, Ch, Dimm, (DA_DDRT_RX_DQ_SWZ3_FNV_DA_UNIT_0_REG), RxDqSwz[3]);
        WriteFnvCfg(Host, Socket, Ch, Dimm, (DA_DDRT_TX_DQ_SWZ4_FNV_DA_UNIT_0_REG), TxDqSwz[4]);
        WriteFnvCfg(Host, Socket, Ch, Dimm, (DA_DDRT_RX_DQ_SWZ4_FNV_DA_UNIT_0_REG), RxDqSwz[4]);
**/
      }
#endif //HW_EMULATION

    } // Dimm loop
  } // Ch loop
  RcDebugPrintWithDevice(SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Clear Dq Swizzle PMEM -- END\n");
}

/**

  Routine Description: Enables/Disables Training Mode for NVMCTLR

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param mode    - training mode

  @retval SUCCESS
**/
UINT32
SetAepTrainingMode (
                 PSYSHOST Host,
                 UINT8    socket,
                 UINT8    mode
                 )
{
  UINT8                                       ch;
  UINT8                                       dimm;
  BOOLEAN                                     ddrtEnabled = FALSE;
  struct channelNvram                         (*channelNvList)[MAX_CH];
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];
  DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_STRUCT    daTrainingEn;
  D_DDRT_TRAINING_EN_CWV_D_UNIT_0_STRUCT      dTrainingEn;
  DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_STRUCT hsioTrainingEn;
  UINT8                                       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if ((*channelNvList)[ch].ddrtEnabled) {
      ddrtEnabled = TRUE;
      break;
    }
  }
  if (ddrtEnabled == FALSE) {
    return SUCCESS;
  }
  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "PMEM training mode = %d\n", mode);

  daTrainingEn.Data   = 0;
  dTrainingEn.Data    = 0;
  hsioTrainingEn.Data = 0;

  daTrainingEn.Bits.tm_en   = 1;
  dTrainingEn.Bits.tm_en    = 1;
  hsioTrainingEn.Bits.tm_en = 1;

  switch (mode){
    case DISABLE_TRAINING_MODE:
      // Exit Training Mode
      daTrainingEn.Data = 0;
      dTrainingEn.Data  = 0;
      hsioTrainingEn.Data = 0;
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, " Exit Training Mode\n");
      break;
    case CHECKPOINT_MINOR_EARLY_CMD_CLK:
      daTrainingEn.Bits.early_cmd_ck_trn   = 1;
      hsioTrainingEn.Bits.early_cmd_ck_trn = 1;
      break;
    case CHECKPOINT_MINOR_CMD_VREF_CENTERING:
      daTrainingEn.Bits.adv_cmd_ck_trn   = 1;
      dTrainingEn.Bits.adv_cmd_ck_trn    = 1;
      hsioTrainingEn.Bits.adv_cmd_ck_trn = 1;
      break;
    case CHECKPOINT_MINOR_LATE_CMD_CLK:
      daTrainingEn.Bits.adv_cmd_ck_trn   = 1;
      dTrainingEn.Bits.adv_cmd_ck_trn    = 1;
      hsioTrainingEn.Bits.adv_cmd_ck_trn = 1;
      break;
#ifdef LRDIMM_SUPPORT
    case CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING:
      hsioTrainingEn.Bits.recen_dq_dqs_mpr_trn = 1;
      break;
#endif
    case CHECKPOINT_MINOR_REQ_CLK:
      hsioTrainingEn.Bits.basic_req_trn = 1;
      break;
    case DISABLE_TRAINING_STEP:
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, " Training Step Disabled\n");
      break;
    case ENABLE_TRAINING_MODE:
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT," Training Mode Enabled\n");
      break;
    default:
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT," Training Step Not Supported\n");
      break;
  }


  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }

      WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_REG, daTrainingEn.Data);
      WriteFnvCfg(Host, socket, ch, dimm, D_DDRT_TRAINING_EN_CWV_D_UNIT_0_REG, dTrainingEn.Data);
      WriteFnvCfg(Host, socket, ch, dimm, DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_REG, hsioTrainingEn.Data);
    }
  }

  return SUCCESS;
}


/**

  Routine Description: Clears Prefetch Cache miss for NVMCTLR

  @param Host    - Pointer to sysHost
  @param socket  - Socket number

  @retval SUCCESS
**/
UINT32
ClrPcheMiss (
                 PSYSHOST Host,
                 UINT8    socket
                 )
{

  return SUCCESS;
}


/**

  Routine Description: Enables/Disables Training Mode for NVMCTLR, SMBUS version

  @param Host    - Pointer to sysHost
  @param socket  - Socket number
  @param mode    - training mode

  @retval SUCCESS
**/
UINT32
SetAepTrainingModeSMB (
                 PSYSHOST Host,
                 UINT8    socket,
                 UINT8    mode
                 )
{
  UINT8                                       ch;
  UINT8                                       dimm;
  BOOLEAN                                     ddrtEnabled = FALSE;
  struct channelNvram                         (*channelNvList)[MAX_CH];
  struct dimmNvram                            (*dimmNvList)[MAX_DIMM];
  DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_STRUCT    daTrainingEn;
  D_DDRT_TRAINING_EN_CWV_D_UNIT_0_STRUCT      dTrainingEn;
  DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_STRUCT hsioTrainingEn;
  UINT8                                       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].ddrtEnabled) {
      ddrtEnabled = TRUE;
      break;
    }
  }
  if (ddrtEnabled == FALSE) {
    return SUCCESS;
  }

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "PMEM training mode = %d\n", mode);

  daTrainingEn.Data = 0;
  dTrainingEn.Data = 0;
  hsioTrainingEn.Data = 0;

  daTrainingEn.Bits.tm_en = 1;
  dTrainingEn.Bits.tm_en = 1;
  hsioTrainingEn.Bits.tm_en = 1;

  switch (mode){
    case DISABLE_TRAINING_MODE:
      // Exit Training Mode
      daTrainingEn.Data = 0;
      dTrainingEn.Data = 0;
      hsioTrainingEn.Data = 0;
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, " Exit Training Mode\n");
      break;
    case CHECKPOINT_MINOR_EARLY_CMD_CLK:
      daTrainingEn.Bits.early_cmd_ck_trn  = 1;
      hsioTrainingEn.Bits.early_cmd_ck_trn = 1;
      break;
    case CHECKPOINT_MINOR_CMD_VREF_CENTERING:
    case CHECKPOINT_MINOR_LATE_CMD_CLK:
      daTrainingEn.Bits.adv_cmd_ck_trn  = 1;
      dTrainingEn.Bits.adv_cmd_ck_trn  = 1;
      hsioTrainingEn.Bits.adv_cmd_ck_trn = 1;
      break;
#ifdef LRDIMM_SUPPORT
    case CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING:
      hsioTrainingEn.Bits.recen_dq_dqs_mpr_trn = 1;
      break;
#endif
    case DISABLE_TRAINING_STEP:
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, " Training Step Disabled\n");
      break;
    case ENABLE_TRAINING_MODE:
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT," Training Mode Enabled\n");
      break;
    default:
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT," Training Step Not Supported\n");
      break;
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }

      //
      // Keep tm_en for the disable Channel
      //
      if ((mode == DISABLE_TRAINING_MODE) && (IsChannelEnabled (socket, ch) == FALSE)) {
        WriteFnvCfgSmb(Host, socket, ch, dimm, DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_REG, daTrainingEn.Data | BIT0);
        WriteFnvCfgSmb(Host, socket, ch, dimm, D_DDRT_TRAINING_EN_CWV_D_UNIT_0_REG, dTrainingEn.Data | BIT0);
        WriteFnvCfgSmb(Host, socket, ch, dimm, DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_REG, hsioTrainingEn.Data | BIT0);
      } else {
        WriteFnvCfgSmb(Host, socket, ch, dimm, DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_REG, daTrainingEn.Data);
        WriteFnvCfgSmb(Host, socket, ch, dimm, D_DDRT_TRAINING_EN_CWV_D_UNIT_0_REG, dTrainingEn.Data);
        WriteFnvCfgSmb(Host, socket, ch, dimm, DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_REG, hsioTrainingEn.Data);
      }
    }
  }

  return SUCCESS;
}

/**

  Routine Description: Enables/Disables Training Mode for NVMCTLR, SMBUS version

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number
  @param[in] GetFlag - True: Get, False: Restore
  @param[in/out] DaTrainingEnOrg - DCPMM training mode register setting
  @param[in/out] DTrainingEnOrg  - DCPMM training mode register setting
  @param[in/out] HsioTrainingEnOrg - DCPMM training mode register setting

  @retval SUCCESS
**/
UINT32
GetRestoreAepTrainingModeSMB (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket,
  IN  UINT8       Ch,
  IN  BOOLEAN     GetFlag,
  IN OUT  UINT32  *DaTrainingEnOrg,
  IN OUT  UINT32  *DTrainingEnOrg,
  IN OUT  UINT32  *HsioTrainingEnOrg
)
{
  UINT8                                       Dimm;
  struct dimmNvram                            (*DimmNvList)[MAX_DIMM];
  DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_STRUCT    DaTrainingEn;
  D_DDRT_TRAINING_EN_CWV_D_UNIT_0_STRUCT      DTrainingEn;
  DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_STRUCT HsioTrainingEn;

  DimmNvList = GetDimmNvList(Host, Socket, Ch);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
      continue;
    }

    if (GetFlag == TRUE) {
      ReadFnvCfgSmb(Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_REG, &DaTrainingEn.Data);
      ReadFnvCfgSmb(Host, Socket, Ch, Dimm, D_DDRT_TRAINING_EN_CWV_D_UNIT_0_REG, &DTrainingEn.Data);
      ReadFnvCfgSmb(Host, Socket, Ch, Dimm, DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_REG, &HsioTrainingEn.Data);
      *DaTrainingEnOrg = DaTrainingEn.Data;
      *DTrainingEnOrg = DTrainingEn.Data;
      *HsioTrainingEnOrg = HsioTrainingEn.Data;
    } else {
      WriteFnvCfgSmb(Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_REG, *DaTrainingEnOrg);
      WriteFnvCfgSmb(Host, Socket, Ch, Dimm, D_DDRT_TRAINING_EN_CWV_D_UNIT_0_REG, *DTrainingEnOrg);
      WriteFnvCfgSmb(Host, Socket, Ch, Dimm, DDRT_HSIO_TRAINING_EN_CWV_DDRIO_HSIO_REG, *HsioTrainingEnOrg);
    }
  }

  return SUCCESS;
}

/**

  This resets the NVMCTLR DDRTIO FIFO Pointers

  @param Host    - Pointer to sysHost
  @param socket    - Socket number

  @retval N/A

**/
VOID
IODdrtReset (
             PSYSHOST Host,
             UINT8    socket,
             UINT8    dimm
             )
{
  UINT8                                          ch;
  struct channelNvram                            (*channelNvList)[MAX_CH];
  struct dimmNvram                               (*dimmNvList)[MAX_DIMM];
//CWV_TODO  MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_STRUCT fnvioCtl;
  UINT8                                          MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, socket, ch);
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
      continue;
    }

//CWV_TODO    fnvioCtl.Data = (*dimmNvList)[dimm].fnvioControl;
//CWV_TODO    fnvioCtl.Bits.mc2ddrttrainreset = 1;
//CWV_TODO    WriteFnvCfg(Host, socket, ch, dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, fnvioCtl.Data);
//CWV_TODO    fnvioCtl.Bits.mc2ddrttrainreset = 0;
//CWV_TODO    WriteFnvCfg(Host, socket, ch, dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, fnvioCtl.Data);
//CWV_TODO    (*dimmNvList)[dimm].fnvioControl = fnvioCtl.Data;
  } // ch
} // IODdrtReset

VOID
IODdrtResetAll(
  PSYSHOST Host,
  UINT8 socket
  )
{
  UINT8 dimm;

  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    IODdrtReset(Host, socket, dimm);
  } // dimm
} // IODdrtResetAll


/**

This resets the FNV DDRTIO FIFO Pointers

@param Host    - Pointer to sysHost
@param socket    - Socket number

@retval N/A

**/
VOID
IODdrtResetPerCh(
PSYSHOST Host,
UINT8    socket,
UINT8    ch,
UINT8    dimm
)
{
  struct channelNvram(*channelNvList)[MAX_CH];
  struct dimmNvram(*dimmNvList)[MAX_DIMM];
//CWV_TODO  MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_STRUCT fnvioCtl;

  channelNvList = GetChannelNvList(Host, socket);
  if ((*channelNvList)[ch].enabled == 0) {
    return;
  }
  dimmNvList = GetDimmNvList(Host, socket, ch);
  if ((*dimmNvList)[dimm].dimmPresent == 0) {
    return;
  }
  if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
    return;
  }

//CWV_TODO  fnvioCtl.Data = (*dimmNvList)[dimm].fnvioControl;
//CWV_TODO  fnvioCtl.Bits.mc2ddrttrainreset = 1;
//CWV_TODO  WriteFnvCfg(Host, socket, ch, dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, fnvioCtl.Data);
//CWV_TODO  fnvioCtl.Bits.mc2ddrttrainreset = 0;
//CWV_TODO  WriteFnvCfg(Host, socket, ch, dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, fnvioCtl.Data);
//CWV_TODO  (*dimmNvList)[dimm].fnvioControl = fnvioCtl.Data;
} // IODdrtResetPerCh

/**

  This sets the CTL loopback mode in the buffer

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number
  @param[in] Ch        - Channel number
  @param[in] Dimm      - Dimm number
  @param[in] TrainFlag - TRUE:  Set Rc0c
                         False: Clear Rc0c

  @retval N/A

**/
VOID
ChipSetCTLLoopbackFmc (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Ch,
  IN  UINT8    Dimm,
  IN  BOOLEAN  TrainFlag
)
{
  struct dimmNvram            (*DimmNvList)[MAX_DIMM];
  UINT8 CwData;

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  ReadRcdCwCache (Socket, Ch, SUB_CH_A, Dimm, RDIMM_CW_PAGE0, DDR5_RCD_HOST_INTERFACE_TRAINING_MODE_GLOBAL_REG, &CwData);
  WriteFnvCfgSmb (Host, Socket, Ch, Dimm, RW02_HOST_INTERFACE_TRAINING_MODE_CWV_DDRIO_HSIO_REG, CwData);

} // ChipSetCTLLoopbackFmc

/**

  Put CS0_n into loopback mode in the buffer

  @param[in] Host      - Pointer to sysHost
  @param[in] Socket    - Socket number
  @param[in] Ch        - Channel number

  @retval N/A

**/
VOID
ChipCS0ToLoopback (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
)
{
//CWV_TODO  WriteFnvCfgSmb(Host, Socket, Ch, 1, DA_DDRT_TRAINING_RC0C_FNV_DA_UNIT_0_REG, 0x2);
} // ChipCS0ToLoopback

/**

  This functions enables TxOn in datacontrol0

  @param Host     - Pointer to sysHost
  @param socket   - Socket number

  @retval - None

**/
VOID
ToggleTxOnFnv(
              PSYSHOST Host,
              UINT8    socket,
              UINT32   chBitmask,
              UINT8    dimm,
              UINT8    value
)
{
  UINT8                             ch;
  UINT8                             strobe;
//CWV_TODO  DATACONTROL0N0_0_FNV_DDRIO_COMP_STRUCT fnvDataControl0;
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8                              MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (!((1 << ch) & chBitmask)) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, socket, ch);
    if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
      continue;
    }
    for (strobe = 0; strobe < MSVx4; strobe++) {
//CWV_TODO      GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG), &fnvDataControl0.Data, 0, FNV_IO_READ, 0xF);
//CWV_TODO      fnvDataControl0.Bits.txon = value;
//CWV_TODO      GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG), &fnvDataControl0.Data, 0, FNV_IO_WRITE, 0xF);
    } // strobe
    IODdrtResetPerCh (Host, socket, ch, dimm);
  } // ch
}

/**

  This functions enables InternalClocks On in datacontrol0

  @param Host     - Pointer to sysHost
  @param socket   - Socket number

  @retval - None

**/
VOID
ToggleInternalClocksOnFnv(
                       PSYSHOST Host,
                       UINT8    socket,
                       UINT32   chBitmask,
                       UINT8    dimm,
                       UINT8    value
)
{
  UINT8                             ch;
  UINT8                             strobe;
//CWV_TODO  DATACONTROL0N0_0_FNV_DDRIO_COMP_STRUCT fnvDataControl0;
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  UINT8                             MaxChDdr;
  UINT8 MSVx4 = GetMaxStrobeValid(Host);

  MaxChDdr = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (!((1 << ch) & chBitmask)) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, socket, ch);
    if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
      continue;
    }
    for (strobe = 0; strobe < MSVx4; strobe++) {
//CWV_TODO      GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG), &fnvDataControl0.Data, 0, FNV_IO_READ, 0xF);
//CWV_TODO      fnvDataControl0.Bits.internalclockson = value;
//CWV_TODO      GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG), &fnvDataControl0.Data, 0, FNV_IO_WRITE, 0xF);
    } // strobe
    IODdrtResetPerCh (Host, socket, ch, dimm);
  } // ch
}

VOID
RPQDrain (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Ch,
  UINT8    Dimm,
  UINT8    Rank
  )
{
  struct dimmNvram                               (*DimmNvList)[MAX_DIMM];
  DA_DDRT_TRAINING_OTHER_EN_CWV_DA_UNIT_0_STRUCT DdrtTrainingOtherEn;
  BUS_TYPE                                       OriginalFmcAccessMode;

  DimmNvList = GetDimmNvList(Host, Socket, Ch);
  if ((*DimmNvList)[Dimm].DcpmmPresent) {

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "\nCalling RPQ Draining Sequence\n");

    //
    // DDRT DIMMs must be accessed over SMBUS if the RPQ is full.
    //
    OriginalFmcAccessMode = GetFmcAccessMode (Socket, Ch);
    SetFmcAccessMode (Socket, Ch, SMBUS);

    //
    // 1. Invalidate Prefetch Cache
    //
    if ((*DimmNvList)[Dimm].fmcType == FMC_CWV_TYPE) {
      DdrtTrainingOtherEn.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_OTHER_EN_CWV_DA_UNIT_0_REG);
      DdrtTrainingOtherEn.Bits.sa_pche_inv_allsets = 1;
      DdrtTrainingOtherEn.Bits.sb_pche_inv_allsets = 1;
      WriteFnvCfg (Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_OTHER_EN_CWV_DA_UNIT_0_REG, DdrtTrainingOtherEn.Data);
    }

    //
    // 2. Send Read commands to clear the RPQ. We will send 1 command at a time until the rpq is empty.
    //
    RPQDrainCadbMRSMode (Host, Socket, Ch, Dimm, Rank);

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //

    CteDelayQclk (450);

    ClearDdrtReqCounterMc (Host, Socket, Ch, Dimm);

    SetFmcAccessMode (Socket, Ch, OriginalFmcAccessMode);
  }
}

VOID
FifoTrainReset (
                PSYSHOST Host,
                UINT8    socket
)
{
  UINT8 ch;
  UINT8 dimm;
  struct channelNvram                                     (*channelNvList)[MAX_CH];
  struct dimmNvram                                        (*dimmNvList)[MAX_DIMM];
//CWV_TODO  DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_STRUCT            fifoTrainReset;
  UINT8                                                   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
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
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }
//CWV_TODO      fifoTrainReset.Data = ReadFnvCfg (Host, socket, ch, dimm, DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_REG);
//CWV_TODO      fifoTrainReset.Bits.cxfifo_trainreset = 1;
//CWV_TODO      WriteFnvCfg (Host, socket, ch, dimm, DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_REG, fifoTrainReset.Data);
//CWV_TODO      fifoTrainReset.Bits.cxfifo_trainreset = 0;
//CWV_TODO      WriteFnvCfg (Host, socket, ch, dimm, DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_REG, fifoTrainReset.Data);
    } // dimm
  } // ch loop
}

/**

  Enable/Disable BCOM Mode for DDRT

  @param[in] Host     - Pointer to sysHost, the system Host (root) structure
  @param[in] Socket   - Socket Id
  @param[in] Value    - Enable/Disable

  @retval None

**/
VOID
ToggleBcomFlopEn (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Value
)
{
// Keep these functions empty as there are still common files in ICX
}


/**

  Enable/Disable BCOM Mode for DDRT on specified Channel

  @param[in] Host    - Pointer to sysHost, the system host (root) structure struct
  @param[in] Socket  - current socket
  @param[in] Ch      - Channel number
  @param[in] Value   - Enable/Disable

  @retval None

**/
VOID
ToggleBcomFlopEnPerCh (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Ch,
  IN  UINT8     Value
)
{
// Keep these functions empty as there are still common files in ICX
} // ToggleBcomFlopEnPerCh

VOID
ToggleBcom(PSYSHOST Host,
           UINT8    socket,
           UINT8    value
)
{
// Keep these functions empty as there are still common files in ICX
}

VOID
ProgramDAFnvMprPattern (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    pattern
)
{
  DA_DDRT_TRAINING_MPR_CWV_DA_UNIT_0_STRUCT mprPage0;

  mprPage0.Bits.mpr0 = (UINT8)(pattern & 0xFF);
  mprPage0.Bits.mpr1 = (UINT8)(pattern >> 8);
  mprPage0.Bits.mpr2 = 0x0F;
  mprPage0.Bits.mpr3 = 0;
  WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_TRAINING_MPR_CWV_DA_UNIT_0_REG, mprPage0.Data);
}

VOID
ProgramDFnvMprPattern (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    pattern
  )
{
  D_DDRT_TRAINING_MPR_CWV_D_UNIT_0_STRUCT mprPage0;

  mprPage0.Bits.mpr0 = (UINT8)(pattern & 0xFF);
  mprPage0.Bits.mpr1 = (UINT8)(pattern >> 8);
  mprPage0.Bits.mpr2 = 0x0F;
  mprPage0.Bits.mpr3 = 0;
  WriteFnvCfg(Host, socket, ch, dimm, D_DDRT_TRAINING_MPR_CWV_D_UNIT_0_REG, mprPage0.Data);
}

VOID
SetRankDAMprFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    mprMode
  )
{
  WriteFnvCfg (Host, socket, ch, dimm, DA_DDRT_TRAINING_MR3_CWV_DA_UNIT_0_REG, mprMode);
}

VOID
SetRankDMprFnv (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    mprMode
  )
{

  struct dimmNvram                (*dimmNvList)[MAX_DIMM];

  dimmNvList    = GetDimmNvList(Host, socket, ch);
  if ((*dimmNvList)[dimm].DcpmmPresent) {
    WriteFnvCfg (Host, socket, ch, dimm, D_DDRT_TRAINING_MR3_CWV_D_UNIT_0_REG, mprMode);
  }
} // SetRankDMprFnv

VOID
DdrtBufferReset (
                PSYSHOST Host,
                UINT8    socket,
                UINT8    ch,
                UINT8    dimm
)
{
//CWV_TODO  DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_STRUCT   daDdrtRc06;

  FixedDelayMicroSecond (500);
//CWV_TODO  daDdrtRc06.Data = 0;
//CWV_TODO  daDdrtRc06.Bits.misc_ctrl = 1;
//CWV_TODO  WriteFnvCfgSmb(Host, socket, ch, dimm, DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_REG, daDdrtRc06.Data);
//CWV_TODO  daDdrtRc06.Bits.misc_ctrl = 0;
//CWV_TODO  WriteFnvCfgSmb(Host, socket, ch, dimm, DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_REG, daDdrtRc06.Data);

} // DdrtBufferReset

VOID
SetFmcTempCrit (
                  PSYSHOST Host,
                  UINT8    socket,
                  UINT8    ch,
                  UINT8    dimm,
                  UINT32   TempCrit
)
{
  MSC_TEMP_CRIT_CSR_CWV_DFX_MISC_0_STRUCT     fnvTempCrit;

  fnvTempCrit.Data = 0;
  fnvTempCrit.Bits.tempcrit = TempCrit;
  WriteFnvCfg(Host, socket, ch, dimm, MSC_TEMP_CRIT_CSR_CWV_DFX_MISC_0_REG, fnvTempCrit.Data);
}  //SetFmcTempCrit

UINT32
GetFmcCoreFreq (
                  PSYSHOST Host,
                  UINT8    Socket,
                  UINT8    Ch,
                  UINT8    Dimm
)
{
  CORE_RESET_STATUS_CWV_DFX_MISC_0_STRUCT       mscngnpllctrl1;

  mscngnpllctrl1.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, CORE_RESET_STATUS_CWV_DFX_MISC_0_REG);
  return mscngnpllctrl1.Bits.freqcur;
}  //GetFmcCoreFreq

/**
  Config Read ID feedback for NVMDIMM when during training mode or RMT

  @param Host            - Pointer to sysHost
  @param RidFeedbackDis  - 1: Disable RID feedback
                           0: Enable RIT feedback

**/
VOID
ConfigNvmdimmtRidFeedback (
  IN PSYSHOST Host,
  IN BOOLEAN  RidFeedbackDis
  )
{
  UINT8                                       Socket;
  UINT8                                       Ch;
  UINT8                                       Dimm;
  struct channelNvram                         (*ChannelNvList)[MAX_CH];
  struct dimmNvram                            (*DimmNvList)[MAX_DIMM];
  DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_STRUCT    DaTrainingEn;
  D_DDRT_TRAINING_EN_CWV_D_UNIT_0_STRUCT      DTrainingEn;
  UINT8                                       MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if ((Host->nvram.mem.socket[Socket].enabled == 0) || (Host->nvram.mem.socket[Socket].maxDimmPop == 0))  continue;

    ChannelNvList = GetChannelNvList(Host, Socket);
    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if ((*ChannelNvList)[Ch].ddrtEnabled == 0) continue;

      DimmNvList = GetDimmNvList(Host, Socket, Ch);
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) continue;
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0) continue;

        ReadFnvCfgSmb(Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_REG, &DaTrainingEn.Data);
        ReadFnvCfgSmb(Host, Socket, Ch, Dimm, D_DDRT_TRAINING_EN_CWV_D_UNIT_0_REG, &DTrainingEn.Data);

        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "daTrainingEn = 0x%x, dTrainingEn = 0x%x\n", DaTrainingEn.Data, DTrainingEn.Data);

        if(RidFeedbackDis) {
          DaTrainingEn.Bits.disable_rid_feedback = 1;
          DTrainingEn.Bits.disable_rid_feedback = 1;
        } else {
          DaTrainingEn.Bits.disable_rid_feedback = 0;
          DTrainingEn.Bits.disable_rid_feedback = 0;
        }

        WriteFnvCfgSmb(Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_EN_CWV_DA_UNIT_0_REG, DaTrainingEn.Data);
        WriteFnvCfgSmb(Host, Socket, Ch, Dimm, D_DDRT_TRAINING_EN_CWV_D_UNIT_0_REG, DTrainingEn.Data);
      }
    }
  }
} // ConfigNvmdimmtRidFeedback

/**
  This function gets the FFW digest data from the specified DCPMM DIMM.

  @param[in] Host                         Pointer to the sysHost structure.
  @param[in] SocketId                     Socket index.
  @param[in] ChId                         Channel index.
  @param[in] DimmId                       DIMM index.
  @param[in] DigestLen                    Digest data length in units of UINT32.
  @param[out] FfwDigest                   Pointer to the FFW digest data.
  @param[out] FfwDigest1                  Pointer to the FFW digest1 data.

  @retval MRC_STATUS_SUCCESS              This function is executed successfully.
  @retval MRC_STATUS_NOT_FOUND            DCPMM DIMM is not found on the specified DIMM slot.
  @retval MRC_STATUS_INVALID_PARAMETER    Some of the input parameters are invalid.

**/
MRC_STATUS
GetFfwDigest (
  IN  PSYSHOST Host,
  IN  UINT8    SocketId,
  IN  UINT8    ChId,
  IN  UINT8    DimmId,
  IN  UINT8    DigestLen,
  OUT UINT32   *FfwDigest,
  OUT UINT32   *FfwDigest1
  )
{
  UINT8  RegId;
  UINT32 FfwDigestRegs[] = {
    A_FFW_DIGEST_CSR_0_CWV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_1_CWV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_2_CWV_A_UNIT_0_REG,
    A_FFW_DIGEST_CSR_3_CWV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_4_CWV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_5_CWV_A_UNIT_0_REG,
    A_FFW_DIGEST_CSR_6_CWV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_7_CWV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_8_CWV_A_UNIT_0_REG,
    A_FFW_DIGEST_CSR_9_CWV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_10_CWV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_11_CWV_A_UNIT_0_REG
  };

  UINT32 FfwDigest1Regs[] = {
    A_FFW_DIGEST1_CSR_0_CWV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_1_CWV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_2_CWV_A_UNIT_0_REG,
    A_FFW_DIGEST1_CSR_3_CWV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_4_CWV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_5_CWV_A_UNIT_0_REG,
    A_FFW_DIGEST1_CSR_6_CWV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_7_CWV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_8_CWV_A_UNIT_0_REG,
    A_FFW_DIGEST1_CSR_9_CWV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_10_CWV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_11_CWV_A_UNIT_0_REG
  };

  if ((FfwDigest == NULL) || (FfwDigest1 == NULL) || (DigestLen > sizeof (FfwDigestRegs) / sizeof (UINT32))) {
    return MRC_STATUS_INVALID_PARAMETER;
  }

  for (RegId = 0; RegId < DigestLen; RegId++) {
    *(FfwDigest + RegId)  = ReadFnvCfg (Host, SocketId, ChId, DimmId, FfwDigestRegs[RegId]);
    *(FfwDigest1 + RegId) = ReadFnvCfg (Host, SocketId, ChId, DimmId, FfwDigest1Regs[RegId]);
  }

  return MRC_STATUS_SUCCESS;

}

/** Checks if the NGN/DDRT Firmware Interface Spec version on a given DIMM is less than a particular value.

  @param[in] Host           Pointer to the system host (root) structure
  @param[in] Socket         The socket number where the DIMM resides
  @param[in] Channel        The channel number where the DIMM resides on the provided socket
  @param[in] Dimm           The DIMM number on the provided channel
  @param[in] MajorVersion   The major version of the FIS (from Major.Minor format, e.g. 1 in 1.6)
  @param[in] MinorVersion   The minor version of the FIS (from Major.Minor format, e.g. 6 in 1.6)

  @retval FALSE The DIMM's FIS is greater than or equal to the one determined by the input values
  @retval TRUE  The DIMM's FIS is less than the one determined by the input values

**/
BOOLEAN
CheckFisVersionLessThan (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Channel,
  IN UINT8     Dimm,
  IN UINT8     MajorVersion,
  IN UINT8     MinorVersion
  )
{

  UINT8             ActualMajorVersion;
  UINT8             ActualMinorVersion;

  ActualMajorVersion = Host->var.mem.socket[Socket].channelList[Channel].FisVersion[Dimm] >> 8;
  ActualMinorVersion = Host->var.mem.socket[Socket].channelList[Channel].FisVersion[Dimm] & 0xFF;

  if (ActualMajorVersion < MajorVersion) {
    // Major version is less than the input
    return TRUE;
  } else if (ActualMajorVersion > MajorVersion) {
    // Major version is greater than the input
    return FALSE;
  } else {
    if (ActualMinorVersion < MinorVersion) {
      // Major version is equal to the input, and minor version is less than the input
      return TRUE;
    } else {
      // Major version is equal to the input, and minor version is greater than or equal to the input
      return FALSE;
    }
  }
} // CheckFisVersionLessThan


/**

  Enable/Disable NVMDIMM C2

  @param[in] Host    - Pointer to sysHost, the system host (root) structure struct
  @param[in] Socket  - current socket
  @param[in] Value   - TRUE: Disable, FALSE: Enable

  @retval None

**/
VOID
ConfigNvmDimmC2 (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  BOOLEAN   Value
)
{
  UINT8                                                  Ch;
  UINT8                                                  Dimm;
/**
CWV_TODO
  DDRCRCMDCOMPOFFSET_1_FNV_DDRIO_DAT7_CH_STRUCT          DdrCrCmdCompOffset1;
  DDRCRCMDCOMPOFFSET_2_FNV_DDRIO_DAT7_CH_STRUCT          DdrCrCmdCompOffset2;
**/
  struct channelNvram                                    (*ChannelNvList)[MAX_CH];
  struct dimmNvram                                       (*DimmNvList)[MAX_DIMM];
  UINT8                                                  MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (((*DimmNvList)[Dimm].dimmPresent == 0) || ((*DimmNvList)[Dimm].DcpmmPresent == 0)) {
        continue;
      }
/**
CWV_TODO
      if ((*DimmNvList)[Dimm].fmcType == FMC_BWV_TYPE) {
        GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCOMPOFFSET_2_FNV_DDRIO_DAT7_CH_REG, &DdrCrCmdCompOffset2.Data, 0, FNV_IO_READ, 0xF);
        if (Value) {
          DdrCrCmdCompOffset2.Bits.ddrcmdctlrx_en &= ~BIT3;
        } else {
          DdrCrCmdCompOffset2.Bits.ddrcmdctlrx_en |= BIT3;
        }
        GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCOMPOFFSET_2_FNV_DDRIO_DAT7_CH_REG, &DdrCrCmdCompOffset2.Data, 0, FNV_IO_WRITE, 0xF);
      } else {
        GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCOMPOFFSET_1_FNV_DDRIO_DAT7_CH_REG, &DdrCrCmdCompOffset1.Data, 0, FNV_IO_READ, 0xF);
        if (Value) {
          DdrCrCmdCompOffset1.Bits.ddrcmdctlrx_en &= ~BIT8 ;
        } else {
          DdrCrCmdCompOffset1.Bits.ddrcmdctlrx_en |= BIT8;
        }
        GetSetFnvIO (Host, Socket, Ch, Dimm, DDRCRCMDCOMPOFFSET_1_FNV_DDRIO_DAT7_CH_REG, &DdrCrCmdCompOffset1.Data, 0, FNV_IO_WRITE, 0xF);
        IODdrtResetPerCh (Host, Socket, Ch, Dimm);
      }
**/
    }
  }
}

/** DDRT 2.0: For CWV A0, BIOS must issue BRST to the databuffer when it asserts DRST low.

  @param[in] Host           Pointer to the system host (root) structure
  @param[in] Socket         The socket number where the DIMM resides
  @param[in] Channel        The channel number where the DIMM resides on the provided socket

  @retval None

**/
VOID
DdrtAssertBrst (
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Channel
  )
{
  UINT8                                     Dimm;
  struct dimmNvram                          (*DimmNvList)[MAX_DIMM];
  RW04_CMD_SPACE_CTRL_CWV_DDRIO_HSIO_STRUCT DdrtLatency;

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  //
  // BRST should not be asserted before DDRT IO Init. The interface has not been initialized and BRST would not assert.
  //
  if (Host->var.mem.socket[Socket].ioInitdone == 0) {
    return;
  }

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }

    if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
      continue;
    }

    DdrtLatency.Data = ReadFnvCfg (Host, Socket, Channel, Dimm, RW04_CMD_SPACE_CTRL_CWV_DDRIO_HSIO_REG);
    DdrtLatency.Bits.cmdnum = ASSERT_BRST_A;
    WriteFnvCfg (Host, Socket, Channel, Dimm, RW04_CMD_SPACE_CTRL_CWV_DDRIO_HSIO_REG, DdrtLatency.Data);
    DdrtLatency.Bits.cmdnum = DE_ASSERT_BRST_A;
    WriteFnvCfg (Host, Socket, Channel, Dimm, RW04_CMD_SPACE_CTRL_CWV_DDRIO_HSIO_REG, DdrtLatency.Data);
    DdrtLatency.Bits.cmdnum = ASSERT_BRST_B;
    WriteFnvCfg (Host, Socket, Channel, Dimm, RW04_CMD_SPACE_CTRL_CWV_DDRIO_HSIO_REG, DdrtLatency.Data);
    DdrtLatency.Bits.cmdnum = DE_ASSERT_BRST_B;
    WriteFnvCfg (Host, Socket, Channel, Dimm, RW04_CMD_SPACE_CTRL_CWV_DDRIO_HSIO_REG, DdrtLatency.Data);
  }
}

/** Get the type of FMC on a given channel

  @param[in]  Socket  The socket containing the DDRT DIMM
  @param[in]  Channel The channel containing the DDRT DIMM

  @retval The type of FMC (e.g. FMC_EKV_TYPE, FMC_BWV_TYPE, etc.)

**/
UINT8
EFIAPI
GetFmcType (
  IN  UINT8 Socket,
  IN  UINT8 Channel
  )
{
  UINT8             Dimm;
  DIMM_NVRAM_STRUCT (*DimmNvList)[MAX_DIMM];
  UINT8             FmcType = FMC_NONE_TYPE;
  PSYSHOST          Host;

  Host = (PSYSHOST)GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if (((*DimmNvList)[Dimm].dimmPresent == 1) &&
        ((*DimmNvList)[Dimm].DcpmmPresent == 1))
    {
      FmcType = (*DimmNvList)[Dimm].fmcType;
      break;
    }
  }

  return FmcType;
}

/**
  Retrieves the manufacturer's revision of the FMC

  @param[in] Host             Pointer to sysHost
  @param[in] Socket           Socket Id
  @param[in] Channel          Socket channel number
  @param[in] Dimm             DIMM slot number

  @retval FMC_STEPPING        Stepping of the FMC
**/
FmcStepping
EFIAPI
GetFmcStepping (
  PSYSHOST Host,
  UINT8    Socket,
  UINT8    Channel,
  UINT8    Dimm
  )
{
  DIMM_NVRAM_STRUCT     (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  return (FmcStepping) (*DimmNvList)[Dimm].fmcRev & FMC_STEPPING_MASK;
}  // GetFmcStepping

/**
  Checks to see if the DDRT Round Trip value timing should be used.

  @param[in]  Socket                - Socket Number
  @param[in]  Channel               - DDR Channel ID

  @retval  TRUE                     - DDRT Round Trip value should be used
  @retval  FALSE                    - Default Round Trip value should be used
**/
BOOLEAN
EFIAPI
DdrtRoundTripIsNeeded (
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  return FALSE;
} // DdrtRoundTripIsNeeded


/**
  Determine if clock movement should be avoided to prevent FMC from hanging.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)

  @retval TRUE  - clock movement should be avoided
  @retval FALSE - clock movement is allowed.

**/
BOOLEAN
FmcAvoidClockMovementInSocket (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  return FALSE;
}

/**
  Determine if clock movement should be avoided to prevent FMC from hanging.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket

  @retval TRUE  - clock movement should be avoided
  @retval FALSE - clock movement is allowed.

**/
BOOLEAN
FmcAvoidClockMovementInChannel (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  return FALSE;
}

/**
  Determine if clock movement should be avoided to prevent FMC from hanging.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket
  @param[in]  Dimm        - Dimm number within the Channel

  @retval TRUE  - clock movement should be avoided
  @retval FALSE - clock movement is allowed.

**/
BOOLEAN
FmcAvoidClockMovementInDimm (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm
  )
{
  return FALSE;
}

/**
  Determine if the given socket and channel is populated with a DIMM that may require frequency limitations

  @param[in]    Host    Pointer to host structure
  @param[in]    Socket  Socket number to check
  @param[in]    Channel Channel number to check

  @retval   FALSE   No DCPMM DIMMs found on the given socket & channel that require frequency limitations
  @retval   TRUE    DCPMM DIMMs found on the given socket & channel that may require frequency limitations
**/
BOOLEAN
EFIAPI
FmcEnforceDcpmmFreqLimit (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  return FALSE;
}

/**
  Check if the current FMC executing is this type and revision.

  @param[in] FmcType      UINT8 of the FmcType define
  @param[in] Revision      UINT8 of the Revision to check against

  @retval  TRUE    Is the requested FMC and Revision
           FALSE   Is not the requested FMC and Revision

**/
BOOLEAN
IsFmcTypeAndRevision (
  IN UINT8         FmcType,
  IN FmcStepping   Revision
  )
{
  PSYSHOST          Host;
  UINT8             Socket;
  UINT8             MaxChDdr;
  UINT8             Channel;
  UINT8             Dimm;
  DIMM_NVRAM_STRUCT (*DimmNvList)[MAX_DIMM];
  BOOLEAN           RevisionMatch = FALSE;
  UINT8             LocalFmcType;
  FmcStepping       LocalFmcStepping = FmcStepMax;

  if (Revision < FmcStepA0) {
    DEBUG ((DEBUG_ERROR, "%a: Invalid Revision input: 0x%x\n", __FUNCTION__, Revision));
    ASSERT (FALSE);
    return FALSE;
  }

  MaxChDdr = GetMaxChDdr ();
  Host = (PSYSHOST)GetSysHostPointer ();
  Socket = Host->var.mem.currentSocket;

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    if (IsChannelEnabled (Socket, Channel) == FALSE) {
      continue;
    }
    LocalFmcType = GetFmcType (Socket, Channel);
    DimmNvList = GetDimmNvList (Host, Socket, Channel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDcpmmPresentDimm (Socket, Channel, Dimm) == 0) {
        continue;
      }
      LocalFmcStepping = GetFmcStepping (Host, Socket, Channel, Dimm);
      break;
    }
    if ((LocalFmcType == FmcType) && (LocalFmcStepping == Revision)) {
      RevisionMatch = TRUE;
      break;
    }
  } // Channel loop

  return RevisionMatch;
}

/**

Routine Description: GetSetupDdrtFreq will get Ddrt Freq information from BIOS setup menu structure

  @param Socket          - Socket Number
  @param ddrFreqLimit   - DDR Channel ID

  @retval SetupDdrtFreqLimit

**/
UINT8
GetSetupDdrtFreq (
  IN  UINT8       Socket,
  IN  UINT8       ddrFreqLimit
)
{
  UINT8       SetupDdrtFreqLimit = 0;

   switch (ddrFreqLimit) {
   case DDR_3200:
     SetupDdrtFreqLimit = FMC_OPFREQ_3200MTS;
     break;
   case DDR_3600:
     SetupDdrtFreqLimit = FMC_OPFREQ_3600MTS;
     break;
   case DDR_4000:
     SetupDdrtFreqLimit = FMC_OPFREQ_4000MTS;
     break;
   case DDR_4400:
     SetupDdrtFreqLimit = FMC_OPFREQ_4400MTS;
     break;
   case DDR_4800:
     SetupDdrtFreqLimit = FMC_OPFREQ_4800MTS;
     break;
   default:
     RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
       "Out of Pmem Supported Frequencies Limit\n");
     break;
  }

  return SetupDdrtFreqLimit;
}

/**

Routine Description: GetpCodeMbDdrtFreq will get Ddrt freq from pcode mailbox QClkRatio

  @param Socket      - Socket Number
  @param QClkRatio          - DDR Channel ID
  @param DdrtFreq    - Pointer to 8 bit Fmc freq

  @retval 0 SUCCESS
  @retval 1 FAILURE

**/
UINT32
GetpCodeMbDdrtFreq (
  IN  UINT8       Socket,
  IN  UINT8       QClkRatio,
  OUT UINT8       *DdrtFreq
  )
{
  UINT32    Status =FAILURE;

  switch (QClkRatio) {
  case 32:
    *DdrtFreq = FMC_OPFREQ_3200MTS;
    Status = SUCCESS;
    break;
  case 36:
    *DdrtFreq = FMC_OPFREQ_3600MTS;
    Status = SUCCESS;
    break;
  case 40:
    *DdrtFreq = FMC_OPFREQ_4000MTS;
    Status = SUCCESS;
    break;
  case 44:
    *DdrtFreq = FMC_OPFREQ_4400MTS;
     Status = SUCCESS;
     break;
  case 48:
    *DdrtFreq = FMC_OPFREQ_4800MTS;
    Status = SUCCESS;
    break;
  default:
    RcDebugPrintWithDevice (SDBG_MEM_TRAIN, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Out of Pmem Supported Frequencies\n");
    Status = FAILURE;
    break;
  }

  return Status;
}

/**

  Collects DDRT Freq from B2P Mailbox, and compare if setup current Freq limit same as DIMM init freq

  @param[in] Host      - Point to sysHost
  @param[in] Socket    - CPU socket ID

  @retval TRUE  - Dcpmem freq is changed
          FALSE - Dcpmem freq is not changed

**/
BOOLEAN
FmcCheckDdrtFreqChanged (
  IN  PSYSHOST    Host,
  IN  UINT8       Socket
  )
{
  SYS_SETUP            *Setup;
  UINT8                 RefClkType = 0;
  UINT8                 QClkRatio = 0;
  UINT8                 SetupDdrtFreqLimit;
  UINT8                 DdrtFreq;
  UINT32                Status = FAILURE;

  Setup = GetSysSetupPointer ();

  if (IsFmcTypeAndRevision (FMC_CWV_TYPE, FmcStepA0)) {
    //
    // Check if the DDR frequency has been specified by an input parameter
    //
    if (Setup->mem.ddrFreqLimit) {
      SetupDdrtFreqLimit = GetSetupDdrtFreq (Socket, (Setup->mem.ddrFreqLimit - 1));
      //
      // Detect DIMM Freq use pCode Mailbox
      //
      GetQclkRatio (Host, Socket, &RefClkType, &QClkRatio);
      if (RefClkType == 1) {
        Status = GetpCodeMbDdrtFreq (Socket, QClkRatio, &DdrtFreq);
      }
      if ((Status == SUCCESS) && (DdrtFreq != SetupDdrtFreqLimit)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
  Get internal write leveling WL_ADJ_end value.

  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket
  @param[in]  Dimm        - Dimm number within the Channel

  @retval   - write leveling WL_ADJ_end result

**/
UINT16
GetDdrt2WLAdjEndValue (
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm
  )
{

  UINT8         WritePreamble;
  UINT16        AdjValues;

  WritePreamble = GetPreambleState (Socket, WRITE_PREAMBLE);

  switch (WritePreamble) {
  case PREAMBLE_2TCLK:
    AdjValues = WR_LVL_FINE_PI_DDRT_1P25tCK;
    break;

  case PREAMBLE_3TCLK:
    AdjValues = WR_LVL_FINE_PI_DDRT_1P75tCK;
    break;

  default:
    AdjValues = WR_LVL_FINE_PI_DDRT_1P25tCK;
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Not supportted WRITE_PREAMBLE %d tCK for Write Leveling training\n", WritePreamble + 1);
    break;
  }

  return AdjValues;
}

