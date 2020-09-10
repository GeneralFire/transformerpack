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
#include <Library/ProcMemInitChipLib.h>
#include <Library/SystemInfoLib.h>
#include <Cpu/CpuCoreRegs.h>

#include <ScratchpadList.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/CsrAccessLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/ProcMemInitTargetLib.h>
#include <Library/UsraCsrLib.h>
#include <Memory/MemoryCheckpointCodes.h>

#include "Include/ProcMemInitLibInternal.h"
#include <Library/SiliconWorkaroundLib.h>
#include <Library/MemRcLib.h>

VOID
SetSetupOptionsChip (
  PSYSHOST Host
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  if (IsSiliconWorkaroundEnabled ("S1408942955")) {
    //
    // SNR A0 Stepping SKU QQZK is fused to max DDR frequency of 1866
    // This forces all A0 SNR stepping's to 1866
    //
    Setup->mem.ddrFreqLimit = DDR_1866 + 1;
  }
} // SetSetupOptionsChip

#ifdef DEBUG_CODE_BLOCK
/**

    Print Memory Setup Options

    @param Host   - Pointer to SysHost
    @param socket - Current socket

    @retval N/A

**/
VOID
PrintMemSetupOptionsChip (
  PSYSHOST Host,
  UINT8    socket
  )
{
  UINT8           i ;
  KTI_HOST_IN     *KtiHostInPtr;
  KTI_HOST_OUT    *KtiHostOutPtr;
  SYS_SETUP       *Setup;
  UINT8           MaxImc;

  Setup  = GetSysSetupPointer ();
  MaxImc = GetMaxImc ();

  KtiHostInPtr = KTI_HOST_IN_PTR;
  KtiHostOutPtr = KTI_HOST_OUT_PTR;

  RcDebugPrint (SDBG_DEFAULT, "  lowGap:          %02X\n", KtiHostOutPtr->lowGap);
  RcDebugPrint (SDBG_DEFAULT, "  highGap:         %02X\n", KtiHostInPtr->highGap);
  RcDebugPrint (SDBG_DEFAULT, "  mmiohSize:       %08X\n", KtiHostInPtr->mmiohSize);
  RcDebugPrint (SDBG_DEFAULT, "  isocEn:          %02X\n", KtiHostInPtr->isocEn);
  RcDebugPrint (SDBG_DEFAULT, "  dcaEn:           %02X\n", KtiHostInPtr->dcaEn);
  RcDebugPrint (SDBG_DEFAULT, "  options (Chip):  %08X\n", Setup->mem.options);
  RcDebugPrint (SDBG_DEFAULT, "      SCRAMBLE_EN_DDRT         ");
  if (Setup->mem.options & SCRAMBLE_EN_DDRT) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      RAS_TO_INDP_EN      ");
  if (Setup->mem.options & RAS_TO_INDP_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  DfxForceOdtOn ");
  if (Setup->mem.dfxMemSetup.DfxForceOdtOn == FORCE_ODT_ON_ENABLE) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, " optionsNgn: %08X\n", Setup->mem.optionsNgn);
  RcDebugPrint (SDBG_DEFAULT, "  PMem_CMD_TIME ");
  if (Setup->mem.optionsNgn & NGN_CMD_TIME) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  PMem_ECC_CORR ");
  if (Setup->mem.dfxMemSetup.dfxOptions & NGN_ECC_CORR) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  PMem_ECC_WR_CHK ");
  if (Setup->mem.dfxMemSetup.dfxOptions & NGN_ECC_WR_CHK) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  PMem_ECC_EXIT_CORR ");
  if (Setup->mem.dfxMemSetup.dfxOptions & NGN_ECC_EXIT_CORR) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  PMem_ECC_RD_CHK ");
  if (Setup->mem.optionsNgn & NGN_ECC_RD_CHK) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  PMem_DEBUG_LOCK ");
  if (Setup->mem.optionsNgn & NGN_DEBUG_LOCK) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  PMem_ARS_PUBLISH ");
  if (Setup->mem.optionsNgn & NGN_ARS_PUBLISH) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }

  RcDebugPrint (SDBG_DEFAULT, "DfxTmeCmiCplFabCreditWa = %d\n", Setup->mem.dfxMemSetup.DfxTmeCmiCplFabCreditWa);
  RcDebugPrint (SDBG_DEFAULT, "PMem AveragePower: %d\n", Setup->mem.NgnAveragePower);
  RcDebugPrint (SDBG_DEFAULT, "PMem AveragePowerLimit: %d\n", Setup->mem.DcpmmAveragePowerLimit);
  RcDebugPrint (SDBG_DEFAULT, "PMem AveragePowerTimeConstant: %d\n", Setup->mem.DcpmmAveragePowerTimeConstant);
  RcDebugPrint (SDBG_DEFAULT, "PMem MbbAveragePowerTimeConstant: %d\n", Setup->mem.DcpmmMbbAveragePowerTimeConstant);
  RcDebugPrint (SDBG_DEFAULT, "PMem MbbFeature: %d\n", Setup->mem.DcpmmMbbFeature);
  RcDebugPrint (SDBG_DEFAULT, "PMem MbbMaxPowerLimit: %d\n", Setup->mem.DcpmmMbbMaxPowerLimit);

  RcDebugPrint (SDBG_DEFAULT, "ExtendedType17: %x\n", Setup->mem.ExtendedType17);

  RcDebugPrint (SDBG_DEFAULT, "FactoryResetClear: %x\n", Setup->mem.FactoryResetClear);
  RcDebugPrint (SDBG_DEFAULT, "\nstruct sysHost.setup.mem (Chip) {\n");
  RcDebugPrint (SDBG_DEFAULT, "  imcInter:        %02X\n", Setup->mem.imcInter);
  RcDebugPrint (SDBG_DEFAULT, "  DdrtMemPwrSave:  %02X\n", Setup->mem.DdrtMemPwrSave);
  RcDebugPrint (SDBG_DEFAULT, "  spareErrTh:      %02X\n", Setup->mem.spareErrTh);
  RcDebugPrint (SDBG_DEFAULT, "  DieSparing:      %02X\n", Setup->mem.DieSparing);
  RcDebugPrint (SDBG_DEFAULT, "  enableBiosSsaRMT: %02X\n", Setup->mem.enableBiosSsaRMT);
  RcDebugPrint (SDBG_DEFAULT, "  enableBiosSsaRMTonFCB: %02X\n", Setup->mem.enableBiosSsaRMTonFCB);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaPerBitMargining: %02X\n", Setup->mem.biosSsaPerBitMargining);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaDisplayTables: %02X\n", Setup->mem.biosSsaDisplayTables);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaPerDisplayPlots: %02X\n", Setup->mem.biosSsaPerDisplayPlots);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaLoopCount: %02X\n", Setup->mem.biosSsaLoopCount);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaBacksideMargining: %02X\n", Setup->mem.biosSsaBacksideMargining);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaEarlyReadIdMargining: %02X\n", Setup->mem.biosSsaEarlyReadIdMargining);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaStepSizeOverride: %02X\n", Setup->mem.biosSsaStepSizeOverride);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaRxDqs: %02X\n", Setup->mem.biosSsaRxDqs);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaRxVref: %02X\n", Setup->mem.biosSsaRxVref);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaTxDq: %02X\n", Setup->mem.biosSsaTxDq);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaTxVref: %02X\n", Setup->mem.biosSsaTxVref);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaCmdAll: %02X\n", Setup->mem.biosSsaCmdAll);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaCmdVref: %02X\n", Setup->mem.biosSsaCmdVref);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaCtlAll: %02X\n", Setup->mem.biosSsaCtlAll);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaEridDelay: %02X\n", Setup->mem.biosSsaEridDelay);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaEridVref: %02X\n", Setup->mem.biosSsaEridVref);
  RcDebugPrint (SDBG_DEFAULT, "  biosSsaDebugMessages: %02X\n", Setup->mem.biosSsaDebugMessages);
  RcDebugPrint (SDBG_DEFAULT, "  NgnAddressRangeScrub:      %02X\n", Setup->mem.NgnAddressRangeScrub);
  RcDebugPrint (SDBG_DEFAULT, "  leakyBktLo:      %02X\n", Setup->mem.leakyBktLo);
  RcDebugPrint (SDBG_DEFAULT, "  leakyBktHi:      %02X\n", Setup->mem.leakyBktHi);
  RcDebugPrint (SDBG_DEFAULT, "  };\n");
  //
  //**Memmap related options**
  //
  RcDebugPrint (SDBG_DEFAULT, "  volMemMode = %d\n", Setup->mem.volMemMode );
  RcDebugPrint (SDBG_DEFAULT, "  CacheMemType = %d\n", Setup->mem.CacheMemType );
  RcDebugPrint (SDBG_DEFAULT, "  DdrCacheSize = %d\n", Setup->mem.DdrCacheSize );
  RcDebugPrint (SDBG_DEFAULT, "  PmemCaching = %d\n", Setup->mem.PmemCaching);
  RcDebugPrint (SDBG_DEFAULT, "  EadrSupport = %d\n", Setup->mem.EadrSupport);
  if (Setup->mem.EadrSupport == EADR_ENABLED) {
    RcDebugPrint (SDBG_DEFAULT, "  EadrCacheFlushMode = %d\n", Setup->mem.EadrCacheFlushMode);
  }
  RcDebugPrint (SDBG_DEFAULT, "  memInterleaveGran1LM = %d\n", Setup->mem.memInterleaveGran1LM);
  RcDebugPrint (SDBG_DEFAULT, "  FadrSupport = %d\n", Setup->mem.FadrSupport);

  //
  // Biased 2-way near memory cache setup options
  //
  RcDebugPrint (SDBG_DEFAULT, "  EnableTwoWayNmCache = %d\n", Setup->mem.EnableTwoWayNmCache);
  RcDebugPrint (SDBG_DEFAULT, "  NonPreferredWayMask = 0x%03x\n", Setup->mem.NonPreferredWayMask);
  RcDebugPrint (SDBG_DEFAULT, "  PreferredReadFirst = %d\n", Setup->mem.PreferredReadFirst);

  //
  // Boot-time fast zero memory
  //
  RcDebugPrint (SDBG_DEFAULT, "  FastZeroMemSupport = %d\n", Setup->mem.FastZeroMemSupport);

  //
  //**Memmap related options for DFX
  //
  RcDebugPrint (SDBG_DEFAULT, "  dfxCfgMask2LM = %d\n", Setup->mem.dfxMemSetup.dfxCfgMask2LM);
  RcDebugPrint (SDBG_DEFAULT, "  DfxDdrtChInterleaveGran = %02X\n", Setup->mem.dfxMemSetup.DfxDdrtChInterleaveGran);
  RcDebugPrint (SDBG_DEFAULT, "  dfxRankInter = %02X\n", Setup->mem.dfxMemSetup.dfxRankInter);
  RcDebugPrint (SDBG_DEFAULT, "  dfxDimmManagement = %02X\n", Setup->mem.dfxMemSetup.dfxDimmManagement);
  RcDebugPrint (SDBG_DEFAULT, "  dfxLoadDimmMgmtDriver = %02X\n", Setup->mem.dfxMemSetup.dfxLoadDimmMgmtDriver);
  RcDebugPrint (SDBG_DEFAULT, "  dfxPerMemMode = %d\n", Setup->mem.dfxMemSetup.dfxPerMemMode);
  RcDebugPrint (SDBG_DEFAULT, "  dfxPartitionDDRTDimm = %02X\n", Setup->mem.dfxMemSetup.dfxPartitionDDRTDimm);
  RcDebugPrint (SDBG_DEFAULT, "  dfxOptions(BIT0->HIGH_ADDR_EN) = %02X\n", (Setup->mem.dfxMemSetup.dfxOptions & HIGH_ADDR_EN));
  RcDebugPrint (SDBG_DEFAULT, "  dfxOptions(BIT2->CR_MIXED_SKU) = %02X\n", (Setup->mem.dfxMemSetup.dfxOptions & CR_MIXED_SKU));
  RcDebugPrint (SDBG_DEFAULT, "  dfxOptions(BIT3->LOCK_NGN_CSR_DIS) = %02X\n", (Setup->mem.dfxMemSetup.dfxOptions & LOCK_NGN_CSR_DIS));
  RcDebugPrint (SDBG_DEFAULT, "  dfxOptions(BIT4->SHORT_STROKE_INT) = %02X\n", (Setup->mem.dfxMemSetup.dfxOptions & SHORT_STROKE_INT));
  RcDebugPrint (SDBG_DEFAULT, "  dfxOptions(BIT5->SHORT_STROKE_NO_INT) = %02X\n", (Setup->mem.dfxMemSetup.dfxOptions & SHORT_STROKE_NO_INT));
  RcDebugPrint (SDBG_DEFAULT, "  dfxOptions(BIT8->CR_SKIP_ARS_ON_BOOT) = %02X\n", (Setup->mem.dfxMemSetup.dfxOptions & CR_SKIP_ARS_ON_BOOT));
  RcDebugPrint (SDBG_DEFAULT, "  dfxOptions(BIT9->CR_NO_BACKGROUND_ARS) = %02X\n", (Setup->mem.dfxMemSetup.dfxOptions & CR_NO_BACKGROUND_ARS));
#ifdef CWV_A0_REMOVE_POST_PO
  RcDebugPrint (SDBG_DEFAULT, "  dfxOptions(BIT16->CR_SKIP_MB2_COMMAND) = %02X\n", (Setup->mem.dfxMemSetup.dfxOptions & CR_SKIP_MB2_COMMAND));
#endif
  RcDebugPrint (SDBG_DEFAULT, "  DfxDdrEarlyCmpEn = %d\n", Setup->mem.dfxMemSetup.DfxDdrEarlyCmpEn);
  RcDebugPrint (SDBG_DEFAULT, "  DfxHbmEarlyCmpEn = %d\n", Setup->mem.dfxMemSetup.DfxHbmEarlyCmpEn);
  RcDebugPrint (SDBG_DEFAULT, "  DfxClusterXorEn = %d\n", Setup->mem.dfxMemSetup.DfxClusterXorEn);
  RcDebugPrint (SDBG_DEFAULT, "  DfxChannelXorEn = %d\n", Setup->mem.dfxMemSetup.DfxChannelXorEn);
  RcDebugPrint (SDBG_DEFAULT, "  DfxPseudoChannelXorEn = %d\n", Setup->mem.dfxMemSetup.DfxPseudoChannelXorEn);
  RcDebugPrint (SDBG_DEFAULT, "  DfxDimmXorEn = %d\n", Setup->mem.dfxMemSetup.DfxDimmXorEn);
  RcDebugPrint (SDBG_DEFAULT, "  DfxRankXorEn = %d\n", Setup->mem.dfxMemSetup.DfxRankXorEn);
  RcDebugPrint (SDBG_DEFAULT, "  DfxSidXorEn = %d\n", Setup->mem.dfxMemSetup.DfxSidXorEn);
  RcDebugPrint (SDBG_DEFAULT, "  DfxBankXorEn = %d\n", Setup->mem.dfxMemSetup.DfxBankXorEn);
  RcDebugPrint (SDBG_DEFAULT, "  DfxSwXoverOption = %d\n", Setup->mem.dfxMemSetup.DfxSwXoverOption);

  for( i = 0 ; i < ( MAX_SOCKET * MaxImc ) ; i++ )  {
    RcDebugPrint (SDBG_DEFAULT, "  dfxPartitionRatio[%02X] = %02X\n", i, Setup->mem.dfxMemSetup.dfxPartitionRatio[i] );
  }
  RcDebugPrint (SDBG_DEFAULT, "  DfxMcDataParityCheck: %02X\n", Setup->mem.dfxMemSetup.DfxMcDataParityCheck);

  //
  // DDRT related setup option
  //
  RcDebugPrint (SDBG_DEFAULT, "  ddrtXactor = %d\n", Setup->common.ddrtXactor);
  RcDebugPrint (SDBG_DEFAULT, "  DfxDdrtSchedulerDebug = %d\n", Setup->mem.dfxMemSetup.DfxDdrtSchedulerDebug);

  //
  // ** struct ddrIMCSetup **
  //
  RcDebugPrint (SDBG_DEFAULT, "  struct ddrIMCSetup[%02X] {\n", socket);
  RcDebugPrint (SDBG_DEFAULT, "    enabled:        %08X\n", Setup->mem.socket[socket].enabled);
  RcDebugPrint (SDBG_DEFAULT, "  }\n");

  return;
}
#endif // DEBUG_CODE_BLOCK


VOID
InitDdrFreqChip (
  PSYSHOST Host,
  UINT8 socket
  )
/*++

Routine Description:



Arguments:

  Host   - Pointer to the system Host (root) structure
  socket - socket id

Returns:

  NA

--*/
{
  return;
} // InitDdrFreqChip

UINT32
SavedSetupDataChangedChip (
  PSYSHOST Host
  )
/*++

Routine Description:

  Initialize internal data structures

Arguments:

  Host  - Pointer to the system Host (root) structure

Returns:

  TRUE / FALSE

--*/
{
  UINT32 result;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  result = FALSE;

  if (
     Host->nvram.mem.savedSetupData.memFlows != Setup->mem.memFlows ||
     Host->nvram.mem.savedSetupData.memFlowsExt != Setup->mem.memFlowsExt ||
     Host->nvram.mem.savedSetupData.memFlowsExt2 != Setup->mem.memFlowsExt2 ||
     Host->nvram.mem.savedSetupData.memFlowsExt3 != Setup->mem.memFlowsExt3 ||
     Host->nvram.mem.savedSetupData.DdrtMemPwrSave != Setup->mem.DdrtMemPwrSave ||
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
     Host->nvram.mem.savedSetupData.meRequestedSize != Setup->mem.meRequestedSize ||
#endif // ME_SUPPORT_FLAG
#if defined(IE_SUPPORT) && IE_SUPPORT
     Host->nvram.mem.savedSetupData.ieRequestedSize != Setup->mem.ieRequestedSize ||
#endif // IE_SUPPORT
     Host->nvram.mem.savedSetupData.spareErrTh != Setup->mem.spareErrTh ||
     Host->nvram.mem.savedSetupData.DieSparing != Setup->mem.DieSparing ||
     Host->nvram.mem.savedSetupData.enableBiosSsaRMT != Setup->mem.enableBiosSsaRMT ||
     Host->nvram.mem.savedSetupData.enableBiosSsaRMTonFCB != Setup->mem.enableBiosSsaRMTonFCB ||
     Host->nvram.mem.savedSetupData.biosSsaPerBitMargining != Setup->mem.biosSsaPerBitMargining ||
     Host->nvram.mem.savedSetupData.biosSsaDisplayTables != Setup->mem.biosSsaDisplayTables ||
     Host->nvram.mem.savedSetupData.biosSsaPerDisplayPlots != Setup->mem.biosSsaPerDisplayPlots ||
     Host->nvram.mem.savedSetupData.biosSsaLoopCount != Setup->mem.biosSsaLoopCount ||
     Host->nvram.mem.savedSetupData.biosSsaBacksideMargining != Setup->mem.biosSsaBacksideMargining ||
     Host->nvram.mem.savedSetupData.biosSsaEarlyReadIdMargining != Setup->mem.biosSsaEarlyReadIdMargining ||
     Host->nvram.mem.savedSetupData.biosSsaStepSizeOverride != Setup->mem.biosSsaStepSizeOverride ||
     Host->nvram.mem.savedSetupData.biosSsaRxDqs != Setup->mem.biosSsaRxDqs ||
     Host->nvram.mem.savedSetupData.biosSsaRxVref != Setup->mem.biosSsaRxVref ||
     Host->nvram.mem.savedSetupData.biosSsaTxDq != Setup->mem.biosSsaTxDq ||
     Host->nvram.mem.savedSetupData.biosSsaTxVref != Setup->mem.biosSsaTxVref ||
     Host->nvram.mem.savedSetupData.biosSsaCmdAll != Setup->mem.biosSsaCmdAll ||
     Host->nvram.mem.savedSetupData.biosSsaCmdVref != Setup->mem.biosSsaCmdVref ||
     Host->nvram.mem.savedSetupData.biosSsaCtlAll != Setup->mem.biosSsaCtlAll ||
     Host->nvram.mem.savedSetupData.biosSsaEridDelay != Setup->mem.biosSsaEridDelay ||
     Host->nvram.mem.savedSetupData.biosSsaEridVref != Setup->mem.biosSsaEridVref ||
     Host->nvram.mem.savedSetupData.biosSsaDebugMessages != Setup->mem.biosSsaDebugMessages ||
     Host->nvram.mem.savedSetupData.NgnAddressRangeScrub != Setup->mem.NgnAddressRangeScrub
    ) {
    result = TRUE;
  }
  return result;
} // SavedSetupDataChangedChip

UINT32
DetectIMCEnabledChangeChip (
  PSYSHOST Host,
  UINT8 socket
  )
/*++

Routine Description:



Arguments:

  Host   - Pointer to the system Host (root) structure
  socket - socket id

Returns:

  TRUE / FALSE

--*/
{
  UINT32 result;

  result = FALSE;

  return result;
} // DetectIMCEnabledChangeChip


UINT32
SavedSetupDataChangedIMCChip (
  PSYSHOST Host,
  UINT8 socket
  )
/*++

Routine Description:

  Initialize internal data structures

Arguments:

  Host   - Pointer to the system Host (root) structure
  socket - socket id

Returns:

  TRUE / FALSE

--*/
{
  UINT32 result;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  result = FALSE;

  if (Host->nvram.mem.savedSetupData.socket[socket].options != Setup->mem.socket[socket].options) {
    result = TRUE;
  }

  return result;
} // SavedSetupDataChangedIMCChip

UINT32
DetectMCAErrorsChip (
  PSYSHOST Host,
  UINT8 socket,
  UINT32 *logData
  )
/*++

Routine Description:



Arguments:

  Host    - Pointer to the system Host (root) structure
  socket  - socket id
  logData - pointer to 32-bit lag data

Returns:

  TRUE / FALSE

--*/
{
  UINT32                          result;
  MCA_ERR_SRC_LOG_PCU_FUN2_STRUCT errSrcLog;

  result = FALSE;

  if (IsHvmModeEnable ()) {
    return FALSE;
  }

  //
  // Disable fast boot if there were any core errors on any socket
  //

  errSrcLog.Data = UsraCsrRead (socket, 0, MCA_ERR_SRC_LOG_PCU_FUN2_REG);
  *logData = errSrcLog.Data;
  if (errSrcLog.Bits.mcerr_internal || errSrcLog.Bits.ierr_internal || errSrcLog.Bits.caterr_internal) {
    result = TRUE;
  }

  return result;
} // DetectMCAErrorsChip

/**

  Gets Processor PPINs

  @param Host:  pointer to sysHost structure on stack

  @retval PPIN
**/
UINT64_STRUCT
GetPpinChip (
  PSYSHOST Host,
  UINT8    socket
  )
{
  UINT64_STRUCT msrReg;
  UINT64_STRUCT procPpin;

  procPpin.hi = 0x0;
  procPpin.lo = 0x0;
  RcDebugPrint (SDBG_DEFAULT, "Get socket PPIN\n");

  //
  // If not running on Simics
  //  and socket present
  //
  if (!(GetEmulation() & SIMICS_FLAG)) {
    if (SocketPresent (socket)) {
      //
      // If PPIN not supported, return
      //
      msrReg = ReadMsrPipe (Host, socket, ICX_MSR_PLATFORM_INFO);
      if ((msrReg.lo & BIT23) == 0) {
        RcDebugPrint (SDBG_DEFAULT, " : Socket not PPIN Capable\n");
        return procPpin;
      }

      //
      // If MSR_PPIN is locked, return
      //
      msrReg = ReadMsrPipe (Host, socket, MSR_PPIN_CTL);
      if ((msrReg.lo & BIT0) == 1) {
        RcDebugPrint (SDBG_DEFAULT, " : PPIN locked\n");
        return procPpin;
      }

      //
      // Set enable bit in MSR_PPIN_CTL
      //  and read MSR_PPIN
      // Then clear enable bit
      //
      msrReg.lo = 2;
      msrReg.hi = 0;
      WriteMsrPipe (Host, socket, MSR_PPIN_CTL, msrReg);
      procPpin = ReadMsrPipe (Host, socket, MSR_PPIN);
      msrReg.lo = 0;
      WriteMsrPipe (Host, socket, MSR_PPIN_CTL, msrReg);
    }
  } else {
    //
    // Use hardcoded PPIN if running under Simics
    //
    RcDebugPrint (SDBG_DEFAULT, " : Running in Simics\n");
    procPpin.hi = 0xFFFFFFFF;
    procPpin.lo = 0x12345678;
  }

  RcDebugPrint (SDBG_DEFAULT, " : PPIN Hi = %08X, PPIN Lo = %08X\n", procPpin.hi, procPpin.lo);

  return procPpin;
} // GetPpinChip

UINT32
DetectProcessorRemovedChip (
  PSYSHOST Host
  )
/*++

Routine Description:

  Initialize internal data structures

Arguments:

  Host  - Pointer to the system Host (root) structure

Returns:

  TRUE / FALSE

--*/
{
  UINT32 result;

  result = FALSE;
  if  (Host->nvram.mem.socketBitMap != GetSocketPresentBitMap ()) {
    result = TRUE;
  }

  return result;
} // DetectProcessorRemovedChip


UINT32
AttemptColdBootFastChip (
  PSYSHOST Host
  )
/*++

Routine Description:



Arguments:

  Host   - Pointer to the system Host (root) structure

Returns:

  TRUE / FALSE

--*/
{
  UINT32   result;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  result = FALSE;
  if ((Host->var.mem.subBootMode == ColdBoot || Host->var.mem.subBootMode == WarmBoot) && (Setup->mem.options & ATTEMPT_FAST_BOOT_COLD)) {
    result = TRUE;
  }
  return result;
} // AttemptColdBootFastChip


/*++

  Determines if quad-rank DIMMs are enabled or disabled by the hardware

  If MEM_IP_VER is MEM_IP17ww51f or greater, then Host->var.common.procCom[socket].capid3
  must be initialized with the contents of the capid3 register before calling this function.

  @param[in]  Host    Pointer to the system Host (root) structure
  @param[in]  Socket  Socket ID

  @retval TRUE  Support for quad-rank DIMMs is disabled in the hardware
  @retval FALSE Support for quad-rank DIMMs is enabled in the hardware

--*/
BOOLEAN
GetDisableQRDimmChip (
  IN  PSYSHOST  Host,
  IN  UINT8     Socket
  )
{
  BOOLEAN Result = FALSE;

#if MEM_IP_VER < MEM_IP_17ww51f

  CAPID3_PCU_FUN3_STRUCT CapId3;

  //
  // Get IMC Fuse data
  //
  CapId3.Data = Host->var.common.procCom[socket].capid3;

  //
  // Check if QR DIMM is disabled in HW
  //
  if (CapId3.Bits.disable_qr_dimm) {
    Result = TRUE;
  }

#endif // if MEM_IP_VER < MEM_IP_17ww51f

  return Result;
} // GetDisableQRDimmChip


VOID
SetIMCEnabledChip (
  PSYSHOST Host,
  UINT8 socket
  )
/*++

Routine Description:



Arguments:

  Host   - Pointer to the system Host (root) structure
  socket - socket id

Returns:

  TRUE / FALSE

--*/
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  Host->nvram.mem.socket[socket].enabled = Setup->mem.socket[socket].enabled;

  DisplayMemSsList (Host, socket);

  return;
} // SetIMCEnabledChip


UINT32
GetChnDisableChip (
  PSYSHOST Host,
  UINT8 socket,
  UINT8 ch
  )
/*++

Routine Description:



Arguments:

  Host   - Pointer to the system Host (root) structure
  socket - socket id
  ch     - ch id

Returns:

  TRUE / FALSE

--*/
{
  UINT32 result;

  result = FALSE;

  return result;
} // GetChnDisableChip


UINT32
GetDisable2dpcChip (
  PSYSHOST Host,
  UINT8 socket
  )
/*++

Routine Description:



Arguments:

  Host   - Pointer to the system Host (root) structure
  socket - socket id

Returns:

  TRUE / FALSE

--*/
{
  UINT32 result = FALSE;

  return result;
} // GetDisable2dpcChip


UINT32
GetDisable3dpcChip (
  PSYSHOST Host,
  UINT8 socket
  )
/*++

Routine Description:



Arguments:

  Host   - Pointer to the system Host (root) structure
  socket - socket id

Returns:

  TRUE / FALSE

--*/
{
  UINT32 result;

  result = TRUE;

  return result;
} // GetDisable3dpcChip

