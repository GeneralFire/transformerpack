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
#include "MemCommon.h"
#include "HbmDefaults.h"
#include "Include/MemMcRegs.h"
#include "Include/MemMcIpLibInternal.h"
#include <Library/MemRcLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemAccessLib.h>
#include <Library/HbmCoreLib.h>
#include <Library/TimerLib.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Guid/SocketCommonRcVariable.h>

/**

  Set HBM MC Mode

  @param[in]                Socket             Current Socket (0-Based)
  @param[in]                HbmCh              Current HBM Ch (0-Based)

**/
VOID
EFIAPI
HbmSetMcMode (
  IN        UINT8           Socket,
  IN        UINT8           HbmCh
  )
{
  PSYSHOST                            Host;
  MCMTR_MC_MAIN_STRUCT                McMtr;
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT MemoryTimingsAdj;

  Host = (PSYSHOST) GetSysHostPointer();

  McMtr.Data = ChRegisterRead (MemTechHbm, Socket, HbmCh, MCMTR_MC_MAIN_REG);

  //
  // Enable ECC if required
  //
  if (Host->nvram.mem.eccEn) {
    McMtr.Bits.ecc_en = 1;
  }

  //
  // Set HBM MC Mode
  //
  McMtr.Bits.hbm_mc_mode = 1;

  ChRegisterWrite (MemTechHbm, Socket, HbmCh, MCMTR_MC_MAIN_REG, McMtr.Data);

  MemoryTimingsAdj.Data = ChRegisterRead (MemTechHbm, Socket, HbmCh, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
  MemoryTimingsAdj.Bits.dram_device_type = 1; //HBM2E
  ChRegisterWrite (MemTechHbm, Socket, HbmCh, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, MemoryTimingsAdj.Data);

} // HbmSetMcMode

/**

  Initialize HBM MC page policy and Timing Parameters

  @param[in]                Socket              Current Socket (0-Based)
  @param[in]                Ch             Current Ch (0-Based)

**/
VOID
EFIAPI
HbmInitPagePolicy (
  IN        UINT8           Socket,
  IN        UINT8           Ch
  )
{
  SCHEDULER_IDLETIME_MCDDC_CTL_STRUCT   IdleTime;
  SCHEDULER_IDLETIME2_MCDDC_CTL_STRUCT  IdleTime2;

  IdleTime.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_IDLETIME_MCDDC_CTL_REG);
  IdleTime2.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_IDLETIME2_MCDDC_CTL_REG);

  IdleTime.Bits.ppc_th = HBM_PPC_TH;
  IdleTime.Bits.opc_th = HBM_OPC_TH;

  IdleTime2.Bits.idle_page_rst_val = HBM_IDLE_PAGE_RST_VAL;
  IdleTime2.Bits.win_size = HBM_WIN_SIZE;
  IdleTime2.Bits.adapt_pg_clse = 0;
  IdleTime2.Bits.opp_pg_en = 1;
  IdleTime2.Bits.idle_page_max_limit_en = 0;

  ChRegisterWrite (MemTechHbm, Socket, Ch,  SCHEDULER_IDLETIME_MCDDC_CTL_REG, IdleTime.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_IDLETIME2_MCDDC_CTL_REG, IdleTime2.Data);
} // HbmInitPagePolicy

/**

  Initialize HBM MC page policy

  @param[in]                Socket         Current Socket (0-Based)
  @param[in]                Ch             Current Ch (0-Based)
  @param[in]                EnableDbi      BOOLEAN, to enable DBI or disable it

**/
VOID
EFIAPI
HbmInitDbi (
  IN        UINT8           Socket,
  IN        UINT8           Ch,
  IN        BOOLEAN         EnableDbi
  )
{

  MCMTR_MC_MAIN_STRUCT    McMtr;
  McMtr.Data = ChRegisterRead (MemTechHbm, Socket, Ch, MCMTR_MC_MAIN_REG);

  if (EnableDbi) {
    McMtr.Bits.write_dbi_en = 1;
    McMtr.Bits.read_dbi_en  = 1;
  } else {
    McMtr.Bits.write_dbi_en = 0;
    McMtr.Bits.read_dbi_en  = 0;
  }

  if (IsEmulation ()) {
    McMtr.Bits.write_dbi_en = 0;
  }

  ChRegisterWrite (MemTechHbm, Socket, Ch, MCMTR_MC_MAIN_REG, McMtr.Data);
}

/**
  This function programs HBM MC registers required after training and before normal mode.

  @param[in] SocketId                     Socket index.
  @param[in] ChId                         Channel index within socket.

  @retval MRC_STATUS_SUCCESS              This function is executed successfully.
  @retval MRC_STATUS_UNSUPPORTED          Register programming is not supported.
  @retval MRC_STATUS_INVALID_PARAMETER    Some of the input parameters are invalid.
**/
MRC_STATUS
EFIAPI
HbmMcLateConfig (
  IN UINT8     SocketId,
  IN UINT8     ChId
  )
{
  AMAP_MC_MAIN_STRUCT                                     AmapReg;
  MEMORY_TIMINGS_BANK_IDLE_BYPASS_MCDDC_CTL_STRUCT        IdleBypassReg;
  MEMORY_TIMINGS_BANK_TCL_TWL_MCDDC_CTL_STRUCT            TclTwlReg;
  MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_STRUCT               TrcdReg;
  MEMORY_TIMINGS_BANK_TRP_TRC_TRAS_MCDDC_CTL_STRUCT       TrpTrcTrasReg;
  MEMORY_TIMINGS_BANK_TRTP_TWR_MCDDC_CTL_STRUCT           TrtpTwrReg;
  MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_STRUCT   TrdaTwraTwrpreReg;

  if ((SocketId >= MAX_SOCKET) || (ChId >= GetMaxChHbm ())) {
    return MRC_STATUS_INVALID_PARAMETER;
  }

  //
  // AMAP_MC_MAIN_STRUCT register programming
  //
  AmapReg.Data = ChRegisterRead (MemTechHbm, SocketId, ChId, AMAP_MC_MAIN_REG);
  AmapReg.Bits.fast_decode = 0;
  AmapReg.Bits.wait4bothhalves = 1;
  ChRegisterWrite (MemTechHbm, SocketId, ChId, AMAP_MC_MAIN_REG, AmapReg.Data);

  if (FixedPcdGetBool (PcdUseEmulationInjection)) {
    //
    // Memory Timing Bank Injection: IDLE_BYPASS
    //
    IdleBypassReg.Data = ChRegisterRead (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_IDLE_BYPASS_MCDDC_CTL_REG);
    IdleBypassReg.Bits.t_ras_bypass    = 0x28;
    IdleBypassReg.Bits.t_rc_bypass     = 0x40;
    IdleBypassReg.Bits.t_rcd_rd_bypass = 0x22;
    IdleBypassReg.Bits.t_rcd_wr_bypass = 0x39;
    ChRegisterWrite (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_IDLE_BYPASS_MCDDC_CTL_REG, IdleBypassReg.Data);

    //
    // Memory Timing Bank Injection: TCL_TWL
    //
    TclTwlReg.Data = ChRegisterRead (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_TCL_TWL_MCDDC_CTL_REG);
    TclTwlReg.Bits.t_cl = 0x1C;
    TclTwlReg.Bits.t_wl = 0x13;
    ChRegisterWrite (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_TCL_TWL_MCDDC_CTL_REG, TclTwlReg.Data);

    //
    // Memory Timing Bank Injection: TRCD
    //
    TrcdReg.Data = ChRegisterRead (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_REG);
    TrcdReg.Bits.t_rcd_imprd = 0x3F;
    TrcdReg.Bits.t_rcd_impwr = 0x56;
    TrcdReg.Bits.t_rcd_rd    = 0x25;
    TrcdReg.Bits.t_rcd_wr    = 0x3C;
    ChRegisterWrite (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_REG, TrcdReg.Data);

    //
    // Memory Timing Bank Injection: TRP_TRC_TRAS
    //
    TrpTrcTrasReg.Data = ChRegisterRead (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_TRP_TRC_TRAS_MCDDC_CTL_REG);
    TrpTrcTrasReg.Bits.t_ras = 0x29;
    TrpTrcTrasReg.Bits.t_rc  = 0x43;
    TrpTrcTrasReg.Bits.t_rp  = 0x18;
    ChRegisterWrite (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_TRP_TRC_TRAS_MCDDC_CTL_REG, TrpTrcTrasReg.Data);

    //
    // Memory Timing Bank Injection: TRTP_TWR
    //
    TrtpTwrReg.Data = ChRegisterRead (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_TRTP_TWR_MCDDC_CTL_REG);
    TrtpTwrReg.Bits.t_rtp = 0x9;
    TrtpTwrReg.Bits.t_wr  = 0x19;
    ChRegisterWrite (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_TRTP_TWR_MCDDC_CTL_REG, TrtpTwrReg.Data);

    //
    // Memory Timing Bank Injection: TRDA_TWRA_TWRPRE
    //
    TrdaTwraTwrpreReg.Data = ChRegisterRead (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_REG);
    TrdaTwraTwrpreReg.Bits.t_rda    = 0x21;
    TrdaTwraTwrpreReg.Bits.t_wr_rda = 0x22;
    TrdaTwraTwrpreReg.Bits.t_wra    = 0x43;
    TrdaTwraTwrpreReg.Bits.t_wrpre  = 0x2B;
    ChRegisterWrite (MemTechHbm, SocketId, ChId, MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_REG, TrdaTwraTwrpreReg.Data);
  }

  return MRC_STATUS_SUCCESS;
}

/**

  Program HBM MC into normal mode

  @param[in] Socket                 Socket index (0-Based).
  @param[in] Ch                     Channel index within Socket (0-Based).

  @retval EFI_SUCCESS               HBM MC were programmed into normal mode.
  @retval EFI_UNSUPPORTED           Register programming for normal mode is unsupported.
  @retval EFI_INVALID_PARAMETER     Some of input parameters are invalid.

**/
EFI_STATUS
EFIAPI
HbmMcSetNormalMode (
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  MCMTR_MC_MAIN_STRUCT    McMtr;

  if ((Socket >= MAX_SOCKET) || (Ch >= GetMaxChHbm ())) {
    return EFI_INVALID_PARAMETER;
  }

  McMtr.Data = ChRegisterRead (MemTechHbm, Socket, Ch, MCMTR_MC_MAIN_REG);

  //
  // Enter normal mode
  //
  McMtr.Bits.normal     = 1;
  McMtr.Bits.trng_mode  = 3;

  //
  // Set HBM MC XOR mode
  //
  McMtr.Bits.channel_xor_enable = 1;
  if (GetUmaBasedClustering () != UMABASEDCLUSTERING_DISABLED) {
    McMtr.Bits.cluster_xor_enable = 1;
  }

  ChRegisterWrite (MemTechHbm, Socket, Ch, MCMTR_MC_MAIN_REG, McMtr.Data);

  return EFI_SUCCESS;
}

/**

  Program HBM DIMM MTR register

  @param[in]   Socket    Current Socket (0-Based)

  @retval  EFI_SUCCESS             This function completed execution with no warnings/errors
  @retval  EFI_INVALID_PARAMETER   Invalid density detected on one of HBM IO modules.

**/
EFI_STATUS
EFIAPI
HbmDimmMtrConfig (
  IN        UINT8           Socket
  )
{
  DEVICE_ID_WDR               *DeviceIdData;
  EFI_STATUS                  Status = EFI_SUCCESS;
  UINT8                       IoModule;
  UINT8                       Channel;
  UINT8                       StartChannel;
  UINT8                       EndChannel;
  DIMMMTR_0_MC_MAIN_STRUCT    DimmMtrStruct;
  UINT32                      DimmMtrRegOffset;
  HBM_DENSITY                 HbmDensity;
  UINT8                       AddressingMode;
  DIMMMTR_DATA_STRUCT         DimmMtr;

  for (IoModule = 0; IoModule < GetMaxIoHbm (); IoModule++) {
    if (!IsHbmMemSsEnabled (Socket, IoModule)) {
      continue;
    }

    DeviceIdData = GetHbmDeviceId (Socket, IoModule);
    HbmDensity = DeviceIdData->Bits.density;
    AddressingMode = (UINT8) DeviceIdData->Bits.addresing_mode;

    Status = HbmGetDimmMtr (HbmDensity, AddressingMode, &DimmMtr);
    if (EFI_ERROR (Status)) {
      RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "ERROR: Invalid density: %d\n", HbmDensity);
      return EFI_INVALID_PARAMETER;
    }

    StartChannel = IoModule * GetNumChannelPerIoHbm ();
    EndChannel   = StartChannel + GetNumChannelPerIoHbm ();

    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, StartChannel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "HBM Density: %d  ", HbmDensity);
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, StartChannel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Column Address width: %d; ", DimmMtr.Bits.ca_width);
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, StartChannel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Row Address width: %d; ", DimmMtr.Bits.ra_width);
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, StartChannel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Number of banks: %d\n", DimmMtr.Bits.number_of_banks);

    for (Channel = StartChannel; Channel < EndChannel; Channel++) {
      if (!IsHbmChannelEnabled (Socket, Channel)) {
        continue;
      }

      DimmMtrRegOffset = GetDimmMtrRegOffset (0);

      DimmMtrStruct.Data = ChRegisterRead (MemTechHbm, Socket, Channel, DimmMtrRegOffset);

      DimmMtrStruct.Bits.ca_width = DimmMtr.Bits.ca_width;
      DimmMtrStruct.Bits.ra_width = DimmMtr.Bits.ra_width;
      DimmMtrStruct.Bits.dimm_pop = 1;

      if (HbmDensity == Hbm16Gb8H) {
        DimmMtrStruct.Bits.ddr3_dnsty = DDR3_DNSTY_16Gb;
        DimmMtrStruct.Bits.rank_disable = RANK_DISABLE_NO;
        DimmMtrStruct.Bits.rank_cnt = RANK_CNT_QR;
        DimmMtrStruct.Bits.ba_shrink = BA_SHRINK_NORMAL;
        DimmMtrStruct.Bits.ddr4_3dsnumranks_cs = SUBRANK_CS_2R;
      } else {
        RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Channel, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "ERROR: Only support 16Gb8H, it is unsupported density: %d\n", HbmDensity);
      }

      ChRegisterWrite (MemTechHbm, Socket, Channel, DimmMtrRegOffset, DimmMtrStruct.Data);
    } // Channel
  } // IoModule

  return Status;
}

/**
  This function toggles the reset_io register field.

  @param[in] SocketId             Socket index.

  @retval MRC_STATUS_SUCCESS             This function is executed successfully.
  @retval MRC_STATUS_UNSUPPORTED         The toggling of reset_io is not supported.
  @retval MRC_STATUS_INVALID_PARAMETER   Some of the input parameters are invalid.
**/
MRC_STATUS
EFIAPI
HbmMcSetResetIo (
  IN UINT8   SocketId
  )
{
  UINT8      ChId;
  UINT8      MaxCh;
  UINT16     DelayNanoSec;
  MC_INIT_STATE_G_MC_MAIN_STRUCT   McInitStateG;

  if (SocketId >= MAX_SOCKET) {
    return MRC_STATUS_INVALID_PARAMETER;
  }

  RcDebugPrintWithDevice (SDBG_INFO, SocketId, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "HBM IO Reset Starts\n");

  MaxCh = GetMaxChHbm ();
  DelayNanoSec = (20 * 1000) / (HbmGetFrequencyInMhz (SocketId) / 2);  // 20 DCLK delay in nanosecond.

  //
  // Reset HBM IO.
  //
  for (ChId = 0; ChId < MaxCh; ChId++) {
    if (!IsHbmChannelEnabled (SocketId, ChId)) {
      continue;
    }
    McInitStateG.Data = ChRegisterRead (MemTechHbm, SocketId, ChId, MC_INIT_STATE_G_MC_MAIN_REG);
    McInitStateG.Bits.reset_io = 1;
    ChRegisterWrite (MemTechHbm, SocketId, ChId, MC_INIT_STATE_G_MC_MAIN_REG, McInitStateG.Data);
  }

  //
  // Wait for reset high.
  //
  NanoSecondDelay (DelayNanoSec);

  //
  // Clear HBM IO Reset.
  //
  for (ChId = 0; ChId < MaxCh; ChId++) {
    if (!IsHbmChannelEnabled (SocketId, ChId)) {
      continue;
    }
    McInitStateG.Data = ChRegisterRead (MemTechHbm, SocketId, ChId, MC_INIT_STATE_G_MC_MAIN_REG);
    McInitStateG.Bits.reset_io = 0;
    ChRegisterWrite (MemTechHbm, SocketId, ChId, MC_INIT_STATE_G_MC_MAIN_REG, McInitStateG.Data);
  }

  //
  // Wait again for stable IO.
  //
  NanoSecondDelay (DelayNanoSec);

  RcDebugPrintWithDevice (SDBG_INFO, SocketId, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "HBM IO Reset Done\n");

  return MRC_STATUS_SUCCESS;
}

/**

  Configures registers for Memory hot feature

  @param[in] Socket  - Socket Id, 0 based

  @retval N/A

**/
VOID
HbmConfigureMemHotFeature (
  IN UINT8 Socket
  )
{
  UINT8      Ch;
  UINT8      Dimm;
  UINT8      MaxChHbm;
  SYS_SETUP  *Setup;

  MaxChHbm = GetMaxChHbm ();
  Setup = GetSysSetupPointer ();

  //
  // Configure MEMHOT, it is already programmed in function ConfigureMemHotFeature
  //
  //ProgramMemHotFeature (Socket, Setup->mem.thermalThrottlingOptions);

  for (Ch = 0; Ch < MaxChHbm; Ch++) {
    if (!IsHbmChannelEnabled (Socket, Ch)) {
      continue;
    }

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      SetMemHotTemperatureRange (MemTechHbm, Socket, Ch, Dimm, Setup->mem.memhotOutputOnlyOpt);
    } // Dimm
  } // Ch

  return;
} // ConfigureMemHotFeature
