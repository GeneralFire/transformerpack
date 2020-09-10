/** @file
  Implementation of memory SSR library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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
#include <Base.h>
#include <Library/RasGlobalDataLib.h>
#include <Library/CpuCsrAccessLib.h>
#include <Library/RasDebugLib.h>

#include <Guid/MemoryMapData.h>
#include <RcRegs.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/IioTopologyLib.h>
#include <Library/PlatSSRLib.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/RasSiliconLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Library/MemMcIpRasLib.h>
#include <Library/ProcessorRasLib.h>

/**
  Keep for SKX support.
  Fixed by ICX silicon. To be removed until remove SKX support.
**/
VOID
EFIAPI
Wa_4930005 (
  IN  UINT8       Socket,
  IN  UINT8       SktCh,
  IN  UINT8       SrcRank
  )
{
}

/**
  Keep for SKX support.
  ICX doesn't need to support FastDiv3 Mode.
**/
VOID
EFIAPI
Wa_5330286 (
  IN   UINT8      NodeId,
  IN   UINT8      Ch
  )
{
}

/**
  Keep for SKX support.
  ICX doesn't need to support FastDiv3 Mode.
**/
VOID
EFIAPI
ClearSparingFastDiv3Wa (
  IN UINT8 NodeId
  )
{
}


/**
  Keep for SKX support.
  ICX does not need s5331846 w/a.
**/
VOID
EFIAPI
Wa_5331846 (
  IN  INT8   Skt
  )
{
}


/**
  Keep for SKX support.
  Fixed by ICX silicon. To be removed until remove SKX support.
**/
VOID
EFIAPI
Wa_5372419 (
  IN   UINT8   Skt,
  IN   UINT8   ChOnSkt,
  IN   UINT8   Rank
  )
{
}


/**
  CDF IEH behavior of CSR parity error is inconsistent. In the event of a CSR parity error, IEH will keep on sending URs for
  any register read and gests locked up.
  due to this reason, WA is to
  1. map CSR parity error condition to a fatal error.
  2. during initialization when BIOS read CDF ieh VID/DID, sIEH is sending USs, BIOS gets 0xFFFFFFFF. BIOS treat it as a CSR parity error, and do global reset.
     this part code should be implemented in PCH package.
  3. when error handling, if UR from IEH when reading its register, reset the system.

  @param [IN]  IehLocalUcSeverity    current IIO local UC severity.

  @retval      IIO local error UC severity to be set.
 **/
UINT32
EFIAPI
Wa_2006679835 (
  IN  UINT32   IehLocalUcSeverity
  )
{
  //
  // Bit 0 is CSR parity error.
  //
  RAS_DEBUG ((LEVEL_FUNC_FLOW, " change CDF IEH local error -- CSR parity severity to fatal\n"));
  return  IehLocalUcSeverity | BIT0;
}


/**
  CDF IEH behavior of CSR parity error is inconsistent. In the event of a CSR parity error, IEH will keep on sending URs for
  any register read and gests locked up.

  @param [IN]  SouthIehFlag    indicate this is south IEH device
  @param [IN]  RegValue        register value of south IEH

  @retval      none
 **/
VOID
EFIAPI
Wa_2006679835_reset (
  IN  BOOLEAN   SouthIehFlag,
  IN  UINT32    RegValue
  )
{
  if ((SouthIehFlag == TRUE) && (RegValue == 0xFFFFFFFF)) {
    RAS_DEBUG ((LEVEL_BASIC_FLOW, " CDF IEH meet CSR parity error, resetting system\n"));
    ResetWarm ();
  }
}


/**
ICXR_RAS: SPD_BITMAP.IP_BITMAP for SPD Smbus defaults to 0xC rather than 0x00 resulting in errors
logged in global error logic rather than local error logic of Global IEH

  @param [IN]  Socket    The socket number
  @retval      none
 **/
VOID
EFIAPI
Wa_1407595452 (
  IN  UINT8   Socket
  )
{
  if (IsSiliconWorkaroundEnabled ("S1407595452")) {
    ClearSmbIpBitmap(Socket);
  }
}

/**
  When a C2U.MSMI happens, the source id is erroneously getting logged in the SMISRCLOG register.
  BIOS WA: BIOS snoops the SMISRCLOG Register’s SidebandValid and SidebandPortID fields When a C2U.MSMI happens.
           If the SidebandValid is set, and the logged SidebandPortID is within core range,
           clear the SMISRCLOG.SidebandValid and SMISRCLOG.SidebandPortID.

  @retval      none
 **/
VOID
EFIAPI
Wa_1706854359 (
  VOID
  )
{
  NCEVENTS_CR_SMISRCLOG_UBOX_CFG_STRUCT UboxSmiSrcLogReg;
  UINT8                                 MaxCoreId = 0;
  UINT8                                 Socket;

  if (IsSiliconWorkaroundEnabled ("S1706854359")) {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      if (IsSocketPresent (Socket) == FALSE) {
        continue;
      }

      UboxSmiSrcLogReg.Data = ReadCpuCsr (Socket, 0, NCEVENTS_CR_SMISRCLOG_UBOX_CFG_REG);
      if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
        MaxCoreId = 22;
      } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
        MaxCoreId =34;
      } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
        MaxCoreId = 6;
      }

      //
      // Clear the SMISRCLOG.SidebandValid and SMISRCLOG.SidebandPortID when the core id is erroneously getting logged in the SMISRCLOG register.
      //
      if ((UboxSmiSrcLogReg.Bits.sidebandvalid == 1) && (UboxSmiSrcLogReg.Bits.sidebandportid < MaxCoreId)) {
        UboxSmiSrcLogReg.Bits.sidebandvalid = 0;
        UboxSmiSrcLogReg.Bits.sidebandportid = 0;
        WriteCpuCsr (Socket, 0, NCEVENTS_CR_SMISRCLOG_UBOX_CFG_REG, UboxSmiSrcLogReg.Data);
      }
    }
  }
}

/**
  Zero the scrub interval.

  This function must be called before modifying the scrub enable for any memory
  channel (except the first time after reset).

  @param[in]  Socket    Socket index.
  @param[in]  Mc        MC index within the socket.
**/
VOID
EFIAPI
ClearScrubInterval (
  IN UINT8  Socket,
  IN UINT8  Mc
  )
{
  SetScrubIntervalMailbox (Socket, Mc, 0);
}

/**
  Restore the scrub interval.

  This function must be called after modifying the scrub enable for any memory
  channel (except the first time after reset).

  @param[in]  Socket    Socket index.
  @param[in]  Mc        MC index within the socket.
**/
VOID
EFIAPI
RestoreScrubInterval (
  IN UINT8  Socket,
  IN UINT8  Mc
  )
{
  MEM_TOPOLOGY  *MemTopology;

  MemTopology = GetMemTopology ();
  RAS_ASSERT (MemTopology != NULL);

  if (MemTopology != NULL && MemTopology->Socket[Socket].PatrolScrubInterval[Mc] != 0) {
    SetScrubIntervalMailbox (Socket, Mc, MemTopology->Socket[Socket].PatrolScrubInterval[Mc]);
  }
}

/**
  B37:FW_UPD in MCi_STATUS is incorrectly repurposed to a valid for extra_err_info in MCi_MISC.
  BIOS WA: Skip set B37:FW_UPD in MCi_STATUS for ICX, ICXD and SNR A0 stepping CPU.

  @retval TRUE       Skip FW_UPD setting.
  @retval FALSE      Set FW_UPD according to system configuration.
 **/
BOOLEAN
EFIAPI
SkipFwUpdSettingWa1606892119 (
  VOID
  )
{
   if (IsSiliconWorkaroundEnabled ("S1606892119")) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  IEH errors that are disabled in the MCi_CTL2 of the Ubox MCA Bank will still set the SMI_ERR_SRC_STATUS.
  BIOS WA: BIOS add checking MCi_CTL2 during error handling. BIOS skip the bank if MCi_CTL2  bit 34 is cleared.

  @retval TRUE        Error log in MC bank 6 should be skipped.
  @retval FALSE       Error log in MC bank 6 should be used.
 **/
BOOLEAN
EFIAPI
SkikpMc6IehErrorLogWa1506843150 (
  VOID
  )
{
  UINT64    Ctl2;

  if (IsSiliconWorkaroundEnabled ("S1506843150")) {
    Ctl2 = AsmReadMsr64 (MSR_IA32_MC6_CTL2);
    if (Ctl2 & BIT34) {
      return FALSE;
    } else {
      return TRUE;
    }
  }

  return FALSE;
}


/**
  Apply the work around required for ADDDC sparing on Standard RAS parts.
  If either the dimm slot is empty or populated with DDRT dimm

  @param Socket    - Socket ID
  @param SktCh     - Channel on socket

  @retval none

**/
VOID
EFIAPI
AdddcStandardRasWa (
  UINT8 Socket,
  UINT8 SktCh
  )
{
  UINT8   Dimm;
  UINT32  DimmPop;
  MEM_TOPOLOGY            *MemTopology;

  if (IsSiliconWorkaroundEnabled ("S1408298748")) {
    MemTopology = GetMemTopology ();

    if ((MemTopology->SystemRasType == ADVANCED_RAS) || (MemTopology->SystemRasType == ICXP_RAS)) {
      return; // WA Applicable only for Standard RAS parts
    } else {
      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {
        GetDimmMtr (Socket, SktCh, Dimm, &DimmPop);
        if (DimmPop == 0) {  //Cleared if dimm is empty or populated with DDRT dimm
          SetDimmMtr (Socket, SktCh, Dimm, 1); //Socket, SktCh, Dimm, RankCnt
        }
      }
    }
  }
  return;
}

/**
  5370783: Infinite looping SMM error handler when Emcagen2 enabled and system recoverying from IERR.

  @param[in]  Socket           - Socket number

  @retval None
**/
VOID
EFIAPI
ClearPcuMcaErrSrcWa5370783 (
  IN   UINT8   Socket
  )
{
}

/**
  1409498645: BIOS workaround to enable internal Punit error sources for GlobalFatal/Viral signaling.

  @param[in]  Socket           - Socket number

  @retval None
**/
VOID
EFIAPI
EnablePunitErrorSourceWa1409498645 (
  IN   UINT8   Socket
  )
{
  VIRAL_CONTROL_PCU_FUN4_STRUCT ViralControlPcuReg;

  if (IsSiliconWorkaroundEnabled ("S1409498645")) {
    ViralControlPcuReg.Data = ReadCpuCsr (Socket, 0, VIRAL_CONTROL_PCU_FUN4_REG);
    ViralControlPcuReg.Bits.en_pcu_hw_error = 1;
    ViralControlPcuReg.Bits.en_pcu_uc_error = 1;
    ViralControlPcuReg.Bits.en_pcu_fw_error = 1;
    WriteCpuCsr (Socket, 0, VIRAL_CONTROL_PCU_FUN4_REG, ViralControlPcuReg.Data);
  }
}

/**
  1507442372: BIOS doesn't signal MCE when exit SMM for PCU MSMI.

   @param McBankIdx - The MC Bank Number which registered the error. It starts from 0 until MCG_CAP.Count.

  @retval TRUE        Skip MCE signal.
  @retval FALSE       Signal MCE.
**/
BOOLEAN
EFIAPI
SkipMceSignalWa1507442372 (
  IN UINT32 McBankIdx
  )
{
  UINT32 UnitType;

  if (IsSiliconWorkaroundEnabled ("S1507442372")) {
    UnitType = GetMcBankUnitType (McBankIdx);
    if ((UnitType == MCA_UNIT_TYPE_PCU) || (UnitType == MCA_UNIT_TYPE_PUNIT)) {
      RAS_DEBUG ((LEVEL_BASIC_FLOW, "Skip MCE Signal for PCU\n"));
      return TRUE;
    }
  }
  return FALSE;
}
