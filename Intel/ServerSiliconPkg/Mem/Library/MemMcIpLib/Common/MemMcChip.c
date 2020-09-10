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
#include <Memory/CpgcDefinitions.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <UncoreCommonIncludes.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemoryServicesLib.h>

#include <Library/MemMcIpLib.h>
#include <Library/MemRcLib.h>
#include "Include/MemMcRegs.h"
#include <Library/EmulationConfigurationLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include "Include/MemMcIpLibInternal.h"
#include <Library/MemAccessLib.h>

/**

  This function gets the Target Socket and Channel interleave values

  @param Host    - Pointer to sysHost
  @param socket  - Socket
  @param ch      - Channel
  @param *TargetIntlv
  @param *ChannelIntlv

  @retval none

**/
VOID
GetNmCachingIntlvValues (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT8     Ch,
  UINT8     *TargetIntlv,
  UINT8     *ChannelIntlv
)
{
  MCNMCACHINGINTLV_MC_2LM_STRUCT mcNMCachingIntlv10nm;

  mcNMCachingIntlv10nm.Data = MemReadPciCfgEp (Socket, Ch, MCNMCACHINGINTLV_MC_2LM_REG);
  *TargetIntlv = (UINT8)mcNMCachingIntlv10nm.Bits.nm_target_ways;
  *ChannelIntlv = (UINT8)mcNMCachingIntlv10nm.Bits.nm_chn_ways;
}

/**

  Initialize the DDR4 and DDRT scrambler config in the MC

  @param[in]  Host          - Point to sysHost
  @param[in]  Socket        - Socket number
  @param[in]  Ch            - Channel number
  @param[in]  Context       - Indicates what action to take

  @retval n/a

**/
VOID
SetScrambleConfigMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Context
  )
{
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT    scrambleConfig;
  struct channelNvram                 (*channelNvList)[MAX_CH];

  channelNvList = GetChannelNvList(Host, Socket);

  scrambleConfig.Data = MemReadPciCfgEp (Socket, Ch, MCSCRAMBLECONFIG_MCDDC_DP_REG);
  scrambleConfig.Bits.rx_enable_ddrt = 1;
  // 4928558  Cloned From SKX Si Bug Eco: NVMDIMM Write Scrambling in 2N mode causes NVMCTLR ERROR
  if ((*channelNvList)[Ch].timingMode != TIMING_2N) {
    scrambleConfig.Bits.tx_enable_ddrt = 1;
  } else {
    scrambleConfig.Bits.tx_enable_ddrt = 0;
  }
  scrambleConfig.Bits.ch_enable_ddrt = 1;
  MemWritePciCfgEp (Socket, Ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, scrambleConfig.Data);
}

/**
  Update MCDDRTCFG slot present

  @param[in]  Host    Pointer to SysHost

  @retval     None
**/
VOID
EFIAPI
SetMcDdrtCfg (
  IN  PSYSHOST  Host
  )
{
  UINT8                     Socket;
  UINT8                     Channel;
  UINT8                     Dimm;
  MCDDRTCFG_MC_MAIN_STRUCT  McDdrtCfg;
  UINT8                     MaxChDdr;

  if (!(UbiosGenerationOrHsleEnabled ())) {
    return;
  }

  Socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();

  for (Channel = 0; Channel < MaxChDdr; Channel++) {
    McDdrtCfg.Data = MemReadPciCfgEp (Socket, Channel, MCDDRTCFG_MC_MAIN_REG);
    McDdrtCfg.Bits.slot0 = 0;
    McDdrtCfg.Bits.slot1 = 0;

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

      if (IsDcpmmPresentDimm (Socket, Channel, Dimm) == TRUE) {

        if (Dimm == 0) {
          McDdrtCfg.Bits.slot0 = 1;
        } else if (Dimm == 1) {
          McDdrtCfg.Bits.slot1 = 1;
        } else {
          RcDebugPrintWithDevice (SDBG_ERROR, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "\nInvalid DIMM slot encountered during SetMcDdrtCfg");
          ASSERT (FALSE);
        }
      }

      MemWritePciCfgEp (Socket, Channel, MCDDRTCFG_MC_MAIN_REG, McDdrtCfg.Data);
      IO_Reset (Host, Socket);
    }
  }
}

/**

  Memory controller initialization required after training

  @param[in] MemTechType  - Memory technology type
  @param[in] Socket       - Socket number

  @retval N/A

**/
VOID
EFIAPI
PostTrainingInitMc (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          Socket
  )
{
  UINT8                 Ch;
  AMAP_MC_MAIN_STRUCT   ImcAmapReg;
  SYS_SETUP             *Setup;
  UINT8                 MaxCh;

  MaxCh = GetMaxChPerTechType (MemTechType);
  Setup = GetSysSetupPointer ();

  //
  // Program fine grain bank group interleaving mode
  //
  for (Ch = 0; Ch < MaxCh; Ch++) {
    if (!IsChEnabledPerTechType (MemTechType, Socket, Ch)) {
      continue;
    }

    ImcAmapReg.Data = ChRegisterRead (MemTechType, Socket, Ch, AMAP_MC_MAIN_REG);
    ImcAmapReg.Bits.cgbg_interleave = Setup->mem.dfxMemSetup.DfxBankGroupInterleavingMode;
    ChRegisterWrite (MemTechType, Socket, Ch, AMAP_MC_MAIN_REG, ImcAmapReg.Data);
  }
  return;
} // PostTrainingInitMc

/**

  Set the IO latency register value in the memory controller

  @param[in] Socket          - Socket numer
  @param[in] Ch              - Channel number (0-based)
  @param[in] Rank            - Rank number (0-based)
  @param[in] IoLatencyValue  - New IO Latency value

  @retval EFI_SUCCESS if value found
          EFI_UNSUPPORTED if the function is not supported
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
SetIoLatencyMc (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    IoLatencyValue
  )
{
  //
  // Stub function to satisfy the library class API,
  // should not be called in 10nm. Needs investigation
  // as to whether it should be in the library class.
  //

  RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_27);

  return EFI_UNSUPPORTED;

} // SetIoLatencyMc

/**
  Get the IO latency value for a single rank

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0 >= Ch < MaxChDdr)
  @param[in]  Rank            - Rank number (0 >= Rank < MAX_RANK_CH)
  @param[in]  Mode            - Bit field of access modes
  @param[out] IoLatencyValue  - Pointer to caller's IO Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetIoLatencyMc (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *IoLatencyValue
  )
{
  //
  // Stub function to satisfy the library class API,
  // should not be called in 10nm. Needs investigation
  // as to whether it should be in the library class.
  //

  RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_27);

  return 0;

} // GetIoLatencyMc

/**

  Set the round trip register value in the memory controller

  @param[in] Socket          - Socket number
  @param[in] Ch              - Channel number (0-based)
  @param[in] Rank            - Rank number (0-based)
  @param[in] RoundTripValue  - New Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_UNSUPPORTED if the function is not supported
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
SetRoundTripMc (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    RoundTripValue
  )
{
#ifndef GNRSRF_HOST
  UINT8                                     Dimm;
  T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_STRUCT   DdrtDimm0BasicTiming;
  T_DDRT_DIMM1_BASIC_TIMING_MC_2LM_STRUCT   DdrtDimm1BasicTiming;
  SYSHOST                                   *Host;
  struct dimmNvram                          (*DimmNvList)[MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = &Host->nvram.mem.socket[Socket].channelList[Ch].dimmList;

  Dimm = (Rank / MAX_RANK_DIMM);

  if ((*DimmNvList)[Dimm].DcpmmPresent == 1) {
    // Note that the t_ddrt_tcl bit field is not used by RTL, but we are programming it to
    // the roundtrip latency value to aid in debug (per validation request)
    if (Dimm == 0) {
      DdrtDimm0BasicTiming.Data = MemReadPciCfgEp (Socket, Ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG);
      DdrtDimm0BasicTiming.Bits.t_ddrt_tcl = RoundTripValue;
      MemWritePciCfgEp (Socket, Ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG, DdrtDimm0BasicTiming.Data);
    } else {
      DdrtDimm1BasicTiming.Data = MemReadPciCfgEp (Socket, Ch, T_DDRT_DIMM1_BASIC_TIMING_MC_2LM_REG);
      DdrtDimm1BasicTiming.Bits.t_ddrt_tcl = RoundTripValue;
      MemWritePciCfgEp (Socket, Ch, T_DDRT_DIMM1_BASIC_TIMING_MC_2LM_REG, DdrtDimm1BasicTiming.Data);
    }
  }
#endif // !GNRSRF_HOST
  return EFI_SUCCESS;

} // SetRoundTripMc

/**
  Get the round trip value

  @param[in]  Socket          - Socket number
  @param[in]  Ch              - Channel number (0-based)
  @param[in]  Rank            - Rank number (0-based)
  @param[in]  Mode            - Bit field of access modes
  @param[out] RoundTripValue  - Pointer to caller's Round Trip Latency value

  @retval EFI_SUCCESS if value found
          EFI_INVALID_PARAMETER otherwise

**/
EFI_STATUS
GetRoundTripMc (
  IN    UINT8    Socket,
  IN    UINT8    Ch,
  IN    UINT8    Rank,
  IN    UINT8    Mode,
  OUT   UINT8    *RoundTripValue
  )
{
  //
  // Stub function to satisfy the library class API,
  // should not be called in 10nm. Needs investigation
  // as to whether it should be in the library class.
  //

  RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_27);

  return 0;

} // GetRoundTripMc

/**

This function applies the given CKE override to the bits specified by ckeMask

@param Host      - Pointer to sysHost
@param socket    - Socket
@param ch        - Channel
@param ckeMask   - CKE settings to apply
@param assertCke - Flag to ASSERT/DEASERT CKE

@retval none

**/
VOID
SetCkeMask (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     ckeMask,
  UINT8     assertCke
  )
{
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT  miscCKECtl;

  // Override CKE
  miscCKECtl.Data = MemReadPciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG);
  miscCKECtl.Bits.cke_override |= (UINT32) ckeMask; //Keep all the prev overrides and enable current overrides also.
  if (assertCke){
    miscCKECtl.Bits.cke_on |= (UINT32) ckeMask; //Keep all the prev overrides and enable current overrides also.
  } else {
    miscCKECtl.Bits.cke_on &= (UINT32) ~ckeMask;
  }

  MemWritePciCfgEp (socket, ch, CPGC_MISCCKECTL_MCDDC_CTL_REG, miscCKECtl.Data);
} // SetCkeMask

/**

  Overrides CKE value

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Mask                Mask to get overwritten (Bitwise)
  @param[in]                Value               Specify value for CKE (Bitwise)

  @retval                   EFI_STATUS          This function completed execution with no warnings/errors
**/
EFI_STATUS
EFIAPI
OverrideCke (
  IN        UINT8           Socket,
  IN        UINT8           Channel,
  IN        UINT32          Mask,
  IN        UINT32          Value
  )
{
  PSYSHOST                          Host;
  CPGC_MISCCKECTL_MCDDC_CTL_STRUCT  MiscCkeCtl;

  Host = GetSysHostPointer ();
  if (Host->DdrioUltSupport) {
    if (Value == 0) {
      CteDdrCmd ("pde", 1 << Channel, 3, Mask, 0, 0);
    } else {
      CteDdrCmd ("pdx", 1 << Channel, 3, Mask, 0, 0);
    }
  } else {
    if (!IsBrsPresent (Host, Socket)) {
      MiscCkeCtl.Data = MemReadPciCfgEp (Socket, Channel, CPGC_MISCCKECTL_MCDDC_CTL_REG);
      MiscCkeCtl.Bits.cke_override = Mask;
      MiscCkeCtl.Bits.cke_on = Value;
      MemWritePciCfgEp (Socket, Channel, CPGC_MISCCKECTL_MCDDC_CTL_REG, MiscCkeCtl.Data);
    }
  }
  return EFI_SUCCESS;
}
