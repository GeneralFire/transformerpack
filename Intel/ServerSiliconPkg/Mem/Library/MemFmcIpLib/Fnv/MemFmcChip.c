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
#include <Library/CpuAndRevisionLib.h>
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

#define BWV_READ_CREDIT     0x30
#define EKV_READ_CREDIT     0x28
#define FIS_MAJOR_VERSION_1 0x1
#define FIS_MINOR_VERSION_6 0x6

#define  DQ_SWIZZLE_CLEARING_PATTERN 0xe4e4e4e4

#define DDRT_SUPPORTED_FREQUENCIES 7
#define DDRT_FREQUENCY_MAX_INDEX  (DDRT_SUPPORTED_FREQUENCIES - 1)
//                                                                       1600 1866 2133 2400 2666 2933 3200
// FNV: 1DPC, or 2DPC with RDIMM
UINT8 FnvDdrtCASLatencyRDIMM[DDRT_SUPPORTED_FREQUENCIES]                = {10,  12,  14,  15,  15, 19, 20};
UINT8 FnvDdrtCASLatencyAdderRDIMM[DDRT_SUPPORTED_FREQUENCIES]           = { 6,   6,   6,   6,   6,  6,  9};
UINT8 FnvDdrtCASWriteLatencyRDIMM[DDRT_SUPPORTED_FREQUENCIES]           = { 9,  10,  11,  12,  14, 20, 20};
UINT8 FnvDdrtCASWriteLatencyAdderRDIMM[DDRT_SUPPORTED_FREQUENCIES]      = { 7,   7,   7,   7,   9,  9,  9};
// FNV or EKV: 2DPC with LRDIMM
UINT8 DdrtCASLatencyLRDIMM[DDRT_SUPPORTED_FREQUENCIES]                  = {11,  12,  14,  15,  15, 19, 20};
UINT8 DdrtCASLatencyAdderLRDIMM[DDRT_SUPPORTED_FREQUENCIES]             = { 4,   5,   5,   5,   5,  5,  5};
UINT8 DdrtCASWriteLatencyLRDIMM[DDRT_SUPPORTED_FREQUENCIES]             = { 9,  10,  11,  12,  14, 20, 20};
UINT8 DdrtCASWriteLatencyAdderLRDIMM[DDRT_SUPPORTED_FREQUENCIES]        = { 7,   7,   7,   5,   7,  8,  8};
// EKV: 1DPC, or 2DPC with RDIMM
UINT8 EkvDdrtCASLatencyRDIMM[DDRT_SUPPORTED_FREQUENCIES]                = {10,  12,  14,  15,  15, 19, 14};
UINT8 EkvDdrtCASLatencyAdderRDIMM[DDRT_SUPPORTED_FREQUENCIES]           = { 6,   6,   6,   6,   6,  6,  9};
UINT8 EkvDdrtCASWriteLatencyRDIMM[DDRT_SUPPORTED_FREQUENCIES]           = { 9,  10,  14,  16,  18, 20, 20};
UINT8 EkvDdrtCASWriteLatencyAdderRDIMM[DDRT_SUPPORTED_FREQUENCIES]      = { 7,   7,   6,   5,   6,  7,  3};
// BWV: 1DPC, or 2DPC with RDIMM for ICX+BPS
UINT8 BwvDdrtCASLatencyRDIMM[DDRT_SUPPORTED_FREQUENCIES]                = {10,  12,  14,  20,  20, 20, 20};
UINT8 BwvDdrtCASLatencyAdderRDIMM[DDRT_SUPPORTED_FREQUENCIES]           = { 6,   6,   6,   4,   4,  4,  4};
UINT8 BwvDdrtCASWriteLatencyRDIMM[DDRT_SUPPORTED_FREQUENCIES]           = { 9,  10,  14,  16,  16, 20, 20};
UINT8 BwvDdrtCASWriteLatencyAdderRDIMM[DDRT_SUPPORTED_FREQUENCIES]      = { 7,   7,   6,   5,   6,  6,  6};
// BWV: 1DPC, or 2DPC with RDIMM for CLX+BPS
UINT8 BwvClxDdrtCASWriteLatencyRDIMM[DDRT_SUPPORTED_FREQUENCIES]        = { 9,  10,  14,  16,  20, 20, 20};
UINT8 BwvClxDdrtCASWriteLatencyAdderRDIMM[DDRT_SUPPORTED_FREQUENCIES]   = { 7,   7,   6,   6,   6,  6,  6};
UINT8 BwvA1ClxDdrtCASWriteLatencyAdderRDIMM[DDRT_SUPPORTED_FREQUENCIES] = { 7,   7,   6,   6,   6,  7,  7};
// BWV: 2DPC with LRDIMM
UINT8 BwvDdrtCASLatencyLRDIMM[DDRT_SUPPORTED_FREQUENCIES]             = {10,  12,  14,  20,  20, 20, 20};
UINT8 BwvDdrtCASLatencyAdderLRDIMM[DDRT_SUPPORTED_FREQUENCIES]        = { 6,   6,   6,   4,   6,  6,  6};
UINT8 BwvDdrtCASWriteLatencyLRDIMM[DDRT_SUPPORTED_FREQUENCIES]        = { 9,  10,  14,  16,  16, 20, 20};
UINT8 BwvDdrtCASWriteLatencyAdderLRDIMM[DDRT_SUPPORTED_FREQUENCIES]   = { 7,   7,   6,   5,   6,  6,  6};
// UBIOS/HSLE
UINT8 UbiosOrHsleDdrtCASWriteLatency[DDRT_SUPPORTED_FREQUENCIES]      = {11,  12,  14,  16,  18, 20, 20};

//
// Local Prototypes
//
/**
Determines if the FMC in the given channel is BWV and the stepping is A1 and up.

@param[in]   Host    Pointer to sysHost
@param[in]   Socket  Socket ID
@param[in]   Channel Channel ID

@retval      True    The type of the FMC in the given channel is BWV and the stepping is A1 and up.
@retval      False   The type of the FMC in the given channel is NOT BWV or the stepping is not A1 and up.
**/
BOOLEAN
IsBwvA1SteppingAndUp (PSYSHOST Host, UINT8 Socket, UINT8 Channel);

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
  //
  // NULL
  //
  return FALSE;
}

VOID
DisableScrambling (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  UINT8                                   dimm;
  struct dimmNvram                        (*dimmNvList)[MAX_DIMM];
  DA_WR_SCR_LFSR_L_FNV_DA_UNIT_0_STRUCT   txSeedLo;
  DA_RD_SCR_LFSR_FNV_DA_UNIT_0_STRUCT     rxSeed;

  dimmNvList = GetDimmNvList(Host, socket, ch);
  for (dimm = 0; dimm < MAX_DIMM; dimm++) {
    if ((*dimmNvList)[dimm].dimmPresent == 0) {
      continue;
    }
    if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
      continue;
    }
    txSeedLo.Data = ReadFnvCfg(Host, socket, ch, dimm, DA_WR_SCR_LFSR_L_FNV_DA_UNIT_0_REG);
    rxSeed.Data = ReadFnvCfg(Host, socket, ch, dimm, DA_RD_SCR_LFSR_FNV_DA_UNIT_0_REG);

    txSeedLo.Bits.en = 0;
    rxSeed.Bits.en   = 0;

    WriteFnvCfg(Host, socket, ch, dimm, DA_WR_SCR_LFSR_L_FNV_DA_UNIT_0_REG, txSeedLo.Data);
    WriteFnvCfg(Host, socket, ch, dimm, DA_RD_SCR_LFSR_FNV_DA_UNIT_0_REG, rxSeed.Data);
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
  DN_SPARE_CTRL_FNV_D_UNIT_0_STRUCT              DnSpareCtrlFnv;
  D_DDRT_TRAINING_OTHER_EN_FNV_D_UNIT_0_STRUCT   DdrtTrainingOtherEn;

  DimmNvList = GetDimmNvList(Host, Socket, Ch);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
      continue;
    }

    if ((*DimmNvList)[Dimm].fmcType == FMC_FNV_TYPE) {
      DnSpareCtrlFnv.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, DN_SPARE_CTRL_FNV_D_UNIT_0_REG);
      DnSpareCtrlFnv.Bits.spare_1 |= BIT15;
      WriteFnvCfg (Host, Socket, Ch, Dimm, DN_SPARE_CTRL_FNV_D_UNIT_0_REG, DnSpareCtrlFnv.Data);

      DnSpareCtrlFnv.Bits.spare_1 &= ~BIT15;
      WriteFnvCfg (Host, Socket, Ch, Dimm, DN_SPARE_CTRL_FNV_D_UNIT_0_REG, DnSpareCtrlFnv.Data);
    } else { // EKV
      DdrtTrainingOtherEn.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, D_DDRT_TRAINING_OTHER_EN_FNV_D_UNIT_0_REG);
      DdrtTrainingOtherEn.Bits.pche_inv_set0_only = 1;
      WriteFnvCfg (Host, Socket, Ch, Dimm, D_DDRT_TRAINING_OTHER_EN_FNV_D_UNIT_0_REG, DdrtTrainingOtherEn.Data);
    }
  }
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
BgfReset(
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel,
  IN  UINT8     Dimm,
  IN  UINT8     Value
)
{
  return;
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
        // Fall through to FNV values as a backup if the warning is not promoted to an error.
      case FMC_FNV_TYPE:
        // Fall through: FNV shares values with EKV.
      case FMC_EKV_TYPE:
        *CASLatency = DdrtCASLatencyLRDIMM[DdrtFrequency];
        *CASLatencyAdder = DdrtCASLatencyAdderLRDIMM[DdrtFrequency];
        *CASWriteLatency = DdrtCASWriteLatencyLRDIMM[DdrtFrequency];
        *CASWriteLatencyAdder = DdrtCASWriteLatencyAdderLRDIMM[DdrtFrequency];
        break;
      case FMC_BWV_TYPE:
        *CASLatency = BwvDdrtCASLatencyLRDIMM[DdrtFrequency];
        *CASLatencyAdder = BwvDdrtCASLatencyAdderLRDIMM[DdrtFrequency];
        *CASWriteLatency = BwvDdrtCASWriteLatencyLRDIMM[DdrtFrequency];
        *CASWriteLatencyAdder = BwvDdrtCASWriteLatencyAdderLRDIMM[DdrtFrequency];
        break;
    } // switch (FmcType)
  } else { // 2 DPC with RDIMM or 1 DPC
    switch (FmcType) {
      default:
        OutputWarning (WARN_DIMM_COMPAT, WARN_DIMM_NVMDIMM_NOT_SUPPORTED, Socket, Channel, Dimm, NO_RANK);
        Status = EFI_UNSUPPORTED;
        // Fall through to FNV values as a backup if the warning is not promoted to an error.
      case FMC_FNV_TYPE:
        *CASLatency = FnvDdrtCASLatencyRDIMM[DdrtFrequency];
        *CASLatencyAdder = FnvDdrtCASLatencyAdderRDIMM[DdrtFrequency];
        *CASWriteLatency = FnvDdrtCASWriteLatencyRDIMM[DdrtFrequency];
        *CASWriteLatencyAdder = FnvDdrtCASWriteLatencyAdderRDIMM[DdrtFrequency];
        break;
      case FMC_EKV_TYPE:
        *CASLatency = EkvDdrtCASLatencyRDIMM[DdrtFrequency];
        *CASLatencyAdder = EkvDdrtCASLatencyAdderRDIMM[DdrtFrequency];
        *CASWriteLatency = EkvDdrtCASWriteLatencyRDIMM[DdrtFrequency];
        *CASWriteLatencyAdder = EkvDdrtCASWriteLatencyAdderRDIMM[DdrtFrequency];
        break;
      case FMC_BWV_TYPE:
        *CASLatency = BwvDdrtCASLatencyRDIMM[DdrtFrequency];
        *CASLatencyAdder = BwvDdrtCASLatencyAdderRDIMM[DdrtFrequency];
        if (IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
          *CASWriteLatency = BwvDdrtCASWriteLatencyRDIMM[DdrtFrequency];
          *CASWriteLatencyAdder = BwvDdrtCASWriteLatencyAdderRDIMM[DdrtFrequency];
        } else {
          *CASWriteLatency = BwvClxDdrtCASWriteLatencyRDIMM[DdrtFrequency];
          if (GetFmcStepping (Host, Socket, Channel, Dimm) > FmcStepA0) {
            *CASWriteLatencyAdder = BwvA1ClxDdrtCASWriteLatencyAdderRDIMM[DdrtFrequency];
          } else {
            *CASWriteLatencyAdder = BwvClxDdrtCASWriteLatencyAdderRDIMM[DdrtFrequency];
          }
        }
        break;
    } // switch (FmcType)
  } // if (IsLrdimmPresent (Socket, Channel, OtherDimm)) ... else

  if (UbiosGenerationOrHsleEnabled ()) {
    *CASWriteLatency = UbiosOrHsleDdrtCASWriteLatency[DdrtFrequency];
  }

  *CASWriteLatencyAdder += CalculateDdrtCwlAddPxcAdder (Socket);

  return Status;
}

VOID
ProgramtCLtCWLAdd (
  PSYSHOST Host,
  UINT8    socket,
  UINT8    ch
  )
{
  UINT8                          dimm;
  UINT8                          tclAdd;
  UINT8                          tcwlAdd;
  UINT8                          tCWL;
  struct dimmNvram               (*dimmNvList)[MAX_DIMM];
  DA_DDRT_TRAINING_RC0F_FNV_DA_UNIT_0_STRUCT DdrtTrainingRC0F;
  DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_STRUCT ddrtDqBcomCtrl;
  DA_DDRIO_INIT_CONTROL_FNV_DA_UNIT_0_STRUCT ddrioInitControl;

    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }
      GetDdrtCasLatencies (socket, ch, dimm, NULL, &tclAdd, &tCWL, &tcwlAdd);
      ddrtDqBcomCtrl.Data   = ReadFnvCfg(Host, socket, ch, dimm, DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_REG);
      ddrioInitControl.Data = ReadFnvCfg(Host, socket, ch, dimm, DA_DDRIO_INIT_CONTROL_FNV_DA_UNIT_0_REG);
      ddrtDqBcomCtrl.Bits.tcl_add = (tclAdd & 0x7);
      ddrtDqBcomCtrl.Bits.tcwl_add = tcwlAdd;
      // PlaceHolder
      if (tCWL + tcwlAdd <= 12){
        ddrioInitControl.Bits.ddrtio_par_flopen = 0;
      } else {
        ddrioInitControl.Bits.ddrtio_par_flopen = 1;
      }
      if ((*dimmNvList)[dimm].fmcType > FMC_EKV_TYPE) {
        DdrtTrainingRC0F.Data = ReadFnvCfg (Host, socket, ch, dimm, DA_DDRT_TRAINING_RC0F_FNV_DA_UNIT_0_REG);
        DdrtTrainingRC0F.Bits.tcl_add_3 = (tclAdd >> 3) & 0x3;
        WriteFnvCfg (Host, socket, ch, dimm, DA_DDRT_TRAINING_RC0F_FNV_DA_UNIT_0_REG, DdrtTrainingRC0F.Data);
      }
      WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_REG, ddrtDqBcomCtrl.Data);
      WriteFnvCfg(Host, socket, ch, dimm, DA_DDRIO_INIT_CONTROL_FNV_DA_UNIT_0_REG, ddrioInitControl.Data);
    } //dimm
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
  DA_DDRT_LATENCY_FNV_DA_UNIT_0_STRUCT ddrtLatency;
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
    ddrtLatency.Data = ReadFnvCfg(Host, socket, ch, dimm, DA_DDRT_LATENCY_FNV_DA_UNIT_0_REG);
    ddrtLatency.Bits.tcl_gnt_erid = (*dimmNvList)[dimm].DimmGnt2Erid;
    WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_LATENCY_FNV_DA_UNIT_0_REG, ddrtLatency.Data);
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
  DDRCRCMDCONTROLS_SA_FNV_DDRIO_DAT7_CH_STRUCT  ddrCrDimmCmdControl1; // NVMCTLR CMD
  DDRCRCOMPCTL2_CH_FNV_DDRIO_COMP_STRUCT        ddrCompCtl2Fnv;
  DATACONTROL3N0_0_FNV_DDRIO_COMP_STRUCT        dataControl3CompFnv;      // NVMCTLR data
  DDRCRCLKCONTROLS_CH_FNV_DDRIO_DAT7_CH_STRUCT  DdrCrDimmClkControl1;
  UINT8                   strobe;
  UINT8                   MSVx4 = GetMaxStrobeValid(Host);
  UINT8                   MaxChDdr;

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
  DA_ECC_ENABLE_FNV_DA_UNIT_0_STRUCT            daEccEnable;
  DN_ECC_ENABLE_FNV_D_UNIT_0_STRUCT             dnEccEnable;
  FNV_DEBUG_LOCK_FNV_DFX_MISC_0_STRUCT          fnvDebugLock;
  DRDP_WCRD_CONFIG_FNV_D_UNIT_0_STRUCT          drdpWcrdConfig;
  DA_WRITE_CREDIT_FNV_DA_UNIT_0_STRUCT          daWriteCredit;
  D_READ_CREDIT_FNV_D_UNIT_0_STRUCT             dReadCredit;
  DN_EMASK_FNV_D_UNIT_0_STRUCT                  dnEmask;
  DA_EMASK_FNV_DA_UNIT_0_STRUCT                 daEmask;
  D_FUNC_DEFEATURE1_FNV_D_UNIT_0_STRUCT         dFuncDefeature1;
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

    if (UbiosGenerationOrHsleEnabled ()) {
      SetFmcAccessMode (Socket, ch, EMRS);
    } else {
      SetFmcAccessMode (Socket, ch, SMBUS);
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
      daEccEnable.Data     = ReadFnvCfg (Host, Socket, ch, dimm, DA_ECC_ENABLE_FNV_DA_UNIT_0_REG);
      dnEccEnable.Data     = ReadFnvCfg (Host, Socket, ch, dimm, DN_ECC_ENABLE_FNV_D_UNIT_0_REG);
      drdpWcrdConfig.Data  = ReadFnvCfg (Host, Socket, ch, dimm, DRDP_WCRD_CONFIG_FNV_D_UNIT_0_REG);
      daWriteCredit.Data   = ReadFnvCfg (Host, Socket, ch, dimm, DA_WRITE_CREDIT_FNV_DA_UNIT_0_REG);
      dReadCredit.Data     = ReadFnvCfg (Host, Socket, ch, dimm, D_READ_CREDIT_FNV_D_UNIT_0_REG);

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

      if (Setup->mem.optionsNgn & NGN_ECC_RD_CHK) {
        dnEccEnable.Bits.ecc_rd_chk_en = 1;
      } else {
        dnEccEnable.Bits.ecc_rd_chk_en = 0;
      }

      WriteFnvCfg (Host, Socket, ch, dimm, DA_ECC_ENABLE_FNV_DA_UNIT_0_REG, daEccEnable.Data);
      WriteFnvCfg (Host, Socket, ch, dimm, DN_ECC_ENABLE_FNV_D_UNIT_0_REG, dnEccEnable.Data);

      //
      // Reset Write Credit
      //
      drdpWcrdConfig.Bits.wcrd_cfg = 0;
      WriteFnvCfg (Host, Socket, ch, dimm, DRDP_WCRD_CONFIG_FNV_D_UNIT_0_REG, drdpWcrdConfig.Data);

      //
      // Trigger Write Credit Load by clearing and then setting D_FUNC_DEAFEATURE1.wcrd_init
      //
      dFuncDefeature1.Data = 0;
      GetSetProtectedDcpmmRegister (Host, Socket, ch, dimm, GSM_FORCE_WRITE, D_FUNC_DEFEATURE1_FNV_D_UNIT_0_REG,
        PROT_REG_MASK_WCRD_INIT, &dFuncDefeature1.Data);

      dFuncDefeature1.Bits.wcrd_init = 1;
      GetSetProtectedDcpmmRegister (Host, Socket, ch, dimm, GSM_FORCE_WRITE, D_FUNC_DEFEATURE1_FNV_D_UNIT_0_REG,
        PROT_REG_MASK_WCRD_INIT, &dFuncDefeature1.Data);

      //
      // Program Write Credits and Read Credits
      //
      drdpWcrdConfig.Bits.wcrd_cfg = (*channelNvList)[ch].FmcWrCreditLimit;
      daWriteCredit.Bits.val = (*channelNvList)[ch].FmcWrCreditLimit;
      dReadCredit.Bits.val = (*channelNvList)[ch].FmcRdCreditLimit;
      WriteFnvCfg (Host, Socket, ch, dimm, DRDP_WCRD_CONFIG_FNV_D_UNIT_0_REG, drdpWcrdConfig.Data);
      WriteFnvCfg (Host, Socket, ch, dimm, DA_WRITE_CREDIT_FNV_DA_UNIT_0_REG, daWriteCredit.Data);
      WriteFnvCfg (Host, Socket, ch, dimm, D_READ_CREDIT_FNV_D_UNIT_0_REG, dReadCredit.Data);

      //
      // Unmask Write Credit Check
      //
      if (CheckFisVersionLessThan (Host, Socket, ch, dimm, FIS_MAJOR_VERSION_1, FIS_MINOR_VERSION_6)) {
        daEmask.Data = ReadFnvCfg (Host, Socket, ch, dimm, DA_EMASK_FNV_DA_UNIT_0_REG);
        dnEmask.Data = ReadFnvCfg (Host, Socket, ch, dimm, DN_EMASK_FNV_D_UNIT_0_REG);
        dnEmask.Bits.wr_credit = 0;
        daEmask.Bits.wr_credit = 0;
        WriteFnvCfg (Host, Socket, ch, dimm, DN_EMASK_FNV_D_UNIT_0_REG, dnEmask.Data);
        WriteFnvCfg (Host, Socket, ch, dimm, DA_EMASK_FNV_DA_UNIT_0_REG, daEmask.Data);
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
        fnvDebugLock.Data = ReadFnvCfg(Host, Socket, ch, dimm, FNV_DEBUG_LOCK_FNV_DFX_MISC_0_REG);
        fnvDebugLock.Bits.dbg_en = 1;     // Write 1 to enable
        WriteFnvCfg (Host, Socket, ch, dimm, FNV_DEBUG_LOCK_FNV_DFX_MISC_0_REG, fnvDebugLock.Data);
        fnvDebugLock.Data = ReadFnvCfg(Host, Socket, ch, dimm, FNV_DEBUG_LOCK_FNV_DFX_MISC_0_REG);
        if (fnvDebugLock.Bits.dwe == 1) { // Check debug enabled by HW
          RcDebugPrint (SDBG_DEFAULT, "\n: PMem unlocked for debug S%d.CH%d.D%d:\n", Socket, ch, dimm);
        }
        fnvDebugLock.Bits.lock = 1;       // // Lock the register
        WriteFnvCfg (Host, Socket, ch, dimm, FNV_DEBUG_LOCK_FNV_DFX_MISC_0_REG, fnvDebugLock.Data);
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
  DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_STRUCT daDdrtTrainingRc06;
  DA_DDRT_TRAINING_RC4X_FNV_DA_UNIT_0_STRUCT daDdrtTrainingRc4x;
  DA_DDRT_TRAINING_RC6X_FNV_DA_UNIT_0_STRUCT daDdrtTrainingRc6x;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_STRUCT daDdrtDqBcomCtrl;

  dimmNvList    = GetDimmNvList(Host, socket, ch);

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
  DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_STRUCT  daTrainingEn;
  D_DDRT_TRAINING_EN_FNV_D_UNIT_0_STRUCT    dTrainingEn;
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

      WriteFnvCfgSmb(Host, socket, ch, dimm, DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_REG, daTrainingEn.Data);
      WriteFnvCfgSmb(Host, socket, ch, dimm, D_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG, dTrainingEn.Data);
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
  if (UbiosGenerationOrHsleEnabled ()) {
    SetFmcAccessMode (socket, ch, EMRS);
  } else {
    SetFmcAccessMode (socket, ch, SMBUS);
  }

  switch (controlWordAddr) {
    case RDIMM_RC0E:
      WriteFnvCfg (Host, socket, ch, dimm, DA_DDRT_TRAINING_RC0E_FNV_DA_UNIT_0_REG, controlWordData);
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
                "Clear Dq Swizzle PMem -- START\n");
  ChannelNvList = GetChannelNvList(Host, Socket);
  MaxChDdr = GetMaxChDdr ();

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
      if (IsMemFlowEnabled (DqSwizzlingDiscovery)) {
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
      }
#endif //HW_EMULATION

    } // Dimm loop
  } // Ch loop
  RcDebugPrintWithDevice(SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "Clear Dq Swizzle PMem -- END\n");
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
  DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_STRUCT    daTrainingEn;
  D_DDRT_TRAINING_EN_FNV_D_UNIT_0_STRUCT      dTrainingEn;
  UINT8                                       MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
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
                 "PMem training mode = %d\n", mode);

  daTrainingEn.Data = 0;
  dTrainingEn.Data = 0;

  daTrainingEn.Bits.tm_en = 1;
  dTrainingEn.Bits.tm_en = 1;
  daTrainingEn.Bits.disable_rid_feedback = 1;
  dTrainingEn.Bits.disable_rid_feedback = 1;

  switch (mode){
    case DISABLE_TRAINING_MODE:
      // Exit Training Mode
      daTrainingEn.Data = 0;
      dTrainingEn.Data = 0;
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, " Exit Training Mode\n");
      break;
    case CHECKPOINT_MINOR_EARLY_CMD_CLK:
      daTrainingEn.Bits.early_cmd_ck_trn  = 1;
      break;
    case CHECKPOINT_MINOR_EARLY_RID_FINE:
      daTrainingEn.Bits.basic_erid_trn = 1;
      daTrainingEn.Bits.disable_rid_feedback = 0;
      dTrainingEn.Bits.disable_rid_feedback = 0;
      break;
    case CHECKPOINT_MINOR_EARLY_RID_COARSE:
      daTrainingEn.Bits.disable_rid_feedback = 0;
      dTrainingEn.Bits.disable_rid_feedback = 0;
      break;
    case CHECKPOINT_MINOR_CMD_VREF_CENTERING:
      daTrainingEn.Bits.adv_cmd_ck_trn  = 1;
      dTrainingEn.Bits.adv_cmd_ck_trn  = 1;
      break;
    case CHECKPOINT_MINOR_LATE_CMD_CLK:
      daTrainingEn.Bits.adv_cmd_ck_trn  = 1;
      dTrainingEn.Bits.adv_cmd_ck_trn  = 1;
      break;
#ifdef LRDIMM_SUPPORT
    case CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING:
      daTrainingEn.Bits.recen_dq_dqs_mpr_trn = 1;
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

      WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_REG, daTrainingEn.Data);
      WriteFnvCfg(Host, socket, ch, dimm, D_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG, dTrainingEn.Data);
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
  DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_STRUCT    daTrainingEn;
  D_DDRT_TRAINING_EN_FNV_D_UNIT_0_STRUCT      dTrainingEn;
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
                 "PMem training mode = %d\n", mode);

  daTrainingEn.Data = 0;
  dTrainingEn.Data = 0;

  daTrainingEn.Bits.tm_en = 1;
  dTrainingEn.Bits.tm_en = 1;
  daTrainingEn.Bits.disable_rid_feedback = 1;
  dTrainingEn.Bits.disable_rid_feedback = 1;

  switch (mode){
    case DISABLE_TRAINING_MODE:
      // Exit Training Mode
      daTrainingEn.Data = 0;
      dTrainingEn.Data = 0;
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, " Exit Training Mode\n");
      break;
    case CHECKPOINT_MINOR_EARLY_CMD_CLK:
      daTrainingEn.Bits.early_cmd_ck_trn  = 1;
      break;
    case CHECKPOINT_MINOR_EARLY_RID_FINE:
      daTrainingEn.Bits.basic_erid_trn = 1;
      daTrainingEn.Bits.disable_rid_feedback = 0;
      dTrainingEn.Bits.disable_rid_feedback = 0;
      break;
    case CHECKPOINT_MINOR_EARLY_RID_COARSE:
      daTrainingEn.Bits.disable_rid_feedback = 0;
      dTrainingEn.Bits.disable_rid_feedback = 0;
      break;
    case CHECKPOINT_MINOR_CMD_VREF_CENTERING:
    case CHECKPOINT_MINOR_LATE_CMD_CLK:
      daTrainingEn.Bits.adv_cmd_ck_trn  = 1;
      dTrainingEn.Bits.adv_cmd_ck_trn  = 1;
      break;
#ifdef LRDIMM_SUPPORT
    case CHECKPOINT_MINOR_RX_BACKSIDE_PHASE_TRAINING:
      daTrainingEn.Bits.recen_dq_dqs_mpr_trn = 1;
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
        WriteFnvCfgSmb(Host, socket, ch, dimm, DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_REG, daTrainingEn.Data | BIT0);
        WriteFnvCfgSmb(Host, socket, ch, dimm, D_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG, dTrainingEn.Data | BIT0);
      } else {
        WriteFnvCfgSmb(Host, socket, ch, dimm, DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_REG, daTrainingEn.Data);
        WriteFnvCfgSmb(Host, socket, ch, dimm, D_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG, dTrainingEn.Data);
      }
    }
  }

  return SUCCESS;
} // SetAepTrainingModeSMB

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
  DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_STRUCT    DaTrainingEn;
  D_DDRT_TRAINING_EN_FNV_D_UNIT_0_STRUCT      DTrainingEn;

  DimmNvList = GetDimmNvList(Host, Socket, Ch);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
      continue;
    }

    if (GetFlag == TRUE) {
      ReadFnvCfgSmb(Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_REG, &DaTrainingEn.Data);
      ReadFnvCfgSmb(Host, Socket, Ch, Dimm, D_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG, &DTrainingEn.Data);
      *DaTrainingEnOrg = DaTrainingEn.Data;
      *DTrainingEnOrg = DTrainingEn.Data;
    } else {
      WriteFnvCfgSmb(Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_REG, *DaTrainingEnOrg);
      WriteFnvCfgSmb(Host, Socket, Ch, Dimm, D_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG, *DTrainingEnOrg);
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
  MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_STRUCT fnvioCtl;
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

    fnvioCtl.Data = ReadFnvCfg (Host, socket, ch, dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG);
    fnvioCtl.Bits.mc2ddrttrainreset = 1;
    WriteFnvCfg (Host, socket, ch, dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, fnvioCtl.Data);
    fnvioCtl.Bits.mc2ddrttrainreset = 0;
    WriteFnvCfg (Host, socket, ch, dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, fnvioCtl.Data);
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
  Resets the FNV DDRTIO FIFO Pointers

  @param[in]  Host    Pointer to sysHost
  @param[in]  Socket  Socket number
  @param[in]  Channel Channel number
  @param[in]  Dimm    Dimm number

  @retval N/A
**/
VOID
EFIAPI
IODdrtResetPerCh(
  IN  PSYSHOST Host,
  IN  UINT8    Socket,
  IN  UINT8    Channel,
  IN  UINT8    Dimm
  )
{
  MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_STRUCT FnvioCtl;

  if (IsDcpmmPresentDimm (Socket, Channel, Dimm) == FALSE) {
    return;
  }

  FnvioCtl.Data = ReadFnvCfg (Host, Socket, Channel, Dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG);
  FnvioCtl.Bits.mc2ddrttrainreset = 1;
  WriteFnvCfg (Host, Socket, Channel, Dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, FnvioCtl.Data);
  FnvioCtl.Bits.mc2ddrttrainreset = 0;
  WriteFnvCfg (Host, Socket, Channel, Dimm, MSC_FNVIO_INIT_CONTROL_0_FNV_DFX_MISC_0_REG, FnvioCtl.Data);

  return;
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

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  /* training_mode - Bits[2:0], RW, default = 3'h0
      Training mode selection[br]
            v  000 = Normal operating [br]
               001 = CK-CA training[br]
            v  010 = CS0_n loopback[br]
            v  011 = GNT loopback (CS1_n loopback)[br]
            v  100 = CKE0 loopback[br]
               101 = reserved (CKE1 loopback; NVMDIMM REQ)[br]
            v  110 = ODT0 loopback[br]
               111 = ERID loopback (ODT1 loopback, NVMDIMM ERR)
  */
  if (TrainFlag) {
    if (((*DimmNvList)[Dimm].rcCache[RDIMM_RC0C] == 0) || ((*DimmNvList)[Dimm].rcCache[RDIMM_RC0C] == 2) || ((*DimmNvList)[Dimm].rcCache[RDIMM_RC0C] == 3) ||
      ((*DimmNvList)[Dimm].rcCache[RDIMM_RC0C] == 4) || ((*DimmNvList)[Dimm].rcCache[RDIMM_RC0C] == 6)) {
      WriteFnvCfgSmb (Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_RC0C_FNV_DA_UNIT_0_REG, (*DimmNvList)[Dimm].rcCache[RDIMM_RC0C]);
    } else {
      //
      // Put FMC CS_0# into loopback mode to avoid CS# got toggle when train CTL on rank0 of DDR4 but not on FMC
      //
      WriteFnvCfgSmb (Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_RC0C_FNV_DA_UNIT_0_REG, 0x2);
    }
  } else {
    //
    // Put FMC CS_0# into loopback mode to avoid CS# got toggle when train
    // non-zeon rank of DDR4 in different channel behind of same imc
    //
    WriteFnvCfgSmb (Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_RC0C_FNV_DA_UNIT_0_REG, 0x2);
  }

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
  UINT8                       Dimm;
  struct dimmNvram            (*dimmNvList)[MAX_DIMM];

  dimmNvList = GetDimmNvList(Host, Socket, Ch);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*dimmNvList)[Dimm].DcpmmPresent) {
      WriteFnvCfgSmb(Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_RC0C_FNV_DA_UNIT_0_REG, 0x2);
    }
  }
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
  DATACONTROL0N0_0_FNV_DDRIO_COMP_STRUCT fnvDataControl0;
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MaxChDdr;

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
      GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG), &fnvDataControl0.Data, 0, FNV_IO_READ, 0xF);
      fnvDataControl0.Bits.txon = value;
      GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG), &fnvDataControl0.Data, 0, FNV_IO_WRITE, 0xF);
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
  DATACONTROL0N0_0_FNV_DDRIO_COMP_STRUCT fnvDataControl0;
  struct dimmNvram                  (*dimmNvList)[MAX_DIMM];
  UINT8 MSVx4 = GetMaxStrobeValid(Host);
  UINT8 MaxChDdr;

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
      GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG), &fnvDataControl0.Data, 0, FNV_IO_READ, 0xF);
      fnvDataControl0.Bits.internalclockson = value;
      GetSetFnvIO (Host, socket, ch, dimm, UpdateIoRegisterChDdrt (Host, strobe, DATACONTROL0N0_0_FNV_DDRIO_COMP_REG), &fnvDataControl0.Data, 0, FNV_IO_WRITE, 0xF);
    } // strobe
    IODdrtResetPerCh (Host, socket, ch, dimm);
  } // ch
}

VOID
RPQDrain(
         PSYSHOST Host,
         UINT8    socket,
         UINT8    ch,
         UINT8    dimm,
         UINT8    rank
)
{
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  DN_SPARE_CTRL_FNV_D_UNIT_0_STRUCT         dSpareCtrlFnv;
  D_DDRT_TRAINING_OTHER_EN_FNV_D_UNIT_0_STRUCT DdrtTrainingOtherEn;
  BUS_TYPE                                  OriginalFmcAccessMode;

  dimmNvList = GetDimmNvList(Host, socket, ch);
  if ((*dimmNvList)[dimm].DcpmmPresent) {

    RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "\nCalling RPQ Draining Sequence\n");

    //
    // DDRT DIMMs must be accessed over SMBUS if the RPQ is full. (Other modes, such as EMRS, will fail.)
    //
    OriginalFmcAccessMode = GetFmcAccessMode (socket, ch);
    SetFmcAccessMode (socket, ch, SMBUS);

    //
    // 1. Invalidate Prefetch Cache
    //
    if ((*dimmNvList)[dimm].fmcType == FMC_FNV_TYPE) {
      dSpareCtrlFnv.Data = ReadFnvCfg (Host, socket, ch, dimm, DN_SPARE_CTRL_FNV_D_UNIT_0_REG);
      dSpareCtrlFnv.Bits.spare_1 |= BIT15;
      WriteFnvCfg (Host, socket, ch, dimm, DN_SPARE_CTRL_FNV_D_UNIT_0_REG, dSpareCtrlFnv.Data);

      dSpareCtrlFnv.Bits.spare_1 &= ~BIT15;
      WriteFnvCfg(Host, socket, ch, dimm, DN_SPARE_CTRL_FNV_D_UNIT_0_REG, dSpareCtrlFnv.Data);
    } else { // EKV
      DdrtTrainingOtherEn.Data = ReadFnvCfg (Host, socket, ch, dimm, D_DDRT_TRAINING_OTHER_EN_FNV_D_UNIT_0_REG);
      DdrtTrainingOtherEn.Bits.pche_inv_set0_only = 1;
      WriteFnvCfg (Host, socket, ch, dimm, D_DDRT_TRAINING_OTHER_EN_FNV_D_UNIT_0_REG, DdrtTrainingOtherEn.Data);
    }
    //
    // 2. Send Read commands to clear the RPQ. We will send 1 command at a time until the rpq is empty.
    //
    RPQDrainCadbMRSMode (Host, socket, ch, dimm, rank);

    //
    // Delay only if this is a CTE build, otherwise do nothing.
    //

    CteDelayQclk (450);

    ClearDdrtReqCounterMc (Host, socket, ch, dimm);

    SetFmcAccessMode (socket, ch, OriginalFmcAccessMode);
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
  DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_STRUCT            fifoTrainReset;
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
      fifoTrainReset.Data = ReadFnvCfg (Host, socket, ch, dimm, DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_REG);
      fifoTrainReset.Bits.cxfifo_trainreset = 1;
      WriteFnvCfg (Host, socket, ch, dimm, DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_REG, fifoTrainReset.Data);
      fifoTrainReset.Bits.cxfifo_trainreset = 0;
      WriteFnvCfg (Host, socket, ch, dimm, DA_CXFIFO_TRAININGRESET_FNV_DA_UNIT_1_REG, fifoTrainReset.Data);
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
  UINT8                                     ch;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, Socket);
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    ToggleBcomFlopEnPerCh (Host, Socket, ch, Value);
  }
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
  UINT8                                     Dimm;
  DA_DDRIO_CMDEN_FNV_DA_UNIT_0_STRUCT       DdrioCmdEn;
  struct dimmNvram                          (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Ch);

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if (((*DimmNvList)[Dimm].dimmPresent == 0) || ((*DimmNvList)[Dimm].DcpmmPresent == 0)) {
      continue;
    }

    DdrioCmdEn.Data = ReadFnvCfg (Host, Socket, Ch, Dimm, DA_DDRIO_CMDEN_FNV_DA_UNIT_0_REG);
    DdrioCmdEn.Bits.fnvbcomflopen = Value;
    // Use IO BCOM for Barlow Valley
    if ((*DimmNvList)[Dimm].fmcType == FMC_BWV_TYPE) {
      DdrioCmdEn.Bits.fnvbcomrden = 0;
    }
    WriteFnvCfg (Host, Socket, Ch, Dimm, DA_DDRIO_CMDEN_FNV_DA_UNIT_0_REG, DdrioCmdEn.Data);
  }
} // ToggleBcomFlopEnPerCh

VOID
ToggleBcom(PSYSHOST Host,
           UINT8    socket,
           UINT8    value
)
{
  UINT8                                     ch;
  UINT8                                     dimm;
  DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_STRUCT ddrtDqBcomCtrl;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  struct dimmNvram                          (*dimmNvList)[MAX_DIMM];
  UINT8                                     MaxChDdr;

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
      if ((*dimmNvList)[dimm].fmcType != FMC_FNV_TYPE) {
        continue;
      }

      ddrtDqBcomCtrl.Data = ReadFnvCfg(Host, socket, ch, dimm, DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_REG);
      ddrtDqBcomCtrl.Bits.en = value;
      WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_DQ_BCOM_CTRL_FNV_DA_UNIT_0_REG, ddrtDqBcomCtrl.Data);
    }
  }
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
  DA_DDRT_MPR3_PAGE0_FNV_DA_UNIT_0_STRUCT mprPage0;

  mprPage0.Bits.mpr0 = (UINT8)(pattern & 0xFF);
  mprPage0.Bits.mpr1 = (UINT8)(pattern >> 8);
  mprPage0.Bits.mpr2 = 0x0F;
  mprPage0.Bits.mpr3 = 0;
  WriteFnvCfg(Host, socket, ch, dimm, DA_DDRT_MPR3_PAGE0_FNV_DA_UNIT_0_REG, mprPage0.Data);
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
  D_DDRT_TRAINING_MPR_FNV_D_UNIT_0_STRUCT mprPage0;

  mprPage0.Bits.mpr0 = (UINT8)(pattern & 0xFF);
  mprPage0.Bits.mpr1 = (UINT8)(pattern >> 8);
  mprPage0.Bits.mpr2 = 0x0F;
  mprPage0.Bits.mpr3 = 0;
  WriteFnvCfg(Host, socket, ch, dimm, D_DDRT_TRAINING_MPR_FNV_D_UNIT_0_REG, mprPage0.Data);
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
  WriteFnvCfg (Host, socket, ch, dimm, DA_DDRT_TRAINING_MR3_FNV_DA_UNIT_0_REG, mprMode);
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
    WriteFnvCfg (Host, socket, ch, dimm, D_DDRT_TRAINING_MR3_FNV_D_UNIT_0_REG, mprMode);
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
  DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_STRUCT   daDdrtRc06;

  FixedDelayMicroSecond (500);
  daDdrtRc06.Data = 0;
  daDdrtRc06.Bits.misc_ctrl = 1;
  WriteFnvCfgSmb(Host, socket, ch, dimm, DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_REG, daDdrtRc06.Data);
  daDdrtRc06.Bits.misc_ctrl = 0;
  WriteFnvCfgSmb(Host, socket, ch, dimm, DA_DDRT_TRAINING_RC06_FNV_DA_UNIT_0_REG, daDdrtRc06.Data);

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
  MSC_TEMP_CRIT_CSR_FNV_DFX_MISC_0_STRUCT     fnvTempCrit;

  fnvTempCrit.Data = 0;
  fnvTempCrit.Bits.tempcrit = TempCrit;
  WriteFnvCfg(Host, socket, ch, dimm, MSC_TEMP_CRIT_CSR_FNV_DFX_MISC_0_REG, fnvTempCrit.Data);
}  //SetFmcTempCrit

UINT32
GetFmcCoreFreq (
                  PSYSHOST Host,
                  UINT8    Socket,
                  UINT8    Ch,
                  UINT8    Dimm
)
{
  MSC_NGN_PLL_CTRL1_FNV_DFX_MISC_0_STRUCT       mscngnpllctrl1;

  mscngnpllctrl1.Data = ReadFnvCfg(Host, Socket, Ch, Dimm, MSC_NGN_PLL_CTRL1_FNV_DFX_MISC_0_REG);
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
  DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_STRUCT    DaTrainingEn;
  D_DDRT_TRAINING_EN_FNV_D_UNIT_0_STRUCT      DTrainingEn;
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

        ReadFnvCfgSmb(Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_REG, &DaTrainingEn.Data);
        ReadFnvCfgSmb(Host, Socket, Ch, Dimm, D_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG, &DTrainingEn.Data);

        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "daTrainingEn = 0x%x, dTrainingEn = 0x%x\n", DaTrainingEn.Data, DTrainingEn.Data);

        if(RidFeedbackDis) {
          DaTrainingEn.Bits.disable_rid_feedback = 1;
          DTrainingEn.Bits.disable_rid_feedback = 1;
        } else {
          DaTrainingEn.Bits.disable_rid_feedback = 0;
          DTrainingEn.Bits.disable_rid_feedback = 0;
        }

        WriteFnvCfgSmb(Host, Socket, Ch, Dimm, DA_DDRT_TRAINING_EN_FNV_DA_UNIT_0_REG, DaTrainingEn.Data);
        WriteFnvCfgSmb(Host, Socket, Ch, Dimm, D_DDRT_TRAINING_EN_FNV_D_UNIT_0_REG, DTrainingEn.Data);
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
    A_FFW_DIGEST_CSR_0_FNV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_1_FNV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_2_FNV_A_UNIT_0_REG,
    A_FFW_DIGEST_CSR_3_FNV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_4_FNV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_5_FNV_A_UNIT_0_REG,
    A_FFW_DIGEST_CSR_6_FNV_A_UNIT_0_REG, A_FFW_DIGEST_CSR_7_FNV_A_UNIT_0_REG
  };

  UINT32 FfwDigest1Regs[] = {
    A_FFW_DIGEST1_CSR_0_FNV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_1_FNV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_2_FNV_A_UNIT_0_REG,
    A_FFW_DIGEST1_CSR_3_FNV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_4_FNV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_5_FNV_A_UNIT_0_REG,
    A_FFW_DIGEST1_CSR_6_FNV_A_UNIT_0_REG, A_FFW_DIGEST1_CSR_7_FNV_A_UNIT_0_REG
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
  DDRCRCMDCOMPOFFSET_1_FNV_DDRIO_DAT7_CH_STRUCT          DdrCrCmdCompOffset1;
  DDRCRCMDCOMPOFFSET_2_FNV_DDRIO_DAT7_CH_STRUCT          DdrCrCmdCompOffset2;
  struct channelNvram                                    (*ChannelNvList)[MAX_CH];
  struct dimmNvram                                       (*DimmNvList)[MAX_DIMM];
  UINT8                                                  MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
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
  return;
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
  UINT8       Dimm;
  BOOLEAN     Needed = FALSE;
  PSYSHOST    Host;

  Host = GetSysHostPointer ();

  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((GetFmcType (Socket, Channel) == FMC_BWV_TYPE) && (GetFmcStepping (Host, Socket, Channel, Dimm) == FmcStepA0)) {
      Needed = TRUE;
      break;
    }
  }

  return Needed;
} // DdrtRoundTripIsNeeded

 /**
   Determines if the DDRIO CMD buffers need to be tristated during DDRIO init
   based on DIMM population and stepping

   @param[in]   Host    Pointer to sysHost
   @param[in]   Socket  Socket ID
   @param[in]   Channel Channel ID

   @retval      True    The given channel requires CMD pin tristate during DDRIO init
   @retval      False   The given channel does not require CMD pin tristate during DDRIO init
 **/
BOOLEAN
EFIAPI
ChannelRequiresCmdTristate (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  return IsBwvA1SteppingAndUp (Host, Socket, Channel);
}

/**
Determines if the FMC in the given channel is BWV and the stepping is A1 and up.

@param[in]   Host    Pointer to sysHost
@param[in]   Socket  Socket ID
@param[in]   Channel Channel ID

@retval      True    The type of the FMC in the given channel is BWV and the stepping is A1 and up.
@retval      False   The type of the FMC in the given channel is NOT BWV or the stepping is not A1 and up.
**/
BOOLEAN
IsBwvA1SteppingAndUp (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  struct dimmNvram (*DimmNvList)[MAX_DIMM];
  UINT8             Dimm;

  //
  // Find the first DDRT DIMM present so we can get its revision
  //
  DimmNvList = GetDimmNvList (Host, Socket, Channel);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].DcpmmPresent != 0) {
      break;
    }
  }

  //
  // Even though multiple DDRT DIMMs may be present on a channel, it is
  // only necessary to get the revision ID for one because mixing FMC revision
  // on a channel is not allowed. This is enforced during SPD decode.
  //
  if (GetFmcStepping (Host, Socket, Channel, Dimm) < FmcStepA1) {
    return FALSE;
  }

  return TRUE;
}

/**
  Determine if clock movement should be avoided to prevent FMC from hanging.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)
  @param[in]  Channel     - Channel within the Socket

  @retval TRUE  - clock movement should be avoided
  FALSE - clock movement is allowed.

**/
BOOLEAN
FmcAvoidClockMovementInChannel (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  struct channelNvram     (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  if ((*ChannelNvList)[Channel].ddrtEnabled == 0) {
    return FALSE;
  }

  // BWV A0 stepping should avoid clock movement. A1 stepping and up have silicon fix.
  if (IsBwvA1SteppingAndUp (Host, Socket, Channel) == FALSE) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
  Determine if clock movement should be avoided to prevent FMC from hanging.

  @param[in]  Host        - Pointer to sysHost, the system Host (root) structure struct
  @param[in]  Socket      - Processor socket within the system (0-based)

  @retval TRUE  - clock movement should be avoided
  FALSE - clock movement is allowed.

**/
BOOLEAN
FmcAvoidClockMovementInSocket (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  UINT8 Ch;
  UINT8 MaxChDdr;
  struct channelNvram     (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  MaxChDdr = GetMaxChDdr ();
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    if ((*ChannelNvList)[Ch].ddrtEnabled == 0) {
      continue;
    }

    if (FmcAvoidClockMovementInChannel (Host, Socket, Ch)) {
      return TRUE;
    }
  }

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
  struct dimmNvram (*DimmNvList)[MAX_DIMM];

  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  if (((*DimmNvList)[Dimm].fmcType == FMC_BWV_TYPE) && (GetFmcStepping (Host, Socket, Channel, Dimm) < FmcStepA1)) {
    return TRUE;
  }

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
  struct channelNvram     (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  //No DDRT, then no frequency restriction
  if ((*ChannelNvList)[Channel].ddrtEnabled == 0) {
    return FALSE;
  }

  //A1 and Higher stepping, then no frequency restriction
  if (IsBwvA1SteppingAndUp (Host, Socket, Channel) == TRUE) {
    return FALSE;
  }

  //All other case, enforce frequency restriction
  return TRUE;
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
  return FALSE;
}

/**
  Program direct snoop settings to DQ buffer

  @param[in] Host        - Pointer to sysHost
  @param[in] Socket      - Socket Id
  @param[in] Ch          - Ch number
  @param[in] SubChMask   - Mask of subchannel numbers to be updated
  @param[in] Dimm        - Dimm number (zero-based)
  @param[in] Rank        - Rank number (zero-based)
  @param[in] Address     - Address for the MRS write
  @param[in] Cw          - Data to write

  @retval none
**/
VOID
EFIAPI
ProgramSnoopSettingsToDb (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      Ch,
  IN UINT8      SubChMask,
  IN UINT8      Dimm,
  IN UINT8      Rank,
  IN UINT8      Address,
  IN UINT8      Data
  )
{
  // Null
}