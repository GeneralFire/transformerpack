/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <MemCommon.h>
#include <Library/HbmCoreLib.h>
#include <Library/SystemInfoLib.h>
#include "Include/MemMcRegs.h"
#include "Chip10nm/Include/MemHostChip10nm.h"
#include "Include/MemMcIpLibInternal.h"
#include <Library/HbmCoreLib.h>
#include <MemHostChipCommon.h>
#include <Library/BaseHbmCoreLib/HbmCoreHost.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/RcDebugLib.h>
#include <Library/MemAccessLib.h>

//
// HBM Mc refresh settings
//
#define ENABLE_6REF_WINDOW_HBM  0
#define REF_PANIC_WM_HBM        5
#define REF_HI_WM_HBM           4
#define MAX_CHANNEL_REF_HBM     0x6
#define OREFNI_HBM              0x3f
#define T_REF_BLOCK_HBM         0x0b
#define T_REFI_HBM_2P8          5460
#define T_REFI_HBM_3P2          6240

#define TRFCSB_HBM_2P8          280
#define TRFCSB_HBM_3P2          320
#define TRREFD_HBM_2P8          12
#define TRREFD_HBM_3P2          13

#define STAGGER_REF_EN_HBM          1
#define DIS_OPP_REF_HBM             1
#define DIS_REFRESH_START_DELAY_HBM 1

#define PERBANK_MODE_HBM              1
#define NO_BANK_OVERLAP_HBM           0
#define KEEP_CKE_HIGH_DURING_TRFC_HBM 0

//
// HBM Refresh command period tRFC in nano seconds per HBM DRAM Specification (Rev 2.60r6)
//
#define T_RFC_1GB  110
#define T_RFC_2GB  160
#define T_RFC_4GB  260
#define T_RFC_8GB  350
#define T_RFC_16GB 450

//
// HBM Page Policy and Timing parameters
//
#define IDLE_PAGE_RST_VAL_HBM       0x200

//
// HBM Performance settings
//
#define OPP_TIMER                   0xF
#define OPP_LIMIT                   0xF
#define OPP_TIMER_GRAN              0x2
#define OPP_LIMIT_GRAN              0x0

#define CAS_STARVE_COUNT_HBM        0x8
#define CAS_CRITICAL_COUNT_HBM      0x8

#define READ_WAIT_COUNT_HBM         0x40
#define READ_STARVE_COUNT_HBM       0x80

#define WRITE_WAIT_COUNT_HBM        0x80
#define WRITE_STARVE_COUNT_HBM      0x180

#define WMM_ENTRY_HBM               0x1B
#define WMM_EXIT_HBM                0xE


/**

  Return values for MC JEDEC parameter register offsets
  These are the offsets applied to JEDEC parameters before programming the register.

  @param[in]  Host                - Pointer to sysHost
  @param[in]  Socket              - Socket number
  @param[in]  MemTechType         - Memory Technology Type
  @param[out] JedecTimingOffsets  - Pointer to caller's buffer pointer
                                    Updated by this function to pointer to the idle bypass offsets table

  @retval EFI_SUCCESS if values are supported
          EFI_UNSUPPORTED otherwise

**/
EFI_STATUS
GetJedecTimingOffsets (
  IN  PSYSHOST              Host,
  IN  UINT8                 Socket,
  IN  MEM_TECH_TYPE         MemTechType,
  OUT JEDEC_TIMING_OFFSETS  **JedecTimingOffsets
  );

/**

  Obtain HBM refresh command period tRFC in units of Dclk cycles

  @param[in] Socket          Socket Id
  @param[in]  Density        HBM density, see HBM_DENSITY enum.
  @param[out] Trfc           Pointer to tRFC in Dclk cycles.

  @retval EFI_SUCCESS            HBM Density is supported and tRFC is returned.
  @retval EFI_INVALID_PARAMETER  HBM Density is not supported.

**/
EFI_STATUS
HbmGetTrfc (
  IN        UINT8           Socket,
  IN        UINT8           Density,
      OUT   UINT16          *Trfc
  )
{
  UINT16   TrfcNanoSeconds;
  *Trfc = 0;

  switch (Density) {
  case Hbm1Gb:  // 1 Gb
    TrfcNanoSeconds = T_RFC_1GB;
    break;

  case Hbm2Gb:  // 2 Gb
    TrfcNanoSeconds = T_RFC_2GB;
    break;

  case Hbm4Gb:  // 4 Gb
    TrfcNanoSeconds = T_RFC_4GB;
    break;

  case Hbm6Gb:  // 6 Gb
    TrfcNanoSeconds = T_RFC_4GB;  // As the Actual values for 6 Gb not available, keeping it same as 4 Gb
    break;

  case Hbm8Gb8H:  // 8 Gb
  case Hbm8Gb:
    TrfcNanoSeconds = T_RFC_8GB;
    break;

  case Hbm12Gb8H:  // 12 Gb
  case Hbm8Gb12H:
    TrfcNanoSeconds = T_RFC_8GB;// keeping it same as 12 Gb
    break;

  case Hbm16Gb8H:  // 16 Gb
    TrfcNanoSeconds = T_RFC_16GB;
    break;

  case Hbm12Gb12H: // 18 Gb
    TrfcNanoSeconds = T_RFC_16GB;// keeping it same as 18 Gb
    break;

  case Hbm16Gb12H:  // 24 Gb
    TrfcNanoSeconds = T_RFC_16GB;// keeping it same as 24 Gb
    break;

  default:
    return EFI_INVALID_PARAMETER;
    break;
  }

  *Trfc = (1000 * TrfcNanoSeconds) / HbmGettCK (Socket);

  return EFI_SUCCESS;
}

/**

  Check if HBM is HBM2 8H configurations with more than 2 ranks

  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number

  @retval TRUE   - Yes
          FALSE  - No

**/
BOOLEAN
IsHbmMoreThanTwoRanksPerCh (
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT8           HbmIo;
  DEVICE_ID_WDR   *DeviceIdData;

  HbmIo = (UINT8) (Ch / GetNumChannelPerIoHbm ());
  DeviceIdData = GetHbmDeviceId (Socket, HbmIo);

  if (DeviceIdData->Bits.hbm_stack_height == 1) {
    return TRUE;   // 8-High stack
  } else {
    return FALSE;  // 2- or 4-High stack
  }
}

/**

  Program HBM timing registers related to refresh

  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number

  @retval N/A

**/
VOID
EFIAPI
ProgramRefreshTimingsHbm (
  UINT8    Socket,
  UINT8    Ch
  )
{
  UINT8                                HbmIo;
  UINT8                                Density;
  UINT16                               Trfc;
  TCZQCAL_MCDDC_CTL_STRUCT             Tczqcal;
  TCRFTP_MCDDC_CTL_STRUCT              Tcrftp;
  TCSTAGGER_REF_MCDDC_CTL_STRUCT       Tcstagger;
  MCMNT_CHKN_BIT_MCDDC_CTL_STRUCT      ChknBit;
  TCRFP_MCDDC_CTL_STRUCT               Tcrfp;
  DEVICE_ID_WDR                        *DeviceIdData;

  //
  // Program TCZQCAL_MCDDC_CTL_REG
  //
  Tczqcal.Data = ChRegisterRead (MemTechHbm, Socket, Ch, TCZQCAL_MCDDC_CTL_REG);

  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    Tczqcal.Bits.t_rfcpb = TRFCSB_HBM_2P8;
    Tczqcal.Bits.t_rrefd = TRREFD_HBM_2P8;
  } else {
    Tczqcal.Bits.t_rfcpb = TRFCSB_HBM_3P2;
    Tczqcal.Bits.t_rrefd = TRREFD_HBM_3P2;
  }

  ChRegisterWrite (MemTechHbm, Socket, Ch, TCZQCAL_MCDDC_CTL_REG, Tczqcal.Data);

  //
  // Program TCRFTP_MCDDC_CTL_REG
  //
  Tcrftp.Data = ChRegisterRead (MemTechHbm, Socket, Ch, TCRFTP_MCDDC_CTL_REG);

  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    Tcrftp.Bits.t_refi = T_REFI_HBM_2P8;
  } else {
    Tcrftp.Bits.t_refi = T_REFI_HBM_3P2;
  }

  HbmIo = (UINT8) (Ch / GetNumChannelPerIoHbm ());
  DeviceIdData = GetHbmDeviceId (Socket, HbmIo);
  Density = (UINT8) DeviceIdData->Bits.density;

  if (EFI_SUCCESS == HbmGetTrfc (Socket, Density, &Trfc)) {
    Tcrftp.Bits.t_rfc = Trfc;
  } else {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "HBM HbmGetTrfc failed: Density = 0x%x\n", Density);
  }

  Tcrftp.Bits.t_ref_block = T_REF_BLOCK_HBM;
  ChRegisterWrite (MemTechHbm, Socket, Ch, TCRFTP_MCDDC_CTL_REG, Tcrftp.Data);

  //
  // REF-REF separation
  //
  Tcstagger.Data = ChRegisterRead (MemTechHbm, Socket, Ch, TCSTAGGER_REF_MCDDC_CTL_REG);

  Tcstagger.Bits.perbank_mode = PERBANK_MODE_HBM;
  Tcstagger.Bits.no_bank_overlap = NO_BANK_OVERLAP_HBM;
  Tcstagger.Bits.keep_cke_high_during_trfc = KEEP_CKE_HIGH_DURING_TRFC_HBM;
  ChRegisterWrite (MemTechHbm, Socket, Ch, TCSTAGGER_REF_MCDDC_CTL_REG, Tcstagger.Data);

  //
  // SetStaggerRefEnHbm
  //
  ChknBit.Data = ChRegisterRead (MemTechHbm, Socket, Ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG);

  ChknBit.Bits.stagger_ref_en = STAGGER_REF_EN_HBM;
  ChknBit.Bits.dis_opp_ref = DIS_OPP_REF_HBM;
  ChknBit.Bits.dis_refresh_start_delay = DIS_REFRESH_START_DELAY_HBM;
  ChRegisterWrite (MemTechHbm, Socket, Ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG, ChknBit.Data);

  //
  // Timing Constraints Refresh Timing Parameters
  //
  Tcrfp.Data = ChRegisterRead (MemTechHbm, Socket, Ch, TCRFP_MCDDC_CTL_REG);

  Tcrfp.Bits.orefni = OREFNI_HBM;
  Tcrfp.Bits.max_channel_ref = MAX_CHANNEL_REF_HBM;
  Tcrfp.Bits.ref_hi_wm = REF_HI_WM_HBM;
  Tcrfp.Bits.ref_panic_wm = REF_PANIC_WM_HBM;
  Tcrfp.Bits.enable_6ref_window = ENABLE_6REF_WINDOW_HBM;
  ChRegisterWrite (MemTechHbm, Socket, Ch, TCRFP_MCDDC_CTL_REG, Tcrfp.Data);
}

/**

  Program JEDEC timing registers

  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number

  @retval N/A

**/
VOID
ProgramJedecTimingsHbm (
  IN  UINT8    Socket,
  IN  UINT8    Ch
  )
{
  MEMORY_TIMINGS_BANK_TCL_TWL_MCDDC_CTL_STRUCT          MtTclTwl;
  MEMORY_TIMINGS_BANK_TRTP_TWR_MCDDC_CTL_STRUCT         MtTrtpTwr;
  MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_STRUCT        MtTrrdTfaw;
  MEMORY_TIMINGS_RANK_TRRD_IMPLPRE_MCDDC_CTL_STRUCT     MtTrrdImplpre;
  MEMORY_TIMINGS_BANK_TRP_TRC_TRAS_MCDDC_CTL_STRUCT     MtTrpTrcTras;
  MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_STRUCT             MtTrcdRW;
  TCKE_MCDDC_CTL_STRUCT                                 TCke;
  MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_STRUCT MtTrdaTwraTwrpre;
  UINT8                                                 MaxChHbm;

  MaxChHbm = GetMaxChHbm ();

  if (Ch >= MaxChHbm) {
    // invalid Ch specified
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_121);
    return;
  }

  MtTclTwl.Data         = ChRegisterRead (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_TCL_TWL_MCDDC_CTL_REG);
  MtTrcdRW.Data         = ChRegisterRead (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_REG);
  MtTrpTrcTras.Data     = ChRegisterRead (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_TRP_TRC_TRAS_MCDDC_CTL_REG);
  TCke.Data             = ChRegisterRead (MemTechHbm, Socket, Ch, TCKE_MCDDC_CTL_REG);
  MtTrrdTfaw.Data       = ChRegisterRead (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_REG);
  MtTrrdImplpre.Data    = ChRegisterRead (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_IMPLPRE_MCDDC_CTL_REG);
  MtTrtpTwr.Data        = ChRegisterRead (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_TRTP_TWR_MCDDC_CTL_REG);
  MtTrdaTwraTwrpre.Data = ChRegisterRead (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_REG);

  MtTrcdRW.Bits.t_rcd_rd          = HbmGettRCD_RD (Socket);
  MtTrcdRW.Bits.t_rcd_wr          = HbmGettRCD_WR (Socket);
  MtTrcdRW.Bits.t_rcd_imprd       = HbmGettRCD_ImpRD (Socket);
  MtTrcdRW.Bits.t_rcd_impwr       = HbmGettRCD_ImpWR (Socket);

  MtTrrdTfaw.Bits.t_rrd_l         = HbmGettRRD_L (Socket);
  MtTrrdTfaw.Bits.t_rrd_s         = HbmGettRRD (Socket);
  MtTrrdTfaw.Bits.t_faw           = HbmGettFAW (Socket);

  MtTrrdImplpre.Bits.t_rrd_s_a2i       = 0x1;
  MtTrrdImplpre.Bits.t_rrd_l_a2i       = 0x1;

  if (HbmGetFrequency (Socket) == HBM2E_2p8) {
    MtTrrdImplpre.Bits.t_rrd_s_i2a       = 0x19;
    MtTrrdImplpre.Bits.t_rrd_l_i2a       = 0x19;
  } else { // HBM2E_3p2
    MtTrrdImplpre.Bits.t_rrd_s_i2a       = 0x1D;
    MtTrrdImplpre.Bits.t_rrd_l_i2a       = 0x1D;
  }

  MtTclTwl.Bits.t_cl              = HbmGettCL (Socket);
  MtTclTwl.Bits.t_wl              = HbmGettCWL (Socket);

  MtTrtpTwr.Bits.t_rtp            = HbmGettRTP (Socket);
  MtTrtpTwr.Bits.t_wr             = HbmGettWR (Socket);

  MtTrpTrcTras.Bits.t_rp          = HbmGettRP (Socket);
  MtTrpTrcTras.Bits.t_rc          = HbmGettRC (Socket);
  MtTrpTrcTras.Bits.t_ras         = HbmGettRAS (Socket);

  TCke.Bits.t_prpden              = HbmGettPRPDEN (Socket);
  TCke.Bits.t_cke                 = HbmGettCKE (Socket);
  TCke.Bits.t_xp                  = HbmGettXP (Socket);
  TCke.Bits.t_rdpden              = HbmGettRDPDEN (Socket);
  TCke.Bits.t_wrpden              = HbmGettWRPDEN (Socket);

  MtTrdaTwraTwrpre.Bits.t_rda     = HbmGettRDA (Socket);
  MtTrdaTwraTwrpre.Bits.t_wra     = HbmGettWRA (Socket);
  MtTrdaTwraTwrpre.Bits.t_wrpre   = HbmGettWRPRE (Socket);

  ChRegisterWrite (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_TCL_TWL_MCDDC_CTL_REG, MtTclTwl.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_TRCD_MCDDC_CTL_REG, MtTrcdRW.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_TRP_TRC_TRAS_MCDDC_CTL_REG, MtTrpTrcTras.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, TCKE_MCDDC_CTL_REG, TCke.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_TFAW_MCDDC_CTL_REG, MtTrrdTfaw.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_RANK_TRRD_IMPLPRE_MCDDC_CTL_REG, MtTrrdImplpre.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_TRTP_TWR_MCDDC_CTL_REG, MtTrtpTwr.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_REG, MtTrdaTwraTwrpre.Data);
}

/**

  Initialize the CMD timing mode registers
  If skipping training, go straight to the normal CMD timing.
  Otherwise, start with the training timing.

  @param[in] Socket      - Socket number
  @param[in] Channel          - Channel number

  @retval N/A

**/
VOID
InitCmdTimingHbm (
  UINT8    Socket,
  UINT8    Channel
  )
{
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT         MemTimingsAdj;

  MemTimingsAdj.Data = ChRegisterRead (MemTechHbm, Socket, Channel, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);

  MemTimingsAdj.Bits.dparity_latency = 0x0;  //   HBM data parity latency
  MemTimingsAdj.Bits.cmd_stretch     = TIMING_1N; // 00 = 1N, 01 = rsvd, 10 = 2N, 11 = 3N
  MemTimingsAdj.Bits.en_3ds          = 0x0;  //   enable 3DS RDIMM/LRDIMM support
  MemTimingsAdj.Bits.t_cwl_adj       = 0x0;  //   Additional tCWL latency (Wr flyby issues)
  MemTimingsAdj.Bits.t_cwl_adj_neg   = 0x0;  //   Reduction of tCWL latency (Wr flyby issues)
  MemTimingsAdj.Bits.write_preamble  = 0x0;
  MemTimingsAdj.Bits.read_preamble   = 0x0;

  ChRegisterWrite (MemTechHbm, Socket, Channel, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, MemTimingsAdj.Data);
}

/**

  Configure Rid Vc before Training

  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number

  @retval N/A

**/
VOID
ConfigureRidVcbeforeTrainingHbm (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  SCHEDULER_RID_MCDDC_CTL_STRUCT                Rid;

  Rid.Data = ChRegisterRead (MemTechHbm, Socket, Channel, SCHEDULER_RID_MCDDC_CTL_REG);

  //
  // Scheduler RID
  //
  Rid.Bits.enable_rid_vc0 = 1;
  Rid.Bits.enable_rid_vc1 = 0;
  Rid.Bits.enable_rid_vc2 = 0;
  Rid.Bits.enable_rid_vc3 = 0;

  Rid.Bits.enable_rid_underfill = 1;//reserved underfill read entry enable

  Rid.Bits.enable_rid = 0xffffff;
  Rid.Bits.rid_vc2_low_threshold = 0;
  ChRegisterWrite (MemTechHbm, Socket, Channel, SCHEDULER_RID_MCDDC_CTL_REG, Rid.Data);
} // ConfigureRidVcbeforeTraining

/**

  Pre-training initialization of scheduler registers

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval N/A

**/
VOID
SchedulerConfigHbm (
  IN  UINT8    Socket,
  IN  UINT8    Ch
  )
{
  HBM_HOST                                  *HbmHost;
  UINT8                                     IoModule;
  HBM_DENSITY                               HbmDensity;
  UINT8                                     AddressingMode;
  DIMMMTR_DATA_STRUCT                       DimmMtr;
  SCHEDULER_ENABLES_BS_MCDDC_CTL_STRUCT     SchedulerEnablesBs;
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT        SchedulerEnables;
  SCHEDULER_PENDINGQ_MCDDC_CTL_STRUCT       PendingQ;
  SCHEDULER_RID_ENTRY_MCDDC_CTL_STRUCT      RidEntry;
  SCHEDULER_QOS_LOW_MCDDC_CTL_STRUCT        SchedulerQosLow;
  SCHEDULER_QOS_MED_MCDDC_CTL_STRUCT        SchedulerQosMed;
  SCHEDULER_QOS_HIGH_MCDDC_CTL_STRUCT       SchedulerQosHigh;
  //UINT8                                     BankSchedulerSelection;
  //UINT8                                     PageTableIndexSelection;

  DimmMtr.Data = 0;
  HbmHost = (HBM_HOST *) GetHbmHostPointer ();
  IoModule = (UINT8) (Ch / GetNumChannelPerIoHbm ());

  HbmDensity = HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId.Bits.density;
  AddressingMode = (UINT8) HbmHost->Var.Socket[Socket].HbmCh[IoModule].DevId.Bits.addresing_mode;

  if (EFI_ERROR (HbmGetDimmMtr (HbmDensity, AddressingMode, &DimmMtr))) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "ERROR: Invalid density: %d\n", HbmDensity);
  }

  SchedulerEnablesBs.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG);
  SchedulerEnables.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG);
  PendingQ.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_PENDINGQ_MCDDC_CTL_REG);
  RidEntry.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_RID_ENTRY_MCDDC_CTL_REG);
  SchedulerQosLow.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_QOS_LOW_MCDDC_CTL_REG);
  SchedulerQosMed.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_QOS_MED_MCDDC_CTL_REG);
  SchedulerQosHigh.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_QOS_HIGH_MCDDC_CTL_REG);

  // Scheduler Enables
  SchedulerEnablesBs.Bits.implicit_pre_en         = 1; // Set to 0x1 only if HBM2 open page modes
  SchedulerEnablesBs.Bits.link_major_mode_en      = 0;

  if (DimmMtr.Bits.number_of_banks >= NUMBER_OF_BANKS_48) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "ERROR: MC donot support 12-HI HBM(48 banks), downgrade to 32 banks\n");
    DimmMtr.Bits.number_of_banks = NUMBER_OF_BANKS_32;
  }

  SchedulerEnablesBs.Bits.number_of_banks         = DimmMtr.Bits.number_of_banks; // depend on density
  SchedulerEnablesBs.Bits.idle_bypass_en          = 1;

  SchedulerEnables.Bits.rpq_vc1_llt_en            = 0;
  SchedulerEnables.Bits.idle_bypass_en            = 1;
  SchedulerEnables.Bits.xpq_llt_en                = 1;
  SchedulerEnables.Bits.pseudo_channel_selection  = 0;

  // Scheduler Pending Queue
  PendingQ.Bits.wpq_link_list_threshold   = LINK_LIST_THRESHOLD;
  PendingQ.Bits.rpq_link_list_threshold   = LINK_LIST_THRESHOLD;
  PendingQ.Bits.wpq_empty_llt_priority_en = 0;
  PendingQ.Bits.rpq_empty_llt_priority_en = 0;
  PendingQ.Bits.wpq_priority_en           = 1;
  PendingQ.Bits.rpq_priority_en           = 1;

  // Scheduler RID entry
  RidEntry.Bits.enable_rid_loopback = 0;
  RidEntry.Bits.rid_loopback_entry = 0x13;
  RidEntry.Bits.rid_underfill_entry = 0x17;
  RidEntry.Bits.rid_vc0_entry = 0x14;
  RidEntry.Bits.rid_vc1_entry = 0x15;
  RidEntry.Bits.rid_vc2_entry = 0x15;
  RidEntry.Bits.rid_vc3_entry = 0x16;

  ConfigureRidVcbeforeTrainingHbm (Socket, Ch);

  // Scheduler promotion LOW
  SchedulerQosLow.Bits.qos_lowenable = 1;
  SchedulerQosLow.Bits.qos_lowtimer_lim = FixedPcdGet16 (PcdMrcQosLowTimerLim);

  // Scheduler promotion MED
  SchedulerQosMed.Bits.qos_medenable = 1;
  SchedulerQosMed.Bits.qos_medtimer_lim = FixedPcdGet16 (PcdMrcQosMedTimerLim);

  // Scheduler promotion HIGH
  SchedulerQosHigh.Bits.qos_highenable = 1;
  SchedulerQosHigh.Bits.qos_hightimer_lim = FixedPcdGet16 (PcdMrcQosHighTimerLim);

  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_ENABLES_BS_MCDDC_CTL_REG, SchedulerEnablesBs.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_PENDINGQ_MCDDC_CTL_REG, PendingQ.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_RID_ENTRY_MCDDC_CTL_REG, RidEntry.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_QOS_LOW_MCDDC_CTL_REG, SchedulerQosLow.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_QOS_MED_MCDDC_CTL_REG, SchedulerQosMed.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_QOS_HIGH_MCDDC_CTL_REG, SchedulerQosHigh.Data);
}

/**
  Program timing registers related to write major mode

  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number

  @retval N/A
**/
VOID
InitWriteMajorModeHbm (
  UINT8    Socket,
  UINT8    Ch
  )
{
  SCHEDULER_WMM_MCDDC_CTL_STRUCT          SchedulerWmm;

  SchedulerWmm.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_WMM_MCDDC_CTL_REG);

  SchedulerWmm.Bits.wmm_read_hit_en              = 1;
  SchedulerWmm.Bits.wmm_exit_threshold           = WMM_EXIT_HBM;
  SchedulerWmm.Bits.wmm_enter_threshold          = WMM_ENTRY_HBM;
  SchedulerWmm.Bits.wmm_opp_timer                = OPP_TIMER;
  SchedulerWmm.Bits.wmm_opp_read_limit           = OPP_LIMIT;
  SchedulerWmm.Bits.wmm_opp_timer_gran           = OPP_TIMER_GRAN;
  SchedulerWmm.Bits.wmm_opp_read_limit_gran      = OPP_LIMIT_GRAN;
  SchedulerWmm.Bits.wpq_empty_opp_rd_en          = 0;
  SchedulerWmm.Bits.wmm_leaky_bucket_en          = 0;

  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_WMM_MCDDC_CTL_REG, SchedulerWmm.Data);
}

/**
  Program timing registers related to read major mode

  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number

  @retval N/A
**/
VOID
InitReadMajorModeHbm (
  UINT8    Socket,
  UINT8    Ch
  )
{
  SCHEDULER_RMM_MCDDC_CTL_STRUCT              SchedulerRmm;

  SchedulerRmm.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_RMM_MCDDC_CTL_REG);

  SchedulerRmm.Bits.rmm_write_hit_en             = 1;
  SchedulerRmm.Bits.rmm_opp_timer                = OPP_TIMER;
  SchedulerRmm.Bits.rmm_opp_write_limit          = OPP_LIMIT;
  SchedulerRmm.Bits.rmm_opp_timer_gran           = OPP_TIMER_GRAN;
  SchedulerRmm.Bits.rmm_opp_write_limit_gran     = OPP_LIMIT_GRAN;
  SchedulerRmm.Bits.rpq_empty_opp_wr_en          = 1;
  SchedulerRmm.Bits.rmm_leaky_bucket_en          = 0;

  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_RMM_MCDDC_CTL_REG, SchedulerRmm.Data);
}

/**
  Program starvation registers

  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number

  @retval N/A
**/
VOID
InitStarvationCountersHbm (
  UINT8    Socket,
  UINT8    Ch
  )
{
  SCHEDULER_CMD_STARVATION_MCDDC_CTL_STRUCT   schedulerCmdStarve;
  SCHEDULER_READ_STARVE_MCDDC_CTL_STRUCT      schedulerReadStarve;
  SCHEDULER_WRITE_STARVE_MCDDC_CTL_STRUCT     schedulerWriteStarve;

  schedulerCmdStarve.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_CMD_STARVATION_MCDDC_CTL_REG);
  schedulerReadStarve.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_READ_STARVE_MCDDC_CTL_REG);
  schedulerWriteStarve.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_WRITE_STARVE_MCDDC_CTL_REG);

  schedulerCmdStarve.Bits.starved_count = CAS_STARVE_COUNT_HBM;
  schedulerCmdStarve.Bits.critical_count = CAS_CRITICAL_COUNT_HBM;
  schedulerCmdStarve.Bits.granularity = 2;
  schedulerCmdStarve.Bits.starved_en = 1;

  schedulerReadStarve.Bits.read_wait_count      = READ_WAIT_COUNT_HBM;
  schedulerReadStarve.Bits.read_starve_count    = READ_STARVE_COUNT_HBM;

  schedulerWriteStarve.Bits.write_wait_count    = WRITE_WAIT_COUNT_HBM;
  schedulerWriteStarve.Bits.write_starve_count  = WRITE_STARVE_COUNT_HBM;

  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_CMD_STARVATION_MCDDC_CTL_REG, schedulerCmdStarve.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_READ_STARVE_MCDDC_CTL_REG, schedulerReadStarve.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_WRITE_STARVE_MCDDC_CTL_REG, schedulerWriteStarve.Data);
}

/**

  Programs timing parameters

  @param[in] Socket      - Socket number

  @retval EFI_SUCCESS     Success
          !EFI_SUCCESS    Failed

**/
EFI_STATUS
EFIAPI
ProgramTimingsHbm (
  UINT8    Socket
  )
{
  UINT8                       Ch;
  UINT8                       MaxChHbm;

  MaxChHbm = GetMaxChHbm ();

  for (Ch = 0; Ch < MaxChHbm; Ch++) {
    if (!IsHbmChannelEnabled (Socket, Ch)) {
      continue;
    }

    //
    // Program HBM Refresh timing parameters
    //
    ProgramRefreshTimingsHbm (Socket, Ch);

    //
    // Program JEDEC timings contraints
    //
    ProgramJedecTimingsHbm (Socket, Ch);

    //
    // Initialize command timing mode
    //
    InitCmdTimingHbm (Socket, Ch);

    //
    // Scheduler Configuration
    //
    SchedulerConfigHbm (Socket, Ch);

    //
    // Initialize Read/Write Major Modes and Partial Write Starvation
    //
    InitWriteMajorModeHbm (Socket, Ch);
    InitReadMajorModeHbm (Socket, Ch);

    //
    // Initialize Read/Write/CAS starvation counters
    //
    InitStarvationCountersHbm (Socket, Ch);
  }

  return EFI_SUCCESS;
} // ProgramTimings10nmHbm

/**

  Program scheduler blocking rules registers in late step

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval   MRC_STATUS_SUCCESS

**/
MRC_STATUS
SchedulerBlockingRulesConfigHbm (
  IN    UINT8    Socket,
  IN    UINT8    Ch
  )
{
  SCHEDULER_BLOCKING_RULES_MCDDC_CTL_STRUCT       BlockingRules;

  BlockingRules.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_BLOCKING_RULES_MCDDC_CTL_REG);

  //
  // Scheduler Blocking Rules
  //
  BlockingRules.Bits.rpq_loaded_threshold                                         = 4;
  BlockingRules.Bits.wpq_loaded_threshold                                         = 4;
  BlockingRules.Bits.maj_exp_act_blck_noncrit_impre_sp_en                         = 0;
  BlockingRules.Bits.noncrit_maj_write_cas_blck_noncrit_min_read_cas_threshold_en = 0;
  BlockingRules.Bits.noncrit_maj_read_cas_blck_noncrit_min_write_cas_threshold_en = 1;
  BlockingRules.Bits.noncrit_maj_write_act_blck_noncrit_min_read_act_threshold_en = 0;
  BlockingRules.Bits.noncrit_maj_read_act_blck_noncrit_min_write_act_threshold_en = 1;
  BlockingRules.Bits.noncrit_maj_act_blck_noncrit_min_act_sr_en                   = 0;

  BlockingRules.Bits.crit_act_bclk_noncrit_act_sr_en                              = 1;
  BlockingRules.Bits.top_strv_act_blck_act_sr_en                                  = 1;
  BlockingRules.Bits.crit_min_blck_noncrit_maj_wmm_en                             = 1;
  BlockingRules.Bits.crit_min_blck_noncrit_maj_rmm_en                             = 1;
  BlockingRules.Bits.crit_maj_blck_crit_min_wmm_en                                = 1;
  BlockingRules.Bits.crit_maj_blck_crit_min_rmm_en                                = 1;
  BlockingRules.Bits.crit_maj_blck_noncrit_min_wmm_en                             = 1;
  BlockingRules.Bits.crit_maj_blck_noncrit_min_rmm_en                             = 1;
  BlockingRules.Bits.top_strv_blck_en                                             = 1;

  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_BLOCKING_RULES_MCDDC_CTL_REG, BlockingRules.Data);

  return MRC_STATUS_SUCCESS;
}

/**

  Program scheduler preemption registers

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval   MRC_STATUS_SUCCESS

**/
MRC_STATUS
SchedulerPreemptionConfigHbm (
  IN    UINT8    Socket,
  IN    UINT8    Ch
  )
{
  UINT32                                      ReadPreemption;
  UINT32                                      WritePreemption;
  UINT32                                      MissPreemption;
  UINT32                                      HitPreemption;
  SCHEDULER_PREEMPTION_MCDDC_CTL_STRUCT       SchedulerPreemption;

  //
  // Scheduler Preemption rules from Memory Performance CSR Settings 04/26/2017
  //
  SchedulerPreemption.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_PREEMPTION_MCDDC_CTL_REG);

  SchedulerPreemption.Bits.rpq_preempt_watermark        = 1;
  SchedulerPreemption.Bits.wpq_preempt_watermark        = 1;
  SchedulerPreemption.Bits.enable_critical_promotion    = 0;
  SchedulerPreemption.Bits.enable_critical_preemption   = 0;

  GetPreemptionEnableSetting (&ReadPreemption, &WritePreemption, &MissPreemption, &HitPreemption); // can reuse for HBM
  SchedulerPreemption.Bits.enable_wmm_read_preemption   = ReadPreemption;
  SchedulerPreemption.Bits.enable_rmm_write_preemption  = WritePreemption;
  SchedulerPreemption.Bits.enable_omm_miss_preemption   = MissPreemption;
  SchedulerPreemption.Bits.enable_omm_hit_preemption    = HitPreemption;

  //
  // Enable Refresh Preemption - preempt transaction if bank-scheduler is blocked by refresh.
  //
  if (IsHbmMoreThanTwoRanksPerCh (Socket, Ch)) {
    SchedulerPreemption.Bits.enable_refresh_preemption  = 1; // Enable on HBM2 8H or DDR4 configurations with more than 2 ranks.
  }

  SchedulerPreemption.Bits.enable_rdb_preemption        = 1;
  SchedulerPreemption.Bits.rdb_preempt_watermark        = 5;

  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_PREEMPTION_MCDDC_CTL_REG, SchedulerPreemption.Data);

  return MRC_STATUS_SUCCESS;
}

/**

  Configure idle bypass timings

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval N/A

**/
VOID
SchedulerIdleBypassConfigHbm (
  IN    UINT8    Socket,
  IN    UINT8    Ch
  )
{
  SCHEDULER_ENABLES_MCDDC_CTL_STRUCT                      SchedulerEnables;
  MEMORY_TIMINGS_TRRD_IDLE_BYPASS_MCDDC_CTL_STRUCT        MtTrrdIdleBypass;
  MEMORY_TIMINGS_BANK_IDLE_BYPASS_MCDDC_CTL_STRUCT        MtBankIdleBypass;
  IDLE_BYPASS_OFFSETS                                     *IdleBypassOffsets;

  SchedulerEnables.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG);
  MtTrrdIdleBypass.Data = ChRegisterRead (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_TRRD_IDLE_BYPASS_MCDDC_CTL_REG);
  MtBankIdleBypass.Data = ChRegisterRead (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_IDLE_BYPASS_MCDDC_CTL_REG);

  GetIdleBypassOffsets (MemTechHbm, &IdleBypassOffsets);

  // Scheduler Enables
  SchedulerEnables.Bits.idle_bypass_en = 1;

  //
  // The programming for the idle bypass timings comes directly from the register definitions
  //
  MtTrrdIdleBypass.Bits.t_rrd_s_bypass  = HbmGettRRD (Socket)    - IdleBypassOffsets->t_rrd_s_bypass;
  MtTrrdIdleBypass.Bits.t_rrd_l_bypass  = HbmGettRRD_L (Socket)  - IdleBypassOffsets->t_rrd_l_bypass;
  MtBankIdleBypass.Bits.t_rcd_wr_bypass = HbmGettRCD_WR (Socket) - IdleBypassOffsets->t_rcd_wr_bypass;
  MtBankIdleBypass.Bits.t_rcd_rd_bypass = HbmGettRCD_RD (Socket) - IdleBypassOffsets->t_rcd_rd_bypass;
  MtBankIdleBypass.Bits.t_rc_bypass     = HbmGettRC (Socket)     - IdleBypassOffsets->t_rc_bypass;
  MtBankIdleBypass.Bits.t_ras_bypass    = HbmGettRAS (Socket)    - IdleBypassOffsets->t_ras_bypass;

  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_ENABLES_MCDDC_CTL_REG, SchedulerEnables.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_TRRD_IDLE_BYPASS_MCDDC_CTL_REG, MtTrrdIdleBypass.Data);
  ChRegisterWrite (MemTechHbm, Socket, Ch, MEMORY_TIMINGS_BANK_IDLE_BYPASS_MCDDC_CTL_REG, MtBankIdleBypass.Data);

  EnableBsIdleBypass (MemTechHbm, Socket, Ch);
}

/**

  Program page policy registers after DDR training

  @param[in] Socket      - Socket number
  @param[in] Ch          - Channel number

  @retval N/A

**/
VOID
SchedulerPagePolicyConfigLateHbm (
  IN    UINT8    Socket,
  IN    UINT8    Ch
  )
{
  SCHEDULER_IDLETIME2_MCDDC_CTL_STRUCT        IdleTime2;

  IdleTime2.Data = ChRegisterRead (MemTechHbm, Socket, Ch, SCHEDULER_IDLETIME2_MCDDC_CTL_REG);

  IdleTime2.Bits.idle_page_rst_val = IDLE_PAGE_RST_VAL_HBM;

  ChRegisterWrite (MemTechHbm, Socket, Ch, SCHEDULER_IDLETIME2_MCDDC_CTL_REG, IdleTime2.Data);
}

/**

  Program scheduler registers, chip specific

  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number

  @retval N/A

**/
VOID
SchedulerConfigLateChipHbm (
  IN UINT8 Socket,
  IN UINT8 Channel
  )
{
  SCHEDULER_RID_MCDDC_CTL_STRUCT                Rid;

  Rid.Data = ChRegisterRead (MemTechHbm, Socket, Channel, SCHEDULER_RID_MCDDC_CTL_REG);

  //
  // Scheduler RID
  //
  Rid.Bits.enable_rid_vc0 = 1;
  Rid.Bits.enable_rid_vc1 = 0;
  Rid.Bits.enable_rid_vc2 = 0;
  Rid.Bits.enable_rid_vc3 = 0;

  ChRegisterWrite (MemTechHbm, Socket, Channel, SCHEDULER_RID_MCDDC_CTL_REG, Rid.Data);
} // SchedulerConfigLateChipHbm

/**

  Program scheduler registers after training

  @param[in] Socket - Socket number

  @retval EFI_SUCCESS     Success
          !EFI_SUCCESS    Failed

**/
EFI_STATUS
EFIAPI
SchedulerConfigLateHbm (
  IN    UINT8    Socket
  )
{
  UINT8                       Ch;
  UINT8                       MaxChHbm;

  MaxChHbm = GetMaxChHbm ();

  for (Ch = 0; Ch < MaxChHbm; Ch++) {
    if (!IsHbmChannelEnabled (Socket, Ch)) {
      continue;
    }

    //
    // Scheduler Blocking Rules programming
    //
    SchedulerBlockingRulesConfigHbm (Socket, Ch);

    //
    // Scheduler Preemption programming
    //
    SchedulerPreemptionConfigHbm (Socket, Ch);

    //
    // Scheduler idle bypass programming
    //
    SchedulerIdleBypassConfigHbm (Socket, Ch);

    //
    // Page policy programming
    //
    SchedulerPagePolicyConfigLateHbm (Socket, Ch);

    //
    // Chip specific settings
    //
    SchedulerConfigLateChipHbm (Socket, Ch);
  } // Ch

  return EFI_SUCCESS;
}

