/** @file
  Implementation of the MemoryPolicyLib library class.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <SysHost.h>
#include <UncoreCommonIncludes.h>
#include <KtiSetupDefinitions.h>
#include <IioPlatformData.h>

#include <Guid/PprVariable.h>
#include <Guid/PartialMirrorGuid.h>
#include <Guid/MemoryMapData.h>
#include <Guid/MemoryOverwriteControl.h>

#include <Ppi/MemoryPolicyPpi.h>
#include <Ppi/ReadOnlyVariable2.h>

#include <Library/SysHostPointerLib.h>
#include <Library/LegacyRmtLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/CrcLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/PerformanceLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/MemoryServicesLib.h>

/**
  Dump SYS_SETUP to the serial log. Dumps a lot of
  data to the log, so only use when debugging.

  @param[in]  SysSetup          Ptr to SYS_SETUP

  @retval None

**/

VOID
DumpSysSetup (
  IN SYS_SETUP *SysSetup
  )
{

  UINT8 Index;
  UINT8 Dimm;
  UINT8 Channel;
  UINT8 MemoryController;
  UINT8 Socket;

  DEBUG ((EFI_D_INFO, "\nDumping SYS_SETUP structure\n"));
  DEBUG ((EFI_D_INFO, "\nsizeof (SYS_SETUP) = %d\n", sizeof (SYS_SETUP)));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.options = 0x%x\n", SysSetup->mem.options));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.optionsExt = 0x%x\n", SysSetup->mem.optionsExt));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.optionsNgn = 0x%x\n", SysSetup->mem.optionsNgn));


  DEBUG ((EFI_D_INFO, "SysSetup->mem.imcBclk = 0x%x\n", SysSetup->mem.imcBclk));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.enforcePOR = 0x%x\n", SysSetup->mem.enforcePOR));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.EnforcePopulationPor = 0x%x\n", SysSetup->mem.EnforcePopulationPor));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.ddrFreqLimit = 0x%x\n", SysSetup->mem.ddrFreqLimit));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.chInter = 0x%x\n", SysSetup->mem.chInter));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dimmTypeSupport = 0x%x\n", SysSetup->mem.dimmTypeSupport));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.ckeThrottling = 0x%x\n", SysSetup->mem.ckeThrottling));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.olttPeakBWLIMITPercent = 0x%x\n", SysSetup->mem.olttPeakBWLIMITPercent));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.thermalThrottlingOptions = 0x%x\n", SysSetup->mem.thermalThrottlingOptions));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.memhotOutputOnlyOpt = 0x%x\n", SysSetup->mem.memhotOutputOnlyOpt));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.DramRaplEnable = 0x%x\n", SysSetup->mem.DramRaplEnable));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dramraplbwlimittf = 0x%x\n", SysSetup->mem.dramraplbwlimittf));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.CmsEnableDramPm = 0x%x\n", SysSetup->mem.CmsEnableDramPm));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dramraplRefreshBase = 0x%x\n", SysSetup->mem.dramraplRefreshBase));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.perBitDeSkew = 0x%x\n", SysSetup->mem.perBitDeSkew));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.enableBiosSsaLoader = 0x%x\n", SysSetup->mem.enableBiosSsaLoader));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.enableBiosSsaRMT = 0x%x\n", SysSetup->mem.enableBiosSsaRMT));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.enableBiosSsaRMTonFCB = 0x%x\n", SysSetup->mem.enableBiosSsaRMTonFCB));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaPerBitMargining = 0x%x\n", SysSetup->mem.biosSsaPerBitMargining));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaDisplayTables = 0x%x\n", SysSetup->mem.biosSsaDisplayTables));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaPerDisplayPlots = 0x%x\n", SysSetup->mem.biosSsaPerDisplayPlots));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaLoopCount = 0x%x\n", SysSetup->mem.biosSsaLoopCount));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaBacksideMargining = 0x%x\n", SysSetup->mem.biosSsaBacksideMargining));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaEarlyReadIdMargining = 0x%x\n", SysSetup->mem.biosSsaEarlyReadIdMargining));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaStepSizeOverride = 0x%x\n", SysSetup->mem.biosSsaStepSizeOverride));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaRxDqs = 0x%x\n", SysSetup->mem.biosSsaRxDqs));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaRxVref = 0x%x\n", SysSetup->mem.biosSsaRxVref));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaTxDq = 0x%x\n", SysSetup->mem.biosSsaTxDq));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaTxVref = 0x%x\n", SysSetup->mem.biosSsaTxVref));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaCmdAll = 0x%x\n", SysSetup->mem.biosSsaCmdAll));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaCmdVref = 0x%x\n", SysSetup->mem.biosSsaCmdVref));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaCtlAll = 0x%x\n", SysSetup->mem.biosSsaCtlAll));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaEridDelay = 0x%x\n", SysSetup->mem.biosSsaEridDelay));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaEridVref = 0x%x\n", SysSetup->mem.biosSsaEridVref));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.biosSsaDebugMessages = 0x%x\n", SysSetup->mem.biosSsaDebugMessages));

#ifdef LRDIMM_SUPPORT

  DEBUG ((EFI_D_INFO, "SysSetup->mem.enableBacksideRMT = 0x%x\n", SysSetup->mem.enableBacksideRMT));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.enableBacksideCMDRMT = 0x%x\n", SysSetup->mem.enableBacksideCMDRMT));

#endif

  DEBUG ((EFI_D_INFO, "SysSetup->mem.enableNgnBcomMargining =              0x%x\n", SysSetup->mem.enableNgnBcomMargining));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.trainingResultOffsetFunctionEnable =  0x%x\n", SysSetup->mem.trainingResultOffsetFunctionEnable));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.offsetTxDq =                          0x%x\n", SysSetup->mem.offsetTxDq));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.offsetRxDq =                          0x%x\n", SysSetup->mem.offsetRxDq));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.offsetTxVref =                        0x%x\n", SysSetup->mem.offsetTxVref));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.offsetRxVref =                        0x%x\n", SysSetup->mem.offsetRxVref));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.offsetCmdAll =                        0x%x\n", SysSetup->mem.offsetCmdAll));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.offsetCmdVref =                       0x%x\n", SysSetup->mem.offsetCmdVref));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.offsetCtlAll =                        0x%x\n", SysSetup->mem.offsetCtlAll));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.OffsetRecEn =                         0x%x\n", SysSetup->mem.OffsetRecEn));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.rmtPatternLength =                    0x%x\n", SysSetup->mem.rmtPatternLength));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.rmtPatternLengthExt =                 0x%x\n", SysSetup->mem.rmtPatternLengthExt));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.patrolScrubDuration =           0x%x\n", SysSetup->mem.patrolScrubDuration));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.DieSparing =                    0x%x\n", SysSetup->mem.DieSparing));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.NgnAddressRangeScrub =          0x%x\n", SysSetup->mem.NgnAddressRangeScrub));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.AdvMemTestOptions =              0x%x\n", SysSetup->mem.AdvMemTestOptions));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.AdvMemTestResetList =            0x%x\n", SysSetup->mem.AdvMemTestResetList));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.AdvMemTestCondition =            0x%x\n", SysSetup->mem.AdvMemTestCondition));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.AdvMemTestCondVdd =              0x%x\n", SysSetup->mem.AdvMemTestCondVdd));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.AdvMemTestCondTwr =              0x%x\n", SysSetup->mem.AdvMemTestCondTwr));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.AdvMemTestCondTrefi =            0x%x\n", SysSetup->mem.AdvMemTestCondTrefi));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.AdvMemTestCondPause =            0x%x\n", SysSetup->mem.AdvMemTestCondPause));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.memTestLoops =                   0x%x\n", SysSetup->mem.memTestLoops));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.scrambleSeedLow =               0x%x\n", SysSetup->mem.scrambleSeedLow));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.scrambleSeedHigh =              0x%x\n", SysSetup->mem.scrambleSeedHigh));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.ADREn =                         0x%x\n", SysSetup->mem.ADREn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.LegacyADRModeEn =               0x%x\n", SysSetup->mem.LegacyADRModeEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MinNormalMemSize =              0x%x\n", SysSetup->mem.MinNormalMemSize));

  if (PcdGetBool (PcdNvDimmEn)) {

    DEBUG ((EFI_D_INFO, "SysSetup->mem.ADRDataSaveMode = 0x%x\n", SysSetup->mem.ADRDataSaveMode));

  }

  DEBUG ((EFI_D_INFO, "SysSetup->mem.check_pm_sts = 0x%x\n", SysSetup->mem.check_pm_sts));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.check_platform_detect = 0x%x\n", SysSetup->mem.check_platform_detect));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.normOppIntvl = 0x%x\n", SysSetup->mem.normOppIntvl));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.SpdSmbSpeed = 0x%x\n", SysSetup->mem.SpdSmbSpeed));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.SpdPrintEn = 0x%x\n", SysSetup->mem.SpdPrintEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.SpdPrintLength = 0x%x\n", SysSetup->mem.SpdPrintLength));
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    DEBUG ((EFI_D_INFO, "SysSetup->mem.socket[%d].enabled = 0x%x\n", Socket, SysSetup->mem.socket[Socket].enabled));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.socket[%d].options = 0x%x\n", Socket, SysSetup->mem.socket[Socket].options));

    for (Channel = 0; Channel < MAX_CH; Channel++) {

      UINT8 Rank;

      DEBUG ((EFI_D_INFO, "SysSetup->mem.socket[%d].ddrCh[%d].enabled = 0x%x\n", Socket, Channel, SysSetup->mem.socket[Socket].ddrCh[Channel].enabled));
      DEBUG ((EFI_D_INFO, "SysSetup->mem.socket[%d].ddrCh[%d].numDimmSlots = 0x%x\n", Socket, Channel, SysSetup->mem.socket[Socket].ddrCh[Channel].numDimmSlots));
      DEBUG ((EFI_D_INFO, "SysSetup->mem.socket[%d].ddrCh[%d].batterybacked = 0x%x\n", Socket, Channel, SysSetup->mem.socket[Socket].ddrCh[Channel].batterybacked));
      DEBUG ((EFI_D_INFO, "SysSetup->mem.socket[%d].ddrCh[%d].rankmask = 0x%x\n", Socket, Channel, SysSetup->mem.socket[Socket].ddrCh[Channel].rankmask));

      for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

        for (Rank = 0; Rank < MAX_RANK_DIMM; Rank++) {

          DEBUG ((EFI_D_INFO, "SysSetup->mem.socket[%d].ddrCh[%d].dimmList[%d].mapOut[%d] = 0x%x\n", Socket, Channel, Dimm, Rank, SysSetup->mem.socket[Socket].ddrCh[Channel].dimmList[Dimm].mapOut[Rank]));

        }

      }

    }

    for (MemoryController = 0; MemoryController < MAX_IMC; MemoryController++) {

      DEBUG ((EFI_D_INFO, "SysSetup->mem.socket[%d].imcEnabled[%d] = 0x%x\n", Socket, MemoryController, SysSetup->mem.socket[Socket].imcEnabled[MemoryController]));

    }

  }

  if (PcdGetBool (PcdMrcXmpSupport)) {

    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nCL = 0x%x\n", SysSetup->mem.inputMemTime.nCL));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nRP = 0x%x\n", SysSetup->mem.inputMemTime.nRP));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nRCD = 0x%x\n", SysSetup->mem.inputMemTime.nRCD));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nRRD = 0x%x\n", SysSetup->mem.inputMemTime.nRRD));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nRRD_L = 0x%x\n", SysSetup->mem.inputMemTime.nRRD_L));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nWTR = 0x%x\n", SysSetup->mem.inputMemTime.nWTR));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nRAS = 0x%x\n", SysSetup->mem.inputMemTime.nRAS));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nRTP = 0x%x\n", SysSetup->mem.inputMemTime.nRTP));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nWR = 0x%x\n", SysSetup->mem.inputMemTime.nWR));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nFAW = 0x%x\n", SysSetup->mem.inputMemTime.nFAW));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nCWL = 0x%x\n", SysSetup->mem.inputMemTime.nCWL));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nRC = 0x%x\n", SysSetup->mem.inputMemTime.nRC));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nCMDRate = 0x%x\n", SysSetup->mem.inputMemTime.nCMDRate));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.ddrFreqLimit = 0x%x\n", SysSetup->mem.inputMemTime.ddrFreqLimit));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.vdd = 0x%x\n", SysSetup->mem.inputMemTime.vdd));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.ucVolt = 0x%x\n", SysSetup->mem.inputMemTime.ucVolt));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.casSup = 0x%x\n", SysSetup->mem.inputMemTime.casSup));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.tREFI = 0x%x\n", SysSetup->mem.inputMemTime.tREFI));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.nRFC = 0x%x\n", SysSetup->mem.inputMemTime.nRFC));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.inputMemTime.ddrFreq = 0x%x\n", SysSetup->mem.inputMemTime.ddrFreq));

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

      for (Channel = 0; Channel < MAX_CH; Channel++) {

        for (Dimm = 0; Dimm < MAX_DIMM; Dimm++) {

          DEBUG ((EFI_D_INFO, "SysSetup->mem.XMPChecksum[%d][%d][%d] = 0x%x\n", Socket, Channel, Dimm, SysSetup->mem.XMPChecksum[Socket][Channel][Dimm]));
        }

      }

    }

  }

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG

  DEBUG ((EFI_D_INFO, "SysSetup->mem.meRequestedSize = 0x%x\n", SysSetup->mem.meRequestedSize));

#endif // ME_SUPPORT_FLAG

#if defined(IE_SUPPORT) && IE_SUPPORT

  DEBUG ((EFI_D_INFO, "SysSetup->mem.ieRequestedSize = 0x%x\n", SysSetup->mem.ieRequestedSize));

#endif // IE_SUPPORT


  DEBUG ((EFI_D_INFO, "SysSetup->mem.customRefreshRate = 0x%x\n", SysSetup->mem.customRefreshRate));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.partialmirrorsad0 = 0x%x\n", SysSetup->mem.partialmirrorsad0));

  for (Index = 0; Index < MAX_PARTIAL_MIRROR; Index ++) {
    DEBUG ((EFI_D_INFO, "SysSetup->mem.partialmirrorsize[%d] = 0x%x\n", Index, SysSetup->mem.partialmirrorsize[Index]));
  }

  DEBUG ((EFI_D_INFO, "SysSetup->mem.partialMirrorUEFI = 0x%x\n", SysSetup->mem.partialMirrorUEFI));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.partialmirrorpercent = 0x%x\n", SysSetup->mem.partialmirrorpercent));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.partialmirrorsts = 0x%x\n", SysSetup->mem.partialmirrorsts));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dllResetTestLoops = 0x%x\n", SysSetup->mem.dllResetTestLoops));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.memFlows = 0x%x\n", SysSetup->mem.memFlows));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.memFlowsExt = 0x%x\n", SysSetup->mem.memFlowsExt));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.memFlowsExt2 = 0x%x\n", SysSetup->mem.memFlowsExt2));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.memFlowsExt3 = 0x%x\n", SysSetup->mem.memFlowsExt3));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.writePreamble = 0x%x\n", SysSetup->mem.writePreamble));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.readPreamble = 0x%x\n", SysSetup->mem.readPreamble));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.dfxRankInter = 0x%x\n", SysSetup->mem.dfxMemSetup.dfxRankInter));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.DramRaplExtendedRange = 0x%x\n", SysSetup->mem.DramRaplExtendedRange));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.spareErrTh = 0x%x\n", SysSetup->mem.spareErrTh));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.leakyBktLo = 0x%x\n", SysSetup->mem.leakyBktLo));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.leakyBktHi = 0x%x\n", SysSetup->mem.leakyBktHi));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.spareRanks = 0x%x\n", SysSetup->mem.spareRanks));

  if (PcdGetBool (PcdNvDimmEn)) {

    DEBUG ((EFI_D_INFO, "SysSetup->mem.interNVDIMMS = 0x%x\n", SysSetup->mem.interNVDIMMS));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.restoreNVDIMMS = 0x%x\n", SysSetup->mem.restoreNVDIMMS));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.eraseArmNVDIMMS = 0x%x\n", SysSetup->mem.eraseArmNVDIMMS));

  }

  DEBUG ((EFI_D_INFO, "SysSetup->mem.cmdSetupPercentOffset = 0x%x\n", SysSetup->mem.cmdSetupPercentOffset));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.pprType = 0x%x\n", SysSetup->mem.pprType));

  for (Index = 0; Index < MAX_PPR_ADDR_ENTRIES; Index++) {

    DEBUG ((EFI_D_INFO, "SysSetup->mem.pprAddrSetup[Index].pprAddrStatus =      0x%x\n", Index, SysSetup->mem.pprAddrSetup[Index].pprAddrStatus));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.pprAddrSetup[Index].socket =             0x%x\n", Index, SysSetup->mem.pprAddrSetup[Index].socket));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.pprAddrSetup[Index].mc =                 0x%x\n", Index, SysSetup->mem.pprAddrSetup[Index].mc));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.pprAddrSetup[Index].ch =                 0x%x\n", Index, SysSetup->mem.pprAddrSetup[Index].ch));

    DEBUG ((EFI_D_INFO, "SysSetup->mem.pprAddrSetup[Index].pprAddr.dimm =       0x%x\n", Index, SysSetup->mem.pprAddrSetup[Index].pprAddr.dimm));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.pprAddrSetup[Index].pprAddr.rank =       0x%x\n", Index, SysSetup->mem.pprAddrSetup[Index].pprAddr.rank));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.pprAddrSetup[Index].pprAddr.subRank =    0x%x\n", Index, SysSetup->mem.pprAddrSetup[Index].pprAddr.subRank));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.pprAddrSetup[Index].pprAddr.nibbleMask = 0x%x\n", Index, SysSetup->mem.pprAddrSetup[Index].pprAddr.nibbleMask));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.pprAddrSetup[Index].pprAddr.bank =       0x%x\n", Index, SysSetup->mem.pprAddrSetup[Index].pprAddr.bank));
    DEBUG ((EFI_D_INFO, "SysSetup->mem.pprAddrSetup[Index].pprAddr.row =        0x%x\n", Index, SysSetup->mem.pprAddrSetup[Index].pprAddr.row));

  }

  DEBUG ((EFI_D_INFO, "SysSetup->mem.imcInter = 0x%x\n", SysSetup->mem.imcInter));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.oneRankTimingModeEn = 0x%x\n", SysSetup->mem.oneRankTimingModeEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxDdrtChInterleaveGran = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxDdrtChInterleaveGran));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.dfxDimmManagement = 0x%x\n", SysSetup->mem.dfxMemSetup.dfxDimmManagement));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxTmeCmiCplFabCreditWa = %d\n", SysSetup->mem.dfxMemSetup.DfxTmeCmiCplFabCreditWa));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxMcDataParityCheck = %d\n", SysSetup->mem.dfxMemSetup.DfxMcDataParityCheck));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.dfxPartitionDDRTDimm = 0x%x\n", SysSetup->mem.dfxMemSetup.dfxPartitionDDRTDimm));

  for (Index = 0; Index < MAX_SOCKET * MAX_IMC; Index ++) {
    DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.dfxPartitionRatio[%d] = 0x%x\n", Index, SysSetup->mem.dfxMemSetup.dfxPartitionRatio[Index]));
  }

  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.dfxCfgMask2LM = 0x%x\n", SysSetup->mem.dfxMemSetup.dfxCfgMask2LM));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.dfxOptions = 0x%x\n", SysSetup->mem.dfxMemSetup.dfxOptions));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.dfxHighAddrBitStart = 0x%x\n", SysSetup->mem.dfxMemSetup.dfxHighAddrBitStart));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.dfxStraddleHighAddressBit = 0x%x\n", SysSetup->mem.dfxMemSetup.dfxStraddleHighAddressBit));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.dfxLowMemChannel = 0x%x\n", SysSetup->mem.dfxMemSetup.dfxLowMemChannel));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.dfxPerMemMode = 0x%x\n", SysSetup->mem.dfxMemSetup.dfxPerMemMode));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.dfxLoadDimmMgmtDriver = 0x%x\n", SysSetup->mem.dfxMemSetup.dfxLoadDimmMgmtDriver));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxDdrtScheduler = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxDdrtSchedulerDebug));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxCrAcpiDebugInterface = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxCrAcpiDebugInterface));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputRt = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputRt));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputIoLatency = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputIoLatency));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputIoLatencyIoComp = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputIoLatencyIoComp));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputTxDqDelay = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputTxDqDelay));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputTxDqsDelay = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputTxDqsDelay));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputTxDqBitDelay = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputTxDqBitDelay));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputDdrtTxDqsDelay = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputDdrtTxDqsDelay));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputDdrtTxDqBitDelay = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputDdrtTxDqBitDelay));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputRxDqDelay = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputRxDqDelay));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputRxDqsDelay = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputRxDqsDelay));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputRxDqBitDelay = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputRxDqBitDelay));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxInputGnt2Erid = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxInputGnt2Erid));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxCwlAdjPos = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxCwlAdjPos));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxCwlAdjNeg = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxCwlAdjNeg));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxRcvEnDelay = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxRcvEnDelay));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxDdrtRcvEnDelay = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxDdrtRcvEnDelay));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxDdrtRt = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxDdrtRt));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxNfitDebugLogsEnable = %d\n", SysSetup->mem.dfxMemSetup.DfxNfitDebugLogsEnable));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxNfitNvdimmSkuBasedDisable = %d\n", SysSetup->mem.dfxMemSetup.DfxNfitNvdimmSkuBasedDisable));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxNfitMailboxWithPropGuid = %d\n", SysSetup->mem.dfxMemSetup.DfxNfitMailboxWithPropGuid));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxEadrDebugLogsEnable = %d\n", SysSetup->mem.dfxMemSetup.DfxEadrDebugLogsEnable));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxEadrFlowTimeLogsEnable = %d\n", SysSetup->mem.dfxMemSetup.DfxEadrFlowTimeLogsEnable));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxEadrProtocolLogsEnable = %d\n", SysSetup->mem.dfxMemSetup.DfxEadrProtocolLogsEnable));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxFadrDebugLogsEnable = %d\n", SysSetup->mem.dfxMemSetup.DfxFadrDebugLogsEnable));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxFadrFlowTimeLogsEnable = %d\n", SysSetup->mem.dfxMemSetup.DfxFadrFlowTimeLogsEnable));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxDdrEarlyCmpEn = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxDdrEarlyCmpEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxHbmEarlyCmpEn = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxHbmEarlyCmpEn));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxClusterXorEn = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxClusterXorEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxChannelXorEn = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxChannelXorEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxPseudoChannelXorEn = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxPseudoChannelXorEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxDimmXorEn = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxDimmXorEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxRankXorEn = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxRankXorEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxSidXorEn = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxSidXorEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxBankXorEn = 0x%x\n", SysSetup->mem.dfxMemSetup.DfxBankXorEn));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxDdr5RefreshMode = %d\n", SysSetup->mem.dfxMemSetup.DfxDdr5RefreshMode));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.dfxMemSetup.DfxDdr5SsaRxSamplerSelection = %d\n", SysSetup->mem.dfxMemSetup.DfxDdr5SsaRxSamplerSelection));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.volMemMode = 0x%x\n", SysSetup->mem.volMemMode));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.CacheMemType = 0x%x\n", SysSetup->mem.CacheMemType));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.DdrCacheSize = 0x%x\n", SysSetup->mem.DdrCacheSize));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.PmemCaching = 0x%x\n", SysSetup->mem.PmemCaching));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.EadrSupport = 0x%x\n", SysSetup->mem.EadrSupport));

  if (SysSetup->mem.EadrSupport == EADR_ENABLED) {
    DEBUG ((EFI_D_INFO, "SysSetup->mem.EadrCacheFlushMode = 0x%x\n", SysSetup->mem.EadrCacheFlushMode));
  }

  DEBUG ((EFI_D_INFO, "SysSetup->mem.memInterleaveGran1LM = 0x%x\n", SysSetup->mem.memInterleaveGran1LM));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.FadrSupport = 0x%x\n", SysSetup->mem.FadrSupport));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.EnableTwoWayNmCache = 0x%x\n", SysSetup->mem.EnableTwoWayNmCache));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.NonPreferredWayMask = 0x%x\n", SysSetup->mem.NonPreferredWayMask));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.PreferredReadFirst = 0x%x\n", SysSetup->mem.PreferredReadFirst));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.FastZeroMemSupport = 0x%x\n", SysSetup->mem.FastZeroMemSupport));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.DdrtMemPwrSave = 0x%x\n", SysSetup->mem.DdrtMemPwrSave));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.patrolScrubAddrMode = 0x%x\n", SysSetup->mem.patrolScrubAddrMode));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.SrefProgramming = 0x%x\n", SysSetup->mem.SrefProgramming));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.OppSrefEn = 0x%x\n", SysSetup->mem.OppSrefEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MdllOffEn = 0x%x\n", SysSetup->mem.DataDllOff));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MdllOffEn = 0x%x\n", SysSetup->mem.MdllOffEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.PkgcSrefEn = 0x%x\n", SysSetup->mem.PkgcSrefEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.CkMode = 0x%x\n", SysSetup->mem.CkMode));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.CkeProgramming = 0x%x\n", SysSetup->mem.CkeProgramming));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.CkeIdleTimer = 0x%x\n", SysSetup->mem.CkeIdleTimer));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.ApdEn = 0x%x\n", SysSetup->mem.ApdEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.PpdEn = 0x%x\n", SysSetup->mem.PpdEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.DdrtCkeEn = 0x%x\n", SysSetup->mem.DdrtCkeEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.ExtendedADDDCEn = 0x%x\n", SysSetup->mem.ExtendedADDDCEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.Blockgnt2cmd1cyc = 0x%x\n", SysSetup->mem.Blockgnt2cmd1cyc));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.Disddrtopprd = 0x%x\n", SysSetup->mem.Disddrtopprd));
  DEBUG ((EFI_D_INFO, "PMem AveragePower = 0x%x\n", SysSetup->mem.NgnAveragePower));
  DEBUG ((EFI_D_INFO, "PMem AveragePowerLimit = 0x%x\n", SysSetup->mem.DcpmmAveragePowerLimit));
  DEBUG ((EFI_D_INFO, "PMem AveragePowerTimeConstant = 0x%x\n", SysSetup->mem.DcpmmAveragePowerTimeConstant));
  DEBUG ((EFI_D_INFO, "PMem MbbAveragePowerTimeConstant = 0x%x\n", SysSetup->mem.DcpmmMbbAveragePowerTimeConstant));
  DEBUG ((EFI_D_INFO, "PMem MbbFeature = 0x%x\n", SysSetup->mem.DcpmmMbbFeature));
  DEBUG ((EFI_D_INFO, "PMem MbbMaxPowerLimit = 0x%x\n", SysSetup->mem.DcpmmMbbMaxPowerLimit));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.setSecureEraseAllDIMMs = 0x%x\n", SysSetup->mem.setSecureEraseAllDIMMs));

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    for (Channel = 0; Channel < MAX_CH; Channel++) {

      DEBUG ((EFI_D_INFO, "SysSetup->mem.setSecureEraseSktCh[%d][%d] = 0x%x\n", Socket, Channel, SysSetup->mem.setSecureEraseSktCh[Socket][Channel]));

    }

  }
  DEBUG ((EFI_D_INFO, "SysSetup->mem.AppDirectMemoryHole = 0x%x\n", SysSetup->mem.AppDirectMemoryHole));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.LsxImplementation = 0x%x\n", SysSetup->mem.LsxImplementation));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.NfitPublishMailboxStructsDisable = %d\n", SysSetup->mem.NfitPublishMailboxStructsDisable));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.ExtendedType17 = 0x%x\n", SysSetup->mem.ExtendedType17));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.FastGoConfig = 0x%x\n", SysSetup->mem.FastGoConfig));

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Channel = 0; Channel < MAX_CH; Channel++) {
      DEBUG ((EFI_D_INFO, "SysSetup->mem.MemoryTopology[%d][%d] = 0x%x\n", Socket, Channel, SysSetup->mem.MemoryTopology[Socket][Channel]));
    }
  }

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Channel = 0; Channel < MAX_CH; Channel++) {
      DEBUG((EFI_D_INFO, "SysSetup->mem.MemoryConnectorType[%d][%d] = 0x%x\n", Socket, Channel, SysSetup->mem.MemoryConnectorType[Socket][Channel]));
    }
  }

  DEBUG ((EFI_D_INFO, "SysSetup->mem.LatchSystemShutdownState = 0x%x\n", SysSetup->mem.LatchSystemShutdownState));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.EliminateDirectoryInFarMemory = 0x%x\n", SysSetup->mem.EliminateDirectoryInFarMemory));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.NvDimmEnergyPolicy = 0x%x\n", SysSetup->mem.NvDimmEnergyPolicy));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.RxDfeEn = 0x%x\n", SysSetup->mem.RxDfeEn));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.TxRiseFallSlewRate = 0x%x\n", SysSetup->mem.TxRiseFallSlewRate));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.ForcePxcInit = 0x%x\n", SysSetup->mem.ForcePxcInit));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.CmiInitOption = 0x%x\n", SysSetup->mem.CmiInitOption));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.MemBootHealthCheck   = 0x%x\n", SysSetup->mem.MemBootHealthConfig.MemBootHealthCheck));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.ResetOnCriticalError = 0x%x\n", SysSetup->mem.MemBootHealthConfig.ResetOnCriticalError));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.WarningTxDqDelayLeftEdge   = 0x%x\n", SysSetup->mem.MemBootHealthConfig.WarningTxDqDelayLeftEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.WarningTxDqDelayRightEdge  = 0x%x\n", SysSetup->mem.MemBootHealthConfig.WarningTxDqDelayRightEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.WarningTxVrefLeftEdge      = 0x%x\n", SysSetup->mem.MemBootHealthConfig.WarningTxVrefLeftEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.WarningTxVrefRightEdge     = 0x%x\n", SysSetup->mem.MemBootHealthConfig.WarningTxVrefRightEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.WarningRxDqsDelayLeftEdge  = 0x%x\n", SysSetup->mem.MemBootHealthConfig.WarningRxDqsDelayLeftEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.WarningRxDqsDelayRightEdge = 0x%x\n", SysSetup->mem.MemBootHealthConfig.WarningRxDqsDelayRightEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.WarningRxVrefLeftEdge      = 0x%x\n", SysSetup->mem.MemBootHealthConfig.WarningRxVrefLeftEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.WarningRxVrefRightEdge     = 0x%x\n", SysSetup->mem.MemBootHealthConfig.WarningRxVrefRightEdge));

  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.CriticalTxDqDelayLeftEdge   = 0x%x\n", SysSetup->mem.MemBootHealthConfig.CriticalTxDqDelayLeftEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.CriticalTxDqDelayRightEdge  = 0x%x\n", SysSetup->mem.MemBootHealthConfig.CriticalTxDqDelayRightEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.CriticalTxVrefLeftEdge      = 0x%x\n", SysSetup->mem.MemBootHealthConfig.CriticalTxVrefLeftEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.CriticalTxVrefRightEdge     = 0x%x\n", SysSetup->mem.MemBootHealthConfig.CriticalTxVrefRightEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.CriticalRxDqsDelayLeftEdge  = 0x%x\n", SysSetup->mem.MemBootHealthConfig.CriticalRxDqsDelayLeftEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.CriticalRxDqsDelayRightEdge = 0x%x\n", SysSetup->mem.MemBootHealthConfig.CriticalRxDqsDelayRightEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.CriticalRxVrefLeftEdge      = 0x%x\n", SysSetup->mem.MemBootHealthConfig.CriticalRxVrefLeftEdge));
  DEBUG ((EFI_D_INFO, "SysSetup->mem.MemBootHealthConfig.CriticalRxVrefRightEdge     = 0x%x\n", SysSetup->mem.MemBootHealthConfig.CriticalRxVrefRightEdge));

  DEBUG ((EFI_D_INFO, "SysSetup->common.options = 0x%x\n", SysSetup->common.options));

#ifdef DEBUG_CODE_BLOCK

  DEBUG ((EFI_D_INFO, "SysSetup->common.debugJumper = 0x%x\n", SysSetup->common.debugJumper));
  DEBUG ((EFI_D_INFO, "SysSetup->common.serialDebugMsgLvl = 0x%x\n", SysSetup->common.serialDebugMsgLvl));
  DEBUG ((EFI_D_INFO, "SysSetup->common.serialBufEnable = 0x%x\n", SysSetup->common.serialBufEnable));
  DEBUG ((EFI_D_INFO, "SysSetup->common.serialPipeEnable = 0x%x\n", SysSetup->common.serialPipeEnable));
  DEBUG ((EFI_D_INFO, "SysSetup->common.serialPipeCompress = 0x%x\n", SysSetup->common.serialPipeCompress));

#endif // #ifdef DEBUG_CODE_BLOCK

  DEBUG ((EFI_D_INFO, "SysSetup->common.maxAddrMem = 0x%x\n", SysSetup->common.maxAddrMem));
  DEBUG ((EFI_D_INFO, "SysSetup->common.debugPort = 0x%x\n", SysSetup->common.debugPort));
  DEBUG ((EFI_D_INFO, "SysSetup->common.nvramPtr = 0x%x\n", SysSetup->common.nvramPtr));
  DEBUG ((EFI_D_INFO, "SysSetup->common.sysHostBufferPtr = 0x%x\n", SysSetup->common.sysHostBufferPtr));
  DEBUG ((EFI_D_INFO, "SysSetup->common.ddrtXactor = 0x%x\n", SysSetup->common.ddrtXactor));
  DEBUG ((EFI_D_INFO, "SysSetup->common.DfxPpvEnabled = 0x%x\n", SysSetup->common.DfxPpvEnabled));

  for (Index = 0; Index < MAX_B2P_MAILBOX_GROUPS; Index++) {
    DEBUG ((EFI_D_INFO, "SysSetup->common.DfxB2PMailboxCmdEnMask[%d] = 0x%x\n", Index, SysSetup->common.DfxB2PMailboxCmdEnMask[Index]));
  }

  DEBUG ((EFI_D_INFO, "SysSetup->WFRWAEnable = 0x%x\n", SysSetup->WFRWAEnable));
  DEBUG ((EFI_D_INFO, "SysSetup->PmaxDisable = 0x%x\n", SysSetup->PmaxDisable));

  return;

} // DumpSysSetup

/**
  Dump the memory Setup policy to the serial log. Dumps a lot of
  data to the log, so only use when debugging.

  @param[in]  MemSetupPolicyPpi - Ptr to the policy PPI.

  @retval None

**/

VOID
DumpMemorySetupPolicy (
  IN MEMORY_POLICY_PPI *PolicyPpi
  )
{

  DEBUG ((EFI_D_INFO, "\nDumping MEMORY_POLICY_PPI structure\n"));

  DEBUG ((EFI_D_INFO, "MemSetupPolicyPpi->Revision = 0x%x\n", PolicyPpi->Revision));

  DumpSysSetup (PolicyPpi->SysSetup);

  return;

} // DumpMemorySetupPolicy

/**
  Helper function to enable or disable memory flows in the policy.

  @param[in]  Flow          The flow to enable or disable.
  @param[in]  Enable        Enable or disabled.
  @param[out] PolicyPpi     A pointer to the Policy to fill in.

  @retval None

**/

VOID
EFIAPI
SetPolicyMemFlow (
  IN MEMFLOWS Flow,
  IN BOOLEAN Enable,
  OUT SYS_SETUP *Policy
  )
{

  UINT32    *MemFlowField;
  UINT32    FlowBit;

  FlowBit = 0x1 << (Flow % MEMFLOWS_BLOCK_SIZE);

  if (Flow < MEMFLOWS_BLOCK_SIZE) {

    MemFlowField = &(Policy->mem.memFlows);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 2) {

    MemFlowField = &(Policy->mem.memFlowsExt);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 3) {

    MemFlowField = &(Policy->mem.memFlowsExt2);

  } else if (Flow < MEMFLOWS_BLOCK_SIZE * 4) {

    MemFlowField = &(Policy->mem.memFlowsExt3);

  } else {
    ASSERT (FALSE);
  }

  if (Enable == TRUE) {
    *MemFlowField |= FlowBit;
  } else {
    *MemFlowField &= ~FlowBit;
  }

  return;

} // SetPolicyMemFlow

/**

  Set the mem flows default values in
  the memory policy.

  @param[in]   CpuType         The cpu type.
  @param[out]  Policy          A pointer to policy to fill in.

  @retval None

**/

VOID
SetMemFlowsDefaults (
  OUT SYS_SETUP *Policy
  )
{
  BOOLEAN Is14nmCpu;

  Is14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);

  //
  // First set the basic defaults
  //

  Policy->mem.memFlows      = MEM_FLOWS_DEFAULT;
  Policy->mem.memFlowsExt   = MEM_FLOWS_EXT_DEFAULT;
  Policy->mem.memFlowsExt2  = MEM_FLOWS_EXT2_DEFAULT;
  Policy->mem.memFlowsExt3  = MEM_FLOWS_EXT3_DEFAULT;

  //
  // Now set the defaults for individual flows in memFlows.
  //

  SetPolicyMemFlow (TxEqTraining, TX_EQ_DEFAULT, Policy);

  //
  // IMode training
  //

  SetPolicyMemFlow (ImodeTraining, FixedPcdGet8 (PcdMrcImodeTrainingEnable), Policy);

  //
  // Round Trip Latency
  //

  SetPolicyMemFlow (RoundTripOptimizeTraining, FixedPcdGet8 (PcdMrcRoundTripOptimizationTrainingEnable), Policy);

  //
  // Read Vref Training
  //

  SetPolicyMemFlow (EarlyReadVrefCenteringTraining, RX_VREF_DEFAULT, Policy);



  //
  // Write Vref Training
  //

  SetPolicyMemFlow (EarlyWriteVrefCenteringTraining, TX_VREF_DEFAULT, Policy);

  //
  //  LRDIMM WR VREF
  //

  SetPolicyMemFlow (LrdimmWriteVrefCenteringTraining, LRDIMM_WR_VREF_DEFAULT, Policy);

  //
  //  LRDIMM RD VREF
  //

  SetPolicyMemFlow (LrdimmReadVrefCenteringTraining, LRDIMM_RD_VREF_DEFAULT, Policy);

  //
  // Command Vref Training
  //

  SetPolicyMemFlow (CmdVrefCenteringTraining, FixedPcdGet8 (PcdMrcCmdVrefCenteringTrainingEnable), Policy);


  SetPolicyMemFlow (PhaseCrosstalkTraining, FixedPcdGet8 (PcdMrcPxcDefault), Policy);

  SetPolicyMemFlow (TxDdjcTraining, FixedPcdGet8 (PcdMrcDdjcDefault), Policy);

  //
  // CMD TxEq Calibration
  //

  if (Is14nmCpu) {

    SetPolicyMemFlow (CmdTxEqTraining, CMD_TX_EQ_DEFAULT_SKX, Policy);

  } else {

    SetPolicyMemFlow (CmdTxEqTraining, CMD_TX_EQ_DEFAULT_10NM, Policy);

  }

  //
  // CMD Normalization
  //

  SetPolicyMemFlow (CmdNormalization, FixedPcdGet8 (PcdMrcCmdNormalizationTrainingEnable), Policy);

  //
  // DRAM RON Training
  //

  SetPolicyMemFlow (DramROnTraining, FixedPcdGet8 (PcdMrcDramRonDefault), Policy);

  //
  // RX ODT Training
  //

  SetPolicyMemFlow (McOdtTraining, FixedPcdGet8 (PcdMrcMcOdtDefault), Policy);


  //
  // MC RON Training
  //

  SetPolicyMemFlow (McROnTraining, FixedPcdGet8 (PcdMrcMcRonDefault), Policy);

  //
  // RX CTLE Training
  //

  SetPolicyMemFlow (RxCtleTraining, FixedPcdGet8 (PcdMrcCtleTrainingEnable), Policy);

  //
  // LRDIMM Backside Read DQ Centering
  //

  SetPolicyMemFlow (LrdimmBacksideDqTraining, LRDIMM_RX_DQ_CENTERING_DEFAULT, Policy);

  //
  // LRDIMM Backside Write DQ Centering
  //

  SetPolicyMemFlow (LrdimmBacksideDqTraining, LRDIMM_TX_DQ_CENTERING_DEFAULT, Policy);

  //
  // RX DFE
  //
  SetPolicyMemFlow (
    RxDfeDdr5Training,
    (FixedPcdGet8 (PcdMrcRxDfeDdr5TrainingDefault) == RX_DFE_DDR5_TRAINING_ENABLE),
    Policy
    );

  //
  // TX DFE
  //
  SetPolicyMemFlow (
    TxDfeDdr5Training,
    (FixedPcdGet8 (PcdMrcTxDfeDdr5TrainingDefault) == TX_DFE_DDR5_TRAINING_ENABLE),
    Policy
    );

  //
  // LRDIMM DB DFE training
  //
  if (FixedPcdGet8 (PcdMrcLrdimmDbDfeDefault) != 0) {
    SetPolicyMemFlow (LrdimmDbDfe, TRUE, Policy);
  } else {
    SetPolicyMemFlow (LrdimmDbDfe, FALSE, Policy);
  }

  //
  // DRAM Duty Cycle Adjuster Training
  //

  SetPolicyMemFlow (DramDutyCycleAdjusterTraining, FixedPcdGet8 (PcdMrcDramDutyCycleAdjusterTrainingDefault), Policy);

  return;

} // SetMemFlowsDefaults

/**
  Set the memory policy default values.

  Set the default values for the MRC. UpdatePeiMemoryPolicy will update
  these default values via Setup variables. Default values come from MemDefaults.h

  @param[out]  PolicyPpi          A pointer to PPI to fill in.

  @retval EFI_SUCCESS             Success
  @retval EFI_INVALID_PARAMETER   Failure

**/

EFI_STATUS
EFIAPI
SetMemoryPolicyDefaults (
  OUT MEMORY_POLICY_PPI *PolicyPpi
  )
{
  UINT8  Socket;
  UINT8  Channel;
  UINT8  Index;
  SYS_SETUP *Setup;
  BOOLEAN Is14nmCpu;

  Is14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);

  //
  // Set the revision
  //

  PolicyPpi->Revision = MEMORY_POLICY_PPI_REVISION;

  //
  // Assign our Setup pointer.
  //

  Setup = PolicyPpi->SysSetup;

  Setup->common.options = PROMOTE_WARN_EN | PROMOTE_MRC_WARN_EN | HALT_ON_ERROR_EN;

  //
  // Set all the mem flows defaults
  //

  SetMemFlowsDefaults (Setup);


  if (HybridSystemLevelEmulationEnabled ()) {
    Setup->common.options = HALT_ON_ERROR_EN;
  }

  Setup->common.debugPort  = DEFAULT_COM_PORT;

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    Setup->common.maxAddrMem = MAX_MEM_ADDR_SPR;
  } else if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    Setup->common.maxAddrMem = MAX_MEM_ADDR_ICX;
  } else if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    Setup->common.maxAddrMem = MAX_MEM_ADDR_SNR;
  } else {
    Setup->common.maxAddrMem = MAX_MEM_ADDR;
  }

  Setup->common.sysHostBufferPtr = 0;      // zero:  Use default memory buffer address at 1MB
                                            // non-zero : OEM-specified 32-bit buffer address
#ifdef DEBUG_CODE_BLOCK
  //
  // For Simulations (RcSim, CteSim, and KtiSim only, since the Setup
  // pointer isn't filled in from default file for simulation),
  // use default debug level at build time configuration
  //
  Setup->common.serialDebugMsgLvl = FixedPcdGet32 (PcdRcDebugBuildTimeDefault);
#endif  // DEBUG_CODE_BLOCK

  for (Index = 0; Index < MAX_B2P_MAILBOX_GROUPS; Index++) {
    Setup->common.DfxB2PMailboxCmdEnMask[Index] = 0xFF;
  }

  Setup->common.DfxPpvEnabled = DFX_PPV_AUTO;

  Setup->common.SocketConfig = SOCKET_UNDEFINED;

  Setup->mem.enforcePOR = FixedPcdGet8 (PcdEnforcePorDefault);
  Setup->mem.EnforcePopulationPor = FixedPcdGet8 (PcdEnforcePopulationPorDefault);
  Setup->mem.TempRefreshOption = FixedPcdGet8 (PcdTempRefreshOption);
  Setup->mem.PanicWm = FixedPcdGet8 (PcdMrcPanicWatermarkDefault);

  Setup->mem.HalfxRefreshValue = FixedPcdGet8 (PcdHalfxRefreshValue);
  Setup->mem.TwoxRefreshValue = FixedPcdGet8 (PcdTwoxRefreshValue);
  Setup->mem.FourxRefreshValue = FixedPcdGet8 (PcdFourxRefreshValue);


  //
  // Stagger Host Refresh options
  //
  Setup->mem.TrefiPerChannel = 0;
  Setup->mem.TrefiNumofRank = 1;
  Setup->mem.TrefiDelay = 0;

  //
  // Initialize PPR options
  //
  Setup->mem.pprType       = PPR_TYPE_DEFAULT;
  Setup->mem.EnableTapSweep = 0;

  Setup->mem.DfeGainBias = 0;

  Setup->mem.Tap1Start  = -40;
  Setup->mem.Tap1End    = 10;
  Setup->mem.Tap1Size   = 1;

  Setup->mem.Tap2Start  = -15;
  Setup->mem.Tap2End    = 15;
  Setup->mem.Tap2Size   = 1;

  Setup->mem.Tap3Start  = -12;
  Setup->mem.Tap3End    = 12;
  Setup->mem.Tap3Size   = 1;

  Setup->mem.Tap4Start  = -9;
  Setup->mem.Tap4End    = 9;
  Setup->mem.Tap4Size   = 1;

  //
  //  ddrFreqLimit
  //    0:Auto
  //    1:Force DDR-800
  //    2:Force DDR-1066
  //    3:Force DDR-1333
  //    4:Force DDR-1600
  //    5:Force DDR-1866
  //    6:Force DDR-2133
  //   19:Force DDR-3200
  //
  Setup->mem.ddrFreqLimit = FixedPcdGet8 (PcdMrcDdrFrequencyDefault);
  //
  // common options
  //
  Setup->common.options &= ~(PROMOTE_MRC_WARN_EN | HALT_ON_ERROR_EN | PROMOTE_WARN_EN | HALT_ON_ERROR_AUTO);

  if (PROMOTE_MRC_WARN_DEFAULT == PROMOTE_MRC_WARN_ENABLE) {
    Setup->common.options |= PROMOTE_MRC_WARN_EN;
  }

  if (PROMOTE_WARN_DEFAULT == PROMOTE_WARN_ENABLE) {
    Setup->common.options |= PROMOTE_WARN_EN;
  }

  if (HALT_ON_ERROR_DEFAULT == HALT_ON_ERROR_ENABLE) {
    Setup->common.options |= HALT_ON_ERROR_EN;
  }

#ifdef DEBUG_CODE_BLOCK
  if (HALT_ON_ERROR_DEFAULT == HALT_ON_ERROR_AUTO_RESET) {
    Setup->common.options |= HALT_ON_ERROR_AUTO;
  }
#endif //DEBUG_CODE_BLOCK

  Setup->mem.options = 0;

  //
  // Multi-Threaded MRC
  //

  if (FixedPcdGetBool (PcdMrcMultiThreadedDefault) == TRUE) {
    Setup->mem.options |= MULTI_THREAD_MRC_EN;
  } else {
    Setup->mem.options &= ~MULTI_THREAD_MRC_EN;
  }

  // Force ODT ON
  Setup->mem.dfxMemSetup.DfxForceOdtOn = FORCE_ODT_ON_DEFAULT;

  // ECC Support
  if (ECC_DEFAULT == ECC_ENABLE) {
    Setup->mem.dfxMemSetup.dfxOptions |= ECC_CHECK_EN;
  }

  // Setup option for memory test
  if (MEM_TEST_DEFAULT == MEM_TEST_ENABLE) {
    Setup->mem.options |= MEMORY_TEST_EN;
  }

  //
  // Memory test loops
  //
  Setup->mem.memTestLoops = MEM_TEST_LOOPS_DEFAULT;


  //
  //  dimmTypeSupport
  //    0:RDIMM
  //    1:UDIMM
  //    2:RDIMM and UDIMM
  //
  Setup->mem.dimmTypeSupport = DDR_TYPE_DEFAULT;

  //
  // Receive Enable Average Feature
  //
  Setup->mem.RecEnDelayAverage = FixedPcdGet8 (PcdRankSwitchFixOption);

  //
  //Legacy Rank Margin Tool
  //
  if (FixedPcdGet8 (PcdLegacyRmtEnable) == LEGACY_RMT_ENABLE) {
    Setup->mem.options |= MARGIN_RANKS_EN;
  } else {
    Setup->mem.options &= ~MARGIN_RANKS_EN;
  }
#ifdef LRDIMM_SUPPORT
  //
  // Backside Rank Margin Tool
  //

  if (PcdGetBool (EnableBacksideRmt)) {
    Setup->mem.enableBacksideRMT = BACKSIDE_RMT_ENABLE;
  } else {
    Setup->mem.enableBacksideRMT = BACKSIDE_RMT_DISABLE;
  }
  //
  // Backside CMD Rank Margin Tool
  //
  if (PcdGetBool (EnableBacksideCmdRmt)) {
    Setup->mem.enableBacksideCMDRMT = BACKSIDE_CMD_RMT_ENABLE;
  } else {
    Setup->mem.enableBacksideCMDRMT = BACKSIDE_CMD_RMT_DISABLE;
  }
#endif  //LRDIMM_SUPPORT

  Setup->mem.rmtPatternLength = PcdGet32 (RmtPatternLengthCmd);
  Setup->mem.rmtPatternLengthExt = PcdGet32 (RmtPatternLengthExtCmdCtlVref);

  if (PcdGetBool (PerBitMargin)) {
    Setup->mem.optionsExt |= PER_BIT_MARGINS;
  } else {
    Setup->mem.optionsExt &= ~PER_BIT_MARGINS;
  }

  if (PcdGetBool (PcdEnableNgnBcomMargining)) {
    Setup->mem.enableNgnBcomMargining = NGN_BCOM_MARGINING_ENABLE;
  } else {
    Setup->mem.enableNgnBcomMargining = NGN_BCOM_MARGINING_DISABLE;
  }

  // Setup option for fast boot
  if (FixedPcdGet8 (PcdMrcFastBootDefault) == FAST_BOOT_ENABLE) {
    Setup->mem.options |= ATTEMPT_FAST_BOOT;
  }

  // Setup option for fast cold boot
  if (FixedPcdGetBool (PcdMrcFastColdBootDefault) == TRUE) {
    Setup->mem.options |= ATTEMPT_FAST_BOOT_COLD;
  }

  // Setup option for memory test when fast boot
  if (MEM_TEST_COLD_FAST_BOOT_DEFAULT == MEM_TEST_COLD_FAST_BOOT_ENABLE) {
    Setup->mem.options |= MEMORY_TEST_COLD_FAST_BOOT_EN;
  }

  // Setup option for RMT on Fast cold boot

  if (Is14nmCpu) {

    if (RMT_COLD_FAST_BOOT_DEFAULT_SKX == RMT_COLD_FAST_BOOT_ENABLE) {
      Setup->mem.options |= RMT_COLD_FAST_BOOT;
    }

  } else {

    if (RMT_COLD_FAST_BOOT_DEFAULT_10NM == RMT_COLD_FAST_BOOT_ENABLE) {
      Setup->mem.options |= RMT_COLD_FAST_BOOT;
    }

  }

  if (SCRAMBLE_DDRT_DEFAULT == SCRAMBLE_DDRT_ENABLE) {
    Setup->mem.options |= SCRAMBLE_EN_DDRT;
  }

  //
  // Data Scrambling
  //
  if (SCRAMBLE_DEFAULT == SCRAMBLE_ENABLE) {
    Setup->mem.options |= SCRAMBLE_EN; // ddr4
  }

  //
  // Allow SBE during memory training
  //
  if (ALLOW_CORRECTABLE_ERROR_DEFAULT == ALLOW_CORRECTABLE_ERROR_ENABLE) {
    Setup->mem.optionsExt |= ALLOW_CORRECTABLE_ERROR;
  } else {
    Setup->mem.optionsExt &= ~ALLOW_CORRECTABLE_ERROR;
  }

  //
  // Allow correctable error during Memtest
  //
  if (ALLOW_CORRECTABLE_MEM_TEST_ERROR_DEFAULT == ALLOW_CORRECTABLE_MEM_TEST_ERROR_ENABLE) {
    Setup->mem.optionsExt |= ALLOW_CORRECTABLE_MEM_TEST_ERROR;
  } else {
    Setup->mem.optionsExt &= ~ALLOW_CORRECTABLE_MEM_TEST_ERROR;
  }

  //
  // Early Write CRC for the DDRIO
  //
  if (EARLY_WR_CRC_DDRIO_DEFAULT == WR_CRC_ENABLE) {
    Setup->mem.optionsExt |= EARLY_WR_CRC_DDRIO;
  } else {
    Setup->mem.optionsExt &= ~EARLY_WR_CRC_DDRIO;
  }

  //
  // Late Write CRC for the DDRIO, MC and DRAM
  //
  if (WR_CRC_DEFAULT == WR_CRC_ENABLE) {
    Setup->mem.optionsExt |= WR_CRC;
  } else {
    Setup->mem.optionsExt &= ~WR_CRC;
  }

  //
  // DIMM ISOLATION
  //
  if (FixedPcdGet8 (PcdDimmIsolationDefault) == DIMM_ISOLATION_ENABLE) {
    Setup->mem.optionsExt |= DIMM_ISOLATION_EN;
  } else {
    Setup->mem.optionsExt &= ~DIMM_ISOLATION_EN;
  }

  //
  // Read / Write 2TCLK preamble
  //

  Setup->mem.writePreamble = FixedPcdGet8 (PcdMrcWritePreambleTclkDefault);
  Setup->mem.readPreamble = FixedPcdGet8 (PcdMrcReadPreambleTclkDefault);

  Setup->mem.scrambleSeedLow = SCRAMBLE_SEED_LOW;
  Setup->mem.scrambleSeedHigh = SCRAMBLE_SEED_HIGH;

  Setup->mem.ADREn = ADR_DEFAULT;
  Setup->mem.check_pm_sts = CHECK_PM_STS_DEFAULT;
  Setup->mem.check_platform_detect = CHECK_PLATFORM_DETECT_DEFAULT;

  if (CUSTOM_REFRESH_RATE_DEFAULT == CUSTOM_REFRESH_RATE_ENABLE) {
    Setup->mem.customRefreshRate = CUST_REFRESH_RATE_DEFAULT;
    if (!Is14nmCpu) {
      //
      // This sets temp_twoxref = 0 in order to drive 2x refresh by HW, and then modify t_refi
      // starting at 2x as base
      //
      Setup->mem.TwoxRefreshValue = 0;
    }
  } else {
    Setup->mem.customRefreshRate = 0;
  }

  //
  // DLL Reset Test
  //
  Setup->mem.dllResetTestLoops = DLL_RESET_TEST_LOOPS_DEFAULT;

  // Enable WMM Opportunistic Reads
  if (OPP_RD_WMM_DEFAULT == OPP_RD_WMM_ENABLE) {
    Setup->mem.options &= ~DISABLE_WMM_OPP_READ;
  }

  //
  // Normal operation interval.
  //
  Setup->mem.normOppIntvl = NORMAL_OPPERATION_INTERVAL_DEFAULT;

  //
  // C/A Parity
  //
  if (CA_PARITY_DEFAULT == CA_PARITY_ENABLE) {
    Setup->mem.dfxMemSetup.dfxOptions |= CA_PARITY_EN;
  } else {
    Setup->mem.dfxMemSetup.dfxOptions &= ~CA_PARITY_EN;
  }

  //
  // DDR4/DDR5 SMBus Speed
  //
  Setup->mem.SpdSmbSpeed = (SMB_CLOCK_FREQUENCY) FixedPcdGet8 (PcdMrcSmbusSpeedDefault);

  //
  // SPD Print
  //
  Setup->mem.SpdPrintEn = SPD_PRINT_DIS;
  if (FixedPcdGetBool (PcdMrcSpdPrintDefault)) {
    Setup->mem.SpdPrintEn = SPD_PRINT_EN;
  }

  Setup->mem.SpdPrintLength = MAX_SPD_BYTE_DDR;

  if (DDR_CYCLING_DEFAULT == DDR_CYCLING_ENABLE) {
    Setup->mem.options |= DDR_RESET_LOOP;
  } else {
    Setup->mem.options &= ~DDR_RESET_LOOP;
  }



  // POR values
  Setup->mem.Blockgnt2cmd1cyc = BLOCK_GNT2CMD1CYC_DEFAULT;
  Setup->mem.Disddrtopprd = DIS_NVMDIMM_OPPRDC_DEFAULT;

  //
  // Rank Disable Mask
  //
  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Channel = 0; Channel < MAX_CH; Channel++) {
      Setup->mem.socket[Socket].ddrCh[Channel].rankmask = RANK_ENABLE_DEFAULT;
    }
  }

  // Page mode
  if (CLOSED_PAGE_DEFAULT == CLOSED_PAGE_EN) {
    // Closed
    Setup->mem.options |= PAGE_POLICY;
    Setup->mem.options &= ~ADAPTIVE_PAGE_EN;
  } else {
    // Open Adaptive
    Setup->mem.options &= ~PAGE_POLICY;
    Setup->mem.options |= ADAPTIVE_PAGE_EN;
  }



  //
  // Rx DFE options
  //
  Setup->mem.RxDfeEn = FixedPcdGet8 (PcdMrcRxDfeDefault);

  //
  // TX Rise/Fall Slew Rate options
  //
  Setup->mem.TxRiseFallSlewRate = FixedPcdGet8 (PcdMrcTxRfSlewRateDefault);

  // CMI Init mode
  if (FixedPcdGetBool (PcdMrcCmiInitUseResetDefault) == TRUE) {
    Setup->mem.CmiInitOption = CMI_INIT_RESET;
  } else {
    Setup->mem.CmiInitOption = CMI_INIT_AUTO;
  }

  //
  // Tco Comp training
  //
  if (FixedPcdGet8 (PcdMrcTcoCompTrainingEnable) == TCO_COMP_ENABLE) {
    Setup->mem.optionsExt |= TCO_COMP_EN;
  } else {
    Setup->mem.optionsExt &= ~TCO_COMP_EN;
  }

  //
  // Duty Cycle Training
  //
  if (DUTY_CYCLE_DEFAULT == DUTY_CYCLE_ENABLE) {
    Setup->mem.optionsExt |= DUTY_CYCLE_EN;
  } else {
    Setup->mem.optionsExt &= ~DUTY_CYCLE_EN;
  }

  //
  // Display Eye Diagrams
  //
  if (EYE_DIAGRAM_DEFAULT == EYE_DIAGRAM_ENABLE) {
    Setup->mem.options |= DISPLAY_EYE_EN;
  } else {
    Setup->mem.options &= ~DISPLAY_EYE_EN;
  }

  //
  // Turnaround Time Optimization
  //
  if (FixedPcdGetBool (PcdMrcTurnaroundOptimizations) == TURNAROUND_ENABLE) {
    Setup->mem.optionsExt |= TURNAROUND_OPT_EN;
  } else {
    Setup->mem.optionsExt &= ~TURNAROUND_OPT_EN;
  }

  //
  // Turnaround Time Optimization
  //
  if (FixedPcdGetBool (PcdMrcTurnaroundOptimizationsDdrt) == TURNAROUND_DDRT_ENABLE) {
    Setup->mem.optionsExt |= TURNAROUND_OPT_EN_DDRT;
  } else {
    Setup->mem.optionsExt &= ~TURNAROUND_OPT_EN_DDRT;
  }

  //
  // Enable/Disable NVMCTRL Media status as exception
  //
  Setup->mem.NvmMediaStatusException = NVMCTRL_MEDIA_STATUS_EXCEPTION_DEFAULT;

  //
  // Enable/Disable not supported DCPMM as exception
  //
  Setup->mem.AepNotSupportedException = AEP_DIMM_NOT_SUPPORTED_EXCEPTION_DEFAULT;

  //
  // One Rank Timing Mode Option
  //
  Setup->mem.oneRankTimingModeEn = ONE_RANK_TIMING_MODE_DEFAULT;

  //
  // PDA
  //
  if (PDA_DEFAULT == PDA_ENABLE) {
    Setup->mem.optionsExt |= PDA_EN;
  } else {
    Setup->mem.optionsExt &= ~PDA_EN;
  }

  //
  // PDA behavior for x16 devices
  //
  Setup->mem.PdaModeX16    = DEFAULT_PDA_MODE_X16;

  //
  // Thermal throttling options
  //
  Setup->mem.thermalThrottlingOptions &= ~(OLTT_EN | CLTT_EN | MH_OUTPUT_EN | MH_SENSE_EN | CLTT_PECI_EN);

  if (THERMAL_THROT_DEFAULT == CLTT_ENABLE) {
    Setup->mem.thermalThrottlingOptions |= CLTT_EN;
  } else if (THERMAL_THROT_DEFAULT == OLTT_ENABLE) {
    Setup->mem.thermalThrottlingOptions |= OLTT_EN;
  } else if (THERMAL_THROT_DEFAULT == CLTT_PECI_ENABLE) {
    Setup->mem.thermalThrottlingOptions |= CLTT_PECI_EN;
  }


  //
  // thermal memtrip options (Memtrip to ThermTrip)
  //
  Setup->mem.thermalThrottlingOptions &= ~OFFPKG_MEM_TO_THERMTRIP_EN;

  if (OFFPKG_MEM_TO_THERMTRIP_DEFAULT == OFFPKG_MEM_TO_THERMTRIP_ENABLE) {
    Setup->mem.thermalThrottlingOptions |= OFFPKG_MEM_TO_THERMTRIP_EN;
  }

  //
  // thermal memtrip options (Memtrip to Memtrip tree)
  //
  Setup->mem.thermalThrottlingOptions &= ~OFFPKG_MEM_TO_MEMTRIP_EN;

  if (OFFPKG_MEM_TO_MEMTRIP_DEFAULT == OFFPKG_MEM_TO_MEMTRIP_ENABLE) {
    Setup->mem.thermalThrottlingOptions |= OFFPKG_MEM_TO_MEMTRIP_EN;
  }

  //
  // thermal memtrip options (Memtrip to ThermTrip)
  //
  Setup->mem.thermalThrottlingOptions &= ~INPKG_MEM_TO_THERMTRIP_EN;

  if (INPKG_MEM_TO_THERMTRIP_DEFAULT == INPKG_MEM_TO_THERMTRIP_ENABLE) {
    Setup->mem.thermalThrottlingOptions |= INPKG_MEM_TO_THERMTRIP_EN;
  }

  //
  // thermal memtrip options (Memtrip to Memtrip tree)
  //
  Setup->mem.thermalThrottlingOptions &= ~INPKG_MEM_TO_MEMTRIP_EN;

  if (INPKG_MEM_TO_MEMTRIP_DEFAULT == INPKG_MEM_TO_MEMTRIP_ENABLE) {
    Setup->mem.thermalThrottlingOptions |= INPKG_MEM_TO_MEMTRIP_EN;
  }
  //
  // MEMHOT options
  //
  if (MEMHOT_DEFAULT == MEMHOT_OUTPUT_ONLY) {
    Setup->mem.thermalThrottlingOptions |= MH_OUTPUT_EN;
  } else if (MEMHOT_DEFAULT == MEMHOT_INPUT_ONLY) {
    Setup->mem.thermalThrottlingOptions |= MH_SENSE_EN;
  } else if (MEMHOT_DEFAULT == MEMHOT_INPUT_OUTPUT) {
    Setup->mem.thermalThrottlingOptions |= (MH_OUTPUT_EN + MH_SENSE_EN);
  }

  //
  // MEMHOT Output Only options
  //
  if (MEMHOT_OUTPUT_ONLY_DEFAULT == MEMHOT_OUTPUT_ONLY_DIS) {
    Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_DIS;
  } else if(MEMHOT_OUTPUT_ONLY_DEFAULT == MEMHOT_OUTPUT_ONLY_HIGH) {
    Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH;
  } else if(MEMHOT_OUTPUT_ONLY_DEFAULT == MEMHOT_OUTPUT_ONLY_HIGH_MED) {
    Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH_MED;
  } else if(MEMHOT_OUTPUT_ONLY_DEFAULT == MEMHOT_OUTPUT_ONLY_HIGH_MED_LOW) {
    Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH_MED_LOW;
  } else {
    Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH; //Default = high
  }

  //
  // THRTMID on TEMPLO
  //
  Setup->mem.ThrottlingMidOnTempLo = FixedPcdGet8 (PcdMrcThrottlingMidOnTempLo);

  Setup->mem.CkeProgramming = CKE_DEFAULT;
  Setup->mem.SrefProgramming = SREF_PROG_DEFAULT;
  Setup->mem.PkgcSrefEn = PKGC_SREF_DEFAULT;;
  Setup->mem.CkeIdleTimer = CKE_IDLE_TIMER_DEFAULT;
  Setup->mem.ApdEn = APD_DEFAULT;
  Setup->mem.PpdEn = PPD_DEFAULT;
  Setup->mem.DdrtCkeEn = DDRT_CKE_DEFAULT;
  Setup->mem.OppSrefEn = PcdGet8 (PcdOpportunisticSelfRefreshDefault);
  Setup->mem.DataDllOff = DATA_DLL_OFF_DEFAULT;
  Setup->mem.MdllOffEn = MDLL_OFF_DEFAULT;
  Setup->mem.CkMode = CK_SELF_REF_DEFAULT;

  if (PcdGetBool (PcdMrcXmpSupport)) {

    //
    //    MEM_OVERRIDE_EN
    //
    Setup->mem.options &= ~MEM_OVERRIDE_EN;

    Setup->mem.inputMemTime.vdd = VDD_DEFAULT;

    Setup->mem.inputMemTime.nCMDRate = CMD_TIMING_DEFAULT;
    Setup->mem.inputMemTime.tREFI = REFRESH_RATE_DEFAULT;
    Setup->mem.inputMemTime.nCL = CAS_LATENCY_DEFAULT;
    Setup->mem.inputMemTime.nRP = TRP_LATENCY_DEFAULT;
    Setup->mem.inputMemTime.nRCD = TRCD_LATENCY_DEFAULT;
    Setup->mem.inputMemTime.nRAS = TRAS_LATENCY_DEFAULT;
    Setup->mem.inputMemTime.nWR = TWR_LATENCY_DEFAULT;
    Setup->mem.inputMemTime.nRFC = TRFC_LATENCY_DEFAULT;
    Setup->mem.inputMemTime.nRRD = TRRD_LATENCY_DEFAULT;
    Setup->mem.inputMemTime.nRTP = TRTP_LATENCY_DEFAULT;
    Setup->mem.inputMemTime.nWTR = TWTR_LATENCY_DEFAULT;
    Setup->mem.inputMemTime.nFAW = TFAW_LATENCY_DEFAULT;
    Setup->mem.inputMemTime.nRC = TRC_LATENCY_DEFAULT;
    Setup->mem.inputMemTime.nCWL = TCWL_LATENCY_DEFAULT;
  }

  if (IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    Setup->mem.volMemMode = VOL_MEM_MODE_1LM;
  }
  // Check whether this is need for wave-1
/*  else {
    //Setup->mem.volMemMode = VOL_MEM_MODE_2LM;
  }
*/
  Setup->mem.DdrCacheSize = DDR_ENCODED_CACHE_SIZE_DEFAULT;

  Setup->mem.PmemCaching = PMEM_WBCACHE_DISABLED;
  Setup->mem.EadrSupport = EADR_DISABLED;
  Setup->mem.EadrCacheFlushMode = EADR_FLUSH_SERIAL;

  //
  // fADR setup default
  //
  Setup->mem.FadrSupport = FADR_DEFAULT;

  //
  // CR Mixed SKU
  //
  if (CR_MIXED_SKU_DEFAULT == CR_MIXED_SKU_EN) {
    Setup->mem.dfxMemSetup.dfxOptions |= CR_MIXED_SKU;
  } else {
    Setup->mem.dfxMemSetup.dfxOptions &= ~CR_MIXED_SKU;
  }

  // Interleave and NUMA
  if (NUMA_DEFAULT == NUMA_EN) {
    Setup->mem.options |= NUMA_AWARE;
  } else {
    Setup->mem.options &= ~NUMA_AWARE;
  }

  Setup->mem.memInterleaveGran1LM = MEM_INT_GRAN_1LM_DEFAULT;
  //
  // IMC interleave
  //    0:Auto
  //    1:1-way IMC interleave
  //    2:2-way IMC interleave
  Setup->mem.imcInter = IMC_INTER_DEFAULT;

  //
  //  chInter
  //    0:Auto
  //    1:1-way channel interleave
  //    2:2-way channel interleave
  //    3:3-way channel interleave
  //    4:4-way channel interleave
  //
  Setup->mem.chInter = CH_INTER_DEFAULT;

  //
  // DfxMod3Support = MOD3_SUPPORT_ENABLE
  // This knob is exposed in HFR but not directly a policy input and results in overriding the chInter to 3 in all programs.
  // if this is desired behavior we need to update code to handle.  Setting default here does not resolve as platform code
  // sets chInter = MAX_MC_CH when Dfx not present
  //
  // Setup->mem.chInter = 3;

  //
  //  chInter
  //    1:1-way rank interleave
  //    2:2-way rank interleave
  //    4:4-way rank interleave
  //    8:8-way rank interleave
  //

  if (Is14nmCpu) {
    Setup->mem.dfxMemSetup.dfxRankInter = RANK_4WAY;
  } else {
    Setup->mem.dfxMemSetup.dfxRankInter = RANK_8WAY;
  }

  Setup->mem.NgnAveragePower = NGN_AVERAGE_POWER_DEFAULT;

  //
  // FIS 2.x
  //
  Setup->mem.DcpmmAveragePowerLimit = AVERAGE_POWER_LIMIT_DEFAULT;
  Setup->mem.DcpmmAveragePowerTimeConstant = AVERAGE_POWER_TIME_CONSTANT_DEFAULT_2_0;
  Setup->mem.DcpmmMbbAveragePowerTimeConstant = AVERAGE_POWER_TIME_CONSTANT_DEFAULT_2_1_OR_LATER;
  Setup->mem.DcpmmMbbFeature = TURBO_MODE_STATE_DEFAULT;
  Setup->mem.DcpmmMbbMaxPowerLimit = AVERAGE_POWER_LIMIT_MAX;

  //
  // Lock NGN CSRs
  //
  if (LOCK_NGN_CSR_DEFAULT == LOCK_NGN_CSR_DISABLE) {
    Setup->mem.dfxMemSetup.dfxOptions |= LOCK_NGN_CSR_DIS;
  } else {
    Setup->mem.dfxMemSetup.dfxOptions &= ~LOCK_NGN_CSR_DIS;
  }

  //
  // NGN options
  //
  if (NGN_DEBUG_LOCK_DEFAULT == NGN_DEBUG_LOCK_ENABLE) {
    Setup->mem.optionsNgn |= NGN_DEBUG_LOCK;
  } else {
    Setup->mem.optionsNgn &= ~NGN_DEBUG_LOCK;
  }

  if (NGN_CMD_TIME_DEFAULT == NGN_CMD_TIME_AUTO) {
    Setup->mem.optionsNgn &= ~NGN_CMD_TIME;
  } else if (NGN_CMD_TIME_DEFAULT == NGN_CMD_2N) {
    Setup->mem.optionsNgn |= NGN_CMD_TIME;
  } else {
    Setup->mem.optionsNgn &= ~NGN_CMD_TIME;
  }

  if (NGN_ECC_CORR_DEFAULT == NGN_ECC_CORR_ENABLE) {
    Setup->mem.dfxMemSetup.dfxOptions |= NGN_ECC_CORR;
  } else {
    Setup->mem.dfxMemSetup.dfxOptions &= ~NGN_ECC_CORR;
  }

  //
  // NgnEccCorr = NGN_ECC_CORR_ENABLE,
  // This knob is exposed in HFR but HFR does not use the NGN_ECC_CORR_DEFAULT so there is a mismatch between Dfx enabled and disabled bios
  //
  Setup->mem.dfxMemSetup.dfxOptions |= NGN_ECC_CORR;


  if (NGN_ECC_WR_CHK_DEFAULT == NGN_ECC_WR_CHK_ENABLE) {
    Setup->mem.dfxMemSetup.dfxOptions |= NGN_ECC_WR_CHK;
  } else {
    Setup->mem.dfxMemSetup.dfxOptions &= ~NGN_ECC_WR_CHK;
  }

  if (NGN_ECC_EXIT_CORR_DEFAULT == NGN_ECC_EXIT_CORR_ENABLE) {
    Setup->mem.dfxMemSetup.dfxOptions |= NGN_ECC_EXIT_CORR;
  } else {
    Setup->mem.dfxMemSetup.dfxOptions &= ~NGN_ECC_EXIT_CORR;
  }

  if (NGN_ECC_RD_CHK_DEFAULT == NGN_ECC_RD_CHK_ENABLE) {
    Setup->mem.optionsNgn |= NGN_ECC_RD_CHK;
  } else {
    Setup->mem.optionsNgn &= ~NGN_ECC_RD_CHK;
  }

  if (NGN_ARS_PUBLISH_DEFAULT == NGN_ARS_PUBLISH_EN) {
    Setup->mem.optionsNgn |= NGN_ARS_PUBLISH;
  } else {
    Setup->mem.optionsNgn &= ~NGN_ARS_PUBLISH;
  }

  Setup->mem.dfxMemSetup.dfxOptions &= ~CR_SKIP_ARS_ON_BOOT;
  if (CR_SKIP_ARS_ON_BOOT_DEFAULT == CR_SKIP_ARS_ON_BOOT_EN) {
    Setup->mem.dfxMemSetup.dfxOptions |= CR_SKIP_ARS_ON_BOOT;
  }

  Setup->mem.dfxMemSetup.dfxOptions &= ~CR_SEAMLESS_OPT_IN;
  if (CR_SEAMLESS_OPT_IN_DEFAULT == CR_SEAMLESS_OPT_IN_EN) {
    Setup->mem.dfxMemSetup.dfxOptions |= CR_SEAMLESS_OPT_IN;
  }

  if (CR_BACKGROUND_ARS_DEFAULT == CR_BACKGROUND_ARS_DIS) {
    Setup->mem.dfxMemSetup.dfxOptions |= CR_NO_BACKGROUND_ARS;
  } else {
    Setup->mem.dfxMemSetup.dfxOptions &= ~CR_NO_BACKGROUND_ARS;
  }

  Setup->mem.setSecureEraseAllDIMMs = DISABLE_SECURE_ERASE;

  SetMem ((UINT8 *)&(Setup->mem.setSecureEraseSktCh), sizeof (Setup->mem.setSecureEraseSktCh), SECURE_ERASE_ALL_DEFAULT);

  Setup->mem.AppDirectMemoryHole = FixedPcdGetBool (PcdMrcPmemMemHoleDefault);

  //
  // Biased 2-way near memory cache setup defaults
  //
  Setup->mem.EnableTwoWayNmCache = TWO_WAY_NM_CACHE_DEFAULT;
  Setup->mem.NonPreferredWayMask = NON_PREFERRED_WAY_MASK_DEFAULT;
  Setup->mem.PreferredReadFirst  = PREFERRED_READ_FIRST_DEFAULT;

  //
  // Boot-time fast zero memory setup default
  //
  Setup->mem.FastZeroMemSupport = FAST_ZERO_MEM_DEFAULT;

  //
  // LSx implementation
  //
  Setup->mem.LsxImplementation = LSX_IMPLEMENTATION_SWSMI;

  //
  // Publish Mailbox structures in the NFIT
  //
  Setup->mem.NfitPublishMailboxStructsDisable = !NFIT_PUBLISH_MAILBOX_STRUCTS_DEFAULT;

  //
  // SMBIOS Extended Type 17 Structure Support
  //
  Setup->mem.ExtendedType17 = EXTENDED_TYPE_17_DEFAULT;

  //
  // NVMDIMM Factory Reset Clear
  //
  Setup->mem.FactoryResetClear = NGN_FACTORY_RESET_CLEAR_DEFAULT;

  //
  // Latch System Shutdown State is Enabled by default
  //
  Setup->mem.LatchSystemShutdownState = CR_LATCH_SYSTEM_SHUTDOWN_DEFAULT;

  //
  // Set default for RMT on Advanced Memory Training
  //
  Setup->mem.RmtOnAdvancedTraining = FixedPcdGet8 (PcdRmtAdvancedTraining);

  //
  // Enabling the BIOS SSA loader
  //
  Setup->mem.enableBiosSsaLoader = FixedPcdGet8 (PcdSsaLoaderEnable);

  //
  // Enabling the BIOS SSA Stitched Mode (RMT)
  //
  Setup->mem.enableBiosSsaRMT = FixedPcdGet8 (PcdSsaBuiltInRmtEnable);

  //
  // Enabling the BIOS SSA Stitched Mode (RMT) on FCB
  //
  Setup->mem.enableBiosSsaRMTonFCB = BIOS_SSA_RMT_FCB_DEFAULT;

  //
  // Enabling BiosSsaPerBitMargining
  //
  Setup->mem.biosSsaPerBitMargining = BIOS_SSA_PER_BIT_MARGINING_DEFAULT;

  //
  // Enabling BiosSsaDisplayTables
  //
  Setup->mem.biosSsaDisplayTables = BIOS_SSA_DISPLAY_TABLE_DEFAULT;

  //
  // Enabling BiosSsaPerDisplayPlots
  //
  Setup->mem.biosSsaPerDisplayPlots = BIOS_SSA_DISPLAY_PLOTS_DEFAULT;

  //
  // Enabling BiosSsaBacksideMargining;
  //
  Setup->mem.biosSsaBacksideMargining = BIOS_SSA_BACKSIDE_MARGINING_DEFAULT;

  //
  // Enabling BiosSsaEarlyReadIdMargining
  //
  Setup->mem.biosSsaEarlyReadIdMargining = BIOS_SSA_EARLY_READ_ID_DEFAULT;

  //
  // Enabling BiosSsaStepSizeOverride;
  //
  Setup->mem.biosSsaStepSizeOverride = BIOS_SSA_STEP_SIZE_OVERRIDE_DEFAULT;


  //
  // Enabling BiosSsaDebugMessages
  //
  Setup->mem.biosSsaDebugMessages = BIOS_SSA_RMT_DEBUG_MSGS_DEFAULT;

  //
  //Exponential loop count for single rank test
  //
  Setup->mem.biosSsaLoopCount = BIOS_SSA_LOOP_CNT_DEFAULT;

  //
  //  Step size of RxDqs
  //
  Setup->mem.biosSsaRxDqs = BIOS_SSA_RMT_DEFAULT_VALUE;
  //
  //  Step size of RxVref
  //
  Setup->mem.biosSsaRxVref = BIOS_SSA_RMT_DEFAULT_VALUE;
  //
  //  Step size of TxDq
  //
  Setup->mem.biosSsaTxDq = BIOS_SSA_RMT_DEFAULT_VALUE;
  //
  //  Step size of TxVref
  //
  Setup->mem.biosSsaTxVref = BIOS_SSA_RMT_DEFAULT_VALUE;
  //
  //  Step size of CmdAll
  //
  Setup->mem.biosSsaCmdAll = BIOS_SSA_RMT_DEFAULT_VALUE;
  //
  //  Step size of CmdVref
  //
  Setup->mem.biosSsaCmdVref = BIOS_SSA_RMT_DEFAULT_VALUE;
  //
  //  Step size of CtlVref
  //
  Setup->mem.biosSsaCtlAll = BIOS_SSA_RMT_DEFAULT_VALUE;
  //
  //  Step size of EridDelay
  //
  Setup->mem.biosSsaEridDelay = BIOS_SSA_RMT_DEFAULT_VALUE;
  //
  //  Step size of EridVref
  //
  Setup->mem.biosSsaEridVref = BIOS_SSA_RMT_DEFAULT_VALUE;


  // MemorySetup.hfr finishes

  Setup->mem.spareRanks = TWO_RANK_SPARE;

  Setup->mem.patrolScrubAddrMode |= PATROL_SCRUB_SPA;


  //
  //  options
  //    TEMPHIGH_EN
  //    PAGE_POLICY
  //    ALLOW2XREF_EN
  //    ADAPTIVE_PAGE_EN
  //    SCRAMBLE_EN
  //    NUMA_AWARE
  //    ECC_CHECK_EN
  //    DISABLE_WMM_OPP_RD
  //    DISABLE_ECC_SUPPORT
  //    CLTT_EN
  //    OLTT_EN
  //    RAS_TO_INDP_EN
  //    CLTT_TEST_EN
  //    MARGIN_RANKS_EN
  //

  if (PcdGetBool (PcdMrcXmpSupport)) {
    Setup->mem.inputMemTime.nRRD_L = 0;
  }

  //
  //    DRAM RAPL Init
  //
  Setup->mem.DramRaplEnable = DRAM_RAPL_DEFAULT;
  Setup->mem.dramraplbwlimittf = 1;
  Setup->mem.DramRaplExtendedRange = DRAM_RAPL_EXTENDED_RANGE_DEFAULT;
  Setup->mem.dramraplRefreshBase = DRAM_RAPL_REFRESH_BASE_DEFAULT;
  Setup->mem.CmsEnableDramPm = 0;

  // ECC-NonECC Mix dimms
  Setup->mem.options &= ~ECC_MIX_EN;

  // ECC Support
  Setup->mem.options &= ~DISABLE_ECC_SUPPORT;

  //
  // Advanced Memory Test
  //
  Setup->mem.AdvMemTestOptions = ADV_MEM_TEST_OPTIONS_DEFAULT;
  Setup->mem.AdvMemTestResetList = ADV_MEM_TEST_RESET_LIST_DEFAULT;
  //
  // Advanced Memory Test Conditions
  //
  Setup->mem.AdvMemTestCondition = ADV_MEM_TEST_COND_DEFAULT;
  Setup->mem.AdvMemTestCondVdd = ADV_MEM_TEST_COND_VDD_DEFAULT;
  Setup->mem.AdvMemTestCondTwr = ADV_MEM_TEST_COND_TWR_DEFAULT;
  Setup->mem.AdvMemTestCondTrefi = ADV_MEM_TEST_COND_TREFI_DEFAULT;
  Setup->mem.AdvMemTestCondPause = ADV_MEM_TEST_COND_PAUSE_DEFAULT;

  //
  // MDLL Shut Down Enable
  //

  if (MDLL_SDEN_DEFAULT == MDLL_SDEN_DIS) {
    Setup->mem.options &= ~MDLL_SHUT_DOWN_EN;
  } else {
    Setup->mem.options |= MDLL_SHUT_DOWN_EN;
  }

  //
  // High Temp Enable
  //
  if (HTE_DEFAULT) {
    Setup->mem.options |= TEMPHIGH_EN;
  } else {
    Setup->mem.options &= ~TEMPHIGH_EN;
  }

  //
  // Allow 2x Refresh
  //
  if (A2R_DEFAULT) {
    Setup->mem.options |= ALLOW2XREF_EN;
  } else {
    Setup->mem.options &= ~ALLOW2XREF_EN;
  }

  //
  // DRAM Rx Eq Training
  //
  if (DRAM_RX_EQ_DEFAULT == DRAM_RX_EQ_ENABLE) {
    Setup->mem.optionsExt |= DRAM_RX_EQ_EN;
  } else {
    Setup->mem.optionsExt &= ~DRAM_RX_EQ_EN;
  }

  //
  // DRAM Tx Echo Cancellation
  //
  Setup->mem.optionsExt &= ~TX_ECHO_CANCEL_EN;
  if (DRAM_TX_ECHO_CANCEL_DEFAULT == DRAM_TX_ECHO_CANCEL_ENABLE) {
    Setup->mem.optionsExt |= TX_ECHO_CANCEL_EN;
  }

  //
  // DRAM RxDfe Cancellation
  //
  Setup->mem.dfxMemSetup.DfxLegacyRxDfe = DRAM_LEGACY_RXDFE_DEFAULT;

  //
  // RX CTLE Extended Range
  //
  Setup->mem.dfxMemSetup.DfxRxCtleExtendedRange = RX_CTLE_EXTENDED_RANGE_DEFAULT;

  Setup->mem.dfxMemSetup.dfxPerMemMode = MEM_MODE_DEFAULT;
  Setup->mem.dfxMemSetup.DfxBankGroupInterleavingMode = BANK_GROUP_INTERLEAVING_DEFAULT;

  Setup->mem.dfxMemSetup.dfxCfgMask2LM = CFG_MASK_2LM_NORMAL;
  Setup->mem.dfxMemSetup.DfxDdrtChInterleaveGran = MEM_INT_GRAN_DDRT_DEFAULT;
  Setup->mem.dfxMemSetup.dfxDimmManagement = DIMM_MGMT_DEFAULT;
  Setup->mem.dfxMemSetup.DfxTmeCmiCplFabCreditWa = TME_CMI_CPL_FAB_CREDIT_WORKAROUND_DEFAULT;
  Setup->mem.dfxMemSetup.dfxLoadDimmMgmtDriver = LOAD_NGN_DIMM_MGMT_DRIVER_DEFAULT;
  Setup->mem.dfxMemSetup.DfxMcDataParityCheck = MC_DATA_PARITY_CHECK_DEFAULT;

  Setup->mem.dfxMemSetup.DfxDdrtSchedulerDebug = DDRT_SCHEDULER_DBG_DISABLE;
  if (FixedPcdGetBool (PcdDdrtSchedulerDebugDefault)) {
    Setup->mem.dfxMemSetup.DfxDdrtSchedulerDebug = DDRT_SCHEDULER_DBG_ENABLE;
  }

  Setup->mem.dfxMemSetup.DfxCrAcpiDebugInterface = CR_ACPI_DEBUG_INTERFACE_DEFAULT;
  Setup->mem.dfxMemSetup.dfxPartitionDDRTDimm = 0;

  SetMem (&Setup->mem.dfxMemSetup.dfxPartitionRatio, sizeof (Setup->mem.dfxMemSetup.dfxPartitionRatio), RATIO_DEFAULT);

  Setup->mem.dfxMemSetup.DfxInputRt = INPUT_RT_DEFAULT;
  Setup->mem.dfxMemSetup.DfxInputIoLatency = INPUT_IOLAT_DEFAULT;
  Setup->mem.dfxMemSetup.DfxInputIoLatencyIoComp = INPUT_IOLATIOCOMP_DEFAULT;
  Setup->mem.dfxMemSetup.DfxInputTxDqDelay = INPUT_TXDQ_DEFAULT;
  Setup->mem.dfxMemSetup.DfxInputTxDqsDelay = INPUT_TXDQS_DEFAULT;
  Setup->mem.dfxMemSetup.DfxInputTxDqBitDelay =  INPUT_TXDQBIT_DEFAULT;
  Setup->mem.dfxMemSetup.DfxInputRxDqDelay = INPUT_RXDQ_DEFAULT;
  Setup->mem.dfxMemSetup.DfxInputRxDqsDelay = INPUT_RXDQS_DEFAULT;
  Setup->mem.dfxMemSetup.DfxInputRxDqBitDelay = INPUT_RXDQBIT_DEFAULT;
  Setup->mem.dfxMemSetup.DfxInputGnt2Erid = INPUT_GNT2ERID_DEFAULT;
  Setup->mem.dfxMemSetup.DfxCwlAdjPos = INPUT_CWLP_DEFAULT;
  Setup->mem.dfxMemSetup.DfxCwlAdjNeg = INPUT_CWLN_DEFAULT;
  Setup->mem.dfxMemSetup.DfxRcvEnDelay = RCV_EN_DEL_DEFAULT;
  Setup->mem.dfxMemSetup.DfxDdrtRcvEnDelay = DDRT_RCV_EN_DEL_DEFAULT;
  Setup->mem.dfxMemSetup.DfxDdrtRt = INPUT_DDRT_RT_DEFAULT;

  Setup->mem.dfxMemSetup.DfxInputCmdDelay = INPUT_CMD_DELAY_DEFAULT;

  Setup->mem.dfxMemSetup.DfxUbiosSmbusTraceEnable = UBIOS_SMBUS_TRACE_DEFAULT;
  Setup->mem.dfxMemSetup.DfxSmbusI3cModeEnable = SMBUS_I3C_MODE_DEFAULT;

  //
  // Early completion setup option defaults
  //
  Setup->mem.dfxMemSetup.DfxDdrEarlyCmpEn = DDR_EARLY_CMP_DEFAULT;
  Setup->mem.dfxMemSetup.DfxHbmEarlyCmpEn = HBM_EARLY_CMP_DEFAULT;

  //
  // DDR XOR deoder setup option defaults
  //
  Setup->mem.dfxMemSetup.DfxClusterXorEn = CLUSTER_XOR_DEFAULT;
  Setup->mem.dfxMemSetup.DfxChannelXorEn = CHANNEL_XOR_DEFAULT;
  Setup->mem.dfxMemSetup.DfxPseudoChannelXorEn = PSEUDO_CHANNEL_XOR_DEFAULT;
  Setup->mem.dfxMemSetup.DfxDimmXorEn    = DIMM_XOR_DEFAULT;
  Setup->mem.dfxMemSetup.DfxRankXorEn    = RANK_XOR_DEFAULT;
  Setup->mem.dfxMemSetup.DfxSidXorEn     = SID_XOR_DEFAULT;

  if (BANK_XOR_DEFAULT == BANK_XOR_ENABLED) {
    Setup->mem.dfxMemSetup.DfxBankXorEn    = BANK_XOR_ENABLED;
  } else {
    Setup->mem.dfxMemSetup.DfxBankXorEn    = BANK_XOR_DISABLED;
  }
  //
  // Training Comp Options Values
  //
  Setup->mem.TrainingCompOptions = TRAINING_COMP_DEFAULT;

  //
  // Periodic Rcomp.
  //
  Setup->mem.PeriodicRcomp = PERIODIC_RCOMP_DEFAULT;
  Setup->mem.PeriodicRcompInterval = MAX_PERIODIC_RCOMP_INTERVAL;

  //
  // LRDIMM Backside Vref Training
  //
  if (LRDIMM_BACKSIDE_VREF_DEFAULT == LRDIMM_BACKSIDE_VREF_ENABLE) {
    Setup->mem.optionsExt |= LRDIMM_BACKSIDE_VREF_EN;
  } else {
    Setup->mem.optionsExt &= ~LRDIMM_BACKSIDE_VREF_EN;
  }

  //
  // Per bit deskew
  //
  Setup->mem.perBitDeSkew = BIT_DESKEW_DEFAULT;


  //
  // High Address Enable: default to disabled
  //
  Setup->mem.dfxMemSetup.dfxOptions &= ~HIGH_ADDR_EN;

  //
  //  HighAddrBitStart
  //    Address bit to start mapping memory above 4GB
  //
  Setup->mem.dfxMemSetup.dfxHighAddrBitStart = 0;

  //
  //  dfxStraddleHighAddressBit
  //  Enable/Disable straddle memory above 4GB
  //
  Setup->mem.dfxMemSetup.dfxStraddleHighAddressBit = 0;

  //
  //  lowMemChannel
  //    Channel to use for low memory, if chInterleveave = 1
  //
  Setup->mem.dfxMemSetup.dfxLowMemChannel = 0;

  //
  //  spareErrTh
  //
  Setup->mem.spareErrTh = 0x7FFF;
  //
  // NGN Die Sparing status and Aggressiveness
  //
  Setup->mem.DieSparing = NGN_DIE_SPARING_EN;

  //
  // Address range scrub Setup, disabled by default
  //
  Setup->mem.NgnAddressRangeScrub = NGN_ADDRESS_RANGE_SCRUB_DIS;

  //Crystal Ridge 2LM: Eliminate directory in Far Memory (FM)
  Setup->mem.EliminateDirectoryInFarMemory = ELIMINATE_DIRECTORY_IN_FM_DISABLE;

  Setup->mem.FastGoConfig = FixedPcdGet8 (PcdMrcCrQosConfigDefault);

  //
  // NVM QOS
  //
  Setup->mem.NvmQos = NVM_QOS_DEFAULT;

  //
  // Disable Directory for AD
  //
  Setup->mem.DisableDirForAppDirect = DIRECTORY_FOR_APP_DIRECT_DISABLE;

  //
  //  patrolScrubDuration
  //  Patrol Scrub duration defaults to 24 hours
  //
  Setup->mem.patrolScrubDuration = PATROL_SCRUB_DURATION_DEFAULT;

  //
  // Leaky bucket values (1 leak per 10 days for 1GHz DCLK)
  //
  Setup->mem.leakyBktTimeWindow = LEAKY_BUCKET_TIME_WINDOW_DISABLED;
  Setup->mem.leakyBktLo = LEAKY_BUCKET_LO;
  Setup->mem.leakyBktHi = LEAKY_BUCKET_HI;
  Setup->mem.leakyBktHour = LEAKY_BUCKET_HOUR;
  Setup->mem.leakyBktMinute = LEAKY_BUCKET_MINUTE;


  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    //
    // Enable the memory controller
    //
    Setup->mem.socket[Socket].enabled = 1;

    //
    // Set options
    //
    Setup->mem.socket[Socket].options = 0;

    for (Channel = 0; Channel < MAX_CH; Channel++) {
      //
      // Enable this channel
      //
      Setup->mem.socket[Socket].ddrCh[Channel].enabled = 1;
      //
      // MAX DIMM slots available per channel
      //
      Setup->mem.socket[Socket].ddrCh[Channel].numDimmSlots = MAX_DIMM;
    } // Channel loop

  } // socket loop

  if (HybridSystemLevelEmulationEnabled ()) {
    Setup->mem.DramRaplEnable = DRAM_RAPL_DISABLE;
    Setup->mem.thermalThrottlingOptions = 0;
    Setup->mem.options &= ~MEMORY_TEST_EN;
    Setup->mem.options &= ~MDLL_SHUT_DOWN_EN;
    Setup->mem.options &= ~SCRAMBLE_EN;
    Setup->mem.options &= ~MULTI_THREAD_MRC_EN;
  }

  //
  // pkgc self refresh enabled by default
  //
  Setup->mem.PkgcSrefEn = 1;

  // NVMDIMM transactor
  Setup->common.ddrtXactor = DDRT_XACTOR_DEFAULT;

  //
  // IMC BCLK
  //
  Setup->mem.imcBclk = IMC_BCLK_AUTO;

  //
  // SPD CRC Check
  //
  if (SPD_CRC_CHECK_DEFAULT == SPD_CRC_CHECK_EN) {
    Setup->mem.optionsExt |= SPD_CRC_CHECK;
  } else {
    Setup->mem.optionsExt &= ~SPD_CRC_CHECK;
  }

  //
  // 2GB short stroke configuration
  //

  Setup->mem.dfxMemSetup.dfxOptions &= ~(SHORT_STROKE_INT | SHORT_STROKE_NO_INT);

  if (PcdGetBool (PcdNvDimmEn)) {
    Setup->mem.ADREn = 1;
    Setup->mem.LegacyADRModeEn = FixedPcdGet8 (PcdLegacyAdrDefault);
    Setup->mem.MinNormalMemSize = 2;
    Setup->mem.ADRDataSaveMode = ADR_NVDIMM;
    Setup->mem.interNVDIMMS = 0;
    Setup->mem.eraseArmNVDIMMS = 1;
    Setup->mem.restoreNVDIMMS = 1;
  }
  Setup->mem.NvDimmEnergyPolicy = ENERGY_POLICY_DEFAULT;

  //
  // Cmd Setup hold offset
  //
  Setup->mem.cmdSetupPercentOffset = CMD_SETUP_PERCENT_DEFAULT;

  //
  // Posted CSR accesses
  //
  if (PcdGetBool (PcdPostedCsrAccessSupported) && !IsSiliconWorkaroundEnabled ("S2207989002")) {
    Setup->mem.dfxMemSetup.DfxPostedCsrAccessAllowed = PcdGetBool (PcdPostedCsrAccessAllowedDefault);
  } else {
    Setup->mem.dfxMemSetup.DfxPostedCsrAccessAllowed = FALSE;
  }

  //
  // CPGC register cache accesses
  //
  if (PcdGetBool (PcdCpgcRegCacheAccessSupported)) {
    Setup->mem.dfxMemSetup.DfxCpgcRegCacheAccessAllowed = PcdGetBool (PcdCpgcRegCacheAccessAllowedDefault);
  } else {
    Setup->mem.dfxMemSetup.DfxCpgcRegCacheAccessAllowed = FALSE;
  }

  //
  // Synchronized CPGC test start
  //
  if (PcdGetBool (PcdCpgcGlobalSyncCtrlSupported)) {
    Setup->mem.dfxMemSetup.DfxCpgcGlobalSyncCtrlEnabled = PcdGetBool (PcdCpgcGlobalSyncCtrlEnableDefault);
  } else {
    Setup->mem.dfxMemSetup.DfxCpgcGlobalSyncCtrlEnabled = FALSE;
  }

  //
  // SSA RMT Receive Enable Denormalize Default
  //
  Setup->mem.dfxMemSetup.DfxSsaRmtRecvEnableDenormalize = PcdGetBool (PcdDfxSsaRmtRecvEnableDenormalize);

  //
  // DFX Periodic ZQCal default
  //
  Setup->mem.dfxMemSetup.DfxPeriodicZqCal = PcdGetBool (PcdDfxPeriodicZqCalDefault);

  //
  // DFX Sense Amp Calibration Algorithm selection default
  // Use the SW based algorithm if the HW FSM algorithm is not supported
  //
  if (PcdGetBool (PcdSenseAmpCalibHwFsmSupported)) {
    Setup->mem.dfxMemSetup.SenseAmpCalibHwSwOption = FixedPcdGet8 (PcdDfxSenseAmpCalibHwSwDefault);
  } else {
    Setup->mem.dfxMemSetup.SenseAmpCalibHwSwOption = DFX_SENSE_AMP_CALIB_SW;
  }

  //
  // DFE SW Xover selection
  //
  Setup->mem.dfxMemSetup.DfxSwXoverOption = FixedPcdGet8 (PcdDfxXoverSwCalibParallelMode);

  //
  // Always use SMBUS to send MRW commands before QCA training completes.
  // This policy exists to allow emulation overrides.
  //
  Setup->mem.UseSmbusForMrwEarly = TRUE;

  //
  //Get margin check data from PCD default
  //
  Setup->mem.MemBootHealthConfig.MemBootHealthCheck   = FixedPcdGet8 (PcdMemBootHealthCheck);   // 0=>Auto, 1=>Manual and 2=>Disabled Memory Boot Health Check
  Setup->mem.MemBootHealthConfig.ResetOnCriticalError = FixedPcdGet8 (PcdResetOnCriticalError); // 0 => Dont reboot on critical error, 1 = Reboot on critical error

  //
  // Memory Boot Health check parameters
  //
  Setup->mem.MemBootHealthConfig.WarningTxDqDelayLeftEdge   = FixedPcdGet8 (PcdWarningTxDqDelayLeftEdge);
  Setup->mem.MemBootHealthConfig.WarningTxDqDelayRightEdge  = FixedPcdGet8 (PcdWarningTxDqDelayRightEdge);
  Setup->mem.MemBootHealthConfig.WarningTxVrefLeftEdge      = FixedPcdGet8 (PcdWarningTxVrefLeftEdge);
  Setup->mem.MemBootHealthConfig.WarningTxVrefRightEdge     = FixedPcdGet8 (PcdWarningTxVrefRightEdge);
  Setup->mem.MemBootHealthConfig.WarningRxDqsDelayLeftEdge  = FixedPcdGet8 (PcdWarningRxDqsDelayLeftEdge);
  Setup->mem.MemBootHealthConfig.WarningRxDqsDelayRightEdge = FixedPcdGet8 (PcdWarningRxDqsDelayRightEdge);
  Setup->mem.MemBootHealthConfig.WarningRxVrefLeftEdge      = FixedPcdGet8 (PcdWarningRxVrefLeftEdge);
  Setup->mem.MemBootHealthConfig.WarningRxVrefRightEdge     = FixedPcdGet8 (PcdWarningRxVrefRightEdge);

  Setup->mem.MemBootHealthConfig.CriticalTxDqDelayLeftEdge   = FixedPcdGet8 (PcdCriticalTxDqDelayLeftEdge);
  Setup->mem.MemBootHealthConfig.CriticalTxDqDelayRightEdge  = FixedPcdGet8 (PcdCriticalTxDqDelayRightEdge);
  Setup->mem.MemBootHealthConfig.CriticalTxVrefLeftEdge      = FixedPcdGet8 (PcdCriticalTxVrefLeftEdge);
  Setup->mem.MemBootHealthConfig.CriticalTxVrefRightEdge     = FixedPcdGet8 (PcdCriticalTxVrefRightEdge);
  Setup->mem.MemBootHealthConfig.CriticalRxDqsDelayLeftEdge  = FixedPcdGet8 (PcdCriticalRxDqsDelayLeftEdge);
  Setup->mem.MemBootHealthConfig.CriticalRxDqsDelayRightEdge = FixedPcdGet8 (PcdCriticalRxDqsDelayRightEdge);
  Setup->mem.MemBootHealthConfig.CriticalRxVrefLeftEdge      = FixedPcdGet8 (PcdCriticalRxVrefLeftEdge);
  Setup->mem.MemBootHealthConfig.CriticalRxVrefRightEdge     = FixedPcdGet8 (PcdCriticalRxVrefRightEdge);

  //
  // DDR5 Refresh Mode
  //
  Setup->mem.dfxMemSetup.DfxDdr5RefreshMode = FixedPcdGet8 (PcdDdr5RefreshModeDefault);

  //
  //  DDR5 SSA RxSampler Margin Parameter Selection
  //
  Setup->mem.dfxMemSetup.DfxDdr5SsaRxSamplerSelection = FixedPcdGet8 (PcdDdr5SsaRxSamplerSelectionDefault);

  return EFI_SUCCESS;

} // SetMemoryPolicyDefaults

/**
  Install the memory policy.

  Allocates memory for the policy, installs the policy, and
  sets the static pointer table pointer to the policy.

  @param[in]  PolicyPpi          A pointer to PPI to fill in.

  @retval EFI_SUCCESS             Success
  @retval EFI_INVALID_PARAMETER   Failure

**/

EFI_STATUS
EFIAPI
InstallMemoryPolicyPpi (
  IN  MEMORY_POLICY_PPI *PolicyPpi
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_PPI_DESCRIPTOR      *PpiDescriptor = NULL;

  PERF_START (NULL, "InstallMemoryPolicyPpi", "PEI", 0);
  Status = EFI_SUCCESS;

  PpiDescriptor = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (PpiDescriptor == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  PpiDescriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PpiDescriptor->Guid  = &gMemoryPolicyPpiGuid;
  PpiDescriptor->Ppi   = PolicyPpi;

  DEBUG ((DEBUG_INFO, "Installing Memory Policy PPI \n"));

  Status = PeiServicesInstallPpi (PpiDescriptor);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PeiServicesInstallPpi Error = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Now that we have installed the policy, set the pointer
  // in the static pointer table.
  //

  SetSysSetupPointer (PolicyPpi->SysSetup);

  //
  // Comment this out normally to save code space and duplicate prints in serial log.
  // Retaining function to allow easy debug of policy code if needed
  //
  //DumpMemorySetupPolicy (PolicyPpi);
  PERF_END (NULL, "InstallMemoryPolicyPpi", "PEI", 0);
  return Status;

} // InstallMemoryPolicyPpi
