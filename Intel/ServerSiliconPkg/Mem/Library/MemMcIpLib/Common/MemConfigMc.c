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
#include <Chip/Include/MemHostChip.h>
#include <Library/MemPorLib.h>
#include <Library/ProcMemInitChipLib.h>

#include <Cpu/CpuIds.h>
#include <Chip/Include/FnvAccessCommon.h>
#include <ScratchpadList.h>

#include <Library/MemFmcIpLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemCpgcIpLib.h>
#include <Library/MemRoutingLib.h>
#include <Library/CheckpointLib.h>
#include <Library/EmulationConfigurationLib.h>

#include "Include/MemMcRegs.h"
#include "Include/MemMcChip.h"
#include "Include/MemMcIpLibInternal.h"

#include <Library/MemDdrioIpLib.h>
#include <Library/CteNetLib.h>

#include <Library/SystemInfoLib.h>
#include <Library/SysHostPointerLib.h>

#include "MemHostChipCommon.h"
#include <Guid/UboxIpInterface.h>
#include <Library/KtiApi.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Include/MemCommon.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemThermManageLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/IoAccessLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/MemMcIpTargetLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/OdtTableLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/SecurityPolicyLib.h>
#include <Library/MemCallTableLib.h>
#include <Library/ProcMemInitTargetLib.h>
#include <Library/MemTypeLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/UsraCsrLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/PchAdrLib.h>
#include <CpuPciAccess.h>
#include <Library/MemRcLib.h>
#include <Include/MemMapHost.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/TimerLib.h>
#include <Library/MemSmeeIpLib.h>
#include <Library/MemTurnaroundTimingLib.h>
#include <Library/PcuIpLib.h>
#include <Library/MemVrSvidMapLib.h>
#include <Library/MemAccessLib.h>
#include <Library/PerformanceTrackerLib.h>

/*****************************************************************************
 * Definitions
 *****************************************************************************/
#ifndef NELEMENTS
#define NELEMENTS(Array) (sizeof(Array)/sizeof((Array)[0]))
#endif

#define COMPLETION_DELAY_RESET_COUNT_MAX  3
#define INTERNAL_PIPESTAGE_DELAY_DCLK 3
#define INCREASE_SPID_RDDAT_VALID_TO_RDDATA_SPACING BIT11

#define ADR_TYPE_PCODE_ASSISTED     BIT18
#define ADR_IO_CACHE_FLUSH_ENABLED  BIT25

#if FixedPcdGetBool(PcdMcrSupport)
#define NOP_CYCLES_2CLK    2      // 2 CLK NOP
#define NOP_CYCLES_64CLK   64     // 64 CLK NOP
#endif // FixedPcdGetBool(PcdMcrSupport)

/*****************************************************************************
 * Variables
 *****************************************************************************/

//sref_idle_timer, opp_sref_en, mdll_off_en, ck_mode, ddrt_idle_cmd
STATIC CONST UINT32 SREF_LL0_AUTO[]  = {0xFFFFF, 0, 1, 2, 1};
STATIC CONST UINT32 SREF_LL1_AUTO[]  = {0x0E800, 1, 1, 2, 1};
STATIC CONST UINT32 SREF_LL2_AUTO[]  = {0x0C800, 1, 1, 2, 1};
STATIC CONST UINT32 SREF_LL3_AUTO[]  = {    0xF, 1, 1, 2, 2};
STATIC CONST UINT32 SREF_PKGC_AUTO[] = {    0xF, 0, 1, 2, 1};
STATIC CONST UINT32 SREF_UCR_AUTO[]  = {    0xF, 0, 1, 2, 1};

// cke_idle_timer, apd_en, ppd_en, ddrt_cke_en
STATIC CONST UINT8 CKE_LL0_AUTO[]  = {0x80, 0, 0, 0};
STATIC CONST UINT8 CKE_LL1_AUTO[]  = {0x60, 0, 1, 1};
STATIC CONST UINT8 CKE_LL2_AUTO[]  = {0x30, 0, 1, 1};
STATIC CONST UINT8 PKGC_CKE_AUTO[] = {0x30, 0, 1, 1};

extern UINT8 tWL_DDR4[MAX_SUP_FREQ];

extern UINT8  lrdimmExtraOdtDelay[MAX_SUP_FREQ];
extern CONST UINT8 tPARRECOVERY[MAX_SUP_FREQ];

//
// Local Prototypes
//
VOID   ExitSR10nm (PSYSHOST Host, UINT8 socket);
VOID   EnableHostRefresh (PSYSHOST Host, UINT8 Socket);
VOID   PowerManagementSetup10nm (PSYSHOST Host, UINT8 socket);
VOID   EarlyDdrtConfig (PSYSHOST Host, UINT8 socket);
VOID   SetupCapWrCrcErrorFlow10nm (PSYSHOST Host, UINT8 socket);
VOID   PmSelfRefreshProgramming10nm (PSYSHOST Host, UINT8 socket, UINT8 ch);
VOID   PmCkeProgramming10nm (IN PSYSHOST Host, IN UINT8 Socket, IN UINT8 Ch);
VOID   ProgramErid2DataValid (PSYSHOST Host, UINT8 Socket);
VOID   DdrtCapErrorWA (PSYSHOST Host, UINT8 socket, UINT8 ch);
VOID   ProgramNvmdimmPerf (IN PSYSHOST Host, IN UINT8 Socket);
VOID   ConfigSetupPMem (IN PSYSHOST Host);

/**
  Disable the memory controller to issue two refreshes upon exiting self-refresh.

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number

  @retval N\A

**/
VOID
DisableTwoRefreshes (
  IN UINT8     Socket,
  IN UINT8     Ch
  );

/**
DDRT commands can be issued with UI1 in high impedance mode depending on timing of
DDR4 error event with regard to DDRT command issue leading to silent data corruption
in the DDRT DIMM.  Setting scheduler_cmd_debug.cmd_ca_oe=1 and scheduler_cmd_debug.cmd_ca_on=1
works around this bug.

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval N/A
**/
VOID
DdrtSilentDataCorruptionWA (
  IN UINT8 Socket,
  IN UINT8 Ch
  );

EFI_STATUS
WriteCompletionCredits (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN BOOLEAN   EnableDisable
  );

#define DDRT_RETRY_LEVEL_TRIGGER_EN  BIT9  // Described in bios_purley HSD5385403
#define TABLE_NULL (UINT32(*)(PSYSHOST Host))NULL
#define DDRT_PDEN_ADJUST          5
#define DDRT_WRITE_LATENCY_DELAY  0xA
#define DDRT_WRITE_LATENCY_MIN    0x1
#define DDRT_DATAPATH_DELAY_WR    0x7
#define InNormalMode              TRUE
#define NotInNormalMode           FALSE
//
// Command to data completion delays
//
#define CMD_TO_DATA_DELAY_DDRT                0x2     // Completion delay for DDRT
#define CMPL_PROGRAM_DELAY_DDR4_LIMIT_LO      5
#define   CMPL_PROGRAM_DELAY_DDR4_ADDER_LO    3 // This is for erid_to_rdvalid_delay.
#define CMPL_PROGRAM_DELAY_DDR4_BASE_HI       8
#define   CMPL_PROGRAM_DELAY_DDR4_ADDER_HI   -8 // This is for erid_to_rdvalid_delay.
#define COMPLETION_DELAY_DCLKS_LO_MIN         2
// COMPLETION_DELAY_DCLKS_LO_MAX would be 4 if it were needed
#define COMPLETION_DELAY_DCLKS_LO_ADDER       6
#define COMPLETION_DELAY_DCLKS_HI_MIN         5
#define COMPLETION_DELAY_DCLKS_HI_MAX         10 // This max value is specifically for DDR4.
#define COMPLETION_DELAY_DCLKS_HI_MAX_EXT     12
#define COMPLETION_DELAY_DCLKS_HI_ADDER       -5


// -------------------------------------
// Code section
// -------------------------------------

/**
  Internal function to check if OppSR Enable is TRUE for any rank.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Channel - Memory channel number

  @retval  OppSR Auto status.
*/
BOOLEAN
IsOppSrEnabled (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  )
{
  UINT32                      Data32;
  SREF_LL0_MCDDC_CTL_STRUCT   SrefLl0;
  SREF_LL1_MCDDC_CTL_STRUCT   SrefLl1;
  SREF_LL2_MCDDC_CTL_STRUCT   SrefLl2;
  SREF_LL3_MCDDC_CTL_STRUCT   SrefLl3;
  SREF_PKGC_MCDDC_CTL_STRUCT  SrefPkgc;

  Data32 = 0;

  SrefLl0.Data = MemReadPciCfgEp (Socket, Channel, CKE_LL0_MCDDC_CTL_REG);
  SrefLl1.Data = MemReadPciCfgEp (Socket, Channel, CKE_LL1_MCDDC_CTL_REG);
  SrefLl2.Data = MemReadPciCfgEp (Socket, Channel, CKE_LL2_MCDDC_CTL_REG);
  SrefLl3.Data = MemReadPciCfgEp (Socket, Channel, CKE_LL3_MCDDC_CTL_REG);
  SrefPkgc.Data = MemReadPciCfgEp (Socket, Channel, SREF_PKGC_MCDDC_CTL_REG);

  Data32 |= SrefLl0.Bits.opp_sref_en;
  Data32 |= SrefLl1.Bits.opp_sref_en;
  Data32 |= SrefLl2.Bits.opp_sref_en;
  Data32 |= SrefLl3.Bits.opp_sref_en;
  Data32 |= SrefPkgc.Bits.opp_sref_en;

  if (Data32 != 0x00) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**
  Internal function to check if CKE Power Down Enable is TRUE for any rank.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Channel - Memory channel number

  @retval  CKE Power Down Enable status.
*/
BOOLEAN
IsCkePowerDownEnabled (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  )
{
  UINT32                     Data32;
  CKE_LL0_MCDDC_CTL_STRUCT   CkeLl0;
  CKE_LL1_MCDDC_CTL_STRUCT   CkeLl1;
  CKE_LL2_MCDDC_CTL_STRUCT   CkeLl2;
  CKE_LL3_MCDDC_CTL_STRUCT   CkeLl3;
  PKGC_CKE_MCDDC_CTL_STRUCT  PkgcCke;

  Data32 = 0;

  CkeLl0.Data = MemReadPciCfgEp (Socket, Channel, CKE_LL0_MCDDC_CTL_REG);
  CkeLl1.Data = MemReadPciCfgEp (Socket, Channel, CKE_LL1_MCDDC_CTL_REG);
  CkeLl2.Data = MemReadPciCfgEp (Socket, Channel, CKE_LL2_MCDDC_CTL_REG);
  CkeLl3.Data = MemReadPciCfgEp (Socket, Channel, CKE_LL3_MCDDC_CTL_REG);
  PkgcCke.Data = MemReadPciCfgEp (Socket, Channel, PKGC_CKE_MCDDC_CTL_REG);

  Data32 |= CkeLl0.Bits.apd_en | CkeLl0.Bits.ppd_en | CkeLl0.Bits.ddrt_cke_en;
  Data32 |= CkeLl1.Bits.apd_en | CkeLl1.Bits.ppd_en | CkeLl1.Bits.ddrt_cke_en;
  Data32 |= CkeLl2.Bits.apd_en | CkeLl2.Bits.ppd_en | CkeLl2.Bits.ddrt_cke_en;
  Data32 |= CkeLl3.Bits.apd_en | CkeLl3.Bits.ppd_en | CkeLl3.Bits.ddrt_cke_en;
  Data32 |= PkgcCke.Bits.apd_en | PkgcCke.Bits.ppd_en | PkgcCke.Bits.ddrt_cke_en;

  if (Data32 != 0x00) {
    return TRUE;
  } else {
    return FALSE;
  }
}


/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost

  @retval N/A

**/
UINT32
InitMem (
  PSYSHOST Host
  )
{
  UINT8                           socket;

  socket = Host->var.mem.currentSocket;
  UpdateCpuCsrAccessVar_PEI (GetSysCpuCsrAccessVar());

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  // Clear WorkAround register prior to starting
  Host->nvram.mem.socket[socket].wa = 0;

  SetUbiosOutputMode (ASM_OUTPUT_ENABLE);

  ConfigSetupPMem (Host);

  return SUCCESS;
} // InitMem

/**
  Calculates the socket channel number and sets the MCMTR register funcblock offset for a given
  channel and memory controller

  @param[in]      McId          Memory controller ID
  @param[in]      McChannel     Channel number (memory controller scope)
  @param[out]     SocketChannel Channel number (socket scope)
  @param[in, out] McMtrOffset   MCMTR register address with appropriate funcblock set for the given channel

  @retval EFI_SUCCESS           Valid SocketChannel and McMtrOffset are set
  @retval EFI_INVALID_PARAMETER Pointers to either SocketChannel or McMtrOffset are NULL
**/
EFI_STATUS
EFIAPI
GetSocketChannelAndMcmtrOffset (
  IN      UINT8   McId,
  IN      UINT8   McChannel,
  OUT     UINT8   *SocketChannel,
  IN OUT  UINT32  *McMtrOffset
  )
{
  UINT32  MaxMcChannels = (UINT32) FixedPcdGet8 (PcdMaxMcIpChannels);
  UINT8   NumChPerMc    = GetNumChannelPerMc ();

  if ((SocketChannel == NULL) || (McMtrOffset == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (McChannel >= NumChPerMc) {
    *SocketChannel = (McId * ((UINT8) MaxMcChannels)) + McChannel;
    *McMtrOffset = SET_UNSPTD_FUNCBLOCK_MC(MCMTR_MC_MAIN_REG);
  } else {
    *SocketChannel = (McId * NumChPerMc) + McChannel;
    *McMtrOffset = MCMTR_MC_MAIN_REG;
  }

  return EFI_SUCCESS;
}

/**
  Reads a channel mask to all registers/fields required to disable a channel in the MC

  @param[in]  Socket          Socket number to disable channels for
  @param[in]  McId            Memory controller ID

  @retval     Bitmask indicating which channels are disabled in any register
                enabled = 0, disabled = 1
                Bit 0 = MC<McId> CH 0, Bit 1 = MC<McId> CH 1
                Channel numbers are relative to the memory controller
                Note:
                  Multiple registers and fields are used to disable channels. This function
                  returns all of their states OR'd together. The returned mask indicates
                  which channels are disabled in any source, but not necessarily all of them.
**/
UINT32
EFIAPI
GetChannelDisableMaskMc (
  IN UINT8  Socket,
  IN UINT8  McId
  )
{
  PSYSHOST                            Host;
  MCMTR_MC_MAIN_STRUCT                McMtr;
  RCOMP_GLOBALCTRL1_MC_GLOBAL_STRUCT  RcompGlobalCtrl;
  UINT8                               SocketChannel;
  UINT8                               McChannel;
  UINT32                              McMtrOffset;
  UINT32                              MaxMcChannels = (UINT32) FixedPcdGet8 (PcdMaxMcIpChannels);
  UINT32                              ChDisableMask;

  Host = GetSysHostPointer ();

  RcompGlobalCtrl.Data = MemReadPciCfgMC (Socket, McId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG);
  ChDisableMask = RcompGlobalCtrl.Bits.chnl_unpopulated;
  ChDisableMask |= RcompGlobalCtrl.Bits.chnl_disabled;

  for (McChannel = 0; McChannel < MaxMcChannels; McChannel++) {

    if (EFI_ERROR (GetSocketChannelAndMcmtrOffset (McId, McChannel, &SocketChannel, &McMtrOffset))) {
      RcDebugPrint (SDBG_ERROR,
        "McId: %d McChannel: %d Error resolving MCMTR access parameters in GetChannelDisableMaskMc()\n",
        McId, McChannel);
      continue;
    }
    McMtr.Data = MemReadPciCfgEp (Socket, SocketChannel, McMtrOffset);

    ChDisableMask |= (((UINT32) McMtr.Bits.chn_disable) << McChannel);
  }

  return ChDisableMask;
}

/**
  Writes a channel mask to all registers/fields required to disable a channel in the MC

  @param[in]  Socket          Socket number to disable channels for
  @param[in]  McId            Memory controller ID
  @param[in]  ChDisableMask   Bitmask indicating which channels to disable.
                                Leave enabled = 0, disable = 1
                                Bit 0 = MC<McId> CH 0, Bit 1 = MC<McId> CH 1
                                Channel numbers are relative to the memory controller

  @retval     EFI_SUCCESS     Channels successfully disabled
  @retval     !EFI_SUCCESS    An error occurred while disabling channels
**/
EFI_STATUS
EFIAPI
SetChannelDisableMaskMc (
  IN UINT8  Socket,
  IN UINT8  McId,
  IN UINT32 ChDisableMask
  )
{
  PSYSHOST                            Host;
  MCMTR_MC_MAIN_STRUCT                McMtr;
  RCOMP_GLOBALCTRL1_MC_GLOBAL_STRUCT  RcompGlobalCtrl;
  UINT8                               SocketChannel;
  UINT8                               McChannel;
  UINT32                              McMtrOffset;
  UINT32                              MaxMcChannels = (UINT32) FixedPcdGet8 (PcdMaxMcIpChannels);
  EFI_STATUS                          Status = EFI_SUCCESS;

  Host = GetSysHostPointer ();

  RcompGlobalCtrl.Data = MemReadPciCfgMC (Socket, McId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG);
  RcompGlobalCtrl.Bits.chnl_unpopulated = ChDisableMask;
  RcompGlobalCtrl.Bits.chnl_disabled = ChDisableMask;
  MemWritePciCfgMC (Socket, McId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG, RcompGlobalCtrl.Data);

  for (McChannel = 0; McChannel < MaxMcChannels; McChannel++) {

    if (EFI_ERROR (GetSocketChannelAndMcmtrOffset (McId, McChannel, &SocketChannel, &McMtrOffset))) {
      RcDebugPrint (SDBG_ERROR,
        "McId: %d McChannel: %d Error resolving MCMTR access parameters in SetChannelDisableMaskMc()\n",
        McId, McChannel);
      Status = EFI_DEVICE_ERROR;
      continue;
    }

    McMtr.Data = MemReadPciCfgEp (Socket, SocketChannel, McMtrOffset);

    if ((ChDisableMask & (BIT0 << McChannel)) == 0) {
      McMtr.Bits.chn_disable = 0;
    } else {
      McMtr.Bits.chn_disable = 1;
    }
    MemWritePciCfgEp (Socket, SocketChannel, McMtrOffset, McMtr.Data);
  }

  return Status;
}

/**
  Enables all channels in memory controller registers

  @param[in]  Socket          Socket number

  @retval     EFI_SUCCESS     Channels successfully enabled
  @retval     !EFI_SUCCESS    An error occurred during channel enabling
**/
EFI_STATUS
EFIAPI
EnableAllChannelsMc (
  IN  UINT8 Socket
  )
{
  UINT8       McId;
  EFI_STATUS  Status = EFI_SUCCESS;
  PSYSHOST    Host;
  UINT8       MaxImc;

  Host   = GetSysHostPointer();
  MaxImc = GetMaxImc ();
  for (McId = 0; McId < MaxImc; McId++) {
    if (EFI_ERROR (SetChannelDisableMaskMc (Socket, McId, 0))) {
      Status = EFI_DEVICE_ERROR;
    }
  }

  return Status;
}

/**
  Disables channels in MC hardware that are unsupported by the platform

  @param[in]  Socket    Socket number to disable unused channels for

  @retval     EFI_SUCCESS   Unsupported channels successfully disabled
  @retval     !EFI_SUCCESS  An error occurred during the disabling of unsupported channels
**/
EFI_STATUS
EFIAPI
DisableUnsupportedChannelsMc (
  IN  UINT8   Socket
  )
{
  UINT8                  Ch;
  UINT8       NumChPerMc = GetNumChannelPerMc ();
  UINT8                  NumChPerMcFused[MAX_IMC];
  UINT32                 SupportedChMaskPlatform[MAX_IMC];
  UINT32                 UnsupportedChMask[MAX_IMC];
  UINT8       Mc;
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT8                  MaxMcChannels = FixedPcdGet8 (PcdMaxMcIpChannels);
  UINT32      MaxMcChannelsMask = ((1 << MaxMcChannels) - 1);
  UINT8                  ChDisabledMask = 0;
  UINT8       MaxImc;
  UINT8                  FusedChannelsCount;
  UINT8                  FusedChannelsBitMap;
  PSYSHOST               Host;

  Host = GetSysHostPointer ();
  MaxImc = GetMaxImc ();
  SetMem32 (SupportedChMaskPlatform, sizeof (SupportedChMaskPlatform), (1 << NumChPerMc) - 1);
  SetMem (NumChPerMcFused, sizeof (NumChPerMcFused), MaxMcChannels);
  SetMem32 (UnsupportedChMask, sizeof (UnsupportedChMask), SupportedChMaskPlatform[0] ^ MaxMcChannelsMask);

  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    //
    // Read the number of channels enabled in the fuse
    //
    FusedChannelsCount = (Host->var.common.procCom[Socket].capid5 >> 21) & 0xF;
    // ICX-D LCC 1 MC, 2 channels per MC, and ICX-D HCC 2 MC, 2 channels per MC case
    FusedChannelsBitMap = 0x4;
    // ICX-D HCC 1 MC, 3 channels per MC case
    if (FusedChannelsCount == 3) {
      FusedChannelsBitMap = 0;
    }
    //
    // Loop through the channels in one MC to discover what is disabled by fuse
    //
    for (Mc = 0; Mc < MaxImc; Mc++) {
      ChDisabledMask = 0;
      for (Ch = 0; Ch < MaxMcChannels; Ch++) {
        if (FusedChannelsBitMap & (1 << ((Mc*MaxMcChannels) + Ch))) {
          ChDisabledMask |= 1 << Ch;
          NumChPerMcFused[Mc]--;
        }
      }

      if (NumChPerMc != NumChPerMcFused[Mc]) {
        NumChPerMc = NumChPerMcFused[Mc];
        UnsupportedChMask[Mc] = ChDisabledMask;
      }

      RcDebugPrint (SDBG_MAX, "MC%d: Channels disabled in fuse mask = 0x%X, Calculated channel disable mask MC = 0x%X\n", Mc, FusedChannelsBitMap, UnsupportedChMask[Mc]);
    }
  }

  //
  // Nothing to do if the platform and MC support the same number of channels
  //
  if (NumChPerMc == MaxMcChannels) {
    return EFI_SUCCESS;
  }

  for (Mc = 0; Mc < MaxImc; Mc++) {
    if (EFI_ERROR (DisableChannelsMc (Socket, Mc, UnsupportedChMask[Mc]))) {
      Status = EFI_DEVICE_ERROR;
    }
  }

  return Status;
} // DisableUnsupportedChannelsMc

/**
  Disables unused channels in memory controller registers

  Note:

     There is a hardware requirement for channel0 to remain active if all channels on
     a memory controller are going to be disabled. If a channel disable mask is passed in that
     would cause all channels to be disabled, channel0 will be automatically enabled.

  @param[in]  Socket          Socket number
  @param[in]  McId            Memory controller ID
  @param[in]  ChDisableMask   Bitmask indicating which channels to disable.
                                Leave enabled = 0, disable = 1
                                Bit 0 = MC<McId> CH 0, Bit 1 = MC<McId> CH 1
                                Channel numbers are relative to the memory controller

  @retval     EFI_SUCCESS     Channels successfully disabled
  @retval     !EFI_SUCCESS    An error occurred during the channel disabling
**/
EFI_STATUS
EFIAPI
DisableChannelsMc (
  IN  UINT8     Socket,
  IN  UINT8     McId,
  IN  UINT32    ChDisableMask
  )
{
  UINT32  MaxMcChannels = (UINT32) FixedPcdGet8 (PcdMaxMcIpChannels);
  UINT32  MaxMcChannelsMask = ((1 << MaxMcChannels) - 1);

  //
  // Emit warning if disable mask is wider than this MC supports
  //
  if (ChDisableMask > MaxMcChannelsMask) {
    RcDebugPrint (SDBG_WARN,
      "Error disabling channels in MC. Invalid channel mask. McId: %d Channel Disable Mask: 0x%x\n" \
      "Mask will be truncated to maximum number of channels supported by the MC IP\n",
      McId, ChDisableMask);
      ChDisableMask &= MaxMcChannelsMask;
  }

  //
  // Some channels may have been previously disabled due to being supported by the MC IP but
  // not the platform, so we need a RMW cycle to avoid accidentally re-enabling them
  //
  ChDisableMask |= GetChannelDisableMaskMc (Socket, McId);

  //
  // Ensure at least Channel 0 remains enabled to fulfill hardware requirements
  //
  if (ChDisableMask == MaxMcChannelsMask) {
    RcDebugPrint (SDBG_WARN,
      "Cannot disable all requested channels due to hardware requirement.\n" \
      "Socket: %d McId: %d Requested Channel Disable Mask: 0x%x\n" \
      "Channel 0 of this MC will remain enabled.\n",
      Socket, McId, ChDisableMask);
    ChDisableMask &= ~(BIT0);
  }

  return SetChannelDisableMaskMc (Socket, McId, ChDisableMask);
}

/**
  Programs the MC for the correct preamble state

  @param[in] Host   - Pointer to sysHost
  @param[in] Socket - Socket Id
  @param[in] Ch     - Channel number

  @retval VOID
**/
VOID
PreambleInitMc (
  IN      PSYSHOST Host,
  IN      UINT8    Socket,
  IN      UINT8    Ch
  )
{
  UINT8       WritePreambleState;
  UINT8       ReadPreambleState;

  WritePreambleState = GetPreambleState (Socket, WRITE_PREAMBLE);
  ReadPreambleState  = GetPreambleState (Socket, READ_PREAMBLE);

  SetLongPreambleMc (Host, Socket, Ch, WritePreambleState, ReadPreambleState);

}

/** Initialize values and behavior for the Read Pending Queue timer.

  @param[in]  Host     Pointer to SysHost
  @param[in]  Socket   The socket containing the timer to initialize
  @param[in]  Channel  The channel containing the timer to initialize

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
InitializeDdrtReadPendingQueueTimer (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  CPGC_DDRT_ERR_CTL_STS_MC_2LM_STRUCT       CpgcDdrtErrCtlSts;

  // Set credit and timeout values.
  CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
  CpgcDdrtErrCtlSts.Bits.crdt_limit = FixedPcdGet32 (PcdReadPendingQueueTimeoutCreditLimit);
  CpgcDdrtErrCtlSts.Bits.timeout_base_clk = FixedPcdGet32 (PcdReadPendingQueueTimeoutBaseClock);
  CpgcDdrtErrCtlSts.Bits.timeout_threshold = FixedPcdGet32 (PcdReadPendingQueueTimeoutThreshold);

  // Toggle error bits to clear errors and restart the timer. Step 1 of 2: Setting bits.
  CpgcDdrtErrCtlSts.Bits.crdt_limit_hit = 1;
  CpgcDdrtErrCtlSts.Bits.timeout_error = 1;
  MemWritePciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG, CpgcDdrtErrCtlSts.Data);

  // Toggle error bits to clear errors and restart the timer. Step 2 of 2: Clearing bits.
  CpgcDdrtErrCtlSts.Data = MemReadPciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG);
  CpgcDdrtErrCtlSts.Bits.crdt_limit_hit = 0;
  CpgcDdrtErrCtlSts.Bits.timeout_error = 0;
  MemWritePciCfgEp (Socket, Channel, CPGC_DDRT_ERR_CTL_STS_MC_2LM_REG, CpgcDdrtErrCtlSts.Data);

  return EFI_SUCCESS;

}  // InitializeDdrtReadPendingQueueTimer

/**
  Enable back-2-back writes in CPGC mode for DDRT

  @param[in]  Host     Pointer to SysHost
  @param[in]  Socket   The socket containing the timer to initialize
  @param[in]  Channel  The channel containing the timer to initialize

  @retval EFI_SUCCESS   No errors were encountered
  @retval !EFI_SUCCESS  An error was encountered

**/
EFI_STATUS
InitializeDdrtB2bCpgcWrites (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
#ifndef GNRSRF_HOST
  DDRT_MISC_CTL_MC_2LM_STRUCT               DdrtMiscCtl;

  DdrtMiscCtl.Data = MemReadPciCfgEp (Socket, Channel, DDRT_MISC_CTL_MC_2LM_REG);
  DdrtMiscCtl.Bits.cpgc_en_wpq_early_dealloc = 1;
  MemWritePciCfgEp (Socket, Channel, DDRT_MISC_CTL_MC_2LM_REG, DdrtMiscCtl.Data);

#endif // GNRSRF_HOST
  return EFI_SUCCESS;
}

/**

  This routine gets memory ready to be written and read for NVMDIMM

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
VOID
EarlyDdrtConfig (
  PSYSHOST                  Host,
  UINT8                     socket
  )
{
#ifdef DDRT_SUPPORT
  DDRT_TIMING_MC_2LM_STRUCT                 ddrtTiming;
  T_PERSIST_TIMING_MC_2LM_STRUCT            persistTiming;
  T_DDRT_MISC_DELAY_MC_2LM_STRUCT           ddrtMiscDelay;
  DDRT_MAJOR_MODE_THRESHOLD2_MC_2LM_STRUCT  ddrtMajorModeTh2;
  DDRT_CREDIT_LIMIT_MC_2LM_STRUCT           ddrtCreditLimit;
  T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_STRUCT   ddrtDimm0BasicTiming;
  T_DDRT_WR_CNSTRNT_MC_2LM_STRUCT           DdrtWrCnstrnt;
  CHN_TEMP_CFG_MCDDC_CTL_STRUCT             ChnTempCfgReg;
  DIMM_TEMP_EV_OFST_0_MCDDC_CTL_STRUCT      dimmTempEvOfst;
  DDRT_ERROR_MC_2LM_STRUCT                  errSignals;
  DDRT_ECC_MODE_MC_2LM_STRUCT               ddrtEccMode;
  MODE_DELAYS_MC_2LM_STRUCT                 modeDelays;
  DDRT_DEFEATURE_MC_2LM_STRUCT              ddrtDefeature;
  CMPL_TO_DATA_DELAY_MCDDC_DP_STRUCT        cmplDataDelay;
  DDRT_CLK_GATING_MC_2LM_STRUCT             DdrtClkGating;
  UINT8                                     ch;
  struct channelNvram                       (*channelNvList)[MAX_CH];
  struct dimmNvram                          (*DimmNvList)[MAX_DIMM];
  UINT32                                    DimmTempEvOfstRegOffset[] = {DIMM_TEMP_EV_OFST_0_MCDDC_CTL_REG, DIMM_TEMP_EV_OFST_1_MCDDC_CTL_REG};
  UINT8                                     dimm;
  SYS_SETUP                                 *Setup;
  INT16                                     Gnt2Erid;
  INT16                                     Gnt2Erid_Bwv;
  BootMode                                  SysBootMode;
  UINT8                                     MaxChDdr;

  Setup = GetSysSetupPointer ();
  SysBootMode = GetSysBootMode ();

  if (Host->nvram.mem.socket[socket].DcpmmPresent == 0) {
    return;
  }
  RcDebugPrintWithDevice (SDBG_MEM_TRAIN, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "EarlyPmemConfig Starts\n");

  if (UbiosGenerationOrHsleEnabled ()) {
    if (Setup->common.ddrtXactor == 1) {
      Gnt2Erid = GNT2ERID_START_EMULATION;
    } else {
      Gnt2Erid = GNT2ERID_START_COEMULATION;
    }
  } else {
    Gnt2Erid = GNT2ERID_START;
  }

  //
  // Minimum Grant to Early Read ID has increased by 5 for FMC_BWV_TYPE
  //
  Gnt2Erid_Bwv = Gnt2Erid + 5;

  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr      = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    ChnTempCfgReg.Data = MemReadPciCfgEp (socket, ch, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG);
    ChnTempCfgReg.Bits.oltt_en = 0;
    ChnTempCfgReg.Bits.bw_limit_thrt_en = 0;
    ChnTempCfgReg.Bits.thrt_allow_isoch = 1;
    MemWritePciCfgEp (socket, ch, ALIAS_CHN_TEMP_CFG_MCDDC_CTL_REG, ChnTempCfgReg.Data);

    DimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      dimmTempEvOfst.Data = MemReadPciCfgEp (socket, ch, DimmTempEvOfstRegOffset[dimm]);
      dimmTempEvOfst.Bits.ev_2x_ref_en = 0;
      MemWritePciCfgEp (socket, ch, DimmTempEvOfstRegOffset[dimm], dimmTempEvOfst.Data);
      //
      // Set the Dimm/Rank as DDRT
      //
      if ((*DimmNvList)[dimm].DcpmmPresent == 1) {
        SetDdrioRankTypeDdrt (socket, ch, dimm);
      }
    }

    if ((*channelNvList)[ch].ddrtEnabled == 0) {
      continue;
    }

    //
    // Early config for DDRT2
    //
    EarlyDdrt2Config (Host, socket, ch);

    // If DDRT scheduler debug is enabled in setup, enable now
    if (Setup->mem.dfxMemSetup.DfxDdrtSchedulerDebug == DDRT_SCHEDULER_DBG_ENABLE) {
      DdrtClkGating.Data = MemReadPciCfgEp (socket, ch, DDRT_CLK_GATING_MC_2LM_REG);
      DdrtClkGating.Bits.en_ddrt_dfd_clk =  1;
      MemWritePciCfgEp (socket, ch, DDRT_CLK_GATING_MC_2LM_REG, DdrtClkGating.Data);
    }

    //
    // This gets recalculated during LateConfig()
    cmplDataDelay.Data = MemReadPciCfgEp (socket, ch, CMPL_TO_DATA_DELAY_MCDDC_DP_REG);
    cmplDataDelay.Bits.cmpl_program_delay = 4;
    MemWritePciCfgEp (socket, ch, CMPL_TO_DATA_DELAY_MCDDC_DP_REG, cmplDataDelay.Data);

    ddrtTiming.Data = MemReadPciCfgEp (socket, ch, DDRT_TIMING_MC_2LM_REG);
    ddrtTiming.Bits.rid2dealloc = 6;
    MemWritePciCfgEp (socket, ch, DDRT_TIMING_MC_2LM_REG, ddrtTiming.Data);

    ddrtDimm0BasicTiming.Data = MemReadPciCfgEp (socket, ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG);
    ddrtDimm0BasicTiming.Bits.t_ddrt_twl = GetDdrtTwl(Host, socket, ch);
    MemWritePciCfgEp (socket, ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG, ddrtDimm0BasicTiming.Data);

    SetDdrtCwlDdrio (Host, socket, ch, ddrtDimm0BasicTiming.Bits.t_ddrt_twl);

    if ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*DimmNvList)[dimm].fmcType >= FMC_BWV_TYPE) {
          GetSetSxpGnt2Erid (Host, socket, ch, dimm, GSM_READ_CSR | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &Gnt2Erid_Bwv);
        } else {
          GetSetSxpGnt2Erid (Host, socket, ch, dimm, GSM_READ_CSR | GSM_FORCE_WRITE | GSM_UPDATE_CACHE, &Gnt2Erid);
        }
      }
    }

    persistTiming.Data = MemReadPciCfgEp (socket, ch, T_PERSIST_TIMING_MC_2LM_REG);
#ifndef GNRSRF_HOST
    persistTiming.Bits.t_gnt_dq = 2;
#endif // GNRSRF_HOST
    persistTiming.Bits.t_persist_delay = 0;
    MemWritePciCfgEp (socket, ch, T_PERSIST_TIMING_MC_2LM_REG, persistTiming.Data);

    ddrtCreditLimit.Data = MemReadPciCfgEp (socket, ch, DDRT_CREDIT_LIMIT_MC_2LM_REG);
    ddrtCreditLimit.Bits.ddrt_rd_credit = 0;
    ddrtCreditLimit.Bits.ddrt_wr_credit = 0;
    MemWritePciCfgEp (socket, ch, DDRT_CREDIT_LIMIT_MC_2LM_REG, ddrtCreditLimit.Data);

    errSignals.Data = MemReadPciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG);
    errSignals.Bits.ignore_ddrt_ecc_error = 1;
    errSignals.Bits.ignore_ddrt_err0 = 1;
    errSignals.Bits.ignore_ddrt_err1 = 1;
    errSignals.Bits.ignore_ddr4_error = 1;
    errSignals.Bits.ignore_erid_parity_error = 1;
    MemWritePciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG, errSignals.Data);

    InitRelaxTurnAroundTimesDcpmm (socket, ch);

    //
    // Program DDRT RD/WR/GNT to Power Down Timing Constraints.
    //
    ConfigDdrtPowerDownTiming (Host, socket, ch, 1, 1, (UINT8)(GettCL (Host, socket, ch) + DDRT_PDEN_ADJUST));

    ddrtMajorModeTh2.Data = MemReadPciCfgEp (socket, ch, DDRT_MAJOR_MODE_THRESHOLD2_MC_2LM_REG);
    ddrtMajorModeTh2.Bits.gnt_exit = 1;
    ddrtMajorModeTh2.Bits.pwr_enter = 1;
    ddrtMajorModeTh2.Bits.pwr_exit = 1;
    MemWritePciCfgEp (socket, ch, DDRT_MAJOR_MODE_THRESHOLD2_MC_2LM_REG, ddrtMajorModeTh2.Data);

    ddrtMiscDelay.Data = MemReadPciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG);
#ifndef GNRSRF_HOST
    ddrtMiscDelay.Bits.rpq_rid_to_credit_ret = 4;
    ddrtMiscDelay.Bits.wpq_rid_to_fwr = 4;
#endif // GNRSRF_HOST
    ddrtMiscDelay.Bits.wpq_dealloc_to_credit_ret = 2;
//    ddrtMiscDelay.Bits.wpq_rid_to_rt_ufill = 2;
    MemWritePciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG, ddrtMiscDelay.Data);

    SetCpgcDdrtMiscCtl (
      Host,
      socket,
      ch,
      TRUE,  //  ChangeMaxCredit,
      0x20,  //  CpgcMaxCredit,
      FALSE, //  DisableMaxCreditCheck,
      TRUE,  //  EnableEridReturn,
      TRUE   //  MultiCreditOn
      );

    ddrtEccMode.Data = MemReadPciCfgEp (socket, ch, DDRT_ECC_MODE_MC_2LM_REG);

    SetFnvEccModeTarget ((VOID *)&ddrtEccMode);

    MemWritePciCfgEp (socket, ch, DDRT_ECC_MODE_MC_2LM_REG, ddrtEccMode.Data);

    modeDelays.Data = MemReadPciCfgEp (socket, ch, MODE_DELAYS_MC_2LM_REG);
    modeDelays.Bits.ddr4_to_ddrt_delay = 0x20;
    modeDelays.Bits.ddrt_to_ddr4_delay = 0x20;
    MemWritePciCfgEp (socket, ch, MODE_DELAYS_MC_2LM_REG, modeDelays.Data);

    DdrtWrCnstrnt.Data = MemReadPciCfgEp (socket, ch, T_DDRT_WR_CNSTRNT_MC_2LM_REG);
    DdrtWrCnstrnt.Bits.t_ddrt_wrwr_s = 8;
    MemWritePciCfgEp (socket, ch, T_DDRT_WR_CNSTRNT_MC_2LM_REG, DdrtWrCnstrnt.Data);

    // 4929208: MC RPQ command starves when read credit remains 1 and BIDS Clears out the counters
    ddrtDefeature.Data = MemReadPciCfgEp (socket, ch, DDRT_DEFEATURE_MC_2LM_REG);
    // no stepping check, as the RTL bug is not going to be fixed until GO
    ddrtDefeature.Bits.ignore_erid_parity_error = 1;
    ddrtDefeature.Data |= (BIT25+BIT24);
    //4929239: PowerOnSafe Setting: DDRT_DEFEATURE.block_gnt2cmd_1cyc should be set to 1
#ifndef GNRSRF_HOST
    if (Setup->mem.Blockgnt2cmd1cyc == 0) {
      ddrtDefeature.Bits.block_gnt2cmd_1cyc = 1; //PO Safe value
    } else {
      ddrtDefeature.Bits.block_gnt2cmd_1cyc = 0; //POR value
    }
#endif // GNRSRF_HOST
    MemWritePciCfgEp (socket, ch, DDRT_DEFEATURE_MC_2LM_REG, ddrtDefeature.Data);

    InitializeDdrtReadPendingQueueTimer (Host, socket, ch);

    InitializeDdrtB2bCpgcWrites (Host, socket, ch);

    //
    // Set DDRT_DATAPATH_DELAY.WR to 0x7, the length of time the data path should be held in DDRT mode
    //
    InitializeDdrtWrPathHoldTime (Host, socket, ch, DDRT_DATAPATH_DELAY_WR);

    //
    // 1707024887 Disable Thermal Event in the Early Ddrt Config
    //
    SetDefeatureThermalEvent (socket, ch, NotInNormalMode);
  }  // Channel loop

  //
  // Start FMC cache access
  //
  Host->nvram.mem.FmcCacheDone = 0;
#endif // DDRT_SUPPORT
}

/**

  Programs the WrWr_s for ddrt2 ranks

  @param Host    - Pointer to sysHost
  @param Socket  - Processor to initialize
  @param Value   - Wr Wr S delay
  @retval N/A

**/

VOID
ProgramDdrt2WrWrS (
  PSYSHOST                  Host,
  UINT8                     Socket,
  UINT8                     Value
  )
{
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  UINT8                                     Ch;
  UINT8                                     MaxChDdr;
  T_DDRT_WR_CNSTRNT_MC_2LM_STRUCT           DdrtWrCnstrnt;

  ChannelNvList = GetChannelNvList(Host, Socket);
  MaxChDdr      = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    if ((*ChannelNvList)[Ch].ddrtEnabled == 0) {
      continue;
    }
    DdrtWrCnstrnt.Data = MemReadPciCfgEp (Socket, Ch, T_DDRT_WR_CNSTRNT_MC_2LM_REG);
    DdrtWrCnstrnt.Bits.t_ddrt_wrwr_s = Value;
    MemWritePciCfgEp (Socket, Ch, T_DDRT_WR_CNSTRNT_MC_2LM_REG, DdrtWrCnstrnt.Data);
  }
}

/**
  Programs PC6 chicken bits across all memory controllers according to DDRT population

  @param[in] Host     Pointer to sysHost
  @param[in] Socket   Processor to initialize
**/
VOID
SetDdrtPC6ChickenBits (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  MCMNT_CHKN_BIT_MCDDC_CTL_STRUCT   McmntChknBit;
  UINT8                             Channel;
  BOOLEAN                           DdrtPresentMc[MAX_IMC];
  UINT8                             McIndex;
  UINT8                             MaxChDdr;
  UINT8                             MaxImc;

  MaxImc = GetMaxImc ();

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    return;
  }

  for (McIndex = 0; McIndex < MaxImc; McIndex++) {
    if (IsDdrtPresentMC (Socket, McIndex)) {
      DdrtPresentMc[McIndex] = TRUE;
    } else {
      DdrtPresentMc[McIndex] = FALSE;
    }
  }

  //
  // Program en_ddrt_pc6_clkstp_fsm
  // This bit must be set *on all channels* if the MC has any DDRT DIMM present, and must
  // be cleared if not
  //
  MaxChDdr = GetMaxChDdr ();
  for (Channel = 0; Channel < MaxChDdr; Channel++) {

    McIndex = GetMCID (Host, Socket, Channel);
    McmntChknBit.Data = MemReadPciCfgEp (Socket, Channel, MCMNT_CHKN_BIT_MCDDC_CTL_REG);
    if (McIndex < MaxImc){
      if (DdrtPresentMc[McIndex]) {
        McmntChknBit.Bits.en_ddrt_pc6_clkstp_fsm = 1;
      } else {
        McmntChknBit.Bits.en_ddrt_pc6_clkstp_fsm = 0;
      }
    } else {
      return;
    }

    MemWritePciCfgEp (Socket, Channel, MCMNT_CHKN_BIT_MCDDC_CTL_REG, McmntChknBit.Data);
  }

  return;
}

#define MAX_CSR_WRITE 10000
/**

  This routine measures the MC CSR access time using both SW functions and direct MMIO access

  @param Host  - Pointer to sysHost

  @retval N/A

**/
VOID
MeasureCsrAccessTime (
  PSYSHOST  Host
)
{
#ifdef  DEBUG_CODE_BLOCK
  UINT64             startTsc = 0;
  UINT64             endTsc = 0;
  UINT32             i;
  UINT32             tempVal;
  UINT32             testValue;
  UINT32             result;
  UINTN              TestPtr[MAX_CH];
  UINTN              TestPtr2[MAX_CH];
  SYS_SETUP          *Setup;
  UINT8              socket;
  UINT8              ch;
  UINT8              MaxChDdr;
  BOOLEAN            PostedWritesSupported;

  PostedWritesSupported = PcdGetBool (PcdPostedCsrAccessSupported);
  //
  // Skip the timing measurement for Warm Fast Boot, Cold Fast Boot, and S3
  //
  if (IsFastBootPath ()) {
    return;
  }

  socket = Host->var.mem.currentSocket;
  MaxChDdr = GetMaxChDdr ();

  if (!(GetEmulation () & SIMICS_FLAG)) {
    //
    // Test non-posted writes
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      startTsc = GetPerformanceCounter ();
      for (i = 0; i < MAX_CSR_WRITE; i++) {
        MemWritePciCfgEp (socket, ch, MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG, 0);
      }
      endTsc = GetPerformanceCounter ();
      RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG access function - average write latency = %d ns\n",
        TimeDiff (startTsc, endTsc, TDIFF_UNIT_NS) / MAX_CSR_WRITE);

      Host->nvram.mem.socket[socket].CsrWriteLatency[ch] = TimeDiff (startTsc, endTsc, TDIFF_UNIT_NS) / MAX_CSR_WRITE;
    }

    for (ch = 0; ch < MaxChDdr; ch++) {
      startTsc = GetPerformanceCounter ();
      for (i = 0; i < MAX_CSR_WRITE; i++) {
        tempVal = MemReadPciCfgEp (socket, ch, MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG);
      }
      endTsc = GetPerformanceCounter ();
      RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG access function - average read latency = %d ns\n",
        TimeDiff (startTsc, endTsc, TDIFF_UNIT_NS) / MAX_CSR_WRITE);
      Host->nvram.mem.socket[socket].CsrReadLatency[ch] = TimeDiff (startTsc, endTsc, TDIFF_UNIT_NS) / MAX_CSR_WRITE;
    }

    if (GetDebugLevel () & SDBG_MAX) {

      Setup = GetSysSetupPointer ();

      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "CSR latency measurement begins\n");

      if (PostedWritesSupported) {
        //
        // Initialize the MMIO addresses
        //
        for (ch = 0; ch < MaxChDdr; ch++) {
          TestPtr[ch] = UsraGetCsrRegisterAddress (socket, ch, MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG);
          PostedMethodEnable ();
          TestPtr2[ch] = UsraGetCsrRegisterAddress (socket, ch, MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG);
          PostedMethodDisable ();
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG address: non-posted = 0x%08x, posted = 0x%08x\n",
            TestPtr[ch], TestPtr2[ch]);
        }
      } // PostedWritesSupported

      if (PostedWritesSupported) {
        //
        // Test posted writes
        //
        testValue = 0xaa55;
        PostedMethodEnable ();
        for (ch = 0; ch < MaxChDdr; ch++) {
          startTsc = GetPerformanceCounter ();
          for (i = 0; i < MAX_CSR_WRITE; i++) {
            MemWritePciCfgEp (socket, ch, MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG, testValue);
          }
          endTsc = GetPerformanceCounter ();
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG function posted - average write latency = %d ns\n",
            TimeDiff (startTsc, endTsc, TDIFF_UNIT_NS) / MAX_CSR_WRITE);
        }
        for (ch = 0; ch < MaxChDdr; ch++) {
          startTsc = GetPerformanceCounter ();
          for (i = 0; i < MAX_CSR_WRITE; i++) {
            tempVal = MemReadPciCfgEp (socket, ch, MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG);
          }
          endTsc = GetPerformanceCounter ();
          if (testValue == tempVal) {
            result = 1;
          } else {
            result = 0;
          }
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG access function - average read latency = %d ns, testResult = %d, testValue = 0x%08x, readValue = 0x%08x\n",
            TimeDiff (startTsc, endTsc, TDIFF_UNIT_NS) / MAX_CSR_WRITE, result, testValue, tempVal);
        }
        PostedMethodDisable ();

        testValue = 0x5a5a;
        for (ch = 0; ch < MaxChDdr; ch++) {
          startTsc = GetPerformanceCounter ();
          for (i = 0; i < MAX_CSR_WRITE; i++) {
            *(volatile UINT32 *)TestPtr[ch] = testValue;
          }
          endTsc = GetPerformanceCounter ();
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG direct MMIO - average write latency = %d ns\n",
            TimeDiff (startTsc, endTsc, TDIFF_UNIT_NS) / MAX_CSR_WRITE);
        }
        for (ch = 0; ch < MaxChDdr; ch++) {
          startTsc = GetPerformanceCounter ();
          for (i = 0; i < MAX_CSR_WRITE; i++) {
            tempVal = *(volatile UINT32 *)TestPtr[ch];
          }
          endTsc = GetPerformanceCounter ();
          if (testValue == tempVal) {
            result = 1;
          } else {
            result = 0;
          }
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG direct MMIO - average read latency = %d ns, testResult = %d, testValue = 0x%08x, readValue = 0x%08x\n",
            TimeDiff (startTsc, endTsc, TDIFF_UNIT_NS) / MAX_CSR_WRITE, result, testValue, tempVal);
        }

        //
        // Enable Posted CSR writes
        //
        testValue = 0xa5a5;
        for (ch = 0; ch < MaxChDdr; ch++) {
          startTsc = GetPerformanceCounter ();
          for (i = 0; i < MAX_CSR_WRITE; i++) {
            *(volatile UINT32 *)TestPtr2[ch] = testValue;
          }
          endTsc = GetPerformanceCounter ();
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG MMIO posted - average write latency = %d ns\n",
            TimeDiff (startTsc, endTsc, TDIFF_UNIT_NS) / MAX_CSR_WRITE);
        }
        for (ch = 0; ch < MaxChDdr; ch++) {
          startTsc = GetPerformanceCounter ();
          for (i = 0; i < MAX_CSR_WRITE; i++) {
            tempVal = *(volatile UINT32 *)TestPtr[ch];
          }
          endTsc = GetPerformanceCounter ();
          if (testValue == tempVal) {
            result = 1;
          } else {
            result = 0;
          }
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG direct MMIO - average read latency = %d ns, testResult = %d, testValue = 0x%08x, readValue = 0x%08x\n",
            TimeDiff (startTsc, endTsc, TDIFF_UNIT_NS) / MAX_CSR_WRITE, result, testValue, tempVal);
        }
      } // PostedWritesSupported
    } // if (GetDebugLevel () & SDBG_MAX) {
  } // if (!(GetEmulation () & SIMICS_FLAG))

#endif  // DEBUG_CODE_BLOCK
} // MeasureCsrAccessTime

/**

  This routine gets memory ready to be written and read

  @param Host  - Pointer to sysHost
  @param socket  - Processor to initialize

  @retval N/A

**/
UINT32
EarlyConfig (
  PSYSHOST  Host
  )
{
  UINT8                                           socket;
  UINT8                                           ch;
  UINT8                                           dimm;
  UINT8                                           rank;
  UINT8                                           x4Mode;
  UINT8                                           nCLOrg;
  UINT8                                           casSupBase;
  UINT8                                           maxCL;
  BOOLEAN                                         NormalDimmPresentCh;
  struct channelNvram                             (*channelNvList)[MAX_CH];
  struct dimmNvram                                (*dimmNvList)[MAX_DIMM];
  struct ddrRank                                  (*rankList)[MAX_RANK_DIMM];
  MCMTR_MC_MAIN_STRUCT                            mcMtr;
  MC_INIT_STATE_G_MC_MAIN_STRUCT                  mcInitStateG;
  MCMNT_CHKN_BIT_MCDDC_CTL_STRUCT                 mcmntChknBit;
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT                scrambleConfig;
  MCMNT_CHKN_BIT2_MCDDC_CTL_STRUCT                mcMntChknBit2;
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT             MemTimingsAdjReg;
  MCSCRAMBLE_SEED_SEL_MCDDC_DP_STRUCT             scrambleSeedSel;
  BIOS_SCRATCHPAD0_STRUCT                         BiosScratchPad0;
  MCSCRAMBLE_SEED_SEL_DDRT_MCDDC_DP_STRUCT        scrambleSeedSelDdrt;
  EFI_STATUS                                      Status;
#if defined(DEBUG_CODE_BLOCK) && defined(DDRT_SUPPORT)
  UINT8                               CASTemp;
  UINT8                               CWLTemp;
  UINT8                               ClAdd;
  UINT8                               CwlAdd;
#endif //DEBUG_CODE_BLOCK && DDRT_SUPPORT
  UINT8                               tempData;
  UINT8                               IncrementRateforCasLatencies;
  BootMode                            SysBootMode;
  SYS_SETUP                           *Setup;
  UINT8                               MaxChDdr;

  Setup = GetSysSetupPointer ();

  SysBootMode = GetSysBootMode ();

  socket = Host->var.mem.currentSocket;

  if (!IsMemFlowEnabled (EarlyMemoryControllerConfig)) {
    return SUCCESS;
  }

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  DisClkGateSchedMc (Host, socket);

  DisRefDqsOscWaitDuringRetry (Host, socket);

  //
  // Measure the CSR access time
  //
  MeasureCsrAccessTime (Host);

  //
  // Enable posted CSR writes
  //
  PostedMethodEnable ();

  EarlyConfigCmdToDataCompletionDelay (socket);

  if (Setup->mem.options & SCRAMBLE_EN) {
    if ((SysBootMode == NormalBoot) &&
        ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == WarmBoot))) {

      //
      // Verify the value is in range if not use default.
      //
      if (Setup->mem.scrambleSeedLow > MAX_SCRAMBLE_SEED_LOW){
        Host->nvram.mem.scrambleSeed = (Setup->mem.scrambleSeedHigh << 16) | SCRAMBLE_SEED_LOW;
      } else {
        Host->nvram.mem.scrambleSeed = (Setup->mem.scrambleSeedHigh << 16) | Setup->mem.scrambleSeedLow;
      }
    }
  } // if scrambling

  channelNvList = GetChannelNvList(Host, socket);
  //
  // Configure memory mode
  //
  ConfigureMemoryMode (Host, socket);
  MaxChDdr = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    OutputCheckpoint (CHECKPOINT_MAJOR_GLOBAL_EARLY, CHECKPOINT_MINOR_CLOCK_DISABLE, 0);
    SetRankEnDdrio (Host, socket, ch, 0);
  }

  //
  // Set BIOSSCRATCHPAD0.TrainingMode Bit[9] to indicate iMC enter CPGC training mode
  //
  if ((SysBootMode != S3Resume) && (Host->nvram.mem.DcpmmPresent == 1)){
    BiosScratchPad0.Data = GetStickyScratchpad (socket, BIOS_STICKY_SCRATCHPAD_0);
    BiosScratchPad0.Bits.TrainingMode = 0x1;
    SetStickyScratchpad (socket, BIOS_STICKY_SCRATCHPAD_0, BiosScratchPad0.Data);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    mcMtr.Data = MemReadPciCfgEp (socket, ch, MCMTR_MC_MAIN_REG);
    //
    // Enable ECC if required
    //
    if (Host->nvram.mem.eccEn) {
      if (IsBrsPresent (Host, socket)) {
        //
        // BRS RxDq Fail Silicon WA
        //
        mcMtr.Bits.ecc_en = 0;
      }
      else {
        mcMtr.Bits.ecc_en = 1;
      }
    }

    //
    // Enter CPGC training mode
    //
    mcMtr.Bits.trng_mode = 1;
    //
    // Disable any HBM - Only features
    //
    mcMtr.Bits.read_dbi_en  = 0;
    mcMtr.Bits.write_dbi_en = 0;
    MemWritePciCfgEp (socket, ch, MCMTR_MC_MAIN_REG, mcMtr.Data);
    //
    // Enable the DCLK
    //
    mcInitStateG.Data = MemReadPciCfgEp (socket, ch, MC_INIT_STATE_G_MC_MAIN_REG);
    if (SysBootMode != S3Resume){
      mcInitStateG.Bits.dclk_enable = 1;
    }
    MemWritePciCfgEp (socket, ch, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
  } // ch loop

  //
  // Check if there are secrets in memory
  //
  CheckSecrets10nm(Host);

  SetDdrtPC6ChickenBits (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Set the wr_cmpl_fifo_crdt_dis bit
    //
    WriteCompletionCredits (Host, socket, ch, TRUE);

    //
    // Insure Scrambling Disable for Training
    //
    scrambleConfig.Data = MemReadPciCfgEp (socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG);      // this clears all the enables

    // Same 16-bit value used in lower and upper seed select
    if (scrambleConfig.Bits.seed_lock == 0) {
      scrambleSeedSel.Bits.scrb_lower_seed_sel = Host->nvram.mem.scrambleSeed & 0xFFFF;
      scrambleSeedSel.Bits.scrb_upper_seed_sel = Host->nvram.mem.scrambleSeed >> 16;
      MemWritePciCfgEp (socket, ch, MCSCRAMBLE_SEED_SEL_MCDDC_DP_REG, scrambleSeedSel.Data);

      //
      // 1408769603: DCPMM only has 16-bit for descrambling, need to initial ddrt upper seed equal to ddrt lower seed
      //
      scrambleSeedSelDdrt.Bits.scrb_lower_seed_sel_ddrt = Host->nvram.mem.scrambleSeed & 0xFFFF;
      scrambleSeedSelDdrt.Bits.scrb_upper_seed_sel_ddrt = Host->nvram.mem.scrambleSeed & 0xFFFF;
      MemWritePciCfgEp (socket, ch, MCSCRAMBLE_SEED_SEL_DDRT_MCDDC_DP_REG, scrambleSeedSelDdrt.Data);
    }
    scrambleConfig.Data = 0;            // clear all the enable bits
    scrambleConfig.Bits.seed_lock = 1;  // this may have been locked already, but we need it locked in either case
    MemWritePciCfgEp (socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, scrambleConfig.Data);

    mcmntChknBit.Data = MemReadPciCfgEp (socket, ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG);
    mcmntChknBit.Bits.dis_ck_gate_mnts = 0;
    MemWritePciCfgEp (socket, ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG, mcmntChknBit.Data);

    if (IsSiliconWorkaroundEnabled ("S1909216002")) {
      //
      // 4929010: Cloned From SKX Si Bug Eco: CKE chaging when MC2GDCompUpdate Happens
      //
      mcMntChknBit2.Data = MemReadPciCfgEp (socket, ch, MCMNT_CHKN_BIT2_MCDDC_CTL_REG);
      mcMntChknBit2.Bits.dis_cke_pri_rcb_gate = 1;
      MemWritePciCfgEp (socket, ch, MCMNT_CHKN_BIT2_MCDDC_CTL_REG, mcMntChknBit2.Data);
    }

    ConfigRidMrrEntry (Host, socket, ch, FALSE);

    DisRtUnexpSchedActive (socket, ch);
  } //ch

  if ((SysBootMode == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot)) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      dimmNvList  = GetDimmNvList(Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

          rankList = GetRankNvList(Host, socket, ch, dimm);
          for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }

          //
          // Construct CKE mask
          //
          (*channelNvList)[ch].ckeMask |= 1 << (*rankList)[rank].CKEIndex;

          //
          // Call DDRIO hook to set RxOffset
          //
          SetRxOffsetDdrio (Host, socket, ch, dimm, rank);

        } // rank loop
      } // dimm loop

      //
      // tRCD must be at least 5
      //
      if ((*channelNvList)[ch].common.nRCD < 5) {
        (*channelNvList)[ch].common.nRCD = 5;
      }

      //
      // tRP must be at least 5
      //
      if ((*channelNvList)[ch].common.nRP < 5) {
        (*channelNvList)[ch].common.nRP = 5;
      }

      //
      // tWTR_S must be at least 2
      //
      if ((*channelNvList)[ch].common.nWTR < 2) {
        (*channelNvList)[ch].common.nWTR = 2;
      }
      //
      // tWTR_L must be at least 4
      //
      if ((*channelNvList)[ch].common.nWTR_L < 4) {
        (*channelNvList)[ch].common.nWTR_L = 4;
      }

      //
      // tRTP must be at least 4
      //
      if ((*channelNvList)[ch].common.nRTP < 4) {
        (*channelNvList)[ch].common.nRTP = 4;
      }

      //
      // tRRD must be at least 4
      //
      if ((*channelNvList)[ch].common.nRRD < 4) {
        (*channelNvList)[ch].common.nRRD = 4;
      }

    } // ch loop
  } // S3

  for (ch = 0; ch < MaxChDdr; ch += GetChInc (Host)) {

    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Initialize to 0
    //
    x4Mode      = 0;
    NormalDimmPresentCh = FALSE;

    dimmNvList  = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < Host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      //
      // Write the DIMM Memory Technology information to DIMMMTR
      //
      MemWritePciCfgEp (socket, ch, GetDimmMtrRegOffset (dimm), (UINT32) (*dimmNvList)[dimm].dimmMemTech);

      //
      // Check if this is a x4 DIMM
      //
      if ((*dimmNvList)[dimm].x4Present) {
        x4Mode |= (1 << dimm);
      }

      if (((*dimmNvList)[dimm].DcpmmPresent == 1)) {
        continue;
      }
      // if code come here there is at least 1 DDR4/DDR5 dimm present per channel
      NormalDimmPresentCh = TRUE;
    } // dimm loop

    //
    // Set bits indicating which DIMMs are x4 if any
    //
    if (x4Mode) {
      MemWritePciCfgEp (socket, ch, X4MODESEL_MCDDC_DP_REG, x4Mode);
    }

    if (SysBootMode == NormalBoot) {

      //
      // Make sure the selected CAS Latency is supported by each DIMM on this channel
      //
      IncrementRateforCasLatencies = 1;
      casSupBase = 7;
      maxCL = 36;
      if ((*channelNvList)[ch].common.casSupRange == HIGH_CL_RANGE) {
        casSupBase = 23;
        maxCL = 52;
      }

      nCLOrg = (*channelNvList)[ch].common.nCL;

      if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
        //
        // CAS latencies supported in the SPD spec are in increments of 2
        //
        IncrementRateforCasLatencies = 2;
        casSupBase = 20;
        maxCL      = 98;
        //
        // Make sure the nCL is an even number
        //
        if ((*channelNvList)[ch].common.nCL % 2) {
          (*channelNvList)[ch].common.nCL++;
        }

        CteConfiguration (Host, socket, ch, CTE_CONFIG_UPDATE_CL, 22);
      }

      IncreaseReadWritePreamble10nm(Host, socket, ch);

      PreambleInitMc (Host, socket, ch);

      //
      // DDRT DIMM do not need common.nCL
      // So Skip the logic if there is no DDR4/DDR5 DIMM in this channel since it will not programmed.
      //
      if (NormalDimmPresentCh) {
        //
        // Check if the nCL value is valid or not
        //
        if ((*channelNvList)[ch].common.nCL >= casSupBase) {
          // make sure CL is supported, if not, increment to next setting
          while (!(LShiftU64 (1, (((*channelNvList)[ch].common.nCL - casSupBase) >> (IncrementRateforCasLatencies - 1))) & (*channelNvList)[ch].common.casSup)) {
            (*channelNvList)[ch].common.nCL += IncrementRateforCasLatencies;

            if ((*channelNvList)[ch].common.nCL > maxCL) {
              // Use the original version of nCL if we can't find a good one
              (*channelNvList)[ch].common.nCL = nCLOrg;
              Host->var.mem.read2tckCL[ch] = 0;
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "tCL could not be found in the list of supported tCL settings found in the SPD\n");
              break;
            }
          } // (while)
        } else {
          RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "\nERROR: invalid tCL value %d! Please check SPD data for Minimum CAS Latency Time (tAAmin)\n", (*channelNvList)[ch].common.nCL);
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_202);
          (*channelNvList)[ch].common.nCL = casSupBase;
          Host->var.mem.read2tckCL[ch] = 0;
        }
      }

      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "tCL is set to: %d\nOriginal tCL is: %d\n", (*channelNvList)[ch].common.nCL, nCLOrg);

    } // NormalBoot

    if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
      //
      // In DDR5 tWCL is always tCL - 2
      //
      (*channelNvList)[ch].common.nWL = (*channelNvList)[ch].common.nCL - 2;
    } else {
      (*channelNvList)[ch].common.nWL = tWL_DDR4[Host->nvram.mem.ratioIndex];
      //
      // Enforce tWL minimum for SKX NVMDIMM_IF logic
      //
      if (((*channelNvList)[ch].ddrtEnabled) && ((*channelNvList)[ch].common.nWL < 11)) {
        (*channelNvList)[ch].common.nWL = 11;
      }

    }


    if (PcdGetBool (PcdMrcXmpSupport)) {

      //
      // Check for manual overrides
      //
      if (Setup->mem.options & MEM_OVERRIDE_EN) {
        if (Setup->mem.inputMemTime.nRRD != 0) {
          (*channelNvList)[ch].common.nRRD = Setup->mem.inputMemTime.nRRD;
        }

        if (Setup->mem.inputMemTime.nRRD_L != 0) {
          (*channelNvList)[ch].common.nRRD_L = Setup->mem.inputMemTime.nRRD_L;
        }

        if (Setup->mem.inputMemTime.nWTR != 0) {
          (*channelNvList)[ch].common.nWTR = Setup->mem.inputMemTime.nWTR;
        }

        if (Setup->mem.inputMemTime.nRP != 0) {
          (*channelNvList)[ch].common.nRP = Setup->mem.inputMemTime.nRP;
        }

        if (Setup->mem.inputMemTime.nRAS != 0) {
          (*channelNvList)[ch].common.nRAS = Setup->mem.inputMemTime.nRAS;
        }

        if (Setup->mem.inputMemTime.nRCD != 0) {
          (*channelNvList)[ch].common.nRCD = Setup->mem.inputMemTime.nRCD;
        }

        if (Setup->mem.inputMemTime.nRC != 0) {
          (*channelNvList)[ch].common.nRC = Setup->mem.inputMemTime.nRC;
        }

        if (Setup->mem.inputMemTime.nRTP != 0) {
          (*channelNvList)[ch].common.nRTP = Setup->mem.inputMemTime.nRTP;
        }

        if (Setup->mem.inputMemTime.nRFC != 0) {
          (*channelNvList)[ch].common.nRFC = Setup->mem.inputMemTime.nRFC;
        }

        if (Setup->mem.inputMemTime.nWR != 0) {
          (*channelNvList)[ch].common.nWR = Setup->mem.inputMemTime.nWR;
        }

        if (Setup->mem.inputMemTime.nCL != 0) {
          (*channelNvList)[ch].common.nCL = Setup->mem.inputMemTime.nCL;
        }

        if (Setup->mem.inputMemTime.nFAW != 0) {
          (*channelNvList)[ch].common.nFAW = Setup->mem.inputMemTime.nFAW;
        }

        if (Setup->mem.inputMemTime.nCWL != 0) {
          (*channelNvList)[ch].common.nWL = Setup->mem.inputMemTime.nCWL;
        }
      } // MEM_OVERRIDE_EN
    }

    //
    // Program memory timings for this ch
    //
    ProgramTimings10nm (Host, socket, ch);

    dimmNvList  = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < Host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent && ((*dimmNvList)[dimm].fmcType == FMC_EKV_TYPE || (*dimmNvList)[dimm].fmcType == FMC_BWV_TYPE)) {
        if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_1600) {
          tempData = 0;
        } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_1866) {
          tempData = 1;
        } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2133) {
          tempData = 2;
        } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2400) {
          tempData = 3;
        } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2666) {
          tempData = 4;
        } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_2933) {
          tempData = 5;
        } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_3200) {
          tempData = 6;
        } else {
          tempData = 7;
        }

        Host->nvram.mem.socket[socket].ddrtFreq = (UINT8) tempData;
      } else if ((*dimmNvList) [dimm].DcpmmPresent == 1 && (*dimmNvList)[dimm].fmcType == FMC_CWV_TYPE) {
        if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_3200) {
          tempData = 0;
        } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_3600) {
          tempData = 1;
        } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_4000) {
          tempData = 2;
        } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_4400) {
          tempData = 3;
        } else if (Host->nvram.mem.socket[socket].ddrFreq <= DDR_4800) {
          tempData = 4;
        } else {
          tempData = 5;
        }

        Host->nvram.mem.socket[socket].ddrtFreq = (UINT8) tempData;
      }
    }

#ifdef DEBUG_CODE_BLOCK
    if (GetDebugLevel () & SDBG_MAX) {
      AcquirePrintControl ();

      RcDebugPrint (SDBG_MAX,
                     "Mem Timings:\n");

      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "WR_CRC=%d, TCLK readPreamble=%d, TCLK writePreamble=%d\n",
                     CalculateWrCrcAdder (ch),
                     GetPreambleState (socket, READ_PREAMBLE) + 1,
                     GetPreambleState (socket, WRITE_PREAMBLE) + 1);

      //RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      //               "CL adder from 2tclk readPreamble=%d, CWL adder from 2tclk writePreamble=%d\n",
      //               Host->var.mem.read2tckCL[ch], Host->var.mem.write2tckCWL[ch]);
      //
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tCCD     = %d\n", (*channelNvList)[ch].common.tCCD);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tCCD_L   = %d\n", (*channelNvList)[ch].common.tCCD_L);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tCCD_WR  = %d\n", (*channelNvList)[ch].common.tCCD_WR);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tCCD_WR_L= %d\n", (*channelNvList)[ch].common.tCCD_WR_L);

#ifdef DDRT_SUPPORT
      if ((*channelNvList)[ch].ddrtEnabled) {
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 1) {
            if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "DDR4 tCL      = %d\n", (*channelNvList)[ch].common.nCL);
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "DDR4 tCWL     = %d\n", (*channelNvList)[ch].common.nWL);
            } else {
              GetDdrtCasLatencies (socket, ch, dimm, &CASTemp, &ClAdd, &CWLTemp, &CwlAdd);
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "Pmem gnt2Data   = %d\n", CASTemp);
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "Pmem tCL Adder  = %d\n", ClAdd);
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "Pmem tCWL       = %d\n", CWLTemp);
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                             "Pmem tCWL Adder = %d\n", CwlAdd);
            }
          }
        }
      }
#endif // DDRT_SUPPORT
      if ((*channelNvList)[ch].ddrtEnabled == 0) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "tCL      = %d\n", (*channelNvList)[ch].common.nCL);
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "tCWL     = %d\n", (*channelNvList)[ch].common.nWL);
      }

      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tRP      = %d\n", (*channelNvList)[ch].common.nRP);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tRCD     = %d\n", (*channelNvList)[ch].common.nRCD);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tRRD     = %d\n", (*channelNvList)[ch].common.nRRD);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tRRD_L   = %d\n", (*channelNvList)[ch].common.nRRD_L);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tWTR_S   = %d\n", (*channelNvList)[ch].common.nWTR);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tWTR_L   = %d\n", (*channelNvList)[ch].common.nWTR_L);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tRAS     = %d\n", (*channelNvList)[ch].common.nRAS);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tRTP     = %d\n", (*channelNvList)[ch].common.nRTP);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tWR      = %d\n", (*channelNvList)[ch].common.nWR);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tFAW     = %d\n", (*channelNvList)[ch].common.nFAW);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tRC      = %d\n", (*channelNvList)[ch].common.nRC);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "tRFC     = %d\n", (*channelNvList)[ch].common.nRFC);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "casSup   = 0x%x\n", (*channelNvList)[ch].common.casSup);

      ReleasePrintControl ();
    }
#endif //DEBUG_CODE_BLOCK


    //
    // Fetch some ODT values and store them in the Host structure
    //
    BufferDramAndLrdimmBacksideOdtToHost (socket, ch);

    //
    // Set ODT timing parameters
    //
    SetOdtTiming (Host, socket, ch);

    MemTimingsAdjReg.Data = MemReadPciCfgEp (socket, ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);

    //
    // for LRDIMM in encoded mode, set subrank timing mode settings
    //
    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((IsLrdimmPresent (socket, ch, dimm) || (*channelNvList)[ch].encodedCSMode == 2)) {
        if ((*channelNvList)[ch].encodedCSMode == 2) {
          //Enable 3DS support
          MemTimingsAdjReg.Bits.en_3ds = 1;
        }
#if defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
        //
        // initialize F0BC1x variable
        //
        (*dimmNvList)[dimm].lrBuf_BC1x = 0;
#endif // defined(LRDIMM_SUPPORT) && !defined(DDR5_SUPPORT)
      } // lrdimm
    } //dimm loop

    MemWritePciCfgEp (socket, ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, MemTimingsAdjReg.Data);

    //
    // Reset counters
    //
    ResetPmonUnitCtrlCounters (MemTechDdr, socket, ch);
  } //ch loop

  //
  // Ensure all channels are enabled so sticky channel disable registers don't get out of sync on a warm reset
  //
  Status = EnableAllChannelsMc (socket);
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_ERROR, "Error clearing disable channel bits in hardware\n", Status);
  }

  //
  // Disable channels that are supported by MC or DDRIO IP but not this platform
  //
  Status = DisableUnsupportedChannelsHw (socket);
  if (EFI_ERROR (Status)) {
    RcDebugPrint (SDBG_ERROR,
      "Error disabling channels supported by MC or DDRIO IP but not the platform. Return Status: %r\n",
      Status);
  }

  //
  // Hooks for NVMDIMM
  //
  EarlyDdrtConfig (Host, socket);
  EarlyFnvConfigAccess (Host, socket);
  CADBChickenBit (Host, socket);
  EnableCpgcCmiCredit (Host, socket);

  InitVrefControls (Host, socket);

  return SUCCESS;

} // EarlyConfig

/**

  Report VDD Error

  @param socket  - Socket Id

  @retval N/A

**/
VOID
ReportVddError10nm (
  UINT8    socket
  )
{
  RcDebugPrintWithDevice (SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Cant program VDD!\n");
  OutputError (ERR_SET_VDD, ERR_UNKNOWN_VR_MODE, socket, 0xFF, 0xFF, 0xFF);
}
/**
  Build a list of unique DRAM VR SVIDs

  @param[in]  Socket           - Socket number
  @param[out] VrSvidList       - Array to be populated with VR SVIDs
  @param[out] VrSvidListLength - Number of unique VR SVIDs found

  @retval   EFI_INVALID_PARAMETER if input is invalid
  @retval   EFI_NOT_FOUND if SVID list not found
  @retval   EFI_SUCCESS if SVID list and length are found

**/
EFI_STATUS
BuildVrSvidList (
  IN  UINT8   Socket,
  OUT UINT8   VrSvidList[MAX_SVID_SOCKET],
  OUT UINT8   *VrSvidListLength
)
{
  EFI_STATUS  Status;
  UINT8       McId;
  UINT8       MaxImc;
  UINT8       VrSvidCount;
  UINT8       SvidIndex;
  UINT8       VrAddress;

  Status = EFI_SUCCESS;

  if ((VrSvidList == NULL) || (VrSvidListLength == NULL)) {
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "BuildVrSvidList: Invalid input -  VrSvidList=0x%x, VrSvidListLength=0x%x\n", VrSvidList, VrSvidListLength);
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  MaxImc = GetMaxImc ();
  VrSvidCount = 0;
  *VrSvidListLength = 0;
  ZeroMem (VrSvidList, sizeof (*VrSvidList) * MAX_SVID_SOCKET);

  for (McId = 0; McId < MaxImc; McId++) {
    //
    // Find VR SVID for this McId and append to the list if not already in the list
    //
    if (EFI_ERROR (GetSvidMap (Socket, McId, &VrAddress))) {
      RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "BuildVrSvidList: No VR found for McId %d\n", McId);
      //
      // Assert in debug builds - SVID Map is missing an entry.
      //
      ASSERT (FALSE);
      Status = EFI_NOT_FOUND;
      break;
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "SocketId = %d, McId = %d, SvidValue = %d\n", Socket, McId, VrAddress);
    }
    //
    // Search the VrSvidList for the VrAddress for this McId
    //
    for (SvidIndex = 0; SvidIndex < VrSvidCount; SvidIndex++) {
      if (VrSvidList[SvidIndex] == VrAddress) {
        //
        // VrAddress is already in the list
        //
        break;
      }
    }
    //
    // If VrAddress not already in the list, add it
    //
    if (SvidIndex == VrSvidCount) {
      //
      // Bounds check the array index
      //
      if (VrSvidCount < MAX_SVID_SOCKET) {
        VrSvidList[VrSvidCount] = VrAddress;
      }
      VrSvidCount++;
    }

    if (VrSvidCount > MAX_SVID_SOCKET) {
      RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "BuildVrSvidList: VrSvidCount (%d) exceeds limit of %d\n", VrSvidCount, MAX_SVID_SOCKET);
      //
      // Assert in debug builds - SVID Map is built incorrectly. Exit the loop otherwise.
      //
      ASSERT (FALSE);
      Status = EFI_NOT_FOUND;
      break;
    }

  } // McId
  *VrSvidListLength = VrSvidCount;
  return Status;
}

/**

  Program Vddq

  @param Host  - Pointer to sysHost
  @param socket  - Socket Id
  @param SvidEncodingValue - VRM12 defined VR Ramp Encoding Value

  @retval N/A

**/
VOID
SetVdd (
  PSYSHOST Host,
  UINT8    socket
  )
{
  UINT8                               ch;
  UINT8                               status;
  UINT32                              ActiveVrMask = 0;
  struct channelNvram                 (*channelNvList)[MAX_CH];
  BIOS_SCRATCHPAD1_STRUCT             biosScratchPad1;
  MC_INIT_STATE_G_MC_MAIN_STRUCT      mcInitStateG;
  UINT16                              userVdd = 0;
  UINT32                              VrMode = 0;
  EFI_STATUS                          EfiStatus = EFI_SUCCESS;
  UINT8                               VrIndex;
  UINT32                              VrAddress;
  SYS_SETUP                           *Setup;
  UINT8                               MaxChDdr;
  UINT8                               VrSvidList[MAX_SVID_SOCKET];
  UINT8                               VrSvidListLength;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList(Host, socket);

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CLOCK_INIT, CHECKPOINT_MINOR_SET_CLK_VDD, socket));
  //
  // Only assert reset for cold boot cases
  //
  if (((GetSysBootMode () == NormalBoot) &&
      ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast)))) {

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      //
      // Assert IO reset
      //
      mcInitStateG.Data = MemReadPciCfgEp (socket, ch, MC_INIT_STATE_G_MC_MAIN_REG);
      // Clear reset_io to assert reset
      mcInitStateG.Bits.reset_io    = 0;
      MemWritePciCfgEp (socket, ch, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
    } // ch loop

    //
    // Assert DRAM reset in cold boot path
    //
    SetDramResetN(socket, CH_BITMASK, DRAM_RESET_ASSERT);
  }

  // Loop for each channel
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    SetCkeMask (Host, socket, ch, 0xFF, 0);
  } // ch loop

  //
  // Read the Active DRAM VR's from the Mailbox Data Register
  //

  if (EFI_ERROR (PcuReadActiveVr (socket, &ActiveVrMask))) {
    ReportVddError10nm (socket);
  }

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   ":: Read the Active DRAM VR's from the Mailbox Data Register: %x\n", ActiveVrMask);

  if (HybridSystemLevelEmulationEnabled ()) {
    ActiveVrMask = 0;
  }

  if (PcdGetBool (PcdMrcXmpSupport)) {
    userVdd = Setup->mem.inputMemTime.vdd;

#ifdef DDRT_SUPPORT
    //
    // AEP VDD must be within +/-5% of 1.2V
    //
    if ((userVdd != 0) && (Host->nvram.mem.socket[socket].DcpmmPresent == 1)) {
      if (userVdd < MIN_AEP_VDD) {
        userVdd = MIN_AEP_VDD;
        OutputWarning (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_AEP_VDD_CHANGED, socket, NO_CH, NO_DIMM, NO_RANK);
        RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "PMem DIMM is present on Socket, therefore VDD min = %d mV\n", MIN_AEP_VDD);
      } else if (userVdd > MAX_AEP_VDD) {
        userVdd = MAX_AEP_VDD;
        OutputWarning(WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_AEP_VDD_CHANGED, socket, NO_CH, NO_DIMM, NO_RANK);
        RcDebugPrintWithDevice(SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "PMem DIMM is present on Socket, therefore VDD max = %d mV\n", MAX_AEP_VDD);
      }
    }
#endif // DDRT_SUPPORT

    //
    // Make sure the requested Vdd makes sense
    //
    if (userVdd != 0) {
      if((userVdd < MIN_DDR4_VDD) || (userVdd > MAX_DDR4_VDD)) {
        userVdd = 0;
        RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "User requesed a Vdd setting that is out of range.\n");
      }
    }
  }

  EfiStatus = BuildVrSvidList (socket, VrSvidList, &VrSvidListLength);
  if (EFI_ERROR (EfiStatus)) {
    //
    // If BuildVrSvidList failed, ensure the VrIndex loop does not execute
    //
    VrSvidListLength = 0;
  }

  if (VrSvidListLength > ARRAY_SIZE (VrSvidList)) {
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "SetVdd: VR SVID list length (%d) exceeds max allowed (%d)\n", VrSvidListLength, ARRAY_SIZE (VrSvidList));
    ASSERT (FALSE);
    VrSvidListLength = ARRAY_SIZE (VrSvidList);
  }

  for (VrIndex = 0; VrIndex < VrSvidListLength; VrIndex++) {
    //
    // If VR is active, program requested voltage
    //
    if (ActiveVrMask & (BIT0 << VrIndex)) {

      VrAddress = VrSvidList[VrIndex];
      //
      // Use MAILBOX_BIOS_CMD_VR_INTERFACE commandto read protocol ID register 0x05 for VR Mode
      //
      EfiStatus = PcodeReadVrMode (socket, VrAddress, &VrMode);

      if (EfiStatus != EFI_INVALID_PARAMETER && EfiStatus != EFI_SUCCESS) {
        ReportVddError10nm (socket);
        break;   // Break loop on error
      }

      //
      // Setup Encoding value based on VrMode and Selected Voltage
      //
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "VR%d DDR Voltage: ", VrIndex);

      //
      // Set appropriate Vdd
      //
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n:: VrAddress[%d] = %x, write MAILBOX_BIOS_CMD_VDD_RAMP,  userVdd = %dmv\n", VrIndex, VrAddress, userVdd);
      EfiStatus = PcuSetVdd (socket, VrAddress, VrMode, userVdd);
      if (EfiStatus == EFI_UNSUPPORTED) {
        ReportVddError10nm (socket);
        break;    // Break loop on error
      } else if (EFI_ERROR (EfiStatus)) {
        ActiveVrMask &= ~(BIT0 << VrIndex);
        RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "VR%d SVID Command Failed\n", VrIndex);
      }
    }
  } // VrIndex loop

  if ((ActiveVrMask & (BIT3 | BIT2 | BIT1 | BIT0)) == 0) {
    //
    // No SVID detected, call a hook to see if a platform specific Set VDD function is available
    //
    status = CoreSetVdd(Host, socket);
    if (status == FAILURE) {
      //
      // If CoreSetVdd() fails, change the Host structure to reflect it's running at the default voltage depending on the DDR type
      //
      if (IsDdr5MemSsPresent (Host, socket)) {
        Host->nvram.mem.socket[socket].ddrVoltage = SPD_VDD_110;
        RcDebugPrintWithDevice (SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "\n:: Setting VDD to 1.1V.\n");
      } else {
        Host->nvram.mem.socket[socket].ddrVoltage = SPD_VDD_120;
        RcDebugPrintWithDevice (SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                       "\n:: Setting VDD to 1.2V.\n");
      }

    }
  }

  //
  // set base voltage into bios scratchpad1 register bits 1-0 (zero out rest of bits for now)
  //  0 = Not set yet (read before MRC initializes the value)
  //  1 = 1.5V
  //  2 = 1.35V
  //  3 = Reserved for 1.2x setting
  //
  biosScratchPad1.Data = GetStickyScratchpad (socket, BIOS_STICKY_SCRATCHPAD_1);

  biosScratchPad1.Bits.MemVolt = 0;

  if (Host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_120) {
    biosScratchPad1.Bits.MemVolt = SPD_VDD_120;
  } else if (Host->nvram.mem.socket[socket].ddrVoltage == SPD_VDD_110) {
    biosScratchPad1.Bits.MemVolt = SPD_VDD_110;
  }

  SetStickyScratchpad (socket, BIOS_STICKY_SCRATCHPAD_1, biosScratchPad1.Data);

  PmicEnableChip (Host, socket);
  //
  // Initialization complete
  //
} // SetVdd

/**
Program Dimm specific WA

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket Id
  @param[in] Ch      - Ch number

  @retval N/A

**/
VOID
DimmSpecificPreTraining (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
)
{
  UINT8            Dimm;
  UINT8            Rank;
  struct dimmNvram (*DimmNvList)[MAX_DIMM];
  struct ddrRank   (*RankList)[MAX_RANK_DIMM];
  UINT16           DateCode = 0;


  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    DimmNvList = GetDimmNvList(Host, Socket, Ch);
    if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
      continue;
    }

    if (IsDcpmmPresentDimm (Socket, Ch, Dimm) == TRUE) {
      continue;
    }

    RankList = GetRankNvList (Host, Socket, Ch, Dimm);
    for (Rank = 0; Rank < Host->var.mem.socket[Socket].maxRankDimm; Rank++) {
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT) || (Rank >= (*DimmNvList)[Dimm].numRanks)) {
        continue;
      }
      DateCode = (*DimmNvList)[Dimm].SPDModDate >> 8;
      DateCode |= (*DimmNvList)[Dimm].SPDModDate << 8;

      //Samsung 2RX4 less than datecode 0x1950
      if (((*DimmNvList)[Dimm].SPDRegVen == MFGID_SAMSUNG) && ((*DimmNvList)[Dimm].x4Present) && ((*DimmNvList)[Dimm].numRanks == 2) && (DateCode <= 0x1950)) {

        RcDebugPrintWithDevice (
          SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Samsung WA for 2RX4\n");

        CpgcAdvCmdParity (Host, Socket, Ch, GetPhyRank (Dimm, Rank), 0, PRECHARGE, NormalCadbSetup);
      }//Samsung WA
    } //Rank
  }//Dimm
}

#if FixedPcdGetBool(PcdMcrSupport)
/**

  Initialize MCR Pseuodo-Channel Phase

  Host      - Pointer to sysHost
  Socket    - Socket Id
  ChBitMask - Bitmask of channels to reset

  @retval N/A

**/
VOID
McrPhaseInit (
  PSYSHOST  Host,
  UINT8     Socket,
  UINT32    ChBitMask
  )
{
  UINT8   Ch;
  UINT8   Dimm;
  UINT8   MaxChDdr;

  MaxChDdr = GetMaxChDdr ();

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if (IsChannelEnabled (Socket, Ch) == FALSE) {
      continue;
    }
    if ((ChBitMask & (1 << Ch)) == 0) {
      continue;
    }
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
        continue;
      }
      McrSetPhaseCmd (Socket, Ch, Dimm, GetMaxValidSubChannelMask (), NOP_CYCLES_2CLK);
      CteDelayDclk (10);
      McrSetPhaseCmd (Socket, Ch, Dimm, GetMaxValidSubChannelMask (), NOP_CYCLES_64CLK);
    } // dimm loop
  } // Ch loop
} // McrPhaseInit
#endif // FixedPcdGetBool(PcdMcrSupport)

/**

  Reset the DDR channels for the given socket number (or per MC for BSSA)

  Host      - Pointer to sysHost
  socket    - Socket Id
  chBitMask - Bitmask of channels to reset

  @retval N/A

**/
VOID
ResetAllDdrChannels (
  PSYSHOST Host,
  UINT8     socket,
  UINT32    chBitMask
  )
{
  UINT8   ch;
  UINT8   nv_ckeMask;
  struct channelNvram (*channelNvList) [MAX_CH];
  MC_INIT_STATE_G_MC_MAIN_STRUCT     mcInitStateG;
  UINT8                              MaxChDdr;

  //
  // Bypass for the S3 resume path
  //
  if ((GetSysBootMode () == S3Resume || Host->var.mem.subBootMode == WarmBootFast)) {
    return;
  }

  RcDebugPrint (SDBG_MAX,
    "\nReset All Channels\n");

  CountTrackingData (PTC_JEDEC_COUNT, 1);

  channelNvList = GetChannelNvList (Host, socket);
  MaxChDdr = GetMaxChDdr ();

  // De-assert CKE
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }
    if ((chBitMask & (1 << ch)) == 0) {
      continue;
    }

    OverrideCke (
      socket,
      ch,
      (*channelNvList) [ch].ckeMask,
      0
      );

  } // ch loop

  //
  // Delay only if this is a CTE build, otherwise do nothing.
  //

  CteDelayQclk (2 * 16);

  //clear the reset io bits for each Ch
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }
    if ((chBitMask & (1 << ch)) == 0) {
      continue;
    }
    //
    // clear IO reset
    //
    mcInitStateG.Data = MemReadPciCfgEp (socket, ch, MC_INIT_STATE_G_MC_MAIN_REG);
    mcInitStateG.Bits.reset_io = 0;
    MemWritePciCfgEp (socket, ch, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
  } // ch loop

  SetDramResetN (socket, chBitMask, DRAM_RESET_ASSERT);
  //
  // Reset should be held for JEDEC mandated 200us, but a 2 usec
  // fixed delay in MRC should be plenty for stable power DDR reset flow.
  //
  if (Host->var.mem.socket[socket].firstJEDECDone) {
    FixedDelayMicroSecond (2);
  } else {
    FixedDelayMicroSecond (200);
    Host->var.mem.socket[socket].firstJEDECDone = 1;
  }

  SetDramResetN (socket, chBitMask, DRAM_RESET_DEASSERT);

  if (Host->nvram.mem.dimmTypePresent != RDIMM) {
    // Loop for each channel
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList) [ch].enabled == 0) {
        continue;
      }
      if ((chBitMask & (1 << ch)) == 0) {
        continue;
      }
      nv_ckeMask = (*channelNvList) [ch].ckeMask;

      //
      // Reset deassertion to CKE assertion (JEDEC mandated 500us)
      //
      FixedDelayMicroSecond (500);

      // Assert CKE
      OverrideCke (socket, ch, (*channelNvList) [ch].ckeMask, nv_ckeMask);

    } // ch loop

    //
    // wait the 20 nano sec tCKSRX
    //
    FixedDelayMicroSecond (1);
  } else {
    if (IsDcpmmPresentSocket (socket) == TRUE) {
      // wait 10 usec for tStab
      FixedDelayMicroSecond (10);
    } else {
      // wait 5 usec for tStab
      FixedDelayMicroSecond (5);
    }
  }

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    //
    // De-assert CKE
    //
    for (ch = 0; ch < MaxChDdr; ch++) {
      if (IsChannelEnabled (socket, ch) == FALSE) {
        continue;
      }
      if ((chBitMask & (1 << ch)) == 0) {
        continue;
      }

      OverrideCke (
        socket,
        ch,
        (*channelNvList) [ch].ckeMask,
        (*channelNvList) [ch].ckeMask  // Handle both PS
        );
    } //ch
  }

#if FixedPcdGetBool(PcdMcrSupport)
  //
  // Initialize Pseuodo-CH phase for MCR
  //
  McrPhaseInit (Host, socket, chBitMask);
#endif // FixedPcdGetBool(PcdMcrSupport)
} // ResetAllDdrChannels


/**

  Reset the DDR channels for the given socket number (or per MC for BSSA)

  Host      - Pointer to sysHost
  socket    - Socket Id
  chBitMask - Bitmask of channels to reset

  @retval N/A

**/
VOID
ResetAllDdrChannelsPreTraining (
  PSYSHOST Host,
  UINT8     socket,
  UINT32    chBitMask
  )
{
  UINT8   ch;
  UINT8   nv_ckeMask;
  struct channelNvram (*channelNvList) [MAX_CH];
  MC_INIT_STATE_G_MC_MAIN_STRUCT     mcInitStateG;
  UINT8                              MaxChDdr;

  //
  // Bypass for the S3 resume path
  //
  if ((GetSysBootMode () == S3Resume || Host->var.mem.subBootMode == WarmBootFast)) {
    return;
  }

  RcDebugPrint (SDBG_MAX,
                  "\nReset All Channels\n");

  CountTrackingData (PTC_JEDEC_COUNT, 1);

  channelNvList = GetChannelNvList (Host, socket);
  MaxChDdr      = GetMaxChDdr ();

  //
  // DDRT 2.0: Before DRST is asserted, make sure ERR# and REQ# are gated
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }
    if ((chBitMask & (1 << ch)) == 0) {
      continue;
    }

    if (GetFmcType (socket, ch) == FMC_CWV_TYPE) {
      ProgramReqRxPathGate (Host, socket, ch);
    }

    //Dimm specific WA
    DimmSpecificPreTraining (Host, socket, ch);
  } // ch loop

  RcDebugPrint (SDBG_MAX, "\nAssert DRAM Reset_N\n");
  SetDbReset (socket, chBitMask, DB_BRST_ASSERT);
  SetDramResetN (socket, chBitMask, DRAM_RESET_ASSERT);
  FixedDelayMicroSecond (2);
  //
  // Drive CS_n low
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }
    if ((chBitMask & (1 << ch)) == 0) {
      continue;
    }
    nv_ckeMask = (*channelNvList) [ch].ckeMask;
    OverrideCke (
      socket,
      ch,
      (*channelNvList) [ch].ckeMask,
      0
      );
    RcDebugPrint (SDBG_MAX, "\n Deassert CKE to drive CS low\n");
  } // ch loop

  //
  // Delay only if this is a CTE build, otherwise do nothing.
  //

  CteDelayQclk (2 * 16);

  //clear the reset io bits for each Ch
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }
    if ((chBitMask & (1 << ch)) == 0) {
      continue;
    }
    //
    // clear IO reset
    //
    mcInitStateG.Data = MemReadPciCfgEp (socket, ch, MC_INIT_STATE_G_MC_MAIN_REG);
    mcInitStateG.Bits.reset_io = 0;
    MemWritePciCfgEp (socket, ch, MC_INIT_STATE_G_MC_MAIN_REG, mcInitStateG.Data);
  } // ch loop

  //
  // DRAM tINIT1 or RCD tRINIT1
  // Reset should be held low for JEDEC mandated 200us after voltage ramp or 1us for stable power
  //
  if (Host->var.mem.socket[socket].firstJEDECDone) {
    FixedDelayMicroSecond (1);
  } else {
    FixedDelayMicroSecond (200);
    Host->var.mem.socket[socket].firstJEDECDone = 1;
  }
  SetDramResetN (socket, chBitMask, DRAM_RESET_DEASSERT);
  SetDbReset (socket, chBitMask, DB_BRST_DEASSERT);

  if ((Host->nvram.mem.dimmTypePresent == UDIMM) || (Host->nvram.mem.dimmTypePresent == SODIMM)) {
    //
    // tINIT3 = 4ms
    // Minimum CS_n LOW time after RESET_n HIGH
    //
    FixedDelayMicroSecond (4000);
  } else {
    //
    // tRINIT3 = 20ns
    // Minimum DCS LOW time after DRST_n HIGH
    //
    FixedDelayMicroSecond (1);
  }
  //
  // Drive CS_n high
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList) [ch].enabled == 0) {
      continue;
    }
    if ((chBitMask & (1 << ch)) == 0) {
      continue;
    }
    RcDebugPrint (SDBG_MAX,
                    "\n Assert CKE to drive CS high\n");
    nv_ckeMask = (*channelNvList) [ch].ckeMask;

    OverrideCke (socket, ch, (*channelNvList) [ch].ckeMask, nv_ckeMask);

  } // ch loop

  if (Host->nvram.mem.dimmTypePresent != RDIMM) {
    //
    // wait the 20 nano sec tCKSRX
    //
    FixedDelayMicroSecond (1);
  } else {
    if (IsDcpmmPresentSocket (socket) == TRUE) {
      // wait 10 usec for tStab
      FixedDelayMicroSecond (10);
    } else {
      // wait 5 usec for tStab
      FixedDelayMicroSecond (5);
    }
  }

  //
  // tSTAB01 and tSTAB02
  //
  TrainingDelay (Host, 1000, 10);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if ((chBitMask & (1 << ch)) == 0) {
      continue;
    }

    //Dimm specific WA
    DimmSpecificPreTraining (Host, socket, ch);
  } //ch

} // ResetAllDdrChannelsPreTraining


/*
  Handles the S3 exit flow. After the DIMM configuration is
  restored, BIOS can perform the following steps to bring the
  DRAM out of self-refresh and enabling the refresh operations:
  1. Enable DCLK at DDR interface and wait for tSTAB (~6usec)
     for clock to be stable.
  2. Set up  commands:
     A. NOP with tXS wait
     B. Refresh with tRFC wait
     C. ZQCL with tZQoper wait
  3. Set rank disable at DIMMMTR_x.RANK_DISABLE for any rank
     that is populated but need to be disabled from all
     channels.
  4. Set CKE via MC_INIT_STATE_Cx.CKE_ON[5:0] for all
     occupied channels (up to four CSR writes).
  5. Wait tRFC+10ns (but subtract the amount of time took for
     the last CSR write from step #4) via subsequence control
  7. Poll for done
  8. Enable automatic refresh via setting REFRESH_ENABLE in
     MC_INIT_STATE_G

  Due to the timing constraint of the above step 4-8 which may affect the DRAM refresh
  operation without violating 9xTREFI panic refresh requirement, these steps (4-8) must
  be pre-loaded into the LLC during the Non-Evicted Memory Mode (NEM); otherwise, the
  fetch delay from the Flash device may take a INT32 time and violate the refresh timing.
*/
VOID
ExitSelfRefresh10nm (
  PSYSHOST Host,
  UINT8 socket
  )
{
  UINT8                             ch;
  struct channelNvram               (*channelNvList)[MAX_CH];
  UINT8                             dimm;
  UINT8                             rank;
  UINT32                            lrbufData;
  UINT32                            PrevDebugLevel;
  struct   dimmNvram                (*dimmNvList)[MAX_DIMM];
  UINT8                             MaxChDdr;
#ifndef DDR5_SUPPORT
  UINT16                            MR6;
  struct   rankDevice               (*rankStruct)[MAX_RANK_DIMM];
  struct   ddrRank                  (*rankList)[MAX_RANK_DIMM];
#endif // !DDR5_SUPPORT

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Exit Self Refresh\n");

  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr      = GetMaxChDdr ();

  //
  // Setup engine with commands needed for SR exit.
  // 1. NOP with tXS wait
  // 2. ZQCL with tZQoper wait
  // 3. Refresh with tRFC wait
  //

  //
  // Bring the IO out of reset on S3 path before enabling refresh
  // and switching to normal mode.
  //
  if (Host->var.mem.subBootMode != NvDimmResume) {
    //
    // Assert that refresh engine is not enabled
    // TODO: move assert inside IO_Reset function and verify hostRefreshStatus is set on normal path
    //
    RC_FATAL_ERROR ((Host->var.mem.socket[socket].hostRefreshStatus == 0), ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_6);
    IO_Reset(Host, socket);
  }

  //Initialize the masks before starting the refresh engine
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Deassert CKE
    //
    SetCkeMask (Host, socket, ch, 0xFF, 0);

    // Disable Refresh
    SetRefreshMask (Host, socket, ch, 0xFF);

    // Disable ZQCALs
    SetZQMask (Host, socket, ch, 0xFF);

    //
    // Apply 10 usec delay to satisfy tstab for DCPMM
    //
    if (IsDcpmmPresentSocket (socket) == TRUE) {
      FixedDelayMicroSecond (10);
    }
  } // ch loop

  if (Host->var.mem.subBootMode == NvDimmResume) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList(Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        if ((*dimmNvList)[dimm].NvDimmNPresent) {

          //
          // Note: RCD PLL settings must be written while CKE is low to prevent violations on CK input to DRAM.
          // This step must be done via SMBus interface because the RCD is in CKE PD state after NVDIMM restore flow
          // and will ignore RCW writes until CKE goes high.
          //

          // SMBus offset 0xD byte contains RC0B in bits[7:4] and RC0A in bits[3:0]
          lrbufData = ((*dimmNvList)[dimm].rcCache[RDIMM_RC0B] << 4) | ((*dimmNvList)[dimm].rcCache[RDIMM_RC0A] & 0x0F);
          WriteLrbufSmb(Host, socket, ch, dimm, 1, 0x0D, &lrbufData);

          // SMBus offset 0x12 byte contains RC3x in bits[7:0]
          lrbufData = (*dimmNvList)[dimm].rcxCache[RDIMM_RC3x >> 4];
          WriteLrbufSmb (Host, socket, ch, dimm, 1, 0x12, &lrbufData);

        } // NvDimmNPresent
      } // dimm loop
    } // ch loop
  } // if NvDimmResume

  PrevDebugLevel = GetDebugLevel ();
  if ((PrevDebugLevel & SDBG_FORCE_ALL) == 0) {
    SetDebugLevel (SDBG_ERROR);
  }

  //
  // Assert CKE on each channel and unmask periodic refresh
  // Note that this function must be prefilled into cache before calling.
  //
  ExitSelfRefreshCriticalSection(Host, socket);

  //
  // Restore output messages after auto refresh is enabled
  //
  SetDebugLevel (PrevDebugLevel);

  if (Host->var.mem.subBootMode == NvDimmResume) {  // Initialize MRx rgisters for NVDIMMs

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      dimmNvList = GetDimmNvList(Host, socket, ch);
#ifndef DDR5_SUPPORT
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        if ((*dimmNvList)[dimm].NvDimmNPresent) {

          DoRegisterInitDDR4(Host, socket, ch, dimm, 0);
          DoRegisterInitDDR4_CKE(Host, socket, ch, dimm, 0);

          rankList = GetRankNvList(Host, socket, ch, dimm);
          rankStruct = GetRankStruct(Host, socket, ch, dimm);
          for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
            if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            MR6 = GetVrefRange(Host, (*channelNvList)[ch].txVrefSafe[(*rankList)[rank].rankIndex]);
#ifdef LRDIMM_SUPPORT
            if (IsLrdimmPresent (socket, ch, dimm)) {
              MR6 = (UINT8)(*rankList)[rank].lrbufTxVref[0];
            }
#endif
            MR6 |= ((GetTccd_L (socket, ch) - 4) & 7) << 10;

            //
            // Note that NVDIMM microcontroller will leave DRAM in DLL-off state, so the
            // DLL-off to DLL-on flow must be used to write MR1, MR0 prior to other MRS
            //
            WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR1, BANK1);
            WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR0, BANK0);
            //
            // Follow MRS programming order in the DDR4 SDRAM spec
            //
            WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR3, BANK3);
            WriteMR6 (Host, socket, ch, dimm, rank, MR6, BANK6);
            WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR5, BANK5);
            WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR4, BANK4);
            WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR2, BANK2);
          } // rank loop
        } // if NvDimmNPresent
      } // dimm loop
#endif // !DDR5_SUPPORT

      DoZQ(Host, socket, ch, ZQ_LONG);

      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }

        if ((*dimmNvList)[dimm].NvDimmNPresent) {

          for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
            if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            if (Host->nvram.mem.socket[socket].TrainingStepDone.DramRxEq == 1) {
              RestoreOffset (Host, socket, ch, dimm, rank, 0, DdrLevel, DramRxEq);
            }

            //
            // Only restore Tx Vref on rank 0 for LRDIMMs
            //
            if ((rank > 0) & (IsLrdimmPresent (socket, ch, dimm))) {
              continue;
            }
            RestoreOffset(Host, socket, ch, dimm, rank, 0, DdrLevel, TxVref);
          } // rank loop
        } // if NvDimmNPresent
      } // dimm loop
    } // ch loop
  } // if NvDimmResume
} // ExitSelfRefresh10nm

/**
   Determine the min and max values of roundtrip per channel for ddr4 dimms in qclks

   @param[in] Host            - Pointer to sysHost
   @param[in] Socket          - Socket Id
   @param[in] Ch              - Channel number
   @param[out] MinRoundTrip   - Pointer to caller's min roundtrip parameter
   @param[out] MaxRoundTrip   - Pointer to caller's max roundtrip parameter

   @retval EFI_SUCCESS if at least 1 DDR4 DIMM installed in channel
           EFI_NOT_FOUND otherwise - MinRoundTrip and MaxRoundTrip not updated
**/
EFI_STATUS
GetMinMaxRoundTrip (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  OUT   UINT8     *MinRoundTrip,
  OUT   UINT8     *MaxRoundTrip
  )
{
  UINT8                   Dimm;
  UINT8                   Rank;
  UINT8                   MinRoundTripLocal = MAX_UINT8;
  UINT8                   MaxRoundTripLocal = 0;
  UINT8                   TempRoundtrip;
  BOOLEAN                 DimmPresentCh = FALSE;
  struct dimmNvram        (*DimmNvList)[MAX_DIMM];

  DimmNvList  = GetDimmNvList (Host, Socket, Ch);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if ((*DimmNvList)[Dimm].dimmPresent == 0) {
      continue;
    }
    if (((*DimmNvList)[Dimm].DcpmmPresent == 1)) {
      // only ddr4 dimms
      continue;
    }

    // if you come here there is atleast 1 ddr4 dimm present per channel
    DimmPresentCh = TRUE;

    for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {
      if (CheckRank(Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }

      GetRoundTrip (Socket, Ch, GetLogicalRank (Host, Socket, Ch, Dimm, Rank), GSM_READ_CSR, &TempRoundtrip);

      if (TempRoundtrip < MinRoundTripLocal) {
        MinRoundTripLocal = TempRoundtrip;
      }

      if (TempRoundtrip > MaxRoundTripLocal) {
        MaxRoundTripLocal = TempRoundtrip;
      }
    } // Rank
  } // Dimm

  // set the min and max values of roundtrip per channel for ddr4 dimms in qclks
  if (DimmPresentCh == TRUE) {  // atleast 1 ddr4 dimm present per channel
    if (MinRoundTrip != NULL) {
      *MinRoundTrip = MinRoundTripLocal;
    }
    if (MaxRoundTrip != NULL) {
      *MaxRoundTrip = MaxRoundTripLocal;
    }
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
} // GetMinMaxRoundTrip

#ifdef DDRT_SUPPORT
/**
  Determine whether resets based on the completion delay value are supported.

  @retval   Whether a reset is supported.
**/
BOOLEAN
CompletionDelayResetsAreSupported (
  VOID
  )
{
  return (!IsSimics () && IsSiliconWorkaroundEnabled ("S1409301288"));
}

/**
  Determine the availability for issuing a reset based on the completion delay value.

  @param[in]  Socket  The target socket to check for reset availability.

  @retval   Whether a reset is available.
**/
BOOLEAN
GetCompletionDelayResetAvailable (
  IN  UINT8 Socket
  )
{
  BOOLEAN ResetAvailable = FALSE;
  UINT32  ResetCount;

  if (CompletionDelayResetsAreSupported ()) {
    UBoxGetCompletionDelayResetCount (Socket, &ResetCount);
    if (ResetCount < COMPLETION_DELAY_RESET_COUNT_MAX) {
      ResetAvailable = TRUE;
    }
  }

  return ResetAvailable;
}

/**
  Request a reset due to the completion delay value, if such a reset is available. If a reset is successfully
  requested, the number of completion delay resets will be incremented.

  @param[in]  Socket  The target socket to increment the reset count on.

  @retval   Whether a reset has been requested.
**/
BOOLEAN
RequestCompletionDelayReset (
  IN  UINT8   Socket
  )
{
  UINT32      ResetCount;
  BOOLEAN     ResetRequested = FALSE;
  EFI_STATUS  Status;

  if (CompletionDelayResetsAreSupported ()) {
    UBoxGetCompletionDelayResetCount (Socket, &ResetCount);
    if (ResetCount < COMPLETION_DELAY_RESET_COUNT_MAX) {
      ResetCount++;
      Status = UBoxSetCompletionDelayResetCount (Socket, ResetCount);
      ASSERT_EFI_ERROR (Status);
      SetSysResetRequired (GetSysResetRequired () | POST_RESET_WARM);
      ResetRequested = TRUE;
      RcDebugPrint (SDBG_DEFAULT, "Completion delay out of bounds! Reset Requested: %x\n", GetSysResetRequired ());
    }
  }

  return ResetRequested;
}

/**
  Sync the completion delay reset counts so all sockets are updated with max number of resets that have been requested.
  This is only active on the SBSP.
**/
VOID
SyncCompletionDelayResetCounts (
  VOID
  )
{
  UINT32      MaxResetCount = 0;
  UINT32      ResetCount[MAX_SOCKET];
  UINT8       Socket;
  EFI_STATUS  Status;

  // Sync the reset count only with the SBSP.
  if (CompletionDelayResetsAreSupported () &&
      (GetSysSocketId () == GetSysSbspSocketIdNv ()))
  {
    // Get reset counts for all sockets and determine the max reset count value.
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      ResetCount[Socket] = 0;
      if (SocketPresent (Socket)) {
        UBoxGetCompletionDelayResetCount (Socket, &(ResetCount[Socket]));
        MaxResetCount = MAX (ResetCount[Socket], MaxResetCount);
      }
    }

    // Update all sockets with the max reset count value (as needed).
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (SocketPresent (Socket) &&
          (ResetCount[Socket] < MaxResetCount))
      {
        Status = UBoxSetCompletionDelayResetCount (Socket, MaxResetCount);
        ASSERT_EFI_ERROR (Status);
      }
    }
  }
}

/**
  Calculate the completion delay in Dclks.

  @param[in]  RoundTripQclk     Round trip delay in Qclks
  @param[in]  GrantToEridDclk   Grant-to-ERID delay in Dclks

  @retval   The completion delay in Dclks
**/
INT32
CalculateCompletionDelayDclk (
  IN  INT16 RoundTripQclk,
  IN  INT16 GrantToEridDclk
  )
{
  return ((RoundTripQclk / QCLK_PER_DCLK) - GrantToEridDclk - INTERNAL_PIPESTAGE_DELAY_DCLK);
}

/**
  Calculate the max supported round trip delay in Qclks.

  @param[in]  GrantToEridDclk   Grant-to-ERID delay in Dclks

  @retval   The max supported round trip delay in Qclks
**/
UINT32
CalculateMaxRoundTripQclk (
  IN  UINT32  GrantToEridDclk
  )
{
  return ((COMPLETION_DELAY_DCLKS_HI_MAX + GrantToEridDclk + INTERNAL_PIPESTAGE_DELAY_DCLK) * QCLK_PER_DCLK);
}
#endif // DDRT_SUPPORT

/**
  Program the command to data completion delay

  @param  Socket    - Socket number

  @return None
**/
VOID
ProgramCmdToDataCompletionDelay (
  IN    UINT8     Socket
  )
{
  PSYSHOST                                  Host;
  UINT8                                     Ch;
  UINT8                                     CmplProgramDelay;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  CMPL_TO_DATA_DELAY_MCDDC_DP_STRUCT        CmplDataDelay;
  BOOLEAN                                   DisablingChannel;
  UINT8                                     MaxChDdr;
#ifdef DDRT_SUPPORT
  UINT8                                     RoundtripRtRank;
  UINT8                                     Dimm;
  INT32                                     CompletionDelayDclks;
  INT16                                     Gnt2Erid;
  struct dimmNvram                          (*DimmNvList)[MAX_DIMM];
  BOOLEAN                                   DelayLimitingAvailable;
  BOOLEAN                                   DelayOutOfBounds;
  UINT8                                     Rank;
  BOOLEAN                                   ResetAvailable;
  BOOLEAN                                   ResetRequested = FALSE;
  BOOLEAN                                   ExtendMaxDelay;
  INT32                                     CompletionDelayMaxActual;
  LINK_CFG_READ_1_MCDDC_DP_STRUCT           LinkCfgRead;

  ResetAvailable = GetCompletionDelayResetAvailable (Socket);
  DelayLimitingAvailable = ResetAvailable || IsSimics ();

  if (IsSiliconWorkaroundEnabled ("S1409782849")) {
    CompletionDelayMaxActual = COMPLETION_DELAY_DCLKS_HI_MAX_EXT;
  } else {
    CompletionDelayMaxActual = COMPLETION_DELAY_DCLKS_HI_MAX;
  }
#endif // DDRT_SUPPORT

  Host = GetSysHostPointer ();
  ChannelNvList = GetChannelNvList (Host, Socket);
  MaxChDdr = GetMaxChDdr ();

  //
  // Program command to data completion delay based on configuration
  //
  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    DisablingChannel = FALSE;
    CmplProgramDelay = GetCmplDelayDdr (Socket);
#ifdef DDRT_SUPPORT
    ExtendMaxDelay = FALSE;
    DelayOutOfBounds = FALSE;
    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    if (Host->nvram.mem.DcpmmPresent == 1) {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        if ((*DimmNvList)[Dimm].dimmPresent == 0) {
          continue;
        }
        if ((*DimmNvList)[Dimm].DcpmmPresent == 0 || (GetFmcType(Socket, Ch) == FMC_CWV_TYPE)) {
          continue;
        }

        Rank = (Dimm * MAX_RANK_DIMM) + DDRT_DIMM_RANK;
        GetRoundTrip (Socket, Ch, Rank, GSM_READ_CSR, &RoundtripRtRank);
        GetSetSxpGnt2Erid (Host, Socket, Ch, Dimm, GSM_READ_ONLY, &Gnt2Erid);
        CompletionDelayDclks = CalculateCompletionDelayDclk (RoundtripRtRank, Gnt2Erid);

        if (CompletionDelayDclks < COMPLETION_DELAY_DCLKS_LO_MIN) {
          DEBUG ((EFI_D_ERROR, "ERROR: Completion Delay input value is less than %d: %d\n",
                  COMPLETION_DELAY_DCLKS_LO_MIN, CompletionDelayDclks));
          if (DelayLimitingAvailable) {
            CompletionDelayDclks = COMPLETION_DELAY_DCLKS_LO_MIN;
          } else {
            OutputWarning (WARN_COMPLETION_DELAY_ERROR, WARN_CMPL_DELAY_BELOW_MIN, Socket, Ch, Dimm, 0xFF);
          }
          DelayOutOfBounds = TRUE;
        } else if (CompletionDelayDclks > COMPLETION_DELAY_DCLKS_HI_MAX) {
          if (IsSiliconWorkaroundEnabled ("S1409782849")) {
            ExtendMaxDelay = TRUE;
          }

          if (CompletionDelayDclks > CompletionDelayMaxActual) {
            DEBUG ((EFI_D_ERROR, "ERROR: Completion Delay input value is greater than %d: %d\n",
                   CompletionDelayMaxActual, CompletionDelayDclks));
            if (DelayLimitingAvailable) {
              CompletionDelayDclks = CompletionDelayMaxActual;
            } else {
              OutputWarning (WARN_COMPLETION_DELAY_ERROR, WARN_CMPL_DELAY_MAX_EXCEEDED, Socket, Ch, Dimm, 0xFF);
            }
            DelayOutOfBounds = TRUE;
          }
        }

        if (DelayOutOfBounds) {
          if (ResetAvailable && !ResetRequested) {
            ResetRequested = RequestCompletionDelayReset (Socket);
          }
          if (!DelayLimitingAvailable && !ResetRequested) {
            // Only disable the channel when no resets are requested. Otherwise, we could end up with all channels
            // mapped out, which would cause a fatal error in mem map before the reset point could be reached.
            DisablingChannel = TRUE;
            break; // Exit DIMM loop
          }
        }

        if (CompletionDelayDclks < COMPLETION_DELAY_DCLKS_HI_MIN) {
          //
          // CompletionDelayDclks is now in a UINT8 range
          //
          CmplProgramDelay = (UINT8)CompletionDelayDclks + COMPLETION_DELAY_DCLKS_LO_ADDER;
        } else {
          //
          // CompletionDelayDclks is now in a UINT8 range
          //
          CmplProgramDelay = (UINT8)CompletionDelayDclks + COMPLETION_DELAY_DCLKS_HI_ADDER;
        }
      } // for (Dimm = 0; Dimm < MAX_DIMM; Dimm++)
    } // if (Host->nvram.mem.DcpmmPresent == 1)

    if (DisablingChannel) {
      // Note: "if (DisablingChannel)" and "if (!DisablingChannel)" are separated (rather than being
      // "if (DisablingChannel) ... else") because this "if" block is unreachable in non-DDRT_SUPPORT builds.
      DisableChannelSw (Host, Socket, Ch);
    }
#endif // DDRT_SUPPORT

    if (!DisablingChannel) {
#ifdef DDRT_SUPPORT
      if (ExtendMaxDelay) {
        //
        // Configure MC channels and DDRIO for increased SPID rddata_valid to rddata delay
        //
        ExtendCmdToDataCompletionDdrio (Host, Socket, Ch);
        LinkCfgRead.Data = MemReadPciCfgEp (Socket, Ch, LINK_CFG_READ_1_MCDDC_DP_REG);
        LinkCfgRead.Bits.read_data |= INCREASE_SPID_RDDAT_VALID_TO_RDDATA_SPACING;
        MemWritePciCfgEp (Socket, Ch, LINK_CFG_READ_1_MCDDC_DP_REG, LinkCfgRead.Data);
      }
#endif // DDRT_SUPPORT
      CmplDataDelay.Data = MemReadPciCfgEp (Socket, Ch, CMPL_TO_DATA_DELAY_MCDDC_DP_REG);

      CmplDataDelay.Bits.cmpl_program_delay = CmplProgramDelay;

      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
           "Configure cmpl_program_delay: %d\n", CmplProgramDelay);

      MemWritePciCfgEp (Socket, Ch, CMPL_TO_DATA_DELAY_MCDDC_DP_REG, CmplDataDelay.Data);
    }
  } // Ch loop
}

/**
  Configure hardware for the DIMM population on a memory controller

  @param[in] Host     - Pointer to SysHost
  @param[in] Socket   - Socket number
  @param[in] McId     - Memory controller number

  @retval N/A
**/
VOID
ProgramDimmPopulation (
  IN PSYSHOST   Host,
  IN UINT8      Socket,
  IN UINT8      McId
  )
{
  UINT8                 SocketChannel;
  UINT8                 McChannel;
  UINT8                 Dimm;
  UINT32                Ddrtchnlpopulation;
  UINT32                Ddrchnlpopulation;
  struct channelNvram   (*ChannelNvList)[MAX_CH];
  struct dimmNvram      (*DimmNvList)[MAX_DIMM];
  UINT8                 NumChPerMc;
  NumChPerMc = GetNumChannelPerMc ();

  //
  // Determine how channels are populated for the MC
  //
  Ddrchnlpopulation = 0;
  Ddrtchnlpopulation = 0;
  ChannelNvList = GetChannelNvList (Host, Socket);
  for (McChannel = 0; McChannel < NumChPerMc; McChannel++) {
    //
    // Convert from MC channel number to socket channel number
    //
    SocketChannel = McId * NumChPerMc + McChannel;
    if ((*ChannelNvList)[SocketChannel].enabled == 0) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, SocketChannel);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }

      switch ((*ChannelNvList)[SocketChannel].dimmList[Dimm].keyByte) {
        case SPD_TYPE_AEP:
          Ddrtchnlpopulation |= (1 << McChannel);
          break;
        case SPD_TYPE_DDR4:
          Ddrchnlpopulation |= (1 << McChannel);
          break;
        case SPD_TYPE_DDR5:
          Ddrchnlpopulation |= (1 << McChannel);
          break;
        default:
          // Intentionally unhandled
          break;
      } //switch (...keybyte)
    } //for (dimm...)
  } //for (ch...)

  //
  // Progam DIMM population into Mesh2Mem registers
  //
  SetM2mMemoryChPopulation (Socket, McId, (UINT8)Ddrtchnlpopulation, (UINT8)Ddrchnlpopulation);
}

/**
  Apply any dimm specific workarounds required before enabling refresh

  @param[in] Socket   - Socket number

  @retval n/a
**/
VOID
DimmWorkaroundsBeforeRefreshEnable (
  IN UINT8      Socket
  )
{
#ifdef DDR5_SUPPORT
#ifdef USE_LATEST_DRAM_SPEC
  PSYSHOST                                     Host;
  UINT8                                        MaxChDdr;
  UINT8                                        Ch;
  struct dimmNvram                             (*DimmNvList)[MAX_DIMM];
  UINT8                                        Dimm;
  UINT8                                        Rank;
  UINT16                                       DateCode;
  DDR5_MODE_REGISTER_14_STRUCT                 Mr14;


  Host = (PSYSHOST) GetSysHostPointer ();
  MaxChDdr = GetMaxChDdr ();

  // Initialize Mr14 to default, then set ECS to manual mode per workaround to be applied

  Mr14.Data = 0;
  Mr14.Bits.ecs_mode = 1;

  for (Ch = 0; Ch < MaxChDdr; Ch++) {

    if (!IsChannelEnabled (Socket, Ch)) {
      continue;
    }

    DimmNvList = GetDimmNvList (Host, Socket, Ch);

    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if (!(IsDimmPresent (Socket, Ch, Dimm))) {
        continue;
      }

      if ((Host->nvram.mem.dramType == SPD_TYPE_DDR5) && (((*DimmNvList)[Dimm].SPDDramMfgId) == MFGID_HYNIX)) {
        // Check date code for Hynix DDR5 dimms
        DateCode = (((*DimmNvList)[Dimm].SPDModDate >> 8) & 0xFF) | (((*DimmNvList)[Dimm].SPDModDate << 8) & 0xFF00);

        if (DateCode <= 0x2018) {
          //For Hynix DDR5 dimms WW18 2020 or earlier, apply workaround to set ECS mode to manual
          for (Rank = 0; Rank < (*DimmNvList)[Dimm].numRanks; Rank++) {

            DramModeRegWrite (Socket, Ch, TWO_SUBCH_MASK, Dimm, Rank, DDR5_MODE_REGISTER_14_REG, Mr14.Data);
          } //Rank

        } //Date Code WW18, 2020

      } //DDR5 Hynix Dimm

    } // Dimm

  } // Ch
#endif //USE_LATEST_DRAM_SPEC
#endif //DDR5_SUPPORT
}

/**

  Initialize channel specific memory controller registers after training and before normal mode

  @param Host  - Pointer to sysHost

  @retval SUCCESS

**/
UINT32
LateConfig (
  PSYSHOST Host
  )
{
  UINT8                                        socket;
  UINT8                                        mcId;
  UINT8                                        ch;
  UINT8                                        dimm;
#ifdef DDRT_SUPPORT
  UINT8                                        MailboxStatus = 0;
  UINT32                                       Status;
  DIMM_INFO_STRUCT                             CurrentDimmInfo;
#endif // DDRT_SUPPORT
  INT16                                        CwlAdj = 0;
  INT16                                        Gnt2Erid = 0;
  UINT32                                       TwlEffDdrt;
  UINT8                                        DdrtCl = 0;  // DDRT CAS latency
  struct channelNvram                          (*channelNvList)[MAX_CH];
  struct dimmNvram                             (*dimmNvList)[MAX_DIMM];
  SCHEDULER_IDLETIME2_MCDDC_CTL_STRUCT         IdleTime2;
  T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_STRUCT      ddrtDimmBasicTiming;
  DDRT_ERR_LOG_CTL_MC_2LM_STRUCT               ddrtErrorLogCtrl;
  T_DDRT_PWRUP_MC_2LM_STRUCT                   ddrtPwUpMrc2Lm;
  DDRT_ERROR_MC_2LM_STRUCT                     errSignals;
  DDRT_CLK_GATING_MC_2LM_STRUCT                ddrtClkGating;
  BIOS_SCRATCHPAD0_STRUCT                      BiosScratchPad0;
  DDRT_TIMING_MC_2LM_STRUCT                    DdrtTiming;
  T_DDRT_MISC_DELAY_MC_2LM_STRUCT              DdrtMiscDelay;
  UINT8                                        RoundTrip = 0;
  BootMode                                     SysBootMode;
  BOOLEAN                                      TimeOutLockVal;
  SYS_SETUP                                    *Setup;
  MCCMICFG_N0_MC_2LM_STRUCT                    mccmiCfgN0Mc2LM;
  SPAREINTERVAL_MC_MAIN_STRUCT                 SpareInterval;
  UINT8                                        MaxChDdr;
  UINT8                                        MaxImc;
  UINT8                                        rank;

#ifndef DDR5_SUPPORT
  struct rankDevice                            (*rankStruct)[MAX_RANK_DIMM];
#endif // !DDR5_SUPPORT

  MaxImc = GetMaxImc ();
  Setup  = GetSysSetupPointer ();

  SysBootMode = GetSysBootMode ();
  ddrtDimmBasicTiming.Data = 0x0;

  socket = Host->var.mem.currentSocket;
  channelNvList = GetChannelNvList (Host, socket);

  OutputExtendedCheckpoint ((CHECKPOINT_MAJOR_CHANNEL_TRAINING, CHECKPOINT_MINOR_RMT, (UINT16)((CHECKPOINT_DATA_LATE_CONFIG << 8)|socket)));

  if (!IsMemFlowEnabled (LateMemoryControllerConfig)) {
    return SUCCESS;
  }
  //
  // Return if this socket is disabled
  //
  if (Host->nvram.mem.socket[socket].enabled == 0) {
    return SUCCESS;
  }

  if (Host->nvram.mem.socket[socket].maxDimmPop == 0) {
    return SUCCESS;
  }

  ConfigureCpgcRankMask (socket);

  for (mcId = 0; mcId < MaxImc; mcId++) {
    if (Host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
      continue;
    }

    ProgramDimmPopulation (Host, socket, mcId);

    TimeOutLockVal = GetM2mTimeoutLock (socket, mcId);
    // clear lock
    SetM2mTimeoutLock (socket, mcId, FALSE);

    // 4928667  imcX_m2mem_timeout should be enabled
    SetM2mTimeoutEn (socket, mcId, TRUE);

    // restore the lock if its there
    if (TimeOutLockVal) {
      SetM2mTimeoutLock (socket, mcId, TimeOutLockVal);
    }

  }

  //
  // NVMCTLR hook after DDR training
  //
  if (SysBootMode == NormalBoot && (Host->var.mem.subBootMode == ColdBoot || Host->var.mem.subBootMode == ColdBootFast)) {
    LateFnvConfigAccess (Host, socket);

    //
    // Clear BIOSSCRATCHPAD0.TrainingMode Bit[9] to indicate NVMDIMM exit training mode(training complete)
    //
    BiosScratchPad0.Data = GetStickyScratchpad (socket, BIOS_STICKY_SCRATCHPAD_0);
    BiosScratchPad0.Bits.TrainingMode = 0;
    SetStickyScratchpad (socket, BIOS_STICKY_SCRATCHPAD_0, BiosScratchPad0.Data);
  }

  TrrddDdrt2ChickenBit (Host, socket);

  //
  // ProgramErid2DataValid requires ProgramCmdToDataCompletionDelay to be executed first
  //
  ProgramCmdToDataCompletionDelay (socket);

  ProgramErid2DataValid (Host, socket);
  MaxChDdr = GetMaxChDdr ();
#ifdef DDRT_SUPPORT
  //Set delay from wpq ERID to underfill indication on read return / per Ch.
  //Done after training to get Correct RTL
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if ((*channelNvList)[ch].ddrtEnabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }
      GetWpqRidtoRt10nm ( Host, socket, ch, dimm);
    } // dimm loop
  } // ch loop
#endif // DDRT_SUPPORT

  //
  // HSD5330883: Need to program imcX_cY_ddrt_datapath_delay2.rrd_gnt2erid = imcX_cY_t_ddrt_dimmZ_basic_timing.t_ddrt_gnt2erid + 1
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if ((*channelNvList)[ch].ddrtEnabled == 0) {
      continue;
    }

    //
    // Get the CWL Adjust for the DDRT DIMM on this channel if it exists
    //
    dimmNvList = GetDimmNvList (Host, socket, ch);

#ifdef DDRT_SUPPORT
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }
      GetCwlAdj (Host, socket, ch, dimm, &CwlAdj);
      //
      // Get tCL and tCWL from DDRT (they can be different from DDR4)
      //
      GetDdrtCasLatencies (socket, ch, dimm, &DdrtCl, NULL, NULL, NULL);
    } // dimm loop
#endif // DDRT_SUPPORT

    //
    // twl_eff_ddrt = twl_ + ddrt_twl_adj - t_ddrt_twl_adj_neg
    //
    ddrtDimmBasicTiming.Data = MemReadPciCfgEp (socket, ch, T_DDRT_DIMM0_BASIC_TIMING_MC_2LM_REG);
    TwlEffDdrt = ddrtDimmBasicTiming.Bits.t_ddrt_twl + CwlAdj;

    //
    // Program DDRT RD/WR/GNT to Power Down Timing Constraints.
    //
    ConfigDdrtPowerDownTiming (Host, socket, ch, 2, (UINT8)(TwlEffDdrt + DDRT_PDEN_ADJUST), (UINT8)(DdrtCl + DDRT_PDEN_ADJUST));

    //
    // HSD5370588: CLONE from skylake_server: [CR B0] Powerdown exit to command timing register programmed incorrectly
    // This register is currently programmed to 8.  BIOS needs to program it to 15 on all channels with a DDRT DIMM installed.
    //
    ddrtPwUpMrc2Lm.Data = MemReadPciCfgEp (socket, ch, T_DDRT_PWRUP_MC_2LM_REG);
    ddrtPwUpMrc2Lm.Bits.t_ddrt_xp = 15;
    MemWritePciCfgEp (socket, ch, T_DDRT_PWRUP_MC_2LM_REG, ddrtPwUpMrc2Lm.Data);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }
      GetSetSxpGnt2Erid (Host, socket, ch, dimm, GSM_READ_ONLY, &Gnt2Erid);
      GetRoundTrip (socket, ch, GetLogicalRank (Host, socket, ch, dimm, DDRT_DIMM_RANK), GSM_READ_CSR, &RoundTrip);
      break;  // Max of one DDRT DIMM per channel
    }

    DdrtTiming.Data = MemReadPciCfgEp (socket, ch, DDRT_TIMING_MC_2LM_REG);
    DdrtTiming.Bits.rid2dealloc = RoundTrip - (Gnt2Erid - 9);
    MemWritePciCfgEp (socket, ch, DDRT_TIMING_MC_2LM_REG, DdrtTiming.Data);

    DdrtMiscDelay.Data = MemReadPciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG);
#ifndef GNRSRF_HOST
    DdrtMiscDelay.Bits.rpq_rid_to_credit_ret = DdrtCl - (Gnt2Erid - 9);
#endif // GNRSRF_HOST
    MemWritePciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG, DdrtMiscDelay.Data);

  } //Ch

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if ((*channelNvList)[ch].ddrtEnabled == 0) {
      continue;
    }
    ddrtClkGating.Data = MemReadPciCfgEp (socket, ch, DDRT_CLK_GATING_MC_2LM_REG);
    ddrtErrorLogCtrl.Data = MemReadPciCfgEp (socket, ch, DDRT_ERR_LOG_CTL_MC_2LM_REG);
    ddrtClkGating.Bits.dis_ddrts_clkgating = 1; // Disable clock gating preventing write credit starvation deassertion
    ddrtErrorLogCtrl.Bits.mask =  0x20;
    MemWritePciCfgEp (socket, ch, DDRT_CLK_GATING_MC_2LM_REG, ddrtClkGating.Data);
    MemWritePciCfgEp (socket, ch, DDRT_ERR_LOG_CTL_MC_2LM_REG, ddrtErrorLogCtrl.Data);

    //
    // HSD1706878655: [ICX] DDRT Writes needs to be posted writes by default
    // MRC must set .ddrt_wr_late_rsp_vld = 0 posted writes on all populated channels after DDR training and before normal traffic.
    // This is required for ICX, ICX-DE and SPR-SP.
    //
    if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      mccmiCfgN0Mc2LM.Data = MemReadPciCfgEp (socket, ch, MCCMICFG_N0_MC_2LM_REG);
      mccmiCfgN0Mc2LM.Bits.ddrt_wr_late_rsp_vld = 0;
      MemWritePciCfgEp (socket, ch, MCCMICFG_N0_MC_2LM_REG, mccmiCfgN0Mc2LM.Data);
    }
  } //Ch

#ifdef DDRT_SUPPORT
  //
  // check for INTEL DCPMM factory/clear required.
  // this should be done first, so that factory defaults the value of power setting; but it needs to be 15w/AVG power setting.
  //
  if (Setup->mem.FactoryResetClear == NGN_FACTORY_RESET_CLEAR_ENABLE) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      dimmNvList = GetDimmNvList (Host, socket, ch);
      for (dimm = 0; dimm < (*channelNvList)[ch].maxDimm; dimm++) {
        if (!((*dimmNvList)[dimm].DcpmmPresent)) {
          continue;
        }
        //
        // Send Factory Reset/Clear MB command to Intel DCPMM
        //
        RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Factory reset and clear\n");
        MailboxStatus = MB_STAT_SUCCESS;
        Status = FactoryResetClear (Host, socket, ch, dimm, &MailboxStatus);
        if (Status != SUCCESS || MailboxStatus != MB_STAT_SUCCESS) {

          RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            "Communication with the PMem DIMM failed during factory reset (Status %2d) (MB status 0x%X)\n", Status, MailboxStatus);

          if (MailboxStatus != MB_STAT_MEDIA_DISABLED) {   // Allow media disabled case to continue booting
            //log data format:
            //Byte 0 (Bit 0 - DIMM Slot Number, Bits 1-3 - Channel Number, Bits 4-6 - Socket Number,Bit 7 - Reserved)
            //Byte 1 - FW MB Opcode,Byte 2 - FW MB Sub-Opcode,Byte 3 - FW MB Status Code
            EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, socket, ch, dimm, NO_RANK);
            DisableDIMM (Host, socket, ch, dimm);
          }
        }
      }
    }
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    dimmNvList = &Host->nvram.mem.socket[socket].channelList[ch].dimmList;
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      if (!((*dimmNvList)[dimm].DcpmmPresent)) {
        continue;
      }

      Status = IdentifyDimm (Host, socket, ch, dimm, AIT_DRAM_FLAG_CHECK_DISABLE, &CurrentDimmInfo, &MailboxStatus);
      if (Status != SUCCESS) {
        RcDebugPrintWithDevice (SDBG_MINMAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "Communication with the PMEM DIMM failed during identifying dimms\n");
        EwlOutputType1 (WARN_DIMM_COMM_FAILED, WARN_MINOR_DIMM_COMM_FAILED_STATUS, socket, ch, dimm, NO_RANK);
        DisableDIMM (Host, socket, ch, dimm);
      }

      InitDcpmmPowerManagementPolicy (Host, socket, ch, dimm, CurrentDimmInfo.api);
    } // dimm
  } // ch
#endif // DDRT_SUPPORT

  //
  // Apply any vendor specific dimm workarounds required before refresh enabling
  //
  DimmWorkaroundsBeforeRefreshEnable (socket);

  //
  // Set DDRIO Signal direction
  //
  SetIodirectionValidDdrio (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, socket, ch);
    errSignals.Data = MemReadPciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG);
    if ((*channelNvList)[ch].ddrtEnabled) {
      if ((*dimmNvList)[0].DcpmmPresent == 1) {
        errSignals.Bits.ignore_ddrt_err0 = 0;
      }
      if ((*dimmNvList)[1].DcpmmPresent == 1) {
        errSignals.Bits.ignore_ddrt_err1 = 0;
      }
      errSignals.Bits.ignore_ddr4_error = 0;
      errSignals.Bits.ignore_erid_parity_error = 0;
      errSignals.Bits.ignore_ddrt_ecc_error = 0;

      SetCpgcMultiCreditOn (Host, socket, ch, FALSE);
    } else {
      errSignals.Bits.ignore_erid_parity_error = 1;
      errSignals.Bits.ignore_ddr4_error = 1;
      errSignals.Bits.ignore_ddrt_ecc_error = 1;
      errSignals.Bits.ignore_ddrt_err0 = 1;
      errSignals.Bits.ignore_ddrt_err1 = 1;
    }
    MemWritePciCfgEp (socket, ch, DDRT_ERROR_MC_2LM_REG, errSignals.Data);
  }

  //
  // These registers need to be cleared before normal mode
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    CpgcInorderModeEnable (socket, ch, FALSE);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    //
    // Enable adaptive page policy  (We set it to disabled in programTimings)
    //
    if ((Host->nvram.mem.RASmodeEx & ADDDC_EN) != ADDDC_EN) {
      if (Setup->mem.options & ADAPTIVE_PAGE_EN) {
        IdleTime2.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_IDLETIME2_MCDDC_CTL_REG);
        IdleTime2.Bits.adapt_pg_clse = 1;
        MemWritePciCfgEp (socket, ch, SCHEDULER_IDLETIME2_MCDDC_CTL_REG, IdleTime2.Data);
      }
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n Force Closed Page Mode due to ADDDC Workaround");
    }
  } // ch loop

  //
  // If we're coming out of S3, enable CKE_ON to bring DIMMs out
  // of self-refresh before enabling refresh and turning on normal
  // mode.
  //
  if (SysBootMode == S3Resume || Host->var.mem.subBootMode == WarmBootFast || Host->var.mem.subBootMode == ColdBootFast || Host->var.mem.subBootMode == NvDimmResume) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      MemWritePciCfgEp (socket, ch, TCMR2SHADOW_MCDDC_CTL_REG, (*channelNvList)[ch].TCMr2Shadow);
    } // ch loop
  }

  if (!(Host->var.mem.subBootMode == NvDimmResume)) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      dimmNvList = GetDimmNvList (Host, socket, ch);
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        if ((*dimmNvList)[dimm].dimmPresent == 0) {
          continue;
        }
        if ((Host->nvram.mem.dimmTypePresent == RDIMM) && ((*dimmNvList)[dimm].DcpmmPresent == 0)) {
          //
          // Disable register SMBUS per BIOS sighting #4930001
          //
          WriteRC(Host, socket, ch, dimm, 0, 0x1, RDIMM_RC2x);
        }
#ifdef  LRDIMM_SUPPORT
        if (IsLrdimmPresent (socket, ch, dimm)) {
          WriteBC (Host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
          (*dimmNvList)[dimm].rcLrFunc = 0;
        } //lrDimmPresent
#endif // LRDIMM_SUPPORT
      } //dimm
    } // ch loop
    // Disable any further RCW's
    Host->nvram.mem.socket[socket].wa = Host->nvram.mem.socket[socket].wa | WA_NO_MORE_RCW;
  }

  if (SysBootMode == NormalBoot && (Host->var.mem.subBootMode == ColdBoot || Host->var.mem.subBootMode == ColdBootFast)) {
    //
    // Enable C2 of NVMDIMM before Parity checking is enabled
    //
    ConfigNvmDimmC2 (Host, socket, FALSE);
  }

  if (!(Host->var.mem.subBootMode == NvDimmResume)) {
    if ((Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN) && (Host->nvram.mem.dimmTypePresent == RDIMM)) {
      EnableParityChecking (Host, socket);
    } else {
      DisableParityChecking (Host, socket);
    }
  }

  if ((SysBootMode == NormalBoot && Host->var.mem.subBootMode == ColdBootFast)) {
    IO_Reset(Host, socket);
  }

#ifndef DDR5_SUPPORT
  if (((Host->nvram.mem.RASmode & STAT_VIRT_LOCKSTEP) == STAT_VIRT_LOCKSTEP)) {
    // HSD 5331451
    RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "\n Set on-the-fly-burst per rank\n", socket);

    if (SysBootMode == NormalBoot) {
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }

        dimmNvList = GetDimmNvList(Host, socket, ch);
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }
          rankStruct  = &Host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;

          for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
            if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            (*rankStruct)[rank].MR0 = (*rankStruct)[rank].MR0 | MR0_ONTHEFLY;
            WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR0, BANK0);

            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT, " MR0=on-the-fly-burst\n", socket);

          } // rank loop
        } // DIMM loop
      } // ch loop
    } // NormalBoot
  } // Is sVLS enabled

  if ((Host->nvram.mem.RASmodeEx & ADDDC_EN) == ADDDC_EN) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }

      //
      // Skip the BC4/8 (on the fly) programming when WrCRC is enabled and E-die is present
      //
      if (IsEarlyWrCrcRequired (ch)) {
        continue;
      }

      if (((*channelNvList)[ch].features & X4_PRESENT) != X4_PRESENT) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT, "No X4 dimms in channel\n");
        continue;
      }

      if ( (SysBootMode == NormalBoot) && (Host->var.mem.subBootMode != WarmBootFast) ) {
        dimmNvList = GetDimmNvList(Host, socket, ch);
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }

          rankStruct  = &Host->var.mem.socket[socket].channelList[ch].dimmList[dimm].rankStruct;

          for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
            if (CheckRank(Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
              continue;
            }

            (*rankStruct)[rank].MR0 = (*rankStruct)[rank].MR0 | MR0_ONTHEFLY;
            WriteMRS (Host, socket, ch, dimm, rank, (*rankStruct)[rank].MR0, BANK0);

            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT, " MR0=on-the-fly-burst\n", socket);
          } // rank loop
        } // dimm loop
      } // if (Host->var.common.bootMode == NormalBoot)
    } // ch loop
  } // if ADDDC enabled in setup
#endif // !DDR5_SUPPORT

  if ((Host->nvram.mem.RASmode & RK_SPARE) == RK_SPARE) {
    for (mcId = 0; mcId < MaxImc; mcId++) {
      if (Host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
        continue;
      }
      //
      // numspare should be initialized to decimal 32 or less when rank sparing is enabled
      //
      SpareInterval.Data = MemReadPciCfgMC (socket, mcId, SPAREINTERVAL_MC_MAIN_REG);
      SpareInterval.Bits.numspare = 32;
      MemWritePciCfgMC (socket, mcId, SPAREINTERVAL_MC_MAIN_REG, SpareInterval.Data);
    } // Mc loop
  } // HSD 5331451

  SetBurstChop (Host);

  //
  // Set dis_wpq_clkgating on any channel populated with DDRT
  //
  if (IsSiliconWorkaroundEnabled ("S1409740754")) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if (IsDcpmmPresentChannel (socket, ch)) {
        ddrtClkGating.Data = MemReadPciCfgEp (socket, ch, DDRT_CLK_GATING_MC_2LM_REG);
        ddrtClkGating.Bits.dis_wpq_clkgating = 1;
        MemWritePciCfgEp (socket, ch, DDRT_CLK_GATING_MC_2LM_REG, ddrtClkGating.Data);
      }
    }
  }

  SetDdrtZqToAnyCmdDelay (socket);

  //
  // This needs to be done before refresh is enabled
  //
  if (!((SysBootMode == S3Resume || Host->var.mem.subBootMode == WarmBootFast))) {
    CheckAndProgramDqSwizzleAep (Host, socket);
  }

  if (IsSiliconWorkaroundEnabled ("S14010181268")) {

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      DisableTwoRefreshes (socket, ch);
    }
  }

  if ((!(Setup->mem.optionsExt & WR_CRC)) || (Host->nvram.mem.RASmode & STAT_VIRT_LOCKSTEP)) {
    //
    // Disable write CRC chicken bit before scrambling enabled
    //
    DisableWriteCrcChickenBits (Host, socket);
  }

  if (SysBootMode == S3Resume || Host->var.mem.subBootMode == WarmBootFast || Host->var.mem.subBootMode == NvDimmResume) {

    // Run DdrT S3 resume routine before exiting self refresh
    ddrtS3Resume (Host, socket);

    //
    // Exit self refresh
    //
    ExitSR10nm (Host, socket);

    if ((SysBootMode == NormalBoot && Host->var.mem.subBootMode == WarmBootFast)) {
      FnvPollingBootStatusRegister (Host, socket, FNV_CSR_UNLOCK);
      FnvPollingBootStatusRegister (Host, socket, FNV_MEDIA_DONE);
    }

    DdrtClearCkIdleMask (socket);

    if ((SysBootMode == NormalBoot && Host->var.mem.subBootMode == WarmBootFast) || (SysBootMode == S3Resume)) {
      //
      // Reload Scramble read seeds
      //
      LateFnvConfigAccess (Host, socket);

      //
      // Clear BIOSSCRATCHPAD0.TrainingMode Bit[9] after CKE to indicate NVMDIMM exit training mode
      //
      BiosScratchPad0.Data = GetStickyScratchpad (socket, BIOS_STICKY_SCRATCHPAD_0);
      BiosScratchPad0.Bits.TrainingMode = 0;
      SetStickyScratchpad (socket, BIOS_STICKY_SCRATCHPAD_0, BiosScratchPad0.Data);
    }

    if (Host->var.mem.subBootMode == NvDimmResume)  {
      for (ch = 0; ch < MaxChDdr; ch++) {
        if ((*channelNvList)[ch].enabled == 0) {
          continue;
        }
        dimmNvList = GetDimmNvList (Host, socket, ch);
        for (dimm = 0; dimm < MAX_DIMM; dimm++) {
          if ((*dimmNvList)[dimm].dimmPresent == 0) {
            continue;
          }
          if ((Host->nvram.mem.dimmTypePresent == RDIMM) && ((*dimmNvList)[dimm].DcpmmPresent == 0)) {
            //
            // Disable register SMBUS per BIOS sighting #4930001
            //
            WriteRC (Host, socket, ch, dimm, 0, 0x1, RDIMM_RC2x);
          }
#ifdef  LRDIMM_SUPPORT
          if (IsLrdimmPresent (socket, ch, dimm)) {
            WriteBC (Host, socket, ch, dimm, 0, 0, LRDIMM_F0, LRDIMM_BC7x);
            (*dimmNvList)[dimm].rcLrFunc = 0;
          } //lrDimmPresent
#endif // LRDIMM_SUPPORT
        } //dimm
      } // ch loop
      // Disable any further RCW's
      Host->nvram.mem.socket[socket].wa = Host->nvram.mem.socket[socket].wa | WA_NO_MORE_RCW;
      if ((Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN) && (Host->nvram.mem.dimmTypePresent == RDIMM)) {
        EnableParityChecking (Host, socket);
      } else {
        DisableParityChecking (Host, socket);
      }
    }
  } // if (S3Resume)
  else {

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      //
      // Set Panic/High WM = 2/1 when 16Gb DIMM is present on channel
      //
      ProgramRefreshTimingsWaterMark (socket, ch);

      //
      // Set refresh mode in the DRAM
      //
      SetDramRefreshMode (socket, ch, GetRefreshModePolicy ());
      SetMcTimingsLateConfig (socket, ch);
    } // ch loop

    //
    // Assert CKE on each channel and enable refresh
    //
    EnableHostRefresh (Host, socket);

    Host->var.mem.socket[socket].hostRefreshStatus = 1;
  }

  //
  // 1507940650: [SPR A0 PO] Periodic retrain on DDRIO causes SW memtest failure
  // This code needs to be revisited
  //

  // Enable the periodic retrain on DDRIO
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (!IsChannelEnabled (socket, ch)) {
      continue;
    }
    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if (IsDcpmmPresentDimm (socket, ch, dimm)) {
        continue;
      }

      for (rank = 0; rank < Host->var.mem.socket[socket].maxRankDimm; rank++) {
        if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT) || (rank >= (*dimmNvList)[dimm].numRanks)) {
          continue;
        }
        if (IsMemFlowEnabled (PeriodicTxReTraining)) {
          RcDebugPrintWithDevice (
            SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
            "TX_RETRAIN_ENABLE\n"
            );
          // Configure data buffer tDQS2DQ tracking control word
          ConfigureDbDqs2Dq (socket, ch, dimm, rank, DQS2DQ_TRACKING);

          ConfigureDdrioTxRetrainEnable (socket, ch, dimm, rank, TX_RETRAIN_ENABLE);
        } else {
          RcDebugPrintWithDevice (
            SDBG_MAX, socket, ch, dimm, NO_SUBCH, rank, NO_STROBE, NO_BIT,
            "TX_RETRAIN_DISABLE\n"
            );
          // Retrain disabled - leave DB in default (initialization) state
          ConfigureDbDqs2Dq (socket, ch, dimm, rank, DQS2DQ_INITIALIZATION);

          ConfigureDdrioTxRetrainEnable (socket, ch, dimm, rank, TX_RETRAIN_DISABLE);
        }

      }//Rank
    }//dimm
  }//ch



  SetPostRefreshEnableChickenBits (Host, socket);

  FnvPollCreditReadyTarget (Host, socket, SysBootMode);

  //
  // Stop FMC cache access
  //
  Host->nvram.mem.FmcCacheDone = 1;

  //
  // Poll for AIT DRAM Ready
  //
  if ((SysBootMode == NormalBoot && (Host->var.mem.subBootMode == ColdBoot || Host->var.mem.subBootMode == ColdBootFast))
    || (SysBootMode == S3Resume)) {
    FnvPollingBootStatusRegister (Host, socket, FNV_DRAM_READY);
  }

  //
  // Disable posted CSR writes
  //
  PostedMethodDisable ();

  return SUCCESS;
} // LateConfig


/**

  Enables CMD/ADDR Parity

  @param Host    - Pointer to sysHost
  @param socket  - Processor socket

  @retval N/A

**/
VOID
EnableCAParityRuntime (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                                             ch;
  UINT8                                             mcId;
  UINT8                                             dimm;
  UINT8                                             aepDimmCountImc[MAX_IMC]={0};
  struct  channelNvram                              (*channelNvList)[MAX_CH];
  struct  dimmNvram                                 (*dimmNvList)[MAX_DIMM];

  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT                   dp_chkn;
  DDR4_CBIT_PARITY_MCDDC_CTL_STRUCT   ddr4cbitParity;
  UINT8                                             MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);

  RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                 "Enabling C/A Parity\n");

  if (IsBrsPresent(Host, socket)) {
    return;
  }

  // count NVMDIMMs
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    mcId = GetMCID(Host, socket, ch);
    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent){
        aepDimmCountImc[mcId]++;
      }
    } // dimm
  } // ch

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    ConfigureCaParityControl (socket, ch, Host->nvram.mem.ratioIndex);

    //program Per bit inclusion control for ddr4 Cbits in command/address parity generation.
    ddr4cbitParity.Data = MemReadPciCfgEp (socket, ch, DDR4_CBIT_PARITY_MCDDC_CTL_REG);

    // for encoded QuadCS mode with 2 ranks per CS
    // for direct mapped QR or DR
    ddr4cbitParity.Bits.res_enable = 0;
    if ((*channelNvList)[ch].encodedCSMode == 2) {
      ddr4cbitParity.Bits.res_enable = (*channelNvList)[ch].cidBitMap;
    }
    MemWritePciCfgEp (socket, ch, DDR4_CBIT_PARITY_MCDDC_CTL_REG, ddr4cbitParity.Data);

    dp_chkn.Data= MemReadPciCfgEp (socket, ch, MC0_DP_CHKN_BIT_MCDDC_DP_REG);
    dp_chkn.Bits.dis_rdimm_par_chk = 0;
    dp_chkn.Bits.en_rdimm_par_err_log = 1;  // Enable RDIMM parity error logging
    if (!(UbiosGenerationOrHsleEnabled ())) {
      if (Host->nvram.mem.socket[socket].cmdClkTrainingDone == 0) {
        dp_chkn.Bits.en_rdimm_par_err_log = 0;
      }
    }
    MemWritePciCfgEp (socket, ch, MC0_DP_CHKN_BIT_MCDDC_DP_REG, dp_chkn.Data);
  } // ch loop

} // EnableCAParityRuntime


/**
  Disable the memory controller to issue two refreshes upon exiting self-refresh.

  @param[in] Socket  - Socket number
  @param[in] Ch      - Channel number

  @retval N\A

**/
VOID
DisableTwoRefreshes (
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  MCMNT_CHKN_BIT_MCDDC_CTL_STRUCT chknBitMcddcCtl;

  chknBitMcddcCtl.Data = MemReadPciCfgEp (Socket, Ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG);
  chknBitMcddcCtl.Bits.zqlsrl = 0;
  MemWritePciCfgEp (Socket, Ch, MCMNT_CHKN_BIT_MCDDC_CTL_REG, chknBitMcddcCtl.Data);
}

/**

  Exits self refresh

  @param Host    - Pointer to sysHost
  @param socket  - Processor socket

  @retval N/A

**/
VOID
ExitSR10nm (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                     ch;
  volatile UINT32           data = 0;
  VOID                      (*functionPtr)(PSYSHOST Host, UINT8 socket);
  UINT8                     (*endFunctionPtr)();
  UINT32                    **dataPtr;
  UINT32                    **endDataPtr;
  TCRFTP_MCDDC_CTL_STRUCT   tcrftp;
  UINT32                    tcrftpOrg[MAX_CH];
  struct  channelNvram      (*channelNvList)[MAX_CH];
  UINT32                    tczqcalOrg[MAX_CH];
  TCZQCAL_MCDDC_CTL_STRUCT  tczqcal;
  UINT8                     MaxChDdr;
  MaxChDdr      = GetMaxChDdr ();
  channelNvList = GetChannelNvList(Host, socket);
  //
  // First read the entire function into cache so that there are
  // no flash reads during the S3 exit flow.  A flash read takes
  // too long and will violate timing parameters.
  //
  functionPtr = ExitSelfRefreshCriticalSection;
  endFunctionPtr = ExitSelfRefreshCriticalSectionEndMarker;
  dataPtr = (UINT32 **)(&functionPtr);
  endDataPtr = (UINT32 **)(&endFunctionPtr);
  //
  // Check to make sure the compiler didn't reorganize the end
  // function marker to someplace before the critical function.
  // If it did, then set the endFunctionPtr to a place 1000 hex
  // bytes after the start of the function.  This is more than
  // enough bytes to read the entire function into processor
  // cache.
  if (*endDataPtr <= *dataPtr) {
    *endDataPtr = *dataPtr + 0x1000;
  }

  while (*dataPtr < *endDataPtr) {
    data = **dataPtr;
    (*dataPtr)++;
  }

  if (data) {
    data = 0;
  } // To prevent GCC compiler from warning.(unused-but-set-variable)

  if (Host->var.mem.subBootMode == AdrResume) {
    CoreReleaseADRClamps(Host, socket);
  }
  ZeroMem (tcrftpOrg, sizeof (tcrftpOrg));
  ZeroMem (tczqcalOrg, sizeof (tczqcalOrg));
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    tcrftp.Data          = MemReadPciCfgEp (socket, ch, TCRFTP_MCDDC_CTL_REG);
    tcrftpOrg[ch]        = tcrftp.Data;
    tcrftp.Bits.t_refi >>= 1; // divide by 2 during ExitSelfRefresh10nm
    MemWritePciCfgEp (socket, ch, TCRFTP_MCDDC_CTL_REG, tcrftp.Data);
    tczqcal.Data         = MemReadPciCfgEp (socket, ch, TCZQCAL_MCDDC_CTL_REG);
    tczqcalOrg[ch]       = tczqcal.Data;
    tczqcal.Bits.zqcsperiod = 2;
    MemWritePciCfgEp (socket, ch, TCZQCAL_MCDDC_CTL_REG, tczqcal.Data);
  } // ch loop

  //
  // At this point, the entire function - ExitSelfRefresh10nm - has
  // been read into processor cache.  Now we can execute it.
  //
  ExitSelfRefresh10nm(Host, socket);

  //
  // Enable ZQCS
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    SetZQMask (Host, socket, ch, 0x00);
  } // ch loop

  // Guarantee multiple ZQCS commands on S3 resume and fast warm reset paths.(The ZQCL inside ExitSelfRefresh10nm function is only executed for NVDIMM)
  if (GetSysBootMode () == S3Resume || Host->var.mem.subBootMode == WarmBootFast) {
    FixedDelayMicroSecond (16000);
  }

  //
  // Restore tREFI and ZQCAL
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    MemWritePciCfgEp (socket, ch, TCRFTP_MCDDC_CTL_REG, tcrftpOrg[ch]);
    MemWritePciCfgEp (socket, ch, TCZQCAL_MCDDC_CTL_REG, tczqcalOrg[ch]);
  } // ch loop
} // ExitSR10nm

//
// Enable Stagger Host Refresh feature
//
VOID
EnableHostRefresh (
  PSYSHOST  Host,
  UINT8     Socket
  )
{
  volatile UINT32           Data = 0;
  VOID                      (*FunctionPtr)(PSYSHOST Host, UINT8 Socket, UINT8 TrefiPerAviableChannel, UINT8 NumberOfRank, UINT16 TrefiDelay);
  UINT8                     (*EndFunctionPtr)();
  UINT32                    **DataPtr;
  UINT32                    **EndDataPtr;
  SYS_SETUP                 *Setup;

  Setup = GetSysSetupPointer ();

  //
  // First read the entire function into cache so that there are no flash reads.
  // A flash read takes too long and will violate timing parameters.
  //
  FunctionPtr = SetRefreshEnable10nm;
  EndFunctionPtr = SetRefreshEnable10nmEndMarker;
  DataPtr = (UINT32 **)(&FunctionPtr);
  EndDataPtr = (UINT32 **)(&EndFunctionPtr);
  //
  // Check to make sure the compiler didn't reorganize the end
  // function marker to someplace before the critical function.
  // If it did, then set the endFunctionPtr to a place 1000 hex
  // bytes after the start of the function.  This is more than
  // enough bytes to read the entire function into processor
  // cache.
  if (*EndDataPtr <= *DataPtr) {
    *EndDataPtr = *DataPtr + 0x1000;
  }

  while (*DataPtr < *EndDataPtr) {
    Data = **DataPtr;
    (*DataPtr)++;
  }

  if (Data) {
    Data = 0;
  } // To prevent GCC compiler from warning.(unused-but-set-variable)

  //
  // At this point, the entire function - SetRefreshEnable10 - has
  // been read into processor cache.  Now we can execute it.
  //
  GetPerformanceCounter ();
  SetRefreshEnable10nm (Host, Socket, Setup->mem.TrefiPerChannel, Setup->mem.TrefiNumofRank, Setup->mem.TrefiDelay);
}

#ifdef  LRDIMM_SUPPORT
#ifdef  DEBUG_CODE_BLOCK
VOID
DumpLrbuf10nm (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  )
{
  UINT32 lrbufData;
  UINT16 offset;

  if (GetDebugLevel () & SDBG_MAX) {

    AcquirePrintControl ();
    // debug... dump all MB config space via SMBus reads
    RcDebugPrint (SDBG_DEFAULT, "\nN%d.C%d.D%d - LRDIMM config dump:\n", socket, ch, dimm);
    RcDebugPrint (SDBG_DEFAULT, "Byte offset: 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F");

    for (offset = 0; offset < 0xFF; offset = offset + 4) {

      if (SUCCESS == ReadLrbufSmb (Host, socket, ch, dimm, offset, &lrbufData)) {
        if (offset % 16 == 0) {
          RcDebugPrint (SDBG_DEFAULT, "\n       0x%03x:", offset);
        }
        RcDebugPrint (SDBG_DEFAULT, " 0x%02x 0x%02x 0x%02x 0x%02x",
                  lrbufData & 0xFF, (lrbufData >> 8) & 0xFF, (lrbufData >> 16) & 0xFF, (lrbufData >> 24) & 0xFF);
      } else {
         RcDebugPrint (SDBG_DEFAULT, "Failure: LRDIMM SMBus read at offset 0x%03x\n", offset);
      }
    }
    RcDebugPrint (SDBG_DEFAULT, "\n\n");

    ReleasePrintControl ();
  }
}
#endif  // DEBUG_CODE_BLOCK
#endif  //LRDIMM_SUPPORT

/**

  Check if the input Mc is configured for DDR Single-Device Data Correction (SDDC)

  @param[in]  Sckt  - Socket index to check
  @param[in]  Mc    - Mc index to check

  @retval TRUE  The input Mc is configured for SDDC
  @retval FALSE The input MC is not configured for SDDC

**/
BOOLEAN
IsDdrSddcConfig (
  IN UINT8 Sckt,
  IN UINT8 Mc
  )
{
  SYSHOST *Host;
  BOOLEAN DdrtPresentOnMc;
  UINT8   ChIndex;
  UINT8   ScktCh;
  UINT8   NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  Host = GetSysHostPointer ();

  DdrtPresentOnMc = FALSE;
  for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
    ScktCh = (Mc * NumChPerMc) + ChIndex;
    if (Host->nvram.mem.socket[Sckt].channelList[ScktCh].enabled == 0) {
      continue;
    }
    if (AepDimmPresent (Sckt, ScktCh, NULL)) {
      DdrtPresentOnMc = TRUE;
      break;
    }
  }

  if (!DdrtPresentOnMc && Host->nvram.mem.eccEn) {
    return TRUE;
  }
  return FALSE;
}

/**

  Set the m2mem_defeatures0.wait4bothhalves and mcchan_amap.wait4bothhalves register fields

  @param[in]  Sckt  - Socket index to configure
  @param[in]  Mc    - Mc index to configure

**/
VOID
SetAllWait4BothHalves (
  IN UINT8 Sckt,
  IN UINT8 Mc
  )
{
  SYSHOST *Host;
  UINT8   ChIndex;
  UINT8   ScktCh;
  UINT8   NumChPerMc;

  NumChPerMc = GetNumChannelPerMc ();
  Host = GetSysHostPointer ();
  //
  // The copy of wait4bothhalves in mcchan_amap should be set to match the one in defeatures0 for ICX
  //
  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    SetM2mDefeatures0Wait4BothHalves (Sckt, Mc, TRUE);
  }
  for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
    ScktCh = (Mc * NumChPerMc) + ChIndex;
    if (Host->nvram.mem.socket[Sckt].channelList[ScktCh].enabled == 0) {
      continue;
    }
    McSetWaitForBothHalves (
      Sckt,
      Mc,
      ChIndex
      );
  }
}

/**

  Set the required registers for the current SDDC configuration

  @param[in]  Sckt  - Socket index to configure
  @param[in]  Mc    - Mc index to configure

**/
VOID
SetSddcCsrs (
  VOID
  )
{
  UINT8 Sckt;
  UINT8 Mc;
  UINT8 MaxImc;

  MaxImc = GetMaxImc();

  for (Sckt = 0; Sckt < MAX_SOCKET; Sckt++) {
    if (!SocketPresent (Sckt)) {
      continue;
    }
    for (Mc = 0; Mc < MaxImc; Mc++) {
      if (!CheckSocketMcEnabled (Sckt, Mc)) {
        continue;
      }
      if (!IsDdrSddcConfig (Sckt, Mc)) {
        SetAllWait4BothHalves (Sckt, Mc);
      }
    }
  }
}

/**
  Adjust the DDRT starvation timeout as needed, depending on work-arounds, etc.

  @param[in]  Host      A pointer to the SysHost structure
  @param[in]  Socket    A socket containing DDRT DIMMs
  @param[in]  Channel   A channel containing a DDRT DIMM

  @retval   EFI_STATUS  Indicates whether the timeout was succesfully adjusted
**/
EFI_STATUS
AdjustDdrtStarvationTimeoutAsNeeded (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  DDRT_STARVE_CTL_MC_2LM_STRUCT DdrtStarveCtl;

  if (IsSiliconWorkaroundEnabled ("S1909288819")) {
    DdrtStarveCtl.Data = MemReadPciCfgEp (Socket, Channel, DDRT_STARVE_CTL_MC_2LM_REG);
    DdrtStarveCtl.Bits.tmr = 0x13;
    DdrtStarveCtl.Bits.wrcrdt_threshold = 0x8;
    DdrtStarveCtl.Bits.tgr_threshold = 0x8;
    MemWritePciCfgEp (Socket, Channel, DDRT_STARVE_CTL_MC_2LM_REG, DdrtStarveCtl.Data);
  }

  return EFI_SUCCESS;
}

/**
  Disable DropNoData for ICX DDRT since unexpected Data Packet is not supported for ICX (was a SKX feature) DDRT

  @param[in]  Host      A pointer to the SysHost structure
  @param[in]  Socket    A socket containing DDRT DIMMs
  @param[in]  Channel   A channel containing a DDRT DIMM

  @retval   None
**/
VOID
DisableDropNoData (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket,
  IN  UINT8     Channel
  )
{
  SBDEFEATURES0_MC_2LM_STRUCT SbDefeatureS0;

  SbDefeatureS0.Data = MemReadPciCfgEp (Socket, Channel, SBDEFEATURES0_MC_2LM_REG);
  SbDefeatureS0.Bits.disdropnodata = 0x1;
  MemWritePciCfgEp (Socket, Channel, SBDEFEATURES0_MC_2LM_REG, SbDefeatureS0.Data);
}

/**
  Increase the time to block subsequent data returns after the start of non-retry data transfer as a means of
  preventing system hangs.  The application of this is dependent on which security features are enabled and which
  programs and steppings this work-around targets.

  @param[in]  Host      A pointer to the SysHost structure
  @param[in]  Socket    The target socket

  @retval   N/A
**/
VOID
IncreaseBlockTimeAsNeeded (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  UINT8                               Channel;
  CMPL_TO_DATA_DELAY_MCDDC_DP_STRUCT  CmplToDataDelay;
  UINT8                               Controller;
  UINT8                               ControllerChannel;
  SECURITY_POLICY                     *SecurityPolicy;
  UINT8                               MaxChannel;
  UINT8                               MaxController;
  UINT8                               MaxControllerChannel;
  BOOLEAN                             SgxEnabled;
  UINT8                               EarlyCompletionsEnabled;

  // Determine whether SGX is enabled.
  SecurityPolicy = GetSecurityPolicy ();
  SgxEnabled = (SecurityPolicy->EnableSgx == SECURITY_POLICY_ENABLE);

  if (IsSiliconWorkaroundEnabled ("S1707017332") && SgxEnabled) {

    // Loop over enabled socket channels.
    MaxChannel = GetMaxChDdr ();
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (IsChannelEnabled (Socket, Channel) == FALSE) {
        continue;
      }

      CmplToDataDelay.Data = MemReadPciCfgEp (Socket, Channel, CMPL_TO_DATA_DELAY_MCDDC_DP_REG);
      CmplToDataDelay.Bits.data_return_blk_norm = 5;
      MemWritePciCfgEp (Socket, Channel, CMPL_TO_DATA_DELAY_MCDDC_DP_REG, CmplToDataDelay.Data);
    } // for (Channel = 0...)

  } else if (IsSiliconWorkaroundEnabled ("S1409508104")) {

    // If SGX or TME is enabled...
    if (SgxEnabled || IsTmeEnabled (Socket)) {

      // Loop over populated memory controllers.
      MaxController = GetMaxImc ();
      for (Controller = 0; Controller < MaxController; Controller++) {
        if (IsMcPopulated (Socket, Controller) == FALSE) {
          continue;
        }

        GetSetM2mAkEarlyCompletionsEnabled (Socket, Controller, GSM_READ_ONLY, &EarlyCompletionsEnabled);

        // If AK early completions or prefetch is enabled...
        if ((EarlyCompletionsEnabled != 0) ||
            IfFeatureEnabled (TypeXptPrefetch))
        {

          // Loop over enabled channels in this memory controller.
          MaxControllerChannel = GetNumChannelPerMc ();
          for (ControllerChannel = 0; ControllerChannel < MaxControllerChannel; ControllerChannel++) {
            Channel = (Controller * MaxControllerChannel) + ControllerChannel;
            if (IsChannelEnabled (Socket, Channel) == FALSE) {
              continue;
            }

            CmplToDataDelay.Data = MemReadPciCfgEp (Socket, Channel, CMPL_TO_DATA_DELAY_MCDDC_DP_REG);
            CmplToDataDelay.Bits.data_return_blk_norm = 4;
            MemWritePciCfgEp (Socket, Channel, CMPL_TO_DATA_DELAY_MCDDC_DP_REG, CmplToDataDelay.Data);
          }

        } // if (EarlyCompletionsEnabled || (GetM2mSysFeatures0PreftchDis == FALSE))

      } // for (Controller = 0...)

    } // if (SgxEnabled || IsTmeEnabled (Socket))

  }
}

/**
  Ignore alert signaling for x16 ECC devices when WrCRC is enabled

  @param[in] Host   - Pointer to sysHost
  @param[in] socket - Processor socket

  @retval N/A
**/
VOID
IgnoreX16EccAlertSignal (
  IN PSYSHOST Host,
  IN UINT8    Socket
  )
{
  UINT8                           Ch;
  struct  channelNvram            (*ChannelNvList)[MAX_CH];
  DDR4_CA_CTL_MCDDC_DP_STRUCT     Ddr4CaCtl;
  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT DpChkn;
  SYS_SETUP                       *Setup;
  UINT8                           MaxChDdr;

  Setup = GetSysSetupPointer ();

  if ((Setup->mem.optionsExt & WR_CRC) == 0) {
    return;
  }

  RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                          "Programming the host registers to ignore ALERT_n signaling for x16 ECC\n");

  MaxChDdr = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }

    if (IsThereAnyDimmX16InChannel (Socket, Ch) && IsThereAnyEccDimmInChannel (Socket, Ch)) {
      Ddr4CaCtl.Data = MemReadPciCfgEp (Socket, Ch, DDR4_CA_CTL_MCDDC_DP_REG);
      //
      // Disable error flow
      //
      Ddr4CaCtl.Bits.erf_en0 = 0;
      MemWritePciCfgEp (Socket, Ch, DDR4_CA_CTL_MCDDC_DP_REG, Ddr4CaCtl.Data);

      DpChkn.Data = MemReadPciCfgEp (Socket, Ch, MC0_DP_CHKN_BIT_MCDDC_DP_REG);
      //
      // Disable parity error logging
      //
      DpChkn.Bits.dis_rdimm_par_chk    = 1;
      DpChkn.Bits.en_rdimm_par_err_log = 0;
      MemWritePciCfgEp (Socket, Ch, MC0_DP_CHKN_BIT_MCDDC_DP_REG, DpChkn.Data);
    }
  } // Ch loop
} // IgnoreX16EccAlertSignal

/**

  Switches to normal mode

  @param[in] Host    - Pointer to sysHost

  @retval SUCCESS    - Always returns SUCCESS status

**/
UINT32
SwitchToNormalMode (
  IN  PSYSHOST  Host
  )
{
  UINT8                                         socket = Host->var.mem.currentSocket;
  UINT8                                         ch;
  UINT8                                         dimm;
  struct                                        dimmNvram(*dimmNvList)[MAX_DIMM];
  UINT8                                         mcId;
  UINT8                                         rank;
  UINT8                                         rankNum;
  struct channelNvram                           (*channelNvList)[MAX_CH];
  CKE_LL0_MCDDC_CTL_STRUCT                      cke_ll0;
  CKE_LL1_MCDDC_CTL_STRUCT                      cke_ll1;
  CKE_LL2_MCDDC_CTL_STRUCT                      cke_ll2;
  PKGC_CKE_MCDDC_CTL_STRUCT                     PkgcCke;
  UINT32                                        PkgcCkeReg = PKGC_CKE_MCDDC_CTL_REG;
  RCOMP_GLOBALCTRL1_MC_GLOBAL_STRUCT            RcompGlobalCtrl;
  MCMTR_MC_MAIN_STRUCT                          mcMtr;
  DDRT_MCA_CTL_MC_2LM_STRUCT                    ddrtMcaCtl;
  DDRT_MISC_CTL_MC_2LM_STRUCT                   ddrtMiscCtl;
  DDRT_RETRY_TIMER2_MC_2LM_STRUCT               ddrtRetryTimer2;
#if (MC_IP_VER < MEM_IP_19ww29g)
  DDRT_RETRY_CTL_MC_2LM_STRUCT                  ddrtRetryCtl;
#else
  DDRT_RETRY_CTL_N0_MC_2LM_STRUCT               ddrtRetryCtl;
#endif
  DDRT_RETRY_TIMER_MC_2LM_STRUCT                ddrtRetryTimer;
#ifdef DDRT_SUPPORT
  T_DDRT_MISC_DELAY_MC_2LM_STRUCT               ddrtMiscDelay;
  DDRT_CREDIT_LIMIT_MC_2LM_STRUCT               ddrtCreditLimit;
  DDRT_DEFEATURE_MC_2LM_STRUCT                  ddrtDefeature;
  UINT32                                        DdrtWriteLatency;
#endif // DDRT_SUPPORT
#if (MEM_IP_VER == MEM_IP_TODO)
  DIMMTEMPSTAT_0_MCDDC_CTL_STRUCT               DimmTempStat;
  UINT32                                        DimmTempStatRegOffset[] = {DIMMTEMPSTAT_0_MCDDC_CTL_REG, DIMMTEMPSTAT_1_MCDDC_CTL_REG};
  UINT8                                         McCreditCount[MAX_IMC];
#endif  // (MEM_IP_VER == MEM_IP_TODO)
  UINT8                                         EnabledChannelCount[MAX_IMC];
  DDRT_FNV_INTR_CTL_MCDDC_DP_STRUCT             ddrtIntrCtl;
  MC0_DP_CHKN_BIT_MCDDC_DP_STRUCT               Dp_Chkn;
  SYS_SETUP                                     *Setup;
#ifndef GNRSRF_HOST
  UINT32                                        DdrioDelay;
  PCLS_SPAREDNIBBLE_CAPTURE_DELAY_MCDDC_DP_STRUCT PclsSpareNibbleCaptureDelay;
#endif // !GNRSRF_HOST
  UINT8                                         NumChPerMc;
  UINT8                                         MaxChDdr;
  UINT8                                         MaxImc;
  SCHEDULER_SCRATCH_MCDDC_CTL_STRUCT            SchedulerScratch;
  BootMode                                      SysBootMode;
  EFI_STATUS                                    EfiStatus;
  NumChPerMc = GetNumChannelPerMc ();
  Setup = GetSysSetupPointer ();
  MaxImc = GetMaxImc ();
  SysBootMode = GetSysBootMode();

  //
  // Enable WrCRC in the DDRIO, MC and DRAM
  //
  if ((Setup->mem.optionsExt & WR_CRC) && !(Host->nvram.mem.RASmode & STAT_VIRT_LOCKSTEP)) {
    EnableWrCrc (Host, socket);
    if (IsSiliconWorkaroundEnabled ("S14011403994")) {
      SetWriteToWriteTurnaroundTimingsByOffset (MemTechDdr, socket, WRITECAS_TO_WRITECAS_DELAY_OFFSET_1_DCLK);
    }
  }

  if (!IsMemFlowEnabled (NormalModeSwitch)) {
    return SUCCESS;
  }
  // Continue if this socket is not present
  if (Host->nvram.mem.socket[socket].enabled == 0) {
    return SUCCESS;
  }

  ProgramTemperatureRefresh();

  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr      = GetMaxChDdr ();
#ifdef DDRT_SUPPORT
  SyncCompletionDelayResetCounts ();
#endif // DDRT_SUPPORT
  SetVLSModePerChannel(Host, socket);
  SetVLSRegionPerChannel(Host, socket);

  for (mcId = 0; mcId < MaxImc; mcId++) {
    if (Host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
      continue;
    }

    // HSD 5331110 Disable Periodic Rcomp during memory training
    if (Host->nvram.mem.socket[socket].DcpmmPresent == 0) {
      RcompGlobalCtrl.Data = MemReadPciCfgMC (socket, mcId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG);
      RcompGlobalCtrl.Bits.dis_rcomp = 0;
      MemWritePciCfgMC (socket, mcId, RCOMP_GLOBALCTRL1_MC_GLOBAL_REG, RcompGlobalCtrl.Data);
    }

    //
    // Bios must program M2MBIOSQUIESCE.usecmiidlefordrain to '0' before enabling normal memory traffic
    // This workaround is applicable on ICX-SP and ICX-D A0
    //
    if (IsSiliconWorkaroundEnabled ("S1407609824")) {
      SetM2mBiosQuiesceUseCmiIdleForDrain (socket, mcId, 0);
    }
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

    //
    // Clear the wr_cmpl_fifo_crdt_dis bit for each channel
    //
    WriteCompletionCredits (Host, socket, ch, FALSE);

    RcompTimerConfig (Host, socket, ch);

#ifndef GNRSRF_HOST
    // Program PCLS_SPAREDNIBBLE_CAPTURE_DELAY with DDRIO delay
    if (Host->nvram.mem.RASmodeEx & PCLS_EN) {
      if (GetPclsCaptureDelayDddrio (socket, ch, &DdrioDelay) == EFI_SUCCESS) {
        PclsSpareNibbleCaptureDelay.Data = 0;
        PclsSpareNibbleCaptureDelay.Bits.capture_delay = DdrioDelay;
        MemWritePciCfgEp (socket, ch, PCLS_SPAREDNIBBLE_CAPTURE_DELAY_MCDDC_DP_REG, PclsSpareNibbleCaptureDelay.Data);
      } else {
        RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_157);
      }
    }
#endif // !GNRSRF_HOST
  }

  SwitchToNormalModeDdrio( Host, socket);

  SwitchToNormalModeFmc1 (Host, socket);

  //
  // Determine how many channels enabled on each MC
  //
  for (mcId = 0; mcId < MaxImc; mcId++) {
    EnabledChannelCount[mcId] = 0;
    for (ch = 0; ch < NumChPerMc; ch++) {
      if ((*channelNvList)[ch + (NumChPerMc * mcId)].enabled == 1) {
        EnabledChannelCount[mcId]++;
      }
    }
  }

  SwitchToNormalModeMcPerformance (Host, socket);

  //
  // Program NVMDIMM performance profile
  //
  ProgramNvmdimmPerf (Host, socket);

  RestoreRidEntries (socket);
#ifdef DDRT_SUPPORT
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].ddrtEnabled == 0) {
      continue;
    }

    DdrtCapErrorWA (Host, socket, ch);
    if (IsSiliconWorkaroundEnabled ("22010473670")) {
      DdrtSilentDataCorruptionWA (socket, ch);
    }
    ddrtMiscDelay.Data = MemReadPciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG);
    ddrtMiscDelay.Bits.force_ddr4_mode = 0;
    ddrtMiscDelay.Bits.force_ddrt_mode = 0;
    MemWritePciCfgEp (socket, ch, T_DDRT_MISC_DELAY_MC_2LM_REG, ddrtMiscDelay.Data);
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if ((*channelNvList)[ch].ddrtEnabled == 0) {
      continue;
    }

    ddrtCreditLimit.Data = MemReadPciCfgEp (socket, ch, DDRT_CREDIT_LIMIT_MC_2LM_REG);
    ddrtCreditLimit.Bits.ddrt_rd_credit = (*channelNvList)[ch].FmcRdCreditLimit;
    ddrtCreditLimit.Bits.ddrt_wr_credit = (*channelNvList)[ch].FmcWrCreditLimit;
    ddrtCreditLimit.Bits.min_rd_credit_partials = 1;
    MemWritePciCfgEp (socket, ch, DDRT_CREDIT_LIMIT_MC_2LM_REG, ddrtCreditLimit.Data);

    ddrtDefeature.Data = MemReadPciCfgEp (socket, ch, DDRT_DEFEATURE_MC_2LM_REG);
    ddrtDefeature.Bits.ignore_erid_parity_error = 0;

    // End Performance Timings
#if (MEM_IP_VER == MEM_IP_TODO)
    // 5330588: EKV sends up interrupt packet with mismatching RID in metadata and MC logs MCA for ERID UC
    // No stepping check, as applies for all SKX steppings
    ddrtDefeature.Data |= BIT17;
    //
    // 5332747: Need to clear imcX_cX_ddrt_err_log_1st after training.
    //
    MemWritePciCfgEp (socket, ch, DDRT_ERR_LOG_1ST_MC_2LM_REG,  0);
    MemWritePciCfgEp (socket, ch, DDRT_ERR_LOG_NEXT_MC_2LM_REG, 0);
#endif  // (MEM_IP_VER == MEM_IP_TODO)

    // HSD5385403 (bios_purley, but confirmed on ICX): Add chicken bit to trigger DDRT error flow using DDRT error pin level instead of edge
#ifndef GNRSRF_HOST
    ddrtDefeature.Bits.ddrt_defeature |= DDRT_RETRY_LEVEL_TRIGGER_EN;
#endif // !GNRSRF_HOST
    MemWritePciCfgEp (socket, ch, DDRT_DEFEATURE_MC_2LM_REG, ddrtDefeature.Data);

    // Program DDRT_DATAPATH_DELAY2.EN_WR_DDRT_MODE.
    DdrtWriteLatency = GetDdrtWriteLatency (socket, ch);
    if (DdrtWriteLatency > DDRT_WRITE_LATENCY_DELAY) {
      InitializeDdrtWrPathDelay (Host, socket, ch, (UINT8) (DdrtWriteLatency - DDRT_WRITE_LATENCY_DELAY));
    } else {
      InitializeDdrtWrPathDelay (Host, socket, ch, DDRT_WRITE_LATENCY_MIN);
    }

    AdjustDdrtStarvationTimeoutAsNeeded (Host, socket, ch);

    //
    // Disable DropNoData for ICX DDRT since unexpected Data Packet is not supported for ICX
    // A0-steping workaround will be applied by pcode, B-stepping and beyond go thru MRC
    //
    if (IsSiliconWorkaroundEnabled ("S1409008977")) {
      DisableDropNoData (Host, socket, ch);
    }

  } // ch loop

  if (Setup->mem.optionsExt & TURNAROUND_OPT_EN_DDRT) {
    OptimizeTurnaroundTimingsDcpmm (socket);
  } else {
    if (!IsSiliconWorkaroundEnabled ("S1707009848")) {
      SetSafeTurnaroundTimingsDcpmm (socket);
    }
  }

  SetWaitDdrtRefIdle (Host, socket);
#endif // DDRT_SUPPORT

  DdrSchedulerConfigLate (socket);

#if (MEM_IP_VER == MEM_IP_TODO)

  //
  // 4929281: SKX Egress credit programming
  // Calculate enabled channels count for MC credit programming,
  // and then calculate the credit count
  //
  for (mcId = 0; mcId < MaxImc; mcId++) {
    EnabledChannelCount[mcId] = 0;
    for (ch = 0; ch < NumChPerMc; ch++) {
      if ((*channelNvList)[ch + (NumChPerMc * mcId)].enabled == 1) {
        EnabledChannelCount[mcId]++;
      }
    }

    GetMcCreditCountEnabledTarget (EnabledChannelCount[mcId], &McCreditCount[mcId]);

  }

  for (mcId = 0; mcId < MaxImc; mcId++) {
    if (Host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
      continue;
    }

    SetM2mWrTrKrAllocPwrThresh (socket, mcId, 11);
  }
#endif  // (MEM_IP_VER == MEM_IP_TODO)

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    rankNum = 0;
    dimmNvList = GetDimmNvList (Host, socket, ch);

    // if DIMM 0 and 1 are not NVMDIMM
    if (((*dimmNvList)[0].DcpmmPresent == 0) && ((*dimmNvList)[1].DcpmmPresent == 0)) {
      for (dimm = 0; dimm < MAX_DIMM; dimm++) {
        for (rank = 0; rank < (*dimmNvList)[dimm].numRanks; rank++) {
          if (CheckRank (Host, socket, ch, dimm, rank, DONT_CHECK_MAPOUT)) {
            continue;
          }
          rankNum++;
        }
      }
      if (rankNum > 4) {
        if (Setup->mem.PpdEn == PPD_ENABLE) {
        } else {
          cke_ll0.Data = MemReadPciCfgEp (socket, ch, CKE_LL0_MCDDC_CTL_REG);
          cke_ll1.Data = MemReadPciCfgEp (socket, ch, CKE_LL1_MCDDC_CTL_REG);
          cke_ll2.Data = MemReadPciCfgEp (socket, ch, CKE_LL2_MCDDC_CTL_REG);
          PkgcCke.Data = MemReadPciCfgEp (socket, ch, PkgcCkeReg);

          cke_ll0.Bits.ppd_en = PPD_DISABLE;
          cke_ll1.Bits.ppd_en = PPD_DISABLE;
          cke_ll2.Bits.ppd_en = PPD_DISABLE;
          PkgcCke.Bits.ppd_en = PPD_DISABLE;

          MemWritePciCfgEp (socket, ch, CKE_LL0_MCDDC_CTL_REG, cke_ll0.Data);
          MemWritePciCfgEp (socket, ch, CKE_LL1_MCDDC_CTL_REG, cke_ll1.Data);
          MemWritePciCfgEp (socket, ch, CKE_LL2_MCDDC_CTL_REG, cke_ll2.Data);
          MemWritePciCfgEp (socket, ch, PkgcCkeReg, PkgcCke.Data);
        }
      }
    }
    //
    // clear the multi credit, disable_max_credit_check for NVMDIMM
    //
    if ((*channelNvList)[ch].ddrtEnabled) {
      dimmNvList = GetDimmNvList (Host, socket, ch);

      SetCpgcDdrtMiscCtl (
        Host,
        socket,
        ch,
        FALSE,
        0,
        FALSE,
        FALSE,
        FALSE
      );

      DdrtPersistentDomainDelayLate (Host, socket, ch);
    }
    //
    // 4929408: PowerOn Safe Settings in the DDRT_MISC_CTL register
    //
    ddrtMiscCtl.Data = MemReadPciCfgEp (socket, ch, DDRT_MISC_CTL_MC_2LM_REG);
    if (Setup->mem.Disddrtopprd == 0) {
      // PO Safe
      ddrtMiscCtl.Bits.dis_ddrt_opp_rd = 1;
      ddrtMiscCtl.Bits.dis_ddrt_opp_ufill_rd = 1;
    } else {
      // POR
      ddrtMiscCtl.Bits.dis_ddrt_opp_rd = 0;
      ddrtMiscCtl.Bits.dis_ddrt_opp_ufill_rd = 0;
    }
    ddrtMiscCtl.Data &= ~BIT30;

    if (UbiosGenerationOrHsleEnabled ()) {
      ddrtMiscCtl.Bits.dis_ddrt_opp_rd = 0;
      ddrtMiscCtl.Bits.dis_ddrt_opp_ufill_rd = 0;
    }
    MemWritePciCfgEp (socket, ch, DDRT_MISC_CTL_MC_2LM_REG, ddrtMiscCtl.Data);
    //
    // 4929410: MC_2LMDDRT_CRNODE_CH0_MC0_CR_DDRT_MCA_CTL.dis_erid_par should be set to 0 before normal mode
    // 4929409: Cloned From SKX Si Bug Eco: DDRT_SCHED_ERROR MCA can incorrectly fire
    //
    ddrtMcaCtl.Data = MemReadPciCfgEp (socket, ch, DDRT_MCA_CTL_MC_2LM_REG);
    ddrtMcaCtl.Bits.dis_erid_par = 0;
    ddrtMcaCtl.Bits.dis_ddrt_scheduler_mismatch = 1;
    ddrtMcaCtl.Bits.dis_pending_intr_error = 1;
    MemWritePciCfgEp (socket, ch, DDRT_MCA_CTL_MC_2LM_REG, ddrtMcaCtl.Data);

    //
    // 4929053  BIOS shoud set reissue_err_ack CSR bit as W/A to finish NVMDIMM Error Flow
    //
#if (MC_IP_VER < MEM_IP_19ww29g)
    ddrtRetryCtl.Data = MemReadPciCfgEp (socket, ch, DDRT_RETRY_CTL_MC_2LM_REG);
    ddrtRetryCtl.Bits.reissue_err_ack = 1;
    ddrtRetryCtl.Bits.force_timer_non_ddrt_mode = 0x48;  // HSD 4929270
    MemWritePciCfgEp (socket, ch, DDRT_RETRY_CTL_MC_2LM_REG, ddrtRetryCtl.Data);
#else
    ddrtRetryCtl.Data = MemReadPciCfgEp (socket, ch, DDRT_RETRY_CTL_N0_MC_2LM_REG);
    ddrtRetryCtl.Bits.reissue_err_ack = 1;
    ddrtRetryCtl.Bits.force_timer_non_ddrt_mode = 0x48;  // HSD 4929270
    MemWritePciCfgEp (socket, ch, DDRT_RETRY_CTL_N0_MC_2LM_REG, ddrtRetryCtl.Data);
#endif

    //
    // HSD 4929270 - NVMDIMM Retry FSM shows slow progress with DDR4 WPQ has transactions during Retry flow
    //
    ddrtRetryTimer2.Data = MemReadPciCfgEp (socket, ch, DDRT_RETRY_TIMER2_MC_2LM_REG);
    ddrtRetryTimer2.Bits.train_reset_time = 0x20;
    MemWritePciCfgEp (socket, ch, DDRT_RETRY_TIMER2_MC_2LM_REG, ddrtRetryTimer2.Data);

    //
    // 4929246  MC_2LMDDRT_CRNODE_CHX_MCY_CR_DDRT_RETRY_TIMER.disable_retry_timer should be set to 0
    //
    ddrtRetryTimer.Data = MemReadPciCfgEp (socket, ch, DDRT_RETRY_TIMER_MC_2LM_REG);
    ddrtRetryTimer.Bits.disable_retry_timer = 0;
    MemWritePciCfgEp (socket, ch, DDRT_RETRY_TIMER_MC_2LM_REG, ddrtRetryTimer.Data);
  } // ch loop

  ModifyMesh2MemCsrsChip (socket);

#ifdef DDRT_SUPPORT
  // HSD 5386509 MRC: BIOS needs to configure 100 usec for ADR timer in PCU
  if (Host->nvram.mem.DcpmmPresent == 1) {
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "Enable ADR surprise reset\n");
    if (EFI_ERROR (PcuSetWarmResetCtrl (socket, PCU_ADR_RST, SurprizRst2Adr_Counter))) {
      RcDebugPrintWithDevice (SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "ERROR: Enabling ADR surprise reset failed\n");
    }
  }
#endif // DDRT_SUPPORT

  //HSD 5332703-EKV S0 IO power optimization requires DDRT training update
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    ddrtIntrCtl.Data = 0;
    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }
      if ((*dimmNvList)[dimm].DcpmmPresent == 0) {
        continue;
      }

      if ((*dimmNvList)[dimm].fmcType == FMC_CWV_TYPE) {
        LateDdrt2Config(Host, socket, ch);
      }

      // 5370437: [CR B0] BIOS needs to enable SMI & ERR pin signaling on DDRT interrupts by programming MC_RRD_CRNODE_CH0_MC0_CR_DDRT_FNV_INTR_CTL register
      if (dimm == 0) {
        // Enable SMI & ERRO signalling for DDRT priority lo & hi interrupt
        ddrtIntrCtl.Bits.slot0_lo_smi_en = 1;
        ddrtIntrCtl.Bits.slot0_hi_smi_en = 1;
        ddrtIntrCtl.Bits.slot0_lo_err0_en = 1;
        ddrtIntrCtl.Bits.slot0_hi_err0_en = 1;
      } else if (dimm == 1) {
        // Enable SMI & ERRO signalling for DDRT priority lo & hi interrupt
        ddrtIntrCtl.Bits.slot1_lo_smi_en = 1;
        ddrtIntrCtl.Bits.slot1_hi_smi_en = 1;
        ddrtIntrCtl.Bits.slot1_lo_err0_en = 1;
        ddrtIntrCtl.Bits.slot1_hi_err0_en = 1;
      }
    } // dimm loop

    SetTxPiOnFmc(Host, socket, ch, 0);

    if (ddrtIntrCtl.Data != 0) {
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "ddrtIntrCtl.Data = %d \n", ddrtIntrCtl.Data);
      MemWritePciCfgEp (socket, ch, DDRT_FNV_INTR_CTL_MCDDC_DP_REG, ddrtIntrCtl.Data);
    }
  }  // ch loop

#ifdef DDRT_SUPPORT
  if ((GetSysBootMode () == NormalBoot && (Host->var.mem.subBootMode == ColdBoot || Host->var.mem.subBootMode == ColdBootFast))
    || (GetSysBootMode () == S3Resume)) {
    FnvDdrtIoInit (Host, socket, DDRT_ENTER_NORMAL_MODE);
  }
#endif // DDRT_SUPPORT

  EnableTsod (socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, socket, ch);
    //
    // Get MCMTR
    //
    mcMtr.Data = MemReadPciCfgEp (socket, ch, MCMTR_MC_MAIN_REG);
    //
    // Set MC XOR mode to match CHA (GlobalDramIntlv.Bits.rt1_interleave_shift)
    //
    McSetXorModeCfg (&mcMtr, GetSocketXorMode (socket), dimmNvList);

    mcMtr.Bits.trng_target = 0;

    //
    // Enter normal mode
    //
    mcMtr.Bits.normal     = 1;
    mcMtr.Bits.trng_mode  = 3;
    //
    // Enable ECC if required
    //
    if ((Host->nvram.mem.eccEn) && IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      mcMtr.Bits.ecc_en = 1;
    }
    MemWritePciCfgEp (socket, ch, MCMTR_MC_MAIN_REG, mcMtr.Data);
  } // ch loop

  //
  // Configure C/A Parity after normal mode is set
  //
  SetupCapWrCrcErrorFlow10nm (Host, socket);

  //
  // Set the corresponding registers to ignore ALERT_n signaling for x16 ECC devices
  //
  IgnoreX16EccAlertSignal (Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    //
    // Clear the CKE override
    //
    OverrideCke (socket, ch, 0, 0);
#if (MEM_IP_VER == MEM_IP_TODO)
    //
    // Initialize per dimm throttling and power related registers
    //
    dimmNvList = &Host->nvram.mem.socket[socket].channelList[ch].dimmList;
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      //
      // Program a constant temperature in the DIMMTEMPSTAT register
      // Expectation is that BIOS programs temperature between temp_mid and temp_hi
      //
      DimmTempStat.Data = MemReadPciCfgEp (socket, ch, DimmTempStatRegOffset[dimm]);
      DimmTempStat.Bits.ev_asrt_temphi = 1;
      DimmTempStat.Bits.ev_asrt_templo = 1;
      DimmTempStat.Bits.ev_asrt_tempmid = 1;
      MemWritePciCfgEp (socket, ch, DimmTempStatRegOffset[dimm], DimmTempStat.Data);
    }
#endif  // (MEM_IP_VER == MEM_IP_TODO)

    // Configure ECC
    if (((Setup->mem.dfxMemSetup.dfxOptions & ECC_CHECK_EN) == 0) || (IsBrsPresent (Host, socket))) {
      // Disable ECC checking if DFX option selected
      Dp_Chkn.Data = MemReadPciCfgEp (socket, ch, MC0_DP_CHKN_BIT_MCDDC_DP_REG);
      Dp_Chkn.Bits.dis_ecc_chk = 1;
      MemWritePciCfgEp (socket, ch, MC0_DP_CHKN_BIT_MCDDC_DP_REG, Dp_Chkn.Data);
    } else {

      // Configure ECC settings
      ProgramEccRetryFailLimit (socket, ch);
    }

  } // ch loop

  IncreaseBlockTimeAsNeeded (Host, socket);

  //
  // Configure power management features
  //
  PowerManagementSetup10nm (Host, socket);

  SetBiosResetCplTarget (Host, socket, (UINT8) SET_CPL_2);  // Set CPL2

  //
  // After SET_CPL_2 is sent to PCU,  iMC and PCU are ready for starting Periodic RCOMP.
  // Note: Do nothing here if Setup->mem.PeriodicRcomp == PERIODIC_RCOMP_AUTO.
  //
  if (Setup->mem.PeriodicRcomp == PERIODIC_RCOMP_ENABLE) {
    PcuEnablePeriodicRcomp (socket, TRUE);
    PcuSetPeriodicRcompInterval (socket, Setup->mem.PeriodicRcompInterval);
  } else if (Setup->mem.PeriodicRcomp == PERIODIC_RCOMP_DISABLE) {
    PcuEnablePeriodicRcomp (socket, FALSE);
  }

  SetSddcCsrs ();

#ifdef DDRT_SUPPORT
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (((*channelNvList)[ch].enabled == 0) || ((*channelNvList)[ch].ddrtEnabled == 0)) {
      continue;
    }
    SetMcaDdrtThermalEvent (socket, ch, FALSE);

    //
    // 1707024887 Toggle Thermal Event in SwitchToNormalMode to clear what MC detected
    //
    SetDefeatureThermalEvent (socket, ch, InNormalMode);

    //
    // 1909299769: Enable MCA Unexpected Pkg CMI Idle when exit CPGC mode
    //
    SetMcaCmiIdleEvent (socket, ch, 0);
  }
#endif

  if (IsSiliconWorkaroundEnabled ("S1409628083") || IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
         continue;
      }
      //
      // Set M2Mem Defeatures0.Wait4BothHalves and MC.AMAP.wait4bothhalvesshould to 1
      //
      SetM2mDefeatures0Wait4BothHalves (socket, ch/MAX_MC_CH, TRUE);
      McSetWaitForBothHalves (socket, ch/MAX_MC_CH, ch%MAX_MC_CH);
    }
  }

  //
  // BIOS needs to enable bug workaround by setting scheduler scratch bit for ICXSP-D0 and ICXSP-M0
  //
  if (IsSiliconWorkaroundEnabled ("S22010000197")) {
    for (ch = 0; ch < MaxChDdr; ch++) {
      SchedulerScratch.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_SCRATCH_MCDDC_CTL_REG);
      SchedulerScratch.Data |= 0x1;
      MemWritePciCfgEp (socket, ch, SCHEDULER_SCRATCH_MCDDC_CTL_REG, SchedulerScratch.Data);
    }
  }

  if (IsSiliconWorkaroundEnabled ("S14010993241")) {
    EfiStatus = PcuMcTimeoutWaDisable (socket);
    if (EFI_ERROR (EfiStatus)) {
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_242);
    }
  }

  if (IsSiliconWorkaroundEnabled ("S14010762787") && (IsBrsPresent (Host, socket) == FALSE)) {
    MaximizeDcpmmReadDrainTimeout (socket);
  }

  return SUCCESS;
} // SwitchToNormalMode

/**

  Wrapper function to enable 2N mode

  @param[in]    Host      SysHost structure pointer

  @retval       SUCCESS   Function executed completely (it does not necessarily
                          mean the mailbox command was sent properly)

**/
UINT32
Enable2nMode (
  IN        PSYSHOST          Host
  )
{
  UINT8         Socket;
  UINT8         McIndex;
  UINT8         ChIndex;
  UINT8         ChOffset;
  UINT8         McBitMap;
  UINT8         ChBitMap;
  UINT8         NumChPerMc;
  struct channelNvram(*ChannelNvList)[MAX_CH];
  EFI_STATUS    Status;
  UINT8         MaxImc;

  MaxImc = GetMaxImc ();
  NumChPerMc = GetNumChannelPerMc ();


  if (!IsSiliconWorkaroundEnabled ("S1909283362")) {
    //
    // If the silicon WA is NOT enabled,
    // we need to skip this function
    //
    return SUCCESS;
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Loop for each CPU socket
    //
    if ((Host->nvram.mem.socket[Socket].enabled == 0) ||
        (Host->nvram.mem.socket[Socket].maxDimmPop == 0)) {
      continue;
    }

    ChannelNvList = GetChannelNvList (Host, Socket);

    for (McIndex = 0; McIndex < MaxImc; McIndex++) {

      McBitMap  = 1 << McIndex;
      ChBitMap  = 0;
      //
      // Set CH bitwise mask
      //
      for (ChIndex = 0; ChIndex < NumChPerMc; ChIndex++) {
        ChOffset = (McIndex * NumChPerMc) + ChIndex;
        if ((*ChannelNvList)[ChOffset].timingMode == TIMING_2N){
          ChBitMap = ChBitMap | (1 << ChIndex);
        }
      } // ChIndex loop

      if (ChBitMap == 0) {
        continue;
      }

      RcDebugPrintWithDevice (
        SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "McBitMap = 0x%02x\nChBitMap = 0x%02x\n",
        McBitMap,
        ChBitMap
        );

      Status = PcuEnable2nMode (Socket, PCU_MEM_DDR4, McBitMap, ChBitMap);

      if (Status == EFI_SUCCESS) {
        RcDebugPrintWithDevice (
          SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "2N Mode enabled successfully\n"
          );
      } else {
        RcDebugPrintWithDevice (
          SDBG_MAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Error enabling 2N Mode\n"
          );
      }

    } // McIndex loop

  } // socket loop


  return SUCCESS;
}

  /**

  Configure power management features

  @param Host    - Pointer to sysHost
  @param socket  - Processor socket

  @retval N/A

**/
VOID
PowerManagementSetup10nm (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                                       ch;
  struct  channelNvram                        (*channelNvList)[MAX_CH];
  SYS_SETUP                                   *Setup;
  UINT8                                       MaxChDdr;

  Setup = GetSysSetupPointer ();

  socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return;
  }

  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr      = GetMaxChDdr ();

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }

     PmSelfRefreshProgramming10nm (Host, socket, ch);
     PmCkeProgramming10nm (Host, socket, ch);
     PmLpModeDataDllOff (Host, socket, ch);
     SetPowerSaveOdt (socket, ch);
  } // ch loop

  if (Setup->mem.OppSrefEn == 0) {
    SetSelectRefreshDdrio(Host, socket);
  }
} // PowerManagementSetup10nm

/**

  Programming required late in the memory initialization process

  @param Host  - Pointer to sysHost

  @retval N/A

**/
UINT32
MemLate (
  PSYSHOST Host
  )
{
  UINT8                        socket;
  UINT8                        mcId;
  SYS_SETUP                    *Setup;
  UINT8                        MaxImc;

  MaxImc = GetMaxImc ();

  Setup = GetSysSetupPointer ();

  if (Setup->mem.options & PAGE_POLICY) {
    for (socket = 0; socket < MAX_SOCKET; socket++) {
      // Continue if this socket is not present
      if (Host->nvram.mem.socket[socket].enabled == 0) {
        continue;
      }

      for (mcId = 0; mcId < MaxImc; mcId++) {
        if (Host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
          continue;
        }

        // Set to closed page mode
        SetPageModeMc(Host, socket, mcId, CLOSED_PAGE_EN);

      } // mcId loop
    } // socket loop
  } // if closed page

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (Host->nvram.mem.socket[socket].enabled == 0) {
      continue;
    }

    for (mcId = 0; mcId < MaxImc; mcId++) {
      if (Host->var.mem.socket[socket].imcEnabled[mcId] == 0) {
        continue;
      }

      SetM2mDefeatures0BadChRetry (socket, mcId, TRUE);

    } // mcId

    SetSbMcaCtl (Host, socket);

  } // socket

  //
  // Indicate data is good
  //
  Host->nvram.mem.DataGood = 1;

  return SUCCESS;
} // MemLate


/**

  Returns the physical rank number

  @param dimm    - DIMM number
  @param rank    - Logical rank number

  @retval N/A

**/
UINT8
GetPhyRank (
  UINT8 dimm,
  UINT8 rank
  )
{
  return (dimm * MAX_RANK_DIMM) + rank;
} // GetPhyRank

#ifdef DDRT_SUPPORT
/**
    Initialize ADR for system with DDRT DIMMs

    @param Host - Pointer to SysHost

    @retval None
**/
VOID
InitAdrForDdrt (
  PSYSHOST  Host
  )
{
  PM_ADR_MCDDC_CTL_STRUCT PmAdrCtrl;
  DIMM_NVRAM_STRUCT       (*DimmNvList)[MAX_DIMM];
  UINT8                   Ch;
  UINT8                   Socket;
  UINT8                   Dimm;
  UINT8                   MaxChDdr;
  SYS_SETUP               *Setup;

  Setup = GetSysSetupPointer ();
  MaxChDdr = GetMaxChDdr ();

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CheckSocketEnabled (Socket) == FALSE) {
      continue;
    }

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

      if ((Setup->mem.ADREn == ADR_ENABLE) && (AepDimmPresent (Socket, Ch, &Dimm) == TRUE)) {

        if (Dimm >= MAX_DIMM) {
          RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_203);
          return;
        }

        RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "[ADR] Enabled DDRT ADR flow\n");

        PmAdrCtrl.Data = MemReadPciCfgEp (Socket, Ch, PM_ADR_MCDDC_CTL_REG);

        DimmNvList = GetDimmNvList (Host, Socket, Ch);
        if ((*DimmNvList)[Dimm].fmcType == FMC_CWV_TYPE) {
          //
          // DDRT-2 (0x2 -> reset value)
          //
          PmAdrCtrl.Bits.num_pwr_fail_cmds = 2;
        } else {
          if (IsCpuAndRevision (CPU_SPRSP, REV_ALL) == TRUE) {
            //
            // SPR DDRT (0x3 -> reset value)
            //
            PmAdrCtrl.Bits.num_pwr_fail_cmds = 3;
          } else {
            //
            // others DDRT (0x1 -> reset value)
            //
            PmAdrCtrl.Bits.num_pwr_fail_cmds = 1;
          }
        }
        PmAdrCtrl.Bits.ddrt_adr_en = 1;
        MemWritePciCfgEp (Socket, Ch, PM_ADR_MCDDC_CTL_REG, PmAdrCtrl.Data);

        Host->var.mem.AdrStateForPmemModule[Socket][Ch] = (BOOLEAN)(PmAdrCtrl.Bits.ddrt_adr_en);
      }
    }
  }
}
#endif

/**
    Initialize ADR PCH Timer

    @param Host - Pointer to SysHost

    @retval None
**/
VOID
InitAdrTimer (
  PSYSHOST  Host
  )
{
  UINT8                 Ch;
  UINT8                 Socket;
  UINT8                 MaxChDdr;
  UINT8                 EnabledSocketsNum;
  BOOLEAN               FpgaSocketPresent;
  UINT32                AdrTimerValue;
#ifdef DDRT_SUPPORT
  UINT32                DdrtAdrTimerValues[] = {PCH_ADR_TIMER_1000US,  // 1 socket enabled
                                                PCH_ADR_TIMER_1800US,  // 2 sockets enabled
                                                PCH_ADR_TIMER_3600US,  // 3 sockets enabled
                                                PCH_ADR_TIMER_3600US,  // 4 sockets enabled
                                                PCH_ADR_TIMER_7200US,  // 5 sockets enabled
                                                PCH_ADR_TIMER_7200US,  // 6 sockets enabled
                                                PCH_ADR_TIMER_7200US,  // 6 sockets enabled
                                                PCH_ADR_TIMER_7200US}; // 7 sockets enabled
  UINT32                DdrtAdrTimerFpgaValues[] = {PCH_ADR_TIMER_1200US,  // 1 socket enabled
                                                    PCH_ADR_TIMER_2200US}; // 2 sockets enabled
  BOOLEAN               DdrtDimmPresent;
#endif
  UINT32                NvdimmNAdrTimerValues[] = {PCH_ADR_TIMER_400US,   // 1 socket enabled
                                                   PCH_ADR_TIMER_600US,   // 2 sockets enabled
                                                   PCH_ADR_TIMER_800US,   // 3 sockets enabled
                                                   PCH_ADR_TIMER_800US,   // 4 sockets enabled
                                                   PCH_ADR_TIMER_1800US,  // 5 sockets enabled
                                                   PCH_ADR_TIMER_1800US,  // 6 sockets enabled
                                                   PCH_ADR_TIMER_1800US,  // 6 sockets enabled
                                                   PCH_ADR_TIMER_1800US}; // 7 sockets enabled
  UINT32                NvdimmNAdrTimerFpgaValues[] = {PCH_ADR_TIMER_400US,  // 1 socket enabled
                                                        PCH_ADR_TIMER_600US}; // 2 sockets enabled
  BOOLEAN               NvdimmNDimmPresent;
  SYS_SETUP             *Setup;

#ifdef DDRT_SUPPORT
  DdrtDimmPresent = FALSE;
#endif
  NvdimmNDimmPresent = FALSE;
  Setup = GetSysSetupPointer ();
  MaxChDdr = GetMaxChDdr ();
  EnabledSocketsNum = 0;
  FpgaSocketPresent = FALSE;

  //
  // Legacy ADR mode (100us for all DIMM types and socket configurations)
  //
  if (Setup->mem.LegacyADRModeEn) {
    RcDebugPrint (SDBG_MAX, "[ADR] PCH ADR timer = %d us (legacy mode)\n", PCH_ADR_TIMER_100US);
    PchAdrConfigureForDdrt (PCH_ADR_TIMER_100US);
    return;
  }

  //
  // Non-legacy ADR mode
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    if (CheckSocketEnabled (Socket) == FALSE) {
      continue;
    }
    EnabledSocketsNum++;

    if (IfFpgaActive (Socket) == TRUE) {
      FpgaSocketPresent = TRUE;
    }

    for (Ch = 0; Ch < MaxChDdr; Ch++) {
      if (IsChannelEnabled (Socket, Ch) == FALSE) {
        continue;
      }

#ifdef DDRT_SUPPORT
      if (AepDimmPresent (Socket, Ch, NULL) == TRUE) {
        DdrtDimmPresent = TRUE;
      }
#endif
      if ((PcdGetBool (PcdNvDimmEn) == TRUE) && (NvdimmNPresent (Socket, Ch, NULL) == TRUE)) {
        NvdimmNDimmPresent = TRUE;
      }
    }
  }

  if (EnabledSocketsNum == 0) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_204);
    return;
  }

#ifdef DDRT_SUPPORT
  if (DdrtDimmPresent) {

    if (NvdimmNDimmPresent) {
      RcDebugPrint (SDBG_MAX, "[ADR] DDRT DIMMs mixed with NVDIMM-N DIMMs - not supported ADR configuration - use DDRT DIMMs ADR Timer values\n");
    }

    if (FpgaSocketPresent) {
      if (NELEMENTS (DdrtAdrTimerFpgaValues) <= (EnabledSocketsNum - 1)) {
        RcDebugPrint (SDBG_MAX, "[ADR] Number of enabled FPGA sockets (%d) not supported - setting max value for PCH ADR timer (%d us)\n",
          EnabledSocketsNum, PCH_ADR_TIMER_9600US);
        PchAdrConfigureForDdrt (PCH_ADR_TIMER_9600US);
        return;
      }
    } else {
      if (NELEMENTS (DdrtAdrTimerValues) <= (EnabledSocketsNum - 1)) {
        RcDebugPrint (SDBG_MAX, "[ADR] Number of enabled sockets (%d) not supported - setting max value for PCH ADR timer (%d us)\n",
          EnabledSocketsNum, PCH_ADR_TIMER_9600US);
        PchAdrConfigureForDdrt (PCH_ADR_TIMER_9600US);
        return;
      }
    }

    RcDebugPrint (SDBG_MAX, "[ADR] Reconfigure PCH ADR timer and sources for DDRT\n");
    if (FpgaSocketPresent) {
      AdrTimerValue = DdrtAdrTimerFpgaValues[EnabledSocketsNum - 1];
    } else {
      AdrTimerValue = DdrtAdrTimerValues[EnabledSocketsNum - 1];
    }
    RcDebugPrint (SDBG_MAX, "[ADR] PCH ADR timer = %d us\n", AdrTimerValue);
    PchAdrConfigureForDdrt (AdrTimerValue);
    return;
  }
#endif

  if (NvdimmNDimmPresent) {
    RcDebugPrint (SDBG_MAX, "[ADR] Reconfigure PCH ADR timer and sources for NVDIMM-N\n");

    if (FpgaSocketPresent) {
      if (NELEMENTS (NvdimmNAdrTimerFpgaValues) <= (EnabledSocketsNum - 1)) {
        RcDebugPrint (SDBG_MAX, "[ADR] Number of enabled FPGA sockets (%d) not supported - setting max value for PCH ADR timer (%d us)\n",
          EnabledSocketsNum, PCH_ADR_TIMER_9600US);
        PchAdrConfigureForDdrt (PCH_ADR_TIMER_9600US);
        return;
      }
    } else {
      if (NELEMENTS (NvdimmNAdrTimerValues) <= (EnabledSocketsNum - 1)) {
        RcDebugPrint (SDBG_MAX, "[ADR] Number of enabled sockets (%d) not supported - setting max value for PCH ADR timer (%d us)\n",
          EnabledSocketsNum, PCH_ADR_TIMER_9600US);
        PchAdrConfigureForDdrt (PCH_ADR_TIMER_9600US);
        return;
      }
    }

    if (FpgaSocketPresent) {
      AdrTimerValue = NvdimmNAdrTimerFpgaValues[EnabledSocketsNum - 1];
    } else {
      AdrTimerValue = NvdimmNAdrTimerValues[EnabledSocketsNum - 1];
    }
    RcDebugPrint (SDBG_MAX, "[ADR] PCH ADR timer = %d us\n", AdrTimerValue);
    PchAdrConfigureForNvdimm (AdrTimerValue);
  }
}

/**
    Initialize PCU config for ADR

    @param None

    @retval None
**/
VOID
InitAdrPcuConfig (
  VOID
  )
{
  PCU_BIOS_SPARE2_PCU_FUN5_STRUCT PcuBiosSpare2Reg;
  SYS_SETUP                       *Setup;
  UINT8                           Socket;

  Setup = GetSysSetupPointer ();

  if (Setup->mem.ADREn == ADR_ENABLE) {
    //
    // Configure PCU for ADR
    //
    if (Setup->mem.LegacyADRModeEn == LEGACY_ADR_MODE_DISABLE) {
      for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
        if (CheckSocketEnabled (Socket)) {
          PcuBiosSpare2Reg.Data = UsraCsrRead (Socket, 0, PCU_BIOS_SPARE2_PCU_FUN5_REG);
          PcuBiosSpare2Reg.Data |= ADR_TYPE_PCODE_ASSISTED;
          PcuBiosSpare2Reg.Data |= ADR_IO_CACHE_FLUSH_ENABLED;
          UsraCsrWrite (Socket, 0, PCU_BIOS_SPARE2_PCU_FUN5_REG, PcuBiosSpare2Reg.Data);
        }
      }
    }
  }
}

/**

    Initialize ADR

    @param Host - Pointer to SysHost

    @retval SUCCESS

**/
UINT32
InitADR (
  PSYSHOST  Host
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  //
  // Clear ADR_RST_STS on all boots regardless of if ADR is enabled
  // or not to make sure ADR_COMPLETE works correct.
  //
  if (!GetSysResetRequired()) {
    RcDebugPrint (SDBG_MAX, "[ADR] No Pending Reset, clearing the ADR status bit\n");
    PchAdrClearAdrResetStatus ();
  }

  if (Setup->mem.ADREn == ADR_DISABLE) {
    return SUCCESS;
  }

#ifdef DDRT_SUPPORT
  InitAdrForDdrt (Host);
#endif // DDRT_SUPPORT
  InitAdrTimer (Host);
  InitAdrPcuConfig ();

  RcDebugPrint (SDBG_MAX, (Setup->mem.LegacyADRModeEn) ? "[ADR] ADR Enabled (legacy mode)\n" : "[ADR] ADR Enabled (non-legacy mode)\n");
  return SUCCESS;
}


/**

  Returns the memory controller ID

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param ch      - Channel number

  @retval MC ID

**/
UINT8
GetMCCh (
  UINT8     mcId,
  UINT8     ch
  )
{
  UINT8 NumChPerMc;
  NumChPerMc = GetNumChannelPerMc ();
  return (ch % NumChPerMc);
} // GetMCCh

/**

  Returns the memory controller ID

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param ch      - Channel number

  @retval MC ID

**/
VOID
GetMcChInstance (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     *mcId,
  UINT8     *mcCh
  )
{
  UINT8 NumChPerMc;
  NumChPerMc = GetNumChannelPerMc ();
  *mcId = Host->var.mem.socket[socket].channelList[ch].mcId;
  *mcCh = ch % NumChPerMc;
} // GetMcChInstance

/**

    Check to see if Rank for each socket is populated

    @param Host - Pointer to SysHost

    @retval N/A

**/
VOID
CheckRankPop (
  PSYSHOST  Host
  )
{
  UINT8 socket;

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    CheckRankPopLocal (Host, socket);

  } // socket loop
} // CheckRankPop


/**

    Check to see if Rank for specified socket is populated

    @param Host   - Pointer to SysHost
    @param socket - Current socket

    @retval N/A

**/
VOID
CheckRankPopLocal (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8                           ch;
  UINT8                           rank;
  UINT8                           dimm;
  UINT32                          dimmAmap;
  UINT8                           chRankEnabled;
  struct channelNvram             (*channelNvList)[MAX_CH];
  struct dimmNvram                (*dimmNvList)[MAX_DIMM];
  DIMMMTR_0_MC_MAIN_STRUCT        dimmMtr;
  AMAP_MC_MAIN_STRUCT             imcAMAPReg;
  UINT32                          DimmMtrRegOffset;
  UINT8                           MaxChDdr;

  if (Host->nvram.mem.socket[socket].enabled == 0) {
    return;
  }

  if (IsHvmModeEnable ()) {
    return;
  }

  channelNvList = GetChannelNvList (Host, socket);

  MaxChDdr = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if (!IsChannelValid (Host, ch)) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, socket, ch);
    dimmAmap = 0;
    chRankEnabled = 0;

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      for (rank = 0; rank < (*dimmNvList)[dimm].numDramRanks; rank++) {

        if (((*dimmNvList)[dimm].mapOut[rank] == 0) || (((*channelNvList)[ch].mb3trainingfailure == 0) && ((*channelNvList)[ch].ddrtEnabled == 1))) {
          chRankEnabled++;
        }
      } // rank loop
    } // dimm loop

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {

      DimmMtrRegOffset = GetDimmMtrRegOffset (dimm);

      if (chRankEnabled == 0) {
        dimmMtr.Data = MemReadPciCfgEp (socket, ch, DimmMtrRegOffset);
      } else {
        dimmMtr.Data = (*dimmNvList)[dimm].dimmMemTech;
      }

      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (((*dimmNvList)[dimm].dimmPresent == 0) || (rank >= (*dimmNvList)[dimm].numDramRanks) || (chRankEnabled == 0)) {
          dimmMtr.Bits.rank_disable |= 1 << rank;
        }

        if (((*dimmNvList)[dimm].dimmPresent) && (*dimmNvList)[dimm].mapOut[rank] == 1){
          // Disable Patrol Scrub on mapped out ranks
          dimmAmap |= (1 << (dimm * 8 + rank));

          // Update AMAP register
          imcAMAPReg.Data = MemReadPciCfgEp (socket, ch, AMAP_MC_MAIN_REG);
          imcAMAPReg.Bits.dimm0_pat_rank_disable |= (UINT8)(dimmAmap & 0xff);
          imcAMAPReg.Bits.dimm1_pat_rank_disable |= (UINT8)((dimmAmap >> 8) & 0xff);
          MemWritePciCfgEp (socket, ch, AMAP_MC_MAIN_REG,imcAMAPReg.Data);
        }
      } //rank loop

      (*dimmNvList)[dimm].dimmMemTech = dimmMtr.Data;
      MemWritePciCfgEp (socket, ch, DimmMtrRegOffset, (UINT32) dimmMtr.Data);

      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "dimmMtr: 0x%08x\n", (*dimmNvList)[dimm].dimmMemTech);
    } //dimm loop

    if (chRankEnabled == 0) {
      Host->nvram.mem.socket[socket].channelList[ch].enabled = 0;
    }
  } // ch loop
} // CheckRankPopLocal

/**

  Get the QCLK Ratio at CPU Startup

  @param[in] Socket   - Current Socket
  @param[out] BaseClockType  - Base frequency for the Qclk Multiplier  <BR>
                         0h     MC base clock of 133 Mhz <BR>
                         1h     MC base clock 100 Mhz <BR>
                         All other values are reserved.
  @param[out] QclkMultiplier  - Product of this value with the bus clock results in the Qclk frequency             <BR>
                        Examples of some encodings of this field with a 133 MHz multiplier for QCLK:               <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq   <BR>
                        000110b    6d         3 * 133.33          400.00 MHz       6  * 133.33 MHz      800.00 MHz  <BR>
                        001000b    8d         4 * 133.33          533.33 MHz       8  * 133.33 MHz      1066.67 MHz <BR>
                        001010b    10d        5 * 133.33          666.67 MHz       10 * 133.33 MHz      1333.33 MHz <BR>
                        001100b    12d        6 * 133.33          800.00 MHz       12 * 133.33 MHz      1600.00 MHz <BR>
                        001110b    14d        7 * 133.33          933.33 MHz       14 * 133.33 MHz      1866.67 MHz <BR>
                        010000b    16d        8 * 133.33          1066.67 MHz      16 * 133.33 MHz      2133.33 MHz <BR>

                        Examples of some encodings of this field with a 100 MHz multiplier for QCLK:               <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq   <BR>
                        100000b    32d        16 * 100            1600MHz          32 * 100 MHz         3200.00 MHz <BR>
                        100100b    36d        18 * 100            1800MHz          36 * 100 MHz         3600.00 MHz <BR>
                        101000b    40d        20 * 100            2000MHz          40 * 100 MHz         4000.00 MHz <BR>
                        101100b    44d        22 * 100            2200MHz          44 * 100 MHz         4400.00 MHz <BR>
                        110000b    48d        24 * 100            2400MHz          48 * 100 MHz         4800.00 MHz <BR>

  @retval EFI_SUCCESS   Settings were retrieved successfully.
  @retval !EFI_SUCCESS  Settings were not retrieved successfully.

**/
EFI_STATUS
EFIAPI
GetStartupQclkRatio (
  IN  UINT8       Socket,
  OUT UINT8       *BaseClockType,
  OUT UINT8       *QclkMultiplier
  )
{
  //
  // For 10nm products, the MC is not enabled at CPU start, so the ratio is 0.
  //
  *BaseClockType = 0;
  *QclkMultiplier = 0;

  return EFI_SUCCESS;
}

/**
  Get the QCLK Ratio.

  @param Host       - Pointer to sysHost, the system Host (root) structure
  @param socket     - Current Socket
  @param RefClkType - Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_type <BR>
                        0h     MC frequency request for 133 Mhz              <BR>
                        1h     MC frequency request for 100 Mhz              <BR>
                        All other values are reserved.
  @param QClkRatio  - Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_data <BR>
                        The data for the request.                            <BR>
                        The encoding of this field is the 133 MHz or 100MHz multiplier for QCLK: <BR>

                        Examples of some encodings of this field with a 133 MHz multiplier for QCLK:               <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq   <BR>
                        000110b    6d         3 * 133.33          400.00 MHz       6  * 133.33 MHz      800.00 MHz  <BR>
                        001000b    8d         4 * 133.33          533.33 MHz       8  * 133.33 MHz      1066.67 MHz <BR>
                        001010b    10d        5 * 133.33          666.67 MHz       10 * 133.33 MHz      1333.33 MHz <BR>
                        001100b    12d        6 * 133.33          800.00 MHz       12 * 133.33 MHz      1600.00 MHz <BR>
                        001110b    14d        7 * 133.33          933.33 MHz       14 * 133.33 MHz      1866.67 MHz <BR>
                        010000b    16d        8 * 133.33          1066.67 MHz      16 * 133.33 MHz      2133.33 MHz <BR>

                        Examples of some encodings of this field with a 100 MHz multiplier for QCLK:               <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq   <BR>
                        100000b    32d        16 * 100            1600MHz          32 * 100 MHz         3200.00 MHz <BR>
                        100100b    36d        18 * 100            1800MHz          36 * 100 MHz         3600.00 MHz <BR>
                        101000b    40d        20 * 100            2000MHz          40 * 100 MHz         4000.00 MHz <BR>
                        101100b    44d        22 * 100            2200MHz          44 * 100 MHz         4400.00 MHz <BR>
                        110000b    48d        24 * 100            2400MHz          48 * 100 MHz         4800.00 MHz <BR>

    @retval None
**/
VOID
EFIAPI
GetQclkRatio (
  IN   PSYSHOST  Host,
  IN   UINT8     socket,
  OUT  UINT8    *RefClkType,
  OUT  UINT8    *QClkRatio
  )
{
  PCU_MEM_TYPE  MemType;
  UINT8         MaxImc;
  UINT8         McId;
  UINT8         MaxFreq;

  *RefClkType = 0;
  *QClkRatio = 0;

  if (IsHvmModeEnable ()) {
    return;
  }

  MemType = PCU_MEM_DDR4;
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    MemType = PCU_MEM_DDR5;
  }

  MaxImc = GetMaxImc ();

  for (McId = 0; McId < MaxImc; McId++) {
    if (Host->var.mem.socket[socket].ImcPresent[McId]) {
      break;
    }
  }

  if (McId >= MaxImc) {
    McId = 0;
  }

  //
  // Read the current MC freq via mailbox command.
  //
  if (!EFI_ERROR (PcuReadMcFreq (socket, MemType, (BIT0 << McId), RefClkType, QClkRatio, &MaxFreq))) {
    RcDebugPrint (SDBG_MAX,
                   "\n Current QCLK Ratio: %d; RefClkType = %d\n", *QClkRatio, *RefClkType);
  } else {
    DisplayPcodeMailboxReadMcFreqErrorTarget (socket);
  }
}

/** Set memory controller frequency, handling any errors

  @param[in]  Host          Pointer to the SysHost structure
  @param[in]  Socket        0 based Socket number
  @param[in]  RefClk        Reference Clock type
  @param[in]  QclkRatio     Qclk Ratio
  @param[in]  MemType       Memory Type (HBM, DDR4, DDR5)
  @param[in]  MemSsEnabled  Memory Subsystem enable mask
  @param[in]  RampBitEnable Ramp Bit Enable, if its last call to set MC

  @retval N/A

**/
VOID
SetMCFreqAndHandleErrors (
  IN  PSYSHOST      Host,
  IN  UINT8         Socket,
  IN  UINT8         RefClk,
  IN  UINT8         QCLKRatio,
  IN  PCU_MEM_TYPE  MemType,
  IN  UINT8         MemSsEnabled,
  IN  BOOLEAN       RampBitEnable
  )
{
  MC_BIOS_REQ_PCU_FUN1_STRUCT McBiosReq;
  EFI_STATUS                  Status;

  Status = PcuSetMCFreq (Socket, RefClk, QCLKRatio, MemType, MemSsEnabled, RampBitEnable);

  if (!EFI_ERROR (Status)) {
    McBiosReq.Data = UsraCsrRead (Socket, 0, MC_BIOS_REQ_PCU_FUN1_REG);
    McBiosReq.Bits.req_data = QCLKRatio; // The encoding of this field is BCLK multiplier for QCLK.
    McBiosReq.Bits.req_type = RefClk;
    UsraCsrWrite (Socket, 0, MC_BIOS_REQ_PCU_FUN1_REG, McBiosReq.Data);
    RcDebugPrintWithDevice (SDBG_MAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "PCODE mailbox to Set MC Frequency succeeded!\n");
  } else if (Status == EFI_UNSUPPORTED) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "PCODE mailbox to Set MC Frequency failed!\n");
    OutputError (ERR_SET_MC_FREQ, ERR_UNSUPPORTED_MC_FREQ, Socket, NO_CH, NO_DIMM, NO_RANK);
  } else {
    DisplayPcodeMailboxSetMcFreqErrorTarget (Host, Socket, RefClk, QCLKRatio);
    OutputError (ERR_SET_MC_FREQ, ERR_UNSPECIFIED_MC_FREQ_SETTING_ERROR, Socket, NO_CH, NO_DIMM, NO_RANK);
  }
}

/**
  Set the DDR QCLK Ratio.

  @param Host       - Pointer to sysHost, the system Host (root) structure
  @param Socket     - Current Socket
  @param RefClkType - Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_type <BR>
                        0h     MC frequency request for 133 Mhz <BR>
                        1h     MC frequency request for 100 Mhz <BR>
                        All other values are reserved.
  @param QClkRatio  - Settings based on MC_BIOS_REQ_PCU_FUN1_STRUCT req_data <BR>
                        The data for the request. <BR>
                        The encoding of this field is the 133 MHz or 100MHz multiplier for  QCLK: <BR>

                        Examples of some encodings of this field with a 133 MHz multiplier for QCLK:                <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq    <BR>
                        000110b    6d         3 * 133.33          400.00 MHz       6  * 133.33 MHz      800.00 MHz  <BR>
                        001000b    8d         4 * 133.33          533.33 MHz       8  * 133.33 MHz      1066.67 MHz <BR>
                        001010b    10d        5 * 133.33          666.67 MHz       10 * 133.33 MHz      1333.33 MHz <BR>
                        001100b    12d        6 * 133.33          800.00 MHz       12 * 133.33 MHz      1600.00 MHz <BR>
                        001110b    14d        7 * 133.33          933.33 MHz       14 * 133.33 MHz      1866.67 MHz <BR>
                        010000b    16d        8 * 133.33          1066.67 MHz      16 * 133.33 MHz      2133.33 MHz <BR>

                        Examples of some encodings of this field with a 100 MHz multiplier for QCLK:                <BR>
                        Binary     Dec       DCLK Equation      DCLK Freq         QCLK Equation        QCLK Freq    <BR>
                        100000b    32d        16 * 100            1600MHz          32 * 100 MHz         3200.00 MHz <BR>
                        100100b    36d        18 * 100            1800MHz          36 * 100 MHz         3600.00 MHz <BR>
                        101000b    40d        20 * 100            2000MHz          40 * 100 MHz         4000.00 MHz <BR>
                        101100b    44d        22 * 100            2200MHz          44 * 100 MHz         4400.00 MHz <BR>
                        110000b    48d        24 * 100            2400MHz          48 * 100 MHz         4800.00 MHz <BR>
                        ...
    @param FrequencyChangeNeeded - Indicate if frequency change is required.

    @retval None
**/
VOID
EFIAPI
SetQclkRatio (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     RefClkType,
  IN UINT8     QClkRatio,
  IN BOOLEAN   FrequencyChangeNeeded
  )
{
  UINT8        ImcBitMap;
  UINT8        Index = 0;
  PCU_MEM_TYPE MemType;
  UINT8        MaxImc;

  MaxImc = GetMaxImc ();

  //
  // Update the MaxImc which was MAX_IMC regardless the real MC count (aka die count) in headless MRC build.
  // This might cause headless MRC problem when sending mailbox command to not existing MC. In SPR the MaxImc
  // can be overridden by the RcSim.ini thru capid3.
  //

  //
  // Exit if this routine for CTE
  //
  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    RcDebugPrintWithDevice (SDBG_MINMAX, Socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "Set MC Frequency skipped!\n");
    return;
  }

  MemType = PCU_MEM_DDR4;

  ImcBitMap = 0;
  //Set MC Instance Mask
  for (Index = 0; Index < MaxImc; Index++) {
    if (Host->var.mem.socket[Socket].ImcPresent[Index]) {
      ImcBitMap |= (BIT0 << Index);
    }
  }
  //
  // Change req_type information for 10nm
  //
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5) {
    MemType = PCU_MEM_DDR5;
  }

  SetMCFreqAndHandleErrors (Host, Socket, RefClkType, QClkRatio, MemType, ImcBitMap, TRUE);

} // SetQclkRatio

/** Perform any DDR frequency settings necessary for cases where a socket has no DIMMs.

  @param[in]  Socket  A socket without any DIMMs populated.

  @retval EFI_SUCCESS   Settings were performed successfully.
  @retval !EFI_SUCCESS  Settings were not performed successfully.

**/
EFI_STATUS
EFIAPI
SetDDRFreqNoDimms (
  IN  UINT8 Socket
  )
{
  PSYSHOST  Host;

  // Skip this function for CTE
  if (FeaturePcdGet (PcdCteBuild)) {
    return EFI_SUCCESS;
  }

  Host = GetSysHostPointer ();

  SetMCFreqAndHandleErrors (
    Host,
    Socket,
    MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MC_REF_CLK_TYPE_NO_DIMMS,
    MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MC_RATIO_NO_DIMMS,
    PCU_MEM_NONE,
    MAILBOX_BIOS_CMD_SET_MC_FREQ_DATA_MC_INSTANCE_NONE,
    TRUE
    );

  return EFI_SUCCESS;
}

UINT32
IsRankDisabled (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm,
  UINT16    rank
  )
{
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
  DIMMMTR_0_MC_MAIN_STRUCT      dimmMtr;

  dimmNvList    = GetDimmNvList(Host, socket, ch);
  dimmMtr.Data = (*dimmNvList)[dimm].dimmMemTech;

  // Skip ranks that are disabled
  if (dimmMtr.Bits.rank_disable & (1 << rank)) {
    return 1;
  }
  return 0;
}

VOID
SetDimmPop (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     dimm
  )
{
  DIMMMTR_0_MC_MAIN_STRUCT dimmMtr;
  UINT32                   DimmMtrRegOffset;

  DimmMtrRegOffset = GetDimmMtrRegOffset (dimm);

  //
  // Disable DIMM so no maintenance operations go to it
  //
  dimmMtr.Data = MemReadPciCfgEp (socket, ch, DimmMtrRegOffset);
  dimmMtr.Bits.dimm_pop = 1;
  MemWritePciCfgEp (socket, ch, DimmMtrRegOffset, dimmMtr.Data);
}

/**

    display processor abbreviation, stepping and socket

    @param[in] Host   - Pointer to sysHost, the system Host (root) structure
    @param[in] Socket - Processor socket ID

    @retval None

**/
VOID
DimmInfoDisplayProc (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  BOOLEAN    StepUknown = TRUE;

  //
  // Display processor abbreviation, stepping and socket
  //
  if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "SNR");
  } else if (IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "TNR");
  } else if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "ICX-D");
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "ICX");
  } else if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    RcDebugPrint (SDBG_DEFAULT, "SPR");
    if (IsCpuAndRevision (CPU_SPRSP, REV_A0)) {
      RcDebugPrint (SDBG_DEFAULT, " A0");
      StepUknown = FALSE;
    } else if (IsCpuAndRevision (CPU_SPRSP, REV_B0)) {
      RcDebugPrint (SDBG_DEFAULT, " B0");
      StepUknown = FALSE;
    }
  } else {
    RcDebugPrint (SDBG_DEFAULT, "?");
  }

  if (StepUknown == TRUE) {
    RcDebugPrint (SDBG_DEFAULT, " (Stepping Unknown)");
  }

  PrintPorSocketConfigDimmInfo ();
}


VOID
DimmInfoDisplayProcFeatures (
  PSYSHOST  Host
  )
{
#ifdef DEBUG_CODE_BLOCK
  UINT8                                 socket;
  UINT8                                 ch;
  struct channelNvram                   (*channelNvList)[MAX_CH];
  MCMTR_MC_MAIN_STRUCT                  mcMtr;
  SCHEDULER_IDLETIME2_MCDDC_CTL_STRUCT  IdleTime2;
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT      scrambleConfig;
  SCRUBCTL_MC_MAIN_STRUCT               imcSCRUBCTL;
  UINT8                                 MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  IdleTime2.Data = 0;
  scrambleConfig.Data = 0;
  imcSCRUBCTL.Data = 0;
  mcMtr.Data = 0;

  for (socket = 0; socket < MAX_SOCKET; socket++) {

    if (IsSocketDimmPopulated (Host, socket) != TRUE) {
      continue;
    }

    channelNvList = GetChannelNvList(Host, socket);

    for (ch = 0; ch < MaxChDdr; ch++) {
      if ((*channelNvList)[ch].enabled == 0) {
        continue;
      }
      IdleTime2.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_IDLETIME2_MCDDC_CTL_REG);
      scrambleConfig.Data = MemReadPciCfgEp (socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG);
      // Assume ecc_en symmetry between channels and MCs
      mcMtr.Data = MemReadPciCfgEp (socket, ch, MCMTR_MC_MAIN_REG);
      imcSCRUBCTL.Data = MemReadPciCfgEp (socket, ch, SCRUBCTL_MC_MAIN_REG);
      break;
    } // ch loop

    break;
  } // socket loop

  // ECC Support
  RcDebugPrint (SDBG_DEFAULT, "ECC Checking");
  RcDebugPrintEmptyBlock (SDBG_DEFAULT, 8);
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    RcDebugPrintEmptyBlock (SDBG_DEFAULT, 12);
  }
  if (Host->nvram.mem.eccEn) {
    RcDebugPrint (SDBG_DEFAULT, "     On     |\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "     Off    |\n");
  }

  // Scrubbing
  RcDebugPrint (SDBG_DEFAULT, "Patrol/Demand Scrub");
  RcDebugPrintEmptyBlock (SDBG_DEFAULT, 1);
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    RcDebugPrintEmptyBlock (SDBG_DEFAULT, 12);
  }
  if (imcSCRUBCTL.Bits.scrub_en) {
    RcDebugPrint (SDBG_DEFAULT, "     On     |\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "     Off    |\n");
  }

  // RAS Mode
  RcDebugPrint (SDBG_DEFAULT, "RAS Mode");
  RcDebugPrintEmptyBlock (SDBG_DEFAULT, 12);
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    RcDebugPrintEmptyBlock (SDBG_DEFAULT, 12);
  }
  if (Host->nvram.mem.RASmode == 0) {
    RcDebugPrint (SDBG_DEFAULT, "   Indep    |\n");
  } else if (Host->nvram.mem.RASmode == FULL_MIRROR_1LM) {
    RcDebugPrint (SDBG_DEFAULT, "   1LM Mirror   |\n");
  } else if (Host->nvram.mem.RASmode == FULL_MIRROR_2LM) {
      RcDebugPrint (SDBG_DEFAULT, "  2LM Mirror   |\n");
  } else if (Host->nvram.mem.RASmode == STAT_VIRT_LOCKSTEP) {
    RcDebugPrint (SDBG_DEFAULT, "  Lockstep  |\n");
  } else if (Host->nvram.mem.RASmode == RK_SPARE) {
    RcDebugPrint (SDBG_DEFAULT, " Rank Spare |\n");
  } else if (Host->nvram.mem.RASmode == ADDDC_EN) {
    RcDebugPrint (SDBG_DEFAULT, " ADDDC Spare |\n");
  } else if (Host->nvram.mem.RASmode == SDDC_EN) {
    RcDebugPrint (SDBG_DEFAULT, " SDDC Spare |\n");
  } else if (Host->nvram.mem.RASmode == CH_SL) {
    RcDebugPrint (SDBG_DEFAULT, "    SP/LK   |\n");
  } else if (Host->nvram.mem.RASmode == CH_MS) {
    RcDebugPrint (SDBG_DEFAULT, "    MR/SP   |\n");
  }

  // Sparing Per Channel

  // Paging Policy
  RcDebugPrint (SDBG_DEFAULT, "Paging Policy");
  RcDebugPrintEmptyBlock (SDBG_DEFAULT, 7);
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    RcDebugPrintEmptyBlock (SDBG_DEFAULT, 12);
  }
  if (mcMtr.Bits.close_pg) {
    RcDebugPrint (SDBG_DEFAULT, "   Closed   |\n");
  } else if (IdleTime2.Bits.adapt_pg_clse) {
    RcDebugPrint (SDBG_DEFAULT, " Adapt Open |\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "    Open    |\n");
  }

  // Scrambling
  RcDebugPrint (SDBG_DEFAULT, "Data Scrambling");
  RcDebugPrintEmptyBlock (SDBG_DEFAULT, 5);
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    RcDebugPrintEmptyBlock (SDBG_DEFAULT, 12);
  }
  if (scrambleConfig.Bits.rx_enable) {
    RcDebugPrint (SDBG_DEFAULT, "     On     |\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "     Off    |\n");
  }

  // NUMA
#endif // DEBUG_CODE_BLOCK
}

/**

    Set tWR for this channel

    @param Host   - Pointer to sysHost, the system Host (root) structure
    @param socket - Current socket
    @param ch     - Current channel
    @param nWR    - nWR value
    @param *PrevWR    - Previous WR value

    @retval None

**/
VOID
SetChTwr (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch,
  UINT8     nWR,
  UINT8    *PrevWR
)
{
  struct channelNvram (*channelNvList)[MAX_CH];
  UINT32  Delta;
  UINT32  tWRPRE_Local;
  UINT8   BL = 8;                   // Burst length
  MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_STRUCT MemoryTimingsBankTrdaTwraTwrpre;

  channelNvList = GetChannelNvList (Host, socket);

  Delta = (*channelNvList)[ch].common.nWL + BL / 2;
  tWRPRE_Local = nWR + Delta;
  CheckBoundsChip (MemTechDdr, socket, ch, tWRPRE, &tWRPRE_Local);

  //
  // The API is to set the Write Recovery time, but in 10nm design, the write recovery time is replaced
  // with the write CAS to Precharge Delay time.  The GettWRPRE core routine uses the common write recovery
  // time for calculating tWrPre, so setting the input value to the common structure
  //

  MemoryTimingsBankTrdaTwraTwrpre.Data = MemReadPciCfgEp (socket, ch, MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_REG);
  *PrevWR = (UINT8) (MemoryTimingsBankTrdaTwraTwrpre.Bits.t_wrpre - Delta);
  MemoryTimingsBankTrdaTwraTwrpre.Bits.t_wrpre = tWRPRE_Local;
  MemWritePciCfgEp (socket, ch, MEMORY_TIMINGS_BANK_TRDA_TWRA_TWRPRE_MCDDC_CTL_REG, MemoryTimingsBankTrdaTwraTwrpre.Data);
} // SetChTwr


/**
  Returns CMD timing mode (TIMING_1N, TIMING_2N or TIMING_3N) currently
  programmed in HW

  @param Host     - Pointer to sysHost structure
  @param socket   - Socket number
  @param ch       - Channel number

  @retval   - TIMING_1N, TIMING_2N or TIMING_3N
**/
UINT8
GetCurrentTimingMode (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch
  )
{
  UINT8                                   cmd_stretch = TIMING_1N;
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT     memTimingsAdj;

  memTimingsAdj.Data = MemReadPciCfgEp (socket, ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
  cmd_stretch = (UINT8)memTimingsAdj.Bits.cmd_stretch;
  return cmd_stretch;
} // GetCurrentTimingMode

/**
  Set the command timing

  @param[in] Host       - Pointer to SysHost
  @param[in] Socket     - Socket number
  @param[in] Ch         - Channel number
  @param[in] CmdTiming  - Command timing (TIMING_1N, TIMING_2N, TIMING_3N)

  @retval EFI_SUCCESS if timing is successfully set
  !EFI_SUCCESS otherwise
**/
EFI_STATUS
EFIAPI
SetTimingMode (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket,
  IN    UINT8     Ch,
  IN    UINT8     CmdTiming
  )
{
  MEMORY_TIMINGS_ADJ_MCDDC_CTL_STRUCT         MemTimingsAdj;
  struct channelNvram                         (*ChannelNvList)[MAX_CH];

  ChannelNvList = GetChannelNvList (Host, Socket);

  MemTimingsAdj.Data = MemReadPciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG);
  MemTimingsAdj.Bits.cmd_stretch = CmdTiming;
  MemWritePciCfgEp (Socket, Ch, MEMORY_TIMINGS_ADJ_MCDDC_CTL_REG, MemTimingsAdj.Data);
  (*ChannelNvList)[Ch].MemoryTimingsAdj = MemTimingsAdj.Data;

  CteConfiguration (Host, Socket, Ch, CTE_CONFIG_UPDATE_TIMING_MODE, CmdTiming);

  return EFI_SUCCESS;
} // SetTimingMode

VOID
SetEncodedCsMode (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch
  )
{
  UINT8                dimm;
  struct channelNvram  (*channelNvList)[MAX_CH];
  struct dimmNvram     (*dimmNvList)[MAX_DIMM];

  channelNvList = GetChannelNvList (Host, socket);
  dimmNvList    = GetDimmNvList (Host, socket, ch);

  if ((*channelNvList)[ch].encodedCSMode) {
    dimmNvList = GetDimmNvList (Host, socket, ch);

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].dimmPresent == 0) {
        continue;
      }

      MemWritePciCfgEp (socket, ch, GetDimmMtrRegOffset (dimm), (UINT32) (*dimmNvList)[dimm].dimmMemTech);
    } //dimm
  } // encodedCSMode
} //SetEncodedCsMode

VOID
SetConfigBeforeRmt (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  UINT8               ch;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  //
  //Set RfOn = 0 before RMT per sighting #4987510
  //
  for (ch = 0; ch < MaxChDdr; ch++) {
    SetRfOn (Host, socket, ch, 0);
  } // ch loop
}


VOID
PmSelfRefreshProgramming10nm (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch
  )
{
  UINT8                             McId;
  UINT8                             ChId;
  UINT8                             tempMdllValue;
  UINT8                             tempCkModeValue;
  UINT8                             NumChPerMc;
  UINT32                            tempSrefTimer = 0;
  struct channelNvram               (*ChannelNvList)[MAX_CH];
  SREF_LL0_MCDDC_CTL_STRUCT         sref_ll0;
  SREF_LL1_MCDDC_CTL_STRUCT         sref_ll1;
  SREF_LL2_MCDDC_CTL_STRUCT         sref_ll2;
  SREF_LL3_MCDDC_CTL_STRUCT         sref_ll3;
  SREF_PKGC_MCDDC_CTL_STRUCT        Sref_Pkgc;
  SREF_STATIC_MCDDC_CTL_STRUCT      sref_static;
  MCMNT_CHKN_BIT2_MCDDC_CTL_STRUCT  mcmnt2_chkn_bit2;
  SYS_SETUP                         *Setup;

  Setup = GetSysSetupPointer ();
  NumChPerMc = GetNumChannelPerMc ();

  ChannelNvList = GetChannelNvList (Host, socket);

  McId = ch / NumChPerMc;
  ChId = ch % NumChPerMc; //Channel Id per MC

  mcmnt2_chkn_bit2.Data = MemReadPciCfgEp (socket, ch, MCMNT_CHKN_BIT2_MCDDC_CTL_REG);

  sref_ll0.Data = MemReadPciCfgEp (socket, ch, SREF_LL0_MCDDC_CTL_REG);
  sref_ll1.Data = MemReadPciCfgEp (socket, ch, SREF_LL1_MCDDC_CTL_REG);
  sref_ll2.Data = MemReadPciCfgEp (socket, ch, SREF_LL2_MCDDC_CTL_REG);
  sref_ll0.Data &= ~(BIT27 | BIT26 | BIT25 | BIT24 | BIT22 | BIT20 | 0xfffff);
  sref_ll1.Data &= ~(BIT27 | BIT26 | BIT25 | BIT24 | BIT22 | BIT20 | 0xfffff);
  sref_ll2.Data &= ~(BIT27 | BIT26 | BIT25 | BIT24 | BIT22 | BIT20 | 0xfffff);
  sref_ll3.Data = MemReadPciCfgEp (socket, ch, SREF_LL3_MCDDC_CTL_REG);
  sref_ll3.Data &= ~(BIT27 | BIT26 | BIT25 | BIT24 | BIT22 | BIT20 | 0xfffff);
  Sref_Pkgc.Data = MemReadPciCfgEp (socket, ch, SREF_PKGC_MCDDC_CTL_REG);
  Sref_Pkgc.Data &= ~(BIT27 | BIT26 | BIT25 | BIT24 | BIT22 | BIT20 | 0xfffff);

  tempCkModeValue = CK_SELF_REF_LOW;

  if (Setup->mem.SrefProgramming == SREF_PROG_MANUAL) {
    if (Setup->mem.CkMode == CK_SELF_REF_LOW) {
      mcmnt2_chkn_bit2.Bits.dis_cke_pri_rcb_gate = 1;
      mcmnt2_chkn_bit2.Bits.dis_ref_pri_rcb_gate = 1;
    }

    //
    // Self Refresh idle timer = tRFC + tZQCS + 100
    //
    tempSrefTimer = (*ChannelNvList)[ch].common.nRFC + tZQCS_DDR4 + 100;

    sref_ll0.Data |= ((2 << 26) | (tempCkModeValue << 24) | (Setup->mem.MdllOffEn <<22) | (Setup->mem.OppSrefEn <<20) | tempSrefTimer);
    sref_ll1.Data |= ((2 << 26) | (tempCkModeValue << 24) | (Setup->mem.MdllOffEn <<22) | (Setup->mem.OppSrefEn <<20) | tempSrefTimer);
    sref_ll2.Data |= ((2 << 26) | (tempCkModeValue << 24) | (Setup->mem.MdllOffEn <<22) | (Setup->mem.OppSrefEn <<20) | tempSrefTimer);
    sref_ll3.Data |= ((2 << 26) | (tempCkModeValue << 24) | (Setup->mem.MdllOffEn <<22) | (Setup->mem.OppSrefEn <<20) | tempSrefTimer);
    Sref_Pkgc.Data |= ((1 << 26) | (tempCkModeValue << 24) | (Setup->mem.MdllOffEn <<22) | (Setup->mem.OppSrefEn <<20) | tempSrefTimer);
  } else { //Use Auto values from table.
    if (Host->nvram.mem.dimmTypePresent == UDIMM) {
      //if udimm mdlloffen will be 0.
      tempMdllValue = 0;
    } else { // CLK STOP ENABLED
      tempMdllValue = 1;
      mcmnt2_chkn_bit2.Bits.dis_cke_pri_rcb_gate = 1;
      mcmnt2_chkn_bit2.Bits.dis_ref_pri_rcb_gate = 1;
    }

    //
    // Self Refresh idle timer = tRFC + tZQCS + 100
    //
    tempSrefTimer = (*ChannelNvList)[ch].common.nRFC + tZQCS_DDR4 + 100;

    sref_ll0.Data |= ((2 << 26) | (tempCkModeValue << 24) | (tempMdllValue << 22) | (SREF_LL0_AUTO[1] << 20) | (SREF_LL0_AUTO[0]));
    sref_ll1.Data |= ((2 << 26) | (tempCkModeValue << 24) | (tempMdllValue << 22) | (SREF_LL1_AUTO[1] << 20) | (SREF_LL1_AUTO[0]));
    sref_ll2.Data |= ((2 << 26) | (tempCkModeValue << 24) | (tempMdllValue << 22) | (SREF_LL2_AUTO[1] << 20) | (SREF_LL2_AUTO[0]));
    sref_ll3.Data |= ((SREF_LL3_AUTO[4]  << 26) | (tempCkModeValue << 24) | (tempMdllValue << 22) | (SREF_LL3_AUTO[1] << 20) | tempSrefTimer);
    Sref_Pkgc.Data |= ((1 << 26) | (tempCkModeValue << 24) | (tempMdllValue << 22) | (SREF_PKGC_AUTO[1] << 20) | tempSrefTimer);

  }//end of Auto Values

  if (Setup->mem.OppSrefEn == 0) {
    sref_ll0.Bits.opp_sref_en = 0;
    sref_ll1.Bits.opp_sref_en = 0;
    sref_ll2.Bits.opp_sref_en = 0;
    sref_ll3.Bits.opp_sref_en = 0;
    Sref_Pkgc.Bits.opp_sref_en = 0;
  }

  // Write the register value into the Register
  MemWritePciCfgEp (socket, ch, SREF_LL0_MCDDC_CTL_REG, sref_ll0.Data);
  MemWritePciCfgEp (socket, ch, SREF_LL1_MCDDC_CTL_REG, sref_ll1.Data);
  MemWritePciCfgEp (socket, ch, SREF_LL2_MCDDC_CTL_REG, sref_ll2.Data);
  MemWritePciCfgEp (socket, ch, MCMNT_CHKN_BIT2_MCDDC_CTL_REG, mcmnt2_chkn_bit2.Data);

  MemWritePciCfgEp (socket, ch, SREF_PKGC_MCDDC_CTL_REG, Sref_Pkgc.Data);
  MemWritePciCfgEp (socket, ch, SREF_LL3_MCDDC_CTL_REG, sref_ll3.Data);
  RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "opp_sref_en: %d\n", sref_ll3.Bits.opp_sref_en);

  sref_static.Data = MemReadPciCfgEp (socket, ch, SREF_STATIC_MCDDC_CTL_REG);
  sref_static.Data &= ~BIT30;

  if (Setup->mem.PkgcSrefEn == 1) {
    sref_static.Data |= BIT30; //set PkgcSrefEn. This is default unless otherwise changed in setup.
  }

  MemWritePciCfgEp (socket, ch, SREF_STATIC_MCDDC_CTL_REG, sref_static.Data);

} // PmSelfRefreshProgramming10nm

/**
  Program CKE Power Down feature

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Ch      - Memory channel number

  @retval  N/A
*/
VOID
PmCkeProgramming10nm (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  CKE_LL0_MCDDC_CTL_STRUCT  CkeLl0;
  CKE_LL1_MCDDC_CTL_STRUCT  CkeLl1;
  CKE_LL2_MCDDC_CTL_STRUCT  CkeLl2;
  PKGC_CKE_MCDDC_CTL_STRUCT PkgcCke;
  UINT32                    PkgcCkeReg = PKGC_CKE_MCDDC_CTL_REG;
  SYS_SETUP                 *Setup;
  UINT8                     CkeIdleTimer;

  Setup = GetSysSetupPointer ();

  CkeLl0.Data  = MemReadPciCfgEp (Socket, Ch, CKE_LL0_MCDDC_CTL_REG);
  CkeLl1.Data  = MemReadPciCfgEp (Socket, Ch, CKE_LL1_MCDDC_CTL_REG);
  CkeLl2.Data  = MemReadPciCfgEp (Socket, Ch, CKE_LL2_MCDDC_CTL_REG);
  PkgcCke.Data = MemReadPciCfgEp (Socket, Ch, PkgcCkeReg);

  if (Setup->mem.CkeProgramming == CKE_MANUAL) { // Manual
    //
    // Both APD and PPD cannot be set to 1. Warning will be issued in set up.
    // One of the bits will be forced to 0.
    //
    if ((Setup->mem.ApdEn == APD_ENABLE) && (Setup->mem.PpdEn == PPD_ENABLE)) {
      Setup->mem.PpdEn = PPD_DISABLE;
    }

    //
    // Get the CSR-appropriate value.
    //
    CkeIdleTimer = GetCkeIdleTimer (Host, Socket, Setup->mem.CkeIdleTimer);

    CkeLl0.Bits.ddrt_cke_en     = Setup->mem.DdrtCkeEn;
    CkeLl1.Bits.ddrt_cke_en     = Setup->mem.DdrtCkeEn;
    CkeLl2.Bits.ddrt_cke_en     = Setup->mem.DdrtCkeEn;
    PkgcCke.Bits.ddrt_cke_en    = Setup->mem.DdrtCkeEn;

    CkeLl0.Bits.ppd_en          = Setup->mem.PpdEn;
    CkeLl1.Bits.ppd_en          = Setup->mem.PpdEn;
    CkeLl2.Bits.ppd_en          = Setup->mem.PpdEn;
    PkgcCke.Bits.ppd_en         = Setup->mem.PpdEn;

    CkeLl0.Bits.apd_en          = Setup->mem.ApdEn;
    CkeLl1.Bits.apd_en          = Setup->mem.ApdEn;
    CkeLl2.Bits.apd_en          = Setup->mem.ApdEn;
    PkgcCke.Bits.apd_en         = Setup->mem.ApdEn;

    CkeLl0.Bits.cke_idle_timer  = CkeIdleTimer;
    CkeLl1.Bits.cke_idle_timer  = CkeIdleTimer;
    CkeLl2.Bits.cke_idle_timer  = CkeIdleTimer;
    PkgcCke.Bits.cke_idle_timer = CkeIdleTimer;

    RcDebugPrint (SDBG_MAX, "SRI CkeIdleTimer = %d\n", CkeIdleTimer);

  } else { // Auto

    CkeLl0.Bits.ddrt_cke_en     = CKE_LL0_AUTO[3];
    CkeLl1.Bits.ddrt_cke_en     = CKE_LL1_AUTO[3];
    CkeLl2.Bits.ddrt_cke_en     = CKE_LL2_AUTO[3];
    PkgcCke.Bits.ddrt_cke_en    = PKGC_CKE_AUTO[3];

    CkeLl0.Bits.ppd_en          = CKE_LL0_AUTO[2];
    CkeLl1.Bits.ppd_en          = CKE_LL1_AUTO[2];
    CkeLl2.Bits.ppd_en          = CKE_LL2_AUTO[2];
    PkgcCke.Bits.ppd_en         = PKGC_CKE_AUTO[2];

    CkeLl0.Bits.apd_en          = CKE_LL0_AUTO[1];
    CkeLl1.Bits.apd_en          = CKE_LL1_AUTO[1];
    CkeLl2.Bits.apd_en          = CKE_LL2_AUTO[1];
    PkgcCke.Bits.apd_en         = PKGC_CKE_AUTO[1];

    CkeLl0.Bits.cke_idle_timer  = CKE_LL0_AUTO[0];
    CkeLl1.Bits.cke_idle_timer  = CKE_LL1_AUTO[0];
    CkeLl2.Bits.cke_idle_timer  = CKE_LL2_AUTO[0];
    PkgcCke.Bits.cke_idle_timer = PKGC_CKE_AUTO[0];
  }

  RcDebugPrint (SDBG_MAX, "SRI CkeLl0.Data  = 0x%x\n", CkeLl0.Data);
  RcDebugPrint (SDBG_MAX, "SRI CkeLl1.Data  = 0x%x\n", CkeLl1.Data);
  RcDebugPrint (SDBG_MAX, "SRI CkeLl2.Data  = 0x%x\n", CkeLl2.Data);
  RcDebugPrint (SDBG_MAX, "SRI PkgcCke.Data = 0x%x\n", PkgcCke.Data);

  MemWritePciCfgEp (Socket, Ch, CKE_LL0_MCDDC_CTL_REG, CkeLl0.Data);
  MemWritePciCfgEp (Socket, Ch, CKE_LL1_MCDDC_CTL_REG, CkeLl1.Data);
  MemWritePciCfgEp (Socket, Ch, CKE_LL2_MCDDC_CTL_REG, CkeLl2.Data);
  MemWritePciCfgEp (Socket, Ch, PkgcCkeReg, PkgcCke.Data);
}

/**

  For certain DCLK to UCLK settings we need to add bubbles

  @param Host    - Pointer to sysHost
  @param socket  - Socket to switch

**/
VOID
ProgramBgfTable (
  PSYSHOST  Host,
  UINT8     socket
  )
{
  return;
} //ProgramBgfTable

/**
  Determine whether or not DIMM isolation is available to be enabled.

  @retval FALSE   No attempt should be made to enable DIMM isolation.
  @retval TRUE    DIMM isolation can be enabled.
**/
BOOLEAN
IsDimmIsolationAvailable (
  VOID
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (((Setup->mem.optionsExt & DIMM_ISOLATION_EN) == 0) ||
      IsSiliconWorkaroundEnabled ("S22010658199"))
  {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**

  Finds the Maximum Round Trip Latency for a channel based on
  existing per rank round trip latency values

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Current Socket
  @param[in] Ch      - Current Channel

  @retVal  Maximum round trip latency

**/
UINT8
GetMaxRoundTripLatency (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch
  )
{
  UINT8                                         Dimm;
  UINT8                                         Rank;
  UINT8                                         CurrentRoundTripLatency;
  UINT8                                         MaxRoundTripLatency;
  struct dimmNvram                              (*DimmNvList)[MAX_DIMM];

  MaxRoundTripLatency = 0;
  DimmNvList = GetDimmNvList (Host, Socket, Ch);
  for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
    if (IsDimmPresent (Socket, Ch, Dimm) == FALSE) {
      continue;
    }
    for (Rank = 0; Rank < GetNumberOfRanksOnDimm (Socket, Ch, Dimm); Rank ++) {
      if (CheckRank (Host, Socket, Ch, Dimm, Rank, DONT_CHECK_MAPOUT)) {
        continue;
      }
      GetRoundTrip (Socket, Ch, GetLogicalRank (Host, Socket, Ch, Dimm, Rank), GSM_READ_CSR, &CurrentRoundTripLatency);

      if (MaxRoundTripLatency < CurrentRoundTripLatency) {
        MaxRoundTripLatency = CurrentRoundTripLatency;
      }
    } // Rank
  } // Dimm

  return MaxRoundTripLatency;
}

// CRC_ALERT is defined in the JEDEC SDRAM Spec, max value is 13ns == 13000ps
#define MAX_CRC_ALERT      13000

// ALERT_HL is defined in the JEDEC SDRAM Spec, value is 1.5ns = 1500ps
#define ALERT_HL           1500

/**

  Setup Command Address Parity and Write CRC error flows

  @param[in] Host    - Pointer to sysHost
  @param[in] socket  - Socket to switch

**/
VOID
SetupCapWrCrcErrorFlow10nm (
  IN PSYSHOST  Host,
  IN UINT8     socket
  )
{
  UINT8                                         ch;
  UINT8                                         IntermediateLatencyValue;
  UINT8                                         MaxRoundTripLatency;
  UINT8                                         writeErrorLatency;
  struct channelNvram                           (*channelNvList)[MAX_CH];
  struct dimmNvram                              (*dimmNvList)[MAX_DIMM];
  ERF_DDR4_CMD_REG0_MCDDC_CTL_STRUCT            erfDdr4CmdReg0;
  ERF_DDR4_CMD_REG1_MCDDC_CTL_STRUCT            erfDdr4CmdReg1;
  ERF_DDR4_CMD_REG2_MCDDC_CTL_STRUCT            erfDdr4CmdReg2;
  ERF_DDR4_CMD_REG3_MCDDC_CTL_STRUCT            erfDdr4CmdReg3;
  ERF_DDR4_CMD_REG4_MCDDC_CTL_STRUCT            erfDdr4CmdReg4;
  DDR4_CA_CTL_MCDDC_DP_STRUCT                   Ddr4CaCtl;
  SCHEDULER_DELAY_MCDDC_CTL_STRUCT              schedulerdelaydata;
  SYS_SETUP                                     *Setup;
  UINT8                                         MaxChDdr;
#ifdef DDR5_SUPPORT
  UINT8                                         Index;
  UINT32                                        RegAddress;
#endif

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  channelNvList = GetChannelNvList(Host, socket);

  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    schedulerdelaydata.Data = MemReadPciCfgEp (socket, ch , SCHEDULER_DELAY_MCDDC_CTL_REG);

    if (((Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN) == 0) && ((Setup->mem.optionsExt & WR_CRC) == 0)) {
      writeErrorLatency = (*channelNvList)[ch].common.nWL / 2;
    } else {

      MaxRoundTripLatency = GetMaxRoundTripLatency (Host, socket, ch);
#ifdef DDR5_SUPPORT
      if ((Setup->mem.optionsExt & WR_CRC) == WR_CRC) {

        // Default burst length for DDR5 is 16, but WR CRC adds 2 cycles at the end of the burst for the CRC data

        IntermediateLatencyValue = (*channelNvList)[ch].common.nWL + ((BURST_LENGTH + 2) / 2) + MaxRoundTripLatency - (*channelNvList)[ch].common.nCL + (UINT8)PicosecondToClocks (socket, MAX_CRC_ALERT) + 1;
      } else {
        IntermediateLatencyValue = MaxRoundTripLatency - (*channelNvList)[ch].common.nCL + 4 + (UINT8)PicosecondToClocks (socket, ALERT_HL) + 1;
      }
      writeErrorLatency = (IntermediateLatencyValue / 2) - 2 + 8;
#else
      IntermediateLatencyValue = (*channelNvList)[ch].common.nWL + 5 + (((MaxRoundTripLatency + 1) / 2) - (*channelNvList)[ch].common.nCL) + (UINT8)((CRC_ALERT_DDR4/(Host->nvram.mem.socket[socket].QCLKps * 2) + 1));
      writeErrorLatency = (IntermediateLatencyValue / 2) - 2;
#endif
    }
    if (IsCpuAndRevision (CPU_GNRSP, REV_ALL)) {
      writeErrorLatency = writeErrorLatency / 2;
    }

    schedulerdelaydata.Bits.write_err_latency = writeErrorLatency;
    MemWritePciCfgEp (socket, ch , SCHEDULER_DELAY_MCDDC_CTL_REG, schedulerdelaydata.Data);

    ConfigDqsCapErrorWaitTime (Host, socket, ch);

    dimmNvList = GetDimmNvList (Host, socket, ch);
    //
    // Enable or Disable the Error Flow depending on setup options and if cmdClkTrainingDone is set.
    //
    if (!(((*channelNvList)[ch].ddrtEnabled == 1) && ((*dimmNvList)[0].DcpmmPresent == 1))) {
      Ddr4CaCtl.Data = MemReadPciCfgEp (socket, ch, DDR4_CA_CTL_MCDDC_DP_REG);
      Ddr4CaCtl.Bits.erf_en0 = 1;  // enable error flow
      if (!(UbiosGenerationOrHsleEnabled ())) {
        if (!((Host->nvram.mem.socket[socket].cmdClkTrainingDone) && ((Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN) || (Setup->mem.optionsExt & WR_CRC)))) {
          Ddr4CaCtl.Bits.erf_en0 = 0;
        }
      }
      MemWritePciCfgEp (socket, ch, DDR4_CA_CTL_MCDDC_DP_REG, Ddr4CaCtl.Data);
    }

    //
    //if dimm isolation is disabled or parity and Write CRC are disabled, program default settings for REG2
    // note that the values in the ERF_DDR4_CMD_REG settings correspond to DDR4 commands in the JEDEC specs
    //
    if (((~Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN) && (~Setup->mem.optionsExt & WR_CRC)) || (IsDimmIsolationAvailable () == FALSE)) {

      if ((IsDdr5Present (Host, socket) == TRUE) && (IsBrsPresent (Host, socket) == FALSE) && (IsDimmIsolationAvailable () == FALSE)) {
#ifdef DDR5_SUPPORT
        //
        // For DDR5, when DIMM isolation is disabled, clear all ERF_DDR4_CMD_REG.
        //
        for (Index = 0; Index < GetErfDdr4CmdRegNum (); Index++) {
          RegAddress = GetErfDdr4CmdRegAddress (Index);
          MemWritePciCfgEp (socket, ch, RegAddress, 0x00000000);
        }
#endif // DDR5_SUPPORT
      } else {
        erfDdr4CmdReg2.Data              = 0;
        //
        // F0RC06 - command space control word
        // setting 6 - Clear CA Parity Error
        //
        erfDdr4CmdReg2.Bits.ad           = (RDIMM_RC06 << 4) | 0x6;
        //
        //BG0, BA1:BA0 MR select, 111= RCW
        //
        erfDdr4CmdReg2.Bits.ba           = (BANK7 & (BIT0 | BIT1));
        erfDdr4CmdReg2.Bits.bg           = ((BANK7 & BIT2) >> 2);
        //
        // RAS#/CAS#/WE# 0/0/0 -> MRS command
        //
        erfDdr4CmdReg2.Bits.rcw          = 0;
        //
        // Allow address invert must be 0 for MR7 (RCW) commands
        //
        erfDdr4CmdReg2.Bits.allow_invert = 0;
        erfDdr4CmdReg2.Bits.rdimm        = 1;
        erfDdr4CmdReg2.Bits.en           = 1;

        MemWritePciCfgEp(socket, ch, ERF_DDR4_CMD_REG2_MCDDC_CTL_REG, erfDdr4CmdReg2.Data);
      }

    // something is enabled
    } else {
      //
      // Reg 0 setup - enter serial MPR mode
      //
      erfDdr4CmdReg0.Data              = 0;
      //
      // set A2 to change MPR operation to dataflow from/to MPR
      // clear A12:A11 for MPR read serial mode
      //
      erfDdr4CmdReg0.Bits.ad           = MR3_MPR;
      //
      //BG0, BA1:BA0 MR select, 011= MR3
      //
      erfDdr4CmdReg0.Bits.ba           = BANK3;
      erfDdr4CmdReg0.Bits.bg           = 0;
      //
      // RAS#/CAS#/WE# 0/0/0 -> MRS command
      //
      erfDdr4CmdReg0.Bits.rcw          = 0;
      //
      // Allow address invert must be 1 for MR0-6 commands
      //
      erfDdr4CmdReg0.Bits.allow_invert = 1;
      erfDdr4CmdReg0.Bits.rdimm        = 1;
      // Only enable if CA parity and DIMM isolation are enabled and available
      if ((Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN) && IsDimmIsolationAvailable ()) {
        erfDdr4CmdReg0.Bits.en         = 1;
      }
      MemWritePciCfgEp (socket, ch, ERF_DDR4_CMD_REG0_MCDDC_CTL_REG, erfDdr4CmdReg0.Data);
      //
      // Reg 1 setup - transfer error log to DRAM
      //
      erfDdr4CmdReg1.Data              = 0;
      //
      // F0RC06 - command space control word
      // setting 4 - CW Read Operation
      //
      erfDdr4CmdReg1.Bits.ad           = (RDIMM_RC06 << 4) | 0x4;
      //
      //BG0, BA1:BA0 MR select, 111= RCW
      //
      erfDdr4CmdReg1.Bits.ba           = (BANK7 & (BIT0 | BIT1));
      erfDdr4CmdReg1.Bits.bg           = ((BANK7 & BIT2) >> 2);
      //
      // RAS#/CAS#/WE# 0/0/0 -> MRS command
      //
      erfDdr4CmdReg1.Bits.rcw          = 0;
      //
      // Allow address invert must be 0 for MR7 (RCW) commands
      //
      erfDdr4CmdReg1.Bits.allow_invert = 0;
      erfDdr4CmdReg1.Bits.rdimm        = 1;
      if ((Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN) && IsDimmIsolationAvailable ()) {
        erfDdr4CmdReg1.Bits.en         = 1;
      }
      MemWritePciCfgEp (socket, ch, ERF_DDR4_CMD_REG1_MCDDC_CTL_REG, erfDdr4CmdReg1.Data);
      //
      // Reg 2 setup - CAS read to get page 0 data
      //
      erfDdr4CmdReg2.Data              = 0;
      //
      // A12 & A10 set, BL8 read
      //
      erfDdr4CmdReg2.Bits.ad           = 0x1400;
      erfDdr4CmdReg2.Bits.ba           = 0;
      erfDdr4CmdReg2.Bits.bg           = 0;
      //
      // RAS#/CAS#/WE# 1/0/1 -> read command
      //
      erfDdr4CmdReg2.Bits.rcw          = 5;
      erfDdr4CmdReg2.Bits.allow_invert = 0;
      erfDdr4CmdReg2.Bits.rdimm        = 1;
      if ((Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN) && IsDimmIsolationAvailable ()) {
        erfDdr4CmdReg2.Bits.en         = 1;
      }
      MemWritePciCfgEp (socket, ch, ERF_DDR4_CMD_REG2_MCDDC_CTL_REG, erfDdr4CmdReg2.Data);

      // Reg 3 setup - clear CAP error logs
      erfDdr4CmdReg3.Data              = 0;
      //
      // F0RC06 - command space control word
      // setting 6 - Clear CA Parity Error
      //
      erfDdr4CmdReg3.Bits.ad           = (RDIMM_RC06 << 4) | 0x6;
      //
      //BG0, BA1:BA0 MR select, 111= RCW
      //
      erfDdr4CmdReg3.Bits.ba           = (BANK7 & (BIT0 | BIT1));
      erfDdr4CmdReg3.Bits.bg           = ((BANK7 & BIT2) >> 2);
      //
      // RAS#/CAS#/WE# 0/0/0 -> MRS command
      //
      erfDdr4CmdReg3.Bits.rcw          = 0;
      //
      // Allow address invert must be 0 for MR7 (RCW) commands
      //
      erfDdr4CmdReg3.Bits.allow_invert = 0;
      erfDdr4CmdReg3.Bits.rdimm        = 1;
      if ((Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN) && IsDimmIsolationAvailable ()) {
        erfDdr4CmdReg3.Bits.en         = 1;
      }
      MemWritePciCfgEp (socket, ch, ERF_DDR4_CMD_REG3_MCDDC_CTL_REG, erfDdr4CmdReg3.Data);

      // Reg 4 setup - exit serial MPR mode
      erfDdr4CmdReg4.Data              = 0;
      erfDdr4CmdReg4.Bits.ad           = 0;
      //
      //BG0, BA1:BA0 MR select, 011= MR3
      //
      erfDdr4CmdReg4.Bits.ba           = BANK3;
      erfDdr4CmdReg4.Bits.bg           = 0;
      //
      // RAS#/CAS#/WE# 0/0/0 -> MRS command
      //
      erfDdr4CmdReg4.Bits.rcw          = 0;
      //
      // Allow address invert must be 1 for MR0-6 commands
      //
      erfDdr4CmdReg4.Bits.allow_invert = 1;
      erfDdr4CmdReg4.Bits.rdimm        = 1;
      if ((Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN) && IsDimmIsolationAvailable ()) {
        erfDdr4CmdReg4.Bits.en         = 1;
      }
      MemWritePciCfgEp (socket, ch, ERF_DDR4_CMD_REG4_MCDDC_CTL_REG, erfDdr4CmdReg4.Data);
    }
  } // ch
} //SetupCapWrCrcErrorFlow10nm

/**
  Program the delay between ERID and RdValid for DDR4-only channels
  The delay is calculated based on cmd_program_delay.
  cmd_program_delay programming must be completed before calling this routine.

  @param[in] Host     - Pointer to SysHost
  @param[in] Socket   - Socket number

  @retval   N/A
**/
VOID
ProgramErid2DataValidDdr4 (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket
  )
{
  UINT8                                     Ch;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  DDRT_DATAPATH_DELAY_MCDDC_DP_STRUCT       DdrtDatapathDelay;
  CMPL_TO_DATA_DELAY_MCDDC_DP_STRUCT        CmplDataDelay;
  UINT8                                     MaxChDdr;

  MaxChDdr      = GetMaxChDdr ();
  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    if ((*ChannelNvList)[Ch].ddrtEnabled == 1) {
      continue;
    }
    CmplDataDelay.Data        = MemReadPciCfgEp (Socket, Ch, CMPL_TO_DATA_DELAY_MCDDC_DP_REG);
    DdrtDatapathDelay.Data    = MemReadPciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG);

    //
    // cmpl_program_delay does not support values of 6 or 7 for DDR4-only configs.
    //
    if (CmplDataDelay.Bits.cmpl_program_delay <= CMPL_PROGRAM_DELAY_DDR4_LIMIT_LO) {
      DdrtDatapathDelay.Bits.erid_to_rdvalid_delay = CmplDataDelay.Bits.cmpl_program_delay + CMPL_PROGRAM_DELAY_DDR4_ADDER_LO;
    } else if (CmplDataDelay.Bits.cmpl_program_delay >= CMPL_PROGRAM_DELAY_DDR4_BASE_HI) {
      DdrtDatapathDelay.Bits.erid_to_rdvalid_delay = CmplDataDelay.Bits.cmpl_program_delay + CMPL_PROGRAM_DELAY_DDR4_ADDER_HI;
    } else {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
        "INVALID cmpl_program_delay found = %d\n", CmplDataDelay.Bits.cmpl_program_delay);
      RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_151);
    }

    RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "erid_to_rdvalid_delay = %d\n", DdrtDatapathDelay.Bits.erid_to_rdvalid_delay);

    MemWritePciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG, DdrtDatapathDelay.Data);
  } // Ch loop
}

/**
Program the delay for erid_to_rdvalid_delay for DDR5-only channels

@param[in] Host     - Pointer to SysHost
@param[in] Socket   - Socket number

@retval   N/A
**/
VOID
ProgramErid2DataValidDdr5 (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket
)
{
  UINT8                                     Ch;
  struct channelNvram(*ChannelNvList)[MAX_CH];
  DDRT_DATAPATH_DELAY_MCDDC_DP_STRUCT       DdrtDatapathDelay;
  CMPL_TO_DATA_DELAY_MCDDC_DP_STRUCT        CmplDataDelay;

  ChannelNvList = GetChannelNvList(Host, Socket);

  for (Ch = 0; Ch < MAX_CH; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    if ((*ChannelNvList)[Ch].ddrtEnabled == 1) {
      continue;
    }
    CmplDataDelay.Data = MemReadPciCfgEp (Socket, Ch, CMPL_TO_DATA_DELAY_MCDDC_DP_REG);
    DdrtDatapathDelay.Data = MemReadPciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG);
    DdrtDatapathDelay.Bits.erid_to_rdvalid_delay = CmplDataDelay.Bits.cmpl_program_delay - 4;
    MemWritePciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG, DdrtDatapathDelay.Data);
  } // Ch loop
}

/**
  Program the delay between ERID and RdValid for DDRT

  @param[in] Host     - Pointer to SysHost
  @param[in] Socket   - Socket number

  @retval   N/A
**/
VOID
ProgramErid2DataValidDdrt (
  IN    PSYSHOST  Host,
  IN    UINT8     Socket
  )
{
  UINT8                                     Ch;
  UINT8                                     Dimm;
  struct channelNvram                       (*ChannelNvList)[MAX_CH];
  struct dimmNvram                          (*DimmNvList)[MAX_DIMM];
  DDRT_DATAPATH_DELAY_MCDDC_DP_STRUCT       DdrtDatapathDelay;
  UINT8                                     Roundtrip;
  INT16                                     Gnt2Erid;
#if (MEM_IP_VER == MEM_IP_TODO)
  DDRT_DATAPATH_DELAY2_MCDDC_DP_STRUCT      DdrtDataPathDelay2;
#endif // (MEM_IP_VER == MEM_IP_TODO)
  SYS_SETUP                                 *Setup;
  UINT8                                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  ChannelNvList = GetChannelNvList (Host, Socket);

  for (Ch = 0; Ch < MaxChDdr; Ch++) {
    if ((*ChannelNvList)[Ch].enabled == 0) {
      continue;
    }
    if ((*ChannelNvList)[Ch].ddrtEnabled == 0) {
      continue;
    }
    DdrtDatapathDelay.Data    = MemReadPciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG);
    DimmNvList = GetDimmNvList(Host, Socket, Ch);
    for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
      if ((*DimmNvList)[Dimm].dimmPresent == 0) {
        continue;
      }
      if ((*DimmNvList)[Dimm].DcpmmPresent == 0) {
        continue;
      }
      switch(Dimm) {
        case 0:
          // roundtrip0
          GetRoundTrip (Socket, Ch, 0, GSM_READ_CSR, &Roundtrip);
          break;
        case 1:
        default:
          // roundtrip1
          GetRoundTrip (Socket, Ch, MAX_RANK_DIMM, GSM_READ_CSR, &Roundtrip);
          break;
      } // switch
      // Read to Data Valid = Roundtrip[Q Clks]/2 - Trained Grant2Erid[D Clks] - 4 [D Clks]
      GetSetSxpGnt2Erid (Host, Socket, Ch, Dimm, GSM_READ_ONLY, &Gnt2Erid);
      DdrtDatapathDelay.Bits.erid_to_rdvalid_delay = (Roundtrip / 2) - 1 - Gnt2Erid - 4;
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Ch, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "erid2datavalid = %d\n", DdrtDatapathDelay.Bits.erid_to_rdvalid_delay);
    } // Dimm loop

    //
    // HSD5331313: 2lm Scoreboard rejects MemInvXtoI multiple times
    //
#if (MEM_IP_VER == MEM_IP_TODO)
    DdrtDatapathDelay.Bits.rd = DdrtDatapathDelay.Bits.erid_to_rdvalid_delay + 4 + 1;
    DdrtDataPathDelay2.Data = MemReadPciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY2_MCDDC_DP_REG);
    DdrtDataPathDelay2.Bits.erid_to_retry_delay = DdrtDatapathDelay.Bits.erid_to_rdvalid_delay + 1;
    DdrtDataPathDelay2.Bits.erid_to_earlyretry_delay = DdrtDataPathDelay2.Bits.erid_to_retry_delay - 2;
    if ((Host->nvram.mem.socket[Socket].ddrFreq >= DDR_2666) || ((Host->nvram.mem.socket[Socket].ddrFreq >= DDR_2400) && (Setup->mem.optionsExt & WR_CRC))) {
      ddrtDatapathDelay2.Bits.wr = 0x15;
    }
    MemWritePciCfgEp (socket, Ch, DDRT_DATAPATH_DELAY2_MCDDC_DP_REG, DdrtDataPathDelay2.Data);
#endif // (MEM_IP_VER == MEM_IP_TODO)

    MemWritePciCfgEp (Socket, Ch, DDRT_DATAPATH_DELAY_MCDDC_DP_REG, DdrtDatapathDelay.Data);
  } // Ch loop
}

/**
  Program the delay between ERID and RdValid
  This function invokes both the DDRT and the DDR4-only methods.

  @param[in] Host     - Pointer to SysHost
  @param[in] Socket   - The socket number

  @retval   N/A
**/
VOID
ProgramErid2DataValid (
  IN    PSYSHOST Host,
  IN    UINT8    Socket
  )
{
  //
  // Program DDR-only channels
  //
  if (Host->nvram.mem.dramType == SPD_TYPE_DDR5 && IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    ProgramErid2DataValidDdr5(Host, Socket);
  } else {
    ProgramErid2DataValidDdr4 (Host, Socket);
  }
  //
  // Program DDRT channels
  //
  ProgramErid2DataValidDdrt (Host, Socket);

}

UINT32
SetupSVLandScrambling (
  PSYSHOST Host
  )
{
  UINT8                                         socket;
  UINT8                                         ch, SktCh;
  UINT8                                         mc;
  UINT8                                         NumChPerMc;
  UINT32                                        VirtualLockstepEn;
  struct channelNvram                           (*channelNvList)[MAX_CH];
  MCSCRAMBLECONFIG_MCDDC_DP_STRUCT              scrambleConfig;
  SPARING_CONTROL_MC_MAIN_STRUCT                SprngCtrl;
  SYS_SETUP                                     *Setup;
  UINT8                                         MaxChDdr;
  UINT8                                         MaxImc;

  Setup = GetSysSetupPointer ();
  NumChPerMc = GetNumChannelPerMc ();
  MaxImc = GetMaxImc ();
  socket = Host->var.mem.currentSocket;

  //
  // Return if this socket is disabled
  //

  if (IsSocketDimmPopulated (Host, socket) != TRUE) {
    return SUCCESS;
  }

  channelNvList = GetChannelNvList(Host, socket);

  if ((Host->nvram.mem.RASmode & STAT_VIRT_LOCKSTEP) == STAT_VIRT_LOCKSTEP) {
    for(mc = 0; mc< MaxImc; mc++) {
      VirtualLockstepEn = 0;
      for (ch = 0; ch < NumChPerMc; ch++) {
        SktCh= NODECH_TO_SKTCH(mc,ch);

        if ((*channelNvList)[SktCh].enabled == 0) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                         "\nChannel not enabled, no SVL \n");
          continue;            // Channel not enabled, no SVL
        }

        if ( (Host->nvram.mem.socket[socket].channelList[SktCh].dimmList[0].DcpmmPresent)
            || (Host->nvram.mem.socket[socket].channelList[SktCh].dimmList[1].DcpmmPresent)){
          continue; // Cannot enable SVL on a channel with AEP dimm
        }

        SprngCtrl.Data = MemReadPciCfgEp (socket, SktCh, SPARING_CONTROL_MC_MAIN_REG);
        VirtualLockstepEn = VirtualLockstepEn | 1<<(ch);
//18ww01e_ICX_TODO virtual_lockstep_en removed from CRIF file
//      SprngCtrl.Bits.virtual_lockstep_en = VirtualLockstepEn;
        MemWritePciCfgEp (socket, SktCh, SPARING_CONTROL_MC_MAIN_REG, SprngCtrl.Data);
//      RcDebugPrintWithDevice (SDBG_MAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
//                     "IMC[%d].SPARING_CONTROL.virtual_lockstep_en = %d\n", mc, SprngCtrl.Bits.virtual_lockstep_en);
      }
    }
  }

  MaxChDdr = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    //
    // Enable Scrambling
    //
    if (Setup->mem.options & SCRAMBLE_EN) {

      scrambleConfig.Data = MemReadPciCfgEp (socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG);
      scrambleConfig.Bits.rx_enable = 1;
      scrambleConfig.Bits.tx_enable = 1;
      scrambleConfig.Bits.ch_enable = 1;

      MemWritePciCfgEp (socket, ch, MCSCRAMBLECONFIG_MCDDC_DP_REG, scrambleConfig.Data);
      RcDebugPrintWithDevice (SDBG_MAX, socket, ch, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                     "\nScrambling Enabled\n");
    }
  }

  return SUCCESS;
}

VOID
DdrtCapErrorWA (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch
  )
{
#ifdef DDRT_SUPPORT
  SCHEDULER_CMD_DEBUG_MCDDC_CTL_STRUCT SchedulerCmdDebug;

  SchedulerCmdDebug.Data = MemReadPciCfgEp (socket, ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG);
  SchedulerCmdDebug.Bits.cmd_odt_on = 0;  //disable cmd_odt_oe
  SchedulerCmdDebug.Bits.cmd_cs_on = 0;  //disable cmd_cs_oe
  MemWritePciCfgEp (socket, ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG, SchedulerCmdDebug.Data);

  return;
#endif // DDRT_SUPPORT
}

/**
DDRT commands can be issued with UI1 in high impedance mode depending on timing of
DDR4 error event with regard to DDRT command issue leading to silent data corruption
in the DDRT DIMM.  Setting scheduler_cmd_debug.cmd_ca_oe=1 and scheduler_cmd_debug.cmd_ca_on=1
works around this bug.

  @param[in] Socket - Socket number
  @param[in] Ch     - Channel number

  @retval N/A
**/
VOID
DdrtSilentDataCorruptionWA (
  IN UINT8 Socket,
  IN UINT8 Ch
  )
{
#ifdef DDRT_SUPPORT
  SCHEDULER_CMD_DEBUG_MCDDC_CTL_STRUCT SchedulerCmdDebug;

  if (IsDdrAndDdrtPresent (Socket, Ch)) {
    SchedulerCmdDebug.Data = MemReadPciCfgEp (Socket, Ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG);
    SchedulerCmdDebug.Bits.cmd_ca_oe = 1;
    SchedulerCmdDebug.Bits.cmd_ca_on = 1;
    MemWritePciCfgEp (Socket, Ch, SCHEDULER_CMD_DEBUG_MCDDC_CTL_REG, SchedulerCmdDebug.Data);
  }
  return;
#endif // DDRT_SUPPORT
}



VOID
ddrtS3Resume (
  PSYSHOST Host,
  UINT8 socket
  )
{
#ifdef DDRT_SUPPORT
  UINT8  ch;
  struct channelNvram (*channelNvList)[MAX_CH];
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT8  dimm;
  BOOLEAN Ddrt2DimmPresent = FALSE;
  UINT8   MaxChDdr;

  if ((GetSysBootMode () == NormalBoot)) {
    return;
  }
  channelNvList = GetChannelNvList(Host, socket);
  MaxChDdr      = GetMaxChDdr ();
  for (ch = 0; ch < MaxChDdr; ch++) {
    if ((*channelNvList)[ch].enabled == 0) {
      continue;
    }
    if ((*channelNvList)[ch].ddrtEnabled == 0) {
      continue;
    }
    dimmNvList = GetDimmNvList(Host, socket, ch);
    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      if ((*dimmNvList)[dimm].fmcType == FMC_CWV_TYPE) {
        Ddrt2DimmPresent = TRUE;
      }
    }
    if (!Ddrt2DimmPresent) {
      JedecInitDdrChannel(Host, socket, ch);
    }
  } // Channel loop
#endif // DDRT_SUPPORT
}

/**

  Set the CPGC transaction target is DDRT or DRAM scheduler.

  @param[in]                Socket              Socket Id
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                CpgcForDdrt         BOOLEAN TRUE: target is DDRT; FALSE: target is DRAM

  @retval N/A

**/
VOID
SetCpgcTrainTarget (
  IN UINT8           Socket,
  IN UINT8           Channel,
  IN BOOLEAN         CpgcForDdrt
  )
{
  MCMTR_MC_MAIN_STRUCT                 McMtr;

  McMtr.Data = MemReadPciCfgEp ( Socket, Channel, MCMTR_MC_MAIN_REG);
  McMtr.Bits.trng_target = CpgcForDdrt;
  MemWritePciCfgEp (Socket, Channel, MCMTR_MC_MAIN_REG, McMtr.Data);
}

/**

  Enables/Disables the CPGC Mode

  @param[in]                Socket              Socket Id
  @param[in]                MemSs               Current Memory SubSystem (0-Based)
  @param[in]                Channel             Current Channel (0-Based)
  @param[in]                Enable              TRUE: Enables CPGC Mode.FALSE: Disables CPGC Mode

**/
VOID
SetCpgcTrainMode (
  IN        UINT8           Socket,
  IN        UINT8           MemSs,
  IN        UINT8           Channel,
  IN        BOOLEAN         Enable
  )
{
  MCMTR_MC_MAIN_STRUCT                 McMtr;

  McMtr.Data  = MemReadPciCfgEp (Socket, Channel, MCMTR_MC_MAIN_REG);

  if (Enable) {
    McMtr.Bits.trng_mode = 1;
  } else {
    McMtr.Bits.trng_mode = 0;
  }

  MemWritePciCfgEp (Socket, Channel, MCMTR_MC_MAIN_REG, McMtr.Data);
}
/**

  Get the CPGC transaction target is DDRT or DRAM scheduler.

  @param[in]                Socket              Socket Id
  @param[in]                Channel             Current Channel (0-Based)

  @retval BOOLEAN       - TRUE: DDRT, FALSE: DRAM

**/
BOOLEAN
GetCpgcTrainTarget (
  IN UINT8           Socket,
  IN UINT8           Channel
  )
{
  MCMTR_MC_MAIN_STRUCT                      McMtr;
  BOOLEAN                                   DdrtTarget;

  McMtr.Data = MemReadPciCfgEp (Socket, Channel, MCMTR_MC_MAIN_REG);
  if (McMtr.Bits.trng_target == 1){
    DdrtTarget = TRUE;
  } else {
    DdrtTarget = FALSE;
  }
  return DdrtTarget;
}


/**
  Cache MC roundtrip registers

  @param[in]  Host    - Pointer to sysHost
  @param[in]  Socket  - Socket number

  @retval - N/A
**/
VOID
CacheRoundTripMc (
  IN PSYSHOST  Host,
  IN UINT8     Socket
  )
{
  //
  // Stub function to satisfy the library class API,
  // should not be called in 10nm. Needs investigation
  // as to whether it should be in the library class.
  //

  RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_24);

  return;

} // CacheRoundTripMc

/**

  Returns the memory controller ID

  @param Host    - Pointer to sysHost
  @param socket  - Socket Id
  @param ch      - Channel number

  @retval MC ID

**/

UINT8
GetMCID (
  PSYSHOST  Host,
  UINT8     socket,
  UINT8     ch
  )
{
  UINT8  mcId;

  if ((ch >= MAX_CH) || (socket >= MAX_SOCKET)) {
    RC_FATAL_ERROR(FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_96);
  }

  mcId = Host->var.mem.socket[socket].channelList[ch].mcId;

  return mcId;

} // GetMCID


  /**

  Sets(ENABLE) or Clears(DISABLE) Mc_2lmddrt_crnode.DDRT_STARVE_CTL.wr_cmpl_fifo_crdt_dis
  based on the EnableDisable input parameter.

  @param Host          - Pointer to sysHost
  @param socket        - Socket Id
  @param ch            - Channel number
  @param EnableDisable - TRUE = set this bit; FALSE = clear this bit

  @retval MC ID

  **/
EFI_STATUS
WriteCompletionCredits (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN BOOLEAN   EnableDisable
  )
{
#if (MEM_IP_VER >= MEM_IP_17ww37e)

  DDRT_STARVE_CTL_MC_2LM_STRUCT                   DdrtCredits;

  DdrtCredits.Data = MemReadPciCfgEp (Socket, Ch, DDRT_STARVE_CTL_MC_2LM_REG);
  if (TRUE == EnableDisable) {
    DdrtCredits.Bits.wr_cmpl_fifo_crdt_dis = 1;
  } else {
    DdrtCredits.Bits.wr_cmpl_fifo_crdt_dis = 0;
  }
  MemWritePciCfgEp (Socket, Ch, DDRT_STARVE_CTL_MC_2LM_REG, DdrtCredits.Data);

#endif // if (MEM_IP_VER >= MEM_IP_17ww37e)

  return EFI_SUCCESS;
}

/**
  This function gets the transfer speed of DDR/HBM from fuse shadow register.

  @param[in] MemTechType       Memory technology type.
  @param[in] SocketId          Socket index.
  @param[in] ChId              Channel index on socket.

  @retval  The value of transfer speed.
**/
UINT8
McGetTransferSpeed (
  IN MEM_TECH_TYPE  MemTechType,
  IN UINT8          SocketId,
  IN UINT8          ChId
  )
{
  IMC_FUSE_DOWNLOAD2_SHADOW_MC_MAIN_STRUCT  FuseShadow2Reg;

  FuseShadow2Reg.Data = ChRegisterRead (MemTechType, SocketId, ChId, IMC_FUSE_DOWNLOAD2_SHADOW_MC_MAIN_REG);
  return (UINT8) FuseShadow2Reg.Bits.fuse_shadow_speed;
}

/**

  Set CPL2 MRC - PCODE Handshake at end of MRC

  @param[in] Host    - Pointer to sysHost

  @retval SUCCESS - Always returns SUCCESS status.

**/

UINT32
EFIAPI
SetCpl2 (
  IN PSYSHOST  Host
  )
{

  //
  // No op on 10nm
  //

  return SUCCESS;

} // SetCpl2

/**
  Get minimum vc floor

  @param[in]   Vc0Floor         - Vc0 Floor Value
  @param[in]   Vc1Floor         - Vc1 Floor Value
  @param[in]   Vc3Floor         - Vc3 Floor Value

  @param[out]  *VcFloor          -   Pointer to minimum vc floor

  @retval None
**/
VOID
GetMinimumVcFloor (
  IN  UINT32  Vc0Floor,
  IN  UINT32  Vc1Floor,
  IN  UINT32  Vc3Floor,
  OUT UINT32  *VcFloor
  )
{
  UINT32 ArrayValue[3];
  UINT32 ArraySize = sizeof (ArrayValue)/sizeof (ArrayValue[0]);

  ArrayValue[0] = Vc0Floor;
  ArrayValue[1] = Vc1Floor;
  ArrayValue[2] = Vc3Floor;

  *VcFloor = GetMinimumOfNonZeroNumberN (ArrayValue, ArraySize);
}

/**

  Configure setup option for PMem

  @param[in]  Host    - Pointer to sysHost

  @retval N/A

**/
VOID
ConfigSetupPMem (
  IN  PSYSHOST  Host
)
{
#ifdef DDRT_SUPPORT
  SYS_SETUP     *Setup;

  Setup = GetSysSetupPointer ();

  if ((Host->nvram.mem.DcpmmPresent == 1) && (IsCpuAndRevision (CPU_ICXSP, REV_ALL))) {
    Setup->mem.optionsExt &= ~WR_CRC;
    RcDebugPrintWithDevice (SDBG_MINMAX, NO_SOCKET, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMem population is discovered. WrCRC disabled!\n");
  }
#endif // DDRT_SUPPORT
}
